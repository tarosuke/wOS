/**************************************** GENERIC PROCESSING UNIT MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _PU_
#define _PU_

#include <config.h>
#include <types.h>
#include <cpu/cpu.h>
#include <task.h>
#include <heap.h>
#include <atomic.h>


class PU : public CPU{
	PU(PU&);
	void operator=(PU&);
public:
	PU() : current(&idle){};
	void* operator new(munit){
		const uint id(GetID());
dprintf("gs:%04x.\n", id);
		assert(id < CF_MAX_PROCESSORs);
		return (void*)&pus[id];
	};
	/// 全プロセッサを対象としてディスパッチ
	static void Dispatch(){
		//TODO:全プロセッサを対象としてディスパッチ
	};
	static TASK& GetCurrentTask(){ return *GetPU().current; };
private:
	static inline PU& GetPU(){
#if CF_SMP
		return pus[GetID()];
#else
		return pus[0];
#endif
	};
	typedef PLACER<PU, CF_MAX_PROCESSORs> PUPLACER;
	static PUPLACER pus;
	static ATOMIC numOfPu;
	static TASK::TASKQUEUE ready;
	static LOCK dispatching;
	TASK* current;			//このプロセッサで実行中のタスク
	TASK idle;
};


#endif
