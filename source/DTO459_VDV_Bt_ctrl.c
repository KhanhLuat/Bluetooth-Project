/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: FR81S								*/
/* Date				: 16/09/02							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Kamakura						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		:									*/
/* date				:									*/
/********************************************************/
/********************************************************/
/* file DTO459_VDV_Bt_ctrl.c							*/
/* BT制御												*/
/********************************************************/

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include "DTO459_VDV_Bt_ctrl.h"
#include "DTO459_VDV_Sw_ctrl.h"
#include "DTO459_VDV_Eeprom_ctrl.h"
#include "System_control_TASK.h"
#include "DTO459_VDV_bt_module_control_TASK.h"
#include "DTO459_VDV_Signal_ctrl.h"
#include "DTO459_VDV_Common.h"
#include "DTO459_VDV_Advance_ctrl.h"
#include "timer_ctrl.h"
#include "dbglog.h"
#include <string.h>

/********************************************************/
/*	デバッグ定義[DEBUG]									*/
/********************************************************/

/********************************************************/
/*	動作モード:要求されたIDが正しいかどうかチェック	*/
/********************************************************/
/* #define D_BTC_CHECK_REQ_ID */

#define BTC_LOCAL	static

/********************************************************/
/*	定数定義											*/
/********************************************************/
#define D_BTC_MAX_SIGNAL_COUNT			(50U)			/*	最大信号数				*/
#define D_BTC_MAX_ERRCODE_COUNT			(10U)			/*	最大エラーコード数		*/

/*	VDV BT通信データフォーマット	*/
#define BTC_FORMAT_VERSION				(0x00010000)	/*	フォーマットバージョン	*/
/*											 ^^^^^^
											  | | |
											  | | |
											  | | メンテナスバージョン
											  | マイナーバージョン
											  メジャーバージョン
*/

/* ヘッダ */
/* START	1byte */
#define D_BTC_FMT_HEADER_START			(0x7EU)			/*	START					*/
/* LEN		2byte */
/* TYPE		1byte */
#define D_BTC_FMT_HEADER_BYTES			(4U)			/*	ヘッダサイズ			*/

/* TYPE: データ種別 */
#define D_BTC_FMT_TYPE_INVALID			(0xFFU)			/*	無効なデータ種別				*/
#define D_BTC_FMT_TYPE_REQ_LIST			(0x00U)			/*	車両信号一覧取得(リスト)要求	*/
#define D_BTC_FMT_TYPE_REQ_START		(0x01U)			/*	車両情報(取得開始)要求			*/
#define D_BTC_FMT_TYPE_REQ_STOP			(0x02U)			/*	車両情報(取得停止)要求			*/
#define D_BTC_FMT_TYPE_REP_LIST			(0x10U)			/*	車両信号一覧取得(リスト)応答	*/
#define D_BTC_FMT_TYPE_REP_START		(0x11U)			/*	車両情報(取得開始)応答			*/
#define D_BTC_FMT_TYPE_REP_STOP			(0x12U)			/*	車両情報(取得停止)応答			*/

/* フッタ */
/* CRC		2byte */
/* END		1byte */
#define D_BTC_FMT_FOOTER_END			(0x7FU)			/*	END						*/
#define D_BTC_FMT_FOOTER_BYTES			(3U)			/*	フッタサイズ			*/

#define D_BTC_FMT_BYTES_FIX				(D_BTC_FMT_HEADER_BYTES + D_BTC_FMT_FOOTER_BYTES)

/* 要求データサイズ計算式 */
#define D_BTC_FMT_BYTES_REQ_LIST		(D_BTC_FMT_HEADER_BYTES + D_BTC_FMT_FOOTER_BYTES)
														/*	車両信号一覧取得(リスト)要求	*/
#define D_BTC_FMT_MAX_BYTES_REQ_START	(D_BTC_FMT_HEADER_BYTES + (8 + (2 * D_BTC_MAX_SIGNAL_COUNT)) + D_BTC_FMT_FOOTER_BYTES)
														/*	車両情報(取得開始)要求(最大サイズ)	*/
#define D_BTC_FMT_BYTES_REQ_STOP		(D_BTC_FMT_HEADER_BYTES + D_BTC_FMT_FOOTER_BYTES)
														/*	車両情報(取得停止)要求			*/
/* 応答データサイズ計算式 */
#define D_BTC_FMT_MAX_BYTES_REP_START	(D_BTC_FMT_HEADER_BYTES + (6 + (6 * D_BTC_MAX_SIGNAL_COUNT)+ (6 * D_BTC_MAX_ERRCODE_COUNT)) + D_BTC_FMT_FOOTER_BYTES)
														/*	車両情報(取得開始)応答(最大サイズ)	*/
#define D_BTC_FMT_BYTES_REP_STOP		(D_BTC_FMT_HEADER_BYTES + D_BTC_FMT_FOOTER_BYTES)
														/*	車両情報(取得停止)応答			*/

#define D_BTC_MAX_SEND_BYTES			(((D_BTC_FMT_MAX_BYTES_REP_START + 15) / 16) * 16)	/* 送信データ最大サイズ(16byteアライメント調整)	*/
#define D_BTC_MAX_RECV_BYTES			(((D_BTC_FMT_MAX_BYTES_REQ_START + 15) / 16) * 16)	/* 受信データ最大サイズ(16byteアライメント調整)	*/

/* 送受信バッファサイズ */
#define D_BTC_SEND_BUFF_BYTES			(D_BTC_MAX_SEND_BYTES)
														/*	送信バッファサイズ		*/
#define D_BTC_RECV_BUFF_BYTES			(D_BTC_MAX_RECV_BYTES * 2)		
														/*	受信バッファサイズ		*/

/* 時間 */
#define D_BTC_TIMEOUT_MSEC_PAIRING		((60U * 1000U) / 10U)	/*	ペアリングタイムアウト時間(1カウント10msec)			*/
#define D_BTC_TIMEOUT_MSEC_STARTUP		((5U * 1000U) / 10U)	/*	BTモジュール起動タイムアウト時間(1カウント10msec)	*/
#define D_BTC_TIMEOUT_MSEC_RECV_PACKET	((1U * 1000U) / 10U)	/*	受信パケット検出タイムアウト時間(1カウント10msec)	*/
#define D_BTC_CYCLE_MSEC_SEND_REP_START	(50U / 10U)				/*	車両信号送信間隔(1カウント10msec)					*/

/* BT制御イベント */
#define BTC_EVENT_NONE					(0L)
#define BTC_EVENT_ACC_ON				(1UL << 16)		/*	ACC ON								*/
#define BTC_EVENT_ACC_OFF				(1UL << 17)		/*	ACC OFF								*/
#define BTC_EVENT_REQ_STARTUP			(1UL << 1)		/*	BTモジュール起動要求				*/
#define BTC_EVENT_REQ_SHUTDOWN			(1UL << 2)		/*	BTモジュール停止要求				*/
#define BTC_EVENT_REQ_PAIRING			(1UL << 3)		/*	ペアリング開始要求					*/
#define BTC_EVENT_PAIRING				(1UL << 4)		/*	ペアリング完了						*/
#define BTC_EVENT_PAIRING_TIMEOUT		(1UL << 5)		/*	ペアリングタイムアウト				*/
#define BTC_EVENT_STARTUP				(1UL << 6)		/*	BTモジュール起動完了				*/
#define BTC_EVENT_SHUTDOWN				(1UL << 7)		/*	BTモジュール停止完了				*/
#define BTC_EVENT_PEER_CONNECTED		(1UL << 8)		/*	Peer接続							*/
#define BTC_EVENT_PEER_DISCONNECTED		(1UL << 9)		/*	Peer切断							*/
#define BTC_EVENT_REQ_SIGNAL_LIST		(1UL << 10)		/*	車両信号一覧取得(リスト)要求		*/
#define BTC_EVENT_REQ_START_SEND		(1UL << 11)		/*	車両情報(取得開始)要求				*/
#define BTC_EVENT_REQ_STOP_SEND			(1UL << 12)		/*	車両情報(取得停止)要求				*/
#define BTC_EVENT_SEND_TRIGGER			(1UL << 13)		/*	車両信号送信トリガ					*/
#define BTC_EVENT_STARTUP_TIMEOUT		(1UL << 14)		/*	BTモジュール起動タイムアウト		*/
#define BTC_EVENT_RECV_PACKET_TIMEOUT	(1UL << 15)		/*	パケット受信タイムアウト			*/
#define BTC_EVENT_ERROR					(1UL << 18)		/*	BTモジュール異常検知				*/
#define BTC_EVENT_REQ_UART_SEND			(1UL << 19)		/*	UARTデータ送信						*/
#define BTC_EVENT_REQ_RETURN_SEND		(1UL << 20)		/*	検査モードデータ送信（受信データをそのまま返信）*/

/* BT制御状態 */
typedef enum e_btc_status {
	BTC_STATUS_NONE = 0L,
	BTC_STATUS_ACC_OFF					,				/*	ACC OFF								*/
	BTC_STATUS_WAIT_STARTUP_ACC_ON		,				/*	ACC ONによるBTモジュール起動待ち	*/
	BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF	,				/*	ACC OFFによるBTモジュール停止待ち	*/
	BTC_STATUS_SHUTDOWN					,				/*	BTモジュール停止中					*/
	BTC_STATUS_WAIT_STARTUP				,				/*	BTモジュール起動待ち				*/
	BTC_STATUS_STARTUP					,				/*	BTモジュール起動中					*/
	BTC_STATUS_WAIT_SHUTDOWN			,				/*	BTモジュール停止待ち				*/
	BTC_STATUS_PEEE_CONNECTED_IDLE		,				/*	BT通信中: データ未送信				*/
	BTC_STATUS_PEEE_CONNECTED_SENDING	,				/*	BT通信中: データ送信中				*/
	BTC_STATUS_WAIT_STARTUP_PAIRING		,				/*	ペアリング移行中					*/
	BTC_STATUS_PAIRING					,				/*	ペアリング							*/
	BTC_STATUS_ERROR					,				/*	異常発生							*/
	/* ↑状態はここより上に追加すること */
	BTC_STATUS_COUNT									/*	状態定義数							*/
} E_BTC_STATUS;

/* 時刻種別 */
typedef enum e_btc_time_kind {
	BTC_TIME_KIND_PACKET_FOUND = 0		, 				/*	パケット先頭を発見した最終時刻	*/
	BTC_TIME_KIND_SEND_REP_START		,				/*	車両信号パケット送信した最終時刻*/
	BTC_TIME_KIND_PAIRING_START			,				/*	ペアリングを開始した時刻		*/
	BTC_TIME_KIND_STARTUP				,				/*	BTモジュール起動要求時刻		*/
	/* ↑種別はここより上に追加すること */
	BTC_TIME_KIND_COUNT
} E_BTC_TIME_KIND;

/* BTモジュール起動要因 */
typedef enum e_btc_startup_factor {
	BTC_STARTUP_FACTOR_NONE = 0			,				/*	要因なし						*/
	BTC_STARTUP_FACTOR_ACC_ON			,				/*	ACC ON による起動				*/
	BTC_STARTUP_FACTOR_PAIRING			,				/*	ペアリング開始要求による起動	*/
	BTC_STARTUP_FACTOR_OTHERS							/*	その他要因による停止			*/
} E_BTC_STARTUP_FACTOR;

/* BTモジュール停止要因 */
typedef enum e_btc_shutdown_factor {
	BTC_SHUTDOWN_FACTOR_NONE = 0		,				/*	要因なし						*/
	BTC_SHUTDOWN_FACTOR_ACC_OFF			,				/*	ACC OFF による停止				*/
	BTC_SHUTDOWN_FACTOR_OTHERS							/*	その他要因による停止			*/
} E_BTC_SHUTDOWN_FACTOR;

/********************************************************/
/*	型定義												*/
/********************************************************/
/* リングバッファ */
typedef struct btc_ringbuff {
	UI_8*		pu8_buff;						/*  バッファアドレス			*/
	UI_16		u16_buff_bytes;					/*  バッファ全体サイズ			*/
	UI_16		u16_w_index;					/*	書き込み位置				*/
	UI_16		u16_r_index;					/*	読み出し位置				*/
	UI_16		u16_w_total;					/*	書き込みバイト数(通算)		*/
	UI_16		u16_r_total;					/*	読み出しバイト数(通算)		*/
	UI_8		rsv[2];
} BTC_RINGBUFF;

/* 時間情報 */
typedef struct btc_time_info {
	UI_16		time[BTC_TIME_KIND_COUNT];		/*	種別毎の時刻(1カウント10msec)	*/
	UI_32		flags;							/*	時刻の有効無効を管理		*/
} BTC_TIME_INFO;

/* パケット情報 */
typedef struct {
	UI_16		packet_bytes;					/*	パケット全体の長さ 7E=>7F	*/
	UI_16		length;							/*	ヘッダから抽出した Length	*/
	UI_8		type;							/*	ヘッダから抽出した Type		*/
	UI_8		rsv[3];
	UI_8*		header_part;					/*	バッファ内のヘッダ先頭		*/
	UI_8*		data_part;						/*	バッファ内のデータ先頭		*/
	UI_8*		footer_part;					/*	バッファ内のフッタ先頭		*/
	UI_8		packet[D_BTC_MAX_RECV_BYTES];	/*	パケットバッファ			*/
} BTC_PACKET_INFO;

/* イベント情報 */
typedef UI_32	BTC_EVENT_FLG;
typedef struct {
	BTC_EVENT_FLG			event_flg;			/* イベントフラグ				*/
	E_BTC_STARTUP_FACTOR	startup_factor;		/* BTモジュール起動要因			*/
	E_BTC_SHUTDOWN_FACTOR	shutdown_factor;	/* BTモジュール停止要因			*/
} BTC_EVENT_INFO;

/* 状態関数 */
typedef E_BTC_STATUS (*PFN_BTC_STATE_PROC)( const BTC_EVENT_INFO* event_info );

/********************************************************/
/*	定数テーブル										*/
/********************************************************/
/*	外部状態へのマップテーブル	*/
BTC_LOCAL const E_BT_CONTROL_STAT S_state_map[] = {
	BT_CONTROL_STAT_UNKNOWN			,
	BT_CONTROL_STAT_SHUTDOWN		,	/*	ACC OFF								*/
	BT_CONTROL_STAT_STARTUP			,	/*	ACC ONによるBTモジュール起動待ち	*/
	BT_CONTROL_STAT_STARTUP			,	/*	ACC OFFによるBTモジュール停止待ち	*/
	BT_CONTROL_STAT_SHUTDOWN		,	/*	BTモジュール停止中		*/
	BT_CONTROL_STAT_STARTUP			,	/*	BTモジュール起動待ち	*/
	BT_CONTROL_STAT_STARTUP			,	/*	BTモジュール起動中		*/
	BT_CONTROL_STAT_STARTUP			,	/*	BTモジュール停止待ち	*/
	BT_CONTROL_STAT_COMM_ILDE		,	/*	BT通信中: データ未送信	*/
	BT_CONTROL_STAT_COMM_SENDING	,	/*	BT通信中: データ送信中	*/
	BT_CONTROL_STAT_PAIRING			,	/*	ペアリング移行中		*/
	BT_CONTROL_STAT_PAIRING			,	/*	ペアリング				*/
	BT_CONTROL_STAT_ERROR			,	/*	異常発生				*/
};
/********************************************************/
/*	内部変数定義										*/
/********************************************************/
/* バッファ */
BTC_LOCAL	BTC_RINGBUFF	S_btc_recv_ring;			/*	ミドルから受信したデータを保持するリングバッファ	*/
BTC_LOCAL	UI_8			S_btc_recv_data_buff[D_BTC_RECV_BUFF_BYTES];
														/*	受信データバッファ本体	*/
BTC_LOCAL	UI_8			S_btc_send_data_buff[D_BTC_SEND_BUFF_BYTES];
														/*	送信データ作成用バッファ	*/
/* パケット解析 */
BTC_LOCAL	BTC_PACKET_INFO	S_btc_packet_info;

/* 状態制御 */
BTC_LOCAL	E_BTC_STATUS	S_btc_status;					/*	BT制御状態			*/
BTC_LOCAL	E_BTC_STARTUP_FACTOR	S_btc_startup_factor;	/* BTモジュール起動要因 */

/* 送信信号 */
BTC_LOCAL	SI_32			S_btc_encode_key;			/*	暗号化キー		*/
BTC_LOCAL	UI_8			S_btc_send_signal_id_num;	/*	送信信号IDの数	*/
BTC_LOCAL	UI_16			S_btc_send_signal_id_array[D_BTC_MAX_SIGNAL_COUNT];
														/*	送信信号ID配列(送信開始要求で指定されたID列を保持)	*/

/* 時刻情報 */
BTC_LOCAL	BTC_TIME_INFO	S_btc_time_info;			/*	各種時刻情報					*/

BTC_LOCAL	UI_8			S_btc_req_Paring_Inspection;		/*	検査モードペアリング要求	*/
BTC_LOCAL	UI_8			S_btc_req_Communication_Inspection;	/*	検査モード通信要求			*/

/********************************************************/
/*	外部関数											*/
/********************************************************/

/********************************************************/
/*	内部関数宣言										*/
/********************************************************/
/* リングバッファ制御	*/
BTC_LOCAL E_RC	btc_ring_Init( BTC_RINGBUFF* ringbuff, UI_16 bytes, UI_8* buff );
BTC_LOCAL E_RC	btc_ring_Empty( BTC_RINGBUFF* ringbuff );
BTC_LOCAL UI_16 btc_ring_GetDataBytes( const BTC_RINGBUFF* ringbuff );
BTC_LOCAL UI_16	btc_ring_GetFreeBytes( const BTC_RINGBUFF* ringbuff );
BTC_LOCAL E_RC	btc_ring_Ref( const BTC_RINGBUFF* ringbuff, UI_16 ref_bytes, UI_8* buff );
BTC_LOCAL E_RC	btc_ring_Read( BTC_RINGBUFF* ringbuff, UI_16 read_bytes, UI_8* buff );
BTC_LOCAL E_RC	btc_ring_Write( BTC_RINGBUFF* ringbuff, UI_16 write_bytes, const UI_8* data );

/* イベント操作関数	*/
BTC_LOCAL void btc_state_MakeEvent( BTC_EVENT_INFO* event_info );

/* 状態関数	*/
BTC_LOCAL E_BTC_STATUS btc_state_ProcNone( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcAccOff( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitStartupAccOn( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitShutdownAccOff( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcShutdown( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitStartup( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcStartup( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitShutdown( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcPeerConnectedIdle( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcPeerConnectedSending( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitStartupPairing( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcPairing( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcError( const BTC_EVENT_INFO* event_info );

/* 状態遷移時関数 */
BTC_LOCAL void btc_state_ProcOnEnter( E_BTC_STATUS status );
BTC_LOCAL void btc_state_ProcOnLeave( E_BTC_STATUS status );

/* 状態処理内手続き関数 */
BTC_LOCAL E_RC btc_proc_BtMdlStartup( E_BTC_STARTUP_FACTOR startup_factor );
BTC_LOCAL E_RC btc_proc_BtMdlShutdown( E_BTC_SHUTDOWN_FACTOR shutdown_factor );
BTC_LOCAL E_RC btc_proc_SendData( UI_8 data_type );
BTC_LOCAL E_RC btc_proc_UartDataSend(void);
BTC_LOCAL E_RC btc_proc_ReturnSend(void);

/* 状態関数テーブル	*/
BTC_LOCAL const PFN_BTC_STATE_PROC S_state_proc[] = {
	btc_state_ProcNone,
	btc_state_ProcAccOff				,	/*	ACC OFF					*/
	btc_state_ProcWaitStartupAccOn		,	/*	ACC ON 起動待ち			*/
	btc_state_ProcWaitShutdownAccOff	,	/*	ACC OFF 停止待ち		*/
	btc_state_ProcShutdown				,	/*	BTモジュール停止中		*/
	btc_state_ProcWaitStartup			,	/*	BTモジュール起動待ち	*/
	btc_state_ProcStartup				,	/*	BTモジュール起動中		*/
	btc_state_ProcWaitShutdown			,	/*	BTモジュール停止待ち	*/
	btc_state_ProcPeerConnectedIdle		,	/*	BT通信中: データ未送信	*/
	btc_state_ProcPeerConnectedSending	,	/*	BT通信中: データ送信中	*/
	btc_state_ProcWaitStartupPairing	,	/*	ペアリング移行中		*/
	btc_state_ProcPairing				,	/*	ペアリング				*/
	btc_state_ProcError					,	/*	エラー					*/
};
/* 送信データ作成関数 */
BTC_LOCAL E_RC btc_data_MakeRepList( UI_8* buff, UI_16 buff_bytes, UI_16* data_bytes );
BTC_LOCAL E_RC btc_data_MakeRepStart( UI_8* buff, UI_16 buff_bytes, UI_16* data_bytes );
BTC_LOCAL E_RC btc_data_MakeRepStop( UI_8* buff, UI_16 buff_bytes, UI_16* data_bytes );
BTC_LOCAL UI_16 btc_data_GetBytesRepList( UI_8 fixed_signal_num, UI_8 vehicle_signal_num );
BTC_LOCAL UI_16 btc_data_GetBytesRepStart( UI_8 signal_num, UI_8 errcode_num );

/* 受信データ解析関数 */
BTC_LOCAL E_RC btc_data_AnalyzeRecvData( BTC_PACKET_INFO* packet_info );
BTC_LOCAL SI_32 btc_data_SkipToSTART( void );
BTC_LOCAL SI_32 btc_data_GetPacketInfo( BTC_PACKET_INFO* packet_info );
BTC_LOCAL SI_32 btc_data_CheckDataPartReqStart( const UI_8* data_part, UI_16 length );
BTC_LOCAL SI_32 btc_data_GetSendSignalInfo( const BTC_PACKET_INFO* packet_info );
BTC_LOCAL SI_32 btc_data_ClearRecvPacket( BTC_PACKET_INFO* packet_info );

/* 時間関連 */
BTC_LOCAL void	btc_time_RegTime( E_BTC_TIME_KIND kind );
BTC_LOCAL void	btc_time_ClearTime( E_BTC_TIME_KIND kind );
BTC_LOCAL SI_32	btc_time_CheckTimeout( E_BTC_TIME_KIND kind, UI_16 timeout_10msec );
BTC_LOCAL SI_32	btc_time_CheckTimerStartup( E_BTC_TIME_KIND kind );

/* コールバック関数 */
BTC_LOCAL void btc_cb_mdlReceiveData( const UI_8 *buf, UI_16 len );

/*==========================================================================*/
/* 単体テスト用定義															*/
/*==========================================================================*/

/*==========================================================================*/
/* 外部提供関数																*/
/*==========================================================================*/
/********************************************************/
/* 関数名称		: Bt_control_Init						*/
/* 機能の説明	: BT制御初期化							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Bt_control_Init(void)
{
	/* BT通信ミドル初期化 */
	Bt_comm_mdl_Init();
	/* 受信コールバック登録 */
	Bt_comm_mdl_RegisterCbReceiveData( btc_cb_mdlReceiveData );

	/* 変数初期化 */
	S_btc_status = BTC_STATUS_NONE;
	S_btc_send_signal_id_num = 0U;
	S_btc_encode_key = 0UL;

	/* バッファクリア */
	(void)memset( S_btc_recv_data_buff, 0, sizeof(S_btc_recv_data_buff) );
	(void)memset( S_btc_send_data_buff, 0, sizeof(S_btc_send_data_buff) );
	(void)memset( S_btc_send_signal_id_array, 0, sizeof(S_btc_send_signal_id_array) );

	/* 受信リングバッファ初期化 */
	(void)btc_ring_Init( &S_btc_recv_ring, sizeof(S_btc_recv_data_buff), S_btc_recv_data_buff );

	/* BTモジュール起動・停止要因クリア */
	S_btc_startup_factor = BTC_STARTUP_FACTOR_NONE;
	S_btc_req_Paring_Inspection = 0;
	S_btc_req_Communication_Inspection = 0;

}

/********************************************************/
/* 関数名称		: Bt_control_Main						*/
/* 機能の説明	: BT制御メイン							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Bt_control_Main(void)
{
	PFN_BTC_STATE_PROC	state_proc;
	BTC_EVENT_INFO		event_info;
	E_BTC_STATUS		next_status;

	/* ミドルのメイン処理を呼び出す */
	Bt_comm_mdl_Main();

	if( BTC_STATUS_COUNT > S_btc_status ) {
		/* 状態関数コール */
		state_proc = S_state_proc[S_btc_status];
		if( NULL != state_proc ) {
			/* イベント作成	*/
			btc_state_MakeEvent( &event_info );
			next_status = state_proc( &event_info );

			if( (BTC_STATUS_NONE != next_status) && (next_status != S_btc_status) ) {
				/* 状態遷移発生 */
				btc_state_ProcOnLeave( S_btc_status );
				OUTLOG(OutputLog( "[BTC] STATUS %d->%d [EV:%08x(%d:%d)][%08d]\n", S_btc_status, next_status, event_info.event_flg, event_info.startup_factor, event_info.shutdown_factor, Timer_ctrl_GetTime1ms() );)
				S_btc_status = next_status;
				btc_state_ProcOnEnter( S_btc_status );
			}
		}
	}
}

/********************************************************/
/* 関数名称		: Bt_control_GetStatus					*/
/* 機能の説明	: BT状態取得							*/
/* 引数			: void									*/
/* 戻り値		: BT状態 E_BT_CONTROL_STAT				*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
E_BT_CONTROL_STAT Bt_control_GetStatus(void)
{
	E_BT_CONTROL_STAT	stat = BT_CONTROL_STAT_UNKNOWN;

	if( BTC_STATUS_COUNT > S_btc_status ) {
		stat = S_state_map[S_btc_status];
	}
	return stat;
}

/********************************************************/
/* 関数名称		:	Bt_control_getUartData				*/
/* 機能の説明	:	UARTデータ取得						*/
/* 引数			:	( /O)buf	データバッファ			*/
/*				:	(I/ )len	取得データ長(byte)		*/
/* 戻り値		:	取得できたデータ長(byte)			*/
/* 作成者		:	(OFT)Toguchi						*/
/* 作成日		:	17/01/20							*/
/* 備考			:										*/
/********************************************************/
UI_16 Bt_control_getUartData(UI_8 *buf, UI_16 len)
{
	UI_16	data_bytes;

	if (buf == NULL) {
		return 0;
	}
	/*リングバッファのデータサイズ取得*/
	data_bytes = btc_ring_GetDataBytes( &S_btc_recv_ring );
	if (data_bytes > 0){
		/*データあり*/
		if(data_bytes > len){
			/*取得データ長より大きいサイズのデータがあるなら取得サイズを変更*/
			data_bytes = len;
		}
		/*リングバッファよりデータ取得*/
		(void)btc_ring_Read( &S_btc_recv_ring, data_bytes, buf );
	}
	return data_bytes;
}

/*------------------------------------------------------*/
/*       以下の関数は検査モード時のみ使用する           */
/*------------------------------------------------------*/
/*↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓*/
/********************************************************/
/* 関数名称		:	Bt_control_getMdlState				*/
/* 機能の説明	:	BT状態取得							*/
/* 引数			:	void								*/
/* 戻り値		: 	BT状態 E_BT_COMM_MDL_STATE			*/
/* 作成者		:	(OFT)Sakata							*/
/* 作成日		:	17/02/15							*/
/* 備考			:	検査モード時のみ使用				*/
/********************************************************/
E_BT_COMM_MDL_STATE Bt_control_getMdlState( void )
{
	return Bt_comm_mdl_GetState();
}

/********************************************************/
/* 関数名称		:	Bt_control_getIapAuthInfo			*/
/* 機能の説明	:	iAP接続情報取得						*/
/* 引数			:	(/O)iAP接続情報構造体				*/
/* 戻り値		: 	N_OK		正常					*/
/* 				: 	N_NG		異常					*/
/* 作成者		:	(OFT)Sakata							*/
/* 作成日		:	17/02/15							*/
/* 備考			:	検査モード時のみ使用				*/
/********************************************************/
SI_8 Bt_control_getIapAuthInfo( const S_IAP_CONNECTON_INFO** info )
{
	E_BT_COMM_MDL_RESULT result;
	SI_8 ret = N_OK;
	const S_IAP_CONNECTON_INFO* w_info = NULL;

	if (info != NULL) {
		result = Bt_comm_mdl_GetIapAuthInfo( &w_info );
		*info = w_info;
		if(result != BT_COMM_MDL_RESULT_OK)
		{
			ret = N_NG;
		}
	}
	else {
		ret = N_NG;
	}
	return ret;
}

/********************************************************/
/* 関数名称		:	Bt_control_reqPairing				*/
/* 機能の説明	:	ペアリング検査要求					*/
/* 引数			: 	void								*/
/* 戻り値		: 	N_OK		正常					*/
/* 				: 	N_NG		異常					*/
/* 作成者		:	(OFT)Sakata							*/
/* 作成日		:	17/02/15							*/
/* 備考			:	検査モード時のみ使用				*/
/********************************************************/
SI_8 Bt_control_reqPairingInspection(void)
{
	/*ペアリング要求あり設定*/
	S_btc_req_Paring_Inspection = 1;
	return N_OK;
}

/********************************************************/
/* 関数名称		:	Bt_control_reqCommunicationInspection*/
/* 機能の説明	:	通信検査要求						*/
/* 引数			: 	void								*/
/* 戻り値		: 	N_OK		正常					*/
/* 				: 	N_NG		異常					*/
/* 作成者		:	(OFT)Sakata							*/
/* 作成日		:	17/02/15							*/
/* 備考			:	検査モード時のみ使用				*/
/********************************************************/
SI_8 Bt_control_reqCommunicationInspection(void)
{
	/* 受信バッファを空にする */
	(void)btc_ring_Empty( &S_btc_recv_ring );
	/*通信要求あり設定*/
	S_btc_req_Communication_Inspection = 1;
	return N_OK;
}
/*↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑*/
/*------------------------------------------------------*/
/*       以上の関数は検査モード時のみ使用する           */
/*------------------------------------------------------*/

/*==========================================================================*/
/* リングバッファ制御関数													*/
/*==========================================================================*/
/****************************************************************************/
/* 関数名称		: btc_ring_Init												*/
/* 機能の説明	: リングバッファの初期化									*/
/* 引数			: ringbuff	リングバッファ									*/
/* 引数			: bytes	バッファサイズ										*/
/* 引数			: buff		割り当てるバッファ								*/
/* 戻り値		: 処理結果 E_RC												*/
/* 作成者		: (OFT)Kamakura												*/
/* 作成日		: 16/09/02													*/
/* 備考			: 															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_ring_Init( BTC_RINGBUFF* ringbuff, UI_16 bytes, UI_8* buff )
{
	E_RC	rc = E_RC_NG;

	if( (NULL == ringbuff) || (NULL == buff) || (0U == bytes) ) {
		rc = E_RC_NG_PARAM; 
	}
	else {
		(void)memset( ringbuff, 0, sizeof(BTC_RINGBUFF) );
		ringbuff->pu8_buff = buff;
		ringbuff->u16_buff_bytes = bytes;
		rc = E_RC_OK;
	}
	return rc;
}
/****************************************************************************/
/* 関数名称		: btc_ring_Empty											*/
/* 機能の説明	: リングバッファを空にする									*/
/* 引数			: ringbuff	リングバッファ									*/
/* 戻り値		: 処理結果 E_RC												*/
/* 作成者		: (OFT)Kamakura												*/
/* 作成日		: 16/09/02													*/
/* 備考			: 															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_ring_Empty( BTC_RINGBUFF* ringbuff )
{
	E_RC	rc = E_RC_NG;
	UI_16	bytes;

	if( NULL == ringbuff ) {
		rc = E_RC_NG_PARAM;
	}
	else {
		/* 入っているサイズ分読み進める */
		bytes = btc_ring_GetDataBytes( ringbuff );
		if( 0U < bytes ) {
			/* バッファ指定なしで読み進めのみ行う */
			rc = btc_ring_Read( ringbuff, bytes, NULL );
		}
		else {
			rc = E_RC_OK;
		}
	}
	return rc;
}
/****************************************************************************/
/* 関数名称		: btc_ring_GetDataBytes										*/
/* 機能の説明	: リングバッファデータサイズ取得							*/
/* 引数			: ringbuff	リングバッファ									*/
/* 戻り値		: 処理結果 参照(読込)可能データサイズ						*/
/* 作成者		: (OFT)Kamakura												*/
/* 作成日		: 16/09/02													*/
/* 備考			: 															*/
/****************************************************************************/
BTC_LOCAL UI_16 btc_ring_GetDataBytes( const BTC_RINGBUFF* ringbuff )
{
	UI_16	bytes = 0U;

	if( NULL != ringbuff ) {
		bytes = ringbuff->u16_w_total - ringbuff->u16_r_total;
	}
	return bytes;
}
/****************************************************************************/
/* 関数名称		: btc_ring_GetFreeBytes										*/
/* 機能の説明	: リングバッファ空きサイズ取得								*/
/* 引数			: ringbuff	リングバッファ									*/
/* 戻り値		: 処理結果 書込可能データサイズ								*/
/* 作成者		: (OFT)Kamakura												*/
/* 作成日		: 16/09/02													*/
/* 備考			: 															*/
/****************************************************************************/
BTC_LOCAL UI_16 btc_ring_GetFreeBytes( const BTC_RINGBUFF* ringbuff )
{
	UI_16	bytes = 0U;

	if( NULL != ringbuff ) {
		bytes = ringbuff->u16_buff_bytes - (ringbuff->u16_w_total - ringbuff->u16_r_total);
	}
	return bytes;
}
/****************************************************************************/
/* 関数名称		: btc_ring_Ref												*/
/* 機能の説明	: リングバッファからデータ参照								*/
/* 引数			: ringbuff		リングバッファ								*/
/* 引数			: ref_bytes	参照サイズ										*/
/* 引数			: buff			参照バッファ(NULL不可)						*/
/* 戻り値		: 処理結果 E_RC												*/
/* 作成者		: (OFT)Kamakura												*/
/* 作成日		: 16/09/02													*/
/* 備考			: 本関数で参照後も読み出し位置は不変						*/
/*				  要求サイズ分ないときもエラー								*/
/****************************************************************************/
BTC_LOCAL E_RC btc_ring_Ref( const BTC_RINGBUFF* ringbuff, UI_16 ref_bytes, UI_8* buff )
{
	E_RC	rc = E_RC_NG;
	UI_16	data_bytes;
	UI_16	tmp_bytes;

	if( (NULL == ringbuff) || (NULL == buff) || (0U == ref_bytes) || (0U == ringbuff->u16_buff_bytes) ) {
		rc = E_RC_NG_PARAM; 
	}
	else {
		/* データ残りバイト数取得 */
		data_bytes = btc_ring_GetDataBytes( ringbuff );
		if( ref_bytes > data_bytes ) {
			/* 指定サイズ分読み込めない */
			rc = E_RC_NG_PARAM;
		}
		else {
			if( (ringbuff->u16_r_index + ref_bytes) <= ringbuff->u16_buff_bytes ) {
				/* 終端までまとめ読み可 */
				(void)memcpy( buff, ringbuff->pu8_buff + ringbuff->u16_r_index, ref_bytes );
			}
			else {
				/* 折り返し */
				tmp_bytes = ringbuff->u16_buff_bytes - ringbuff->u16_r_index;
				(void)memcpy( buff, ringbuff->pu8_buff + ringbuff->u16_r_index, tmp_bytes );
				(void)memcpy( buff + tmp_bytes, ringbuff->pu8_buff, ref_bytes - tmp_bytes );
			}
			rc = E_RC_OK;
		}
	}
	return rc;
}
/****************************************************************************/
/* 関数名称		: btc_ring_Read												*/
/* 機能の説明	: リングバッファからデータ読みこみ							*/
/* 引数			: ringbuff		リングバッファ								*/
/* 引数			: read_bytes	読込サイズ									*/
/* 引数			: buff			読込バッファ(NULL可)						*/
/* 戻り値		: 処理結果 E_RC												*/
/* 作成者		: (OFT)Kamakura												*/
/* 作成日		: 16/09/02													*/
/* 備考			: 本関数で読込後読み出し位置を更新							*/
/*				  要求サイズ分ないときもエラー								*/
/****************************************************************************/
BTC_LOCAL E_RC btc_ring_Read( BTC_RINGBUFF* ringbuff, UI_16 read_bytes, UI_8* buff )
{
	E_RC	rc = E_RC_NG;
	UI_16	data_bytes;

	if( (NULL == ringbuff) || (0U == read_bytes) || (0U == ringbuff->u16_buff_bytes) ) {
		rc = E_RC_NG_PARAM; 
	}
	else {
		/* データ残りバイト数取得 */
		data_bytes = btc_ring_GetDataBytes( ringbuff );
		if( read_bytes > data_bytes ) {
			/* 指定サイズ分読み込めない */
			rc = E_RC_NG_PARAM;
		}
		else {
			if( NULL != buff ) {
				(void)btc_ring_Ref( ringbuff, read_bytes, buff );
			}
			/* リング情報更新 */
			ringbuff->u16_r_total += read_bytes;
			ringbuff->u16_r_index += read_bytes;
			if( ringbuff->u16_r_index >= ringbuff->u16_buff_bytes ) {
				ringbuff->u16_r_index -= ringbuff->u16_buff_bytes;
			}

			rc = E_RC_OK;	
		}
	}
	return rc;
}
/****************************************************************************/
/* 関数名称		: btc_ring_Write											*/
/* 機能の説明	: リングバッファへのデータ書き込み							*/
/* 引数			: ringbuff		リングバッファ								*/
/* 引数			: write_bytes	書込サイズ									*/
/* 引数			: data			書込データ									*/
/* 戻り値		: 処理結果 E_RC												*/
/* 作成者		: (OFT)Kamakura												*/
/* 作成日		: 16/09/02													*/
/* 備考			: 要求サイズ分書き込めないときエラー						*/
/****************************************************************************/
BTC_LOCAL E_RC btc_ring_Write( BTC_RINGBUFF* ringbuff, UI_16 write_bytes, const UI_8* data )
{
	E_RC	rc = E_RC_NG;
	UI_16	free_bytes;
	UI_16	tmp_bytes;

	if( (NULL == ringbuff) || (0U == write_bytes) || (0U == ringbuff->u16_buff_bytes) ) {
		rc = E_RC_NG_PARAM; 
	}
	else {
		/* 空きバイト数取得 */
		free_bytes = btc_ring_GetFreeBytes( ringbuff );
		if( free_bytes < write_bytes ) {
			/* 指定サイズ分書き込めない */
			rc = E_RC_NG_PARAM;
		}
		else {
			if( (ringbuff->u16_w_index + write_bytes) <= ringbuff->u16_buff_bytes ) {
				/* 終端までまとめ書き可 */
				(void)memcpy( ringbuff->pu8_buff + ringbuff->u16_w_index, data, write_bytes );
			}
			else {
				/* 折り返し */
				tmp_bytes = ringbuff->u16_buff_bytes - ringbuff->u16_w_index;
				(void)memcpy( ringbuff->pu8_buff + ringbuff->u16_w_index, data, tmp_bytes );
				(void)memcpy( ringbuff->pu8_buff, data + tmp_bytes, write_bytes - tmp_bytes );
			}
			/* リング情報更新 */
			ringbuff->u16_w_total += write_bytes;
			ringbuff->u16_w_index += write_bytes;
			if( ringbuff->u16_w_index >= ringbuff->u16_buff_bytes ) {
				ringbuff->u16_w_index -= ringbuff->u16_buff_bytes;
			}

			rc = E_RC_OK;
		}
	}
	return rc;
}
/*==========================================================================*/
/* イベント操作関数															*/
/*==========================================================================*/
/****************************************************************************/
/* 関数名称		:	btc_state_MakeEvent										*/
/* 機能の説明	:	イベント作成（イベントフラグ形式で返す）				*/
/* 引数			:	BTC_EVENT_INFO	イベント情報							*/
/* 戻り値		:	なし													*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:	イベントは同時に複数発生する。							*/
/****************************************************************************/
BTC_LOCAL void btc_state_MakeEvent( BTC_EVENT_INFO* event_info )
{
	UI_16						uartDataLen;
	UI_16						buffDataBytes;
	BTC_EVENT_FLG				event_flag = BTC_EVENT_NONE;
	E_SYS_CTRL_STATUS			sys_ctrl_status;
	E_SW_REC_BTN_STATUS			sw_rec_btn_status = SYS_REC_BTN_UNKNOWN;
	E_BT_COMM_MDL_STATE			bt_state;
	E_BT_COMM_MDL_PEER_STATUS	bt_peer_state;
	E_BT_COMM_MDL_PAIRED_STATUS	bt_paired_state;

	if( NULL == event_info ) {
		return;
	}
	(void)memset( event_info, 0, sizeof(BTC_EVENT_INFO) );

	/*======================================================================*/
	/* イベントの元になる各種状態を収集										*/
	/*======================================================================*/
	/* システム動作状態取得 */
	sys_ctrl_status = System_control_GetStatus();
	
	if(	(sys_ctrl_status != SYS_OPERATION_BT_COMM) && 
		(sys_ctrl_status != SYS_OPERATION_INSPECTION) )
		/* BTモジュール通信モード,検査モード以外に受信データ解析を行う*/
	{
		/* 受信データ解析して１個のパケット情報を得る */
		(void)btc_data_AnalyzeRecvData( &S_btc_packet_info );
	}

	/* RECボタン状態取得 */
	sw_rec_btn_status = Sw_control_GetRecButtonStatus();

	/* BTミドル状態 */
	bt_state = Bt_comm_mdl_GetState();

	/* BTミドルPeer接続状態 */
	bt_peer_state = Bt_comm_mdl_GetPeerState();

	/* ペアリング状態取得 */
	bt_paired_state = Bt_comm_mdl_GetPairedStatus();

	uartDataLen = Advance_control_getUartDataLen();

	/* リングバッファのデータ長を取得 */
	buffDataBytes = btc_ring_GetDataBytes( &S_btc_recv_ring );

	/*======================================================================*/
	/* E01	BTモジュール起動要求											*/
	/* E02	BTモジュール停止要求											*/
	/*======================================================================*/
	/*
		起動要求
			モードが以下のいずれかである(or)
			・ACC ON
			・通常:オートRECモード
			・通常:マニュアルRECモード
			・シミュレーション
		停止要求
			モードが以下のいずれかである(or)
			・通常:OFF
			・異常
			・EEPROM*
			・ファームウェア書換*
			・ACC OFF移行
	*/
	switch( sys_ctrl_status ) {
	case SYS_OPERATION_WAIT_ACC_ON:			/**< ACC ON待ち				*/
		/* 何もしない */
		break;
	case SYS_OPERATION_ACC_ON_UNKNOWN_STAT:	/**< ACC ON（状態判定）		*/
		/* ACC ON */
		event_flag |= BTC_EVENT_ACC_ON;
		break;
	case SYS_OPERATION_NORMAL_AUTO:			/**< 通常:オートREC			*/
	case SYS_OPERATION_NORMAL_MANUAL:		/**< 通常:マニュアルREC		*/
	case SYS_OPERATION_SIMULATION:			/**< シミュレーション		*/
	case SYS_OPERATION_BT_COMM:				/**< BTモジュール通信モード	*/
		/* その他要因による起動要求 */
		event_flag |= BTC_EVENT_REQ_STARTUP;
		event_info->startup_factor = BTC_STARTUP_FACTOR_OTHERS;
		break;
	case SYS_OPERATION_INSPECTION:			/**< 検査モード				*/
		if( BTC_STARTUP_FACTOR_PAIRING != S_btc_startup_factor ) {
			event_flag |= BTC_EVENT_REQ_STARTUP;
			event_info->startup_factor = BTC_STARTUP_FACTOR_OTHERS;
		}
		break;
	case SYS_OPERATION_NORMAL_OFF:			/**< 通常:OFFモード			*/
		/* OFFモードによる停止要求 */
		if( BTC_STARTUP_FACTOR_PAIRING != S_btc_startup_factor ) {
			event_flag |= BTC_EVENT_REQ_SHUTDOWN;
			event_info->shutdown_factor = BTC_SHUTDOWN_FACTOR_OTHERS;
		}
		break;
	case SYS_OPERATION_EEPROM_WRITE:		/**< EEPROM書換中			*/
	case SYS_OPERATION_EEPROM_WRITE_COMP:	/**< EEPROM書換完了			*/
	case SYS_OPERATION_FIRMWARE_WRITE:		/**< ファームウェア書換中	*/
	case SYS_OPERATION_FIRMWARE_WRITE_COMP:	/**< ファームウェア書換完了	*/
		/* その他要因による停止要求 */
		event_flag |= BTC_EVENT_REQ_SHUTDOWN;
		event_info->shutdown_factor = BTC_SHUTDOWN_FACTOR_OTHERS;
		break;
	case SYS_OPERATION_PRE_ACC_OFF:			/**< ACC OFF移行			*/
		/* ACC OFF */
		event_flag |= BTC_EVENT_ACC_OFF;
		break;
	case SYS_OPERATION_ACC_OFF:				/**< ACC OFF				*/
		/* 何もしない */
		break;
	case SYS_OPERATION_ERROR:				/**< 異常					*/
		/* 何もしない */
		break;
	default:
		/* 何もしない */
		break;
	}
	/*======================================================================*/
	/* E03	ペアリング開始要求												*/
	/*======================================================================*/
	/*
		OFFモード and RECスイッチONが3秒継続
	*/
	if( SYS_OPERATION_NORMAL_OFF == sys_ctrl_status ) {
		/* OFF モード */
		if( SYS_REC_BTN_ON_LONGPRESS == sw_rec_btn_status ) {
			/* RECスイッチON(長押し) */
			event_flag |= BTC_EVENT_REQ_PAIRING;
		}
	}
	else if( SYS_OPERATION_INSPECTION == sys_ctrl_status ) {
		/* 検査モード */
		if(S_btc_req_Paring_Inspection != 0) {
			/*ペアリング要求あり*/
			event_flag |= BTC_EVENT_REQ_PAIRING;
		}
	}
	else {
		/* nop */
	}
	/*======================================================================*/
	/* E04	ペアリング完了													*/
	/*======================================================================*/
	if( BTC_STATUS_PAIRING == S_btc_status ) {
		if( (BT_COMM_MDL_STATE_COMM_MODE == bt_state) && (BT_COMM_MDL_PAIRED_STATUS_EXIST == bt_paired_state) ) {
			if( SYS_OPERATION_INSPECTION != sys_ctrl_status ) {
				/* 検査モード以外 */
				event_flag |= BTC_EVENT_PAIRING;
			}
			else {
				/* 検査モード中はスタートアップ */
				event_flag |= BTC_EVENT_REQ_STARTUP;
			}
		}
	}
	/*======================================================================*/
	/* E05	ペアリングタイムアウト											*/
	/*======================================================================*/
	if( SYS_OPERATION_INSPECTION != sys_ctrl_status ) {
		/* 検査モード以外 */
		if( BTC_STATUS_PAIRING == S_btc_status ) {
			if( 1L == btc_time_CheckTimeout( BTC_TIME_KIND_PAIRING_START, D_BTC_TIMEOUT_MSEC_PAIRING ) ) {
				event_flag |= BTC_EVENT_PAIRING_TIMEOUT;
			}
		}
	}
	/*======================================================================*/
	/* E06	BTモジュール起動完了											*/
	/*======================================================================*/
	if ((BTC_STATUS_WAIT_STARTUP == S_btc_status) || (BTC_STATUS_WAIT_STARTUP_ACC_ON == S_btc_status) || (BTC_STATUS_WAIT_STARTUP_PAIRING == S_btc_status)) {
		if( BT_COMM_MDL_STATE_COMM_MODE == bt_state ) {
			event_flag |= BTC_EVENT_STARTUP;
		}
	}
	/*======================================================================*/
	/* E07	BTモジュール停止完了											*/
	/*======================================================================*/
	if ((BTC_STATUS_WAIT_SHUTDOWN == S_btc_status) || (BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF == S_btc_status)) {
		if( BT_COMM_MDL_STATE_IDLE == bt_state ) {
			event_flag |= BTC_EVENT_SHUTDOWN;
		}
	}
	/*======================================================================*/
	/* E08	Peer接続														*/
	/*======================================================================*/
	if( BTC_STATUS_STARTUP == S_btc_status ) {
		if( BT_COMM_MDL_PEER_STATUS_CONNECTED == bt_peer_state ) {
			event_flag |= BTC_EVENT_PEER_CONNECTED;
		}
	}
	/*======================================================================*/
	/* E09	Peer切断														*/
	/*======================================================================*/
	if( (BTC_STATUS_PEEE_CONNECTED_IDLE == S_btc_status) || (BTC_STATUS_PEEE_CONNECTED_SENDING == S_btc_status) ) {
		if( BT_COMM_MDL_PEER_STATUS_DISCONNECTED == bt_peer_state ) {
			event_flag |= BTC_EVENT_PEER_DISCONNECTED;
		}
	}
	/*======================================================================*/
	/* E10	車両信号一覧取得(リスト)要求									*/
	/*======================================================================*/
	if( (BTC_STATUS_PEEE_CONNECTED_IDLE == S_btc_status) || (BTC_STATUS_PEEE_CONNECTED_SENDING == S_btc_status) ) {
		if( D_BTC_FMT_TYPE_REQ_LIST == S_btc_packet_info.type ) {
			event_flag |= BTC_EVENT_REQ_SIGNAL_LIST;
		}
	}
	/*======================================================================*/
	/* E11	車両情報(取得開始)要求											*/
	/*======================================================================*/
	if( (BTC_STATUS_PEEE_CONNECTED_IDLE == S_btc_status) || (BTC_STATUS_PEEE_CONNECTED_SENDING == S_btc_status) ) {
		if( D_BTC_FMT_TYPE_REQ_START == S_btc_packet_info.type ) {
			event_flag |= BTC_EVENT_REQ_START_SEND;
		}
	}
	/*======================================================================*/
	/* E12	車両情報(取得停止)要求											*/
	/*======================================================================*/
	if( (BTC_STATUS_PEEE_CONNECTED_IDLE == S_btc_status) || (BTC_STATUS_PEEE_CONNECTED_SENDING == S_btc_status) ) {
		if( D_BTC_FMT_TYPE_REQ_STOP == S_btc_packet_info.type ) {
			event_flag |= BTC_EVENT_REQ_STOP_SEND;
		}
	}
	/*======================================================================*/
	/* E13	車両信号送信トリガ												*/
	/*======================================================================*/
	if( 1L == btc_time_CheckTimeout( BTC_TIME_KIND_SEND_REP_START, D_BTC_CYCLE_MSEC_SEND_REP_START ) ) {
		event_flag |= BTC_EVENT_SEND_TRIGGER;
	}
	/*======================================================================*/
	/* E14	BTモジュール起動タイムアウト									*/
	/*======================================================================*/
	if( 1L == btc_time_CheckTimeout( BTC_TIME_KIND_STARTUP, D_BTC_TIMEOUT_MSEC_STARTUP ) ) {
		event_flag |= BTC_EVENT_STARTUP_TIMEOUT;
	}
	/*======================================================================*/
	/* E15	パケット検出タイムアウト										*/
	/*======================================================================*/
	if( 1L == btc_time_CheckTimeout( BTC_TIME_KIND_PACKET_FOUND, D_BTC_TIMEOUT_MSEC_RECV_PACKET ) ) {
		event_flag |= BTC_EVENT_RECV_PACKET_TIMEOUT;
	}
	/*======================================================================*/
	/* E16	BTモジュール異常検知											*/
	/*======================================================================*/
	if( BTC_STATUS_ERROR != S_btc_status ) {
		if( BT_COMM_MDL_STATE_ERROR == bt_state ) {
			event_flag |= BTC_EVENT_ERROR;
		}
	}
	/*======================================================================*/
	/* E17	UART送信データ要求												*/
	/*======================================================================*/
	if(sys_ctrl_status == SYS_OPERATION_BT_COMM)
	{
		/**< BTモジュール通信モード	*/
		if(uartDataLen > 0){
			/*UARTデータあり*/
			event_flag |= BTC_EVENT_REQ_UART_SEND;
		}
	}
	/*======================================================================*/
	/* E18	検査モードデータ送信											*/
	/*======================================================================*/
	if(sys_ctrl_status == SYS_OPERATION_INSPECTION) {
		/**< 検査モード	*/
		if (S_btc_req_Communication_Inspection != 0) {
			/*通信要求あり*/
			if(buffDataBytes > 0){
				/*受信データあり*/
				event_flag |= BTC_EVENT_REQ_RETURN_SEND;
			}
		}
	}
	event_info->event_flg = event_flag;
}
/*==========================================================================*/
/* 状態関数																	*/
/*==========================================================================*/
/****************************************************************************/
/* 関数名称		:	btc_state_ProcNone										*/
/* 機能の説明	:	状態関数 BTC_STATUS_NONE								*/
/* 引数			:	event_info	イベント情報								*/
/* 戻り値		:	次の遷移先	BTC_STATUS_XXX								*/
/*					遷移なし	BTC_STATUS_NONE								*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcNone( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		/* 初期状態へ遷移する */
		next_status = BTC_STATUS_ACC_OFF;
	}
	return next_status;
}
/****************************************************************************/
/* 関数名称		:	btc_state_ProcAccOff									*/
/* 機能の説明	:	状態関数 BTC_STATUS_ACC_OFF		ACC OFF					*/
/* 引数			:	event_info	イベント情報								*/
/* 戻り値		:	次の遷移先	BTC_STATUS_XXX								*/
/*					遷移なし	BTC_STATUS_NONE								*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcAccOff( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_ON) ) {
			/* BTモジュール起動 */
			(void)btc_proc_BtMdlStartup( BTC_STARTUP_FACTOR_ACC_ON );

			next_status = BTC_STATUS_WAIT_STARTUP_ACC_ON;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BTモジュール異常検知 */

			/* システム制御にイベント送信【状態異常】*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* 異常発生状態へ遷移して ACC OFF → ON 待機  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UARTデータ送信 */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* その他処理なし */
		}
	}
	return next_status;
}
/****************************************************************************/
/* 関数名称		:	btc_state_ProcWaitStartupAccOn							*/
/* 機能の説明	:	状態関数 BTC_STATUS_WAIT_STARTUP_ACC_ON		ACC ONによるBTモジュール起動待ち	*/
/* 引数			:	event_info	イベント情報								*/
/* 戻り値		:	次の遷移先	BTC_STATUS_XXX								*/
/*					遷移なし	BTC_STATUS_NONE								*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitStartupAccOn( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC ON */

			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_STARTUP) ) {
			/* BTモジュール起動 */

			/* システム制御にイベント送信【状態正常】 */
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_OK_BT );
			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_OTHERS );
			next_status = BTC_STATUS_WAIT_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_STARTUP_TIMEOUT) ) {
			/* BTモジュール起動タイムアウト */

			/* システム制御にイベント送信【状態異常】*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* 異常発生状態へ遷移して ACC OFF → ON 待機  */
			next_status = BTC_STATUS_ERROR;

			OUTLOG(OutputLog( "[BTC] BTC_EVENT_STARTUP_TIMEOUT\n" );)
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BTモジュール異常検知 */

			/* システム制御にイベント送信【状態異常】*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* 異常発生状態へ遷移して ACC OFF → ON 待機  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UARTデータ送信 */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* その他処理なし */
		}
	}
	return next_status;
}
/****************************************************************************/
/* 関数名称		:	btc_state_ProcWaitShutdownAccOff						*/
/* 機能の説明	:	状態関数 BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF	ACC OFFによるBTモジュール停止待ち	*/
/* 引数			:	event_info	イベント情報								*/
/* 戻り値		:	次の遷移先	BTC_STATUS_XXX								*/
/*					遷移なし	BTC_STATUS_NONE								*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitShutdownAccOff( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_SHUTDOWN) ) {
			/* BTモジュール停止完了 */

			/* システム制御にイベント送信【BT終了】*/
			(void)System_control_NotifyEvent( SYS_EVENT_END_PROC_COMP_BT, SYS_EVENT_PARAM_NO );

			next_status = BTC_STATUS_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BTモジュール異常検知 */

			/* システム制御にイベント送信【状態異常】*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* 異常発生状態へ遷移して ACC OFF → ON 待機  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UARTデータ送信 */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* その他処理なし */
		}
	}
	return next_status;
}
/****************************************************************************/
/* 関数名称		:	btc_state_ProcShutdown									*/
/* 機能の説明	:	状態関数 BTC_STATUS_SHUTDOWN	BTモジュール停止中		*/
/* 引数			:	event_info	イベント情報								*/
/* 戻り値		:	次の遷移先	BTC_STATUS_XXX								*/
/*					遷移なし	BTC_STATUS_NONE								*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcShutdown( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* システム制御にイベント送信【BT終了】*/
			(void)System_control_NotifyEvent( SYS_EVENT_END_PROC_COMP_BT, SYS_EVENT_PARAM_NO );
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_STARTUP) ) {
			/* BTモジュール起動 */
			(void)btc_proc_BtMdlStartup( event_info->startup_factor );

			next_status = BTC_STATUS_WAIT_STARTUP;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_PAIRING) ) {
			/* ペアリング開始要求でもBTモジュール起動 */
			(void)btc_proc_BtMdlStartup( BTC_STARTUP_FACTOR_PAIRING );

			next_status = BTC_STATUS_WAIT_STARTUP_PAIRING;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BTモジュール異常検知 */

			/* システム制御にイベント送信【状態異常】*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* 異常発生状態へ遷移して ACC OFF → ON 待機  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UARTデータ送信 */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* その他処理なし */
		}
	}
	return next_status;
}
/****************************************************************************/
/* 関数名称		:	btc_state_ProcWaitStartup								*/
/* 機能の説明	:	状態関数 BTC_STATUS_WAIT_STARTUP	BTモジュール起動待ち*/
/* 引数			:	event_info	イベント情報								*/
/* 戻り値		:	次の遷移先	BTC_STATUS_XXX								*/
/*					遷移なし	BTC_STATUS_NONE								*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitStartup( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SHUTDOWN) ) {
			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( event_info->shutdown_factor );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_ON) ) {
			/* ACC-ON */

			/* 起動要因をACC ONに変更（フェールセーフ）*/
			S_btc_startup_factor = BTC_STARTUP_FACTOR_ACC_ON;
			next_status = BTC_STATUS_WAIT_STARTUP_ACC_ON;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_STARTUP) ) {
			/* BTモジュール起動 */
			next_status = BTC_STATUS_STARTUP;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_STARTUP_TIMEOUT) ) {
			/* BTモジュール起動タイムアウト */

			/* システム制御にイベント送信【状態異常】*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* 異常発生状態へ遷移して ACC OFF → ON 待機  */
			next_status = BTC_STATUS_ERROR;

			OUTLOG(OutputLog( "[BTC] BTC_EVENT_STARTUP_TIMEOUT\n" );)
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BTモジュール異常検知 */

			/* システム制御にイベント送信【状態異常】*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* 異常発生状態へ遷移して ACC OFF → ON 待機  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UARTデータ送信 */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* その他処理なし */
		}
	}
	return next_status;
}
/****************************************************************************/
/* 関数名称		:	btc_state_ProcStartup									*/
/* 機能の説明	:	状態関数 BTC_STATUS_STARTUP	BTモジュール起動中			*/
/* 引数			:	event_info	イベント情報								*/
/* 戻り値		:	次の遷移先	BTC_STATUS_XXX								*/
/*					遷移なし	BTC_STATUS_NONE								*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcStartup( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SHUTDOWN) ) {
			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( event_info->shutdown_factor );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_PAIRING) ) {
			/* ペアリング */
			(void)Bt_comm_mdl_EnterPairingMode();
			S_btc_startup_factor = BTC_STARTUP_FACTOR_PAIRING;

			next_status = BTC_STATUS_PAIRING;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_PEER_CONNECTED) ) {
			/* Peer接続 */
			next_status = BTC_STATUS_PEEE_CONNECTED_IDLE;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BTモジュール異常検知 */

			/* システム制御にイベント送信【状態異常】*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* 異常発生状態へ遷移して ACC OFF → ON 待機  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UARTデータ送信 */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* その他処理なし */
		}
	}
	return next_status;
}
/****************************************************************************/
/* 関数名称		:	btc_state_ProcWaitShutdown								*/
/* 機能の説明	:	状態関数 BTC_STATUS_WAIT_SHUTDOWN	BTモジュール停止待ち*/
/* 引数			:	event_info	イベント情報								*/
/* 戻り値		:	次の遷移先	BTC_STATUS_XXX								*/
/*					遷移なし	BTC_STATUS_NONE								*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitShutdown( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_SHUTDOWN) ) {
			/* BTモジュール停止完了 */
			next_status = BTC_STATUS_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BTモジュール異常検知 */

			/* システム制御にイベント送信【状態異常】*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* 異常発生状態へ遷移して ACC OFF → ON 待機  */
			next_status = BTC_STATUS_ERROR;
		}
		else {
			/* その他処理なし */
		}
	}
	return next_status;
}
/****************************************************************************/
/* 関数名称		:	btc_state_ProcPeerConnectedIdle							*/
/* 機能の説明	:	状態関数 BTC_STATUS_PEEE_CONNECTED_IDLE	BT通信中: データ未送信	*/
/* 引数			:	event_info	イベント情報								*/
/* 戻り値		:	次の遷移先	BTC_STATUS_XXX								*/
/*					遷移なし	BTC_STATUS_NONE								*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcPeerConnectedIdle( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;
	E_RC			rc;

	if( NULL != event_info ) {
		if( 0L == S_btc_encode_key ) {
			/* 暗号化キー未取得なら暗号用乱数生成 */
			S_btc_encode_key = Signal_control_GetRandomValue();

			/* BT用暗号化キー設定 */
			(void)Signal_control_SetEncodeKey( DEVICE_TYPE_BT, S_btc_encode_key );
		}
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SHUTDOWN) ) {
			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( event_info->shutdown_factor );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_PEER_DISCONNECTED) ) {
			/* Peer切断 */
			next_status = BTC_STATUS_STARTUP;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SIGNAL_LIST) ) {
			/* 車両信号一覧取得(リスト)要求 */

			/* 受信パケットをクリア */
			(void)btc_data_ClearRecvPacket( &S_btc_packet_info );

			/* 車両信号一覧取得(リスト)応答を送信 */
			(void)btc_proc_SendData( D_BTC_FMT_TYPE_REP_LIST );
			/* 状態遷移なし */
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_START_SEND) ) {
			/* 車両情報(取得開始)要求 */

			/* 送信中信号情報を取り出す */
			(void)btc_data_GetSendSignalInfo( &S_btc_packet_info );

			/* 受信パケットをクリア */
			(void)btc_data_ClearRecvPacket( &S_btc_packet_info );

			/* 車両情報(取得開始)応答を送信 */
			rc = btc_proc_SendData( D_BTC_FMT_TYPE_REP_START );
			if( E_RC_OK == rc ) {
				next_status = BTC_STATUS_PEEE_CONNECTED_SENDING;
			}
			else {
				/* 送信異常は無視 */
			}
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_STOP_SEND) ) {
			/* 車両情報(取得停止)要求 */

			/* 受信パケットをクリア */
			(void)btc_data_ClearRecvPacket( &S_btc_packet_info );

			/* 車両情報(取得停止)応答を送信（フェール） */
			(void)btc_proc_SendData( D_BTC_FMT_TYPE_REP_STOP );	
				/* 本来応答不可だがフェールで送信 */
			/* 状態遷移なし */
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_RECV_PACKET_TIMEOUT) ) {
			/* パケット受信タイムアウト */

			/* 受信バッファを空にする */
			(void)btc_ring_Empty( &S_btc_recv_ring );

			/* パケット受信時刻をクリア */
			btc_time_ClearTime( BTC_TIME_KIND_PACKET_FOUND );
			/* 状態遷移なし */
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BTモジュール異常検知 */

			/* システム制御にイベント送信【状態異常】*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* 異常発生状態へ遷移して ACC OFF → ON 待機  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UARTデータ送信 */
			(void)btc_proc_UartDataSend();
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_RETURN_SEND) ) {
			/* 受信データをそのまま送信 */
			(void)btc_proc_ReturnSend();
		}
		else {
			/* その他処理なし */
		}
	}
	return next_status;
}
/****************************************************************************/
/* 関数名称		:	btc_state_ProcPeerConnectedSending						*/
/* 機能の説明	:	状態関数 BTC_STATUS_PEEE_CONNECTED_SENDING	BT通信中: データ送信中	*/
/* 引数			:	event_info	イベント情報								*/
/* 戻り値		:	次の遷移先	BTC_STATUS_XXX								*/
/*					遷移なし	BTC_STATUS_NONE								*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcPeerConnectedSending( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SHUTDOWN) ) {
			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( event_info->shutdown_factor );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_PEER_DISCONNECTED) ) {
			/* Peer切断 */
			next_status = BTC_STATUS_STARTUP;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SIGNAL_LIST) ) {
			/* 車両信号一覧取得(リスト)要求 */

			/* 受信パケットをクリア */
			(void)btc_data_ClearRecvPacket( &S_btc_packet_info );

			/* 状態遷移なし */
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_START_SEND) ) {
			/* 車両情報(取得開始)要求 */

			/* 受信パケットをクリア */
			(void)btc_data_ClearRecvPacket( &S_btc_packet_info );

			/* 状態遷移なし */
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_STOP_SEND) ) {
			/* 受信パケットをクリア */
			(void)btc_data_ClearRecvPacket( &S_btc_packet_info );

			/* 車両情報(取得停止)応答を送信 */
			(void)btc_proc_SendData( D_BTC_FMT_TYPE_REP_STOP );	

			next_status = BTC_STATUS_PEEE_CONNECTED_IDLE;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_SEND_TRIGGER) ) {
			/* 車両信号送信トリガ */

			/* 車両情報(取得開始)応答を送信 */
			(void)btc_proc_SendData( D_BTC_FMT_TYPE_REP_START );
			/* 状態遷移なし */
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_RECV_PACKET_TIMEOUT) ) {
			/* パケット受信タイムアウト */

			/* 受信バッファを空にする */
			(void)btc_ring_Empty( &S_btc_recv_ring );

			/* パケット受信時刻をクリア */
			btc_time_ClearTime( BTC_TIME_KIND_PACKET_FOUND );
			/* 状態遷移なし */
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BTモジュール異常検知 */

			/* システム制御にイベント送信【状態異常】*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* 異常発生状態へ遷移して ACC OFF → ON 待機  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UARTデータ送信 */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* その他処理なし */
		}
	}
	return next_status;
}
/****************************************************************************/
/* 関数名称		:	btc_state_ProcWaitStartupPairing						*/
/* 機能の説明	:	状態関数 BTC_STATUS_WAIT_STARTUP_PAIRING	ペアリング移行中	*/
/* 引数			:	event_info	イベント情報								*/
/* 戻り値		:	次の遷移先	BTC_STATUS_XXX								*/
/*					遷移なし	BTC_STATUS_NONE								*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitStartupPairing( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SHUTDOWN) ) {
			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( event_info->shutdown_factor );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_STARTUP) ) {
			/* ペアリング移行中にマニュアルモード等への切替は起動待ちの状態へ遷移させる */
			S_btc_startup_factor = BTC_STARTUP_FACTOR_OTHERS;

			next_status = BTC_STATUS_WAIT_STARTUP;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_STARTUP) ) {
			/* BTモジュール起動 */

			/* ペアリング開始 */
			(void)Bt_comm_mdl_EnterPairingMode();
			S_btc_startup_factor = BTC_STARTUP_FACTOR_PAIRING;

			next_status = BTC_STATUS_PAIRING;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_STARTUP_TIMEOUT) ) {
			/* BTモジュール起動タイムアウト */

			/* システム制御にイベント送信【状態異常】*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* 異常発生状態へ遷移して ACC OFF → ON 待機  */
			next_status = BTC_STATUS_ERROR;

			OUTLOG(OutputLog( "[BTC] BTC_EVENT_STARTUP_TIMEOUT\n" );)
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BTモジュール異常検知 */

			/* システム制御にイベント送信【状態異常】*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* 異常発生状態へ遷移して ACC OFF → ON 待機  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UARTデータ送信 */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* その他処理なし */
		}
	}
	return next_status;
}
/****************************************************************************/
/* 関数名称		:	btc_state_ProcPairing									*/
/* 機能の説明	:	状態関数 BTC_STATUS_PAIRING	ペアリング					*/
/* 引数			:	event_info	イベント情報								*/
/* 戻り値		:	次の遷移先	BTC_STATUS_XXX								*/
/*					遷移なし	BTC_STATUS_NONE								*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcPairing( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_STARTUP) ) {
			/* BTモジュール通信モード開始 */
			(void)Bt_comm_mdl_EnterCommMode();
			S_btc_startup_factor = BTC_STARTUP_FACTOR_OTHERS;

			next_status = BTC_STATUS_WAIT_STARTUP;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SHUTDOWN) ) {
			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( event_info->shutdown_factor );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_PAIRING) ) {
			/* ペアリング完了 */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_OTHERS );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;

			OUTLOG(OutputLog( "[BTC] BTC_EVENT_PAIRING\n" );)
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_PAIRING_TIMEOUT) ) {
			/* ペアリングタイムアウト */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_OTHERS );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;

			OUTLOG(OutputLog( "[BTC] BTC_EVENT_PAIRING_TIMEOUT\n" );)
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BTモジュール異常検知 */

			/* システム制御にイベント送信【状態異常】*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* 異常発生状態へ遷移して ACC OFF → ON 待機  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UARTデータ送信 */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* その他処理なし */
		}
	}
	return next_status;
}
/****************************************************************************/
/* 関数名称		:	btc_state_ProcError										*/
/* 機能の説明	:	状態関数 BTC_STATUS_ERROR	異常発生 					*/
/* 引数			:	event_info	イベント情報								*/
/* 戻り値		:	次の遷移先	BTC_STATUS_XXX								*/
/*					遷移なし	BTC_STATUS_NONE								*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/16												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcError( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		/* 現在処理なし */
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* BTモジュール停止 */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
	}
	return next_status;
}
/*==========================================================================*/
/* 状態遷移時関数															*/
/*==========================================================================*/
/****************************************************************************/
/* 関数名称		:	btc_state_ProcOnEnter									*/
/* 機能の説明	:	状態に入るときの処理									*/
/* 引数			:	これから入る状態										*/
/* 戻り値		:	なし													*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL void btc_state_ProcOnEnter( E_BTC_STATUS status )
{
	switch( status ) {
	case BTC_STATUS_ACC_OFF:				/*	ACC OFF								*/
		break;
	case BTC_STATUS_WAIT_STARTUP_ACC_ON:	/*	ACC ONによるBTモジュール起動待ち	*/
		/* BTモジュール起動時刻記録（BTモジュール起動タイムアウトのため）*/
		btc_time_RegTime( BTC_TIME_KIND_STARTUP );
		break;
	case BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF:	/*	ACC OFFによるBTモジュール停止待ち	*/
		S_btc_req_Paring_Inspection = 0;
		S_btc_req_Communication_Inspection = 0;
		break;
	case BTC_STATUS_SHUTDOWN:				/*	BTモジュール停止中		*/
		break;
	case BTC_STATUS_WAIT_STARTUP:			/*	BTモジュール起動待ち	*/
		/* BTモジュール起動時刻記録（BTモジュール起動タイムアウトのため）*/
		btc_time_RegTime( BTC_TIME_KIND_STARTUP );
		break;
	case BTC_STATUS_STARTUP:				/*	BTモジュール起動中		*/
		/* 受信バッファを空にする */
		(void)btc_ring_Empty( &S_btc_recv_ring );
		break;
	case BTC_STATUS_WAIT_SHUTDOWN:			/*	BTモジュール停止待ち	*/
		break;
	case BTC_STATUS_PEEE_CONNECTED_IDLE:	/*	BT通信中: データ未送信	*/
		break;
	case BTC_STATUS_PEEE_CONNECTED_SENDING:	/*	BT通信中: データ送信中	*/
		break;
	case BTC_STATUS_WAIT_STARTUP_PAIRING:	/*	ペアリング移行中		*/
		/* BTモジュール起動時刻記録（BTモジュール起動タイムアウトのため）*/
		btc_time_RegTime( BTC_TIME_KIND_STARTUP );
		break;
	case BTC_STATUS_PAIRING:				/*	ペアリング				*/
		/* ペアリング開始時刻記録（60秒タイムアウトのため） */
		btc_time_RegTime( BTC_TIME_KIND_PAIRING_START );
		break;
	case BTC_STATUS_ERROR:					/*	異常発生				*/
		S_btc_req_Paring_Inspection = 0;
		S_btc_req_Communication_Inspection = 0;
		break;
	default:
		break;
	}
}
/****************************************************************************/
/* 関数名称		:	btc_state_ProcOnLeave									*/
/* 機能の説明	:	状態から出るときの処理									*/
/* 引数			:	これから出る状態										*/
/* 戻り値		:	なし													*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL void btc_state_ProcOnLeave( E_BTC_STATUS status )
{
	switch( status ) {
	case BTC_STATUS_ACC_OFF:				/*	ACC OFF								*/
		break;
	case BTC_STATUS_WAIT_STARTUP_ACC_ON:	/*	ACC ONによるBTモジュール起動待ち	*/
		/* BTモジュール起動時刻クリア（BTモジュール起動タイムアウトのため） */
		btc_time_ClearTime( BTC_TIME_KIND_STARTUP );
		break;
	case BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF:	/*	ACC OFFによるBTモジュール停止待ち	*/
		break;
	case BTC_STATUS_SHUTDOWN:				/*	BTモジュール停止中		*/
		break;
	case BTC_STATUS_WAIT_STARTUP:			/*	BTモジュール起動待ち	*/
		/* BTモジュール起動時刻クリア（BTモジュール起動タイムアウトのため） */
		btc_time_ClearTime( BTC_TIME_KIND_STARTUP );
		break;
	case BTC_STATUS_STARTUP:				/*	BTモジュール起動中		*/
		break;
	case BTC_STATUS_WAIT_SHUTDOWN:			/*	BTモジュール停止待ち	*/
		break;
	case BTC_STATUS_PEEE_CONNECTED_IDLE:	/*	BT通信中: データ未送信	*/
		/*通信要求なし設定*/
		S_btc_req_Communication_Inspection = 0;
		break;
	case BTC_STATUS_PEEE_CONNECTED_SENDING:	/*	BT通信中: データ送信中	*/
		/* 送信中情報をクリア */
		S_btc_send_signal_id_num = 0U;
		(void)memset( S_btc_send_signal_id_array, 0, sizeof(S_btc_send_signal_id_array) );
		break;
	case BTC_STATUS_WAIT_STARTUP_PAIRING:	/*	ペアリング移行中		*/
		/* BTモジュール起動時刻クリア（BTモジュール起動タイムアウトのため） */
		btc_time_ClearTime( BTC_TIME_KIND_STARTUP );
		break;
	case BTC_STATUS_PAIRING:				/*	ペアリング				*/
		/* ペアリング開始時刻クリア（60秒タイムアウトのため） */
		btc_time_ClearTime( BTC_TIME_KIND_PAIRING_START );
		/*ペアリング要求なし設定*/
		S_btc_req_Paring_Inspection = 0;
		break;
	case BTC_STATUS_ERROR:					/*	異常発生				*/
		break;
	default:
		break;
	}
}
/*==========================================================================*/
/* 状態処理内手続き関数														*/
/*==========================================================================*/
/****************************************************************************/
/* 関数名称		:	btc_proc_BtMdlStartup									*/
/* 機能の説明	:	BTモジュール起動手続き									*/
/* 引数			:	startup_factor	起動要因								*/
/* 戻り値		:	E_RC	処理結果										*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_proc_BtMdlStartup( E_BTC_STARTUP_FACTOR startup_factor )
{
	E_BT_COMM_MDL_RESULT	ret;
	E_RC					rc = E_RC_NG;

	/* BTモジュール起動 */
	ret = Bt_comm_mdl_Startup();
	if( BT_COMM_MDL_RESULT_OK == ret ) {
		/* 起動要因を記録 */
		S_btc_startup_factor = startup_factor;

		rc = E_RC_OK;
	}
	return rc;
}
/****************************************************************************/
/* 関数名称		:	btc_proc_BtMdlShutdown									*/
/* 機能の説明	:	BTモジュール停止手続き									*/
/* 引数			:	shutdown_factor	停止要因								*/
/* 戻り値		:	E_RC	処理結果										*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_proc_BtMdlShutdown( E_BTC_SHUTDOWN_FACTOR shutdown_factor )
{
	E_BT_COMM_MDL_RESULT	ret;
	E_RC					rc = E_RC_NG;

	/* BTモジュール停止 */
	ret = Bt_comm_mdl_Shutdown();
	if( BT_COMM_MDL_RESULT_OK == ret ) {

		rc = E_RC_OK;
	}
	return rc;
}
/****************************************************************************/
/* 関数名称		:	btc_proc_SendData										*/
/* 機能の説明	:	データ送信手続き										*/
/* 引数			:	data_type	データ種別									*/
/* 戻り値		:	E_RC	処理結果										*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_proc_SendData( UI_8 data_type )
{
	E_RC					rc = E_RC_NG;
	UI_16					data_bytes = 0U;
	E_BT_COMM_MDL_RESULT	bt_result;

	switch( data_type ) {
	case D_BTC_FMT_TYPE_REP_LIST:	/* 車両信号一覧取得(リスト)応答 */
		rc = btc_data_MakeRepList( S_btc_send_data_buff, sizeof(S_btc_send_data_buff), &data_bytes );
		break;
	case D_BTC_FMT_TYPE_REP_START:	/* 車両情報(取得開始)応答 */
		rc = btc_data_MakeRepStart( S_btc_send_data_buff, sizeof(S_btc_send_data_buff), &data_bytes );
		break;
	case D_BTC_FMT_TYPE_REP_STOP:	/* 車両情報(取得停止)応答 */
		rc = btc_data_MakeRepStop( S_btc_send_data_buff, sizeof(S_btc_send_data_buff), &data_bytes );
		break;
	default:
		rc = E_RC_NG_PARAM;
		break;
	}
	if( E_RC_OK == rc ) {
		/* 作成したパケットを送信 */
		bt_result = Bt_comm_mdl_SendData( S_btc_send_data_buff, data_bytes );
		if( BT_COMM_MDL_RESULT_OK == bt_result ) {
			/* 送信成功 */
			if( D_BTC_FMT_TYPE_REP_START == data_type ) {
				/* 周期的な送信トリガ発生用に送信時刻を記録 */
				btc_time_RegTime( BTC_TIME_KIND_SEND_REP_START );
			}
		}
		else {
			/* 送信失敗 */
			rc = E_RC_NG;
		}
	}
	return rc;
}
/****************************************************************************/
/* 関数名称		:	btc_proc_UartDataSend									*/
/* 機能の説明	:	UARTデータ送信											*/
/* 引数			:	void													*/
/* 戻り値		:	E_RC	処理結果										*/
/* 作成者		:	(OFT)Sakata												*/
/* 作成日		:	17/02/07												*/
/* 備考			:	UART（AdvanceControlUni）のデータをBTで送信する			*/
/* 				:	BT未接続状態の場合は、UARTデータは破棄される			*/
/****************************************************************************/
BTC_LOCAL E_RC btc_proc_UartDataSend(void)
{
	E_RC					rc = E_RC_NG;
	UI_16					data_bytes = 0U;
	E_BT_COMM_MDL_RESULT	bt_result;
	
	/* AdvanceControlからUARTデータ取得 */
	data_bytes = Advance_control_getUartData(S_btc_send_data_buff, sizeof(S_btc_send_data_buff));
	if( data_bytes > 0 ){
		/* 取得したデータを送信 */
		bt_result = Bt_comm_mdl_SendData( S_btc_send_data_buff, data_bytes );
		if( BT_COMM_MDL_RESULT_OK == bt_result ) {
			/* 送信成功 */
			rc = E_RC_OK;
		}
	}
	return rc;
}
/****************************************************************************/
/* 関数名称		:	btc_proc_ReturnSend										*/
/* 機能の説明	:	検査モードデータ送信									*/
/* 引数			:	void													*/
/* 戻り値		:	E_RC	処理結果										*/
/* 作成者		:	(OFT)Sakata												*/
/* 作成日		:	17/02/15												*/
/* 備考			:	受信データをそのまま送信する							*/
/****************************************************************************/
BTC_LOCAL E_RC btc_proc_ReturnSend(void)
{
	E_RC					rc = E_RC_NG;
	UI_16					data_bytes = 0U;
	E_BT_COMM_MDL_RESULT	bt_result;

	/* 残りデータバイト数取得 */
	data_bytes = btc_ring_GetDataBytes( &S_btc_recv_ring );
	if( 0U < data_bytes ) {
		if( data_bytes > sizeof(S_btc_send_data_buff) ) {
			data_bytes = sizeof(S_btc_send_data_buff);
		}
		/* バッファ参照 */
		(void)btc_ring_Read( &S_btc_recv_ring, data_bytes, S_btc_send_data_buff );
		/* 取得したデータを送信 */
		bt_result = Bt_comm_mdl_SendData( S_btc_send_data_buff, data_bytes );
		if( BT_COMM_MDL_RESULT_OK == bt_result ) {
			/* 送信成功 */
			rc = E_RC_OK;
		}
	}
	return rc;
}

/*==========================================================================*/
/* 送信データ作成関数														*/
/*==========================================================================*/
/****************************************************************************/
/* 関数名称		:	btc_data_MakeRepList									*/
/* 機能の説明	:	送信データ作成:車両信号一覧取得(リスト)応答				*/
/* 引数			:	buff		[ /o]データ作成先バッファ					*/
/* 引数			:	buff_bytes	[i/ ]バッファサイズ							*/
/* 引数			:	data_bytes	[ /o]作成したデータのサイズ					*/
/* 戻り値		:	E_RC	処理結果										*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_data_MakeRepList( UI_8* buff, UI_16 buff_bytes, UI_16* data_bytes )
{
	E_RC			rc = E_RC_NG;
	UI_8*			p = NULL;
	UI_16			crc = 0U;
	UI_16			data_length = 0U;
	UI_16			length = 0U;
	UI_16			packet_bytes = 0U;
	UI_16			firmware_version = 0U;
	const UI_16*	fixed_signal_id_list = NULL;	/* 取得可能な車両信号数（固定情報）		*/
	const UI_16*	vehicle_signal_id_list = NULL;	/* 取得可能な車両信号数（車両情報）		*/
	UI_8			fixed_signal_num = 0U;			/* 取得可能な車両信号数（固定情報）の数	*/
	UI_8			vehicle_signal_num = 0U;		/* 取得可能な車両信号数（車両情報）の数	*/
	UI_8			i;
	BTC_LOCAL		SI_32			encode_key;		/*	暗号化キー		*/

	if( (NULL == buff) || (NULL == data_bytes) ) {
		rc = E_RC_NG_PARAM;
	}
	else {
		/* 公開信号ID一覧取得 */
		/* 信号種別:固定値	*/
		(void)Signal_control_GetSignalList( DEVICE_TYPE_BT, ID_TYPE_FIXED_ID, &fixed_signal_num, &fixed_signal_id_list );
		/* 信号種別:変数値	*/
		(void)Signal_control_GetSignalList( DEVICE_TYPE_BT, ID_TYPE_VARIABLE_ID, &vehicle_signal_num, &vehicle_signal_id_list );
		if ((fixed_signal_id_list == NULL) || (vehicle_signal_id_list == NULL)) {
			return E_RC_NG;
		}

		/* パケット長 */
		packet_bytes = btc_data_GetBytesRepList(fixed_signal_num, vehicle_signal_num);
		data_length = packet_bytes - D_BTC_FMT_BYTES_FIX;

		if( packet_bytes > buff_bytes ) {
			rc = E_RC_NG_PARAM;
		}
		else {
			/* バッファクリア */
			(void)memset( buff, 0, buff_bytes );

			/* lengthを計算 */
			length = data_length + 1U;
		
			p = buff;

			/*==================================================================*/
			/* ヘッダ作成														*/
			/*==================================================================*/
			*p = D_BTC_FMT_HEADER_START;
			p++;
			Common_Set16( p, length );
			p += 2;
			*p = D_BTC_FMT_TYPE_REP_LIST;
			p++;

			/*==================================================================*/
			/* データ部作成														*/
			/*==================================================================*/
			/* 固体識別番号(16バイト) */
			/* 00固定 */
			p += 16;

			/* 車種コード(2バイト) */
			/* 00固定 */
			p += 2;

			/* フォーマットバージョン(3バイト) */
			Common_Set24( p, BTC_FORMAT_VERSION );
			p += 3;

			/* 信号リストバージョン(3バイト) */
			Common_Set24( p, Signal_control_GetSignalVersion() );
			p += 3;

			/* ファームウェアバージョン(3バイト) */
			(void)Eeprom_control_GetData( EEPROM_DATA_FIRMWARE_VER, &firmware_version );
			/* 16bitのバージョンを前詰めで記録 */
			Common_Set24( p, ((UI_32)firmware_version) << 8 );
			p += 3;

			/* コメント(10バイト) */
			/* 00固定 */
			p += 10;

			/* 取得可能な車両信号数（固定情報） */
			*p = fixed_signal_num;
			p++;
			for( i = 0; i < fixed_signal_num; i++ ) {
				/* 取得可能な車両信号ID（固定情報） */
				Common_Set16( p, fixed_signal_id_list[i] );
				p += 2;
			}
			/* 取得可能な車両信号数（車両情報） */
			*p = vehicle_signal_num;
			p++;
			for( i = 0; i < vehicle_signal_num; i++ ) {
				/* 取得可能な車両信号ID（車両情報） */
				Common_Set16( p, vehicle_signal_id_list[i] );
				p += 2;
			}
			/* 乱数(4バイト) */
			/* BT用暗号化キー取得 */
			(void)Signal_control_GetEncodeKey( DEVICE_TYPE_BT, &encode_key );
			Common_Set32( p, (UI_32)encode_key );
			p += 4;

			/*==================================================================*/
			/* フッタ作成														*/
			/*==================================================================*/
			crc = Common_CalcCRC( buff, 1U, length + 2U );
			Common_Set16( p, crc );
			p += 2;
			*p = D_BTC_FMT_FOOTER_END;

			/* データ長を格納 */
			*data_bytes = packet_bytes;

			rc = E_RC_OK;
		}
	}
	return rc;
}
/****************************************************************************/
/* 関数名称		:	btc_data_MakeRepStart									*/
/* 機能の説明	:	送信データ作成:車両情報(取得開始)応答					*/
/* 引数			:	buff		[ /o]データ作成先バッファ					*/
/* 引数			:	buff_bytes	[i/ ]バッファサイズ							*/
/* 引数			:	data_bytes	[ /o]作成したデータのサイズ					*/
/* 戻り値		:	E_RC	処理結果										*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_data_MakeRepStart( UI_8* buff, UI_16 buff_bytes, UI_16* data_bytes )
{
	E_RC	rc = E_RC_NG;
	UI_8*	p = NULL;
	SI_32	signal_value = 0UL;
	UI_16	crc = 0U;
	UI_16	data_length = 0U;
	UI_16	length = 0U;
	UI_16	packet_bytes = 0U;
	UI_8	errcode_num = 0U;
	UI_8	i = 0U;

	if( (NULL == buff) || (NULL == data_bytes) ) {
		rc = E_RC_NG_PARAM;
	}
	else {
		/* パケット長 */
		packet_bytes = btc_data_GetBytesRepStart(S_btc_send_signal_id_num, errcode_num);
		data_length = packet_bytes - D_BTC_FMT_BYTES_FIX;

		if( packet_bytes > buff_bytes ) {
			rc = E_RC_NG_PARAM;
		}
		else {
			/* バッファクリア */
			(void)memset( buff, 0, buff_bytes );

			/* lengthを計算 */
			length = data_length + 1U;

			p = buff;

			/*==================================================================*/
			/* ヘッダ作成														*/
			/*==================================================================*/
			*p = D_BTC_FMT_HEADER_START;
			p++;
			Common_Set16( p, length );
			p += 2;
			*p = D_BTC_FMT_TYPE_REP_START;
			p++;

			/*==================================================================*/
			/* データ部作成														*/
			/*==================================================================*/
			/* タイムスタンプ */
			Common_Set32( p, Timer_ctrl_GetTime1ms() );
			p += 4;

			/* エラーコード数 */
			*p = errcode_num;
			p++;
			/* 予約エラーコードは現在格納しない */

			/* 応答する車両信号数(要求された数) */
			*p = S_btc_send_signal_id_num;
			p++;

			/* 応答する車両信号IDと信号値 */
			for( i = 0; i < S_btc_send_signal_id_num; i++ ) {
				/* 車両信号ID */
				Common_Set16( p, S_btc_send_signal_id_array[i] );
				p += 2;

				/* 信号値 */
				(void)Signal_control_GetSignalInfo( DEVICE_TYPE_BT, S_btc_send_signal_id_array[i], &signal_value );
				Common_Set32( p, (UI_32)signal_value );
				p += 4;
			}
			/*==================================================================*/
			/* フッタ作成														*/
			/*==================================================================*/
			crc = Common_CalcCRC( buff, 1U, length + 2U );
			Common_Set16( p, crc );
			p += 2;
			*p = D_BTC_FMT_FOOTER_END;

			/* データ長を格納 */
			*data_bytes = packet_bytes;

			rc = E_RC_OK;
		}
	}
	return rc;
}
/****************************************************************************/
/* 関数名称		:	btc_data_MakeRepStop									*/
/* 機能の説明	:	送信データ作成:車両情報(取得停止)応答					*/
/* 引数			:	buff		[ /o]データ作成先バッファ					*/
/* 引数			:	buff_bytes	[i/ ]バッファサイズ							*/
/* 引数			:	data_bytes	[ /o]作成したデータのサイズ					*/
/* 戻り値		:	E_RC	処理結果										*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_data_MakeRepStop( UI_8* buff, UI_16 buff_bytes, UI_16* data_bytes )
{
	E_RC	rc = E_RC_NG;
	UI_8*	p;
	UI_16	crc = 0U;
	UI_16	data_length = 0U;
	UI_16	length = 0U;
	UI_16	packet_bytes = 0U;

	if( (NULL == buff) || (NULL == data_bytes) ) {
		rc = E_RC_NG_PARAM;
	}
	else {
		/* パケット長 */
		packet_bytes = D_BTC_FMT_BYTES_REP_STOP;
		data_length = packet_bytes - D_BTC_FMT_BYTES_FIX;

		if( packet_bytes > buff_bytes ) {
			rc = E_RC_NG_PARAM;
		}
		else {
			/* バッファクリア */
			(void)memset( buff, 0, buff_bytes );

			/* lengthを計算 */
			length = data_length + 1U;
		
			p = buff;

			/*==================================================================*/
			/* ヘッダ作成														*/
			/*==================================================================*/
			*p = D_BTC_FMT_HEADER_START;
			p++;
			Common_Set16( p, length );
			p += 2;
			*p = D_BTC_FMT_TYPE_REP_STOP;
			p++;

			/*==================================================================*/
			/* フッタ作成														*/
			/*==================================================================*/
			crc = Common_CalcCRC( buff, 1U, length + 2U );
			Common_Set16( p, crc );
			p += 2;
			*p = D_BTC_FMT_FOOTER_END;

			/* データ長を格納 */
			*data_bytes = packet_bytes;

			rc = E_RC_OK;
		}
	}
	return rc;
}
/****************************************************************************/
/* 関数名称		:	btc_data_MakeRepStop									*/
/* 機能の説明	:	「車両信号一覧取得(リスト)応答」データサイズ取得		*/
/* 引数			:	fixed_signal_num	[i/ ]固定情報数						*/
/* 引数			:	vehicle_signal_num	[i/ ]車両情報数						*/
/* 戻り値		:	データサイズ											*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/08/29												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL UI_16 btc_data_GetBytesRepList( UI_8 fixed_signal_num, UI_8 vehicle_signal_num )
{
	return ((UI_16)D_BTC_FMT_HEADER_BYTES + (43 + (2 * (UI_16)fixed_signal_num) + (2 * (UI_16)vehicle_signal_num)) + (UI_16)D_BTC_FMT_FOOTER_BYTES);
}
/****************************************************************************/
/* 関数名称		:	btc_data_GetBytesRepStart								*/
/* 機能の説明	:	「車両情報(取得開始)応答」データサイズ取得				*/
/* 引数			:	signal_num		[i/ ]信号数								*/
/* 引数			:	errcode_num		[i/ ]エラー数							*/
/* 戻り値		:	データサイズ											*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/08/29												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL UI_16 btc_data_GetBytesRepStart( UI_8 signal_num, UI_8 errcode_num )
{
	return ((UI_16)D_BTC_FMT_HEADER_BYTES + (6 + (6 * (UI_16)signal_num) + (6 * (UI_16)errcode_num)) + (UI_16)D_BTC_FMT_FOOTER_BYTES);
}

/*==========================================================================*/
/* 受信データ解析関数														*/
/*==========================================================================*/
/****************************************************************************/
/* 関数名称		:	btc_data_AnalyzeRecvData								*/
/* 機能の説明	:	受信データ解析											*/
/* 引数			:	packet_info	解析で得たパケットの情報					*/
/* 戻り値		:	E_RC	処理結果										*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:	受信データない場合 D_BTC_FMT_TYPE_INVALID を出力する	*/
/****************************************************************************/
BTC_LOCAL E_RC btc_data_AnalyzeRecvData( BTC_PACKET_INFO* packet_info )
{
	E_RC	rc = E_RC_NG;
	SI_32	ret;

	if( NULL == packet_info ) {
		rc = E_RC_NG_PARAM;
	}
	else {
		/* パケット情報を無効値で初期化 */
		(void)memset( packet_info, 0, sizeof(BTC_PACKET_INFO) );
		packet_info->type = D_BTC_FMT_TYPE_INVALID;

		/* 受信バッファを START マークまで読み飛ばす */
		ret = btc_data_SkipToSTART();
		/* 1: START あり */
		/* 0: START なし */
		while( 1L == ret ) {
			/* タイマー起動中か判定 */
			if(btc_time_CheckTimerStartup( BTC_TIME_KIND_PACKET_FOUND ) == 0){
				/* 起動中でなければパケット検出時刻を登録 */
				btc_time_RegTime( BTC_TIME_KIND_PACKET_FOUND );
			}
			
			ret = btc_data_GetPacketInfo( packet_info );
			if( 1L == ret ) {
				/* 1:  パケットあり */
				/* パケット検出時刻をクリア */
				btc_time_ClearTime( BTC_TIME_KIND_PACKET_FOUND );
				break;
			}
			else if( 0L == ret ) {
				/* 0:  パケットではない（異常パケットも含む） */
				/* STARTマークを飛ばして次の解析 */
				(void)btc_ring_Read( &S_btc_recv_ring, 1U, NULL ); 
			}
			else {
				/* -1: データ不足で判定不能 */
				break;
			}
			/* 受信バッファを START マークまで読み飛ばす */
			ret = btc_data_SkipToSTART();
		}
		rc = E_RC_OK;
	}
	return rc;
}
/****************************************************************************/
/* 関数名称		:	btc_data_SkipToSTART									*/
/* 機能の説明	:	受信バッファを START マークまで読み飛ばす				*/
/* 引数			:	なし													*/
/* 戻り値		:	1: START あり											*/
/* 戻り値		:	0: START なし											*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL SI_32 btc_data_SkipToSTART( void )
{
	SI_32	ret = 0L;
	UI_16	data_bytes = 0L;
	UI_16	skip_bytes = 0L;
	UI_8	buf[16];	/* 適当なブロックサイズで処理 */
	UI_16	buf_bytes =	sizeof(buf);

	/* 残りデータバイト数取得 */
	data_bytes = btc_ring_GetDataBytes( &S_btc_recv_ring );
	while( 0U < data_bytes ) {
		if( data_bytes > buf_bytes ) {
			data_bytes = buf_bytes;
		}
		/* バッファ参照 */
		(void)btc_ring_Ref( &S_btc_recv_ring, data_bytes, buf );

		/* START マークを探す */
		skip_bytes = 0U;
		while( skip_bytes < data_bytes ) {
			if( D_BTC_FMT_HEADER_START == buf[skip_bytes] ) {
				/* 発見 */
				ret = 1L;
				break;
			}
			skip_bytes++;
		}
		if( 0U < skip_bytes ) {
			/* みつからなかったデータをバッファから捨てる */
			(void)btc_ring_Read( &S_btc_recv_ring, skip_bytes, NULL );
		}
		if( 1L == ret ) {
			/* ループを抜けて終了 */
			break;
		}
		/* 残りデータバイト数取得 */
		data_bytes = btc_ring_GetDataBytes( &S_btc_recv_ring );
	}
	return ret;
}
/****************************************************************************/
/* 関数名称		:	btc_data_GetPacketInfo									*/
/* 機能の説明	:	受信バッファを解析してパケット情報を得る				*/
/* 引数			:	packet_info	解析で得たパケットの情報					*/
/* 戻り値		:	1:  パケットあり										*/
/* 戻り値		:	0:  パケットではない（異常パケットも含む）				*/
/* 戻り値		:	-1: データ不足で判定不能								*/
/* 戻り値		:	-2:	引数不正											*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/02												*/
/* 備考			:	パケットがない場合 D_BTC_FMT_TYPE_INVALID を出力する	*/
/****************************************************************************/
BTC_LOCAL SI_32 btc_data_GetPacketInfo( BTC_PACKET_INFO* packet_info )
{
	SI_32	ret = 0L;
	UI_16	data_bytes = 0U;
	UI_16	buf_bytes = sizeof(packet_info->packet);
	UI_8*	p = NULL;
	UI_8*	header_part = NULL;
	UI_8*	data_part = NULL;
	UI_8*	footer_part = NULL;
	UI_16	packet_bytes = 0;
	UI_16	length = 0U;
	UI_16	crc = 0U;
	UI_16	crc_calc = 0U;
	UI_8	type = 0U;

	if( NULL == packet_info ) {
		ret = -2L;
	}
	else {
		/* パケット情報を無効値で初期化 */
		(void)memset( packet_info, 0, sizeof(BTC_PACKET_INFO) );
		packet_info->type = D_BTC_FMT_TYPE_INVALID;

		/* リングバッファのデータを参照 */
		data_bytes = btc_ring_GetDataBytes( &S_btc_recv_ring );
		if( data_bytes > buf_bytes ) {
			/* パケットバッファの最大数分までデータ参照する */
			data_bytes = buf_bytes;
		}
		if( D_BTC_FMT_BYTES_FIX > data_bytes ) {
			/* 固定部分のサイズにも満たない */
			ret = -1L;
			goto END_ANALYZE;
		}
		(void)btc_ring_Ref( &S_btc_recv_ring, data_bytes, packet_info->packet );

		/* パケット解析 */
		p = packet_info->packet;

		/*==================================================================*/
		/* ヘッダの解析														*/
		/*==================================================================*/
		header_part = p;
		/* START */
		if( D_BTC_FMT_HEADER_START != *p ) {
			goto END_ANALYZE;
		}
		p++;
		/* LENGTH */
		length = Common_Get16( p );
		p += 2;
		/* TYPE */
		type = *p;
		p++;

		/* パケット全体の長さを計算 */
		packet_bytes = D_BTC_FMT_BYTES_FIX + length - 1U;
		if( packet_bytes > data_bytes ) {
			/* パケット全体のサイズに満たない */
			ret = -1L;
			goto END_ANALYZE;
		}
		/*==================================================================*/
		/* データの解析														*/
		/*==================================================================*/
		switch( type ) {
		case D_BTC_FMT_TYPE_REQ_LIST:
			/* データ部なし */
			break;
		case D_BTC_FMT_TYPE_REQ_START:
			/* 車両情報(取得開始)要求パケットのデータ部をチェック */
			if( 1L != btc_data_CheckDataPartReqStart( p, length - 1U ) ) {
				/* 不正なデータ部 */
				goto END_ANALYZE;
			}
			break;
		case D_BTC_FMT_TYPE_REQ_STOP:
			/* データ部なし */
			break;
		default:
			/* 不正なデータ種別 */
			goto END_ANALYZE;
		}
		data_part = p;
		p += (length - 1U);

		/*==================================================================*/
		/* フッタの解析														*/
		/*==================================================================*/
		footer_part = p;
		/* CRC */
		crc = Common_Get16( p );
		p += 2;
		/* END */
		if( D_BTC_FMT_FOOTER_END != *p ) {
			goto END_ANALYZE;
		}

		/* CRC計算 */
		crc_calc = Common_CalcCRC( header_part, 1U, length + 2 );
		if( crc_calc != crc ) {
			/* CRC 不一致 */
			goto END_ANALYZE;
		}
		/*==================================================================*/
		/* 解析結果を格納													*/
		/*==================================================================*/
		packet_info->packet_bytes = packet_bytes;
		packet_info->length = length;
		packet_info->type = type;
		packet_info->header_part = header_part;
		packet_info->data_part = data_part;
		packet_info->footer_part = footer_part;

		/* パケットがみつかった */
		ret = 1L;
	}
END_ANALYZE:
	return ret;
}
/****************************************************************************/
/* 関数名称		:	btc_data_GetSendSignalInfo								*/
/* 機能の説明	:	車両情報(取得開始)要求パケットのデータ部をチェック		*/
/* 引数			:	data_part	i: データ部先頭アドレス						*/
/* 引数			:	length		i: データ部全体のサイズ(バイト数)			*/
/* 戻り値		:	1:  正常終了（データ部は正常）							*/
/* 戻り値		:	0:  正常終了（データ部不正）							*/
/* 戻り値		:	-1: 引数不正											*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/17												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL SI_32 btc_data_CheckDataPartReqStart( const UI_8* data_part, UI_16 length )
{
	SI_32			ret = -1L;
	UI_8			num = 0U;

	if( NULL != data_part ) {
		/* 引数正常 */
		ret = 0L;

		if( 0U < length ) {
			/* データ部を先頭からチェック */
			num = *data_part;
			if( (0U < num) && (num <= D_BTC_MAX_SIGNAL_COUNT) &&
				(length == (UI_16)(1U + (num * sizeof(UI_16)))) ) {
				/* データ数が１個以上制限数以下であり長さとデータ数が合致している */
				ret = 1L;
			}
		}
	}
	return ret;
}
/****************************************************************************/
/* 関数名称		:	btc_data_GetSendSignalInfo								*/
/* 機能の説明	:	送信信号情報取得										*/
/* 引数			:	packet_info	解析で得たパケットの情報					*/
/* 戻り値		:	0:  正常終了											*/
/* 戻り値		:	-1: 引数不正（パケット種別が不正な場合含む）			*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/12												*/
/* 備考			:	packet_info	には D_BTC_FMT_TYPE_REQ_START パケットを渡す*/
/****************************************************************************/
BTC_LOCAL SI_32 btc_data_GetSendSignalInfo( const BTC_PACKET_INFO* packet_info )
{
	SI_32	ret = -1L;

	if( NULL != packet_info ) {
		if( D_BTC_FMT_TYPE_REQ_START == packet_info->type ) {
			/* 送信信号IDの数 */
			S_btc_send_signal_id_num = *(packet_info->data_part);
			if( D_BTC_MAX_SIGNAL_COUNT < S_btc_send_signal_id_num ) {
				/* 最大数を超えているときは丸める */
				S_btc_send_signal_id_num = D_BTC_MAX_SIGNAL_COUNT;
			}
			/* 送信信号ID配列を抽出 */
			(void)memcpy( S_btc_send_signal_id_array, packet_info->data_part + 1, (size_t)(S_btc_send_signal_id_num * sizeof(UI_16)) );
			
			ret = 0L;
		}
	}
	return ret;
}
/****************************************************************************/
/* 関数名称		:	btc_data_ClearRecvPacket								*/
/* 機能の説明	:	受信パケットクリア										*/
/* 引数			:	packet_info	解析で得たパケットの情報					*/
/* 戻り値		:	0:  正常終了											*/
/* 戻り値		:	-1: 引数不正											*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/12												*/
/* 備考			:	packet_info が示すパケットをバッファから取り除く		*/
/****************************************************************************/
BTC_LOCAL SI_32 btc_data_ClearRecvPacket( BTC_PACKET_INFO* packet_info )
{
	SI_32	ret = -1L;

	if( NULL != packet_info ) {
		/* 受信パケットをリングバッファから取り除く */
		(void)btc_ring_Read( &S_btc_recv_ring, packet_info->packet_bytes, NULL );

		(void)memset( packet_info, 0, sizeof(BTC_PACKET_INFO) );

		ret = 0L;
	}
	return ret;
}
/*==========================================================================*/
/* 時間関連																	*/
/*==========================================================================*/
/****************************************************************************/
/* 関数名称		:	btc_time_RegTime										*/
/* 機能の説明	:	時刻を登録												*/
/* 引数			:	kind	時刻種別										*/
/* 戻り値		:	なし													*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/03												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL void	btc_time_RegTime( E_BTC_TIME_KIND kind )
{
	UI_32	flag = (1UL << kind);
	
	if( kind < BTC_TIME_KIND_COUNT ) {
		S_btc_time_info.time[kind] = Timer_ctrl_GetTime10ms();
		S_btc_time_info.flags |= flag;
	}
}
/****************************************************************************/
/* 関数名称		:	btc_time_ClearTime										*/
/* 機能の説明	:	時刻をクリア											*/
/* 引数			:	kind	時刻種別										*/
/* 戻り値		:	なし													*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/03												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL void	btc_time_ClearTime( E_BTC_TIME_KIND kind )
{
	UI_32	flag = (1UL << kind);

	if( kind < BTC_TIME_KIND_COUNT ) {
		S_btc_time_info.time[kind] = 0;
		S_btc_time_info.flags &= (~flag);
	}
}
/****************************************************************************/
/* 関数名称		:	btc_time_CheckTimeout									*/
/* 機能の説明	:	タイムアウトチェック									*/
/* 引数			:	kind			時刻種別								*/
/* 引数			:	timeout_10msec	タイムアウト時間(1カウント10msec)		*/
/* 戻り値		:	1: タイムアウト検出										*/
/* 戻り値		:	0: タイムアウトなし										*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/03												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL SI_32	btc_time_CheckTimeout( E_BTC_TIME_KIND kind, UI_16 timeout_10msec )
{
	UI_32	flag = (1UL << kind);
	SI_32	ret = 0L;

	if( kind < BTC_TIME_KIND_COUNT ) {
		if( 0UL != (S_btc_time_info.flags & flag) ) {
			/* 時刻有効ならチェック */
			if (Timer_ctrl_TimeCompare10ms(S_btc_time_info.time[kind], timeout_10msec) == N_OK) {
				/* タイムアウト検出 */
				ret = 1L;
			}
		}
	}
	return ret;
}

/****************************************************************************/
/* 関数名称		:	btc_time_CheckTimerStartup								*/
/* 機能の説明	:	タイマー起動中判定										*/
/* 引数			:	kind	時刻種別										*/
/* 戻り値		:	1: タイマー起動中										*/
/* 戻り値		:	0: タイマー未起動										*/
/* 作成者		:	(OFT)Sakata												*/
/* 作成日		:	17/08/28												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL SI_32	btc_time_CheckTimerStartup( E_BTC_TIME_KIND kind )
{
	UI_32	flag = (1UL << kind);
	SI_32	ret = 0L;

	if( kind < BTC_TIME_KIND_COUNT ) {
		if( 0UL != (S_btc_time_info.flags & flag) ){
			ret = 1L;
		}
	}
	return ret;
}
/*==========================================================================*/
/* コールバック関数															*/
/*==========================================================================*/
/****************************************************************************/
/* 関数名称		:	btc_cb_mdlReceiveData									*/
/* 機能の説明	:	BTミドルコールバック関数: データ受信					*/
/* 引数			:	buf	受信データ											*/
/* 引数			:	len	受信データ長										*/
/* 戻り値		:	なし													*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/09/03												*/
/* 備考			:															*/
/****************************************************************************/
BTC_LOCAL void btc_cb_mdlReceiveData( const UI_8 *buf, UI_16 len )
{
	E_RC	rc;

	if( NULL != buf ) {
		/* 受信データをリングバッファに記録 */
		rc = btc_ring_Write( &S_btc_recv_ring, len, buf );
		if( E_RC_OK != rc ) {
			/* 異常発生 */
			OUTLOG(OutputLog( "[BTC] btc_ring_Write ERROR(%d:len=%d)\n", rc, len );)
		}
	}
}
