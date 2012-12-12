/************************************************************************ HASH
 *	Copyright (C) 2012- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _HASH_
#define _HASH_

#include <types.h>
#include <queue.h>
#include <config.h>


template<class T, typename M, uint width = 256> class HASH{
public:
	T* operator[](const M& condition){
		/// Hashは絶対にマッチしなければならない要素のみで計算
		/// MatchScoreは0が完全不一致
		QUEUE<T>& q(table[T::Hash(condition) % width]);
		T* ht(0);
		T* t(0);
		uint score(0);
		for(QUEUE<T>::ITOR i(q); (t = (*i.Owner()))i; i++){
			const uint s(t.MatchScore(condition));
			if(score < s){
				ht = t;
				score = s;
			}
		}
		return ht;
	};
private:
	QUEUE<T> table[width];
};


#endif
