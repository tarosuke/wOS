/************************************************************************ TIME
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _TIME_
#define _TIME_


class TIME{
	UNDEFAULT(TIME);
public:
	static const tunit infinite = (tunit)-1LL;
	static const tunit continuous = (tunit)-2LL;
	struct HMS{
		tunit hour;
		uint min;
		uint sec;
		uint usec;
	};
	TIME(tunit origin = 0) : time(origin){};
private:
	tunit time;
};

#endif
