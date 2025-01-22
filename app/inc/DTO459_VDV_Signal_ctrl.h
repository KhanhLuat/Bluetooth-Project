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
/* ���J�M������											*/
/********************************************************/
#ifndef	DTO459_VDV_SIGNAL_CTRL_H
#define	DTO459_VDV_SIGNAL_CTRL_H

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include "type.h"

/********************************************************/
/*	�}�N����`											*/
/********************************************************/

/* 
 ** �f�o�C�X���
 */ 
typedef enum e_signal_device_type
{
	DEVICE_TYPE_BT = 0,				/**< �f�o�C�X���:BT	*/
	DEVICE_TYPE_SD,					/**< �f�o�C�X���:SD	*/
	DEVICE_TYPE_CAN,				/**< �f�o�C�X���:CAN	*/
	DEVICE_TYPE_NUM					/**< �f�o�C�X��ʐ�		*/
} E_SIGNAL_DEVICE_TYPE;

/* 
 ** �M��ID���
 */ 
typedef enum e_signal_id_type
{
	ID_TYPE_FIXED_ID = 0,			/**< �M�����:�Œ�l	*/
	ID_TYPE_VARIABLE_ID				/**< �M�����:�ϐ��l	*/
} E_SIGNAL_ID_TYPE;

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/

/********************************************************/
/*	�O���֐�											*/
/********************************************************/

/********************************************************/
/* �֐�����		: Signal_control_Init					*/
/* �@�\�̐���	: ���J�M�����䏉����					*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Signal_control_Init(void);


/********************************************************/
/* �֐�����		: Signal_control_GetSignalVersion		*/
/* �@�\�̐���	: ���J�M���o�[�W����					*/
/* ����			: void									*/
/* �߂�l		: ���J�M���o�[�W����					*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
UI_32 Signal_control_GetSignalVersion(void);


/********************************************************/
/* �֐�����		: Signal_control_GetRandomValue			*/
/* �@�\�̐���	: �����擾								*/
/* ����			: void									*/
/* �߂�l		: ����									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_32 Signal_control_GetRandomValue(void);


/********************************************************/
/* �֐�����		: Signal_control_GetEncodeKey			*/
/* �@�\�̐���	: �Í����L�[�擾						*/
/* ����			: (I/ )deviceType 		�f�o�C�X���	*/
/*				: ( /O)key				�Í����p�L�[	*/
/* �߂�l		: ����	:N_OK							*/
/*				: �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Signal_control_GetEncodeKey(E_SIGNAL_DEVICE_TYPE deviceType, SI_32 *key);


/********************************************************/
/* �֐�����		: Signal_control_SetEncodeKey			*/
/* �@�\�̐���	: �Í����L�[�ݒ�						*/
/* ����			: (I/ )deviceType 		�f�o�C�X���	*/
/*				: (I/ )key				�Í����p�L�[	*/
/* �߂�l		: ����	:N_OK							*/
/*				: �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Signal_control_SetEncodeKey(E_SIGNAL_DEVICE_TYPE deviceType, SI_32 key);

/********************************************************/
/* �֐�����		: Signal_control_GetSignalInfo			*/
/* �@�\�̐���	: ���J�M���擾							*/
/* ����			: (I/ )deviceType 		�f�o�C�X���	*/
/* 				: (I/ )req_id 			�M��ID			*/
/*				: ( /O)signal_value	�M���l				*/
/* �߂�l		: ����	:N_OK							*/
/*				: �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Signal_control_GetSignalInfo(E_SIGNAL_DEVICE_TYPE deviceType, UI_16 req_id, SI_32 *signal_value);

/********************************************************/
/* �֐�����		: Signal_control_GetSignalsInfo			*/
/* �@�\�̐���	: ���J�M���擾(�����M��)				*/
/* ����			: (I/ )deviceType 		�f�o�C�X���	*/
/* 				: (I/ )req_id_count	�M��ID��			*/
/* 				: (I/ )req_ids 		�M��ID�i�����j		*/
/*				: ( /O)signal_values	�M���l�i�����j	*/
/* �߂�l		: ����	:N_OK							*/
/*				: �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Signal_control_GetSignalsInfo(E_SIGNAL_DEVICE_TYPE deviceType, UI_8 req_id_count, const UI_16* req_ids, SI_32 *signal_values);


/********************************************************/
/* �֐�����		: Signal_control_GetSignalList			*/
/* �@�\�̐���	: ���J�M��ID�ꗗ�擾					*/
/* ����			: (I/ )deviceType 		�f�o�C�X���	*/
/*				: (I/ )idtype			�M�����		*/
/*				: ( /O)signal_Num		�M����			*/
/*				: ( /O)signal_List		�M�����X�g		*/
/* �߂�l		: ����	:N_OK							*/
/*				: �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Signal_control_GetSignalList(E_SIGNAL_DEVICE_TYPE deviceType, E_SIGNAL_ID_TYPE idtype, UI_8 *signal_Num, const UI_16 **signal_List);


/********************************************************/
/* �֐�����		: Signal_control_isGnssEnable			*/
/* �@�\�̐���	: GNSS�M����M�ϔ���					*/
/* ����			: void									*/
/* �߂�l		: TRUE:��M��	 FALSE:����M			*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
B_8 Signal_control_isGnssEnable(void);

/********************************************************/
/* �֐�����		: Signal_control_isCanEnable			*/
/* �@�\�̐���	: CAN�M����M�ϔ���						*/
/* ����			: void									*/
/* �߂�l		: TRUE:��M��	 FALSE:����M			*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
B_8 Signal_control_isCanEnable(void);


#endif	/*DTO459_VDV_SIGNAL_CTRL_H*/
