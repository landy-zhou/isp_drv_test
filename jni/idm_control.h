/******************************************************************************
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/

#ifndef __IDM_CONTROL_H__
#define __IDM_CONTROL_H__

enum idm_subdev_code {
    SDCODE_ISP_IDI1	= 0,
    SDCODE_ISP_IDI2,
    SDCODE_ISP_PIPE1,
    SDCODE_ISP_DUMP1,
    SDCODE_ISP_MS1,
    SDCODE_ISP_PIPE2,
    SDCODE_ISP_DUMP2,
    SDCODE_ISP_MS2,
    SDCODE_ISP_HS,
    SDCODE_ISP_HDR,
    SDCODE_ISP_3D,
    SDCODE_ISP_YUVOFFLINE1,
    SDCODE_ISP_YUVOFFLINE2,
    SDCODE_ISP_PDNS,
    SDCODE_ISP_A1W1,
    SDCODE_ISP_A1W2,
    SDCODE_ISP_A1W3,
    SDCODE_ISP_A1W4,
    SDCODE_ISP_A1W5,
    SDCODE_ISP_A1W6,
    SDCODE_ISP_A1R1,
    SDCODE_ISP_A1R2,
    SDCODE_ISP_A1R3,
    SDCODE_ISP_A1R4,
    SDCODE_ISP_A2W1,
    SDCODE_ISP_A2W2,
    SDCODE_ISP_A2R1,
    SDCODE_ISP_A2R2,
    SDCODE_CCICV2_CSI0,
    SDCODE_CCICV2_CSI1,
    SDCODE_CCICV2_DMA0,
    SDCODE_CCICV2_DMA1,
    SDCODE_SENSOR0,
    SDCODE_VCM0,
    SDCODE_FLASH0,
    SDCODE_SENSOR1,
    SDCODE_VCM1,
    SDCODE_FLASH1,
    SDCODE_MAX_CNT,
};

enum idm_vnode_code {
    VNODE_ISP_A1W1,
    VNODE_ISP_A1W2,
    VNODE_ISP_A1W3,
    VNODE_ISP_A1W4,
    VNODE_ISP_A1W5,
    VNODE_ISP_A1W6,
    VNODE_ISP_A1R1,
    VNODE_ISP_A1R2,
    VNODE_ISP_A1R3,
    VNODE_ISP_A1R4,
    VNODE_ISP_A2W1,
    VNODE_ISP_A2W2,
    VNODE_ISP_A2R1,
    VNODE_ISP_A2R2,
    VNODE_MAX_CNT,
};

enum aquilav1_pad_id {
       PAD_SENSOR_OUT = 0,

	PAD_IDI_IN = 0,
	PAD_IDI_PIPE1,
	PAD_IDI_DUMP1,
	PAD_IDI_PIPE2,
	PAD_IDI_DUMP2,
	PAD_IDI_BOTH,
	IDI_PAD_CNT,

	PAD_PIPE_IN = 0,
	PAD_PIPE_OUT,
	PIPE_PAD_CNT,

	PAD_MS_IN = 0,
	PAD_MS_OUT,

       PAD_YUVOFFLINE_IN = 0,
	PAD_YUVOFFLINE_OUT,

       PAD_PDNS_IN = 0,
	PAD_PDNS_OUT,

	PAD_AXI_IN = 0,
	PAD_AXI_OUT,
	AXI_PAD_CNT,
};
enum ccic_pad_id {
	CCIC_CSI_PAD_IN = 0,
	CCIC_CSI_PAD_LOCAL,
	CCIC_CSI_PAD_XFEED,
	CCIC_CSI_PAD_ISP,
	CCIC_CSI_PAD_CNT,

	CCIC_DMA_PAD_IN = 0,
	CCIC_DMA_PAD_OUT,
	CCIC_DMA_PAD_CNT,
};
#define MEDIA_LNK_FL_ENABLED		(1)
#define MEDIA_LNK_FL_DISABLED		(0)

#define ISPSD_PAD_MAX 15
#define MAX_OUTPUT_PER_PIPELINE	8
struct idm_subdev {
	struct media_entity	*me;
	char			name[32];
       struct media_pad	pads[ISPSD_PAD_MAX];
       	struct idm_PlatCam		*plat;
	struct CamNodeOps	*ops;
       char     open_flag;
	int			fd;
};
struct idm_vnode {
	struct media_entity	*me;
	char			name[32];
       struct media_pad	pads[ISPSD_PAD_MAX];
       	struct idm_PlatCam		*plat;
       char     open_flag;
	int			fd;
};
struct idm_PlatCam {
	struct media_device	*media;
	struct idm_subdev		*sd_node[SDCODE_MAX_CNT];
       struct idm_vnode		*vnode[VNODE_MAX_CNT];
};

struct idm_online_topology {
       struct idm_subdev	*sensor;
	struct idm_subdev	*sd_ccic;
	struct idm_subdev	*sd_idi;
	struct v4l2_rect	*sd_idi_crop;
	struct idm_subdev	*sd_path;
	struct v4l2_rect	*sd_path_crop;
	struct idm_subdev	*sd_axi_wr[MAX_OUTPUT_PER_PIPELINE];
	struct idm_vnode	*vnode_dst[MAX_OUTPUT_PER_PIPELINE];
	int			dst_map;
};

struct idm_offline_topology {
	struct idm_vnode	*vnode_src;
       struct idm_subdev	*sd_axi_rd;
	struct idm_subdev	*sd_path;
	struct v4l2_rect	*sd_path_crop;
	struct idm_subdev	*sd_axi_wr[MAX_OUTPUT_PER_PIPELINE];
	struct idm_vnode	*vnode_dst[MAX_OUTPUT_PER_PIPELINE];
	int			dst_map;
};

#endif //__IDM_CONTROL_H__

