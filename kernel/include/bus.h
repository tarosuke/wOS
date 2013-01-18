/***************************************************** UTILITIES FOR BUSDRIVER
 *	Copyright (C) 2013- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _BUS_
#define _BUS_


#include <types.h>

class BUS{
protected:
	void FindDriver(UUID driverType, void* condition); //conditionにマッチしたdriverTypeのドライバを起動する。
};

#endif
