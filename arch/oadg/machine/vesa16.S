
/************************************************************** VESA-VBE初期化 */


.code16

.global __VESA_InfoBlock, __VESA_displayInfo

.section .pdef16
	.long	.L_Init

.section .init
.L_Init:
	// 有効モードの取得
	mov	%cs, %ax
	mov	%ax, %ds
	mov	%ax, %es
	lea	(__VESA_InfoBlock), %di
	mov	$0x4f00, %ax
	int	$0x10
	cmp	$0x4f00, %ax
	jne	.L_failed

	// 画面情報を取得して有効モードの最大値をbxに取得
	push	%ds
	mov	(.L_videoModes), %ax
	mov	(.L_videoModes+2), %si
	mov	%ax, %ds
	xor	%bx, %bx
1:	lodsw
	cmp	$0xffff, %ax
	je	1f
	cmp	%ax, %bx
	jna	1b
	mov	%bx, %ax
	jmp	1b
1:	pop	%ds
	cmp	$0xc10d, %bx
	jb	.L_failed //有効なモードを取得できていないので失敗

	// 画面モードを設定
	mov	$0x4f02, %ax
	int	$0x10
	cmp	$0x4f00, %ax
	jne	.L_failed

	// 画面データを取得
	mov	$0x4f01, %ax
	mov	%bx, %cx
	lea	(__VESA_displayInfo), %di
	int	$0x10
	cmp	$0x4f00, %ax
	jne	.L_failed
	ret

.L_failed:
	// 失敗メッセージの出力
	ret


/// VbeInfoBlock@VESA-VBE3.0
__VESA_InfoBlock:
.L_sighneture:	.long	0
.L_version:	.word	0
.L_oemString:	.long	0
.L_capabilities: .long	0
		.long	0
		.long	0
		.long	0
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
