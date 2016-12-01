/******************************************************************************
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/

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

int main(int argc, char **argv)
{
  
}
