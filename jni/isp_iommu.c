#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include "MediaLib.h"
#include "isp_iommu.h"

int CamIommuInit(int fd,int enable)
{
	struct v4l2_control stCtrlParam = { V4L2_CID_VDEV_BUFFER_LAYOUT, VDEV_BUFFER_LAYOUT_PA_CONTIG};

	if (enable )
	{
		app_info("The buffer layout config as VDEV_BUFFER_LAYOUT_VA_CONTIG for port %d, MMU enabled", fd);
		stCtrlParam.value = VDEV_BUFFER_LAYOUT_VA_CONTIG;
	}
	else
	{
		app_info("The buffer layout config as VDEV_BUFFER_LAYOUT_PA_CONTIG for port %d, MMU disabledd", fd);
		stCtrlParam.value = VDEV_BUFFER_LAYOUT_PA_CONTIG;
	}
	return  ioctl( fd, VIDIOC_S_CTRL, &stCtrlParam );
}



