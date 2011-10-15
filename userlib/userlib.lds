/************************************ LDSCRIPT TO CONVERT USERMODE LIBRARY OBJECTS
	Copyright (C) 2009- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id$
*/

SECTIONS {
	.ulib_ro : {
		*(.text*)
		*(.rodata*)
	}
	.ulib_rw : {
		*(.data*)
		*(.eh_frame)
		*(.gnu*)
		*(.bss*)
	}
}
