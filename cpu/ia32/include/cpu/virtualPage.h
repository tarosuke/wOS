/**************************************************** VIRTUALPAGE MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#ifndef _VIRTUALPAGE_
#define _VIRTUALPAGE_

#include <config.h>
#include <types.h>
#include <map.h>
#include <lock.h>
#include <debug.h>


class VIRTUALPAGE{
public:
	// ページ属性
	static const u32 present = 0x01;
	static const u32 readOnly = 0x02;
	static const u32 user = 0x04;
	static const u32 wrightThorugh = 0x08;
	static const u32 disableCache = 0x10;
	static const u32 accessed = 0x20;
	static const u32 dirty = 0x40;
	static const u32 pageAttributeIndex = 0x80;
	static const u32 bigPage = 0x80;
	static const u32 global = 0x100;
	static const u32 valid = 0x200;
	static const u32 maped = 0x400;
	static const u32 copyOnWrite = 0x800;
	static const u32 pageAttributeIndex4M = 0x1000;
	// ページ有効化(あるいは実ページ／マップ割り当て)
	static void Enable(void*, munit pages = 1);
	static void Enable(void*, uint mapID, munit pages, u32 attr = 0);
	static void Enable(void*, runit pa, punit pages);
	// ページ無効化・返却
	static void Disable(void*, munit pages);
	// ページフォルトハンドラ
	static void Fault(const u32 code);
	// カーネルヒープの末端
	static runit* const pageTableArray;
	VIRTUALPAGE();
private:
	static const punit kernelStartPage;
	static LOCK lock;
	static bool InKernel(munit pageNum);
};


#endif

