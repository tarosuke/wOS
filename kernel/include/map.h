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
	virtual runit GetPage(punit) = 0;
	void SystemRequest(void*);
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

// 何もしないマップ
class NULLMAP : public MAP{
public:
	runit GetPage(punit){ return 0; };
	NULLMAP(){};
};

// 実アドレス指定のマップ
class REALPAGEMAP : public MAP{
public:
	runit GetPage(punit pageIndex);
	REALPAGEMAP(runit start, punit pages) : start(start), pages(pages){};
private:
	const runit start;
	const punit pages;
};

// 共有メモリのマップ
class TASK;
class COMMONMAP : public MAP{
public:
	runit GetPage(punit pageIndex);
	COMMONMAP(runit start,
		punit pages,
		TASK& originTask,
		punit originPage) :
		start(start),
		pages(pages),
		originTask(originTask),
		originPage(originPage){};
private:
	const runit start;
	const punit pages;
	TASK& originTask;
	const punit originPage;
};


#endif
