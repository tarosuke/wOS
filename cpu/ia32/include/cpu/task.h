/********************************************* PROCESSOR DEPENDED PART OF TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#ifndef _CPU_TASK_
#define _CPU_TASK_

#include <types.h>


class CPUTASK {
	CPUTASK(CPUTASK&);
	void operator=(CPUTASK&);
public:
	static inline runit GetPageRoot(){
		runit r;
		asm volatile("mov %%cr3, %0" : "=r"(r));
		return r;
	};
	static inline void SetPageRoot(runit root){
		asm volatile("mov %0, %%cr3" :: "r"(root));
	};
protected:
	CPUTASK(){};
	void DispatchTo(CPUTASK& to){
		asm volatile(
#if CF_AMD64
			"mov %%rsp, %0;"
			"mov %1, %%rsp;"
#endif
#if CF_IA32
			"mov %%esp, %0;"
			"mov %1, %%esp;"
#endif
			"mov %2, %%cr3"
			: "=r"(stack) : "l"(to.stack), "r"(pageRoot));
	};
private:
	void* stack;
	munit pageRoot;
};




#endif
