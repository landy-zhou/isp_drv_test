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

#include "MediaLib.h"
#include "CameraAPI.h"
//#define DUMPRAW
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

#define PRV_FMT 0
#define PRV_RES 3
#define PRV_CNT 30

#define CAP_FMT 2	// 0:422pack, 1:422plan, 2:420plan, 3:jpeg, 4: rgb565
#define CAP_RES 3	// 0-QCIF, 1-CIF, 2-QVGA, 3-VGA, 4-WVGA, 5-720p, 6-1080p, 7-5M
#define CAP_CNT 30

#define NR_VIDEO_BUFFER 5

#define CTX1_OUTPUT_PORTS	1
#define CTX2_OUTPUT_PORTS	1

struct ctx_input {
    char source;
    char path;
    char online:1;
};




int ParseCmd(struct ctx_input *top_ctx, int argc, char *argv[])
{
    unsigned int i;
    int ret;

    switch (argc) {
	case 4:
	    if (!strcmp("-v", argv[1]))
		top_ctx->online = 0;
	    else
		top_ctx->online = 1;
	    top_ctx->source = atoi(argv[2]);
	    top_ctx->path = atoi(argv[3]);
	    break;
	default:
	    printf("usage: %s -v <input_id> <pipeline_id>\n    or %s -s <sensor_id> <pipeline_id>\n", argv[0], argv[0]);
	    return -EINVAL;
    }
    return 0;
}

int main(int argc, char **argv)
{
    struct PlatCam *pcam;
    int i, j, NrSrc, NrPath, CID1, CID2, ret = -1;
    struct CamNodeEnum source[10];
    struct CamNodeEnum path[10];
    int out_fd[3], in_fd;
    struct ctx_input topology;

    pcam = MediaLibInit();
    if (pcam == NULL)
	return EXIT_FAILURE;

    for (i = 0, NrSrc = 0; ; i++) {
	struct CamNodeEnum *tmp = &source[NrSrc];
	ret = MediaLibEnumSource(pcam, i, tmp);
	if (ret < 0)
	    break;
	NrSrc++;
    }
    for (i = 0, NrPath = 0; ; i++) {
	struct CamNodeEnum *tmp = &path[NrPath];
	ret = MediaLibEnumPath(pcam, i, tmp);
	if (ret < 0)
	    break;
	NrPath++;
    }

    ret = ParseCmd(&topology, argc, argv);
    if (ret < 0)
	return ret;

    int SrcID = topology.source, PathID = topology.path;
    if (SrcID >= NrSrc) {
	app_err("Source ID %d don't exist", SrcID);
	for (i = 0; i < NrSrc; i++) {
	    struct CamNodeEnum *tmp = &source[i];
	    app_info("List of viable route for Source #%d:%s", i, tmp->name);
	    for (j = 0; j < tmp->NrRoute; j++)
		app_info("Route%d: '%s' ==> '%s'", j, tmp->route[j]->src->name, tmp->route[j]->dst->name);
	    app_info("=====================================");
	}
	ret = -ENOENT;
	goto out;
    }
    if (PathID >= NrPath) {
	app_err("Path ID %d don't exist", PathID);
	for (i = 0; i < NrPath; i++) {
	    struct CamNodeEnum *tmp = &path[i];
	    app_info("List of viable route for Path #%d:%s", i, tmp->name);
	    for (j = 0; j < tmp->NrRoute; j++)
		app_info("Route%d: '%s' ==> '%s'", j, tmp->route[j]->src->name, tmp->route[j]->dst->name);
	    app_info("=====================================");
	}
	ret = -ENOENT;
	goto out;
    }

    if (topology.online)
	app_info("request online context '%s' ==> '%s'", source[topology.source].name, path[topology.path].name);
    else
	app_info("request offline context '%s' ==> '%s'", source[topology.source].name, path[topology.path].name);

    CID1 = CameraContextInit(pcam, SrcID, PathID, CTX1_OUTPUT_PORTS);
    if (CID1 < 0)
	goto out;
#ifdef DUMPRAW
    CID2 = CameraContextInit(pcam, 0, 1, CTX2_OUTPUT_PORTS);
    if (CID2 < 0)
	goto out;
#endif
    struct camera_stream_t *in = NULL;
    struct camera_stream_t out_stream[] = {
	[0] = {
	    .name = {"OutputA"},
	    .buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE,
	    .width = 640,
	    .height = 480,
	    .fmt_id = FMT_ID_NV12,
	    .NrBuf = 1,
	    .NrFrame = 1,
	    .save = 1,
	},
	[1] = {
	    .name = {"OutputB"},
	    .buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE,
	    .width = 176,
	    .height = 144,
	    .fmt_id = FMT_ID_UYVY,
	    .NrBuf = 3,
	    .NrFrame = 8,
	    .save = 1,
	},
	[2] = {
	    .name = {"OutputC"},
	    .buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE,
	    .width = 640,
	    .height = 480,
	    .fmt_id = FMT_ID_UYVY,
	    .NrBuf = 3,
	    .NrFrame = 8,
	    .save = 1,
	},
	[3] = {
	    .name = {"OutputD"},
	    .buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE,
	    .width = 1280,
	    .height = 720,
	    .fmt_id = FMT_ID_UYVY,
	    .NrBuf = 3,
	    .NrFrame = 8,
	    .save = 1,
	},
    };
#ifdef DUMPRAW
    struct camera_stream_t c2_out_stream[] = {
	[0] = {
	    .name = {"dump"},
	    .buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE,
	    .width = 2592,
	    .height = 1944,
	    .fmt_id = FMT_ID_BA81,
	    .NrBuf = 3,
	    .NrFrame = 1,
	    .save = 1,
	},
    };
#endif
    /*
     * Setup pipeline parameters
     */
    if (topology.online) {
	// Online process parameters
	struct CamNodeFmtCombo SensorCfg = {
	    .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
	    //	.pad	= 0,
	    .width	= 640,
	    .height	= 480,
	    .code	= V4L2_MBUS_FMT_SBGGR10_1X10,
	    .scope	= (1 << PARAM_APPLY_FMT), // Current only support set format
	};
	struct CamNodeFmtCombo IDICfg = {
	    .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
	    //	.pad	= 2,	// V5216PAD_IDI_PIPE1
	    .CropWnd = {	// For Output <t,l,w,h>
		.left	= 0,
		.top	= 0,
		.width	= 640,
		.height	= 480,
	    },
	    .width	= 640,	// IDI Control: scale disable
	    .height	= 480,
	    .code	= V4L2_MBUS_FMT_UYVY8_2X8,
	    .scope	= (1 << PARAM_APPLY_CROP) | (1 << PARAM_APPLY_FMT),
	};
	struct CamNodeFmtCombo PathCfg = {
	    .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
	    //	.pad	= 1,
	    .CropWnd = {
		.left	= 0,
		.top	= 0,
		.width	= 640,	// 2X Zoom
		.height	= 480,
	    },
	    .scope	= (1 << PARAM_APPLY_CROP),
	};
	struct CamCtxParam para[] = {
	    {
		.ParamName	= PARAM_SRC_SET_COMBO,
		.ParamArg	= &SensorCfg,
	    },
	    {
		.ParamName	= PARAM_CROP_SET_COMBO,
		.ParamArg	= &IDICfg,
	    },
	    {
		.ParamName	= PARAM_PATH_SET_CROP,
		.ParamArg	= &PathCfg,
	    },
	};

	ret = CameraContextPara(pcam, CID1, para, ARRAY_SIZE(para));
	if (ret < 0)
	{
	    app_err("CID1 camera context para configure failed\n");
	    goto out;
	}
	ret = CameraContextPara(pcam, CID2, para, ARRAY_SIZE(para));
	if (ret < 0)
	{
	    app_err("CID2 camera context para configure failed\n");
	    goto out;
	}
    } else {
	// Offline process parameters
	struct CamNodeFmtCombo CropCfg = {
	    .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
	    .pad	= 1,	// 
	    .CropWnd = {
		.left	= 0,
		.top	= 0,
		.width	= 640,
		.height	= 480,
	    },
	    .width	= 640,	// 1X1 Bining
	    .height	= 480,
	    .code	= V4L2_MBUS_FMT_NV12_1X12,
	    .scope	= (1 << PARAM_APPLY_CROP) | (1 << PARAM_APPLY_FMT),
	};
	struct CamNodeFmtCombo PathCfg = {
	    .which	= V4L2_SUBDEV_FORMAT_ACTIVE,
	    .pad	= 1,
	    .CropWnd = {
		.left	= 0,
		.top	= 0,
		.width	= 640,	// 1X Zoom
		.height	= 480,
	    },
	    .scope	= (1 << PARAM_APPLY_CROP),
	};
	struct CamCtxParam para[] = {
	    {
		.ParamName	= PARAM_CROP_SET_COMBO,
		.ParamArg	= &CropCfg,
	    },
	    {
		.ParamName	= PARAM_PATH_SET_CROP,
		.ParamArg	= &PathCfg,
	    },
	};
	ret = CameraContextPara(pcam, CID1, para, ARRAY_SIZE(para));
	if (ret < 0)
	    goto out;

	in_fd = CameraContextInputOpen(pcam, CID1);
	if (in_fd < 0) {
	    ret = -EPIPE;
	    goto out;
	}
	in = calloc(1, sizeof(*in));
	if (in == NULL) {
	    c_err("failed to allocate stream handle");
	    return EXIT_FAILURE;
	}
	memset(in, 0, sizeof(in));
	strcpy(in->name, "RAWInput");
	strcpy(in->InputName, "/bin/640x480.raw10");
	in->NrFrame = 1;
	in->buf_type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	in->fmt_id = FMT_ID_BA81;
	in->width = 640;
	in->height = 480;
	in->NrBuf = 1;
	in->load = 1;
	in->fd_cam = in_fd;
    }

    /* Create output V4L2 thread */
    for (i = 0; i < CTX1_OUTPUT_PORTS; i++) {
	out_fd[i] = CameraContextOutputOpen(pcam, CID1, i);
	if (out_fd[i] < 0)
	    goto out;
	out_stream[i].fd_cam = out_fd[i];
    }
#ifdef DUMPRAW
    for (i = 0; i < CTX2_OUTPUT_PORTS; i++) {
	out_fd[i] = CameraContextOutputOpen(pcam, CID2, i);
	if (out_fd[i] < 0)
	    goto out;
	c2_out_stream[i].fd_cam = out_fd[i];
    }
#endif
    for (i = 0; i < CTX1_OUTPUT_PORTS; i++) {
	ret = SetStreamFmt(out_stream + i);
	if (ret < 0) {
	    c_err("%s: failed to setup stream format", out_stream[i].name);
	    goto exit;
	}
    }
#ifdef DUMPRAW
    for (i = 0; i < CTX2_OUTPUT_PORTS; i++) {
	ret = SetStreamFmt(c2_out_stream + i);
	if (ret < 0) {
	    c_err("%s: failed to setup context2 stream format", c2_out_stream[i].name);
	    goto exit;
	}
    }
#endif
    for (i = 0; i < CTX1_OUTPUT_PORTS; i++) {

	ret = CameraThreadCreate(out_stream + i);
	if (ret < 0)
	    goto exit;
    }
    //sleep(3);
#ifdef DUMPRAW
    for (i = 0; i < CTX2_OUTPUT_PORTS; i++) {
	ret = CameraThreadCreate(c2_out_stream + i);
	if (ret < 0)
	    goto exit;
    }
#endif
    if (!topology.online) {
	/* Input V4L2 thread must be created AFTER output thread */
	ret = SetStreamFmt(in);
	if (ret < 0) {
	    c_err("%s: failed to setup stream format", in->name);
	    goto exit;
	}
	ret = CameraThreadCreate(in);
	if (ret < 0)
	    goto exit;
    }

    sleep(8);

    if (in)
	CameraThreadKill(in);

    for (i = 0; i < CTX1_OUTPUT_PORTS; i++)
	CameraThreadKill(out_stream + i);
#ifdef DUMPRAW
    for (i = 0; i < CTX2_OUTPUT_PORTS; i++)
	CameraThreadKill(c2_out_stream + i);
#endif
exit:
    for (i = 0; i < CTX1_OUTPUT_PORTS; i++)
	CameraContextOutputClose(pcam, CID1, i);
#ifdef DUMPRAW
    for (i = 0; i < CTX2_OUTPUT_PORTS; i++)
	CameraContextOutputClose(pcam, CID2, i);
#endif	
    CameraContextInputClose(pcam, CID1);

    CameraContextKill(pcam, CID1);
    CameraContextKill(pcam, CID2);

    MediaLibExit(pcam);
    return EXIT_SUCCESS;

out:
    printf("abnormal release: %d\n", ret);
    MediaLibExit(pcam);

    return EXIT_FAILURE;
}
