/************************************************************ PLATFORM SERVERS
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <types.h>
#include <debug.h>
#include <cpu/io.h>
#include <pu.h>
#include <cpu/virtualPage.h>
#include <string.h>
#include <clock.h>


#if CF_DEBUG_LEVEL
extern "C"{
	extern const uint __kernel_base[];
}
//首なし状態のためのコード
static struct CGAVRAM{
	uchar charCode;
	uchar attribute;
}*vram = (CGAVRAM*)((munit)(__kernel_base) + 0xb8000), *cur;
static const uint size = 80 * 25;
static u16 vcar;
static inline uint GetCur(void){
	uint p;
	out8(vcar, 0x0e);
	p = in8(vcar + 1);
	p <<= 8;
	out8(vcar, 0x0f);
	p = in8(vcar + 1);
	return p;
}
static inline void SetCur(void){
	const uint pos = (uint)(cur - vram);
	out8(vcar, 0x0e);
	out8(vcar + 1, (pos >> 8) & 255);
	out8(vcar, 0x0f);
	out8(vcar + 1, pos & 255);
}
static void ArchConsoleInit(){
	vcar = (in8(0x03cc) & 1) ? 0x03d4 : 0x03b4;
	cur = &vram[GetCur()];
}
#endif


/// ARCH(プラットフォーム依存)コードの入り口
extern "C"{
	void Init(void)__attribute__((noreturn));
	extern const void (*__ArchCons[])(void);
	extern const void (*__ArchDest[])(void);
	extern const void (*__KernelConstructor[])(void);
	extern runit __pageRoot_VMA[];
	extern const bool __VESA_Ready;

	void Init(void){
		if(!__VESA_Ready){
			//画面初期化に失敗したか、首なし設定
			ArchConsoleInit();
		}
		// カーネルを初期化
		dputs("Initializing kernel..." INDENT);
		for(const void (**cons)(void)(__KernelConstructor); *cons; cons++){
			(*cons)();
		}
		dputs(UNINDENT "OK.\n");

		// BSP関連を初期化
		new PU;

		// APを起動(もしあれば)
		PU::WakeupAP();

		// 最初のディスパッチ
		PU::Dispatch();

		// ここには来ないはず
		assert(false);
		for(;;){
			asm volatile("hlt");
		}
	};
}

extern void VESA_Check();
///
static class ARCH{
 public:
	ARCH(){
		if(__VESA_Ready){
			VESA_Check();
		}
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
	if(!__VESA_Ready){
		switch(c){
			case '\n' :
				cur += 80;
			case '\r' :
				cur -= (cur - vram) % 80;
				break;
			default :
				if(&vram[size] <= cur){
					///// roll up
					memcpy((void*)vram, (void*)&vram[80], sizeof(CGAVRAM) * (size - 80));
					memset4(&vram[80 * 24], 0x00070007, sizeof(CGAVRAM) * 80);
					cur -= 80;
				}

				(*cur).charCode = c;
				(*cur).attribute = 7;
				cur++;
		}
		SetCur();
	}
}
#endif


