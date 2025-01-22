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
/* LEDモジュール制御									*/
/********************************************************/

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include "DTO459_VDV_Led_ctrl.h"
#include "System_control_TASK.h"
#include "Output_control_led_CTRL.h"	/* 出力状態制御(LED) */
#include "SHRVSTD_PWM_Drv.h"

/********************************************************/
/*	マクロ定義											*/
/********************************************************/
#define LED_LOCAL	static

#define	LED_OFF_DUTY			(0)			/**< LED消灯時duty比			 */
#define PWM_LED_FREQUENCY		(5000)		/**< PWM 設定周期5ms/1μs=5000	*/

/********************************************************/
/*	内部変数定義										*/
/********************************************************/
LED_LOCAL	T_LED_OUTPUT S_Led_output[LED_TYPE_NUM];	/* LED出力情報 */


/* LECのチャネル */
LED_LOCAL const UI_8	C_led_pwm_ch[LED_TYPE_NUM] = {
	PWM_DRV_BODY_IND_CH			,/* LED_TYPE_BODY_IND, */
	PWM_DRV_REC_IND_CH			,/* LED_TYPE_SW_IND, */
	PWM_DRV_SW_BKLIGHT_CH		,/* LED_TYPE_SW_BKLT, */
};

/********************************************************/
/*	外部関数											*/
/********************************************************/

/********************************************************/
/* 関数名称		: Led_control_Init						*/
/* 機能の説明	: LED制御初期化							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
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
/* 関数名称		: Led_control_Main						*/
/* 機能の説明	: LED制御メイン							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Led_control_Main(void)
{
	T_LED_OUTPUT led_status; /* LEDの点灯/消灯状態 */
	UI_8 i;
	UI_16 duty;
	
	Output_control_led_main();
	
	/* LEDのDuty設定 */
	for(i = 0 ; i < LED_TYPE_NUM ; i++)
	{
		led_status = Get_output_led_status((LED_TYPE)i);			/* LEDの点灯/消灯状態を取得 */
		
		/* 点灯情報に差分が出たらPWM出力する */
		if( (S_Led_output[i].Power != led_status.Power) || (S_Led_output[i].Duty  != led_status.Duty) )
		{
			if (led_status.Power == LED_ON) 
			{
				if( led_status.Duty > 0)
				{
					/* 周波数の比率に対するの値を計算 */
					duty = (UI_16)((PWM_LED_FREQUENCY / 100 ) * led_status.Duty);
				}
				else
				{
					/* 点灯（調光レベルが０の場合） */
					duty = LED_OFF_DUTY;
				}
			}
			else
			{
				/* 消灯 */
				duty = LED_OFF_DUTY;
			}
			/* 値の保持 */
			S_Led_output[i].Power = led_status.Power;
			S_Led_output[i].Duty  = led_status.Duty;
			
			/* PWMへ出力 */
			(void)PWMDrv_SetDutyCycle( (E_PWM_DRV_PPG_CH)C_led_pwm_ch[i], PWM_LED_FREQUENCY, duty );
		}
	}
}
