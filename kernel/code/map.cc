/********************************************************* MAPPING VIRTUALPAGE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <map.h>
#include <task.h>


runit FIXMAP::GetPage(punit page){
	if(page < start){
		// マップより手前
		return 0;
	}
	page -= start;
	if(pages <= page){
		// マップより先
		return 0;
	}
	return start + page;
}


COMMONMAP::COMMONMAP(void* start, munit size, TASK& master) :
	MAP::BODY(start, size),
	master(master){
		master.MapLock(); //masterの空間解放を禁止する
}

runit COMMONMAP::GetPage(punit page){
	return 0;
}

COMMONMAP::~COMMONMAP(){
	master.MapUnlock(); //masterの空間解放を許可する
}
