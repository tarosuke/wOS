/******************************************************** MEMORYBANK COMPANION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id: 6cc7799dacd4f238fe8f5b256350f88e32dfeb68 $
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
	extern uint __kernel_heap[];
	extern uint __kernel_base[];
};


static class MEMORYBANK{
public:
	MEMORYBANK(){
		dputs("memorybank..." INDENT);
		runit totalSize(0);
		MB* b(__ARCH_MemoryBlocks);
#if !CF_PAE
		const runit _4G(1ULL << 32);
#endif
		for(uint i(0); i < CF_MAX_MEMORYBANKs && (*b).type != ~0UL; i++, b++){
			hprintf("start:%16llx size:%16llx type:%lu.\n", (*b).start, (*b).size, (*b).type);
			if((*b).type == 1){
				const runit kbase((runit)__kernel_heap - (runit)__kernel_base);
#if !CF_PAE
				if(_4G <= (*b).start || _4G < (*b).start + (*b).size){
					dputs(LIGHTYELLOW"WARNING: PAE not available.\n"ORIGIN);
					continue;
				}
#endif
				totalSize += (*b).size;
				REALPAGE::NewMemoryBank((*b).start, (*b).size, (*b).start < kbase ? kbase : 0);
			}
		}
		dprintf(UNINDENT "OK(%lld.%02lld[MiB] available).\n", totalSize >> 20, ((totalSize & 0xfffff) * 100) >> 20);
	};
}memorybank;


