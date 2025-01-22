/****************************************************************/
/* file self_holding_ctrl.h										*/
/* 自己保持制御													*/
/* 																*/
/* 自己保持制御をする。											*/
/* <ul>															*/
/* <li>ACCがONになった場合、自己保持ON出力しACC ONイベントを	*/
/* 		発行する。												*/
/* <li>ACCがOFFとなった場合、ACC OFF判定を開始しACC OFFイベント	*/
/* 		を発行する。											*/
/* <li>ACCがOFFになった場合（5sec以上ACCがOFFになった場合）、	*/
/* 		ACC OFF判定カウント5sec経過イベントを発行する。			*/
/* <li>システム状態が終了モードの場合、自己保持OFFする。		*/
/* </ul>														*/
/* 																*/
/****************************************************************/
/* author      Y.Sorimachi										*/
/* date        2013/--/--										*/
/****************************************************************/

#include "system.h"
#include "Timer.h"	
#include "SSFTSTD_FR81_Port_Drv.h"
#include "timer_ctrl.h"
#include "System_control_TASK.h"
#include "chatt_absorber.h"
#include "self_holding_ctrl.h"

/********************************************************/
/*	内部マクロ定義										*/
/********************************************************/
#define SELFHOLD_LOCAL	static
#define ACC_NOT_NOTIFY_EVENT			(SYS_EVENT_NUM)		/**< ACC通知イベントなし	*/
#define SELF_OFF_DELAY_TIME				(10)				/**< 10ms タイマカウント	*/
/********************************************************/
/*	内部定数定義										*/
/********************************************************/
/**
 *	ACC判定状態
 */
typedef enum e_acc_state {
	ACC_STATE_OFF	= 0,				/**< ACC状態:OFF					*/
	ACC_STATE_ON,						/**< ACC状態:ON						*/
	ACC_STATE_JUDGMENT_ON_TO_OFF,		/**< ACC状態:ONからOFFへの判定中	*/
	ACC_STATE_NUM
} E_ACC_STATE;

/**
 *	通知イベント
 */
SELFHOLD_LOCAL const UI_8 C_acc_notify_event[ACC_STATE_NUM] = 
{
	ACC_NOT_NOTIFY_EVENT,					/**< ACC_STATE_OFF					*/
	SYS_EVENT_ACC_ON,						/**< ACC_STATE_ON					*/
	SYS_EVENT_ACC_OFF						/**< ACC_STATE_JUDGMENT_ON_TO_OFF	*/
};

/********************************************************/
/*	内部変数定義										*/
/********************************************************/
SELFHOLD_LOCAL UI_8 		S_acc_change_state;		/**< ACC変化状態		*/
SELFHOLD_LOCAL E_ACC_STATE 	S_acc_state;			/**< ACC状態			*/
SELFHOLD_LOCAL UI_8			S_acc_val;				/**< ACCの値			*/
SELFHOLD_LOCAL E_SYS_CTRL_STATUS	S_acc_prev_system_state;/**< 前のシステム状態	*/
SELFHOLD_LOCAL UI_8			S_acc_noti_eve_state;	/**< 通知イベント状態	*/
SELFHOLD_LOCAL UI_16		S_self_off_delay_time;	/**< 自己保持OFF遅延時間	*/

/********************************************************/
/*	内部関数プロトタイプ宣言							*/
/********************************************************/
SELFHOLD_LOCAL void S_acc_set_state( const UI_8 state );
SELFHOLD_LOCAL void S_acc_self_cb( UI_8 ca_acc );

/********************************************************/
/*	外部関数											*/
/********************************************************/

/** 
 * /// 初期化処理
 * 
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		<ul>
 *			<li>自己保持制御で使用されるRAMを初期化する。
 *			<li>チャタリング吸収モジュールにACCをコールバックされるよう要求する。
 *			</ul>
 */ 
void Self_holding_ctrl_Init( void )
{
	/**	<ol>	*/
	/**	<li>RAM 初期化	*/
	S_acc_change_state 		= N_OFF;
	S_acc_prev_system_state	= SYS_OPERATION_WAIT_ACC_ON;
	S_acc_noti_eve_state	= ACC_NOT_NOTIFY_EVENT;
	S_acc_state				= ACC_STATE_OFF;
	S_acc_val				= N_LOW;
	S_self_off_delay_time	= 0;
	/**	<li>ACCがコールバックされるよう要求する	*/
	(void)Chatt_absorber_ReqPortData(S_acc_self_cb, CHATT_PORT_ACC, CHATT_ABS_MOD_SHC);
	/**	</ol>	*/
	return;
}

/** 
 * メイン処理
 * 
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		<ul>
 *			<li>ACCがONになった場合、自己保持ON出力しACC ONイベントを発行する。
 *			<li>ACCがOFFとなった場合、ACC OFF判定を開始しACC OFFイベントを発行する。
 *			<li>ACCがOFFになった場合（5sec以上ACCがOFFになった場合）、ACC OFF判定カウント5sec経過イベントを発行する。
 *			<li>システム状態が終了モードの場合、自己保持OFFする。
 *			</ul>
 */ 
void Self_holding_ctrl_Main( void )
{
	E_SYS_CTRL_STATUS system_state;
	/**	<ol>	*/
	/**	<li>ACC変化状態がN_ONのとき実行	*/	
	if ( S_acc_change_state == N_ON )
	{
		switch ( S_acc_state )
		{
		case ( ACC_STATE_OFF ):		/* 現在ACC OFF */
		/**	<li>ACC ON判定	*/
			if ( S_acc_val == IO_PI_LEV_ACTIVE_ACC_STAT )
			{
				S_acc_set_state(ACC_STATE_ON);
				/**	<li>自己保持ON出力	*/
				(void)PortDrv_SetDataBit(IO_PO_GR_5V_HOLD, IO_PO_BIT_5V_HOLD, IO_PO_LEV_ACTIVE_5V_HOLD);
				/**	<li>ポート（3.3V_EN）ON出力	*/
				(void)PortDrv_SetDataBit(IO_PO_GR_33V_EN, IO_PO_BIT_33V_EN, IO_PO_LEV_ACTIVE_33V_EN);
				/**	<li>ポート（ADC5V_EN）ON出力	*/
				(void)PortDrv_SetDataBit(IO_PO_GR_ADC5V_EN, IO_PO_BIT_ADC5V_EN, IO_PO_LEV_ACTIVE_ADC5V_EN);
			}
			break;
		case ( ACC_STATE_ON ):		/* 現在ACC ON */
		/**	<li>ACC OFF判定	*/
			if ( S_acc_val == IO_PI_LEV_NONACT_ACC_STAT )
			{
				/**	<li>ACC ONからOFFへ判定開始	*/
				S_acc_set_state(ACC_STATE_JUDGMENT_ON_TO_OFF);
			}
			break;
		case ( ACC_STATE_JUDGMENT_ON_TO_OFF ):		/* 現在ACC ON->OFFへの判定中 */
		/**	<li>ACC OFF判定中にACC変化があった場合	*/
			if ( S_acc_val == IO_PI_LEV_ACTIVE_ACC_STAT )
			{
				S_acc_set_state(ACC_STATE_ON);
			}
			break;
		default:
			/**	<li>ありえない処理	*/
			break;
		}
		/**	<li>ACC変化状態をN_OFFにする	*/
		S_acc_change_state = N_OFF;
	}

	/**	<li>システム状態を取得	*/
	system_state = System_control_GetStatus();

	/**	<li>ACC OFF判定中になったとき実行	*/
	if ( S_acc_state == ACC_STATE_JUDGMENT_ON_TO_OFF )
	{
		/**	<li>ACC ONからOFFへ判定中	*/
		if(system_state == SYS_OPERATION_ACC_OFF)
		{
			/* システム状態がACC＿OFFならACC＿OFFとする */
			S_acc_set_state(ACC_STATE_OFF);
		}
	}
	
	/**	<li>通知イベントが発生したとき、システム状態制御に通知する	*/
	if ( S_acc_noti_eve_state != ACC_NOT_NOTIFY_EVENT )
	{
		/**	<li>システム状態制御にイベント通知する	*/
		(void)System_control_NotifyEvent((E_SYS_CTRL_EVENT)S_acc_noti_eve_state,SYS_EVENT_PARAM_NO);
		/**	<li>イベント通知状態をイベントなしにする	*/
		S_acc_noti_eve_state = ACC_NOT_NOTIFY_EVENT;
	}

	if ( S_acc_prev_system_state != system_state )
	{
		if (system_state == SYS_OPERATION_ACC_OFF)
		{
			if(S_self_off_delay_time == 0)
			{
				/**	自己保持OFF遅延時間を未保持の場合は取得	*/
				S_self_off_delay_time = Timer_ctrl_GetTime10ms();
			}
			else
			{
				/**	自己保持OFF遅延時間を保持の場合は経過時間を経過したか確認	*/
				if (Timer_ctrl_TimeCompare10ms(S_self_off_delay_time, SELF_OFF_DELAY_TIME/TIME_MAIN) == N_OK)
				{
					/**	時間経過	*/
					/**	<li>ポート（3.3V_EN）OFF出力	*/
					(void)PortDrv_SetDataBit(IO_PO_GR_33V_EN, IO_PO_BIT_33V_EN, IO_PO_LEV_NONACT_33V_EN);
					/**	<li>ポート（ADC5V_EN）OFF出力	*/
					(void)PortDrv_SetDataBit(IO_PO_GR_ADC5V_EN, IO_PO_BIT_ADC5V_EN, IO_PO_LEV_NONACT_ADC5V_EN);
					/**	<li>自己保持OFF出力	*/
					(void)PortDrv_SetDataBit(IO_PO_GR_5V_HOLD, IO_PO_BIT_5V_HOLD, IO_PO_LEV_NONACT_5V_HOLD);
					/**	<li>システム状態を保存	*/
					S_acc_prev_system_state = system_state;
				}
				else
				{
					/**	時間未経過	*/
				}
			}
		}
		else
		{
			S_self_off_delay_time = 0;
		}
	}

	/**	</ol>	*/
	return;
}

/********************************************************/
/*	内部関数											*/
/********************************************************/
/** 
 * /// 状態設定処理
 * 
 * param	state	[in]	ACC状態
 * 
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		ACC状態と通知イベント状態を更新する。
 */ 
SELFHOLD_LOCAL void S_acc_set_state( const UI_8 state )
{
	/**	<ol>	*/
	/**	<li>ACC状態を設定する	*/	
	S_acc_state = (E_ACC_STATE)state;
	/**	<li>通知イベント状態を設定する	*/	
	S_acc_noti_eve_state = C_acc_notify_event[state];
	/**	</ol>	*/	
	return;
}

/********************************************************/
/*	コールバック関数									*/
/********************************************************/
SELFHOLD_LOCAL void S_acc_self_cb( UI_8 ca_acc )
{
	/**	<ol>	*/
	/**	<li>ACC変化状態をN_ONにする	*/	
	S_acc_change_state = N_ON;
	/**	<li>ACCの値を設定する	*/	
	S_acc_val = ca_acc;
	/**	</ol>	*/	
	return;
}
