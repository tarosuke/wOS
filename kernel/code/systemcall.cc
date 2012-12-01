/******************************************************* SYSTEMCALL ENTRYPOINT
 *	Copyright (C) 2012- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <config.h>
#include <types.h>
#include <debug.h>


extern "C"{
#include <systemcall.h>
	/** システムコールのエントリポイント
	 * 引数はシステムコール用パラメタブロック
	 */
	void __SYSTEMCALL_HANDLER(SYSTEMCALL_PARAMS* params){
		switch((*params).request){
		case SYSTEMCALL_PARAMS::sys_noop :
			(*params).result = SYSTEMCALL_PARAMS::rc_fine;
			break;
		default:
			(*params).result =
				SYSTEMCALL_PARAMS::rc_request_not_available;
			break;
		}
	}
}


