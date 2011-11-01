/************************************************************ PLATFORM SERVERS
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <types.h>
#include <debug.h>
#include <cpu/cpu.h>
#include <core.h>
#include <cpu/virtualPage.h>


/// ARCH(プラットフォーム依存)コードの入り口
extern "C"{
	void Init(void)__attribute__((noreturn));
	extern const void (*__ArchCons[])(void);
	extern const void (*__ArchDest[])(void);
	extern const void (*__KernelConstructor[])(void);
	extern const uchar __kernel_base[];
#if CF_IA32
	extern const u32 __VMA_GDTPT;
#endif
	extern const munit __ulib_LMA[];
	extern const uchar __kProcHeader_LMA[];
#if CF_IA32
#if CF_PAE
	static runit pageDirs[4][512] __attribute__((aligned(4096)));
	static runit hiPageTable[512] __attribute__((aligned(4096)));
	static runit loPageTable[512] __attribute__((aligned(4096)));
	runit __pageRoot[4] __attribute__((aligned(4096)));
#else
	static runit hiPageTable[1024] __attribute__((aligned(4096)));
	static runit loPageTable[1024] __attribute__((aligned(4096)));
	runit __pageRoot[1024] __attribute__((aligned(4096)));
#endif
#endif
#if CF_AMD64
	extern runit __pageRoot[512];
#endif

	void Init(void){
		// 初期ページテーブルを初期化、設定、ページング開始
		const munit kb((munit)__kernel_base);
#if CF_IA32
#if CF_PAE
		//ページディレクトリ構造を構築
		for(uint i(0); i < 4; i++){
			//PDPTとページディレクトリ自身
			__pageRoot[i] =
			pageDirs[3][512 - 4 + i] =
				(runit)pageDirs[i] | 0x201;

			//ページディレクトリはすべてvalidに初期化する
			for(uint j(0); j < 512; j++){
				pageDirs[i][j] |= 0x200ULL;
			}
		}

		//hiPageTable、loPageTableを登録
		pageDirs[0][0] = (runit)loPageTable | 0x207;
		pageDirs[kb >> 30][(kb >> 21) & 0x1ff] =
			(runit)hiPageTable | 0x203;
#else
		//ページディレクトリ構造を構築
		__pageRoot[1023] = (runit)__pageRoot | 0x303;

		//ページディレクトリはすべてvalidに初期化する
		for(uint j(0); j < 512; j++){
			__pageRoot[j] = 0x200ULL;
		}

		//hiPageTable、loPageTableを登録
		__pageRoot[0] = (runit)loPageTable | 0x307;
		__pageRoot[kb >> 22] = (runit)hiPageTable | 0x307;
#endif

		// 実メモリ1MB以下の部分とそのカーネル内イメージを有効化
		for(uint i(0); i < 256; i++){
			loPageTable[i] = 0x00000007 | (i << 12);
			hiPageTable[i] = 0x00000103 | (i << 12);
		}
		// 64KiB以下のカーネル領域はユーザプロセス向けライブラリ
		for(uint i(0); i < 16; i++){
			hiPageTable[i] =
				((runit)__ulib_LMA + 0x00000305) | (i << 12);
		}

		// カーネルプロセス空間の一番下4KiBはカーネルプロセスヘッダ
		loPageTable[0] = (u32)__kProcHeader_LMA | 0x00000005;

		// ページング開始
		asm volatile(
			"mov %%eax, %%cr3;"
			"mov %%cr4, %%eax;"
#if CF_PAE
			"or $0x000000a0, %%eax;"
#else
			"or $0x00000080, %%eax;"
#endif
			"mov %%eax, %%cr4;"
			"mov %%cr0, %%eax;"
			"or $0x80000000, %%eax;"
			"mov %%eax, %%cr0" :: "a"(__pageRoot));
#endif

		/// これ以降はカーネルコードを使用可能

		// GDTを上位メモリのミラーに更新
#if CF_IA32
		asm volatile("lgdt %0;" :: "m"(__VMA_GDTPT)); //内容が同一なのでセグメントは放置
#endif

		// カーネルを初期化
		dputs("Initializing kernel..." INDENT);

		// 初期カーネルページテーブルを記録
#if CF_PAE && !CF_AMD64
		VIRTUALPAGE((munit)pageDirs + kb);
#else
		VIRTUALPAGE((munit)__pageRoot + kb);
#endif

		for(const void (**cons)(void)(__KernelConstructor); *cons; cons++){
			(*cons)();
		}
		dputs(UNINDENT "OK.\n");

		// staticなコンストラクタ呼び出し。ARCH関連はすべてこれで初期化する
		//TODO:モジュールを追加すると死亡するので原因を探る
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


#if CF_DEBUG_LEVEL
void __ARCH_putc(char c){
	asm volatile(
		"mov	$0x03fd, %%dx;"
		"2: inb	%%dx, %%al;"
		"test	$0x20, %%al;"
		"je	2b;"
		"mov	%%cl, %%al;"
		"mov	$0x03f8, %%dx;"
		"outb	%%al, %%dx"
		:: "c"(c) : "ax", "dx");
}
#endif


