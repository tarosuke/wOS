/*********************************************************** EXCEPTION HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
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
				u32 rip;
				u32 cs;
				u32 rflags;
				u32 rsp;
				u32 ss;
			}noErrorCode;
			struct{
				u32 errorCode;
				u32 rip;
				u32 cs;
				u32 rflags;
				u32 rsp;
				u32 ss;
			}withErrorCode;
		};
	}__attribute__((packed));
	EXCEPTION();
	typedef void (*HANDLER)(u32, FRAME&);
	void RegisterFault(uint num, HANDLER);
private:
	static u64 vector[];
	static const struct IDTP{
		u16 limit; u64* table; }__attribute__((packed)) idtp;
};


#endif

