/********************************************************/
/* file reg_setting.h									*/
/* レジスタ設定用ヘッダファイル							*/
/* 														*/
/* レジスタの設定をする。								*/
/* <ul>													*/
/* <li>レジスタの初期化を行う							*/
/* <li>レジスタの再設定を行う							*/
/* <li>10msec周期のタイマ割込みを開始する				*/
/* <li>ウォッチドッグタイマをリセットする				*/
/* </ul>												*/
/* 														*/
/********************************************************/
/* author      Y.Sorimachi								*/
/* date        2013/--/--								*/
/********************************************************/
#include "reg_setting.h"
#include "SSFTSTD_Type.h"
#include "SSFTSTD_Macro.h"
#include "SHRVSTD_CPU_Drv.h"
#include "SHRVSTD_Interrupt_Drv.h"
#include "SHRVSTD_Wdt_Drv.h"
#include "SHRVSTD_Timer_Drv.h"
#include "SHRVSTD_Port_Drv.h"
#include "SHRVSTD_UART_Drv.h"
#include "SHRVSTD_ADC_Drv.h"
#include "SHRVSTD_PWM_Drv.h"

/********************************************************/
/*	内部マクロ											*/
/********************************************************/
#define D_1MTIMER_RELOADTIMER_CYCLE			(999U)		/* 1msec分のサイクル数-1 (1cycle=1us, 1msec:1000cycle-1=999(999開始->0終了のため-1する)) */
#define D_MAIN_TASK_RELOADTIMER_CYCLE		(4999U)		/* 5msec分のサイクル数-1 (1cycle=1us, 5msec:5000cycle-1=4999(4999開始->0終了のため-1する)) */

/********************************************************/
/*	内部関数プロトタイプ宣言							*/
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
/*	外部関数											*/
/********************************************************/

/** 
 * /// レジスタ初期化
 *
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		レジスタの初期化処理を行う。
 */ 
void Reg_setting_Init( void )
{	
	/* 初期化関数の引数はE_INIT_RESETでよい（電源OFF状態からの起動のみで省電力モードからの復帰などはないため） */

	/* システムクロックドライバレジスタ初期化 */
	SysClkDrv_Init(E_INIT_RESET);	

	/* リセットドライバレジスタ初期化 */
	RstDrv_Init(E_INIT_RESET);

	/* 割込機能ドライバレジスタ初期化 */
	IntrDrv_Init(E_INIT_RESET);

	/* WDTドライバ初期化 */
	WdtDrv_Init(E_INIT_RESET);

	/* リロードタイマ制御ﾚｼﾞｽﾀ初期化 */
	ReloadTmDrv_Init(E_INIT_RESET);

	/* GPIOドライバレジスタ初期化 */
	PortDrv_Init(E_INIT_RESET);

#ifdef _VDV_BOOT
	/* フラッシュドライバレジスタ初期化 */
	FlashDrv_Init(E_INIT_RESET);
	FlashCtrlDrv_Init(E_INIT_RESET);
#endif /* _VDV_BOOT */

#ifdef _VDV_APP
	/* UARTレジスタ初期化 */
	UartDrv_Init(E_INIT_RESET);

	/* A/Dドライバレジスタ初期化 */
	ADCDrv_Init(E_INIT_RESET);
#endif /* _VDV_APP */

	/* PWMドライバレジスタ初期 */
	PWMDrv_Init(E_INIT_RESET);

#ifdef _VDV_APP
#endif	/* _VDV_APP */

	return;
}

/********************************************************/
/* 関数名称		: Reg_setting_Term						*/
/* 機能の説明	: レジスタ終了設定						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 16/10/21								*/
/* 備考			: 										*/
/********************************************************/
void Reg_setting_Term( void )
{	
	/* PWMドライバ停止 */
	PWMDrv_Sleep();

#ifdef _VDV_APP
	/* A/Dドライバ停止 */
	ADCDrv_Sleep();

	/* UARTドライバ停止 */
	UartDrv_Sleep();
#endif /* _VDV_APP */

#ifdef _VDV_BOOT
	/* フラッシュドライバ停止 */
	FlashDrv_Sleep();
#endif /* _VDV_BOOT */

	/* GPIOドライバ停止 */
	PortDrv_Sleep();

	/* リロードタイマドライバ停止 */
	ReloadTmDrv_Sleep();

	/* WDTドライバ停止 */
	WdtDrv_Sleep();

	/* 割込機能ドライバ停止 */
	IntrDrv_Sleep();

	/* リセットドライバ停止 */
	RstDrv_Sleep();

	/* システムクロックドライバ停止 */
	SysClkDrv_Sleep();	

	return;
}

/** 
 * /// タイマ割込み開始
 *
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		10msec周期のタイマ割込みを開始する。
 */ 
void Reg_setting_TimerStart( void )
{
	ReloadTmDrv_Ctrl(RELOADTIMER_CH_1M_TIME_COUNT, D_1MTIMER_RELOADTIMER_CYCLE, D_ENABLE);		/* 経過時刻カウント用タイマ（1ms）起動 */
	ReloadTmDrv_Ctrl(RELOADTIMER_CH_MAIN_TASK, D_MAIN_TASK_RELOADTIMER_CYCLE, D_ENABLE);		/* メイン定期処理用タイマ（5ms）起動 */

	return;
}

/** 
 * /// ウォッチドッグタイマリセット
 *
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		ウォッチドッグタイマをリセットする。
 */ 
void Reg_setting_WtdRestart( void )
{
	/**	<ol>	*/
	/**	<li>HWウォッチドッグタイマをリセットする	*/
	WdtDrv_ReqClrCpu();

	return;
}

/** 
 * /// 割込みマスク許可設定
 */ 
void Reg_setting_EnableIntMask(void)
{
	(void)IntrDrv_SetMaskLevel(INTR_ENABLE_LEVEL);
}

/** 
 * /// 割込みマスク禁止設定
 */ 
void Reg_setting_DisableIntMask(void)
{
	(void)IntrDrv_SetMaskLevel(INTR_DISABLE_LEVEL);
}
