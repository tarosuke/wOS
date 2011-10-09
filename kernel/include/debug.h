/*************************************************************** DEBUG CONSOLE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#ifndef _DEBUG_
#define _DEBUG_


#include <config.h>

#if CF_DEBUG_LEVEL
void dputc(char);
void dputs(const char*);
void dprintf(const char*, ...);
#endif

#endif





