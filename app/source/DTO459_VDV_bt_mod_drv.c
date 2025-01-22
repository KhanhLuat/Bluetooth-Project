/*==================================================================================================*/
/* @file	bt_mod_drv.c																			*/
/* @brief	Bluetooth Module Driver																	*/
/*==================================================================================================*/
/* @par Copyright																					*/
/*	Copyright (c) 2015 by N.S. Ltd. All rights reserved.											*/
/*																									*/
/* @par File History																				*/
/*   | Date       | Summary                                  | Source Repository	 | Revision |	*/
/*   | ---------- | ---------------------------------------- | -------------------- | -------- |	*/
/*   | 2015-xx-xx | Created for XXXXXXX                      | N/A                  | N/A      |	*/
/*==================================================================================================*/


#include "DTO459_VDV_bt_mod_drv.h"
#include "SHRVSTD_Port_Drv.h"
#include "SHRVSTD_UART_Drv.h"
#include "SHRVSTD_CPU_Drv.h"
#include "SSFTSTD_Macro.h"
#include <string.h>
#include "dbglog.h"
#include "time_measure.h"

#define	BTMODDRV_LOCAL	static
/********************************************************/
/*	pragma宣言											*/
/********************************************************/
/**
 *	割り込み
**/
BTMODDRV_LOCAL void S_Intr_BT_mod_drv_Send( void );
BTMODDRV_LOCAL void S_Intr_BT_mod_drv_Receive( void );
BTMODDRV_LOCAL void S_Intr_BT_mod_drv_Err( void );

/********************************************************/
/*	内部マクロ											*/
/********************************************************/
/**
 * GPIO関係マクロ 
**/
#define D_BTMODDRV_RESET_WAIT_USEC				(200U)		/**< リセット待ち時間(usec)				*/

/**
 * 汎用定数
**/
#ifndef N_TRUE
#define N_TRUE			(1U)	/* 真:TRUE */
#endif /* N_TRUE */
#ifndef N_FALSE
#define N_FALSE			(0U)	/* 偽:FALSE */
#endif /* N_FALSE */

/********************************************************/
/*	定数宣言											*/
/********************************************************/

/********************************************************/
/*	内部変数定義										*/
/********************************************************/
BTMODDRV_LOCAL UI_8 S_send_permission_state;				/**< 送信許可状態（N_TRUE：送信許可、N_FALSE：送信禁止）*/
BTMODDRV_LOCAL UI_8 S_bt_mod_buf_rx[BT_MOD_DRV_RX_BUF];		/**< 受信データ格納用バッファ */
BTMODDRV_LOCAL UI_8 S_bt_mod_buf_tx[BT_MOD_DRV_TX_BUF];		/**< 送信データ格納用バッファ */
BTMODDRV_LOCAL UI_8 S_bt_mod_buf_err[BT_MOD_DRV_RX_BUF];	/**< エラーデータ格納用バッファ */
BTMODDRV_LOCAL UI_16 S_bt_mod_set_rx_data_pos;				/**< 受信データ格納位置 */
BTMODDRV_LOCAL UI_16 S_bt_mod_get_rx_data_pos;				/**< 受信データ取得位置 */
BTMODDRV_LOCAL UI_16 S_bt_mod_set_tx_data_pos;				/**< 送信データ格納位置 */
BTMODDRV_LOCAL UI_16 S_bt_mod_get_tx_data_pos;				/**< 送信データ取得位置 */

/********************************************************/
/*	プロトタイプ宣言									*/
/********************************************************/
BTMODDRV_LOCAL UI_16 S_Bt_mod_get_data_size(void);
BTMODDRV_LOCAL UI_16 S_Bt_mod_send_data_size(void);
BTMODDRV_LOCAL void S_Bt_mod_UartEnable( void );
BTMODDRV_LOCAL void S_Bt_mod_UartDisable( void );

/********************************************************/
/*	外部関数											*/
/********************************************************/

void Bt_mod_drv_Init( void ) 
{	
	/**	<ol> */
	/**	<li>内部変数の初期化 */
	S_send_permission_state = N_TRUE;
	(void)memset(S_bt_mod_buf_rx, 0, sizeof(S_bt_mod_buf_rx));
	(void)memset(S_bt_mod_buf_tx, 0, sizeof(S_bt_mod_buf_tx));
	(void)memset(S_bt_mod_buf_err, 0, sizeof(S_bt_mod_buf_err));
	S_bt_mod_set_rx_data_pos = 0;
	S_bt_mod_get_rx_data_pos = 0;
	S_bt_mod_set_tx_data_pos = 0;
	S_bt_mod_get_tx_data_pos = 0;
	/**	</ol> */
	return;
}

void Bt_mod_drv_StartBtModule( void )
{
	/* 処理なし(POWERピンなし) */
	return;
}

void Bt_mod_drv_StopBtModule( void )
{
	/* 処理なし(POWERピンなし) */
	return;
}

void Bt_mod_drv_StartComm( void )
{
	/** UARTドライバ送受信を許可する */
	S_Bt_mod_UartEnable();
	return;
}

void Bt_mod_drv_StopComm( void )
{	
	/** UARTドライバ送受信を禁止する */
	S_Bt_mod_UartDisable();
	return;
}

void Bt_mod_drv_Reset( void )
{
	UI_32 i;
	/** リセット実行 */
	(void)PortDrv_SetDataBit( IO_PO_GR_BT_RST, IO_PO_BIT_BT_RST, IO_PO_LEV_ACTIVE_BT_RST );
	
	/** 200usウェイト */
	for( i = 0 ; i < D_BTMODDRV_RESET_WAIT_USEC ; i++){
		/* 1μｓウェイト */
		D_NOP80();
	}

	/** リセット解除 */
	(void)PortDrv_SetDataBit( IO_PO_GR_BT_RST, IO_PO_BIT_BT_RST, IO_PO_LEV_NONACT_BT_RST );
	return;
}


void Bt_mod_drv_ResetON( void )
{
	/** リセット実行 */
	(void)PortDrv_SetDataBit( IO_PO_GR_BT_RST, IO_PO_BIT_BT_RST, IO_PO_LEV_ACTIVE_BT_RST );
	return;
}
void Bt_mod_drv_ResetOFF( void )
{
	/** リセット解除 */
	(void)PortDrv_SetDataBit( IO_PO_GR_BT_RST, IO_PO_BIT_BT_RST, IO_PO_LEV_NONACT_BT_RST );
	return;
}


E_BT_MOD_DRV_RESULT Bt_mod_drv_GetRxData(UI_8* buf, UI_16* len)
{
	UI_16 i;
	UI_8 tmp;
	UI_16 index;
	UI_16 get_data_size;
	index = 0;
	
	/** <ol> */
	/** <li>引数チェック */
	/** <li>引数が不正だったら、BT_MOD_DRV_ERR_INVALIDARGを返す */
	if ((buf == NULL) || (len == NULL))
	{
		return BT_MOD_DRV_ERR_INVALIDARG;
	}
	/**	<li> 受信確認データがなければ、BT_MOD_DRV_ERR_NOT_RECIEVE_DATAを返す */
	get_data_size = S_Bt_mod_get_data_size();
	if (get_data_size <= 0)
	{
		return BT_MOD_DRV_ERR_NOT_RECIEVE_DATA;
	}
	/** <li>受信データの取得 */
	for (i = 0; i < get_data_size; i++)
	{
		/* エラーデータを確認 */
		tmp = S_bt_mod_buf_err[S_bt_mod_get_rx_data_pos];
		if(tmp == 0x00)
		{
			/* エラーなし */
			/* データをコピー */
			buf[index] = S_bt_mod_buf_rx[S_bt_mod_get_rx_data_pos];
			index++;
		}
		else
		{
			/* エラーあり */
			/* コピーしたデータを破棄 */
			index = 0;
		}
		S_bt_mod_get_rx_data_pos++;
		S_bt_mod_get_rx_data_pos %= BT_MOD_DRV_RX_BUF;
	}
	/** <li>受信データ長の取得 */
	*len = index;
	/** </ol> */
	return BT_MOD_DRV_OK;
}

E_BT_MOD_DRV_RESULT Bt_mod_drv_SetTxData(const UI_8* buf, UI_16 len)
{
	UI_16 i;
	UI_16 send_data_size;

	/** <ol> */
	/** <li>引数チェック */
	/** <li>引数が不正だったら、BT_MOD_DRV_ERR_INVALIDARGを返す */
	if ((buf == NULL) || (len <= 0))
	{
		return BT_MOD_DRV_ERR_INVALIDARG;
	}
	/** <li>バッファサイズを超えていれば、BT_MOD_DRV_ERR_BUFFERFULLを返す */
	send_data_size = S_Bt_mod_send_data_size();
	if ((len + send_data_size) > BT_MOD_DRV_TX_BUF)
	{
		return BT_MOD_DRV_ERR_BUFFERFULL;
	}
	/** <li>送信バッファに送信するデータを設定する */
	for (i = 0; i < len; i++)
	{
		S_bt_mod_buf_tx[S_bt_mod_set_tx_data_pos++] = buf[i];
		S_bt_mod_set_tx_data_pos %= BT_MOD_DRV_TX_BUF;
	}
	/** <li>送信許可状態が許可されている場合(送信中でなければ) */
	if (S_send_permission_state == N_TRUE)
	{
		/** <ul> */
		/** <li>送信許可状態を禁止にする */
		S_send_permission_state = N_FALSE;
		/* UARTドライバ送信割り込み許可 */
		UartDrv_TxIntrReqEnblCtrl( UARTDRV_CH_BT, D_ON );
		/* バッファエンプティ割込みが入るのでここでは送信しない */
		/**	</ul> */
	}
	/**	</ol> */
	return BT_MOD_DRV_OK;
}


UI_16 Bt_mod_drv_GetTxEmptyBufferSize(void)
{
	UI_16 empty_tx_buf_size;
	/**	<ol> */
	/** <li>送信バッファの空きサイズを確認する */
	empty_tx_buf_size = BT_MOD_DRV_TX_BUF - S_Bt_mod_send_data_size();
	/**	</ol> */
	return empty_tx_buf_size;
}


E_BT_MOD_DRV_RESULT Bt_mod_drv_SetSystemState( E_BT_SYS_STATE_TYPE_OUT type, UI_8 level )
{
	/* 不要 */
	return BT_MOD_DRV_OK;
}

/********************************************************/
/*	割込み関数											*/
/********************************************************/
/** 
 * @brief	受信割込み
 *
 * @author	
 * @date	2015/-/-
 * @note	UART受信割込みにより実行され、受信データをバッファに格納する。
 */ 
BTMODDRV_LOCAL void S_Intr_BT_mod_drv_Receive( void )
{
	SI_8 result;
	UI_8 rxFlg;
	UI_8 rxData;
	/* 受信データフルフラグの取得 */
	rxFlg = UartDrv_GetRxFullFlag( UARTDRV_CH_BT );
	if(rxFlg == D_ON)
	{
		/* 受信データの取得 */
		result = UartDrv_GetRxData(UARTDRV_CH_BT, &rxData);
		if(result == D_OK)
		{
			/** <li>受信バッファに空きがあれば */
			if (S_Bt_mod_get_data_size() < (BT_MOD_DRV_RX_BUF - 1))
			{
				/** <ul> */
				/** <li> 受信データをバッファに入れる */
				S_bt_mod_buf_rx[S_bt_mod_set_rx_data_pos] = rxData;
				S_bt_mod_buf_err[S_bt_mod_set_rx_data_pos] = 0x00;	/* エラーなし */
				S_bt_mod_set_rx_data_pos++;
				S_bt_mod_set_rx_data_pos %= BT_MOD_DRV_RX_BUF;
				/** </ul> */
			}
		}
	}
}

/** 
 * @brief	送信割込み
 *
 * @author	
 * @date	2015/-/-
 * @note	UART送信完了割込みにより実行され、残送信データを送信する。
 */ 
BTMODDRV_LOCAL void S_Intr_BT_mod_drv_Send( void )
{
	UI_8 txData;
	if (S_Bt_mod_send_data_size() > 0)
	{
		/** <li>バッファから送信データを書き込む */
		txData =  S_bt_mod_buf_tx[S_bt_mod_get_tx_data_pos++];
		S_bt_mod_get_tx_data_pos %= BT_MOD_DRV_TX_BUF;
		(void)UartDrv_SetTxData(UARTDRV_CH_BT, txData);
	}
	/** <li>未送信データ残数がなければ */
	else
	{
		/** <ul> */
		/* UARTドライバ送信割り込み禁止 */
		UartDrv_TxIntrReqEnblCtrl( UARTDRV_CH_BT, D_OFF );
		/** <li>送信許可状態を許可にする */
		S_send_permission_state = N_TRUE;
		/** </ul> */
	}
}

/** 
 * @brief	エラー割込み
 *
 * @author	
 * @date	2015/-/-
 * @note	受信エラーによって実行され、エラー処理（割込み処理の解除）を行う。
 */ 
BTMODDRV_LOCAL void S_Intr_BT_mod_drv_Err( void )
{
		/** UARTドライバ送受信を禁止する */
	S_Bt_mod_UartDisable();
	/** UARTドライバ送受信を許可する */
	S_Bt_mod_UartEnable();
}

/********************************************************/
/*	内部関数											*/
/********************************************************/
/** 
 * @brief	受信データサイズ取得
 *
 * @return	受信データサイズ
 * @note	受信バッファに保存されているデータサイズを返す。
 */ 
 
BTMODDRV_LOCAL UI_16 S_Bt_mod_get_data_size(void)
{
	UI_16 rtn_size;
	UI_16 tmp_set_rx_data_pos;
	UI_16 tmp_get_rx_data_pos;
	/** <ol> */
	/** <li>受信データ格納位置と受信データ取得位置を一時領域に保存する */
	tmp_set_rx_data_pos = S_bt_mod_set_rx_data_pos;
	tmp_get_rx_data_pos = S_bt_mod_get_rx_data_pos;
	/** <li>書き込み位置と読み込み位置から受信バッファのデータ数を返す */
	if(tmp_get_rx_data_pos <= tmp_set_rx_data_pos)
	{
		rtn_size = tmp_set_rx_data_pos - tmp_get_rx_data_pos;
	}
	else
	{
		rtn_size = BT_MOD_DRV_RX_BUF - tmp_get_rx_data_pos + tmp_set_rx_data_pos;
	}
	/** </ol> */
	return rtn_size;
}
/** 
 * @brief	未送信データサイズ取得
 *
 * @return	未送信データサイズ
 * @note	送信バッファに保存されているデータサイズを返す。
 */
BTMODDRV_LOCAL UI_16 S_Bt_mod_send_data_size(void)
{
		UI_16 rtn_size;
	UI_16 tmp_set_tx_data_pos;
	UI_16 tmp_get_tx_data_pos;
	/** <ol> */
	/** <li>送信データ格納位置と送信データ取得位置を一時領域に保存する */
	tmp_set_tx_data_pos = S_bt_mod_set_tx_data_pos;
	tmp_get_tx_data_pos = S_bt_mod_get_tx_data_pos;
	/** <li>書き込み位置と読み込み位置から送信バッファのデータ数を返す */
	if (tmp_get_tx_data_pos <= tmp_set_tx_data_pos)
	{
		rtn_size = tmp_set_tx_data_pos - tmp_get_tx_data_pos;
	}
	else
	{
		rtn_size = BT_MOD_DRV_TX_BUF - tmp_get_tx_data_pos + tmp_set_tx_data_pos;
	}
	/** </ol> */
	return rtn_size;
}

/** 
 * @brief	UART送受信を許可する
 *
 * @note	成功時 S_bt_uart_status は UART_STATUS_TX_ENABLED になる
 */
BTMODDRV_LOCAL void S_Bt_mod_UartEnable( void )
{
	/** UARTドライバ受信許可 */
	(void)UartDrv_ReqRxCtrl( UARTDRV_CH_BT, D_ENABLE );
	/* UARTドライバ受信割り込み許可 */
	UartDrv_RxIntrReqEnblCtrl( UARTDRV_CH_BT, D_ON );
}

/** 
 * @brief	UART送受信を禁止する
 *
 * @note	成功時 S_bt_uart_status は UART_STATUS_DISABLED になる
 */
BTMODDRV_LOCAL void S_Bt_mod_UartDisable( void )
{
	/* UARTドライバ送信割り込み禁止 */
	UartDrv_TxIntrReqEnblCtrl( UARTDRV_CH_BT, D_OFF );
	/* UARTドライバ受信割り込み禁止 */
	UartDrv_RxIntrReqEnblCtrl( UARTDRV_CH_BT, D_OFF );
	/** UARTドライバ受信禁止 */
	(void)UartDrv_ReqRxCtrl( UARTDRV_CH_BT, D_DISABLE );
}

/** 
 * UART受信完了割込み
 */ 
__interrupt void Intr_LinUart6_Rx (void)
{
	UI_8 rxData;
	SI_8 errResult;
	T_UartDrv_ErrInfo err;
	
	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_BT_INTR_RX);)

	/* 受信データ取得 */
	errResult = UartDrv_GetErr(UARTDRV_CH_BT, &err);
	if(errResult == D_OK)
	{
		if( (err.parity_flg  == D_ON) ||	/* パリティエラーフラグ */
			(err.overrun_flg == D_ON) ||	/* オーバランエラーフラグ */
			(err.flaming_flg == D_ON) )		/* フレーミングエラーフラグ */
		{
			rxData = 0x00;
			if (err.parity_flg  == D_ON)
			{
				rxData |= 0x01;
			}
			if (err.overrun_flg == D_ON)
			{
				rxData |= 0x02;
			}
			if (err.flaming_flg == D_ON)
			{
				rxData |= 0x04;
			}
			S_bt_mod_buf_rx[S_bt_mod_set_rx_data_pos] = 0x00;		/* 受信データは0とする */
			S_bt_mod_buf_err[S_bt_mod_set_rx_data_pos] = rxData;	
			S_bt_mod_set_rx_data_pos++;
			S_bt_mod_set_rx_data_pos %= BT_MOD_DRV_RX_BUF;
			/* 受信エラー */
			S_Intr_BT_mod_drv_Err();
		}
		else
		{
			/* 受信OK */
			S_Intr_BT_mod_drv_Receive();
		}
	}
	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_BT_INTR_RX);)
}

/** 
 * UART送信完了割込み（送信バッファエンプティ割込み）
 */ 
__interrupt void Intr_LinUart6_Tx (void)
{
	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_BT_INTR_TX);)
	S_Intr_BT_mod_drv_Send();
	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_BT_INTR_TX);)
}
