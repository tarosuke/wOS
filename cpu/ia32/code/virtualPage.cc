/**************************************************** VIRTUALPAGE MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <cpu/virtualPage.h>
#include <realPage.h>


extern "C"{
	extern uchar __kernel_base[];
}

// ページテーブルが丸見えという便利な配列。ただし、メモリが割り当てられていないことがあるのでrootPageDirで確認
u32* VIRTUALPAGE::pageTableArray((u32*)pageTableArrayStarts);
u32* const VIRTUALPAGE::rootPageDir((u32*)0xfffff000);



