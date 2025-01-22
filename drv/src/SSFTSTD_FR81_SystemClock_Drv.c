/********************************************************************************/
/* 客先名		:	標準モジュール												*/
/* 機種名		:	SSFT														*/
/* ﾏｲｺﾝｿﾌﾄﾃｰﾏ名	:	PF															*/
/*==============================================================================*/
/* 作成ﾌｧｲﾙ名	:	SSFTSTD_FR81_SystemClock_Drv.c								*/
/* 				:	ｼｽﾃﾑｸﾛｯｸﾄﾞﾗｲﾊﾞﾓｼﾞｭｰﾙ										*/
/*==============================================================================*/
/* 対象ﾏｲｺﾝ		:	MB91570 Series												*/
/*==============================================================================*/
/* 作成ﾊﾞｰｼﾞｮﾝ	:	020102														*/
/* 作成年月日	:	2014.01.23													*/
/* 作成者		:	K.Uchiyama													*/
/*------------------------------------------------------------------------------*/
/* 変更履歴		:																*/
/* [010101]		:	新規作成													*/
/* [010102]		:	レジスタ初期設定からPLLクロック切り替えを分離				*/
/*				:	処理上、もっと早いタイミングで設定を行う必要があったため	*/
/* [010103]		:	標準IF対応。起動種別の名称変更								*/
/* [020101]		:	外バス分周(TCLK)設定用の外部IF追加							*/
/* [020102]		:	QAC対応。													*/
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

#define	 __MB91570_SYSTEMCLOCK_DRV_INTERNAL__
#include "SSFTSTD_FR81_SystemClock_Drv.h"

/*==============================================================================*/
/*	typedef定義（外部非公開）													*/
/*==============================================================================*/
/* なし */

/*==============================================================================*/
/*	関数ﾌﾟﾛﾄﾀｲﾌﾟ宣言（外部非公開)												*/
/*==============================================================================*/
static void sys_clk_drv_main_to_pll(E_INIT_TYPE req);							/* メイン→PLL クロック切換え処理 */
static void sys_clk_drv_pll_to_main(void);										/* PLL→メイン クロック切換え処理 */
static void sys_clk_drv_main_to_sub(void);										/* メイン→サブ クロック切換え処理 */
static void sys_clk_drv_sub_to_main(void);										/* サブ→メイン クロック切換え処理 */
static void sys_clk_drv_rtc_pmu_set(void);										/* RTC/PMUクロック設定処理 */

/*==============================================================================*/
/*	内部定数																	*/
/*==============================================================================*/
#define		MASK_CSVCR_MM			(0x40U)										/* CSVCR.MM 選択マスク */

/****************************************************************************************/
/*	[モジュール名]	SysClkDrv_Init		[名称]	ｼｽﾃﾑｸﾛｯｸﾄﾞﾗｲﾊﾞﾚｼﾞｽﾀ初期化設定処理		*/
/*======================================================================================*/
/*	[処理概要]	レジスタ初期設定時に本関数を呼び、初期化を行う。						*/
/*======================================================================================*/
/*	[記述形式]	void SysClkDrv_Init( E_INIT_TYPE req  )									*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_INIT_TYPE				req		：起動種別								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void SysClkDrv_Init( E_INIT_TYPE req )
{
	
	switch ( req ) {
	case E_INIT_RESET:															/* CPUリセットによる起動 */
	case E_INIT_WAKEUP:															/* 省電力解除（WAKEUP）による起動 */
		sys_clk_drv_main_to_pll(req);											/* ﾒｲﾝ→PLL ｸﾛｯｸ切換え処理 */
		break;
	case E_INIT_IGN_ON:															/* IGN ON */
		break;
	case E_INIT_RET_NORMAL_VOL:													/* 低電圧復帰 */
		break;
	case E_INIT_INTERVAL_WAKEUP:												/* 間欠起動ウェイクアップ */
		break;
	default:																	/* 上記以外 */
		break;
	}
	
}

/****************************************************************************************/
/*	[モジュール名]	SysClkDrv_Sleep	[名称]	ｼｽﾃﾑｸﾛｯｸﾄﾞﾗｲﾊﾞﾚｼﾞｽﾀ停止設定処理				*/
/*======================================================================================*/
/*	[処理概要]	省電力モード遷移時に本関数を呼び、初期化を行う。						*/
/*======================================================================================*/
/*	[記述形式]	void SysClkDrv_Sleep( void )											*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void SysClkDrv_Sleep( void )
{
	sys_clk_drv_pll_to_main();													/* PLL→ﾒｲﾝ ｸﾛｯｸ切換え処理 */
}


/****************************************************************************************/
/*	[モジュール名]	SysClkDrv_Refresh	[名称]	ｼｽﾃﾑｸﾛｯｸﾄﾞﾗｲﾊﾞﾚｼﾞｽﾀ再設定設定処理		*/
/*======================================================================================*/
/*	[処理概要]	レジスタ再設定時に本関数を呼び、初期化を行う。							*/
/*======================================================================================*/
/*	[記述形式]	void SysClkDrv_Refresh( void )											*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void SysClkDrv_Refresh( void )
{
	
	/* 空関数 */
	
}


/****************************************************************************************/
/*	[モジュール名]	sys_clk_drv_main_to_pll	[名称] ﾒｲﾝ→PLL ｸﾛｯｸ切換え処理				*/
/*======================================================================================*/
/*	[処理概要]	ソースクロックをメインクロック→PLL/SSCGクロックに切り換える。			*/
/*======================================================================================*/
/*	[記述形式]	static void sys_clk_drv_main_to_pll(E_INIT_TYPE req)					*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static void sys_clk_drv_main_to_pll(E_INIT_TYPE req)
{
	UI_16				main_clk_osc_wait;										/* メインクロック発振安定待ち時間 */
	UI_16				main_clk_osc_tm;										/* メインクロック発振安定待ちカウンタ */
	UI_16				main_tmp;												/* メインクロック切替え待ち時間カウンタ */
	UI_16				pll_tmp;												/* PLL発振安定待ちカウンタ */
	UI_16				clk_tmp;												/* PLL切替え待ち時間カウンタ */
	UI_16				gear_tmp;												/* ギア完了待ち時間カウンタ */
	UI_8				flg_exit;												/* レジスタ値変化待ちフラグ */
	UI_8				csvcr;													/* クロックスーパバイザ制御レジスタ */
	SI_8				main_clk_osc_stat;										/* メインクロック発振状態 */
	
	if (req == E_INIT_WAKEUP) {
		main_clk_osc_wait = D_MAINWAIT_WAKE_UP;									/* Weak Up起動時は、電源安定待ち設定レジスタ値に依存 */
	} else {
		main_clk_osc_wait = D_MAINWAIT_RESET;									/* リセット起動時は、2~15×4MHzの待ち時間 */
	}
	
	main_clk_osc_tm = 0;
	main_tmp = 0;
	pll_tmp = 0;
	clk_tmp = 0;
	flg_exit = D_OFF;
	main_clk_osc_stat = D_OK;

	while (flg_exit == D_OFF) {													/* 終了フラグがOFFの間はループ */
		if (CLKG_CMONR_MCRDY == D_ON) {											/* メインクロックが発振安定した？*/
			flg_exit = D_ON;													/* ループ終了 */
			main_clk_osc_stat = D_OK;											/* メインクロック発振状態:OK */
		} else if(main_clk_osc_tm >= main_clk_osc_wait) {						/* 発振安定待ち時間、経過？ */
			flg_exit = D_ON;													/* ループ終了 */
			main_clk_osc_stat = D_NG;											/* メインクロック発振状態:NG */
		} else {
			;
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* 待機処理 */
		main_clk_osc_tm++;
	}
	
	CLKG_CSELR_CKS = INIT_CSELR_CKS_SELECT_MAIN_00;								/* メインクロック(00b)に設定 */
	
	flg_exit = D_OFF;
	
	while (flg_exit == D_OFF) {													/* 終了フラグがOFFの間はループ */
		if ( (CLKG_CMONR_CKM == INIT_CSELR_CKS_SELECT_MAIN_00)					/* ソースクロックがメインクロックに切り替わった？ */
		|| (main_tmp >= D_MAINCLKWAIT) ) {										/* または、ソースクロックの切り替り時間待機した? */
			flg_exit = D_ON;													/* ループ終了 */
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* 待機処理 */
		main_tmp++;
	}
	CLKG_CSTBR = INIT_CSTBR;													/* 発振安定待ち設定レジスタ */
	CLKG_PLLCR_POSW = INIT_PLLCR_POSW;											/* PLL/SSCGクロック安定待ち時間を設定 */

	CCTL_CCPSSELR_PCSEL = INIT_CCPSSELR_PCSEL;									/* PLL/SSCGクロックソース選択 */
	
	/* PLLの逓倍率を設定(CAN、OCD用) */
	CLKG_PLLCR_PDS = INIT_PLLCR_PDS;											/* PLL入力クロック分周選択 */
	CCTL_CCPLLFBR_IDIV = INIT_CCPLLFBR_IDIV;									/* PLLマクロFB入力分周比設定 */
	CCTL_CCPSDIVR_PODS = INIT_CCPSDIVR_PODS;									/* PLLマクロ発振クロック分周比選択 */

	/* SSCGの逓倍率を設定 */
	CCTL_CCSSFBR0_NDIV = INIT_CCSSFBR0_NDIV;									/* SSCGマクロFB入力N分周比設定 */
	CCTL_CCSSFBR1_PDIV = INIT_CCSSFBR1_PDIV;									/* SSCGマクロFB入力P分周比設定 */
	CCTL_CCPSDIVR_SODS = INIT_CCPSDIVR_SODS;									/* SSCGマクロ発振クロック分周比選択 */
	
	/* SSCGの拡散方法を設定 */
	CCTL_CCSSCCR0_SFREQ = INIT_CCSSCCR0_SFREQ;									/* モジュレーション周波数 */
	CCTL_CCSSCCR0_SMODE = INIT_CCSSCCR0_SMODE;									/* 拡散モジュレーションモード選択 */
	CCTL_CCSSCCR0_SSEN = INIT_CCSSCCR0_SSEN;									/* 拡散スペクトル許可 */
	CCTL_CCSSCCR1_RATESEL = INIT_CCSSCCR1_RATESEL;								/* 拡散モジュレーションレート選択 */
	
	/* ギアを有効状態に設定 */
	CCTL_CCCGRCR0_GREN = INIT_CCCGRCR0_GREN;									/* クロックギアイネーブル */
	
	/* ギアステップの設定 */
	CCTL_CCCGRCR1_GRSTP = INIT_CCCGRCR1_GRSTP;									/* クロックギアステップ選択 */
	CCTL_CCCGRCR1_GRSTN = INIT_CCCGRCR1_GRSTN;									/* クロックギア開始ステップ選択 */
	CCTL_CCCGRCR2_GRLP = INIT_CCCGRCR2_GRLP;									/* クロックギア繰返し回数 */
	
	csvcr = CSVCR;																/* クロックスーパバイザ制御レジスタ値 */
	
	if ( (main_clk_osc_stat == D_OK) &&											/* メインクロックの発振安定を確認できた？ */
	     ((csvcr & MASK_CSVCR_MM) == 0) ) {										/* かつ、クロックスーパバイザのMM bitがメインクロック動作中？ */
		CLKG_CSELR_PCEN = INIT_CSELR_PCEN;										/* PLL発振を許可 */	
	
		flg_exit = D_OFF;
	
		while (flg_exit == D_OFF) {												/* 終了フラグがOFFの間はループ */
			if ( (CLKG_CMONR_PCRDY == D_ON) 									/* PLL発振が安定した？ */
			  || (pll_tmp >= D_PLLWAIT) ) {										/* または発振安定待ち時間待機した？ */
				flg_exit = D_ON;												/* ループ終了 */
			}
			SysClkDrv_UsecWait(D_SYSCLKWATCH);									/* 待機処理 */
			pll_tmp++;
		}
	
		/* 各種クロック(CPU/周辺)の分周を設定 */
		DIVR0_DIVB = INIT_DIVR0_DIVB;											/* ソースクロックを分周→ベースクロック、CPUクロック(CCLK)、オンチップバスクロック(HCLK) */
		DIVR1      = INIT_DIVR1;												/* ベースクロックを分周→外部バスクロック(TCLK) */
		DIVR2_DIVP = INIT_DIVR2_DIVP;											/* ベースクロックを分周→周辺クロック(PCLK1) */
	
		/* 非同期周辺クロックの分周を設定 */
		PICD_PDIV = INIT_PICD_PDIV;												/* PLLクロックを分周→周辺クロック(PCLK2) */
		/* CPU/周辺クロックの関係を非同期に設定 */
		SACR_M = INIT_SACR_M;													/* 同期/非同期設定 */
	
		/* PLL/SSCGクロックへの切替え */
		CLKG_CSELR_CKS = INIT_CSELR_CKS_SELECT_PLL_SSCG;						/* ソースクロック選択 */
	
		flg_exit = D_OFF;
	
		while (flg_exit == D_OFF) {												/* 終了フラグがOFFの間はループ */
			if ( (CLKG_CMONR_CKM == INIT_CSELR_CKS_SELECT_PLL_SSCG) 			/* ソースクロックが切り替った？ */
			  || (clk_tmp >= D_PLLCLKWAIT) ) {									/* または切り替り時間待機した？ */
				flg_exit = D_ON;												/* ループ終了 */
			}
			SysClkDrv_UsecWait(D_SYSCLKWATCH);									/* 待機処理 */
			clk_tmp++;
		}
	
		/* GEAR UP 動作中 */
		if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_GEAR_UP ) {
			/* ギア完了を確認 */
			flg_exit = D_OFF;
			gear_tmp = 0;
			while (flg_exit == D_OFF) {											/* 終了フラグがOFFの間はループ */
				if ( (CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_HIGH)			/* クロックギアが高速停止中 */
				  || (gear_tmp >= D_GEARWAIT) ) {								/* または、ギア完了時間完了 */
					flg_exit = D_ON;											/* ループ終了 */
				}
				SysClkDrv_UsecWait(D_SYSCLKWATCH);								/* 待機処理 */
				gear_tmp++;
			}
		}
		/* クロックが高速で停止 */
		if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_HIGH ) {
			/* ギア開始 */
			CCTL_CCCGRCR0_GRSTR = INIT_CCCGRCR0_GRSTR;							/* クロックギアスタート */
		}
	
		/* GEAR DOWN 動作中 */
		if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_GEAR_DOWN ) {
			/* ギア完了を確認 */
			flg_exit = D_OFF;
			gear_tmp = 0;
			while (flg_exit == D_OFF) {											/* 終了フラグがOFFの間はループ */
				if ( (CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_LOW)			/* クロックギアが低速停止中 */
				  || (gear_tmp >= D_GEARWAIT) ) {								/* または、ギア完了時間完了 */
					flg_exit = D_ON;											/* ループ終了 */
				}
				SysClkDrv_UsecWait(D_SYSCLKWATCH);								/* 待機処理 */
				gear_tmp++;
			}
		}
		/* クロックが低速で停止 */
		if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_LOW ) {
			/* ギア開始 */
			CCTL_CCCGRCR0_GRSTR = INIT_CCCGRCR0_GRSTR;							/* クロックギアスタート */
		}
	
		/* ギア完了を確認 */
		flg_exit = D_OFF;
		gear_tmp = 0;
		while (flg_exit == D_OFF) {												/* 終了フラグがOFFの間はループ */
			if ( (CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_HIGH)				/* クロックギアが高速停止中 */
			  || (gear_tmp >= D_GEARWAIT) ) {									/* または、ギア完了時間完了 */
				flg_exit = D_ON;												/* ループ終了 */
			}
			SysClkDrv_UsecWait(D_SYSCLKWATCH);									/* 待機処理 */
			gear_tmp++;
		}

		sys_clk_drv_rtc_pmu_set();												/* RTC/PMUクロック設定処理 */
	}
}


/****************************************************************************************/
/*	[モジュール名]	sys_clk_drv_pll_to_main	[名称] PLL→ﾒｲﾝ ｸﾛｯｸ切換え処理				*/
/*======================================================================================*/
/*	[処理概要]	ソースクロックをPLL/SSCGクロック→メインクロックに切り換える。			*/
/*======================================================================================*/
/*	[記述形式]	static void sys_clk_drv_pll_to_main(void)								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static void sys_clk_drv_pll_to_main(void)
{
	UI_16				clk_tmp;												/* クロック切換え待ち時間カウンタ */
	UI_16				gear_tmp;												/* ギア完了待ち時間カウンタ */
	UI_8				flg_exit;												/* レジスタ値変化待ちフラグ */
	
	clk_tmp = 0;
	flg_exit = D_OFF;
	
	CLKG_CSTBR = STOP_CSTBR;													/* 発振安定待ち設定レジスタ */
	
	/* PLL/SSCGクロック・モードを確認 */
	while (flg_exit == D_OFF) {
		if ( (CLKG_CMONR_CKM == CMONR_CKM_PLL_SSCG)
		  || (clk_tmp >= D_PLLCLKWAIT) ) {
			flg_exit = D_ON;
		}
		clk_tmp++;
		SysClkDrv_UsecWait(D_SYSCLKWATCH);
	}
	
	/* GEAR DOWN 動作中 */
	if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_GEAR_DOWN ) {
		/* ギア完了を確認 */
		flg_exit = D_OFF;
		gear_tmp = 0;
		while (flg_exit == D_OFF) {												/* 終了フラグがOFFの間はループ */
			if ( (CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_LOW)				/* クロックギアが低速停止中 */
			  || (gear_tmp >= D_GEARWAIT) ) {									/* または、ギア完了時間完了 */
				flg_exit = D_ON;												/* ループ終了 */
			}
			SysClkDrv_UsecWait(D_SYSCLKWATCH);									/* 待機処理 */
			gear_tmp++;
		}
	}
	/* クロックが低速で停止 */
	if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_LOW ) {
		/* ギア開始 */
		CCTL_CCCGRCR0_GRSTR = INIT_CCCGRCR0_GRSTR;								/* クロックギアスタート */
	}
	/* GEAR UP 動作中 */
	if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_GEAR_UP ) {
		/* ギア完了を確認 */
		flg_exit = D_OFF;
		gear_tmp = 0;
		while (flg_exit == D_OFF) {												/* 終了フラグがOFFの間はループ */
			if ( (CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_HIGH)				/* クロックギアが高速停止中 */
			  || (gear_tmp >= D_GEARWAIT) ) {									/* または、ギア完了時間完了 */
				flg_exit = D_ON;												/* ループ終了 */
			}
			SysClkDrv_UsecWait(D_SYSCLKWATCH);									/* 待機処理 */
			gear_tmp++;
		}
	}
	/* クロックが高速で停止 */
	if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_HIGH ) {
		/* ギア開始 */
		CCTL_CCCGRCR0_GRSTR = INIT_CCCGRCR0_GRSTR;								/* クロックギアスタート */
	}
	
	/* ギア完了を確認 */
	flg_exit = D_OFF;
	gear_tmp = 0;
	while (flg_exit == D_OFF) {													/* 終了フラグがOFFの間はループ */
		if ( (CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_LOW)					/* クロックギアが低速停止中 */
		  || (gear_tmp >= D_GEARWAIT) ) {										/* または、ギア完了時間完了 */
			flg_exit = D_ON;													/* ループ終了 */
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* 待機処理 */
		gear_tmp++;
	}

	/* メインクロックへの切替え */
	CLKG_CSELR_CKS = STOP_CSELR_CKS;											/* ソースクロック選択 */
	
	clk_tmp = 0;
	flg_exit = D_OFF;
	/* メインクロックに切替わっていることを確認 */
	while (flg_exit == D_OFF) {
		if ( (CLKG_CMONR_CKM == CMONR_CKM_MCLK_DIV2)
		  || (clk_tmp >= D_PLL_MAINCLKWAIT) ) {
			flg_exit = D_ON;
		}
		clk_tmp++;
		SysClkDrv_UsecWait(D_SYSCLKWATCH);
	}
	
	/* PLL/SSCGの動作を停止 */
	CLKG_CSELR_PCEN = STOP_CSELR_PCEN;											/* PLL発振 */
	/* 各種クロック(CPU/周辺)の分周を設定 */
	DIVR0_DIVB = STOP_DIVR0_DIVB;												/* ソースクロックを分周→ベースクロック、CPUクロック(CCLK)、オンチップバスクロック(HCLK) */
	DIVR1      = STOP_DIVR1;													/* ベースクロックを分周→外部バスクロック(TCLK) */
	DIVR2_DIVP = STOP_DIVR2_DIVP;												/* ベースクロックを分周→周辺クロック(PCLK1) */
	/* CPU/周辺クロックの関係を非同期に設定 */
	SACR_M = STOP_SACR_M;														/* 同期/非同期設定 */
}

/****************************************************************************************/
/*	[モジュール名]	sys_clk_drv_main_to_sub	[名称] ﾒｲﾝ→ｻﾌﾞ ｸﾛｯｸ切換え処理				*/
/*======================================================================================*/
/*	[処理概要]	ソースクロックをメインクロック→サブクロックに切り換える。				*/
/*======================================================================================*/
/*	[記述形式]	static void sys_clk_drv_main_to_sub(void)								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static void sys_clk_drv_main_to_sub(void)
{
	
	/* 空関数 */
	
}

/****************************************************************************************/
/*	[モジュール名]	sys_clk_drv_sub_to_main	[名称] ｻﾌﾞ→ﾒｲﾝ ｸﾛｯｸ切換え処理				*/
/*======================================================================================*/
/*	[処理概要]	ソースクロックをサブクロック→メインクロックに切り換える。				*/
/*======================================================================================*/
/*	[記述形式]	static void sys_clk_drv_sub_to_main(void)								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static void sys_clk_drv_sub_to_main(void)
{
	
	/* 空関数 */
	
}

/****************************************************************************************/
/*	[モジュール名]	sys_clk_drv_rtc_pmu_set	[名称] RTC/PMUクロック設定処理				*/
/*======================================================================================*/
/*	[処理概要]	RTC/PMUクロックのソースクロックと分周比を設定する。						*/
/*======================================================================================*/
/*	[記述形式]	static void sys_clk_drv_rtc_pmu_set(void)								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static void sys_clk_drv_rtc_pmu_set(void)
{
	UI_16				src_clk_change_time;
	UI_16				f_div_refrect_time;
	UI_16				g_div_refrect_time;
	UI_8				flg_exit;
	
	src_clk_change_time = 0;
	f_div_refrect_time = 0;
	g_div_refrect_time = 0;
	flg_exit = D_OFF;
	
	/* RTC/PMUクロック選択レジスタ CST=0でないと、書換えできないので、監視 */
	while(flg_exit == D_OFF) {
		if ( (CCTL_CCRTSELR_CST == 0)
		  || (src_clk_change_time >= D_RTC_PMU_CLKWAIT) ) {
			flg_exit = D_ON;
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* 待機処理 */
		src_clk_change_time++;
	}
	
	CCTL_CCRTSELR_CSC = INIT_CCRTSELR_CSC;
	
	/* ソースクロックの切替り待ち */
	src_clk_change_time = 0;
	flg_exit = D_OFF;
	while(flg_exit == D_OFF) {
		if ( (CCTL_CCRTSELR_CST == 0)
		  || (src_clk_change_time >= D_RTC_PMU_CLKWAIT) ) {
			flg_exit = D_ON;
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* 待機処理 */
		src_clk_change_time++;
	}
	
	flg_exit = D_OFF;
	while(flg_exit == D_OFF) {
		if ( (CCTL_CCPMUCR0_FST == 0)
		  || (f_div_refrect_time >= D_PMU_DIV_WAIT) ) {
			flg_exit = D_ON;
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* 待機処理 */
		f_div_refrect_time++;
	}
	
	CCTL_CCPMUCR0_FDIV = INIT_CCPMUCR0_FDIV;
	
	f_div_refrect_time = 0;
	flg_exit = D_OFF;
	while(flg_exit == D_OFF) {
		if ( (CCTL_CCPMUCR0_FST == 0)
		  || (f_div_refrect_time >= D_PMU_DIV_WAIT) ) {
			flg_exit = D_ON;
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* 待機処理 */
		f_div_refrect_time++;
	}
	
	flg_exit = D_OFF;
	while(flg_exit == D_OFF) {
		if ( (CCTL_CCPMUCR1_GST == 0)
		  || (g_div_refrect_time >= D_PMU_DIV_WAIT) ) {
			flg_exit = D_ON;
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* 待機処理 */
		g_div_refrect_time++;
	}
	
	CCTL_CCPMUCR1_GDIV = INIT_CCPMUCR1_GDIV;
	
	g_div_refrect_time = 0;
	flg_exit = D_OFF;
	while(flg_exit == D_OFF) {
		if ( (CCTL_CCPMUCR1_GST == 0)
		  || (g_div_refrect_time >= D_PMU_DIV_WAIT) ) {
			flg_exit = D_ON;
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* 待機処理 */
		g_div_refrect_time++;
	}
}

/****************************************************************************************/
/*	[モジュール名]	SysClkDrv_UsecWait	[名称] ウェイト処理								*/
/*======================================================================================*/
/*	[処理概要]	指定された設定値に対応した時間分ウェイトを行なう。						*/
/*======================================================================================*/
/*	[記述形式]	void SysClkDrv_UsecWait(void)											*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	wait_time:	wait時間(引数=1のとき 80MHzで1[us])							*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	引数とclock_weight設定値についてはモジュール仕様書参照のこと			*/
/****************************************************************************************/
void SysClkDrv_UsecWait(UI_16 wait_time)
{
	volatile UI_16	wk1;			/* ループ1用										*/
	volatile UI_16	wk2;			/* ループ2用					    				*/
	volatile UI_16	clock_weight;	/* システムクロック別用に重み付け 					*/

	switch( CLKG_CMONR_CKM ) {		/* Clock Monitor Register (ソースクロック表示）		*/
		case E_SYSCLK_INIT:			/* Initial Value（メインクロックの2分周） 		 	*/
		case E_SYSCLK_MCLK_DV2:		/* MCLK devided by 2（メインクロックの2分周）		*/
			clock_weight = D_SYSCLKDRV_WEIGHT_INIT;				/* 4MHz/2 相当の重み付け*/
			break;

		case E_SYSCLKDRV_PS_SEL:	/* PLL/SSCG CLK.（PLL/SSCGクロック）				*/
			if ( CCTL_CCPSSELR_PCSEL == D_ON ) {	/* PLL/SSCGクロック SSCG有：1		*/
				clock_weight = D_SYSCLKDRV_WEIGHT_SSCG;			/*  SSCG相当の重み付け 	*/
			} else {
				clock_weight = D_SYSCLKDRV_WEIGHT_PLL;			/*  PLL相当の重み付け	*/
			}
			break;

		case E_SYSCLKDRV_SBCLK:		/* SBCLK.（サブクロック）							*/
			clock_weight = D_SYSCLKDRV_WEIGHT_SBCLK;			/*  SBCLK相当の重み付け */
			break;

		default:					/* 範囲外 											*/
			clock_weight = D_SYSCLKDRV_ERROR;					/*  範囲外相当の重み付け*/
			break;
	}

	for ( wk1 = 1 ; wk1 < wait_time ; wk1++ ) { 
		for ( wk2 = 1 ; wk2 < clock_weight; wk2++ ) {
			(void) __wait_nop();						/* NOP		 1[cycle] 			*/
		}
	}
	return;
}

/****************************************************************************************/
/*	[モジュール名]	SysClkDrv_SetTCLK	[名称] Ext-Bus Clock (TCLK)分周設定処理			*/
/*======================================================================================*/
/*	[処理概要]	指定された設定値に対応したTCLK分周設定を行なう。						*/
/*======================================================================================*/
/*	[記述形式]	void SysClkDrv_SetTCLK(UI_8 select_clk)									*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	select_clk:	外部バスクロック分周値										*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	本APIは外部バス通信停止後(CS無効化後)に使用してください。				*/
/****************************************************************************************/
void SysClkDrv_SetTCLK(UI_8 select_clk)
{
	if (select_clk <= SYSCLKDRV_DIVR1_DIVT_MAX) {
		DIVR1_DIVT = select_clk;
	}
}
