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
typedef u128 funit;

/// time(unit:microseconds)
typedef u64 tunit;
#ifdef __cplusplus
namespace TIME{
	static const tunit INFINITE = ~(tunit)0;
	static const tunit CONTINUE = (INFINITE - 1);
};
#else
#define TIME_INFINITE = (~(tunit)0)
#define TIME_CONTINUE = (TIME_INFINITE - 1)
#endif

/// wOS charactor
typedef u32 tchar;


/// UUID
typedef u128 UUID;

/// VERSION
typedef struct{
	u32 major;
	u32 minor;
	u32 release;
}VERSION;


/// elementof
#define elementof(a) (sizeof(a) / sizeof(*a))


#endif
