/******************************************************************** talosMBR
   Copyright (C) 2007- Yuji Tateno<webmaster@tarosuke.net>

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
// $Id: 5f27f845bcb2d92b7927cc719890f99e543720a6 $

.code16
.att_syntax

.extern partitionTable

.text
	// setup
	mov %cs, %ax
	mov %ax, %ds
	mov %ax, %es

	// (re)load mbr
	mov	$2, %ah
	call	Mbr

	// move itself
	mov	$0x7c00, %si
	mov	$0x7e00, %di
	mov	$223, %cx
	rep movsw
	.byte 0xe9, 0x00, 0x02

newplace:
	/***** SAY HELLO */
	mov	$hello, %si
	call	puts

	// check media
9:	jmp	Choose

	// check modifier key state
	mov	$2, %ah
	int	$0x16
	test	$0x5f, %al
	jz	boot

	// choose partition
Choose:	movw	$choose, %si
	call	puts
1:	xor	%ah, %ah
	int	$0x16
	cmp	$'f', %al
	je	fdBoot
	cmp	$'F', %al
	je	fdBoot
	cmp	$'1', %al
	jb	1b
	cmp	$'4', %al
	ja	1b
	push	%ax
	call	Choice

	// clear installer mode
	movw	$0x9090, (9b)

	// clear active flags
	xor	%al, %al
	mov	$partitionTable, %bx
	mov	%al, (%bx)
	mov	%al, 16(%bx)
	mov	%al, 32(%bx)
	mov	%al, 48(%bx)

	// set active flag
	pop	%ax
	dec	%al
	shl	$4, %al
	xor	%ah, %ah
	add	%ax, %bx
	movb	$0x80, (%bx)

	// check CAPS
	mov	$2, %ah
	int	$0x16
	test	$0x40, %al
	jz	boot

	// save mbr
	mov	$3, %ah
	call	Mbr

boot:
	// scan partitionTable
	mov	$partitionTable, %si
	mov	$4, %cx
1:	cmpb	$0x80, (%si)
	je	1f
	add	$16, %si
	loop	1b

	// no active partition
	mov	$noActive, %si
	call	puts
	jmp	Choose

1:	// check partition type
	xor	%al, %al
	or	4(%si), %al
	jz	2f

1:	// get sector
	mov	8(%si), %eax
	mov	%eax, (sector)

	// load PBR
	mov	$0x42, %ah
	mov	$0x80, %dl
	mov	$pack, %si
	int	$0x13
	jc	SayError

	// check PBR
runDirect:
	cmpw	$0xaa55, (0x7dfe)
	je	1f
2:	mov	$illigal, %si
	call	puts
	jmp	Choose

	// run PBR
1:	.byte 0xea, 0x00, 0x7c, 0, 0

SayError:
	mov	$error, %si
	call	puts
	cli
1:	hlt
	jmp	1b

	// read/write MBR(ah=2:read 3:write)
Mbr:	mov	$0x0080, %dx
	mov	$0x0001, %cx
	mov	%cl, %al
	mov	$0x7e00, %bx
	int	$0x13
	jc	SayError
	ret

	// boot from fd
fdBoot:	call	Choice
	mov	$0x0201, %ax
	xor	%dx, %dx
	mov	%dx, %cx
	inc	%cx
	mov	$0x7c00, %bx
	int	$0x13
	jc	SayError
	jmp	runDirect

	// print choice
Choice:	call	putc
	mov	$crlf, %si
	call	puts
	ret


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



hello:	.ascii	"talosMBR ver 0.3.5"
	.ascii	" (C) 2007 Yuji Tateno <webmaster@tarosuke.net>"
crlf:	.asciz	"\r\n"
choose:	.asciz	"boot HD(1-4/CAPS to save)/FD(f):"
noActive: .asciz "No active partition.\r\n"
error:	.asciz	"Fail to load."
illigal: .asciz	"Illigal partition.\r\n"
pack:	.short	16
	.short	1
	.short	0x7c00
	.short	0
sector:	.quad	0

