/********************************************************/
/* @file System_control_TASK.h							*/
/* システム状態制御										*/
/* 														*/
/* システム動作状態の制御を行う。						*/
/* <ul>													*/
/* <li>通知されたイベントによって、システム動作状態（モード）を決定する。 */
/* <li>システム動作状態を取得する。						*/
/* </ul>												*/
/* 														*/
/********************************************************/
/* @author      Y.Sorimachi								*/
/* @date        2013/--/--								*/
/********************************************************/
/* @page */
#ifndef	SYSTEM_CONTROL_TASK_H
#define	SYSTEM_CONTROL_TASK_H

#include "type.h"

/*--------------------------------------------------------------------------*/
/*	機能切り替え定義（デバッグ用ソースでのみ有効）							*/
/*--------------------------------------------------------------------------*/
#define GNSS_ENABLE					/* 有効にしたらGNSS有効 */
#define BT_ENABLE					/* 有効にしたらBT有効 */
#define CAN_ENABLE					/* 有効にしたらCAN有効 */
//#define CAN_ERR_DETECT_ENABLE		/* 有効にしたらCANエラー検出が有効 */
//#define GNSS_ADR_ON				/* 有効にしたらADR ON 無効にしたらADR OFF(ADF OFFの場合、HNR-PVTまたはNAV-HNRは取得しない)*/
#define GNSS_SIGNAL_OUTPUT_NAVPVT	/* 有効にしたらADR ON/OFFに関係なくGNSS信号のNAV-PVTが出力 
									   無効にしたら定義GNSS_HNR_PVT_ENABLEに応じて（HNR-PVTまたはNAV-PVT）を出力
									   【注意】
									    本定義を無効にする場合、GNSS_ADR_ONを有効にしないと信号が取得できない
									    また、GNSS_ADR_ONの場合は更新周期が2Hzになる*/
#define GNSS_HNR_PVT_ENABLE			/* 有効にしたらHNR-PVTを取得 無効にしたらNAV-HNRを取得 */
/*SMK Bluetooth*/
#define GNSS_VER_410				/* GNSSファームウェアバージョン4.10の基盤のときに有効にする*/


/*--------------------------------------------------------------------------*/
/*	外部マクロ																*/
/*--------------------------------------------------------------------------*/
/* 
 ** システム動作状態
 */ 
typedef enum e_sys_ctrl_status
{
	SYS_OPERATION_WAIT_ACC_ON = 0,		/**< ACC ON待ち				*/
	SYS_OPERATION_ACC_ON_UNKNOWN_STAT,	/**< ACC ON（状態判定）		*/
	SYS_OPERATION_NORMAL_OFF,			/**< 通常:OFFモード			*/
	SYS_OPERATION_NORMAL_AUTO,			/**< 通常:オートREC			*/
	SYS_OPERATION_NORMAL_MANUAL,		/**< 通常:マニュアルREC		*/
	SYS_OPERATION_ERROR,				/**< 異常					*/
	SYS_OPERATION_SIMULATION,			/**< シミュレーション		*/
	SYS_OPERATION_EEPROM_WRITE,			/**< EEPROM書換中			*/
	SYS_OPERATION_EEPROM_WRITE_COMP,	/**< EEPROM書換完了			*/
	SYS_OPERATION_FIRMWARE_WRITE,		/**< ファームウェア書換中	*/
	SYS_OPERATION_FIRMWARE_WRITE_COMP,	/**< ファームウェア書換完了	*/
	SYS_OPERATION_INSPECTION,			/**< 検査モード				*/
	SYS_OPERATION_INSPECTION_COMP,		/**< 検査完了				*/
	SYS_OPERATION_BT_COMM,				/**< BTモジュール通信モード	*/
	SYS_OPERATION_GNSS_COMM,			/**< GNSSモジュール通信モード*/
	SYS_OPERATION_PRE_ACC_OFF,			/**< ACC OFF移行			*/
	SYS_OPERATION_ACC_OFF,				/**< ACC OFF				*/
	SYS_OPERATION_NUM					/**< システム動作状態数		*/
} E_SYS_CTRL_STATUS;

/* 
 ** イベント
 */ 
typedef enum e_sys_ctrl_event
{
	SYS_EVENT_ACC_ON		= 0,		/**< ACC ON														*/
	SYS_EVENT_ACC_OFF,					/**< ACC OFF													*/
	
	SYS_EVENT_MODULE_STAT,				/**< モジュール状態(BT)											*/
	SYS_EVENT_CHANGE_SW_MODE,			/**< SWモード切替												*/
	SYS_EVENT_DECISION_ACTION_MODE,		/**< 動作モード決定												*/
	
	SYS_EVENT_END_PROC_COMP_BT,			/**< 終了処理完了(BT通信制御)									*/
	SYS_EVENT_END_PROC_COMP_SD,			/**< 終了処理完了(SD制御)										*/
	SYS_EVENT_END_PROC_COMP_EEPROM,		/**< 終了処理完了(EEPROM制御)									*/
	SYS_EVENT_END_PROC_COMP_FLASH,		/**< 終了処理完了(FLASH書換M制御)								*/
	
	SYS_EVENT_END_EEPROM_WRITE,			/**< EEPROM書換完了												*/
	SYS_EVENT_END_FLASH_WRITE,			/**< FLASH書換完了												*/
	SYS_EVENT_END_INSPECTION,			/**< 検査完了													*/

	SYS_EVENT_MODULE_STAT_CAN,			/**< モジュール状態(CAN)										*/
	SYS_EVENT_MODULE_STAT_GNSS,			/**< モジュール状態(GNSS)										*/
	SYS_EVENT_MODULE_STAT_SW,			/**< モジュール状態(SW)											*/

	SYS_EVENT_NUM						/**< イベント数						*/
} E_SYS_CTRL_EVENT;

/* 
 ** イベントパラメタ
 */ 
typedef enum e_sys_ctrl_event_param
{
	SYS_EVENT_PARAM_NO		= 0,		/**< パラメタなし												*/
	SYS_EVENT_PARAM_MODSTAT_ATT_NG_SW,	/**< 取付不良：操作スイッチ＆外付けインジケータユニット未接続	*/
	SYS_EVENT_PARAM_MODSTAT_ATT_NG_GNSS,/**< 取付不良：GNSSアンテナ未接続								*/
	SYS_EVENT_PARAM_MODSTAT_ATT_NG_CAN,	/**< 取付不良：車両CANエラー									*/
	SYS_EVENT_PARAM_MODSTAT_FNC_NG_CAN,	/**< 内部機能不良：車両CANエラー（認証）						*/
	SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT,	/**< 内部機能不良：マイコン～Bluetooth間、内部通信エラー		*/
	SYS_EVENT_PARAM_MODSTAT_FNC_NG_GNSS,/**< 内部機能不良：マイコン～GNSS間、内部通信エラー				*/
	SYS_EVENT_PARAM_MODSTAT_FNC_OK_BT,	/**< 機能正常：Bluetooth機能									*/
	SYS_EVENT_PARAM_MODSTAT_FNC_OK_GNSS,/**< 機能正常：GNSS機能											*/
	SYS_EVENT_PARAM_MODSTAT_FNC_OK_SW,	/**< 機能正常：SW機能											*/
	SYS_EVENT_PARAM_MODSTAT_FNC_OK_CAN,	/**< 機能正常：CAN機能											*/
	SYS_EVENT_PARAM_SW_MODE_OFF,		/**< SWモード切替：OFF											*/
	SYS_EVENT_PARAM_SW_MODE_AUTO,		/**< SWモード切替：オートREC									*/
	SYS_EVENT_PARAM_SW_MODE_MANUAL,		/**< SWモード切替：マニュアルREC								*/
	SYS_EVENT_PARAM_ACT_MODE_NORMAL,	/**< 動作モード：通常											*/
	SYS_EVENT_PARAM_ACT_MODE_SIMULATION,/**< 動作モード：シミュレーション								*/
	SYS_EVENT_PARAM_ACT_MODE_EEPROM,	/**< 動作モード：EEPROM書換										*/
	SYS_EVENT_PARAM_ACT_MODE_FIRMWARE,	/**< 動作モード：ファームウェア書換								*/
	SYS_EVENT_PARAM_ACT_MODE_INSPECTION, /**< 動作モード：検査モード									*/
	SYS_EVENT_PARAM_ACT_MODE_BT_COMM,	/**< 動作モード：BTモジュール通信								*/
	SYS_EVENT_PARAM_ACT_MODE_GNSS_COM,	/**< 動作モード：GNSSモジュール通信								*/
	SYS_EVENT_PARAM_NUM					/**< パラメタ数													*/
} E_SYS_CTRL_EVENT_PARAM;


/* 
 ** 実行結果
 */ 
typedef enum e_sys_ctrl_event_notifi_result
{
	SYS_EVENT_NOTIFY_OK		= 0,		/**< 正常終了			*/
	SYS_EVENT_NOTIFY_ERR_INVALID_ARG	/**< 引数不正			*/
} E_SYS_CTRL_EVENT_NOTIFI_RESULT;


/* 
 ** モジュール状態
 */ 
typedef enum e_sys_module_stat
{
	SYS_MODULE_STAT_UNKNOWN = 0x01,		/**< モジュール状態：未確定		*/
	SYS_MODULE_STAT_OK		= 0x02,		/**< モジュール状態：正常		*/
	SYS_MODULE_STAT_FNC_NG	= 0x04,		/**< モジュール状態：機能不良	*/
	SYS_MODULE_STAT_ATT_NG	= 0x08,		/**< モジュール状態：取付不良	*/
} E_SYS_MODULE_STAT;


/*--------------------------------------------------------------------------*/
/*	定数宣言																*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*	構造体宣言																*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*	extern宣言																*/
/*--------------------------------------------------------------------------*/
#ifdef _VDV_BOOT
extern UI_8	g_MainLoopFlag;
#endif /* _VDV_BOOT */

/*--------------------------------------------------------------------------*/
/*	プロトタイプ宣言														*/
/*--------------------------------------------------------------------------*/
/** 
 * 初期化
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	システム状態制御で使用するRAMの初期化処理を行う。
 */ 
void System_control_Init(void);

/** 
 * メイン
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	イベント通知状態によって、システム動作状態（モード）を決定する。
 */ 
void System_control_Main(void);

/********************************************************************************/
/* 関数名称		： System_control_GetStatus										*/
/* 機能の説明	： システム動作状態取得											*/
/* 引数			： void															*/
/* 戻り値		： システム動作状態												*/
/* 				： SYS_OPERATION_WAIT_ACC_ON,			ACC ON待ち				*/
/* 				： SYS_OPERATION_ACC_ON_UNKNOWN_STAT,	ACC ON（状態判定）		*/
/* 				： SYS_OPERATION_NORMAL_OFF,			通常:OFFモード			*/
/* 				： SYS_OPERATION_NORMAL_AUTO,			通常:オートREC			*/
/* 				： SYS_OPERATION_NORMAL_MANUAL,			通常:マニュアルREC		*/
/* 				： SYS_OPERATION_ERROR,					異常					*/
/* 				： SYS_OPERATION_SIMULATION,			シミュレーション		*/
/* 				： SYS_OPERATION_EEPROM_WRITE,			EEPROM書換中			*/
/* 				： SYS_OPERATION_EEPROM_WRITE_COMP,		EEPROM書換完了			*/
/* 				： SYS_OPERATION_FIRMWARE_WRITE,		ファームウェア書換中	*/
/* 				： SYS_OPERATION_FIRMWARE_WRITE_COMP,	ファームウェア書換完了	*/
/* 				： SYS_OPERATION_PRE_ACC_OFF,			ACC OFF移行				*/
/* 				： SYS_OPERATION_ACC_OFF,				ACC OFF					*/
/* 作成者		： (OFT)Sakata													*/
/* 作成日		： 16/08/12														*/
/* 備考			： 																*/
/********************************************************************************/
E_SYS_CTRL_STATUS System_control_GetStatus(void);

/** 
 * イベント通知によるシステム動作状態決定
 * 
 * @param	event		[in]	通知するイベント
 * @param	param		[in]	通知するパラメタ
 *
 * @return	実行結果
 * @retval	SYS_EVENT_NOTIFY_OK				正常終了
 * @retval	SYS_EVENT_NOTIFY_ERR_INVALIDARG	引数不正
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	イベントを通知する。
 */ 
/************************************************************/
/* 関数名称		： System_control_NotifyEvent				*/
/* 機能の説明	： イベント通知								*/
/* 引数			： (I/ )イベント							*/
/* 引数			： (I/ )イベントパラメタ					*/
/* 戻り値		： 取付不良		SYS_MODULE_STAT_ATT_NG		*/
/* 				： 機能不良		SYS_MODULE_STAT_FNC_NG		*/
/* 				： 未確定		SYS_MODULE_STAT_UNKNOWN		*/
/* 				： 正常			SYS_MODULE_STAT_OK			*/
/* 作成者		： (OFT)Sakata								*/
/* 作成日		： 16/08/12									*/
/* 備考			： 											*/
/************************************************************/
UI_8 System_control_NotifyEvent(E_SYS_CTRL_EVENT event, E_SYS_CTRL_EVENT_PARAM param);

/********************************************************/
/* 関数名称		： System_control_GetModuleStatus		*/
/* 機能の説明	： モジュール状態取得					*/
/* 引数			： void									*/
/* 戻り値		： 取付不良		SYS_MODULE_STAT_ATT_NG	*/
/* 				： 機能不良		SYS_MODULE_STAT_FNC_NG	*/
/* 				： 未確定		SYS_MODULE_STAT_UNKNOWN	*/
/* 				： 正常			SYS_MODULE_STAT_OK		*/
/* 作成者		： (OFT)Sakata							*/
/* 作成日		： 16/08/12								*/
/* 備考			： 										*/
/********************************************************/
E_SYS_MODULE_STAT System_control_GetModuleStatus(void);

#endif	/* SYSTEM_CONTROL_TASK_H */
