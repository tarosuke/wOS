/********************************************************* MAPPING VIRTUALPAGE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _MAP_
#define _MAP_

#include <types.h>
#include <resource.h>
#include <lock.h>


/// 抽象マップクラス
class MAP : public RESOURCE{
	friend class MAPHANDLER;
public:
	virtual punit GetPage(punit entry) = 0;
protected:
private:
};

/// ユーザプロセスごとのマップハンドラ
class MAPHANDLER : public HANDLER{
public:
	MAPHANDLER(MAP& org, void* start) :
		HANDLER(org), org(org), start((munit)start / PAGESIZE){};
	runit GetPage(void* v){
		return org.GetPage((munit)v / PAGESIZE);
	};
private:
	MAP& org;
	const punit start; //仮想ページで
};



/// マップのバリアント

// 実アドレス指定のマップ
class REALPAGEMAP : public MAP{
public:
	runit GetPage(punit pageIndex);
	REALPAGEMAP(runit start, punit pages) : start(start), pages(pages){};
private:
	const runit start;
	const punit pages;
};


#endif
