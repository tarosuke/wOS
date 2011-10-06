/************************************ LDSCRIPT TO CONVERT USERMODE LIBRARY OBJECTS
	Copyright (C) 2009- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id: bf1ef64204a9a73071ae8c37bbe92fa155259796 $
*/

SECTIONS {
	.kp_init : {
		*(.init)
	}
	.kp_def : {
		__kernelProcessDefs16 = .; /* 16bitsモードの初期化後にエントリが順にcallされる */
		*(.kpdef16)
		LONG(0);
		__kernelProcessDefs32 = .; /* 32bitsモードの初期化後にエントリがサーバとして登録される */
		*(.kpdef32)
		LONG(0);
	}
	.kp_ro : {
		*(.text*)
		*(.rodata*)
	}
	.kp_rw : {
		*(.data*)
		*(.eh_frame)
		*(.gnu*)
		*(.bss*)
	}
}
