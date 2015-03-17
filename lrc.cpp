#include "lrc.h"

dentry lrc_menu(lcd *l, dirmngr *dm){
  int r;
  int i;
  dentry x;
  l->clear();
  x = dm->getEntry(0);
  r = 0;
  l->message_fixed(x.name, 0);
  if(x.valid == 1){
    l->setColor(1);
  }else{
    l->setColor(0);
  }

  while(r != 0x1){
    r = l->get_button(-1);
    if(r == 0x4){
      i++;
      if(i >= dm->entries()){
	i = 0;
      }
    }else if(r == 0x8){
      i--;
      if(i < 0){
	i = dm->entries()-1;
      }
    }
    x = dm->getEntry(i);
    l->message_fixed(x.name,0);
    if(x.valid == 1){
      l->setColor(1);
    }else{
      l->setColor(0);
    }
  }
  return x;
}


