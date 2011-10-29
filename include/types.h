/*********************************************************************** TYPES
	Copyright (C) 2009- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
*/


#ifndef _TYPES_
#define _TYPES_

#include <config.h>
#include <cpu/types.h>


/// position & size of filesystem
typedef u64 funit;


/// time(unit:microseconds)
typedef u64 tunit;
namespace TIME{
	static const tunit infinite = (tunit)-1LL;
	static const tunit continuous = (tunit)-2LL;
};

/// talos charactor
typedef u32 tchar;





#endif

