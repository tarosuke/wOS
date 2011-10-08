/******************************************* ARCHITECTURE DEPENDED INITIALIZER
	Copyright (C) 2004- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id: 3db58d0e79c8b548d0951f5bb51f160af1b1c3c4 $
*/

#include <config.h>

.global __init16, __LMA32, __ARCH_Memoryblocks
.extern __kernelProcessDefs16, __InitConsole16, __Init32


/******************************************************** SEGMENT SELECTORS */
#define KCSel	8
#define	TCSel	(KCSel + 8)



/********************************************** PREPAREING TO BOOTSTRAPS UP */
.code16
.section .init
__init16:
	/***** LOCK THE SYSTEM */
	cli

	/***** SETUP SEGMENTS */
	movw	%cs, %ax
	movw	%ax, %ds
	movw	%ax, %es

	/***** SETUP STACK16 */
	xor	%ax, %ax
	movw	%ax, %ss
	movw	%ax, %sp

	/***** initialize console */
	call	.L_initConsole

	/***** enable A20 */
	mov	$0x2401, %ax
	int	$0x15

	/***** say 'hello' */
	call	.L_hello

	/***** get memorybanks */
	call	.L_GET_MEMORYBANKS



/********************************************************** GET 32bits MODE */

	/***** SETUP GDT */
	lea	($.L_GDTPT), %bx
	lgdt	(%bx)

	/***** ENABLE 32bits MODE */
	mov	%cr0, %eax
	or	$1, %eax
	mov	%eax, %cr0
	.byte	0xea
	.short	__LMA32_16
	.short	TCSel

	/***** TEMPORARY GDT */
.align 8
.L_GDT:	.quad	0
	.quad	0x00cf9a000000ffff	// CS(kernel)
	.quad	0x00cf9a010000ffff	// TEMPORARY CS FROM 0x10000

	/***** GDT POINTER */
.L_GDTPT:	.word	8 * 3		// LIMIT OF GDT
.L_GDTPTV:	.long	.L_GDT		// GDT



	/***** MEMORYBANKs */
__ARCH_Memoryblocks:
	.rept	CF_MAX_MEMORYBLOCKs + 1
	.quad	0xffffffffffffffff
	.quad	0xffffffffffffffff
	.long	0xffffffff
	.endr






/********************************************** GET 32BITS MODE INITIALIZER */
.code32
__LMA32:
	ljmp	$KCSel, $Init32






















/** これ以降は別のファイルで */
#if 0
.L_init32:
	/* SETUP SEGMENTS */
	mov	$KDSel, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %fs
	mov	%ax, %gs
	mov	%ax, %ss

	/* SETUP STACK */
	mov	$0x10000, %esp

	/* INITIALIZE PAGETABLE */
	mov	$__archMasterPageDirectory, %eax
	mov	%eax, %cr3

	/* ENABLE PAGING */
	mov	%cr4, %eax
	or	$0x00000080, %eax
	mov	%eax, %cr4
	mov	%cr0, %eax
	or	$0x80000000, %eax
	mov	%eax, %cr0

	/* GET KERNELSPACE */
	jmp	.L_boot



.section .inittable
__archInitialPageTable:
	.macro	PageDir f t
	.long	0x00000103 + (\f << 12)
	.if	\t - \f
	PageDir	"(\f+1)", \t
	.endif
	.endm

	PageDir	0, 63
	PageDir	64, 127
	PageDir	128, 191
	PageDir	192, 255
	.space	3072, 0




/************************************************************** KERNELSPACE */
.section .kernel_ro
SUNKDOWN_ENTRY:
	ret
	jmp	1f
.section .init
1:	// LOAD low GDT
	mov	$.L_GDTPT, %eax
	subl	$__kernel_base, (.L_GDTPTV)
	lgdt	(%eax)

	// DISABLE PAGING
	mov	%cr0, %eax
	and	$~0x80000000, %eax
	mov	%eax, %cr0
	xor	%eax, %eax
	mov	%eax, %cr3

	// DISABLE 32BITS MODE
	mov	%cr0, %eax
	and	$~1, %eax
	mov	%eax, %cr0
	.byte	0xea
	.long	__SUNKED_DOWN16
	.short	0x1000

.code16
SUNKED_DOWN:
	/** RE-SETUP SEGMENTs */
	movw	%cs, %ax
	movw	%ax, %ds
	movw	%ax, %es

	/** RE-SETUP STACK16 */
	xor	%ax, %ax
	movw	%ax, %ss
	movw	%ax, %sp

	/**/
	mov	$0xb800, %ax
	mov	%ax, %es
	mov	$'@', %al
	mov	$0x03f8, %dx
1:	outb	%al, %dx
jmp 1b
	movb	%al, %es:(0)

	hlt







.code32
.section .kernel_ro
#define numOfFaults	32
#define numOfIRQs	16
#define MASTER_PIC	0x20
#define SLAVE_PIC	0xa0
.L_boot:
	/* SETUP GDT FOR VMA*/
	mov	$.L_GDTPT, %eax
	addl	$__kernel_base, (.L_GDTPTV)
	lgdt	(%eax)

	/* RE-SETUP SEGMENTS */
	mov	$KDSel, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %fs
	mov	%ax, %gs
	mov	%ax, %ss

	/* SETUP STACK FOR VMA */
	mov	(.L_STACK), %esp
	subl	$1024, (.L_STACK)

	/* SETUP TSS SEGMENTs */
	mov	$__cpu_tss, %eax
	mov	%eax, %edx
	and	$0x00ffffff, %eax
	or	%eax, (.L_TSSS + 2)
	and	$0xff000000, %edx
	or	%edx, (.L_TSSS + 4)

	/* MAKE IT THE TASK */
	mov	(.L_TSS), %ax
	ltr	%ax
	addw	$8, (.L_TSS)

	/* SETUP IDT */
	mov	$.L_interruptVector, %ebx
	mov	$.L_Except, %eax
	mov	$(numOfFaults + numOfIRQs), %ecx
1:	call	.L_STORE_HANDLER
	loop	1b
	mov	$.L_SYSTEMCALL, %eax
	call	.L_STORE_HANDLER
	lidt	(.L_vectorPosition)

	/* UNLOCK INITIALIZER */
	movb	$0, (__arch_initlock_kernel)

	/* BRANCH BSP/AP */
1:	jmp	2f
	.long	0x90909090
	jmp	AP_KERNEL_ENTRY
2:	movl	$0x90909090, (1b)

	/* SETUP NMI */
	mov	$__init16, %eax
	mov	%ax, (__kernel_base + 16)
	movw	$0x1000, (__kernel_base + 18)

	/* IT's THE SHOWTIME! */
	jmp	KERNEL_ENTRY

	/* store handler */
.L_STORE_HANDLER:
	mov	%ax, (%ebx)
	mov	%eax, %edx
	shr	$16, %edx
	mov	%dx, 6(%ebx)
	add	$16, %eax
	add	$8, %ebx
	ret

	/* VALUE for EACH PROCESSORS */
.L_STACK: .long	__kernel_stack
.L_TSS:	.word	TSSSel


	/* systemcall jumpPad */
.L_SYSTEMCALL:
	push	%ds
	push	%es
	mov	$KDSel, %dx
	mov	%dx, %ds
	mov	%dx, %es
	push	%ebx
	push	%eax
	call	SYSTEMCALL
	add	$8, %esp
	pop	%es
	pop	%ds
	iret



.L_jumpPadEdge:
	push	%ds
	push	%es
	mov	$KDSel, %dx
	mov	%dx, %ds
	mov	%dx, %es
	push	%eax
	//
	mov	%al, %ah
	mov	$0x20, %al
	cmp	$0x20, %ah
	jb	3f
	cmp	$0x28, %ah
	jb	1f
	// SLAVE EOI
	mov	$SLAVE_PIC, %dx
	out	%al, %dx
	// MASTER EOI
1:	mov	$MASTER_PIC, %dx
	out	%al, %dx
	// CALL HANDLER
	call	InterruptHandler
	pop	%eax
	// GET BACK
3:	pop	%es
	pop	%ds
	popal
	iret
.L_faultPad:
	mov	%esp, %ebp
	push	%ds
	push	%es
	mov	$KDSel, %dx
	mov	%dx, %ds
	mov	%dx, %es
	push	40(%ebp)
	push	%eax
	call	CPU_FaultHandler
	add	$8, %esp
	pop	%es
	pop	%ds
	popal
	add	$4, %esp
	iret
	.align 16
.L_Except:
	.macro	jumpers f t
	.align	16, 0x90
	pushal
	mov	$\f, %eax
	.if	(numOfFaults <= \f)
	/***** NORMAL HANDLER */
	jmp	.L_jumpPadEdge
	.else
	/***** FAULT HANDLER */
	jmp	.L_faultPad
	.endif
	.if	\t - \f
	jumpers	"(\f+1)", \t
	.endif
	.endm
	jumpers	0, (numOfFaults + numOfIRQs)


/********************************************************* EXCEPTION VECTOR */
.section .kernel_ro
	.align 16
.L_interruptVector:
	.macro Vector f t
	.long	KCSel << 16
	.long	0x00008e00
	.if	\t - \f
	Vector	"(\f + 1)", \t
	.endif
	.endm
	Vector	0, (32 + 16 - 1)
	.long	KCSel << 16
	.long	0x0000ef00
.L_vectorPosition:
	.word	511
	.long	.L_interruptVector






#if 0
	/* SETUP SYSENTER_XX_MSRs */
	xor	%edx, %edx
	mov	$0x174, %ecx	/* SYSENTER_CS_MSR */
	mov	$KCSel, %eax
	wrmsr
	inc	%ecx		/* SYSENTER_ESP_MSR(0x175) */
	mov	$ARCH_KERNEL_BASE + 0x20000, %eax /* tempsrary stack */
	wrmsr
	inc	%ecx		/* SYSENTER_EIP_MSR(0x176) */
	mov	$SYSENTER_ENTRY, %eax
	wrmsr

	/***** START KERNELCORE */
	call	Entry

	cli
1:	hlt
	jmp	1b

SYSENTER_ENTRY:
	jmp	1b
#endif

#endif
