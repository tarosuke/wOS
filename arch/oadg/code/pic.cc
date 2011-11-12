/*********************************************************** INTERRUPT HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <arch/pic.h>


uint PIC::irqMask(0xfffb);
const uint PIC::levels(elcr.in16(0));
static PIC pic;
