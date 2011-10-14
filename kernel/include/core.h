/***************************************************************** KERNEL CORE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
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
	static const tunit tickTime = 1000000ULL / CF_HZ;
	static const uint systemCallIRQ = 16;
	static tunit uptime;
	static void SystemCall();
public:
	CORE(){
		dputs("core..." INDENT);
		INTERRUPT::RegisterHandler(systemCallIRQ, SystemCall);
		dputs(UNINDENT "OK.\n");
	};
	static inline void Tick(){
		uptime += tickTime;
	};
	static tunit GetUptime(){
		tunit now;
		do{ now = uptime; }while(now != uptime);
		return now;
	};
};



#endif


