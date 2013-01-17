/*********************************************************************** QUEUE
 *	Copyright (C) 2011- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _QUEUE_
#define _QUEUE_

#include <types.h>
#include <config.h>
#include <types.h>
#include <lock.h>
#include <debug.h>
#include <key.h>


template<class T> class NODE{
public:
	NODE(T* owner) : owner(owner), next(this), prev(this){};
	//thisをnの前に接続
	void Insert(NODE& n){
		if(prev != next){
			Detach();
		}
		prev = n.prev;
		next = &n;
		n.prev = (*prev).next = this;
	};
	//thisをnの後に接続
	void Attach(NODE& n){
		if(prev != next){
			Detach();
		}
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
			if(!n){
				(*q).Insert(node);
			}else{
				(*n).Insert(node);
			}
		};
		void Add(NODE<T>& node){
			//ITORが指すノードの後にnodeを追加する
			if(!n){
				(*q).Add(node);
			}else{
				(*n).Attach(node);
			}
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
		operator bool(){
			return !!(*n).owner;
		};
	private:
		NODE<T>* const q;
		NODE<T>* n;
		KEY<LOCK> key;
	};
	bool IsThere(){ KEY<LOCK> key(lock); return IsThere(key); };
	T* Get(){ KEY<LOCK> key(lock); return Get(key); };
	void Add(NODE<T>& n){ KEY<LOCK> key(lock); Add(key, n); };
	void Insert(NODE<T>& n){ KEY<LOCK> key(lock); Insert(key, n); };
private:
	LOCK lock;
	inline bool IsThere(KEY<LOCK>&){
		return (*this).next != this;
	};
	T* Get(KEY<LOCK>& key){
		if(IsThere(key)){
			T* o((*this).owner);
			(*(*this).next).Detach();
			return o;
		}
		return 0;
	};
	inline void Add(KEY<LOCK>&, NODE<T>& n){
		//リンクは輪になっているのでアンカーであるキューの前は最後
		NODE<T>::Insert(n);
	};
	inline void Insert(KEY<LOCK>&, NODE<T>& n){
		//リンクは輪になっているのでアンカーであるキューの後は最初
		NODE<T>::Attach(n);
	};
};


template<typename T, uint max> class MULTIQUEUE{
public:
	T* Get(uint e = max){
		assert(e <= max);
		KEY<LOCK> key(lock);
		for(uint i(0); i < e; i++){
			if(q[i].IsThere()){
				return q[i].Get();
			}
		}
		return 0;
	};
	void Add(uint index, NODE<T>& node){
		KEY<LOCK> key(lock);
		assert(index < max);
		q[index].Add(node);
	};
	void Insert(uint index, NODE<T>& node){
		KEY<LOCK> key(lock);
		assert(index < max);
		q[index].Insert(node);
	};
	uint GetMax(){
		KEY<LOCK> key(lock);
		for(uint i(0); i < max; i++){
			if(q[i].IsThere()){
				return i;
			}
		}
		return max;
	};
private:
	QUEUE<T> q[max];
	LOCK lock;
};


#endif
