/**************************************************** PRIMER EXCEPTION HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <config.h>

#if CF_IA32

#include <arch/segments.h>

.global __ExceptionTable
.extern __INTERRUPT_Handler, __FAULT_Handler, __SYSTEMCALL_HANDLER


.code32

.text
/***** フォルト専用ハンドラ(フラグを引数に) */
/* エラーコード付き(8〜14,17) */
.L_faultPad_withErrorcode:
	mov	%esp, %ebp
	push	%ds
	push	%es
	mov	$KDSel, %dx
	mov	%dx, %ds
	mov	%dx, %es
	push	%esp
	push	40(%ebp)
	push	%eax
	call	__FAULT_Handler
	add	$8, %esp
	/****** 終了 */
	pop	%es
	pop	%ds
	popal
	add	$4, %esp
	iret
/* エラーコードなし */
.L_faultPad_withoutErrorcode:
	mov	%esp, %ebp
	push	%ds
	push	%es
	mov	$KDSel, %dx
	mov	%dx, %ds
	mov	%dx, %es
	push	%esp
	push	40(%ebp)
	push	%eax
	call	__FAULT_Handler
	add	$8, %esp
	/****** 終了 */
	pop	%es
	pop	%ds
	popal
	iret

/***** 割り込みハンドラ(共通部) */
.L_jumpPad:
	push	%ds
	push	%es
	mov	%ds, %dx
	and	$3, %edx
	push	%edx
	mov	$KDSel, %dx
	mov	%dx, %ds
	mov	%dx, %es
	sub	$CF_ARCH_BASE_VECTOR, %eax
	push	%eax
	/***** ハンドラ実行 */
	call	__INTERRUPT_Handler
	pop	%eax
	pop	%eax
	/***** 終了 */
3:	pop	%es
	pop	%ds
	popal
	iret

/***** システムコールハンドラ */
.L_SYSTEMCALL:
	push	%edi
	call	__SYSTEMCALL_HANDLER
	pop	%edi
	iret

/***** 割り込みハンドラ(個別部) */
	.align	16
__ExceptionTable:
	.macro	jumpers f t
	.align	16, 0x90
	pushal
	mov	$\f, %eax
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
