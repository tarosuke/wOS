/********************************************* PROCESSOR DEPENDED PART OF TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#include <cpu/task.h>
#include <realPage.h>


CPUTASK::CPUTASK() :
	TASKVIRTUALPAGE(0),
	stack((void*)&kernelStack[CF_KERNELSTACK_ENTRIES]){
	//現在のページ構成をそのまま使う
};

CPUTASK::CPUTASK(const CPUTASK& org) :
	stack((void*)&kernelStack[CF_KERNELSTACK_ENTRIES]){
	//TODO:pageRootに仮想ページを割り当てて初期化
};
