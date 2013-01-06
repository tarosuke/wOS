/**************************************************** VIRTUALPAGE MANIPULATION
 *	Copyright (C) 2011- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#include <cpu/virtualPage.h>
#include <realPage.h>
#include <cpu/task.h>
#include <heap.h>
#include <pu.h>


extern "C"{
	extern uchar __kernel_base[];
	extern runit __pageRoot[];
#if CF_AMD64
	extern runit __hiPageTable_VMA[]; //カーネルが収まってるページのテーブル
#endif
}

// ページテーブルが丸見えという便利な配列。
VIRTUALPAGE::PTA VIRTUALPAGE::pageTableArray(heapTop);


VIRTUALPAGE::PTA::PTA(munit a) :
#if CF_IA32
	array((runit*)a)
#endif
#if CF_AMD64
	pw((runit*)HEAP::GetByIndex(HEAP::GetBlockIndex(PAGESIZE))),
	wcp(__hiPageTable_VMA[((munit)pw / PAGESIZE) & 511]), lcr3(0), lwcp(0)
#endif
	{
#if CF_AMD64
	dputs("pageTableArray..."INDENT);
	dprintf("pw:%p.\n", pw);
	dprintf("wcp:%p.\n", &wcp);
	dputs(UNINDENT "OK.\n");
#endif
	dputs("virtual pages..." INDENT);
#if CF_IA32
	dprintf("pageTableArray: %p.\n", heapTop);
#endif
	dprintf("kernelPageDir: %p.\n", __pageRoot);
	dputs(UNINDENT "OK.\n");
}

#if CF_AMD64
runit& VIRTUALPAGE::PTA::operator[](punit pageNum){
	const runit page(pageNum);

	//rootを取得
	const runit cr3(GetPageRoot());
	if(cr3 != lcr3 || ((lwcp ^ page) & ~511ULL)){
		Assign(cr3);

		for(uint i(0); i < 3; i++, pageNum <<= 9){
			const uint en((pageNum >> 27) & 511);
			runit e(pw[en]);
			if(!(e & present)){
				//エントリの割り当て
				e = REALPAGE::GetPages(1) * PAGESIZE | present;
				pw[en] = e;
			}
			hprintf("te:%r %r %p %u.\n", e, pw[en], wcp, en);
			Assign(e);
		}
		lcr3 = cr3;
		lwcp = page;
	}
	return pw[page & 511];
}
#endif

// カーネル領域とユーザ領域を分ける値
const punit VIRTUALPAGE::kernelStartPage((munit)__kernel_base / PAGESIZE);

// システムページテーブル
static VIRTUALPAGE vpage __attribute__((init_priority(6000)));


VIRTUALPAGE::VIRTUALPAGE(){
#if CF_AMD64 && 6 <= CF_DEBUG_LEVEL
	dputs("checking pageTableArray..."INDENT);
	for(uint i(0); i < 513; i++){
		const runit ra(i * PAGESIZE);
		dprintf("%05x:%r.\n", ra,
			pageTableArray[((runit)__kernel_base + ra) / PAGESIZE]);
	}
	dputs(UNINDENT "OK.\n");
#endif
}

bool VIRTUALPAGE::InKernel(munit pageNum){
	return kernelStartPage <= pageNum;
}


void VIRTUALPAGE::_Enable(void* start, munit pages){
	const punit p((munit)start / PAGESIZE);

	// ページイネーブル(普通)
	for(munit v(p); v < p + pages; v++){
		runit& pte(pageTableArray[v]);
		if(!(pte & (present | valid))){
			pte = valid | (InKernel(v) ? 0x102 : 6);
		}
	}
}

// void VIRTUALPAGE::Enable(void* start, uint mapID, munit pages, runit attr){
// 	const munit p((munit)start / PAGESIZE);
// 	mapID <<= 12;
// 	KEY key(lock);
//
// 	// ページイネーブル(マップ)
// 	for(munit v(p); v < p + pages; v++){
// 		runit& pte(pageTableArray[v]);
// 		if(!(pte & (present || valid))){
// 			pte = mapID | maped | valid | attr |
// 				(InKernel(v) ? 0x102 : 6);
// 		}
// 	}
// }

void VIRTUALPAGE::_Enable(void* start, runit pa, punit pages){
	const munit p((munit)start / PAGESIZE);
	const runit pageMask(~(runit)(PAGESIZE - 1));

	// 実メモリ割り当て
	for(munit v(p); v < p + pages; v++, pa += PAGESIZE){
		runit& pte(pageTableArray[v]);
//TODO:初期ページ割り当てを__kernel_heapの前までにしとく。残りはちゃんと実ページに登録されてる
//		assert(!(pte & (present || valid)));
		pte = (pa & pageMask) | (InKernel(v) ? 0x103 : 7);
	}
}

void VIRTUALPAGE::_Disable(void* start, munit pages){
	const munit p((munit)start / PAGESIZE);

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
	KEY key(pageTableArray);

	///// ページエントリ取得
	const punit page(addr / PAGESIZE);
	runit& pte(pageTableArray[page]);

	// 違反など検出
	if(code & 1){
		// ページ保護違反

		///// check kernel/user
		if(code & ~pte & user){
			dprintf("Page protection violated at %p(%08x):%r.", addr, code, pte);
			Panic("");
		}

		///// check writable & not CoW
		if(code & pte & readOnly){
			if(~pte & copyOnWrite){
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
		//TODO:REBOOT ME if (code & user). Panic if it's in kernel
		dprintf("Touched invalid page at %p(%08x):%r.", addr, code, pte);
		Panic("");
	}

	if(pte & mapped){
		//マップから実ページを取得
		MAP* const map(PU::GetCurrentTask().GetMap(pte >> 12));
		if(!map){
			//TODO:本来は異常動作としてプロセス再起動
			Panic("No map found.");
		}

		const runit newPage((*map).GetPage(page));
		if(!newPage){
			Panic("Out of map pages.");
		}

		// ページ割り当て
		Assign(pte, addr, newPage);
	}else{
		if((munit)&pageTableArray[kernelStartPage] <= addr){
			//カーネル領域のページテーブル要求なのでmasterを参照
			runit here(GetPageRoot());
			SetPageRoot((runit)__pageRoot);
			const runit thePage(pageTableArray[addr]);
			SetPageRoot(here);
			pte = thePage;
			return;
		}

		// 普通のページ割り当て
		const punit newPage(REALPAGE::GetPages());
		if(!newPage){
			Panic("Out of memory.");
		}

		// ページ割り当て
		Assign(pte, addr, newPage);
	}
}


TASKVIRTUALPAGE::TASKVIRTUALPAGE() : rootPage(REALPAGE::GetPages()){}
