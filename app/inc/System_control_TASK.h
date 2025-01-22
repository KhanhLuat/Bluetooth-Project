/********************************************************/
/* @file System_control_TASK.h							*/
/* �V�X�e����Ԑ���										*/
/* 														*/
/* �V�X�e�������Ԃ̐�����s���B						*/
/* <ul>													*/
/* <li>�ʒm���ꂽ�C�x���g�ɂ���āA�V�X�e�������ԁi���[�h�j�����肷��B */
/* <li>�V�X�e�������Ԃ��擾����B						*/
/* </ul>												*/
/* 														*/
/********************************************************/
/* @author      Y.Sorimachi								*/
/* @date        2013/--/--								*/
/********************************************************/
/* @page */
#ifndef	SYSTEM_CONTROL_TASK_H
#define	SYSTEM_CONTROL_TASK_H

#include "type.h"

/*--------------------------------------------------------------------------*/
/*	�@�\�؂�ւ���`�i�f�o�b�O�p�\�[�X�ł̂ݗL���j							*/
/*--------------------------------------------------------------------------*/
#define GNSS_ENABLE					/* �L���ɂ�����GNSS�L�� */
#define BT_ENABLE					/* �L���ɂ�����BT�L�� */
#define CAN_ENABLE					/* �L���ɂ�����CAN�L�� */
//#define CAN_ERR_DETECT_ENABLE		/* �L���ɂ�����CAN�G���[���o���L�� */
//#define GNSS_ADR_ON				/* �L���ɂ�����ADR ON �����ɂ�����ADR OFF(ADF OFF�̏ꍇ�AHNR-PVT�܂���NAV-HNR�͎擾���Ȃ�)*/
#define GNSS_SIGNAL_OUTPUT_NAVPVT	/* �L���ɂ�����ADR ON/OFF�Ɋ֌W�Ȃ�GNSS�M����NAV-PVT���o�� 
									   �����ɂ������`GNSS_HNR_PVT_ENABLE�ɉ����āiHNR-PVT�܂���NAV-PVT�j���o��
									   �y���Ӂz
									    �{��`�𖳌��ɂ���ꍇ�AGNSS_ADR_ON��L���ɂ��Ȃ��ƐM�����擾�ł��Ȃ�
									    �܂��AGNSS_ADR_ON�̏ꍇ�͍X�V������2Hz�ɂȂ�*/
#define GNSS_HNR_PVT_ENABLE			/* �L���ɂ�����HNR-PVT���擾 �����ɂ�����NAV-HNR���擾 */
/*SMK Bluetooth*/
#define GNSS_VER_410				/* GNSS�t�@�[���E�F�A�o�[�W����4.10�̊�Ղ̂Ƃ��ɗL���ɂ���*/


/*--------------------------------------------------------------------------*/
/*	�O���}�N��																*/
/*--------------------------------------------------------------------------*/
/* 
 ** �V�X�e��������
 */ 
typedef enum e_sys_ctrl_status
{
	SYS_OPERATION_WAIT_ACC_ON = 0,		/**< ACC ON�҂�				*/
	SYS_OPERATION_ACC_ON_UNKNOWN_STAT,	/**< ACC ON�i��Ԕ���j		*/
	SYS_OPERATION_NORMAL_OFF,			/**< �ʏ�:OFF���[�h			*/
	SYS_OPERATION_NORMAL_AUTO,			/**< �ʏ�:�I�[�gREC			*/
	SYS_OPERATION_NORMAL_MANUAL,		/**< �ʏ�:�}�j���A��REC		*/
	SYS_OPERATION_ERROR,				/**< �ُ�					*/
	SYS_OPERATION_SIMULATION,			/**< �V�~�����[�V����		*/
	SYS_OPERATION_EEPROM_WRITE,			/**< EEPROM������			*/
	SYS_OPERATION_EEPROM_WRITE_COMP,	/**< EEPROM��������			*/
	SYS_OPERATION_FIRMWARE_WRITE,		/**< �t�@�[���E�F�A������	*/
	SYS_OPERATION_FIRMWARE_WRITE_COMP,	/**< �t�@�[���E�F�A��������	*/
	SYS_OPERATION_INSPECTION,			/**< �������[�h				*/
	SYS_OPERATION_INSPECTION_COMP,		/**< ��������				*/
	SYS_OPERATION_BT_COMM,				/**< BT���W���[���ʐM���[�h	*/
	SYS_OPERATION_GNSS_COMM,			/**< GNSS���W���[���ʐM���[�h*/
	SYS_OPERATION_PRE_ACC_OFF,			/**< ACC OFF�ڍs			*/
	SYS_OPERATION_ACC_OFF,				/**< ACC OFF				*/
	SYS_OPERATION_NUM					/**< �V�X�e�������Ԑ�		*/
} E_SYS_CTRL_STATUS;

/* 
 ** �C�x���g
 */ 
typedef enum e_sys_ctrl_event
{
	SYS_EVENT_ACC_ON		= 0,		/**< ACC ON														*/
	SYS_EVENT_ACC_OFF,					/**< ACC OFF													*/
	
	SYS_EVENT_MODULE_STAT,				/**< ���W���[�����(BT)											*/
	SYS_EVENT_CHANGE_SW_MODE,			/**< SW���[�h�ؑ�												*/
	SYS_EVENT_DECISION_ACTION_MODE,		/**< ���샂�[�h����												*/
	
	SYS_EVENT_END_PROC_COMP_BT,			/**< �I����������(BT�ʐM����)									*/
	SYS_EVENT_END_PROC_COMP_SD,			/**< �I����������(SD����)										*/
	SYS_EVENT_END_PROC_COMP_EEPROM,		/**< �I����������(EEPROM����)									*/
	SYS_EVENT_END_PROC_COMP_FLASH,		/**< �I����������(FLASH����M����)								*/
	
	SYS_EVENT_END_EEPROM_WRITE,			/**< EEPROM��������												*/
	SYS_EVENT_END_FLASH_WRITE,			/**< FLASH��������												*/
	SYS_EVENT_END_INSPECTION,			/**< ��������													*/

	SYS_EVENT_MODULE_STAT_CAN,			/**< ���W���[�����(CAN)										*/
	SYS_EVENT_MODULE_STAT_GNSS,			/**< ���W���[�����(GNSS)										*/
	SYS_EVENT_MODULE_STAT_SW,			/**< ���W���[�����(SW)											*/

	SYS_EVENT_NUM						/**< �C�x���g��						*/
} E_SYS_CTRL_EVENT;

/* 
 ** �C�x���g�p�����^
 */ 
typedef enum e_sys_ctrl_event_param
{
	SYS_EVENT_PARAM_NO		= 0,		/**< �p�����^�Ȃ�												*/
	SYS_EVENT_PARAM_MODSTAT_ATT_NG_SW,	/**< ��t�s�ǁF����X�C�b�`���O�t���C���W�P�[�^���j�b�g���ڑ�	*/
	SYS_EVENT_PARAM_MODSTAT_ATT_NG_GNSS,/**< ��t�s�ǁFGNSS�A���e�i���ڑ�								*/
	SYS_EVENT_PARAM_MODSTAT_ATT_NG_CAN,	/**< ��t�s�ǁF�ԗ�CAN�G���[									*/
	SYS_EVENT_PARAM_MODSTAT_FNC_NG_CAN,	/**< �����@�\�s�ǁF�ԗ�CAN�G���[�i�F�؁j						*/
	SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT,	/**< �����@�\�s�ǁF�}�C�R���`Bluetooth�ԁA�����ʐM�G���[		*/
	SYS_EVENT_PARAM_MODSTAT_FNC_NG_GNSS,/**< �����@�\�s�ǁF�}�C�R���`GNSS�ԁA�����ʐM�G���[				*/
	SYS_EVENT_PARAM_MODSTAT_FNC_OK_BT,	/**< �@�\����FBluetooth�@�\									*/
	SYS_EVENT_PARAM_MODSTAT_FNC_OK_GNSS,/**< �@�\����FGNSS�@�\											*/
	SYS_EVENT_PARAM_MODSTAT_FNC_OK_SW,	/**< �@�\����FSW�@�\											*/
	SYS_EVENT_PARAM_MODSTAT_FNC_OK_CAN,	/**< �@�\����FCAN�@�\											*/
	SYS_EVENT_PARAM_SW_MODE_OFF,		/**< SW���[�h�ؑցFOFF											*/
	SYS_EVENT_PARAM_SW_MODE_AUTO,		/**< SW���[�h�ؑցF�I�[�gREC									*/
	SYS_EVENT_PARAM_SW_MODE_MANUAL,		/**< SW���[�h�ؑցF�}�j���A��REC								*/
	SYS_EVENT_PARAM_ACT_MODE_NORMAL,	/**< ���샂�[�h�F�ʏ�											*/
	SYS_EVENT_PARAM_ACT_MODE_SIMULATION,/**< ���샂�[�h�F�V�~�����[�V����								*/
	SYS_EVENT_PARAM_ACT_MODE_EEPROM,	/**< ���샂�[�h�FEEPROM����										*/
	SYS_EVENT_PARAM_ACT_MODE_FIRMWARE,	/**< ���샂�[�h�F�t�@�[���E�F�A����								*/
	SYS_EVENT_PARAM_ACT_MODE_INSPECTION, /**< ���샂�[�h�F�������[�h									*/
	SYS_EVENT_PARAM_ACT_MODE_BT_COMM,	/**< ���샂�[�h�FBT���W���[���ʐM								*/
	SYS_EVENT_PARAM_ACT_MODE_GNSS_COM,	/**< ���샂�[�h�FGNSS���W���[���ʐM								*/
	SYS_EVENT_PARAM_NUM					/**< �p�����^��													*/
} E_SYS_CTRL_EVENT_PARAM;


/* 
 ** ���s����
 */ 
typedef enum e_sys_ctrl_event_notifi_result
{
	SYS_EVENT_NOTIFY_OK		= 0,		/**< ����I��			*/
	SYS_EVENT_NOTIFY_ERR_INVALID_ARG	/**< �����s��			*/
} E_SYS_CTRL_EVENT_NOTIFI_RESULT;


/* 
 ** ���W���[�����
 */ 
typedef enum e_sys_module_stat
{
	SYS_MODULE_STAT_UNKNOWN = 0x01,		/**< ���W���[����ԁF���m��		*/
	SYS_MODULE_STAT_OK		= 0x02,		/**< ���W���[����ԁF����		*/
	SYS_MODULE_STAT_FNC_NG	= 0x04,		/**< ���W���[����ԁF�@�\�s��	*/
	SYS_MODULE_STAT_ATT_NG	= 0x08,		/**< ���W���[����ԁF��t�s��	*/
} E_SYS_MODULE_STAT;


/*--------------------------------------------------------------------------*/
/*	�萔�錾																*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*	�\���̐錾																*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*	extern�錾																*/
/*--------------------------------------------------------------------------*/
#ifdef _VDV_BOOT
extern UI_8	g_MainLoopFlag;
#endif /* _VDV_BOOT */

/*--------------------------------------------------------------------------*/
/*	�v���g�^�C�v�錾														*/
/*--------------------------------------------------------------------------*/
/** 
 * ������
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	�V�X�e����Ԑ���Ŏg�p����RAM�̏������������s���B
 */ 
void System_control_Init(void);

/** 
 * ���C��
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	�C�x���g�ʒm��Ԃɂ���āA�V�X�e�������ԁi���[�h�j�����肷��B
 */ 
void System_control_Main(void);

/********************************************************************************/
/* �֐�����		�F System_control_GetStatus										*/
/* �@�\�̐���	�F �V�X�e�������Ԏ擾											*/
/* ����			�F void															*/
/* �߂�l		�F �V�X�e��������												*/
/* 				�F SYS_OPERATION_WAIT_ACC_ON,			ACC ON�҂�				*/
/* 				�F SYS_OPERATION_ACC_ON_UNKNOWN_STAT,	ACC ON�i��Ԕ���j		*/
/* 				�F SYS_OPERATION_NORMAL_OFF,			�ʏ�:OFF���[�h			*/
/* 				�F SYS_OPERATION_NORMAL_AUTO,			�ʏ�:�I�[�gREC			*/
/* 				�F SYS_OPERATION_NORMAL_MANUAL,			�ʏ�:�}�j���A��REC		*/
/* 				�F SYS_OPERATION_ERROR,					�ُ�					*/
/* 				�F SYS_OPERATION_SIMULATION,			�V�~�����[�V����		*/
/* 				�F SYS_OPERATION_EEPROM_WRITE,			EEPROM������			*/
/* 				�F SYS_OPERATION_EEPROM_WRITE_COMP,		EEPROM��������			*/
/* 				�F SYS_OPERATION_FIRMWARE_WRITE,		�t�@�[���E�F�A������	*/
/* 				�F SYS_OPERATION_FIRMWARE_WRITE_COMP,	�t�@�[���E�F�A��������	*/
/* 				�F SYS_OPERATION_PRE_ACC_OFF,			ACC OFF�ڍs				*/
/* 				�F SYS_OPERATION_ACC_OFF,				ACC OFF					*/
/* �쐬��		�F (OFT)Sakata													*/
/* �쐬��		�F 16/08/12														*/
/* ���l			�F 																*/
/********************************************************************************/
E_SYS_CTRL_STATUS System_control_GetStatus(void);

/** 
 * �C�x���g�ʒm�ɂ��V�X�e�������Ԍ���
 * 
 * @param	event		[in]	�ʒm����C�x���g
 * @param	param		[in]	�ʒm����p�����^
 *
 * @return	���s����
 * @retval	SYS_EVENT_NOTIFY_OK				����I��
 * @retval	SYS_EVENT_NOTIFY_ERR_INVALIDARG	�����s��
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	�C�x���g��ʒm����B
 */ 
/************************************************************/
/* �֐�����		�F System_control_NotifyEvent				*/
/* �@�\�̐���	�F �C�x���g�ʒm								*/
/* ����			�F (I/ )�C�x���g							*/
/* ����			�F (I/ )�C�x���g�p�����^					*/
/* �߂�l		�F ��t�s��		SYS_MODULE_STAT_ATT_NG		*/
/* 				�F �@�\�s��		SYS_MODULE_STAT_FNC_NG		*/
/* 				�F ���m��		SYS_MODULE_STAT_UNKNOWN		*/
/* 				�F ����			SYS_MODULE_STAT_OK			*/
/* �쐬��		�F (OFT)Sakata								*/
/* �쐬��		�F 16/08/12									*/
/* ���l			�F 											*/
/************************************************************/
UI_8 System_control_NotifyEvent(E_SYS_CTRL_EVENT event, E_SYS_CTRL_EVENT_PARAM param);

/********************************************************/
/* �֐�����		�F System_control_GetModuleStatus		*/
/* �@�\�̐���	�F ���W���[����Ԏ擾					*/
/* ����			�F void									*/
/* �߂�l		�F ��t�s��		SYS_MODULE_STAT_ATT_NG	*/
/* 				�F �@�\�s��		SYS_MODULE_STAT_FNC_NG	*/
/* 				�F ���m��		SYS_MODULE_STAT_UNKNOWN	*/
/* 				�F ����			SYS_MODULE_STAT_OK		*/
/* �쐬��		�F (OFT)Sakata							*/
/* �쐬��		�F 16/08/12								*/
/* ���l			�F 										*/
/********************************************************/
E_SYS_MODULE_STAT System_control_GetModuleStatus(void);

#endif	/* SYSTEM_CONTROL_TASK_H */
