/**************************************************************** SYSTEM TIMER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */


#include <interrupt.h>
#include <clock.h>
#include <cpu/io.h>
#include <arch/pic.h>
#include <debug.h>


static class TIMER{
private:
	static const tunit timerSource = 1190000ULL;
	static const uint timerCount = timerSource / CF_HZ;
	static const uint timerIRQ = 0;
	static const u16 counterP = 0;
	static const u16 controlP = 3;
	static SBIO<0x40> io;
	static void Tick();
public:
	TIMER(){
		dputs("timer..." INDENT);
		dprintf("timer cycle %d[Hz]/%d count.\n", CF_HZ, timerCount);

		///// re-initialize timer
		io.out8(controlP, 0x34);
		io.out8(counterP, timerCount & 0xff);
		io.out8(counterP, timerCount >> 8);

		///// register the handler
		INTERRUPT::RegisterHandler(timerIRQ, Tick);

		dputs(UNINDENT "OK.\n");
	};
}timer;

void TIMER::Tick(){
	PIC::EOI(timerIRQ);
	CLOCK::Tick();
};
