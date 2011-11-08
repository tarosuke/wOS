/************************************************************************ CLOCK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _CLOCK_
#define _CLOCK_

#include <types.h>
#include <task.h>


class CLOCK{
	friend class TIMER;
	CLOCK(CLOCK&);
	void operator=(CLOCK&);
public:
	static const tunit infinite = -1ULL;
	static const tunit continuous = -2ULL;
	static const tunit underSec = 1000000ULL; // 1[tunit]=1[microseconds]
	enum DOW{ sun, mon, tue, wed, thu, fri, sat, dow_unknown };
	class DATE{
	public:
		tunit year;
		uint month;
		uint date;
		DOW dow;
		uint hour;
		uint min;
		uint sec;
		uint usec;
		DATE(){};
		DATE(tunit t){
			*const_cast<DATE*>(this) = CLOCK::Convert(t);
		};
		DATE(uint year, uint month, uint date, DOW dow, uint hour, uint min, uint sec, uint usec) :
			year(year), month(month), date(date), dow(dow), hour(hour), min(min), sec(sec), usec(usec){};
	};
	static void SetGlobalTime(const DATE& d){
		baseTime = Convert(d);
		baseDoW = d.dow;
	};
	static tunit Uptime(){
		tunit u(systemUptime);
		while(u != systemUptime){ u = systemUptime; };
		return u;
	};
	static tunit GetGlobalTime(){
		return Uptime() + baseTime;
	};
	static tunit Convert(const DATE&);
	static DATE Convert(tunit);
	CLOCK(){};
private:
	static const tunit tickTime = 1000000ULL / CF_HZ;
	static tunit systemUptime;	//起動時間
	static tunit baseTime;		//設定時刻-その時の起動時間
	static DOW baseDoW;		//時刻を設定した時の曜日
	static void Tick(){
		systemUptime += tickTime;
		TASK::Cron(systemUptime);
	};
};

#endif
