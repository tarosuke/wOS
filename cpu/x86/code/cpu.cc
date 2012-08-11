/************************************************************ CPU MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <config.h>
#include <cpu/cpu.h>
#include <arch/segments.h>
#include <debug.h>
#include <arch/pic.h>
#include <task.h>


extern "C"{
#if CF_IA32
	extern u64 __TSSPH[];
#endif
#if CF_AMD64
	extern u64 __TSSPH[][2];
#endif
};


CPU::TSS CPU::tsss[CF_MAX_PROCESSORs]__attribute__((aligned(1024)));


CPU::CPU() : cpuid(GetID()), idleStack(GetStack()), tss(tsss[cpuid]){
	dprintf("cpu(%d)..."INDENT, cpuid);

	//  当該プロセッサ用のTSSを設定
	dprintf("selector: %d.\n", TSSSel + cpuid);
	const u64 p((munit)&tss);
#if CF_IA32
	__TSSPH[cpuid] = 0x000089000000006c |
		((p & 0x00ffffff) << 16) |
		((p & 0xff000000) << 32);
	dprintf("tss: %p.\n", &tss);
	dprintf("desc: %016llx.\n", __TSSPH[cpuid]);
	asm volatile(
		"ltr %%ax; mov %%ss, %0"
		: "=r"(tss.ss0) : "a"(TSSSel + cpuid));
#endif
#if CF_AMD64
	__TSSPH[cpuid][0] = 0x000089000000006c |
		((p & 0x00ffffff) << 16) |
		((p & 0xff000000) << 32);
	__TSSPH[cpuid][1] = p >> 32;
	dprintf("tss: %p.\n", &tss);
	dprintf("desc(0): %016llx.\n", __TSSPH[cpuid][0]);
	dprintf("desc(1): %016llx.\n", __TSSPH[cpuid][1]);
	asm volatile("ltr %%ax" :: "a"(TSSSel + cpuid));
#endif

	asm volatile("sti");

	dputs(UNINDENT"OK.\n");
}

