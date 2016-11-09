
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <pthread.h>
#include <sys/mman.h>
#include "ion.h"	
#include "ovmem.h"	

static int ovm_device_fd = -ENOENT;
static pthread_mutex_t ovm_mutex = PTHREAD_MUTEX_INITIALIZER;

static int get_device_fd(void)
{
	pthread_mutex_lock(&ovm_mutex);

	if (ovm_device_fd < 0)
		ovm_device_fd = ion_open();

	pthread_mutex_unlock(&ovm_mutex);

	return ovm_device_fd;
}

int ovmem_alloc(size_t len, unsigned int flags, size_t align)
{
	unsigned int ion_flags = 0;
	int device_fd, share_fd;
	unsigned int heap;
	int ret;

	if (flags & OVM_CACHED)
		ion_flags |= ION_FLAG_CACHED;
	if (flags & OVM_NESYNC)
		ion_flags |= ION_FLAG_CACHED_NEEDS_SYNC;

	if (flags & OVM_CONTIG)
		heap = ION_HEAP_CARVEOUT_MASK;
	if (flags & OVM_SYSTEM)
		heap = ION_HEAP_SYSTEM_MASK;

	device_fd = get_device_fd();
	if (device_fd < 0)
		return -ENOENT;

	ret = ion_alloc_fd(device_fd, len, align, heap, ion_flags, &share_fd);
	if (ret < 0)
		return -ENOENT;

	return share_fd;
}

int ovmem_free(int fd)
{
	if (fd < 0) {
		return -EINVAL;
	}

	close(fd);
	return 0;
}

int ovmem_set_name(int fd, const char *name)
{
	struct ion_buffer_name_data data;
	int device_fd;
	int ret;

	device_fd = get_device_fd();
	if (device_fd < 0)
		return -ENOENT;

	data.fd = fd;
	strncpy(data.name, name, ION_BUFFER_NAME_LEN);
	ret = ioctl(device_fd, ION_IOC_NAME, &data);

	return ret;
}

int ovmem_get_phys(int fd, unsigned long *pa)
{
	struct ion_phys_data data;
	int device_fd;
	int ret;

	device_fd = get_device_fd();
	if (device_fd < 0)
		return -ENOENT;

	data.fd = fd;
	ret =  ioctl(device_fd, ION_IOC_PHYS, &data);

	if (ret == 0)
		*pa = (unsigned long)data.addr;

	return ret;
}

int ovmem_sync(int fd)
{
	struct ion_fd_data data;
	int device_fd;
	int ret;

	device_fd = get_device_fd();
	if (device_fd < 0)
		return -ENOENT;

	data.fd = fd;
	ret =  ioctl(device_fd, ION_IOC_SYNC, &data);

	return ret;
}

int ovmem_sync_range(int fd, unsigned int offset, unsigned int size)
{
	struct ion_sync_range_data data;
	int device_fd;
	int ret;

	device_fd = get_device_fd();
	if (device_fd < 0)
		return -ENOENT;

	data.fd = fd;
	data.offset = offset;
	data.size = size;
	ret =  ioctl(device_fd, ION_IOC_SYNC_RANGE, &data);

	return ret;
}

void *ovmem_mmap(int fd, size_t len, unsigned long offset)
{
	return mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
}

int ovmem_munmap(void *addr, size_t len)
{
	return munmap(addr, len);
}

int ovmem_notify(int fd, unsigned int note)
{
	struct ion_notify_data data;
	int device_fd;
	int ret;

	data.fd = fd;
	data.note = note;

	device_fd = get_device_fd();
	if (device_fd < 0)
		return -ENOENT;

	ret =  ioctl(device_fd, ION_IOC_NOTIFY, &data);
	if (ret)
		return ret;

	return data.note;
}

int ovmem_set_usage(int fd, unsigned int op)
{
	int ret = ovmem_notify(fd, op);
	if (ret < 0)
		return ret;

	return 0;
}

int ovmem_get_usage(int fd, unsigned int *status)
{
	int ret = ovmem_notify(fd, ION_BUFFER_NOTIFY_QUERY);
	if (ret < 0)
		return ret;

	*status = ret;
	return 0;
}
