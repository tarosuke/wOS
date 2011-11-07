/***************************************************************** KERNEL CORE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <core.h>
#include <cpu/cpu.h>
#include <config.h>
#include <debug.h>


TIME CORE::uptime(0, 0);
static CORE core;

void CORE::SystemCall(){
	assert(true);
	//TODO:パラメタの受け取りとか
};

