/*--------------------------------------------------------------------------*/
/*	LED�o�͐���																*/
/*--------------------------------------------------------------------------*/
/* �T�v																		*/
/* 				LED�̏o�͏�Ԃ����肷��										*/
/*																			*/
/*	�t�@�C���쐬���̃e�[�}:													*/
/*				ER330 �����A�_�v�^											*/
/*																			*/
/* �쐬			2010/09/24	�֓�											*/
/* �C��			2013/08/26	Y.Sorumachi										*/
/* 				�E�s�v�ȃR�[�h���폜										*/
/* 				�E�u�ݼ޹���̓_���p�^�[��1����уp�^�[��2�́A���̃p�^�[����	*/
/*					�̓_���J�n�ォ��5�����10%�̋P�x�Ɍ�������v�����ǉ�	*/
/* 				�EDUT���[�h�A�F�؃��[�h���폜								*/
/* 				�EBT�����Ԃ��擾����API�ύX								*/
/* 				�EBT�����Ԃ̐ڑ��������̂Ƃ��A�p�^�[��5�Ƃ��鏈���ǉ�		*/
/* �C��			2013/12/23	Y.Sorumachi										*/
/* 				�E�V�X�e����Ԑ���ύX�ɔ����C��							*/
/* 				�E���[�h���m���ԂŁABT�ݒ菈�������������ꍇ�A�p�^�[��4	*/
/*					�Ƃ��鏈���ǉ�											*/
/* 																			*/
/* ���l																		*/
/*				����														*/
/*																			*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "Timer.h"
#include "timer_ctrl.h"
#ifdef _VDV_APP
#include "DTO459_VDV_Bt_ctrl.h"
#include "DTO459_VDV_Gnss_ctrl.h"
#include "DTO459_VDV_Inspection_ctrl.h"
#endif /* _VDV_APP */
#include "DTO459_VDV_Sd_ctrl.h"
#include "System_control_TASK.h"							/* ���я�Ԑ��� */
#include "Output_control_led_CTRL.h"						/* �o�͏�Ԑ���(LED) */

/*--------------------------------------------------------------------------*/
/*	�����}�N��																*/
/*--------------------------------------------------------------------------*/
#define OUTLED_LOCAL	static

#define	LED_REDUCE_OFF					(0)					/* �����Ȃ� */
#define	LED_REDUCE_ON					(1)					/* �������� */
#define	LED_DUTY_MAX					(100)				/* �ő�duty��i�������x��) */
#define	LED_DUTY_MID					(15)				/* ����duty��i�������x��) */
#define	LED_DUTY_MIN					(10)				/* �ŏ�duty��i�������x��) */

#define	LED_BLINK						(3)					/* �_�ŏ�� */

/* LED�_���p�^�[�� */
typedef enum e_led_pattern
{
	LED_PTR_OFF = 0,			/* ���� */
	LED_PTR_ON,					/* �����1:�_�� */
	LED_PTR_FLASH_PAIRING,		/* �����2:�_��:1Hz 50%Duty */
	LED_PTR_FLASH_REC_NO_GPS,	/* �����3:�_��:1Hz 80%Duty */
	LED_PTR_FLASH_FNC_ERR,		/* �����4:�_��:2Hz 50% Duty */
	LED_PTR_FLASH_ATT_ERR,		/* �����5:�_��:1/2Hz 50% Duty */
	LED_PTR_FLASH_WRITING,		/* �����6:�_��:1/5Hz 50% Duty */
	LED_PTR_NUM
} E_LED_PATTERN;

/*--------------------------------------------------------------------------*/
/*	�\���̐錾																*/
/*--------------------------------------------------------------------------*/
typedef struct t_led_blink_pattern_tbl {
	UI_8	led_output_state;			/* LED�o�͏�ԁiLED_ON:�_���ALED_OFF:�����ALED_BLINK�A�_�Łj */
	UI_16	led_on_time;				/* LED ON���ԁi10ms�P�ʁj */
	UI_16	led_cycle_time;				/* LED�������ԁi10ms�P�ʁj */
	UI_8	led_reduce_state;			/* LED������ԁiLED_REDUCE_ON�Ȃ�5����ɋP�x����������B�j */
} T_LED_BLINK_PATTERN_TBL;

/*--------------------------------------------------------------------------*/
/*	�萔�錾																*/
/*--------------------------------------------------------------------------*/
OUTLED_LOCAL const T_LED_BLINK_PATTERN_TBL	C_led_blink_pattern_tbl[LED_PTR_NUM] = {				/* LED�����ð��� */
	{	LED_OFF		,	0								,	0				,	LED_REDUCE_OFF	},	/* ���� */
	{	LED_ON		,	0								,	0				,	LED_REDUCE_OFF	},	/* �����1:�_�� */
	{	LED_BLINK	,	TIME_MAIN_500MS					,	TIME_MAIN_1S	,	LED_REDUCE_OFF	},	/* �����2:�_��:1Hz 50%Duty */
	{	LED_BLINK	,	TIME_MAIN_800MS					,	TIME_MAIN_1S	,	LED_REDUCE_OFF	},	/* �����3:�_��:1Hz 80%Duty */
	{	LED_BLINK	,	TIME_MAIN_250MS					,	TIME_MAIN_500MS	,	LED_REDUCE_OFF	},	/* �����4:�_��:2Hz 50% Duty */
	{	LED_BLINK	,	TIME_MAIN_1S					,	TIME_MAIN_2S	,	LED_REDUCE_OFF	},	/* �����5:�_��:1/2Hz 50% Duty */
	{	LED_BLINK	,	TIME_MAIN_2500MS				,	TIME_MAIN_5S	,	LED_REDUCE_OFF	}	/* �����6:�_��:1/5Hz 50% Duty */
};

/*--------------------------------------------------------------------------*/
/*	�����ϐ���`															*/
/*--------------------------------------------------------------------------*/
OUTLED_LOCAL	T_LED_OUTPUT		Led_output_status[LED_TYPE_NUM];		/* LED�o�͏�� */
OUTLED_LOCAL	UI_16				Led_blink_cycle_count[LED_TYPE_NUM];	/* �_�Ŏ������� */
OUTLED_LOCAL	UI_8				Prev_pattern[LED_TYPE_NUM];				/* �O������� */
OUTLED_LOCAL	UI_16				Led_on_time[LED_TYPE_NUM];				/* LED ON���� */
OUTLED_LOCAL	UI_16				Led_max_time[LED_TYPE_NUM];				/* LED�_�Ŏ��� */


/*--------------------------------------------------------------------------*/
/*	�����֐���`															*/
/*--------------------------------------------------------------------------*/
OUTLED_LOCAL UI_8 output_control_getLedPattern(LED_TYPE ledType, E_SYS_CTRL_STATUS sysStat);
OUTLED_LOCAL UI_8 output_control_getLedLuminance(LED_TYPE ledType, E_SYS_CTRL_STATUS sysStat);
#ifdef _VDV_APP
OUTLED_LOCAL E_LED_PATTERN output_control_getLedPattern_inspection(LED_TYPE ledType);
OUTLED_LOCAL UI_8 output_control_getLedLuminance_inspcetion(LED_TYPE ledType);
#endif /* _VDV_APP */


/*--------------------------------------------------------------------------*/
/* RAM������																*/
/*--------------------------------------------------------------------------*/
/* �@�\�̐���:																*/
/* ����      :UI_8			mode		����������							*/
/* �߂�l    :																*/
/* ���l      :																*/
/*--------------------------------------------------------------------------*/
void Output_control_led_ram_init(void)
{
	UI_8 i;
	for(i = 0 ; i < LED_TYPE_NUM ; i++)
	{
		Prev_pattern[i] = LED_PTR_OFF;															
		Led_output_status[i].Power = C_led_blink_pattern_tbl[Prev_pattern[i]].led_output_state;	/* LED�o�͏��(�d��LED)��ð���[�O�������].led_output_state */
		Led_output_status[i].Duty = LED_DUTY_MAX;
		Led_on_time[i] = C_led_blink_pattern_tbl[Prev_pattern[i]].led_on_time;						/* LED ON���ԁ�ð���[�O�������].led_on_time */
		Led_max_time[i] = C_led_blink_pattern_tbl[Prev_pattern[i]].led_cycle_time;					/* LED�_�Ŏ�����ð���[�O�������].led_cycle_time */
	}
}


/*--------------------------------------------------------------------------*/
/* LED�o�͏�Ԕ���															*/
/*--------------------------------------------------------------------------*/
/* �@�\�̐���:LED�o�͂̏�Ԃ𔻒肷��										*/
/* ����      :�Ȃ�															*/
/* �߂�l    :�Ȃ�															*/
/* ���l      :																*/
/*--------------------------------------------------------------------------*/
void Output_control_led_main(void)
{
	E_SYS_CTRL_STATUS	sys_stat;										/* ������ */
	UI_8	now_pattern;												/* ��������� */
	UI_8 i;
	sys_stat = System_control_GetStatus();								/* �����Ԏ擾 */
	for(i = 0 ; i < LED_TYPE_NUM ; i++)
	{
		/* LED�p�^�[���擾 */
		now_pattern = output_control_getLedPattern((LED_TYPE)i, sys_stat);
		/* LED�P�x�擾 */
		Led_output_status[i].Duty = output_control_getLedLuminance((LED_TYPE)i, sys_stat);
		Led_output_status[i].Power = C_led_blink_pattern_tbl[now_pattern].led_output_state;	/* LED�o�͏��(�d��LED)��ð���[���������].led_output_state */
		
		if (now_pattern != Prev_pattern[i]) 
		{
			/* �O������݂ƈ�v���Ă��Ȃ� */
			Led_on_time[i] = C_led_blink_pattern_tbl[now_pattern].led_on_time;					/* LED ON���ԁ�ð���[���������].led_on_time */
			Led_max_time[i] = C_led_blink_pattern_tbl[now_pattern].led_cycle_time;				/* LED�_�Ŏ�����ð���[���������].led_cycle_time */
			
			Led_blink_cycle_count[i] = 0;										/* �����ر */
			Prev_pattern[i] = now_pattern;										/* �O������݁���������� */
			
			Led_output_status[i].Duty = LED_DUTY_MAX;
		}
			
		if (Led_output_status[i].Power == LED_BLINK) {					/* �_�ŁH */
			if (Led_blink_cycle_count[i] < Led_on_time[i]) {			/* LED ON���Ԗ��o�߁H */
				Led_output_status[i].Power = LED_ON;					/* ON */
			} else {														
				Led_output_status[i].Power = LED_OFF;					/* OFF */
			}
			Led_blink_cycle_count[i]++;									/* +10ms */
			if (Led_blink_cycle_count[i] >= Led_max_time[i]) {			/* LED�_�Ŏ����o��? */
				Led_blink_cycle_count[i] = 0;							/* �����ر */
			}
		}
	}
}


/********************************************************/
/* �֐�����		: Get_output_led_status					*/
/* �@�\�̐���	: LED��Ԏ擾							*/
/* ����			: (I/)ledType:LED���					*/
/* 				:	LED_TYPE_BODY_IND 	�{�̃C���W�P�[�^*/
/* 				:	LED_TYPE_SW_IND		SW�C���W�P�[�^	*/
/* 				:	LED_TYPE_SW_BKLT	SW�o�b�N���C�g	*/
/* �߂�l		: LED���								*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/12								*/
/* ���l			: 										*/
/********************************************************/
T_LED_OUTPUT Get_output_led_status(LED_TYPE ledType)
{
	if((ledType >= LED_TYPE_NUM) || (ledType < 0))
	{
		/* �����ُ펞�͖{�̃C���W�P�[�^��Ԃ� */
		return Led_output_status[LED_TYPE_BODY_IND];
	}
	return Led_output_status[ledType];
}

#ifdef _VDV_BOOT
/********************************************************/
/* �֐�����		: output_control_GetLedChangeTime		*/
/* �@�\�̐���	: LED��ԕω��܂ł̎��Ԏ擾�i�_�ł̂݁j	*/
/* ����			: (I/)ledType:LED���					*/
/* 				:	LED_TYPE_BODY_IND 	�{�̃C���W�P�[�^*/
/* 				:	LED_TYPE_SW_IND		SW�C���W�P�[�^	*/
/* 				:	LED_TYPE_SW_BKLT	SW�o�b�N���C�g	*/
/* �߂�l		: LED��ԕω��܂ł̎��ԁims�j			*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 17/09/11								*/
/* ���l			: �_���A�����̏ꍇ��0���Ԃ�				*/
/********************************************************/
UI_16 output_control_GetLedChangeTime(LED_TYPE ledType)
{
	UI_16 timer;
	UI_8 index;
	if((ledType >= LED_TYPE_NUM) || (ledType < 0))
	{
		/* �����ُ펞�͖{�̃C���W�P�[�^�Ƃ��� */
		index = LED_TYPE_BODY_IND;
	}
	else
	{
		/* �������펞�͎�ʂ̃C���f�b�N�X�w�� */
		index = (UI_8)ledType;
	}
	/*�c�莞�Ԃ�Ԃ�*/
	if (Led_blink_cycle_count[index] < Led_on_time[index]) {/* LED ON���Ԗ��o�߁H */
		/*ON���Ԗ��o�߁��_��*/
		timer = (UI_16)((Led_on_time[index] - Led_blink_cycle_count[index]) * TIME_MAIN);
	} else if (Led_blink_cycle_count[index] <= Led_max_time[index]) {
		/*ON���Ԍo�߁�����*/
		timer = (UI_16)((Led_max_time[index] - Led_blink_cycle_count[index]) * TIME_MAIN);
	} else {
		/*OFF���Ԍo��(�t�F�[��)*/
		timer = 0;
	}
	return timer;
}


/********************************************************/
/* �֐�����		: output_control_NotifyEndFlashErase	*/
/* �@�\�̐���	: FLASH�������Ԓʒm						*/
/* ����			: (I/)eraseTime:�����ɂ�����������		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 17/09/11								*/
/* ���l			: 										*/
/********************************************************/
void output_control_NotifyEndFlashErase(UI_16 eraseTime)
{
	UI_8 i;
	E_SYS_CTRL_STATUS	sys_stat;										/* ������ */
	UI_8		 now_pattern;											/* ��������� */
	T_LED_OUTPUT now_led_output_status;
	sys_stat = System_control_GetStatus();								/* �����Ԏ擾 */
	for(i = 0 ; i < LED_TYPE_NUM ; i++)
	{
		/* LED�p�^�[���擾 */
		now_pattern = output_control_getLedPattern((LED_TYPE)i, sys_stat);
		/* LED�P�x�擾 */
		now_led_output_status.Power = C_led_blink_pattern_tbl[now_pattern].led_output_state;	/* LED�o�͏��(�d��LED)��ð���[���������].led_output_state */
		/* �_�ŁH */
		if (now_led_output_status.Power == LED_BLINK) {
			/*��������*/
			Led_blink_cycle_count[i] += (UI_16)(eraseTime / TIME_MAIN);
		}
	}
}
#endif /* _VDV_BOOT */


/********************************************************************************/
/* �֐�����		: output_control_getLedPattern									*/
/* �@�\�̐���	: LED�p�^�[���擾												*/
/* ����			: (I/)ledType:LED���											*/
/* 				:	LED_TYPE_BODY_IND 	�{�̃C���W�P�[�^						*/
/* 				:	LED_TYPE_SW_IND		SW�C���W�P�[�^							*/
/* 				:	LED_TYPE_SW_BKLT	SW�o�b�N���C�g							*/
/* 				: (I/)sysStat:�V�X�e�����										*/
/* 				: SYS_OPERATION_WAIT_ACC_ON				ACC ON�҂�				*/
/* 				: SYS_OPERATION_ACC_ON_UNKNOWN_STAT		ACC ON�i��Ԕ���j		*/
/* 				: SYS_OPERATION_NORMAL_OFF				�ʏ�:OFF���[�h			*/
/* 				: SYS_OPERATION_NORMAL_AUTO				�ʏ�:�I�[�gREC			*/
/* 				: SYS_OPERATION_NORMAL_MANUAL			�ʏ�:�}�j���A��REC		*/
/* 				: SYS_OPERATION_ERROR					�ُ�					*/
/* 				: SYS_OPERATION_SIMULATION				�V�~�����[�V����		*/
/* 				: SYS_OPERATION_EEPROM_WRITE			EEPROM������			*/
/* 				: SYS_OPERATION_EEPROM_WRITE_COMP		EEPROM��������			*/
/* 				: SYS_OPERATION_FIRMWARE_WRITE			�t�@�[���E�F�A������	*/
/* 				: SYS_OPERATION_FIRMWARE_WRITE_COMP		�t�@�[���E�F�A��������	*/
/* 				: SYS_OPERATION_PRE_ACC_OFF				ACC OFF�ڍs				*/
/* 				: SYS_OPERATION_ACC_OFF					ACC OFF					*/
/* �߂�l		: UI_8:LED�p�^�[��												*/
/* 				: LED_PTR_OFF					����							*/
/* 				: LED_PTR_ON					�����1:�_��						*/
/* 				: LED_PTR_FLASH_PAIRING			�����2:�_��:1Hz 50%Duty			*/
/* 				: LED_PTR_FLASH_REC_NO_GPS		�����3:�_��:1Hz 80%Duty			*/
/* 				: LED_PTR_FLASH_FNC_ERR			�����4:�_��:2Hz 50% Duty		*/
/* 				: LED_PTR_FLASH_ATT_ERR			�����5:�_��:1/2Hz 50% Duty		*/
/* 				: LED_PTR_FLASH_WRITING			�����6:�_��:1/5Hz 50% Duty		*/
/* �쐬��		: (OFT)Sakata													*/
/* �쐬��		: 16/08/12														*/
/* ���l			: 																*/
/********************************************************************************/
OUTLED_LOCAL UI_8 output_control_getLedPattern(LED_TYPE ledType, E_SYS_CTRL_STATUS sysStat)
{
#ifdef _VDV_APP
	E_BT_CONTROL_STAT btStat;
	E_SD_CONTROL_REC_STAT sdStat;
#endif /* _VDV_APP */
	E_LED_PATTERN led_pattern = LED_PTR_OFF;
	if(ledType == LED_TYPE_BODY_IND)
	{
		/**< �{�̃C���W�P�[�^			*/
		switch (sysStat) {													
#ifdef _VDV_APP
			case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON�i��Ԕ���j	*/
			case (SYS_OPERATION_NORMAL_OFF):			/*�ʏ�:OFF���[�h		*/
			case (SYS_OPERATION_NORMAL_AUTO):			/*�ʏ�:�I�[�gREC		*/
			case (SYS_OPERATION_NORMAL_MANUAL):			/*�ʏ�:�}�j���A��REC	*/
			case (SYS_OPERATION_SIMULATION):			/*�V�~�����[�V����		*/
			case (SYS_OPERATION_EEPROM_WRITE_COMP):		/*EEPROM��������		*/
			case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF�ڍs			*/
				led_pattern = LED_PTR_ON;
				break;
			case (SYS_OPERATION_ERROR):					/*�ُ�					*/
				/*���W���[����Ԏ擾*/
				/* �ُ펞��OFF */
				led_pattern = LED_PTR_OFF;
				break;
			case (SYS_OPERATION_EEPROM_WRITE):			/*EEPROM������			*/
			case (SYS_OPERATION_GNSS_COMM):				/**< GNSS���W���[���ʐM���[�h*/
			case (SYS_OPERATION_BT_COMM):				/**< BT���W���[���ʐM���[�h	*/
				led_pattern = LED_PTR_FLASH_WRITING;
				break;
			case (SYS_OPERATION_WAIT_ACC_ON):			/*ACC ON�҂�			*/
			case (SYS_OPERATION_ACC_OFF):				/*ACC OFF				*/
				led_pattern = LED_PTR_OFF;
				break;
			case (SYS_OPERATION_INSPECTION):			/**< �������[�h				*/
			case (SYS_OPERATION_INSPECTION_COMP):		/**< ��������				*/
				led_pattern = output_control_getLedPattern_inspection(ledType);
				break;
#else /* _VDV_APP */
			case (SYS_OPERATION_FIRMWARE_WRITE):		/*�t�@�[���E�F�A������	*/
				led_pattern = LED_PTR_FLASH_WRITING;
				break;
			case (SYS_OPERATION_FIRMWARE_WRITE_COMP):	/*�t�@�[���E�F�A��������*/
				led_pattern = LED_PTR_ON;
				break;
#endif /* _VDV_APP */
			default:
				led_pattern = LED_PTR_OFF;
				break;
		}
	}
	else if(ledType == LED_TYPE_SW_IND)
	{
		/**< SW�C���W�P�[�^				*/
		switch (sysStat) {													
#ifdef _VDV_APP
			case (SYS_OPERATION_NORMAL_OFF):			/*�ʏ�:OFF���[�h		*/
				/*�y�A�����O�����ǂ�������*/
				btStat = Bt_control_GetStatus();
				if(btStat == BT_CONTROL_STAT_PAIRING)
				{
					/* �y�A�����O�� */
					led_pattern = LED_PTR_FLASH_PAIRING;
				}
				else
				{
					/* �y�A�����O���łȂ� */
					led_pattern = LED_PTR_OFF;
				}
				break;
			case (SYS_OPERATION_NORMAL_AUTO):			/*�ʏ�:�I�[�gREC		*/
			case (SYS_OPERATION_NORMAL_MANUAL):			/*�ʏ�:�}�j���A��REC	*/
				/*REC�����ǂ�������*/
				sdStat = Sd_control_GetRecStatus();
				if(sdStat == SD_CONTROL_REC_STAT_RECORDING)
				{
					/**< SD���W���[�����:REC��			*/
					if (Signal_control_isGnssEnable() == N_TRUE) 
					{
						/*GNSS����*/
						led_pattern = LED_PTR_ON;
					}
					else
					{
						/*GNSS������*/
						led_pattern = LED_PTR_FLASH_REC_NO_GPS;
					}
				}
				else
				{
					/**< SD���W���[�����:REC��~��		*/
					led_pattern = LED_PTR_OFF;
				}
				break;
			case (SYS_OPERATION_ERROR):					/*�ُ�					*/
				/* �ُ펞��OFF */
				led_pattern = LED_PTR_OFF;
				break;
			case (SYS_OPERATION_EEPROM_WRITE):			/*EEPROM������			*/
				led_pattern = LED_PTR_FLASH_WRITING;
				break;
			case (SYS_OPERATION_WAIT_ACC_ON):			/*ACC ON�҂�			*/
			case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON�i��Ԕ���j	*/
			case (SYS_OPERATION_SIMULATION):			/*�V�~�����[�V����		*/
			case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF�ڍs			*/
			case (SYS_OPERATION_ACC_OFF):				/*ACC OFF				*/
			case (SYS_OPERATION_EEPROM_WRITE_COMP):		/*EEPROM��������		*/
			case (SYS_OPERATION_GNSS_COMM):				/**< GNSS���W���[���ʐM���[�h*/
			case (SYS_OPERATION_BT_COMM):				/**< BT���W���[���ʐM���[�h	*/
				led_pattern = LED_PTR_OFF;
				break;
			case (SYS_OPERATION_INSPECTION):			/**< �������[�h				*/
			case (SYS_OPERATION_INSPECTION_COMP):		/**< ��������				*/
				led_pattern = output_control_getLedPattern_inspection(ledType);
				break;
#else /* _VDV_APP */
			case (SYS_OPERATION_FIRMWARE_WRITE):		/*�t�@�[���E�F�A������	*/
				led_pattern = LED_PTR_FLASH_WRITING;
				break;
			case (SYS_OPERATION_FIRMWARE_WRITE_COMP):	/*�t�@�[���E�F�A��������*/
				led_pattern = LED_PTR_OFF;
				break;
#endif /* _VDV_APP */
			default:
				led_pattern = LED_PTR_OFF;
				break;
		}
	}
	else if(ledType == LED_TYPE_SW_BKLT)
	{
		/**< SW�o�b�N���C�g				*/
		switch (sysStat) {													
#ifdef _VDV_APP
			case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON�i��Ԕ���j	*/
			case (SYS_OPERATION_NORMAL_AUTO):			/*�ʏ�:�I�[�gREC		*/
			case (SYS_OPERATION_NORMAL_MANUAL):			/*�ʏ�:�}�j���A��REC	*/
			case (SYS_OPERATION_SIMULATION):			/*�V�~�����[�V����		*/
			case (SYS_OPERATION_EEPROM_WRITE):			/*EEPROM������			*/
			case (SYS_OPERATION_EEPROM_WRITE_COMP):		/*EEPROM��������		*/
			case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF�ڍs			*/
			case (SYS_OPERATION_GNSS_COMM):				/**< GNSS���W���[���ʐM���[�h*/
			case (SYS_OPERATION_BT_COMM):				/**< BT���W���[���ʐM���[�h	*/
				led_pattern = LED_PTR_ON;
				break;
			case (SYS_OPERATION_NORMAL_OFF):			/*�ʏ�:OFF���[�h		*/
				/*�y�A�����O�����ǂ�������*/
				btStat = Bt_control_GetStatus();
				if(btStat == BT_CONTROL_STAT_PAIRING)
				{
					/* �y�A�����O�� */
					led_pattern = LED_PTR_FLASH_PAIRING;
				}
				else
				{
					/* �y�A�����O���łȂ� */
					led_pattern = LED_PTR_ON;
				}
				break;
			case (SYS_OPERATION_ERROR):					/*�ُ�					*/
				/* �ُ펞��OFF */
				led_pattern = LED_PTR_OFF;
				break;
			case (SYS_OPERATION_WAIT_ACC_ON):			/*ACC ON�҂�			*/
			case (SYS_OPERATION_ACC_OFF):				/*ACC OFF				*/
				led_pattern = LED_PTR_OFF;
				break;
			case (SYS_OPERATION_INSPECTION):			/**< �������[�h				*/
			case (SYS_OPERATION_INSPECTION_COMP):		/**< ��������				*/
				led_pattern = output_control_getLedPattern_inspection(ledType);
				break;
#else /* _VDV_APP */
			case (SYS_OPERATION_FIRMWARE_WRITE):		/*�t�@�[���E�F�A������	*/
			case (SYS_OPERATION_FIRMWARE_WRITE_COMP):	/*�t�@�[���E�F�A��������*/
				led_pattern = LED_PTR_ON;
				break;
#endif /* _VDV_APP */
			default:
				led_pattern = LED_PTR_OFF;
				break;
		}
	}
	else
	{
		/* nop */
	}
	return (UI_8)led_pattern;
}



/********************************************************************************/
/* �֐�����		: output_control_getLedLuminance								*/
/* �@�\�̐���	: LED�P�x�擾													*/
/* ����			: (I/)ledType:LED���											*/
/* 				:	LED_TYPE_BODY_IND 	�{�̃C���W�P�[�^						*/
/* 				:	LED_TYPE_SW_IND		SW�C���W�P�[�^							*/
/* 				:	LED_TYPE_SW_BKLT	SW�o�b�N���C�g							*/
/* 				: (I/)sysStat:�V�X�e�����										*/
/* 				: SYS_OPERATION_WAIT_ACC_ON				ACC ON�҂�				*/
/* 				: SYS_OPERATION_ACC_ON_UNKNOWN_STAT		ACC ON�i��Ԕ���j		*/
/* 				: SYS_OPERATION_NORMAL_OFF				�ʏ�:OFF���[�h			*/
/* 				: SYS_OPERATION_NORMAL_AUTO				�ʏ�:�I�[�gREC			*/
/* 				: SYS_OPERATION_NORMAL_MANUAL			�ʏ�:�}�j���A��REC		*/
/* 				: SYS_OPERATION_ERROR					�ُ�					*/
/* 				: SYS_OPERATION_SIMULATION				�V�~�����[�V����		*/
/* 				: SYS_OPERATION_EEPROM_WRITE			EEPROM������			*/
/* 				: SYS_OPERATION_EEPROM_WRITE_COMP		EEPROM��������			*/
/* 				: SYS_OPERATION_FIRMWARE_WRITE			�t�@�[���E�F�A������	*/
/* 				: SYS_OPERATION_FIRMWARE_WRITE_COMP		�t�@�[���E�F�A��������	*/
/* 				: SYS_OPERATION_PRE_ACC_OFF				ACC OFF�ڍs				*/
/* 				: SYS_OPERATION_ACC_OFF					ACC OFF					*/
/* �߂�l		: UI_8:LED�P�x													*/
/* 				: LED_DUTY_MAX	�ő�duty��i�������x��)							*/
/* 				: LED_DUTY_MID	����duty��i�������x��)							*/
/* 				: LED_DUTY_MIN	�ŏ�duty��i�������x��)							*/
/* �쐬��		: (OFT)Sakata													*/
/* �쐬��		: 17/01/20														*/
/* ���l			: 																*/
/********************************************************************************/
OUTLED_LOCAL UI_8 output_control_getLedLuminance(LED_TYPE ledType, E_SYS_CTRL_STATUS sysStat)
{
	UI_8 led_luminance = LED_DUTY_MAX; /* �f�t�H���g�ő�l */
	if(ledType == LED_TYPE_BODY_IND)
	{
		/**< �{�̃C���W�P�[�^			*/
#ifdef _VDV_APP
		if ((sysStat == SYS_OPERATION_INSPECTION) ||
			(sysStat == SYS_OPERATION_INSPECTION_COMP))
		{
			/* �������[�h�� */
			led_luminance = output_control_getLedLuminance_inspcetion(ledType);
		}
#else /* _VDV_APP */
#endif /* _VDV_APP */
	}
	else if(ledType == LED_TYPE_SW_IND)
	{
		/**< SW�C���W�P�[�^				*/
#ifdef _VDV_APP
		if ((sysStat == SYS_OPERATION_INSPECTION) ||
			(sysStat == SYS_OPERATION_INSPECTION_COMP))
		{
			/* �������[�h�� */
			led_luminance = output_control_getLedLuminance_inspcetion(ledType);
		}
#else /* _VDV_APP */
#endif /* _VDV_APP */
	}
	else if(ledType == LED_TYPE_SW_BKLT)
	{
		/**< SW�o�b�N���C�g				*/
#ifdef _VDV_APP
		if ((sysStat == SYS_OPERATION_INSPECTION) ||
			(sysStat == SYS_OPERATION_INSPECTION_COMP))
		{
			/* �������[�h�� */
			led_luminance = output_control_getLedLuminance_inspcetion(ledType);
		}
#else /* _VDV_APP */
#endif /* _VDV_APP */
	}
	else
	{
		/* nop */
	}
	return led_luminance;
}

#ifdef _VDV_APP
/********************************************************************************/
/* �֐�����		: output_control_getLedPattern_inspection						*/
/* �@�\�̐���	: LED�p�^�[���擾(�������[�h�p)									*/
/* ����			: (I/)ledType:LED���(LED_TYPE_xxx)								*/
/* �߂�l		: LED�p�^�[��(LED_PTR_xxx)										*/
/* �쐬��		: (OFT)Toguchi													*/
/* �쐬��		: 17/01/20														*/
/* ���l			: 																*/
/********************************************************************************/
OUTLED_LOCAL E_LED_PATTERN output_control_getLedPattern_inspection(LED_TYPE ledType)
{
	E_LED_PATTERN	led_pattern = LED_PTR_OFF;
	E_INSP_LED_MODE	insp_led_mode;

	/* ��������LED���[�h�擾	*/
	insp_led_mode = Insp_get_led_mode();

	if (ledType == LED_TYPE_BODY_IND) {				/* LED���:�{�̃C���W�P�[�^		*/
		if ((insp_led_mode == INSP_LED_MODE_NOMAL) ||			/* ����LED���[�h:�ʏ�_��		*/
			(insp_led_mode == INSP_LED_MODE_BT_PAIRING) ||		/* ����LED���[�h:Buletooth�y�A�����O		*/
			(insp_led_mode == INSP_LED_MODE_SW_INSPECT)) {		/* ����LED���[�h:SW����		*/
			/* �_��		*/
			led_pattern = LED_PTR_ON;
		}
	}
	else if (ledType == LED_TYPE_SW_IND) {			/* LED���:SW�C���W�P�[�^			*/
		if (insp_led_mode == INSP_LED_MODE_BT_PAIRING) {		/* ����LED���[�h:Buletooth�y�A�����O		*/
			/* �_�ŁiBuletooth�y�A�����O�p�j	*/
			led_pattern = LED_PTR_FLASH_PAIRING;
		}
		else if (insp_led_mode == INSP_LED_MODE_SW_INSPECT) {	/* ����LED���[�h:SW����					*/
			/* �_��		*/
			led_pattern = LED_PTR_ON;
		}
		else {
			/* nop */
		}
	}
	else if (ledType == LED_TYPE_SW_BKLT) {			/* LED���:SW�o�b�N���C�g			*/
		if ((insp_led_mode == INSP_LED_MODE_NOMAL) ||			/* ����LED���[�h:�ʏ�_��		*/
			(insp_led_mode == INSP_LED_MODE_SW_INSPECT)) {		/* ����LED���[�h:SW����		*/
			/* �_��		*/
			led_pattern = LED_PTR_ON;
		}
		else if (insp_led_mode == INSP_LED_MODE_BT_PAIRING) {	/* ����LED���[�h:Buletooth�y�A�����O		*/
			/* �_�ŁiBuletooth�y�A�����O�p�j	*/
			led_pattern = LED_PTR_FLASH_PAIRING;
		}
		else {
			/* nop */
		}
	}
	else {
		/* nop */
	}

	return led_pattern;
}
/********************************************************************************/
/* �֐�����		: output_control_getLedLuminance_inspcetion						*/
/* �@�\�̐���	: LED�P�x�擾(�������[�h�p)										*/
/* ����			: (I/)ledType:LED���(LED_TYPE_xxx)								*/
/* �߂�l		: UI_8:LED�P�x													*/
/* 				: LED_DUTY_MAX	�ő�duty��i�������x��)							*/
/* 				: LED_DUTY_MID	����duty��i�������x��)							*/
/* 				: LED_DUTY_MIN	�ŏ�duty��i�������x��)							*/
/* �쐬��		: (OFT)Toguchi													*/
/* �쐬��		: 17/01/20														*/
/* ���l			: 																*/
/********************************************************************************/
OUTLED_LOCAL UI_8 output_control_getLedLuminance_inspcetion(LED_TYPE ledType)
{
	UI_8			led_luminance = LED_DUTY_MAX;	/* �f�t�H���g�ő�l */
	E_INSP_LED_MODE	insp_led_mode;

	/* ��������LED���[�h�擾	*/
	insp_led_mode = Insp_get_led_mode();

	if (ledType == LED_TYPE_BODY_IND) {				/* LED���:�{�̃C���W�P�[�^		*/
		if ((insp_led_mode == INSP_LED_MODE_NOMAL) ||			/* ����LED���[�h:�ʏ�_��		*/
			(insp_led_mode == INSP_LED_MODE_BT_PAIRING) ||		/* ����LED���[�h:Buletooth�y�A�����O		*/
			(insp_led_mode == INSP_LED_MODE_SW_INSPECT)) {		/* ����LED���[�h:SW����		*/
			/* ����duty��		*/
			led_luminance = LED_DUTY_MID;
		}
	}
	else if (ledType == LED_TYPE_SW_IND) {			/* LED���:SW�C���W�P�[�^			*/
		if (insp_led_mode == INSP_LED_MODE_SW_INSPECT) {		/* ����LED���[�h:SW����					*/
			/* �ŏ�duty��		*/
			led_luminance = LED_DUTY_MIN;
		}
	}
	else if (ledType == LED_TYPE_SW_BKLT) {			/* LED���:SW�o�b�N���C�g			*/
		/* nop */
	}
	else {
		/* nop */
	}

	return led_luminance;
}
#endif /* _VDV_APP */

