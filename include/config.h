/************************************************************ CONFIGURE KERNEL
	Copyright (C) 2010- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id$

	ビルド位置にあるtalos.confのwrapper。
	デフォルト設定を作ったり、問題があればコンパイルエラーにする。
	個々の設定の説明はtalos.confに書いてある。
*/

#ifndef _CONFIG_
#define _CONFIG_

#include "config"


#ifndef CF_HZ
#	define CF_HZ 20
#endif

#ifndef CF_MAX_PROCESSORs
#	define CF_MAX_PROCESSORs 1
#endif
#if !CF_MAX_PROCESSORs
#error "CF_MAX_PROCESSORs must be least 1."
#endif

#ifndef CF_TASKROTATE_CYCLE
#	define CF_TASKROTATE_CYCLE 5
#endif

#ifndef CF_MAX_IRQs
#	define CF_MAX_IRQs 17
#endif

#ifndef CF_MAX_IRQ_SHAREs
#	define CF_MAX_IRQ_SHAREs 4
#endif

#ifndef CF_MAX_MEMORYBLOCKs
#	define CF_MAX_MEMORYBANKs 16
#endif

#ifndef CF_DEBUG_LEVEL
#	define CF_DEBUG_LEVEL 0
#endif



#endif
