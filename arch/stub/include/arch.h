/********************************************************* ARCHTECTURE DEPENDS
 *	Copyright (C) 2013- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _ARCH__
#define _ARCH__

#include <types.h>


class ARCH{
	/// 割り込みコントローラの制御
	class INTERRUPT{
	public:
		static void Enable(uint irq);	//irqの割り込みを許可
		static void Disable(uint irq);	//irqの割り込みを禁止
		static void EOI(uint irq);	//irqの割り込みに終了通知
	};

	/// 時計とシステムタイマ
	class CLOCK{
	public:
		static tunit GetUptime();		//システムの起動時間を返す
		static tunit UTC();		//現在時刻をUTCで返す
		static tunit GetLocalTime();	//現在時刻を返す
		static void SetTimezone(int);	//タイムゾーンを設定
		static int GetTimezone();		//タイムゾーンを取得
	}
}

#endif
