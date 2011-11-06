/***************************************************************** KERNEL CORE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, contact us.
 *	$Id$
 */


#ifndef _LOCK_
#define _LOCK_

#include <types.h>
#include <config.h>
#include <cpu/lock.h>


class KEY{
	KEY(KEY&);
	void operator=(KEY&);
private:
	LOCK& lock;
public:
	KEY(LOCK& lock) :lock(lock){ lock.Lock(); };
	~KEY(){ lock.Unlock(); };
};

class IKEY{
	IKEY(IKEY&);
	void operator=(IKEY&);
private:
	ILOCK lock;
public:
	IKEY(){ lock.Lock(); };
	~IKEY(){ lock.Unlock(); };
};

#endif

