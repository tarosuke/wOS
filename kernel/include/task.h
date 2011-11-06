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

extern "C"{
#include <userlib/task.h>
#include <userlib/message.h>
}



class TASK{
	UNDEFAULT(TASK);
// 	TASK();
// 	TASK(TASK&);
// 	void operator=(TASK&);
	friend class PU;
public:
	enum PRIORITY{
		PRI_REALTIME,	//リアルタイム(ソフト化ハード化は設定)
		PRI_INTERRUPT,	//割り込みハンドラ
		PRI_UI,		//ユーザインターフェイス(ボタンに反応するとか)
		PRI_NORMAL,	//通常優先度
		PRI_BACKGROUND,	//バックグラウンド処理用
		__pri_max
	};
	class TASKQUEUE{	//優先度別タスク待ち行列
	public:
		void Add(TASK&){};
		TASK* Get(){
			//TODO:QUEUEを仕上げる
			return 0;
		};
	private:
		QUEUE<TASK> q[__pri_max];
	};
	void Enqueue(MESSAGE*); //TODO:メッセージをタスクのキューに追加してタスクをreadyにする。もし既にタスクがreadyであり、かつメッセージ優先度がタスク優先度より高ければ優先度継承に従いタスク優先度をメッセージ優先度に変更し新しい優先度のレディキューに繋ぎ直す。
	TASK(MAP&);
private:
	QUEUE<MESSAGE> in;	//受信メッセージ //TODO:優先度別のものを準備
	PU* owner;		//現在このタスクを実行しているプロセッサ
	static TASKQUEUE readyQueue; //レディキュー
	static TASK* GetReadyOne(){
		return readyQueue.Get();
	};
};


#endif

