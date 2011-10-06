/************************************************************ CONFIGURE KERNEL
	Copyright (C) 2010- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id: 4fb8004d2a2073a1d865c64aae03e15c8227cd9f $

	ビルド位置にあるconfig.hのwrapper。
	config.hをincludeして不足している設定を追加したりする。
*/

#ifndef _CONFIG_
#define _CONFIG_

#include "config"

/* プロセッサ数修正 */
#ifndef CF_MAX_PROCESSORs
#	define CF_MAX_PROCESSORs 1
#endif

/* メモリブロック数 */
#ifndef CF_MAX_MEMORYBLOCKs
#	define CF_MAX_MEMORYBLOCKs 16
#endif

/* デバグコンソール設定*/
#ifndef CF_DEBUG_LEVEL
#	define CF_DEBUG_LEVEL 0
#endif
#define CF_NULL_CONSOLE (!(CF_SERIAL_CONSOLE || CF_CGA_CONSOLE)\
	|| !CF_DEBUG_LEVEL)
#if (1 < (CF_SERIAL_CONSOLE + CF_CGA_CONSOLE))
#	error "指定できるデバグコンソールは一つだけです。"
#endif


/* 画面モードが設定されている時はCGAコンソールは使えない */
#if (CF_DEFAULT_DISPLAYMODE & CF_CGA_CONSOLE)
#	error "画面モードが設定されている時はCGAコンソールは使えません。"
#endif
/* 画面モードリトライはVGAまで */
#ifndef CF_DISPLAYMODE_RETRIES_TO
#	define CF_DISPLAYMODE_RETRIES_TO 0xc110
#endif
/* 画面モードリトライは3つ置き(解像度を一段階づつ下げる) */
#ifndef CF_DISPLAYMODE_RETRIES_STEP
#	define CF_DISPLAYMODE_RETRIES_STEP 3
#endif


#endif
