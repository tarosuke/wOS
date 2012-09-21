/********************************************************* MAPPING VIRTUALPAGE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _MAP_
#define _MAP_

#include <types.h>
#include <reference.h>
#include <debug.h>
#include <vector.h>


/// 抽象マップ
class MAP : public REFERENCE{
	MAP();
	void operator=(MAP&);
public:
	MAP(BODY*) : REFERENCE(body), body(body){};
	MAP(MAP& org) : REFERENCE(org.body), body(org.body){};
	runit GetPage(punit p){
		assert(body);
		return (*body).GetPage(p); };
	class BODY : public REFERENCE::BODY{
	protected:
		BODY(munit size) :
			pages(size / PAGESIZE){};
		virtual ~BODY() = 0;
		punit pages;
	public:
		virtual runit GetPage(punit) = 0;
	};
protected:
	BODY* const body;
private:
};


class FIXMAP : public MAP::BODY{
public:
	FIXMAP(runit r, munit size) :
		MAP::BODY(size),
		start(r / PAGESIZE){};
	~FIXMAP(){};
	runit GetPage(punit);
private:
	const punit start;
};


class TASK;
class COMMONMAP : public MAP::BODY{
public:
	COMMONMAP(munit size) :
		MAP::BODY(size){};
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
