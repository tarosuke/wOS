/********************************************* PROCESSOR DEPENDED PART OF TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#include <cpu/task.h>
#include <realPage.h>


CPUTASK::CPUTASK() : pageRoot(0){};

CPUTASK::CPUTASK(void* stack) :
	stack(stack),
	pageRoot(REALPAGE::GetPages(1) * PAGESIZE){
	//TODO:pageRootに仮想ページを割り当てて初期化
};
