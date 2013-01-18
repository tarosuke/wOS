/*********************************************************** VERY LONG INTEGERs
 *	Copyright (C) 2012- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _LONG_
#define _LONG_


#include <types.h>


///128bits整数
#ifdef __cplusplus
class u128{
public:
	u128(){
		v64[0] =
		v64[1] = 0ULL;
	};
	u128(uint iv){
		v32[0] = iv;
		v32[1] = v32[2] = v32[3] = 0;
	};
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
	u128 operator&(const u128& v){
		u128 r;
		r.v64[0] = v64[0] & v.v64[0];
		r.v64[1] = v64[1] & v.v64[1];
		return r;
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
			//TODO:1bitづつシフト
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
			//TODO:1bitづつシフト
		}
		return *this;
	};
	u128 operator~(){
		u128 r;
		r.v64[0] = ~v64[0];
		r.v64[1] = ~v64[1];
		return r;
	};
	//算術演算
	const u128& operator+=(const u128& v){
		return *this;
	};
	const u128& operator-=(const u128& v){
		return *this;
	};
	u128 operator+(const u128& v){
		return r;
	};
	u128 operator-(const u128& v){
		return r;
	};
	u128 operator*(u128 v){
		u128 r;
		for(u128 m(1); m; v <<= 1, m <<= 1){
			if(m & *this){
				r += v;
			}
		}
		return r;
	};
	const u128& operator*=(const u128& v){
		*this = *this * v;
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
			//TODO:1bitづつシフト
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
