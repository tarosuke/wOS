/********************************************************** RESOURCE & HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _RESOURCE_
#define _RESOURCE_

#include <types.h>
#include <lock.h>
#include <reference.h>
#include <map.h>


/// MAPやソケットなどの抽象リソース
class RESOURCE{
	friend class HANDLER;
public:
	RESOURCE(){};
	virtual ~RESOURCE(){};
	virtual void SystemRequest(void* message);
	virtual runit GetPage(punit);
protected:
private:
	static void IlligalOperation();
};


class MAPRESOURCE : public RESOURCE{
public:
	//COMMONMAP用
	MAPRESOURCE(void* start, munit size);
	//実アドレス指定マップ用
	MAPRESOURCE(void* start, runit pa, munit size);
	//マップ利用者用
	MAPRESOURCE(void* start, MAP& map);
	~MAPRESOURCE();
	runit GetPage(punit);
protected:
	MAP map;
	punit start;
};


#endif
