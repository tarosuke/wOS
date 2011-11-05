/**************************************************** PRIMER EXCEPTION HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#include <config.h>
#include <arch/segments.h>

.global __ExceptionTable
.extern __INTERRUPT_Handler, __FAULT_Handler

#define ARCH_BASE_VECTOR 32

.code64

.text
/***** フォルト専用ハンドラ(フラグを引数に) */
.L_faultPad:
	push	%rax
	push	%rbx
	push	%rcx
	push	%rdx
	push	%rsi
	push	%rbp
	push	%r8
	push	%r9
	push	%r10
	push	%r11
	push	%r12
	push	%r13
	push	%r14
	push	%r15
	mov	%rsp, %rbp
	mov	$0, %dx
	mov	%dx, %ds
	mov	%dx, %es
	mov	120(%rbp), %rsi
	call	__FAULT_Handler
	/****** 終了 */
	mov	$UDSel, %ax
	mov	%ax, %es
	mov	%ax, %ds
	pop	%r15
	pop	%r14
	pop	%r13
	pop	%r12
	pop	%r11
	pop	%r10
	pop	%r9
	pop	%r8
	pop	%rbp
	pop	%rsi
	pop	%rdx
	pop	%rcx
	pop	%rbx
	pop	%rax
	pop	%rdi
	add	$8, %rsp
	iret

/***** 割り込みハンドラ(共通部) */
.L_jumpPad:
	push	%rax
	push	%rbx
	push	%rcx
	push	%rdx
	push	%rsi
	push	%rbp
	push	%r8
	push	%r9
	push	%r10
	push	%r11
	push	%r12
	push	%r13
	push	%r14
	push	%r15
	mov	$0, %dx
	mov	%dx, %ds
	mov	%dx, %es
	sub	$ARCH_BASE_VECTOR, %rax
	/***** ハンドラ実行 */
	call	__INTERRUPT_Handler
	add	$8, %rsp
	/***** 終了 */
	mov	$UDSel, %ax
	mov	%ax, %es
	mov	%ax, %ds
	pop	%r15
	pop	%r14
	pop	%r13
	pop	%r12
	pop	%r11
	pop	%r10
	pop	%r9
	pop	%r8
	pop	%rbp
	pop	%rsi
	pop	%rdx
	pop	%rcx
	pop	%rbx
	pop	%rax
	pop	%rdi
	iret

/***** 割り込みハンドラ(個別部) */
	.align	16, 0x90
__ExceptionTable:
	.macro	jumpers f t
	.align	16, 0x90
	push	%rdi
	mov	$\f, %rdi
	.if	(ARCH_BASE_VECTOR <= \f)
	/***** NORMAL HANDLER */
	jmp	.L_jumpPad
	.else
	/***** FAULT HANDLER */
	jmp	.L_faultPad
	.endif
	.if	\t - \f
	jumpers	"(\f+1)", \t
	.endif
	.endm

	jumpers	0, (ARCH_BASE_VECTOR + CF_MAX_IRQs)

