/******************************************************************************
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/

#include "idm_control.h"

struct idm_platform idm_plat;

struct node_code_map node_code_table[]={
    { SDCODE_SENSOR0,			V5628_SENSOR_OV8856_NAME},
    { SDCODE_SENSOR1,			NULL},
    { SDCODE_VCM0,			    V5628_VCM_PDRV_NAME},
    { SDCODE_VCM1,			    NULL},
    { SDCODE_FLASH0,			V5628_FLASH_PDRV_NAME},
    { SDCODE_FLASH1,			NULL},
    { SDCODE_CCICV2_CSI0,		V5628_CCIC_CSI0_NAME},
    { SDCODE_CCICV2_CSI1,		NULL},
    { SDCODE_CCICV2_DMA0,		V5628_CCIC_DMA_NAME},
    { SDCODE_CCICV2_DMA1,		NULL},
    { SDCODE_ISP_IDI1,			V5628_IDI1_NAME},
    { SDCODE_ISP_IDI2,			V5628_IDI2_NAME},
    { SDCODE_ISP_PIPE1,			V5628_PATH_YUV_1_NAME},
    { SDCODE_ISP_DUMP1,			V5628_PATH_RAW_1_NAME},
    { SDCODE_ISP_MS1,			V5628_PATH_M2M_1_NAME},
    { SDCODE_ISP_PIPE2,			V5628_PATH_YUV_2_NAME},
    { SDCODE_ISP_DUMP2,			V5628_PATH_RAW_2_NAME},
    { SDCODE_ISP_MS2,			V5628_PATH_M2M_2_NAME},
    { SDCODE_ISP_HS,			V5628_PATH_HIGHSPEED_NAME},
    { SDCODE_ISP_HDR,			V5628_PATH_HDRPROCESS_NAME},
    { SDCODE_ISP_3D,			V5628_PATH_3DSTEREO_NAME},
    { SDCODE_ISP_YUVOFFLINE1,		V5628_PATH_YUVOFFLINE1_NAME},
    { SDCODE_ISP_YUVOFFLINE2,		V5628_PATH_YUVOFFLINE2_NAME},
    { SDCODE_ISP_PDNS,			V5628_PATH_PDNS_NAME},
    { SDCODE_ISP_A1W1,			V5628_OUTPUT_1_NAME},
    { SDCODE_ISP_A1W2,			V5628_OUTPUT_2_NAME},
    { SDCODE_ISP_A1W3,			V5628_OUTPUT_3_NAME},
    { SDCODE_ISP_A1W4,			V5628_OUTPUT_4_NAME},
    { SDCODE_ISP_A1W5,			V5628_OUTPUT_5_NAME},
    { SDCODE_ISP_A1W6,			V5628_OUTPUT_6_NAME},
    { SDCODE_ISP_A1R1,			V5628_INPUT_1_NAME},
    { SDCODE_ISP_A1R2,			V5628_INPUT_2_NAME},
    { SDCODE_ISP_A1R3,			V5628_INPUT_3_NAME},
    { SDCODE_ISP_A1R4,			V5628_INPUT_4_NAME},
    { SDCODE_ISP_A2W1,			V5628_OUTPUT_7_NAME},
    { SDCODE_ISP_A2W2,			V5628_OUTPUT_8_NAME},
    { SDCODE_ISP_A2R1,			V5628_INPUT_5_NAME},
    { SDCODE_ISP_A2R2,			V5628_INPUT_6_NAME},
    { VNODE_ISP_A1W1,			VDEV_OUTPUT_1_NAME},
    { VNODE_ISP_A1W2,			VDEV_OUTPUT_2_NAME},
    { VNODE_ISP_A1W3,			VDEV_OUTPUT_3_NAME},
    { VNODE_ISP_A1W4,			VDEV_OUTPUT_4_NAME},
    { VNODE_ISP_A1W5,			VDEV_OUTPUT_5_NAME},
    { VNODE_ISP_A1W6,			VDEV_OUTPUT_6_NAME},
    { VNODE_ISP_A1R1,			VDEV_INPUT_1_NAME},
    { VNODE_ISP_A1R2,			VDEV_INPUT_2_NAME},
    { VNODE_ISP_A1R3,			VDEV_INPUT_3_NAME},
    { VNODE_ISP_A1R4,			VDEV_INPUT_4_NAME},
    { VNODE_ISP_A2W1,			VDEV_OUTPUT_7_NAME},
    { VNODE_ISP_A2W2,			VDEV_OUTPUT_8_NAME},
    { VNODE_ISP_A2R1,			VDEV_INPUT_5_NAME},
    { VNODE_ISP_A2R2,			VDEV_INPUT_6_NAME}
};

/* node->me field must be set before */
static int idm_find_node_type(struct idm_platform *plat, struct idm_node *node)
{
    enum idm_node_type ret=-1;
}

int idm_node_add(struct idm_platform *plat, struct media_entity *me, int id)
{
    struct idm_node *node = calloc(1, sizeof(struct idm_node));

    if (NULL == node)
        return -1;
    node->me = me;
    node->plat = plat;
    strcpy(node->name, me->info.name);
    if(0 > idm_find_node_type(&idm_plat,node))
    {
        idm_err("failed to find node type");
        return -1;
    }

    //node->id = id;
    //SetContainer(me, node);
    plat->nodes[id] = node;
    plat->node_num++;
    return 0;
}

int idm_node_open(struct idm_platform *plat, struct idm_node *node)
{}

int idm_ctrl_init(void)
{
    //todo: gen all media device/sub device/video device info
    //todo:set all pad info
    
    /* Open media device */
    idm_plat.media = media_open("/dev/media0");
    if (NULL == idm_plat.media) {
        idm_err("failed to open media node");
        return -1;
    }

    /* init debug function */
    media_debug_set_handler(idm_plat.media,
			(void (*)(void *, ...))fprintf, stdout);


    /* enum entities Add node to idm for each media_entity */
    int i;
    for (i = 0; i < IDM_NODE_MAX_CNT; i++) {
        struct node_code_map  *node_map = &node_code_table[i];
        if(NULL == node_map->name)
            continue;
        struct media_entity *me = media_get_entity_by_name(idm_plat.media,node_map->name,sizeof(node_map->name));
        if (NULL == me)
            break;
        idm_info("Find media_entity: %s",me->info.name);
        if(0 > idm_node_add(&idm_plat, me, node_map->code))
            goto out;
    }
#if 0
    /* Setup each node for the dst and links */
    for (i = 0; i < idm_plat.NrME; i++) {
        if (idm_plat.node[i]) {
            ret = CamNodeSetup(idm_plat.node[i]);
            if (ret < 0)
                goto out;
#if 1
            int j;
            for (j = 0; j < idm_plat.node[i]->NrLink; j++) {
                struct CamLink *link = idm_plat.node[i]->link[j];
                app_info("%s link%d: '%s'=>'%s'", idm_plat.node[i]->name, j, link->src->name, link->dst->name);
            }
#endif
        }
    }

    /* Identify node types among all Nodes */
    ret = MediaLibFindSource(cam); //sensors or offline input
    if (ret < 0)
        goto out;

    ret = MediaLibFindPath(cam); //isp pipeline
    if (ret < 0)
        goto out;

    ret = MediaLibFindDst(cam); //output video device
    if (ret < 0)
        goto out;

    ret = MediaLibFindCCIC(cam); //ccic
    if (ret < 0)
        goto out;

    ret = MediaLibFindIDI(cam); //idi 
    if (ret < 0)
        goto out;

    ret = MediaLibFindOutport(cam); //axi output port
    if (ret < 0)
        goto out;
#endif
out:
    media_close(idm_plat.media);

}

int idm_ctrl_setup_link()
{

}



