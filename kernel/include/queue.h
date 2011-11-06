/*********************************************************************** QUEUE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _QUEUE_
#define _QUEUE_

#include <types.h>
#include <config.h>
#include <types.h>
#include <lock.h>


template<typename T> class QUEUE{
public:
	bool IsThere(KEY&){ return false; };
	T* Get(KEY&){ return 0; };
	void Add(KEY&, T&){};
	bool IsThere(){ KEY key(lock); return IsThere(key); };
	T* Get(){ KEY key(lock); return Get(key); };
	void Add(T&){ KEY key(lock); Add(key); };
private:
	LOCK lock;
};


template<typename T, uint max> class MULTIQUEUE{
public:
	T* Get(){
		KEY key(lock);
		for(uint i(0); i < max; i++){
			if(q[i].IsThere(key)){
				return q[i].Get(key);
			}
		}
		return 0;
	};
private:
	QUEUE<T> q[max];
	LOCK lock;
};



#endif
