/************************************************************ CPU MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#ifndef _CPU_
#define _CPU_


#include <types.h>
#include <config.h>

class CPU{
	UNDEFAULT(CPU);
protected:
	CPU(uint id);
private:
	struct TSS{
		u32 link;
		u32 esp0;
		u32 ss0;
		u32 esp1;
		u32 ss1;
		u32 esp2;
		u32 ss2;
		u32 cr3;
		u32 eip;
		u32 eflags;
		u32 eax;
		u32 ecx;
		u32 edx;
		u32 ebx;
		u32 esp;
		u32 ebp;
		u32 esi;
		u32 edi;
		u32 es;
		u32 cs;
		u32 ss;
		u32 ds;
		u32 fs;
		u32 gs;
		u32 ldtss;
		u32 iobase;
	}__attribute__((packed));
	const uint cpuid;
	TSS& tss;
	static TSS tsss[];
	static uint idPool;
	static uint GetID();
};


#endif

