


#include <core.h>
#include <cpu/cpu.h>
#include <config.h>
#include <debug.h>


uchar CORE::kernelStacks[CF_MAX_PROCESSORs][4096]__attribute__((aligned(4096)));


void CORE::Entry(uint cpuid){
assert(true); //個々は通る
	CPU::SetStack((void*)kernelStacks[cpuid][4096]); //これがinlineになってない臭い
assert(true); //が、個々は通らない。なので↑
	CPU cpu(cpuid); //プロセッサごとの初期化

	assert(false);
	for(;;);
}




