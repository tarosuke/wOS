/*********************************************************************** CLOCK
 *	Copyright (C) 2011- project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _CLOCK_
#define _CLOCK_

#include <types.h>
#include <config.h>
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
	static void SetSystemTimezone(int tz){
		timezone = tz * underSec * 3600;
	};
	static int GetSystemTimezone(){
		return timezone / (underSec * 60);
	};
	static tunit Uptime(){
		tunit t(systemUptime);
		while(t != systemUptime){
			t = systemUptime;
		}
		return t;
	};
	static tunit GetGlobalTime(){
		return Uptime() + baseTime;
	};
	static tunit GetLocalTime(){
		return Uptime() + baseTime + (RTCinLocal ? 0 : timezone);
	};
	static void SetRTCinLocal(){
		if(!RTCinLocal){
			baseTime -= timezone;
		}
	};
	static void SetRTCinGlobal(){
		if(RTCinLocal){
			baseTime += timezone;
		}
	};
	static tunit Convert(const DATE&);
	static DATE Convert(tunit);
	CLOCK(){};
private:
	static const tunit tickTime = 1000000ULL / CF_HZ;
	static tunit systemUptime;		//起動時間
	static tunit baseTime;		//設定時刻-その時の起動時間
	static DOW baseDoW;		//時刻を設定した時の曜日
	static i64 timezone;		//この機械のタイムゾーン
	static bool RTCinLocal;		//RTCはローカルタイム
	static void Tick(){
		systemUptime += tickTime;
		TASK::CRONQUEUE::Tick(systemUptime);
	};
};

#endif
