/*************************************** COMMON LDSCRIPT FOR OADG ARCHITECTURE
	Copyright (C) 2004- project wOS (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
*/

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

__BOOTLOCK = ABSOLUTE(__LMA_BOOTLOCK) + __kernel_base;

/* utilities */
__arch_CGA_VRAM = __kernel_base + 0xb8000;
__pageRoot_VMA = ABSOLUTE(__pageRoot) + __kernel_base;
__preset_pages = (__kernel_heap - __kernel_base) / 4096;

/* VARIANT of ADDRESSSPACE */
__image_size = SIZEOF(.init) +
SIZEOF(.user) +
SIZEOF(.ulib) +
SIZEOF(.kernel);
__arch_LOMEM = __image_size + __init_LMA;

.init 0x10000 : {
	KEEP(*(.prefix))
	. = ALIGN(16);
	LONG(ABSOLUTE(__init16));
	LONG(__arch_LOMEM);
	KEEP(*(.init16))
	. = ALIGN(4);
	__kernelProcessDefs16 = .;
	KEEP(*(.pdef16))
	LONG(0);
	KEEP(*(.init))
} AT > IMAGE

.kernel (. + ABSOLUTE(__kernel_base)) : {
	__core_base = ABSOLUTE(.);
	*(.text*)
	. = ALIGN(4);
	__KernelConstructor = .;
	KEEP(heap.o(.ctors))
	KEEP(heap.o(.init_array))
	KEEP(pic.o(.ctors))
	KEEP(pic.o(.init_array))
	KEEP(*(.ctors))
	KEEP(*(.init_array))
	KEEP(*(SORT(.ctors.*)))
	KEEP(*(SORT(.init_array.*)))
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
	KEEP(*(.ulib_h))
	*(.ulib_ro)
	. = ALIGN(4096);
} AT > IMAGE

.user 0 : {
	*(.userProcess)
	LONG(0)
} AT > IMAGE
