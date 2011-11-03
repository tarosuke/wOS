/********************************************************* IA32 DEPENDED TYPES
	Copyright (C) 2006- project talos (http://talos-kernel.sf.net/)
	check LICENSE.txt. If you don't have the file, mail us.
*/


#ifndef _CPUTYPES_
#define _CPUTYPES_

/// size-aware types
typedef unsigned char uchar;
typedef unsigned uint;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef long long i64;
typedef unsigned long long u64;
typedef struct{ u64 content[2]; } i128;
typedef struct{ u64 content[2]; } u128;


/// position & size of virtual memory
typedef u32 munit;

/// position & size of real memory
#if CF_PAE
typedef u64 runit;
#else
typedef u32 runit;
#endif

/// position & numbers of page
typedef u32 punit;

const munit PAGESIZE(4096);

/// UUID
typedef u128 uuid;


/// endian-aware types
typedef u16 be16;
typedef u16 le16;
typedef u32 be32;
typedef u32 le32;
#define BE16(org) ( (((org) >> 8) & 0xff) | (((org) << 8) & 0xff00) )
#define LE16(org) (org)
#define BE32(org) ((BE16((org) & 0xffff) << 16) | BE16(((org) >> 16) & 0xffff))
#define LE32(org) (org)



/// biggest n of v < 2^n
static inline uint LN2(uint v){
#if 1
	uint n;
	uint m;
	for(m = 0x80000000, n = 31; !(m & v); m >>= 1, n--);
	return (m == v) ? n : n + 1;
#else
	uint n;
	asm volatile("bsr %1, %0; jnz 1f; xor %0, %0; 1:" : "=r"(n) : "r"(v));
	return (1U << n) < v ? n + 1 : n;
#endif
}

static inline u32 bcd2bin(uint v){
	u32 r;
	int s;
	for(r = 0, s = 28; 0 <= s; s -= 4, r *= 10){
		r += (v >> s) & 15;
	}
	return r / 10;
}



#endif
