/********************************************** LDSCRIPT FOR OADG ARCHITECTURE
	Copyright (C) 2004- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
*/


OUTPUT_FORMAT("elf32-i386")
ENTRY(__init16)
GROUP(kproc.o ulib.o libuser.a)

MEMORY {
	IMAGE (wxi) : ORIGIN = 0x10000, LENGTH = 0x80000
}

SECTIONS {
	__kernel_base = 0xf0000000; /* NOTE:変更するときは4MiB単位 */
	__kernelBase_hi = 0;
	__kernelBase_lo = __kernel_base;
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
	__TSSPH = ABSOLUTE(__LMA_TSSPH) + __kernel_base;

	/* utilities */
	__arch_CGA_VRAM = __kernel_base + 0xb8000;
	__kProcHeader_LMA = LOADADDR(.kProcHeader);
	__pageRoot_VMA = ABSOLUTE(__pageRoot) + __kernel_base;

	/* VARIANT of ADDRESSSPACE */
	__arch_LOMEM = __init_LMA +
		SIZEOF(.init) +
		SIZEOF(.user) +
		SIZEOF(.ulib) +
		SIZEOF(.kernel);

	.init 0x10000 : {
		LONG(ABSOLUTE(__init16));
		LONG(__arch_LOMEM);
		KEEP(*(.kp_init))
		KEEP(*(.kp_ro))
		KEEP(*(.kp_rw))
		KEEP(*(.kp_def))
	} AT > IMAGE

	.kernel (. + ABSOLUTE(__kernel_base)) : {
		__core_base = ABSOLUTE(.);
		*(.text*)
		. = ALIGN(4);
		__KernelConstructor = .;
		KEEP(*(.ctors))
		KEEP(*(SORT(.ctors.*)))
		QUAD(0);
		__KernelDestructor = .;
		KEEP(*(.dtors))
		KEEP(*(SORT(.dtors.*)))
		QUAD(0);
		*(.rodata*)
		*(.data*)
		*(.eh_frame)
		*(.gnu*)
		*(.bss*)
		. = ALIGN(4096);
		__kernel_heap = ABSOLUTE(.);
	} AT > IMAGE

	.ulib __kernel_base : {
		*(.ulib_h)
		*(.ulib_ro)
		. = ALIGN(4096);
	} AT > IMAGE

	.kProcHeader 0 : {
		*(.kp_def)
		LONG(0);
	} AT > IMAGE

	.user 0 : {
		*(.userProcess)
		LONG(0)
	} AT > IMAGE
}

