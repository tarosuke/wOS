/**************************************************************ATOMIC COUNTER
 *	Copyright (C) 2012- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _ATOMIC_
#define _ATOMIC_

#include <types.h>


class ATOMIC{
public:
	ATOMIC(u32 initalValue = 0) : counter(initalValue){};
	u32 operator =(u32 newValue){
		return counter = newValue;
	};
	u32 operator++(int){
		return counter++;
	};
	u32 operator++(){
		return ++counter;
	};
	u32 operator--(int){
		return counter--;
	};
	u32 operator--(){
		return --counter;
	};
	u32 operator+=(u32 v){
		return counter += v;
	};
	u32 operator-=(u32 v){
		return counter -= v;
	};
	operator u32(){
		return counter;
	};
private:
	u32 counter;
};


#endif
