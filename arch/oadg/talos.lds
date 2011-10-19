/********************************************** LDSCRIPT FOR OADG ARCHITECTURE
	Copyright (C) 2004- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id$
*/


OUTPUT_FORMAT("elf32-i386")
EXTERN(__init16)
GROUP(kproc.o kernel.o ulib.o libuser.a)

MEMORY {
	IMAGE (wxi) : ORIGIN = 0x10000, LENGTH = 0x80000
}

SECTIONS {
	__kernel_base = 0xf0000000; /* NOTE:変更するときは4MiB単位 */
	__cxa_pure_virtual = 0;

	__init_LMA = LOADADDR(.init);
	__user_LMA = LOADADDR(.user);
	__ulib_LMA = LOADADDR(.ulib);
	__kernel_LMA = LOADADDR(.kernel);
	__init_VMA = ADDR(.init);
	__user_VMA = ADDR(.user);
	__kernel_VMA = ADDR(.kernel);
	__init_size = SIZEOF(.init);
	__user_size = SIZEOF(.user);
	__ulib_size = SIZEOF(.ulib);
	__kernel_size = SIZEOF(.kernel);

	__LMA32_16 = ABSOLUTE(__LMA32) & 0xffff;
	__VMA_GDT = ABSOLUTE(__LMA_GDT) + __kernel_base;
	__VMA_GDTPT = ABSOLUTE(__LMA_GDTPT) + __kernel_base;
	__TSSPH = ABSOLUTE(__LMA_TSSPH) + __kernel_base;
	__kernelPageDir_VMA = ABSOLUTE(__kernelPageDir) + __kernel_base;

	/* utilities */
	__arch_CGA_VRAM = __kernel_base + 0xb8000;
	__kProcHeader_LMA = LOADADDR(.kProcHeader);

	/* VARIANT of ADDRESSSPACE */
	__arch_LOMEM = __init_LMA +
		SIZEOF(.init) +
		SIZEOF(.user) +
		SIZEOF(.ulib) +
		SIZEOF(.kernel);

	.init 0x10000 : {
		LONG(ABSOLUTE(__init16));
		LONG(__arch_LOMEM);
		*(.kp_init)
		*(.kp_ro)
		*(.kp_rw)
		*(.kp_def)
	} AT > IMAGE

	.kernel (. + ABSOLUTE(__kernel_base)) : {
		__core_base = ABSOLUTE(.);
		*(.kernel_ro*)
		LONG(0);
		*(.kernel_rw*)
		. = ALIGN(4096);
		__kernel_heap = ABSOLUTE(.);
	} AT > IMAGE

	.ulib __kernel_base : {
		*(.ulib_ro)
		*(.ulib_rw)
		. = ALIGN(4096);
	} AT > IMAGE

	.kProcHeader 0 : {
		*(.kp_def)
		LONG(0);
	} AT > IMAGE

	.user 0 : {
		*(.portDef)
		*(.text*)
		*(.rodata*)
		*(.data*)
		*(.eh_frame)
		*(.gnu*)
/*		__GlobalConstructor = .;
		*(.ctors)
		*(SORT(.ctors.*))
		LONG(0);
		__GlobalDestructor = .;
		*(.dtors)
		*(SORT(.dtors.*))
		LONG(0);
*/		*(.bss)
	} AT > IMAGE

	/DISCARD/ : {
		*(*)
	} AT > IMAGE
}

