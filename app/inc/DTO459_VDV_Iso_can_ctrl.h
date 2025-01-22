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
/* Local CAN����										*/
/********************************************************/
#ifndef	DTO459_VDV_ISO_CAN_CTRL_H
#define	DTO459_VDV_ISO_CAN_CTRL_H

#ifdef ISOCAN_ENABLE
/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include "type.h"

/********************************************************/
/*	�}�N����`											*/
/********************************************************/
typedef enum e_iso_can_result {
	ISO_CAN_CTRL_OK = 0,			/**< ����l		*/
	ISO_CAN_CTRL_ERROR,				/**< �ُ�l		*/
	ISO_CAN_CTRL_NO_RECEIVE			/**< ����M		*/
} E_ISO_CAN_RESULT;
/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/

/********************************************************/
/*	�O���֐�											*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_eng_cool_temp(UI_8 *data);		/* �G���W���������擾 		*/
E_ISO_CAN_RESULT Iso_can_ctrl_get_im_abs_press(UI_8 *data);			/* �z�C�}�j�t�H�[���h�����擾 */
E_ISO_CAN_RESULT Iso_can_ctrl_get_eng_rpm(UI_16 *data);				/* �G���W����]�����擾 		*/
E_ISO_CAN_RESULT Iso_can_ctrl_get_vehicle_speed(UI_8 *data);		/* �ԑ����擾 				*/
E_ISO_CAN_RESULT Iso_can_ctrl_get_intake_air_temp(UI_8 *data);		/* �z�C�����擾				*/
E_ISO_CAN_RESULT Iso_can_ctrl_get_throttle_pos(UI_8 *data);			/* �X���b�g���|�W�V�������擾 */
E_ISO_CAN_RESULT Iso_can_ctrl_get_ctrl_mod_volt(UI_16 *data);		/* �d�����擾					*/

/********************************************************/
/* �֐�����		: Iso_can_ctrl_Init						*/
/* �@�\�̐���	: ISO CAN���䏉����						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/10/24								*/
/* ���l			: 										*/
/********************************************************/
void Iso_can_ctrl_Init(void);

/********************************************************/
/* �֐�����		: Iso_can_ctrl_Main						*/
/* �@�\�̐���	: ISO CAN���䃁�C��						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/10/11								*/
/* ���l			: 										*/
/********************************************************/
void Iso_can_ctrl_Main(void);

#endif /* ISOCAN_ENABLE */
#endif	/* DTO459_VDV_ISO_CAN_CTRL_H */
