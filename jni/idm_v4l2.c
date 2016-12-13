/******************************************************************************
 * V4L2 camera driver access wrapper
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "idm_utils.h"

int __idm_ioctl( int device, int cmd, void *data, const char *str_device, 
        const char *str_cmd, const char *str_data,const char *file, int line )
{
	int ret = 0;
	idm_tick t1  = 0;

#if defined( BUILD_OPTION_DEBUG_DUMP_V4L2_CALLING )
	idm_info( "%s - %d:\n ioctl(%s, %s, %s)\n", file, line, str_device, str_cmd, str_data );
	idm_info( "%s = %d\n", str_device, device );
	idm_info( "%s = 0x%x\n", str_data, (unsigned int)data );

	switch ( cmd )
	{
	case VIDIOC_S_INPUT:
		idm_info( "*(int*)(%s) = %d\n", str_data, *(int*)data );
		break;

	case VIDIOC_S_PARM:
		idm_info( "*(struct v4l2_streamparm*)(%s)           = {\n", str_data );
		idm_info( "\t.type                                  = %d,\n", ((struct v4l2_streamparm*)data)->type );
//		idm_info( "\t.parm.capture.capability               = %d,\n", ((struct v4l2_streamparm*)data)->parm.capture.capability );
		idm_info( "\t.parm.capture.capturemode              = %d,\n", ((struct v4l2_streamparm*)data)->parm.capture.capturemode );
		idm_info( "\t.parm.capture.timeperframe.numerator   = %d,\n", ((struct v4l2_streamparm*)data)->parm.capture.timeperframe.numerator );
		idm_info( "\t.parm.capture.timeperframe.denominator = %d,\n", ((struct v4l2_streamparm*)data)->parm.capture.timeperframe.denominator );
//		idm_info( "\t.parm.capture.extendedmode             = %d,\n", ((struct v4l2_streamparm*)data)->parm.capture.extendedmode );
//		idm_info( "\t.parm.capture.readbuffers              = %d,\n", ((struct v4l2_streamparm*)data)->parm.capture.readbuffers );
		idm_info( "}\n" );
		break;

	case VIDIOC_S_FMT:
        int is_multi_plane = 0;
        int type = ((struct v4l2_format*)data)->type;
        if((V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE == type) || (V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE == type))
            is_multi_plane = 1;

		idm_info( "*(struct v4l2_format*)(%s) = {\n", str_data );
		idm_info( "\t.type                    = %d,\n", ((struct v4l2_format*)data)->type );
		if ( is_multi_plane )
		{
			idm_info( "\t.fmt.pix_mp.pixelformat = 0x%x,\n", ((struct v4l2_format*)data)->fmt.pix_mp.pixelformat );
			idm_info( "\t.fmt.pix_mp.width       = %d,\n", ((struct v4l2_format*)data)->fmt.pix_mp.width );
			idm_info( "\t.fmt.pix_mp.height      = %d,\n", ((struct v4l2_format*)data)->fmt.pix_mp.height );
			idm_info( "}\n" );
		}
		else
		{
			idm_info( "\t.fmt.pix.pixelformat     = 0x%x,\n", ((struct v4l2_format*)data)->fmt.pix.pixelformat );
			idm_info( "\t.fmt.pix.width           = %d,\n", ((struct v4l2_format*)data)->fmt.pix.width );
			idm_info( "\t.fmt.pix.height          = %d,\n", ((struct v4l2_format*)data)->fmt.pix.height );
			idm_info( "}\n" );
		}
		break;

	case VIDIOC_REQBUFS:
		idm_info( "*(struct v4l2_requestbuffers*)(%s) = {\n", str_data );
		idm_info( "\t.type   = %d,\n", ((struct v4l2_requestbuffers*)data)->type );
		idm_info( "\t.count  = %d,\n", ((struct v4l2_requestbuffers*)data)->count );
		idm_info( "\t.memory = %d,\n", ((struct v4l2_requestbuffers*)data)->memory );
		idm_info( "}\n" );
		break;

	case VIDIOC_STREAMON:
		idm_info( "*(int*)(%s) = %d\n", str_data, *(int*)data );
		break;

	case VIDIOC_STREAMOFF:
		idm_info( "*(int*)(%s) = %d\n", str_data, *(int*)data );
		break;

	case VIDIOC_QBUF:
		idm_info( "*(struct v4l2_buffer*)(%s) = {\n", str_data );
		idm_info( "\t.type      = %d,\n", ((struct v4l2_buffer*)data)->type );
		idm_info( "\t.index     = %d,\n", ((struct v4l2_buffer*)data)->index );
		idm_info( "\t.bytesused = %d,\n", ((struct v4l2_buffer*)data)->bytesused );
		idm_info( "\t.flags     = %d,\n", ((struct v4l2_buffer*)data)->flags );
		idm_info( "\t.field     = %d,\n", ((struct v4l2_buffer*)data)->field );
		idm_info( "}\n" );
		break;

	case VIDIOC_DQBUF:
		idm_info( "Before Dequeue:\n" );
		idm_info( "*(struct v4l2_buffer*)(%s) = {\n", str_data );
		idm_info( "\t.type      = %d,\n", ((struct v4l2_buffer*)data)->type );
		idm_info( "\t.index     = %d,\n", ((struct v4l2_buffer*)data)->index );
		idm_info( "\t.bytesused = %d,\n", ((struct v4l2_buffer*)data)->bytesused );
		idm_info( "\t.flags     = %d,\n", ((struct v4l2_buffer*)data)->flags );
		idm_info( "\t.field     = %d,\n", ((struct v4l2_buffer*)data)->field );
		break;

	default:
		break;
	}
	t1 = -idm_get_tick_count();

#endif

	ret = ioctl( device, cmd, data );

#if defined( BUILD_OPTION_DEBUG_DUMP_V4L2_CALLING )
	t1 += idm_get_tick_count();

	idm_info( "Return value: %d\n", ret );
	if ( ret != 0 )
	{
		idm_info( "Error code: %s\n", strerror( errno ) );
	}
	switch ( cmd )
	{
	case VIDIOC_DQBUF:
		idm_info( "After Dequeue:\n" );
		idm_info( "*(struct v4l2_buffer*)(%s) = {\n", str_data );
		idm_info( "\t.type      = %d,\n", ((struct v4l2_buffer*)data)->type );
		idm_info( "\t.index     = %d,\n", ((struct v4l2_buffer*)data)->index );
		idm_info( "\t.bytesused = %d,\n", ((struct v4l2_buffer*)data)->bytesused );
		idm_info( "\t.flags     = %d,\n", ((struct v4l2_buffer*)data)->flags );
		idm_info( "\t.field     = %d,\n", ((struct v4l2_buffer*)data)->field );
		idm_info( "}\n" );
		break;

	default:
		break;
	}
	idm_info( "Perf: ioctl elapse: %.2f ms\n", t1 / 1000.0 );
	idm_info( "\n\n" );
#endif

	return ret;
}

int __idm_open( const char *dev, int flag, const char *str_dev, const char *str_flag, const char *file, int line )
{
	int ret = 0;
	idm_tick t1  = 0;

#if defined( BUILD_OPTION_DEBUG_DUMP_V4L2_CALLING )
	idm_info( "%s - %d:\n open(\"%s\", %s)\n", file, line, str_dev, str_flag );
	t1 = -idm_get_tick_count();
#endif

	ret = open( dev, flag );

#if defined( BUILD_OPTION_DEBUG_DUMP_V4L2_CALLING )
	t1 += idm_get_tick_count();
	idm_info( "Return value: %d\n", ret );
	if ( ret != 0 )
	{
		idm_info( "Error code: %s\n", strerror( errno ) );
	}
	idm_info( "Perf: open elapse: %.2f ms\n", t1 / 1000.0 );
	idm_info( "\n\n" );
#endif

	return ret;
}

int __idm_close( int dev, const char *str_dev, const char *file, int line )
{
	int ret = 0;
	idm_tick t1  = 0;

#if defined( BUILD_OPTION_DEBUG_DUMP_V4L2_CALLING )
	idm_info( "%s - %d:\n close(%s)\n", file, line, str_dev );
	t1 = -idm_get_tick_count();
#endif

	ret = close( dev );

#if defined( BUILD_OPTION_DEBUG_DUMP_V4L2_CALLING )
	t1 += idm_get_tick_count();
	idm_info( "Return value: %d\n", ret );
	if ( ret != 0 )
	{
		idm_info( "Error code: %s\n", strerror( errno ) );
	}
	idm_info( "Elapse: %.2f ms\n", t1 / 1000.0 );
	idm_info( "\n\n" );
#endif

	return ret;
}

int __idm_poll( struct pollfd *ufds, unsigned int nfds, int timeout,const char *str_ufds, 
        const char *str_nfds, const char *str_timeout, const char *file, int line )
{
	int ret = 0;
	idm_tick t1  = 0;

#if defined( BUILD_OPTION_DEBUG_DUMP_V4L2_CALLING )
	idm_info( "%s - %d:\n poll(%s, %s, %s) \n", file, line, str_ufds, str_nfds, str_timeout );
	t1 = -idm_get_tick_count();
#endif

	ret = poll( ufds, nfds, timeout );

#if defined( BUILD_OPTION_DEBUG_DUMP_V4L2_CALLING )
	t1 += idm_get_tick_count();
	idm_info( "Return value: %d\n", ret );
	if ( ret != 0 )
	{
		idm_info( "Error code: %s\n", strerror( errno ) );
	}
	idm_info( "Elapse: %.2f ms\n", t1 / 1000.0 );
	idm_info( "\n\n" );
#endif

	return ret;
}
