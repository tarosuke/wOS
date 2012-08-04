/********************************************* PROCESSOR DEPENDED PART OF TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#ifndef _CPU_TASK_
#define _CPU_TASK_

#include <types.h>


class CPUTASK {
	friend class VIRTUALPAGE;
public:
protected:
	CPUTASK();
	CPUTASK(void* stack);
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
	munit userStack;
	munit userIP;
	const runit pageRoot;
};



#endif
