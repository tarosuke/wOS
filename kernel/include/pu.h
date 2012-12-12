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
	PU();
	void* operator new(munit){
		const uint id(GetID());
		assert(id < CF_MAX_PROCESSORs);
		return (void*)&pus[id];
	};
	/// ディスパッチ
	static inline void Dispatch(){
		GetPU()._Dispatch();
	};
	/// 現在の状態を取得、など
	static TASK& GetCurrentTask(){ return *GetPU().current; };
	/// タスク終了
	static void Kill(){
		//タスク終了時にいきなりdeleteするとその時点で使っているスタックまで開放してしまうことになって都合が悪いため、delete待ち行列(grave)に投入し、墓守がdeleteするまでゾンビとする。また次のタスクでdeleteすると優先度を無視してdeleteが走るためこうしている。
		PU& pu(GetPU());
		IKEY key;
		grave.Add((*pu.current).qNode);
		(*pu.current).priority = TASK::__pri_max;
	};
	/// タスクの起床
	void WakeUp(TASK& task){
		ready.Add(task);

//TODO:IPIはDispatchToで発行される
		IssueIPI();
	};
private:
	static inline void RequestDispatch(PU& pu){
		#if CF_SMP
		if(&pu != &GetPU()){
			//TODO:(*task.owner)に対してIPI
		}
		#endif
	};
	static inline PU& GetPU(){
#if CF_SMP
		return pus[GetID()];
#else
		return pus[0];
#endif
	};
	void _Dispatch();
	typedef PLACER<PU, CF_MAX_PROCESSORs> PUPLACER;
	static PUPLACER pus;
	static ATOMIC numOfPu;
	static QUEUE<TASK> grave;		//終了後のタスク構造を保持する
	TASK::TASKQUEUE ready;
	TASK* current;			//このプロセッサで実行中のタスク
	TASK* next;
	TASK idle;
};


#endif
