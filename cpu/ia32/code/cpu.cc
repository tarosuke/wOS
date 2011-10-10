/************************************************************ CPU MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <config.h>
#include <cpu/cpu.h>
#include <arch/segments.h>


CPU::TSS CPU::tss[CF_MAX_PROCESSORs]__attribute__((aligned(1024)));


void CPU::SetupTSSDescriptor(u64* e){
	for(uint i(0); i < CF_MAX_PROCESSORs; i++, e++){
		const u64 p((munit)&tss[i]);
		*e |= ((p & 0x00ffffff) << 16) | ((p & 0xff000000) << 32);
	}
}


//TODO:そのプロセッサ用のスタックを割り当ててから、そのスタックの上にCPUのインスタンスを生成
CPU::CPU(uint id) : cpuid(id), mytss(tss[id]){
	// そのプロセッサ用のTSSを設定
	asm volatile("ltr %%ax" :: "a"(TSSSel + id));
}
