/************************************************************* VERSION STRINGS
	Copyright (C) 2006- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
	$Id: 048a72a67f77ffb9a5e5519645bc1903789c3f5c $
*/

// NOTE: EUC-JP

// #include <version.h>
// #include <config.h>


const char __version[] =
	"talos 0.7("
	"built at " __DATE__ " " __TIME__ ")"
#if (0 < CF_DEBUG_LEVEL)
	" with debug option"
#endif
;

const char __copyright[] =
	" Copyright (C) 2004-2008 project talos (http://talos-kernel.sf.net/)\n"
	" ���Υ��եȥ�������GPL�˴�Ť��ƥ饤���󥹤���Ƥ��ޤ���\n"
	" �ܺ٤�GNU���̸�ͭ���ѵ�����(gpl-*.txt)�򻲾Ȥ��Ƥ���������";

const char __thanks[] =
	"****************** ���꤬�Ȥ��ʤ���� *******************\n"
	"̾�դ��ơ���ȯ���������礢�꤬�ȡ�\n"
	"	espy ��\n"
	"	yuri ��\n"
	"\n"
	"��ȯ����\n"
	"	etsav ��\n"
	"	yuri ��\n"
	"\n"
	"��ȯ�Ķ�\n"
	"	GNU��Linux��debian��Gentoo��bochs��qemu���������\n"
	"\n"
	"��ȯ�ˤ����äƤλ���\n"
	"	NetBSD��Linux��IA32�ȳƼ�������������\n"
	"*********************************************************\n";

const char funds[] = "���¾\n"
	"���ߴ��դ�����դ���Ǥ������åȤȤ��Ƥ�\n"
	"	1.��ȯ®�٤������뤫�⤷��ʤ���\n"
	"	2.��ȯ��ͥ���̤��ѹ��Ǥ��뤫�⤷��ʤ���\n"
	"	3.���쥸�åȤ˥�å�������񤱤롣\n"
	"�ʤɤ�����ޤ���������¾�˥ϡ��ɤʤ󤫤��ߤ��Ƥ��줿�ꤹ��Ȥ��Υϡ��ɤ��б���\n"
	"�ʤफ�⤷��ޤ��󡣤�äȤ⡢���ޤ��꤬�ʤ��ΤǤ�������Ϣ��򤯤������ʡ�\n"
	"�ޤ����л������դ��Ƥ��ޤ����ǽ�˽л���������뤤�Ϥ�������˥ץ������Ȥ�ˡ�ͤ��뤤���ȹ粽\n"
	"�����Ѱդ�����ޤ������ξ�硢\n"
	"	4.�ץ�ץ饤������ʥ饤���󥹤����䤷�����˽л�ۡ��׸��٤˱��������������롣\n"
	"�Ȥ������åȤ�����ޤ���";


