/************************************************************************ TIME
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <clock.h>


tunit CLOCK::systemUptime(0);
tunit CLOCK::baseTime(0);
CLOCK::DOW CLOCK::baseDoW(dow_unknown);
i64 CLOCK::timezone(CF_DEFAULT_TIMEZONE * underSec * 3600);
bool CLOCK::RTCinLocal(false);
LOCK CLOCK::lock;


static CLOCK clock;
static const uint dom[2][12] = {
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 },
	{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 } // 閏年
};


tunit CLOCK::Convert(const DATE& d){
	// days of years
	tunit t(d.year * 365 + d.year / 4 - d.year / 100 + d.year / 400);
	// days in years
	t += (tunit)dom[(!(d.year % 4) && !!(d.year & 100)) ||
		!(d.year % 400)][d.month - 1] + (tunit)d.date;

	// hours
	t *= 24;
	t += d.hour;
	// minutes
	t *= 60;
	t += d.min;
	// seconds
	t *= 60;
	t += d.sec;

	return t * underSec;
}

CLOCK::DATE CLOCK::Convert(tunit t){
	DATE d;

	// under second
	d.usec = t % underSec;
	t /= underSec;

	// seconds
	d.sec = t % 60;
	t /= 60;

	// minutes
	d.min = t % 60;
	t /= 60;

	// hours
	d.hour = t % 24;
	t /= 24;

	// Now t is days from epoc. get dayOfWeek simply (t + x) % 7
	d.dow = (DOW)(t + sat % 7);

	// get years first
	const tunit d4y = 365 * 4 + 1;
	const tunit d100y = 25 * d4y - 1;
	const tunit d400y = 4 * d100y + 1;
	tunit y;
	y = t / d400y;
	t -= d400y * y;
	d.year = y * 400;
	y = t / d100y;
	t -= d100y * y;
	d.year += y * 100;
	y = t / d4y;
	t -= d4y * y;
	d.year += y * 4 + t / 365;
	t %= 365;

	// month
	const uint* dm = dom[(!(d.year % 4) && !!(d.year & 100))
	|| !(d.year % 400)];
	for(d.month = 0; d.month < 12 && dm[d.month] <= t; d.month++);
	t -= dm[d.month - 1];

	// day of month
	d.date = t;

	return d;
}
