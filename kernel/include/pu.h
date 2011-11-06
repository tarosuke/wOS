/**************************************** GENERIC PROCESSING UNIT MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _PU_
#define _PU_

#include <config.h>
#include <types.h>
#include <cpu/cpu.h>


class TASK;
class PU : public CPU{
public:
	PU();
	void* operator new(munit);
	void Dispatch();
private:
	TASK* running;		//このプロセッサで実行中のタスク
	static uint idPool;
	static uint poolPool;
	static uint NewID();
	static TASK idleTasks[];
};


#endif
