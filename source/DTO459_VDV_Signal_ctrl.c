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
/* file DTO459_VDV_Signal_cgtrl.c						*/
/* 公開信号制御											*/
/********************************************************/

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include "system.h"
#include <stdlib.h> /* 乱数関数を利用するのに必要 */
#include <string.h>
#include "System_control_TASK.h"
#include "DTO459_VDV_Signal_ctrl.h"
#ifdef ADVANCE_ENABLE
#include "DTO459_VDV_Advance_ctrl.h"
#endif /*ADVANCE_ENABLE*/
#include "DTO459_VDV_Can_ctrl.h"
#include "DTO459_VDV_Gnss_ctrl.h"
#include "DTO459_VDV_Sd_ctrl.h"
#include "timer_ctrl.h"
#include "DTO459_VDV_Sw_ctrl.h"

/********************************************************/
/*	マクロ定義											*/
/********************************************************/
#define SIGNAL_LOCAL	static

#define SIGNAL_VERSION	0x00010000UL		/*公開信号リストバージョン*/
/*                          ^^^^^^
                            | | |
                            | | |
                            | | メンテナスバージョン
                            | マイナーバージョン
                            メジャーバージョン
*/

/* 信号値:共通定義 */
#define	SIGNAL_INFO_FUNC_NUM			(UI_16)700			/* 信号関数の数 */
#define	SIGNAL_ID_MIN					(UI_16)1			/* 信号ID最小値（現状） */
#define	SIGNAL_ID_MAX					(UI_16)699			/* 信号ID最大値（現状） */
#define	SIGNAL_VALUE_IGNORE_32			0xFFFFFFFF			/* 信号無効値 */
#define	SIGNAL_VALUE_IGNORE_32_SIGNED	(UI_32)0x7FFFFFFF	/* 信号無効値 */
#define	SIGNAL_VALUE_IGNORE_16			(UI_16)0xFFFF		/* 信号無効値 */
#define	SIGNAL_VALUE_IGNORE_16_SIGNED	(UI_16)0x7FFF		/* 信号無効値 */
#define	SIGNAL_VALUE_IGNORE_12			(UI_16)0x0FFF		/* 信号無効値 */
#define	SIGNAL_VALUE_IGNORE_8			(UI_8)0xFF			/* 信号無効値 */
#define	SIGNAL_VALUE_IGNORE_8_SIGNED	(UI_8)0x7F			/* 信号無効値 */
#define	SIGNAL_VALUE_IGNORE_4			(UI_8)0x0F			/* 信号無効値 */


/* メーカーID */
#define	SIGNAL_VALUE_MAKERID_INIT			0x00		/*	メーカーID（000）: Initial or Other								*/
#define	SIGNAL_VALUE_MAKERID_TOYOTA			0x01		/*	メーカーID（001）: TOYOTA										*/
#define	SIGNAL_VALUE_MAKERID_SIMULATOR		0xF4		/*	メーカーID（244）: Simulator									*/
#define	SIGNAL_VALUE_MAKERID_IGNORE			0xFF		/*	メーカーID（255）: Ignore										*/
/* モデル番号 */
#define	SIGNAL_VALUE_MODEL_NUMBER			0x01		/*	モデル番号: ECU_Model_Number									*/
/* 有効フラグ */
#define	SIGNAL_VALUE_VALIDFLAG_INIT			0x00		/*	有効フラグ:000: Initial											*/
#define	SIGNAL_VALUE_VALIDFLAG_VALID		0x01		/*	有効フラグ:001: Valid											*/
#define	SIGNAL_VALUE_VALIDFLAG_INVALID		0x02		/*	有効フラグ:002: Invalid											*/
#define	SIGNAL_VALUE_VALIDFLAG_IGNORE		0xFF		/*	有効フラグ:255: Ignore											*/

/*	CAN-Gateway ECU ID	*/
#define	SIGNAL_VALUE_CANGATEWAYECUID		( (SIGNAL_VALUE_MODEL_NUMBER 					<< 8	)	|\
											  (SIGNAL_VALUE_MAKERID_TOYOTA							)	)

/* 車両名称 */
#define	SIGNAL_VALUE_VEHICLENAMEHIGHORDER			"86  "	/*	車両名称	Vehicle Name (High-Order)							*/
#define	SIGNAL_VALUE_VEHICLENAMELOWORDER    		"    " 	/*	車両名称	Vehicle Name (Low-Order)							*/
/* 仕向地 */
#define	SIGNAL_VALUE_DESTINATION		    "    "  	/*	仕向け地(空白）	Destination										*/
/* ドライバーポジション */
#define	SIGNAL_VALUE_DRIVERPOSITION_INIT	0x00		/*	ドライバーポジション(初期値)									*/
#define	SIGNAL_VALUE_DRIVERPOSITION_RIGHT	0x02		/*	ドライバーポジション(右)										*/
#define	SIGNAL_VALUE_DRIVERPOSITION_CENTER	0x04		/*	ドライバーポジション(中)										*/
#define	SIGNAL_VALUE_DRIVERPOSITION_LEFT	0x08		/*	ドライバーポジション(左)										*/
#define	SIGNAL_VALUE_DRIVERPOSITION_IGNORE	0x0F		/*	ドライバーポジション(無効値)									*/
/* エンジン型式 */
#define	SIGNAL_VALUE_ENGINEMODELHIGHORDE_86A     "FA20"	 	/*	エンジン型式(先頭4文字)	Engine Model Code (High-Order)			*/
#define	SIGNAL_VALUE_ENGINEMODELHIGHORDE_86B     "FA24"	 	/*	エンジン型式(先頭4文字)	Engine Model Code (High-Order)			*/
#define	SIGNAL_VALUE_ENGINEMODELHIGHORDE_UNKNOWN "    "	 	/*	エンジン型式(先頭4文字)	Engine Model Code (High-Order)			*/
#define	SIGNAL_VALUE_ENGINEMODELLOWORDER    "    "  	/*	エンジン型式(後方4文字)	Engine Model Code (Low-Order)			*/
/* 燃料種別 */
#define	SIGNAL_VALUE_FUELTYPE_GASOLINE		0x01 		/*	燃料種別:Gasoline												*/
#define	SIGNAL_VALUE_FUELTYPE_DIESEL		0x02 		/*	燃料種別:Diesel													*/
#define	SIGNAL_VALUE_FUELTYPE_ALCOHOL		0x04 		/*	燃料種別:Alcohol												*/
#define	SIGNAL_VALUE_FUELTYPE_ELECTRICAL	0x08 		/*	燃料種別:Electrical												*/
#define	SIGNAL_VALUE_FUELTYPE_HYDROGEN		0x10 		/*	燃料種別:Hydrogen												*/
#define	SIGNAL_VALUE_FUELTYPE_HYBRID		0x80 		/*	燃料種別:Hybrid													*/
/* エンジン方式 */
#define	SIGNAL_VALUE_ENGINETYPE_4STROKE		0x01		/*	エンジン特性	4 Stroke Reciprocating Engine 					*/
#define	SIGNAL_VALUE_ENGINETYPE_2STROKE		0x02		/*	エンジン特性	2 Stroke Reciprocating Engine					*/
#define	SIGNAL_VALUE_ENGINETYPE_ROTARY		0x04		/*	エンジン特性	Rotary Engine									*/
#define	SIGNAL_VALUE_ENGINETYPE_ELECTRICAL	0x08		/*	エンジン特性	Electrical Motor								*/
#define	SIGNAL_VALUE_ENGINETYPE_INWHEEL		0x10		/*	エンジン特性	In-wheel Motor									*/
#define	SIGNAL_VALUE_ENGINETYPE_FUELCELL	0x20		/*	エンジン特性	Fuel Cell Motor									*/
#define	SIGNAL_VALUE_ENGINETYPE_OTHER		0x40		/*	エンジン特性	Other Type Engine								*/
/* 気筒数 */
#define	SIGNAL_VALUE_NUMBEROFCYLINDERS		0x04		/*	気筒数															*/
/* 吸気方式 */
#define	SIGNAL_VALUE_INTAKESYSTEM_NATURALASPIRATION		0x01	/*	吸気方式: Natural Aspiration							*/
#define	SIGNAL_VALUE_INTAKESYSTEM_TURBOCHARGER			0x02	/*	吸気方式: Turbocharger									*/
#define	SIGNAL_VALUE_INTAKESYSTEM_SUPERCHARGER			0x03	/*	吸気方式: Supercharger									*/
#define	SIGNAL_VALUE_INTAKESYSTEM_ELECTRICTURBOCHARGER	0x04	/*	吸気方式: Electric Turbocharger							*/
/*	エンジン特性	Engine Character	*/
#define	SIGNAL_VALUE_ENGINECHARACTER		( ((UI_32)SIGNAL_VALUE_INTAKESYSTEM_NATURALASPIRATION	<< 20	)	|\
											  ((UI_32)SIGNAL_VALUE_NUMBEROFCYLINDERS  				<< 16	)	|\
											  ((UI_32)SIGNAL_VALUE_ENGINETYPE_4STROKE 				<< 8	)	|\
											  ((UI_32)SIGNAL_VALUE_FUELTYPE_GASOLINE						)	)

/* トランスミッション種別 */
#define	SIGNAL_VALUE_TRANSMISSION_INITIAL			0x00	/*	トランスミッション種別 00: Initial							*/
#define	SIGNAL_VALUE_TRANSMISSION_MANUAL			0x01	/*	トランスミッション種別 01: Manual							*/
#define	SIGNAL_VALUE_TRANSMISSION_STEPAUTOMATIC		0x02	/*	トランスミッション種別 02: Step Automatic					*/
#define	SIGNAL_VALUE_TRANSMISSION_CVT				0x03	/*	トランスミッション種別 03: CVT								*/
#define	SIGNAL_VALUE_TRANSMISSION_ELECTRIC_CVT		0x04	/*	トランスミッション種別 04: Electric CVT						*/
#define	SIGNAL_VALUE_TRANSMISSION_MOTOR				0x05	/*	トランスミッション種別 05: Motor							*/
#define	SIGNAL_VALUE_TRANSMISSION_IGNORE			0x0F	/*	トランスミッション種別 15: Ignore							*/
/* 駆動輪 */
#define	SIGNAL_VALUE_DRIVEWHEEL_INITIAL				0x00	/*	駆動輪 00: Initial											*/
#define	SIGNAL_VALUE_DRIVEWHEEL_FRONT				0x01	/*	駆動輪 01: Front Drive										*/
#define	SIGNAL_VALUE_DRIVEWHEEL_REAR				0x02	/*	駆動輪 02: Rear Drive										*/
#define	SIGNAL_VALUE_DRIVEWHEEL_ALLWHEEL_FULLTIME	0x03	/*	駆動輪 03: All Wheel Drive(Full Time)						*/
#define	SIGNAL_VALUE_DRIVEWHEEL_ALLWHEEL_PARTTIME	0x04	/*	駆動輪 04: All Wheel Drive(Part Time)						*/
#define	SIGNAL_VALUE_DRIVEWHEEL_IGNORE				0x0F	/*	駆動輪 15: Ignore											*/
/* ギア段数 */
#define	SIGNAL_VALUE_NUMBEROFTHESTEPS				0x06	/* ギア段数														*/
/* タイヤ動荷重半径 */
#define	SIGNAL_VALUE_DYNAMICXLOADRADIUSOFTIRE		0xFFF	/* タイヤ動荷重半径:無効										*/
/*	ドライブトレーン	Drive Train Character						*/
#define	SIGNAL_VALUE_DRIVETRAINCHARACTER	( ((UI_32)SIGNAL_VALUE_DYNAMICXLOADRADIUSOFTIRE		<< 12	)	|\
											  ((UI_32)SIGNAL_VALUE_NUMBEROFTHESTEPS  			<< 8	)	|\
											  ((UI_32)SIGNAL_VALUE_DRIVEWHEEL_REAR 				<< 4	)	)
/* ドアSW状態 */
#define SIGNAL_VALUE_DOOR_CLOSE						0x00	/* ドアSW状態	(0000)2: Initial or Close						*/
#define SIGNAL_VALUE_DOOR_RIGHT_OPEN				0x02	/* ドアSW状態	(--*-)2: Right  1-Open, 0-Close					*/
#define SIGNAL_VALUE_DOOR_LEFT_OPEN					0x04	/* ドアSW状態	(-*--)2: Left  1-Open, 0-Close					*/
#define SIGNAL_VALUE_DOOR_IGNORE					0x0F	/* ドアSW状態	(1111)2: Ignore									*/
#define SIGNAL_VALUE_DOOR_OPEN						0x00	/* ドアロック状態	(0000)2: Initial or Open					*/
#define SIGNAL_VALUE_DOOR_RIGHT_LOCK				0x02	/* ドアロック状態	(--*-)2: Right  1-Rock, 0-UnRock			*/
#define SIGNAL_VALUE_DOOR_LEFT_LOCK					0x04	/* ドアロック状態	(-*--)2: Left  1-Rock, 0-UnRock				*/

/* シートベルト状態 */
#define SIGNAL_VALUE_SEATBELT_INITIAL				0x00	/* シートベルト状態	(0000)2: Initial							*/
#define SIGNAL_VALUE_SEATBELT_NOONELOCKED			0x01	/* シートベルト状態	(0001)2: No one Locked						*/
#define SIGNAL_VALUE_SEATBELT_RIGHT_LOCKED			0x02	/* シートベルト状態	(--*-)2: Right  1-Locked, 0-Unlocked		*/
#define SIGNAL_VALUE_SEATBELT_CENTER_LOCKED			0x04	/* シートベルト状態	(-*--)2: Center  1-Locked, 0-Unlocked		*/
#define SIGNAL_VALUE_SEATBELT_LEFT_LOCKED			0x08	/* シートベルト状態	(*---)2: Left  1-Locked, 0-Unlocked			*/
#define SIGNAL_VALUE_SEATBELT_IGNORE				0x0F	/* シートベルト状態	(1111)2: Ignore								*/
/* アクセル特性選択状態 */
#define	SIGNAL_VALUE_ACCEL_CHARACTER_INITIAL		0x00	/* アクセル特性選択状態 Initial									*/
#define	SIGNAL_VALUE_ACCEL_CHARACTER_NORMAL			0x01	/* アクセル特性選択状態 Normal									*/
#define	SIGNAL_VALUE_ACCEL_CHARACTER_ECO			0x02	/* アクセル特性選択状態 ECO										*/
#define	SIGNAL_VALUE_ACCEL_CHARACTER_SNOW			0x03	/* アクセル特性選択状態 Snow									*/
#define	SIGNAL_VALUE_ACCEL_CHARACTER_POWER			0x04	/* アクセル特性選択状態 Power									*/
#define	SIGNAL_VALUE_ACCEL_CHARACTER_SPORTS			0x05	/* アクセル特性選択状態 Sports									*/
#define	SIGNAL_VALUE_ACCEL_CHARACTER_SPECIAL		0x06	/* アクセル特性選択状態 Special									*/
#define	SIGNAL_VALUE_ACCEL_CHARACTER_IGNORE			0x0F	/* アクセル特性選択状態 Ignore									*/

/* シート設定(1列目) */
#define	SIGNAL_VALUE_SEATSETTING_FIRSTROW_RIGHT		0x02	/*	シート設定(1列目) (**10)2: Right  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_FIRSTROW_CENTER	0x04	/*	シート設定(1列目) (*1**)2: Center  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_FIRSTROW_LEFT		0x08	/*	シート設定(1列目) (1***)2: Left  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_FIRSTROW_IGNORE	0x0F	/*	シート設定(1列目) (1111)2: Ignore							*/
/* シート設定(2列目) */
#define	SIGNAL_VALUE_SEATSETTING_SECONDROW_RIGHT	0x02	/*	シート設定(2列目) (**10)2: Right  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_SECONDROW_CENTER	0x04	/*	シート設定(2列目) (*1**)2: Center  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_SECONDROW_LEFT		0x08	/*	シート設定(2列目) (1***)2: Left  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_SECONDROW_IGNORE	0x0F	/*	シート設定(2列目) (1111)2: Ignore							*/
/* シート設定(3列目) */
#define	SIGNAL_VALUE_SEATSETTING_THIRDROW_RIGHT		0x02	/*	シート設定(3列目) (**10)2: Right  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_THIRDROW_CENTER	0x04	/*	シート設定(3列目) (*1**)2: Center  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_THIRDROW_LEFT		0x08	/*	シート設定(3列目) (1***)2: Left  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_THIRDROW_IGNORE	0x0F	/*	シート設定(3列目) (1111)2: Ignore							*/
/*	シート設定	Seat Setting										*/
#define	SIGNAL_VALUE_SEATSETTING			( ((SIGNAL_VALUE_SEATSETTING_THIRDROW_IGNORE)											<< 8)	|\
											  ((SIGNAL_VALUE_SEATSETTING_SECONDROW_RIGHT | SIGNAL_VALUE_SEATSETTING_SECONDROW_LEFT) << 4)	|\
											  ((SIGNAL_VALUE_SEATSETTING_FIRSTROW_RIGHT  | SIGNAL_VALUE_SEATSETTING_FIRSTROW_LEFT) 		)	)




/* アクセル特性 */
#define	SIGNAL_VALUE_ACCELCHARACTER_INITIAL	0x00		/*	アクセル特性:Initial											*/
#define	SIGNAL_VALUE_ACCELCHARACTER_NORMAL	0x01		/*	アクセル特性:Normal												*/
#define	SIGNAL_VALUE_ACCELCHARACTER_ECO		0x02		/*	アクセル特性:ECO												*/
#define	SIGNAL_VALUE_ACCELCHARACTER_SNOW	0x03		/*	アクセル特性:Snow												*/
#define	SIGNAL_VALUE_ACCELCHARACTER_POWER	0x04		/*	アクセル特性:Power												*/
#define	SIGNAL_VALUE_ACCELCHARACTER_SPORTS	0x05		/*	アクセル特性:Sports												*/
#define	SIGNAL_VALUE_ACCELCHARACTER_SPECIAL	0x06		/*	アクセル特性:Special											*/
#define	SIGNAL_VALUE_ACCELCHARACTER_IGNORE	0x15		/*	アクセル特性:Ignore												*/
/* ブレーキランプ */
#define	SIGNAL_VALUE_BRAKELAMP_INIT			0x00		/*	ブレーキランプ初期値											*/
#define	SIGNAL_VALUE_BRAKELAMP_OFF			0x01		/*	ブレーキランプOFF値												*/
#define	SIGNAL_VALUE_BRAKELAMP_ON			0x02		/*	ブレーキランプON値												*/
#define	SIGNAL_VALUE_BRAKELAMP_IGNORE		0x0F		/*	ブレーキランプ無視値											*/
/* パーキングブレーキ操作状態 */
#define	SIGNAL_VALUE_PARKINGBRAKE_INIT		0x00		/*	パーキングブレーキ初期値										*/
#define	SIGNAL_VALUE_PARKINGBRAKE_OFF		0x01		/*	パーキングブレーキOFF値											*/
#define	SIGNAL_VALUE_PARKINGBRAKE_ON		0x02		/*	パーキングブレーキON値											*/
#define	SIGNAL_VALUE_PARKINGBRAKE_IGNORE	0x0F		/*	パーキングブレーキ無視値										*/
/* ATシフト状態 */
#define	SIGNAL_VALUE_ATSHIFT_INIT			0x00		/*	ATシフト状態(00000000)2:Initial									*/
#define	SIGNAL_VALUE_ATSHIFT_P				0x01		/*	ATシフト状態(00000001)2:P (Parking) 							*/
#define	SIGNAL_VALUE_ATSHIFT_R				0x02		/*	ATシフト状態(00000010)2:R (Reverse) 							*/
#define	SIGNAL_VALUE_ATSHIFT_N				0x04		/*	ATシフト状態(00000100)2:N (Neutral)								*/
#define	SIGNAL_VALUE_ATSHIFT_D				0x08		/*	ATシフト状態(00001000)2:D (Drive)								*/
#define	SIGNAL_VALUE_ATSHIFT_B				0x18		/*	ATシフト状態(00011000)2:B (Engine Break Mode)					*/
#define	SIGNAL_VALUE_ATSHIFT_M				0x20		/*	ATシフト状態(00100000)2:M (Manual mode)							*/
#define	SIGNAL_VALUE_ATSHIFT_DM				0x28		/*	ATシフト状態(00101000)2:DM (Temporary Paddle Control Mode)		*/
#define	SIGNAL_VALUE_ATSHIFT_DS				0x48		/*	ATシフト状態(01001000)2:DS (Sporty Manner Drive Mode)			*/
#define	SIGNAL_VALUE_ATSHIFT_IGNORE			0xFF		/*	ATシフト状態(11111111)2:Ignore or MT(Manual Transmission) 		*/
/* ギア段数 */
#define	SIGNAL_VALUE_GEARPOS_INIT			0x00		/*	ギア段数(00000000)2:Initial or not decision						*/
#define	SIGNAL_VALUE_GEARPOS_1ST			0x01		/*	ギア段数:1ST													*/
#define	SIGNAL_VALUE_GEARPOS_2ND			0x02		/*	ギア段数:2ND													*/
#define	SIGNAL_VALUE_GEARPOS_3RD			0x03		/*	ギア段数:3RD													*/
#define	SIGNAL_VALUE_GEARPOS_4TH			0x04		/*	ギア段数:4TH													*/
#define	SIGNAL_VALUE_GEARPOS_5TH			0x05		/*	ギア段数:5TH													*/
#define	SIGNAL_VALUE_GEARPOS_6TH			0x06		/*	ギア段数:6TH													*/
#define	SIGNAL_VALUE_GEARPOS_7TH			0x07		/*	ギア段数:7TH													*/
#define	SIGNAL_VALUE_GEARPOS_8TH			0x08		/*	ギア段数:8TH													*/
#define	SIGNAL_VALUE_GEARPOS_9TH			0x09		/*	ギア段数:9TH													*/
#define	SIGNAL_VALUE_GEARPOS_10TH			0x0A		/*	ギア段数:10TH													*/
#define	SIGNAL_VALUE_GEARPOS_REVERSE		0x80		/*	ギア段数(1000 0000)2:Reverse									*/
#define	SIGNAL_VALUE_GEARPOS_IGNORE			0xFF		/*	ギア段数(1111 1111)2:Ignore										*/
/* ヘッドライト点灯信号 */
#define	SIGNAL_VALUE_HEADLIGHT_INITIAL		0x00		/*	ヘッドライト(0000)2: Initial									*/
#define	SIGNAL_VALUE_HEADLIGHT_OFF			0x01		/*	ヘッドライト(0001)2: Off										*/
#define	SIGNAL_VALUE_HEADLIGHT_SMALL		0x02		/*	ヘッドライト(--10)2: Parking Lamp On(small)						*/
#define	SIGNAL_VALUE_HEADLIGHT_LO			0x04		/*	ヘッドライト(-1--)2: Lo Beam 1-On, 0-Off						*/
#define	SIGNAL_VALUE_HEADLIGHT_HIGHANDLO	0x0C		/*	ヘッドライト(11--)2: High and Lo Beam 1-On, 0-Off				*/
#define	SIGNAL_VALUE_HEADLIGHT_IGNORE		0x0F		/*	ヘッドライト(1111)2: Ignore										*/
/* 起動状態 */
#define	SIGNAL_VALUE_POWER_INITIAL			0x00		/*	起動状態: (0000 0000)2: Initial									*/
#define	SIGNAL_VALUE_POWER_PLUS_B			0x01		/*	起動状態: (0*** ***1)2: +B										*/
#define	SIGNAL_VALUE_POWER_ACC				0x02		/*	起動状態: (0*** **1*)2: ACC										*/
#define	SIGNAL_VALUE_POWER_IG				0x04		/*	起動状態: (0*** *1**)2: IG										*/
#define	SIGNAL_VALUE_POWER_READY			0x08		/*	起動状態: (0*** 1***)2: READY									*/
#define	SIGNAL_VALUE_POWER_CHANGING 		0x10		/*	起動状態: (0**1 ****)2: Changing								*/
#define	SIGNAL_VALUE_POWER_IGNORE			0xFF		/*	起動状態: (1111 1111)2: Ignore									*/
/* 車速（方向） */
#define	SIGNAL_VALUE_DIRECTION_INITIAL		0x00		/*	方向(0000)2: Initial											*/	
#define	SIGNAL_VALUE_DIRECTION_STOP			0x01		/*	方向(0001)2: Stop												*/
#define	SIGNAL_VALUE_DIRECTION_FORWARD		0x02		/*	方向(0010)2: Forward											*/
#define	SIGNAL_VALUE_DIRECTION_REVERSE		0x04		/*	方向(0100)2: Reverse											*/
#define	SIGNAL_VALUE_DIRECTION_IGNORE		0x0F		/*	方向(1111)2: Ignore												*/
/* ヨーレイト出力分解能 */
#define	SIGNAL_VALUE_YAWRATE_RESOLUTION		0.0005		/* ヨーレイト出力分解能											*/
/* GNSS信号 */
#define	SIGNAL_VALUE_UTC_RESOLUTION			0.001		/*	UTC時間分解能												*/
#define	SIGNAL_VALUE_LATITUDE_RESOLUTION	0.0000001	/*	緯度分解能													*/
#define	SIGNAL_VALUE_LONGTUDE_RESOLUTION	0.0000001	/*	経度分解能													*/
#define	SIGNAL_VALUE_ALTITUDE_RESOLUTION	0.001		/*	高度分解能													*/
#define	SIGNAL_VALUE_GEOID_RESOLUTION		0.001		/*	ジオイド高分解能											*/

#define	SIGNAL_VALUE_VEHICLESPEED_RESOLUTION		0.01		/*	水平方向速度分解能									*/
#define	SIGNAL_VALUE_HEADING_VEHICLE_RESOLUTION		0.01		/*	水平方向移動方向									*/
#define	SIGNAL_VALUE_VDOP_RESOLUTION				0.1			/*	VDOP出力分解能										*/
#define	SIGNAL_VALUE_HDOP_RESOLUTION				0.1			/*	HDOP出力分解能										*/
#define	SIGNAL_VALUE_PDOP_RESOLUTION				0.1			/*	PDOP出力分解能										*/
#define	SIGNAL_VALUE_ACCURACY_RESOLUTION			0.01		/*	精度出力分解能										*/

#define	SIGNAL_VALUE_TIREPRESS_RESOLUTION		0.1			/* タイヤ空気圧分解能										*/

/* ABS状態 */
#define	SIGNAL_VALUE_ABS_INIT						0x00	/* VSC状態：初期値												*/
#define	SIGNAL_VALUE_ABS_OFF						0x01	/* VSC状態：OFF（未使用）										*/
#define	SIGNAL_VALUE_ABS_SET						0x02	/* VSC状態：有効												*/
#define	SIGNAL_VALUE_ABS_SET_AND_ACTIVE				0x06	/* VSC状態：有効＆動作											*/
#define	SIGNAL_VALUE_ABS_IGNORE						0x0F	/* VSC状態：無効												*/
/* VSC状態 */
#define	SIGNAL_VALUE_VSC_INIT_OR_NORMAL				0x00	/* VSC状態：初期値 or ノーマル									*/
#define	SIGNAL_VALUE_VSC_SET_TRC_OFF				0x01	/* VSC状態：TRC OFF												*/
#define	SIGNAL_VALUE_VSC_SET_VSC_OFF				0x02	/* VSC状態：VSC OFF												*/
#define	SIGNAL_VALUE_VSC_SET_TRC_VSC_OFF			0x03	/* VSC状態：TRC & VSC OFF										*/
#define	SIGNAL_VALUE_VSC_SET_SPECIAL				0x04	/* VSC状態：Special(TRACK or EXPERT)							*/
#define	SIGNAL_VALUE_VSC_IGNORE						0xFF	/* VSC状態：無効												*/
#define	SIGNAL_VALUE_VSC_TRC_ACTIVE_BIT				0x08	/* VSC状態：TRC制御中											*/
#define	SIGNAL_VALUE_VSC_VSC_ACTIVE_BIT				0x10	/* VSC状態：VSC制御中											*/
#define	SIGNAL_VALUE_VSC_NOTACTIVE					0x00	/* VSC状態：VSC/TRC非制御										*/

#define	SIGNAL_VALUE_NONACTIVE						0x00	/* ABS/VSC/TRC状態：非動作										*/
#define	SIGNAL_VALUE_ACTIVE							0x01	/* ABS/VSC/TRC状態：動作										*/
#define	SIGNAL_VALUE_ENGTRQ_NO_REQUEST				0x00	/* エンジントルクアップダウン要求なし							*/
#define	SIGNAL_VALUE_ENGTRQ_REQUEST					0x01	/* エンジントルクアップダウン要求あり							*/
#define SIGNAL_CONV_MMHG_TO_KPA 					0.13332	/*	単位変換	1mmHg = 0.13332kPa								*/
#define SIGNAL_VALUE_INTAKE_PRESS_RESOLUTION 		0.1		/*	吸入管圧力　分解能											*/
#define SIGNAL_VALUE_INTAKE_PRESS_OFFSET 			-100	/*	吸入管圧力　オフセット										*/
#define SIGNAL_VALUE_ATM_PRESS_RESOLUTION 			0.01	/*	大気圧　分解能												*/
#define	SIGNAL_VALUE_TARGET_GEAR_INIT				0x00	/*	目標ギア段数:初期値											*/
#define	SIGNAL_VALUE_TARGET_GEAR_1ST				0x01	/*	目標ギア段数:1ST											*/
#define	SIGNAL_VALUE_TARGET_GEAR_2ND				0x02	/*	目標ギア段数:2ND											*/
#define	SIGNAL_VALUE_TARGET_GEAR_3RD				0x03	/*	目標ギア段数:3RD											*/
#define	SIGNAL_VALUE_TARGET_GEAR_4TH				0x04	/*	目標ギア段数:4TH											*/
#define	SIGNAL_VALUE_TARGET_GEAR_5TH				0x05	/*	目標ギア段数:5TH											*/
#define	SIGNAL_VALUE_TARGET_GEAR_6TH				0x06	/*	目標ギア段数:6TH											*/
#define	SIGNAL_VALUE_TARGET_GEAR_P					0x10	/*	目標ギア：シフト状態(00000001)2:P (Parking) 				*/
#define	SIGNAL_VALUE_TARGET_GEAR_R					0x20	/*	目標ギア：シフト状態(00000010)2:R (Reverse) 				*/
#define	SIGNAL_VALUE_TARGET_GEAR_N					0x40	/*	目標ギア：シフト状態(00000100)2:N (Neutral)					*/
#define	SIGNAL_VALUE_TARGET_GEAR_D					0x80	/*	目標ギア：シフト状態(00001000)2:D (Drive)					*/
#define	SIGNAL_VALUE_TARGET_GEAR_INVALID			0xFF	/*	目標ギア：無効												*/
#define	SIGNAL_VALUE_BLINKER_INITIAL				0x00	/*	ウィンカー状態: (0000)2: Initial							*/
#define	SIGNAL_VALUE_BLINKER_OFF					0x01	/*	ウィンカー状態: (0001)2: Off								*/
#define	SIGNAL_VALUE_BLINKER_TURN_RIGHT				0x02	/*	ウィンカー状態: (0010)2: Right 								*/
#define	SIGNAL_VALUE_BLINKER_TURN_LEFT				0x04	/*	ウィンカー状態: (0100)2: Left								*/
#define	SIGNAL_VALUE_BLINKER_INVALID				0x0F	/*	ウィンカー状態: (1111)2: Invalid							*/
#define	SIGNAL_VALUE_HODD_INITIAL_OR_CLOSE			0x00	/*	フード状態: (0000)2: Initial or Close						*/
#define	SIGNAL_VALUE_HODD_FRONT_OPEN				0x02	/*	フード状態: (0010)2: Front									*/
#define	SIGNAL_VALUE_HODD_REAR_OPEN					0x04	/*	フード状態: (0100)2: Rear 									*/

/* GPS精度 */
#define	SIGNAL_VALUE_GSPFIX_NOFIX						0 		/* GPS Fix Type: 0 :利用できない、無効 													*/
#define	SIGNAL_VALUE_GSPFIX_GPSFIX						1 		/* GPS Fix Type: 1 : GPS 測位 															*/
#define	SIGNAL_VALUE_GSPFIX_DGPSFIX						2 		/* GPS Fix Type: 2 : DGPS 測位															*/
#define	SIGNAL_VALUE_GSPFIX_GPS PPS						3 		/* GPS Fix Type: 3 : GPS-PPS 															*/
#define	SIGNAL_VALUE_GSPFIX_REALTIMEKINEMATIC			4 		/* GPS Fix Type: 4 : Real Time Kinematic. System used in RTK mode with fixed integers 	*/
#define	SIGNAL_VALUE_GSPFIX_FLOATRTK					5 		/* GPS Fix Type: 5 : Float RTK. Satellite system used in RTK mode, floating integers 	*/
#define	SIGNAL_VALUE_GSPFIX_DEADRECKONIG_MODE			6 		/* GPS Fix Type: 6 : Estimated (dead reckoning) mode 									*/
#define	SIGNAL_VALUE_GSPFIX_MANUALINPUTMODE				7 		/* GPS Fix Type: 7 :マニュアル入力モード 												*/
#define	SIGNAL_VALUE_GSPFIX_SIMULATORMODE				8 		/* GPS Fix Type: 8 :シミュレーションモード												*/


/* 始動後燃料噴射量積算値出力分解能 */
#define	SIGNAL_VALUE_FUEL_CONSUMPTION_RESOLUTION	0.0001	/* 始動後燃料噴射量積算値分解能								*/
/* 始動後走行距離積算値出力分解能 */
#define	SIGNAL_VALUE_RUNNING_DISTANCE_RESOLUTION	0.01	/* 始動後走行距離積算値分解能								*/
/* 表示用車速出力分解能 */
#define	SIGNAL_VALUE_VEHICLESPEED_DISP_RESOLUTION	0.01	/*	表示用車速出力分解能									*/
/* ４輪車速出力分解能 */
#define	SIGNAL_VALUE_WHEELSPEED_RESOLUTION		0.01		/* ４輪車速出力分解能										*/
/* ４輪車速出力オフセット */
#define	SIGNAL_VALUE_WHEELSPEED_OFFSET			-100		/* ４輪車速出力オフセット									*/
/* 加速度出力分解能 */
#define	SIGNAL_VALUE_ACCERERATION_RESOLUTION	0.01		/* 加速度出力分解能											*/
/* エンジン水温出力オフセット */
#define	SIGNAL_VALUE_ENG_TEMP_COOLANT_OFFSET	-50			/* エンジン水温出力オフセット								*/
/* エンジン油温出力オフセット */
#define	SIGNAL_VALUE_ENG_TEMP_OIL_OFFSET		-50			/* エンジン油温出力オフセット								*/
/* 外気温出力オフセット */
#define	SIGNAL_VALUE_OUTSIDE_TEMP_OFFSET		-60			/* 外気温出力オフセット										*/
/* 吸気温度出力オフセット */
#define	SIGNAL_VALUE_INTAKE_MANIFOLD_TEMP_OFFSET_ISOCAN	-40	/* 吸気温度出力オフセット									*/
/* 吸気圧出力オフセット */
#define	SIGNAL_VALUE_INTAKE_MANIFOLD_PRESS_OFFSET_ISOCAN	-100/* 吸気圧出力オフセット									*/

/* GPS精度 */
#define	SIGNAL_VALUE_QUALITY_IGNORE				0xFF 	/*	GPS精度: Ignore																	*/
#define	SIGNAL_VALUE_QUALITY_FIXNOTAVAILABLE	0x00 	/*	GPS精度: Fix not available or invalid											*/
#define	SIGNAL_VALUE_QUALITY_GPSSPSMODE			0x01 	/*	GPS精度: GPS SPS Mode, fix valid												*/
#define	SIGNAL_VALUE_QUALITY_DIFFERENTIALGPS	0x02 	/*	GPS精度: Differential GPS, SPS Mode, fix valid									*/
#define	SIGNAL_VALUE_QUALITY_GPSPPSMODE			0x03 	/*	GPS精度: GPS PPS Mode, fix valid												*/
#define	SIGNAL_VALUE_QUALITY_REALTIMEKINEMATIC	0x04 	/*	GPS精度: Real Time Kinematic. System used in RTK mode with fixed integers		*/
#define	SIGNAL_VALUE_QUALITY_FLOATRTK			0x05 	/*	GPS精度: Float RTK. Satellite system used in RTK mode, floating integers		*/
#define	SIGNAL_VALUE_QUALITY_DEADRECKONINGMODE	0x06 	/*	GPS精度: Estimated (dead reckoning) Mode										*/
#define	SIGNAL_VALUE_QUALITY_MANUALINPUTMODE	0x07 	/*	GPS精度: Manual Input Mode														*/
#define	SIGNAL_VALUE_QUALITY_SIMULATORMODE		0x08 	/*	GPS精度: Simulator Mode															*/

/********************************************************/
/*	内部変数定義										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EncodeValue(UI_16 req_id, SI_32* signal_value, SI_32 key);	/*	暗号化計算値取得										*/

SIGNAL_LOCAL UI_16 get_signal_InvalidValue(SI_32 *out);							/*																	*/
SIGNAL_LOCAL UI_16 get_signal_RandomValue(SI_32 *out);							/*	乱数	Random Value											*/
SIGNAL_LOCAL UI_16 get_signal_CangatewayEcuId(SI_32 *out);						/*	通信機識別信号	CAN-Gateway ECU ID								*/
SIGNAL_LOCAL UI_16 get_signal_VehicleNameHighOrder(SI_32 *out);					/*	車両名称(先頭4文字)	Vehicle Name (High-Order)					*/
SIGNAL_LOCAL UI_16 get_signal_VehicleNameLowOrder(SI_32 *out);					/*	車両名称(後方4文字)	Vehicle Name (Low-Order)					*/
SIGNAL_LOCAL UI_16 get_signal_Destination(SI_32 *out);							/*	仕向け地	Destination											*/
SIGNAL_LOCAL UI_16 get_signal_DriverPosition(SI_32 *out);							/*	ドライバーポジション	Driver Position							*/
SIGNAL_LOCAL UI_16 get_signal_EngineModelHighOrder(SI_32 *out);					/*	エンジン型式(先頭4文字)	Engine Model Code (High-Order)			*/
SIGNAL_LOCAL UI_16 get_signal_EngineModelLowOrder(SI_32 *out);					/*	エンジン型式(後方4文字)	Engine Model Code (Low-Order)			*/
SIGNAL_LOCAL UI_16 get_signal_EngineCharacter(SI_32 *out);						/*	エンジン特性	Engine Character								*/
SIGNAL_LOCAL UI_16 get_signal_DriveTrainCharacter(SI_32 *out);					/*	ドライブトレーン	Drive Train Character						*/
SIGNAL_LOCAL UI_16 get_signal_UTCTime(SI_32 *out);								/*	UTC時間	UTCTime													*/
SIGNAL_LOCAL UI_16 get_signal_Date(SI_32 *out);									/*	日付	Date													*/
SIGNAL_LOCAL UI_16 get_signal_Latitude(SI_32 *out);								/*	緯度	Latitude												*/
SIGNAL_LOCAL UI_16 get_signal_Longitude(SI_32 *out);								/*	経度	Longitude												*/
SIGNAL_LOCAL UI_16 get_signal_Altitude(SI_32 *out);								/*	高度	Altitude												*/
SIGNAL_LOCAL UI_16 get_signal_GeoidalSeparation(SI_32 *out);						/*	ジオイド高	Geoidal Separation									*/
SIGNAL_LOCAL UI_16 get_signal_HorizontallySpeed(SI_32 *out);						/*	対地速度	Horizontally Speed									*/
SIGNAL_LOCAL UI_16 get_signal_Accuracy(SI_32 *out);								/*	精度	Accuracy 												*/
SIGNAL_LOCAL UI_16 get_signal_DOP(SI_32 *out);									/*	DOP情報	DOP														*/
SIGNAL_LOCAL UI_16 get_signal_AcceleratorOperation(SI_32 *out);					/*	アクセル操作	Accelerator Operation							*/
SIGNAL_LOCAL UI_16 get_signal_BrakeFlag(SI_32 *out);								/*	ブレーキ操作フラグ	Brake Flag									*/
SIGNAL_LOCAL UI_16 get_signal_BrakeOperation(SI_32 *out);							/*	ブレーキ操作量	Brake Operation									*/
SIGNAL_LOCAL UI_16 get_signal_ShiftOperation(SI_32 *out);							/*	シフト操作	Shift Operation										*/
SIGNAL_LOCAL UI_16 get_signal_FuelIndicator(SI_32 *out);						/*	ヒューエル残量													*/
SIGNAL_LOCAL UI_16 get_signal_Steering(SI_32 *out);								/*	ステアリング	Steering										*/
SIGNAL_LOCAL UI_16 get_signal_Doors(SI_32 *out);								/*	ドア	Doors													*/
SIGNAL_LOCAL UI_16 get_signal_AirCondition(SI_32 *out);							/*	エアーコンディション	Air Condition							*/
SIGNAL_LOCAL UI_16 get_signal_DrivingSupport(SI_32 *out);						/*	ドライビングサポート	Driving Support							*/
SIGNAL_LOCAL UI_16 get_signal_Blinker(SI_32 *out);								/*	ウィンカー	Blinker												*/
SIGNAL_LOCAL UI_16 get_signal_SittingandSeatBeltStatus(SI_32 *out);				/*	シート、シートベルト	Sitting and SeatBelt Status				*/
SIGNAL_LOCAL UI_16 get_signal_LampStatus(SI_32 *out);								/*	ランプ	Lamp Status												*/
SIGNAL_LOCAL UI_16 get_signal_Power(SI_32 *out);								/*	起動	Power													*/
SIGNAL_LOCAL UI_16 get_signal_EngineRev(SI_32 *out);								/*	エンジンスピード	Engine Rev									*/
SIGNAL_LOCAL UI_16 get_signal_VehicleSpeed(SI_32 *out);							/*	車速	Vehicle Speed											*/
SIGNAL_LOCAL UI_16 get_signal_HighResolutionSpeedFront(SI_32 *out);				/*	高精度車速(フロント)	High Resolution Speed (Front)			*/
SIGNAL_LOCAL UI_16 get_signal_HighResolutionSpeedRear(SI_32 *out);				/*	高精度車速(リア)	High Resolution Speed (Rear)				*/
SIGNAL_LOCAL UI_16 get_signal_AccelerationPlane(SI_32 *out);						/*	加速度(平面)	Acceleration(Plane)								*/
SIGNAL_LOCAL UI_16 get_signal_YawandPitchRate(SI_32 *out);						/*	ヨーレート、ピッチレート	Yaw and Pitch Rate					*/
SIGNAL_LOCAL UI_16 get_signal_UnitTemperature(SI_32 *out);						/*	ユニット温度	Unit Temperature								*/	
SIGNAL_LOCAL UI_16 get_signal_TirePressureFront(SI_32 *out);					/*	フロントタイヤ圧	Tire Pressure (Front)						*/
SIGNAL_LOCAL UI_16 get_signal_TirePressureRear(SI_32 *out);						/*	リアタイヤ圧	Tire Pressure (Rear)							*/
SIGNAL_LOCAL UI_16 get_signal_FuelConsumption(SI_32 *out);						/*	始動後燃料噴射量積算値	Fuel Consumption						*/
SIGNAL_LOCAL UI_16 get_signal_DistancefromRestart(SI_32 *out);					/*	始動後走行距離	Distance from Restart							*/
SIGNAL_LOCAL UI_16 get_signal_OutputTorque(SI_32 *out);							/*	出力トルク	Output Torque										*/
SIGNAL_LOCAL UI_16 get_signal_EngineSensor(SI_32 *out);							/*	エンジンセンサー	Engine Sensor								*/
SIGNAL_LOCAL UI_16 get_signal_AtmosphericPressure(SI_32 *out);					/*	大気圧	Atmospheric pressure									*/
SIGNAL_LOCAL UI_16 get_signal_EngineRevRawValue(SI_32 *out);					/*	エンジン回転数（生値）	Engine Rev (Raw Value)					*/
SIGNAL_LOCAL UI_16 get_signal_OutputUpDwnTorque(SI_32 *out);					/*	出力アップダウントルク	Output Torque UpDwon					*/

#ifdef ADVANCE_ENABLE
SIGNAL_LOCAL UI_16 get_signal_IntakeManifoldPressure(SI_32 *out);					/*	吸気圧	Intake Manifold Pressure								*/
SIGNAL_LOCAL UI_16 get_signal_EngineOilPressure(SI_32 *out);						/*	エンジン油圧	Engine Oil Pressure								*/
SIGNAL_LOCAL UI_16 get_signal_FuelPressure(SI_32 *out);							/*	燃料圧力	Fuel Pressure										*/
SIGNAL_LOCAL UI_16 get_signal_EngineCoolantTempertureExt1(SI_32 *out);			/*	エンジン水温（拡張）	Engine Coolant Temperture（Ext）		*/
SIGNAL_LOCAL UI_16 get_signal_EngineCoolantTempertureExt2(SI_32 *out);			/*	エンジン水温（拡張）	Engine Coolant Temperture（Ext）		*/
SIGNAL_LOCAL UI_16 get_signal_EngineOilTemperture(SI_32 *out);					/*	エンジン油温	Engine Oil Temperture							*/
SIGNAL_LOCAL UI_16 get_signal_ExhaustGasTemperture(SI_32 *out);					/*	排気温度	Exhaust Gas Temperture								*/
SIGNAL_LOCAL UI_16 get_signal_Volts(SI_32 *out);									/*	電圧	Volts													*/
SIGNAL_LOCAL UI_16 get_signal_AirFuelRatio(SI_32 *out);							/*	空燃比	Air-Fuel Ratio											*/
#endif /*ADVANCE_ENABLE*/
#ifdef	ISOCAN_ENABLE
SIGNAL_LOCAL UI_16 get_signal_IsoCanIntakeManifoldPressure(SI_32 *out);			/*	吸気圧	Intake Manifold Pressure								*/
SIGNAL_LOCAL UI_16 get_signal_IsoCanEngineCoolantTemperture(SI_32 *out);			/*	エンジン水温	Engine Coolant Temperture						*/
SIGNAL_LOCAL UI_16 get_signal_IsoCanThrottle(SI_32 *out);							/*	スロットル開度	Throttle										*/
SIGNAL_LOCAL UI_16 get_signal_IsoCanIntakeTemperature(SI_32 *out);				/*	吸気温度	IntakeTemperature									*/
SIGNAL_LOCAL UI_16 get_signal_IsoCanVehicleSpeed(SI_32 *out);						/*	車速	Vehicle Speed											*/
SIGNAL_LOCAL UI_16 get_signal_IsoCanEngineRev(SI_32 *out);						/*	エンジン回転数	Engine Rev										*/
SIGNAL_LOCAL UI_16 get_signal_IsoCanVolts(SI_32 *out);							/*	電圧	Volts													*/
#endif	/* ISOCAN_ENABLE */


/*SDカード用固定値ID*/
SIGNAL_LOCAL const UI_16 signalFixedListTable_SD[] = 
{
	1	,	/*	通信機識別信号	CAN-Gateway ECU ID									*/
	2	,	/*	車両名称(先頭4文字)	Vehicle Name (High-Order)						*/
	3	,	/*	車両名称(後方4文字)	Vehicle Name (Low-Order)						*/
	4	,	/*	仕向け地	Destination												*/
	5	,	/*	ドライバーポジション	Driver Position								*/
	6	,	/*	エンジン型式(先頭4文字)	Engine Model Code (High-Order)				*/
	7	,	/*	エンジン型式(後方4文字)	Engine Model Code (Low-Order)				*/
	8	,	/*	エンジン特性	Engine Character									*/
	9	,	/*	ドライブトレーン	Drive Train Character							*/
};

/*SDカード用変数値ID*/
SIGNAL_LOCAL const UI_16 signalValiableListTable_SD[] = 
{
	101	,	/*	UTC時間	UTCTime														*/
	102	,	/*	日付	Date														*/
	103	,	/*	乱数	Random Value												*/
	104	,	/*	緯度	Latitude													*/
	105	,	/*	経度	Longitude													*/
	106	,	/*	高度	Altitude													*/
	107	,	/*	ジオイド高	Geoidal Separation										*/
	108	,	/*	対地速度	Horizontally Speed										*/
	110	,	/*	乱数	Random Value												*/
	111	,	/*	精度	Accuracy 													*/
	112	,	/*	DOP情報	DOP															*/
	201	,	/*	アクセル操作	Accelerator Operation								*/
	202	,	/*	ブレーキ操作フラグ	Brake Flag										*/
	203	,	/*	ブレーキ操作量	Brake Operation										*/
	204	,	/*	シフト操作	Shift Operation											*/
	207	,	/*	乱数	Random Value												*/
	208	,	/*	ステアリング	Steering											*/
	209	,	/*	ウィンカー	Blinker													*/
	211	,	/*	乱数	Random Value												*/
	212	,	/*	エアーコンディション	Air Condition								*/
	214	,	/*	ドライビングサポート	Driving Support								*/
	215	,	/*	ドア	Doors														*/
	218	,	/*	シート、シートベルト	Sitting and Seat-belt Status				*/
	219	,	/*	ランプ	Lamp Status													*/
	220	,	/*	起動	Power														*/
	221	,	/*	エンジンスピード	Engine Rev										*/
	222	,	/*	車速	Vehicle Speed												*/
	223	,	/*	高精度車速(フロント)	High Resolution Speed (Front)				*/
	224	,	/*	高精度車速(リア)	High Resolution Speed (Rear)					*/
	225	,	/*	加速度(平面)	Acceleration(Plane)									*/
	227	,	/*	ヨーレート、ピッチレート	Yaw and Pitch Rate						*/
	231	,	/*	ユニット温度	Unit Temperature									*/
	232	,	/*	フロントタイヤ圧	Tire Pressure (Front)							*/
	233	,	/*	リアタイヤ圧	Tire Pressure (Rear)								*/
	235	,	/*	始動後燃料噴射量積算値	Fuel Consumption							*/
	240	,	/*	始動後走行距離	Distance from Restart								*/
	253	,	/*	出力トルク	Output Torque											*/
	254	,	/*	エンジンセンサー	Engine Sensor									*/
	259	,	/*	出力アップダウントルク	Output Torque UpDwon						*/
	260	,	/*	大気圧	Atmospheric pressure										*/
	270	,	/*	エンジン回転数（生値）Engine Rev(Raw Value)							*/
#ifdef ISOCAN_ENABLE
	640	,	/*	車速	Vehicle Speed												*/
	641	,	/*	エンジン回転数	Engine Rev											*/
	642	,	/*	吸気圧	Intake Manifold Pressure									*/
	643	,	/*	エンジン水温	Engine Coolant Temperture							*/
	644	,	/*	電圧	Volts														*/
	645	,	/*	スロットル開度	Throttle											*/
	646	,	/*	吸気温度	IntakeTemperature										*/
#endif /* ISOCAN_ENABLE */
};

/*Bluetooth用固定値ID*/
SIGNAL_LOCAL const UI_16 signalFixedListTable_BT[] = 
{
	1	,	/*	通信機識別信号	CAN-Gateway ECU ID									*/
	2	,	/*	車両名称(先頭4文字)	Vehicle Name (High-Order)						*/
	3	,	/*	車両名称(後方4文字)	Vehicle Name (Low-Order)						*/
	4	,	/*	仕向け地	Destination												*/
	5	,	/*	ドライバーポジション	Driver Position								*/
	6	,	/*	エンジン型式(先頭4文字)	Engine Model Code (High-Order)				*/
	7	,	/*	エンジン型式(後方4文字)	Engine Model Code (Low-Order)				*/
	8	,	/*	エンジン特性	Engine Character									*/
	9	,	/*	ドライブトレーン	Drive Train Character							*/
};

/*Bluetooth用変数値ID*/
SIGNAL_LOCAL const UI_16 signalValiableListTable_BT[] = 
{
	101	,	/*	UTC時間	UTCTime														*/
	102	,	/*	日付	Date														*/
	103	,	/*	乱数	Random Value												*/
	104	,	/*	緯度	Latitude													*/
	105	,	/*	経度	Longitude													*/
	106	,	/*	高度	Altitude													*/
	107	,	/*	ジオイド高	Geoidal Separation										*/
	108	,	/*	対地速度	Horizontally Speed										*/
	110	,	/*	乱数	Random Value												*/
	111	,	/*	精度	Accuracy 													*/
	112	,	/*	DOP情報	DOP															*/
	201	,	/*	アクセル操作	Accelerator Operation								*/
	202	,	/*	ブレーキ操作フラグ	Brake Flag										*/
	203	,	/*	ブレーキ操作量	Brake Operation										*/
	204	,	/*	シフト操作	Shift Operation											*/
	207	,	/*	乱数	Random Value												*/
	208	,	/*	ステアリング	Steering											*/
	209	,	/*	ウィンカー	Blinker													*/
	211	,	/*	乱数	Random Value												*/
	212	,	/*	エアーコンディション	Air Condition								*/
	214	,	/*	ドライビングサポート	Driving Support								*/
	215	,	/*	ドア	Doors														*/
	218	,	/*	シート、シートベルト	Sitting and Seat-belt Status				*/
	219	,	/*	ランプ	Lamp Status													*/
	220	,	/*	起動	Power														*/
	221	,	/*	エンジンスピード	Engine Rev										*/
	222	,	/*	車速	Vehicle Speed												*/
	223	,	/*	高精度車速(フロント)	High Resolution Speed (Front)				*/
	224	,	/*	高精度車速(リア)	High Resolution Speed (Rear)					*/
	225	,	/*	加速度(平面)	Acceleration(Plane)									*/
	227	,	/*	ヨーレート、ピッチレート	Yaw and Pitch Rate						*/
	231	,	/*	ユニット温度	Unit Temperature									*/
	232	,	/*	フロントタイヤ圧	Tire Pressure (Front)							*/
	233	,	/*	リアタイヤ圧	Tire Pressure (Rear)								*/
	235	,	/*	始動後燃料噴射量積算値	Fuel Consumption							*/
	240	,	/*	始動後走行距離	Distance from Restart								*/
	253	,	/*	出力トルク	Output Torque											*/
	254	,	/*	エンジンセンサー	Engine Sensor									*/
	259	,	/*	出力アップダウントルク	Output Torque UpDwon						*/
	260	,	/*	大気圧	Atmospheric pressure										*/
	270	,	/*	エンジン回転数（生値）Engine Rev(Raw Value)							*/
#ifdef ADVANCE_ENABLE
	601	,	/*	吸気圧	Intake Manifold Pressure									*/
	602	,	/*	エンジン油圧	Engine Oil Pressure									*/
	603	,	/*	燃料圧力	Fuel Pressure											*/
	604	,	/*	エンジン水温（拡張）	Engine Coolant Temperture（Ext）			*/
	605	,	/*	エンジン水温（拡張）	Engine Coolant Temperture（Ext）			*/
	606	,	/*	エンジン油温	Engine Oil Temperture								*/
	607	,	/*	排気温度	Exhaust Gas Temperture									*/
	608	,	/*	電圧	Volts														*/
	609	,	/*	空燃比	Air-Fuel Ratio												*/
#endif /*ADVANCE_ENABLE*/
#ifdef ISOCAN_ENABLE
	640	,	/*	車速	Vehicle Speed												*/
	641	,	/*	エンジン回転数	Engine Rev											*/
	642	,	/*	吸気圧	Intake Manifold Pressure									*/
	643	,	/*	エンジン水温	Engine Coolant Temperture							*/
	644	,	/*	電圧	Volts														*/
	645	,	/*	スロットル開度	Throttle											*/
	646	,	/*	吸気温度	IntakeTemperature										*/
#endif /* ISOCAN_ENABLE */
};

typedef UI_16 (* T_SignalInfoFunc) (SI_32 *out);

/*公開信号取得用関数テーブル*/
SIGNAL_LOCAL const T_SignalInfoFunc C_SignalInfoFuncTbl[SIGNAL_INFO_FUNC_NUM][2] = {
	/*	BT											SD											仕様で割り振られているもの						*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	0																	*/
	{	&get_signal_CangatewayEcuId				,	&get_signal_CangatewayEcuId				/*	&get_signal_CangatewayEcuId						*/,},/*	1	通信機識別信号	CAN-Gateway ECU ID								*/
	{	&get_signal_VehicleNameHighOrder		,	&get_signal_VehicleNameHighOrder		/*	&get_signal_VehicleNameHighOrder				*/,},/*	2	車両名称(先頭4文字)	Vehicle Name (High-Order)					*/
	{	&get_signal_VehicleNameLowOrder			,	&get_signal_VehicleNameLowOrder			/*	&get_signal_VehicleNameLowOrder					*/,},/*	3	車両名称(後方4文字)	Vehicle Name (Low-Order)					*/
	{	&get_signal_Destination					,	&get_signal_Destination					/*	&get_signal_Destination							*/,},/*	4	仕向け地	Destination											*/
	{	&get_signal_DriverPosition				,	&get_signal_DriverPosition				/*	&get_signal_DriverPosition						*/,},/*	5	ドライバーポジション	Driver Position							*/
	{	&get_signal_EngineModelHighOrder		,	&get_signal_EngineModelHighOrder		/*	&get_signal_EngineModelHighOrder				*/,},/*	6	エンジン型式(先頭4文字)	Engine Model Code (High-Order)			*/
	{	&get_signal_EngineModelLowOrder			,	&get_signal_EngineModelLowOrder			/*	&get_signal_EngineModelLowOrder					*/,},/*	7	エンジン型式(後方4文字)	Engine Model Code (Low-Order)			*/
	{	&get_signal_EngineCharacter				,	&get_signal_EngineCharacter				/*	&get_signal_EngineCharacter						*/,},/*	8	エンジン特性	Engine Character								*/
	{	&get_signal_DriveTrainCharacter			,	&get_signal_DriveTrainCharacter			/*	&get_signal_DriveTrainCharacter					*/,},/*	9	ドライブトレーン	Drive Train Character						*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_SeatSetting							*/,},/*	10	シート設定	Seat Setting										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	11																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	12																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	13																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	14	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	15																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	16																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	17																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	18	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	19																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	20																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	21																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	22																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	23																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	24																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	25	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	26																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	27																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	28																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	29																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	30																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	31																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	32																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	33																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	34																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	35																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	36																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	37																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	38																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	39																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	40																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	41																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	42																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	43																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	44																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	45																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	46																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	47																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	48																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	49																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	50	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	51																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	52																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	53																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	54																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	55																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	56																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	57																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	58																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	59																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	60																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	61																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	62																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	63																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	64																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	65																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	66																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	67																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	68																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	69																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	70																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	71																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	72																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	73																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	74																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	75																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	76																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	77																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	78																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	79																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	80																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	81																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	82																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	83	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	84																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	85																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	86																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	87																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	88																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	89																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	90																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	91																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	92																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	93																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	94																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	95																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	96																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	97																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	98																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	99																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	100																	*/
	{	&get_signal_UTCTime						,	&get_signal_UTCTime						/*	&get_signal_UTCTime								*/,},/*	101	UTC時間	UTCTime													*/
	{	&get_signal_Date						,	&get_signal_Date						/*	&get_signal_Date								*/,},/*	102	日付	Date													*/
	{	&get_signal_RandomValue					,	&get_signal_RandomValue					/*	&get_signal_RandomValue							*/,},/*	103	乱数	Random Value											*/
	{	&get_signal_Latitude					,	&get_signal_Latitude					/*	&get_signal_Latitude							*/,},/*	104	緯度	Latitude												*/
	{	&get_signal_Longitude					,	&get_signal_Longitude					/*	&get_signal_Longitude							*/,},/*	105	経度	Longitude												*/
	{	&get_signal_Altitude					,	&get_signal_Altitude					/*	&get_signal_Altitude							*/,},/*	106	高度	Altitude												*/
	{	&get_signal_GeoidalSeparation			,	&get_signal_GeoidalSeparation			/*	&get_signal_GeoidalSeparation					*/,},/*	107	ジオイド高	Geoidal Separation									*/
	{	&get_signal_HorizontallySpeed			,	&get_signal_HorizontallySpeed			/*	&get_signal_HorizontallySpeed					*/,},/*	108	対地速度	Horizontally Speed									*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_VerticalSpeed						*/,},/*	109	垂直速度	Vertical Speed										*/
	{	&get_signal_RandomValue					,	&get_signal_RandomValue					/*	&get_signal_RandomValue							*/,},/*	110	乱数	Random Value											*/
	{	&get_signal_Accuracy					,	&get_signal_Accuracy					/*	&get_signal_Accuracy							*/,},/*	111	精度	Accuracy 												*/
	{	&get_signal_DOP							,	&get_signal_DOP							/*	&get_signal_DOP									*/,},/*	112	DOP情報	DOP														*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	113	加速度	Acceleration(Plane)										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	114	加速度	Acceleration(Vertical)									*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	115	ヨーレート&ピッチレート	Yaw Rate & Pitch Rate					*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	116	ロールレート	Roll Rate										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	117																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	118																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	119																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	120																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	121																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	122																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	123																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	124																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	125																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	126																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	127																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	128																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	129																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	130																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	131																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	132																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	133																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	134	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	135																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	136																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	137																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	138																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	139																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	140																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	141																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	142	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	143																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	144																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	145																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	146																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	147																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	148																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	149																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	150																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	151																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	152																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	153																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	154																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	155																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	156																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	157																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	158																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	159																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	160																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	161																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	162																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	163																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	164																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	165																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	166																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	167																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	168																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	169																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	170																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	171																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	172																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	173																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	174																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	175																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	176																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	177																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	178																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	179																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	180																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	181																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	182																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	183																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	184																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	185																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	186																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	187																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	188	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	189																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	190																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	191																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	192																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	193																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	194																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	195																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	196																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	197																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	198																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	199																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	200																	*/
	{	&get_signal_AcceleratorOperation		,	&get_signal_AcceleratorOperation		/*	&get_signal_AcceleratorOperation				*/,},/*	201	アクセル操作	Accelerator Operation							*/
	{	&get_signal_BrakeFlag					,	&get_signal_BrakeFlag					/*	&get_signal_BrakeFlag							*/,},/*	202	ブレーキ操作フラグ	Brake Flag									*/
	{	&get_signal_BrakeOperation				,	&get_signal_BrakeOperation				/*	&get_signal_BrakeOperation						*/,},/*	203	ブレーキ操作量	Brake Operation									*/
	{	&get_signal_ShiftOperation				,	&get_signal_ShiftOperation				/*	&get_signal_ShiftOperation						*/,},/*	204	シフト操作	Shift Operation										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_ClutchPedalOperation				*/,},/*	205	クラッチ操作	Clutch Pedal Operation							*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_HybridPowerSelect					*/,},/*	206	ハイブリッドパワー選択	Hybrid Power Select						*/
	{	&get_signal_FuelIndicator				,	&get_signal_FuelIndicator				/*	&get_signal_RandomValue							*/,},/*	207	ヒューエル残量													*/
	{	&get_signal_Steering					,	&get_signal_Steering					/*	&get_signal_Steering							*/,},/*	208	ステアリング	Steering										*/
	{	&get_signal_Blinker						,	&get_signal_Blinker						/*	&get_signal_Blinker								*/,},/*	209	ウィンカー	Blinker												*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_Wiper								*/,},/*	210	ワイパー	Wiper												*/
	{	&get_signal_RandomValue					,	&get_signal_RandomValue					/*	&get_signal_RandomValue							*/,},/*	211	乱数	Random Value											*/
	{	&get_signal_AirCondition				,	&get_signal_AirCondition				/*	&get_signal_AirCondition						*/,},/*	212	エアーコンディション	Air Condition							*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_HeaterControl						*/,},/*	213	ヒーターコントロール	Heater Control							*/
	{	&get_signal_DrivingSupport				,	&get_signal_DrivingSupport				/*	&get_signal_DrivingSupport						*/,},/*	214	ドライビングサポート	Driving Support							*/
	{	&get_signal_Doors						,	&get_signal_Doors						/*	&get_signal_Doors								*/,},/*	215	ドア	Doors													*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_Lock								*/,},/*	216	ロック	Lock													*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_Window								*/,},/*	217	ウィンドウ	Window												*/
	{	&get_signal_SittingandSeatBeltStatus	,	&get_signal_SittingandSeatBeltStatus	/*	&get_signal_SittingandSeatBeltStatus			*/,},/*	218	シート、シートベルト	Sitting and SeatBelt Status				*/
	{	&get_signal_LampStatus					,	&get_signal_LampStatus					/*	&get_signal_LampStatus							*/,},/*	219	ランプ	Lamp Status												*/
	{	&get_signal_Power						,	&get_signal_Power						/*	&get_signal_Power								*/,},/*	220	起動	Power													*/
	{	&get_signal_EngineRev					,	&get_signal_EngineRev					/*	&get_signal_EngineRev							*/,},/*	221	エンジンスピード	Engine Rev									*/
	{	&get_signal_VehicleSpeed				,	&get_signal_VehicleSpeed				/*	&get_signal_VehicleSpeed						*/,},/*	222	車速	Vehicle Speed											*/
	{	&get_signal_HighResolutionSpeedFront	,	&get_signal_HighResolutionSpeedFront	/*	&get_signal_HighResolutionSpeedFront			*/,},/*	223	高精度車速(フロント)	High Resolution Speed (Front)			*/
	{	&get_signal_HighResolutionSpeedRear		,	&get_signal_HighResolutionSpeedRear		/*	&get_signal_HighResolutionSpeedRear				*/,},/*	224	高精度車速(リア)	High Resolution Speed (Rear)				*/
	{	&get_signal_AccelerationPlane			,	&get_signal_AccelerationPlane			/*	&get_signal_AccelerationPlane					*/,},/*	225	加速度(平面)	Acceleration(Plane)								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_AccelerationVertical				*/,},/*	226	加速度(垂直)	Acceleration(Vertical)							*/
	{	&get_signal_YawandPitchRate				,	&get_signal_YawandPitchRate				/*	&get_signal_YawandPitchRate						*/,},/*	227	ヨーレート、ピッチレート	Yaw and Pitch Rate					*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RollRate							*/,},/*	228	ロールレート	Roll Rate										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_YawandPitchAngle					*/,},/*	229	ヨー、ピッチ角	Yaw and Pitch Angle								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RollAngle							*/,},/*	230	ロール角	Roll Angle											*/
	{	&get_signal_UnitTemperature				,	&get_signal_UnitTemperature				/*	&get_signal_UnitTemperature						*/,},/*	231	ユニット温度	Unit Temperature								*/
	{	&get_signal_TirePressureFront			,	&get_signal_TirePressureFront			/*	&get_signal_TirePressureFront					*/,},/*	232	フロントタイヤ圧	Tire Pressure (Front)						*/
	{	&get_signal_TirePressureRear			,	&get_signal_TirePressureRear			/*	&get_signal_TirePressureRear					*/,},/*	233	リアタイヤ圧	Tire Pressure (Rear)							*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_EnergyIndicator						*/,},/*	234	エネルギインジケータ	Energy Indicator						*/
	{	&get_signal_FuelConsumption				,	&get_signal_FuelConsumption				/*	&get_signal_FuelConsumption						*/,},/*	235	始動後燃料噴射量積算値	Fuel Consumption						*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_ElectricityConsumptionsincerestart	*/,},/*	236	始動後電力消費量積算値	Electricity Consumption (since restart)	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_GaseousFuelConsumption				*/,},/*	237	始動後気体燃料噴射量積算値	Gaseous Fuel Consumption			*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_HyblidDriveMode						*/,},/*	238	ハイブリッド走行状態	Hyblid Drive Mode						*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_Odometer							*/,},/*	239	ODOメータ走行距離	Odometer									*/
	{	&get_signal_DistancefromRestart			,	&get_signal_DistancefromRestart			/*	&get_signal_DistancefromRestart					*/,},/*	240	始動後走行距離	Distance from Restart							*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DistanceofEngineDrive				*/,},/*	241	エンジン走行距離	Distance of Engine Drive					*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DistanceofEVDrive					*/,},/*	242	EV走行距離	Distance of EV Drive								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DistanceofGaseousFuel				*/,},/*	243	ガスエンジン走行距離	Distance of Gaseous Fuel				*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DrivableDistanceALL					*/,},/*	244	走行可能距離	Drivable Distance(ALL)							*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DrivableDistanceofFuel				*/,},/*	245	エンジン走行可能距離	Drivable Distance of Fuel				*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DrivableDistanceofElectricity		*/,},/*	246	EV走行可能距離	Drivable Distance of Electricity				*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DrivableDistanceofGaseousFuel		*/,},/*	247	ガスエンジン走行可能距離	Drivable Distance of Gaseous Fuel	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_ChargingLidStatus					*/,},/*	248	供給口開閉状態	Charging Lid Status								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_ChargingPlugStatus					*/,},/*	249	充電ケーブル接続状態	Charging Lid Status						*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_ChargingandSupplyingStatus			*/,},/*	250	充放電状態	Charging and Supplying Status						*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RaderSignal							*/,},/*	251	最近接障害物距離	Rader Signal								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_MinimumTimetoCollision				*/,},/*	252	最接近障害物衝突時間	Minimum Time to Collision 				*/
	{	&get_signal_OutputTorque				,	&get_signal_OutputTorque				/*	&get_signal_OutputTorque						*/,},/*	253	出力トルク	Output Torque										*/
	{	&get_signal_EngineSensor				,	&get_signal_EngineSensor				/*	&get_signal_InvalidValue						*/,},/*	254 エンジンセンサー Engine Sensor									*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	255																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	256																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	257																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	258																	*/
	{	&get_signal_OutputUpDwnTorque			,	&get_signal_OutputUpDwnTorque			/*	&get_signal_OutputUpDwnTorque					*/,},/*	259	出力アップダウントルク	Output Torque UpDwon					*/
	{	&get_signal_AtmosphericPressure			,	&get_signal_AtmosphericPressure			/*	&get_signal_InvalidValue						*/,},/*	260 大気圧	Atmospheric pressure									*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	261																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	262																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	263																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	264																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	265																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	266																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	267																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	268																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	269																	*/
	{	&get_signal_EngineRevRawValue			,	&get_signal_EngineRevRawValue			/*	&get_signal_InvalidValue						*/,},/*	270 エンジン回転数(生値)	Engine Rev (Raw Value)					*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	271																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	272																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	273																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	274																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	275																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	276																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	277																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	278																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	279																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	280																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	281																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	282																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	283																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	284	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	285																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	286																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	287																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	288																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	289																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	290																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	291																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	292																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	293																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	294																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	295																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	296																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	297																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	298																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	299																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	300																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	301																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	302																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	303																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	304	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	305																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	306																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	307																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	308																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	309																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	310																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	311																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	312																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	313																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	314																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	315																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	316																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	317																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	318																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	319																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	320																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	321																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	322																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	323																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	324																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	325																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	326																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	327																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	328																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	329																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	330																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	331																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	332																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	333																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	334																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	335																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	336																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	337	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	338																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	339																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	340	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	341																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	342																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	343																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	344																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	345																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	346																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	347																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	348																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	349																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	350																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	351																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	352																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	353																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	354																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	355																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	356	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	357																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	358																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	359																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	360																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	361																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	362																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	363																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	364																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	365																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	366																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	367																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	368																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	369																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	370	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	371	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	372																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	373																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	374																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	375																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	376																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	377																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	378																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	379																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	380																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	381																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	382																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	383																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	384																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	385																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	386																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	387																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	388																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	389																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	390																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	391																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	392																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	393																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	394																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	395																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	396																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	397																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	398																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	399	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	400																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	401																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	402																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	403	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	404																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	405																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	406																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	407																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	408																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	409																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	410																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	411																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	412																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	413																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	414																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	415																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	416																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	417																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	418																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	419																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	420																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	421																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	422	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	423																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	424																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	425																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	426																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	427																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	428																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	429																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	430	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	431																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	432																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	433																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	434																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	435																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	436																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	437																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	438																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	439																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	440																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	441																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	442																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	443																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	444																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	445																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	446																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	447																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	448																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	449																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	450																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	451																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	452																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	453																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	454																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	455	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	456																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	457																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	458																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	459																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	460																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	461																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	462																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	463																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	464																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	465	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	466																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	467																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	468																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	469																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	470																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	471																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	472																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	473																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	474																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	475																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	476																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	477																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	478																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	479																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	480																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	481																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	482																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	483																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	484																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	485																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	486																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	487																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	488																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	489																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	490																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	491																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	492																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	493																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	494																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	495																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	496																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	497																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	498																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	499																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	500																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_SOCSegmentPrius						*/,},/*	501	SOCセグメント PRIUS for PriusCUP for TMA	SOC(Segment)		*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_SOCSegment748A						*/,},/*	502	SOCセグメント 748A個別	SOC(Segment)							*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DiagnosticCode						*/,},/*	503	ダイアグコード	Diagnostic Code									*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_Warning								*/,},/*	504	ワーニング	 Warning											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_PlugErr								*/,},/*	505	プラグエラー	Plug Err										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	506																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	507																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	508																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	509																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	510																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	511																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	512																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	513																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	514																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	515																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	516																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	517																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	518																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	519																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	520																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	521																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	522																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	523																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	524																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	525																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	526																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	527																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	528																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	529																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	530																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	531																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	532																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	533																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	534																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	535																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	536																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	537	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	538																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	539																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	540																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	541																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	542																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	543																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	544																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	545																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	546																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	547																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	548																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	549																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	550																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	551																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	552																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	553																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	554																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	555																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	556																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	557	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	558																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	559																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	560																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	561																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	562																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	563	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	564																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	565	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	566																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	567																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	568																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	569																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	570																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	571																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	572																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	573																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	574																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	575																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	576																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	577																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	578																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	579																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	580																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	581																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	582																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	583																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	584																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	585																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	586																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	587																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	588																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	589																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	590																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	591																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	592																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	593																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	594	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	595																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	596																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	597																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	598																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	599																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	600																	*/
#ifdef ADVANCE_ENABLE
	{	&get_signal_IntakeManifoldPressure		,	&get_signal_InvalidValue				/*	&get_signal_IntakeManifoldPressure				*/,},/*	601	吸気圧	Intake Manifold Pressure								*/
	{	&get_signal_EngineOilPressure			,	&get_signal_InvalidValue				/*	&get_signal_EngineOilPressure					*/,},/*	602	エンジン油圧	Engine Oil Pressure								*/
	{	&get_signal_FuelPressure				,	&get_signal_InvalidValue				/*	&get_signal_FuelPressure						*/,},/*	603	燃料圧力	Fuel Pressure										*/
	{	&get_signal_EngineCoolantTempertureExt1	,	&get_signal_InvalidValue				/*	&get_signal_EngineCoolantTempertureExt1			*/,},/*	604	エンジン水温（拡張）	Engine Coolant Temperture（Ext）		*/
	{	&get_signal_EngineCoolantTempertureExt2	,	&get_signal_InvalidValue				/*	&get_signal_EngineCoolantTempertureExt2			*/,},/*	605	エンジン水温（拡張）	Engine Coolant Temperture（Ext）		*/
	{	&get_signal_EngineOilTemperture			,	&get_signal_InvalidValue				/*	&get_signal_EngineOilTemperture					*/,},/*	606	エンジン油温	Engine Oil Temperture							*/
	{	&get_signal_ExhaustGasTemperture		,	&get_signal_InvalidValue				/*	&get_signal_ExhaustGasTemperture				*/,},/*	607	排気温度	Exhaust Gas Temperture								*/
	{	&get_signal_Volts						,	&get_signal_InvalidValue				/*	&get_signal_Volts								*/,},/*	608	電圧	Volts													*/
	{	&get_signal_AirFuelRatio				,	&get_signal_InvalidValue				/*	&get_signal_AirFuelRatio						*/,},/*	609	空燃比	Air-Fuel Ratio											*/
#else /*ADVANCE_ENABLE*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IntakeManifoldPressure				*/,},/*	601	吸気圧	Intake Manifold Pressure								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_EngineOilPressure					*/,},/*	602	エンジン油圧	Engine Oil Pressure								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_FuelPressure						*/,},/*	603	燃料圧力	Fuel Pressure										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_EngineCoolantTempertureExt1			*/,},/*	604	エンジン水温（拡張）	Engine Coolant Temperture（Ext）		*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_EngineCoolantTempertureExt2			*/,},/*	605	エンジン水温（拡張）	Engine Coolant Temperture（Ext）		*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_EngineOilTemperture					*/,},/*	606	エンジン油温	Engine Oil Temperture							*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_ExhaustGasTemperture				*/,},/*	607	排気温度	Exhaust Gas Temperture								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_Volts								*/,},/*	608	電圧	Volts													*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_AirFuelRatio						*/,},/*	609	空燃比	Air-Fuel Ratio											*/
#endif /*ADVANCE_ENABLE*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	610																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	611																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	612																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	613																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	614																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	615																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	616	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	617																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	618																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	619																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	620																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	621																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	622																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	623																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	624																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	625																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	626																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	627																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	628																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	629																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	630																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	631																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	632																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	633																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	634																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	635																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	636	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	637																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	638																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	639																	*/
#ifdef ISOCAN_ENABLE
	{	&get_signal_IsoCanVehicleSpeed			,	&get_signal_IsoCanVehicleSpeed			/*	&get_signal_IsoCanVehicleSpeed					*/,},/*	640	車速	Vehicle Speed											*/
	{	&get_signal_IsoCanEngineRev				,	&get_signal_IsoCanEngineRev				/*	&get_signal_IsoCanEngineRev						*/,},/*	641	エンジン回転数	Engine Rev										*/
	{	&get_signal_IsoCanIntakeManifoldPressure,	&get_signal_IsoCanIntakeManifoldPressure/*	&get_signal_IsoCanIntakeManifoldPressure		*/,},/*	642	吸気圧	Intake Manifold Pressure								*/
	{	&get_signal_IsoCanEngineCoolantTemperture,	&get_signal_IsoCanEngineCoolantTemperture/*	&get_signal_IsoCanEngineCoolantTemperture		*/,},/*	643	エンジン水温	Engine Coolant Temperture						*/
	{	&get_signal_IsoCanVolts					,	&get_signal_IsoCanVolts					/*	&get_signal_IsoCanVolts							*/,},/*	644	電圧	Volts													*/
	{	&get_signal_IsoCanThrottle				,	&get_signal_IsoCanThrottle				/*	&get_signal_IsoCanThrottle						*/,},/*	645	スロットル開度	Throttle										*/
	{	&get_signal_IsoCanIntakeTemperature		,	&get_signal_IsoCanIntakeTemperature		/*	&get_signal_IsoCanIntakeTemperature				*/,},/*	646	吸気温度	IntakeTemperature									*/
#else	/* ISOCAN_ENABLE */
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IsoCanVehicleSpeed					*/,},/*	640	車速	Vehicle Speed											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IsoCanEngineRev						*/,},/*	641	エンジン回転数	Engine Rev										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IsoCanIntakeManifoldPressure		*/,},/*	642	吸気圧	Intake Manifold Pressure								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IsoCanEngineCoolantTemperture		*/,},/*	643	エンジン水温	Engine Coolant Temperture						*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IsoCanVolts							*/,},/*	644	電圧	Volts													*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IsoCanThrottle						*/,},/*	645	スロットル開度	Throttle										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IsoCanIntakeTemperature				*/,},/*	646	吸気温度	IntakeTemperature									*/
#endif /* ISOCAN_ENABLE */
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	647																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	648																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	649																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	650																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	651																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	652																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	653																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	654																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	655																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	656																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	657																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	658																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	659																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	660																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	661																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	662																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	663																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	664																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	665																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	666																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	667																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	668																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	669																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	670																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	671																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	672	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	673	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	674																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	675																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	676																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	677																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	678																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	679																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	680																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	681																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	682	乱数	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	683																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	684																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	685																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	686																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	687																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	688																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	689																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	690																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	691																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	692																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	693																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	694																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	695																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	696																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	697																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	698																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	699																	*/
};


typedef UI_16 (* T_EncodeFunc) (UI_16 req_id, SI_32 *signal_value, SI_32 key);
/*暗号化用関数テーブル NULLは暗号化不要*/
SIGNAL_LOCAL const T_EncodeFunc C_EncodeFuncTbl[SIGNAL_INFO_FUNC_NUM] = {
	NULL										,/*	0																	*/
	NULL										,/*	1	通信機識別信号	CAN-Gateway ECU ID								*/
	NULL										,/*	2	車両名称(先頭4文字)	Vehicle Name (High-Order)					*/
	NULL										,/*	3	車両名称(後方4文字)	Vehicle Name (Low-Order)					*/
	NULL										,/*	4	仕向け地	Destination											*/
	NULL										,/*	5	ドライバーポジション	Driver Position							*/
	NULL										,/*	6	エンジン型式(先頭4文字)	Engine Model Code (High-Order)			*/
	NULL										,/*	7	エンジン型式(後方4文字)	Engine Model Code (Low-Order)			*/
	NULL										,/*	8	エンジン特性	Engine Character								*/
	NULL										,/*	9	ドライブトレーン	Drive Train Character						*/
	NULL										,/*	10	シート設定	Seat Setting										*/
	NULL										,/*	11																	*/
	NULL										,/*	12																	*/
	NULL										,/*	13																	*/
	NULL										,/*	14	乱数	Random Value											*/
	NULL										,/*	15																	*/
	NULL										,/*	16																	*/
	NULL										,/*	17																	*/
	NULL										,/*	18	乱数	Random Value											*/
	NULL										,/*	19																	*/
	NULL										,/*	20																	*/
	NULL										,/*	21																	*/
	NULL										,/*	22																	*/
	NULL										,/*	23																	*/
	NULL										,/*	24																	*/
	NULL										,/*	25	乱数	Random Value											*/
	NULL										,/*	26																	*/
	NULL										,/*	27																	*/
	NULL										,/*	28																	*/
	NULL										,/*	29																	*/
	NULL										,/*	30																	*/
	NULL										,/*	31																	*/
	NULL										,/*	32																	*/
	NULL										,/*	33																	*/
	NULL										,/*	34																	*/
	NULL										,/*	35																	*/
	NULL										,/*	36																	*/
	NULL										,/*	37																	*/
	NULL										,/*	38																	*/
	NULL										,/*	39																	*/
	NULL										,/*	40																	*/
	NULL										,/*	41																	*/
	NULL										,/*	42																	*/
	NULL										,/*	43																	*/
	NULL										,/*	44																	*/
	NULL										,/*	45																	*/
	NULL										,/*	46																	*/
	NULL										,/*	47																	*/
	NULL										,/*	48																	*/
	NULL										,/*	49																	*/
	NULL										,/*	50	乱数	Random Value											*/
	NULL										,/*	51																	*/
	NULL										,/*	52																	*/
	NULL										,/*	53																	*/
	NULL										,/*	54																	*/
	NULL										,/*	55																	*/
	NULL										,/*	56																	*/
	NULL										,/*	57																	*/
	NULL										,/*	58																	*/
	NULL										,/*	59																	*/
	NULL										,/*	60																	*/
	NULL										,/*	61																	*/
	NULL										,/*	62																	*/
	NULL										,/*	63																	*/
	NULL										,/*	64																	*/
	NULL										,/*	65																	*/
	NULL										,/*	66																	*/
	NULL										,/*	67																	*/
	NULL										,/*	68																	*/
	NULL										,/*	69																	*/
	NULL										,/*	70																	*/
	NULL										,/*	71																	*/
	NULL										,/*	72																	*/
	NULL										,/*	73																	*/
	NULL										,/*	74																	*/
	NULL										,/*	75																	*/
	NULL										,/*	76																	*/
	NULL										,/*	77																	*/
	NULL										,/*	78																	*/
	NULL										,/*	79																	*/
	NULL										,/*	80																	*/
	NULL										,/*	81																	*/
	NULL										,/*	82																	*/
	NULL										,/*	83	乱数	Random Value											*/
	NULL										,/*	84																	*/
	NULL										,/*	85																	*/
	NULL										,/*	86																	*/
	NULL										,/*	87																	*/
	NULL										,/*	88																	*/
	NULL										,/*	89																	*/
	NULL										,/*	90																	*/
	NULL										,/*	91																	*/
	NULL										,/*	92																	*/
	NULL										,/*	93																	*/
	NULL										,/*	94																	*/
	NULL										,/*	95																	*/
	NULL										,/*	96																	*/
	NULL										,/*	97																	*/
	NULL										,/*	98																	*/
	NULL										,/*	99																	*/
	NULL										,/*	100																	*/
	NULL										,/*	101	UTC時間	UTCTime													*/
	NULL										,/*	102	日付	Date													*/
	NULL										,/*	103	乱数	Random Value											*/
	NULL										,/*	104	緯度	Latitude												*/
	NULL										,/*	105	経度	Longitude												*/
	NULL										,/*	106	高度	Altitude												*/
	NULL										,/*	107	ジオイド高	Geoidal Separation									*/
	NULL										,/*	108	対地速度	Horizontally Speed									*/
	NULL										,/*	109	垂直速度	Vertical Speed										*/
	NULL										,/*	110	乱数	Random Value											*/
	NULL										,/*	111	精度	Accuracy 												*/
	NULL										,/*	112	DOP情報	DOP														*/
	NULL										,/*	113	加速度	Acceleration(Plane)										*/
	NULL										,/*	114	加速度	Acceleration(Vertical)									*/
	NULL										,/*	115	ヨーレート&ピッチレート	Yaw Rate & Pitch Rate					*/
	NULL										,/*	116	ロールレート	Roll Rate										*/
	NULL										,/*	117																	*/
	NULL										,/*	118																	*/
	NULL										,/*	119																	*/
	NULL										,/*	120																	*/
	NULL										,/*	121																	*/
	NULL										,/*	122																	*/
	NULL										,/*	123																	*/
	NULL										,/*	124																	*/
	NULL										,/*	125																	*/
	NULL										,/*	126																	*/
	NULL										,/*	127																	*/
	NULL										,/*	128																	*/
	NULL										,/*	129																	*/
	NULL										,/*	130																	*/
	NULL										,/*	131																	*/
	NULL										,/*	132																	*/
	NULL										,/*	133																	*/
	NULL										,/*	134	乱数	Random Value											*/
	NULL										,/*	135																	*/
	NULL										,/*	136																	*/
	NULL										,/*	137																	*/
	NULL										,/*	138																	*/
	NULL										,/*	139																	*/
	NULL										,/*	140																	*/
	NULL										,/*	141																	*/
	NULL										,/*	142	乱数	Random Value											*/
	NULL										,/*	143																	*/
	NULL										,/*	144																	*/
	NULL										,/*	145																	*/
	NULL										,/*	146																	*/
	NULL										,/*	147																	*/
	NULL										,/*	148																	*/
	NULL										,/*	149																	*/
	NULL										,/*	150																	*/
	NULL										,/*	151																	*/
	NULL										,/*	152																	*/
	NULL										,/*	153																	*/
	NULL										,/*	154																	*/
	NULL										,/*	155																	*/
	NULL										,/*	156																	*/
	NULL										,/*	157																	*/
	NULL										,/*	158																	*/
	NULL										,/*	159																	*/
	NULL										,/*	160																	*/
	NULL										,/*	161																	*/
	NULL										,/*	162																	*/
	NULL										,/*	163																	*/
	NULL										,/*	164																	*/
	NULL										,/*	165																	*/
	NULL										,/*	166																	*/
	NULL										,/*	167																	*/
	NULL										,/*	168																	*/
	NULL										,/*	169																	*/
	NULL										,/*	170																	*/
	NULL										,/*	171																	*/
	NULL										,/*	172																	*/
	NULL										,/*	173																	*/
	NULL										,/*	174																	*/
	NULL										,/*	175																	*/
	NULL										,/*	176																	*/
	NULL										,/*	177																	*/
	NULL										,/*	178																	*/
	NULL										,/*	179																	*/
	NULL										,/*	180																	*/
	NULL										,/*	181																	*/
	NULL										,/*	182																	*/
	NULL										,/*	183																	*/
	NULL										,/*	184																	*/
	NULL										,/*	185																	*/
	NULL										,/*	186																	*/
	NULL										,/*	187																	*/
	NULL										,/*	188	乱数	Random Value											*/
	NULL										,/*	189																	*/
	NULL										,/*	190																	*/
	NULL										,/*	191																	*/
	NULL										,/*	192																	*/
	NULL										,/*	193																	*/
	NULL										,/*	194																	*/
	NULL										,/*	195																	*/
	NULL										,/*	196																	*/
	NULL										,/*	197																	*/
	NULL										,/*	198																	*/
	NULL										,/*	199																	*/
	NULL										,/*	200																	*/
	NULL										,/*	201	アクセル操作	Accelerator Operation							*/
	NULL										,/*	202	ブレーキ操作フラグ	Brake Flag									*/
	NULL										,/*	203	ブレーキ操作量	Brake Operation									*/
	NULL										,/*	204	シフト操作	Shift Operation										*/
	NULL										,/*	205	クラッチ操作	Clutch Pedal Operation							*/
	NULL										,/*	206	ハイブリッドパワー選択	Hybrid Power Select						*/
	NULL										,/*	207	乱数	Random Value											*/
	NULL										,/*	208	ステアリング	Steering										*/
	NULL										,/*	209	ウィンカー	Blinker												*/
	NULL										,/*	210	ワイパー	Wiper												*/
	NULL										,/*	211	乱数	Random Value											*/
	NULL										,/*	212	エアーコンディション	Air Condition							*/
	NULL										,/*	213	ヒーターコントロール	Heater Control							*/
	NULL										,/*	214	ドライビングサポート	Driving Support							*/
	NULL										,/*	215	ドア	Doors													*/
	NULL										,/*	216	ロック	Lock													*/
	NULL										,/*	217	ウィンドウ	Window												*/
	NULL										,/*	218	シート、シートベルト	Sitting and SeatBelt Status				*/
	NULL										,/*	219	ランプ	Lamp Status												*/
	NULL										,/*	220	起動	Power													*/
	NULL										,/*	221	エンジンスピード	Engine Rev									*/
	NULL										,/*	222	車速	Vehicle Speed											*/
	&get_signal_EncodeValue						,/*	223	高精度車速(フロント)	High Resolution Speed (Front)			*/
	&get_signal_EncodeValue						,/*	224	高精度車速(リア)	High Resolution Speed (Rear)				*/
	NULL										,/*	225	加速度(平面)	Acceleration(Plane)								*/
	NULL										,/*	226	加速度(垂直)	Acceleration(Vertical)							*/
	NULL										,/*	227	ヨーレート、ピッチレート	Yaw and Pitch Rate					*/
	NULL										,/*	228	ロールレート	Roll Rate										*/
	NULL										,/*	229	ヨー、ピッチ角	Yaw and Pitch Angle								*/
	NULL										,/*	230	ロール角	Roll Angle											*/
	NULL										,/*	231	ユニット温度	Unit Temperature								*/
	NULL										,/*	232	フロントタイヤ圧	Tire Pressure (Front)						*/
	NULL										,/*	233	リアタイヤ圧	Tire Pressure (Rear)							*/
	NULL										,/*	234	エネルギインジケータ	Energy Indicator						*/
	NULL										,/*	235	始動後燃料噴射量積算値	Fuel Consumption						*/
	NULL										,/*	236	始動後電力消費量積算値	Electricity Consumption (since restart)	*/
	NULL										,/*	237	始動後気体燃料噴射量積算値	Gaseous Fuel Consumption			*/
	NULL										,/*	238	ハイブリッド走行状態	Hyblid Drive Mode						*/
	NULL										,/*	239	ODOメータ走行距離	Odometer									*/
	NULL										,/*	240	始動後走行距離	Distance from Restart							*/
	NULL										,/*	241	エンジン走行距離	Distance of Engine Drive					*/
	NULL										,/*	242	EV走行距離	Distance of EV Drive								*/
	NULL										,/*	243	ガスエンジン走行距離	Distance of Gaseous Fuel				*/
	NULL										,/*	244	走行可能距離	Drivable Distance(ALL)							*/
	NULL										,/*	245	エンジン走行可能距離	Drivable Distance of Fuel				*/
	NULL										,/*	246	EV走行可能距離	Drivable Distance of Electricity				*/
	NULL										,/*	247	ガスエンジン走行可能距離	Drivable Distance of Gaseous Fuel	*/
	NULL										,/*	248	供給口開閉状態	Charging Lid Status								*/
	NULL										,/*	249	充電ケーブル接続状態	Charging Lid Status						*/
	NULL										,/*	250	充放電状態	Charging and Supplying Status						*/
	NULL										,/*	251	最近接障害物距離	Rader Signal								*/
	NULL										,/*	252	最接近障害物衝突時間	Minimum Time to Collision 				*/
	NULL										,/*	253	出力トルク	Output Torque										*/
	NULL										,/*	254																	*/
	NULL										,/*	255																	*/
	NULL										,/*	256																	*/
	NULL										,/*	257																	*/
	NULL										,/*	258																	*/
	NULL										,/*	259	出力アップダウントルク	Output Torque UpDwon					*/
	NULL										,/*	260																	*/
	NULL										,/*	261																	*/
	NULL										,/*	262																	*/
	NULL										,/*	263																	*/
	NULL										,/*	264																	*/
	NULL										,/*	265																	*/
	NULL										,/*	266																	*/
	NULL										,/*	267																	*/
	NULL										,/*	268																	*/
	NULL										,/*	269																	*/
	NULL										,/*	270																	*/
	NULL										,/*	271																	*/
	NULL										,/*	272																	*/
	NULL										,/*	273																	*/
	NULL										,/*	274																	*/
	NULL										,/*	275																	*/
	NULL										,/*	276																	*/
	NULL										,/*	277																	*/
	NULL										,/*	278																	*/
	NULL										,/*	279																	*/
	NULL										,/*	280																	*/
	NULL										,/*	281																	*/
	NULL										,/*	282																	*/
	NULL										,/*	283																	*/
	NULL										,/*	284	乱数	Random Value											*/
	NULL										,/*	285																	*/
	NULL										,/*	286																	*/
	NULL										,/*	287																	*/
	NULL										,/*	288																	*/
	NULL										,/*	289																	*/
	NULL										,/*	290																	*/
	NULL										,/*	291																	*/
	NULL										,/*	292																	*/
	NULL										,/*	293																	*/
	NULL										,/*	294																	*/
	NULL										,/*	295																	*/
	NULL										,/*	296																	*/
	NULL										,/*	297																	*/
	NULL										,/*	298																	*/
	NULL										,/*	299																	*/
	NULL										,/*	300																	*/
	NULL										,/*	301																	*/
	NULL										,/*	302																	*/
	NULL										,/*	303																	*/
	NULL										,/*	304	乱数	Random Value											*/
	NULL										,/*	305																	*/
	NULL										,/*	306																	*/
	NULL										,/*	307																	*/
	NULL										,/*	308																	*/
	NULL										,/*	309																	*/
	NULL										,/*	310																	*/
	NULL										,/*	311																	*/
	NULL										,/*	312																	*/
	NULL										,/*	313																	*/
	NULL										,/*	314																	*/
	NULL										,/*	315																	*/
	NULL										,/*	316																	*/
	NULL										,/*	317																	*/
	NULL										,/*	318																	*/
	NULL										,/*	319																	*/
	NULL										,/*	320																	*/
	NULL										,/*	321																	*/
	NULL										,/*	322																	*/
	NULL										,/*	323																	*/
	NULL										,/*	324																	*/
	NULL										,/*	325																	*/
	NULL										,/*	326																	*/
	NULL										,/*	327																	*/
	NULL										,/*	328																	*/
	NULL										,/*	329																	*/
	NULL										,/*	330																	*/
	NULL										,/*	331																	*/
	NULL										,/*	332																	*/
	NULL										,/*	333																	*/
	NULL										,/*	334																	*/
	NULL										,/*	335																	*/
	NULL										,/*	336																	*/
	NULL										,/*	337	乱数	Random Value											*/
	NULL										,/*	338																	*/
	NULL										,/*	339																	*/
	NULL										,/*	340	乱数	Random Value											*/
	NULL										,/*	341																	*/
	NULL										,/*	342																	*/
	NULL										,/*	343																	*/
	NULL										,/*	344																	*/
	NULL										,/*	345																	*/
	NULL										,/*	346																	*/
	NULL										,/*	347																	*/
	NULL										,/*	348																	*/
	NULL										,/*	349																	*/
	NULL										,/*	350																	*/
	NULL										,/*	351																	*/
	NULL										,/*	352																	*/
	NULL										,/*	353																	*/
	NULL										,/*	354																	*/
	NULL										,/*	355																	*/
	NULL										,/*	356	乱数	Random Value											*/
	NULL										,/*	357																	*/
	NULL										,/*	358																	*/
	NULL										,/*	359																	*/
	NULL										,/*	360																	*/
	NULL										,/*	361																	*/
	NULL										,/*	362																	*/
	NULL										,/*	363																	*/
	NULL										,/*	364																	*/
	NULL										,/*	365																	*/
	NULL										,/*	366																	*/
	NULL										,/*	367																	*/
	NULL										,/*	368																	*/
	NULL										,/*	369																	*/
	NULL										,/*	370	乱数	Random Value											*/
	NULL										,/*	371	乱数	Random Value											*/
	NULL										,/*	372																	*/
	NULL										,/*	373																	*/
	NULL										,/*	374																	*/
	NULL										,/*	375																	*/
	NULL										,/*	376																	*/
	NULL										,/*	377																	*/
	NULL										,/*	378																	*/
	NULL										,/*	379																	*/
	NULL										,/*	380																	*/
	NULL										,/*	381																	*/
	NULL										,/*	382																	*/
	NULL										,/*	383																	*/
	NULL										,/*	384																	*/
	NULL										,/*	385																	*/
	NULL										,/*	386																	*/
	NULL										,/*	387																	*/
	NULL										,/*	388																	*/
	NULL										,/*	389																	*/
	NULL										,/*	390																	*/
	NULL										,/*	391																	*/
	NULL										,/*	392																	*/
	NULL										,/*	393																	*/
	NULL										,/*	394																	*/
	NULL										,/*	395																	*/
	NULL										,/*	396																	*/
	NULL										,/*	397																	*/
	NULL										,/*	398																	*/
	NULL										,/*	399	乱数	Random Value											*/
	NULL										,/*	400																	*/
	NULL										,/*	401																	*/
	NULL										,/*	402																	*/
	NULL										,/*	403	乱数	Random Value											*/
	NULL										,/*	404																	*/
	NULL										,/*	405																	*/
	NULL										,/*	406																	*/
	NULL										,/*	407																	*/
	NULL										,/*	408																	*/
	NULL										,/*	409																	*/
	NULL										,/*	410																	*/
	NULL										,/*	411																	*/
	NULL										,/*	412																	*/
	NULL										,/*	413																	*/
	NULL										,/*	414																	*/
	NULL										,/*	415																	*/
	NULL										,/*	416																	*/
	NULL										,/*	417																	*/
	NULL										,/*	418																	*/
	NULL										,/*	419																	*/
	NULL										,/*	420																	*/
	NULL										,/*	421																	*/
	NULL										,/*	422	乱数	Random Value											*/
	NULL										,/*	423																	*/
	NULL										,/*	424																	*/
	NULL										,/*	425																	*/
	NULL										,/*	426																	*/
	NULL										,/*	427																	*/
	NULL										,/*	428																	*/
	NULL										,/*	429																	*/
	NULL										,/*	430	乱数	Random Value											*/
	NULL										,/*	431																	*/
	NULL										,/*	432																	*/
	NULL										,/*	433																	*/
	NULL										,/*	434																	*/
	NULL										,/*	435																	*/
	NULL										,/*	436																	*/
	NULL										,/*	437																	*/
	NULL										,/*	438																	*/
	NULL										,/*	439																	*/
	NULL										,/*	440																	*/
	NULL										,/*	441																	*/
	NULL										,/*	442																	*/
	NULL										,/*	443																	*/
	NULL										,/*	444																	*/
	NULL										,/*	445																	*/
	NULL										,/*	446																	*/
	NULL										,/*	447																	*/
	NULL										,/*	448																	*/
	NULL										,/*	449																	*/
	NULL										,/*	450																	*/
	NULL										,/*	451																	*/
	NULL										,/*	452																	*/
	NULL										,/*	453																	*/
	NULL										,/*	454																	*/
	NULL										,/*	455	乱数	Random Value											*/
	NULL										,/*	456																	*/
	NULL										,/*	457																	*/
	NULL										,/*	458																	*/
	NULL										,/*	459																	*/
	NULL										,/*	460																	*/
	NULL										,/*	461																	*/
	NULL										,/*	462																	*/
	NULL										,/*	463																	*/
	NULL										,/*	464																	*/
	NULL										,/*	465	乱数	Random Value											*/
	NULL										,/*	466																	*/
	NULL										,/*	467																	*/
	NULL										,/*	468																	*/
	NULL										,/*	469																	*/
	NULL										,/*	470																	*/
	NULL										,/*	471																	*/
	NULL										,/*	472																	*/
	NULL										,/*	473																	*/
	NULL										,/*	474																	*/
	NULL										,/*	475																	*/
	NULL										,/*	476																	*/
	NULL										,/*	477																	*/
	NULL										,/*	478																	*/
	NULL										,/*	479																	*/
	NULL										,/*	480																	*/
	NULL										,/*	481																	*/
	NULL										,/*	482																	*/
	NULL										,/*	483																	*/
	NULL										,/*	484																	*/
	NULL										,/*	485																	*/
	NULL										,/*	486																	*/
	NULL										,/*	487																	*/
	NULL										,/*	488																	*/
	NULL										,/*	489																	*/
	NULL										,/*	490																	*/
	NULL										,/*	491																	*/
	NULL										,/*	492																	*/
	NULL										,/*	493																	*/
	NULL										,/*	494																	*/
	NULL										,/*	495																	*/
	NULL										,/*	496																	*/
	NULL										,/*	497																	*/
	NULL										,/*	498																	*/
	NULL										,/*	499																	*/
	NULL										,/*	500																	*/
	NULL										,/*	501	SOCセグメント PRIUS for PriusCUP for TMA	SOC(Segment)		*/
	NULL										,/*	502	SOCセグメント 748A個別	SOC(Segment)							*/
	NULL										,/*	503	ダイアグコード	Diagnostic Code									*/
	NULL										,/*	504	ワーニング	 Warning											*/
	NULL										,/*	505	プラグエラー	Plug Err										*/
	NULL										,/*	506																	*/
	NULL										,/*	507																	*/
	NULL										,/*	508																	*/
	NULL										,/*	509																	*/
	NULL										,/*	510																	*/
	NULL										,/*	511																	*/
	NULL										,/*	512																	*/
	NULL										,/*	513																	*/
	NULL										,/*	514																	*/
	NULL										,/*	515																	*/
	NULL										,/*	516																	*/
	NULL										,/*	517																	*/
	NULL										,/*	518																	*/
	NULL										,/*	519																	*/
	NULL										,/*	520																	*/
	NULL										,/*	521																	*/
	NULL										,/*	522																	*/
	NULL										,/*	523																	*/
	NULL										,/*	524																	*/
	NULL										,/*	525																	*/
	NULL										,/*	526																	*/
	NULL										,/*	527																	*/
	NULL										,/*	528																	*/
	NULL										,/*	529																	*/
	NULL										,/*	530																	*/
	NULL										,/*	531																	*/
	NULL										,/*	532																	*/
	NULL										,/*	533																	*/
	NULL										,/*	534																	*/
	NULL										,/*	535																	*/
	NULL										,/*	536																	*/
	NULL										,/*	537	乱数	Random Value											*/
	NULL										,/*	538																	*/
	NULL										,/*	539																	*/
	NULL										,/*	540																	*/
	NULL										,/*	541																	*/
	NULL										,/*	542																	*/
	NULL										,/*	543																	*/
	NULL										,/*	544																	*/
	NULL										,/*	545																	*/
	NULL										,/*	546																	*/
	NULL										,/*	547																	*/
	NULL										,/*	548																	*/
	NULL										,/*	549																	*/
	NULL										,/*	550																	*/
	NULL										,/*	551																	*/
	NULL										,/*	552																	*/
	NULL										,/*	553																	*/
	NULL										,/*	554																	*/
	NULL										,/*	555																	*/
	NULL										,/*	556																	*/
	NULL										,/*	557	乱数	Random Value											*/
	NULL										,/*	558																	*/
	NULL										,/*	559																	*/
	NULL										,/*	560																	*/
	NULL										,/*	561																	*/
	NULL										,/*	562																	*/
	NULL										,/*	563	乱数	Random Value											*/
	NULL										,/*	564																	*/
	NULL										,/*	565	乱数	Random Value											*/
	NULL										,/*	566																	*/
	NULL										,/*	567																	*/
	NULL										,/*	568																	*/
	NULL										,/*	569																	*/
	NULL										,/*	570																	*/
	NULL										,/*	571																	*/
	NULL										,/*	572																	*/
	NULL										,/*	573																	*/
	NULL										,/*	574																	*/
	NULL										,/*	575																	*/
	NULL										,/*	576																	*/
	NULL										,/*	577																	*/
	NULL										,/*	578																	*/
	NULL										,/*	579																	*/
	NULL										,/*	580																	*/
	NULL										,/*	581																	*/
	NULL										,/*	582																	*/
	NULL										,/*	583																	*/
	NULL										,/*	584																	*/
	NULL										,/*	585																	*/
	NULL										,/*	586																	*/
	NULL										,/*	587																	*/
	NULL										,/*	588																	*/
	NULL										,/*	589																	*/
	NULL										,/*	590																	*/
	NULL										,/*	591																	*/
	NULL										,/*	592																	*/
	NULL										,/*	593																	*/
	NULL										,/*	594	乱数	Random Value											*/
	NULL										,/*	595																	*/
	NULL										,/*	596																	*/
	NULL										,/*	597																	*/
	NULL										,/*	598																	*/
	NULL										,/*	599																	*/
	NULL										,/*	600																	*/
	NULL										,/*	601	吸気圧	Intake Manifold Pressure								*/
	NULL										,/*	602	エンジン油圧	Engine Oil Pressure								*/
	NULL										,/*	603	燃料圧力	Fuel Pressure										*/
	NULL										,/*	604	エンジン水温（拡張）	Engine Coolant Temperture（Ext）		*/
	NULL										,/*	605	エンジン水温（拡張）	Engine Coolant Temperture（Ext）		*/
	NULL										,/*	606	エンジン油温	Engine Oil Temperture							*/
	NULL										,/*	607	排気温度	Exhaust Gas Temperture								*/
	NULL										,/*	608	電圧	Volts													*/
	NULL										,/*	609	空燃比	Air-Fuel Ratio											*/
	NULL										,/*	610																	*/
	NULL										,/*	611																	*/
	NULL										,/*	612																	*/
	NULL										,/*	613																	*/
	NULL										,/*	614																	*/
	NULL										,/*	615																	*/
	NULL										,/*	616	乱数	Random Value											*/
	NULL										,/*	617																	*/
	NULL										,/*	618																	*/
	NULL										,/*	619																	*/
	NULL										,/*	620																	*/
	NULL										,/*	621																	*/
	NULL										,/*	622																	*/
	NULL										,/*	623																	*/
	NULL										,/*	624																	*/
	NULL										,/*	625																	*/
	NULL										,/*	626																	*/
	NULL										,/*	627																	*/
	NULL										,/*	628																	*/
	NULL										,/*	629																	*/
	NULL										,/*	630																	*/
	NULL										,/*	631																	*/
	NULL										,/*	632																	*/
	NULL										,/*	633																	*/
	NULL										,/*	634																	*/
	NULL										,/*	635																	*/
	NULL										,/*	636	乱数	Random Value											*/
	NULL										,/*	637																	*/
	NULL										,/*	638																	*/
	NULL										,/*	639																	*/
	NULL										,/*	640	車速	Vehicle Speed											*/
	NULL										,/*	641	エンジン回転数	Engine Rev										*/
	NULL										,/*	642	吸気圧	Intake Manifold Pressure								*/
	NULL										,/*	643	エンジン水温	Engine Coolant Temperture						*/
	NULL										,/*	644	電圧	Volts													*/
	NULL										,/*	645	スロットル開度	Throttle										*/
	NULL										,/*	646	吸気温度	IntakeTemperature									*/
	NULL										,/*	647																	*/
	NULL										,/*	648																	*/
	NULL										,/*	649																	*/
	NULL										,/*	650																	*/
	NULL										,/*	651																	*/
	NULL										,/*	652																	*/
	NULL										,/*	653																	*/
	NULL										,/*	654																	*/
	NULL										,/*	655																	*/
	NULL										,/*	656																	*/
	NULL										,/*	657																	*/
	NULL										,/*	658																	*/
	NULL										,/*	659																	*/
	NULL										,/*	660																	*/
	NULL										,/*	661																	*/
	NULL										,/*	662																	*/
	NULL										,/*	663																	*/
	NULL										,/*	664																	*/
	NULL										,/*	665																	*/
	NULL										,/*	666																	*/
	NULL										,/*	667																	*/
	NULL										,/*	668																	*/
	NULL										,/*	669																	*/
	NULL										,/*	670																	*/
	NULL										,/*	671																	*/
	NULL										,/*	672	乱数	Random Value											*/
	NULL										,/*	673	乱数	Random Value											*/
	NULL										,/*	674																	*/
	NULL										,/*	675																	*/
	NULL										,/*	676																	*/
	NULL										,/*	677																	*/
	NULL										,/*	678																	*/
	NULL										,/*	679																	*/
	NULL										,/*	680																	*/
	NULL										,/*	681																	*/
	NULL										,/*	682	乱数	Random Value											*/
	NULL										,/*	683																	*/
	NULL										,/*	684																	*/
	NULL										,/*	685																	*/
	NULL										,/*	686																	*/
	NULL										,/*	687																	*/
	NULL										,/*	688																	*/
	NULL										,/*	689																	*/
	NULL										,/*	690	吸気圧	Intake Manifold Pressure								*/
	NULL										,/*	691	エンジン水温	Engine Coolant Temperture						*/
	NULL										,/*	692	スロットル開度	Throttle										*/
	NULL										,/*	693	吸気温度	IntakeTemperature									*/
	NULL										,/*	694																	*/
	NULL										,/*	695																	*/
	NULL										,/*	696																	*/
	NULL										,/*	697																	*/
	NULL										,/*	698																	*/
	NULL										,/*	699																	*/
};



SIGNAL_LOCAL UI_8	S_RandInitialFlag;					/*乱数初期化				*/
SIGNAL_LOCAL SI_32	S_encodeKey[DEVICE_TYPE_NUM];		/*エンコードキー			*/


/********************************************************/
/*	外部関数											*/
/********************************************************/

/********************************************************/
/* 関数名称		: Signal_control_Init					*/
/* 機能の説明	: 公開信号制御初期化					*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Signal_control_Init(void)
{
	UI_8 i;
	S_RandInitialFlag = 1;			/*乱数初期化*/
	for(i = 0 ; i < DEVICE_TYPE_NUM ; i++)
	{
		S_encodeKey[i] = 0;
	}
}


/********************************************************/
/* 関数名称		: Signal_control_GetSignalVersion		*/
/* 機能の説明	: 公開信号バージョン					*/
/* 引数			: void									*/
/* 戻り値		: 公開信号バージョン					*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
UI_32 Signal_control_GetSignalVersion(void)
{
	E_SYS_CTRL_STATUS	sys_stat;										/* 動作状態 */
	sys_stat = System_control_GetStatus();
	if(sys_stat == SYS_OPERATION_SIMULATION)
	{
		/*シミュレーション		*/
		/* SDからバージョン取得 */
		return Sd_control_GetSignalVersion();
	}
	return SIGNAL_VERSION;
}

/********************************************************/
/* 関数名称		: Signal_control_GetSignalInfo			*/
/* 機能の説明	: 公開信号取得							*/
/* 引数			: (I/ )deviceType 		デバイス種別	*/
/* 				: (I/ )req_id 			信号ID			*/
/*				: ( /O)signal_value	信号値				*/
/* 戻り値		: 正常	:N_OK							*/
/*				: 異常	:N_NG							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Signal_control_GetSignalInfo(E_SIGNAL_DEVICE_TYPE deviceType, UI_16 req_id, SI_32 *signal_value)
{
	SI_32 key;
	E_SYS_CTRL_STATUS	sys_stat;										/* 動作状態 */
	E_SIGNAL_DEVICE_TYPE orgDeviceType = deviceType;
	if( (deviceType != DEVICE_TYPE_SD) && (deviceType != DEVICE_TYPE_BT) && (deviceType != DEVICE_TYPE_CAN) )
	{
		return N_NG;
	}
	if( deviceType == DEVICE_TYPE_CAN )
	{
		/*デバイスタイプがCANならBTに置き換える*/
		deviceType = DEVICE_TYPE_BT;
	}
	/* 動作状態取得 */
	sys_stat = System_control_GetStatus();
	if(sys_stat == SYS_OPERATION_SIMULATION)
	{
		/*シミュレーション		*/
		/* SDから信号取得 */
		return Sd_control_GetSignalsInfo(1, &req_id, signal_value);
	}
	else
	{
		/*シミュレーション以外	*/
		if( (SIGNAL_ID_MIN <= req_id)  && (req_id <= SIGNAL_ID_MAX) )
		{
			/* 信号ID:有効 */
			C_SignalInfoFuncTbl[ req_id ][ (UI_8)deviceType ](signal_value);

			if(C_EncodeFuncTbl[ req_id ] != NULL)
			{
				/* 暗号化必要(置き換え前の引数のデバイスタイプを使用） */
				(void)Signal_control_GetEncodeKey(orgDeviceType, &key);
				C_EncodeFuncTbl[ req_id ](req_id, signal_value, key);
			}
		}
		else
		{
			/* 信号ID:無効 */
			(void)get_signal_InvalidValue(signal_value);
		}
	}
	return N_OK;
	
}


/********************************************************/
/* 関数名称		: Signal_control_GetSignalsInfo			*/
/* 機能の説明	: 公開信号取得(複数信号)				*/
/* 引数			: (I/ )deviceType 		デバイス種別	*/
/* 				: (I/ )req_id_count	信号ID数			*/
/* 				: (I/ )req_ids 		信号ID（複数）		*/
/*				: ( /O)signal_values	信号値（複数）	*/
/* 戻り値		: 正常	:N_OK							*/
/*				: 異常	:N_NG							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Signal_control_GetSignalsInfo(E_SIGNAL_DEVICE_TYPE deviceType, UI_8 req_id_count, const UI_16* req_ids, SI_32 *signal_values)
{
	UI_8 i;
	SI_32 key;
	E_SYS_CTRL_STATUS	sys_stat;										/* 動作状態 */
	E_SIGNAL_DEVICE_TYPE orgDeviceType = deviceType;
	if( (deviceType != DEVICE_TYPE_SD) && (deviceType != DEVICE_TYPE_BT) && (deviceType != DEVICE_TYPE_CAN) )
	{
		return N_NG;
	}
	if( deviceType == DEVICE_TYPE_CAN )
	{
		/*デバイスタイプがCANならBTに置き換える*/
		deviceType = DEVICE_TYPE_BT;
	}
	/* 動作状態取得 */
	sys_stat = System_control_GetStatus();
	if(sys_stat == SYS_OPERATION_SIMULATION)
	{
		/*シミュレーション		*/
		/* SDから信号取得 */
		return Sd_control_GetSignalsInfo(req_id_count, req_ids, signal_values);
	}
	else
	{
		/*シミュレーション以外	*/
		for( i = 0 ; i < req_id_count ; i ++ )
		{
			if( (SIGNAL_ID_MIN <= req_ids[i])  && (req_ids[i] <= SIGNAL_ID_MAX) )
			{
				/* 信号ID:有効 */
				C_SignalInfoFuncTbl[ req_ids[i] ][ (UI_8)deviceType ](&signal_values[i]);
				
				if(C_EncodeFuncTbl[ req_ids[i] ] != NULL)
				{
					/* 暗号化必要(置き換え前の引数のデバイスタイプを使用） */
					(void)Signal_control_GetEncodeKey(orgDeviceType, &key);
					C_EncodeFuncTbl[ req_ids[i] ](req_ids[i], &signal_values[i], key);
				}
			}
			else
			{
				/* 信号ID:無効 */
				(void)get_signal_InvalidValue(&signal_values[i]);
			}
		}
	}
	return N_OK;
}


/********************************************************/
/* 関数名称		: Signal_control_GetSignalList			*/
/* 機能の説明	: 公開信号ID一覧取得					*/
/* 引数			: (I/ )deviceType 		デバイス種別	*/
/*				: (I/ )idtype			信号種別		*/
/*				: ( /O)signal_Num		信号数			*/
/*				: ( /O)signal_List		信号リスト		*/
/* 戻り値		: 正常	:N_OK							*/
/*				: 異常	:N_NG							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Signal_control_GetSignalList(E_SIGNAL_DEVICE_TYPE deviceType, E_SIGNAL_ID_TYPE idtype, UI_8 *signal_Num, const UI_16 **signal_List)
{
	E_SYS_CTRL_STATUS	sys_stat;										/* 動作状態 */
	SI_8 ret = N_NG;
	if( (deviceType != DEVICE_TYPE_SD) && (deviceType != DEVICE_TYPE_BT) && (deviceType != DEVICE_TYPE_CAN) )
	{
		return N_NG;
	}
	if( deviceType == DEVICE_TYPE_CAN )
	{
		/*デバイスタイプがCANならBTに置き換える*/
		deviceType = DEVICE_TYPE_BT;
	}
	
	/* 動作状態取得 */
	sys_stat = System_control_GetStatus();
	if(sys_stat == SYS_OPERATION_SIMULATION)
	{
		/*シミュレーション		*/
		/* SDから信号取得 */
		return Sd_control_GetSignalList(idtype, signal_Num, signal_List);
	}
	else
	{
		/*シミュレーション以外	*/
		if( deviceType == DEVICE_TYPE_SD )
		{
			/* SD */
			if( idtype == ID_TYPE_FIXED_ID )
			{
				/* 固定値 */
				*signal_List = signalFixedListTable_SD;
				*signal_Num = sizeof(signalFixedListTable_SD) / sizeof(UI_16);
				ret = N_OK;
			}
			else if( idtype == ID_TYPE_VARIABLE_ID )
			{
				/* 変数値 */
				*signal_List = signalValiableListTable_SD;
				*signal_Num = sizeof(signalValiableListTable_SD) / sizeof(UI_16);
				ret = N_OK;
			}
			else {
				/* nop */
			}
		}
		else if( deviceType == DEVICE_TYPE_BT )
		{
			/* BT */
			if( idtype == ID_TYPE_FIXED_ID )
			{
				/* 固定値 */
				*signal_List = signalFixedListTable_BT;
				*signal_Num = sizeof(signalFixedListTable_BT) / sizeof(UI_16);
				ret = N_OK;
			}
			else if( idtype == ID_TYPE_VARIABLE_ID )
			{
				/* 変数値 */
				*signal_List = signalValiableListTable_BT;
				*signal_Num = sizeof(signalValiableListTable_BT) / sizeof(UI_16);
				ret = N_OK;
			}
			else {
				/* nop */
			}
		}
		else {
			/* nop */
		}
	}
	return ret;
}

/********************************************************/
/* 関数名称		: Signal_control_GetRandomValue			*/
/* 機能の説明	: 乱数取得								*/
/* 引数			: void									*/
/* 戻り値		: 乱数									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_32 Signal_control_GetRandomValue(void)
{
	SI_32 randomValue;
	(void)get_signal_RandomValue(&randomValue);
	return randomValue;
}


/********************************************************/
/* 関数名称		: Signal_control_GetEncodeKey			*/
/* 機能の説明	: 暗号化キー取得						*/
/* 引数			: (I/ )deviceType 		デバイス種別	*/
/*				: ( /O)key				暗号化用キー	*/
/* 戻り値		: 正常	:N_OK							*/
/*				: 異常	:N_NG							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Signal_control_GetEncodeKey(E_SIGNAL_DEVICE_TYPE deviceType, SI_32 *key)
{
	E_SYS_CTRL_STATUS	sys_stat;										/* 動作状態 */
	if( (deviceType != DEVICE_TYPE_SD) && (deviceType != DEVICE_TYPE_BT) && (deviceType != DEVICE_TYPE_CAN) )
	{
		return N_NG;
	}
	sys_stat = System_control_GetStatus();
	if(sys_stat == SYS_OPERATION_SIMULATION)
	{
		/*シミュレーション		*/
		/* SDから信号取得 */
		return Sd_control_GetEncodeKey(key);
	}
	*key = S_encodeKey[deviceType];

	return N_OK;
}


/********************************************************/
/* 関数名称		: Signal_control_SetEncodeKey			*/
/* 機能の説明	: 暗号化キー設定						*/
/* 引数			: (I/ )deviceType 		デバイス種別	*/
/*				: (I/ )key				暗号化用キー	*/
/* 戻り値		: 正常	:N_OK							*/
/*				: 異常	:N_NG							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SI_8 Signal_control_SetEncodeKey(E_SIGNAL_DEVICE_TYPE deviceType, SI_32 key)
{
	if( (deviceType != DEVICE_TYPE_SD) && (deviceType != DEVICE_TYPE_BT) && (deviceType != DEVICE_TYPE_CAN) )
	{
		return N_NG;
	}
	S_encodeKey[deviceType] = key;
	
	return N_OK;
}

/********************************************************/
/* 関数名称		: Signal_control_isCanEnable			*/
/* 機能の説明	: CAN信号受信済判定						*/
/* 引数			: void									*/
/* 戻り値		: TRUE:受信済	 FALSE:未受信			*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
B_8 Signal_control_isCanEnable(void)
{
#ifdef _VDV_APP
	T_Can_ctrl_tcan_info* pCanInfo;
	/* CAN受信済みか判定 */
	pCanInfo = Can_control_GetTCanInfo();
	if(	(pCanInfo->en_brand_flag == 1) && 		/*ブランドフラグ*/
		(pCanInfo->en_destination == 1) && 		/*仕向*/
		(pCanInfo->en_vehicle_info == 1) && 	/*車種情報*/
		(pCanInfo->en_transmission_type == 1) )	/*トランスミッション種別*/
	{
		/* CAN受信済*/
		return N_TRUE;
	}
	/* CAN未受信 */
	return N_FALSE;
#else /* _VDV_APP */
	return N_TRUE;
#endif /* _VDV_APP */
}

/********************************************************/
/* 関数名称		: Signal_control_isGnssEnable			*/
/* 機能の説明	: GNSS信号受信済判定					*/
/* 引数			: void									*/
/* 戻り値		: TRUE:受信済	 FALSE:未受信			*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
B_8 Signal_control_isGnssEnable(void)
{
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo;
	/*NAV-PVT取得*/	
	pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	/* GNSS受信済みか判定 */
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:受信済み */
		if( (pGnssHnrInfo->gpsFix == GNSS_GSPFIX_2DFIX) || 
			(pGnssHnrInfo->gpsFix == GNSS_GSPFIX_3DFIX) || 
			(pGnssHnrInfo->gpsFix == GNSS_GSPFIX_GPS_AND_DEADRECKONING) ||
			(pGnssHnrInfo->gpsFix == GNSS_GSPFIX_DEADRECKONIN_ONLY)		)
		{
			/* 測位 or デッドレコニング:自律航法 */
			/* GNSS受信済 */
			return N_TRUE;
		}
	}
	/* GNSS未受信 */
	return N_FALSE;
}

/********************************************************/
/* 関数名称		: get_signal_EncodeValue				*/
/* 機能の説明	: 暗号化計算値取得						*/
/* 引数			: (I/ )req_id 			信号ID			*/
/*				: (I/O)signal_value	信号値				*/
/*				: (I/ )key				暗号化キー		*/
/* 戻り値		: 正常	:N_OK							*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EncodeValue(UI_16 req_id, SI_32* signal_value, SI_32 key)
{
    UI_32 val;
	SI_32 srcVal = *signal_value;
    UI_16 shift = req_id % 32;
    if (shift == 0){
        val = (UI_32)key;
    }
    else{
        val = ((UI_32)key << shift) | ((UI_32)key >> (32 - shift));
    }
    *signal_value = (SI_32)(val ^ (UI_32)srcVal);
    return 0;
}


/********************************************************/
/* 関数名称		: get_signal_InvalidValue				*/
/* 機能の説明	: 無効値取得							*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_InvalidValue(SI_32 *out)
{
	*out = (SI_32)SIGNAL_VALUE_IGNORE_32;
	return 0;
}

/********************************************************/
/* 関数名称		: get_signal_RandomValue				*/
/* 機能の説明	: 乱数値取得							*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_RandomValue(SI_32 *out)
{
	UI_32 val;
	SI_32 outVal;
	UI_8 shift;
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(S_RandInitialFlag == 1)
	{
		/* 未初期化 */
		if(pGnssHnrInfo != NULL)
		{
			/* NAV-HNR:有効 */
			srand((UI_16)((pGnssHnrInfo->hour * 10000) + (pGnssHnrInfo->min * 100) + pGnssHnrInfo->sec));
		}
		else
		{
			/* NAV-HNR:無効 */
			srand((UI_16)Timer_ctrl_GetTime1ms());
		}
		S_RandInitialFlag = 0;
	}
	do{
		/* 乱数生成（randで取得できる値は0x7FFまでなので２回取得＋ビットシフトさせる） */
		val = ((UI_32)rand() + ((UI_32)rand() << 16));
		shift = (UI_8)(rand() % 32);
		outVal = (SI_32)((val << shift) | (val >> (32 - shift)));
		/* フェールとして乱数が０ならやり直す */
	}while(outVal == 0);
    *out = outVal;
    
	return 0;
}

/********************************************************/
/* 関数名称		: get_signal_CangatewayEcuId			*/
/* 機能の説明	: CANGatewayECUID取得					*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_CangatewayEcuId(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_brand_flag == 1)
	{
		/* ブランドフラグ:有効 */
		if(pCanInfo->brand_flag == CAN_BRAND_FLAG_TMC)
		{
			/*	ブランドフラグ:TMC												*/
			*out = (SI_32)(SIGNAL_VALUE_CANGATEWAYECUID | ((UI_32)SIGNAL_VALUE_VALIDFLAG_VALID << 24));
		}
		else
		{
			/*	ブランドフラグ:その他											*/
			*out = (SI_32)(SIGNAL_VALUE_CANGATEWAYECUID | ((UI_32)SIGNAL_VALUE_VALIDFLAG_INVALID << 24));
		}
	}
	else
	{
		/* ブランドフラグ:無効 */
		*out = (SI_32)(SIGNAL_VALUE_CANGATEWAYECUID | ((UI_32)SIGNAL_VALUE_VALIDFLAG_INIT << 24));
	}
	return 0;
}

/********************************************************/
/* 関数名称		: get_signal_VehicleNameHighOrder		*/
/* 機能の説明	: 車両名称(先頭4文字)取得				*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_VehicleNameHighOrder(SI_32 *out)
{
	(void)memcpy(out, SIGNAL_VALUE_VEHICLENAMEHIGHORDER, 4);
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_VehicleNameLowOrder		*/
/* 機能の説明	: 車両名称(後方4文字)取得				*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_VehicleNameLowOrder(SI_32 *out)
{
	(void)memcpy(out, SIGNAL_VALUE_VEHICLENAMELOWORDER, 4);
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_Destination				*/
/* 機能の説明	: 仕向け地取得							*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Destination(SI_32 *out)
{
	(void)memcpy(out, SIGNAL_VALUE_DESTINATION, 4);
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_DriverPosition				*/
/* 機能の説明	: ドライバーポジション取得				*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_DriverPosition(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();

	if(pCanInfo->en_destination == 1)
	{
		switch(pCanInfo->destination)
		{
			case CAN_DESTINATION_JP:	/*	仕向け:日本国内				*/
			case CAN_DESTINATION_UK:	/*	仕向け:イギリス				*/
			case CAN_DESTINATION_TH:	/*	仕向け:タイ・シンガポール	*/
			case CAN_DESTINATION_AU:	/*	仕向け:オーストラリア		*/
			case CAN_DESTINATION_KC:	/*	仕向け:南アフリカ他			*/
				*out = (SI_32)SIGNAL_VALUE_DRIVERPOSITION_RIGHT;
				break;
			case CAN_DESTINATION_US:	/*	仕向け:米国					*/
			case CAN_DESTINATION_CA:	/*	仕向け:カナダ				*/
			case CAN_DESTINATION_EU:	/*	仕向け:欧州					*/
			case CAN_DESTINATION_K4:	/*	仕向け:一般					*/
			case CAN_DESTINATION_KS:	/*	仕向け:中近東				*/
			case CAN_DESTINATION_CN:	/*	仕向け:中国					*/
			case CAN_DESTINATION_KR:	/*	仕向け:韓国					*/
				*out = (SI_32)SIGNAL_VALUE_DRIVERPOSITION_LEFT;
				break;
			default:
				*out = (SI_32)SIGNAL_VALUE_DRIVERPOSITION_IGNORE;
				break;
		}
	}
	else
	{
		*out = (SI_32)SIGNAL_VALUE_DRIVERPOSITION_INIT;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_EngineModelHighOrder		*/
/* 機能の説明	: エンジン型式(先頭4文字)取得			*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineModelHighOrder(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_vehicle_info == 1)
	{
		/*車種情報：有効*/
		if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086A)
		{
			/*車種情報：86A*/
			(void)memcpy(out, SIGNAL_VALUE_ENGINEMODELHIGHORDE_86A, 4);
		}
		else if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086B)
		{
			/*車種情報：86B*/
			(void)memcpy(out, SIGNAL_VALUE_ENGINEMODELHIGHORDE_86B, 4);
		}
		else
		{
			/*車種情報：不明*/
			(void)memcpy(out, SIGNAL_VALUE_ENGINEMODELHIGHORDE_UNKNOWN, 4);
		}
	}
	else
	{
		/*車種情報：無効*/
		(void)memcpy(out, SIGNAL_VALUE_ENGINEMODELHIGHORDE_UNKNOWN, 4);
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_EngineModelHighOrder		*/
/* 機能の説明	: エンジン型式(後方4文字)取得			*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineModelLowOrder(SI_32 *out)
{
	(void)memcpy(out, SIGNAL_VALUE_ENGINEMODELLOWORDER, 4);
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_EngineCharacter			*/
/* 機能の説明	: エンジン特性取得						*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineCharacter(SI_32 *out)
{
	*out = (SI_32)SIGNAL_VALUE_ENGINECHARACTER;
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_DriveTrainCharacter		*/
/* 機能の説明	: ドライブトレーン取得					*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_DriveTrainCharacter(SI_32 *out)
{
	UI_8 tm;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();

	if(pCanInfo->en_transmission_type == 1)
	{
		/* トランスミッション種別、AT:有効 */
		switch(pCanInfo->transmission_type)
		{
			case CAN_TRANSMISSION_ATORCVT:			/*	トランスミッション種別 00: AT or CVT						*/
				
				tm = SIGNAL_VALUE_TRANSMISSION_STEPAUTOMATIC;
				break;
			case CAN_TRANSMISSION_MT:				/*	トランスミッション種別 01: MT								*/
				tm = SIGNAL_VALUE_TRANSMISSION_MANUAL;
				break;
			default:
				tm = SIGNAL_VALUE_TRANSMISSION_IGNORE;
				break;
		}
	}
	else
	{
		/* トランスミッション種別:無効 */
		tm = SIGNAL_VALUE_TRANSMISSION_INITIAL;
	}
	
	*out = (SI_32)(SIGNAL_VALUE_DRIVETRAINCHARACTER | (UI_32)tm);
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_UTCTime					*/
/* 機能の説明	: UTC時間取得							*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_UTCTime(SI_32 *out)
{
	UI_32 utctime;
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:有効 */
		/* 秒単位を先に計算 */
		utctime = ((UI_32)pGnssHnrInfo->hour * 3600);
		utctime += ((UI_32)pGnssHnrInfo->min * 60);
		utctime += (UI_32)pGnssHnrInfo->sec;
		/* 分解能演算 */
		utctime = (UI_32)((F_64)utctime / SIGNAL_VALUE_UTC_RESOLUTION);
		/* ナノ秒を出力分解能（マイクロ秒）に変換して加算 */
		if(pGnssHnrInfo->nano > 0)
		{
			/* ナノ秒が0より大きいなら計算 */
			utctime += (UI_32)((F_64)pGnssHnrInfo->nano * (GNSS_NANOSEC_RESOLUTION / SIGNAL_VALUE_UTC_RESOLUTION));
		}
	}
	else
	{
		/* NAV-HNR:無効 */
		utctime = SIGNAL_VALUE_IGNORE_32;
	}
	*out = (SI_32)utctime;

	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_Date						*/
/* 機能の説明	: 日付取得								*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Date(SI_32 *out)
{
	UI_8 day;
	UI_8 month;
	UI_16 year;
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:有効 */
		day = pGnssHnrInfo->day;
		month = pGnssHnrInfo->month;
		year = 	pGnssHnrInfo->year;
	}
	else
	{
		/* NAV-HNR:無効 */
		day = SIGNAL_VALUE_IGNORE_8;
		month = SIGNAL_VALUE_IGNORE_8;
		year = SIGNAL_VALUE_IGNORE_16;
	}
	
	*out = (SI_32)((UI_32)day | ((UI_32)month << 8) | ((UI_32)year << 16));
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_Latitude					*/
/* 機能の説明	: 緯度取得								*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Latitude(SI_32 *out)
{
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:有効 */
		*out = pGnssHnrInfo->lat;
	}
	else
	{
		/* NAV-HNR:無効 */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_32_SIGNED;
	}
	return 0;
}

/********************************************************/
/* 関数名称		: get_signal_Longitude					*/
/* 機能の説明	: 経度取得								*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Longitude(SI_32 *out)
{
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:有効 */
		*out = pGnssHnrInfo->lon;
	}
	else
	{
		/* NAV-HNR:無効 */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_32_SIGNED;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_Altitude					*/
/* 機能の説明	: 高度取得								*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Altitude(SI_32 *out)
{
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:有効 */
		*out = pGnssHnrInfo->h_msl;
	}
	else
	{
		/* NAV-HNR:無効 */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_32_SIGNED;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_GeoidalSeparation			*/
/* 機能の説明	: ジオイド高取得						*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_GeoidalSeparation(SI_32 *out)
{
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:有効 */
		*out = pGnssHnrInfo->height;
	}
	else
	{
		/* NAV-HNR:無効 */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_32_SIGNED;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_HorizontallySpeed			*/
/* 機能の説明	: 水平方向速度取得						*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_HorizontallySpeed(SI_32 *out)
{
	UI_16 speed;
	UI_16 cource;
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:有効 */
		/* mm/s -> km/h :(3600 / 1000 x 1000) = 0.0036 */
		speed = (UI_16)((F_64)pGnssHnrInfo->g_speed * (0.0036 / SIGNAL_VALUE_VEHICLESPEED_RESOLUTION));
		cource = (UI_16)((F_64)pGnssHnrInfo->head_veh * (GNSS_HEADING_VEHICLE_RESOLUTION / SIGNAL_VALUE_HEADING_VEHICLE_RESOLUTION));
	}
	else
	{
		/* NAV-HNR:無効 */
		speed = SIGNAL_VALUE_IGNORE_16;
		cource = SIGNAL_VALUE_IGNORE_16;
	}
	*out = (SI_32)((UI_32)speed | ((UI_32)cource << 16));
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_Accuracy					*/
/* 機能の説明	: 位置精度取得							*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Accuracy(SI_32 *out)
{
	UI_16	accuracy;
	UI_8	quality;
	T_Gnss_ctrl_nav_posecef_info* pGnssNavPosecefInfo;
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:有効 */
		if( (pGnssHnrInfo->gpsFix == GNSS_GSPFIX_2DFIX) || 
			(pGnssHnrInfo->gpsFix == GNSS_GSPFIX_3DFIX) || 
			(pGnssHnrInfo->gpsFix == GNSS_GSPFIX_GPS_AND_DEADRECKONING))
		{
			/* 測位 */
			quality = SIGNAL_VALUE_GSPFIX_GPSFIX;
		}
		else if(pGnssHnrInfo->gpsFix == GNSS_GSPFIX_DEADRECKONIN_ONLY)
		{
			/* デッドレコニング:自律航法 */
			quality = SIGNAL_VALUE_GSPFIX_DEADRECKONIG_MODE;
		}
		else if(pGnssHnrInfo->gpsFix == GNSS_GSPFIX_NOFIX)
		{
			/* 未測位 */
			quality = SIGNAL_VALUE_GSPFIX_NOFIX;
		}
		else
		{
			/* その他 */
			quality = SIGNAL_VALUE_VALIDFLAG_IGNORE;
		}
	}
	else
	{
		/* NAV-HNR:無効 */
		quality = SIGNAL_VALUE_IGNORE_8;
	}
	
	pGnssNavPosecefInfo = Gnss_cntrol_GetNavPosecefInfo();
	if(pGnssNavPosecefInfo != NULL)
	{
		/* NAV-POSECEF:有効 */
		accuracy = (UI_16)pGnssNavPosecefInfo->p_acc;
	}
	else
	{
		/* NAV-POSECEF:無効 */
		accuracy = SIGNAL_VALUE_IGNORE_16;
	}
	
	*out = (SI_32)((UI_32)accuracy | ((UI_32)quality << 16));

	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_DOP						*/
/* 機能の説明	: DOP取得								*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_DOP(SI_32 *out)
{
	UI_8 SatellitesNum;
	UI_8 pdop;
	UI_8 hdop;
	UI_8 vdop;
	T_Gnss_ctrl_nav_dop_info* pGnssNavDopInfo = Gnss_control_GetNavDopInfo();
	T_Gnss_ctrl_nav_pvt_info* pGnssNavPvtInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssNavDopInfo != NULL)
	{
		/* NAV-DOP:有効 */
		pdop = (UI_8)((F_64)pGnssNavDopInfo->p_dop * (GNSS_PDOP_RESOLUTION / SIGNAL_VALUE_PDOP_RESOLUTION));
		hdop = (UI_8)((F_64)pGnssNavDopInfo->h_dop * (GNSS_HDOP_RESOLUTION / SIGNAL_VALUE_HDOP_RESOLUTION));
		vdop = (UI_8)((F_64)pGnssNavDopInfo->v_dop * (GNSS_VDOP_RESOLUTION / SIGNAL_VALUE_VDOP_RESOLUTION));
	}
	else
	{
		/* NAV-DOP:無効 */
		pdop = SIGNAL_VALUE_IGNORE_8;
		hdop = SIGNAL_VALUE_IGNORE_8;
		vdop = SIGNAL_VALUE_IGNORE_8;
	}

	if(pGnssNavPvtInfo != NULL)
	{
		/* NAV-PVT:有効 */
		SatellitesNum = pGnssNavPvtInfo->num_sv;
	}
	else
	{
		/* NAV-PVT:無効 */
		SatellitesNum = SIGNAL_VALUE_IGNORE_8;
	}
	
	*out = (SI_32)((UI_32)SatellitesNum | ((UI_32)pdop << 8) | ((UI_32)hdop << 16) | ((UI_32)vdop << 24));
	return 0;
}


/********************************************************/
/* 関数名称		: get_signal_AcceleratorOperation		*/
/* 機能の説明	: アクセル操作取得						*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_AcceleratorOperation(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	UI_8	sports_lamp;
	UI_8	snow_hold_lamp;	
	UI_8	accelRatio;
	UI_8	accelChar = SIGNAL_VALUE_ACCEL_CHARACTER_INITIAL;
	if(pCanInfo->en_accel_ratio == 1)
	{
		/* アクセル開度:有効 */
		accelRatio = (UI_8)(((UI_16)pCanInfo->accel_ratio * 100) / 255);		/*  単位:[100/255% = 0.392156862745098] */
	}
	else
	{
		/* アクセル開度:無効 */
		accelRatio = SIGNAL_VALUE_IGNORE_8;
	}
	
	if(pCanInfo->en_transmission_type == 1)
	{
		/* トランスミッション種別:有効 */
		if(pCanInfo->transmission_type == CAN_TRANSMISSION_ATORCVT)
		{
			/* AT or CVT */
			/* スポーツランプ、スノーホールドランプ */
			if( (pCanInfo->en_sports_lamp == 1) && (pCanInfo->en_snow_hold_lamp == 1) )
			{
				/* スポーツランプ、スノーホールドランプ:有効 */
				sports_lamp		= pCanInfo->sports_lamp;
				snow_hold_lamp	= pCanInfo->snow_hold_lamp;
				if( (sports_lamp == CAN_SPORTLAMP_TURN_OFF) && (snow_hold_lamp == CAN_SNOWLAMP_OFF) )
				{
					/* スポーツランプ:OFF、スノーホールドランプ:OFF */
					accelChar = SIGNAL_VALUE_ACCEL_CHARACTER_NORMAL;
				}
				else if((sports_lamp == CAN_SPORTLAMP_TURN_OFF) && (snow_hold_lamp == CAN_SNOWLAMP_ON))
				{
					/* スポーツランプ:OFF、スノーホールドランプ:ON */
					accelChar = SIGNAL_VALUE_ACCEL_CHARACTER_SNOW;
				}
				else
				{
					/* その他（スポーツランプ:OFF以外） */
					accelChar = SIGNAL_VALUE_ACCEL_CHARACTER_SPORTS;
				}
			}
			else
			{
				/* スポーツランプ、スノーホールドランプ:無効 */
				/* 初期値 */
			}
			
		}
		else
		{
			/* その他（AT or CVT以外） */
			accelChar = SIGNAL_VALUE_ACCEL_CHARACTER_IGNORE;
		}
	}
	else
	{
		/* トランスミッション種別:無効 */
		/* 初期値 */
	}
	*out = (SI_32)((UI_32)accelRatio | ((UI_32)accelChar << 8));
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_BrakeFlag					*/
/* 機能の説明	: ブレーキ操作フラグ取得				*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_BrakeFlag(SI_32 *out)
{
	UI_16 footbrakeLamp;
	UI_16 parkingBrakeLamp;
	
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	/* ブレーキランプ */
	if(pCanInfo->en_brake_lamp_sw == 1)
	{
		/* ブレーキランプ:有効 */
		if(pCanInfo->brake_lamp_sw == CAN_BRAKE_LAMP_ON)
		{
			/* ON */
			footbrakeLamp = (UI_16)SIGNAL_VALUE_BRAKELAMP_ON;
		}
		else
		{
			/* OFF */
			footbrakeLamp = (UI_16)SIGNAL_VALUE_BRAKELAMP_OFF;
		}
	}
	else
	{
		/* ブレーキランプ:無効 */
		/* 初期値 */
		footbrakeLamp = (UI_16)SIGNAL_VALUE_BRAKELAMP_INIT;
	}
	/* パーキングブレーキ */
	if(pCanInfo->en_hand_brake_sw == 1)
	{
		/* パーキングブレーキ:有効 */
		if(pCanInfo->hand_brake_sw == CAN_BRAKE_LAMP_ON)
		{
			/* ON */
			parkingBrakeLamp = (UI_16)SIGNAL_VALUE_PARKINGBRAKE_ON;
		}
		else
		{
			/* OFF */
			parkingBrakeLamp = (UI_16)SIGNAL_VALUE_PARKINGBRAKE_OFF;
		}
	}
	else
	{
		/* パーキングブレーキ:無効 */
		parkingBrakeLamp = (UI_16)SIGNAL_VALUE_PARKINGBRAKE_INIT;
	}
	*out = (SI_32)(((UI_32)footbrakeLamp) | ((UI_32)parkingBrakeLamp << 4));
	return 0;
}

/********************************************************/
/* 関数名称		: get_signal_BrakeOperation				*/
/* 機能の説明	: ブレーキペダル操作量取得				*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_BrakeOperation(SI_32 *out)
{
	F_64 brakeMax = 0.0;
	UI_16 brake;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if ((pCanInfo->en_smoothing_brake_pressure == 1) && (pCanInfo->en_vehicle_info == 1))
	{
		if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086A)
		{
			/*086A用ブレーキ最大値*/
			brakeMax = CAN_BRAKEPRESSURE_MAX_86A;
		}
		else if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086B)
		{
			/*086B用ブレーキ最大値*/
			brakeMax = CAN_BRAKEPRESSURE_MAX_86B;
		}
		else
		{
			/*その他はブレーキ最大値0*/
			brakeMax = 0;
		}
		if (brakeMax > 0)
		{
			/*ブレーキ最大値あり*/
			brake = pCanInfo->smoothing_brake_pressure;
			/* 先に分解能を計算すると％の変化が大きくなるので後で計算する */
			/* 小数点を考慮して100倍する */
			if ((brake * 100) >= (UI_16)((brakeMax * 100) / CAN_BRAKEPRESSURE_RESOLUTION))
			{
				/* 最大値以上の場合は100％ */
				*out = 100;
			}
			else
			{
				/* 最大値未満の場合は比率を計算 */
				*out = (SI_32)((((F_64)brake * CAN_BRAKEPRESSURE_RESOLUTION) * 100) / brakeMax);
			}
		}
		else
		{
			/*ブレーキ最大値なしのときは無効値*/
			*out = SIGNAL_VALUE_IGNORE_8;
		}
	}
	else
	{
		*out = SIGNAL_VALUE_IGNORE_8;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_ShiftOperation				*/
/* 機能の説明	: シフト操作取得						*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_ShiftOperation(SI_32 *out)
{
	UI_8 atShift 	= SIGNAL_VALUE_ATSHIFT_INIT;
	UI_8 gearPos 	= SIGNAL_VALUE_GEARPOS_INIT;
	UI_16 gearRatio = SIGNAL_VALUE_IGNORE_16;	/* 設定なし */
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if((pCanInfo->en_transmission_type == 1) && (pCanInfo->en_vehicle_info == 1))
	{
		/* トランスミッション種別:有効 */
		if(pCanInfo->transmission_type == CAN_TRANSMISSION_ATORCVT)
		{
			/* AT or CVT */
			if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086A){
				/* 86A車両 */
				/* ATシフト位置情報 */
				if((pCanInfo->en_shift_pos == 1) && (pCanInfo->en_mode_shift_control == 1) && (pCanInfo->en_sports_gear_pos == 1))
				{
					/* TCM変速制御モード */
					if (pCanInfo->mode_shift_control == CAN_MODESHIFT_GEARFIXED_AUTORETURN)
					{
						/*TCM変速制御モード:ギア固定自動復帰モード（マニュアル）	*/
						atShift = SIGNAL_VALUE_ATSHIFT_DM;
					}
					else if (pCanInfo->mode_shift_control == CAN_MODESHIFT_GEARFIXED_MODE)
					{
						/*TCM変速制御モード:ギア固定モード（マニュアル）			*/
						atShift = SIGNAL_VALUE_ATSHIFT_M;
					}
					else
					{
						/*TCM変速制御モード:その他				*/
						switch (pCanInfo->shift_pos)
						{
							case CAN_ATSHIFT_D	:	/* D */
								atShift = SIGNAL_VALUE_ATSHIFT_D;
								break;
							case CAN_ATSHIFT_N	:	/* N */
								atShift = SIGNAL_VALUE_ATSHIFT_N;
								break;
							case CAN_ATSHIFT_R	:	/* R */
								atShift = SIGNAL_VALUE_ATSHIFT_R;
								break;
							case CAN_ATSHIFT_P	:	/* P */
								atShift = SIGNAL_VALUE_ATSHIFT_P;
								break;
							default:
								atShift = SIGNAL_VALUE_ATSHIFT_IGNORE;
								break;
						}
					}
					
					if ((pCanInfo->mode_shift_control == CAN_MODESHIFT_GEARFIXED_AUTORETURN) || 
						(pCanInfo->mode_shift_control == CAN_MODESHIFT_GEARFIXED_MODE)){
						/*TCM変速制御モード:ギア固定自動復帰モード（マニュアル）	*/
						/*TCM変速制御モード:ギア固定モード（マニュアル）			*/
						switch (pCanInfo->sports_gear_pos)
						{
							case CAN_GEARPOS_1ST:	/*	ギア段数:1ST													*/
								gearPos = SIGNAL_VALUE_GEARPOS_1ST;
								break;
							case CAN_GEARPOS_2ND:	/*	ギア段数:2ND													*/
								gearPos = SIGNAL_VALUE_GEARPOS_2ND;
								break;
							case CAN_GEARPOS_3RD:	/*	ギア段数:3RD													*/
								gearPos = SIGNAL_VALUE_GEARPOS_3RD;
								break;
							case CAN_GEARPOS_4TH:	/*	ギア段数:4TH													*/
								gearPos = SIGNAL_VALUE_GEARPOS_4TH;
								break;
							case CAN_GEARPOS_5TH:	/*	ギア段数:5TH													*/
								gearPos = SIGNAL_VALUE_GEARPOS_5TH;
								break;
							case CAN_GEARPOS_6TH:	/*	ギア段数:6TH													*/
								gearPos = SIGNAL_VALUE_GEARPOS_6TH;
								break;
							default:
								/* その他 */
								gearPos = SIGNAL_VALUE_GEARPOS_IGNORE;
								break;
						}
					}
					else if(atShift == SIGNAL_VALUE_ATSHIFT_R )
					{
						/* リバース */
						gearPos = SIGNAL_VALUE_GEARPOS_REVERSE;
					}
					else{
						/* その他 */
						gearPos = SIGNAL_VALUE_GEARPOS_IGNORE;
					}
				}
				else
				{
					/* 初期値 */
				}
			}else if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086B){
				/* 86B車両 */
				/* ATシフト位置情報 */
				if((pCanInfo->en_shift_pos == 1) && (pCanInfo->en_sports_gear_pos == 1) && (pCanInfo->en_manual_mode_sw == 1))
				{
					gearPos = SIGNAL_VALUE_GEARPOS_IGNORE;
					switch (pCanInfo->shift_pos)
					{
						case CAN_ATSHIFT_86B_D	:	/* D */
							
							/*シフトD*/
							switch (pCanInfo->sports_gear_pos)
							{
								case CAN_ATGEAR_86B_MANUAL_1ST:	/*	ギア段数:1ST（Mレンジ＋DレンジテンポラリM）				*/
								case CAN_ATGEAR_86B_MANUAL_2ND:	/*	ギア段数:2ND（Mレンジ＋DレンジテンポラリM）				*/
								case CAN_ATGEAR_86B_MANUAL_3RD:	/*	ギア段数:3RD（Mレンジ＋DレンジテンポラリM）				*/
								case CAN_ATGEAR_86B_MANUAL_4TH:	/*	ギア段数:4TH（Mレンジ＋DレンジテンポラリM）				*/
								case CAN_ATGEAR_86B_MANUAL_5TH:	/*	ギア段数:5TH（Mレンジ＋DレンジテンポラリM）				*/
								case CAN_ATGEAR_86B_MANUAL_6TH:	/*	ギア段数:6TH（Mレンジ＋DレンジテンポラリM）				*/
									if (pCanInfo->manual_mode_sw == CAN_MANUALMODE_SW_ON)
									{
										/*マニュアルモードSW:ON*/
										/*Mレンジ*/
										atShift = SIGNAL_VALUE_ATSHIFT_M;
									}
									else
									{
										/*マニュアルモードSW:OFF*/
										/*DMレンジ*/
										atShift = SIGNAL_VALUE_ATSHIFT_DM;
									}
									break;
								case CAN_ATGEAR_86B_DSPORT_1ST:	/*	ギア段数:1ST（Dレンジスポーツモード）					*/
								case CAN_ATGEAR_86B_DSPORT_2ND:	/*	ギア段数:2ND（Dレンジスポーツモード）					*/
								case CAN_ATGEAR_86B_DSPORT_3RD:	/*	ギア段数:3RD（Dレンジスポーツモード）					*/
								case CAN_ATGEAR_86B_DSPORT_4TH:	/*	ギア段数:4TH（Dレンジスポーツモード）					*/
								case CAN_ATGEAR_86B_DSPORT_5TH:	/*	ギア段数:5TH（Dレンジスポーツモード）					*/
								case CAN_ATGEAR_86B_DSPORT_6TH:	/*	ギア段数:6TH（Dレンジスポーツモード）					*/
									/*DMレンジ*/
									atShift = SIGNAL_VALUE_ATSHIFT_DM;
									break;
								default:
									/* その他*/
									/*Dレンジ*/
									atShift = SIGNAL_VALUE_ATSHIFT_D;
									break;
							}

							switch (pCanInfo->sports_gear_pos)
							{
								case CAN_ATGEAR_86B_MANUAL_1ST:	/*	ギア段数:1ST（Mレンジ＋DレンジテンポラリM）				*/
								case CAN_ATGEAR_86B_DSPORT_1ST:	/*	ギア段数:1ST（Dレンジスポーツモード）					*/
									gearPos = SIGNAL_VALUE_GEARPOS_1ST;
									break;
								case CAN_ATGEAR_86B_MANUAL_2ND:	/*	ギア段数:2ND（Mレンジ＋DレンジテンポラリM）				*/
								case CAN_ATGEAR_86B_DSPORT_2ND:	/*	ギア段数:2ND（Dレンジスポーツモード）					*/
									gearPos = SIGNAL_VALUE_GEARPOS_2ND;
									break;
								case CAN_ATGEAR_86B_MANUAL_3RD:	/*	ギア段数:3RD（Mレンジ＋DレンジテンポラリM）				*/
								case CAN_ATGEAR_86B_DSPORT_3RD:	/*	ギア段数:3RD（Dレンジスポーツモード）					*/
									gearPos = SIGNAL_VALUE_GEARPOS_3RD;
									break;
								case CAN_ATGEAR_86B_MANUAL_4TH:	/*	ギア段数:4TH（Mレンジ＋DレンジテンポラリM）				*/
								case CAN_ATGEAR_86B_DSPORT_4TH:	/*	ギア段数:4TH（Dレンジスポーツモード）					*/
									gearPos = SIGNAL_VALUE_GEARPOS_4TH;
									break;
								case CAN_ATGEAR_86B_MANUAL_5TH:	/*	ギア段数:5TH（Mレンジ＋DレンジテンポラリM）				*/
								case CAN_ATGEAR_86B_DSPORT_5TH:	/*	ギア段数:5TH（Dレンジスポーツモード）					*/
									gearPos = SIGNAL_VALUE_GEARPOS_5TH;
									break;
								case CAN_ATGEAR_86B_MANUAL_6TH:	/*	ギア段数:6TH（Mレンジ＋DレンジテンポラリM）				*/
								case CAN_ATGEAR_86B_DSPORT_6TH:	/*	ギア段数:6TH（Dレンジスポーツモード）					*/
									gearPos = SIGNAL_VALUE_GEARPOS_6TH;
									break;
								default:
									/* その他*/
									/* 処理なし*/
									break;
							}
							break;
						case CAN_ATSHIFT_86B_N	:	/* N */
							atShift = SIGNAL_VALUE_ATSHIFT_N;
							break;
						case CAN_ATSHIFT_86B_R	:	/* R */
							atShift = SIGNAL_VALUE_ATSHIFT_R;
							gearPos = SIGNAL_VALUE_GEARPOS_REVERSE;
							break;
						case CAN_ATSHIFT_86B_P	:	/* P */
							atShift = SIGNAL_VALUE_ATSHIFT_P;
							break;
						default:
							atShift = SIGNAL_VALUE_ATSHIFT_IGNORE;
							break;
					}
				}
				else
				{
					/* 初期値 */
				}
			}else{
				/* 車種：不明 */
				/* 初期値 */
			}
		}
		else if(pCanInfo->transmission_type == CAN_TRANSMISSION_MT)
		{
			/* MT */
			atShift = SIGNAL_VALUE_ATSHIFT_IGNORE;
			if((pCanInfo->en_mt_rev_sw == 1) && (pCanInfo->en_mt_gear_pos == 1))
			{
				/* MTリバース、MTギア:有効 */
				if(pCanInfo->mt_rev_sw == CAN_MT_REVERSE_ON)
				{
					/* MTリバースON */
					gearPos = SIGNAL_VALUE_GEARPOS_REVERSE;
				}
				else
				{
					/* MTリバースOFF */
					switch (pCanInfo->mt_gear_pos)
					{
						case CAN_GEARPOS_1ST:	/*	ギア段数:1ST													*/
							gearPos = SIGNAL_VALUE_GEARPOS_1ST;
							break;
						case CAN_GEARPOS_2ND:	/*	ギア段数:2ND													*/
							gearPos = SIGNAL_VALUE_GEARPOS_2ND;
							break;
						case CAN_GEARPOS_3RD:	/*	ギア段数:3RD													*/
							gearPos = SIGNAL_VALUE_GEARPOS_3RD;
							break;
						case CAN_GEARPOS_4TH:	/*	ギア段数:4TH													*/
							gearPos = SIGNAL_VALUE_GEARPOS_4TH;
							break;
						case CAN_GEARPOS_5TH:	/*	ギア段数:5TH													*/
							gearPos = SIGNAL_VALUE_GEARPOS_5TH;
							break;
						case CAN_GEARPOS_6TH:	/*	ギア段数:6TH													*/
							gearPos = SIGNAL_VALUE_GEARPOS_6TH;
							break;
						case CAN_GEARPOS_NOT_DECISION:	/*	ギア段数:不定											*/
							gearPos = SIGNAL_VALUE_GEARPOS_INIT;
							break;
						default:
							/* その他 */
							gearPos = SIGNAL_VALUE_GEARPOS_IGNORE;
							break;
					}
				}
			}
			else
			{
				/* MTリバース、MTギア:無効 */
				/* 初期値 */
			}
		}
		else
		{
			/* その他 */
			atShift = SIGNAL_VALUE_ATSHIFT_IGNORE;
			gearPos = SIGNAL_VALUE_GEARPOS_IGNORE;
		}
	}
	else
	{
		/* トランスミッション種別:無効 */
		/* 初期値 */
	}
	*out = (SI_32)(((UI_32)atShift) | ((UI_32)gearRatio << 8) | ((UI_32)gearPos << 24));
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_FuelIndicator				*/
/* 機能の説明	: フューエル量,VSC状態取得				*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)S.Sakata							*/
/* 作成日		: 21/02/-								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_FuelIndicator(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	UI_8 absAct = SIGNAL_VALUE_NONACTIVE;
	UI_8 vscAct = SIGNAL_VALUE_NONACTIVE;
	UI_8 trcAct = SIGNAL_VALUE_NONACTIVE;
	UI_8 trqUpReq = SIGNAL_VALUE_ENGTRQ_NO_REQUEST;
	UI_8 trqDwnReq = SIGNAL_VALUE_ENGTRQ_NO_REQUEST;
	UI_8 vsc = SIGNAL_VALUE_IGNORE_4;										/* VSC状態*/
	UI_16 fuelIndicator = SIGNAL_VALUE_IGNORE_12;							/* フューエル量 */
	SI_32 randomValue;

	if (pCanInfo->en_vehicle_info == 1){
		/*車種有効*/
		if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086B){
			/*車種：086B*/
			/*ABS制御状態有効チェック*/
			if (pCanInfo->en_absAct == 1)
			{
				/*ABS制御状態判定*/
				if (pCanInfo->absAct == CAN_ABSON_ACTIVE)
				{
					/*ABS制御中*/
					absAct = SIGNAL_VALUE_ACTIVE;
				}
				else
				{
					/*ABS制御外*/
					/*処理なし*/
				}
			}

			/*VSC制御状態有効チェック*/
			if (pCanInfo->en_vscAct == 1)
			{
				/*VSC制御状態判定*/
				if (pCanInfo->vscAct == CAN_VSCON_ACTIVE)
				{
					/*VSC制御中*/
					vscAct = SIGNAL_VALUE_ACTIVE;
				}
				else
				{
					/*VSC制御外*/
					/*処理なし*/
				}
			}

			/*TRC制御状態有効チェック*/
			if (pCanInfo->en_trcAct == 1)
			{
				/*TRC制御状態判定*/
				if (pCanInfo->trcAct == CAN_TRCON_ACTIVE)
				{
					/*TRC制御中*/
					trcAct = SIGNAL_VALUE_ACTIVE;
				}
				else
				{
					/*TRC制御外*/
					/*処理なし*/
				}
			}
			else
			{
				/*VSCOFF状態、VSC制御状態：無効*/
				/*処理なし*/
			}

			/*エンジントルクアップ要求有効チェック*/
			if (pCanInfo->en_engTrqUpReqFlag == 1)
			{
				/*エンジントルクアップ要求判定*/
				if (pCanInfo->engTrqUpReqFlag == CAN_ENGTRQ_REQUEST)
				{
					/*要求あり*/
					trqUpReq = SIGNAL_VALUE_ENGTRQ_REQUEST;
				}
				else
				{
					/*要求なし*/
					/*処理なし*/
				}
			}
			else
			{
				/*エンジントルクアップ要求：無効*/
				/*処理なし*/
			}

			/*エンジントルクダウン要求有効チェック*/
			if (pCanInfo->en_engTrqDwnReqFlag == 1)
			{
				/*エンジントルクダウン要求判定*/
				if (pCanInfo->engTrqDwnReqFlag == CAN_ENGTRQ_REQUEST)
				{
					/*要求あり*/
					trqDwnReq = SIGNAL_VALUE_ENGTRQ_REQUEST;
				}
				else
				{
					/*要求なし*/
					/*処理なし*/
				}
			}
			else
			{
				/*エンジントルクダウン要求：無効*/
				/*処理なし*/
			}
		}
	}

	/* 乱数取得 */
	(void)get_signal_RandomValue(&randomValue);

	*out = (SI_32)( (UI_32)fuelIndicator | ((UI_32)vsc << 12) | ((UI_32)absAct << 16) | 
					((UI_32)vscAct << 17) | ((UI_32)trcAct << 18) |
					((UI_32)trqUpReq << 19) | ((UI_32)trqDwnReq << 20) | 
					((UI_32)randomValue & 0xFFE00000));
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_Steering					*/
/* 機能の説明	: ステアリング取得						*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Steering(SI_32 *out)
{
	/*
	ステアリング　　右旋回時：プラス／左旋回時：マイナス
	*/
	SI_16 steer;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_steer_angle == 1)
	{
		/* ステアリング:有効 */
		if(pCanInfo->steer_angle != CAN_STEERING_ERRORVALUE)
		{
			/* 正常値 */
			steer = (SI_16)((F_64)((SI_16)pCanInfo->steer_angle) * CAN_STEERING_RESOLUTION);
			*out = (SI_32)((UI_32)steer & 0x0000FFFF);
		}
		else
		{
			/* 異常値 */
			*out = SIGNAL_VALUE_IGNORE_16_SIGNED;
		}
	}
	else
	{
		/* ステアリング:無効 */
		*out = SIGNAL_VALUE_IGNORE_16_SIGNED;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_Blinker					*/
/* 機能の説明	: ウィンカー取得						*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Blinker(SI_32 *out)
{

	UI_8 blinker = SIGNAL_VALUE_BLINKER_INITIAL;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	
	/*車種状態チェック*/
	if (pCanInfo->en_vehicle_info == 1)
	{
		/*車種有効*/
		if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086B)
		{
			/*車種：086B*/
			/*ウィンカー有効チェック*/
			if((pCanInfo->en_blinker_left == 1) && (pCanInfo->en_blinker_right == 1))
			{
				/*ウィンカー：有効*/
				if( (pCanInfo->blinker_right == CAN_BLINKER_OFF) && (pCanInfo->blinker_left == CAN_BLINKER_OFF) )
				{
					/*ウィンカー：両方OFF*/
					blinker = SIGNAL_VALUE_BLINKER_OFF;
				}
				else 
				{
					/*ウィンカー：どちらか点灯*/
					if(pCanInfo->blinker_right == CAN_BLINKER_ON)
					{
						/*ウィンカー：右点灯*/
						blinker |= SIGNAL_VALUE_BLINKER_TURN_RIGHT;
					}
					if(pCanInfo->blinker_left == CAN_BLINKER_ON)
					{
						/*ウィンカー：左点灯*/
						blinker |= SIGNAL_VALUE_BLINKER_TURN_LEFT;
					}
				}
			}
			else
			{
				/* ウィンカー:無効 */
				/* 初期値*/
			}
		}
		else
		{
			/*車種：086B以外*/
			blinker = SIGNAL_VALUE_BLINKER_INVALID;
		}
	}
	else
	{
		/*車種無効*/
		/*処理無し*/
	}
	*out = (SI_32)blinker;
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_AirCondition				*/
/* 機能の説明	: エアーコンディション取得				*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_AirCondition(SI_32 *out)
{
	UI_8 airconStat = SIGNAL_VALUE_IGNORE_4;
	UI_8 airconTemp = SIGNAL_VALUE_IGNORE_8;
	UI_8 insideTemp = SIGNAL_VALUE_IGNORE_8;
	UI_8 outsideTemp = SIGNAL_VALUE_IGNORE_8;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_out_temp == 1)
	{
		/* 外気温:有効 */
		if(pCanInfo->out_temp <= CAN_OUOTSIDE_TEMP_MAX)
		{
			/* 正常値 */
			outsideTemp = (UI_8)((UI_8)((F_32)pCanInfo->out_temp * CAN_OUOTSIDE_TEMP_RESOLUTION) + CAN_OUTSIDE_TEMP_OFFSET - SIGNAL_VALUE_OUTSIDE_TEMP_OFFSET);
		}
		else
		{
			/* 異常値 */
			/* 初期値 */
		}
	}
	else
	{
		/* 外気温:無効 */
		/* 初期値 */
	}
	*out = (SI_32)((airconStat) | ((UI_32)airconTemp << 4) | ((UI_32)insideTemp << 12) | ((UI_32)outsideTemp << 20));
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_DrivingSupport				*/
/* 機能の説明	: ドライビングサポート取得				*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_DrivingSupport(SI_32 *out)
{
	UI_8 absSet = SIGNAL_VALUE_ABS_INIT;					/*ABSセット状態*/
	UI_8 vscSet = SIGNAL_VALUE_VSC_INIT_OR_NORMAL;			/*VSCセット状態*/
	UI_32 other = SIGNAL_VALUE_IGNORE_32;	/*その他信号：すべて無効値*/
	UI_8 vscMode;			/*VSC：モード*/
	UI_8 vscActive;			/*VSC：Active状態*/

	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	/*車種状態チェック*/
	if (pCanInfo->en_vehicle_info == 1){
		/*車種有効*/
		if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086B){
			/*車種：086B*/
			/*ABS情報チェック*/
			if ((pCanInfo->en_absAct == 1) || (pCanInfo->en_absFail == 1) || (pCanInfo->en_absWarning == 1))
			{
				/*ABS情報有効*/
				if ((pCanInfo->absFail == CAN_ABS_SYSFAIL_ON) || (pCanInfo->absWarning == CAN_ABS_WARNING_ON))
				{
					/*ABS異常またはウォーニング*/
					absSet = SIGNAL_VALUE_ABS_IGNORE;
				}
				/*VSC制御状態判定*/
				else if (pCanInfo->absAct == CAN_ABSON_ACTIVE)
				{
					/*ABS制御中*/
					absSet = SIGNAL_VALUE_ABS_SET_AND_ACTIVE;
				}
				else
				{
					/*処理なし*/
				}
			}
			else
			{
				/*ABS情報無効*/
				/*処理無し*/
			}

			/*VSC情報チェック*/
			if ((pCanInfo->en_trcMode == 1) || (pCanInfo->en_trcOnOff == 1) || (pCanInfo->en_trcAct == 1) ||
				(pCanInfo->en_vscUnderStr == 1) || (pCanInfo->en_vscOverStr == 1) || 
				(pCanInfo->en_vscOnOff == 1) || (pCanInfo->en_vscWarning == 1) )
			{
				/*VSC情報有効*/
				if (pCanInfo->vscWarning == CAN_VSC_WARNING_ON)
				{
					/*VSCウォーニング*/
					vscSet = SIGNAL_VALUE_VSC_IGNORE;
				}
				else
				{
					/*VSCウォーニング以外*/
					/*VSCモード判定*/
					if (pCanInfo->vscOnOff == CAN_VSC_ON)
					{
						/*VSC ON*/
						if (pCanInfo->trcOnOff == CAN_TRC_OFF)
						{
							/*VSC ON /TRC OFF*/
							vscMode = SIGNAL_VALUE_VSC_SET_TRC_OFF;
						}
						else
						{
							/*VSC ON /TRC ON*/
							/*ノーマル*/
							vscMode = SIGNAL_VALUE_VSC_INIT_OR_NORMAL;
						}
					}
					else
					{
						/*VSC OFF*/
						if (pCanInfo->trcOnOff == CAN_TRC_OFF)
						{
							/*VSC OFF /TRC OFF*/
							vscMode = SIGNAL_VALUE_VSC_SET_TRC_VSC_OFF;
						}
						else if (pCanInfo->trcMode == CAN_TRC_MODE_ON)
						{
							/*VSC OFF /TRC ON / TRC重視モードON*/
							vscMode = SIGNAL_VALUE_VSC_SET_SPECIAL;
						}
						else
						{
							/*VSC OFF /TRC ON / TRC重視モードOFF*/
							vscMode = SIGNAL_VALUE_VSC_SET_VSC_OFF;
						}
					}

					/*VSC 制御/非制御判定*/
					if ( (pCanInfo->vscUnderStr == CAN_VSC_CONTROL_STR_ON) || (pCanInfo->vscOverStr == CAN_VSC_CONTROL_STR_ON) )
					{
						/*VSC制御中(アンダーステア、オーバーステア制御中)*/
						if (pCanInfo->trcAct == CAN_TRCON_ACTIVE)
						{
							/*VSC & TRC 制御中*/
							vscActive = SIGNAL_VALUE_VSC_TRC_ACTIVE_BIT | SIGNAL_VALUE_VSC_VSC_ACTIVE_BIT;
						}
						else
						{
							/*VSC制御中*/
							vscActive = SIGNAL_VALUE_VSC_VSC_ACTIVE_BIT;
						}
					}
					else
					{
						/*VSC非制御中*/
						if (pCanInfo->trcAct == CAN_TRCON_ACTIVE)
						{
							/*TRC制御中*/
							vscActive = SIGNAL_VALUE_VSC_TRC_ACTIVE_BIT;
						}
						else
						{
							/*VSC & TRC 非制御中*/
							vscActive = SIGNAL_VALUE_VSC_NOTACTIVE;
						}
					}
					/*VSC状態に設定*/
					vscSet = vscActive + vscMode;
				}
			}
			else
			{
				/*ABS情報無効*/
				/*処理無し*/
			}
		}else{
			/*車種：086B以外*/
			absSet = SIGNAL_VALUE_ABS_IGNORE;
			vscSet = SIGNAL_VALUE_VSC_IGNORE;
		}
	}else{
		/*車種無効*/
		/*処理無し*/
	}


	*out = (SI_32)((absSet) | ((UI_32)vscSet << 4) | (other << 12));
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_Doors						*/
/* 機能の説明	: ドア状態取得							*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Doors(SI_32 *out)
{
	UI_8 firstDoor = SIGNAL_VALUE_DOOR_CLOSE;
	UI_8 secondDoor = SIGNAL_VALUE_DOOR_IGNORE;
	UI_8 hood = SIGNAL_VALUE_DOOR_IGNORE;
	UI_8 roof = SIGNAL_VALUE_DOOR_IGNORE;
	SI_32 driverPos;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if( (pCanInfo->en_door_sw_seat_p == 1) && (pCanInfo->en_door_sw_seat_d == 1) )
	{
		/* ドアSW P席、D席:有効 */
		/* ドライバー位置取得 */
		(void)get_signal_DriverPosition(&driverPos);
		if(driverPos == SIGNAL_VALUE_DRIVERPOSITION_RIGHT)
		{
			/* 右ハンドル */
			if(pCanInfo->door_sw_seat_d == CAN_DOOR_SW_OPEN)
			{
				/* D席オープン（右） */
				firstDoor |= SIGNAL_VALUE_DOOR_RIGHT_OPEN;
			}
			
			if(pCanInfo->door_sw_seat_p == CAN_DOOR_SW_OPEN)
			{
				/* P席オープン（左） */
				firstDoor |= SIGNAL_VALUE_DOOR_LEFT_OPEN;
			}
		}
		else if(driverPos == SIGNAL_VALUE_DRIVERPOSITION_LEFT)
			/* 左ハンドル */
		{
			if(pCanInfo->door_sw_seat_d == CAN_DOOR_SW_OPEN)
			{
				/* D席オープン（左） */
				firstDoor |= SIGNAL_VALUE_DOOR_LEFT_OPEN;
			}
			
			if(pCanInfo->door_sw_seat_p == CAN_DOOR_SW_OPEN)
			{
				/* P席オープン（右） */
				firstDoor |= SIGNAL_VALUE_DOOR_RIGHT_OPEN;
			}
		}
		else
		{
			/* その他 */
			/* 初期値 */
		}
	}
	else
	{
		/* ドアSW P席、D席:無効 */
		/* 初期値 */
	}
	*out = (SI_32)((firstDoor) | ((UI_32)secondDoor << 4) | ((UI_32)hood << 8) | ((UI_32)roof << 12));
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_SittingandSeatBeltStatus	*/
/* 機能の説明	: シート着席、シートベルト取得			*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 21/03/-								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_SittingandSeatBeltStatus(SI_32 *out)
{
	UI_8 firstSitting = SIGNAL_VALUE_IGNORE_4;
	UI_8 secondSitting = SIGNAL_VALUE_IGNORE_4;
	UI_8 thirdSitting = SIGNAL_VALUE_IGNORE_4;
	UI_8 firstSeatbelt = SIGNAL_VALUE_SEATBELT_INITIAL;
	UI_8 secondSeatbelt = SIGNAL_VALUE_SEATBELT_IGNORE;
	UI_8 thirdSeatbelt = SIGNAL_VALUE_SEATBELT_IGNORE;
	SI_32 driverPos;

	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if( (pCanInfo->en_seat_belt_sw_p == 1) && (pCanInfo->en_seat_belt_sw_d == 1) )
	{
		/* シートベルト P席、D席:有効 */
		if((pCanInfo->seat_belt_sw_d == CAN_SEATBELT_OFF) && (pCanInfo->seat_belt_sw_p == CAN_SEATBELT_OFF))
		{
			/* D席、P席ともにシートベルト非装着 */
			firstSeatbelt |= SIGNAL_VALUE_SEATBELT_NOONELOCKED;
		}
		else
		{
			/* D席、P席いずれかがシートベルト装着 */
			/* ドライバー位置取得 */
			(void)get_signal_DriverPosition(&driverPos);
			if(driverPos == SIGNAL_VALUE_DRIVERPOSITION_RIGHT)
			{
				/* 右ハンドル */
				if(pCanInfo->seat_belt_sw_d == CAN_SEATBELT_ON)
				{
					/* D席シートベルト装着(右） */
					firstSeatbelt |= SIGNAL_VALUE_SEATBELT_RIGHT_LOCKED;
				}
				
				if(pCanInfo->seat_belt_sw_p == CAN_SEATBELT_ON)
				{
					/* P席シートベルト装着(左） */
					firstSeatbelt |= SIGNAL_VALUE_SEATBELT_LEFT_LOCKED;
				}
			}
			else if(driverPos == SIGNAL_VALUE_DRIVERPOSITION_LEFT)
			{
				/* 左ハンドル */
				if(pCanInfo->seat_belt_sw_d == CAN_SEATBELT_ON)
				{
					/* D席シートベルト装着(左） */
					firstSeatbelt |= SIGNAL_VALUE_SEATBELT_LEFT_LOCKED;
				}
				
				if(pCanInfo->seat_belt_sw_p == CAN_SEATBELT_ON)
				{
					/* P席シートベルト装着(右） */
					firstSeatbelt |= SIGNAL_VALUE_SEATBELT_RIGHT_LOCKED;
				}
			}
			else
			{
				/*その他*/
				/*処理なし：初期値*/
			}
		}
	}
	else
	{
		/* シートベルト P席、D席:無効 */
		/* 初期値 */
	}
	*out =	(SI_32)((firstSitting) 				 | ((UI_32)secondSitting << 4) 	 | ((UI_32)thirdSitting << 8) | 
					((UI_32)firstSeatbelt << 12) | ((UI_32)secondSeatbelt << 16) | ((UI_32)thirdSeatbelt << 20));

	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_LampStatus				*/
/* 機能の説明	: ランプ状態取得						*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_LampStatus(SI_32 *out)
{
	UI_8 headLamp = SIGNAL_VALUE_HEADLIGHT_INITIAL;
	UI_8 fogLamp = SIGNAL_VALUE_IGNORE_4;
	UI_8 rooLamp = SIGNAL_VALUE_IGNORE_4;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	
	if((pCanInfo->en_head_lamp_hi == 1) && (pCanInfo->en_head_lamp_lo == 1) && (pCanInfo->en_head_lamp_small == 1))
	{
		/* ヘッドライト:有効 */
		if((pCanInfo->head_lamp_lo == CAN_LIGHT_TURN_OFF) && (pCanInfo->head_lamp_small == CAN_LIGHT_TURN_OFF) && (pCanInfo->head_lamp_hi == CAN_LIGHT_TURN_OFF))
		{
			/* すべてOFF */
			headLamp = SIGNAL_VALUE_HEADLIGHT_OFF;
		}
		else{
			/* いずれががON */
			if(pCanInfo->head_lamp_small == CAN_LIGHT_TURN_ON)
			{
				/* ヘッドライト:Small */
				headLamp |= SIGNAL_VALUE_HEADLIGHT_SMALL;
			}
			if(pCanInfo->head_lamp_lo == CAN_LIGHT_TURN_ON)
			{
				/* ヘッドライト:Lo */
				headLamp |= SIGNAL_VALUE_HEADLIGHT_LO;
			}
			if((pCanInfo->head_lamp_hi == CAN_LIGHT_TURN_ON) && (pCanInfo->head_lamp_lo == CAN_LIGHT_TURN_ON))
			{
				/* ヘッドライト:Hight and lo */
				headLamp |= SIGNAL_VALUE_HEADLIGHT_HIGHANDLO;
			}
		}
	}
	else
	{
		/* ヘッドライト:無効 */
		/* 初期値 */
	}
	*out = (SI_32)((UI_32)headLamp | ((UI_32)fogLamp << 4) | ((UI_32)rooLamp << 8));
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_Power						*/
/* 機能の説明	: 起動状態取得							*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Power(SI_32 *out)
{
	UI_8 power;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if((pCanInfo->en_ign == 1) && (pCanInfo->en_acc == 1))
	{
		/* ACC IGN:有効 */
		power = SIGNAL_VALUE_POWER_PLUS_B; /* +Bは固定でON */
		if(pCanInfo->acc == CAN_POWER_ON)
		{
			/* ACC */
			power |= SIGNAL_VALUE_POWER_ACC;
		}
		if(pCanInfo->ign == CAN_POWER_ON)
		{
			/* IG */
			power |= SIGNAL_VALUE_POWER_IG;
		}
	}
	else
	{
		/* ACC IGN:無効 */
		power = SIGNAL_VALUE_POWER_INITIAL;
	}
	*out = (SI_32)power;
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_EngineRev					*/
/* 機能の説明	: エンジン回転数取得					*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineRev(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_eng_rpm_taco == 1)
	{
		/* エンジン回転数:有効 */
		*out = pCanInfo->eng_rpm_taco;
	}
	else
	{
		/* エンジン回転数:無効 */
		*out = SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_VehicleSpeed				*/
/* 機能の説明	: 車速取得								*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_VehicleSpeed(SI_32 *out)
{
	UI_16 speed = 0;
	UI_8  direction = SIGNAL_VALUE_DIRECTION_IGNORE;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if( pCanInfo->en_vehicle_speed_for_disp == 1 )
	{
		/* 車速:有効 */
		speed = (UI_16)((F_64)pCanInfo->vehicle_speed_for_disp * (CAN_VEHICLESPEED_DISP_RESOLUTION / SIGNAL_VALUE_VEHICLESPEED_DISP_RESOLUTION));
	}
	else
	{
		/* 車速:無効 */
		speed = SIGNAL_VALUE_IGNORE_16;
	}
	
	*out = (SI_32)(((UI_32)direction << 16) | (UI_32)speed);
	
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_HighResolutionSpeedFront	*/
/* 機能の説明	: 高精度車速(フロント)取得				*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_HighResolutionSpeedFront(SI_32 *out)
{
	UI_16 fl,fr;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if( (pCanInfo->en_speed_wheel_ftlh == 1) && (pCanInfo->en_speed_wheel_ftrh == 1) )
	{
		/* 車輪速(FL輪),車輪速(FR輪):有効 */
		fl = (UI_16)(((F_64)pCanInfo->speed_wheel_ftlh * (CAN_WHEELSPEED_RESOLUTION / SIGNAL_VALUE_WHEELSPEED_RESOLUTION)) - ((F_64)SIGNAL_VALUE_WHEELSPEED_OFFSET / SIGNAL_VALUE_WHEELSPEED_RESOLUTION));
		fr = (UI_16)(((F_64)pCanInfo->speed_wheel_ftrh * (CAN_WHEELSPEED_RESOLUTION / SIGNAL_VALUE_WHEELSPEED_RESOLUTION)) - ((F_64)SIGNAL_VALUE_WHEELSPEED_OFFSET / SIGNAL_VALUE_WHEELSPEED_RESOLUTION));
	}
	else
	{
		/* 車輪速(FL輪),車輪速(FR輪):無効 */
		fl = SIGNAL_VALUE_IGNORE_16;
		fr = SIGNAL_VALUE_IGNORE_16;
	}
	*out = (SI_32)((UI_32)fl | ((UI_32)fr << 16));
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_HighResolutionSpeedRear	*/
/* 機能の説明	: 高精度車速(リア)取得					*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_HighResolutionSpeedRear(SI_32 *out)
{
	UI_16 rl,rr;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if( (pCanInfo->en_speed_wheel_rrlh == 1) && (pCanInfo->en_speed_wheel_rrrh == 1) )
	{
		/* 車輪速(RL輪),車輪速(RR輪):有効 */
		rl = (UI_16)(((F_64)pCanInfo->speed_wheel_rrlh * (CAN_WHEELSPEED_RESOLUTION / SIGNAL_VALUE_WHEELSPEED_RESOLUTION)) - ((F_64)SIGNAL_VALUE_WHEELSPEED_OFFSET / SIGNAL_VALUE_WHEELSPEED_RESOLUTION));
		rr = (UI_16)(((F_64)pCanInfo->speed_wheel_rrrh * (CAN_WHEELSPEED_RESOLUTION / SIGNAL_VALUE_WHEELSPEED_RESOLUTION)) - ((F_64)SIGNAL_VALUE_WHEELSPEED_OFFSET / SIGNAL_VALUE_WHEELSPEED_RESOLUTION));
	}
	else
	{
		/* 車輪速(RL輪),車輪速(RR輪):無効 */
		rl = SIGNAL_VALUE_IGNORE_16;
		rr = SIGNAL_VALUE_IGNORE_16;
	}
	*out = (SI_32)((UI_32)rl | ((UI_32)rr << 16));
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_AccelerationPlane			*/
/* 機能の説明	: 加速度（水平方向:前後、横）取得		*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_AccelerationPlane(SI_32 *out)
{
	SI_16 acc_lr,acc_fr;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if( pCanInfo->en_acc_fr == 1 )
	{
		/* 前後加速度:有効 */
		acc_fr = (SI_16)((F_64)((SI_8)pCanInfo->acc_fr) * (CAN_ACCERERATION_RESOLUTION_FR / SIGNAL_VALUE_ACCERERATION_RESOLUTION));
	}
	else
	{
		/* 前後加速度:無効 */
		acc_fr = SIGNAL_VALUE_IGNORE_16_SIGNED;
	}
	if( pCanInfo->en_acc_lr == 1 )
	{
		/* 横加速度:有効 */
		acc_lr = (SI_16)((F_64)((SI_8)pCanInfo->acc_lr) * (CAN_ACCERERATION_RESOLUTION_LR / SIGNAL_VALUE_ACCERERATION_RESOLUTION));
	}
	else
	{
		/* 横加速度:無効 */
		acc_lr = SIGNAL_VALUE_IGNORE_16_SIGNED;
	}
	*out = (SI_32)(((UI_32)acc_fr & 0x0000FFFF) | ((UI_32)acc_lr << 16));
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_YawandPitchRate			*/
/* 機能の説明	: ヨーレート、ピッチレート取得			*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_YawandPitchRate(SI_32 *out)
{
	SI_16 sYawrate;
	SI_16 yawrate = SIGNAL_VALUE_IGNORE_16_SIGNED;
	SI_16 pitchrate = SIGNAL_VALUE_IGNORE_16_SIGNED; /* 設定なし */
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_yaw_rate == 1)
	{
		/* ヨーレート:有効 */
		sYawrate = (SI_16)pCanInfo->yaw_rate;
		if( (sYawrate <= (SI_16)CAN_YAWRATE_MAX) && (sYawrate >= (SI_16)CAN_YAWRATE_MIN) )
		{
			/* 正常値 */
			yawrate = (SI_16)((F_64)sYawrate * (CAN_YAWRATE_RESOLUTION_FR / SIGNAL_VALUE_YAWRATE_RESOLUTION));
		}
		else
		{
			/* 異常値 */
			/* 初期値 */
		}
	}
	else
	{
		/* ヨーレート:無効 */
		/* 初期値 */
	}
	
	*out = (SI_32)(((UI_32)yawrate & 0x0000FFFF) | (((UI_32)pitchrate << 16) & 0xFFFF0000));
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_UnitTemperature			*/
/* 機能の説明	: ユニット温度取得						*/
/*				:（エンジン水温、エンジン油温、変速機油温）*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_UnitTemperature(SI_32 *out)
{
	UI_16 tmp_value;
	UI_8 eng_temp_coolant = SIGNAL_VALUE_IGNORE_8;
	UI_8 eng_temp_oil = SIGNAL_VALUE_IGNORE_8;
	UI_8 trans_temp_oil = SIGNAL_VALUE_IGNORE_8;	/* 設定なし */
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_temp_coolant == 1)
	{            
		/* エンジン水温:有効 */
		if(pCanInfo->temp_coolant <= CAN_TEMP_COOLANT_MAX)
		{
			/* 正常値 */
			tmp_value = pCanInfo->temp_coolant + CAN_ENG_TEMP_COOLANT_OFFSET - SIGNAL_VALUE_ENG_TEMP_COOLANT_OFFSET;
			if(tmp_value >= 0xFF)
			{
				/* 最大値を超える場合は丸める */
				tmp_value = 0xFE;
			}
			eng_temp_coolant = (UI_8)tmp_value;
		}
		else
		{
			/* 異常値 */
			/* 初期値 */
		}
	}
	else
	{
		/* エンジン水温:無効 */
		/* 初期値 */
	}
	
	if(pCanInfo->en_temp_oil == 1)
	{
		/* エンジン油温:有効 */
		tmp_value = pCanInfo->temp_oil + CAN_ENG_TEMP_OIL_OFFSET - SIGNAL_VALUE_ENG_TEMP_OIL_OFFSET;
		if(tmp_value >= 0xFF)
		{
			/* 最大値を超える場合は丸める */
			tmp_value = 0xFE;
		}
		eng_temp_oil = (UI_8)tmp_value;
	}
	else
	{
		/* エンジン油温:無効 */
		/* 初期値 */
	}
	
	*out = (SI_32)((UI_32)eng_temp_coolant | ((UI_32)eng_temp_oil << 8) | ((UI_32)trans_temp_oil << 16));
	return 0;
}

/********************************************************/
/* 関数名称		: get_signal_TirePressureFront			*/
/* 機能の説明	: フロントタイヤ圧取得					*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_TirePressureFront(SI_32 *out)
{
	F_64  unit;
	UI_16 fr = SIGNAL_VALUE_IGNORE_16;
	UI_16 fl = SIGNAL_VALUE_IGNORE_16;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if( (pCanInfo->en_air_press_unit == 1) && (pCanInfo->en_air_press_ftlh == 1) && (pCanInfo->en_air_press_ftrh == 1) )
	{
		/* タイヤ空気圧単位、タイヤ空気圧(FL輪),タイヤ空気圧(FR輪):有効 */
		if(pCanInfo->air_press_unit == CAN_TIREPRESS_UNIT_KPA)
		{
			/*単位kPa*/
			unit = CAN_TIREPRESS_RESOLUTION_KPA;
		}
		else if(pCanInfo->air_press_unit == CAN_TIREPRESS_UNIT_PSI)
		{
			/*単位psi*/
			/*1kPa = 0.145038 psi */
			/*単位pis -> kPa*/
			unit = CAN_TIREPRESS_RESOLUTION_PSI * (1 / 0.145038);
		}
		else if(pCanInfo->air_press_unit == CAN_TIREPRESS_UNIT_BAR)
		{
			/*単位bar*/
			/*1kPa = 0.01bar*/
			/*単位bar -> kPa*/
			unit = CAN_TIREPRESS_RESOLUTION_BAR * (1 / 0.01);
		}
		else
		{
			/*単位不明*/
			unit = 0;
		}
		if(unit > 0)
		{
			/*単位有効*/
			if(pCanInfo->air_press_ftlh != CAN_TIREPRESS_STATUS_CHECKING)
			{
				/*FL輪チェック中状態でない*/
				fl = (UI_16)((F_64)pCanInfo->air_press_ftlh * (unit / SIGNAL_VALUE_TIREPRESS_RESOLUTION));
			}
			else
			{
				/* nop */
			}
			if(pCanInfo->air_press_ftrh != CAN_TIREPRESS_STATUS_CHECKING)
			{
				/*FR輪チェック中状態でない*/
				fr = (UI_16)((F_64)pCanInfo->air_press_ftrh * (unit / SIGNAL_VALUE_TIREPRESS_RESOLUTION));
			}
			else
			{
				/* nop */
			}
		}
	}
	else
	{
		/* タイヤ空気圧単位、タイヤ空気圧(FL輪),タイヤ空気圧(FR輪):有効 */
		/*初期値*/
	}
	*out = (SI_32)((UI_32)fl | ((UI_32)fr << 16));
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_TirePressureRear			*/
/* 機能の説明	: リアタイヤ圧取得						*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_TirePressureRear(SI_32 *out)
{
	F_64  unit;
	UI_16 rr = SIGNAL_VALUE_IGNORE_16;
	UI_16 rl = SIGNAL_VALUE_IGNORE_16;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if( (pCanInfo->en_air_press_unit == 1) && (pCanInfo->en_air_press_rrlh == 1) && (pCanInfo->en_air_press_rrrh == 1) )
	{
		/* タイヤ空気圧単位、タイヤ空気圧(RL輪),タイヤ空気圧(RR輪):有効 */
		if(pCanInfo->air_press_unit == CAN_TIREPRESS_UNIT_KPA)
		{
			/*単位kPa*/
			unit = CAN_TIREPRESS_RESOLUTION_KPA;
		}
		else if(pCanInfo->air_press_unit == CAN_TIREPRESS_UNIT_PSI)
		{
			/*単位psi*/
			/*1kPa = 0.145038 psi */
			/*単位pis -> kPa*/
			unit = CAN_TIREPRESS_RESOLUTION_PSI * (1 / 0.145038);
		}
		else if(pCanInfo->air_press_unit == CAN_TIREPRESS_UNIT_BAR)
		{
			/*単位bar*/
			/*1kPa = 0.01bar*/
			unit = CAN_TIREPRESS_RESOLUTION_BAR * (1 / 0.01);
		}
		else
		{
			/*単位不明*/
			unit = 0;
		}
		if(unit > 0)
		{
			/*単位有効*/
			if(pCanInfo->air_press_rrlh != CAN_TIREPRESS_STATUS_CHECKING)
			{
				/*RL輪チェック中状態でない*/
				rl = (UI_16)((F_64)pCanInfo->air_press_rrlh * (unit / SIGNAL_VALUE_TIREPRESS_RESOLUTION));
			}
			else
			{
				/* nop */
			}
			if(pCanInfo->air_press_rrrh != CAN_TIREPRESS_STATUS_CHECKING)
			{
				/*RR輪チェック中状態でない*/
				rr = (UI_16)((F_64)pCanInfo->air_press_rrrh * (unit / SIGNAL_VALUE_TIREPRESS_RESOLUTION));
			}
			else
			{
				/* nop */
			}
		}
	}
	else
	{
		/* タイヤ空気圧単位、タイヤ空気圧(R輪),タイヤ空気圧(RR輪):有効 */
		/*初期値*/
	}
	*out = (SI_32)((UI_32)rl | ((UI_32)rr << 16));
	return 0;
}


/********************************************************/
/* 関数名称		: get_signal_FuelConsumption			*/
/* 機能の説明	: 燃料噴射量積算値取得					*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_FuelConsumption(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_fuel_consumption == 1)
	{
		/* 燃料噴射量積算値:有効 */
		*out = (SI_32)((F_64)pCanInfo->fuel_consumption * (CAN_FUEL_CONSUMPTION_RESOLUTION / SIGNAL_VALUE_FUEL_CONSUMPTION_RESOLUTION)); 
	}
	else
	{
		/* 燃料噴射量積算値:無効 */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_32;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_DistancefromRestart		*/
/* 機能の説明	: 始動後走行距離取得					*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_DistancefromRestart(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_running_distance == 1)
	{
		/* 始動後走行距離:有効 */
		*out = (SI_32)pCanInfo->running_distance;
	}
	else
	{
		/* 始動後走行距離:無効 */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_32;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_OutputTorque				*/
/* 機能の説明	: 出力トルク取得						*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_OutputTorque(SI_32 *out)
{
	SI_16 engTrq = SIGNAL_VALUE_IGNORE_16_SIGNED;
	SI_16 motTrq = SIGNAL_VALUE_IGNORE_16_SIGNED;

	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	/* エンジントルク軸有効チェック */
	if(pCanInfo->en_engTrcShaft == 1)
	{
		/* エンジントルク軸：有効 */
		engTrq = (SI_16)(pCanInfo->engTrcShaft + CAN_ENGTRQ_OFFSET_INC_RESOLUTION);
	}

	*out = (SI_32)(((UI_32)engTrq & 0x0000FFFF) | (((UI_32)motTrq << 16) & 0xFFFF0000));
	return 0;
}

/********************************************************/
/* 関数名称		： get_signal_EngineSensor				*/
/* 機能の説明	： エンジンセンサー						*/
/* 引数			： ( /O)out				取得値			*/
/* 戻り値		： 0									*/
/* 作成者		： (OFT)Sakata							*/
/* 作成日		： 21/03/-								*/
/* 備考			： 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineSensor(SI_32 *out)
{
	UI_16 oil_pressure = SIGNAL_VALUE_IGNORE_16;
	UI_16 intake_pressure = SIGNAL_VALUE_IGNORE_16;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	/* 吸入管圧力有効チェック */
	if(pCanInfo->en_intake_press == 1)
	{            
		/* 吸入管圧力：有効 */
		/*	出力値　=　((吸入管圧力 x 分解能 x 単位変換) - オフセット ) / 出力分解能（0.1） */
		/*	出力値　=　((吸入管圧力 x 分解能 x 単位変換) / 出力分解能（0.1)）- (オフセット / 出力分解能（0.1）) */
		intake_pressure = (UI_16)((SI_16)(((F_64)(pCanInfo->intake_press * CAN_INTAKE_PRESS_RESOLUTION) * SIGNAL_CONV_MMHG_TO_KPA) / SIGNAL_VALUE_INTAKE_PRESS_RESOLUTION) - (SI_16)((F_64)SIGNAL_VALUE_INTAKE_PRESS_OFFSET / SIGNAL_VALUE_INTAKE_PRESS_RESOLUTION));
	}
	else
	{
		/* 吸入管圧力：無効 */
		/* 初期値 */
	}
	*out = (SI_32)((UI_32)oil_pressure | ((UI_32)intake_pressure << 16));
	
	return 0;
}

/********************************************************/
/* 関数名称		： get_signal_AtmosphericPressure		*/
/* 機能の説明	： 大気圧								*/
/* 引数			： ( /O)out				取得値			*/
/* 戻り値		： 0									*/
/* 作成者		： (OFT)Sakata							*/
/* 作成日		： 21/03/-								*/
/* 備考			： 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_AtmosphericPressure(SI_32 *out)
{
	UI_16 atm_pressure = SIGNAL_VALUE_IGNORE_16;

	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	/* 大気圧有効チェック */
	if(pCanInfo->en_atmospheric_press == 1)
	{            
		/* 大気圧：有効 */
		/*	出力値　=　((吸入管圧力 x 分解能 + オフセット) x 単位変換) / 出力分解能（0.1） */
		atm_pressure = (UI_16)(((F_64)((pCanInfo->atmospheric_press * CAN_ATM_PRESS_RESOLUTION) + CAN_ATM_PRESS_OFFSET) * SIGNAL_CONV_MMHG_TO_KPA) / SIGNAL_VALUE_ATM_PRESS_RESOLUTION);
	}
	else
	{
		/* 大気圧：無効 */
		/* 初期値 */
	}
	*out = (SI_32)atm_pressure;
	
	return 0;
}

/********************************************************/
/* 関数名称		： get_signal_EngineRevRawValue			*/
/* 機能の説明	： エンジン回転数（生値）				*/
/* 引数			： ( /O)out				取得値			*/
/* 戻り値		： 0									*/
/* 作成者		： (OFT)Sakata							*/
/* 作成日		： 21/03/-								*/
/* 備考			： 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineRevRawValue(SI_32 *out)
{
	SI_32 randomValue;
	UI_16 engRevRaw = SIGNAL_VALUE_IGNORE_16;
	UI_8  targetGear = SIGNAL_VALUE_TARGET_GEAR_INIT;

	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	/* エンジン回転数(生値)有効チェック */
	if(pCanInfo->en_eng_rpm == 1)
	{
		/* エンジン回転数(生値):有効 */
		engRevRaw = pCanInfo->eng_rpm;
	}
	else
	{
		/* エンジン回転数(生値):無効 */
	}
	
	/*トランスミッション種別有効チェック*/
	if(pCanInfo->en_transmission_type == 1)
	{
		/* トランスミッション種別:有効 */
		if(pCanInfo->transmission_type == CAN_TRANSMISSION_ATORCVT)
		{
			/*トランスミッション種別:AT or CVT*/
			/*目標ギア段数有効チェック*/
			if(pCanInfo->en_target_gear_pos == 1) 
			{
				/*目標ギア段数：有効*/
				switch (pCanInfo->target_gear_pos)
				{
					case	CAN_TARGET_GEAR_N:		/* ATシフト:N	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_N;	
						break;
					case	CAN_TARGET_GEAR_D:		/* ATシフト:D	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_D;	
						break;
					case	CAN_TARGET_GEAR_R:		/* ATシフト:R	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_R;	
						break;
					case	CAN_TARGET_GEAR_P:		/* ATシフト:P	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_P;	
						break;
					case	CAN_TARGET_GEAR1ST:		/* ATギア：1ST	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_1ST;	
						break;
					case	CAN_TARGET_GEAR2ND:		/* ATギア：2ND	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_2ND;	
						break;
					case	CAN_TARGET_GEAR3RD:		/* ATギア：3RD	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_3RD;	
						break;
					case	CAN_TARGET_GEAR4TH:		/* ATギア：4TH	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_4TH;	
						break;
					case	CAN_TARGET_GEAR5TH:		/* ATギア：5TH	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_5TH;	
						break;
					case	CAN_TARGET_GEAR6TH:		/* ATギア：6TH	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_6TH;	
						break;
					default:
						/*その他*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_INVALID;
						break;
				}
			}
			else
			{
				/*目標ギア段数：無効*/
				/*処理なし：初期値*/
			}
		}else{
			/*トランスミッション種別:MT*/
			targetGear = SIGNAL_VALUE_TARGET_GEAR_INVALID;
		}
	}
	else
	{
		/*トランスミッション：無効*/
		/*処理なし：初期値*/
	}
	
	/* 乱数取得 */
	(void)get_signal_RandomValue(&randomValue);

	*out = (SI_32)(engRevRaw | ((UI_32)targetGear << 16) | ((UI_32)randomValue & 0xFF000000));
	
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_OutputUpDwnTorque			*/
/* 機能の説明	: 出力アップダウントルク取得			*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 21/03/-								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_OutputUpDwnTorque(SI_32 *out)
{
	UI_16 engTrqUp = SIGNAL_VALUE_IGNORE_16;
	UI_16 engTrqDwn = SIGNAL_VALUE_IGNORE_16;

	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	/* エンジントルクアップ要求値有効チェック */
	if(pCanInfo->en_engTrqUpReqVal == 1)
	{
		/* エンジントルクアップ要求値：有効 */
		if (pCanInfo->engTrqUpReqVal == CAN_ENGTRQ_REQ_MAXVAL)
		{
			/*最大値の場合－１する：無効値としないため*/
			engTrqUp = pCanInfo->engTrqUpReqVal - 1;
		}
		else
		{
			/*最大値以外の場合、そのまま設定*/
			engTrqUp = pCanInfo->engTrqUpReqVal;
		}
	}

	/* エンジントルクダウン要求値有効チェック */
	if(pCanInfo->en_engTrqDwnReqVal == 1)
	{
		/* エンジントルクダウン要求値：有効 */
		/*そのまま設定（最大値のチェックは不要:
		CAN_ENGTRQ_REQ_MAXVALのときはフェール時なのでen_engTrqDwnReqValが0となる）*/
		engTrqDwn = pCanInfo->engTrqDwnReqVal;
	}
	
	*out = (SI_32)(engTrqUp | (((UI_32)engTrqDwn << 16) & 0xFFFF0000));
	return 0;
}
#ifdef ADVANCE_ENABLE
/********************************************************/
/* 関数名称		: get_signal_IntakeManifoldPressure		*/
/* 機能の説明	: 吸気圧取得							*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IntakeManifoldPressure(SI_32 *out)
{
	T_ADVANCE_CTRL_UNIT_INFO* pAdvanceInfo;
	/* Advance Control Unitの信号取得 */
	Advance_control_GetData(&pAdvanceInfo);
	if(pAdvanceInfo->invalidFlag == 1)
	{
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	else
	{
		*out = (SI_32)pAdvanceInfo->TurboPressure;/* ターボ圧(過給圧) */
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_EngineOilPressure			*/
/* 機能の説明	: エンジン油圧取得						*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineOilPressure(SI_32 *out)
{
	T_ADVANCE_CTRL_UNIT_INFO* pAdvanceInfo;
	/* Advance Control Unitの信号取得 */
	Advance_control_GetData(&pAdvanceInfo);
	if(pAdvanceInfo->invalidFlag == 1)
	{
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	else
	{
		*out = (SI_32)pAdvanceInfo->EngineOilPressure;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_FuelPressure				*/
/* 機能の説明	: 燃料圧力取得							*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_FuelPressure(SI_32 *out)
{
	T_ADVANCE_CTRL_UNIT_INFO* pAdvanceInfo;
	/* Advance Control Unitの信号取得 */
	Advance_control_GetData(&pAdvanceInfo);
	if(pAdvanceInfo->invalidFlag == 1)
	{
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	else
	{
		*out = (SI_32)pAdvanceInfo->FuelPressure;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_EngineCoolantTempertureExt1*/
/* 機能の説明	: エンジン水温（拡張1）取得				*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineCoolantTempertureExt1(SI_32 *out)
{
	T_ADVANCE_CTRL_UNIT_INFO* pAdvanceInfo;
	/* Advance Control Unitの信号取得 */
	Advance_control_GetData(&pAdvanceInfo);
	if(pAdvanceInfo->invalidFlag == 1)
	{
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	else
	{
		*out = (SI_32)((F_64)pAdvanceInfo->EngineCoolantTemperture * 0.05);
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_EngineCoolantTempertureExt2*/
/* 機能の説明	: エンジン水温（拡張2）取得				*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineCoolantTempertureExt2(SI_32 *out)
{
	*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_EngineOilTemperture		*/
/* 機能の説明	: エンジン油温取得						*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineOilTemperture(SI_32 *out)
{
	T_ADVANCE_CTRL_UNIT_INFO* pAdvanceInfo;
	/* Advance Control Unitの信号取得 */
	Advance_control_GetData(&pAdvanceInfo);
	if(pAdvanceInfo->invalidFlag == 1)
	{
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	else
	{
		*out = (SI_32)((F_64)pAdvanceInfo->EngineOiTemperture * 0.05);
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_ExhaustGasTemperture		*/
/* 機能の説明	: 排気温度取得							*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_ExhaustGasTemperture(SI_32 *out)
{
	T_ADVANCE_CTRL_UNIT_INFO* pAdvanceInfo;
	/* Advance Control Unitの信号取得 */
	Advance_control_GetData(&pAdvanceInfo);
	if(pAdvanceInfo->invalidFlag == 1)
	{
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	else
	{
		*out = (SI_32)((F_64)pAdvanceInfo->ExhaustGasTemperture * 0.2);
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_Volts						*/
/* 機能の説明	: 電圧取得								*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Volts(SI_32 *out)
{
	T_ADVANCE_CTRL_UNIT_INFO* pAdvanceInfo;
	/* Advance Control Unitの信号取得 */
	Advance_control_GetData(&pAdvanceInfo);
	if(pAdvanceInfo->invalidFlag == 1)
	{
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	else
	{
		*out = (SI_32)pAdvanceInfo->Volts;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_AirFuelRatio				*/
/* 機能の説明	: 空圧比取得							*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_AirFuelRatio(SI_32 *out)
{
	*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	return 0;
}
#endif /*ADVANCE_ENABLE*/
#ifdef	ISOCAN_ENABLE
/********************************************************/
/* 関数名称		: get_signal_IsoCanVehicleSpeed			*/
/* 機能の説明	: 車速取得								*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IsoCanVehicleSpeed(SI_32 *out)
{
	T_Can_ctrl_iso_can_info* pIsoCanInfo = Can_control_GetIsoCanInfo();
	if(pIsoCanInfo->en_vehicle_speed == 1)
	{
		/* 車速:有効 */
		*out = pIsoCanInfo->vehicle_speed;
	}
	else
	{
		/* 車速:無効 */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_IsoCanEngineRev			*/
/* 機能の説明	: エンジン回転数取得					*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IsoCanEngineRev(SI_32 *out)
{
	T_Can_ctrl_iso_can_info* pIsoCanInfo = Can_control_GetIsoCanInfo();
	if(pIsoCanInfo->en_eng_rpm == 1)
	{
		/* エンジン回転数:有効 */
		*out = pIsoCanInfo->eng_rpm * 0.25;
	}
	else
	{
		/* エンジン回転数:無効 */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_IsoCanIntakeManifoldPressure*/
/* 機能の説明	: 吸気圧取得							*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IsoCanIntakeManifoldPressure(SI_32 *out)
{
	T_Can_ctrl_iso_can_info* pIsoCanInfo = Can_control_GetIsoCanInfo();
	if(pIsoCanInfo->en_ima_pressure == 1)
	{
		/* 吸気圧:有効 */
		*out = pIsoCanInfo->ima_pressure;
	}
	else
	{
		/* 吸気圧:無効 */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_IsoCanEngineCoolantTemperture*/
/* 機能の説明	: エンジン水温取得						*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IsoCanEngineCoolantTemperture(SI_32 *out)
{
	T_Can_ctrl_iso_can_info* pIsoCanInfo = Can_control_GetIsoCanInfo();
	if(pIsoCanInfo->en_temp_colant == 1)
	{
		/* エンジン水温:有効 */
		*out = pIsoCanInfo->temp_colant + ISOCAN_ENG_TEMP_COOLANT_OFFSET;
	}
	else
	{
		/* エンジン水温:無効 */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_IsoCanVolts				*/
/* 機能の説明	: 電圧取得								*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IsoCanVolts(SI_32 *out)
{
	T_Can_ctrl_iso_can_info* pIsoCanInfo = Can_control_GetIsoCanInfo();
	if(pIsoCanInfo->en_volts == 1)
	{
		/* 電圧:有効 */
		*out = (SI_32)pIsoCanInfo->volts * (0.001 / 0.1);
	}
	else
	{
		/* 電圧:無効 */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_IsoCanThrottle				*/
/* 機能の説明	: スロットル開度取得					*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IsoCanThrottle(SI_32 *out)
{
	T_Can_ctrl_iso_can_info* pIsoCanInfo = Can_control_GetIsoCanInfo();
	if(pIsoCanInfo->en_throttle_pos == 1)
	{
		/* スロットル開度:有効 */
		/* PID最新.xlsに計算方法記述 */
		*out = pIsoCanInfo->throttle_pos * 100 / 255;
	}
	else
	{
		/* スロットル開度:無効 */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
/********************************************************/
/* 関数名称		: get_signal_IsoCanIntakeTemperature	*/
/* 機能の説明	: 吸気温度取得							*/
/* 引数			: ( /O)out				取得値			*/
/* 戻り値		: 0										*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IsoCanIntakeTemperature(SI_32 *out)
{
	T_Can_ctrl_iso_can_info* pIsoCanInfo = Can_control_GetIsoCanInfo();
	if(pIsoCanInfo->en_temp_intake_manifold == 1)
	{
		/* 吸気温度:有効 */
		*out = pIsoCanInfo->temp_intake_manifold;
	}
	else
	{
		/* 吸気温度:無効 */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
#endif	/* ISOCAN_ENABLE */

