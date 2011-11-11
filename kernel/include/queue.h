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


template<class T> class NODE{
public:
	NODE(T* owner) : owner(owner), next(this), prev(this){};
	//thisをnの前に接続
	void Insert(NODE& n){
		prev = n.prev;
		next = n;
		n.prev = (*prev).next = this;
	};
	//thisをnの後に接続
	void Attach(NODE& n){
		next = n.next;
		prev = n;
		n.next = (*next).prev = this;
	};
	//thisをどこかから外す(どこにも繋がっていなくてもおｋ)
	void Detach(){
		(*next).prev = prev;
		(*prev).next = next;
		prev = next = this;
	};
	T* Owner(){ return owner; };
	NODE* Next(){ return next; };
	NODE* Prev(){ return prev; };
protected:
	NODE() : owner(0), next(this), prev(this){};
	T* const owner;
	NODE* next;
	NODE* prev;
};


template<class T> class QUEUE : public NODE<T>{
	friend class ITOR;
public:
	class ITOR{
	public:
		ITOR(QUEUE& q) : q(&q), n(&q), key(q.lock){};
		T* operator++(int){
			if((*n).Next() != q){
				n = (*n).Next();
				return (*n).Owner();
			}
			return 0;
		};
		T* operator--(int){
			if((*n).Prev() != q){
				n = (*n).Prev();
				return (*n).Owner();
			}
			return 0;
		};
	private:
		const NODE<T>* const q;
		NODE<T>* n;
		KEY key;
	};
	bool IsThere(KEY&){
		return (*this).next != this;
	};
	T* Get(KEY&){
		if(IsThere()){
			T* o((*this).owner);
			(*(*this).next).Detach();
			return o;
		}
		return 0;
	};
	void Add(KEY&, NODE<T>& n){
		//リンクは輪になっているのでアンカーであるキューの前は最後
		Insert(n);
	};
	bool IsThere(){ KEY key(lock); return IsThere(key); };
	T* Get(){ KEY key(lock); return Get(key); };
	void Add(NODE<T>&){ KEY key(lock); Add(key); };
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
