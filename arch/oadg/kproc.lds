/************************************ LDSCRIPT TO CONVERT USERMODE LIBRARY OBJECTS
	Copyright (C) 2009- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id: bf1ef64204a9a73071ae8c37bbe92fa155259796 $
*/

SECTIONS {
	.kp_init : {
		*(.init)
		__kernelProcessDefs16 = .; /* 16bitsモードの初期化後にエントリが順にcallされる */
		*(.pdef16)
		LONG(0);
	}
	.kp_ro : {
		*(.text*)
		*(.rodata*)
		__ArchCons = .;
		*(.ctors)
		*(SORT(.ctors.*))
		LONG(0);
		__ArchDest = .;
		*(.dtors)
		*(SORT(.dtors.*))
		LONG(0);
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
}
