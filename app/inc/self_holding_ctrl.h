/****************************************************************/
/* file self_holding_ctrl.h										*/
/* 自己保持制御													*/
/* 																*/
/* 自己保持制御をする。											*/
/* <ul>															*/
/* <li>ACCがONになった場合、自己保持ON出力しACC ONイベントを	*/
/* 		発行する。												*/
/* <li>ACCがOFFとなった場合、ACC OFF判定を開始しACC OFFイベント	*/
/* 		を発行する。											*/
/* <li>ACCがOFFになった場合（5sec以上ACCがOFFになった場合）、	*/
/* 		ACC OFF判定カウント5sec経過イベントを発行する。			*/
/* <li>システム状態が終了モードの場合、自己保持OFFする。		*/
/* </ul>														*/
/* 																*/
/****************************************************************/
/* author      Y.Sorimachi										*/
/* date        2013/--/--										*/
/****************************************************************/

#ifndef	SELF_HOLDING_CTRL_H
#define	SELF_HOLDING_CTRL_H

#include "type.h"

/********************************************************/
/*	外部マクロ											*/
/********************************************************/
/********************************************************/
/*	定数宣言											*/
/********************************************************/

/********************************************************/
/*	外部関数											*/
/********************************************************/
/** 
 * /// 初期化処理
 * 
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		<ul>
 *			<li>自己保持制御で使用されるRAMを初期化する。
 *			<li>チャタリング吸収モジュールにACCをコールバックされるよう要求する。
 *			</ul>
 */ 
void Self_holding_ctrl_Init( void );

/** 
 * メイン処理
 * 
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		<ul>
 *			<li>ACCがONになった場合、自己保持ON出力しACC ONイベントを発行する。
 *			<li>ACCがOFFとなった場合、ACC OFF判定を開始しACC OFFイベントを発行する。
 *			<li>ACCがOFFになった場合（5sec以上ACCがOFFになった場合）、ACC OFF判定カウント5sec経過イベントを発行する。
 *			<li>システム状態が終了モードの場合、自己保持OFFする。
 *			</ul>
 */ 
void Self_holding_ctrl_Main( void );

#endif	/* SELF_HOLDING_CTRL_H */
