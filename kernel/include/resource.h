/********************************************************** RESOURCE & HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _RESOURCE_
#define _RESOURCE_

#include <types.h>
#include <lock.h>


/// MAPやソケットなどの抽象リソース
class RESOURCE{
	friend class HANDLER;
public:
	RESOURCE() : users(0){};
	virtual ~RESOURCE(){};
	virtual void SystemRequest(void* message) = 0;
protected:
private:
	void Use();
	void Release();
	LOCK lock;
	uint users;
};

/// ユーザプロセスからリソースへアクセスするための抽象ハンドラ
class HANDLER{
public:
	HANDLER(RESOURCE& org) : org(org){ org.Use(); }
	virtual ~HANDLER(){ org.Release(); };
	uint GetID() const { return 0; };
private:
	RESOURCE& org;
};




#endif
