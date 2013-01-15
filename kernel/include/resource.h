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
#include <reference.h>


/// 通信路とMAPの複合体...の参照
class RESOURCE : public REFERENCE{
public:
	RESOURCE(const SERVER_DESCRIPTOR& target, MAP* map);
	RESOURCE(MAP& map);
private:
	static const SERVER_DESCRIPTOR nullServer;
	/// 通信路とMAPの複合体
	class BODY : public REFERENCE::BODY{
	public:
		BODY(const SERVER_DESCRIPTOR& target, MAP* map) :
			map(map),
			target(target){


		};
		runit GetPage(punit);
		void Signal(TASK& task, u32 value);
	protected:
	private:
		MAP* const map;
		const SERVER_DESCRIPTOR target;
		static void IlligalOperation();
	};
	BODY* GetBody(){ return (BODY*)body; };
};


#endif
