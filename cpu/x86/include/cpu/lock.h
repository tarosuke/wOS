/************************************************************************ LOCK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */


#ifndef _CPU_LOCK_
#define _CPU_LOCK_


#include <config.h>


class ILOCK{
	friend class KEY;
	friend class IKEY;
	ILOCK(ILOCK&);
	void operator=(ILOCK&);
public:
	ILOCK(){};
protected:
	inline void Lock(){
		asm volatile(
			"pushf;"
			"pop %0;"
			"cli" :
			"=g"(stat));
	};
	inline void Unlock(){
		asm volatile("push %0;popf;" :: "g"(stat));
	};
private:
	munit stat;
};



class LOCK : public ILOCK{
	friend class KEY;
	friend class IKEY;
	LOCK(ILOCK&);
	void operator=(ILOCK&);
public:
#if 2 <= CF_MAX_PROCESSORs
	LOCK() : lock(false){};
#else
	LOCK(){};
#endif
private:
	#if 2 <= CF_MAX_PROCESSORs
	bool lock;
	#endif
	inline void Lock(){
		ILOCK::Lock();
		#if 2 <= CF_MAX_PROCESSORs
		asm volatile(
			"mov $1, %%al;"
			"1: xchg %0, %%al;"
			"test %%al, %%al;"
			"jz 1f;"
			"pause;"
			"jmp 1b;"
			"1:":
			"=m"(lock) :: "eax");
		#endif
	};
	inline void Unlock(){
		#if 2 <= CF_MAX_PROCESSORs
		lock = false;
		#endif
		ILOCK::Unlock();
	};
};

#endif
