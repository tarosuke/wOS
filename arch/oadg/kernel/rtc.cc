/************************************************************************* RTC
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <time.h>
#include <debug.h>
#include <cpu/io.h>


static class RTC{
public:
	RTC(){
		const char* nodow[] = { "sun", "mon", "tue", "wed", "thu", "fri", "sat" };
		dputs("RTC..."INDENT);
		do{
			out8(0x70, 10);
		}while(in8(0x71) & 0x80);

		const TIME::DATE baseTime(
			bcd2bin(ReadCMOS(9))
				+ bcd2bin(ReadCMOS(0x32)) * 100,
			bcd2bin(ReadCMOS(8)),
			bcd2bin(ReadCMOS(7)),
			bcd2bin(ReadCMOS(6)),
			bcd2bin(ReadCMOS(4)),
			bcd2bin(ReadCMOS(2)),
			bcd2bin(ReadCMOS(0)),0);

//		CLOCK::SetGlobal(baseTime);
		dprintf("baseTime: %04llu/%02u/%02u(%s) %02u:%02u:%02u.%06u.\n",
			baseTime.year,
			baseTime.month,
			baseTime.date,
			nodow[baseTime.dow],
			baseTime.hour,
			baseTime.min,
			baseTime.sec,
			baseTime.usec);
		dputs(UNINDENT"OK.\n");
	};
private:
	uchar ReadCMOS(uchar addr){
		uchar v;
		asm volatile(
			"mov %1, %%al\n"
			"outb %%al, $0x70\n"
			"jmp 1f\n"
			"1: inb $0x71, %%al\n"
			"jmp 2f\n"
			"2: mov %%al, %0"
			: "=g"(v) : "g"(addr) : "eax");
		return v;
	};
}rtc;
