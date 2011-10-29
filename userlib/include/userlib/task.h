/************************************************************************ TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _ULIB_TASK_
#define _ULIB_TASK_


extern struct{
	void* heap;
	unsigned stackSize;
	struct _MESSAGE* tx;
	struct _MESSAGE* rx;
	unsigned status;
	void (*entry)(void);
}taskHead;


/** statusについてのメモ
 * txは送信メッセージなので送信後は処理が終了するまでタスクは待ちになる
 * ゆえにタスクが実行中ならtxはいつでも空き状態になっている
 * WaitNextMessageが呼ばれるとrmが書き換えられる
 * メッセージがbitで伝えられる場合、rxは無効のままstatusのビットが立つ
 * 例えば割り込みの受信や終了要請などはフラグで伝えられる
 */



#endif
