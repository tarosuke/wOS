/**************************************** GENERIC PROCESSING UNIT MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <pu.h>
#include <lock.h>
#include <debug.h>
#include <core.h>


uint PU::idPool(0);
uint PU::poolPool(0);
static char puPool[CF_MAX_PROCESSORs][sizeof(PU)] __attribute__((aligned(4)));


PU::PU() : CPU(NewID()){
	//TODO:こいつがタスクを分捕って実行する手筈になってる
	asm volatile("sti");
	for(;;){
		asm volatile("hlt");
		dprintf("uptime:%llu\r", CORE::GetUptime());
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