/********************************************************* MAPPING VIRTUALPAGE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _MAP_
#define _MAP_

#include <types.h>
#include <debug.h>
#include <vector.h>


/// 抽象マップ
class MAP{
	MAP();
protected:
	MAP(munit size) : pages(size / PAGESIZE){};
	virtual ~MAP() = 0;
	punit pages;
public:
	virtual runit GetPage(punit) = 0;
};


class FIXMAP : public MAP{
public:
	FIXMAP(runit r, munit size) :
		MAP(size),
		start(r / PAGESIZE){};
	~FIXMAP(){};
	runit GetPage(punit);
private:
	const punit start;
};


class TASK;
class COMMONMAP : public MAP{
public:
	COMMONMAP(munit size) :
		MAP(size){};
	runit GetPage(punit);
	~COMMONMAP(){};
private:
	class LEAF{
	public:
		runit pages[256];
	};
	VECTOR<LEAF> tree;
};


#endif
