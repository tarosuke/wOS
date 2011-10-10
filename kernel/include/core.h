


#ifndef _COCE_
#define _COCE_


#include <types.h>


class CORE{
private:
	static uchar kernelStacks[][4096];
public:
	static void Entry(uint)__attribute__((noreturn));
};



#endif


