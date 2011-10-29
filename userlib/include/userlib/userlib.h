/*********************************************************** USERLIBRARY TABLE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _ULIB_
#define _ULIB_

#include <userlib/message.h>
#include <userlib/uheap.h>


extern const struct _ULIB{
	unsigned version;			/* 要求版番号がこれより大きいユーザプロセスは起動拒否 */
	void (*RawSystemcall)(void);		/* 配置未定のシステムコールはこれを使う(開発用) */
	MESSAGE* (*WaitNextMessage)(unsigned long long); /* 次のイベントを返す。なければタイムアウトまで待つ */
	void (*HEAP_Init)(void);		// ユーザヒープ初期化(カーネルが使う)
	void* (*HEAP_Get)(unsigned size);	// ユーザヒープから取得
	void (*HEAP_Release)(void*);		// ユーザヒープへ返却
	//TODO:メモリ管理(マップ)
	//TODO:条件に合致するサーバを探し、パラメタとして自身への通信路と0個以上のマップを渡す
	//TODO:対象サーバに対してメッセージ送信
}ulib;


#endif
