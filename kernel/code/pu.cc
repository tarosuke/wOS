/**************************************** GENERIC PROCESSING UNIT MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <pu.h>
#include <lock.h>
#include <debug.h>
#include <core.h>
#include <task.h>


TASK PU::idleTasks[CF_MAX_PROCESSORs];
uint PU::idPool(0);
uint PU::poolPool(0);
static char puPool[CF_MAX_PROCESSORs][sizeof(PU)] __attribute__((aligned(4)));


PU::PU() : CPU(NewID()), running(&idleTasks[CPU::cpuid]){
	(*running).owner = this;
	CPU::EnableInterrupt();

	// 最初にタスクを分捕ってみる。暇ならhltして暇潰し
	for(Dispatch();;){
		CPU::Halt();
		dprintf("uptime: %t\r", CORE::GetUptime());
	}
}

void PU::Dispatch(){
	TASK* task(TASK::GetReadyOne());
	if(task){
		//今までのタスクを手放して新しいタスクを入手
		(*running).owner = 0;
		(*task).owner = this;
		//TODO:コンテキストスイッチ
		running = task;
	}
}

uint PU::NewID(){
	static LOCK lock;
	KEY key(lock);
	assert(idPool < CF_MAX_PROCESSORs);
	return idPool++;
}

void* PU::operator new(munit size){
	static LOCK lock;
	KEY key(lock);
	assert(poolPool < CF_MAX_PROCESSORs);
	return (void*)&puPool[poolPool++];
}
