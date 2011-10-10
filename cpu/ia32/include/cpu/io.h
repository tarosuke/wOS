/******************************************************* IA32 I/O MANIPULATION
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id: 4c6ed08644c37db9de278ae3ebfcd744aa737ae8 $
 */

#ifndef _CPU_IO_
#define _CPU_IO_

// TODO:ストリングI/Oを追加すること
#include <types.h>

// RAW I/O
uchar in8(u16 port){
	uchar r;
	asm volatile("inb %1, %0" : "=a"(r) : "d"(port));
	return r;
}
u16 in16(u16 port){
	u16 r;
	asm volatile("inw %1, %0" : "=a"(r) : "d"(port));
	return r;
}
u32 in32(u16 port){
	u32 r;
	asm volatile("inl %1, %0" : "=a"(r) : "d"(port));
	return r;
}
void out8(u16 port, uchar val){
	asm volatile("outb %0, %1" :: "a"(val), "d"(port));
}
void out16(u16 port, u16 val){
	asm volatile("outw %0, %1" :: "a"(val), "d"(port));
}
void out32(u16 port, u32 val){
	asm volatile("outl %0, %1" :: "a"(val), "d"(port));
}


// STATIC BASED I/O
template<u16 basePort> class SBIO{
 public:
	uchar in8(u16 port){
		uchar r;
		asm volatile("inb %1, %0" : "=a"(r) : "d"(basePort + port));
		return r;
	};
	u16 in16(u16 port){
		u16 r;
		asm volatile("inw %1, %0" : "=a"(r) : "d"(basePort + port));
		return r;
	};
	u32 in32(u16 port){
		u32 r;
		asm volatile("inl %1, %0" : "=a"(r) : "d"(basePort + port));
		return r;
	};
	void out8(u16 port, uchar val){
		asm volatile("outb %0, %1" :: "a"(val), "d"(basePort + port));
	};
	void out16(u16 port, u16 val){
		asm volatile("outw %0, %1" :: "a"(val), "d"(basePort + port));
	};
	void out32(u16 port, u32 val){
		asm volatile("outl %0, %1" :: "a"(val), "d"(basePort + port));
	};
};


// DYNAMIC BASED I/O
class DBIO{
 private:
	const u16 basePort;
 public:
	DBIO(u16 base) : basePort(base){};
	uchar in8(u16 port){
		uchar r;
		asm volatile("inb %1, %0" : "=a"(r) : "d"(basePort + port));
		return r;
	};
	u16 in16(u16 port){
		u16 r;
		asm volatile("inw %1, %0" : "=a"(r) : "d"(basePort + port));
		return r;
	};
	u32 in32(u16 port){
		u32 r;
		asm volatile("inl %1, %0" : "=a"(r) : "d"(basePort + port));
		return r;
	};
	void out8(u16 port, uchar val){
		asm volatile("outb %0, %1" :: "a"(val), "d"(basePort + port));
	};
	void out16(u16 port, u16 val){
		asm volatile("outw %0, %1" :: "a"(val), "d"(basePort + port));
	};
	void out32(u16 port, u32 val){
		asm volatile("outl %0, %1" :: "a"(val), "d"(basePort + port));
	};
};




#endif
