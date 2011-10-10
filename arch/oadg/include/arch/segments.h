


#ifndef _ARCH_SELECTORS_
#define _ARCH_SELECTORS_

/******************************************************** SEGMENT SELECTORS */
#define KCSel	8
#define KDSel	(KCSel + 8)
#define UCSel	(KDSel + 8)
#define UDSel	(UCSel + 8)
#define	TCSel	(UDSel + 8)
#define	TSSSel	(TCSel + 8)
#define GDTLIMIT (TSSSel + 8 * CF_MAX_PROCESSORs)

#endif
