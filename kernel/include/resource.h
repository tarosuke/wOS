/********************************************************** RESOURCE & HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _RESOURCE_
#define _RESOURCE_

#include <types.h>
#include <lock.h>
#include <reference.h>


/// MAPやソケットなどの抽象リソース
class RESOURCE{
	friend class HANDLER;
public:
	RESOURCE(){};
	virtual ~RESOURCE(){};
	virtual void SystemRequest(void* message);
	virtual runit GetNewPage(void*);
protected:
private:
	static void IlligalOperation();
};


class MAPRESOURCE : public RESOURCE{
public:
	MAPRESOURCE(punit start);
	~MAPRESOURCE();
	runit GetNewPage(void*);
private:
	punit start;
};


#endif
