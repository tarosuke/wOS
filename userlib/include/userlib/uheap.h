/************************************************************************ HEAP
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _ULIB_HEAP_
#define _ULIB_HEAP_

#include <types.h>


void HEAP_Init(void);
void* HEAP_Get(munit size);
void HEAP_Release(void*);


#endif
