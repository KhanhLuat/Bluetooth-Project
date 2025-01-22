/************************************************************************************************/
/* 客先名		:	機種依存モジュール															*/
/* 機種名		:	-																			*/
/* ﾏｲｺﾝｿﾌﾄﾃｰﾏ名	:	-																			*/
/*==============================================================================================*/
/* 作成ﾌｧｲﾙ名	:	SSFTxxx_FR81_SystemClock_Drv_Config.h										*/
/* 				:	ｼｽﾃﾑｸﾛｯｸﾄﾞﾗｲﾊﾞﾍｯﾀﾞ															*/
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
#ifndef __MB91570_SYSTEMCLOCK_DRV_CONFIG_H__
#define __MB91570_SYSTEMCLOCK_DRV_CONFIG_H__

/* 内部定数 */
#ifdef __MB91570_SYSTEMCLOCK_DRV_INTERNAL__
/*==============================================================================*/
/*	typedef定義（外部非公開）													*/
/*==============================================================================*/
/* なし */


/*==============================================================================*/
/*	define定義（外部非公開）													*/
/*==============================================================================*/


#define INIT_DIVR0		(0x00)		/* 分周設定レジスタ0 (未使用) */
									/*	<07-05>	DIVB	: 000		(ベースクロック分周設定ビット φ(分周なし)) */
									/*	<04-00>	-		: 00000		(予約bit) */
									/* ------------------------------------------------------------------- */
#define INIT_DIVR1		(0x10)		/* 分周設定レジスタ1 */
									/*	<07>	TSTP	: 0			(外部バスクロック停止許可ビット 停止しない) */
									/*	<06-04>	DIVT	: 001		(外部バスクロック分周設定ビット φ/2(2分周)) */
									/*	<03-00>	-		: 0000		(予約bit) */
									/* ------------------------------------------------------------------- */
#define INIT_DIVR2		(0x40)		/* 分周設定レジスタ2 (未使用) */
									/*	<07-04>	DIVP	: 0100		(周辺クロック分周設定ビット φ/5(5分周)) */
									/*	<03-00>	-		: 0000		(予約bit) */
									/* ------------------------------------------------------------------- */
#define INIT_CSELR		(0x62)		/* クロックソース設定レジスタ (未使用) */
									/*	<07>	SCEN	: 0			(サブクロック発振ビット 停止) */
									/*	<06>	PCEN	: 1			(PLL 発振ビット 発振) */
									/*	<05>	MCEN	: 1			(メインクロック発振 開始) */
									/*	<04-02>	-		: 000		(予約bit) */
									/*	<01-00>	CKS		: 10		(ソースクロック選択ビット PLL/SSCG クロック) */
									/* ------------------------------------------------------------------- */
#define INIT_MTMCR		(0x00)		/* メインタイマ制御レジスタ (未使用) */
									/*	<07>	MTIF	: 0			(メインタイマ割込みフラグビット 停止) */
									/*	<06>	MTIE	: 0			(メインタイマ割込み許可ビット 停止) */
									/*	<05>	MTC		: 1			(メインタイマクリアビット 開始) */
									/*	<04>	MTE		: 000		(メインタイマ動作許可ビット ) */
									/*	<03-00>	MTS		: 10		(メインタイマ周期選択ビット PLL/SSCG クロック) */
									/* ------------------------------------------------------------------- */
#define INIT_STMCR		(0x00)		/* サブタイマ制御レジスタ (未使用) */
									/*	<07>	STIF	: 0			(サブタイマ割込みフラグビット クリア) */
									/*	<06>	STIE	: 0			(サブタイマ割込みビット 割込み禁止(初期値)) */
									/*	<05>	STC		: 0			(サブタイマクリア 何もしない) */
									/*	<04>	STE		: 0			(サブタイマ動作許可ビット 動作禁止) */
									/*	<03>	-		: 0			(予約bit) */
									/*	<02-00>	STS		: 000		(サブタイマ周期選択ビット 2^8*サブクロック周期) */
									/* ------------------------------------------------------------------- */
#define INIT_PLLCR		(0x00B0)	/* PLL設定レジスタ (未使用) */
									/* <15-08>	-		: 0			(予約bit) */
									/* <07-04>	POSW	: 1011		(PLLクロック発振安定待ち選択ビット 2^12*メインクロック周期) */
									/* <03-00>	PDS		: 0000		(PLL入力クロック分周選択ビット PLLおよびSSCG入力クロック = メインクロック ) */
									/* ------------------------------------------------------------------- */
#define INIT_CSTBR		(0x0C)		/* 発振安定待ち設定レジスタ */
									/* <07>		-		: 0			(予約bit) */
									/* <06-04>	SOSW	: 000		(サブクロック発振安定待ち選択ビット 2^8*サブクロック周期（初期値) ) */
									/* <03-00>	MOSW	: 1100		(メインクロック発振安定待ち選択ビット 2^17*メインクロック周期) */
									/* ------------------------------------------------------------------- */
#define INIT_PTMCR		(0x00)		/* PLLクロック発振安定待ちタイマ設定レジスタ (未使用) */
									/* <07>		PTIF	: 0			(PLLクロック発振安定待ちタイマ割込みフラグ) */
									/* <06>		PTIE	: 0			(PLLクロック発振安定待ちタイマ割込み許可ビット 割込み禁止（初期値））*/
								 	/* <05-00>	-		: 0			(予約bit ) */
								 	/* ------------------------------------------------------------------- */
#define INIT_CCPSSELR	(0x00)		/* PLL/SSCGクロック選択レジスタ (未使用) */
									/* <07-01>	-		: 0			(予約bit) */
									/* <00>		PCSEL	: 0			(PLL/SSCGクロックソース選択ビット PLLを選択) */
									/* ------------------------------------------------------------------- */
#define INIT_CCPSDIVR	(0x11)		/* PLL/SSCG出力クロック分周設定レジスタ (未使用) */
									/* <07>		-		: 0			(予約bit) */
									/* <06-04>	PODS	: 001		(PLLマクロ発振クロック分周比選択ビット PLLクロック＝PLLマクロ発振クロック/4) */
									/* <03>		-		: 0			(予約bit) */
									/* <02-00>	SODS	: 001		(SSCGマクロ発振クロック分周比選択ビット SSCGクロック＝SSCGマクロ発振クロック/4) */
									/* ------------------------------------------------------------------- */
#define INIT_CCPLLFBR	(0x4F)		/* PLLフィードバック分周設定レジスタ (未使用) */
									/* <07>		-		: 0			(予約bit) */
									/* <06-00>	IDIV	: 1001111	(PLLマクロFB入力分周比設定ビット 80) */
									/* ------------------------------------------------------------------- */
#define INIT_CCSSFBR0	(0x09)		/* SSCGフィードバック分周設定レジスタ0 (未使用) */
									/* <07-06>	-		: 00		(予約bit) */
									/* <05-00>	NDIV	: 001001	(SSCGマクロFB入力N分周比設定ビット 10 ) */
									/* ------------------------------------------------------------------- */
#define INIT_CCSSFBR1	(0x07)		/* SSCGフィードバック分周設定レジスタ1 (未使用) */
									/* <07-05>	-		: 0			(予約bit) */
									/* <04-00>	PDIV	: 00111		(SSCGマクロFB入力P分周比設定ビット 8) */
									/* ------------------------------------------------------------------- */
#define INIT_CCSSCCR0	(0x00)		/* SSCGコンフィグ設定レジスタ0 (未使用) */
									/* <07-04>	-		: 0			(予約bit) */
									/* <03-02>	SFREQ	: 00		(拡散モジュレーション周波数設定ビット DownSpread) */
									/* <01>		SMODE	: 0			(拡散モジュレーションモード選択ビット DownSpread) */
									/* <00>		SSEN	: 0			(拡散スペクトル許可ビット 拡散スペクトラム無効) */
									/* ------------------------------------------------------------------- */
#define INIT_CCSSCCR1	(0x0000)	/* SSCGコンフィグ設定レジスタ1 (未使用) */
									/* <15-13>	RATESEL	: 000		(拡散モジュレーションレート選択ビット 0.5%) */
									/* <12-00>	-		: 0			(予約bit) */
									/* ------------------------------------------------------------------- */
#define INIT_CCCGRCR0	(0x00)		/* クロックギアコンフィグ設定レジスタ0 (未使用) */
									/* <07-06>	GRSTS	: 00		(クロックギアステータスフラグビット ) */
									/* <05-02>	-		: 0			(予約bit) */
									/* <01>		GRSTR	: 0			(クロックギアスタートビット 動作に影響なし) */
									/* <00>		GREN	: 0			(クロックギアイネーブルビット クロックギアを使用しない) */
									/* ------------------------------------------------------------------- */
#define INIT_CCCGRCR1	(0x00)		/* クロックギアコンフィグ設定レジスタ1 (未使用) */
									/* <07-06>	GRST	: 00		(クロックギアステップ選択ビット 1ステップ) */
									/* <05-00>	GRSTN	: 0			(クロックギア開始ステップ選択ビット 0ステップ) */
									/* ------------------------------------------------------------------- */
#define INIT_CCCGRCR2	(0x00)		/* クロックギアコンフィグ設定レジスタ2 (未使用) */
									/* <07-00>	GRLP	: 0			(クロックギア繰返し回数選択ビット ループ1回) */
									/* ------------------------------------------------------------------- */
#define INIT_CCRTSELR	(0x00)		/* RTC/PMUクロック選択レジスタ (未使用) */
									/* <07>		CST		: 0			(クロックソース選択ステータスモニタビット クロック切り替え完了) */
									/* <06-01>	-		: 0			(予約bit) */
									/* <00>		CSC		: 0			(クロックソース選択ビット メイン発振クロック) */
									/* ------------------------------------------------------------------- */
#define INIT_CCPMUCR0	(0x00)		/* PMUクロック分周設定レジスタ0 (未使用) */
									/* <07>		FST		: 0			(F分周器ステータスモニタビット 書き込み値を反映済み) */
									/* <06-02>	-		: 0			(予約bit) */
									/* <01-00>	FDOV	: 0			(F分周比設定ビット 128分周(初期値)) */
									/* ------------------------------------------------------------------- */
#define INIT_CCPMUCR1	(0x00)		/* PMUクロック分周設定レジスタ1 (未使用) */
									/* <07>		GST		: 0			(G分周器ステータスモニタビット 書き込み値を反映済み) */
									/* <06-05>	-		: 0			(予約bit) */
									/* <04-00>	GDIV	: 0			(G分周比設定ビット 分周しない（初期値）） */
									/* ------------------------------------------------------------------- */
#define INIT_SACR		(0x00)		/* 同期/非同期コントロールレジスタ (未使用) */
									/* <07-01>	-		: 0			(予約bit) */
									/* <00>		M		: 0			(周辺クロック(PCLK2)の同期/非同期設定レジスタビット 同期(CPU/周辺共にPLL/SSCG クロック)) */
									/* ------------------------------------------------------------------- */
#define INIT_PICD		(0x00)		/* 周辺インタフェースクロック分周 (未使用) */
									/* <07-04>	-		: 0			(予約bit) */
									/* <03-00>	PDIV	: 0000		(周辺クロック分周比設定ビット 分周なし) */

#define INIT_CCRTSELR_CSC	(0)		/* RTC/PMUクロック選択レジスタ CSCビット初期設定値 0:メインクロックを選択 */
#define INIT_CCPMUCR0_FDIV	(0)		/* PMUクロック分周設定レジスタ0 FDIV 初期設定値 00:128分周 */
#define INIT_CCPMUCR1_GDIV	(0)		/* PMUクロック分周設定レジスタ1 GDIV 初期設定値 0000:分周なし */

#define STOP_DIVR0		(0x00)		/* INIT_DIVR0と同じ設定値 (未使用) */
#define STOP_DIVR1		(0x10)		/* 分周設定レジスタ1 */
									/*	<07>	TSTP	: 0			(外部バスクロック停止許可ビット 停止しない) */
									/*	<06-04>	DIVT	: 001		(外部バスクロック分周設定ビット φ/2(2分周)) */
									/*	<03-00>	-		: 0000		(予約bit) */
#define STOP_DIVR2		(0x40)		/* INIT_DIVR2と同じ設定値 (未使用) */
#define STOP_CSELR		(0x20)		/* クロックソース設定レジスタ (未使用) */
									/*	<07>	SCEN	: 0			(サブクロック発振ビット 停止) */
									/*	<06>	PCEN	: 0			(PLL 発振ビット 停止) */
									/*	<05>	MCEN	: 1			(メインクロック発振 開始) */
									/*	<04-02>	-		: 000		(予約bit) */
									/*	<01-00>	CKS		: 00		(ソースクロック選択ビット メインクロックの2分周) */
									/* ------------------------------------------------------------------- */
#define STOP_MTMCR		(0x00)		/* INIT_MTMCRと同じ設定値 (未使用) */
#define STOP_STMCR		(0x00)		/* INIT_STMCRと同じ設定値 (未使用) */
#define STOP_PLLCR		(0x00B0)	/* INIT_PLLCRと同じ設定値 (未使用) */
#define STOP_CSTBR		(0x0C)		/* INIT_CSTBRと同じ設定値 */
#define STOP_PTMCR		(0x00)		/* INIT_PTMCRと同じ設定値 (未使用) */
#define STOP_CCPSSELR	(0x00)		/* INIT_CCPSSELRと同じ設定値 (未使用) */
#define STOP_CCPSDIVR	(0x11)		/* INIT_CCPSDIVRと同じ設定値 (未使用) */
#define STOP_CCPLLFBR	(0x4F)		/* INIT_CCPLLFBRと同じ設定値 (未使用) */
#define STOP_CCSSFBR0	(0x09)		/* INIT_CCSSFBR0と同じ設定値 (未使用) */
#define STOP_CCSSFBR1	(0x07)		/* INIT_CCSSFBR1と同じ設定値 (未使用) */
#define STOP_CCSSCCR0	(0x00)		/* INIT_CCSSCCR0と同じ設定値 (未使用) */
#define STOP_CCSSCCR1	(0x0000)	/* INIT_CCSSCCR1と同じ設定値 (未使用) */
#define STOP_CCCGRCR0	(0x00)		/* INIT_CCCGRCR0と同じ設定値 (未使用) */
#define STOP_CCCGRCR1	(0x00)		/* INIT_CCCGRCR1と同じ設定値 (未使用) */
#define STOP_CCCGRCR2	(0x00)		/* INIT_CCCGRCR2と同じ設定値 (未使用) */
#define STOP_CCRTSELR	(0x00)		/* INIT_CCRTSELRと同じ設定値 (未使用) */
#define STOP_CCPMUCR0	(0x00)		/* INIT_CCPMUCR0と同じ設定値 (未使用) */
#define STOP_CCPMUCR1	(0x00)		/* INIT_CCPMUCR1と同じ設定値 (未使用) */
#define STOP_SACR		(0x00)		/* INIT_SACRと同じ設定値 (未使用) */
#define STOP_PICD		(0x00)		/* INIT_PICDと同じ設定値 (未使用) */

#define INIT_PLLCR_POSW					(0xB)		/* PLL/SSCGクロック安定待ち時間を設定 1024μs*/
#define INIT_CCPSSELR_PCSEL				(0x0)		/* PLLを選択 */
#define INIT_PLLCR_PDS					(0x0)		/* PLLの逓倍率を設定(CAN、OCD用) 4MHz*/
#define INIT_CCPSDIVR_PODS				(0x1)		/* PLLの逓倍率を設定(CAN、OCD用) 80MHz*/
#define INIT_CCPLLFBR_IDIV				(0x4F)		/* PLLの逓倍率を設定(CAN、OCD用) 320MHz */
#define INIT_CCPSDIVR_SODS				(0x1)		/* SSCGの逓倍率を設定 80MHz */
#define INIT_CCSSFBR0_NDIV				(0x8)		/* SSCGの逓倍率を設定 288MHz */
#define INIT_CCSSFBR1_PDIV				(0x7)		/* SSCGの逓倍率を設定 288MHz */
#define INIT_CCSSCCR0_SFREQ				(0x0)		/* SSCGの拡散方法を設定 1/1024 */
#define INIT_CCSSCCR0_SMODE				(0x1)		/* SSCGの拡散方法を設定 CenterSpread */
#define INIT_CCSSCCR0_SSEN				(0x1)		/* SSCGの拡散方法を設定 拡散スペクトラム許可 */
#define INIT_CCSSCCR1_RATESEL			(0x6)		/* SSCGの拡散方法を設定 5% */
#define INIT_CCCGRCR0_GREN				(0x1)		/* ギアを有効状態に設定 */
#define INIT_CCCGRCR1_GRSTP				(0x0)		/* ギアステップの設定 ステップ数1 */
#define INIT_CCCGRCR1_GRSTN				(0x0)		/* ギアステップの設定 開始ステップ数0 */
#define INIT_CCCGRCR2_GRLP				(0xF)		/* ギアステップの設定 16回 */
#define INIT_CSELR_PCEN					(0x1)		/* PLL発振 */
#define INIT_DIVR0_DIVB					(0x0)		/* 分周なし */
#define INIT_DIVR1_DIVT					(0x1)		/* φ/2 2分周 (未使用) */
#define INIT_DIVR2_DIVP					(0x4)		/* φ/5 5分周 */
#define INIT_PICD_PDIV					(0x9)		/* PLLクロックの10分周(INIT_SACR_Mを非同期とした場合のみ有効) */
#define INIT_SACR_M						(0x0)		/* 同期(SSCGを使う場合のみ有効) */
#define INIT_CSELR_CKS_SELECT_PLL_SSCG	(0x2)		/* PLL/SSCGクロックを選択 */
#define INIT_CSELR_CKS_SELECT_MAIN_00	(0x0)		/* メインクロックを選択 */
#define INIT_CSELR_CKS_SELECT_MAIN_01	(0x1)		/* メインクロックを選択 (未使用) */
#define INIT_CSELR_CKS_SELECT_SUB		(0x3)		/* サブクロックを選択 (未使用) */
#define INIT_CCCGRCR0_GRSTR				(0x1)		/* ギア開始 */
#define STOP_CCCGRCR0_GRSTR				(0x1)		/* ギア開始 (未使用) */
#define STOP_CSELR_CKS					(0x0)		/* メインクロックを選択 */
#define STOP_CSELR_PCEN					(0x0)		/* PLL/SSCGの動作を停止（発振を停止) */
#define STOP_DIVR0_DIVB					(0x0)		/* 分周なし */
#define STOP_DIVR1_DIVT					(0x1)		/* φ/2 2分周 (未使用) */
#define STOP_DIVR2_DIVP					(0x4)		/* φ/5 5分周 */
#define STOP_SACR_M						(0x0)		/* 同期 */

#define DIVR0_INIT_MASK					(0xE0U)			/* 分周設定レジスタ0初期値マスク (未使用) */
#define DIVR2_INIT_MASK					(0xF0U)			/* 分周設定レジスタ2初期値マスク (未使用) */ 
#define CSELR_INIT_MASK					(0xE3U)			/* クロックソース設定レジスタ初期値マスク (未使用) */
#define PLLCR_INIT_MASK					(0x00FF)		/* PLL設定レジスタ初期値マスク (未使用) */
#define CCPSSELR_INIT_MASK				(0x01)			/* PLL/SSCGクロック選択レジスタ初期値マスク (未使用) */
#define CCPSDIVR_INIT_MASK				(0x77)			/* PLL/SSCG出力クロック分周設定レジスタ初期値マスク (未使用) */
#define CCPLLFBR_INIT_MASK				(0x7F)			/* PLLフィードバック分周設定レジスタ初期値マスク (未使用) */
#define CCSSFBR0_INIT_MASK				(0x3F)			/* SSCGフィードバック分周設定レジスタ0初期値マスク (未使用) */
#define CCSSFBR1_INIT_MASK				(0x1F)			/* SSCGフィードバック分周設定レジスタ1初期値マスク (未使用) */
#define CCSSCCR0_INIT_MASK			(0x0F)			/* SSCGコンフィグ設定レジスタ0初期値マスク (未使用) */
#define CCSSCCR1_INIT_MASK			(0xE000U)		/* SSCGコンフィグ設定レジスタ1初期値マスク (未使用) */
#define CCCGRCR0_INIT_MASK			(0x03)			/* クロックギアコンフィグ設定レジスタ0初期値マスク (未使用) */
#define CCCGRCR1_INIT_MASK			(0xFFU)			/* クロックギアコンフィグ設定レジスタ1初期値マスク (未使用) */
#define CCCGRCR2_INIT_MASK			(0xFFU)			/* クロックギアコンフィグ設定レジスタ2初期値マスク (未使用) */
#define SACR_INIT_MASK				(0x00)			/* 同期/非同期コントロールレジスタ初期値マスク (未使用) */
#define PICD_INIT_MASK				(0x0F)			/* 周辺インタフェースクロック分周初期値マスク (未使用) */
#define DIVR0_STOP_MASK				(0xE0U)			/* 分周設定レジスタ0停止値マスク (未使用) */
#define DIVR2_STOP_MASK				(0xF0U)			/* 分周設定レジスタ2停止値マスク (未使用) */
#define CSELR_STOP_MASK				(0xE3U)			/* クロックソース設定レジスタ停止値マスク (未使用) */
#define CCCGRCR0_STOP_MASK			(0x02)			/* クロックギアコンフィグ設定レジスタ0停止値マスク (未使用) */
#define SACR_STOP_MASK				(0x0)			/* 同期/非同期コントロールレジスタ停止値マスク (未使用) */
#define CSELR_CKS_MCLK_DIV2			(0x0)			/* クロックソース設定レジスタソースクロック選択：メインクロック(MCLK)の2分周 (未使用) */
#define CSELR_CKS_PLL_SSCG			(0x2)			/* クロックソース設定レジスタソースクロック選択：PLL/SSCGクロック (未使用) */
#define CMONR_CKM_MCLK_DIV2			(0x0)			/* クロックソース監視レジスタソースクロック表示：メインクロック(MCLK)の2分周 */
#define CMONR_CKM_PLL_SSCG			(0x2)			/* クロックソース監視レジスタソースクロック表示：PLL/SSCGクロック */
#define CMONR_PCRDY_START			(0x1)			/* クロックソース監視レジスタPLLクロックレディ：発振開始 (未使用) */
#define CCCGRCR0_GRSTS_STOP_LOW		(0x0)			/* クロックギアコンフィグ設定レジスタ0クロックギアステータスフラグ：低速で停止 */
#define CCCGRCR0_GRSTS_GEAR_UP		(0x1)			/* クロックギアコンフィグ設定レジスタ0クロックギアステータスフラグ：GEAR UP 動作中 */
#define CCCGRCR0_GRSTS_STOP_HIGH	(0x2)			/* クロックギアコンフィグ設定レジスタ0クロックギアステータスフラグ：高速で停止 */
#define CCCGRCR0_GRSTS_GEAR_DOWN	(0x3)			/* クロックギアコンフィグ設定レジスタ0クロックギアステータスフラグ：GEAR DOWN 動作中 */
#define D_MAINWAIT_RESET			(12288)			/* リセット起動時のメインクロック発振安定待ち時間( 2^15 × 4MHz = 8192us ) 1.5倍狙い */
#define D_MAINWAIT_WAKE_UP			(49152)			/* Wake Up起動時のメインクロック発振判定待ち時間( 2~17 × 4MHz = 32768μs ) 1.5倍狙い  */
#define D_PLLWAIT					(1536)			/* PLL発振安定待ち時間( 2^12 × 4MHz = 1024us) 1.5倍狙い */
#define D_PLL_MAINCLKWAIT			(90)			/* PLL⇒メインクロック切換え完了待ち時間(4800cycle / 80MHz = 60us) 1.5倍狙い */
#define D_MAINCLKWAIT				(3600)			/* メインクロック⇒メインクロック切換え完了待ち時間(4800cycle / (4MHz/2) = 2400us) 1.5倍狙い */
#define D_PLLCLKWAIT				(3600)			/* PLL/SSCGクロック切換え完了待ち時間(4800cycle / (4MHz/2) = 2400us) 1.5倍狙い */
#define D_GEARWAIT					(1211)			/* ギア完了待ち時間( 807us ) 1.5倍狙い */
#define D_RTC_PMU_CLKWAIT			(141)			/* RTC/PMUクロック切替り時間(メインクロック×約3サイクル + サブクロック×約3サイクル=93.75μs 1.5倍狙い */
#define D_PMU_DIV_WAIT				(4)				/* RTCクロック×約4サイクル+PCLK1×約4サイクル=1.025μs 1.5倍狙い */
#define D_SYSCLKWATCH				(1)				/* システムクロック監視時間[us](1us周期) */

#endif								/* __MB91570_SYSTEMCLOCK_DRV_INTERNAL__ */

/* DIVide clock configuration Register 1 */
#define SYSCLKDRV_DIVR1_DIVT_1			(0x00u)			/* DIVR1 bit[6-4]:外部バスクロック分周設定：分周なし*/
#define SYSCLKDRV_DIVR1_DIVT_2			(0x01u)			/* DIVR1 bit[6-4]:外部バスクロック分周設定：2分周	*/
#define SYSCLKDRV_DIVR1_DIVT_3			(0x02u)			/* DIVR1 bit[6-4]:外部バスクロック分周設定：3分周	*/
#define SYSCLKDRV_DIVR1_DIVT_4			(0x03u)			/* DIVR1 bit[6-4]:外部バスクロック分周設定：4分周	*/
#define SYSCLKDRV_DIVR1_DIVT_5			(0x04u)			/* DIVR1 bit[6-4]:外部バスクロック分周設定：5分周	*/
#define SYSCLKDRV_DIVR1_DIVT_6			(0x05u)			/* DIVR1 bit[6-4]:外部バスクロック分周設定：6分周	*/
#define SYSCLKDRV_DIVR1_DIVT_7			(0x06u)			/* DIVR1 bit[6-4]:外部バスクロック分周設定：7分周	*/
#define SYSCLKDRV_DIVR1_DIVT_8			(0x07u)			/* DIVR1 bit[6-4]:外部バスクロック分周設定：8分周	*/

#define SYSCLKDRV_DIVR1_DIVT_MIN		SYSCLKDRV_DIVR1_DIVT_1
#define SYSCLKDRV_DIVR1_DIVT_MAX		SYSCLKDRV_DIVR1_DIVT_8

/* 外部公開定数 */

/*==============================================================================*/
/*	define定義（外部公開）														*/
/*==============================================================================*/
#define D_SYSCLKDRV_WEIGHT_INIT		 (0x01u)		/*	クロックWeight係数（MCLK用）	*/
#define D_SYSCLKDRV_WEIGHT_PLL		 (0x02u)		/*	クロックWeight係数（PLLCLK用）	*/
#define D_SYSCLKDRV_WEIGHT_SSCG		 (0x01u)		/*	クロックWeight係数（SSCGCLK用	*/
#define D_SYSCLKDRV_WEIGHT_SBCLK	 (0x00u)		/*	クロックWeight係数（SBCLKは現在未使用。適用時に値を見直すこと	*/
#define D_SYSCLKDRV_ERROR	 		 (0x00u)		/*	範囲外		*/

/* 外部公開マクロ */
#define EXTBUS_TCLK_LOW		SYSCLKDRV_DIVR1_DIVT_8	/* 低速通信用分周設定：8分周		*/
#define EXTBUS_TCLK_HIGH	SYSCLKDRV_DIVR1_DIVT_2	/* 高速通信用分周設定：2分周		*/

/* システムクロックの逓倍率を考慮応じて使い分けること */
/* CCLK = 80MHz, 12.5 [ns/Cycle] x 2 [Cycle] = 25ns */
/* CCLK = 72MHz, 13.88..  [ns/Cycle] x 9 [Cycle] = 125ns */
#define D_NOP2()		(void)__wait_nop();(void)__wait_nop()
#define D_NOP3()		(void)__wait_nop();(void)__wait_nop();(void)__wait_nop()

#define D_NOP4()		D_NOP2();D_NOP2()
#define D_NOP5()		D_NOP2();D_NOP3()
#define D_NOP7()		D_NOP3();D_NOP4()
#define D_NOP8()		D_NOP4();D_NOP4()

#define D_NOP9()		D_NOP4();D_NOP5()
#define D_NOP10()		D_NOP5();D_NOP5()
#define D_NOP80()		D_NOP10();D_NOP10();D_NOP10();D_NOP10(); \
						D_NOP10();D_NOP10();D_NOP10();D_NOP10()

#endif	/*	__MB91570_SYSTEMCLOCK_DRV_CONFIG_H__ 	*/

