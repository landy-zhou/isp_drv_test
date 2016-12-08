/******************************************************************************
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/

#ifndef __VBMEM_LIB_H__
#define __VBMEM_LIB_H__

#include "ovmem_ion.h"

#if defined (__cplusplus)
extern "C" {
#endif

int vb_init();
void vb_exit();
void *vb_alloc(unsigned long size, int *pfd);
void vb_free(void *vaddr);
int vb_flush_cache_range(unsigned long start, unsigned long size, int direction);

#if defined (__cplusplus)
}
#endif

#endif /* __VBMEM_LIB_H__ */
