/********************************************************/
/* @file DTO459_VDV_main.c								*/
/* App(Flash)領域のmain制御								*/
/* 														*/
/* App(Flash)領域のmain制御をする。						*/
/* <ul>													*/
/* <li>AppのMain処理。									*/
/* <li>10msec周期タイマ割込み処理。						*/
/* </ul>												*/
/* 														*/
/********************************************************/
/* @author      Y.Sorimachi								*/
/* @date        2013/--/--								*/
/********************************************************/ 
#include "SSFTSTD_MacroFunc.h"
#include "system.h"
#include "DTO459_VDV_Firm_info.h"
#include "reg_setting.h"
#include "timer_ctrl.h"
#include "System_control_TASK.h"
#ifdef _VDV_APP
#include "DTO459_VDV_Advance_ctrl.h"
#include "DTO459_VDV_Bt_ctrl.h"
#include "DTO459_VDV_Can_ctrl.h"
#include "DTO459_VDV_Gnss_ctrl.h"
#include "DTO459_VDV_Sw_ctrl.h"
#include "DTO459_VDV_Inspection_ctrl.h"
#endif /* _VDV_APP */
#include "DTO459_VDV_Eeprom_ctrl.h"
#include "DTO459_VDV_Led_ctrl.h"
#include "DTO459_VDV_Sd_ctrl.h"
#include "DTO459_VDV_Fat.h"
#include "DTO459_VDV_File_io.h"
#include "DTO459_VDV_Common.h"
#include "DTO459_VDV_Flash_ctrl.h"
#include "DTO459_VDV_main.h"
#include "dbglog.h"
#include "time_measure.h"


/********************************************************/
/*	内部マクロ											*/
/********************************************************/


/********************************************************/
/*	定数宣言											*/
/********************************************************/


/********************************************************/
/*	構造体宣言											*/
/********************************************************/

/********************************************************/
/*	内部変数定義										*/
/********************************************************/

/********************************************************/
/*	内部関数プロトタイプ宣言							*/
/********************************************************/
static void S_module_init( void );
static void S_module_term( void );

/********************************************************/
/*	外部関数											*/
/********************************************************/


/** 
 * /// App(Flash)領域のmain処理
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	Appのメイン処理を行う。
 */ 
void App_Main( void )
{
	/**	<li> デバッグログ初期化	*/
	OUTLOG(OutputLogInit();)

	TM_MEAS(TmMeas_Init();)

	/**	<li> 割込み禁止	*/
	(void)D_DI();

	/**	<li> 各モジュールの初期化	*/
	S_module_init();

	/**	<li> 割込み許可	*/
	(void)D_EI();
	
	/**	<li> WDTリセット	*/
	Reg_setting_WtdRestart();


	/* 割込みマスク設定（全割込み許可） */
	Reg_setting_EnableIntMask();

	OUTLOG(OutputFirmVer();)	/* ファームウェアバージョンログ出力 */

	/**	<li> タイマ割込み開始（10msec）	*/
	Reg_setting_TimerStart();

	/**	</ol>	*/
	return;
}

/** 
 * タイマ割込み実行処理
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	10msecのタイマ割込み関数から実行される処理。
 * @note	(2016/08/18変更）5msecのタイマ割込み関数から実行される処理。
 */ 
void App_main_IntTimerRun( void )
{
	UI_32 timer;

	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_VDVMAIN);)

	/**	<li> WDTリセット	*/
	Reg_setting_WtdRestart();

	/**	<li>5ms毎のタイマーカウンタ値取得	*/
	timer = Timer_ctrl_GetTime5ms();
	if((timer % 2) == 0)
	{
		/* 5msに1回コールされるため10ms毎（2回に1回）処理実行する */
		
		/**	<li>10ms毎のタイマーカウンタ	*/
		Timer_ctrl_TimeCount10ms();

		/**	<li>共通定義	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_COMMON);)
		Common_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_COMMON);)

		/**	<li>システム状態制御	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_SYSCTRL);)
		System_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_SYSCTRL);)
		
#ifdef _VDV_APP
		/**	<li> 検査モード制御 */
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_INSPECT);)
		Insp_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_INSPECT);)
#endif	/* _VDV_APP */

#ifdef _VDV_APP
		/**	<li>SW制御	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_SW);)
		Sw_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_SW);)
#endif /* _VDV_APP */

		/**	<li>LED制御	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_LED);)
		Led_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_LED);)

#ifdef _VDV_APP
		/**	<li>CAN制御	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_CAN);)
		Can_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_CAN);)

		/**	<li>GNSS制御	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_GNSS);)
		Gnss_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_GNSS);)

		/**	<li>AdvanceControlUnit制御	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_ADVANCE);)
		Advance_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_ADVANCE);)

		/**	<li>BT制御	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_BT);)
		Bt_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_BT);)
#endif /* _VDV_APP */

		/**	<li>SD制御	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_SD);)
		Sd_control_Main(SD_PROCESS_MAIN);
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_SD);)

		/**	<li>EEPROM制御	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_EEPROM);)
		Eeprom_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_EEPROM);)

#ifdef _VDV_BOOT
		/**	<li>FLASH制御	*/
		Flash_control_Main();
#endif /* _VDV_BOOT */

		/**	<li>デバッグログ制御	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_LOG);)
		OUTLOG(OutputLogMain();)
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_LOG);)
	}
#ifdef _VDV_APP
	if((timer % 5) == 0)
	{
		/* 5msに1回コールされるため25ms毎（5回に1回）処理実行する */
		/**	<li>SD制御	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_SD_SUB);)
		Sd_control_Main(SD_PROCESS_SUB);
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_SD_SUB);)
	}
#endif /* _VDV_APP */
	/**	<ol>	*/

	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_VDVMAIN);)
	TM_MEAS(TmMeas_Main();)

	return;
}

/********************************************************/
/* 関数名称		： App_main_Term						*/
/* 機能の説明	： 終了処理								*/
/* 引数			： void									*/
/* 戻り値		： void									*/
/* 作成者		： (OFT)Toguchi							*/
/* 作成日		： 16/10/21								*/
/* 備考			： 										*/
/********************************************************/
void App_main_Term( void )
{
	/* 割込みマスク設定（全割込み禁止）	*/
	Reg_setting_DisableIntMask();

	/* WDTリセット	*/
	Reg_setting_WtdRestart();

	/* モジュール終了処理	*/
	S_module_term();

	return;
}

/********************************************************/
/*	内部関数											*/
/********************************************************/
/** 
 * /// モジュール初期化
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	各モジュールの初期化を行う。
 */ 
/* void S_module_init( void )											;@@ DEL REQ-002(BEX019) */
static void S_module_init( void )									/*	;@@ ADD REQ-002(BEX019) */
{
	/**	<ol>	*/
	/**	<li>レジスタ設定	*/
	Reg_setting_Init();

	/**	<li>共通定義	*/
	Common_Init();

	/**	<li>システム状態制御	*/
	System_control_Init();

	/** <li>ファイルシステム初期化	*/
	FileSystem_Init();

	/** <li>ファイルIO制御初期化	*/
	FileIO_Init();

#ifdef _VDV_APP
	/**	<li> 検査モード制御 */
	Insp_control_Init();
#endif	/* _VDV_APP */

#ifdef _VDV_APP
	/**	<li>SW制御	*/
	Sw_control_Init();
#endif /* _VDV_APP */

	/**	<li>LED制御	*/
	Led_control_Init();

#ifdef _VDV_APP
	/**	<li>CAN制御	*/
	Can_control_Init();

	/**	<li>GNSS制御	*/
	Gnss_control_Init();

	/**	<li>AdvanceControlUnit制御	*/
	Advance_control_Init();

	/**	<li>BT制御	*/
	Bt_control_Init();
#endif /* _VDV_APP */

	/**	<li>SD制御	*/
	Sd_control_Init();

	/**	<li>EEPROM制御	*/
	Eeprom_control_Init();

#ifdef _VDV_BOOT
	/**	<li>FLASH制御	*/
	Flash_control_Init();
#endif /* _VDV_BOOT */

	/**	</ol>	*/	
	return;
}

/********************************************************/
/* 関数名称		： S_module_term						*/
/* 機能の説明	： モジュール終了処理					*/
/* 引数			： void									*/
/* 戻り値		： void									*/
/* 作成者		： (OFT)Toguchi							*/
/* 作成日		： 16/10/21								*/
/* 備考			： 										*/
/********************************************************/
static void S_module_term( void )
{
	/* レジスタ終了	*/
	Reg_setting_Term();

	return;
}

