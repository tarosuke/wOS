/*********************************************************** INTERRUPT HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <interrupt.h>


static INTERRUPT interruptManipulator;
void (*INTERRUPT::handlers[CF_MAX_IRQs])();

extern "C"{ void __INTERRUPT_Handler(uint irq){ INTERRUPT::Handler(irq); } }
