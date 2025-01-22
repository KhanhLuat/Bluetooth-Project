/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: FR81S								*/
/* Date				: 16/10/24							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Katsura						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		:									*/
/* date				:									*/
/********************************************************/
/********************************************************/
/* file DTO459_VDV_Iso_canctrl.h						*/
/* Local CAN制御										*/
/********************************************************/
#ifndef	DTO459_VDV_ISO_CAN_CTRL_H
#define	DTO459_VDV_ISO_CAN_CTRL_H

#ifdef ISOCAN_ENABLE
/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include "type.h"

/********************************************************/
/*	マクロ定義											*/
/********************************************************/
typedef enum e_iso_can_result {
	ISO_CAN_CTRL_OK = 0,			/**< 正常値		*/
	ISO_CAN_CTRL_ERROR,				/**< 異常値		*/
	ISO_CAN_CTRL_NO_RECEIVE			/**< 未受信		*/
} E_ISO_CAN_RESULT;
/********************************************************/
/*	内部変数定義										*/
/********************************************************/

/********************************************************/
/*	外部関数											*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_eng_cool_temp(UI_8 *data);		/* エンジン水温情報取得 		*/
E_ISO_CAN_RESULT Iso_can_ctrl_get_im_abs_press(UI_8 *data);			/* 吸気マニフォールド圧情報取得 */
E_ISO_CAN_RESULT Iso_can_ctrl_get_eng_rpm(UI_16 *data);				/* エンジン回転数情報取得 		*/
E_ISO_CAN_RESULT Iso_can_ctrl_get_vehicle_speed(UI_8 *data);		/* 車速情報取得 				*/
E_ISO_CAN_RESULT Iso_can_ctrl_get_intake_air_temp(UI_8 *data);		/* 吸気温情報取得				*/
E_ISO_CAN_RESULT Iso_can_ctrl_get_throttle_pos(UI_8 *data);			/* スロットルポジション情報取得 */
E_ISO_CAN_RESULT Iso_can_ctrl_get_ctrl_mod_volt(UI_16 *data);		/* 電圧情報取得					*/

/********************************************************/
/* 関数名称		: Iso_can_ctrl_Init						*/
/* 機能の説明	: ISO CAN制御初期化						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/10/24								*/
/* 備考			: 										*/
/********************************************************/
void Iso_can_ctrl_Init(void);

/********************************************************/
/* 関数名称		: Iso_can_ctrl_Main						*/
/* 機能の説明	: ISO CAN制御メイン						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/10/11								*/
/* 備考			: 										*/
/********************************************************/
void Iso_can_ctrl_Main(void);

#endif /* ISOCAN_ENABLE */
#endif	/* DTO459_VDV_ISO_CAN_CTRL_H */
