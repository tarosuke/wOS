/******************************************************* REALPAGE MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#ifndef _REALPAGE_
#define _REALPAGE_

#include <config.h>
#include <types.h>
#include <heap.h>
#include <lock.h>
#include <debug.h>


class REALPAGE{
public:
	class MEMORYBLOCK{
	public:
		MEMORYBLOCK(){};
		MEMORYBLOCK(runit start, runit size, runit used) : start(start / PAGESIZE), size(size / PAGESIZE), used(used / PAGESIZE), stack((punit*)HEAP::Get((size + PAGESIZE - 1) / PAGESIZE).mem), stackTop(0){
			assert(size);
			assert(stack);
		};
		static void* operator new(munit size, void* place){
			return place;
		}
		inline punit GetPages(punit pages){
			punit page(0);
			KEY key(lock);
			if(stackTop){
				//スタックから取得
				page = stack[--stackTop];
			}else{
				//ページヒープから取得
				if(used + pages < used){
					page = used;
					used += pages;
				}
			}
			return start + page;
		};
		inline bool ReleasePage(punit page){
			if(page < start || start + size <= page){
				//範囲外
				return false;
			}
			KEY key(lock);
			stack[stackTop++] = page;
			return true;
		};
	private:
		punit start;
		punit size;
		punit used;
		punit* stack;
		uint stackTop;
		LOCK lock;
	};
	static void NewMemoryBank(runit start, runit size, runit used){
		if(numOfBanks < CF_MAX_MEMORYBANKs){
			new(&memoryBanks[numOfBanks++]) MEMORYBLOCK(start, size, used + PAGESIZE - 1);
		}
	};
	static punit GetPages(punit pages = 1);
	static void ReleasePage(punit page);
private:
	static uint numOfBanks;
	static MEMORYBLOCK memoryBanks[];
};



#endif
