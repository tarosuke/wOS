/************************************************************* RAW SYSTEMCALLL
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <userlib/raw.h>

void __RawSystemcall(void){
	asm volatile("int $0x30");
}

