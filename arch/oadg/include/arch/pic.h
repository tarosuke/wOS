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
	static SBIO<0x20> mpic;
	static SBIO<0xa0> spic;
	static SBIO<0x4d0> elcr;
	static uint irqMask;
	static const uint levels;
	static const uint irqBaseVector = CF_ARCH_BASE_VECTOR;
	static const uint numOfIrq = 9;
	static inline void UpdateMask(uint irq){
		if(irq < 8){
			mpic.out8(1, irqMask & 255);
		}else{
			spic.out8(1, irqMask >> 8);
		}
	};
public:
	PIC(){
		dputs("pics...");

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
	// 対象割り込みマスク、割り込み許可、エッジトリガならEOI発行
	static void Start(uint irq){
		if(CF_MAX_IRQs <= irq){
			assert(false);
			return;
		}
		const uint bit(1U << irq);
		if(~levels & bit){
			// エッジトリガなので処理前EOI発行
			EOI(irq);
		}
		irqMask |= bit;
		UpdateMask(irq);
	};
	// 対象割り込みマスク解除、レベルトリガならEOI発行
	static void Finish(uint irq){
		if(CF_MAX_IRQs <= irq){
			assert(false);
			return;
		}
		const uint bit(1U << irq);
		irqMask &= ~bit;
		UpdateMask(irq);
		if(levels & bit){
			// レベルトリガなので処理終了後EOI発行
			EOI(irq);
		}
	};
	static inline void EOI(uint irq){
		if(CF_MAX_IRQs <= irq){
			return;
		}
		if(irq < 8){
			mpic.out8(0, 0x20);
		}else{
			spic.out8(0, 0x20);
		}
	};
	static void Mask(uint irq){
		if(CF_MAX_IRQs <= irq){
			return;
		}
		irqMask |= (1U << irq);
		UpdateMask(irq);
	};
	static void Unmask(uint irq){
		if(CF_MAX_IRQs <= irq){
			return;
		}
		irqMask &= ~(1U << irq);
		UpdateMask(irq);
	};
};


#endif
