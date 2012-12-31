/********************************************* PROCESSOR DEPENDED PART OF TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#ifndef _CPU_TASK_
#define _CPU_TASK_

#include <config.h>
#include <types.h>
#include <cpu/virtualPage.h>


class CPUTASK : public VIRTUALPAGE {
	friend class PU;
	void operator=(CPUTASK&);
public:
protected:
	CPUTASK();		//アイドルタスクのコンストラクタ
	CPUTASK(const CPUTASK&);	//他のタスクはアイドルタスクのコピーベース
	inline void SaveStack(){
		#if CF_IA32
		asm volatile("mov %%esp, %0" : "=r"(stack));
		#endif
		#if CF_AMD64
		asm volatile("mov %%rsp, %0" : "=r"(stack));
		#endif
	};
	inline void RestoreStack(){
		#if CF_IA32
		asm volatile("mov %0, %%esp" :: "r"(stack));
		#endif
		#if CF_AMD64
		asm volatile("mov %0, %%rsp" :: "r"(stack));
		#endif
	};
	inline void DiveInto(){
		//TODO:TSSをスタックの初期値に設定してユーザ空間へ降りる
	};
	inline void DispatchTo(/*CPUTASK& from*/){
// 		from.SaveStack();
		RestoreStack();
		VIRTUALPAGE::DispatchTo();
	};
private:
	void* stack;
	munit userStack;
	munit userIP;
	u32 kernelStack[CF_KERNELSTACK_ENTRIES];
};



#endif
