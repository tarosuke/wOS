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
	MAPRESOURCE(void* start, MAP& map);
	MAPRESOURCE(void* start, munit size);
	~MAPRESOURCE();
	runit GetPage(punit);
private:
	MAP map;
	punit start;
};


#endif
