/*********************************************************** INTERRUPT HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#ifndef _INTERRUPT_
#define _INTERRUPT_

#include <types.h>
#include <config.h>
#include <arch/pic.h>
#include <cpu/exception.h>
#include <debug.h>

class INTERRUPT{
private:
	static PIC pic;
public:
	INTERRUPT(){
		dputs("INTERRUPT..." INDENT);
		dputs(UNINDENT "OK.\n");
	};
};





#endif
