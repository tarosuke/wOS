
/************************************************************** VESA-VBE初期化 */


.code16

.global __VESA_InfoBlock, __VESA_displayInfo
.extern __putc16, __puts16, __hex16

.section .pdef16
	.long	.L_Init

.section .init
.L_Init:
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
	push	%ds
	mov	$.L_videoModes, %ebx
	mov	(%bx), %si
	mov	2(%bx), %ds
	xor	%bx, %bx
1:	lodsw
	cmp	$0xffff, %ax
	je	1f
	and	$0x1ff, %ax
	cmp	%ax, %bx
	ja	1b
	mov	%ax, %bx
	jmp	1b
1:	pop	%ds
	cmp	$0x010d, %bx
	jb	.L_failed //有効なモードを取得できていないので失敗

	// 画面モードを設定
	or	$0xc000, %bx
	mov	$0x4f02, %ax
	int	$0x10
	cmp	$0x004f, %ax
	jne	.L_failed

	// 画面データを取得
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
.L_sighneture:	.ascii	"VBE2"
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
