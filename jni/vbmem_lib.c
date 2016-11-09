#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "ion.h"

#include <errno.h>
#include "vbmem_lib.h"

//#define DEBUG
//#define USE_SYSTEM_MEM




#ifdef DEBUG
#define pr_debug(fmt, arg ...) printf(fmt, ## arg)
#else
#define pr_debug(fmt, arg ...) do { } while (0)
#endif

#define pr_error(fmt, arg ...)  printf(fmt, ## arg)
#define NO_IMPLEMENTED printf("%s is not implemented yet.", __FUNCTION__)


/* We recorded all malloced physical address into a list. Due to the implementation of
 * pmem, we can only open a new file to malloc some memory and close it to free memory. So
 * we also record the opened fd associated with physical and virtual address map. */
struct memory_item {
	int			fd;
	void			*paddr;
	void			*vaddr;
	int			size;
	struct memory_item	*next;
};

static struct memory_item *header = NULL;

int vb_init()
{
	pr_debug("vbmem_init");
	header = NULL;
	return 0;
}

void vb_exit()
{
	struct memory_item *cur = header;
	struct memory_item *next = NULL;

	pr_debug("vbmem_exit");
	while (cur != NULL) {
		next = cur->next;
		ovmem_munmap(cur->vaddr, cur->size);
		ovmem_free(cur->fd);
		free(cur);
		cur = next;
	}
}

void *vb_alloc(unsigned long size, int *pfd)
{
	void *vaddr, *paddr;
	struct memory_item *item = NULL;
	int buf_fd;


#ifdef USE_SYSTEM_MEM
	buf_fd = ovmem_alloc(size, OVM_SYSTEM, OVM_DEF_ALIGN);
#else
	buf_fd = ovmem_alloc(size, OVM_CONTIG, OVM_DEF_ALIGN);
#endif
	if (buf_fd < 0)
		pr_error("buffer allocate failed");

	vaddr = ovmem_mmap(buf_fd, size, 0);
	if (vaddr == NULL)
		pr_error("failed to get VA");

	ovmem_get_phys(buf_fd, (long unsigned int *)&paddr);

	item = (struct memory_item *)malloc(sizeof(struct memory_item));
	memset(item, 0, sizeof(struct memory_item));
	item->fd	= buf_fd;
	item->paddr	= paddr;
	item->vaddr	= vaddr;
	item->size	= size;

	if (header == NULL) {
		header = item;
	} else {
		item->next = header;
		header = item;
	}
	printf("vb-lib: allocate buffer %X @ %p\n", buf_fd, paddr);
	if (pfd)
		*pfd = buf_fd;
	return vaddr;
}

void vb_free(void *vaddr)
{
	struct memory_item *cur = header;
	struct memory_item *prev = NULL;
	struct memory_item *next = NULL;

	int bFreed = 0;
	int ret = 0;
	struct ion_handle_data req = {0};

	while (cur != NULL) {
		next = cur->next;
		if (cur->vaddr == vaddr) {
			ovmem_munmap(cur->vaddr, cur->size);
			ovmem_free(cur->fd);

			if (prev == NULL)
				header = next;
			else
				prev->next = next;
			free(cur);

			bFreed = 1;

			break;
		}
		prev = cur;
		cur = next;
	}

	if(!bFreed)
	{
		printf("Fatal error: cannot free memory 0x%x\n",(unsigned int)vaddr);
		exit(1);
	}
}

int vb_flush_cache_range(unsigned long start, unsigned long size, int direction)
{
	int offset = 0;
	struct memory_item *cur = header;

	while (cur != NULL) {
		offset = start - (unsigned long)cur->paddr;
		if (offset >= 0 && offset < cur->size)
			break;
		cur = cur->next;
	}
	return -ENOENT;

LABEL_FIND:
	return ovmem_sync_range(cur->fd, offset, size);
}

void *bmm_get_vaddr(void *paddr)
{
	pr_debug("bmm_get_vaddr");
	struct memory_item *cur = header;
	int offset = 0;
	while (cur != NULL) {
		offset = (unsigned int)paddr - (unsigned int)cur->paddr;
		if (offset >= 0 && offset < cur->size)
			return (void *)((unsigned int)cur->vaddr + offset);
		cur = cur->next;
	}
	return NULL;
}

void *bmm_get_paddr(void *vaddr)
{
	pr_debug("bmm_get_paddr");
	struct memory_item *cur = header;
	int offset = 0;
	while (cur != NULL) {
		offset = (unsigned int)vaddr - (unsigned int)cur->vaddr;
		if (offset >= 0 && offset < cur->size)
			return (void *)((unsigned int)cur->paddr + offset);
		cur = cur->next;
	}
	return NULL;
}

unsigned long bmm_get_mem_size(void *vaddr)
{
	struct memory_item *cur = header;

	while (cur != NULL) {
		if (cur->vaddr == vaddr)
			return cur->size;
		cur = cur->next;
	}
	return 0;
}

int bmm_get_mem_attr(void *vaddr)
{
	NO_IMPLEMENTED;
	return 0;
}

int bmm_set_mem_attr(void *vaddr, int attr)
{
	NO_IMPLEMENTED;
	return 0;
}

unsigned long bmm_get_total_space(){//to be added
	return 0;
}

unsigned long bmm_get_free_space()
{
	NO_IMPLEMENTED;
	return 0;
}

unsigned long bmm_get_allocated_space()
{
	NO_IMPLEMENTED;
	return 0;
}

void bmm_flush_cache(void *vaddr, int dir)
{
	NO_IMPLEMENTED;
}

void *bmm_dma_memcpy(void *dest, const void *src, size_t n)
{
	NO_IMPLEMENTED;
	return NULL;
}

void bmm_dma_sync()
{
	NO_IMPLEMENTED;
}

void bmm_dump()
{
	NO_IMPLEMENTED;
}
