/*********************************************************** SEGMENT SELECTORS
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _ARCH_SELECTORS_
#define _ARCH_SELECTORS_

#include <config.h>

#if CF_IA32
#define KCSel	8
#define KDSel	(KCSel + 8)
#define UCSel	(KDSel + 8)
#define UDSel	(UCSel + 8)
#define	TCSel	(UDSel + 8)
#define	TSSSel	(TCSel + 8)
#define GDTLIMIT (TSSSel + 8 * CF_MAX_PROCESSORs)
#endif

#if CF_AMD64
#define KCSel	8
#define KDSel	(KCSel + 8)
#define UCSel	(KDSel + 8)
#define UDSel	(UCSel + 8)
#define	UCSel32	(UDSel + 8)
#define	UDSel32	(UCSel32 + 8)
#define	TSSSel	(UDSel32 + 8)
#define	TCSel	UCSel32
#define GDTLIMIT (TSSSel + 16 * CF_MAX_PROCESSORs)
#endif

#endif
