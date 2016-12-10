/******************************************************************************
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/


#include "idm_interface.h"

CCError CCISPInit(CCISPState** ispState){ }

CCError CCISPDeInit(CCISPState* ispState){ }

CCError CCISPConfigContext(CCISPState* ispState, CCISPConfig* processConfig, int processNum){ }

CCError CCISPGetDeviceSource(CCISPState* ispState, CCISPConfig* config){ }

CCError CCISPProcessOn(CCISPState* ispState, CCProcessNodeType processId, CCISPProcessPara* para){ }

CCError CCISPProcessOff(CCISPState* ispState, CCProcessNodeType processId, CCISPProcessPara* para){ }

CCError CCISPV4l2Poll(CCISPState* ispState, int fd, int timeout, int* result){ }

CCError CCISPV4l2Enqueue(CCISPState* ispState, int fd, CCISPImageBuffer* imgBuf){ }

CCError CCISPV4l2Dequeue(CCISPState* ispState, int fd, CCISPImageBuffer** imgBuf){ }

CCError CCISPV4l2Flush(CCISPState* ispState, int fd){ }

CCError CCISPV4l2Config(CCISPState* ispState, int fd, CCISPBuffInfo* buffConfig){ }

