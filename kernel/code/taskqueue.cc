/******************************************************************* TASKQUEUE
 *	Copyright (C) 2013- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#include <task.h>


/// TASKQUEUE
TASK::TASKQUEUE::~TASKQUEUE(){
	TASK* t;
	while(!!(t = Get())){
		(*t).Wakeup(WC_QUEUE_DISCARDED);
	}
};
void TASK::TASKQUEUE::Add(TASK& task){
	if(task.priority < PRIORITY::__pri_max){
		MULTIQUEUE<TASK, PRIORITY::__pri_max>::Add(task.priority, task.qNode);
	}
};
void TASK::TASKQUEUE::Insert(TASK& task){
	if(task.priority < PRIORITY::__pri_max){
		MULTIQUEUE<TASK, PRIORITY::__pri_max>::Insert(task.priority, task.qNode);
	}
};


/// CRONQUEUE
void TASK::CRONQUEUE::Register(TASK& task, tunit uptime){
	task.uptime = uptime;

	ITOR itor(*this);
	TASK* t;
	while(!!(t = itor++) && (*t).uptime < uptime);
	itor.Insert(task.qNode);
};
void TASK::CRONQUEUE::Tick(tunit now){
	ITOR itor(*this);
	for(TASK* t; !!(t = itor++);){
		if((*t).uptime <= now){
			(*t).Wakeup(WC_TIMEUP);
		}
	}
};
