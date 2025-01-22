/********************************************************/
/* @file System_control_TASK.c							*/
/* システム状態制御										*/
/* 														*/
/* システム動作状態の制御を行う。						*/
/* <ul>													*/
/* <li>ベントを通知によって、システム動作状態（モード）を決定する。 */
/* <li>システム動作状態を取得する。						*/
/* </ul>												*/
/* 														*/
/********************************************************/
/* @author      Y.Sorimachi								*/
/* @date        2013/--/--								*/
/********************************************************/ 

#include "system.h"
#include "Timer.h"	
#include "System_control_TASK.h"
#include "timer_ctrl.h"
#include "DTO459_VDV_Sw_ctrl.h"
#include "DTO459_VDV_Signal_ctrl.h"
#include "chatt_absorber.h"
#include "self_holding_ctrl.h"
#include "dbglog.h"

/********************************************************/
/*	内部マクロ											*/
/********************************************************/
#define SYS_LOCAL	static

#define SYS_END_PROC_BT			(0x01)										/**< 終了処理完了(BT通信制御) 	*/
#define SYS_END_PROC_SD			(0x02)										/**< 終了処理完了(SD制御)		*/
#define SYS_END_PROC_EEPROM		(0x04)										/**< 終了処理完了(SD制御)		*/
#define SYS_END_PROC_FLASH		(0x08)										/**< 終了処理完了(FLASH制御)	*/
#ifdef _VDV_APP
#define SYS_END_PROC_ALL		(SYS_END_PROC_BT | SYS_END_PROC_SD | SYS_END_PROC_EEPROM)	/**< 全終了処理完了	 			*/
#else /* _VDV_APP */
#define SYS_END_PROC_ALL		(SYS_END_PROC_SD | SYS_END_PROC_FLASH)						/**< 全終了処理完了	 			*/
#endif /* _VDV_APP */
#define SYS_FORCE_ACC_OFF_TIME	(10000)														/**< ACCOFF強制以降時間			*/


/********************************************************/
/*	内部関数プロトタイプ宣言							*/
/********************************************************/
/*
 *	イベント処理関数
 */
SYS_LOCAL void S_sys_event_acc_on( UI_8 param );					/**< SYS_EVENT_ACC_ON				*/
SYS_LOCAL void S_sys_event_acc_off( UI_8 param );					/**< SYS_EVENT_ACC_OFF				*/
SYS_LOCAL void S_sys_event_module_stat( UI_8 param );				/**< SYS_EVENT_MODULE_STAT			*/
SYS_LOCAL void S_sys_event_change_sw_mode( UI_8 param );			/**< SYS_EVENT_CHANGE_SW_MODE		*/
SYS_LOCAL void S_sys_event_decision_action_mode( UI_8 param );		/**< SYS_EVENT_DECISION_ACTION_MODE	*/
SYS_LOCAL void S_sys_event_end_proc_comp_bt( UI_8 param );			/**< SYS_EVENT_END_PROC_COMP_BT		*/
SYS_LOCAL void S_sys_event_end_proc_comp_sd( UI_8 param );			/**< SYS_EVENT_END_PROC_COMP_SD		*/
SYS_LOCAL void S_sys_event_end_proc_comp_eeprom( UI_8 param );		/**< SYS_EVENT_END_PROC_COMP_EEPROM	*/
SYS_LOCAL void S_sys_event_end_proc_comp_flash( UI_8 param );		/**< SYS_EVENT_END_PROC_COMP_FLASH	*/
SYS_LOCAL void S_sys_event_end_eeprom_write( UI_8 param );			/**< SYS_EVENT_END_EEPROM_WRITE		*/
SYS_LOCAL void S_sys_event_end_flash_write( UI_8 param );			/**< SYS_EVENT_END_FLASH_WRITE		*/
SYS_LOCAL void S_sys_event_end_inspection( UI_8 param );			/**< SYS_EVENT_END_INSPECTION		*/


/********************************************************/
/*	定数宣言											*/
/********************************************************/
/*
 *	イベント処理関数テーブル
 */
SYS_LOCAL void ( * const C_sys_event_func[SYS_EVENT_NUM] ) ( UI_8 ) = {
	S_sys_event_acc_on,									/**< SYS_EVENT_ACC_ON					*/
	S_sys_event_acc_off,								/**< SYS_EVENT_ACC_OFF					*/
	S_sys_event_module_stat,							/**< SYS_EVENT_MODULE_STAT	(BT)		*/
	S_sys_event_change_sw_mode,							/**< SYS_EVENT_CHANGE_SW_MODE			*/
	S_sys_event_decision_action_mode,					/**< SYS_EVENT_DECISION_ACTION_MODE		*/
	S_sys_event_end_proc_comp_bt,						/**< SYS_EVENT_END_PROC_COMP_BT			*/
	S_sys_event_end_proc_comp_sd,						/**< SYS_EVENT_END_PROC_COMP_SD			*/
	S_sys_event_end_proc_comp_eeprom,					/**< SYS_EVENT_END_PROC_COMP_EEPROM		*/
	S_sys_event_end_proc_comp_flash,					/**< SYS_EVENT_END_PROC_COMP_FLASH		*/
	S_sys_event_end_eeprom_write,						/**< SYS_EVENT_END_EEPROM_WRITE			*/
	S_sys_event_end_flash_write,						/**< SYS_EVENT_END_FLASH_WRITE			*/
	S_sys_event_end_inspection,							/**< SYS_EVENT_END_INSPECTION		*/
	S_sys_event_module_stat,							/**< SYS_EVENT_MODULE_STAT_CAN			*/
	S_sys_event_module_stat,							/**< SYS_EVENT_MODULE_STAT_GNSS			*/
	S_sys_event_module_stat,							/**< SYS_EVENT_MODULE_STAT_SW			*/
};

/* 
 ** モジュール種別
 */ 
typedef enum e_sys_module_kind
{
	SYS_MODULE_KIND_BT = 0,				/**< モジュール状態：BT		*/
	SYS_MODULE_KIND_CAN,				/**< モジュール状態：CAN	*/
	SYS_MODULE_KIND_GNSS,				/**< モジュール状態：GNSS	*/
	SYS_MODULE_KIND_SW,					/**< モジュール状態：SW		*/
	SYS_MODULE_KIND_NUM					/**< モジュール状態数		*/
} E_SYS_MODULE_KIND;


typedef enum e_sys_action_mode
{
	SYS_ACTION_MODE_UNKNOWN = 0,			/**< 動作モード：未確定				*/
	SYS_ACTION_MODE_NORMAL,	    			/**< 動作モード：通常				*/
	SYS_ACTION_MODE_SIMULATION,				/**< 動作モード：シミュレーション	*/
	SYS_ACTION_MODE_EEPROM,	    			/**< 動作モード：EEPROM書換			*/
	SYS_ACTION_MODE_FIRMWARE,				/**< 動作モード：ファームウェア書換	*/
	SYS_ACTION_MODE_INSPECTION,				/**< 動作モード：検査モード			*/
	SYS_ACTION_MODE_BT_COMM,				/**< 動作モード：BTモジュール通信モード		*/
	SYS_ACTION_MODE_GNSS_COMM,				/**< 動作モード：GNSSモジュール通信モード	*/
	SYS_ACTION_MODE_NUM						/**< 動作モード数					*/
} E_SYS_ACTION_MODE;


/********************************************************/
/*	内部変数定義										*/
/********************************************************/
SYS_LOCAL	E_SYS_CTRL_STATUS	S_sys_operation_mode;							/**< システム動作状態 */
SYS_LOCAL	UI_8	S_sys_notify_event_state[SYS_EVENT_NUM];					/**< イベント通知状態 */
SYS_LOCAL	E_SYS_CTRL_EVENT_PARAM	S_sys_notify_event_param[SYS_EVENT_NUM];	/**< イベントパラメタ */
SYS_LOCAL	UI_8	S_sys_notify_event_flg;										/**< イベント通知フラグ(N_ON：通知あり、N_OFF：通知なし) */
SYS_LOCAL	UI_8	S_sys_end_state;											/**< 完了状態 */
SYS_LOCAL	UI_8	S_sys_module_state_list[SYS_MODULE_KIND_NUM];				/**< 各モジュール状態 */
SYS_LOCAL	E_SYS_MODULE_STAT	S_sys_module_state;								/**< 全体モジュール状態 */
SYS_LOCAL	E_SYS_ACTION_MODE	S_sys_action_mode;								/**< SDから取得した動作モード */
SYS_LOCAL	UI_16	S_sys_accoff_time;											/**< ACCOFF移行時間 */
/********************************************************/
/*	外部関数											*/
/********************************************************/


/********************************************************/
/*	外部変数											*/
/********************************************************/
#ifdef _VDV_BOOT
UI_8	g_MainLoopFlag;
#endif /* _VDV_BOOT */

/** 
 * 初期化
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	システム状態制御で使用するRAMの初期化処理を行う。
 */ 
void System_control_Init(void)
{
	UI_8 i;
	/**	<ol>	*/
	/**	<li>初期化	*/
	S_sys_operation_mode 	= SYS_OPERATION_WAIT_ACC_ON;
	S_sys_notify_event_flg 	= N_OFF;
	S_sys_end_state			= 0;
	S_sys_accoff_time = 0;
	S_sys_action_mode 		= SYS_ACTION_MODE_UNKNOWN;
	/*モジュール状態初期化*/
	for ( i = 0; i < SYS_MODULE_KIND_NUM; i++ )
	{
		S_sys_module_state_list[i] = SYS_MODULE_STAT_UNKNOWN;
	}
	S_sys_module_state = SYS_MODULE_STAT_UNKNOWN;
	for ( i = 0; i < SYS_EVENT_NUM; i++ )
	{
		S_sys_notify_event_state[i] = N_OFF;
		S_sys_notify_event_param[i] = SYS_EVENT_PARAM_NO;
	}
#ifdef _VDV_APP
	/*公開信号制御初期化*/
	Signal_control_Init();
#endif /* _VDV_APP */
	/**	<li>チャタリング吸収	*/
	Chatt_absorber_Init();
	/**	<li>自己保持制御	*/
	Self_holding_ctrl_Init();
	/**	</ol>	*/
	return;
	
}

/** 
 * メイン
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	イベント通知状態によって、システム動作状態（モード）を決定する。
 */ 
void System_control_Main(void)
{
	UI_8 i;
	UI_8 param;
	E_SYS_CTRL_STATUS	pre_operation_mode;							/**< システム動作状態 */
	
	/**	<ol>	*/
	/**	<li>イベント通知の有無を判定	*/
	if ( S_sys_notify_event_flg == N_ON )
	{
		/**	<li>通知があったイベント毎によって、システム動作状態（モード）を決定する。	*/
		for ( i = 0; i < SYS_EVENT_NUM; i++ )
		{
			if ( S_sys_notify_event_state[i] == N_ON )
			{
				pre_operation_mode = S_sys_operation_mode;
				param = S_sys_notify_event_param[i];
				C_sys_event_func[i](param);
				S_sys_notify_event_state[i] = N_OFF;
				S_sys_notify_event_param[i] = SYS_EVENT_PARAM_NO;
				if(pre_operation_mode != S_sys_operation_mode)
				{
					OUTLOG(OutputLog( "[SYS] STATUS %d->%d\n", pre_operation_mode, S_sys_operation_mode);)
				}
			}
		}
		/**	<li>イベント通知フラグをクリア	*/
		S_sys_notify_event_flg = N_OFF;
	}
	else
	{
		
	}
	if(S_sys_operation_mode == SYS_OPERATION_PRE_ACC_OFF)
	{
		/* ACC OFF移行中 */
		if (Timer_ctrl_TimeCompare10ms(S_sys_accoff_time, SYS_FORCE_ACC_OFF_TIME/TIME_MAIN) == N_OK)
		{
			/**	タイムアウトでACC OFFに強制移行させる	*/
			S_sys_operation_mode = SYS_OPERATION_ACC_OFF;
		}
	}
	/**	<li>チャタリング吸収	*/
	Chatt_absorber_Main();
	/**	<li> 自己保持制御 */
	Self_holding_ctrl_Main();
	/**	</ol>	*/
	
	
	return;
}

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
/* 				： SYS_OPERATION_INSPECTION,			検査モード				*/
/* 				： SYS_OPERATION_INSPECTION_COMP,		検査完了				*/
/* 				： SYS_OPERATION_BT_COMM,				BTモジュール通信モード	*/
/* 				： SYS_OPERATION_GNSS_COMM,				GNSSモジュール通信モード*/
/* 				： SYS_OPERATION_PRE_ACC_OFF,			ACC OFF移行				*/
/* 				： SYS_OPERATION_ACC_OFF,				ACC OFF					*/
/* 作成者		： (OFT)Sakata													*/
/* 作成日		： 16/08/12														*/
/* 備考			： 																*/
/********************************************************************************/
E_SYS_CTRL_STATUS System_control_GetStatus(void)
{
	/**	<ol>	*/
	/**	</ol>	*/
	return S_sys_operation_mode;
}


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
E_SYS_MODULE_STAT System_control_GetModuleStatus(void)
{
	return S_sys_module_state;
}


/** 
 * イベント通知
 * 
 * @param	event		[in]	通知するイベント
 * @param	param		[in]	通知するパラメタ
 *
 * @return	実行結果
 * @retval	SYS_EVENT_NOTIFY_OK					正常終了
 * @retval	SYS_EVENT_NOTIFY_ERR_INVALID_ARG	引数不正
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
UI_8 System_control_NotifyEvent(E_SYS_CTRL_EVENT event, E_SYS_CTRL_EVENT_PARAM param)
{
	/**	<ol>	*/
	/**	<li>引数チェック	*/
	if ( event >= SYS_EVENT_NUM )
	{
		return SYS_EVENT_NOTIFY_ERR_INVALID_ARG;
	}
	/* 同時にイベントが発生することがあるので状態通知イベントをモジュール毎に分ける*/
	/**< イベント：モジュール状態 */
	if(	event == SYS_EVENT_MODULE_STAT )
	{
		/* ＜注意＞ SYS_EVENT_PARAM_MODSTAT_XXXXXの定義を追加した場合はここに追記すること！*/
		/* パラメタ：CAN状態 */
		if ( (param == SYS_EVENT_PARAM_MODSTAT_FNC_OK_CAN) || (param == SYS_EVENT_PARAM_MODSTAT_FNC_NG_CAN) || (param == SYS_EVENT_PARAM_MODSTAT_ATT_NG_CAN) )
		{
			/* イベント：CAN状態を設定 */
			event = SYS_EVENT_MODULE_STAT_CAN;
		}
		else /* パラメタ：GNSS状態 */ if( (param == SYS_EVENT_PARAM_MODSTAT_FNC_NG_GNSS) || (param == SYS_EVENT_PARAM_MODSTAT_FNC_OK_GNSS) || (param == SYS_EVENT_PARAM_MODSTAT_ATT_NG_GNSS))
		{
			/* イベント：GNSS状態を設定 */
			event = SYS_EVENT_MODULE_STAT_GNSS;
		}
		else /* パラメタ：SW状態 */ if( (param == SYS_EVENT_PARAM_MODSTAT_FNC_OK_SW) || (param == SYS_EVENT_PARAM_MODSTAT_ATT_NG_SW) )
		{
			/* イベント：SW状態を設定 */
			event = SYS_EVENT_MODULE_STAT_SW;
		}
		else
		{
			/* パラメタ：BT状態 */
			/*処理なし：そのまま実施*/
		}
	}
	
	/**	<li>イベント発生を通知	*/
	if ( S_sys_notify_event_state[event] == N_OFF)
	{
		S_sys_notify_event_state[event] = N_ON;
		S_sys_notify_event_param[event] = param;
		S_sys_notify_event_flg = N_ON;
		OUTLOG(OutputLog( "[SYS] EV %d PARAM %d STATUS %d\n", event, param, S_sys_operation_mode);)
	}
	/**	</ol>	*/
	return SYS_EVENT_NOTIFY_OK;
	
}

/********************************************************/
/*	内部関数											*/
/********************************************************/

/**
 * イベント処理関数：ACC ON
 * 
 * @param	param		[in]	通知するパラメタ
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	
 */
SYS_LOCAL void S_sys_event_acc_on( UI_8 param )
{
	UI_8 i;
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_WAIT_ACC_ON):	/*ACC ON待ち*/
	case (SYS_OPERATION_PRE_ACC_OFF):	/*ACC OFF移行*/
		/*ACC ON（状態判定）	*/
		S_sys_operation_mode = SYS_OPERATION_ACC_ON_UNKNOWN_STAT;
		/*動作モード初期化	*/
		S_sys_action_mode 	 = SYS_ACTION_MODE_UNKNOWN;
		/*モジュール状態初期化*/
		for ( i = 0; i < SYS_MODULE_KIND_NUM; i++ )
		{
			S_sys_module_state_list[i] = SYS_MODULE_STAT_UNKNOWN;
		}
		S_sys_module_state = SYS_MODULE_STAT_UNKNOWN;
		break;
	case (SYS_OPERATION_ACC_OFF):	/*ACC ON待ち*/
		while(1)
		{
			/*---フェール処理---*/
			/*自己保持ポートを非アクティブにしても何らかの原因で電源が落ちずにACC ONを検出した場合、
			  ACC ON復帰させるために、無限ループさせてウォッチドッグで落ちて再起動させる		*/
		}
		break;
	default:
		break;
	}
	/**	</ol>	*/
	return;
}

/**
 * イベント処理関数：ACC OFF
 * 
 * @param	param		[in]	通知するパラメタ
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	
 */
SYS_LOCAL void S_sys_event_acc_off( UI_8 param )
{
	/**	<ol>	*/
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_WAIT_ACC_ON):			/*ACC ON待ち			*/
	case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF移行			*/
	case (SYS_OPERATION_ACC_OFF):				/*ACC OFF				*/
		/* nop */
		break;

	case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON（状態判定）	*/
	case (SYS_OPERATION_NORMAL_OFF):			/*通常:OFFモード		*/
	case (SYS_OPERATION_NORMAL_AUTO):			/*通常:オートREC		*/
	case (SYS_OPERATION_NORMAL_MANUAL):			/*通常:マニュアルREC	*/
	case (SYS_OPERATION_ERROR):					/*異常					*/
	case (SYS_OPERATION_SIMULATION):			/*シミュレーション		*/
	case (SYS_OPERATION_EEPROM_WRITE):			/*EEPROM書換中			*/
	case (SYS_OPERATION_EEPROM_WRITE_COMP):		/*EEPROM書換完了		*/
	case (SYS_OPERATION_FIRMWARE_WRITE):		/*ファームウェア書換中	*/
	case (SYS_OPERATION_FIRMWARE_WRITE_COMP):	/*ファームウェア書換完了*/
	case (SYS_OPERATION_INSPECTION):			/*検査モード			*/
	case (SYS_OPERATION_INSPECTION_COMP):		/*検査完了				*/
	case (SYS_OPERATION_BT_COMM):				/*BTモジュール通信モード	*/
	case (SYS_OPERATION_GNSS_COMM):				/*GNSSモジュール通信モード	*/
	default:
		/*ACC OFF移行			*/
		S_sys_end_state		 = 0;
		/* ACC OFF移行時の時間を保持 */
		S_sys_accoff_time = Timer_ctrl_GetTime10ms();
		S_sys_operation_mode = SYS_OPERATION_PRE_ACC_OFF;
		break;
	}
	/**	</ol>	*/
	return;
}

/********************************************************/
/* 関数名称		： S_sys_event_module_stat				*/
/* 機能の説明	： イベント処理関数：モジュール状態		*/
/* 引数			： (I/)param：イベントパラメタ			*/
/* 戻り値		： void									*/
/* 作成者		： (OFT)Sakata							*/
/* 作成日		： 16/08/12								*/
/* 備考			： 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_module_stat( UI_8 param )
{
	/*モジュール状態取得		*/
	UI_8 i;
	UI_8 unknownCnt = 0;
	UI_8 okCnt = 0;
	UI_8 attNGCnt = 0;
	UI_8 fncNGCnt = 0;
	E_SYS_MODULE_STAT stat;
#ifdef _VDV_APP
	E_SW_MODE_STATUS swmode;
#endif /* _VDV_APP */
	/**	<ol>	*/
	switch( param )
	{
		/**< 取付不良：操作スイッチ＆外付けインジケータユニット未接続	*/
		case SYS_EVENT_PARAM_MODSTAT_ATT_NG_SW:
			S_sys_module_state_list[SYS_MODULE_KIND_SW] |= SYS_MODULE_STAT_ATT_NG;
			break;
		/**< 取付不良：GNSSアンテナ未接続								*/
		case SYS_EVENT_PARAM_MODSTAT_ATT_NG_GNSS:
			S_sys_module_state_list[SYS_MODULE_KIND_GNSS] |= SYS_MODULE_STAT_ATT_NG;
			break;
		/**< 取付不良：車両CANエラー									*/
		case SYS_EVENT_PARAM_MODSTAT_ATT_NG_CAN:
			S_sys_module_state_list[SYS_MODULE_KIND_CAN] |= SYS_MODULE_STAT_ATT_NG;
			break;
		/**< 内部機能不良：車両CANエラー（認証エラー）					*/
		case SYS_EVENT_PARAM_MODSTAT_FNC_NG_CAN:
			S_sys_module_state_list[SYS_MODULE_KIND_CAN] |= SYS_MODULE_STAT_FNC_NG;
			break;
		/**< 内部機能不良：マイコン～Bluetooth間、内部通信エラー		*/
		case SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT:
			S_sys_module_state_list[SYS_MODULE_KIND_BT] |= SYS_MODULE_STAT_FNC_NG;
			break;
		/**< 内部機能不良：マイコン～GNSS間、内部通信エラー				*/
		case SYS_EVENT_PARAM_MODSTAT_FNC_NG_GNSS:
			S_sys_module_state_list[SYS_MODULE_KIND_GNSS] |= SYS_MODULE_STAT_FNC_NG;
			break;
		/**< 機能正常：Bluetooth機能									*/
		case SYS_EVENT_PARAM_MODSTAT_FNC_OK_BT:
			S_sys_module_state_list[SYS_MODULE_KIND_BT] = SYS_MODULE_STAT_OK;
			break;
		/**< 機能正常：GNSS機能											*/
		case SYS_EVENT_PARAM_MODSTAT_FNC_OK_GNSS:
			S_sys_module_state_list[SYS_MODULE_KIND_GNSS] = SYS_MODULE_STAT_OK;
			break;
		/**< 機能正常：SW機能											*/
		case SYS_EVENT_PARAM_MODSTAT_FNC_OK_SW:
			S_sys_module_state_list[SYS_MODULE_KIND_SW] = SYS_MODULE_STAT_OK;
			break;
		/**< 機能正常：CAN機能											*/
		case SYS_EVENT_PARAM_MODSTAT_FNC_OK_CAN:
			S_sys_module_state_list[SYS_MODULE_KIND_CAN] = SYS_MODULE_STAT_OK;
			break;
		/**< その他														*/
		default:
			break;
	}
#ifdef _VDV_APP
#endif /* _VDV_APP */
	/* モジュール状態設定 */
	/**	</ol>	*/
	for ( i = 0; i < SYS_MODULE_KIND_NUM; i++ )
	{
		if((S_sys_module_state_list[i] & SYS_MODULE_STAT_UNKNOWN) != 0)
		{
			/**< モジュール状態：未確定		*/
			unknownCnt++;
		}
		if((S_sys_module_state_list[i] & SYS_MODULE_STAT_OK) != 0)
		{
			/**< モジュール状態：正常		*/
			okCnt++;
		}
		if((S_sys_module_state_list[i] & SYS_MODULE_STAT_FNC_NG) != 0)
		{
			/**< モジュール状態：機能不良	*/
			fncNGCnt++;
		}
		if((S_sys_module_state_list[i] & SYS_MODULE_STAT_ATT_NG) != 0)
		{
			/**< モジュール状態：取付不良	*/
			attNGCnt++;
		}
	}
	/* 機能不良、取付不良不良が同時発生の場合は機能不良を優先とする */
	if(fncNGCnt > 0)
	{
		/**< モジュール状態：機能不良	*/
		stat = SYS_MODULE_STAT_FNC_NG;
	}
	else if(attNGCnt > 0)
	{
		/**< モジュール状態：取付不良	*/
		stat = SYS_MODULE_STAT_ATT_NG;
	}
	else if(unknownCnt > 0)
	{
		/**< モジュール状態：未確定		*/
		stat = SYS_MODULE_STAT_UNKNOWN;
	}
	else
	{
		/**< モジュール状態：正常		*/
		stat = SYS_MODULE_STAT_OK;
	}
	/* モジュール状態更新 */
	S_sys_module_state = stat;
	
	switch ( S_sys_operation_mode )
	{
		case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON（状態判定）	*/
			if((S_sys_module_state == SYS_MODULE_STAT_ATT_NG) || (S_sys_module_state == SYS_MODULE_STAT_FNC_NG))
			{
				/*取付不良 or 機能不良*/
				switch( S_sys_action_mode )
				{
					/**< 動作モード：通常											*/
					case SYS_ACTION_MODE_NORMAL:
						S_sys_operation_mode = SYS_OPERATION_ERROR;
						break;
					/**< その他														*/
					case SYS_ACTION_MODE_NUM:	/* QAC対策のため追加(switch文は条件3つ(default含む)以上必要) */
					default:
						break;
				}
			}
			else if(S_sys_module_state == SYS_MODULE_STAT_UNKNOWN)
			{
				/*未確定*/
			}
			else if(S_sys_module_state == SYS_MODULE_STAT_OK)
			{
				/*正常*/
				switch( S_sys_action_mode )
				{
					/**< 動作モード：通常											*/
					case SYS_ACTION_MODE_NORMAL:
#ifdef _VDV_APP
						/*SWモード取得*/
						swmode = Sw_control_GetSwMode();
						switch( swmode )
						{
							case SW_MODE_OFF:		/**< SWモード：OFF				*/
								S_sys_operation_mode = SYS_OPERATION_NORMAL_OFF;
								break;
							case SW_MODE_AUTO:		/**< SWモード：オートREC		*/
								S_sys_operation_mode = SYS_OPERATION_NORMAL_AUTO;
								break;
							case SW_MODE_MANUAL:	/**< SWモード：マニュアルREC	*/
								S_sys_operation_mode = SYS_OPERATION_NORMAL_MANUAL;
								break;
							default:
								break;
						}
#endif /* _VDV_APP */
						break;
					/**< その他														*/
					case SYS_ACTION_MODE_NUM:	/* QAC対策のため追加(switch文は条件3つ(default含む)以上必要) */
					default:
						break;
				}
			}
			else
			{
				/* nop */
			}
			break;
		case (SYS_OPERATION_NORMAL_OFF):			/*通常:OFFモード		*/
		case (SYS_OPERATION_NORMAL_AUTO):			/*通常:オートREC		*/
		case (SYS_OPERATION_NORMAL_MANUAL):			/*通常:マニュアルREC	*/
			if((S_sys_module_state == SYS_MODULE_STAT_ATT_NG) || (S_sys_module_state == SYS_MODULE_STAT_FNC_NG))
			{
				/*取付不良 or 機能不良*/
				S_sys_operation_mode = SYS_OPERATION_ERROR;
			}
			break;
		case (SYS_OPERATION_ERROR):					/*異常					*/
			/* 異常状態になったらACC OFFされるまで復帰しない */
			break;
		default:
			break;
	}
	
	/**	</ol>	*/
	return;
}

/********************************************************/
/* 関数名称		： S_sys_event_change_sw_mode			*/
/* 機能の説明	： イベント処理関数：SWモード切替		*/
/* 引数			： (I/)param：イベントパラメタ			*/
/* 戻り値		： void									*/
/* 作成者		： (OFT)Sakata							*/
/* 作成日		： 16/08/12								*/
/* 備考			： 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_change_sw_mode( UI_8 param )
{
	/**	<ol>	*/
	switch ( S_sys_operation_mode )
	{
		case (SYS_OPERATION_NORMAL_OFF):			/**< 通常:OFFモード			*/
		case (SYS_OPERATION_NORMAL_AUTO):			/**< 通常:オートREC			*/
		case (SYS_OPERATION_NORMAL_MANUAL):			/**< 通常:マニュアルREC		*/
			switch( param )
			{
				/**< モード切替：OFF											*/
				case SYS_EVENT_PARAM_SW_MODE_OFF:
					S_sys_operation_mode = SYS_OPERATION_NORMAL_OFF;
					break;
				/**< モード切替：オートREC										*/
				case SYS_EVENT_PARAM_SW_MODE_AUTO:
					S_sys_operation_mode = SYS_OPERATION_NORMAL_AUTO;
					break;
				/**< モード切替：マニュアルREC									*/
				case SYS_EVENT_PARAM_SW_MODE_MANUAL:
					S_sys_operation_mode = SYS_OPERATION_NORMAL_MANUAL;
					break;
				/**< その他														*/
				default:
					break;
			}
			break;
		case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON（状態判定）	*/
			if(S_sys_module_state == SYS_MODULE_STAT_OK)
			{
				/*正常*/
				switch( S_sys_action_mode )
				{
					/**< 動作モード：通常											*/
					case SYS_ACTION_MODE_NORMAL:
						/*先にSDからの通知を受信済み*/
						switch( param )
						{
							/**< モード切替：OFF											*/
							case SYS_EVENT_PARAM_SW_MODE_OFF:
								S_sys_operation_mode = SYS_OPERATION_NORMAL_OFF;
								break;
							/**< モード切替：オートREC										*/
							case SYS_EVENT_PARAM_SW_MODE_AUTO:
								S_sys_operation_mode = SYS_OPERATION_NORMAL_AUTO;
								break;
							/**< モード切替：マニュアルREC									*/
							case SYS_EVENT_PARAM_SW_MODE_MANUAL:
								S_sys_operation_mode = SYS_OPERATION_NORMAL_MANUAL;
								break;
							/**< その他														*/
							default:
								break;
						}
						break;
					/**< その他														*/
					case SYS_ACTION_MODE_NUM:	/* QAC対策のため追加(switch文は条件3つ(default含む)以上必要) */
					default:
						break;
				}
			}
			break;
		default:
			break;
	}
	/**	</ol>	*/
	return;
}

/********************************************************/
/* 関数名称		： S_sys_event_decision_action_mode		*/
/* 機能の説明	： イベント処理関数：動作モード決定		*/
/* 引数			： (I/)param：イベントパラメタ			*/
/* 戻り値		： void									*/
/* 作成者		： (OFT)Sakata							*/
/* 作成日		： 16/08/12								*/
/* 備考			： 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_decision_action_mode( UI_8 param )
{
	/**	<ol>	*/
#ifdef _VDV_APP
	UI_8 stat;
	E_SW_MODE_STATUS swmode;
#endif /* _VDV_APP */
	switch ( S_sys_operation_mode )
	{
		case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON（状態判定）		*/
			switch( param )
			{
				/**< 動作モード：通常											*/
				case SYS_EVENT_PARAM_ACT_MODE_NORMAL:
#ifdef _VDV_APP
					S_sys_action_mode 		= SYS_ACTION_MODE_NORMAL;
					/*モジュール状態取得		*/
					stat = System_control_GetModuleStatus();
					if(stat == SYS_MODULE_STAT_OK)
					{
						/*SWモード取得*/
						swmode = Sw_control_GetSwMode();
						switch( swmode )
						{
							case SW_MODE_OFF:		/**< SWモード：OFF				*/
								S_sys_operation_mode = SYS_OPERATION_NORMAL_OFF;
								break;
							case SW_MODE_AUTO:		/**< SWモード：オートREC		*/
								S_sys_operation_mode = SYS_OPERATION_NORMAL_AUTO;
								break;
							case SW_MODE_MANUAL:	/**< SWモード：マニュアルREC	*/
								S_sys_operation_mode = SYS_OPERATION_NORMAL_MANUAL;
								break;
							default:
								break;
						}
					}
					else if((stat == SYS_MODULE_STAT_ATT_NG) || (stat == SYS_MODULE_STAT_FNC_NG))
					{
						/*取付不良 or 機能不良*/
						S_sys_operation_mode = SYS_OPERATION_ERROR;
					}
					else
					{
						/* その他状態の時は処理なし */
					}
#else /* _VDV_APP */
					g_MainLoopFlag = 0;
#endif /* _VDV_APP */
					break;
				/**< 動作モード：シミュレーション								*/
				case SYS_EVENT_PARAM_ACT_MODE_SIMULATION:
#ifdef _VDV_APP
					S_sys_action_mode 		= SYS_ACTION_MODE_SIMULATION;
					S_sys_operation_mode 	= SYS_OPERATION_SIMULATION;
#else /* _VDV_APP */
					g_MainLoopFlag = 0;
#endif /* _VDV_APP */
					break;
				/**< 動作モード：EEPROM書換										*/
				case SYS_EVENT_PARAM_ACT_MODE_EEPROM:
#ifdef _VDV_APP
					S_sys_action_mode 		= SYS_ACTION_MODE_EEPROM;
					S_sys_operation_mode 	= SYS_OPERATION_EEPROM_WRITE;
#else /* _VDV_APP */
					g_MainLoopFlag = 0;
#endif /* _VDV_APP */
					break;

				/**< 動作モード：検査モード										*/
				case SYS_EVENT_PARAM_ACT_MODE_INSPECTION:
#ifdef _VDV_APP
					S_sys_action_mode 		= SYS_ACTION_MODE_INSPECTION;
					S_sys_operation_mode 	= SYS_OPERATION_INSPECTION;
#else /* _VDV_APP */
					g_MainLoopFlag = 0;
#endif /* _VDV_APP */
					break;

				/**< 動作モード：BTモジュール通信モード							*/
				case SYS_EVENT_PARAM_ACT_MODE_BT_COMM:
#ifdef _VDV_APP
					S_sys_action_mode 		= SYS_ACTION_MODE_BT_COMM;
					S_sys_operation_mode 	= SYS_OPERATION_BT_COMM;
#else /* _VDV_APP */
					g_MainLoopFlag = 0;
#endif /* _VDV_APP */
					break;

				/**< 動作モード：GNSSモジュール通信モード							*/
				case SYS_EVENT_PARAM_ACT_MODE_GNSS_COM:
#ifdef _VDV_APP
					S_sys_action_mode 		= SYS_ACTION_MODE_GNSS_COMM;
					S_sys_operation_mode 	= SYS_OPERATION_GNSS_COMM;
#else /* _VDV_APP */
					g_MainLoopFlag = 0;
#endif /* _VDV_APP */
					break;

#ifdef _VDV_BOOT
				/**< 動作モード：ファームウェア書換								*/
				case SYS_EVENT_PARAM_ACT_MODE_FIRMWARE:
					S_sys_action_mode 		= SYS_ACTION_MODE_FIRMWARE;
					S_sys_operation_mode 	= SYS_OPERATION_FIRMWARE_WRITE;
					break;
#endif /* _VDV_BOOT */
				/**< その他														*/
				default:
#ifdef _VDV_APP
					S_sys_action_mode 		= SYS_ACTION_MODE_UNKNOWN;
#else /* _VDV_APP */
					g_MainLoopFlag = 0;
#endif /* _VDV_APP */
					break;
			}
			break;
		case SYS_OPERATION_NUM:	/* QAC対策のため追加(switch文は条件3つ(default含む)以上必要) */
		default:
			break;
	}
	/**	</ol>	*/
	return;	
}
/********************************************************/
/* 関数名称		： S_sys_event_end_proc_comp_bt			*/
/* 機能の説明	： イベント処理関数：終了処理完了(BT)	*/
/* 引数			： (I/)param：イベントパラメタ			*/
/* 戻り値		： void									*/
/* 作成者		： (OFT)Sakata							*/
/* 作成日		： 16/08/12								*/
/* 備考			： 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_end_proc_comp_bt( UI_8 param )
{
#ifdef _VDV_APP
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF移行			*/
		S_sys_end_state |= SYS_END_PROC_BT;
		/**	<li>全ての終了処理が完了した場合、ACC OFFに遷移する。	*/
		if ( S_sys_end_state == SYS_END_PROC_ALL )
		{
			S_sys_operation_mode = SYS_OPERATION_ACC_OFF;
		}
		break;
	case SYS_OPERATION_NUM:	/* QAC対策のため追加(switch文は条件3つ(default含む)以上必要) */
	default:
		break;
	}
	/**	</ol>	*/
	return;
#endif /* _VDV_APP */
}
/********************************************************/
/* 関数名称		： S_sys_event_end_proc_comp_sd			*/
/* 機能の説明	： イベント処理関数：終了処理完了(SD)	*/
/* 引数			： (I/)param：イベントパラメタ			*/
/* 戻り値		： void									*/
/* 作成者		： (OFT)Sakata							*/
/* 作成日		： 16/08/12								*/
/* 備考			： 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_end_proc_comp_sd( UI_8 param)
{
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF移行			*/
		S_sys_end_state |= SYS_END_PROC_SD;
		/**	<li>全ての終了処理が完了した場合、ACC OFFに遷移する。	*/
		if ( S_sys_end_state == SYS_END_PROC_ALL )
		{
			S_sys_operation_mode = SYS_OPERATION_ACC_OFF;
		}
		break;
	case SYS_OPERATION_NUM:	/* QAC対策のため追加(switch文は条件3つ(default含む)以上必要) */
	default:
		break;
	}
	/**	</ol>	*/
	return;
}
/*********************************************************/
/* 関数名称		： S_sys_event_end_proc_comp_eeprom		 */
/* 機能の説明	： イベント処理関数：終了処理完了(EEPROM)*/
/* 引数			： (I/)param：イベントパラメタ			 */
/* 戻り値		： void									 */
/* 作成者		： (OFT)Sakata							 */
/* 作成日		： 16/08/12								 */
/* 備考			： 										 */
/*********************************************************/
SYS_LOCAL void S_sys_event_end_proc_comp_eeprom( UI_8 param )
{
#ifdef _VDV_APP
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF移行			*/
		S_sys_end_state |= SYS_END_PROC_EEPROM;
		/**	<li>全ての終了処理が完了した場合、ACC OFFに遷移する。	*/
		if ( S_sys_end_state == SYS_END_PROC_ALL )
		{
			S_sys_operation_mode = SYS_OPERATION_ACC_OFF;
		}
		break;
	case SYS_OPERATION_NUM:	/* QAC対策のため追加(switch文は条件3つ(default含む)以上必要) */
	default:
		break;
	}
	return;
#endif /* _VDV_APP */
}


/*********************************************************/
/* 関数名称		： S_sys_event_end_proc_comp_flash		 */
/* 機能の説明	： イベント処理関数：終了処理完了(FLASH) */
/* 引数			： (I/)param：イベントパラメタ			 */
/* 戻り値		： void									 */
/* 作成者		： (OFT)Sakata							 */
/* 作成日		： 16/08/12								 */
/* 備考			： 										 */
/*********************************************************/
SYS_LOCAL void S_sys_event_end_proc_comp_flash( UI_8 param )
{
#ifdef _VDV_BOOT
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF移行		*/
		S_sys_end_state |= SYS_END_PROC_FLASH;
		/**	<li>全ての終了処理が完了した場合、ACC OFFに遷移する。	*/
		if ( S_sys_end_state == SYS_END_PROC_ALL )
		{
			S_sys_operation_mode = SYS_OPERATION_ACC_OFF;
		}
		break;
	case SYS_OPERATION_NUM:	/* QAC対策のため追加(switch文は条件3つ(default含む)以上必要) */
	default:
		break;
	}
	return;
#endif /* _VDV_BOOT */
}

/********************************************************/
/* 関数名称		： S_sys_event_end_eeprom_write			*/
/* 機能の説明	： EEPROM書換完了処理					*/
/* 引数			： (I/)param：イベントパラメタ			*/
/* 戻り値		： void									*/
/* 作成者		： (OFT)Sakata							*/
/* 作成日		： 16/08/12								*/
/* 備考			： 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_end_eeprom_write( UI_8 param )
{
	/**	<ol>	*/
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_EEPROM_WRITE):			/*EEPROM書換中			*/
		/*EEPROM書換完了		*/
		S_sys_operation_mode = SYS_OPERATION_EEPROM_WRITE_COMP;
		break;
	case SYS_OPERATION_NUM:	/* QAC対策のため追加(switch文は条件3つ(default含む)以上必要) */
	default:
		break;
	}
	return;
}

/********************************************************/
/* 関数名称		： S_sys_event_end_flash_write			*/
/* 機能の説明	： FLASH書換完了処理					*/
/* 引数			： (I/)param：イベントパラメタ			*/
/* 戻り値		： void									*/
/* 作成者		： (OFT)Sakata							*/
/* 作成日		： 16/08/12								*/
/* 備考			： 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_end_flash_write( UI_8 param )
{
	/**	<ol>	*/
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_FIRMWARE_WRITE):		/*ファームウェア書換中	*/
		/*ファームウェア書換完了*/
		S_sys_operation_mode = SYS_OPERATION_FIRMWARE_WRITE_COMP;
		break;
	case SYS_OPERATION_NUM:	/* QAC対策のため追加(switch文は条件3つ(default含む)以上必要) */
	default:
		break;
	}
	return;
}

/********************************************************/
/* 関数名称		： S_sys_event_end_inspection			*/
/* 機能の説明	： 検査完了処理							*/
/* 引数			： (I/)param：イベントパラメタ			*/
/* 戻り値		： void									*/
/* 作成者		： (OFT)Toguchi							*/
/* 作成日		： 17/01/20								*/
/* 備考			： 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_end_inspection( UI_8 param )
{
	/**	<ol>	*/
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_INSPECTION):			/*検査モード	*/
		/*検査換完了*/
		S_sys_operation_mode = SYS_OPERATION_INSPECTION_COMP;
		break;
	case SYS_OPERATION_NUM:	/* QAC対策のため追加(switch文は条件3つ(default含む)以上必要) */
	default:
		break;
	}
	return;
}

