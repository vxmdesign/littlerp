#ifndef LCD_H
#define LCD_H

#include <stdio.h>
#include "i2cbusses.h"
#include "i2c-dev.h"

#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

// Entry flags 
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Control flags 
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00


// Move flags
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

// Function set flags   
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

#define LCD_PLATE_RS             0x8000
#define LCD_PLATE_RW             0x4000
#define LCD_PLATE_EN             0x2000
#define LCD_PLATE_D4             0x1000
#define LCD_PLATE_D5             0x0800
#define LCD_PLATE_D6             0x0400
#define LCD_PLATE_D7             0x0200
#define LCD_PLATE_BLUE           0x0100
#define LCD_PLATE_GREEN          0x0080
#define LCD_PLATE_RED            0x0040


#define IODIR_REG 0x00
#define IODIR_VAL 0x003F
#define GPPU_REG 0x0C
#define GPPU_VAL 0x001F
#define GPIO_REG 0x12
#define OLAT_REG 0x14
#define COLOR_MASK (LCD_PLATE_BLUE | LCD_PLATE_GREEN | LCD_PLATE_RED)





class lcd{
 public:
  lcd(int pBus);
  void setColor(int pColor);
  void home();
  void clear();
  void set_cursor(int pCol, int pRow);
  void enable_display(int pEnable);
  void show_cursor(int pShow);
  void blink(int pBlink);
  void move_left();
  void move_right();
  void set_left_to_right();
  void set_right_to_left();
  void autoscroll(int pScroll);
  void message(const char *pMesg);
  void message_fixed(const char *pMesg, int pRow);
  int get_button(int timeout);
 private:
  void initLcd();
  void write8(unsigned char pChar, int pMode);
  void pulse();
  int mFile;
  unsigned short mOpt;  
  unsigned char mDisplayControl;
  unsigned char mDisplayMode;
};


#endif
