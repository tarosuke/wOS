/**************************************************** VIRTUALPAGE MANIPULATION
 *	Copyright (C) 2011- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#ifndef _VIRTUALPAGE_
#define _VIRTUALPAGE_

#include <config.h>
#include <types.h>
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
	static const runit mapped = 0x400;
	static const runit copyOnWrite = 0x800;
	static const runit pageAttributeIndex4M = 0x1000;
#if CF_AMD64
	static const runit noExec = 0x8000000000000000ULL;
#endif
	// ページ有効化(あるいは実ページ／マップ割り当て)
	static inline void Enable(void* start, munit pages = 1){
		KEY key(pageTableArray);
		_Enable(start, pages);
	};
//	static void Enable(void*, uint mapID, munit pages, runit attr = 0);
	static inline void Enable(void* start, runit pa, punit pages){
		KEY key(pageTableArray);
		_Enable(start, pa, pages);
	};

	// ページ無効化・返却
	static inline void Disable(void* start, munit pages){
		KEY key(pageTableArray);
		_Disable(start, pages);
	};

	// ページフォルトハンドラ
	static void Fault(u32 code, EXCEPTION::FRAME&);
	VIRTUALPAGE();			//アイドルタスク用
	VIRTUALPAGE(const VIRTUALPAGE&);	//アイドルタスクからの派生
#if CF_IA32
#if CF_PAE
	static const munit heapTop = 0xff800000;
#else
	static const munit heapTop = 0xffc00000;
#endif
#endif
#if CF_AMD64
	static const munit heapTop = -2ULL;
#endif
protected:
	static void _Enable(void*, munit pages);
	static void _Enable(void*, runit pa, punit pages);
	static void _Disable(void*, munit pages);
	static class PTA : public LOCK{
	public:
		PTA(munit);
	#if CF_IA32
		inline runit& operator[](punit p){
			return array[p];
		};
	#endif
	#if CF_AMD64
		runit& operator[](punit);
	private:
		//ページの変更をプロセッサに教える
		inline void Assign(runit r){
			wcp = (r & ~(PAGESIZE - 1)) | present;
			asm volatile("invlpg (%0)" :: "r"(pw));
		};
		runit* const pw; //窓
		runit& wcp; //窓のアドレスを書く場所
		runit lcr3; // 最後に設定したCR3
		runit lwcp; // 最後に設定したwcp
	#endif
	#if CF_IA32
		runit* const array;
	#endif
	}pageTableArray;
	static const punit kernelStartPage;
	static bool InKernel(munit pageNum);
	static void Assign(runit& pte, munit addr, punit newPage){
		pte = (newPage << 12) | InKernel(addr) ? 0x103 : 7;
		asm volatile(
			"xor %%eax, %%eax;"
			"rep stosl" :: "D"(addr), "c"(PAGESIZE / 4));
	};
	static inline runit GetPageRoot(){
		runit r;
		asm volatile("mov %%cr3, %0" : "=r"(r));
		return r;
	};
	static inline void SetPageRoot(const runit root){
		asm volatile("mov %0, %%cr3" :: "r"(root));
	};
private:
};


class TASKVIRTUALPAGE : public VIRTUALPAGE{
public:
	TASKVIRTUALPAGE();
	TASKVIRTUALPAGE(int) : rootPage(GetPageRoot()){}; //アイドルタスク用
	runit GetPage(munit va, bool autoAssign = true){
		ROOTSWITCHER rs(pageTableArray, rootPage);
		return pageTableArray[va / PAGESIZE];
	};
	void FlushUserSpace();	//ユーザ空間を完全解放
protected:
	inline void DispatchTo(){
		SetPageRoot(rootPage);
	};
private:
	const runit rootPage;
	class ROOTSWITCHER{
	public:
		inline ROOTSWITCHER(PTA& pta, const runit& to) :
			key(pta), origin(GetPageRoot()){
			SetPageRoot(to);
		};
		inline ~ROOTSWITCHER(){
			SetPageRoot(origin);
		};
	private:
		KEY key;
		const runit origin;
	};
};


#endif

