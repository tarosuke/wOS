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

TASK::TASKQUEUE PU::ready;


void PU::Dispatch(){
	// 最低優先度タスクを実行中のプロセッサを探す
	TASK::PRIORITY pri(TASK::__pri_max);
#if CF_SMP
	uint n(CF_MAX_PROCESSORs);
	for(uint p(0); i < CF_MAX_PROCESSORs; i++){
		if(pus[p].priority < pri){
			// 最低優先度更新
			n = p;
			pri = pus[p].priority;
		}
	}
#else
	pri = pus[0].priority;
#endif

	// レディキューに最低優先度以上のタスクがあったらそのプロセッサにディスパッチ要求
	const uint max(ready.GetMax());
	if(max < pri){
//		TASK* const t(ready.Get());
#if SMP
		//TODO:ディスパッチ対称が自分でなければ要求をIPI
#else
		//ディスパッチ
		//TODO:メッセージを受け取ってユーザモードへ落ちる
#endif
	}
	//ディスパッチ不要
}

