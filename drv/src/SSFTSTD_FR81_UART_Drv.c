/********************************************************************************/
/* 客先名		:	標準モジュール												*/
/* 機種名		:	SSFT														*/
/* ﾏｲｺﾝｿﾌﾄﾃｰﾏ名	:	PF															*/
/*==============================================================================*/
/* 作成ﾌｧｲﾙ名	:	SSFTSTD_FR81_UART_Drv.c										*/
/* 				:	UARTﾄﾞﾗｲﾊﾞﾓｼﾞｭｰﾙ											*/
/*==============================================================================*/
/* 対象ﾏｲｺﾝ		:	MB91570 Series												*/
/*==============================================================================*/
/* 作成ﾊﾞｰｼﾞｮﾝ	:	020201														*/
/* 作成年月日	:	2015.1.29													*/
/* 作成者		:	k.uchiyama													*/
/*------------------------------------------------------------------------------*/
/* 変更履歴		:																*/
/* [010101]		:	新規作成													*/
/* [010102]		:	標準IF対応。IF関数の戻り値SI_8化。関数名称変更。			*/
/*				:	送信割込み要求許可情報セット/クリア処理の統合				*/
/*				:	受信割込み要求許可情報セット/クリア処理の統合				*/
/* [010104]		:	内部関数名の変更											*/
/* [020101]		:	IFの追加（初期化, 通信方式切替, データレジスタアドレス取得)	*/
/* [020201]		:	LCDクロック出力機能を追加する								*/
/********************************************************************************/

/*** START_INC ***/
/********************************************************************************/
/*   Include File                                                               */
/*------------------------------------------------------------------------------*/
/*      ﾍｯﾀﾞｰﾌｧｲﾙのｲﾝｸﾙｰﾄﾞ文は、下記ﾌｧｲﾙに記載すること                          */
/********************************************************************************/
#include "SSFTSTD_Type.h"
#include "SSFTSTD_Macro.h"
#include "mb91570.h"

#define __MB91570_UART_DRV_INTERNAL__
#include "SSFTSTD_FR81_UART_Drv.h"


/********************************************************************************/
/*	内部マクロ定義																*/
/********************************************************************************/
#define SSR_PARITY_MASK			(0x80U)		/* パリティエラーフラグ抽出マスク */
#define SSR_OVERRUN_MASK		(0x40U)		/* オーバーランエラーフラグ抽出マスク */
#define SSR_FLAMING_MASK		(0x20U)		/* フレーミングエラーフラグ抽出マスク */
#define SSR_RDRF_MASK			(0x10U)		/* 受信データフルフラグ抽出マスク */
#define SSR_TDRE_MASK			(0x08U)		/* 送信データエンプティフラグ抽出マスク */
#define SSR_TIE_SET				(0x01U)		/* 送信割込み要求許可セットデータ */
#define SSR_TIE_CLR				(0xFEU)		/* 送信割込み要求許可クリアデータ */
#define SSR_RIE_SET				(0x02U)		/* 受信割込み要求許可セットデータ */
#define SSR_RIE_CLR				(0xFDU)		/* 受信割込み要求許可クリアデータ */
#define SMR_UPCL_SET			(0x04U)		/* プログラマブルクリアビットセットデータ */
#define SMR_MD_MASK				(0xC0U)		/* 動作モード抽出マスク */
#define DUMMY_WRITE				(0x00U)		/* UART送信用ダミーライトデータ */
#define SCR_RX_ERR_CLR_BIT_ON	(0x04U)		/* 受信エラーフラグクリアビット */
#define UARTDRV_MODE_UART		(0x00U)		/* Uartドライバ:非同期モード(動作モード0)で使用 */
#define UARTDRV_MODE_CLOCK		(0x80U)		/* Uartドライバ:同期モード(クロック出力)(動作モード2)で使用 */

#define SCR_PEN_CLR				(0x7FU)		/* パリティ許可クリアデータ(パリティなし) */
#define SCR_PEN_SET				(0x80U)		/* パリティ許可セットデータ(パリティあり) */
#define SCR_P_CLR				(0xBFU)		/* パリティ選択クリアデータ(偶数パリティ) */
#define SCR_P_SET				(0x40U)		/* パリティ選択セットデータ(奇数パリティ) */
#define SCR_SBL_CLR				(0xDFU)		/* ストップビット長選択クリアデータ(1ビット) */
#define SCR_SBL_SET				(0x20U)		/* ストップビット長選択セットデータ(2ビット) */
#define SCR_CL_CLR				(0xEFU)		/* データ長選択クリアデータ(7ビット) */
#define SCR_CL_SET				(0x10U)		/* データ長選択セットデータ(8ビット) */
#define SSR_BDS_CLR				(0xFBU)		/* 転送方向選択クリアデータ(LSBファースト) */
#define SSR_BDS_SET				(0x04U)		/* 転送方向選択セットデータ(MSBファースト) */

/*==============================================================================*/
/*	関数ﾌﾟﾛﾄﾀｲﾌﾟ宣言（外部非公開)												*/
/*==============================================================================*/
static SI_8 uart_drv_init( E_UARTDRV_CH ch );/* UARTドライバ関連レジスタ初期化処理 */
static SI_8 uart_drv_stop( E_UARTDRV_CH ch );/* UARTドライバ関連レジスタ停止処理 */
#if (UART_DRV_TX == 1U) /* 送信処理 */
static void uart_drv_txdrive( E_UARTDRV_CH ch );	/* UARTドライバ送信起動処理 */
static void uart_drv_txstop( E_UARTDRV_CH ch );		/* UARTドライバ送信停止処理 */
#endif /* UART_DRV_TX */
#if (UART_DRV_RX == 1U) /* 受信処理 */
static void uart_drv_rxdrive( E_UARTDRV_CH ch );	/* UARTドライバ受信起動処理 */
static void uart_drv_rxstop( E_UARTDRV_CH ch );		/* UARTドライバ受信停止処理 */
#endif /* UART_DRV_RX */
static UI_8 uart_drv_get_scr_act_value( E_UARTDRV_CH ch );
static UI_8 uart_drv_get_ssr_act_value( E_UARTDRV_CH ch );
static UI_16 uart_drv_get_bgr_act_value( E_UARTDRV_CH ch );

/*==============================================================================*/
/*	内部定数																	*/
/*==============================================================================*/
/* シリアルコントロールレジスタアドレステーブル */
static __io volatile UI_8 *volatile const C_SerialCtrlReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_SCR2),						/* SCR2のアドレス */
	&(LIN_UART3_SCR3),						/* SCR3のアドレス */
	&(LIN_UART4_SCR4),						/* SCR4のアドレス */
	&(LIN_UART5_SCR5),						/* SCR5のアドレス */
	&(LIN_UART6_SCR6),						/* SCR6のアドレス */
	&(LIN_UART7_SCR7)						/* SCR7のアドレス */
};

/* シリアルモードレジスタアドレステーブル */
static __io volatile UI_8 *volatile const C_SerialModeReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_SMR2),						/* SMR2のアドレス */
	&(LIN_UART3_SMR3),						/* SMR3のアドレス */
	&(LIN_UART4_SMR4),						/* SMR4のアドレス */
	&(LIN_UART5_SMR5),						/* SMR5のアドレス */
	&(LIN_UART6_SMR6),						/* SMR6のアドレス */
	&(LIN_UART7_SMR7)						/* SMR7のアドレス */
};

/* シリアルステータスレジスタアドレステーブル */
static __io volatile UI_8 *volatile const C_SerialStatReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_SSR2),						/* SSR2のアドレス */
	&(LIN_UART3_SSR3),						/* SSR3のアドレス */
	&(LIN_UART4_SSR4),						/* SSR4のアドレス */
	&(LIN_UART5_SSR5),						/* SSR5のアドレス */
	&(LIN_UART6_SSR6),						/* SSR6のアドレス */
	&(LIN_UART7_SSR7)						/* SSR7のアドレス */
};

/* 受信データレジスタアドレステーブル */
static __io volatile UI_8 *volatile const C_ReceiveDataReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_RDR2),						/* RDR2のアドレス */
	&(LIN_UART3_RDR3),						/* RDR3のアドレス */
	&(LIN_UART4_RDR4),						/* RDR4のアドレス */
	&(LIN_UART5_RDR5),						/* RDR5のアドレス */
	&(LIN_UART6_RDR6),						/* RDR6のアドレス */
	&(LIN_UART7_RDR7)						/* RDR7のアドレス */
};

/* 送信データレジスタアドレステーブル */
static __io volatile UI_8 *volatile const C_TransDataReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_TDR2),						/* TDR2のアドレス */
	&(LIN_UART3_TDR3),						/* TDR3のアドレス */
	&(LIN_UART4_TDR4),						/* TDR4のアドレス */
	&(LIN_UART5_TDR5),						/* TDR5のアドレス */
	&(LIN_UART6_TDR6),						/* TDR6のアドレス */
	&(LIN_UART7_TDR7)						/* TDR7のアドレス */
};
	
/* 拡張ステータス制御レジスタアドレステーブル */
static __io volatile UI_8 *volatile const C_ExtendCtrlReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_ESCR2),						/* ESCR2のアドレス */
	&(LIN_UART3_ESCR3),						/* ESCR3のアドレス */
	&(LIN_UART4_ESCR4),						/* ESCR4のアドレス */
	&(LIN_UART5_ESCR5),						/* ESCR5のアドレス */
	&(LIN_UART6_ESCR6),						/* ESCR6のアドレス */
	&(LIN_UART7_ESCR7)						/* ESCR7のアドレス */
};

/* 拡張通信コントロールレジスタアドレステーブル */
static __io volatile UI_8 *volatile const C_ExtendCommunicationReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_ECCR2),						/* ECCR2のアドレス */
	&(LIN_UART3_ECCR3),						/* ECCR3のアドレス */
	&(LIN_UART4_ECCR4),						/* ECCR4のアドレス */
	&(LIN_UART5_ECCR5),						/* ECCR5のアドレス */
	&(LIN_UART6_ECCR6),						/* ECCR6のアドレス */
	&(LIN_UART7_ECCR7)						/* ECCR7のアドレス */
};

/* ボーレートジェネレータレジスタアドレステーブル */
static __io volatile UI_16 *volatile const C_BaudRateReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_BGR2),						/* BGR2のアドレス */
	&(LIN_UART3_BGR3),						/* BGR3のアドレス */
	&(LIN_UART4_BGR4),						/* BGR4のアドレス */
	&(LIN_UART5_BGR5),						/* BGR5のアドレス */
	&(LIN_UART6_BGR6),						/* BGR6のアドレス */
	&(LIN_UART7_BGR7)						/* BGR7のアドレス */
};

/* シリアルコントロールレジスタドライバ初期化テーブル */
static UI_8 const C_SerialCtrlRegInitTbl[UARTDRV_CH_NUMBER] =
{
	INIT_SCR2,								/* SCR2のドライバ初期設定値 */
	INIT_SCR3,								/* SCR3のドライバ初期設定値 */
	INIT_SCR4,								/* SCR4のドライバ初期設定値 */
	INIT_SCR5,								/* SCR5のドライバ初期設定値 */
	INIT_SCR6,								/* SCR6のドライバ初期設定値 */
	INIT_SCR7								/* SCR7のドライバ初期設定値 */
};

/* シリアルコントロールレジスタドライバ起動設定テーブル */
static UI_8 const C_SerialCtrlRegDrvActTbl[UARTDRV_CH_NUMBER] =
{
	DRV_ACT_SCR2,							/* SCR2のドライバ起動設定値 */
	DRV_ACT_SCR3,							/* SCR3のドライバ起動設定値 */
	DRV_ACT_SCR4,							/* SCR4のドライバ起動設定値 */
	DRV_ACT_SCR5,							/* SCR5のドライバ起動設定値 */
	DRV_ACT_SCR6,							/* SCR6のドライバ起動設定値 */
	DRV_ACT_SCR7							/* SCR7のドライバ起動設定値 */
};

/* シリアルコントロールレジスタドライバ停止設定テーブル */
static UI_8 const C_SerialCtrlRegDrvStopTbl[UARTDRV_CH_NUMBER] =
{
	DRV_STOP_SCR2,							/* SCR2のドライバ停止設定値 */
	DRV_STOP_SCR3,							/* SCR3のドライバ停止設定値 */
	DRV_STOP_SCR4,							/* SCR4のドライバ停止設定値 */
	DRV_STOP_SCR5,							/* SCR5のドライバ停止設定値 */
	DRV_STOP_SCR6,							/* SCR6のドライバ停止設定値 */
	DRV_STOP_SCR7							/* SCR7のドライバ停止設定値 */
};

/* シリアルコントロールレジスタドライバ停止テーブル */
static UI_8 const C_SerialCtrlRegStopTbl[UARTDRV_CH_NUMBER] =
{
	STOP_SCR2,								/* SCR2のドライバ停止設定値 */
	STOP_SCR3,								/* SCR3のドライバ停止設定値 */
	STOP_SCR4,								/* SCR4のドライバ停止設定値 */
	STOP_SCR5,								/* SCR5のドライバ停止設定値 */
	STOP_SCR6,								/* SCR6のドライバ停止設定値 */
	STOP_SCR7								/* SCR7のドライバ停止設定値 */
};

/* シリアルモードレジスタドライバ初期化テーブル */
static UI_8 const C_SerialModeRegInitTbl[UARTDRV_CH_NUMBER] =
{
	INIT_SMR2,								/* SMR2のドライバ初期設定値 */
	INIT_SMR3,								/* SMR3のドライバ初期設定値 */
	INIT_SMR4,								/* SMR4のドライバ初期設定値 */
	INIT_SMR5,								/* SMR5のドライバ初期設定値 */
	INIT_SMR6,								/* SMR6のドライバ初期設定値 */
	INIT_SMR7								/* SMR7のドライバ初期設定値 */
};

/* シリアルモードレジスタドライバ起動設定テーブル */
static UI_8 const C_SerialModeRegDrvActTbl[UARTDRV_CH_NUMBER] =
{
	DRV_ACT_SMR2,							/* SMR2のドライバ起動設定値 */
	DRV_ACT_SMR3,							/* SMR3のドライバ起動設定値 */
	DRV_ACT_SMR4,							/* SMR4のドライバ起動設定値 */
	DRV_ACT_SMR5,							/* SMR5のドライバ起動設定値 */
	DRV_ACT_SMR6,							/* SMR6のドライバ起動設定値 */
	DRV_ACT_SMR7							/* SMR7のドライバ起動設定値 */
};

/* シリアルモードレジスタドライバ停止設定テーブル */
static UI_8 const C_SerialModeRegDrvStopTbl[UARTDRV_CH_NUMBER] =
{
	DRV_STOP_SMR2,							/* SMR2のドライバ停止設定値 */
	DRV_STOP_SMR3,							/* SMR3のドライバ停止設定値 */
	DRV_STOP_SMR4,							/* SMR4のドライバ停止設定値 */
	DRV_STOP_SMR5,							/* SMR5のドライバ停止設定値 */
	DRV_STOP_SMR6,							/* SMR6のドライバ停止設定値 */
	DRV_STOP_SMR7							/* SMR7のドライバ停止設定値 */
};

/* シリアルモードレジスタドライバ停止テーブル */
static UI_8 const C_SerialModeRegStopTbl[UARTDRV_CH_NUMBER] =
{
	STOP_SMR2,								/* SMR2のドライバ停止設定値 */
	STOP_SMR3,								/* SMR3のドライバ停止設定値 */
	STOP_SMR4,								/* SMR4のドライバ停止設定値 */
	STOP_SMR5,								/* SMR5のドライバ停止設定値 */
	STOP_SMR6,								/* SMR6のドライバ停止設定値 */
	STOP_SMR7								/* SMR7のドライバ停止設定値 */
};

/* シリアルステータスレジスタドライバ初期化テーブル */
static UI_8 const C_SerialStatRegInitTbl[UARTDRV_CH_NUMBER] =
{
	INIT_SSR2,								/* SSR2のドライバ初期設定値 */
	INIT_SSR3,								/* SSR3のドライバ初期設定値 */
	INIT_SSR4,								/* SSR4のドライバ初期設定値 */
	INIT_SSR5,								/* SSR5のドライバ初期設定値 */
	INIT_SSR6,								/* SSR6のドライバ初期設定値 */
	INIT_SSR7								/* SSR7のドライバ初期設定値 */
};

/* シリアルステータスレジスタドライバ起動設定テーブル */
static UI_8 const C_SerialStatRegDrvActTbl[UARTDRV_CH_NUMBER] =
{
	DRV_ACT_SSR2,							/* SSR2のドライバ起動設定値 */
	DRV_ACT_SSR3,							/* SSR3のドライバ起動設定値 */
	DRV_ACT_SSR4,							/* SSR4のドライバ起動設定値 */
	DRV_ACT_SSR5,							/* SSR5のドライバ起動設定値 */
	DRV_ACT_SSR6,							/* SSR6のドライバ起動設定値 */
	DRV_ACT_SSR7							/* SSR7のドライバ起動設定値 */
};

/* シリアルステータスレジスタドライバ停止設定テーブル */
static UI_8 const C_SerialStatRegDrvStopTbl[UARTDRV_CH_NUMBER] =
{
	DRV_STOP_SSR2,							/* SSR2のドライバ停止設定値 */
	DRV_STOP_SSR3,							/* SSR3のドライバ停止設定値 */
	DRV_STOP_SSR4,							/* SSR4のドライバ停止設定値 */
	DRV_STOP_SSR5,							/* SSR5のドライバ停止設定値 */
	DRV_STOP_SSR6,							/* SSR6のドライバ停止設定値 */
	DRV_STOP_SSR7							/* SSR7のドライバ停止設定値 */
};

/* シリアルステータスレジスタドライバ停止テーブル */
static UI_8 const C_SerialStatRegStopTbl[UARTDRV_CH_NUMBER] =
{
	STOP_SSR2,								/* SSR2のドライバ停止設定値 */
	STOP_SSR3,								/* SSR3のドライバ停止設定値 */
	STOP_SSR4,								/* SSR4のドライバ停止設定値 */
	STOP_SSR5,								/* SSR5のドライバ停止設定値 */
	STOP_SSR6,								/* SSR6のドライバ停止設定値 */
	STOP_SSR7								/* SSR7のドライバ停止設定値 */
};

/* 拡張ステータス制御レジスタドライバ初期化テーブル */
static UI_8 const C_ExtendCtrlRegInitTbl[UARTDRV_CH_NUMBER] =
{
	INIT_ESCR2,								/* ESCR2のドライバ初期設定値 */
	INIT_ESCR3,								/* ESCR3のドライバ初期設定値 */
	INIT_ESCR4,								/* ESCR4のドライバ初期設定値 */
	INIT_ESCR5,								/* ESCR5のドライバ初期設定値 */
	INIT_ESCR6,								/* ESCR6のドライバ初期設定値 */
	INIT_ESCR7								/* ESCR7のドライバ初期設定値 */
};

/* 拡張ステータス制御レジスタドライバ起動設定テーブル */
static UI_8 const C_ExtendCtrlRegDrvActTbl[UARTDRV_CH_NUMBER] =
{
	DRV_ACT_ESCR2,							/* ESCR2のドライバ起動設定値 */
	DRV_ACT_ESCR3,							/* ESCR3のドライバ起動設定値 */
	DRV_ACT_ESCR4,							/* ESCR4のドライバ起動設定値 */
	DRV_ACT_ESCR5,							/* ESCR5のドライバ起動設定値 */
	DRV_ACT_ESCR6,							/* ESCR6のドライバ起動設定値 */
	DRV_ACT_ESCR7							/* ESCR7のドライバ起動設定値 */
};

/* 拡張ステータス制御レジスタドライバ停止設定テーブル */
static UI_8 const C_ExtendCtrlRegDrvStopTbl[UARTDRV_CH_NUMBER] =
{
	DRV_STOP_ESCR2,							/* ESCR2のドライバ停止設定値 */
	DRV_STOP_ESCR3,							/* ESCR3のドライバ停止設定値 */
	DRV_STOP_ESCR4,							/* ESCR4のドライバ停止設定値 */
	DRV_STOP_ESCR5,							/* ESCR5のドライバ停止設定値 */
	DRV_STOP_ESCR6,							/* ESCR6のドライバ停止設定値 */
	DRV_STOP_ESCR7							/* ESCR7のドライバ停止設定値 */
};

/* 拡張ステータス制御レジスタドライバ停止テーブル */
static UI_8 const C_ExtendCtrlRegStopTbl[UARTDRV_CH_NUMBER] =
{
	STOP_ESCR2,								/* ESCR2のドライバ停止設定値 */
	STOP_ESCR3,								/* ESCR3のドライバ停止設定値 */
	STOP_ESCR4,								/* ESCR4のドライバ停止設定値 */
	STOP_ESCR5,								/* ESCR5のドライバ停止設定値 */
	STOP_ESCR6,								/* ESCR6のドライバ停止設定値 */
	STOP_ESCR7								/* ESCR7のドライバ停止設定値 */
};

/* 拡張通信コントロールレジスタドライバ初期化テーブル */
static UI_8 const C_ExtendCommunicationRegInitTbl[UARTDRV_CH_NUMBER] =
{
	INIT_ECCR2,								/* ECCR2のドライバ初期設定値 */
	INIT_ECCR3,								/* ECCR3のドライバ初期設定値 */
	INIT_ECCR4,								/* ECCR4のドライバ初期設定値 */
	INIT_ECCR5,								/* ECCR5のドライバ初期設定値 */
	INIT_ECCR6,								/* ECCR6のドライバ初期設定値 */
	INIT_ECCR7								/* ECCR7のドライバ初期設定値 */
};

/* 拡張通信コントロールレジスタドライバ起動設定テーブル */
static UI_8 const C_ExtendCommunicationRegDrvAct[UARTDRV_CH_NUMBER] =
{
	DRV_ACT_ECCR2,							/* ECCR2のドライバ起動設定値 */
	DRV_ACT_ECCR3,							/* ECCR3のドライバ起動設定値 */
	DRV_ACT_ECCR4,							/* ECCR4のドライバ起動設定値 */
	DRV_ACT_ECCR5,							/* ECCR5のドライバ起動設定値 */
	DRV_ACT_ECCR6,							/* ECCR6のドライバ起動設定値 */
	DRV_ACT_ECCR7							/* ECCR7のドライバ起動設定値 */
};

/* 拡張通信コントロールレジスタドライバ停止設定テーブル */
static UI_8 const C_ExtendCommunicationRegDrvStop[UARTDRV_CH_NUMBER] =
{
	DRV_STOP_ECCR2,							/* ECCR2のドライバ停止設定値 */
	DRV_STOP_ECCR3,							/* ECCR3のドライア停止設定値 */
	DRV_STOP_ECCR4,							/* ECCR4のドライア停止設定値 */
	DRV_STOP_ECCR5,							/* ECCR5のドライア停止設定値 */
	DRV_STOP_ECCR6,							/* ECCR6のドライア停止設定値 */
	DRV_STOP_ECCR7							/* ECCR7のドライア停止設定値 */
};

/* 拡張通信コントロールレジスタドライバ停止テーブル */
static UI_8 const C_ExtendCommunicationRegStopTbl[UARTDRV_CH_NUMBER] =
{
	STOP_ECCR2,								/* ECCR2のドライバ停止設定値 */
	STOP_ECCR3,								/* ECCR3のドライア停止設定値 */
	STOP_ECCR4,								/* ECCR4のドライア停止設定値 */
	STOP_ECCR5,								/* ECCR5のドライア停止設定値 */
	STOP_ECCR6,								/* ECCR6のドライア停止設定値 */
	STOP_ECCR7								/* ECCR7のドライア停止設定値 */
};

/* ボーレートジェネレータレジスタドライバ初期化テーブル */
static UI_16 const C_BaudRateRegInitTbl[UARTDRV_CH_NUMBER] =
{
	INIT_BGR2,								/* BGR2のドライバ初期設定値 */
	INIT_BGR3,								/* BGR3のドライバ初期設定値 */
	INIT_BGR4,								/* BGR4のドライバ初期設定値 */
	INIT_BGR5,								/* BGR5のドライバ初期設定値 */
	INIT_BGR6,								/* BGR6のドライバ初期設定値 */
	INIT_BGR7								/* BGR7のドライバ初期設定値 */
};

/* ボーレートジェネレータレジスタドライバ起動設定テーブル */
static UI_16 const C_BaudRateRegDrvActTbl[UARTDRV_CH_NUMBER] =
{
	DRV_ACT_BGR2,							/* BGR2のドライバ起動設定値 */
	DRV_ACT_BGR3,							/* BGR3のドライバ起動設定値 */
	DRV_ACT_BGR4,							/* BGR4のドライバ起動設定値 */
	DRV_ACT_BGR5,							/* BGR5のドライバ起動設定値 */
	DRV_ACT_BGR6,							/* BGR6のドライバ起動設定値 */
	DRV_ACT_BGR7							/* BGR7のドライバ起動設定値 */
};

/* ボーレートジェネレータレジスタドライバ停止設定テーブル */
static UI_16 const C_BaudRateRegDrvStopTbl[UARTDRV_CH_NUMBER] =
{
	DRV_STOP_BGR2,							/* BGR2のドライバ停止設定値 */
	DRV_STOP_BGR3,							/* BGR3のドライバ停止設定値 */
	DRV_STOP_BGR4,							/* BGR4のドライバ停止設定値 */
	DRV_STOP_BGR5,							/* BGR5のドライバ停止設定値 */
	DRV_STOP_BGR6,							/* BGR6のドライバ停止設定値 */
	DRV_STOP_BGR7							/* BGR7のドライバ停止設定値 */
};

/* ボーレートジェネレータレジスタドライバ停止テーブル */
static UI_16 const C_BaudRateRegStopTbl[UARTDRV_CH_NUMBER] =
{
	STOP_BGR2,								/* BGR2のドライバ停止設定値 */
	STOP_BGR3,								/* BGR3のドライバ停止設定値 */
	STOP_BGR4,								/* BGR4のドライバ停止設定値 */
	STOP_BGR5,								/* BGR5のドライバ停止設定値 */
	STOP_BGR6,								/* BGR6のドライバ停止設定値 */
	STOP_BGR7								/* BGR7のドライバ停止設定値 */
};

typedef struct {
	B_8						is_valid;
	T_UartDrv_Protocol		protocol;
}T_UartDrv_Protocol_config;			/* 通信設定情報 */

static T_UartDrv_Protocol_config	S_uart_drv_protocol[UARTDRV_CH_NUMBER];

/****************************************************************************************/
/*	[モジュール名]	uart_drv_init		[名称]	UARTドライバ関連レジスタ初期化処理(内部)*/
/*======================================================================================*/
/*	[処理概要]	レジスタ初期設定時に本関数を呼び、初期化を行う。						*/
/*======================================================================================*/
/*	[記述形式]	SI_8 uart_drv_init( E_UARTDRV_CH ch)									*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH			ch				: ﾁｬﾝﾈﾙ情報						*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8	tmp_rc		: 正常(D_OK)、異常(D_NG)を返す						*/
/*======================================================================================*/
/*	[ 備  考 ]	送受信とその割込み許可ビットを禁止に設定した状態で						*/
/*				プログラマブルクリア（ソフトウェアリセット）を行う						*/
/****************************************************************************************/
static SI_8 uart_drv_init( E_UARTDRV_CH ch )
{
	SI_8	tmp_rc;		/* 戻り値 */
	UI_8	smr_work;	/* シリアルモードレジスタ演算用work変数 */
	UI_8	uart_mode;	/* UARTドライバの動作モード */
	volatile UI_8	dummy_rx_buf;

	if ( ch < UARTDRV_CH_NUMBER ) {
		/* UARTドライバレジスタ初期設定処理 */
		dummy_rx_buf				= *C_ReceiveDataReg[ch];		/* 受信データの読み出しによる割込み要求フラグクリア */

		/* モード設定（要UPCLリセット） */
		uart_mode					= (C_SerialModeRegInitTbl[ch] & SMR_MD_MASK);	/* 動作モードの設定値を抽出 */
		smr_work					= *C_SerialModeReg[ch];
		smr_work					|= uart_mode;
		*C_SerialModeReg[ch]		= smr_work;										/* 動作モード設定 */

		/* 同期モード：スタート/ストップビットの付加設定（要UPCLリセット） */
		*C_ExtendCommunicationReg[ch]	= C_ExtendCommunicationRegInitTbl[ch];	/* 拡張通信コントロールレジスタ設定 */

		if ( (uart_mode & SMR_MD_MASK) == UARTDRV_MODE_CLOCK ) {				/* UPCLリセット：同期モードの場合 */
			smr_work					= *C_SerialModeReg[ch];
			smr_work					|= SMR_UPCL_SET;
			*C_SerialModeReg[ch]		= smr_work;				/* モード・スタート/ストップビットの付加変更によるUPCLリセット */
		}

		*C_ExtendCtrlReg[ch]		= C_ExtendCtrlRegInitTbl[ch];	/* 拡張ステータス制御レジスタ設定（クロック反転時、UPCLリセットNG） */
		*C_SerialCtrlReg[ch]		= C_SerialCtrlRegInitTbl[ch];	/* シリアルコントロールレジスタ：データフォーマット設定（要UPCLリセット） */
		*C_SerialModeReg[ch]		= C_SerialModeRegInitTbl[ch];	/* シリアルモードレジスタ：モード設定 */
		*C_SerialStatReg[ch]		= C_SerialStatRegInitTbl[ch];	/* シリアルステータスレジスタ設定 */
		*C_BaudRateReg[ch]			= C_BaudRateRegInitTbl[ch];		/* ボーレートジェネレータレジスタ設定 */

		if ( (uart_mode & SMR_MD_MASK) == UARTDRV_MODE_UART ) {					/* UPCLリセット：非同期モードの場合 */
			/* ソフトリセット: 送受信の停止と割込み要因のクリア */
			smr_work					= *C_SerialModeReg[ch];
			smr_work					|= SMR_UPCL_SET;
			*C_SerialModeReg[ch]		= smr_work;				/* モード変更、データフォーマット変更によるUPCLリセット */
		}

		S_uart_drv_protocol[ch].is_valid = D_FALSE;

		tmp_rc = D_OK;
	} else {
		tmp_rc = D_NG;
	}
	return tmp_rc;
}

/****************************************************************************************/
/*	[モジュール名]	uart_drv_stop		[名称]	UARTドライバ関連レジスタ停止処理(内部)	*/
/*======================================================================================*/
/*	[処理概要]	レジスタ初期設定時に本関数を呼び、停止を行う。							*/
/*======================================================================================*/
/*	[記述形式]	SI_8 uart_drv_stop( E_UARTDRV_CH ch)									*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH			ch				: ﾁｬﾝﾈﾙ情報						*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8	tmp_rc		: 正常(D_OK)、異常(D_NG)を返す						*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static SI_8 uart_drv_stop( E_UARTDRV_CH ch )
{
	SI_8	tmp_rc;		/* Ch.範囲外チェック */
	volatile UI_8	dummy_rx_buf;

	if ( ch < UARTDRV_CH_NUMBER ) {
		/* UARTドライバレジスタ停止設定処理 */
		dummy_rx_buf				= *C_ReceiveDataReg[ch];	/* 受信データの読み出しによる割込み要求フラグクリア */
		*C_ExtendCtrlReg[ch]		= C_ExtendCtrlRegStopTbl[ch];	/* 拡張ステータス制御レジスタ設定 */
		*C_SerialCtrlReg[ch]		= C_SerialCtrlRegStopTbl[ch];	/* シリアルコントロールレジスタ：データフォーマット設定 */
		*C_SerialModeReg[ch]		= C_SerialModeRegStopTbl[ch];	/* シリアルモードレジスタ：モード設定 */
		*C_SerialStatReg[ch]		= C_SerialStatRegStopTbl[ch];	/* シリアルステータスレジスタ設定 */
		*C_ExtendCommunicationReg[ch]	= C_ExtendCommunicationRegStopTbl[ch];/* 拡張通信コントロールレジスタ設定 */
		*C_BaudRateReg[ch]			= C_BaudRateRegStopTbl[ch];		/* ボーレートジェネレータレジスタ設定 */
		tmp_rc = D_OK;
	} else {
		tmp_rc = D_NG;
	}
	return tmp_rc;
}

#if (UART_DRV_TX == 1U) /* 送信処理 */
/****************************************************************************************/
/*	[モジュール名]	uart_drv_txdrive		[名称]	UARTﾄﾞﾗｲﾊﾞ送信起動処理				*/
/*======================================================================================*/
/*	[処理概要]	LIN-UARTﾚｼﾞｽﾀをﾄﾞﾗｲﾊﾞ送信起動設定にする									*/
/*======================================================================================*/
/*	[記述形式]	static void uart_drv_txdrive( E_UARTDRV_CH ch )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH				ch					: ﾁｬﾝﾈﾙ情報				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	シリアルコントロールレジスタ、シリアルステータスレジスタを				*/
/*				ドライバ送信起動に設定する前に、送信データレジスタにダミーライトを行い	*/
/*				プログラマブルクリア（ソフトウェアリセット）を行っています。			*/
/****************************************************************************************/
static void uart_drv_txdrive( E_UARTDRV_CH ch )
{
	UI_8 smr_work; /* シリアルモードレジスタ演算用work変数 */
	UI_8 uart_mode; /* UARTドライバの動作モード */
	UI_16 bgr_act_value;
	UI_8 scr_act_value;
	UI_8 ssr_act_value;

	*C_ExtendCtrlReg[ch]		= C_ExtendCtrlRegDrvActTbl[ch];			/* 拡張ステータス制御レジスタアドレステーブル */
	*C_ExtendCommunicationReg[ch]	= C_ExtendCommunicationRegDrvAct[ch];	/* 拡張通信コントロールレジスタアドレステーブル */
	bgr_act_value				= uart_drv_get_bgr_act_value(ch);
	*C_BaudRateReg[ch]			= bgr_act_value;						/* ボーレートジェネレータレジスタアドレステーブル */
	*C_SerialModeReg[ch]		= C_SerialModeRegDrvActTbl[ch];			/* シリアルモードレジスタアドレステーブル */
	*C_TransDataReg[ch]			= DUMMY_WRITE;							/* 送信データレジスタアドレステーブル ← UART送信用ダミーライトデータ */

	uart_mode					= (C_SerialModeRegDrvActTbl[ch] & SMR_MD_MASK);	/* 動作モードの設定値を抽出 */
	if ( (uart_mode & SMR_MD_MASK) == UARTDRV_MODE_UART  ) {			/* 非同期モードの場合 */
		smr_work					= *C_SerialModeReg[ch];
		smr_work					|= SMR_UPCL_SET;
		*C_SerialModeReg[ch]		= smr_work;							/* シリアルモードレジスタアドレステーブル | プログラマブルクリアビットセットデータ */
	}

	scr_act_value				= uart_drv_get_scr_act_value(ch);
	ssr_act_value				= uart_drv_get_ssr_act_value(ch);
	*C_SerialCtrlReg[ch]		= scr_act_value;						/* シリアルコントロールレジスタアドレステーブル */
	*C_SerialStatReg[ch]		= ssr_act_value;						/* シリアルステータスレジスタアドレステーブル */
}
#endif /* UART_DRV_TX */

#if (UART_DRV_TX == 1U) /* 送信処理 */
/****************************************************************************************/
/*	[モジュール名]	uart_drv_txstop			[名称]	UARTﾄﾞﾗｲﾊﾞ送信停止処理				*/
/*======================================================================================*/
/*	[処理概要]	LIN-UARTレジスタをドライバ送信停止設定にする。							*/
/*======================================================================================*/
/*	[記述形式]	static void uart_drv_txstop( E_UARTDRV_CH ch )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH				ch					: ﾁｬﾝﾈﾙ情報				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static void uart_drv_txstop( E_UARTDRV_CH ch )
{
	*C_ExtendCtrlReg[ch]		= C_ExtendCtrlRegDrvStopTbl[ch];		/* 拡張ステータス制御レジスタアドレステーブル */
	*C_SerialStatReg[ch]		= C_SerialStatRegDrvStopTbl[ch];		/* シリアルステータスレジスタアドレステーブル */
	*C_SerialCtrlReg[ch]		= C_SerialCtrlRegDrvStopTbl[ch];		/* シリアルコントロールレジスタアドレステーブル */
	*C_SerialModeReg[ch]		= C_SerialModeRegDrvStopTbl[ch];		/* シリアルモードレジスタアドレステーブル */
	*C_BaudRateReg[ch]			= C_BaudRateRegDrvStopTbl[ch];			/* ボーレートジェネレータレジスタアドレステーブル */
	*C_ExtendCommunicationReg[ch]	= C_ExtendCommunicationRegDrvStop[ch];	/* 拡張通信コントロールレジスタアドレステーブル */
}
#endif /* UART_DRV_TX */

#if (UART_DRV_RX == 1U) /* 受信処理 */
/****************************************************************************************/
/*	[モジュール名]	uart_drv_rxdrive		[名称]	UARTﾄﾞﾗｲﾊﾞ受信起動処理				*/
/*======================================================================================*/
/*	[処理概要]	LIN-UARTレジスタをドライバ受信起動設定にする。							*/
/*======================================================================================*/
/*	[記述形式]	static void uart_drv_rxdrive( E_UARTDRV_CH ch )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH				ch					: ﾁｬﾝﾈﾙ情報				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static void uart_drv_rxdrive( E_UARTDRV_CH ch )
{
	volatile UI_8 dummy_rx_buf;
	UI_8 smr_work; /* シリアルモードレジスタ演算用work変数 */
	UI_16 bgr_act_value;
	UI_8 scr_act_value;
	UI_8 ssr_act_value;
	
	dummy_rx_buf				= *C_ReceiveDataReg[ch];				/* UART受信用ダミーバッファ ← 受信データレジスタアドレステーブル */
	*C_ExtendCtrlReg[ch]		= C_ExtendCtrlRegDrvActTbl[ch];			/* 拡張ステータス制御レジスタアドレステーブル */
	*C_ExtendCommunicationReg[ch]	= C_ExtendCommunicationRegDrvAct[ch];	/* 拡張通信コントロールレジスタアドレステーブル */
	bgr_act_value				= uart_drv_get_bgr_act_value(ch);
	*C_BaudRateReg[ch]			= bgr_act_value;						/* ボーレートジェネレータレジスタアドレステーブル */
	smr_work					= *C_SerialModeReg[ch];
	smr_work					|= SMR_UPCL_SET;
	*C_SerialModeReg[ch]		= smr_work;								/* シリアルモードレジスタアドレステーブル | プログラマブルクリアビットセットデータ */
	scr_act_value				= uart_drv_get_scr_act_value(ch);
	ssr_act_value				= uart_drv_get_ssr_act_value(ch);
	*C_SerialCtrlReg[ch]		= scr_act_value;						/* シリアルコントロールレジスタアドレステーブル */
	*C_SerialStatReg[ch]		= ssr_act_value;						/* シリアルステータスレジスタアドレステーブル */
}
#endif /* UART_DRV_RX */

#if (UART_DRV_RX == 1U) /* 受信処理 */
/****************************************************************************************/
/*	[モジュール名]	uart_drv_rxstop			[名称]	UARTﾄﾞﾗｲﾊﾞ受信停止処理				*/
/*======================================================================================*/
/*	[処理概要]	LIN-UARTレジスタをドライバ受信停止設定にする。							*/
/*======================================================================================*/
/*	[記述形式]	static void uart_drv_rxstop( E_UARTDRV_CH ch )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH				ch					: ﾁｬﾝﾈﾙ情報				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static void uart_drv_rxstop( E_UARTDRV_CH ch )
{
	volatile UI_8 dummy_rx_buf;
	
	*C_ExtendCtrlReg[ch]		= C_ExtendCtrlRegDrvStopTbl[ch];		/* 拡張ステータス制御レジスタアドレステーブル */
	*C_SerialStatReg[ch]		= C_SerialStatRegDrvStopTbl[ch];		/* シリアルステータスレジスタアドレステーブル */
	*C_SerialCtrlReg[ch]		= C_SerialCtrlRegDrvStopTbl[ch];		/* シリアルコントロールレジスタアドレステーブル */
	*C_SerialModeReg[ch]		= C_SerialModeRegDrvStopTbl[ch];		/* シリアルモードレジスタアドレステーブル */
	*C_BaudRateReg[ch]			= C_BaudRateRegDrvStopTbl[ch];			/* ボーレートジェネレータレジスタアドレステーブル */
	*C_ExtendCommunicationReg[ch]	= C_ExtendCommunicationRegDrvStop[ch];	/* 拡張通信コントロールレジスタアドレステーブル */
	dummy_rx_buf				= *C_ReceiveDataReg[ch];				/* UART受信用ダミーバッファ ← 受信データレジスタアドレステーブル */
}
#endif /* UART_DRV_RX */

/****************************************************************************************/
/*	[モジュール名]	uart_drv_get_scr_act_value	[名称]	シリアルコントロールレジスタ起動値取得	*/
/*======================================================================================*/
/*	[処理概要]	シリアルコントロールレジスタ起動値を取得する。							*/
/*======================================================================================*/
/*	[記述形式]	static UI_8 uart_drv_get_scr_act_value( E_UARTDRV_CH ch )				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH				ch					: ﾁｬﾝﾈﾙ情報				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	UI_8	シリアルコントロールレジスタ起動値								*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static UI_8 uart_drv_get_scr_act_value( E_UARTDRV_CH ch )
{

	UI_8 scr_act_value;

	/* シリアルコントロールレジスタドライバ起動設定テーブルから取得	*/
	scr_act_value = C_SerialCtrlRegDrvActTbl[ch];

	if (S_uart_drv_protocol[ch].is_valid == D_TRUE) {
		/* 通信方式が設定されている場合 設定されている値で更新する	*/

		/* パリティ設定	*/
		if (S_uart_drv_protocol[ch].protocol.parity == UARTDRV_PARITY_EVEN) {
			/* 偶数パリティ	*/
			scr_act_value |= SCR_PEN_SET;
			scr_act_value &= SCR_P_CLR;
		}
		else if (S_uart_drv_protocol[ch].protocol.parity == UARTDRV_PARITY_ODD) {
			/* 奇数パリティ	*/
			scr_act_value |= SCR_PEN_SET;
			scr_act_value |= SCR_P_SET;
		}
		else if (S_uart_drv_protocol[ch].protocol.parity == UARTDRV_PARITY_NONE) {
			/* パリティなし	*/
			scr_act_value &= SCR_PEN_CLR;
		}
		else {
			/* nop */
		}

		/* ストップビット長設定	*/
		if (S_uart_drv_protocol[ch].protocol.stop_bit_len == UARTDRV_STOP_BIT_LEN_1) {
			/* 1ビット	*/
			scr_act_value &= SCR_SBL_CLR;
		}
		else if (S_uart_drv_protocol[ch].protocol.stop_bit_len == UARTDRV_STOP_BIT_LEN_2) {
			/* 2ビット	*/
			scr_act_value |= SCR_SBL_SET;
		}
		else {
			/* nop */
		}

		/* データ長設定	*/
		if (S_uart_drv_protocol[ch].protocol.bit_len == UARTDRV_BIT_LEN_7) {
			/* 7ビット	*/
			scr_act_value &= SCR_CL_CLR;
		}
		else if (S_uart_drv_protocol[ch].protocol.bit_len == UARTDRV_BIT_LEN_8) {
			/* 8ビット	*/
			scr_act_value |= SCR_CL_SET;
		}
		else {
			/* nop */
		}
	}

	return scr_act_value;

}

/****************************************************************************************/
/*	[モジュール名]	uart_drv_get_ssr_act_value	[名称]	シリアルステータスレジスタ起動値取得	*/
/*======================================================================================*/
/*	[処理概要]	シリアルステータスレジスタ起動値を取得する。							*/
/*======================================================================================*/
/*	[記述形式]	static UI_8 uart_drv_get_ssr_act_value( E_UARTDRV_CH ch )				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH				ch					: ﾁｬﾝﾈﾙ情報				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	UI_8	シリアルステータスレジスタ起動値								*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static UI_8 uart_drv_get_ssr_act_value( E_UARTDRV_CH ch )
{

	UI_8 ssr_act_value;

	/* シリアルステータスレジスタドライバ起動設定テーブルから取得	*/
	ssr_act_value = C_SerialStatRegDrvActTbl[ch];

	if (S_uart_drv_protocol[ch].is_valid == D_TRUE) {
		/* 通信方式が設定されている場合 設定されている値で更新する	*/

		/* 転送方向設定	*/
		if (S_uart_drv_protocol[ch].protocol.direction == UARTDRV_DIRECTION_LSB_FIRST) {
			/* LSBファースト	*/
			ssr_act_value &= SSR_BDS_CLR;
		}
		else if (S_uart_drv_protocol[ch].protocol.direction == UARTDRV_DIRECTION_MSB_FIRST) {
			/* MSBファースト	*/
			ssr_act_value |= SSR_BDS_SET;
		}
		else {
			/* nop */
		}
	}

	return ssr_act_value;

}

/****************************************************************************************/
/*	[モジュール名]	uart_drv_get_bgr_act_value	[名称]	ボーレートジェネレータレジスタ起動値取得	*/
/*======================================================================================*/
/*	[処理概要]	ボーレートジェネレータレジスタ起動値を取得する。						*/
/*======================================================================================*/
/*	[記述形式]	static UI_8 uart_drv_get_bgr_act_value( E_UARTDRV_CH ch )				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH				ch					: ﾁｬﾝﾈﾙ情報				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	UI_16	ボーレートジェネレータレジスタ起動値							*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static UI_16 uart_drv_get_bgr_act_value( E_UARTDRV_CH ch )
{

	UI_16 bgr_act_value;

	/* ボーレートジェネレータレジスタドライバ起動設定テーブルから取得	*/
	bgr_act_value = C_BaudRateRegDrvActTbl[ch];

	if (S_uart_drv_protocol[ch].is_valid == D_TRUE) {
		/* 通信方式が設定されている場合 設定されている値で更新する	*/
		/* レジスタ設定値＝(内部クロック16MHz/ボーレート）- 1	*/
		switch (S_uart_drv_protocol[ch].protocol.baud_rate) {
		case UARTDRV_BAUD_RATE_9600:
			/* 9600bps		*/
			bgr_act_value = 1666;
			break;

		case UARTDRV_BAUD_RATE_14400:
			/* 14400bps		*/
			bgr_act_value = 1110;
			break;

		case UARTDRV_BAUD_RATE_19200:
			/* 19200bps		*/
			bgr_act_value = 832;
			break;

		case UARTDRV_BAUD_RATE_38400:
			/* 38400bps		*/
			bgr_act_value = 416;
			break;

		case UARTDRV_BAUD_RATE_57600:
			/* 57600bps		*/
			bgr_act_value = 277;
			break;

		case UARTDRV_BAUD_RATE_115200:
			/* 115200bps	*/
			bgr_act_value = 138;
			break;

		default:
			break;
		}
	}

	return bgr_act_value;

}

/****************************************************************************************/
/*	[モジュール名]	UartDrv_InitRequest	[名称]	UARTドライバ関連レジスタ初期化要求処理	*/
/*======================================================================================*/
/*	[処理概要]	初期化要求時に本関数を呼び、レジスタ初期化関数の呼び出しを行う。		*/
/*======================================================================================*/
/*	[記述形式]	void UartDrv_InitRequest( E_UARTDRV_CH ch )								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH			ch				: ﾁｬﾝﾈﾙ情報						*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void UartDrv_InitRequest( E_UARTDRV_CH ch )
{
	/* UARTドライバレジスタ初期設定処理 */
	(void)uart_drv_init( ch );
}

#if (UART_DRV_TX == 1U) /* 送信処理 */
/****************************************************************************************/
/*	[モジュール名]	UartDrv_ReqTxCtrl		[名称]	ｼﾘｱﾙ通信ﾄﾞﾗｲﾊﾞ送信制御処理			*/
/*======================================================================================*/
/*	[処理概要]	シリアル通信ドライバの送信制御を行う。									*/
/*				UART実行状態が起動なら、UARTドライバ送信起動処理を呼び出す。			*/
/*				UART実行状態が停止なら、UARTドライバ送信停止処理を呼び出す。			*/
/*				戻り値に、指定チャネルの実行処理ステータスを返す。						*/
/*======================================================================================*/
/*	[記述形式]	SI_8 UartDrv_ReqTxCtrl( E_UARTDRV_CH ch, UI_8 act )						*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH		ch				: ﾁｬﾝﾈﾙ情報							*/
/*				UI_8				act				: UART実行状態						*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8				stat			: 指定ﾁｬﾈﾙの実行処理ｽﾃｰﾀｽ			*/
/*======================================================================================*/
/*	[ 備  考 ]	同じチャネルに送信と受信を同時に割り当てられないため、					*/
/*				シリアル通信ドライバ受信制御処理と同時に呼び出さないこと。				*/
/****************************************************************************************/
SI_8 UartDrv_ReqTxCtrl( E_UARTDRV_CH ch, UI_8 act )
{
	SI_8 stat;
	
	stat = D_OK;
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		if ( act == D_ENABLE ) {					/* 指定CHのUARTの実行状態 */
			/* UART実行状態が起動ならば */
			uart_drv_txdrive(ch);					/* UARTドライバ送信起動処理 */
		} else {
			/* UART実行状態が停止ならば */
			uart_drv_txstop(ch);					/* UARTドライバ送信停止処理 */
		}
	} else {
		stat = D_NG;
	}
	
	return stat;
}
#endif /* UART_DRV_TX */

#if (UART_DRV_RX == 1U) /* 受信処理 */
/****************************************************************************************/
/*	[モジュール名]	UartDrv_ReqRxCtrl	[名称]	ｼﾘｱﾙ通信ﾄﾞﾗｲﾊﾞ受信制御処理				*/
/*======================================================================================*/
/*	[処理概要]	シリアル通信ドライバの受信制御を行う。									*/
/*				UART実行状態が起動なら、UARTドライバ受信起動処理を呼び出す。			*/
/*				UART実行状態が停止なら、UARTドライバ受信停止処理を呼び出す。			*/
/*				戻り値に、指定チャネルの実行処理ステータスを返す。						*/
/*======================================================================================*/
/*	[記述形式]	SI_8 UartDrv_ReqRxCtrl( E_UARTDRV_CH ch, UI_8 act )						*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH			ch			: ﾁｬﾝﾈﾙ情報							*/
/*				UI_8					act			: UART実行状態						*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8					stat		: 指定ﾁｬﾈﾙの実行処理ｽﾃｰﾀｽ			*/
/*======================================================================================*/
/*	[ 備  考 ]	同じチャネルに送信と受信を同時に割り当てられないため、					*/
/*				シリアル通信ドライバ送信制御処理と同時に呼び出さないこと。				*/
/****************************************************************************************/
SI_8 UartDrv_ReqRxCtrl( E_UARTDRV_CH ch, UI_8 act )
{
	SI_8 stat;
	
	stat = D_OK;
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		if ( act == D_ENABLE ) {				/* 指定CHのUARTの実行状態 */
			/* UART実行状態が起動ならば */
			uart_drv_rxdrive(ch);				/* UARTドライバ受信起動処理 */
		} else {
			/* UART実行状態が停止ならば */
			uart_drv_rxstop(ch);				/* UARTドライバ受信停止処理 */
		}
	} else {
		stat = D_NG;
	}
	
	return stat;
}
#endif /* UART_DRV_RX */

#if (UART_DRV_TX == 1U) /* 送信処理 */
/****************************************************************************************/
/*	[モジュール名]	UartDrv_SetTxData	[名称]	ｼﾘｱﾙ通信送信ﾃﾞｰﾀ設定処理				*/
/*======================================================================================*/
/*	[処理概要]	送信データの設定を行う。												*/
/*				送信データレジスタに送信データを設定します。							*/
/*				戻り値に、指定チャネルの実行処理ステータスを返します。					*/
/*======================================================================================*/
/*	[記述形式]	SI_8 UartDrv_SetTxData( E_UARTDRV_CH ch, UI_8 tx_data)					*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH			ch			: ﾁｬﾝﾈﾙ情報							*/
/*				UI_8					tx_data		: 送信ﾃﾞｰﾀ							*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8					stat		: 指定ﾁｬﾈﾙの実行処理ｽﾃｰﾀｽ			*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
SI_8 UartDrv_SetTxData( E_UARTDRV_CH ch, UI_8 tx_data )
{
	SI_8 stat;
	
	stat = D_OK;
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		*C_TransDataReg[ch] = tx_data;			/* 送信データレジスタ ← 送信データ */
	} else {
		stat = D_NG;
	}
		
	return stat;
}
#endif /* UART_DRV_TX */

/****************************************************************************************/
/*	[モジュール名]	UartDrv_GetTransDataAddress	[名称]	UART送信データアドレス取得		*/
/*======================================================================================*/
/*	[処理概要]	UARTの送信データアドレスを取得する。									*/
/*======================================================================================*/
/*	[記述形式]	T_UartTransDataAddress UartDrv_GetTransDataAddress( E_UARTDRV_CH ch )	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH			ch				: ﾁｬﾝﾈﾙ情報						*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	T_UartTransDataAddress	tx_data_addr		: 送信データアドレスを返す	*/
/*======================================================================================*/
/*	[ 備  考 ]	関数の呼び出し元で必ずchの範囲外チェックをする事						*/
/*				送信/受信のデータレジスタアドレスが同じ為、いずれの場合にも使用可能。	*/
/****************************************************************************************/
T_UartTransDataAddress UartDrv_GetTransDataAddress( E_UARTDRV_CH ch )
{
	T_UartTransDataAddress	tx_data_addr;

	tx_data_addr = (T_UartTransDataAddress)C_TransDataReg[ch];

	return tx_data_addr;
}

/****************************************************************************************/
/*	[モジュール名]	UartDrv_ChangeProtocol	[名称]	UARTドライバ通信方式変更処理		*/
/*======================================================================================*/
/*	[処理概要]	通信方式の動的切替を行う。												*/
/*======================================================================================*/
/*	[記述形式]	SI_8 UartDrv_ChangeProtocol(E_UARTDRV_CH ch,							*/
/*													const T_UartDrv_Protocol* protocol)	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH			ch				: ﾁｬﾝﾈﾙ情報						*/
/*				const T_UartDrv_Protocol	*protocol	: 通信設定情報					*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8									: 指定ﾁｬﾈﾙの実行処理ｽﾃｰﾀｽ		*/
/*======================================================================================*/
/*	[ 備  考 ]	変更した通信設定は、UartDrv_ReqTxCtrlまたはUartDrv_ReqRxCtrlにより		*/
/*				起動したタイミングで有効になる											*/
/****************************************************************************************/
SI_8 UartDrv_ChangeProtocol(E_UARTDRV_CH ch, const T_UartDrv_Protocol* protocol)
{
	SI_8 stat;
	
	stat = D_OK;
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		S_uart_drv_protocol[ch].protocol = *protocol;
		S_uart_drv_protocol[ch].is_valid = D_TRUE;
	} else {
		stat = D_NG;
	}
	
	return stat;

}

#if (UART_DRV_RX == 1U) /* 受信処理 */
/****************************************************************************************/
/*	[モジュール名]	UartDrv_GetRxData	[名称]	ｼﾘｱﾙ通信受信ﾃﾞｰﾀ取得処理				*/
/*======================================================================================*/
/*	[処理概要]	受信データの取得を行う。												*/
/*				受信データコピー先アドレスに指定チャネルの受信データレジスタのデータを	*/
/*				取得します。															*/
/*				戻り値に、指定チャネルの実行処理ステータスを返します。					*/
/*======================================================================================*/
/*	[記述形式]	SI_8 UartDrv_GetRxData( E_UARTDRV_CH ch, UI_8 *rx_data )				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH			ch					: ﾁｬﾝﾈﾙ情報					*/
/*				UI_8 *					rx_data				: 受信ﾃﾞｰﾀｺﾋﾟｰ先ｱﾄﾞﾚｽ		*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8					stat				: 指定ﾁｬﾈﾙの実行処理ｽﾃｰﾀｽ	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
SI_8 UartDrv_GetRxData( E_UARTDRV_CH ch, UI_8 *rx_data )
{
	SI_8 stat;
	
	stat = D_OK;
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		*rx_data = *C_ReceiveDataReg[ch];		/* 受信データコピー先アドレス ← 受信データレジスタ */
	} else {
		stat = D_NG;
	}
	
	return stat;
}
#endif /* UART_DRV_RX */

#if (UART_DRV_TX == 1U) /* 送信処理 */
/****************************************************************************************/
/*	[モジュール名]	UartDrv_TxIntrReqEnblCtrl[名称]	送信割込み要求許可情報ｾｯﾄ処理		*/
/*======================================================================================*/
/*	[処理概要]	シリアル通信の送信割込み要求許可情報をセット/クリアする。				*/
/*======================================================================================*/
/*	[記述形式]	void UartDrv_TxIntrReqEnblCtrl( E_UARTDRV_CH ch, UI_8 act )				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH			ch			: ﾁｬﾝﾈﾙ情報							*/
/*				UI_8					act			: 動作種別(D_ON/D_OFF)				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void UartDrv_TxIntrReqEnblCtrl( E_UARTDRV_CH ch, UI_8 act )
{
	UI_8 ssr_work; /* シリアルステータスレジスタ演算用work変数 */

	if ( ch < UARTDRV_CH_NUMBER ) {
		ssr_work = *C_SerialStatReg[ch];
		if ( act == D_ON ) {	/* セット時 */
			ssr_work |= SSR_TIE_SET;	/* 送信割込み要求許可情報をセット */
		} else {
			ssr_work &= SSR_TIE_CLR;	/* 送信割込み要求許可情報をクリア */
		}
		*C_SerialStatReg[ch] = ssr_work;
	}
}
#endif /* UART_DRV_TX */

#if (UART_DRV_RX == 1U) /* 受信処理 */
/****************************************************************************************/
/*	[モジュール名]	UartDrv_RxIntrReqEnblCtrl[名称]	受信割込み要求許可情報ｾｯﾄ処理		*/
/*======================================================================================*/
/*	[処理概要]	シリアル通信の受信割込み要求許可情報をセット/クリアする					*/
/*======================================================================================*/
/*	[記述形式]	void UartDrv_RxIntrReqEnblCtrl( E_UARTDRV_CH ch, UI_8 act )				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH			ch			: ﾁｬﾝﾈﾙ情報							*/
/*				UI_8					act			: 動作種別(D_ON/D_OFF)				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void UartDrv_RxIntrReqEnblCtrl( E_UARTDRV_CH ch, UI_8 act )
{
	UI_8 ssr_work; /* シリアルステータスレジスタ演算用work変数 */

	if ( ch < UARTDRV_CH_NUMBER ) {
		ssr_work = *C_SerialStatReg[ch];
		if ( act == D_ON ) {	/* セット時 */
			ssr_work |= SSR_RIE_SET;	/* 受信割込み要求許可情報をセット */
		} else {
			ssr_work &= SSR_RIE_CLR;	/* 受信割込み要求許可情報をクリア */
		}
		*C_SerialStatReg[ch] = ssr_work;
	}
}
#endif /* UART_DRV_RX */

#if (UART_DRV_TX == 1U) /* 送信処理 */
/****************************************************************************************/
/*	[モジュール名]	UartDrv_GetTxDataEmpty	[名称]	送信ﾃﾞｰﾀｴﾝﾌﾟﾃｨ情報取得処理			*/
/*======================================================================================*/
/*	[処理概要]	シリアル通信で、送信データのエンプティ情報を公開する。					*/
/*				LIN-UARTﾁｬﾈﾙの指定がUARTDRV_CH_2～UARTDRV_CH_7以外の場合は、			*/
/*				必ず送信データなしを返します。											*/
/*				指定チャネルの送信データエンプティフラグを判定し、						*/
/*				OFFなら送信データあり、ONなら送信データなしを返します。					*/
/*======================================================================================*/
/*	[記述形式]	UI_8 UartDrv_GetTxDataEmpty(E_UARTDRV_CH ch )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH			ch						: ﾁｬﾝﾈﾙ情報				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	UI_8					empty_info		: 指定ﾁｬﾈﾙの送信ﾃﾞｰﾀｴﾝﾌﾟﾃｨ情報	*/
/*======================================================================================*/
/*	[ 備  考 ]																			*/
/*																						*/
/****************************************************************************************/
UI_8 UartDrv_GetTxDataEmpty( E_UARTDRV_CH ch )
{
	UI_8 empty_info;
	UI_8 ssr_temp; /* シリアルステータスレジスタ格納用変数 */
	
	empty_info = D_OFF;					/* 送信データなし */
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		ssr_temp = *C_SerialStatReg[ch];
		if ( (ssr_temp & SSR_TDRE_MASK) == D_OFF ) {
			empty_info = D_ON;			/* 送信データあり */
		}
	}
	
	return empty_info;
}
#endif /* UART_DRV_TX */

#if (UART_DRV_RX == 1U) /* 受信処理 */
/****************************************************************************************/
/*	[モジュール名]	UartDrv_GetErr		[名称]	受信ｴﾗｰ情報取得処理						*/
/*======================================================================================*/
/*	[処理概要]	受信エラー情報の取得を行う。											*/
/*				LIN-UARTチャネルの指定がUARTDRV_CH_2～UARTDRV_CH_7以外の場合は、		*/
/*				エラーステータスをエラーなしにし、ﾁｬﾈﾙ情報の範囲正常を返します。		*/
/*				指定チャネルのエラーフラグに応じて、エラー情報の該当ビットを1にします。	*/
/*======================================================================================*/
/*	[記述形式]	SI_8 UartDrv_GetErr( E_UARTDRV_CH ch, T_UartDrv_ErrInfo err )			*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH			ch					: ﾁｬﾝﾈﾙ情報					*/
/*				T_UartDrv_ErrInfo *		err					: ｴﾗｰ情報					*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8					stat				: 指定ﾁｬﾈﾙの実行処理ｽﾃｰﾀｽ	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
SI_8 UartDrv_GetErr( E_UARTDRV_CH ch, T_UartDrv_ErrInfo *err )
{
	SI_8 stat;
	UI_8 ssr_work;	/* シリアルステータスレジスタ作業用 */
	
	stat = D_OK;				/* ﾁｬﾈﾙ情報正常 */
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		/* シリアルステータスレジスタの設定値を取得 */
		ssr_work = *C_SerialStatReg[ch];
		
		/* エラー情報の初期化 */
		err->parity_flg = D_OFF;
		err->overrun_flg = D_OFF;
		err->flaming_flg = D_OFF;
		
		/* パリティエラー */
		if ( (ssr_work & SSR_PARITY_MASK) != D_OFF ) {
			err->parity_flg = D_ON;
		}
		/* オーバーランエラー */
		if ( (ssr_work & SSR_OVERRUN_MASK) != D_OFF ) {
			err->overrun_flg = D_ON;
		}
		/* フレーミングエラー */
		if ( (ssr_work & SSR_FLAMING_MASK) != D_OFF ) {
			err->flaming_flg = D_ON;
		}
	} else {
		stat = D_NG;				/* ﾁｬﾈﾙ情報異常 */
	}
	return stat;
}
#endif /* UART_DRV_RX */

#if (UART_DRV_RX == 1U) /* 受信処理 */
/****************************************************************************************/
/*	[モジュール名]	UartDrv_ReqErrClr		[名称]	受信ｴﾗｰ情報クリア処理				*/
/*======================================================================================*/
/*	[処理概要]	受信エラー情報クリア処理を行う。										*/
/*======================================================================================*/
/*	[記述形式]	void UartDrv_ReqErrClr( E_UARTDRV_CH ch )								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH			ch						: ﾁｬﾝﾈﾙ情報				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void UartDrv_ReqErrClr( E_UARTDRV_CH ch )
{
	UI_8 serial_ctrl_reg;
	
	if ( ch < UARTDRV_CH_NUMBER ) {							/* 指定chが範囲内 */
		serial_ctrl_reg = *C_SerialCtrlReg[ch];				/* シリアルコントロールレジスタを読出し */
		serial_ctrl_reg |= SCR_RX_ERR_CLR_BIT_ON;			/* 受信エラーフラグクリアbitをONにする */
		*C_SerialCtrlReg[ch] = serial_ctrl_reg;
	}
}
#endif /* UART_DRV_RX */

#if (UART_DRV_RX == 1U) /* 受信処理 */
/****************************************************************************************/
/*	[モジュール名]	UartDrv_GetRxFullFlag	[名称]	受信ﾃﾞｰﾀﾌﾙﾌﾗｸﾞ取得処理				*/
/*======================================================================================*/
/*	[処理概要]	受信データフルフラグの取得を行う。										*/
/*======================================================================================*/
/*	[記述形式]	UI_8 UartDrv_GetRxFullFlag( E_UARTDRV_CH ch )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_UARTDRV_CH			ch						: ﾁｬﾝﾈﾙ情報				*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	UI_8				rx_full_flag			: 指定ﾁｬﾈﾙの受信ﾃﾞｰﾀﾌﾙﾌﾗｸﾞ	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
UI_8 UartDrv_GetRxFullFlag( E_UARTDRV_CH ch )
{
	UI_8 rx_full_flag;
	UI_8 ssr_temp; /* シリアルステータスレジスタ格納用変数 */
	
	rx_full_flag = D_OFF;				/* 受信データフルフラグOFF */
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		ssr_temp = *C_SerialStatReg[ch];
		if ( (ssr_temp & SSR_RDRF_MASK) != D_OFF ) {
			rx_full_flag = D_ON;		/* 受信データフルフラグON */
		}
	}
	
	return rx_full_flag;
}
#endif /* UART_DRV_RX */

/****************************************************************************************/
/*	[モジュール名]	UartDrv_Init	[名称]	UARTﾚｼﾞｽﾀ初期設定処理						*/
/*======================================================================================*/
/*	[処理概要]	レジスタ初期設定時に本関数を呼び、初期化を行う。						*/
/*======================================================================================*/
/*	[記述形式]	void UartDrv_Init( E_INIT_TYPE req  )									*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_INIT_TYPE				req		：起動種別								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void UartDrv_Init( E_INIT_TYPE req )
{
	UI_8 cnt;

	switch ( req ) {
	case E_INIT_RESET:			/* CPUリセットによる起動 */
	case E_INIT_WAKEUP:			/* 省電力解除（WAKEUP）による起動 */
		/* UARTレジスタ初期設定処理 */
		for ( cnt = 0U; cnt < UARTDRV_USEDCH_NUMBER; cnt++) {
			(void)uart_drv_init( C_UartUsedChTbl[cnt] );
		}
		break;
	case E_INIT_IGN_ON:			/* IGN ON */
		break;
	case E_INIT_RET_NORMAL_VOL:	/* 低電圧復帰 */
		break;
	case E_INIT_INTERVAL_WAKEUP:	/* 間欠起動ウェイクアップ */
		break;
	default:					/* 上記以外 */
		break;
	}
	
}

/****************************************************************************************/
/*	[モジュール名]	UartDrv_Sleep	[名称]	UARTﾚｼﾞｽﾀ停止設定処理						*/
/*======================================================================================*/
/*	[処理概要]	省電力モード遷移時に本関数を呼び、初期化を行う。						*/
/*======================================================================================*/
/*	[記述形式]	void UartDrv_Sleep( void )												*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void UartDrv_Sleep( void )
{
	UI_8 cnt;

	/* UARTレジスタ停止設定処理 */
	for ( cnt = 0U; cnt < UARTDRV_USEDCH_NUMBER; cnt++) {
		(void)uart_drv_stop( C_UartUsedChTbl[cnt] );
	}
}
/****************************************************************************************/
/*	[モジュール名]	UartDrv_Refresh	[名称]	UARTﾚｼﾞｽﾀ再設定処理							*/
/*======================================================================================*/
/*	[処理概要]	レジスタ再設定時に本関数を呼び、初期化を行う。							*/
/*======================================================================================*/
/*	[記述形式]	void UartDrv_Refresh( void )											*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void UartDrv_Refresh( void )
{
	
	/* 空関数 */
	
}

