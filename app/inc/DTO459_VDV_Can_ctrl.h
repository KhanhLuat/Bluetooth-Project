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
/* file DTO459_VDV_Can_ctrl.h							*/
/* CAN制御												*/
/********************************************************/
#ifndef	DTO459_VDV_CAN_CTRL_H
#define	DTO459_VDV_CAN_CTRL_H

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include "type.h"

/********************************************************/
/*	マクロ定義											*/
/********************************************************/
/* T-CAN車両情報構造体 */
typedef struct {
									/*[ID:152]***********************************/
	UI_8	en_ign:1;				/* データ有効フラグ:IGN						*/
	UI_8	en_acc:1;				/* 					 ACC					*/
	UI_8	en_brake_lamp_sw:1;		/* 					 ブレーキランプSW		*/
	UI_8	en_hand_brake_sw:1;		/* 					 パーキングランプSW		*/
	UI_8	en_mt_rev_sw:1;			/* 					 MTリバースSW			*/
	UI_8	en_head_lamp_hi:1;		/* 					 ヘッドランプ Hiビーム	*/
	UI_8	en_head_lamp_lo:1;		/* 					 ヘッドランプ Loビーム	*/
	UI_8	en_head_lamp_small:1;	/* 					 ヘッドランプ スモール	*/
									/*------------------------------------------*/
	UI_8	ign:1;					/* データ実体:IGN 							*/
	UI_8	acc:1;					/* 			   ACC 							*/
	UI_8	brake_lamp_sw:1;		/* 			   ブレーキランプSW 			*/
	UI_8	hand_brake_sw:1;		/* 			   パーキングランプSW 			*/
	UI_8	mt_rev_sw:1;			/* 			   MTリバースSW 				*/
	UI_8	head_lamp_hi:1;			/* 			   ヘッドランプ Hiビーム	 	*/
	UI_8	head_lamp_lo:1;			/* 			   ヘッドランプ Loビーム 		*/
	UI_8	head_lamp_small:1;		/* 			   ヘッドランプ スモール 		*/
									/*											*/
									/*[ID:374]***********************************/
	UI_8	en_door_sw_trunk:1;		/* データ有効フラグ:ドアSW トランク			*/
	UI_8	en_door_sw_seat_p:1;	/* 					 ドアSW P席				*/
	UI_8	en_door_sw_seat_d:1;	/* 					 ドアSW D席				*/
	UI_8	en_door_sw_hood:1;		/* 					 ドアSW フード			*/
									/*------------------------------------------*/
	UI_8	door_sw_trunk:1;		/* データ実体:ドアSW トランク				*/
	UI_8	door_sw_seat_p:1;		/* 			   ドアSW P席					*/
	UI_8	door_sw_seat_d:1;		/* 			   ドアSW D席					*/
	UI_8	door_sw_hood:1;			/* 			    ドアSW フード				*/
									/*											*/
									/*[ID:282]***********************************/
	UI_8	en_out_temp:1;			/* データ有効フラグ:表示外気温				*/
	UI_8	en_seat_belt_sw_p:1;	/* 					 シートベルトバックルSW P席	*/
	UI_8	en_seat_belt_sw_d:1;	/* 					 シートベルトバックルSW D席	*/
	UI_8	en_destination:1;		/* 					 仕向地情報				*/
	UI_8	en_brand_flag:1;		/* 					 ブランドフラグ			*/
	UI_8	en_blinker_left:1;		/* 					 ウィンカー左			*/
	UI_8	en_blinker_right:1;		/* 			   		 ウィンカー右			*/
									/*------------------------------------------*/
	UI_8	out_temp:8;				/* データ実体:表示外気温					*/
	UI_8	seat_belt_sw_p:1;		/* 			   シートベルトバックルSW P席	*/
	UI_8	seat_belt_sw_d:1;		/* 			   シートベルトバックルSW D席	*/
	UI_8	destination:4;			/* 			   仕向地情報					*/
	UI_8	brand_flag:1;			/* 			   ブランドフラグ				*/
	UI_8	blinker_left:1;			/* 			   ウィンカー左					*/
	UI_8	blinker_right:1;		/* 			   ウィンカー右					*/
									/*											*/
									/*[ID:140]***********************************/
	UI_8	en_accel_ratio:1;		/* データ有効フラグ:アクセル開度			*/
	UI_8	en_eng_rpm:1;			/* 					 エンジン回転数			*/
									/*------------------------------------------*/
	UI_8	accel_ratio:8;			/* データ実体:アクセル開度					*/
	UI_16	eng_rpm:14;				/* 			   エンジン回転数				*/
									/*											*/
									/*[ID:141]***********************************/
	UI_8	en_eng_rpm_taco:1;		/* データ有効フラグ:エンジン回転数タコメータ用	*/
									/*------------------------------------------*/
	UI_16	eng_rpm_taco:14;		/* データ実体:エンジン回転数タコメータ用	*/
									/*											*/
									/*[ID:144]***********************************/
	UI_8	en_transmission_type:1;	/* データ有効フラグ:トランスミッション情報	*/
									/*------------------------------------------*/
	UI_8	transmission_type:1;	/* データ実体:トランスミッション情報		*/
									/*											*/
									/*[ID:360]***********************************/
	UI_8	en_temp_oil:1;			/* データ有効フラグ:エンジン油温			*/
	UI_8	en_temp_coolant:1;		/* 					 エンジン水温			*/
	UI_8	en_cruise_controlrl_set:1;	/* 				 クルコンセットランプ	*/
	UI_8	en_intake_press;		/* 					 吸入管圧力				*/
	UI_8	en_atmospheric_press;	/* 					 大気圧					*/
									/*------------------------------------------*/
	UI_8	temp_oil;				/* データ実体:エンジン油温					*/
	UI_8	temp_coolant;			/* 			   エンジン水温					*/
	UI_8	cruise_controlrl_set:1;	/* 			   クルコンセットランプ			*/
	UI_8	intake_press;			/* 			   吸入管圧力					*/
	UI_8	atmospheric_press;		/* 			   大気圧						*/
									/*											*/
									/*[ID:361]***********************************/
	UI_8	en_mt_gear_pos:1;		/* データ有効フラグ:MTギアポジション		*/
									/*------------------------------------------*/
	UI_8	mt_gear_pos:3;			/* データ実体:MTギアポジション				*/
									/*											*/
									/*[ID:148]***********************************/
	UI_8	en_sports_gear_pos:1;	/* データ有効フラグ:スポーツギアポジション	*/
	UI_8	en_shift_pos:1;			/* 					 シフトポジション（フェールセーフ処理後）*/
	UI_8	en_mode_shift_control:1;/* 					 TCM変速制御モード		*/
	UI_8	en_target_gear_pos:1;	/* 					 目標ギア位置			*/
									/*------------------------------------------*/
	UI_8	sports_gear_pos:4;		/* データ実体:スポーツギアポジション		*/
	UI_8	shift_pos:4;			/* 			   シフトポジション（フェールセーフ処理後）	*/
	UI_8	mode_shift_control:3;	/* 			   TCM変速制御モード			*/
	UI_8	target_gear_pos:4;		/* 			   目標ギア位置					*/
									/*											*/
									/*[ID:368]***********************************/
	UI_8	en_at_type:1;			/* データ有効フラグ:AT種類					*/
	UI_8	en_sports_lamp:1;		/* 					 スポーツランプ			*/
	UI_8	en_snow_hold_lamp:1;	/* 					 Tスノーホールドランプ	*/
									/*------------------------------------------*/
	UI_8	at_type:3;				/* データ実体:AT種類						*/
	UI_8	sports_lamp:2;			/* 			   スポーツランプ				*/
	UI_8	snow_hold_lamp:1;		/* 			   スノーホールドランプ			*/
									/*											*/
									/*[ID:375]***********************************/
	UI_8	en_lock_stat_pass:1;	/* データ有効フラグ:ドアロック状態（助手席）*/
	UI_8	en_lock_stat_driver:1;	/* 					 ドアロック状態（運転席）*/
									/*------------------------------------------*/
	UI_8	lock_stat_pass:1;		/* データ実体:ドアロック状態（助手席）		*/
	UI_8	lock_stat_driver:1;		/* 			   ドアロック状態（運転席）		*/
									/*											*/
									/*[ID:0d0]***********************************/
	UI_8	en_steer_angle:1;		/* データ有効フラグ:舵角センサ				*/
	UI_8	en_yaw_rate:1;			/* 					 ヨーレート				*/
	UI_8	en_acc_lr:1;			/* 					 横加速度				*/
	UI_8	en_acc_fr:1;			/* 					 前後Ｇセンサ			*/
									/*------------------------------------------*/
	UI_16	steer_angle;			/* データ実体:舵角センサ					*/
	UI_16	yaw_rate;				/* 			   ヨーレート					*/
	UI_8	acc_lr;					/* 			   横加速度						*/
	UI_8	acc_fr;					/* 			   前後Ｇセンサ					*/
									/*											*/
									/*[ID:0d1]***********************************/
	UI_8	en_wheel_speed_ave:1;	/* データ有効フラグ:駆動輪平均車輪速		*/
	UI_8	en_brake_pressure:1;	/*					 ブレーキ圧				*/
									/*------------------------------------------*/
	UI_16	wheel_speed_ave;		/* データ実体:駆動輪平均車輪速				*/
	UI_8	brake_pressure;			/*			   ブレーキ圧					*/
									/*											*/
									/*[ID:0d3]***********************************/
	UI_8	en_speed_pulse_integ:1;	/* データ有効フラグ:車速パルス信号積算値	*/
									/*------------------------------------------*/
	UI_32	speed_pulse_integ:6;	/* データ実体:車速パルス信号積算値			*/
									/*											*/
									/*[ID:0d4]***********************************/
	UI_8	en_speed_wheel_ftlh:1;	/* データ有効フラグ:左前輪車輪速			*/
	UI_8	en_speed_wheel_ftrh:1;	/* 					右前輪車輪速			*/
	UI_8	en_speed_wheel_rrlh:1;	/* 					左後輪車輪速			*/
	UI_8	en_speed_wheel_rrrh:1;	/* 					右前輪車輪速			*/
									/*------------------------------------------*/
	UI_16	speed_wheel_ftlh;		/* データ実体:左前輪車輪速					*/
	UI_16	speed_wheel_ftrh;		/* 			  右前輪車輪速					*/
	UI_16	speed_wheel_rrlh;		/* 			  左後輪車輪速					*/
	UI_16	speed_wheel_rrrh;		/* 			  右前輪車輪速					*/
									/*											*/
									/*[ID:44d]***********************************/
	UI_8	en_node_id:1;			/* データ有効フラグ:ノードID				*/
	UI_8	en_sleep_ack:1;			/* 				     Sleep Ack				*/
	UI_8	en_sleep_ind:1;			/* 				     Sleep Ind				*/
	UI_8	en_limp_home:1;			/* 				     Limp Home				*/
	UI_8	en_ring:1;				/* 				     Ring					*/
	UI_8	en_alive:1;				/* 				     Alive					*/
	UI_8	en_wkup:1;				/* 				     Wkup					*/
	UI_8	en_init:1;				/* 				     Init					*/
	UI_8	en_spec_no:1;			/* 				     Spec. No.				*/
									/*------------------------------------------*/
	UI_8	node_id;				/* データ実体:ノードID						*/
	UI_8	sleep_ack:1;			/* 			   Sleep Ack					*/
	UI_8	sleep_ind:1;			/* 			   Sleep Ind					*/
	UI_8	limp_home:1;			/* 			   Limp Home					*/
	UI_8	ring:1;					/* 			   Ring							*/
	UI_8	alive:1;				/* 			   Alive						*/
	UI_8	wkup:1;					/* 			   Wkup							*/
	UI_8	init:1;					/* 			   Init							*/
	UI_16	spec_no;				/* 			   Spec. No.					*/
									/*											*/
									/*[ID:6E2]***********************************/
									/* データ有効フラグ：						*/
	UI_8	en_air_press_ftlh:1;	/* 			左前輪空気圧					*/
	UI_8	en_air_press_ftrh:1;	/* 			右前輪空気圧					*/
	UI_8	en_air_press_rrlh:1;	/* 			左後輪空気圧					*/
	UI_8	en_air_press_rrrh:1;	/* 			右後輪空気圧					*/
	UI_8	en_air_press_unit:1;	/* 			空気圧	単位情報				*/
									/*------------------------------------------*/
									/* データ実体：								*/
	UI_8	air_press_ftlh:7;		/* 			左前輪空気圧					*/
	UI_8	air_press_ftrh:7;		/* 			右前輪空気圧					*/
	UI_8	air_press_rrlh:7;		/* 			左後輪空気圧					*/
	UI_8	air_press_rrrh:7;		/* 			右後輪空気圧					*/
	UI_8	air_press_unit:2;		/* 			空気圧	単位情報				*/
									/*											*/
									/*[ID:048]***********************************/
									/* データ有効フラグ：						*/
	UI_8	en_manual_mode_sw:1;	/* 			マニュアルモードSW				*/
									/*------------------------------------------*/
									/* データ実体：								*/
	UI_8	manual_mode_sw:1;		/* 			マニュアルモードSW				*/
									/*[ID:139]***********************************/
									/* データ有効フラグ：						*/
	UI_8	en_trcAct:1;			/* 			TRC制御状態						*/
	UI_8	en_absAct:1;			/* 			ABS制御状態						*/
	UI_8	en_absFail:1;			/* 			ABSシステムフェイル状態			*/
	UI_8	en_vscUnderStr:1;		/* 			VSCアンダーステア状態			*/
	UI_8	en_vscOverStr:1;		/* 			VSCオーバーステア状態			*/
									/*------------------------------------------*/
									/* データ実体： 							*/
	UI_8	trcAct:1;				/* 			TRC制御状態						*/
	UI_8	absAct:1;				/* 			ABS制御状態						*/
	UI_8	absFail:1;				/* 			ABSシステムフェイル状態			*/
	UI_8	vscUnderStr:1;			/* 			VSCアンダーステア状態			*/
	UI_8	vscOverStr:1;			/* 			VSCオーバーステア状態			*/
									/*											*/
									/*[ID:328]***********************************/
									/* データ有効フラグ：						*/
	UI_8	en_vscAct:1;			/* 			VSC制御状態						*/
	UI_8	en_vscOnOff:1;			/* 			VSCON/OFF状態					*/
	UI_8	en_trcOnOff:1;			/* 			TRCON/OFF状態					*/
	UI_8	en_trcMode:1;			/* 			トラクション重視モード状態		*/
	UI_8	en_vscWarning:1;		/* 			VSCウォーニング状態				*/
	UI_8	en_absWarning:1;		/* 			ABSウォーニング状態				*/
	UI_8	en_ecbWarning:1;		/* 			ECBウォーニング状態				*/
									/*------------------------------------------*/
									/* データ実体： 							*/
	UI_8	vscAct:1;				/* 			VSC制御状態						*/
	UI_8	vscOnOff:1;				/* 			VSCON/OFF状態					*/
	UI_8	trcOnOff:1;				/* 			TRCON/OFF状態					*/
	UI_8	trcMode:1;				/* 			トラクション重視モード状態		*/
	UI_8	vscWarning:1;			/* 			VSCウォーニング状態				*/
	UI_8	absWarning:1;			/* 			ABSウォーニング状態				*/
	UI_8	ecbWarning:1;			/* 			ECBウォーニング状態				*/
									/*											*/
									/*[ID:13B]***********************************/
									/* データ有効フラグ：						*/
	UI_8	en_engTrqUpReqFlag:1;	/* 			エンジントルクアップ要求フラグ	*/
	UI_8	en_engTrqDwnReqFlag:1;	/* 			エンジントルク減少要求フラグ	*/
	UI_8	en_engTrqUpReqVal:1;	/* 			エンジントルクアップ要求値		*/
	UI_8	en_engTrqDwnReqVal:1;	/* 			エンジントルク減少要求値		*/
									/*------------------------------------------*/
									/* データ実体： 							*/
	UI_8	engTrqUpReqFlag:1;		/* 			エンジントルクアップ要求フラグ	*/
	UI_8	engTrqDwnReqFlag:1;		/* 			エンジントルク減少要求フラグ	*/
	UI_16	engTrqUpReqVal;			/* 			エンジントルクアップ要求値		*/
	UI_16	engTrqDwnReqVal;		/* 			エンジントルク減少要求値		*/
									/*[ID:041]***********************************/
									/* データ有効フラグ：						*/
	UI_8	en_engTrcShaft:1;		/* 			実エンジン軸トルク				*/
									/*------------------------------------------*/
									/* データ実体： 							*/
	UI_16	engTrcShaft:15;			/* 			実エンジン軸トルク				*/
									/*[内部積算値]*******************************/
									/* データ有効フラグ（バックアップ）：		*/
	UI_8	en_eep_vehicle_info:1;	/* 			EEPROM保持車種情報				*/
									/*------------------------------------------*/
									/* データ実体：（バックアップ）				*/
	UI_16	eep_vehicle_info;		/* 			車種情報						*/
									/*[内部積算値]*******************************/
	UI_8	en_running_distance:1;	/* データ有効フラグ:始動後走行距離取得		*/
	UI_8	en_fuel_consumption:1;	/* 					 燃料噴射量積算値		*/
	UI_8	en_vehicle_speed_for_disp;/*                 表示用車速				*/
	UI_8	en_smoothing_brake_pressure:1;/*			 なましブレーキ圧		*/
	UI_8	en_vehicle_info:1;		/* 					 EEPROM保持車種情報		*/
									/*------------------------------------------*/
	UI_32	running_distance;		/* データ実体:始動後走行距離取得			*/
	UI_32	fuel_consumption;		/* 			   燃料噴射量積算値				*/
	UI_32	vehicle_speed_for_disp;	/*			   表示用車速(0.00001km/h単位)	*/
	UI_8	smoothing_brake_pressure;/*			   なましブレーキ圧				*/
	UI_16	vehicle_info;			/* 			   車種情報(EEPROMとは定義値が異なる)*/
} T_Can_ctrl_tcan_info;

/* ISO-CAN車両情報構造体 */
typedef struct {
	UI_8	en_temp_colant:1;		/* データ有効フラグ:エンジン水温			*/
	UI_8	en_ima_pressure:1;		/* 					 Intake manifold absolute pressure	*/
	UI_8	en_eng_rpm:1;			/* 					 エンジン回転数			*/
	UI_8	en_vehicle_speed:1;		/* 					 車速					*/
	UI_8	en_throttle_pos:1;		/* 					 スロットル位置			*/
	UI_8	en_volts:1;				/* 			 		 電圧					*/
	UI_8	en_temp_intake_manifold:1;/* 				 吸気温度				*/
									/*------------------------------------------*/
	UI_8	temp_colant;			/* データ実体:エンジン水温					*/
	UI_8	ima_pressure;			/* 			   Intake manifold absolute pressure	*/
	UI_16	eng_rpm;				/* 			   エンジン回転数 				*/
	UI_8	vehicle_speed;			/* 			   車速	 						*/
	UI_8	throttle_pos;			/* 			   スロットル位置				*/
	UI_16	volts;					/* 			   電圧							*/
	UI_8	temp_intake_manifold;	/* 			   吸気温度						*/
} T_Can_ctrl_iso_can_info;


/* CAN信号:ブランドフラグ */
#define CAN_BRAND_FLAG_TMC							0			/*	ブランドフラグ:TMC												*/
#define CAN_BRAND_FLAG_FHI							1			/*	ブランドフラグ:FHI												*/
/* CAN信号:仕向け情報 */
#define CAN_DESTINATION_JP							1			/*	仕向け:日本国内													*/
#define CAN_DESTINATION_US							2			/*	仕向け:米国														*/
#define CAN_DESTINATION_CA							3			/*	仕向け:カナダ													*/
#define CAN_DESTINATION_EU							4			/*	仕向け:欧州														*/
#define CAN_DESTINATION_K4							5			/*	仕向け:一般														*/
#define CAN_DESTINATION_KS							6			/*	仕向け:中近東													*/
#define CAN_DESTINATION_UK							7			/*	仕向け:イギリス													*/
#define CAN_DESTINATION_TH							8			/*	仕向け:タイ・シンガポール										*/
#define CAN_DESTINATION_AU							9			/*	仕向け:オーストラリア											*/
#define CAN_DESTINATION_CN							10			/*	仕向け:中国														*/
#define CAN_DESTINATION_KR							11			/*	仕向け:韓国														*/
#define CAN_DESTINATION_KC							12			/*	仕向け:南アフリカ他												*/
/* CAN信号:トランスミッション種別 */
#define	CAN_TRANSMISSION_ATORCVT					0x00		/*	トランスミッション種別 00: AT or CVT							*/
#define	CAN_TRANSMISSION_MT							0x01		/*	トランスミッション種別 01: MT									*/
/* CAN信号:AT種類 */
#define	CAN_AT_TYPE_CVT								0x01		/*	AT種類:CVT														*/
#define	CAN_AT_TYPE_4AT								0x02		/*	AT種類:4AT														*/
#define	CAN_AT_TYPE_5AT								0x03		/*	AT種類:5AT														*/
#define	CAN_AT_TYPE_6AT								0x04		/*	AT種類:6AT														*/
/* CAN信号:ドア状態 */
#define CAN_DOOR_SW_OPEN							0x01		/* ドアSW状態オープン												*/
#define CAN_DOOR_SW_CLOSE							0x00		/* ドアSW状態クローズ												*/
#define CAN_DOOR_LOCK								0x00		/* ドアロック状態ロック												*/
#define CAN_DOOR_UNLOCK								0x01		/* ドアロック状態アンロック											*/
/* CAN信号:シートベルト状態 */
#define CAN_SEATBELT_ON								0x00		/* シートベルト状態装着												*/
#define CAN_SEATBELT_OFF							0x01		/* シートベルト状態非装着											*/
/* CAN信号:スポーツランプ状態 */
#define	CAN_SPORTLAMP_TURN_OFF						0			/* スポーツランプ :消灯												*/
#define	CAN_SPORTLAMP_TURN_ON						1			/* スポーツランプ :点灯												*/
#define	CAN_SPORTLAMP_BLINK_2Hz						2			/* スポーツランプ :2Hz点滅											*/
#define	CAN_SPORTLAMP_BLINK_4Hz						3			/* スポーツランプ :4Hz点滅											*/
/* CAN信号:スノーホールドランプ状態 */
#define	CAN_SNOWLAMP_ON								1			/* スノーランプ :ON													*/
#define	CAN_SNOWLAMP_OFF							0			/* スノーランプ :OFF												*/
/* CAN信号:ブレーキランプON/OFF */
#define	CAN_BRAKE_LAMP_ON							1			/*	ブレーキランプ: ON												*/
#define	CAN_BRAKE_LAMP_OFF							0			/*	ブレーキランプ: OFF												*/
/* CAN信号:ヘッドライトON/OFF */
#define	CAN_LIGHT_TURN_ON							1			/*	ヘッドライト: ON												*/
#define	CAN_LIGHT_TURN_OFF							0			/*	ヘッドライト: OFF												*/
/* CAN信号:MTリバースSW */
#define	CAN_MT_REVERSE_ON							1			/*	MTリバース: ON（リバース状態）									*/
#define	CAN_MT_REVERSE_OFF							0			/*	MTリバース: OFF													*/
/* CAN信号:IG,ACC */
#define	CAN_POWER_ON								1			/*	電源（ACC,IG): ON												*/
#define	CAN_POWER_OFF								0			/*	電源（ACC,IG): OFF												*/
/* CAN信号:制御車速 */
#define	CAN_WHEELSPEED_RESOLUTION					0.05625		/*	４輪車速分解能変換用:0.05625（演算後、分解能1）					*/
#define	CAN_ACCERERATION_RESOLUTION_LR				0.1923		/*	横方向加速度分解能（演算後、分解能1）							*/
#define	CAN_ACCERERATION_RESOLUTION_FR				0.1			/*	前後方向加速度分解能（演算後、分解能1）							*/
/* CAN信号:ステアリング分解能 */
#define	CAN_STEERING_RESOLUTION						0.1			/*	ステアリング分解能（演算後、分解能1）							*/
/* CAN信号:ステアリング異常値 */
#define	CAN_STEERING_ERRORVALUE						0x8000		/*	ステアリング異常値												*/
/* CAN信号:ブレーキ圧分解能 */
#define	CAN_BRAKEPRESSURE_RESOLUTION				0.1305882	/*	ブレーキ圧分解能 0.1305882（演算後、分解能1）					*/
#define	CAN_BRAKEPRESSURE_MAX_86A					11.4		/*	ブレーキ圧分最大値のセンサー上限値19Mpaｘ60%					*/
#define	CAN_BRAKEPRESSURE_MAX_86B					13.5			/*	ブレーキ圧分最大値のセンサー上限値15Mpaｘ90%					*/
/* CAN信号:ヨーレイト */
#define	CAN_YAWRATE_RESOLUTION_FR					0.0047358	/* ヨーレイト分解能（演算後、分解能1）								*/
#define	CAN_YAWRATE_MAX								0x01FF		/* ヨーレイト 最大値												*/
#define	CAN_YAWRATE_MIN								0xFE00		/* ヨーレイト 最小値												*/
/* CAN信号:ATシフト */
#define	CAN_ATSHIFT_DS								0x0A		/* ATシフト:D-S														*/
#define	CAN_ATSHIFT_D								0x0B		/* ATシフト:D														*/
#define	CAN_ATSHIFT_N								0x0C		/* ATシフト:N														*/
#define	CAN_ATSHIFT_R								0x0D		/* ATシフト:R														*/
#define	CAN_ATSHIFT_P								0x0E		/* ATシフト:P														*/
#define	CAN_ATSHIFT_1								0x01		/* ATシフト:1レンジ													*/
#define	CAN_ATSHIFT_2								0x02		/* ATシフト:21レンジ												*/
#define	CAN_ATSHIFT_3								0x03		/* ATシフト:3レンジ													*/
#define	CAN_ATSHIFT_ERR								0x0F		/* ATシフト:エラー													*/
/* 86B要定義 */
/* CAN信号:ATシフト */
#define	CAN_ATSHIFT_86B_D							0x01		/* ATシフト:D														*/
#define	CAN_ATSHIFT_86B_N							0x02		/* ATシフト:N														*/
#define	CAN_ATSHIFT_86B_R							0x03		/* ATシフト:R														*/
#define	CAN_ATSHIFT_86B_P							0x04		/* ATシフト:P														*/
/* CAN信号ATギア段数 */
#define	CAN_ATGEAR_86B_NOT_D						0x00		/* ATギア：Dレンジ以外												*/
#define	CAN_ATGEAR_86B_MANUAL_1ST					0x01		/* ATギア：1ST(Mﾓｰﾄﾞ、ﾃﾝﾎﾟﾗﾘMﾓｰﾄﾞ)									*/
#define	CAN_ATGEAR_86B_MANUAL_2ND					0x02		/* ATギア：2ND(Mﾓｰﾄﾞ、ﾃﾝﾎﾟﾗﾘMﾓｰﾄﾞ)									*/
#define	CAN_ATGEAR_86B_MANUAL_3RD					0x03		/* ATギア：3RD(Mﾓｰﾄﾞ、ﾃﾝﾎﾟﾗﾘMﾓｰﾄﾞ)									*/
#define	CAN_ATGEAR_86B_MANUAL_4TH					0x04		/* ATギア：4TH(Mﾓｰﾄﾞ、ﾃﾝﾎﾟﾗﾘMﾓｰﾄﾞ)									*/
#define	CAN_ATGEAR_86B_MANUAL_5TH					0x05		/* ATギア：5TH(Mﾓｰﾄﾞ、ﾃﾝﾎﾟﾗﾘMﾓｰﾄﾞ)									*/
#define	CAN_ATGEAR_86B_MANUAL_6TH					0x06		/* ATギア：6TH(Mﾓｰﾄﾞ、ﾃﾝﾎﾟﾗﾘMﾓｰﾄﾞ)									*/
#define	CAN_ATGEAR_86B_DSPORT_1ST					0x07		/* ATギア：1ST(ｽﾎﾟｰﾂﾓｰﾄﾞ)											*/
#define	CAN_ATGEAR_86B_DSPORT_2ND					0x08		/* ATギア：2ND(ｽﾎﾟｰﾂﾓｰﾄﾞ)											*/
#define	CAN_ATGEAR_86B_DSPORT_3RD					0x09		/* ATギア：3RD(ｽﾎﾟｰﾂﾓｰﾄﾞ)											*/
#define	CAN_ATGEAR_86B_DSPORT_4TH					0x0A		/* ATギア：4TH(ｽﾎﾟｰﾂﾓｰﾄﾞ)											*/
#define	CAN_ATGEAR_86B_DSPORT_5TH					0x0B		/* ATギア：5TH(ｽﾎﾟｰﾂﾓｰﾄﾞ)											*/
#define	CAN_ATGEAR_86B_DSPORT_6TH					0x0C		/* ATギア：6TH(ｽﾎﾟｰﾂﾓｰﾄﾞ)											*/
#define	CAN_ATGEAR_86B_D							0x0F		/* ATギア：Dレンジ													*/
/* CAN信号:TCM変速制御モード */
#define	CAN_MODESHIFT_D_RANGE_AUTO					0x01		/*	TCM変速制御モード:1 Ｄレンジ自動変速モード						*/
#define	CAN_MODESHIFT_GEARFIXED_AUTORETURN			0x03		/*	TCM変速制御モード:3 ギア固定自動復帰モード（マニュアル）		*/
#define	CAN_MODESHIFT_GEARFIXED_MODE				0x05		/*	TCM変速制御モード:5 ギア固定モード（マニュアル）				*/
/* CAN信号AT/MTギア段数 */
#define	CAN_GEARPOS_NOT_DECISION					0x00		/*	ギア段数:不定													*/
#define	CAN_GEARPOS_1ST								0x01		/*	ギア段数:1ST													*/
#define	CAN_GEARPOS_2ND								0x02		/*	ギア段数:2ND													*/
#define	CAN_GEARPOS_3RD								0x03		/*	ギア段数:3RD													*/
#define	CAN_GEARPOS_4TH								0x04		/*	ギア段数:4TH													*/
#define	CAN_GEARPOS_5TH								0x05		/*	ギア段数:5TH													*/
#define	CAN_GEARPOS_6TH								0x06		/*	ギア段数:6TH													*/
#define	CAN_GEARPOS_7TH								0x07		/*	ギア段数:7TH													*/
#define	CAN_GEARPOS_8TH								0x08		/*	ギア段数:8TH													*/
#define	CAN_GEARPOS_9TH								0x09		/*	ギア段数:9TH													*/
#define	CAN_GEARPOS_10TH							0x0A		/*	ギア段数:10TH													*/
/* CAN信号：タイヤ空気圧 */
#define	CAN_TIREPRESS_STATUS_CHECKING				0x7F		/*	タイヤ空気圧チェック中状態										*/
#define	CAN_TIREPRESS_RESOLUTION_KPA				5			/*	タイヤ空気圧kPa分解能（演算後、分解能1）						*/
#define	CAN_TIREPRESS_RESOLUTION_PSI				1			/*	タイヤ空気圧psi分解能（演算後、分解能1）						*/
#define	CAN_TIREPRESS_RESOLUTION_BAR				0.1			/*	タイヤ空気圧bar分解能（演算後、分解能1）						*/
/* CAN信号：タイヤ空気圧単位 */
#define	CAN_TIREPRESS_UNIT_KPA						0			/*	タイヤ空気圧単位kPa												*/
#define	CAN_TIREPRESS_UNIT_PSI						1			/*	タイヤ空気圧単位psi												*/
#define	CAN_TIREPRESS_UNIT_BAR						2			/*	タイヤ空気圧単位bar												*/
#define	CAN_VEHICLEINFO_UNKNOWN						0			/*	車両種別：不明													*/
#define	CAN_VEHICLEINFO_086A						1			/*	車両種別：086A													*/
#define	CAN_VEHICLEINFO_086B						2			/*	車両種別：086B													*/
#define	CAN_MANUALMODE_SW_ON						1			/*	マニュアルモードSW：ON											*/
#define	CAN_MANUALMODE_SW_OFF						0			/*	マニュアルモードSW：OFF											*/
/*VSCOFF状態*/ /*CANの信号はOFFランプ点灯/消灯状態*/
#define CAN_VSC_OFF									1			/* VSCOFF（VSCOFFランプ点灯）										*/
#define CAN_VSC_ON									0			/* VSCON（VSCOFFランプ消灯）										*/
/*VSC制御状態*/
#define CAN_VSCON_ACTIVE							1			/* VSC制御状態：制御中												*/
#define CAN_VSCON_NOTACTIVE							0			/* VSC制御状態：制御外												*/
/*TRCOFF状態*/ /*CANの信号はOFFランプ点灯/消灯状態*/
#define CAN_TRC_OFF									1			/* TRCOFF（TRCOFFランプ点灯）										*/
#define CAN_TRC_ON									0			/* TRCON（TRCOFFランプ消灯）										*/
/*TRC制御状態*/
#define CAN_TRCON_ACTIVE							1			/* TRC制御状態：制御中												*/
#define CAN_TRCON_NOTACTIVE							0			/* TRC制御状態：制御外												*/
/*ABS制御状態*/
#define CAN_ABSON_ACTIVE							1			/* ABS制御状態：制御中												*/
#define CAN_ABSON_NOTACTIVE							0			/* ABS制御状態：制御外												*/
/*ABSシステムフェイル状態*/
#define CAN_ABS_SYSFAIL_OFF							1			/* ABSシステムフェイル正常											*/
#define CAN_ABS_SYSFAIL_ON							0			/* ABSシステムフェイル異常											*/
/*ABSウォーニング状態*/
#define CAN_ABS_WARNING_ON							1			/* ABSウォーニングON：（ウォーニングランプ点灯）					*/
#define CAN_ABS_WARNING_OFF							0			/* ABSウォーニングOFF：（ウォーニングランプ消灯）					*/
/*TRCウォーニング状態*/
#define CAN_TRC_WARNING_ON							1			/* TRCウォーニングON：（ウォーニングランプ点灯）					*/
#define CAN_TRC_WARNING_OFF							0			/* TRCウォーニングOFF：（ウォーニングランプ消灯）					*/
/*VSCウォーニング状態*/
#define CAN_VSC_WARNING_ON							1			/* VSCウォーニングON：（ウォーニングランプ点灯）					*/
#define CAN_VSC_WARNING_OFF							0			/* VSCウォーニングOFF：（ウォーニングランプ消灯）					*/
/*トラクション重視状態*/
#define CAN_TRC_MODE_ON								1			/* トラクション重視モード											*/
#define CAN_TRC_MODE_OFF							0			/* トラクション重視モード											*/
/*エンジントルク要求(アップダウン共通)*/
#define CAN_ENGTRQ_NO_REQUEST						0			/* エンジントルクアップダウン要求なし								*/
#define CAN_ENGTRQ_REQUEST							1			/* エンジントルクアップダウン要求あり								*/
/*エンジントルク（要求値と共通）*/
#define CAN_ENGTRQ_OFFSET							-1000		/* エンジントルクオフセット（要求値と共通）							*/
#define CAN_ENGTRQ_RESOLUTION						0.1			/* エンジントルク分解能（演算後、分解能1）（要求値と共通）			*/
#define CAN_ENGTRQ_OFFSET_INC_RESOLUTION			-10000		/* エンジントルクオフセット：分解能演算前（要求値と共通）			*/
#define CAN_ENGTRQ_REQ_MAXVAL						0xFFFF		/* エンジントルク要求最大値											*/
/*VSCアンダーオーバーステア作動中状態*/
#define CAN_VSC_CONTROL_STR_ON						1			/* VSCアンダー、オーバーステア制御中								*/
#define CAN_VSC_CONTROL_STR_OFF						0			/* VSCアンダー、オーバーステア制御中でない							*/
/*EBDウォーニング状態*/
#define CAN_EBD_WARNING_ON							1			/* EBDウォーニングON：（ウォーニングランプ点灯）					*/
#define CAN_EBD_WARNING_OFF							0			/* EBDウォーニングOFF：（ウォーニングランプ消灯）					*/
/* CAN信号:トランク開閉状態 */
#define CAN_TRUNK_SW_OPEN							0x01		/* トランクSW状態オープン											*/
#define CAN_TRUNK_SW_CLOSE							0x00		/* トランクSW状態クローズ											*/
/* CAN信号:フード開閉状態 */
#define CAN_HOOD_SW_OPEN							0x00		/* フードSW状態オープン												*/
#define CAN_HOOD_SW_CLOSE							0x01		/* フードSW状態クローズ												*/
/* CAN信号:クルーズコントロール */
#define CAN_CRUISE_ON								0x01		/* クルーズコントロールON											*/
#define CAN_CRUISE_OFF								0x00		/* クルーズコントロールOFF											*/
/* CAN信号:吸入管圧力 */
#define CAN_INTAKE_PRESS_RESOLUTION					15			/* 吸入管圧力分解能（演算語、分解能１）								*/
/* CAN信号:大気圧 */
#define CAN_ATM_PRESS_RESOLUTION					3			/* 大気圧 分解能（演算語、分解能１）								*/
#define CAN_ATM_PRESS_OFFSET						100			/* 大気圧 オフセット												*/
/* CAN信号:ウィンカー */
#define CAN_BLINKER_ON								1			/* ウィンカーON														*/
#define CAN_BLINKER_OFF								0			/* ウィンカーOFF													*/
/* CAN信号：目標ギア段数 */
#define	CAN_TARGET_GEAR_N							0x00		/* ATシフト:N														*/
#define	CAN_TARGET_GEAR_D							0x0D		/* ATシフト:D														*/
#define	CAN_TARGET_GEAR_R							0x0E		/* ATシフト:R														*/
#define	CAN_TARGET_GEAR_P							0x0F		/* ATシフト:P														*/
#define	CAN_TARGET_GEAR1ST							0x01		/* ATギア：1ST														*/
#define	CAN_TARGET_GEAR2ND							0x02		/* ATギア：2ND														*/
#define	CAN_TARGET_GEAR3RD							0x03		/* ATギア：3RD														*/
#define	CAN_TARGET_GEAR4TH							0x04		/* ATギア：4TH														*/
#define	CAN_TARGET_GEAR5TH							0x05		/* ATギア：5TH														*/
#define	CAN_TARGET_GEAR6TH							0x06		/* ATギア：6TH														*/

/* 始動後燃料噴射量積算値分解能 */
#define	CAN_FUEL_CONSUMPTION_RESOLUTION			0.001			/*	始動後燃料噴射量積算値分解能（演算後、分解能1）					*/
/* 始動後走行距離積算値分解能 */
#define	CAN_RUNNING_DISTANCE_RESOLUTION			0.01			/*	始動後走行距離積算値分解能（演算後、分解能1）					*/
/* 表示用車速分解能 */
#define	CAN_VEHICLESPEED_DISP_RESOLUTION		0.00001			/*	表示用車速分解能												*/
/* CAN信号:エンジン水温 */
#define	CAN_TEMP_COOLANT_MAX					0xFE			/* エンジン水温 最大値												*/
/* CAN信号:エンジン水温オフセット */
#define	CAN_ENG_TEMP_COOLANT_OFFSET				-40				/* エンジン水温オフセット											*/
/* CAN信号:エンジン水温オフセット */
#define	CAN_ENG_TEMP_OIL_OFFSET					-40				/* エンジン油温オフセット											*/
/* CAN信号:外気温最大値 */
#define	CAN_OUOTSIDE_TEMP_MAX					0xF0			/* 外気温最大値														*/
/* CAN信号:外気温分解能 */
#define	CAN_OUOTSIDE_TEMP_RESOLUTION			0.5				/* 外気温分解能														*/
/* CAN信号:外気温オフセット */
#define	CAN_OUTSIDE_TEMP_OFFSET					-40				/* 外気温オフセット													*/
/* ISOCAN信号:吸気温度オフセット */
#define	ISOCAN_INTAKE_MANIFOLD_TEMP_OFFSET		-40				/* 吸気温度オフセット												*/
/* ISOCAN信号:エンジン水温オフセット */
#define	ISOCAN_ENG_TEMP_COOLANT_OFFSET			-40				/* エンジン水温オフセット											*/


/* CAN検査CH	*/
typedef enum e_can_inspection_can_ch
{
	CAN_INSPECTION_CAN_CH0	= 0,
	CAN_INSPECTION_CAN_CH1
} E_CAN_INSPECTION_CAN_CH;


#define CAN_INSPECTION_RESULT_NG				(0x00)
#define CAN_INSPECTION_RESULT_OK				(0x01)

/* 検査コマンド */
#define CAN_INSPECTION_CMD_START				(0xF1)		/* 検査開始			 					*/
#define CAN_INSPECTION_CMD_STOP					(0xF2)		/* 検査停止			 					*/
#define CAN_INSPECTION_CMD_SERIAL_NO_WR			(0x10)		/* シリアルNo書き込み					*/
#define CAN_INSPECTION_CMD_SERIAL_NO_RD			(0x11)		/* シリアルNo読み出し					*/
#define CAN_INSPECTION_CMD_BT_PAIRING			(0x20)		/* Bluetoothペアリング					*/
#define CAN_INSPECTION_CMD_BT_CONNECT			(0x21)		/* Bluetooth通信確認					*/
#define CAN_INSPECTION_CMD_SD					(0x30)		/* SDカード検査							*/
#define CAN_INSPECTION_CMD_CAN0					(0x40)		/* CAN0確認								*/
#define CAN_INSPECTION_CMD_CAN1					(0x50)		/* CAN1確認								*/
#define CAN_INSPECTION_CMD_UART					(0x60)		/* UART確認								*/
#define CAN_INSPECTION_CMD_SW					(0x70)		/* SW確認								*/
#define CAN_INSPECTION_CMD_GNSS					(0x80)		/* GNSS検査								*/

/* 検査コマンド	*/
typedef struct t_can_inspection_cmd_info {
	UI_8		cmd;			/* コマンド(INSPEC_CMD_xxx)				*/
	UI_8		result;			/* 結果(CAN_INSPECTION_RESULT_xx)		*/
	UI_8		data1;			/* データ１(1byte)						*/
	UI_8		data2;			/* データ２(1byte)						*/
	UI_32		data3;			/* データ３(4byte)						*/
} T_Can_inspection_cmd_info;



/********************************************************/
/*	内部変数定義										*/
/********************************************************/

/********************************************************/
/*	外部関数											*/
/********************************************************/

/********************************************************/
/* 関数名称		: Can_control_Init						*/
/* 機能の説明	: CAN制御初期化							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Can_control_Init(void);

/********************************************************/
/* 関数名称		: Can_control_Main						*/
/* 機能の説明	: CAN制御メイン							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Can_control_Main(void);

/********************************************************/
/* 関数名称		: Can_control_RcvInspectionCmd			*/
/* 機能の説明	: 検査コマンド受信						*/
/* 引数			: (I/ )cmdinfo	検査コマンド			*/
/* 戻り値		: N_OK		コマンドを受信した			*/
/* 				: N_NG		コマンド未受信またはエラー	*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Can_control_RcvInspectionCmd(T_Can_inspection_cmd_info *cmdinfo);

/********************************************************/
/* 関数名称		: Can_control_SndInspectionCmd			*/
/* 機能の説明	: 検査コマンド送信						*/
/* 引数			: (I/ )cmdinfo	検査コマンド			*/
/* 戻り値		: N_OK		正常						*/
/* 				: N_NG		異常						*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Can_control_SndInspectionCmd(const T_Can_inspection_cmd_info *cmdinfo);

/********************************************************/
/* 関数名称		: Can_control_ReqInspection				*/
/* 機能の説明	: CAN検査								*/
/* 引数			: ch	CAN CH							*/
/* 戻り値		: N_OK		正常						*/
/* 				: N_NG		異常						*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Can_control_ReqInspection(E_CAN_INSPECTION_CAN_CH ch);

/********************************************************/
/* 関数名称		: Can_control_GetTCanInfo				*/
/* 機能の説明	: T-CAN情報カレント値を取得する			*/
/* 引数			: void									*/
/* 戻り値		: T-CAN情報カレント値格納領域ptr		*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/********************************************************/
T_Can_ctrl_tcan_info *Can_control_GetTCanInfo(void);

/********************************************************/
/* 関数名称		: Can_control_GetVehicleSpeed			*/
/* 機能の説明	: 車速カレント値を取得する				*/
/* 引数			: (O)vehicle_speed 車速カレント値		*/
/* 戻り値		: 取得結果								*/
/*				   E_RC_OK:取得成功, E_RC_NG:取得不可	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/*   返される車速は、0.00001km/hを1単位とする値。		*/
/********************************************************/
E_RC Can_control_GetVehicleSpeed(UI_32 *vehicle_speed);

/********************************************************/
/* 関数名称		: Can_control_GetIsoCanInfo				*/
/* 機能の説明	: ISO-CAN情報カレント値を取得する		*/
/* 引数			: void									*/
/* 戻り値		: ISO-CAN情報カレント値格納領域ptr		*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/11/08								*/
/* 備考			: 										*/
/********************************************************/
T_Can_ctrl_iso_can_info *Can_control_GetIsoCanInfo(void);
#endif	/*DTO459_VDV_CAN_CTRL_H*/
