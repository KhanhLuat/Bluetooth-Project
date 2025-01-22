/************************************************************************************************/
/* 客先名		:	標準モジュール																*/
/* 機種名		:	SSFT																		*/
/* ﾏｲｺﾝｿﾌﾄﾃｰﾏ名	:	PF																			*/
/*==============================================================================================*/
/* 作成ﾌｧｲﾙ名	:	SSFTSTD_FR81_Port_Drv.c														*/
/* 				:	GPIOﾄﾞﾗｲﾊﾞﾓｼﾞｭｰﾙ															*/
/*==============================================================================================*/
/* 対象ﾏｲｺﾝ		:	MB91570 Series																*/
/*==============================================================================================*/
/* 作成ﾊﾞｰｼﾞｮﾝ	:	030103																		*/
/* 作成年月日	:	2014.01.23																	*/
/* 作成者		:	K.Uchiyama																	*/
/*----------------------------------------------------------------------------------------------*/
/* 変更履歴		:																				*/
/* [010101]		:	新規作成																	*/
/* [010102]		:	P127機能切り替えテーブル修正												*/
/*				:	ポート機能切り替え時のポートに一瞬戻す動作修正								*/
/* [010103]		:	標準IF対応。IF関数の戻り値をSI_8化。引数E_PORTDRV_BYTE_NUM追加（予約設計）	*/
/* [010104]		:	ビットマスク位置の定義箇所変更。セキュリティインジケータのポート修正。		*/
/* [010105]		:	EEPROM、シフトレジスタのポート定義追加										*/
/* [030101]		:	ROM容量と処理時間削減のため、コンパイルSW追加、未使用のテーブル削除			*/
/* [030102]		:	インクルード構成見直し。													*/
/* [030103]		:	規則対応。																	*/
/************************************************************************************************/

/*** START_INC ***/
/********************************************************************************/
/*   Include File                                                               */
/*------------------------------------------------------------------------------*/
/*      ﾍｯﾀﾞｰﾌｧｲﾙのｲﾝｸﾙｰﾄﾞ文は、下記ﾌｧｲﾙに記載すること                          */
/********************************************************************************/
#include "SSFTSTD_Type.h"
#include "SSFTSTD_Macro.h"
#include "mb91570.h"

#define	 __MB91570_PORT_DRV_INTERNAL__
#include "SSFTSTD_FR81_Port_Drv.h"

/*==============================================================================*/
/*	内部静的変数																*/
/*==============================================================================*/
static SI_8 S_SelfcheckState;			/* 出力方向レジスタリフレッシュ処理のセルフチェック状態 */
static SI_8 S_ChkSetReg;				/* 優先度の高い割込み内でのレジスタ設定の禁止/許可 */


/*==============================================================================*/
/*	typedef定義（外部非公開）													*/
/*==============================================================================*/
/* なし */

/*==============================================================================*/
/*	関数ﾌﾟﾛﾄﾀｲﾌﾟ宣言（外部非公開)												*/
/*==============================================================================*/
/* なし */

/*==============================================================================*/
/*	内部定数																	*/
/*==============================================================================*/

/*【define定義】*/

#define	PORT_FUNC_CHANGE_NUM0	(14)	/* P0の機能レジスタ数 */
#define	PORT_FUNC_CHANGE_NUM1	(5)		/* P1の機能レジスタ数 */
#define	PORT_FUNC_CHANGE_NUM2	(8)		/* P2の機能レジスタ数 */
#define	PORT_FUNC_CHANGE_NUM3	(9)		/* P3の機能レジスタ数 */
#define	PORT_FUNC_CHANGE_NUM4	(10)	/* P4の機能レジスタ数 */
#define	PORT_FUNC_CHANGE_NUM5	(5)		/* P5の機能レジスタ数 */
#define	PORT_FUNC_CHANGE_NUM6	(8)		/* P6の機能レジスタ数 */
#define	PORT_FUNC_CHANGE_NUM7	(10)	/* P7の機能レジスタ数 */
#define	PORT_FUNC_CHANGE_NUM8	(9)		/* P8の機能レジスタ数 */
#define	PORT_FUNC_CHANGE_NUM9	(16)	/* P9の機能レジスタ数 */
#define	PORT_FUNC_CHANGE_NUM10	(17)	/* P10の機能レジスタ数 */
#define	PORT_FUNC_CHANGE_NUM11	(16)	/* P11の機能レジスタ数 */
#define	PORT_FUNC_CHANGE_NUM12	(18)	/* P12の機能レジスタ数 */
#define	PORT_FUNC_CHANGE_NUM13	(10)	/* P13の機能レジスタ数 */

#define	PORT_FUNC_CHANGE0	(PORT_FUNC_CHANGE_NUM0 + PORT_FUNC)		/* P0の機能レジスタ設定数 */
#define	PORT_FUNC_CHANGE1	(PORT_FUNC_CHANGE_NUM1 + PORT_FUNC)		/* P1の機能レジスタ設定数 */
#define	PORT_FUNC_CHANGE2	(PORT_FUNC_CHANGE_NUM2 + PORT_FUNC)		/* P2の機能レジスタ設定数 */
#define	PORT_FUNC_CHANGE3	(PORT_FUNC_CHANGE_NUM3 + PORT_FUNC)		/* P3の機能レジスタ設定数 */
#define	PORT_FUNC_CHANGE4	(PORT_FUNC_CHANGE_NUM4 + PORT_FUNC)		/* P4の機能レジスタ設定数 */
#define	PORT_FUNC_CHANGE5	(PORT_FUNC_CHANGE_NUM5 + PORT_FUNC)		/* P5の機能レジスタ設定数 */
#define	PORT_FUNC_CHANGE6	(PORT_FUNC_CHANGE_NUM6 + PORT_FUNC)		/* P6の機能レジスタ設定数 */
#define	PORT_FUNC_CHANGE7	(PORT_FUNC_CHANGE_NUM7 + PORT_FUNC)		/* P7の機能レジスタ設定数 */
#define	PORT_FUNC_CHANGE8	(PORT_FUNC_CHANGE_NUM8 + PORT_FUNC)		/* P8の機能レジスタ設定数 */
#define	PORT_FUNC_CHANGE9	(PORT_FUNC_CHANGE_NUM9 + PORT_FUNC)		/* P9の機能レジスタ設定数 */
#define	PORT_FUNC_CHANGE10	(PORT_FUNC_CHANGE_NUM10 + PORT_FUNC)	/* P10の機能レジスタ設定数 */
#define	PORT_FUNC_CHANGE11	(PORT_FUNC_CHANGE_NUM11 + PORT_FUNC)	/* P11の機能レジスタ設定数 */
#define	PORT_FUNC_CHANGE12	(PORT_FUNC_CHANGE_NUM12 + PORT_FUNC)	/* P12の機能レジスタ設定数 */
#define	PORT_FUNC_CHANGE13	(PORT_FUNC_CHANGE_NUM13 + PORT_FUNC)	/* P13の機能レジスタ設定数 */

#define	PORT_TABLE_DDR		(0)		/* データ方向 */
#define	PORT_TABLE_PFR		(1)		/* ポート機能 */
#define	PORT_TABLE_EPFR		(2)		/* 拡張ポート機能設定開始値 */

#define	PORT_FUNC_CHANGE_ID_NUM_3	(3)		/* 機能切替設定値テーブルの機能数(列要素)3 */
#define	PORT_FUNC_CHANGE_ID_NUM_4	(4)		/* 機能切替設定値テーブルの機能数(列要素)4 */
#define	PORT_FUNC_CHANGE_ID_NUM_5	(5)		/* 機能切替設定値テーブルの機能数(列要素)5 */
#define	PORT_FUNC_CHANGE_ID_NUM_6	(6)		/* 機能切替設定値テーブルの機能数(列要素)6 */
#define	PORT_FUNC_CHANGE_ID_NUM_7	(7)		/* 機能切替設定値テーブルの機能数(列要素)7 */
#define	PORT_FUNC_CHANGE_ID_NUM_8	(8)		/* 機能切替設定値テーブルの機能数(列要素)8 */
#define	PORT_FUNC_CHANGE_ID_NUM_9	(9)		/* 機能切替設定値テーブルの機能数(列要素)9 */
#define	PORT_FUNC_CHANGE_ID_NUM_10	(10)	/* 機能切替設定値テーブルの機能数(列要素)10 */
#define	PORT_FUNC_CHANGE_ID_NUM_11	(11)	/* 機能切替設定値テーブルの機能数(列要素)11 */

#define DATA_MIN			(0)			/* データ範囲最小値 */
#define PORT_FUNC			(1)			/* テーブル要素ID追加用 */
#define PORTDRV_EPFR_MAX	(51)		/* 拡張ポート機能レジスタ数 */
#define BIT_GET				(0x0700u)	/* ビット位置抽出用 */
#define BIT_SHIFT			(8)			/* ビットシフト用 */

#define	NO_MASK			(0x8000u)	/* マスクなし */

/* 全体 */
#define D_LOW_PXX0		(0x40FEu)	/* ビット0セット値LOW */
#define D_LOW_PXX1		(0x40FDu)	/* ビット1セット値LOW */
#define D_LOW_PXX2		(0x40FBu)	/* ビット2セット値LOW */
#define D_LOW_PXX3		(0x40F7u)	/* ビット3セット値LOW */
#define D_LOW_PXX4		(0x40EFu)	/* ビット4セット値LOW */
#define D_LOW_PXX5		(0x40DFu)	/* ビット5セット値LOW */
#define D_LOW_PXX6		(0x40BFu)	/* ビット6セット値LOW */
#define D_LOW_PXX7		(0x407Fu)	/* ビット7セット値LOW */

#define D_HIGH_PXX0		(0x2001u)	/* ビット0セット値HIGH */
#define D_HIGH_PXX1		(0x2002u)	/* ビット1セット値HIGH */
#define D_HIGH_PXX2		(0x2004u)	/* ビット2セット値HIGH */
#define D_HIGH_PXX3		(0x2008u)	/* ビット3セット値HIGH */
#define D_HIGH_PXX4		(0x2010u)	/* ビット4セット値HIGH */
#define D_HIGH_PXX5		(0x2020u)	/* ビット5セット値HIGH */
#define D_HIGH_PXX6		(0x2040u)	/* ビット6セット値HIGH */
#define D_HIGH_PXX7		(0x2080u)	/* ビット7セット値HIGH */

/* <<P00>> */
	
/* <P000> */
#define EPFR02_TIN0E_10		(0x11FCu)	/* EPFR02_TIN0Eのセット値10 */
#define EPFR06_SIN2E_HIGH	(0x2001u)	/* EPFR06_SIN2Eのセット値HIGH */
#define EPFR10_PPG0E_XX0	(0x40FEu)	/* EPFR10_PPG0Eのセット値XX0 */
#define EPFR10_PPG0E_XX1	(0x2001u)	/* EPFR10_PPG0Eのセット値XX1 */
#define EPFR43_INT0E_HIGH	(0x2001u)	/* EPFR43_INT0Eのセット値HIGH */
#define EPFR47_EBDSWPE_LOW	(0x40FEu)	/* EPFR47_EBDSWPEのセット値LOW */
#define EPFR47_EBDSWPE_HIGH	(0x2001u)	/* EPFR47_EBDSWPEのセット値HIGH */

/* <P001> */
#define EPFR03_TIN1E_10		(0x11FCu)	/* EPFR03_TIN1Eのセット値10 */
#define EPFR06_SOT2E_0X		(0x40EFu)	/* EPFR06_SOT2Eのセット値0X */
#define EPFR06_SOT2E_1X		(0x2010u)	/* EPFR06_SOT2Eのセット値1X */
#define EPFR10_PPG1E_XXX0	(0x40F7u)	/* EPFR10_PPG1Eのセット値XXX0 */
#define EPFR10_PPG1E_XXX1	(0x2008u)	/* EPFR10_PPG1Eのセット値XXX1 */
#define EPFR43_INT1E_HIGH	(0x2002u)	/* EPFR43_INT1Eのセット値HIGH */

/* <P002> */
#define EPFR04_TIN2E_10		(0x11FCu)	/* EPFR04_TIN2Eのセット値10 */
#define EPFR06_SCK2E_0X		(0x40FBu)	/* EPFR06_SCK2Eのセット値0X */
#define EPFR06_SCK2E_10		(0x12F9u)	/* EPFR06_SCK2Eのセット値10 */
#define EPFR11_PPG2E_XX0	(0x40FEu)	/* EPFR11_PPG2Eのセット値XX0 */
#define EPFR11_PPG2E_XX1	(0x2001u)	/* EPFR11_PPG2Eのセット値XX1 */
#define EPFR43_INT2E_HIGH	(0x2004u)	/* EPFR43_INT2Eのセット値HIGH */

/* <P003> */
#define EPFR05_TIN3E_10		(0x11FCu)	/* EPFR05_TIN3Eのセット値10 */
#define EPFR07_SIN3E_HIGH	(0x2001u)	/* EPFR07_SIN3Eのセット値HIGH */
#define EPFR11_PPG3E_XX0	(0x40F7u)	/* EPFR11_PPG3Eのセット値XX0 */
#define EPFR11_PPG3E_XX1	(0x2008u)	/* EPFR11_PPG3Eのセット値XX1 */
#define EPFR43_INT3E_HIGH	(0x2008u)	/* EPFR43_INT3Eのセット値HIGH */

/* <P004> */
#define EPFR02_TOT0E_0XX	(0x40EFu)	/* EPFR02_TOT0Eのセット値0XX */
#define EPFR02_TOT0E_1XX	(0x2010u)	/* EPFR02_TOT0Eのセット値1XX */
#define EPFR07_SOT3E_0X		(0x40EFu)	/* EPFR07_SOT3Eのセット値0X */
#define EPFR07_SOT3E_1X		(0x2010u)	/* EPFR07_SOT3Eのセット値1X */
#define EPFR12_PPG4E_XX0	(0x40FEu)	/* EPFR12_PPG4Eのセット値XX0 */
#define EPFR12_PPG4E_XX1	(0x2001u)	/* EPFR12_PPG4Eのセット値XX1 */
#define EPFR43_INT4E_HIGH	(0x2010u)	/* EPFR43_INT4Eのセット値HIGH */

/* <P005> */
#define EPFR03_TOT1E_0XX	(0x40EFu)	/* EPFR03_TOT1Eのセット値0XX */
#define EPFR03_TOT1E_1XX	(0x2010u)	/* EPFR03_TOT1Eのセット値1XX */
#define EPFR07_SCK3E_0X		(0x40FBu)	/* EPFR07_SCK3Eのセット値0X */
#define EPFR07_SCK3E_10		(0x12F9u)	/* EPFR07_SCK3Eのセット値10 */
#define EPFR12_PPG5E_XX0	(0x40F7u)	/* EPFR12_PPG5Eのセット値XX0 */
#define EPFR12_PPG5E_XX1	(0x2008u)	/* EPFR12_PPG5Eのセット値XX1 */
#define EPFR43_INT5E_HIGH	(0x2020u)	/* EPFR43_INT5Eのセット値HIGH */

/* <P006> */
#define EPFR04_TOT2E_0XX	(0x40EFu)	/* EPFR04_TOT2Eのセット値0XX */
#define EPFR04_TOT2E_1XX	(0x2010u)	/* EPFR04_TOT2Eのセット値1XX */
#define EPFR13_PPG6E_XX0	(0x40FEu)	/* EPFR13_PPG6Eのセット値XX0 */
#define EPFR13_PPG6E_XX1	(0x2001u)	/* EPFR13_PPG6Eのセット値XX1 */
#define EPFR43_INT6E_HIGH	(0x2040u)	/* EPFR43_INT6Eのセット値HIGH */

/* <P007> */
#define EPFR05_TOT3E_0XX	(0x40EFu)	/* EPFR05_TOT3Eのセット値0XX */
#define EPFR05_TOT3E_1XX	(0x2010u)	/* EPFR05_TOT3Eのセット値1XX */
#define EPFR13_PPG7E_XX0	(0x40F7u)	/* EPFR13_PPG7Eのセット値XX0 */
#define EPFR13_PPG7E_XX1	(0x2008u)	/* EPFR13_PPG7Eのセット値XX1 */
#define EPFR43_INT7E_HIGH	(0x2080u)	/* EPFR43_INT7Eのセット値HIGH */

/* <<P01>> */

/* <P010> */
#define EPFR16_SEG0E_LOW	(0x40FEu)	/* EPFR16_SEG0Eのセット値LOW */
#define EPFR16_SEG0E_HIGH	(0x2001u)	/* EPFR16_SEG0Eのセット値HIGH */
#define EPFR44_INT8E_HIGH	(0x2001u)	/* EPFR44_INT8Eのセット値HIGH */

/* <P011> */
#define EPFR16_SEG1E_LOW	(0x40FDu)	/* EPFR16_SEG1Eのセット値LOW */
#define EPFR16_SEG1E_HIGH	(0x2002u)	/* EPFR16_SEG1Eのセット値HIGH */
#define EPFR44_INT9E_HIGH	(0x2002u)	/* EPFR44_INT9Eのセット値HIGH */

/* <P012> */
#define EPFR16_SEG2E_LOW	(0x40FBu)	/* EPFR16_SEG2Eのセット値LOW */
#define EPFR16_SEG2E_HIGH	(0x2004u)	/* EPFR16_SEG2Eのセット値HIGH */
#define EPFR44_INT10E_HIGH	(0x2004u)	/* EPFR44_INT10Eのセット値HIGH */

/* <P013> */
#define EPFR16_SEG3E_LOW	(0x40F7u)	/* EPFR16_SEG3Eのセット値LOW */
#define EPFR16_SEG3E_HIGH	(0x2008u)	/* EPFR16_SEG3Eのセット値HIGH */
#define EPFR44_INT11E_HIGH	(0x2008u)	/* EPFR44_INT11Eのセット値HIGH */

/* <P014> */
#define EPFR16_SEG4E_LOW	(0x40EFu)	/* EPFR16_SEG4Eのセット値LOW */
#define EPFR16_SEG4E_HIGH	(0x2010u)	/* EPFR16_SEG4Eのセット値HIGH */
#define EPFR44_INT12E_HIGH	(0x2010u)	/* EPFR44_INT12Eのセット値HIGH */

/* <P015> */
#define EPFR16_SEG5E_LOW	(0x40DFu)	/* EPFR16_SEG5Eのセット値LOW */
#define EPFR16_SEG5E_HIGH	(0x2020u)	/* EPFR16_SEG5Eのセット値HIGH */
#define EPFR44_INT13E_HIGH	(0x2020u)	/* EPFR44_INT13Eのセット値HIGH */

/* <P016> */
#define EPFR16_SEG6E_LOW	(0x40BFu)	/* EPFR16_SEG6Eのセット値LOW */
#define EPFR16_SEG6E_HIGH	(0x2040u)	/* EPFR16_SEG6Eのセット値HIGH */
#define EPFR44_INT14E_HIGH	(0x2040u)	/* EPFR44_INT14Eのセット値HIGH */

/* <P017> */
#define EPFR16_SEG7E_LOW	(0x407Fu)	/* EPFR16_SEG7Eのセット値LOW */
#define EPFR16_SEG7E_HIGH	(0x2080u)	/* EPFR16_SEG7Eのセット値HIGH */
#define EPFR44_INT15E_HIGH	(0x2080u)	/* EPFR44_INT15Eのセット値HIGH */

/* <<P02>> */

/* <P020> */
#define EPFR01_ICU6E_00		(0x40CFu)	/* EPFR01_ICU6Eのセット値00 */
#define EPFR17_SEG8E_LOW	(0x40FEu)	/* EPFR17_SEG8Eのセット値LOW */
#define EPFR17_SEG8E_HIGH	(0x2001u)	/* EPFR17_SEG8Eのセット値HIGH */
#define EPFR29_OCU0E_0X		(0x40FDu)	/* EPFR29_OCU0Eのセット値0X */
#define EPFR29_OCU0E_10		(0x11FCu)	/* EPFR29_OCU0Eのセット値10 */

/* <P021> */
#define EPFR01_ICU7E_00		(0x403Fu)	/* EPFR01_ICU7Eのセット値00 */
#define EPFR17_SEG9E_LOW	(0x40FDu)	/* EPFR17_SEG9Eのセット値LOW */
#define EPFR17_SEG9E_HIGH	(0x2002u)	/* EPFR17_SEG9Eのセット値HIGH */
#define EPFR29_OCU1E_0X		(0x40F7u)	/* EPFR29_OCU1Eのセット値0X */
#define EPFR29_OCU1E_10		(0x13F3u)	/* EPFR29_OCU1Eのセット値10 */

/* <P022> */
#define EPFR17_SEG10E_LOW	(0x40FBu)	/* EPFR17_SEG10Eのセット値LOW */
#define EPFR17_SEG10E_HIGH	(0x2004u)	/* EPFR17_SEG10Eのセット値HIGH */
#define EPFR29_OCU2E_0X		(0x40DFu)	/* EPFR29_OCU2Eのセット値0X */
#define EPFR29_OCU2E_10		(0x15CFu)	/* EPFR29_OCU2Eのセット値10 */
#define EPFR39_ICU8E_00		(0x40FCu)	/* EPFR39_ICU8Eのセット値00 */

/* <P023> */
#define EPFR17_SEG11E_LOW	(0x40F7u)	/* EPFR17_SEG11Eのセット値LOW */
#define EPFR17_SEG11E_HIGH	(0x2008u)	/* EPFR17_SEG11Eのセット値HIGH */
#define EPFR29_OCU3E_0X		(0x407Fu)	/* EPFR29_OCU3Eのセット値0X */
#define EPFR29_OCU3E_10		(0x173Fu)	/* EPFR29_OCU3Eのセット値10 */
#define EPFR39_ICU9E_00		(0x40F3u)	/* EPFR39_ICU9Eのセット値00 */

/* <P024> */
#define EPFR17_SEG12E_LOW	(0x40EFu)	/* EPFR17_SEG12Eのセット値LOW */
#define EPFR17_SEG12E_HIGH	(0x2010u)	/* EPFR17_SEG12Eのセット値HIGH */
#define EPFR32_OCU11E_X0	(0x40BFu)	/* EPFR32_OCU11Eのセット値X0 */
#define EPFR32_OCU11E_01	(0x163Fu)	/* EPFR32_OCU11Eのセット値01 */
#define EPFR39_ICU10E_00	(0x40CFu)	/* EPFR39_ICU10Eのセット値00 */

/* <P025> */
#define EPFR17_SEG13E_LOW	(0x40DFu)	/* EPFR17_SEG13Eのセット値LOW */
#define EPFR17_SEG13E_HIGH	(0x2020u)	/* EPFR17_SEG13Eのセット値HIGH */
#define EPFR32_OCU10E_X0	(0x40EFu)	/* EPFR32_OCU10Eのセット値00 */
#define EPFR32_OCU10E_01	(0x14CFu)	/* EPFR32_OCU10Eのセット値01 */
#define EPFR39_ICU11E_00	(0x403Fu)	/* EPFR39_ICU11Eのセット値00 */

/* <P026> */
#define EPFR17_SEG14E_LOW	(0x40BFu)	/* EPFR17_SEG14Eのセット値LOW */
#define EPFR17_SEG14E_HIGH	(0x2040u)	/* EPFR17_SEG14Eのセット値HIGH */
#define EPFR32_OCU9E_X0		(0x40FBu)	/* EPFR32_OCU9Eのセット値X0 */
#define EPFR32_OCU9E_01		(0x12F3u)	/* EPFR32_OCU9Eのセット値01 */
#define EPFR33_SIN6E_HIGH	(0x2001u)	/* EPFR33_SIN6Eのセット値HIGH */

/* <P027> */
#define EPFR17_SEG15E_LOW	(0x407Fu)	/* EPFR17_SEG15Eのセット値LOW */
#define EPFR17_SEG15E_HIGH	(0x2080u)	/* EPFR17_SEG15Eのセット値HIGH */
#define EPFR32_OCU8E_X0		(0x40FEu)	/* EPFR32_OCU8Eのセット値X0 */
#define EPFR32_OCU8E_01		(0x10FCu)	/* EPFR32_OCU8Eのセット値01 */
#define EPFR33_SOT6E_0X		(0x40EFu)	/* EPFR33_SOT6Eのセット値0X */
#define EPFR33_SOT6E_1X		(0x2010u)	/* EPFR33_SOT6Eのセット値1X */

/* <<P03>> */

/* <P030> */
#define EPFR18_SEG16E_LOW	(0x40FEu)	/* EPFR18_SEG16Eのセット値LOW */
#define EPFR18_SEG16E_HIGH	(0x2001u)	/* EPFR18_SEG16Eのセット値HIGH */
#define EPFR33_SCK6E_0X		(0x40FBu)	/* EPFR33_SCK6Eのセット値0X */ 
#define EPFR33_SCK6E_10		(0x12F9u)	/* EPFR33_SCK6Eのセット値10 */

/* <P031> */
#define EPFR18_SEG17E_LOW	(0x40FDu)	/* EPFR18_SEG17Eのセット値LOW */
#define EPFR18_SEG17E_HIGH	(0x2002u)	/* EPFR18_SEG17Eのセット値HIGH */
#define EPFR38_SIN9E_LOW	(0x40FEu)	/* EPFR38_SIN9Eのセット値LOW */

/* <P032> */
#define EPFR18_SEG18E_LOW	(0x40BFu)	/* EPFR18_SEG18Eのセット値LOW */
#define EPFR18_SEG18E_HIGH	(0x2004u)	/* EPFR18_SEG18Eのセット値HIGH */
#define EPFR31_OCU7E_X0		(0x40BFu)	/* EPFR31_OCU7Eのセット値X0 */
#define EPFR31_OCU7E_01		(0x163Fu)	/* EPFR31_OCU7Eのセット値01 */
#define EPFR38_SOT9E_X0		(0x40F7u)	/* EPFR38_SOT9Eのセット値X0 */
#define EPFR38_SOT9E_01		(0x13E7u)	/* EPFR38_SOT9Eのセット値01 */

/* <P033> */
#define EPFR18_SEG19E_LOW	(0x40F7u)	/* EPFR18_SEG19Eのセット値LOW */
#define EPFR18_SEG19E_HIGH	(0x2008u)	/* EPFR18_SEG19Eのセット値HIGH */
#define EPFR31_OCU6E_X0		(0x40EFu)	/* EPFR31_OCU6Eのセット値X0 */
#define EPFR31_OCU6E_01		(0x14CFu)	/* EPFR31_OCU6Eのセット値01 */
#define EPFR38_SCK9E_X0		(0x40FDu)	/* EPFR38_SCK9Eのセット値X0 */
#define EPFR38_SCK9E_01		(0x11F9u)	/* EPFR38_SCK9Eのセット値01 */

/* <P034> */
#define EPFR18_SEG20E_LOW	(0x40EFu)	/* EPFR18_SEG20Eのセット値LOW */
#define EPFR18_SEG20E_HIGH	(0x2010u)	/* EPFR18_SEG20Eのセット値HIGH */
#define EPFR31_OCU5E_0X		(0x40F7u)	/* EPFR31_OCU5Eのセット値0X */
#define EPFR31_OCU5E_10		(0x13F3u)	/* EPFR31_OCU5Eのセット値10 */
#define EPFR37_SIN8E_00		(0x40FCu)	/* EPFR37_SIN8Eのセット値00 */

/* <P035> */
#define EPFR18_SEG21E_LOW	(0x40DFu)	/* EPFR18_SEG21Eのセット値LOW */
#define EPFR18_SEG21E_HIGH	(0x2020u)	/* EPFR18_SEG21Eのセット値HIGH */
#define EPFR31_OCU4E_0X		(0x40FDu)	/* EPFR31_OCU4Eのセット値0X */
#define EPFR31_OCU4E_10		(0x11FCu)	/* EPFR31_OCU4Eのセット値10 */
#define EPFR37_SOT8E_XX0	(0x40DFu)	/* EPFR37_SOT8Eのセット値XX0 */
#define EPFR37_SOT8E_001	(0x151Fu)	/* EPFR37_SOT8Eのセット値001 */

/* <P036> */
#define EPFR15_PPG11E_X0	(0x40F7u)	/* EPFR15_PPG11Eのセット値X0 */
#define EPFR15_PPG11E_01	(0x13E7u)	/* EPFR15_PPG11Eのセット値01 */
#define EPFR18_SEG22E_LOW	(0x40BFu)	/* EPFR18_SEG22Eのセット値LOW */
#define EPFR18_SEG22E_HIGH	(0x2040u)	/* EPFR18_SEG22Eのセット値HIGH */
#define EPFR37_SCK8E_XX0	(0x40FBu)	/* EPFR37_SCK8Eのセット値XX0 */
#define EPFR37_SCK8E_001	(0x12E3u)	/* EPFR37_SCK8Eのセット値001 */

/* <P037> */
#define EPFR15_PPG12E_X0	(0x40BFu)	/* EPFR15_PPG12Eのセット値X0 */
#define EPFR15_PPG12E_01	(0x159Fu)	/* EPFR15_PPG12Eのセット値01 */
#define EPFR18_SEG23E_LOW	(0x407Fu)	/* EPFR18_SEG23Eのセット値LOW */
#define EPFR18_SEG23E_HIGH	(0x2080u)	/* EPFR18_SEG23Eのセット値HIGH */
#define EPFR34_SIN7E_LOW	(0x40FEu)	/* EPFR34_SIN7Eのセット値LOW */

/* <<P04>> */

/* <P040> */
#define EPFR19_SEG24E_LOW	(0x40FEu)	/* EPFR19_SEG24Eのセット値LOW */
#define EPFR19_SEG24E_HIGH	(0x2001u)	/* EPFR19_SEG24Eのセット値HIGH */
#define EPFR34_SOT7E_X0		(0x40F7u)	/* EPFR34_SOT7Eのセット値X0 */
#define EPFR34_SOT7E_X1		(0x2008u)	/* EPFR34_SOT7Eのセット値X1 */
#define EPFR45_PPG13E_X0	(0x40FEu)	/* EPFR45_PPG13Eのセット値X0 */
#define EPFR45_PPG13E_01	(0x10FCu)	/* EPFR45_PPG13Eのセット値01 */

/* <P041> */
#define EPFR19_SEG25E_LOW	(0x40FDu)	/* EPFR19_SEG25Eのセット値LOW */
#define EPFR19_SEG25E_HIGH	(0x2002u)	/* EPFR19_SEG25Eのセット値HIGH */
#define EPFR34_SCK7E_X0		(0x40FDu)	/* EPFR34_SCK7Eのセット値X0 */
#define EPFR34_SCK7E_01		(0x11F9u)	/* EPFR34_SCK7Eのセット値01 */
#define EPFR45_PPG14E_X0	(0x40FBu)	/* EPFR45_PPG14Eのセット値X0 */
#define EPFR45_PPG14E_01	(0x12F3u)	/* EPFR45_PPG14Eのセット値01 */

/* <P042> */
#define EPFR19_SEG26E_LOW	(0x40FBu)	/* EPFR19_SEG26Eのセット値LOW */
#define EPFR19_SEG26E_HIGH	(0x2004u)	/* EPFR19_SEG26Eのセット値HIGH */
#define EPFR40_AIN0E_00		(0x40FCu)	/* EPFR40_AIN0Eのセット値00 */
#define EPFR45_PPG15E_X0	(0x40EFu)	/* EPFR45_PPG15Eのセット値X0 */
#define EPFR45_PPG15E_01	(0x14CFu)	/* EPFR45_PPG15Eのセット値01 */

/* <P043> */
#define EPFR19_SEG27E_LOW	(0x40F7u)	/* EPFR19_SEG27Eのセット値LOW */
#define EPFR19_SEG27E_HIGH	(0x2008u)	/* EPFR19_SEG27Eのセット値HIGH */
#define EPFR30_SGA4E_X0		(0x40EFu)	/* EPFR30_SGA4Eのセット値X0 */
#define EPFR30_SGA4E_01		(0x14CFu)	/* EPFR30_SGA4Eのセット値01 */
#define EPFR31_OCU6E_0X		(0x40DFu)	/* EPFR31_OCU6Eのセット値0X */
#define EPFR31_OCU6E_10		(0x15CFu)	/* EPFR31_OCU6Eのセット値10 */
#define EPFR40_BIN0E_00		(0x40F3u)	/* EPFR40_BIN0Eのセット値00 */

/* <P044> */
#define EPFR19_SEG28E_LOW	(0x40EFu)	/* EPFR19_SEG28Eのセット値LOW */
#define EPFR19_SEG28E_HIGH	(0x2010u)	/* EPFR19_SEG28Eのセット値HIGH */
#define EPFR30_SGO4E_X0		(0x40BFu)	/* EPFR30_SGO4Eのセット値X0 */
#define EPFR30_SGO4E_01		(0x163Fu)	/* EPFR30_SGO4Eのセット値01 */
#define EPFR31_OCU7E_0X		(0x407Fu)	/* EPFR31_OCU7Eのセット値0X */
#define EPFR31_OCU7E_10		(0x173Fu)	/* EPFR31_OCU7Eのセット値10 */
#define EPFR40_ZIN0E_00		(0x40CFu)	/* EPFR40_ZIN0Eのセット値00 */

/* <P045> */
#define EPFR19_SEG29E_LOW	(0x40DFu)	/* EPFR19_SEG29Eのセット値LOW */
#define EPFR19_SEG29E_HIGH	(0x2020u)	/* EPFR19_SEG29Eのセット値HIGH */
#define EPFR37_SIN8E_10		(0x11FCu)	/* EPFR37_SIN8Eのセット値10 */
#define EPFR41_AIN1E_LOW	(0x40FEu)	/* EPFR41_AIN1Eのセット値LOW */

/* <P046> */
#define EPFR19_SEG30E_LOW	(0x40BFu)	/* EPFR19_SEG30Eのセット値LOW */
#define EPFR19_SEG30E_HIGH	(0x2040u)	/* EPFR19_SEG30Eのセット値HIGH */
#define EPFR37_SOT8E_0XX	(0x407Fu)	/* EPFR37_SOT8Eのセット値0XX */
#define EPFR37_SOT8E_100	(0x171Fu)	/* EPFR37_SOT8Eのセット値100 */
#define EPFR41_BIN1E_LOW	(0x40FDu)	/* EPFR41_BIN1Eのセット値LOW */

/* <P047> */
#define EPFR19_SEG31E_LOW	(0x407Fu)	/* EPFR19_SEG31Eのセット値LOW */
#define EPFR19_SEG31E_HIGH	(0x2080u)	/* EPFR19_SEG31Eのセット値HIGH */
#define EPFR37_SCK8E_0XX	(0x40EFu)	/* EPFR37_SCK8Eのセット値0XX */
#define EPFR37_SCK8E_100	(0x14E3u)	/* EPFR37_SCK8Eのセット値100 */
#define EPFR41_ZIN1E_LOW	(0x40FBu)	/* EPFR41_ZIN1Eのセット値LOW */

/* <<P05>> */

/* <P050> */
#define EPFR20_COM0E_LOW	(0x40FEu)	/* EPFR20_COM0Eのセット値LOW */
#define EPFR20_COM0E_HIGH	(0x2001u)	/* EPFR20_COM0Eのセット値HIGH */
#define EPFR32_OCU8E_0X		(0x40FDu)	/* EPFR32_OCU8Eのセット値0X */
#define EPFR32_OCU8E_10		(0x11FCu)	/* EPFR32_OCU8Eのセット値10 */

/* <P051> */
#define EPFR20_COM1E_LOW	(0x40FDu)	/* EPFR20_COM1Eのセット値LOW */
#define EPFR20_COM1E_HIGH	(0x2002u)	/* EPFR20_COM1Eのセット値HIGH */
#define EPFR32_OCU9E_0X		(0x40F7u)	/* EPFR32_OCU9Eのセット値0X */
#define EPFR32_OCU9E_10		(0x13F3u)	/* EPFR32_OCU9Eのセット値10 */

/* <P052> */
#define EPFR20_COM2E_LOW	(0x40FBu)	/* EPFR20_COM2Eのセット値LOW */
#define EPFR20_COM2E_HIGH	(0x2004u)	/* EPFR20_COM2Eのセット値HIGH */
#define EPFR32_OCU10E_0X	(0x40DFu)	/* EPFR32_OCU10Eのセット値0X */
#define EPFR32_OCU10E_10	(0x15CFu)	/* EPFR32_OCU10Eのセット値10 */

/* <P053> */
#define EPFR20_COM3E_LOW	(0x40F7u)	/* EPFR20_COM3Eのセット値LOW */
#define EPFR20_COM3E_HIGH	(0x2008u)	/* EPFR20_COM3Eのセット値HIGH */
#define EPFR32_OCU11E_0X	(0x407Fu)	/* EPFR32_OCU11Eのセット値0X */
#define EPFR32_OCU11E_10	(0x173Fu)	/* EPFR32_OCU11Eのセット値10 */

/* <P054> */
#define EPFR20_V0E_LOW		(0x40EFu)	/* EPFR20_V0Eのセット値LOW */
#define EPFR20_V0E_HIGH		(0x2010u)	/* EPFR20_V0Eのセット値HIGH */
#define EPFR28_FRCK0E_HIGH	(0x2001u)	/* EPFR28_FRCK0Eのセット値HIGH */

/* <P055> */
#define EPFR20_V1E_LOW		(0x40DFu)	/* EPFR20_V1Eのセット値LOW */
#define EPFR20_V1E_HIGH		(0x2020u)	/* EPFR20_V1Eのセット値HIGH */
#define EPFR28_FRCK1E_HIGH	(0x2002u)	/* EPFR28_FRCK1Eのセット値HIGH */

/* <P056> */
#define EPFR20_V2E_LOW		(0x40BFu)	/* EPFR20_V2Eのセット値LOW */
#define EPFR20_V2E_HIGH		(0x2040u)	/* EPFR20_V2Eのセット値HIGH */
#define EPFR28_FRCK2E_HIGH	(0x2004u)	/* EPFR28_FRCK2Eのセット値HIGH */

/* <P057> */
#define EPFR20_V3E_LOW		(0x407Fu)	/* EPFR20_V3Eのセット値LOW */
#define EPFR20_V3E_HIGH		(0x2080u)	/* EPFR20_V3Eのセット値HIGH */
#define EPFR28_FRCK3E_HIGH	(0x2008u)	/* EPFR28_FRCK3Eのセット値HIGH */

/* <<P06>> */

/* <P060> */
#define EPFR21_PWM1P0E_LOW	(0x40FEu)	/* EPFR21_PWM1P0Eのセット値LOW */
#define EPFR21_PWM1P0E_HIGH	(0x2001u)	/* EPFR21_PWM1P0Eのセット値HIGH */

/* <P061> */
#define EPFR21_PWM1M0E_LOW	(0x40FDu)	/* EPFR21_PWM1M0Eのセット値LOW */
#define EPFR21_PWM1M0E_HIGH	(0x2002u)	/* EPFR21_PWM1M0Eのセット値HIGH */
#define EPFR36_SIN1E_HIGH	(0x2001u)	/* EPFR36_SIN1Eのセット値HIGH */

/* <P062> */
#define EPFR21_PWM2P0E_LOW	(0x40FBu)	/* EPFR21_PWM2P0Eのセット値LOW */
#define EPFR21_PWM2P0E_HIGH	(0x2004u)	/* EPFR21_PWM2P0Eのセット値HIGH */
#define EPFR36_SOT1E_0X		(0x40EFu)	/* EPFR36_SOT1Eのセット値0X */
#define EPFR36_SOT1E_10		(0x14E7u)	/* EPFR36_SOT1Eのセット値10 */
#define EPFR41_ZIN1E_HIGH	(0x2004u)	/* EPFR41_ZIN1Eのセット値HIGH */

/* <P063> */
#define EPFR21_PWM2M0E_LOW	(0x40F7u)	/* EPFR21_PWM2M0Eのセット値LOW */
#define EPFR21_PWM2M0E_HIGH	(0x2008u)	/* EPFR21_PWM2M0Eのセット値HIGH */
#define EPFR36_SCK1E_0X		(0x40FBu)	/* EPFR36_SCK1Eのセット値0X */
#define EPFR36_SCK1E_10		(0x12F9u)	/* EPFR36_SCK1Eのセット値10 */
#define EPFR41_BIN1E_HIGH	(0x2002u)	/* EPFR41_BIN1Eのセット値HIGH */

/* <P064> */
#define EPFR21_PWM1P1E_LOW	(0x40EFu)	/* EPFR21_PWM1P1Eのセット値LOW */
#define EPFR21_PWM1P1E_HIGH	(0x2010u)	/* EPFR21_PWM1P1Eのセット値HIGH */
#define EPFR35_SIN0E_HIGH	(0x2001u)	/* EPFR35_SIN0Eのセット値HIGH */
#define EPFR41_AIN1E_HIGH	(0x2001u)	/* EPFR41_AIN1Eのセット値HIGH */

/* <P65> */
#define EPFR21_PWM1M1E_LOW	(0x40DFu)	/* EPFR21_PWM1M1Eのセット値LOW */
#define EPFR21_PWM1M1E_HIGH	(0x2020u)	/* EPFR21_PWM1M1Eのセット値HIGH */
#define EPFR35_SOT0E_0X		(0x40EFu)	/* EPFR35_SOT0Eのセット値0X */
#define EPFR35_SOT0E_10		(0x14E7u)	/* EPFR35_SOT0Eのセット値10 */
#define EPFR40_ZIN0E_01		(0x14CFu)	/* EPFR40_ZIN0Eのセット値01 */

/* <P66> */
#define EPFR21_PWM2P1E_LOW	(0x40BFu)	/* EPFR21_PWM2P1Eのセット値LOW */
#define EPFR21_PWM2P1E_HIGH	(0x2040u)	/* EPFR21_PWM2P1Eのセット値HIGH */
#define EPFR35_SCK0E_0X		(0x40FBu)	/* EPFR35_SCK0Eのセット値0X */
#define EPFR35_SCK0E_10		(0x12F9u)	/* EPFR35_SCK0Eのセット値10 */
#define EPFR40_BIN0E_01		(0x12F3u)	/* EPFR40_BIN0Eのセット値01 */

/* <P67> */
#define EPFR21_PWM2M1E_LOW	(0x407Fu)	/* EPFR21_PWM2M1Eのセット値LOW */
#define EPFR21_PWM2M1E_HIGH	(0x2080u)	/* EPFR21_PWM2M1Eのセット値HIGH */
#define EPFR38_SIN9E_HIGH	(0x2001u)	/* EPFR38_SIN9Eのセット値HIGH */
#define EPFR40_AIN0E_01		(0x10FCu)	/* EPFR40_AIN0Eのセット値01 */

/* <<P07>> */

/* <P70> */
#define EPFR22_PWM1P2E_LOW	(0x40FEu)	/* EPFR22_PWM1P2Eのセット値LOW */
#define EPFR22_PWM1P2E_HIGH	(0x2001u)	/* EPFR22_PWM1P2Eのセット値HIGH */
#define EPFR38_SOT9E_0X		(0x40EFu)	/* EPFR38_SOT9Eのセット値0X */
#define EPFR38_SOT9E_10		(0x14E7u)	/* EPFR38_SOT9Eのセット値10 */

/* <P71> */
#define EPFR22_PWM1M2E_LOW	(0x40FDu)	/* EPFR22_PWM1M2Eのセット値LOW */
#define EPFR22_PWM1M2E_HIGH	(0x2002u)	/* EPFR22_PWM1M2Eのセット値HIGH */
#define EPFR38_SCK9E_0X		(0x40FBu)	/* EPFR38_SCK9Eのセット値0X */
#define EPFR38_SCK9E_10		(0x12F9u)	/* EPFR38_SCK9Eのセット値10 */

/* <P72> */
#define EPFR22_PWM2P2E_LOW	(0x40FBu)	/* EPFR22_PWM2P2Eのセット値LOW */
#define EPFR22_PWM2P2E_HIGH	(0x2004u)	/* EPFR22_PWM2P2Eのセット値HIGH */
#define EPFR37_SIN8E_01		(0x10FCu)	/* EPFR37_SIN8Eのセット値01 */
#define EPFR39_ICU11E_01	(0x163Fu)	/* EPFR39_ICU11Eのセット値01 */

/* <P73> */
#define EPFR22_PWM2M2E_LOW	(0x40F7u)	/* EPFR22_PWM2M2Eのセット値LOW */
#define EPFR22_PWM2M2E_HIGH	(0x2008u)	/* EPFR22_PWM2M2Eのセット値HIGH */
#define EPFR37_SOT8E_X0X	(0x40BFu)	/* EPFR37_SOT8Eのセット値X0X */
#define EPFR37_SOT8E_010	(0x161Fu)	/* EPFR37_SOT8Eのセット値010 */
#define EPFR39_ICU10E_01	(0x14CFu)	/* EPFR39_ICU10Eのセット値01 */

/* <P74> */
#define EPFR15_PPG12E_0X	(0x40BFu)	/* EPFR15_PPG12Eのセット値0X */
#define EPFR15_PPG12E_1X	(0x2040u)	/* EPFR15_PPG12Eのセット値1X */
#define EPFR22_PWM1P3E_LOW	(0x40EFu)	/* EPFR22_PWM1P3Eのセット値LOW */
#define EPFR22_PWM1P3E_HIGH	(0x2010u)	/* EPFR22_PWM1P3Eのセット値HIGH */
#define EPFR37_SCK8E_X0X	(0x40F7u)	/* EPFR37_SCK8Eのセット値X0X */
#define EPFR37_SCK8E_010	(0x13E3u)	/*  EPFR37_SCK8Eのセット値010 */
#define EPFR39_ICU9E_01		(0x12F3u)	/* EPFR39_ICU9Eのセット値01 */

/* <P75> */
#define EPFR22_PWM1M3E_LOW	(0x40DFu)	/* EPFR22_PWM1M3Eのセット値LOW */
#define EPFR22_PWM1M3E_HIGH	(0x2020u)	/* EPFR22_PWM1M3Eのセット値HIGH */
#define EPFR34_SIN7E_HIGH	(0x2001u)	/* EPFR34_SIN7Eのセット値HIGH */
#define EPFR39_ICU8E_01		(0x10FCu)	/* EPFR39_ICU8Eのセット値01 */
#define EPFR45_PPG13E_0X	(0x40FDu)	/* EPFR45_PPG13Eのセット値0X */
#define EPFR45_PPG13E_1X	(0x2002u)	/* EPFR45_PPG13Eのセット値1X */

/* <P76> */
#define EPFR01_ICU7E_01		(0x163Fu)	/* EPFR01_ICU7Eのセット値01 */
#define EPFR22_PWM2P3E_LOW	(0x40BFu)	/* EPFR22_PWM2P3Eのセット値LOW */
#define EPFR22_PWM2P3E_HIGH	(0x2040u)	/* EPFR22_PWM2P3Eのセット値HIGH */
#define EPFR34_SOT7E_0X		(0x40EFu)	/* EPFR34_SOT7Eのセット値0X */
#define EPFR34_SOT7E_1X		(0x2010u)	/* EPFR34_SOT7Eのセット値1X */
#define EPFR45_PPG14E_0X	(0x40F7u)	/* EPFR45_PPG14Eのセット値0X */
#define EPFR45_PPG14E_1X	(0x2008u)	/* EPFR45_PPG14Eのセット値1X */

/* <P77> */
#define EPFR01_ICU6E_01		(0x14CFu)	/* EPFR01_ICU6Eのセット値01 */
#define EPFR22_PWM2M3E_LOW	(0x407Fu)	/* EPFR22_PWM2M3Eのセット値LOW */
#define EPFR22_PWM2M3E_HIGH	(0x2080u)	/* EPFR22_PWM2M3Eのセット値HIGH */
#define EPFR34_SCK7E_0X		(0x40FBu)	/* EPFR34_SCK7Eのセット値0X */
#define EPFR34_SCK7E_10		(0x12F9u)	/* EPFR34_SCK7Eのセット値10 */
#define EPFR45_PPG15E_0X	(0x40DFu)	/* EPFR45_PPG15Eのセット値0X */
#define EPFR45_PPG15E_1X	(0x2020u)	/* EPFR45_PPG15Eのセット値1X */

/* <P08>> */

/* <P80> */
#define EPFR23_PWM1P4E_LOW	(0x40FEu)	/* EPFR23_PWM1P4Eのセット値LOW */
#define EPFR23_PWM1P4E_HIGH	(0x2001u)	/* EPFR23_PWM1P4Eのセット値HIGH */
#define EPFR33_SIN6E_LOW	(0x40FEu)	/* EPFR33_SIN6Eのセット値LOW */
#define EPFR40_AIN0E_10		(0x11FCu)	/* EPFR40_AIN0Eのセット値10 */
#define EPFR45_PPG16E_LOW	(0x40BFu)	/* EPFR45_PPG16Eのセット値LOW */
#define EPFR45_PPG16E_HIGH	(0x2040u)	/* EPFR45_PPG16Eのセット値HIGH */

/* <P81> */
#define EPFR23_PWM1M4E_LOW	(0x40FDu)	/* EPFR23_PWM1M4Eのセット値LOW */
#define EPFR23_PWM1M4E_HIGH	(0x2002u)	/* EPFR23_PWM1M4Eのセット値HIGH */
#define EPFR33_SOT6E_X0		(0x40F7u)	/* EPFR33_SOT6Eのセット値X0 */
#define EPFR33_SOT6E_X1		(0x2008u)	/* EPFR33_SOT6Eのセット値X1 */
#define EPFR40_BIN0E_10		(0x13F3u)	/* EPFR40_BIN0Eのセット値10 */
#define EPFR45_PPG17E_LOW	(0x407Fu)	/* EPFR45_PPG17Eのセット値LOW */
#define EPFR45_PPG17E_HIGH	(0x2080u)	/* EPFR45_PPG17Eのセット値HIGH */

/* <P82> */
#define EPFR23_PWM2P4E_LOW	(0x40FBu)	/* EPFR23_PWM2P4Eのセット値LOW */
#define EPFR23_PWM2P4E_HIGH	(0x2004u)	/* EPFR23_PWM2P4Eのセット値HIGH */
#define EPFR33_SCK6E_X0		(0x40FDu)	/* EPFR33_SCK6Eのセット値X0 */
#define EPFR33_SCK6E_01		(0x11F9u)	/* EPFR33_SCK6Eのセット値01 */
#define EPFR40_ZIN0E_10		(0x15CFu)	/* EPFR40_ZIN0Eのセット値10 */
#define EPFR46_PPG18E_LOW	(0x40FEu)	/* EPFR46_PPG18Eのセット値LOW */
#define EPFR46_PPG18E_HIGH	(0x2001u)	/* EPFR46_PPG18Eのセット値HIGH */

/* <P83> */
#define EPFR00_ICU0E_10		(0x11FCu)	/* EPFR00_ICU0Eのセット値10 */
#define EPFR23_PWM2M4E_LOW	(0x40F7u)	/* EPFR23_PWM2M4Eのセット値LOW */
#define EPFR23_PWM2M4E_HIGH	(0x2008u)	/* EPFR23_PWM2M4Eのセット値HIGH */
#define EPFR46_PPG19E_LOW	(0x40FDu)	/* EPFR46_PPG19Eのセット値LOW */
#define EPFR46_PPG19E_HIGH	(0x2002u)	/* EPFR46_PPG19Eのセット値HIGH */

/* <P84> */
#define EPFR00_ICU1E_10		(0x13F3u)	/* EPFR00_ICU1Eのセット値10 */
#define EPFR23_PWM1P5E_LOW	(0x40EFu)	/* EPFR23_PWM1P5Eのセット値LOW */
#define EPFR23_PWM1P5E_HIGH	(0x2010u)	/* EPFR23_PWM1P5Eのセット値HIGH */
#define EPFR46_PPG20E_LOW	(0x40FBu)	/* EPFR46_PPG20Eのセット値LOW */
#define EPFR46_PPG20E_HIGH	(0x2004u)	/* EPFR46_PPG20Eのセット値HIGH */

/* <P85> */
#define EPFR00_ICU2E_10		(0x15CFu)	/* EPFR00_ICU2Eのセット値10 */
#define EPFR23_PWM1M5E_LOW	(0x40DFu)	/* EPFR23_PWM1M5Eのセット値LOW */
#define EPFR23_PWM1M5E_HIGH	(0x2020u)	/* EPFR23_PWM1M5Eのセット値HIGH */
#define EPFR46_PPG21E_LOW	(0x40F7u)	/* EPFR46_PPG21Eのセット値LOW */
#define EPFR46_PPG21E_HIGH	(0x2008u)	/* EPFR46_PPG21Eのセット値HIGH */

/* <P86> */
#define EPFR00_ICU3E_10		(0x173Fu)	/* EPFR00_ICU3Eのセット値10 */
#define EPFR23_PWM2P5E_LOW	(0x40BFu)	/* EPFR23_PWM2P5Eのセット値LOW */
#define EPFR23_PWM2P5E_HIGH	(0x2040u)	/* EPFR23_PWM2P5Eのセット値HIGH */
#define EPFR46_PPG22E_LOW	(0x40EFu)	/* EPFR46_PPG22Eのセット値LOW */
#define EPFR46_PPG22E_HIGH	(0x2010u)	/* EPFR46_PPG22Eのセット値HIGH */

/* <P87> */
#define EPFR01_ICU4E_10		(0x11FCu)	/* EPFR01_ICU4Eのセット値10 */
#define EPFR23_PWM2M5E_LOW	(0x407Fu)	/* EPFR23_PWM2M5Eのセット値LOW */
#define EPFR23_PWM2M5E_HIGH	(0x2080u)	/* EPFR23_PWM2M5Eのセット値HIGH */
#define EPFR46_PPG23E_LOW	(0x40DFu)	/* EPFR46_PPG23Eのセット値LOW */
#define EPFR46_PPG23E_HIGH	(0x2020u)	/* EPFR46_PPG23Eのセット値HIGH */

/* <<P09>> */

/* <P90> */
#define EPFR10_PPG0E_0XX	(0x40FBu)	/* EPFR10_PPG0Eのセット値0XX */
#define EPFR10_PPG0E_1XX	(0x2004u)	/* EPFR10_PPG0Eのセット値1XX */

/* <P91> */
#define EPFR00_ICU2E_01		(0x14CFu)	/* EPFR00_ICU2Eのセット値01 */
#define EPFR04_TOT2E_X0X	(0x40F7u)	/* EPFR04_TOT2Eのセット値X0X */
#define EPFR04_TOT2E_X1X	(0x2008u)	/* EPFR04_TOT2Eのセット値X1X */
#define EPFR06_SIN2E_LOW	(0x40FEu)	/* EPFR06_SIN2Eのセット値LOW */
#define EPFR13_PPG6E_X0X	(0x40FDu)	/* EPFR13_PPG6Eのセット値X0X */
#define EPFR13_PPG6E_X1X	(0x2002u)	/* EPFR13_PPG6Eのセット値X1X */
#define EPFR27_SGA0E_LOW	(0x40FEu)	/* EPFR27_SGA0Eのセット値LOW */
#define EPFR27_SGA0E_HIGH	(0x2001u)	/* EPFR27_SGA0Eのセット値HIGH */
#define EPFR44_INT12E_LOW	(0x40EFu)	/* EPFR44_INT12Eのセット値LOW */

/* <P92> */
#define EPFR00_ICU0E_01		(0x10FCu)	/* EPFR00_ICU0Eのセット値01 */
#define EPFR05_TOT3E_X0X	(0x40F7u)	/* EPFR05_TOT3Eのセット値X0X */
#define EPFR05_TOT3E_X1X	(0x2008u)	/* EPFR05_TOT3Eのセット値X1X */
#define EPFR06_SCK2E_X0		(0x40FDu)	/* EPFR06_SCK2Eのセット値X0 */
#define EPFR06_SCK2E_01		(0x11F9u)	/* EPFR06_SCK2Eのセット値01 */
#define EPFR13_PPG7E_X0X	(0x40EFu)	/* EPFR13_PPG7Eのセット値X0X */
#define EPFR13_PPG7E_X1X	(0x2010u)	/* EPFR13_PPG7Eのセット値X1X */
#define EPFR27_SGO0E_LOW	(0x40FDu)	/* EPFR27_SGO0Eのセット値LOW */
#define EPFR27_SGO0E_HIGH	(0x2002u)	/* EPFR27_SGO0Eのセット値HIGH */
#define EPFR44_INT13E_LOW	(0x40DFu)	/* EPFR44_INT13Eのセット値LOW */

/* <P93> */
#define EPFR00_ICU3E_01		(0x163Fu)	/* EPFR00_ICU3Eのセット値01 */
#define EPFR06_SOT2E_X0		(0x40F7u)	/* EPFR06_SOT2Eのセット値X0 */
#define EPFR06_SOT2E_X1		(0x2008u)	/* EPFR06_SOT2Eのセット値X1 */
#define EPFR14_PPG8E_X0X	(0x40FDu)	/* EPFR14_PPG8Eのセット値X0X */
#define EPFR14_PPG8E_X1X	(0x2002u)	/* EPFR14_PPG8Eのセット値X1X */
#define EPFR27_SGA1E_LOW	(0x40FBu)	/* EPFR27_SGA1Eのセット値LOW */
#define EPFR27_SGA1E_HIGH	(0x2004u)	/* EPFR27_SGA1Eのセット値HIGH */
#define EPFR44_INT14E_LOW	(0x40BFu)	/* EPFR44_INT14Eのセット値LOW */

/* <P94> */
#define EPFR00_ICU1E_01		(0x12F3u)	/* EPFR00_ICU1Eのセット値01 */
#define EPFR07_SIN3E_LOW	(0x40FEu)	/* EPFR07_SIN3Eのセット値LOW */
#define EPFR14_PPG9E_X0X	(0x40EFu)	/* EPFR14_PPG9Eのセット値X0X */
#define EPFR14_PPG9E_X1X	(0x2010u)	/* EPFR14_PPG9Eのセット値X1X */
#define EPFR27_SGO1E_LOW	(0x40F7u)	/* EPFR27_SGO1Eのセット値LOW */
#define EPFR27_SGO1E_HIGH	(0x2008u)	/* EPFR27_SGO1Eのセット値HIGH */
#define EPFR44_INT15E_LOW	(0x407Fu)	/* EPFR44_INT15Eのセット値LOW */

/* <P95> */
#define EPFR15_PPG10E_X0X	(0x40FDu)	/* EPFR15_PPG10Eのセット値X0X */
#define EPFR15_PPG10E_X1X	(0x2002u)	/* EPFR15_PPG10Eのセット値X1X */
#define EPFR24_TX0E_LOW		(0x40FEu)	/* EPFR24_TX0Eのセット値LOW */
#define EPFR24_TX0E_HIGH	(0x2001u)	/* EPFR24_TX0Eのセット値HIGH */

/* <P96> */
#define EPFR44_INT9E_LOW	(0x40FDu)	/* EPFR44_INT9Eのセット値LOW */

/* <P97> */
#define EPFR01_ICU4E_01		(0x10FCu)	/* EPFR01_ICU4Eのセット値01 */
#define EPFR02_TIN0E_00		(0x40FCu)	/* EPFR02_TIN0Eのセット値00 */
#define EPFR07_SOT3E_X0		(0x40F7u)	/* EPFR07_SOT3Eのセット値X0 */
#define EPFR07_SOT3E_X1		(0x2008u)	/* EPFR07_SOT3Eのセット値X1 */
#define EPFR10_PPG0E_X0X	(0x40FDu)	/* EPFR10_PPG0Eのセット値X0X */
#define EPFR10_PPG0E_X1X	(0x2002u)	/* EPFR10_PPG0Eのセット値X1X */
#define EPFR27_WOTE_LOW		(0x40EFu)	/* EPFR27_WOTEのセット値LOW */
#define EPFR27_WOTE_HIGH	(0x2010u)	/* EPFR27_WOTEのセット値HIGH */
#define EPFR44_INT8E_LOW	(0x40FEu)	/* EPFR44_INT8Eのセット値LOW */

/* <<P10>> */

/* <P100> */
#define EPFR02_TIN0E_01		(0x10FCu)	/* EPFR02_TIN0Eのセット値01 */
#define EPFR08_SIN4E_HIGH	(0x2001u)	/* EPFR08_SIN4Eのセット値HIGH */
#define EPFR14_PPG8E_XX0	(0x40FEu)	/* EPFR14_PPG8Eのセット値XX0 */
#define EPFR14_PPG8E_XX1	(0x2001u)	/* EPFR14_PPG8Eのセット値XX1 */

/* <P101> */
#define EPFR03_TIN1E_01		(0x10FCu)	/* EPFR03_TIN1Eのセット値01 */
#define EPFR08_SOT4E_0X		(0x40EFu)	/* EPFR08_SOT4Eのセット値0X */
#define EPFR08_SOT4E_1X		(0x2010u)	/* EPFR08_SOT4Eのセット値1X */
#define EPFR14_PPG9E_XX0	(0x40F7u)	/* EPFR14_PPG9Eのセット値XX0 */
#define EPFR14_PPG9E_XX1	(0x2008u)	/* EPFR14_PPG9Eのセット値XX1 */

/* <P102> */
#define EPFR01_ICU6E_10		(0x15CFu)	/* EPFR01_ICU6Eのセット値10 */
#define EPFR04_TIN2E_01		(0x10FCu)	/* EPFR04_TIN2Eのセット値01 */
#define EPFR08_SCK4E_0X		(0x40FBu)	/* EPFR08_SCK4Eのセット値0X */
#define EPFR08_SCK4E_10		(0x12F9u)	/* EPFR08_SCK4Eのセット値10 */
#define EPFR15_PPG10E_XX0	(0x40FEu)	/* EPFR15_PPG10Eのセット値XX0 */
#define EPFR15_PPG10E_XX1	(0x2001u)	/* EPFR15_PPG10Eのセット値XX1 */

/* <P103> */
#define EPFR01_ICU7E_10		(0x173Fu)	/* EPFR01_ICU7Eのセット値10 */
#define EPFR05_TIN3E_01		(0x10FCu)	/* EPFR05_TIN3Eのセット値01 */
#define EPFR09_SIN5E_HIGH	(0x2001u)	/* EPFR09_SIN5Eのセット値HIGH */
#define EPFR10_PPG1E_XX0X	(0x40EFu)	/* EPFR10_PPG1Eのセット値XX0X */
#define EPFR10_PPG1E_XX1X	(0x2010u)	/* EPFR10_PPG1Eのセット値XX1X */

/* <P104> */
#define EPFR02_TOT0E_X0X	(0x40F7u)	/* EPFR02_TOT0Eのセット値X0X */
#define EPFR02_TOT0E_X1X	(0x2008u)	/* EPFR02_TOT0Eのセット値X1X */
#define EPFR09_SOT5E_0X		(0x40EFu)	/* EPFR09_SOT5Eのセット値0X */
#define EPFR09_SOT5E_1X		(0x2010u)	/* EPFR09_SOT5Eのセット値1X */
#define EPFR11_PPG2E_X0X	(0x40FDu)	/* EPFR11_PPG2Eのセット値X0X */
#define EPFR11_PPG2E_X1X	(0x2002u)	/* EPFR11_PPG2Eのセット値X1X */
#define EPFR39_ICU8E_10		(0x11FCu)	/* EPFR39_ICU8Eのセット値10 */

/* <P105> */
#define EPFR03_TOT1E_X0X	(0x40F7u)	/* EPFR03_TOT1Eのセット値X0X */
#define EPFR03_TOT1E_X1X	(0x2008u)	/* EPFR03_TOT1Eのセット値X1X */
#define EPFR09_SCK5E_0X		(0x40FBu)	/* EPFR09_SCK5Eのセット値0X */
#define EPFR09_SCK5E_10		(0x12F9u)	/* EPFR09_SCK5Eのセット値10 */
#define EPFR11_PPG3E_X0X	(0x40EFu)	/* EPFR11_PPG3Eのセット値X0X */
#define EPFR11_PPG3E_X1X	(0x2010u)	/* EPFR11_PPG3Eのセット値X1X */
#define EPFR39_ICU9E_10		(0x13F3u)	/* EPFR39_ICU9Eのセット値10 */

/* <P106> */
#define EPFR12_PPG4E_X0X	(0x40FDu)	/* EPFR12_PPG4Eのセット値X0X */
#define EPFR12_PPG4E_X1X	(0x2002u)	/* EPFR12_PPG4Eのセット値X1X */
#define EPFR30_SGA4E_0X		(0x40DFu)	/* EPFR30_SGA4Eのセット値0X */
#define EPFR30_SGA4E_10		(0x15CFu)	/* EPFR30_SGA4Eのセット値10 */
#define EPFR39_ICU10E_10	(0x15CFu)	/* EPFR39_ICU10Eのセット値10 */

/* <P107> */
#define EPFR12_PPG5E_X0X	(0x40EFu)	/* EPFR12_PPG5Eのセット値X0X */
#define EPFR12_PPG5E_X1X	(0x2010u)	/* EPFR12_PPG5Eのセット値X1X */
#define EPFR30_SGO4E_0X		(0x407Fu)	/* EPFR30_SGO4Eのセット値0X */
#define EPFR30_SGO4E_10		(0x173Fu)	/* EPFR30_SGO4Eのセット値10 */
#define EPFR39_ICU11E_10	(0x173Fu)	/* EPFR39_ICU11Eのセット値10 */
#define EPFR42_DAS1_LOW		(0x40FDu)	/* EPFR42_DAS1のセット値LOW */
#define EPFR42_DAS1_HIGH	(0x2002u)	/* EPFR42_DAS1のセット値HIGH */

/* <<P11>> */

/* <P110> */
#define EPFR10_PPG1E_X0XX	(0x40DFu)	/* EPFR10_PPG1Eのセット値X0XX */
#define EPFR10_PPG1E_X1XX	(0x2020u)	/* EPFR10_PPG1Eのセット値X1XX */
#define EPFR24_TX1E_LOW		(0x40FDu)	/* EPFR24_TX1Eのセット値LOW */
#define EPFR24_TX1E_HIGH	(0x2002u)	/* EPFR24_TX1Eのセット値HIGH */

/* <P111> */
#define EPFR11_PPG2E_0XX	(0x40FBu)	/* EPFR11_PPG2Eのセット値0XX */
#define EPFR11_PPG2E_1XX	(0x2004u)	/* EPFR11_PPG2Eのセット値1XX */
#define EPFR44_INT10E_LOW	(0x40FBu)	/* EPFR44_INT10Eのセット値LOW */

/* <P112> */
#define EPFR11_PPG3E_0XX	(0x40DFu)	/* EPFR11_PPG3Eのセット値0XX */
#define EPFR11_PPG3E_1XX	(0x2020u)	/* EPFR11_PPG3Eのセット値1XX */
#define EPFR24_TX2E_LOW		(0x40FBu)	/* EPFR24_TX2Eのセット値LOW */
#define EPFR24_TX2E_HIGH	(0x2004u)	/* EPFR24_TX2Eのセット値HIGH */

/* <P113> */
#define EPFR12_PPG4E_0XX	(0x40FBu)	/* EPFR12_PPG4Eのセット値0XX */
#define EPFR12_PPG4E_1XX	(0x2004u)	/* EPFR12_PPG4Eのセット値1XX */
#define EPFR44_INT11E_LOW	(0x40F7u)	/* EPFR44_INT11Eのセット値LOW */

/* <P114> */
#define EPFR01_ICU5E_01		(0x12F3u)	/* EPFR01_ICU5Eのセット値01 */
#define EPFR03_TIN1E_00		(0x40FCu)	/* EPFR03_TIN1Eのセット値00 */
#define EPFR07_SCK3E_X0		(0x40FDu)	/* EPFR07_SCK3Eのセット値X0 */
#define EPFR07_SCK3E_01		(0x11F9u)	/* EPFR07_SCK3Eのセット値01 */
#define EPFR30_SGA2E_LOW	(0x40FEu)	/* EPFR30_SGA2Eのセット値LOW */
#define EPFR30_SGA2E_HIGH	(0x2001u)	/* EPFR30_SGA2Eのセット値HIGH */

/* <P115> */
#define EPFR04_TIN2E_00		(0x40FCu)	/* EPFR04_TIN2Eのセット値00 */
#define EPFR08_SIN4E_LOW	(0x40FEu)	/* EPFR08_SIN4Eのセット値LOW */
#define EPFR30_SGO2E_LOW	(0x40FDu)	/* EPFR30_SGO2Eのセット値LOW */
#define EPFR30_SGO2E_HIGH	(0x2002u)	/* EPFR30_SGO2Eのセット値HIGH */

/* <P116> */
#define EPFR05_TIN3E_00		(0x40FCu)	/* EPFR05_TIN3Eのセット値00 */
#define EPFR08_SOT4E_X0		(0x40F7u)	/* EPFR08_SOT4Eのセット値X0 */
#define EPFR08_SOT4E_X1		(0x2008u)	/* EPFR08_SOT4Eのセット値X1 */
#define EPFR28_FRCK3E_LOW	(0x40F7u)	/* EPFR28_FRCK3Eのセット値LOW */
#define EPFR30_SGA3E_LOW	(0x40FBu)	/* EPFR30_SGA3Eのセット値LOW */
#define EPFR30_SGA3E_HIGH	(0x2004u)	/* EPFR30_SGA3Eのセット値HIGH */

/* <P117> */
#define EPFR02_TOT0E_XX0	(0x40FBu)	/* EPFR02_TOT0Eのセット値XX0 */
#define EPFR02_TOT0E_XX1	(0x2004u)	/* EPFR02_TOT0Eのセット値XX1 */
#define EPFR08_SCK4E_X0		(0x40FDu)	/* EPFR08_SCK4Eのセット値X0 */
#define EPFR08_SCK4E_01		(0x11F9u)	/* EPFR08_SCK4Eのセット値01 */
#define EPFR28_FRCK2E_LOW	(0x40FBu)	/* EPFR28_FRCK2Eのセット値LOW */
#define EPFR30_SGO3E_LOW	(0x40F7u)	/* EPFR30_SGO3Eのセット値LOW */
#define EPFR30_SGO3E_HIGH	(0x2008u)	/* EPFR30_SGO3Eのセット値HIGH */

/* <<P12>> */

/* <P120> */
#define EPFR03_TOT1E_XX0	(0x40FBu)	/* EPFR03_TOT1Eのセット値XX0 */
#define EPFR03_TOT1E_XX1	(0x2004u)	/* EPFR03_TOT1Eのセット値XX1 */
#define EPFR09_SIN5E_LOW	(0x40FEu)	/* EPFR09_SIN5Eのセット値LOW */
#define EPFR12_PPG5E_0XX	(0x40DFu)	/* EPFR12_PPG5Eのセット値0XX */
#define EPFR12_PPG5E_1XX	(0x2020u)	/* EPFR12_PPG5Eのセット値1XX */
#define EPFR28_FRCK1E_LOW	(0x40FDu)	/* EPFR28_FRCK1Eのセット値LOW */
#define EPFR43_INT6E_LOW	(0x40BFu)	/* EPFR43_INT6Eのセット値LOW */

/* <P121> */
#define EPFR04_TOT2E_XX0	(0x40FBu)	/* EPFR04_TOT2Eのセット値XX0 */
#define EPFR04_TOT2E_XX1	(0x2004u)	/* EPFR04_TOT2Eのセット値XX1 */
#define EPFR09_SOT5E_X0		(0x40F7u)	/* EPFR09_SOT5Eのセット値X0 */
#define EPFR09_SOT5E_X1		(0x2008u)	/* EPFR09_SOT5Eのセット値X1 */
#define EPFR13_PPG6E_0XX	(0x40FBu)	/* EPFR13_PPG6Eのセット値0XX */
#define EPFR13_PPG6E_1XX	(0x2004u)	/* EPFR13_PPG6Eのセット値1XX */
#define EPFR28_FRCK0E_LOW	(0x40FEu)	/* EPFR28_FRCK0Eのセット値LOW */
#define EPFR43_INT7E_LOW	(0x407Fu)	/* EPFR43_INT7Eのセット値LOW */

/* <P122> */
#define EPFR05_TOT3E_XX0	(0x40FBu)	/* EPFR05_TOT3Eのセット値XX0 */
#define EPFR05_TOT3E_XX1	(0x2004u)	/* EPFR05_TOT3Eのセット値XX1 */
#define EPFR09_SCK5E_X0		(0x40FDu)	/* EPFR09_SCK5Eのセット値X0 */
#define EPFR09_SCK5E_01		(0x11F9u)	/* EPFR09_SCK5Eのセット値01 */
#define EPFR13_PPG7E_0XX	(0x40DFu)	/* EPFR13_PPG7Eのセット値0XX */
#define EPFR13_PPG7E_1XX	(0x2020u)	/* EPFR13_PPG7Eのセット値1XX */
#define EPFR29_OCU0E_X0		(0x40FBu)	/* EPFR29_OCU0Eのセット値X0 */
#define EPFR29_OCU0E_01		(0x10FCu)	/* EPFR29_OCU0Eのセット値01 */

/* <P123> */
#define EPFR14_PPG8E_0XX	(0x40FBu)	/* EPFR14_PPG8Eのセット値0XX */
#define EPFR14_PPG8E_1XX	(0x2004u)	/* EPFR14_PPG8Eのセット値1XX */
#define EPFR29_OCU1E_X0		(0x40FBu)	/* EPFR29_OCU1Eのセット値X0 */
#define EPFR29_OCU1E_01		(0x12F3u)	/* EPFR29_OCU1Eのセット値01 */
#define EPFR42_DAS0_LOW		(0x40FEu)	/* EPFR42_DAS0のセット値LOW */
#define EPFR42_DAS0_HIGH	(0x2001u)	/* EPFR42_DAS0のセット値HIGH */

/* <P124> */
#define EPFR01_ICU5E_10		(0x13F3u)	/* EPFR01_ICU5Eのセット値10 */
#define EPFR14_PPG9E_0XX	(0x40DFu)	/* EPFR14_PPG9Eのセット値0XX */
#define EPFR14_PPG9E_1XX	(0x2020u)	/* EPFR14_PPG9Eのセット値1XX */
#define EPFR29_OCU2E_X0		(0x40EFu)	/* EPFR31_OCU5Eのセット値X0 */
#define EPFR29_OCU2E_01		(0x14CFu)	/* EPFR31_OCU5Eのセット値01 */

/* <P125> */
#define EPFR00_ICU0E_00		(0x40FCu)	/* EPFR00_ICU0Eのセット値00 */
#define EPFR15_PPG10E_0XX	(0x40FBu)	/* EPFR15_PPG10Eのセット値0XX */
#define EPFR15_PPG10E_1XX	(0x2004u)	/* EPFR15_PPG10Eのセット値1XX */
#define EPFR29_OCU3E_X0		(0x40BFu)	/* EPFR29_OCU3Eのセット値X0 */
#define EPFR29_OCU3E_01		(0x163Fu)	/* EPFR29_OCU3Eのセット値01 */

/* <P126> */
#define EPFR31_OCU4E_X0		(0x40FEu)	/* EPFR31_OCU4Eのセット値X0 */
#define EPFR31_OCU4E_01		(0x10FCu)	/* EPFR31_OCU4Eのセット値01 */
#define EPFR35_SIN0E_LOW	(0x40FEu)	/* EPFR35_SIN0Eのセット値LOW */
#define EPFR43_INT1E_LOW	(0x40FDu)	/* EPFR43_INT1Eのセット値LOW */

/* <P127> */
#define EPFR31_OCU5E_X0		(0x40FBu)	/* EPFR31_OCU5Eのセット値X0 */
#define EPFR31_OCU5E_01		(0x12F3u)	/* EPFR31_OCU5Eのセット値01 */
#define EPFR35_SOT0E_X0		(0x40E7u)	/* EPFR35_SOT0Eのセット値X0 */
#define EPFR35_SOT0E_01		(0x13E7u)	/* EPFR35_SOT0Eのセット値01 */

/* <<P13>> */

/* <P130> */
#define EPFR00_ICU1E_00		(0x40FCu)	/* EPFR00_ICU1Eのセット値00 */
#define EPFR26_TIA0E_LOW	(0x40FEu)	/* EPFR26_TIA0Eのセット値LOW */
#define EPFR26_TIA0E_HIGH	(0x2001u)	/* EPFR26_TIA0Eのセット値HIGH */
#define EPFR35_SCK0E_X0		(0x40FDu)	/* EPFR35_SCK0Eのセット値X0 */
#define EPFR35_SCK0E_01		(0x11F9u)	/* EPFR35_SCK0Eのセット値01 */
#define EPFR43_INT0E_LOW	(0x40FEu)	/* EPFR43_INT0Eのセット値LOW */

/* <P131> */
#define EPFR00_ICU2E_00		(0x40CFu)	/* EPFR00_ICU2Eのセット値00 */
#define EPFR26_TIA1E_LOW	(0x40FDu)	/* EPFR26_TIA1Eのセット値LOW */
#define EPFR26_TIA1E_HIGH	(0x2002u)	/* EPFR26_TIA1Eのセット値HIGH */
#define EPFR36_SIN1E_LOW	(0x40FEu)	/* EPFR36_SIN1Eのセット値LOW */
#define EPFR43_INT4E_LOW	(0x40EFu)	/* EPFR43_INT4Eのセット値LOW */

/* <P132> */
#define EPFR00_ICU3E_00		(0x403Fu)	/* EPFR00_ICU3Eのセット値00 */
#define EPFR26_TIB0E_LOW	(0x40FBu)	/* EPFR26_TIB0Eのセット値LOW */
#define EPFR26_TIB0E_HIGH	(0x2004u)	/* EPFR26_TIB0Eのセット値HIGH */
#define EPFR36_SOT1E_X0		(0x40F7u)	/* EPFR36_SOT1Eのセット値X0 */
#define EPFR36_SOT1E_01		(0x13E7u)	/* EPFR36_SOT1Eのセット値01 */
#define EPFR43_INT2E_LOW	(0x40FBu)	/* EPFR43_INT2Eのセット値LOW */

/* <P133> */
#define EPFR01_ICU4E_00		(0x40FCu)	/* EPFR01_ICU4Eのセット値00 */
#define EPFR15_PPG11E_0X	(0x40EFu)	/* EPFR15_PPG11Eのセット値0X */
#define EPFR15_PPG11E_1X	(0x2010u)	/* EPFR15_PPG11Eのセット値1X */
#define EPFR26_TIB1E_LOW	(0x40F7u)	/* EPFR26_TIB1Eのセット値LOW */
#define EPFR26_TIB1E_HIGH	(0x2008u)	/* EPFR26_TIB1Eのセット値HIGH */
#define EPFR36_SCK1E_X0		(0x40FDu)	/* EPFR36_SCK1Eのセット値X0 */
#define EPFR36_SCK1E_01		(0x11F9u)	/* EPFR36_SCK1Eのセット値01 */
#define EPFR43_INT3E_LOW	(0x40F7u)	/* EPFR43_INT3Eのセット値LOW */

/* <P134> */
#define EPFR01_ICU5E_00		(0x40F3u)	/* EPFR01_ICU5Eのセット値00 */
#define EPFR10_PPG1E_0XXX	(0x40BFu)	/* EPFR10_PPG1Eのセット値0XXX */
#define EPFR10_PPG1E_1XXX	(0x2040u)	/* EPFR10_PPG1Eのセット値1XXX */
#define EPFR43_INT5E_LOW	(0x40DFu)	/* EPFR43_INT5Eのセット値LOW */

/* <P135> */
/* なし */

/* <P136> */
/* なし */

/* <P137> */
/* なし */

/* 【enum定義】 */
	
/* ビット番号 */
enum {
	BIT_NUM_0 = 0,			/* BIT番号0 */
	BIT_NUM_1,				/* BIT番号1 */
	BIT_NUM_2,				/* BIT番号2 */
	BIT_NUM_3,				/* BIT番号3 */
	BIT_NUM_4,				/* BIT番号4 */
	BIT_NUM_5,				/* BIT番号5 */
	BIT_NUM_6,				/* BIT番号6 */
	BIT_NUM_7,				/* BIT番号7 */
	BIT_MAX					/* BIT番号最大数 */
};

/* 機能レジスタビット番号 */
enum {
	BIT_FUNC_NUM_8 = 0,		/* BIT番号8  */
	BIT_FUNC_NUM_9,			/* BIT番号9  */
	BIT_FUNC_NUM_10,		/* BIT番号10 */
	BIT_FUNC_NUM_11,		/* BIT番号11 */
	BIT_FUNC_NUM_12,		/* BIT番号12 */
	BIT_FUNC_NUM_13,		/* BIT番号13 */
	BIT_FUNC_NUM_14,		/* BIT番号14 */
	BIT_FUNC_NUM_15,		/* BIT番号15 */
	BIT_FUNC_MAX			/* BIT番号最大数 */
};
	
/* ポートレジスタ数 */
enum {
	PORT_NUM_0 = 0,			/* PORT番号0 */
	PORT_NUM_1,				/* PORT番号1 */
	PORT_NUM_2,				/* PORT番号2 */
	PORT_NUM_3,				/* PORT番号3 */
	PORT_NUM_4,				/* PORT番号4 */
	PORT_NUM_5,				/* PORT番号5 */
	PORT_NUM_6,				/* PORT番号6 */
	PORT_NUM_7,				/* PORT番号7 */
	PORT_NUM_8,				/* PORT番号8 */
	PORT_NUM_9,				/* PORT番号9 */
	PORT_NUM_10,			/* PORT番号10 */
	PORT_NUM_11,			/* PORT番号11 */
	PORT_NUM_12,			/* PORT番号12 */
	PORT_NUM_13,			/* PORT番号13 */
	PORT_MAX				/* PORT番号最大数 */
};

/* 【const定義】 */

/* ビット番号テーブル */
const static UI_8 C_PortCheckBitTbl[BIT_MAX] =
{
	0x01u,			/* BIT0 */
	0x02u,			/* BIT1 */
	0x04u,			/* BIT2 */
	0x08u,			/* BIT3 */
	0x10u,			/* BIT4 */
	0x20u,			/* BIT5 */
	0x40u,			/* BIT6 */
	0x80u			/* BIT7 */
};

#if (PORT_DRV_FUNC_CHANGE == 1)
/* 機能レジスタビット番号テーブル */
const static UI_16 C_PortFuncCheckBitTbl[BIT_MAX] =
{
	0x0100u,		/* BIT8  */
	0x0200u,		/* BIT9  */
	0x0400u,		/* BIT10 */
	0x0800u,		/* BIT11 */
	0x1000u,		/* BIT12 */
	0x2000u,		/* BIT13 */
	0x4000u,		/* BIT14 */
	0x8000u			/* BIT15 */
};
#endif	/* PORT_DRV_FUNC_CHANGE == 1 */

/* ポートデータレジスタアドレステーブル */
static __io volatile UI_8 *const C_PortDataTbl[PORTDRV_PORT_MAX] =
{
	&(PDR00),	/* P0のポートデータレジスタアドレス */
	&(PDR01),	/* P1のポートデータレジスタアドレス */
	&(PDR02),	/* P2のポートデータレジスタアドレス */
	&(PDR03),	/* P3のポートデータレジスタアドレス */
	&(PDR04),	/* P4のポートデータレジスタアドレス */
	&(PDR05),	/* P5のポートデータレジスタアドレス */
	&(PDR06),	/* P6のポートデータレジスタアドレス */
	&(PDR07),	/* P7のポートデータレジスタアドレス */
	&(PDR08),	/* P8のポートデータレジスタアドレス */
	&(PDR09),	/* P9のポートデータレジスタアドレス */
	&(PDR10),	/* P10のポートデータレジスタアドレス */
	&(PDR11),	/* P11のポートデータレジスタアドレス */
	&(PDR12),	/* P12のポートデータレジスタアドレス */
	&(PDR13)	/* P13のポートデータレジスタアドレス */
};

/* 入力データダイレクトリードレジスタアドレステーブル */
static __io volatile const UI_8 *const C_PortDataDirectTbl[PORT_MAX] =
{
	&(PDDR00),	/* P0の入力データダイレクトリードレジスタ */
	&(PDDR01),	/* P1の入力データダイレクトリードレジスタ */
	&(PDDR02),	/* P2の入力データダイレクトリードレジスタ */
	&(PDDR03),	/* P3の入力データダイレクトリードレジスタ */
	&(PDDR04),	/* P4の入力データダイレクトリードレジスタ */
	&(PDDR05),	/* P5の入力データダイレクトリードレジスタ */
	&(PDDR06),	/* P6の入力データダイレクトリードレジスタ */
	&(PDDR07),	/* P7の入力データダイレクトリードレジスタ */
	&(PDDR08),	/* P8の入力データダイレクトリードレジスタ */
	&(PDDR09),	/* P9の入力データダイレクトリードレジスタ */
	&(PDDR10),	/* P10の入力データダイレクトリードレジスタ */
	&(PDDR11),	/* P11の入力データダイレクトリードレジスタ */
	&(PDDR12),	/* P12の入力データダイレクトリードレジスタ */
	&(PDDR13)	/* P13の入力データダイレクトリードレジスタ */
};

/* データ方向レジスタアドレステーブル */
static __io volatile UI_8 *const C_DataDirectionTbl[PORT_MAX] =
{
	&(DDR00),	/* P0のデータ方向レジスタアドレス */
	&(DDR01),	/* P1のデータ方向レジスタアドレス */
	&(DDR02),	/* P2のデータ方向レジスタアドレス */
	&(DDR03),	/* P3のデータ方向レジスタアドレス */
	&(DDR04),	/* P4のデータ方向レジスタアドレス */
	&(DDR05),	/* P5のデータ方向レジスタアドレス */
	&(DDR06),	/* P6のデータ方向レジスタアドレス */
	&(DDR07),	/* P7のデータ方向レジスタアドレス */
	&(DDR08),	/* P8のデータ方向レジスタアドレス */
	&(DDR09),	/* P9のデータ方向レジスタアドレス */
	&(DDR10),	/* P10のデータ方向レジスタアドレス */
	&(DDR11),	/* P11のデータ方向レジスタアドレス */
	&(DDR12),	/* P12のデータ方向レジスタアドレス */
	&(DDR13)	/* P13のデータ方向レジスタアドレス */
};

/* ポート機能レジスタアドレステーブル */
static __io volatile UI_8 *const C_PortFunctionTbl[PORT_MAX] =
{
	&(PFR00),	/* P0のポート機能レジスタアドレス */
	&(PFR01),	/* P1のポート機能レジスタアドレス */
	&(PFR02),	/* P2のポート機能レジスタアドレス */
	&(PFR03),	/* P3のポート機能レジスタアドレス */
	&(PFR04),	/* P4のポート機能レジスタアドレス */
	&(PFR05),	/* P5のポート機能レジスタアドレス */
	&(PFR06),	/* P6のポート機能レジスタアドレス */
	&(PFR07),	/* P7のポート機能レジスタアドレス */
	&(PFR08),	/* P8のポート機能レジスタアドレス */
	&(PFR09),	/* P9のポート機能レジスタアドレス */
	&(PFR10),	/* P10のポート機能レジスタアドレス */
	&(PFR11),	/* P11のポート機能レジスタアドレス */
	&(PFR12),	/* P12のポート機能レジスタアドレス */
	&(PFR13)	/* P13のポート機能レジスタアドレス */
};

/* 拡張ポート機能レジスタアドレステーブル */
static __io volatile UI_8 *const C_ExtendedPortFunctionTbl[PORTDRV_EPFR_MAX] =
{
	&(EPFR00),	/* P0の拡張ポート機能レジスタアドレス */
	&(EPFR01),	/* P1の拡張ポート機能レジスタアドレス */
	&(EPFR02),	/* P2の拡張ポート機能レジスタアドレス */
	&(EPFR03),	/* P3の拡張ポート機能レジスタアドレス */
	&(EPFR04),	/* P4の拡張ポート機能レジスタアドレス */
	&(EPFR05),	/* P5の拡張ポート機能レジスタアドレス */
	&(EPFR06),	/* P6の拡張ポート機能レジスタアドレス */
	&(EPFR07),	/* P7の拡張ポート機能レジスタアドレス */
	&(EPFR08),	/* P8の拡張ポート機能レジスタアドレス */
	&(EPFR09),	/* P9の拡張ポート機能レジスタアドレス */
	&(EPFR10),	/* P10の拡張ポート機能レジスタアドレス */
	&(EPFR11),	/* P11の拡張ポート機能レジスタアドレス */
	&(EPFR12),	/* P12の拡張ポート機能レジスタアドレス */
	&(EPFR13),	/* P13の拡張ポート機能レジスタアドレス */
	&(EPFR14),	/* P14の拡張ポート機能レジスタアドレス */
	&(EPFR15),	/* P15の拡張ポート機能レジスタアドレス */
	&(EPFR16),	/* P16の拡張ポート機能レジスタアドレス */
	&(EPFR17),	/* P17の拡張ポート機能レジスタアドレス */
	&(EPFR18),	/* P18の拡張ポート機能レジスタアドレス */
	&(EPFR19),	/* P19の拡張ポート機能レジスタアドレス */
	&(EPFR20),	/* P20の拡張ポート機能レジスタアドレス */
	&(EPFR21),	/* P21の拡張ポート機能レジスタアドレス */
	&(EPFR22),	/* P22の拡張ポート機能レジスタアドレス */
	&(EPFR23),	/* P23の拡張ポート機能レジスタアドレス */
	&(EPFR24),	/* P24の拡張ポート機能レジスタアドレス */
	&(EPFR25),	/* P25の拡張ポート機能レジスタアドレス */
	&(EPFR26),	/* P26の拡張ポート機能レジスタアドレス */
	&(EPFR27),	/* P27の拡張ポート機能レジスタアドレス */
	&(EPFR28),	/* P28の拡張ポート機能レジスタアドレス */
	&(EPFR29),	/* P29の拡張ポート機能レジスタアドレス */
	&(EPFR30),	/* P30の拡張ポート機能レジスタアドレス */
	&(EPFR31),	/* P31の拡張ポート機能レジスタアドレス */
	&(EPFR32),	/* P32の拡張ポート機能レジスタアドレス */
	&(EPFR33),	/* P33の拡張ポート機能レジスタアドレス */
	&(EPFR34),	/* P34の拡張ポート機能レジスタアドレス */
	&(EPFR35),	/* P35の拡張ポート機能レジスタアドレス */
	&(EPFR36),	/* P36の拡張ポート機能レジスタアドレス */
	&(EPFR37),	/* P37の拡張ポート機能レジスタアドレス */
	&(EPFR38),	/* P38の拡張ポート機能レジスタアドレス */
	&(EPFR39),	/* P39の拡張ポート機能レジスタアドレス */
	&(EPFR40),	/* P40の拡張ポート機能レジスタアドレス */
	&(EPFR41),	/* P41の拡張ポート機能レジスタアドレス */
	&(EPFR42),	/* P42の拡張ポート機能レジスタアドレス */
	&(EPFR43),	/* P43の拡張ポート機能レジスタアドレス */
	&(EPFR44),	/* P44の拡張ポート機能レジスタアドレス */
	&(EPFR45),	/* P45の拡張ポート機能レジスタアドレス */
	&(EPFR46),	/* P46の拡張ポート機能レジスタアドレス */
	&(EPFR47),	/* P47の拡張ポート機能レジスタアドレス */
	&(EPFR52),	/* P52の拡張ポート機能レジスタアドレス */
	&(EPFR53),	/* P53の拡張ポート機能レジスタアドレス */
	&(EPFR54)	/* P54の拡張ポート機能レジスタアドレス */
};

/* プルアップダウン制御レジスタアドレステーブル */
static __io volatile UI_8 *const C_PullControlTbl[PORT_MAX] =
{
	&(PPCR00),	/* P0のプルアップダウン制御レジスタアドレス */
	&(PPCR01),	/* P1のプルアップダウン制御レジスタアドレス */
	&(PPCR02),	/* P2のプルアップダウン制御レジスタアドレス */
	&(PPCR03),	/* P3のプルアップダウン制御レジスタアドレス */
	&(PPCR04),	/* P4のプルアップダウン制御レジスタアドレス */
	&(PPCR05),	/* P5のプルアップダウン制御レジスタアドレス */
	&(PPCR06),	/* P6のプルアップダウン制御レジスタアドレス */
	&(PPCR07),	/* P7のプルアップダウン制御レジスタアドレス */
	&(PPCR08),	/* P8のプルアップダウン制御レジスタアドレス */
	&(PPCR09),	/* P9のプルアップダウン制御レジスタアドレス */
	&(PPCR10),	/* P10のプルアップダウン制御レジスタアドレス */
	&(PPCR11),	/* P11のプルアップダウン制御レジスタアドレス */
	&(PPCR12),	/* P12のプルアップダウン制御レジスタアドレス */
	&(PPCR13)	/* P13のプルアップダウン制御レジスタアドレス */
};

/* プルアップダウン許可レジスタアドレステーブル */
static __io volatile UI_8 *const C_PullEnableTbl [PORT_MAX] =
{
	&(PPER00),	/* P0のプルアップダウン許可レジスタアドレス */
	&(PPER01),	/* P1のプルアップダウン許可レジスタアドレス */
	&(PPER02),	/* P2のプルアップダウン許可レジスタアドレス */
	&(PPER03),	/* P3のプルアップダウン許可レジスタアドレス */
	&(PPER04),	/* P4のプルアップダウン許可レジスタアドレス */
	&(PPER05),	/* P5のプルアップダウン許可レジスタアドレス */
	&(PPER06),	/* P6のプルアップダウン許可レジスタアドレス */
	&(PPER07),	/* P7のプルアップダウン許可レジスタアドレス */
	&(PPER08),	/* P8のプルアップダウン許可レジスタアドレス */
	&(PPER09),	/* P9のプルアップダウン許可レジスタアドレス */
	&(PPER10),	/* P10のプルアップダウン許可レジスタアドレス */
	&(PPER11),	/* P11のプルアップダウン許可レジスタアドレス */
	&(PPER12),	/* P12のプルアップダウン許可レジスタアドレス */
	&(PPER13)	/* P13のプルアップダウン許可レジスタアドレス */
};

/* ポート入力レベル選択レジスタアドレステーブル */
static __io volatile UI_8 *const C_PortInputLevelTbl[PORT_MAX] =
{
	&(PILR00),	/* P0のポート入力レベル選択レジスタアドレス */
	&(PILR01),	/* P1のポート入力レベル選択レジスタアドレス */
	&(PILR02),	/* P2のポート入力レベル選択レジスタアドレス */
	&(PILR03),	/* P3のポート入力レベル選択レジスタアドレス */
	&(PILR04),	/* P4のポート入力レベル選択レジスタアドレス */
	&(PILR05),	/* P5のポート入力レベル選択レジスタアドレス */
	&(PILR06),	/* P6のポート入力レベル選択レジスタアドレス */
	&(PILR07),	/* P7のポート入力レベル選択レジスタアドレス */
	&(PILR08),	/* P8のポート入力レベル選択レジスタアドレス */
	&(PILR09),	/* P9のポート入力レベル選択レジスタアドレス */
	&(PILR10),	/* P10のポート入力レベル選択レジスタアドレス */
	&(PILR11),	/* P11のポート入力レベル選択レジスタアドレス */
	&(PILR12),	/* P12のポート入力レベル選択レジスタアドレス */
	&(PILR13)	/* P13のポート入力レベル選択レジスタアドレス */
};

/* 拡張ポート入力レベル選択レジスタアドレステーブル */
static __io volatile UI_8 *const C_ExtendedPortInputLevelTbl[PORT_MAX] =
{
	&(EPILR00),	/* P0の拡張ポート入力レベル選択レジスタアドレス */
	&(EPILR01),	/* P1の拡張ポート入力レベル選択レジスタアドレス */
	&(EPILR02),	/* P2の拡張ポート入力レベル選択レジスタアドレス */
	&(EPILR03),	/* P3の拡張ポート入力レベル選択レジスタアドレス */
	&(EPILR04),	/* P4の拡張ポート入力レベル選択レジスタアドレス */
	&(EPILR05),	/* P5の拡張ポート入力レベル選択レジスタアドレス */
	&(EPILR06),	/* P6の拡張ポート入力レベル選択レジスタアドレス */
	&(EPILR07),	/* P7の拡張ポート入力レベル選択レジスタアドレス */
	&(EPILR08),	/* P8の拡張ポート入力レベル選択レジスタアドレス */
	&(EPILR09),	/* P9の拡張ポート入力レベル選択レジスタアドレス */
	&(EPILR10),	/* P10の拡張ポート入力レベル選択レジスタアドレス */
	&(EPILR11),	/* P11の拡張ポート入力レベル選択レジスタアドレス */
	&(EPILR12),	/* P12の拡張ポート入力レベル選択レジスタアドレス */
	&(EPILR13)	/* P13の拡張ポート入力レベル選択レジスタアドレス */
};

/* ポート出力駆動レジスタアドレステーブル */
static __io volatile UI_8 *const C_PortOutputDriveTbl[PORT_MAX] =
{
	&(PODR00),		/* P0のポート出力駆動レジスタアドレス */
	&(PODR01),		/* P1のポート出力駆動レジスタアドレス */
	&(PODR02),		/* P2のポート出力駆動レジスタアドレス */
	&(PODR03),		/* P3のポート出力駆動レジスタアドレス */
	&(PODR04),		/* P4のポート出力駆動レジスタアドレス */
	&(PODR05),		/* P5のポート出力駆動レジスタアドレス */
	&(PODR06),		/* P6のポート出力駆動レジスタアドレス */
	&(PODR07),		/* P7のポート出力駆動レジスタアドレス */
	&(PODR08),		/* P8のポート出力駆動レジスタアドレス */
	&(PODR09),		/* P9のポート出力駆動レジスタアドレス */
	&(PODR10),		/* P10のポート出力駆動レジスタアドレス */
	&(PODR11),		/* P11のポート出力駆動レジスタアドレス */
	&(PODR12),		/* P12のポート出力駆動レジスタアドレス */
	&(PODR13)		/* P13のポート出力駆動レジスタアドレス */
};

/* 拡張ポート出力駆動レジスタアドレステーブル */
static __io volatile UI_8 *const C_ExtendedPortOutputDriveTbl[PORT_MAX] =
{
	D_NULL,			/* レジスタ設定なし */
	&(EPODR01),		/* P1の拡張ポート出力駆動レジスタアドレス */
	&(EPODR02),		/* P2の拡張ポート出力駆動レジスタアドレス */
	&(EPODR03),		/* P3の拡張ポート出力駆動レジスタアドレス */
	D_NULL,			/* レジスタ設定なし */
	D_NULL,			/* レジスタ設定なし */
	&(EPODR06),		/* P6の拡張ポート出力駆動レジスタアドレス */
	&(EPODR07),		/* P7の拡張ポート出力駆動レジスタアドレス */
	&(EPODR08),		/* P8の拡張ポート出力駆動レジスタアドレス */
	D_NULL,			/* レジスタ設定なし */
	D_NULL,			/* レジスタ設定なし */
	D_NULL,			/* レジスタ設定なし */
	D_NULL,			/* レジスタ設定なし */
	D_NULL,			/* レジスタ設定なし */
};

/* ポートデータレジスタ初期値テーブル */
const static UI_8 C_PortInitTbl[PORTDRV_PORT_MAX] =
{
	INIT_PDR0,	/* P0のポートデータレジスタの初期値 */
	INIT_PDR1,	/* P1のポートデータレジスタの初期値 */
	INIT_PDR2,	/* P2のポートデータレジスタの初期値 */
	INIT_PDR3,	/* P3のポートデータレジスタの初期値 */
	INIT_PDR4,	/* P4のポートデータレジスタの初期値 */
	INIT_PDR5,	/* P5のポートデータレジスタの初期値 */
	INIT_PDR6,	/* P6のポートデータレジスタの初期値 */
	INIT_PDR7,	/* P7のポートデータレジスタの初期値 */
	INIT_PDR8,	/* P8のポートデータレジスタの初期値 */
	INIT_PDR9,	/* P9のポートデータレジスタの初期値 */
	INIT_PDR10,	/* P10のポートデータレジスタの初期値 */
	INIT_PDR11,	/* P11のポートデータレジスタの初期値 */
	INIT_PDR12,	/* P12のポートデータレジスタの初期値 */
	INIT_PDR13	/* P13のポートデータレジスタの初期値 */
};

/* ポートデータレジスタ停止値テーブル */
const static UI_8 C_PortStopTbl[PORTDRV_PORT_MAX] =
{
	STOP_PDR0,	/* P0のポートデータレジスタの停止値 */
	STOP_PDR1,	/* P1のポートデータレジスタの停止値 */
	STOP_PDR2,	/* P2のポートデータレジスタの停止値 */
	STOP_PDR3,	/* P3のポートデータレジスタの停止値 */
	STOP_PDR4,	/* P4のポートデータレジスタの停止値 */
	STOP_PDR5,	/* P5のポートデータレジスタの停止値 */
	STOP_PDR6,	/* P6のポートデータレジスタの停止値 */
	STOP_PDR7,	/* P7のポートデータレジスタの停止値 */
	STOP_PDR8,	/* P8のポートデータレジスタの停止値 */
	STOP_PDR9,	/* P9のポートデータレジスタの停止値 */
	STOP_PDR10,	/* P10のポートデータレジスタの停止値 */
	STOP_PDR11,	/* P11のポートデータレジスタの停止値 */
	STOP_PDR12,	/* P12のポートデータレジスタの停止値 */
	STOP_PDR13	/* P13のポートデータレジスタの停止値 */
};

/* データ方向レジスタ初期値テーブル */
const static UI_8 C_DataDirectionInitTbl[PORT_MAX] =
{
	INIT_DDR0,	/* P0のデータ方向レジスタの初期値 */
	INIT_DDR1,	/* P1のデータ方向レジスタの初期値 */
	INIT_DDR2,	/* P2のデータ方向レジスタの初期値 */
	INIT_DDR3,	/* P3のデータ方向レジスタの初期値 */
	INIT_DDR4,	/* P4のデータ方向レジスタの初期値 */
	INIT_DDR5,	/* P5のデータ方向レジスタの初期値 */
	INIT_DDR6,	/* P6のデータ方向レジスタの初期値 */
	INIT_DDR7,	/* P7のデータ方向レジスタの初期値 */
	INIT_DDR8,	/* P8のデータ方向レジスタの初期値 */
	INIT_DDR9,	/* P9のデータ方向レジスタの初期値 */
	INIT_DDR10,	/* P10のデータ方向レジスタの初期値 */
	INIT_DDR11,	/* P11のデータ方向レジスタの初期値 */
	INIT_DDR12,	/* P12のデータ方向レジスタの初期値 */
	INIT_DDR13	/* P13のデータ方向レジスタの初期値 */
};

/* データ方向レジスタ停止値テーブル */
const static UI_8 C_DataDirectionStopTbl[PORT_MAX] =
{
	STOP_DDR0,	/* P0のデータ方向レジスタの停止値 */
	STOP_DDR1,	/* P1のデータ方向レジスタの停止値 */
	STOP_DDR2,	/* P2のデータ方向レジスタの停止値 */
	STOP_DDR3,	/* P3のデータ方向レジスタの停止値 */
	STOP_DDR4,	/* P4のデータ方向レジスタの停止値 */
	STOP_DDR5,	/* P5のデータ方向レジスタの停止値 */
	STOP_DDR6,	/* P6のデータ方向レジスタの停止値 */
	STOP_DDR7,	/* P7のデータ方向レジスタの停止値 */
	STOP_DDR8,	/* P8のデータ方向レジスタの停止値 */
	STOP_DDR9,	/* P9のデータ方向レジスタの停止値 */
	STOP_DDR10,	/* P10のデータ方向レジスタの停止値 */
	STOP_DDR11,	/* P11のデータ方向レジスタの停止値 */
	STOP_DDR12,	/* P12のデータ方向レジスタの停止値 */
	STOP_DDR13	/* P13のデータ方向レジスタの停止値 */
};

/* データ方向レジスタ再設定値テーブル */
const static UI_8 C_DataDirectionRefreshTbl[PORT_MAX] =
{
	REFRESH_DDR0,	/* P0のデータ方向レジスタの再設定値 */
	REFRESH_DDR1,	/* P1のデータ方向レジスタの再設定値 */
	REFRESH_DDR2,	/* P2のデータ方向レジスタの再設定値 */
	REFRESH_DDR3,	/* P3のデータ方向レジスタの再設定値 */
	REFRESH_DDR4,	/* P4のデータ方向レジスタの再設定値 */
	REFRESH_DDR5,	/* P5のデータ方向レジスタの再設定値 */
	REFRESH_DDR6,	/* P6のデータ方向レジスタの再設定値 */
	REFRESH_DDR7,	/* P7のデータ方向レジスタの再設定値 */
	REFRESH_DDR8,	/* P8のデータ方向レジスタの再設定値 */
	REFRESH_DDR9,	/* P9のデータ方向レジスタの再設定値 */
	REFRESH_DDR10,	/* P10のデータ方向レジスタの再設定値 */
	REFRESH_DDR11,	/* P11のデータ方向レジスタの再設定値 */
	REFRESH_DDR12,	/* P12のデータ方向レジスタの再設定値 */
	REFRESH_DDR13	/* P13のデータ方向レジスタの再設定値 */
};

/* ポート機能レジスタ初期値テーブル */
const static UI_8 C_PortFunctionInitTbl[PORT_MAX] =
{
	INIT_PFR0,	/* P0のポート機能レジスタの初期値 */
	INIT_PFR1,	/* P1のポート機能レジスタの初期値 */
	INIT_PFR2,	/* P2のポート機能レジスタの初期値 */
	INIT_PFR3,	/* P3のポート機能レジスタの初期値 */
	INIT_PFR4,	/* P4のポート機能レジスタの初期値 */
	INIT_PFR5,	/* P5のポート機能レジスタの初期値 */
	INIT_PFR6,	/* P6のポート機能レジスタの初期値 */
	INIT_PFR7,	/* P7のポート機能レジスタの初期値 */
	INIT_PFR8,	/* P8のポート機能レジスタの初期値 */
	INIT_PFR9,	/* P9のポート機能レジスタの初期値 */
	INIT_PFR10,	/* P10のポート機能レジスタの初期値 */
	INIT_PFR11,	/* P11のポート機能レジスタの初期値 */
	INIT_PFR12,	/* P12のポート機能レジスタの初期値 */
	INIT_PFR13	/* P13のポート機能レジスタの初期値 */
};

/* ポート機能レジスタ停止値テーブル */
const static UI_8 C_PortFunctionStopTbl[PORT_MAX] =
{
	STOP_PFR0,	/* P0のポート機能レジスタの停止値 */
	STOP_PFR1,	/* P1のポート機能レジスタの停止値 */
	STOP_PFR2,	/* P2のポート機能レジスタの停止値 */
	STOP_PFR3,	/* P3のポート機能レジスタの停止値 */
	STOP_PFR4,	/* P4のポート機能レジスタの停止値 */
	STOP_PFR5,	/* P5のポート機能レジスタの停止値 */
	STOP_PFR6,	/* P6のポート機能レジスタの停止値 */
	STOP_PFR7,	/* P7のポート機能レジスタの停止値 */
	STOP_PFR8,	/* P8のポート機能レジスタの停止値 */
	STOP_PFR9,	/* P9のポート機能レジスタの停止値 */
	STOP_PFR10,	/* P10のポート機能レジスタの停止値 */
	STOP_PFR11,	/* P11のポート機能レジスタの停止値 */
	STOP_PFR12,	/* P12のポート機能レジスタの停止値 */
	STOP_PFR13	/* P13のポート機能レジスタの停止値 */
};

/* 拡張ポート機能レジスタ初期値テーブル */
const static UI_8 C_ExtendedPortFunctionInitTbl[PORTDRV_EPFR_MAX] =
{
	INIT_EPFR0,		/* P0の拡張ポート機能レジスタの初期値 */
	INIT_EPFR1,		/* P1の拡張ポート機能レジスタの初期値 */
	INIT_EPFR2,		/* P2の拡張ポート機能レジスタの初期値 */
	INIT_EPFR3,		/* P3の拡張ポート機能レジスタの初期値 */
	INIT_EPFR4,		/* P4の拡張ポート機能レジスタの初期値 */
	INIT_EPFR5,		/* P5の拡張ポート機能レジスタの初期値 */
	INIT_EPFR6,		/* P6の拡張ポート機能レジスタの初期値 */
	INIT_EPFR7,		/* P7の拡張ポート機能レジスタの初期値 */
	INIT_EPFR8,		/* P8の拡張ポート機能レジスタの初期値 */
	INIT_EPFR9,		/* P9の拡張ポート機能レジスタの初期値 */
	INIT_EPFR10,	/* P10の拡張ポート機能レジスタの初期値 */
	INIT_EPFR11,	/* P11の拡張ポート機能レジスタの初期値 */
	INIT_EPFR12,	/* P12の拡張ポート機能レジスタの初期値 */
	INIT_EPFR13,	/* P13の拡張ポート機能レジスタの初期値 */
	INIT_EPFR14,	/* P14の拡張ポート機能レジスタの初期値 */
	INIT_EPFR15,	/* P15の拡張ポート機能レジスタの初期値 */
	INIT_EPFR16,	/* P16の拡張ポート機能レジスタの初期値 */
	INIT_EPFR17,	/* P17の拡張ポート機能レジスタの初期値 */
	INIT_EPFR18,	/* P18の拡張ポート機能レジスタの初期値 */
	INIT_EPFR19,	/* P19の拡張ポート機能レジスタの初期値 */
	INIT_EPFR20,	/* P20の拡張ポート機能レジスタの初期値 */
	INIT_EPFR21,	/* P21の拡張ポート機能レジスタの初期値 */
	INIT_EPFR22,	/* P22の拡張ポート機能レジスタの初期値 */
	INIT_EPFR23,	/* P23の拡張ポート機能レジスタの初期値 */
	INIT_EPFR24,	/* P24の拡張ポート機能レジスタの初期値 */
	INIT_EPFR25,	/* P25の拡張ポート機能レジスタの初期値 */
	INIT_EPFR26,	/* P26の拡張ポート機能レジスタの初期値 */
	INIT_EPFR27,	/* P27の拡張ポート機能レジスタの初期値 */
	INIT_EPFR28,	/* P28の拡張ポート機能レジスタの初期値 */
	INIT_EPFR29,	/* P29の拡張ポート機能レジスタの初期値 */
	INIT_EPFR30,	/* P30の拡張ポート機能レジスタの初期値 */
	INIT_EPFR31,	/* P31の拡張ポート機能レジスタの初期値 */
	INIT_EPFR32,	/* P32の拡張ポート機能レジスタの初期値 */
	INIT_EPFR33,	/* P33の拡張ポート機能レジスタの初期値 */
	INIT_EPFR34,	/* P34の拡張ポート機能レジスタの初期値 */
	INIT_EPFR35,	/* P35の拡張ポート機能レジスタの初期値 */
	INIT_EPFR36,	/* P36の拡張ポート機能レジスタの初期値 */
	INIT_EPFR37,	/* P37の拡張ポート機能レジスタの初期値 */
	INIT_EPFR38,	/* P38の拡張ポート機能レジスタの初期値 */
	INIT_EPFR39,	/* P39の拡張ポート機能レジスタの初期値 */
	INIT_EPFR40,	/* P40の拡張ポート機能レジスタの初期値 */
	INIT_EPFR41,	/* P41の拡張ポート機能レジスタの初期値 */
	INIT_EPFR42,	/* P42の拡張ポート機能レジスタの初期値 */
	INIT_EPFR43,	/* P43の拡張ポート機能レジスタの初期値 */
	INIT_EPFR44,	/* P44の拡張ポート機能レジスタの初期値 */
	INIT_EPFR45,	/* P45の拡張ポート機能レジスタの初期値 */
	INIT_EPFR46,	/* P46の拡張ポート機能レジスタの初期値 */
	INIT_EPFR47,	/* P47の拡張ポート機能レジスタの初期値 */
	INIT_EPFR52,	/* P52の拡張ポート機能レジスタの初期値 */
	INIT_EPFR53,	/* P53の拡張ポート機能レジスタの初期値 */
	INIT_EPFR54		/* P54の拡張ポート機能レジスタの初期値 */
};

/* 拡張ポート機能レジスタ停止値テーブル */
const static UI_8 C_ExtendedPortFunctionStopTbl[PORTDRV_EPFR_MAX] =
{
	STOP_EPFR0,		/* P0の拡張ポート機能レジスタの停止値 */
	STOP_EPFR1,		/* P1の拡張ポート機能レジスタの停止値 */
	STOP_EPFR2,		/* P2の拡張ポート機能レジスタの停止値 */
	STOP_EPFR3,		/* P3の拡張ポート機能レジスタの停止値 */
	STOP_EPFR4,		/* P4の拡張ポート機能レジスタの停止値 */
	STOP_EPFR5,		/* P5の拡張ポート機能レジスタの停止値 */
	STOP_EPFR6,		/* P6の拡張ポート機能レジスタの停止値 */
	STOP_EPFR7,		/* P7の拡張ポート機能レジスタの停止値 */
	STOP_EPFR8,		/* P8の拡張ポート機能レジスタの停止値 */
	STOP_EPFR9,		/* P9の拡張ポート機能レジスタの停止値 */
	STOP_EPFR10,	/* P10の拡張ポート機能レジスタの停止値 */
	STOP_EPFR11,	/* P11の拡張ポート機能レジスタの停止値 */
	STOP_EPFR12,	/* P12の拡張ポート機能レジスタの停止値 */
	STOP_EPFR13,	/* P13の拡張ポート機能レジスタの停止値 */
	STOP_EPFR14,	/* P14の拡張ポート機能レジスタの停止値 */
	STOP_EPFR15,	/* P15の拡張ポート機能レジスタの停止値 */
	STOP_EPFR16,	/* P16の拡張ポート機能レジスタの停止値 */
	STOP_EPFR17,	/* P17の拡張ポート機能レジスタの停止値 */
	STOP_EPFR18,	/* P18の拡張ポート機能レジスタの停止値 */
	STOP_EPFR19,	/* P19の拡張ポート機能レジスタの停止値 */
	STOP_EPFR20,	/* P20の拡張ポート機能レジスタの停止値 */
	STOP_EPFR21,	/* P21の拡張ポート機能レジスタの停止値 */
	STOP_EPFR22,	/* P22の拡張ポート機能レジスタの停止値 */
	STOP_EPFR23,	/* P23の拡張ポート機能レジスタの停止値 */
	STOP_EPFR24,	/* P24の拡張ポート機能レジスタの停止値 */
	STOP_EPFR25,	/* P25の拡張ポート機能レジスタの停止値 */
	STOP_EPFR26,	/* P26の拡張ポート機能レジスタの停止値 */
	STOP_EPFR27,	/* P27の拡張ポート機能レジスタの停止値 */
	STOP_EPFR28,	/* P28の拡張ポート機能レジスタの停止値 */
	STOP_EPFR29,	/* P29の拡張ポート機能レジスタの停止値 */
	STOP_EPFR30,	/* P30の拡張ポート機能レジスタの停止値 */
	STOP_EPFR31,	/* P31の拡張ポート機能レジスタの停止値 */
	STOP_EPFR32,	/* P32の拡張ポート機能レジスタの停止値 */
	STOP_EPFR33,	/* P33の拡張ポート機能レジスタの停止値 */
	STOP_EPFR34,	/* P34の拡張ポート機能レジスタの停止値 */
	STOP_EPFR35,	/* P35の拡張ポート機能レジスタの停止値 */
	STOP_EPFR36,	/* P36の拡張ポート機能レジスタの停止値 */
	STOP_EPFR37,	/* P37の拡張ポート機能レジスタの停止値 */
	STOP_EPFR38,	/* P38の拡張ポート機能レジスタの停止値 */
	STOP_EPFR39,	/* P39の拡張ポート機能レジスタの停止値 */
	STOP_EPFR40,	/* P40の拡張ポート機能レジスタの停止値 */
	STOP_EPFR41,	/* P41の拡張ポート機能レジスタの停止値 */
	STOP_EPFR42,	/* P42の拡張ポート機能レジスタの停止値 */
	STOP_EPFR43,	/* P43の拡張ポート機能レジスタの停止値 */
	STOP_EPFR44,	/* P44の拡張ポート機能レジスタの停止値 */
	STOP_EPFR45,	/* P45の拡張ポート機能レジスタの停止値 */
	STOP_EPFR46,	/* P46の拡張ポート機能レジスタの停止値 */
	STOP_EPFR47,	/* P47の拡張ポート機能レジスタの停止値 */
	STOP_EPFR52,	/* P52の拡張ポート機能レジスタの停止値 */
	STOP_EPFR53,	/* P53の拡張ポート機能レジスタの停止値 */
	STOP_EPFR54	/* P54の拡張ポート機能レジスタの停止値 */
};

/* プルアップダウン制御レジスタ初期値テーブル */
const static UI_8 C_PullControlInitTbl[PORT_MAX] =
{
	INIT_PPCR0,		/* P0のプルアップダウン制御レジスタの初期値 */
	INIT_PPCR1,		/* P1のプルアップダウン制御レジスタの初期値 */
	INIT_PPCR2,		/* P2のプルアップダウン制御レジスタの初期値 */
	INIT_PPCR3,		/* P3のプルアップダウン制御レジスタの初期値 */
	INIT_PPCR4,		/* P4のプルアップダウン制御レジスタの初期値 */
	INIT_PPCR5,		/* P5のプルアップダウン制御レジスタの初期値 */
	INIT_PPCR6,		/* P6のプルアップダウン制御レジスタの初期値 */
	INIT_PPCR7,		/* P7のプルアップダウン制御レジスタの初期値 */
	INIT_PPCR8,		/* P8のプルアップダウン制御レジスタの初期値 */
	INIT_PPCR9,		/* P9のプルアップダウン制御レジスタの初期値 */
	INIT_PPCR10,	/* P10のプルアップダウン制御レジスタの初期値 */
	INIT_PPCR11,	/* P11のプルアップダウン制御レジスタの初期値 */
	INIT_PPCR12,	/* P12のプルアップダウン制御レジスタの初期値 */
	INIT_PPCR13		/* P13のプルアップダウン制御レジスタの初期値 */
};

/* プルアップダウン制御レジスタ停止値テーブル */
const static UI_8 C_PullControlStopTbl[PORT_MAX] =
{
	STOP_PPCR0,		/* P0のプルアップダウン制御レジスタの停止値 */
	STOP_PPCR1,		/* P1のプルアップダウン制御レジスタの停止値 */
	STOP_PPCR2,		/* P2のプルアップダウン制御レジスタの停止値 */
	STOP_PPCR3,		/* P3のプルアップダウン制御レジスタの停止値 */
	STOP_PPCR4,		/* P4のプルアップダウン制御レジスタの停止値 */
	STOP_PPCR5,		/* P5のプルアップダウン制御レジスタの停止値 */
	STOP_PPCR6,		/* P6のプルアップダウン制御レジスタの停止値 */
	STOP_PPCR7,		/* P7のプルアップダウン制御レジスタの停止値 */
	STOP_PPCR8,		/* P8のプルアップダウン制御レジスタの停止値 */
	STOP_PPCR9,		/* P9のプルアップダウン制御レジスタの停止値 */
	STOP_PPCR10,	/* P10のプルアップダウン制御レジスタの停止値 */
	STOP_PPCR11,	/* P11のプルアップダウン制御レジスタの停止値 */
	STOP_PPCR12,	/* P12のプルアップダウン制御レジスタの停止値 */
	STOP_PPCR13		/* P13のプルアップダウン制御レジスタの停止値 */
};

/* プルアップダウン許可レジスタ初期値テーブル */
const static UI_8 C_PullEnableInitTbl[PORT_MAX] =
{
	INIT_PPER0,		/* P0のプルアップダウン許可レジスタの初期値 */
	INIT_PPER1,		/* P1のプルアップダウン許可レジスタの初期値 */
	INIT_PPER2,		/* P2のプルアップダウン許可レジスタの初期値 */
	INIT_PPER3,		/* P3のプルアップダウン許可レジスタの初期値 */
	INIT_PPER4,		/* P4のプルアップダウン許可レジスタの初期値 */
	INIT_PPER5,		/* P5のプルアップダウン許可レジスタの初期値 */
	INIT_PPER6,		/* P6のプルアップダウン許可レジスタの初期値 */
	INIT_PPER7,		/* P7のプルアップダウン許可レジスタの初期値 */
	INIT_PPER8,		/* P8のプルアップダウン許可レジスタの初期値 */
	INIT_PPER9,		/* P9のプルアップダウン許可レジスタの初期値 */
	INIT_PPER10,	/* P10のプルアップダウン許可レジスタの初期値 */
	INIT_PPER11,	/* P11のプルアップダウン許可レジスタの初期値 */
	INIT_PPER12,	/* P12のプルアップダウン許可レジスタの初期値 */
	INIT_PPER13		/* P13のプルアップダウン許可レジスタの初期値 */
};

/* プルアップダウン許可レジスタ停止値テーブル */
const static UI_8 C_PullEnableStopTbl[PORT_MAX] =
{
	STOP_PPER0,		/* P0のプルアップダウン許可レジスタの停止値 */
	STOP_PPER1,		/* P1のプルアップダウン許可レジスタの停止値 */
	STOP_PPER2,		/* P2のプルアップダウン許可レジスタの停止値 */
	STOP_PPER3,		/* P3のプルアップダウン許可レジスタの停止値 */
	STOP_PPER4,		/* P4のプルアップダウン許可レジスタの停止値 */
	STOP_PPER5,		/* P5のプルアップダウン許可レジスタの停止値 */
	STOP_PPER6,		/* P6のプルアップダウン許可レジスタの停止値 */
	STOP_PPER7,		/* P7のプルアップダウン許可レジスタの停止値 */
	STOP_PPER8,		/* P8のプルアップダウン許可レジスタの停止値 */
	STOP_PPER9,		/* P9のプルアップダウン許可レジスタの停止値 */
	STOP_PPER10,	/* P10のプルアップダウン許可レジスタの停止値 */
	STOP_PPER11,	/* P11のプルアップダウン許可レジスタの停止値 */
	STOP_PPER12,	/* P12のプルアップダウン許可レジスタの停止値 */
	STOP_PPER13		/* P13のプルアップダウン許可レジスタの停止値 */
};

/* ポート入力レベル選択レジスタ初期値テーブル */
const static UI_8 C_PortInputLevelInitTbl[PORT_MAX] =
{
	INIT_PILR0,		/* P0のポート入力レベル選択レジスタの初期値 */
	INIT_PILR1,		/* P1のポート入力レベル選択レジスタの初期値 */
	INIT_PILR2,		/* P2のポート入力レベル選択レジスタの初期値 */
	INIT_PILR3,		/* P3のポート入力レベル選択レジスタの初期値 */
	INIT_PILR4,		/* P4のポート入力レベル選択レジスタの初期値 */
	INIT_PILR5,		/* P5のポート入力レベル選択レジスタの初期値 */
	INIT_PILR6,		/* P6のポート入力レベル選択レジスタの初期値 */
	INIT_PILR7,		/* P7のポート入力レベル選択レジスタの初期値 */
	INIT_PILR8,		/* P8のポート入力レベル選択レジスタの初期値 */
	INIT_PILR9,		/* P9のポート入力レベル選択レジスタの初期値 */
	INIT_PILR10,	/* P10のポート入力レベル選択レジスタの初期値 */
	INIT_PILR11,	/* P11のポート入力レベル選択レジスタの初期値 */
	INIT_PILR12,	/* P12のポート入力レベル選択レジスタの初期値 */
	INIT_PILR13		/* P13のポート入力レベル選択レジスタの初期値 */
};

/* ポート入力レベル選択レジスタ停止値テーブル */
const static UI_8 C_PortInputLevelStopTbl[PORT_MAX] =
{
	STOP_PILR0,		/* P0のポート入力レベル選択レジスタの停止値 */
	STOP_PILR1,		/* P1のポート入力レベル選択レジスタの停止値 */
	STOP_PILR2,		/* P2のポート入力レベル選択レジスタの停止値 */
	STOP_PILR3,		/* P3のポート入力レベル選択レジスタの停止値 */
	STOP_PILR4,		/* P4のポート入力レベル選択レジスタの停止値 */
	STOP_PILR5,		/* P5のポート入力レベル選択レジスタの停止値 */
	STOP_PILR6,		/* P6のポート入力レベル選択レジスタの停止値 */
	STOP_PILR7,		/* P7のポート入力レベル選択レジスタの停止値 */
	STOP_PILR8,		/* P8のポート入力レベル選択レジスタの停止値 */
	STOP_PILR9,		/* P9のポート入力レベル選択レジスタの停止値 */
	STOP_PILR10,	/* P10のポート入力レベル選択レジスタの停止値 */
	STOP_PILR11,	/* P11のポート入力レベル選択レジスタの停止値 */
	STOP_PILR12,	/* P12のポート入力レベル選択レジスタの停止値 */
	STOP_PILR13		/* P13のポート入力レベル選択レジスタの停止値 */
};

/* 拡張ポート入力レベル選択レジスタ初期値テーブル */
const static UI_8 C_ExtendedPortInputLevelInitTbl[PORT_MAX] =
{
	INIT_EPILR0,	/* P0の拡張ポート入力レベル選択レジスタの初期値 */
	INIT_EPILR1,	/* P1の拡張ポート入力レベル選択レジスタの初期値 */
	INIT_EPILR2,	/* P2の拡張ポート入力レベル選択レジスタの初期値 */
	INIT_EPILR3,	/* P3の拡張ポート入力レベル選択レジスタの初期値 */
	INIT_EPILR4,	/* P4の拡張ポート入力レベル選択レジスタの初期値 */
	INIT_EPILR5,	/* P5の拡張ポート入力レベル選択レジスタの初期値 */
	INIT_EPILR6,	/* P6の拡張ポート入力レベル選択レジスタの初期値 */
	INIT_EPILR7,	/* P7の拡張ポート入力レベル選択レジスタの初期値 */
	INIT_EPILR8,	/* P8の拡張ポート入力レベル選択レジスタの初期値 */
	INIT_EPILR9,	/* P9の拡張ポート入力レベル選択レジスタの初期値 */
	INIT_EPILR10,	/* P10の拡張ポート入力レベル選択レジスタの初期値 */
	INIT_EPILR11,	/* P11の拡張ポート入力レベル選択レジスタの初期値 */
	INIT_EPILR12,	/* P12の拡張ポート入力レベル選択レジスタの初期値 */
	INIT_EPILR13	/* P13の拡張ポート入力レベル選択レジスタの初期値 */
};

/* 拡張ポート入力レベル選択レジスタ停止値テーブル */
const static UI_8 C_ExtendedPortInputLevelStopTbl[PORT_MAX] =
{
	STOP_EPILR0,	/* P0の拡張ポート入力レベル選択レジスタの停止値 */
	STOP_EPILR1,	/* P1の拡張ポート入力レベル選択レジスタの停止値 */
	STOP_EPILR2,	/* P2の拡張ポート入力レベル選択レジスタの停止値 */
	STOP_EPILR3,	/* P3の拡張ポート入力レベル選択レジスタの停止値 */
	STOP_EPILR4,	/* P4の拡張ポート入力レベル選択レジスタの停止値 */
	STOP_EPILR5,	/* P5の拡張ポート入力レベル選択レジスタの停止値 */
	STOP_EPILR6,	/* P6の拡張ポート入力レベル選択レジスタの停止値 */
	STOP_EPILR7,	/* P7の拡張ポート入力レベル選択レジスタの停止値 */
	STOP_EPILR8,	/* P8の拡張ポート入力レベル選択レジスタの停止値 */
	STOP_EPILR9,	/* P9の拡張ポート入力レベル選択レジスタの停止値 */
	STOP_EPILR10,	/* P10の拡張ポート入力レベル選択レジスタの停止値 */
	STOP_EPILR11,	/* P11の拡張ポート入力レベル選択レジスタの停止値 */
	STOP_EPILR12,	/* P12の拡張ポート入力レベル選択レジスタの停止値 */
	STOP_EPILR13	/* P13の拡張ポート入力レベル選択レジスタの停止値 */
};

/* ポート出力駆動レジスタ初期値テーブル */
const static UI_8 C_PortOutputDriveInitTbl[PORT_MAX] =
{
	INIT_PODR0,		/* P0のポート出力駆動レジスタの初期値 */
	INIT_PODR1,		/* P1のポート出力駆動レジスタの初期値 */
	INIT_PODR2,		/* P2のポート出力駆動レジスタの初期値 */
	INIT_PODR3,		/* P3のポート出力駆動レジスタの初期値 */
	INIT_PODR4,		/* P4のポート出力駆動レジスタの初期値 */
	INIT_PODR5,		/* P5のポート出力駆動レジスタの初期値 */
	INIT_PODR6,		/* P6のポート出力駆動レジスタの初期値 */
	INIT_PODR7,		/* P7のポート出力駆動レジスタの初期値 */
	INIT_PODR8,		/* P8のポート出力駆動レジスタの初期値 */
	INIT_PODR9,		/* P9のポート出力駆動レジスタの初期値 */
	INIT_PODR10,	/* P10のポート出力駆動レジスタの初期値 */
	INIT_PODR11,	/* P11のポート出力駆動レジスタの初期値 */
	INIT_PODR12,	/* P12のポート出力駆動レジスタの初期値 */
	INIT_PODR13		/* P13のポート出力駆動レジスタの初期値 */
};

/* ポート出力駆動レジスタ停止値テーブル */
const static UI_8 C_PortOutputDriveStopTbl[PORT_MAX] =
{
	STOP_PODR0,		/* P0のポート出力駆動レジスタの停止値 */
	STOP_PODR1,		/* P1のポート出力駆動レジスタの停止値 */
	STOP_PODR2,		/* P2のポート出力駆動レジスタの停止値 */
	STOP_PODR3,		/* P3のポート出力駆動レジスタの停止値 */
	STOP_PODR4,		/* P4のポート出力駆動レジスタの停止値 */
	STOP_PODR5,		/* P5のポート出力駆動レジスタの停止値 */
	STOP_PODR6,		/* P6のポート出力駆動レジスタの停止値 */
	STOP_PODR7,		/* P7のポート出力駆動レジスタの停止値 */
	STOP_PODR8,		/* P8のポート出力駆動レジスタの停止値 */
	STOP_PODR9,		/* P9のポート出力駆動レジスタの停止値 */
	STOP_PODR10,	/* P10のポート出力駆動レジスタの停止値 */
	STOP_PODR11,	/* P11のポート出力駆動レジスタの停止値 */
	STOP_PODR12,	/* P12のポート出力駆動レジスタの停止値 */
	STOP_PODR13		/* P13のポート出力駆動レジスタの停止値 */
};

/* 拡張ポート出力駆動レジスタ初期値テーブル */
const static UI_8 C_ExtendedPortOutputDriveInitTbl[PORT_MAX] =
{
	D_NULL,			/* レジスタ設定なし*/
	INIT_EPODR1,	/* P1の拡張ポート出力駆動レジスタの初期値 */
	INIT_EPODR2,	/* P2の拡張ポート出力駆動レジスタの初期値 */
	INIT_EPODR3,	/* P3の拡張ポート出力駆動レジスタの初期値 */
	D_NULL,			/* レジスタ設定なし*/
	D_NULL,			/* レジスタ設定なし*/
	INIT_EPODR6,	/* P6の拡張ポート出力駆動レジスタの初期値 */
	INIT_EPODR7,	/* P7の拡張ポート出力駆動レジスタの初期値 */
	INIT_EPODR8,	/* P8の拡張ポート出力駆動レジスタの初期値 */
	D_NULL,			/* レジスタ設定なし*/
	D_NULL,			/* レジスタ設定なし*/
	D_NULL,			/* レジスタ設定なし*/
	D_NULL,			/* レジスタ設定なし*/
	D_NULL			/* レジスタ設定なし*/
};

/* 拡張ポート出力駆動レジスタ停止値テーブル */
const static UI_8 C_ExtendedPortOutputDriveStopTbl[PORT_MAX] =
{
	D_NULL,			/* レジスタ設定なし*/
	STOP_EPODR1,	/* P1の拡張ポート出力駆動レジスタの停止値 */
	STOP_EPODR2,	/* P2の拡張ポート出力駆動レジスタの停止値 */
	STOP_EPODR3,	/* P3の拡張ポート出力駆動レジスタの停止値 */
	D_NULL,			/* レジスタ設定なし*/
	D_NULL,			/* レジスタ設定なし*/
	STOP_EPODR6,	/* P6の拡張ポート出力駆動レジスタの停止値 */
	STOP_EPODR7,	/* P7の拡張ポート出力駆動レジスタの停止値 */
	STOP_EPODR8,	/* P8の拡張ポート出力駆動レジスタの停止値 */
	D_NULL,			/* レジスタ設定なし*/
	D_NULL,			/* レジスタ設定なし*/
	D_NULL,			/* レジスタ設定なし*/
	D_NULL,			/* レジスタ設定なし*/
	D_NULL			/* レジスタ設定なし*/
};

#if (PORT_DRV_FUNC_CHANGE == 1)
/* 機能レジスタ設定数テーブル */
const static UI_8 C_PortFuncChangeTbl[PORT_MAX] =
{
	PORT_FUNC_CHANGE0,		/* P0の機能レジスタ設定数 */
	PORT_FUNC_CHANGE1,		/* P1の機能レジスタ設定数 */
	PORT_FUNC_CHANGE2,		/* P2の機能レジスタ設定数 */
	PORT_FUNC_CHANGE3,		/* P3の機能レジスタ設定数 */
	PORT_FUNC_CHANGE4,		/* P4の機能レジスタ設定数 */
	PORT_FUNC_CHANGE5,		/* P5の機能レジスタ設定数 */
	PORT_FUNC_CHANGE6,		/* P6の機能レジスタ設定数 */
	PORT_FUNC_CHANGE7,		/* P7の機能レジスタ設定数 */
	PORT_FUNC_CHANGE8,		/* P8の機能レジスタ設定数 */
	PORT_FUNC_CHANGE9,		/* P9の機能レジスタ設定数 */
	PORT_FUNC_CHANGE10,		/* P10の機能レジスタ設定数 */
	PORT_FUNC_CHANGE11,		/* P11の機能レジスタ設定数 */
	PORT_FUNC_CHANGE12,		/* P12の機能レジスタ設定数 */
	PORT_FUNC_CHANGE13		/* P13の機能レジスタ設定数 */
};

/* 機能レジスタ設定数テーブル */
const static UI_8 C_PortFuncChangeNumTbl[PORT_MAX] =
{
	PORT_FUNC_CHANGE_NUM0,		/* P0の機能レジスタ数 */
	PORT_FUNC_CHANGE_NUM1,		/* P1の機能レジスタ数 */
	PORT_FUNC_CHANGE_NUM2,		/* P2の機能レジスタ数 */
	PORT_FUNC_CHANGE_NUM3,		/* P3の機能レジスタ数 */
	PORT_FUNC_CHANGE_NUM4,		/* P4の機能レジスタ数 */
	PORT_FUNC_CHANGE_NUM5,		/* P5の機能レジスタ数 */
	PORT_FUNC_CHANGE_NUM6,		/* P6の機能レジスタ数 */
	PORT_FUNC_CHANGE_NUM7,		/* P7の機能レジスタ数 */
	PORT_FUNC_CHANGE_NUM8,		/* P8の機能レジスタ数 */
	PORT_FUNC_CHANGE_NUM9,		/* P9の機能レジスタ数 */
	PORT_FUNC_CHANGE_NUM10,		/* P10の機能レジスタ数 */
	PORT_FUNC_CHANGE_NUM11,		/* P11の機能レジスタ数 */
	PORT_FUNC_CHANGE_NUM12,		/* P12の機能レジスタ数 */
	PORT_FUNC_CHANGE_NUM13		/* P13の機能レジスタ数 */
};

/* 機能切替設定値ID数テーブル */
const static UI_8 C_PortFuncChangeIdNumTbl[PORT_MAX][BIT_MAX] =
{
	/* PXX0							PXX1							PXX2							PXX3							PXX4							PXX5							PXX6							PXX7 */
	{ PORT_FUNC_CHANGE_ID_NUM_10,	PORT_FUNC_CHANGE_ID_NUM_10,		PORT_FUNC_CHANGE_ID_NUM_11,		PORT_FUNC_CHANGE_ID_NUM_10, 	PORT_FUNC_CHANGE_ID_NUM_10,		PORT_FUNC_CHANGE_ID_NUM_11,		PORT_FUNC_CHANGE_ID_NUM_9,		PORT_FUNC_CHANGE_ID_NUM_9	},		/* P000の機能数 */
	{ PORT_FUNC_CHANGE_ID_NUM_8,	PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8	},		/* P010の機能数 */
	{ PORT_FUNC_CHANGE_ID_NUM_6,	PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_7	},		/* P020の機能数 */
	{ PORT_FUNC_CHANGE_ID_NUM_7,	PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_7	},		/* P030の機能数 */
	{ PORT_FUNC_CHANGE_ID_NUM_7,	PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_8	},		/* P040の機能数 */
	{ PORT_FUNC_CHANGE_ID_NUM_5,	PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_4	},		/* P050の機能数 */
	{ PORT_FUNC_CHANGE_ID_NUM_4,	PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_6	},		/* P060の機能数 */
	{ PORT_FUNC_CHANGE_ID_NUM_5,	PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_8	},		/* P070の機能数 */
	{ PORT_FUNC_CHANGE_ID_NUM_7,	PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6	},		/* P080の機能数 */
	{ PORT_FUNC_CHANGE_ID_NUM_4,	PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_9,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_4,		PORT_FUNC_CHANGE_ID_NUM_4,		PORT_FUNC_CHANGE_ID_NUM_8	},		/* P090の機能数 */
	{ PORT_FUNC_CHANGE_ID_NUM_6,	PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_7	},		/* P100の機能数 */
	{ PORT_FUNC_CHANGE_ID_NUM_5,	PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_4,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_9,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_9	},		/* P110の機能数 */
	{ PORT_FUNC_CHANGE_ID_NUM_8,	PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_4	},		/* P120の機能数 */
	{ PORT_FUNC_CHANGE_ID_NUM_8,	PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_10,		PORT_FUNC_CHANGE_ID_NUM_6,		D_NULL,							PORT_FUNC_CHANGE_ID_NUM_3,		PORT_FUNC_CHANGE_ID_NUM_3	}		/* P130の機能数 */
};
	
/*《P00 機能レジスタアドレステーブル》*/
static __io volatile UI_8 * const C_PortFuncTbl_P00[PORT_FUNC_CHANGE_NUM0] =
{
	&(DDR00),		/* データ方向レジスタ00のアドレス */
	&(PFR00),		/* ポート機能レジスタ00のアドレス */
	&(EPFR02),		/* 拡張ポート機能レジスタ02のアドレス */
	&(EPFR03),		/* 拡張ポート機能レジスタ03のアドレス */
	&(EPFR04),		/* 拡張ポート機能レジスタ04のアドレス */
	&(EPFR05),		/* 拡張ポート機能レジスタ05のアドレス */
	&(EPFR06),		/* 拡張ポート機能レジスタ06のアドレス */
	&(EPFR07),		/* 拡張ポート機能レジスタ07のアドレス */
	&(EPFR10),		/* 拡張ポート機能レジスタ10のアドレス */
	&(EPFR11),		/* 拡張ポート機能レジスタ11のアドレス */
	&(EPFR12),		/* 拡張ポート機能レジスタ12のアドレス */
	&(EPFR13),		/* 拡張ポート機能レジスタ13のアドレス */
	&(EPFR43),		/* 拡張ポート機能レジスタ43のアドレス */
	&(EPFR47)		/* 拡張ポート機能レジスタ47のアドレス */
};

/*《P01 機能レジスタアドレステーブル》*/
static __io volatile UI_8 * const C_PortFuncTbl_P01[PORT_FUNC_CHANGE_NUM1] =
{
	&(DDR01),		/* データ方向レジスタ01のアドレス */
	&(PFR01),		/* ポート機能レジスタ01のアドレス */
	&(EPFR16),		/* 拡張ポート機能レジスタ16のアドレス */
	&(EPFR44),		/* 拡張ポート機能レジスタ44のアドレス */
	&(EPFR47)		/* 拡張ポート機能レジスタ47のアドレス */
};

/* 《P02 機能レジスタアドレステーブル》 */
static __io volatile UI_8 * const C_PortFuncTbl_P02[PORT_FUNC_CHANGE_NUM2] =
{
	&(DDR02),		/* データ方向レジスタ02のアドレス */
	&(PFR02),		/* ポート機能レジスタ01のアドレス */
	&(EPFR01),		/* 拡張ポート機能レジスタ01のアドレス */
	&(EPFR17),		/* 拡張ポート機能レジスタ17のアドレス */
	&(EPFR29),		/* 拡張ポート機能レジスタ29のアドレス */
	&(EPFR32),		/* 拡張ポート機能レジスタ32のアドレス */
	&(EPFR33),		/* 拡張ポート機能レジスタ33のアドレス */
	&(EPFR39)		/* 拡張ポート機能レジスタ39のアドレス */
};

/* 《P03 機能レジスタアドレステーブル》 */
static __io volatile UI_8 * const C_PortFuncTbl_P03[PORT_FUNC_CHANGE_NUM3] =
{
	&(DDR03),		/* データ方向レジスタ03のアドレス */
	&(PFR03),		/* ポート機能レジスタ01のアドレス */
	&(EPFR15),		/* 拡張ポート機能レジスタ15のアドレス */
	&(EPFR18),		/* 拡張ポート機能レジスタ18のアドレス */
	&(EPFR31),		/* 拡張ポート機能レジスタ31のアドレス */
	&(EPFR33),		/* 拡張ポート機能レジスタ33のアドレス */
	&(EPFR34),		/* 拡張ポート機能レジスタ34のアドレス */
	&(EPFR37),		/* 拡張ポート機能レジスタ37のアドレス */
	&(EPFR38)		/* 拡張ポート機能レジスタ38のアドレス */
};

/* 《P04 機能レジスタアドレステーブル》*/
static __io volatile UI_8 * const C_PortFuncTbl_P04[PORT_FUNC_CHANGE_NUM4] =
{
	&(DDR04),		/* データ方向レジスタ03のアドレス */
	&(PFR04),		/* ポート機能レジスタ01のアドレス */
	&(EPFR19),		/* 拡張ポート機能レジスタ19のアドレス */
	&(EPFR30),		/* 拡張ポート機能レジスタ30のアドレス */
	&(EPFR31),		/* 拡張ポート機能レジスタ31のアドレス */
	&(EPFR34),		/* 拡張ポート機能レジスタ34のアドレス */
	&(EPFR37),		/* 拡張ポート機能レジスタ37のアドレス */
	&(EPFR40),		/* 拡張ポート機能レジスタ40のアドレス */
	&(EPFR41),		/* 拡張ポート機能レジスタ41のアドレス */
	&(EPFR45)		/* 拡張ポート機能レジスタ45のアドレス */
};

/* 《P05 機能レジスタアドレステーブル》*/
static __io volatile UI_8 * const C_PortFuncTbl_P05[PORT_FUNC_CHANGE_NUM5] =
{	
	&(DDR05),		/* データ方向レジスタ03のアドレス */
	&(PFR05),		/* ポート機能レジスタ01のアドレス */
	&(EPFR20),		/* 拡張ポート機能レジスタ20のアドレス */
	&(EPFR28),		/* 拡張ポート機能レジスタ28のアドレス */
	&(EPFR32)		/* 拡張ポート機能レジスタ32のアドレス */
};

/* 《P06 機能レジスタアドレステーブル》*/
static __io volatile UI_8 * const C_PortFuncTbl_P06[PORT_FUNC_CHANGE_NUM6] =
{
	&(DDR06),		/* データ方向レジスタ03のアドレス */
	&(PFR06),		/* ポート機能レジスタ01のアドレス */
	&(EPFR21),		/* 拡張ポート機能レジスタ21のアドレス */
	&(EPFR35),		/* 拡張ポート機能レジスタ35のアドレス */
	&(EPFR36),		/* 拡張ポート機能レジスタ36のアドレス */
	&(EPFR38),		/* 拡張ポート機能レジスタ38のアドレス */
	&(EPFR40),		/* 拡張ポート機能レジスタ40のアドレス */
	&(EPFR41)		/* 拡張ポート機能レジスタ41のアドレス */
};

/* 《P07 機能レジスタアドレステーブル》*/
static __io volatile UI_8 * const C_PortFuncTbl_P07[PORT_FUNC_CHANGE_NUM7] =
{
	&(DDR07),		/* データ方向レジスタ03のアドレス */
	&(PFR07),		/* ポート機能レジスタ01のアドレス */
	&(EPFR01),		/* 拡張ポート機能レジスタ01のアドレス */
	&(EPFR15),		/* 拡張ポート機能レジスタ15のアドレス */
	&(EPFR22),		/* 拡張ポート機能レジスタ22のアドレス */
	&(EPFR34),		/* 拡張ポート機能レジスタ34のアドレス */
	&(EPFR37),		/* 拡張ポート機能レジスタ37のアドレス */
	&(EPFR38),		/* 拡張ポート機能レジスタ38のアドレス */
	&(EPFR39),		/* 拡張ポート機能レジスタ39のアドレス */
	&(EPFR45)		/* 拡張ポート機能レジスタ45のアドレス */
};

/* 《P08 機能レジスタアドレステーブル》*/
static __io volatile UI_8 * const C_PortFuncTbl_P08[PORT_FUNC_CHANGE_NUM8] =
{
	&(DDR08),		/* データ方向レジスタ03のアドレス */
	&(PFR08),		/* ポート機能レジスタ01のアドレス */
	&(EPFR00),		/* 拡張ポート機能レジスタ00のアドレス */
	&(EPFR01),		/* 拡張ポート機能レジスタ01のアドレス */
	&(EPFR23),		/* 拡張ポート機能レジスタ23のアドレス */
	&(EPFR33),		/* 拡張ポート機能レジスタ33のアドレス */
	&(EPFR40),		/* 拡張ポート機能レジスタ40のアドレス */
	&(EPFR45),		/* 拡張ポート機能レジスタ45のアドレス */
	&(EPFR46)		/* 拡張ポート機能レジスタ46のアドレス */
};

/* 《P09 機能レジスタアドレステーブル》*/
static __io volatile UI_8 * const C_PortFuncTbl_P09[PORT_FUNC_CHANGE_NUM9] =
{
	&(DDR09),		/* データ方向レジスタ09のアドレス */
	&(PFR09),		/* ポート機能レジスタ09のアドレス */
	&(EPFR00),		/* 拡張ポート機能レジスタ00のアドレス */
	&(EPFR01),		/* 拡張ポート機能レジスタ01のアドレス */
	&(EPFR02),		/* 拡張ポート機能レジスタ02のアドレス */
	&(EPFR04),		/* 拡張ポート機能レジスタ04のアドレス */
	&(EPFR05),		/* 拡張ポート機能レジスタ05のアドレス */
	&(EPFR06),		/* 拡張ポート機能レジスタ06のアドレス */
	&(EPFR07),		/* 拡張ポート機能レジスタ07のアドレス */
	&(EPFR10),		/* 拡張ポート機能レジスタ10のアドレス */
	&(EPFR13),		/* 拡張ポート機能レジスタ13のアドレス */
	&(EPFR14),		/* 拡張ポート機能レジスタ14のアドレス */
	&(EPFR15),		/* 拡張ポート機能レジスタ15のアドレス */
	&(EPFR24),		/* 拡張ポート機能レジスタ24のアドレス */
	&(EPFR27),		/* 拡張ポート機能レジスタ27のアドレス */
	&(EPFR44)		/* 拡張ポート機能レジスタ44のアドレス */
};

/* 《P10 機能レジスタアドレステーブル》*/
static __io volatile UI_8 * const C_PortFuncTbl_P10[PORT_FUNC_CHANGE_NUM10] =
{
	&(DDR10),		/* データ方向レジスタ09のアドレス */
	&(PFR10),		/* ポート機能レジスタ01のアドレス */
	&(EPFR01),		/* 拡張ポート機能レジスタ01のアドレス */
	&(EPFR02),		/* 拡張ポート機能レジスタ02のアドレス */
	&(EPFR03),		/* 拡張ポート機能レジスタ03のアドレス */
	&(EPFR04),		/* 拡張ポート機能レジスタ04のアドレス */
	&(EPFR05),		/* 拡張ポート機能レジスタ05のアドレス */
	&(EPFR08),		/* 拡張ポート機能レジスタ08のアドレス */
	&(EPFR09),		/* 拡張ポート機能レジスタ09のアドレス */
	&(EPFR10),		/* 拡張ポート機能レジスタ10のアドレス */
	&(EPFR11),		/* 拡張ポート機能レジスタ11のアドレス */
	&(EPFR12),		/* 拡張ポート機能レジスタ12のアドレス */
	&(EPFR14),		/* 拡張ポート機能レジスタ14のアドレス */
	&(EPFR15),		/* 拡張ポート機能レジスタ15のアドレス */
	&(EPFR30),		/* 拡張ポート機能レジスタ30のアドレス */
	&(EPFR39),		/* 拡張ポート機能レジスタ39のアドレス */
	&(EPFR42)		/* 拡張ポート機能レジスタ42のアドレス */
};

/* 《P11機能レジスタアドレステーブル》*/
static __io volatile UI_8 * const C_PortFuncTbl_P11[PORT_FUNC_CHANGE_NUM11] =
{
	&(DDR11),		/* データ方向レジスタ09のアドレス */
	&(PFR11),		/* ポート機能レジスタ01のアドレス */
	&(EPFR01),		/* 拡張ポート機能レジスタ01のアドレス */
	&(EPFR02),		/* 拡張ポート機能レジスタ02のアドレス */
	&(EPFR03),		/* 拡張ポート機能レジスタ03のアドレス */
	&(EPFR04),		/* 拡張ポート機能レジスタ04のアドレス */
	&(EPFR05),		/* 拡張ポート機能レジスタ05のアドレス */
	&(EPFR07),		/* 拡張ポート機能レジスタ07のアドレス */
	&(EPFR08),		/* 拡張ポート機能レジスタ08のアドレス */
	&(EPFR10),		/* 拡張ポート機能レジスタ10のアドレス */
	&(EPFR11),		/* 拡張ポート機能レジスタ11のアドレス */
	&(EPFR12),		/* 拡張ポート機能レジスタ12のアドレス */
	&(EPFR24),		/* 拡張ポート機能レジスタ24のアドレス */
	&(EPFR28),		/* 拡張ポート機能レジスタ28のアドレス */
	&(EPFR30),		/* 拡張ポート機能レジスタ30のアドレス */
	&(EPFR44)		/* 拡張ポート機能レジスタ44のアドレス */
};

/* 《P12 機能レジスタアドレステーブル》*/
static __io volatile UI_8* const C_PortFuncTbl_P12[PORT_FUNC_CHANGE_NUM12] =
{
	&(DDR12),		/* データ方向レジスタ09のアドレス */
	&(PFR12),		/* ポート機能レジスタ01のアドレス */
	&(EPFR00),		/* 拡張ポート機能レジスタ00のアドレス */
	&(EPFR01),		/* 拡張ポート機能レジスタ01のアドレス */
	&(EPFR03),		/* 拡張ポート機能レジスタ03のアドレス */
	&(EPFR04),		/* 拡張ポート機能レジスタ04のアドレス */
	&(EPFR05),		/* 拡張ポート機能レジスタ05のアドレス */
	&(EPFR09),		/* 拡張ポート機能レジスタ09のアドレス */
	&(EPFR12),		/* 拡張ポート機能レジスタ12のアドレス */
	&(EPFR13),		/* 拡張ポート機能レジスタ13のアドレス */
	&(EPFR14),		/* 拡張ポート機能レジスタ14のアドレス */
	&(EPFR15),		/* 拡張ポート機能レジスタ15のアドレス */
	&(EPFR28),		/* 拡張ポート機能レジスタ28のアドレス */
	&(EPFR29),		/* 拡張ポート機能レジスタ29のアドレス */
	&(EPFR31),		/* 拡張ポート機能レジスタ31のアドレス */
	&(EPFR35),		/* 拡張ポート機能レジスタ35のアドレス */
	&(EPFR42),		/* 拡張ポート機能レジスタ42のアドレス */
	&(EPFR43)		/* 拡張ポート機能レジスタ43のアドレス */
};

/* 《P13 機能レジスタアドレステーブル》*/
static __io volatile UI_8* const C_PortFuncTbl_P13[PORT_FUNC_CHANGE_NUM13] =
{
	&(DDR13),		/* データ方向レジスタ09のアドレス */
	&(PFR13),		/* ポート機能レジスタ01のアドレス */
	&(EPFR00),		/* 拡張ポート機能レジスタ00のアドレス */
	&(EPFR01),		/* 拡張ポート機能レジスタ01のアドレス */
	&(EPFR10),		/* 拡張ポート機能レジスタ10のアドレス */
	&(EPFR15),		/* 拡張ポート機能レジスタ15のアドレス */
	&(EPFR26),		/* 拡張ポート機能レジスタ26のアドレス */
	&(EPFR35),		/* 拡張ポート機能レジスタ35のアドレス */
	&(EPFR36),		/* 拡張ポート機能レジスタ36のアドレス */
	&(EPFR43)		/* 拡張ポート機能レジスタ43のアドレス */
};

/* 機能レジスタポート選択テーブル */
static __io volatile UI_8* const * const C_PortFuncTbl[PORT_MAX] =
{
	&C_PortFuncTbl_P00[0],	/* 機能レジスタアドレステーブルへのポインタP00 */
	&C_PortFuncTbl_P01[0],	/* 機能レジスタアドレステーブルへのポインタP01 */
	&C_PortFuncTbl_P02[0],	/* 機能レジスタアドレステーブルへのポインタP02 */
	&C_PortFuncTbl_P03[0],	/* 機能レジスタアドレステーブルへのポインタP03 */
	&C_PortFuncTbl_P04[0],	/* 機能レジスタアドレステーブルへのポインタP04 */
	&C_PortFuncTbl_P05[0],	/* 機能レジスタアドレステーブルへのポインタP05 */
	&C_PortFuncTbl_P06[0],	/* 機能レジスタアドレステーブルへのポインタP06 */
	&C_PortFuncTbl_P07[0],	/* 機能レジスタアドレステーブルへのポインタP07 */
	&C_PortFuncTbl_P08[0],	/* 機能レジスタアドレステーブルへのポインタP08 */
	&C_PortFuncTbl_P09[0],	/* 機能レジスタアドレステーブルへのポインタP09 */
	&C_PortFuncTbl_P10[0],	/* 機能レジスタアドレステーブルへのポインタP10 */
	&C_PortFuncTbl_P11[0],	/* 機能レジスタアドレステーブルへのポインタP11 */
	&C_PortFuncTbl_P12[0],	/* 機能レジスタアドレステーブルへのポインタP12 */
	&C_PortFuncTbl_P13[0]	/* 機能レジスタアドレステーブルへのポインタP13 */
};

/* 『P000機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P000[PORT_FUNC_CHANGE_ID_NUM_10][PORT_FUNC_CHANGE0] =
{
	/* DDR00 			PFR00			EPFR02				EPFR03		EPFR04		EPFR05		EPFR06				EPFR07		EPFR10				EPFR11		EPFR12		EPFR13		EPFR43				EPFR47					ID */
	{ D_LOW_PXX0, 		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX0, 		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_LOW_PXX0,		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR06_SIN2E_HIGH,	D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SERIAL_IN		},		/* LIN_UART シリアル入力ch.2 リロケーション1 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR02_TIN0E_10,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				RELOAD_TIMER_EVENT_IN	},		/* リロードタイマイベント入力ch.0 リロケーション2 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX1,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG 出力ch.0 リロケーション0 */
	{ D_LOW_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX0,		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		EPFR43_INT0E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.0 リロケーション1 */
};

/* 『P001機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P001[PORT_FUNC_CHANGE_ID_NUM_10][PORT_FUNC_CHANGE0] =
{
	/* DDR00 			PFR00			EPFR02		EPFR03				EPFR04		EPFR05		EPFR06				EPFR07		EPFR10				EPFR11		EPFR12		EPFR13		EPFR43				EPFR47					ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX1,		D_HIGH_PXX1,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_1X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SERIAL_IN		},		/* LIN_UART シリアル出力ch.2 リロケーション1 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		EPFR03_TIN1E_10,	D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				RELOAD_TIMER_EVENT_IN	},		/* リロードタイマイベント入力ch.1 リロケーション2 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX1,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG 出力ch.1 リロケーション0 */
	{ D_LOW_PXX1,		D_HIGH_PXX1,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		EPFR43_INT1E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.1 リロケーション1 */
};

/* 『P002機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P002[PORT_FUNC_CHANGE_ID_NUM_11][PORT_FUNC_CHANGE0] =
{
	/* DDR00			PFR00			EPFR02		EPFR03		EPFR04				EPFR05		EPFR06				EPFR07		EPFR10		EPFR11				EPFR12		EPFR13		EPFR43				EPFR47					ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_10,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SCK_IN			},		/* LIN_UART シリアルクロック入力ch.2 リロケーション1 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_10,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SCK_OUT		},		/* LIN_UART シリアルクロック出力ch.2 リロケーション1 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		EPFR04_TIN2E_10,	D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				RELOAD_TIMER_EVENT_IN	},		/* リロードタイマイベント入力ch.2 リロケーション2 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX1,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG 出力ch.1 リロケーション0 */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		EPFR43_INT2E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.2 リロケーション1 */
};

/* 『P003機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P003[PORT_FUNC_CHANGE_ID_NUM_10][PORT_FUNC_CHANGE0] =
{
	/* DDR00 			PFR00			EPFR02		EPFR03		EPFR04		EPFR05				EPFR06		EPFR07				EPFR10		EPFR11				EPFR12		EPFR13		EPFR43				EPFR47					ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR07_SIN3E_HIGH,	D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SERIAL_IN		},		/* LIN_UART シリアル入力ch.3 リロケーション1 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TIN3E_10,	D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				RELOAD_TIMER_EVENT_IN	},		/* リロードタイマイベント入力ch.3 リロケーション2 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX1,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG 出力ch.3 リロケーション0 */
	{ D_LOW_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		EPFR43_INT3E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.3 リロケーション1 */
};

/* 『P004機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P004[PORT_FUNC_CHANGE_ID_NUM_10][PORT_FUNC_CHANGE0] =
{
	/* DDR00 			PFR00			EPFR02				EPFR03		EPFR04		EPFR05		EPFR06		EPFR07				EPFR10		EPFR11		EPFR12				EPFR13		EPFR43				EPFR47					ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX4,		D_HIGH_PXX4,	EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_1X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SERIAL_OUT		},		/* LIN_UART シリアル出力ch.3 リロケーション1 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR02_TOT0E_1XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				RELOAD_TIMER_EVENT_IN	},		/* リロードタイマ出力ch.0 リロケーション2 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX1,	D_NULL,		NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG 出力ch.4 リロケーション0 */
	{ D_LOW_PXX4,		D_HIGH_PXX4,	EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		EPFR43_INT4E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.4 リロケーション1 */
};

/* 『P005機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P005[PORT_FUNC_CHANGE_ID_NUM_11][PORT_FUNC_CHANGE0] =
{
	/* DDR00 			PFR00			EPFR02		EPFR03				EPFR04		EPFR05		EPFR06		EPFR07				EPFR10		EPFR11		EPFR12				EPFR13		EPFR43				EPFR47					ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_LOW_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_10,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SCK_IN			},		/* LIN_UART シリアルクロック入力ch.3 リロケーション1 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_10,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SCK_OUT		},		/* LIN_UART シリアルクロック出力ch.3 リロケーション1 */	
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_1XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				RELOAD_TIMER_EVENT_IN	},		/* リロードタイマ出力ch.1 リロケーション2 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX1,	D_NULL,		NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG 出力ch.5 リロケーション0 */
	{ D_LOW_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		EPFR43_INT5E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.5 リロケーション1 */
};

/* 『P006機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P006[PORT_FUNC_CHANGE_ID_NUM_9][PORT_FUNC_CHANGE0] =
{
	/* DDR00 			PFR00			EPFR02		EPFR03		EPFR04				EPFR05		EPFR06		EPFR07		EPFR10		EPFR11		EPFR12		EPFR13				EPFR43				EPFR47					ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		EPFR04_TOT2E_1XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	NO_MASK,			NO_MASK,				RELOAD_TIMER_OUT		},		/* リロードタイマ出力ch.2 リロケーション2 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX1,	NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG 出力ch.6 リロケーション0 */
	{ D_LOW_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	EPFR43_INT6E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.6 リロケーション1 */
};

/* 『P007機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P007[PORT_FUNC_CHANGE_ID_NUM_9][PORT_FUNC_CHANGE0] =
{
	/* DDR00 			PFR00			EPFR02		EPFR03		EPFR04		EPFR05				EPFR06		EPFR07		EPFR10		EPFR11		EPFR12		EPFR13				EPFR43				EPFR47					ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_1XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	NO_MASK,			NO_MASK,				RELOAD_TIMER_OUT		},		/* リロードタイマ出力ch.3 リロケーション2 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX1,	NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG 出力ch.7 リロケーション0 */
	{ D_LOW_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	EPFR43_INT7E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.7 リロケーション1 */
};

/* 『P010機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P010[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					EPFR44				EPFR47					ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR16_SEG0E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		EPFR16_SEG0E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX0,		D_HIGH_PXX0,	EPFR16_SEG0E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR16_SEG0E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR16_SEG0E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_LOW_PXX0,		D_HIGH_PXX0,	EPFR16_SEG0E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR16_SEG0E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR16_SEG0E_LOW,		EPFR44_INT8E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.8 リロケーション1 */	
};

/* 『P011機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P011[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					PFR44				EPFR47					ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR16_SEG1E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		EPFR16_SEG1E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX1,		D_HIGH_PXX1,	EPFR16_SEG1E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR16_SEG1E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR16_SEG1E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_LOW_PXX1,		D_HIGH_PXX1,	EPFR16_SEG1E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR16_SEG1E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR16_SEG1E_LOW,		EPFR44_INT9E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.9 リロケーション1 */	
};

/* 『P012機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P012[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					PFR44				EPFR47					ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR16_SEG2E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		EPFR16_SEG2E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	EPFR16_SEG2E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR16_SEG2E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR16_SEG2E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	EPFR16_SEG2E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR16_SEG2E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR16_SEG2E_LOW,		EPFR44_INT10E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.10 リロケーション1 */	
};

/* 『P013機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P013[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					PFR44				EPFR47					ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR16_SEG3E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		EPFR16_SEG3E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX3,		D_HIGH_PXX3,	EPFR16_SEG3E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR16_SEG3E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR16_SEG3E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_LOW_PXX3,		D_HIGH_PXX3,	EPFR16_SEG3E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入出力 32bit */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR16_SEG3E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ入出力 32bit */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR16_SEG3E_LOW,		EPFR44_INT11E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.11 リロケーション1 */	
};

/* 『P014機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P014[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					PFR44				EPFR47					ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR16_SEG4E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		EPFR16_SEG5E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX4,		D_HIGH_PXX4,	EPFR16_SEG4E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR16_SEG4E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR16_SEG4E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_LOW_PXX4,		D_HIGH_PXX4,	EPFR16_SEG4E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR16_SEG4E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR16_SEG4E_LOW,		EPFR44_INT12E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.12 リロケーション1 */	
};

/* 『P015機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P015[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					PFR44				EPFR47					ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR16_SEG5E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		EPFR16_SEG5E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX5,		D_HIGH_PXX5,	EPFR16_SEG5E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR16_SEG5E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR16_SEG5E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_LOW_PXX5,		D_HIGH_PXX5,	EPFR16_SEG5E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR16_SEG5E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR16_SEG5E_LOW,		EPFR44_INT13E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.13 リロケーション1 */	
};

/* 『P016機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P016[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					PFR44				EPFR47					ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR16_SEG6E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		EPFR16_SEG6E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX6,		D_HIGH_PXX6,	EPFR16_SEG6E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR16_SEG6E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR16_SEG6E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_LOW_PXX6,		D_HIGH_PXX6,	EPFR16_SEG6E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR16_SEG6E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR16_SEG6E_LOW,		EPFR44_INT14E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.14 リロケーション1 */	
};

/* 『P017機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P017[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					PFR44				EPFR47					ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR16_SEG7E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		EPFR16_SEG7E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX7,		D_HIGH_PXX7,	EPFR16_SEG7E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* 外部バスデータ入力 16bit */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR16_SEG7E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* 外部バスデータ出力 16bit */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR16_SEG7E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_LOW_PXX7,		D_HIGH_PXX7,	EPFR16_SEG7E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* 外部バスデータ入力 32bit */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR16_SEG7E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* 外部バスデータ出力 32bit */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR16_SEG7E_LOW,		EPFR44_INT15E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* 外部割込み要求入力ch.15 リロケーション1 */	
};

/* 『P020機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P020[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE2] =
{
	/* DDR02 		PFR02			EPFR01				EPFR17				EPFR29				EPFR32		EPFR33		EPFR39		ID */
	{ D_LOW_PXX0,	D_LOW_PXX0,		NO_MASK,			EPFR17_SEG8E_LOW,	EPFR29_OCU0E_0X,	D_NULL,		D_NULL,		D_NULL,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX0,	D_LOW_PXX0,		NO_MASK,			EPFR17_SEG8E_LOW,	EPFR29_OCU0E_0X,	D_NULL,		D_NULL,		D_NULL,		PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX0,	D_HIGH_PXX0,	NO_MASK,			EPFR17_SEG8E_LOW,	EPFR29_OCU0E_0X,	D_NULL,		D_NULL,		D_NULL,		BUS_ADDRESS_STROBE_OUT	},		/* 外部バスアドレスストローブ出力 */
	{ D_HIGH_PXX0,	D_HIGH_PXX0,	NO_MASK,			EPFR17_SEG8E_HIGH,	EPFR29_OCU0E_0X,	D_NULL,		D_NULL,		D_NULL,		LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_LOW_PXX0,	D_LOW_PXX0,		EPFR01_ICU6E_00,	EPFR17_SEG8E_LOW,	EPFR29_OCU0E_0X,	D_NULL,		D_NULL,		D_NULL,		ICU_IN					},		/* インプットキャプチャ入力ch.6 リロケーション0 */
	{ D_HIGH_PXX0,	D_HIGH_PXX0,	NO_MASK,			EPFR17_SEG8E_LOW,	EPFR29_OCU0E_10,	D_NULL,		D_NULL,		D_NULL,		OCU_OUT					}		/* アウトプットコンペア出力ch.0 リロケーション1 */
};

/* 『P021機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P021[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE2] =
{	
	/* DDR02 		PFR02			EPFR01				EPFR17				EPFR29				EPFR32		EPFR33		EPFR39		ID */
	{ D_LOW_PXX1,	D_LOW_PXX1,		NO_MASK,			EPFR17_SEG9E_LOW,	EPFR29_OCU1E_0X,	D_NULL,		D_NULL,		D_NULL,		PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX1,	D_LOW_PXX1,		NO_MASK,			EPFR17_SEG9E_LOW,	EPFR29_OCU1E_0X,	D_NULL,		D_NULL,		D_NULL,		PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX1,	D_HIGH_PXX1,	NO_MASK,			EPFR17_SEG9E_LOW,	EPFR29_OCU1E_0X,	D_NULL,		D_NULL,		D_NULL,		BUS_CHIPSET_OUT		},		/* 外部バスチップセレクト0 出力 */
	{ D_HIGH_PXX1,	D_HIGH_PXX1,	NO_MASK,			EPFR17_SEG9E_HIGH,	EPFR29_OCU1E_0X,	D_NULL,		D_NULL,		D_NULL,		LCD_SEG_DUTY_OUT	},		/* LCDC セグメント(Duty)出力 */
	{ D_LOW_PXX1,	D_LOW_PXX1,		EPFR01_ICU7E_00,	EPFR17_SEG9E_LOW,	EPFR29_OCU1E_0X,	D_NULL,		D_NULL,		D_NULL,		ICU_IN				},		/* インプットキャプチャ入力ch.7 リロケーション0 */
	{ D_HIGH_PXX1,	D_HIGH_PXX1,	NO_MASK,			EPFR17_SEG9E_LOW,	EPFR29_OCU1E_10,	D_NULL,		D_NULL,		D_NULL,		OCU_OUT				}		/* アウトプットコンペア出力ch.1 リロケーション1 */
};

/* 『P022機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P022[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE2] =
{
	/* DDR02 		PFR02			EPFR01		EPFR17					EPFR29				EPFR32		EPFR33		EPFR39				ID */
	{ D_LOW_PXX2,	D_LOW_PXX2,		D_NULL,		EPFR17_SEG10E_LOW,		EPFR29_OCU2E_0X,	D_NULL,		D_NULL,		NO_MASK,			PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX2,	D_LOW_PXX2,		D_NULL,		EPFR17_SEG10E_LOW,		EPFR29_OCU2E_0X,	D_NULL,		D_NULL,		NO_MASK,			PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX2,	D_HIGH_PXX2,	D_NULL,		EPFR17_SEG10E_LOW,		EPFR29_OCU2E_0X,	D_NULL,		D_NULL,		NO_MASK,			BUS_CHIPSET_OUT		},		/* 外部バスチップセレクト1 出力 */
	{ D_HIGH_PXX2,	D_HIGH_PXX2,	D_NULL,		EPFR17_SEG10E_HIGH,		EPFR29_OCU2E_0X,	D_NULL,		D_NULL,		NO_MASK,			LCD_SEG_DUTY_OUT	},		/* LCDC セグメント(Duty)出力 */
	{ D_LOW_PXX2,	D_LOW_PXX2,		D_NULL,		EPFR17_SEG10E_LOW,		EPFR29_OCU2E_0X,	D_NULL,		D_NULL,		EPFR39_ICU8E_00,	ICU_IN				},		/* インプットキャプチャ入力ch.8 リロケーション0 */
	{ D_HIGH_PXX2,	D_HIGH_PXX2,	D_NULL,		EPFR17_SEG10E_LOW,		EPFR29_OCU2E_10,	D_NULL,		D_NULL,		NO_MASK,			OCU_OUT				}		/* アウトプットコンペア出力ch.2 リロケーション1 */
};

/* 『P023機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P023[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE2] =
{
	/* DDR02 		PFR02			EPFR01		EPFR17					EPFR29				EPFR32		EPFR33		EPFR39				ID */
	{ D_LOW_PXX3,	D_LOW_PXX3,		D_NULL,		EPFR17_SEG11E_LOW,		EPFR29_OCU3E_0X,	D_NULL,		D_NULL,		NO_MASK,			PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX3,	D_LOW_PXX3,		D_NULL,		EPFR17_SEG11E_LOW,		EPFR29_OCU3E_0X,	D_NULL,		D_NULL,		NO_MASK,			PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX3,	D_HIGH_PXX3,	D_NULL,		EPFR17_SEG11E_LOW,		EPFR29_OCU3E_0X,	D_NULL,		D_NULL,		NO_MASK,			BUS_READ_STROBE_OUT	},		/* 外部バスリードストローブ出力 */
	{ D_HIGH_PXX3,	D_HIGH_PXX3,	D_NULL,		EPFR17_SEG11E_HIGH,		EPFR29_OCU3E_0X,	D_NULL,		D_NULL,		NO_MASK,			LCD_SEG_DUTY_OUT	},		/* LCDC セグメント(Duty)出力 */
	{ D_LOW_PXX3,	D_LOW_PXX3,		D_NULL,		EPFR17_SEG11E_LOW,		EPFR29_OCU3E_0X,	D_NULL,		D_NULL,		EPFR39_ICU9E_00,	ICU_IN				},		/* インプットキャプチャ入力ch.9 リロケーション0 */
	{ D_HIGH_PXX3,	D_HIGH_PXX3,	D_NULL,		EPFR17_SEG11E_LOW,		EPFR29_OCU3E_10,	D_NULL,		D_NULL,		NO_MASK,			OCU_OUT				}		/* アウトプットコンペア出力ch.3 リロケーション1 */
};

/* 『P024機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P024[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE2] =
{
	/* DDR02 		PFR02			EPFR01		EPFR17					EPFR29		EPFR32					EPFR33		EPFR39				ID */
	{ D_LOW_PXX4,	D_LOW_PXX4,		D_NULL,		EPFR17_SEG12E_LOW,		D_NULL,		EPFR32_OCU11E_X0,		D_NULL,		NO_MASK,			PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX4,	D_LOW_PXX4,		D_NULL,		EPFR17_SEG12E_LOW,		D_NULL,		EPFR32_OCU11E_X0,		D_NULL,		NO_MASK,			PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX4,	D_HIGH_PXX4,	D_NULL,		EPFR17_SEG12E_LOW,		D_NULL,		EPFR32_OCU11E_X0,		D_NULL,		NO_MASK,			BUS_WRITE_STROBE_OUT	},		/* 外部バスライトストローブ0 出力 */
	{ D_HIGH_PXX4,	D_HIGH_PXX4,	D_NULL,		EPFR17_SEG12E_HIGH,		D_NULL,		EPFR32_OCU11E_X0,		D_NULL,		NO_MASK,			LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_LOW_PXX4,	D_LOW_PXX4,		D_NULL,		EPFR17_SEG12E_LOW,		D_NULL,		EPFR32_OCU11E_X0,		D_NULL,		EPFR39_ICU10E_00,	ICU_IN					},		/* インプットキャプチャ入力ch.10 リロケーション0 */
	{ D_HIGH_PXX4,	D_HIGH_PXX4,	D_NULL,		EPFR17_SEG12E_LOW,		D_NULL,		EPFR32_OCU11E_01,		D_NULL,		NO_MASK,			OCU_OUT					}		/* アウトプットコンペア出力ch.11 リロケーション0 */
};

/* 『P025機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P025[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE2] =
{
	/* DDR02 		PFR02			EPFR01		EPFR17					EPFR29		EPFR32					EPFR33		EPFR39				ID */
	{ D_LOW_PXX5,	D_LOW_PXX5,		D_NULL,		EPFR17_SEG13E_LOW,		D_NULL,		EPFR32_OCU10E_X0,		D_NULL,		NO_MASK,			PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX5,	D_LOW_PXX5,		D_NULL,		EPFR17_SEG13E_LOW,		D_NULL,		EPFR32_OCU10E_X0,		D_NULL,		NO_MASK,			PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX5,	D_HIGH_PXX5,	D_NULL,		EPFR17_SEG13E_LOW,		D_NULL,		EPFR32_OCU10E_X0,		D_NULL,		NO_MASK,			BUS_WRITE_STROBE_OUT	},		/* 外部バスライトストローブ1 出力 */
	{ D_HIGH_PXX5,	D_HIGH_PXX5,	D_NULL,		EPFR17_SEG13E_HIGH,		D_NULL,		EPFR32_OCU10E_X0,		D_NULL,		NO_MASK,			LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_LOW_PXX5,	D_LOW_PXX5,		D_NULL,		EPFR17_SEG13E_LOW,		D_NULL,		EPFR32_OCU10E_X0,		D_NULL,		EPFR39_ICU11E_00,	ICU_IN					},		/* インプットキャプチャ入力ch.11 リロケーション0 */
	{ D_HIGH_PXX5,	D_HIGH_PXX5,	D_NULL,		EPFR17_SEG13E_LOW,		D_NULL,		EPFR32_OCU10E_01,		D_NULL,		NO_MASK,			OCU_OUT					}		/* アウトプットコンペア出力ch.10 リロケーション0 */
};

/* 『P026機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P026[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE2] =
{
	/* DDR02 		PFR02			EPFR01		EPFR17					EPFR29		EPFR32					EPFR33				EPFR39		ID */
	{ D_LOW_PXX6,	D_LOW_PXX6,		D_NULL,		EPFR17_SEG14E_LOW,		D_NULL,		EPFR32_OCU9E_X0,		NO_MASK,			D_NULL,		PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX6,	D_LOW_PXX6,		D_NULL,		EPFR17_SEG14E_LOW,		D_NULL,		EPFR32_OCU9E_X0,		NO_MASK,			D_NULL,		PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX6,	D_HIGH_PXX6,	D_NULL,		EPFR17_SEG14E_LOW,		D_NULL,		EPFR32_OCU9E_X0,		NO_MASK,			D_NULL,		BUS_ADDRESS_OUT		},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX6,	D_HIGH_PXX6,	D_NULL,		EPFR17_SEG14E_HIGH,		D_NULL,		EPFR32_OCU9E_X0,		NO_MASK,			D_NULL,		LCD_SEG_DUTY_OUT	},		/* LCDC セグメント(Duty)出力 */
	{ D_NULL,		D_NULL,			D_NULL,		D_NULL,					D_NULL,		D_NULL,					D_NULL,				D_NULL,		NO_USE				},		/* HS_SPI SSEL3 出力 */
	{ D_LOW_PXX6,	D_LOW_PXX6,		D_NULL,		EPFR17_SEG14E_LOW,		D_NULL,		EPFR32_OCU9E_X0,		EPFR33_SIN6E_HIGH,	D_NULL,		LIN_UART_SERIAL_IN	},		/* LIN_UART シリアル入力ch.6 リロケーション1 */
	{ D_HIGH_PXX6,	D_HIGH_PXX6,	D_NULL,		EPFR17_SEG14E_LOW,		D_NULL,		EPFR32_OCU9E_01,		NO_MASK,			D_NULL,		OCU_OUT				}		/* アウトプットコンペア出力ch.9 リロケーション0 */
};

/* 『P027機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P027[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE2] =
{
	/* DDR02 		PFR02			EPFR01		EPFR17					EPFR29		EPFR32					EPFR33				EPFR39		ID */
	{ D_LOW_PXX7,	D_LOW_PXX7,		D_NULL,		EPFR17_SEG15E_LOW,		D_NULL,		EPFR32_OCU8E_X0,		EPFR33_SOT6E_0X,	D_NULL,		PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX7,	D_LOW_PXX7,		D_NULL,		EPFR17_SEG15E_LOW,		D_NULL,		EPFR32_OCU8E_X0,		EPFR33_SOT6E_0X,	D_NULL,		PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX7,	D_HIGH_PXX7,	D_NULL,		EPFR17_SEG15E_LOW,		D_NULL,		EPFR32_OCU8E_X0,		EPFR33_SOT6E_0X,	D_NULL,		BUS_ADDRESS_OUT		},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX7,	D_HIGH_PXX7,	D_NULL,		EPFR17_SEG15E_HIGH,		D_NULL,		EPFR32_OCU8E_X0,		EPFR33_SOT6E_0X,	D_NULL,		LCD_SEG_DUTY_OUT	},		/* LCDC セグメント(Duty)出力 */
	{ D_NULL,		D_NULL,			D_NULL,		D_NULL,					D_NULL,		D_NULL,					D_NULL,				D_NULL,		NO_USE				},		/* HS_SPI SSEL3 出力 */
	{ D_HIGH_PXX7,	D_HIGH_PXX7,	D_NULL,		EPFR17_SEG15E_LOW,		D_NULL,		EPFR32_OCU8E_X0,		EPFR33_SOT6E_1X,	D_NULL,		LIN_UART_SERIAL_OUT	},		/* LIN_UART シリアル出力ch.6 リロケーション1 */
	{ D_HIGH_PXX7,	D_HIGH_PXX7,	D_NULL,		EPFR17_SEG15E_LOW,		D_NULL,		EPFR32_OCU8E_01,		EPFR33_SOT6E_0X,	D_NULL,		OCU_OUT				}		/* アウトプットコンペア出力ch.8 リロケーション0 */
};

/* 『P030機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P030[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03		EPFR015		EPFR18					EPFR31		EPFR33				EPFR34		EPFR37		EPFR38		ID */
	{ D_LOW_PXX0,	D_LOW_PXX0,		D_NULL,		EPFR18_SEG16E_LOW,		D_NULL,		EPFR33_SCK6E_0X,	D_NULL,		D_NULL,		D_NULL,		PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX0,	D_LOW_PXX0,		D_NULL,		EPFR18_SEG16E_LOW,		D_NULL,		EPFR33_SCK6E_0X,	D_NULL,		D_NULL,		D_NULL,		PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX0,	D_HIGH_PXX0,	D_NULL,		EPFR18_SEG16E_LOW,		D_NULL,		EPFR33_SCK6E_0X,	D_NULL,		D_NULL,		D_NULL,		BUS_ADDRESS_OUT		},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX0,	D_HIGH_PXX0,	D_NULL,		EPFR18_SEG16E_HIGH,		D_NULL,		EPFR33_SCK6E_0X,	D_NULL,		D_NULL,		D_NULL,		LCD_SEG_DUTY_OUT	},		/* LCDC セグメント(Duty)出力 */
	{ D_NULL,		D_NULL,			D_NULL,		D_NULL,					D_NULL,		D_NULL,				D_NULL,		D_NULL,		D_NULL,		NO_USE				},		/* HS_SPI SCLK 入出力 */
	{ D_LOW_PXX0,	D_HIGH_PXX0,	D_NULL,		EPFR18_SEG16E_LOW,		D_NULL,		EPFR33_SCK6E_10,	D_NULL,		D_NULL,		D_NULL,		LIN_UART_SCK_IN		},		/* LIN_UART シリアルクロック入力ch.6 リロケーション1 */
	{ D_HIGH_PXX0,	D_HIGH_PXX0,	D_NULL,		EPFR18_SEG16E_LOW,		D_NULL,		EPFR33_SCK6E_10,	D_NULL,		D_NULL,		D_NULL,		LIN_UART_SCK_OUT	}		/* LIN_UART シリアルクロック出力ch.6 リロケーション1 */
};

/* 『P031機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P031[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03		EPFR015		EPFR18					EPFR31		EPFR33		EPFR34		EPFR37		EPFR38				ID */
	{ D_LOW_PXX1,	D_LOW_PXX1,		D_NULL,		EPFR18_SEG17E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX1,	D_LOW_PXX1,		D_NULL,		EPFR18_SEG17E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX1,	D_HIGH_PXX1,	D_NULL,		EPFR18_SEG17E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			BUS_ADDRESS_OUT				},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX1,	D_HIGH_PXX1,	D_NULL,		EPFR18_SEG17E_HIGH,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			LCD_SEG_DUTY_OUT			},		/* LCDC セグメント(Duty)出力 */
	{ D_NULL,		D_NULL,			D_NULL,		D_NULL,					D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,				NO_USE						},		/* HS_SPI SCLK 入出力 */
	{ D_LOW_PXX1,	D_LOW_PXX1,		D_NULL,		EPFR18_SEG17E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR38_SIN9E_LOW,	MULTI_FUNCTION_SERIAL_IN	}		/* マルチファンクションシリアル入力ch.9 リロケーション0 */
};

/* 『P032機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P032[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03			EPFR015		EPFR18					EPFR31				EPFR33		EPFR34		EPFR37		EPFR38				ID	 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		D_NULL,		EPFR18_SEG18E_LOW,		EPFR31_OCU7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SOT9E_X0,	PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		D_NULL,		EPFR18_SEG18E_LOW,		EPFR31_OCU7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SOT9E_X0,	PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		EPFR18_SEG18E_LOW,		EPFR31_OCU7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SOT9E_X0,	BUS_ADDRESS_OUT				},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		EPFR18_SEG18E_HIGH,		EPFR31_OCU7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SOT9E_X0,	LCD_SEG_DUTY_OUT			},		/* LCDC セグメント(Duty)出力 */
	{ D_NULL,			D_NULL,			D_NULL,		D_NULL,					D_NULL,				D_NULL,		D_NULL,		D_NULL,		D_NULL,				NO_USE						},		/* HS_SPI SCLK 入出力 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		EPFR18_SEG18E_LOW,		EPFR31_OCU7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SOT9E_01,	MULTI_FUNCTION_SERIAL_OUT	},		/* マルチファンクションシリアル出力ch.9 リロケーション0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		EPFR18_SEG18E_LOW,		EPFR31_OCU7E_01,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SOT9E_X0,	OCU_OUT						}		/* アウトプットコンペア出力ch.7 リロケーション0 */
};

/* 『P033機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P033[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03			EPFR015		EPFR18					EPFR31				EPFR33		EPFR34		EPFR37		EPFR38				ID	 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		EPFR18_SEG19E_LOW,		EPFR31_OCU6E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SCK9E_X0,	PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		D_NULL,		EPFR18_SEG19E_LOW,		EPFR31_OCU6E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SCK9E_X0,	PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		EPFR18_SEG19E_LOW,		EPFR31_OCU6E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SCK9E_X0,	BUS_ADDRESS_OUT			},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		EPFR18_SEG19E_HIGH,		EPFR31_OCU6E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SCK9E_X0,	LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_NULL,			D_NULL,			D_NULL,		D_NULL,					D_NULL,				D_NULL,		D_NULL,		D_NULL,		D_NULL,				NO_USE					},		/* HS_SPI SCLK 入出力 */
	{ D_LOW_PXX3,		D_HIGH_PXX3,	D_NULL,		EPFR18_SEG19E_LOW,		EPFR31_OCU6E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SCK9E_01,	MULTI_FUNCTION_SCK_IN	},		/* マルチファンクションシリアルクロック入力ch.9 リロケーション0 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		EPFR18_SEG19E_LOW,		EPFR31_OCU6E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SCK9E_01,	MULTI_FUNCTION_SCK_OUT	},		/* マルチファンクションシリアルクロック出力ch.9 リロケーション0 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		EPFR18_SEG19E_LOW,		EPFR31_OCU6E_01,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SCK9E_X0,	OCU_OUT					}		/* アウトプットコンペア出力ch.6 リロケーション0 */
};

/* 『P034機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P034[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03			EPFR015		EPFR18					EPFR31				EPFR33		EPFR34		EPFR37				EPFR38		ID	 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR18_SEG20E_LOW,		EPFR31_OCU5E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR18_SEG20E_LOW,		EPFR31_OCU5E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		EPFR18_SEG20E_LOW,		EPFR31_OCU5E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		BUS_ADDRESS_OUT				},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		EPFR18_SEG20E_HIGH,		EPFR31_OCU5E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		LCD_SEG_DUTY_OUT			},		/* LCDC セグメント(Duty)出力 */
	{ D_NULL,			D_NULL,			D_NULL,		D_NULL,					D_NULL,				D_NULL,		D_NULL,		D_NULL,				D_NULL,		NO_USE						},		/* HS_SPI SCLK 入出力 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR18_SEG20E_LOW,		EPFR31_OCU5E_0X,	D_NULL,		D_NULL,		EPFR37_SIN8E_00,	D_NULL,		MULTI_FUNCTION_SERIAL_IN	},		/* マルチファンクションシリアル入力ch.8 リロケーション0 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		EPFR18_SEG20E_LOW,		EPFR31_OCU5E_10,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		OCU_OUT						}		/* アウトプットコンペア出力ch.5 リロケーション1 */
};

/* 『P035機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P035[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03			EPFR015		EPFR18					EPFR31				EPFR33		EPFR34		EPFR37				EPFR38		ID	 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		EPFR18_SEG21E_LOW,		EPFR31_OCU4E_0X,	D_NULL,		D_NULL,		EPFR37_SOT8E_XX0,	D_NULL,		PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		D_NULL,		EPFR18_SEG21E_LOW,		EPFR31_OCU4E_0X,	D_NULL,		D_NULL,		EPFR37_SOT8E_XX0,	D_NULL,		PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR18_SEG21E_LOW,		EPFR31_OCU4E_0X,	D_NULL,		D_NULL,		EPFR37_SOT8E_XX0,	D_NULL,		BUS_ADDRESS_OUT				},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR18_SEG21E_HIGH,		EPFR31_OCU4E_0X,	D_NULL,		D_NULL,		EPFR37_SOT8E_XX0,	D_NULL,		LCD_SEG_DUTY_OUT			},		/* LCDC セグメント(Duty)出力 */
	{ D_NULL,			D_NULL,			D_NULL,		D_NULL,					D_NULL,				D_NULL,		D_NULL,		D_NULL,				D_NULL,		NO_USE						},		/* HS_SPI SCLK 入出力 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR18_SEG21E_LOW,		EPFR31_OCU4E_0X,	D_NULL,		D_NULL,		EPFR37_SOT8E_001,	D_NULL,		MULTI_FUNCTION_SERIAL_OUT	},		/* マルチファンクションシリアル出力ch.8 リロケーション0 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR18_SEG21E_LOW,		EPFR31_OCU4E_10,	D_NULL,		D_NULL,		EPFR37_SOT8E_XX0,	D_NULL,		OCU_OUT						}		/* アウトプットコンペア出力ch.4 リロケーション1 */
};

/* 『P036機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P036[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03			EPFR015					EPFR18					EPFR31		EPFR33		EPFR34		EPFR37				EPFR38		ID	 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR15_PPG11E_X0,		EPFR18_SEG22E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_XX0,	D_NULL,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		EPFR15_PPG11E_X0,		EPFR18_SEG22E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_XX0,	D_NULL,		PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR15_PPG11E_X0,		EPFR18_SEG22E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_XX0,	D_NULL,		BUS_ADDRESS_OUT			},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR15_PPG11E_X0,		EPFR18_SEG22E_HIGH,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_XX0,	D_NULL,		LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR15_PPG11E_01,		EPFR18_SEG22E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_XX0,	D_NULL,		PPG_OUT					},		/* PPG 出力ch.11 リロケーション0 */
	{ D_NULL,			D_NULL,			D_NULL,					D_NULL,					D_NULL,		D_NULL,		D_NULL,		D_NULL,				D_NULL,		NO_USE					},		/* HS_SPI SCLK 入出力 */
	{ D_LOW_PXX6,		D_HIGH_PXX6,	EPFR15_PPG11E_X0,		EPFR18_SEG22E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_001,	D_NULL,		MULTI_FUNCTION_SCK_IN	},		/* マルチファンクションシリアルクロック入力ch.8 リロケーション0 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR15_PPG11E_X0,		EPFR18_SEG22E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_001,	D_NULL,		MULTI_FUNCTION_SCK_OUT	}		/* マルチファンクションシリアルクロック出力ch.8 リロケーション0 */
};

/* 『P037機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P037[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03			EPFR015					EPFR18					EPFR31		EPFR33		EPFR34				EPFR37		EPFR38		ID	 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR15_PPG12E_X0,		EPFR18_SEG23E_LOW,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		EPFR15_PPG12E_X0,		EPFR18_SEG23E_LOW,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR15_PPG12E_X0,		EPFR18_SEG23E_LOW,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		BUS_ADDRESS_OUT		},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR15_PPG12E_X0,		EPFR18_SEG23E_HIGH,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		LCD_SEG_DUTY_OUT	},		/* LCDC セグメント(Duty)出力 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR15_PPG12E_X0,		EPFR18_SEG23E_HIGH,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		LCD_SEG_STATIC_OUT	},		/* LCDC セグメント(Static)出力 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR15_PPG12E_01,		EPFR18_SEG23E_LOW,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		PPG_OUT				},		/* PPG 出力ch.12 リロケーション0 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR15_PPG12E_X0,		EPFR18_SEG23E_LOW,		D_NULL,		D_NULL,		EPFR34_SIN7E_LOW,	D_NULL,		D_NULL,		LIN_UART_SERIAL_IN	}		/* LIN_UART シリアル入力ch.7 リロケーション0 */
};

/* 『P040機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P040[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30		EPFR31		EPFR34				EPFR37		EPFR40		EPFR41		EPFR45				ID	 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR19_SEG24E_LOW,		D_NULL,		D_NULL,		EPFR34_SOT7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG13E_X0,	PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		EPFR19_SEG24E_LOW,		D_NULL,		D_NULL,		EPFR34_SOT7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG13E_X0,	PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR19_SEG24E_LOW,		D_NULL,		D_NULL,		EPFR34_SOT7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG13E_X0,	BUS_ADDRESS_OUT			},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR19_SEG24E_HIGH,		D_NULL,		D_NULL,		EPFR34_SOT7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG13E_X0,	LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR19_SEG24E_HIGH,		D_NULL,		D_NULL,		EPFR34_SOT7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG13E_X0,	LCD_SEG_STATIC_OUT		},		/* LCDC セグメント(Static)出力 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR19_SEG24E_LOW,		D_NULL,		D_NULL,		EPFR34_SOT7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG13E_01,	PPG_OUT					},		/* PPG 出力ch.13 リロケーション0 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR19_SEG24E_LOW,		D_NULL,		D_NULL,		EPFR34_SOT7E_X1,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG13E_X0,	LIN_UART_SERIAL_OUT		}		/* LIN_UART シリアル出力ch.7 リロケーション0 */
};

/* 『P041機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P041[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30		EPFR31		EPFR34				EPFR37		EPFR40		EPFR41		EPFR45					ID	 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR19_SEG25E_LOW,		D_NULL,		D_NULL,		EPFR34_SCK7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_X0,		PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		EPFR19_SEG25E_LOW,		D_NULL,		D_NULL,		EPFR34_SCK7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_X0,		PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR19_SEG25E_LOW,		D_NULL,		D_NULL,		EPFR34_SCK7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_X0,		BUS_ADDRESS_OUT		},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR19_SEG25E_HIGH,		D_NULL,		D_NULL,		EPFR34_SCK7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_X0,		LCD_SEG_DUTY_OUT	},		/* LCDC セグメント(Duty)出力 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR19_SEG25E_HIGH,		D_NULL,		D_NULL,		EPFR34_SCK7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_X0,		LCD_SEG_STATIC_OUT	},		/* LCDC セグメント(Static)出力 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR19_SEG25E_LOW,		D_NULL,		D_NULL,		EPFR34_SCK7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_01,		PPG_OUT				},		/* PPG 出力ch.14 リロケーション0 */
	{ D_LOW_PXX1,		D_HIGH_PXX1,	EPFR19_SEG25E_LOW,		D_NULL,		D_NULL,		EPFR34_SCK7E_01,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_X0,		LIN_UART_SCK_IN		},		/* LIN_UART シリアルクロック入力ch.7 リロケーション0 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR19_SEG25E_LOW,		D_NULL,		D_NULL,		EPFR34_SCK7E_01,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_X0,		LIN_UART_SCK_OUT	}		/* LIN_UART シリアルクロック出力ch.7 リロケーション0 */
};

/* 『P042機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P042[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30		EPFR31		EPFR34		EPFR37		EPFR40				EPFR41		EPFR45					ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR19_SEG26E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR45_PPG15E_X0,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		EPFR19_SEG26E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR45_PPG15E_X0,		PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR19_SEG26E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR45_PPG15E_X0,		BUS_ADDRESS_OUT			},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR19_SEG26E_HIGH,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR45_PPG15E_X0,		LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR19_SEG26E_HIGH,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR45_PPG15E_X0,		LCD_SEG_STATIC_OUT		},		/* LCDC セグメント(Static)出力 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR19_SEG26E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR45_PPG15E_01,		PPG_OUT					},		/* PPG 出力ch.15 リロケーション0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR19_SEG26E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR40_AIN0E_00,	D_NULL,		EPFR45_PPG15E_X0,		UPDOWN_COUNTER_AIN_IN	}		/* アップダウンカウンタAIN 入力ch.0 リロケーション0 */
};

/* 『P043機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P043[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30					EPFR31					EPFR34		EPFR37		EPFR40				EPFR41		EPFR45		ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR19_SEG27E_LOW,		EPFR30_SGA4E_X0,		EPFR31_OCU6E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		EPFR19_SEG27E_LOW,		EPFR30_SGA4E_X0,		EPFR31_OCU6E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR19_SEG27E_LOW,		EPFR30_SGA4E_X0,		EPFR31_OCU6E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		BUS_ADDRESS_OUT			},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR19_SEG27E_HIGH,		EPFR30_SGA4E_X0,		EPFR31_OCU6E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR19_SEG27E_HIGH,		EPFR30_SGA4E_X0,		EPFR31_OCU6E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		LCD_SEG_STATIC_OUT		},		/* LCDC セグメント(Static)出力 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR19_SEG27E_LOW,		EPFR30_SGA4E_X0,		EPFR31_OCU6E_0X,		D_NULL,		D_NULL,		EPFR40_BIN0E_00,	D_NULL,		D_NULL,		UPDOWN_COUNTER_BIN_IN	},		/* アップダウンカウンタBIN 入力ch.0 リロケーション0 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR19_SEG27E_LOW,		EPFR30_SGA4E_01,		EPFR31_OCU6E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		SOUND_GENERATOR_SGA_OUT	},		/* サウンドジェネレータSGA 出力ch.4 リロケーション0 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR19_SEG27E_LOW,		EPFR30_SGA4E_X0,		EPFR31_OCU6E_10,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		OCU_OUT					}		/* アウトプットコンペア出力ch.6 リロケーション1 */
};

/* 『P044機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P044[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30					EPFR31				EPFR34		EPFR37		EPFR40				EPFR41		EPFR45		ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR19_SEG28E_LOW,		EPFR30_SGO4E_X0,		EPFR31_OCU7E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		EPFR19_SEG28E_LOW,		EPFR30_SGO4E_X0,		EPFR31_OCU7E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR19_SEG28E_LOW,		EPFR30_SGO4E_X0,		EPFR31_OCU7E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		BUS_ADDRESS_OUT			},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR19_SEG28E_HIGH,		EPFR30_SGO4E_X0,		EPFR31_OCU7E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR19_SEG28E_HIGH,		EPFR30_SGO4E_X0,		EPFR31_OCU7E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		LCD_SEG_STATIC_OUT		},		/* LCDC セグメント(Static)出力 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR19_SEG28E_LOW,		EPFR30_SGO4E_X0,		EPFR31_OCU7E_0X,	D_NULL,		D_NULL,		EPFR40_ZIN0E_00,	D_NULL,		D_NULL,		UPDOWN_COUNTER_ZIN_IN	},		/* アップダウンカウンタZIN 入力ch.0 リロケーション0 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR19_SEG28E_LOW,		EPFR30_SGO4E_01,		EPFR31_OCU7E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		SOUND_GENERATOR_SGO_OUT	},		/* サウンドジェネレータSGA 出力ch.4 リロケーション0 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR19_SEG28E_LOW,		EPFR30_SGO4E_X0,		EPFR31_OCU7E_10,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		OCU_OUT					}		/* アウトプットコンペア出力ch.7 リロケーション1 */
};

/* 『P045機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P045[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30		EPFR31		EPFR34		EPFR37				EPFR40		EPFR41				EPFR45		ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR19_SEG29E_LOW,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		EPFR19_SEG29E_LOW,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR19_SEG29E_LOW,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		BUS_ADDRESS_OUT				},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR19_SEG29E_HIGH,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		LCD_SEG_DUTY_OUT			},		/* LCDC セグメント(Duty)出力 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR19_SEG29E_HIGH,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		LCD_SEG_STATIC_OUT			},		/* LCDC セグメント(Static)出力 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR19_SEG29E_LOW,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR41_AIN1E_LOW,	D_NULL,		UPDOWN_COUNTER_AIN_IN		},		/* アップダウンカウンタAIN 入力ch.1 リロケーション0 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR19_SEG29E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SIN8E_10,	D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SERIAL_IN	}		/* マルチファンクションシリアル入力ch.8 リロケーション2 */
};

/* 『P046機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P046[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30		EPFR31		EPFR34		EPFR37				EPFR40		EPFR41				EPFR45		ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR19_SEG30E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SOT8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		EPFR19_SEG30E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SOT8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR19_SEG30E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SOT8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		BUS_ADDRESS_OUT				},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR19_SEG30E_HIGH,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SOT8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		LCD_SEG_DUTY_OUT			},		/* LCDC セグメント(Duty)出力 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR19_SEG30E_HIGH,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SOT8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		LCD_SEG_STATIC_OUT			},		/* LCDC セグメント(Static)出力 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR19_SEG30E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SOT8E_0XX,	D_NULL,		EPFR41_BIN1E_LOW,	D_NULL,		UPDOWN_COUNTER_BIN_IN		},		/* アップダウンカウンタBIN 入力ch.1 リロケーション0 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR19_SEG30E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SOT8E_100,	D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SERIAL_OUT	}		/* マルチファンクションシリアル出力ch.8 リロケーション2 */
};

/* 『P047機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P047[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30		EPFR31		EPFR34		EPFR37				EPFR40		EPFR41				EPFR45		ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR19_SEG31E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		EPFR19_SEG31E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR19_SEG31E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		BUS_ADDRESS_OUT			},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR19_SEG31E_HIGH,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		LCD_SEG_DUTY_OUT		},		/* LCDC セグメント(Duty)出力 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR19_SEG31E_HIGH,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		LCD_SEG_STATIC_OUT		},		/* LCDC セグメント(Static)出力 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR19_SEG31E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_0XX,	D_NULL,		EPFR41_ZIN1E_LOW,	D_NULL,		UPDOWN_COUNTER_ZIN_IN	},		/* アップダウンカウンタZIN 入力ch.1 リロケーション0 */
	{ D_LOW_PXX7,		D_HIGH_PXX7,	EPFR19_SEG31E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_100,	D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SCK_IN	},		/* マルチファンクションシリアルクロック入力ch.8 リロケーション2 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR19_SEG31E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_100,	D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SCK_OUT	}		/* マルチファンクションシリアルクロック出力ch.8 リロケーション2 */
};

/* 『P050機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P050[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20					EPFR28		EPFR32				ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR20_COM0E_LOW,		D_NULL,		EPFR32_OCU8E_0X,	PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		EPFR20_COM0E_LOW,		D_NULL,		EPFR32_OCU8E_0X,	PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR20_COM0E_LOW,		D_NULL,		EPFR32_OCU8E_0X,	BUS_ADDRESS_OUT				},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR20_COM0E_HIGH,		D_NULL,		EPFR32_OCU8E_0X,	LCD_SEG_DUTY_OUT_COM_OUT	},		/* LCDC セグメント(Duty)コモン出力 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR20_COM0E_LOW,		D_NULL,		EPFR32_OCU8E_10,	OCU_OUT						}		/* アウトプットコンペア出力ch.8 リロケーション1 */
};

/* 『P051機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P051[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20					EPFR28		EPFR32				ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR20_COM1E_LOW,		D_NULL,		EPFR32_OCU9E_0X,	PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		EPFR20_COM1E_LOW,		D_NULL,		EPFR32_OCU9E_0X,	PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR20_COM1E_LOW,		D_NULL,		EPFR32_OCU9E_0X,	BUS_ADDRESS_OUT				},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR20_COM1E_HIGH,		D_NULL,		EPFR32_OCU9E_0X,	LCD_SEG_DUTY_OUT_COM_OUT	},		/* LCDC セグメント(Duty)コモン出力 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR20_COM1E_LOW,		D_NULL,		EPFR32_OCU9E_10,	OCU_OUT						}		/* アウトプットコンペア出力ch.9 リロケーション1 */
};

/* 『P052機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P052[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20					EPFR28		EPFR32				ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR20_COM2E_LOW,		D_NULL,		EPFR32_OCU10E_0X,	PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		EPFR20_COM2E_LOW,		D_NULL,		EPFR32_OCU10E_0X,	PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR20_COM2E_LOW,		D_NULL,		EPFR32_OCU10E_0X,	BUS_ADDRESS_OUT				},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR20_COM2E_HIGH,		D_NULL,		EPFR32_OCU10E_0X,	LCD_SEG_DUTY_OUT_COM_OUT	},		/* LCDC セグメント(Duty)コモン出力 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR20_COM2E_LOW,		D_NULL,		EPFR32_OCU10E_10,	OCU_OUT						}		/* アウトプットコンペア出力ch.10 リロケーション1 */
};

/* 『P053機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P053[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20					EPFR28		EPFR32				ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR20_COM3E_LOW,		D_NULL,		EPFR32_OCU11E_0X,	PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		EPFR20_COM3E_LOW,		D_NULL,		EPFR32_OCU11E_0X,	PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR20_COM3E_LOW,		D_NULL,		EPFR32_OCU11E_0X,	BUS_ADDRESS_OUT				},		/* 外部バスアドレス出力 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR20_COM3E_HIGH,		D_NULL,		EPFR32_OCU11E_0X,	LCD_SEG_DUTY_OUT_COM_OUT	},		/* LCDC セグメント(Duty)コモン出力 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR20_COM3E_LOW,		D_NULL,		EPFR32_OCU11E_10,	OCU_OUT						}		/* アウトプットコンペア出力ch.11 リロケーション1 */
};

/* 『P054機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P054[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20				EPFR28					EPFR32		ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR20_V0E_LOW,		NO_MASK,				D_NULL,		PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		EPFR20_V0E_LOW,		NO_MASK,				D_NULL,		PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR20_V0E_LOW,		NO_MASK,				D_NULL,		BUS_CLK_OUT			},		/* 外部バスクロック出力 */
	{ D_LOW_PXX4,		D_HIGH_PXX4,	EPFR20_V0E_HIGH,	NO_MASK,				D_NULL,		LCD_V_IN			},		/* LCDC 基準電圧V0 入力 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR20_V0E_LOW,		EPFR28_FRCK0E_HIGH,		D_NULL,		FREE_RUN_TIMCLK_IN	}		/* フリーランタイマクロック入力ch.0 リロケーション1 */
};

/* 『P055機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P055[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20				EPFR28					EPFR32		ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR20_V1E_LOW,		NO_MASK,				D_NULL,		PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		EPFR20_V1E_LOW,		NO_MASK,				D_NULL,		PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR20_V1E_LOW,		NO_MASK,				D_NULL,		BUS_CHIPSET_OUT		},		/* 外部バスチップセレクト2 出力 */
	{ D_LOW_PXX5,		D_HIGH_PXX5,	EPFR20_V1E_HIGH,	NO_MASK,				D_NULL,		LCD_V_IN			},		/* LCDC 基準電圧V1 入力 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR20_V1E_LOW,		EPFR28_FRCK1E_HIGH,		D_NULL,		FREE_RUN_TIMCLK_IN	}		/* フリーランタイマクロック入力ch.1 リロケーション1 */
};

/* 『P056機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P056[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20				EPFR28					EPFR32		ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR20_V2E_LOW,		NO_MASK,				D_NULL,		PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		EPFR20_V2E_LOW,		NO_MASK,				D_NULL,		PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR20_V2E_LOW,		NO_MASK,				D_NULL,		BUS_CHIPSET_OUT		},		/* 外部バスチップセレクト3 出力 */
	{ D_LOW_PXX6,		D_HIGH_PXX6,	EPFR20_V2E_HIGH,	NO_MASK,				D_NULL,		LCD_V_IN			},		/* LCDC 基準電圧V2 入力 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR20_V2E_LOW,		EPFR28_FRCK2E_HIGH,		D_NULL,		FREE_RUN_TIMCLK_IN	}		/* フリーランタイマクロック入力ch.2 リロケーション1 */
};

/* 『P057機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P057[PORT_FUNC_CHANGE_ID_NUM_4][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20				EPFR28					EPFR32		ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR20_V3E_LOW,		NO_MASK,				D_NULL,		PORT_IN				},		/* 汎用入出力ポート (入力のみ 出力なし) */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR20_V3E_LOW,		NO_MASK,				D_NULL,		BUS_RDY_IN			},		/* 外部バスRDY 入力 */
	{ D_LOW_PXX7,		D_HIGH_PXX7,	EPFR20_V3E_HIGH,	NO_MASK,				D_NULL,		LCD_V_IN			},		/* LCDC 基準電圧V3 入力 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR20_V3E_LOW,		EPFR28_FRCK3E_HIGH,		D_NULL,		FREE_RUN_TIMCLK_IN	}		/* フリーランタイマクロック入力ch.3 リロケーション1 */
};

/* 『P060機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P060[PORT_FUNC_CHANGE_ID_NUM_4][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35		EPFR36		EPFR38		EPFR40		EPFR41		ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR21_PWM1P0E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN			},		/* 汎用入力ポート */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		EPFR21_PWM1P0E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT		},		/* 汎用出力ポート */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR21_PWM1P0E_HIGH,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		SMC_OUT			},		/* SMC 出力ch.0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR21_PWM1P0E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ADC_ANALOG_IN	}		/* ADC アナログ入力ch.8 */
};

/* 『P061機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P061[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35		EPFR36					EPFR38		EPFR40		EPFR41		ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR21_PWM1M0E_LOW,		D_NULL,		NO_MASK,				D_NULL,		D_NULL,		D_NULL,		PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		EPFR21_PWM1M0E_LOW,		D_NULL,		NO_MASK,				D_NULL,		D_NULL,		D_NULL,		PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR21_PWM1M0E_HIGH,	D_NULL,		NO_MASK,				D_NULL,		D_NULL,		D_NULL,		SMC_OUT						},		/* SMC 出力ch.0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR21_PWM1M0E_LOW,		D_NULL,		NO_MASK,				D_NULL,		D_NULL,		D_NULL,		ADC_ANALOG_IN				},		/* ADC アナログ入力ch.9 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR21_PWM1M0E_LOW,		D_NULL,		EPFR36_SIN1E_HIGH,		D_NULL,		D_NULL,		D_NULL,		MULTI_FUNCTION_SERIAL_IN	}		/* マルチファンクションシリアル入力ch.1 リロケーション1 */
};

/* 『P062機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P062[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35		EPFR36					EPFR38		EPFR40		EPFR41				ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR21_PWM2P0E_LOW,		D_NULL,		EPFR36_SOT1E_0X,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		EPFR21_PWM2P0E_LOW,		D_NULL,		EPFR36_SOT1E_0X,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR21_PWM2P0E_HIGH,	D_NULL,		EPFR36_SOT1E_0X,		D_NULL,		D_NULL,		NO_MASK,			SMC_OUT						},		/* SMC 出力ch.0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR21_PWM2P0E_LOW,		D_NULL,		EPFR36_SOT1E_0X,		D_NULL,		D_NULL,		NO_MASK,			ADC_ANALOG_IN				},		/* ADC アナログ入力ch.10 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR21_PWM2P0E_LOW,		D_NULL,		EPFR36_SOT1E_0X,		D_NULL,		D_NULL,		EPFR41_ZIN1E_HIGH,	UPDOWN_COUNTER_ZIN_IN		},		/* アップダウンカウンタZIN 入力ch.1 リロケーション1 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR21_PWM2P0E_LOW,		D_NULL,		EPFR36_SOT1E_10,		D_NULL,		D_NULL,		NO_MASK,			MULTI_FUNCTION_SERIAL_OUT	}		/* マルチファンクションシリアル出力ch.1 リロケーション1 */
};

/* 『P063機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P063[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35		EPFR36					EPFR38		EPFR40		EPFR41				ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR21_PWM2M0E_LOW,		D_NULL,		EPFR36_SCK1E_0X,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		EPFR21_PWM2M0E_LOW,		D_NULL,		EPFR36_SCK1E_0X,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR21_PWM2M0E_HIGH,	D_NULL,		EPFR36_SCK1E_0X,		D_NULL,		D_NULL,		NO_MASK,			SMC_OUT					},		/* SMC 出力ch.0 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR21_PWM2M0E_LOW,		D_NULL,		EPFR36_SCK1E_0X,		D_NULL,		D_NULL,		NO_MASK,			ADC_ANALOG_IN			},		/* ADC アナログ入力ch.11 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR21_PWM2M0E_LOW,		D_NULL,		EPFR36_SCK1E_0X,		D_NULL,		D_NULL,		EPFR41_BIN1E_HIGH,	UPDOWN_COUNTER_BIN_IN	},		/* アップダウンカウンタBIN 入力ch.1 リロケーション1 */
	{ D_LOW_PXX3,		D_HIGH_PXX3,	EPFR21_PWM2M0E_LOW,		D_NULL,		EPFR36_SCK1E_10,		D_NULL,		D_NULL,		NO_MASK,			MULTI_FUNCTION_SCK_IN	},		/* マルチファンクションシリアルクロック入力ch.1 リロケーション1 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR21_PWM2M0E_LOW,		D_NULL,		EPFR36_SCK1E_10,		D_NULL,		D_NULL,		NO_MASK,			MULTI_FUNCTION_SCK_OUT	}		/* マルチファンクションシリアルクロック出力ch.1 リロケーション1 */
};

/* 『P064機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P064[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35				EPFR36		EPFR38		EPFR40		EPFR41				ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR21_PWM1P1E_LOW,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		EPFR21_PWM1P1E_LOW,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR21_PWM1P1E_HIGH,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			SMC_OUT						},		/* SMC 出力ch.1 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR21_PWM1P1E_LOW,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			ADC_ANALOG_IN				},		/* ADC アナログ入力ch.12 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR21_PWM1P1E_LOW,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR41_AIN1E_HIGH,	UPDOWN_COUNTER_AIN_IN		},		/* アップダウンカウンタAIN 入力ch.1 リロケーション1 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR21_PWM1P1E_LOW,		EPFR35_SIN0E_HIGH,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			MULTI_FUNCTION_SERIAL_IN	}		/* マルチファンクションシリアル入力ch.0 リロケーション1 */
};

/* 『P065機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P065[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35					EPFR36		EPFR38		EPFR40				EPFR41		ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR21_PWM1M1E_LOW,		EPFR35_SOT0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		EPFR21_PWM1M1E_LOW,		EPFR35_SOT0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR21_PWM1M1E_HIGH,	EPFR35_SOT0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		SMC_OUT						},		/* SMC 出力ch.1 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR21_PWM1M1E_LOW,		EPFR35_SOT0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		ADC_ANALOG_IN				},		/* ADC アナログ入力ch.13 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR21_PWM1M1E_LOW,		EPFR35_SOT0E_0X,		D_NULL,		D_NULL,		EPFR40_ZIN0E_01,	D_NULL,		UPDOWN_COUNTER_ZIN_IN		},		/* アップダウンカウンタZIN 入力ch.0 リロケーション1 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR21_PWM1M1E_LOW,		EPFR35_SOT0E_10,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SERIAL_OUT	}		/* マルチファンクションシリアル出力ch.0 リロケーション1 */
};

/* 『P066機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P066[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35					EPFR36		EPFR38		EPFR40				EPFR41		ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR21_PWM2P1E_LOW,		EPFR35_SCK0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		EPFR21_PWM2P1E_LOW,		EPFR35_SCK0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR21_PWM2P1E_HIGH,	EPFR35_SCK0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		SMC_OUT					},		/* SMC 出力ch.1 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR21_PWM2P1E_LOW,		EPFR35_SCK0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		ADC_ANALOG_IN			},		/* ADC アナログ入力ch.14 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR21_PWM2P1E_LOW,		EPFR35_SCK0E_0X,		D_NULL,		D_NULL,		EPFR40_BIN0E_01,	D_NULL,		UPDOWN_COUNTER_BIN_IN	},		/* アップダウンカウンタBIN 入力ch.0 リロケーション1 */
	{ D_LOW_PXX6,		D_HIGH_PXX6,	EPFR21_PWM2P1E_LOW,		EPFR35_SCK0E_10,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SCK_IN	},		/* マルチファンクションシリアルクロック入力ch.0 リロケーション1 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR21_PWM2P1E_LOW,		EPFR35_SCK0E_10,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SCK_OUT	}		/* マルチファンクションシリアルクロック出力ch.0 リロケーション1 */
};

/* 『P067機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P067[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35		EPFR36		EPFR38				EPFR40				EPFR41		ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR21_PWM2M1E_LOW,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		EPFR21_PWM2M1E_LOW,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR21_PWM2M1E_HIGH,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		SMC_OUT						},		/* SMC 出力ch.1 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR21_PWM2M1E_LOW,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		ADC_ANALOG_IN				},		/* ADC アナログ入力ch.15 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR21_PWM2M1E_LOW,		D_NULL,		D_NULL,		NO_MASK,			EPFR40_AIN0E_01,	D_NULL,		UPDOWN_COUNTER_AIN_IN		},		/* アップダウンカウンタAIN 入力ch.0 リロケーション1 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR21_PWM2M1E_LOW,		D_NULL,		D_NULL,		EPFR38_SIN9E_HIGH,	NO_MASK,			D_NULL,		MULTI_FUNCTION_SERIAL_IN	}		/* マルチファンクションシリアル入力ch.9 リロケーション1 */
};

/* 『P070機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P070[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07			EPFR01		EPFR15		EPFR22					EPFR34		EPFR37		EPFR38					EPFR39		EPFR45		ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR22_PWM1P2E_LOW,		D_NULL,		D_NULL,		EPFR38_SOT9E_0X,		D_NULL,		D_NULL,		PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR22_PWM1P2E_LOW,		D_NULL,		D_NULL,		EPFR38_SOT9E_0X,		D_NULL,		D_NULL,		PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		D_NULL,		EPFR22_PWM1P2E_HIGH,	D_NULL,		D_NULL,		EPFR38_SOT9E_0X,		D_NULL,		D_NULL,		SMC_OUT						},		/* SMC 出力ch.2 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR22_PWM1P2E_LOW,		D_NULL,		D_NULL,		EPFR38_SOT9E_0X,		D_NULL,		D_NULL,		ADC_ANALOG_IN				},		/* ADC アナログ入力ch.16 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		D_NULL,		EPFR22_PWM1P2E_LOW,		D_NULL,		D_NULL,		EPFR38_SOT9E_10,		D_NULL,		D_NULL,		MULTI_FUNCTION_SERIAL_OUT	}		/* マルチファンクションシリアル出力ch.9 リロケーション1 */
};

/* 『P071機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P071[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07				EPFR01		EPFR15		EPFR22					EPFR34		EPFR37		EPFR38					EPFR39		EPFR45		ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,			D_NULL,		D_NULL,		EPFR22_PWM1M2E_LOW,		D_NULL,		D_NULL,		EPFR38_SCK9E_0X,		D_NULL,		D_NULL,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX1,		D_LOW_PXX1,			D_NULL,		D_NULL,		EPFR22_PWM1M2E_LOW,		D_NULL,		D_NULL,		EPFR38_SCK9E_0X,		D_NULL,		D_NULL,		PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,		D_NULL,		D_NULL,		EPFR22_PWM1M2E_HIGH,	D_NULL,		D_NULL,		EPFR38_SCK9E_0X,		D_NULL,		D_NULL,		SMC_OUT					},		/* SMC 出力ch.2 */
	{ D_LOW_PXX1,		D_LOW_PXX1,			D_NULL,		D_NULL,		EPFR22_PWM1M2E_LOW,		D_NULL,		D_NULL,		EPFR38_SCK9E_0X,		D_NULL,		D_NULL,		ADC_ANALOG_IN			},		/* ADC アナログ入力ch.17 */
	{ D_LOW_PXX1,		D_HIGH_PXX1,		D_NULL,		D_NULL,		EPFR22_PWM1M2E_LOW,		D_NULL,		D_NULL,		EPFR38_SCK9E_10,		D_NULL,		D_NULL,		MULTI_FUNCTION_SCK_IN	},		/* マルチファンクションシリアルクロック入力ch.9 リロケーション1 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,		D_NULL,		D_NULL,		EPFR22_PWM1M2E_LOW,		D_NULL,		D_NULL,		EPFR38_SCK9E_10,		D_NULL,		D_NULL,		MULTI_FUNCTION_SCK_OUT	}		/* マルチファンクションシリアルクロック出力ch.9 リロケーション1 */
};

/* 『P072機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P072[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07				EPFR01		EPFR15		EPFR22					EPFR34		EPFR37				EPFR38		EPFR39					EPFR45		ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR22_PWM2P2E_LOW,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,				D_NULL,		PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR22_PWM2P2E_LOW,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,				D_NULL,		PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,		D_NULL,		D_NULL,		EPFR22_PWM2P2E_HIGH,	D_NULL,		NO_MASK,			D_NULL,		NO_MASK,				D_NULL,		SMC_OUT						},		/* SMC 出力ch.2 */
	{ D_LOW_PXX2,		D_LOW_PXX2	,		D_NULL,		D_NULL,		EPFR22_PWM2P2E_LOW,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,				D_NULL,		ADC_ANALOG_IN				},		/* ADC アナログ入力ch.18 */
	{ D_LOW_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR22_PWM2P2E_LOW,		D_NULL,		NO_MASK,			D_NULL,		EPFR39_ICU11E_01,		D_NULL,		ICU_IN						},		/* インプットキャプチャ入力ch.11 リロケーション1 */
	{ D_LOW_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR22_PWM2P2E_LOW,		D_NULL,		EPFR37_SIN8E_01,	D_NULL,		NO_MASK,				D_NULL,		MULTI_FUNCTION_SERIAL_IN	}		/* マルチファンクションシリアル入力ch.8 リロケーション1 */
};

/* 『P073機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P073[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07				EPFR01		EPFR15		EPFR22					EPFR34		EPFR37					EPFR38		EPFR39					EPFR45		ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,			D_NULL,		D_NULL,		EPFR22_PWM2M2E_LOW,		D_NULL,		EPFR37_SOT8E_X0X,		D_NULL,		NO_MASK,				D_NULL,		PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX3,		D_LOW_PXX3,			D_NULL,		D_NULL,		EPFR22_PWM2M2E_LOW,		D_NULL,		EPFR37_SOT8E_X0X,		D_NULL,		NO_MASK,				D_NULL,		PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,		D_NULL,		D_NULL,		EPFR22_PWM2M2E_HIGH,	D_NULL,		EPFR37_SOT8E_X0X,		D_NULL,		NO_MASK,				D_NULL,		SMC_OUT						},		/* SMC 出力ch.2 */
	{ D_LOW_PXX3,		D_LOW_PXX3,			D_NULL,		D_NULL,		EPFR22_PWM2M2E_LOW,		D_NULL,		EPFR37_SOT8E_X0X,		D_NULL,		NO_MASK,				D_NULL,		ADC_ANALOG_IN				},		/* ADC アナログ入力ch.19 */
	{ D_LOW_PXX3,		D_LOW_PXX3,			D_NULL,		D_NULL,		EPFR22_PWM2M2E_LOW,		D_NULL,		EPFR37_SOT8E_X0X,		D_NULL,		EPFR39_ICU10E_01,		D_NULL,		ICU_IN						},		/* インプットキャプチャ入力ch.10 リロケーション1 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,		D_NULL,		D_NULL,		EPFR22_PWM2M2E_LOW,		D_NULL,		EPFR37_SOT8E_010,		D_NULL,		NO_MASK,				D_NULL,		MULTI_FUNCTION_SERIAL_OUT	}		/* マルチファンクションシリアル出力ch.8 リロケーション1 */
};

/* 『P074機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P074[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07				EPFR01		EPFR15					EPFR22						EPFR34		EPFR37					EPFR38		EPFR39				EPFR45		ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,			D_NULL,		EPFR15_PPG12E_0X,		EPFR22_PWM1P3E_LOW,			D_NULL,		EPFR37_SCK8E_X0X,		D_NULL,		NO_MASK,			D_NULL,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX4,		D_LOW_PXX4,			D_NULL,		EPFR15_PPG12E_0X,		EPFR22_PWM1P3E_LOW,			D_NULL,		EPFR37_SCK8E_X0X,		D_NULL,		NO_MASK,			D_NULL,		PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,		D_NULL,		EPFR15_PPG12E_0X,		EPFR22_PWM1P3E_HIGH,		D_NULL,		EPFR37_SCK8E_X0X,		D_NULL,		NO_MASK,			D_NULL,		SMC_OUT					},		/* SMC 出力ch.3 */
	{ D_LOW_PXX4,		D_LOW_PXX4,			D_NULL,		EPFR15_PPG12E_0X,		EPFR22_PWM1P3E_LOW,			D_NULL,		EPFR37_SCK8E_X0X,		D_NULL,		NO_MASK,			D_NULL,		ADC_ANALOG_IN			},		/* ADC アナログ入力ch.20 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,		D_NULL,		EPFR15_PPG12E_1X,		EPFR22_PWM1P3E_LOW,			D_NULL,		EPFR37_SCK8E_X0X,		D_NULL,		NO_MASK,			D_NULL,		PPG_OUT					},		/* PPG 出力ch.12 リロケーション1 */
	{ D_LOW_PXX4,		D_LOW_PXX4,			D_NULL,		EPFR15_PPG12E_0X,		EPFR22_PWM1P3E_LOW,			D_NULL,		EPFR37_SCK8E_X0X,		D_NULL,		EPFR39_ICU9E_01,	D_NULL,		ICU_IN					},		/* インプットキャプチャ入力ch.9 リロケーション1 */
	{ D_LOW_PXX4,		D_HIGH_PXX4,		D_NULL,		EPFR15_PPG12E_0X,		EPFR22_PWM1P3E_LOW,			D_NULL,		EPFR37_SCK8E_010,		D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SCK_IN	},		/* マルチファンクションシリアルクロック入力ch.8 リロケーション1 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,		D_NULL,		EPFR15_PPG12E_0X,		EPFR22_PWM1P3E_LOW,			D_NULL,		EPFR37_SCK8E_010,		D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SCK_OUT	},		/* マルチファンクションシリアルクロック出力ch.8 リロケーション1 */
};

/* 『P075機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P075[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07				EPFR01		EPFR15		EPFR22					EPFR34				EPFR37		EPFR38		EPFR39				EPFR45					ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,			D_NULL,		D_NULL,		EPFR22_PWM1M3E_LOW,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			EPFR45_PPG13E_0X,		PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX5,		D_LOW_PXX5,			D_NULL,		D_NULL,		EPFR22_PWM1M3E_LOW,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			EPFR45_PPG13E_0X,		PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,		D_NULL,		D_NULL,		EPFR22_PWM1M3E_HIGH,	NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			EPFR45_PPG13E_0X,		SMC_OUT				},		/* SMC 出力ch.3 */
	{ D_LOW_PXX5,		D_LOW_PXX5,			D_NULL,		D_NULL,		EPFR22_PWM1M3E_LOW,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			EPFR45_PPG13E_0X,		ADC_ANALOG_IN		},		/* ADC アナログ入力ch.21 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,		D_NULL,		D_NULL,		EPFR22_PWM1M3E_LOW,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			EPFR45_PPG13E_1X,		PPG_OUT				},		/* PPG 出力ch.13 リロケーション1 */
	{ D_LOW_PXX5,		D_LOW_PXX5,			D_NULL,		D_NULL,		EPFR22_PWM1M3E_LOW,		NO_MASK,			D_NULL,		D_NULL,		EPFR39_ICU8E_01,	EPFR45_PPG13E_0X,		ICU_IN				},		/* インプットキャプチャ入力ch.8 リロケーション1 */
	{ D_LOW_PXX5,		D_LOW_PXX5,			D_NULL,		D_NULL,		EPFR22_PWM1M3E_LOW,		EPFR34_SIN7E_HIGH,	D_NULL,		D_NULL,		NO_MASK,			EPFR45_PPG13E_0X,		LIN_UART_SERIAL_IN	}		/* LIN_UART シリアル入力ch.7 リロケーション1 */
};

/* 『P076機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P076[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07				EPFR01				EPFR15		EPFR22					EPFR34					EPFR37		EPFR38		EPFR39		EPFR45					ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,			NO_MASK,			D_NULL,		EPFR22_PWM2P3E_LOW,		EPFR34_SOT7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_0X,		PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX6,		D_LOW_PXX6,			NO_MASK,			D_NULL,		EPFR22_PWM2P3E_LOW,		EPFR34_SOT7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_0X,		PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,		NO_MASK,			D_NULL,		EPFR22_PWM2P3E_HIGH,	EPFR34_SOT7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_0X,		SMC_OUT				},		/* SMC 出力ch.3 */
	{ D_LOW_PXX6,		D_LOW_PXX6,			NO_MASK,			D_NULL,		EPFR22_PWM2P3E_LOW,		EPFR34_SOT7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_0X,		ADC_ANALOG_IN		},		/* ADC アナログ入力ch.22 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,		NO_MASK,			D_NULL,		EPFR22_PWM2P3E_LOW,		EPFR34_SOT7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_1X,		PPG_OUT				},		/* PPG 出力ch.14 リロケーション1 */
	{ D_LOW_PXX6,		D_LOW_PXX6,			EPFR01_ICU7E_01,	D_NULL,		EPFR22_PWM2P3E_LOW,		EPFR34_SOT7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_0X,		ICU_IN				},		/* インプットキャプチャ入力ch.7 リロケーション1 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,		NO_MASK,			D_NULL,		EPFR22_PWM2P3E_LOW,		EPFR34_SOT7E_1X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_0X,		LIN_UART_SERIAL_OUT	}		/* LIN_UART シリアル出力ch.7 リロケーション1 */
};

/* 『P077機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P077[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07				EPFR01				EPFR15		EPFR22					EPFR34					EPFR37		EPFR38		EPFR39		EPFR45					ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,			NO_MASK,			D_NULL,		EPFR22_PWM2M3E_LOW,		EPFR34_SCK7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_0X,		PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX7,		D_LOW_PXX7,			NO_MASK,			D_NULL,		EPFR22_PWM2M3E_LOW,		EPFR34_SCK7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_0X,		PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,		NO_MASK,			D_NULL,		EPFR22_PWM2M3E_HIGH,	EPFR34_SCK7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_0X,		SMC_OUT				},		/* SMC 出力ch.3 */
	{ D_LOW_PXX7,		D_LOW_PXX7,			NO_MASK,			D_NULL,		EPFR22_PWM2M3E_LOW,		EPFR34_SCK7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_0X,		ADC_ANALOG_IN		},		/* ADC アナログ入力ch.23 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,		NO_MASK,			D_NULL,		EPFR22_PWM2M3E_LOW,		EPFR34_SCK7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_1X,		PPG_OUT				},		/* PPG 出力ch.15 リロケーション1 */
	{ D_LOW_PXX7,		D_LOW_PXX7,			EPFR01_ICU6E_01,	D_NULL,		EPFR22_PWM2M3E_LOW,		EPFR34_SCK7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_0X,		ICU_IN				},		/* インプットキャプチャ入力ch.6 リロケーション1 */
	{ D_LOW_PXX7,		D_HIGH_PXX7,		NO_MASK,			D_NULL,		EPFR22_PWM2M3E_LOW,		EPFR34_SCK7E_10,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_0X,		LIN_UART_SCK_IN		},		/* LIN_UART シリアルクロック入力ch.7 リロケーション1 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,		NO_MASK,			D_NULL,		EPFR22_PWM2M3E_LOW,		EPFR34_SCK7E_10,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_0X,		LIN_UART_SCK_OUT	}		/* LIN_UART シリアルクロック出力ch.7 リロケーション1 */
};

/* 『P080機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P080[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08				EPFR00		EPFR01		EPFR23					EPFR33				EPFR40				EPFR45					EPFR46		ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,			D_NULL,		D_NULL,		EPFR23_PWM1P4E_LOW,		NO_MASK,			NO_MASK,			EPFR45_PPG16E_LOW,		D_NULL,		PORT_IN 				},		/* 汎用入力ポート */
	{ D_HIGH_PXX0,		D_LOW_PXX0,			D_NULL,		D_NULL,		EPFR23_PWM1P4E_LOW,		NO_MASK,			NO_MASK,			EPFR45_PPG16E_LOW,		D_NULL,		PORT_OUT 				},		/* 汎用出力ポート */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,		D_NULL,		D_NULL,		EPFR23_PWM1P4E_HIGH,	NO_MASK,			NO_MASK,			EPFR45_PPG16E_LOW,		D_NULL,		SMC_OUT 				},		/* SMC 出力ch.4 */
	{ D_LOW_PXX0,		D_LOW_PXX0,			D_NULL,		D_NULL,		EPFR23_PWM1P4E_LOW,		NO_MASK,			NO_MASK,			EPFR45_PPG16E_LOW,		D_NULL,		ADC_ANALOG_IN			},		/* ADC アナログ入力ch.24 */
	{ D_LOW_PXX0,		D_LOW_PXX0,			D_NULL,		D_NULL,		EPFR23_PWM1P4E_LOW,		EPFR33_SIN6E_LOW,	NO_MASK,			EPFR45_PPG16E_LOW,		D_NULL,		LIN_UART_SERIAL_IN		},		/* LIN_UART シリアル入力ch.6 リロケーション0 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,		D_NULL,		D_NULL,		EPFR23_PWM1P4E_LOW,		NO_MASK,			NO_MASK,			EPFR45_PPG16E_HIGH,		D_NULL,		PPG_OUT					},		/* PPG 出力ch.16 リロケーション0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,			D_NULL,		D_NULL,		EPFR23_PWM1P4E_LOW,		NO_MASK,			EPFR40_AIN0E_10,	EPFR45_PPG16E_LOW,		D_NULL,		UPDOWN_COUNTER_AIN_IN	}		/* アップダウンカウンタAIN 入力ch.0 リロケーション2 */
};

/* 『P081機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P081[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08				EPFR00		EPFR01		EPFR23					EPFR33				EPFR40				EPFR45					EPFR46		ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,			D_NULL,		D_NULL,		EPFR23_PWM1M4E_LOW,		EPFR33_SOT6E_X0,	NO_MASK,			EPFR45_PPG17E_LOW,		D_NULL,		PORT_IN				 	},		/* 汎用入力ポート */
	{ D_HIGH_PXX1,		D_LOW_PXX1,			D_NULL,		D_NULL,		EPFR23_PWM1M4E_LOW,		EPFR33_SOT6E_X0,	NO_MASK,			EPFR45_PPG17E_LOW,		D_NULL,		PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,		D_NULL,		D_NULL,		EPFR23_PWM1M4E_HIGH,	EPFR33_SOT6E_X0,	NO_MASK,			EPFR45_PPG17E_LOW,		D_NULL,		SMC_OUT					},		/* SMC 出力ch.4 */
	{ D_LOW_PXX1,		D_LOW_PXX1,			D_NULL,		D_NULL,		EPFR23_PWM1M4E_LOW,		EPFR33_SOT6E_X0,	NO_MASK,			EPFR45_PPG17E_LOW,		D_NULL,		ADC_ANALOG_IN 			},		/* ADC アナログ入力ch.25 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,		D_NULL,		D_NULL,		EPFR23_PWM1M4E_LOW,		EPFR33_SOT6E_X1,	NO_MASK,			EPFR45_PPG17E_LOW,		D_NULL,		LIN_UART_SERIAL_OUT		},		/* LIN_UART シリアル出力ch.6 リロケーション0 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,		D_NULL,		D_NULL,		EPFR23_PWM1M4E_LOW,		EPFR33_SOT6E_X0,	NO_MASK,			EPFR45_PPG17E_HIGH,		D_NULL,		PPG_OUT					},		/* PPG 出力ch.17 リロケーション0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,			D_NULL,		D_NULL,		EPFR23_PWM1M4E_LOW,		EPFR33_SOT6E_X0,	EPFR40_BIN0E_10,	EPFR45_PPG17E_LOW,		D_NULL,		UPDOWN_COUNTER_BIN_IN	}		/* アップダウンカウンタBIN 入力ch.0 リロケーション2 */
};

/* 『P082機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P082[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08				EPFR00		EPFR01		EPFR23					EPFR33				EPFR40				EPFR45		EPFR46					ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR23_PWM2P4E_LOW,		EPFR33_SCK6E_X0,	NO_MASK,			D_NULL,		EPFR46_PPG18E_LOW,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR23_PWM2P4E_LOW,		EPFR33_SCK6E_X0,	NO_MASK,			D_NULL,		EPFR46_PPG18E_LOW,		PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,		D_NULL,		D_NULL,		EPFR23_PWM2P4E_HIGH,	EPFR33_SCK6E_X0,	NO_MASK,			D_NULL,		EPFR46_PPG18E_LOW,		SMC_OUT					},		/* SMC 出力ch.4 */
	{ D_LOW_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR23_PWM2P4E_LOW,		EPFR33_SCK6E_X0,	NO_MASK,			D_NULL,		EPFR46_PPG18E_LOW,		ADC_ANALOG_IN			},		/* ADC アナログ入力ch.26 */
	{ D_LOW_PXX2,		D_HIGH_PXX2,		D_NULL,		D_NULL,		EPFR23_PWM2P4E_LOW,		EPFR33_SCK6E_01,	NO_MASK,			D_NULL,		EPFR46_PPG18E_LOW,		LIN_UART_SCK_IN			},		/* LIN_UART シリアルクロック入力ch.6 リロケーション0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,		D_NULL,		D_NULL,		EPFR23_PWM2P4E_LOW,		EPFR33_SCK6E_01,	NO_MASK,			D_NULL,		EPFR46_PPG18E_LOW,		LIN_UART_SCK_OUT		},		/* LIN_UART シリアルクロック出力ch.6 リロケーション0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,		D_NULL,		D_NULL,		EPFR23_PWM2P4E_LOW,		EPFR33_SCK6E_X0,	NO_MASK,			D_NULL,		EPFR46_PPG18E_HIGH,		PPG_OUT					},		/* PPG 出力ch.18 リロケーション0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR23_PWM2P4E_LOW,		EPFR33_SCK6E_X0,	EPFR40_ZIN0E_10,	D_NULL,		EPFR46_PPG18E_LOW,		UPDOWN_COUNTER_ZIN_IN	}		/* アップダウンカウンタZIN 入力ch.0 リロケーション2 */
};

/* 『P083機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P083[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08			EPFR00				EPFR01		EPFR23					EPFR33		EPFR40		EPFR45		EPFR46					ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		EPFR23_PWM2M4E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG19E_LOW,		PORT_IN			},		/* 汎用入力ポート */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		EPFR23_PWM2M4E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG19E_LOW,		PORT_OUT		},		/* 汎用出力ポート */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	NO_MASK,			D_NULL,		EPFR23_PWM2M4E_HIGH,	D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG19E_LOW,		SMC_OUT			},		/* SMC 出力ch.4 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		EPFR23_PWM2M4E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG19E_LOW,		ADC_ANALOG_IN	},		/* ADC アナログ入力ch.27 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR00_ICU0E_10,	D_NULL,		EPFR23_PWM2M4E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG19E_LOW,		ICU_IN			},		/* インプットキャプチャ入力ch.0 リロケーション2 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	NO_MASK,			D_NULL,		EPFR23_PWM2M4E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG19E_HIGH,		PPG_OUT			}		/* PPG 出力ch.19 リロケーション0 */
};

/* 『P084機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P084[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08			EPFR00				EPFR01		EPFR23					EPFR33		EPFR40		EPFR45		EPFR46					ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		EPFR23_PWM1P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG20E_LOW,		PORT_IN			},		/* 汎用入力ポート */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		EPFR23_PWM1P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG20E_LOW,		PORT_OUT		},		/* 汎用出力ポート */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	NO_MASK,			D_NULL,		EPFR23_PWM1P5E_HIGH,	D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG20E_LOW,		SMC_OUT			},		/* SMC 出力ch.5 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		EPFR23_PWM1P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG20E_LOW,		ADC_ANALOG_IN	},		/* ADC アナログ入力ch.28 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR00_ICU1E_10,	D_NULL,		EPFR23_PWM1P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG20E_LOW,		ICU_IN			},		/* インプットキャプチャ入力ch.1 リロケーション2 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	NO_MASK,			D_NULL,		EPFR23_PWM1P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG20E_HIGH,		PPG_OUT			}		/* PPG 出力ch.20 リロケーション0 */
};

/* 『P085機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P085[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08				EPFR00				EPFR01		EPFR23					EPFR33		EPFR40		EPFR45		EPFR46					ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,			NO_MASK,			D_NULL,		EPFR23_PWM1M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG21E_LOW,		PORT_IN			},		/* 汎用入力ポート */
	{ D_HIGH_PXX5,		D_LOW_PXX5,			NO_MASK,			D_NULL,		EPFR23_PWM1M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG21E_LOW,		PORT_OUT		},		/* 汎用出力ポート */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,		NO_MASK,			D_NULL,		EPFR23_PWM1M5E_HIGH,	D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG21E_LOW,		SMC_OUT			},		/* SMC 出力ch.5 */
	{ D_LOW_PXX5,		D_LOW_PXX5,			NO_MASK,			D_NULL,		EPFR23_PWM1M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG21E_LOW,		ADC_ANALOG_IN	},		/* ADC アナログ入力ch.29 */
	{ D_LOW_PXX5,		D_LOW_PXX5,			EPFR00_ICU2E_10,	D_NULL,		EPFR23_PWM1M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG21E_LOW,		ICU_IN			},		/* インプットキャプチャ入力ch.2 リロケーション2 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,		NO_MASK,			D_NULL,		EPFR23_PWM1M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG21E_HIGH,		PPG_OUT			}		/* PPG 出力ch.21 リロケーション0 */
};

/* 『P086機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P086[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08			EPFR00				EPFR01		EPFR23					EPFR33		EPFR40		EPFR45		EPFR46					ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		NO_MASK,			D_NULL,		EPFR23_PWM2P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG22E_LOW,		PORT_IN			},		/* 汎用入力ポート */
	{ D_HIGH_PXX6	,	D_LOW_PXX6,		NO_MASK,			D_NULL,		EPFR23_PWM2P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG22E_LOW,		PORT_OUT		},		/* 汎用出力ポート */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	NO_MASK,			D_NULL,		EPFR23_PWM2P5E_HIGH,	D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG22E_LOW,		SMC_OUT			},		/* SMC 出力ch.5 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		NO_MASK,			D_NULL,		EPFR23_PWM2P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG22E_LOW,		ADC_ANALOG_IN	},		/* ADC アナログ入力ch.30 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR00_ICU3E_10,	D_NULL,		EPFR23_PWM2P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG22E_LOW,		ICU_IN			},		/* インプットキャプチャ入力ch.3 リロケーション2 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	NO_MASK,			D_NULL,		EPFR23_PWM2P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG22E_HIGH,		PPG_OUT			}		/* PPG 出力ch.22 リロケーション0 */
};

/* 『P087機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P087[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08			EPFR00		EPFR01				EPFR23					EPFR33		EPFR40		EPFR45		EPFR46					ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		NO_MASK,			EPFR23_PWM2M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG23E_LOW,		PORT_IN			},		/* 汎用入力ポート */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		D_NULL,		NO_MASK,			EPFR23_PWM2M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG23E_LOW,		PORT_OUT		},		/* 汎用出力ポート */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		NO_MASK,			EPFR23_PWM2M5E_HIGH,	D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG23E_LOW,		SMC_OUT			},		/* SMC 出力ch.5 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		NO_MASK,			EPFR23_PWM2M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG23E_LOW,		ADC_ANALOG_IN	},		/* ADC アナログ入力ch.30 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		EPFR01_ICU4E_10,	EPFR23_PWM2M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG23E_LOW,		ICU_IN			},		/* インプットキャプチャ入力ch.3 リロケーション2 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		NO_MASK,			EPFR23_PWM2M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG23E_HIGH,		PPG_OUT			}		/* PPG 出力ch.22 リロケーション0 */
};

/* 『P090機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P090[PORT_FUNC_CHANGE_ID_NUM_4][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00		EPFR01		EPFR02		EPFR04		EPFR05		EPFR06		EPFR07		EPFR10				EPFR13		EPFR14		EPFR15		EPFR24		EPFR27		EPFR44		ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN 	},		/* 汎用入力ポート */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT	},		/* 汎用出力ポート */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ADC_TRG_IN	},		/* ADC 外部トリガ入力 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG0E_1XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT		}		/* PPG 出力ch.0 リロケーション2 */
};

/* 『P091機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P091[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00				EPFR01		EPFR02		EPFR04				EPFR05		EPFR06				EPFR07		EPFR10		EPFR13					EPFR14		EPFR15		EPFR24		EPFR27					EPFR44			ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		EPFR04_TOT2E_X0X,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR13_PPG6E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_LOW,		NO_MASK,			PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		EPFR04_TOT2E_X0X,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR13_PPG6E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_LOW,		NO_MASK,			PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	NO_MASK,			D_NULL,		D_NULL,		EPFR04_TOT2E_X0X,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR13_PPG6E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_HIGH,		NO_MASK,			SOUND_GENERATOR_SGA_OUT	},		/* サウンドジェネレータSGA 出力ch.0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		EPFR04_TOT2E_X0X,	D_NULL,		EPFR06_SIN2E_LOW,	D_NULL,		D_NULL,		EPFR13_PPG6E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_LOW,		NO_MASK,			LIN_UART_SERIAL_IN		},		/* LIN_UART シリアル入力ch.2 リロケーション0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		EPFR04_TOT2E_X0X,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR13_PPG6E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_LOW,		EPFR44_INT12E_LOW,	INTTERUPT_REQUEST_IN	},		/* 外部割込み要求入力ch.12 リロケーション0 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	NO_MASK,			D_NULL,		D_NULL,		EPFR04_TOT2E_X1X,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR13_PPG6E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_LOW,		NO_MASK,			RELOAD_TIMER_OUT		},		/* リロードタイマ出力ch.2 リロケーション1 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR00_ICU2E_01,	D_NULL,		D_NULL,		EPFR04_TOT2E_X0X,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR13_PPG6E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_LOW,		NO_MASK,			ICU_IN					},		/* インプットキャプチャ入力ch.2 リロケーション1 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	NO_MASK,			D_NULL,		D_NULL,		EPFR04_TOT2E_X0X,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR13_PPG6E_X1X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_LOW,		NO_MASK,			PPG_OUT					}		/* PPG 出力ch.6 リロケーション1 */
};

/* 『P092機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P092[PORT_FUNC_CHANGE_ID_NUM_9][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00				EPFR01		EPFR02		EPFR04		EPFR05				EPFR06					EPFR07		EPFR10		EPFR13				EPFR14		EPFR15		EPFR24		EPFR27					EPFR44			ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_X0,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		NO_MASK,			PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_X0,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		NO_MASK,			PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_X0,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_HIGH,		NO_MASK,			SOUND_GENERATOR_SGO_OUT	},		/* サウンドジェネレータSGO 出力ch.0 */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_01,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		NO_MASK,			LIN_UART_SCK_IN			},		/* LIN_UART シリアルクロック入力ch.2 リロケーション0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_01,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		NO_MASK,			LIN_UART_SCK_OUT		},		/* LIN_UART シリアルクロック出力ch.2 リロケーション0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_X0,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		EPFR44_INT13E_LOW,	INTTERUPT_REQUEST_IN	},		/* 外部割込み要求入力ch.13 リロケーション0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X1X,	EPFR06_SCK2E_X0,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		NO_MASK,			RELOAD_TIMER_OUT		},		/* リロードタイマ出力ch.3 リロケーション1 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR00_ICU0E_01,	D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_X0,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		NO_MASK,			ICU_IN					},		/* インプットキャプチャ入力ch.0 リロケーション1 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_X0,		D_NULL,		D_NULL,		EPFR13_PPG7E_X1X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		NO_MASK,			PPG_OUT					}		/* PPG 出力ch.7 リロケーション1 */
};

/* 『P093機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P093[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00				EPFR01		EPFR02		EPFR04		EPFR05		EPFR06				EPFR07		EPFR10		EPFR13		EPFR14					EPFR15		EPFR24		EPFR27					EPFR44				ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR06_SOT2E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_X0X,		D_NULL,		D_NULL,		EPFR27_SGA1E_LOW,		NO_MASK,			PORT_IN	 					},		/* 汎用入力ポート */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR06_SOT2E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_X0X,		D_NULL,		D_NULL,		EPFR27_SGA1E_LOW,		NO_MASK,			PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR06_SOT2E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_X0X,		D_NULL,		D_NULL,		EPFR27_SGA1E_HIGH,		NO_MASK,			SOUND_GENERATOR_SGA_OUT		},		/* サウンドジェネレータSGA 出力ch.1 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR06_SOT2E_X1,	D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_X0X,		D_NULL,		D_NULL,		EPFR27_SGA1E_LOW,		NO_MASK,			LIN_UART_SERIAL_OUT			},		/* LIN_UART シリアル出力ch.2 リロケーション0 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR06_SOT2E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_X0X,		D_NULL,		D_NULL,		EPFR27_SGA1E_LOW,		EPFR44_INT14E_LOW,	INTTERUPT_REQUEST_IN		},		/* 外部割込み要求入力ch.14 リロケーション0 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR00_ICU3E_01,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR06_SOT2E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_X0X,		D_NULL,		D_NULL,		EPFR27_SGA1E_LOW,		NO_MASK,			ICU_IN						},		/* インプットキャプチャ入力ch.3 リロケーション1 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR06_SOT2E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_X1X,		D_NULL,		D_NULL,		EPFR27_SGA1E_LOW,		NO_MASK,			PPG_OUT						}		/* PPG 出力ch.8 リロケーション1 */
};

/* 『P094機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P094[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00				EPFR01		EPFR02		EPFR04		EPFR05		EPFR06		EPFR07				EPFR10		EPFR13		EPFR14					EPFR15		EPFR24		EPFR27					EPFR44				ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR14_PPG9E_X0X,		D_NULL,		D_NULL,		EPFR27_SGO1E_LOW,		NO_MASK,			PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR14_PPG9E_X0X,		D_NULL,		D_NULL,		EPFR27_SGO1E_LOW,		NO_MASK,			PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR14_PPG9E_X0X,		D_NULL,		D_NULL,		EPFR27_SGO1E_HIGH,		NO_MASK,			SOUND_GENERATOR_SGO_OUT	},		/* サウンドジェネレータSGO 出力ch.1 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SIN3E_LOW,	D_NULL,		D_NULL,		EPFR14_PPG9E_X0X,		D_NULL,		D_NULL,		EPFR27_SGO1E_LOW,		NO_MASK,			LIN_UART_SERIAL_IN		},		/* LIN_UART シリアル入力ch.3 リロケーション0 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR14_PPG9E_X0X,		D_NULL,		D_NULL,		EPFR27_SGO1E_LOW,		EPFR44_INT15E_LOW,	INTTERUPT_REQUEST_IN	},		/* 外部割込み要求入力ch.15 リロケーション0 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR00_ICU1E_01,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR14_PPG9E_X0X,		D_NULL,		D_NULL,		EPFR27_SGO1E_LOW,		NO_MASK,			ICU_IN					},		/* インプットキャプチャ入力ch.1 リロケーション1 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR14_PPG9E_X1X,		D_NULL,		D_NULL,		EPFR27_SGO1E_LOW,		NO_MASK,			PPG_OUT					}		/* PPG 出力ch.9 リロケーション1 */
};

/* 『P095機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P095[PORT_FUNC_CHANGE_ID_NUM_4][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00		EPFR01		EPFR02		EPFR04		EPFR05		EPFR06		EPFR07		EPFR10		EPFR13		EPFR14		EPFR15					EPFR24					EPFR27		EPFR44		ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_X0X,		EPFR24_TX0E_LOW,		D_NULL,		D_NULL,		PORT_IN 	},		/* 汎用入力ポート */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_X0X,		EPFR24_TX0E_LOW,		D_NULL,		D_NULL,		PORT_OUT	},		/* 汎用出力ポート */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_X0X,		EPFR24_TX0E_HIGH,		D_NULL,		D_NULL,		CAN_TX_OUT	},		/* CAN TX データ出力ch.0 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_X1X,		EPFR24_TX0E_LOW,		D_NULL,		D_NULL,		PPG_OUT 	}		/* PPG 出力ch.10 リロケーション1 */
};

/* 『P096機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P096[PORT_FUNC_CHANGE_ID_NUM_4][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00		EPFR01		EPFR02		EPFR04		EPFR05		EPFR06		EPFR07		EPFR10		EPFR13		EPFR14		EPFR15		EPFR24		EPFR27		EPFR44				ID	 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN				 },		/* 汎用入力ポート */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT			 },		/* 汎用出力ポート */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			CAN_RX_IN			 },		/* CAN RX データ入力ch.0 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR44_INT9E_LOW,	INTTERUPT_REQUEST_IN }		/* 外部割込み要求入力ch.9 リロケーション0 */
};	

/* 『P097機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P097[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00		EPFR01				EPFR02				EPFR04		EPFR05		EPFR06		EPFR07					EPFR10					EPFR13		EPFR14		EPFR15		EPFR24		EPFR27					EPFR44			ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X0,		EPFR10_PPG0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_LOW,		NO_MASK,			PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X0,		EPFR10_PPG0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_LOW,		NO_MASK,			PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X0,		EPFR10_PPG0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_HIGH,		NO_MASK,			RTC_OVERFLOW_OUT		},		/* RTC オーバフロー出力 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X1,		EPFR10_PPG0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_LOW,		NO_MASK,			LIN_UART_SERIAL_OUT		},		/* LIN_UART シリアル出力ch.3 リロケーション0 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X0,		EPFR10_PPG0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_LOW,		EPFR44_INT8E_LOW,	INTTERUPT_REQUEST_IN	},		/* 外部割込み要求入力ch.8 リロケーション0 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		NO_MASK,			EPFR02_TIN0E_00,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X0,		EPFR10_PPG0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_LOW,		NO_MASK,			RELOAD_TIMER_EVENT_IN	},		/* リロードタイマイベント入力ch.0 リロケーション0 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		EPFR01_ICU4E_01,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X0,		EPFR10_PPG0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_LOW,		NO_MASK,			ICU_IN					},		/* インプットキャプチャ入力ch.4 リロケーション1 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X0,		EPFR10_PPG0E_X1X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_LOW,		NO_MASK,			PPG_OUT					},		/* PPG 出力ch.0 リロケーション1 */
};

/* 『P100機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P100[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01		EPFR02				EPFR03		EPFR04		EPFR05		EPFR08				EPFR09		EPFR10		EPFR11		EPFR12		EPFR14					EPFR15		EPFR30		EPFR39		EPFR42		ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT 				},		/* 汎用出力ポート */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR08_SIN4E_HIGH,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		LIN_UART_SERIAL_IN 		},		/* LIN_UART シリアル入力ch.4 リロケーション1 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ADC_ANALOG_IN			},		/* ADC アナログ入力ch.0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		EPFR02_TIN0E_01,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		RELOAD_TIMER_EVENT_IN	},		/* リロードタイマイベント入力ch.0 リロケーション1 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_XX1,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT					}		/* PPG 出力ch.8 リロケーション0 */
};

/* 『P101機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P101[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01		EPFR02		EPFR03				EPFR04		EPFR05		EPFR08				EPFR09		EPFR10		EPFR11		EPFR12		EPFR14					EPFR15		EPFR30		EPFR39		EPFR42		ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR08_SOT4E_0X,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR08_SOT4E_0X,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT				},		/* 汎用出力ポート */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR08_SOT4E_1X,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		LIN_UART_SERIAL_IN		},		/* LIN_UART シリアル出力ch.4 リロケーション1 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR08_SOT4E_0X,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ADC_ANALOG_IN		 	},		/* ADC アナログ入力ch.1 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		EPFR03_TIN1E_01,	D_NULL,		D_NULL,		EPFR08_SOT4E_0X,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		RELOAD_TIMER_EVENT_IN 	},		/* リロードタイマイベント入力ch.1 リロケーション1 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR08_SOT4E_0X,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_XX1,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT				 	}		/* PPG 出力ch.8 リロケーション0 */
};

/* 『P102機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P102[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01				EPFR02		EPFR03		EPFR04				EPFR05		EPFR08					EPFR09		EPFR10		EPFR11		EPFR12		EPFR14		EPFR15					EPFR30		EPFR39		EPFR42	ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SCK4E_0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX0,		D_NULL,		D_NULL,		D_NULL,		PORT_IN 				},		/* 汎用入力ポート */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SCK4E_0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX0,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT 				},		/* 汎用出力ポート */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SCK4E_10,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX0,		D_NULL,		D_NULL,		D_NULL,		LIN_UART_SERIAL_IN  	},		/* LIN_UART シリアル入力ch.4 リロケーション1 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SCK4E_10,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX0,		D_NULL,		D_NULL,		D_NULL,		LIN_UART_SERIAL_OUT  	},		/* LIN_UART シリアル出力ch.4 リロケーション1 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SCK4E_0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX0,		D_NULL,		D_NULL,		D_NULL,		ADC_ANALOG_IN			},		/* ADC アナログ入力ch.2 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		EPFR04_TIN2E_01,	D_NULL,		EPFR08_SCK4E_0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX0,		D_NULL,		D_NULL,		D_NULL,		RELOAD_TIMER_EVENT_IN	},		/* リロードタイマイベント入力ch.2 リロケーション1 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SCK4E_0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX1,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT					},		/* PPG 出力ch.10 リロケーション0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR01_ICU6E_10,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SCK4E_0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX0,		D_NULL,		D_NULL,		D_NULL,		ICU_IN					}		/* インプットキャプチャ入力ch.6 リロケーション2 */
};

/* 『P103機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P103[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01				EPFR02		EPFR03		EPFR04		EPFR05				EPFR08		EPFR09				EPFR10					EPFR11		EPFR12		EPFR14		EPFR15		EPFR30		EPFR39		EPFR42		ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			EPFR10_PPG1E_XX0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN 				},		/* 汎用入力ポート */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			EPFR10_PPG1E_XX0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT 				},		/* 汎用出力ポート */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR09_SIN5E_HIGH,	EPFR10_PPG1E_XX0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		LIN_UART_SERIAL_IN		},		/* LIN_UART シリアル入力ch.5 リロケーション1 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			EPFR10_PPG1E_XX0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ADC_ANALOG_IN 			},		/* ADC アナログ入力ch.3 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TIN3E_01,	D_NULL,		NO_MASK,			EPFR10_PPG1E_XX0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		RELOAD_TIMER_EVENT_IN	},		/* リロードタイマイベント入力ch.3 リロケーション1 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			EPFR10_PPG1E_XX1X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT					},		/* PPG 出力ch.1 リロケーション1 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR01_ICU7E_10,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			EPFR10_PPG1E_XX0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ICU_IN					}		/* インプットキャプチャ入力ch.7 リロケーション2 */
};

/* 『P104機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P104[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01		EPFR02					EPFR03		EPFR04		EPFR05		EPFR08		EPFR09					EPFR10		EPFR11					EPFR12		EPFR14		EPFR15		EPFR30		EPFR39				EPFR42		ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR02_TOT0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SOT5E_0X,		D_NULL,		EPFR11_PPG2E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_IN 			 },		/* 汎用入力ポート */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR02_TOT0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SOT5E_0X,		D_NULL,		EPFR11_PPG2E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_OUT			 },		/* 汎用出力ポート */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		EPFR02_TOT0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SOT5E_1X,		D_NULL,		EPFR11_PPG2E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		LIN_UART_SERIAL_OUT  },		/* LIN_UART シリアル出力ch.5 リロケーション1 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR02_TOT0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SOT5E_0X,		D_NULL,		EPFR11_PPG2E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		ADC_ANALOG_IN		 },		/* ADC アナログ入力ch.4 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		EPFR02_TOT0E_X1X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SOT5E_0X,		D_NULL,		EPFR11_PPG2E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		RELOAD_TIMER_OUT	 },		/* リロードタイマ出力ch.0 リロケーション1 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		EPFR02_TOT0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SOT5E_0X,		D_NULL,		EPFR11_PPG2E_X1X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PPG_OUT 			 },		/* PPG 出力ch.2 リロケーション1 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR02_TOT0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SOT5E_0X,		D_NULL,		EPFR11_PPG2E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR39_ICU8E_10,	D_NULL,		ICU_IN				 }		/* インプットキャプチャ入力 ch.8 リロケーション2 */
};

/* 『P105機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P105[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01		EPFR02		EPFR03					EPFR04		EPFR05		EPFR08		EPFR09					EPFR10		EPFR11					EPFR12		EPFR14		EPFR15		EPFR30		EPFR39				EPFR42		ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		EPFR03_TOT1E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_0X,		D_NULL,		EPFR11_PPG3E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_IN 		 },		/* 汎用入力ポート */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		EPFR03_TOT1E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_0X,		D_NULL,		EPFR11_PPG3E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_OUT		 },		/* 汎用出力ポート */
	{ D_LOW_PXX5,		D_HIGH_PXX5,	D_NULL,		D_NULL,		EPFR03_TOT1E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_10,		D_NULL,		EPFR11_PPG3E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		LIN_UART_SCK_IN  },		/* LIN_UART シリアルクロック入力ch.5 リロケーション1 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		D_NULL,		EPFR03_TOT1E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_10,		D_NULL,		EPFR11_PPG3E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		LIN_UART_SCK_OUT },		/* LIN_UART シリアルクロック出力ch.5 リロケーション1 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		EPFR03_TOT1E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_0X,		D_NULL,		EPFR11_PPG3E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		ADC_ANALOG_IN 	 },		/* ADC アナログ入力ch.5 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		D_NULL,		EPFR03_TOT1E_X1X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_0X,		D_NULL,		EPFR11_PPG3E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		RELOAD_TIMER_OUT },		/* リロードタイマ出力ch.1 リロケーション1 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		D_NULL,		EPFR03_TOT1E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_0X,		D_NULL,		EPFR11_PPG3E_X1X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PPG_OUT			 },		/* PPG 出力ch.3 リロケーション1 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		EPFR03_TOT1E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_0X,		D_NULL,		EPFR11_PPG3E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR39_ICU9E_10,	D_NULL,		ICU_IN			 }		/* インプットキャプチャ入力ch.9 リロケーション2 */
};

/* 『P106機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P106[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01		EPFR02		EPFR03		EPFR04		EPFR05		EPFR08		EPFR09		EPFR10		EPFR11		EPFR12					EPFR14		EPFR15		EPFR30					EPFR39				EPFR42		ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_X0X,		D_NULL,		D_NULL,		EPFR30_SGA4E_0X,		NO_MASK,			D_NULL,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_X0X,		D_NULL,		D_NULL,		EPFR30_SGA4E_0X,		NO_MASK,			D_NULL,		PORT_OUT 				},		/* 汎用出力ポート */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_X0X,		D_NULL,		D_NULL,		EPFR30_SGA4E_0X,		NO_MASK,			D_NULL,		ADC_ANALOG_IN			},		/* ADC アナログ入力ch.6 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_X1X,		D_NULL,		D_NULL,		EPFR30_SGA4E_0X,		NO_MASK,			D_NULL,		PPG_OUT					},		/* PPG 出力ch.4 リロケーション1 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_X0X,		D_NULL,		D_NULL,		EPFR30_SGA4E_0X,		EPFR39_ICU10E_10,	D_NULL,		ICU_IN 					},		/* インプットキャプチャ入力ch.10 リロケーション2 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_X0X,		D_NULL,		D_NULL,		EPFR30_SGA4E_10,		NO_MASK,			D_NULL,		SOUND_GENERATOR_SGA_OUT }		/* サウンドジェネレータSGA 出力ch.4 リロケーション1 */
};

/* 『P107機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P107[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01		EPFR02		EPFR03		EPFR04		EPFR05		EPFR08		EPFR09		EPFR10		EPFR11		EPFR12					EPFR14		EPFR15		EPFR30					EPFR39				EPFR42				ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG5E_X0X,		D_NULL,		D_NULL,		EPFR30_SGO4E_0X,		NO_MASK,			EPFR42_DAS1_LOW,	PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG5E_X0X,		D_NULL,		D_NULL,		EPFR30_SGO4E_0X,		NO_MASK,			EPFR42_DAS1_LOW,	PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG5E_X0X,		D_NULL,		D_NULL,		EPFR30_SGO4E_0X,		NO_MASK,			EPFR42_DAS1_LOW,	ADC_ANALOG_IN			},		/* ADC アナログ入力ch.7 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG5E_X1X,		D_NULL,		D_NULL,		EPFR30_SGO4E_0X,		NO_MASK,			EPFR42_DAS1_LOW,	PPG_OUT					},		/* PPG 出力ch.5 リロケーション1 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG5E_X0X,		D_NULL,		D_NULL,		EPFR30_SGO4E_0X,		NO_MASK,			EPFR42_DAS1_HIGH,	DAC_OUT					},		/* DAC 出力ch.1 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG5E_X0X,		D_NULL,		D_NULL,		EPFR30_SGO4E_0X,		EPFR39_ICU11E_10,	EPFR42_DAS1_LOW,	ICU_IN					},		/* インプットキャプチャ入力ch.11 リロケーション2 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG5E_X0X,		D_NULL,		D_NULL,		EPFR30_SGO4E_10,		NO_MASK,			EPFR42_DAS1_LOW,	SOUND_GENERATOR_SGO_OUT	}		/* サウンドジェネレータSGO 出力ch.4 リロケーション1 */
};

/* 『P110機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P110[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR01		EPFR02		EPFR03		EPFR04		EPFR05		EPFR07		EPFR08		EPFR10					EPFR11		EPFR12		EPFR24				EPFR28		EPFR30		EPFR44		ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG1E_X0XX,		D_NULL,		D_NULL,		EPFR24_TX1E_LOW,	D_NULL,		D_NULL,		D_NULL,		PORT_IN				},		/* 汎用入力ポート */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG1E_X0XX,		D_NULL,		D_NULL,		EPFR24_TX1E_LOW,	D_NULL,		D_NULL,		D_NULL,		PORT_OUT			},		/* 汎用出力ポート */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG1E_X0XX,		D_NULL,		D_NULL,		EPFR24_TX1E_HIGH,	D_NULL,		D_NULL,		D_NULL,		CAN_TX_OUT 			},		/* CAN TX データ出力ch.1 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG1E_X1XX,		D_NULL,		D_NULL,		EPFR24_TX1E_LOW,	D_NULL,		D_NULL,		D_NULL,		PPG_OUT				},		/* PPG 出力ch.1 リロケーション2 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG1E_X0XX,		D_NULL,		D_NULL,		EPFR24_TX1E_LOW,	D_NULL,		D_NULL,		D_NULL,		FREE_RUN_TIMCLK_IN	}		/* フリーランタイマクロック入力ch.5 リロケーション0 */
};

/* 『P111機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P111[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR01		EPFR02		EPFR03		EPFR04		EPFR05		EPFR07		EPFR08		EPFR10		EPFR11				EPFR12		EPFR24		EPFR28		EPFR30		EPFR44					ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,				PORT_IN				 },		/* 汎用入力ポート */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,				PORT_OUT			 },		/* 汎用出力ポート */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,				CAN_RX_IN			 },		/* CAN RX データ入力ch.1 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR44_INT10E_LOW,		INTTERUPT_REQUEST_IN },		/* 外部割込み要求入力ch.10 リロケーション0 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG2E_1XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,				PPG_OUT				 }		/* PPG 出力ch.2 リロケーション2 */
};

/* 『P112機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P112[PORT_FUNC_CHANGE_ID_NUM_4][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR01		EPFR02		EPFR03		EPFR04		EPFR05		EPFR07		EPFR08		EPFR10		EPFR11				EPFR12		EPFR24					EPFR28		EPFR30		EPFR44		ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG3E_0XX,	D_NULL,		EPFR24_TX2E_LOW,		D_NULL,		D_NULL,		D_NULL,		PORT_IN 	},		/* 汎用入力ポート */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG3E_0XX,	D_NULL,		EPFR24_TX2E_LOW,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT 	},		/* 汎用出力ポート */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG3E_0XX,	D_NULL,		EPFR24_TX2E_HIGH,		D_NULL,		D_NULL,		D_NULL,		CAN_TX_OUT	},		/* CAN TX データ出力ch.2 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG3E_1XX,	D_NULL,		EPFR24_TX2E_LOW,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT		}		/* PPG 出力ch.3 リロケーション2 */
};

/* 『P113機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P113[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR01		EPFR02		EPFR03		EPFR04		EPFR05		EPFR07		EPFR08		EPFR10		EPFR11		EPFR12				EPFR24		EPFR28		EPFR30		EPFR44				ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN				 },		/* 汎用入力ポート */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT			 },		/* 汎用出力ポート */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			CAN_RX_IN			 },		/* CAN RX データ入力ch.2 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR44_INT11E_LOW,	INTTERUPT_REQUEST_IN },		/* 外部割込み要求入力ch.11 リロケーション0 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_1XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PPG_OUT				 }		/* PPG 出力ch.4 リロケーション2 */
};

/* 『P114機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P114[PORT_FUNC_CHANGE_ID_NUM_9][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR01				EPFR02		EPFR03				EPFR04		EPFR05		EPFR07				EPFR08		EPFR10		EPFR11		EPFR12		EPFR24		EPFR28		EPFR30					EPFR44		ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		PORT_IN					 },		/* 汎用入力ポート */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		PORT_OUT				 },		/* 汎用出力ポート */
	{ D_LOW_PXX4,		D_HIGH_PXX4,	NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_01,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		LIN_UART_SCK_IN 		 },		/* LIN_UART シリアルクロック入力ch.3 リロケーション0 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_01,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		LIN_UART_SCK_OUT		 },		/* LIN_UART シリアルクロック出力ch.3 リロケーション0 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		EPFR03_TIN1E_00,	D_NULL,		D_NULL,		EPFR07_SCK3E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		RELOAD_TIMER_EVENT_IN	 },		/* リロードタイマイベント入力ch.1 リロケーション0 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR01_ICU5E_01,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		ICU_IN 				 	 },		/* インプットキャプチャ入力ch.5 リロケーション1 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_HIGH,		D_NULL,		SOUND_GENERATOR_SGA_OUT	 },		/* サウンドジェネレータSGA 出力ch.2 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		PPG_TRG_IN 				 },		/* PPG トリガ入力3 (ch.12-ch.15) */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		ADC_ANALOG_IN			 }		/* ADC アナログ入力ch.32 */
};

/* 『P115機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P115[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR0		EPFR02		EPFR03		EPFR04				EPFR05		EPFR07		EPFR08				EPFR10		EPFR11		EPFR12		EPFR24		EPFR28		EPFR30					EPFR44		ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGO2E_LOW,		D_NULL,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGO2E_LOW,		D_NULL,		PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR08_SIN4E_LOW,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGO2E_LOW,		D_NULL,		LIN_UART_SERIAL_IN 		},		/* LIN_UART シリアル入力ch.4 リロケーション0 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		EPFR04_TIN2E_00,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGO2E_LOW,		D_NULL,		RELOAD_TIMER_EVENT_IN 	},		/* リロードタイマイベント入力ch.2 リロケーション0 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGO2E_HIGH,		D_NULL,		SOUND_GENERATOR_SGO_OUT },		/* サウンドジェネレータSGO 出力ch.2 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGO2E_LOW,		D_NULL,		FREE_RUN_TIMCLK_IN 		},		/* フリーランタイマクロック入力ch.4 リロケーション0 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGO2E_LOW,		D_NULL,		ADC_ANALOG_IN 			}		/* ADC アナログ入力ch.33 */
};

/* 『P116機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P116[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR0		EPFR02		EPFR03		EPFR04		EPFR05				EPFR07		EPFR08				EPFR10		EPFR11		EPFR12		EPFR24		EPFR28				EPFR30				EPFR44		ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SOT4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGA3E_LOW,	D_NULL,		PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SOT4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGA3E_LOW,	D_NULL,		PORT_OUT 				},		/* 汎用出力ポート */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SOT4E_X1,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGA3E_LOW,	D_NULL,		LIN_UART_SERIAL_OUT		},		/* LIN_UART シリアル出力ch.4 リロケーション0 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TIN3E_00,	D_NULL,		EPFR08_SOT4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGA3E_LOW,	D_NULL,		RELOAD_TIMER_EVENT_IN 	},		/* リロードタイマイベント入力ch.3 リロケーション0 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SOT4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGA3E_HIGH,	D_NULL,		SOUND_GENERATOR_SGA_OUT },		/* サウンドジェネレータSGA 出力ch.3 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SOT4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR28_FRCK3E_LOW,	EPFR30_SGA3E_LOW,	D_NULL,		FREE_RUN_TIMCLK_IN 		},		/* フリーランタイマクロック入力ch.3 リロケーション0 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SOT4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGA3E_LOW,	D_NULL,		ADC_ANALOG_IN 			}		/* ADC アナログ入力ch.34 */
};

/* 『P117機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P117[PORT_FUNC_CHANGE_ID_NUM_9][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR0		EPFR02				EPFR03		EPFR04		EPFR05		EPFR07		EPFR08				EPFR10		EPFR11		EPFR12		EPFR24		EPFR28				EPFR30				EPFR44		ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_LOW,	D_NULL,		PORT_IN					 },		/* 汎用入力ポート */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_LOW,	D_NULL,		PORT_OUT				 },		/* 汎用出力ポート */
	{ D_LOW_PXX7,		D_HIGH_PXX7,	D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_01,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_LOW,	D_NULL,		LIN_UART_SCK_IN			 },		/* LIN_UART シリアルクロック入力ch.4 リロケーション0 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_01,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_LOW,	D_NULL,		LIN_UART_SCK_OUT		 },		/* LIN_UART シリアルクロック出力ch.4 リロケーション0 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		EPFR02_TOT0E_XX1,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_LOW,	D_NULL,		RELOAD_TIMER_OUT		 },		/* リロードタイマ出力ch.0 リロケーション0 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_HIGH,	D_NULL,		SOUND_GENERATOR_SGO_OUT  },		/* サウンドジェネレータSGO 出力ch.3 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_LOW,	D_NULL,		PPG_TRG_IN 				 },		/* PPG トリガ入力4 (ch.16-ch.19) */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR28_FRCK2E_LOW,	EPFR30_SGO3E_LOW,	D_NULL,		FREE_RUN_TIMCLK_IN		 },		/* フリーランタイマクロック入力ch.2 リロケーション0 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_LOW,	D_NULL,		ADC_ANALOG_IN 			 }		/* ADC アナログ入力ch.35 */
};

/* 『P120機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P120[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE12] =
{
	/* DDR12			PFR12			EPFR00		EPFR01		EPFR03				EPFR04		EPFR05		EPFR09				EPFR12				EPFR13		EPFR14		EPFR15		EPFR28				EPFR29		EPFR31		EPFR35		EPFR42		EPFR43				ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR03_TOT1E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR12_PPG5E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN				 },		/* 汎用入力ポート */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR03_TOT1E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR12_PPG5E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT			 },		/* 汎用出力ポート */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR03_TOT1E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR12_PPG5E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR28_FRCK1E_LOW,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			FREE_RUN_TIMCLK_IN	 },		/* フリーランタイマクロック入力ch.1 リロケーション0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR03_TOT1E_XX0,	D_NULL,		D_NULL,		EPFR09_SIN5E_LOW,	EPFR12_PPG5E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			LIN_UART_SERIAL_IN	 },		/* LIN_UART シリアル入力ch.5 リロケーション0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR03_TOT1E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR12_PPG5E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR43_INT6E_LOW,	INTTERUPT_REQUEST_IN },		/* 外部割込み要求入力ch.6 リロケーション0 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		D_NULL,		EPFR03_TOT1E_XX1,	D_NULL,		D_NULL,		NO_MASK,			EPFR12_PPG5E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			RELOAD_TIMER_OUT	 },		/* リロードタイマ出力ch.1 リロケーション0 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		D_NULL,		EPFR03_TOT1E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR12_PPG5E_1XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PPG_OUT				 },		/* PPG 出力ch.5 リロケーション2 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR03_TOT1E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR12_PPG5E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			ADC_ANALOG_IN		 }		/* ADC アナログ入力ch.36 */
};

/* 『P121機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P121[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE12] =
{
	/* DDR12			PFR12			EPFR00		EPFR01		EPFR03		EPFR04				EPFR05		EPFR09				EPFR12		EPFR13				EPFR14		EPFR15		EPFR28				EPFR29		EPFR31		EPFR35		EPFR42		EPFR43				ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX0,	D_NULL,		EPFR09_SOT5E_X0,	D_NULL,		EPFR13_PPG6E_0XX,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN				 },		/* 汎用入力ポート */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX0,	D_NULL,		EPFR09_SOT5E_X0,	D_NULL,		EPFR13_PPG6E_0XX,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT			 },		/* 汎用出力ポート */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX0,	D_NULL,		EPFR09_SOT5E_X0,	D_NULL,		EPFR13_PPG6E_0XX,	D_NULL,		D_NULL,		EPFR28_FRCK0E_LOW,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			FREE_RUN_TIMCLK_IN	 },		/* フリーランタイマクロック入力ch.0 リロケーション0 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX0,	D_NULL,		EPFR09_SOT5E_X1,	D_NULL,		EPFR13_PPG6E_0XX,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			LIN_UART_SERIAL_OUT	 },		/* LIN_UART シリアル出力ch.5 リロケーション0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX0,	D_NULL,		EPFR09_SOT5E_X0,	D_NULL,		EPFR13_PPG6E_0XX,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR43_INT7E_LOW,	INTTERUPT_REQUEST_IN },		/* 外部割込み要求入力ch.7 リロケーション0 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX1,	D_NULL,		EPFR09_SOT5E_X0,	D_NULL,		EPFR13_PPG6E_0XX,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			RELOAD_TIMER_OUT	 },		/* リロードタイマ出力ch.2 リロケーション0 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX0,	D_NULL,		EPFR09_SOT5E_X0,	D_NULL,		EPFR13_PPG6E_1XX,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PPG_OUT				 },		/* PPG 出力ch.6 リロケーション2 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX0,	D_NULL,		EPFR09_SOT5E_X0,	D_NULL,		EPFR13_PPG6E_0XX,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			ADC_ANALOG_IN		 }		/* ADC アナログ入力ch.37 */
};

/* 『P122機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P122[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE12] =
{
	/* DDR12			PFR12			EPFR00		EPFR01		EPFR03		EPFR04		EPFR05				EPFR09				EPFR12		EPFR13				EPFR14		EPFR15		EPFR28		EPFR29					EPFR31		EPFR35		EPFR42		EPFR43		ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX0,	EPFR09_SCK5E_X0,	D_NULL,		EPFR13_PPG7E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN			 },		/* 汎用入力ポート */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX0,	EPFR09_SCK5E_X0,	D_NULL,		EPFR13_PPG7E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT		 },		/* 汎用出力ポート */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX0,	EPFR09_SCK5E_X0,	D_NULL,		EPFR13_PPG7E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_01,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		OCU_OUT 		 },		/* アウトプットコンペア出力ch.0 リロケーション0 */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX0,	EPFR09_SCK5E_01,	D_NULL,		EPFR13_PPG7E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		LIN_UART_SCK_IN  },		/* LIN_UART シリアルクロック入力ch.5 リロケーション0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX0,	EPFR09_SCK5E_01,	D_NULL,		EPFR13_PPG7E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		LIN_UART_SCK_OUT },		/* LIN_UART シリアルクロック出力ch.5 リロケーション0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX1,	EPFR09_SCK5E_X0,	D_NULL,		EPFR13_PPG7E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		RELOAD_TIMER_OUT },		/* リロードタイマ出力ch.3 リロケーション0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX0,	EPFR09_SCK5E_X0,	D_NULL,		EPFR13_PPG7E_1XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT 		 },		/* PPG 出力ch.7 リロケーション2 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX0,	EPFR09_SCK5E_X0,	D_NULL,		EPFR13_PPG7E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ADC_ANALOG_IN	 }		/* ADC アナログ入力ch.38 */
};

/* 『P123機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P123[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE12] =
{
	/* DDR12			PFR12			EPFR00		EPFR01		EPFR03		EPFR04		EPFR05		EPFR09		EPFR12		EPFR13		EPFR14				EPFR15		EPFR28		EPFR29					EPFR31		EPFR35		EPFR42				EPFR43		ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU1E_X0,		D_NULL,		D_NULL,		EPFR42_DAS0_LOW,	D_NULL,		PORT_IN 		},		/* 汎用入力ポート */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU1E_X0,		D_NULL,		D_NULL,		EPFR42_DAS0_LOW,	D_NULL,		PORT_OUT 		},		/* 汎用出力ポート */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU1E_01,		D_NULL,		D_NULL,		EPFR42_DAS0_LOW,	D_NULL,		OCU_OUT 		},		/* アウトプットコンペア出力ch.1 リロケーション0 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_1XX,	D_NULL,		D_NULL,		EPFR29_OCU1E_X0,		D_NULL,		D_NULL,		EPFR42_DAS0_LOW,	D_NULL,		PPG_OUT			},		/* PPG 出力ch.8 リロケーション2 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU1E_X0,		D_NULL,		D_NULL,		EPFR42_DAS0_HIGH,	D_NULL,		DAC_OUT			},		/* DAC 出力ch.0 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU1E_X0,		D_NULL,		D_NULL,		EPFR42_DAS0_LOW,	D_NULL,		ADC_ANALOG_IN 	}		/* ADC アナログ入力ch.39 */
};

/* 『P124機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P124[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE12] =
{
	/* DDR12			PFR12			EPFR00		EPFR01				EPFR03		EPFR04		EPFR05		EPFR09		EPFR12		EPFR13		EPFR14				EPFR15		EPFR28		EPFR29				EPFR31		EPFR35		EPFR42		EPFR43		ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU2E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN	 },		/* 汎用入力ポート */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU2E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT },		/* 汎用出力ポート */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU2E_01,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		OCU_OUT	 },		/* アウトプットコンペア出力ch.2 リロケーション0 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR01_ICU5E_10,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU2E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		ICU_IN	 },		/* インプットキャプチャ入力ch.5 リロケーション2 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_1XX,	D_NULL,		D_NULL,		EPFR29_OCU2E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT	 }		/* PPG 出力ch.9 リロケーション2 */
};

/* 『P125機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P125[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE12] =
{
	/* DDR12			PFR12			EPFR00				EPFR01		EPFR03		EPFR04		EPFR05		EPFR09		EPFR12		EPFR13		EPFR14		EPFR15					EPFR28		EPFR29					EPFR31		EPFR35		EPFR42		EPFR43		ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_0XX,		D_NULL,		EPFR29_OCU3E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN	 },		/* 汎用入力ポート */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_0XX,		D_NULL,		EPFR29_OCU3E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT },		/* 汎用出力ポート */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_0XX,		D_NULL,		EPFR29_OCU3E_01,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		OCU_OUT	 },		/* アウトプットコンペア出力ch.3 リロケーション0 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR00_ICU0E_00,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_0XX,		D_NULL,		EPFR29_OCU3E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ICU_IN 	 },		/* インプットキャプチャ入力ch.0 リロケーション0 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_1XX,		D_NULL,		EPFR29_OCU3E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT	 }		/* PPG 出力ch.10 リロケーション2 */
};

/* 『P126機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P126[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE12] =
{
	/* DDR12			PFR12			EPFR00		EPFR01		EPFR03		EPFR04		EPFR05		EPFR09		EPFR12		EPFR13		EPFR14		EPFR15		EPFR28		EPFR29		EPFR31					EPFR35				EPFR42		EPFR43				ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU4E_X0,		NO_MASK,			D_NULL,		NO_MASK,			PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU4E_X0,		NO_MASK,			D_NULL,		NO_MASK,			PORT_OUT					},		/* 汎用出力ポート */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU4E_X0,		NO_MASK,			D_NULL,		NO_MASK,			PPG_TRG_IN					},		/* PPG トリガ入力0 (ch.0-ch.3) */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU4E_X0,		EPFR35_SIN0E_LOW,	D_NULL,		NO_MASK,			MULTI_FUNCTION_SERIAL_IN	},		/* マルチファンクションシリアル入力ch.0 リロケーション0 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU4E_X0,		NO_MASK,			D_NULL,		EPFR43_INT1E_LOW,	INTTERUPT_REQUEST_IN		},		/* 外部割込み要求入力ch.1 リロケーション0 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU4E_01,		NO_MASK,			D_NULL,		NO_MASK,			OCU_OUT						}		/* アウトプットコンペア出力ch.4 リロケーション0 */
};

/* 『P127機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P127[PORT_FUNC_CHANGE_ID_NUM_4][PORT_FUNC_CHANGE12] =
{	
	/* DDR12			PFR12			EPFR00		EPFR01		EPFR03		EPFR04		EPFR05		EPFR09		EPFR12		EPFR13		EPFR14		EPFR15		EPFR28		EPFR29		EPFR31					EPFR35				EPFR42		EPFR43		ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU5E_X0,		EPFR35_SOT0E_X0,	D_NULL,		D_NULL,		PORT_IN 				 },		/* 汎用入力ポート */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU5E_X0,		EPFR35_SOT0E_X0,	D_NULL,		D_NULL,		PORT_OUT  				 },		/* 汎用出力ポート */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU5E_X0,		EPFR35_SOT0E_01,	D_NULL,		D_NULL,		MULTI_FUNCTION_SERIAL_OUT },	/* マルチファンクションシリアル出力ch.0 リロケーション0 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU5E_01,		EPFR35_SOT0E_X0,	D_NULL,		D_NULL,		OCU_OUT					 }		/* アウトプットコンペア出力ch.5 リロケーション0*/
};

/* 『P130機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P130[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE13] =
{
	/* DDR13			PFR13			EPFR00				EPFR01		EPFR10		EPFR15		EPFR26					EPFR35				EPFR36		EPFR43				ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_LOW,		EPFR35_SCK0E_X0,	D_NULL,		NO_MASK,			PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_LOW,		EPFR35_SCK0E_X0,	D_NULL,		NO_MASK,			PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_LOW,		EPFR35_SCK0E_01,	D_NULL,		NO_MASK,			MULTI_FUNCTION_SCK_IN	},		/* マルチファンクションシリアルクロック入力ch.0 リロケーション0 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_LOW,		EPFR35_SCK0E_01,	D_NULL,		NO_MASK,			MULTI_FUNCTION_SCK_OUT	},		/* マルチファンクションシリアルクロック出力ch.0 リロケーション0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_LOW,		EPFR35_SCK0E_X0,	D_NULL,		EPFR43_INT0E_LOW,	INTTERUPT_REQUEST_IN	},		/* 外部割込み要求入力ch.0 リロケーション0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR00_ICU1E_00,	D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_LOW,		EPFR35_SCK0E_X0,	D_NULL,		NO_MASK,			ICU_IN					},		/* インプットキャプチャ入力ch.1 リロケーション0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_LOW,		EPFR35_SCK0E_X0,	D_NULL,		NO_MASK,			BASE_TIMER_IN			},		/* ベースタイマ入力ch.0 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_HIGH,		EPFR35_SCK0E_X0,	D_NULL,		NO_MASK,			BASE_TIMER_OUT			}		/* ベースタイマ出力ch.0 */
};

/* 『P131機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P131[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE13] =
{
	/* DDR13			PFR13			EPFR00				EPFR01		EPFR10		EPFR15		EPFR26					EPFR35		EPFR36				EPFR43				ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_LOW,		D_NULL,		NO_MASK,			NO_MASK,			PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_LOW,		D_NULL,		NO_MASK,			NO_MASK,			PORT_OUT					},		/* 汎用出力ポート */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_LOW,		D_NULL,		NO_MASK,			NO_MASK,			PPG_TRG_IN					},		/* PPG トリガ入力1 (ch.4-ch.7) */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_LOW,		D_NULL,		EPFR36_SIN1E_LOW,	NO_MASK,			MULTI_FUNCTION_SERIAL_IN	},		/* マルチファンクションシリアル入力ch.1 リロケーション0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_LOW,		D_NULL,		NO_MASK,			EPFR43_INT4E_LOW,	INTTERUPT_REQUEST_IN		},		/* 外部割込み要求入力ch.4 リロケーション0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR00_ICU2E_00,	D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_LOW,		D_NULL,		NO_MASK,			NO_MASK,			ICU_IN						},		/* インプットキャプチャ入力ch.2 リロケーション0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_LOW,		D_NULL,		NO_MASK,			NO_MASK,			BASE_TIMER_IN				},		/* ベースタイマ入力ch.1 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_HIGH,		D_NULL,		NO_MASK,			NO_MASK,			BASE_TIMER_OUT				}		/* ベースタイマ出力ch.1 */
};

/* 『P132機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P132[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE13] =
{
	/* DDR13			PFR13			EPFR00				EPFR01		EPFR10		EPFR15		EPFR26					EPFR35		EPFR36					EPFR43				ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIB0E_LOW,		D_NULL,		EPFR36_SOT1E_X0,		NO_MASK,			PORT_IN						},		/* 汎用入力ポート */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIB0E_LOW,		D_NULL,		EPFR36_SOT1E_X0,		NO_MASK,			PORT_OUT					},		/* 汎用出力ポート */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIB0E_LOW,		D_NULL,		EPFR36_SOT1E_01,		NO_MASK,			MULTI_FUNCTION_SERIAL_OUT	},		/* マルチファンクションシリアル出力ch.1 リロケーション0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIB0E_LOW,		D_NULL,		EPFR36_SOT1E_X0,		EPFR43_INT2E_LOW,	INTTERUPT_REQUEST_IN		},		/* 外部割込み要求入力ch.2 リロケーション0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR00_ICU3E_00,	D_NULL,		D_NULL,		D_NULL,		EPFR26_TIB0E_LOW,		D_NULL,		EPFR36_SOT1E_X0,		NO_MASK,			ICU_IN						},		/* インプットキャプチャ入力ch.3 リロケーション0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIB0E_LOW,		D_NULL,		EPFR36_SOT1E_X0,		NO_MASK,			BASE_TIMER_IN				},		/* ベースタイマ入力ch.0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIB0E_HIGH,		D_NULL,		EPFR36_SOT1E_X0,		NO_MASK,			BASE_TIMER_OUT				}		/* ベースタイマ出力ch.0 */
};

/* 『P133機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P133[PORT_FUNC_CHANGE_ID_NUM_10][PORT_FUNC_CHANGE13] =
{
	/* DDR13			PFR13			EPFR00		EPFR01				EPFR10		EPFR15				EPFR26					EPFR35		EPFR36					EPFR43				ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_X0,		NO_MASK,			PORT_IN					},		/* 汎用入力ポート */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_X0,		NO_MASK,			PORT_OUT				},		/* 汎用出力ポート */
	{ D_LOW_PXX3,		D_HIGH_PXX3,	D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_01,		NO_MASK,			MULTI_FUNCTION_SCK_IN	},		/* マルチファンクションシリアルクロック入力ch.1 リロケーション0 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_01,		NO_MASK,			MULTI_FUNCTION_SCK_OUT	},		/* マルチファンクションシリアルクロック出力ch.1 リロケーション0 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_X0,		EPFR43_INT3E_LOW,	INTTERUPT_REQUEST_IN	},		/* 外部割込み要求入力ch.3 リロケーション0 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		EPFR01_ICU4E_00,	D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_X0,		NO_MASK,			ICU_IN					},		/* インプットキャプチャ入力ch.4 リロケーション0 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_X0,		NO_MASK,			BASE_TIMER_IN			},		/* ベースタイマ入力ch.1 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_HIGH,		D_NULL,		EPFR36_SCK1E_X0,		NO_MASK,			BASE_TIMER_OUT			},		/* ベースタイマ出力ch.1 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_1X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_X0,		NO_MASK,			PPG_OUT					},		/* PPG 出力ch.11 リロケーション1 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_X0,		NO_MASK,			PPG_TRG_IN				}		/* PPG トリガ入力5 (ch.20-ch.23) */
};

/* 『P134機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P134[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE13] =
{
	/* DDR13			PFR13			EPFR00		EPFR01				EPFR10				EPFR15		EPFR26		EPFR35		EPFR36		EPFR43				ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		NO_MASK,			EPFR10_PPG1E_0XXX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN				 },		/* 汎用入力ポート */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		D_NULL,		NO_MASK,			EPFR10_PPG1E_0XXX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT			 },		/* 汎用出力ポート */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		NO_MASK,			EPFR10_PPG1E_0XXX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PPG_TRG_IN			 },		/* PPG トリガ入力2 (ch.8-ch.11) */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		NO_MASK,			EPFR10_PPG1E_0XXX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR43_INT5E_LOW,	INTTERUPT_REQUEST_IN },		/* 外部割込み要求入力ch.5 リロケーション0 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR01_ICU5E_00,	EPFR10_PPG1E_0XXX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			ICU_IN				 },		/* インプットキャプチャ入力ch.5 リロケーション0 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		NO_MASK,			EPFR10_PPG1E_1XXX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PPG_OUT				 }		/* PPG 出力ch.1 リロケーション3 */
};

/* 『P136機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P136[PORT_FUNC_CHANGE_ID_NUM_3][PORT_FUNC_CHANGE13] =
{
	/* DDR13			PFR13			EPFR00		EPFR0		EPFR10		EPFR15		EPFR26		EPFR35		EPFR36		EPFR43		ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN			 },		/* 汎用入力ポート */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT		 },		/* 汎用出力ポート */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		SUB_CLK_OSC_OUT	 }		/* サブクロックOSC 出力(サブクロック搭載品種のみ) */
};

/* 『P137機能切替設定値テーブル』*/
const static UI_16 C_FuncOperateTbl_P137[PORT_FUNC_CHANGE_ID_NUM_3][PORT_FUNC_CHANGE13] =
{
	/* DDR13		PFR13			EPFR00		EPFR0		EPFR10		EPFR15		EPFR26		EPFR35		EPFR36		EPFR43		ID */
	{ D_LOW_PXX7,	D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN			},		/* 汎用入力ポート */
	{ D_HIGH_PXX7,	D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT		},		/* 汎用出力ポート */
	{ D_LOW_PXX7,	D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		SUB_CLK_OSC_IN	}		/* サブクロックOSC 入力(サブクロック搭載品種のみ) */
};

/* 機能切替設定値ビット選択テーブルP00 */
static const UI_16* const C_FuncOperateTbl_P00[BIT_MAX] =
{
	&C_FuncOperateTbl_P000[0][0],		/* 機能切替設定値テーブルへのポインタP000 */
	&C_FuncOperateTbl_P001[0][0],		/* 機能切替設定値テーブルへのポインタP001 */
	&C_FuncOperateTbl_P002[0][0],		/* 機能切替設定値テーブルへのポインタP002 */
	&C_FuncOperateTbl_P003[0][0],		/* 機能切替設定値テーブルへのポインタP003 */
	&C_FuncOperateTbl_P004[0][0],		/* 機能切替設定値テーブルへのポインタP004 */
	&C_FuncOperateTbl_P005[0][0],		/* 機能切替設定値テーブルへのポインタP005 */
	&C_FuncOperateTbl_P006[0][0],		/* 機能切替設定値テーブルへのポインタP006 */
	&C_FuncOperateTbl_P007[0][0]		/* 機能切替設定値テーブルへのポインタP007 */
};

/* 機能切替設定値ビット選択テーブルP01 */
static const UI_16* const C_FuncOperateTbl_P01[BIT_MAX] =
{
	&C_FuncOperateTbl_P010[0][0],		/* 機能切替設定値テーブルへのポインタP010 */
	&C_FuncOperateTbl_P011[0][0],		/* 機能切替設定値テーブルへのポインタP011 */
	&C_FuncOperateTbl_P012[0][0],		/* 機能切替設定値テーブルへのポインタP012 */
	&C_FuncOperateTbl_P013[0][0],		/* 機能切替設定値テーブルへのポインタP013 */
	&C_FuncOperateTbl_P014[0][0],		/* 機能切替設定値テーブルへのポインタP014 */
	&C_FuncOperateTbl_P015[0][0],		/* 機能切替設定値テーブルへのポインタP015 */
	&C_FuncOperateTbl_P016[0][0],		/* 機能切替設定値テーブルへのポインタP016 */
	&C_FuncOperateTbl_P017[0][0]		/* 機能切替設定値テーブルへのポインタP017 */
};

/* 機能切替設定値ビット選択テーブルP02 */
static const UI_16* const C_FuncOperateTbl_P02[BIT_MAX] =
{
	&C_FuncOperateTbl_P020[0][0],		/* 機能切替設定値テーブルへのポインタP020 */
	&C_FuncOperateTbl_P021[0][0],		/* 機能切替設定値テーブルへのポインタP021 */
	&C_FuncOperateTbl_P022[0][0],		/* 機能切替設定値テーブルへのポインタP022 */
	&C_FuncOperateTbl_P023[0][0],		/* 機能切替設定値テーブルへのポインタP023 */
	&C_FuncOperateTbl_P024[0][0],		/* 機能切替設定値テーブルへのポインタP024 */
	&C_FuncOperateTbl_P025[0][0],		/* 機能切替設定値テーブルへのポインタP025 */
	&C_FuncOperateTbl_P026[0][0],		/* 機能切替設定値テーブルへのポインタP026 */
	&C_FuncOperateTbl_P027[0][0]		/* 機能切替設定値テーブルへのポインタP027 */
};

/* 機能切替設定値ビット選択テーブルP03 */
static const UI_16* const C_FuncOperateTbl_P03[BIT_MAX] =
{
	&C_FuncOperateTbl_P030[0][0],		/* 機能切替設定値テーブルへのポインタP030 */
	&C_FuncOperateTbl_P031[0][0],		/* 機能切替設定値テーブルへのポインタP031 */
	&C_FuncOperateTbl_P032[0][0],		/* 機能切替設定値テーブルへのポインタP032 */
	&C_FuncOperateTbl_P033[0][0],		/* 機能切替設定値テーブルへのポインタP033 */
	&C_FuncOperateTbl_P034[0][0],		/* 機能切替設定値テーブルへのポインタP034 */
	&C_FuncOperateTbl_P035[0][0],		/* 機能切替設定値テーブルへのポインタP035 */
	&C_FuncOperateTbl_P036[0][0],		/* 機能切替設定値テーブルへのポインタP036 */
	&C_FuncOperateTbl_P037[0][0]		/* 機能切替設定値テーブルへのポインタP037 */
};

/* 機能切替設定値ビット選択テーブルP04 */
static const UI_16* const C_FuncOperateTbl_P04[BIT_MAX] =
{
	&C_FuncOperateTbl_P040[0][0],		/* 機能切替設定値テーブルへのポインタP040 */
	&C_FuncOperateTbl_P041[0][0],		/* 機能切替設定値テーブルへのポインタP041 */
	&C_FuncOperateTbl_P042[0][0],		/* 機能切替設定値テーブルへのポインタP042 */
	&C_FuncOperateTbl_P043[0][0],		/* 機能切替設定値テーブルへのポインタP043 */
	&C_FuncOperateTbl_P044[0][0],		/* 機能切替設定値テーブルへのポインタP044 */
	&C_FuncOperateTbl_P045[0][0],		/* 機能切替設定値テーブルへのポインタP045 */
	&C_FuncOperateTbl_P046[0][0],		/* 機能切替設定値テーブルへのポインタP046 */
	&C_FuncOperateTbl_P047[0][0]		/* 機能切替設定値テーブルへのポインタP047 */
};

/* 機能切替設定値ビット選択テーブルP05 */
static const UI_16* const C_FuncOperateTbl_P05[BIT_MAX] =
{
	&C_FuncOperateTbl_P050[0][0],		/* 機能切替設定値テーブルへのポインタP050 */
	&C_FuncOperateTbl_P051[0][0],		/* 機能切替設定値テーブルへのポインタP051 */
	&C_FuncOperateTbl_P052[0][0],		/* 機能切替設定値テーブルへのポインタP052 */
	&C_FuncOperateTbl_P053[0][0],		/* 機能切替設定値テーブルへのポインタP053 */
	&C_FuncOperateTbl_P054[0][0],		/* 機能切替設定値テーブルへのポインタP054 */
	&C_FuncOperateTbl_P055[0][0],		/* 機能切替設定値テーブルへのポインタP055 */
	&C_FuncOperateTbl_P056[0][0],		/* 機能切替設定値テーブルへのポインタP056 */
	&C_FuncOperateTbl_P057[0][0]		/* 機能切替設定値テーブルへのポインタP057 */
};

/* 機能切替設定値ビット選択テーブルP06 */
static const UI_16* const C_FuncOperateTbl_P06[BIT_MAX] =
{
	&C_FuncOperateTbl_P060[0][0],		/* 機能切替設定値テーブルへのポインタP060 */
	&C_FuncOperateTbl_P061[0][0],		/* 機能切替設定値テーブルへのポインタP061 */
	&C_FuncOperateTbl_P062[0][0],		/* 機能切替設定値テーブルへのポインタP062 */
	&C_FuncOperateTbl_P063[0][0],		/* 機能切替設定値テーブルへのポインタP063 */
	&C_FuncOperateTbl_P064[0][0],		/* 機能切替設定値テーブルへのポインタP064 */
	&C_FuncOperateTbl_P065[0][0],		/* 機能切替設定値テーブルへのポインタP065 */
	&C_FuncOperateTbl_P066[0][0],		/* 機能切替設定値テーブルへのポインタP066 */
	&C_FuncOperateTbl_P067[0][0]		/* 機能切替設定値テーブルへのポインタP067 */
};

/*  機能切替設定値ビット選択テーブルP07 */
static const UI_16* const C_FuncOperateTbl_P07[BIT_MAX] =
{
	&C_FuncOperateTbl_P070[0][0],		/* 機能切替設定値テーブルへのポインタP070 */
	&C_FuncOperateTbl_P071[0][0],		/* 機能切替設定値テーブルへのポインタP071 */
	&C_FuncOperateTbl_P072[0][0],		/* 機能切替設定値テーブルへのポインタP072 */
	&C_FuncOperateTbl_P073[0][0],		/* 機能切替設定値テーブルへのポインタP073 */
	&C_FuncOperateTbl_P074[0][0],		/* 機能切替設定値テーブルへのポインタP074 */
	&C_FuncOperateTbl_P075[0][0],		/* 機能切替設定値テーブルへのポインタP075 */
	&C_FuncOperateTbl_P076[0][0],		/* 機能切替設定値テーブルへのポインタP076 */
	&C_FuncOperateTbl_P077[0][0]		/* 機能切替設定値テーブルへのポインタP077 */
};

/*  機能切替設定値ビット選択テーブルP08 */
static const UI_16* const C_FuncOperateTbl_P08[BIT_MAX] =
{
	&C_FuncOperateTbl_P080[0][0],		/* 機能切替設定値テーブルへのポインタP080 */
	&C_FuncOperateTbl_P081[0][0],		/* 機能切替設定値テーブルへのポインタP081 */
	&C_FuncOperateTbl_P082[0][0],		/* 機能切替設定値テーブルへのポインタP082 */
	&C_FuncOperateTbl_P083[0][0],		/* 機能切替設定値テーブルへのポインタP083 */
	&C_FuncOperateTbl_P084[0][0],		/* 機能切替設定値テーブルへのポインタP084 */
	&C_FuncOperateTbl_P085[0][0],		/* 機能切替設定値テーブルへのポインタP085 */
	&C_FuncOperateTbl_P086[0][0],		/* 機能切替設定値テーブルへのポインタP086 */
	&C_FuncOperateTbl_P087[0][0]		/* 機能切替設定値テーブルへのポインタP087 */
};

/*  機能切替設定値ビット選択テーブルP09 */
static const UI_16* const C_FuncOperateTbl_P09[BIT_MAX] =
{
	&C_FuncOperateTbl_P090[0][0],		/* 機能切替設定値テーブルへのポインタP090 */
	&C_FuncOperateTbl_P091[0][0],		/* 機能切替設定値テーブルへのポインタP091 */
	&C_FuncOperateTbl_P092[0][0],		/* 機能切替設定値テーブルへのポインタP092 */
	&C_FuncOperateTbl_P093[0][0],		/* 機能切替設定値テーブルへのポインタP093 */
	&C_FuncOperateTbl_P094[0][0],		/* 機能切替設定値テーブルへのポインタP094 */
	&C_FuncOperateTbl_P095[0][0],		/* 機能切替設定値テーブルへのポインタP095 */
	&C_FuncOperateTbl_P096[0][0],		/* 機能切替設定値テーブルへのポインタP096 */
	&C_FuncOperateTbl_P097[0][0]		/* 機能切替設定値テーブルへのポインタP097 */
};

/*  機能切替設定値ビット選択テーブルP10 */
static const UI_16* const C_FuncOperateTbl_P10[BIT_MAX] =
{
	&C_FuncOperateTbl_P100[0][0],		/* 機能切替設定値テーブルへのポインタP100 */
	&C_FuncOperateTbl_P101[0][0],		/* 機能切替設定値テーブルへのポインタP101 */
	&C_FuncOperateTbl_P102[0][0],		/* 機能切替設定値テーブルへのポインタP102 */
	&C_FuncOperateTbl_P103[0][0],		/* 機能切替設定値テーブルへのポインタP103 */
	&C_FuncOperateTbl_P104[0][0],		/* 機能切替設定値テーブルへのポインタP104 */
	&C_FuncOperateTbl_P105[0][0],		/* 機能切替設定値テーブルへのポインタP105 */
	&C_FuncOperateTbl_P106[0][0],		/* 機能切替設定値テーブルへのポインタP106 */
	&C_FuncOperateTbl_P107[0][0]		/* 機能切替設定値テーブルへのポインタP107 */
};

/*  機能切替設定値ビット選択テーブルP11 */
static const UI_16* const C_FuncOperateTbl_P11[BIT_MAX] =
{
	&C_FuncOperateTbl_P110[0][0],		/* 機能切替設定値テーブルへのポインタP110 */
	&C_FuncOperateTbl_P111[0][0],		/* 機能切替設定値テーブルへのポインタP111 */
	&C_FuncOperateTbl_P112[0][0],		/* 機能切替設定値テーブルへのポインタP112 */
	&C_FuncOperateTbl_P113[0][0],		/* 機能切替設定値テーブルへのポインタP113 */
	&C_FuncOperateTbl_P114[0][0],		/* 機能切替設定値テーブルへのポインタP114 */
	&C_FuncOperateTbl_P115[0][0],		/* 機能切替設定値テーブルへのポインタP115 */
	&C_FuncOperateTbl_P116[0][0],		/* 機能切替設定値テーブルへのポインタP116 */
	&C_FuncOperateTbl_P117[0][0]		/* 機能切替設定値テーブルへのポインタP117 */
};

/*  機能切替設定値ビット選択テーブルP12 */
static const UI_16* const C_FuncOperateTbl_P12[BIT_MAX] =
{
	&C_FuncOperateTbl_P120[0][0],		/* 機能切替設定値テーブルへのポインタP120 */
	&C_FuncOperateTbl_P121[0][0],		/* 機能切替設定値テーブルへのポインタP121 */
	&C_FuncOperateTbl_P122[0][0],		/* 機能切替設定値テーブルへのポインタP122 */
	&C_FuncOperateTbl_P123[0][0],		/* 機能切替設定値テーブルへのポインタP123 */
	&C_FuncOperateTbl_P124[0][0],		/* 機能切替設定値テーブルへのポインタP124 */
	&C_FuncOperateTbl_P125[0][0],		/* 機能切替設定値テーブルへのポインタP125 */
	&C_FuncOperateTbl_P126[0][0],		/* 機能切替設定値テーブルへのポインタP126 */
	&C_FuncOperateTbl_P127[0][0]		/* 機能切替設定値テーブルへのポインタP127 */
};

/*  機能切替設定値ビット選択テーブルP13 */
static const UI_16* const C_FuncOperateTbl_P13[BIT_MAX] =
{
	&C_FuncOperateTbl_P130[0][0],		/* 機能切替設定値テーブルへのポインタP130 */
	&C_FuncOperateTbl_P131[0][0],		/* 機能切替設定値テーブルへのポインタP131 */
	&C_FuncOperateTbl_P132[0][0],		/* 機能切替設定値テーブルへのポインタP132 */
	&C_FuncOperateTbl_P133[0][0],		/* 機能切替設定値テーブルへのポインタP133 */
	&C_FuncOperateTbl_P134[0][0],		/* 機能切替設定値テーブルへのポインタP134 */
	D_NULL,								/* 機能切替設定値テーブルへのポインタP135 */
	&C_FuncOperateTbl_P136[0][0],		/* 機能切替設定値テーブルへのポインタP136 */
	&C_FuncOperateTbl_P137[0][0]		/* 機能切替設定値テーブルへのポインタP137 */
};

/* 機能切替設定値ポート選択テーブル */
static const UI_16* const * const C_FuncOperateTbl[PORT_MAX] =
{
	&C_FuncOperateTbl_P00[0],		/* 機能切替設定値ビット選択テーブルへのポインタP00 */
	&C_FuncOperateTbl_P01[0],		/* 機能切替設定値ビット選択テーブルへのポインタP01 */
	&C_FuncOperateTbl_P02[0],		/* 機能切替設定値ビット選択テーブルへのポインタP02 */
	&C_FuncOperateTbl_P03[0],		/* 機能切替設定値ビット選択テーブルへのポインタP03 */
	&C_FuncOperateTbl_P04[0],		/* 機能切替設定値ビット選択テーブルへのポインタP04 */
	&C_FuncOperateTbl_P05[0],		/* 機能切替設定値ビット選択テーブルへのポインタP05 */
	&C_FuncOperateTbl_P06[0],		/* 機能切替設定値ビット選択テーブルへのポインタP06 */
	&C_FuncOperateTbl_P07[0],		/* 機能切替設定値ビット選択テーブルへのポインタP07 */
	&C_FuncOperateTbl_P08[0],		/* 機能切替設定値ビット選択テーブルへのポインタP08 */
	&C_FuncOperateTbl_P09[0],		/* 機能切替設定値ビット選択テーブルへのポインタP09 */
	&C_FuncOperateTbl_P10[0],		/* 機能切替設定値ビット選択テーブルへのポインタP10 */
	&C_FuncOperateTbl_P11[0],		/* 機能切替設定値ビット選択テーブルへのポインタP11 */
	&C_FuncOperateTbl_P12[0],		/* 機能切替設定値ビット選択テーブルへのポインタP12 */
	&C_FuncOperateTbl_P13[0]		/* 機能切替設定値ビット選択テーブルへのポインタP13 */
};
#endif	/* PORT_DRV_FUNC_CHANGE == 1 */

/****************************************************************************************/
/*	[モジュール名]	PortDrv_SetDataByte	[名称]	指定ポートのByteデータ設定				*/
/*======================================================================================*/
/*	[処理概要]	指定ポートのポートデータレジスタに指定Byteデータを設定する				*/
/*======================================================================================*/
/*	[記述形式]	SI_8 PortDrv_SetDataByte( E_PORTDRV_PORT_NUM port_num, 					*/
/* 				, E_PORTDRV_BYTE_NUM byte_num, UI_8 set_byte )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_PORTDRV_PORT_NUM port_num	指定ポート番号	E_PORTDRV_PORT_NUM:enum定義	*/
/*				E_PORTDRV_BYTE_NUM byte_num	16bitﾏｲｺﾝとのIF用Byte設定情報				*/
/*				UI_8 set_byte	設定情報												*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8 state;	処理実行ステータス											*/
/*======================================================================================*/
/*	[ 備  考 ]																			*/
/****************************************************************************************/
SI_8 PortDrv_SetDataByte( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BYTE_NUM byte_num, UI_8 set_byte )
{
	SI_8 state;			/* 処理実行ステータス */
	
	/* 引数1：指定ポート番号のデータ範囲判定 */
	if ( port_num < PORTDRV_PORT_MAX ) {
		state = D_OK;	/* 正常 */
		*C_PortDataTbl[port_num] = set_byte;	/* Byteデータをポートデータレジスタに設定 */
	} else {
		state = D_NG; 	/* 異常 */
	}
	
	return state;	/* 戻り値:処理実行ステータス */
}

/****************************************************************************************/
/*	[モジュール名]	PortDrv_SetDataBit	[名称]	指定ポートの指定Bitデータ設定			*/
/*======================================================================================*/
/*	[処理概要]	指定ポートのポートデータレジスタの指定BitにBitデータを設定する			*/
/*======================================================================================*/
/*	[記述形式]	SI_8 PortDrv_SetDataBit( E_PORTDRV_PORT_NUM port_num, 					*/
/*										E_PORTDRV_BIT_NUM bit_num, UI_8 set_bit);		*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_PORTDRV_PORT_NUM port_num	指定ポート番号	E_PORTDRV_PORT_NUM:enum定義	*/
/*				E_PORTDRV_BIT_NUM bit_num	指定ビット位置	E_PORTDRV_BIT_NUM:enum定義	*/
/*				UI_8 set_bit	設定情報												*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8 state;	処理実行ステータス											*/
/*======================================================================================*/
/*	[ 備  考 ]																			*/
/****************************************************************************************/
SI_8 PortDrv_SetDataBit( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BIT_NUM bit_num, UI_8 set_bit )
{
	UI_8 pdr_work;				/* ポートデータレジスタ演算用work変数 */
	SI_8 state; 				/* 処理実行ステータス */
	UI_16 mask_level; 			/* マスクレベル格納変数 */
	
	/* 引数1：指定ポート番号のデータ範囲判定 */
	if ( port_num < PORTDRV_PORT_MAX ) {
		
		/* 引数2：指定ビット位置のデータ範囲判定 */
		if ( bit_num < PORTDRV_BIT_MAX ) {
			
			/* 引数3：設定情報のデータ範囲判定 */
			if ( ( set_bit == D_LOW ) || ( set_bit == D_HIGH ) ) {
				state = D_OK;	/* 正常 */	
			} else {
				state = D_NG; 	/* 異常 */
			}
		} else {
			state = D_NG; 		/* 異常 */
		}
	} else {
		state = D_NG; 			/* 異常 */
	}
	
	/* 処理実行ステータスの正常判定 */
	if ( state == D_OK ) {
		
		(void) IntrDrv_GetMaskLevel( &mask_level );
		(void) IntrDrv_SetMaskLevel( PORT_INTR_LEVEL );			/* 割込み禁止 */
		pdr_work = *C_PortDataTbl[port_num];
		/* 設定情報Hiレベルを判定 */
		if ( ( set_bit == D_HIGH ) ) {
			pdr_work |= C_PortCheckBitTbl[bit_num];	/* ポートデータレジスタの指定ビット設定 */
		} else {
			pdr_work &= (UI_8)~C_PortCheckBitTbl[bit_num];	 /* ポートデータレジスタの指定ビット設定 */
		}
		*C_PortDataTbl[port_num] = pdr_work;
		(void) IntrDrv_SetMaskLevel( mask_level );						 /* 割込み復帰 */
	} else {
		/* 処理なし */
	}
	
	return state;	/* 戻り値:処理実行ステータス */
}

/****************************************************************************************/
/*	[モジュール名]	PortDrv_GetDataByte	[名称]	指定ポートのByteデータ取得				*/
/*======================================================================================*/
/*	[処理概要]	指定ポートのポートデータレジスタに指定Byteデータを取得する				*/
/*======================================================================================*/
/*	[記述形式]	SI_8 PortDrv_GetDataByte( E_PORTDRV_PORT_NUM port_num,					*/
/* 				, E_PORTDRV_BYTE_NUM byte_num, UI_8 *p_get_data )						*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_PORTDRV_PORT_NUM port_num	指定ポート番号	E_PORTDRV_PORT_NUM:enum定義	*/
/*				E_PORTDRV_BYTE_NUM byte_num	16bitﾏｲｺﾝとのIF用Byte設定情報				*/
/*				UI_8 *p_get_data	ポート情報コピー先アドレス							*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8 state;	処理実行ステータス											*/
/*======================================================================================*/
/*	[ 備  考 ]	ポート範囲外のときはLo レベルのデータを返却する							*/
/****************************************************************************************/
SI_8 PortDrv_GetDataByte( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BYTE_NUM byte_num, UI_8 *p_get_data )
{
	SI_8 state;				/* 処理実行ステータス */
	
	/* 引数2：ポート情報コピー先アドレス判定 */
	if ( p_get_data != D_NULL ) {

		/* 引数1：指定ポート番号のデータ範囲判定 */
		if ( port_num < PORTDRV_PORT_MAX ) {
		
			state = D_OK;									/* 正常 */
			*p_get_data = *C_PortDataDirectTbl[port_num];	/* 入力データダイレクトリードレジスタのByteデータ取得 */
		} else {
			state = D_NG;			/* 異常 */
			*p_get_data = D_LOW;	/* ポート範囲異常時、設定情報 Lo 固定 */
		}
	} else {
		state = D_NG; 				/* 異常 */
	}
	
	return state;	/* 戻り値:処理実行ステータス */
}

/****************************************************************************************/
/*	[モジュール名]	PortDrv_GetDataBit	[名称]	指定ポートの指定Bitデータ取得			*/
/*======================================================================================*/
/*	[処理概要]	指定ポートのポートデータレジスタの指定BitのBitデータを取得する			*/
/*======================================================================================*/
/*	[記述形式]	SI_8 PortDrv_GetDataBit( E_PORTDRV_PORT_NUM port_num, 					*/
/*										E_PORTDRV_BIT_NUM bit_num, UI_8 *p_get_bit )	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_PORTDRV_PORT_NUM port_num	指定ポート番号	E_PORTDRV_PORT_NUM:enum定義	*/
/*				E_PORTDRV_BIT_NUM bit_num	指定ビット位置	E_PORTDRV_BIT_NUM:enum定義	*/
/*				UI_8 *p_get_bit		ポート情報コピー先アドレス							*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8 state;	処理実行ステータス											*/
/*======================================================================================*/
/*	[ 備  考 ]	ポート範囲外のときは Loレベルのデータを返却する							*/
/****************************************************************************************/
SI_8 PortDrv_GetDataBit( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BIT_NUM bit_num, UI_8 *p_get_bit )
{
	UI_8 pddr_temp;					/*  入力データダイレクトリードレジスタ値格納用 */
	SI_8 state; 					/* 処理実行ステータス */
	
	/* 引数3：ポート情報コピー先アドレス判定 */
	if ( p_get_bit != D_NULL ) {

		/* 引数1：指定ポート番号のデータ範囲判定 */
		if ( port_num < PORTDRV_PORT_MAX ) {
				
			/* 引数2：指定ビット位置のデータ範囲判定 */
			if ( bit_num < PORTDRV_BIT_MAX ) {
				state = D_OK;		/* 正常 */	
			} else {
				state = D_NG; 		/* 異常 */
				*p_get_bit = D_LOW;	/* ポート範囲異常時、設定情報 Lo 固定 */
			}
		} else {
			state = D_NG; 			/* 異常 */
			*p_get_bit = D_LOW;		/* ポート範囲異常時、設定情報 Lo 固定 */
		}
	} else {
		state = D_NG; 				/* 異常 */
	}
	
	/* 処理実行ステータスの正常判定 */
	if ( state == D_OK ) {
		
		pddr_temp = *C_PortDataDirectTbl[port_num];
		/* 設定情報 Hiレベルを判定 */
		if ( ( pddr_temp & C_PortCheckBitTbl[bit_num] ) == C_PortCheckBitTbl[bit_num] ) {
			*p_get_bit = D_HIGH;	/* Hiレベル */
		} else {
			*p_get_bit = D_LOW;		/* Loレベル */
		}
	} else {
		/* 処理なし */  
	}
	
	return state;	/* 戻り値:処理実行ステータス */
}

#if (PORT_DRV_FUNC_CHANGE == 1)
/****************************************************************************************/
/*	[モジュール名]	PortDrv_SetFuncBit	[名称]	機能レジスタ切替処理					*/
/*======================================================================================*/
/*	[処理概要]	機能レジスタの切替を行う。IDに従って、レジスタ設定値を変更する。		*/
/*======================================================================================*/
/*	[記述形式]	SI_8 PortDrv_SetFuncBit( E_PORTDRV_PORT_NUM port_num, 					*/
/*							E_PORTDRV_BIT_NUM bit_num, E_PORT_FUNC_CHANGE_ID set_id )	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_PORTDRV_PORT_NUM port_num	指定ポート番号	E_PORTDRV_PORT_NUM:enum定義	*/
/*				E_PORTDRV_BIT_NUM bit_num	指定ビット位置	E_PORTDRV_BIT_NUM:enum定義	*/
/*				E_PORT_FUNC_CHANGE_ID set_id	指定ID									*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8 state;	処理実行ステータス											*/
/*======================================================================================*/
/*	[ 備  考 ]																			*/
/****************************************************************************************/
SI_8 PortDrv_SetFuncBit( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BIT_NUM bit_num, E_PORT_FUNC_CHANGE_ID set_id )
{
	UI_8 i;								/* ループカウンタ */
	__io volatile UI_8 *const *table;	/* 機能レジスタテーブルポインタ格納変数 */
	const UI_16 *const *set1;			/* 機能レジスタ設定値ポインタ格納変数1 */
	const UI_16 *set2;					/* 機能レジスタ設定値ポインタ格納変数2 */
	UI_8 set_data1;						/* 機能レジスタ設定値格納変数1 */
	const UI_16 *set_data2;				/* 機能レジスタ設定値格納変数2 */
	UI_16 set_data3;					/* 機能レジスタ設定値格納変数3 */
	UI_16 set_data4;					/* データ方向&ポート機能レジスタ設定値格納変数 */
	SI_8 state;							/* 処理実行ステータス */
	UI_16 mask_level;					/* マスクレベル格納変数 */
	UI_8 mask;							/* マスク用変数 */
	UI_8 shift;							/* ビットシフト用変数 */
	UI_8 func;							/* 選択機能 */
	UI_8 work;							/* レジスタ演算用work変数 */
	UI_8 ddr_work;						/* データ方向レジスタ演算用work変数 */
	UI_8 pfr_work;						/* ポート機能レジスタ演算用work変数 */
	
	state = D_NG; 						/* 異常 */
	func = 0;							/* 機能初期化 */
	
	/* 引数1：指定ポート番号のデータ範囲判定 */
	if ( port_num < PORTDRV_PORT_MAX ) {
		
		/* 引数2：指定ビット位置のデータ範囲判定 */
		if ( bit_num < PORTDRV_BIT_MAX ) {
			
			/* 引数3：指定IDの機能が該当ポートに存在するか判定 */
			set1 = C_FuncOperateTbl[port_num];	/* 機能切替設定値ポート選択テーブルを差す */
			set2 = set1[bit_num];				/* 機能切替設定値ビット選択テーブルを差す */
			for ( i = 0; i < C_PortFuncChangeIdNumTbl[port_num][bit_num]; i++ ) {
				
				set_data1 = (UI_8)(*(set2 + ((C_PortFuncChangeTbl[port_num] * i) 
										+ (C_PortFuncChangeTbl[port_num] - 1))));	/* 機能レジスタ設定値値算出 */
				
				/* 指定IDが機能切替設定値テーブルの機能に存在 */
				if ( ( set_id == set_data1 ) && ( set_id != NO_USE ) ) {
					state = D_OK;		/* 正常 */
					func = i;
				}
			}
		} else {
			state = D_NG; 				/* 異常 */
		}
	} else {
		state = D_NG; 					/* 異常 */
	}
	
	/* 処理実行ステータスの正常判定 */
	if ( state == D_OK ) {
		
		(void) IntrDrv_GetMaskLevel( &mask_level );
		(void) IntrDrv_SetMaskLevel( PORT_INTR_LEVEL );					/* 割込み禁止 */
		
		/* 機能切替前処理 */
		table = C_PortFuncTbl[port_num];								/* 機能レジスタアドレステーブルを差す */
		ddr_work = *table[PORT_TABLE_DDR];
		ddr_work |= C_PortCheckBitTbl[bit_num];			/* データ方向を出力に設定 */
		*table[PORT_TABLE_DDR] = ddr_work;

		pfr_work = *table[PORT_TABLE_PFR];
		pfr_work &= (UI_8)~C_PortCheckBitTbl[bit_num];	/* ポート機能をポート側に設定*/
		*table[PORT_TABLE_PFR] = pfr_work;
		/* 機能切替処理 */
		for ( i = PORT_TABLE_EPFR; i < C_PortFuncChangeNumTbl[port_num]; i++ ) {
			
			set_data2 = set2 + (C_PortFuncChangeTbl[port_num] * func);	/* 機能レジスタ設定値算出 */
			set_data3 = *(set_data2 + i);								/* 機能レジスタ設定値算出 */
			
			/* 設定値が存在 */
			if ( set_data3 != D_NULL ) {
				
				/* マスク処理なし(元のレジスタ値から変更なし)の場合 */
				if ( (set_data3 & C_PortFuncCheckBitTbl[BIT_FUNC_NUM_15]) == C_PortFuncCheckBitTbl[BIT_FUNC_NUM_15] ) {
					/* 処理なし */
				} else if ( (set_data3 & C_PortFuncCheckBitTbl[BIT_FUNC_NUM_14]) == C_PortFuncCheckBitTbl[BIT_FUNC_NUM_14] ) {
				/* ANDマスク処理(レジスタ値と設定値の&を取得)の場合 */
					work = *table[i];
					work &= (UI_8)set_data3;	/* 機能切替レジスタに値を設定 */
					*table[i] = work;
				} else if ( (set_data3 & C_PortFuncCheckBitTbl[BIT_FUNC_NUM_13]) == C_PortFuncCheckBitTbl[BIT_FUNC_NUM_13] ) {
				/* ORマスク処理(レジスタ値と設定値の|を取得)の場合 */
					work = *table[i];
					work |= (UI_8)set_data3;	/* 機能切替レジスタに値を設定 */
					*table[i] = work;
				} else if ( (set_data3 & C_PortFuncCheckBitTbl[BIT_FUNC_NUM_12]) == C_PortFuncCheckBitTbl[BIT_FUNC_NUM_12] ) {
				/* AND/ORマスク処理(レジスタ値と設定値の"&"かつ"|"両方の処理)の場合 */
					mask = *table[i] & (UI_8)set_data3;						/* 該当ビットを0でマスク */
					shift = (UI_8)((BIT_GET & set_data3) >> BIT_SHIFT);		/* 1でマスクするビット位置の抽出 */
					mask |= C_PortCheckBitTbl[shift];						/* 該当ビットを1でマスク */
					*table[i] = mask;										/* 機能切替レジスタに値を設定 */
				} else {
					/* 処理なし */
				}
			}
		}
		
		/* データ方向&ポート機能設定処理 */
		for ( i = 0;	i < PORT_TABLE_EPFR; i++ ) {
			
			set_data4 = *(set_data2 + i);			/* 機能レジスタ設定値算出 */
		
			/* ANDマスク処理(レジスタ値と設定値の&を取得)の場合 */
			if ( (set_data4 & C_PortFuncCheckBitTbl[BIT_FUNC_NUM_14]) == C_PortFuncCheckBitTbl[BIT_FUNC_NUM_14] ) {
				work = *table[i];
				work &= (UI_8)set_data4;		/* i = 0:データ方向レジスタ、i = 1:ポート機能レジスタの設定 */
				*table[i] = work;
			} else if ( (set_data4 & C_PortFuncCheckBitTbl[BIT_FUNC_NUM_13]) == C_PortFuncCheckBitTbl[BIT_FUNC_NUM_13] ) {
			/* ORマスク処理(レジスタ値と設定値の|を取得)の場合 */
				work = *table[i];
				work |= (UI_8)set_data4;		/* i = 0:データ方向レジスタ、i = 1:ポート機能レジスタの設定 */
				*table[i] = work;
			} else {
				/* 処理なし */
			}
		}
		
		(void) IntrDrv_SetMaskLevel( mask_level );	/* 割込み復帰 */
		
	} else {
		/* 処理なし */
	}

	return state;	/* 戻り値:処理実行ステータス */
}
#endif	/* PORT_DRV_FUNC_CHANGE == 1 */

/****************************************************************************************/
/*	[モジュール名]	PortDrv_Init		[名称]	GPIOﾄﾞﾗｲﾊﾞﾚｼﾞｽﾀ初期設定処理				*/
/*======================================================================================*/
/*	[処理概要]	レジスタ初期設定時に本関数を呼び、初期化を行う。						*/
/*======================================================================================*/
/*	[記述形式]	void PortDrv_Init( E_INIT_TYPE req  )									*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_INIT_TYPE				req		：起動種別								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void PortDrv_Init( E_INIT_TYPE req )
{
	UI_8 i;						/* ループカウンタ */
	UI_8 dummy;					/* レジスタ値格納変数 */

	/* RAM初期化 */
	S_SelfcheckState = D_OK;	/* リフレッシュ動作中 */
	S_ChkSetReg = D_NG;			/* レジスタ設定の禁止 */

	/* ポートデータレジスタ初期設定 */
	if (req == E_INIT_RESET) {			/* CPUリセットによる起動 */
		for ( i = 0; i < PORTDRV_PORT_MAX; i++ ) {	/* ループ条件: ループカウンタ < ポートデータレジスタ番号MAX値 */
			*C_PortDataTbl[i] = C_PortInitTbl[i];		/* ポートデータレジスタの初期設定 */
		}
	} else if (req == E_INIT_WAKEUP) {			/* 省電力解除（WAKEUP）による起動 */
		for ( i = 0; i < PORTDRV_PORT_MAX; i++ ) {	/* ループ条件: ループカウンタ < ポートデータレジスタ番号MAX値 */
			if ( C_PortBitMaskPositionTbl[i] != D_NULL ) {	/* ビットマスクを行うビットが存在 */
				dummy = *C_PortDataDirectTbl[i];			/* 入力データダイレクトリードレジスタ値取得 */
				dummy &= C_PortBitMaskPositionTbl[i];		/* ビットマスク */
				dummy |= ( UI_8 )( C_PortInitTbl[i] & (UI_8)~C_PortBitMaskPositionTbl[i] );		/* マスク箇所以外の設定 */
				*C_PortDataTbl[i] = dummy;					/* ポートデータレジスタの初期設定 */
			} else {
				*C_PortDataTbl[i] = C_PortInitTbl[i];		/* ポートデータレジスタの初期設定 */
			}
		}
	} else {	/* ビットマスク処理の対象外 */
		/* 何もしない */
	}

	switch ( req ) {
	case E_INIT_RESET:			/* CPUリセットによる起動 */
	case E_INIT_WAKEUP:			/* 省電力解除（WAKEUP）による起動 */
		
		/* レジスタ初期設定 */
		for ( i = 0; i < PORT_MAX; i++ ) {	/* ループ条件: ループカウンタ < ポートデータレジスタ番号MAX値 */
			*C_PortFunctionTbl[i] = 0;								/* ポート機能レジスタ初期設定 */
			*C_PullControlTbl[i] = C_PullControlInitTbl[i];			/* プルアップダウン制御レジスタ初期設定 */
			*C_PullEnableTbl[i] = C_PullEnableInitTbl[i];			/* プルアップダウン許可レジスタ初期設定 */
			*C_PortInputLevelTbl[i] = C_PortInputLevelInitTbl[i];	/* ポート入力レベル選択レジスタ初期設定 */
			*C_ExtendedPortInputLevelTbl[i] = C_ExtendedPortInputLevelInitTbl[i];	/* 拡張ポート入力レベル選択レジスタ初期設定 */
			*C_PortOutputDriveTbl[i] = C_PortOutputDriveInitTbl[i];	/* ポート出力駆動レジスタ初期設定 */
			switch ( i ) {											/* ループカウンタの判定 */
			case PORT_NUM_1:	/* ループカウンタ：1 */
			case PORT_NUM_2:	/* ループカウンタ：2 */
			case PORT_NUM_3:	/* ループカウンタ：3 */
			case PORT_NUM_6:	/* ループカウンタ：6 */
			case PORT_NUM_7:	/* ループカウンタ：7 */
			case PORT_NUM_8:	/* ループカウンタ：8 */
				*C_ExtendedPortOutputDriveTbl[i] = C_ExtendedPortOutputDriveInitTbl[i];	/* 拡張ポート出力駆動レジスタ初期設定 */
				break;
			default:			/* 上記以外 */
				break;
			}
		}

		/* 拡張ポート機能レジスタ初期設定 */
		for ( i = 0; i < PORTDRV_EPFR_MAX; i++ ) {	/* ループ条件: ループカウンタ < ポートデータレジスタ番号MAX値 */
			*C_ExtendedPortFunctionTbl[i] = C_ExtendedPortFunctionInitTbl[i];	/* 拡張ポート機能レジスタ初期設定 */
		}

		/* レジスタ初期設定 */
		for ( i = 0; i < PORT_MAX; i++ ) {	/* ループ条件: ループカウンタ < ポートデータレジスタ番号MAX値 */
			*C_DataDirectionTbl[i] = C_DataDirectionInitTbl[i];		/* データ方向レジスタ初期設定 */
			*C_PortFunctionTbl[i] = C_PortFunctionInitTbl[i];		/* ポート機能レジスタ初期設定 */
		}

		PORTEN_GPORTEN = INIT_PORTEN;		/* ポート入力許可レジスタ初期設定 */

		break;
		
	case E_INIT_IGN_ON:			/* IGN ON */
		break;
	case E_INIT_RET_NORMAL_VOL:	/* 低電圧復帰 */
		break;
	case E_INIT_INTERVAL_WAKEUP:	/* 間欠起動ウェイクアップ */
		PortDrv_Sleep();			/* レジスタが初期化されているので最低限ポートの初期化が必要 */
		break;
	default:					/* 上記以外 */
		break;
	}
	
}

/****************************************************************************************/
/*	[モジュール名]	PortDrv_Sleep	[名称]	GPIOﾄﾞﾗｲﾊﾞﾚｼﾞｽﾀ停止設定処理					*/
/*======================================================================================*/
/*	[処理概要]	省電力モード遷移時に本関数を呼び、初期化を行う。						*/
/*======================================================================================*/
/*	[記述形式]	void PortDrv_Sleep( void )												*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void PortDrv_Sleep( void )
{
	UI_8 i;						/* ループカウンタ */
	UI_8 dummy;					/* レジスタ値格納変数 */
	
	/* ポートデータレジスタ停止設定 */
	for ( i = 0; i < PORTDRV_PORT_MAX; i++ ) {	/* ループ条件: ループカウンタ < ポートデータレジスタ番号MAX値 */
		if ( C_PortBitMaskPositionTbl[i] != D_NULL ) {		/* ビットマスクを行うビットが存在 */
			dummy = *C_PortDataDirectTbl[i];				/* 入力データダイレクトリードレジスタ値取得 */
			dummy &= C_PortBitMaskPositionTbl[i];			/* ビットマスク */
			dummy |= ( UI_8 )( C_PortStopTbl[i] & (UI_8)~C_PortBitMaskPositionTbl[i] );		/* マスク箇所以外の設定 */
			*C_PortDataTbl[i] = dummy;						/* ポートデータレジスタの停止設定 */
		} else {
			*C_PortDataTbl[i] = C_PortStopTbl[i];			/* ポートデータレジスタの停止設定 */
		}
	}

	/* レジスタ停止設定 */
	for ( i = 0; i < PORT_MAX; i++ ) {	/* ループ条件: ループカウンタ < ポートデータレジスタ番号MAX値 */
		*C_PortFunctionTbl[i] = 0;								/* ポート機能レジスタ初期設定 */
		*C_PullControlTbl[i] = C_PullControlStopTbl[i];			/* プルアップダウン制御レジスタ停止設定 */
		*C_PullEnableTbl[i] = C_PullEnableStopTbl[i];			/* プルアップダウン許可レジスタ停止設定 */
		*C_PortInputLevelTbl[i] = C_PortInputLevelStopTbl[i];	/* ポート入力レベル選択レジスタ停止設定 */
		*C_ExtendedPortInputLevelTbl[i] = C_ExtendedPortInputLevelStopTbl[i];	/* 拡張ポート入力レベル選択レジスタ停止設定 */
		*C_PortOutputDriveTbl[i] = C_PortOutputDriveStopTbl[i];	/* ポート出力駆動レジスタ停止設定 */
		switch ( i ) {											/* ループカウンタの判定 */
		case PORT_NUM_1:		/* ループカウンタ：1 */
		case PORT_NUM_2:		/* ループカウンタ：2 */
		case PORT_NUM_3:		/* ループカウンタ：3 */
		case PORT_NUM_6:		/* ループカウンタ：6 */
		case PORT_NUM_7:		/* ループカウンタ：7 */
		case PORT_NUM_8:		/* ループカウンタ：8 */
			*C_ExtendedPortOutputDriveTbl[i] = C_ExtendedPortOutputDriveStopTbl[i];	/* 拡張ポート出力駆動レジスタ停止設定 */
			break;
		default:				/* 上記以外 */
			break;
		}
	}

	/* 拡張ポート機能レジスタ停止設定 */
	for ( i = 0; i < PORTDRV_EPFR_MAX; i++ ) {	/* ループ条件: ループカウンタ < 拡張ポート機能レジスタ数 */
		*C_ExtendedPortFunctionTbl[i] = C_ExtendedPortFunctionStopTbl[i];	/* 拡張ポート機能レジスタ停止設定 */
	}

	/* レジスタ停止設定 */
	for ( i = 0; i < PORT_MAX; i++ ) {	/* ループ条件: ループカウンタ < ポートデータレジスタ番号MAX値 */
		*C_DataDirectionTbl[i] = C_DataDirectionStopTbl[i];		/* データ方向レジスタ停止設定 */
		*C_PortFunctionTbl[i] = C_PortFunctionStopTbl[i];		/* ポート機能レジスタ停止設定 */
	}
	
	PORTEN_GPORTEN = STOP_PORTEN;		/* ポート入力許可レジスタ停止設定 */

}

/****************************************************************************************/
/*	[モジュール名]	PortDrv_Refresh	[名称]												*/
/*======================================================================================*/
/*	[処理概要]	レジスタ再設定時に本関数を呼び、初期化を行う。							*/
/*======================================================================================*/
/*	[記述形式]	void PortDrv_Refresh( void )											*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void PortDrv_Refresh( void )
{
	UI_8 i;						/* ループカウンタ */
	
	if ( S_SelfcheckState != D_NG) {/* セルフチェック動作中? */
		S_ChkSetReg = D_NG;	/* レジスタ設定の禁止 */
		for ( i = 0; i < PORT_MAX; i++ ) {	/* ループ条件: ループカウンタ < ポートデータレジスタ番号MAX値 */
			*C_DataDirectionTbl[i] = C_DataDirectionRefreshTbl[i];	/* データ方向レジスタ再設定 */
		}
	} else {
		S_ChkSetReg = D_OK;	/* レジスタ設定の許可 */
	}
}

/****************************************************************************************/
/*	[モジュール名]	PortDrv_SetSelfCheckStat	[名称]	セルフチェック状態設定処理		*/
/*======================================================================================*/
/*	[処理概要]	ラインテスト機能側からセルフチェック状態を取得し、RAMへ格納する			*/
/*======================================================================================*/
/*	[記述形式]	void PortDrv_SetSelfCheckStat( SI_8 stat )								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	SI_8		stat	レジスタリフレッシュ動作情報						*/
/*		（D_NG :リフレッシュ動作NG(セルフチェック駆動) / D_NG以外 :リフレッシュ動作中）	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	設定されなければRAM初期化によりリフレッシュ動作中（0 : D_OK相当）となる	*/
/****************************************************************************************/
void PortDrv_SetSelfCheckStat(SI_8 stat)
{
	S_SelfcheckState = stat;		/* セルフチェック状態を設定  */
}

/****************************************************************************************/
/*	[モジュール名]	PortDrv_GetPermissionStat	[名称]	GPIOの再設定許可禁止状態取		*/
/*======================================================================================*/
/*	[処理概要]	レジスタ設定許可・禁止状態を外部機能へ公開する							*/
/*======================================================================================*/
/*	[記述形式]	SI_8 PortDrv_GetPermissionStat( void )									*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8		レジスタ設定の状態（D_NG：禁止/D_OK：許可）					*/
/*======================================================================================*/
/*	[ 備  考 ]	メイン中で行なわれるPortDrv_Refresh処理の途中で、セルフチェック状態設定	*/
/*	処理（10mss処理）が呼ばれた後、メインが行なわれる前に続けてデータ出力が行なわれる	*/
/*（破綻した10ms処理が、2回連続で行なわれる）ケースを回避するため、設定の確認を行なう。	*/
/****************************************************************************************/
SI_8 PortDrv_GetPermissionStat(void)
{
	return S_ChkSetReg;			/* レジスタ設定の禁止/許可状態  */
}

/****************************************************************************************/
/*	[モジュール名]	PortDrv_SetDirectionBit	[名称]	指定ポートの指定Bit方向設定			*/
/*======================================================================================*/
/*	[処理概要]	指定ポートの指定Bitのデータ方向を設定する								*/
/*======================================================================================*/
/*	[記述形式]	SI_8 PortDrv_SetDirectionBit( E_PORTDRV_PORT_NUM port_num, 				*/
/*				E_PORTDRV_BIT_NUM bit_num, UI_8 set_direction )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_PORTDRV_PORT_NUM port_num	指定ポート番号	E_PORTDRV_PORT_NUM:enum定義	*/
/*				E_PORTDRV_BIT_NUM bit_num	指定ビット位置	E_PORTDRV_BIT_NUM:enum定義	*/
/*				UI_8 set_direction	設定情報											*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8 state;	処理実行ステータス											*/
/*======================================================================================*/
/*	[ 備  考 ]	なし																	*/
/****************************************************************************************/
SI_8 PortDrv_SetDirectionBit( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BIT_NUM bit_num, UI_8 set_direction )
{
	UI_8 ddr_work;				/* データ方向レジスタ演算用work変数 */
	SI_8 state; 				/* 処理実行ステータス */
	UI_16 mask_level; 			/* マスクレベル格納変数 */
	
	/* 引数1：指定ポート番号のデータ範囲判定 */
	if ( port_num < PORTDRV_PORT_MAX ) {
		
		/* 引数2：指定ビット位置のデータ範囲判定 */
		if ( bit_num < PORTDRV_BIT_MAX ) {
			
			/* 引数3：設定情報のデータ範囲判定 */
			if ( ( set_direction == D_LOW ) || ( set_direction == D_HIGH ) ) {
				state = D_OK;	/* 正常 */	
			} else {
				state = D_NG; 	/* 異常 */
			}
		} else {
			state = D_NG; 		/* 異常 */
		}
	} else {
		state = D_NG; 			/* 異常 */
	}
	
	/* 処理実行ステータスの正常判定 */
	if ( state == D_OK ) {
		

		(void) IntrDrv_GetMaskLevel( &mask_level );
		(void) IntrDrv_SetMaskLevel( PORT_INTR_LEVEL );			/* 割込み禁止 */
		ddr_work = *C_DataDirectionTbl[port_num];
		/* 設定情報Hiレベルを判定 */
		if ( ( set_direction == D_HIGH ) ) {
			ddr_work |= C_PortCheckBitTbl[bit_num];	/* ポート方向レジスタの指定ビット設定 */
		} else {
			ddr_work &= (UI_8)~C_PortCheckBitTbl[bit_num];	 /* ポート方向レジスタの指定ビット設定 */
		}
		*C_DataDirectionTbl[port_num] = ddr_work;
		(void) IntrDrv_SetMaskLevel( mask_level );				/* 割込み復帰 */
		
	} else {
		/* 処理なし */
	}
	
	return state;	/* 戻り値:処理実行ステータス */
}
