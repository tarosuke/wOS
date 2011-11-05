/************************************************************************ TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _TASK_
#define _TASK_


#include <types.h>
#include <config.h>
#include <queue.h>

extern "C"{
#include <userlib/task.h>
#include <userlib/message.h>
}



class TASK{
	friend class CPU;
public:
	void Enqueue(MESSAGE*); //TODO:メッセージをタスクのキューに追加してタスクをreadyにする。もし既にタスクがreadyであり、かつメッセージ優先度がタスク優先度より高ければタスク優先度をメッセージ優先度に変更し新しい優先度のレディキューに繋ぎ直す。
private:
	QUEUE<MESSAGE> in;
	CPU* owner;	//現在このタスクを実行しているプロセッサ
};


#endif

