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
#include "tools.h"
#include "idm_utils.h"
#include "idm_control.h"

void case_online_test(void)
{
    int ret;
    //setup links
    idm_ctl_enable_link_by_id(SD_ID_SENSOR0, SD_ID_CCICV2_CSI0);
    idm_ctl_enable_link_by_id(SD_ID_CCICV2_CSI0, SD_ID_ISP_IDI1);
    idm_ctl_enable_link_by_id(SD_ID_ISP_IDI1, SD_ID_ISP_PIPE1);
    idm_ctl_enable_link_by_id(SD_ID_ISP_PIPE1, SD_ID_ISP_A1W1);
    idm_ctl_enable_link_by_id(SD_ID_ISP_A1W1, VD_ID_ISP_A1W1);
                                    
    //set size/formt/crop for subdev
    struct idm_sd_fmt online_sd_fmts[] = {
            { 
                .dev_id = SD_ID_SENSOR0,
                .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
                .pad	= 0,
                .width	= 3264,
                .height	= 2448,
                .code	= V4L2_MBUS_FMT_SBGGR10_1X10,
                .ops	= IDM_APPLY_FMT, // Current only support set format
            },
            {
                .dev_id = SD_ID_ISP_IDI1,
                .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
                .pad	= PAD_IDI_IN,
                .crop_wnd = {	// For Output <t,l,w,h>
                    .left	= 0,
                    .top	= 0,
                    .width	= 3264,
                    .height	= 2448,
                },
                .width	= 3264,	
                .height	= 2448,
                .code	= V4L2_MBUS_FMT_SBGGR10_1X10,
                .ops	= IDM_APPLY_CROP ,
            },
            {
                .dev_id = SD_ID_ISP_IDI1,
                .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
                .pad	= PAD_IDI_PIPE1,
                .crop_wnd = {	// For Output <t,l,w,h>
                    .left	= 0,
                    .top	= 0,
                    .width	= 3264,
                    .height	= 2448,
                },
                .width	= 3264,	
                .height	= 2448,
                .code	= V4L2_MBUS_FMT_SBGGR10_1X10,
                .ops	= IDM_APPLY_CROP ,
            },
            {
                .dev_id = SD_ID_ISP_PIPE1,
                .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
                .pad	= PAD_PIPE_IN,
                .crop_wnd = {
                    .left	= 0,
                    .top	= 0,
                    .width	= 3264,
                    .height	= 2448,
                },
                .width	= 3264,
                .height	= 2448,
                //.code	= V4L2_MBUS_FMT_NV12_1X12,
                .ops	= (IDM_APPLY_CROP | IDM_APPLY_FMT),
            },
            {
                .dev_id = SD_ID_ISP_PIPE1,
                .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
                .pad	= PAD_PIPE_OUT,
                .crop_wnd = {
                    .left	= 0,
                    .top	= 0,
                    .width	= 3264,
                    .height	= 2448,
                },
                .width	= 3264,
                .height	= 2448,
                //.code	= V4L2_MBUS_FMT_NV12_1X12,
                .ops	= (IDM_APPLY_CROP | IDM_APPLY_FMT),
            },
            {
                .dev_id = SD_ID_ISP_A1W1,
                .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
                .pad	= PAD_AXI_IN,
                .crop_wnd = {
                    .left	= 0,
                    .top	= 0,
                    .width	= 3264,	
                    .height	= 2448,
                },
                .code	= V4L2_MBUS_FMT_NV12_1X12,
                .width	= 3264,
                .height	= 2448,
                .ops	=  (IDM_APPLY_CROP | IDM_APPLY_FMT),
            },
            {
                .dev_id = SD_ID_ISP_A1W1,
                .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
                .pad	= PAD_AXI_OUT,
                .crop_wnd = {
                    .left	= 0,
                    .top	= 0,
                    .width	= 640,	
                    .height	= 480,
                },
                .code	= V4L2_MBUS_FMT_NV12_1X12,
                .width	= 640,
                .height	= 480,
                .ops	= (IDM_APPLY_CROP | IDM_APPLY_FMT),
            },
    };
    ret = idm_ctl_sd_set_fmts(online_sd_fmts, ARRAY_SIZE(online_sd_fmts));
    if(0 > ret){
        idm_err("online set fmts error");
        return;
    }

    //set video device fmts
    struct idm_vd_fmt online_vd_fmts[] = {
        [0] = {
            .dev_id = VD_ID_ISP_A1W1,
            .name = {"Output1"},
            .width = 640,
            .height = 480,
            .pix_fmt= V4L2_PIX_FMT_NV12M,
        },
    };
    ret = idm_ctl_vd_set_fmts(&online_vd_fmts[0],ARRAY_SIZE(online_vd_fmts));
    if(0 > ret){
        idm_err("online video device set fmt error");
        return;
    }
/*
    struct idm_buf_info online_vd_buf = {
            .buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE,
            .memory = V4L2_MEMORY_DMABUF,
            .nr_buf = 3;
    };

    idm_ctl_vd_reqbuf(idm_ctl_find_node_by_id(VD_ID_ISP_A1W1), &online_vd_buf);
*/
    //set snopshot enable ioctl
    ret = idm_ctl_trig_snapshot(SD_ID_ISP_PIPE1);
    if(0 > ret){
        idm_err("online trigger snapshot error");
        return;
    }
    //vnode cotrol
    //enqu dequeue
}



int main(int argc, char **argv)
{
    int ret;
    ret = idm_ctl_init();
    if(0 > ret)
        return -1;
    idm_ctl_platform_dump();

    case_online_test();

    idm_ctl_exit();
    return 0;
}
