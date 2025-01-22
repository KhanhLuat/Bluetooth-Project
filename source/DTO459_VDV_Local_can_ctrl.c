/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: FR81S								*/
/* Date				: 16/08/18							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Katsura						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		:									*/
/* date				:									*/
/********************************************************/
/********************************************************/
/* file DTO459_VDV_Local_canctrl.c						*/
/* Local CAN制御										*/
/********************************************************/
/********************************************************/
/*	インクルードファイル								*/
/********************************************************/

#include <type.h>
#include <system.h>
#include <string.h>
#include "DTO459_VDV_Local_can_ctrl.h"
#include "System_control_TASK.h"
#include "DTO459_VDV_Can_Drv.h"
#include "DTO459_VDV_Signal_ctrl.h"
#include "timer_ctrl.h"
#include "dbglog.h"
	
/********************************************************/
/*	マクロ定義											*/
/********************************************************/
#define LCAN_CTRL_LOCAL	static

#define CAN_CH_LOCAL_CAN			CAN_CH_1		/* Local CANのCANチャンネル */

#define SEND_BUFFER_COUNT			10				/* CANの送信バッファ数		*/

#define MAX_SIGNAL_COUNT_FIXED		12				/* 最大信号数（固定値）		*/
#define MAX_SIGNAL_COUNT_VARIABLE	50				/* 最大信号数（変数値）		*/

#define INTERVAL_GET_SIGNAL_DATA_FIXED		10000			/* (固定値）信号取得時間間隔(ms)		*/
#define INTERVAL_GET_SIGNAL_DATA_VARIABLE	100				/* (変数値）信号取得時間間隔(ms)		*/

#define SIGNAL_ID_ENCODE_KEY				100				/* エンコードキーの信号ID				*/

/********************************************************/
/*	内部変数定義										*/
/********************************************************/
LCAN_CTRL_LOCAL UI_32 S_get_signal_data_timestamp_fixed;							/* 車両信号データ(固定値）取得タイムスタンプ	*/
LCAN_CTRL_LOCAL UI_32 S_get_signal_data_timestamp_variable;						/* 車両信号データ(変数値）取得タイムスタンプ	*/
LCAN_CTRL_LOCAL UI_8  S_signal_count_fixed;										/* 信号数（固定値）								*/
LCAN_CTRL_LOCAL UI_8  S_signal_count_variable;									/* 信号数（変数値）								*/
LCAN_CTRL_LOCAL const UI_16 *S_signal_id_list_fixed;								/* 信号ID値一覧（固定値）						*/
LCAN_CTRL_LOCAL const UI_16 *S_signal_id_list_variable;							/* 信号ID値一覧（変数値）						*/
LCAN_CTRL_LOCAL SI_32 S_signal_data_list_fixed[MAX_SIGNAL_COUNT_FIXED];			/* 信号データ値一覧（固定値）					*/
LCAN_CTRL_LOCAL SI_32 S_signal_data_list_variable[MAX_SIGNAL_COUNT_VARIABLE];	/* 信号データ値一覧（変数値）					*/
LCAN_CTRL_LOCAL UI_8  S_waiting_signal_count_fixed;								/* 送信待ち信号数（固定値）						*/
LCAN_CTRL_LOCAL UI_8  S_waiting_signal_count_variable;							/* 送信待ち信号数（変数値）						*/
LCAN_CTRL_LOCAL UI_8  S_waiting_signal_count_encode_key;							/* 送信待ち信号数（エンコードキー）				*/
LCAN_CTRL_LOCAL SI_32 S_lcan_encode_key;											/* Local CAN用エンコードキー					*/

/********************************************************/
/*	内部関数											*/
/********************************************************/
LCAN_CTRL_LOCAL SI_8 get_signal_data_fixed(void);
LCAN_CTRL_LOCAL SI_8 get_signal_data_variable(void);
LCAN_CTRL_LOCAL void send_signal_data(void);
LCAN_CTRL_LOCAL E_RC set_send_signal_data(UI_8 send_buffer_index, UI_16 signal_id, UI_32 signal_data);

/********************************************************/
/* 関数名称		: Local_can_ctrl_Init					*/
/* 機能の説明	: Local CAN制御初期化					*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/10/11								*/
/* 備考			: 										*/
/********************************************************/
void Local_can_ctrl_Init(void)
{
	UI_16 i;

	/* 信号データ取得タイムスタンプクリア */
	S_get_signal_data_timestamp_fixed    = 0;
	S_get_signal_data_timestamp_variable = 0;

	/* 信号数クリア */
	S_signal_count_fixed    = 0;
	S_signal_count_variable = 0;

	/* 信号ID値一覧(ポインタ)初期化 */
	S_signal_id_list_fixed    = NULL;
	S_signal_id_list_variable = NULL;

	/* 信号データ値一覧(実体)初期化 */
	for(i = 0; i < MAX_SIGNAL_COUNT_FIXED; i++) {
		S_signal_data_list_fixed[i] = 0;
	}
	for(i = 0; i < MAX_SIGNAL_COUNT_VARIABLE; i++) {
		S_signal_data_list_variable[i] = 0;
	}

	/* 送信待ち信号数クリア */
	S_waiting_signal_count_fixed    = 0;
	S_waiting_signal_count_variable = 0;
	S_waiting_signal_count_encode_key = 0;

	/* Local CAN用エンコードキークリア */
	S_lcan_encode_key = 0;

	/* CANドライバの初期化 */
	Can_drv_ch_init(CAN_CH_LOCAL_CAN, SEND_BUFFER_COUNT);
}


/********************************************************/
/* 関数名称		: Local_can_ctrl_Main					*/
/* 機能の説明	: Local CAN制御メイン					*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/10/11								*/
/* 備考			: 										*/
/********************************************************/
void Local_can_ctrl_Main(void)
{
	UI_32 cur_time = Timer_ctrl_GetTime1ms();

	if(S_lcan_encode_key == 0) {
		/* 暗号化キー未取得なら暗号用乱数生成 */
		S_lcan_encode_key = Signal_control_GetRandomValue();

		/* Local CAN用暗号化キー設定 */
		(void)Signal_control_SetEncodeKey( DEVICE_TYPE_CAN, S_lcan_encode_key );
	}

	/* 固定値信号データ取得タイミング到来チェック */
	if((S_get_signal_data_timestamp_fixed == 0)
	|| ((cur_time - S_get_signal_data_timestamp_fixed) >= INTERVAL_GET_SIGNAL_DATA_FIXED)) {
		/* 固定値信号データ取得 */
		if(get_signal_data_fixed() == N_OK) {
			/* 固定値信号データ取得タイムスタンプ更新 */
			S_get_signal_data_timestamp_fixed = cur_time;

			/* すべての固定信号を送信待ちにする */
			S_waiting_signal_count_fixed = S_signal_count_fixed;
		}
	}

	/* 変数値信号データ取得タイミング到来チェック */
	if((S_get_signal_data_timestamp_variable == 0)
	|| ((cur_time - S_get_signal_data_timestamp_variable) >= INTERVAL_GET_SIGNAL_DATA_VARIABLE)) {
		/* 変数値信号データ取得 */
		if(get_signal_data_variable() == N_OK) {
			/* 変数値信号データ取得タイムスタンプ更新 */
			S_get_signal_data_timestamp_variable = cur_time;

			/* すべての変数信号を送信待ちにする */
			S_waiting_signal_count_variable = S_signal_count_variable;

/*
			for(index = 0; index < S_signal_count_variable; index++) {
				if(S_signal_id_list_variable[index] == 221) {
					OUTLOG(OutputLog("**** send data[ID=221, data = %08x] ****\n",  S_signal_data_list_variable[index]);)
				}
			}
*/
		}
	}

	/* 送信待ち信号が存在すればLocal CAN 送信処理実行 */
	if((S_waiting_signal_count_fixed > 0) || (S_waiting_signal_count_variable > 0) || (S_waiting_signal_count_encode_key > 0)) {
		send_signal_data();
	}
}

/********************************************************/
/* 関数名称		: get_signal_data_fixed					*/
/* 機能の説明	: 固定値信号データ取得					*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/10/11								*/
/* 備考			: 										*/
/********************************************************/
LCAN_CTRL_LOCAL SI_8 get_signal_data_fixed(void)
{
	SI_8 result;
	
	/* 信号ID一覧が取得できていなければ取得する */
	if(S_signal_id_list_fixed == NULL) {
		result = Signal_control_GetSignalList(DEVICE_TYPE_CAN, ID_TYPE_FIXED_ID, &S_signal_count_fixed, &S_signal_id_list_fixed);
		if(result != N_OK) {
			S_signal_count_fixed   = 0;
			S_signal_id_list_fixed = NULL;
		}
		
		/* 信号数が最大値を超えていたら、最大値とする（フェールセーフ） */
		if(S_signal_count_fixed > MAX_SIGNAL_COUNT_FIXED) {
			S_signal_count_fixed = MAX_SIGNAL_COUNT_FIXED;
		}
	}

	/* 信号データ列取得 */
	result = Signal_control_GetSignalsInfo(DEVICE_TYPE_CAN, S_signal_count_fixed, S_signal_id_list_fixed, S_signal_data_list_fixed);
	if(result != N_OK) {
		return N_NG;
	}

	return N_OK;
}

/********************************************************/
/* 関数名称		: get_signal_data_variable				*/
/* 機能の説明	: 変数値信号データ取得					*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/10/11								*/
/* 備考			: 										*/
/********************************************************/
LCAN_CTRL_LOCAL SI_8 get_signal_data_variable(void)
{

	SI_8 result;
	
	/* 信号ID一覧が取得できていなければ取得する */
	if(S_signal_id_list_variable == NULL) {
		result = Signal_control_GetSignalList(DEVICE_TYPE_CAN, ID_TYPE_VARIABLE_ID, &S_signal_count_variable, &S_signal_id_list_variable);
		if(result != N_OK) {
			S_signal_count_variable   = 0;
			S_signal_id_list_variable = NULL;
		}
		
		/* 信号数が最大値を超えていたら、最大値とする（フェールセーフ） */
		if(S_signal_count_variable > MAX_SIGNAL_COUNT_VARIABLE) {
			S_signal_count_variable = MAX_SIGNAL_COUNT_VARIABLE;
		}
	}

	/* 信号データ列取得 */
	result = Signal_control_GetSignalsInfo(DEVICE_TYPE_CAN, S_signal_count_variable, S_signal_id_list_variable, S_signal_data_list_variable);
	if(result != N_OK) {
		return N_NG;
	}

	return N_OK;
}

/********************************************************/
/* 関数名称		: send_signal_data						*/
/* 機能の説明	: 信号データ送信処理					*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/10/11								*/
/* 備考			: 										*/
/********************************************************/
LCAN_CTRL_LOCAL void send_signal_data(void)
{

	E_RC result;
	UI_8 send_index;
	UI_8 data_index;
	UI_8 msg_no = 1;
	SI_32 encode_key;
	
	/* 変数値信号の送信待ちデータが存在するか？ */
	if(S_waiting_signal_count_variable > 0) {
		/* 送信バッファ１個を残して、変数値信号を送信する */
		for(send_index = 0; send_index < (SEND_BUFFER_COUNT - 1); send_index++) {
			if (S_waiting_signal_count_variable <= 0) {
				break;
			}

			/* 今回送信する信号インデックス決定 */
			data_index = S_signal_count_variable - S_waiting_signal_count_variable;

			/* 送信信号をCANドライバに設定 */
			result = set_send_signal_data(msg_no, S_signal_id_list_variable[data_index],
													(UI_32)S_signal_data_list_variable[data_index]);
			if(result == N_OK) {
				/* 送信待ち数デクリメント */
				S_waiting_signal_count_variable--;
				msg_no++;
			}
		}
	}

	/* 固定値信号の送信待ちデータが存在するか？ */
	if(S_waiting_signal_count_fixed > 0) {
		/* 今回送信する信号インデックス決定 */
		data_index = S_signal_count_fixed - S_waiting_signal_count_fixed;

		/* 送信信号をCANドライバに設定 */
		result = set_send_signal_data(msg_no, S_signal_id_list_fixed[data_index],
													(UI_32)S_signal_data_list_fixed[data_index]);

		if(result == N_OK) {
			/* 送信待ち数デクリメント */
			S_waiting_signal_count_fixed--;
			msg_no++;

			/* 固定値信号を全部送信したらエンコードキーを送信待ちにする */
			if(S_waiting_signal_count_fixed <= 0) {
				S_waiting_signal_count_encode_key = 1;
			}
		}
	}
	/* エンコードキーが送信待ちか？ */
	else if(S_waiting_signal_count_encode_key > 0) {
		/* 送信信号をCANドライバに設定 */
		(void)Signal_control_GetEncodeKey(DEVICE_TYPE_CAN, &encode_key);
		result = set_send_signal_data(msg_no, SIGNAL_ID_ENCODE_KEY, (UI_32)encode_key);
		if(result == N_OK) {
			/* 送信待ち数デクリメント */
			S_waiting_signal_count_encode_key--;
			msg_no++;
		}
	}
	else {
		/* nop */
	}
}

/********************************************************/
/* 関数名称		: set_send_signal_data					*/
/* 機能の説明	: 送信信号データ設定処理				*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/10/11								*/
/* 備考			: 										*/
/********************************************************/
LCAN_CTRL_LOCAL E_RC set_send_signal_data(UI_8 send_buffer_index, UI_16 signal_id, UI_32 signal_data)
{

	E_RC  result;
	UI_32 msg_id;
	UI_8  msg_data[4];

	msg_id = (UI_32)signal_id;

	msg_data[0] = (UI_8)(signal_data >> 24);
	msg_data[1] = (UI_8)((signal_data >> 16) & 0xff);
	msg_data[2] = (UI_8)((signal_data >>  8) & 0xff);
	msg_data[3] = (UI_8)(signal_data & 0xff);

	result = Can_drv_set_tx_data(CAN_CH_LOCAL_CAN, send_buffer_index, msg_id, msg_data, 4);
	return result;
}
