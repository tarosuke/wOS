/***************************************************************** ARCH CONFIG
 *	Copyright (C) 2010- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _ARCH_CONFIG_
#define _ARCH_CONFIG_


#ifndef CF_MAX_IRQs
#	define CF_MAX_IRQs 17
#endif

#ifndef CF_KERNELSTACK_ENTRIES
#	define CF_KERNELSTACK_ENTRIES 256
#endif

#ifndef CF_MAX_IRQ_SHAREs
#	define CF_MAX_IRQ_SHAREs 4
#endif

#ifndef CF_MAX_MEMORYBLOCKs
#	define CF_MAX_MEMORYBANKs 16
#endif

#ifndef CF_PAE
#	define CF_PAE 0
#endif

#ifndef CF_VESA_MODE
#	define CF_VESA_MODE 0 //自動取得
#endif
#ifndef CF_FORCE_VESA_MODE
#	define CF_FORCE_VESA_MODE 0
#endif

#ifndef CF_SERIAL_DEBUG
#	define CF_SERIAL_DEBUG 0
#endif

#endif
