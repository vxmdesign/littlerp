#include "lcd.h"

const unsigned char LCD_ROW_OFFSETS[4] = { 0x0 , 0x40 , 0x14, 0x54};

lcd::lcd(int pBus){
  char filename[20];  
  mFile = open_i2c_dev(pBus, filename, sizeof(filename),0);
  set_slave_addr(mFile,0x20,0);
  initLcd();
  setColor(1);
}

void lcd::setColor(int pColor){
  mOpt = mOpt | COLOR_MASK;
  if(pColor == 0){
    mOpt &= ~(LCD_PLATE_RED);
  }else if(pColor == 1){
    mOpt &= ~(LCD_PLATE_GREEN);
  }else if(pColor == 2){
    mOpt &= ~(LCD_PLATE_BLUE);
  }
  i2c_smbus_write_word_data(mFile, OLAT_REG, mOpt);
}

void lcd::initLcd(){
  unsigned char t;
  mOpt = COLOR_MASK;
  i2c_smbus_write_word_data(mFile, OLAT_REG, mOpt);
  i2c_smbus_write_word_data(mFile, IODIR_REG, IODIR_VAL);
  i2c_smbus_write_word_data(mFile, GPPU_REG, GPPU_VAL);
  usleep(100);
  write8(0x33, 0);
  write8(0x32, 0);
  t = LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  mDisplayControl = t;
  write8(t, 0);
  t = LCD_FUNCTIONSET | LCD_4BITMODE | LCD_1LINE | LCD_2LINE | LCD_5x8DOTS;
  write8(t, 0);
  t = LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  mDisplayMode = t;
  write8(t, 0);
  t = LCD_CLEARDISPLAY;
  write8(t,0);
  usleep(3000);
}

void lcd::write8(unsigned char pChar, int pMode){
  unsigned short tmp;
  unsigned short d;
  unsigned short bk;
  usleep(1000);
  bk = mOpt;
  if(pMode != 0){
    mOpt |= LCD_PLATE_RS;
  }
  d = (pChar & 0x80)?LCD_PLATE_D7:0;
  d |= (pChar & 0x40)?LCD_PLATE_D6:0;
  d |= (pChar & 0x20)?LCD_PLATE_D5:0;
  d |= (pChar & 0x10)?LCD_PLATE_D4:0;
  mOpt |= d;
  i2c_smbus_write_word_data(mFile, OLAT_REG, mOpt);
  pulse();
  mOpt = bk;
  if(pMode != 0){
    mOpt |= LCD_PLATE_RS;
  }
  d = (pChar & 0x08)?LCD_PLATE_D7:0;
  d |= (pChar & 0x04)?LCD_PLATE_D6:0;
  d |= (pChar & 0x02)?LCD_PLATE_D5:0;
  d |= (pChar & 0x01)?LCD_PLATE_D4:0;
  mOpt |= d;
  i2c_smbus_write_word_data(mFile, OLAT_REG, mOpt);
  pulse();
  mOpt = bk;
  i2c_smbus_write_word_data(mFile, OLAT_REG, mOpt);
}

void lcd::pulse(){
  unsigned short tmp; 
  tmp = mOpt | LCD_PLATE_EN;
  i2c_smbus_write_word_data(mFile, OLAT_REG, mOpt);
  usleep(1);
  i2c_smbus_write_word_data(mFile, OLAT_REG, tmp);
  usleep(1);
  i2c_smbus_write_word_data(mFile, OLAT_REG, mOpt);  
  usleep(1);  
}

void lcd::home(){
  write8(LCD_RETURNHOME,0);
  usleep(3000);
}

void lcd::clear(){
  write8(LCD_CLEARDISPLAY,0);
  usleep(3000);
}

void lcd::set_cursor(int pCol, int pRow){
  if(pRow > 1){
    pRow = 1;
  }
  write8(LCD_SETDDRAMADDR | (pCol + LCD_ROW_OFFSETS[pRow]), 0);  
}

void lcd::enable_display(int pEnable){
  if(pEnable != 0){
    mDisplayControl |= LCD_DISPLAYON;
  }else{
    mDisplayControl &= ~LCD_DISPLAYON;
  }
  write8(mDisplayControl, 0);
}

void lcd::show_cursor(int pShow){
  if(pShow != 0){
    mDisplayControl |= LCD_CURSORON;
  }else{
    mDisplayControl &= ~LCD_CURSORON;
  }
  write8(mDisplayControl, 0);
}

void lcd::blink(int pBlink){
  if(pBlink != 0){
    mDisplayControl |= LCD_BLINKON;
  }else{
    mDisplayControl &= ~LCD_BLINKON;
  }
  write8(mDisplayControl, 0);
}

void lcd::move_left(){
  write8(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT,0);
}

void lcd::move_right(){
  write8(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT,0);  
}

void lcd::set_left_to_right(){
  mDisplayMode |= LCD_ENTRYLEFT;
  write8(mDisplayMode, 0);
}

void lcd::set_right_to_left(){
  mDisplayMode &= ~LCD_ENTRYLEFT;
  write8(mDisplayMode, 0);
}

void lcd::autoscroll(int pScroll){
  if(pScroll != 0){
    mDisplayMode |= LCD_ENTRYSHIFTINCREMENT;
  }else{
    mDisplayMode &= ~LCD_ENTRYSHIFTINCREMENT;
  }
  write8(mDisplayMode, 0);
}

void lcd::message(const char *pMesg){
  int c;
  c = 0; 
  while(c < 16 && pMesg[c] != '\0'){
    write8(pMesg[c],1);
    c++;
  }  
}

void lcd::message_fixed(const char *pMesg, int pRow){
  int c;
  int l;
  c = 0;
  l = 0;
  set_cursor(0,pRow);
  while(c < 16){
    if(pMesg[l] == '\0'){
      write8(' ',1);
    }else{
      write8(pMesg[l],1);
      l++;
    }
    c++;
  }
}

int lcd::get_button(int timeout){
  int r;
  do{
    r = i2c_smbus_read_word_data(mFile, GPIO_REG);  
    r = r & GPPU_VAL;
    timeout--;
    usleep(100000);
  }while(timeout != 0 && r == GPPU_VAL);  
  r = (~r) & 0x1f;  
  return r;
}
