#ifndef CAMERA_API_H
#define CAMERA_API_H

#include <linux/types.h>
#include <linux/v4l2-mediabus.h>
#include <linux/v4l2-subdev.h>
#include <linux/videodev2.h>
#include <pthread.h>
#define FILE_NAME_LENGTH 30
#define IDENTIFIER_LENGTH 30

struct camera_fmt_t
{
	char	name[IDENTIFIER_LENGTH];
	__u32	fourcc;
	int	bpp;
};

enum {
	FMT_ID_UYVY	= 0,
	FMT_ID_YV16,
	FMT_ID_YU12,
	FMT_ID_YV12,
	FMT_ID_NV12,
	FMT_ID_NV21,
	FMT_ID_JPEG,
	FMT_ID_RGBP,
	FMT_ID_BA81,
	FMT_ID_GBRG,
	FMT_ID_GRBG,
	FMT_ID_RGGB,
	FMT_ID_BG10,
	FMT_ID_GB10,
	FMT_ID_BA10,
	FMT_ID_RG10,
	FMT_ID_BG12,
	FMT_ID_GB12,
	FMT_ID_BA12,
	FMT_ID_RG12,
	FMT_ID_RG16,
};

struct cam_buf_desc
{
	void	*addr;
	__s32	dmabuf_fd;
	size_t	len;
	struct v4l2_plane ImagePlane[VIDEO_MAX_PLANES];
};

struct camera_stream_t
{
	char	name[IDENTIFIER_LENGTH];
	int	fd_cam;
	__u32	buf_type;
	__u32	width;
	__u32	height;
	__u16	fmt_id;
	__u16	NrBuf;
	__u32	NrFrame;

	struct v4l2_format	v4l2_fmt;
	struct cam_buf_desc	*dsc;
	pthread_mutex_t	signal;
	pthread_t	th_id;

	int	kill:1;
	int	save:1;	// Save image to output file?
	int	load:1;	// Fill buffer with input image?
	char	prefix[IDENTIFIER_LENGTH];
	char	postfix[IDENTIFIER_LENGTH];
	char	InputName[IDENTIFIER_LENGTH];
	int	ret;
	int 	mmu_en;

	/* handlers */
	int	(*BeforeRun)(struct camera_stream_t *stream);
	int	(*ProcessFrame)(struct camera_stream_t *stream, void *addr, size_t len);
};

enum {
		ID_UYVY	= 0,
		ID_YU16,
		ID_YU12,
		ID_JPEG,
		ID_END,
};

#define fourcc_to_char(fourcc) (((fourcc) >> 0) & 0xFF), (((fourcc) >> 8) & 0xFF), (((fourcc) >> 16) & 0xFF), (((fourcc) >> 24) & 0xFF)

#define c_err(format, arg...)	\
do {	\
	printf("[camera:ERROR]: " format "\n", ## arg);	\
	fflush(stdout);	\
} while(0)

#define c_warn(format, arg...)	\
do {	\
	printf("[camera:WARNING]: " format "\n", ## arg);	\
	fflush(stdout);	\
} while(0)

#define c_info(format, arg...)	\
do {	\
	printf("[camera:INFO]: " format "\n", ## arg);	\
	fflush(stdout);	\
} while(0)

#endif
