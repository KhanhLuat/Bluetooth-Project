/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: FR81S								*/
/* Date				: YY/MM/DD							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Katsura						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		:									*/
/* date				:									*/
/********************************************************/

#include <type.h>
#include <system.h>
#include <string.h>
#include "SSFTSTD_Type.h"
#include "SSFTSTD_Macro.h"
#include "mb91570.h"
#include "DTO459_VDV_Gnss_Uart_Drv.h"
#include "dbglog.h"
#include "time_measure.h"

#define GNSS_UART_DRV_LOCAL	static

typedef struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	UPCL:1;
			__BYTE	RESV01:1;
			__BYTE	RESV02:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
			__BYTE	TBIE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} SCR;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:3;
			__BYTE	RESV11:1;
			__BYTE	SBL:1;
			__BYTE	BDS:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} SMR;
	union {
		__BYTE	byte;
		struct {
			__BYTE	REC:1;
			__BYTE	RESV21:1;
			__BYTE	PE:1;
			__BYTE	FRE:1;
			__BYTE	ORE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	TBI:1;
		} bit;
	} SSR;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV30:1;
			__BYTE	ESBL:1;
			__BYTE	INV:1;
			__BYTE	PEN:1;
			__BYTE	P:1;
			__BYTE	L:3;
		} bit;
	} ESCR;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDRL;
		} byte;
		struct {
			__HWORD	RESV1:7;
			__HWORD	D:9;
		} bit9;
		struct {
			__HWORD	RESV1:9;
			__HWORD	D:7;
		} bit7;
		struct {
			__HWORD	RESV1:10;
			__HWORD	D:6;
		} bit6;
		struct {
			__HWORD	RESV1:11;
			__HWORD	D:5;
		} bit5;
	} RDR;
	union {
		__HWORD	hword;
		struct {
			__HWORD	EXT:1;
			__HWORD	BGR:15;
		} bit;
	} BGR;
	__WORD	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV120:1;
			__BYTE	RESV121:1;
			__BYTE	RESV122:1;
			__BYTE	FLSTE:1;
			__BYTE	FRIIE:1;
			__BYTE	FDRQ:1;
			__BYTE	FTIE:1;
			__BYTE	FSEL:1;
		} bit;
	} FCR1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV130:1;
			__BYTE	FLST:1;
			__BYTE	FLD:1;
			__BYTE	FSET:1;
			__BYTE	FCL2:1;
			__BYTE	FCL1:1;
			__BYTE	FE2:1;
			__BYTE	FE1:1;
		} bit;
	} FCR0;
	__HWORD	FBYTE;
} T_Uart_reg;


#define GNSS_BAUDRATE_DEFAULT	GNSS_BAUDRATE_9600_BPS		/* デフォルトボーレート		*/

#define PCLK_FREQ				((UI_32)16 * 1000000)		/* 周辺クロック周波数		*/

/* 各UARTチャンネルごとのUARTレジスタ */
GNSS_UART_DRV_LOCAL volatile T_Uart_reg *const C_Uart_reg[GNSS_UART_DRV_UART_CH_COUNT] = {
	(volatile T_Uart_reg*)&IO_UART0, (volatile T_Uart_reg*)&IO_UART1, (volatile T_Uart_reg*)&IO_UART8, (volatile T_Uart_reg*)&IO_UART9
};

/* 各UARTチャンネルごとのボーレート */
GNSS_UART_DRV_LOCAL E_GNSS_BAUDRATE S_Uart_baudrate[GNSS_UART_DRV_UART_CH_COUNT] = {
	GNSS_BAUDRATE_DEFAULT, GNSS_BAUDRATE_DEFAULT, GNSS_BAUDRATE_DEFAULT, GNSS_BAUDRATE_DEFAULT
};

/* ボーレートジェネレータ設定値 */
GNSS_UART_DRV_LOCAL UI_16 const C_Bgr_value[] = {
	(UI_16)1666, /* (PCLK_FREQ /   9600 - 1), */	/*   9,600bps			*/
	(UI_16)((PCLK_FREQ /  19200) - 1),				/*  19,200bps			*/
	(UI_16)((PCLK_FREQ /  38400) - 1),				/*  38,400bps			*/
	(UI_16)((PCLK_FREQ /  57600) - 1),				/*  57,600bps			*/
	(UI_16)138 /* (PCLK_FREQ / 115200 - 1) */		/* 115,200bps			*/
};


/* 送受信バッファ関連情報 */
GNSS_UART_DRV_LOCAL T_Uart_buf_info uart_buf_info_tbl[GNSS_UART_DRV_UART_CH_COUNT];							/* 各チャンネルごとの送受信バッファ情報	*/
GNSS_UART_DRV_LOCAL UI_32           uart_rx_save_count[GNSS_UART_DRV_UART_CH_COUNT];							/* 受信バッファデータ格納カウント		*/
GNSS_UART_DRV_LOCAL UI_32           uart_rx_load_count[GNSS_UART_DRV_UART_CH_COUNT];							/* 受信バッファデータ読出カウント		*/
GNSS_UART_DRV_LOCAL UI_32           uart_tx_save_count[GNSS_UART_DRV_UART_CH_COUNT];							/* 送信バッファデータ格納カウント		*/
GNSS_UART_DRV_LOCAL UI_32           uart_tx_load_count[GNSS_UART_DRV_UART_CH_COUNT];							/* 送信バッファデータ読出カウント			*/

/* 内部関数プロトタイプ宣言 */
GNSS_UART_DRV_LOCAL void interrupt_rx_common(E_GNSS_UART_DRV_UART_CH uart_ch);
GNSS_UART_DRV_LOCAL void interrupt_tx_common(E_GNSS_UART_DRV_UART_CH uart_ch);
GNSS_UART_DRV_LOCAL void clear_rx_buf(E_GNSS_UART_DRV_UART_CH uart_ch);

/********************************************************/
/* 関数名称		: GNSS UARTドライバ初期化				*/
/* 機能の説明	: 指定チャンネルのUARTドライバを初期化	*/
/*				  する									*/
/* 引数			: (I)uart_ch :UARTチャンネル			*/
/*				  (I)uart_buf_info:送受信バッファ関連情報*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/26								*/
/* 備考			: 										*/
/********************************************************/
void Gnss_uart_drv_Init(E_GNSS_UART_DRV_UART_CH uart_ch, const T_Uart_buf_info *uart_buf_info)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];

	/* このチャンネルの送受信バッファ情報設定 */
	(void)memcpy(&uart_buf_info_tbl[uart_ch], uart_buf_info, sizeof(T_Uart_buf_info));
	uart_rx_save_count[uart_ch] = 0;
	uart_rx_load_count[uart_ch] = 0;
	uart_tx_save_count[uart_ch] = 0;
	uart_tx_load_count[uart_ch] = 0;

    /* UARTリセット */
    uart_reg->SCR.bit.UPCL = 1;
    
	/* 動作モード設定 */
    uart_reg->SMR.bit.MD  = 0;		/* 非同期モード				*/
    uart_reg->SMR.bit.SBL = 0;		/* ストップビット:1bit		*/
    uart_reg->SMR.bit.BDS = 0;		/* LSBファースト			*/
    uart_reg->SMR.bit.SOE = 1;		/* 出力許可					*/

    /* 拡張通信制御レジスタの設定 */
    uart_reg->ESCR.bit.ESBL = 0;	/* ストップビット:1bit		*/
    uart_reg->ESCR.bit.INV  = 0;	/* 反転なし					*/
    uart_reg->ESCR.bit.PEN  = 0;	/* パリティなし				*/
    uart_reg->ESCR.bit.L    = 0;	/* データ長:8bit			*/
}

/********************************************************/
/* 関数名称		: GNSS UARTドライバ開始処理				*/
/* 機能の説明	: 指定チャンネルのUARTドライバの通信	*/
/*				  開始設定を行う。						*/
/* 引数			: (I)uart_ch:UARTチャンネル				*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/26								*/
/* 備考			: 										*/
/********************************************************/
void Gnss_uart_drv_Start(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];
	
	/* ボーレート設定 */
	Gnss_uart_drv_Set_bdrate(uart_ch, S_Uart_baudrate[uart_ch]);

	GNSS_uart_drv_Clr_err_stat(uart_ch);

	/* 制御レジスタ設定 */
    uart_reg->SCR.bit.RIE = 1;		/* 受信割込み許可			*/
    uart_reg->SCR.bit.TXE = 1;		/* 送信許可					*/

    /* UARTリセット */
    uart_reg->SCR.bit.UPCL = 1;
}

/********************************************************/
/* 関数名称		: GNSS UARTドライバ停止処理				*/
/* 機能の説明	: 指定チャンネルのUARTドライバの通信	*/
/*				  停止設定を行う。						*/
/* 引数			: (I)uart_ch:UARTチャンネル				*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/26								*/
/* 備考			: 										*/
/********************************************************/
void Gnss_uart_drv_Stop(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];

	/* 制御レジスタ設定 */
    uart_reg->SCR.bit.RIE = 0;		/* 受信割込み禁止			*/
    uart_reg->SCR.bit.TIE = 0;		/* 送信割込み禁止			*/
    uart_reg->SCR.bit.RXE = 0;		/* 受信禁止					*/
    uart_reg->SCR.bit.TXE = 0;		/* 送信禁止					*/
}

/********************************************************/
/* 関数名称		: 受信停止処理							*/
/* 機能の説明	: 指定チャンネルの受信を停止する。		*/
/* 引数			: (I)uart_ch:UARTチャンネル				*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/26								*/
/* 備考			: 										*/
/********************************************************/
void Gnss_uart_drv_Stop_rx(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];
    uart_reg->SCR.bit.RXE = 0;		/* 受信禁止					*/
}
/********************************************************/
/* 関数名称		: 受信開始処理							*/
/* 機能の説明	: 指定チャンネルの受信を開始する。		*/
/* 引数			: (I)uart_ch:UARTチャンネル				*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/26								*/
/* 備考			: 										*/
/********************************************************/
void Gnss_uart_drv_Start_rx(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];
    uart_reg->SCR.bit.RXE = 1;		/* 受信許可					*/
}

/********************************************************/
/* 関数名称		: GNSS UARTドライバ ボーレート設定処理	*/
/* 機能の説明	: 指定チャンネルのUARTドライバの通信	*/
/*				  ボーレートの設定を行う。				*/
/* 引数			: (I)uart_ch:UARTチャンネル				*/
/* 				: (I)baudrate:ボーレート				*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/26								*/
/* 備考			: 										*/
/********************************************************/
void Gnss_uart_drv_Set_bdrate(E_GNSS_UART_DRV_UART_CH uart_ch, E_GNSS_BAUDRATE baudrate)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];
	
	Gnss_uart_drv_Stop(uart_ch);

	/* ボーレートジェネレータ設定 */
	uart_reg->BGR.hword = C_Bgr_value[baudrate];

    /* UARTリセット */
    uart_reg->SCR.bit.UPCL = 1;

	GNSS_uart_drv_Clr_err_stat(uart_ch);

	/* 制御レジスタ設定 */
    uart_reg->SCR.bit.RIE = 1;		/* 受信割込み許可			*/
    uart_reg->SCR.bit.TXE = 1;		/* 送信許可					*/

	/* 現在のボーレートを保持しておく */
	S_Uart_baudrate[uart_ch] = baudrate;
}

/********************************************************/
/* 関数名称		: GNSS UARTドライバ 受信データ取得		*/
/* 機能の説明	: 指定チャンネルの受信済みデータを取得	*/
/*				  する。								*/
/* 引数			: (I)uart_ch :UARTチャンネル			*/
/* 				: (O)data_buf:受信済みデータ列			*/
/* 				: (O)err_buf :受信エラー情報列(NULL可)	*/
/* 				: (I/O)data_len:受信データサイズ		*/
/* 戻り値		: 処理結果								*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/*   lenにはdata_bufおよびerr_bufのバッファサイズを指定 */
/*	 すること。リターン時、lenにはdata_bufおよびerr_buf */
/*   に格納したデータサイズが設定される。				*/
/*   err_bufには、data_bufに格納した各受信データごとの  */
/*   エラー状態(E_GNSS_UART_ERR_STAT)が格納される。		*/
/********************************************************/
E_GNSS_UART_DRV_RESULT Gnss_uart_drv_Get_rx_data(E_GNSS_UART_DRV_UART_CH uart_ch, UI_8 *data_buf, UI_8 *err_buf, UI_16 *data_len)
{
	T_Uart_buf_info *buf_info = &uart_buf_info_tbl[uart_ch];
	UI_16 return_size;
	UI_16 cpy_size[2];
	UI_32 data_count;
	UI_16 load_index;
	
	/* 受信済みデータ有無チェック */
	data_count = (uart_rx_save_count[uart_ch] - uart_rx_load_count[uart_ch]);
	if(data_count == 0) {
		/* 受信データなし */
		return GNSS_UART_DRV_ERR_NOT_RECIEVE_DATA;
	}
	

	/* 受信バッファフルチェック */
	if(data_count > (UI_32)buf_info->rx_buf_size) {
		/* 受信バッファクリア */
		clear_rx_buf(uart_ch);
		return GNSS_UART_DRV_ERR_BUFFERFULL;
	}

	/* 今回コール元に返すデータサイズ決定 */
	return_size = (UI_16)data_count;
	if(return_size > *data_len) {
		return_size = *data_len;
	}

	/* 循環処理が必要かどうかの判定 */
	load_index = (UI_16)(uart_rx_load_count[uart_ch] % buf_info->rx_buf_size);
	if((load_index + return_size) <= buf_info->rx_buf_size) {
		/* 不要なためmemcpy1回で転送 */
		(void)memcpy(data_buf, &buf_info->rx_buf[load_index], return_size);
		if (err_buf != NULL) {
			(void)memcpy(err_buf,  &buf_info->er_buf[load_index], return_size);
		}
	}
	else {
		/* バッファ終端までと先頭からの2回のmemcpyで転送 */
		cpy_size[0] = buf_info->rx_buf_size - load_index;
		cpy_size[1] = return_size - cpy_size[0];
		(void)memcpy(data_buf,               &buf_info->rx_buf[load_index], cpy_size[0]);
		(void)memcpy(&data_buf[cpy_size[0]], &buf_info->rx_buf[0],          cpy_size[1]);
		if (err_buf != NULL) {
			(void)memcpy(err_buf,                &buf_info->er_buf[load_index], cpy_size[0]);
			(void)memcpy(&err_buf[cpy_size[0]],  &buf_info->er_buf[0],          cpy_size[1]);
		}
	}
	
	/* 受信データ読み出しカウント更新 */
	uart_rx_load_count[uart_ch] += return_size;

	/* データサイズをリターン */
	*data_len = return_size;
	return GNSS_UART_DRV_OK;
}

/********************************************************/
/* 関数名称		: GNSS UARTドライバ 受信データクリア処理*/
/* 機能の説明	: 指定チャンネルに受信済みデータをクリア*/
/*                する。								*/
/* 引数			: (I)uart_ch :UARTチャンネル			*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
void Gnss_uart_drv_Clear_rx_data(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	uart_rx_load_count[uart_ch] = uart_rx_save_count[uart_ch];
}

/********************************************************/
/* 関数名称		: GNSS UARTドライバ 送信可能データ長取得*/
/* 機能の説明	: 指定チャンネルの送信可能なデータ長を	*/
/*				  取得する。							*/
/* 引数			: (I)uart_ch :UARTチャンネル			*/
/* 戻り値		: 送信可能データ長						*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
UI_16 Gnss_uart_drv_get_can_set_tx_data_len(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	UI_16	data_count;
	UI_16	data_len;

	/* 送信データバッファに設定されているデータ数を取得 */
	data_count = (UI_16)(uart_tx_save_count[uart_ch] - uart_tx_load_count[uart_ch]);

	/* 送信可能データ長を取得	*/
	data_len = uart_buf_info_tbl[uart_ch].tx_buf_size - data_count;
	
	return data_len;
}

/********************************************************/
/* 関数名称		: GNSS UARTドライバ 送信データ設定		*/
/* 機能の説明	: 指定チャンネルに送信するデータを設定	*/
/*				  する。								*/
/* 引数			: (I)uart_ch :UARTチャンネル			*/
/* 				: (I)data_buf:送信データ列				*/
/* 				: (I)data_len:送信データサイズ			*/
/* 戻り値		: 処理結果								*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
E_GNSS_UART_DRV_RESULT Gnss_uart_drv_Set_tx_data(E_GNSS_UART_DRV_UART_CH uart_ch, const UI_8 *data_buf, UI_8 data_len)
{
	/* このチャンネルのレジスタ情報とバッファ情報決定 */
	volatile T_Uart_reg      *uart_reg = C_Uart_reg[uart_ch];
	T_Uart_buf_info *buf_info = &uart_buf_info_tbl[uart_ch];
	UI_16 cpy_size[2];
	UI_32 data_count;
	UI_16 save_index;

	if(data_len > buf_info->tx_buf_size) {
		/* バッファサイズを超えているので送信不可 */
		return GNSS_UART_DRV_ERR_DATA_SEND;
	}

	/* 現在のデータ数と格納インデックス決定 */
	data_count = uart_tx_save_count[uart_ch] - uart_tx_load_count[uart_ch];
	save_index = (UI_16)(uart_tx_save_count[uart_ch] % buf_info->tx_buf_size);


	/* 送信バッファオーバーフローチェック */
	if((data_count + data_len) > (UI_32)buf_info->tx_buf_size) {
		return GNSS_UART_DRV_ERR_BUFFERFULL;
	}
	/* 循環処理が必要かどうかの判定 */
	if((save_index + data_len) <= buf_info->tx_buf_size) {
		/* 不要なためmemcpy1回で転送 */
		(void)memcpy(&buf_info->tx_buf[save_index], data_buf, (size_t)data_len);
	}
	else {
		/* バッファ終端までと先頭からの2回のmemcpyで転送 */
		cpy_size[0] = buf_info->tx_buf_size - save_index;
		cpy_size[1] = data_len - cpy_size[0];
		(void)memcpy(&buf_info->tx_buf[save_index], &data_buf[0],           cpy_size[0]);
		(void)memcpy(&buf_info->tx_buf[0],          &data_buf[cpy_size[0]], cpy_size[1]);
	}

	/* 送信データ格納数更新 */
	uart_tx_save_count[uart_ch] += data_len;

    uart_reg->SCR.bit.RXE = 1;      /* 受信許可				*/
	uart_reg->SCR.bit.TIE = 1;		/* 送信割り込み許可		*/
	
	return GNSS_UART_DRV_OK;
}

/********************************************************/
/* 関数名称		: エラー状態取得						*/
/* 機能の説明	: 指定チャンネルのエラー状態を取得する。*/
/* 引数			: (I)uart_ch :UARTチャンネル			*/
/* 戻り値		: エラー状態							*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
E_GNSS_UART_ERR_STAT Gnss_uart_drv_Get_err_stat(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];
	
	/* パリティエラー有無チェック */
	if(uart_reg->SSR.bit.PE == 1) {
		return GNSS_UART_ERR_STAT_PE;
	}

	/* フレーミングエラー有無チェック */
	if(uart_reg->SSR.bit.FRE == 1) {
		return GNSS_UART_ERR_STAT_FRE;
	}

	/* オーバーランエラー有無チェック */
	if(uart_reg->SSR.bit.ORE == 1) {
		return GNSS_UART_ERR_STAT_ORE;
	}

	return GNSS_UART_ERR_STAT_NONE;
}

/********************************************************/
/* 関数名称		: エラーフラグクリア					*/
/* 機能の説明	: 指定チャンネルのエラーフラグをクリア	*/
/*				  する。								*/
/* 引数			: (I)uart_ch :UARTチャンネル			*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
void GNSS_uart_drv_Clr_err_stat(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];

	/* エラーフラグクリア */
	uart_reg->SSR.bit.REC = 1;
}

/********************************************************/
/* 関数名称		: 受信バッファクリア					*/
/* 機能の説明	: 受信バッファをクリアする。			*/
/* 引数			: (I)uart_ch :UARTチャンネル			*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
void clear_rx_buf(E_GNSS_UART_DRV_UART_CH uart_ch)
{

	/* 読み出し数を格納数に合わせる */
	uart_rx_load_count[uart_ch] = uart_rx_save_count[uart_ch];

}

/********************************************************/
/* 関数名称		: UART CH0 受信割込み関数				*/
/* 機能の説明	: UART CH0の受信割込み処理関数			*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_rx0(void)
{
	/* チャンネル共通関数コール */
	interrupt_rx_common(GNSS_UART_DRV_UART_CH_0);
}
/********************************************************/
/* 関数名称		: UART CH0 送信割込み関数				*/
/* 機能の説明	: UART CH0の送信割込み処理関数			*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_tx0(void)
{
	/* チャンネル共通関数コール */
	interrupt_tx_common(GNSS_UART_DRV_UART_CH_0);
}

/********************************************************/
/* 関数名称		: UART CH1 受信割込み関数				*/
/* 機能の説明	: UART CH1の受信割込み処理関数			*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_rx1(void)
{
	/* チャンネル共通関数コール */
	interrupt_rx_common(GNSS_UART_DRV_UART_CH_1);
}
/********************************************************/
/* 関数名称		: UART CH1 送信割込み関数				*/
/* 機能の説明	: UART CH1の送信割込み処理関数			*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_tx1(void)
{
	/* チャンネル共通関数コール */
	interrupt_tx_common(GNSS_UART_DRV_UART_CH_1);
}

/********************************************************/
/* 関数名称		: UART CH8 受信割込み関数				*/
/* 機能の説明	: UART CH8の受信割込み処理関数			*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_rx8(void)
{
	/* チャンネル共通関数コール */
	interrupt_rx_common(GNSS_UART_DRV_UART_CH_8);
}
/********************************************************/
/* 関数名称		: UART CH8 送信割込み関数				*/
/* 機能の説明	: UART CH8の送信割込み処理関数			*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_tx8(void)
{
	/* チャンネル共通関数コール */
	interrupt_tx_common(GNSS_UART_DRV_UART_CH_8);
}

/********************************************************/
/* 関数名称		: UART CH9 受信割込み関数				*/
/* 機能の説明	: UART CH9の受信割込み処理関数			*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_rx9(void)
{
	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_GNSS_INTR_RX);)

	/* チャンネル共通関数コール */
	interrupt_rx_common(GNSS_UART_DRV_UART_CH_9);

	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_GNSS_INTR_RX);)
}
/********************************************************/
/* 関数名称		: UART CH9 送信割込み関数				*/
/* 機能の説明	: UART CH9の送信割込み処理関数			*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_tx9(void)
{
	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_GNSS_INTR_TX);)

	/* チャンネル共通関数コール */
	interrupt_tx_common(GNSS_UART_DRV_UART_CH_9);

	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_GNSS_INTR_TX);)
}

/********************************************************/
/* 関数名称		: UARTチャンネル共通受信割込み関数		*/
/* 機能の説明	: UART各チャンネル共通の受信割込み処理	*/
/*				  関数									*/
/* 引数			: (I)uart_ch :UARTチャンネル			*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
void interrupt_rx_common(E_GNSS_UART_DRV_UART_CH uart_ch)
{

	/* このチャンネルのレジスタ情報とバッファ情報決定 */
	volatile T_Uart_reg      *uart_reg = C_Uart_reg[uart_ch];
	T_Uart_buf_info *buf_info = &uart_buf_info_tbl[uart_ch];
	UI_32 data_count;
	UI_16 save_index;
	volatile UI_8 data_tmp;
	
	/* データ数と格納インデックス決定 */
	data_count = uart_rx_save_count[uart_ch] - uart_rx_load_count[uart_ch];
	save_index = (UI_16)(uart_rx_save_count[uart_ch] % buf_info->rx_buf_size);

	/* 既に受信バッファがいっぱい */
	if(data_count >= (UI_32)buf_info->rx_buf_size) {
		if((uart_reg->SSR.bit.ORE == 1) || (uart_reg->SSR.bit.FRE == 1) || (uart_reg->SSR.bit.PE == 1)) {
			/* エラーフラグクリア */
			uart_reg->SSR.bit.REC = 1;
		}
		else {
			/* 受信データ読み捨て	*/
			data_tmp = uart_reg->RDR.byte.RDRL;
		}
		return;
	}

	/* オーバーランエラー有無チェック */
	if((uart_reg->SSR.bit.ORE == 1) || (uart_reg->SSR.bit.FRE == 1) || (uart_reg->SSR.bit.PE == 1)) {
		/* エラー情報を受信エラーバッファへ格納 */
		if(uart_reg->SSR.bit.ORE == 1) {
			buf_info->er_buf[save_index] = GNSS_UART_ERR_STAT_ORE;
		}
		else if(uart_reg->SSR.bit.FRE == 1) {
			buf_info->er_buf[save_index] = GNSS_UART_ERR_STAT_FRE;
		}
		else if(uart_reg->SSR.bit.PE == 1) {
			buf_info->er_buf[save_index] = GNSS_UART_ERR_STAT_PE;
		}
		else {
			/* nop */
		}
		buf_info->rx_buf[save_index]  = 0;

		/* エラーフラグクリア */
		uart_reg->SSR.bit.REC = 1;
	}
	else {
		/* 受信データ取得して内部バッファへ格納 */
		buf_info->rx_buf[save_index] = uart_reg->RDR.byte.RDRL;
		buf_info->er_buf[save_index] = GNSS_UART_ERR_STAT_NONE;
	}

	/* 受信データ格納数インクリメント */
	uart_rx_save_count[uart_ch]++;

}

/********************************************************/
/* 関数名称		: UARTチャンネル共通送信割込み関数		*/
/* 機能の説明	: UART各チャンネル共通の送信割込み処理	*/
/*				  関数									*/
/* 引数			: (I)uart_ch :UARTチャンネル			*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
void interrupt_tx_common(E_GNSS_UART_DRV_UART_CH uart_ch)
{

	UI_8  byte_data;
	UI_32 data_count;
	UI_16 load_index;
	
	/* このチャンネルのレジスタ情報とバッファ情報決定 */
	volatile T_Uart_reg      *uart_reg = C_Uart_reg[uart_ch];
	T_Uart_buf_info *buf_info = &uart_buf_info_tbl[uart_ch];

	data_count = uart_tx_save_count[uart_ch] - uart_tx_load_count[uart_ch];
	load_index = (UI_16)(uart_tx_load_count[uart_ch] % buf_info->tx_buf_size);
	
	/* 送信待ちデータが存在するかどうかの判定 */
	if(data_count > 0) {
		/* 送信データを送信データレジスタに設定 */
		byte_data = buf_info->tx_buf[load_index];

		/* 送信待ちデータ読出し数更新 */
		uart_tx_load_count[uart_ch]++;

		uart_reg->RDR.byte.RDRL = byte_data;
	}
	else {
    	uart_reg->SCR.bit.TIE = 0;		/* 送信禁止					*/
	}

}
