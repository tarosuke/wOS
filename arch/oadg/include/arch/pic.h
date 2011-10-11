/************************************************************ PIC MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#ifndef _ARCH_PIC_
#define _ARCH_PIC_

#include <config.h>
#include <cpu/io.h>


class PIC{
private:
	SBIO<0x20> mpic;
	SBIO<0xa0> spic;
	uint irqMask;
	static const uint irqBaseVector = 0x20;
public:
	PIC();
};


#endif
