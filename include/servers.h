/************************************************************* SERVERTYPE LIST
 *	Copyright (C) 2011- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */


#ifndef _SERVERS_
#define _SERVERS_


#include <types.h>



/** サーバの説明
 * 各々のベンダが勝手にプロトコルを決めていいようにするためにUUIDを識別に使う
 * UUIDはgenuuidとかでその場で作れる
 * これは実装のIDではなく、プロトコルのIDである
 * 従って、中身がどんなに変わろうともプロトコルがアッパーコンパチである限りは同じID
 */


typedef struct{
	uuid protocol;
	VERSION protocolVersion;
	uuid vender;
	VERSION productVersion;
}SERVER_DEF;



/** talos開発チームのvenderID
 * ベンダもUUIDで識別される
 * 文字列だと処理が面倒なのでUUIDを使う
 */
static const char* VENDER_talos = "3ae3d690-13ad-48a4-bca4-ae4a3e6cbd25";





/** DISPLAY SERVER
 * デバイスドライバ検索により起動される。
 * 起動すると自身への接続と画面のマップを引数にDESKTOP SERVERを起動する。
 * また、リクエストにより画面解像度の変更やbitbltなどの描画操作ができる場合がある。
 * 可能な操作はfeatureQueryで予め調べることができる。
 */
static const char* SERVER_Display = "b612916b-0504-489d-a1e0-d46e231244ba";



/** DESKTOP SERVER
 * DISPLAY SERVER、KEYBOARD SERVER、POINTER SERVERなどにより起動される。
 * デバイススキャンの完了を待ち(これはカーネルにより広報される)、デスクトップを構成してログイン画面を表示。
 * ログインされたら認証して権限を受け取る(この時点でカーネルはDESKTOP SERVERに権限を与える)。
 * 権限を受け取ったら、セッション情報を開いて(DESKTOP SERVERは独自にリソースを開く権限を持つ)再現する。
 */
static const char* SERVER_Desktop = "507cdc22-d3b8-4231-ad70-9016ac6e9344";



/** PCIデバイスのドライバ
 * arch/(arch)/kernel/pcibus.ccにより探索、起動される
 */
static const char* SERVER_PCI_Device = "fba50c55-c731-4908-a98b-65956f8abeb4";




#endif

