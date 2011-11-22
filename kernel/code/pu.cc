/**************************************** GENERIC PROCESSING UNIT MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <pu.h>
#include <lock.h>
#include <debug.h>
#include <core.h>
#include <clock.h>


static PLACER<PU, CF_MAX_PROCESSORs> place;

uint PU::poolPool(0);
PU* PU::pu[CF_MAX_PROCESSORs];
TASK::TASKQUEUE PU::readyQueue;
QUEUE<TASK> PU::cronQueue;
bool PU::dispatchOrder(true);


PU::PU() : CPU(place.GetNumOf(this)), current(0){
	pu[cpuid] = this;
	Dispatch();
}

void PU::Dispatch(){
	if(!dispatchOrder){ return; }
	dispatchOrder = false;

#if CF_SMP
	//TODO:cpuidを推測してPU& pを作る
#else
	PU& p(*pu[0]);
#endif

	const TASK::PRIORITY pri(
		p.current ? (*p.current).priority : TASK::__pri_max);
	TASK* task(0);
	while(!(task = readyQueue.Get(pri)) && !p.current){
#if CF_SMP
		//TODO:暇プロセッサかtaskより低い最低優先度タスクを実行中のプロセッサにDispatch要求を出す。
#endif
		//実行すべきタスクが登録されるまで暇潰し
		EnableInterrupt();
		Halt();
		dprintf("[%t]\r", CLOCK::GetLocalTime());
		DisableInterrupt();
	}

	//タスクディスパッチ
	(*p.current).owner = 0;
	(*task).owner = &p;
	(*p.current).DispatchTo(*task);

	//入れ替え
	TASK* t(task);
	task = p.current;
	p.current = t;
}

void* PU::operator new(munit size){
	static LOCK lock;
	KEY key(lock);
	assert(poolPool < CF_MAX_PROCESSORs);
	return place.Place(poolPool++);
}

void PU::Cron(tunit now){
	QUEUE<TASK>::ITOR c(cronQueue);
	for(TASK* t(0); !!(t = c++) && (*t).uptime <= now;){
		c.Detach();
		(*t).WakeUp(TASK::RS_TIMEUP);
	}
}
