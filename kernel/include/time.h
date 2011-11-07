/************************************************************************ TIME
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _TIME_
#define _TIME_

#include <types.h>


class TIME{
public:
	static const tunit infinite = (tunit)-1LL;
	static const tunit continuous = (tunit)-2LL;
	struct HMS{
		tunit hour;
		uint min;
		uint sec;
		uint usec;
	};
	struct DATE{
		tunit year;
		uint month;
		uint date;
		uint dayOfWeek;
		uint hour;
		uint min;
		uint sec;
		uint usec;
	};
	TIME(tunit origin = 0, uint edow = 0) : time(origin), edow(edow){};
	TIME(TIME& org) : time(org.Get()), edow(org.edow){};
	void operator=(TIME& org){
		time = org.Get();
	};
	tunit Get() const{
		tunit now(time);
		while(now != time){ now = time; };
		return now;
	};
	tunit Get(DATE&) const;
	tunit Get(HMS&) const;
	inline void operator+=(tunit n){
		time += n;
	};
private:
	tunit time;
	const uint edow;
};

#endif
