
#ifndef _V5216_API_H
#define _V5216_API_H

#include <linux/videodev2.h>

#define NR_METERING_WIN_WEIGHT 13
#define V5216_NR_PIPELINE_MAX 2

struct v5216_regval {
	__u32	reg;
	__u32	val;
};

struct v5216_data_node {
	__u32	size;
	void	*buffer;
};

struct v5216isp_profile {
	unsigned int profile_id;
	void	*arg;
};

struct v5216isp_win {
	__s32 left;
	__s32 top;
	__s32 right;
	__s32 bottom;
};

struct v5216isp_expo_metering {
	unsigned int mode;
	struct v5216isp_win stat_win;
	struct v4l2_rect center_win;
	unsigned int win_weight[NR_METERING_WIN_WEIGHT];
};

struct v5216isp_awb_gain {
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

struct v5216isp_adv_dns {
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

struct v5216isp_path_arg {
	enum type_aeag	aeag;
	enum type_combo	combo;
	__u16		nr_frame;
	__u16		ratio_1_2;
	__u16		ratio_1_3;
	__u16		linear_yuv;
};

struct v5216isp_anti_shake_arg {
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

#define V5216_IDI1_NAME		"v5216isd-IDI1"
#define V5216_IDI2_NAME		"v5216isd-IDI2"
#define V5216_PATH_YUV_1_NAME	"v5216isd-Pipeline#1"
#define V5216_PATH_YUV_2_NAME	"v5216isd-Pipeline#2"
#define V5216_PATH_RAW_1_NAME	"v5216isd-DataDump#1"
#define V5216_PATH_RAW_2_NAME	"v5216isd-DataDump#2"
#define V5216_PATH_M2M_1_NAME	"v5216isd-MemorySensor#1"
#define V5216_PATH_M2M_2_NAME	"v5216isd-MemorySensor#2"
#define V5216_PATH_COMBINE_NAME	"v5216isd-combine"
#define V5216_OUTPUT_A_NAME	"v5216isd-Output-A"
#define V5216_OUTPUT_B_NAME	"v5216isd-Output-B"
#define V5216_OUTPUT_C_NAME	"v5216isd-Output-C"
#define V5216_OUTPUT_D_NAME	"v5216isd-Output-D"
#define V5216_OUTPUT_E_NAME	"v5216isd-Output-E"
#define V5216_OUTPUT_F_NAME	"v5216isd-Output-F"
#define V5216_INPUT_A_NAME	"v5216isd-Input-A"
#define V5216_INPUT_B_NAME	"v5216isd-Input-B"
#define V5216_INPUT_C_NAME	"v5216isd-Input-C"
#define VDEV_INPUT_A_NAME	"vin<"V5216_INPUT_A_NAME">"
#define VDEV_INPUT_B_NAME	"vin<"V5216_INPUT_B_NAME">"
#define VDEV_INPUT_C_NAME	"vin<"V5216_INPUT_C_NAME">"

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



#define VIDIOC_PRIVATE_V5216ISP_TOPOLOGY_SNAPSHOT \
	_IOWR('V', BASE_VIDIOC_PRIVATE + 0, struct v5216isp_profile)
#define VIDIOC_PRIVATE_V5216ISP_CONFIG_AF_WINDONW \
	_IOW('V', BASE_VIDIOC_PRIVATE + 1, struct v4l2_rect)
#define VIDIOC_PRIVATE_V5216ISP_CONFIG_EXPO_METERING_MODE \
	_IOW('V', BASE_VIDIOC_PRIVATE + 2, struct v5216isp_expo_metering)
#define VIDIOC_PRIVATE_V5216ISP_CONFIG_EXPO_METERING_ROI \
	_IOW('V', BASE_VIDIOC_PRIVATE + 3, struct v5216isp_win)
#define VIDIOC_PRIVATE_V5216ISP_DOWNLOAD_CTDATA \
	_IOWR('V', BASE_VIDIOC_PRIVATE + 4, struct v5216_data_node)
#define VIDIOC_PRIVATE_V5216ISP_UPLOAD_CTDATA \
	_IOWR('V', BASE_VIDIOC_PRIVATE + 5, struct v5216_data_node)
#define VIDIOC_PRIVATE_V5216ISP_CONFIG_AWB_GAIN \
	_IOWR('V', BASE_VIDIOC_PRIVATE + 6, struct v5216isp_awb_gain)
#define VIDIOC_PRIVATE_V5216ISP_CONFIG_MEMORY_SENSOR \
	_IOW('V', BASE_VIDIOC_PRIVATE + 7, struct memory_sensor)
#define VIDIOC_PRIVATE_V5216ISP_CONFIG_ADV_DNS \
	_IOW('V', BASE_VIDIOC_PRIVATE + 8, struct v5216isp_adv_dns)
#define VIDIOC_PRIVATE_V5216ISP_SET_PATH_ARG \
	_IOW('V', BASE_VIDIOC_PRIVATE + 9, struct v5216isp_path_arg)
#define VIDIOC_PRIVATE_V5216ISP_ANTI_SHAKE\
	_IOW('V', BASE_VIDIOC_PRIVATE + 10, struct v5216isp_anti_shake_arg)
#endif /* _V5216_API_H */
