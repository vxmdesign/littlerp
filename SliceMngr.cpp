#include "SliceMngr.h"

SliceMngr::SliceMngr(){
  int result;
  uint16_t background = 0x000F;
  DISPMANX_UPDATE_HANDLE_T update;
  mState = 0;
  bcm_host_init();
  mDisplay = vc_dispmanx_display_open(0);
  if(mDisplay == 0){
    return;
  }
  result = vc_dispmanx_display_get_info(mDisplay, &mInfo);
  if(result != 0){
    return;
  }
  initBackgroundLayer(&mBackground, background, 1);
  initImage(&(mImage.image), VC_IMAGE_RGBA32, mInfo.width, mInfo.height, false);
  createResourceImageLayer(&mImage, 0);
  vc_dispmanx_rect_set(&(mImage.srcRect),
		       0 << 16,
                       0 << 16,
                       mImage.image.width << 16,
                       mImage.image.height << 16);

  vc_dispmanx_rect_set(&(mImage.dstRect),
                       (mInfo.width - mImage.image.width) / 2,
                       (mInfo.height - mImage.image.height) / 2,
                       mImage.image.width,
		       mImage.image.height);
  update = vc_dispmanx_update_start(0);
  if(update == 0){
    return;
  }
  addElementBackgroundLayer(&mBackground, mDisplay, update);
  addElementImageLayerCentered(&mImage, &mInfo, mDisplay, update);
  result = vc_dispmanx_update_submit_sync(update);
  if(result != 0){
    return;
  }
  mState = 1;
}

SliceMngr::~SliceMngr(){
  destroyBackgroundLayer(&mBackground);
  destroyImageLayer(&mImage);
  vc_dispmanx_display_close(mDisplay);
}

int SliceMngr::sliceScreen(){
  DISPMANX_UPDATE_HANDLE_T update;
  update = vc_dispmanx_update_start(0);
  vc_dispmanx_element_change_layer(update, mBackground.element, 0);
  vc_dispmanx_element_change_layer(update, mImage.element, 1);

  return vc_dispmanx_update_submit_sync(update);
}


int SliceMngr::blankScreen(){
  DISPMANX_UPDATE_HANDLE_T update;
  update = vc_dispmanx_update_start(0);
  vc_dispmanx_element_change_layer(update, mBackground.element, 1);
  vc_dispmanx_element_change_layer(update, mImage.element, 0);
  return vc_dispmanx_update_submit_sync(update);
}

int SliceMngr::loadSlice(const char *pFilename){
  if (loadPng(&(mImage.image), pFilename) == false){
    printf("failed to load\n");
    return -1;
  }else{
    vc_dispmanx_resource_write_data(mImage.resource,
                                    mImage.image.type,
                                    mImage.image.pitch,
                                    mImage.image.buffer,
                                    &(mImage.dstRect));
  }
  return 0;
}
