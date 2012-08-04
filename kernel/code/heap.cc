/***************************************************************** KERNEL HEAP
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <heap.h>


extern "C"{
	extern uint __kernel_heap[];
}
static HEAP heap((munit)__kernel_heap, VIRTUALPAGE::heapTop);
HEAP::STACK HEAP::memoryPools[32];
const munit HEAP::memoryPoolSizes[32] = {
	64U, 128U, 256U, 512U, 1024U, 2048U, 4096U, 8192U, 16384U, 32768U,
	65536, 131072U, 262144U, 524288U, 1048576U, 2097152U, 4194304U,
	8388608U, 16777216, 33554432U, 67108864U, 134217728U, 268435456U,
	536870912U, 1073741824U, 2147483648U
};
munit HEAP::top;
munit HEAP::limit;
LOCK HEAP::lock;


HEAP::HEAP(munit start, munit limit){
	dputs("kernel heap..." INDENT);
	top = start;
	HEAP::limit = limit;
	const munit size(limit - start);
	dprintf(UNINDENT "OK(start:%08x size:%m).\n", top, size);
}

HEAP::BLOCK HEAP::Get(munit size){
	BLOCK mb;
	const uint index(GetBlockIndex(size));
	size = memoryPoolSizes[index];
	mb.index = index;
	mb.mem = GetByIndex(index);
	return mb;
}

void* HEAP::GetByIndex(uint index){
	void* r(0);
	const munit size(memoryPoolSizes[index]);
	if(!(r = memoryPools[index].Pop())){
		KEY key(lock);
		const munit newTop(top + size);
		if(limit <= newTop){
			dputs("out of heap.\n");
			return 0;
		}
		VIRTUALPAGE::Enable((void*)top, size / PAGESIZE);
		r = (void*)top;
		top = newTop;
	}
	return r;
}

void HEAP::Release(HEAP::BLOCK& mb){
	memoryPools[mb.index].Push(mb.mem);
}

uint HEAP::GetBlockIndex(munit size){
	uint i(0);
	for(; i < 32 && size < memoryPoolSizes[i]; i++);
	return i;
}



// 簡単なヒープ操作
void* operator new(munit size){
	const HEAP::BLOCK mb(HEAP::Get(size + sizeof(uint)));
	uint* const mem((uint*)mb.mem);
	mem[0] = mb.index;
	return &mem[1];
}
void operator delete(void* mem){
	HEAP::BLOCK mb = { mem, ((uint*)mem)[-1] };
	HEAP::Release(mb);
}
