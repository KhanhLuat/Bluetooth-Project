/*==================================================================================================*/
/* @file	bt_mod_drv.h																			*/
/* @brief	Bluetooth Module Driver																	*/
/*==================================================================================================*/
/* @par Copyright																					*/
/*	Copyright (c) 2014 by N.S. Ltd. All rights reserved.											*/
/*																									*/
/* @par File History																				*/
/*   | Date       | Summary                                  | Source Repository	 | Revision |	*/
/*   | ---------- | ---------------------------------------- | -------------------- | -------- |	*/
/*   | 2014-xx-xx | Created for XXXXXXX                      | N/A                  | N/A      |	*/
/*==================================================================================================*/

#ifndef __BT_MOD_DRV_H__
#define __BT_MOD_DRV_H__

/*==================================================================================================*/
/*	Include files																					*/
/*==================================================================================================*/

#include "type.h"

/*==================================================================================================*/
/*	Constant, Macro definitions																		*/
/*==================================================================================================*/
#define BT_MOD_DRV_RX_BUF	(1000)	/**< 受信バッファのバイト数	*/
#define BT_MOD_DRV_TX_BUF	(512)	/**< 送信バッファのバイト数	*/

/*==================================================================================================*/
/*	Structure, Union, Type definitions																*/
/*==================================================================================================*/

/**
 * 処理結果
 */
typedef enum e_bt_mod_dev_result
{
	BT_MOD_DRV_OK = 0,					/**< 正常終了			*/
	BT_MOD_DRV_ERR_INVALIDARG,			/**< 引数不正			*/
	BT_MOD_DRV_ERR_BUFFERFULL,			/**< バッファフル		*/
	BT_MOD_DRV_ERR_NOT_RECIEVE_DATA		/**< 受信データなし		*/
} E_BT_MOD_DRV_RESULT;

/*
 ** ボーレート
 */
typedef enum e_bt_baudrate
{
	BT_BAUDRATE_115200_BPS = 0,			/**< 115200bps			*/
	BT_BAUDRATE_230400_BPS,				/**< 230400bps			*/
	BT_BAUDRATE_NUM						/**< ボーレート数		*/
} E_BT_BAUDRATE;

/*
 ** システム状態種別（入力）
 */
typedef enum e_bt_sys_state_type_in
{
	BT_SYS_STATE_TYPE_IN_DSR = 0,		/**< UART_DSR							*/
	BT_SYS_STATE_TYPE_IN_NUM			/**< システム状態種別（入力）数			*/
} E_BT_SYS_STATE_TYPE_IN;

/*
 ** システム状態種別（出力）
 */
typedef enum e_bt_sys_state_type_out
{
	BT_SYS_STATE_TYPE_OUT_DTR = 0,		/**< UART_DTR							*/
	BT_SYS_STATE_TYPE_OUT_NUM			/**< システム状態種別（出力）数			*/
} E_BT_SYS_STATE_TYPE_OUT;


/*==================================================================================================*/
/*	Variable declarations																			*/
/*==================================================================================================*/

/*==================================================================================================*/
/*	Function prototypes																				*/
/*==================================================================================================*/
/** 
 * @brief	初期化
 *
 * @note	<ul>
 *			<li>Bluetoothモジュールドライバで使用するRAMの初期化処理を行う。
 *			<li>Bluetoothモジュールドライバで使用するレジスタの設定を行う。
 *			</ul>
 */ 
void Bt_mod_drv_Init( void );

/** 
 * @brief	BTモジュール起動
 * 
 * @note	
 */ 
void Bt_mod_drv_StartBtModule( void );

/** 
 * @brief	BTモジュール停止
 * 
 * @note	
 */ 
void Bt_mod_drv_StopBtModule( void );

/** 
 * @brief	通信開始
 * 
 * @note	BluetoothモジュールとのUARTによる送受信を開始する。
 */ 
void Bt_mod_drv_StartComm( void );

/** 
 * @brief	通信停止
 * 
 * @note	BluetoothモジュールとのUARTによる送受信を停止する。
 */ 
void Bt_mod_drv_StopComm( void );

/** 
 * @brief	BTモジュールリセット
 * 
 * @note	Bluetoothモジュールに対して、リセット処理する。
 */ 
void Bt_mod_drv_Reset( void );

/** 
 * @brief	BTモジュールリセットON
 * 
 * @note	Bluetoothモジュールに対して、リセットON処理する。
 */ 
void Bt_mod_drv_ResetON( void );

/** 
 * @brief	BTモジュールリセットOFF
 * 
 * @note	Bluetoothモジュールに対して、リセットOFF処理する。
 */ 
void Bt_mod_drv_ResetOFF( void );

/** 
 * @brief	受信データ取得
 * 
 * @param	buf		[out]	受信したデータを格納するバッファへのポインタ
 * @param	len		[out]	受信したデータのバイト数
 *
 * @return	実行結果
 * @retval	BT_MOD_DRV_OK						正常終了
 * @retval	BT_MOD_DRV_ERR_INVALIDARG			引数不正
 * @retval	BT_MOD_DRV_ERR_NOT_RECIEVE_DATA		受信データなし
 *
 * @note	受信したデータを取得する。
 */ 
E_BT_MOD_DRV_RESULT Bt_mod_drv_GetRxData(UI_8* buf, UI_16* len);

/** 
 * @brief	送信データ設定
 * 
 * @param	*buf	[out]	送信するデータが格納されているバッファへのポインタ
 * @param	len		[in]	送信するデータのバイト数
 *
 * @return	実行結果
 * @retval	BT_MOD_DRV_OK				正常終了
 * @retval	BT_MOD_DRV_ERR_INVALIDARG	引数不正
 * @retval	BT_MOD_DRV_ERR_BUFFERFULL	バッファフル
 *
 * @note	送信するデータを設定する。
 */ 
E_BT_MOD_DRV_RESULT Bt_mod_drv_SetTxData(const UI_8* buf, UI_16 len);

/** 
 * @brief	送信用バッファ空きサイズ取得
 *
 * @return	送信用バッファ空きサイズ
 * @note	<ul>
 *			<li>送信用バッファの空きサイズを取得する。
 *			</ul>
 */ 
UI_16 Bt_mod_drv_GetTxEmptyBufferSize( void );

/** 
 * @brief	システム状態設定
 *
 * @param	type	[in]	システム状態種別（出力）
 * @param	level	[in]	出力レベル（N_HIGH or N_LOW）
 *
 * @return	実行結果（E_BT_MOD_DRV_RESULT）
 * @retval	BT_MOD_DRV_OK				正常終了
 * @retval	BT_MOD_DRV_ERR_INVALIDARG	引数不正
 *
 * @note
 */ 
E_BT_MOD_DRV_RESULT Bt_mod_drv_SetSystemState( E_BT_SYS_STATE_TYPE_OUT type, UI_8 level );

/** 
 * UART受信完了割込み
 */ 
__interrupt void Intr_LinUart6_Rx (void);

/** 
 * UART送信完了割込み（送信バッファエンプティ割込み）
 */ 
__interrupt void Intr_LinUart6_Tx (void);

#endif	/* __BT_MOD_DRV_H__ */
