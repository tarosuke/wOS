/************************************ LDSCRIPT TO CONVERT USERMODE LIBRARY OBJECTS
	Copyright (C) 2009- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id: bf1ef64204a9a73071ae8c37bbe92fa155259796 $
*/

SECTIONS {
	.ulib : {
		*(.data*)
		*(.eh_frame)
		*(.gnu*)
		*(.bss*)
	}
	.ulib_ro : {
		*(.text*)
		*(.rodata*)
	}
}
