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
	friend class PU;
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
	inline void SaveStack(){
		#if CF_IA32
		asm volatile("mov %%esp, %0" : "=r"(stack));
		#endif
		#if CF_AMD64
		asm volatile("mov %%rsp, %0" : "=r"(stack));
		#endif
	};
	inline void DiveInto(){
		//TODO:TSSをスタックの初期値に設定してユーザ空間へ降りる
	};
	inline void DispatchTo(){
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
