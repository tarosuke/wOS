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
	static void (*handlers[])();
public:
	INTERRUPT(){
		dputs("interrupt..." INDENT);
		dputs(UNINDENT "OK.\n");
	};
	static inline void Handler(uint irq){
		if(handlers[irq]){
			handlers[irq](); //EOIはハンドラが出す
		}else{
			PIC::Start(irq);
			// TODO:タスクが登録されてたら起動する。全く登録されてなかったらFinish
		}
	};
	static void Finish(uint irq){
		//TODO:Handlerで起動した割り込み処理が全部終わったら呼ばれる
		PIC::Finish(irq);
	};
	static void RegisterHandler(uint irq, void (*handler)()){
		// ハンドラの登録(タイマとシステムコールしか使わないけどなw)
		handlers[irq] = handler;
	};
};





#endif
