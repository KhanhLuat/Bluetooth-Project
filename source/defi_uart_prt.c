/********************************************************************************************/
/* file defi_uart_prt.c																		*/
/* Defi UARTプロトコル 																		*/
/* 																							*/
/* Defiユニットまたは、テスト治具から受信したメッセージを解析と応答メッセージを送信する。	*/
/* <ul>																						*/
/* <li>「Defi UARTデバイスドライバ」から受信データを取得し、メッセージプロトコル解析を行い、*/
/* 		正常なメッセージを「Defiユニット制御」に渡す。										*/
/* <li>「Defiユニット制御」から受取った応答メッセージを「Defi UARTデバイスドライバ」へ渡し、*/
/* 		送信する。																			*/
/* </ul>																					*/
/* 																							*/
/********************************************************************************************/
/* author      Y.Sorimachi																	*/
/* date        2014/4/11																	*/
/********************************************************************************************/
#include <string.h>														/* 標準ﾍｯﾀﾞｰﾌｧｲﾙ */
#include "type.h"
#include "system.h"
#include "defi_uart_prt.h"												/* Defi UARTﾌﾟﾛﾄｺﾙﾍｯﾀﾞｰﾌｧｲﾙ */
#include "SSFTSTD_Macro.h"
#include "SHRVSTD_UART_Drv.h"											/* UARTドライバヘッダーファイル */
#include "SHRVSTD_Interrupt_Drv.h"
#include "DTO459_VDV_RringBuffer.h"
#include "time_measure.h"

/****************************************************************************/
/*	内部マクロ																*/
/****************************************************************************/
#define DEFIUART_LOCAL	static

#define		UA1_INTRX_BUF_SZ			(1 * 1024)						/**< UART1受信ﾊﾞｯﾌｧｻｲｽﾞ(割込み) 	*/
#define		UA_ANALYZE_WORK_BUF_SZ		(25)							/**< UARTデータ解析用ワークバッファサイズ	*/
#define		UA1_RX_BUF_NUM				(2)								/**< UART1受信ﾊﾞｯﾌｧ数 				*/
#define		UA1_RX_BUF_SZ				(35)							/**< UART1受信ﾊﾞｯﾌｧｻｲｽﾞ 			*/
#define		UA1_TX_BUF_SZ				(1 * 1024)						/**< UART1送信ﾊﾞｯﾌｧｻｲｽﾞ 			*/ 
#define		UA_HEAD_7701				(0xFF)							/**< DF07701ﾍｯﾀﾞ 					*/

/**
 *	UART1受信頭出しｽﾃｰﾀｽ
 */
typedef enum e_ua1_status { 
	ST_UA1_WAIT_HEAD1 = 0,												/**< ヘッダ受信待ち1 */
	ST_UA1_WAIT_HEAD2,													/**< ヘッダ受信待ち2 */
	ST_UA1_WAIT_MODE,													/**< ﾓｰﾄﾞ受信待ち 	 */
	ST_UA1_WAIT_TESTMODE,												/**< ﾃｽﾄﾓｰﾄﾞ 2byte目の受信待ち 	*/
	ST_UA1_RECEIVING,													/**< 受信中 		*/
	ST_UA1_MAX															/**< 要素数 		*/
} E_UA1_STATUS;


/****************************************************************************/
/*	定数宣言																*/
/****************************************************************************/
#ifdef ADVANCE_ENABLE
/**
 *	DF07701ﾊﾟｹｯﾄ長ﾃｰﾌﾞﾙ 
 */
DEFIUART_LOCAL const UI_8	C_07701_length_tbl[16] = {
	0,								/**< 0x00:--- 			*/
	26,								/**< 0x01:OPENING 		*/
	4,								/**< 0x02:ENDING 		*/
	34,								/**< 0x03:REAL 			*/
	0,								/**< 0x04:--- 			*/
	34,								/**< 0x05:REC 			*/
	34,								/**< 0x06:PLAY 			*/
	0,								/**< 0x07:--- 			*/
	25,								/**< 0x08:SET UP 		*/
	25,								/**< 0x09:CYLINDER SET 	*/
	25,								/**< 0x0A:RESPONSE SET 	*/
	31,								/**< 0x0B:METER SELECT 	*/
	28,								/**< 0x0C:WARNING SET 	*/
	0,								/**< 0x0D:--- 			*/
	0,								/**< 0x0E:--- 			*/
	4								/**< 0x0F:INITIALIZE 	*/
};
#endif /* ADVANCE_ENABLE */

/* パリティ値変換テーブル（DEFI定義値->UART DRV定義値変換用）*/
/* DEFI_UART_PARITY_xxxの定義順にすること */
DEFIUART_LOCAL const E_UARTDRV_PARITY	C_cnv_parity_defi2drv_tbl[] = {
	UARTDRV_PARITY_NONE,					/* DEFI_UART_PARITY_NONE	パリティなし				*/
	UARTDRV_PARITY_EVEN,					/* DEFI_UART_PARITY_EVEN	偶数パリティ				*/
	UARTDRV_PARITY_ODD,						/* DEFI_UART_PARITY_ODD		奇数パリティ				*/
};

/* ボーレート値変換テーブル（DEFI定義値->UART DRV定義値変換用）*/
/* DEFI_UART_BAUD_RATE_xxxの定義順にすること */
DEFIUART_LOCAL const E_UARTDRV_BAUD_RATE	C_cnv_brate_defi2drv_tbl[] = {
	UARTDRV_BAUD_RATE_9600,					/* DEFI_UART_BAUD_RATE_9600		9600bps					*/
	UARTDRV_BAUD_RATE_19200,				/* DEFI_UART_BAUD_RATE_19200	19200bps				*/
	UARTDRV_BAUD_RATE_115200,				/* DEFI_UART_BAUD_RATE_115200	115200bps				*/
};

/****************************************************************************/
/*	構造体宣言																*/
/****************************************************************************/


/****************************************************************************/
/*	内部変数定義															*/
/****************************************************************************/
DEFIUART_LOCAL 	T_RINGBUFFER Rx_buf_int1_mng;									/**< UART1受信ﾊﾞｯﾌｧ管理領域 		*/
DEFIUART_LOCAL 	T_RINGBUFFER Rx_err_inf1_mng;									/**< UART1受信ｴﾗｰ情報管理領域 		*/
DEFIUART_LOCAL	UI_8	Rx_buf_int1[UA1_INTRX_BUF_SZ];							/**< UART1受信ﾊﾞｯﾌｧ(割込み) 		*/
DEFIUART_LOCAL	UI_8	Rx_err_inf1[UA1_INTRX_BUF_SZ];							/**< UART1受信ｴﾗｰ情報 				*/
#ifdef ADVANCE_ENABLE
DEFIUART_LOCAL	UI_8	Rx_buf1[UA1_RX_BUF_NUM][UA1_RX_BUF_SZ];					/**< UART1受信ﾊﾞｯﾌｧ(解析結果格納) 	*/
#endif /* ADVANCE_ENABLE */
DEFIUART_LOCAL	UI_8	Rx_len1[UA1_RX_BUF_NUM];								/**< UART1受信ﾊﾞｯﾌｧﾃﾞｰﾀ長 			*/
#ifdef ADVANCE_ENABLE
DEFIUART_LOCAL	UI_8	Rx_idx1;												/**< UART1受信ﾃﾞｰﾀ格納位置 			*/
DEFIUART_LOCAL	UI_8	Rx_set_pos1;											/**< UART1受信ﾌﾚｰﾑ格納位置 			*/
DEFIUART_LOCAL	UI_8	Rx_get_pos1;											/**< UART1受信ﾌﾚｰﾑ取得位置 			*/
DEFIUART_LOCAL	UI_8	Rx_status1;												/**< UART1受信頭出しｽﾃｰﾀｽ 			*/
#endif /* ADVANCE_ENABLE */

DEFIUART_LOCAL 	T_RINGBUFFER Tx_buf_mng;										/**< UART送信ﾊﾞｯﾌｧ管理領域 			*/
DEFIUART_LOCAL  UI_8	Tx_buf[UA1_TX_BUF_SZ];									/**< UART送信用バッファ				*/


/****************************************************************************/
/*	内部関数プロトタイプ宣言												*/
/****************************************************************************/
DEFIUART_LOCAL void uart_buf_init(void);										/* UARTﾊﾞｯﾌｧ初期化処理 */
#ifdef ADVANCE_ENABLE
DEFIUART_LOCAL void uart1_store_buf_7701(UI_8 rx_data, UI_8 rx_err);			/* UART1受信ﾊﾞｯﾌｧ格納処理(DF07701接続) */
#endif /* ADVANCE_ENABLE */
DEFIUART_LOCAL UI_16 get_uart1_rx_data(UI_8 *rx_data, UI_8 *rx_err, UI_16 data_len);	/* UART1受信ﾃﾞｰﾀ取得処理 */
DEFIUART_LOCAL void uartEnable(void);											/* UART送受信処理開始 */
DEFIUART_LOCAL void uartDisable(void);											/* UART送受信処理停止 */

/****************************************************************************/
/*	外部関数																*/
/****************************************************************************/
#ifdef ADVANCE_ENABLE
/** 
 * RAM初期化
 * 
 * param	mode		[in]	初期化条件
 *
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		RAMの初期化を行う。
 */ 
void Uart_ram_init(void)
{
}
#endif /* ADVANCE_ENABLE */

/** 
 * UART通信送受信開始処理 
 * 
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		ﾊﾞｯﾌｧの初期化後、UART1の送受信開始設定を行う。 
 */ 
void Uart_start(void)
{
	uart_buf_init();
	/* UART送受信処理開始 */
	uartEnable();
}

/** 
 * UART通信送受信停止処理 
 * 
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		UART1の送受信停止設定を行う。
 */ 
void Uart_stop(void)
{
	/* UART送受信処理停止 */
	uartDisable();
}

#ifdef ADVANCE_ENABLE
/** 
 * UART受信解析処理  
 * 
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		UART1の受信ﾃﾞｰﾀ解析を実行する。
 */ 
void Uart_rx_data_analyze(void)
{
	UI_8	rx_data[UA_ANALYZE_WORK_BUF_SZ];							/* 受信ﾃﾞｰﾀ格納ﾜｰｸ */
	UI_8	rx_err[UA_ANALYZE_WORK_BUF_SZ];								/* ｴﾗｰ情報取得ﾜｰｸ */
	UI_8	rx_len;														/* 受信ﾃﾞｰﾀ長 */
	UI_8	i;
	
	rx_len = (UI_8)get_uart1_rx_data(rx_data, rx_err, sizeof(rx_data));

	i = 0;
	while (rx_len > 0) {
		/* データ解析バッファ（Rx_buf1）にデータを詰める */
		uart1_store_buf_7701(rx_data[i], rx_err[i]);
		rx_len--;
		i++;
	}
}

/** 
 * Defiﾕﾆｯﾄ受信ﾌﾚｰﾑ取得処理 
 * 
 * param	*msg		[out]	受信ﾌﾚｰﾑ格納ﾊﾞｯﾌｧのﾎﾟｲﾝﾀ
 * param	*len		[out]	受信ﾌﾚｰﾑﾃﾞｰﾀ長格納変数のﾎﾟｲﾝﾀ
 *
 * return	受信ﾌﾚｰﾑ有無（有:0xFF以外、無:0xFF） 
 *
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		Defiﾕﾆｯﾄからの受信ﾌﾚｰﾑ取得を行う。
 */ 
UI_8 Get_uart_rx_data_defi(UI_8 *msg, UI_8 *len)
{
	UI_8	ret;														/* 戻り値 */
	UI_8	rx_get_pos;													/* UART1受信ﾌﾚｰﾑ取得位置 */
	UI_8	rx_set_pos;													/* UART1受信ﾌﾚｰﾑ格納位置 */

	ret = 0xFF;
	rx_get_pos = Rx_get_pos1;
	rx_set_pos = Rx_set_pos1;

	/* データ解析バッファ（Rx_buf1）に１フレーム分のデータがなければ以下の処理を行わない */
	if (rx_get_pos != rx_set_pos) {
		(void)memcpy(msg, &Rx_buf1[rx_get_pos][0], (size_t)Rx_len1[rx_get_pos]);
		*len = Rx_len1[rx_get_pos];

		rx_get_pos++;

		if (rx_get_pos >= UA1_RX_BUF_NUM) {
			rx_get_pos = 0;
		}

		if (rx_set_pos >= rx_get_pos) {
			ret = rx_set_pos - rx_get_pos;
		} else {
			ret = UA1_RX_BUF_NUM - rx_get_pos + rx_set_pos;
		}
	}

	Rx_get_pos1 = rx_get_pos;

	return ret;
}
#endif /* ADVANCE_ENABLE */

/****************************************************************************/
/* 関数名称		:	Uart_set_comm_protocol									*/
/* 機能の説明	:	通信プロトコル設定										*/
/* 引数			:	(I/ )protocol	通信プロトコル							*/
/* 戻り値		:	N_OK	正常											*/
/*				:	N_NG	異常											*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
SI_8 Uart_set_comm_protocol(const T_DEFI_UART_PROTOCOL *protocol)
{
	T_UartDrv_Protocol	drv_protocol;

	if (protocol == NULL) {
		return N_NG;
	}

	drv_protocol.bit_len		= UARTDRV_BIT_LEN_8;
	drv_protocol.stop_bit_len	= UARTDRV_STOP_BIT_LEN_1;
	drv_protocol.direction		= UARTDRV_DIRECTION_LSB_FIRST;

	if (protocol->parity >= DEFI_UART_PARITY_NUM) {
		return N_NG;
	}
	drv_protocol.parity			= C_cnv_parity_defi2drv_tbl[protocol->parity];

	if (protocol->baud_rate >= DEFI_UART_BAUD_RATE_NUM) {
		return N_NG;
	}
	drv_protocol.baud_rate		= C_cnv_brate_defi2drv_tbl[protocol->baud_rate];

	if (UartDrv_ChangeProtocol(UARTDRV_CH_ADVANCE, &drv_protocol) != D_OK) {
		return N_NG;
	}

	return N_OK;
}

/****************************************************************************/
/* 関数名称		:	Uart_get_rx_data										*/
/* 機能の説明	:	受信データ取得											*/
/* 引数			:	( /O)buf	データバッファ								*/
/*				:	(I/ )len	取得データ長(byte)							*/
/* 戻り値		:	取得できたデータ長(byte)								*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
UI_16 Uart_get_rx_data(UI_8 *buf, UI_16 len)
{
	UI_16	get_len;

	if (buf == NULL) {
		return 0;
	}

	get_len = get_uart1_rx_data(buf, NULL, len);

	return get_len;
}

/****************************************************************************/
/* 関数名称		:	Uart_get_rx_data_len									*/
/* 機能の説明	:	受信データ長取得										*/
/* 引数			:	void													*/
/* 戻り値		:	受信データ長(byte)										*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
UI_16 Uart_get_rx_data_len(void)
{
	UI_16	len;

	/* 読み込み可能データサイズ取得 */
	len = RingBuffer_GetCanReadSize(&Rx_buf_int1_mng);

	return len;
}

/****************************************************************************/
/* 関数名称		:	Uart_set_tx_data										*/
/* 機能の説明	:	送信データ設定											*/
/* 引数			:	(I/ )buf	データバッファ								*/
/*				:	(I/ )len	送信データ長(byte)							*/
/* 戻り値		:	N_OK	正常											*/
/*				:	N_NG	異常											*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
SI_8 Uart_set_tx_data(UI_8 *buf, UI_16 len)
{
	UI_16	can_tx_len;

	if (buf == NULL) {
		return N_NG;
	}

	/* 設定可能データサイズ取得	*/
	can_tx_len = RingBuffer_GetCanWriteSize(&Tx_buf_mng);
	if (can_tx_len < len) {
		/* 送信データバッファに指定サイズ分の空きがない	*/
		return N_NG;
	}

	/* 送信データバッファにデータを設定	*/
	(void)RingBuffer_SetArrayData(&Tx_buf_mng, buf, len);

	/* UARTドライバ送信割り込み許可（送信割込み処理でデータ送信が実行される） */
	UartDrv_TxIntrReqEnblCtrl( UARTDRV_CH_ADVANCE, D_ON );

	return N_OK;
}

/****************************************************************************/
/* 関数名称		:	Uart_get_can_set_tx_data_len							*/
/* 機能の説明	:	設定可能送信データ長取得								*/
/* 引数			:	void													*/
/* 戻り値		:	設定可能送信データ長									*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
UI_16 Uart_get_can_set_tx_data_len(void)
{
	UI_16	can_tx_len;

	/* 設定可能データサイズ取得	*/
	can_tx_len = RingBuffer_GetCanWriteSize(&Tx_buf_mng);

	return can_tx_len;
}

/****************************************************************************/
/*	内部関数																*/
/****************************************************************************/
/** 
 * UARTﾊﾞｯﾌｧ初期化処理
 * 
 * author	Y.Sorimachi
 * date	2014/4/11
 * note	UART送受信ﾊﾞｯﾌｧの初期化を行う。
 */ 
DEFIUART_LOCAL void uart_buf_init(void)
{
	/**< UART1受信ﾊﾞｯﾌｧ(割込み) 		*/
	RingBuffer_Init(&Rx_buf_int1_mng, Rx_buf_int1, sizeof(Rx_buf_int1));
	/**< UART1受信ｴﾗｰ情報 				*/
	RingBuffer_Init(&Rx_err_inf1_mng, Rx_err_inf1, sizeof(Rx_err_inf1));

	/* UART送信バッファ初期化	*/
	RingBuffer_Init(&Tx_buf_mng, Tx_buf, sizeof(Tx_buf));

#ifdef ADVANCE_ENABLE
	Rx_idx1 = 0;
	Rx_set_pos1 = 0;
	Rx_get_pos1 = 0;
	Rx_status1 = 0;
#endif /* ADVANCE_ENABLE */
	(void)memset(Rx_len1, 0, sizeof(Rx_len1));

}

#ifdef ADVANCE_ENABLE
/** 
 * UART1受信ﾊﾞｯﾌｧ格納処理(DF07701接続) 
 * 
 * param	rx_data		[in]	受信ﾃﾞｰﾀ  
 * param	rx_err		[in]	受信ｴﾗｰ情報（0:ｴﾗｰなし、1:ｴﾗｰあり）
 *
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		<ul>
 *			<li>受信ﾃﾞｰﾀの頭出しを行ない、ﾊﾞｯﾌｧに格納する。 
 *			<li>受信ｴﾗｰがあった場合、現在格納中ﾌﾚｰﾑを破棄する。
 *			</ul>
 */ 
DEFIUART_LOCAL void uart1_store_buf_7701(UI_8 rx_data, UI_8 rx_err)
{
	UI_8	tbl_pos;													/* ﾃｰﾌﾞﾙ位置算出用 */
	UI_8	rx_stat;													/* UART1受信頭出しｽﾃｰﾀｽ */
	UI_8	rx_set_pos;													/* UART1受信ﾌﾚｰﾑ格納位置 */
	UI_8	rx_idx;														/* UART1受信ﾃﾞｰﾀ格納位置 */

	tbl_pos = 0;
	rx_stat = Rx_status1;
	rx_set_pos = Rx_set_pos1;
	rx_idx = Rx_idx1;

	if (rx_err == 0) {
		switch (rx_stat) {
		case ST_UA1_WAIT_HEAD2:
			if (rx_data == UA_HEAD_7701) {
				rx_stat = ST_UA1_WAIT_MODE;
			} else {
				rx_stat = ST_UA1_WAIT_HEAD1;
			}
			break;

		case ST_UA1_WAIT_MODE:
			tbl_pos = rx_data & 0x0F;
			if (C_07701_length_tbl[tbl_pos] == 0) {
				/* モード値のデータサイズが０の場合 */
				tbl_pos = 0;
			}

			if(tbl_pos != 0) {
				rx_stat = ST_UA1_RECEIVING;
				Rx_buf1[rx_set_pos][0] = UA_HEAD_7701;
				Rx_buf1[rx_set_pos][1] = UA_HEAD_7701;
				Rx_buf1[rx_set_pos][2] = tbl_pos;	/* モード値 */
				Rx_len1[rx_set_pos] = 3;
				rx_idx = 3;
			} else {
				rx_stat = ST_UA1_WAIT_HEAD1;
			}
			break;

		case ST_UA1_RECEIVING:
			Rx_buf1[rx_set_pos][rx_idx] = rx_data;
			Rx_len1[rx_set_pos]++;
			rx_idx++;

			tbl_pos = Rx_buf1[rx_set_pos][2];	/* モード値 */
			if(Rx_len1[rx_set_pos] < C_07701_length_tbl[tbl_pos]) {
				/* まだ受信完了していない */
				tbl_pos = 0;
			}

			if(tbl_pos != 0) {
				/* 受信完了した */
				rx_set_pos++;
				rx_idx = 0;

				if (rx_set_pos >= UA1_RX_BUF_NUM) {
					rx_set_pos = 0;
				}

				Rx_len1[rx_set_pos] = 0;
				rx_stat = ST_UA1_WAIT_HEAD1;
			}
			break;

		case ST_UA1_WAIT_HEAD1:
		default:
			if (rx_data == UA_HEAD_7701) {
				rx_stat = ST_UA1_WAIT_HEAD2;
			}
			break;
		}
	} else {
		rx_idx = 0;
		Rx_len1[rx_set_pos] = 0;
		rx_stat = ST_UA1_WAIT_HEAD1;
	}

	Rx_status1 = rx_stat;
	Rx_set_pos1 = rx_set_pos;
	Rx_idx1 = rx_idx;
}
#endif /* ADVANCE_ENABLE */

/** 
 * UART1受信ﾃﾞｰﾀ取得処理 
 * 
 * param	*rx_data	[out]	受信ﾃﾞｰﾀ格納ﾊﾞｯﾌｧのﾎﾟｲﾝﾀ 
 * param	*rx_err		[out]	受信ｴﾗｰ情報格納ﾊﾞｯﾌｧのﾎﾟｲﾝﾀ（NULL可）
 * param	data_len	[in]	取得するデータ長（最大長）
 *
 * return	受信ﾃﾞｰﾀ数
 *
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		UART1受信割込みにて格納された受信ﾃﾞｰﾀ、ｴﾗｰ情報を取得する。
 */ 
DEFIUART_LOCAL UI_16 get_uart1_rx_data(UI_8 *rx_data, UI_8 *rx_err, UI_16 data_len)
{
	/* 読み込み可能データサイズ取得 */
	UI_16 rtn_rx_len = RingBuffer_GetCanReadSize(&Rx_buf_int1_mng);
	if(rtn_rx_len > 0){
		/* データありならそれぞれデータ取得 */
		if (data_len < rtn_rx_len) {
			rtn_rx_len = data_len;
		}
		/* 途中で割込みが入るとデータがずれるためこの間は割込み禁止する */
		/* UARTドライバ受信割込み禁止 */
		UartDrv_RxIntrReqEnblCtrl( UARTDRV_CH_ADVANCE, D_OFF );
		
		/* 受信データとエラー情報は同じデータサイズ分格納されている */
		(void)RingBuffer_GetArrayData(&Rx_buf_int1_mng, rx_data, rtn_rx_len, RINGBUF_READEND);
		if (rx_err != NULL) {
			(void)RingBuffer_GetArrayData(&Rx_err_inf1_mng, rx_err, rtn_rx_len, RINGBUF_READEND);
		}
		else {
			/* エラー情報格納バッファが指定されていない場合は読み捨てる	*/
			(void)ReadEndRingBuffer(&Rx_err_inf1_mng, rtn_rx_len);
		}

		/* UARTドライバ受信割込み許可 */
		UartDrv_RxIntrReqEnblCtrl( UARTDRV_CH_ADVANCE, D_ON );
	}

	return rtn_rx_len;
}





/** 
 * UART受信完了割込み
 */ 
__interrupt void Intr_LinUart7_Rx (void)
{
	SI_8 result;
	UI_8 rxFulFlg;
	UI_8 rxData;
	UI_16 can_write_len;
	T_UartDrv_ErrInfo err;
	
	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_ADVANCE_INTR_RX);)

	/* 書き込み可能サイズ取得 */
	can_write_len = RingBuffer_GetCanWriteSize(&Rx_buf_int1_mng);
	/* 受信データ取得 */
	result = UartDrv_GetErr(UARTDRV_CH_ADVANCE, &err);
	if(result == D_OK)
	{
		if( (err.parity_flg  == D_ON) ||	/* パリティエラーフラグ */
			(err.overrun_flg == D_ON) ||	/* オーバランエラーフラグ */
			(err.flaming_flg == D_ON) )		/* フレーミングエラーフラグ */
		{
			/* エラーあり */
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
			/* 書き込み可能サイズ取得 */
			if(can_write_len > 0){
				/* 書き込み可能 */
				(void)RingBuffer_SetByteData(&Rx_buf_int1_mng, 0x00);	/* 受信データは0x00を設定 */
				(void)RingBuffer_SetByteData(&Rx_err_inf1_mng, rxData);
			}
			/* UART送受信処理停止 */
			uartDisable();
			/* UART送受信処理開始 */
			uartEnable();
		}
		else
		{
			/* エラーなし */
			/* 受信データフルフラグの取得 */
			rxFulFlg = UartDrv_GetRxFullFlag( UARTDRV_CH_ADVANCE );
			if(rxFulFlg == D_ON)
			{
				/* 受信データの取得 */
				result = UartDrv_GetRxData(UARTDRV_CH_ADVANCE, &rxData);
				if(result == D_OK)
				{
					if(can_write_len > 0){
						/* 書き込み可能 */
						(void)RingBuffer_SetByteData(&Rx_buf_int1_mng, rxData);
						(void)RingBuffer_SetByteData(&Rx_err_inf1_mng, 0x00);	/* エラーデータは0x00を設定 */
					}
				}
			}
		}
	}
	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_ADVANCE_INTR_RX);)
	return;
}

/** 
 * UART送信完了割込み（送信バッファエンプティ割込み）
 */ 
__interrupt void Intr_LinUart7_Tx (void)
{
	UI_8	data;

	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_ADVANCE_INTR_TX);)

	if (RingBuffer_GetByteData(&Tx_buf_mng, RINGBUF_READEND, &data) == 0) {
		/* 送信データあり */
		/* データ送信 */
		(void)UartDrv_SetTxData(UARTDRV_CH_ADVANCE, data);
	}
	else {
		/* 送信データなし */
		/* UARTドライバ送信割り込み禁止 */
		UartDrv_TxIntrReqEnblCtrl( UARTDRV_CH_ADVANCE, D_OFF );
	}

	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_ADVANCE_INTR_TX);)

	return;
}

/********************************************************/
/* 関数名称		: uartEnable							*/
/* 機能の説明	: UART送受信処理開始					*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
DEFIUART_LOCAL void uartEnable(void)
{
	/* UARTドライバ受信許可(送信も許可される) */
	(void)UartDrv_ReqRxCtrl( UARTDRV_CH_ADVANCE, D_ENABLE );

	/*  UARTドライバ受信割り込み許可 */
	UartDrv_RxIntrReqEnblCtrl( UARTDRV_CH_ADVANCE, D_ON );
}

/********************************************************/
/* 関数名称		: uartDisable							*/
/* 機能の説明	: UART送受信処停止						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
DEFIUART_LOCAL void uartDisable(void)
{
	/* UARTドライバ受信割込み禁止 */
	UartDrv_RxIntrReqEnblCtrl( UARTDRV_CH_ADVANCE, D_OFF );

	/* UARTドライバ送信割込み禁止 */
	UartDrv_TxIntrReqEnblCtrl( UARTDRV_CH_ADVANCE, D_OFF );

	/* UARTドライバ受信禁止(送信も禁止される) */
	(void)UartDrv_ReqRxCtrl( UARTDRV_CH_ADVANCE, D_DISABLE );
}
