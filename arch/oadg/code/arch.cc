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
	extern runit __pageRoot[512];

	void Init(void){
		// カーネルを初期化
		dputs("Initializing kernel..." INDENT);
#if 0
// 		// 初期カーネルページテーブルを記録
// #if CF_PAE && !CF_AMD64
// 		VIRTUALPAGE((munit)pageDirs + kb);
// #else
// 		VIRTUALPAGE((munit)__pageRoot + kb);
// #endif

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
#endif
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


