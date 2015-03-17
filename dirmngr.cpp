#include "dirmngr.h"



int dirmngr::checkFile(const char *p){
  int r;
  r = strlen(p);
  r = r - 6;
  if(r < 1){
    return 0;
  }
  if(strcmp(&(p[r]), ".gcode")==0){
    return 1;
  }
  return 0;
}

void dirmngr::validate(dentry *d){
  DIR *dp;
  int c;
  struct dirent *ep;
  dp = opendir(d->path);
  d->valid = -1;
  if(dp != NULL){
    while( (ep = readdir(dp)) != NULL){
      if(ep->d_type == DT_REG){
	if(checkFile(ep->d_name) == 1){
	  c = strlen(d->path);
	  sprintf(&(d->path[c]), "%s", ep->d_name);
	  d->valid = 1;
	  closedir(dp);
	  return;
	}
      }
    }
    closedir(dp);
  }
}

int dirmngr::entries(){
  return dl.size();
}

dentry dirmngr::getEntry(int i){
  return dl[i];
}

dirmngr::dirmngr(){
  DIR *dp;
  unsigned int c;
  dentry d;
  struct dirent *ep;
  dp = opendir(BASE_DIR);
  
  if(dp != NULL){
    while( (ep = readdir(dp)) != NULL){
      if(ep->d_type == DT_DIR && ep->d_name[0] != '.'){
	d.path[0] = '\0';
	d.valid = 0;       
	sprintf(d.path, "%s%s/", BASE_DIR, ep->d_name);
	snprintf(d.name, 17, "%s", ep->d_name);
	validate(&d);
	dl.push_back(d);
      }
    }
    closedir(dp);
  }

  for(c = 0; c < dl.size(); c++){
    if(dl[c].valid == 1){
      printf("Dir: *%s*\n", dl[c].path);
    }else{
      printf("Dir: %s\n", dl[c].path);
    }
  }


}
