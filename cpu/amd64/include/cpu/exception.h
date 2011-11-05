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
public:
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
				u64 rip;
				u64 cs;
				u64 rflags;
				u64 rsp;
				u64 ss;
			}noErrorCode;
			struct{
				u64 errorCode;
				u64 rip;
				u64 cs;
				u64 rflags;
				u64 rsp;
				u64 ss;
			}withErrorCode;
		};
	};
	EXCEPTION();
	void RegisterFault(uint num, void (*handler)(u32));
private:
	static const uint systemExceptions = 32;
	static u64 vector[][2];
	static const struct IDTP{ u16 limit; void* table; }__attribute__((packed)) idtp;
};


#endif

