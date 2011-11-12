/********************************************** LDSCRIPT FOR OADG ARCHITECTURE
	Copyright (C) 2004- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
*/


OUTPUT_FORMAT(elf64-x86-64)
ENTRY(__init16)
INPUT(ulib.o libuser.a)

MEMORY {
	IMAGE (wxi) : ORIGIN = 0x10000, LENGTH = 0x80000
}

SECTIONS {
	__kernel_base = 0xfffffffff0000000; /* NOTE:変更するときは4MiB単位 */
	__kernelBase_hi = __kernel_base >> 32;
	__kernelBase_lo = __kernel_base & 0xffffffff;

	INCLUDE ../../arch/oadg/common.lds
}

