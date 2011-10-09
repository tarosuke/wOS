/************************************************************ PLATFORM SERVERS
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <types.h>

/// ARCH(プラットフォーム依存)コードの入り口
extern "C"{
	void __Init32(void)__attribute__((noreturn));
	extern const void (*__ArchCons[])(void);
	extern const void (*__ArchDest[])(void);
	extern const void* __kernel_base;
	static u32 kernelPageDir[1024] __attribute__((aligned(4096)));
	static u32 loPageTable[1024] __attribute__((aligned(4096)));
	static u32 kernelPageTable[1024] __attribute__((aligned(4096)));
	void __Init32(void){
		//  初期ページテーブルを初期化、設定、ページング開始
		for(uint i(0); i < 256; i++){
			loPageTable[i] = 0x0000003 | (i << 12);
			kernelPageTable[i] = 0x0000103 | (i << 12);
		}
		kernelPageDir[1023] = ((u32)kernelPageDir & 0xfffff000) | 0x0000103;
		kernelPageDir[0] = ((u32)loPageTable & 0xfffff000) | 0x0000003;
		kernelPageDir[((u32)__kernel_base) >> 22] = ((u32)kernelPageTable & 0xfffff000) | 0x0000103;
		asm volatile(
			"mov %%eax, %%cr3;"
			"mov %%cr4, %%eax;"
			"or $0x00000080, %%eax;"
			"mov %%eax, %%cr4;"
			"mov %%cr0, %%eax;"
			"or $0x80000000, %%eax;"
			"mov %%eax, %%cr0" :: "a"(kernelPageDir));

		// staticなコンストラクタ呼び出し。ARCH関連はすべてこれで初期化する
		for(const void (**cons)(void)(__ArchCons); *cons; cons++){
			(*cons)();
		}
		// 呼ばれるはずはないけれど、デストラクタ呼び出し
		for(const void (**dest)(void)(__ArchDest); *dest; dest++){
			(*dest)();
		}
		asm volatile("cli; hlt");
		for(;;);
	};
}


///
static class ARCH{
 public:
	ARCH(){
		asm volatile(
			"mov $0x3f8, %%dx;"
			"mov $'*', %%al;"
			"outb %%al, %%dx;"
			"hlt" ::: "al", "dx");
	};
}arch;




