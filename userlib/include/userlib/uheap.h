/************************************************************************ HEAP
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _ULIB_HEAP_
#define _ULIB_HEAP_

//TODO:ファイル名にheapを使うとカーネル本体のHEAPと混同するので分離してビルドする方法を考える
typedef struct{
	unsigned top; //ヒープトップ。本来はmunit。taskHead.heatStart + 1をPAGESIZEにラウンドアップした値に初期化される。
	unsigned limit; //ヒープの上限。このすぐ上がスタックで、更に上がユーザライブラリ
	union LFP{ //ロックフリープロトコル用ポインタ。ABA'問題を避けるためにトランザクション番号と共に扱う
		struct{
			void* mem;
			unsigned transaction;
		};
		unsigned long long whole;
	}anchorStack; //解放後領域をスタックに維持するためだけのvoid*サイズのメモリスタック(この領域は解放されない)
	union LFP stack[32]; //解放された領域のサイズごとスタック。PAGESIZE以上の領域のページは解放され"anchor"が代わりに入る。
}UHEAP;

void HEAP_Init(void);
void* HEAP_Get(unsigned size);


#endif
