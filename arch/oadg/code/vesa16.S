
/************************************************************ VESA-VBE初期化 */
#include <config.h>

.code16

.global __VESA_InfoBlock, __VESA_displayInfo
.extern __putc16, __puts16, __hex16

.section .pdef16
	.long	.L_Init

.section .init16
.L_Init:
#if !CF_FORCE_VESA_MODE
	// 有効モードの取得
	mov	%cs, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	$__VESA_InfoBlock, %edi
	mov	$0x4f00, %ax
	int	$0x10
	cmp	$0x004f, %ax
	jne	.L_failed

	// 画面情報を取得して有効モードの最大値をbxに取得
	xor	%edx, %edx
	push	%ds
	mov	$.L_videoModes, %ebx
	test	%ebx, %ebx
	jz	4f
	mov	(%bx), %si
	mov	2(%bx), %ds
	xor	%bx, %bx
1:	lodsw
	cmp	$0xffff, %ax //終了
	je	1f
	test	$0xfe00, %ax //異常値
	jne	4f

	pusha
	push	%cs
	pop	%ds
	mov	%ax, %cx
	mov	$0x4f01, %ax
	mov	$__VESA_displayInfo, %edi
	int	$0x10
	cmp	$0x004f, %ax
	je	2f
3:	popa
	jmp	1b
2:	cmpb	$15, 25(%di) //15bpp未満は無視
	jb	3b
/*	xor	%eax, %eax
	mov	%cx, %ax
	call	__hex16
	mov	$' ', %al
	call	__putc16
	mov	18(%di), %eax
	call	__hex16
	mov	$' ', %al
	call	__putc16
	xor	%eax, %eax
	mov	25(%di), %al
	call	__hex16
	mov	$'\n', %al
	call	__putc16*/
	popa

	push	%ds
	push	%cs
	pop	%ds
	mov	$__VESA_displayInfo, %edi
	cmp	%edx, 18(%di)
	jb	2f
	mov	18(%di), %edx
	mov	%ax, %bx
2:	pop	%ds
	jmp	1b
1:	pop	%ds
	cmp	$0x010d, %bx
	jae	1f //有効なモードを取得できたので設定しに行く
#endif
4:	pop	%ds
#if CF_VESA_MODE || CF_FORCE_VESA_MODE
#if CF_FORCE_VESA_MODE
	mov	$CF_FORCE_VESA_MODE, %bx
#else
	mov	$CF_VESA_MODE, %bx
#endif
	and	$0x1ff, %bx
#else
	jmp	.L_failed
#endif
	// 画面モードを設定
1:	or	$0xc000, %bx
	mov	$0x4f02, %ax
	int	$0x10
	cmp	$0x004f, %ax
	jne	.L_failed

	// 最終的な画面データを取得
	mov	$0x4f01, %ax
	mov	%bx, %cx
	mov	$__VESA_displayInfo, %edi
	int	$0x10
	cmp	$0x004f, %ax
	jne	.L_failed
	ret

.L_failed:
	// 失敗メッセージの出力
	mov	$1f, %esi
	call	__puts16
	hlt

1:	.asciz	"VESA-VBE initalize failed.\n"


/// VbeInfoBlock@VESA-VBE3.0
.align 4
__VESA_InfoBlock:
.L_signature:	.ascii	"VBE2"
.L_version:	.word	0x0300
.L_oemString:	.long	0
.L_capabilities: .rep	4
		.long	0
		.endr
.L_videoModes:	.long	0
.L_totalMemory:	.word	0
.L_oemRev:	.word	0
.L_venderName:	.long	0
.L_productName:	.long	0
.L_productRev:	.long	0
		.space	222, 0
.L_oemData:	.space	256, 0


// ModeInfoBlock
__VESA_displayInfo:
		.space	256, 0
