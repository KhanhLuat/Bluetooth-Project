/*--------------------------------------------------------------------------*/
/*	BTモジュール制御														*/
/*--------------------------------------------------------------------------*/
/* 概要																		*/
/* 				BTモジュール制御											*/
/*																			*/
/*	ファイル作成時のテーマ:													*/
/*				ER330 無線アダプタ											*/
/*																			*/
/* 作成			2010/09/24	小林											*/
/* 																			*/
/* 修正			2012/03/23	松本											*/
/*				REQ-001：スタンバイモードを状態遷移から削除					*/
/*				REQ-011：電源ON時のUARTシーケンス変更						*/
/*				REQ-013：Bluetooth接続シーケンス変更						*/
/*				REQ-014：テストモード時処理作成								*/
/*				REQ-015：DUTモード時処理作成								*/
/*				REQ-016：認証モード時処理作成								*/
/* 修正			2020/08/25	(OFT)Kamakura									*/
/*				BLE試作：ANNA-B112制御対応									*/
/*																			*/
/* 備考																		*/
/*				無し														*/
/*																			*/
/*--------------------------------------------------------------------------*/
#include <string.h>														/* 標準ﾍｯﾀﾞｰﾌｧｲﾙ */
#include "System.h"
#include "Timer.h"
#include "timer_ctrl.h"
#include "SSFTSTD_Macro.h"
#include "DTO459_VDV_bt_module_control_TASK.h"							/* BTﾓｼﾞｭｰﾙ制御ﾍｯﾀﾞｰﾌｧｲﾙ */
#include "DTO459_VDV_bt_mod_drv.h"
#include "SHRVSTD_Port_Drv.h"
#include "DTO459_VDV_RringBuffer.h"
#include "dbglog.h"

#define	BTMDL_LOCAL	static
#define BT_SOFT_FLOW_CONTROL
/*--------------------------------------------------------------------------*/
/*	内部マクロ																*/
/*--------------------------------------------------------------------------*/
/**
 *	BTモジュール状態
 */
typedef enum e_bt_module_status {						/* BTモジュール状態 						add REQ-015, 016 */
	STATUS_BT_IDLE = 0,									/* アイドル状態 */
	STATUS_BT_STARTUP,									/* スタートアップ状態 */
	STATUS_BT_COMM_MODE,								/* 接続可能状態 */
	STATUS_BT_PRE_PAIRING_MODE,							/* ペアリング移行状態 */
	STATUS_BT_PAIRING_MODE,								/* ペアリング状態 */
	STATUS_BT_SHUTDOWN,									/* 終了処理中状態 */
	STATUS_BT_ERROR,									/* 異常状態 */
	STATUS_BT_NUM
} E_BT_MODULE_STATUS;

/**
 *	コマンドID
 */
typedef enum e_bmc_cmd {
	BMC_CMD_NONE = 0,									/**< コマンドなし			*/
	BMC_CMD_STARTUP,									/**< BTモジュール起動		*/
	BMC_CMD_SHUTDOWN,									/**< BTモジュール停止		*/
	BMC_CMD_COMM,										/**< 通信モード開始			*/
	BMC_CMD_PAIR,										/**< ペアリングモード開始	*/
	BMC_CMD_NUM											/**< コマンド数				*/
} E_BMC_CMD;

/**
 *	ペアリングプロセス
 */
typedef enum e_bmc_pairing_proc {
	PAIRING_PROC_NONE = 0,								/**< プロセスなし			*/
	PAIRING_PROC_REQ_DISCOVERABLE,						/**< 発見可能要求			*/
	PAIRING_PROC_DISCOVERABLE,							/**< 発見可能				*/
	PAIRING_PROC_WAIT_PAIRING,							/**< ペアリング完了待ち		*/
	PAIRING_PROC_PAIRING,								/**< ペアリング中			*/
	PAIRING_PROC_PAIRING_COMP							/**< ペアリング完了			*/
} E_BMC_PAIRING_PROC;

/**
 *	ペアリング完了チェックモード
 */
typedef enum e_bmc_check_pairing_finished_mode {
	CHECK_PAIRING_FINISHED_MODE_NOT_CMD_READ_END = 0,	/**< チェック結果が処理中の場合はコマンドデータを読み捨てない	*/
	CHECK_PAIRING_FINISHED_MODE_CMD_READ_END			/**< コマンドデータを読み捨てる									*/
} E_BMC_CHECK_PAIRING_FINISHED_MODE;

#define		BT_PACKET_SZ						(46)					/* BTモジュール通信パケットサイズ */
#define		BT_RX_BUFF_SZ						(512)					/* BTモジュール受信データバッファサイズ */
#define		BT_STARTUP_INTERVAL					(199)					/* BTモジュール起動リトライインターバル(2秒) */
#define		BT_AT_CMD_INTERVAL					(99)					/* BTモジュールATコマンド送信インターバル(1秒) */
#define		BT_AT_CMD_RETRYCOUNT				(1)						/* BTモジュールATコマンド送信リトライ回数 */
#define		BT_AT_SEQ_STAT_PROCESSING			(0)						/* ATコマンドシーケンス：処理中 */
#define		BT_AT_SEQ_STAT_COMPLETE				(1)						/* ATコマンドシーケンス：成功 */
#define		BT_AT_SEQ_STAT_FAILED				(2)						/* ATコマンドシーケンス：失敗：機能的な失敗・否定 */
#define		BT_AT_SEQ_STAT_ERROR				(9)						/* ATコマンドシーケンス：異常：通常起こりえない失敗 */

#define 	FRAME_NOTFOUND	 					(0)						/* １フレーム未受信 */
#define 	FRAME_FOUND 						(1)						/* １フレーム受信 */

#define 	SEARCH_HEADER_CR					(0)						/* ヘッダCR検索 */
#define 	SEARCH_HEADER_LF					(1)						/* ヘッダLF検索 */
#define 	SEARCH_FOOTER_CR					(2)						/* フッタCR検索 */
#define 	SEARCH_FOOTER_LF					(3)						/* フッタCR検索 */

#define 	AT_CMD_TERMINATIONCODE				"\r\n"					/* ATコマンド：終端コード*/
#define 	AT_CMD_TERMINATIONLEN				(2)						/* ATコマンド：終端コード長 */
#define		PAIRING_WAIT_COUNT					(49)					/* ペアリング手続き完了カウンタ(500ms)	*/

#define 	DEVNAMESIZE							(6)						/*デバイス名サイズ*/

#ifdef BT_SOFT_FLOW_CONTROL	/* BLE試作 T.Kamakura */
#define		BT_FC_BUF_BYTES						(512)					/* フロー制御バッファサイズ	*/
#define		BT_FC_BYTES_PER_TRIGGER				(10000 / 100)			/* 10msecあたりのバイト数	*/
#endif	/* BT_SOFT_FLOW_CONTROL */

/*--------------------------------------------------------------------------*/
/*	構造体宣言																*/
/*--------------------------------------------------------------------------*/
/**
 * ATコマンドシーケンスデータ
 */
typedef struct {
	const UI_8	tx_len;						/* 送信データ長	*/
	const UI_8*	tx_data;					/* 送信データ（ヌル終端）	*/
	const UI_8	rx_len;						/* 受信（応答）データ長	*/
	const UI_8*	rx_data;					/* 受信（応答）データ（ヌル終端）	*/
	const UI_8	rx_timeout;					/* 受信（応答）タイムアウト（0:タイムアウト無効／0<:インターバル)	*/
} S_BT_AT_SEQ_DATA;

/**
 * ATコマンドシーケンス情報
 */
typedef struct {
	UI_8		status;						/* ステータス	*/
	UI_8		retry_count;				/* リトライ回数 */
	UI_8		interval;					/* リトライ間隔 */
} S_BT_AT_SEQ_INFO;

/*--------------------------------------------------------------------------*/
/*	定数宣言																*/
/*--------------------------------------------------------------------------*/
/**
 *	BLEモジュール初期化状態
 */
typedef enum {
	STATUS_BLE_INIT_WAIT_STARTUP = 0,
	STATUS_BLE_INIT_DISCOVER_MODE_1,
	STATUS_BLE_INIT_CONNECT_MODE_1,
	STATUS_BLE_INIT_SECURITY_MODE_OFF,
	STATUS_BLE_INIT_STORE_0,
	STATUS_BLE_INIT_RESTART,

	STATUS_BLE_INIT_WAIT_STARTUP_2,
	STATUS_BLE_INIT_ECHO_OFF,
	STATUS_BLE_INIT_MANUFACTURER,
	STATUS_BLE_INIT_MODEL_NUM,
	STATUS_BLE_INIT_FIRMWARE_VER,
	STATUS_BLE_INIT_SOFTWARE_VER,
	STATUS_BLE_INIT_SECURITY_TYPE,
	STATUS_BLE_INIT_SECURITY_MODE,
	STATUS_BLE_INIT_LOCAL_NAME,
	STATUS_BLE_INIT_DTR_BEHAVIOR,
	STATUS_BLE_INIT_DSR_OVERRIDE,
	STATUS_BLE_INIT_DSR_CONFIGURATION,
	STATUS_BLE_INIT_BLE_CFG_04,
	STATUS_BLE_INIT_BLE_CFG_05,
	STATUS_BLE_INIT_BLE_CFG_06,
	STATUS_BLE_INIT_BLE_CFG_26,
	STATUS_BLE_INIT_STORE,
	STATUS_BLE_INIT_RESTART_2,

	STATUS_BLE_INIT_WAIT_STARTUP_3,
	STATUS_BLE_INIT_DISCOVER_MODE_2STATUS_BLE_INIT_DISCOVER_MODE_2,
	STATUS_BLE_INIT_CONNECT_MODE_2,
	STATUS_BLE_INIT_PAIRING_MODE,
	STATUS_BLE_INIT_BLE_ROLE,
/* ↓ERRORになるため設定しない（工場出荷と同じ値を設定するのはNG） */
/*	STATUS_BLE_INIT_SERVER_CONFIGURATION, */
	STATUS_BLE_INIT_ENTER_DATA_MODE,
	/* ↑ */
	STATUS_BLE_INIT_NUM
} E_STATUS_BLE_INIT;

/* BTモジュール設定送信データ(初期化) */
BTMDL_LOCAL const S_BT_AT_SEQ_DATA	C_bt_at_seq_data_tbl_Init[STATUS_BLE_INIT_NUM] = {
	{ 0,	(const UI_8*)"",						8,	(const UI_8*)"+STARTUP",	BT_STARTUP_INTERVAL	},	/* 起動待機 */
	{ 10,	(const UI_8*)"AT+UBTDM=1",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* 検出可能モードの設定:GAP非検出モード */
	{ 10,	(const UI_8*)"AT+UBTCM=1",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* 接続モードの設定:GAP非接続モード */
	{ 10,	(const UI_8*)"AT+UBTSM=1",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* セキュリティの設定:工場出荷状態に戻す */
	{ 4,	(const UI_8*)"AT&W",					2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* 現在の設定を保存 */
	{ 11,	(const UI_8*)"AT+CPWROFF",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* 反映のため再起動 */

	{ 0,	(const UI_8*)"",						8,	(const UI_8*)"+STARTUP",	BT_STARTUP_INTERVAL	},	/* 起動待機 */
	{ 4,	(const UI_8*)"ATE0",					2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* エコーOFF */
	{ 38,	(const UI_8*)"AT+UBTLEDIS=1,\"NIPPON SEIKI CO., LTD.\"",	2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* マニュファクチャの設定 */
	{ 23,	(const UI_8*)"AT+UBTLEDIS=2,\"OP-0459\"",2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* モデル番号 */
	{ 21,	(const UI_8*)"AT+UBTLEDIS=3,\"1.0.0\"",	2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* ファームウェア番号 */
	{ 21,	(const UI_8*)"AT+UBTLEDIS=4,\"1.0.0\"",	2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* ソフトウェア番号 */
	{ 10,	(const UI_8*)"AT+UBTST=0",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* セキュリティタイプ:シンプルペアリングモード */
	{ 10,	(const UI_8*)"AT+UBTSM=2",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* セキュリティの設定:セキュリティが有効 */
	{ 21,	(const UI_8*)"AT+UBTLN=\"GRRecorder\"", 2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* ローカル名の設定 */
	{ 5,	(const UI_8*)"AT&D1",					2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* DTRの制御方法の選択:コマンドモード切替制御 */
	{ 5,	(const UI_8*)"AT&S2",					2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* DSRの制御方法の選択:DSRは接続状態を示す */
	{ 12,	(const UI_8*)"AT+UDCFG=3,2",			2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* DSRの表示設定:BLE bond 済デバイス接続状態を表示 */
	{ 15,	(const UI_8*)"AT+UBTLECFG=4,6",			2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* BLE設定:Connection interval minimum */
	{ 15,	(const UI_8*)"AT+UBTLECFG=5,6",			2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* BLE設定:Connection interval maximum */
	{ 15,	(const UI_8*)"AT+UBTLECFG=6,0",			2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* BLE設定:Connection slave latency */
	{ 16,	(const UI_8*)"AT+UBTLECFG=26,0",		2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* BLE設定:LL PDU payload size (Data Length Extension) and ATT MTU size negotiation */
	{ 4,	(const UI_8*)"AT&W",					2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* 現在の設定を保存 */
	{ 11,	(const UI_8*)"AT+CPWROFF",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* 反映のため再起動 */

	{ 0,	(const UI_8*)"",						8,	(const UI_8*)"+STARTUP",	BT_STARTUP_INTERVAL	},	/* 起動待機 */
	{ 10,	(const UI_8*)"AT+UBTDM=1",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* 検出可能モードの設定:GAP非検出モード */
	{ 10,	(const UI_8*)"AT+UBTCM=2",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* 接続モードの設定:GAP接続モード */
	{ 10,	(const UI_8*)"AT+UBTPM=2",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* ペアリングモードの設定:GAPペアリングモード */
	{ 10,	(const UI_8*)"AT+UBTLE=2",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* セントラル/ペリフェラルの設定:ペリフェラル */
/* ↓ERRORになるため設定しない（工場出荷と同じ値を設定するのはNG）*/
/*	{ 11,	(const UI_8*)"AT+UDSC=0,6",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	*//* シリアルポートサービスの設定:サーバID:0 タイプ:SPS */
	{ 4,	(const UI_8*)"ATO1",					2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* データモードに切り替え */
};

/**
 *	発見可能要求状態
 */
typedef enum  {
	STATUS_STA_DSCV_WAIT_COMMAND_MODE = 0,
	STATUS_STA_DSCV_DISCOVERABILITY_MODE,
	STATUS_STA_DSCV_CONNECTABILITY_MODE,
	STATUS_STA_DSCV_LOCAL_NAME,
	/* ↑ */
	STATUS_STA_DSCV_NUM
} E_STATUS_STA_DSCV;

/* BTモジュール設定送信データ(発見可能要求) */
BTMDL_LOCAL const S_BT_AT_SEQ_DATA	C_bt_at_seq_data_tbl_sta_dscv[STATUS_STA_DSCV_NUM] = {
	{ 0,	(const UI_8*)"",						2,	(const UI_8*)"OK",	BT_AT_CMD_INTERVAL	},	/* コマンドモード移行待機 */
	{ 10,	(const UI_8*)"AT+UBTDM=3",				2,	(const UI_8*)"OK",	BT_AT_CMD_INTERVAL	},	/* 検出可能モードの設定:GAP一般検出可能モード */
	{ 10,	(const UI_8*)"AT+UBTCM=2",				2,	(const UI_8*)"OK",	BT_AT_CMD_INTERVAL	},	/* 接続モードの設定:GAP接続モード */
	{ 21,	(const UI_8*)"AT+UBTLN=\"GRRecorder\"",	2,	(const UI_8*)"OK",	BT_AT_CMD_INTERVAL	},	/* ローカル名の設定 */
};

/**
 *	発見不可能要求状態
 */
typedef enum  {
	STATUS_END_DSCV_DUMMY = 0,
	STATUS_END_DSCV_DISCOVERABILITY_MODE,
	STATUS_END_DSCV_CONNECTABILITY_MODE,
	STATUS_END_DSCV_ENTER_DATA_MODE,
	/* ↑ */
	STATUS_END_DSCV_NUM
} E_STATUS_END_DSCV;

/* BTモジュール設定送信データ(発見不可能要求) */
BTMDL_LOCAL const S_BT_AT_SEQ_DATA	C_bt_at_seq_data_tbl_end_dscv[STATUS_END_DSCV_NUM] = {
	{ 0,	(const UI_8*)"",			0,	(const UI_8*)"",	0					},	/* ダミー受信から開始 */
	{ 10,	(const UI_8*)"AT+UBTDM=1",	2,	(const UI_8*)"OK",	BT_AT_CMD_INTERVAL	},	/* 検出可能モードの設定:GAP非検出モード */
	{ 10,	(const UI_8*)"AT+UBTCM=2",	2,	(const UI_8*)"OK",	BT_AT_CMD_INTERVAL	},	/* 接続モードの設定:GAP接続モード */
	{ 4,	(const UI_8*)"ATO1",		2,	(const UI_8*)"OK",	BT_AT_CMD_INTERVAL	},	/* データモードに切り替え */
};

/**
 *	内部状態-外部状態変換テーブル
 */
BTMDL_LOCAL const E_BT_COMM_MDL_STATE C_bmc_state_in2out[STATUS_BT_NUM] = {
	BT_COMM_MDL_STATE_IDLE,				/* STATUS_BT_IDLE,				アイドル状態 */
	BT_COMM_MDL_STATE_STARTUP,			/* STATUS_BT_STARTUP,			起動処理中状態 */
	BT_COMM_MDL_STATE_COMM_MODE,		/* STATUS_BT_COMM_MODE,			接続可能状態 */
	BT_COMM_MDL_STATE_PRE,				/* STATUS_BT_PRE_PAIRING_MODE,	ペアリング移行状態 */
	BT_COMM_MDL_STATE_PAIRING_MODE,		/* STATUS_BT_PAIRING_MODE,		ペアリング状態 */
	BT_COMM_MDL_STATE_SHUTDOWN,			/* STATUS_BT_SHUTDOWN,			終了処理中状態 */
	BT_COMM_MDL_STATE_ERROR,			/* STATUS_BT_ERROR,				異常状態 */
};

/*--------------------------------------------------------------------------*/
/*	内部関数																*/
/*--------------------------------------------------------------------------*/
/* 状態関数群 */
BTMDL_LOCAL void S_bmc_state_idle( E_BMC_CMD cmd );
BTMDL_LOCAL void S_bmc_state_startup( E_BMC_CMD cmd );
BTMDL_LOCAL void S_bmc_state_pre_pairing_mode( E_BMC_CMD cmd );
BTMDL_LOCAL void S_bmc_state_pairing_mode( E_BMC_CMD cmd );
BTMDL_LOCAL void S_bmc_state_comm_mode( E_BMC_CMD cmd );
BTMDL_LOCAL void S_bmc_state_shutdown( E_BMC_CMD cmd );
BTMDL_LOCAL void S_bmc_state_error( E_BMC_CMD cmd );
/* 処理関数群 */
BTMDL_LOCAL void S_bmc_imp_shutdown(void);
BTMDL_LOCAL UI_8 S_bmc_module_init(void);
BTMDL_LOCAL UI_8 S_bmc_set_discoverable(B_8 b_discoverable);
BTMDL_LOCAL UI_8 S_bmc_chk_pairing_finished_cmd1( E_BMC_CHECK_PAIRING_FINISHED_MODE mode );
BTMDL_LOCAL UI_8 S_bmc_chk_pairing_finished_cmd2( E_BMC_CHECK_PAIRING_FINISHED_MODE mode );
BTMDL_LOCAL UI_8 S_bmc_at_command_sequence( const S_BT_AT_SEQ_DATA* at_seq_data_tbl, UI_8 status_count, S_BT_AT_SEQ_INFO* seq_info );
BTMDL_LOCAL UI_8 S_bmc_check_frame(const UI_8* p_data, UI_16 data_len, UI_16* p_frame_index, UI_16* p_frame_len);
#ifdef BT_SOFT_FLOW_CONTROL	/* BLE試作 T.Kamakura */
BTMDL_LOCAL void S_bmc_fc_Init( void );
BTMDL_LOCAL UI_16 S_bmc_fc_GetDataBytes( void );
BTMDL_LOCAL UI_16 S_bmc_fc_SetData(const UI_8* data, UI_16 len);
BTMDL_LOCAL void S_bmc_fc_TriggerProc( void );
#endif	/* BT_SOFT_FLOW_CONTROL */

/* 状態関数テーブル */
BTMDL_LOCAL void ( *const C_bmc_state_func[STATUS_BT_NUM] )( E_BMC_CMD cmd ) = {
	S_bmc_state_idle,					/* STATUS_BT_IDLE, */
	S_bmc_state_startup,				/* STATUS_BT_STARTUP, */
	S_bmc_state_comm_mode,				/* STATUS_BT_COMM_MODE, */
	S_bmc_state_pre_pairing_mode,		/* STATUS_BT_PRE_PAIRING_MODE, */
	S_bmc_state_pairing_mode,			/* STATUS_BT_PAIRING_MODE, */
	S_bmc_state_shutdown,				/* STATUS_BT_SHUTDOWN, */
	S_bmc_state_error,					/* STATUS_BT_ERROR, */
};

/*--------------------------------------------------------------------------*/
/*	内部変数定義																*/
/*--------------------------------------------------------------------------*/
BTMDL_LOCAL	E_BT_MODULE_STATUS						S_Bt_module_status;				/* モジュール動作状態 */
BTMDL_LOCAL	E_BT_COMM_MDL_PEER_STATUS 				S_Bt_peer_status;				/* ピア接続状態 */
BTMDL_LOCAL	S_BT_AT_SEQ_INFO						S_Bt_at_seq_info_init;			/* モジュール初期化リトライ情報 */
BTMDL_LOCAL	S_BT_AT_SEQ_INFO						S_Bt_at_seq_info_sta_dscv;		/* モジュール初期化リトライ情報 */
BTMDL_LOCAL	S_BT_AT_SEQ_INFO						S_Bt_at_seq_info_end_dscv;		/* モジュール初期化リトライ情報 */

BTMDL_LOCAL	UI_8									S_Bt_send_data[BT_PACKET_SZ];	/* UART送信データ */
BTMDL_LOCAL	UI_8									S_Bt_tmp_buf[BT_RX_BUFF_SZ];	/* 受信ﾃﾞｰﾀ取得用テンポラリﾊﾞｯﾌｧ */
BTMDL_LOCAL	UI_8									S_Bt_rx_buf[BT_RX_BUFF_SZ];		/* 受信ﾃﾞｰﾀ格納ﾊﾞｯﾌｧ */
BTMDL_LOCAL	T_RINGBUFFER 							S_Bt_rx_buf_mng;				/* 受信ﾃﾞｰﾀ用リングバッファ管理領域 */

BTMDL_LOCAL	P_BT_COMM_MDL_FUNC_RECEIVE_DATA			S_bpcc_cb_receive_data;			/* データ受信コールバック関数 */
BTMDL_LOCAL	E_BMC_CMD								S_bmc_cmd;						/* 要求コマンド */

BTMDL_LOCAL	E_BMC_PAIRING_PROC						S_Bt_Pairing_Proc;				/* BT ペアリングプロセス */
BTMDL_LOCAL B_8										S_bmc_f_first_init;				/* 初回初期化フラグ */

BTMDL_LOCAL UI_8									S_Bt_Pairing_WaitCount;			/* ペアリング完了待ちカウンタ	*/
#ifdef BT_SOFT_FLOW_CONTROL	/* BLE試作 T.Kamakura */
BTMDL_LOCAL UI_8									S_fc_buf[BT_FC_BUF_BYTES];		/* フロー制御用リングバッファ	*/
BTMDL_LOCAL UI_16									S_fc_buf_w_pos;					/* リングバッファ書き込み位置	*/
BTMDL_LOCAL UI_16									S_fc_buf_r_pos;					/* リングバッファ読み出し位置	*/
BTMDL_LOCAL UI_8									S_fc_send_buff[BT_FC_BYTES_PER_TRIGGER];	/* １タスク周期で送信するバッファ	*/
#endif	/* BT_SOFT_FLOW_CONTROL */
/*--------------------------------------------------------------------------*/
/* RAM初期化 																*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	Bt_comm_mdl_Init										*/
/* 機能の説明	：	初期化種別ﾀｲﾐﾝｸﾞ時に初期値を設定する。					*/
/* 戻値			：	なし													*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
void Bt_comm_mdl_Init( void )
{
	S_Bt_module_status = STATUS_BT_IDLE;									/* モジュール動作状態 初期化 */

	(void)memset( &S_Bt_at_seq_info_init, 0, sizeof(S_Bt_at_seq_info_init) );			/* リトライ情報初期化：モジュール初期化 */
	(void)memset( &S_Bt_at_seq_info_sta_dscv, 0, sizeof(S_Bt_at_seq_info_sta_dscv) );	/* リトライ情報初期化：発見可能要求 */
	(void)memset( &S_Bt_at_seq_info_end_dscv, 0, sizeof(S_Bt_at_seq_info_end_dscv) );	/* リトライ情報初期化：発見不可能要求 */

	S_Bt_peer_status = BT_COMM_MDL_PEER_STATUS_DISCONNECTED;				/* ピア接続状態 初期化 */

	RingBuffer_Init(&S_Bt_rx_buf_mng, S_Bt_rx_buf, sizeof(S_Bt_rx_buf));	/* 受信バッファ初期化 */
	S_Bt_Pairing_Proc = PAIRING_PROC_NONE;									/* BT ペアリングプロセス初期化 */
	S_bmc_f_first_init = D_TRUE;

	S_bpcc_cb_receive_data  = NULL;											/* データ受信コールバック関数 */
	S_bmc_cmd				= BMC_CMD_NONE;									/* コマンド初期化 */

	(void)PortDrv_SetDataBit( IO_PO_GR_BT_RST, 		IO_PO_BIT_BT_RST, 		IO_PO_LEV_ACTIVE_BT_RST );	/* リセット実行 */
	Bt_mod_drv_Init();														/* BTモジュールドライバ初期化 */
#ifdef BT_SOFT_FLOW_CONTROL	/* BLE試作 T.Kamakura */
	/* フロー制御 */
	S_bmc_fc_Init();
#endif	/* BT_SOFT_FLOW_CONTROL */
}


/*--------------------------------------------------------------------------*/
/* BTモジュール状態制御処理 												*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	Bt_comm_mdl_Main										*/
/* 機能の説明	：	BTモジュールの動作状態制御を行う 						*/
/* 				：		電源オフ状態 										*/
/* 				：		コマンド状態 										*/
/* 				：		スキャン状態 										*/
/* 				：		オンライン状態 										*/
/* 				：		DUT状態 						add REQ-015			*/
/* 				：		認証状態 						add REQ-016			*/
/* 引数			：	なし													*/
/* 戻値			：	なし													*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
void Bt_comm_mdl_Main( void )
{
	UI_8 	port_data;
	UI_16 	data_len = 0;
	
	/*	受信データ取得	*/
	(void)Bt_mod_drv_GetRxData(S_Bt_tmp_buf, &data_len);
	
	if (data_len > 0)
	{
		/* リングバッファにデータを設定	*/
		(void)RingBuffer_SetArrayData(&S_Bt_rx_buf_mng, S_Bt_tmp_buf, data_len);
	}
#ifdef BT_SOFT_FLOW_CONTROL	/* BLE試作 T.Kamakura */
	/* フロー制御 */
	S_bmc_fc_TriggerProc();
#endif	/* BT_SOFT_FLOW_CONTROL */

	switch( S_Bt_module_status ) {
	case STATUS_BT_COMM_MODE:
	case STATUS_BT_PRE_PAIRING_MODE:
	case STATUS_BT_PAIRING_MODE:
		/*接続状態をポートから取得:DSR*/
		(void)PortDrv_GetDataBit( IO_PO_GR_BLE_DSR, IO_PO_BIT_BLE_DSR, &port_data );
		if(port_data == IO_PO_LEV_BLE_DSR_HIGH)
		{
			/*接続済*/
			S_Bt_peer_status = BT_COMM_MDL_PEER_STATUS_CONNECTED;
		}
		else
		{
			/*切断*/
			S_Bt_peer_status = BT_COMM_MDL_PEER_STATUS_DISCONNECTED;
		}
		break;
	default:
		/*切断*/
		S_Bt_peer_status = BT_COMM_MDL_PEER_STATUS_DISCONNECTED;
		break;
	}

	/**	状態関数実行	*/
	C_bmc_state_func[S_Bt_module_status]( S_bmc_cmd );


	
}


/*--------------------------------------------------------------------------*/
/* ペアリング状態の取得														*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	Bt_comm_mdl_GetPairedStatus								*/
/* 機能の説明	：	ペアリング状態の取得									*/
/* 引数			：	なし													*/
/* 戻値			：	E_BT_COMM_MDL_PAIRED_STATUS			ペアリング状態 		*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_PAIRED_STATUS Bt_comm_mdl_GetPairedStatus( void )
{
	E_BT_COMM_MDL_PAIRED_STATUS status;
	if (S_Bt_Pairing_Proc == PAIRING_PROC_PAIRING_COMP)
	{
		/* ペアリング完了状態：ペアリングデバイスあり */
		status = BT_COMM_MDL_PAIRED_STATUS_EXIST;
	}
	else
	{
		/* ペアリング完了状態以外:ペアリングデバイスなし */
		status = BT_COMM_MDL_PAIRED_STATUS_NONE;
	}
	return status;
}

/*--------------------------------------------------------------------------*/
/* BTモジュール初期化状態の取得												*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	Get_Bt_module_status									*/
/* 機能の説明	：	BTモジュール初期化状態の取得							*/
/* 引数			：	なし													*/
/* 戻値			：	E_BT_COMM_MDL_STATE			BTモジュール初期化状態 		*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_STATE Bt_comm_mdl_GetState( void )
{
	return C_bmc_state_in2out[S_Bt_module_status];
}

/*--------------------------------------------------------------------------*/
/* BTモジュールPeer接続状態の取得											*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	Bt_comm_mdl_GetPeerState								*/
/* 機能の説明	：	BTモジュールPeer接続状態の取得 							*/
/* 引数			：	なし													*/
/* 戻値			：	E_BT_COMM_MDL_PEER_STATUS	BTモジュールPeer接続状態	*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_PEER_STATUS Bt_comm_mdl_GetPeerState( void )
{
	return S_Bt_peer_status;
}

/*--------------------------------------------------------------------------*/
/* BTモジュールスタートアップ要求											*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	Bt_comm_mdl_Startup										*/
/* 機能の説明	：	BTモジュールスタートアップ要求 							*/
/* 引数			：	なし													*/
/* 戻値			：	E_BT_COMM_MDL_RESULT	要求結果						*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_RESULT Bt_comm_mdl_Startup( void )
{
	E_BT_COMM_MDL_STATE status = Bt_comm_mdl_GetState();
	if(status != BT_COMM_MDL_STATE_IDLE)
	{
		return BT_COMM_MDL_RESULT_ERR_INVALID_STATE;
	}
	/**	<li>コマンド登録	*/
	S_bmc_cmd = BMC_CMD_STARTUP;
	return BT_COMM_MDL_RESULT_OK;
}

/*--------------------------------------------------------------------------*/
/* BTモジュールシャットダウン要求											*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	Bt_comm_mdl_Shutdown									*/
/* 機能の説明	：	BTモジュールシャットダウン要求							*/
/* 引数			：	なし													*/
/* 戻値			：	E_BT_COMM_MDL_RESULT	要求結果						*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_RESULT Bt_comm_mdl_Shutdown( void )
{
	E_BT_COMM_MDL_STATE status = Bt_comm_mdl_GetState();
	if(status == BT_COMM_MDL_STATE_IDLE)
	{
		return BT_COMM_MDL_RESULT_ERR_INVALID_STATE;
	}
	/**	<li>コマンド登録	*/
	S_bmc_cmd = BMC_CMD_SHUTDOWN;
	return BT_COMM_MDL_RESULT_OK;
}

/*--------------------------------------------------------------------------*/
/* データ送信要求															*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	Bt_comm_mdl_SendData									*/
/* 機能の説明	：	データ送信要求											*/
/* 引数			：	送信データ、送信サイズ									*/
/* 戻値			：	E_BT_COMM_MDL_RESULT	要求結果						*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_RESULT Bt_comm_mdl_SendData( const UI_8 *buf, UI_16 len )
{
	if ( buf == NULL )
	{
		return BT_COMM_MDL_RESULT_ERR_INVALID_ARG;
	}
	/**	ピア状態チェック	*/
	if ( S_Bt_peer_status != BT_COMM_MDL_PEER_STATUS_CONNECTED )
	{
		return BT_COMM_MDL_RESULT_ERR_INVALID_STATE;
	}
#ifdef BT_SOFT_FLOW_CONTROL	/* BLE試作 T.Kamakura */
	if( S_bmc_fc_SetData( buf, len ) != 1 ) {
		return BT_COMM_MDL_RESULT_ERR_BUSY;
	}
#else	/* BT_SOFT_FLOW_CONTROL */
	/**	データ長確認	*/
	if ( Bt_mod_drv_GetTxEmptyBufferSize() < len )
	{
		return BT_COMM_MDL_RESULT_ERR_BUSY;
	}
	/**	送信	*/
	if ( Bt_mod_drv_SetTxData( buf, len ) != BT_MOD_DRV_OK )
	{
		return BT_COMM_MDL_RESULT_ERR_BUSY;
	}
#endif	/* BT_SOFT_FLOW_CONTROL */
	return BT_COMM_MDL_RESULT_OK;
}


/*--------------------------------------------------------------------------*/
/* 通信モード移行要求要求													*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	Bt_comm_mdl_EnterCommMode								*/
/* 機能の説明	：	通信モード移行要求要求									*/
/* 引数			：	なし													*/
/* 戻値			：	E_BT_COMM_MDL_RESULT	要求結果						*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_RESULT Bt_comm_mdl_EnterCommMode( void )
{
	E_BT_COMM_MDL_STATE status = Bt_comm_mdl_GetState();
	if (( status == BT_COMM_MDL_STATE_IDLE ) 		|| 
		( status == BT_COMM_MDL_STATE_STARTUP )		|| 
		( status == BT_COMM_MDL_STATE_COMM_MODE )	|| 
		( status == BT_COMM_MDL_STATE_SHUTDOWN )	|| 
		( status == BT_COMM_MDL_STATE_ERROR ) )
	{
		return BT_COMM_MDL_RESULT_ERR_INVALID_STATE;
	}
	/**	<li>コマンド登録	*/
	S_bmc_cmd = BMC_CMD_COMM;
	return BT_COMM_MDL_RESULT_OK;
}

/*--------------------------------------------------------------------------*/
/* ペアリングモード移行要求													*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	Bt_comm_mdl_EnterPairingMode							*/
/* 機能の説明	：	ペアリングモード移行要求								*/
/* 引数			：	なし													*/
/* 戻値			：	E_BT_COMM_MDL_RESULT	要求結果						*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_RESULT Bt_comm_mdl_EnterPairingMode( void )
{
	E_BT_COMM_MDL_STATE status = Bt_comm_mdl_GetState();
	if ( status != BT_COMM_MDL_STATE_COMM_MODE )
	{
		return BT_COMM_MDL_RESULT_ERR_INVALID_STATE;
	}
	/**	<li>コマンド登録	*/
	S_bmc_cmd = BMC_CMD_PAIR;
	return BT_COMM_MDL_RESULT_OK;
}


/*--------------------------------------------------------------------------*/
/* iAP接続情報取得要求														*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	Bt_comm_mdl_GetIapAuthInfo								*/
/* 機能の説明	：	iAP接続情報取得要求										*/
/* 引数			：	iAP接続情報												*/
/* 戻値			：	E_BT_COMM_MDL_RESULT	要求結果						*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_RESULT Bt_comm_mdl_GetIapAuthInfo( const S_IAP_CONNECTON_INFO** info )
{
	return BT_COMM_MDL_RESULT_ERR_FATAL;
}

/*--------------------------------------------------------------------------*/
/* データ受信コールバック関数登録											*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	Bt_comm_mdl_RegisterCbReceiveData						*/
/* 機能の説明	：	データ受信コールバック関数登録							*/
/* 引数			：	コールバック関数										*/
/* 戻値			：	なし													*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
void Bt_comm_mdl_RegisterCbReceiveData( const P_BT_COMM_MDL_FUNC_RECEIVE_DATA cb )
{
	S_bpcc_cb_receive_data = cb;
}

/*--------------------------------------------------------------------------*/
/* BLEモジュール(ANNA-B112)初期設定処理 									*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	S_bmc_module_init										*/
/* 機能の説明	：	BLEモジュール(ANNA-B112)の初期設定処理を実行する。 		*/
/* 引数			：	void													*/
/* 戻値			：	UI_8	初期設定処理完了 								*/
/*				：			BT_AT_SEQ_STAT_PROCESSING						*/
/*				：			BT_AT_SEQ_STAT_COMPLETE							*/
/*				：			BT_AT_SEQ_STAT_ERROR							*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
BTMDL_LOCAL UI_8 S_bmc_module_init(void)
{
	UI_8	ret;
	
	ret = S_bmc_at_command_sequence( C_bt_at_seq_data_tbl_Init, STATUS_BLE_INIT_NUM, &S_Bt_at_seq_info_init );

	return ret;
}

/*--------------------------------------------------------------------------*/
/* 発見可能状態切替															*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	S_bmc_set_discoverable									*/
/* 機能の説明	：	発見可能可否状態をATコマンドシーケンスで切り替える。	*/
/* 引数			：	B_8		発見可否(D_TRUE:発見可/D_FALSE:発見不可)		*/
/* 戻値			：	UI_8	BLE発見可能状態設定状態							*/
/*				：			BT_AT_SEQ_STAT_PROCESSING						*/
/*				：			BT_AT_SEQ_STAT_COMPLETE							*/
/*				：			BT_AT_SEQ_STAT_ERROR							*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
BTMDL_LOCAL UI_8 S_bmc_set_discoverable(B_8 b_discoverable)
{
	UI_8	ret;
	
	if( D_TRUE == b_discoverable ) {
		/* 発見可に設定する */
		ret = S_bmc_at_command_sequence( C_bt_at_seq_data_tbl_sta_dscv, STATUS_STA_DSCV_NUM, &S_Bt_at_seq_info_sta_dscv );
	}
	else {
		/* 発見不可に設定する */
		ret = S_bmc_at_command_sequence( C_bt_at_seq_data_tbl_end_dscv, STATUS_END_DSCV_NUM, &S_Bt_at_seq_info_end_dscv );
	}
	return ret;
}

/*--------------------------------------------------------------------------*/
/* ペアリング完了判定１														*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	S_bmc_chk_pairing_finished_cmd1						*/
/* 機能の説明	：	ペアリングの完了をATコマンドシーケンスで確認する。		*/
/* 引数			：	mode	チェックモード									*/
/* 戻値			：	UI_8	ペアリング完了判定状態							*/
/*				：			BT_AT_SEQ_STAT_PROCESSING						*/
/*				：			BT_AT_SEQ_STAT_COMPLETE							*/
/*				：			BT_AT_SEQ_STAT_FAILED							*/
/*				：			BT_AT_SEQ_STAT_ERROR							*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
BTMDL_LOCAL UI_8 S_bmc_chk_pairing_finished_cmd1( E_BMC_CHECK_PAIRING_FINISHED_MODE mode )
{
	UI_16						data_len;
	UI_16 						frame_len;									/* 1フレーム長 */
	UI_16 						cmd_data_len;								/* コマンドデータ長 */
	UI_16						frame_index;								/* バッファ内のフレーム先頭インデックス */
	UI_8						frame_chkeck_result = FRAME_NOTFOUND;		/* フレームチェック結果 */
	UI_8*						p_data;
	const UI_8*					cmd_header = (const UI_8*)"+UUBTB:";
	const UI_16					cmd_header_len = 7;
	UI_8						ret = BT_AT_SEQ_STAT_PROCESSING;

	/* +UUBTB:<接続機器のBTアドレス>,0:成功 を待機する	*/
	/* +UUBTB:<接続機器のBTアドレス>,2:などは失敗		*/

	/* リングバッファから読み込み可能なデータサイズを取得 */
	data_len = RingBuffer_GetCanReadSize(&S_Bt_rx_buf_mng);
	if( 0 < data_len ) {
		/*リングバッファよりデータ取得 */
		(void)RingBuffer_GetArrayData(&S_Bt_rx_buf_mng, S_Bt_tmp_buf, data_len, RINGBUF_ONLY_READ);
		/*1フレーム分のデータがあるかチェック */
		frame_chkeck_result = S_bmc_check_frame(S_Bt_tmp_buf, data_len, &frame_index, &frame_len);
		if( FRAME_FOUND == frame_chkeck_result ) {
			/* フレーム発見 */
			if( (AT_CMD_TERMINATIONLEN * 2) == frame_len ) {
				/* [CRLF][CRLF]受信時のフェール（通常はありえない）*/
				/*リングバッファのデータ読み捨て */
				(void)ReadEndRingBuffer(&S_Bt_rx_buf_mng, frame_index + AT_CMD_TERMINATIONLEN);
			}
			else {
		
				cmd_data_len = frame_len - (AT_CMD_TERMINATIONLEN * 2);						/* 先頭終端の\r\n分減算 */
				p_data = &S_Bt_tmp_buf[frame_index + AT_CMD_TERMINATIONLEN];	/* 実データ先頭アドレス保持 */
				if( cmd_data_len >= (cmd_header_len + 3) ) { /* [*,0] 最低3バイト */
					/* 応答が待機中応答長に合致 */
					if( 0 == memcmp( p_data, cmd_header, (size_t)cmd_header_len ) ) {
						/* コマンドヘッダまで確認したらステータス確認開始 */
						ret = BT_AT_SEQ_STAT_ERROR;	/* この値のまま終了するのは想定外：コマンド解析処理を確認すること */

						/* カンマを確認 */
						if( (UI_8)',' == *(p_data + cmd_data_len - 2) ) {
							/* ステータスを確認 */
							if( (UI_8)'0' == *(p_data + cmd_data_len - 1) ) {
								/* 成功を確認 */
								ret = BT_AT_SEQ_STAT_COMPLETE;
							}
							else {
								ret = BT_AT_SEQ_STAT_FAILED;
							}
						}
						else {
						}
					}
				}
				if (( CHECK_PAIRING_FINISHED_MODE_CMD_READ_END == mode ) || ( BT_AT_SEQ_STAT_PROCESSING != ret )) {
					/*リングバッファのデータ読み捨て */
					(void)ReadEndRingBuffer(&S_Bt_rx_buf_mng, frame_index + frame_len);
				}
			}
		}
	}
	return ret;
}

/*--------------------------------------------------------------------------*/
/* ペアリング完了判定														*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	S_bmc_chk_pairing_finished_cmd2						*/
/* 機能の説明	：	ペアリングの完了をATコマンドシーケンスで確認する。		*/
/* 引数			：	mode	チェックモード									*/
/* 戻値			：	UI_8	ペアリング完了判定状態							*/
/*				：			BT_AT_SEQ_STAT_PROCESSING						*/
/*				：			BT_AT_SEQ_STAT_COMPLETE							*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
BTMDL_LOCAL UI_8 S_bmc_chk_pairing_finished_cmd2( E_BMC_CHECK_PAIRING_FINISHED_MODE mode )
{
	UI_16						data_len;
	UI_16 						frame_len;									/* 1フレーム長 */
	UI_16 						cmd_data_len;								/* コマンドデータ長 */
	UI_16						frame_index;								/* バッファ内のフレーム先頭インデックス */
	UI_8						frame_chkeck_result = FRAME_NOTFOUND;		/* フレームチェック結果 */
	UI_8*						p_data;
	const UI_8*					cmd_header = (const UI_8*)"+UUDPC:";
	const UI_16					cmd_header_len = 7;
	UI_8						ret = BT_AT_SEQ_STAT_PROCESSING;

	/* +UUDPC:<ハンドル>,<タイプ>,<プロファイル>,<アドレス>,<フレームサイズ> を待機する	*/

	/* リングバッファから読み込み可能なデータサイズを取得 */
	data_len = RingBuffer_GetCanReadSize(&S_Bt_rx_buf_mng);
	if( 0 < data_len ) {
		/*リングバッファよりデータ取得 */
		(void)RingBuffer_GetArrayData(&S_Bt_rx_buf_mng, S_Bt_tmp_buf, data_len, RINGBUF_ONLY_READ);
		/*1フレーム分のデータがあるかチェック */
		frame_chkeck_result = S_bmc_check_frame(S_Bt_tmp_buf, data_len, &frame_index, &frame_len);
		if( FRAME_FOUND == frame_chkeck_result ) {
			/* フレーム発見 */
			if( (AT_CMD_TERMINATIONLEN * 2) == frame_len ) {
				/* [CRLF][CRLF]受信時のフェール（通常はありえない） */
				/*リングバッファのデータ読み捨て */
				(void)ReadEndRingBuffer(&S_Bt_rx_buf_mng, frame_index + AT_CMD_TERMINATIONLEN);
			}
			else {
				cmd_data_len = frame_len - (AT_CMD_TERMINATIONLEN * 2);						/* 先頭終端の\r\n分減算 */
				p_data = &S_Bt_tmp_buf[frame_index + AT_CMD_TERMINATIONLEN];	/* 実データ先頭アドレス保持 */
				if( cmd_data_len >= (cmd_header_len + 9) ) { /* [*,*,*,*,*] 最低9バイト */
					/* 応答が待機中応答長に合致 */
					if( 0 == memcmp( p_data, cmd_header, (size_t)cmd_header_len ) ) {
						/* 待機中応答に合致 */
						ret = BT_AT_SEQ_STAT_COMPLETE;
					}
				}
				if (( CHECK_PAIRING_FINISHED_MODE_CMD_READ_END == mode ) || ( BT_AT_SEQ_STAT_PROCESSING != ret )) {
					/*リングバッファのデータ読み捨て */
					(void)ReadEndRingBuffer(&S_Bt_rx_buf_mng, frame_index + frame_len);
				}
			}
		}
	}
	return ret;
}

/*--------------------------------------------------------------------------*/
/* 汎用ATコマンドシーケンス実行関数											*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	S_bmc_at_command_sequence								*/
/* 機能の説明	：	ATコマンドシーケンスを実行する。						*/
/* 引数			：	なし													*/
/* 戻値			：	UI_8	ペアリング完了判定状態							*/
/*				：			BT_AT_SEQ_STAT_PROCESSING						*/
/*				：			BT_AT_SEQ_STAT_COMPLETE							*/
/*				：			BT_AT_SEQ_STAT_ERROR							*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
BTMDL_LOCAL UI_8 S_bmc_at_command_sequence( const S_BT_AT_SEQ_DATA* at_seq_data_tbl, UI_8 status_count, S_BT_AT_SEQ_INFO* seq_info )
{
	const S_BT_AT_SEQ_DATA*		at_seq_data;								/* ATコマンドシーケンスデータ	*/
	UI_16						data_len;
	UI_16 						frame_len;									/* 1フレーム長 */
	UI_16						frame_index;								/* バッファ内のフレーム先頭インデックス */
	UI_8						frame_chkeck_result = FRAME_NOTFOUND;		/* フレームチェック結果 */
	UI_8*						p_data;
	UI_8						Bt_send_len;								/* UART送信データ長 */
	B_8							f_recv = D_FALSE;							/* UART受信フラグ */
	B_8							f_send = D_FALSE;							/* UART送信フラグ */
	B_8							f_echo = D_FALSE;							/* ECHO受信フラグ */
	UI_8						ret = BT_AT_SEQ_STAT_PROCESSING;			/* 初期設定処理の未完了設定 */

	/* カレント状態のATコマンドシーケンスを取得 */
	at_seq_data = &at_seq_data_tbl[seq_info->status];

	if( 0 < at_seq_data->rx_len ) {
		/* リングバッファから読み込み可能なデータサイズを取得 */
		data_len = RingBuffer_GetCanReadSize(&S_Bt_rx_buf_mng);
		if( 0 < data_len ) {
			/*リングバッファよりデータ取得 */
			(void)RingBuffer_GetArrayData(&S_Bt_rx_buf_mng, S_Bt_tmp_buf, data_len, RINGBUF_ONLY_READ);

			if( (0 < at_seq_data->tx_len) && (data_len >= (at_seq_data->tx_len + AT_CMD_TERMINATIONLEN)) ) {
				/* エコーをキャンセル */
				if( 0 == memcmp( S_Bt_tmp_buf, at_seq_data->tx_data, (size_t)at_seq_data->tx_len ) ) {
					if( 0 == memcmp( S_Bt_tmp_buf + at_seq_data->tx_len, AT_CMD_TERMINATIONCODE, AT_CMD_TERMINATIONLEN ) ) {
						/*リングバッファのデータ読み捨て */
						(void)ReadEndRingBuffer(&S_Bt_rx_buf_mng, (UI_16)at_seq_data->tx_len + AT_CMD_TERMINATIONLEN);
						f_echo = D_TRUE;
					}
				}
			}
			if( D_FALSE == f_echo ) {
				/*1フレーム分のデータがあるかチェック */
				frame_chkeck_result = S_bmc_check_frame(S_Bt_tmp_buf, data_len, &frame_index, &frame_len);
				if( FRAME_FOUND == frame_chkeck_result ) {
					/* フレーム発見 */
					if( (AT_CMD_TERMINATIONLEN * 2) == frame_len ) {
						/* [CRLF][CRLF]受信時のフェール（通常はありえない） */
						/*リングバッファのデータ読み捨て */
						(void)ReadEndRingBuffer(&S_Bt_rx_buf_mng, frame_index + AT_CMD_TERMINATIONLEN);
					}
					else {
						/*リングバッファのデータ読み捨て */
						(void)ReadEndRingBuffer(&S_Bt_rx_buf_mng, frame_index + frame_len);
		
						frame_len -= (AT_CMD_TERMINATIONLEN * 2);						/* 先頭終端の\r\n分減算 */
						p_data = &S_Bt_tmp_buf[frame_index + AT_CMD_TERMINATIONLEN];	/* 実データ先頭アドレス保持 */
						if( frame_len == at_seq_data->rx_len ) {
							/* 応答が待機中応答長と等しい */
							if( 0 == memcmp( p_data, at_seq_data->rx_data, (size_t)frame_len ) ) {
								/* 待機応答検出したらステータス更新 */
								seq_info->status++;
								f_recv = D_TRUE;
							}
						}
					}
				}
			}
		}
	}
	else {
		/* 待機コマンドない場合は受信したことにしてステータス更新 */
		seq_info->status++;
		f_recv = D_TRUE;
	}

	if( D_TRUE == f_recv ) {
		/* 受信したら送信 */
		f_send = D_TRUE;
	}
	else {
		/* 受信しない間はタイムアウト制御 */
		if( 0 < seq_info->interval ) {
			seq_info->interval--;
		}
		else {
			/* タイムアウト */
			if( 0 < at_seq_data->rx_timeout ) {
				/* リトライ回数更新 */
				seq_info->retry_count++;
				if( BT_AT_CMD_RETRYCOUNT >= seq_info->retry_count ) {
					/* リトライ回数制限を超えていないなら再送 */
					f_send = D_TRUE;
				}
				else {
					/* タイムアウト */
					ret = BT_AT_SEQ_STAT_ERROR;
				}
			}
		}
	}

	if( D_TRUE == f_send ) {
		if( seq_info->status < status_count ) {
			at_seq_data = &at_seq_data_tbl[seq_info->status];
			if( 0 < at_seq_data->tx_len ) {
				/* 受信バッファを破棄 */
				RingBuffer_Clear(&S_Bt_rx_buf_mng);

				/* 送信コマンド組み立て */
				(void)memset( S_Bt_send_data, 0, sizeof(S_Bt_send_data) );
				(void)memcpy( S_Bt_send_data, at_seq_data->tx_data, (size_t)at_seq_data->tx_len );
				(void)memcpy( S_Bt_send_data + at_seq_data->tx_len, AT_CMD_TERMINATIONCODE, AT_CMD_TERMINATIONLEN );
				Bt_send_len = at_seq_data->tx_len + AT_CMD_TERMINATIONLEN;

				(void)Bt_mod_drv_SetTxData( S_Bt_send_data, (UI_16)Bt_send_len );		/* UARTコマンド送信 */
			}
			/* リトライ送信カウンタにインターバル時間設定 */
			seq_info->interval = at_seq_data->rx_timeout;
		}
		else {
			/* 終端状態に到達：送信コマンドなし */
			ret = BT_AT_SEQ_STAT_COMPLETE;
		}
	}
	return ret;
}

/*--------------------------------------------------------------------------*/
/* 1フレーム分の受信データか判定											*/
/*--------------------------------------------------------------------------*/
/* 関数名称		：	S_bmc_check_frame										*/
/* 機能の説明	：	BTモジュール(BT401AP2)から受信したデータから			*/
/*				：	1フレーム分受信しているか判定する 						*/
/* 引数			：	UI_8*				受信データバッファ					*/
/* 引数			：	UI_16				受信データ長						*/
/* 引数			：	UI_16*				フレーム先頭インデックス			*/
/* 引数			：	UI_16*				フレーム長							*/
/* 戻値			：	UI_8				FRAME_FOUND		:データあり 		*/
/*				：						FRAME_NOTFOUND	:データなし			*/
/* 備考			：															*/
/*--------------------------------------------------------------------------*/
BTMDL_LOCAL UI_8 S_bmc_check_frame(const UI_8* p_data, UI_16 data_len, UI_16* p_frame_index, UI_16* p_frame_len)
{
	UI_16	i;
	SI_16 	frameTopIndex;
	SI_16 	frameBottomIndex;
	UI_8	result = FRAME_NOTFOUND;
	UI_8	search_procNo = SEARCH_HEADER_CR;

	if ((p_data == NULL) || (p_frame_index == NULL) || (p_frame_len == NULL) || (data_len <= 4))
	{
		return result;
	}
	
	for( i = 0 ; (i < data_len) && (result != FRAME_FOUND) ; i++ )
	{
		switch(search_procNo)
		{
			case SEARCH_HEADER_CR:		/* ヘッダ1チェック'\r' */
				if (p_data[i] == '\r') 
				{
					search_procNo++;
					/* 先頭インデックス保持 */
					frameTopIndex = (SI_16)i;
				}
				break;
			case SEARCH_HEADER_LF:		/* ヘッダ2チェック'\n' */
				if (p_data[i] == '\n') 
				{
					search_procNo++;
				}
				else
				{
					search_procNo = SEARCH_HEADER_CR;
					frameTopIndex = -1;
				}
				break;
			case SEARCH_FOOTER_CR:		/* フッタ1チェック'\r' */
				if (p_data[i] == '\r') 
				{
					search_procNo++;
				}
				break;
			case SEARCH_FOOTER_LF:		/* フッタ2チェック'\n' */
				if (p_data[i] == '\n') 
				{
					/*１フレーム分見つかった */
					search_procNo++;
					/* 終端インデックス保持 */
					frameBottomIndex = (SI_16)i;
					/* 結果あり */
					result = FRAME_FOUND;
					
					/* 1フレームの先頭インデックス */
					*p_frame_index = (UI_16)frameTopIndex;
					/* 1フレームのデータ長算出 */
					*p_frame_len = (UI_16)((frameBottomIndex - frameTopIndex) + 1);
				}
				else
				{
					search_procNo = SEARCH_HEADER_CR;
					frameTopIndex = -1;
				}
				break;
			default:
				break;
		}
	}
	return result;
}

/**
 *
 * 状態関数：Idle
 *
 * @param	[in]	cmd	登録コマンド
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_state_idle( E_BMC_CMD cmd )
{
	UI_16 data_len;
	
	switch ( cmd )
	{
		case BMC_CMD_STARTUP:	/**< BTモジュール起動						*/
			(void)PortDrv_SetDataBit( IO_PO_GR_BT_RST, 		IO_PO_BIT_BT_RST, 		IO_PO_LEV_NONACT_BT_RST );			/* リセット解除 */
			(void)PortDrv_SetDataBit( IO_PO_GR_BLE_SW1,		IO_PO_BIT_BLE_SW1,		IO_PO_LEV_BLE_SW1_LOW );			/* SW1:LOW固定 */
			(void)PortDrv_SetDataBit( IO_PO_GR_BLE_SW2,		IO_PO_BIT_BLE_SW2,		IO_PO_LEV_BLE_SW2_LOW );			/* SW2:LOW固定 */
			(void)PortDrv_SetDataBit( IO_PO_GR_BLE_DTR,		IO_PO_BIT_BLE_DTR,		IO_PO_LEV_BLE_DTR_HIGH );			/* DTR:HIGHで開始 */
			(void)memset( &S_Bt_at_seq_info_init, 0, sizeof(S_Bt_at_seq_info_init) );	/* リトライ情報初期化：モジュール初期化 */
			if( D_TRUE == S_bmc_f_first_init ) {
				S_Bt_at_seq_info_init.status = STATUS_BLE_INIT_WAIT_STARTUP;				/* モジュール初期化状態 初期化 */
				S_bmc_f_first_init = D_FALSE;
			}
			else {
				S_Bt_at_seq_info_init.status = STATUS_BLE_INIT_WAIT_STARTUP_3;				/* モジュール初期化状態 初期化 */
			}
			S_Bt_at_seq_info_init.interval = C_bt_at_seq_data_tbl_Init[S_Bt_at_seq_info_init.status].rx_timeout;
			/* 起動前にBTドライバの受信バッファが残っている（直前の通信データ）場合があるので受信データをすべて取得して破棄する */
			do{
				data_len = 0;
				(void)Bt_mod_drv_GetRxData(S_Bt_tmp_buf, &data_len);
			}while(data_len > 0);
			/* リングバッファも破棄 */
			RingBuffer_Clear(&S_Bt_rx_buf_mng);
			
			/**	BTモジュール起動	*/
			Bt_mod_drv_StartBtModule();
			/**	通信開始	*/
			Bt_mod_drv_StartComm();
			
			S_Bt_module_status = STATUS_BT_STARTUP;									/* 起動処理中状態へ状態遷移 */

			break;
		case BMC_CMD_SHUTDOWN:	/**< BTモジュール停止						*/
		case BMC_CMD_COMM:		/**< 通信モード開始							*/
		case BMC_CMD_PAIR:		/**< ペアリングモード開始					*/
		default:
			S_bmc_cmd = BMC_CMD_NONE;
			break;
	}
}


/**
 *
 * 状態関数：startup
 *
 * @param	[in]	cmd	登録コマンド
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_state_startup( E_BMC_CMD cmd )
{
	UI_8	init_stat;				/* 初期設定状態 */
	switch ( cmd )
	{
		case BMC_CMD_STARTUP:	/**< BTモジュール起動						*/
			init_stat = S_bmc_module_init();							/* BTモジュールの初期設定処理 */
			if (init_stat == BT_AT_SEQ_STAT_COMPLETE) {					/* 初期設定処理完了？ */
#ifdef BT_SOFT_FLOW_CONTROL	/* BLE試作 T.Kamakura */
				/* フロー制御初期化 */
				S_bmc_fc_Init();
#endif	/* BT_SOFT_FLOW_CONTROL */
				S_Bt_module_status = STATUS_BT_COMM_MODE;				/* 通信可能状態へ遷移 */
				
				S_bmc_cmd = BMC_CMD_NONE;
			}
			else if (init_stat == BT_AT_SEQ_STAT_ERROR) {				/* 初期設定処理異常 */
				S_Bt_module_status = STATUS_BT_ERROR;					/* 異常状態へ遷移 */
				
				S_bmc_cmd = BMC_CMD_NONE;
			}
			else {
				/* nop */
			}
			break;
		case BMC_CMD_SHUTDOWN:	/**< BTモジュール停止						*/
			
			S_bmc_imp_shutdown();										/* シャットダウン処理 */
			S_Bt_module_status = STATUS_BT_SHUTDOWN;					/* 終了処理中状態へ遷移 */
			
			break;
		case BMC_CMD_COMM:		/**< 通信モード開始							*/
		case BMC_CMD_PAIR:		/**< ペアリングモード開始					*/
		default:
			S_bmc_cmd = BMC_CMD_NONE;
			break;
	}
}


/**
 *
 * 状態関数：comm_mode
 *
 * @param	[in]	cmd	登録コマンド
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_state_comm_mode( E_BMC_CMD cmd )
{
	UI_16 	data_len;
	
	switch ( cmd )
	{
		case BMC_CMD_SHUTDOWN:	/**< BTモジュール停止						*/

			S_bmc_imp_shutdown();										/* シャットダウン処理 */
			S_Bt_module_status = STATUS_BT_SHUTDOWN;					/* 終了処理中状態へ遷移 */
			
			break;
		case BMC_CMD_PAIR:		/**< ペアリングモード開始					*/
			/* コマンドモードに入る前に受信バッファを破棄 */
			RingBuffer_Clear(&S_Bt_rx_buf_mng);
#ifdef BT_SOFT_FLOW_CONTROL	/* BLE試作 T.Kamakura */
			/* フロー制御初期化 */
			S_bmc_fc_Init();
#endif	/* BT_SOFT_FLOW_CONTROL */

			/* コマンドモードに入る: DTR(HIGH→LOW) */
			(void)PortDrv_SetDataBit( IO_PO_GR_BLE_DTR, IO_PO_BIT_BLE_DTR, IO_PO_LEV_BLE_DTR_LOW );
			/* ※DTRはLOWのまま状態遷移する（HIGHへ戻すのはペアリング状態で）*/

			(void)memset( &S_Bt_at_seq_info_sta_dscv, 0, sizeof(S_Bt_at_seq_info_sta_dscv) );	/* リトライ情報初期化 */
			S_Bt_at_seq_info_sta_dscv.status = STATUS_STA_DSCV_WAIT_COMMAND_MODE;
			S_Bt_at_seq_info_sta_dscv.interval = C_bt_at_seq_data_tbl_sta_dscv[S_Bt_at_seq_info_sta_dscv.status].rx_timeout;

			(void)memset( &S_Bt_at_seq_info_end_dscv, 0, sizeof(S_Bt_at_seq_info_end_dscv) );	/* リトライ情報初期化 */
			S_Bt_at_seq_info_end_dscv.status = STATUS_END_DSCV_DUMMY;
			S_Bt_at_seq_info_end_dscv.interval = C_bt_at_seq_data_tbl_end_dscv[S_Bt_at_seq_info_end_dscv.status].rx_timeout;

			S_Bt_module_status  = STATUS_BT_PRE_PAIRING_MODE;			/* ペアリング移行中状態へ遷移 */
			S_Bt_Pairing_Proc = PAIRING_PROC_REQ_DISCOVERABLE;			/* ペアリングプロセス：発見可能要求状態 */
			break;
		case BMC_CMD_STARTUP:	/**< BTモジュール起動						*/
		case BMC_CMD_COMM:		/**< 通信モード開始							*/
		default:
			S_bmc_cmd = BMC_CMD_NONE;
			break;
	}

	/* リングバッファのデータ長を取得 */
	data_len = RingBuffer_GetCanReadSize(&S_Bt_rx_buf_mng);
	if (data_len > 0)
	{
		/* リングバッファよりデータ取得 */
		(void)RingBuffer_GetArrayData(&S_Bt_rx_buf_mng, S_Bt_tmp_buf, data_len, RINGBUF_READEND);
		/* 受信データあり */
		if (S_bpcc_cb_receive_data != NULL)
		{
			/* コールバック関数登録済み */
			S_bpcc_cb_receive_data( S_Bt_tmp_buf, data_len );
		}
	}
}


/**
 *
 * 状態関数：pre_pairing_mode
 *
 * @param	[in]	cmd	登録コマンド
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_state_pre_pairing_mode( E_BMC_CMD cmd )
{
	UI_8 set_discoverable_stat;
	switch ( cmd )
	{
		case BMC_CMD_SHUTDOWN:	/**< BTモジュール停止						*/
			
			S_bmc_imp_shutdown();							/* シャットダウン処理 */
			S_Bt_module_status = STATUS_BT_SHUTDOWN;		/* 終了処理中状態へ遷移 */
			
			break;
		case BMC_CMD_COMM:		/**< 通信モード開始							*/
		
			S_bmc_imp_shutdown();							/* シャットダウン処理 */
			S_Bt_module_status = STATUS_BT_SHUTDOWN;		/* 終了処理中状態へ遷移 */
			
			break;
		case BMC_CMD_PAIR:		/**< ペアリングモード開始					*/
			/* ペアリングプロセス：発見可能要求状態 */
			set_discoverable_stat = S_bmc_set_discoverable(D_TRUE);
			if( BT_AT_SEQ_STAT_COMPLETE == set_discoverable_stat ) {
				/* BLE発見可能状態設定：完了 */
				S_Bt_module_status = STATUS_BT_PAIRING_MODE;		/* ペアリング状態へ遷移 */
				S_Bt_Pairing_Proc = PAIRING_PROC_DISCOVERABLE;		/* 発見可能状態 */
			}
			else if( BT_AT_SEQ_STAT_ERROR == set_discoverable_stat ) {
				/* BLE発見可能状態設定：異常 */
				S_Bt_module_status = STATUS_BT_ERROR;				/* 異常状態へ遷移 */
			}
			else {
				/* BLE発見可能状態設定：処理中 */
				;
			}
			break;
		case BMC_CMD_STARTUP:	/**< BTモジュール起動						*/
		default:
			S_bmc_cmd = BMC_CMD_NONE;
			break;
	}
	if(S_Bt_module_status != STATUS_BT_PRE_PAIRING_MODE) {
		/* 状態から出るとき */
		(void)PortDrv_SetDataBit( IO_PO_GR_BLE_DTR, IO_PO_BIT_BLE_DTR, IO_PO_LEV_BLE_DTR_HIGH );	/* DTR(LOW→HIGH) */

		if(S_Bt_module_status != STATUS_BT_PAIRING_MODE) {
			/* ペアリングモード以外の状態になる場合 */
			S_Bt_Pairing_Proc = PAIRING_PROC_NONE;		/* BT ペアリングプロセス初期化 */
		}
	}
}


/**
 *
 * 状態関数：pairing_mode
 *
 * @param	[in]	cmd	登録コマンド
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_state_pairing_mode( E_BMC_CMD cmd )
{
	UI_8 pairing_finished_stat;
	UI_8 set_discoverable_stat;
	switch ( cmd )
	{
		case BMC_CMD_SHUTDOWN:	/**< BTモジュール停止						*/
			
			S_bmc_imp_shutdown();							/* シャットダウン処理 */
			S_Bt_module_status = STATUS_BT_SHUTDOWN;		/* 終了処理中状態へ遷移 */
			
			break;
		case BMC_CMD_COMM:		/**< 通信モード開始							*/
		
			S_bmc_imp_shutdown();							/* シャットダウン処理 */
			S_Bt_module_status = STATUS_BT_SHUTDOWN;		/* 終了処理中状態へ遷移 */
			
			break;
		case BMC_CMD_PAIR:		/**< ペアリングモード開始					*/
			if( PAIRING_PROC_DISCOVERABLE == S_Bt_Pairing_Proc ) {
				/* ペアリングプロセス：ペアリング完了判定 */
				pairing_finished_stat = S_bmc_chk_pairing_finished_cmd1( CHECK_PAIRING_FINISHED_MODE_NOT_CMD_READ_END );
				if( BT_AT_SEQ_STAT_COMPLETE == pairing_finished_stat ) {
					/* ペアリング完了判定：完了 */
					S_Bt_Pairing_WaitCount = PAIRING_WAIT_COUNT;
					S_Bt_Pairing_Proc = PAIRING_PROC_WAIT_PAIRING;		/* ペアリング完了待ち状態 */
				}
				else if( BT_AT_SEQ_STAT_FAILED == pairing_finished_stat ) {
					/* ペアリング完了判定：失敗 */
					/* そのまま接続完了を待つ */
				}
				else if( BT_AT_SEQ_STAT_ERROR == pairing_finished_stat ) {
					/* ペアリング完了判定：異常 */
					S_Bt_module_status = STATUS_BT_ERROR;				/* 異常状態へ遷移 */
				}
				else {
					pairing_finished_stat = S_bmc_chk_pairing_finished_cmd2( CHECK_PAIRING_FINISHED_MODE_CMD_READ_END );
					if( BT_AT_SEQ_STAT_COMPLETE == pairing_finished_stat ) {
						/* ペアリング完了判定：完了 */
						S_Bt_Pairing_WaitCount = PAIRING_WAIT_COUNT;
						S_Bt_Pairing_Proc = PAIRING_PROC_WAIT_PAIRING;		/* ペアリング完了待ち状態 */
					}
					else {
						/* ペアリング完了判定：処理中 */
						;
					}
				}
			}
			else if( PAIRING_PROC_WAIT_PAIRING == S_Bt_Pairing_Proc ) {
				/* ペアリング完了後に来る通知コマンドをしばらく待機する */
				S_Bt_Pairing_WaitCount--;
				if( 0 == S_Bt_Pairing_WaitCount ) {
					/* リングバッファクリア */
					RingBuffer_Clear( &S_Bt_rx_buf_mng );

					S_Bt_Pairing_Proc = PAIRING_PROC_PAIRING;			/* ペアリング状態 */
				}
			}
			else if( PAIRING_PROC_PAIRING == S_Bt_Pairing_Proc ) {
				/* ペアリングプロセス：発見不可能要求状態 */
				set_discoverable_stat = S_bmc_set_discoverable(D_FALSE);
				if( BT_AT_SEQ_STAT_COMPLETE == set_discoverable_stat ) {
					/* BLE発見不可能状態設定：完了 */
					S_Bt_Pairing_Proc = PAIRING_PROC_PAIRING_COMP;		/* ペアリング完了状態 */
					S_Bt_module_status = STATUS_BT_COMM_MODE;			/* 通信可能状態へ遷移 */
					S_bmc_cmd = BMC_CMD_NONE;
				}
				else if( BT_AT_SEQ_STAT_ERROR == set_discoverable_stat ) {
					/* BLE発見不可能状態設定：異常 */
					S_Bt_module_status = STATUS_BT_ERROR;				/* 異常状態へ遷移 */
				}
				else {
					/* BLE発見可能状態設定：処理中 */
					;
				}
			}
			else {
				/* 状態異常 */
				S_Bt_module_status = STATUS_BT_ERROR;					/* 異常状態へ遷移 */
			}
			break;
		case BMC_CMD_STARTUP:	/**< BTモジュール起動						*/
		default:
			S_bmc_cmd = BMC_CMD_NONE;
			break;
	}
	if(S_Bt_module_status != STATUS_BT_PAIRING_MODE) {
		if(	S_Bt_Pairing_Proc != PAIRING_PROC_PAIRING_COMP) {
			/* ペアリング完了状態以外で抜ける場合は初期化 */
			S_Bt_Pairing_Proc = PAIRING_PROC_NONE;						/* BT ペアリングプロセス初期化 */
		}
	}
}




/**
 *
 * 状態関数：shutdown
 *
 * @param	[in]	cmd	登録コマンド
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_state_shutdown( E_BMC_CMD cmd )
{
	switch ( cmd )
	{
		case BMC_CMD_SHUTDOWN:	/**< BTモジュール停止						*/

			S_bmc_cmd = BMC_CMD_NONE;
			S_Bt_module_status = STATUS_BT_IDLE;			/* アイドル状態へ遷移 */
			
			break;
		case BMC_CMD_COMM:		/**< 通信モード開始							*/
			
			S_bmc_cmd = BMC_CMD_STARTUP;					/* BTモジュール起動コマンド発行 */
			S_Bt_module_status = STATUS_BT_IDLE;			/* アイドル状態へ遷移 */
			break;
		case BMC_CMD_STARTUP:	/**< BTモジュール起動						*/
		case BMC_CMD_PAIR:		/**< ペアリングモード開始					*/
		default:
			S_bmc_cmd = BMC_CMD_NONE;
			break;
	}
}


/**
 *
 * 状態関数：error
 *
 * @param	[in]	cmd	登録コマンド
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_state_error( E_BMC_CMD cmd )
{
	switch ( cmd )
	{
		case BMC_CMD_SHUTDOWN:	/**< BTモジュール停止						*/
			
			S_bmc_imp_shutdown();							/* シャットダウン処理 */
			S_Bt_module_status = STATUS_BT_SHUTDOWN;		/* 終了処理中状態へ遷移 */
			
			break;
		case BMC_CMD_STARTUP:	/**< BTモジュール起動						*/
		case BMC_CMD_COMM:		/**< 通信モード開始							*/
		case BMC_CMD_PAIR:		/**< ペアリングモード開始					*/
		default:
			S_bmc_cmd = BMC_CMD_NONE;
			break;
	}
}

/**
 *
 * 処理関数：shutdown
 *
 * @param  none
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_imp_shutdown(void)
{
	/**	通信停止	*/
	Bt_mod_drv_StopComm();
	/**	BTモジュール停止	*/
	Bt_mod_drv_StopBtModule();
	
	(void)PortDrv_SetDataBit( IO_PO_GR_BT_RST, 		IO_PO_BIT_BT_RST, 		IO_PO_LEV_ACTIVE_BT_RST );			/* リセット実行 */
}

#ifdef BT_SOFT_FLOW_CONTROL	/* BLE試作 T.Kamakura */
/**
 * 
 * フロー制御：初期化
 * 
 * @param	なし
 * @return	なし
 *
 * @note
 */
BTMDL_LOCAL void S_bmc_fc_Init( void )
{
	(void)memset( S_fc_buf, 0, sizeof(S_fc_buf) );
}
#endif	/* BT_SOFT_FLOW_CONTROL */

#ifdef BT_SOFT_FLOW_CONTROL	/* BLE試作 T.Kamakura */
/**
 * 
 * フロー制御：未送信データサイズ取得
 * 
 * @param	なし
 * @return	送信データサイズ(バイト単位)
 *
 * @note
 */
BTMDL_LOCAL UI_16 S_bmc_fc_GetDataBytes( void )
{
	if( S_fc_buf_w_pos >= S_fc_buf_r_pos ) {
		return S_fc_buf_w_pos - S_fc_buf_r_pos;
	}
	else {
		return BT_FC_BUF_BYTES + S_fc_buf_w_pos - S_fc_buf_r_pos;
	}
}
#endif	/* BT_SOFT_FLOW_CONTROL */

#ifdef BT_SOFT_FLOW_CONTROL	/* BLE試作 T.Kamakura */
/**
 * 
 * フロー制御：送信データ設定
 * 
 * @param	data	送信データ先頭アドレス
 * @param	len		送信データサイズ(バイト単位)
 * @return	0	バッファフル
 * @return	1	送信データ設定成功
 *
 * @note
 */
BTMDL_LOCAL UI_16 S_bmc_fc_SetData(const UI_8* data, UI_16 len)
{
	UI_16	i;
	UI_16	empty_bytes;

	empty_bytes = BT_FC_BUF_BYTES - S_bmc_fc_GetDataBytes();
	if( len >= empty_bytes ) {
		return 0;
	}
	for( i = 0; i < len; i++ ) {
		S_fc_buf[S_fc_buf_w_pos] = data[i];
		S_fc_buf_w_pos++;
		S_fc_buf_w_pos %= BT_FC_BUF_BYTES;
	}
	return 1;
}
#endif	/* BT_SOFT_FLOW_CONTROL */

#ifdef BT_SOFT_FLOW_CONTROL	/* BLE試作 T.Kamakura */
/**
 * 
 * フロー制御：送信トリガ処理
 * １トリガ（タスク周期）毎にリングバッファにあるデータのうち送信サイズ分だけ送信する。
 * @param	なし
 * @return	なし
 *
 * @note
 */
BTMDL_LOCAL void S_bmc_fc_TriggerProc( void )
{
	UI_16	i;
	UI_16	data_bytes;
	UI_16	len;

	data_bytes = S_bmc_fc_GetDataBytes();
	if( 0 < data_bytes ) {
		if( BT_FC_BYTES_PER_TRIGGER < data_bytes ) {
			len = BT_FC_BYTES_PER_TRIGGER;
		}
		else {
			len = data_bytes;
		}
		if( Bt_mod_drv_GetTxEmptyBufferSize() >= len ) {
			/* ここでフロー制御済の送信データを送信 */
			for( i = 0; i < len; i++ ) {
				S_fc_send_buff[i] = S_fc_buf[S_fc_buf_r_pos];
				S_fc_buf_r_pos++;
				S_fc_buf_r_pos %= BT_FC_BUF_BYTES;
			}
			(void)Bt_mod_drv_SetTxData( S_fc_send_buff, len );
		}
	}
}
#endif	/* BT_SOFT_FLOW_CONTROL */

