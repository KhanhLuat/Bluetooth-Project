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
/* �������� */
/********************************************************/
#ifndef	DTO459_VDV_INSPECTION_CTRL_H
#define	DTO459_VDV_INSPECTION_CTRL_H

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include "type.h"

/********************************************************/
/*	�萔��`											*/
/********************************************************/
/* �������� */
typedef enum e_insp_inspection_result {
	INSP_INSPECTION_RESULT_NG = 0,					/* NG									*/
	INSP_INSPECTION_RESULT_OK,						/* OK									*/
	INSP_INSPECTION_RESULT_INVALID					/* �����l								*/
} E_INSP_INSPECTION_RESULT;

/* ������� */
typedef enum e_insp_inspection_type {
	INSP_INSPECTION_TYPE_START = 0,					/* �����J�n								*/
	INSP_INSPECTION_TYPE_STOP,						/* ������~								*/
	INSP_INSPECTION_TYPE_SERIAL_NO_WR,				/* �V���A��No��������					*/
	INSP_INSPECTION_TYPE_SERIAL_NO_RD,				/* �V���A��No�ǂݏo��					*/
	INSP_INSPECTION_TYPE_BT_PAIRING,				/* Bluetooth�y�A�����O					*/
	INSP_INSPECTION_TYPE_BT_CONNECT,				/* Bluetooth�ʐM						*/
	INSP_INSPECTION_TYPE_SD,						/* SD�J�[�h								*/
	INSP_INSPECTION_TYPE_CAN0,						/* CAN0									*/
	INSP_INSPECTION_TYPE_CAN1,						/* CAN1									*/
	INSP_INSPECTION_TYPE_UART,						/* UART									*/
	INSP_INSPECTION_TYPE_SW,						/* SW									*/
	INSP_INSPECTION_TYPE_GNSS,						/* GNSS									*/

	INSP_INSPECTION_TYPE_NUM,						/* ������ʐ�							*/
	INSP_INSPECTION_TYPE_INVLID						/* �����l								*/
} E_INSP_INSPECTION_TYPE;

/* LED���[�h */
typedef enum e_insp_led_mode {
	INSP_LED_MODE_OFF = 0,							/* ����									*/
	INSP_LED_MODE_NOMAL,							/* �ʏ�_��								*/
	INSP_LED_MODE_BT_PAIRING,						/* Buletooth�y�A�����O					*/
	INSP_LED_MODE_SW_INSPECT						/* SW����								*/
} E_INSP_LED_MODE;

/********************************************************/
/*	�\���̒�`											*/
/********************************************************/
/* �����p�����^	*/
typedef struct t_insp_inspection_param {
	UI_8		data1;					/* �f�[�^�P(1byte)						*/
	UI_8		data2;					/* �f�[�^�Q(1byte)						*/
	UI_8		rsv[2];
	UI_32		data3;					/* �f�[�^�R(4byte)						*/
} T_INSP_INSPECTION_PARAM;

/* �f�o�b�O�p��` */

/********************************************************/
/*	�O���֐�											*/
/********************************************************/
void Insp_control_Init(void);
void Insp_control_Main(void);
void Insp_notify_inspection_result(E_INSP_INSPECTION_TYPE type, const T_INSP_INSPECTION_PARAM *param, E_INSP_INSPECTION_RESULT result);
E_INSP_LED_MODE Insp_get_led_mode(void);

#endif	/*DTO459_VDV_INSPECTION_CTRL_H*/
