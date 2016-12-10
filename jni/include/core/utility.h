#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

#define ASSERT( v )\
if ( !(v) )\
{\
    printf( "assertion failed: '%s' at File %s, Line %d\n", #v, __FILE__, __LINE__ );\
    exit( -1 );\
}

#define ZERO_FILL_POINTER(v) \
    do { \
        (void)memset(v, 0, sizeof(*v)); \
    } while (0)

#define MALLOC_POINTER(v) \
    do { \
        v=(typeof(*v)*)malloc(sizeof(*v)); \
    } while (0)

////////////////////////////////////////////////////
// queue implemented with Doubly Link List
struct _CCLinkNode;
typedef struct _CCLinkNode {
    void *element;
    struct _CCLinkNode *prior;
    struct _CCLinkNode *next;
} CCLinkNode;

typedef struct {
    CCLinkNode *head;
    CCLinkNode *tail;
    CCLinkNode *cache;
    int num;
    int magic;
} CCLinkList;

void initList(CCLinkList *list);
void deinitList(CCLinkList *list);
int getListSize(CCLinkList *list);
int searchList(CCLinkList *list, void *item);
void* getListNextItem(CCLinkList *list, void* item); // return the element after *item; and return list head if item is NULL
int appendList(CCLinkList *list, void* item); // append an element at the end of the list
int removeList(CCLinkList *list, void* item); // remove item

typedef struct {
    CCLinkNode *head;
    CCLinkNode *tail;
    int num;
    int magic;
} CCQueue;

void initQueue(CCQueue *queue);
void deinitQueue(CCQueue *queue);
int getQueueSize(CCQueue *queue);
int enqueue(CCQueue *queue, void* item);
int dequeue(CCQueue *queue, void** item);

typedef struct
{
    int msgId;
} CCThreadMsg;

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    CCQueue         queue;
} CCMsgQueue;

int CCThreadMsgQueueInit( CCMsgQueue *msgQueue );
int CCThreadMsgQueueDeinit( CCMsgQueue *msgQueue );
int CCThreadMsgSend( CCMsgQueue *msgQueue, CCThreadMsg *msg );
int CCThreadMsgWait( CCMsgQueue *msgQueue, int timeoutMs );
int CCThreadMsgAvailable( CCMsgQueue *msgQueue );
int CCThreadMsgReceive( CCMsgQueue *msgQueue, CCThreadMsg **msg );

typedef void *(*CCThreadFunc)( void *param );
typedef struct {
    CCThreadFunc    func;
    pthread_t       tid;
    char            name[16];
    CCMsgQueue      msgQueue;
} CCThread;

int CCThreadCreate( CCThread *thread, CCThreadFunc func, char *threadName);
int CCThreadWaitExit( CCThread *thread );
int CCThreadDestroy( CCThread *thread );

////////////////////////////////////////////////////

#endif
