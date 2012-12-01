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
#include <heap.h>


extern "C"{
#if CF_IA32
	extern u64 __TSSPH[];
#endif
#if CF_AMD64
	extern u64 __TSSPH[][2];
#endif
	extern char __BOOTLOCK;
};


CPU::TSS CPU::tsss[CF_MAX_PROCESSORs]__attribute__((aligned(1024)));

LOCK CPU::lock;
CPU::APIC CPU::apic __attribute__((init_priority(59535)));


CPU::APIC::APIC() :
	body((u32*)HEAP::Assign(0xfee00000U, PAGESIZE)){};


CPU::CPU() :
	cpuid(GetID()),
	tss(tsss[cpuid]){
	KEY key(lock);
#if CF_SMP
	dprintf("cpu(%d)..."INDENT, cpuid);

	if(apic.IsReady()){
		hputs("checking APIC..."INDENT);
		hprintf("mapped APIC register:%p.\n", apic);
		hprintf("APIC ver: %x.\n", apic.body[12] & 255);
		hprintf("APIC LVT timer: %x.\n", apic.body[0x320 / 4]);
		hprintf("APIC LVT temp: %x.\n", apic.body[0x340 / 4]);
		hputs(UNINDENT"OK.\n");

		if(!cpuid){
#if 0
			hputs("waking up APs...NMI...");
			apic.body[0xc0] = 0xc4400;
#else
			hputs("waking up APs...INIT...");
			apic.body[0xc0] = 0xc4500;
			while(apic.body[0xc0] & 0x1000);
			hputs("SIPI(1)...");
			apic.body[0xc0] = 0xc4610;
			while(apic.body[0xc0] & 0x1000);
			hputs("SIPI(2)...");
			apic.body[0xc0] = 0xc4610;
#endif
			hputs("OK.\n");
		}
	}
#endif

	//  当該プロセッサ用のTSSを設定
	hprintf("selector: %d.\n", TSSSel + cpuid * 8);
	const u64 p((munit)&tss);
#if CF_IA32
	__TSSPH[cpuid] = 0x000089000000006c |
		((p & 0x00ffffff) << 16) |
		((p & 0xff000000) << 32);
	hprintf("tss: %p.\n", &tss);
	hprintf("desc: %016llx.\n", __TSSPH[cpuid]);
	asm volatile(
		"ltr %%ax; mov %%ss, %0"
		: "=r"(tss.ss0) : "a"(TSSSel + cpuid * 8));
#endif
#if CF_AMD64
	__TSSPH[cpuid][0] = 0x000089000000006c |
		((p & 0x00ffffff) << 16) |
		((p & 0xff000000) << 32);
	__TSSPH[cpuid][1] = p >> 32;
	hprintf("tss: %p.\n", &tss);
	hprintf("desc(0): %016llx.\n", __TSSPH[cpuid][0]);
	hprintf("desc(1): %016llx.\n", __TSSPH[cpuid][1]);
	asm volatile("ltr %%ax" :: "a"(TSSSel + cpuid * 16));
#endif

	// IDTの設定
	EXCEPTION::LoadIDT();

	dputs(UNINDENT"OK.\n");
}

void CPU::ReleaseBootLock(){
	//ロック解除
	__BOOTLOCK = 0;
}

