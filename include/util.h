/******************************************************************* UTILITIES
 *	Copyright (C) 2012- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#ifndef _UTIL_
#define _UTIL_


//繰り上げ、繰り下げ処理(tは2^n かつ 0 < tであること)
#define DivDown(e, t)	((e) / (t))
#define DivUp(e, t)	DivDown((e) + (t) - 1, t)
#define RoundDown(e, t)	((e) % ~((t) - 1))
#define RoundUp(e, t)	RoundDown((e) + (t) - 1, t)

#endif
