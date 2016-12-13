/******************************************************************************
 * (C) Copyright [2016 - 2017] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/


#ifndef _CAMERA_DRIVER_INTERFACE_H_
#define _CAMERA_DRIVER_INTERFACE_H_


#include "camera_core.h"
#include "sys/poll.h"

#ifdef __cplusplus
    extern "C" {
#endif /* extern "C" */


/* ------------------------------------------------------------------------ */

typedef enum {
    CC_ISP_BUFFER_OK,
    CC_ISP_BUFFER_CANCEL,
} CCISPBuffStatus;

typedef struct {
    CCImageInfo common;
    int32_t zoomFlag;
    int32_t cropFlag;

    int32_t fd;
} CCISPBuffInfo;

typedef struct
{
    CCImageInfo common;
    int32_t zoomFlag;
    int32_t cropFlag;

    int32_t frameId;
    uint8_t* addr;
    CCISPBuffStatus buffStatus;
} CCISPImageBuffer;


typedef struct {
    uint32_t inputNum;
    CCISPBuffInfo inputBuffConfig[MAX_CONNECTIONS_PER_NODE];

    uint32_t outputNum;
    CCISPBuffInfo outputBuffConfig[MAX_CONNECTIONS_PER_NODE];

    uint32_t tmpNum;
    CCISPBuffInfo tmpBuffConfig[MAX_CONNECTIONS_PER_NODE];

    uint32_t zoomRatio;
} CCISPProcessPara;

typedef struct {
    CCISPProcessPara processConfig[MAX_NUM_PER_PROCESS_ID];
    uint32_t totalProcessNum;
    CCProcessNodeType processType;
    uint32_t bufDepth; //buffer queue size of every video device
} CCISPConfig;

typedef struct {
    CCHandle commonHandle;

    void* privateHandle;
} CCISPHandle;

CCError CCISPInit(CCHandle** ispHandle);

CCError CCISPDeInit(CCHandle* ispHandle);

CCError CCISPConfigContext(CCHandle* ispHandle, CCISPConfig* processConfig, int processNum);

CCError CCISPGetDeviceSource(CCHandle* ispHandle, CCISPConfig* config);

CCError CCISPProcessOn(CCHandle* ispHandle, CCProcessNodeType processType, CCISPProcessPara* para);

CCError CCISPProcessOff(CCHandle* ispHandle, CCProcessNodeType processType, CCISPProcessPara* para);

CCError CCISPV4l2Poll(CCHandle* ispHandle, struct pollfd fds[], int numFd, int timeout, int* result);

CCError CCISPV4l2Enqueue(CCHandle* ispHandle, int fd, CCISPImageBuffer* imgBuf);

CCError CCISPV4l2Dequeue(CCHandle* ispHandle, int fd, CCISPImageBuffer* imgBuf);

CCError CCISPV4l2Flush(CCHandle* ispHandle, int fd);

CCError CCISPV4l2Config(CCHandle* ispHandle, int fd, CCISPBuffInfo* buffConfig);


/* ------------------------------------------------------------------------ */


#ifdef __cplusplus
    }
#endif /* extern "C" */


#endif /* _CAMERA_DRIVER_INTERFACE_H_ */
