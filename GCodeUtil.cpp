#include "GCodeUtil.h"

int trimComment(char *x){
  int c;
  c = 0;
  while(c < 255){
    if(x[c] == '\0'){
      return -1;
    }
    if(x[c] == ';'){
      x[c] = '\0';
      return c;
    }
    c++;
  }
  return -1;
}

// " \r\n\b\t";

int iswhitespace(char c){
  switch(c){
  case ' ':
  case '\r':
  case '\b':
  case '\n':
  case '\t':
  case '\0':
    return 1;
  }
  return 0;
}

char *trim(char *x){
  int l;
  if(x == NULL){
    return NULL;
  }
  l = strlen(x);
  while(l >= 0 && (iswhitespace(x[l])==1)){
    l--;    
  }
  if(l < 0){
    return NULL;
  }
  x[l+1] = '\0';
  l = 0;
  while(iswhitespace(x[l])==1){
    l++;
  }
  return &(x[l]);  
}

void strcpybase(char *dst, char *src, char delim){
  int idx;
  idx = strlen(src)-1;
  while(idx >= 0 && src[idx] != delim){
    idx--;
  }
  dst[idx] = '\0';
  idx--;
  while(idx >= 0){
    dst[idx] = src[idx];
    idx--;
  }
}
