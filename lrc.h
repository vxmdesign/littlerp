#ifndef LRC_H
#define LRC_H

#include <sys/ioctl.h>
#include <errno.h>
#include <stdio.h>
#include "lcd.h"
#include "dirmngr.h"

dentry lrc_menu(lcd *l, dirmngr *dm);

#endif
