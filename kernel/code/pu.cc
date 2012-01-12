/**************************************** GENERIC PROCESSING UNIT MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <pu.h>
#include <lock.h>
#include <debug.h>
#include <core.h>
#include <clock.h>


PU* PU::pus[CF_MAX_PROCESSORs];
TASK::TASKQUEUE PU::readyQueue;
QUEUE<TASK> PU::cronQueue;
bool PU::dispatchRequest(true);


PU::PU(){
	pus[cpuid] = this;
	for(;;Dispatch());
}

void PU::Dispatch(){
	if(!dispatchRequest){ return; }
	dispatchRequest = false;

	//ディスパッチの必要があるプロセッサは全てディスパッチ
	for(uint i(0); i < CF_MAX_PROCESSORs; i++){
		if(pus[i]){
			(*pus[i]).GetNewOwn();
		}
	}
}

void PU::Cron(tunit now){
	QUEUE<TASK>::ITOR c(cronQueue);
	for(TASK* t(0); !!(t = c++) && (*t).uptime <= now;){
		c.Detach();
		(*t).WakeUp(TASK::RS_TIMEUP);
	}
}
