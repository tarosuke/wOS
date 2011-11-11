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
public:
	enum PRIORITY{
		PRI_REALTIME,	//リアルタイム(ソフト／ハードは別に設定)
		PRI_INTERRUPT,	//割り込みハンドラ
		PRI_UI,		//ユーザインターフェイス(ボタンに反応するとか)
		PRI_NORMAL,	//通常優先度
		PRI_BACKGROUND,	//バックグラウンド処理用
		__pri_max
	};
	typedef MULTIQUEUE<TASK, __pri_max> TASKQUEUE;
	typedef MULTIQUEUE<MESSAGE, __pri_max> MESSAGEQUEUE;
	void Enqueue(MESSAGE*); //TODO:メッセージをタスクのキューに追加してタスクをreadyにする。もし既にタスクがreadyであり、かつメッセージ優先度がタスク優先度より高ければ優先度継承に従いタスク優先度をメッセージ優先度に変更し新しい優先度のレディキューに繋ぎ直す。
	TASK();			//現在のコンテキストをこのタスクとする
	TASK(MAP&);		//マップを0から配置してタスクとする
	void* operator new(munit);
private:
	MESSAGEQUEUE in;	//このタスクの受信メッセージ
	PU* owner;		//現在このタスクを実行しているプロセッサ
	PRIORITY priority;	//現在の優先度
	NODE<TASK> qNode;	//待ちやレディキューのためのノード
	NODE<TASK> cronNode;	//時間管理のためのキューノード
	tunit uptime;		//時間切れ時刻

	void WakeUp(/*TODO:理由*/){};

	///// 以下はシステム全体の話
	static TASKQUEUE readyQueue; //レディキュー
	static TASK* GetReadyOne(){
		return readyQueue.Get();
	};
	static QUEUE<TASK> cronQueue;
	static void Cron(tunit);
	static const uint thisSizeIndex;
};

extern TASK kernelTask;

#endif

