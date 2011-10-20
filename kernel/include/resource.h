/************************************************************ RESOURCE HANDLER
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 *	$Id$
 */

#ifndef _RESOURCE_
#define _RESOURCE_

/** リソースハンドラ
 * ユーザプロセスごとにインデクスされたリソースの管理
 * リソースはPOSIXで言えばファイルハンドルのようなもので、talosではメモリマップや他のサーバへの通信路などになる。
 */

class RESOURCE{
public:
	uint GetID() const { return 0; };
};






#endif

