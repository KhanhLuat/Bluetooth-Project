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
/* file DTO459_VDV_Sd_ctrl.c							*/
/* SD制御												*/
/********************************************************/

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include <string.h>
#include "system.h"
#include "SSFTSTD_Macro.h"
#include "System_control_TASK.h"
#include "SHRVSTD_Interrupt_Drv.h"
#include "SHRVSTD_Port_Drv.h"
#include "timer_ctrl.h"
#include "DTO459_VDV_RringBuffer.h"
#include "DTO459_VDV_Sd_Drv.h"
#include "DTO459_VDV_Sw_ctrl.h"
#include "DTO459_VDV_File_io.h"
#include "DTO459_VDV_Gnss_ctrl.h"
#include "DTO459_VDV_Can_ctrl.h"
#include "DTO459_VDV_Sd_ctrl.h"
#include "DTO459_VDV_Eeprom_ctrl.h"
#include "DTO459_VDV_Common.h"
#include "DTO459_VDV_Inspection_ctrl.h"
#include "dbglog.h"



/********************************************************/
/*	定数定義											*/
/********************************************************/
/* SD制御状態（SDモジュール内部の制御状態） */
typedef enum e_sd_ctrl_stat {
	SD_CTRL_STAT_INIT = 0L,								/* [S00]初期状態						*/
	SD_CTRL_STAT_IDLE,									/* [S01]アイドル						*/
	SD_CTRL_STAT_GET_SYSMODE,							/* [S02]システム動作モード取得			*/
	SD_CTRL_STAT_REC,									/* [S03]REC中							*/
	SD_CTRL_STAT_WAIT_REC_END,							/* [S04]REC終了待ち						*/
	SD_CTRL_STAT_SIMULATION,							/* [S05]シミュレーション中				*/
	SD_CTRL_STAT_INSPECTION,							/* [S06]SD検査							*/
	SD_CTRL_STAT_WAIT_SHUTDOWN,							/* [S07]SDモジュール停止待ち			*/
	SD_CTRL_STAT_SHUTDOWN,								/* [S08]SDモジュール停止				*/
	/* ↑状態はここより上に追加すること */
	SD_CTRL_STAT_COUNT									/*	状態定義数	*/
} E_SD_CTRL_STAT;

/* シミュレーション制御状態 */
typedef enum e_sd_sim_stat {
	SD_SIM_STAT_NONE = 0L,	
	SD_SIM_STAT_SEARCH_SIM_FILE,						/* SIMファイル検索中		 				*/
	SD_SIM_STAT_WAIT_HEADER,							/* ファイルヘッダ受信待ち 					*/
	SD_SIM_STAT_WAIT_FIX_ID_LIST,						/* 固定信号ID受信待ち 						*/
	SD_SIM_STAT_WAIT_FIX_VALUES,						/* 固定車両情報受信待ち						*/
	SD_SIM_STAT_WAIT_VAR_ID_LIST,						/* 変数信号ID受信待ち						*/
	SD_SIM_STAT_WAIT_VAR_VALUES,						/* 車両情報受信待ち 						*/
	SD_SIM_STAT_WAIT_STOP,								/* 停止待ち									*/
	SD_SIM_STAT_WAIT_STOP_ACC_OFF,						/* ACC OFFによる停止待ち					*/
	SD_SIM_STAT_ACC_OFF,								/* ACC OFF									*/
	/* ↑状態はここより上に追加すること */
	SD_SIM_STAT_COUNT									/*	状態定義数	*/
} E_SD_SIM_STAT;

typedef enum e_sd_ctrl_sys_mode {
	SD_CTRL_SYS_MODE_UNKNOWN = 0,						/* モード不明(初期値)									*/
	SD_CTRL_SYS_MODE_NOMAL,								/* 通常(SDが挿入されていない、またはSDにモードファイルが存在しない場合に設定)	*/
	SD_CTRL_SYS_MODE_SIMULATION,						/* シミュレーション										*/
	SD_CTRL_SYS_MODE_EEPROM,							/* EEPROM書き換え										*/
	SD_CTRL_SYS_MODE_FIRMWARE,							/* ファームウェア書き換え								*/
	SD_CTRL_SYS_MODE_INSPECTION,						/* 検査													*/
	SD_CTRL_SYS_MODE_BT_COMM,							/* BTモジュール通信										*/
	SD_CTRL_SYS_MODE_GNSS_COMM							/* GNSSモジュール通信									*/
} E_SD_CTRL_SYS_MODE;

/* シミュレーション制御初期化モード */
typedef enum e_sd_sim_init_mode {
	SD_SIM_INIT_MODE_ENTER_SIM = 0,						/* 上位遷移表からシミュレーションモードに入る		*/
	SD_SIM_INIT_MODE_REWIND_SIM,						/* 下位遷移表でSIMファイルの開始および巻き戻し		*/
	SD_SIM_INIT_MODE_CONTINUE							/* ファイル終端から続けて読みこむとき				*/
} E_SD_SIM_INIT_MODE;

/* シミュレーション制御時間種別 */
typedef enum e_sd_sim_time_kind {
	SD_SIM_TIME_KIND_LAST_SEARCH_START = 0,				/* 最後に検索を開始した時間							*/
	/* ↑これより上に追加すること */
	SD_SIM_TIME_KIND_COUNT
} E_SD_SIM_TIME_KIND;

/* シミュレーションファイル部分定義		*/
typedef enum e_sim_file_part {
	SIM_FILE_PART_HEADER	= 0,						/* ファイルヘッダ							*/
	SIM_FILE_PART_FIX_ID_LIST,							/* 固定IDリスト								*/
	SIM_FILE_PART_FIX_VALUES,							/* 固定車両情報								*/
	SIM_FILE_PART_VAR_ID_LIST,							/* 変数IDリスト								*/
	SIM_FILE_PART_VAR_VALUES,							/* 変数車両情報								*/
	/* ↑これより上に追加すること */
	SIM_FILE_PART_COUNT
} E_SIM_FILE_PART;

/* SD制御イベント */
typedef UI_32			SD_CTRL_EVENT; 
#define SD_CTRL_EVENT_NONE						(0L)			/* イベントなし								*/
#define SD_CTRL_EVENT_REQ_STARTUP				(1UL << 1)		/* [E01]SDモジュール起動要求				*/
#define SD_CTRL_EVENT_REQ_SHUTDOWN				(1UL << 2)		/* [E02]SDモジュール停止要求				*/
#define SD_CTRL_EVENT_REQ_GET_SYSMODE			(1UL << 3)		/* [E03]システム動作モード取得要求			*/
#define SD_CTRL_EVENT_NOTIFY_GET_SYSMODE		(1UL << 4)		/* [E04]システム動作モード取得完了通知		*/
#define SD_CTRL_EVENT_REQ_START_REC				(1UL << 5)		/* [E05]REC開始要求							*/
#define SD_CTRL_EVENT_REQ_WRITE_REC_DATA		(1UL << 6)		/* [E06]RECデータ書き込み要求				*/
#define SD_CTRL_EVENT_NOTIFY_WRITE_RESULT		(1UL << 7)		/* [E07]RECデータ書き込み結果通知			*/
#define SD_CTRL_EVENT_REQ_STOP_REC				(1UL << 8)		/* [E08]REC停止要求							*/
#define SD_CTRL_EVENT_REQ_START_SIM				(1UL << 9)		/* [E09]シミュレーション開始要求			*/
#define SD_CTRL_EVENT_NOTIFY_STOP_REC			(1UL << 10)		/* [E10]REC停止完了通知						*/
#define SD_CTRL_EVENT_NOTIFY_SHUTDOWN			(1UL << 11)		/* [E11]SDモジュール停止完了通知			*/
#define SD_CTRL_EVENT_REQ_INSPECTION			(1UL << 12)		/* [E12]SD検査要求							*/
#define SD_CTRL_EVENT_REQ_INSPECTION_START		(1UL << 13)		/* [E13]SD検査開始							*/
#define SD_CTRL_EVENT_NOTIFY_INSPECTION_SD_RES	(1UL << 14)		/* [E14]SD検査用SDアクセス結果				*/


/* SD書き込み制御 */
/*実データ部1フレームのサイズ
　ヘッダ：4byte
　データ：6+(データ数ｘ4)byte
　フッタ：3byte
　実データ部フレームサイズ＝13 + (データ数ｘ4)
　
　例）ID40個なら 13 + (40ｘ4) ＝173 byteとなる
　25ms間隔で出力するため1000ms間に40フレーム必要
*/
#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata */
/*25ms間隔で出力する場合、1000ms間で40フレーム*/
/*1フレーム165byte x 40フレーム = 6600 byte) 6.5k x 1024 = 6656byteとする*/
#define SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE	(6656)						/* RECデータをSDに書き込むサイズ	*/
#elif defined _VDV_YARIS  /* 2020.02.05 S.Sakata */
/*25ms間隔で出力する場合、1000ms間で40フレーム*/
/*1フレーム169byte x 40フレーム = 6760 byte) */
#define SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE	(6760)						/* RECデータをSDに書き込むサイズ	*/
#elif defined _VDV_86B  /* 2021.02.18 S.Sakata */
/*ID数が41  13 + (41ｘ4) ＝177 byte*/
/*1フレーム177byte x 40フレーム = 7080 byte) 7.0k x 1024 = 7168byte*/
#define SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE	(7168)						/* RECデータをSDに書き込むサイズ	*/
#else
#define SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE	(5 * 1024)					/* RECデータをSDに書き込むサイズ	*/
#endif /* _VDV_SUPRA　2018.12.14 S.Sakata */
#define SD_CTRL_REC_DATA_WRITE_BLOCK_NUM	(3)							/* RECデータのバッファ面数			*/
#define	SD_CTRL_REC_TIME_MAX				(8UL * 3600UL * 1000UL)		/* REC最大時間[ms]（8時間） 		*/
#define SD_CTRL_FIO_WRITE_NOTFY_NUM			(SD_CTRL_REC_DATA_WRITE_BLOCK_NUM + 2)	/* FileIoからの書き込み完了通知数	*/
													/* FileIoからの書き込み完了通知数＝FileIoに対する最大要求数	*/
													/*	＝RECデータの書き込み要求数（＝リングバッファ面数）		*/
													/*		＋ヘッダ書き込み要求数（＝2（初回と最後の更新））	*/
#define SD_CTRL_MAX_FIX_ID_COUNT			(30U)						/* 固定IDの最大数（増加考慮）	*/
#define SD_CTRL_MAX_VAR_ID_COUNT			(60U)						/* 変数IDの最大数（増加考慮）	*/
#define SD_CTRL_MAX_ERRCODE_COUNT			(10U)						/*	最大エラーコード数			*/

#define SD_CTRL_FILE_HEADER_PART_SIZE_MAX		(90U + (2U * SD_CTRL_MAX_FIX_ID_COUNT) + (2U * SD_CTRL_MAX_VAR_ID_COUNT))	/* ファイルヘッダ部最大サイズ		*/
#define SD_CTRL_FIX_IDINFO_DATA_PART_SIZE_MAX	(8U + (2U * SD_CTRL_MAX_FIX_ID_COUNT))										/* 固定ID情報部最大サイズ			*/
#define SD_CTRL_VAR_IDINFO_DATA_PART_SIZE_MAX	(8U + (2U * SD_CTRL_MAX_VAR_ID_COUNT))										/* 変数ID情報部最大サイズ			*/
#define SD_CTRL_FIX_SIGNAL_DATA_PART_SIZE_MAX	(13U + (6U * SD_CTRL_MAX_ERRCODE_COUNT) + (4U * SD_CTRL_MAX_FIX_ID_COUNT))	/* 固定ID 車両情報部最大サイズ		*/
#define SD_CTRL_VAR_SIGNAL_DATA_PART_SIZE_MAX	(13U + (6U * SD_CTRL_MAX_ERRCODE_COUNT) + (4U * SD_CTRL_MAX_VAR_ID_COUNT))	/* 変数ID 車両情報部最大サイズ		*/

#define SD_CTRL_REC_DATA_HEAD_SIZE_MAX			(SD_CTRL_FILE_HEADER_PART_SIZE_MAX + SD_CTRL_FIX_IDINFO_DATA_PART_SIZE_MAX + SD_CTRL_FIX_SIGNAL_DATA_PART_SIZE_MAX + SD_CTRL_VAR_IDINFO_DATA_PART_SIZE_MAX)
																															/* RECヘッダ最大サイズ				*/
#define SD_CTRL_REC_HEAD_BUF_SIZE				(((SD_CTRL_REC_DATA_HEAD_SIZE_MAX + 3) / 4) * 4)							/* RECヘッダバッファサイズ(4byteアライメント調整)	*/
#define SD_CTRL_REC_DATA_BUF_SIZE				(((SD_CTRL_VAR_SIGNAL_DATA_PART_SIZE_MAX + 3) / 4) * 4)						/* RECデータバッファサイズ(4byteアライメント調整)	*/
#if (SD_CTRL_REC_HEAD_BUF_SIZE > SD_CTRL_REC_DATA_BUF_SIZE)
#define SD_CTRL_REC_MAX_PACKET_SIZE				SD_CTRL_REC_HEAD_BUF_SIZE
#else
#define SD_CTRL_REC_MAX_PACKET_SIZE				SD_CTRL_REC_DATA_BUF_SIZE
#endif

/* システム動作モード */
#define SD_CTRL_SYS_MODE_FILE_NAME			"mode.inf"
#define SD_CTRL_SYS_MODE_FILE_PATH			FS_DRV_PATH_SD SD_CTRL_SYS_MODE_FILE_NAME
#define SD_CTRL_SYS_MODE_FILE_SIZE			(8)

/* システム動作モードファイル内のモード値 */
#define	SD_CTRL_SYS_MODE_FILE_VALUE_SIMU	(0xF0)
#define	SD_CTRL_SYS_MODE_FILE_VALUE_EEPROM	(0xF1)
#define	SD_CTRL_SYS_MODE_FILE_VALUE_FIRM	(0xF2)
#define	SD_CTRL_SYS_MODE_FILE_VALUE_INSPECTION	(0xF3)
#define	SD_CTRL_SYS_MODE_FILE_VALUE_BT_COMM		(0xF4)
#define	SD_CTRL_SYS_MODE_FILE_VALUE_GNSS_COMM	(0xF5)

#define	SD_CTRL_REC_FILE_EXTENSION			".vdv"

/*	VDV SDデータフォーマット	*/
#define SD_CTRL_FORMAT_VERSION				(0x00010000)	/*	フォーマットバージョン	*/
/*												 ^^^^^^
												  | | |
												  | | |
												  | | メンテナスバージョン
												  | マイナーバージョン
												  メジャーバージョン
*/

/* ヘッダ */
/* START	1byte */
#define SD_CTRL_FMT_HEADER_START			(0x7EU)			/*	START						*/
/* LEN		2byte */
/* TYPE		1byte */
#define SD_CTRL_FMT_HEADER_BYTES			(4U)			/*	ヘッダサイズ				*/
#define SD_CTRL_FMT_UTC_LEN					(12U)			/*	UTC時刻表記長(YYMMDDHHmmSS)	*/

/* TYPE: データ種別 */
#define SD_CTRL_FMT_TYPE_HEADER				(0x20U)			/*	車両信号一覧（SDカード用）	*/
#define SD_CTRL_FMT_TYPE_ID_INFO_DATA		(0x21U)			/*	車両情報ID（SDカード用）	*/
#define SD_CTRL_FMT_TYPE_SIGNAL_DATA		(0x22U)			/*	車両情報（SDカード用）		*/
#define SD_CTRL_FMT_TYPE_INVALID			(0xFFU)			/*	不正なデータ種別			*/

/* フッタ */
/* CRC		2byte */
/* END		1byte */
#define SD_CTRL_FMT_FOOTER_END				(0x7FU)			/*	END							*/
#define SD_CTRL_FMT_FOOTER_BYTES			(3U)			/*	フッタサイズ				*/

#define SD_CTRL_FMT_BYTES_FIX				(SD_CTRL_FMT_HEADER_BYTES + SD_CTRL_FMT_FOOTER_BYTES)

#define SD_SIM_SIGNAL_INVALID_VALUE			(0xFFFFFFFFL)

/* シミュレーション制御イベント */
typedef UI_32			SD_SIM_EVENT; 
#define SD_SIM_EVENT_NONE					(0L)			/* イベントなし								*/
#define SD_SIM_EVENT_FOUND_SIM_FILE			(1UL << 0)		/* SIMファイル検索完了						*/
#define SD_SIM_EVENT_READ_ERROR				(1UL << 1)		/* ファイル読みエラー発生					*/
#define SD_SIM_EVENT_RECV_HEADER			(1UL << 2)		/* ファイルヘッダ受信						*/
#define SD_SIM_EVENT_RECV_FIX_ID_LIST		(1UL << 3)		/* 固定ID情報受信							*/
#define SD_SIM_EVENT_RECV_FIX_VALUES		(1UL << 4)		/* 固定車両情報受信							*/
#define SD_SIM_EVENT_RECV_VAR_ID_LIST		(1UL << 5)		/* 変数ID情報受信							*/
#define SD_SIM_EVENT_RECV_VAR_VALUES		(1UL << 6)		/* 変数車両情報受信							*/
#define SD_SIM_EVENT_REQ_REWIND				(1UL << 7)		/* シミュレーション頭出し要求				*/
#define SD_SIM_EVENT_END_READ				(1UL << 8)		/* ファイル読み終了							*/
#define SD_SIM_EVENT_ACC_OFF				(1UL << 9)		/* ACC OFF									*/

/* シミュレーション進行フラグ				*/
#define SD_SIM_FLAG_FOUND_SIM_FILE			(1UL << 0)		/* SIMファイル発見済			*/
#define SD_SIM_FLAG_GET_FILE_SIZE			(1UL << 1)		/* SIMファイルサイズ取得済		*/
#define SD_SIM_FLAG_RECV_HEADER				(1UL << 2)		/* ヘッダ受信済					*/
#define SD_SIM_FLAG_RECV_FIX_ID_LIST		(1UL << 3)		/* 固定IDリスト取得済			*/
#define SD_SIM_FLAG_RECV_FIX_VALUES			(1UL << 4)		/* 固定車両情報取得済			*/
#define SD_SIM_FLAG_RECV_VAR_ID_LIST		(1UL << 5)		/* 変数IDリスト取得済			*/
#define SD_SIM_FLAG_RECV_VAR_VALUES			(1UL << 6)		/* 変数車両情報取得済			*/
#define SD_SIM_FLAG_READ_ERROR				(1UL << 7)		/* READエラーあり				*/
#define SD_SIM_FLAG_CHANGE_VAR_ID_LIST		(1UL << 8)		/* 変数IDリスト変更検出			*/
#define SD_SIM_FLAG_REQ_REWIND				(1UL << 9)		/* 頭出し要求					*/
/* シミュレーション処理フラグ				*/
#define SD_SIM_FLAG_SEARCHING_FILE			(1UL << 16)		/* ファイルSEARCH中				*/
#define SD_SIM_FLAG_READING_FILE			(1UL << 17)		/* ファイルREAD中				*/

/* 時間定義 */
#define SD_SIM_SEARCH_INTERVAL_MSEC			(1000UL)		/* ファイル検索試行間隔(msec)	*/

/* ログデータ情報フラグ */
#define SD_CTRL_LOG_DATA_TIME_ENABLE		(1UL << 0)		/* 時刻情報有効					*/
#define SD_CTRL_LOG_DATA_POS_ENABLE			(1UL << 1)		/* 座標情報有効					*/

/* ヘッダ更新 */
#define SD_CTRL_UPDATE_HEADER_CYCLE_MSEC	(30 * 1000)		/* ヘッダ更新間隔				*/

#define SD_CTRL_INSPECTION_DIR					FS_DRV_PATH_SD "VDV\\test"	/* 検査ディレクトリ		*/
#define SD_CTRL_INSPECTION_DATA_SIZE			(2)								/* 検査データサイズ		*/

#define SD_CTRL_REC_STOP_TIMEOUT			(2UL * 60UL * 1000UL)	/* REC停止タイムアウト[ms] (2分)	*/

/* SD検査ステップ */
typedef enum e_sd_inspection_step {
	SD_INSPECTION_STEP_MKDIR = 0,					/* 検査用ディレクトリ作成		*/
	SD_INSPECTION_STEP_WRITE,						/* 書き込み検査					*/
	SD_INSPECTION_STEP_READ,						/* 読み込み検査					*/
	SD_INSPECTION_STEP_INVALID						/* 無効値						*/
} E_SD_INSPECTION_STEP;

/********************************************************/
/*	構造体定義											*/
/********************************************************/
/* RECデータ書き込み完了通知情報 */
typedef struct {

	/* 書き込み要求の内容		*/
	UI_32				bytes;						/* 書き込みバイト数						*/

	/* 書き込み結果				*/
	E_FIO_RESULT		result;						/* 書込み結果							*/
	UI_32				bytesWritten;				/* 書き込んだバイト数					*/
} T_SD_CTRL_WRITE_END_NOTIFY_INFO;

/* ログデータ情報 */
typedef struct {
	SI_32				latitudeNorthernMost;		/* 最北端緯度 							*/
	SI_32				longtudeWesternMost;		/* 最西端緯度 							*/
	SI_32				latitudeSouthernMost;		/* 最南端緯度 							*/
	SI_32				longtudeEasternMost;		/* 最東端経度 							*/
	UI_8				timeLogStart[SD_CTRL_FMT_UTC_LEN+4];	/* ログ書き込み開始時間 	*/
	UI_8				timeLogEnd[SD_CTRL_FMT_UTC_LEN+4];		/* ログ書き込み終了時間		*/
						/* 時刻のフォーマット（UTC) YYMMDDHHMMSS(例151110154322） */
	UI_32				flags;						/* ログ情報初期化/更新状態を示すフラグ	*/
} T_SD_CTRL_LOG_DATA_INFO;

/* パケット情報 */
typedef struct {
	UI_16				packet_bytes;					/*	パケット全体の長さ 7E=>7F		*/
	UI_16				length;							/*	ヘッダから抽出した Length		*/
	UI_8				type;							/*	ヘッダから抽出した Type			*/
	UI_8				rsv[3];
	UI_8*				header_part;					/*	バッファ内のヘッダ先頭			*/
	UI_8*				data_part;						/*	バッファ内のデータ先頭			*/
	UI_8*				footer_part;					/*	バッファ内のフッタ先頭			*/
	UI_8				packet[SD_CTRL_REC_MAX_PACKET_SIZE];	
														/*	パケットバッファ				*/
} T_SD_CTRL_PACKET_INFO;

/* シミュレーション制御情報 */
typedef struct t_sd_sim_ctrl_info {
	UI_32					flags;								/* シミュレーション進行フラグ				*/
	T_SD_CTRL_PACKET_INFO	packet_info;						/* パケット情報（シミュレーションデータ）	*/
	UI_32					time_flags;							/* 有効な時間フラグ							*/
	UI_32					time[SD_SIM_TIME_KIND_COUNT];		/* 種別毎の時間記録領域						*/
	UI_32					sim_total_write_size;				/* バッファ書き込み済サイズ（通算）			*/
	UI_32					sim_total_read_size;				/* バッファ読み込み済サイズ（通算）			*/
	UI_32					sim_file_size;						/* シミュレーションファイル全体サイズ		*/
	UI_32					sim_file_read_size;					/* １ファイル内のREAD済サイズ				*/
	UI_32					sim_file_part_size[SIM_FILE_PART_COUNT];
																/* シミュレーションファイル部分サイズ		*/
	UI_8					sim_file_path[((((FIO_PATH_LEN_MAX+1) + 3) / 4) * 4)];
																/* シミュレーションファイルパス(4byteアライメント調整) */
} T_SD_SIM_CTRL_INFO;

/* シミュレーションデータID情報 */
typedef struct t_sd_ctrl_sim_id_info {
	UI_32				randomValue;							/* 乱数						*/
	UI_32				signalListVersion;						/* 信号リストバージョン		*/
	UI_8				fixIdNum;								/* 固定IDの数				*/
	UI_8				varIdNum;								/* 変数IDの数				*/
	UI_16				fixIdList[SD_CTRL_MAX_FIX_ID_COUNT];	/* 固定IDリスト				*/
	UI_16				varIdList[SD_CTRL_MAX_VAR_ID_COUNT];	/* 変数IDリスト				*/
} T_SD_CTRL_SIM_ID_INFO;

/* SD検査情報 */
typedef struct t_sd_ctrl_inspection_info {
	UI_8					insp_req_flg;								/* 検査要求フラグ				*/
	UI_8					fio_req_flg;								/* ファイルアクセス要求フラグ	*/
	UI_16					serialno;									/* シリアルNo					*/
	E_SD_INSPECTION_STEP	step;										/* 検査ステップ					*/
	UI_8					filepath[FIO_PATH_LEN_MAX];					/* ファイルパス					*/
	UI_8					write_buf[SD_CTRL_INSPECTION_DATA_SIZE];	/* SD書き込み用バッファ			*/
	UI_8					read_buf[SD_CTRL_INSPECTION_DATA_SIZE];		/* SD読み込み用バッファ			*/

	E_FIO_RESULT			fio_result;									/* ファイルアクセス結果			*/
	UI_32					fio_bytes;									/* ファイルに書き込めたサイズ	*/
} T_SD_CTRL_INSPECTION_INFO;


/* 状態関数 */
typedef E_SD_CTRL_STAT (*PFN_SD_STATE_PROC)( SD_CTRL_EVENT event );
typedef E_SD_SIM_STAT (*PFN_SD_SIM_STATE_PROC)( SD_SIM_EVENT event, B_8* fcontinue );

/********************************************************/
/*	内部変数定義										*/
/********************************************************/
/* SD制御状態(SDモジュール内部の状態) */
static E_SD_CTRL_STAT	S_sd_stat;
#ifdef _VDV_APP
static SD_PROCESS_TYPE		S_processType;
#endif /* _VDV_APP */

/* ファイルIOからの通知フラグ */
static UI_8	S_sd_fio_notify_get_mode;		/* モードファイル取得完了通知フラグ */
static UI_8	S_sd_fio_notify_write;			/* RECデータ書き込み完了通知フラグ */
static UI_8	S_sd_fio_notify_inspection;		/* SD検査用ファイルアクセス完了通知フラグ */

/* システム動作モード */
static UI_8 S_sd_sys_mode_req_flg;
static UI_8 S_sd_sys_mode_data[SD_CTRL_SYS_MODE_FILE_SIZE];
static E_SD_CTRL_SYS_MODE S_sd_sys_mode;
#ifdef _VDV_APP
/* RECボタンON/OFF変化カウント */
static UI_32 S_sd_recBtnOnOffCnt;

/* RECデータバッファ */
static UI_8 S_sd_rec_head[SD_CTRL_REC_HEAD_BUF_SIZE];
static UI_8 S_sd_rec_data[SD_CTRL_REC_DATA_BUF_SIZE];
static SI_32 S_sd_signal_buff[SD_CTRL_MAX_FIX_ID_COUNT+SD_CTRL_MAX_VAR_ID_COUNT];
static T_RINGBUFFER S_sd_rec_data_mng;							/* RECデータリングバッファ管理情報	*/
static UI_8 S_sd_rec_data_buff[(SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE * SD_CTRL_REC_DATA_WRITE_BLOCK_NUM)];
																/* RECデータリングバッファ			*/
static UI_32 S_sd_ring_write_size;								/* リングバッファに書き込んだRECデータサイズ		*/
static UI_32 S_sd_ring_flush_size;								/* リングバッファからSDに書き込んだサイズ			*/

/* RECデータ書き込み回数	*/
static UI_32 S_sd_rec_write_req_cnt;							/* 書き込み要求回数					*/
static UI_32 S_sd_rec_write_notify_cnt;							/* 書き込み通知回数					*/

/* RECデータ書き込み結果通知情報バッファ */
static T_RINGBUFFER S_sd_rec_write_notify_mng;
static T_SD_CTRL_WRITE_END_NOTIFY_INFO S_sd_rec_write_notify_buff[(sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO) * SD_CTRL_FIO_WRITE_NOTFY_NUM)];
static T_SD_CTRL_WRITE_END_NOTIFY_INFO S_sd_notifyinfo_set_buff;
static T_SD_CTRL_WRITE_END_NOTIFY_INFO S_sd_notifyinfo_get_buff;

/* RECデータ書き込み管理情報	*/
static UI_32 S_sd_write_start_time;								/* REC開始時間				*/
static UI_32 S_sd_update_header_time;							/* ヘッダ更新時間(LAST)		*/

/* RECデータファイルパス(4byteアライメント調整) */
static UI_8 S_sd_rec_filepath[((((FIO_PATH_LEN_MAX+1) + 3) / 4) * 4)];

/* ログデータ情報 */
static T_SD_CTRL_LOG_DATA_INFO	S_sd_log_data_info;

/* シミュレーション関連情報 */
static E_SD_SIM_STAT			S_sd_sim_status;					/* シミュレーション状態						*/
static T_SD_SIM_CTRL_INFO		S_sd_sim_ctrl_info;					/* シミュレーション制御情報					*/
static T_SD_CTRL_SIM_ID_INFO	S_sd_sim_id_info;					/* シミュレーションデータID情報				*/
/* 車両情報（信号データ）実体は S_sd_signal_buff に記録 */
/*	S_sd_signal_buff[0]～[S_sd_sim_fix_id_num - 1] */
/*		→ 固定車両情報 */
/*	S_sd_signal_buff[S_sd_sim_fix_id_num]～[S_sd_sim_fix_id_num + S_sd_sim_var_id_num - 1] */
/*		→ 変数車両情報 */

/* SD検査情報 */
static T_SD_CTRL_INSPECTION_INFO	S_sd_inspection_info;

static UI_32	S_sd_running_distance;								/* 走行距離					*/
static UI_32	S_sd_speed_stop_time;								/* 停車時間					*/

#endif /* _VDV_APP */

/********************************************************/
/*	内部関数宣言										*/
/********************************************************/
/* イベント操作関数	*/
static void sd_ctrl_state_MakeEvent( SD_PROCESS_TYPE processType, SD_CTRL_EVENT *event );

/* 状態遷移時関数 */
static void sd_ctrl_state_ProcOnEnter( E_SD_CTRL_STAT stat );
static void sd_ctrl_state_ProcOnLeave( E_SD_CTRL_STAT stat );

/* 状態関数	*/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcInit( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcIdle( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcGetSysMode( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcRec( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcWaitRecEnd( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcSimulation( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcInspection( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcWaitShutdown( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcShutdown( SD_CTRL_EVENT event );

/* 初期化関数	*/
static void sd_ctrl_InitModuleParam( void );
#ifdef _VDV_APP
static void sd_ctrl_InitRecParam( void );
static void sd_ctrl_InitSimParam( void );
#endif /* _VDV_APP */

/* システム動作モード取得コールバック関数	*/
static void sd_ctrl_SysModeCallback( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo );

#ifdef _VDV_APP
/* REC関連関数	*/
static void sd_ctrl_RecDataWriteCallback( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo );
static B_8 sd_ctrl_CheckWriteStartCnd( void );
static B_8 sd_ctrl_CheckWriteEndCnd( void );
static void sd_ctrl_MakeRecFilepath( void );
static void sd_ctrl_MakeRecHead( UI_8 *headBuff, UI_16 buffSize, UI_16 *headSize, B_8 init );
static void sd_ctrl_MakeRecData( UI_8 *dataBuff, UI_16 buffSize, UI_16 *dataSize );
static void sd_ctrl_MakeFileHeaderPart( UI_8 *headBuff, UI_16 buffSize, UI_16 *headSize );
static void sd_ctrl_MakeIdInfoDataPart( UI_8 signalNum, const UI_16 *idList, UI_8 *dataBuff, UI_16 buffSize, UI_16 *dataSize );
static void sd_ctrl_MakeSignalDataPart( UI_8 signalNum, const SI_32 *signalValues, UI_8 *dataBuff, UI_16 buffSize, UI_16 *dataSize );
static void sd_ctrl_UpdateLogDataInfo( B_8 init );
static UI_32 sd_ctrl_CheckGnssHnrInfo( const T_Gnss_ctrl_nav_pvt_info* hnr_info );
static void sd_ctrl_WriteRecData( UI_8 *dataBuff, UI_16 dataSize );
static void sd_ctrl_FlushRecData( B_8 allFlush );
static void sd_ctrl_UpdateHeader( void );
static void sd_ctrl_EndRec( void );
static UI_16 sd_ctrl_GetFileHeaderPartSize( UI_8 fixedSignalNum, UI_8 vehicleSignalNum );
static UI_16 sd_ctrl_GetIdInfoDataPartSize( UI_8 signalNum );
static UI_16 sd_ctrl_GetSignalDataPartSize( UI_8 signalNum, UI_8 errNum );

/* シミュレーション制御関数	*/
static void sd_sim_state_MainProc( void );
static void sd_sim_state_MakeEvent( SD_SIM_EVENT* sim_event );
static void sd_sim_state_ProcOnEnter( E_SD_SIM_STAT stat );
static void sd_sim_state_ProcOnLeave( E_SD_SIM_STAT stat );

/* シミュレーション操作関数 */
static void sd_sim_InitSimCtrlInfo( E_SD_SIM_INIT_MODE mode );
static void sd_sim_SearchSimFile( void );
static void sd_sim_SearchSimFileCB( const T_COMMON_SRC_VDVF_RET *retinfo );
static void sd_sim_ReadSimFile( void );
static void sd_sim_ReadSimFileCB( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo );
static B_8 sd_sim_GetReadBlock( UI_8** block, UI_16* block_size );
static void sd_sim_RegTime( E_SD_SIM_TIME_KIND kind );
static void sd_sim_ClearTime( E_SD_SIM_TIME_KIND kind );
static SI_32 sd_sim_CheckTimeout( E_SD_SIM_TIME_KIND kind, UI_32 msec );
static void sd_sim_ClearReadError( void );

/* 状態関数(シミュレーション)	*/
static E_SD_SIM_STAT sd_sim_stat_None( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_SearchSimFile( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_WaitHeader( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_WaitFixIdList( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_WaitFixValues( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_WaitVarIdList( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_WaitVarValues( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_WaitStop( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_WaitStopAccOff( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_AccOff( SD_SIM_EVENT sim_event, B_8* fcontinue );

/* 受信データ解析関数 */
static E_RC sd_sim_data_AnalyzeRecvData( T_SD_CTRL_PACKET_INFO* packet_info );
static SI_32 sd_sim_data_SkipToSTART( void );
static void sd_sim_data_ClearRecvPacket( T_SD_CTRL_PACKET_INFO* packet_info );
static SI_32 sd_sim_data_GetPacketInfo( T_SD_CTRL_PACKET_INFO* packet_info );
static UI_16 sd_sim_data_GetRecvDataSize( void );
static E_RC sd_sim_data_ReadRecvData( UI_16 size, UI_8* buff, UI_8 fclear );
static SI_32 sd_sim_data_GetIDList( T_SD_CTRL_PACKET_INFO* packet_info, UI_16* idList, UI_8 max_idNum );
static SI_32 sd_sim_data_GetValues( T_SD_CTRL_PACKET_INFO* packet_info, SI_32* values, UI_8 max_valueNum );

/* 信号データ操作 */
static void sd_sim_signal_ClearBuffer( B_8 clear_all );
static SI_32 sd_sim_signal_AnalyzeHeader( const T_SD_CTRL_PACKET_INFO* packet_info );
static SI_32 sd_sim_signal_SetIDList( B_8 fix, T_SD_CTRL_PACKET_INFO* packet_info );
static SI_32 sd_sim_signal_SetValues( B_8 fix, T_SD_CTRL_PACKET_INFO* packet_info );
static UI_32 sd_sim_signal_GetFixPartSize( void );

static void sd_ctrl_InitInspectionInfo(void);
static UI_8 sd_ctrl_Inspect(void);
static void sd_ctrl_InspectionFioCallback( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo );
static void sd_ctrl_InspectionDirCallback( const T_FIO_DIR_REQ_INFO *reqinfo, const T_FIO_DIR_RET_INFO *retinfo );


#endif /* _VDV_APP */

/* 状態関数テーブル	*/
static const PFN_SD_STATE_PROC S_sd_state_proc[] = {
	sd_ctrl_stat_ProcInit,					/*	初期状態					*/
	sd_ctrl_stat_ProcIdle,					/*	アイドル					*/
	sd_ctrl_stat_ProcGetSysMode,			/*	システム動作モード取得		*/
	sd_ctrl_stat_ProcRec,					/*	REC中						*/
	sd_ctrl_stat_ProcWaitRecEnd,			/*	REC終了待ち					*/
	sd_ctrl_stat_ProcSimulation,			/*	シミュレーション中			*/
	sd_ctrl_stat_ProcInspection,			/*	SD検査						*/
	sd_ctrl_stat_ProcWaitShutdown,			/*	SDモジュール停止待ち		*/
	sd_ctrl_stat_ProcShutdown,				/*	SDモジュール停止			*/
};

#ifdef _VDV_APP
/* 状態関数テーブル(シミュレーション)	*/
static const PFN_SD_SIM_STATE_PROC S_sd_sim_state_proc[] = {
	sd_sim_stat_None,
	sd_sim_stat_SearchSimFile,				/*	SIMファイル検索中		 	*/
	sd_sim_stat_WaitHeader,					/*	ファイルヘッダ受信待ち		*/
	sd_sim_stat_WaitFixIdList,				/*	固定信号ID受信待ち			*/
	sd_sim_stat_WaitFixValues,				/*	固定車両情報受信待ち		*/
	sd_sim_stat_WaitVarIdList,				/*	変数信号ID受信待ち			*/
	sd_sim_stat_WaitVarValues,				/*	車両情報受信待ち			*/
	sd_sim_stat_WaitStop,					/*	停止待ち					*/
	sd_sim_stat_WaitStopAccOff,				/*	ACC OFFによる停止待ち		*/
	sd_sim_stat_AccOff,						/*	ACC OFF						*/
};
#endif /* _VDV_APP */

/********************************************************/
/*	外部関数											*/
/********************************************************/

/********************************************************/
/* 関数名称		: Sd_control_Init						*/
/* 機能の説明	: SD制御初期化							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Sd_control_Init(void)
{
	/* モジュールで使用する変数初期化 */
	sd_ctrl_InitModuleParam();

	/* 初期状態設定	*/
	S_sd_stat = SD_CTRL_STAT_INIT;

	/* SDカードドライバ初期化 */
	SDCardDrv_Init();
}

/********************************************************/
/* 関数名称		: Sd_control_Main						*/
/* 機能の説明	: SD制御メイン							*/
/* 引数			: (I/ )processType プロセス種別			*/
/* 					SD_PROCESS_MAIN:通常処理	10ms毎	*/
/* 					SD_PROCESS_SUB :サブ処理	25ms毎	*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Sd_control_Main(SD_PROCESS_TYPE processType)
{
	SD_CTRL_EVENT		event;
	E_SD_CTRL_STAT		nextStat;
	
	/* イベント作成 */
	sd_ctrl_state_MakeEvent( processType, &event );

#ifdef _VDV_APP
	/* プロセス種別を記録 */
	S_processType = processType;
#endif /* _VDV_APP */

	/* 状態関数コール */
	nextStat = S_sd_state_proc[S_sd_stat]( event );
	if (S_sd_stat != nextStat) {			/* 状態遷移あり		*/
		OUTLOG(OutputLog("[SdCtrl]stat:%d->%d\n", S_sd_stat, nextStat);)
		sd_ctrl_state_ProcOnLeave(S_sd_stat);
		S_sd_stat = nextStat;
		sd_ctrl_state_ProcOnEnter(nextStat);
	}

	return;
}

/************************************************************/
/* 関数名称		: Sd_control_GetRecStatus					*/
/* 機能の説明	: SD REC状態取得							*/
/* 引数			: void										*/
/* 戻り値		: 処理結果 E_SD_CONTROL_REC_STAT			*/
/* 作成者		: (OFT)Sakata								*/
/* 作成日		: 16/08/18									*/
/* 備考			: 											*/
/************************************************************/
E_SD_CONTROL_REC_STAT Sd_control_GetRecStatus(void)
{
	E_SD_CONTROL_REC_STAT recStat;

	if (S_sd_stat == SD_CTRL_STAT_INIT) {
		recStat = SD_CONTROL_REC_STAT_UNKNOWN;
	}
	else if ((S_sd_stat == SD_CTRL_STAT_REC) ||
			 (S_sd_stat == SD_CTRL_STAT_WAIT_REC_END)) {
		recStat = SD_CONTROL_REC_STAT_RECORDING;
	}
	else {
		recStat = SD_CONTROL_REC_STAT_UNRECORDING;
	}
	return recStat;
}

/********************************************************/
/* 関数名称		: Sd_control_GetSignalList				*/
/* 機能の説明	: 公開信号ID一覧取得					*/
/* 引数			: (I/ )idtype			信号種別		*/
/*				  ( /O)signal_Num		信号数			*/
/*				  ( /O)signal_List		信号リスト		*/
/* 戻り値		: 正常	:N_OK							*/
/*				  異常	:N_NG							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Sd_control_GetSignalList(E_SIGNAL_ID_TYPE idtype, UI_8 *signal_Num, const UI_16 **signal_List)
{
#ifdef _VDV_APP
	SI_8	ret = N_NG;

	if( (NULL != signal_Num) && (NULL != signal_List) ) {
		*signal_Num = 0;
		*signal_List = NULL;
		if( (0 < S_sd_sim_id_info.fixIdNum) && (0 < S_sd_sim_id_info.varIdNum) ) {
			switch( idtype ) {
			case ID_TYPE_FIXED_ID:		/**< 信号種別:固定値	*/
				*signal_Num = S_sd_sim_id_info.fixIdNum;
				*signal_List = S_sd_sim_id_info.fixIdList;
				ret = N_OK;
				break;

			case ID_TYPE_VARIABLE_ID:	/**< 信号種別:変数値	*/
				*signal_Num = S_sd_sim_id_info.varIdNum;
				*signal_List = S_sd_sim_id_info.varIdList;
				ret = N_OK;
				break;

			default:
				break;
			}
		}
	}
	return ret;
#else /* _VDV_APP */
	return N_NG;
#endif /* _VDV_APP */
}

/********************************************************/
/* 関数名称		: Sd_control_GetEncodeKey				*/
/* 機能の説明	: 暗号化キー取得						*/
/* 引数			: ( /O)key				暗号化用キー	*/
/* 戻り値		: 正常	:N_OK							*/
/*				  異常	:N_NG							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Sd_control_GetEncodeKey(SI_32 *key)
{
#ifdef _VDV_APP
	*key = (SI_32)S_sd_sim_id_info.randomValue;
	return N_OK;
#else /* _VDV_APP */
	*key = 0;
	return N_NG;
#endif /* _VDV_APP */
}


/********************************************************/
/* 関数名称		: Sd_control_GetSignalVersion			*/
/* 機能の説明	: 公開信号バージョン					*/
/* 引数			: void									*/
/* 戻り値		: 公開信号バージョン					*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
UI_32 Sd_control_GetSignalVersion(void)
{
#ifdef _VDV_APP
	return S_sd_sim_id_info.signalListVersion;
#else /* _VDV_APP */
	return 0;
#endif /* _VDV_APP */
}

/********************************************************/
/* 関数名称		: Sd_control_GetSignalsInfo				*/
/* 機能の説明	: 公開信号取得(複数信号)				*/
/* 引数			: (I/ )req_id_count	信号ID数			*/
/* 				: (I/ )req_ids 		信号ID（複数）		*/
/*				  ( /O)signal_values	信号値（複数）	*/
/* 戻り値		: 正常	:N_OK							*/
/*				  異常	:N_NG							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Sd_control_GetSignalsInfo( UI_8 req_id_count, const UI_16* req_ids, SI_32 *signal_values)
{
#ifdef _VDV_APP
	SI_8			ret = N_NG;
	UI_8			i = 0U;
	UI_8			j = 0U;
	UI_16			req_id = 0U;
	UI_16			fixIdNum = 0U;
	UI_16			varIdNum = 0U;
	const UI_16*	fixIdList = NULL;
	const UI_16*	varIdList = NULL;
	B_8				find = N_FALSE;
	SI_32			value = 0L;

	if( (NULL != req_ids) && (NULL != signal_values) ) {
		fixIdNum = S_sd_sim_id_info.fixIdNum;
		varIdNum = S_sd_sim_id_info.varIdNum;
		fixIdList = S_sd_sim_id_info.fixIdList;
		varIdList = S_sd_sim_id_info.varIdList;

		/* 要求信号ID数の処理を行う */
		for( i = 0U; i < req_id_count; i++ ) {
			find = N_FALSE;
			req_id = req_ids[i];
			value = (SI_32)SD_SIM_SIGNAL_INVALID_VALUE;

			/* 固定ID配列を検索 */
			for( j = 0U; j < fixIdNum; j++ ) {
				if( fixIdList[j] == req_id ) {
					/* IDが一致した */
					value = S_sd_signal_buff[j];
					find = N_TRUE;
					break;
				}
			}
			if( N_TRUE != find ) {
				/* 変数ID配列を検索 */
				for( j = 0U; j < varIdNum; j++ ) {
					if( varIdList[j] == req_id ) {
						/* IDが一致した */
						value = S_sd_signal_buff[fixIdNum + j];
						break;
					}
				}
			}
			signal_values[i] = value;
		}
		ret = N_OK;
	}
	return ret;
#else /* _VDV_APP */
	return N_NG;
#endif /* _VDV_APP */
}

#ifdef _VDV_APP
/****************************************************************************/
/* 関数名称		: Sd_control_GetInspectionVariation							*/
/* 機能の説明	: 検査モードのバリエーション情報取得						*/
/* 引数			: variation	バリエーション情報								*/
/* 戻り値		: 正常	:N_OK												*/
/*				  異常	:N_NG												*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 17/01/20													*/
/* 備考			: 															*/
/****************************************************************************/
SI_8 Sd_control_GetInspectionVariation(UI_8 *variation)
{
	if (variation == NULL) {
		return N_NG;
	}

	if (S_sd_sys_mode_data[0] != SD_CTRL_SYS_MODE_FILE_VALUE_INSPECTION) {
		return N_NG;
	}

	*variation = S_sd_sys_mode_data[1];

	return N_OK;
}

/****************************************************************************/
/* 関数名称		: Sd_control_ReqInspection									*/
/* 機能の説明	: SD検査要求												*/
/* 引数			: serialno		シリアルNo									*/
/* 戻り値		: 正常	:N_OK												*/
/*				  異常	:N_NG												*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 17/01/20													*/
/* 備考			: 本関数が正常終了した場合、検査制御モジュールに対して		*/
/*				  Insp_notify_inspection_resultにより検査結果を通知する		*/
/****************************************************************************/
SI_8 Sd_control_ReqInspection(UI_16 serialno)
{
	E_SYS_CTRL_STATUS	sysStat;
	UI_32	filepathlen;

	/* システム状態取得	*/
	sysStat = System_control_GetStatus();
	if (sysStat != SYS_OPERATION_INSPECTION) {
		/* 検査状態ではない	*/
		return N_NG;
	}

	if (S_sd_inspection_info.insp_req_flg == N_ON) {
		/* 検査実行中	*/
		return N_NG;
	}

	/* 検査情報を設定	*/
	sd_ctrl_InitInspectionInfo();
	S_sd_inspection_info.insp_req_flg = N_ON;
	S_sd_inspection_info.serialno = serialno;
	S_sd_inspection_info.filepath[0] = '\0';
	(void)strcpy((CH_8*)S_sd_inspection_info.filepath, SD_CTRL_INSPECTION_DIR);
	(void)strcat((CH_8*)S_sd_inspection_info.filepath, "\\");
	filepathlen = strlen((const CH_8*)S_sd_inspection_info.filepath);
	Common_utoa((UI_32)serialno, &S_sd_inspection_info.filepath[filepathlen]);
	(void)strcat((CH_8*)S_sd_inspection_info.filepath, ".dat");

	return N_OK;
}
#endif /* _VDV_APP */

/****************************************************************************/
/*	内部関数																*/
/****************************************************************************/
/****************************************************************************/
/* 関数名称		:	sd_ctrl_state_MakeEvent									*/
/* 機能の説明	:	イベント作成											*/
/* 引数			:	(I/ )processType	プロセス種別						*/
/* 							SD_PROCESS_MAIN:通常処理	10ms毎				*/
/* 							SD_PROCESS_SUB :サブ処理	25ms毎				*/
/*					( /O)event			イベント							*/
/* 戻り値		:	SD_CTRL_STAT_xxx	遷移先状態							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_ctrl_state_MakeEvent( SD_PROCESS_TYPE processType, SD_CTRL_EVENT *event )
{
	E_SYS_CTRL_STATUS	sysStat;

	if (event == NULL) {
		return;
	}

	*event = SD_CTRL_EVENT_NONE;

	sysStat = System_control_GetStatus();

	switch ( sysStat ) {
	case SYS_OPERATION_WAIT_ACC_ON:				/* システム動作状態:ACC ON待ち	--------------------*/
		/* 何もしない */
		break;

	case SYS_OPERATION_ACC_ON_UNKNOWN_STAT:		/* システム動作状態:ACC ON（状態判定）	------------*/
		if ((S_sd_stat == SD_CTRL_STAT_INIT) ||			/* SD制御状態:初期状態				*/
			(S_sd_stat == SD_CTRL_STAT_SHUTDOWN)) {		/* SD制御状態:SDモジュール停止		*/
			/* SDモジュール起動要求	*/
			*event |= SD_CTRL_EVENT_REQ_STARTUP;
		}
		else if (S_sd_stat == SD_CTRL_STAT_IDLE) {			/* SD制御状態:アイドル				*/
			if (S_sd_sys_mode == SD_CTRL_SYS_MODE_UNKNOWN) {			/* システム動作モード不明（モードファイル未取得）	*/
				if (FileIO_Get_MountExecStat((const UI_8*)FS_DRV_PATH_SD) == N_TRUE) {	/* SDマウント実行済み								*/
					/* システム動作モード取得要求	*/
					*event |= SD_CTRL_EVENT_REQ_GET_SYSMODE;
				}
			}
		}
		else if (S_sd_stat == SD_CTRL_STAT_GET_SYSMODE) {	/* SD制御状態:システム動作モード取得	*/
			/* 何もしない	*/
		}
		else if (S_sd_stat == SD_CTRL_STAT_WAIT_SHUTDOWN) {	/* SD制御状態:SDモジュール停止待ち	*/
#ifdef _VDV_APP
			if ((S_sd_sys_mode_req_flg == N_OFF) &&							/* システム動作モード取得中ではない	*/
				(S_sd_inspection_info.fio_req_flg == N_OFF) &&				/* SD検査ファイルアクセス中ではない	*/
				(S_sd_rec_write_req_cnt <= S_sd_rec_write_notify_cnt)) {	/* REC中ではない					*/
#else /* _VDV_APP */
			if (S_sd_sys_mode_req_flg == N_OFF) {							/* システム動作モード取得中ではない	*/
#endif /* _VDV_APP */
				/* SDモジュール起動要求	*/
				*event |= SD_CTRL_EVENT_REQ_STARTUP;
			}
		}
#ifdef _VDV_APP
		else if (S_sd_stat == SD_CTRL_STAT_SIMULATION) {	/* SD制御状態:シミュレーション中	*/
			/* 何もしない	*/
		}
#endif /* _VDV_APP */
		else {
			/* 状態不正 */
			OUTLOG(OutputLog("[SdCtrl]Stat ERR!! SysStat:%d SdStat:%d\n", sysStat, S_sd_stat);)
			/* SDモジュールを一度停止して復帰させる	*/
			/* SDモジュール停止要求	*/
			*event |= SD_CTRL_EVENT_REQ_SHUTDOWN;
		}
		break;

#ifdef _VDV_APP
	case SYS_OPERATION_NORMAL_OFF:				/* システム動作状態:通常:OFFモード	----------------*/
		if (S_sd_stat == SD_CTRL_STAT_REC) {				/* SD制御状態:REC中				*/
			/* REC停止要求	*/
			*event |= SD_CTRL_EVENT_REQ_STOP_REC;
		}
		else if (S_sd_stat == SD_CTRL_STAT_IDLE) {			/* SD制御状態:アイドル				*/
			/* 何もしない	*/
		}
		else if (S_sd_stat == SD_CTRL_STAT_WAIT_REC_END) {	/* SD制御状態:REC終了待ち			*/
			if (S_sd_rec_write_req_cnt <= S_sd_rec_write_notify_cnt) {	/* REC完了		*/
				/* REC停止完了	*/
				*event |= SD_CTRL_EVENT_NOTIFY_STOP_REC;
			}
		}
		else if ((S_sd_stat == SD_CTRL_STAT_INIT) ||		/* SD制御状態:初期状態				*/
				(S_sd_stat == SD_CTRL_STAT_SHUTDOWN)) {		/* SD制御状態:SDモジュール停止		*/
			/* 状態不正 */
			OUTLOG(OutputLog("[SdCtrl]Stat ERR!! SysStat:%d SdStat:%d\n", sysStat, S_sd_stat);)
			/* SDモジュールを起動する	*/
			/* SDモジュール起動要求	*/
			*event |= SD_CTRL_EVENT_REQ_STARTUP;
		}
		else if (S_sd_stat == SD_CTRL_STAT_WAIT_SHUTDOWN) {	/* SD制御状態:SDモジュール停止待ち	*/
			/* 状態不正 */
			OUTLOG(OutputLog("[SdCtrl]Stat ERR!! SysStat:%d SdStat:%d\n", sysStat, S_sd_stat);)
			if ((S_sd_sys_mode_req_flg == N_OFF) &&							/* システム動作モード取得中ではない	*/
				(S_sd_rec_write_req_cnt <= S_sd_rec_write_notify_cnt)) {	/* REC中ではない					*/
				/* SDモジュール起動要求	*/
				*event |= SD_CTRL_EVENT_REQ_STARTUP;
			}
			else {
				/* システム動作モード取得 または REC完了まで待つためここでは何もしない */
			}
		}
		else {
			/* 状態不正 */
			OUTLOG(OutputLog("[SdCtrl]Stat ERR!! SysStat:%d SdStat:%d\n", sysStat, S_sd_stat);)
			/* SDモジュールを一度停止して復帰させる	*/
			/* SDモジュール停止要求	*/
			*event |= SD_CTRL_EVENT_REQ_SHUTDOWN;
		}
		break;

	case SYS_OPERATION_NORMAL_AUTO:				/* システム動作状態:通常:オートREC	----------------*/
	case SYS_OPERATION_NORMAL_MANUAL:			/* システム動作状態:通常:マニュアルREC	------------*/
		if (S_sd_stat == SD_CTRL_STAT_IDLE) {				/* SD制御状態:アイドル				*/
			if (sd_ctrl_CheckWriteStartCnd() == N_TRUE) {	/* SD書き込み開始条件に一致した	*/
				/* REC開始要求	*/
				*event |= SD_CTRL_EVENT_REQ_START_REC;
			}
		}
		else if (S_sd_stat == SD_CTRL_STAT_REC) {			/* SD制御状態:REC中				*/
			if (sd_ctrl_CheckWriteEndCnd() == N_TRUE) {		/* SD書き込み停止条件に一致した	*/
				/* REC停止要求	*/
				*event |= SD_CTRL_EVENT_REQ_STOP_REC;
			}
			else {
				if (processType == SD_PROCESS_SUB) {
					/* RECデータ書き込み要求	*/
					*event |= SD_CTRL_EVENT_REQ_WRITE_REC_DATA;
				}
			}
		}
		else if (S_sd_stat == SD_CTRL_STAT_WAIT_REC_END) {	/* SD制御状態:REC終了待ち			*/
			if (S_sd_rec_write_req_cnt <= S_sd_rec_write_notify_cnt) {	/* REC完了		*/
				/* REC停止完了	*/
				*event |= SD_CTRL_EVENT_NOTIFY_STOP_REC;
			}
		}
		else if ((S_sd_stat == SD_CTRL_STAT_INIT) ||		/* SD制御状態:初期状態				*/
				(S_sd_stat == SD_CTRL_STAT_SHUTDOWN)) {		/* SD制御状態:SDモジュール停止		*/
			/* 状態不正 */
			OUTLOG(OutputLog("[SdCtrl]Stat ERR!! SysStat:%d SdStat:%d\n", sysStat, S_sd_stat);)
			/* SDモジュールを起動する	*/
			/* SDモジュール起動要求	*/
			*event |= SD_CTRL_EVENT_REQ_STARTUP;
		}
		else if (S_sd_stat == SD_CTRL_STAT_WAIT_SHUTDOWN) {	/* SD制御状態:SDモジュール停止待ち	*/
			/* 状態不正 */
			OUTLOG(OutputLog("[SdCtrl]Stat ERR!! SysStat:%d SdStat:%d\n", sysStat, S_sd_stat);)
			/* SDモジュールを起動する	*/
			if ((S_sd_sys_mode_req_flg == N_OFF) &&							/* システム動作モード取得中ではない	*/
				(S_sd_rec_write_req_cnt <= S_sd_rec_write_notify_cnt)) {	/* REC中ではない					*/
				/* SDモジュール起動要求	*/
				*event |= SD_CTRL_EVENT_REQ_STARTUP;
			}
		}
		else {
			/* 状態不正 */
			OUTLOG(OutputLog("[SdCtrl]Stat ERR!! SysStat:%d SdStat:%d\n", sysStat, S_sd_stat);)
			/* SDモジュールを一度停止して復帰させる	*/
			/* SDモジュール停止要求	*/
			*event |= SD_CTRL_EVENT_REQ_SHUTDOWN;
		}
		break;
#endif /* _VDV_APP */

	case SYS_OPERATION_ERROR:					/* システム動作状態:異常	------------------------*/
#ifdef _VDV_APP
		if (S_sd_stat == SD_CTRL_STAT_REC) {				/* SD制御状態:REC中				*/
			/* REC停止要求	*/
			*event |= SD_CTRL_EVENT_REQ_STOP_REC;
		}
#endif /* _VDV_APP */
		break;

#ifdef _VDV_APP
	case SYS_OPERATION_SIMULATION:				/* システム動作状態:シミュレーション	------------*/
		if (S_sd_stat == SD_CTRL_STAT_IDLE) {				/* SD制御状態:アイドル				*/
			/* シミュレーション開始要求	*/
			*event |= SD_CTRL_EVENT_REQ_START_SIM;
		}
		break;

	case SYS_OPERATION_INSPECTION:				/* システム動作状態:検査				------------*/
		if (S_sd_stat == SD_CTRL_STAT_IDLE) {				/* SD制御状態:アイドル				*/
			if (S_sd_inspection_info.insp_req_flg == N_ON) {
				/* SD検査要求	*/
				*event |= SD_CTRL_EVENT_REQ_INSPECTION;
			}
		}
		else if (S_sd_stat == SD_CTRL_STAT_INSPECTION) {	/* SD制御状態:SD検査				*/
			if (S_sd_inspection_info.step == SD_INSPECTION_STEP_INVALID) {
				/* SD検査開始要求	*/
				*event |= SD_CTRL_EVENT_REQ_INSPECTION_START;
			}
		}
		else {
			/* nop */
		}
		break;

	case SYS_OPERATION_EEPROM_WRITE:			/* システム動作状態:EEPROM書換中	----------------*/
		/* 何もしない */
		break;

	case SYS_OPERATION_EEPROM_WRITE_COMP:		/* システム動作状態:EEPROM書換完了	----------------*/
		/* 何もしない */
		break;
#endif /* _VDV_APP */

	case SYS_OPERATION_FIRMWARE_WRITE:			/* システム動作状態:ファームウェア書換中	--------*/
		/* 何もしない */
		break;

	case SYS_OPERATION_FIRMWARE_WRITE_COMP:		/* システム動作状態:ファームウェア書換完了	--------*/
		/* 何もしない */
		break;

	case SYS_OPERATION_PRE_ACC_OFF:				/* システム動作状態:ACC OFF移行	--------*/
		if ((S_sd_stat == SD_CTRL_STAT_IDLE) ||				/* SD制御状態:アイドル					*/
			(S_sd_stat == SD_CTRL_STAT_GET_SYSMODE) ||		/* SD制御状態:システム動作モード取得	*/
			(S_sd_stat == SD_CTRL_STAT_REC) ||				/* SD制御状態:REC中					*/
			(S_sd_stat == SD_CTRL_STAT_WAIT_REC_END) ||		/* SD制御状態:REC終了待ち				*/
			(S_sd_stat == SD_CTRL_STAT_INSPECTION) ||		/* SD制御状態:SD検査					*/
			(S_sd_stat == SD_CTRL_STAT_SIMULATION)) {		/* SD制御状態:シミュレーション中		*/
			/* SDモジュール停止要求	*/
			*event |= SD_CTRL_EVENT_REQ_SHUTDOWN;
		}
		else if (S_sd_stat == SD_CTRL_STAT_WAIT_SHUTDOWN) {	/* SD制御状態:SDモジュール停止待ち		*/
#ifdef _VDV_APP
			if ((S_sd_sys_mode_req_flg == N_OFF) &&							/* システム動作モード取得中ではない	*/
				(S_sd_inspection_info.fio_req_flg == N_OFF) &&				/* SD検査ファイルアクセス中ではない	*/
				(S_sd_rec_write_req_cnt <= S_sd_rec_write_notify_cnt)) {	/* REC中ではない					*/
#else /* _VDV_APP */
			if (S_sd_sys_mode_req_flg == N_OFF) {							/* システム動作モード取得中ではない	*/
#endif /* _VDV_APP */
				/* SDモジュール停止完了	*/
				*event |= SD_CTRL_EVENT_NOTIFY_SHUTDOWN;
			}
		}
		else {
			/* nop */
		}
		break;

	case SYS_OPERATION_ACC_OFF:					/* システム動作状態:ACC OFF	--------*/
		/* 何もしない */
		break;
		
	default:
		/* 何もしない */
		break;
	}

	if (S_sd_fio_notify_get_mode == N_ON) {	/* 動作モード取得完了通知あり */
		/* システム動作モード取得完了通知 */
		*event |= SD_CTRL_EVENT_NOTIFY_GET_SYSMODE;
		S_sd_fio_notify_get_mode = N_OFF;
	}
	if (S_sd_fio_notify_write == N_ON) {	/* RECデータ書き込み完了通知あり */
		/* RECデータ書き込み結果通知	*/
		*event |= SD_CTRL_EVENT_NOTIFY_WRITE_RESULT;
		S_sd_fio_notify_write = N_OFF;
	}
	if (S_sd_fio_notify_inspection == N_ON) {	/* SD検査用ファイルアクセス完了通知あり */
		/* SD検査用アクセス結果通知	*/
		*event |= SD_CTRL_EVENT_NOTIFY_INSPECTION_SD_RES;
		S_sd_fio_notify_inspection = N_OFF;
	}

	return;
}

/****************************************************************************/
/* 関数名称		:	btc_state_ProcOnEnter									*/
/* 機能の説明	:	状態に入るときの処理									*/
/* 引数			:	これから入る状態										*/
/* 戻り値		:	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_ctrl_state_ProcOnEnter( E_SD_CTRL_STAT stat )
{

	switch( stat ) {
	case SD_CTRL_STAT_IDLE:					/*	アイドル				*/
		break;

	case SD_CTRL_STAT_GET_SYSMODE:			/*	システム動作モード取得	*/
		break;

	case SD_CTRL_STAT_REC:					/*	REC中					*/
#ifdef _VDV_APP
		/* REC関連変数初期化 */
		sd_ctrl_InitRecParam();
#endif /* _VDV_APP */
		break;

	case SD_CTRL_STAT_WAIT_REC_END:			/*	REC終了待ち				*/
		break;

	case SD_CTRL_STAT_SIMULATION:			/*	シミュレーション中		*/
#ifdef _VDV_APP
		/* シミュレーション関連変数初期化 */
		sd_ctrl_InitSimParam();
#endif /* _VDV_APP */
		break;

	case SD_CTRL_STAT_WAIT_SHUTDOWN:		/*	SDモジュール停止待ち	*/
		break;

	case SD_CTRL_STAT_SHUTDOWN:				/*	SDモジュール停止		*/
		break;
	
	default:
		break;
	}

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_state_ProcOnLeave								*/
/* 機能の説明	:	状態から出るときの処理									*/
/* 引数			:	これから出る状態										*/
/* 戻り値		:	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/02												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_ctrl_state_ProcOnLeave( E_SD_CTRL_STAT stat )
{

	switch( stat ) {
	case SD_CTRL_STAT_IDLE:					/*	アイドル				*/
		break;

	case SD_CTRL_STAT_GET_SYSMODE:			/*	システム動作モード取得	*/
		break;

	case SD_CTRL_STAT_REC:					/*	REC中					*/
#ifdef _VDV_APP
		/* Rec終了処理	*/
		sd_ctrl_EndRec();
#endif /* _VDV_APP */
		break;

	case SD_CTRL_STAT_WAIT_REC_END:			/*	REC終了待ち				*/
		break;

	case SD_CTRL_STAT_SIMULATION:			/*	シミュレーション中		*/
		break;

	case SD_CTRL_STAT_WAIT_SHUTDOWN:		/*	SDモジュール停止待ち	*/
		/* パラメタ初期化	*/
		sd_ctrl_InitModuleParam();
		break;

	case SD_CTRL_STAT_SHUTDOWN:				/*	SDモジュール停止		*/
		/* パラメタ初期化	*/
		sd_ctrl_InitModuleParam();
		break;
	
	default:
		break;
	}

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_stat_ProcInit									*/
/* 機能の説明	:	状態関数:SD_CTRL_STAT_INIT								*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	SD_CTRL_STAT_xxx	遷移先状態							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcInit( SD_CTRL_EVENT event )
{

	E_SD_CTRL_STAT nextStat = SD_CTRL_STAT_INIT;

	if ((event & SD_CTRL_EVENT_REQ_STARTUP) != 0) {			/* SDモジュール起動要求				*/
		/* 遷移先状態:アイドル	*/
		nextStat = SD_CTRL_STAT_IDLE;
	}

	return nextStat;

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_stat_ProcIdle									*/
/* 機能の説明	:	状態関数:SD_CTRL_STAT_IDLE								*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	SD_CTRL_STAT_xxx	遷移先状態							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcIdle( SD_CTRL_EVENT event )
{

	E_SD_CTRL_STAT	nextStat = SD_CTRL_STAT_IDLE;
	E_FIO_RESULT	fioRet;
	UI_32			reqNo;

	if ((event & SD_CTRL_EVENT_REQ_SHUTDOWN) != 0) {			/* SDモジュール停止要求				*/
		/* 遷移先状態:SDモジュール停止	*/
		nextStat = SD_CTRL_STAT_WAIT_SHUTDOWN;
	}
	else if ((event & SD_CTRL_EVENT_REQ_GET_SYSMODE) != 0) {	/* システム動作モード取得要求		*/

		S_sd_fio_notify_get_mode = N_OFF;		/* システム動作モード取得完了通知フラグクリア	*/

		/* システム動作モードファイル読み込み */
		fioRet = FileIO_Read( (const UI_8*)SD_CTRL_SYS_MODE_FILE_PATH, S_sd_sys_mode_data, 0, SD_CTRL_SYS_MODE_FILE_SIZE, sd_ctrl_SysModeCallback, &reqNo );
		if (fioRet != FIO_RESULT_OK) {								/* モードファイル読み込み失敗	*/

			/* モード:通常 */
			S_sd_sys_mode = SD_CTRL_SYS_MODE_NOMAL;

			/* システム動作モード取得完了通知ありを設定 */
			S_sd_fio_notify_get_mode = N_ON;

			/* システム動作モード取得要求フラグOFF	*/
			S_sd_sys_mode_req_flg = N_OFF;
		}
		else {
			/* システム動作モード取得要求フラグON	*/
			S_sd_sys_mode_req_flg = N_ON;
		}

		/* 遷移先状態:システム動作モード取得	*/
		nextStat = SD_CTRL_STAT_GET_SYSMODE;
	}
	else if ((event & SD_CTRL_EVENT_REQ_START_REC) != 0) {	/* REC開始要求						*/
		/* 遷移先状態:REC中	*/
		nextStat = SD_CTRL_STAT_REC;
	}
	else if ((event & SD_CTRL_EVENT_REQ_START_SIM) != 0) {	/* シミュレーション開始要求			*/
		/* 遷移先状態:シミュレーション中	*/
		nextStat = SD_CTRL_STAT_SIMULATION;
	}
	else if ((event & SD_CTRL_EVENT_REQ_INSPECTION) != 0) {	/* SD検査							*/
		/* 遷移先状態:SD検査	*/
		nextStat = SD_CTRL_STAT_INSPECTION;
	}
	else {
		/* nop */
	}

	return nextStat;

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_stat_ProcGetSysMode								*/
/* 機能の説明	:	状態関数:SD_CTRL_STAT_GET_SYSMODE						*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	SD_CTRL_STAT_xxx	遷移先状態							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcGetSysMode( SD_CTRL_EVENT event )
{

	E_SD_CTRL_STAT			nextStat = SD_CTRL_STAT_GET_SYSMODE;
	E_SYS_CTRL_EVENT_PARAM	eventParam;

	if ((event & SD_CTRL_EVENT_REQ_SHUTDOWN) != 0) {			/* SDモジュール停止要求				*/
		/* 遷移先状態:SDモジュール停止	*/
		nextStat = SD_CTRL_STAT_WAIT_SHUTDOWN;
	}
	else if ((event & SD_CTRL_EVENT_NOTIFY_GET_SYSMODE) != 0) {		/* システム動作モード取得完了通知			*/
		/* システム制御に動作モードを通知する */
		switch (S_sd_sys_mode) {
		case SD_CTRL_SYS_MODE_NOMAL:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_NORMAL;			/* 動作モード:通常					*/
			break;
		case SD_CTRL_SYS_MODE_SIMULATION:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_SIMULATION;		/* 動作モード:シミュレーション		*/
			break;
		case SD_CTRL_SYS_MODE_EEPROM:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_EEPROM;			/* 動作モード:EEPROM書換			*/
			break;
		case SD_CTRL_SYS_MODE_FIRMWARE:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_FIRMWARE;			/* 動作モード:ファームウェア書換	*/
			break;
		case SD_CTRL_SYS_MODE_INSPECTION:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_INSPECTION;		/* 動作モード:検査					*/
			break;
		case SD_CTRL_SYS_MODE_BT_COMM:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_BT_COMM;			/* 動作モード:BTモジュール通信		*/
			break;
		case SD_CTRL_SYS_MODE_GNSS_COMM:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_GNSS_COM;			/* 動作モード:GNSSモジュール通信	*/
			break;
		default:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_NORMAL;			/* 動作モード:通常					*/
			OUTLOG(OutputLog("[SdCtrl]SysMode ERR!!! mode:%d\n", S_sd_sys_mode);)
			break;
		}
		(void)System_control_NotifyEvent( SYS_EVENT_DECISION_ACTION_MODE, eventParam );

		/* 遷移先状態:アイドル	*/
		nextStat = SD_CTRL_STAT_IDLE;
	}
	else {
		/* nop */
	}

	return nextStat;

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_stat_ProcRec									*/
/* 機能の説明	:	状態関数:SD_CTRL_STAT_REC								*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	SD_CTRL_STAT_xxx	遷移先状態							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcRec( SD_CTRL_EVENT event )
{

#ifdef _VDV_APP
	E_SD_CTRL_STAT		nextStat = SD_CTRL_STAT_REC;
	E_FIO_RESULT		fioRet;
	UI_32				reqNo;
	UI_32				time1ms;
	UI_16				headSize;
	UI_16				dataSize;
	UI_8				i;

	if ((event & SD_CTRL_EVENT_REQ_SHUTDOWN) != 0) {		/* SDモジュール停止要求				*/
		/* 遷移先状態:SDモジュール停止待ち	*/
		nextStat = SD_CTRL_STAT_WAIT_SHUTDOWN;
	}
	else if ((event & SD_CTRL_EVENT_REQ_STOP_REC) != 0) {	/* REC停止要求						*/
		/* 遷移先状態:REC終了待ち	*/
		nextStat = SD_CTRL_STAT_WAIT_REC_END;
	}
	else {
		if ((event & SD_CTRL_EVENT_NOTIFY_WRITE_RESULT) != 0) {		/* RECデータ書き込み結果通知					*/
			/* 書き込み結果を取得	*/
			for (i = 0; i < SD_CTRL_FIO_WRITE_NOTFY_NUM; i++) {
				if (RingBuffer_GetCanReadSize( &S_sd_rec_write_notify_mng ) < sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO)) {
					break;
				}
				(void)RingBuffer_GetArrayData( &S_sd_rec_write_notify_mng, (UI_8*)&S_sd_notifyinfo_get_buff, sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO), RINGBUF_READEND );
				if (S_sd_notifyinfo_get_buff.result == FIO_RESULT_OK) {										/* 書き込み正常									*/
					if ( S_sd_notifyinfo_get_buff.bytes > S_sd_notifyinfo_get_buff.bytesWritten ) {			/* 要求サイズ分書き込めていない（SD空きなし）	*/
						/* 遷移先状態:REC終了待ち	*/
						nextStat = SD_CTRL_STAT_WAIT_REC_END;
					}
				}
				else {											/* 書き込みエラー発生							*/
					/* 遷移先状態:REC終了待ち	*/
					nextStat = SD_CTRL_STAT_WAIT_REC_END;
				}
			}
		}

		if (((event & SD_CTRL_EVENT_REQ_WRITE_REC_DATA) != 0) &&		/* RECデータ書き込み要求						*/
			(nextStat == SD_CTRL_STAT_REC)) {							/* 遷移先状態:REC状態							*/

			if ( S_sd_rec_write_req_cnt == 0 ) {						/* 初回書き込み		*/
				/* REC開始時間を保持	*/
				S_sd_write_start_time = Timer_ctrl_GetTime1ms();

				/* RECファイルパス作成	*/
				sd_ctrl_MakeRecFilepath();

				/* ログ情報を初期化する */
				sd_ctrl_UpdateLogDataInfo( N_TRUE );

				/* RECファイルのヘッダ情報作成	*/
				sd_ctrl_MakeRecHead( S_sd_rec_head, sizeof(S_sd_rec_head), &headSize, N_TRUE );

				/* RECファイルのヘッダ情報を書き込む	*/
				fioRet = FileIO_Write( FIO_WRITE_TYPE_FILE_CREATE, S_sd_rec_filepath, S_sd_rec_head, 0, (UI_32)headSize, sd_ctrl_RecDataWriteCallback, &reqNo );
				if (fioRet == FIO_RESULT_OK) {
					S_sd_rec_write_req_cnt++;
					S_sd_update_header_time = Timer_ctrl_GetTime1ms();
				}
				else {											/* 書き込みエラー発生							*/
					/* 遷移先状態:REC終了待ち	*/
					nextStat = SD_CTRL_STAT_WAIT_REC_END;
				}
			}

			if (nextStat == SD_CTRL_STAT_REC) {
				time1ms = Timer_ctrl_GetTime1ms();
				if ((time1ms - S_sd_write_start_time) > SD_CTRL_REC_TIME_MAX) {	/* REC最大時間オーバー	*/
					/* 遷移先状態:REC終了待ち	*/
					nextStat = SD_CTRL_STAT_WAIT_REC_END;
				}
				else {
					/* ログ情報を更新する */
					sd_ctrl_UpdateLogDataInfo( N_FALSE );
					/* 定期ヘッダ更新処理(内部で時間判定あり) */
					sd_ctrl_UpdateHeader();

					/* RECデータ作成	*/
					sd_ctrl_MakeRecData( S_sd_rec_data, sizeof(S_sd_rec_data), &dataSize );

					/* RECデータを書き込む	*/
					sd_ctrl_WriteRecData( S_sd_rec_data, dataSize );
				}
			}
		}
	}

	return nextStat;
#else /* _VDV_APP */
	return SD_CTRL_STAT_IDLE;
#endif /* _VDV_APP */

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_stat_ProcWaitRecEnd								*/
/* 機能の説明	:	状態関数:SD_CTRL_STAT_WAIT_REC_END						*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	SD_CTRL_STAT_xxx	遷移先状態							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcWaitRecEnd( SD_CTRL_EVENT event )
{

#ifdef _VDV_APP
	E_SD_CTRL_STAT nextStat = SD_CTRL_STAT_WAIT_REC_END;
	UI_8	i;

	if ((event & SD_CTRL_EVENT_NOTIFY_WRITE_RESULT) != 0) {	/* RECデータ書き込み結果通知			*/
		for (i = 0; i < SD_CTRL_FIO_WRITE_NOTFY_NUM; i++) {
			/* RECデータ書き込み結果通知情報バッファを読み進める	*/
			if (RingBuffer_GetCanReadSize( &S_sd_rec_write_notify_mng ) < sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO)) {
				break;
			}
			(void)ReadEndRingBuffer( &S_sd_rec_write_notify_mng, sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO) );
		}
	}

	if ((event & SD_CTRL_EVENT_REQ_SHUTDOWN) != 0) {			/* SDモジュール停止要求				*/
		/* 遷移先状態:SDモジュール停止	*/
		nextStat = SD_CTRL_STAT_WAIT_SHUTDOWN;
	}
	else if ((event & SD_CTRL_EVENT_NOTIFY_STOP_REC) != 0) {	/* REC停止完了						*/
		/* 遷移先状態:アイドル	*/
		nextStat = SD_CTRL_STAT_IDLE;
	}
	else {
		/* nop */
	}

	return nextStat;
#else /* _VDV_APP */
	return SD_CTRL_STAT_IDLE;
#endif /* _VDV_APP */

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_stat_ProcSimulation								*/
/* 機能の説明	:	状態関数:SD_CTRL_STAT_SIMULATION						*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	SD_CTRL_STAT_xxx	遷移先状態							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcSimulation( SD_CTRL_EVENT event )
{

#ifdef _VDV_APP
	E_SD_CTRL_STAT nextStat = SD_CTRL_STAT_SIMULATION;

	/* サブ状態遷移のメイン処理を呼び出す */
	sd_sim_state_MainProc();

	if( SD_SIM_STAT_ACC_OFF == S_sd_sim_status ) {
		/* ACC OFF による停止を確認してから遷移 */
		nextStat = SD_CTRL_STAT_WAIT_SHUTDOWN;
	}
	return nextStat;
#else /* _VDV_APP */
	return SD_CTRL_STAT_IDLE;
#endif /* _VDV_APP */

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_stat_ProcInspection								*/
/* 機能の説明	:	状態関数:SD_CTRL_STAT_INSPECTION						*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	SD_CTRL_STAT_xxx	遷移先状態							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcInspection( SD_CTRL_EVENT event )
{

#ifdef _VDV_APP
	E_SD_CTRL_STAT	nextStat = SD_CTRL_STAT_INSPECTION;

	if ((event & SD_CTRL_EVENT_REQ_SHUTDOWN) != 0) {					/* SDモジュール停止要求				*/
		/* 遷移先状態:SDモジュール停止待ち	*/
		nextStat = SD_CTRL_STAT_WAIT_SHUTDOWN;
	}
	else if (((event & SD_CTRL_EVENT_REQ_INSPECTION_START) != 0) ||		/* SD検査開始						*/
			 ((event & SD_CTRL_EVENT_NOTIFY_INSPECTION_SD_RES) != 0)) {	/* SD検査用アクセス結果通知			*/
		/* SD検査	*/
		if (sd_ctrl_Inspect() == N_OFF) {
			/* 遷移先状態:アイドル	*/
			nextStat = SD_CTRL_STAT_IDLE;
		}
	}
	else {
		/* nop */
	}

	return nextStat;

#else /* _VDV_APP */
	return SD_CTRL_STAT_IDLE;
#endif /* _VDV_APP */

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_stat_ProcWaitShutdown							*/
/* 機能の説明	:	状態関数:SD_CTRL_STAT_WAIT_SHUTDOWN						*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	SD_CTRL_STAT_xxx	遷移先状態							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcWaitShutdown( SD_CTRL_EVENT event )
{

#ifdef _VDV_APP
	E_SD_CTRL_STAT nextStat = SD_CTRL_STAT_WAIT_SHUTDOWN;
	UI_8	i;

	if ((event & SD_CTRL_EVENT_NOTIFY_WRITE_RESULT) != 0) {	/* RECデータ書き込み結果通知			*/
		/* RECデータ書き込み結果通知情報バッファを読み進める	*/
		for (i = 0; i < SD_CTRL_FIO_WRITE_NOTFY_NUM; i++) {
			if (RingBuffer_GetCanReadSize( &S_sd_rec_write_notify_mng ) < sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO)) {
				break;
			}
			(void)ReadEndRingBuffer( &S_sd_rec_write_notify_mng, sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO) );
		}
	}

	if ((event & SD_CTRL_EVENT_NOTIFY_INSPECTION_SD_RES) != 0) {	/* SD検査用アクセス結果通知		*/
		S_sd_inspection_info.fio_req_flg = N_OFF;
	}

	if ((event & SD_CTRL_EVENT_NOTIFY_SHUTDOWN) != 0) {		/* SDモジュール停止完了					*/
		/* システム状態制御にSD終了処理完了を通知	*/
		(void)System_control_NotifyEvent( SYS_EVENT_END_PROC_COMP_SD, SYS_EVENT_PARAM_NO );

		/* 遷移先状態:SDモジュール停止	*/
		nextStat = SD_CTRL_STAT_SHUTDOWN;
	}
	else if ((event & SD_CTRL_EVENT_REQ_STARTUP) != 0) {	/* SDモジュール起動要求					*/
		/* 遷移先状態:アイドル	*/
		nextStat = SD_CTRL_STAT_IDLE;
	}
	else {
		/* nop */
	}

	return nextStat;
#else /* _VDV_APP */
	/* システム状態制御にSD終了処理完了を通知	*/
	(void)System_control_NotifyEvent( SYS_EVENT_END_PROC_COMP_SD, SYS_EVENT_PARAM_NO );

	return SD_CTRL_STAT_SHUTDOWN;
#endif /* _VDV_APP */

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_stat_ProcShutdown								*/
/* 機能の説明	:	状態関数:SD_CTRL_STAT_SHUTDOWN							*/
/* 引数			:	(I/ )event	イベント									*/
/* 戻り値		: 	SD_CTRL_STAT_xxx	遷移先状態							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcShutdown( SD_CTRL_EVENT event )
{

	E_SD_CTRL_STAT nextStat = SD_CTRL_STAT_SHUTDOWN;

	if ((event & SD_CTRL_EVENT_REQ_STARTUP) != 0) {			/* SDモジュール起動要求				*/
		/* 遷移先状態:アイドル	*/
		nextStat = SD_CTRL_STAT_IDLE;
	}

	return nextStat;

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_InitModuleParam									*/
/* 機能の説明	:	モジュールで使用する変数初期化							*/
/* 引数			:	なし													*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_ctrl_InitModuleParam( void )
{

	S_sd_sys_mode = SD_CTRL_SYS_MODE_UNKNOWN;
	S_sd_fio_notify_get_mode = N_OFF;
	S_sd_fio_notify_write = N_OFF;
	S_sd_fio_notify_inspection = N_OFF;
	S_sd_sys_mode_req_flg = N_OFF;
	(void)memset(S_sd_sys_mode_data, 0, sizeof(S_sd_sys_mode_data));

#ifdef _VDV_APP
	S_processType = SD_PROCESS_MAIN;
	S_sd_recBtnOnOffCnt = 0;
	sd_ctrl_InitRecParam();
	sd_ctrl_InitSimParam();
	sd_ctrl_InitInspectionInfo();

	sd_ctrl_UpdateLogDataInfo( N_TRUE );
#endif /* _VDV_APP */

	return;

}

#ifdef _VDV_APP
/****************************************************************************/
/* 関数名称		:	sd_ctrl_InitRecParam									*/
/* 引数			:	REC関連変数初期化										*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_ctrl_InitRecParam( void )
{

	/* RECデータバッファ初期化	*/
	(void)memset(S_sd_rec_head, 0, sizeof(S_sd_rec_head));
	(void)memset(S_sd_rec_data, 0, sizeof(S_sd_rec_data));
	(void)memset(S_sd_rec_data_buff, 0, sizeof(S_sd_rec_data_buff));
	RingBuffer_Init( &S_sd_rec_data_mng, S_sd_rec_data_buff, (UI_16)sizeof(S_sd_rec_data_buff) );
	S_sd_ring_write_size = 0;
	S_sd_ring_flush_size = 0;

	/* RECデータ書き込み回数初期化	*/
	S_sd_rec_write_req_cnt = 0;
	S_sd_rec_write_notify_cnt = 0;

	/* RECデータ書き込み結果通知情報バッファ初期化	*/
	(void)memset(S_sd_rec_write_notify_buff, 0, sizeof(S_sd_rec_write_notify_buff));
	RingBuffer_Init( &S_sd_rec_write_notify_mng, (UI_8*)S_sd_rec_write_notify_buff, (UI_16)sizeof(S_sd_rec_write_notify_buff) );
	(void)memset(&S_sd_notifyinfo_set_buff, 0, sizeof(S_sd_notifyinfo_set_buff));
	(void)memset(&S_sd_notifyinfo_get_buff, 0, sizeof(S_sd_notifyinfo_get_buff));

	/* RECデータ書き込み管理情報初期化	*/
	S_sd_write_start_time = 0;
	S_sd_update_header_time = 0;

	/* RECデータファイルパス初期化	*/
	(void)memset(S_sd_rec_filepath, 0, sizeof(S_sd_rec_filepath));

	return ;

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_InitSimParam									*/
/* 引数			:	シミュレーション関連変数初期化							*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_ctrl_InitSimParam( void )
{

	sd_sim_InitSimCtrlInfo( SD_SIM_INIT_MODE_ENTER_SIM );

	return ;

}
#endif /* _VDV_APP */

/****************************************************************************/
/* 関数名称		:	sd_ctrl_SysModeCallback									*/
/* 機能の説明	:	システム動作モード取得コールバック						*/
/* 引数			:	(I/ )reqinfo	要求情報								*/
/* 				:	(I/ )retinfo	結果									*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_ctrl_SysModeCallback( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo )
{

	/* モードを設定 */
	S_sd_sys_mode = SD_CTRL_SYS_MODE_NOMAL;			/* モード:通常			*/
	if (retinfo->result == FIO_RESULT_OK) {		/* システム動作モードファイルを読めた		*/
		if (S_sd_sys_mode_data[0] == SD_CTRL_SYS_MODE_FILE_VALUE_SIMU) {
			S_sd_sys_mode = SD_CTRL_SYS_MODE_SIMULATION;	/* モード:シミュレーション			*/
		}
		else if (S_sd_sys_mode_data[0] == SD_CTRL_SYS_MODE_FILE_VALUE_EEPROM) {
			S_sd_sys_mode = SD_CTRL_SYS_MODE_EEPROM;		/* モード:EEPROM書き換え			*/
		}
		else if (S_sd_sys_mode_data[0] == SD_CTRL_SYS_MODE_FILE_VALUE_FIRM) {
			S_sd_sys_mode = SD_CTRL_SYS_MODE_FIRMWARE;		/* モード:ファームウェア書き換え	*/
		}
		else if (S_sd_sys_mode_data[0] == SD_CTRL_SYS_MODE_FILE_VALUE_INSPECTION) {
			S_sd_sys_mode = SD_CTRL_SYS_MODE_INSPECTION;	/* モード:検査						*/
		}
#ifdef ADVANCE_ENABLE
		else if (S_sd_sys_mode_data[0] == SD_CTRL_SYS_MODE_FILE_VALUE_BT_COMM) {
			S_sd_sys_mode = SD_CTRL_SYS_MODE_BT_COMM;		/* モード:BTモジュール通信			*/
		}
		else if (S_sd_sys_mode_data[0] == SD_CTRL_SYS_MODE_FILE_VALUE_GNSS_COMM) {
			S_sd_sys_mode = SD_CTRL_SYS_MODE_GNSS_COMM;		/* モード:GNSSモジュール通信		*/
		}
#endif /* ADVANCE_ENABLE */
		else {
			/* nop */
		}
	}

	/* システム動作モード取得通知フラグをON */
	S_sd_fio_notify_get_mode = N_ON;

	/* システム動作モード取得要求フラグOFF	*/
	S_sd_sys_mode_req_flg = N_OFF;

	return;

}

#ifdef _VDV_APP
/****************************************************************************/
/* 関数名称		:	sd_ctrl_RecDataWriteCallback							*/
/* 機能の説明	:	RECデータ書き込みコールバック							*/
/* 引数			:	(I/ )reqinfo	要求情報								*/
/* 				:	(I/ )retinfo	結果									*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_ctrl_RecDataWriteCallback( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo )
{

	/* RECデータ書き込み結果通知情報にセットする */
	if ((reqinfo != NULL) && (retinfo != NULL)) {
		if (RingBuffer_GetCanWriteSize( &S_sd_rec_write_notify_mng ) >= sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO) ) {
			S_sd_notifyinfo_set_buff.result = retinfo->result;
			S_sd_notifyinfo_set_buff.bytes = reqinfo->bytes;
			S_sd_notifyinfo_set_buff.bytesWritten = retinfo->bytes;
			(void)RingBuffer_SetArrayData( &S_sd_rec_write_notify_mng, (UI_8*)&S_sd_notifyinfo_set_buff, sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO) );
		}
	}

	if ((reqinfo != NULL) &&
		(S_sd_rec_data_buff <= reqinfo->buff) &&
		(reqinfo->buff <= (S_sd_rec_data_buff + sizeof(S_sd_rec_data_buff) - 1))) {	/* RECリングバッファのデータに対する書き込み完了通知 */
		/* RECリングバッファのリード位置を進める */
		(void)ReadEndRingBuffer( &S_sd_rec_data_mng, (UI_16)reqinfo->bytes );
	}

	/* RECデータ書き込み結果通知回数をインクリメント	*/
	S_sd_rec_write_notify_cnt++;

	/* RECデータ書き込み結果通知フラグをON */
	S_sd_fio_notify_write = N_ON;

	return;

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_CheckWriteStartCnd								*/
/* 機能の説明	:	SD書き込み開始条件に一致するかチェックする				*/
/* 引数			:	なし													*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static B_8 sd_ctrl_CheckWriteStartCnd( void )
{

	E_SYS_CTRL_STATUS		sysStat;
	B_8						btnOn;
	B_8						speedOn;
	E_FIO_RESULT			fioRet;
	UI_32					freeSectNum;
	E_SW_REC_BTN_STATUS		recBtnStat;
	UI_8					btnOnOffCnt;
	T_Can_ctrl_tcan_info	*canInfo;
	UI_16					speed;
	UI_8					portLev;

	sysStat = System_control_GetStatus();

	/* システム動作状態チェック	*/
	if ((sysStat != SYS_OPERATION_NORMAL_AUTO) &&
		(sysStat != SYS_OPERATION_NORMAL_MANUAL)) {		/*「オートREC」、「マニュアルREC」以外	*/
		return N_FALSE;
	}

	/* RECボタン/車速をチェック	*/
	btnOn = N_FALSE;
	speedOn = N_FALSE;
	/* RECボタンが押されたかチェック*/
	recBtnStat = Sw_control_GetRecButtonStatus();
	if ((recBtnStat == SYS_REC_BTN_ON_SHORTPRESS) ||
		(recBtnStat == SYS_REC_BTN_ON_LONGPRESS)) {		/* ボタンが押された		*/
		btnOnOffCnt = Sw_control_GetRecButtonOnOffChangeCount();
		if (S_sd_recBtnOnOffCnt != btnOnOffCnt) {	/* 前回のRECボタンカウントと異なる	*/
			/* チャタリング吸収のためON/OFF判定期間がメイン定期処理間隔より長い。	*/
			/* 1回のボタン押下を何回も押されたと判断してしまわないためON/OFFの変化も判定する。*/
			/* 保持しているボタンカウントと異なる場合は押されたと判断する	*/
			S_sd_recBtnOnOffCnt = btnOnOffCnt;
			btnOn = N_TRUE;
		}
		else {
			/* 保持しているボタンカウントと同じ場合は押された状態が継続している（＝今回はおされていない）と判断する	*/
			/* 何もしない	*/
		}
	}
	/* オートRECの場合は車速をチェックする	*/
	if (sysStat == SYS_OPERATION_NORMAL_AUTO) {
		canInfo = Can_control_GetTCanInfo();
		if (canInfo != NULL) {
			if( canInfo->en_vehicle_speed_for_disp == 1 ) {	/* CANが車速を受信している	*/
				speed = (UI_16)((F_64)canInfo->vehicle_speed_for_disp * CAN_VEHICLESPEED_DISP_RESOLUTION);
				if(speed >= 1){									/* 車速が1Km/h以上			*/
					speedOn = N_TRUE;
				}
			}
		}
	}
	if ((btnOn == N_FALSE) && (speedOn == N_FALSE)) {
		return N_FALSE;
	}

	/* GNSS動作状態チェック	*/
#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata */
	if (Gnss_control_iSGnssFixedAtOneTime() == N_FALSE) {	/* GNSS1回でも補足済み	*/
#else
	if (Signal_control_isGnssEnable() == N_FALSE) {		/* GNSS未受信			*/
#endif /* _VDV_SUPRA　2018.12.14 S.Sakata */
		OUTLOG(OutputLog("[SdCtrl]RecStartCheck GnssDisable\n");)
		return N_FALSE;
	}

	/* CAN動作状態チェック	*/
	if (Signal_control_isCanEnable() == N_FALSE) {		/* CAN未受信			*/
/*		OUTLOG(OutputLog("[SdCtrl]RecStartCheck CanDisable\n");) */
		return N_FALSE;
	}


	/* SDに空きがあるかチェック	*/
	fioRet = FileIO_Get_FreeSectNum( (const UI_8*)FS_DRV_PATH_SD, &freeSectNum );
	if (fioRet != FIO_RESULT_OK) {						/* SD空き容量を取得できない（SD使用可能状態ではない）	*/
/*		OUTLOG(OutputLog("[SdCtrl]RecStartCheck SdNotReady\n");) */
		return N_FALSE;
	}
	else {
		if (freeSectNum == 0) {							/* SDに空きが無い		*/
/*			OUTLOG(OutputLog("[SdCtrl]RecStartCheck SdNotEmpty\n");) */
			return N_FALSE;
		}
	}

	/* ライトプロテクト状態をチェック */
	if (PortDrv_GetDataBit(IO_PI_GR_SD_WP, IO_PI_BIT_SD_WP, &portLev) == D_OK) {
		if (portLev == IO_PI_LEV_NONACT_SD_WP) {
			/* プロテクト有 */
/*			OUTLOG(OutputLog("[SdCtrl]RecStartCheck SdWriteProtect\n");) */
			return N_FALSE;
		}
	}

	/* 走行距離をクリア	*/
	S_sd_running_distance = 0;
	/* CAN情報取得 */
	canInfo = Can_control_GetTCanInfo();
	if (canInfo != NULL) {
		/* 走行距離を保持 */
		S_sd_running_distance = canInfo->running_distance;
	}
	/* 停車時間に現在時刻をセット */
	S_sd_speed_stop_time = Timer_ctrl_GetTime1ms();

	return N_TRUE;

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_CheckWriteEndCnd								*/
/* 機能の説明	:	SD書き込み停止条件に一致するかチェックする				*/
/* 引数			:	なし													*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static B_8 sd_ctrl_CheckWriteEndCnd( void )
{

	E_SW_REC_BTN_STATUS		recBtnStat;
	UI_8					btnOnOffCnt;
	T_Can_ctrl_tcan_info	*canInfo;

	/* RECボタンが押されたかチェック*/
	recBtnStat = Sw_control_GetRecButtonStatus();
	if ((recBtnStat == SYS_REC_BTN_ON_SHORTPRESS) ||
		(recBtnStat == SYS_REC_BTN_ON_LONGPRESS)) {		/* ボタンが押された		*/
		btnOnOffCnt = Sw_control_GetRecButtonOnOffChangeCount();
		if (S_sd_recBtnOnOffCnt != btnOnOffCnt) {	/* 前回のRECボタンカウントと異なる	*/
			/* チャタリング吸収のためON/OFF判定期間がメイン定期処理間隔より長い。	*/
			/* 1回のボタン押下を何回も押されたと判断してしまわないためON/OFFの変化も判定する。*/
			/* 保持しているボタンカウントと異なる場合は押されたと判断する	*/
			S_sd_recBtnOnOffCnt = btnOnOffCnt;
			return N_TRUE;
		}
		else {
			/* 保持しているボタンカウントと同じ場合は押された状態が継続している（＝今回はおされていない）と判断する	*/
			/* 何もしない	*/
		}
	}

	/* 規定時間以上停車している場合はRECを停止 */
	/* CAN情報取得 */
	canInfo = Can_control_GetTCanInfo();
	if (canInfo != NULL) {
		/* 停車しているかチェック	*/
		if ((SYS_OPERATION_NORMAL_AUTO == System_control_GetStatus()) &&	/* オートモード */
			(canInfo->wheel_speed_ave == 0) &&								/* 車速が0				*/
			(canInfo->running_distance == S_sd_running_distance)) {			/* 走行距離に変化なし	*/
			/* 停車して規定時間経過したかチェック	*/
			if (Timer_ctrl_TimeCompare1ms(S_sd_speed_stop_time, SD_CTRL_REC_STOP_TIMEOUT) == N_OK) {	
				/* 停車して規定時間経過	*/
				return N_TRUE;
			}
		}
		else {																/* 停車していない		*/
			/* 走行距離を更新	*/
			S_sd_running_distance = canInfo->running_distance;
			/* 停車時間を現在時刻で更新 */
			S_sd_speed_stop_time = Timer_ctrl_GetTime1ms();
		}
	}

	return N_FALSE;

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_MakeRecFilepath									*/
/* 機能の説明	:	RECデータファイルパス作成								*/
/* 引数			:	なし													*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/09/26												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_ctrl_MakeRecFilepath( void )
{

	T_COMMON_DATE_TIME	dtm;
	UI_32				pathlen;
	T_Gnss_ctrl_nav_pvt_info *hnrinf;
	hnrinf = Gnss_cntrol_GetNavPvtInfo();

	(void)memset(&dtm, 0, sizeof(dtm));
	if (hnrinf != NULL) {
		dtm.year = hnrinf->year;
		dtm.month = hnrinf->month;
		dtm.day = hnrinf->day;
		dtm.hour = hnrinf->hour;
		dtm.min = hnrinf->min;
		dtm.sec = hnrinf->sec;
	}

	/* RECデータファイルパスにSDドライブのパスを設定 */
	S_sd_rec_filepath[0] = '\0';
	(void)strcpy((CH_8*)S_sd_rec_filepath, FS_DRV_PATH_SD);

	/* RECデータファイルパスに日時を追加 */
	pathlen = strlen((const CH_8*)S_sd_rec_filepath);
	Common_MakeDateTimeStr(&dtm, COMMON_DATE_TIME_STR_TYPE_YEAR_4, &S_sd_rec_filepath[pathlen]);

	/* RECデータファイルパスに拡張子を追加 */
	(void)strcat((CH_8*)S_sd_rec_filepath, SD_CTRL_REC_FILE_EXTENSION);

	return;

}

/****************************************************************************/
/* 関数名称		: sd_ctrl_MakeRecHead										*/
/* 機能の説明	: ファイルヘッダ作成										*/
/* 引数			: ( /O)headBuff	バッファ先頭アドレス						*/
/*				  (I/ )buffSize	バッファサイズ								*/
/*				  ( /O)headSize	作成したヘッダサイズ						*/
/*				  (I/ )init		初期化(N_TRUE:初期化/それ以外:更新)			*/
/* 戻り値		: void														*/
/* 作成者		: (OFT)T.Kamakura											*/
/* 作成日		: 16/10/01													*/
/* 備考			: init により動作が異なる。									*/
/*	初期化モード	暗号キー生成登録を行いファイル先頭の固定部を作成する。	*/
/*	更新モード		ファイルヘッダのみを作成する。							*/
/****************************************************************************/
static void sd_ctrl_MakeRecHead( UI_8 *headBuff, UI_16 buffSize, UI_16 *headSize, B_8 init )
{

	UI_16			fileHeaderPartSize = 0U;
	UI_16			fixIdInfoPartSize = 0U;
	UI_16			fixSignalPartSize = 0U;
	UI_16			varIdInfoPartSize = 0U;
	UI_8*			pBuff = NULL;
	UI_16			restSize = 0U;
	UI_8			fixedSignalNum = 0U;
	UI_16*			fixedSignalIdList = 0U;
	UI_8			vehicleSignalNum = 0U;
	UI_16*			vehicleSignalIdList = 0U;

	if( (NULL != headBuff) && (NULL != headSize) ) {
		/* バッファ先頭から格納 */
		pBuff = headBuff;
		restSize = buffSize;

		if( N_TRUE == init ) {
			/*==============================================================*/
			/* 初期モードではエンコードキー作成して信号制御へ設定			*/
			/*==============================================================*/
			(void)Signal_control_SetEncodeKey( DEVICE_TYPE_SD, Signal_control_GetRandomValue() );
		}
		/* ファイルヘッダ部を作成 */
		sd_ctrl_MakeFileHeaderPart( pBuff, restSize, &fileHeaderPartSize );
		pBuff += fileHeaderPartSize;
		restSize -= fileHeaderPartSize;

		if( N_TRUE == init ) {
			/*==============================================================*/
			/* 初期モードではヘッダ部の後の固定部も同時作成					*/
			/*==============================================================*/
			/* 信号制御からID列を取得 */
			(void)Signal_control_GetSignalList( DEVICE_TYPE_SD, ID_TYPE_FIXED_ID, &fixedSignalNum, (const UI_16**)&fixedSignalIdList );
			(void)Signal_control_GetSignalList( DEVICE_TYPE_SD, ID_TYPE_VARIABLE_ID, &vehicleSignalNum, (const UI_16**)&vehicleSignalIdList );

			/* 固定信号データ配列を取得 */
			(void)Signal_control_GetSignalsInfo( DEVICE_TYPE_SD, fixedSignalNum, fixedSignalIdList, S_sd_signal_buff );

			/* 固定ID情報部を作成 */
			sd_ctrl_MakeIdInfoDataPart( fixedSignalNum, fixedSignalIdList, pBuff, restSize, &fixIdInfoPartSize );
			pBuff += fixIdInfoPartSize;
			restSize -= fixIdInfoPartSize;

			/* 固定車両情報部を作成 */
			sd_ctrl_MakeSignalDataPart( fixedSignalNum, S_sd_signal_buff, pBuff, restSize, &fixSignalPartSize );
			pBuff += fixSignalPartSize;
			restSize -= fixSignalPartSize;

			/* 変数ID情報部を作成 */
			sd_ctrl_MakeIdInfoDataPart( vehicleSignalNum, vehicleSignalIdList, pBuff, restSize, &varIdInfoPartSize );
			pBuff += fixIdInfoPartSize;
			restSize -= fixIdInfoPartSize;
		}
		/* ヘッダ全体サイズ格納 */
		*headSize = (fileHeaderPartSize + fixIdInfoPartSize + fixSignalPartSize + varIdInfoPartSize);
/*		OUTLOG(OutputLog("[SD] Header(init=%d) size=[%d]\n", init, *headSize);) */
	}
	return;

}

/****************************************************************************/
/* 関数名称		: sd_ctrl_MakeRecData										*/
/* 機能の説明	: 記録データ部作成											*/
/* 引数			: ( /O)dataBuff	バッファ先頭アドレス						*/
/*				  (I/ )buffSize	バッファサイズ								*/
/*				  ( /O)dataSize	作成したデータサイズ						*/
/* 戻り値		: void														*/
/* 作成者		: (OFT)T.Kamakura											*/
/* 作成日		: 16/10/01													*/
/* 備考			: 															*/
/****************************************************************************/
static void sd_ctrl_MakeRecData( UI_8 *dataBuff, UI_16 buffSize, UI_16 *dataSize )
{

	UI_16			varSignalPartSize = 0U;
	UI_8*			pBuff = NULL;
	UI_16			restSize = 0U;
	UI_8			vehicleSignalNum = 0U;
	UI_16*			vehicleSignalIdList = 0U;

	if( (NULL != dataBuff) && (NULL != dataSize) ) {
		/* バッファ先頭から格納 */
		pBuff = dataBuff;
		restSize = buffSize;

		/* 信号制御からID列を取得 */
		(void)Signal_control_GetSignalList( DEVICE_TYPE_SD, ID_TYPE_VARIABLE_ID, &vehicleSignalNum, (const UI_16**)&vehicleSignalIdList );

		/* 変数信号データ配列を取得 */
		(void)Signal_control_GetSignalsInfo( DEVICE_TYPE_SD, vehicleSignalNum, vehicleSignalIdList, S_sd_signal_buff );

		/* 固定車両情報部を作成 */
		sd_ctrl_MakeSignalDataPart( vehicleSignalNum, S_sd_signal_buff, pBuff, restSize, &varSignalPartSize );
		pBuff += varSignalPartSize;
		restSize -= varSignalPartSize;

		/* データ部全体サイズ格納 */
		*dataSize = varSignalPartSize;
	}
	return;

}

/****************************************************************************/
/* 関数名称		: sd_ctrl_MakeFileHeaderPart								*/
/* 機能の説明	: ファイルヘッダ情報（File Header）部作成					*/
/* 引数			: ( /O)headBuff	バッファ先頭アドレス						*/
/*				  (I/ )buffSize	バッファサイズ								*/
/*				  ( /O)headSize	作成したヘッダサイズ						*/
/* 戻り値		: void														*/
/* 作成者		: (OFT)T.Kamakura											*/
/* 作成日		: 16/10/01													*/
/* 備考			: 															*/
/****************************************************************************/
static void sd_ctrl_MakeFileHeaderPart( UI_8 *headBuff, UI_16 buffSize, UI_16 *headSize )
{

	UI_8*			p = NULL;
	UI_16			totalSize = 0U;
	UI_16			length = 0U;
	UI_16			firmwareVersion = 0U;
	UI_16			crc = 0UL;
	UI_16*			fixedSignalIdList = 0U;
	UI_16*			vehicleSignalIdList = 0U;
	SI_32			sdEncodeKey = 0UL;
	UI_8			fixedSignalNum = 0U;
	UI_8			vehicleSignalNum = 0U;
	UI_8			i = 0U;

	if( (NULL != headBuff) && (NULL != headSize) ) {
		/* 信号制御からID列を取得 */
		(void)Signal_control_GetSignalList( DEVICE_TYPE_SD, ID_TYPE_FIXED_ID, &fixedSignalNum, (const UI_16**)&fixedSignalIdList );
		(void)Signal_control_GetSignalList( DEVICE_TYPE_SD, ID_TYPE_VARIABLE_ID, &vehicleSignalNum, (const UI_16**)&vehicleSignalIdList );

		/* 出力初期化 */
		(void)memset(headBuff, 0, buffSize);
		*headSize = 0;

		/* 全体サイズ計算 */
		totalSize = sd_ctrl_GetFileHeaderPartSize( fixedSignalNum, vehicleSignalNum );
		length = totalSize - (SD_CTRL_FMT_HEADER_BYTES + SD_CTRL_FMT_FOOTER_BYTES) + 1U;
/*		OUTLOG(OutputLog("[SD] sd_ctrl_MakeFileHeaderPart(%d,%d)[%d,%d]\n", fixedSignalNum, vehicleSignalNum, totalSize, length);) */

		/* エンコードキー取得 */
		(void)Signal_control_GetEncodeKey( DEVICE_TYPE_SD, &sdEncodeKey );

		if( totalSize > buffSize ) {
			/* バッファ不足 */
		}
		else {
			/* データ作成開始 */
			p = headBuff;

			/*==================================================================*/
			/* ヘッダ作成														*/
			/*==================================================================*/
			*p = SD_CTRL_FMT_HEADER_START;
			p++;
			Common_Set16( p, length );
			p += 2;
			*p = SD_CTRL_FMT_TYPE_HEADER;
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
			Common_Set24( p, SD_CTRL_FORMAT_VERSION );
			p += 3;

			/* 信号リストバージョン(3バイト) */
			Common_Set24( p, Signal_control_GetSignalVersion() );
			p += 3;

			/* ファームウェアバージョン(3バイト) */
			(void)Eeprom_control_GetData( EEPROM_DATA_FIRMWARE_VER, &firmwareVersion );

			/* 16bitのバージョンを前詰めで記録 */
			Common_Set24( p, ((UI_32)firmwareVersion) << 8 );
			p += 3;

			/* コメント(10バイト) */
			/* 00固定 */
			p += 10;

			/* 取得可能な車両信号数（固定情報） */
			*p = fixedSignalNum;
			p++;
			for( i = 0; i < fixedSignalNum; i++ ) {
				/* 取得可能な車両信号ID（固定情報） */
				Common_Set16( p, fixedSignalIdList[i] );
				p += 2;
			}
			/* 取得可能な車両信号数（車両情報） */
			*p = vehicleSignalNum;
			p++;
			for( i = 0; i < vehicleSignalNum; i++ ) {
				/* 取得可能な車両信号ID（車両情報） */
				Common_Set16( p, vehicleSignalIdList[i] );
				p += 2;
			}
			/* ログ書き込み開始時間（UTC) YYMMDDHHMMSS(例151110154322）	*/
			(void)memcpy( p, S_sd_log_data_info.timeLogStart, SD_CTRL_FMT_UTC_LEN );
			p += SD_CTRL_FMT_UTC_LEN;
			/* ログ書き込み終了時間（UTC) YYMMDDHHMMSS（例151110161515） */
			(void)memcpy( p, S_sd_log_data_info.timeLogEnd, SD_CTRL_FMT_UTC_LEN );
			p += SD_CTRL_FMT_UTC_LEN;

			/* 最北端緯度 */
			Common_Set32( p, (UI_32)S_sd_log_data_info.latitudeNorthernMost );
			p += 4;
			/* 最西端緯度 */
			Common_Set32( p, (UI_32)S_sd_log_data_info.longtudeWesternMost );
			p += 4;
			/* 最南端緯度 */
			Common_Set32( p, (UI_32)S_sd_log_data_info.latitudeSouthernMost );
			p += 4;
			/* 最東端経度 */
			Common_Set32( p, (UI_32)S_sd_log_data_info.longtudeEasternMost );
			p += 4;

			/* 乱数(4バイト) */
			Common_Set32( p, (UI_32)sdEncodeKey );
			p += 4;

			/*==================================================================*/
			/* フッタ作成														*/
			/*==================================================================*/
			crc = Common_CalcCRC( headBuff, 1U, length + 2U );
			Common_Set16( p, crc );
			p += 2;

			*p = SD_CTRL_FMT_FOOTER_END;
			p++;

			/* データ長を格納 */
			*headSize = totalSize;
		}
	}

}

/****************************************************************************/
/* 関数名称		: sd_ctrl_MakeIdInfoDataPart								*/
/* 機能の説明	: ID情報（ID Info Data）部作成								*/
/* 引数			: (I/ )signalNum	信号数									*/
/*				  (I/ )idList		IDリスト								*/
/*				  ( /O)dataBuff	バッファ先頭アドレス						*/
/*				  (I/ )buffSize	バッファサイズ								*/
/*				  ( /O)dataSize	作成したデータサイズ						*/
/* 戻り値		: void														*/
/* 作成者		: (OFT)T.Kamakura											*/
/* 作成日		: 16/10/01													*/
/* 備考			: 															*/
/****************************************************************************/
static void sd_ctrl_MakeIdInfoDataPart( UI_8 signalNum, const UI_16 *idList, UI_8 *dataBuff, UI_16 buffSize, UI_16 *dataSize )
{

	UI_8*			p = NULL;
	UI_16			totalSize = 0U;
	UI_16			length = 0U;
	UI_16			crc = 0U;
	UI_8			i = 0U;

	if( (NULL != idList) && (NULL != dataBuff) && (NULL != dataSize) ) {
		/* 出力初期化 */
		(void)memset(dataBuff, 0, buffSize);
		*dataSize = 0;

		/* 全体サイズ計算 */
		totalSize = sd_ctrl_GetIdInfoDataPartSize( signalNum );
		length = totalSize - (SD_CTRL_FMT_HEADER_BYTES + SD_CTRL_FMT_FOOTER_BYTES) + 1U;

		if( totalSize > buffSize ) {
			/* バッファ不足 */
		}
		else {
			/* データ作成開始 */
			p = dataBuff;

			/*==================================================================*/
			/* ヘッダ作成														*/
			/*==================================================================*/
			*p = SD_CTRL_FMT_HEADER_START;
			p++;
			Common_Set16( p, length );
			p += 2;
			*p = SD_CTRL_FMT_TYPE_ID_INFO_DATA;
			p++;

			/*==================================================================*/
			/* データ部作成														*/
			/*==================================================================*/
			/* 書き込み車両信号数 */
			*p = signalNum;
			p++;

			/* 応答する車両信号ID */
			for( i = 0U; i < signalNum; i++ ) {
				/* 車両信号ID */
				Common_Set16( p, idList[i] );
				p += 2;
			}
			/*==================================================================*/
			/* フッタ作成														*/
			/*==================================================================*/
			crc = Common_CalcCRC( dataBuff, 1U, length + 2U );
			Common_Set16( p, crc );
			p += 2;

			*p = SD_CTRL_FMT_FOOTER_END;
			p++;

			/* データ長を格納 */
			*dataSize = totalSize;
		}
	}

}

/****************************************************************************/
/* 関数名称		: sd_ctrl_MakeSignalDataPart								*/
/* 機能の説明	: 車両情報（Signal Data）部作成								*/
/* 引数			: (I/ )signalNum		信号数								*/
/*				  (I/ )signalValues	信号リスト								*/
/*				  ( /O)dataBuff	バッファ先頭アドレス						*/
/*				  (I/ )buffSize	バッファサイズ								*/
/*				  ( /O)dataSize	作成したデータサイズ						*/
/* 戻り値		: void														*/
/* 作成者		: (OFT)T.Kamakura											*/
/* 作成日		: 16/10/01													*/
/* 備考			: 															*/
/****************************************************************************/
static void sd_ctrl_MakeSignalDataPart( UI_8 signalNum, const SI_32 *signalValues, UI_8 *dataBuff, UI_16 buffSize, UI_16 *dataSize )
{

	UI_8*			p = NULL;
	UI_16			totalSize = 0U;
	UI_16			length = 0U;
	UI_16			crc = 0U;
	UI_8			i = 0U;

	if( (NULL != signalValues) && (NULL != dataBuff) && (NULL != dataSize) ) {
		/* 出力初期化 */
		(void)memset(dataBuff, 0, buffSize);
		*dataSize = 0;

		/* 全体サイズ計算 */
		totalSize = sd_ctrl_GetSignalDataPartSize( signalNum, 0 );
		length = totalSize - (SD_CTRL_FMT_HEADER_BYTES + SD_CTRL_FMT_FOOTER_BYTES) + 1U;

		if( totalSize > buffSize ) {
			/* バッファ不足 */
		}
		else {
			/* データ作成開始 */
			p = dataBuff;

			/*==================================================================*/
			/* ヘッダ作成														*/
			/*==================================================================*/
			*p = SD_CTRL_FMT_HEADER_START;
			p++;
			Common_Set16( p, length );
			p += 2;
			*p = SD_CTRL_FMT_TYPE_SIGNAL_DATA;
			p++;

			/*==================================================================*/
			/* データ部作成														*/
			/*==================================================================*/
			/* 電源ONからのタイムスタンプ(ms) */
			Common_Set32( p, Timer_ctrl_GetTime1ms() );
			p += 4;

			/* エラーコード数 */
			*p = 0U;
			p++;
			/* 予約エラーコードは現在格納しない */
			
			/* 書き込み車両信号数 */
			*p = signalNum;
			p++;

			/* 書き込み車両信号の値 */
			for( i = 0U; i < signalNum; i++ ) {
				/* 信号値 */
				Common_Set32( p, (UI_32)signalValues[i] );
				p += 4;
			}
			/*==================================================================*/
			/* フッタ作成														*/
			/*==================================================================*/
			crc = Common_CalcCRC( dataBuff, 1U, length + 2U );
			Common_Set16( p, crc );
			p += 2;

			*p = SD_CTRL_FMT_FOOTER_END;
			p++;

			/* データ長を格納 */
			*dataSize = totalSize;
		}
	}

}

/****************************************************************************/
/* 関数名称		: sd_ctrl_UpdateLogDataInfo									*/
/* 機能の説明	: ログ情報更新												*/
/* 引数			: (I/ )init	初期化フラグ(N_TRUE:初期化／それ以外:更新)		*/
/* 戻り値		: void														*/
/* 作成者		: (OFT)T.Kamakura											*/
/* 作成日		: 16/10/01													*/
/* 備考			: 															*/
/****************************************************************************/
static void sd_ctrl_UpdateLogDataInfo( B_8 init )
{

	T_Gnss_ctrl_nav_pvt_info*	hnr_info = NULL;
	UI_32						check_flags = 0UL;
	T_COMMON_DATE_TIME	dtm;

	if( N_TRUE == init ) {
		/*==================================================================*/
		/* 初期化動作時														*/
		/*==================================================================*/
		(void)memset(&S_sd_log_data_info, 0, sizeof(S_sd_log_data_info));
	}
	/* GNSSから情報取得 */
	hnr_info = Gnss_cntrol_GetNavPvtInfo();

	/* 取得した情報をチェック */
	check_flags = sd_ctrl_CheckGnssHnrInfo( hnr_info );

	if( 0UL != (check_flags & SD_CTRL_LOG_DATA_POS_ENABLE) ) {
		/*==================================================================*/
		/* GNSS 座標有効													*/
		/*==================================================================*/
		if( 0UL == (S_sd_log_data_info.flags & SD_CTRL_LOG_DATA_POS_ENABLE ) ) {
			/*==============================================================*/
			/* 座標未初期化なら初期化										*/
			/*==============================================================*/
			/* 緯度経度範囲を現在の地点座標で初期化 */
			S_sd_log_data_info.latitudeNorthernMost = hnr_info->lat;
			S_sd_log_data_info.longtudeWesternMost = hnr_info->lon;
			S_sd_log_data_info.latitudeSouthernMost = hnr_info->lat;
			S_sd_log_data_info.longtudeEasternMost = hnr_info->lon;

			/* 初期化済フラグセット */
			S_sd_log_data_info.flags |= SD_CTRL_LOG_DATA_POS_ENABLE;

			OUTLOG(OutputLog("[SD][%08d] sd_ctrl_UpdateLogDataInfo(init=%d) SD_CTRL_LOG_DATA_POS_ENABLE\n", Timer_ctrl_GetTime1ms(), init );)
		}
		else {
			/*==============================================================*/
			/* 座標初期済なら領域拡張										*/
			/*==============================================================*/
			/* 緯度経度範囲を現在の地点座標で更新 */
			if( S_sd_log_data_info.latitudeNorthernMost < hnr_info->lat ) {
				/* 北端拡張 */
				S_sd_log_data_info.latitudeNorthernMost = hnr_info->lat;
			}
			if( S_sd_log_data_info.latitudeSouthernMost > hnr_info->lat ) {
				/* 南端拡張 */
				S_sd_log_data_info.latitudeSouthernMost = hnr_info->lat;
			}
			if( S_sd_log_data_info.longtudeEasternMost < hnr_info->lon ) {
				/* 東端拡張 */
				S_sd_log_data_info.longtudeEasternMost = hnr_info->lon;
			}
			if( S_sd_log_data_info.longtudeWesternMost > hnr_info->lon ) {
				/* 西端拡張 */
				S_sd_log_data_info.longtudeWesternMost = hnr_info->lon;
			}
		}
	}

	if( 0UL != (check_flags & SD_CTRL_LOG_DATA_TIME_ENABLE) ) {
		/*==================================================================*/
		/* GNSS 時刻有効													*/
		/*==================================================================*/
		dtm.year	= hnr_info->year;
		dtm.month	= hnr_info->month;
		dtm.day		= hnr_info->day;
		dtm.hour	= hnr_info->hour;
		dtm.min		= hnr_info->min;
		dtm.sec		= hnr_info->sec;
		if( 0UL == (S_sd_log_data_info.flags & SD_CTRL_LOG_DATA_TIME_ENABLE ) ) {
			/*==============================================================*/
			/* 時刻未初期化なら初期化										*/
			/*==============================================================*/
			/* ログ書き込み開始時間 = ログ書き込み終了時間 */
			Common_MakeDateTimeStr(&dtm, COMMON_DATE_TIME_STR_TYPE_YEAR_2, S_sd_log_data_info.timeLogStart);	/* 年は2桁で作成 */
			(void)memcpy( S_sd_log_data_info.timeLogEnd, S_sd_log_data_info.timeLogStart, sizeof(S_sd_log_data_info.timeLogEnd) );

			/* 初期化済フラグセット */
			S_sd_log_data_info.flags |= SD_CTRL_LOG_DATA_TIME_ENABLE;

			OUTLOG(OutputLog("[SD][%08d] sd_ctrl_UpdateLogDataInfo(init=%d) SD_CTRL_LOG_DATA_TIME_ENABLE\n", Timer_ctrl_GetTime1ms(), init );)
		}
		else {
			/*==============================================================*/
			/* 時刻初期化済													*/
			/*==============================================================*/
			/* ログ書き込み終了時間を更新 */
			Common_MakeDateTimeStr(&dtm, COMMON_DATE_TIME_STR_TYPE_YEAR_2, S_sd_log_data_info.timeLogEnd);	/* 年は2桁で作成 */
		}
	}

}

/****************************************************************************/
/* 関数名称		: sd_ctrl_CheckGnssHnrInfo									*/
/* 機能の説明	: GNSS NAV-HNR or HNR-PVT情報 をチェックする				*/
/* 引数			: (I/ )hnr_info	NAV-HNR or HNR-PVT情報						*/
/* 戻り値		: bitフラグの論理和											*/
/*				  SD_CTRL_LOG_DATA_TIME_ENABLE:	時刻が取得できている		*/
/*				  SD_CTRL_LOG_DATA_POS_ENABLE:		座標が取得できている	*/
/* 作成者		: (OFT)T.Kamakura											*/
/* 作成日		: 16/11/07													*/
/* 備考			: 															*/
/****************************************************************************/
static UI_32 sd_ctrl_CheckGnssHnrInfo( const T_Gnss_ctrl_nav_pvt_info* hnr_info )
{

	UI_32	flags = 0UL;

	if( NULL != hnr_info ) {
		switch( hnr_info->gpsFix ) {
		case GNSS_GSPFIX_NOFIX:					/* No Fix */
			break;
		case GNSS_GSPFIX_DEADRECKONIN_ONLY:		/* Dead Reckoning only */
		case GNSS_GSPFIX_2DFIX:					/* 2D-Fix */
		case GNSS_GSPFIX_3DFIX:					/* 3D-Fix */
		case GNSS_GSPFIX_GPS_AND_DEADRECKONING:	/* GPS + dead reckoning combined */
			/* 座標も時刻も正しい */
			flags = SD_CTRL_LOG_DATA_TIME_ENABLE | SD_CTRL_LOG_DATA_POS_ENABLE;
			break;
		case GNSS_GSPFIX_TIME_ONLY_FIX:			/* Time only fix */
			/* 時刻のみ正しい */
			flags = SD_CTRL_LOG_DATA_TIME_ENABLE;
			break;
		default:
			break;
		}
	}
	return flags;

}

/****************************************************************************/
/* 関数名称		: sd_ctrl_WriteRecData										*/
/* 機能の説明	: RECデータ書き込み											*/
/* 引数			: (I/ )dataBuff	データバッファ								*/
/*				   (I/ )dataSize	データサイズ							*/
/* 戻り値		: void														*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 16/10/08													*/
/* 備考			: リングバッファにデータを書き込む							*/
/*				  リングバッファのデータがSD書き込みサイズに達した場合は	*/
/*				  リングバッファにたまったデータをSDに書き込む				*/
/****************************************************************************/
static void sd_ctrl_WriteRecData( UI_8 *dataBuff, UI_16 dataSize )
{

	/* リングバッファへの書き込み	*/
	if (RingBuffer_GetCanWriteSize(&S_sd_rec_data_mng) >= dataSize) {
		/* リングバッファに書き込む	*/
		(void)RingBuffer_SetArrayData(&S_sd_rec_data_mng, dataBuff, dataSize);
		S_sd_ring_write_size += dataSize;
	}

	/* リングバッファのデータをブロック単位でSDに書き込む	*/
	sd_ctrl_FlushRecData(N_FALSE);

	return;

}

/****************************************************************************/
/* 関数名称		: sd_ctrl_FlushRecData										*/
/* 機能の説明	: リングバッファのデータをSDに書き込む						*/
/* 引数			: (I/ )allFlush	全データ書き込みフラグ						*/
/*							N_TRUE:全データ書き込む							*/
/*							N_FALSE:ブロック区切りまで書き込む				*/
/* 戻り値		: void														*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 16/10/08													*/
/* 備考			: 															*/
/****************************************************************************/
static void sd_ctrl_FlushRecData( B_8 allFlush )
{

	E_FIO_RESULT	fioRet;
	UI_32			reqNo;
	UI_32			writeSize;
	UI_8			*pBuff;

	/* ブロックの途中から未書き込みのデータがある場合の処理 */
	if ((S_sd_ring_flush_size % SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE) != 0) {	/* ブロック途中からの未書き込みデータあり	*/
		/* ブロック途中からブロック区切りまでのデータをSDに書き込む	*/

		/* SDに書き込むサイズを算出（未書き込みデータ先頭からブロック区切り位置までのサイズ）	*/
		writeSize = (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE - (S_sd_ring_flush_size % (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE);
		if ((S_sd_ring_write_size - S_sd_ring_flush_size) >= writeSize) {		/* 未書き込みデータがSD書き込みサイズ以上ある	*/
			/* SDに書き込むバッファ位置を算出	*/
			pBuff = S_sd_rec_data_buff + (S_sd_ring_flush_size % ((UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE * (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_NUM));

			/* SDに書き込む */
			fioRet = FileIO_Write( FIO_WRITE_TYPE_FILE_OPEN, S_sd_rec_filepath, pBuff, FIO_OFS_EOF, writeSize, sd_ctrl_RecDataWriteCallback, &reqNo );
			if (fioRet != FIO_RESULT_OK) {
				/* SD書き込み失敗時は何もしない	*/
				return;
			}
			S_sd_ring_flush_size += writeSize;
			S_sd_rec_write_req_cnt++;
		}
	}

	/* ブロック区切り先頭からブロック単位でSDに書き込む処理	*/
	while ((S_sd_ring_write_size - S_sd_ring_flush_size) >= SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE) {	/* 未書き込みデータがブロックサイズ以上ある	*/
		/* SDに書き込むバッファ位置を算出	*/
		pBuff = S_sd_rec_data_buff + (S_sd_ring_flush_size % ((UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE * (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_NUM));

		/* SDに書き込む */
		fioRet = FileIO_Write( FIO_WRITE_TYPE_FILE_OPEN, S_sd_rec_filepath, pBuff, FIO_OFS_EOF, SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE, sd_ctrl_RecDataWriteCallback, &reqNo );
		if (fioRet != FIO_RESULT_OK) {
			/* SD書き込み失敗時は何もしない	*/
			return;
		}
		S_sd_ring_flush_size += (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE;
		S_sd_rec_write_req_cnt++;
	}

	/* ブロック区切り先頭からブロック単位に満たないサイズのデータを書き込む処理	*/
	if (allFlush == N_TRUE) {				/* リングバッファ全データ書き込みが指定されている	*/

		/* SDに書き込むサイズを算出	*/
		writeSize = S_sd_ring_write_size - S_sd_ring_flush_size;
		if (writeSize > 0) {

			/* SDに書き込むバッファ位置を算出	*/
			pBuff = S_sd_rec_data_buff + (S_sd_ring_flush_size % ((UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE * (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_NUM));

			/* SDに書き込む */
			fioRet = FileIO_Write( FIO_WRITE_TYPE_FILE_OPEN, S_sd_rec_filepath, pBuff, FIO_OFS_EOF, writeSize, sd_ctrl_RecDataWriteCallback, &reqNo );
			if (fioRet != FIO_RESULT_OK) {
				/* SD書き込み失敗時は何もしない	*/
				return;
			}
			S_sd_ring_flush_size += writeSize;
			S_sd_rec_write_req_cnt++;
		}
	}
	return;

}

/****************************************************************************/
/* 関数名称		: sd_ctrl_UpdateHeader										*/
/* 機能の説明	: 定期ヘッダ更新処理										*/
/* 引数			: void														*/
/* 戻り値		: void														*/
/* 作成者		: (OFT)T.Kamakura											*/
/* 作成日		: 16/11/07													*/
/* 備考			: 															*/
/****************************************************************************/
static void sd_ctrl_UpdateHeader( void )
{

	E_FIO_RESULT		fioRet;
	UI_32				reqNo;
	UI_16				headSize;
	UI_32				curTime = 0UL;

	if( S_sd_rec_write_req_cnt > 0 ) {
		/* 現在時刻取得 */
		curTime = Timer_ctrl_GetTime1ms();
		if( SD_CTRL_UPDATE_HEADER_CYCLE_MSEC <= (curTime - S_sd_update_header_time) ) {
			/*==============================================================*/
			/* 更新周期時間経過												*/
			/*==============================================================*/
			/* RECファイルのヘッダ情報作成	*/
			sd_ctrl_MakeRecHead( S_sd_rec_head, sizeof(S_sd_rec_head), &headSize, N_FALSE );

			/* RECファイルのヘッダ情報を書き込む【更新】*/
			fioRet = FileIO_Write( FIO_WRITE_TYPE_FILE_OPEN, S_sd_rec_filepath, S_sd_rec_head, 0, (UI_32)headSize, sd_ctrl_RecDataWriteCallback, &reqNo );
			if( fioRet == FIO_RESULT_OK ) {
				S_sd_rec_write_req_cnt++;
				S_sd_update_header_time = curTime;
			}
/*			OUTLOG(OutputLog("[SD][%08d] sd_ctrl_UpdateHeader()\n", curTime );) */
		}
	}

}

/****************************************************************************/
/* 関数名称		: sd_ctrl_EndRec											*/
/* 機能の説明	: REC終了処理												*/
/* 引数			: void														*/
/* 戻り値		: void														*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 16/10/08													*/
/* 備考			: 															*/
/****************************************************************************/
static void sd_ctrl_EndRec( void )
{

	E_FIO_RESULT		fioRet;
	UI_32				reqNo;
	UI_16				headSize;

	if (S_sd_rec_write_req_cnt > 0) {
		/* リングバッファに残っているデータをSDに書き込む	*/
		sd_ctrl_FlushRecData( N_TRUE );

		/* RECファイルのヘッダ情報作成	*/
		sd_ctrl_MakeRecHead( S_sd_rec_head, sizeof(S_sd_rec_head), &headSize, N_FALSE );

		/* RECファイルのヘッダ情報を書き込む【更新】*/
		fioRet = FileIO_Write( FIO_WRITE_TYPE_FILE_OPEN, S_sd_rec_filepath, S_sd_rec_head, 0, (UI_32)headSize, sd_ctrl_RecDataWriteCallback, &reqNo );
		if (fioRet == FIO_RESULT_OK) {
			S_sd_rec_write_req_cnt++;
		}
	}

}

/****************************************************************************/
/* 関数名称		: sd_ctrl_GetFileHeaderPartSize								*/
/* 機能の説明	: ファイルヘッダ部サイズ取得								*/
/* 引数			: (I/ )fixedSignalNum		固定IDの数						*/
/* 				: (I/ )vehicleSignalNum		変数IDの数						*/
/* 戻り値		: ファイルヘッダ部サイズ									*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 17/08/28													*/
/* 備考			: 															*/
/****************************************************************************/
static UI_16 sd_ctrl_GetFileHeaderPartSize( UI_8 fixedSignalNum, UI_8 vehicleSignalNum )
{
	return (UI_16)(90U + (2U * (UI_16)fixedSignalNum) + (2U * (UI_16)vehicleSignalNum));

}

/****************************************************************************/
/* 関数名称		: sd_ctrl_GetIdInfoDataPartSize								*/
/* 機能の説明	: ID情報部サイズ取得										*/
/* 引数			: (I/ )signalNum			IDの数							*/
/* 戻り値		: ID情報部サイズ											*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 17/08/28													*/
/* 備考			: 															*/
/****************************************************************************/
static UI_16 sd_ctrl_GetIdInfoDataPartSize( UI_8 signalNum )
{
	return (UI_16)(8U + (2U * (UI_16)signalNum));

}

/****************************************************************************/
/* 関数名称		: sd_ctrl_GetSignalDataPartSize								*/
/* 機能の説明	: 車両情報部サイズ取得										*/
/* 引数			: (I/ )signalNum			IDの数							*/
/* 				: (I/ )errNum				エラーコードの数				*/
/* 戻り値		: 車両情報部サイズ											*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 17/08/28													*/
/* 備考			: 															*/
/****************************************************************************/
static UI_16 sd_ctrl_GetSignalDataPartSize( UI_8 signalNum, UI_8 errNum )
{
	return (UI_16)((13U + (6U * (UI_16)errNum)) + (4U * (UI_16)signalNum));

}
#endif /* _VDV_APP */

#ifdef _VDV_APP
/*==========================================================================*/
/* シミュレーション制御関数													*/
/*==========================================================================*/
/****************************************************************************/
/* 関数名称		:	sd_sim_state_MainProc									*/
/* 機能の説明	:	シミュレーション制御メイン								*/
/* 引数			:	void													*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/03												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_sim_state_MainProc( void )
{

	PFN_SD_SIM_STATE_PROC	state_proc = NULL;
	SD_SIM_EVENT			sim_event = SD_SIM_EVENT_NONE;
	E_SD_SIM_STAT			next_status = SD_SIM_STAT_NONE;
	B_8						fcontinue = N_TRUE;

	if( SD_SIM_STAT_COUNT > S_sd_sim_status ) {
		state_proc = S_sd_sim_state_proc[S_sd_sim_status];

		/* 状態関数内で連続呼出しが要求された場合は連続 */
		while( (NULL != state_proc) && (N_TRUE == fcontinue) ) {
			/* イベント作成 */
			sd_sim_state_MakeEvent( &sim_event );

			/* デフォルトは１回呼出し */
			fcontinue = N_FALSE;

			/* 状態関数呼び出し */
			next_status = state_proc( sim_event, &fcontinue );
			if( next_status != SD_SIM_STAT_NONE ) {
				/* 状態遷移発生 */
				sd_sim_state_ProcOnLeave( S_sd_sim_status );
				OUTLOG(OutputLog( "[SDSIM] STATUS %d->%d [EV:%08x][%08d]\n", S_sd_sim_status, next_status, sim_event, Timer_ctrl_GetTime1ms() );)
				S_sd_sim_status = next_status;
				sd_sim_state_ProcOnEnter( next_status );
			}
			state_proc = S_sd_sim_state_proc[S_sd_sim_status];
		}
		/* バックグラウンド処理 */
		switch( S_sd_sim_status ) {
		case SD_SIM_STAT_SEARCH_SIM_FILE:			/* SIMファイル検索中		*/
			/* シミュレーションファイル検索（開始）*/
			sd_sim_SearchSimFile();
			break;
		case SD_SIM_STAT_WAIT_HEADER:				/* ファイルヘッダ受信待ち 	*/
		case SD_SIM_STAT_WAIT_FIX_ID_LIST:			/* 固定信号ID受信待ち 		*/
		case SD_SIM_STAT_WAIT_FIX_VALUES:			/* 固定車両情報受信待ち		*/
		case SD_SIM_STAT_WAIT_VAR_ID_LIST:			/* 変数信号ID受信待ち		*/
		case SD_SIM_STAT_WAIT_VAR_VALUES:			/* 車両情報受信待ち 		*/
			/* シミュレーションファイル周回読み（開始）*/
			sd_sim_ReadSimFile();
			break;
		default:
			break;
		}
	}

}
/****************************************************************************/
/* 関数名称		:	sd_sim_state_MakeEvent									*/
/* 機能の説明	:	イベント作成											*/
/* 引数			:	( /O)sim_event	作成したイベントの格納先				*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/03												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_sim_state_MakeEvent( SD_SIM_EVENT* sim_event )
{

	E_SW_REC_BTN_STATUS rec_btn_status = SYS_REC_BTN_UNKNOWN;
	UI_32				work_sim_event = 0UL;
	UI_32				flags = 0UL;

	if( NULL != sim_event ) {
		flags = S_sd_sim_ctrl_info.flags;

		/* 検索状態 */
		if( 0 != (SD_SIM_FLAG_FOUND_SIM_FILE & flags ) ) {
			/* SIMファイル検索完了 */
			work_sim_event |= (UI_32)SD_SIM_EVENT_FOUND_SIM_FILE;
		}
		/* 受信パケット */
		(void)sd_sim_data_AnalyzeRecvData( &S_sd_sim_ctrl_info.packet_info );
		switch( S_sd_sim_ctrl_info.packet_info.type ) {
		case SD_CTRL_FMT_TYPE_HEADER:
			work_sim_event |= (UI_32)SD_SIM_EVENT_RECV_HEADER;
			break;
		case SD_CTRL_FMT_TYPE_ID_INFO_DATA:
			if( 0 == (SD_SIM_FLAG_RECV_FIX_ID_LIST & flags) ) {
				/* 固定ID情報未受信 */
				work_sim_event |= (UI_32)SD_SIM_EVENT_RECV_FIX_ID_LIST;
			}
			else {
				/* 固定ID情報受信済 */
				work_sim_event |= (UI_32)SD_SIM_EVENT_RECV_VAR_ID_LIST;
			}
			break;
		case SD_CTRL_FMT_TYPE_SIGNAL_DATA:
			if( 0 == (SD_SIM_FLAG_RECV_FIX_VALUES & flags) ) {
				/* 固定車両情報未受信 */
				work_sim_event |= (UI_32)SD_SIM_EVENT_RECV_FIX_VALUES;
			}
			else {
				/* 固定車両情報受信済 */
				if (SD_PROCESS_SUB == S_processType) {
					/* 固定車両情報はサブ処理で受信 */
					work_sim_event |= (UI_32)SD_SIM_EVENT_RECV_VAR_VALUES;
				}
			}
			break;
		default:
			break;
		}

		if( 0 != (SD_SIM_FLAG_READ_ERROR & flags) ) {
			/* ファイル読みエラー発生 */
			work_sim_event |= (UI_32)SD_SIM_EVENT_READ_ERROR;
		}
		
		/* RECボタン状態 */
		rec_btn_status = Sw_control_GetRecButtonStatus();
		if( SYS_REC_BTN_ON_SHORTPRESS == rec_btn_status ) {
			/* シミュレーション頭出し要求 */
			work_sim_event |= (UI_32)SD_SIM_EVENT_REQ_REWIND;
		}

		if( 0 == ((SD_SIM_FLAG_SEARCHING_FILE | SD_SIM_FLAG_READING_FILE)& flags) ) {
			/* ファイル読み終了 */
			work_sim_event |= (UI_32)SD_SIM_EVENT_END_READ;
		}

		if( SYS_OPERATION_PRE_ACC_OFF == System_control_GetStatus() ) {
			/* ACC OFF */
			work_sim_event |= (UI_32)SD_SIM_EVENT_ACC_OFF;
		}

		/* イベント格納 */
		*sim_event = work_sim_event;
	}

}
/****************************************************************************/
/* 関数名称		:	sd_sim_state_ProcOnEnter								*/
/* 機能の説明	:	状態遷移時処理:状態に入るとき							*/
/* 引数			:	stat	遷移先状態										*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/03												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_sim_state_ProcOnEnter( E_SD_SIM_STAT stat )
{

	switch( stat ) {
	case SD_SIM_STAT_SEARCH_SIM_FILE:
		/* シミュレーション制御情報初期化 */
		sd_sim_InitSimCtrlInfo( SD_SIM_INIT_MODE_REWIND_SIM );
		sd_sim_ClearTime( SD_SIM_TIME_KIND_LAST_SEARCH_START );
		break;
	case SD_SIM_STAT_WAIT_HEADER:				/* ファイルヘッダ受信待ち 	*/
		break;
	case SD_SIM_STAT_WAIT_FIX_ID_LIST:			/* 固定信号ID受信待ち 		*/
		break;
	case SD_SIM_STAT_WAIT_FIX_VALUES:			/* 固定車両情報受信待ち		*/
		break;
	case SD_SIM_STAT_WAIT_VAR_ID_LIST:			/* 変数信号ID受信待ち		*/
		break;
	case SD_SIM_STAT_WAIT_VAR_VALUES:			/* 車両情報受信待ち 		*/
		/* 変数車両情報バッファクリア */
		sd_sim_signal_ClearBuffer( N_FALSE );
		break;
	case SD_SIM_STAT_WAIT_STOP:					/* 停止待ち					*/
		break;
	case SD_SIM_STAT_WAIT_STOP_ACC_OFF:			/* ACC OFFによる停止待ち	*/
		break;
	case SD_SIM_STAT_ACC_OFF:					/* ACC OFF					*/
		break;

	default:
		break;
	}

}
/****************************************************************************/
/* 関数名称		:	sd_sim_state_ProcOnLeave								*/
/* 機能の説明	:	状態遷移時処理:状態を出るとき							*/
/* 引数			:	stat	遷移元状態										*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/03												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_sim_state_ProcOnLeave( E_SD_SIM_STAT stat )
{

	switch( stat ) {
	case SD_SIM_STAT_SEARCH_SIM_FILE:
		break;
	case SD_SIM_STAT_WAIT_HEADER:				/* ファイルヘッダ受信待ち 	*/
		break;
	case SD_SIM_STAT_WAIT_FIX_ID_LIST:			/* 固定信号ID受信待ち 		*/
		break;
	case SD_SIM_STAT_WAIT_FIX_VALUES:			/* 固定車両情報受信待ち		*/
		break;
	case SD_SIM_STAT_WAIT_VAR_ID_LIST:			/* 変数信号ID受信待ち		*/
		break;
	case SD_SIM_STAT_WAIT_VAR_VALUES:			/* 車両情報受信待ち 		*/
		break;
	case SD_SIM_STAT_WAIT_STOP:					/* 停止待ち					*/
		break;
	case SD_SIM_STAT_WAIT_STOP_ACC_OFF:			/* ACC OFFによる停止待ち	*/
		break;
	case SD_SIM_STAT_ACC_OFF:					/* ACC OFF					*/
		break;
	default:
		break;
	}

}
/*==========================================================================*/
/* シミュレーション操作関数													*/
/*==========================================================================*/
/****************************************************************************/
/* 関数名称		:	sd_sim_InitSimCtrlInfo									*/
/* 機能の説明	:	シミュレーション制御初期化								*/
/* 引数			:	mode	初期化モード									*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_sim_InitSimCtrlInfo( E_SD_SIM_INIT_MODE mode )
{

	switch( mode ) {
	case SD_SIM_INIT_MODE_ENTER_SIM:
		/*==================================================================*/
		/* シミュレーションモードに入るときの初期化							*/
		/*==================================================================*/
		/* 制御情報を0で初期化 */
		(void)memset(&S_sd_sim_ctrl_info, 0, sizeof(S_sd_sim_ctrl_info));
		/* ID情報を0で初期化 */
		(void)memset(&S_sd_sim_id_info, 0, sizeof(S_sd_sim_id_info));

		/* SD_SIM_STAT_NONE にして起動処理を走らせる */
		S_sd_sim_status = SD_SIM_STAT_NONE;
		break;

	case SD_SIM_INIT_MODE_REWIND_SIM:
		/*==================================================================*/
		/* シミュレーション頭出し時の初期化									*/
		/*==================================================================*/
		/* 制御情報を0で初期化 */
		(void)memset(&S_sd_sim_ctrl_info, 0, sizeof(S_sd_sim_ctrl_info));
		break;

	case SD_SIM_INIT_MODE_CONTINUE:
		/*==================================================================*/
		/* 周回読み継続時の初期化											*/
		/*==================================================================*/
		/* ファイル読み進行状況のフラグを落とす */
		S_sd_sim_ctrl_info.flags &= (~(
			SD_SIM_FLAG_RECV_HEADER			|	/* ヘッダ受信済				*/
			SD_SIM_FLAG_RECV_FIX_ID_LIST	|	/* 固定IDリスト取得済		*/
			SD_SIM_FLAG_RECV_FIX_VALUES		|	/* 固定車両情報取得済		*/
			SD_SIM_FLAG_RECV_VAR_ID_LIST	|	/* 変数IDリスト取得済		*/
			SD_SIM_FLAG_RECV_VAR_VALUES		|	/* 変数車両情報取得済		*/
			SD_SIM_FLAG_READ_ERROR			|	/* READエラーあり			*/
			0UL));

		/* ファイル読みサイズを初期化 */
		S_sd_sim_ctrl_info.sim_file_read_size = 0UL;
		break;

	default:
		break;
	}

}
/****************************************************************************/
/* 関数名称		:	sd_sim_SearchSimFile									*/
/* 機能の説明	:	シミュレーションファイル検索（開始）					*/
/* 引数			:	void													*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_sim_SearchSimFile( void )
{

	E_COMMON_RESULT	res;

	if( 0 == (S_sd_sim_ctrl_info.flags & (SD_SIM_FLAG_SEARCHING_FILE|SD_SIM_FLAG_FOUND_SIM_FILE)) ) {
		/*==================================================================*/
		/* ファイル検索中ではなく、ファイルパス未発見						*/
		/*==================================================================*/
		/* あまり頻繁にやらないためにタイムアウトチェック */
		if( 0 != sd_sim_CheckTimeout( SD_SIM_TIME_KIND_LAST_SEARCH_START, SD_SIM_SEARCH_INTERVAL_MSEC ) ) {
			/*==============================================================*/
			/* 間隔空いていれば検索OK										*/
			/*==============================================================*/
			/* ファイルパスバッファクリア */
			(void)memset(S_sd_sim_ctrl_info.sim_file_path, 0, sizeof(S_sd_sim_ctrl_info.sim_file_path));

			/* ファイル検索開始 */
			res = Common_SerchVdvFile( COMMON_VDV_FILE_TYPE_SIM, sd_sim_SearchSimFileCB );
			if( COMMON_RESULT_OK == res ) {
				/* ファイル検索開始時刻を記録 */
				sd_sim_RegTime( SD_SIM_TIME_KIND_LAST_SEARCH_START );

				/* 検索中フラグを立てる */
				S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_SEARCHING_FILE;
			}
		}
	}

}
/* コールバック関数 */
/****************************************************************************/
/* 関数名称		:	sd_sim_SearchSimFileCB									*/
/* 機能の説明	:	シミュレーションファイル検索コールバック関数			*/
/* 引数			:	(I/ )retinfo	処理結果								*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/10/19												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_sim_SearchSimFileCB( const T_COMMON_SRC_VDVF_RET *retinfo )
{

	if( NULL != retinfo ) {
		if( N_TRUE == retinfo->exist ) {
			/* パスが取得できた */
			(void)strcpy( (CH_8*)S_sd_sim_ctrl_info.sim_file_path, (const CH_8*)retinfo->filepath );
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_FOUND_SIM_FILE;

			OUTLOG(OutputLog("[SD] found path=[%s]\n", S_sd_sim_ctrl_info.sim_file_path);)
		}
		/* 検索中フラグOFF */
		S_sd_sim_ctrl_info.flags &= (~SD_SIM_FLAG_SEARCHING_FILE);
	}
	return;

}

/****************************************************************************/
/* 関数名称		:	sd_sim_ReadSimFile										*/
/* 機能の説明	:	シミュレーションファイル周回読み（開始）				*/
/* 引数			:	void													*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_sim_ReadSimFile( void )
{

	E_FIO_RESULT	res;
	UI_32			reqno = 0UL;
	UI_32			ofs = 0UL;
	UI_16			block_size = 0U;
	UI_8*			block = NULL;
	B_8				bret = N_FALSE;

	if( 0 != (S_sd_sim_ctrl_info.flags & SD_SIM_FLAG_FOUND_SIM_FILE) ) {
		/* シミュレーションファイル発見済 */
		if( 0 == (S_sd_sim_ctrl_info.flags & SD_SIM_FLAG_READING_FILE) ) {
			/*==============================================================*/
			/* READ中ではない												*/
			/*==============================================================*/
			if( 0 != (S_sd_sim_ctrl_info.flags & SD_SIM_FLAG_REQ_REWIND ) ) {
				/*==========================================================*/
				/* 頭出し要求あり											*/
				/*==========================================================*/
				S_sd_sim_ctrl_info.flags &= (~SD_SIM_FLAG_REQ_REWIND);

				/* 受信バッファクリア */
				(void)sd_sim_data_ReadRecvData( sd_sim_data_GetRecvDataSize(), NULL, N_TRUE );

				/* ファイルの頭出し（固定部の直後まで戻す）*/
				S_sd_sim_ctrl_info.sim_file_read_size = sd_sim_signal_GetFixPartSize();

				/* 各種受信済み情報クリア(固定部読み込み済みを考慮) */
				S_sd_sim_ctrl_info.flags &= ~(SD_SIM_FLAG_CHANGE_VAR_ID_LIST );
			}
			/* シミュレーションデータ読みブロック取得 */
			bret = sd_sim_GetReadBlock( &block, &block_size );
			if( (N_TRUE == bret) && (NULL != block) && (0U < block_size) ) {
				/*==========================================================*/
				/* 読み込み可能な空きブロックがある							*/
				/*==========================================================*/
				/* 読み込み済ファイルサイズの続きから読む */
				ofs = S_sd_sim_ctrl_info.sim_file_read_size;
				res = FileIO_Read( S_sd_sim_ctrl_info.sim_file_path, block, ofs, (UI_32)block_size, sd_sim_ReadSimFileCB, &reqno );
				if( FIO_RESULT_OK == res ) {
					/* READ中フラグを立てる */
					S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_READING_FILE;
				}
			}
		}
	}

}
/* コールバック関数 */
/****************************************************************************/
/* 関数名称		:	sd_sim_ReadSimFileCB									*/
/* 機能の説明	:	シミュレーションファイル読み込みコールバック関数		*/
/* 引数			:	(I/ )reqinfo	要求情報								*/
/* 				:	(I/ )retinfo	処理結果								*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_sim_ReadSimFileCB( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo )
{

	UI_32	fix_part_size = 0L;
	UI_16	mask_level;

	if( (NULL != reqinfo) && (NULL != retinfo) ) {
		/* 現在の割り込みレベルを取得しメイン処理(SD制御のメイン処理)の割り込みを禁止にする */
		(void)IntrDrv_GetMaskLevel(&mask_level);
		(void)IntrDrv_SetMaskLevel( MAIN_INTR_LEVEL );

		if( FIO_RESULT_OK == retinfo->result ) {
			/*==============================================================*/
			/* READ成功														*/
			/*==============================================================*/
			if( 0 == (S_sd_sim_ctrl_info.flags & SD_SIM_FLAG_GET_FILE_SIZE) ) {
				/* シミュレーションファイルサイズ未取得なら取得 */
				S_sd_sim_ctrl_info.sim_file_size = retinfo->fsize;
				S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_GET_FILE_SIZE;
			}
			/* 書込み済サイズ更新（通算） */
			S_sd_sim_ctrl_info.sim_total_write_size += retinfo->bytes;

			/* READ済サイズ更新（このファイル） */
			S_sd_sim_ctrl_info.sim_file_read_size += retinfo->bytes;
			if( S_sd_sim_ctrl_info.sim_file_read_size >= S_sd_sim_ctrl_info.sim_file_size ) {
				fix_part_size = sd_sim_signal_GetFixPartSize();
				if( 0 < fix_part_size ) {
					/* ファイル終端に達したら固定部の直後まで戻す */
					S_sd_sim_ctrl_info.sim_file_read_size = fix_part_size;

					/* 各種受信済み情報クリア(固定部読み込み済みを考慮) */
					S_sd_sim_ctrl_info.flags &= ~(SD_SIM_FLAG_CHANGE_VAR_ID_LIST );
				}
				else {
					/* ファイル終端に達したら先頭に戻す */
					S_sd_sim_ctrl_info.sim_file_read_size = 0;

					/* 各種受信済み情報クリア */
					S_sd_sim_ctrl_info.flags &= ~(SD_SIM_FLAG_RECV_HEADER | SD_SIM_FLAG_RECV_FIX_ID_LIST | SD_SIM_FLAG_RECV_FIX_VALUES | SD_SIM_FLAG_RECV_VAR_ID_LIST | SD_SIM_FLAG_RECV_VAR_VALUES | SD_SIM_FLAG_CHANGE_VAR_ID_LIST );
				}
			}
		}
		else {
			/*==============================================================*/
			/* READエラー													*/
			/*==============================================================*/
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_READ_ERROR;
		}
		/* READ中フラグを落とす */
		S_sd_sim_ctrl_info.flags &= (~SD_SIM_FLAG_READING_FILE);

		/* 割り込みレベルを復帰する */
		(void)IntrDrv_SetMaskLevel( mask_level );
	}

}
/****************************************************************************/
/* 関数名称		:	sd_sim_GetReadBlock										*/
/* 機能の説明	:	シミュレーションデータ読みブロック取得					*/
/* 引数			:	( /O)block		読み込みブロック先頭アドレス			*/
/*					( /O)block_size	読み込みブロックサイズ					*/
/* 戻り値		:	N_TRUE: 正常／N_FALSE: 異常								*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static B_8 sd_sim_GetReadBlock( UI_8** block, UI_16* block_size )
{

	UI_16	block_idx = 0U;
	UI_16	last_block_size = 0U;
	UI_16	temp_size = 0U;
	B_8		bret = N_FALSE;
	UI_16	empty_size = 0U;

	if( (NULL != block) && (NULL != block_size) ) {
		/* 結果を初期化 */
		*block = NULL;
		*block_size = 0U;

		/* 空きサイズを取得 */
		empty_size = sizeof(S_sd_rec_data_buff) - sd_sim_data_GetRecvDataSize();

		/* 現在書き込まれている終端位置を計算 */
		block_idx = (UI_16)((S_sd_sim_ctrl_info.sim_total_write_size / (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE) % (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_NUM);
		last_block_size = (UI_16)(S_sd_sim_ctrl_info.sim_total_write_size % (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE);

		if( 0U == last_block_size ) {
			/* ブロックサイズきっちり読みこまれている場合 */
			if( SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE <= empty_size ) {
				/* １ブロック空いている */
				*block = (S_sd_rec_data_buff + (SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE * block_idx));
				*block_size = SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE;
			}
		}
		else {
			/* あまりブロックがある場合ブロックの残りを埋める */
			temp_size = SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE - last_block_size;
			if( temp_size <= empty_size ) {
				/* ブロック残り分空いている */
				*block = (S_sd_rec_data_buff + ((SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE * block_idx) + last_block_size));
				*block_size = temp_size;
			}
		}
		bret = N_TRUE;
	}
	return bret;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_RegTime											*/
/* 機能の説明	:	時間登録												*/
/* 引数			:	(I/ )kind	時間種別									*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_sim_RegTime( E_SD_SIM_TIME_KIND kind )
{

	UI_32	flag = 1UL << kind;

	if( SD_SIM_TIME_KIND_COUNT > kind ) {
		S_sd_sim_ctrl_info.time[kind] = Timer_ctrl_GetTime1ms();
		S_sd_sim_ctrl_info.time_flags |= flag;
	}

}
/****************************************************************************/
/* 関数名称		:	sd_sim_ClearTime										*/
/* 機能の説明	:	時間クリア												*/
/* 引数			:	(I/ )kind	時間種別									*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_sim_ClearTime( E_SD_SIM_TIME_KIND kind )
{

	UI_32	flag = 1UL << kind;

	if( SD_SIM_TIME_KIND_COUNT > kind ) {
		S_sd_sim_ctrl_info.time_flags &= (~flag);
	}

}
/****************************************************************************/
/* 関数名称		:	sd_sim_CheckTimeout										*/
/* 機能の説明	:	タイムアウトチェック									*/
/* 引数			:	(I/ )kind	時間種別									*/
/*					(I/ )msec	タイムアウト時間(msec)						*/
/* 戻り値		:	1	タイムアウト時間経過								*/
/*					0	タイムアウト時間経過していない						*/
/*					-1	時間登録されていない								*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static SI_32 sd_sim_CheckTimeout( E_SD_SIM_TIME_KIND kind, UI_32 msec )
{

	SI_32	ret = -1;
	UI_32	flag = 1UL << kind;
	UI_32	now = 0UL;

	if( SD_SIM_TIME_KIND_COUNT > kind ) {
		if( 0 != (S_sd_sim_ctrl_info.time_flags & flag) ) {
			now = Timer_ctrl_GetTime1ms();
			if( (now - S_sd_sim_ctrl_info.time[kind]) >= msec ) {
				/* タイムアウト検出 */
				ret = 1L;
			}
			else {
				/* タイムアウトなし */
				ret = 0L;
			}
		}
	}
	return ret;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_ClearReadError									*/
/* 機能の説明	:	リードエラークリア										*/
/* 引数			:	void													*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_sim_ClearReadError( void )
{

	S_sd_sim_ctrl_info.flags &= (~SD_SIM_FLAG_READ_ERROR);

}
/*==========================================================================*/
/* 状態関数(シミュレーション)												*/
/*==========================================================================*/
/****************************************************************************/
/* 関数名称		:	sd_sim_stat_None										*/
/* 機能の説明	:	状態関数	SD_SIM_STAT_NONE							*/
/* 引数			:	(I/ )sim_event	シミュレーションイベント				*/
/* 				:	( /O)fcontinue	処理継続フラグ							*/
/* 戻り値		:	SD_SIM_STAT_XXXX	遷移先状態							*/
/*					SD_SIM_STAT_NONE	状態遷移なし						*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_None( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;

	if( NULL != fcontinue ) {
		*fcontinue = N_FALSE;
	}

	return next_stat;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_stat_SearchSimFile								*/
/* 機能の説明	:	状態関数	SD_SIM_STAT_SEARCH_SIM_FILE					*/
/* 引数			:	(I/ )sim_event	シミュレーションイベント				*/
/* 				:	( /O)fcontinue	処理継続フラグ							*/
/* 戻り値		:	SD_SIM_STAT_XXXX	遷移先状態							*/
/*					SD_SIM_STAT_NONE	状態遷移なし						*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_SearchSimFile( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_FOUND_SIM_FILE & sim_event) ) {
			/* SIMファイル検索完了 */

			next_stat = SD_SIM_STAT_WAIT_HEADER;
		}
		else if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* ファイル読みエラー発生 */

			/* READエラークリア */
			sd_sim_ClearReadError();
		}
		else if( 0 != (SD_SIM_EVENT_ACC_OFF & sim_event) ) {
			/* ACC OFF */

			next_stat = SD_SIM_STAT_WAIT_STOP_ACC_OFF;
		}
		else {
			/* 何もしない */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_stat_WaitHeader									*/
/* 機能の説明	:	状態関数	SD_SIM_STAT_WAIT_HEADER						*/
/* 引数			:	(I/ )sim_event	シミュレーションイベント				*/
/* 				:	( /O)fcontinue	処理継続フラグ							*/
/* 戻り値		:	SD_SIM_STAT_XXXX	遷移先状態							*/
/*					SD_SIM_STAT_NONE	状態遷移なし						*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_WaitHeader( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* ファイル読みエラー発生 */

			/* READエラークリア */
			sd_sim_ClearReadError();

			next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_HEADER & sim_event) ) {
			/* ファイルヘッダ受信 */
			
			/* ヘッダ解析 */
			(void)sd_sim_signal_AnalyzeHeader( &S_sd_sim_ctrl_info.packet_info );
			/* パケットクリア */
			sd_sim_data_ClearRecvPacket( &S_sd_sim_ctrl_info.packet_info );

			/* フォーマット解析は連続実行で高速処理 */
			*fcontinue = N_TRUE;

			next_stat = SD_SIM_STAT_WAIT_FIX_ID_LIST;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_ID_LIST & sim_event) ) {
			/* 固定ID情報受信 */

			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_VALUES & sim_event) ) {
			/* 固定車両情報受信 */

			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_ID_LIST & sim_event) ) {
			/* 変数ID情報受信 */

			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_VALUES & sim_event) ) {
			/* 変数車両情報受信 */

			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_ACC_OFF & sim_event) ) {
			/* ACC OFF */

			next_stat = SD_SIM_STAT_WAIT_STOP_ACC_OFF;
		}
		else {
			/* 何もしない */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_stat_WaitFixIdList								*/
/* 機能の説明	:	状態関数	SD_SIM_STAT_WAIT_FIX_ID_LIST				*/
/* 引数			:	(I/ )sim_event	シミュレーションイベント				*/
/* 				:	( /O)fcontinue	処理継続フラグ							*/
/* 戻り値		:	SD_SIM_STAT_XXXX	遷移先状態							*/
/*					SD_SIM_STAT_NONE	状態遷移なし						*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_WaitFixIdList( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* ファイル読みエラー発生 */

			/* READエラークリア */
			sd_sim_ClearReadError();

			next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_HEADER & sim_event) ) {
			/* ファイルヘッダ受信 */
			
			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_ID_LIST & sim_event) ) {
			/* 固定ID情報受信 */

			/* 固定信号ID格納 */
			(void)sd_sim_signal_SetIDList( N_TRUE, &S_sd_sim_ctrl_info.packet_info );
			/* パケットクリア */
			sd_sim_data_ClearRecvPacket( &S_sd_sim_ctrl_info.packet_info );

			/* フォーマット解析は連続実行で高速処理 */
			*fcontinue = N_TRUE;

			next_stat = SD_SIM_STAT_WAIT_FIX_VALUES;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_VALUES & sim_event) ) {
			/* 固定車両情報受信 */

			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_ID_LIST & sim_event) ) {
			/* 変数ID情報受信 */

			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_VALUES & sim_event) ) {
			/* 変数車両情報受信 */

			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_ACC_OFF & sim_event) ) {
			/* ACC OFF */

			next_stat = SD_SIM_STAT_WAIT_STOP_ACC_OFF;
		}
		else {
			/* 何もしない */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_stat_WaitFixValues								*/
/* 機能の説明	:	状態関数	SD_SIM_STAT_WAIT_FIX_VALUES					*/
/* 引数			:	(I/ )sim_event	シミュレーションイベント				*/
/* 				:	( /O)fcontinue	処理継続フラグ							*/
/* 戻り値		:	SD_SIM_STAT_XXXX	遷移先状態							*/
/*					SD_SIM_STAT_NONE	状態遷移なし						*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_WaitFixValues( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* ファイル読みエラー発生 */

			/* READエラークリア */
			sd_sim_ClearReadError();

			next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_HEADER & sim_event) ) {
			/* ファイルヘッダ受信 */
			
			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_ID_LIST & sim_event) ) {
			/* 固定ID情報受信 */

			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_VALUES & sim_event) ) {
			/* 固定車両情報受信 */

			/* 固定車両情報格納 */
			(void)sd_sim_signal_SetValues( N_TRUE, &S_sd_sim_ctrl_info.packet_info );
			/* パケットクリア */
			sd_sim_data_ClearRecvPacket( &S_sd_sim_ctrl_info.packet_info );

			/* フォーマット解析は連続実行で高速処理 */
			*fcontinue = N_TRUE;

			next_stat = SD_SIM_STAT_WAIT_VAR_ID_LIST;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_ID_LIST & sim_event) ) {
			/* 変数ID情報受信 */

			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_VALUES & sim_event) ) {
			/* 変数車両情報受信 */

			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_ACC_OFF & sim_event) ) {
			/* ACC OFF */

			next_stat = SD_SIM_STAT_WAIT_STOP_ACC_OFF;
		}
		else {
			/* 何もしない */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_stat_WaitVarIdList								*/
/* 機能の説明	:	状態関数	SD_SIM_STAT_WAIT_VAR_ID_LIST				*/
/* 引数			:	(I/ )sim_event	シミュレーションイベント				*/
/* 				:	( /O)fcontinue	処理継続フラグ							*/
/* 戻り値		:	SD_SIM_STAT_XXXX	遷移先状態							*/
/*					SD_SIM_STAT_NONE	状態遷移なし						*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_WaitVarIdList( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* ファイル読みエラー発生 */

			/* READエラークリア */
			sd_sim_ClearReadError();

			next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_HEADER & sim_event) ) {
			/* ファイルヘッダ受信 */
			
			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_ID_LIST & sim_event) ) {
			/* 固定ID情報受信 */

			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_VALUES & sim_event) ) {
			/* 固定車両情報受信 */

			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_ID_LIST & sim_event) ) {
			/* 変数ID情報受信 */

			/* 変数信号ID格納 */
			(void)sd_sim_signal_SetIDList( N_FALSE, &S_sd_sim_ctrl_info.packet_info );
			/* パケットクリア */
			sd_sim_data_ClearRecvPacket( &S_sd_sim_ctrl_info.packet_info );

			/* フォーマット解析は連続実行で高速処理 */
			*fcontinue = N_TRUE;

			next_stat = SD_SIM_STAT_WAIT_VAR_VALUES;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_VALUES & sim_event) ) {
			/* 変数車両情報受信 */

			/* フォーマット不正のため停止を待って最初からやりなおし */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_ACC_OFF & sim_event) ) {
			/* ACC OFF */

			next_stat = SD_SIM_STAT_WAIT_STOP_ACC_OFF;
		}
		else {
			/* 何もしない */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_stat_WaitVarValues								*/
/* 機能の説明	:	状態関数	SD_SIM_STAT_WAIT_VAR_VALUES					*/
/* 引数			:	(I/ )sim_event	シミュレーションイベント				*/
/* 				:	( /O)fcontinue	処理継続フラグ							*/
/* 戻り値		:	SD_SIM_STAT_XXXX	遷移先状態							*/
/*					SD_SIM_STAT_NONE	状態遷移なし						*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_WaitVarValues( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* ファイル読みエラー発生 */

			/* READエラークリア */
			sd_sim_ClearReadError();

			next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_HEADER & sim_event) ) {
			/* ファイルヘッダ受信 */
			
			/* パケットクリア */
			OUTLOG(OutputLog("[SD] SKIP HEADER(%d)\n", S_sd_sim_ctrl_info.packet_info.packet_bytes);)
			sd_sim_data_ClearRecvPacket(&S_sd_sim_ctrl_info.packet_info);

			/* 受信情報だけ保持 */
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_HEADER;

			/* 無視してスキップ */
			*fcontinue = N_TRUE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_ID_LIST & sim_event) ) {
			/* 固定ID情報受信 */

			/* パケットクリア */
			OUTLOG(OutputLog("[SD] SKIP FIX ID LIST(%d)\n", S_sd_sim_ctrl_info.packet_info.packet_bytes);)
			sd_sim_data_ClearRecvPacket(&S_sd_sim_ctrl_info.packet_info);

			/* 受信情報だけ保持 */
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_FIX_ID_LIST;

			/* 無視してスキップ */
			*fcontinue = N_TRUE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_VALUES & sim_event) ) {
			/* 固定車両情報受信 */

			/* パケットクリア */
			OUTLOG(OutputLog("[SD] SKIP FIX VALUES(%d)\n", S_sd_sim_ctrl_info.packet_info.packet_bytes);)
			sd_sim_data_ClearRecvPacket(&S_sd_sim_ctrl_info.packet_info);

			/* 受信情報だけ保持 */
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_FIX_VALUES;

			/* 無視してスキップ */
			*fcontinue = N_TRUE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_ID_LIST & sim_event) ) {
			/* 変数ID情報受信 */

			/* 変数信号ID格納 */
			(void)sd_sim_signal_SetIDList( N_FALSE, &S_sd_sim_ctrl_info.packet_info );
			/* 変数IDリストの変更を記録 */
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_CHANGE_VAR_ID_LIST;

			/* パケットクリア */
			sd_sim_data_ClearRecvPacket( &S_sd_sim_ctrl_info.packet_info );
			/* 変数車両情報バッファクリア */
			sd_sim_signal_ClearBuffer( N_FALSE );

			/* フォーマット解析は連続実行で高速処理 */
			*fcontinue = N_TRUE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_VALUES & sim_event) ) {
			/* 変数車両情報受信 */

			/* 変数車両情報格納 */
			(void)sd_sim_signal_SetValues( N_FALSE, &S_sd_sim_ctrl_info.packet_info );
			/* パケットクリア */
			sd_sim_data_ClearRecvPacket( &S_sd_sim_ctrl_info.packet_info );
		}
		else if( 0 != (SD_SIM_EVENT_REQ_REWIND & sim_event) ) {
			/* シミュレーション頭出し要求 */

			/* 頭出し要求フラグを記録 */
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_REQ_REWIND;
		}
		else if( 0 != (SD_SIM_EVENT_ACC_OFF & sim_event) ) {
			/* ACC OFF */

			next_stat = SD_SIM_STAT_WAIT_STOP_ACC_OFF;
		}
		else {
			/* 何もしない */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_stat_WaitStop									*/
/* 機能の説明	:	状態関数	SD_SIM_STAT_WAIT_STOP						*/
/* 引数			:	(I/ )sim_event	シミュレーションイベント				*/
/* 				:	( /O)fcontinue	処理継続フラグ							*/
/* 戻り値		:	SD_SIM_STAT_XXXX	遷移先状態							*/
/*					SD_SIM_STAT_NONE	状態遷移なし						*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_WaitStop( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* ファイル読みエラー発生 */

			/* READエラークリア */
			sd_sim_ClearReadError();

			next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;
		}
		else if( 0 != (SD_SIM_EVENT_END_READ & sim_event) ) {
			/* ファイル読み終了 */

			/* ファイル名決定から再開 */
			next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;
		}
		else if( 0 != (SD_SIM_EVENT_ACC_OFF & sim_event) ) {
			/* ACC OFF */

			next_stat = SD_SIM_STAT_WAIT_STOP_ACC_OFF;
		}
		else {
			/* 何もしない */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_stat_WaitStopAccOff								*/
/* 機能の説明	:	状態関数	SD_SIM_STAT_WAIT_STOP_ACC_OFF				*/
/* 引数			:	(I/ )sim_event	シミュレーションイベント				*/
/* 				:	( /O)fcontinue	処理継続フラグ							*/
/* 戻り値		:	SD_SIM_STAT_XXXX	遷移先状態							*/
/*					SD_SIM_STAT_NONE	状態遷移なし						*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_WaitStopAccOff( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* ファイル読みエラー発生 */

			/* READエラークリア */
			sd_sim_ClearReadError();

			next_stat = SD_SIM_STAT_ACC_OFF;
		}
		else if( 0 != (SD_SIM_EVENT_END_READ & sim_event) ) {
			/* ファイル読み終了 */

			next_stat = SD_SIM_STAT_ACC_OFF;
		}
		else {
			/* 何もしない */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_stat_AccOff										*/
/* 機能の説明	:	状態関数	SD_SIM_STAT_ACC_OFF							*/
/* 引数			:	(I/ )sim_event	シミュレーションイベント				*/
/* 				:	( /O)fcontinue	処理継続フラグ							*/
/* 戻り値		:	SD_SIM_STAT_XXXX	遷移先状態							*/
/*					SD_SIM_STAT_NONE	状態遷移なし						*/
/* 作成者		:	(OFT)T.Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_AccOff( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

	}
	return next_stat;

}
/*==========================================================================*/
/* 受信データ解析関数														*/
/*==========================================================================*/
/****************************************************************************/
/* 関数名称		:	sd_sim_data_AnalyzeRecvData								*/
/* 機能の説明	:	受信データ解析											*/
/* 引数			:	( /O)packet_info	解析で得たパケットの情報			*/
/* 戻り値		:	E_RC	処理結果										*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/03												*/
/* 備考			:	受信データない場合 SD_CTRL_FMT_TYPE_INVALID を出力する	*/
/****************************************************************************/
static E_RC sd_sim_data_AnalyzeRecvData( T_SD_CTRL_PACKET_INFO* packet_info )
{

	E_RC	rc = E_RC_OK;
	SI_32	ret;

	if( NULL == packet_info ) {
		rc = E_RC_NG_PARAM;
	}
	else {
		/* パケット情報を無効値で初期化 */
		(void)memset( packet_info, 0, sizeof(T_SD_CTRL_PACKET_INFO) );
		packet_info->type = SD_CTRL_FMT_TYPE_INVALID;

		/* 受信バッファを START マークまで読み飛ばす */
		ret = sd_sim_data_SkipToSTART();
		/* 1: START あり */
		/* 0: START なし */
		while( 1L == ret ) {
			/* パケット情報を１個取り出す */
			ret = sd_sim_data_GetPacketInfo( packet_info );
			if( 1L == ret ) {
				/* 1: パケットあり */
				break;
			}
			else if( 0L == ret ) {
				/* 0: パケットではない（異常パケットも含む） */
				/* STARTマークを飛ばして次の解析 */
				(void)sd_sim_data_ReadRecvData( 1U, NULL, N_TRUE ); 
			}
			else {
				/* -1: データ不足で判定不能 */
				rc = E_RC_NG;
				break;
			}
			/* 受信バッファを START マークまで読み飛ばす */
			ret = sd_sim_data_SkipToSTART();
		}
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
/* 作成日		:	16/10/03												*/
/* 備考			:															*/
/****************************************************************************/
static SI_32 sd_sim_data_SkipToSTART( void )
{

	SI_32	ret = 0L;
	UI_16	data_bytes = 0L;
	UI_16	skip_bytes = 0L;
	UI_8	buf[16];	/* 適当なブロックサイズで処理 */
	UI_16	buf_bytes =	sizeof(buf);

	/* 残りデータバイト数取得 */
	data_bytes = sd_sim_data_GetRecvDataSize();
	while( 0U < data_bytes ) {
		if( data_bytes > buf_bytes ) {
			data_bytes = buf_bytes;
		}
		/* バッファ参照 */
		(void)sd_sim_data_ReadRecvData( data_bytes, buf, N_FALSE );

		/* START マークを探す */
		skip_bytes = 0U;
		while( skip_bytes < data_bytes ) {
			if( SD_CTRL_FMT_HEADER_START == buf[skip_bytes] ) {
				/* 発見 */
				ret = 1L;
				break;
			}
			skip_bytes++;
		}
		if( 0U < skip_bytes ) {
			/* みつからなかったデータをバッファから捨てる */
			(void)sd_sim_data_ReadRecvData( skip_bytes, NULL, N_TRUE );
		}
		if( 1L == ret ) {
			/* ループを抜けて終了 */
			break;
		}
		/* 残りデータバイト数取得 */
		data_bytes = sd_sim_data_GetRecvDataSize();
	}
	return ret;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_data_ClearRecvPacket								*/
/* 機能の説明	:	受信バッファを解析してパケット情報を得る				*/
/* 引数			:	(I/O)packet_info	解析で得たパケットの情報			*/
/* 戻り値		:	1:  パケットあり										*/
/* 戻り値		:	0:  パケットではない（異常パケットも含む）				*/
/* 戻り値		:	-1: データ不足で判定不能								*/
/* 戻り値		:	-2:	引数不正											*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/03												*/
/* 備考			:	パケットがない場合 SD_CTRL_FMT_TYPE_INVALID を出力する	*/
/****************************************************************************/
static void sd_sim_data_ClearRecvPacket( T_SD_CTRL_PACKET_INFO* packet_info )
{

	if( NULL != packet_info ) {
		/* 受信パケットをバッファから取り除く */
		(void)sd_sim_data_ReadRecvData( packet_info->packet_bytes, NULL, N_TRUE );
	
		(void)memset(packet_info, 0, sizeof(T_SD_CTRL_PACKET_INFO));
	}

}
/****************************************************************************/
/* 関数名称		:	sd_sim_data_GetPacketInfo								*/
/* 機能の説明	:	受信バッファを解析してパケット情報を得る				*/
/* 引数			:	( /O)packet_info	解析で得たパケットの情報			*/
/* 戻り値		:	1:  パケットあり										*/
/* 戻り値		:	0:  パケットではない（異常パケットも含む）				*/
/* 戻り値		:	-1: データ不足で判定不能								*/
/* 戻り値		:	-2:	引数不正											*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/03												*/
/* 備考			:	パケットがない場合 SD_CTRL_FMT_TYPE_INVALID を出力する	*/
/****************************************************************************/
static SI_32 sd_sim_data_GetPacketInfo( T_SD_CTRL_PACKET_INFO* packet_info )
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
		(void)memset( packet_info, 0, sizeof(T_SD_CTRL_PACKET_INFO) );
		packet_info->type = SD_CTRL_FMT_TYPE_INVALID;

		/* リングバッファのデータを参照 */
		data_bytes = sd_sim_data_GetRecvDataSize();
		if( data_bytes > buf_bytes ) {
			/* パケットバッファの最大数分までデータ参照する */
			data_bytes = buf_bytes;
		}
		if( SD_CTRL_FMT_BYTES_FIX > data_bytes ) {
			/* 固定部分のサイズにも満たない */
			ret = -1L;
			goto END_ANALYZE;
		}
		(void)sd_sim_data_ReadRecvData( data_bytes, packet_info->packet, N_FALSE );

		/* パケット解析 */
		p = packet_info->packet;

		/*==================================================================*/
		/* ヘッダの解析														*/
		/*==================================================================*/
		header_part = p;
		/* START */
		if( SD_CTRL_FMT_HEADER_START != *p ) {
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
		packet_bytes = SD_CTRL_FMT_BYTES_FIX + length - 1U;
		if( packet_bytes > data_bytes ) {
			/* パケット全体のサイズに満たない */
			ret = -1L;
			goto END_ANALYZE;
		}
		/*==================================================================*/
		/* データの解析														*/
		/*==================================================================*/
		switch( type ) {
		case SD_CTRL_FMT_TYPE_HEADER:			/*	車両信号一覧（SDカード用）	*/
			/* データ部チェック */
			/* 省略 */
			break;
		case SD_CTRL_FMT_TYPE_ID_INFO_DATA:		/*	車両情報ID（SDカード用）	*/
			/* データ部チェック */
			/* 省略 */
			break;
		case SD_CTRL_FMT_TYPE_SIGNAL_DATA:		/*	車両情報（SDカード用）		*/
			/* データ部チェック */
			/* 省略 */
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
		if( SD_CTRL_FMT_FOOTER_END != *p ) {
			goto END_ANALYZE;
		}
		p++;

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
/* 関数名称		:	sd_sim_data_GetRecvDataSize								*/
/* 機能の説明	:	受信バッファにあるデータのサイズを取得する				*/
/* 引数			:	void													*/
/* 戻り値		:	受信データバイト数										*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/03												*/
/* 備考			:															*/
/****************************************************************************/
static UI_16 sd_sim_data_GetRecvDataSize( void )
{

	UI_32	recv_data_size;
	UI_32	buff_size = sizeof(S_sd_rec_data_buff);

	recv_data_size = S_sd_sim_ctrl_info.sim_total_write_size - S_sd_sim_ctrl_info.sim_total_read_size;
	if( recv_data_size > buff_size ) {
		/* ありえない:フェールセーフ */
		recv_data_size = buff_size;
	}
	return (UI_16)recv_data_size;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_data_ReadRecvData								*/
/* 機能の説明	:	受信バッファデータ読み									*/
/* 引数			:	(I/ )size	読み込みサイズ								*/
/*					( /O)buff	読み込みバッファ	NULL可					*/
/*					(I/ )fclear	取得したデータをクリアするか？				*/
/* 戻り値		:	E_RC_OK			size分取得成功							*/
/*					E_RC_NG_PARAM	パラメタ不正（size分不足時も）			*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/03												*/
/* 備考			:															*/
/****************************************************************************/
static E_RC sd_sim_data_ReadRecvData( UI_16 size, UI_8* buff, UI_8 fclear )
{

	E_RC	rc = E_RC_NG;
	UI_32	read_pos = 0UL;
	UI_32	rest_size = 0UL;
	UI_32	buff_size = sizeof(S_sd_rec_data_buff);

	if( 0 == size ) {
		rc = E_RC_NG_PARAM;
	}
	else if( size > sd_sim_data_GetRecvDataSize() ) {
		rc = E_RC_NG_PARAM;
	}
	else {
		/* 読み出し開始位置を計算 */
		read_pos = S_sd_sim_ctrl_info.sim_total_read_size % buff_size;
		
		if( NULL != buff ) {
			/* 実際に読み出す場合 */
			rest_size = buff_size - read_pos;
			if( size <= rest_size ) {
				/* まとめ読み可能 */
				(void)memcpy( buff, &S_sd_rec_data_buff[read_pos], size );
			}
			else {
				/* 折り返し読み必要 */
				(void)memcpy( buff, &S_sd_rec_data_buff[read_pos], (size_t)rest_size );
				(void)memcpy( buff + rest_size, S_sd_rec_data_buff, (size_t)(size - rest_size) );
			}
		}
		if( N_TRUE == fclear ) {
			/* バッファクリアする場合 */
			S_sd_sim_ctrl_info.sim_total_read_size += size;
		}
		rc = E_RC_OK;
	}
	return rc;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_data_GetIDList									*/
/* 機能の説明	:	IDリストをパケットから抽出								*/
/* 引数			:	(I/ )packet_info	パケット情報						*/
/*					( /O)idList		IDリスト	NULL可						*/
/*					(I/ )max_idNum	最大ID数								*/
/* 戻り値		:	0 <=	抽出したID数									*/
/*					-1		以上（引数不正／フォーマット不正）				*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static SI_32 sd_sim_data_GetIDList( T_SD_CTRL_PACKET_INFO* packet_info, UI_16* idList, UI_8 max_idNum )
{

	SI_32	count = -1;
	UI_8	idNum = 0U;
	UI_8	i = 0U;
	UI_8*	p = NULL;

	if( NULL != packet_info ) {
		if( SD_CTRL_FMT_TYPE_ID_INFO_DATA == packet_info->type ) {
			/* ID数を取得 */
			idNum = *packet_info->data_part;
			p = packet_info->data_part + 1;
			if( idNum <= max_idNum ) {
				if( NULL != idList ) {
					/* IDリストを取得 */
					for( i = 0; i < idNum; i++ ) {
						idList[i] = Common_Get16( p );
						p += 2;
					}
				}
				count = (SI_32)idNum;
			}
		}
	}
	return count;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_data_GetValues									*/
/* 機能の説明	:	信号配列をパケットから抽出								*/
/* 引数			:	(I/ )packet_info		パケット情報					*/
/*					( /O)values			信号配列	NULL可					*/
/*					(I/ )max_valueNum	最大信号数							*/
/* 戻り値		:	0 <=	抽出した信号数									*/
/*					-1		以上（引数不正／フォーマット不正）				*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static SI_32 sd_sim_data_GetValues( T_SD_CTRL_PACKET_INFO* packet_info, SI_32* values, UI_8 max_valueNum )
{

	SI_32	count = -1;
	UI_8	errorNum = 0U;
	UI_8	valueNum = 0U;
	UI_8	i = 0U;
	UI_8*	p = NULL;

	if( NULL != packet_info ) {
		if( SD_CTRL_FMT_TYPE_SIGNAL_DATA == packet_info->type ) {
			/* エラーコード数から参照 */
			p = packet_info->data_part + 4;
			errorNum = *p;
			p++;
			if( SD_CTRL_MAX_ERRCODE_COUNT >= errorNum ) {
				/* エラーコードを読み飛ばす */
				p += (6 * errorNum);

				/* 信号数を取得 */
				valueNum = *p;
				p++;
				if( valueNum <= max_valueNum ) {
					if( NULL != values ) {
						/* 信号配列を取得 */
						for( i = 0; i < valueNum; i++ ) {
							values[i] = (SI_32)Common_Get32( p );
							p += 4;
						}
					}
					count = (SI_32)valueNum;
				}
			}
		}
	}
	return count;

}
/*==========================================================================*/
/* 信号データ操作															*/
/*==========================================================================*/
/****************************************************************************/
/* 関数名称		:	sd_sim_signal_ClearBuffer								*/
/* 機能の説明	:	信号バッファクリア										*/
/* 引数			:	(I/ )clear_all	N_TRUE:全信号クリア／N_FALSE:変数信号のみクリア	*/
/* 戻り値		:	void													*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/03												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_sim_signal_ClearBuffer( B_8 clear_all )
{

	UI_8	fixIdNum;

	if( N_TRUE == clear_all ) {
		/* 全信号クリア */
		(void)memset(S_sd_signal_buff, 0xFF, sizeof(S_sd_signal_buff));
	}
	else {
		/* 変数信号のみクリア */
		fixIdNum = S_sd_sim_id_info.fixIdNum;
		(void)memset(S_sd_signal_buff + fixIdNum, 0xFF, sizeof(S_sd_signal_buff)-(fixIdNum * sizeof(SI_32)));
	}

}
/****************************************************************************/
/* 関数名称		:	sd_sim_signal_AnalyzeHeader								*/
/* 機能の説明	:	ヘッダ解析												*/
/* 引数			:	(I/ )packet_info	パケット情報（ファイルヘッダ）		*/
/* 戻り値		:	0 	正常												*/
/*					-1	パラメタ不正										*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static SI_32 sd_sim_signal_AnalyzeHeader( const T_SD_CTRL_PACKET_INFO* packet_info )
{

	SI_32	ret = -1;
	UI_8	varIdNum;
	UI_8	fixIdNum;
	UI_8	fixIdNumIndex;
	UI_8	varIdNumIndex;
	UI_8	randomValIndex;
	UI_8	signalListVerIndex;

	if( NULL != packet_info ) {
		if( SD_CTRL_FMT_TYPE_HEADER == packet_info->type ) {
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_HEADER;
			S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_HEADER] = packet_info->packet_bytes;
			signalListVerIndex = 21;
			/*信号リストバージョン保持*/
			S_sd_sim_id_info.signalListVersion = Common_Get24( &packet_info->data_part[signalListVerIndex] );
			/*固定ID数インデックス*/
			fixIdNumIndex = 37;
			fixIdNum = packet_info->data_part[fixIdNumIndex];

			/*変数ID数インデックス*/
			varIdNumIndex = fixIdNumIndex + 1 + (fixIdNum * sizeof(UI_16));
			varIdNum = packet_info->data_part[varIdNumIndex];
			
			/*乱数インデックス*/
			randomValIndex = varIdNumIndex + 1 + (varIdNum * sizeof(UI_16)) + 40;
			
			/*乱数保持*/
			S_sd_sim_id_info.randomValue = Common_Get32( &packet_info->data_part[randomValIndex] );
			/*【MEMO】
				現状実装では各ID情報リストから抽出したIDリストを
				ID一覧として上位に返しているが、本来はヘッダから取得したものを
				返すようにしたほうがよいかもしれない。
				※現状設計ではどちらも同じ値であるため簡単のためこうしている。
			*/
			ret = 0;
		}
	}
	return ret;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_signal_SetIDList									*/
/* 機能の説明	:	IDリスト設定											*/
/* 引数			:	(I/ )fix			N_TRUE:固定IDリスト／N_FALSE:変数IDリスト	*/
/*					(I/ )packet_info	パケット情報（IDリスト）			*/
/* 戻り値		:	0 <=	セットしたIDの数								*/
/*					-1		パラメタ不正									*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static SI_32 sd_sim_signal_SetIDList( B_8 fix, T_SD_CTRL_PACKET_INFO* packet_info )
{

	SI_32	count = -1;

	if( NULL != packet_info ) {
		if( N_TRUE == fix ) {
			count = sd_sim_data_GetIDList( packet_info, S_sd_sim_id_info.fixIdList, SD_CTRL_MAX_FIX_ID_COUNT );
			if( 0 <= count ) {
				S_sd_sim_id_info.fixIdNum = (UI_8)count;
				S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_FIX_ID_LIST;
				S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_FIX_ID_LIST] = packet_info->packet_bytes;
			}
		}
		else {
			count = sd_sim_data_GetIDList( packet_info, S_sd_sim_id_info.varIdList, SD_CTRL_MAX_VAR_ID_COUNT );
			if( 0 <= count ) {
				S_sd_sim_id_info.varIdNum = (UI_8)count;
				S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_VAR_ID_LIST;
				S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_VAR_ID_LIST] = packet_info->packet_bytes;
			}
		}
	}
	return count;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_signal_SetValues									*/
/* 機能の説明	:	信号配列設定											*/
/* 引数			:	(I/ )fix			N_TRUE:固定信号配列／N_FALSE:変数信号配列	*/
/*					(I/ )packet_info	パケット情報（IDリスト）			*/
/* 戻り値		:	0 <=	セットした値の数								*/
/*					-1		パラメタ不正									*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static SI_32 sd_sim_signal_SetValues( B_8 fix, T_SD_CTRL_PACKET_INFO* packet_info )
{

	SI_32	count = -1;

	if( NULL != packet_info ) {
		if( N_TRUE == fix ) {
			count = sd_sim_data_GetValues( packet_info, S_sd_signal_buff, SD_CTRL_MAX_FIX_ID_COUNT );
			if( 0 <= count ) {
				S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_FIX_VALUES;
				S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_FIX_VALUES] = packet_info->packet_bytes;
			}
		}
		else {
			count = sd_sim_data_GetValues( packet_info, S_sd_signal_buff + S_sd_sim_id_info.fixIdNum, SD_CTRL_MAX_VAR_ID_COUNT );
			if( 0 <= count ) {
				S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_VAR_VALUES;
				S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_VAR_VALUES] = packet_info->packet_bytes;
			}
		}
	}
	return count;

}
/****************************************************************************/
/* 関数名称		:	sd_sim_signal_GetFixPartSize							*/
/* 機能の説明	:	シミュレーションファイル固定部サイズ取得				*/
/* 引数			:	void													*/
/* 戻り値		:	0 <	シミュレーションファイル固定部サイズ				*/
/*					0		取得不可										*/
/* 作成者		:	(OFT)Kamakura											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
/****************************************************************************/
static UI_32 sd_sim_signal_GetFixPartSize( void )
{

	UI_32	mask = SD_SIM_FLAG_RECV_HEADER | SD_SIM_FLAG_RECV_FIX_ID_LIST | SD_SIM_FLAG_RECV_FIX_VALUES | SD_SIM_FLAG_RECV_VAR_ID_LIST;
	UI_32	fix_part_size = 0UL;

	if( mask == (S_sd_sim_ctrl_info.flags & mask) ) {
		/* ヘッダ部＋固定IDリスト＋固定車両情報は確定 */
		fix_part_size = S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_HEADER] + 
			S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_FIX_ID_LIST] + 
			S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_FIX_VALUES];

		if( 0 == (S_sd_sim_ctrl_info.flags & SD_SIM_FLAG_CHANGE_VAR_ID_LIST) ) {
			/* 変数IDリスト変更を検出していないなら変数IDリストまで固定 */
			fix_part_size += S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_VAR_ID_LIST];
		}
	}
	return fix_part_size;

}

#ifdef _VDV_APP
/****************************************************************************/
/* 関数名称		: sd_ctrl_InitInspectionInfo								*/
/* 機能の説明	: SD検査情報初期化											*/
/* 引数			: void														*/
/* 戻り値		: void														*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 17/01/20													*/
/* 備考			: 															*/
/****************************************************************************/
static void sd_ctrl_InitInspectionInfo(void)
{

	(void)memset(&S_sd_inspection_info, 0, sizeof(S_sd_inspection_info));
	S_sd_inspection_info.insp_req_flg = N_OFF;
	S_sd_inspection_info.fio_req_flg = N_OFF;
	S_sd_inspection_info.step = SD_INSPECTION_STEP_INVALID;

	return;

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_Inspect											*/
/* 機能の説明	:	SD検査													*/
/* 引数			:	void													*/
/* 戻り値		: 	N_ON	検査を継続する									*/
/*				: 	N_OFF	検査を継続しない（検査終了）					*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
static UI_8 sd_ctrl_Inspect(void)
{

	E_FIO_RESULT	fioRes;
	UI_32			reqNo;
	UI_8			contflg;		/* 検査継続フラグ N_ON:検査継続  N_OFF:検査終了		*/
	E_INSP_INSPECTION_RESULT	inspectRes;
	T_INSP_INSPECTION_PARAM		inspectParam;
	
	if (S_sd_inspection_info.insp_req_flg == N_OFF) {
		return N_OFF;
	}

	contflg = N_OFF;
	inspectRes = INSP_INSPECTION_RESULT_NG;
	S_sd_inspection_info.fio_req_flg = N_OFF;

	if (S_sd_inspection_info.step == SD_INSPECTION_STEP_MKDIR) {	/* 検査ステップ:検査用ディレクトリ作成	*/

		/* ディレクトリ作成結果をチェック	*/
		if ((S_sd_inspection_info.fio_result == FIO_RESULT_OK) ||
			(S_sd_inspection_info.fio_result == FIO_RESULT_ERR_EXIST)) {
			/* ディテクトリ作成成功(ディレクトリが存在していた場合もOK)	*/

			/* 検査ステップに「書き込み検査」を設定	*/
			S_sd_inspection_info.step = SD_INSPECTION_STEP_WRITE;

			/* SDにシリアルNoを書き込む	*/
			Common_Set16(S_sd_inspection_info.write_buf, S_sd_inspection_info.serialno);
			fioRes = FileIO_Write( FIO_WRITE_TYPE_FILE_CREATE, S_sd_inspection_info.filepath,
									S_sd_inspection_info.write_buf, 0, SD_CTRL_INSPECTION_DATA_SIZE,
									sd_ctrl_InspectionFioCallback, &reqNo );
			if (fioRes == FIO_RESULT_OK) {
				S_sd_inspection_info.fio_req_flg = N_ON;
				contflg = N_ON;
			}
		}
	}
	else if (S_sd_inspection_info.step == SD_INSPECTION_STEP_WRITE) {	/* 検査ステップ:書き込み検査			*/

		/* 書き込み結果をチェック	*/
		if ((S_sd_inspection_info.fio_result == FIO_RESULT_OK) &&
			(S_sd_inspection_info.fio_bytes == SD_CTRL_INSPECTION_DATA_SIZE)) {
			/* 書き込み成功	*/
			/* 検査ステップに「読み込み検査」を設定	*/
			S_sd_inspection_info.step = SD_INSPECTION_STEP_READ;

			/* SDからシリアルNoを読み込む	*/
			fioRes = FileIO_Read( S_sd_inspection_info.filepath,
								S_sd_inspection_info.read_buf, 0, SD_CTRL_INSPECTION_DATA_SIZE,
								sd_ctrl_InspectionFioCallback, &reqNo );
			if (fioRes == FIO_RESULT_OK) {
				S_sd_inspection_info.fio_req_flg = N_ON;
				contflg = N_ON;
			}
		}
	}
	else if (S_sd_inspection_info.step == SD_INSPECTION_STEP_READ) {	/* 検査ステップ:読み込み検査			*/

		/* 読み込み結果をチェック	*/
		if ((S_sd_inspection_info.fio_result == FIO_RESULT_OK) &&
			(S_sd_inspection_info.fio_bytes == SD_CTRL_INSPECTION_DATA_SIZE)) {
			if (memcmp(S_sd_inspection_info.write_buf, S_sd_inspection_info.read_buf, SD_CTRL_INSPECTION_DATA_SIZE) == 0) {
				/* SD検査OK	*/
				inspectRes = INSP_INSPECTION_RESULT_OK;
			}
		}
	}
	else {																/* 検査ステップ:無効(検査未実施状態)	*/

		/* 検査ステップに「検査用ディレクトリ作成」を設定	*/
		S_sd_inspection_info.step = SD_INSPECTION_STEP_MKDIR;

		/* SDに検査用ディレクトリを作成する	*/
		fioRes = FileIO_MakeDir((const UI_8*)SD_CTRL_INSPECTION_DIR, sd_ctrl_InspectionDirCallback, &reqNo);
		if (fioRes == FIO_RESULT_OK) {
			S_sd_inspection_info.fio_req_flg = N_ON;
			contflg = N_ON;
		}
	}

	if (contflg == N_OFF) {				/* SD検査終了		*/
		/* 検査結果パラメタ作成	*/
		(void)memset(&inspectParam, 0, sizeof(inspectParam));
		inspectParam.data1 = (UI_8)(S_sd_inspection_info.serialno >> 8);
		inspectParam.data2 = (UI_8)(S_sd_inspection_info.serialno & 0x00FF);

		/* 検査制御モジュールに結果を通知する	*/
		Insp_notify_inspection_result(INSP_INSPECTION_TYPE_SD, &inspectParam, inspectRes);

		/* SD検査情報をクリア	*/
		sd_ctrl_InitInspectionInfo();
	}

	return contflg;

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_InspectionFioCallback							*/
/* 機能の説明	:	SD検査ファイルアクセスコールバック						*/
/* 引数			:	(I/ )reqinfo	要求情報								*/
/* 				:	(I/ )retinfo	結果									*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_ctrl_InspectionFioCallback( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo )
{

	if ((reqinfo != NULL) && (retinfo != NULL)) {
		/* SD検査情報に検査ファイルアクセス結果をセットする */
		S_sd_inspection_info.fio_result = retinfo->result;
		S_sd_inspection_info.fio_bytes = retinfo->bytes;

		/* ファイルIOからの通知（SD検査用通知）フラグをON */
		S_sd_fio_notify_inspection = N_ON;
	}

	return;

}

/****************************************************************************/
/* 関数名称		:	sd_ctrl_InspectionFioCallback							*/
/* 機能の説明	:	SD検査ディレクトリ操作コールバック						*/
/* 引数			:	(I/ )reqinfo	要求情報								*/
/* 				:	(I/ )retinfo	結果									*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:															*/
/****************************************************************************/
static void sd_ctrl_InspectionDirCallback( const T_FIO_DIR_REQ_INFO *reqinfo, const T_FIO_DIR_RET_INFO *retinfo )
{

	if ((reqinfo != NULL) && (retinfo != NULL)) {
		/* SD検査情報に検査ファイルアクセス結果をセットする */
		S_sd_inspection_info.fio_result = retinfo->result;
		S_sd_inspection_info.fio_bytes = 0;

		/* ファイルIOからの通知（SD検査用通知）フラグをON */
		S_sd_fio_notify_inspection = N_ON;
	}

	return;

}
#endif /* _VDV_APP */


#endif /* _VDV_APP */
