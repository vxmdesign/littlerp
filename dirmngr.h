#ifndef DIRMNGR_H
#define DIRMNGR_H

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <vector>

#define BASE_DIR "/models/"

using namespace std;

typedef struct{ 
  char path[256];
  char name[16];
  int valid;
}dentry;

class dirmngr{
 public:
  dirmngr();
  int entries();
  dentry getEntry(int i);
 private:
  int checkFile(const char *p);
  void validate(dentry *d);
  vector<dentry> dl;
};

#endif
