/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: FR81S								*/
/* Date				: 17/01/20							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Toguchi						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		: 									*/
/* date				: 									*/
/********************************************************/
/********************************************************/
/* file DTO459_VDV_Inspection_ctrl.c					*/
/* SD制御												*/
/********************************************************/

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include <string.h>
#include "system.h"
#include "SSFTSTD_Macro.h"
#include "System_control_TASK.h"
#include "timer_ctrl.h"
#include "DTO459_VDV_Sw_ctrl.h"
#include "DTO459_VDV_Bt_ctrl.h"
#include "DTO459_VDV_Gnss_ctrl.h"
#include "DTO459_VDV_Can_ctrl.h"
#include "DTO459_VDV_Sd_ctrl.h"
#include "DTO459_VDV_Eeprom_ctrl.h"
#include "DTO459_VDV_Advance_ctrl.h"
#include "DTO459_VDV_Common.h"
#include "DTO459_VDV_Inspection_ctrl.h"
#include "dbglog.h"

/********************************************************/
/*	定数定義											*/
/********************************************************/
/* 検査モジュール制御状態 */
typedef enum e_insp_stat {
	INSP_STAT_INIT = 0,								/* [S00]初期状態						*/
	INSP_STAT_WAIT_INSPECTION_START,				/* [S01]検査開始待ち					*/
	INSP_STAT_WAIT_INSPECTION_REQ,					/* [S02]検査要求待ち					*/
	INSP_STAT_INSPECTION,							/* [S03]検査中							*/
	INSP_STAT_INSPECTION_END,						/* [S04]検査終了						*/
	INSP_STAT_SHUTDOWN								/* [S05]モジュール終了					*/
} E_INSP_STAT;

/* 検査モジュール制御イベント */
typedef enum e_insp_event {
	INSP_EVENT_NONE = 0,
	INSP_EVENT_STARTUP,								/* [E01]モジュール起動					*/
	INSP_EVENT_INSPECTION_START,					/* [E02]検査開始						*/
	INSP_EVENT_INSPECTION_REQ,						/* [E03]検査要求						*/
	INSP_EVENT_INSPECTION_RES,						/* [E04]検査応答						*/
	INSP_EVENT_INSPECTION_END,						/* [E05]検査終了						*/
	INSP_EVENT_INSPECTION_PERIOD_PROC,				/* [E06]検査定期処理					*/
	INSP_EVENT_PRE_ACC_OFF,							/* [E07]ACC OFF移行						*/
	INSP_EVENT_SHUTDOWN								/* [S08]モジュール終了					*/
} E_INSP_EVENT;

#define INSP_GNSS_LAT					(356807870L)	/* 検査地点の緯度:北緯35.680787		*/
#define INSP_GNSS_LON					(1397660930L)	/* 検査地点の経度:東経139.766093	*/
#define INSP_GNSS_LAT_TOLERANCE			(2000L)			/* 緯度の許容誤差:+-0.0002(約20m)	*/
#define INSP_GNSS_LON_TOLERANCE			(2000L)			/* 経度の許容誤差:+-0.0002(約20m)	*/
#define INSP_GNSS_LAT_TOLERANCE_MIN		(INSP_GNSS_LAT - INSP_GNSS_LAT_TOLERANCE)	/* 許容緯度の最小値	*/
#define INSP_GNSS_LAT_TOLERANCE_MAX		(INSP_GNSS_LAT + INSP_GNSS_LAT_TOLERANCE)	/* 許容緯度の最大値	*/
#define INSP_GNSS_LON_TOLERANCE_MIN		(INSP_GNSS_LON - INSP_GNSS_LON_TOLERANCE)	/* 許容経度の最小値	*/
#define INSP_GNSS_LON_TOLERANCE_MAX		(INSP_GNSS_LON + INSP_GNSS_LON_TOLERANCE)	/* 許容経度の最大値	*/

#define INSP_BT_PAIRING_TIMEOUT			(10000)			/* BTペアリング検査タイムアウト(10000ms)	*/

#if defined _VDV_MARKX
/*MarkX*/
#define INSP_VDV_MODEL_INFO  (0x03)		/*機種情報*/
#define INSP_VDV_FIRM_INFO	 (0x10)		/*ファームウェア情報*/
#elif defined _VDV_RCF
/*RCF*/
#define INSP_VDV_MODEL_INFO  (0x04)		/*機種情報*/
#define INSP_VDV_FIRM_INFO	 (0x10)		/*ファームウェア情報*/
#elif defined _VDV_SUPRA
/*SUPRA*/
#define INSP_VDV_MODEL_INFO  (0x05)		/*機種情報*/
#define INSP_VDV_FIRM_INFO	 (0x10)		/*ファームウェア情報*/
#elif defined _VDV_LC
/*LC*/
#define INSP_VDV_MODEL_INFO  (0x06)		/*機種情報*/
#define INSP_VDV_FIRM_INFO	 (0x10)		/*ファームウェア情報*/
#elif defined _VDV_YARIS
/*YARIS*/
#define INSP_VDV_MODEL_INFO  (0x07)		/*機種情報*/
#define INSP_VDV_FIRM_INFO	 (0x10)		/*ファームウェア情報*/
#elif defined _VDV_86B
/*86B*/
#define INSP_VDV_MODEL_INFO  (0xA1)		/*機種情報*/
#define INSP_VDV_FIRM_INFO	 (0x10)		/*ファームウェア情報*/
#else
/*その他（Vitz,86含む）車種*/
#define INSP_VDV_MODEL_INFO  (0x00)		/*機種情報*/
#define INSP_VDV_FIRM_INFO	 (0x00)		/*ファームウェア情報*/
#endif

/* デバッグ用定義 */
#define INSP_LOCAL	static

/********************************************************/
/*	構造体定義											*/
/********************************************************/
/* 検査種別－コマンド情報	*/
typedef struct t_insp_type_cmd_info {
	E_INSP_INSPECTION_TYPE		type;
	UI_8						cmd;
} T_INSP_TYPE_CMD_INFO;

/* 検査情報	*/
typedef struct t_insp_inspection_info {
	E_INSP_INSPECTION_TYPE		type;				/* 検査種別								*/
	T_INSP_INSPECTION_PARAM		param;				/* パラメタ								*/
	E_INSP_INSPECTION_RESULT	result;				/* 検査結果								*/
} T_INSP_INSPECTION_INFO;

/********************************************************/
/*	内部定数テーブル									*/
/********************************************************/
/* 検査種別－コマンド情報テーブル */
INSP_LOCAL const T_INSP_TYPE_CMD_INFO C_insp_type_cmd_table[INSP_INSPECTION_TYPE_NUM] = {
	/*	検査種別,							コマンド				*/
	{	INSP_INSPECTION_TYPE_START,			CAN_INSPECTION_CMD_START				},	/* 検査開始								*/
	{	INSP_INSPECTION_TYPE_STOP,			CAN_INSPECTION_CMD_STOP					},	/* 検査停止								*/
	{	INSP_INSPECTION_TYPE_SERIAL_NO_WR,	CAN_INSPECTION_CMD_SERIAL_NO_WR			},	/* シリアルNo書き込み					*/
	{	INSP_INSPECTION_TYPE_SERIAL_NO_RD,	CAN_INSPECTION_CMD_SERIAL_NO_RD			},	/* シリアルNo読み出し					*/
	{	INSP_INSPECTION_TYPE_BT_PAIRING,	CAN_INSPECTION_CMD_BT_PAIRING			},	/* Bluetoothペアリング					*/
	{	INSP_INSPECTION_TYPE_BT_CONNECT,	CAN_INSPECTION_CMD_BT_CONNECT			},	/* Bluetooth通信						*/
	{	INSP_INSPECTION_TYPE_SD,			CAN_INSPECTION_CMD_SD					},	/* SDカード								*/
	{	INSP_INSPECTION_TYPE_CAN0,			CAN_INSPECTION_CMD_CAN0					},	/* CAN0									*/
	{	INSP_INSPECTION_TYPE_CAN1,			CAN_INSPECTION_CMD_CAN1					},	/* CAN1									*/
	{	INSP_INSPECTION_TYPE_UART,			CAN_INSPECTION_CMD_UART					},	/* UART									*/
	{	INSP_INSPECTION_TYPE_SW,			CAN_INSPECTION_CMD_SW					},	/* SW									*/
	{	INSP_INSPECTION_TYPE_GNSS,			CAN_INSPECTION_CMD_GNSS					},	/* GNSS									*/
};

/********************************************************/
/*	内部変数定義										*/
/********************************************************/
INSP_LOCAL E_INSP_STAT				S_insp_stat;					/* 検査モジュール制御状態		*/
INSP_LOCAL T_INSP_INSPECTION_INFO	S_insp_inspection_info;			/* 現在検査中の情報				*/
INSP_LOCAL T_INSP_INSPECTION_INFO	S_insp_inspection_info_prev;	/* 前回の検査情報				*/
INSP_LOCAL E_INSP_LED_MODE			S_insp_led_mode;				/* LEDモード					*/
INSP_LOCAL UI_32					S_insp_bt_pairing_time;			/* BTペアリング検査開始時間		*/

/********************************************************/
/*	内部関数宣言										*/
/********************************************************/
/* イベント操作関数	*/
INSP_LOCAL void insp_make_event( E_INSP_EVENT *event );

/* 状態遷移時関数 */
INSP_LOCAL void insp_state_on_enter( E_INSP_STAT stat );
INSP_LOCAL void insp_state_on_leave( E_INSP_STAT stat );

/* 状態関数	*/
INSP_LOCAL E_INSP_STAT insp_stat_init( E_INSP_EVENT event );
INSP_LOCAL E_INSP_STAT insp_stat_wait_start( E_INSP_EVENT event );
INSP_LOCAL E_INSP_STAT insp_stat_wait_inspection_req( E_INSP_EVENT event );
INSP_LOCAL E_INSP_STAT insp_stat_inspection( E_INSP_EVENT event );
INSP_LOCAL E_INSP_STAT insp_stat_end( E_INSP_EVENT event );
INSP_LOCAL E_INSP_STAT insp_stat_shutdown( E_INSP_EVENT event );

/* 状態関数テーブル	*/
typedef E_INSP_STAT (*PFN_INSP_STATE_PROC)( E_INSP_EVENT event );
INSP_LOCAL const PFN_INSP_STATE_PROC S_insp_state_proc[] = {
	insp_stat_init,							/*	初期状態					*/
	insp_stat_wait_start,					/*	検査開始待ち				*/
	insp_stat_wait_inspection_req,			/*	検査要求待ち				*/
	insp_stat_inspection,					/*	検査中						*/
	insp_stat_end,							/*	検査終了					*/
	insp_stat_shutdown,						/*	モジュール終了				*/
};

/* 初期化関数	*/
INSP_LOCAL void insp_init_param( void );
INSP_LOCAL void insp_init_inspection_info(T_INSP_INSPECTION_INFO *insp_info);

/* 検査関数	*/
INSP_LOCAL SI_8 insp_inspection_start(void);
INSP_LOCAL SI_8 insp_inspection_end(void);
INSP_LOCAL SI_8 insp_inspection_req(E_INSP_INSPECTION_TYPE type, const T_INSP_INSPECTION_PARAM *param);
INSP_LOCAL SI_8 insp_inspection_res(const T_INSP_INSPECTION_INFO *inspinfo);
INSP_LOCAL void insp_inspection_serial_no_read(void);
INSP_LOCAL void insp_inspection_bt_pairing(void);
INSP_LOCAL SI_8 insp_inspection_bt_connect(void);
INSP_LOCAL void insp_inspection_gnss(void);

/* コールバック関数	*/
INSP_LOCAL void insp_eeprom_setdata_cb(E_EEPROM_DATA_TYPE type, UI_16 data, SI_8 result);

/* 検査コマンド送受信関数	*/
INSP_LOCAL SI_8 insp_rcv_inspection_reqcmd(T_INSP_INSPECTION_INFO *inspinfo);
INSP_LOCAL SI_8 insp_snd_inspection_rescmd(const T_INSP_INSPECTION_INFO *inspinfo);
INSP_LOCAL SI_8 insp_type2cmd(E_INSP_INSPECTION_TYPE type, UI_8 *cmd);
INSP_LOCAL SI_8 insp_cmd2type(UI_8 cmd, E_INSP_INSPECTION_TYPE *type);

INSP_LOCAL void insp_upd_led_mode(void);

/****************************************************************************/
/*	外部関数																*/
/****************************************************************************/
/****************************************************************************/
/* 関数名称		: Insp_control_Init											*/
/* 機能の説明	: 検査モード制御初期化										*/
/* 引数			:	void													*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
void Insp_control_Init(void)
{
	/* モジュールで使用する変数初期化 */
	insp_init_param();

	/* 検査中のLEDモード:通常点灯	*/
	S_insp_led_mode = INSP_LED_MODE_NOMAL;

	/* 初期状態設定	*/
	S_insp_stat = INSP_STAT_INIT;

	return;
}

/****************************************************************************/
/* 関数名称		: Insp_control_Main											*/
/* 機能の説明	: 検査モード制御メイン										*/
/* 引数			:	void													*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
void Insp_control_Main(void)
{
	E_INSP_EVENT	event;
	E_INSP_STAT		nextstat;
	
	/* イベント作成 */
	insp_make_event( &event );

	/* 状態関数コール */
	if (event != INSP_EVENT_NONE) {
		nextstat = S_insp_state_proc[S_insp_stat]( event );
		if (S_insp_stat != nextstat) {			/* 状態遷移あり		*/
			OUTLOG(OutputLog("[Insp]stat:%d->%d\n", S_insp_stat, nextstat);)
			insp_state_on_leave(S_insp_stat);
			S_insp_stat = nextstat;
			insp_state_on_enter(nextstat);
		}
	}

	/* LEDモード更新	*/
	insp_upd_led_mode();

	return;
}

/****************************************************************************/
/* 関数名称		:	Insp_notify_inspection_result							*/
/* 機能の説明	:	検査結果通知											*/
/* 引数			:	(I/ )type			検査種別							*/
/* 				:	(I/ )param			パラメタ							*/
/* 				:	(I/ )result			検査結果							*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
void Insp_notify_inspection_result(E_INSP_INSPECTION_TYPE type, const T_INSP_INSPECTION_PARAM *param, E_INSP_INSPECTION_RESULT result)
{

	/* 検査結果を設定 */
	if (S_insp_inspection_info.type == INSP_INSPECTION_TYPE_INVLID) {
		/* 検査中ではない	*/
		OUTLOG(OutputLog("[Insp]notify res insptype invalid!! type:%u restype:%u result:%u\n", S_insp_inspection_info.type, type, result);)
		return;
	}

	if (S_insp_inspection_info.type != type) {
		/* 検査中とは異なる通知	*/
		OUTLOG(OutputLog("[Insp]notify res insptype diff!! type:%u restype:%u result:%u\n", S_insp_inspection_info.type, type, result);)
		return;
	}

	/* パラメタをコマンド情報に格納 */
	if (param != NULL) {
		S_insp_inspection_info.param = *param;
	}

	/* 結果をコマンド情報に格納 */
	if (result == INSP_INSPECTION_RESULT_OK) {
		S_insp_inspection_info.result = INSP_INSPECTION_RESULT_OK;
	}
	else {
		S_insp_inspection_info.result = INSP_INSPECTION_RESULT_NG;
	}

	OUTLOG(OutputLog("[Insp]notify res type:%u res:%u para:%02X %02X %08X\n",
						S_insp_inspection_info.type, S_insp_inspection_info.result,
						S_insp_inspection_info.param.data1, S_insp_inspection_info.param.data2, S_insp_inspection_info.param.data3);)

	return;

}

/****************************************************************************/
/* 関数名称		:	Insp_get_led_mode										*/
/* 機能の説明	:	検査LEDモード取得										*/
/* 引数			:	void													*/
/* 戻り値		:	LEDモード(E_INSP_LED_MODE_xxx)							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
E_INSP_LED_MODE Insp_get_led_mode(void)
{
	return S_insp_led_mode;
}

/****************************************************************************/
/*	内部関数																*/
/****************************************************************************/
/****************************************************************************/
/* 関数名称		:	insp_make_event											*/
/* 機能の説明	:	イベント作成											*/
/* 引数				( /O)event			イベント							*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL void insp_make_event( E_INSP_EVENT *event )
{

	E_SYS_CTRL_STATUS		sysStat;
	T_INSP_INSPECTION_INFO	inspinfo;
	SI_8					ret;

	if (event == NULL) {
		return;
	}

	*event = INSP_EVENT_NONE;

	sysStat = System_control_GetStatus();

	switch ( sysStat ) {
	case SYS_OPERATION_INSPECTION:				/* システム動作状態:検査モード	----------------*/
		if ((S_insp_stat == INSP_STAT_INIT) ||						/* 検査モジュール制御状態:初期状態				*/
			(S_insp_stat == INSP_STAT_INSPECTION_END)) {			/* 検査モジュール制御状態:検査終了				*/
			/* モジュール起動イベント	*/
			*event = INSP_EVENT_STARTUP;
		}
		else if (S_insp_stat == INSP_STAT_WAIT_INSPECTION_START) {	/* 検査モジュール制御状態:検査開始待ち			*/
			/* 検査要求コマンド受信	*/
			ret = insp_rcv_inspection_reqcmd( &inspinfo );
			if ((ret == N_OK) && (inspinfo.type == INSP_INSPECTION_TYPE_START)) {
				/* 検査開始イベント */
				*event = INSP_EVENT_INSPECTION_START;
			}
		}
		else if (S_insp_stat == INSP_STAT_WAIT_INSPECTION_REQ) {	/* 検査モジュール制御状態:検査要求待ち			*/
			/* 検査要求コマンド受信	*/
			ret = insp_rcv_inspection_reqcmd( &inspinfo );
			if (ret == N_OK) {
				if (inspinfo.type == INSP_INSPECTION_TYPE_STOP) {
					/* 検査終了イベント */
					*event = INSP_EVENT_INSPECTION_END;
				}
				else if (inspinfo.type == INSP_INSPECTION_TYPE_START) {
					/* 検査開始は受け付けない	*/
					/* nop */
				}
				else {
					/* 検査要求情報を設定 */
					S_insp_inspection_info.type		= inspinfo.type;
					S_insp_inspection_info.param	= inspinfo.param;
					S_insp_inspection_info.result	= INSP_INSPECTION_RESULT_INVALID;

					/* 検査要求イベント */
					*event = INSP_EVENT_INSPECTION_REQ;
				}
			}
		}
		else if (S_insp_stat == INSP_STAT_INSPECTION) {				/* 検査モジュール制御状態:検査中				*/
			/* 検査結果をチェック	*/
			if (S_insp_inspection_info.result != INSP_INSPECTION_RESULT_INVALID) {	/* 検査結果が格納されている	*/
				/* 検査応答イベント */
				*event = INSP_EVENT_INSPECTION_RES;
			}
			else {
				/* 検査定期処理イベント */
				*event = INSP_EVENT_INSPECTION_PERIOD_PROC;
			}
		}
		else {
			/* nop */
		}
		break;

	case SYS_OPERATION_PRE_ACC_OFF:				/* システム動作状態:ACC OFF移行	----------------*/
		if (S_insp_stat != INSP_STAT_INSPECTION_END) {				/* 検査モジュール制御状態:検査終了以外					*/
			/* ACC OFF移行イベント */
			*event = INSP_EVENT_PRE_ACC_OFF;
		}
		break;

	case SYS_OPERATION_ACC_OFF:					/* システム動作状態:ACC OFF	----------------*/
		if (S_insp_stat != INSP_STAT_SHUTDOWN) {					/* 検査モジュール制御状態:モジュール終了以外			*/
			/* モジュール終了イベント */
			*event = INSP_EVENT_SHUTDOWN;
		}
		break;

	default:
		break;
	}

	return;

}

/****************************************************************************/
/* 関数名称		:	insp_state_on_enter										*/
/* 機能の説明	:	状態に入るときの処理									*/
/* 引数				(I/ )stat	:これから入る状態							*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL void insp_state_on_enter( E_INSP_STAT stat )
{

	switch (stat) {
	case INSP_STAT_INIT:					/*	初期状態				*/
		insp_init_inspection_info(&S_insp_inspection_info);
		insp_init_inspection_info(&S_insp_inspection_info_prev);
		break;

	case INSP_STAT_WAIT_INSPECTION_START:	/*	検査開始待ち			*/
	case INSP_STAT_WAIT_INSPECTION_REQ:		/*	検査要求待ち			*/
		insp_init_inspection_info(&S_insp_inspection_info);
		break;

	case INSP_STAT_INSPECTION_END:			/*	検査終了				*/
		insp_init_inspection_info(&S_insp_inspection_info);
		insp_init_inspection_info(&S_insp_inspection_info_prev);

		/* システム制御に検査完了を通知	*/
		(void)System_control_NotifyEvent(SYS_EVENT_END_INSPECTION, SYS_EVENT_PARAM_NO);
		break;

	default:
		break;
	}

	return;

}

/****************************************************************************/
/* 関数名称		:	insp_state_on_leave										*/
/* 機能の説明	:	状態から出るときの処理									*/
/* 引数			:	(I/ )stat	:これから出る状態							*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL void insp_state_on_leave( E_INSP_STAT stat )
{

	if (stat == INSP_STAT_INSPECTION) {				/* 検査中					*/
		/* 今回の検査情報を前回の検査情報にコピー	*/
		S_insp_inspection_info_prev = S_insp_inspection_info;
	}

	return;

}

/****************************************************************************/
/* 関数名称		:	insp_stat_init											*/
/* 機能の説明	:	状態関数:INSP_STAT_INIT									*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	INSP_STAT_xxx	遷移先状態								*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL E_INSP_STAT insp_stat_init( E_INSP_EVENT event )
{

	E_INSP_STAT	nextStat = INSP_STAT_INIT;
	UI_8			variation;

	if (event == INSP_EVENT_STARTUP) {					/* モジュール起動				*/

		/* バリエーション情報を取得する	*/
		if (Sd_control_GetInspectionVariation(&variation) == N_OK) {

			OUTLOG(OutputLog("[Insp]variation write :%02X\n", variation);)

			/* バリエーション情報をEEPROMに書き込む	*/
			if (Eeprom_control_SetData(EEPROM_DATA_VARIATION, (UI_16)variation, NULL) == EEPROM_CONTROL_RESULT_OK) {
				/* 遷移先状態:検査開始待ち	*/
				nextStat = INSP_STAT_WAIT_INSPECTION_START;
			}
			else {
				/* 遷移先状態:検査終了	*/
				nextStat = INSP_STAT_INSPECTION_END;
			}
		}
		else {
			/* 遷移先状態:検査終了	*/
			nextStat = INSP_STAT_INSPECTION_END;
		}
	}
	else if (event == INSP_EVENT_PRE_ACC_OFF) {			/* ACC OFF移行				*/
		/* 遷移先状態:検査終了	*/
		nextStat = INSP_STAT_INSPECTION_END;
	}
	else {
		/* nop */
	}

	return nextStat;

}

/****************************************************************************/
/* 関数名称		:	insp_stat_wait_start									*/
/* 機能の説明	:	状態関数:INSP_STAT_WAIT_INSPECTION_START				*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	INSP_STAT_xxx	遷移先状態								*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL E_INSP_STAT insp_stat_wait_start( E_INSP_EVENT event )
{

	E_INSP_STAT	nextStat = INSP_STAT_WAIT_INSPECTION_START;

	if (event == INSP_EVENT_INSPECTION_START) {			/* 検査開始						*/
		/* 検査開始処理 */
		(void)insp_inspection_start();

		/* 遷移先状態:検査要求待ち	*/
		nextStat = INSP_STAT_WAIT_INSPECTION_REQ;
	}
	else if (event == INSP_EVENT_PRE_ACC_OFF) {			/* ACC OFF移行					*/
		/* 遷移先状態:検査終了	*/
		nextStat = INSP_STAT_INSPECTION_END;
	}
	else {
		/* nop */
	}

	return nextStat;

}

/****************************************************************************/
/* 関数名称		:	insp_stat_wait_inspection_req							*/
/* 機能の説明	:	状態関数:INSP_STAT_WAIT_INSPECTION_REQ					*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	INSP_STAT_xxx	遷移先状態								*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL E_INSP_STAT insp_stat_wait_inspection_req( E_INSP_EVENT event )
{

	E_INSP_STAT	nextStat = INSP_STAT_WAIT_INSPECTION_REQ;
	SI_8			ret;

	if (event == INSP_EVENT_INSPECTION_REQ) {				/* 検査要求						*/
		/* 検査要求 */
		ret = insp_inspection_req(S_insp_inspection_info.type, &S_insp_inspection_info.param);
		if (ret != N_OK) {
			/* 検査要求が受け付けられない場合はエラーを通知 */
			Insp_notify_inspection_result(S_insp_inspection_info.type, NULL, INSP_INSPECTION_RESULT_NG);
		}

		/* 遷移先状態:検査中	*/
		nextStat = INSP_STAT_INSPECTION;
	}
	else if (event == INSP_EVENT_INSPECTION_END) {			/* 検査終了						*/
		/* 検査終了処理 */
		(void)insp_inspection_end();

		/* 遷移先状態:検査終了	*/
		nextStat = INSP_STAT_INSPECTION_END;
	}
	else if (event == INSP_EVENT_PRE_ACC_OFF) {				/* ACC OFF移行					*/
		/* 遷移先状態:検査終了	*/
		nextStat = INSP_STAT_INSPECTION_END;
	}
	else {
		/* nop */
	}

	return nextStat;

}

/****************************************************************************/
/* 関数名称		:	insp_stat_inspection									*/
/* 機能の説明	:	状態関数:INSP_STAT_INSPECTION							*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	INSP_STAT_xxx	遷移先状態								*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL E_INSP_STAT insp_stat_inspection( E_INSP_EVENT event )
{

	E_INSP_STAT			nextStat = INSP_STAT_INSPECTION;

	if (event == INSP_EVENT_INSPECTION_RES) {				/* 検査応答						*/
		/* 検査応答時処理 */
		(void)insp_inspection_res(&S_insp_inspection_info);

		/* 遷移先状態:検査要求待ち	*/
		nextStat = INSP_STAT_WAIT_INSPECTION_REQ;
	}
	else if (event == INSP_EVENT_INSPECTION_PERIOD_PROC) {	/* 検査定期処理					*/
		if (S_insp_inspection_info.type == INSP_INSPECTION_TYPE_BT_PAIRING) {		/* 実行中の検査:BTペアリング	*/
			/* BTペアリング検査	*/
			insp_inspection_bt_pairing();
		}
		else if (S_insp_inspection_info.type == INSP_INSPECTION_TYPE_GNSS) {		/* 実行中の検査:GNSS			*/
			/* GNSS検査	*/
			insp_inspection_gnss();
		}
		else {
			/* nop */
		}
	}
	else if (event == INSP_EVENT_PRE_ACC_OFF) {				/* ACC OFF移行					*/
		/* 遷移先状態:検査終了	*/
		nextStat = INSP_STAT_INSPECTION_END;
	}
	else {
		/* nop */
	}

	return nextStat;

}

/****************************************************************************/
/* 関数名称		:	insp_stat_end											*/
/* 機能の説明	:	状態関数:INSP_STAT_INSPECTION_END						*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	INSP_STAT_xxx	遷移先状態								*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL E_INSP_STAT insp_stat_end( E_INSP_EVENT event )
{

	E_INSP_STAT	nextStat = INSP_STAT_INSPECTION_END;

	if (event == INSP_EVENT_STARTUP) {					/* モジュール起動				*/
		/* 遷移先状態:初期状態	*/
		nextStat = INSP_STAT_INIT;
	}
	else if (event == INSP_EVENT_SHUTDOWN) {			/* モジュール終了				*/
		/* 遷移先状態:モジュール終了	*/
		nextStat = INSP_STAT_SHUTDOWN;
	}
	else {
		/* nop */
	}

	return nextStat;

}

/****************************************************************************/
/* 関数名称		:	insp_stat_shutdown										*/
/* 機能の説明	:	状態関数:INSP_STAT_SHUTDOWN								*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	INSP_STAT_xxx	遷移先状態								*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL E_INSP_STAT insp_stat_shutdown( E_INSP_EVENT event )
{

	E_INSP_STAT	nextStat = INSP_STAT_SHUTDOWN;

	/* NOP */

	return nextStat;

}

/****************************************************************************/
/* 関数名称		:	insp_init_param											*/
/* 機能の説明	:	パラメタ初期化											*/
/* 引数			:	void													*/
/* 戻り値		: 	void													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:	モジュール状態は更新しない								*/
/****************************************************************************/
INSP_LOCAL void insp_init_param( void )
{

	/* モジュール内変数初期化	*/
	S_insp_led_mode = INSP_LED_MODE_OFF;
	insp_init_inspection_info(&S_insp_inspection_info);
	insp_init_inspection_info(&S_insp_inspection_info_prev);
	S_insp_bt_pairing_time = 0;

	return;

}

/****************************************************************************/
/* 関数名称		:	insp_init_inspection_info								*/
/* 機能の説明	:	検査情報初期化											*/
/* 引数			:	( /O)insp_info		検査情報							*/
/* 戻り値		: 	void													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL void insp_init_inspection_info(T_INSP_INSPECTION_INFO *insp_info)
{

	if (insp_info != NULL) {
		(void)memset(insp_info, 0, sizeof(T_INSP_INSPECTION_INFO));
		insp_info->type		= INSP_INSPECTION_TYPE_INVLID;
		insp_info->result	= INSP_INSPECTION_RESULT_INVALID;
	}

	return;

}

/****************************************************************************/
/* 関数名称		:	insp_inspection_start									*/
/* 機能の説明	:	検査開始処理											*/
/* 引数			:	void													*/
/* 戻り値		:	正常:N_OK												*/
/*				:	異常:N_NG												*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_inspection_start(void)
{

	T_INSP_INSPECTION_INFO	inspinfo;
	SI_8	ret;

	/* 検査開始応答コマンド送信	*/
	(void)memset(&inspinfo, 0, sizeof(inspinfo));
	inspinfo.type = INSP_INSPECTION_TYPE_START;
	inspinfo.result = INSP_INSPECTION_RESULT_OK;
	inspinfo.param.data1 = INSP_VDV_MODEL_INFO;
	inspinfo.param.data2 = INSP_VDV_FIRM_INFO;
	ret = insp_snd_inspection_rescmd(&inspinfo);

	return ret;

}

/****************************************************************************/
/* 関数名称		:	insp_inspection_end										*/
/* 機能の説明	:	検査終了処理											*/
/* 引数			:	void													*/
/* 戻り値		:	正常:N_OK												*/
/*				:	異常:N_NG												*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_inspection_end(void)
{

	T_INSP_INSPECTION_INFO	inspinfo;
	SI_8	ret;

	/* 検査停止応答コマンド送信	*/
	(void)memset(&inspinfo, 0, sizeof(inspinfo));
	inspinfo.type = INSP_INSPECTION_TYPE_STOP;
	inspinfo.result = INSP_INSPECTION_RESULT_OK;
	ret = insp_snd_inspection_rescmd(&inspinfo);

	return ret;

}

/****************************************************************************/
/* 関数名称		:	insp_inspection_req										*/
/* 機能の説明	:	検査要求処理											*/
/* 引数			:	(I/ )type		検査種別								*/
/* 				:	(I/ )param			パラメタ							*/
/* 戻り値		:	正常:N_OK												*/
/*				:	異常:N_NG												*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:	戻り値が正常(N_OK)の場合、検査要求が受け付けられ		*/
/*					検査結果がS_insp_inspection_infoに設定される			*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_inspection_req(E_INSP_INSPECTION_TYPE type, const T_INSP_INSPECTION_PARAM *param)
{

	UI_16	serial_no;
	SI_8	ret = N_OK;


	switch(type) {
	case INSP_INSPECTION_TYPE_SERIAL_NO_WR:
		/* シリアルNo書き込み検査	*/
		if (param != NULL) {
			serial_no = (((UI_16)param->data1) << 8) | ((UI_16)param->data2);
			if (Eeprom_control_SetData(EEPROM_DATA_SERIAL_NO, serial_no, insp_eeprom_setdata_cb) != EEPROM_CONTROL_RESULT_OK) {
				ret = N_NG;
			}
		}
		else {
			ret = N_NG;
		}
		break;

	case INSP_INSPECTION_TYPE_SERIAL_NO_RD:
		/* シリアルNo読み出し検査	*/
		insp_inspection_serial_no_read();
		break;

	case INSP_INSPECTION_TYPE_BT_PAIRING:
		/* Bluetoothペアリング検査	*/
		(void)Bt_control_reqPairingInspection();
		S_insp_bt_pairing_time = Timer_ctrl_GetTime1ms();
		break;

	case INSP_INSPECTION_TYPE_BT_CONNECT:
		/* Bluetooth通信検査	*/
		ret = insp_inspection_bt_connect();
		break;

	case INSP_INSPECTION_TYPE_SD:
		/* SDカード検査	*/
		if (param != NULL) {
			serial_no = (((UI_16)param->data1) << 8) | ((UI_16)param->data2);
			ret = Sd_control_ReqInspection(serial_no);
		}
		else {
			ret = N_NG;
		}
		break;

	case INSP_INSPECTION_TYPE_CAN0:
		/* CAN0検査	*/
		ret = Can_control_ReqInspection(CAN_INSPECTION_CAN_CH0);
		break;

	case INSP_INSPECTION_TYPE_CAN1:
		/* CAN1検査	*/
		ret = Can_control_ReqInspection(CAN_INSPECTION_CAN_CH1);
		break;

	case INSP_INSPECTION_TYPE_UART:
		/* UART検査	*/
		ret = Advance_control_ReqInspection();
		break;

	case INSP_INSPECTION_TYPE_SW:
		/* SW検査	*/
		ret = Sw_control_ReqInspection();
		break;

	case INSP_INSPECTION_TYPE_GNSS:
		/* GNSS検査	*/
		insp_inspection_gnss();
		break;

	default:
		ret = N_NG;
		break;
	}

	if (ret != N_OK) {
		OUTLOG(OutputLog("[Insp]insp req err!! type:%u\n", type);)
	}

	return ret;

}

/****************************************************************************/
/* 関数名称		:	insp_inspection_res										*/
/* 機能の説明	:	検査応答処理											*/
/* 引数			:	(I/ )inspinfo		検査情報							*/
/* 戻り値		:	正常:N_OK												*/
/*				:	異常:N_NG												*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_inspection_res(const T_INSP_INSPECTION_INFO *inspinfo)
{

	SI_8	ret;

	if (inspinfo == NULL) {
		return N_NG;
	}

	/* 検査応答コマンド送信 */
	ret = insp_snd_inspection_rescmd(inspinfo);
	if (ret != N_OK) {
		return N_NG;
	}

	return N_OK;

}

/****************************************************************************/
/* 関数名称		:	insp_inspection_serial_no_read							*/
/* 機能の説明	:	シリアルNo読み出し検査									*/
/* 引数			:	void													*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL void insp_inspection_serial_no_read(void)
{

	E_EEPROM_CONTROL_RESULT		eep_res;
	E_INSP_INSPECTION_RESULT	insp_res;
	T_INSP_INSPECTION_PARAM		param;
	UI_16	serial_no;

	(void)memset(&param, 0, sizeof(param));

	/* EEPROMからシリアルNoを読みだす	*/
	eep_res = Eeprom_control_GetData(EEPROM_DATA_SERIAL_NO, &serial_no);
	if (eep_res == EEPROM_CONTROL_RESULT_OK) {
		param.data1 = (UI_8)(serial_no >> 8);
		param.data2 = (UI_8)(serial_no & 0x00FF);
		insp_res = INSP_INSPECTION_RESULT_OK;
	}
	else {
		insp_res = INSP_INSPECTION_RESULT_NG;
	}

	/* 読み出し結果を通知	*/
	Insp_notify_inspection_result(INSP_INSPECTION_TYPE_SERIAL_NO_RD, &param, insp_res);

	return;

}

/****************************************************************************/
/* 関数名称		:	insp_inspection_bt_pairing								*/
/* 機能の説明	:	Bluetoothペアリング検査									*/
/* 引数			:	void													*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL void insp_inspection_bt_pairing(void)
{

	E_BT_CONTROL_STAT			btStat;
	E_BT_COMM_MDL_STATE			btMdlStat;

	/* BT状態取得	*/
	btStat = Bt_control_GetStatus();
	if (btStat == BT_CONTROL_STAT_PAIRING) {					/* BT状態:ペアリング	*/

		/* BTミドル状態を取得	*/
		btMdlStat = Bt_control_getMdlState();
		if (btMdlStat == BT_COMM_MDL_STATE_PAIRING_MODE) {		/* BTミドル状態:ペアリング	*/


				/* BTペアリング検査結果 正常を通知	*/
				Insp_notify_inspection_result(INSP_INSPECTION_TYPE_BT_PAIRING, NULL, INSP_INSPECTION_RESULT_OK);
				return;
		}
	}

	if (Timer_ctrl_TimeCompare1ms(S_insp_bt_pairing_time, INSP_BT_PAIRING_TIMEOUT) == N_OK) {	/* BTペアリング検査タイムアウト	*/

		/* BTペアリング検査結果 異常を通知	*/
		Insp_notify_inspection_result(INSP_INSPECTION_TYPE_BT_PAIRING, NULL, INSP_INSPECTION_RESULT_NG);
	}

	return;

}

/****************************************************************************/
/* 関数名称		:	insp_inspection_bt_connect								*/
/* 機能の説明	:	Bluetooth通信検査										*/
/* 引数			:	void													*/
/* 戻り値		:	正常:N_OK												*/
/*				:	異常:N_NG												*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_inspection_bt_connect(void)
{

	E_BT_CONTROL_STAT			btStat;

	/* Bluetooth状態取得	*/
	btStat = Bt_control_GetStatus();
	if ((btStat == BT_CONTROL_STAT_COMM_ILDE) || (btStat == BT_CONTROL_STAT_COMM_SENDING)) {
		/* BT接続済み	*/
		/* BT通信検査結果 正常を通知	*/
		Insp_notify_inspection_result(INSP_INSPECTION_TYPE_BT_CONNECT, NULL, INSP_INSPECTION_RESULT_OK);

		/* BT通信開始	*/
		(void)Bt_control_reqCommunicationInspection();
	}
	else {
		/* BT通信検査結果 異常を通知	*/
		Insp_notify_inspection_result(INSP_INSPECTION_TYPE_BT_CONNECT, NULL, INSP_INSPECTION_RESULT_NG);
	}

	return N_OK;

}

/****************************************************************************/
/* 関数名称		:	insp_inspection_gnss									*/
/* 機能の説明	:	GNSS検査												*/
/* 引数			:	void													*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL void insp_inspection_gnss(void)
{

	UI_8	gps_fix_type;
	E_N_RC	gnss_res;
	T_Gnss_ctrl_nav_pvt_info 	*hnr_info;


	E_INSP_INSPECTION_RESULT	insp_res;

	/* GPS Fix Type値を取得	*/
	gnss_res = Gnss_control_GetGpsFixType(&gps_fix_type);
	if ((gnss_res == N_RC_OK) &&
		((gps_fix_type == GNSS_GSPFIX_3DFIX) || (gps_fix_type == GNSS_GSPFIX_GPS_AND_DEADRECKONING))) {
		/* 3D測位中になったので検査する	*/

		/* GNSS NAV-HNR情報カレント値を取得	*/
		hnr_info = Gnss_cntrol_GetNavPvtInfo();
		if (hnr_info != NULL) {
			/* 緯度経度をチェックする	*/
			if ((INSP_GNSS_LAT_TOLERANCE_MIN <= hnr_info->lat) && (hnr_info->lat <= INSP_GNSS_LAT_TOLERANCE_MAX) &&
				(INSP_GNSS_LON_TOLERANCE_MIN <= hnr_info->lon) && (hnr_info->lon <= INSP_GNSS_LON_TOLERANCE_MAX)) {
				/* 緯度経度が検査地点の許容範囲内にある	*/
				insp_res = INSP_INSPECTION_RESULT_OK;
			}
			else {
				insp_res = INSP_INSPECTION_RESULT_NG;
			}

			/* GNSS検査結果を通知する	*/
			Insp_notify_inspection_result(INSP_INSPECTION_TYPE_GNSS, NULL, insp_res);
		}
	}

	return;

}

/****************************************************************************/
/* 関数名称		:	insp_eeprom_setdata_cb									*/
/* 機能の説明	:	eeprom書き込みコールバック								*/
/* 引数			: (I/ )type	データ種別  E_EEPROM_DATA_TYPE					*/
/* 				: (I/ )data	設定データ  									*/
/*				: (I/ )result	書き込み結果(N_OK:正常, N_NG:異常)			*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/02/09												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL void insp_eeprom_setdata_cb(E_EEPROM_DATA_TYPE type, UI_16 data, SI_8 result)
{

	E_INSP_INSPECTION_RESULT	insp_res;

	if (S_insp_inspection_info.type == INSP_INSPECTION_TYPE_SERIAL_NO_WR) {		/* 検査種別:シリアルNo書き込み	*/
		if (type == EEPROM_DATA_SERIAL_NO) {			/* EEPROMデータ種別:シリアルNo		*/

			if (result == N_OK) {
				insp_res = INSP_INSPECTION_RESULT_OK;
			}
			else {
				insp_res = INSP_INSPECTION_RESULT_NG;
			}

			/* シリアルNo書き込み結果を通知	*/
			Insp_notify_inspection_result(INSP_INSPECTION_TYPE_SERIAL_NO_WR, NULL, insp_res);
		}
	}

	return;

}

/****************************************************************************/
/* 関数名称		:	insp_rcv_inspection_reqcmd								*/
/* 機能の説明	:	検査要求コマンド受信									*/
/* 引数			:	( /O)inspinfo		検査情報							*/
/* 戻り値		:	N_OK		検査要求を受信した							*/
/* 				:	N_NG		検査要求未受信またはエラー					*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_rcv_inspection_reqcmd(T_INSP_INSPECTION_INFO *inspinfo)
{

	T_Can_inspection_cmd_info	cmdinfo;
	E_INSP_INSPECTION_TYPE		type;
	SI_8	ret;

	if (inspinfo == NULL) {
		return N_NG;
	}

	/* CANから検査コマンドを受信する	*/
	ret = Can_control_RcvInspectionCmd(&cmdinfo);
	if (ret != N_OK) {
		return N_NG;
	}

	OUTLOG(OutputLog("[Insp]rcv can cmd cmd:%02X res:%02X data:%02X %02X %08X\n",
						cmdinfo.cmd, cmdinfo.result, cmdinfo.data1, cmdinfo.data2, cmdinfo.data3);)

	/* CAN検査コマンドを検査種別に変換 */
	ret = insp_cmd2type(cmdinfo.cmd, &type);
	if (ret != N_OK) {
		OUTLOG(OutputLog("[Insp]rcv can cmd invalid cmd:%02X\n", cmdinfo.cmd);)
		return N_NG;
	}

	/* 検査情報設定 */
	inspinfo->type = type;
	inspinfo->result = INSP_INSPECTION_RESULT_INVALID;
	inspinfo->param.data1 = cmdinfo.data1;
	inspinfo->param.data2 = cmdinfo.data2;
	inspinfo->param.data3 = cmdinfo.data3;

	return N_OK;

}

/****************************************************************************/
/* 関数名称		:	insp_snd_inspection_rescmd								*/
/* 機能の説明	:	検査応答コマンド送信									*/
/* 引数			:	(I/ )inspinfo		検査情報							*/
/* 戻り値		:	正常:N_OK												*/
/*				:	異常:N_NG												*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_snd_inspection_rescmd(const T_INSP_INSPECTION_INFO *inspinfo)
{

	T_Can_inspection_cmd_info	cmdinfo;
	SI_8	ret;

	if (inspinfo == NULL) {
		return N_NG;
	}

	/* 検査種別をCAN検査コマンドに変換 */
	ret = insp_type2cmd(inspinfo->type, &cmdinfo.cmd);
	if (ret != N_OK) {
		return N_NG;
	}

	/* CANコマンド設定 */
	if (inspinfo->result == INSP_INSPECTION_RESULT_OK) {
		cmdinfo.result = CAN_INSPECTION_RESULT_OK;
	}
	else {
		cmdinfo.result = CAN_INSPECTION_RESULT_NG;
	}
	cmdinfo.data1 = inspinfo->param.data1;
	cmdinfo.data2 = inspinfo->param.data2;
	cmdinfo.data3 = inspinfo->param.data3;

	OUTLOG(OutputLog("[Insp]snd can cmd cmd:%02X res:%02X data:%02X %02X %08X\n",
						cmdinfo.cmd, cmdinfo.result, cmdinfo.data1, cmdinfo.data2, cmdinfo.data3);)

	/* CANで検査コマンドを送信する	*/
	ret = Can_control_SndInspectionCmd(&cmdinfo);
	if (ret != N_OK) {
		return N_NG;
	}

	return N_OK;

}

/****************************************************************************/
/* 関数名称		:	insp_type2cmd											*/
/* 機能の説明	:	検査種別->コマンド変換									*/
/* 引数			:	(I/ )type			検査種別							*/
/* 				: 	( /O)cmd			コマンド							*/
/* 戻り値		:	正常:N_OK												*/
/*				:	異常:N_NG												*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_type2cmd(E_INSP_INSPECTION_TYPE type, UI_8 *cmd)
{

	UI_8	i;

	if (cmd == NULL) {
		return N_NG;
	}

	for (i = 0; i < INSP_INSPECTION_TYPE_NUM; i++) {
		if (C_insp_type_cmd_table[i].type == type) {
			*cmd = C_insp_type_cmd_table[i].cmd;
			return N_OK;
		}
	}
	return N_NG;

}

/****************************************************************************/
/* 関数名称		:	insp_cmd2type											*/
/* 機能の説明	:	コマンド->検査種別変換									*/
/* 引数			: 	(I/ )cmd			コマンド							*/
/* 				:	( /O)type			検査種別							*/
/* 戻り値		:	正常:N_OK												*/
/*				:	異常:N_NG												*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_cmd2type(UI_8 cmd, E_INSP_INSPECTION_TYPE *type)
{

	UI_8	i;

	if (type == NULL) {
		return N_NG;
	}

	for (i = 0; i < INSP_INSPECTION_TYPE_NUM; i++) {
		if (C_insp_type_cmd_table[i].cmd == cmd) {
			*type = C_insp_type_cmd_table[i].type;
			return N_OK;
		}
	}
	return N_NG;

}

/****************************************************************************/
/* 関数名称		:	insp_upd_led_mode										*/
/* 機能の説明	:	LEDモード更新											*/
/* 引数			:	void													*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
INSP_LOCAL void insp_upd_led_mode(void)
{

	E_INSP_INSPECTION_TYPE		type;
	E_INSP_INSPECTION_RESULT	result;
	E_BT_COMM_MDL_STATE			btMdlStat;

	switch (S_insp_stat) {
	case INSP_STAT_WAIT_INSPECTION_REQ:
	case INSP_STAT_INSPECTION:
		if (S_insp_inspection_info.type != INSP_INSPECTION_TYPE_INVLID) {			/* 現在検査中							*/
			type	= S_insp_inspection_info.type;
			result	= S_insp_inspection_info.result;
		}
		else if (S_insp_inspection_info_prev.type != INSP_INSPECTION_TYPE_INVLID) {	/* 現在未検査  前回の検査結果あり		*/
			type	= S_insp_inspection_info_prev.type;
			result	= S_insp_inspection_info_prev.result;
		}
		else {
			type	= INSP_INSPECTION_TYPE_INVLID;
			result	= INSP_INSPECTION_RESULT_INVALID;
		}

		if (type == INSP_INSPECTION_TYPE_BT_PAIRING) {		/* 検査種別:Bluetoothペアリング	*/
			/* BTミドル状態を取得	*/
			btMdlStat = Bt_control_getMdlState();
			if (btMdlStat == BT_COMM_MDL_STATE_PAIRING_MODE) {	/* BTミドル状態:ペアリング	*/
				/* 検査中のLEDモード:Buletoothペアリング	*/
				S_insp_led_mode = INSP_LED_MODE_BT_PAIRING;
			}
			else {
				/* 検査中のLEDモード:通常点灯	*/
				S_insp_led_mode = INSP_LED_MODE_NOMAL;
			}
		}
		else if (type == INSP_INSPECTION_TYPE_SW) {			/* 検査種別:SW検査					*/
			if (result == INSP_INSPECTION_RESULT_OK) {			/* SW検査正常終了	*/
				/* 検査中のLEDモード:通常点灯	*/
				S_insp_led_mode = INSP_LED_MODE_NOMAL;
			}
			else {												/* SW検査中または異常終了	*/
				/* 検査中のLEDモード:SW検査	*/
				S_insp_led_mode = INSP_LED_MODE_SW_INSPECT;
			}
		}
		else {
			/* 検査中のLEDモード:通常点灯	*/
			S_insp_led_mode = INSP_LED_MODE_NOMAL;
		}
		break;

	case INSP_STAT_SHUTDOWN:
		/* 検査中のLEDモード:消灯	*/
		S_insp_led_mode = INSP_LED_MODE_OFF;
		break;

	default:
		/* 検査中のLEDモード:通常点灯	*/
		S_insp_led_mode = INSP_LED_MODE_NOMAL;
		break;
	}

	return;

}

