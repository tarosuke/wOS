/*********************************************************** EXCEPTION HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _CPU_EXCEPTION_
#define _CPU_EXCEPTION_


#include <types.h>
#include <config.h>
#include <debug.h>


class EXCEPTION{
	EXCEPTION(EXCEPTION&);
	void operator=(EXCEPTION&);
public:
	static const uint systemExceptions = 32;
#if CF_IA32
	struct FRAME{
		u16 es;
		u16 ds;
		u32 edi;
		u32 esi;
		u32 ebp;
		u32 esp;
		u32 ebx;
		u32 edx;
		u32 ecx;
		u32 eax;
		union{
			struct{
				u32 ip;
				u32 cs;
				u32 flags;
				u32 sp;
				u32 ss;
			}noErrorCode;
			struct{
				u32 errorCode;
				u32 ip;
				u32 cs;
				u32 flags;
				u32 sp;
				u32 ss;
			}withErrorCode;
		};
	}__attribute__((packed));
#endif
#if CF_AMD64
	struct FRAME{
		u64 r15;
		u64 r14;
		u64 r13;
		u64 r12;
		u64 r11;
		u64 r10;
		u64 r9;
		u64 r8;
		u64 rbp;
		u64 rsi;
		u64 rdx;
		u64 rcx;
		u64 rbx;
		u64 rax;
		u64 rdi;
		union{
			struct{
				u64 ip;
				u64 cs;
				u64 flags;
				u64 sp;
				u64 ss;
			}noErrorCode;
			struct{
				u64 errorCode;
				u64 ip;
				u64 cs;
				u64 flags;
				u64 sp;
				u64 ss;
			}withErrorCode;
		};
	}__attribute__((packed));
#endif
	EXCEPTION();
	typedef void (*HANDLER)(u32, FRAME&);
	static void RegisterHandler(uint num, HANDLER);
	static void LoadIDT();
private:
#if CF_IA32
	static u64 vector[];
#endif
#if CF_AMD64
	static u64 vector[][2];
#endif
	static const struct IDTP{
		u16 limit;
		void* table;
	}__attribute__((packed)) idtp;
};


#endif

