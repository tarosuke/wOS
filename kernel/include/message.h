/************************************************************ MESSAGING SYSTEM
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _MESSAGE_
#define _MESSAGE_

#include <task.h>
#include <queue.h>


class MESSAGE{
public:
	TASK::PRIORITY priority;
	NODE<MESSAGE> node;
};

#endif
