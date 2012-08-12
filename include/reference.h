/**********************************************************A REFERENCE & BODY
 *	Copyright (C) 2012- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _REFERENCE_
#define _REFERENCE_

#include <atomic.h>

/** REFERENCE
 * 参照カウンタ付き参照を実現する。
 * 具体的な使い方はkernel/include/map.hを参照のこと。
 * 1.抽象クラスでREFERENCEとREFERENCE::BODYの子クラスを作る。
 * 2.具象クラスでREFERENCE:;BODYの孫クラスを作る。
 * 3.REFERENCEの子クラスの構築子に具象クラスをnewしたものを与える。
 * 抽象クラスを使わない場合は抽象クラス=具象クラスとする。
 */
class REFERENCE{
	REFERENCE();
public:
	class BODY{
		friend class REFERENCE;
	public:
		virtual ~BODY();
	protected:
		BODY() : users(1){};
	private:
		ATOMIC users;
	};
	REFERENCE(BODY* body) : body(body){};
	REFERENCE(REFERENCE& ref) : body(ref.body){
		(*body).users++;
	};
	virtual ~REFERENCE(){
		if(!--(*body).users){
			delete body;
		}
	}
	bool IsValid(){ return !!body; };
private:
	BODY* body;
};


#endif
