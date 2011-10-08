/********************************************************* ASSEMBLER UTILITIES
	Copyright (C) 2009- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id: 73df46f58963e9dc7115210b9aec4177350994e2 $
*/

#include <config.h>

.code16
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



/********************************************************* GET MEMORY BANKS */
.L_GET_MEMORYBANKS:
#define SMAP 0x534d4150
	mov	$__archMemBanks, %ebx // __archMemBanks is in .kernel_rw
	call	.L_GetKernelLMA
	call	.L_getpos
	mov	%bx, %di
	mov	%ds, %ax
	mov	%ax, %es
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
	mov	$__archMemBanks, %ebx
	call	.L_getpos
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
	mov	%cs, %ax
	movw	%ax, %ds
	movw	%ax, %es
	ret



/********************************************************* 16BITS UTILITIES */
.L_getpos:	push	%bx	/* ebx -> ds:bx */
	shr	$4, %ebx
	mov	%bx, %ds
	pop	%bx
	and	$0x000f, %bx
	ret



/*********************************************************** 16BITS CONSOLE */
#if CF_NULL_CONSOLE
.L_initConsole:
.L_putc16_:
	ret;
#else
#if CF_SERIAL_CONSOLE
.L_initConsole:
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
	ret
.L_putc16_:
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
	ret
#endif

#if CF_CGA_CONSOLE
.L_initConsole:
	ret
.L_putc16_:
	movb	$0x0e, %ah
	movb	$0x0f, %bl
	int	$0x10
	ret
#endif
#endif

.L_putc16:
	cmp	$'\n', %al
	jne	1f
	mov	$'\r', %al
	call	.L_putc16_
	mov	$'\n', %al
1:	jmp	.L_putc16_
.L_puts16:
	cld
1:	lodsb
	testb	%al, %al
	jz	2f
	call	.L_putc16
	jmp	1b
2:	ret
.L_hello:
	mov	$'\n', %al
	call	.L_putc16
	mov	$__version, %ebx
	call	.L_GetKernelLMA
	call	.L_getpos
	mov	%bx, %si
	call	.L_puts16
	mov	$'\n', %al
	call	.L_putc16
	ret
.L_hex16:
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


