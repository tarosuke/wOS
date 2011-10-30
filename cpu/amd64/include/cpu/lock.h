/***************************************************************** KERNEL CORE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */


#ifndef _CPU_LOCK_
#define _CPU_LOCK_


#include <config.h>


class LOCK{
	friend class KEY;
	friend class RKEY;
private:
	u32 stat;
#if 2 <= CF_MAX_PROCESSORs
	bool lock;
#endif
	void Lock(){
		asm volatile(
			"pushf;"
			"pop %0;"
			"cli" :
			"=g"(stat));
#if 2 <= CF_MAX_PROCESSORs
		// TODO:WHEN FAILED, USE TEST $1,(%ebx) INSTEAD XCHG.
		asm volatile(
			"mov $1,%%eax;"
			"mov %0, %%ebx;"
			"1: xchg (%%ebx), %%eax;"
			"test %%eax, %%eax;"
			"jnz 1b" :
			"=m"(lock) :: "eax", "ebx");

#endif
	};
	void Unlock(){
#if 2 <= CF_MAX_PROCESSORs
		asm volatile("movl $0, %0" :: "m"(lock));
#endif
		asm volatile("push %0;popf;" :: "g"(stat));
	};
};

#endif

