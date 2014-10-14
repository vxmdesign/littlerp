#ifndef SLICE_MNGR_H
#define SLICE_MNGR_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "backgroundLayer.h"
#include "imageLayer.h"
#include "key.h"
#include "loadpng.h"

#include "bcm_host.h"

class SliceMngr{
 public:
  SliceMngr();
  ~SliceMngr();
  int blankScreen();
  int loadSlice(const char *pFilename);
  int sliceScreen();
 private:
  DISPMANX_DISPLAY_HANDLE_T mDisplay;
  DISPMANX_MODEINFO_T mInfo;
  BACKGROUND_LAYER_T mBackground;
  IMAGE_LAYER_T mImage;
  int mState;
};


#endif
