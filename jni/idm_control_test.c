/******************************************************************************
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/

#if 0
void case_online_test(idm_PlatCam * platcam)
{
    idm_PlatCam * platcam = idm_ctrl_Init();
    struct media_device	*media = platcam->media;
    media_setup_link(media,platcam->sd_node[SDCODE_SENSOR0]->pads[PAD_SENSOR_OUT],
                                            platcam->sd_node[SDCODE_CCICV2_CSI0]->pads[CCIC_CSI_PAD_IN],MEDIA_LNK_FL_ENABLED);
    media_setup_link(media,platcam->sd_node[SDCODE_CCICV2_CSI0]->pads[CCIC_CSI_PAD_ISP],
                                            platcam->sd_node[SDCODE_ISP_IDI1]->pads[PAD_IDI_IN],MEDIA_LNK_FL_ENABLED);
    media_setup_link(media,platcam->sd_node[SDCODE_SENSOR0]->pads[PAD_SENSOR_OUT],
                                            platcam->sd_node[SDCODE_CCICV2_CSI0]->pads[CCIC_CSI_PAD_IN],MEDIA_LNK_FL_ENABLED);
    media_setup_link(media,platcam->sd_node[SDCODE_SENSOR0]->pads[PAD_SENSOR_OUT],
                                            platcam->sd_node[SDCODE_CCICV2_CSI0]->pads[CCIC_CSI_PAD_IN],MEDIA_LNK_FL_ENABLED);
    media_setup_link(media,platcam->sd_node[SDCODE_SENSOR0]->pads[PAD_SENSOR_OUT],
                                            platcam->sd_node[SDCODE_CCICV2_CSI0]->pads[CCIC_CSI_PAD_IN],MEDIA_LNK_FL_ENABLED);
                                
//todo set size formt crop ioctrl
//todo set snopshot ioctrl
//vnode cotrol
//enqu dequeue    
}
#endif
/******************************************************************************
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <linux/v5628_api.h>
#include "idm_control.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))


int main(int argc, char **argv)
{

    idm_ctrl_init();
    return 0;
}
