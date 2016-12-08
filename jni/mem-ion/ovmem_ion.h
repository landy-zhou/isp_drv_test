/*
 * Copyright 2013 Omnivision International Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __SYS_CORE_OVMEM_H
#define __SYS_CORE_OVMEM_H

#include <sys/types.h>

#define OVM_CONTIG	(1u << 0)
#define OVM_SYSTEM	(1u << 1)
#define OVM_DMA		(1u << 4)
#define OVM_CACHED	(1u << 16)
#define OVM_NESYNC	(1u << 17)

#define OVM_DMA_READ	(1u << 0)
#define OVM_DMA_WRITE	(1u << 1)
#define OVM_CPU_READ	(1u << 2)
#define OVM_CPU_WRITE	(1u << 3)

#define OVM_DEF_FLAGS	(OVM_CONTIG | OVM_CACHED | OVM_NESYNC)
#define OVM_DEF_ALIGN	(4096)

#define OVMEM_NOTIFY_QUERY	0
#define OVMEM_NOTIFY_DMA_READ	1
#define OVMEM_NOTIFY_DMA_WRITE	2
#define OVMEM_NOTIFY_CPU_READ	4
#define OVMEM_NOTIFY_CPU_WRITE	8
#define OVMEM_NOTIFY_VARIED	0xF

#define OVM_BUF_STATE_INIT 	0
#define OVM_BUF_STATE_NEW 	1
#define OVM_BUF_STATE_DIRTY 	2
#define OVM_BUF_STATE_CLEAN 	3

__BEGIN_DECLS

int ovmem_alloc(size_t len, unsigned int flags, size_t align);
int ovmem_free(int fd);
int ovmem_sync(int fd);
int ovmem_sync_range(int fd, unsigned int offset, unsigned int size);

int ovmem_set_name(int fd, const char *name);
int ovmem_get_phys(int fd, unsigned long *pa);
int ovmem_notify(int fd, unsigned int note);

int ovmem_set_usage(int fd, unsigned int op);
int ovmem_get_usage(int fd, unsigned int *status);

void *ovmem_mmap(int fd, size_t len, unsigned long offset);
int ovmem_munmap(void *addr, size_t len);

__END_DECLS

#endif /* __SYS_CORE_OVMEM_H */
