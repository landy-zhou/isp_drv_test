/******************************************************************************
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "idm_control.h"
#include "tools.h"
//#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

struct idm_platform idm_plat;

struct idm_idname_map idm_idname_table[]={
    { .id=SD_ID_SENSOR0,			.name=AQUILAV1_SENSOR_OV8856_NAME		.type=IDM_NODE_SENSOR},
    { .id=SD_ID_SENSOR1,			.name=NULL		.type=IDM_NODE_SENSOR},
    { .id=SD_ID_VCM0,			    .name=AQUILAV1_VCM_PDRV_NAME		.type=IDM_NODE_VCM},
    { .id=SD_ID_VCM1,			    .name=NULL		.type=IDM_NODE_VCM},
    { .id=SD_ID_FLASH0,			    .name=AQUILAV1_FLASH_PDRV_NAME		.type=IDM_NODE_FLASH},
    { .id=SD_ID_FLASH1,			    .name=NULL		.type=IDM_NODE_FLASH},
    { .id=SD_ID_CCICV2_CSI0,		.name=AQUILAV1_CCIC_CSI0_NAME		.type=IDM_NODE_CCIC},
    { .id=SD_ID_CCICV2_CSI1,		.name=NULL		.type=IDM_NODE_CCIC},
    { .id=SD_ID_CCICV2_DMA0,		.name=AQUILAV1_CCIC_DMA_NAME		.type=IDM_NODE_CCIC},
    { .id=SD_ID_CCICV2_DMA1,		.name=NULL		.type=IDM_NODE_CCIC},
    { .id=SD_ID_ISP_IDI1,			.name=AQUILAV1_IDI1_NAME		.type=IDM_NODE_IDI},
    { .id=SD_ID_ISP_IDI2,			.name=AQUILAV1_IDI2_NAME		.type=IDM_NODE_IDI},
    { .id=SD_ID_ISP_PIPE1,			.name=AQUILAV1_PATH_YUV_1_NAME		.type=IDM_NODE_PIPELINE},
    { .id=SD_ID_ISP_DUMP1,			.name=AQUILAV1_PATH_RAW_1_NAME		.type=IDM_NODE_PIPELINE},
    { .id=SD_ID_ISP_MS1,			.name=AQUILAV1_PATH_M2M_1_NAME		.type=IDM_NODE_PIPELINE},
    { .id=SD_ID_ISP_PIPE2,			.name=AQUILAV1_PATH_YUV_2_NAME		.type=IDM_NODE_PIPELINE},
    { .id=SD_ID_ISP_DUMP2,			.name=AQUILAV1_PATH_RAW_2_NAME		.type=IDM_NODE_PIPELINE},
    { .id=SD_ID_ISP_MS2,			.name=AQUILAV1_PATH_M2M_2_NAME		.type=IDM_NODE_PIPELINE},
    { .id=SD_ID_ISP_HS,			    .name=AQUILAV1_PATH_HIGHSPEED_NAME		.type=IDM_NODE_PIPELINE},
    { .id=SD_ID_ISP_HDR,			.name=AQUILAV1_PATH_HDRPROCESS_NAME		.type=IDM_NODE_PIPELINE},
    { .id=SD_ID_ISP_3D,			    .name=AQUILAV1_PATH_3DSTEREO_NAME		.type=IDM_NODE_PIPELINE},
    { .id=SD_ID_ISP_YUVOFFLINE1,	.name=AQUILAV1_PATH_YUVOFFLINE1_NAME		.type=IDM_NODE_PIPELINE},
    { .id=SD_ID_ISP_YUVOFFLINE2,	.name=AQUILAV1_PATH_YUVOFFLINE2_NAME		.type=IDM_NODE_PIPELINE},
    { .id=SD_ID_ISP_PDNS,			.name=AQUILAV1_PATH_PDNS_NAME		.type=IDM_NODE_PIPELINE},
    { .id=SD_ID_ISP_A1W1,			.name=AQUILAV1_OUTPUT_1_NAME		.type=IDM_NODE_AXIW},
    { .id=SD_ID_ISP_A1W2,			.name=AQUILAV1_OUTPUT_2_NAME		.type=IDM_NODE_AXIW},
    { .id=SD_ID_ISP_A1W3,			.name=AQUILAV1_OUTPUT_3_NAME		.type=IDM_NODE_AXIW},
    { .id=SD_ID_ISP_A1W4,			.name=AQUILAV1_OUTPUT_4_NAME		.type=IDM_NODE_AXIW},
    { .id=SD_ID_ISP_A1W5,			.name=AQUILAV1_OUTPUT_5_NAME		.type=IDM_NODE_AXIW},
    { .id=SD_ID_ISP_A1W6,			.name=AQUILAV1_OUTPUT_6_NAME		.type=IDM_NODE_AXIW},
    { .id=SD_ID_ISP_A1R1,			.name=AQUILAV1_INPUT_1_NAME		.type=IDM_NODE_AXIR},
    { .id=SD_ID_ISP_A1R2,			.name=AQUILAV1_INPUT_2_NAME		.type=IDM_NODE_AXIR},
    { .id=SD_ID_ISP_A1R3,			.name=AQUILAV1_INPUT_3_NAME		.type=IDM_NODE_AXIR},
    { .id=SD_ID_ISP_A1R4,			.name=AQUILAV1_INPUT_4_NAME		.type=IDM_NODE_AXIR},
    { .id=SD_ID_ISP_A2W1,			.name=AQUILAV1_OUTPUT_7_NAME		.type=IDM_NODE_AXIW},
    { .id=SD_ID_ISP_A2W2,			.name=AQUILAV1_OUTPUT_8_NAME		.type=IDM_NODE_AXIW},
    { .id=SD_ID_ISP_A2R1,			.name=AQUILAV1_INPUT_5_NAME		.type=IDM_NODE_AXIR},
    { .id=SD_ID_ISP_A2R2,			.name=AQUILAV1_INPUT_6_NAME		.type=IDM_NODE_AXIR},
    { .id=VD_ID_ISP_A1W1,			.name=VDEV_OUTPUT_1_NAME		.type=IDM_NODE_VOUT},
    { .id=VD_ID_ISP_A1W2,			.name=VDEV_OUTPUT_2_NAME		.type=IDM_NODE_VOUT},
    { .id=VD_ID_ISP_A1W3,			.name=VDEV_OUTPUT_3_NAME		.type=IDM_NODE_VOUT},
    { .id=VD_ID_ISP_A1W4,			.name=VDEV_OUTPUT_4_NAME		.type=IDM_NODE_VOUT},
    { .id=VD_ID_ISP_A1W5,			.name=VDEV_OUTPUT_5_NAME		.type=IDM_NODE_VOUT},
    { .id=VD_ID_ISP_A1W6,			.name=VDEV_OUTPUT_6_NAME		.type=IDM_NODE_VOUT},
    { .id=VD_ID_ISP_A1R1,			.name=VDEV_INPUT_1_NAME		.type=IDM_NODE_VIN},
    { .id=VD_ID_ISP_A1R2,			.name=VDEV_INPUT_2_NAME		.type=IDM_NODE_VIN},
    { .id=VD_ID_ISP_A1R3,			.name=VDEV_INPUT_3_NAME		.type=IDM_NODE_VIN},
    { .id=VD_ID_ISP_A1R4,			.name=VDEV_INPUT_4_NAME		.type=IDM_NODE_VIN},
    { .id=VD_ID_ISP_A2W1,			.name=VDEV_OUTPUT_7_NAME		.type=IDM_NODE_VOUT},
    { .id=VD_ID_ISP_A2W2,			.name=VDEV_OUTPUT_8_NAME		.type=IDM_NODE_VOUT},
    { .id=VD_ID_ISP_A2R1,			.name=VDEV_INPUT_5_NAME		.type=IDM_NODE_VIN},
    { .id=VD_ID_ISP_A2R2,			.name=VDEV_INPUT_6_NAME     .type=IDM_NODE_VIN}
};

struct idm_typename_map idm_typename_table[]={
    { .type = IDM_NODE_SENSOR, .name = idm_sensor_name},
    { .type = IDM_NODE_CCIC, .name = idm_ccic_name},
    { .type = IDM_NODE_IDI, .name = idm_idi_name},
    { .type = IDM_NODE_PIPELINE, .name = idm_pipeline_name},
    { .type = IDM_NODE_AXIW, .name = idm_axiw_name},
    { .type = IDM_NODE_AXIR, .name = idm_axir_name},
    { .type = IDM_NODE_VOUT, .name = idm_vout_name},
    { .type = IDM_NODE_VIN, .name = idm_vin_name},
    { .type = IDM_NODE_VCM, .name = idm_vcm_name},
    { .type = IDM_NODE_FLASH, .name = idm_flash_name}
};


/* node->me field must be set before */
static int idm_ctl_find_node_type(struct idm_node *node)
{
    int i,j;
    for( i=0; i<ARRAY_SIZE(idm_typename_table); i++ ){
        for( j=0; j<ARRAY_SIZE(idm_typename_table[i].name); j++){
            idm_info("i=%d,j=%d",i,j);
            if( 0 == strncmp(node->name, idm_typename_table[i].name[j], strlen(node->name)) ){
                node->type = idm_typename_table[i].type;
                return 0;
            }
        }
    }
    return -1;
}

static struct idm_node *idm_ctl_find_node_by_id(enum idm_dev_id id)
{
    if(id >= IDM_NODE_MAX_CNT){
        idm_err("invalid idm device id");
        return NULL;
    }
    return idm_plat.nodes[id];
}

/* 
 * create a node for entity and add it to idm_plat
 */
int idm_node_add(struct idm_platform *plat, struct media_entity *me, int id)
{
    struct idm_node *node = calloc(1, sizeof(struct idm_node));

    if (NULL == node)
        return -1;
    node->me = me;
    node->plat = plat;
    strcpy(node->name, me->info.name);
    if(0 > idm_ctl_find_node_type(node)){
        idm_err("failed to find node type");
        return -1;
    }
    node->id = id;
    plat->nodes[id] = node;
    plat->node_num++;
    return 0;
}

static int idm_ctl_node_dump(struct idm_node *node)
{
    idm_info("node id %d, type %d, name '%s'",node->id,node->type,node->name);
    return 0;
}

int idm_ctl_platform_dump(void)
{
    int i;
    for( i=0; i<IDM_NODE_MAX_CNT; i++ ){
        if(idm_plat.nodes[i])
            idm_ctl_node_dump(idm_plat.nodes[i]);
    }
    return 0;
}


int idm_ctl_node_open(struct idm_node *node)
{
    if (node->open_flag == 0){
        node->fd = open(node->me->devname, O_RDWR | O_NONBLOCK);
        if(node->fd < 0){
            idm_err("failed to open '%s': %s", node->name, strerror(errno));
            return -errno;
        }
    }
    node->open_flag = 1;
    return node->fd;
}

int idm_ctl_node_close(struct idm_node *node)
{
    int ret = 0;
    if (node->open_flag == 1) {
        ret = close(node->fd);
        if(ret < 0) {
            idm_err("failed to close '%s': %s", node->name, strerror(errno));
            return -errno;
        }
        node->open_flag = 0;
        return 0;
    }
}

int idm_ctl_init(void)
{
    int i,j;

    memset(&idm_plat,0,sizeof(idm_plat));

    /* open media device */
    idm_plat.media = media_open("/dev/media0");
    if (NULL == idm_plat.media) {
        idm_err("failed to open media node");
        return -1;
    }

    /* init debug function */
    media_debug_set_handler(idm_plat.media,
            (void (*)(void *, ...))fprintf, stdout);

    /* enum entities, add node to idm for each media_entity */
    for (i = 0; i < IDM_NODE_MAX_CNT; i++) {
        struct idm_idname_map  *idname_map = &idm_idname_table[i];
        idm_info("Finding node: %s",idname_map->name);
        if(NULL == idname_map->name)
            continue;
        struct media_entity *me = media_get_entity_by_name(idm_plat.media,idname_map->name,strlen(idname_map->name));
        if (NULL == me)
            break;
        idm_info("Found media_entity: %s",me->info.name);
        if(0 > idm_node_add(&idm_plat, me, idname_map->id))
            goto err_out;
    }
    return 0;
    //todo? :identify node types and add to pools, 
    //todo:set all pad info
err_out:
    media_close(idm_plat.media);
}

/* 
 * idm_match_link: find the link from src to dst 
 * return: link index of src
 */
int idm_ctl_match_link(struct idm_node *src, struct idm_node *dst)
{
    int i;
    for (i = 0; i < src->me->num_links; i++) {
        if (src->me->links[i].sink->entity == dst->me)
            return i;
    }
    idm_warn("no match links from %s to %s",src->name,dst->name);
    return -1;
}

/*
 * setup links with given flag
 */
int idm_ctl_setup_link(struct idm_node *src, struct idm_node *dst, int flag)
{
    int ret;
    ret = idm_ctl_match_link(src,dst);
    if(0 > ret){
        idm_err("node %s has no link to %s",src->name,dst->name);
        return -1;
    }
    struct media_link *link = &src->me->links[ret];
    ret = media_setup_link(idm_plat.media,link->source,link->sink,flag);
    if(0 > ret)
        idm_err("setup link error, flags 0x%x ",flag);
    return ret;
}

/*
 * enable link from src to dst
 */
int idm_ctl_enable_link(struct idm_node *src, struct idm_node *dst)
{
    return idm_ctl_setup_link(src,dst,MEDIA_LNK_FL_ENABLED);
}

/*
 * disable link from src to dst
 */
int idm_ctl_disable_link(struct idm_node *src, struct idm_node *dst)
{
    return idm_ctl_setup_link(src,dst,0);
}

/* 
 * enable/disable by idm device id
 */
int idm_ctl_enable_link_by_id(enum idm_dev_id sid, enum idm_dev_id did)
{
    return idm_ctl_enable_link(idm_ctl_find_node_by_id(sid), idm_ctl_find_node_by_id(did));
}

int idm_ctl_disable_link_by_id(enum idm_dev_id sid, enum idm_dev_id did)
{
    return idm_ctl_disable_link(idm_ctl_find_node_by_id(sid), idm_ctl_find_node_by_id(did));
}

/* 
 * set format & crop
 * args: fmts, format arrays; n number of formats
 */ 
/*
   static int idm_ctl_set_fmt(int fd, struct v4l2_subdev_format *fmt)
   {
   return ioctl(fd, VIDIOC_SUBDEV_S_FMT, &fmt);
   }

   static int idm_ctl_set_crop(int fd, struct v4l2_subdev_selection *sel)
   {
   return ioctl(fd, VIDIOC_SUBDEV_S_SELECTION, &sel);
   }
   */
int idm_ctl_set_fmts(struct idm_fmt_crop *fmts, int n)
{
    int i,fd,ret;
    struct idm_node *node;
    struct idm_fmt_crop *fmt;

    for(i=0; i<n; i++){
        fmt = &fmts[i];
        node = idm_ctl_find_node_by_id(fmt->dev_id);
        if(NULL == node){
            idm_err("node for fmts[%d].dev_id=%d not found",i,fmt->dev_id);
            break;
        }
        fd = idm_ctl_node_open(node);
        if(0 > fd)
            break;
        if(fmt->ops & IDM_APPLY_FMT){
            struct v4l2_subdev_format format= {
                .pad    = fmt->pad,
                .which  = fmt->which,
                .format = {
                    .width  = fmt->width,
                    .height = fmt->height,
                    .code   = fmt->code,
                },
            };
            ret = ioctl(fd, VIDIOC_SUBDEV_S_FMT, &format);
            if(ret < 0) {
                idm_err("failed to set format '%s': %s", node->me->devname, strerror(errno));
                return -errno;
            }
        }
        if(fmt->ops & IDM_APPLY_CROP){
            struct v4l2_subdev_selection selection = {
                .which  = fmt->which,
                .pad    = fmt->pad,
                .r  = fmt->crop_wnd,
            };
            ret = ioctl(fd, VIDIOC_SUBDEV_S_SELECTION, &selection);
            if(ret < 0) {
                idm_err("failed to set crop '%s': %s", node->me->devname, strerror(errno));
                return -errno;
            }
        }
    }
    return 0;
}

/* 
 * do private ioctls, only for isp sub device
 */
int idm_ctl_private_cmd(struct idm_node *node,int cmd, struct idm_topology *tp)
{
    int i, ret = 0, fd;

    fd = idm_ctl_node_open(node);
    if (fd < 0)
        return fd;
    ret = ioctl(fd, cmd, tp);
    idm_info("%s ioctl,private ioctl\n",node->name);
    if (ret < 0) {
        idm_err("failed to trigger private ioctl for '%s' : %s", node->name, strerror(errno));
        ret = -errno;
        goto exit;
    }

exit:
    idm_ctl_node_close(node);
    return ret;
}

/* 
 * trigger snapshot ioctl
 */
int idm_ctl_trig_snapshot(enum idm_dev_id id)
{
    struct idm_node *node = idm_ctl_find_node_by_id(id);
    return idm_ctl_private_cmd(node,VIDIOC_PRIVATE_AQUILAV1ISP_TOPOLOGY_SNAPSHOT,NULL);
}

/* 
 * exit idm control modlue
 */
int idm_ctl_exit(void)
{
    if(idm_plat.media)
        media_close(idm_plat.media);
    return 0;
}
