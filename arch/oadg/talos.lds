/********************************************** LDSCRIPT FOR OADG ARCHITECTURE
	Copyright (C) 2004- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id: e4e4b9f75cd9fee7a9df24990857b260256b5915 $
*/


OUTPUT_FORMAT("elf32-i386")
EXTERN(__init16)
GROUP(kernel.ko ulib.o libuser.a)

MEMORY {
	IMAGE (wxi) : ORIGIN = 0x10000, LENGTH = 0x80000
}

SECTIONS {
	__kernel_base = 0xfe000000;
	__ulib_start = 0xfd000000;
	__archPageTableArray = 0xffc00000;
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

	__LMA32_16 = ABSOLUTE(LMA32) & 0xffff;

	/* utilities */
	__arch_CGA_VRAM = __kernel_base + 0xb8000;

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
		. = . + 8192;
		__kernel_stack = .;
		*(.kernel_rw*)
		. = ALIGN(4096);
		__core_base = ABSOLUTE(.);
		*(.kernel_ro*)
		LONG(0);
		. = ALIGN(4096);
		__kernel_heap = ABSOLUTE(.);
	} AT > IMAGE

	.ulib __ulib_start : {
		*(.ulib)
		*(.ulib_ro)
		. = ALIGN(4096);
	} AT > IMAGE

	.user 0 : {
		*(.portDef)
		*(.text*)
		*(.rodata*)
		*(.data*)
		*(.eh_frame)
		*(.gnu*)
		__GlobalConstructor = .;
		*(.ctors)
		*(SORT(.ctors.*))
		LONG(0);
		__GlobalDestructor = .;
		*(.dtors)
		*(SORT(.dtors.*))
		LONG(0);
		*(.bss)
	} AT > IMAGE

	/DISCARD/ : {
		*(*)
	} AT > IMAGE
}

