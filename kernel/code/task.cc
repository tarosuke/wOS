/************************************************************************ TASK
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <task.h>


TASK::TASKQUEUE TASK::readyQueue;


TASK::TASK() : owner(0){
}