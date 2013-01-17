/********************************************************** STIRNG MANIPULATOR
	Copyright (C) 2004- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
*/

#ifndef _STRING_
#define _STRING_

#include <types.h>
#include <config.h>

#define memset(d,s,l) _memset((d),(s),(l))
#define memcmp(d,s,l) _memcmp((d),(s),(l))
#define memmove(d,s,l) _memmove((d),(s),(l))
#define strcmp(d,s) _strcmp((d),(s))



// TODO:☆あとでインラインアセンブラ記述に置き換える
static inline void* memcpy(void* d, const void* s, unsigned long len){
	char* dp;
	const char* sp;
	for(dp = (char*)d, sp = (const char*)s; len--; *dp++ = *sp++);
	return d;
}

static __inline void* _memmove(void* d, void* s, unsigned long len){
	if(s > d){
		memcpy(d, s, len);
	}else{
		char* dp;
		const char* sp;
		for(dp = (char*)d, sp = (const char*)s, d = (void*)(dp + len); len--; *dp-- = *sp--);
	}
	return d;
}

static __inline void* _memset(void* d, uchar v, munit len){
#if 1
	uchar* dp = (uchar*)d;
	while(len--){
		*dp++ = v;
	}
	return dp;
#else
	asm volatile(
		"mov %0, %%edi;"
		"mov %1, %%al;"
		"mov %2, %%ecx;"
		"1: rep stosb;"
		"test %%ecx, %%ecx; jnz 1b"
		:: "m"(d), "g"(v), "g"(len) : "eax", "edi", "ecx");
#endif
	return (void*)((munit)d + len);
}

static __inline int _strcmp(const char* s0, const char* s1){
	for(; *s0 && (*s0 == *s1); s0++, s1++);
	return (int)(uchar)*s0 - (int)(uchar)*s1;
}

static __inline int _memcmp(const void* d, const void* s, munit len){
	const uchar* de = (const uchar*)d;
	const uchar* sr = (const uchar*)s;
	for(; len; len--, de++, sr++){
		if(*de != *sr){
			return (int)*sr - (int)*de;
		}
	}
	return 0;
}


static __inline void memset2(void* d, u16 v, munit len){
	u16* dp = (u16*)d;

	// 連続転送の前に端数があれば先に転送
	if(((munit)dp & 0x01) && len){
		*(uchar*)dp = (uchar)(v & 0xff);
		v = BE16(v);
		dp = (u16*)((munit)dp + 1);
		len--;
	}

	// 連続転送
	for(; 1 < len; len -= 2){
		*dp++ = v;
	}

	// 残りがあればそれを転送
	if(len){
		*(uchar*)dp = (uchar)(v & 0xff);
	}
}

// static __inline void memset3(void* d, u32 v, munit len){
// 	char temp[12];
//
// 	/***** データの4n化 */
// 	*(u32*)temp = v;
// 	memcpy(temp + 3, temp, 3);
// 	memcpy(temp + 6, temp, 6);
//
// 	/***** 12bytesごとに転送 */
// 	for(; 12 <= len; len -= 12, d = (void*)((munit)d + 12)){
// 		memcpy(d, temp, 12);
// 	}
//
// 	/***** 残りを転送 */
// 	if(len){
// 		memcpy(d, temp, len);
// 	}
// }

static __inline void memset4(void* d, u32 v, munit len){
	for(u32* p((u32*)d); 4 < len; *p++ = v, len -= 4);
	len &= 3;
	if(len){
		memcpy(d, &v, len);
	}
}


#endif

