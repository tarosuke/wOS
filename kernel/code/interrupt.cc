/*********************************************************** INTERRUPT HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <interrupt.h>


static INTERRUPT interruptManipulator;
INTERRUPT::HANDLER INTERRUPT::handlers[CF_MAX_IRQs];

extern "C"{
	void __INTERRUPT_Handler(uint irq){
		INTERRUPT::Handler(irq);
	}
}
