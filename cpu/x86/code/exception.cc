/*********************************************************** EXCEPTION HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <cpu/exception.h>
#include <arch/segments.h>
#include <cpu/virtualPage.h>


static EXCEPTION::HANDLER handlers[EXCEPTION::systemExceptions] = {
	0, //0
	0, //1
	0, //2
	0, //3
	0, //4
	0, //5
	0, //6
	0, //7
	0, //8
	0, //9
	0, //10
	0, //11
	0, //12
	0, //13:GPE
	VIRTUALPAGE::Fault, //14:PageFault
	0, //15
	0, //16
	0, //17
	0, //18
	0, //19
	0, //20
	0, //21
	0, //22
	0, //23
	0, //24
	0, //25
	0, //26
	0, //27
	0, //28
	0, //29
	0, //30
	0, //31
};



extern "C"{
	void __FAULT_Handler(uint num, uint err, EXCEPTION::FRAME* f){
		if(handlers[num]){
			handlers[num](err, *f);
		}else{
			EXCEPTION::FRAME frame(*f);
			dprintf("EXCEPTION(%d:%08x)."INDENT, num, err);
			dprintf("ip:%p.\n", frame.withErrorCode.ip);
			dprintf("cs:%llx.\n", frame.withErrorCode.cs);
			dprintf("fl:%08llx.\n", frame.withErrorCode.flags);
			dprintf("sp:%p.\n", frame.withErrorCode.sp);
			dprintf("ss:%llx.\n"UNINDENT, frame.withErrorCode.ss);
			assert(false);
		}
	}
	extern u32 __ExceptionTable[CF_MAX_IRQs][4];
}

#if CF_IA32
u64 EXCEPTION::vector[systemExceptions + CF_MAX_IRQs]__attribute__((aligned(8)));
const EXCEPTION::IDTP EXCEPTION::idtp = { (systemExceptions + CF_MAX_IRQs) * 8, (void*)vector };
#endif
#if CF_AMD64
u64 EXCEPTION::vector[systemExceptions + CF_MAX_IRQs][2]__attribute__((aligned(8)));
const EXCEPTION::IDTP EXCEPTION::idtp = { (systemExceptions + CF_MAX_IRQs) * 16, (void*)vector };
#endif
static EXCEPTION exeptionHandler;

EXCEPTION::EXCEPTION(){
	dputs("exception..." INDENT);
	for(uint n(0); n < systemExceptions + CF_MAX_IRQs; n++){
		u64 p((munit)__ExceptionTable[n]);
#if CF_IA32
		vector[n] = 0x00008e0000000000ULL | (KCSel << 16) | ((p & 0xffff0000ULL) << 32) | (p & 0xffffULL);
#endif
#if CF_AMD64
		vector[n][0] = 0x00008e0000000000ULL | (KCSel << 16) |
		((p & 0xffff0000ULL) << 32) |
		(p & 0xffffULL);
		vector[n][1] = p >> 32;
#endif
	}
	asm volatile("lidt %0" :: "m"(idtp));
	dputs(UNINDENT "OK.\n");
}

void EXCEPTION::RegisterHandler(uint num, HANDLER handler){
	assert(num < systemExceptions);
	assert(!handlers[num]);
	handlers[num] = handler;
}