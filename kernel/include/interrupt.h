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
public:
	INTERRUPT(){
		dputs("interrupt..." INDENT);
		dputs(UNINDENT "OK.\n");
	};
	static void Handleer(uint irq){
		PIC::Start(irq);
		// TODO:タイマ割り込みだったら直接カーネル内のTick(たぶんCORE::Tick)を呼ぶ
		// TODO:割り込み処理終了のマークが全部付いたらPIC::Finishを呼ぶ
	};
};





#endif
