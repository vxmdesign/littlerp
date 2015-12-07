#include "UdevMonitor.h"

UdevMonitor::UdevMonitor(){
  muDev = udev_new();
}

UdevMonitor::~UdevMonitor(){
  if(muDev != NULL){
    udev_unref(muDev);
  }
}

int UdevMonitor::checkCurrent(deventry *pDev){
  struct udev_enumerate *enumerate;
  struct udev_list_entry *devices, *dev_list_entry;
  const char *path;
  struct udev_device *dev;
  struct udev_device *par;
  enumerate = udev_enumerate_new(muDev);
  udev_enumerate_add_match_subsystem(enumerate, "block");
  udev_enumerate_scan_devices(enumerate);
  devices = udev_enumerate_get_list_entry(enumerate);

  udev_list_entry_foreach(dev_list_entry, devices) {
    const char *path;

    /* Get the filename of the /sys entry for the device
       and create a udev_device object (dev) representing it */
    path = udev_list_entry_get_name(dev_list_entry);
    dev = udev_device_new_from_syspath(muDev, path);
    if((par=udev_device_get_parent_with_subsystem_devtype(dev,"usb","usb_device"))!=NULL){
      if(strcmp("partition", udev_device_get_devtype(dev))==0){
	strcpy(pDev->device, udev_device_get_devnode(dev));
	strcpy(pDev->name, udev_device_get_sysattr_value(par, "product"));
	udev_device_unref(dev);
	udev_enumerate_unref(enumerate);
	return 1;
      }
      udev_device_unref(dev);
    }
  }
  udev_enumerate_unref(enumerate);
  return 0;
}


int UdevMonitor::monitor(deventry *pDev){
  int fd;
  struct udev_monitor *mon;
  struct udev_device *dev;
  struct udev_device *par;
  fd_set fds;
  int ret;
  mon = udev_monitor_new_from_netlink(muDev, "udev");
  udev_monitor_filter_add_match_subsystem_devtype(mon, "block", NULL);
  udev_monitor_enable_receiving(mon);
  fd = udev_monitor_get_fd(mon);
  while(1){
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    ret = select(fd+1,&fds,NULL,NULL,NULL);
    if(ret > 0 && FD_ISSET(fd, &fds)){
      dev = udev_monitor_receive_device(mon);
      if(dev){
	if(strcmp("partition", udev_device_get_devtype(dev))==0){
	  if((par=udev_device_get_parent_with_subsystem_devtype(dev,"usb","usb_device"))!=NULL){
	    strcpy(pDev->device, udev_device_get_devnode(dev));
	    strcpy(pDev->name, udev_device_get_sysattr_value(par, "product"));
	    udev_device_unref(dev);
	    udev_monitor_unref(mon);
	    return 1;
	  }
	}
	udev_device_unref(dev);
      }
    }else{
      udev_monitor_unref(mon);
      return -1;
    }
  }
  return 0;
}

int UdevMonitor::getDiskDevice(deventry *pDev){
  if(checkCurrent(pDev) == 1){
    return 1;
  }
  return monitor(pDev);
}


int UdevMonitor::getDiskDevicePoll(deventry *pDev){
  return checkCurrent(pDev);
}
