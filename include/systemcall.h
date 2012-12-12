/****************************************************************** SYSTEMCALL
 *	Copyright (C) 2012- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _SYSTEMCALL_
#define _SYSTEMCALL_

#include <types.h>


typedef struct{
	/** システムコールタイプ */
	enum{
		sys_noop,			/* 何もせずに戻る */
		sys_requestIdle,		/* バックグラウンドの要/不要通知 */
		sys_exit,		/* プロセスを終了する */
		sys_notify,		/* 通知の送信 */
		sys_waitNotify,		/* 次のイベントを待つ */
		sys_alloc,		/* 仮想メモリ域の割り当て */
		sys_release,		/* 仮想メモリ域の解放 */
		sys_getServer,		/* サーバの検索、起動、接続の取得 */
		sys_disposeServer,	/* サーバへの接続を破棄 */
		sys_sendServer,		/* 接続先へ別の接続を複製 */
	}request;
	/** 終了コード */
	enum{
		rc_fine = 0,		/* 正常終了(fine以外はinfo) */
		rc_idle,
		rc_timeup,
		rc_warn = 16384,		/* 問題を検出 */
		rc_error = -32768,	/* 異常終了 */
		rc_request_not_available,
		rc_target_not_present,
	}result;
	union{
		struct{
			u32 needIdle; /* 真ならアイドル通知が必要 */
		}pequestIdle;
		struct{
			void* start;
			u32 pages;
			/*TODO:フラグとか考えておくこと*/
		}alloc;
		struct{
			void* start;
			u32 pages;
		}release;
		struct{
			u32 target;	/* notify先 */
		}notify;
		struct{
			u32 from;	/* notifyしてきた相手(戻値) */
		}waitNotify;
//		struct{
//			u32 target;	/* 発見できたサーバ(戻値) */
//			UUID protocol;	/* 探すプロトコル */
//			VERSION protocolVersion;	/* 探すプロトコルの版 */
//			UUID vender;	/* ベンダ(ヒント) */
//			UUID product;	/* 製品(ヒント) */
//			VERSION productVersion; /* 製品の版(ヒント) */
//		}getServer;
	};
}SYSTEMCALL_PARAMS;





#endif
