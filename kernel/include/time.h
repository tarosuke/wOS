/************************************************************************ TIME
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _TIME_
#define _TIME_

#include <types.h>


class TIME{
public:
	static const tunit infinite = -1ULL;
	static const tunit continuous = -2ULL;
	static const tunit underSec = 1000000ULL; // 1[tunit]=1[microseconds]
	class HMS{
	public:
		tunit hour;
		uint min;
		uint sec;
		uint usec;
		HMS(TIME&);
		HMS(uint hour, uint min, uint sec, uint usec) : hour(hour), min(min), sec(sec), usec(usec){};
	};
	class DATE{
	public:
		tunit year;
		uint month;
		uint date;
		int dow;
		uint hour;
		uint min;
		uint sec;
		uint usec;
		DATE(TIME&);
		DATE(uint year, uint month, uint date, int dow, uint hour, uint min, uint sec, uint usec) :
			year(year), month(month), date(date), dow(dow), hour(hour), min(min), sec(sec), usec(usec){};
	};
	TIME(tunit origin = 0, uint edow = 0) : time(0), epoc(origin), edow(edow){};
	TIME(TIME& org) : time(org.GetRaw()), epoc(org.epoc), edow(org.edow){};
	void operator=(TIME& org){
		time = org.GetRaw();
	};
	void operator=(DATE&);	//現在日時、時刻設定
	void operator=(HMS&);		//現在時刻設定
	inline tunit GetRaw() const{
		tunit now(time);
		while(now != time){ now = time; };
		return now;
	};
	inline tunit Get() const{
		return GetRaw() + epoc;
	};
	inline void operator+=(tunit n){
		time += n;
	};
private:
	tunit time;	//起動時間
	tunit epoc;	//時刻設定時の基準時刻からの時間-時刻設定時のuptime(time + epocが現在時刻)
	uint edow;	//epocDayOfWeek：epocの日の曜日(現在時刻から逆算するが、設定時にdowが負数ならそのまま)
};

#endif
