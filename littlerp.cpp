#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include "GCodeUtil.h"
#include "SliceMngr.h"
#include "lcd.h"
#include "dirmngr.h"
#include "lrc.h"

int process(char *x){
  int c;
  if(x == NULL){
    return -1;
  }
  if(strncmp(x, "<Delay>", 7) == 0){ //HACK FOR NOW
    return 2;
  }
  if(x[0] != ';'){
    return 0;
  }
  if(strncmp(x, ";<Slice>", 8) == 0){
    return 1;
  }else if(strncmp(x, ";<Delay>", 8) == 0){
    return 2;
  }
  c=0;
  while(iswhitespace(x[c]) == 1){
    if(x[c] == '\0' || c >= 255){
      return -1;
    }   
    c++;
  }
  return 0;
}

void delayCmd(char *cmd){
  char *arg;
  int i;
  if(cmd[0] == ';'){
    arg = trim(&(cmd[8]));
  }else{
    arg = trim(&(cmd[7]));
  }
  trimComment(arg);
  i = atoi(arg);
  printf("Delay %d\n", i);
  usleep(i * 1000);
}

void sliceCmd(SliceMngr *sm, char *base, char *cmd){
  char *arg;
  char pngname[255];
  int i;
  arg = trim(&(cmd[8]));
  trimComment(arg);
  if(strcmp("Blank", arg) == 0){
    sm->blankScreen();
    printf("Blank Screen\n");
    return;
  }
  i = atoi(arg);
  sprintf(pngname, "%s%04d.png", base,i);
  printf("%s\n", pngname);
  sm->loadSlice(pngname);
  sm->sliceScreen();
}



int main(int argc, char **argv){
  FILE *fp;
  int mt;
  char cmd[255];
  char base[255];
  char wrcmd[255];
  char *tmp;
  lcd *l;
  dirmngr *dm;
  dentry slfile;
  struct termios orig;
  struct termios raw;
  SliceMngr *sm;
  l = new lcd(1);
  dm = new dirmngr();
  slfile = lrc_menu(l,dm);

  if(slfile.valid == 1){
    l->setColor(2);
    printf("gcode: %s\n", slfile.path);
  }else{
    exit(1);
  }
  strcpy(slfile.path, "/models/huntress_final.slice/huntress_final.gcode");
  strcpybase(base,slfile.path, '.');
  fp = fopen(slfile.path, "r");

  mt = open(argv[1], O_RDWR);
  if(!isatty(mt)){
    printf("not a tty\n");
    return -1;
  }
  if(tcgetattr(mt, &orig) < 0){
    printf("Could not get attr\n");
    return -1;
  }
  raw = orig;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

  /* output modes - clear giving: no post processing such as NL to CR+NL */
  raw.c_oflag &= ~(OPOST);

  /* control modes - set 8 bit chars */
  raw.c_cflag |= (CS8);

  /* local modes - clear giving: echoing off, canonical off (no erase with      
     backspace, ^U,...),  no extended functions, no signal chars (^Z,^C) */
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  /* control chars - set return condition: min number of bytes and timer */
  raw.c_cc[VMIN] = 5; raw.c_cc[VTIME] = 8; /* after 5 bytes or .8 seconds       
                                              after first byte seen      */
  raw.c_cc[VMIN] = 0; raw.c_cc[VTIME] = 0; /* immediate - anything       */
  raw.c_cc[VMIN] = 2; raw.c_cc[VTIME] = 0; /* after two bytes, no timer  */
  raw.c_cc[VMIN] = 0; raw.c_cc[VTIME] = 8; /* after a byte or .8 seconds */

  cfsetspeed(&raw, B9600);
  if(tcsetattr(mt, TCSAFLUSH, &raw) < 0){
    printf("Could not set speed\n");
    return -1;
  }

  sm = new SliceMngr();
  while(fgets(cmd, 255, fp) != NULL){
    //trimComment(cmd);
    tmp = trim(cmd);
    switch(process(tmp)){
    case 0:
      if( trimComment(tmp) != 0){      
	tmp = trim(tmp);
	sprintf(wrcmd, "%s\r\n", tmp);
	if(write(mt, wrcmd, strlen(wrcmd))!=strlen(wrcmd)){
	  printf("write did not complete\n");
	}
	printf("Motor cmd: #%s#, %d\n", tmp, strlen(tmp));
      }
      break;
    case 1:
      sliceCmd(sm, base, tmp);
      break;
    case 2:
      delayCmd(tmp);
      break;
    }
  }
  tcsetattr(mt, TCSAFLUSH, &orig);
  close(mt);
  delete sm;
  fclose(fp);
}
