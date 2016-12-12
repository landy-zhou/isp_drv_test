/******************************************************************************
 * isp driver middleware control functions
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/

#ifndef __IDM_CONTROL_H__
#define __IDM_CONTROL_H__

#include <stdio.h>
#include "mediactl.h"
#include "v4l2subdev.h"
#include <linux/v5628_api.h>
#include <linux/v4l2-subdev.h>

#define IDM_DEBUG_EN

enum idm_dev_id {
    //subdev
    SD_ID_ISP_IDI1	= 0,
    SD_ID_ISP_IDI2,
    SD_ID_ISP_PIPE1,
    SD_ID_ISP_DUMP1,
    SD_ID_ISP_MS1,
    SD_ID_ISP_PIPE2,
    SD_ID_ISP_DUMP2,
    SD_ID_ISP_MS2,
    SD_ID_ISP_HS,
    SD_ID_ISP_HDR,
    SD_ID_ISP_3D,
    SD_ID_ISP_YUVOFFLINE1,
    SD_ID_ISP_YUVOFFLINE2,
    SD_ID_ISP_PDNS,
    SD_ID_ISP_A1W1,
    SD_ID_ISP_A1W2,
    SD_ID_ISP_A1W3,
    SD_ID_ISP_A1W4,
    SD_ID_ISP_A1W5,
    SD_ID_ISP_A1W6,
    SD_ID_ISP_A1R1,
    SD_ID_ISP_A1R2,
    SD_ID_ISP_A1R3,
    SD_ID_ISP_A1R4,
    SD_ID_ISP_A2W1,
    SD_ID_ISP_A2W2,
    SD_ID_ISP_A2R1,
    SD_ID_ISP_A2R2,
    SD_ID_CCICV2_CSI0,
    SD_ID_CCICV2_CSI1,
    SD_ID_CCICV2_DMA0,
    SD_ID_CCICV2_DMA1,
    SD_ID_SENSOR0,
    SD_ID_VCM0,
    SD_ID_FLASH0,
    SD_ID_SENSOR1,
    SD_ID_VCM1,
    SD_ID_FLASH1,
    //video device
    VD_ID_ISP_A1W1,
    VD_ID_ISP_A1W2,
    VD_ID_ISP_A1W3,
    VD_ID_ISP_A1W4,
    VD_ID_ISP_A1W5,
    VD_ID_ISP_A1W6,
    VD_ID_ISP_A1R1,
    VD_ID_ISP_A1R2,
    VD_ID_ISP_A1R3,
    VD_ID_ISP_A1R4,
    VD_ID_ISP_A2W1,
    VD_ID_ISP_A2W2,
    VD_ID_ISP_A2R1,
    VD_ID_ISP_A2R2,
    IDM_NODE_MAX_CNT,
};


static char *idm_sensor_name[] = {  //sensor
    AQUILAV1_SENSOR_OV8856_NAME
};

static char *idm_vcm_name[] = {  //vcm
    AQUILAV1_VCM_PDRV_NAME
};

static char *idm_flash_name[] = {  //flash led
    AQUILAV1_FLASH_PDRV_NAME
};

static char *idm_ccic_name[] = {    //ccic
    AQUILAV1_CCIC_CSI0_NAME,
    AQUILAV1_CCIC_DMA_NAME
};

static char *idm_idi_name[] = {   //idi
    AQUILAV1_IDI1_NAME,
    AQUILAV1_IDI2_NAME
};

static char *idm_pipeline_name[] = { //isp pipeline
    AQUILAV1_PATH_YUV_1_NAME,
    AQUILAV1_PATH_RAW_1_NAME,
    AQUILAV1_PATH_M2M_1_NAME,
    AQUILAV1_PATH_YUV_2_NAME,
    AQUILAV1_PATH_RAW_2_NAME,
    AQUILAV1_PATH_M2M_2_NAME,
    AQUILAV1_PATH_COMBINE_NAME,
    AQUILAV1_PATH_HIGHSPEED_NAME,  
    AQUILAV1_PATH_HDRPROCESS_NAME,
    AQUILAV1_PATH_3DSTEREO_NAME,
    AQUILAV1_PATH_YUVOFFLINE1_NAME,
    AQUILAV1_PATH_YUVOFFLINE2_NAME,
    AQUILAV1_PATH_PDNS_NAME,
    AQUILAV1_PATH_COMBINE_NAME
};

static char *idm_axiw_name[] = {    //axi write port
    AQUILAV1_OUTPUT_1_NAME,
    AQUILAV1_OUTPUT_2_NAME,
    AQUILAV1_OUTPUT_3_NAME,
    AQUILAV1_OUTPUT_4_NAME,
    AQUILAV1_OUTPUT_5_NAME,
    AQUILAV1_OUTPUT_6_NAME,
    AQUILAV1_OUTPUT_7_NAME,
    AQUILAV1_OUTPUT_8_NAME
};

static char *idm_vout_name[] = {  //video device capture (isp output)
    VDEV_OUTPUT_1_NAME,
    VDEV_OUTPUT_2_NAME,
    VDEV_OUTPUT_3_NAME,
    VDEV_OUTPUT_4_NAME,
    VDEV_OUTPUT_5_NAME,
    VDEV_OUTPUT_6_NAME,
    VDEV_OUTPUT_7_NAME,
    VDEV_OUTPUT_8_NAME
};

static char *idm_axir_name[] = {    //axi read port
    AQUILAV1_INPUT_1_NAME,
    AQUILAV1_INPUT_2_NAME,
    AQUILAV1_INPUT_3_NAME,
    AQUILAV1_INPUT_4_NAME,
    AQUILAV1_INPUT_5_NAME,
    AQUILAV1_INPUT_6_NAME,
};

static char *idm_vin_name[] = {  //video device output (isp input)
    VDEV_INPUT_1_NAME,
    VDEV_INPUT_2_NAME,
    VDEV_INPUT_3_NAME,
    VDEV_INPUT_4_NAME,
    VDEV_INPUT_5_NAME,
    VDEV_INPUT_6_NAME,
};

#define ISPSD_PAD_MAX 15
#define MAX_OUTPUT_PER_PIPELINE	8

enum idm_node_type{
    IDM_NODE_SENSOR=0,
    IDM_NODE_CCIC,
    IDM_NODE_IDI,
    IDM_NODE_PIPELINE,
    IDM_NODE_AXIW, //output port
    IDM_NODE_AXIR, //input port
    IDM_NODE_VOUT,
    IDM_NODE_VIN,
    IDM_NODE_VCM,
    IDM_NODE_FLASH
};

struct idm_typename_map{
    enum idm_node_type type;
    char **name;
};

struct idm_idname_map{
    enum idm_dev_id id;
    char *name;
    enum idm_node_type type;
};

struct idm_node{
    struct media_entity	*me;
    enum idm_node_type type;
    char   name[32];
    int id;
    //struct media_pad	pads[ISPSD_PAD_MAX];
    struct idm_platform *plat;
    //struct CamNodeOps *ops;
    char open_flag;
    int	fd;
};

struct idm_platform {
    struct media_device	*media;
    struct idm_node *nodes[IDM_NODE_MAX_CNT];
    int node_num;
};

enum {
    IDM_APPLY_FMT = 0x01<<0,
	IDM_APPLY_CROP = 0x01<<1
};

struct idm_fmt_crop{
	enum idm_dev_id dev_id;
    __u32	which;
	__u32	pad;
	struct  v4l2_rect	crop_wnd;
	__u32	width;
	__u32	height;
	__u32	code;
	__u8	ops;	/* If bit[IDM_APPLY_FMT] set: apply w/h/c. if bit[IDM_APPLY_CROP] set: apply CropWnd */
};

enum idm_topology_type{
    IDM_TOPOLOGY_ONLINE = 0,
    IDM_TOPOLOGY_OFFLINE = 1
};

struct idm_topology {
    int type;
    struct idm_subdev	*sensor;
    struct idm_subdev	*sd_ccic;
    struct idm_subdev	*sd_idi;
    struct v4l2_rect	*sd_idi_crop;
    struct idm_subdev	*sd_path;
    struct v4l2_rect	*sd_path_crop;
    struct idm_subdev	*sd_axi_wr[MAX_OUTPUT_PER_PIPELINE];
    struct idm_vnode	*vnode_dst[MAX_OUTPUT_PER_PIPELINE];
    int		dst_map;
};
/*
struct idm_offline_topology {
    struct idm_vnode	*vnode_src;
    struct idm_subdev	*sd_axi_rd;
    struct idm_subdev	*sd_path;
    struct v4l2_rect	*sd_path_crop;
    struct idm_subdev	*sd_axi_wr[MAX_OUTPUT_PER_PIPELINE];
    struct idm_vnode	*vnode_dst[MAX_OUTPUT_PER_PIPELINE];
    int	   dst_map;
};
*/

/*
 * idm_ctl_init: idm control module init
 */
int idm_ctl_init(void);

/*
 * open node and reture fd
 */
int idm_ctl_node_open(struct idm_node *node);
/* 
 * idm_platform_dump: dump struct idm_platform info
 */
int idm_ctl_platform_dump(void);

/* 
 * enable/disable by idm device id
 */
int idm_ctl_enable_link_by_id(enum idm_dev_id sid, enum idm_dev_id did);
int idm_ctl_disable_link_by_id(enum idm_dev_id sid, enum idm_dev_id did);

/* 
 * set format & crop
 * args: fmts, format arrays; n number of formats
 */ 
int idm_ctl_set_fmts(struct idm_fmt_crop *fmts, int n);
 
/* 
 * trigger snapshot ioctl
 */
int idm_ctl_trig_snapshot(enum idm_dev_id id);

/* 
 * exit idm control modlue
 */
int idm_ctl_exit(void);


#ifdef IDM_DEBUG_EN 

#define idm_err(format, arg...)		\
	do { \
	printf("[IDM:ERROR]: " format "\n", ## arg); \
	fflush(stdout); \
	} while (0)
#define idm_warn(format, arg...)		\
	do { \
	printf("[IDM:WARNING]: " format "\n", ## arg); \
	fflush(stdout); \
	} while (0)
#define idm_info(format, arg...)		\
	do { \
	printf("[IDM:INFO][%s]: " format "\n",__FUNCTION__, ## arg); \
	fflush(stdout); \
	} while (0)

#else

#define idm_err(format, arg...)	    do{}while(0)	
#define idm_warn(format, arg...)    do{}while(0)	
#define idm_info(format, arg...)	do{}while(0)

#endif

#endif //__IDM_CONTROL_H__

