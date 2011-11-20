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
			KEY key(lock);
			*(void**)mem = top;
			top = mem;
		}
		inline void* Pop(){
			KEY key(lock);
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
void* operator new(munit);
void operator delete(void*);


//HEAPではないけどplacement newのための領域確保
template<class T, uint elements = 1> class PLACER{
public:
	void* Place(uint index = 0){ return (void*)thePlace[index]; };
private:
	char thePlace[elements][sizeof(T)];
};

#endif
