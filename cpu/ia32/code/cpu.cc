/************************************************************ CPU MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <config.h>
#include <cpu/cpu.h>
#include <arch/segments.h>
#include <debug.h>


extern "C"{
	extern u64 __TSSPH[];
};


CPU::TSS CPU::tsss[CF_MAX_PROCESSORs]__attribute__((aligned(1024)));
uchar CPU::kernelStacks[CF_MAX_PROCESSORs][4096]__attribute__((aligned(sizeof(int))));




CPU::CPU(uint id) : cpuid(id), tss(tsss[id]){
	assert(cpuid < CF_MAX_PROCESSORs);

	// ここより下位がカーネルスタック
	asm volatile("mov %0, %%esp" :: "r"(this));

	// TSS設定
	tss.ss2 = UDSel;
	tss.ss0 = KDSel;
	tss.esp0 = (u32)this;

	// そのプロセッサ用のTSSを設定
	const u64 p((munit)&tss);
	__TSSPH[cpuid] |= ((p & 0x00ffffff) << 16) | ((p & 0xff000000) << 32);
	asm volatile("ltr %%ax" :: "a"(TSSSel + cpuid));

	// TODO:スケジューラを呼ぶ
assert(true);
asm volatile("sti");
asm volatile("int $0x20");
assert(true);
for(;;){ dputc('.'); asm volatile("hlt"); };
}






void* CPU::operator new(munit size, uint cpuid){
	return (void*)&kernelStacks[cpuid][4096 - size];
}

