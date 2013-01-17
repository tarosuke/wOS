/******************************************************************** TASKLOCK
 *	Copyright (C) 2013- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#include <task.h>


/// 通常ロック
void TASK::LOCK::Lock(){
	KEY<HARDLOCK> key(lock);
	if(inUse){
		TASK::Sleep(&q);
	}else{
		inUse = true;
	}
};
void TASK::LOCK::Unlock(){
	KEY<HARDLOCK> key(lock);
	TASK* const t(q.Get());
	if(t){
		(*t).Wakeup();
	}else{
		inUse = false;
	}
};


/// reader-writerロック
void TASK::RWLOCK::ReaderLock(){
	KEY<HARDLOCK> key(lock);
	if(writing){
		TASK::Sleep(&rq);
	}else{
		readers++;
	}
};
void TASK::RWLOCK::ReaderUnlock(){
	KEY<HARDLOCK> key(lock);
	if(!--readers){
		TASK* const t(wq.Get());
		if(t){
			(*t).Wakeup();
		}
	}
};
void TASK::RWLOCK::WriterLock(){
	KEY<HARDLOCK> key(lock);
	if(readers){
		TASK::Sleep(&wq);
	}else{
		if(writing){
			TASK::Sleep(&wq);
		}else{
			writing = true;
		}
	}
};
void TASK::RWLOCK::WriterUnlock(){
	KEY<HARDLOCK> key(lock);
	TASK* const t(rq.Get());
	if(!t){
		if(!--readers){
			TASK* const wt(wq.Get());
			if(wt){
				(*wt).Wakeup();
				writing = true;
			}
		}
	}else{
		(*t).Wakeup();
	}
};
