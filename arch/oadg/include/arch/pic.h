/************************************************************ PIC MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#ifndef _ARCH_PIC_
#define _ARCH_PIC_

#include <config.h>
#include <cpu/io.h>
#include <debug.h>

//TODO:APIC対応
class PIC{
private:
	SBIO<0x20> mpic;
	SBIO<0xa0> spic;
	uint irqMask;
	static const uint irqBaseVector = 0x20;
	static const uint numOfIrq = 9;
public:
	PIC() : irqMask(0xfffb){
		dputs("PICs...");

		/* MASTER EDGE-TRIGER, BUFFER, 0x20-0x27 */
		mpic.out8(0, 0x11);
		mpic.out8(1, irqBaseVector);
		mpic.out8(1, 0x04);
		mpic.out8(1, 0x01);

		/* SLAVE EDGE-TRIGGER, BUFFER, 0x28-0x2f */
		spic.out8(0, 0x11);
		spic.out8(1, irqBaseVector + 8);
		spic.out8(1, 0x02);
		spic.out8(1, 0x01);

		// setup masks
		mpic.out8(1, irqMask & 255);
		spic.out8(1, irqMask >> 8);

		dputs("OK.\n");
	};
	static void Start(uint irq); // 対象割り込みマスク、割り込み許可、エッジトリガならEOI発行
	static void Finish(uint irq); // 対象割り込みマスク解除、レベルトリガならEOI発行
};


#endif
