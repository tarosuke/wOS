/************************************************************ CPU MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <config.h>
#include <cpu/cpu.h>

CPU::TSS CPU::tss[CF_MAX_PROCESSORs]__attribute__((aligned(1024)));


void CPU::SetupTSSDescriptor(u64* e){
	for(uint i(0); i < CF_MAX_PROCESSORs; i++, e++){
//		const munit p((munit)&tss[i]);
	}
}

