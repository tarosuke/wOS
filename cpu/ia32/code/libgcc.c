/********************************************************** ALTERNATIVE libgcc
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */


typedef unsigned long long u64;

u64 __umoddi3(u64 n, u64 d){
	u64 b = 1;

	//開始ビット設定
	for(; !(d & (1ULL << 63)); d <<=1, b <<= 1);

	//除算
	for(; b && n; d >>= 1, b >>= 1){
		if(d <= n){
			n -= d;
		}
	}

	return n;
}

u64 __udivdi3(u64 n, u64 d){
	u64 b = 1;
	u64 r = 0;

	//開始ビット設定
	for(; !(d & (1ULL << 63)); d <<=1, b <<= 1);

	//除算
	for(; b && n; d >>= 1, b >>= 1){
		if(d <= n){
			n -= d;
			r |= b;
		}
	}

	return r;
}

