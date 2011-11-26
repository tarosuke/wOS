/**************************************************** VIRTUALPAGE MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#include <cpu/virtualPage.h>
#include <realPage.h>
#include <cpu/task.h>
#include <heap.h>


extern "C"{
	extern uchar __kernel_base[];
	extern runit __pageRoot[];
#if CF_AMD64
	extern runit __hiPageTable_VMA[]; //カーネルが収まってるページのテーブル
#endif
}

// ページテーブルが丸見えという便利な配列。
#if CF_IA32
VIRTUALPAGE::PTA VIRTUALPAGE::pageTableArray(heapTop);
#endif
#if CF_AMD64
VIRTUALPAGE::PTA VIRTUALPAGE::pageTableArray(
	(runit*)HEAP::GetByIndex(HEAP::GetBlockIndex(PAGESIZE)));

VIRTUALPAGE::PTA::PTA(runit* pw) :
	pw(pw),
	wcp(__hiPageTable_VMA[((munit)pw / PAGESIZE) & 511]){};

runit& VIRTUALPAGE::PTA::operator[](punit pageNum){
	//rootを取得
	Assign(GetCR3());

	for(uint i(0); i < 3; i++, pageNum <<= 9){
		runit& te(pw[(pageNum >> 27) & 511]);
		if(!(te & present)){
			//エントリの割り当て
			te = REALPAGE::GetPages(1)*PAGESIZE | present;
		}
		Assign(te);
	}
	return pw[(pageNum >> 27) & 511];
}
#endif

// カーネル領域とユーザ領域を分ける値
const punit VIRTUALPAGE::kernelStartPage((munit)__kernel_base / PAGESIZE);

// ページテーブルのためのロック(他のプロセッサがページを埋めるのを阻止するため)
LOCK VIRTUALPAGE::lock;

static VIRTUALPAGE vpage;


VIRTUALPAGE::VIRTUALPAGE(){
	dputs("virtual pages..." INDENT);
#if CF_IA32
	dprintf("pageTableArray: %p.\n", heapTop);
#endif
	dprintf("kernelPageDir: %p.\n", __pageRoot);
	dputs(UNINDENT "OK.\n");
}

bool VIRTUALPAGE::InKernel(munit pageNum){
	return kernelStartPage <= pageNum;
}


void VIRTUALPAGE::Enable(void* start, munit pages){
	const punit p((munit)start / PAGESIZE);
	KEY key(lock);

	// ページイネーブル(普通)
	for(munit v(p); v < p + pages; v++){
		runit& pte(pageTableArray[v]);
		if(!(pte & (present | valid))){
			pte = valid | (InKernel(v) ? 0x102 : 6);
		}
	}
}

void VIRTUALPAGE::Enable(void* start, uint mapID, munit pages, runit attr){
	const munit p((munit)start / PAGESIZE);
	mapID <<= 12;
	KEY key(lock);

	// ページイネーブル(マップ)
	for(munit v(p); v < p + pages; v++){
		runit& pte(pageTableArray[v]);
		if(!(pte & (present || valid))){
			pte = mapID | maped | valid | attr |
				(InKernel(v) ? 0x102 : 6);
		}
	}
}

void VIRTUALPAGE::Enable(void* start, runit pa, punit pages){
	const munit p((munit)start / PAGESIZE);
	KEY key(lock);

	// 実メモリ割り当て
	runit rm(REALPAGE::GetPages(pages));
	assert(rm);
	for(munit v(p); v < p + pages; v++, rm++){
		runit& pte(pageTableArray[v]);
		assert(!(pte & (present || valid)));
		pte = (rm * PAGESIZE) | valid | present |
			(InKernel(v) ? 0x103 : 7);
	}
}

void VIRTUALPAGE::Disable(void* start, munit pages){
	const munit p((munit)start / PAGESIZE);
	KEY key(lock);

	// ページ無効化(割り当てられていれば返却)
	for(munit v(p); v < p + pages; v++){
		runit& pte(pageTableArray[v]);
		if(pte & present){
			REALPAGE::ReleasePage(pte >> 12);
		}
		pte = 0;
	}
}



// ページフォルトハンドラ
void VIRTUALPAGE::Fault(u32 code, EXCEPTION::FRAME&){
	///// get targetaddress
	munit addr;
	asm volatile("mov %%cr2, %0" : "=r"(addr));

	// マルチプロセサ時に処理中に値が変わると困るのでロック
	KEY key(lock);

	///// ページエントリ取得
	const punit page(addr / PAGESIZE);
	runit& pte(pageTableArray[page]);

	// 違反など検出
	if(code & 1){
		// ページ保護違反

		///// check kernel/user
		if(code & ~pte & user){
			//TODO:KILL IT
			dprintf("Page protection violated at %p(%08x):%r.", addr, code, pte);
			Panic("");
		}

		///// check writable & not CoW
		if(code & pte & readOnly){
			if(~pte & copyOnWrite){
				//TODO:KILL IT
				dprintf("Page isn't writable at %p(%08x):%r.", addr, code, pte);
				Panic("");
			}else{
				//TODO:CoWな処理(ページを取得して割り当てて複製して戻る)
				dprintf("Execuse me. CoW isn't available at %p(%08x):%r.", addr, code, pte);
				Panic("");
			}
		}
		Panic("Undefined page fault.");
	}

	///// check enabled
	if(~pte & valid){
		//TODO:KILL IT if (code & user). Panic if it's in kernel
		dprintf("Touched invalid page at %p(%08x):%r.", addr, code, pte);
		Panic("");
	}

	if(pte & maped){
		//TODO:マップによる割り当て
		Panic("page mapping isn't available.");
	}else{
		if((munit)&pageTableArray[kernelStartPage] <= addr){
			//カーネル領域のページテーブル要求なのでmasterを参照
			runit here(CPUTASK::GetPageRoot());
			CPUTASK::SetPageRoot((runit)__pageRoot);
			const runit thePage(pageTableArray[addr]);
			CPUTASK::SetPageRoot(here);
			pte = thePage;
			return;
		}

		// 普通のページ割り当て
		const punit newPage(REALPAGE::GetPages());
		assert(newPage);

		// ページを割り当ててページをクリア
		pte = (newPage << 12) | InKernel(addr) ? 0x103 : 7;
		asm volatile(
			"xor %%eax, %%eax;"
			"rep stosl" :: "D"(addr), "c"(PAGESIZE / 4));
	}
}
