/************************************************************************ TIME
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <time.h>


tunit TIME::Get(DATE& date) const{
	static const uint daysOfMonth[2][12] = {
		{ 0, 31, 59, 90,120,151,181,212,243,273,304,334 },
		{ 0, 31, 60, 91,121,152,182,213,244,274,305,335 } };
	tunit now(Get());
	tunit days(now / (24ULL * 3600 * 1000000));

	date.dayOfWeek = (days + edow) / 7;

	//年
	tunit n;
	n = days / (365 * 400 + 96);
	date.year = n * 400;
	days -= n * (365 * 400 + 96);
	n = days / (365 * 100 + 24);
	date.year += n * 100;
	days -= n * (365 * 100 + 24);
	n = days / (365 * 4 + 1);
	date.year += n * 4;
	days -= n * (365 * 4 + 1);

	//月
	const uint leap(
		!(date.year % 4) &&
		(!!(date.year % 100) || !(date.year % 400)));
	for(uint m(0); m < 12; m++){
		if(days < daysOfMonth[leap][m]){
			date.month = m;
			days -= daysOfMonth[leap][m];
			break;
		}
	}

	//日
	date.date = days;

	//時分秒
	date.hour = (now / 3600000000ULL) % 24;
	date.min = (now / 60000000ULL) % 60;
	date.sec = (now / 1000000) % 60;
	date.usec = now % 1000000;

	//整形
	date.year += 1;
	date.month += 1;

	return now;
}

tunit TIME::Get(HMS& hms) const{
	tunit now(Get());
	hms.hour = now / 3600000000ULL;
	hms.min = (now / 60000000ULL) % 60;
	hms.sec = (now / 1000000) % 60;
	hms.usec = now % 1000000;
	return now;
}