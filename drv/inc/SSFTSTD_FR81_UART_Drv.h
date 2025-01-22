/************************************************************************************************/
/* 客先名		:	標準モジュール																*/
/* 機種名		:	SSFT																		*/
/* ﾏｲｺﾝｿﾌﾄﾃｰﾏ名	:	PF																			*/
/*==============================================================================================*/
/* 作成ﾌｧｲﾙ名	:	SSFTSTD_FR81_UART_Drv.h														*/
/* 				:	UARTﾄﾞﾗｲﾊﾞﾍｯﾀﾞ																*/
/*==============================================================================================*/
/* 対象ﾏｲｺﾝ		:	MB91570 Series																*/
/*==============================================================================================*/
/* 作成ﾊﾞｰｼﾞｮﾝ	:	020201																		*/
/* 作成年月日	:	2015.1.29																	*/
/* 作成者		:	k.uchiyama																	*/
/*----------------------------------------------------------------------------------------------*/
/* 変更履歴		:																				*/
/* [010101]		:	新規作成																	*/
/* 12.09.28		:	チャネル2 シフターインジケータUART制御対応									*/
/* 12.10.11		:	チャネル3 インジケータASSY UART制御対応										*/
/* 12.11.10		:	チャネル7 HUD モータASSY UART制御対応										*/
/* [010102]		:	標準IF対応。IF関数の戻り値SI_8化。関数名称変更。							*/
/*				:	送信割込み要求許可情報セット/クリア処理の統合								*/
/*				:	受信割込み要求許可情報セット/クリア処理の統合								*/
/* [010104]		:	内部関数名の変更に伴いドライババージョンの更新								*/
/* [020101]		:	IFの追加（初期化, 通信方式切替, データレジスタアドレス取得)					*/
/* [020201]		:	LCDクロック出力機能を追加する												*/
/************************************************************************************************/
#ifndef __MB91570_UART_DRV_H__
#define __MB91570_UART_DRV_H__

/*** START_INC ***/
/********************************************************************************/
/*   Include File                                                               */
/*------------------------------------------------------------------------------*/
/*      ﾍｯﾀﾞｰﾌｧｲﾙのｲﾝｸﾙｰﾄﾞ文は、下記ﾌｧｲﾙに記載すること                          */
/********************************************************************************/
#include "SSFTxxx_FR81_UART_Drv_Config.h"

/*==============================================================================*/
/*	enum定義（外部公開)															*/
/*==============================================================================*/
/* データビット長	*/
typedef enum {
	UARTDRV_BIT_LEN_7,						/* 7ビット					*/
	UARTDRV_BIT_LEN_8						/* 8ビット					*/
}E_UARTDRV_BIT_LEN;

/* パリティ	*/
typedef enum {
	UARTDRV_PARITY_NONE,					/* パリティなし				*/
	UARTDRV_PARITY_EVEN,					/* 偶数パリティ				*/
	UARTDRV_PARITY_ODD						/* 奇数パリティ				*/
}E_UARTDRV_PARITY;

/* ストップビット長	*/
typedef enum {
	UARTDRV_STOP_BIT_LEN_1,					/* 1ビット					*/
	UARTDRV_STOP_BIT_LEN_2					/* 2ビット					*/
}E_UARTDRV_STOP_BIT_LEN;

/* 転送方向	*/
typedef enum {
	UARTDRV_DIRECTION_LSB_FIRST,			/* LSBファースト			*/
	UARTDRV_DIRECTION_MSB_FIRST				/* MSBファースト			*/
}E_UARTDRV_DIRECTION;

/* ボーレート	*/
typedef enum {
	UARTDRV_BAUD_RATE_9600,					/* 9600bps					*/
	UARTDRV_BAUD_RATE_14400,				/* 14400bps					*/
	UARTDRV_BAUD_RATE_19200,				/* 19200bps					*/
	UARTDRV_BAUD_RATE_38400,				/* 38400bps					*/
	UARTDRV_BAUD_RATE_57600,				/* 57600bps					*/
	UARTDRV_BAUD_RATE_115200				/* 115200bps				*/
}E_UARTDRV_BAUD_RATE;

/*==============================================================================*/
/*	typedef定義（外部公開）														*/
/*==============================================================================*/
typedef struct {
	UI_8	parity_flg;				/* パリティエラーフラグ */
	UI_8	overrun_flg;			/* オーバランエラーフラグ */
	UI_8	flaming_flg;			/* フレーミングエラーフラグ */
}T_UartDrv_ErrInfo;					/* エラー情報 */

typedef struct {
	E_UARTDRV_BIT_LEN			bit_len;			/* ビット長				*/
	E_UARTDRV_PARITY			parity;				/* パリティ				*/
	E_UARTDRV_STOP_BIT_LEN		stop_bit_len;		/* ストップビット長		*/
	E_UARTDRV_DIRECTION			direction;			/* 転送方向				*/
	E_UARTDRV_BAUD_RATE			baud_rate;			/* ボーレート			*/
}T_UartDrv_Protocol;				/* 通信設定情報 */

/* FR81マイコン用型定義	*/
typedef UI_32 T_UartTransDataAddress;


/*==============================================================================*/
/*	定数定義（外部公開）														*/
/*==============================================================================*/
#ifdef	_VERSION_CONST_DEFINE
UI_8 const C_UARTDRV_VER[6] = {		/* UARTドライバ ドライババージョンNo(020201) */
	0x00U,							/* xx"0" */
	0x02U,							/* xx"2" */
	0x00U,							/* yy"0" */
	0x02U,							/* yy"2" */
	0x00U,							/* zz"0" */
	0x01U							/* zz"1" */
};
#endif	/*	_VERSION_CONST_DEFINE	*/

/*==============================================================================*/
/*	関数ﾌﾟﾛﾄﾀｲﾌﾟ宣言（外部公開）												*/
/*==============================================================================*/

#if (UART_DRV_TX == 1U) /* 送信処理 */
extern SI_8 UartDrv_ReqTxCtrl( E_UARTDRV_CH ch, UI_8 act );
extern SI_8 UartDrv_SetTxData( E_UARTDRV_CH ch, UI_8 tx_data );
extern void UartDrv_TxIntrReqEnblCtrl( E_UARTDRV_CH ch, UI_8 act );
extern UI_8 UartDrv_GetTxDataEmpty( E_UARTDRV_CH ch );
#endif /* UART_DRV_TX */

#if (UART_DRV_RX == 1U) /* 受信処理 */
extern SI_8 UartDrv_ReqRxCtrl( E_UARTDRV_CH ch, UI_8 act );
extern SI_8 UartDrv_GetRxData( E_UARTDRV_CH ch, UI_8 *rx_data );
extern void UartDrv_RxIntrReqEnblCtrl( E_UARTDRV_CH ch, UI_8 act );
extern SI_8 UartDrv_GetErr( E_UARTDRV_CH ch, T_UartDrv_ErrInfo *err );
extern void UartDrv_ReqErrClr( E_UARTDRV_CH ch );
extern UI_8 UartDrv_GetRxFullFlag( E_UARTDRV_CH ch );
#endif /* UART_DRV_RX */

extern SI_8 UartDrv_ChangeProtocol(E_UARTDRV_CH ch, const T_UartDrv_Protocol* protocol);
extern T_UartTransDataAddress UartDrv_GetTransDataAddress( E_UARTDRV_CH ch );
extern void UartDrv_InitRequest( E_UARTDRV_CH ch );
extern void UartDrv_Init( E_INIT_TYPE req );
extern void UartDrv_Sleep( void );
extern void UartDrv_Refresh( void );

#endif	/*	__MB91570_UART_DRV_H__	*/

