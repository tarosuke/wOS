/************************************************************ CPU MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
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
		u32 reserved0;
		void* rsp[3];
		u64 reserved1;
		void* ist[7];
		u64 reserved2;
		u16 reserved3;
		u16 iobase;
	}__attribute__((packed));
	const uint cpuid;
	TSS& tss;
	static TSS tsss[];
	static uint idPool;
	static uint GetID();
};


#endif

