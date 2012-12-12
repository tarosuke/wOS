/**********************************************************A REFERENCE & BODY
 *	Copyright (C) 2012- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#ifndef _REFERENCE_
#define _REFERENCE_

#include <atomic.h>


/** REFERENCE
 * 参照カウンタ付き参照を実現する。
 * 具体的な使い方はkernel/include/resource.hを参照のこと。
 * ・インターフェイスはREFERENCEの一種
 * ・実体はREFERENCE::BODYの一種
 * ...として作ると機能するように作ってある。
 * また、純粋な参照や実態は作れないようになっている。
 * NOTE:循環参照には対応していない。
 * NOTE:リファレンスそれ自体の排他処理はしていないので同一コンテキストで使うこと。
 * NOTE:タスク間でコピーするときにコピーされる側が取りに行くのは禁忌とする。
 * NOTE:必ずコピー元を管理するコンテキストでpushするようにコピーすること。
 * NOTE:これを守らないと再現性の低いデータ破壊などの症状に悩まされるであろう。
 * NOTE:この実装に限らずね。排他制御も無駄だよ。
 * NOTE:ああそれと、ポインタやC++の参照は作成禁止な。意味ないし。
 */
class REFERENCE{
	//このクラスはポインタ使用禁止
	void operator*();
	void operator&();
	void operator->();
	void* operator new(munit);
	void* operator new[](munit);
//	void operator delete(void*); アホg++がBODY::deleteをこれと混同する
	void operator delete[](void*);
protected:
	class BODY{
	public:
		ATOMIC users;
		virtual ~BODY(){};
		BODY() : users(1){};
	};
public:
	REFERENCE() : body(0){};
	REFERENCE(REFERENCE& org) : body(org.body){
		UseBody();
	};
	REFERENCE(BODY* body) : body(body){ /* usersの初期値は1 */ };
	void operator=(REFERENCE& org){
		ReleaseBody();
		body = org.body;
		UseBody();
	};
	void Discard(){
		ReleaseBody();
		body = 0;
	};
protected:
	BODY* body;
	void UseBody(){
		if(body){
			(*body).users++;
		};
	};
	void ReleaseBody(){
		if(body){
			if(!--(*body).users){
				delete body;
			}
			body = 0;
		}
	};
	virtual ~REFERENCE(){
		ReleaseBody();
	};
	bool IsValid(){ return !!body; };
};


#endif
