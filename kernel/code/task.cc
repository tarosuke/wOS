/************************************************************************ TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <task.h>
#include <heap.h>
#include <pu.h>
#include <debug.h>


const uint TASK::thisSizeIndex(HEAP::GetBlockIndex(PAGESIZE));

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


void* TASK::operator new(munit contentSize){
	void* const r(HEAP::GetByIndex(thisSizeIndex));
	assert(r);
	return r;
}

void TASK::operator delete(void* mem){
	HEAP::Release(mem, thisSizeIndex);
}

