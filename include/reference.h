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
 * ・インターフェイスはREFERENCEの一種
 * ・抽象クラスはREFERENCE::BODYの一種
 * ・具象クラスは抽象クラスの一種
 * ...として作ると機能するように作ってある。
 * NOTE:循環参照には対応していない。
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
