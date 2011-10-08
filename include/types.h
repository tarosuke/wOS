/*********************************************************************** TYPES
	Copyright (C) 2009- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id: e76d06cdf231d2e582596215c8b39e99e191017a $
*/


#ifndef _TYPES_
#define _TYPES_

#include <config.h>
#include <cpu/types.h>


/// position & size of filesystem
typedef u64 funit;


/// time(unit:microseconds)
typedef u64 time;
namespace TIME{
	static const time infinite = (time)-1LL;
	static const time continuous = (time)-2LL;
};

/// talos charactor
typedef u32 tchar;


/// version
typedef struct{
	u16 major;	/** 互換性がなくなった時にインクリメント(ただし旧verをサポートすること) */
	u16 minor;	/** 修正、要素の追加などでインクリメント */
	u32 release;	/** 実装のリリースでインクリメント */
}VERSION;





#endif

