/*********************************************************** EXCEPTION HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <cpu/exception.h>


u64 EXCEPTION::vector[systemExceptions + CF_MAX_IRQs];
static EXCEPTION exeptionHandler;

EXCEPTION::EXCEPTION(){
	dputs("exception..." INDENT);
	dputs(UNINDENT "OK.\n");
}

void RegisterIRQ(uint irq, void (*handler)()){
	//TODO:Registerするんじゃなくて、irq番号を作ってINTERRUPT::Handlerを呼ぶ
}

void RegisterFault(uint num, void (*handler)(u32)){
}

