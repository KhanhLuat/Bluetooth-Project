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
/* file DTO459_VDV_Can_ctrl.c							*/
/* CAN制御												*/
/********************************************************/

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/

#include <type.h>
#include <system.h>
#include <string.h>
#include "DTO459_VDV_Common.h"
#include "timer_ctrl.h"
#include "DTO459_VDV_Inspection_ctrl.h"
#include "dbglog.h"
#include "DTO459_VDV_Can_ctrl.h"
#include "System_control_TASK.h"
#include "TCan_comm_Ctrl.h"
#include "DTO459_VDV_Can_Drv.h"
#include "DTO459_VDV_local_can_ctrl.h"
#ifdef ISOCAN_ENABLE
#include "DTO459_VDV_Iso_can_ctrl.h"
#endif /* ISOCAN_ENABLE */
#include "SSFTSTD_FR81_Port_drv.h"
#include "DTO459_VDV_Eeprom_ctrl.h"

/********************************************************/
/*	マクロ定義											*/
/********************************************************/
#define CAN_CTRL_LOCAL	static



#define CAN_CH_TCAN		CAN_CH_0		/* CANチャンネル : T-CAN */

#define CAN_TCAN_DATA_RCV_TIMEOUT			(10000)		/* T-CANデータ受信タイムアウト(10000ms)	*/

#define CAN_INSPECTION_SEND_BUFFER_COUNT	(5)			/* 検査用CANの送信バッファ数			*/
#define CAN_INSPECTION_MSG_ID_RX_MIN		(CAN_INSPECTION_SEND_BUFFER_COUNT + 1)
														/* 検査用CANの受信用メッセージID最小値	*/

#define CAN_INSPECTION_RCV_CMD_CAN_ID		(0x100)		/* 検査コマンド受信用CAN ID				*/
#define CAN_INSPECTION_SND_CMD_CAN_ID		(0x101)		/* 検査コマンド送信用CAN ID				*/

#define CAN_INSPECTION_TIMEOUT				(3000)		/* CAN検査タイムアウト(3000ms)			*/
#define CAN_INSPECTION_MSG_NUM				(3)			/* CAN検査メッセージ数					*/


/* 検査状態 */
typedef enum {
	CAN_INSPECTION_STAT_IDLE = 0,			/* アイドル								*/
	CAN_INSPECTION_STAT_START,				/* 検査開始								*/
	CAN_INSPECTION_STAT_INSPECT				/* 検査中								*/
} E_CAN_INSPECTION_STAT;

/* CAN制御状態 */
typedef enum {
	CAN_STAT_IDLE = 0,					/* アイドル								*/
	CAN_STAT_PRE_WORKING,				/* 動作準備中							*/
	CAN_STAT_INSPECTION,				/* 検査動作中							*/
	CAN_STAT_WORKING,					/* 通常動作中							*/
	CAN_STAT_ERROR						/* エラー状態							*/
} E_CAN_STAT;

/* CANタイプ */
typedef enum {
	CAN_TYPE_TCAN = 0,					/* T-CAN								*/
	CAN_TYPE_ISO						/* ISO CAN								*/
} E_CAN_TYPE;

/********************************************************/
/*	構造体定義											*/
/********************************************************/
typedef struct t_can_inspection_info {
	UI_8						reqflg;
	UI_8						msg_cnt;
	UI_32						start_time;
	E_CAN_INSPECTION_CAN_CH		ch;
} T_CAN_INSPECTION_INFO;

/********************************************************/
/*	内部変数定義										*/
/********************************************************/
CAN_CTRL_LOCAL E_CAN_STAT           S_can_stat  = CAN_STAT_IDLE;	/* CAN制御状態					*/	
CAN_CTRL_LOCAL T_Can_ctrl_tcan_info S_Can_ctrl_tcan_info;			/* 外部提供用T-CAN車両情報		*/
CAN_CTRL_LOCAL T_Can_ctrl_iso_can_info S_Can_ctrl_iso_can_info;		/* 外部提供用ISO CAN車両情報	*/
#ifdef ISOCAN_ENABLE
CAN_CTRL_LOCAL E_CAN_TYPE			S_can_type;
#endif /* ISOCAN_ENABLE */
CAN_CTRL_LOCAL T_CAN_INSPECTION_INFO	S_can_inspection_info;		/* CAN検査情報				*/

/********************************************************/
/*	内部関数											*/
/********************************************************/
CAN_CTRL_LOCAL void proc_for_idle(void);
CAN_CTRL_LOCAL void proc_for_pre_working(void);
CAN_CTRL_LOCAL void proc_for_inspection(void);
CAN_CTRL_LOCAL void proc_for_working(void);
CAN_CTRL_LOCAL void change_stat(E_CAN_STAT can_stat);
CAN_CTRL_LOCAL void readEepData(void);
CAN_CTRL_LOCAL void writeEepData(void);

CAN_CTRL_LOCAL void update_tcan_info_all(void);
CAN_CTRL_LOCAL void update_tcan_info_086A(void);
CAN_CTRL_LOCAL void update_tcan_info_id_152(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_374(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_282(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_140(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_141(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_144(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_360(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_361(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_148(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_368(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_0d0(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_0d1(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_0d3(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_0d4(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_44d(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_375(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_calc(T_Can_ctrl_tcan_info *tcan_info);

CAN_CTRL_LOCAL void update_tcan_info_086B(void);
CAN_CTRL_LOCAL void update_tcan_info_id_040(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_048(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_139(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_13A(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_13B(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_13C(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_138(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_146(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_241(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_345(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_390(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_3AC(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_34A(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_652(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_68C(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_6E2(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_328(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_041(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_04B(T_Can_ctrl_tcan_info *tcan_info);

#ifdef ISOCAN_ENABLE
CAN_CTRL_LOCAL void update_iso_can_info_all(void);
#endif /* ISOCAN_ENABLE */

CAN_CTRL_LOCAL B_8 receive_tcan_data(void);

CAN_CTRL_LOCAL void can_init_inspection_info(void);
CAN_CTRL_LOCAL void can_inspect_can0(void);
CAN_CTRL_LOCAL void can_inspect_can1(void);
CAN_CTRL_LOCAL void can_inspect_stop(void);

/********************************************************/
/* 関数名称		: Can_control_Init						*/
/* 機能の説明	: CAN制御初期化							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Can_control_Init(void)
{
	/* T-CAN車両情報初期化 */
	(void)memset(&S_Can_ctrl_tcan_info, 0x00, sizeof(S_Can_ctrl_tcan_info));
	
	/* ISO-CAN車両情報初期化 */
	(void)memset(&S_Can_ctrl_iso_can_info, 0x00, sizeof(S_Can_ctrl_iso_can_info));


	/* CAN検査情報初期化	*/
	can_init_inspection_info();
}

/********************************************************/
/* 関数名称		: Can_control_Main						*/
/* 機能の説明	: CAN制御メイン							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Can_control_Main(void)
{
	switch(S_can_stat) {
	case CAN_STAT_IDLE:
		/* アイドル状態での処理 */
		proc_for_idle();
		break;
	case CAN_STAT_PRE_WORKING:
		/* 動作準備中の処理 */
		proc_for_pre_working();
		break;
	case CAN_STAT_INSPECTION:
		/* 検査動作中の処理 */
		proc_for_inspection();
		break;
	case CAN_STAT_WORKING:
		/* 通常動作中の処理 */
		proc_for_working();
		break;
	default:
		break;
	}
}


/********************************************************/
/* 関数名称		: Can_control_RcvInspectionCmd			*/
/* 機能の説明	: 検査コマンド受信						*/
/* 引数			: (I/ )cmdinfo	検査コマンド			*/
/* 戻り値		: N_OK		コマンドを受信した			*/
/* 				: N_NG		コマンド未受信またはエラー	*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Can_control_RcvInspectionCmd(T_Can_inspection_cmd_info *cmdinfo)
{
	T_Can_data	candata;
	E_RC		ret;

	if (S_can_stat != CAN_STAT_INSPECTION) {
		/* CAN状態が「検査動作」以外	*/
		return N_NG;
	}

	if (cmdinfo == NULL) {
		return N_NG;
	}

	ret = Can_drv_get_rx_data(CAN_CH_1, &candata);
	if (ret != E_RC_OK) {
		/* メッセージ未受信	*/
		return N_NG;
	}
	if (candata.id != CAN_INSPECTION_RCV_CMD_CAN_ID) {
		return N_NG;
	}

	cmdinfo->cmd	= candata.data[0];
	cmdinfo->result	= candata.data[1];
	cmdinfo->data1	= candata.data[2];
	cmdinfo->data2	= candata.data[3];
	cmdinfo->data3	= Common_Get32(&candata.data[4]);

	return N_OK;
}

/********************************************************/
/* 関数名称		: Can_control_SndInspectionCmd			*/
/* 機能の説明	: 検査コマンド送信						*/
/* 引数			: (I/ )cmdinfo	検査コマンド			*/
/* 戻り値		: N_OK		正常						*/
/* 				: N_NG		異常						*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Can_control_SndInspectionCmd(const T_Can_inspection_cmd_info *cmdinfo)
{
	UI_8  candata[8];
	E_RC  ret;

	if (S_can_stat != CAN_STAT_INSPECTION) {
		/* CAN状態が「検査動作」以外	*/
		return N_NG;
	}

	if (cmdinfo == NULL) {
		return N_NG;
	}

	candata[0] = cmdinfo->cmd;	
	candata[1] = cmdinfo->result;
	candata[2] = cmdinfo->data1;
	candata[3] = cmdinfo->data2;
	Common_Set32( &candata[4], cmdinfo->data3 );

	ret = Can_drv_set_tx_data(CAN_CH_1, 1, CAN_INSPECTION_SND_CMD_CAN_ID, candata, 8);
	if (ret != E_RC_OK) {
		return N_NG;
	}

	return N_OK;
}

/****************************************************************************/
/* 関数名称		: Can_control_ReqInspection									*/
/* 機能の説明	: CAN検査要求												*/
/* 引数			: ch	CAN CH												*/
/* 戻り値		: N_OK		正常											*/
/* 				: N_NG		異常											*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 17/01/20													*/
/* 備考			: 本関数が正常終了した場合、検査制御モジュールに対して		*/
/*				   Insp_notify_inspection_resultにより検査結果を通知する	*/
/****************************************************************************/
SI_8 Can_control_ReqInspection(E_CAN_INSPECTION_CAN_CH ch)
{
	if ((ch != CAN_INSPECTION_CAN_CH0) &&
		(ch != CAN_INSPECTION_CAN_CH1)) {				/* CAN CH不正	*/
		return N_NG;
	}

	if (S_can_stat != CAN_STAT_INSPECTION) {		/* CAN状態が「検査動作」以外	*/
		return N_NG;
	}

	if (S_can_inspection_info.reqflg == N_ON) {		/* CAN検査中	*/
		return N_NG;
	}

	/* 検査情報セット	*/
	can_init_inspection_info();
	S_can_inspection_info.ch = ch;
	S_can_inspection_info.start_time = Timer_ctrl_GetTime1ms();
	S_can_inspection_info.reqflg = N_ON;

	return N_OK;
}

/********************************************************/
/* 関数名称		: proc_for_idle							*/
/* 機能の説明	: アイドル状態時定期処理				*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void proc_for_idle(void)
{
	E_SYS_CTRL_STATUS sys_ctrl_status;
	
	sys_ctrl_status = System_control_GetStatus();

	switch(sys_ctrl_status) {
	case SYS_OPERATION_WAIT_ACC_ON:
		/* ACC ON待ち状態なら何もしない */
		break;
	case SYS_OPERATION_ACC_ON_UNKNOWN_STAT:
		/* ACC ON（状態未確定）なら動作準備中へ遷移 */
		change_stat(CAN_STAT_PRE_WORKING);
		break;
	default:
		break;
	}
}

/********************************************************/
/* 関数名称		: change_stat							*/
/* 機能の説明	: 状態遷移処理							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/*   状態値の更新と共に処理フェーズカウンタのリセットを	*/
/*	 行う。												*/
/********************************************************/
CAN_CTRL_LOCAL void change_stat(E_CAN_STAT can_stat)
{
#ifdef ISOCAN_ENABLE
	UI_8	port_data;
#endif /* ISOCAN_ENABLE */
	UI_32	msg_id;
	UI_8	i;

	S_can_stat  = can_stat;

	switch(S_can_stat) {
	case CAN_STAT_PRE_WORKING:
		(void)System_control_NotifyEvent(SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_OK_CAN);
		break;

	case CAN_STAT_INSPECTION:
		/* CAN検査情報初期化	*/
		can_init_inspection_info();

		/* CANドライバ CH0 初期化*/
		Can_drv_ch_init(CAN_CH_0, CAN_INSPECTION_SEND_BUFFER_COUNT);
		for (i = 0; i < 6; i++) {
			if (i < 3) {
				msg_id = 0x7FF;			/* 11bitメッセージID */
			}
			else {
				msg_id = 0x1FFFFFFFUL;	/* 29bitメッセージID */
			}
			Can_drv_change_message_buffer_mask(CAN_CH_0, (CAN_INSPECTION_MSG_ID_RX_MIN + i), msg_id, 0);
		}

		/* CANドライバ CH1 初期化*/
		Can_drv_ch_init(CAN_CH_1, CAN_INSPECTION_SEND_BUFFER_COUNT);
		Can_drv_change_message_buffer(CAN_CH_1, CAN_INSPECTION_MSG_ID_RX_MIN, CAN_INSPECTION_RCV_CMD_CAN_ID);

		break;

	case CAN_STAT_WORKING:
#ifdef ISOCAN_ENABLE
		PortDrv_GetDataBit(IO_PI_GR_CAN_SW, IO_PI_BIT_CAN_SW, &port_data);
		if (port_data == IO_PI_LEV_CAN_SW_ISOCAN) {
			/* ISO CANが有効 */
			S_can_type = CAN_TYPE_ISO;

			/* ISO CANコントローラ初期化*/
			Iso_can_ctrl_Init();
		}
		else {
			/* T-CANが有効 */
			S_can_type = CAN_TYPE_TCAN;

			/* T-CAN用CANチャンネル初期化 */
			Can_drv_ch_init(CAN_CH_TCAN, 0);

			/* T-CANコントローラ初期化 */
			TCAN_Ctrl_Init();
		}
#else /* ISOCAN_ENABLE  */
		/* T-CAN用CANチャンネル初期化 */
		Can_drv_ch_init(CAN_CH_TCAN, 0);

		/* T-CANコントローラ初期化 */
		TCAN_Ctrl_Init();
#endif /* ISOCAN_ENABLE  */

		/* Local CANコントローラ初期化 */
		Local_can_ctrl_Init();
		/*不揮発メモリから車両情報未読み込みチェック*/
		if(S_Can_ctrl_tcan_info.en_eep_vehicle_info == 0)
		{
			/*未読み込みなら不揮発メモリからデータ読み込み*/
			readEepData();
			/*車種情報に応じたCANメッセージのセットアップ*/
			TCAN_Ctrl_SetupCanMessage((E_TCAN_VEHICLE_INFO)S_Can_ctrl_tcan_info.eep_vehicle_info);
		}
		break;

	case CAN_STAT_ERROR:
		(void)System_control_NotifyEvent(SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_ATT_NG_CAN);
		break;

	default:
		break;
	}
}

/********************************************************/
/* 関数名称		: proc_for_pre_working					*/
/* 機能の説明	: 動作準備中状態時定期処理				*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void proc_for_pre_working(void)
{
	E_SYS_CTRL_STATUS sys_ctrl_status;
	
	sys_ctrl_status = System_control_GetStatus();

	switch(sys_ctrl_status) {
	case SYS_OPERATION_PRE_ACC_OFF:
		change_stat(CAN_STAT_IDLE);
		break;

	case SYS_OPERATION_NORMAL_OFF:
	case SYS_OPERATION_NORMAL_AUTO:
	case SYS_OPERATION_NORMAL_MANUAL:
	case SYS_OPERATION_SIMULATION:
		change_stat(CAN_STAT_WORKING);
		break;

	case SYS_OPERATION_INSPECTION:
		change_stat(CAN_STAT_INSPECTION);
		break;

	default:
		break;
	}
}

/********************************************************/
/* 関数名称		: proc_for_inspection					*/
/* 機能の説明	: 検査動作中処理						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void proc_for_inspection(void)
{
	E_SYS_CTRL_STATUS sys_ctrl_status;

	sys_ctrl_status = System_control_GetStatus();
	switch(sys_ctrl_status) {
	case SYS_OPERATION_INSPECTION:
		if (S_can_inspection_info.reqflg == N_ON) {
			if (S_can_inspection_info.ch == CAN_INSPECTION_CAN_CH0) {
				can_inspect_can0();
			}
			else {
				can_inspect_can1();
			}
		}
		break;

	case SYS_OPERATION_INSPECTION_COMP:
	case SYS_OPERATION_PRE_ACC_OFF:
		if (S_can_inspection_info.reqflg == N_ON) {
			can_inspect_stop();
		}
		change_stat(CAN_STAT_IDLE);
		break;
	
	default:
		break;
	}

}

/********************************************************/
/* 関数名称		: proc_for_working						*/
/* 機能の説明	: CAN制御通常動作中処理					*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)katsura							*/
/* 作成日		: 16/09/28								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void proc_for_working(void)
{
	E_SYS_CTRL_STATUS sys_ctrl_status;

	sys_ctrl_status = System_control_GetStatus();
	if (sys_ctrl_status == SYS_OPERATION_PRE_ACC_OFF) {
		/* ACC OFF移行ならアイドルへ遷移 */
		change_stat(CAN_STAT_IDLE);
	}

#ifdef ISOCAN_ENABLE
	if (S_can_type == CAN_TYPE_ISO) {
		/* ISO CANコントローラメイン処理コール */
		Iso_can_ctrl_Main();

		/* ISO-CANカレント情報の更新 */
		update_iso_can_info_all();
	}
	else {
#endif /* ISOCAN_ENABLE */

		/* T-CANデータ受信 */
		(void)receive_tcan_data();

		/* T-CANコントローラメイン処理コール */
		TCAN_Ctrl_Main();

		/* T-CANカレント情報の更新 */
		update_tcan_info_all();
#ifdef ISOCAN_ENABLE
	}
#endif /* ISOCAN_ENABLE */

	/* Local CANコントローラメイン処理コール */
	Local_can_ctrl_Main();

}

/********************************************************/
/* 関数名称		: Can_control_GetTCanInfo				*/
/* 機能の説明	: T-CAN情報カレント値を取得する			*/
/* 引数			: void									*/
/* 戻り値		: T-CAN情報カレント値格納領域ptr		*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
T_Can_ctrl_tcan_info *Can_control_GetTCanInfo(void)
{
	return(&S_Can_ctrl_tcan_info);
}

/********************************************************/
/* 関数名称		: Can_control_GetIsoCanInfo				*/
/* 機能の説明	: ISO-CAN情報カレント値を取得する		*/
/* 引数			: void									*/
/* 戻り値		: ISO-CAN情報カレント値格納領域ptr		*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/11/08								*/
/* 備考			: 										*/
/********************************************************/
T_Can_ctrl_iso_can_info *Can_control_GetIsoCanInfo(void)
{
	return(&S_Can_ctrl_iso_can_info);
}

/********************************************************/
/* 関数名称		: Can_control_GetVehicleSpeed			*/
/* 機能の説明	: 車速カレント値を取得する				*/
/* 引数			: (O)vehicle_speed 車速カレント値		*/
/* 戻り値		: 取得結果								*/
/*				   E_RC_OK:取得成功, E_RC_NG:取得不可	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/*   返される車速は、0.00001km/hを1単位とする値。		*/
/********************************************************/
E_RC Can_control_GetVehicleSpeed(UI_32 *vehicle_speed)
{
	E_TCAN_RESULT result;
	
	result = TCAN_Ctrl_GetVehicleSpeed(vehicle_speed);
	if(result != TCAN_CTRL_OK) {
		return E_RC_NG;
	}
	else {
		return E_RC_OK;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_all					*/
/* 機能の説明	: T-CANカレント情報を更新する			*/
/* 引数			: なし									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_all(void)
{
	E_TCAN_VEHICLE_INFO vehicleInfo; 
	/*車種情報取得*/
	vehicleInfo = TCAN_Ctrl_GetVehicleInfo();

	if ( (S_Can_ctrl_tcan_info.en_eep_vehicle_info == 1) &&
		 (S_Can_ctrl_tcan_info.eep_vehicle_info != (UI_16)vehicleInfo) ){
		/*EEPROM保持値と変化あり*/
#ifdef CAN_DEBUG
		OUTLOG(OutputLog( "[CAN] @@@@@@@@@ VehileInfo Change (%04x) @@@@@@@@ \n", vehicleInfo );)
#endif /*CAN_DEBUG*/
		/* T-CAN用CANチャンネル初期化 */
		Can_drv_ch_init(CAN_CH_TCAN, 0);
		/* T-CANコントローラ初期化 */
		TCAN_Ctrl_Init();
		/*車種情報に応じたCANメッセージのセットアップ*/
		TCAN_Ctrl_SetupCanMessage(vehicleInfo);
		/*EEPROM保持値更新*/
		S_Can_ctrl_tcan_info.eep_vehicle_info = (UI_16)vehicleInfo;
		/*不揮発メモリへデータ書き込み*/
		writeEepData();
	}

	/*車両種別更新*/
	S_Can_ctrl_tcan_info.en_vehicle_info = 1;
	/*車両種別チェック*/
	switch (vehicleInfo) {
	case TCAN_VEHICLE_INFO_86A:
		/*086A*/
		S_Can_ctrl_tcan_info.vehicle_info = CAN_VEHICLEINFO_086A;
		update_tcan_info_086A();
		break;
	case TCAN_VEHICLE_INFO_86B:
		/*086B*/
		S_Can_ctrl_tcan_info.vehicle_info = CAN_VEHICLEINFO_086B;
		update_tcan_info_086B();
		break;
	default:
		/*不明*/
		S_Can_ctrl_tcan_info.vehicle_info = CAN_VEHICLEINFO_UNKNOWN;
		break;
	}

}

/********************************************************/
/* 関数名称		: update_tcan_info_086A					*/
/* 機能の説明	: T-CANカレント情報を更新する			*/
/* 引数			: なし									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_086A(void)
{
	/* ID=152情報更新 */
	update_tcan_info_id_152(&S_Can_ctrl_tcan_info);

	/* ID=374情報更新 */
	update_tcan_info_id_374(&S_Can_ctrl_tcan_info);

	/* ID=282情報更新 */
	update_tcan_info_id_282(&S_Can_ctrl_tcan_info);

	/* ID=140情報更新 */
	update_tcan_info_id_140(&S_Can_ctrl_tcan_info);

	/* ID=141情報更新 */
	update_tcan_info_id_141(&S_Can_ctrl_tcan_info);

	/* ID=144情報更新 */
	update_tcan_info_id_144(&S_Can_ctrl_tcan_info);

	/* ID=360情報更新 */
	update_tcan_info_id_360(&S_Can_ctrl_tcan_info);

	/* ID=361情報更新 */
	update_tcan_info_id_361(&S_Can_ctrl_tcan_info);

	/* ID=148情報更新 */
	update_tcan_info_id_148(&S_Can_ctrl_tcan_info);

	/* ID=368情報更新 */
	update_tcan_info_id_368(&S_Can_ctrl_tcan_info);

	/* ID=0D0情報更新 */
	update_tcan_info_id_0d0(&S_Can_ctrl_tcan_info);
	
	/* ID=0D1情報更新 */
	update_tcan_info_id_0d1(&S_Can_ctrl_tcan_info);

	/* ID=0D3情報更新 */
	update_tcan_info_id_0d3(&S_Can_ctrl_tcan_info);

	/* ID=0D4情報更新 */
	update_tcan_info_id_0d4(&S_Can_ctrl_tcan_info);

	/* ID=44D情報更新 */
	update_tcan_info_id_44d(&S_Can_ctrl_tcan_info);

	/* ID=375情報更新 */
	update_tcan_info_id_375(&S_Can_ctrl_tcan_info);

	/* 内部算出情報更新 */
	update_tcan_info_calc(&S_Can_ctrl_tcan_info);
}


/********************************************************/
/* 関数名称		: update_tcan_info_086B					*/
/* 機能の説明	: T-CANカレント情報を更新する			*/
/* 引数			: なし									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_086B(void)
{

	/* ID=040情報更新 */
	update_tcan_info_id_040(&S_Can_ctrl_tcan_info);

	/* ID=048情報更新 */
	update_tcan_info_id_048(&S_Can_ctrl_tcan_info);

	/* ID=139情報更新 */
	update_tcan_info_id_139(&S_Can_ctrl_tcan_info);

	/* ID=13A情報更新 */
	update_tcan_info_id_13A(&S_Can_ctrl_tcan_info);

	/* ID=13B情報更新 */
	update_tcan_info_id_13B(&S_Can_ctrl_tcan_info);

	/* ID=13C情報更新 */
	update_tcan_info_id_13C(&S_Can_ctrl_tcan_info);

	/* ID=138情報更新 */
	update_tcan_info_id_138(&S_Can_ctrl_tcan_info);

	/* ID=146情報更新 */
	update_tcan_info_id_146(&S_Can_ctrl_tcan_info);

	/* ID=241情報更新 */
	update_tcan_info_id_241(&S_Can_ctrl_tcan_info);

	/* ID=345情報更新 */
	update_tcan_info_id_345(&S_Can_ctrl_tcan_info);

	/* ID=390情報更新 */
	update_tcan_info_id_390(&S_Can_ctrl_tcan_info);

	/* ID=3AC情報更新 */
	update_tcan_info_id_3AC(&S_Can_ctrl_tcan_info);

	/* ID=34A情報更新 */
	update_tcan_info_id_34A(&S_Can_ctrl_tcan_info);

	/* ID=652情報更新 */
	update_tcan_info_id_652(&S_Can_ctrl_tcan_info);

	/* ID=68C情報更新 */
	update_tcan_info_id_68C(&S_Can_ctrl_tcan_info);

	/* ID=6E2情報更新 */
	update_tcan_info_id_6E2(&S_Can_ctrl_tcan_info);

	/* ID=328情報更新 */
	update_tcan_info_id_328(&S_Can_ctrl_tcan_info);

	/* ID=041情報更新 */
	update_tcan_info_id_041(&S_Can_ctrl_tcan_info);

	/* ID=04B情報更新 */
	update_tcan_info_id_04B(&S_Can_ctrl_tcan_info);

	/* 内部算出情報更新 */
	update_tcan_info_calc(&S_Can_ctrl_tcan_info);

}


/********************************************************/
/* 関数名称		: update_tcan_info_id_040				*/
/* 機能の説明	: T-CAN ID=040のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_040(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* アクセル開度情報取得 */
	result = TCAN_Ctrl_GetMTRatioAccel(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->accel_ratio    = data_8;
		tcan_info->en_accel_ratio = 1;
	}
	else {
		tcan_info->en_accel_ratio = 0;
	}

	/* エンジン回転数情報取得 */
	result = TCAN_Ctrl_GetMTEngRpm(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->eng_rpm    = data_16;
		tcan_info->en_eng_rpm = 1;
	}
	else {
		tcan_info->en_eng_rpm = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_048				*/
/* 機能の説明	: T-CAN ID=048のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_048(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* スポーツシフトギア位置表示情報取得 */
	result = TCAN_Ctrl_GetMTSportGearPosition(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->sports_gear_pos    = (data_8 & 0x0f);
		tcan_info->en_sports_gear_pos = 1;
	}
	else {
		tcan_info->en_sports_gear_pos = 0;
	}

	/* シフト位置情報(フェールセーフ処理後)情報取得 */
	result = TCAN_Ctrl_GetMTShiftPosition(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->shift_pos    = (data_8 & 0x07);
		tcan_info->en_shift_pos = 1;
	}
	else {
		tcan_info->en_shift_pos = 0;
	}

	/* マニュアルモードSW情報取得 */
	result = TCAN_Ctrl_GetAtManualModeSW(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->manual_mode_sw    = (data_8 & 0x01);
		tcan_info->en_manual_mode_sw = 1;
	}
	else {
		tcan_info->en_manual_mode_sw = 0;
	}

}




/********************************************************/
/* 関数名称		: update_tcan_info_id_146				*/
/* 機能の説明	: T-CAN ID=146のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_146(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* エンジン回転数タコメータ用情報取得 */
	result = TCAN_Ctrl_GetMTEngRpmTa(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->eng_rpm_taco    = data_16;
		tcan_info->en_eng_rpm_taco = 1;
	}
	else {
		tcan_info->en_eng_rpm_taco = 0;
	}
}


/********************************************************/
/* 関数名称		: update_tcan_info_id_68C				*/
/* 機能の説明	: T-CAN ID=68Cのカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_68C(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* ブランドフラグ情報取得 */
	result = TCAN_Ctrl_GetMTBrandFlag(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->brand_flag    = (data & 0x01);
		tcan_info->en_brand_flag = 1;
	}
	else {
		tcan_info->en_brand_flag = 0;
	}

}

/********************************************************/
/* 関数名称		: update_tcan_info_id_241				*/
/* 機能の説明	: T-CAN ID=241のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_241(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* MTギアポジション表示情報取得 */
	result = TCAN_Ctrl_GetMTGearEstm(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->mt_gear_pos    = (data_8 & 0x07);
		tcan_info->en_mt_gear_pos = 1;
	}
	else {
		tcan_info->en_mt_gear_pos = 0;
	}

	/* クルコンセットランプ情報取得 */
	result = TCAN_Ctrl_GetMTSetCc(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->cruise_controlrl_set    = (data_8 & 0x01);
		tcan_info->en_cruise_controlrl_set = 1;
	}
	else {
		tcan_info->en_cruise_controlrl_set = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_34A				*/
/* 機能の説明	: T-CAN ID=34Aのカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_34A(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;


	/* スポーツランプ情報取得 */
	result = TCAN_Ctrl_GetMTSportsLamp(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->sports_lamp    = (data_8 & 0x01);
		tcan_info->en_sports_lamp = 1;
	}
	else {
		tcan_info->en_sports_lamp = 0;
	}

	/* スノーホールドランプ情報取得 */
	result = TCAN_Ctrl_GetMTSnowHoldLamp(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->snow_hold_lamp    = (data_8 & 0x01);
		tcan_info->en_snow_hold_lamp = 1;
	}
	else {
		tcan_info->en_snow_hold_lamp = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_139				*/
/* 機能の説明	: T-CAN ID=139のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_139(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16         data_16;
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* 駆動輪平均車輪速情報取得 */
	result = TCAN_Ctrl_GetMTSpWheelAve(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->wheel_speed_ave    = data_16;
		tcan_info->en_wheel_speed_ave = 1;
	}
	else {
		tcan_info->en_wheel_speed_ave = 0;
	}

	/* ブレーキ圧情報取得 */
	result = TCAN_Ctrl_GetMtBrakePressure(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->brake_pressure    = data_8;
		tcan_info->en_brake_pressure = 1;
	}
	else {
		tcan_info->en_brake_pressure = 0;
	}

	/* TRC作動状態情報取得 */
	result = TCAN_Ctrl_GetAtTrcAct(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->trcAct    = (data_8 & 0x01);
		tcan_info->en_trcAct = 1;
	}
	else {
		tcan_info->en_trcAct = 0;
	}
	/* ABS作動状態情報取得 				*/
	result = TCAN_Ctrl_GetAtAbsAct(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->absAct    = (data_8 & 0x01);
		tcan_info->en_absAct = 1;
	}
	else {
		tcan_info->en_absAct = 0;
	}
	
	/* ABSシステムフェイル情報取得 		*/
	result = TCAN_Ctrl_GetAtAbsFail(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->absFail    = (data_8 & 0x01);
		tcan_info->en_absFail = 1;
	}
	else {
		tcan_info->en_absFail = 0;
	}
	
	/* VSCアンダーステア状態 */
	result = TCAN_Ctrl_GetAtVscUnderStr(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->vscUnderStr    = (data_8 & 0x01);
		tcan_info->en_vscUnderStr = 1;
	}
	else {
		tcan_info->en_vscUnderStr = 0;
	}

	/* VSCオーバーステア状態 */
	result = TCAN_Ctrl_GetAtVscOverStr(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->vscOverStr    = (data_8 & 0x01);
		tcan_info->en_vscOverStr = 1;
	}
	else {
		tcan_info->en_vscOverStr = 0;
	}

}

/********************************************************/
/* 関数名称		: update_tcan_info_id_13C				*/
/* 機能の説明	: T-CAN ID=13Cのカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_13C(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* 車速パルス積算値情報取得 */
	result = TCAN_Ctrl_GetMTSpPulseInteg(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_pulse_integ    = (data_8 & 0x3f);
		tcan_info->en_speed_pulse_integ = 1;
	}
	else {
		tcan_info->en_speed_pulse_integ = 0;
	}
}


/********************************************************/
/* 関数名称		: update_tcan_info_id_3AC				*/
/* 機能の説明	: T-CAN ID=3ACのカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_3AC(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* IGN情報取得 */
	result = TCAN_Ctrl_GetMTIgn(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->ign    = (data & 0x01);
		tcan_info->en_ign = 1;
	}
	else {
		tcan_info->en_ign = 0;
	}
	
	/* ACC情報取得 */
	result= TCAN_Ctrl_GetMTAcc(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->acc    = (data & 0x01);
		tcan_info->en_acc = 1;
	}
	else {
		tcan_info->en_acc = 0;
	}
	
	/* ブレーキランプSW情報取得 */
	result= TCAN_Ctrl_GetMTBreakLamp(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->brake_lamp_sw     = (data & 0x01);
		tcan_info->en_brake_lamp_sw  = 1;
	}
	else {
		tcan_info->en_brake_lamp_sw = 0;
	}

	/* パーキングランプSW情報取得 */
	result= TCAN_Ctrl_GetMTHandBreak(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->hand_brake_sw    = (data & 0x01);
		tcan_info->en_hand_brake_sw = 1;
	}
	else {
		tcan_info->en_hand_brake_sw = 0;
	}

	/* MTリバースSW情報取得 */
	result= TCAN_Ctrl_GetMTReverse(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->mt_rev_sw    = (data & 0x01);
		tcan_info->en_mt_rev_sw = 1;
	}
	else {
		tcan_info->en_mt_rev_sw = 0;
	}

	/* ヘッドランプ Hiビーム情報取得 */
	result= TCAN_Ctrl_GetMTHeadLampHi(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->head_lamp_hi    = (data & 0x01);
		tcan_info->en_head_lamp_hi = 1;
	}
	else {
		tcan_info->en_head_lamp_hi = 0;
	}

	/* ヘッドランプ Loビーム情報取得 */
	result= TCAN_Ctrl_GetMTHeadLampLo(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->head_lamp_lo    = (data & 0x01);
		tcan_info->en_head_lamp_lo = 1;
	}
	else {
		tcan_info->en_head_lamp_lo = 0;
	}

	/* ヘッドランプ スモール情報取得 */
	result= TCAN_Ctrl_GetMTHeadLampSmall(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->head_lamp_small    = (data & 0x01);
		tcan_info->en_head_lamp_small = 1;
	}
	else {
		tcan_info->en_head_lamp_small = 0;
	}
	
	/* ドアSW トランク情報取得 */
	result = TCAN_Ctrl_GetMTTrunkDoorOpen(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->door_sw_trunk    = (data & 0x01);
		tcan_info->en_door_sw_trunk = 1;
	}
	else {
		tcan_info->en_door_sw_trunk = 0;
	}

	/* ドアSW P席情報取得 */
	result = TCAN_Ctrl_GetMTPDoorOpen(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->door_sw_seat_p    = (data & 0x01);
		tcan_info->en_door_sw_seat_p = 1;
	}
	else {
		tcan_info->en_door_sw_seat_p = 0;
	}

	/* ドアSW D席情報取得 */
	result = TCAN_Ctrl_GetMTDoorOpen(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->door_sw_seat_d    = (data & 0x01);
		tcan_info->en_door_sw_seat_d = 1;
	}
	else {
		tcan_info->en_door_sw_seat_d = 0;
	}

	/* ドアSW フード情報取得 */
	result = TCAN_Ctrl_GetMtHoodDoorSW(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->door_sw_hood    = (data & 0x01);
		tcan_info->en_door_sw_hood = 1;
	}
	else {
		tcan_info->en_door_sw_hood = 0;
	}
}


/********************************************************/
/* 関数名称		: update_tcan_info_id_345				*/
/* 機能の説明	: T-CAN ID=345のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_345(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* トランスミッション情報取得 */
	result = TCAN_Ctrl_GetMTTransmissionType(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->transmission_type    = (data & 0x01);
		tcan_info->en_transmission_type = 1;
	}
	else {
		tcan_info->en_transmission_type = 0;
	}
	
	/* エンジン油温情報取得 */
	result = TCAN_Ctrl_GetMTTempOil(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->temp_oil    = data;
		tcan_info->en_temp_oil = 1;
	}
	else {
		tcan_info->en_temp_oil = 0;
	}

	/* エンジン水温情報取得 */
	result = TCAN_Ctrl_GetMTTempCoolant(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->temp_coolant    = data;
		tcan_info->en_temp_coolant = 1;
	}
	else {
		tcan_info->en_temp_coolant = 0;
	}
	
	/* 吸入管圧力情報取得 */
	result = TCAN_Ctrl_GetAtIntakePress(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->intake_press    = data;
		tcan_info->en_intake_press = 1;
	}
	else {
		tcan_info->en_intake_press = 0;
	}

	/* 大気圧情報取得 */
	result = TCAN_Ctrl_GetAtAtmosphericPress(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->atmospheric_press    = data;
		tcan_info->en_atmospheric_press = 1;
	}
	else {
		tcan_info->en_atmospheric_press = 0;
	}
	
}


/********************************************************/
/* 関数名称		: update_tcan_info_id_390				*/
/* 機能の説明	: T-CAN ID=390のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_390(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* 表示外気温情報取得 */
	result = TCAN_Ctrl_GetMTOutTemp(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->out_temp    = data;
		tcan_info->en_out_temp = 1;
	}
	else {
		tcan_info->en_out_temp = 0;
	}

	/* シートベルトSW P席情報取得 */
	result = TCAN_Ctrl_GetMTPSeatBelt(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->seat_belt_sw_p    = (data & 0x01);
		tcan_info->en_seat_belt_sw_p = 1;
	}
	else {
		tcan_info->en_seat_belt_sw_p = 0;
	}

	/* シートベルトSW D席情報取得 */
	result = TCAN_Ctrl_GetMTDSeatBelt(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->seat_belt_sw_d    = (data & 0x01);
		tcan_info->en_seat_belt_sw_d = 1;
	}
	else {
		tcan_info->en_seat_belt_sw_d = 0;
	}

	/* 仕向地情報取得 */
	result = TCAN_Ctrl_GetMTPlace(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->destination    = (data & 0x0f);
		tcan_info->en_destination = 1;
	}
	else {
		tcan_info->en_destination = 0;
	}

	/* ウィンカー左情報取得 */
	result = TCAN_Ctrl_GetAtBlinkerLeft(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->blinker_left    = (data & 0x0f);
		tcan_info->en_blinker_left = 1;
	}
	else {
		tcan_info->en_blinker_left = 0;
	}

	/* ウィンカー右情報取得 */
	result = TCAN_Ctrl_GetAtBlinkerRight(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->blinker_right    = (data & 0x0f);
		tcan_info->en_blinker_right = 1;
	}
	else {
		tcan_info->en_blinker_right = 0;
	}

}


/********************************************************/
/* 関数名称		: update_tcan_info_id_138				*/
/* 機能の説明	: T-CAN ID=138のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_138(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* 舵角センサ情報取得 */
	result = TCAN_Ctrl_GetMTSteerAngle(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->steer_angle    = data_16;
		tcan_info->en_steer_angle = 1;
	}
	else {
		tcan_info->en_steer_angle = 0;
	}

	/* ヨーレート情報取得 */
	result = TCAN_Ctrl_GetMTYawRate(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->yaw_rate    = data_16;
		tcan_info->en_yaw_rate = 1;
	}
	else {
		tcan_info->en_yaw_rate = 0;
	}

}

/********************************************************/
/* 関数名称		: update_tcan_info_id_13B				*/
/* 機能の説明	: T-CAN ID=13Bのカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_13B(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16          data_16;
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* 横加速度情報取得 */
	result = TCAN_Ctrl_GetMTAccLR(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->acc_lr    = data_8;
		tcan_info->en_acc_lr = 1;
	}
	else {
		tcan_info->en_acc_lr = 0;
	}

	/* 前後Ｇセンサ情報取得 */
	result = TCAN_Ctrl_GetMTAccFR(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->acc_fr    = data_8;
		tcan_info->en_acc_fr = 1;
	}
	else {
		tcan_info->en_acc_fr = 0;
	}

	/*エンジントルクアップ要求フラグ情報取得*/	
	result = TCAN_Ctrl_GetAtEngTrqUpReqFlg(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->engTrqUpReqFlag    = (data_8 & 0x01);
		tcan_info->en_engTrqUpReqFlag = 1;
	}
	else {
		tcan_info->en_engTrqUpReqFlag = 0;
	}
	
	/*エンジントルク減少要求フラグ情報取得*/	
	result = TCAN_Ctrl_GetAtEngTrqDwnReqFlg(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->engTrqDwnReqFlag    = (data_8 & 0x01);
		tcan_info->en_engTrqDwnReqFlag = 1;
	}
	else {
		tcan_info->en_engTrqDwnReqFlag = 0;
	}

	/*エンジントルクアップ要求値情報取得*/	
	result = TCAN_Ctrl_GetAtEngTrqUpReqVal(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->engTrqUpReqVal    = data_16;
		tcan_info->en_engTrqUpReqVal = 1;
	}
	else {
		tcan_info->en_engTrqUpReqVal = 0;
	}

	/*エンジントルク減少要求値情報取得*/	
	result = TCAN_Ctrl_GetAtEngTrqDwnReqVal(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->engTrqDwnReqVal    = data_16;
		tcan_info->en_engTrqDwnReqVal = 1;
	}
	else {
		tcan_info->en_engTrqDwnReqVal = 0;
	}
}


/********************************************************/
/* 関数名称		: update_tcan_info_id_13A				*/
/* 機能の説明	: T-CAN ID=13Aのカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_13A(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16          data_16;
	E_TCAN_RESULT result;

	/* 左前輪車輪速情報取得 */
	result = TCAN_Ctrl_GetMtSpdWheelFtlh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_ftlh    = data_16;
		tcan_info->en_speed_wheel_ftlh = 1;
	}
	else {
		tcan_info->en_speed_wheel_ftlh = 0;
	}

	/* 右前輪車輪速情報取得 */
	result = TCAN_Ctrl_GetMtSpdWheelFtrh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_ftrh    = data_16;
		tcan_info->en_speed_wheel_ftrh = 1;
	}
	else {
		tcan_info->en_speed_wheel_ftrh = 0;
	}

	/* 左後輪車輪速情報取得 */
	result = TCAN_Ctrl_GetMtSpdWheelRrlh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_rrlh    = data_16;
		tcan_info->en_speed_wheel_rrlh = 1;
	}
	else {
		tcan_info->en_speed_wheel_rrlh = 0;
	}

	/* 右後輪車輪速情報取得 */
	result = TCAN_Ctrl_GetMtSpdWheelRrrh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_rrrh    = data_16;
		tcan_info->en_speed_wheel_rrrh = 1;
	}
	else {
		tcan_info->en_speed_wheel_rrrh = 0;
	}
}


/********************************************************/
/* 関数名称		: update_tcan_info_id_6E2				*/
/* 機能の説明	: T-CAN ID=6E2のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_6E2(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8         data_8;
	E_TCAN_RESULT result;

	/* 左前輪空気圧情報取得 */
	result = TCAN_Ctrl_GetAtAirPressFtlh(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->air_press_ftlh    = data_8;
		tcan_info->en_air_press_ftlh = 1;
	}
	else {
		tcan_info->en_air_press_ftlh = 0;
	}

	/* 右前輪空気圧情報取得 */
	result = TCAN_Ctrl_GetAtAirPressFtrh(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->air_press_ftrh    = data_8;
		tcan_info->en_air_press_ftrh = 1;
	}
	else {
		tcan_info->en_air_press_ftrh = 0;
	}

	/* 左後輪空気圧情報取得 */
	result = TCAN_Ctrl_GetAtAirPressRrlh(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->air_press_rrlh    = data_8;
		tcan_info->en_air_press_rrlh = 1;
	}
	else {
		tcan_info->en_air_press_rrlh = 0;
	}

	/* 右後輪空気圧情報取得 */
	result = TCAN_Ctrl_GetAtAirPressRrrh(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->air_press_rrrh    = data_8;
		tcan_info->en_air_press_rrrh = 1;
	}
	else {
		tcan_info->en_air_press_rrrh = 0;
	}
	
	/* 空気圧単位情報取得 */
	result = TCAN_Ctrl_GetAtAirPressUnit(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->air_press_unit    = data_8;
		tcan_info->en_air_press_unit = 1;
	}
	else {
		tcan_info->en_air_press_unit = 0;
	}	

}


/********************************************************/
/* 関数名称		: update_tcan_info_id_652				*/
/* 機能の説明	: T-CAN ID=652のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/09/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_652(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* ドアロック状態（助手席）情報取得 */
	result = TCAN_Ctrl_GetMtDoorLockStatPass(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->lock_stat_pass    = data_8;
		tcan_info->en_lock_stat_pass = 1;
	}
	else {
		tcan_info->en_lock_stat_pass = 0;
	}

	/* ドアロック状態（運転席）情報取得 */
	result = TCAN_Ctrl_GetMtDoorLockStatDrvr(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->lock_stat_driver    = data_8;
		tcan_info->en_lock_stat_driver = 1;
	}
	else {
		tcan_info->en_lock_stat_driver = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_328				*/
/* 機能の説明	: T-CAN ID=328のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 21/02/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_328(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* TRC ON/OFF情報取得 */
	result = TCAN_Ctrl_GetAtTrcOnOff(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->trcOnOff =  data_8 & 0x01;
		tcan_info->en_trcOnOff = 1;
	}
	else {
		tcan_info->en_trcOnOff = 0;
	}
	/* ECBウォーニング情報取得 			*/
	result = TCAN_Ctrl_GetAtEcbWarning(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->ecbWarning =  data_8 & 0x01;
		tcan_info->en_ecbWarning = 1;
	}
	else {
		tcan_info->en_vscWarning = 0;
	}
	/* VSCウォーニング情報取得 			*/
	result = TCAN_Ctrl_GetAtVscWarning(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->vscWarning =  data_8 & 0x01;
		tcan_info->en_vscWarning = 1;
	}
	else {
		tcan_info->en_vscWarning = 0;
	}

	/* ABSウォーニング情報取得 			*/
	result = TCAN_Ctrl_GetAtAbsWarning(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->absWarning =  data_8 & 0x01;
		tcan_info->en_absWarning = 1;
	}
	else {
		tcan_info->en_absWarning = 0;
	}

	/* VSC ON/OFF情報取得 */
	result = TCAN_Ctrl_GetAtVscOnOff(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->vscOnOff =  data_8 & 0x01;
		tcan_info->en_vscOnOff = 1;
	}
	else {
		tcan_info->en_vscOnOff = 0;
	}

	/* VSC制御状態取得 */
	result = TCAN_Ctrl_GetAtVscAct(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->vscAct    =  data_8 & 0x01;
		tcan_info->en_vscAct = 1;
	}
	else {
		tcan_info->en_vscAct = 0;
	}

	/* トラクション重視モード状態取得 */
	result = TCAN_Ctrl_GetAtTrcMode(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->trcMode    =  data_8 & 0x01;
		tcan_info->en_trcMode = 1;
	}
	else {
		tcan_info->en_trcMode = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_04B				*/
/* 機能の説明	: T-CAN ID=141のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 21/03/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_04B(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* 目標ギア位置情報取得 */
	result = TCAN_Ctrl_GetMTTargetGearPos(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->target_gear_pos = (data_8 & 0x0f);
		tcan_info->en_target_gear_pos = 1;
	}
	else {
		tcan_info->en_target_gear_pos = 0;
	}

}
/********************************************************/
/* 関数名称		: update_tcan_info_id_041				*/
/* 機能の説明	: T-CAN ID=141のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 21/03/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_041(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16          data_16;
	E_TCAN_RESULT result;

	/* 実エンジン軸トルク情報取得 */
	result = TCAN_Ctrl_GetAtEngTrqShaft(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->engTrcShaft = (data_16 & 0x7FFF);
		tcan_info->en_engTrcShaft = 1;
	}
	else {
		tcan_info->en_engTrcShaft = 0;
	}

}

/********************************************************/
/* 関数名称		: update_tcan_info_id_152				*/
/* 機能の説明	: T-CAN ID=152のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_152(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* IGN情報取得 */
	result = TCAN_Ctrl_GetMTIgn(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->ign    = (data & 0x01);
		tcan_info->en_ign = 1;
	}
	else {
		tcan_info->en_ign = 0;
	}
	
	/* ACC情報取得 */
	result= TCAN_Ctrl_GetMTAcc(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->acc    = (data & 0x01);
		tcan_info->en_acc = 1;
	}
	else {
		tcan_info->en_acc = 0;
	}
	
	/* ブレーキランプSW情報取得 */
	result= TCAN_Ctrl_GetMTBreakLamp(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->brake_lamp_sw     = (data & 0x01);
		tcan_info->en_brake_lamp_sw  = 1;
	}
	else {
		tcan_info->en_brake_lamp_sw = 0;
	}

	/* パーキングランプSW情報取得 */
	result= TCAN_Ctrl_GetMTHandBreak(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->hand_brake_sw    = (data & 0x01);
		tcan_info->en_hand_brake_sw = 1;
	}
	else {
		tcan_info->en_hand_brake_sw = 0;
	}

	/* MTリバースSW情報取得 */
	result= TCAN_Ctrl_GetMTReverse(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->mt_rev_sw    = (data & 0x01);
		tcan_info->en_mt_rev_sw = 1;
	}
	else {
		tcan_info->en_mt_rev_sw = 0;
	}

	/* ヘッドランプ Hiビーム情報取得 */
	result= TCAN_Ctrl_GetMTHeadLampHi(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->head_lamp_hi    = (data & 0x01);
		tcan_info->en_head_lamp_hi = 1;
	}
	else {
		tcan_info->en_head_lamp_hi = 0;
	}

	/* ヘッドランプ Loビーム情報取得 */
	result= TCAN_Ctrl_GetMTHeadLampLo(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->head_lamp_lo    = (data & 0x01);
		tcan_info->en_head_lamp_lo = 1;
	}
	else {
		tcan_info->en_head_lamp_lo = 0;
	}

	/* ヘッドランプ スモール情報取得 */
	result= TCAN_Ctrl_GetMTHeadLampSmall(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->head_lamp_small    = (data & 0x01);
		tcan_info->en_head_lamp_small = 1;
	}
	else {
		tcan_info->en_head_lamp_small = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_374				*/
/* 機能の説明	: T-CAN ID=374のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_374(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* ドアSW トランク情報取得 */
	result = TCAN_Ctrl_GetMTTrunkDoorOpen(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->door_sw_trunk    = (data & 0x01);
		tcan_info->en_door_sw_trunk = 1;
	}
	else {
		tcan_info->en_door_sw_trunk = 0;
	}

	/* ドアSW P席情報取得 */
	result = TCAN_Ctrl_GetMTPDoorOpen(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->door_sw_seat_p    = (data & 0x01);
		tcan_info->en_door_sw_seat_p = 1;
	}
	else {
		tcan_info->en_door_sw_seat_p = 0;
	}

	/* ドアSW D席情報取得 */
	result = TCAN_Ctrl_GetMTDoorOpen(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->door_sw_seat_d    = (data & 0x01);
		tcan_info->en_door_sw_seat_d = 1;
	}
	else {
		tcan_info->en_door_sw_seat_d = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_282				*/
/* 機能の説明	: T-CAN ID=282のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_282(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* 表示外気温情報取得 */
	result = TCAN_Ctrl_GetMTOutTemp(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->out_temp    = data;
		tcan_info->en_out_temp = 1;
	}
	else {
		tcan_info->en_out_temp = 0;
	}

	/* シートベルトSW P席情報取得 */
	result = TCAN_Ctrl_GetMTPSeatBelt(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->seat_belt_sw_p    = (data & 0x01);
		tcan_info->en_seat_belt_sw_p = 1;
	}
	else {
		tcan_info->en_seat_belt_sw_p = 0;
	}

	/* シートベルトSW D席情報取得 */
	result = TCAN_Ctrl_GetMTDSeatBelt(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->seat_belt_sw_d    = (data & 0x01);
		tcan_info->en_seat_belt_sw_d = 1;
	}
	else {
		tcan_info->en_seat_belt_sw_d = 0;
	}

	/* 仕向地情報取得 */
	result = TCAN_Ctrl_GetMTPlace(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->destination    = (data & 0x0f);
		tcan_info->en_destination = 1;
	}
	else {
		tcan_info->en_destination = 0;
	}

	/* ブランドフラグ情報取得 */
	result = TCAN_Ctrl_GetMTBrandFlag(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->brand_flag    = (data & 0x01);
		tcan_info->en_brand_flag = 1;
	}
	else {
		tcan_info->en_brand_flag = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_140				*/
/* 機能の説明	: T-CAN ID=140のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_140(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* アクセル開度情報取得 */
	result = TCAN_Ctrl_GetMTRatioAccel(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->accel_ratio    = data_8;
		tcan_info->en_accel_ratio = 1;
	}
	else {
		tcan_info->en_accel_ratio = 0;
	}

	/* エンジン回転数情報取得 */
	result = TCAN_Ctrl_GetMTEngRpm(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->eng_rpm    = data_16;
		tcan_info->en_eng_rpm = 1;
	}
	else {
		tcan_info->en_eng_rpm = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_141				*/
/* 機能の説明	: T-CAN ID=141のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_141(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* エンジン回転数タコメータ用情報取得 */
	result = TCAN_Ctrl_GetMTEngRpmTa(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->eng_rpm_taco    = data_16;
		tcan_info->en_eng_rpm_taco = 1;
	}
	else {
		tcan_info->en_eng_rpm_taco = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_144				*/
/* 機能の説明	: T-CAN ID=152のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_144(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* トランスミッション情報取得 */
	result = TCAN_Ctrl_GetMTTransmissionType(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->transmission_type    = (data & 0x01);
		tcan_info->en_transmission_type = 1;
	}
	else {
		tcan_info->en_transmission_type = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_360				*/
/* 機能の説明	: T-CAN ID=360のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_360(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* エンジン油温情報取得 */
	result = TCAN_Ctrl_GetMTTempOil(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->temp_oil    = data_8;
		tcan_info->en_temp_oil = 1;
	}
	else {
		tcan_info->en_temp_oil = 0;
	}

	/* エンジン水温情報取得 */
	result = TCAN_Ctrl_GetMTTempCoolant(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->temp_coolant    = data_8;
		tcan_info->en_temp_coolant = 1;
	}
	else {
		tcan_info->en_temp_coolant = 0;
	}

	/* クルコンセットランプ情報取得 */
	result = TCAN_Ctrl_GetMTSetCc(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->cruise_controlrl_set    = (data_8 & 0x01);
		tcan_info->en_cruise_controlrl_set = 1;
	}
	else {
		tcan_info->en_cruise_controlrl_set = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_361				*/
/* 機能の説明	: T-CAN ID=361のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_361(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* MTギアポジション表示情報取得 */
	result = TCAN_Ctrl_GetMTGearEstm(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->mt_gear_pos    = (data_8 & 0x07);
		tcan_info->en_mt_gear_pos = 1;
	}
	else {
		tcan_info->en_mt_gear_pos = 0;
	}
}
/********************************************************/
/* 関数名称		: update_tcan_info_id_148				*/
/* 機能の説明	: T-CAN ID=148のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_148(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* スポーツシフトギア位置表示情報取得 */
	result = TCAN_Ctrl_GetMTSportGearPosition(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->sports_gear_pos    = (data_8 & 0x0f);
		tcan_info->en_sports_gear_pos = 1;
	}
	else {
		tcan_info->en_sports_gear_pos = 0;
	}

	/* シフト位置情報(フェールセーフ処理後)情報取得 */
	result = TCAN_Ctrl_GetMTShiftPosition(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->shift_pos    = (data_8 & 0x0f);
		tcan_info->en_shift_pos = 1;
	}
	else {
		tcan_info->en_shift_pos = 0;
	}

	/* TCM変速制御モード情報取得 */
	result = TCAN_Ctrl_GetMTModeShiftControl(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->mode_shift_control    = (data_8 & 0x07);
		tcan_info->en_mode_shift_control = 1;
	}
	else {
		tcan_info->en_mode_shift_control = 0;
	}

	/* 目標ギア位置情報取得 */
	result = TCAN_Ctrl_GetMTTargetGearPos(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->target_gear_pos = (data_8 & 0x0f);
		tcan_info->en_target_gear_pos = 1;
	}
	else {
		tcan_info->en_target_gear_pos = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_368				*/
/* 機能の説明	: T-CAN ID=368のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_368(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* AT種類情報取得 */
	result = TCAN_Ctrl_GetMTAtType(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->at_type    = (data_8 & 0x07);
		tcan_info->en_at_type = 1;
	}
	else {
		tcan_info->en_at_type = 0;
	}

	/* スポーツランプ情報取得 */
	result = TCAN_Ctrl_GetMTSportsLamp(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->sports_lamp    = (data_8 & 0x03);
		tcan_info->en_sports_lamp = 1;
	}
	else {
		tcan_info->en_sports_lamp = 0;
	}

	/* スノーホールドランプ情報取得 */
	result = TCAN_Ctrl_GetMTSnowHoldLamp(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->snow_hold_lamp    = (data_8 & 0x03);
		tcan_info->en_snow_hold_lamp = 1;
	}
	else {
		tcan_info->en_snow_hold_lamp = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_0d0				*/
/* 機能の説明	: T-CAN ID=0d0のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_0d0(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* 舵角センサ情報取得 */
	result = TCAN_Ctrl_GetMTSteerAngle(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->steer_angle    = data_16;
		tcan_info->en_steer_angle = 1;
	}
	else {
		tcan_info->en_steer_angle = 0;
	}

	/* ヨーレート情報取得 */
	result = TCAN_Ctrl_GetMTYawRate(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->yaw_rate    = data_16;
		tcan_info->en_yaw_rate = 1;
	}
	else {
		tcan_info->en_yaw_rate = 0;
	}

	/* 横加速度情報取得 */
	result = TCAN_Ctrl_GetMTAccLR(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->acc_lr    = data_8;
		tcan_info->en_acc_lr = 1;
	}
	else {
		tcan_info->en_acc_lr = 0;
	}

	/* 前後Ｇセンサ情報取得 */
	result = TCAN_Ctrl_GetMTAccFR(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->acc_fr    = data_8;
		tcan_info->en_acc_fr = 1;
	}
	else {
		tcan_info->en_acc_fr = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_0d1				*/
/* 機能の説明	: T-CAN ID=0d1のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_0d1(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16         data_16;
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* 駆動輪平均車輪速情報取得 */
	result = TCAN_Ctrl_GetMTSpWheelAve(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->wheel_speed_ave    = data_16;
		tcan_info->en_wheel_speed_ave = 1;
	}
	else {
		tcan_info->en_wheel_speed_ave = 0;
	}

	/* ブレーキ圧情報取得 */
	result = TCAN_Ctrl_GetMtBrakePressure(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->brake_pressure    = data_8;
		tcan_info->en_brake_pressure = 1;
	}
	else {
		tcan_info->en_brake_pressure = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_0d3				*/
/* 機能の説明	: T-CAN ID=0d3のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_0d3(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* 車速パルス積算値情報取得 */
	result = TCAN_Ctrl_GetMTSpPulseInteg(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_pulse_integ    = (data_8 & 0x3f);
		tcan_info->en_speed_pulse_integ = 1;
	}
	else {
		tcan_info->en_speed_pulse_integ = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_0d4				*/
/* 機能の説明	: T-CAN ID=0d4のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/05								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_0d4(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16          data_16;
	E_TCAN_RESULT result;

	/* 左前輪車輪速情報取得 */
	result = TCAN_Ctrl_GetMtSpdWheelFtlh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_ftlh    = data_16;
		tcan_info->en_speed_wheel_ftlh = 1;
	}
	else {
		tcan_info->en_speed_wheel_ftlh = 0;
	}

	/* 右前輪車輪速情報取得 */
	result = TCAN_Ctrl_GetMtSpdWheelFtrh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_ftrh    = data_16;
		tcan_info->en_speed_wheel_ftrh = 1;
	}
	else {
		tcan_info->en_speed_wheel_ftrh = 0;
	}

	/* 左後輪車輪速情報取得 */
	result = TCAN_Ctrl_GetMtSpdWheelRrlh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_rrlh    = data_16;
		tcan_info->en_speed_wheel_rrlh = 1;
	}
	else {
		tcan_info->en_speed_wheel_rrlh = 0;
	}

	/* 右後輪車輪速情報取得 */
	result = TCAN_Ctrl_GetMtSpdWheelRrrh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_rrrh    = data_16;
		tcan_info->en_speed_wheel_rrrh = 1;
	}
	else {
		tcan_info->en_speed_wheel_rrrh = 0;
	}
}

#ifdef _FOR_RCF
CAN_CTRL_LOCAL void update_tcan_info_id_0b4(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* 駆動輪平均車輪速情報取得 */
	result = TCAN_Ctrl_GetMTSpWheelAve(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->wheel_speed_ave    = data_16;
		tcan_info->en_wheel_speed_ave = 1;
	}
	else {
		tcan_info->en_wheel_speed_ave = 0;
	}
}
#endif	/* _FOR_RCF */

/********************************************************/
/* 関数名称		: update_tcan_info_id_44d				*/
/* 機能の説明	: T-CAN ID=44Dのカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/26								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_44d(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* ノードID情報取得 */
	result = TCAN_Ctrl_GetMTNodeID(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->node_id  = data_8;
		tcan_info->en_node_id = 1;
	}
	else {
		tcan_info->en_node_id = 0;
	}

	/* Sleep Ack情報取得 */
	result = TCAN_Ctrl_GetMTSleepAck(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->sleep_ack    = (data_8 & 0x01);
		tcan_info->en_sleep_ack = 1;
	}
	else {
		tcan_info->en_sleep_ack = 0;
	}

	/* Sleep Ind情報取得 */
	result = TCAN_Ctrl_GetMTSleepInd(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->sleep_ind    = (data_8 & 0x01);
		tcan_info->en_sleep_ind = 1;
	}
	else {
		tcan_info->en_sleep_ind = 0;
	}

	/* Limp Home情報取得 */
	result = TCAN_Ctrl_GetMTLimpHome(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->limp_home    = (data_8 & 0x01);
		tcan_info->en_limp_home = 1;
	}
	else {
		tcan_info->en_limp_home = 0;
	}

	/* Ring情報取得 */
	result = TCAN_Ctrl_GetMTRing(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->ring    = (data_8 & 0x01);
		tcan_info->en_ring = 1;
	}
	else {
		tcan_info->en_ring = 0;
	}

	/* Alive情報取得 */
	result = TCAN_Ctrl_GetMTAlive(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->alive    = (data_8 & 0x01);
		tcan_info->en_alive = 1;
	}
	else {
		tcan_info->en_alive = 0;
	}

	/* Wkup情報取得 */
	result = TCAN_Ctrl_GetMTWkup(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->wkup    = (data_8 & 0x01);
		tcan_info->en_wkup = 1;
	}
	else {
		tcan_info->en_wkup = 0;
	}

	/* Init情報取得 */
	result = TCAN_Ctrl_GetMTInit(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->init    = (data_8 & 0x01);
		tcan_info->en_init = 1;
	}
	else {
		tcan_info->en_init = 0;
	}

	/* Spec. No.情報取得 */
	result = TCAN_Ctrl_GetMTSpecNo(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->spec_no    = data_16;
		tcan_info->en_spec_no = 1;
	}
	else {
		tcan_info->en_spec_no = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_id_375				*/
/* 機能の説明	: T-CAN ID=375のカレント情報を更新する	*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/11/03								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_375(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* ドアロック状態（助手席）情報取得 */
	result = TCAN_Ctrl_GetMtDoorLockStatPass(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->lock_stat_pass    = data_8;
		tcan_info->en_lock_stat_pass = 1;
	}
	else {
		tcan_info->en_lock_stat_pass = 0;
	}

	/* ドアロック状態（運転席）情報取得 */
	result = TCAN_Ctrl_GetMtDoorLockStatDrvr(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->lock_stat_driver    = data_8;
		tcan_info->en_lock_stat_driver = 1;
	}
	else {
		tcan_info->en_lock_stat_driver = 0;
	}
}

/********************************************************/
/* 関数名称		: update_tcan_info_calc					*/
/* 機能の説明	: T-CAN 内部算出のカレント情報を更新する*/
/* 引数			: ( /O)tcan_info:T-CANカレント情報 		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_calc(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8         data_8;
	UI_32         data_32;
	E_TCAN_RESULT result;

	/* 始動後走行距離情報取得 */
	result = TCAN_Ctrl_GetOdo(&data_32);
	if(result == TCAN_CTRL_OK) {
		tcan_info->running_distance    = data_32;
		tcan_info->en_running_distance = 1;
	}
	else {
		tcan_info->en_running_distance = 0;
	}

	/* 燃料噴射量積算値情報取得 */
	result = TCAN_Ctrl_GetFCSum(&data_32);
	if(result == TCAN_CTRL_OK) {
		tcan_info->fuel_consumption    =  data_32;
		tcan_info->en_fuel_consumption = 1;
	}
	else {
		tcan_info->en_fuel_consumption = 0;
	}

	/* 表示用車速情報取得 */
	result = TCAN_Ctrl_GetVehicleSpeedForDisp(&data_32);
	if(result == TCAN_CTRL_OK) {
		tcan_info->vehicle_speed_for_disp    =  data_32;
		tcan_info->en_vehicle_speed_for_disp = 1;
	}
	else {
		tcan_info->en_vehicle_speed_for_disp = 0;
	}

	/* なましブレーキ圧取得 */
	result = TCAN_Ctrl_GetSmoothingBrakePressure(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->smoothing_brake_pressure    =  data_8;
		tcan_info->en_smoothing_brake_pressure = 1;
	}
	else {
		tcan_info->en_smoothing_brake_pressure = 0;
	}
}

#ifdef ISOCAN_ENABLE
/********************************************************/
/* 関数名称		: update_iso_can_info_all				*/
/* 機能の説明	: ISO-CANカレント情報を更新する			*/
/* 引数			: なし									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/11/08								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_iso_can_info_all(void)
{
	E_ISO_CAN_RESULT result;
	UI_8  data_8;
	UI_16 data_16;
	
	/* エンジン水温情報取得 */
	result = Iso_can_ctrl_get_eng_cool_temp(&data_8);
	if(result == ISO_CAN_CTRL_OK) {
		S_Can_ctrl_iso_can_info.temp_colant    =  data_8;
		S_Can_ctrl_iso_can_info.en_temp_colant = 1;
	}
	else {
		S_Can_ctrl_iso_can_info.en_temp_colant = 0;
	}

	/* 吸気マニフォールド圧情報取得 */
	result = Iso_can_ctrl_get_im_abs_press(&data_8);
	if(result == ISO_CAN_CTRL_OK) {
		S_Can_ctrl_iso_can_info.ima_pressure    =  data_8;
		S_Can_ctrl_iso_can_info.en_ima_pressure = 1;
	}
	else {
		S_Can_ctrl_iso_can_info.en_ima_pressure = 0;
	}

	/* エンジン回転数情報取得 */
	result = Iso_can_ctrl_get_eng_rpm(&data_16);
	if(result == ISO_CAN_CTRL_OK) {
		S_Can_ctrl_iso_can_info.eng_rpm    =  data_16;
		S_Can_ctrl_iso_can_info.en_eng_rpm = 1;
	}
	else {
		S_Can_ctrl_iso_can_info.en_eng_rpm = 0;
	}

	/* 車速情報取得 */
	result = Iso_can_ctrl_get_vehicle_speed(&data_8);
	if(result == ISO_CAN_CTRL_OK) {
		S_Can_ctrl_iso_can_info.vehicle_speed    =  data_8;
		S_Can_ctrl_iso_can_info.en_vehicle_speed = 1;
	}
	else {
		S_Can_ctrl_iso_can_info.en_vehicle_speed = 0;
	}

	/* スロットル位置報取得 */
	result = Iso_can_ctrl_get_throttle_pos(&data_8);
	if(result == ISO_CAN_CTRL_OK) {
		S_Can_ctrl_iso_can_info.throttle_pos    =  data_8;
		S_Can_ctrl_iso_can_info.en_throttle_pos = 1;
	}
	else {
		S_Can_ctrl_iso_can_info.en_throttle_pos = 0;
	}

	/* 電圧情報取得 */
	result = Iso_can_ctrl_get_ctrl_mod_volt(&data_16);
	if(result == ISO_CAN_CTRL_OK) {
		S_Can_ctrl_iso_can_info.volts    =  data_16;
		S_Can_ctrl_iso_can_info.en_volts = 1;
	}
	else {
		S_Can_ctrl_iso_can_info.en_volts = 0;
	}

	/* 吸気温度情報取得 */
	result = Iso_can_ctrl_get_intake_air_temp(&data_8);
	if(result == ISO_CAN_CTRL_OK) {
		S_Can_ctrl_iso_can_info.temp_intake_manifold    =  data_8;
		S_Can_ctrl_iso_can_info.en_temp_intake_manifold = 1;
	}
	else {
		S_Can_ctrl_iso_can_info.en_temp_intake_manifold = 0;
	}

}
#endif /* ISOCAN_ENABLE */

/********************************************************/
/* 関数名称		: receive_tcan_data						*/
/* 機能の説明	: T-CANデータ受信処理					*/
/* 引数			: なし									*/
/* 戻り値		: N_TRUE	受信データあり				*/
/* 				: N_FALSE	受信データなし				*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL B_8 receive_tcan_data(void)
{
	E_RC       result;
	T_Can_data can_data;
	B_8        rcv_flg = N_FALSE;
	
	/* CANドライバから受信済みデータ取得 */
	result = Can_drv_get_rx_data(CAN_CH_TCAN, &can_data);
	while (result == E_RC_OK) {
		rcv_flg = N_TRUE;

		/* T-CANコントローラでこのデータを解釈 */
		TCAN_Ctrl_SetMessage(can_data.id, can_data.len, can_data.data);

		/* CANドライバから受信済みデータ取得 */
		result = Can_drv_get_rx_data(CAN_CH_TCAN, &can_data);
	}

	return rcv_flg;

}

/********************************************************/
/* 関数名称		: can_init_inspection_info				*/
/* 機能の説明	: 検査情報初期化						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void can_init_inspection_info(void)
{
	(void)memset(&S_can_inspection_info, 0, sizeof(S_can_inspection_info));
	S_can_inspection_info.reqflg = N_OFF;
	return;
}

/********************************************************/
/* 関数名称		: can_inspect_can0						*/
/* 機能の説明	: CAN CH0検査							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void can_inspect_can0(void)
{
	E_INSP_INSPECTION_RESULT	inspect_res = INSP_INSPECTION_RESULT_INVALID;
	T_Can_data		candata;
	E_RC			ret;

	if (Timer_ctrl_TimeCompare1ms( S_can_inspection_info.start_time, CAN_INSPECTION_TIMEOUT ) == N_OK) {
		/* CAN検査タイムアウト	*/
		inspect_res = INSP_INSPECTION_RESULT_NG;
#ifdef CAN_DEBUG
		OUTLOG(OutputLog("[Can]insp can0 timeout!!\n");)
#endif /*CAN_DEBUG*/
	}
	else {
		for ( ; S_can_inspection_info.msg_cnt < CAN_INSPECTION_MSG_NUM; S_can_inspection_info.msg_cnt++) {
			/* CAN0からメッセージを受信	*/
			ret = Can_drv_get_rx_data(CAN_CH_0, &candata);
			if (ret != E_RC_OK) {
				/* メッセージ未受信の場合は処理を抜ける	*/
				break;
			}

#ifdef CAN_DEBUG
			OUTLOG(OutputLog("[Can]insp can0 rcv id:%X len:%u data:%02X %02X %02X %02X %02X %02X %02X %02X\n",
						candata.id, candata.len,
						candata.data[0], candata.data[1], candata.data[2], candata.data[3],
						candata.data[4], candata.data[5], candata.data[6], candata.data[7]);)
#endif /*CAN_DEBUG*/

			/* 受信したメッセージをCAN0、CAN1に送信する	*/
			ret = Can_drv_set_tx_data(CAN_CH_0, ((S_can_inspection_info.msg_cnt % CAN_INSPECTION_SEND_BUFFER_COUNT) + 1), candata.id, candata.data, candata.len);
			if (ret != E_RC_OK) {
				/* CAN0 送信異常	*/
				inspect_res = INSP_INSPECTION_RESULT_NG;
#ifdef CAN_DEBUG
				OUTLOG(OutputLog("[Can]insp can0 can0 snd err!!\n");)
#endif /*CAN_DEBUG*/
				break;
			}
			ret = Can_drv_set_tx_data(CAN_CH_1, ((S_can_inspection_info.msg_cnt % CAN_INSPECTION_SEND_BUFFER_COUNT) + 1), candata.id, candata.data, candata.len);
			if (ret != E_RC_OK) {
				/* CAN1 送信異常	*/
				inspect_res = INSP_INSPECTION_RESULT_NG;
#ifdef CAN_DEBUG
				OUTLOG(OutputLog("[Can]insp can0 can1 snd err!!\n");)
#endif /*CAN_DEBUG*/
				break;
			}
		}
		if (S_can_inspection_info.msg_cnt == CAN_INSPECTION_MSG_NUM) {
			/* CAN検査 正常	*/
			inspect_res = INSP_INSPECTION_RESULT_OK;
		}
	}

	if (inspect_res != INSP_INSPECTION_RESULT_INVALID) {
		/* 検査制御モジュールに結果を通知する	*/
		Insp_notify_inspection_result(INSP_INSPECTION_TYPE_CAN0, NULL, inspect_res);

		/* 検査情報をクリア	*/
		can_init_inspection_info();
	}

	return;
}

/********************************************************/
/* 関数名称		: can_inspect_can1						*/
/* 機能の説明	: CAN CH1検査							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void can_inspect_can1(void)
{
	/* CAN1は検査なし	*/
	/* 検査制御モジュールに結果OKを通知する		*/
	Insp_notify_inspection_result(INSP_INSPECTION_TYPE_CAN1, NULL, INSP_INSPECTION_RESULT_OK);

	/* 検査情報をクリア	*/
	can_init_inspection_info();

	return;
}

/********************************************************/
/* 関数名称		: can_inspect_stop						*/
/* 機能の説明	: CAN検査停止							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void can_inspect_stop(void)
{
	E_INSP_INSPECTION_TYPE	type;

	if (S_can_inspection_info.ch == CAN_INSPECTION_CAN_CH0) {
		type = INSP_INSPECTION_TYPE_CAN0;
	}
	else if (S_can_inspection_info.ch == CAN_INSPECTION_CAN_CH1) {
		type = INSP_INSPECTION_TYPE_CAN1;
	}
	else {
		type = INSP_INSPECTION_TYPE_INVLID;
	}

	if (type != INSP_INSPECTION_TYPE_INVLID) {
		/* 検査制御モジュールに結果NGを通知する		*/
		Insp_notify_inspection_result(type, NULL, INSP_INSPECTION_RESULT_NG);
	}

	/* 検査情報をクリア	*/
	can_init_inspection_info();

	return;
}


/********************************************************/
/* 関数名称		: readEepData							*/
/* 機能の説明	: 不揮発メモリからデータ読み込み		*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/11/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void readEepData(void)
{
	UI_16 vehicleInfo = 0x0000;	
	/* EEPROMから車種情報を読みだす	*/
	(void)Eeprom_control_GetData(EEPROM_DATA_VEHICLEINFO, &vehicleInfo);
	/*車両判定情報*/
	S_Can_ctrl_tcan_info.en_eep_vehicle_info = 1;	
	if ( (vehicleInfo == (UI_16)TCAN_VEHICLE_INFO_86A) || (vehicleInfo == (UI_16)TCAN_VEHICLE_INFO_86B) ){
		/*86A または 86B*/
		S_Can_ctrl_tcan_info.eep_vehicle_info = vehicleInfo;
	}else{
		/*不明*/
		/*最悪不明状態から変化しないと困るので不明時は86Bとする*/
		S_Can_ctrl_tcan_info.eep_vehicle_info = (UI_16)TCAN_VEHICLE_INFO_86B;
	}
#ifdef CAN_DEBUG
	OUTLOG(OutputLog( "[CAN] EEPROM Read (%04x)\n", S_Can_ctrl_tcan_info.eep_vehicle_info );)
#endif /*CAN_DEBUG*/
}

/********************************************************/
/* 関数名称		: writeEepData							*/
/* 機能の説明	: 不揮発メモリへデータ書き込み			*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 20/11/-								*/
/* 備考			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void writeEepData(void)
{
	UI_16 vehicleInfo;

#ifdef CAN_DEBUG
	OUTLOG(OutputLog( "[CAN] EEPROM Write (%04x)\n", S_Can_ctrl_tcan_info.eep_vehicle_info );)
#endif /*CAN_DEBUG*/
	/*車種情報*/
	vehicleInfo = S_Can_ctrl_tcan_info.eep_vehicle_info;
	/* EEPROMへ車種情報を書き出す	*/
	(void)Eeprom_control_SetData(EEPROM_DATA_VEHICLEINFO, vehicleInfo, NULL);
}
