/********************************************************* MAPPING VIRTUALPAGE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _MAP_
#define _MAP_

#include <types.h>
#include <resource.h>
#include <reference.h>
#include <resource.h>



/// 抽象マップ
class MAP : public REFERENCE{
	MAP();
	void operator=(MAP&);
public:
	MAP(BODY*) : REFERENCE(body), body(body){};
	MAP(MAP& org) : REFERENCE(org.body), body(org.body){};
	runit GetPage(punit p){
		return body ? (*body).GetPage(p) : 0; };
	class BODY : public REFERENCE::BODY{
	protected:
		BODY(void* start, munit size) :
			startPage((munit)start / PAGESIZE),
			pages(size / PAGESIZE){};
		punit startPage;
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
	FIXMAP(runit r, void* v, munit size) :
		MAP::BODY(v,size),
		start(r / PAGESIZE){};
	runit GetPage(punit);
private:
	const runit start;
};


class COMMONMAP : public MAP::BODY{
public:
	COMMONMAP(void* v, munit size);
private:
	class TASK* master; //マスタータスク。終了したら共有メモリ自体が無効化する。
};






class USERMAP : public RESOURCE{
	USERMAP();
public:
	USERMAP(void* v, MAP& map) :
		map(map), startPage((munit)v / PAGESIZE){};
private:
	MAP map;
	const punit startPage;
};

#endif
