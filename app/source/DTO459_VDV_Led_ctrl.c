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
/* file DTO459_VDV_LED_CTRL.c							*/
/* LED���W���[������									*/
/********************************************************/

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include "DTO459_VDV_Led_ctrl.h"
#include "System_control_TASK.h"
#include "Output_control_led_CTRL.h"	/* �o�͏�Ԑ���(LED) */
#include "SHRVSTD_PWM_Drv.h"

/********************************************************/
/*	�}�N����`											*/
/********************************************************/
#define LED_LOCAL	static

#define	LED_OFF_DUTY			(0)			/**< LED������duty��			 */
#define PWM_LED_FREQUENCY		(5000)		/**< PWM �ݒ����5ms/1��s=5000	*/

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
LED_LOCAL	T_LED_OUTPUT S_Led_output[LED_TYPE_NUM];	/* LED�o�͏�� */


/* LEC�̃`���l�� */
LED_LOCAL const UI_8	C_led_pwm_ch[LED_TYPE_NUM] = {
	PWM_DRV_BODY_IND_CH			,/* LED_TYPE_BODY_IND, */
	PWM_DRV_REC_IND_CH			,/* LED_TYPE_SW_IND, */
	PWM_DRV_SW_BKLIGHT_CH		,/* LED_TYPE_SW_BKLT, */
};

/********************************************************/
/*	�O���֐�											*/
/********************************************************/

/********************************************************/
/* �֐�����		: Led_control_Init						*/
/* �@�\�̐���	: LED���䏉����							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Led_control_Init(void)
{
	UI_8 i;

	Output_control_led_ram_init();
	for(i = 0 ; i < LED_TYPE_NUM ; i++)
	{
		S_Led_output[i].Power = LED_OFF;
		S_Led_output[i].Duty  = LED_OFF_DUTY;
	}
}

/********************************************************/
/* �֐�����		: Led_control_Main						*/
/* �@�\�̐���	: LED���䃁�C��							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Led_control_Main(void)
{
	T_LED_OUTPUT led_status; /* LED�̓_��/������� */
	UI_8 i;
	UI_16 duty;
	
	Output_control_led_main();
	
	/* LED��Duty�ݒ� */
	for(i = 0 ; i < LED_TYPE_NUM ; i++)
	{
		led_status = Get_output_led_status((LED_TYPE)i);			/* LED�̓_��/������Ԃ��擾 */
		
		/* �_�����ɍ������o����PWM�o�͂��� */
		if( (S_Led_output[i].Power != led_status.Power) || (S_Led_output[i].Duty  != led_status.Duty) )
		{
			if (led_status.Power == LED_ON) 
			{
				if( led_status.Duty > 0)
				{
					/* ���g���̔䗦�ɑ΂���̒l���v�Z */
					duty = (UI_16)((PWM_LED_FREQUENCY / 100 ) * led_status.Duty);
				}
				else
				{
					/* �_���i�������x�����O�̏ꍇ�j */
					duty = LED_OFF_DUTY;
				}
			}
			else
			{
				/* ���� */
				duty = LED_OFF_DUTY;
			}
			/* �l�̕ێ� */
			S_Led_output[i].Power = led_status.Power;
			S_Led_output[i].Duty  = led_status.Duty;
			
			/* PWM�֏o�� */
			(void)PWMDrv_SetDutyCycle( (E_PWM_DRV_PPG_CH)C_led_pwm_ch[i], PWM_LED_FREQUENCY, duty );
		}
	}
}
