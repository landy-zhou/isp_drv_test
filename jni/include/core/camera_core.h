/******************************************************************************
 * (C) Copyright [2016 - 2017] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/


#ifndef _CAMERA_CORE_H_
#define _CAMERA_CORE_H_


#include "camera_base.h"
#include "utility.h"


#ifdef __cplusplus
    extern "C" {
#endif /* extern "C" */


////////////////////////////////////////////////////
// context & image buffer

#define MAX_NODES_PER_CONTEXT 32
#define MAX_CONNECTIONS_PER_NODE 5
typedef enum {
    CC_BUFFER_NODE_TYPE_NULL = 0, // initialization value: this item is NULL!
    CC_BUFFER_NODE_TYPE_INTERNAL, // allocated by core
    CC_BUFFER_NODE_TYPE_INTERNAL_PHYSICAL, // physical memory reserved by core
    CC_BUFFER_NODE_TYPE_EXTERNAL, // allocated by upper layer
} CCBufferNodeType;

typedef enum {
    CC_PROCESS_NODE_TYPE_NULL = 0, // initialization value: this item is NULL!
    CC_PROCESS_NODE_TYPE_ONLINE, // automatical process
    CC_PROCESS_NODE_TYPE_REPROCESS, // process done by upper layer rather than core
    CC_PROCESS_NODE_TYPE_OFFLINE_YUV,
    CC_PROCESS_NODE_TYPE_OFFLINE_PDNS,
    CC_PROCESS_NODE_TYPE_OFFLINE_RAW,
    CC_PROCESS_NODE_TYPE_ACCEL_DSP,
    CC_PROCESS_NODE_TYPE_CPU_METADATA_PARSE,
    CC_PROCESS_NODE_TYPE_CPU_4A_MONITOR,
    CC_PROCESS_NODE_TYPE_CPU_3RD_LIB,
    CC_PROCESS_NODE_TYPE_DELIVER,
    CC_PROCESS_NODE_TYPE_STORE,
    CC_PROCESS_NODE_TYPE_FETCH,
    CC_PROCESS_NODE_TYPE_NUM,
} CCProcessNodeType;

typedef enum {
    CC_BUFFER_STATE_NULL = 0,
    CC_BUFFER_STATE_IDLE, // yet to be sent to modules
    CC_BUFFER_STATE_PENDING, // pending on process
    CC_BUFFER_STATE_READY, // ready for next process
} CCBufferState;

struct _CCProcessNode;
struct _CCBufferNode;
struct _CCContext;
struct _CCImageBuffer;
struct _CCHandle;
struct _CCProcessConfigParam;
struct _CCThreadMsgBuffer;
struct _CCProcessRequest;
struct _CCIppFuncTable;

// module APIs
typedef CCError (*CCIppInitFunc)(struct _CCHandle **ippHandle);
typedef CCError (*CCIppDeinitFunc)(struct _CCHandle **ippHandle);
typedef CCError (*CCIppConfigureFunc)(struct _CCHandle *ippHandle, struct _CCProcessConfigParam *config);
typedef CCError (*CCIppProcessFunc)(struct _CCHandle *ippHandle, struct _CCProcessRequest *request);
typedef CCError (*CCIppFlushFunc)(struct _CCHandle *ippHandle,CCProcessNodeType type);

typedef struct _CCIppFuncTable {
    CCIppInitFunc      init;
    CCIppDeinitFunc    deinit;
    CCIppConfigureFunc configure;
    CCIppProcessFunc   process;
    CCIppFlushFunc     flush;
} CCIppFuncTable;

typedef struct _CCBufferNode {
    int               streamMask;
    CCQueue           bufferQueue; // CCImageBuffer*
    CCSize            size;
    CCBufferNodeType  type;       // allocated by whom
    int               followingNum;
    struct _CCProcessNode     *followingPathes[MAX_CONNECTIONS_PER_NODE];
} CCBufferNode;

typedef struct _CCProcessNode {
    CCIppProcessFunc     processFunc;
    CCProcessNodeType    type;
    int                  inputNum;
    // inputNodes can point to buffer nodes outside current context ( for example, HDR mode needs 3 still buffers with different frame ids )
    struct _CCBufferNode *inputNodes[MAX_CONNECTIONS_PER_NODE];
    int                  outputNum;
    struct _CCBufferNode *outputNodes[MAX_CONNECTIONS_PER_NODE];
} CCProcessNode;

typedef struct _CCContext{
    CCProcessNode processNodes[MAX_NODES_PER_CONTEXT];
    CCBufferNode  bufferNodes[MAX_NODES_PER_CONTEXT];
    CCProcessNode *streamingRoot;
    CCProcessNode *triggerRoot;
} CCContext;

typedef struct _CCImageBuffer {
    int frameId;
    //CCStreamId streamId;
    int streamMask;
    CCImageInfo imageInfo;
    void* bufferHandle;
    uint8_t* addr;
    struct _CCBufferNode *node;
    struct _CCContext *context;
    CCBufferState state;

    int32_t zoomFlag;
    int32_t cropFlag;
} CCImageBuffer;

////////////////////////////////////////////////////
// process request & msg

typedef int(*CCSendMessage)(struct _CCHandle *handle, CCThreadMsg *msg);

typedef struct _CCHandle {
    struct _CCHandle  *parent;
    CCSendMessage     sendMessage;
} CCHandle;

typedef struct _CCProcessConfigParam {
    CCProcessNodeType    type;
} CCProcessConfigParam;

// a process request sent from core control, the receiver may:
// 1. do light-weight task within the calling thread of core control
// 2. arrange a task to the process thread
#define MAX_PROCESS_BUF_NUM 8
typedef struct _CCProcessRequest {
    int               frameId;
    CCProcessNodeType type;
} CCProcessRequest;

#define __OFFSET_DECLARE(TYPE) CC_THREAD_MSG_TYPE_OFFSET_##TYPE = 0x10000 * CC_PROCESS_NODE_TYPE_##TYPE

typedef enum
{
    // thread msg for buffer, use CCThreadMsgBuffer
    CC_THREAD_MSG_TYPE_ENQUEUE = 0x0,
    CC_THREAD_MSG_TYPE_BUFFER_READY,
    CC_THREAD_MSG_TYPE_BUFFER_RELEASED,
    CC_THREAD_MSG_TYPE_BUFFER_CANCELED,

    // thread msg for stream, use CCThreadMsgStream
    CC_THREAD_MSG_TYPE_CONFIGURE_STREAM,

    // thread msg for process, use CCThreadMsgProcess
    CC_THREAD_MSG_TYPE_PROCESS_TASK,

    //to be defined for error handle
    CC_THREAD_MSG_TYPE_ERROR_ISP_RECOVER,

    // to be defined
    CC_THREAD_MSG_TYPE_REPORT_ERROR,
    CC_THREAD_MSG_TYPE_FLUSH_TASK,

    // use default CCThreadMsg
    CC_THREAD_MSG_TYPE_SEAMLESS_SWITCH_READY,
    CC_THREAD_MSG_TYPE_TRIGGER_EXIT,

    // offset for module specific msg type
    __OFFSET_DECLARE(ONLINE),
    __OFFSET_DECLARE(REPROCESS),
    __OFFSET_DECLARE(OFFLINE_YUV),
    __OFFSET_DECLARE(OFFLINE_PDNS),
    __OFFSET_DECLARE(OFFLINE_RAW),
    __OFFSET_DECLARE(ACCEL_DSP),
    __OFFSET_DECLARE(CPU_METADATA_PARSE),
    __OFFSET_DECLARE(CPU_4A_MONITOR),
    __OFFSET_DECLARE(CPU_3RD_LIB),
    __OFFSET_DECLARE(DELIVER),
    __OFFSET_DECLARE(STORE),
    __OFFSET_DECLARE(FETCH),
} CCThreadMsgType;

typedef struct _CCThreadMsgBuffer {
    CCThreadMsg   common;
    CCImageBuffer *buffer;
} CCThreadMsgBuffer;

typedef struct _CCThreadMsgStream {
    CCThreadMsg   common;
    CCStreamId streamId;
    CCImageInfo config;
} CCThreadMsgStream;

typedef struct _CCThreadMsgProcess {
    CCThreadMsg   common;
    CCProcessRequest *request;
} CCThreadMsgProcess;

typedef struct _CCCoreMetadata {
    int frameId;
    CCFrameMetadataParam metadataParam;
} CCCoreMetadata;

////////////////////////////////////////////////////


#ifdef __cplusplus
    }
#endif /* extern "C" */


#endif /* _CAMERA_CORE_H_ */
