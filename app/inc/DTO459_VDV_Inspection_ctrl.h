/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: FR81S								*/
/* Date				: 17/01/20							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Toguchi						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		:									*/
/* date				:									*/
/********************************************************/
/********************************************************/
/* file DTO459_VDV_Inspection_ctrl.h */
/* 検査制御 */
/********************************************************/
#ifndef	DTO459_VDV_INSPECTION_CTRL_H
#define	DTO459_VDV_INSPECTION_CTRL_H

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include "type.h"

/********************************************************/
/*	定数定義											*/
/********************************************************/
/* 検査結果 */
typedef enum e_insp_inspection_result {
	INSP_INSPECTION_RESULT_NG = 0,					/* NG									*/
	INSP_INSPECTION_RESULT_OK,						/* OK									*/
	INSP_INSPECTION_RESULT_INVALID					/* 無効値								*/
} E_INSP_INSPECTION_RESULT;

/* 検査種別 */
typedef enum e_insp_inspection_type {
	INSP_INSPECTION_TYPE_START = 0,					/* 検査開始								*/
	INSP_INSPECTION_TYPE_STOP,						/* 検査停止								*/
	INSP_INSPECTION_TYPE_SERIAL_NO_WR,				/* シリアルNo書き込み					*/
	INSP_INSPECTION_TYPE_SERIAL_NO_RD,				/* シリアルNo読み出し					*/
	INSP_INSPECTION_TYPE_BT_PAIRING,				/* Bluetoothペアリング					*/
	INSP_INSPECTION_TYPE_BT_CONNECT,				/* Bluetooth通信						*/
	INSP_INSPECTION_TYPE_SD,						/* SDカード								*/
	INSP_INSPECTION_TYPE_CAN0,						/* CAN0									*/
	INSP_INSPECTION_TYPE_CAN1,						/* CAN1									*/
	INSP_INSPECTION_TYPE_UART,						/* UART									*/
	INSP_INSPECTION_TYPE_SW,						/* SW									*/
	INSP_INSPECTION_TYPE_GNSS,						/* GNSS									*/

	INSP_INSPECTION_TYPE_NUM,						/* 検査種別数							*/
	INSP_INSPECTION_TYPE_INVLID						/* 無効値								*/
} E_INSP_INSPECTION_TYPE;

/* LEDモード */
typedef enum e_insp_led_mode {
	INSP_LED_MODE_OFF = 0,							/* 消灯									*/
	INSP_LED_MODE_NOMAL,							/* 通常点灯								*/
	INSP_LED_MODE_BT_PAIRING,						/* Buletoothペアリング					*/
	INSP_LED_MODE_SW_INSPECT						/* SW検査								*/
} E_INSP_LED_MODE;

/********************************************************/
/*	構造体定義											*/
/********************************************************/
/* 検査パラメタ	*/
typedef struct t_insp_inspection_param {
	UI_8		data1;					/* データ１(1byte)						*/
	UI_8		data2;					/* データ２(1byte)						*/
	UI_8		rsv[2];
	UI_32		data3;					/* データ３(4byte)						*/
} T_INSP_INSPECTION_PARAM;

/* デバッグ用定義 */

/********************************************************/
/*	外部関数											*/
/********************************************************/
void Insp_control_Init(void);
void Insp_control_Main(void);
void Insp_notify_inspection_result(E_INSP_INSPECTION_TYPE type, const T_INSP_INSPECTION_PARAM *param, E_INSP_INSPECTION_RESULT result);
E_INSP_LED_MODE Insp_get_led_mode(void);

#endif	/*DTO459_VDV_INSPECTION_CTRL_H*/
