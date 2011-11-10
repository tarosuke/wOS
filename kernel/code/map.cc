/********************************************************* MAPPING VIRTUALPAGE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <map.h>






runit REALPAGEMAP::GetPage(punit index){
	if(pages <= index){
		return 0;
	}
	return (start + index) * PAGESIZE;
}
