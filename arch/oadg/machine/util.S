/********************************************************* ASSEMBLER UTILITIES
	Copyright (C) 2009- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id: 73df46f58963e9dc7115210b9aec4177350994e2 $
*/

#include <config.h>

.code16

.global __ARCH_SayHello, __puts16, __hex16

.section .init

/*********************************************** CONVERT VMA(in EBX) TO LMA */
.L_GetUserLMA:
	sub	$__user_VMA, %ebx
	add	$__user_LMA, %ebx
	ret
.L_GetKernelLMA:
	sub	$__kernel_VMA, %ebx
	add	$__kernel_LMA, %ebx
	ret
.L_UserToKernel:
	sub	$__user_VMA, %ebx
	add	$__user_LMA, %ebx
	sub	$__kernel_LMA, %ebx
	add	$__kernel_VMA, %ebx
	ret






/*********************************************************** 16BITS CONSOLE */
.L_putc16_:
#if CF_DEBUG_LEVEL
	push	%dx
	push	%ax
	mov	$0x03fd, %dx
2:	inb	%dx, %al
	test	$0x20, %al
	je	2b
	pop	%ax
	mov	$0x03f8, %dx
	outb	%al, %dx
	pop	%dx
#endif
	movb	$0x0e, %ah
	movb	$0x0f, %bl
	int	$0x10
	ret

.L_putc16:
	cmp	$'\n', %al
	jne	1f
	mov	$'\r', %al
	call	.L_putc16_
	mov	$'\n', %al
1:	jmp	.L_putc16_
__puts16:
	cld
1:	lodsb
	testb	%al, %al
	jz	2f
	call	.L_putc16
	jmp	1b
2:	ret
__hex16:
	mov	%eax, %edx
	mov	$8, %cx
1:	rol	%edx
	rol	%edx
	rol	%edx
	rol	%edx
	mov	%dl, %al
	and	$0x0f, %al
	add	$0x30, %al
	cmp	$0x39, %al
	jbe	2f
	add	$7, %al
2:	call	.L_putc16
	loop	1b
	ret
__ARCH_SayHello: //端末の初期化と初期メッセージの出力。もしデバグモードなら両方に出力
	//初期化
#if CF_DEBUG_LEVEL
	mov	$0x03fa, %dx
	mov	$0x07, %al
	outb	%al, %dx
	mov	$0xfb, %dl
	mov	$0x03, %al
	outb	%al, %dx
	mov	$0xf9, %dl
	mov	$0x00, %al
	outb	%al, %dx
	mov	$0xfb, %dx
	mov	$0x83, %al
	outb	%al, %dx
	mov	$0xf8, %dx
	mov	$0x01, %al
	outb	%al, %dx
	mov	$0xfb, %dx
	mov	$0x03, %al
	outb	%al, %dx
	mov	$0xf9, %dl
	mov	$0x00, %al
	outb	%al, %dx
#endif
	//初期メッセージの出力
	lea	(1f), %si
	call	__puts16
	ret
1:	.asciz	"welcome to talos.\n"

