/**************************************************** VIRTUALPAGE MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, contact us.
 *	$Id$
 */

#ifndef _VIRTUALPAGE_
#define _VIRTUALPAGE_

#include <config.h>
#include <types.h>
#include <map.h>
#include <lock.h>

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
	static const u32 map = 0x400;
	static const u32 copyOnWrite = 0x800;
	static const u32 pageAttributeIndex4M = 0x1000;
	// ページ操作(カーネル領域ならEnableでglobalを立て、でなければuserを立てる)
	static void Enable(void*, munit pages = 1); // 対象ページのvalidを立てる。
	static void Enable(void*, const MAP&, munit pages = 1); // マップとして割り当てる。
	//TODO:実行ファイルのためのEnableを考えておく。基本的にはcowなファイルマップだが。
	static void Enable(void*, munit pa, punit pages = 1); // 実ページを割り当てる。
	static void Disable(void*, munit pages); // 実ページが割り当てられていたら解放し、presentもvalidも0にする。
	// ページフォルトハンドラ
	static void Fault(u32 code);
private:
	static const munit pageTableArrayStarts = 0xffc00000;
	static const munit pageTableArrayStartsPAE = 0xfe000000;
	static const munit kernelStartPage;
	static u32* pageTableArray;
	static u32* const rootPageDir;
	static LOCK lock;
	static bool InKernel(munit pageNum);
	static void PrepareTable(munit startPage, munit pages);
};


#endif

