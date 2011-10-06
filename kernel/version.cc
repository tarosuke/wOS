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
	" このソフトウェアはGPLに基づいてライセンスされています。\n"
	" 詳細はGNU一般公有使用許諾書(gpl-*.txt)を参照してください。";

const char __thanks[] =
	"****************** ありがとうなきもち *******************\n"
	"名付け親＆開発初期から応援ありがと〜\n"
	"	espy 様\n"
	"	yuri 様\n"
	"\n"
	"開発協力\n"
	"	etsav 様\n"
	"	yuri 様\n"
	"\n"
	"開発環境\n"
	"	GNUとLinuxとdebianとGentooとbochsとqemuの中の方々\n"
	"\n"
	"開発にあたっての参考\n"
	"	NetBSDとLinuxとIA32と各種資料の中の方々\n"
	"*********************************************************\n";

const char funds[] = "資金他\n"
	"現在寄付を受け付け中です。メリットとしては\n"
	"	1.開発速度を増せるかもしれない。\n"
	"	2.開発の優先順位を変更できるかもしれない。\n"
	"	3.クレジットにメッセージを書ける。\n"
	"などがありますが、この他にハードなんかを貸してくれたりするとそのハードの対応が\n"
	"進むかもしれません。もっとも、あまり場所がないのでその前に連絡をくださいな。\n"
	"また、出資も受け付けています。最初に出資時点、あるいはそれ以前にプロジェクトを法人あるいは組合化\n"
	"する用意があります。その場合、\n"
	"	4.プロプライエタリなライセンスで販売した時に出資額、貢献度に応じた配当がある。\n"
	"というメリットがあります。";


