/************************************************************************ TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _TASK_
#define _TASK_

#include <types.h>
#include <config.h>
#include <queue.h>
#include <pu.h>
#include <map.h>
#include <cpu/task.h>

extern "C"{
#include <userlib/task.h>
#include <userlib/message.h>
}


class TASK : public CPUTASK{
	friend class PU;
	friend class CLOCK;
	friend class INTERRUPT;
public:
	enum PRIORITY{
		/** 優先度
		 * 値が小さいほうが優先度が高い
		 * 新規タスク生成時には現在の優先度が最高として継承される */
		PRI_REALTIME,	//リアルタイム(ソフト／ハードは別に設定)
		PRI_INTERRUPT,	//割り込みハンドラ
		PRI_UI,		//ユーザインターフェイス(ボタンに反応するとか)
		PRI_NORMAL,	//通常優先度
		PRI_BACKGROUND,	//バックグラウンド処理用
		__pri_max
	};
	union CAPABILITIES{
		/** いわゆるCapability
		 * ビットが1ならリソース生成許可。0なら使用許可。
		 * 同一のタスクでリソースの生成と消費の両方は不可能
		 * 新規タスク生成時にビットを0にするのは可能という形で継承される */
		struct{
			u32 display : 1;
			u32 uid : 1;
			u32 file : 1;
			u32 socket : 1;
		};
		u32 raw;
	};
	enum REASON{
		/** 処理終了理由 */
		RS_FINE,	//正常終了
		RS_PARAM,	//パラメタに問題がある
		RS_CAPABILITY,	//タスクに権限がない
		RS_PERMITION,	//ユーザに権限がない
		RS_TIMEUP,	//時間切れ
	};
	typedef MULTIQUEUE<TASK, __pri_max> TASKQUEUE;
	typedef MULTIQUEUE<MESSAGE, __pri_max> MESSAGEQUEUE;
	void Enqueue(MESSAGE*); //TODO:メッセージをタスクのキューに追加してタスクをreadyにする。もし既にタスクがreadyであり、かつメッセージ優先度がタスク優先度より高ければ優先度継承に従いタスク優先度をメッセージ優先度に変更し新しい優先度のレディキューに繋ぎ直す。
	void Enqueue(uint irq){
		priority = PRI_INTERRUPT;
		(*this).irq = irq;
		//TODO:タスクを起動登録
	};
	TASK();			//現在のコンテキストをこのタスクとする
	TASK(MAP&);		//マップを0から配置してタスクとする
	void* operator new(munit);
	void operator delete(void* mem);
private:
	MESSAGEQUEUE in;	//このタスクの受信メッセージ
	PU* owner;		//現在このタスクを実行しているプロセッサ
	PRIORITY priority;	//現在の優先度
	NODE<TASK> qNode;	//待ちやレディキューのためのノード
	NODE<TASK> cronNode;	//時間管理のためのキューノード
	tunit uptime;		//時間切れ時刻
	NODE<TASK> irqNode;	//割り込みを使う場合のノード
	int irq;		//割り込みリクエスト(負数は無効)
	REASON reason;		//処理再開時に返すための理由メモ
	CAPABILITIES capabilities; //タスクの権限

	void WakeUp(REASON r){};

	///// 以下はシステム全体の話
	static TASKQUEUE readyQueue;
	static QUEUE<TASK> cronQueue;
	static void Cron(tunit);
	static const uint thisSizeIndex;
	static TASK* GetReadyOne(){
		return readyQueue.Get();
	};
	static inline void Dispatch(){
		//TODO:必要ならタスクディスパッチ
	}
};

extern TASK kernelTask;

#endif

