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
/* EEPROM����											*/
/********************************************************/
#ifndef	DTO459_VDV_EEPROM_CTRL_H
#define	DTO459_VDV_EEPROM_CTRL_H

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include "type.h"

/********************************************************/
/*	�}�N����`											*/
/********************************************************/
/**
 **	��������
 */
typedef enum e_eeprom_control_result {
	EEPROM_CONTROL_RESULT_OK = 0,				/**< ����I��	*/
	EEPROM_CONTROL_RESULT_ERR_INVALID_ARG,		/**< �����s��	*/
	EEPROM_CONTROL_RESULT_ERR_INVALID_STATE,	/**< ��ԕs��	*/
	EEPROM_CONTROL_RESULT_ERR_BUSY,				/**< �r�V�[		*/
	EEPROM_CONTROL_RESULT_ERR_FATAL,			/**< �ُ�I��	*/

	EEPROM_CONTROL_RESULT_ERR_NUM				/**< ��Ԑ�		*/
} E_EEPROM_CONTROL_RESULT;


/**
 **	�f�[�^���
 */
typedef enum e_eeprom_data_type {
	EEPROM_DATA_FIRMWARE_VER = 0,	/**< �t�@�[���E�F�A�o�[�W����	*/
	EEPROM_DATA_EEPROM_VER,			/**< EEPROM�E�F�A�o�[�W����		*/
	EEPROM_DATA_VARIATION,			/**< �d�������					*/
	EEPROM_DATA_BAT,				/**< �o�b�e���[�d���␳�l		*/
	EEPROM_DATA_SERIAL_NO,			/**< �V���A��No					*/
#if defined _VDV_SUPRA /* 2018.09.11 S.Sakata */
	EEPROM_DATA_VEHICLETYPE,		/**< �ԗ��^���					*/
#endif /* defined _VDV_SUPRA 2018.09.11 S.Sakata */
	EEPROM_DATA_VEHICLEINFO,		/**< �Ԏ헼���					*/
	EEPROM_DATA_NUM					/**< �f�[�^��ʐ�				*/
} E_EEPROM_DATA_TYPE;




/********************************************************/
/*	�R�[���o�b�N�֐�									*/
/********************************************************/
/********************************************************/
/* �@�\�̐���	: Eeprom_control_SetData�̃R�[���o�b�N�֐�	*/
/* ����			: (I/ )�f�[�^���  E_EEPROM_DATA_TYPE	*/
/* 				: (I/ )�ݒ�f�[�^  						*/
/*				: (I/ )�������݌���						*/
/*							(N_OK:����, N_NG:�ُ�)		*/
/* �߂�l		: void									*/
/* ���l			: 										*/
/********************************************************/
typedef void (*PFN_EEPROM_SETDATA_CB)(E_EEPROM_DATA_TYPE type, UI_16 data, SI_8 result);

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/

/********************************************************/
/*	�O���֐�											*/
/********************************************************/

/********************************************************/
/* �֐�����		: Eeprom_control_Init					*/
/* �@�\�̐���	: EEPROM���䏉����						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Eeprom_control_Init(void);

/********************************************************/
/* �֐�����		: Eeprom_control_Main					*/
/* �@�\�̐���	: EEPROM���䃁�C��						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Eeprom_control_Main(void);

/********************************************************/
/* �֐�����		: Eeprom_control_SetData				*/
/* �@�\�̐���	: EEPROM�փf�[�^�ݒ�					*/
/* ����			: (I/ )�f�[�^���  E_EEPROM_DATA_TYPE	*/
/* 				: (I/ )�ݒ�f�[�^  						*/
/*				: (I/ )�������݊����R�[���o�b�N�֐�		*/
/* �߂�l		: �������� E_EEPROM_CONTROL_RESULT		*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
E_EEPROM_CONTROL_RESULT Eeprom_control_SetData(E_EEPROM_DATA_TYPE type, UI_16 data, PFN_EEPROM_SETDATA_CB cbfnc);

/********************************************************/
/* �֐�����		: Eeprom_control_GetData				*/
/* �@�\�̐���	: EEPROM���䃁�C��						*/
/* ����			: (I/ )�f�[�^���  E_EEPROM_DATA_TYPE	*/
/* 				: ( /O)�擾�f�[�^  						*/
/* �߂�l		: �������� E_EEPROM_CONTROL_RESULT		*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
E_EEPROM_CONTROL_RESULT Eeprom_control_GetData(E_EEPROM_DATA_TYPE type, UI_16* pData);

#endif	/*DTO459_VDV_EEPROM_CTRL_H*/
