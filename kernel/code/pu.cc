/**************************************** GENERIC PROCESSING UNIT MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <pu.h>
#include <lock.h>
#include <debug.h>
#include <core.h>


PU::PUPLACER PU::pus;
ATOMIC PU::numOfPu;

extern "C"{
	//割り込みorトラップハンドラの戻り先がユーザプロセスの時に呼ばれる
	//呼ばれるのはレジスタを復帰する前
	void Dispach(){
		PU::Dispatch();
	}
}


void PU::DispatchItsOwn(){
	DisableInterrupt();
	while(true){
		next = ready.Get(current ? (*current).priority : TASK::__pri_max);
		if(next){
			(*current).SaveStack();
			if((*next).newbie){
				(*next).newbie = false;
				(*next).DiveInto();
			}else{
				(*next).DispatchTo();
			}
			ready.Add(*current);
			current = next;
			next = 0;
		}else if(!current){
			//アイドル
			//TODO:負荷測定とかもする
			EnableInterrupt();
			Idle();
			DisableInterrupt();
		}
		break;
	}
}
