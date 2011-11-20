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
#include <debug.h>


template<class T> class NODE{
public:
	NODE(T* owner) : owner(owner), next(this), prev(this){};
	//thisをnの前に接続
	void Insert(NODE& n){
		prev = n.prev;
		next = &n;
		n.prev = (*prev).next = this;
	};
	//thisをnの後に接続
	void Attach(NODE& n){
		next = n.next;
		prev = &n;
		n.next = (*next).prev = this;
	};
	//thisをどこかから外す(どこにも繋がっていなくてもおｋ)
	void Detach(){
		(*next).prev = prev;
		(*prev).next = next;
		prev = next = this;
	};
	inline T* Owner(){ return owner; };
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
		/** インスタンスが存在する間は対象キューが変化しないことが保証される
		 * が、その間他の一切の操作はブロックされるのでキュー操作はすべて
		 * ITORを通す必要がある */
	public:
		ITOR(QUEUE& q) : q(&q), n(q.next), key(q.lock){};
		T* operator++(int){
			if(n != q){
				T* t((*n).Owner());
				n = (*n).Next();
				return t;
			}
			return 0;
		};
		inline T* Owner(){
			return (*n).Owner();
		}
		void Insert(NODE<T>& node){
			//ITORが指すノードの前にnodeを追加する
			(*n).Insert(node);
		};
		void Add(NODE<T>& node){
			//ITORが指すノードの後にnodeを追加する
			(*n).Attach(node);
		};
		T* Detach(){
			//ITORが指すノードを除去し、次の要素を指す
			if(n != q){
				T* const t((*n).Owner());
				n = (*n).Next();
				return t;
			}
			//ITORが何も指していなければ0を返す
			return 0;
		};
	private:
		const NODE<T>* const q;
		NODE<T>* n;
		KEY key;
	};
	bool IsThere(){ KEY key(lock); return IsThere(key); };
	T* Get(){ KEY key(lock); return Get(key); };
	void Add(NODE<T>& n){ KEY key(lock); Add(key, n); };
private:
	LOCK lock;
	inline bool IsThere(KEY&){
		return (*this).next != this;
	};
	T* Get(KEY& key){
		if(IsThere(key)){
			T* o((*this).owner);
			(*(*this).next).Detach();
			return o;
		}
		return 0;
	};
	inline void Add(KEY&, NODE<T>& n){
		//リンクは輪になっているのでアンカーであるキューの前は最後
		Insert(n);
	};
};


template<typename T, uint max> class MULTIQUEUE{
public:
	T* Get(uint n = 0){
		KEY key(lock);
		for(uint i(n); i < max; i++){
			if(q[i].IsThere()){
				return q[i].Get();
			}
		}
		return 0;
	};
	void Add(uint index, T& node){
		assert(index < max);
		q[index].Add(node);
	}
private:
	QUEUE<T> q[max];
	LOCK lock;
};


#endif
