/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: R81S								*/
/* Date				: 16/08/18							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Sakata						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		: 									*/
/* date				: 									*/
/********************************************************/
/********************************************************/
/* file DTO459_VDV_Advance_ctrl.c						*/
/* AdvanceControlUnit制御								*/
/********************************************************/

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include <string.h>
#include "system.h"
#include "DTO459_VDV_Sw_ctrl.h"
#include "DTO459_VDV_Inspection_ctrl.h"
#include "DTO459_VDV_Gnss_ctrl.h"
#include "DTO459_VDV_Bt_ctrl.h"
#include "DTO459_VDV_Advance_ctrl.h"
#include "System_control_TASK.h"
#include "defi_07701_CTRL.h"						/* DF07701制御ﾍｯﾀﾞｰﾌｧｲﾙ */
#include "defi_uart_prt.h"							/* Defi UARTﾌﾟﾛﾄｺﾙﾍｯﾀﾞｰﾌｧｲﾙ */

/********************************************************/
/*	マクロ定義											*/
/********************************************************/
#define ADVC_LOCAL	static

#define ADVANCE_UART_DATA_BUF_SIZE		(1 * 1024)
#define		DEFI_REAL_SZ				(36)							/**< ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀｻｲｽﾞ		*/

/********************************************************/
/*	構造体定義											*/
/********************************************************/
typedef struct t_advance_inspection_info {
	UI_8						reqflg;
} T_ADVANCE_INSPECTION_INFO;

/********************************************************/
/*	内部変数定義										*/
/********************************************************/
ADVC_LOCAL	UI_8	Defi_real_data[DEFI_REAL_SZ];							/**< リアルモードデータ 			*/
ADVC_LOCAL	UI_8	Defi_real_len;											/**< リアルモードデータ長 			*/
ADVC_LOCAL	E_SYS_CTRL_STATUS			S_df_prev_system_state;				/**< 前のシステム状態 				*/
ADVC_LOCAL	T_ADVANCE_CTRL_UNIT_INFO	S_acu_Info;							/**<AdvanceControlUnitから取得した値*/
ADVC_LOCAL T_ADVANCE_INSPECTION_INFO	S_advance_inspection_info;			/* 検査情報							*/
ADVC_LOCAL E_DEFI_UART_BAUD_RATE		S_advance_gnsscomm_brate;			/* GNSS通信モード時のボーレート		*/
ADVC_LOCAL UI_8		S_advance_uart_data[ADVANCE_UART_DATA_BUF_SIZE];		/* UARTデータバッファ				*/
																			/*	以下の動作モードで使用する		*/
																			/*	検査モード						*/
																			/*	BT通信モード					*/
																			/*	GNSS通信モード					*/

/********************************************************/
/*	内部関数											*/
/********************************************************/
#ifdef ADVANCE_ENABLE
ADVC_LOCAL void advance_control_storeData(const UI_8* buff);
#endif /*ADVANCE_ENABLE*/

ADVC_LOCAL void advance_control_initInspection(void);
ADVC_LOCAL void advance_control_initInspectionInfo(void);
ADVC_LOCAL void advance_control_inspection(void);

ADVC_LOCAL void advance_control_initBtComm(void);
ADVC_LOCAL void advance_control_btComm(void);

ADVC_LOCAL void advance_control_initGnssComm(void);
ADVC_LOCAL void advance_control_gnssComm(void);

/********************************************************/
/*	外部関数											*/
/********************************************************/

/********************************************************/
/* 関数名称		: Advance_control_Init					*/
/* 機能の説明	: AdvanceControlUnit制御初期化			*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Advance_control_Init(void)
{
	Defi_real_len = 0;
	(void)memset( Defi_real_data, 0, DEFI_REAL_SZ*sizeof(UI_8));
	(void)memset(&S_acu_Info, 0x00, sizeof(T_ADVANCE_CTRL_UNIT_INFO));
	S_acu_Info.invalidFlag = 1;
	S_df_prev_system_state = SYS_OPERATION_WAIT_ACC_ON;

	return;
}

/********************************************************/
/* 関数名称		: Advance_control_Main					*/
/* 機能の説明	: AdvanceControlUnit制御メイン			*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Advance_control_Main(void)
{
	E_SYS_CTRL_STATUS	system_state;									/* システム状態 */
#ifdef ADVANCE_ENABLE
	UI_8	length;
	UI_8	acu_buff[DEFI_REAL_SZ];										/*バッファ*/
#endif /* ADVANCE_ENABLE */

	/* システム状態が変化したかを判定 */
	system_state = System_control_GetStatus();
	if ( system_state != S_df_prev_system_state )
	{
		switch (system_state)
		{
		case (SYS_OPERATION_PRE_ACC_OFF):
		case (SYS_OPERATION_NORMAL_OFF):
		case (SYS_OPERATION_INSPECTION_COMP):
			/* Advance用UART停止	*/
			Uart_stop();
			break;

#ifdef ADVANCE_ENABLE
		case (SYS_OPERATION_NORMAL_AUTO):
		case (SYS_OPERATION_NORMAL_MANUAL):
			if ((S_df_prev_system_state != SYS_OPERATION_NORMAL_AUTO) &&
				(S_df_prev_system_state != SYS_OPERATION_NORMAL_MANUAL)) {
				/* Advance制御初期化(UART開始)	*/
				Defi_07701_control_Init();
			}
			break;
#endif /*ADVANCE_ENABLE*/

		case (SYS_OPERATION_INSPECTION):
			/* 検査制御初期化(UART開始)	*/
			advance_control_initInspection();
			break;

		case (SYS_OPERATION_BT_COMM):
			/* BT通信モード制御初期化(UART開始)	*/
			advance_control_initBtComm();
			break;

		case (SYS_OPERATION_GNSS_COMM):
			/* GNSS通信モード制御初期化(UART開始)	*/
			advance_control_initGnssComm();
			break;
			
		default:
			break;
		}
		S_df_prev_system_state = system_state;
	}

	switch (system_state)
	{
#ifdef ADVANCE_ENABLE
		case (SYS_OPERATION_NORMAL_AUTO):
		case (SYS_OPERATION_NORMAL_MANUAL):
			/**< 通常モード:オートRECまたはマニュアルREC	*/
			/* DF07701制御処理を実行 */
			Defi_07701_control_main();

			/*リアルモードデータ（車両情報）取得*/
			length = Get_real_mode_data(acu_buff);
			if(length > 0)
			{
				/*受信データありなら車両情報構造体に格納*/
				advance_control_storeData(acu_buff);
			}
			break;
#endif /*ADVANCE_ENABLE*/

		case (SYS_OPERATION_INSPECTION):
			/**< 検査モード		*/
			advance_control_inspection();
			break;

		case (SYS_OPERATION_BT_COMM):
			/**< BTモジュール通信モード		*/
			advance_control_btComm();
			break;

		case (SYS_OPERATION_GNSS_COMM):
			/**< GNSSモジュール通信モード		*/
			advance_control_gnssComm();
			break;

		default:
			return;
	}
	return;
}


/********************************************************/
/* 関数名称		: Advance_control_GetData				*/
/* 機能の説明	: 車両情報取得							*/
/* 引数			: 車両情報構造体						*/
/* 				: T_ADVANCE_CTRL_UNIT_INFO				*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Advance_control_GetData(T_ADVANCE_CTRL_UNIT_INFO **data)
{
	*data = &S_acu_Info;
}

/** 
 * ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀ設定処理
 * 
 * param	*data_p		[in]	ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀのﾎﾟｲﾝﾀ 
 * param	len			[in]	ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀ長  
 *
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		<ul>
 *			<li>ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀをstatic変数に設定する。 
 *			<li>DF07701ﾕﾆｯﾄ接続時のﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀは34byte 
 *			</ul>
 */ 
void Set_real_mode_data(const UI_8 *data_p, UI_8 len)
{
	if ((1 <= len) &&
		(len <= DEFI_REAL_SZ)) {
		(void)memcpy(Defi_real_data, data_p, (size_t)len);
		Defi_real_len = len;
	}
}

/** 
 * ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀ取得処理 
 * 
 * param	*data_p		[out]	ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀ格納ﾊﾞｯﾌｧのﾎﾟｲﾝﾀ 
 *
 * return	ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀ長  
 * 
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		<ul>
 *			<li>static変数に設定されたﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀを取得する。   
 *			<li>DF07701ﾕﾆｯﾄ接続時のﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀは34byte 
 *			</ul>
 */ 
UI_8 Get_real_mode_data(UI_8 *data_p)
{
	UI_8 real_len;
	
	if (Defi_real_len > 0) {
		(void)memcpy(data_p, Defi_real_data, (size_t)Defi_real_len);
	}
	
	real_len = Defi_real_len;
	Defi_real_len = 0;

	return real_len;
}

/********************************************************/
/* 関数名称		: Advance_control_ReqInspection			*/
/* 機能の説明	: 検査要求								*/
/* 引数			: void									*/
/* 戻り値		: N_OK		正常						*/
/* 				: N_NG		異常						*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 本関数が正常終了した場合、検査制御モジュールに対して		*/
/*				   Insp_notify_inspection_resultにより検査結果を通知する	*/
/********************************************************/
SI_8 Advance_control_ReqInspection(void)
{
	E_SYS_CTRL_STATUS	system_state;

	/* システム状態取得	*/
	system_state = System_control_GetStatus();
	if (system_state != SYS_OPERATION_INSPECTION) {		/* システム状態が「検査モード」以外	*/
		return N_NG;
	}

	/* 検査制御モジュールに結果(OK)を通知する	*/
	Insp_notify_inspection_result(INSP_INSPECTION_TYPE_UART, NULL, INSP_INSPECTION_RESULT_OK);

	if (S_advance_inspection_info.reqflg != N_ON) {
		/* UART検査ON	*/
		S_advance_inspection_info.reqflg = N_ON;

		/* UART通信開始	*/
		Uart_start();
	}

	return N_OK;
}

/********************************************************/
/* 関数名称		:	Advance_control_getUartData			*/
/* 機能の説明	:	UARTデータ取得						*/
/* 引数			:	( /O)buf	データバッファ			*/
/*				:	(I/ )len	取得データ長(byte)		*/
/* 戻り値		:	取得できたデータ長(byte)			*/
/* 作成者		:	(OFT)Toguchi						*/
/* 作成日		:	17/01/20							*/
/* 備考			:										*/
/********************************************************/
UI_16 Advance_control_getUartData(UI_8 *buf, UI_16 len)
{
	UI_16	get_len;

	if (buf == NULL) {
		return 0;
	}

	get_len = Uart_get_rx_data(buf, len);

	return get_len;
}

/********************************************************/
/* 関数名称		:	Advance_control_getUartDataLen		*/
/* 機能の説明	:	UARTデータ長取得					*/
/* 引数			:	void								*/
/* 戻り値		:	UARTデータ長(byte)					*/
/* 作成者		:	(OFT)Toguchi						*/
/* 作成日		:	17/01/20							*/
/* 備考			:										*/
/********************************************************/
UI_16 Advance_control_getUartDataLen(void)
{
	UI_16	len;

	len = Uart_get_rx_data_len();

	return len;
}

/********************************************************/
/*	内部関数											*/
/********************************************************/
#ifdef ADVANCE_ENABLE
/********************************************************/
/* 関数名称		: advance_control_storeData				*/
/* 機能の説明	: 受信データを車両情報として格納		*/
/* 引数			: (I/ )受信データ						*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
ADVC_LOCAL void advance_control_storeData(const UI_8* buff)
{
	/* 受信データありならすべてのデータを受信済み */
	UI_8 index = 11;	/* 先頭は11 */
	/* 無効フラグをOFFにする */
	S_acu_Info.invalidFlag = 0;
	/* 車速 */
	S_acu_Info.VehicleSpeed				 = (UI_16)((UI_16)(((UI_16)(buff[index + 1] & 0x7FU)) << 8) | buff[index + 0]);
	/* エンジン回転数 */
	S_acu_Info.EngineRevolutionSpeed	 = (UI_16)((UI_16)(((UI_16)(buff[index + 3] & 0x7FU)) << 8) | buff[index + 2]);
	/* ターボ圧(過給圧) */
	S_acu_Info.TurboPressure			 = (UI_16)((UI_16)(((UI_16)(buff[index + 5] & 0x7FU)) << 8) | buff[index + 4]);
	/* エンジン油圧 */
	S_acu_Info.EngineOilPressure		 = (UI_16)((UI_16)(((UI_16)(buff[index + 7] & 0x7FU)) << 8) | buff[index + 6]);
	/* 燃料圧力 */
	S_acu_Info.FuelPressure				 = (UI_16)((UI_16)(((UI_16)(buff[index + 9] & 0x7FU)) << 8) | buff[index + 8]);
	/* 差圧 */
	S_acu_Info.DifferentialPressure		 = (UI_8)((buff[index + 9] >> 7) & 0x01U);
	/* エンジン油温 */
	S_acu_Info.EngineOiTemperture		 = (UI_16)((UI_16)(((UI_16)(buff[index + 11] & 0x7FU)) << 8) | buff[index + 10]);
	/* エンジン水温 */
	S_acu_Info.EngineCoolantTemperture	 = (UI_16)((UI_16)(((UI_16)(buff[index + 13] & 0x7FU)) << 8) | buff[index + 12]);
	/* 排気温度 */
	S_acu_Info.ExhaustGasTemperture		 = (UI_16)((UI_16)(((UI_16)(buff[index + 15] & 0x7FU)) << 8) | buff[index + 14]);
	/* 電圧 */
	S_acu_Info.Volts					 = (UI_8)(buff[index + 16] & 0xFFU);
}
#endif /*ADVANCE_ENABLE*/

/************************************************************/
/* 関数名称		: advance_control_initInspection			*/
/* 機能の説明	: 検査制御初期化							*/
/* 引数			: void										*/
/* 戻り値		: void										*/
/* 作成者		: (OFT)Toguchi								*/
/* 作成日		: 17/01/20									*/
/* 備考			: 											*/
/************************************************************/
ADVC_LOCAL void advance_control_initInspection(void)
{
	T_DEFI_UART_PROTOCOL	protocol;

	/* 検査制御情報初期化	*/
	advance_control_initInspectionInfo();

	/* UART通信設定	*/
	protocol.parity		= DEFI_UART_PARITY_EVEN;
	protocol.baud_rate	= DEFI_UART_BAUD_RATE_19200;
	(void)Uart_set_comm_protocol(&protocol);

	return;
}

/************************************************************/
/* 関数名称		: advance_control_initInspectionInfo		*/
/* 機能の説明	: 検査情報初期化							*/
/* 引数			: void										*/
/* 戻り値		: void										*/
/* 作成者		: (OFT)Toguchi								*/
/* 作成日		: 17/01/20									*/
/* 備考			: 											*/
/************************************************************/
ADVC_LOCAL void advance_control_initInspectionInfo(void)
{
	(void)memset(&S_advance_inspection_info, 0, sizeof(S_advance_inspection_info));
	S_advance_inspection_info.reqflg = N_OFF;

	return;
}

/************************************************************/
/* 関数名称		: advance_control_inspection				*/
/* 機能の説明	: 検査										*/
/* 引数			: void										*/
/* 戻り値		: void										*/
/* 作成者		: (OFT)Toguchi								*/
/* 作成日		: 17/01/20									*/
/* 備考			: 											*/
/************************************************************/
ADVC_LOCAL void advance_control_inspection(void)
{
	UI_16	data_len;

	if (S_advance_inspection_info.reqflg != N_ON) {
		return;
	}

	/* UART送信可能データ長取得	*/
	data_len = Uart_get_can_set_tx_data_len();
	if (data_len > 0) {						/* UART送信可能(送信バッファに空きあり)	*/

		/* UARTデータ受信	*/
		if (data_len > ADVANCE_UART_DATA_BUF_SIZE) {
			/* 受信データをデータ格納バッファサイズ以下にする	*/
			data_len = ADVANCE_UART_DATA_BUF_SIZE;
		}
		data_len = Uart_get_rx_data(S_advance_uart_data, data_len);
		if (data_len > 0) {					/* UART受信データあり					*/

			/* 受信したデータを送信する	*/
			(void)Uart_set_tx_data(S_advance_uart_data, data_len);
		}
	}

	return;
}

/************************************************************/
/* 関数名称		: advance_control_initBtComm				*/
/* 機能の説明	: BT通信モード制御初期化					*/
/* 引数			: void										*/
/* 戻り値		: void										*/
/* 作成者		: (OFT)Toguchi								*/
/* 作成日		: 17/01/20									*/
/* 備考			: 											*/
/************************************************************/
ADVC_LOCAL void advance_control_initBtComm(void)
{
	T_DEFI_UART_PROTOCOL	protocol;

	/* UART通信設定	*/
	protocol.parity		= DEFI_UART_PARITY_NONE;
	protocol.baud_rate	= DEFI_UART_BAUD_RATE_115200;
	(void)Uart_set_comm_protocol(&protocol);

	/* UART通信開始	*/
	Uart_start();

	return;
}

/************************************************************/
/* 関数名称		: advance_control_btComm					*/
/* 機能の説明	: BT通信モード制御							*/
/* 引数			: void										*/
/* 戻り値		: void										*/
/* 作成者		: (OFT)Toguchi								*/
/* 作成日		: 17/01/20									*/
/* 備考			: 											*/
/************************************************************/
ADVC_LOCAL void advance_control_btComm(void)
{
	UI_16	data_len;

	/* UART送信可能データ長取得	*/
	data_len = Uart_get_can_set_tx_data_len();
	if (data_len > 0) {						/* UART送信可能(送信バッファに空きあり)	*/

		if (data_len > ADVANCE_UART_DATA_BUF_SIZE) {
			/* データサイズをデータ格納バッファサイズ以下にする	*/
			data_len = ADVANCE_UART_DATA_BUF_SIZE;
		}
		/* BTデータ取得	*/
		data_len = Bt_control_getUartData(S_advance_uart_data, data_len);
		if (data_len > 0) {					/* BT受信データあり					*/
			
			/* 受信したデータを送信する	*/
			(void)Uart_set_tx_data(S_advance_uart_data, data_len);
		}
	}

	return;
}

/************************************************************/
/* 関数名称		: advance_control_initGnssComm				*/
/* 機能の説明	: GNSS通信モード制御初期化					*/
/* 引数			: void										*/
/* 戻り値		: void										*/
/* 作成者		: (OFT)Toguchi								*/
/* 作成日		: 17/01/20									*/
/* 備考			: 											*/
/************************************************************/
ADVC_LOCAL void advance_control_initGnssComm(void)
{
	T_DEFI_UART_PROTOCOL	protocol;

	/* UART通信設定	*/
	protocol.parity		= DEFI_UART_PARITY_NONE;
	if (Sw_control_GetSwMode() == SW_MODE_MANUAL) {
		S_advance_gnsscomm_brate = DEFI_UART_BAUD_RATE_9600;
	}
	else {
		S_advance_gnsscomm_brate = DEFI_UART_BAUD_RATE_115200;
	}
	protocol.baud_rate	= S_advance_gnsscomm_brate;
	(void)Uart_set_comm_protocol(&protocol);

	/* UART通信開始	*/
	Uart_start();

	return;
}

/************************************************************/
/* 関数名称		: advance_control_gnssComm					*/
/* 機能の説明	: GNSS通信モード制御						*/
/* 引数			: void										*/
/* 戻り値		: void										*/
/* 作成者		: (OFT)Toguchi								*/
/* 作成日		: 17/01/20									*/
/* 備考			: 											*/
/************************************************************/
ADVC_LOCAL void advance_control_gnssComm(void)
{
	T_DEFI_UART_PROTOCOL	protocol;
	UI_16	data_len;

	/* SWによるボーレート切り替えをチェック	*/
	protocol.baud_rate = DEFI_UART_BAUD_RATE_NUM;
	if (Sw_control_GetSwMode() == SW_MODE_MANUAL) {
		if (S_advance_gnsscomm_brate != DEFI_UART_BAUD_RATE_9600) {
			protocol.baud_rate = DEFI_UART_BAUD_RATE_9600;
		}
	}
	else {
		if (S_advance_gnsscomm_brate != DEFI_UART_BAUD_RATE_115200) {
			protocol.baud_rate = DEFI_UART_BAUD_RATE_115200;
		}
	}
	if (protocol.baud_rate != DEFI_UART_BAUD_RATE_NUM) {
		/* UART停止 */
		Uart_stop();

		/* 通信設定 */
		protocol.parity = DEFI_UART_PARITY_NONE;
		(void)Uart_set_comm_protocol(&protocol);

		/* UART開始 */
		Uart_start();

		/* 現在のボーレートを保持 */
		S_advance_gnsscomm_brate = protocol.baud_rate;
	}

	/* UART送信可能データ長取得	*/
	data_len = Uart_get_can_set_tx_data_len();
	if (data_len > 0) {						/* UART送信可能(送信バッファに空きあり)	*/

		if (data_len > ADVANCE_UART_DATA_BUF_SIZE) {
			/* 受信データをデータ格納バッファサイズ以下にする	*/
			data_len = ADVANCE_UART_DATA_BUF_SIZE;
		}
		/* GNSSデータ取得	*/
		data_len = Gnss_control_getUartData(S_advance_uart_data, data_len);
		if (data_len > 0) {					/* GNSS受信データあり					*/

			/* 受信したデータを送信する	*/
			(void)Uart_set_tx_data(S_advance_uart_data, data_len);
		}
	}

	return;
}
