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
private:
	static const uint systemExceptions = 32;
	static u64 vector[][2];
	static const struct IDTP{ u16 limit; void* table; }__attribute__((packed)) idtp;
public:
	EXCEPTION();
	void RegisterFault(uint num, void (*handler)(u32));
};


#endif

