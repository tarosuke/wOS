/************************************************************ PLATFORM SERVERS
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <types.h>
#include <debug.h>
#include <cpu/cpu.h>
#include <core.h>


/// ARCH(プラットフォーム依存)コードの入り口
extern "C"{
	void __Init32(void)__attribute__((noreturn));
	extern const void (*__ArchCons[])(void);
	extern const void (*__ArchDest[])(void);
	extern const void (*__KernelConstructor[])(void);
	extern const uchar __kernel_base[];
	extern const u32 __VMA_GDTPT;
	static u32 kernelPageDir[1024] __attribute__((aligned(4096)));
	static u32 loPageTable[1024] __attribute__((aligned(4096)));
	static u32 kernelPageTable[1024] __attribute__((aligned(4096)));
	void __Init32(void){
		// 初期ページテーブルを初期化、設定、ページング開始
		for(uint i(0); i < 256; i++){
			loPageTable[i] = 0x0000007 | (i << 12);
			kernelPageTable[i] = 0x0000103 | (i << 12);
		}
		kernelPageDir[1023] = ((u32)kernelPageDir & 0xfffff000) | 0x0000103;
		kernelPageDir[0] = ((u32)loPageTable & 0xfffff000) | 0x0000007;
		kernelPageDir[((u32)__kernel_base) >> 22] = ((u32)kernelPageTable & 0xfffff000) | 0x0000103;
		asm volatile(
			"mov %%eax, %%cr3;"
			"mov %%cr4, %%eax;"
			"or $0x00000080, %%eax;"
			"mov %%eax, %%cr4;"
			"mov %%cr0, %%eax;"
			"or $0x80000000, %%eax;"
			"mov %%eax, %%cr0" :: "a"(kernelPageDir));

		/// これ以降はカーネルコードを使用可能

		// GDTを上位メモリのミラーに更新
		asm volatile("lgdt %0;" :: "m"(__VMA_GDTPT)); //内容が同一なのでセグメントは放置

		// カーネルを初期化
		dputs("Initializing kernel..." INDENT);
		for(const void (**cons)(void)(__KernelConstructor); *cons; cons++){
			(*cons)();
		}
		dputs(UNINDENT "OK.\n");

		// staticなコンストラクタ呼び出し。ARCH関連はすべてこれで初期化する
		dputs("Initializing arch module..." INDENT);
		for(const void (**cons)(void)(__ArchCons); *cons; cons++){
			(*cons)();
		}
		dputs(UNINDENT "OK.\n");

		// プロセッサを起動
		new(0) CPU(0); // TODO:SMPの時は初期化しないルートからプロセッサ番号を渡す

		//ここには到達しないはず
		assert(false);
		asm volatile("cli; hlt");
		for(;;);
	};
}

extern void VESA_Check();
///
static class ARCH{
 public:
	ARCH(){
		VESA_Check();
	};
}arch;




