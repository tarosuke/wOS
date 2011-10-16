/******************************************************** MEMORYBANK COMPANION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <config.h>
#include <types.h>
#include <realPage.h>
#include <debug.h>


extern "C"{
	extern struct MB{
		u64 start;
		u64 size;
		u32 type;
	}__attribute__((packed)) __ARCH_MemoryBlocks[CF_MAX_MEMORYBANKs];
};


static class MEMORYBANK{
public:
	MEMORYBANK(){
		dputs("memorybank..." INDENT);
		munit totalSize(0);
		MB* b(__ARCH_MemoryBlocks);
		for(uint i(0); i < CF_MAX_MEMORYBANKs && (*b).type != ~0UL; i++, b++){
			hprintf("start:%16llx size:%16llx type:%lu.\n", (*b).start, (*b).size, (*b).type);
			if((*b).type == 1){
				totalSize += (*b).size;
				REALPAGE::NewMemoryBank((*b).start, (*b).size);
			}
		}
		dprintf(UNINDENT "OK.%dMiB avaliable.\n", totalSize / (1024 * 1024));
	};
}memorybank;


