/**************************************************** VIRTUALPAGE MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, contact us.
 *	$Id$
 */

#include <cpu/virtualPage.h>
#include <realPage.h>


//NOTE:ページディレクトリ、ページテーブルの操作は排他で、排他領域中でまず問題が解消されていないことを再確認すること。これは他のプロセサによる問題解決を認識するために必要である。なお、ロックフリープロトコルは無駄にリソースを確保／解放する必要が発生するためカーネルでは使わず、必ず排他処理で実装すること。

extern "C"{
	extern uchar __kernel_base[];
	extern u32 __kernelPageDir_VMA[];
}

// ページテーブルが丸見えという便利な配列。ただし、メモリが割り当てられていないことがあるのでrootPageDirで確認
u32* VIRTUALPAGE::pageTableArray((u32*)pageTableArrayStarts);
u32* const VIRTUALPAGE::rootPageDir((u32*)0xfffff000);
const munit VIRTUALPAGE::kernelStartPage((munit)__kernel_base / PAGESIZE);

// ページテーブルのためのロック(他のプロセッサがページを埋めたのを認識するため)
LOCK VIRTUALPAGE::lock;



bool VIRTUALPAGE::InKernel(munit pageNum){
	return kernelStartPage <= pageNum;
}


void VIRTUALPAGE::Enable(void* start, munit pages){
	const munit p((munit)start / PAGESIZE);
	KEY key(lock);

	// ページイネーブル(普通)
	for(munit v(p); v < p + pages; v++){
		u32& pte(pageTableArray[v]);
		if(!(pte & (present || valid))){
			pte = valid | (InKernel(v) ? 0x102 : 6);
		}
	}
}

void VIRTUALPAGE::Enable(void* start, uint mapID, munit pages, u32 attr){
	const munit p((munit)start / PAGESIZE);
	mapID <<= 12;
	KEY key(lock);

	// ページイネーブル(マップ)
	for(munit v(p); v < p + pages; v++){
		u32& pte(pageTableArray[v]);
		if(!(pte & (present || valid))){
			pte = mapID | maped | valid | attr | (InKernel(v) ? 0x102 : 6);
		}
	}
}

void VIRTUALPAGE::Enable(void* start, munit pa, punit pages){
	const munit p((munit)start / PAGESIZE);
	KEY key(lock);

	// 実メモリ割り当て
	punit rm(REALPAGE::GetPages(pages));
	assert(rm);
	for(munit v(p); v < p + pages; v++, rm++){
		u32& pte(pageTableArray[v]);
		assert(!(pte & (present || valid)));
		pte = (rm * PAGESIZE) | valid | present | (InKernel(v) ? 0x103 : 7);
	}
}

void VIRTUALPAGE::Disable(void* start, munit pages){
	const munit p((munit)start / PAGESIZE);
	KEY key(lock);

	// ページ無効化(割り当てられていれば返却)
	for(munit v(p); v < p + pages; v++){
		u32& pte(pageTableArray[v]);
		if(pte & present){
			REALPAGE::ReleasePages(v);
		}
		pte = 0;
	}
}



// ページフォルトハンドラ
void VIRTUALPAGE::Fault(const u32 code){
	///// get targetaddress
	munit addr;
	asm volatile("mov %%cr2, %0" : "=g"(addr));

	// マルチプロセサ時に処理中に値が変わると困るのでロック
	KEY key(lock);

	///// get the pageentry
	u32& pte(pageTableArray[addr >> 12]);

	if(code & 1){
		// ページ保護違反

		///// check kernel/user
		if(code & ~pte & user){
			//TODO:KILL IT
			dprintf("Page protection violated at %08x(%08x):%08x.", addr, code, pte);
			Panic("");
		}

		///// check writable & not CoW
		if(code & pte & readOnly){
			if(~pte & copyOnWrite){
				//TODO:KILL IT
				dprintf("Page isn't writable at %08x(%08x):%08x.", addr, code, pte);
				Panic("");
			}else{
				//TODO:CoWな処理(ページを取得して割り当てて複製して戻る)
				dprintf("Execuse me. CoW isn't avaliable at %08x(%08x):%08x.", addr, code, pte);
				Panic("");
			}
		}
		Panic("Undefined page fault.");
	}

	///// check enabled
	if(~pte & valid){
		//TODO:KILL IT if (code & user). Panic if it's in kernel
		dprintf("Touched invalid page at %08x(%08x):%08x.", addr, code, pte);
		Panic("");
	}

	if(pte & maped){
		//TODO:マップによる割り当て
		Panic("page mapping isn't avaliable.");
	}else{
		//普通のページ割り当て
		const punit newPage(REALPAGE::GetPages());
		if(!newPage){
			//TODO:ページアウト待ち
			Panic("Out of memory.");
		}

		// ページを割り当ててページをクリア
		pte = (newPage << 12) | InKernel(addr) ? 0x103 : 7;
		asm volatile("xor %%eax, %%eax; rep stosl" :: "D"(addr), "c"(PAGESIZE / 4));
	}
}


