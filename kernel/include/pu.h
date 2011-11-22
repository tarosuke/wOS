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


class TASK;
class PU : public CPU{
	friend class CLOCK;
public:
	PU();
	static void* operator new(munit);
	static void Dispatch();
private:
	TASK* current;		//このプロセッサで実行中のタスク
	static uint poolPool;
	static uint NewID();
	static void Cron(tunit);
	static PU* pu[];
	static TASK::TASKQUEUE readyQueue;
	static QUEUE<TASK> cronQueue;
	static bool dispatchOrder;
};


#endif
