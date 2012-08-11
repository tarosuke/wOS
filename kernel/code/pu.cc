/**************************************** GENERIC PROCESSING UNIT MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <pu.h>
#include <lock.h>
#include <debug.h>
#include <core.h>


PU::PUPLACER PU::pus;
ATOMIC PU::numOfPu;
LOCK PU::dispatching;
TASK::TASKQUEUE PU::ready;

extern "C"{
	//ディスパッチャのエントリポイント(例外ハンドラの終了前に呼ばれる)
	void __Dispatch(){
		PU::Dispatch();
	}
}
