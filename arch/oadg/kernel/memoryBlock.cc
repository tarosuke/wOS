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
	extern uint __kernel_heap[];
	extern uint __kernel_base[];
};


static class MEMORYBANK{
public:
	MEMORYBANK(){
		dputs("memorybank..." INDENT);
		u64 totalSize(0);
		MB* b(__ARCH_MemoryBlocks);
		const u64 _4G(1ULL << 32);
		for(uint i(0); i < CF_MAX_MEMORYBANKs && (*b).type != ~0UL; i++, b++){
			hprintf("start:%16llx size:%16llx type:%lu.\n", (*b).start, (*b).size, (*b).type);
			if((*b).type == 1){
				u64 start((*b).start);
				u64 size((*b).size);
				const u64 kbase((u64)__kernel_heap - (u64)__kernel_base);
				if(start < kbase){
					//カーネルが置いてある場所は除外
					const punit diff(kbase - start);
					if(size < diff){
						//小さすぎるのでそのブロックは無視
						continue;
					}
					start += diff;
					size -= diff;
				}
				if(_4G <= (*b).start){
					dputs(LIGHTYELLOW"WARNING: PAE not available.\n"ORIGIN);
					continue;
				}else if(_4G < (*b).start + size){
					dputs(LIGHTYELLOW"WARNING: PAE not available.\n"ORIGIN);
					size = _4G - (*b).start;
				}
				totalSize += size;
				REALPAGE::NewMemoryBank((*b).start, size);
			}
		}
		dprintf(UNINDENT "OK(%lld.%02lld[MiB] available).\n", totalSize >> 20, ((totalSize & 0xfffff) * 100) >> 20);
	};
}memorybank;


