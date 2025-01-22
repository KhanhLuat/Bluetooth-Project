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
/* file DTO459_VDV_Eeprom_ctrl.c						*/
/* EEPROM制御											*/
/********************************************************/

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include <string.h>
#include "DTO459_VDV_Eeprom_ctrl.h"
#include "System.h"
#include "SSFTSTD_Macro.h"
#include "SSFTSTD_FR81_Eep_Drv.h"
#include "System_control_TASK.h"
#include "DTO459_VDV_File_io.h"
#include "DTO459_VDV_Common.h"
#include "DTO459_VDV_RringBuffer.h"
#include "timer_ctrl.h"
#include "Timer.h"
#include "dbglog.h"

/********************************************************/
/*	マクロ定義											*/
/********************************************************/
#define EEP_LOCAL	static

#define EEPROM_ADR_FIRMWARE_VER		0x03FE	/**< ファームウェアバージョン	*/
#define EEPROM_ADR_EEPROM_VER		0x03FF	/**< EEPROMウェアバージョン		*/
#define EEPROM_ADR_VARIATION		0x0000	/**< 仕向け情報					*/
#define EEPROM_ADR_BAT				0x0005	/**< バッテリー電圧補正値		*/
#define EEPROM_ADR_SERIAL_NO		0x000A	/**< シリアルNo					*/
#if defined _VDV_SUPRA /* 2018.09.11 S.Sakata */
#define EEPROM_ADR_VEHICLETYPE		0x000F	/**< 車両型						*/
#endif /* defined _VDV_SUPRA 2018.09.11 S.Sakata */
#define EEPROM_ADR_VEHICLEINFO		0x0014	/**< 車両情報					*/
#define EEPROM_ADR_INVALID			0xFFFFU	/**< 無効						*/

#define EEPROM_DEFLT_VAL_FIRMWARE_VER	0x0000	/**< 初期値:ファームウェアバージョン	*/
#define EEPROM_DEFLT_VAL_EEPROM_VER		0x0000	/**< 初期値:EEPROMウェアバージョン		*/
#define EEPROM_DEFLT_VAL_VARIATION		0x0000	/**< 初期値:仕向け情報					*/
#define EEPROM_DEFLT_VAL_BAT			0x000A	/**< 初期値:バッテリー電圧補正値		*/
#define EEPROM_DEFLT_VAL_SERIAL_NO		0x0000	/**< 初期値:シリアルNo					*/
#if defined _VDV_SUPRA /* 2018.09.11 S.Sakata */
#define EEPROM_DEFLT_VAL_VEHICLETYPE	0x81FF	/**< 初期値:車両型						*/
												/**< bit0-3:気筒数：無効値				*/
												/**< bit4-7:ハンドル位置：無効値		*/
												/**< bit8-16:車両型情報：スープラ		*/
#endif /* defined _VDV_SUPRA 2018.09.11 S.Sakata */
#define EEPROM_DEFLT_VAL_VEHICLEJUDGE	0x0011	/**< 初期値:車両判定情報				*/
												/**< bit0-15:車両判定情報：086B	定義値TCAN_VEHICLE_INFO_86Bにあわせること	*/
#define EEPROM_ADR_INVALID			0xFFFFU	/**< 無効						*/

#define EEPROM_READ_BUFSIZE			64		/**< 読み込みバッファサイズ		*/
#define EEPROM_RING_BUFSIZE			128		/**< リングバッファサイズ		*/
#define EEPROM_RETRY_START			1000	/**< リトライ開始タイマ			*/

/* コールバックイベント用定義 */
#define EEPROM_FILE_SEARCH_EXIST	0x01	/**< ファイル検索:ファイルあり	*/
#define EEPROM_FILE_SEARCH_NOTEXIST	0x02	/**< ファイル検索:ファイルなし	*/
#define EEPROM_FILE_READ_OK			0x01	/**< ファイル読込:読み込み正常	*/
#define EEPROM_FILE_READ_NG			0x02	/**< ファイル読込:読み込み異常	*/
#define EEPROM_FILE_READ_COMP		0x03	/**< ファイル読込:読み込み完了	*/

/* パリティチェック用定義 */
#define PARITY_NORMAL		0x00		/*  パリティチェック結果正常 */
#define PARITY_ERROR_1BIT	0x01		/*  パリティチェック結果異常（1ビット） */
#define PARITY_ERROR_2BIT	0x02		/*  パリティチェック結果異常（2ビット以上） */

/* 
 ** システム動作状態
 */ 
typedef enum e_eeprom_ctrl_status
{
	EEPROM_STAT_WRITE_NOT = 0,				/**< EEPROM未書き込み				*/
	EEPROM_STAT_WRITE_NOW,					/**< EEPROM書き込み中				*/
	EEPROM_STAT_WRITE_COMPLETE,				/**< EEPROM書き込み終了			*/
	EEPROM_STAT_WRITE_SUSPEND,				/**< EEPROM書き込み中断			*/
	EEPROM_STAT_NUM							/**< EEPROM動作状態数				*/
} E_EEPROM_CTRL_STATUS;

/* 
 ** イベント
 */ 
typedef enum e_eeprom_ctrl_event
{
	EEPROM_EVENT_EEPROM_MODE_DETECT	= 0,	/**< EEPROM書換モード検出					*/
	EEPROM_EVENT_FILE_SEARCH,				/**< ファイル検索結果						*/
	EEPROM_EVENT_FILE_READ,					/**< ファイル読込結果						*/
	EEPROM_EVENT_EEPROM_WRITE,				/**< EEPROM書込み							*/
	EEPROM_EVENT_ACC_OFF_DETECT,			/**< ACC OFF検出							*/
	EEPROM_EVENT_ACC_ON_DETECT,				/**< ACC ON検出（ACC OFF移行状態からの）	*/
	EEPROM_EVENT_RETRY_TIMEOUT,				/**< リトライタイムアウト					*/
	EEPROM_EVENT_NUM						/**< イベント数								*/
} E_EEPROM_CTRL_EVENT;

/* 
 ** イベントパラメタ
 */ 
typedef enum e_eeprom_ctrl_event_param
{
	EEPROM_EVENT_PARAM_NO		= 0,		/**< パラメタなし												*/
	EEPROM_EVENT_PARAM_FILE_SRC_EXIST,		/**< ファイル検索:ファイルあり									*/
	EEPROM_EVENT_PARAM_FILE_SRC_NOTEXIST,	/**< ファイル検索:ファイルなし									*/

	EEPROM_EVENT_PARAM_FILE_READ_OK,		/**< ファイル読込:読み込み正常									*/
	EEPROM_EVENT_PARAM_FILE_READ_NG			/**< ファイル読込:読み込み異常									*/

} E_EEPROM_CTRL_EVENT_PARAM;


/********************************************************/
/*	構造体定義											*/
/********************************************************/
/* EEPROMデータ設定情報 */
typedef struct {
	E_EEPROM_DATA_TYPE		type;
	UI_16					data;
	PFN_EEPROM_SETDATA_CB	cbfnc;
} T_EEPROM_SET_DATA;

/* EEPROMデータ設定管理 */
typedef struct {
	UI_16					eep_adr;
	UI_8					data_num;
	UI_8					write_num;
	UI_16					data[5];
	T_EEPROM_SET_DATA		set_data;
} T_EEPROM_SET_DATA_MNG;

/********************************************************/
/*	内部変数定義										*/
/********************************************************/
EEP_LOCAL	E_EEPROM_CTRL_STATUS		S_eeprom_ctrl_state;						/**< EEPROM制御状態		*/
EEP_LOCAL	E_SYS_CTRL_STATUS			S_eeprom_prev_system_state;					/**< 前のシステム状態	*/
EEP_LOCAL	UI_8						S_eeprom_event_state[EEPROM_EVENT_NUM];		/**< イベント通知状態 	*/
EEP_LOCAL	E_EEPROM_CTRL_EVENT_PARAM	S_eeprom_event_param[EEPROM_EVENT_NUM];		/**< イベントパラメタ 	*/
EEP_LOCAL	UI_8						S_eeprom_event_fileread_stat;				/**< イベント発生状態(N_OFF以外:発生、N_OFF:未発生) */
EEP_LOCAL	UI_8						S_eeprom_event_filesearch_stat;				/**< イベント発生状態(N_OFF以外:発生、N_OFF:未発生) */

EEP_LOCAL	UI_8						S_eeprom_filepath[COMMON_VDV_FILE_PATH_LEN];/**< 検索したファイルパス名(ディレクトリ+ファイル名)	*/
EEP_LOCAL	UI_8						S_eeprom_readBuff[EEPROM_READ_BUFSIZE];		/**< ファイル読み込み用バッファ							*/
EEP_LOCAL	UI_16						S_eeprom_readFileSize;						/**< ファイルサイズ										*/
EEP_LOCAL	UI_16						S_eeprom_readSizeTotal;						/**< ファイル読み込み合計サイズ							*/
EEP_LOCAL	UI_16						S_eeprom_readSize;							/**< ファイル読み込みサイズ（直近）						*/
EEP_LOCAL	UI_16						S_eeprom_fileAccessWaitCB;					/**< ファイルアクセスコールバック待ちフラグ				*/
EEP_LOCAL	UI_16						S_eeprom_writeSizeTotal;					/**< EEPROM書き込み合計サイズ							*/

EEP_LOCAL	UI_16						S_eeprom_retry_timerCount;					/**< リトライタイマー									*/

EEP_LOCAL 	T_RINGBUFFER 				S_eeprom_ringBuffMng;						/**< ファイル読み込みデータ保持用リングバッファ管理領域 */
EEP_LOCAL	UI_8						S_eeprom_ringBuff[EEPROM_RING_BUFSIZE];		/**< ファイル読み込みデータ保持用リングバッファ			*/

EEP_LOCAL	T_EEPROM_SET_DATA_MNG		S_eeprom_setdata_mng;						/**< EEPROM設定データ管理								*/
EEP_LOCAL 	T_RINGBUFFER 				S_eeprom_setdata_ringBuffMng;				/**< EEPROMデータ設定用リングバッファ管理領域			*/
EEP_LOCAL	UI_8						S_eeprom_setdata_ringBuff[(sizeof(T_EEPROM_SET_DATA) * 4)];
																				/**< EEPROMデータ設定用リングバッファ					*/

/********************************************************/
/*	内部関数プロトタイプ宣言							*/
/********************************************************/
EEP_LOCAL void S_eeprom_setDataProc(void);
EEP_LOCAL UI_8 S_eeprom_checkEventIssue(E_SYS_CTRL_STATUS system_state);
EEP_LOCAL UI_8 S_eeprom_createEvent(E_EEPROM_CTRL_EVENT event, E_EEPROM_CTRL_EVENT_PARAM param);
EEP_LOCAL void S_eeprom_event_eeprom_mode_detect( UI_8 param );
EEP_LOCAL void S_eeprom_event_file_search( UI_8 param );
EEP_LOCAL void S_eeprom_event_file_read( UI_8 param );
EEP_LOCAL void S_eeprom_event_eeprom_write( UI_8 param );
EEP_LOCAL void S_eeprom_event_acc_off_detect( UI_8 param );
EEP_LOCAL void S_eeprom_event_acc_on_detect( UI_8 param );
EEP_LOCAL void S_eeprom_event_retry_timeout( UI_8 param );
EEP_LOCAL void S_eeprom_setStatus(E_EEPROM_CTRL_STATUS status);
EEP_LOCAL void S_eeprom_initData(void);
EEP_LOCAL void S_eeprom_writeData(UI_16 adr, const UI_8* pData, UI_16 dataSize);
EEP_LOCAL SI_8 S_eeprom_readData(UI_16 adr, UI_16* pData);
EEP_LOCAL void S_eeprom_ReadEepromFileCB( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo );
EEP_LOCAL void S_eeprom_SearchEepromFileCB( const T_COMMON_SRC_VDVF_RET *pSearchResult );
EEP_LOCAL UI_16 parity8(UI_16 *p, SI_8 n);
EEP_LOCAL UI_8 parity_check8(UI_16 *p, SI_8 n);


/*
 *	イベント処理関数テーブル
 */
EEP_LOCAL void ( * const C_eeprom_event_func[EEPROM_EVENT_NUM] ) ( UI_8 ) = {
	S_eeprom_event_eeprom_mode_detect,					/**< EEPROM_EVENT_EEPROM_MODE_DETECT	*/
	S_eeprom_event_file_search,							/**< EEPROM_EVENT_FILE_SEARCH			*/
	S_eeprom_event_file_read,							/**< EEPROM_EVENT_FILE_READ				*/
	S_eeprom_event_eeprom_write,						/**< EEPROM_EVENT_EEPROM_WRITE			*/
	S_eeprom_event_acc_off_detect,						/**< EEPROM_EVENT_ACC_OFF_DETECT		*/
	S_eeprom_event_acc_on_detect,						/**< EEPROM_EVENT_ACC_ON_DETECT			*/
	S_eeprom_event_retry_timeout,						/**< EEPROM_EVENT_RETRY_TIMEOUT			*/
};


/********************************************************/
/*	外部関数											*/
/********************************************************/

/********************************************************/
/* 関数名称		: Eeprom_control_Init					*/
/* 機能の説明	: EEPROM制御初期化						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Eeprom_control_Init(void)
{
	UI_8 i;
	S_eeprom_setStatus(EEPROM_STAT_WRITE_NOT);
	S_eeprom_prev_system_state	= SYS_OPERATION_NUM;
	/* リングバッファ初期化 */
	RingBuffer_Init(&S_eeprom_ringBuffMng, S_eeprom_ringBuff, sizeof(S_eeprom_ringBuff));
	/* 内部データ初期化 */
	S_eeprom_initData();

	/* EEPROM設定データ初期化 */
	RingBuffer_Init(&S_eeprom_setdata_ringBuffMng, S_eeprom_setdata_ringBuff, sizeof(S_eeprom_setdata_ringBuff));
	(void)memset(&S_eeprom_setdata_mng, 0, sizeof(S_eeprom_setdata_mng));
	for (i = 0; i < EEPROM_EVENT_NUM; i++)
	{
		S_eeprom_event_state[i] = N_OFF;
		S_eeprom_event_param[i] = EEPROM_EVENT_PARAM_NO;
	}
}

/********************************************************/
/* 関数名称		: Eeprom_control_Main					*/
/* 機能の説明	: EEPROM制御メイン						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Eeprom_control_Main(void)
{
	UI_8	i;
	E_SYS_CTRL_STATUS system_state;
	E_EEPROM_CTRL_EVENT_PARAM	param;
	UI_8 eventFlag;


	/**	<li>システム状態を取得	*/
	system_state = System_control_GetStatus();
	switch (system_state)
	{
		case (SYS_OPERATION_ERROR):					/**< 異常					*/
			/* これらのモードでは何もしない */
			return;

		case (SYS_OPERATION_NORMAL_OFF):			/**< 通常:OFFモード			*/
		case (SYS_OPERATION_NORMAL_AUTO):			/**< 通常:オートREC			*/
		case (SYS_OPERATION_NORMAL_MANUAL):			/**< 通常:マニュアルREC		*/
		case (SYS_OPERATION_SIMULATION):			/**< シミュレーション		*/
		case (SYS_OPERATION_FIRMWARE_WRITE):		/**< ファームウェア書換中	*/
		case (SYS_OPERATION_FIRMWARE_WRITE_COMP):	/**< ファームウェア書換完了	*/
		case (SYS_OPERATION_INSPECTION):			/**< 検査モード				*/
		case (SYS_OPERATION_INSPECTION_COMP):		/**< 検査完了				*/
		case (SYS_OPERATION_BT_COMM):				/**< BTモジュール通信モード	*/
		case (SYS_OPERATION_GNSS_COMM):				/**< GNSSモジュール通信モード*/
			/* EEPROMデータ設定処理実行 */
			S_eeprom_setDataProc();
			return;

		default:
			break;
	}
	
	/* イベント発生チェック */
	eventFlag = S_eeprom_checkEventIssue(system_state);
	/**	<li>イベントの有無を判定	*/
	if ( eventFlag == N_ON )
	{
		/**	<li>イベントがあった処理を実行	*/
		for ( i = 0; i < EEPROM_EVENT_NUM; i++ )
		{
			if ( S_eeprom_event_state[i] == N_ON )
			{
				param = S_eeprom_event_param[i];
				C_eeprom_event_func[i]((UI_8)param);
				S_eeprom_event_state[i] = N_OFF;
				S_eeprom_event_param[i] = EEPROM_EVENT_PARAM_NO;
			}
		}
	}
}


/********************************************************/
/* 関数名称		: Eeprom_control_GetData				*/
/* 機能の説明	: EEPROMからデータ取得					*/
/* 引数			: (I/ )データ種別  E_EEPROM_DATA_TYPE	*/
/* 				: ( /O)取得データ  						*/
/* 戻り値		: 処理結果 E_EEPROM_CONTROL_RESULT		*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
E_EEPROM_CONTROL_RESULT Eeprom_control_GetData(E_EEPROM_DATA_TYPE type, UI_16* pData)
{
	UI_16 adr;
	/* データ読み込み */
	switch(type)
	{
		case EEPROM_DATA_FIRMWARE_VER:	/**< ファームウェアバージョン	*/
			adr = EEPROM_ADR_FIRMWARE_VER;
			break;
		case EEPROM_DATA_EEPROM_VER:	/**< EEPROMウェアバージョン		*/
			adr = EEPROM_ADR_EEPROM_VER;
			break;
		case EEPROM_DATA_VARIATION:		/**< 仕向け情報					*/
			adr = EEPROM_ADR_VARIATION;
			break;
		case EEPROM_DATA_BAT:			/**< バッテリー電圧補正値		*/
			adr = EEPROM_ADR_BAT;
			break;
		case EEPROM_DATA_SERIAL_NO:		/**< シリアルNo					*/
			adr = EEPROM_ADR_SERIAL_NO;
			break;
#if defined _VDV_SUPRA /* 2018.09.11 S.Sakata */
		case EEPROM_DATA_VEHICLETYPE:
			adr = EEPROM_ADR_VEHICLETYPE;
			break;
#endif /* defined _VDV_SUPRA 2018.09.11 S.Sakata */
		case EEPROM_DATA_VEHICLEINFO:	/**< 車種情報					*/
			adr = EEPROM_ADR_VEHICLEINFO;
			break;
		default:
			adr = EEPROM_ADR_INVALID;	/**< 異常						*/
			break;
	}
	if(adr == EEPROM_ADR_INVALID)
	{
		/* 引数不正 */
		return EEPROM_CONTROL_RESULT_ERR_INVALID_ARG;
	}
	/* EEPROM読み込み */
	if (S_eeprom_readData(adr,pData) != N_OK) {
		return EEPROM_CONTROL_RESULT_ERR_FATAL;
	}

	return EEPROM_CONTROL_RESULT_OK;
}

/********************************************************/
/* 関数名称		: Eeprom_control_SetData				*/
/* 機能の説明	: EEPROMへデータ設定					*/
/* 引数			: (I/ )データ種別  E_EEPROM_DATA_TYPE	*/
/* 				: (I/ )設定データ  						*/
/*				: (I/ )書き込み完了コールバック関数		*/
/* 戻り値		: 処理結果 E_EEPROM_CONTROL_RESULT		*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
E_EEPROM_CONTROL_RESULT Eeprom_control_SetData(E_EEPROM_DATA_TYPE type, UI_16 data, PFN_EEPROM_SETDATA_CB cbfnc)
{
	T_EEPROM_SET_DATA	setdata;

	if (type >= EEPROM_DATA_NUM) {
		/* 引数不正 */
		return EEPROM_CONTROL_RESULT_ERR_INVALID_ARG;
	}

	/* データ設定用リングバッファ書き込み可能サイズ取得	*/
	if (RingBuffer_GetCanWriteSize(&S_eeprom_setdata_ringBuffMng) < sizeof(setdata)) {
		return EEPROM_CONTROL_RESULT_ERR_BUSY;
	}

	/* データ設定用リングバッファに格納	*/
	(void)memset(&setdata, 0, sizeof(setdata));
	setdata.type = type;
	setdata.data = data;
	setdata.cbfnc = cbfnc;
	(void)RingBuffer_SetArrayData(&S_eeprom_setdata_ringBuffMng, (UI_8*)&setdata, sizeof(setdata));

	return EEPROM_CONTROL_RESULT_OK;
}

/********************************************************/
/* 関数名称		: S_eeprom_setDataProc					*/
/* 機能の説明	: EEPROMへデータ設定					*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/02/09								*/
/* 備考			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_setDataProc(void)
{
	UI_16	adr;
	UI_16	data;
	UI_8	i;

	if (S_eeprom_setdata_mng.data_num == 0) {
		/* EEPROM書き込み中のデータが無い場合はリングバッファから設定データを取得する */
		if(RingBuffer_GetCanReadSize(&S_eeprom_setdata_ringBuffMng) >= sizeof(T_EEPROM_SET_DATA)) {

			(void)memset(&S_eeprom_setdata_mng, 0, sizeof(S_eeprom_setdata_mng));

			/* 設定データを取得 */
			(void)RingBuffer_GetArrayData(&S_eeprom_setdata_ringBuffMng, (UI_8*)&S_eeprom_setdata_mng.set_data, sizeof(T_EEPROM_SET_DATA), RINGBUF_READEND);

			/* 設定データからEEPROMに書き込むデータに変換 */
			switch (S_eeprom_setdata_mng.set_data.type) {

				case EEPROM_DATA_FIRMWARE_VER:	/**< ファームウェアバージョン	*/
					S_eeprom_setdata_mng.eep_adr = EEPROM_ADR_FIRMWARE_VER;
					S_eeprom_setdata_mng.data_num = 1;
					S_eeprom_setdata_mng.data[0] = S_eeprom_setdata_mng.set_data.data;
					break;
				case EEPROM_DATA_EEPROM_VER:	/**< EEPROMウェアバージョン		*/
					S_eeprom_setdata_mng.eep_adr = EEPROM_ADR_EEPROM_VER;
					S_eeprom_setdata_mng.data_num = 1;
					S_eeprom_setdata_mng.data[0] = S_eeprom_setdata_mng.set_data.data;
					break;
				case EEPROM_DATA_VARIATION:		/**< 仕向け情報					*/
					S_eeprom_setdata_mng.eep_adr = EEPROM_ADR_VARIATION;
					S_eeprom_setdata_mng.data_num = 5;
					S_eeprom_setdata_mng.data[0] = S_eeprom_setdata_mng.set_data.data;
					break;
				case EEPROM_DATA_BAT:			/**< バッテリー電圧補正値		*/
					S_eeprom_setdata_mng.eep_adr = EEPROM_ADR_BAT;
					S_eeprom_setdata_mng.data_num = 5;
					/* bit8-15なので8bitシフト */
					S_eeprom_setdata_mng.data[0] = (S_eeprom_setdata_mng.set_data.data << 8);
					break;
				case EEPROM_DATA_SERIAL_NO:		/**< シリアルNo					*/
					S_eeprom_setdata_mng.eep_adr = EEPROM_ADR_SERIAL_NO;
					S_eeprom_setdata_mng.data_num = 5;
					S_eeprom_setdata_mng.data[0] = S_eeprom_setdata_mng.set_data.data;
					break;
#if defined _VDV_SUPRA /* 2018.09.11 S.Sakata */
				case EEPROM_DATA_VEHICLETYPE:	/**< 車両型情報					*/
					S_eeprom_setdata_mng.eep_adr = EEPROM_ADR_VEHICLETYPE;
					S_eeprom_setdata_mng.data_num = 5;
					S_eeprom_setdata_mng.data[0] = S_eeprom_setdata_mng.set_data.data;
					break;
#endif /* defined _VDV_SUPRA 2018.09.11 S.Sakata */
				case EEPROM_DATA_VEHICLEINFO:	/**< 車両情報					*/
					S_eeprom_setdata_mng.eep_adr = EEPROM_ADR_VEHICLEINFO;
					S_eeprom_setdata_mng.data_num = 5;
					S_eeprom_setdata_mng.data[0] = S_eeprom_setdata_mng.set_data.data;
					break;
				default:						/**< 異常						*/
					break;
			}
			if (S_eeprom_setdata_mng.data_num == 0) {
				/* 書き込み異常コールバック */
				if (S_eeprom_setdata_mng.set_data.cbfnc != NULL) {
					S_eeprom_setdata_mng.set_data.cbfnc(S_eeprom_setdata_mng.set_data.type, S_eeprom_setdata_mng.set_data.data, N_NG);
				}
			}
			else if (S_eeprom_setdata_mng.data_num == 5) {
				/* パリティ含めた書き込みデータを作成する */
				for (i = 1; i < 4; i++) {
					S_eeprom_setdata_mng.data[i] = EepDrv_ReadDataWord( S_eeprom_setdata_mng.eep_adr + i);
				}
				S_eeprom_setdata_mng.data[4] = parity8(S_eeprom_setdata_mng.data, 4);
			}
			else {
				/* nop */
			}
		}
	}

	if (S_eeprom_setdata_mng.data_num > 0) {
		/* EEPROM書き込みデータがある場合EEPROMに書き込む */
		/* EEPROMドライバ有効 */
		EepDrv_Enable();
	
		/* EEPROMデータ書き込み */
		adr = S_eeprom_setdata_mng.eep_adr + S_eeprom_setdata_mng.write_num;
		data = S_eeprom_setdata_mng.data[S_eeprom_setdata_mng.write_num];
		EepDrv_WriteDataWord(adr, data);
		S_eeprom_setdata_mng.write_num++;

		/* EEPROMドライバ無効 */
		EepDrv_Disable();

		if (S_eeprom_setdata_mng.data_num <= S_eeprom_setdata_mng.write_num) {
			/* 書き込み完了 */
			/* コールバック通知 */
			if (S_eeprom_setdata_mng.set_data.cbfnc != NULL) {
				S_eeprom_setdata_mng.set_data.cbfnc(S_eeprom_setdata_mng.set_data.type, S_eeprom_setdata_mng.set_data.data, N_OK);
			}

			/* 書き込みデータクリア */
			(void)memset(&S_eeprom_setdata_mng, 0, sizeof(S_eeprom_setdata_mng));
		}
	}

	return;
}

/************************************************************/
/* 関数名称		: S_eeprom_checkEventIssue					*/
/* 機能の説明	: イベント発行チェック						*/
/* 引数			: system_state:システム制御状態				*/
/* 戻り値		: N_ON:イベントあり N_OFF:イベントなし		*/
/* 作成者		: (OFT)Sakata								*/
/* 作成日		: 16/10/12									*/
/* 備考			: 											*/
/************************************************************/
EEP_LOCAL UI_8 S_eeprom_checkEventIssue(E_SYS_CTRL_STATUS system_state)
{
	E_EEPROM_CTRL_EVENT_PARAM param;
	UI_8 ret = N_OFF;

	if(system_state != S_eeprom_prev_system_state)
	{
		/**	<li>システム状態を変化あり	*/
		switch (system_state)
		{
		case (SYS_OPERATION_EEPROM_WRITE):			/*EEPROM書換中			*/
			/* EEPROM書換モード検出 */
			(void)S_eeprom_createEvent(EEPROM_EVENT_EEPROM_MODE_DETECT, EEPROM_EVENT_PARAM_NO);
			ret = N_ON;
			break;
		case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF移行			*/
			/* ACC OFF検出 */
			(void)S_eeprom_createEvent(EEPROM_EVENT_ACC_OFF_DETECT, EEPROM_EVENT_PARAM_NO);
			ret = N_ON;
			break;
		case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON検出			*/
			if(S_eeprom_prev_system_state == SYS_OPERATION_PRE_ACC_OFF)
			{
				/* ACC OFF移行 -> ACC ON検出 */
				(void)S_eeprom_createEvent(EEPROM_EVENT_ACC_ON_DETECT, EEPROM_EVENT_PARAM_NO);
				ret = N_ON;
			}
			break;
		default:
			break;
		}
		S_eeprom_prev_system_state = system_state;
	}
	/* イベント発生フラグセット */
	if(S_eeprom_event_fileread_stat != N_OFF)
	{
		/* ファイル読み結果あり */
		if(S_eeprom_event_fileread_stat == EEPROM_FILE_READ_OK)
		{
			param = EEPROM_EVENT_PARAM_FILE_READ_OK;
		}
		else if(S_eeprom_event_fileread_stat == EEPROM_FILE_READ_NG)
		{
			param = EEPROM_EVENT_PARAM_FILE_READ_NG;
		}
		else
		{
			/* フェール */
			param = EEPROM_EVENT_PARAM_NO;
		}
		S_eeprom_event_fileread_stat = N_OFF;
		/* 読み込み結果 */
		(void)S_eeprom_createEvent(EEPROM_EVENT_FILE_READ, param);
		ret = N_ON;
	}
	if(S_eeprom_event_filesearch_stat != N_OFF)
	{
		/* ファイル検索結果あり */
		if(S_eeprom_event_filesearch_stat == EEPROM_FILE_SEARCH_EXIST)
		{
			param = EEPROM_EVENT_PARAM_FILE_SRC_EXIST;
		}
		else if(S_eeprom_event_filesearch_stat == EEPROM_FILE_SEARCH_NOTEXIST)
		{
			param = EEPROM_EVENT_PARAM_FILE_SRC_NOTEXIST;
		}
		else
		{
			/* フェール */
			param = EEPROM_EVENT_PARAM_NO;
		}
		S_eeprom_event_filesearch_stat = N_OFF;
		(void)S_eeprom_createEvent(EEPROM_EVENT_FILE_SEARCH, param);
		ret = N_ON;
	}
	if(RingBuffer_GetCanReadSize(&S_eeprom_ringBuffMng) >= sizeof(UI_16))
	{
		/* EEPROM書込み */
		(void)S_eeprom_createEvent(EEPROM_EVENT_EEPROM_WRITE, EEPROM_EVENT_PARAM_NO);
		ret = N_ON;
	}
	if(S_eeprom_retry_timerCount > 0)
	{
		/* リトライタイマー起動中 */
		/**	リトライタイマータイムアウトしたかチェック	*/
		if (Timer_ctrl_TimeCompare10ms(S_eeprom_retry_timerCount, EEPROM_RETRY_START/TIME_MAIN) == N_OK)
		{
			S_eeprom_retry_timerCount = 0;
			(void)S_eeprom_createEvent(EEPROM_EVENT_RETRY_TIMEOUT, EEPROM_EVENT_PARAM_NO);
			ret = N_ON;
		}
	}
	
	return ret;
}

/************************************************************/
/* 関数名称		: S_eeprom_createEvent						*/
/* 機能の説明	: イベント通知								*/
/* 引数			: (I/ )イベント								*/
/* 引数			: (I/ )イベントパラメタ						*/
/* 戻り値		: 引数異常		EEPROM_CONTROL_RESULT_ERR_INVALID_ARG*/
/* 				: 正常			EEPROM_CONTROL_RESULT_OK	*/
/* 作成者		: (OFT)Sakata								*/
/* 作成日		: 16/10/12									*/
/* 備考			: 											*/
/************************************************************/
EEP_LOCAL UI_8 S_eeprom_createEvent(E_EEPROM_CTRL_EVENT event, E_EEPROM_CTRL_EVENT_PARAM param)
{
	
	/**	<ol>	*/
	/**	<li>引数チェック	*/
	if ( event >= EEPROM_EVENT_NUM )
	{
		return EEPROM_CONTROL_RESULT_ERR_INVALID_ARG;
	}
	
	/**	<li>イベント発生を通知	*/
	if ( S_eeprom_event_state[event] == N_OFF)
	{
		S_eeprom_event_state[event] = N_ON;
		S_eeprom_event_param[event] = param;
	}
	/**	</ol>	*/
	return EEPROM_CONTROL_RESULT_OK;
}


/********************************************************/
/* 関数名称		: S_eeprom_event_eeprom_mode_detect		*/
/* 機能の説明	: イベント処理関数:EEPROM動作モード検出	*/
/* 引数			: (I/)param:イベントパラメタ			*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/10/12								*/
/* 備考			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_event_eeprom_mode_detect( UI_8 param )
{
	E_COMMON_RESULT ret;
	switch(S_eeprom_ctrl_state)
	{
		case EEPROM_STAT_WRITE_NOT:				/**< EEPROM未書き込み			*/
			/*ファイル検索開始*/
			ret = Common_SerchVdvFile	(COMMON_VDV_FILE_TYPE_EEP, S_eeprom_SearchEepromFileCB);
			if(COMMON_RESULT_OK == ret)
			{
				/* ファイル検索正常 */
				/* コールバック待ち */
				S_eeprom_fileAccessWaitCB = N_ON;
			}
			else
			{
				/* ファイル検索異常 */
				/* 状態変更 */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
				/* システム制御へEEPROM書換完了通知 */
				(void)System_control_NotifyEvent(SYS_EVENT_END_EEPROM_WRITE, SYS_EVENT_PARAM_NO);
			}
		break;
		case EEPROM_STAT_WRITE_NOW:				/**< EEPROM書き込み中			*/
		break;
		case EEPROM_STAT_WRITE_COMPLETE:		/**< EEPROM書き込み終了		*/
		break;
		case EEPROM_STAT_WRITE_SUSPEND:			/**< EEPROM書き込み中断		*/
		break;
		default:
		break;
	}
}


/********************************************************/
/* 関数名称		: S_eeprom_event_file_search			*/
/* 機能の説明	: イベント処理関数:ファイル検索結果		*/
/* 引数			: (I/)param:イベントパラメタ			*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/10/12								*/
/* 備考			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_event_file_search( UI_8 param )
{
	UI_32 reqno;
	E_FIO_RESULT	ret;
	/* コールバック待ち */
	S_eeprom_fileAccessWaitCB = N_OFF;
	switch(S_eeprom_ctrl_state)
	{
		case EEPROM_STAT_WRITE_NOT:				/**< EEPROM未書き込み			*/
			if(param == EEPROM_EVENT_PARAM_FILE_SRC_EXIST)
			{
				/**< ファイル検索:ファイルあり									*/
				/* ファイル読み込み開始 */
				ret = FileIO_Read( S_eeprom_filepath, S_eeprom_readBuff, (UI_32)S_eeprom_readSizeTotal, sizeof(S_eeprom_readBuff), S_eeprom_ReadEepromFileCB, &reqno );
				if (ret == FIO_RESULT_OK) 
				{
					/* ファイル読み込み開始:正常 */
					/* コールバック待ち */
					S_eeprom_fileAccessWaitCB = N_ON;
					/* 状態変更 */
					S_eeprom_setStatus(EEPROM_STAT_WRITE_NOW);
				}
				else
				{
					/* ファイル読み込み開始:異常 */
					/* リトライタイマー開始時間取得 */
					S_eeprom_retry_timerCount = Timer_ctrl_GetTime10ms();
					/* 状態変更 */
					S_eeprom_setStatus(EEPROM_STAT_WRITE_SUSPEND);
				}
			}
			else
			{
				/**< ファイル検索:ファイルなし									*/
				/* 状態変更 */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
				/* ファイルがなければEEPROM書換終了とする */
				/* システム制御へEEPROM書換完了通知 */
				(void)System_control_NotifyEvent(SYS_EVENT_END_EEPROM_WRITE, SYS_EVENT_PARAM_NO);
			}
		break;
		case EEPROM_STAT_WRITE_SUSPEND:			/**< EEPROM書き込み中断		*/
			/* システム制御へEEPROM終了通知 */
			(void)System_control_NotifyEvent(SYS_EVENT_END_PROC_COMP_EEPROM, SYS_EVENT_PARAM_NO);
			/* 状態変更 */
			S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
		break;
		default:
		break;
	}
}


/********************************************************/
/* 関数名称		: S_eeprom_event_file_read				*/
/* 機能の説明	: イベント処理関数:ファイル読込結果		*/
/* 引数			: (I/)param:イベントパラメタ			*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/10/12								*/
/* 備考			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_event_file_read( UI_8 param )
{
	UI_32 reqno;
	E_FIO_RESULT	ret;
	UI_16 can_write_len;
	/* コールバック待ち */
	S_eeprom_fileAccessWaitCB = N_OFF;
	switch(S_eeprom_ctrl_state)
	{
		case EEPROM_STAT_WRITE_NOW:				/**< EEPROM書き込み中			*/
			if(param == EEPROM_EVENT_PARAM_FILE_READ_OK)
			{
				/**< ファイル読込:読み込み正常									*/
				if(S_eeprom_readSize > 0)
				{
					/* リングバッファへデータ格納 */
					(void)RingBuffer_SetArrayData(&S_eeprom_ringBuffMng, S_eeprom_readBuff, S_eeprom_readSize);
				}
				/* 総読み込みサイズ加算 */
				S_eeprom_readSizeTotal += S_eeprom_readSize;
				if(S_eeprom_readSizeTotal < S_eeprom_readFileSize)
				{
					/* まだ最後まで読み込んでいない */
					/* 書き込み可能サイズ取得 */
					can_write_len = RingBuffer_GetCanWriteSize(&S_eeprom_ringBuffMng);
					if(can_write_len >= sizeof(S_eeprom_readBuff))
					{
						/* まだ空きがあればデータ読み */
						ret = FileIO_Read( S_eeprom_filepath, S_eeprom_readBuff, (UI_32)S_eeprom_readSizeTotal, sizeof(S_eeprom_readBuff), S_eeprom_ReadEepromFileCB, &reqno );
						if (ret == FIO_RESULT_OK) 
						{
							/* ファイル読み込み開始:正常 */
							/* コールバック待ち */
							S_eeprom_fileAccessWaitCB = N_ON;
						}
						else
						{
							/* ファイル読み込み開始:異常 */
							/* リトライタイマー開始時間取得 */
							S_eeprom_retry_timerCount = Timer_ctrl_GetTime10ms();
							/* 状態変更 */
							S_eeprom_setStatus(EEPROM_STAT_WRITE_SUSPEND);
						}
					}
				}
			}
			else
			{
				/**< ファイル読込:読み込み異常									*/
				/* リトライタイマー開始時間取得 */
				S_eeprom_retry_timerCount = Timer_ctrl_GetTime10ms();
				/* 状態変更 */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_SUSPEND);
			}
		break;
		case EEPROM_STAT_WRITE_SUSPEND:			/**< EEPROM書き込み中断		*/
			/* システム制御へEEPROM終了通知 */
			(void)System_control_NotifyEvent(SYS_EVENT_END_PROC_COMP_EEPROM, SYS_EVENT_PARAM_NO);
			/* 状態変更 */
			S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
		break;
		default:
		break;
	}
}


/********************************************************/
/* 関数名称		: S_eeprom_event_eeprom_write			*/
/* 機能の説明	: イベント処理関数:EEPROMへ書き込み		*/
/* 引数			: (I/)param:イベントパラメタ			*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/10/12								*/
/* 備考			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_event_eeprom_write( UI_8 param )
{
	UI_8  buf[2];
	UI_16 can_write_len;
	UI_32 reqno;
	E_FIO_RESULT	ret;

	if(S_eeprom_ctrl_state != EEPROM_STAT_WRITE_NOW)
	{
		/**< EEPROM書き込み中以外なら抜ける	*/
		return;
	}

	if(RingBuffer_GetCanReadSize(&S_eeprom_ringBuffMng) >= sizeof(UI_16))
	{
		/* リングバッファからデータ取得 */
		(void)RingBuffer_GetArrayData(&S_eeprom_ringBuffMng, buf, sizeof(buf), RINGBUF_READEND);

		/* EEPROMへデータ書き込み */
		S_eeprom_writeData(S_eeprom_writeSizeTotal / sizeof(UI_16), buf, sizeof(buf));
		
		/* EEPROM書き込み合計加算 */
		S_eeprom_writeSizeTotal+= sizeof(UI_16);

	}
	if(S_eeprom_writeSizeTotal >= S_eeprom_readFileSize)
	{
		/* すべて書き込み完了 */
		/* 状態変更 */
		S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
		/* システム制御へEEPROM書換完了通知 */
		(void)System_control_NotifyEvent(SYS_EVENT_END_EEPROM_WRITE, SYS_EVENT_PARAM_NO);
	}
	else if(S_eeprom_readSizeTotal < S_eeprom_readFileSize)
	{
		/* まだ最後まで読み込んでいない */
		if(S_eeprom_fileAccessWaitCB == N_OFF){
			/* ファイル未アクセス */
			/* 書き込み可能サイズ取得 */
			can_write_len = RingBuffer_GetCanWriteSize(&S_eeprom_ringBuffMng);
			if(can_write_len >= sizeof(S_eeprom_readBuff))
			{
				/* まだ空きがあればデータ読み */
				ret = FileIO_Read( S_eeprom_filepath, S_eeprom_readBuff, (UI_32)S_eeprom_readSizeTotal, sizeof(S_eeprom_readBuff), S_eeprom_ReadEepromFileCB, &reqno );
				if (ret == FIO_RESULT_OK) 
				{
					/* ファイル読み込み開始:正常 */
					/* コールバック待ち */
					S_eeprom_fileAccessWaitCB = N_ON;
				}
				else
				{
					/* ファイル読み込み開始:異常 */
					/* リトライタイマー開始時間取得 */
					S_eeprom_retry_timerCount = Timer_ctrl_GetTime10ms();
					/* 状態変更 */
					S_eeprom_setStatus(EEPROM_STAT_WRITE_SUSPEND);
				}
			}
		}
	}
	else
	{
		/* nop */
	}
}

/********************************************************/
/* 関数名称		: S_eeprom_event_acc_off_detect			*/
/* 機能の説明	: イベント処理関数:ACC OFF検出			*/
/* 引数			: (I/)param:イベントパラメタ			*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/10/12								*/
/* 備考			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_event_acc_off_detect( UI_8 param )
{
	switch(S_eeprom_ctrl_state)
	{
		case EEPROM_STAT_WRITE_NOT:				/**< EEPROM未書き込み			*/
		case EEPROM_STAT_WRITE_NOW:				/**< EEPROM書き込み中			*/
			if(S_eeprom_fileAccessWaitCB == N_ON)
			{
				/* コールバック待ち */
				/* 状態変更 */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_SUSPEND);
			}
			else
			{
				/* コールバック待ちでない */
				/* 状態変更 */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
				/* システム制御へEEPROM終了通知 */
				(void)System_control_NotifyEvent(SYS_EVENT_END_PROC_COMP_EEPROM, SYS_EVENT_PARAM_NO);
			}	
		break;
		case EEPROM_STAT_WRITE_COMPLETE:		/**< EEPROM書き込み終了		*/
			/* システム制御へEEPROM終了通知 */
			(void)System_control_NotifyEvent(SYS_EVENT_END_PROC_COMP_EEPROM, SYS_EVENT_PARAM_NO);
		break;
		case EEPROM_STAT_WRITE_SUSPEND:			/**< EEPROM書き込み中断		*/
			if(S_eeprom_fileAccessWaitCB == N_ON)
			{
				/* コールバック待ち */
				/* 処理なし */
			}
			else
			{
				/* コールバック待ちでない */
				/* リトライタイマー開始時間取得 */
				S_eeprom_retry_timerCount = 0;
				/* 状態変更 */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
				/* システム制御へEEPROM終了通知 */
				(void)System_control_NotifyEvent(SYS_EVENT_END_PROC_COMP_EEPROM, SYS_EVENT_PARAM_NO);
			}
		break;
		default:
		break;
	}
}




/********************************************************/
/* 関数名称		: S_eeprom_event_acc_on_detect			*/
/* 機能の説明	: イベント処理関数:ACC ON検出			*/
/* 引数			: (I/)param:イベントパラメタ			*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/10/12								*/
/* 備考			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_event_acc_on_detect( UI_8 param )
{
	switch(S_eeprom_ctrl_state)
	{
		case EEPROM_STAT_WRITE_NOT:				/**< EEPROM未書き込み			*/
		case EEPROM_STAT_WRITE_NOW:				/**< EEPROM書き込み中			*/
		case EEPROM_STAT_WRITE_COMPLETE:		/**< EEPROM書き込み終了		*/
		case EEPROM_STAT_WRITE_SUSPEND:			/**< EEPROM書き込み中断		*/
			/* 内部初期化 */
			S_eeprom_initData();
			/* 状態変更 */
			S_eeprom_setStatus(EEPROM_STAT_WRITE_NOT);
		break;
		default:
		break;
	}
}


/********************************************************/
/* 関数名称		: S_eeprom_event_retry_timeout			*/
/* 機能の説明	: イベント処理関数:リトライタイムアウト	*/
/* 引数			: (I/)param:イベントパラメタ			*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/10/12								*/
/* 備考			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_event_retry_timeout( UI_8 param )
{
	E_COMMON_RESULT ret;
	switch(S_eeprom_ctrl_state)
	{
		case EEPROM_STAT_WRITE_NOT:				/**< EEPROM未書き込み			*/
		break;
		case EEPROM_STAT_WRITE_NOW:				/**< EEPROM書き込み中			*/
		break;
		case EEPROM_STAT_WRITE_COMPLETE:		/**< EEPROM書き込み終了		*/
		break;
		case EEPROM_STAT_WRITE_SUSPEND:			/**< EEPROM書き込み中断		*/
			/*ファイル検索開始*/
			ret = Common_SerchVdvFile	(COMMON_VDV_FILE_TYPE_EEP, S_eeprom_SearchEepromFileCB);
			if(COMMON_RESULT_OK == ret)
			{
				/* ファイル検索正常 */
				/* コールバック待ち */
				S_eeprom_fileAccessWaitCB = N_ON;
				/* 内部初期化 */
				S_eeprom_initData();
				/* 状態変更 */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_NOT);
			}
			else
			{
				/* ファイル検索異常 */
				/* 状態変更 */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
				/* システム制御へEEPROM書換完了通知 */
				(void)System_control_NotifyEvent(SYS_EVENT_END_EEPROM_WRITE, SYS_EVENT_PARAM_NO);
			}
		break;
		default:
		break;
	}
}


/********************************************************/
/* 関数名称		: S_eeprom_ReadEepromFileCB				*/
/* 機能の説明	: ファイル読込コールバック関数			*/
/* 引数			: (I/)reqinfo:要求情報					*/
/* 				: (I/)retinfo:処理結果情報				*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/10/12								*/
/* 備考			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_ReadEepromFileCB( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo )
{
	if( (NULL != reqinfo) && (NULL != retinfo) ) {
		if( FIO_RESULT_OK == retinfo->result ) 
		{
			/* 読込正常	*/
			S_eeprom_readSize = (UI_16)retinfo->bytes;
			S_eeprom_readFileSize = (UI_16)retinfo->fsize;
			S_eeprom_event_fileread_stat = EEPROM_FILE_READ_OK;
		}
		else 
		{
			/* 読込異常	*/
			S_eeprom_event_fileread_stat = EEPROM_FILE_READ_NG;
		}
	}
	else
	{
		/* 読込失敗 */
		S_eeprom_event_fileread_stat = EEPROM_FILE_READ_NG;
	}

}



/********************************************************/
/* 関数名称		: S_eeprom_SearchEepromFileCB			*/
/* 機能の説明	: ファイル検索コールバック関数			*/
/* 引数			: (I/)pSearchResult:Vファイル検索結果	*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/10/12								*/
/* 備考			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_SearchEepromFileCB( const T_COMMON_SRC_VDVF_RET *pSearchResult )
{
	if(pSearchResult == NULL)
	{
		/* 引数異常 */
		S_eeprom_event_filesearch_stat = EEPROM_FILE_SEARCH_NOTEXIST;
		return;
	}
	if(pSearchResult->exist == N_TRUE)
	{
		/* ファイルあり */
		/* ファイルパスコピー */
		(void)memcpy(S_eeprom_filepath, pSearchResult->filepath, sizeof(pSearchResult->filepath));
		S_eeprom_event_filesearch_stat = EEPROM_FILE_SEARCH_EXIST;
	}
	else
	{
		/* ファイルなし */
		S_eeprom_event_filesearch_stat = EEPROM_FILE_SEARCH_NOTEXIST;
	}
	
}


/********************************************************/
/* 関数名称		: S_eeprom_setStatus					*/
/* 機能の説明	: 状態設定								*/
/* 引数			: status:状態							*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/10/12								*/
/* 備考			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_setStatus(E_EEPROM_CTRL_STATUS status)
{
	S_eeprom_ctrl_state = status;
	if(status == EEPROM_STAT_WRITE_SUSPEND)
	{
		/* リングバッファクリア */
		RingBuffer_Clear(&S_eeprom_ringBuffMng);
	}
}


/********************************************************/
/* 関数名称		: S_eeprom_initData						*/
/* 機能の説明	: 内部データ初期化						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/10/12								*/
/* 備考			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_initData(void)
{
	S_eeprom_writeSizeTotal = 0;
	S_eeprom_readSizeTotal = 0;
	S_eeprom_readFileSize = 0;
	S_eeprom_readSize = 0;
	/* コールバック待ち */
	S_eeprom_fileAccessWaitCB = N_OFF;

	S_eeprom_event_fileread_stat = N_OFF;
	S_eeprom_event_filesearch_stat = N_OFF;
	
	S_eeprom_retry_timerCount = 0;
	
	/* リングバッファクリア */
	RingBuffer_Clear(&S_eeprom_ringBuffMng);
	
}

/********************************************************/
/* 関数名称		: S_eeprom_writeData					*/
/* 機能の説明	: EEPROMへデータ書き込み				*/
/* 引数			: adr:書き込み開始アドレス				*/
/* 				: 		（EEPROMメモリマップのアドレス	*/
/* 				: pData:書き込みデータアドレス			*/
/* 				: dataSize:書き込みサイズ				*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/10/12								*/
/* 備考			: 書き込みサイズは2byte単位のデータを設定すること*/
/********************************************************/
EEP_LOCAL void S_eeprom_writeData(UI_16 adr, const UI_8* pData, UI_16 dataSize)
{
	UI_8 i;
	UI_16 writeData;
	UI_8 writeCount = (UI_8)(dataSize / sizeof(UI_16));
	
	/* EEPROMドライバ有効 */
	EepDrv_Enable();
	
	for(i = 0 ; i < writeCount ; i++)
	{
		/* バイト配列から16bitのデータ取得 */
		writeData = Common_Get16(pData);
		/* EEPROMデータ書き込み */
		EepDrv_WriteDataWord(adr,writeData);
		pData += sizeof(UI_16);
		adr++;
	}
	/* EEPROMドライバ無効 */
	EepDrv_Disable();
	
	
	
}


/********************************************************/
/* 関数名称		: S_eeprom_readData						*/
/* 機能の説明	: EEPROMからデータ読み込み				*/
/* 引数			: adr:書き込み開始アドレス				*/
/* 				: 		（EEPROMメモリマップのアドレス	*/
/* 				: pData:読み込みデータアドレス			*/
/* 戻り値		: N_OK	:正常							*/
/*				: N_NG	:異常							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/10/12								*/
/* 備考			: 										*/
/********************************************************/
EEP_LOCAL SI_8 S_eeprom_readData(UI_16 adr, UI_16* pData)
{
	UI_8 i;
	UI_16 data[5];
	UI_16 wordData;
	UI_8  parityResult;
	SI_8  ret = N_OK;

	/* データ取得 */
	wordData = EepDrv_ReadDataWord(adr);
	if((adr == EEPROM_ADR_VARIATION) ||
#if defined _VDV_SUPRA /* 2018.09.11 S.Sakata */
	   (adr == EEPROM_ADR_VEHICLETYPE) ||
#endif /* defined _VDV_SUPRA 2018.09.11 S.Sakata */
	   (adr == EEPROM_ADR_VEHICLEINFO) ||
		(adr == EEPROM_ADR_SERIAL_NO))
	{
		/* 仕向情報 */
		/* パリティ計算用にデータ取得 */
		for (i = 0 ; i < (sizeof(data) / sizeof(UI_16)) ; i++)
		{
			data[i] = EepDrv_ReadDataWord( adr + i);
		}
		/* パリティチェック */
		parityResult = parity_check8(data, (sizeof(data) / sizeof(UI_16)));
		if(parityResult == PARITY_NORMAL)
		{
			*pData = wordData;
		}
		else
		{
			if (adr == EEPROM_ADR_VARIATION) {
				*pData = EEPROM_DEFLT_VAL_VARIATION;
			}
#if defined _VDV_SUPRA /* 2018.09.11 S.Sakata */
			else if (adr == EEPROM_ADR_VEHICLETYPE) {
				*pData = EEPROM_DEFLT_VAL_VEHICLETYPE;
			}
#endif /* defined _VDV_SUPRA 2018.09.11 S.Sakata */
			else if (adr == EEPROM_ADR_VEHICLEINFO) {
				*pData = EEPROM_DEFLT_VAL_VEHICLEJUDGE;
			}
			else {
				*pData = EEPROM_DEFLT_VAL_SERIAL_NO;
			}
			ret = N_NG;
		}
	}
	else if(adr == EEPROM_ADR_BAT)
	{
		/* バッテリー電圧 */
		/* パリティ計算用にデータ取得 */
		for (i = 0 ; i < (sizeof(data) / sizeof(UI_16)) ; i++)
		{
			data[i] = EepDrv_ReadDataWord( adr + i);
		}
		/* パリティチェック */
		parityResult = parity_check8(data, (sizeof(data) / sizeof(UI_16)));
		if(parityResult == PARITY_NORMAL)
		{
			/* bit8-15なので8bitシフト */
			*pData = (wordData >> 8);
		}
		else
		{
			*pData = EEPROM_DEFLT_VAL_BAT;
			ret = N_NG;
		}
	}
	else if(adr == EEPROM_ADR_FIRMWARE_VER)
	{
		if (wordData == 0xFFFFU)
		{
			*pData = EEPROM_DEFLT_VAL_FIRMWARE_VER;
		}
		else
		{
			*pData = wordData;
		}
	}
	else if(adr == EEPROM_ADR_EEPROM_VER)
	{
		if (wordData == 0xFFFFU)
		{
			*pData = EEPROM_DEFLT_VAL_EEPROM_VER;
		}
		else
		{
			*pData = wordData;
		}
	}
	else
	{
		*pData = wordData;
	}

	return ret;
}


/*==========================================================================*/
/*	8bitパリティ算出/補正処理												*/
/*--------------------------------------------------------------------------*/
/*	UI_16 parity8(UI_16 *p, SI_8 n)											*/
/*--------------------------------------------------------------------------*/
/*	引数:	UI_16	*p		parity算出対象データの先頭アドレス				*/
/*			SI_8	n		データのワード数(1～4)							*/
/*	戻値:	parity算出結果													*/
/*--------------------------------------------------------------------------*/
/*	機能:	endianに依存しない8bit単位のparity計算							*/
/*																			*/
/*		データに対するparity算出結果は以下のとおり							*/
/*																			*/
/*				(b7-0)														*/
/*				 b15-8 b7-0													*/
/*		addr	+-----+-----+												*/
/*		+0		|  H  |  L  |												*/
/*				+-----+-----+												*/
/*		+1		|  H  |  L  |												*/
/*				+-----+-----+												*/
/*		～																	*/
/*				+-----+-----+												*/
/*		+(n-1)	|  H  |  L  |												*/
/*				+-----+-----+												*/
/*																			*/
/*																			*/
/*	|b15 b14 b13 b12 b11 b10 b9  b8 | b7 b6  b5  b4  b3  b2  b1  b0 |		*/
/*	|			水平parity			|			垂直parity			|		*/
/*	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+		*/
/*	|+3L|+3H|+2L|+2H|+1L|+1H|+0L|+0H| b7| b6| b5| b4| b3| b2| b1| b0|		*/
/*	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+		*/
/*																			*/
/*--------------------------------------------------------------------------*/
/*	備考:	データワード数 n に以下の値を指定した場合の動作は保証しません	*/
/*				1より小さい値												*/
/*				4より大きい値												*/
/*--------------------------------------------------------------------------*/
/*	author:	2002/07/25	YAMAZAKI Iwao										*/
/*==========================================================================*/
EEP_LOCAL UI_16 parity8(UI_16 *p, SI_8 n)
{
	UI_16 *q;
	UI_16 t;
	UI_16 v, h;

	q = p + (n-1);						/* 算出対象ブロック最終位置			*/

	h = 0x0000;							/* 水平parity 偶数					*/
	v = 0x00ff;							/* 垂直parity 奇数					*/
										/*   初期値 0x0000: 偶数parity		*/
										/*          0x00ff: 奇数parity		*/
	do {
		t = *q;							/* ワードデータ取得					*/
		v ^= t;							/* 垂直parity算出					*/
		t ^= (UI_16)(t >> 4);			/* 水平parity算出					*/
		t ^= (UI_16)(t >> 2);
		t ^= (UI_16)(t >> 1);
		t &= 0x0101;					/* 0x0101の位置にparityが出る		*/
		h = (UI_16)((h<<1) | (t<<8));	/* 下位byteの水平parity				*/
		h = (UI_16)((h<<1) | t);		/* 上位byteの水平parity				*/
		q--;							/* 先頭に向かってデータ更新			*/
	} while (q >= p);					/* データ先頭まで計算を続ける		*/
	v = (UI_16)((v>>8) ^ v);			/* 上位,下位byteの垂直parityを合成	*/

	return (UI_16)((h & 0xff00U) | (v & 0x00ffU));
}

/*==========================================================================*/
/*	8bitパリティチェック/補正処理（未使用領域無視）							*/
/*--------------------------------------------------------------------------*/
/*	UI_8 parity_check8(UI_16 *p, SI_8 n)									*/
/*--------------------------------------------------------------------------*/
/*	引数:	UI_16	*p		parityチェック対象ブロックの先頭アドレス		*/
/*			UI_8	n		ブロックのワード数(2～5)						*/
/*	戻値:	parityチェック結果	= PARITY_NORMAL		:	正常				*/
/*								= PARITY_ERROR_1BIT	:	1bit異常			*/
/*								= PARITY_ERROR_2BIT :	異常				*/
/*--------------------------------------------------------------------------*/
/*	機能:	parityの未使用領域をデータとして使用しないタイプの				*/
/*			endianに依存しない8bit単位のparityチェック						*/
/*																			*/
/*		対象とするブロックの構造は以下のとおり								*/
/*																			*/
/*				(b7-0)														*/
/*				 b15-8 b7-0													*/
/*		addr	+-----+-----+												*/
/*		+0		|  H  |  L  |												*/
/*				+-----+-----+												*/
/*		+1		|  H  |  L  |												*/
/*				+-----+-----+												*/
/*		～																	*/
/*				+-----+-----+												*/
/*		+(n-2)	|  H  |  L  |												*/
/*				+-+---+-----+												*/
/*		+(n-1)	|/|PH | PV  |												*/
/*				+-+---+-----+												*/
/*																			*/
/*																			*/
/*	|b15 b14 b13 b12 b11 b10 b9  b8 | b7 b6  b5  b4  b3  b2  b1  b0 |		*/
/*	|			水平parity			|			垂直parity			|		*/
/*	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+		*/
/*	|+3L|+3H|+2L|+2H|+1L|+1H|+0L|+0H| b7| b6| b5| b4| b3| b2| b1| b0|		*/
/*	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+		*/
/*																			*/
/*--------------------------------------------------------------------------*/
/*	備考:	ブロックワード数 n に以下の値を指定した場合の動作は保証しません	*/
/*				2より小さい値												*/
/*				5より大きい値												*/
/*--------------------------------------------------------------------------*/
/*	author:	2002/07/25	YAMAZAKI Iwao										*/
/*	modify:	2010/11/15	YASHIRO Hidetaka									*/
/*==========================================================================*/
EEP_LOCAL UI_8 parity_check8(UI_16 *p, SI_8 n)
{
	UI_16 v, h;
	UI_8 ret;
	static const UI_16 mask[] = {
		0x03ffU, 0x0fffU, 0x3fffU, 0xffffU,
	};

	v = (UI_16)(p[n-1] & mask[n-2]);	/* parityの記憶値取得				*/
	v ^= parity8(p, (SI_8)(n-1));		/* parityの算出値と記憶値を比較		*/

	h = (UI_16)(v >> 8);				/* 水平parityの相違					*/
	v = (UI_16)(v & 0x00ff);			/* 垂直parityの相違					*/

	if (((v & (v-1)) != 0U) || ((h & (h-1)) != 0U)){
		ret = PARITY_ERROR_2BIT;		/* 2bit以上の相違→異常				*/
	}else if ((v == 0) && (h == 0)){
		ret = PARITY_NORMAL;			/* 異常なし							*/
	}else if ((v == 0) || (h == 0)){
		ret = PARITY_ERROR_1BIT;		/* 1bitの相違→parity部の故障		*/
	}else{

		/*----------------------------------*/
		/* 以下水平、垂直parityにそれぞれ	*/
		/* 1bitの相違→データ部の1bit故障	*/
		/* 故障bitの補正を行う				*/
		/*----------------------------------*/
		n = 0;
		if ((h & 0xcc) != 0U){
			n += 1;							/* 水平parityの相違から故障bitを検出*/
		}
		if ((h & 0xf0) != 0U){
			n += 2;
		}
		if ((h & 0x55) != 0U){
			v <<= 8;						/* 上位byteの場合、上位に移動		*/
		}
		p[n] ^= v;							/* 故障bitの補正					*/

		ret = PARITY_ERROR_1BIT;
	}
	return ret;
}





