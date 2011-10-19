/***************************************************************** KERNEL CORE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, contact us.
 *	$Id$
 */


#ifndef _LOCK_
#define _LOCK_


#include <config.h>
#include <cpu/lock.h>


class KEY{
	KEY();
	KEY(KEY&);
private:
	LOCK& lock;
public:
	KEY(LOCK& lock) :lock(lock){ lock.Lock(); };
	~KEY(){ lock.Unlock(); };
};

#if 0
class RKEY{
	RKEY(RKEY&);
private:
	static LOCK lock;
public:
	RKEY(){ lock.Lock(); };
	~RKEY(){ lock.Unlock(); };
};
#endif

#endif

