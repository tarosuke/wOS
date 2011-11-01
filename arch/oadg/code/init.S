/******************************************* ARCHITECTURE DEPENDED INITIALIZER
	Copyright (C) 2004- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
*/

#include <config.h>
#include <arch/segments.h>

.global __init16, __LMA32, __ARCH_MemoryBlocks, __LMA_GDT, __LMA_GDTPT, __LMA_TSSPH
.extern __kernelProcessDefs16, __InitConsole16, Init, __LMA32_16
#ifdef CF_AMD64
.global __hiPageTable, __pageRoot
.extern __pageRoot, __kernelBase_hi, __kernelBase_lo
#endif
#ifndef CF_IA32
.extern __VMA_GDT
#endif


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

	/***** system check */
#if CF_AMD64
	mov	$0x80000000, %eax
	cpuid
	cmp	$0x80000000, %eax
	jbe	2f
	mov	$0x80000001, %eax
	cpuid
	bt	$29, %edx
	jc	1f
2:	mov	$3f, %esi
	jmp	2f
3:	.ascii	"FATAL: 64bits mode not available. "
	.asciz	"Use ia32(oadg) kernel."
#endif
1:
#if CF_PAE && !CF_AMD64
	mov	$2, %eax
	cpuid
	test	$0x40, %edx
	jnz	1f
	mov	$3f, %esi
	jmp	2f
3:	.ascii	"FATAL: PAE not available. "
	.asciz	"Use non-PAE kernel."
#endif
1:	jmp	1f

	/* fails */
2:	mov	$'\n', %al
	call	__putc16
	call	__puts16
3:	hlt
	jmp	3b

1:

	/***** get memorybanks */
#define SMAP 0x534d4150
	mov	$__ARCH_MemoryBlocks, %edi
	xor	%ebx, %ebx
1:	mov	$0xe820, %eax
	mov	$SMAP, %edx
	mov	$20, %ecx
	int	$0x15
	jc	3f
	cmp	$SMAP, %eax
	jne	3f
	mov	4(%di),%eax
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
	mov	$__ARCH_MemoryBlocks, %ebx
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
	test	%ax, %ax
	jz	1f
	push	%si
	call	*%ax
	pop	%si
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
	.quad	0x00cff8000000ffff	// CS(user)
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
#if CF_IA32
	.word	GDTLIMIT	// LIMIT OF GDT
	.long	__VMA_GDT	// GDT(upper mirror)
#endif


	/***** MEMORYBANKs */
__ARCH_MemoryBlocks:
	.rept	CF_MAX_MEMORYBANKs + 1
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
	mov	$9f, %esp
#if CF_IA32
	ljmp	$KCSel, $Init
#endif

/********************************************** GET 64BITS MODE INITIALIZER */
#if CF_AMD64
	/***** RPDPにPDPRを登録 */
	mov	$.L_RPDP, %edi
	mov	$.L_loPDPR, %eax
	or	$0x207, %eax
	mov	%eax, (%edi)
	mov	$.L_hiPDPR, %ebx
	mov	$39, %ecx
	call	.L_SetKernelBase

	/***** hiPDPRにhiPDTを登録 */
	mov	$.L_hiPDPR, %edi
	mov	$.L_hiPDT, %ebx
	mov	$30, %ecx
	call	.L_SetKernelBase

	/***** hiPDTにhiPTを登録 */
	mov	$.L_hiPDT, %edi
	mov	$.L_hiPT, %ebx
	mov	$21, %ecx
	call	.L_SetKernelBase

	/***** loPDPRにloPDTを設定 */
	mov	$.L_loPDT, %eax
	or	$0x207, %eax
	mov	%eax, (.L_loPDPR)

	/***** loPDTにloPTを設定 */
	mov	$.L_loPT, %eax
	or	$0x207, %eax
	mov	%eax, (.L_loPDT)

	/***** loPTとhiPTに1MiB以下の物理空間をエイリアス */
	mov	$256, %ecx
	mov	$0x203, %eax
	mov	$0x207, %edx
	mov	$0x1000, %ebx
	mov	$.L_hiPT, %edi
	mov	$.L_loPT, %esi
1:	mov	%eax, (%edi)
	add	%ebx, %eax
	add	$8, %edi
	mov	%edx, (%esi)
	add	%ebx, %edx
	add	$8, %esi
	loop	1b

	/***** いよいよ64bitsモードをONにする */
	mov	$__pageRoot, %eax
	mov	%eax, %cr3
	mov	%cr4, %eax
	or	$0x000000a0, %eax
	mov	%eax, %cr4
	mov	$0xc0000080, %ecx
	rdmsr
	bts	$8, %eax
	wrmsr
	mov	%cr0, %eax
	bts	$31, %eax
	mov	%eax, %cr0

	/***** TODO:64bitsセグメントの準備 */

	/***** 下準備が終わったのでInitへ */

mov $'*', %al
mov $0x3f8, %dx
out %al, %dx

	hlt


.L_SetKernelBase:
	/* edxのテーブルにebxのアドレスを設定。ビット位置はecx */
	mov	$__kernelBase_hi, %edx
	mov	$__kernelBase_lo, %eax
1:	/* edx:eaxをecx回右シフト */
	clc
	rcr	$1, %edx
	rcr	$1, %eax
	loop	1b
	and	$0x1ff, %eax
	or	$0x203, %ebx
	mov	%ebx, (%edi, %eax, 8)
	ret



/***** INITIAL PAGETABLE */
.align 4096
.L_RPDP:
__pageRoot:
	.rept 512
	.quad	0
	.endr
.L_hiPDPR:
	.rept 512
	.quad	0
	.endr
.L_hiPDT:
	.rept 512
	.quad	0
	.endr
.L_hiPT:
__hiPageTable: /* 各階層のページディレクトリを覗く穴を置くためにこれをメモリとしてアクセスできる必要がある */
	.rept 512
	.quad	0
	.endr
.L_loPDPR:
	.rept 512
	.quad	0
	.endr
.L_loPDT:
	.rept 512
	.quad	0
	.endr
.L_loPT:
	.rept 512
	.quad	0
	.endr
#endif

	.space	4096, 0
9:


