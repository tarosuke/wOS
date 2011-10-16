/*************************************************************** DEBUG CONSOLE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#ifndef _DEBUG_
#define _DEBUG_


#include <config.h>


#define INDENT "\001"
#define UNINDENT "\002"

#if CF_DEBUG_LEVEL
void __AssertionFailed(const char* file, const unsigned line);
#if 5 <= CF_DEBUG_LEVEL
void __AssertionPassed(const char* file, const unsigned line);
#else
#define __AssertionPassed(a,b)
#endif
void dputc(char);
void dputs(const char*);
void dprintf(const char*, ...);
void Panic(const char*);
#define assert(c) {\
	if(!(c)){\
		__AssertionFailed(__FILE__, __LINE__);\
	}else{\
		__AssertionPassed(__FILE__, __LINE__);\
	}\
}
#define ORIGIN "\033[0m"
#define LIGHTWHITE "\033[1;37m"
#define LIGHTRED "\033[1;31m"
#else
#define dputc(c)
#define dputs(s)
#define dprintf(f,...)
#define Panic(s)
#define assert(c);
#endif

#if 2 <= CF_DEBUG_LEVEL
#define eputc(c) dputc((c))
#define eputs(s) dputs((s))
#define eprintf(f, ...) dprintf(f, __VA_ARGS__)
#else
#define eputc(c)
#define eputs(s)
#define eprintf(f,...)
#endif

#if 3 <= CF_DEBUG_LEVEL
#define fputc(c) dputc((c))
#define fputs(s) dputs((s))
#define fprintf(f, ...) dprintf(f, __VA_ARGS__)
#else
#define fputc(c)
#define fputs(s)
#define fprintf(f,...)
#endif

#if 4 <= CF_DEBUG_LEVEL
#define gputc(c) dputc((c))
#define gputs(s) dputs((s))
#define gprintf(f, ...) dprintf(f, __VA_ARGS__)
#else
#define gputc(c)
#define gputs(s)
#define gprintf(f,...)
#endif

#if 5 <= CF_DEBUG_LEVEL
#define hputc(c) dputc((c))
#define hputs(s) dputs((s))
#define hprintf(f, ...) dprintf(f, __VA_ARGS__)
#else
#define hputc(c)
#define hputs(s)
#define hprintf(f,...)
#endif

#endif





