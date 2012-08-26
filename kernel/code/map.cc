/********************************************************* MAPPING VIRTUALPAGE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <map.h>
#include <task.h>
#include <pu.h>
#include <realPage.h>


runit FIXMAP::GetPage(punit page){
	return page < pages ? start + page : 0;
}


runit COMMONMAP::GetPage(punit page){
	if(pages <= page){
		return 0;
	}
	const uint branch(page >> 8);
	LEAF* leaf(tree[branch]);
	if(!leaf){
		//割り当てられてないので新しく割り当てる
		leaf = new LEAF;
		tree.Set(branch, leaf);
	}
	runit& p((*leaf).pages[page & 255]);
	if(!p){
		//ページが割り当てられていないので割り当てる
		p = REALPAGE::GetPages();
		assert(p);
	}

	return p;
}
