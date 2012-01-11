/***************************************************** KERNEL LOADER FROM FD
	Copyright (C) 2004- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
*/


.code16
.att_syntax

.text
	/**** SETUP */
	mov	%cs, %ax
	mov	%ax, %ds
	mov	$0x1000, %ax
	mov	%ax, %es

	///// SAY HELLO
	movw	$hello, %si
	call	puts

	///// check drive
	xor	%ah, %ah
	mov	%ah, %dl
	int	$13
	jc	SayError

	///// load kernel header
	mov	$0x0201, %ax
	mov	$0x0002, %cx
	xor	%dx, %dx
	mov	%dx, %bx
	int	$0x13
	jc	SayError

	///// check header
	mov	%es:(16), %eax
	mov	%eax, (kentry)
	mov	%es:(20), %eax
	add	$511, %eax
	shr	$9, %eax
	mov	%eax, (ksize)

1:
	///// next sector
	// increment sector
	inc	%cl
	cmp	$18, %cl
	jbe	2f
	mov	$1, %cl

	// increment head
	xor	$1, %dh
	jnz	2f

	// increment track
	inc	%ch

2:
	// show progress
	push	%bx
	push	%cx
	mov	(rotPos), %bx
	inc	%bx
	and	$3, %bx
	mov	%bx, (rotPos)
	add	$rotate, %bx
	mov	(%bx), %al
	mov	$0x000f, %bx
	mov	$1, %cx
	mov	$9, %ah
	int	$0x10
	pop	%cx
	pop	%bx

	///// point next
	mov	%es, %ax
	add	$0x20, %ax
	mov	%ax, %es

	///// read a sector
	mov	$0x0201, %ax
	int	$0x13
	jc	SayError

	///// check sectors
	decl	(ksize)
	jnz	1b

	///// run kernel
	mov	$' ', %al
	mov	$0x0007, %bx
	mov	$1, %cx
	mov	$9, %ah
	int	$0x10
	mov	$ok, %si
	call	puts
	mov	(kentry), %eax
	mov	%ax, (kentry)
	shr	$4, %eax
	and	$0xf000, %eax
	mov	%ax, (kentry + 2)
	ljmp	*(kentry)

/***** error, etc. */
SayError:
	push	%ax
	movw	$error, %si
	call	puts
	pop	%ax
	call	hex
	cli
8:	hlt
	jmp	8b


/***** print messeages */
putc:	movb	$0x0e, %ah
	movb	$0x0f, %bl
	int	$0x10
	ret
puts:	cld
1:	lodsb
	testb	%al, %al
	jz	2f
	call	putc
	jmp	1b
2:	ret
hex:	mov	%eax, %edx
	mov	$8, %cx
1:	rol	$4, %edx
	mov	%dl, %al
	and	$0x0f, %al
	add	$0x30, %al
	cmp	$0x39, %al
	jbe	2f
	add	$7, %al
2:	call	putc
	loop	1b
	ret
hex4:	rol	$16, %eax
	mov	%eax, %edx
	mov	$4, %cx
	jmp	1b

#if 0
rdump:	push	%ax
	push	%bx
	push	%cx
	push	%dx

	push	%es
	push	%ds
	push	%dx
	push	%cx
	push	%bx
	push	%ax
	mov	$a, %si
	call	puts
	pop	%ax
	call	hex4
	mov	$b, %si
	call	puts
	pop	%ax
	call	hex4
	mov	$c, %si
	call	puts
	pop	%ax
	call	hex4
	mov	$d, %si
	call	puts
	pop	%ax
	call	hex4
	mov	$ds, %si
	call	puts
	pop	%ax
	call	hex4
	mov	$es, %si
	call	puts
	pop	%ax
	call	hex4
	mov	$'\r', %al
	call	putc
	mov	$'\n', %al
	call	putc
	pop	%dx
	pop	%cx
	pop	%bx
	pop	%ax
	ret
a:	.asciz	" ax:"
b:	.asciz	" bx:"
c:	.asciz	" cx:"
d:	.asciz	" dx:"
ds:	.asciz	" ds:"
es:	.asciz	" es:"
#endif

	/**** DUMP TARGET */
#if 0
dump:	mov	$128, %cx
	mov	$0x1000, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	$0x1000, %si
1:	test	$0x000f, %si
	jnz	2f
	mov	$'\r', %al
	call	putc
	mov	$'\n', %al
	call	putc
	mov	%si, %ax
	push	%cx
	call	hex4
	mov	$':', %al
	call	putc
	pop	%cx
2:	lodsw
	push	%cx
	call	hex4
	mov	$' ', %al
	call	putc
	pop	%cx
	loop	1b
	ret
#endif

.data
hello:	.ascii	"talosFDL ver 0.2.1\r\n"
	.ascii	"Copyright (C) 2004-2007 Yuji Tateno <webmaster@tarosuke.net>\r\n"
loading: .asciz	"loading..."
ok:	.ascii	"OK."
crlf:	.asciz	"\r\n"
error:	.ascii	"NG.\r\ncouldn't load kernel."
	.ascii	" be sure floppy drive and media.\r\n"
	.asciz	"code:"
rotate:	.ascii	"/-\\|"

.bss
ksize:	.long	0
kentry:	.long	0
rotPos:	.short	0




