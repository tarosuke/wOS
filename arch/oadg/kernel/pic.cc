/************************************************************ PIC MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */


#include <arch/pic.h>
#include <debug.h>



static PIC pic;
void (*PIC::vector[irqBaseVector + numOfIrq])();


PIC::PIC() : irqMask(0xfffb){
	dputs("PICs...");
	/***** RE-INITIALIZE PIC
	 *	  PICのICW2に8なんて設定する事にした奴にはこれを32に
	 *	  するのと8にするのとで何がどう違うのかと小一時間(ry */

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
}
























// TODO:PICの初期化と割り込みデスクリプタの初期化、それからベクタの受け口もここ
// TODO:カーネルサイドで割り込みメッセージを発信できるようにする

