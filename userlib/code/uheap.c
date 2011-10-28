/************************************************************************ HEAP
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <userlib/uheap.h>
#include <userlib/task.h>


extern unsigned __kernel_base[];
static const unsigned sizes[32] = {
	64U, 128U, 256U, 512U, 1024U, 2048U, 4096U, 8192U, 16384U, 32768U,
	65536, 131072U, 262144U, 524288U, 1048576U, 2097152U, 4194304U,
	8388608U, 16777216, 33554432U, 67108864U, 134217728U, 268435456U,
	536870912U, 1073741824U, 2147483648U
};
#define HEAP UHEAP* const heap = taskHead.heapStart

void	HEAP_Init(void){
	HEAP;
	unsigned i;
	(*heap).top = (unsigned)&heap[1];
	(*heap).limit = (unsigned)__kernel_base - taskHead.stackSize;
	(*heap).anchorStack.whole = 0;
	for(i = 0; i < 32; i++){
		(*heap).stack[i].whole = 0;
	}
}


void* HEAP_Get(unsigned size){
	HEAP;
	unsigned p;
	unsigned getSize;
	void* r = 0;

	//プール番号取得
	for(p = 0; p < 32 && sizes[p] < size; p++);
	if(32 <= p){
		//そんなでかいプールはない
		return 0;
	}

	//要求サイズを確保サイズに更新
	size = sizes[p];

	if((*heap).stack[p].mem){
		//プールから取得 TODO:関数ぽくしてcpuモジュールへ移動しておくこと
		asm volatile(
			"mov (%%edi), %%eax;"
			"mov 4(%%edi), %%edx;"
			"1: mov (%%eax), %%ebx;"
			"inc %%edx;"
			"lock cmpxchg8b (%%edi);"
			"jnz 1b"
			: "=a"(r) : "D"(&(*heap).stack[p]) : "ebx", "ecx", "edx");
	}

	if(!r){
		//ヒープからはページサイズ未満のサイズは切り出せないので、要求サイズが小さければページサイズを切り出して分解する
		getSize = size < 4096 ? 4096 : size;

		//ヒープから取得
		asm volatile(
			"mov (%%edi), %%eax;"
			"mov %1, %%edx;" //取得サイズ
			"1: add %%eax, %%edx;"
			"cmp %2, %%edx;" //リミットチェック
			"jae 1f;" //リミットオーバー
			"lock cmpxchg %%edx, (%%edi);"
			"jnz 1b;"
			"jmp 2f;"
			"1: xor %%eax, %%eax;"
			"2:"
			: "=a"(r) : "g"(getSize), "g"((*heap).limit), "D"(&(*heap).top) : "edx");
		
		//TODO:要求サイズが小さかったら分解
		if(size < getSize){
			
		}
	}

	if(!r){
		//out of heap
		return 0;
	}

	//TODO:対象ページをイネーブル(ulibの別の関数)

	return r;
}


