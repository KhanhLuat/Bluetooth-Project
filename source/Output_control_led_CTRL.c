/*--------------------------------------------------------------------------*/
/*	LED出力制御																*/
/*--------------------------------------------------------------------------*/
/* 概要																		*/
/* 				LEDの出力状態を決定する										*/
/*																			*/
/*	ファイル作成時のテーマ:													*/
/*				ER330 無線アダプタ											*/
/*																			*/
/* 作成			2010/09/24	斎藤											*/
/* 修正			2013/08/26	Y.Sorumachi										*/
/* 				・不要なコードを削除										*/
/* 				・「ｲﾝｼﾞｹｰﾀの点灯パターン1およびパターン2は、そのパターンで	*/
/*					の点灯開始後から5分後に10%の輝度に減光する」処理追加	*/
/* 				・DUTモード、認証モードを削除								*/
/* 				・BT動作状態を取得するAPI変更								*/
/* 				・BT動作状態の接続処理中のとき、パターン5とする処理追加		*/
/* 修正			2013/12/23	Y.Sorumachi										*/
/* 				・システム状態制御変更に伴う修正							*/
/* 				・モード未確定状態で、BT設定処理が完了した場合、パターン4	*/
/*					とする処理追加											*/
/* 																			*/
/* 備考																		*/
/*				無し														*/
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
#include "System_control_TASK.h"							/* ｼｽﾃﾑ状態制御 */
#include "Output_control_led_CTRL.h"						/* 出力状態制御(LED) */

/*--------------------------------------------------------------------------*/
/*	内部マクロ																*/
/*--------------------------------------------------------------------------*/
#define OUTLED_LOCAL	static

#define	LED_REDUCE_OFF					(0)					/* 減光なし */
#define	LED_REDUCE_ON					(1)					/* 減光あり */
#define	LED_DUTY_MAX					(100)				/* 最大duty比（調光レベル) */
#define	LED_DUTY_MID					(15)				/* 中間duty比（調光レベル) */
#define	LED_DUTY_MIN					(10)				/* 最小duty比（調光レベル) */

#define	LED_BLINK						(3)					/* 点滅状態 */

/* LED点灯パターン */
typedef enum e_led_pattern
{
	LED_PTR_OFF = 0,			/* 消灯 */
	LED_PTR_ON,					/* ﾊﾟﾀｰﾝ1:点灯 */
	LED_PTR_FLASH_PAIRING,		/* ﾊﾟﾀｰﾝ2:点滅:1Hz 50%Duty */
	LED_PTR_FLASH_REC_NO_GPS,	/* ﾊﾟﾀｰﾝ3:点滅:1Hz 80%Duty */
	LED_PTR_FLASH_FNC_ERR,		/* ﾊﾟﾀｰﾝ4:点滅:2Hz 50% Duty */
	LED_PTR_FLASH_ATT_ERR,		/* ﾊﾟﾀｰﾝ5:点滅:1/2Hz 50% Duty */
	LED_PTR_FLASH_WRITING,		/* ﾊﾟﾀｰﾝ6:点滅:1/5Hz 50% Duty */
	LED_PTR_NUM
} E_LED_PATTERN;

/*--------------------------------------------------------------------------*/
/*	構造体宣言																*/
/*--------------------------------------------------------------------------*/
typedef struct t_led_blink_pattern_tbl {
	UI_8	led_output_state;			/* LED出力状態（LED_ON:点灯、LED_OFF:消灯、LED_BLINK、点滅） */
	UI_16	led_on_time;				/* LED ON時間（10ms単位） */
	UI_16	led_cycle_time;				/* LED周期時間（10ms単位） */
	UI_8	led_reduce_state;			/* LED減光状態（LED_REDUCE_ONなら5分後に輝度を減光する。） */
} T_LED_BLINK_PATTERN_TBL;

/*--------------------------------------------------------------------------*/
/*	定数宣言																*/
/*--------------------------------------------------------------------------*/
OUTLED_LOCAL const T_LED_BLINK_PATTERN_TBL	C_led_blink_pattern_tbl[LED_PTR_NUM] = {				/* LEDﾊﾟﾀｰﾝﾃｰﾌﾞﾙ */
	{	LED_OFF		,	0								,	0				,	LED_REDUCE_OFF	},	/* 消灯 */
	{	LED_ON		,	0								,	0				,	LED_REDUCE_OFF	},	/* ﾊﾟﾀｰﾝ1:点灯 */
	{	LED_BLINK	,	TIME_MAIN_500MS					,	TIME_MAIN_1S	,	LED_REDUCE_OFF	},	/* ﾊﾟﾀｰﾝ2:点滅:1Hz 50%Duty */
	{	LED_BLINK	,	TIME_MAIN_800MS					,	TIME_MAIN_1S	,	LED_REDUCE_OFF	},	/* ﾊﾟﾀｰﾝ3:点滅:1Hz 80%Duty */
	{	LED_BLINK	,	TIME_MAIN_250MS					,	TIME_MAIN_500MS	,	LED_REDUCE_OFF	},	/* ﾊﾟﾀｰﾝ4:点滅:2Hz 50% Duty */
	{	LED_BLINK	,	TIME_MAIN_1S					,	TIME_MAIN_2S	,	LED_REDUCE_OFF	},	/* ﾊﾟﾀｰﾝ5:点滅:1/2Hz 50% Duty */
	{	LED_BLINK	,	TIME_MAIN_2500MS				,	TIME_MAIN_5S	,	LED_REDUCE_OFF	}	/* ﾊﾟﾀｰﾝ6:点滅:1/5Hz 50% Duty */
};

/*--------------------------------------------------------------------------*/
/*	内部変数定義															*/
/*--------------------------------------------------------------------------*/
OUTLED_LOCAL	T_LED_OUTPUT		Led_output_status[LED_TYPE_NUM];		/* LED出力状態 */
OUTLED_LOCAL	UI_16				Led_blink_cycle_count[LED_TYPE_NUM];	/* 点滅周期ｶｳﾝﾀ */
OUTLED_LOCAL	UI_8				Prev_pattern[LED_TYPE_NUM];				/* 前回ﾊﾟﾀｰﾝ */
OUTLED_LOCAL	UI_16				Led_on_time[LED_TYPE_NUM];				/* LED ON時間 */
OUTLED_LOCAL	UI_16				Led_max_time[LED_TYPE_NUM];				/* LED点滅周期 */


/*--------------------------------------------------------------------------*/
/*	内部関数定義															*/
/*--------------------------------------------------------------------------*/
OUTLED_LOCAL UI_8 output_control_getLedPattern(LED_TYPE ledType, E_SYS_CTRL_STATUS sysStat);
OUTLED_LOCAL UI_8 output_control_getLedLuminance(LED_TYPE ledType, E_SYS_CTRL_STATUS sysStat);
#ifdef _VDV_APP
OUTLED_LOCAL E_LED_PATTERN output_control_getLedPattern_inspection(LED_TYPE ledType);
OUTLED_LOCAL UI_8 output_control_getLedLuminance_inspcetion(LED_TYPE ledType);
#endif /* _VDV_APP */


/*--------------------------------------------------------------------------*/
/* RAM初期化																*/
/*--------------------------------------------------------------------------*/
/* 機能の説明:																*/
/* 引数      :UI_8			mode		初期化条件							*/
/* 戻り値    :																*/
/* 備考      :																*/
/*--------------------------------------------------------------------------*/
void Output_control_led_ram_init(void)
{
	UI_8 i;
	for(i = 0 ; i < LED_TYPE_NUM ; i++)
	{
		Prev_pattern[i] = LED_PTR_OFF;															
		Led_output_status[i].Power = C_led_blink_pattern_tbl[Prev_pattern[i]].led_output_state;	/* LED出力状態(電源LED)←ﾃｰﾌﾞﾙ[前回ﾊﾟﾀｰﾝ].led_output_state */
		Led_output_status[i].Duty = LED_DUTY_MAX;
		Led_on_time[i] = C_led_blink_pattern_tbl[Prev_pattern[i]].led_on_time;						/* LED ON時間←ﾃｰﾌﾞﾙ[前回ﾊﾟﾀｰﾝ].led_on_time */
		Led_max_time[i] = C_led_blink_pattern_tbl[Prev_pattern[i]].led_cycle_time;					/* LED点滅周期←ﾃｰﾌﾞﾙ[前回ﾊﾟﾀｰﾝ].led_cycle_time */
	}
}


/*--------------------------------------------------------------------------*/
/* LED出力状態判定															*/
/*--------------------------------------------------------------------------*/
/* 機能の説明:LED出力の状態を判定する										*/
/* 引数      :なし															*/
/* 戻り値    :なし															*/
/* 備考      :																*/
/*--------------------------------------------------------------------------*/
void Output_control_led_main(void)
{
	E_SYS_CTRL_STATUS	sys_stat;										/* 動作状態 */
	UI_8	now_pattern;												/* 今回ﾊﾟﾀｰﾝ */
	UI_8 i;
	sys_stat = System_control_GetStatus();								/* 動作状態取得 */
	for(i = 0 ; i < LED_TYPE_NUM ; i++)
	{
		/* LEDパターン取得 */
		now_pattern = output_control_getLedPattern((LED_TYPE)i, sys_stat);
		/* LED輝度取得 */
		Led_output_status[i].Duty = output_control_getLedLuminance((LED_TYPE)i, sys_stat);
		Led_output_status[i].Power = C_led_blink_pattern_tbl[now_pattern].led_output_state;	/* LED出力状態(電源LED)←ﾃｰﾌﾞﾙ[今回ﾊﾟﾀｰﾝ].led_output_state */
		
		if (now_pattern != Prev_pattern[i]) 
		{
			/* 前回ﾊﾟﾀｰﾝと一致していない */
			Led_on_time[i] = C_led_blink_pattern_tbl[now_pattern].led_on_time;					/* LED ON時間←ﾃｰﾌﾞﾙ[今回ﾊﾟﾀｰﾝ].led_on_time */
			Led_max_time[i] = C_led_blink_pattern_tbl[now_pattern].led_cycle_time;				/* LED点滅周期←ﾃｰﾌﾞﾙ[今回ﾊﾟﾀｰﾝ].led_cycle_time */
			
			Led_blink_cycle_count[i] = 0;										/* ｶｳﾝﾀｸﾘｱ */
			Prev_pattern[i] = now_pattern;										/* 前回ﾊﾟﾀｰﾝ←今回ﾊﾟﾀｰﾝ */
			
			Led_output_status[i].Duty = LED_DUTY_MAX;
		}
			
		if (Led_output_status[i].Power == LED_BLINK) {					/* 点滅？ */
			if (Led_blink_cycle_count[i] < Led_on_time[i]) {			/* LED ON時間未経過？ */
				Led_output_status[i].Power = LED_ON;					/* ON */
			} else {														
				Led_output_status[i].Power = LED_OFF;					/* OFF */
			}
			Led_blink_cycle_count[i]++;									/* +10ms */
			if (Led_blink_cycle_count[i] >= Led_max_time[i]) {			/* LED点滅周期経過? */
				Led_blink_cycle_count[i] = 0;							/* ｶｳﾝﾀｸﾘｱ */
			}
		}
	}
}


/********************************************************/
/* 関数名称		: Get_output_led_status					*/
/* 機能の説明	: LED状態取得							*/
/* 引数			: (I/)ledType:LED種別					*/
/* 				:	LED_TYPE_BODY_IND 	本体インジケータ*/
/* 				:	LED_TYPE_SW_IND		SWインジケータ	*/
/* 				:	LED_TYPE_SW_BKLT	SWバックライト	*/
/* 戻り値		: LED状態								*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/12								*/
/* 備考			: 										*/
/********************************************************/
T_LED_OUTPUT Get_output_led_status(LED_TYPE ledType)
{
	if((ledType >= LED_TYPE_NUM) || (ledType < 0))
	{
		/* 引数異常時は本体インジケータを返す */
		return Led_output_status[LED_TYPE_BODY_IND];
	}
	return Led_output_status[ledType];
}

#ifdef _VDV_BOOT
/********************************************************/
/* 関数名称		: output_control_GetLedChangeTime		*/
/* 機能の説明	: LED状態変化までの時間取得（点滅のみ）	*/
/* 引数			: (I/)ledType:LED種別					*/
/* 				:	LED_TYPE_BODY_IND 	本体インジケータ*/
/* 				:	LED_TYPE_SW_IND		SWインジケータ	*/
/* 				:	LED_TYPE_SW_BKLT	SWバックライト	*/
/* 戻り値		: LED状態変化までの時間（ms）			*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 17/09/11								*/
/* 備考			: 点灯、消灯の場合は0が返る				*/
/********************************************************/
UI_16 output_control_GetLedChangeTime(LED_TYPE ledType)
{
	UI_16 timer;
	UI_8 index;
	if((ledType >= LED_TYPE_NUM) || (ledType < 0))
	{
		/* 引数異常時は本体インジケータとする */
		index = LED_TYPE_BODY_IND;
	}
	else
	{
		/* 引数正常時は種別のインデックス指定 */
		index = (UI_8)ledType;
	}
	/*残り時間を返す*/
	if (Led_blink_cycle_count[index] < Led_on_time[index]) {/* LED ON時間未経過？ */
		/*ON時間未経過＝点灯*/
		timer = (UI_16)((Led_on_time[index] - Led_blink_cycle_count[index]) * TIME_MAIN);
	} else if (Led_blink_cycle_count[index] <= Led_max_time[index]) {
		/*ON時間経過＝消灯*/
		timer = (UI_16)((Led_max_time[index] - Led_blink_cycle_count[index]) * TIME_MAIN);
	} else {
		/*OFF時間経過(フェール)*/
		timer = 0;
	}
	return timer;
}


/********************************************************/
/* 関数名称		: output_control_NotifyEndFlashErase	*/
/* 機能の説明	: FLASH消去時間通知						*/
/* 引数			: (I/)eraseTime:消去にかかった時間		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 17/09/11								*/
/* 備考			: 										*/
/********************************************************/
void output_control_NotifyEndFlashErase(UI_16 eraseTime)
{
	UI_8 i;
	E_SYS_CTRL_STATUS	sys_stat;										/* 動作状態 */
	UI_8		 now_pattern;											/* 今回ﾊﾟﾀｰﾝ */
	T_LED_OUTPUT now_led_output_status;
	sys_stat = System_control_GetStatus();								/* 動作状態取得 */
	for(i = 0 ; i < LED_TYPE_NUM ; i++)
	{
		/* LEDパターン取得 */
		now_pattern = output_control_getLedPattern((LED_TYPE)i, sys_stat);
		/* LED輝度取得 */
		now_led_output_status.Power = C_led_blink_pattern_tbl[now_pattern].led_output_state;	/* LED出力状態(電源LED)←ﾃｰﾌﾞﾙ[今回ﾊﾟﾀｰﾝ].led_output_state */
		/* 点滅？ */
		if (now_led_output_status.Power == LED_BLINK) {
			/*消去時間*/
			Led_blink_cycle_count[i] += (UI_16)(eraseTime / TIME_MAIN);
		}
	}
}
#endif /* _VDV_BOOT */


/********************************************************************************/
/* 関数名称		: output_control_getLedPattern									*/
/* 機能の説明	: LEDパターン取得												*/
/* 引数			: (I/)ledType:LED種別											*/
/* 				:	LED_TYPE_BODY_IND 	本体インジケータ						*/
/* 				:	LED_TYPE_SW_IND		SWインジケータ							*/
/* 				:	LED_TYPE_SW_BKLT	SWバックライト							*/
/* 				: (I/)sysStat:システム状態										*/
/* 				: SYS_OPERATION_WAIT_ACC_ON				ACC ON待ち				*/
/* 				: SYS_OPERATION_ACC_ON_UNKNOWN_STAT		ACC ON（状態判定）		*/
/* 				: SYS_OPERATION_NORMAL_OFF				通常:OFFモード			*/
/* 				: SYS_OPERATION_NORMAL_AUTO				通常:オートREC			*/
/* 				: SYS_OPERATION_NORMAL_MANUAL			通常:マニュアルREC		*/
/* 				: SYS_OPERATION_ERROR					異常					*/
/* 				: SYS_OPERATION_SIMULATION				シミュレーション		*/
/* 				: SYS_OPERATION_EEPROM_WRITE			EEPROM書換中			*/
/* 				: SYS_OPERATION_EEPROM_WRITE_COMP		EEPROM書換完了			*/
/* 				: SYS_OPERATION_FIRMWARE_WRITE			ファームウェア書換中	*/
/* 				: SYS_OPERATION_FIRMWARE_WRITE_COMP		ファームウェア書換完了	*/
/* 				: SYS_OPERATION_PRE_ACC_OFF				ACC OFF移行				*/
/* 				: SYS_OPERATION_ACC_OFF					ACC OFF					*/
/* 戻り値		: UI_8:LEDパターン												*/
/* 				: LED_PTR_OFF					消灯							*/
/* 				: LED_PTR_ON					ﾊﾟﾀｰﾝ1:点灯						*/
/* 				: LED_PTR_FLASH_PAIRING			ﾊﾟﾀｰﾝ2:点滅:1Hz 50%Duty			*/
/* 				: LED_PTR_FLASH_REC_NO_GPS		ﾊﾟﾀｰﾝ3:点滅:1Hz 80%Duty			*/
/* 				: LED_PTR_FLASH_FNC_ERR			ﾊﾟﾀｰﾝ4:点滅:2Hz 50% Duty		*/
/* 				: LED_PTR_FLASH_ATT_ERR			ﾊﾟﾀｰﾝ5:点滅:1/2Hz 50% Duty		*/
/* 				: LED_PTR_FLASH_WRITING			ﾊﾟﾀｰﾝ6:点滅:1/5Hz 50% Duty		*/
/* 作成者		: (OFT)Sakata													*/
/* 作成日		: 16/08/12														*/
/* 備考			: 																*/
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
		/**< 本体インジケータ			*/
		switch (sysStat) {													
#ifdef _VDV_APP
			case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON（状態判定）	*/
			case (SYS_OPERATION_NORMAL_OFF):			/*通常:OFFモード		*/
			case (SYS_OPERATION_NORMAL_AUTO):			/*通常:オートREC		*/
			case (SYS_OPERATION_NORMAL_MANUAL):			/*通常:マニュアルREC	*/
			case (SYS_OPERATION_SIMULATION):			/*シミュレーション		*/
			case (SYS_OPERATION_EEPROM_WRITE_COMP):		/*EEPROM書換完了		*/
			case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF移行			*/
				led_pattern = LED_PTR_ON;
				break;
			case (SYS_OPERATION_ERROR):					/*異常					*/
				/*モジュール状態取得*/
				/* 異常時はOFF */
				led_pattern = LED_PTR_OFF;
				break;
			case (SYS_OPERATION_EEPROM_WRITE):			/*EEPROM書換中			*/
			case (SYS_OPERATION_GNSS_COMM):				/**< GNSSモジュール通信モード*/
			case (SYS_OPERATION_BT_COMM):				/**< BTモジュール通信モード	*/
				led_pattern = LED_PTR_FLASH_WRITING;
				break;
			case (SYS_OPERATION_WAIT_ACC_ON):			/*ACC ON待ち			*/
			case (SYS_OPERATION_ACC_OFF):				/*ACC OFF				*/
				led_pattern = LED_PTR_OFF;
				break;
			case (SYS_OPERATION_INSPECTION):			/**< 検査モード				*/
			case (SYS_OPERATION_INSPECTION_COMP):		/**< 検査完了				*/
				led_pattern = output_control_getLedPattern_inspection(ledType);
				break;
#else /* _VDV_APP */
			case (SYS_OPERATION_FIRMWARE_WRITE):		/*ファームウェア書換中	*/
				led_pattern = LED_PTR_FLASH_WRITING;
				break;
			case (SYS_OPERATION_FIRMWARE_WRITE_COMP):	/*ファームウェア書換完了*/
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
		/**< SWインジケータ				*/
		switch (sysStat) {													
#ifdef _VDV_APP
			case (SYS_OPERATION_NORMAL_OFF):			/*通常:OFFモード		*/
				/*ペアリング中かどうか判定*/
				btStat = Bt_control_GetStatus();
				if(btStat == BT_CONTROL_STAT_PAIRING)
				{
					/* ペアリング中 */
					led_pattern = LED_PTR_FLASH_PAIRING;
				}
				else
				{
					/* ペアリング中でない */
					led_pattern = LED_PTR_OFF;
				}
				break;
			case (SYS_OPERATION_NORMAL_AUTO):			/*通常:オートREC		*/
			case (SYS_OPERATION_NORMAL_MANUAL):			/*通常:マニュアルREC	*/
				/*REC中かどうか判定*/
				sdStat = Sd_control_GetRecStatus();
				if(sdStat == SD_CONTROL_REC_STAT_RECORDING)
				{
					/**< SDモジュール状態:REC中			*/
					if (Signal_control_isGnssEnable() == N_TRUE) 
					{
						/*GNSS測位*/
						led_pattern = LED_PTR_ON;
					}
					else
					{
						/*GNSS未測位*/
						led_pattern = LED_PTR_FLASH_REC_NO_GPS;
					}
				}
				else
				{
					/**< SDモジュール状態:REC停止中		*/
					led_pattern = LED_PTR_OFF;
				}
				break;
			case (SYS_OPERATION_ERROR):					/*異常					*/
				/* 異常時はOFF */
				led_pattern = LED_PTR_OFF;
				break;
			case (SYS_OPERATION_EEPROM_WRITE):			/*EEPROM書換中			*/
				led_pattern = LED_PTR_FLASH_WRITING;
				break;
			case (SYS_OPERATION_WAIT_ACC_ON):			/*ACC ON待ち			*/
			case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON（状態判定）	*/
			case (SYS_OPERATION_SIMULATION):			/*シミュレーション		*/
			case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF移行			*/
			case (SYS_OPERATION_ACC_OFF):				/*ACC OFF				*/
			case (SYS_OPERATION_EEPROM_WRITE_COMP):		/*EEPROM書換完了		*/
			case (SYS_OPERATION_GNSS_COMM):				/**< GNSSモジュール通信モード*/
			case (SYS_OPERATION_BT_COMM):				/**< BTモジュール通信モード	*/
				led_pattern = LED_PTR_OFF;
				break;
			case (SYS_OPERATION_INSPECTION):			/**< 検査モード				*/
			case (SYS_OPERATION_INSPECTION_COMP):		/**< 検査完了				*/
				led_pattern = output_control_getLedPattern_inspection(ledType);
				break;
#else /* _VDV_APP */
			case (SYS_OPERATION_FIRMWARE_WRITE):		/*ファームウェア書換中	*/
				led_pattern = LED_PTR_FLASH_WRITING;
				break;
			case (SYS_OPERATION_FIRMWARE_WRITE_COMP):	/*ファームウェア書換完了*/
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
		/**< SWバックライト				*/
		switch (sysStat) {													
#ifdef _VDV_APP
			case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON（状態判定）	*/
			case (SYS_OPERATION_NORMAL_AUTO):			/*通常:オートREC		*/
			case (SYS_OPERATION_NORMAL_MANUAL):			/*通常:マニュアルREC	*/
			case (SYS_OPERATION_SIMULATION):			/*シミュレーション		*/
			case (SYS_OPERATION_EEPROM_WRITE):			/*EEPROM書換中			*/
			case (SYS_OPERATION_EEPROM_WRITE_COMP):		/*EEPROM書換完了		*/
			case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF移行			*/
			case (SYS_OPERATION_GNSS_COMM):				/**< GNSSモジュール通信モード*/
			case (SYS_OPERATION_BT_COMM):				/**< BTモジュール通信モード	*/
				led_pattern = LED_PTR_ON;
				break;
			case (SYS_OPERATION_NORMAL_OFF):			/*通常:OFFモード		*/
				/*ペアリング中かどうか判定*/
				btStat = Bt_control_GetStatus();
				if(btStat == BT_CONTROL_STAT_PAIRING)
				{
					/* ペアリング中 */
					led_pattern = LED_PTR_FLASH_PAIRING;
				}
				else
				{
					/* ペアリング中でない */
					led_pattern = LED_PTR_ON;
				}
				break;
			case (SYS_OPERATION_ERROR):					/*異常					*/
				/* 異常時はOFF */
				led_pattern = LED_PTR_OFF;
				break;
			case (SYS_OPERATION_WAIT_ACC_ON):			/*ACC ON待ち			*/
			case (SYS_OPERATION_ACC_OFF):				/*ACC OFF				*/
				led_pattern = LED_PTR_OFF;
				break;
			case (SYS_OPERATION_INSPECTION):			/**< 検査モード				*/
			case (SYS_OPERATION_INSPECTION_COMP):		/**< 検査完了				*/
				led_pattern = output_control_getLedPattern_inspection(ledType);
				break;
#else /* _VDV_APP */
			case (SYS_OPERATION_FIRMWARE_WRITE):		/*ファームウェア書換中	*/
			case (SYS_OPERATION_FIRMWARE_WRITE_COMP):	/*ファームウェア書換完了*/
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
/* 関数名称		: output_control_getLedLuminance								*/
/* 機能の説明	: LED輝度取得													*/
/* 引数			: (I/)ledType:LED種別											*/
/* 				:	LED_TYPE_BODY_IND 	本体インジケータ						*/
/* 				:	LED_TYPE_SW_IND		SWインジケータ							*/
/* 				:	LED_TYPE_SW_BKLT	SWバックライト							*/
/* 				: (I/)sysStat:システム状態										*/
/* 				: SYS_OPERATION_WAIT_ACC_ON				ACC ON待ち				*/
/* 				: SYS_OPERATION_ACC_ON_UNKNOWN_STAT		ACC ON（状態判定）		*/
/* 				: SYS_OPERATION_NORMAL_OFF				通常:OFFモード			*/
/* 				: SYS_OPERATION_NORMAL_AUTO				通常:オートREC			*/
/* 				: SYS_OPERATION_NORMAL_MANUAL			通常:マニュアルREC		*/
/* 				: SYS_OPERATION_ERROR					異常					*/
/* 				: SYS_OPERATION_SIMULATION				シミュレーション		*/
/* 				: SYS_OPERATION_EEPROM_WRITE			EEPROM書換中			*/
/* 				: SYS_OPERATION_EEPROM_WRITE_COMP		EEPROM書換完了			*/
/* 				: SYS_OPERATION_FIRMWARE_WRITE			ファームウェア書換中	*/
/* 				: SYS_OPERATION_FIRMWARE_WRITE_COMP		ファームウェア書換完了	*/
/* 				: SYS_OPERATION_PRE_ACC_OFF				ACC OFF移行				*/
/* 				: SYS_OPERATION_ACC_OFF					ACC OFF					*/
/* 戻り値		: UI_8:LED輝度													*/
/* 				: LED_DUTY_MAX	最大duty比（調光レベル)							*/
/* 				: LED_DUTY_MID	中間duty比（調光レベル)							*/
/* 				: LED_DUTY_MIN	最小duty比（調光レベル)							*/
/* 作成者		: (OFT)Sakata													*/
/* 作成日		: 17/01/20														*/
/* 備考			: 																*/
/********************************************************************************/
OUTLED_LOCAL UI_8 output_control_getLedLuminance(LED_TYPE ledType, E_SYS_CTRL_STATUS sysStat)
{
	UI_8 led_luminance = LED_DUTY_MAX; /* デフォルト最大値 */
	if(ledType == LED_TYPE_BODY_IND)
	{
		/**< 本体インジケータ			*/
#ifdef _VDV_APP
		if ((sysStat == SYS_OPERATION_INSPECTION) ||
			(sysStat == SYS_OPERATION_INSPECTION_COMP))
		{
			/* 検査モード中 */
			led_luminance = output_control_getLedLuminance_inspcetion(ledType);
		}
#else /* _VDV_APP */
#endif /* _VDV_APP */
	}
	else if(ledType == LED_TYPE_SW_IND)
	{
		/**< SWインジケータ				*/
#ifdef _VDV_APP
		if ((sysStat == SYS_OPERATION_INSPECTION) ||
			(sysStat == SYS_OPERATION_INSPECTION_COMP))
		{
			/* 検査モード中 */
			led_luminance = output_control_getLedLuminance_inspcetion(ledType);
		}
#else /* _VDV_APP */
#endif /* _VDV_APP */
	}
	else if(ledType == LED_TYPE_SW_BKLT)
	{
		/**< SWバックライト				*/
#ifdef _VDV_APP
		if ((sysStat == SYS_OPERATION_INSPECTION) ||
			(sysStat == SYS_OPERATION_INSPECTION_COMP))
		{
			/* 検査モード中 */
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
/* 関数名称		: output_control_getLedPattern_inspection						*/
/* 機能の説明	: LEDパターン取得(検査モード用)									*/
/* 引数			: (I/)ledType:LED種別(LED_TYPE_xxx)								*/
/* 戻り値		: LEDパターン(LED_PTR_xxx)										*/
/* 作成者		: (OFT)Toguchi													*/
/* 作成日		: 17/01/20														*/
/* 備考			: 																*/
/********************************************************************************/
OUTLED_LOCAL E_LED_PATTERN output_control_getLedPattern_inspection(LED_TYPE ledType)
{
	E_LED_PATTERN	led_pattern = LED_PTR_OFF;
	E_INSP_LED_MODE	insp_led_mode;

	/* 検査中のLEDモード取得	*/
	insp_led_mode = Insp_get_led_mode();

	if (ledType == LED_TYPE_BODY_IND) {				/* LED種別:本体インジケータ		*/
		if ((insp_led_mode == INSP_LED_MODE_NOMAL) ||			/* 検査LEDモード:通常点灯		*/
			(insp_led_mode == INSP_LED_MODE_BT_PAIRING) ||		/* 検査LEDモード:Buletoothペアリング		*/
			(insp_led_mode == INSP_LED_MODE_SW_INSPECT)) {		/* 検査LEDモード:SW検査		*/
			/* 点灯		*/
			led_pattern = LED_PTR_ON;
		}
	}
	else if (ledType == LED_TYPE_SW_IND) {			/* LED種別:SWインジケータ			*/
		if (insp_led_mode == INSP_LED_MODE_BT_PAIRING) {		/* 検査LEDモード:Buletoothペアリング		*/
			/* 点滅（Buletoothペアリング用）	*/
			led_pattern = LED_PTR_FLASH_PAIRING;
		}
		else if (insp_led_mode == INSP_LED_MODE_SW_INSPECT) {	/* 検査LEDモード:SW検査					*/
			/* 点灯		*/
			led_pattern = LED_PTR_ON;
		}
		else {
			/* nop */
		}
	}
	else if (ledType == LED_TYPE_SW_BKLT) {			/* LED種別:SWバックライト			*/
		if ((insp_led_mode == INSP_LED_MODE_NOMAL) ||			/* 検査LEDモード:通常点灯		*/
			(insp_led_mode == INSP_LED_MODE_SW_INSPECT)) {		/* 検査LEDモード:SW検査		*/
			/* 点灯		*/
			led_pattern = LED_PTR_ON;
		}
		else if (insp_led_mode == INSP_LED_MODE_BT_PAIRING) {	/* 検査LEDモード:Buletoothペアリング		*/
			/* 点滅（Buletoothペアリング用）	*/
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
/* 関数名称		: output_control_getLedLuminance_inspcetion						*/
/* 機能の説明	: LED輝度取得(検査モード用)										*/
/* 引数			: (I/)ledType:LED種別(LED_TYPE_xxx)								*/
/* 戻り値		: UI_8:LED輝度													*/
/* 				: LED_DUTY_MAX	最大duty比（調光レベル)							*/
/* 				: LED_DUTY_MID	中間duty比（調光レベル)							*/
/* 				: LED_DUTY_MIN	最小duty比（調光レベル)							*/
/* 作成者		: (OFT)Toguchi													*/
/* 作成日		: 17/01/20														*/
/* 備考			: 																*/
/********************************************************************************/
OUTLED_LOCAL UI_8 output_control_getLedLuminance_inspcetion(LED_TYPE ledType)
{
	UI_8			led_luminance = LED_DUTY_MAX;	/* デフォルト最大値 */
	E_INSP_LED_MODE	insp_led_mode;

	/* 検査中のLEDモード取得	*/
	insp_led_mode = Insp_get_led_mode();

	if (ledType == LED_TYPE_BODY_IND) {				/* LED種別:本体インジケータ		*/
		if ((insp_led_mode == INSP_LED_MODE_NOMAL) ||			/* 検査LEDモード:通常点灯		*/
			(insp_led_mode == INSP_LED_MODE_BT_PAIRING) ||		/* 検査LEDモード:Buletoothペアリング		*/
			(insp_led_mode == INSP_LED_MODE_SW_INSPECT)) {		/* 検査LEDモード:SW検査		*/
			/* 中間duty比		*/
			led_luminance = LED_DUTY_MID;
		}
	}
	else if (ledType == LED_TYPE_SW_IND) {			/* LED種別:SWインジケータ			*/
		if (insp_led_mode == INSP_LED_MODE_SW_INSPECT) {		/* 検査LEDモード:SW検査					*/
			/* 最小duty比		*/
			led_luminance = LED_DUTY_MIN;
		}
	}
	else if (ledType == LED_TYPE_SW_BKLT) {			/* LED種別:SWバックライト			*/
		/* nop */
	}
	else {
		/* nop */
	}

	return led_luminance;
}
#endif /* _VDV_APP */

