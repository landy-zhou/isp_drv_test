/******************************************************************************
 * idm utils
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/

#ifndef __IDM_UTILS_H__
#define __IDM_UTILS_H__

#include <stdio.h>

typedef signed long long idm_tick;

idm_tick idm_get_tick_count();

#define IDM_DEBUG_EN

#ifdef IDM_DEBUG_EN 

#define idm_err(format, arg...)		\
	do { \
	printf("[IDM:ERROR]: " format "\n", ## arg); \
	fflush(stdout); \
	} while (0)
#define idm_warn(format, arg...)		\
	do { \
	printf("[IDM:WARNING]: " format "\n", ## arg); \
	fflush(stdout); \
	} while (0)
#define idm_info(format, arg...)		\
	do { \
	printf("[IDM:INFO][%s]: " format "\n",__FUNCTION__, ## arg); \
	fflush(stdout); \
	} while (0)

#else

#define idm_err(format, arg...)	    do{}while(0)	
#define idm_warn(format, arg...)    do{}while(0)	
#define idm_info(format, arg...)	do{}while(0)

#endif

#endif //__IDM_UTILS_H__

