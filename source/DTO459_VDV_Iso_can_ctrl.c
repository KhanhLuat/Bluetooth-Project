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
/* file DTO459_VDV_Iso_canctrl.c						*/
/* ISO CAN制御											*/
/********************************************************/

#ifdef ISOCAN_ENABLE
/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include <type.h>
#include <system.h>
#include <string.h>
#include "DTO459_VDV_Iso_can_ctrl.h"
#include "DTO459_VDV_Can_Drv.h"
#include "timer_ctrl.h"
#include "dbglog.h"

/********************************************************/
/*	マクロ定義											*/
/********************************************************/
#define CAN_CH_ISO_CAN					CAN_CH_0		/* ISO CANのCANチャンネル 		*/

#define ISO_CAN_SEND_BUFFER_COUNT		10				/* ISO CANの送信バッファ数			*/
#define ISO_CAN_MSG_ID_RX_MIN			(ISO_CAN_SEND_BUFFER_COUNT + 1)	/* ISO CANの受信用メッセージID最小値 */

#define INIT_LOOP_CNT					0				/**< (for)ループ用変数初期値			*/
#define INIT_GROUP1_NUM					0				/**< group1_numberの初期値				*/
#define INIT_GROUP2_NUM					0				/**< group2_numberの初期値				*/
#define INIT_GROUP3_NUM					0				/**< group3_numberの初期値				*/
#define INIT_VAL_SUM					0				/**< sumの初期値						*/
#define SHIFT_1BIT						1				/**< 1ビットシフト						*/
#define DATA1_SHIFT						8				/**< 8ビットシフト(CANデータを格納)		*/
#define DATA2_SHIFT						8				/**< 8ビットシフト(CANデータを格納)		*/
#define DATA3_SHIFT						8				/**< 8ビットシフト(CANデータを格納)		*/
#define JUDGE_REQ_BIT					0x8000U			/**< リクエスト判定用MASKビット			*/
#define NO_REQUEST						0				/**< リクエストなし						*/
#define TAKEOUT_HIGH_4BIT				4				/**< 上位4ビット取り出し				*/
#define TAKEOUT_LOW_4BIT				0x0F			/**< 下位4ビット取り出し				*/
#define TAKEOUT_HIGH_16BIT				16				/**< 上位16ビット取り出し				*/
#define TAKEOUT_LOW_16BIT				0x0000FFFFUL		/**< 下位16ビット取り出し				*/
#define NO_RESPONSE						0				/**< レスポンス無し						*/
#define GR2_EFF_REQ_0					0				/**< グループ2有効リクエスト無し		*/
#define GR3_EFF_REQ_0					0				/**< グループ3有効リクエスト無し		*/
#define JUDGE_WAIT_TIME					100				/**< CAN通信待ち時間(100ms)				*/
#define JUDGE_RETRY_WAIT_TIME			5000			/**< CAN接続確認リトライ待ち時間(5s)	*/
#define JUDGE_CONNECT_START_WAIT_TIME	500				/**< 接続確認開始待ち時間(500ms)		*/

/* type 1 リクエスト */
#define CAN_ID_TYPE_1_REQ_CAN_ID_ADDRESS	0x7DFUL		/* CAN ID アドレスリクエスト	*/
#define CAN_ID_TYPE_1_REQ_CAN_ID_ECU_1		0x7E0UL		/* CAN ID リクエスト ECU #1		*/
#define CAN_ID_TYPE_1_RES_CAN_ID_ECU_1		0x7E8UL		/* CAN ID レスポンス ECU #1		*/
#define CAN_ID_TYPE_1_REQ_CAN_ID_ECU_8		0x7E7UL		/* CAN ID リクエスト ECU #8		*/
#define CAN_ID_TYPE_1_RES_CAN_ID_ECU_8		0x7EFUL		/* CAN ID レスポンス ECU #8		*/

/* type 2 リクエスト */
#define CAN_ID_TYPE_2_REQ_CAN_ID			0x18DB33F1UL	/* CAN ID Functionリクエスト ECUs with #33			*/
#define CAN_ID_TYPE_2_RES_CAN_ID			0x18DAF100UL	/* CAN ID レスポンス（最下位バイト無効）			*/
#define CAN_ID_TYPE_2_REQ_TEMP				0x18DA00F1UL	/* 信号リクエストのテンプレート（00の箇所にECU番号）*/
#define CAN_ID_TYPE_2_RES_TEMP				0x18DAF100UL	/* 信号レスポンスのテンプレート（00の箇所にECU番号）*/

/* モード */
#define MODE_REQ_NORMAL						0x01		/* 通常リクエスト				*/
#define MODE_REQ_TOYOTA						0x21		/* TOYOTAリクエスト				*/
#define MODE_RES_NORMAL						0x41		/* 通常レスポンス				*/
#define MODE_RES_TOYOTA						0x61		/* TOYOTAレスポンス				*/

/* 各優先順位におけるPID数 */ 
#define MAX_COUNT_PRIORITY_1_PID			2			/* 優先１の最大PID数			*/
#define MAX_COUNT_PRIORITY_2_PID			5			/* 優先２の最大PID数			*/
#define	MAX_REQUEST_NUMBER					(MAX_COUNT_PRIORITY_1_PID+MAX_COUNT_PRIORITY_2_PID)
#define SUP_PID_INFO_MAX_COUNT				10			/* PID対応情報最大数 			*/

/* PID */ 
#define PID_SUPPORT_INFO_COUNT				3			/* PIDサポート情報数					*/
#define PID_SUPPORT_INFO_00					0x00		/* PID 01-20 サポート情報 				*/
#define PID_SUPPORT_INFO_20					0x20		/* PID 21-40 サポート情報 				*/
#define PID_SUPPORT_INFO_40					0x40		/* PID 41-60 サポート情報 				*/
#define	PID_ENG_COOL_TEMP					0x05		/**< engine coolant temperature			*/
#define	PID_IMA_PRESSURE					0x0B		/**< intake manifold absolute pressure	*/
#define	PID_ENG_RPM							0x0C		/**< engine rpm							*/
#define	PID_VEHICLE_SPEED					0x0D		/**< vehicle speed						*/
#define	PID_INTAKE_AIR_TEMP					0x0F		/**< intake air temperature				*/
#define	PID_THROTTLE_POS					0x11		/**< throttle position					*/
#define	PID_CTRL_MOD_VOLT					0x42		/**< control module voltage				*/

/* 解析状態 */
enum e_crrespondence_num{
	CORRESPONDENCE_NUM0 = 0,							/**< CORRESPONDENCEの0番目				*/
	CORRESPONDENCE_NUM1,								/**< CORRESPONDENCEの1番目				*/
	CORRESPONDENCE_NUM2,								/**< CORRESPONDENCEの2番目				*/
	CORRESPONDENCE_NUM3,								/**< CORRESPONDENCEの3番目				*/
	CORRESPONDENCE_NUM4,								/**< CORRESPONDENCEの4番目				*/
	CORRESPONDENCE_NUM5,								/**< CORRESPONDENCEの5番目				*/
	CORRESPONDENCE_NUM_MAX								/**< CRRESPONDENCEの最大数				*/
};

/* 制御モード */
typedef enum e_ccc_mode {
	CCC_MODE_WAIT_CONNECT = 0,							/**< 接続確認待ち状態					*/
	CCC_MODE_GET_PID,									/**< 対応PID取得状態					*/
	CCC_MODE_REQUEST_PID,								/**< PIDリクエスト送信状態				*/
	CCC_MODE_CONNECT_ERROR,								/**< 通信エラー状態						*/
	CCC_MODE_MAX										/**< 制御状態数							*/
} E_CCC_MODE;

/* PIDモード */
typedef enum e_ccc_pid_mode {
	CCC_PID_STD_CAN_1A = 0,								/**< TYPE-1A接続確認					*/
	CCC_PID_STD_CAN_1B,									/**< TYPE-1B接続確認					*/
	CCC_PID_STD_CAN_1T,									/**< TYPE-1T接続確認					*/
	CCC_PID_EXT_CAN,									/**< TYPE-2接続確認						*/
	CCC_PID_PID_INFO,									/**< 対応PID取得						*/
	CCC_PID_MAKE_REQUEST,								/**< リクエスト優先順位作成				*/
	CCC_PID_WAIT,										/**< CANリクエスト送信待ち				*/
	CCC_PID_MAX											/**< 状態関数数							*/
} E_CCC_PID_MODE;

/* リクエストタイプ */
typedef enum {
	ISO_CAN_REQ_TYPE_1A = 0,							/* Type 1A								*/
	ISO_CAN_REQ_TYPE_1B,								/* Type 1B								*/
	ISO_CAN_REQ_TYPE_1T,								/* Type 1T								*/
	ISO_CAN_REQ_TYPE_2									/* Type 2								*/
} E_ISO_CAN_REQ_TYPE;

typedef enum {
	ISO_CAN_RESP_ACK = 0,
	ISO_CAN_RESP_NACK,
	ISO_CAN_RESP_NONE
} E_ISO_CAN_RESPONSE;

/********************************************************/
/*	内部構造体定義										*/
/********************************************************/
/* PID対応情報 */
typedef struct {
	UI_8		ecu_no;								/* ECU番号						*/
	UI_8		pid_supported[4];					/* PID対応情報一覧				*/
} SUPPORTED_PID_INFO;

/* CAN情報(UI_8用) */
typedef struct t_can_info_ui8 {
	UI_8	data;					/* データ			*/
	UI_8	first_flg;				/* 初回受信フラグ	*/
	UI_8	fail_flg;				/* フェールフラグ	*/
} T_CanInfo_UI8;

/* CAN情報(UI_16用) */
typedef struct t_can_info_ui16 {
	UI_16	data;					/* データ			*/
	UI_8	first_flg;				/* 初回受信フラグ	*/
	UI_8	fail_flg;				/* フェールフラグ	*/
} T_CanInfo_UI16;

/********************************************************/
/*	内部定数定義										*/
/********************************************************/
/* CAN-DATA request index */
static const UI_8 C_CCC_REQ_INDEX[3] ={
	PID_SUPPORT_INFO_00,						/**< request 0x01 to 0x1F				*/
	PID_SUPPORT_INFO_20,						/**< request 0x21 to 0x3F				*/
	PID_SUPPORT_INFO_40							/**< request 0x41 to 0x5F				*/
};

/* CAN-DATA response offset */
static const UI_8 C_CCC_RES_OFFSET[3] = {
	0, 2, 4										/**< 対応PIDレスポンス buff格納用offset	*/
};

/* PID優先順位:優先順位の高い順に配置 */
static const UI_8 C_correspondence[MAX_REQUEST_NUMBER] = {
	PID_ENG_RPM,										/**< engine rpm							*/
	PID_IMA_PRESSURE,									/**< intake manifold absolute pressure	*/
	PID_THROTTLE_POS,									/**< throttle position					*/
	PID_VEHICLE_SPEED,									/**< vehicle speed						*/
	PID_CTRL_MOD_VOLT,									/**< control module voltage				*/
	PID_ENG_COOL_TEMP,									/**< engine coolant temperature			*/
	PID_INTAKE_AIR_TEMP,								/**< intake air temperature				*/
};

/********************************************************/
/*	内部変数定義										*/
/********************************************************/
static UI_16 S_correspondence[CORRESPONDENCE_NUM_MAX];	/**< CAN受信解析データ					*/
static UI_8 S_pid_req_count;							/**< PIDリクエストカウント				*/
static UI_32 S_can_id;									/**< CAN-ID								*/
static UI_8 S_ccc_mode;									/**< 制御モード							*/
static UI_8 S_ccc_pid_mode;								/**< PIDモード							*/
static UI_32 S_retry_wait_time;							/**< CANリクエストリトライ待ち時間		*/
static UI_32 S_connect_stat_wait_time;					/**< 接続確認開始待ち時間				*/
static UI_32 S_connect_time;							/**< 対応PIDリクエスト待機時間			*/

static UI_8 S_tx_flg;									/**< CANリクエスト送信フラグ			*/
static UI_8 S_pid_seq;									/**< 対応PID取得シーケンス番号			*/
static UI_8 S_connect_check_cnt;						/**< 接続確認回数						*/

static UI_8					S_target_ecu_no;			/* 通信先ECU番号(Type 1B/Type 2にて使用)*/
static E_ISO_CAN_REQ_TYPE	S_connection_type;			/* 接続タイプ							*/

static UI_8					S_sup_pid_info_count;					/* タイプ2 PID対応情報数	*/
static SUPPORTED_PID_INFO	S_sup_pid_info[SUP_PID_INFO_MAX_COUNT];	/* タイプ2 PID対応情報配列	*/

static UI_8 S_priority_1_pid_count;						/* 優先１のPID数						*/
static UI_8 S_priority_2_pid_count;						/* 優先２のPID数						*/
static UI_8 S_priority_1_pid_index;						/* 優先１のPID列のインデックス			*/
static UI_8 S_priority_2_pid_index;						/* 優先２のPID列のインデックス			*/
static UI_8 S_priority_1_pid[MAX_COUNT_PRIORITY_1_PID];	/* 優先１のPID列						*/
static UI_8 S_priority_2_pid[MAX_COUNT_PRIORITY_2_PID];	/* 優先２のPID列						*/

/* 各種CAN信号情報 */
static T_CanInfo_UI8  S_can_info_eng_cool_temp;				/* Engine coolant temperature			*/
static T_CanInfo_UI8  S_can_info_im_abs_press;				/* Intake manifold absolute presure		*/
static T_CanInfo_UI16 S_can_info_eng_rpm;					/* Engine RPM							*/
static T_CanInfo_UI8  S_can_info_vehicle_speed;				/* Vehicle speed						*/
static T_CanInfo_UI8  S_can_info_intake_air_temp;			/* Intake air temperature				*/
static T_CanInfo_UI8  S_can_info_throttle_pos;				/* Throttle position					*/
static T_CanInfo_UI16 S_can_info_ctrl_mod_volt;				/* Control module voltage				*/

/********************************************************/
/*	内部関数											*/
/********************************************************/
static void S_ccc_set_mode( const UI_8 mode );

/*
 *	状態関数＠制御モード
 */
static void S_ccc_mode_wait_connect( void );			/**< CCC_MODE_WAIT_CONNECT				*/
static void S_ccc_mode_get_pid( void );					/**< CCC_MODE_GET_PID					*/
static void S_ccc_mode_request_pid( void );				/**< CCC_MODE_REQUEST_PID				*/
static void S_ccc_mode_connect_error( void );			/**< CCC_MODE_CONNECT_ERROR				*/

/*
 *	状態関数＠PIDモード
 */
static void S_ccc_pid_std_can_1a( void );				/**< CCC_PID_STD_CAN_1A					*/
static void S_ccc_pid_std_can_1b( void );				/**< CCC_PID_STD_CAN_1B					*/
static void S_ccc_pid_std_can_1t( void );				/**< CCC_PID_STD_CAN_1T					*/
static void S_ccc_pid_ext_can( void );					/**< CCC_PID_EXT_CAN					*/
static void S_ccc_pid_info( void );						/**< CCC_PID_PID_INFO					*/
static void S_ccc_pid_make_request( void );				/**< CCC_PID_MAKE_REQUEST				*/
static void S_ccc_pid_wait( void );						/**< CCC_PID_WAIT						*/

static E_RC send_request(E_ISO_CAN_REQ_TYPE req_type, UI_32 can_id, UI_8 pid);
static E_RC receive_response(E_ISO_CAN_REQ_TYPE req_type, UI_8 pid, UI_32 *can_id, E_ISO_CAN_RESPONSE *response, SUPPORTED_PID_INFO *sup_pid_info);
static UI_8 select_ecu_for_type_2(void);
static UI_8 get_enable_pid_count(const UI_8 *pid_supported);
static E_RC  parse_response_data(const T_Can_data *t_can_data, UI_8 *pid, UI_8 *value_data_len, UI_8 *value_data);
static void update_signal_value(UI_8 pid, UI_8 value_data_len, const UI_8 *value_data);
static UI_32 get_resp_msg_id(E_ISO_CAN_REQ_TYPE connection_type);
static UI_8 get_value_data_len(UI_8 pid);

/* 内部定数定義 */
/*
 *	制御モードテーブル
 */
static void ( * const C_ccc_mode_func[CCC_MODE_MAX] ) ( void ) = {
	S_ccc_mode_wait_connect,							/**< CCC_MODE_WAIT_CONNECT				*/
	S_ccc_mode_get_pid,     							/**< CCC_MODE_GET_PID					*/
	S_ccc_mode_request_pid, 							/**< CCC_MODE_REQUEST_PID				*/
	S_ccc_mode_connect_error							/**< CCC_MODE_CONNECT_ERROR				*/
};

/*
 *	PIDモードテーブル
 */
static void ( * const C_ccc_pid_func[CCC_PID_MAX] ) ( void ) ={
	S_ccc_pid_std_can_1a,								/**< CCC_PID_STD_CAN_1A					*/
	S_ccc_pid_std_can_1b,								/**< CCC_PID_STD_CAN_1B					*/
	S_ccc_pid_std_can_1t,								/**< CCC_PID_STD_CAN_1T					*/
	S_ccc_pid_ext_can,									/**< CCC_PID_EXT_CAN					*/
	S_ccc_pid_info,										/**< CCC_PID_PID_INFO					*/
	S_ccc_pid_make_request,								/**< CCC_PID_MAKE_REQUEST				*/
	S_ccc_pid_wait,										/**< CCC_PID_WAIT						*/
};


/********************************************************/
/*	外部関数											*/
/********************************************************/
/********************************************************/
/* 関数名称		: Iso_can_ctrl_get_eng_cool_temp		*/
/* 機能の説明	: エンジン水温情報取得					*/
/* 引数			: (O)data:エンジン水温情報				*/
/* 戻り値		: 取得結果								*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/11/08								*/
/* 備考			: 										*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_eng_cool_temp(UI_8 *data)
{
	E_ISO_CAN_RESULT	result;
	
	if (S_can_info_eng_cool_temp.first_flg == 0) {
		result = ISO_CAN_CTRL_NO_RECEIVE;
	} else if (S_can_info_eng_cool_temp.fail_flg == 1) {
		result = ISO_CAN_CTRL_ERROR;
	} else {
		result = ISO_CAN_CTRL_OK;
	}
	*data = S_can_info_eng_cool_temp.data;
	
	return result;
}

/********************************************************/
/* 関数名称		: Iso_can_ctrl_get_im_abs_press			*/
/* 機能の説明	: 吸気マニフォールド圧情報取得			*/
/* 引数			: (O)data:吸気マニフォールド圧情報		*/
/* 戻り値		: 取得結果								*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/11/08								*/
/* 備考			: 										*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_im_abs_press(UI_8 *data)
{
	E_ISO_CAN_RESULT	result;
	
	if (S_can_info_im_abs_press.first_flg == 0) {
		result = ISO_CAN_CTRL_NO_RECEIVE;
	} else if (S_can_info_im_abs_press.fail_flg == 1) {
		result = ISO_CAN_CTRL_ERROR;
	} else {
		result = ISO_CAN_CTRL_OK;
	}
	*data = S_can_info_im_abs_press.data;
	
	return result;
}

/********************************************************/
/* 関数名称		: Iso_can_ctrl_get_eng_rpm				*/
/* 機能の説明	: エンジン回転数情報取得				*/
/* 引数			: (O)data:エンジン回転数情報			*/
/* 戻り値		: 取得結果								*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/11/08								*/
/* 備考			: 										*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_eng_rpm(UI_16 *data)
{
	E_ISO_CAN_RESULT	result;
	
	if (S_can_info_eng_rpm.first_flg == 0) {
		result = ISO_CAN_CTRL_NO_RECEIVE;
	} else if (S_can_info_eng_rpm.fail_flg == 1) {
		result = ISO_CAN_CTRL_ERROR;
	} else {
		result = ISO_CAN_CTRL_OK;
	}
	*data = S_can_info_eng_rpm.data;
	
	return result;
}

/********************************************************/
/* 関数名称		: Iso_can_ctrl_get_vehicle_speed		*/
/* 機能の説明	: 車速情報取得							*/
/* 引数			: (O)data:車速情報						*/
/* 戻り値		: 取得結果								*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/11/08								*/
/* 備考			: 										*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_vehicle_speed(UI_8 *data)
{
	E_ISO_CAN_RESULT	result;
	
	if (S_can_info_vehicle_speed.first_flg == 0) {
		result = ISO_CAN_CTRL_NO_RECEIVE;
	} else if (S_can_info_vehicle_speed.fail_flg == 1) {
		result = ISO_CAN_CTRL_ERROR;
	} else {
		result = ISO_CAN_CTRL_OK;
	}
	*data = S_can_info_vehicle_speed.data;
	
	return result;
}

/********************************************************/
/* 関数名称		: Iso_can_ctrl_get_intake_air_temp		*/
/* 機能の説明	: 吸気温情報取得						*/
/* 引数			: (O)data:吸気温情報					*/
/* 戻り値		: 取得結果								*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 16/12/27								*/
/* 備考			: 										*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_intake_air_temp(UI_8 *data)
{
	E_ISO_CAN_RESULT	result;
	
	if (S_can_info_intake_air_temp.first_flg == 0) {
		result = ISO_CAN_CTRL_NO_RECEIVE;
	} else if (S_can_info_intake_air_temp.fail_flg == 1) {
		result = ISO_CAN_CTRL_ERROR;
	} else {
		result = ISO_CAN_CTRL_OK;
	}
	*data = S_can_info_intake_air_temp.data;
	
	return result;
}

/********************************************************/
/* 関数名称		: Iso_can_ctrl_get_throttle_pos			*/
/* 機能の説明	: スロットルポジション情報取得			*/
/* 引数			: (O)data:スロットルポジション情報		*/
/* 戻り値		: 取得結果								*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/11/08								*/
/* 備考			: 										*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_throttle_pos(UI_8 *data)
{
	E_ISO_CAN_RESULT	result;
	
	if (S_can_info_throttle_pos.first_flg == 0) {
		result = ISO_CAN_CTRL_NO_RECEIVE;
	} else if (S_can_info_throttle_pos.fail_flg == 1) {
		result = ISO_CAN_CTRL_ERROR;
	} else {
		result = ISO_CAN_CTRL_OK;
	}
	*data = S_can_info_throttle_pos.data;
	
	return result;
}

/********************************************************/
/* 関数名称		: Iso_can_ctrl_get_ctrl_mod_volt		*/
/* 機能の説明	: 電圧情報取得							*/
/* 引数			: (O)data:電圧情報						*/
/* 戻り値		: 取得結果								*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 16/12/27								*/
/* 備考			: 										*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_ctrl_mod_volt(UI_16 *data)
{
	E_ISO_CAN_RESULT	result;
	
	if (S_can_info_ctrl_mod_volt.first_flg == 0) {
		result = ISO_CAN_CTRL_NO_RECEIVE;
	} else if (S_can_info_ctrl_mod_volt.fail_flg == 1) {
		result = ISO_CAN_CTRL_ERROR;
	} else {
		result = ISO_CAN_CTRL_OK;
	}
	*data = S_can_info_ctrl_mod_volt.data;
	
	return result;
}

/********************************************************/
/* 関数名称		: Iso_can_ctrl_Init						*/
/* 機能の説明	: ISO CAN制御初期化						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/10/24								*/
/* 備考			: 										*/
/********************************************************/
void Iso_can_ctrl_Init(void)
{
	UI_32 i;
	
	S_pid_req_count = 0;
	S_ccc_mode = CCC_MODE_WAIT_CONNECT;
	S_ccc_pid_mode = CCC_PID_STD_CAN_1A;
	S_tx_flg = N_OFF;
	S_pid_seq = 0;
	S_connect_check_cnt = 0;
	S_connect_time = 0;
	S_retry_wait_time = 0;
	S_connect_stat_wait_time = 0;
	S_connection_type = ISO_CAN_REQ_TYPE_1A;
	S_target_ecu_no = 0;
	S_sup_pid_info_count = 0;

	for(i = 0; i < CORRESPONDENCE_NUM_MAX; i++)
	{
		S_correspondence[i] = 0;
	}

	/* PID優先テーブルクリア */
	S_priority_1_pid_count = 0;
	S_priority_2_pid_count = 0;
	S_priority_1_pid_index = 0;
	S_priority_2_pid_index = 0;
	for(i = 0; i < (sizeof(S_priority_1_pid) / sizeof(UI_8)); i++) {
		S_priority_1_pid[i] = 0;
	}
	for(i = 0; i < (sizeof(S_priority_2_pid) / sizeof(UI_8)); i++) {
		S_priority_2_pid[i] = 0;
	}

	/* 各種CAN信号情報初期化 */
	(void)memset(&S_can_info_eng_cool_temp,		0, sizeof(S_can_info_eng_cool_temp));
	(void)memset(&S_can_info_im_abs_press,		0, sizeof(S_can_info_im_abs_press));
	(void)memset(&S_can_info_eng_rpm,			0, sizeof(S_can_info_eng_rpm));
	(void)memset(&S_can_info_vehicle_speed,		0, sizeof(S_can_info_vehicle_speed));
	(void)memset(&S_can_info_intake_air_temp,	0, sizeof(S_can_info_intake_air_temp));
	(void)memset(&S_can_info_throttle_pos,		0, sizeof(S_can_info_throttle_pos));
	(void)memset(&S_can_info_ctrl_mod_volt,		0, sizeof(S_can_info_ctrl_mod_volt));

	/* CANドライバの初期化 */
	Can_drv_ch_init(CAN_CH_ISO_CAN, ISO_CAN_SEND_BUFFER_COUNT);
}

/********************************************************/
/* 関数名称		: Iso_can_ctrl_Main						*/
/* 機能の説明	: ISO CAN制御メイン						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/10/24								*/
/* 備考			: 										*/
/********************************************************/
void Iso_can_ctrl_Main(void)
{
	/**	<ol>	*/
	/**	<li>状態関数を呼び出す	*/
	C_ccc_mode_func[S_ccc_mode]();

	/**	</ol>	*/
	return;
}

/********************************************************/
/*	内部関数											*/
/********************************************************/
/********************************************************
 * 制御モード設定処理
 * 
 * param	mode	[in]	制御モード
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		制御モードを更新する．
********************************************************/
static void S_ccc_set_mode( const UI_8 mode )
{
	if (S_ccc_mode != mode) {
		if (mode == (UI_8)CCC_MODE_REQUEST_PID) {
			S_pid_req_count = 0;
			S_priority_1_pid_index = 0;
			S_priority_2_pid_index = 0;
		}
		OUTLOG(OutputLog("[ISOCAN]CtrlMode:%d->%d\n", S_ccc_mode, mode);)
	}

	/**	<ol>	*/
	/**	<li>指定された制御モードを内部変数に保持	*/
	S_ccc_mode = mode;
	/**	</ol>	*/
	return;
}

/********************************************************
 * 制御モード:接続確認待ち状態
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note	
********************************************************/
static void S_ccc_mode_wait_connect( void )
{
	if (S_connect_stat_wait_time == 0) {
		S_connect_stat_wait_time = Timer_ctrl_GetTime1ms();
	}

	/* 接続確認開始待ち */
	if (Timer_ctrl_TimeCompare1ms(S_connect_stat_wait_time, JUDGE_CONNECT_START_WAIT_TIME) == N_OK) {
		/**	<li>[対応PID取得]モードに遷移	*/
		S_ccc_set_mode( CCC_MODE_GET_PID );
	}
}

/********************************************************
 * 制御モード:対応PID取得状態
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		対応PIDモードに従った処理を行う．
********************************************************/
static void S_ccc_mode_get_pid( void )
{
	C_ccc_pid_func[S_ccc_pid_mode]();
}

/********************************************************
 * 制御モード:PIDリクエスト送信状態
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		PIDリクエスト優先順位に従って、PIDリクエストを送信する．
********************************************************/
static void S_ccc_mode_request_pid( void )
{
	UI_8		pid_index;
	UI_8		*priority_pid;
	T_Can_data	can_data;
	UI_8		pid;
	UI_8		value_data_len;
	UI_8		value_data[5];
	UI_8		i;

	if ((S_priority_1_pid_count + S_priority_2_pid_count) > 0) {

		priority_pid = NULL;
		for (i = 0; i < 2; i++) {
			if((S_pid_req_count % 2) == 0) {
				if (S_priority_1_pid_count > 0) {
					/* 優先１を選択する */
					priority_pid = S_priority_1_pid;
					pid_index =  S_priority_1_pid_index;
					S_priority_1_pid_index++;
					if (S_priority_1_pid_count <= S_priority_1_pid_index) {
						S_priority_1_pid_index = 0;
					}
				}
			}
			else {
				if (S_priority_2_pid_count > 0) {
					/* 優先２を選択する */
					priority_pid = S_priority_2_pid;
					pid_index =  S_priority_2_pid_index;
					S_priority_2_pid_index++;
					if (S_priority_2_pid_count <= S_priority_2_pid_index) {
						S_priority_2_pid_index = 0;
					}
				}
			}
			S_pid_req_count++;

			if (priority_pid != NULL) {
				break;
			}
		}

		/* 信号リクエスト送信 */
		(void)send_request(S_connection_type, S_can_id, priority_pid[pid_index]);

		/* レスポンス受信ループ */
		while(Can_drv_get_rx_data(CAN_CH_ISO_CAN, &can_data) == E_RC_OK) {
			/* レスポンスデータのパース */
			if(parse_response_data(&can_data, &pid, &value_data_len, value_data) == E_RC_OK) {
				update_signal_value(pid, value_data_len, value_data);
			}
		}
	}
}

/********************************************************
 * 状態関数:接続エラー状態
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		何もしない．
********************************************************/
static void S_ccc_mode_connect_error( void )
{
	/**	<ol>	*/
	/**	<li>何もしない	*/

	/**	</ol>	*/
}


/********************************************************
 * PIDモード:TYPE-1A接続確認
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		車両がTYPE-1Aに対応しているか判定を行う．
 * 			規定時間が過ぎるまでに正常なレスポンスが得られた場合、
 * 			PIDモードを対応PID対応取得に設定する．
 * 			規定時間中に正常なレスポンスが得られなかった場合、
 * 			PIDモードをTYPE-1B接続確認に設定する．
********************************************************/
static void S_ccc_pid_std_can_1a( void )
{
	E_RC				ret_code;
	UI_32				can_id;
	E_ISO_CAN_RESPONSE	response;

	if( S_tx_flg == N_OFF)
	{
		S_tx_flg = N_ON;

		/* メッセージバッファの設定(受信ID:0x7E8)	*/
		Can_drv_change_message_buffer(CAN_CH_ISO_CAN, ISO_CAN_MSG_ID_RX_MIN, CAN_ID_TYPE_1_RES_CAN_ID_ECU_1);
		Can_drv_clear_rx_data(CAN_CH_ISO_CAN);

		/* 通常対応PIDリクエスト */
		(void)send_request(ISO_CAN_REQ_TYPE_1A, CAN_ID_TYPE_1_REQ_CAN_ID_ECU_1, PID_SUPPORT_INFO_00);
		S_connect_time = Timer_ctrl_GetTime1ms();
		S_retry_wait_time = S_connect_time;
	}
	else
	{
		if (Timer_ctrl_TimeCompare1ms(S_connect_time, JUDGE_WAIT_TIME) == N_NG)
		{
			/* レスポンス確認 */
			ret_code = receive_response(ISO_CAN_REQ_TYPE_1A, PID_SUPPORT_INFO_00, &can_id, &response, NULL);
			if ((ret_code == E_RC_OK) && (response == ISO_CAN_RESP_ACK)) {
				/* 接続タイプ 1A 確定して、PID情報収集状態へ遷移 */
				S_can_id = can_id & 0x0000FFF7UL;
				S_ccc_pid_mode = CCC_PID_PID_INFO;
				S_connection_type = ISO_CAN_REQ_TYPE_1A;
				S_tx_flg = N_OFF;

				OUTLOG(OutputLog("[ISOCAN]TYPE-1A\n");)
			}
		}
		else
		{
			/* レスポンス待ちタイムアウトの場合 Type 1B リクエスト発行へ遷移 */
			S_ccc_pid_mode = CCC_PID_STD_CAN_1B;
			S_tx_flg = N_OFF;
		}
	}
}


/********************************************************
 * PIDモード:TYPE-1B接続確認
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		車両がTYPE-1Bに対応しているか判定を行う．
 * 			規定時間が過ぎるまでに正常なレスポンスが得られた場合、
 * 			PIDモードを対応PID対応取得に設定する．
 * 			規定時間中に正常なレスポンスが得られなかった場合、
 * 			PIDモードをTYPE-1T接続確認に設定する．
********************************************************/
static void S_ccc_pid_std_can_1b( void )
{
	E_RC				ret_code;
	UI_32				can_id;
	E_ISO_CAN_RESPONSE	response;
	SUPPORTED_PID_INFO	sup_pid_info;

	if( S_tx_flg == N_OFF)
	{
		S_tx_flg = N_ON;

		/* メッセージバッファの設定(受信ID:0x7E8)	*/
		Can_drv_change_message_buffer(CAN_CH_ISO_CAN, ISO_CAN_MSG_ID_RX_MIN, CAN_ID_TYPE_1_RES_CAN_ID_ECU_1);
		Can_drv_clear_rx_data(CAN_CH_ISO_CAN);

		/* 通常対応PIDリクエスト */
		(void)send_request(ISO_CAN_REQ_TYPE_1B, CAN_ID_TYPE_1_REQ_CAN_ID_ADDRESS, PID_SUPPORT_INFO_00);
		S_connect_time = Timer_ctrl_GetTime1ms();
	}
	else
	{
		if (Timer_ctrl_TimeCompare1ms(S_connect_time, JUDGE_WAIT_TIME) == N_NG)
		{
			/* レスポンス確認 */
			ret_code = receive_response(ISO_CAN_REQ_TYPE_1B, PID_SUPPORT_INFO_00, &can_id, &response, &sup_pid_info);
			if ((ret_code == E_RC_OK) && (response == ISO_CAN_RESP_ACK)) {
				/* 接続タイプ 1B 確定して、PID情報収集状態へ遷移 */
				S_can_id = CAN_ID_TYPE_1_REQ_CAN_ID_ADDRESS;
				S_ccc_pid_mode = CCC_PID_PID_INFO;
				S_connection_type = ISO_CAN_REQ_TYPE_1B;
				S_target_ecu_no = sup_pid_info.ecu_no;
				S_tx_flg = N_OFF;

				OUTLOG(OutputLog("[ISOCAN]TYPE-1B\n");)
			}
		}
		else
		{
			/* レスポンス待ちタイムアウトの場合 Type 1T リクエスト発行へ遷移 */
			S_ccc_pid_mode = CCC_PID_STD_CAN_1T;
			S_tx_flg = N_OFF;
		}
	}
}

/********************************************************
 * PIDモード:TYPE-1T接続確認
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		車両がTYPE-1Tに対応しているか判定を行う．
 * 			規定時間が過ぎるまでに正常なレスポンスが得られた場合、
 * 			PIDモードを対応PID対応取得に設定する．
 * 			規定時間中に正常なレスポンスが得られなかった場合、
 * 			PIDモードをTYPE-2接続確認に設定する．
********************************************************/
static void S_ccc_pid_std_can_1t( void )
{
	E_RC				ret_code;
	UI_32				can_id;
	E_ISO_CAN_RESPONSE	response;

	if( S_tx_flg == N_OFF)
	{
		S_tx_flg = N_ON;

		/* メッセージバッファの設定(受信ID:0x7E8)	*/
		Can_drv_change_message_buffer(CAN_CH_ISO_CAN, ISO_CAN_MSG_ID_RX_MIN, CAN_ID_TYPE_1_RES_CAN_ID_ECU_1);
		Can_drv_clear_rx_data(CAN_CH_ISO_CAN);

		/* 通常対応PIDリクエスト */
		(void)send_request(ISO_CAN_REQ_TYPE_1T, CAN_ID_TYPE_1_REQ_CAN_ID_ECU_1, PID_SUPPORT_INFO_00);
		S_connect_time = Timer_ctrl_GetTime1ms();
	}
	else
	{
		if (Timer_ctrl_TimeCompare1ms(S_connect_time, JUDGE_WAIT_TIME) == N_NG)
		{
			/* レスポンス確認 */
			ret_code = receive_response(ISO_CAN_REQ_TYPE_1T, PID_SUPPORT_INFO_00, &can_id, &response, NULL);
			if ((ret_code == E_RC_OK) && (response == ISO_CAN_RESP_ACK)) {
				/* 接続タイプ 1T 確定して、PID情報収集状態へ遷移 */
				S_can_id = can_id & 0x0000FFF7UL;
				S_ccc_pid_mode = CCC_PID_PID_INFO;
				S_connection_type = ISO_CAN_REQ_TYPE_1T;
				S_tx_flg = N_OFF;

				OUTLOG(OutputLog("[ISOCAN]TYPE-1T\n");)
			}
		}
		else
		{
			/* レスポンス待ちタイムアウトの場合 Type 2 リクエスト発行へ遷移 */
			S_ccc_pid_mode = CCC_PID_EXT_CAN;
			S_tx_flg = N_OFF;
		}
	}
}

/********************************************************
 * PIDモード:TYPE-2接続確認
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		車両がTYPE-2に対応しているか判定を行う．
 * 			規定時間が過ぎるまでに正常なレスポンスが得られた場合、
 * 			PIDモードを対応PID対応取得に設定する．
 * 			規定時間中に正常なレスポンスが得られなかった場合、
 * 			リクエスト回数が5回以下であればPIDモードをCANリクエスト送信待ちに設定する．
 * 			リクエスト回数が6回以上の場合は制御モードを通信エラー状態に設定する．
********************************************************/
static void S_ccc_pid_ext_can( void )
{
	UI_32				can_id;
	E_ISO_CAN_RESPONSE	response;
	SUPPORTED_PID_INFO	sup_pid_info;
	UI_8				i;

	/* 送信ID確認(29bit) */
	
	if( S_tx_flg == N_OFF)
	{
		S_tx_flg = N_ON;
		S_sup_pid_info_count = 0;

		/* メッセージバッファの設定(受信ID:0x18DAF1xx)	*/
		for (i = 0; i < 10; i++) {
			Can_drv_change_message_buffer_mask(CAN_CH_ISO_CAN, ISO_CAN_MSG_ID_RX_MIN + i, CAN_ID_TYPE_2_RES_CAN_ID, 0xFFFFFF00UL);
		}
		Can_drv_clear_rx_data(CAN_CH_ISO_CAN);

		/* 通常対応PIDリクエスト */
		(void)send_request(ISO_CAN_REQ_TYPE_2, CAN_ID_TYPE_2_REQ_CAN_ID, PID_SUPPORT_INFO_00);
		S_connect_time = Timer_ctrl_GetTime1ms();
	}
	else
	{
		if (Timer_ctrl_TimeCompare1ms(S_connect_time, JUDGE_WAIT_TIME) == N_NG)
		{
			/* レスポンス確認 */
			while(receive_response(ISO_CAN_REQ_TYPE_2, PID_SUPPORT_INFO_00, &can_id, &response, &sup_pid_info) == E_RC_OK) {
				if(response == ISO_CAN_RESP_ACK) {
					if(S_sup_pid_info_count < SUP_PID_INFO_MAX_COUNT) {
						(void)memcpy(&S_sup_pid_info[S_sup_pid_info_count], &sup_pid_info, sizeof(SUPPORTED_PID_INFO));
						S_sup_pid_info_count++;
					}
				}
			}
		}
		else
		{
			S_tx_flg = N_OFF;
			S_target_ecu_no = 0;

			/* メッセージバッファの設定(未使用にする)	*/
			for (i = 0; i < 10; i++) {
				Can_drv_change_message_buffer_mask(CAN_CH_ISO_CAN, ISO_CAN_MSG_ID_RX_MIN + i, 0, 0);
			}
			Can_drv_clear_rx_data(CAN_CH_ISO_CAN);

			if(S_sup_pid_info_count > 0) {
				/* データ有効数の一番多いECUを選択する */
				S_target_ecu_no = select_ecu_for_type_2();
			}

			if(S_target_ecu_no > 0) {
				/* ECUが選択できた */
				/* 接続タイプ 2 確定して、PID情報収集状態へ遷移 */
				S_can_id = CAN_ID_TYPE_2_REQ_TEMP | ((UI_32)S_target_ecu_no << 8);
				S_ccc_pid_mode = CCC_PID_PID_INFO;
				S_connection_type = ISO_CAN_REQ_TYPE_2;

				/* メッセージバッファの設定(受信ID:0x18DAF1xx  xxはecu no)	*/
				Can_drv_change_message_buffer(CAN_CH_ISO_CAN, ISO_CAN_MSG_ID_RX_MIN, (CAN_ID_TYPE_2_RES_TEMP | (UI_32)S_target_ecu_no));

				OUTLOG(OutputLog("[ISOCAN]TYPE-2 EcuNo:%d\n", S_target_ecu_no);)
			}
			else {
				S_connect_check_cnt++;
				if(S_connect_check_cnt < 6) {
					S_ccc_pid_mode = CCC_PID_WAIT;
				}
				else {
					S_ccc_set_mode(CCC_MODE_CONNECT_ERROR);
					OUTLOG(OutputLog("[ISOCAN]ERR MODE\n");)
				}
			}
		}
	}
}

/********************************************************
 * PIDモード:対応PID取得
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		対応PIDの取得を行う．
 * 			取得が完了したら、PIDモードをリクエスト優先順位作成に設定する．
********************************************************/
static void S_ccc_pid_info( void )
{
	E_RC				ret_code;
	UI_32				can_id;
	E_ISO_CAN_RESPONSE	response;
	SUPPORTED_PID_INFO	sup_pid_info;
	UI_32				tmp;

	if(S_pid_seq < 3)
	{
		if( S_tx_flg == N_OFF)
		{
			S_tx_flg = N_ON;

			/* 通常対応PIDリクエスト */
			(void)send_request(S_connection_type, S_can_id, C_CCC_REQ_INDEX[S_pid_seq]);
			S_connect_time = Timer_ctrl_GetTime1ms();
		}
		else
		{
			if (Timer_ctrl_TimeCompare1ms(S_connect_time, JUDGE_WAIT_TIME) == N_NG)
			{
				/* レスポンス確認 */
				ret_code = receive_response(S_connection_type, C_CCC_REQ_INDEX[S_pid_seq], &can_id, &response, &sup_pid_info);
				if ((ret_code == E_RC_OK) && (response == ISO_CAN_RESP_ACK)) {
					S_tx_flg = N_OFF;

					tmp = sup_pid_info.pid_supported[0];
					tmp <<= DATA1_SHIFT;
					tmp |= sup_pid_info.pid_supported[1];
					tmp <<= DATA2_SHIFT;
					tmp |= sup_pid_info.pid_supported[2];
					tmp <<= DATA3_SHIFT;
					tmp |= sup_pid_info.pid_supported[3];

					OUTLOG(OutputLog("[ISOCAN]%d PID:%08X\n", S_pid_seq, tmp);)

					/* リクエスト格納bitがズレている事への対策 */
					tmp >>= SHIFT_1BIT;

					S_correspondence[C_CCC_RES_OFFSET[S_pid_seq]] = (UI_16)(tmp >> TAKEOUT_HIGH_16BIT);
					S_correspondence[C_CCC_RES_OFFSET[S_pid_seq]+1] = (UI_16)(tmp & TAKEOUT_LOW_16BIT);

					S_pid_seq++;
				}
			}
			else {
				S_tx_flg = N_OFF;
				S_pid_seq++;
			}
		}
	}
	else
	{
		S_ccc_pid_mode = CCC_PID_MAKE_REQUEST;
	}
}

/********************************************************
 * PIDモード:リクエスト優先順位作成
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		得られたPIDに対して、リクエスト優先順位に基づきリクエストテーブルを作成する．
 * 			作成が完了したら制御モードをPIDリクエスト送信状態に設定する．
********************************************************/
static void S_ccc_pid_make_request( void )
{
	UI_8 group;
	UI_8 value;
	UI_8 i;

	/* 対象PIDを優先順位順に走査 */
	S_priority_1_pid_count = 0;
	S_priority_2_pid_count = 0;
	for(i = 0; i < MAX_REQUEST_NUMBER; i++) {
		/* このPIDがサポートされているかチェック */
		group = C_correspondence[i] >> TAKEOUT_HIGH_4BIT;			/* 上位ビットで種別判断 */
		value = C_correspondence[i] & TAKEOUT_LOW_4BIT;				/* 下位ビットで値判断 */
		
		if((S_correspondence[group] & (JUDGE_REQ_BIT >> value)) != NO_RESPONSE){
			if(i < MAX_COUNT_PRIORITY_1_PID) {
				/* 優先順位１のスケジュールに追加 */
				S_priority_1_pid[S_priority_1_pid_count] = C_correspondence[i];
				S_priority_1_pid_count++;
				OUTLOG(OutputLog("[ISOCAN]pri1-%d PID:%02X\n", S_priority_1_pid_count, C_correspondence[i]);)
			}
			else {
				/* 優先順位２のスケジュールに追加 */
				S_priority_2_pid[S_priority_2_pid_count] = C_correspondence[i];
				S_priority_2_pid_count++;
				OUTLOG(OutputLog("[ISOCAN]pri2-%d PID:%02X\n", S_priority_2_pid_count, C_correspondence[i]);)
			}
		}
	}

	S_ccc_set_mode( CCC_MODE_REQUEST_PID );
}

/********************************************************
 * PIDモード:CANリクエスト送信待ち
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		TYPE-1Aリクエストから5sec待つ．（その間は何もしない）
 * 			5sec経過後に、PIDモードをTYPE-1A接続確認に設定する．
********************************************************/
static void S_ccc_pid_wait( void )
{
	/**	<ol>	*/
	/**	<li>TYPE-1Aリクエストを送信してから5sec待つ	*/
	if (Timer_ctrl_TimeCompare1ms(S_retry_wait_time, JUDGE_RETRY_WAIT_TIME) == N_OK) {
		/**	<li>5sec経過していたのならば、対応PID数取得モード状態をCCC_PID_STD_CAN_1Aに変更する	*/
		S_ccc_pid_mode = CCC_PID_STD_CAN_1A;
	}
	/**	</ol>	*/

}

/********************************************************/
/* 関数名称		: send_request							*/
/* 機能の説明	: リクエスト送信処理					*/
/* 引数			: (I)req_type : リクエストタイプ		*/
/* 戻り値		: 送信結果								*/
/*				   E_RC_OK:送信成功, E_RC_NG:送信エラー	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/10/24								*/
/* 備考			: 										*/
/********************************************************/
static E_RC send_request(E_ISO_CAN_REQ_TYPE req_type, UI_32 can_id, UI_8 pid)
{
	UI_8  msg_data[8];
	UI_8  msg_len;
	E_RC  ret_code;

	(void)memset(msg_data, 0, sizeof(msg_data));
	msg_len = 8;

	switch(req_type) {
	case ISO_CAN_REQ_TYPE_1A:
		/* type 1A のリクエストメッセージ作成 */
		msg_data[0] = 2;				/* データ数 */
		msg_data[1] = MODE_REQ_NORMAL;	/* モード */
		msg_data[2] = pid;				/* PID */
		break;
	case ISO_CAN_REQ_TYPE_1B:
		/* type 1B のリクエストメッセージ作成 */
		msg_data[0] = 2;
		msg_data[1] = MODE_REQ_NORMAL;
		msg_data[2] = pid;
		break;
	case ISO_CAN_REQ_TYPE_1T:
		/* type 1T のリクエストメッセージ作成 */
		msg_data[0] = 2;
		msg_data[1] = MODE_REQ_TOYOTA;
		msg_data[2] = pid;
		break;
	case ISO_CAN_REQ_TYPE_2:
		/* type 2 のリクエストメッセージ作成 */
		msg_data[0] = 2;
		msg_data[1] = MODE_REQ_NORMAL;
		msg_data[2] = pid;
		break;
	default:
		return E_RC_NG;
	}

	/* リクエストメッセージ送信 */
	ret_code = Can_drv_set_tx_data(CAN_CH_ISO_CAN, 1, can_id, msg_data, msg_len);
	return ret_code;
}

/********************************************************/
/* 関数名称		: receive_response						*/
/* 機能の説明	: レスポンス受信処理					*/
/* 引数			: (I)req_type : リクエストタイプ		*/
/* 				: (O)response : 応答結果				*/
/* 				: (O)sup_pid_info : PID対応情報			*/
/* 戻り値		: 受信結果								*/
/*				   E_RC_OK:受信成功, E_RC_NG:受信エラー	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/10/24								*/
/* 備考			: 										*/
/*  sup_pid_infoがNULLの場合には、当該領域へのPID対応   */
/* 情報の格納は行わない。                               */
/********************************************************/
static E_RC receive_response(E_ISO_CAN_REQ_TYPE req_type, UI_8 pid, UI_32 *can_id, E_ISO_CAN_RESPONSE *response, SUPPORTED_PID_INFO *sup_pid_info)
{
	E_RC       ret_code;
	T_Can_data can_data;

	ret_code = Can_drv_get_rx_data(CAN_CH_ISO_CAN, &can_data);
	if(ret_code != E_RC_OK) {
		return ret_code;
	}

/*
	OUTLOG(OutputLog("[ISOCAN]rcv id:%X len:%d %02X %02X %02X %02X %02X %02X %02X %02X\n",
			can_data.id, can_data.len,
			can_data.data[0], can_data.data[1], can_data.data[2], can_data.data[3],
			can_data.data[4], can_data.data[5], can_data.data[6], can_data.data[7]);)
*/

	switch(req_type) {
	case ISO_CAN_REQ_TYPE_1A:
		/* type 1A の応答メッセージチェック */
		if((can_data.id == CAN_ID_TYPE_1_RES_CAN_ID_ECU_1)
		 &&(can_data.data[0] == 6)
		 &&(can_data.data[1] == MODE_RES_NORMAL)
		 &&(can_data.data[2] == pid)) {
		 	*response = ISO_CAN_RESP_ACK;
		 	if(sup_pid_info != NULL) {
		 		sup_pid_info->ecu_no = 1;
				(void)memcpy(sup_pid_info->pid_supported, &can_data.data[3], 4);
		 	}
		}
		else {
		 	*response = ISO_CAN_RESP_NACK;
		}
		break;
	case ISO_CAN_REQ_TYPE_1B:
		/* type 1A の応答メッセージチェック */
		if((can_data.id >= CAN_ID_TYPE_1_RES_CAN_ID_ECU_1)
		 &&(can_data.id <= CAN_ID_TYPE_1_RES_CAN_ID_ECU_8)
		 &&(can_data.data[0] == 6)
		 &&(can_data.data[1] == MODE_RES_NORMAL)
		 &&(can_data.data[2] == pid)) {
		 	*response = ISO_CAN_RESP_ACK;
		 	if(sup_pid_info != NULL) {
		 		sup_pid_info->ecu_no = (UI_8)((can_data.id & 0x00000007) + 1);
				(void)memcpy(sup_pid_info->pid_supported, &can_data.data[3], 4);
		 	}
		}
		else {
		 	*response = ISO_CAN_RESP_NACK;
		}
		break;
	case ISO_CAN_REQ_TYPE_1T:
		/* type 1T の応答メッセージチェック */
		if((can_data.id == CAN_ID_TYPE_1_RES_CAN_ID_ECU_1)
		 &&(can_data.data[0] == 6)
		 &&(can_data.data[1] == MODE_RES_TOYOTA)
		 &&(can_data.data[2] == pid)) {
		 	*response = ISO_CAN_RESP_ACK;
		 	if(sup_pid_info != NULL) {
		 		sup_pid_info->ecu_no = 1;
				(void)memcpy(sup_pid_info->pid_supported, &can_data.data[3], 4);
		 	}
		}
		else {
		 	*response = ISO_CAN_RESP_NACK;
		}
		break;
	case ISO_CAN_REQ_TYPE_2:
		if(((can_data.id & 0xffffff00UL) == CAN_ID_TYPE_2_RES_CAN_ID)
		 &&(can_data.data[0] == 6)
		 &&(can_data.data[1] == MODE_RES_NORMAL)
		 &&(can_data.data[2] == pid)) {
		 	*response = ISO_CAN_RESP_ACK;
		 	if(sup_pid_info != NULL) {
		 		sup_pid_info->ecu_no = (UI_8)(can_data.id & 0x000000ff);
				(void)memcpy(sup_pid_info->pid_supported, &can_data.data[3], 4);
		 	}
		}
		else {
		 	*response = ISO_CAN_RESP_NACK;
		}
		break;
	default:
		return E_RC_NG;
	}

	*can_id = can_data.id;
	return E_RC_OK;
}

/********************************************************/
/* 関数名称		: select_ecu_for_type_2					*/
/* 機能の説明	: type 2 要求先ECUの選択処理			*/
/* 引数			: void									*/
/* 戻り値		: 要求先ECU番号							*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/10/24								*/
/* 備考			: 										*/
/********************************************************/
static UI_8 select_ecu_for_type_2(void)
{
	UI_8 i;
	UI_8 enable_count;
	UI_8 max_enable_count  = 0;
	UI_8 max_enable_ecu_no = 0;

	/* すべての取得済みPID対応情報を走査 */
	for(i = 0; i < S_sup_pid_info_count; i++) {
		/* 有効PID数を取得 */
		enable_count = get_enable_pid_count(S_sup_pid_info[i].pid_supported);

		/* 最大のものを特定する */
		if(enable_count > max_enable_count) {
			max_enable_count  = enable_count;
			max_enable_ecu_no = S_sup_pid_info[i].ecu_no;
		}
	}

	/* 有効PIDが最大のECU番号をリターン */
	return max_enable_ecu_no;
}

/********************************************************/
/* 関数名称		: get_enable_pid_count					*/
/* 機能の説明	: 有効PID数取得処理						*/
/* 引数			: (i)pid_supported : PID対応情報一覧	*/
/* 戻り値		: 有効PID数								*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/10/24								*/
/* 備考			: 										*/
/*   入力されたpid_supportedから、取得したいPIDのサポ   */
/* ート数をカウントしてリターンする。					*/
/********************************************************/
static UI_8 get_enable_pid_count(const UI_8 *pid_supported)
{
	UI_8 i;
	UI_8 byte_index;
	UI_8 bit_index;
	UI_8 enable_count = 0;
	
	/* 取得対象PIDを走査 */
	for(i = 0; i < MAX_REQUEST_NUMBER; i++) {
		/* 0x20までのPIDについて処理する */
		if(C_correspondence[i] <= 0x20) {
			/* PID対応情報のByte/bitインデックス算出 */
			byte_index = (C_correspondence[i] - 1)  / 8;
			bit_index  = 7 - ((C_correspondence[i] - 1) % 8);

			/* 当該ビットが立っていれば有効数をインクリメント */
			if((pid_supported[byte_index] & (UI_8)(1<< bit_index)) != 0) {
				enable_count++;
			}
		}
	}

	return(enable_count);
}

/********************************************************/
/* 関数名称		: parse_response_data					*/
/* 機能の説明	: レスポンスデータのパース処理			*/
/* 引数			: (I)t_can_data     : 受信データ		*/
/* 				: (O)pid            : PID				*/
/* 				: (O)value_data_len : 当該PIDのValueデータ長*/
/* 				: (O)value_data     : 当該PIDのValueデータ  */
/* 戻り値		: パース結果							*/
/*				   E_RC_OK:受容可能応答データ			*/
/*				   E_RC_NG:受容不可データー				*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/11/07								*/
/* 備考			: 										*/
/********************************************************/
static E_RC parse_response_data(const T_Can_data *t_can_data, UI_8 *pid, UI_8 *value_data_len, UI_8 *value_data)
{
	UI_32 resp_msg_id;
	UI_8  resp_value_data_len;
	UI_8  mode_resp;
	UI_8  i;

/*
	OUTLOG(OutputLog("[ISOCAN]rcv id:%X len:%d %02X %02X %02X %02X %02X %02X %02X %02X\n",
					t_can_data->id, t_can_data->len,
					t_can_data->data[0], t_can_data->data[1], t_can_data->data[2], t_can_data->data[3],
					t_can_data->data[4], t_can_data->data[5], t_can_data->data[6], t_can_data->data[7]);)
*/

	/* メッセージIDの妥当性チェック */
	resp_msg_id = get_resp_msg_id(S_connection_type);
	if(t_can_data->id != resp_msg_id) {
/*
		OUTLOG(OutputLog("[ISOCAN]rcv id ERR!!\n");)
*/
		return E_RC_NG;
	}

	/* メッセージデータ先頭のデータ長の妥当性チェック */
	resp_value_data_len = get_value_data_len(t_can_data->data[2]);
	if((t_can_data->data[0] - 2) != resp_value_data_len) {
/*
		OUTLOG(OutputLog("[ISOCAN]rcv len ERR!!\n");)
*/
		return E_RC_NG;
	}

	/* メッセージデータ2byte目のモード値の妥当性チェック */
	if(S_connection_type == ISO_CAN_REQ_TYPE_1T) {
		mode_resp = MODE_RES_TOYOTA;
	}
	else {
		mode_resp = MODE_RES_NORMAL;
	}
	if(t_can_data->data[1] != mode_resp) {
/*
		OUTLOG(OutputLog("[ISOCAN]rcv mode ERR!!\n");)
*/
		return E_RC_NG;
	}

	/* PIDと後続値データをリターンする */
	*pid = t_can_data->data[2];
	*value_data_len = t_can_data->data[0] - 2;
	for(i = 0; i < *value_data_len; i++) {
		value_data[i] = t_can_data->data[i + 3];
	}

	return E_RC_OK;
}

/********************************************************/
/* 関数名称		: update_signal_value					*/
/* 機能の説明	: 信号値の更新							*/
/* 引数			: (I)pid        : PID					*/
/* 				: (I)value_data : 当該PIDのValueデータ	*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/11/07								*/
/* 備考			: 										*/
/********************************************************/
static void update_signal_value(UI_8 pid, UI_8 value_data_len, const UI_8 *value_data)
{
	switch(pid) {
	case PID_ENG_COOL_TEMP:
		/* エンジン水温 */
		S_can_info_eng_cool_temp.data = value_data[0];
		S_can_info_eng_cool_temp.first_flg = 1;
		S_can_info_eng_cool_temp.fail_flg  = 0;
		break;
	case PID_IMA_PRESSURE:
		/* 吸気マニフォールド圧 */
		S_can_info_im_abs_press.data = value_data[0];
		S_can_info_im_abs_press.first_flg = 1;
		S_can_info_im_abs_press.fail_flg  = 0;
		break;
	case PID_ENG_RPM:
		/* エンジン回転数 */
		S_can_info_eng_rpm.data = ((UI_16)value_data[0] << 8) | (UI_16)value_data[1];
		S_can_info_eng_rpm.first_flg = 1;
		S_can_info_eng_rpm.fail_flg  = 0;
		break;
	case PID_VEHICLE_SPEED:
		/* 車速 */
		S_can_info_vehicle_speed.data = value_data[0];
		S_can_info_vehicle_speed.first_flg = 1;
		S_can_info_vehicle_speed.fail_flg  = 0;
		break;
	case PID_INTAKE_AIR_TEMP:
		/* 吸気温 */
		S_can_info_intake_air_temp.data = value_data[0];
		S_can_info_intake_air_temp.first_flg = 1;
		S_can_info_intake_air_temp.fail_flg  = 0;
		break;
	case PID_THROTTLE_POS:
		/* スロットルポジション */
		S_can_info_throttle_pos.data = value_data[0];
		S_can_info_throttle_pos.first_flg = 1;
		S_can_info_throttle_pos.fail_flg  = 0;
		break;
	case PID_CTRL_MOD_VOLT:
		/* 電圧 */
		S_can_info_ctrl_mod_volt.data = ((UI_16)value_data[0] << 8) | (UI_16)value_data[1];
		S_can_info_ctrl_mod_volt.first_flg = 1;
		S_can_info_ctrl_mod_volt.fail_flg  = 0;
		break;
	default:
		break;
	}
}

/********************************************************/
/* 関数名称		: get_resp_msg_id						*/
/* 機能の説明	: 応答メッセージＩＤ取得				*/
/* 引数			: (I)connection_type:接続タイプ			*/
/* 戻り値		: 応答メッセージＩＤ					*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/11/07								*/
/* 備考			: 										*/
/********************************************************/
static UI_32 get_resp_msg_id(E_ISO_CAN_REQ_TYPE connection_type)
{
	UI_32 resp_msg_id = 0;
	
	switch(connection_type) {
	case ISO_CAN_REQ_TYPE_1A:
		resp_msg_id = CAN_ID_TYPE_1_RES_CAN_ID_ECU_1;
		break;
	case ISO_CAN_REQ_TYPE_1B:
		resp_msg_id = CAN_ID_TYPE_1_RES_CAN_ID_ECU_1 + ((UI_32)S_target_ecu_no - 1);
		break;
	case ISO_CAN_REQ_TYPE_1T:
		resp_msg_id = CAN_ID_TYPE_1_RES_CAN_ID_ECU_1;
		break;
	case ISO_CAN_REQ_TYPE_2:
		resp_msg_id = (CAN_ID_TYPE_2_RES_TEMP | (UI_32)S_target_ecu_no);
		break;
	default:
		break;
	}

	/* 応答メッセージＩＤリターン */
	return resp_msg_id;
}

/********************************************************/
/* 関数名称		: get_value_data_len					*/
/* 機能の説明	: 信号データ長の取得					*/
/* 引数			: (I)pid        : PID					*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/16								*/
/* 備考			: 										*/
/********************************************************/
static UI_8 get_value_data_len(UI_8 pid)
{
	UI_8	len;

	switch(pid) {
	case PID_ENG_COOL_TEMP:
		/* エンジン水温 */
		len = 1;
		break;
	case PID_IMA_PRESSURE:
		/* 吸気マニフォールド圧 */
		len = 1;
		break;
	case PID_ENG_RPM:
		/* エンジン回転数 */
		len = 2;
		break;
	case PID_VEHICLE_SPEED:
		/* 車速 */
		len = 1;
		break;
	case PID_INTAKE_AIR_TEMP:
		/* 吸気温 */
		len = 1;
		break;
	case PID_THROTTLE_POS:
		/* スロットルポジション */
		len = 1;
		break;
	case PID_CTRL_MOD_VOLT:
		/* 電圧 */
		len = 2;
		break;
	default:
		len = 0;
		break;
	}
	return len;
}

#endif /* ISOCAN_ENABLE */
