/********************************************************************* MESSAGE
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <userlib/message.h>
#include <userlib/task.h>


MESSAGE* __WaitNextMessage(tunit uptime){
	// TODO:taskHeadのrxを処理済みにして。uptimeの設定
	asm volatile("int $0x30");
	return taskHead.rx;
}
