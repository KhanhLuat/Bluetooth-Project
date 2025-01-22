/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: FR81S								*/
/* Date				: 16/08/18							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Sakata						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		:									*/
/* date				:									*/
/********************************************************/
/********************************************************/
/* file DTO459_VDV_Sw_ctrl.c							*/
/* SW制御												*/
/********************************************************/

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include "string.h"
#include "system.h"
#include "Timer.h"	
#include "timer_ctrl.h"
#include "System_control_TASK.h"							/* ｼｽﾃﾑ状態制御 */
#include "DTO459_VDV_Sw_ctrl.h"
#include "SHRVSTD_ADC_Drv.h"
#include "System_control_TASK.h"
#include "SSFTSTD_Macro.h"
#include "DTO459_VDV_Inspection_ctrl.h"

#define SW_LOCAL	static


/********************************************************/
/*	マクロ定義											*/
/********************************************************/
#define CA_COUNT		(5)					/**< チャタリング吸収カウント		*/
#define ADC_VALUE_MAX	(0x3FF)				/**< AD変換最大値					*/
#define SW_REC_LONG_PRESS_TIME	(3000)		/**< 長押し検出時間(ms)				*/


/* 
 ** SWモード判定用定数
 */ 
typedef enum e_sw_mode_judge_status
{
	SW_MODE_JUDGE_UNKNOWN = 0,				/**< SWモード:不明(初期値)		*/
	SW_MODE_JUDGE_OFF,						/**< SWモード:OFF				*/
	SW_MODE_JUDGE_AUTO,						/**< SWモード:オートREC			*/
	SW_MODE_JUDGE_MANUAL,					/**< SWモード:マニュアルREC		*/
	SW_MODE_JUDGE_UNCONNECT,				/**< SWモード:未接続			*/
	SW_MODE_JUDGE_RECBTN,					/**< SWモード:RECボタン押下		*/
	SW_MODE_JUDGE_INVALID,					/**< SWモード:無効				*/
	SW_MODE_JUDGE_NUM						/**< SWモード数					*/
} E_SW_MODE_JUDGE_STATUS;


/*SWモード判定用構造体*/
typedef struct t_sw_mode_judge_tbl 
{
	E_SW_MODE_JUDGE_STATUS	mode;				/* モード判定状態 */
	UI_16					minVale;			/* 最小値 */
	UI_16					maxVale;			/* 最大値 */
} T_SW_MODE_JUDGE_TBL;


#define MODE_JUDGE_TBLDATA_NUM		(5)						/**< SWモード判定用テーブルデータ数		*/
#define SW_NOT_NOTIFY_EVENT			(SYS_EVENT_NUM)			/**< SW通知イベントなし					*/
#define SW_NOT_NOTIFY_EVENT_PARAM	(SYS_EVENT_PARAM_NUM)	/**< SW通知イベントパラメタなし			*/
#define SW_REC_BTN_CHANGED			(SW_MODE_NUM)			/**< RECボタン変化						*/

#define SW_INSPECTION_CHECK_CYCLE	(200)		/**< SW検査周期（200ms）			*/

/* SW検査ステップ */
typedef enum e_sw_inspection_step {
	SW_INSPECTION_STEP_1 = 0,					/* 検査1						*/
	SW_INSPECTION_STEP_2,						/* 検査2						*/
	SW_INSPECTION_STEP_3,						/* 検査3						*/
	SW_INSPECTION_STEP_4,						/* 検査4						*/
	SW_INSPECTION_STEP_5,						/* 検査5						*/
	SW_INSPECTION_STEP_NUM,						/* 検査数						*/
	SW_INSPECTION_STEP_INVALID					/* 無効値						*/
} E_SW_INSPECTION_STEP;

/* SW検査情報 */
typedef struct t_sw_inspection_info {
	UI_8					req_flg;					/* 検査要求フラグ				*/
	E_SW_INSPECTION_STEP	step;						/* 検査ステップ					*/
	UI_32					start_time;					/* 検査開始時間					*/
} T_SW_INSPECTION_INFO;

/* SW検査設定値 */
typedef struct t_sw_inspection_config {
	E_SW_INSPECTION_STEP	step;						/* 検査ステップ					*/
	UI_32					checktime;					/* 検査時間（開始からの時間）	*/
	UI_16					adMinVale;					/* AD値 最小値					*/
	UI_16					adMaxVale;					/* AD値 最大値					*/
} T_SW_INSPECTION_CONFIG;

/* SW検査設定テーブル */
SW_LOCAL const T_SW_INSPECTION_CONFIG	C_sw_inspection_config_tbl[SW_INSPECTION_STEP_NUM] = {
	{	SW_INSPECTION_STEP_1,	(UI_32)(SW_INSPECTION_CHECK_CYCLE * 1),	983	,	1023	},	/* 検査1	4.8-5.0[v]			*/
	{	SW_INSPECTION_STEP_2,	(UI_32)(SW_INSPECTION_CHECK_CYCLE * 2),	594	,	 634	},	/* 検査2	2.9-3.1[v]			*/
	{	SW_INSPECTION_STEP_3,	(UI_32)(SW_INSPECTION_CHECK_CYCLE * 3),	389	,	 429	},	/* 検査3	1.9-2.1[v]			*/
	{	SW_INSPECTION_STEP_4,	(UI_32)(SW_INSPECTION_CHECK_CYCLE * 4),	185	,	 225	},	/* 検査4	0.9-1.1[v]			*/
	{	SW_INSPECTION_STEP_5,	(UI_32)(SW_INSPECTION_CHECK_CYCLE * 5),	  0	,	  40	},	/* 検査5	0.0-0.2[v]			*/
};

/*SWモード判定用テーブル*/
SW_LOCAL const T_SW_MODE_JUDGE_TBL	C_sw_mode_judge_tbl[MODE_JUDGE_TBLDATA_NUM] = {	/* SWモード判定テーブル */
	{	SW_MODE_JUDGE_RECBTN	,	  0	,	  40	},	/* SWモード:RECボタン押下 */
	{	SW_MODE_JUDGE_OFF		,	368	,	 450	},	/* SWモード:OFF */
	{	SW_MODE_JUDGE_MANUAL	,	573	,	 655	},	/* SWモード:マニュアルREC */
	{	SW_MODE_JUDGE_AUTO		,	778	,	 860	},	/* SWモード:オートREC */
	{	SW_MODE_JUDGE_UNCONNECT	,	982	,	1023	},	/* SWモード:未接続 */
};
/* AD変換値は10bit(0-1023)の範囲で取得される */
/* これを０－５Vに割当てる */
/* 0.1v は 1024 / 50 = 20.48                   -0.2v     基準v   +0.2v */
/* SWモード:RECボタン押下		0.0V ＋ 0.2v ( XXX		   0	  40.96 ) */
/* SWモード:未定義				1.0V ± 0.2v ( 163.84	 204.8	 245.76 ) */
/* SWモード:マニュアルREC		2.0V ± 0.2v ( 368.64	 409.6	 450.56 ) */
/* SWモード:オートREC			3.0V ± 0.2v ( 573.44	 614.4	 655.36 ) */
/* SWモード:OFF					4.0V ± 0.2v ( 778.24	 819.2	 860.16 ) */
/* SWモード:未接続				5.0V － 0.2v ( 982.04	1023.0	 XXX	) */
		

/**
 *	通知イベント
 */
typedef struct t_sw_mode_notify_event {
	E_SYS_CTRL_EVENT			event;			/* イベント */
	E_SYS_CTRL_EVENT_PARAM		param;			/* パラメタ */
} T_SW_MODE_NOTIFY_EVENT;

/*イベント通知用テーブル */
SW_LOCAL const T_SW_MODE_NOTIFY_EVENT	C_sw_mode_notify_event_tbl[SW_MODE_JUDGE_NUM] = {
	{	SW_NOT_NOTIFY_EVENT			,	SW_NOT_NOTIFY_EVENT_PARAM			},	/* SW_MODE_UNKNOWN, */
	{	SYS_EVENT_CHANGE_SW_MODE	,	SYS_EVENT_PARAM_SW_MODE_OFF			},	/* SW_MODE_OFF, */
	{	SYS_EVENT_CHANGE_SW_MODE	,	SYS_EVENT_PARAM_SW_MODE_AUTO		},	/* SW_MODE_AUTO, */
	{	SYS_EVENT_CHANGE_SW_MODE	,	SYS_EVENT_PARAM_SW_MODE_MANUAL		},	/* SW_MODE_MANUAL, */
	{	SYS_EVENT_MODULE_STAT		,	SYS_EVENT_PARAM_MODSTAT_ATT_NG_SW	},	/* SW_MODE_UNCONNECT, */
	{	SW_NOT_NOTIFY_EVENT			,	SW_NOT_NOTIFY_EVENT_PARAM			},	/* SW_MODE_JUDGE_RECBTN, */
	{	SW_NOT_NOTIFY_EVENT			,	SW_NOT_NOTIFY_EVENT_PARAM			},	/* SW_MODE_UNKNOWN, */
};

/* SWモード判定用を外部公開用モードに変換数テーブル */
SW_LOCAL const E_SW_MODE_STATUS C_sw_chg_mode_tbl[SW_MODE_JUDGE_NUM] = 
{
	SW_MODE_UNKNOWN,						/* SW_MODE_JUDGE_UNKNOWN, */
	SW_MODE_OFF,	  						/* SW_MODE_JUDGE_OFF, */
	SW_MODE_AUTO,							/* SW_MODE_JUDGE_AUTO, */
	SW_MODE_MANUAL, 						/* SW_MODE_JUDGE_MANUAL, */
	SW_MODE_UNCONNECT,						/* SW_MODE_JUDGE_UNCONNECT, */
	SW_REC_BTN_CHANGED,						/* SW_MODE_JUDGE_RECBTN, */
	SW_MODE_INVALID							/* SW_MODE_JUDGE_INVALID, */
};

/********************************************************/
/*	内部変数定義										*/
/********************************************************/
SW_LOCAL B_8						S_firstevent_flag;						/**< 初期イベント通知フラグ	*/
SW_LOCAL UI_8						S_chatt_count;							/**< チャタリングカウンタ*/
SW_LOCAL E_SW_MODE_JUDGE_STATUS		S_swmode_hist_value[CA_COUNT];			/**< AD変換値の履歴		*/
SW_LOCAL E_SW_REC_BTN_STATUS		S_sw_rec_btn_status;					/**< RECボタン状態		*/
SW_LOCAL E_SW_MODE_STATUS			S_sw_mode_status;						/**< SWモード状態		*/
SW_LOCAL UI_16						S_sw_btn_on_time;						/**< RECボタン押し時間	*/
SW_LOCAL E_SYS_CTRL_STATUS			S_pre_sys_stat;							/**< 前回システム状態	*/
SW_LOCAL UI_8						S_sw_btn_onoff_cnt;						/**< ボタンONOFF変化回数*/
SW_LOCAL T_SW_INSPECTION_INFO		S_sw_inspection_info;					/**< SW検査情報			*/


/********************************************************/
/*	外部関数											*/
/********************************************************/
SW_LOCAL B_8 sw_control_judge_mode( void );
SW_LOCAL void sw_control_setSwitchMode( E_SW_MODE_STATUS swmodestatus);
SW_LOCAL void sw_control_notifyEvent( E_SW_MODE_STATUS swmodestatus );
SW_LOCAL E_SW_MODE_JUDGE_STATUS sw_control_cnvSwMode( UI_16 value );
#ifdef _VDV_APP
SW_LOCAL void sw_ctrl_initInspectionInfo(void);
SW_LOCAL void sw_control_inspection(void);
#endif	/* _VDV_APP */


/********************************************************/
/* 関数名称		: Sw_control_Init						*/
/* 機能の説明	: Sw制御初期化							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Sw_control_Init(void)
{
	UI_8 i;
	S_firstevent_flag = N_TRUE;
	S_chatt_count = 0;
	S_sw_btn_on_time = 0;
	S_sw_btn_onoff_cnt = 0;
	S_sw_rec_btn_status = SYS_REC_BTN_UNKNOWN;
	S_sw_mode_status	= SW_MODE_UNKNOWN;
	S_pre_sys_stat		= SYS_OPERATION_NUM;
	for(i = 0 ; i < CA_COUNT ; i++)
	{
		S_swmode_hist_value[i] = SW_MODE_JUDGE_UNKNOWN;
	}

#ifdef _VDV_APP
	sw_ctrl_initInspectionInfo();
#endif	/* _VDV_APP */
}

/********************************************************/
/* 関数名称		: Sw_control_Main						*/
/* 機能の説明	: SW制御メイン							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Sw_control_Main(void)
{
	B_8 bchange;
	E_SW_MODE_STATUS	swmodestatus;
	E_SYS_CTRL_STATUS	sys_stat;
	
	/**	システム状態取得	*/
	sys_stat =  System_control_GetStatus();
	switch ( sys_stat )
	{
		case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON（状態判定）	*/
		case (SYS_OPERATION_NORMAL_OFF):			/*通常:OFFモード		*/
		case (SYS_OPERATION_NORMAL_AUTO):			/*通常:オートREC		*/
		case (SYS_OPERATION_NORMAL_MANUAL):			/*通常:マニュアルREC	*/
		case (SYS_OPERATION_SIMULATION):			/*シミュレーション		*/
		case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF移行			*/
		case (SYS_OPERATION_INSPECTION):			/*検査モード			*/
		case (SYS_OPERATION_GNSS_COMM):				/*GNSSモジュール通信モード*/
			/* 処理継続 */
		break;
		default:
			/* スイッチ判定する必要ないので抜ける */
			return;
	}
	if( (S_pre_sys_stat == SYS_OPERATION_PRE_ACC_OFF) && (sys_stat == SYS_OPERATION_ACC_ON_UNKNOWN_STAT) )
	{
		/* ACC OFF -> ACC ONなら内部初期化 */
		Sw_control_Init();
	}
	S_pre_sys_stat = sys_stat;
	
	if (sys_stat == SYS_OPERATION_INSPECTION) {
#ifdef _VDV_APP
		sw_control_inspection();
#endif /* _VDV_APP */
	}
	else {
		/**	スイッチモード確定したかチェックする	*/
		bchange = sw_control_judge_mode();
		if(bchange == N_TRUE)
		{
			/**	確定した	*/
			/* 外部公開用モードに変換 */
			swmodestatus = C_sw_chg_mode_tbl[S_swmode_hist_value[0]];
			/**	スイッチモード設定	*/
			sw_control_setSwitchMode( swmodestatus );
		}
	}
}


/********************************************************/
/* 関数名称		: Sw_control_GetSwMode					*/
/* 機能の説明	: SWモード状態取得						*/
/* 引数			: void									*/
/* 戻り値		: SWモード状態 E_SW_MODE_STATUS			*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
E_SW_MODE_STATUS Sw_control_GetSwMode(void)
{
	return S_sw_mode_status;
}


/********************************************************/
/* 関数名称		: Sw_control_GetRecButtonStatus			*/
/* 機能の説明	: RECボタン状態取得						*/
/* 引数			: void									*/
/* 戻り値		: ボタン状態 E_SW_REC_BTN_STATUS		*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
E_SW_REC_BTN_STATUS Sw_control_GetRecButtonStatus(void)
{
	return S_sw_rec_btn_status;
}

/********************************************************/
/* 関数名称		: Sw_control_GetRecButtonChangeCount	*/
/* 機能の説明	: RECボタン状態OnOff変化回数取得		*/
/* 引数			: void									*/
/* 戻り値		: 変化回数								*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: RECボタン状態(ON/OFF)変化回数			*/
/* 				: 長押しでは変化しない					*/
/********************************************************/
UI_8 Sw_control_GetRecButtonOnOffChangeCount(void)
{
	return S_sw_btn_onoff_cnt;
}

#ifdef _VDV_APP
/********************************************************/
/* 関数名称		: Sw_control_ReqInspection				*/
/* 機能の説明	: SW検査要求							*/
/* 引数			: void									*/
/* 戻り値		: 正常	:N_OK							*/
/*				  異常	:N_NG							*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 本関数が正常終了した場合、検査制御モジュールに対して	*/
/*				   Insp_notify_inspection_resultにより検査結果を通知する*/
/********************************************************/
SI_8 Sw_control_ReqInspection(void)
{
	E_SYS_CTRL_STATUS	sysStat;

	/* システム状態取得	*/
	sysStat = System_control_GetStatus();
	if (sysStat != SYS_OPERATION_INSPECTION) {
		/* 検査状態ではない	*/
		return N_NG;
	}

	if (S_sw_inspection_info.req_flg == N_ON) {
		/* 検査実行中	*/
		return N_NG;
	}

	/* 検査情報を設定	*/
	sw_ctrl_initInspectionInfo();
	S_sw_inspection_info.start_time = Timer_ctrl_GetTime1ms();
	S_sw_inspection_info.step = SW_INSPECTION_STEP_1;
	S_sw_inspection_info.req_flg = N_ON;

	return N_OK;
}
#endif /* _VDV_APP */

/************************************************************/
/* 関数名称		: sw_control_judge_mode						*/
/* 機能の説明	: スイッチモード判定						*/
/* 引数			: void										*/
/* 戻り値		: 結果（確定true / 未確定false)				*/
/* 作成者		: (OFT)Sakata								*/
/* 作成日		: 16/08/18									*/
/* 備考			: 											*/
/************************************************************/
SW_LOCAL B_8 sw_control_judge_mode( void )
{
	UI_8 i;
	SI_8 result;
	T_ADCDrv_DataLevel datalevel;
	
	/**	<ol>	*/	
	/**	<li>ポート（ADC）データ取得	*/
	result = ADCDrv_GetDataLevel(AD_CH_OPERATION_SW, &datalevel);
	if(result < 0){
		/**	<li>チャタリング吸収（SW）の変数リセット	*/
		S_chatt_count	= 0;
		return N_FALSE;
	}
	/* AD変換値をSWモード値に変換 */
	S_swmode_hist_value[0] = sw_control_cnvSwMode(datalevel);
	if (S_chatt_count < CA_COUNT)
	{
		/**	<li>保存データがCA_COUNT回分以上たまっていない場合はカウント加算	*/
		S_chatt_count++;
	}
	if (S_chatt_count >= CA_COUNT)
	{
		/**	<li>保存データがCA_COUNT回分以上たまったらSWモード判定*/
		for(i = 0 ; i < (CA_COUNT - 1) ; i++)
		{
			if(S_swmode_hist_value[i] != S_swmode_hist_value[i + 1])
			{
				/* 違うデータが混ざっている */
				break;
			}
		}
		if(i >= (CA_COUNT - 1))
		{
			/* 最後までループした＝＝すべて同じ値 -> 状態確定 */
			/**	<li>チャタリング吸収（SW）の変数リセット	*/
			S_chatt_count	= 0;
			return N_TRUE;
		}
	}
	/* 取得値を後ろへずらす */
	for(i = S_chatt_count ; i > 0 ; i--)
	{
		if (i < CA_COUNT)
		{
			S_swmode_hist_value[i] = S_swmode_hist_value[i - 1];
		}
	}

	/**	</ol>	*/
	return N_FALSE;
}


/************************************************************/
/* 関数名称		: sw_control_setSwitchMode					*/
/* 機能の説明	: スイッチモード設定						*/
/* 引数			: スイッチモード　E_SW_MODE_STATUS			*/
/* 戻り値		: void										*/
/* 作成者		: (OFT)Sakata								*/
/* 作成日		: 16/08/18									*/
/* 備考			: 											*/
/************************************************************/
SW_LOCAL void sw_control_setSwitchMode( E_SW_MODE_STATUS swmodestatus)
{
	SI_8 result;
	
	if(swmodestatus == SW_REC_BTN_CHANGED)
	{
		/**< ボタンが押されているためSWモードは不明なので保持値を設定	*/
		swmodestatus = S_sw_mode_status;
		/* RECボタンON検出 */
		if(S_sw_rec_btn_status == SYS_REC_BTN_OFF)
		{
			/* RECボタン OFF -> ON検出 */
			S_sw_rec_btn_status = SYS_REC_BTN_ON_SHORTPRESS;
			/* 長押し検出用開始時間保持 */
			S_sw_btn_on_time = Timer_ctrl_GetTime10ms();
			/* ON/OFF変化検出カウンタ加算 */
			S_sw_btn_onoff_cnt++;
		}
		else if(S_sw_rec_btn_status == SYS_REC_BTN_ON_SHORTPRESS)
		{
			/* 単押し状態 */
			result = Timer_ctrl_TimeCompare10ms( S_sw_btn_on_time, SW_REC_LONG_PRESS_TIME/TIME_MAIN );
			if ( result == N_OK )
			{
				/* 指定時間分経過した　→　長押し検出 */
				/* RECボタン 長押し検出 */
				S_sw_rec_btn_status = SYS_REC_BTN_ON_LONGPRESS;
				/* 長押し検出用開始時間クリア */
				S_sw_btn_on_time = 0;
			}
		}
		else
		{
			/* nop */
		}
	}
	else
	{
		if(S_sw_rec_btn_status != SYS_REC_BTN_OFF)
		{
			/* RECボタン ON -> OFF、または未確定 ->OFF検出 */
			/* ON/OFF変化検出カウンタ加算 */
			S_sw_btn_onoff_cnt++;
		}
		/* ボタン押下以外ならRECボタン OFF検出とする */
		S_sw_rec_btn_status = SYS_REC_BTN_OFF;
		/* 長押し検出用開始時間クリア */
		S_sw_btn_on_time = 0;
	}
	if( swmodestatus != S_sw_mode_status )
	{
		/* システム制御に通知 */
		sw_control_notifyEvent(swmodestatus);
		/* 内部保持モード更新 */
		S_sw_mode_status = swmodestatus;
	}
}

/************************************************************/
/* 関数名称		: sw_control_cnvSwMode						*/
/* 機能の説明	: AD変換値にからSWモードへ変換				*/
/* 引数			: AD変換値									*/
/* 戻り値		: SWモード判定値							*/
/* 作成者		: (OFT)Sakata								*/
/* 作成日		: 16/08/18									*/
/* 備考			: 											*/
/************************************************************/
SW_LOCAL E_SW_MODE_JUDGE_STATUS sw_control_cnvSwMode( UI_16 value )
{
	UI_8 i;
	if(value > ADC_VALUE_MAX)
	{
		return SW_MODE_JUDGE_INVALID;
	}

	for(i = 0 ; i < MODE_JUDGE_TBLDATA_NUM ; i++)
	{
		if( value < C_sw_mode_judge_tbl[i].minVale)
		{
			/*範囲の最小値より小さい*/
			break;
		}
		if( value <= C_sw_mode_judge_tbl[i].maxVale )
		{
			/*範囲の最大値以下*/
			/*範囲内のデータが見つかった*/
			return C_sw_mode_judge_tbl[i].mode;
		}
	}
	return SW_MODE_JUDGE_INVALID;
}

/************************************************************/
/* 関数名称		: sw_control_notifyEvent					*/
/* 機能の説明	: システム制御へ状態通知					*/
/* 引数			: SWモード E_SW_MODE_STATUS					*/
/* 戻り値		: void										*/
/* 作成者		: (OFT)Sakata								*/
/* 作成日		: 16/08/18									*/
/* 備考			: 											*/
/************************************************************/
SW_LOCAL void sw_control_notifyEvent( E_SW_MODE_STATUS swmodestatus )
{
	E_SYS_CTRL_EVENT event;
	E_SYS_CTRL_EVENT_PARAM param;
	
	event = C_sw_mode_notify_event_tbl[swmodestatus].event;
	param = C_sw_mode_notify_event_tbl[swmodestatus].param;
	if(event == SW_NOT_NOTIFY_EVENT)
	{
		/* イベント無し時は抜ける */
		return;
	}
	/* 初回SWモード変更イベント時はSWモジュール正常通知を行う */
	if(S_firstevent_flag == N_TRUE)
	{
		S_firstevent_flag = N_FALSE;
		if(event == SYS_EVENT_CHANGE_SW_MODE)
		{
			/* システム制御へイベント通知 */
			/* SW制御モジュール:状態正常 */
			(void)System_control_NotifyEvent(SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_OK_SW);
		}
	}
	/* システム制御へイベント通知 */
	(void)System_control_NotifyEvent(event, param);
}

#ifdef _VDV_APP
/************************************************************/
/* 関数名称		: sw_ctrl_initInspectionInfo				*/
/* 機能の説明	: SW検査情報初期化							*/
/* 引数			: void										*/
/* 戻り値		: void										*/
/* 作成者		: (OFT)Toguchi								*/
/* 作成日		: 17/01/20									*/
/* 備考			: 											*/
/************************************************************/
SW_LOCAL void sw_ctrl_initInspectionInfo(void)
{
	(void)memset(&S_sw_inspection_info, 0, sizeof(S_sw_inspection_info));
	S_sw_inspection_info.req_flg = N_OFF;
	S_sw_inspection_info.step = SW_INSPECTION_STEP_INVALID;

	return;
}

/************************************************************/
/* 関数名称		: sw_control_inspection						*/
/* 機能の説明	: SW検査									*/
/* 引数			: void										*/
/* 戻り値		: void										*/
/* 作成者		: (OFT)Toguchi								*/
/* 作成日		: 17/01/20									*/
/* 備考			: 											*/
/************************************************************/
SW_LOCAL void sw_control_inspection(void)
{
	UI_32				timeout;
	SI_8				adcRes;
	T_ADCDrv_DataLevel	adcLevel;
	E_INSP_INSPECTION_RESULT	inspectRes;
	
	if ((S_sw_inspection_info.req_flg == N_OFF) ||
		(S_sw_inspection_info.step >= SW_INSPECTION_STEP_NUM)) {
		return;
	}

	inspectRes = INSP_INSPECTION_RESULT_INVALID;
	timeout = C_sw_inspection_config_tbl[S_sw_inspection_info.step].checktime;

	if (Timer_ctrl_TimeCompare1ms( S_sw_inspection_info.start_time, timeout ) == N_OK) {

		/* SWのAD値を取得	*/
		adcRes = ADCDrv_GetDataLevel(AD_CH_OPERATION_SW, &adcLevel);
		if ((adcRes == D_OK) &&
			(C_sw_inspection_config_tbl[S_sw_inspection_info.step].adMinVale <= adcLevel) &&
			(C_sw_inspection_config_tbl[S_sw_inspection_info.step].adMaxVale >= adcLevel)) {
			/* AD値チェック結果正常 */
			if (S_sw_inspection_info.step == SW_INSPECTION_STEP_5) {
				/* SW検査終了	*/
				inspectRes = INSP_INSPECTION_RESULT_OK;
			}
			else {
				S_sw_inspection_info.step += 1;
			}
		}
		else {
			/* AD値チェック結果異常 */
			inspectRes = INSP_INSPECTION_RESULT_NG;
		}

		if (inspectRes != INSP_INSPECTION_RESULT_INVALID) {
			/* 検査制御モジュールに結果を通知する	*/
			Insp_notify_inspection_result(INSP_INSPECTION_TYPE_SW, NULL, inspectRes);

			/* 検査情報をクリア	*/
			sw_ctrl_initInspectionInfo();
		}
	}

	return;
}
#endif	/* _VDV_APP */
