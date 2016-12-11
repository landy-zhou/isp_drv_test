/******************************************************************************
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/

#ifndef _V5628_API_H
#define _V5628_API_H

#include <linux/videodev2.h>

#define NR_METERING_WIN_WEIGHT 13
#define V5628_NR_PIPELINE_MAX 2

struct v5628_regval {
	__u32	reg;
	__u32	val;
};

struct v5628_data_node {
	__u32	size;
	void	*buffer;
};

struct v5628isp_profile {
	unsigned int profile_id;
	void	*arg;
};

struct v5628isp_win {
	__s32 left;
	__s32 top;
	__s32 right;
	__s32 bottom;
};

struct v5628isp_expo_metering {
	unsigned int mode;
	struct v5628isp_win stat_win;
	struct v4l2_rect center_win;
	unsigned int win_weight[NR_METERING_WIN_WEIGHT];
};

struct v5628isp_awb_gain {
	int write;
	unsigned int b;
	unsigned int gb;
	unsigned int gr;
	unsigned int r;
};

struct memory_sensor {
	char	name[32];
};

enum adv_dns_type {
	ADV_DNS_NONE = 0,
	ADV_DNS_Y,
	ADV_DNS_UV,
	ADV_DNS_YUV,
	ADV_DNS_MAX,
};

struct v5628isp_adv_dns {
	enum adv_dns_type type;
	unsigned int times;
};

enum type_aeag {
	TYPE_3A_UNLOCK,
	TYPE_3A_LOCKED,
	TYPE_3A_CNT,
};

enum type_combo {
	TYPE_3D_COMBO,
	TYPE_HS_COMBO,
	TYPE_HDR_COMBO,
	TYPE_COMBO_CNT,
};

struct v5628isp_path_arg {
	enum type_aeag	aeag;
	enum type_combo	combo;
	__u16		nr_frame;
	__u16		ratio_1_2;
	__u16		ratio_1_3;
	__u16		linear_yuv;
};

struct v5628isp_anti_shake_arg {
	__u16		block_size;
	int			enable;
};

enum v4l2_priv_colorfx {
	V4L2_PRIV_COLORFX_NONE          = 0,
	V4L2_PRIV_COLORFX_MONO_CHROME   = 1,
	V4L2_PRIV_COLORFX_NEGATIVE      = 2,
	V4L2_PRIV_COLORFX_SEPIA         = 3,
	V4L2_PRIV_COLORFX_SKETCH        = 4,
	V4L2_PRIV_COLORFX_WATER_COLOR   = 5,
	V4L2_PRIV_COLORFX_INK           = 6,
	V4L2_PRIV_COLORFX_CARTOON       = 7,
	V4L2_PRIV_COLORFX_COLOR_INK     = 8,
	V4L2_PRIV_COLORFX_AQUA          = 9,
	V4L2_PRIV_COLORFX_BLACK_BOARD   = 10,
	V4L2_PRIV_COLORFX_WHITE_BOARD   = 11,
	V4L2_PRIV_COLORFX_POSTER        = 12,
	V4L2_PRIV_COLORFX_SOLARIZATION  = 13,
	V4L2_PRIV_COLORFX_MAX,
};

/*
enum aquilav1_pad_id {
	AQUILAV1PAD_IDI_IN = 0,
	AQUILAV1PAD_IDI_PIPE1,
	AQUILAV1PAD_IDI_DUMP1,
	AQUILAV1PAD_IDI_PIPE2,
	AQUILAV1PAD_IDI_DUMP2,
	AQUILAV1PAD_IDI_BOTH,
	AQUILAV1_IDI_PAD_CNT,

	AQUILAV1PAD_PIPE_IN = 0,
	AQUILAV1PAD_PIPE_OUT,
	AQUILAV1_PIPE_PAD_CNT,

	AQUILAV1PAD_MS_IN = 0,
	AQUILAV1PAD_MS_OUT,

       AQUILAV1PAD_YUVOFFLINE_IN = 0,
	AQUILAV1PAD_YUVOFFLINE_OUT,

       AQUILAV1PAD_PDNS_IN = 0,
	AQUILAV1PAD_PDNS_OUT,

	AQUILAV1PAD_AXI_IN = 0,
	AQUILAV1PAD_AXI_OUT,

	AQUILAV1_AXI_PAD_CNT,
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
*/

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
    /* ccic pads */
    CCIC_CSI_PAD_IN = 0,
    CCIC_CSI_PAD_LOCAL,
    CCIC_CSI_PAD_XFEED,
    CCIC_CSI_PAD_ISP,
    CCIC_CSI_PAD_CNT,

    CCIC_DMA_PAD_IN = 0,
    CCIC_DMA_PAD_OUT,
    CCIC_DMA_PAD_CNT,
};

#define CID_AF_SNAPSHOT	1
#define CID_AF_CONTINUOUS	2

/*
 * the auto frame rate control.
 * if set it to enable, the frame rate will drop to increase exposure.
 */
#define CID_AUTO_FRAME_RATE_DISABLE   0
#define CID_AUTO_FRAME_RATE_ENABLE    1

/*
 * the range for auto frame rate
 */
#define CID_AFR_MIN_FPS_MIN   5
#define CID_AFR_MIN_FPS_MAX   30

/*
 * save and restore min fps for auto frame rate
 */
#define CID_AFR_SAVE_MIN_FPS      0
#define CID_AFR_RESTORE_MIN_FPS   1

/*
 * If enable AF 5x5 windown, the focus is based on central part.
 */
#define CID_AF_5X5_WIN_DISABLE    0
#define CID_AF_5X5_WIN_ENABLE     1

/*
 * If 3A work on threshold, the gain or exposure value will be
 * decided by max and min value, otherwise the value is setted value.
 */
#define CID_AEC_AUTO_THRESHOLD	0
#define CID_AEC_MANUAL		1

#define CID_AEC_NO_STABLE		0
#define CID_AEC_STABLE		1

#define CID_AFR_MAX_GAIN_MIN   0
#define CID_AFR_MAX_GAIN_MAX   0xffff
/*
 * The OTP control variant
 */
#define V4L2_CID_SENSOR_OTP_CONTROL_NONE	0x00
#define V4L2_CID_SENSOR_OTP_CONTROL_WB	0x01
#define V4L2_CID_SENSOR_OTP_CONTROL_LENC	0x02

/* entity component names */
#define V5628_IDI1_NAME		"aquilav1isd-IDI1"
#define V5628_IDI2_NAME		"aquilav1isd-IDI2"
#define V5628_PATH_YUV_1_NAME	"aquilav1isd-Pipeline#1"
#define V5628_PATH_YUV_2_NAME	"aquilav1isd-Pipeline#2"
#define V5628_PATH_RAW_1_NAME	"aquilav1isd-DataDump#1"
#define V5628_PATH_RAW_2_NAME	"aquilav1isd-DataDump#2"
#define V5628_PATH_M2M_1_NAME	"aquilav1isd-MemorySensor#1"
#define V5628_PATH_M2M_2_NAME	"aquilav1isd-MemorySensor#2"
#define V5628_PATH_HIGHSPEED_NAME	"aquilav1isd-HighSpeed"
#define V5628_PATH_HDRPROCESS_NAME	"aquilav1isd-HDRProcess"
#define V5628_PATH_3DSTEREO_NAME	"aquilav1isd-3DStereo"
#define V5628_PATH_YUVOFFLINE1_NAME	"aquilav1isd-YUVOFFLINE1"
#define V5628_PATH_YUVOFFLINE2_NAME	"aquilav1isd-YUVOFFLINE2"
#define V5628_PATH_PDNS_NAME	"aquilav1isd-PDNS"
#define V5628_PATH_COMBINE_NAME	"aquilav1isd-combine"

#define V5628_OUTPUT_1_NAME	"aquilav1isd-Output-1"
#define V5628_OUTPUT_2_NAME	"aquilav1isd-Output-2"
#define V5628_OUTPUT_3_NAME	"aquilav1isd-Output-3"
#define V5628_OUTPUT_4_NAME	"aquilav1isd-Output-4"
#define V5628_OUTPUT_5_NAME	"aquilav1isd-Output-5"
#define V5628_OUTPUT_6_NAME	"aquilav1isd-Output-6"
#define V5628_OUTPUT_7_NAME	"aquilav1isd-Output-7"
#define V5628_OUTPUT_8_NAME	"aquilav1isd-Output-8"
#define VDEV_OUTPUT_1_NAME	"vout<AQUILAV1_OUTPUT_1_NAME>"
#define VDEV_OUTPUT_2_NAME	"vout<AQUILAV1_OUTPUT_2_NAME>"
#define VDEV_OUTPUT_3_NAME	"vout<AQUILAV1_OUTPUT_3_NAME>"
#define VDEV_OUTPUT_4_NAME	"vout<AQUILAV1_OUTPUT_4_NAME>"
#define VDEV_OUTPUT_5_NAME	"vout<AQUILAV1_OUTPUT_5_NAME>"
#define VDEV_OUTPUT_6_NAME	"vout<AQUILAV1_OUTPUT_6_NAME>"
#define VDEV_OUTPUT_7_NAME	"vout<AQUILAV1_OUTPUT_7_NAME>"
#define VDEV_OUTPUT_8_NAME	"vout<AQUILAV1_OUTPUT_8_NAME>"

#define V5628_INPUT_1_NAME	"aquilav1isd-Input-1"
#define V5628_INPUT_2_NAME	"aquilav1isd-Input-2"
#define V5628_INPUT_3_NAME	"aquilav1isd-Input-3"
#define V5628_INPUT_4_NAME	"aquilav1isd-Input-4"
#define V5628_INPUT_5_NAME	"aquilav1isd-Input-5"
#define V5628_INPUT_6_NAME	"aquilav1isd-Input-6"
#define VDEV_INPUT_1_NAME	"vin<AQUILAV1_INPUT_1_NAME>"
#define VDEV_INPUT_2_NAME	"vin<AQUILAV1_INPUT_2_NAME>"
#define VDEV_INPUT_3_NAME	"vin<AQUILAV1_INPUT_3_NAME>"
#define VDEV_INPUT_4_NAME	"vin<AQUILAV1_INPUT_4_NAME>"
#define VDEV_INPUT_5_NAME	"vin<AQUILAV1_INPUT_5_NAME>"
#define VDEV_INPUT_6_NAME	"vin<AQUILAV1_INPUT_6_NAME>"

#define V5628_SENSOR_OV8856_NAME	"sensor:ovt.ov8856"
#define V5628_VCM_PDRV_NAME	"vcm-pdrv"
#define V5628_FLASH_PDRV_NAME	"flash-pdrv"
#define V5628_CCIC_CSI0_NAME	"ccic-csi #0"
#define V5628_CCIC_DMA_NAME	"ccic-dma #0"



/* the specific controls */
#define V4L2_CID_PRIVATE_AF_MODE \
	(V4L2_CID_CAMERA_CLASS_BASE + 0x1000)
#define V4L2_CID_PRIVATE_COLORFX \
	(V4L2_CID_CAMERA_CLASS_BASE + 0x1001)
#define V4L2_CID_PRIVATE_AUTO_FRAME_RATE \
	(V4L2_CID_CAMERA_CLASS_BASE + 0x1002)
#define V4L2_CID_PRIVATE_AFR_MIN_FPS \
	(V4L2_CID_CAMERA_CLASS_BASE + 0x1003)
#define V4L2_CID_PRIVATE_AFR_SR_MIN_FPS \
	(V4L2_CID_CAMERA_CLASS_BASE + 0x1004)
#define V4L2_CID_PRIVATE_AF_5X5_WIN \
	(V4L2_CID_CAMERA_CLASS_BASE + 0x1005)
#define V4L2_CID_PRIVATE_SENSOR_OTP_CONTROL	\
	(V4L2_CID_CAMERA_CLASS_BASE + 0x1006)
#define V4L2_CID_PRIVATE_AEC_MANUAL_MODE\
	(V4L2_CID_CAMERA_CLASS_BASE + 0x1007)
#define V4L2_CID_PRIVATE_TARGET_Y\
	(V4L2_CID_CAMERA_CLASS_BASE + 0x1008)
#define V4L2_CID_PRIVATE_MEAN_Y\
	(V4L2_CID_CAMERA_CLASS_BASE + 0x1009)
#define V4L2_CID_PRIVATE_AEC_STABLE\
	(V4L2_CID_CAMERA_CLASS_BASE + 0x100a)
#define V4L2_CID_PRIVATE_AFR_MAX_GAIN \
	(V4L2_CID_CAMERA_CLASS_BASE + 0x100B)

#define V4L2_PLANE_SIGNATURE_PIPELINE_META	\
	v4l2_fourcc('M', 'E', 'T', 'A')
#define V4L2_PLANE_SIGNATURE_PIPELINE_INFO	\
	v4l2_fourcc('P', 'P', 'I', 'F')



#define VIDIOC_PRIVATE_AQUILAV1ISP_TOPOLOGY_SNAPSHOT \
	_IOWR('V', BASE_VIDIOC_PRIVATE + 0, struct v5628isp_profile)
#define VIDIOC_PRIVATE_AQUILAV1ISP_CONFIG_AF_WINDONW \
	_IOW('V', BASE_VIDIOC_PRIVATE + 1, struct v4l2_rect)
#define VIDIOC_PRIVATE_AQUILAV1ISP_CONFIG_EXPO_METERING_MODE \
	_IOW('V', BASE_VIDIOC_PRIVATE + 2, struct v5628isp_expo_metering)
#define VIDIOC_PRIVATE_AQUILAV1ISP_CONFIG_EXPO_METERING_ROI \
	_IOW('V', BASE_VIDIOC_PRIVATE + 3, struct v5628isp_win)
#define VIDIOC_PRIVATE_AQUILAV1ISP_DOWNLOAD_CTDATA \
	_IOWR('V', BASE_VIDIOC_PRIVATE + 4, struct v5628_data_node)
#define VIDIOC_PRIVATE_AQUILAV1ISP_UPLOAD_CTDATA \
	_IOWR('V', BASE_VIDIOC_PRIVATE + 5, struct v5628_data_node)
#define VIDIOC_PRIVATE_AQUILAV1ISP_CONFIG_AWB_GAIN \
	_IOWR('V', BASE_VIDIOC_PRIVATE + 6, struct v5628isp_awb_gain)
#define VIDIOC_PRIVATE_AQUILAV1ISP_CONFIG_MEMORY_SENSOR \
	_IOW('V', BASE_VIDIOC_PRIVATE + 7, struct memory_sensor)
#define VIDIOC_PRIVATE_AQUILAV1ISP_CONFIG_ADV_DNS \
	_IOW('V', BASE_VIDIOC_PRIVATE + 8, struct v5628isp_adv_dns)
#define VIDIOC_PRIVATE_AQUILAV1ISP_SET_PATH_ARG \
	_IOW('V', BASE_VIDIOC_PRIVATE + 9, struct v5628isp_path_arg)
#define VIDIOC_PRIVATE_AQUILAV1ISP_ANTI_SHAKE\
	_IOW('V', BASE_VIDIOC_PRIVATE + 10, struct v5628isp_anti_shake_arg)
#endif /* _V5628_API_H */
