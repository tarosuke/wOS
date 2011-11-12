/*********************************************************** USERLIBRARY TABLE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#define DEF(n) .global n; .extern __##n; n: jmp __##n


.section .ulibdef
DEF(RawSystemcall)
DEF(WaitNextMessage)
DEF(HEAP_Init)
DEF(HEAP_Get)
DEF(HEAP_Release)
