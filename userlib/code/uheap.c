/************************************************************************ HEAP
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <userlib/uheap.h>
#include <userlib/task.h>


//TODO:ファイル名にheapを使うとカーネル本体のHEAPと混同するので分離してビルドする方法を考える

typedef struct{
	void* link;
	void* target;
}ANCHOR;

typedef struct{
	munit* mem;
	unsigned sizeIndex;
}MEM;

typedef struct{
	punit top; // ヒープトップのページ番号
	punit limit; //ヒープの上限(ページ番号)
	ANCHOR* anchorStack; //解放後領域をスタックに維持するためだけのスタック
	ANCHOR* stack[32]; //解放された領域のサイズごとスタック
}UHEAP;


extern unsigned __kernel_base[];
static const munit sizes[32] = {
	64U, 128U, 256U, 512U, 1024U, 2048U, 4096U, 8192U, 16384U, 32768U,
	65536, 131072U, 262144U, 524288U, 1048576U, 2097152U, 4194304U,
	8388608U, 16777216, 33554432U, 67108864U, 134217728U, 268435456U,
	536870912U, 1073741824U, 2147483648U
};




#define HEAP UHEAP* const heap = taskHead.heap
void	HEAP_Init(void){
	HEAP;
	unsigned i;
	(*heap).top = ((munit)&heap[1] + PAGESIZE - 1) / PAGESIZE;
	(*heap).limit =
		((munit)__kernel_base - taskHead.stackSize) / PAGESIZE;
	(*heap).anchorStack = 0;
	for(i = 0; i < 32; i++){
		(*heap).stack[i] = 0;
	}
}

static void Push(ANCHOR** stack, void* node){
	(*(ANCHOR*)node).link = (**stack).link;
	*stack = node;
}

static void* Pop(ANCHOR** stack){
	ANCHOR* r = *stack;
	*stack = (*r).link;
	return r;
}

static MEM Get(munit size){
	HEAP;
	unsigned si;
	ANCHOR** target;
	munit* r = 0;
	MEM mem = { 0, 0 };

	//サイズインデクスを求める
	for(si = 0; size < sizes[si] && si < 32; si++);
	if(32 <= si){
		//そんな大きなプールはない
		return mem;
	}
	size = sizes[si];
	target = &(*heap).stack[si];

	if(*target){
		//プールから取得
		ANCHOR* a = Pop(target);
		if(size < PAGESIZE){
			//PAGESIZE未満のスタックには対象領域がそのまま入っている
			r = (void*)a;
		}else{
			//PAGESIZE以上のスタックにはANCHORが入っている
			r = (*a).target;
			Push(&(*heap).anchorStack, a);
		}
	}

	if(!r){
		//ヒープから取得
		r = (void*)((*heap).top * PAGESIZE);
		if(size < PAGESIZE){
			//PAGESIZE未満なのでヒープは1ページ進めて余りはスタックへ
			unsigned i;
			(*heap).top++;
			for(i = size; i < PAGESIZE; i += size){
				Push(target, &r[i / sizeof(unsigned)]);
			}
		}else{
			//PAGESIZE以上なのでヒープをそのまま進める
			(*heap).top += size / PAGESIZE;
		}
	}

	if(!r){
		//out of heap
		return mem;
	}

	mem.mem = r;
	mem.sizeIndex = si;
	return mem;
}

void* HEAP_Get(munit size){
	MEM mem = Get(size + sizeof(munit));
	if(!mem.mem){ return 0; }
	//TODO: ページイネーブル
	mem.mem[0] = mem.sizeIndex;
	return &mem.mem[1];
}

void HEAP_Release(void* mem){
	HEAP;

	//種別判定
	if((munit)mem & (PAGESIZE - 1)){
		//普通のメモリ
		unsigned* m = mem;
		m--;
		if(*m < 6){
			//ページサイズ未満なので領域をそのままスタックへ
			Push(&(*heap).stack[*m], m);
		}else{
			//ページサイズ以上なのでANCHORを使う
			ANCHOR* a = Pop(&(*heap).anchorStack);
			if(!a){
				//anchorStackは空なので新しく割り当てる
				unsigned i;
				a = (void*)((*heap).top * PAGESIZE);
				(*heap).top++;
				for(i = sizeof(ANCHOR); i < PAGESIZE; i += sizeof(ANCHOR)){
					Push(&(*heap).anchorStack, &a[i]);
				}
			}
			(*a).target = m;
			Push(&(*heap).stack[*m], a);

			//TODO:ページの解放
		}
	}else{
		//TODO:マップの解放
	}
}


