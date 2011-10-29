/********************************************************************* MESSAGE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _ULIB_MESSAGE_
#define _ULIB_MESSAGE_

#include <userlib/types.h>


typedef struct _MESSAGE{
	void* reserved[3];	// メッセージのキューイングに使う
}MESSAGE;


MESSAGE* WaitNextMessage(tunit);


#endif
