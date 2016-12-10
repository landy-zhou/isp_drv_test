/******************************************************************************
 * (C) Copyright [2016 - 2017] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/


#ifndef _CAMERA_DRIVER_INTERFACE_H_
#define _CAMERA_DRIVER_INTERFACE_H_


#include "include/camera_core.h"


#ifdef __cplusplus
    extern "C" {
#endif /* extern "C" */


/* ------------------------------------------------------------------------ */


typedef struct {
    CCImageFormat format;
    CCSize size;
    int fd;
    int zoomRatio;
    int cropFlag;
} CCISPBuffInfo;

typedef struct
{
    int frameId;
    CCSize size;
    CCImageFormat format;
    uint8_t* addr;
} CCISPImageBuffer;


typedef struct {
    int inputNum;
    CCISPBuffInfo inputBuffConfig[MAX_CONNECTIONS_PER_NODE];

    int outputNum;
    CCISPBuffInfo outputBuffConfig[MAX_CONNECTIONS_PER_NODE];

    int tmpNum;
    CCISPBuffInfo tmpBuffConfig[MAX_CONNECTIONS_PER_NODE];
} CCISPProcessPara;

typedef struct {
    CCISPProcessPara processConfig[MAX_NUM_PER_PROCESS_ID];
    int totalProcessNum;
    CCProcessNodeType processId;
} CCISPConfig;

typedef struct {

} CCISPState;

CCError CCISPInit(CCISPState** ispState);

CCError CCISPDeInit(CCISPState* ispState);

CCError CCISPConfigContext(CCISPState* ispState, CCISPConfig* processConfig, int processNum);

CCError CCISPGetDeviceSource(CCISPState* ispState, CCISPConfig* config);

CCError CCISPProcessOn(CCISPState* ispState, CCProcessNodeType processId, CCISPProcessPara* para);

CCError CCISPProcessOff(CCISPState* ispState, CCProcessNodeType processId, CCISPProcessPara* para);

CCError CCISPV4l2Poll(CCISPState* ispState, int fd, int timeout, int* result);

CCError CCISPV4l2Enqueue(CCISPState* ispState, int fd, CCISPImageBuffer* imgBuf);

CCError CCISPV4l2Dequeue(CCISPState* ispState, int fd, CCISPImageBuffer** imgBuf);

CCError CCISPV4l2Flush(CCISPState* ispState, int fd);

CCError CCISPV4l2Config(CCISPState* ispState, int fd, CCISPBuffInfo* buffConfig);


/* ------------------------------------------------------------------------ */


#ifdef __cplusplus
    }
#endif /* extern "C" */


#endif /* _CAMERA_DRIVER_INTERFACE_H_ */