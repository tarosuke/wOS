/************************************************************************ TASK
 *	Copyright (C) 2011- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <task.h>
#include <heap.h>
#include <pu.h>
#include <message.h>
#include <realPage.h>
#include <debug.h>


const uint TASK::thisSizeIndex(HEAP::GetBlockIndex(PAGESIZE));
TASK::CRONQUEUE TASK::cronQueue;





























#if 0



/// 現在のコンテキストをタスクにする。アイドルタスクのためのタスク。
TASK::TASK() :
	owner(0),
	originalPriority(__pri_max),
	priority(__pri_max),
	qNode(this),
	cronNode(this),
	irqNode(this),
	irq(-1),
	reason(RS_FINE),
	newbie(false){
		capabilities.raw = 0;
}


/// 実行形式ファイルやメモリの一部を指定する普通のタスク
TASK::TASK(MAP& map) :
	owner(0),
	originalPriority(__pri_max),
	priority(__pri_max),
	qNode(this),
	cronNode(this),
	irqNode(this),
	irq(-1),
	reason(RS_FINE),
	newbie(true){
	RESOURCE code(map);
}






void* TASK::operator new(munit contentSize){
	void* const r(HEAP::GetByIndex(thisSizeIndex));
	assert(r);
	return r;
}

void TASK::operator delete(void* mem){
	HEAP::Release(mem, thisSizeIndex);
}


void TASK::Wakeup(TASK::PRIORITY p){
	if(p < priority){
		//もしタスクが割り込み以上の優先度なら起動処理は不要
		priority = p;
		(*owner).WakeUp(*this);
	}
};

void TASK::WakeupByMessage(MESSAGE& m){
	reason = RS_FINE;
	in.Add(m.priority, m.node);
	Wakeup(m.priority);
};


TASK::~TASK(){
	///TODO:ユーザライブラリ内でユーザ空間とリソース類を解放
}
#endif
