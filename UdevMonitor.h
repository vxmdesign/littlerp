#ifndef UDEV_MONITOR_H
#define UDEV_MONITOR_H


#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct{
  char device[64];
  char name[64];
}deventry;

class UdevMonitor{
 public:
  UdevMonitor();
  ~UdevMonitor();
  int getDiskDevice(deventry *pDev);
  int getDiskDevicePoll(deventry *pDev);
 private:
  int checkCurrent(deventry *pDev);
  int monitor(deventry *pDev);
  struct udev *muDev;
};


#endif
