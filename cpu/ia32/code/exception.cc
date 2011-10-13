/*********************************************************** EXCEPTION HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <cpu/exception.h>


extern "C"{
	void __FAULT_Handler(uint num, uint err){ /*INTERRUPT::Handler(irq);*/ assert(false); }
	extern u32 __ExceptionTable[CF_MAX_IRQs][4];
}

u64 EXCEPTION::vector[systemExceptions + CF_MAX_IRQs]__attribute__((aligned(8)));
const EXCEPTION::IDTP EXCEPTION::idtp = { systemExceptions + CF_MAX_IRQs, vector };
static EXCEPTION exeptionHandler;

EXCEPTION::EXCEPTION(){
	dputs("exception..." INDENT);
	for(uint n(0); n < systemExceptions + CF_MAX_IRQs; n++){
		u64 p((munit)__ExceptionTable[n]);
		vector[n] = 0x0000860000000000ULL | 0x00080000ULL | ((p & 0xffff0000ULL) << 16) | (p & 0xffffULL);
	}
	asm volatile("lidt %0" :: "m"(idtp));
	dputs(UNINDENT "OK.\n");
}

void EXCEPTION::RegisterFault(uint num, void (*handler)(u32)){
}

