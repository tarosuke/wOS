/********************************************* PROCESSOR DEPENDED PART OF TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#ifndef _CPU_TASK_
#define _CPU_TASK_

#include <config.h>
#include <types.h>


class CPUTASK {
	friend class VIRTUALPAGE;
	CPUTASK(CPUTASK&);
	void operator=(CPUTASK&);
public:
protected:
	CPUTASK();
	static inline runit GetPageRoot(){
		runit r;
		asm volatile("mov %%cr3, %0" : "=r"(r));
		return r;
	};
	static inline void SetPageRoot(runit root){
		asm volatile("mov %0, %%cr3" :: "r"(root));
	};
	inline void DispatchTo(CPUTASK& to){
		//このインスタンスが指しているタスクへ切り替える
		SetPageRoot(pageRoot);
	};
private:
	void* stack;
	munit userStack;
	munit userIP;
	const runit pageRoot;
	u32 kernelStack[CF_KERNELSTACK_ENTRIES];
};



#endif
