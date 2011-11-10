/********************************************************* MAPPING VIRTUALPAGE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <map.h>
#include <task.h>


void MAP::SystemRequest(void* message){
	//TODO:この「マップ関連のシステムコールメッセージを処理する」スタブを埋める。メッセージがマップ関連でない場合もあり得るが、その時はエラーコードを書きこんで終了する
}



runit REALPAGEMAP::GetPage(punit index){
	if(pages <= index){
		return 0;
	}
	return (start + index) * PAGESIZE;
}


runit COMMONMAP::GetPage(punit indek){
	return 0; //TODO:スタブ:originTaskにページだけ切り替えてoriginPage+indexが指す内容を取ってくる
}