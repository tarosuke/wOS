/*********************************************************** INTERRUPT HANDLER
 *	Copyright (C) 2011- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _INTERRUPT_
#define _INTERRUPT_

#include <types.h>
#include <config.h>
#include <arch.h>
#include <pu.h>
#include <debug.h>


class INTERRUPT : private ARCH::INTERRUPT{
public:
	INTERRUPT(){
		dputs("interrupt..." INDENT);
		dputs(UNINDENT "OK.\n");
	};
	static inline void Handler(uint irq){
		if(CF_MAX_IRQs <= irq){
			dprintf("irq %d not supported.\n", irq);
			assert(false);
			return;
		}
		HANDLER& h(handlers[irq]);
		PIC::Start(irq); //エッジトリガなら前EOIを出す
		if(h.handler){
			h.handler();
			PIC::Finish(irq); //レベルトリガなら後EOIを出す
		}else{
			h.rc = 0;
			TASK* t;
			for(QUEUE<TASK>::ITOR i(h.tasks); (t = i++); h.rc++){
				(*t).WakeupByInterrupt(irq);
			}
		}

		//タスクディスパッチ
		PU::Dispatch();
	};
	static void Finish(uint irq){
		if(!--handlers[irq].rc){
			PIC::Finish(irq);
		}
	};
	static void RegisterHandler(uint irq, void (*handler)()){
		// ハンドラの登録(タイマしか使わないけどなw)
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
		//TODO:EOI情報を覚えておく
	}handlers[];
};





#endif
