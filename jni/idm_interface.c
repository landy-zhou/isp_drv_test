/******************************************************************************
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/

#include "idm_interface.h"
#include "idm_control.h"

CCError CCISPInit(CCHandle** ispHandle)
{ 
    return idm_ctl_init();
}

CCError CCISPDeInit(CCHandle* ispHandle){ }

CCError CCISPConfigContext(CCHandle* ispHandle, CCISPConfig* processConfig, int processNum){ }

CCError CCISPGetDeviceSource(CCHandle* ispHandle, CCISPConfig* config){ }

CCError CCISPProcessOn(CCHandle* ispHandle, CCProcessNodeType processType, CCISPProcessPara* para){ }

CCError CCISPProcessOff(CCHandle* ispHandle, CCProcessNodeType processType, CCISPProcessPara* para){ }

CCError CCISPV4l2Poll(CCHandle* ispHandle, struct pollfd fds[], int numFd, int timeout, int* result){ }

CCError CCISPV4l2Enqueue(CCHandle* ispHandle, int fd, CCISPImageBuffer* imgBuf){ }

CCError CCISPV4l2Dequeue(CCHandle* ispHandle, int fd, CCISPImageBuffer* imgBuf){ }

CCError CCISPV4l2Flush(CCHandle* ispHandle, int fd){ }

CCError CCISPV4l2Config(CCHandle* ispHandle, int fd, CCISPBuffInfo* buffConfig){ }


