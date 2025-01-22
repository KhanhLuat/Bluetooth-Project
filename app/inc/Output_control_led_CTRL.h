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
/* 																			*/
/* 備考																		*/
/*				無し														*/
/*																			*/
/*--------------------------------------------------------------------------*/
#ifndef	OUTPUT_CONTROL_LED_CTRL_H
#define	OUTPUT_CONTROL_LED_CTRL_H

/*--------------------------------------------------------------------------*/
/*	外部マクロ																*/
/*--------------------------------------------------------------------------*/
/* 
 ** LED種別
 */ 
typedef enum led_type
{
	LED_TYPE_BODY_IND	= 0,		/**< 本体インジケータ			*/
	LED_TYPE_SW_IND,				/**< SWインジケータ				*/
	LED_TYPE_SW_BKLT,				/**< SWバックライト				*/
	LED_TYPE_NUM					/**< LED位置数					*/
} LED_TYPE;

/* LED */
#define		LED_ON						(1)					/*  LED点灯(H) */
#define		LED_OFF						(0)					/*  LED消灯(L) */


/*--------------------------------------------------------------------------*/
/*	構造体宣言																*/
/*--------------------------------------------------------------------------*/
typedef struct t_led_output {
															/* ﾗﾝﾌﾟ */
	UI_8		Power;										/* 電源LED（ON/OFF) */
	UI_8		Duty;										/* Duty調光（明るさ指定） */
} T_LED_OUTPUT;


/*--------------------------------------------------------------------------*/
/*	プロトタイプ宣言														*/
/*--------------------------------------------------------------------------*/
void Output_control_led_main(void);						/* LED出力状態判定	[機能関数] */
void Output_control_led_ram_init(void);					/* RAM初期化		[機能関数] */
T_LED_OUTPUT Get_output_led_status(LED_TYPE ledType);	/* LED状態の取得	 */
#ifdef _VDV_BOOT
UI_16 output_control_GetLedChangeTime(LED_TYPE ledType);/* LED状態変化までの時間取得	 */
void output_control_NotifyEndFlashErase(UI_16 eraseTime);/* FLASH消去時間通知 */
#endif /* _VDV_BOOT */

#endif		/* OUTPUT_CONTROL_LED_CTRL_H */
