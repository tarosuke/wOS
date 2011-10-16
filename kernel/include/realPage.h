/******************************************************* REALPAGE MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#ifndef _REALPAGE_
#define _REALPAGE_

#include <config.h>
#include <types.h>
#include <debug.h>


class REALPAGE{
public:
	class MEMORYBLOCK{
	private:
		punit start;
		punit size;
		punit used;
	public:
		MEMORYBLOCK(){};
		MEMORYBLOCK(punit start, punit size) : start(start / PAGESIZE), size(size / PAGESIZE), used(0){};
		static void* operator new(munit size, void* place){
			return place;
		}
		inline punit GetPages(punit pages){
			punit page(0);
			//TODO:ページプールから取得(そのためには仮想メモリのサポートが要る)
			//ページヒープから取得
			if(used + pages < used){
				page = used;
				used += pages;
			}
			return start + page;
		};
	};
	static void NewMemoryBank(punit start, punit size){
		if(numOfBanks < CF_MAX_MEMORYBANKs){
			new(&memoryBanks[numOfBanks++]) MEMORYBLOCK(start, size);
		}
	};
	punit GetPages(punit pages = 1);
private:
	static uint numOfBanks;
	static MEMORYBLOCK memoryBanks[];
};



#endif
