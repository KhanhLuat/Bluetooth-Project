/************************************************************************************************/
/* 客先名		:	標準モジュール																*/
/* 機種名		:	SSFT																		*/
/* ﾏｲｺﾝｿﾌﾄﾃｰﾏ名	:	PF																			*/
/*==============================================================================================*/
/* 作成ﾌｧｲﾙ名	:	SSFTxxx_FR81_Flash_Drv_Config.h												*/
/* 				:	ﾌﾗｯｼｭｺﾝﾌｨｸﾞﾍｯﾀﾞ																*/
/*==============================================================================================*/
/* 対象ﾏｲｺﾝ		:	MB91570 Series																*/
/*==============================================================================================*/
/* 作成ﾊﾞｰｼﾞｮﾝ	:	010101																		*/
/* 作成年月日	:	2013.05.24																	*/
/* 作成者		:	M.Inoue																		*/
/*----------------------------------------------------------------------------------------------*/
/* 変更履歴		:	アプリ用に書換え機能を無しに設定											*/
/************************************************************************************************/
#ifndef __MB91570_FLASH_DRV_CONFIG_H__
#define __MB91570_FLASH_DRV_CONFIG_H__

#define FLASH_DRV_REPRGM_ENABLE						/* 書き換え機能有無設定 [定義有り:機能有り 定義無し:機能無し] */
/* #define FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM	 リプロ機能 実行エリア [定義有り:全てRAM 定義無し:ROMR⇔RAM] */

/*** START_INC ***/
/********************************************************************************/
/*   Include File                                                               */
/*------------------------------------------------------------------------------*/
/*      ﾍｯﾀﾞｰﾌｧｲﾙのｲﾝｸﾙｰﾄﾞ文は、下記ﾌｧｲﾙに記載すること                          */
/********************************************************************************/
#include "SSFTSTD_FR81_Interrupt_Drv.h"
#include "SSFTSTD_FR81_Wdt_Drv.h"
#include "SSFTSTD_FR81_SystemClock_Drv.h"

/*==============================================================================*/
/*	typedef定義(外部公開)														*/
/*==============================================================================*/
/* FR81ﾏｲｺﾝ用型定義	*/
typedef UI_32	T_FlashDrv_Addr;

/* 内部定数 */
#ifdef __MB91570_FLASH_DRV_INTERNAL__
/*==============================================================================*/
/*	typedef定義																	*/
/*==============================================================================*/

/*==============================================================================*/
/*	define定義（機種依存）														*/
/*==============================================================================*/
#define FLASH_START_ADDR			(0x00070000UL)	/* ﾌﾗｯｼｭﾒﾓﾘ開始ｱﾄﾞﾚｽ						*/
#define FLASH_END_ADDR				(0x0017FFFFUL)	/* ﾌﾗｯｼｭﾒﾓﾘ終了ｱﾄﾞﾚｽ						*/
#define FLASH_SC_UNIT				(0x04U)			/* ｾｸﾀ加算のｵﾌｾｯﾄ							*/

#define FLASH_DPOLL					(0x0080U)		/* 自動割り込み/消去中は反転ﾃﾞｰﾀ			*/
#define FLASH_TOGG1					(0x0040U)		/* 自動割り込み/消去中はﾄｸﾞﾙ動作			*/
#define FLASH_TLOV					(0x0020U)		/* 自動割り込み/消去中は0,ﾀｲﾑｱｳﾄ時は1		*/
#define FLASH_SETI					(0x0008U)		/* 自動消去ｳｪｲﾄ期間中は0,ﾀｲﾑｱｳﾄ時は1		*/

#define FLASH_TOGCHK_LOOPMAX		(0x02U)			/* ﾌﾞﾛｯｸ自動消去の結果ﾁｪｯｸﾙｰﾌﾟｶﾝｳﾄ			*/

#define FLASH_ALGO_OFFSET_MASK		(0xFFFFE000UL)	/* 自動ｱﾙｺﾞﾘｽﾞﾑ用ｵﾌｾｯﾄﾏｽｸ					*/
#define FLASH_ALGO1_OFFSET			(0x00001554UL)	/* 自動ｱﾙｺﾞﾘｽﾞﾑｺﾏﾝﾄﾞ1回目ｵﾌｾｯﾄ				*/
#define FLASH_ALGO2_OFFSET			(0x00000AA8UL)	/* 自動ｱﾙｺﾞﾘｽﾞﾑｺﾏﾝﾄﾞ2回目ｵﾌｾｯﾄ				*/
#define FLASH_ALGO3_OFFSET			(0x00001554UL)	/* 自動ｱﾙｺﾞﾘｽﾞﾑｺﾏﾝﾄﾞ3回目ｵﾌｾｯﾄ				*/
#define FLASH_ALGO4_OFFSET			(0x00001554UL)	/* 自動ｱﾙｺﾞﾘｽﾞﾑｺﾏﾝﾄﾞ4回目ｵﾌｾｯﾄ				*/
#define FLASH_ALGO5_OFFSET			(0x00000AA8UL)	/* 自動ｱﾙｺﾞﾘｽﾞﾑｺﾏﾝﾄﾞ5回目ｵﾌｾｯﾄ				*/
#define FLASH_ALGO1_VALUE			(0xAAAAU)		/* 自動ｱﾙｺﾞﾘｽﾞﾑｺﾏﾝﾄﾞ1回目書き込みﾃﾞｰﾀ		*/
#define FLASH_ALGO2_VALUE			(0x5555U)		/* 自動ｱﾙｺﾞﾘｽﾞﾑｺﾏﾝﾄﾞ2回目書き込みﾃﾞｰﾀ		*/
#define FLASH_ALGO3_VALUE			(0x8080U)		/* 自動ｱﾙｺﾞﾘｽﾞﾑｺﾏﾝﾄﾞ3回目書き込みﾃﾞｰﾀ		*/
#define FLASH_ALGO4_VALUE			(0xAAAAU)		/* 自動ｱﾙｺﾞﾘｽﾞﾑｺﾏﾝﾄﾞ4回目書き込みﾃﾞｰﾀ		*/
#define FLASH_ALGO5_VALUE			(0x5555U)		/* 自動ｱﾙｺﾞﾘｽﾞﾑｺﾏﾝﾄﾞ5回目書き込みﾃﾞｰﾀ		*/
#define FLASH_ALGOW_VALUE			(0xA0A0U)		/* 自動ｱﾙｺﾞﾘｽﾞﾑｺﾏﾝﾄﾞ書き込みﾃﾞｰﾀ			*/
#define FLASH_ALGOE_VALUE			(0x3030U)		/* 自動ｱﾙｺﾞﾘｽﾞﾑｺﾏﾝﾄﾞ消去ﾃﾞｰﾀ				*/
#define FLASH_ALGOR_VALUE			(0x00F0U)		/* 自動ｱﾙｺﾞﾘｽﾞﾑｺﾏﾝﾄﾞﾘｾｯﾄﾃﾞｰﾀ				*/

/* ────────────────────────────────────────────── */
/* 		フラッシュレジスタ																		*/
/* ────────────────────────────────────────────── */
#define INIT_FCTLR		(0x8800U)	/*	ﾌﾗｯｼｭ制御ﾚｼﾞｽﾀ													*/
									/*	<15>	-		: 1		(予約bit)								*/
									/*	<14>	FWE		: 0		(Flash書き込み許可:割り込み禁止)		*/
									/*	<13-12>	-		: 00	(予約bit)								*/
									/*	<11-10>	FSZ		: 10	(Flash書き込みｱｸｾｽｻｲｽﾞ設定:16bit)		*/
									/*	<09-08>	FAW		: 00	(FLASH ｱｸｾｽ/ｳｪｲﾄ設定:0ｻｲｸﾙ)				*/
									/*	<07>	FDSBL	: 0		(Flash Disable指示:Flash Enable)		*/
									/*	<06-05>	-		: 00	(予約bit)								*/
									/*	<04>	RDYF	: 0		(分岐ｱｸｾｽ時のRDYﾈｹﾞｰﾄ指示:FLASH I/F状態)*/
									/*	<03-00>	-		: 0000	(予約bit)								*/
									/*	--------------------------------------------------------------- */
#define INIT_FSTR		(0x01U)		/*	ﾌﾗｯｼｭｽﾃｰﾀｽﾚｼﾞｽﾀ													*/
									/*	<07-03>	-		: 0		(予約bit)								*/
									/*	<02>	FECCER	: 0		(ﾃﾞｰﾀ読み出しECC訂正発生:ｸﾘｱ)			*/
									/*	<01>	FHANG	: 0		(Flash Hang状態:通常状態)				*/
									/*	<00>	FRDY	: 1		(Flash 書き込み許可:動作完了)			*/
									/*	--------------------------------------------------------------- */
#define INIT_FLIFCTLR	(0x00U)		/*	ﾌﾗｯｼｭｲﾝﾀﾌｪｰｽ制御ﾚｼﾞｽﾀ											*/
									/*	<07-05>	-		:000	(予約bit)								*/
									/*	<04>	DFWDSBL	:0		(ﾃﾞｰﾀﾌｪｯﾁ/ｳｪｲﾄｻｲｸﾙ無効:ｳｪｲﾄ/ｻｲｸﾙ有効)	*/
									/*	<03-02>	-		:00		(予約bit)								*/
									/*	<01>	ECCDSBL1:0		(ECC 機能無効1:ﾜｰｸﾌﾗｯｼｭECC 機能有効)	*/
									/*	<00>	ECCDSBL0:0		(ECC 機能無効0:ﾌﾟﾛｸﾞﾗﾑﾌﾗｯｼｭECC機能有効)	*/
									/*	--------------------------------------------------------------- */

#define STOP_FCTLR		(0x8800U)	/*	INIT_FCTLR		と同じ設定値 */
#define STOP_FSTR		(0x01U)		/*	INIT_FSTR 		と同じ設定値 */
#define STOP_FLIFCTLR	(0x00U)		/*	INIT_FLIFCTLR	と同じ設定値 */

#endif	/* __MB91570_FLASH_DRV_INTERNAL__ */

#ifdef FLASH_DRV_REPRGM_ENABLE
#if defined __MB91570_FLASH_DRV_INTERNAL__ || defined __MB91570_FLASHCTRL_DRV_INTERNAL__
/*==============================================================================*/
/*	セクション配置（機種依存）													*/
/*==============================================================================*/
#define REPRGM_START_SEC_RAMCODE_AREA
#pragma segment CODE=RAMCODE	/* フラッシュドライバはRAMで動作させる */
//#include "SSFTxxx_ReprgmSecDefinition_Config.h"
/* RAMCODE START */

/*==============================================================================*/
/*	define定義（機種依存）														*/
/*==============================================================================*/
/* 待ち時間定義 CPUｸﾛｯｸ = 80M の時 0.125μs単位とする							*/
/*  (実際はもっと精度が悪いが、高い精度は不要)									*/
#define FLASH_WAIT_TM_FOR_WRITE			(0x00002A00UL)		/* 0.125μs * 10752 = 1344us = (384 + 288)us(16bit+ECC書込み最大時間) * 2	*/
#define FLASH_WAIT_TM_FOR_ERASE			(0x03D09000UL)		/* 0.125μs * 64000000 = 8000ms = 2000ms(64KBｾｸﾀ最大消去時間) * 2 * 2		*/

#endif	/* __MB91570_FLASH_DRV_INTERNAL__ || __MB91570_FLASHCTRL_DRV_INTERNAL__ */

#if defined __MB91570_FLASHCTRL_DRV_INTERNAL__
/*==============================================================================*/
/*	typedef定義(ｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞ公開)												*/
/*==============================================================================*/
/* ﾌﾗｯｼｭﾌﾞﾛｯｸﾃｰﾌﾞﾙの構造体 */
typedef struct t_flashblktbl_t{
	T_FlashDrv_Addr	start_addr;				/* ﾌﾗｯｼｭﾌﾞﾛｯｸの開始ｱﾄﾞﾚｽ			*/
	T_FlashDrv_Addr	end_addr;				/* ﾌﾗｯｼｭﾌﾞﾛｯｸの終了ｱﾄﾞﾚｽ			*/
}T_FlashBlkTbl;

/*==============================================================================*/
/*	define定義（機種依存）														*/
/*==============================================================================*/
#define FLASH_MAX_BLKNUM				(0x0CU)				/* 最大ﾌﾞﾛｯｸ数		*/

/*==============================================================================*/
/*	外部定義（機種依存）														*/
/*==============================================================================*/
static const T_FlashBlkTbl C_FlashBlkTbl[FLASH_MAX_BLKNUM]={
	{	0x00070000UL,	0x00073FFFUL	},			/* ﾌﾗｯｼｭﾌﾞﾛｯｸ0				*/
	{	0x00074000UL,	0x00077FFFUL	},			/* ﾌﾗｯｼｭﾌﾞﾛｯｸ1				*/
	{	0x00078000UL,	0x0007BFFFUL	},			/* ﾌﾗｯｼｭﾌﾞﾛｯｸ2				*/
	{	0x0007C000UL,	0x0007FFFFUL	},			/* ﾌﾗｯｼｭﾌﾞﾛｯｸ3				*/
	{	0x00080000UL,	0x0009FFFFUL	},			/* ﾌﾗｯｼｭﾌﾞﾛｯｸ4				*/
	{	0x000A0000UL,	0x000BFFFFUL	},			/* ﾌﾗｯｼｭﾌﾞﾛｯｸ5				*/
	{	0x000C0000UL,	0x000DFFFFUL	},			/* ﾌﾗｯｼｭﾌﾞﾛｯｸ6				*/
	{	0x000E0000UL,	0x000FFFFFUL	},			/* ﾌﾗｯｼｭﾌﾞﾛｯｸ7				*/
	{	0x00100000UL,	0x0011FFFFUL	},			/* ﾌﾗｯｼｭﾌﾞﾛｯｸ8				*/
	{	0x00120000UL,	0x0013FFFFUL	},			/* ﾌﾗｯｼｭﾌﾞﾛｯｸ9				*/
	{	0x00140000UL,	0x0015FFFFUL	},			/* ﾌﾗｯｼｭﾌﾞﾛｯｸ10				*/
	{	0x00160000UL,	0x0017FFFFUL	}			/* ﾌﾗｯｼｭﾌﾞﾛｯｸ11				*/
};
#endif	/* FLASH_DRV_REPRGM_ENABLE */

#endif	/* __MB91570_FLASHCTRL_DRV_INTERNAL__ */

#endif										/* __MB91570_FLASH_DRV_CONFIG_H__ 	*/
