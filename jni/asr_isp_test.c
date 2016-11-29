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
#include "media_lib.h"
#include "camera_api.h"
//#define DUMPRAW
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

struct ctx_input {
    int source;
    int ccic;
    int idi;
    int path;
    int outport;
    int output;
    int online;
    int iommu;
};

int ParseArgs(struct ctx_input *top_ctx, int argc, char *argv[])
{
    unsigned int i;
    int ret;

    switch (argc) {
	case 9:
	    if (!strcmp("-v", argv[1]))
		top_ctx->online = 0;
	    else
		top_ctx->online = 1;
	    top_ctx->source = atoi(argv[2]);
	    top_ctx->ccic = atoi(argv[3]);
	    top_ctx->idi = atoi(argv[4]);
	    top_ctx->path = atoi(argv[5]);
	    top_ctx->outport = atoi(argv[6]);
	    top_ctx->output  = atoi(argv[7]);
	    top_ctx->iommu  = atoi(argv[8]);
	    printf("%s %s %d %d %d %d %d %d %d\n",argv[0],argv[1],top_ctx->source,top_ctx->ccic,top_ctx->idi,top_ctx->path,top_ctx->outport,top_ctx->output,top_ctx->iommu);
	    break;
	default:
	    printf("usage: %s -v/-s <input_id> <ccic_id> <idi_id> <pipeline_id> <axiw_id> <output_id> <iommu_enable>\n", argv[0]);
	    return -EINVAL;
    }
    return 0;
}

int MediaMatchLink(struct CamNode *src,struct CamNode *dst)
{
    int i;
    for (i = 0; i < src->NrLink; i++) {
	if (src->link[i]->dst == dst)
	    return i;
    }
    return -1;
}

int MediaSetupLink(struct CamNode *src,struct CamNode *dst)
{
    int ret;
    ret = MediaMatchLink(src,dst);
    if(0>ret)
    {
	app_err("node %s has no link to %s",src->name,dst->name);
	goto exit;
    }
    app_info("node %s --> %s matched",src->name,dst->name);
    ret = CamLinkApply(src, src->link[ret],1);
    if(0>ret)
    {
	app_err("CamLinkApply %s to %s error",src->name,dst->name);
	goto exit;
    }

exit:
    return ret;
}

int MediaEnablePipelineLinks(struct PlatCam *pcam,struct ctx_input *topology)
{
    struct CamNode *match_src,*match_dst;
    int ret = 0;

    //src-->ccic
    if((topology->source>=0)&&(topology->ccic>=0))
    {
	match_src = pcam->SrcPool[topology->source];
	match_dst = pcam->CcicPool[topology->ccic];
	ret = MediaSetupLink(match_src,match_dst);
	if(0>ret)
	{
	    goto out;
	}
    }
    //ccic-->idi
    if((topology->ccic>=0)&&(topology->idi>=0))
    {
	match_src = pcam->CcicPool[topology->ccic];
	match_dst = pcam->IdiPool[topology->idi];
	ret = MediaSetupLink(match_src,match_dst);
	if(0>ret)
	{
	    goto out;
	}
    }
    //idi-->path
    if((topology->idi>=0)&&(topology->path>=0))
    {
	match_src = pcam->IdiPool[topology->idi];
	match_dst = pcam->PathPool[topology->path];
	ret = MediaSetupLink(match_src,match_dst);
	if(0>ret)
	{
	    goto out;
	}
    }
    //path-->outport
    if((topology->path>=0)&&(topology->outport>=0))
    {
	match_src = pcam->PathPool[topology->path];
	match_dst = pcam->OutportPool[topology->outport];
	ret = MediaSetupLink(match_src,match_dst);
	if(0>ret)
	{
	    goto out;
	}
    }
    //outport-->output
    if((topology->outport>=0)&&(topology->output>=0))
    {
	match_src = pcam->OutportPool[topology->outport];
	match_dst = pcam->DstPool[topology->output];
	ret = MediaSetupLink(match_src,match_dst);
	if(0>ret)
	{
	    goto out;
	}
    }
out:
    return ret;
}


int main(int argc, char **argv)
{
    struct PlatCam *pcam;
    int i,j,ret = -1;
    int out_fd, in_fd;
    struct ctx_input topology;

    //enum nodes, prepare links, setup media lib
    pcam = MediaLibInit();
    if (pcam == NULL)
	return -1;

    ret = ParseArgs(&topology, argc, argv);
    if (ret < 0)
	return ret;

    //enable media links to setup video route
    MediaEnablePipelineLinks(pcam,&topology);


    /* set format & crop */
    if (topology.online) {

	struct CamNodeFmtCombo SensorCfg = {
	    .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
	    .pad	= 0,
	    .width	= 3264,
	    .height	= 2448,
	    .code	= V4L2_MBUS_FMT_SBGGR10_1X10,
	    .scope	= (1 << PARAM_APPLY_FMT), // Current only support set format
	};
	/*	struct CamNodeFmtCombo CcicCfg = {
		.which	= V4L2_SUBDEV_FORMAT_ACTIVE,
		.CropWnd = {
		.left	= 0,
		.top	= 0,
		.width	= 3264,
		.height	= 2448,
		},
		.scope	= (1 << PARAM_APPLY_CROP),
		};
		*/
	struct CamNodeFmtCombo IdiCfgIn = {
	    .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
	    .pad	= AQUILAV1PAD_IDI_IN,
	    .CropWnd = {	// For Output <t,l,w,h>
		.left	= 0,
		.top	= 0,
		.width	= 3264,
		.height	= 2448,
	    },
	    .width	= 3264,	
	    .height	= 2448,
	    .code	= V4L2_MBUS_FMT_SBGGR10_1X10,
	    .scope	= (1 << PARAM_APPLY_CROP) ,
	};
	struct CamNodeFmtCombo IdiCfgOut = {
	    .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
	     .pad	= AQUILAV1PAD_IDI_PIPE1,
	    .CropWnd = {	// For Output <t,l,w,h>
		.left	= 0,
		.top	= 0,
		.width	= 3264,
		.height	= 2448,
	    },
	    .width	= 3264,	
	    .height	= 2448,
	    .code	= V4L2_MBUS_FMT_SBGGR10_1X10,
	    .scope	= (1 << PARAM_APPLY_CROP) ,
	};
	struct CamNodeFmtCombo PathCfgIn = {
	    .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
	    .pad	= AQUILAV1PAD_PIPE_IN,
	    .CropWnd = {
		.left	= 0,
		.top	= 0,
		.width	= 3264,
		.height	= 2448,
	    },
	    .width	= 3264,
	    .height	= 2448,
	    //.code	= V4L2_MBUS_FMT_NV12_1X12,
	    .scope	= (1 << PARAM_APPLY_CROP)|(1 << PARAM_APPLY_FMT),
	};
	struct CamNodeFmtCombo PathCfgOut = {
	    .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
	    .pad	= AQUILAV1PAD_PIPE_OUT,
	    .CropWnd = {
		.left	= 0,
		.top	= 0,
		.width	= 3264,
		.height	= 2448,
	    },
	    .width	= 3264,
	    .height	= 2448,
	    //.code	= V4L2_MBUS_FMT_NV12_1X12,
	    .scope	= (1 << PARAM_APPLY_CROP)|(1 << PARAM_APPLY_FMT),
	};
	struct CamNodeFmtCombo OutportCfgIn = {
	    .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
	    .pad	= AQUILAV1PAD_AXI_IN,
	    .CropWnd = {
		.left	= 0,
		.top	= 0,
		.width	= 3264,	
		.height	= 2448,
	    },
	    .code	= V4L2_MBUS_FMT_NV12_1X12,
	    .width	= 3264,
	    .height	= 2448,
	    .scope	= (1 << PARAM_APPLY_CROP)|(1 << PARAM_APPLY_FMT),
	};
	struct CamNodeFmtCombo OutportCfgOut = {
	    .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
	    .pad	= AQUILAV1PAD_AXI_OUT,
	    .CropWnd = {
		.left	= 0,
		.top	= 0,
		.width	= 640,	
		.height	= 480,
	    },
	    .code	= V4L2_MBUS_FMT_NV12_1X12,
	    .width	= 640,
	    .height	= 480,
	    .scope	= (1 << PARAM_APPLY_CROP)|(1 << PARAM_APPLY_FMT),
	};
	/*	struct CamNodeFmtCombo OutputCfg = {
		.which	= V4L2_SUBDEV_FORMAT_ACTIVE,
	//	.pad	= 1,
	.CropWnd = {
	.left	= 0,
	.top	= 0,
	.width	= 3264,	// 2X Zoom
	.height	= 2448,
	},
	//.scope	= (1 << PARAM_APPLY_CROP),
	};
	*/
	struct CamCtxParam para[] = {
	    {
		.ParamName	= PARAM_SENSOR_SET_FMT,
		.ParamArg	= &SensorCfg,
	    },
	    /*	    {
		    .ParamName	= PARAM_CCIC_SET_COMBO,
		    .ParamArg	= &CcicCfg,
		    },
		    */
	    {
		.ParamName	= PARAM_IDI_SET_CROP,
		.ParamArg	= &IdiCfgIn,
	    },
	    {
		.ParamName	= PARAM_IDI_SET_CROP,
		.ParamArg	= &IdiCfgOut,
	    },
	    {
		.ParamName	= PARAM_PATH_SET_COMBO,
		.ParamArg	= &PathCfgIn,
	    },
	    {
		.ParamName	= PARAM_PATH_SET_COMBO,
		.ParamArg	= &PathCfgOut,
	    },
	    {
		.ParamName	= PARAM_OUTPORT_SET_COMBO,
		.ParamArg	= &OutportCfgIn,
	    },
	    {
		.ParamName	= PARAM_OUTPORT_SET_COMBO,
		.ParamArg	= &OutportCfgOut,
	    },
	    /*	    {
		    .ParamName	= PARAM_OUTPUT_SET_COMBO,
		    .ParamArg	= &OutputCfg,
		    },
		    */
	};

	for(i=0;i<ARRAY_SIZE(para);i++)
	{
	    struct CamNode * node;
	    switch(para[i].ParamName){
		case PARAM_SENSOR_SET_FMT:
		    node = pcam->SrcPool[topology.source];
		    break;
		case PARAM_IDI_SET_CROP:
		    node = pcam->IdiPool[topology.idi];
		    break;
		case PARAM_PATH_SET_COMBO:
		    node = pcam->PathPool[topology.path];
		    break;
		case PARAM_OUTPORT_SET_COMBO:
		    node = pcam->OutportPool[topology.outport];
		    break;
		case PARAM_OUTPUT_SET_COMBO:
		    node = pcam->DstPool[topology.output];
		    break;
		default:
		    app_info("configuration not support\n");
		    continue;
	    }
	    ret = CamNodeSetCombo(node,para[i].ParamArg);
	    if (ret < 0)
	    {
		app_err("node %s CamNodeSetCombo failed",node->name);
		goto out;
	    }
	}
    } else { }

    ret = CamNodeSetCtrl(pcam->PathPool[topology.path],&topology);
    if (ret < 0)
	goto out;

    /* create output thread */
    struct camera_stream_t out_stream[] = {
	[0] = {
	    .name = {"OutputA"},
	    .buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE,
	    .width = 640,
	    .height = 480,
	    .fmt_id = FMT_ID_NV12,
	    .NrBuf = 3,
	    .NrFrame = 4,
	    .save = 1,
        .mmu_en = topology.iommu,
	},
    };
    /* open video device */
    out_fd = CamNodeOpen(pcam->DstPool[topology.output]);
    if (out_fd < 0)
	goto out;
    out_stream[0].fd_cam = out_fd;
    /* set video device format */
    ret = SetStreamFmt(&out_stream[0]);
    if (ret < 0) {
	c_err("%s: failed to setup stream format", out_stream[0].name);
	goto exit;
    }
    ret = CameraThreadCreate(&out_stream[0]);
    if (ret < 0)
	goto exit;

    sleep(3*out_stream[0].NrFrame);
    CameraThreadKill(&out_stream[0]);

exit:
    CamNodeClose(pcam->DstPool[topology.output]);

    MediaLibExit(pcam);
    return 0;

out:
    app_err("abnormal release: %d\n", ret);
    MediaLibExit(pcam);

    return -1;
}
