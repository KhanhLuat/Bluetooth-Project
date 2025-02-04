/********************************************************************************/
/* file defi_07701_CTRL.h														*/
/*  Defiユニット制御(DF07701/ﾃｽﾄ治具)											*/
/*  																			*/
/*  Defiユニット(DF07701/ﾃｽﾄ治具)を制御する。									*/
/*  <ul>																		*/
/*  <li>DF07701と通信し、OPﾃﾞｰﾀ、ﾘｱﾙﾃﾞｰﾀを取得する。							*/
/* <li>ﾃｽﾄ治具と通信し、MFi認証ﾁｯﾌﾟ ｾﾙﾌﾃｽﾄを実行、ｼﾘｱﾙﾃﾞｰﾀの読書きを実行する。	*/
/* </ul>																		*/
/********************************************************************************/
/* author      Y.Sorimachi														*/
/* date        2014/4/11														*/
/********************************************************************************/
#ifndef	DEFI_07701_CTRL_H
#define	DEFI_07701_CTRL_H

#ifdef ADVANCE_ENABLE

#include "type.h"

/*--------------------------------------------------------------------------*/
/*	外部マクロ																*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*	定数宣言																*/
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*	構造体宣言																*/
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*	プロトタイプ宣言														*/
/*--------------------------------------------------------------------------*/
/** 
 * RAM初期化
 * 
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		DF07701制御で使用するRAMの初期化処理を行う。
 */ 
void Defi_07701_control_Init( void );

/** 
 * DF07701制御ﾒｲﾝ処理 
 * 
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		<ul>
 *			<li>DF07701と通信し、OPﾃﾞｰﾀのDisp No.1〜7接続要求を4回以上取得したとき、接続要求ﾚｽﾎﾟﾝｽを送信する。
 *			<li>DF07701と通信し、OPﾃﾞｰﾀ、RealﾃﾞｰﾀをDefiユニット制御に設定する。
 *			<li>ﾃｽﾄﾓｰﾄﾞ時にﾃｽﾄ治具と通信し、MFi認証ﾁｯﾌﾟ ｾﾙﾌﾃｽﾄを実行、ｼﾘｱﾙﾃﾞｰﾀの読書きを実行し、結果を送信する。
 *			</ul>
 */ 
void Defi_07701_control_main(void);

#endif /* ADVANCE_ENABLE */

#endif	/* DEFI_07701_CTRL_H */
