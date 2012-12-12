/**************************************************** PRIMER EXCEPTION HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <config.h>

#if CF_AMD64

#include <arch/segments.h>

.global __ExceptionTable
.extern __INTERRUPT_Handler, __FAULT_Handler, __SYSTEMCALL_HANDLER


.code64

.text
/***** フォルト専用ハンドラ(フラグを引数に) */
/* エラーコード付き(8〜14,17) */
.L_faultPad_withErrorcode:
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
	mov	%rsp, %rdx
	mov	120(%rsp), %rsi
	mov	%ds, %ax
	push	%ax
	mov	%es, %ax
	push	%ax
	mov	$KDSel, %ax
	mov	%ax, %ds
	mov	%ax, %es
	call	__FAULT_Handler
	pop	%ax
	mov	%ax, %es
	pop	%ax
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
	iretq
/* エラーコードなし */
.L_faultPad_withoutErrorcode:
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
	mov	%rsp, %rdx
	mov	120(%rsp), %rsi
	mov	%ds, %ax
	push	%ax
	mov	%es, %ax
	push	%ax
	mov	$KDSel, %ax
	mov	%ax, %ds
	mov	%ax, %es
	call	__FAULT_Handler
	pop	%ax
	mov	%ax, %es
	pop	%ax
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
	iretq

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
	mov	128(%rsp), %rsi	//dispatchable引数
	and	$3, %rsi
	mov	%ds, %ax		//セグメント保存
	push	%ax
	mov	%es, %ax
	push	%ax
	mov	$KDSel, %ax
	mov	%ax, %ds
	mov	%ax, %es
	sub	$CF_ARCH_BASE_VECTOR, %rdi //ベクタ番号を割り込み用に修正
	call	__INTERRUPT_Handler
	pop	%ax
	mov	%ax, %es
	pop	%ax
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
	iretq

/***** システムコールハンドラ */
.L_SYSTEMCALL:
	call	__SYSTEMCALL_HANDLER
	iretq

/***** 割り込みハンドラ(個別部) */
	.align	16, 0x90
__ExceptionTable:
	.macro	jumpers f t
	.align	16, 0x90
	push	%rdi
	mov	$\f, %rdi
	.if	(CF_ARCH_SYSTEMCALL_VECTOR == \f)
	/***** SYSTEMCALL */
	jmp	.L_SYSTEMCALL
	.elseif	(CF_ARCH_BASE_VECTOR <= \f)
	/***** NORMAL HANDLER */
	jmp	.L_jumpPad
	.else
	/***** FAULT HANDLER */
	.if	(8 <= \f && \f <= 14) || (17 == \f)
	jmp	.L_faultPad_withErrorcode
	.else
	jmp	.L_faultPad_withoutErrorcode
	.endif
	.endif
	.if	\t - \f
	jumpers	"(\f+1)", \t
	.endif
	.endm

	jumpers	0, (CF_ARCH_BASE_VECTOR + CF_ARCH_MAX_INTERRUPTs)
#endif
