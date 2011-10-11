/****************************************** LDSCRIPT TO CONVERT KERNEL OBJECTS
	Copyright (C) 2009- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id: e881070b72bda47c58026e678deef2ab91c98fba $
*/

SECTIONS {
	.kernel_ro : {
		*(.text*)
		. = ALIGN(4);
		__KernelConstructor = .;
		*(.ctors)
		*(SORT(.ctors.*))
		LONG(0);
		__KernelDestructor = .;
		*(.dtors)
		*(SORT(.dtors.*))
		LONG(0);
		*(.rodata*)
	}
	.kernel_rw : {
		*(.data*)
		*(.eh_frame)
		*(.gnu*)
		*(.bss*)
	}
	.kp_ro : {
		*(.kproc_ro)
	}
	.kp_rw : {
		*(.kproc_rw)
	}
}
