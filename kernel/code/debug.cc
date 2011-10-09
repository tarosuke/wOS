/*************************************************************** DEBUG CONSOLE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <debug.h>
#include <version.h>

#if CF_DEBUG_LEVEL
static void _putc(char c){
	asm volatile(
		"mov	$0x03fd, %%dx;"
		"2: inb	%%dx, %%al;"
		"test	$0x20, %%al;"
		"je	2b;"
		"mov	%%cl, %%al;"
		"mov	$0x03f8, %%dx;"
		"outb	%%al, %%dx"
		:: "c"(c) : "ax", "dx");
}


void dputc(char c){
	if(c == '\n'){
		_putc('\r');
	}
	_putc(c);
}

void dputs(const char* s){
	for(; *s; s++){
		dputc(*s);
	}
}
void dprintf(const char* format, ...){

}
#endif
