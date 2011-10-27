/************************************************************************ TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _TASK_
#define _TASK_


#include <types.h>
#include <config.h>
#include <queue.h>

extern "C"{
#include <userlib/task.h>
#include <userlib/message.h>
}



class TASK{
public:
private:
	QUEUE<MESSAGE> in;
};


#endif

