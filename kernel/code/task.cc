/************************************************************************ TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <task.h>
#include <heap.h>
#include <debug.h>


TASK::TASKQUEUE TASK::readyQueue;
QUEUE<TASK> TASK::cronQueue;
const uint TASK::thisSizeIndex(HEAP::GetBlockIndex(PAGESIZE));
TASK kernelTask;


TASK::TASK() :
	owner(0),
	priority(__pri_max),
	qNode(this),
	cronNode(this){}


void* TASK::operator new(munit contentSize){
	void* const r(HEAP::GetByIndex(thisSizeIndex));
	assert(r);
	return r;
}

void TASK::Cron(tunit now){
	QUEUE<TASK>::ITOR c(cronQueue);
	for(TASK* t(0); !!(t = c++) && (*t).uptime <= now;){
		(*t).WakeUp(/*TODO:時間切れで*/);
	}
}