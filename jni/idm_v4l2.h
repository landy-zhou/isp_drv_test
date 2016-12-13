/******************************************************************************
 * V4L2 camera driver access wrapper
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/

#ifndef __IDM_V4L2_H__
#define __IDM_V4L2_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/poll.h>
#include "linux/videodev2.h"

#define BUILD_OPTION_DEBUG_DUMP_V4L2_CALLING 1

int __idm_ioctl( int device, int cmd, void* data,
               const char *str_device, const char *str_cmd, const char *str_data,
               const char *file, int line );
int __idm_open( const char *dev, int flag, const char *str_dev, const char *str_flag, const char *file, int line );
int __idm_close( int dev, const char *str_dev, const char *file, int line );
int __idm_poll( struct pollfd *ufds, unsigned int nfds, int timeout,
              const char *str_ufds, const char *str_nfds, const char *str_timeout, const char *file, int line );

#define idm_open(a, b)       __idm_open( a, b, a, #b, __FILE__, __LINE__ )
#define idm_close(a)         __idm_close( a, #a, __FILE__, __LINE__ )
#define idm_ioctl(a, b, c)   __idm_ioctl( a, b, c, #a, #b, #c, __FILE__, __LINE__ )
#define idm_poll(a, b, c)    __idm_poll( a, b, c, #a, #b, #c, __FILE__, __LINE__ )


#ifdef __cplusplus
}
#endif

#endif // __IDM_V4L2_H__

