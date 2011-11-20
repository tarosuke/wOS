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

uint PU::idPool(0);
uint PU::poolPool(0);
PU* PU::pu[CF_MAX_PROCESSORs];
TASK::TASKQUEUE PU::readyQueue;
QUEUE<TASK> PU::cronQueue;
bool PU::dispatchOrder(false);


PU::PU() : CPU(place.GetNumOf(this)), current(0){
	pu[cpuid] = this;
	EnableInterrupt();
	Dispatch();
}

void PU::Dispatch(){
	TASK* task;
	while(!(task = readyQueue.Get())){
		//実行すべきタスクが登録されるまで暇潰し
		EnableInterrupt();
		Halt();
		dprintf("[%t]\r", CLOCK::GetLocalTime());
	}
	DisableInterrupt();
assert(false);
#if CF_SMP
#else
	PU& p(*pu[0]);
#endif


	//今までのタスクを手放して新しいタスクを入手
	(*p.current).owner = 0;
	(*task).owner = &p;
	//TODO:コンテキストスイッチ
	p.current = task;
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
