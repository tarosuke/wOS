/********************************************************** RESOURCE & HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <resource.h>
#include <debug.h>


// 抽象リソースクラス
void RESOURCE::IlligalOperation(){
	//TODO:不正動作処理：ログを出力してプロセスを再起動
	Panic("Illigal rosource operation.");
}

void RESOURCE::SystemRequest(void* message){
	IlligalOperation();
}

runit RESOURCE::GetPage(punit){
	IlligalOperation();
	return 0;
}


// マップリソース
MAPRESOURCE::MAPRESOURCE(void* start, munit size) :
	map(new COMMONMAP(size)),
	start((munit)start / PAGESIZE){}
MAPRESOURCE::MAPRESOURCE(void* start, runit pa, munit size) :
	map(new FIXMAP(pa, size)),
	start((munit)start / PAGESIZE){}
MAPRESOURCE::MAPRESOURCE(void* start, MAP& map) :
	map(map),
	start((munit)start / PAGESIZE){}

MAPRESOURCE::~MAPRESOURCE(){};
runit MAPRESOURCE::GetPage(punit target){
	return map.GetPage(target - start);
}

