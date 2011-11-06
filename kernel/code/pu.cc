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
	asm volatile("sti");

	// タスクを分捕って実行。なければhltして暇潰し
	for(;;){
		TASK* task(TASK::GetReadyOne());
		if(task){
			(*task).owner = this;
			//TODO:コンテキストスイッチ
			running = task;
		}else{
			asm volatile("hlt");
			dprintf("uptime:%llu\r", CORE::GetUptime());
		}
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
