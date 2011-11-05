/************************************************************ CPU MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <config.h>
#include <cpu/cpu.h>
#include <arch/segments.h>
#include <debug.h>
#include <arch/pic.h>
#include <lock.h>


extern "C"{
	extern u64 __TSSPH[];
};


CPU::TSS CPU::tsss[CF_MAX_PROCESSORs]__attribute__((aligned(1024)));
uint CPU::idPool(0);

static CPU cpu;


uint CPU::GetID(){
	static LOCK lock;
	KEY key(lock);
	return idPool++;
}


CPU::CPU() : cpuid(GetID()), tss(tsss[cpuid]){
	assert(cpuid < CF_MAX_PROCESSORs);

	dprintf("cpu(%d)..."INDENT, cpuid);

	//  当該プロセッサ用のTSSを設定
	dprintf("selector: %d.\n", TSSSel + cpuid);
	const u64 p((munit)&tss);
	__TSSPH[cpuid] = 0x000089000000006c |
		((p & 0x00ffffff) << 16) |
		((p & 0xff000000) << 32);
	dprintf("tss: %p.\n", &tss);
	dprintf("desc: %016llx.\n", __TSSPH[cpuid]);
	asm volatile("ltr %%ax" :: "a"(TSSSel + cpuid));

	dputs(UNINDENT"OK.\n");
}


