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
/* SD����												*/
/********************************************************/
#ifndef	DTO459_VDV_SD_CTRL_H
#define	DTO459_VDV_SD_CTRL_H

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include "type.h"
#include "DTO459_VDV_Signal_ctrl.h"


/********************************************************/
/*	�}�N����`											*/
/********************************************************/
/* 
 ** �������
 */ 
typedef enum sd_process_type
{
	SD_PROCESS_MAIN	= 0,	/**< �ʏ폈��	10ms��		*/
	SD_PROCESS_SUB			/**< �T�u����	25ms��		*/
} SD_PROCESS_TYPE;

/* 
 ** REC���
 */ 
typedef enum e_sd_control_stat
{
	SD_CONTROL_REC_STAT_UNKNOWN = 0	,		/**< SD���W���[�����:���m��			*/
	SD_CONTROL_REC_STAT_RECORDING	,		/**< SD���W���[�����:REC��				*/
	SD_CONTROL_REC_STAT_UNRECORDING	,		/**< SD���W���[�����:REC��~��			*/

	SD_CONTROL_REC_STAT_NUM					/**< SD���W���[����Ԑ�					*/
} E_SD_CONTROL_REC_STAT;


/**
 **	��������
 */
typedef enum e_sd_control_result {
	SD_CONTROL_RESULT_OK = 0,				/**< ����I��	*/
	SD_CONTROL_RESULT_ERR_INVALID_ARG,		/**< �����s��	*/
	SD_CONTROL_RESULT_ERR_INVALID_STATE,	/**< ��ԕs��	*/
	SD_CONTROL_RESULT_ERR_BUSY,				/**< �r�V�[		*/
	SD_CONTROL_RESULT_ERR_FATAL,			/**< �ُ�I��	*/

	SD_CONTROL_RESULT_ERR_NUM				/**< ��Ԑ�		*/
} E_SD_CONTROL_RESULT;


/********************************************************/
/*	�\���̒�`											*/
/********************************************************/


/********************************************************/
/*	�O���֐�											*/
/********************************************************/

/********************************************************/
/* �֐�����		: Sd_control_Init						*/
/* �@�\�̐���	: SD���䏉����							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Sd_control_Init(void);

/********************************************************/
/* �֐�����		: Sd_control_Main						*/
/* �@�\�̐���	: SD���䃁�C��							*/
/* ����			: (I/ )processType �v���Z�X���			*/
/* 					SD_PROCESS_MAIN:�ʏ폈��	10ms��	*/
/* 					SD_PROCESS_SUB :�T�u����	25ms��	*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Sd_control_Main(SD_PROCESS_TYPE processType);


/************************************************************/
/* �֐�����		: Sd_control_GetRecStatus					*/
/* �@�\�̐���	: SD REC��Ԏ擾							*/
/* ����			: void										*/
/* �߂�l		: �������� E_SD_CONTROL_REC_STAT			*/
/* �쐬��		: (OFT)Sakata								*/
/* �쐬��		: 16/08/18									*/
/* ���l			: 											*/
/************************************************************/
E_SD_CONTROL_REC_STAT Sd_control_GetRecStatus(void);


/********************************************************/
/* �֐�����		: Sd_control_GetSignalList				*/
/* �@�\�̐���	: ���J�M��ID�ꗗ�擾					*/
/* ����			: (I/ )idtype			�M�����		*/
/*				  ( /O)signal_Num		�M����			*/
/*				  ( /O)signal_List		�M�����X�g		*/
/* �߂�l		: ����	:N_OK							*/
/*				  �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Sd_control_GetSignalList(E_SIGNAL_ID_TYPE idtype, UI_8 *signal_Num, const UI_16 **signal_List);

/********************************************************/
/* �֐�����		: Sd_control_GetSignalsInfo				*/
/* �@�\�̐���	: ���J�M���擾(�����M��)				*/
/* ����			: (I/ )req_id_count	�M��ID��			*/
/* 				: (I/ )req_ids 		�M��ID�i�����j		*/
/*				  ( /O)signal_values	�M���l�i�����j	*/
/* �߂�l		: ����	:N_OK							*/
/*				  �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Sd_control_GetSignalsInfo( UI_8 req_id_count, const UI_16* req_ids, SI_32 *signal_values);

/********************************************************/
/* �֐�����		: Sd_control_GetEncodeKey				*/
/* �@�\�̐���	: �Í����L�[�擾						*/
/* ����			: ( /O)key				�Í����p�L�[	*/
/* �߂�l		: ����	:N_OK							*/
/*				  �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Sd_control_GetEncodeKey(SI_32 *key);

/********************************************************/
/* �֐�����		: Sd_control_GetSignalVersion			*/
/* �@�\�̐���	: ���J�M���o�[�W����					*/
/* ����			: void									*/
/* �߂�l		: ���J�M���o�[�W����					*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
UI_32 Sd_control_GetSignalVersion(void);

/********************************************************/
/* �֐�����		: Sd_control_GetInspectionVariation		*/
/* �@�\�̐���	: �������[�h�̃o���G�[�V�������擾	*/
/* ����			: variation	�o���G�[�V�������			*/
/* �߂�l		: ����	:N_OK							*/
/*				  �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Sd_control_GetInspectionVariation(UI_8 *variation);

/****************************************************************************/
/* �֐�����		: Sd_control_ReqInspection									*/
/* �@�\�̐���	: SD�����v��												*/
/* ����			: serialno		�V���A��No									*/
/* �߂�l		: ����	:N_OK												*/
/*				  �ُ�	:N_NG												*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 17/01/20													*/
/* ���l			: �{�֐�������I�������ꍇ�A�������䃂�W���[���ɑ΂���		*/
/*				  Insp_notify_inspection_result�ɂ�茟�����ʂ�ʒm����		*/
/****************************************************************************/
SI_8 Sd_control_ReqInspection(UI_16 serialno);

#endif	/*DTO459_VDV_SD_CTRL_H*/
