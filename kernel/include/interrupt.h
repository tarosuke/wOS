/*********************************************************** INTERRUPT HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _INTERRUPT_
#define _INTERRUPT_

#include <types.h>
#include <config.h>
#include <arch/pic.h>
#include <cpu/exception.h>
#include <pu.h>
#include <debug.h>


class INTERRUPT{
public:
	INTERRUPT(){
		dputs("interrupt..." INDENT);
		dputs(UNINDENT "OK.\n");
	};
	static inline void Handler(uint irq, bool dispatchable){
		if(CF_MAX_IRQs <= irq){
			dprintf("irq %d not supported.\n", irq);
			assert(false);
			return;
		}
		HANDLER& h(handlers[irq]);
		if(h.handler){
			h.handler(); //この場合EOIはハンドラが出す
		}else{
			PIC::Start(irq);
			h.rc = 0;
			TASK* t;
			for(QUEUE<TASK>::ITOR i(h.tasks); (t = i++); h.rc++){
				(*t).WakeupByInterrupt(irq);
			}
		}

		//必要ならタスクディスパッチ
		if(dispatchable){
			PU::Dispatch();
		}
	};
	static void Finish(uint irq){
		if(!--handlers[irq].rc){
			PIC::Finish(irq);
		}
	};
	static void RegisterHandler(uint irq, void (*handler)()){
		// ハンドラの登録(タイマとシステムコールしか使わないけどなw)
		handlers[irq].handler = handler;
		PIC::Unmask(irq);
	};
	static void RegisterHandler(uint irq, NODE<TASK>& task){
		// ハンドラタスクの登録
		handlers[irq].tasks.Add(task);
		PIC::Unmask(irq);
	};
private:
	static struct HANDLER{
		void (*handler)();
		QUEUE<TASK> tasks;
		uint rc; //現在実行中のハンドラタスクの数
	}handlers[];
};





#endif
