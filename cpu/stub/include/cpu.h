/************************************************************ CPU MANIPULATION
 *	Copyright (C) 2011- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _CPU_
#define _CPU_

#include <types.h>
#include <config.h>


class CPU{
	CPU(CPU&);
	void operator=(CPU&);
public:
	/// 割り込み制御
	class INTERRUPT{
		template<class INTERRUPT> friend class KEY;
		INTERRUPT(INTERRUPT&);
		void operator=(INTERRUPT&);
	public:
		void LOCK();
		void Unlock();
		static void Enable();
		static void Disable();
	};

	/// 例外処理、あるいは例外それ自体
	class EXCEPTION{
		EXCEPTION(EXCEPTION&);
		void operator=(EXCEPTION&);
	public:
		EXCEPTION();
		typedef void (*HANDLER)(class const EXCEPTION&);
		static void RegisterHandler(uint exceptionNumber, HANDLER);
	private:
	};

	/// 割り込み禁止とスピンロック
	class LOCK : private INTERRUPT{
		template<class LOCK> friend class KEY;
		LOCK(LOCK&);
		void operator=(LOCK&);
	public:
		void LOCK();
		void Unlock();
	};

	/// 仮想空間操作
	class MEMORY{
	public:
		MEMORY();		//アイドルタスク用(カーネルページのみ)
		struct ATTRIBUTE{
			// ページ属性(基本分)
			static const runit present = 0x01;
			static const runit readOnly = 0x02;
			static const runit user = 0x04;
			static const runit kernel = 0;
			static const runit noExec = 0;
			static const runit exec = 0;
			//ページ属性(カーネル管理分)
			static const runit valid = 0x200;
			static const runit mapped = 0x400;
			static const runit copyOnWrite = 0x800;
			// ページ属性(合成、Enable時の初期値)
			static const runit kernelEnable = valid + kernel;
			static const runit userEnable = valid + user;
			static const runit mapEnable = valid + mapped;
			static const runit cowEnable = valid + copyOnWrite;
		};

		// ページ有効化/実ページ割り当て
		static void Enable(PUNIT start, PUNIT size);
		static void Enable(PUNIT start, uint mapID, PUNIT size);
		static void Enable(PUNIT start, RUNIT pa, PUNIT size);

		// ページ無効化・ユーザ空間なら返却
		static void Disable(PUNIT start, PUNIT pages);

		// ページフォルトハンドラ
		static void Fault(const EXCEPTION&);
	protected:
		MEMORY(const MEMORY&);	//その他の空間はアイドルタスクからの派生
		static void _Enable(void*, munit pages);
		static void _Enable(void*, runit pa, punit pages);
		static void _Disable(void*, munit pages);
	private:
	};

	/// タスク操作
	class TASK{
	protected:
		TASK();
		TASK(const TASK&);
		void DiveInto();		//このインスタンスが指すユーザプロセスへ
		void DispatchTo();	//このインスタンスが指すコンテキストへ
	};
protected:
	/// プロセッサ自体の操作
	CPU();
	static uint GetID();	//これを実行しているプロセッサIDの取得
	static void Idle();	//低消費電力状態に入って待つ
};


#endif
