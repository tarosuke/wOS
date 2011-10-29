/**************************************************** VIRTUALPAGE MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#include <cpu/virtualPage.h>
#include <realPage.h>


extern "C"{
	extern uchar __kernel_base[];
	extern runit __pageRoot_VMA[];
}

// ページテーブルが丸見えという便利な配列。
#if CF_PAE
runit* const VIRTUALPAGE::pageTableArray((runit*)0xff800000);
#else
runit* const VIRTUALPAGE::pageTableArray((runit*)0xffc00000);
#endif

// カーネル領域とユーザ領域を分ける値
const punit VIRTUALPAGE::kernelStartPage((munit)__kernel_base / PAGESIZE);

// ページテーブルのためのロック(他のプロセッサがページを埋めるのを阻止するため)
LOCK VIRTUALPAGE::lock;

// マスターページディレクトリ
runit* const VIRTUALPAGE::masterPageDir(0);


VIRTUALPAGE::VIRTUALPAGE(munit mpd){
	dputs("virtual pages..." INDENT);
	dprintf("pageTableArray: %p.\n", pageTableArray);
	dprintf("kernelPageDir: %p.\n", mpd);
	*const_cast<runit**>(&masterPageDir) = (runit*)mpd;
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
void VIRTUALPAGE::Fault(const u32 code){
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
			//カーネル領域のページテーブルの要求なのでmasterを参照
			runit& mpe(masterPageDir[page]);
			if(!(mpe & present)){
				//masterにもないので確保して設定
				const punit newPage(REALPAGE::GetPages());
				assert(newPage);
				pte = mpe = (newPage << 12) | 0x303;
			}
			pte = mpe;
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


