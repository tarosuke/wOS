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


PU::PU() : current(&idle){
	/// アイドルスタックの設定
	idle.RestoreStack();

	/// 初期化ロックを解除 TODO:CPUのコンストラクタに移動
	CPU::ReleaseBootLock();

	/// ディスパッチとアイドル
	for(;;){
		//TODO:負荷測定とかもする
#if 4 < CF_DEBUG_LEVEL
		if(!cpuid && 10000000U <= CLOCK::Uptime()){
			hprintf("[%t]\r", CLOCK::GetLocalTime());
		}
#endif

		//grave-keeping(graveに置いてある構造を解放)
		for(TASK* t; (t = grave.Get()); delete t);

		// 本当にアイドル
		CPU::Idle();
	}
};


void PU::_Dispatch(){
	DisableInterrupt();
	if(!!(next = ready.Get((*current).priority))){
		//次のタスクがあるので切り替え
		//TODO:CPUの方でやるほうがスマートか？
		if((*next).newbie){
			//新規タスクはスタックフレームがないので直接移行
			(*next).newbie = false;
			(*next).DiveInto();
		}else{
			//既存タスクはリターンで
			(*next).DispatchTo();
		}

		//タスクをレディキューに戻す(ただしゾンビとアイドルは除く)
		if((*current).priority < TASK::__pri_max){
			ready.Add(*current);
		}
		current = next;
		next = 0;
	}
}
