/************************************************** SOMETHING VECTOR of POINTER
 *	Copyright (C) 2012- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _VECTOR_
#define _VECTOR_

#include <types.h>
#include <heap.h>
#include <cpu/lock.h>

/** VECTOR：ポインタのベクタテンプレート
 * ベクタのサイズはメモリサイズを上限とする
 */

template<class T, typename INDEX = u32> class VECTOR{
public:
	VECTOR() :
		depth(0),
		entry(0),
		heapIndex(HEAP::GetBlockIndex(sizeof(void*) * 256)){};
	~VECTOR(){
		Free(entry, depth);
	};
	T* operator[](INDEX index){
		KEY<LOCK> key(lock);
		if(!!(index >> (depth * 8))){
			//indexがディレクトリツリーより大きい
			return 0;
		}
		void* p(entry);
		for(uint d(depth); p && d; d--){
			p = ((void**)p)[(index >> (d * 8)) & 255];
		}
		return (T*)p;
	};
	bool Set(INDEX index, T* value){
		KEY<LOCK> key(lock);
		//indexがディレクトリツリーに収まるまでディレクトリツリーを拡大
		void* p(entry);
		while(!!(index >> (depth * 8))){
			void* const p(HEAP::GetByIndex(heapIndex));
			if(!p){
				return false; //確保できなかったのでfalse
			}
			((void**)p)[0] = entry;
			entry = p;
			depth++;
		}

		//indexが指すエントリを示す。パスがなければ作る。
		p = entry;
		for(uint d(depth); 1 < d; d--){
			const uint i((index >> (d * 8)) & 255);
			void* q(((void**)p)[i]);
			if(!q){
				//indexまでのディレクトリパスがないので作る
				q = HEAP::GetByIndex(heapIndex);
				if(!q){
					//確保できなかったのでfalse
					return false;
				}
				((void**)p)[i] = q;
			}
			p = q;
		}
		//エントリに書き込んで正常終了
		((T**)p)[index & 255] = value;
		return true;
	};
private:
	uint depth;
	void* entry;
	LOCK lock;
	const uint heapIndex;
	void Free(void* e, uint d){
		if(d){
			for(uint i(0); i < 256; i++){
				void* const f(((void**)e)[i]);
				if(f){
					Free(f, d - 1);
				}
			}
			HEAP::Release(e, heapIndex);
		}else{
			for(uint i(0); i < 256; i++){
				T* const t(((T**)e)[i]);
				if(t){
					delete t;
				}
			}
			HEAP::Release(e, heapIndex);
		}
	};
};

#endif
