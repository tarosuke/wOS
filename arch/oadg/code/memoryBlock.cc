/******************************************************** MEMORYBANK COMPANION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
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
#if !CF_PAE && !CF_AMD64
		const u64 _4G(1ULL << 32);
		bool needPAE(false);
#endif
		for(uint i(0); i < CF_MAX_MEMORYBANKs && (*b).type != 0xffffffff; i++, b++){
			hprintf("start:%16llx size:%16llx type:%x.\n", (*b).start, (*b).size, (*b).type);
			if((*b).type == 1){
				const runit kbase((runit)__kernel_heap - (runit)__kernel_base);
#if !CF_PAE && !CF_AMD64
				if(_4G <= (*b).start || _4G < (*b).start + (*b).size){
					needPAE = true;
					continue;
				}
#endif
				totalSize += (*b).size;
				REALPAGE::NewMemoryBank((*b).start, (*b).size, (*b).start < kbase ? kbase : 0);
			}
		}
#if !CF_PAE && !CF_AMD64
		if(needPAE){
			dputs(LIGHTYELLOW"NOTE: PAE not available but there're over-4GiB memory.\nNOTE: Try PAE kernel if you want use their memory.\n"ORIGIN);
		}
#endif
		dprintf(UNINDENT "OK(%m available).\n", totalSize);
	};
}memorybank;


