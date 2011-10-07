/******************************************** LDSCRIPT FOR OADG ARCHITECTURE */
/* $Id: d5dc48bc4ea8f56a66b316ac947dcc9928548b82 $ */

OUTPUT_FORMAT("elf32-i386")


MEMORY {
	IMAGE (wxi) : ORIGIN = 0x7c00, LENGTH = 446
}

SECTIONS {
	.mbr 0x7e00 : {
		*(.text)
		partitionTable = 446;
	} AT > IMAGE
}
