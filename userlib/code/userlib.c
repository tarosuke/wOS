/*********************************************************** USERLIBRARY TABLE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <userlib/userlib.h>
#include <userlib/raw.h>

#define D(n) n : n
const struct _ULIB ulib __attribute__((section(".ulibdef"))) = {
	version : 0x0000005a, /* 0.0.0.90 */
	D(RawSystemcall),
	D(WaitNextMessage),
	D(HEAP_Init),
};
