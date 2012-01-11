/******************************************************** KERNEL LOADER FROM HD
   Copyright (C) 2005-2008 Yuji Tateno<webmaster@tarosuke.net>

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston,
	 MA  02111-1307, USA.

	本プログラムはフリー・ソフトウェアです。あなたは、Free Software Foundation
	が公表したGNU 一般公有使用許諾の「バージョン２」或いはそれ以降の各バージョ
	ンの中からいずれかを選択し、そのバージョンが定める条項に従って本プログラム
	を再頒布または変更することができます。

	本プログラムは有用とは思いますが、頒布にあたっては、市場性及び特定目的適合
	性についての暗黙の保証を含めて、いかなる保証も行ないません。詳細については
	GNU 一般公有使用許諾書をお読みください。

	あなたは、本プログラムと一緒にGNU 一般公有使用許諾の写しを受け取ってい
	るはずです。そうでない場合は、Free Software Foundation, Inc., 675 Mass Ave,
	Cambridge, MA 02139, USA へ手紙を書いてください。
*/

#include "config.h"


.code16
.att_syntax


.text
	/**** SETUP */
	cli
	mov	%cs, %ax
	mov	%ax, %ds
	mov	$0x2000, %ax
	mov	%ax, %es
	sti

	/***** SAY HELLO */
	movw	$hello, %si
	call	puts

	/**** CHECK DRIVE */
	xor	%ax, %ax
	mov	$0x80, %dl
	int	$0x13
	jc	7f

	/**** LOAD KERNEL */
	mov	(startOfPartition), %eax
	inc	%eax
	mov	%eax, (sector)
	mov	$params, %si
	mov	$1, %cx
1:	push	%cx

	mov	$0x42, %ah
	mov	driveOfPartition, %dx
	or	$0x80, %dl
	int	$0x13
	jc	7f

	cmpw	$0x2000, (buff + 2)
	jne	2f
	/* GET KERNEL HEADER */
	mov	%es:(16), %ecx
	mov	%ecx, (kentry)
	mov	%es:(20), %ecx
	add	$65535, %ecx
	shr	$16, %ecx
	mov	%ecx, (ksize)
	pop	%cx
	mov	(ksize), %cx
	push	%cx
2:
	// INCREMENT
	addw	$0x1000, (buff + 2)
	addl	$128, (sector)
	adcl	$0, (sector + 4)

	// PROGRESS DOT
	push	%si
	mov	$'.', %al
	call	putc
	pop	%si

	pop	%cx
	loop	1b

	// LOADED
	mov	$ok, %si
	call	puts


/**** RUN KERNEL */
 	mov	(kentry), %eax
	mov	%ax, (kentry)
	shr	$4, %eax
	and	$0xf000, %eax
	mov	%ax, (kentry + 2)
	ljmp	*(kentry)


/***** error, etc. */
7:	push	%ax
	movw	$error, %si
	call	puts
	pop	%ax
	call	hex
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

#if 0
rdump:	push	%ax
	push	%bx
	push	%cx
	push	%dx

	push	%dx
	push	%cx
	push	%bx
	push	%ax
	mov	$a, %si
	call	puts
	pop	%ax
	call	hex
	mov	$b, %si
	call	puts
	pop	%ax
	call	hex
	mov	$c, %si
	call	puts
	pop	%ax
	call	hex
	mov	$d, %si
	call	puts
	pop	%ax
	call	hex
	mov	$'\n', %al
	call	putc
	pop	%dx
	pop	%cx
	pop	%bx
	pop	%ax
	ret

	/**** DUMP TARGET */
dump:	mov	$128, %cx
	mov	$0x2000, %ax
	mov	%ax, %ds
	mov	%ax, %es
	xor	%si, %si
1:	test	$0x000f, %si
	jnz	2f
	mov	$'\n', %al
	call	putc
	mov	%si, %ax
	push	%cx
	call	hex
	mov	$':', %al
	call	putc
	pop	%cx
2:	lodsw
	push	%cx
	call	hex
	mov	$' ', %al
	call	putc
	pop	%cx
	loop	1b
	ret
#endif


.data
hello:	.ascii	"talosPBR ver 0.2.0\r\n"
	.ascii	"copyright (c) 2005-2008 Yuji Tateno <webmaster@tarosuke.net>\r\n"
	.asciz	"loading"
ok:	.ascii	"OK."
crlf:	.asciz	"\r\n"
error:	.ascii	"...NG.\r\ncouldn't load kernel."
	.ascii	" be sure drive.\r\n"
	.asciz	"code:"

#if 0
a:	.asciz	" ax:"
b:	.asciz	" bx:"
c:	.asciz	" cx:"
d:	.asciz	" dx:"
#endif

params:	.short	16
sectors: .short 128
buff:	.long	0x20000000
sector:	.quad	0


.bss
ksize:	.long	0
kentry:	.long	0



