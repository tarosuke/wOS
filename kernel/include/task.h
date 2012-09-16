/************************************************************************ TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _TASK_
#define _TASK_

#include <types.h>
#include <config.h>
#include <queue.h>
#include <map.h>
#include <cpu/task.h>
#include <vector.h>
#include <resource.h>


class TASK : public CPUTASK{
	friend class PU;
public:
	enum PRIORITY{
		/** 優先度
		 * 値が小さいほうが優先度が高い
		 * 新規タスク生成時には現在の優先度を限度として継承される */
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
		 * 同一のタスクでリソースの生成と消費の両方はできない
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
	class TASKQUEUE : public MULTIQUEUE<TASK, __pri_max>{
	public:
		inline void Add(TASK& task){
			if(task.priority < __pri_max){
				MULTIQUEUE<TASK, __pri_max>::Add(task.priority, task.qNode);
			}
		};
		inline void Insert(TASK& task){
			if(task.priority < __pri_max){
				MULTIQUEUE<TASK, __pri_max>::Insert(task.priority, task.qNode);
			}
		};
	};

	TASK(MAP&);		//マップを0から配置してタスクとする
	void* operator new(munit);
	void operator delete(void* mem);

	/// タスクの起床
	void WakeupByInterrupt(uint irq){
		(*this).irq = irq;
		reason = RS_FINE;
		Wakeup(PRI_INTERRUPT);
	};
	void WakeupByTimeup(){
		cronNode.Detach();
		reason = RS_TIMEUP;
		Wakeup(originalPriority);
	};
	void WakeupByMessage(class MESSAGE&);

	/// タスクを待ち状態にする
	void Wait(TASKQUEUE* const wait = 0, tunit uptime = TIME::INFINITE);

	VECTOR<RESOURCE> resources; //タスクが使えるリソース(ファイルハンドルみたいなもん)
private:
	TASK();			//現在のコンテキストをこのタスクとする
	~TASK();			//タスクの完全解放。graveKeeperからしか呼べない

	void Wakeup(PRIORITY);	//タスクを引数以上の優先度で起床

	MULTIQUEUE<class MESSAGE, __pri_max> in; //このタスクの受信メッセージ
	class PU* owner;		//現在このタスクを実行しているプロセッサ
	PRIORITY originalPriority; //元の優先度
	PRIORITY priority;	//現在の優先度
	NODE<TASK> qNode;	//待ちやレディキューのためのノード
	NODE<TASK> cronNode;	//時間管理のためのキューノード
	tunit uptime;		//時間切れ時刻
	NODE<TASK> irqNode;	//割り込みを使う場合のノード
	int irq;		//割り込みリクエスト(負数は無効)
	REASON reason;		//処理再開時に返すための理由メモ
	CAPABILITIES capabilities; //タスクの権限
	bool newbie;		//新規作成タスク(カーネルスタックが無効)

	///// 以下はシステム全体の話
	static const uint thisSizeIndex;
};

//TODO:SwitchSpace,RestoreSpaceなどの一時的メモリ空間切り替えなど。
#endif

