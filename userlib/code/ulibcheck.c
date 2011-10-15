/******************************************************** user library checker
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */


void CheckUserLib(void){
	asm volatile("int $0x30");
}


