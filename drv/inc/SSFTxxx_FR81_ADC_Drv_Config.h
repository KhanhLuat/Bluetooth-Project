/************************************************************************************************/
/* 客先名		:	機種依存モジュール															*/
/* 機種名		:	-																			*/
/* ﾏｲｺﾝｿﾌﾄﾃｰﾏ名	:	-																			*/
/*==============================================================================================*/
/* 作成ﾌｧｲﾙ名	:	SSFTxxx_FR81_ADC_Drv_Config.h												*/
/* 				:	A/Dﾍｯﾀﾞ																		*/
/*==============================================================================================*/
/* 対象ﾏｲｺﾝ		:	MB91570 Series																*/
/*==============================================================================================*/
/* 作成ﾊﾞｰｼﾞｮﾝ	:	010103																		*/
/* 作成年月日	:	2013.05.23																	*/
/* 作成者		:	K.Uchiyama																	*/
/*----------------------------------------------------------------------------------------------*/
/* 変更履歴		:																				*/
/* [010103]		:	新規作成																	*/
/************************************************************************************************/
#ifndef __MB91570_ADC_DRV_CONFIG_H__
#define __MB91570_ADC_DRV_CONFIG_H__

/* 内部定数 */
#ifdef __MB91570_ADC_DRV_INTERNAL__
/********************************************************************************/
/*   Include File                                                               */
/*------------------------------------------------------------------------------*/
/*      ﾍｯﾀﾞｰﾌｧｲﾙのｲﾝｸﾙｰﾄﾞ文は、下記ﾌｧｲﾙに記載すること                          */
/********************************************************************************/
#include "SSFTSTD_FR81_SystemClock_Drv.h"


/*==============================================================================*/
/*	typedef定義																	*/
/*==============================================================================*/

/*==============================================================================*/
/*	define定義（機種依存）														*/
/*==============================================================================*/

#define D_ADWAIT			15U			/* A/D変換時間[μs](設定15μs) */
#define D_ADWATCH			1U			/* A/D変換監視時間[μs](1μs周期) */

#define INIT_ADCT1			(0x0CU)		/* 変換時間設定レジスタ(上位) */
										/*	<15-10>	CT		: 000011	(比較操作時間のクロック分周値設定ビット PCLK=16MHz時 )	*/
										/*	<09-08>	ST		: 00		(アナログ入力サンプリング時間設定ビット PCLK=16MHz時 )	*/
#define INIT_ADCT0			(0x16U)		/* 変換時間設定レジスタ(下位) */
										/*	<07-00>	ST		: 00010110	(アナログ入力サンプリング時間設定ビット サンプリング時間)	*/
										/* ------------------------------------------------------------------- */
#define INIT_ADCS1			(0x00U)		/* A/D制御ステータスレジスタ(上位) */
										/* <07>		BUSY	: 0			(強制停止指示ビット/動作確認ビット A/Dコンバータ強制停止) */
										/* <06>		INT		: 0			(A/D変換終了フラグ/割込み要求ビット フラグのクリア) */
										/* <05>		INTE	: 0			(A/D割込み要求許可ビット 割込み要求禁止) */
										/* <04>		PAUS	: 0			(A/D一時停止フラグビット フラグのクリア) */
										/* <03-02>	STS		: 00		(A/D変換の起動要因の選択 ソフトトリガ) */
										/* <01>		STRT	: 0			(A/D変換のソフトトリガビット 動作に影響なし) */
										/* <00>		-		: 0			(予約bit) */
										/* ------------------------------------------------------------------- */
#define INIT_ADCS0			(0x40U)		/* A/D制御ステータスレジスタ(下位) */
										/* <07-06>	MD		: 01		(動作モード設定ビット 単発変換モード、動作中の再起動不可能) */
										/* <05>		S10		: 0			(分解能設定ビット 10ビット A/D変換) */
										/* <04-00>	-		: 0			(予約bit) */
										/* ------------------------------------------------------------------- */
#define INIT_ADERH			(0x0000U)	/* アナログ入力許可レジスタ */
										/* <15-08>	ADE		: 00000000	(アナログ入力許可ビット 31ch〜24ch	) */
										/* <07-00>	ADE		: 00000000	(アナログ入力許可ビット 23ch〜16ch	) */
#define INIT_ADERL			(0x0001U)	/* アナログ入力許可レジスタ */
										/* <15-08>	ADE		: 00000000	(アナログ入力許可ビット 15ch〜 8ch	) */
										/* <07-00>	ADE		: 00000011	(アナログ入力許可ビット  7ch〜 0ch	) */
										/* ------------------------------------------------------------------- */
#define INIT_EADERLL		(0x00U)		/* 拡張アナログ入力許可レジスタ */
										/* <07-00>	ADE		: 00000000	(アナログ入力許可ビット 32ch〜39ch	) */
										/* ------------------------------------------------------------------- */
#define INIT_ADSCH			(0x00U)		/* A/D開始/終了チャネル設定レジスタ */
										/* <07-06>	-		:00			(予約bit) */
										/* <05-00>	ANS		:0			(開始チャネル 初期値) */
										/* ------------------------------------------------------------------- */
#define INIT_ADECH			(0x00U)		/* A/D開始/終了チャネル設定レジスタ */
										/* <07-06>	-		:00			(予約bit) */
										/* <05-00>	ANE		:0			(終了チャネル 初期値) */
										/* ------------------------------------------------------------------- */

#define STOP_ADCT1			(0x0CU)		/* INIT_ADCT1と同じ設定値 */
#define STOP_ADCT0			(0x16U)		/* INIT_ADCT0と同じ設定値 */
#define STOP_ADCS1			(0x00U)		/* INIT_ADCS1と同じ設定値 */
#define STOP_ADCS0			(0x40U)		/* INIT_ADCS0と同じ設定値 */
#define STOP_ADERH			(0x0000U)	/* INIT_ADERHと同じ設定値 */
#define STOP_ADERL			(0x0001U)	/* INIT_ADERLと同じ設定値 */
#define STOP_EADERLL		(0x00U)		/* INIT_EADERLLと同じ設定値 */
#define STOP_ADSCH			(0x00U)		/* INIT_ADSCHと同じ設定値 */
#define STOP_ADECH			(0x00U)		/* INIT_ADECHと同じ設定値 */

#endif								/* __MB91570_ADC_DRV_INTERNAL__ */

/* 外部公開定数 */
/*==============================================================================*/
/*	enum定義（外部非公開：アプリ側で使用する場合、外部公開用に再度定義すること	*/
/*==============================================================================*/
/* A/D CHを指定する型 */
typedef enum{
	ADCDRV_CH0,			/* A/D ch0 */
	ADCDRV_CH1,			/* A/D ch1 */
	ADCDRV_CH2,			/* A/D ch2 */
	ADCDRV_CH3,			/* A/D ch3 */
	ADCDRV_CH4,			/* A/D ch4 */
	ADCDRV_CH5,			/* A/D ch5 */
	ADCDRV_CH6,			/* A/D ch6 */
	ADCDRV_CH7,			/* A/D ch7 */
	ADCDRV_CH8,			/* A/D ch8 */
	ADCDRV_CH9,			/* A/D ch9 */
	ADCDRV_CH10,		/* A/D ch10 */
	ADCDRV_CH11,		/* A/D ch11 */
	ADCDRV_CH12,		/* A/D ch12 */
	ADCDRV_CH13,		/* A/D ch13 */
	ADCDRV_CH14,		/* A/D ch14 */
	ADCDRV_CH15,		/* A/D ch15 */
	ADCDRV_CH16,		/* A/D ch16 */
	ADCDRV_CH17,		/* A/D ch17 */
	ADCDRV_CH18,		/* A/D ch18 */
	ADCDRV_CH19,		/* A/D ch19 */
	ADCDRV_CH20,		/* A/D ch20 */
	ADCDRV_CH21,		/* A/D ch21 */
	ADCDRV_CH22,		/* A/D ch22 */
	ADCDRV_CH23,		/* A/D ch23 */
	ADCDRV_CH24,		/* A/D ch24 */
	ADCDRV_CH25,		/* A/D ch25 */
	ADCDRV_CH26,		/* A/D ch26 */
	ADCDRV_CH27,		/* A/D ch27 */
	ADCDRV_CH28,		/* A/D ch28 */
	ADCDRV_CH29,		/* A/D ch29 */
	ADCDRV_CH30,		/* A/D ch30 */
	ADCDRV_CH31,		/* A/D ch31 */
	ADCDRV_CH32,		/* A/D ch32 */
	ADCDRV_CH33,		/* A/D ch33 */
	ADCDRV_CH34,		/* A/D ch34 */
	ADCDRV_CH35,		/* A/D ch35 */
	ADCDRV_CH36,		/* A/D ch36 */
	ADCDRV_CH37,		/* A/D ch37 */
	ADCDRV_CH38,		/* A/D ch38 */
	ADCDRV_CH39,		/* A/D ch39 */
	ADCDRV_CH_NUMBER	/* A/D ch数 最大 */
}E_ADCDRV_CH;


/*==============================================================================*/
/*	define定義（外部公開）														*/
/*==============================================================================*/
#define AD_CH_OPERATION_SW		ADCDRV_CH0			/* 操作 SW A/D入力チャネル */

#endif	/*	__MB91570_ADC_DRV_CONFIG_H__ 	*/
