/******************************************** LDSCRIPT FOR OADG ARCHITECTURE */
/* $Id: 204bed2aa35b4635c47708eeda13e5b38d3df5d4 $ */

OUTPUT_FORMAT("elf32-i386")

SECTIONS {
	. = 0x7c00;
	.loader : {
		*(.text)
		*(.data)
		*(.bss)
	}
	.kernelParamBlock : {
		kernelParams = .;
		/* define kernel parametors here */
		LONG(0)  /* force memsize in KB(detect if this is 0) */
		SHORT(0) /* displayMode use defaultmode if 0 */
		SHORT(0xc112) /* displayMode(for fallback) */
	}
	. = 0x7df4;
	.loaderParamBlock : {
		driveOfPartition = .;
		SHORT(0x0000);
		startOfPartition = .;
		LONG(0);
	}
	.signeture : {
		LONG(ABSOLUTE(kernelParams) + 0xc0000000) /* index for kernel */
		SHORT(0xaa55)
	}
}
