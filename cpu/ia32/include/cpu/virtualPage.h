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
#include <cpu/exception.h>


class VIRTUALPAGE{
public:
	// ページ属性
	static const runit present = 0x01;
	static const runit readOnly = 0x02;
	static const runit user = 0x04;
	static const runit wrightThorugh = 0x08;
	static const runit disableCache = 0x10;
	static const runit accessed = 0x20;
	static const runit dirty = 0x40;
	static const runit pageAttributeIndex = 0x80;
	static const runit bigPage = 0x80;
	static const runit global = 0x100;
	static const runit valid = 0x200;
	static const runit maped = 0x400;
	static const runit copyOnWrite = 0x800;
	static const runit pageAttributeIndex4M = 0x1000;
#if CF_AMD64
	static const runit noExec = 0x8000000000000000ULL;
#endif
	// ページ有効化(あるいは実ページ／マップ割り当て)
	static void Enable(void*, munit pages = 1);
	static void Enable(void*, uint mapID, munit pages, runit attr = 0);
	static void Enable(void*, runit pa, punit pages);
	// ページ無効化・返却
	static void Disable(void*, munit pages);
	// ページフォルトハンドラ
	static void Fault(u32 code, EXCEPTION::FRAME&);
	// カーネルヒープの末端
	static runit* const pageTableArray;
	VIRTUALPAGE();
private:
	static const punit kernelStartPage;
	static LOCK lock;
	static bool InKernel(munit pageNum);
};


#endif

