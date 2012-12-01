/********************************************************** RESOURCE & HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _RESOURCE_
#define _RESOURCE_

#include <types.h>
#include <lock.h>
#include <map.h>
#include <servers.h>


/// 通信路とMAPの複合体
///TODO:通信路をMAPに統合。つまりただのMAPにする。あとMAPは複数参照を可能にしとく。
class RESOURCE{
	friend class HANDLER;
public:
	RESOURCE(TASK* current, SERVER_DESCRIPTOR* target, MAP* map = 0)
		: map(map){
		terminals[0] = current;
	};
	virtual ~RESOURCE(){};
	virtual void Send(void* message);
	virtual runit GetPage(punit);
protected:
private:
	MAP* const map;
	TASK* terminals[2];
	static void IlligalOperation();
};


#endif
