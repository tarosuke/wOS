/************************************************************************ TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _ULIB_TASK_
#define _ULIB_TASK_


extern struct{
	void* heap;
	unsigned stackSize;
	struct _MESSAGE* tx;
	struct _MESSAGE* rx;
	unsigned status;
	void (*entry)(void);
}taskHead;


#endif
