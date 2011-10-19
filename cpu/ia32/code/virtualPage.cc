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


void VIRTUALPAGE::PrepareTable(munit startPage, munit pages){
	for(munit p(startPage >> 10); p <= ((startPage + pages) >> 10); p++){
		u32& pde(rootPageDir[p]);
		if(!(pde & present)){
			//ページテーブル割り当て
			const punit pt(REALPAGE::GetPages(pages));
			assert(pt);
			if((kernelStartPage >> 10) <= p){
				//カーネル領域なので共通ディレクトリにも設定する
				const u32 v((pt * PAGESIZE) | 0x103);
				pde = v;
				__kernelPageDir_VMA[p] = v;
			}else{
				// ユーザ領域の
				pde = (pt * PAGESIZE) | 7;
			}
		}
	}
}


bool VIRTUALPAGE::InKernel(munit pageNum){
	return kernelStartPage <= pageNum;
}


void VIRTUALPAGE::Enable(void* start, munit pages){
	const munit p((munit)start / PAGESIZE);
	KEY key(lock);

	// ページテーブル準備
	PrepareTable(p, pages);

	// ページイネーブル(普通)
	for(munit v(p); v < p + pages; v++){
		u32& pte(pageTableArray[v]);
		if(!(pte & (present || valid))){
			pte = valid | (InKernel(v) ? 0x102 : 6);
		}
	}
}

void VIRTUALPAGE::Enable(void* start, const MAP&, munit pages){
	const munit p((munit)start / PAGESIZE);
	KEY key(lock);

	// ページテーブル準備
	PrepareTable(p, pages);

	// ページイネーブル(普通)
	for(munit v(p); v < p + pages; v++){
		u32& pte(pageTableArray[v]);
		if(!(pte & (present || valid))){
			pte = valid | (InKernel(v) ? 0x102 : 6);
		}
	}
}

void VIRTUALPAGE::Enable(void* start, munit pa, punit pages){
	const munit p((munit)start / PAGESIZE);
	KEY key(lock);

	// ページテーブル準備
	PrepareTable(p, pages);

	// 実メモリ割り当て
	punit rm(REALPAGE::GetPages(pages));
	assert(rm);
	for(munit v(p); v < p + pages; v++, rm++){
		u32& pte(pageTableArray[v]);
		assert(!(pte & (present || valid)));
		pte = (rm * PAGESIZE) | valid | present | (InKernel(v) ? 0x103 : 7);
	}
}

void VIRTUALPAGE::Disable(void*, munit pages){
}



// ページフォルトハンドラ
void VIRTUALPAGE::Fault(u32 code){
}


