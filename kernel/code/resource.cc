/********************************************************** RESOURCE & HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <resource.h>


void RESOURCE::Use(){
	KEY key(lock); users++;
};
void RESOURCE::Release(){
	{ KEY key(lock); users--; }
	if(!users){
		delete this;
	}
};
