/******************************************************************************
 * idm utils
 * (C) Copyright [2016] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/

#include <time.h>
#include "idm_utils.h"

idm_tick idm_get_tick_count()
{
	struct timespec t;

	t.tv_sec = t.tv_nsec = 0;

	clock_gettime( CLOCK_MONOTONIC, &t );
	return ( (long long)t.tv_sec * 1000000000LL + t.tv_nsec ) / 1000;
}

