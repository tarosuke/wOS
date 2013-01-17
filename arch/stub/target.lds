/********************************************** LDSCRIPT FOR STUB ARCHITECTURE
 C opyright (C) 2013- project wOS (https://github.com/tarosuke/wOS)*
	check LICENSE.txt. If you don't have the file, mail us.
*/


OUTPUT_FORMAT("elf32-i386")
ENTRY(__init16)
GROUP(ulib.o libuser.a)

MEMORY {
	IMAGE (wxi) : ORIGIN = 0x10000, LENGTH = 0x80000
}

SECTIONS {
	__kernel_base = 0xf0000000; /* NOTE:変更するときは4MiB単位 */
	__kernelBase_hi = 0;
	__kernelBase_lo = __kernel_base;

	INCLUDE ../../arch/oadg/common.lds
}

