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
#if CF_IA32
typedef long long i64;
typedef unsigned long long u64;
#endif
#if CF_AMD64
typedef long i64;
typedef unsigned long u64;
#endif


///128bits整数
typedef struct{ u64 content[2]; } i128;
#ifdef __cplusplus
class u128{
public:
	operator u64(){ return v64[0]; };
	operator u32(){ return v32[0]; };
	const u128& operator+=(const u128& v){
#if CF_IA32
		asm volatile(
			"add %4, %0;"
			"adc %5, %1;"
			"adc %6, %2;"
			"adc %7, %3;"
			: "=Q"(v32[0]), "=Q"(v32[1]),
				"=Q"(v32[2]), "=Q"(v32[3])
			: "g"(v.v32[0]), "g"(v.v32[1]),
				"g"(v.v32[2]), "g"(v.v32[3]));
#endif
#if CF_AMD64
		asm volatile(
			"add %2, %0;"
			"adc %3, %1"
			: "=Q"(v64[0]), "=Q"(v64[1])
			: "g"(v.v64[0]), "g"(v.v64[1]));
#endif
		return *this;
	};
	const u128& operator-=(const u128& v){
#if CF_IA32
		asm volatile(
			"sub %4, %0;"
			"sbb %5, %1;"
			"sbb %6, %2;"
			"sbb %7, %3;"
			: "=Q"(v32[0]), "=Q"(v32[1]),
				"=Q"(v32[2]), "=Q"(v32[3])
			: "g"(v.v32[0]), "g"(v.v32[1]),
				"g"(v.v32[2]), "g"(v.v32[3]));
#endif
#if CF_AMD64
		asm volatile(
			"sub %2, %0;"
			"sbb %3, %1"
			: "=Q"(v64[0]), "=Q"(v64[1])
			: "g"(v.v64[0]), "g"(v.v64[1]));
#endif
		return *this;
	};
private:
	union{
		u32 v32[4];
		u64 v64[2];
	};
};
#else
typedef struct{ u64 content[2]; } u128;
#endif

/// position & size of virtual memory
#if CF_IA32
typedef u32 munit;
#endif
#if CF_AMD64
typedef u64 munit;
#endif

/// position & size of real memory
#if CF_PAE || CF_AMD64
typedef u64 runit;
#else
typedef u32 runit;
#endif

/// position & numbers of page
#if CF_IA32
typedef u32 punit;
#endif
#if CF_AMD64
typedef u64 punit;
#endif

#define PAGESIZE (4096)

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
