/******************************************* ARCHITECTURE DEPENDED INITIALIZER
	Copyright (C) 2004- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id: 3db58d0e79c8b548d0951f5bb51f160af1b1c3c4 $
*/

#include <config.h>
#include <arch/segments.h>

.global __init16, __LMA32, __ARCH_Memoryblocks, __LMA_GDT, __LMA_GDTPT, __LMA_TSSPH
.extern __kernelProcessDefs16, __InitConsole16, __Init32, __LMA32_16, __VMA_GDT



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

	/***** enable A20 */
	mov	$0x2401, %ax
	int	$0x15

	/***** say 'hello' */
	call	__ARCH_SayHello

	/***** get memorybanks */
#define SMAP 0x534d4150
	mov	$__ARCH_Memoryblocks, %edi
	xor	%ebx, %ebx
1:	mov	$0xe820, %eax
	mov	$SMAP, %edx
	mov	$20, %ecx
	int	$0x15
	jc	3f
	cmp	$SMAP, %eax
	jne	3f
	add	$20, %di
	and	%ebx, %ebx
	jnz	1b
	jmp	2f
3:
	mov	$0x88, %ah
	int	$0x15
	jc	2f
	xor	%edx, %edx
	mov	%ax, %dx
	shl	$10, %edx
	push	%edx
	mov	$__ARCH_Memoryblocks, %ebx
	pop	%eax
	movl	$0x100000, (%bx)
	add	$4, %bx
	movl	$0, (%bx)
	add	$4, %bx
	mov	%eax, (%bx)
	add	$4, %bx
	movl	$1, (%bx)
	add	$4, %bx
	movl	$0, (%bx)
2:

	/***** initialize other 16-bits code pointed from __kernelProcessDefs16 array */
	mov	$__kernelProcessDefs16, %esi
1:	lodsl
	test	%eax, %eax
	jz	1f
	call	*%ax
	jmp	1b
1:


/********************************************************** GET 32bits MODE */

	/***** SETUP GDT */
	mov	$.L_GDTPT, %ebx
	lgdt	(%bx)

	/***** ENABLE 32bits MODE */
	mov	%cr0, %eax
	or	$1, %eax
	mov	%eax, %cr0
	.byte	0xea
	.short	__LMA32_16
	.short	TCSel

	/***** TEMPORARY GDT */
.align 256
__LMA_GDT:
	.quad	0
	.quad	0x00cf9a000000ffff	// CS(kernel)
	.quad	0x00cf92000000ffff	// DS/ES/FS/GS/SS(kernel)
	.quad	0x00c3f8000000ffff	// CS(user:0-1G)
	.quad	0x00cff2000000ffff	// DS/ES/FS/GS/SS(user)
	.quad	0x00cf9a010000ffff	// TEMPORARY CS FROM 0x10000
__LMA_TSSPH:
	.rept	CF_MAX_PROCESSORs
	.quad	0x000089000000006c	// PLACEHOLDER FOR TSS
	.endr

	/***** GDT POINTER */
.L_GDTPT:
	.word	GDTLIMIT	// LIMIT OF GDT
	.long	__LMA_GDT	// GDT
__LMA_GDTPT:
	.word	GDTLIMIT	// LIMIT OF GDT
	.long	__VMA_GDT	// GDT(upper mirror)


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
	mov	$KDSel, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %fs
	mov	%ax, %gs
	mov	%ax, %ss
	mov	$1f, %esp
	ljmp	$KCSel, $__Init32
	.space	4096, 0
1:


