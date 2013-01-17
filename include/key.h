/************************************************************************* KEY
 *	Copyright (C) project wOS (https://github.com/tarosuke/wOS)
 *	check LICENSE.txt. If you don't have the file, contact us.
 */


#ifndef _LOCK_
#define _LOCK_


template<class L> class KEY{
public:
	KEY(L& lock) : lock(lock){ lock.Lock(); };
	~KEY(){ lock.Unlock(); };
private:
	L& lock;
};

#endif
