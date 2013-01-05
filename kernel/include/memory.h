/*********************************************************** MEMORY DEFINITION
 *	Copyright (C) 2013- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#ifndef _MEMORY_
#define _MEMORY_

#include <types.h>
#include <util.h>


template<typename T> class MEMORYUNIT{
public:
	MEMORYUNIT(T v) : body(v){};
	const MEMORYUNIT& operator=(T v){
		body = v;
		return *this;
	};
	operator T() const{
		return body;
	};
	T operator+(T o) const{
		return body + o;
	};
	const T& operator+=(T o){
		body += o;
		return *this;
	};
	T operator-(T o) const{
		return body - o;
	};
	const T& operator-=(T o){
		body -= o;
		return *this;
	};
protected:
	T body;
};

class MUNIT : public MEMORYUNIT<munit>{
public:
	MUNIT(void* p) : MEMORYUNIT((munit)p){};
	operator void*() const{
		return (void*)body;
	};
};

class RUNIT : public MEMORYUNIT<runit>{
public:
	runit operator |(runit o) const{
		return body | o;
	};
	const runit operator |=(runit o){
		body |= o;
		return *this;
	};
	runit operator &(runit o) const{
		return body & o;
	};
	const runit operator &=(runit o){
		body &= o;
		return *this;
	};
};

class PUNIT : public MEMORYUNIT<punit>{
public:
	const PUNIT& operator=(const MUNIT& m){
		const munit mv(m);
		body = mv / PAGESIZE;
		return *this;
	};
	const PUNIT& operator=(const RUNIT& r){
		const runit rv(r);
		body = rv / PAGESIZE;
		return *this;
	};
};


#endif
