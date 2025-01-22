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
/* file DTO459_VDV_Sd_ctrl.h							*/
/* SD制御												*/
/********************************************************/
#ifndef	DTO459_VDV_SD_CTRL_H
#define	DTO459_VDV_SD_CTRL_H

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include "type.h"
#include "DTO459_VDV_Signal_ctrl.h"


/********************************************************/
/*	マクロ定義											*/
/********************************************************/
/* 
 ** 処理種別
 */ 
typedef enum sd_process_type
{
	SD_PROCESS_MAIN	= 0,	/**< 通常処理	10ms毎		*/
	SD_PROCESS_SUB			/**< サブ処理	25ms毎		*/
} SD_PROCESS_TYPE;

/* 
 ** REC状態
 */ 
typedef enum e_sd_control_stat
{
	SD_CONTROL_REC_STAT_UNKNOWN = 0	,		/**< SDモジュール状態:未確定			*/
	SD_CONTROL_REC_STAT_RECORDING	,		/**< SDモジュール状態:REC中				*/
	SD_CONTROL_REC_STAT_UNRECORDING	,		/**< SDモジュール状態:REC停止中			*/

	SD_CONTROL_REC_STAT_NUM					/**< SDモジュール状態数					*/
} E_SD_CONTROL_REC_STAT;


/**
 **	処理結果
 */
typedef enum e_sd_control_result {
	SD_CONTROL_RESULT_OK = 0,				/**< 正常終了	*/
	SD_CONTROL_RESULT_ERR_INVALID_ARG,		/**< 引数不正	*/
	SD_CONTROL_RESULT_ERR_INVALID_STATE,	/**< 状態不正	*/
	SD_CONTROL_RESULT_ERR_BUSY,				/**< ビシー		*/
	SD_CONTROL_RESULT_ERR_FATAL,			/**< 異常終了	*/

	SD_CONTROL_RESULT_ERR_NUM				/**< 状態数		*/
} E_SD_CONTROL_RESULT;


/********************************************************/
/*	構造体定義											*/
/********************************************************/


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
void Sd_control_Init(void);

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
void Sd_control_Main(SD_PROCESS_TYPE processType);


/************************************************************/
/* 関数名称		: Sd_control_GetRecStatus					*/
/* 機能の説明	: SD REC状態取得							*/
/* 引数			: void										*/
/* 戻り値		: 処理結果 E_SD_CONTROL_REC_STAT			*/
/* 作成者		: (OFT)Sakata								*/
/* 作成日		: 16/08/18									*/
/* 備考			: 											*/
/************************************************************/
E_SD_CONTROL_REC_STAT Sd_control_GetRecStatus(void);


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
SI_8 Sd_control_GetSignalList(E_SIGNAL_ID_TYPE idtype, UI_8 *signal_Num, const UI_16 **signal_List);

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
SI_8 Sd_control_GetSignalsInfo( UI_8 req_id_count, const UI_16* req_ids, SI_32 *signal_values);

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
SI_8 Sd_control_GetEncodeKey(SI_32 *key);

/********************************************************/
/* 関数名称		: Sd_control_GetSignalVersion			*/
/* 機能の説明	: 公開信号バージョン					*/
/* 引数			: void									*/
/* 戻り値		: 公開信号バージョン					*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
UI_32 Sd_control_GetSignalVersion(void);

/********************************************************/
/* 関数名称		: Sd_control_GetInspectionVariation		*/
/* 機能の説明	: 検査モードのバリエーション情報取得	*/
/* 引数			: variation	バリエーション情報			*/
/* 戻り値		: 正常	:N_OK							*/
/*				  異常	:N_NG							*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Sd_control_GetInspectionVariation(UI_8 *variation);

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
SI_8 Sd_control_ReqInspection(UI_16 serialno);

#endif	/*DTO459_VDV_SD_CTRL_H*/
