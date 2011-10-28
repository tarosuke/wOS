/************************************ LDSCRIPT TO CONVERT USERMODE LIBRARY OBJECTS
	Copyright (C) 2009- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
*/

SECTIONS {
	taskHead = 0;
	.ulib_h : {
		*(.ulibdef*) /* ユーザライブラリの呼び出しテーブル*/
	}
	.ulib_ro : {
		*(.text*)
		*(.rodata*)
	}
}
