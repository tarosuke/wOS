/********************************************************** RESOURCE & HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <resource.h>
#include <pu.h>


// 抽象リソースクラス
void RESOURCE::IlligalOperation(){
	//TODO:不正動作処理：ログを出力してプロセスを再起動
}

void RESOURCE::SystemRequest(void* message){
	IlligalOperation();
}

runit RESOURCE::GetNewPage(void*){
	IlligalOperation();
	return 0;
}


// マップリソース
MAPRESOURCE::MAPRESOURCE(void* start, munit size) :
	map(new COMMONMAP(start, size, PU::GetCurrentTask())),
	start((munit)start / PAGESIZE){}
MAPRESOURCE::MAPRESOURCE(void* start, MAP& map) :
	map(map),
	start((munit)start / PAGESIZE){}

MAPRESOURCE::~MAPRESOURCE(){};
runit MAPRESOURCE::GetNewPage(void* target){
	const punit pageNum(((munit)target / PAGESIZE) - start);
	return map.GetPage(pageNum);
}

