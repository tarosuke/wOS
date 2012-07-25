/**************************************************************ATOMIC COUNTER
 *	Copyright (C) 2012- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _ATOMIC_
#define _ATOMIC_

#include <types.h>
#include <lock.h>


class ATOMIC{
public:
	ATOMIC(u32 initalValue = 0) : counter(initalValue){};
	u32 operator =(u32 newValue){
		counter = newValue;
	};
	u32 operator++(int){
		u32 orgValue;
		asm volatile(
			"mov %0, %%eax;"
			"1: mov %%eax, %%edx;"
			"inc %%edx;"
			"lock cmpxchg %%edx, %0;"
			"jnz 1b"
			: "=m"(counter), "=a"(orgValue) :: "edx");
		)
		return orgValue;
	};
	u32 operator++(){
		u32 newValue;
		asm volatile(
			"mov %0, %%eax;"
			"1: mov %%eax, %%edx;"
			"inc %%edx;"
			"lock cmpxchg %%edx, %0;"
			"jnz 1b"
			: "=m"(counter), "=d"(newValue));
		)
		return newValue;
	};
	u32 operator--(int){
		u32 orgValue;
		asm volatile(
			"mov %0, %%eax;"
			"1: mov %%eax, %%edx;"
			"decc %%edx;"
			"lock cmpxchg %%edx, %0;"
			"jnz 1b"
			: "=m"(counter), "=a"(orgValue) :: "edx");
		)
		return orgValue;
	};
	u32 operator--(){
		u32 newValue;
		asm volatile(
			"mov %0, %%eax;"
			"1: mov %%eax, %%edx;"
			"dec %%edx;"
			"lock cmpxchg %%edx, %0;"
			"jnz 1b"
			: "=m"(counter), "=d"(newValue));
		)
		return newValue;
	};
	u32 operator+=(u32 v){
		u32 newValue;
		asm volatile(
			"mov %0, %%eax;"
			"1: mov %%eax, %%edx;"
			"add %2, %%edx;"
			"lock cmpxchg %%edx, %0;"
			"jnz 1b"
			: "=m"(counter), "=d"(newValue) : "r"(v));
		)
		return newValue;
	};
	u32 operator-=(u32 v){
		u32 newValue;
		asm volatile(
			"mov %0, %%eax;"
			"1: mov %%eax, %%edx;"
			"sub %2, %%edx;"
			"lock cmpxchg %%edx, %0;"
			"jnz 1b"
			: "=m"(counter), "=d"(newValue) : "r"(v));
		)
		return newValue;
	};
private:
	u32 counter;
	LOCK lock;
};



#endif
