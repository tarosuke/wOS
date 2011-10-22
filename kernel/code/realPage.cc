/******************************************************* REALPAGE MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <realPage.h>

REALPAGE::MEMORYBLOCK REALPAGE::memoryBanks[CF_MAX_MEMORYBANKs];
uint REALPAGE::numOfBanks(0);

punit REALPAGE::GetPages(punit pages){
	punit page(0);
	for(uint i(0); i < numOfBanks && !(page = memoryBanks[i].GetPages(pages)); i++);
	return page;
}

void REALPAGE::ReleasePage(punit page){
	for(uint b(0); b < numOfBanks && !memoryBanks[b].ReleasePage(page); b++);
}

