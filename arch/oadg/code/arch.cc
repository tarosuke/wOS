/************************************************************ PLATFORM SERVERS
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <types.h>
#include <debug.h>
#include <cpu/cpu.h>


/// ARCH(プラットフォーム依存)コードの入り口
extern "C"{
	void __Init32(void)__attribute__((noreturn));
	extern const void (*__ArchCons[])(void);
	extern const void (*__ArchDest[])(void);
	extern const uchar __kernel_base[];
	extern const u32 __VMA_GDTPT;
	extern u64 __TSSPH[];
	static u32 kernelPageDir[1024] __attribute__((aligned(4096)));
	static u32 loPageTable[1024] __attribute__((aligned(4096)));
	static u32 kernelPageTable[1024] __attribute__((aligned(4096)));
	void __Init32(void){
		// 初期ページテーブルを初期化、設定、ページング開始
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
		// GDTを上位メモリのミラーに更新
		asm volatile("lgdt %0;" :: "m"(__VMA_GDTPT)); //内容が同一なのでセグメントは放置

		// TSSを設定する
		CPU::SetupTSSDescriptor(__TSSPH);

		// staticなコンストラクタ呼び出し。ARCH関連はすべてこれで初期化する
		for(const void (**cons)(void)(__ArchCons); *cons; cons++){
			(*cons)();
		}
		// 呼ばれるはずはないけれど、デストラクタ呼び出し
		for(const void (**dest)(void)(__ArchDest); *dest; dest++){
			(*dest)();
		}
		//到達マーカー
		assert(true);
		asm volatile("cli; hlt");
		for(;;);
	};
}

extern void VESA_Check();
///
static class ARCH{
 public:
	ARCH(){
		dputs(" get 32bits mode with paging.\n");
		VESA_Check();
	};
}arch;




