/******************************************************************************
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <errno.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "media_lib.h"
#include "camera_api.h"
#include "vbmem_lib.h"
#include "isp_iommu.h"

//#define USE_USERPTR
#define USE_DMABUF
//#define USE_MMAP

#if defined(USE_USERPTR)
// user pointer memory
#define DEFAULT_MEM_TYPE	V4L2_MEMORY_USERPTR

#elif defined(USE_DMABUF)
//dmabuf style
#define DEFAULT_MEM_TYPE	V4L2_MEMORY_DMABUF

#elif defined(USE_MMAP) 
//mmap style
#define DEFAULT_MEM_TYPE	V4L2_MEMORY_MMAP

#endif

#define	V4L2_POLL_TIMEOUT_MS	2000	// -1: never time out
#define ALIGN(x, a) (((a>0) && (x%a)) ? ((x/a+1)*a) : x)

static struct camera_fmt_t FmtMap[] = {
    [FMT_ID_UYVY]	= {"UYVY",	V4L2_PIX_FMT_UYVY,	16},
    [FMT_ID_YV16]	= {"YV16",	V4L2_PIX_FMT_YUV422P,	16},
    [FMT_ID_YU12]	= {"YU12",	V4L2_PIX_FMT_YUV420M,	12},
    [FMT_ID_YV12]	= {"YV12",	V4L2_PIX_FMT_YVU420M,	12},
    [FMT_ID_NV12]	= {"NV12",	V4L2_PIX_FMT_NV12M,	12},
    [FMT_ID_NV21]	= {"NV21",	V4L2_PIX_FMT_NV21M,	12},
    [FMT_ID_JPEG]	= {"JPEG",	V4L2_PIX_FMT_JPEG,	16},
    [FMT_ID_RGBP]	= {"RGBP",	V4L2_PIX_FMT_RGB565,	16},
    [FMT_ID_BA81]	= {"BA81",	V4L2_PIX_FMT_SBGGR8,	16},
    [FMT_ID_GBRG]	= {"GBRG",	V4L2_PIX_FMT_SGBRG8,	8},
    [FMT_ID_GRBG]	= {"GRBG",	V4L2_PIX_FMT_SGRBG8,	8},
    [FMT_ID_RGGB]	= {"RGGB",	V4L2_PIX_FMT_SRGGB8,	8},
    [FMT_ID_BG10]	= {"BG10",	V4L2_PIX_FMT_SBGGR10,	10},
    [FMT_ID_GB10]	= {"GB10",	V4L2_PIX_FMT_SGBRG10,	10},
    [FMT_ID_BA10]	= {"BA10",	V4L2_PIX_FMT_SGRBG10,	10},
    [FMT_ID_RG10]	= {"RG10",	V4L2_PIX_FMT_SRGGB10,	10},
    [FMT_ID_BG12]	= {"BG12",	V4L2_PIX_FMT_SBGGR12,	12},
    [FMT_ID_BG12]	= {"GB12",	V4L2_PIX_FMT_SGBRG12,	12},
    [FMT_ID_BA12]	= {"BA12",	V4L2_PIX_FMT_SGRBG12,	12},
    [FMT_ID_RG12]	= {"RG12",	V4L2_PIX_FMT_SRGGB12,	12},
};

int fill_v4l2_fmt(struct v4l2_format *v4l2_fmt, int fmt_id, __u32 width, __u32 height)
{
    __u32 t;
    __u32 w = width;
    __u32 h = height;
    __u32 f = FmtMap[fmt_id].fourcc;

    if (v4l2_fmt == NULL)
	return -EINVAL;

    t = v4l2_fmt->type;
    memset(v4l2_fmt, 0, sizeof(struct v4l2_format));
    v4l2_fmt->type = t;
    switch (t)
    {
	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
	case V4L2_BUF_TYPE_VIDEO_OUTPUT:
	    v4l2_fmt->fmt.pix.width = w;
	    v4l2_fmt->fmt.pix.height = h;
	    v4l2_fmt->fmt.pix.pixelformat = f;
	    break;
	case V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE:
	    v4l2_fmt->fmt.pix_mp.width = w;
	    v4l2_fmt->fmt.pix_mp.height = h;
	    v4l2_fmt->fmt.pix_mp.pixelformat = f;
	    v4l2_fmt->fmt.pix_mp.num_planes = 2;	// Doesn't matters, driver should overwrite it with the preferred value
	    break;
	case V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE:
	    v4l2_fmt->fmt.pix_mp.width = w;
	    v4l2_fmt->fmt.pix_mp.height = h;
	    v4l2_fmt->fmt.pix_mp.pixelformat = f;
	    v4l2_fmt->fmt.pix_mp.num_planes = 1;	// Doesn't matters, driver should overwrite it with the preferred value
	    break;
	default:
	    c_err("unsupported buffer type");
	    return -EINVAL;
    }
    return 0;
}

int fill_v4l2_buf(struct v4l2_buffer *buf, struct camera_stream_t *pcam, int index, int align)
{
    if (buf == NULL)
	return -1;

    const struct v4l2_format *fmt = &pcam->v4l2_fmt;
    void *baddr = pcam->dsc[index].addr;
    __u32 bsize = pcam->dsc[index].len;
    struct v4l2_plane *plane;
    int i, len = bsize;
    int w = ALIGN(fmt->fmt.pix_mp.width, align);
    int h = ALIGN(fmt->fmt.pix_mp.height, align);

    memset(buf, 0, sizeof(struct v4l2_buffer));
    buf->type	= fmt->type;
    buf->index	= index;
    buf->memory	= DEFAULT_MEM_TYPE;
    switch (buf->type)
    {
	case V4L2_BUF_TYPE_VIDEO_OUTPUT:
	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
	    if (buf->memory == V4L2_MEMORY_USERPTR) {
		buf->m.userptr	= (__u32)baddr;
		buf->length	= bsize;
	    }
	    if (buf->memory == V4L2_MEMORY_DMABUF) {
		buf->m.fd	= pcam->dsc[index].dmabuf_fd;
		buf->length	= bsize;
	    }
	    break;
	case V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE:
	case V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE:
	    plane = pcam->dsc->ImagePlane;
	    for (i = 0; i < fmt->fmt.pix_mp.num_planes-1; i++)
	    {
		int len;
		switch (i) {
		    case 0:
			len = w * h;
			break;
		    case 1:
			len = bsize / 2;
			break;
		    default:
			len = bsize;
		}
		memset(plane + i, 0, sizeof(struct v4l2_plane));
		plane[i].length		= len;
		if (buf->memory == V4L2_MEMORY_USERPTR) {
		    plane[i].m.userptr	= (__u32)baddr;
		    plane[i].data_offset	= 0;
		}
		if (buf->memory == V4L2_MEMORY_DMABUF) {
		    plane[i].m.fd		= pcam->dsc[index].dmabuf_fd;
		    plane[i].data_offset	= baddr - pcam->dsc[index].addr;
		    app_info("DMABUF plane[%d] data_offset %X",i,plane[i].data_offset);
		}
		bsize -= len;
		baddr = (char *)baddr + len;
		app_info("plane[%d],bsize=%d, baddr= %X",i,bsize,baddr);
	    }
	    plane[i].length		= bsize;
	    if (buf->memory == V4L2_MEMORY_USERPTR) {
		plane[i].m.userptr	= (__u32)baddr;
		plane[i].data_offset	= 0;
	    }
	    if (buf->memory == V4L2_MEMORY_DMABUF) {
		plane[i].m.fd		= pcam->dsc[index].dmabuf_fd;
		plane[i].data_offset	= baddr - pcam->dsc[index].addr;
		app_info("DMABUF plane[%d] data_offset %X",i,plane[i].data_offset);
	    }
	    buf->m.planes   = plane;
	    buf->length     = fmt->fmt.pix_mp.num_planes;
	    for(i=0;i<buf->length;i++)
	    {
	    	app_info("dump plane: plane[%d],fd 0x%X, data_offset 0x%X",i,buf->m.planes[i].m.fd,buf->m.planes[i].data_offset );
	    }
	    break;
	default:
	    return -2;
    }
    return 0;
}

int SetStreamFmt(struct camera_stream_t *pstr)
{
    int ret = 0;

    if (pstr == NULL)
	return -EINVAL;

    /* Set up V4L2 format */
    struct v4l2_format StrFmt;
    StrFmt.type = pstr->buf_type;
    ret = fill_v4l2_fmt(&StrFmt, pstr->fmt_id, pstr->width, pstr->height);
    if (ret < 0)
	return ret;
    ret = ioctl(pstr->fd_cam, VIDIOC_S_FMT, &StrFmt);
    app_info("ioctl,VIDIOC_S_FMT\n");
    if (ret  < 0)
    {
	c_err("%s: set format failed: %s", pstr->name, strerror(errno));
	return -errno;
    }
    memcpy(&pstr->v4l2_fmt, &StrFmt, sizeof(struct v4l2_format));
    c_info("%s: select format<%c%c%c%c, w%d, h%d>", pstr->name,
	    fourcc_to_char(FmtMap[pstr->fmt_id].fourcc), pstr->width, pstr->height);

    /* Allocate buffer coherent */
    if (pstr->dsc)
	return -3;
    pstr->dsc = calloc(pstr->NrBuf, sizeof(*pstr->dsc));
    if (pstr->dsc == NULL) {
	c_err("can't alloc camera buffer descriptor");
	return -4;
    }

    /* setup iommu    */
    if(CamIommuInit(pstr->fd_cam, pstr->mmu_en))
    {
	c_err("can't init Iommu:%s", strerror(errno));
	return -1;
    }
		
    /* request driver buffers */
    struct v4l2_requestbuffers buf_req;
    buf_req.type	= pstr->buf_type;
    buf_req.count	= pstr->NrBuf;
    buf_req.memory	= DEFAULT_MEM_TYPE;
    if(ioctl(pstr->fd_cam, VIDIOC_REQBUFS, &buf_req))
    {
	c_err("can't require buffer:%s", strerror(errno));
	return -5;
    }
    app_info("ioctl,VIDIOC_REQBUFS\n");
    if (buf_req.count != pstr->NrBuf)
    {
	c_err("insufficient buffer allocated\n");
	return -6;
    }
    c_info("%d buffer requested", buf_req.count);

    int i, j, w = pstr->width, h = pstr->height;
    int size = w * h * FmtMap[pstr->fmt_id].bpp / 8;
    void *addr = NULL;
    app_info("%s_%s_%dX%d", pstr->name,FmtMap[pstr->fmt_id].name, pstr->width, pstr->height);


    if (pstr->fmt_id == FMT_ID_JPEG && size < 1024*1024)
    {
	c_info("round JPEG buffer size to 1MB");
	size = 1024*1024;
    }

    /* allocate video buffer */
    for (i = 0; i < (int)buf_req.count; i++)
    {
	addr = vb_alloc(size, &pstr->dsc[i].dmabuf_fd);
	if (addr == NULL) {
	    c_err("failed malloc video buffer,%s \n",strerror(errno));
	    ret = -7;
	    goto LABEL_RELEASE_CAM_BUFFER;
	}

	pstr->dsc[i].addr	= addr;
	pstr->dsc[i].len	= size;

	if (pstr->load) {
	    FILE *file = fopen(pstr->InputName, "r");
	    if (file == NULL)
	    {
		c_err("Filed to open input file '%s'", pstr->InputName);
		ret = -1;
		goto LABEL_RELEASE_CAM_BUFFER;
	    }
	    c_info("%s: filling buffer%d with file '%s'...", pstr->name, i, pstr->InputName);
	    fread(addr, size, 1, file);
	    fclose(file);
	} else
	    memset(addr, 0xCD, size);

	struct  v4l2_buffer buffer;
	fill_v4l2_buf(&buffer, pstr, i, 0);
	if (ioctl(pstr->fd_cam, VIDIOC_QBUF, &buffer) < 0)
	{
	    c_err("can't enqueue buffer<%d>:%s", i, strerror(errno));
	    ret = -8;
	    goto LABEL_RELEASE_CAM_BUFFER;
	}
    }
    app_info("ioctl,VIDIOC_QBUF\n");

    /* misc */
    if (pstr->save) {
	sprintf(pstr->prefix, "%s_%s_%dX%d", pstr->name,
		FmtMap[pstr->fmt_id].name, pstr->width, pstr->height);
	switch(pstr->fmt_id) {
	    case FMT_ID_UYVY:
	    case FMT_ID_YV16:
	    case FMT_ID_YU12:
	    case FMT_ID_YV12:
	    case FMT_ID_NV12:
	    case FMT_ID_NV21:
		sprintf(pstr->postfix, ".yuv");
		break;
	    case FMT_ID_JPEG:
		sprintf(pstr->postfix, ".jpg");
		break;
	    case FMT_ID_RGBP:
		sprintf(pstr->postfix, ".rgb");
		break;
	    case FMT_ID_BA81:
	    case FMT_ID_GBRG:
	    case FMT_ID_GRBG:
	    case FMT_ID_RGGB:
	    case FMT_ID_BG10:
	    case FMT_ID_GB10:
	    case FMT_ID_BA10:
	    case FMT_ID_RG10:
	    case FMT_ID_BG12:
	    case FMT_ID_GB12:
	    case FMT_ID_BA12:
	    case FMT_ID_RG12:
		sprintf(pstr->postfix, ".raw");
		break;
	}
    }
    return 0;

LABEL_RELEASE_CAM_BUFFER:
    for (j = 0; j < i; j++) {
	if (pstr->dsc[j].addr)
	    vb_free(pstr->dsc[j].addr);
    }

    free(pstr->dsc);
    pstr->dsc = NULL;
    return ret;
}

int ClrCamFmt(struct camera_stream_t *pstr)
{
    int i, ret = 0;
    if (pstr == NULL)
	return -EINVAL;
    if (pstr->dsc == NULL)
	return 0;

    struct v4l2_requestbuffers buf_req = {
	.type	= pstr->buf_type,
	.count	= 0,
	.memory	= DEFAULT_MEM_TYPE,
    };
    ret = ioctl(pstr->fd_cam, VIDIOC_REQBUFS, &buf_req);
    app_info("ioctl,VIDIOC_REQBUFS\n");
    if (ret < 0)
	c_warn("%s: can't clean buffer:%s", pstr->name, strerror(errno));

    for (i = 0; i < pstr->NrBuf; i++) {
	if (pstr->dsc[i].addr)
	    vb_free(pstr->dsc[i].addr);
    }
    free(pstr->dsc);
    pstr->dsc = NULL;
    return 0;
}

int ContextRun(struct camera_stream_t *pstr)
{
    unsigned char *src = NULL;
    int len = 0;
    int ret = 0;

    int i;
    int count;
    //__u32 i;
   // __u64 count;
    struct v4l2_buffer cam_buffer;
    struct pollfd cam_ufds = {
	.fd = pstr->fd_cam,
	.events = POLLIN,
    };
    FILE *file;

    //if (pstr->NrFrame == ~0)
//	count = ~0;
 //   else
    app_info("NrFrame %d\n",pstr->NrFrame);

    if (pstr->save) {
	char FileName[FILE_NAME_LENGTH];
	sprintf(FileName, "%s%s", pstr->prefix, pstr->postfix);

	file = fopen(FileName, "wb");
	if (file == NULL)
	{
	    c_err("Filed to open output file '%s'", FileName);
	    ret = -1;
	    goto LABEL_STREAM_OFF;
	}
    }

    if (pstr->BeforeRun) {
	ret = pstr->BeforeRun(pstr);
	if (ret < 0)
	    return ret;
    }

    // Now we start the real thing
    ret = ioctl(pstr->fd_cam, VIDIOC_STREAMON, &pstr->buf_type);
    app_info("%s ioctl,VIDIOC_STREAMON,total frame number %d\n",pstr->name,pstr->NrFrame);
    if (ret < 0)
    {
	c_err("%s: streamon failed: %s", pstr->name, strerror(errno));
	return ret;
    };

    sleep(1);
    for (i = 0; i < pstr->NrFrame; i++)
    {
	if (pstr->kill)
	    break;

	ret = poll(&cam_ufds, 1, V4L2_POLL_TIMEOUT_MS);
	if (ret)
	    goto LABEL_DQBUF;
	c_info("%s poll timeout no data",pstr->name);
	goto LABEL_STREAM_OFF;
LABEL_DQBUF:
	memset(&cam_buffer, 0, sizeof(cam_buffer));
	cam_buffer.type    = pstr->buf_type;
	cam_buffer.memory  = DEFAULT_MEM_TYPE;
	fill_v4l2_buf(&cam_buffer, pstr, cam_buffer.index, 0);

	/*dequeue  buffer, get a buffer fetch data*/
	if (ioctl(pstr->fd_cam, VIDIOC_DQBUF, &cam_buffer)) {
	    c_err("%s can't dequeue output buffer: %s",pstr->name, strerror(errno));
	    ret = errno;
	    goto LABEL_STREAM_OFF;
	}
	app_info("%s ioctl,VIDIOC_DQBUF\n",pstr->name);
	c_info("%s: Got Frame %4d", pstr->name, i+1);

	src = pstr->dsc[cam_buffer.index].addr;
	len = pstr->dsc[cam_buffer.index].len;
	if ( (pstr->fmt_id == FMT_ID_JPEG)
		&& !((0xff == src[0]) && (0xd8 == src[1])) )
	{
	    c_err("JPEG frame head not found!");
	}

	if (pstr->save)
	    fwrite(src, len, 1, file);

	if (pstr->ProcessFrame)
	    pstr->ProcessFrame(pstr, src, len);

	fill_v4l2_buf(&cam_buffer, pstr, cam_buffer.index, 0);
	/* queue buffer, return the buffer to the queue */
	if (0 != ioctl (pstr->fd_cam, VIDIOC_QBUF, &cam_buffer)) {
	    printf("%s VIDIOC_QBUF can't queue camera buffer\n",pstr->name);
	    ret = -6;
	    goto LABEL_STREAM_OFF;
	}
	app_info("%s ioctl,VIDIOC_QBUF success\n",pstr->name);
    }

LABEL_STREAM_OFF:
    ioctl(pstr->fd_cam, VIDIOC_STREAMOFF, &pstr->buf_type);
    app_info("%s ioctl,VIDIOC_STREAMOFF\n",pstr->name);

LABEL_EXIT:
    if (pstr->save) {
	fclose(file);
	c_info("%s file '%s%s' saved",pstr->name, pstr->prefix, pstr->postfix);
    }
    pstr->ret = ret;
    return ret;
}

void *CameraThreadRun(void *arg)
{
    struct camera_stream_t *pstr = arg;
    ContextRun(pstr);
    pthread_exit(&pstr->ret);
    return NULL;
}

int CameraThreadCreate(struct camera_stream_t *stream)
{
    int ret;

    if (stream == NULL)
	return -EINVAL;

    if ((strlen(stream->name) == 0) ||
	    (stream->width == 0) ||
	    (stream->height == 0) ||
	    (stream->NrBuf == 0) ||
	    (stream->NrFrame == 0))	{
	c_err("stream parameter error");
	return -EINVAL;
    }

    if (stream->fd_cam < 0) {
	c_err("%s: video device not opened: %d", stream->name, stream->fd_cam);
	return -EINVAL;
    }

    ret = pthread_create(&stream->th_id, NULL, CameraThreadRun, stream);
    if (ret) {
	c_err("%s: failed to create camera thread", stream->name);
	goto LABEL_EXIT;
    }
    return 0;

LABEL_EXIT:
    ClrCamFmt(stream);
    return ret;
}

int CameraThreadKill(struct camera_stream_t *stream)
{
    int ret;

    if (stream == NULL)
	return -EINVAL;

    if (stream->fd_cam < 0)
	return -EINVAL;

    if (stream->th_id == 0)
	goto LABEL_CLEAR;

    stream->kill = 1;
    pthread_join(stream->th_id, NULL);
    if (stream->ret < 0)
	c_err("%s: stream failed when running", stream->name);
    else
	c_info("%s: success", stream->name);

LABEL_CLEAR:
    ClrCamFmt(stream);
    stream->th_id = 0;
    return stream->ret;
}
