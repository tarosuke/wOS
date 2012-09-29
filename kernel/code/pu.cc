/**************************************** GENERIC PROCESSING UNIT MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <pu.h>
#include <lock.h>
#include <debug.h>
#include <core.h>
#include <clock.h>


PU::PUPLACER PU::pus;
ATOMIC PU::numOfPu;
QUEUE<TASK> PU::grave;

extern "C"{
	//割り込みorトラップハンドラの戻り先がユーザプロセスの時に呼ばれる
	//呼ばれるのはレジスタを復帰する前
	void Dispach(){
		PU::Dispatch();
	}
}


void PU::_Dispatch(){
	for(;;){
		DisableInterrupt();
		next = ready.Get((*current).priority);
		if(next){
			(*current).SaveStack();
			if((*next).newbie){
				(*next).newbie = false;
				(*next).DiveInto();
			}else{
				(*next).DispatchTo();
			}
			if((*current).priority < TASK::__pri_max){
				//ゾンビやアイドルはレディキューには戻さない。
				ready.Add(*current);
			}
			current = next;
			next = 0;
			break;
		}
		if(current == &idle){
			//アイドル
			//TODO:負荷測定とかもする
			#if 4 < CF_DEBUG_LEVEL
			if(!cpuid && 10000000U <= CLOCK::Uptime()){ hprintf("[%t]\r", CLOCK::GetLocalTime()); }
			#endif
			//grave-keeping(graveに置いてある構造を解放)
			for(TASK* t; (t = grave.Get()); delete t);
			// 完全に休み
			Idle();
		}
	}
}


