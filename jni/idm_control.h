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
    IDM_NODE_MAX_CNT,
};

struct node_code_map{
    enum idm_subdev_code code;
    char *name;
};

static char *idm_sensor_name[] = {  //sensor
    V5628_SENSOR_OV8856_NAME
};

static char *idm_ccic_name[] = {    //ccic
    V5628_CCIC_CSI0_NAME,
    V5628_CCIC_DMA_NAME
};

static char *v5628_idi_name[] = {   //idi
    V5628_IDI1_NAME,
    V5628_IDI2_NAME
};

static char *idm_pipeline_name[] = { //isp pipeline
    V5628_PATH_YUV_1_NAME,
    V5628_PATH_RAW_1_NAME,
    V5628_PATH_M2M_1_NAME,
    V5628_PATH_YUV_2_NAME,
    V5628_PATH_RAW_2_NAME,
    V5628_PATH_M2M_2_NAME,
    V5628_PATH_COMBINE_NAME,
    V5628_PATH_HIGHSPEED_NAME,  
    V5628_PATH_HDRPROCESS_NAME,
    V5628_PATH_3DSTEREO_NAME,
    V5628_PATH_YUVOFFLINE1_NAME,
    V5628_PATH_YUVOFFLINE2_NAME,
    V5628_PATH_PDNS_NAME,
    V5628_PATH_COMBINE_NAME
};

static char *idm_axiw_name[] = {    //axi write port
    V5628_OUTPUT_1_NAME,
    V5628_OUTPUT_2_NAME,
    V5628_OUTPUT_3_NAME,
    V5628_OUTPUT_4_NAME,
    V5628_OUTPUT_5_NAME,
    V5628_OUTPUT_6_NAME,
    V5628_OUTPUT_7_NAME,
    V5628_OUTPUT_8_NAME
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
    V5628_INPUT_1_NAME,
    V5628_INPUT_2_NAME,
    V5628_INPUT_3_NAME,
    V5628_INPUT_4_NAME,
    V5628_INPUT_5_NAME,
    V5628_INPUT_6_NAME,
};

static char *idm_vin_name[] = {  //video device output (isp input)
    VDEV_INPUT_1_NAME,
    VDEV_INPUT_2_NAME,
    VDEV_INPUT_3_NAME,
    VDEV_INPUT_4_NAME,
    VDEV_INPUT_5_NAME,
    VDEV_INPUT_6_NAME,
};

#define MEDIA_LNK_FL_ENABLED		(1)
#define MEDIA_LNK_FL_DISABLED		(0)

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
};

//struct idm_platform;

struct idm_node{
    struct media_entity	*me;
    enum idm_node_type type;
    char   name[32];
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

struct idm_online_topology {
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

struct idm_offline_topology {
    struct idm_vnode	*vnode_src;
    struct idm_subdev	*sd_axi_rd;
    struct idm_subdev	*sd_path;
    struct v4l2_rect	*sd_path_crop;
    struct idm_subdev	*sd_axi_wr[MAX_OUTPUT_PER_PIPELINE];
    struct idm_vnode	*vnode_dst[MAX_OUTPUT_PER_PIPELINE];
    int	   dst_map;
};

int idm_ctrl_init(void);


#ifdef IDM_DEBUG_EN 

#define idm_err(format, arg...)		\
	do { \
	printf("[app:ERROR]: " format "\n", ## arg); \
	fflush(stdout); \
	} while (0)
#define idm_warn(format, arg...)		\
	do { \
	printf("[app:WARNING]: " format "\n", ## arg); \
	fflush(stdout); \
	} while (0)
#define idm_info(format, arg...)		\
	do { \
	printf("[app:INFO][%s]: " format "\n",__FUNCTION__, ## arg); \
	fflush(stdout); \
	} while (0)

#else

#define idm_err(format, arg...)	    do{}while(0)	
#define idm_warn(format, arg...)    do{}while(0)	
#define idm_info(format, arg...)	do{}while(0)
#endif

#endif //__IDM_CONTROL_H__

