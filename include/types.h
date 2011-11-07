/*********************************************************************** TYPES
	Copyright (C) 2009- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
*/


#ifndef _TYPES_
#define _TYPES_

#include <config.h>
#include <cpu/types.h>


///デフォルト封じ(デフォルトコンストラクタとか余計なお世話なのですよ)
#define UNDEFAULT(t) t(); t(t&); void operator=(t&);

/// position & size of filesystem
typedef u64 funit;

/// time(unit:microseconds)
typedef u64 tunit;

/// talos charactor
typedef u32 tchar;


#endif
