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
	PU() : current(0){};
	void* operator new(munit){
		const uint n(numOfPu++);
		if(!n){
			//最初のプロセッサなら誤ディスパッチを防ぐために初期化
			///PUの初期化子作ってそれで初期化したほうがいいかも知れず
			for(int m(0); m < CF_MAX_PROCESSORs; m++){
				pus[m].priority = TASK::__pri_max;
			}
		}
		return (void*)&pus[n];
	};
	static void Dispatch();
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
	TASK* current;			//このプロセッサで実行中のタスク
	TASK::PRIORITY priority;		//実行中の優先度
};


#endif
