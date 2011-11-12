/******************************************************* IA32 I/O MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _CPU_IO_
#define _CPU_IO_

#include <types.h>

// RAW I/O
static inline uchar in8(u16 port){
	uchar r;
	asm volatile("inb %%dx, %%al" : "=a"(r) : "d"(port));
	return r;
}
static inline void in8s(uchar* dest, u16 port, munit size){
	asm volatile("cld; rep insb" :: "D"(dest), "d"(port), "c"(size));
}
static inline u16 in16(u16 port){
	u16 r;
	asm volatile("inw %%dx, %%ax" : "=a"(r) : "d"(port));
	return r;
}
static inline void in16s(u16* dest, u16 port, munit size){
	asm volatile("cld; rep insw" :: "D"(dest), "d"(port), "c"(size));
}
static inline u32 in32(u16 port){
	u32 r;
	asm volatile("inl %%dx, %%eax" : "=a"(r) : "d"(port));
	return r;
}
static inline void in32s(u32* dest, u16 port, munit size){
	asm volatile("cld; rep insl" :: "D"(dest), "d"(port), "c"(size));
}
static inline void out8(u16 port, uchar val){
	asm volatile("outb %%al, %%dx" :: "a"(val), "d"(port));
}
static inline void out8s(const uchar* src, u16 port, munit size){
	asm volatile("cld; rep outsb" :: "S"(src), "d"(port), "c"(size));
}
static inline void out16(u16 port, u16 val){
	asm volatile("outw %%ax, %%dx" :: "a"(val), "d"(port));
}
static inline void out16s(const u16* src, u16 port, munit size){
	asm volatile("cld; rep outsw" :: "S"(src), "d"(port), "c"(size));
}
static inline void out32(u16 port, u32 val){
	asm volatile("outl %%eax, %%dx" :: "a"(val), "d"(port));
}
static inline void out32s(const u32* src, u16 port, munit size){
	asm volatile("cld; rep outsl" :: "S"(src), "d"(port), "c"(size));
}


// STATIC BASED I/O
template<u16 basePort> class SBIO{
 public:
	static inline uchar in8(u16 offset){ return ::in8(basePort + offset); };
	static inline u16 in16(u16 offset){ return ::in16(basePort + offset); };
	static inline u32 in32(u16 offset){ return ::in32(basePort + offset); };
	static inline void out8(u16 offset, uchar val){ ::out8(basePort + offset, val); };
	static inline void out16(u16 offset, u16 val){ ::out16(basePort + offset, val); };
	static inline void out32(u16 offset, u32 val){ ::out32(basePort + offset, val); };
	static inline void in8s(uchar* dest, u16 offset, munit size){ ::in8s(dest, basePort + offset, size); };
	static inline void in16s(u16* dest, u16 offset, munit size){ ::in16s(dest, basePort + offset, size); };
	static inline void in32s(u32* dest, u16 offset, munit size){ ::in32s(dest, basePort + offset, size); };
};


// DYNAMIC BASED I/O
class DBIO{
 private:
	const u16 basePort;
 public:
	DBIO(u16 base) : basePort(base){};
	inline uchar in8(u16 offset){ return ::in8(basePort + offset); };
	inline u16 in16(u16 offset){ return ::in16(basePort + offset); };
	inline u32 in32(u16 offset){ return ::in32(basePort + offset); };
	inline void out8(u16 offset, uchar val){ ::out8(basePort + offset, val); };
	inline void out16(u16 offset, u16 val){ ::out16(basePort + offset, val); };
	inline void out32(u16 offset, u32 val){ ::out32(basePort + offset, val); };
	inline void in8s(uchar* dest, u16 offset, munit size){ ::in8s(dest, basePort + offset, size); };
	inline void in16s(u16* dest, u16 offset, munit size){ ::in16s(dest, basePort + offset, size); };
	inline void in32s(u32* dest, u16 offset, munit size){ ::in32s(dest, basePort + offset, size); };
};




#endif
