/************************************************************ CPU MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _CPU_
#define _CPU_

#include <types.h>
#include <config.h>
#include <lock.h>


class TASK;
class CPU{
	CPU(CPU&);
	void operator=(CPU&);
public:
	static inline void EnableInterrupt(){
		asm volatile("sti");
	};
	static inline void DisableInterrupt(){
		asm volatile("cli");
	};
	//暇なのでhltして待つ
	static void Idle(){
		asm volatile("hlt");
	};
protected:
	CPU();
	const uint cpuid;
	static inline uint GetID(){
#if !CF_SMP
		return 0;
#else
		u16 id;
		asm volatile("mov %%gs, %0" : "=r"(id));
		return id;
#endif
	};
private:
#if CF_IA32
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
#endif
#if CF_AMD64
	struct TSS{
		u32 reserved0;
		void* rsp[3];
		u64 reserved1;
		void* ist[7];
		u64 reserved2;
		u16 reserved3;
		u16 iobase;
	}__attribute__((packed));
#endif
	TSS& tss;
	static TSS tsss[];
	class APIC{
	public:
		volatile u32* const body;
		APIC();
		bool IsReady(){ return !!body; };
	}apic;
	static LOCK lock;
};


#endif
