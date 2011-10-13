/************************************ LDSCRIPT TO CONVERT USERMODE LIBRARY OBJECTS
	Copyright (C) 2009- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id$
*/

SECTIONS {
	.kp_init : {
		*(.init)
		. = ALIGN(4);
		__kernelProcessDefs16 = .; /* 16bitsモードの初期化後にエントリが順にcallされる */
		*(.pdef16)
		LONG(0);
	}
	.kp_ro : {
		*(.text*)
		. = ALIGN(4);
		__ArchCons = .;
		*(.ctors)
		*(SORT(.ctors.*))
		LONG(0);
		__ArchDest = .;
		*(.dtors)
		*(SORT(.dtors.*))
		LONG(0);
		*(.rodata*)
	}
	.kp_rw : {
		*(.data*)
		*(.eh_frame)
		*(.gnu*)
		*(.bss*)
	}
	.kp_def : {
		*(.pdef32)
	}
	.kernel_rw : {
		*(.k_rw)
	}
	.kernel_ro : {
		*(.k_ro)
	}
}
