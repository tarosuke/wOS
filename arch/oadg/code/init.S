/******************************************* ARCHITECTURE DEPENDED INITIALIZER
	Copyright (C) 2004- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
*/

#include <config.h>
#include <arch/segments.h>

.global __init16, __LMA32, __ARCH_MemoryBlocks, __LMA_TSSPH
.extern __kernelProcessDefs16, __InitConsole16, Init, __LMA32_16, __kProcHeader
.extern __kernelBase_hi, __kernelBase_lo, __kernel_base
.global __hiPageTable, __pageRoot


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
	.asciz	"Try ia32(oadg) kernel not oadg64."
#endif
1:
#if CF_PAE && CF_IA32
	mov	$2, %eax
	cpuid
	test	$0x40, %edx
	jnz	1f
	mov	$3f, %esi
	jmp	2f
3:	.ascii	"FATAL: PAE not available. "
	.asciz	"Try non-PAE kernel(\"#define CF_PAE 0\" in config)."
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
1:	jmp	.L_getMode32


/********************************************************** GET 32bits MODE */
.L_getMode32:
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
1:	.quad	0
	.quad	0x00cf9a000000ffff	// CS(kernel)
	.quad	0x00cf92000000ffff	// DS/ES/FS/GS/SS(kernel)
	.quad	0x00cff8000000ffff	// CS(user)
	.quad	0x00cff2000000ffff	// DS/ES/FS/GS/SS(user)
	.quad	0x00cf9a010000ffff	// TEMPORARY CS FROM 0x10000
#if CF_IA32
__LMA_TSSPH:
	.rept	CF_MAX_PROCESSORs
	.quad	0x000089000000006c	// PLACEHOLDER FOR TSS
	.endr
#endif

	/***** GDT POINTER */
.L_GDTPT:
	.word	GDTLIMIT	// LIMIT OF GDT
	.long	1b		// GDT


	/***** MEMORYBANKs */
__ARCH_MemoryBlocks:
	.rept	CF_MAX_MEMORYBANKs + 1
	.quad	0xffffffffffffffff
	.quad	0xffffffffffffffff
	.long	0xffffffff
	.endr



/************************************* SET SEGMENTS & GET TRUE CODE SEGMENT */
.code32
__LMA32:
	mov	$KDSel, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %fs
	mov	%ax, %gs
	mov	%ax, %ss
	mov	$.L_temporaryStackTop, %esp
	ljmp	$KCSel, $.L_initPages
	.space	4096, 0
.L_temporaryStackTop:



/******************************************************* INITIALIZE PAGEING */
.L_initPages:
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
#endif
#if CF_IA32
#if CF_PAE
	/***** PDTPとPDT3自身にPDT0-3を設定 */
	xor	%edx, %edx
	mov	$.L_PDT3, %edi
	add	$4064, %edi
	mov	$.L_PDT0, %eax
	call	1f
	mov	$.L_PDT1, %eax
	call	1f
	mov	$.L_PDT2, %eax
	call	1f
	mov	$.L_PDT3, %eax
	call	1f
	jmp	2f

1:	push	%eax
	or	$0x201, %eax
	mov	%eax, .L_PDTP(, %edx, 8)
	mov	%eax, (%edi, %edx, 8)
	pop	%eax
	/* ページディレクトリの全エントリをvalidにしておく */
	mov	$512, %ecx
	xor	%ebx, %ebx
3:	orl	$0x200, (%eax, %ebx, 8)
	inc	%ebx
	loop	3b
	inc	%edx
	ret
2:
	/***** PDT3にhiPTを登録 */
	mov	$.L_PDT3, %edx
	mov	$.L_hiPT, %ebx
	or	$0x207, %ebx
	mov	$__kernelBase_lo, %eax
	shr	$21, %eax
	and	$0x1ff, %eax
	mov	%ebx, (%edx, %eax, 8)
#else
	/***** __pageRootの全エントリをvalidに */
	mov	$__pageRoot, %edi
	mov	$0x200, %eax
	mov	$1024, %cx
	rep stosl

	/***** __pageRootにloPT、hiPTを登録 */
	mov	$__pageRoot, %ebx
	mov	$.L_loPT, %eax
	or	$0x207, %eax
	mov	%eax, (%ebx)

	mov	$.L_hiPT, %edx
	or	$0x303, %edx
	mov	$__kernelBase_lo, %eax
	shr	$22, %eax
	mov	%edx, (%ebx, %eax, 4)

	/***** __pageRootにそれ自身を登録 */
	mov	%ebx, %eax
	or	$0x303, %eax
	mov	%eax, 4092(%ebx)
#endif
#endif

#if CF_AMD64 || (CF_IA32 && CF_PAE)
	/***** loPDT(long)／PDT0(PAE)にloPTを設定 */
	mov	$.L_loPT, %eax
	or	$0x207, %eax
	mov	%eax, (.L_loPDT)
#endif

	/***** loPTとhiPTに1MiB以下の物理空間をエイリアス */
	mov	$256, %ecx
	mov	$0x203, %eax
	mov	$0x207, %edx
	mov	$0x1000, %ebx
	mov	$.L_hiPT, %edi
	mov	$.L_loPT, %esi
1:	mov	%eax, (%edi)
	add	%ebx, %eax
#if CF_IA32 && !CF_PAE
	add	$4, %edi
#else
	add	$8, %edi
#endif
	mov	%edx, (%esi)
	add	%ebx, %edx
#if CF_IA32 && !CF_PAE
	add	$4, %esi
#else
	add	$8, %esi
#endif
	loop	1b

	/***** カーネルの下位64KiBをユーザプロセス向けライブラリにする */
	mov	$__ulib_LMA, %eax
	or	$0x305, %eax
	xor	%edx, %edx
	mov	$16, %ecx
#if CF_IA32 && !CF_PAE
1:	mov	%eax, .L_hiPT(, %edx, 4)
#else
1:	mov	%eax, .L_hiPT(, %edx, 8)
#endif
	add	$0x1000, %eax
	inc	%edx
	loop	1b

	/***** TODO:カーネルプロセス空間の最下位4KiBはカーネルプロセスヘッダ */
/*	mov	$__kProcHeader, %eax
	or	$5, %eax
	mov	%eax, (.L_loPT)*/

	/***** ページングをONにする */
	mov	$__pageRoot, %eax
	mov	%eax, %cr3
	mov	%cr4, %eax
#if CF_IA32 && !CF_PAE
	or	$0x00000080, %eax
#else
	or	$0x000000a0, %eax
#endif
	mov	%eax, %cr4
#if CF_AMD64
	mov	$0xc0000080, %ecx
	rdmsr
	bts	$8, %eax
	wrmsr
#endif
	mov	%cr0, %eax
	bts	$31, %eax
	mov	%eax, %cr0

#if CF_IA32
	jmp	.L_initPagedSegments
#endif
#if CF_AMD64
	ljmp	$KCSel, $.L_initPagedSegments
#endif


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
	or	$0x207, %ebx
	mov	%ebx, (%edi, %eax, 8)
	ret


/***** INITIAL PAGETABLE */
	.align 4096
.L_RPDP:
.L_PDTP:
__pageRoot:
	.space 4096
#if CF_PAE || CF_AMD64
.L_hiPDPR:
.L_PDT3:
	.space 4096
.L_hiPDT:
.L_PDT2:
	.space 4096
.L_loPDPR:
.L_PDT1:
	.space 4096
.L_loPDT:
.L_PDT0:
	.space 4096
#endif
.L_hiPT:
__hiPageTable: /* 各階層のページディレクトリを覗く穴を置くためにこれをメモリとしてアクセスできる必要がある */
	.space 4096
.L_loPT:
	.space 4096



/************************************************ INITIALIZE PAGED SEGMENTS */
.L_initPagedSegments:
#if CF_IA32
	/***** カーネルイメージへミラーしたGDTを読み込む */
	mov	$__kernelBase_lo, %eax
	mov	$.L_GDTPT, %edi
	add	%eax, 2(%edi)
	add	%eax, %edi
	lgdt	(%edi)
	mov	$KDSel, %ax
	mov	%ax, %ds
	mov	%ax, %es
	jmp	.L_getKernel
#define xax eax
#define xsp esp
#endif
#if CF_AMD64
	.code64
	/***** カーネルイメージへミラーした64bitsなGDTを読み込む */
	mov	$.L_GDTPT64, %rdi
	push	$16
	push	$.L_temporaryStackTop
	pushf
	push	$KCSel
	push	$1f
	lgdt	(%rdi)
	iret
1:	mov	$KDSel, %ax
	mov	%ax, %ss
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %fs
	mov	%ax, %gs
	/***** ミラーのGDTを設定(内容は同一なので再設定はしない) */
	mov	$__kernel_base, %rax
	add	%rax, 2(%rdi)
	lgdt	(%rdi)
	jmp	.L_getKernel

	.align 4
.L_GDTPT64:
	.word	GDTLIMIT	// LIMIT OF GDT64
	.quad	1f		// GDT64
	.quad	0
	.align 256
1:	.quad	0
	.quad	0x0020980000000000	// CS(kernel)
	.quad	0x00c0920000000000	// DS(kernel)
	.quad	0x0060fa0000000000	// CS(user)
	.quad	0x00c0f20000000000	// DS(user)
	.quad	0x00cffa000000ffff	// CS(user, 32bits)
	.quad	0x00cff2000000ffff	// DS(user, 32bits)
__LMA_TSSPH:
	.space	CF_MAX_PROCESSORs * 16	// PLACEHOLDER FOR TSS
#define xax rax
#define xsp rsp
#endif

/************************************************* 下準備が終わったのでInitへ */
.L_getKernel:
	/***** スタックをカーネルイメージ内へ移動 */
	mov	$__kernel_base, %xax
	add	%xax, %xsp
	jmp	Init
