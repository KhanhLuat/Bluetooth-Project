/********************************************************/
/* @file System_control_TASK.c							*/
/* �V�X�e����Ԑ���										*/
/* 														*/
/* �V�X�e�������Ԃ̐�����s���B						*/
/* <ul>													*/
/* <li>�x���g��ʒm�ɂ���āA�V�X�e�������ԁi���[�h�j�����肷��B */
/* <li>�V�X�e�������Ԃ��擾����B						*/
/* </ul>												*/
/* 														*/
/********************************************************/
/* @author      Y.Sorimachi								*/
/* @date        2013/--/--								*/
/********************************************************/ 

#include "system.h"
#include "Timer.h"	
#include "System_control_TASK.h"
#include "timer_ctrl.h"
#include "DTO459_VDV_Sw_ctrl.h"
#include "DTO459_VDV_Signal_ctrl.h"
#include "chatt_absorber.h"
#include "self_holding_ctrl.h"
#include "dbglog.h"

/********************************************************/
/*	�����}�N��											*/
/********************************************************/
#define SYS_LOCAL	static

#define SYS_END_PROC_BT			(0x01)										/**< �I����������(BT�ʐM����) 	*/
#define SYS_END_PROC_SD			(0x02)										/**< �I����������(SD����)		*/
#define SYS_END_PROC_EEPROM		(0x04)										/**< �I����������(SD����)		*/
#define SYS_END_PROC_FLASH		(0x08)										/**< �I����������(FLASH����)	*/
#ifdef _VDV_APP
#define SYS_END_PROC_ALL		(SYS_END_PROC_BT | SYS_END_PROC_SD | SYS_END_PROC_EEPROM)	/**< �S�I����������	 			*/
#else /* _VDV_APP */
#define SYS_END_PROC_ALL		(SYS_END_PROC_SD | SYS_END_PROC_FLASH)						/**< �S�I����������	 			*/
#endif /* _VDV_APP */
#define SYS_FORCE_ACC_OFF_TIME	(10000)														/**< ACCOFF�����ȍ~����			*/


/********************************************************/
/*	�����֐��v���g�^�C�v�錾							*/
/********************************************************/
/*
 *	�C�x���g�����֐�
 */
SYS_LOCAL void S_sys_event_acc_on( UI_8 param );					/**< SYS_EVENT_ACC_ON				*/
SYS_LOCAL void S_sys_event_acc_off( UI_8 param );					/**< SYS_EVENT_ACC_OFF				*/
SYS_LOCAL void S_sys_event_module_stat( UI_8 param );				/**< SYS_EVENT_MODULE_STAT			*/
SYS_LOCAL void S_sys_event_change_sw_mode( UI_8 param );			/**< SYS_EVENT_CHANGE_SW_MODE		*/
SYS_LOCAL void S_sys_event_decision_action_mode( UI_8 param );		/**< SYS_EVENT_DECISION_ACTION_MODE	*/
SYS_LOCAL void S_sys_event_end_proc_comp_bt( UI_8 param );			/**< SYS_EVENT_END_PROC_COMP_BT		*/
SYS_LOCAL void S_sys_event_end_proc_comp_sd( UI_8 param );			/**< SYS_EVENT_END_PROC_COMP_SD		*/
SYS_LOCAL void S_sys_event_end_proc_comp_eeprom( UI_8 param );		/**< SYS_EVENT_END_PROC_COMP_EEPROM	*/
SYS_LOCAL void S_sys_event_end_proc_comp_flash( UI_8 param );		/**< SYS_EVENT_END_PROC_COMP_FLASH	*/
SYS_LOCAL void S_sys_event_end_eeprom_write( UI_8 param );			/**< SYS_EVENT_END_EEPROM_WRITE		*/
SYS_LOCAL void S_sys_event_end_flash_write( UI_8 param );			/**< SYS_EVENT_END_FLASH_WRITE		*/
SYS_LOCAL void S_sys_event_end_inspection( UI_8 param );			/**< SYS_EVENT_END_INSPECTION		*/


/********************************************************/
/*	�萔�錾											*/
/********************************************************/
/*
 *	�C�x���g�����֐��e�[�u��
 */
SYS_LOCAL void ( * const C_sys_event_func[SYS_EVENT_NUM] ) ( UI_8 ) = {
	S_sys_event_acc_on,									/**< SYS_EVENT_ACC_ON					*/
	S_sys_event_acc_off,								/**< SYS_EVENT_ACC_OFF					*/
	S_sys_event_module_stat,							/**< SYS_EVENT_MODULE_STAT	(BT)		*/
	S_sys_event_change_sw_mode,							/**< SYS_EVENT_CHANGE_SW_MODE			*/
	S_sys_event_decision_action_mode,					/**< SYS_EVENT_DECISION_ACTION_MODE		*/
	S_sys_event_end_proc_comp_bt,						/**< SYS_EVENT_END_PROC_COMP_BT			*/
	S_sys_event_end_proc_comp_sd,						/**< SYS_EVENT_END_PROC_COMP_SD			*/
	S_sys_event_end_proc_comp_eeprom,					/**< SYS_EVENT_END_PROC_COMP_EEPROM		*/
	S_sys_event_end_proc_comp_flash,					/**< SYS_EVENT_END_PROC_COMP_FLASH		*/
	S_sys_event_end_eeprom_write,						/**< SYS_EVENT_END_EEPROM_WRITE			*/
	S_sys_event_end_flash_write,						/**< SYS_EVENT_END_FLASH_WRITE			*/
	S_sys_event_end_inspection,							/**< SYS_EVENT_END_INSPECTION		*/
	S_sys_event_module_stat,							/**< SYS_EVENT_MODULE_STAT_CAN			*/
	S_sys_event_module_stat,							/**< SYS_EVENT_MODULE_STAT_GNSS			*/
	S_sys_event_module_stat,							/**< SYS_EVENT_MODULE_STAT_SW			*/
};

/* 
 ** ���W���[�����
 */ 
typedef enum e_sys_module_kind
{
	SYS_MODULE_KIND_BT = 0,				/**< ���W���[����ԁFBT		*/
	SYS_MODULE_KIND_CAN,				/**< ���W���[����ԁFCAN	*/
	SYS_MODULE_KIND_GNSS,				/**< ���W���[����ԁFGNSS	*/
	SYS_MODULE_KIND_SW,					/**< ���W���[����ԁFSW		*/
	SYS_MODULE_KIND_NUM					/**< ���W���[����Ԑ�		*/
} E_SYS_MODULE_KIND;


typedef enum e_sys_action_mode
{
	SYS_ACTION_MODE_UNKNOWN = 0,			/**< ���샂�[�h�F���m��				*/
	SYS_ACTION_MODE_NORMAL,	    			/**< ���샂�[�h�F�ʏ�				*/
	SYS_ACTION_MODE_SIMULATION,				/**< ���샂�[�h�F�V�~�����[�V����	*/
	SYS_ACTION_MODE_EEPROM,	    			/**< ���샂�[�h�FEEPROM����			*/
	SYS_ACTION_MODE_FIRMWARE,				/**< ���샂�[�h�F�t�@�[���E�F�A����	*/
	SYS_ACTION_MODE_INSPECTION,				/**< ���샂�[�h�F�������[�h			*/
	SYS_ACTION_MODE_BT_COMM,				/**< ���샂�[�h�FBT���W���[���ʐM���[�h		*/
	SYS_ACTION_MODE_GNSS_COMM,				/**< ���샂�[�h�FGNSS���W���[���ʐM���[�h	*/
	SYS_ACTION_MODE_NUM						/**< ���샂�[�h��					*/
} E_SYS_ACTION_MODE;


/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
SYS_LOCAL	E_SYS_CTRL_STATUS	S_sys_operation_mode;							/**< �V�X�e�������� */
SYS_LOCAL	UI_8	S_sys_notify_event_state[SYS_EVENT_NUM];					/**< �C�x���g�ʒm��� */
SYS_LOCAL	E_SYS_CTRL_EVENT_PARAM	S_sys_notify_event_param[SYS_EVENT_NUM];	/**< �C�x���g�p�����^ */
SYS_LOCAL	UI_8	S_sys_notify_event_flg;										/**< �C�x���g�ʒm�t���O(N_ON�F�ʒm����AN_OFF�F�ʒm�Ȃ�) */
SYS_LOCAL	UI_8	S_sys_end_state;											/**< ������� */
SYS_LOCAL	UI_8	S_sys_module_state_list[SYS_MODULE_KIND_NUM];				/**< �e���W���[����� */
SYS_LOCAL	E_SYS_MODULE_STAT	S_sys_module_state;								/**< �S�̃��W���[����� */
SYS_LOCAL	E_SYS_ACTION_MODE	S_sys_action_mode;								/**< SD����擾�������샂�[�h */
SYS_LOCAL	UI_16	S_sys_accoff_time;											/**< ACCOFF�ڍs���� */
/********************************************************/
/*	�O���֐�											*/
/********************************************************/


/********************************************************/
/*	�O���ϐ�											*/
/********************************************************/
#ifdef _VDV_BOOT
UI_8	g_MainLoopFlag;
#endif /* _VDV_BOOT */

/** 
 * ������
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	�V�X�e����Ԑ���Ŏg�p����RAM�̏������������s���B
 */ 
void System_control_Init(void)
{
	UI_8 i;
	/**	<ol>	*/
	/**	<li>������	*/
	S_sys_operation_mode 	= SYS_OPERATION_WAIT_ACC_ON;
	S_sys_notify_event_flg 	= N_OFF;
	S_sys_end_state			= 0;
	S_sys_accoff_time = 0;
	S_sys_action_mode 		= SYS_ACTION_MODE_UNKNOWN;
	/*���W���[����ԏ�����*/
	for ( i = 0; i < SYS_MODULE_KIND_NUM; i++ )
	{
		S_sys_module_state_list[i] = SYS_MODULE_STAT_UNKNOWN;
	}
	S_sys_module_state = SYS_MODULE_STAT_UNKNOWN;
	for ( i = 0; i < SYS_EVENT_NUM; i++ )
	{
		S_sys_notify_event_state[i] = N_OFF;
		S_sys_notify_event_param[i] = SYS_EVENT_PARAM_NO;
	}
#ifdef _VDV_APP
	/*���J�M�����䏉����*/
	Signal_control_Init();
#endif /* _VDV_APP */
	/**	<li>�`���^�����O�z��	*/
	Chatt_absorber_Init();
	/**	<li>���ȕێ�����	*/
	Self_holding_ctrl_Init();
	/**	</ol>	*/
	return;
	
}

/** 
 * ���C��
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	�C�x���g�ʒm��Ԃɂ���āA�V�X�e�������ԁi���[�h�j�����肷��B
 */ 
void System_control_Main(void)
{
	UI_8 i;
	UI_8 param;
	E_SYS_CTRL_STATUS	pre_operation_mode;							/**< �V�X�e�������� */
	
	/**	<ol>	*/
	/**	<li>�C�x���g�ʒm�̗L���𔻒�	*/
	if ( S_sys_notify_event_flg == N_ON )
	{
		/**	<li>�ʒm���������C�x���g���ɂ���āA�V�X�e�������ԁi���[�h�j�����肷��B	*/
		for ( i = 0; i < SYS_EVENT_NUM; i++ )
		{
			if ( S_sys_notify_event_state[i] == N_ON )
			{
				pre_operation_mode = S_sys_operation_mode;
				param = S_sys_notify_event_param[i];
				C_sys_event_func[i](param);
				S_sys_notify_event_state[i] = N_OFF;
				S_sys_notify_event_param[i] = SYS_EVENT_PARAM_NO;
				if(pre_operation_mode != S_sys_operation_mode)
				{
					OUTLOG(OutputLog( "[SYS] STATUS %d->%d\n", pre_operation_mode, S_sys_operation_mode);)
				}
			}
		}
		/**	<li>�C�x���g�ʒm�t���O���N���A	*/
		S_sys_notify_event_flg = N_OFF;
	}
	else
	{
		
	}
	if(S_sys_operation_mode == SYS_OPERATION_PRE_ACC_OFF)
	{
		/* ACC OFF�ڍs�� */
		if (Timer_ctrl_TimeCompare10ms(S_sys_accoff_time, SYS_FORCE_ACC_OFF_TIME/TIME_MAIN) == N_OK)
		{
			/**	�^�C���A�E�g��ACC OFF�ɋ����ڍs������	*/
			S_sys_operation_mode = SYS_OPERATION_ACC_OFF;
		}
	}
	/**	<li>�`���^�����O�z��	*/
	Chatt_absorber_Main();
	/**	<li> ���ȕێ����� */
	Self_holding_ctrl_Main();
	/**	</ol>	*/
	
	
	return;
}

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
/* 				�F SYS_OPERATION_INSPECTION,			�������[�h				*/
/* 				�F SYS_OPERATION_INSPECTION_COMP,		��������				*/
/* 				�F SYS_OPERATION_BT_COMM,				BT���W���[���ʐM���[�h	*/
/* 				�F SYS_OPERATION_GNSS_COMM,				GNSS���W���[���ʐM���[�h*/
/* 				�F SYS_OPERATION_PRE_ACC_OFF,			ACC OFF�ڍs				*/
/* 				�F SYS_OPERATION_ACC_OFF,				ACC OFF					*/
/* �쐬��		�F (OFT)Sakata													*/
/* �쐬��		�F 16/08/12														*/
/* ���l			�F 																*/
/********************************************************************************/
E_SYS_CTRL_STATUS System_control_GetStatus(void)
{
	/**	<ol>	*/
	/**	</ol>	*/
	return S_sys_operation_mode;
}


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
E_SYS_MODULE_STAT System_control_GetModuleStatus(void)
{
	return S_sys_module_state;
}


/** 
 * �C�x���g�ʒm
 * 
 * @param	event		[in]	�ʒm����C�x���g
 * @param	param		[in]	�ʒm����p�����^
 *
 * @return	���s����
 * @retval	SYS_EVENT_NOTIFY_OK					����I��
 * @retval	SYS_EVENT_NOTIFY_ERR_INVALID_ARG	�����s��
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
UI_8 System_control_NotifyEvent(E_SYS_CTRL_EVENT event, E_SYS_CTRL_EVENT_PARAM param)
{
	/**	<ol>	*/
	/**	<li>�����`�F�b�N	*/
	if ( event >= SYS_EVENT_NUM )
	{
		return SYS_EVENT_NOTIFY_ERR_INVALID_ARG;
	}
	/* �����ɃC�x���g���������邱�Ƃ�����̂ŏ�Ԓʒm�C�x���g�����W���[�����ɕ�����*/
	/**< �C�x���g�F���W���[����� */
	if(	event == SYS_EVENT_MODULE_STAT )
	{
		/* �����Ӂ� SYS_EVENT_PARAM_MODSTAT_XXXXX�̒�`��ǉ������ꍇ�͂����ɒǋL���邱�ƁI*/
		/* �p�����^�FCAN��� */
		if ( (param == SYS_EVENT_PARAM_MODSTAT_FNC_OK_CAN) || (param == SYS_EVENT_PARAM_MODSTAT_FNC_NG_CAN) || (param == SYS_EVENT_PARAM_MODSTAT_ATT_NG_CAN) )
		{
			/* �C�x���g�FCAN��Ԃ�ݒ� */
			event = SYS_EVENT_MODULE_STAT_CAN;
		}
		else /* �p�����^�FGNSS��� */ if( (param == SYS_EVENT_PARAM_MODSTAT_FNC_NG_GNSS) || (param == SYS_EVENT_PARAM_MODSTAT_FNC_OK_GNSS) || (param == SYS_EVENT_PARAM_MODSTAT_ATT_NG_GNSS))
		{
			/* �C�x���g�FGNSS��Ԃ�ݒ� */
			event = SYS_EVENT_MODULE_STAT_GNSS;
		}
		else /* �p�����^�FSW��� */ if( (param == SYS_EVENT_PARAM_MODSTAT_FNC_OK_SW) || (param == SYS_EVENT_PARAM_MODSTAT_ATT_NG_SW) )
		{
			/* �C�x���g�FSW��Ԃ�ݒ� */
			event = SYS_EVENT_MODULE_STAT_SW;
		}
		else
		{
			/* �p�����^�FBT��� */
			/*�����Ȃ��F���̂܂܎��{*/
		}
	}
	
	/**	<li>�C�x���g������ʒm	*/
	if ( S_sys_notify_event_state[event] == N_OFF)
	{
		S_sys_notify_event_state[event] = N_ON;
		S_sys_notify_event_param[event] = param;
		S_sys_notify_event_flg = N_ON;
		OUTLOG(OutputLog( "[SYS] EV %d PARAM %d STATUS %d\n", event, param, S_sys_operation_mode);)
	}
	/**	</ol>	*/
	return SYS_EVENT_NOTIFY_OK;
	
}

/********************************************************/
/*	�����֐�											*/
/********************************************************/

/**
 * �C�x���g�����֐��FACC ON
 * 
 * @param	param		[in]	�ʒm����p�����^
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	
 */
SYS_LOCAL void S_sys_event_acc_on( UI_8 param )
{
	UI_8 i;
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_WAIT_ACC_ON):	/*ACC ON�҂�*/
	case (SYS_OPERATION_PRE_ACC_OFF):	/*ACC OFF�ڍs*/
		/*ACC ON�i��Ԕ���j	*/
		S_sys_operation_mode = SYS_OPERATION_ACC_ON_UNKNOWN_STAT;
		/*���샂�[�h������	*/
		S_sys_action_mode 	 = SYS_ACTION_MODE_UNKNOWN;
		/*���W���[����ԏ�����*/
		for ( i = 0; i < SYS_MODULE_KIND_NUM; i++ )
		{
			S_sys_module_state_list[i] = SYS_MODULE_STAT_UNKNOWN;
		}
		S_sys_module_state = SYS_MODULE_STAT_UNKNOWN;
		break;
	case (SYS_OPERATION_ACC_OFF):	/*ACC ON�҂�*/
		while(1)
		{
			/*---�t�F�[������---*/
			/*���ȕێ��|�[�g���A�N�e�B�u�ɂ��Ă����炩�̌����œd������������ACC ON�����o�����ꍇ�A
			  ACC ON���A�����邽�߂ɁA�������[�v�����ăE�H�b�`�h�b�O�ŗ����čċN��������		*/
		}
		break;
	default:
		break;
	}
	/**	</ol>	*/
	return;
}

/**
 * �C�x���g�����֐��FACC OFF
 * 
 * @param	param		[in]	�ʒm����p�����^
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	
 */
SYS_LOCAL void S_sys_event_acc_off( UI_8 param )
{
	/**	<ol>	*/
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_WAIT_ACC_ON):			/*ACC ON�҂�			*/
	case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF�ڍs			*/
	case (SYS_OPERATION_ACC_OFF):				/*ACC OFF				*/
		/* nop */
		break;

	case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON�i��Ԕ���j	*/
	case (SYS_OPERATION_NORMAL_OFF):			/*�ʏ�:OFF���[�h		*/
	case (SYS_OPERATION_NORMAL_AUTO):			/*�ʏ�:�I�[�gREC		*/
	case (SYS_OPERATION_NORMAL_MANUAL):			/*�ʏ�:�}�j���A��REC	*/
	case (SYS_OPERATION_ERROR):					/*�ُ�					*/
	case (SYS_OPERATION_SIMULATION):			/*�V�~�����[�V����		*/
	case (SYS_OPERATION_EEPROM_WRITE):			/*EEPROM������			*/
	case (SYS_OPERATION_EEPROM_WRITE_COMP):		/*EEPROM��������		*/
	case (SYS_OPERATION_FIRMWARE_WRITE):		/*�t�@�[���E�F�A������	*/
	case (SYS_OPERATION_FIRMWARE_WRITE_COMP):	/*�t�@�[���E�F�A��������*/
	case (SYS_OPERATION_INSPECTION):			/*�������[�h			*/
	case (SYS_OPERATION_INSPECTION_COMP):		/*��������				*/
	case (SYS_OPERATION_BT_COMM):				/*BT���W���[���ʐM���[�h	*/
	case (SYS_OPERATION_GNSS_COMM):				/*GNSS���W���[���ʐM���[�h	*/
	default:
		/*ACC OFF�ڍs			*/
		S_sys_end_state		 = 0;
		/* ACC OFF�ڍs���̎��Ԃ�ێ� */
		S_sys_accoff_time = Timer_ctrl_GetTime10ms();
		S_sys_operation_mode = SYS_OPERATION_PRE_ACC_OFF;
		break;
	}
	/**	</ol>	*/
	return;
}

/********************************************************/
/* �֐�����		�F S_sys_event_module_stat				*/
/* �@�\�̐���	�F �C�x���g�����֐��F���W���[�����		*/
/* ����			�F (I/)param�F�C�x���g�p�����^			*/
/* �߂�l		�F void									*/
/* �쐬��		�F (OFT)Sakata							*/
/* �쐬��		�F 16/08/12								*/
/* ���l			�F 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_module_stat( UI_8 param )
{
	/*���W���[����Ԏ擾		*/
	UI_8 i;
	UI_8 unknownCnt = 0;
	UI_8 okCnt = 0;
	UI_8 attNGCnt = 0;
	UI_8 fncNGCnt = 0;
	E_SYS_MODULE_STAT stat;
#ifdef _VDV_APP
	E_SW_MODE_STATUS swmode;
#endif /* _VDV_APP */
	/**	<ol>	*/
	switch( param )
	{
		/**< ��t�s�ǁF����X�C�b�`���O�t���C���W�P�[�^���j�b�g���ڑ�	*/
		case SYS_EVENT_PARAM_MODSTAT_ATT_NG_SW:
			S_sys_module_state_list[SYS_MODULE_KIND_SW] |= SYS_MODULE_STAT_ATT_NG;
			break;
		/**< ��t�s�ǁFGNSS�A���e�i���ڑ�								*/
		case SYS_EVENT_PARAM_MODSTAT_ATT_NG_GNSS:
			S_sys_module_state_list[SYS_MODULE_KIND_GNSS] |= SYS_MODULE_STAT_ATT_NG;
			break;
		/**< ��t�s�ǁF�ԗ�CAN�G���[									*/
		case SYS_EVENT_PARAM_MODSTAT_ATT_NG_CAN:
			S_sys_module_state_list[SYS_MODULE_KIND_CAN] |= SYS_MODULE_STAT_ATT_NG;
			break;
		/**< �����@�\�s�ǁF�ԗ�CAN�G���[�i�F�؃G���[�j					*/
		case SYS_EVENT_PARAM_MODSTAT_FNC_NG_CAN:
			S_sys_module_state_list[SYS_MODULE_KIND_CAN] |= SYS_MODULE_STAT_FNC_NG;
			break;
		/**< �����@�\�s�ǁF�}�C�R���`Bluetooth�ԁA�����ʐM�G���[		*/
		case SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT:
			S_sys_module_state_list[SYS_MODULE_KIND_BT] |= SYS_MODULE_STAT_FNC_NG;
			break;
		/**< �����@�\�s�ǁF�}�C�R���`GNSS�ԁA�����ʐM�G���[				*/
		case SYS_EVENT_PARAM_MODSTAT_FNC_NG_GNSS:
			S_sys_module_state_list[SYS_MODULE_KIND_GNSS] |= SYS_MODULE_STAT_FNC_NG;
			break;
		/**< �@�\����FBluetooth�@�\									*/
		case SYS_EVENT_PARAM_MODSTAT_FNC_OK_BT:
			S_sys_module_state_list[SYS_MODULE_KIND_BT] = SYS_MODULE_STAT_OK;
			break;
		/**< �@�\����FGNSS�@�\											*/
		case SYS_EVENT_PARAM_MODSTAT_FNC_OK_GNSS:
			S_sys_module_state_list[SYS_MODULE_KIND_GNSS] = SYS_MODULE_STAT_OK;
			break;
		/**< �@�\����FSW�@�\											*/
		case SYS_EVENT_PARAM_MODSTAT_FNC_OK_SW:
			S_sys_module_state_list[SYS_MODULE_KIND_SW] = SYS_MODULE_STAT_OK;
			break;
		/**< �@�\����FCAN�@�\											*/
		case SYS_EVENT_PARAM_MODSTAT_FNC_OK_CAN:
			S_sys_module_state_list[SYS_MODULE_KIND_CAN] = SYS_MODULE_STAT_OK;
			break;
		/**< ���̑�														*/
		default:
			break;
	}
#ifdef _VDV_APP
#endif /* _VDV_APP */
	/* ���W���[����Ԑݒ� */
	/**	</ol>	*/
	for ( i = 0; i < SYS_MODULE_KIND_NUM; i++ )
	{
		if((S_sys_module_state_list[i] & SYS_MODULE_STAT_UNKNOWN) != 0)
		{
			/**< ���W���[����ԁF���m��		*/
			unknownCnt++;
		}
		if((S_sys_module_state_list[i] & SYS_MODULE_STAT_OK) != 0)
		{
			/**< ���W���[����ԁF����		*/
			okCnt++;
		}
		if((S_sys_module_state_list[i] & SYS_MODULE_STAT_FNC_NG) != 0)
		{
			/**< ���W���[����ԁF�@�\�s��	*/
			fncNGCnt++;
		}
		if((S_sys_module_state_list[i] & SYS_MODULE_STAT_ATT_NG) != 0)
		{
			/**< ���W���[����ԁF��t�s��	*/
			attNGCnt++;
		}
	}
	/* �@�\�s�ǁA��t�s�Ǖs�ǂ����������̏ꍇ�͋@�\�s�ǂ�D��Ƃ��� */
	if(fncNGCnt > 0)
	{
		/**< ���W���[����ԁF�@�\�s��	*/
		stat = SYS_MODULE_STAT_FNC_NG;
	}
	else if(attNGCnt > 0)
	{
		/**< ���W���[����ԁF��t�s��	*/
		stat = SYS_MODULE_STAT_ATT_NG;
	}
	else if(unknownCnt > 0)
	{
		/**< ���W���[����ԁF���m��		*/
		stat = SYS_MODULE_STAT_UNKNOWN;
	}
	else
	{
		/**< ���W���[����ԁF����		*/
		stat = SYS_MODULE_STAT_OK;
	}
	/* ���W���[����ԍX�V */
	S_sys_module_state = stat;
	
	switch ( S_sys_operation_mode )
	{
		case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON�i��Ԕ���j	*/
			if((S_sys_module_state == SYS_MODULE_STAT_ATT_NG) || (S_sys_module_state == SYS_MODULE_STAT_FNC_NG))
			{
				/*��t�s�� or �@�\�s��*/
				switch( S_sys_action_mode )
				{
					/**< ���샂�[�h�F�ʏ�											*/
					case SYS_ACTION_MODE_NORMAL:
						S_sys_operation_mode = SYS_OPERATION_ERROR;
						break;
					/**< ���̑�														*/
					case SYS_ACTION_MODE_NUM:	/* QAC�΍�̂��ߒǉ�(switch���͏���3��(default�܂�)�ȏ�K�v) */
					default:
						break;
				}
			}
			else if(S_sys_module_state == SYS_MODULE_STAT_UNKNOWN)
			{
				/*���m��*/
			}
			else if(S_sys_module_state == SYS_MODULE_STAT_OK)
			{
				/*����*/
				switch( S_sys_action_mode )
				{
					/**< ���샂�[�h�F�ʏ�											*/
					case SYS_ACTION_MODE_NORMAL:
#ifdef _VDV_APP
						/*SW���[�h�擾*/
						swmode = Sw_control_GetSwMode();
						switch( swmode )
						{
							case SW_MODE_OFF:		/**< SW���[�h�FOFF				*/
								S_sys_operation_mode = SYS_OPERATION_NORMAL_OFF;
								break;
							case SW_MODE_AUTO:		/**< SW���[�h�F�I�[�gREC		*/
								S_sys_operation_mode = SYS_OPERATION_NORMAL_AUTO;
								break;
							case SW_MODE_MANUAL:	/**< SW���[�h�F�}�j���A��REC	*/
								S_sys_operation_mode = SYS_OPERATION_NORMAL_MANUAL;
								break;
							default:
								break;
						}
#endif /* _VDV_APP */
						break;
					/**< ���̑�														*/
					case SYS_ACTION_MODE_NUM:	/* QAC�΍�̂��ߒǉ�(switch���͏���3��(default�܂�)�ȏ�K�v) */
					default:
						break;
				}
			}
			else
			{
				/* nop */
			}
			break;
		case (SYS_OPERATION_NORMAL_OFF):			/*�ʏ�:OFF���[�h		*/
		case (SYS_OPERATION_NORMAL_AUTO):			/*�ʏ�:�I�[�gREC		*/
		case (SYS_OPERATION_NORMAL_MANUAL):			/*�ʏ�:�}�j���A��REC	*/
			if((S_sys_module_state == SYS_MODULE_STAT_ATT_NG) || (S_sys_module_state == SYS_MODULE_STAT_FNC_NG))
			{
				/*��t�s�� or �@�\�s��*/
				S_sys_operation_mode = SYS_OPERATION_ERROR;
			}
			break;
		case (SYS_OPERATION_ERROR):					/*�ُ�					*/
			/* �ُ��ԂɂȂ�����ACC OFF�����܂ŕ��A���Ȃ� */
			break;
		default:
			break;
	}
	
	/**	</ol>	*/
	return;
}

/********************************************************/
/* �֐�����		�F S_sys_event_change_sw_mode			*/
/* �@�\�̐���	�F �C�x���g�����֐��FSW���[�h�ؑ�		*/
/* ����			�F (I/)param�F�C�x���g�p�����^			*/
/* �߂�l		�F void									*/
/* �쐬��		�F (OFT)Sakata							*/
/* �쐬��		�F 16/08/12								*/
/* ���l			�F 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_change_sw_mode( UI_8 param )
{
	/**	<ol>	*/
	switch ( S_sys_operation_mode )
	{
		case (SYS_OPERATION_NORMAL_OFF):			/**< �ʏ�:OFF���[�h			*/
		case (SYS_OPERATION_NORMAL_AUTO):			/**< �ʏ�:�I�[�gREC			*/
		case (SYS_OPERATION_NORMAL_MANUAL):			/**< �ʏ�:�}�j���A��REC		*/
			switch( param )
			{
				/**< ���[�h�ؑցFOFF											*/
				case SYS_EVENT_PARAM_SW_MODE_OFF:
					S_sys_operation_mode = SYS_OPERATION_NORMAL_OFF;
					break;
				/**< ���[�h�ؑցF�I�[�gREC										*/
				case SYS_EVENT_PARAM_SW_MODE_AUTO:
					S_sys_operation_mode = SYS_OPERATION_NORMAL_AUTO;
					break;
				/**< ���[�h�ؑցF�}�j���A��REC									*/
				case SYS_EVENT_PARAM_SW_MODE_MANUAL:
					S_sys_operation_mode = SYS_OPERATION_NORMAL_MANUAL;
					break;
				/**< ���̑�														*/
				default:
					break;
			}
			break;
		case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON�i��Ԕ���j	*/
			if(S_sys_module_state == SYS_MODULE_STAT_OK)
			{
				/*����*/
				switch( S_sys_action_mode )
				{
					/**< ���샂�[�h�F�ʏ�											*/
					case SYS_ACTION_MODE_NORMAL:
						/*���SD����̒ʒm����M�ς�*/
						switch( param )
						{
							/**< ���[�h�ؑցFOFF											*/
							case SYS_EVENT_PARAM_SW_MODE_OFF:
								S_sys_operation_mode = SYS_OPERATION_NORMAL_OFF;
								break;
							/**< ���[�h�ؑցF�I�[�gREC										*/
							case SYS_EVENT_PARAM_SW_MODE_AUTO:
								S_sys_operation_mode = SYS_OPERATION_NORMAL_AUTO;
								break;
							/**< ���[�h�ؑցF�}�j���A��REC									*/
							case SYS_EVENT_PARAM_SW_MODE_MANUAL:
								S_sys_operation_mode = SYS_OPERATION_NORMAL_MANUAL;
								break;
							/**< ���̑�														*/
							default:
								break;
						}
						break;
					/**< ���̑�														*/
					case SYS_ACTION_MODE_NUM:	/* QAC�΍�̂��ߒǉ�(switch���͏���3��(default�܂�)�ȏ�K�v) */
					default:
						break;
				}
			}
			break;
		default:
			break;
	}
	/**	</ol>	*/
	return;
}

/********************************************************/
/* �֐�����		�F S_sys_event_decision_action_mode		*/
/* �@�\�̐���	�F �C�x���g�����֐��F���샂�[�h����		*/
/* ����			�F (I/)param�F�C�x���g�p�����^			*/
/* �߂�l		�F void									*/
/* �쐬��		�F (OFT)Sakata							*/
/* �쐬��		�F 16/08/12								*/
/* ���l			�F 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_decision_action_mode( UI_8 param )
{
	/**	<ol>	*/
#ifdef _VDV_APP
	UI_8 stat;
	E_SW_MODE_STATUS swmode;
#endif /* _VDV_APP */
	switch ( S_sys_operation_mode )
	{
		case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON�i��Ԕ���j		*/
			switch( param )
			{
				/**< ���샂�[�h�F�ʏ�											*/
				case SYS_EVENT_PARAM_ACT_MODE_NORMAL:
#ifdef _VDV_APP
					S_sys_action_mode 		= SYS_ACTION_MODE_NORMAL;
					/*���W���[����Ԏ擾		*/
					stat = System_control_GetModuleStatus();
					if(stat == SYS_MODULE_STAT_OK)
					{
						/*SW���[�h�擾*/
						swmode = Sw_control_GetSwMode();
						switch( swmode )
						{
							case SW_MODE_OFF:		/**< SW���[�h�FOFF				*/
								S_sys_operation_mode = SYS_OPERATION_NORMAL_OFF;
								break;
							case SW_MODE_AUTO:		/**< SW���[�h�F�I�[�gREC		*/
								S_sys_operation_mode = SYS_OPERATION_NORMAL_AUTO;
								break;
							case SW_MODE_MANUAL:	/**< SW���[�h�F�}�j���A��REC	*/
								S_sys_operation_mode = SYS_OPERATION_NORMAL_MANUAL;
								break;
							default:
								break;
						}
					}
					else if((stat == SYS_MODULE_STAT_ATT_NG) || (stat == SYS_MODULE_STAT_FNC_NG))
					{
						/*��t�s�� or �@�\�s��*/
						S_sys_operation_mode = SYS_OPERATION_ERROR;
					}
					else
					{
						/* ���̑���Ԃ̎��͏����Ȃ� */
					}
#else /* _VDV_APP */
					g_MainLoopFlag = 0;
#endif /* _VDV_APP */
					break;
				/**< ���샂�[�h�F�V�~�����[�V����								*/
				case SYS_EVENT_PARAM_ACT_MODE_SIMULATION:
#ifdef _VDV_APP
					S_sys_action_mode 		= SYS_ACTION_MODE_SIMULATION;
					S_sys_operation_mode 	= SYS_OPERATION_SIMULATION;
#else /* _VDV_APP */
					g_MainLoopFlag = 0;
#endif /* _VDV_APP */
					break;
				/**< ���샂�[�h�FEEPROM����										*/
				case SYS_EVENT_PARAM_ACT_MODE_EEPROM:
#ifdef _VDV_APP
					S_sys_action_mode 		= SYS_ACTION_MODE_EEPROM;
					S_sys_operation_mode 	= SYS_OPERATION_EEPROM_WRITE;
#else /* _VDV_APP */
					g_MainLoopFlag = 0;
#endif /* _VDV_APP */
					break;

				/**< ���샂�[�h�F�������[�h										*/
				case SYS_EVENT_PARAM_ACT_MODE_INSPECTION:
#ifdef _VDV_APP
					S_sys_action_mode 		= SYS_ACTION_MODE_INSPECTION;
					S_sys_operation_mode 	= SYS_OPERATION_INSPECTION;
#else /* _VDV_APP */
					g_MainLoopFlag = 0;
#endif /* _VDV_APP */
					break;

				/**< ���샂�[�h�FBT���W���[���ʐM���[�h							*/
				case SYS_EVENT_PARAM_ACT_MODE_BT_COMM:
#ifdef _VDV_APP
					S_sys_action_mode 		= SYS_ACTION_MODE_BT_COMM;
					S_sys_operation_mode 	= SYS_OPERATION_BT_COMM;
#else /* _VDV_APP */
					g_MainLoopFlag = 0;
#endif /* _VDV_APP */
					break;

				/**< ���샂�[�h�FGNSS���W���[���ʐM���[�h							*/
				case SYS_EVENT_PARAM_ACT_MODE_GNSS_COM:
#ifdef _VDV_APP
					S_sys_action_mode 		= SYS_ACTION_MODE_GNSS_COMM;
					S_sys_operation_mode 	= SYS_OPERATION_GNSS_COMM;
#else /* _VDV_APP */
					g_MainLoopFlag = 0;
#endif /* _VDV_APP */
					break;

#ifdef _VDV_BOOT
				/**< ���샂�[�h�F�t�@�[���E�F�A����								*/
				case SYS_EVENT_PARAM_ACT_MODE_FIRMWARE:
					S_sys_action_mode 		= SYS_ACTION_MODE_FIRMWARE;
					S_sys_operation_mode 	= SYS_OPERATION_FIRMWARE_WRITE;
					break;
#endif /* _VDV_BOOT */
				/**< ���̑�														*/
				default:
#ifdef _VDV_APP
					S_sys_action_mode 		= SYS_ACTION_MODE_UNKNOWN;
#else /* _VDV_APP */
					g_MainLoopFlag = 0;
#endif /* _VDV_APP */
					break;
			}
			break;
		case SYS_OPERATION_NUM:	/* QAC�΍�̂��ߒǉ�(switch���͏���3��(default�܂�)�ȏ�K�v) */
		default:
			break;
	}
	/**	</ol>	*/
	return;	
}
/********************************************************/
/* �֐�����		�F S_sys_event_end_proc_comp_bt			*/
/* �@�\�̐���	�F �C�x���g�����֐��F�I����������(BT)	*/
/* ����			�F (I/)param�F�C�x���g�p�����^			*/
/* �߂�l		�F void									*/
/* �쐬��		�F (OFT)Sakata							*/
/* �쐬��		�F 16/08/12								*/
/* ���l			�F 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_end_proc_comp_bt( UI_8 param )
{
#ifdef _VDV_APP
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF�ڍs			*/
		S_sys_end_state |= SYS_END_PROC_BT;
		/**	<li>�S�Ă̏I�����������������ꍇ�AACC OFF�ɑJ�ڂ���B	*/
		if ( S_sys_end_state == SYS_END_PROC_ALL )
		{
			S_sys_operation_mode = SYS_OPERATION_ACC_OFF;
		}
		break;
	case SYS_OPERATION_NUM:	/* QAC�΍�̂��ߒǉ�(switch���͏���3��(default�܂�)�ȏ�K�v) */
	default:
		break;
	}
	/**	</ol>	*/
	return;
#endif /* _VDV_APP */
}
/********************************************************/
/* �֐�����		�F S_sys_event_end_proc_comp_sd			*/
/* �@�\�̐���	�F �C�x���g�����֐��F�I����������(SD)	*/
/* ����			�F (I/)param�F�C�x���g�p�����^			*/
/* �߂�l		�F void									*/
/* �쐬��		�F (OFT)Sakata							*/
/* �쐬��		�F 16/08/12								*/
/* ���l			�F 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_end_proc_comp_sd( UI_8 param)
{
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF�ڍs			*/
		S_sys_end_state |= SYS_END_PROC_SD;
		/**	<li>�S�Ă̏I�����������������ꍇ�AACC OFF�ɑJ�ڂ���B	*/
		if ( S_sys_end_state == SYS_END_PROC_ALL )
		{
			S_sys_operation_mode = SYS_OPERATION_ACC_OFF;
		}
		break;
	case SYS_OPERATION_NUM:	/* QAC�΍�̂��ߒǉ�(switch���͏���3��(default�܂�)�ȏ�K�v) */
	default:
		break;
	}
	/**	</ol>	*/
	return;
}
/*********************************************************/
/* �֐�����		�F S_sys_event_end_proc_comp_eeprom		 */
/* �@�\�̐���	�F �C�x���g�����֐��F�I����������(EEPROM)*/
/* ����			�F (I/)param�F�C�x���g�p�����^			 */
/* �߂�l		�F void									 */
/* �쐬��		�F (OFT)Sakata							 */
/* �쐬��		�F 16/08/12								 */
/* ���l			�F 										 */
/*********************************************************/
SYS_LOCAL void S_sys_event_end_proc_comp_eeprom( UI_8 param )
{
#ifdef _VDV_APP
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF�ڍs			*/
		S_sys_end_state |= SYS_END_PROC_EEPROM;
		/**	<li>�S�Ă̏I�����������������ꍇ�AACC OFF�ɑJ�ڂ���B	*/
		if ( S_sys_end_state == SYS_END_PROC_ALL )
		{
			S_sys_operation_mode = SYS_OPERATION_ACC_OFF;
		}
		break;
	case SYS_OPERATION_NUM:	/* QAC�΍�̂��ߒǉ�(switch���͏���3��(default�܂�)�ȏ�K�v) */
	default:
		break;
	}
	return;
#endif /* _VDV_APP */
}


/*********************************************************/
/* �֐�����		�F S_sys_event_end_proc_comp_flash		 */
/* �@�\�̐���	�F �C�x���g�����֐��F�I����������(FLASH) */
/* ����			�F (I/)param�F�C�x���g�p�����^			 */
/* �߂�l		�F void									 */
/* �쐬��		�F (OFT)Sakata							 */
/* �쐬��		�F 16/08/12								 */
/* ���l			�F 										 */
/*********************************************************/
SYS_LOCAL void S_sys_event_end_proc_comp_flash( UI_8 param )
{
#ifdef _VDV_BOOT
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF�ڍs		*/
		S_sys_end_state |= SYS_END_PROC_FLASH;
		/**	<li>�S�Ă̏I�����������������ꍇ�AACC OFF�ɑJ�ڂ���B	*/
		if ( S_sys_end_state == SYS_END_PROC_ALL )
		{
			S_sys_operation_mode = SYS_OPERATION_ACC_OFF;
		}
		break;
	case SYS_OPERATION_NUM:	/* QAC�΍�̂��ߒǉ�(switch���͏���3��(default�܂�)�ȏ�K�v) */
	default:
		break;
	}
	return;
#endif /* _VDV_BOOT */
}

/********************************************************/
/* �֐�����		�F S_sys_event_end_eeprom_write			*/
/* �@�\�̐���	�F EEPROM������������					*/
/* ����			�F (I/)param�F�C�x���g�p�����^			*/
/* �߂�l		�F void									*/
/* �쐬��		�F (OFT)Sakata							*/
/* �쐬��		�F 16/08/12								*/
/* ���l			�F 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_end_eeprom_write( UI_8 param )
{
	/**	<ol>	*/
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_EEPROM_WRITE):			/*EEPROM������			*/
		/*EEPROM��������		*/
		S_sys_operation_mode = SYS_OPERATION_EEPROM_WRITE_COMP;
		break;
	case SYS_OPERATION_NUM:	/* QAC�΍�̂��ߒǉ�(switch���͏���3��(default�܂�)�ȏ�K�v) */
	default:
		break;
	}
	return;
}

/********************************************************/
/* �֐�����		�F S_sys_event_end_flash_write			*/
/* �@�\�̐���	�F FLASH������������					*/
/* ����			�F (I/)param�F�C�x���g�p�����^			*/
/* �߂�l		�F void									*/
/* �쐬��		�F (OFT)Sakata							*/
/* �쐬��		�F 16/08/12								*/
/* ���l			�F 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_end_flash_write( UI_8 param )
{
	/**	<ol>	*/
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_FIRMWARE_WRITE):		/*�t�@�[���E�F�A������	*/
		/*�t�@�[���E�F�A��������*/
		S_sys_operation_mode = SYS_OPERATION_FIRMWARE_WRITE_COMP;
		break;
	case SYS_OPERATION_NUM:	/* QAC�΍�̂��ߒǉ�(switch���͏���3��(default�܂�)�ȏ�K�v) */
	default:
		break;
	}
	return;
}

/********************************************************/
/* �֐�����		�F S_sys_event_end_inspection			*/
/* �@�\�̐���	�F ������������							*/
/* ����			�F (I/)param�F�C�x���g�p�����^			*/
/* �߂�l		�F void									*/
/* �쐬��		�F (OFT)Toguchi							*/
/* �쐬��		�F 17/01/20								*/
/* ���l			�F 										*/
/********************************************************/
SYS_LOCAL void S_sys_event_end_inspection( UI_8 param )
{
	/**	<ol>	*/
	switch ( S_sys_operation_mode )
	{
	case (SYS_OPERATION_INSPECTION):			/*�������[�h	*/
		/*����������*/
		S_sys_operation_mode = SYS_OPERATION_INSPECTION_COMP;
		break;
	case SYS_OPERATION_NUM:	/* QAC�΍�̂��ߒǉ�(switch���͏���3��(default�܂�)�ȏ�K�v) */
	default:
		break;
	}
	return;
}

