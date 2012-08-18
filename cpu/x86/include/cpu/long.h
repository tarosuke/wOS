/*********************************************************** VERY LONG INTEGERs
 *	Copyright (C) 2012- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _LONG_
#define _LONG_


///128bits整数
#ifdef __cplusplus
class u128{
public:
	operator u64(){ return v64[0]; };
	operator u32(){ return v32[0]; };
	u64 Hi64(){ return v64[1]; };
	//論理演算
	operator bool(){ return !!(v64[0] | v64[1]); };
	//ビット演算
	const u128& operator&=(const u128& v){
		v64[0] &= v.v64[0];
		v64[1] &= v.v64[1];
		return *this;
	};
	const u128& operator|=(const u128& v){
		v64[0] |= v.v64[0];
		v64[1] |= v.v64[1];
		return *this;
	};
	const u128& operator>>=(uint v){
		if(64 <= v){
			v64[0] = v64[1]; v64[1] = 0;
			v -= 64;
		}
		if(32 <= v){
			v32[0] = v32[1]; v32[1] = v32[2];
			v32[2] = v32[3]; v32[3] = 0;
			v -= 32;
		}
		if(16 <= v){
			for(uint n(0); n < 7; n++){
				v16[n] = v16[n + 1];
			}
			v16[7] = 0;
			v -= 16;
		}
		if(8 < v){
			for(uint n(0); n < 15; n++){
				v8[n] = v8[n + 1];
			}
			v8[15] = 0;
			v -= 8;
		}
		while(v--){
			//1bitづつシフト
#if CF_IA32
			asm volatile(
				"shr $1, %3;"
				"rcr $1, %2;"
				"rcr $1, %1;"
				"rcr $1, %0;"
				: "=Q"(v32[0]), "=Q"(v32[1]),
					"=Q"(v32[2]), "=Q"(v32[3]));
#endif
#if CF_AMD64
			asm volatile(
				"shr $1, %1;"
				"rcr $1, %0"
				: "=Q"(v64[0]), "=Q"(v64[1]));
#endif
		}
		return *this;
	};
	const u128& operator<<=(uint v){
		if(64 <= v){ v64[1] = v64[0]; v64[0] = 0; v -= 64; }
		if(32 <= v){
			v32[3] = v32[2]; v32[2] = v32[1];
			v32[1] = v32[0]; v32[0] = 0;
			v -= 32;
		}
		if(16 < v){
			for(uint n(7); n; n--){
				v16[n] = v16[n - 1];
			}
			v -= 16;
		}
		if(8 < v){
			for(uint n(15); n; n--){
				v8[n] = v8[n - 1];
			}
			v -= 8;
		}
		while(v--){
			//1bitづつシフト
#if CF_IA32
			asm volatile(
				"shl $1, %0;"
				"rcl $1, %1;"
				"rcl $1, %2;"
				"rcl $1, %3;"
				: "=Q"(v32[0]), "=Q"(v32[1]),
				"=Q"(v32[2]), "=Q"(v32[3]));
#endif
#if CF_AMD64
			asm volatile(
				"shl $1, %0;"
				"rcl $1, %1"
				: "=Q"(v64[0]), "=Q"(v64[1]));
#endif
		}
		return *this;
	};
	//算術演算
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
protected:
	union{
		uchar v8[16];
		u16 v16[8];
		u32 v32[4];
		u64 v64[2];
	};
};

class i128 : public u128{
public:
	operator i64(){ return (i64)v64[0]; };
	operator i32(){ return (i32)v32[0]; };
	const i128& operator>>=(uint v){
		while(v--){
			//1bitづつシフト
			#if CF_IA32
			asm volatile(
				"sar $1, %3;"
				"rcr $1, %2;"
				"rcr $1, %1;"
				"rcr $1, %0;"
				: "=Q"(v32[0]), "=Q"(v32[1]),
					"=Q"(v32[2]), "=Q"(v32[3]));
#endif
#if CF_AMD64
			asm volatile(
				"sar $1, %1;"
				"rcr $1, %0"
				: "=Q"(v64[0]), "=Q"(v64[1]));
#endif
		}
		return *this;
	};
private:
};

#else
typedef struct{ u64 content[2]; } u128;
typedef struct{ u64 content[2]; } i128;
#endif



#endif
