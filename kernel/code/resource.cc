/********************************************************** RESOURCE & HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <resource.h>
#include <debug.h>


const SERVER_DESCRIPTOR RESOURCE::nullServer = {
	0, { 0, 0, 0 }, 0, { 0, 0, 0 }
};



RESOURCE::RESOURCE(const SERVER_DESCRIPTOR& target, MAP* map) :
	REFERENCE(new BODY(target, map)){}

RESOURCE::RESOURCE(MAP& map) :
	REFERENCE(new BODY(nullServer, &map)){}


















void RESOURCE::BODY::IlligalOperation(){
	//TODO:不正動作処理：本来はログを出力してプロセスを再起動
	Panic("Illigal resource operation.");
}
