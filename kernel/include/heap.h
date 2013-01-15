/***************************************************************** KERNEL HEAP
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _HEAP_
#define _HEAP_

#include <config.h>
#include <types.h>
#include <lock.h>
#include <cpu/virtualPage.h>
#include <debug.h>

/** HEAP：カーネルヒープの管理
 * Get〜で取得、Releaseで返却する。
 * 取得メモリサイズを記録しておく必要があるが、HEAP側では管理しない。
 * 高速化のためサイズの代わりにブロックインデックスを使える。
 * 標準のnew/deleteはサイズを記録してくれるが先頭数byteを消費する。
 * なのでページ境界が必要なときはnew/deleteは使えない。
 * なお、Assignは実メモリをヒープに割り当てるメソッド。
 * 実メモリ管理が混乱するのでこれで取得した領域は返却してはならない。
 * ユーザ領域で実メモリをマップするときはユーザヒープを操作すること。
 *
 * 基本的に各クラスにnew/deleteを用意してメモリ管理するために存在している。
 * static constにブロックインデクスを用意し、GetBlockIndexで初期化しておく。
 * そうすれば取得時も開放時もブロックインデクスで速やかに確保/開放できる。
 * 面倒ならnew/deleteを作らなければいいが、前述の通りページ境界は無視される。
 */

class HEAP{
public:
	HEAP(munit start, munit limit);
	struct BLOCK{
		void* mem;
		uint index;
	};
	static uint GetBlockIndex(munit size);
	static void* GetByIndex(uint);
	static BLOCK Get(munit);
	static void* Assign(runit start, munit size); //NOTE:返却しないこと！
	static void Release(BLOCK&);
	static void Release(void* mem, uint index){
		BLOCK b = { mem, index };
		Release(b);
	}
private:
	class STACK{
	public:
		STACK() : top(0){};
		inline void Push(void* mem){
			KEY<LOCK> key(lock);
			*(void**)mem = top;
			top = mem;
		}
		inline void* Pop(){
			KEY<LOCK> key(lock);
			void* r(top);
			if(top){ top = *(void**)top; }
			return r;
		};
	private:
		void* top;
		LOCK lock;
	};
	static STACK memoryPools[];
	static munit top;
	static munit limit;
	static const munit memoryPoolSizes[];
	static LOCK lock;
};


// 簡単なヒープ操作(要求サイズがページサイズの倍数だと無駄が出るので注意)
// その場合は専用のクラスを用意してそのクラスで専用new/deleteを用意すること
void* operator new(munit);
void operator delete(void*);


//HEAPではないけどplacement newのための領域確保
template<class T, uint elements = 1, typename E = u32> class PLACER{
public:
	T& operator[](uint index){
		return *(T*)&thePlace[index][0];
	};
	uint GetNumOf(T* t){ return t - (T*)thePlace; };
private:
	E thePlace[elements][(sizeof(T) + sizeof(E) - 1) / sizeof(E)];
};

#endif
