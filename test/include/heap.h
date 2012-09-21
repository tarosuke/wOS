/******************************************************** DRYTEST STUB for HEAP
 *	Copyright (C) 2012- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _HEAP_
#define _HEAP_

#include <stdlib.h>


class HEAP{
public:
	struct BLOCK{
		void* mem;
		uint index;
	};
	static uint GetBlockIndex(munit size){ return 0; };
	static void* GetByIndex(uint){ return 0; };
	static BLOCK Get(munit s){
		BLOCK b;
		b.index = 0;
		b.mem = malloc(s);
		return b;
	};
	static void* Assign(runit start, munit size){
		return 0;
	};
	static void Release(BLOCK&){};
	static void Release(void* mem, uint index){
		BLOCK b = { mem, index };
		Release(b);
	;}
};


#endif
