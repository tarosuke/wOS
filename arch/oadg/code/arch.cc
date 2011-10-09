/************************************************************ PLATFORM SERVERS
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */



extern "C"{
	void __Init32(void)__attribute__((noreturn));
	void __Init32(void){
		asm volatile(
			"mov $0x3f8, %%dx;"
			"mov $'*', %%al;"
			"outb %%al, %%dx" ::: "al", "dx");
		for(;;);
	};
}

