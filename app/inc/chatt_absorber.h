/********************************************************/
/* file chatt_absorber.h								*/
/* チャタリング吸収										*/
/* 														*/
/* <ul>													*/
/* <li>入力ポートから取得したデータに対しチャタリング吸収する。			*/
/* <li>チャタリング吸収したデータは、設定された関数にコールバックする。	*/
/* <li>チャタリング吸収は、20ms周期に3回連続で同じ値のものとする。		*/
/* </ul>												*/
/* 														*/
/********************************************************/
/* author      Y.Sorimachi								*/
/* date        2013/--/--								*/
/********************************************************/ 
/* page */

#ifndef	CHATT_ABSORBER_H
#define	CHATT_ABSORBER_H

#include "type.h"
#include "System_control_TASK.h"

/********************************************************/
/*	外部マクロ											*/
/********************************************************/
/**
 *	チャタリング吸収する汎用ポート
 */
typedef enum e_chatt_abs_port {
	CHATT_PORT_ACC = 0,				/**< ACC				*/
	CHATT_PORT_SDC,					/**< SDカード検出		*/
	CHATT_PORT_NUM
} E_CHATT_ABS_PORT;

/**
 *	実行結果
 */
typedef enum e_chatt_abs_result {
	CHATT_ABS_OK = 0,				/**< 正常終了			*/
	CHATT_ABS_ERR_INVALID_ARG		/**< 引数不正			*/
} E_CHATT_ABS_RESULT;

/**
 *	チャタリング吸収するモジュール
 */
typedef enum e_chatt_abs_module {
	CHATT_ABS_MOD_SHC = 0,			/**< 自己保持制御		*/
	CHATT_ABS_MOD_FIO,				/**< ファイルIO制御		*/
	CHATT_ABS_MOD_NUM
} E_CHATT_ABS_MODULE;


/********************************************************/
/*	定数宣言											*/
/********************************************************/

/********************************************************/
/*	型宣言												*/
/********************************************************/
typedef void (*P_CHATT_ABS_CB_FUNC)(UI_8);	/**< コールバック関数	*/

/********************************************************/
/*	外部関数											*/
/********************************************************/

/**
 * チャタリング吸収 初期化
 * 
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		チャタリング吸収で使用されるRAMの初期化を行う。
 */
void Chatt_absorber_Init( void );

/**
 * チャタリング吸収 メイン処理
 * 
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		チャタリング吸収のメイン処理を行う。
 */
void Chatt_absorber_Main( void );

/**
 * チャタリング吸収 ポートデータ要求
 * 
 * param	ca_data_cb	[in]	チャタリング吸収後のデータコールバック関数
 * param	port		[in]	指定するポート（E_CHATT_ABS_PORT）
 * param	module		[in]	コールバック関数を登録するモジュール（E_CHATT_ABS_MODULE）
 *
 * return	実行結果（E_CHATT_ABS_RESULT）
 * 
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		<ul>
 *			<li>指定したポートのチャタリング吸収後のデータを要求する。
 *			<li>指定したポートにエッジ変化があったとき、チャタリング吸収結果データをコールバック関数で渡す。
 *			</ul>
 */
UI_8 Chatt_absorber_ReqPortData( P_CHATT_ABS_CB_FUNC ca_data_cb, UI_8 port, UI_8 module );

#endif	/* CHATT_ABSORBER_H */
