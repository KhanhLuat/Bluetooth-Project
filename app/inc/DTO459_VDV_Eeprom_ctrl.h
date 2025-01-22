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
/* file DTO459_VDV_Eeprom_ctrl.h						*/
/* EEPROM制御											*/
/********************************************************/
#ifndef	DTO459_VDV_EEPROM_CTRL_H
#define	DTO459_VDV_EEPROM_CTRL_H

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include "type.h"

/********************************************************/
/*	マクロ定義											*/
/********************************************************/
/**
 **	処理結果
 */
typedef enum e_eeprom_control_result {
	EEPROM_CONTROL_RESULT_OK = 0,				/**< 正常終了	*/
	EEPROM_CONTROL_RESULT_ERR_INVALID_ARG,		/**< 引数不正	*/
	EEPROM_CONTROL_RESULT_ERR_INVALID_STATE,	/**< 状態不正	*/
	EEPROM_CONTROL_RESULT_ERR_BUSY,				/**< ビシー		*/
	EEPROM_CONTROL_RESULT_ERR_FATAL,			/**< 異常終了	*/

	EEPROM_CONTROL_RESULT_ERR_NUM				/**< 状態数		*/
} E_EEPROM_CONTROL_RESULT;


/**
 **	データ種別
 */
typedef enum e_eeprom_data_type {
	EEPROM_DATA_FIRMWARE_VER = 0,	/**< ファームウェアバージョン	*/
	EEPROM_DATA_EEPROM_VER,			/**< EEPROMウェアバージョン		*/
	EEPROM_DATA_VARIATION,			/**< 仕向け情報					*/
	EEPROM_DATA_BAT,				/**< バッテリー電圧補正値		*/
	EEPROM_DATA_SERIAL_NO,			/**< シリアルNo					*/
#if defined _VDV_SUPRA /* 2018.09.11 S.Sakata */
	EEPROM_DATA_VEHICLETYPE,		/**< 車両型情報					*/
#endif /* defined _VDV_SUPRA 2018.09.11 S.Sakata */
	EEPROM_DATA_VEHICLEINFO,		/**< 車種両情報					*/
	EEPROM_DATA_NUM					/**< データ種別数				*/
} E_EEPROM_DATA_TYPE;




/********************************************************/
/*	コールバック関数									*/
/********************************************************/
/********************************************************/
/* 機能の説明	: Eeprom_control_SetDataのコールバック関数	*/
/* 引数			: (I/ )データ種別  E_EEPROM_DATA_TYPE	*/
/* 				: (I/ )設定データ  						*/
/*				: (I/ )書き込み結果						*/
/*							(N_OK:正常, N_NG:異常)		*/
/* 戻り値		: void									*/
/* 備考			: 										*/
/********************************************************/
typedef void (*PFN_EEPROM_SETDATA_CB)(E_EEPROM_DATA_TYPE type, UI_16 data, SI_8 result);

/********************************************************/
/*	内部変数定義										*/
/********************************************************/

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
void Eeprom_control_Init(void);

/********************************************************/
/* 関数名称		: Eeprom_control_Main					*/
/* 機能の説明	: EEPROM制御メイン						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Eeprom_control_Main(void);

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
E_EEPROM_CONTROL_RESULT Eeprom_control_SetData(E_EEPROM_DATA_TYPE type, UI_16 data, PFN_EEPROM_SETDATA_CB cbfnc);

/********************************************************/
/* 関数名称		: Eeprom_control_GetData				*/
/* 機能の説明	: EEPROM制御メイン						*/
/* 引数			: (I/ )データ種別  E_EEPROM_DATA_TYPE	*/
/* 				: ( /O)取得データ  						*/
/* 戻り値		: 処理結果 E_EEPROM_CONTROL_RESULT		*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
E_EEPROM_CONTROL_RESULT Eeprom_control_GetData(E_EEPROM_DATA_TYPE type, UI_16* pData);

#endif	/*DTO459_VDV_EEPROM_CTRL_H*/
