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


class PU : public CPU{
	friend class CLOCK;
public:
	PU();
	static void Dispatch();
private:
	static inline PU& GetPU(){
		return *pus[GetID()];
	};
	inline void GetNewOwn(){
		const uint p(current ? (*current).priority : TASK::__pri_max);
		if(!!(haveToOwn = readyQueue.Get(p))){
			//ディスパッチが必要
			DispatchTo();
		}
	};
	static void Cron(tunit);
	static PU* pus[];
	static TASK::TASKQUEUE readyQueue;
	static QUEUE<TASK> cronQueue;
	static bool dispatchRequest;
};


#endif
