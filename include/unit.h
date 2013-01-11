/************************************************************* UNIT DEFINITION
 *	Copyright (C) 2013- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */

#ifndef _UNIT_
#define _UNIT_

#include <types.h>
#include <util.h>
#include <cpu/long.h>


template<typename T> class UNIT{
public:
	const UNIT& operator=(T v){
		body = v;
		return *this;
	};
	inline operator bool(){
		return !!body;
	};
	inline operator T() const{
		return body;
	};
	inline T operator+(T o) const{
		return body + o;
	};
	inline const T& operator+=(T o){
		body += o;
		return *this;
	};
	inline T operator-(T o) const{
		return body - o;
	};
	inline const T& operator-=(T o){
		body -= o;
		return *this;
	};
	inline const T& operator++(){
		return ++body;
	};
	inline const T& operator++(int){
		return body++;
	};
	inline const T& operator--(){
		return --body;
	};
	inline const T& operator--(int){
		return body--;
	};
	inline bool operator<(const UNIT& o) const{
		return body < o.body;
	};
	inline bool operator<=(const UNIT& o) const{
		return body <= o.body;
	};
	inline bool operator>(const UNIT& o) const{
		return body > o.body;
	};
	inline bool operator>=(const UNIT& o) const{
		return body >= o.body;
	};
	inline bool operator==(const UNIT& o) const{
		return body == o.body;
	};
protected:
	T body;
	UNIT(T v) : body(v){};
	UNIT(const UNIT& o) : body(o.body){};
	UNIT();
};


///// 仮想メモリの単位
class MUNIT : public UNIT<munit>{
public:
	MUNIT(void* p) : UNIT((munit)p){};
	operator void*() const{
		return (void*)body;
	};
};

///// 実メモリの単位
class RUNIT : public UNIT<runit>{
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

///// ページの単位
class PUNIT : public UNIT<punit>{
public:
	PUNIT(punit p) : UNIT(p){};
	PUNIT(const MUNIT& m) : UNIT(m / PAGESIZE){};
	PUNIT(void* m) : UNIT((munit)m / PAGESIZE){};
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


///// ストレージの単位
class SUNIT : public UNIT<u128>{
public:
};


#endif
