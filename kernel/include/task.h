/************************************************************************ TASK
 *	Copyright (C) 2011- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _TASK_
#define _TASK_

#include <types.h>
#include <config.h>
#include <queue.h>
#include <cpu/task.h>
#include <vector.h>
#include <resource.h>


class TASK : private CPUTASK{
	friend class PU;
	// 再利用するためnew/deleteは使わない
	void* operator new(munit);
	void operator delete(void*);
public:
	/** 常数などの定義 */
	class PRIORITY{
		/** 優先度
		 * 値が小さいほうが優先度が高い
		 * 新規タスク生成時には現在の優先度を限度として継承される */
		PRIORITY();
	public:
		enum VALUE{
			REALTIME,	//リアルタイム(ソフト／ハードは別に設定)
			INTERRUPT,	//割り込みハンドラ
			UI,		//ユーザインターフェイス
			NORMAL,		//通常優先度
			BACKGROUND,	//バックグラウンド処理用
			__pri_max
		};
		PRIORITY(VALUE v) : value(v){};
		PRIORITY(const PRIORITY& o) : value(o.value){};
		void operator=(VALUE v){
			value = v;
		};
		const PRIORITY& operator=(const PRIORITY& o){
			value = o.value;
			return *this;
		};
		operator VALUE(){
			return value;
		};
	private:
		VALUE value;
	};
	enum WAKECODE{
		/** 待ち終了理由 */
		WC_FINE,			//正常終了
		WC_TIMEUP,		//時間切れ
		WC_QUEUE_DISCARDED,	//対象が消滅
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

	/** タスクのキュー */
	class TASKQUEUE : public MULTIQUEUE<TASK, PRIORITY::__pri_max>{
	public:
		~TASKQUEUE();
		void Add(TASK&);
		void Insert(TASK&);
		void Wakeup(WAKECODE wc = WC_FINE);	//TODO:先頭タスクを起床
	};

	/** 待ちにタスク切り替えを使うロック */
	class LOCK{
		template<class LOCK> friend class KEY;
	public:
		LOCK() : inUse(false){};
	private:
		typedef ::LOCK HARDLOCK;
		TASK::TASKQUEUE q;
		HARDLOCK lock;
		bool inUse;
		void Lock();
		void Unlock();
	};
	typedef KEY<LOCK> key;

	/** 待ちにタスク切り替えを使うロック(reader-writer) */
	class RWLOCK{
	public:
		class READKEY{
		public:
			READKEY(RWLOCK& lock) : lock(lock){
				lock.ReaderLock();
			};
			~READKEY(){
				lock.ReaderUnlock();
			};
		private:
			RWLOCK& lock;
		};
		class WRITEKEY{
		public:
			WRITEKEY(RWLOCK& lock) : lock(lock){
				lock.WriterLock();
			};
			~WRITEKEY(){
				lock.WriterUnlock();
			};
		private:
			RWLOCK& lock;
		};
		RWLOCK() : readers(0), writing(false){};
	private:
		typedef ::LOCK HARDLOCK;
		TASK::TASKQUEUE rq;
		TASK::TASKQUEUE wq;
		int readers;
		bool writing;
		HARDLOCK lock;
		void ReaderLock();
		void ReaderUnlock();
		void WriterLock();
		void WriterUnlock();
	};

	/// タスクの起動、終了(new/deleteではない)
	static QUEUE<TASK> pool;	//再利用のためのプール
	static bool Create(class MAP&);	//TODO:poolから取り出してみて、なければ新規作成
	static void Dispose();		//TODO:優先度を下げ、ユーザ空間を開放してpoolに入る

	/// 待ち
	static WAKECODE Sleep(TASKQUEUE* target = 0, tunit uptime = TIME::INFINITE); //TODO:TUNITもUNITの子にする

	/// TICK
	static void Tick(tunit now){
		cronQueue.Tick(now);
	};
private:
	/// アイドルタスク生成(現在のコンテキストをこのタスクとする)
	TASK(); //TODO:PUからしか呼ばれない

	/** 時間待ちキュー */
	static class CRONQUEUE : private QUEUE<TASK>{
	public:
		void Register(TASK&, tunit);
		void Tick(tunit);
	}cronQueue;

	/// 起床
	void Wakeup(WAKECODE wc = WC_FINE);	//TODO:WAKECODEを設定して起床(外からは呼べず、CRONQUEUEやTASKQUEUEなどのサブクラスから呼ばれる)。その際時間待ち、QUEUE待ちの両方が解除される





	PRIORITY priority;	//優先度
	NODE<TASK> qNode;		//待ちやレディキューのためのノード
	NODE<TASK> cronNode;	//時間管理のためのキューノード
	tunit uptime;		//時間切れ時刻



	static const uint thisSizeIndex;
};











#if 0

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

	/// マップ取得
	MAP* GetMap(uint resNum){
		return 0;
	};

private:
	TASK();			//現在のコンテキストをこのタスクとする
	~TASK();			//タスクの完全解放。graveKeeperからしか呼べない

	void Wakeup(PRIORITY);	//タスクを引数以上の優先度で起床

	MULTIQUEUE<class MESSAGE, __pri_max> in; //このタスクの受信メッセージ
	QUEUE<class RESOURCE> resources;

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
};

//TODO:SwitchSpace,RestoreSpaceなどの一時的メモリ空間切り替えなど。
#endif

#endif

