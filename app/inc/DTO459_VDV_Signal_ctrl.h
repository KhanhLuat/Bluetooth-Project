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
/* file DTO459_VDV_Signal_ctrl.h						*/
/* 公開信号制御											*/
/********************************************************/
#ifndef	DTO459_VDV_SIGNAL_CTRL_H
#define	DTO459_VDV_SIGNAL_CTRL_H

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include "type.h"

/********************************************************/
/*	マクロ定義											*/
/********************************************************/

/* 
 ** デバイス種別
 */ 
typedef enum e_signal_device_type
{
	DEVICE_TYPE_BT = 0,				/**< デバイス種別:BT	*/
	DEVICE_TYPE_SD,					/**< デバイス種別:SD	*/
	DEVICE_TYPE_CAN,				/**< デバイス種別:CAN	*/
	DEVICE_TYPE_NUM					/**< デバイス種別数		*/
} E_SIGNAL_DEVICE_TYPE;

/* 
 ** 信号ID種別
 */ 
typedef enum e_signal_id_type
{
	ID_TYPE_FIXED_ID = 0,			/**< 信号種別:固定値	*/
	ID_TYPE_VARIABLE_ID				/**< 信号種別:変数値	*/
} E_SIGNAL_ID_TYPE;

/********************************************************/
/*	内部変数定義										*/
/********************************************************/

/********************************************************/
/*	外部関数											*/
/********************************************************/

/********************************************************/
/* 関数名称		: Signal_control_Init					*/
/* 機能の説明	: 公開信号制御初期化					*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Signal_control_Init(void);


/********************************************************/
/* 関数名称		: Signal_control_GetSignalVersion		*/
/* 機能の説明	: 公開信号バージョン					*/
/* 引数			: void									*/
/* 戻り値		: 公開信号バージョン					*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
UI_32 Signal_control_GetSignalVersion(void);


/********************************************************/
/* 関数名称		: Signal_control_GetRandomValue			*/
/* 機能の説明	: 乱数取得								*/
/* 引数			: void									*/
/* 戻り値		: 乱数									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_32 Signal_control_GetRandomValue(void);


/********************************************************/
/* 関数名称		: Signal_control_GetEncodeKey			*/
/* 機能の説明	: 暗号化キー取得						*/
/* 引数			: (I/ )deviceType 		デバイス種別	*/
/*				: ( /O)key				暗号化用キー	*/
/* 戻り値		: 正常	:N_OK							*/
/*				: 異常	:N_NG							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Signal_control_GetEncodeKey(E_SIGNAL_DEVICE_TYPE deviceType, SI_32 *key);


/********************************************************/
/* 関数名称		: Signal_control_SetEncodeKey			*/
/* 機能の説明	: 暗号化キー設定						*/
/* 引数			: (I/ )deviceType 		デバイス種別	*/
/*				: (I/ )key				暗号化用キー	*/
/* 戻り値		: 正常	:N_OK							*/
/*				: 異常	:N_NG							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Signal_control_SetEncodeKey(E_SIGNAL_DEVICE_TYPE deviceType, SI_32 key);

/********************************************************/
/* 関数名称		: Signal_control_GetSignalInfo			*/
/* 機能の説明	: 公開信号取得							*/
/* 引数			: (I/ )deviceType 		デバイス種別	*/
/* 				: (I/ )req_id 			信号ID			*/
/*				: ( /O)signal_value	信号値				*/
/* 戻り値		: 正常	:N_OK							*/
/*				: 異常	:N_NG							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Signal_control_GetSignalInfo(E_SIGNAL_DEVICE_TYPE deviceType, UI_16 req_id, SI_32 *signal_value);

/********************************************************/
/* 関数名称		: Signal_control_GetSignalsInfo			*/
/* 機能の説明	: 公開信号取得(複数信号)				*/
/* 引数			: (I/ )deviceType 		デバイス種別	*/
/* 				: (I/ )req_id_count	信号ID数			*/
/* 				: (I/ )req_ids 		信号ID（複数）		*/
/*				: ( /O)signal_values	信号値（複数）	*/
/* 戻り値		: 正常	:N_OK							*/
/*				: 異常	:N_NG							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Signal_control_GetSignalsInfo(E_SIGNAL_DEVICE_TYPE deviceType, UI_8 req_id_count, const UI_16* req_ids, SI_32 *signal_values);


/********************************************************/
/* 関数名称		: Signal_control_GetSignalList			*/
/* 機能の説明	: 公開信号ID一覧取得					*/
/* 引数			: (I/ )deviceType 		デバイス種別	*/
/*				: (I/ )idtype			信号種別		*/
/*				: ( /O)signal_Num		信号数			*/
/*				: ( /O)signal_List		信号リスト		*/
/* 戻り値		: 正常	:N_OK							*/
/*				: 異常	:N_NG							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Signal_control_GetSignalList(E_SIGNAL_DEVICE_TYPE deviceType, E_SIGNAL_ID_TYPE idtype, UI_8 *signal_Num, const UI_16 **signal_List);


/********************************************************/
/* 関数名称		: Signal_control_isGnssEnable			*/
/* 機能の説明	: GNSS信号受信済判定					*/
/* 引数			: void									*/
/* 戻り値		: TRUE:受信済	 FALSE:未受信			*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
B_8 Signal_control_isGnssEnable(void);

/********************************************************/
/* 関数名称		: Signal_control_isCanEnable			*/
/* 機能の説明	: CAN信号受信済判定						*/
/* 引数			: void									*/
/* 戻り値		: TRUE:受信済	 FALSE:未受信			*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
B_8 Signal_control_isCanEnable(void);


#endif	/*DTO459_VDV_SIGNAL_CTRL_H*/
