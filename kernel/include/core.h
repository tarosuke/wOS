/***************************************************************** KERNEL CORE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */


#ifndef _CORE_
#define _CORE_

#include <types.h>
#include <config.h>
#include <interrupt.h>
#include <debug.h>


class CORE{
	CORE(CORE&);
private:
	static const uint systemCallIRQ = 16;
	static void SystemCall();
public:
	CORE(){
		dputs("core..." INDENT);
		INTERRUPT::RegisterHandler(systemCallIRQ, SystemCall);
		dputs(UNINDENT "OK.\n");
	};
};



#endif
