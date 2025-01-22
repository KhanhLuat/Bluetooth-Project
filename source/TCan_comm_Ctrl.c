/********************************************************/
/* file TCan_comm_Ctrl.c								*/
/* T-CAN制御											*/
/* 														*/
/* T-CANのメッセージの抽出、積算を行う。				*/
/* <ul>													*/
/* <li>													*/
/* </ul>												*/
/* 														*/
/********************************************************/
/* author      NSCS										*/
/* date        2013/12/--								*/
/********************************************************/


#include "type.h"
#include "DTO459_VDV_Can_Drv.h"
#include "TCan_comm_Ctrl.h"
#include "timer_ctrl.h"
#include <string.h>
#include "dbglog.h"



/********************************************************/
/*	内部マクロ											*/
/********************************************************/
#define TCAN_COMMON_CTRL_LOCAL	static

#define		SP_RUN_TH_MT		(31)				/* 1.75[km/h]相当値(MT)  分解能:0.05625km/h */
#define		SP_RUN_TH_HV		(175)				/* 1.75[km/h]相当値(HV)  分解能:0.01km/h */

#define		COEF				(19.623233909f)		/* 始動後走行距離変換係数:100000÷(2×637×NP[4]) */
#define		SP_MAX_TH			(218871528UL)		/* SP-ODO最大値:(2^32)÷COEF) */

#define		FUEL_CYCLE			(50)				/* 燃料噴射量積算値 算出周期:500ms */

#define		UI32_MAX			(4294967295UL)		

#define		WHEEL_AVE_MIN		(0x0018)			/* 駆動輪軸平均車輪速 最小 */
#define		WHEEL_AVE_MAX		(0x14D6)			/* 駆動輪軸平均車輪速 最大 */

#define		WHEEL_AVE_MIN_86B	(0x000D)			/* 駆動輪軸平均車輪速 最小 */
#define		WHEEL_AVE_MAX_86B	(0x14D5)			/* 駆動輪軸平均車輪速 最大 */

#define 	CAN_CH_TCAN							CAN_CH_0
#define 	VEHICLE_SPEED_RESOLUTION			100000
#ifdef _FOR_RCF
#define 	VSO_RESOLUTION						(0.01 * VEHICLE_SPEED_RESOLUTION)
#else	/* _FOR_RCF */
#define 	VSO_RESOLUTION						(0.05625 * VEHICLE_SPEED_RESOLUTION)
#endif	/* _FOR_RCF */
#define 	TIMEOUT_0D1							120
#define 	VEHICLE_SPEED_HOLD_TIME_86A				280
#define 	VEHICLE_SPEED_VALUE_COUNT_MAX_86A		14 /*14回分保持x20ms = 280ms*/
#define 	VEHICLE_SPEED_HOLD_TIME_86B				300/*300ms間隔で更新*/
#define 	VEHICLE_SPEED_VALUE_COUNT_MAX_86B		10 /*10回分保持して平均化*/
#define 	VEHICLE_SPEED_VALUE_COUNT_MAX			VEHICLE_SPEED_VALUE_COUNT_MAX_86A /*車速保持数：86A,86Bで保持数の大きい方を設定*/
#define 	VEHICLE_BRAKE_PRESS_COUNT_MAX		6				/*ブレーキ圧保持数*/
#define 	THRESHOLD_SPEED_MAX					(300 * VEHICLE_SPEED_RESOLUTION)
#define 	VEHICLEINFO_NUM						2				/* 仕向地速度算出テーブル車種配列数 */
#define 	TBL_NUM								15				/* 仕向地速度算出テーブル配列数 */
#define 	DIRECTION_TBL_NUM					16
#define 	TIMEOUT_SUM_FC						1000			/* 燃料消費量積算処理タイムアウト値(ms) */

#define 	BRAKE_PRESS_SMOOTHINGCOEFFICIENT	(0.2)		/* ブレーキ圧なまし係数					*/
#define 	SMOOTHING_BRAKE_PRESS_RESOLUTION	10000		/* なまし処理ブレーキ圧分解能			*/
#define 	BRAKE_PRESS_RESOLUTION				(0.1305882 * SMOOTHING_BRAKE_PRESS_RESOLUTION)	/* ブレーキ圧分解能	*/

#define 	BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX	10				/*ブランドフラグ保持数*/
#define 	BRANDFLAG_RECEIVE_INTERVAL_86A			100				/*86Aブランドフラグ受信間隔ms*/
#define 	BRANDFLAG_RECEIVE_INTERVAL_86B			1500			/*86Bブランドフラグ受信間隔ms*/
#define 	VEHICLEINFO_PROC_INTERVAL				10000			/*車両判定情報間隔ms*/

/********************************************************/
/*	内部変数定義										*/
/********************************************************/

/********************************************************/
/*	定数宣言											*/
/********************************************************/

/********************************************************/
/*	構造体宣言											*/
/********************************************************/
/* CAN情報(UI_8用) */
typedef struct t_can_info_ui8 {
	UI_8	data;				/* データ */
	UI_8	first_flg;			/* 初回受信フラグ */
	UI_8	fail_flg;			/* フェールフラグ */
} T_CanInfo_UI8;

/* CAN情報(UI_16用) */
typedef struct t_can_info_ui16 {
	UI_16	data;				/* データ */
	UI_8	first_flg;			/* 初回受信フラグ */
	UI_8	fail_flg;			/* フェールフラグ */
} T_CanInfo_UI16;

/*公差指示テーブルの型*/
typedef struct{
	UI_32		speed;		/* 車速(km/h) * 100000 */
	F_32		slope;		/* 傾き */
	SI_32		intercept;	/* 切片(km/h) * 100000 */
}S_CENTER_TOLE_TBL;

/********************************************************/
/*	内部変数定義										*/
/********************************************************/
/* 始動後走行距離 */
TCAN_COMMON_CTRL_LOCAL	UI_32	OdoTrip;				/* 始動後走行距離[0.01m] */
TCAN_COMMON_CTRL_LOCAL	UI_32	SpOdo;					/* 車速パルス総積算値 */
TCAN_COMMON_CTRL_LOCAL	UI_8	Sp1p_old;				/* 車速パルス信号積算値 前回値 */

/* 燃料噴射積算値 */
TCAN_COMMON_CTRL_LOCAL	UI_32	Sum_Fc;					/* 燃料噴射量積算値 一定期間積算値 */
TCAN_COMMON_CTRL_LOCAL	UI_16	Sum_Fc_N;				/* 燃料噴射量積算値 積算回数 */
TCAN_COMMON_CTRL_LOCAL	UI_8	FuelProcCycleCnt;		/* 燃料噴射量積算値 算出周期カウンタ */
TCAN_COMMON_CTRL_LOCAL	UI_32	FC_Sum;					/* 燃料噴射量積算値 (最終確定値) */
TCAN_COMMON_CTRL_LOCAL UI_8  S_OdoTrip_enable;			/* OdoTrip値の有効フラグ				*/
TCAN_COMMON_CTRL_LOCAL UI_8  S_FC_Sum_enable;			/* Sum_Fc値の有効フラグ					*/

/* MT */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtDDoorSW;			/* ドアSW D席 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtPDoorSW;			/* ドアSW P席 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtTrunkDoorSW;		/* ドアSW トランク */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtRatioAccel;		/* アクセル開度率 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtEngRpm;			/* エンジン回転数 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtEngRpm_Ta;		/* エンジン回転数(タコメータ用) */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtGearEstm;			/* MTギアポジション表示情報 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtSpWheelAve;		/* 駆動輪軸平均車輪速(VSO) */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtSpPulseInteg;		/* 車速パルス信号積算値(SP1P) */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtBreakLampSW;		/* ブレーキランプSW */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtHandBreakSW;		/* パーキングブレーキSW */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtReverseSW;		/* リバースSW */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtHeadLampHi;		/* ヘッドランプHi */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtHeadLampLo;		/* ヘッドランプLo */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtHeadLampSmall;	/* ヘッドランプSmarll */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtOutTemp;			/* 表示用外気温 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtPSeatBeltSW;		/* シートベルトバックルSW P席 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtDSeatBeltSW;		/* シートベルトバックルSW D席 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtTempOil;			/* エンジン油温 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtTempCoolant;		/* エンジン水温 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtSetCc;			/* クルコンセットランプ状態 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtSteerAngle;		/* 舵角センサ */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtYawRate;			/* ヨーレート */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtAccLR;			/* 横加速度 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtAccFR;			/* 前後Gセンサ */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtTrnsType;			/* トランスミッション情報 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtSportGearPos;		/* スポーツシフトギア位置 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtShiftPos;			/* シフト位置 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtModeShiftCtrl;	/* TCM変速制御モード情報 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtAtType;			/* AT種類 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtPlace;			/* 仕向地情報 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtIgn;				/* IGN						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtAcc;				/* ACC						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtBrandFlag;		/* ブランドフラグ			*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtTargetGearPos;	/* 目標ギア位置				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtSportsLamp;		/* スポーツランプ			*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtSnowHoldLamp;		/* スノーホールドランプ		*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtNodeID;			/* ノードID					*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtSleepAck;			/* Sleep Ack				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtSleepInd;			/* Sleep Ind				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtLimpHome;			/* Limp Home				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtRing;				/* Ring						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtAlive;			/* Alive					*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtWkup;				/* Wkup						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtInit;				/* Init						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtSpecNo;			/* Spec. No.				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtSpdWheelFtlh;		/* 左前輪車輪速				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtSpdWheelFtrh;		/* 右前輪車輪速				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtSpdWheelRrlh;		/* 左後輪車輪速				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtSpdWheelRrrh;		/* 右後輪車輪速				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtDoorLockStatPass;	/* ドアロック状態（助手席）	*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtDoorLockStatDrvr;	/* ドアロック状態（運転席）	*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtBrakePressure;	/* ブレーキ圧				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAirPressFtlh;		/* 左前輪空気圧 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAirPressFtrh;		/* 右前輪空気圧 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAirPressRrlh;		/* 左後輪空気圧 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAirPressRrrh;		/* 右後輪空気圧 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAirPressUnit;		/* 空気圧単位 */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtManualModeSW;		/* マニュアルモードSW */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtVscOnOff;			/* VSC ON/OFF情報（VSC OFFランプ状態） 	*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtVscAct;			/* VSC作動状態(VSC,TRC両方)				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtTrcMode;			/* トラクション重視モード 				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtTrcOnOff;			/* TRC ON/OFF情報（TRC OFFランプ状態） 	*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtTrcAct;			/* TRC作動状態 							*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAbsAct;			/* ABS作動状態 							*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtVscWarning;		/* VSCウォーニング						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAbsWarning;		/* ABSウォーニング 						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAbsFail;			/* ABSシステムフェイルフラグ			*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtVscUnderStr;		/* VSCアンダー制御状態					*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtVscOverStr;		/* VSCオーバー制御状態					*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtEngTrqUpReqFlg;	/* エンジントルクアップ要求フラグ		*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtEngTrqDwnReqFlg;	/* エンジントルク減少要求フラグ			*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	AtEngTrqUpReqVal;	/* エンジントルクアップ要求値			*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	AtEngTrqDwnReqVal;	/* エンジントルク減少要求値				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	AtEngTrqShaft;		/* 実エンジン軸トルク					*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtEcbWarning;		/* ECBウォーニング						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtIntakePress;		/* 吸入管圧力							*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAtmosphericPress;	/* 大気圧								*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtBlinkerLeft;		/* ウィンカー左							*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtBlinkerRight;		/* ウィンカー右							*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtHoodDoorSW;		/* ドアSW フード						*/

TCAN_COMMON_CTRL_LOCAL  UI_32	S_can_0d1_received_time;				/* 0D1メッセージ最終受信時刻	*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_vehicle_speed_value_buf[VEHICLE_SPEED_VALUE_COUNT_MAX];	/* 車速値保持バッファ	*/
TCAN_COMMON_CTRL_LOCAL  UI_8  	S_vehicle_speed_value_count;			/* 車速値保持バッファデータ数		*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_speed_value_mem;						/* 車速メモリ保持値					*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_speed_value_dsp;						/* 車速表示値						*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_speed_proc_data_time;					/* 表示用車速計算処理前回処理時刻	*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_speed_value_updated_time;				/* 車速表示値前回更新時刻			*/
TCAN_COMMON_CTRL_LOCAL  UI_8    S_speed_value_dsp_enable;				/* S_speed_value_dsp値の有効フラグ	*/

TCAN_COMMON_CTRL_LOCAL  UI_32 	S_brake_press_smooth;					/* なましブレーキ圧値							*/
TCAN_COMMON_CTRL_LOCAL  UI_8    S_brake_press_smooth_enable;			/* なましブレーキ圧値の有効フラグ				*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_brake_press_proc_data_time;			/* ブレーキ圧前回処理時刻						*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_vehicle_brake_press_buf[VEHICLE_BRAKE_PRESS_COUNT_MAX];	/* ブレーキ圧保持バッファ	*/
TCAN_COMMON_CTRL_LOCAL  UI_8  	S_vehicle_brake_press_count;			/* ブレーキ圧保持バッファデータ数				*/

TCAN_COMMON_CTRL_LOCAL  UI_32 	S_vehicle_proc_time;								/*車両情報判定時間					*/
TCAN_COMMON_CTRL_LOCAL E_TCAN_VEHICLE_INFO S_vehicleJudgeInfo;						/*車両種別:内部判定*/
TCAN_COMMON_CTRL_LOCAL E_TCAN_VEHICLE_INFO S_vehicleCanInfo;						/*車両種別:CAN受信状態*/
/*
CAN受信状態	：上位モジュールから指定される。この状態に応じたCANIDを受信する
			  演算処理はこの状態に応じて実施する。Unknown状態時は信号演算しない。
			  起動時には必ず上位から指定される。
内部判定	：TCanモジュール内で判定した状態。上位からCAN受信状態が設定されると本状態も合わせて変更する。
			  内部判定では86A/86BのみでUnknownになることはない。（初期状態を除く）
			  内部判定状態が変化すると上位からCAN受信状態の設定(TCAN_Ctrl_SetupCanMessage)が行われる。
----------------------------------------------------------
CAN受信状態			内部判定				信号演算
(S_vehicleCanInfo）	(S_vehicleJudgeInfo)	
------------------------------------------------------------
Unknown				Unknown					車種判定のみ
					86A						車種判定のみ
					86B						車種判定のみ
------------------------------------------------------------
86A					-(なし）				-
					86A						86A
					86B						86A
------------------------------------------------------------
86B					-(なし）				-
					86A						86B
					86B						86B
------------------------------------------------------------*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_brandflag_receive_interval_86A[BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX];	/*86Aブランドフラグ受信間隔				*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_brandflag_receive_interval_86B[BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX];	/*86Bブランドフラグ受信間隔				*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_brandflag_receive_lasttime_86A;										/*86Aブランドフラグ前回受信時刻			*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_brandflag_receive_lasttime_86B;										/*86Bブランドフラグ前回受信時刻			*/
TCAN_COMMON_CTRL_LOCAL  UI_8 	S_brandflag_receive_count_86A;											/*86Aブランドフラグ受信回数				*/
TCAN_COMMON_CTRL_LOCAL  UI_8 	S_brandflag_receive_count_86B;											/*86Bブランドフラグ受信回数				*/
TCAN_COMMON_CTRL_LOCAL  UI_16   S_speed_value_hold_time;							/* 表示用車速保持時間	*/
TCAN_COMMON_CTRL_LOCAL  UI_8    S_speed_value_count_max;							/* 車速保持数			*/


/* 仕向地テーブルインデクス*/
typedef enum e_sp_direction
{
	SP_DIRECTION_U4_86A = 0,
	SP_DIRECTION_C0_86A,
	SP_DIRECTION_KA_86A,
	SP_DIRECTION_EC_86A,
	SP_DIRECTION_EK_86A,
	SP_DIRECTION_U4_86B,
	SP_DIRECTION_C0_86B,
	SP_DIRECTION_KA_86B,
	SP_DIRECTION_EC_86B,
	SP_DIRECTION_EK_86B,
	SP_DIRECTION_MAX
}E_SP_DIRECTION;

/* 仕向地から対象の速度算出テーブルインデクスに変換*/ 
TCAN_COMMON_CTRL_LOCAL const E_SP_DIRECTION C_Direction2SP[VEHICLEINFO_NUM][DIRECTION_TBL_NUM] = 
{
	{/*086A*/
		SP_DIRECTION_EC_86A,/* NO_DIRECTION (不定はJPN) */
		SP_DIRECTION_EC_86A,/* JPN */
		SP_DIRECTION_U4_86A,/* U4 */
		SP_DIRECTION_C0_86A,/* C0 */
		SP_DIRECTION_EC_86A,/* EC */
		SP_DIRECTION_EC_86A,/* K4 */
		SP_DIRECTION_EC_86A,/* KS */
		SP_DIRECTION_EK_86A,/* EK */
		SP_DIRECTION_EC_86A,/* ER */
		SP_DIRECTION_KA_86A,/* KA */
		SP_DIRECTION_EC_86A,/* EH */
		SP_DIRECTION_C0_86A,/* C6 */
		SP_DIRECTION_EC_86A,/* KC */
		SP_DIRECTION_EC_86A,/* OTHER */
		SP_DIRECTION_EC_86A,/* OTHER */
		SP_DIRECTION_EC_86A /* OTHER */
	},
	{/*086B 4パターンのみ JP,U4,C0,EK */
		SP_DIRECTION_EC_86B,/* NO_DIRECTION (不定はJPN) */
		SP_DIRECTION_EC_86B,/* JPN */
		SP_DIRECTION_U4_86B,/* U4 */
		SP_DIRECTION_C0_86B,/* C0 */
		SP_DIRECTION_EC_86B,/* EC */
		SP_DIRECTION_EC_86B,/* K4 */
		SP_DIRECTION_EC_86B,/* KS */
		SP_DIRECTION_EK_86B,/* EK */
		SP_DIRECTION_EC_86B,/* ER */
		SP_DIRECTION_EC_86B,/* KA */
		SP_DIRECTION_EC_86B,/* EH */
		SP_DIRECTION_EC_86B,/* C6 */
		SP_DIRECTION_EC_86B,/* KC */
		SP_DIRECTION_EC_86B,/* OTHER */
		SP_DIRECTION_EC_86B,/* OTHER */
		SP_DIRECTION_EC_86B /* OTHER */
	}
};



/* 速度算出テーブル */
/*①米国規格 SP_DIRECTION_U4 */
TCAN_COMMON_CTRL_LOCAL const S_CENTER_TOLE_TBL center_tole_us[VEHICLEINFO_NUM][TBL_NUM] = {
	{/*086A*/
		{	 2000000	,1.030f ,     0		},
		{	 4000000	,1.018f , 23439		},
		{	 6000000	,1.000f	, 96561		},
		{	 8000000	,1.000f	, 96561		},
		{	10000000	,1.000f	, 96561		},
		{	12000000	,1.000f	, 96561		},
		{	14000000	,1.000f	, 96561		},
		{	16000000	,1.000f	, 96561		},
		{	18000000	,1.000f	, 96561		},
		{	20000000	,1.000f , 96561		},
		{	22000000	,1.000f , 96561		},
		{	24000000	,1.000f , 96561		},
		{	26000000	,1.000f , 96561		},
		{	28000000	,1.000f , 96561		},
		{	30000000	,1.000f , 96561		},
	},
	{/*086B MPH->km/hに変換しているため車速が20km/h単位ではない*/
		{	1600000		,1.03125f 	, 0		},/* 10MPHをkm/hに変換 */
		{	3200000		,1.00625f 	, 40000	},/* 20MPHをkm/hに変換 */
		{	6400000		,1.003125f	, 50000	},/* 40MPHをkm/hに変換 */
		{	9600000		,1.00625f	, 30000	},/* 60MPHをkm/hに変換 */
		{	12800000	,1.003125f	, 60000	},/* 80MPHをkm/hに変換 */
		{	16000000	,1.00625f 	, 20000	},/* 100MPHをkm/hに変換 */
		{	19300000	,1.003030303f,71515	},/* 120MPHをkm/hに変換 */
		{	22500000	,1.003125f 	, 69687	},/* 140MPHをkm/hに変換 */
		{	25700000	,1.00625f 	, -625	},/* 160MPHをkm/hに変換 */
		{	28900000	,1.003125f 	, 79687	},/* 180MPHをkm/hに変換 */
		{	29600000	,1.003125f 	, 79687	},/* これ以上はないので180MPHの値をそのまま使用(数を揃えるため車速は300km/hの範囲内で設定)*/
		{	29700000	,1.003125f 	, 79687	},/* これ以上はないので180MPHの値をそのまま使用(数を揃えるため車速は300km/hの範囲内で設定)*/
		{	29800000	,1.003125f 	, 79687	},/* これ以上はないので180MPHの値をそのまま使用(数を揃えるため車速は300km/hの範囲内で設定)*/
		{	29900000	,1.003125f 	, 79687	},/* これ以上はないので180MPHの値をそのまま使用(数を揃えるため車速は300km/hの範囲内で設定)*/
		{	30000000	,1.003125f 	, 79687	},/* これ以上はないので180MPHの値をそのまま使用(数を揃えるため車速は300km/hの範囲内で設定)*/
	}
};

/*②カナダ規格 SP_DIRECTION_C0*/
TCAN_COMMON_CTRL_LOCAL const S_CENTER_TOLE_TBL center_tole_ca[VEHICLEINFO_NUM][TBL_NUM] = {
	{/*086A*/
		{	 2000000	,1.050f ,     0		},
		{	 4000000	,1.000f ,100000		},
		{	 6000000	,1.000f	,100000		},
		{	 8000000	,1.000f	,100000		},
		{	10000000	,1.000f	,100000		},
		{	12000000	,1.000f	,100000		},
		{	14000000	,1.000f	,100000		},
		{	16000000	,1.000f	,100000		},
		{	18000000	,1.000f	,100000		},
		{	20000000	,1.000f ,100000		},
		{	22000000	,1.000f ,100000		},
		{	24000000	,1.000f ,100000		},
		{	26000000	,1.000f ,100000		},
		{	28000000	,1.000f ,100000		},
		{	30000000	,1.000f ,100000		},
	},
	{/*086B*/
		{	 2000000	,1.045f ,   	0	},
		{	 4000000	,1.005f ,	80000	},
		{	 6000000	,1.000f	,	100000	},
		{	 8000000	,1.005f	,	70000	},
		{	10000000	,1.005f	,	70000	},
		{	12000000	,1.000f	,	120000	},
		{	14000000	,1.005f	,	60000	},
		{	16000000	,1.000f	,	130000	},
		{	18000000	,1.005f	,	50000	},
		{	20000000	,1.005f ,	50000	},
		{	22000000	,1.000f ,	150000	},
		{	24000000	,1.005f ,	40000	},
		{	26000000	,1.000f ,	160000	},
		{	28000000	,1.005f ,	30000	},
		{	30000000	,1.005f ,	30000	},/* これ以上はないので280km/hの値をそのまま使用*/
	}
};

/*③豪州規格 SP_DIRECTION_KA */
TCAN_COMMON_CTRL_LOCAL const S_CENTER_TOLE_TBL center_tole_au[VEHICLEINFO_NUM][TBL_NUM] = {
	{/*086A*/
		{	 2000000	,1.140f ,     0		},
		{	 4000000	,1.025f ,230000		},
		{	 6000000	,1.025f	,230000		},
		{	 8000000	,1.025f	,230000		},
		{	10000000	,1.025f	,230000		},
		{	12000000	,1.025f	,230000		},
		{	14000000	,1.025f	,230000		},
		{	16000000	,1.025f	,230000		},
		{	18000000	,1.025f	,230000		},
		{	20000000	,1.025f ,230000		},
		{	22000000	,1.025f ,230000		},
		{	24000000	,1.025f ,230000		},
		{	26000000	,1.025f ,230000		},
		{	28000000	,1.025f ,230000		},
		{	30000000	,1.025f ,230000		},
	},
	{/*086B:86Bではこの仕向は無いなのでSP_DIRECTION_ECと同じ値を設定*/
		{	 2000000	,1.04f	,	 0		},/*SP_DIRECTION_ECの値を設定*/
		{	 4000000	,1.02f	,40000		},/*SP_DIRECTION_ECの値を設定*/
		{	 6000000	,1.02f	,40000		},/*SP_DIRECTION_ECの値を設定*/
		{	 8000000	,1.02f	,40000		},/*SP_DIRECTION_ECの値を設定*/
		{	10000000	,1.02f	,40000		},/*SP_DIRECTION_ECの値を設定*/
		{	12000000	,1.02f	,40000		},/*SP_DIRECTION_ECの値を設定*/
		{	14000000	,1.02f	,40000		},/*SP_DIRECTION_ECの値を設定*/
		{	16000000	,1.02f	,40000		},/*SP_DIRECTION_ECの値を設定*/
		{	18000000	,1.02f	,40000		},/*SP_DIRECTION_ECの値を設定*/
		{	20000000	,1.02f	,40000		},/*SP_DIRECTION_ECの値を設定*/
		{	22000000	,1.02f	,40000		},/*SP_DIRECTION_ECの値を設定*/
		{	24000000	,1.02f	,40000		},/*SP_DIRECTION_ECの値を設定*/
		{	26000000	,1.02f	,40000		},/*SP_DIRECTION_ECの値を設定*/
		{	28000000	,1.02f	,40000		},/*SP_DIRECTION_ECの値を設定*/
		{	30000000	,1.02f	,40000		},/*SP_DIRECTION_ECの値を設定*/
	}
};

/*④ECE39（km/h）対応規格（豪州以外）SP_DIRECTION_EC */
TCAN_COMMON_CTRL_LOCAL const S_CENTER_TOLE_TBL center_tole_jp_eu[VEHICLEINFO_NUM][TBL_NUM] = {
	{/*086A*/
		{	 2000000	,1.140f ,     0		},
		{	 4000000	,1.045f ,190000		},
		{	 6000000	,1.045f	,190000		},
		{	 8000000	,1.045f	,190000		},
		{	10000000	,1.045f	,190000		},
		{	12000000	,1.045f	,190000		},
		{	14000000	,1.045f	,190000		},
		{	16000000	,1.045f	,190000		},
		{	18000000	,1.045f	,190000		},
		{	20000000	,1.045f ,190000		},
		{	22000000	,1.045f ,190000		},
		{	24000000	,1.045f ,190000		},
		{	26000000	,1.045f ,190000		},
		{	28000000	,1.045f ,190000		},
		{	30000000	,1.045f ,190000		},
	},
	{/*086B*/
		{	 2000000	,1.04f	,	 0		},
		{	 4000000	,1.02f	,40000		},
		{	 6000000	,1.02f	,40000		},
		{	 8000000	,1.02f	,40000		},
		{	10000000	,1.02f	,40000		},
		{	12000000	,1.02f	,40000		},
		{	14000000	,1.02f	,40000		},
		{	16000000	,1.02f	,40000		},
		{	18000000	,1.02f	,40000		},
		{	20000000	,1.02f	,40000		},
		{	22000000	,1.02f	,40000		},
		{	24000000	,1.02f	,40000		},
		{	26000000	,1.02f	,40000		},
		{	28000000	,1.02f	,40000		},/* これ以上はないので260km/hの値をそのまま使用*/
		{	30000000	,1.02f	,40000		},/* これ以上はないので260km/hの値をそのまま使用*/
	}
};

/*⑤ECE39（MPH）対応規格（豪州以外）SP_DIRECTION_EK */
TCAN_COMMON_CTRL_LOCAL const S_CENTER_TOLE_TBL center_tole_jp_uk[VEHICLEINFO_NUM][TBL_NUM] = {
	{/*086A*/
		{	 2000000	,1.100f ,     0		},
		{	 4000000	,1.079f , 42972		},
		{	 6000000	,1.045f	,177028		},
		{	 8000000	,1.045f	,177028		},
		{	10000000	,1.045f	,177028		},
		{	12000000	,1.045f	,177028		},
		{	14000000	,1.045f	,177028		},
		{	16000000	,1.045f	,177028		},
		{	18000000	,1.045f	,177028		},
		{	20000000	,1.045f ,177028		},
		{	22000000	,1.045f ,177028		},
		{	24000000	,1.045f ,177028		},
		{	26000000	,1.045f ,177028		},
		{	28000000	,1.045f ,177028		},
		{	30000000	,1.022f ,827067		},
	},
	{/*086B*/
		{	1600000		,1.025f 	, 		0	},/* 10MPHをkm/hに変換 */
		{	3200000		,1.0125f 	, 	20000	},/* 20MPHをkm/hに変換 */
		{	6400000		,1.0125f	, 	20000	},/* 40MPHをkm/hに変換 */
		{	9600000		,1.009375f	, 	40000	},/* 60MPHをkm/hに変換 */
		{	12800000	,1.0125f	, 	10000	},/* 80MPHをkm/hに変換 */
		{	16000000	,1.0125f	 ,	10000	},/* 100MPHをkm/hに変換 */
		{	19300000	,1.012121212f,	16060	},/* 120MPHをkm/hに変換 */
		{	22500000	,1.0125f 	, 	8750	},/* 140MPHをkm/hに変換 */
		{	25700000	,1.009375f 	, 	79062	},/* 160MPHをkm/hに変換 */
		{	28900000	,1.0125f 	, 	-1250	},/* 180MPHをkm/hに変換 */
		{	29600000	,1.0125f 	, 	-1250	},/* これ以上はないので180MPHの値をそのまま使用(数を揃えるため車速は300km/hの範囲内で設定)*/
		{	29700000	,1.0125f 	, 	-1250	},/* これ以上はないので180MPHの値をそのまま使用(数を揃えるため車速は300km/hの範囲内で設定)*/
		{	29800000	,1.0125f 	, 	-1250	},/* これ以上はないので180MPHの値をそのまま使用(数を揃えるため車速は300km/hの範囲内で設定)*/
		{	29900000	,1.0125f 	, 	-1250	},/* これ以上はないので180MPHの値をそのまま使用(数を揃えるため車速は300km/hの範囲内で設定)*/
		{	30000000	,1.0125f 	, 	-1250	},/* これ以上はないので180MPHの値をそのまま使用(数を揃えるため車速は300km/hの範囲内で設定)*/
	}
};

/* 速度算出テーブルを仕向地と対応させるテーブル */
TCAN_COMMON_CTRL_LOCAL const S_CENTER_TOLE_TBL* C_center_tole[SP_DIRECTION_MAX] = {
	center_tole_us[0],			/*086A*/
	center_tole_ca[0],			/*086A*/
	center_tole_au[0],			/*086A*/
	center_tole_jp_eu[0],		/*086A*/
	center_tole_jp_uk[0],		/*086A*/
	center_tole_us[1],			/*086B*/
	center_tole_ca[1],			/*086B*/
	center_tole_au[1],			/*086B*/
	center_tole_jp_eu[1],		/*086B*/
	center_tole_jp_uk[1],		/*086B*/
};



/********************************************************/
/*	内部関数プロトタイプ宣言							*/
/********************************************************/
TCAN_COMMON_CTRL_LOCAL void s_odo_proc(void);
TCAN_COMMON_CTRL_LOCAL void s_fuel_proc(void);
TCAN_COMMON_CTRL_LOCAL void s_speed_proc(void);
TCAN_COMMON_CTRL_LOCAL void s_vehicleInfo_proc(void);
TCAN_COMMON_CTRL_LOCAL void s_vehicleInfo_proc_86A(void);
TCAN_COMMON_CTRL_LOCAL void s_vehicleInfo_proc_86B(void);
TCAN_COMMON_CTRL_LOCAL void s_vehicleInfo_proc_Unknown(void);
TCAN_COMMON_CTRL_LOCAL B_8 is_can_enalbe_86A(void);
TCAN_COMMON_CTRL_LOCAL B_8 is_can_enalbe_86B(void);
TCAN_COMMON_CTRL_LOCAL void s_clear_vehilceinfo(void);
TCAN_COMMON_CTRL_LOCAL UI_32 s_get_receive_interval_average(const UI_32* data, UI_8 count);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId374_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId140_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId141_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId144_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId148_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId360_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId361_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId368_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId0D1_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId0D3_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId152_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId282_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId0D0_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId0D4_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId375_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId040_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId146_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId345_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId048_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId68C_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId241_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId34A_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId139_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId13C_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId13A_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId3AC_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId390_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId138_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId13B_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId652_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId6E2_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId328_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId041_state(const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId04B_state(const UI_8 len, const UI_8* data);

TCAN_COMMON_CTRL_LOCAL E_TCAN_RESULT s_tccc_GetValStateUI8(const T_CanInfo_UI8* data);
TCAN_COMMON_CTRL_LOCAL E_TCAN_RESULT s_tccc_GetValStateUI16(const T_CanInfo_UI16* data);

#ifdef _FOR_RCF
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId0B4_state(const UI_8* data);
#endif	/* _FOR_RCF */

TCAN_COMMON_CTRL_LOCAL void  add_vehicle_speed_value(UI_32 speed_value_can);
TCAN_COMMON_CTRL_LOCAL UI_32 get_vehicle_speed_average(void);
TCAN_COMMON_CTRL_LOCAL UI_32 calc_vehicle_speed_tolerance(UI_32 speed_value);
TCAN_COMMON_CTRL_LOCAL UI_32 calc_vehicle_speed_fraction(UI_32 speed_value);
TCAN_COMMON_CTRL_LOCAL UI_32 calc_vehicle_speed_target(UI_32 speed_value);
TCAN_COMMON_CTRL_LOCAL UI_32 calc_vehicle_speed_near0(UI_32 speed_value_trg, UI_32 speed_value_fra);

TCAN_COMMON_CTRL_LOCAL void s_brake_proc(void);
TCAN_COMMON_CTRL_LOCAL void add_vehicle_brake_press(UI_32 brake_press);
TCAN_COMMON_CTRL_LOCAL UI_32 get_vehicle_brake_press_average(void);
TCAN_COMMON_CTRL_LOCAL UI_32 calc_vehicle_brake_press_lpf(UI_32 brake_press_can, UI_32 brake_press_ave, UI_32 brake_press_smooth);

TCAN_COMMON_CTRL_LOCAL void s_setMessage_086A(UI_32 id, const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_setMessage_086B(UI_32 id, const UI_8 len, const UI_8* data);
TCAN_COMMON_CTRL_LOCAL void s_setMessage_Unknown(UI_32 id, const UI_8 len, const UI_8* data);

/** 
 * T-CANメイン初期化処理
 * 
 * author	NSCS
 * date		2013/12/--
 * note		車両種別による受信可能IDの設定、変数の初期化
 */
void TCAN_Ctrl_Init(void)
{
#ifdef TCAN_DEBUG
	OUTLOG(OutputLog( "[TCAN] TCAN_Ctrl_Init \n");)
#endif /*TCAN_DEBUG*/
	/* 変数初期化 */
	OdoTrip = 0;
	SpOdo = 0;
	Sp1p_old = 0;
	Sum_Fc = 0;
	Sum_Fc_N = 0;
	FuelProcCycleCnt = 0;
	FC_Sum = 0;
	
	S_can_0d1_received_time     = 0;
	S_vehicle_speed_value_count = 0;
	S_speed_value_mem           = 0;
	S_speed_value_dsp           = 0;
	S_speed_proc_data_time      = 0;
	S_speed_value_updated_time  = 0;
	S_speed_value_dsp_enable    = 0;
	S_OdoTrip_enable            = 0;
	S_FC_Sum_enable             = 0;
	S_brake_press_smooth 		= 0;
	S_brake_press_smooth_enable = 0;
	S_brake_press_proc_data_time= 0;
	S_vehicle_brake_press_count = 0;
	(void)memset(S_vehicle_brake_press_buf, 0, sizeof(S_vehicle_brake_press_buf));

	(void)memset(&MtDDoorSW, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtPDoorSW, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtTrunkDoorSW, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtRatioAccel, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtEngRpm, 0, sizeof(T_CanInfo_UI16));
	(void)memset(&MtEngRpm_Ta, 0, sizeof(T_CanInfo_UI16));
	(void)memset(&MtGearEstm, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtSpWheelAve, 0, sizeof(T_CanInfo_UI16));
	(void)memset(&MtSpPulseInteg, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtBreakLampSW, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtHandBreakSW, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtReverseSW, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtHeadLampHi, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtHeadLampLo, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtHeadLampSmall, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtOutTemp, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtPSeatBeltSW, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtDSeatBeltSW, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtTempOil, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtTempCoolant, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtSetCc, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtSteerAngle, 0, sizeof(T_CanInfo_UI16));	
	(void)memset(&MtYawRate, 0, sizeof(T_CanInfo_UI16));	
	(void)memset(&MtAccLR, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtAccFR, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtTrnsType, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtSportGearPos, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtShiftPos, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtModeShiftCtrl, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtAtType, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtPlace, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtIgn,              0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtAcc,              0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtBrandFlag,        0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtTargetGearPos,    0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtSportsLamp,       0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtSnowHoldLamp,     0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtNodeID,           0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtSleepAck,         0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtSleepInd,         0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtLimpHome,         0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtRing,             0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtAlive,            0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtWkup,             0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtInit,             0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtSpecNo,           0, sizeof(T_CanInfo_UI16));
	(void)memset(&MtSpdWheelFtlh,     0, sizeof(T_CanInfo_UI16));
	(void)memset(&MtSpdWheelFtrh,     0, sizeof(T_CanInfo_UI16));
	(void)memset(&MtSpdWheelRrlh,     0, sizeof(T_CanInfo_UI16));
	(void)memset(&MtSpdWheelRrrh,     0, sizeof(T_CanInfo_UI16));
	(void)memset(&MtDoorLockStatPass, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtDoorLockStatDrvr, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtBrakePressure,    0, sizeof(T_CanInfo_UI8));

	(void)memset(&AtVscOnOff,        0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtVscAct,          0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtTrcMode,         0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtTrcOnOff,        0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtTrcAct,          0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtAbsAct,          0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtVscWarning,      0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtAbsWarning,      0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtAbsFail,         0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtVscUnderStr,     0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtVscOverStr,      0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtEngTrqUpReqFlg,  0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtEngTrqDwnReqFlg, 0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtEngTrqUpReqVal,  0, sizeof(T_CanInfo_UI16));
	(void)memset(&AtEngTrqDwnReqVal, 0, sizeof(T_CanInfo_UI16));
	(void)memset(&AtEngTrqShaft,     0, sizeof(T_CanInfo_UI16));
	(void)memset(&AtEcbWarning		,     0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtIntakePress		,     0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtAtmosphericPress,     0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtBlinkerLeft		,     0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtBlinkerRight	,     0, sizeof(T_CanInfo_UI8));
	(void)memset(&MtHoodDoorSW		,     0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtManualModeSW,    0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtAirPressFtlh,    0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtAirPressFtrh,    0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtAirPressRrlh,    0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtAirPressRrrh,    0, sizeof(T_CanInfo_UI8));
	(void)memset(&AtAirPressUnit,    0, sizeof(T_CanInfo_UI8));

	S_vehicle_proc_time			= 	0;
	S_vehicleJudgeInfo 			=	TCAN_VEHICLE_INFO_UNKNOWN;
	S_vehicleCanInfo			=	TCAN_VEHICLE_INFO_UNKNOWN;
	S_speed_value_hold_time		=	0;
	S_speed_value_count_max		=	0;
	/*車種情報を判定するための情報クリア*/
	s_clear_vehilceinfo();

}

/** 
 * T-CANメイン処理
 * 
 * author	NSCS
 * date		2013/12/--
 * note		10ms周期でコールされる。主に積算処理を行う。
 */
void TCAN_Ctrl_Main(void)
{
	if(S_vehicleCanInfo != TCAN_VEHICLE_INFO_UNKNOWN)
	{
		/*CAN受信状態が不明以外*/
		s_odo_proc();
		s_fuel_proc();
		s_speed_proc();			/* 表示用車速計算処理	*/
		s_brake_proc();			/* ブレーキ圧なまし処理	*/
	}	
	s_vehicleInfo_proc();	/* 車両情報判定処理	*/
}


/** 
 * 受信データ設定処理
 * 
 * author	NSCS
 * param	id		[in]	受信CAN-ID
 * param	data	[in]	受信CANデータの先頭アドレス
 * date		2013/12/--
 * note		受信データの解析処理を行う。
 */
void TCAN_Ctrl_SetMessage(UI_32 id, const UI_8 len, const UI_8* data)
{

	switch (S_vehicleCanInfo) {
	case TCAN_VEHICLE_INFO_86A:
		/*086A*/
		s_setMessage_086A(id, len, data);
		break;
	case TCAN_VEHICLE_INFO_86B:
		/*086B*/
		s_setMessage_086B(id, len, data);
		break;
	default:
		/*不明*/
		s_setMessage_Unknown(id, len, data);
		break;
	}
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_SetupCanMessage								*/
/* 機能の説明	：	車種情報に応じた受信メッセージの設定					*/
/* 引数			：	(I/ )vehicleInfo	車種情報							*/
/* 				：	(I/ )data	受信CANデータの先頭アドレス					*/
/* 戻り値		： 	void													*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	20/11/--												*/
/* 備考			：	車種情報に応じたCANIDを受信する設定を行う				*/
/****************************************************************************/
void TCAN_Ctrl_SetupCanMessage(E_TCAN_VEHICLE_INFO info)
{
#ifdef TCAN_DEBUG
	OUTLOG(OutputLog( "[TCAN] SetupCanMessage (%04x)\n", info );)
#endif /*TCAN_DEBUG*/

	/* 車種情報更:内部判定状態 */
	S_vehicleJudgeInfo = info;

	/* 車種情報更:CAN受信状態 */
	S_vehicleCanInfo = info;

	switch (S_vehicleCanInfo) {
	case TCAN_VEHICLE_INFO_86A:
		/*086A*/
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  0, MT_MSG_ID_0D0);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  1, MT_MSG_ID_140);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  2, MT_MSG_ID_141);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  3, MT_MSG_ID_360);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  4, MT_MSG_ID_361);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  5, MT_MSG_ID_0D1);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  6, MT_MSG_ID_0D3);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  7, MT_MSG_ID_0D4);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  8, MT_MSG_ID_152);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  9, MT_MSG_ID_282);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 10, MT_MSG_ID_144);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 11, MT_MSG_ID_148);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 12, MT_MSG_ID_368);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 13, MT_MSG_ID_374);
		/*086B用のCANIDも設定する*/
		/*086Bブランドフラグ*/
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 14, MT_MSG_ID_68C);

		/*車速保持時間設定*/
		S_speed_value_hold_time		=	VEHICLE_SPEED_HOLD_TIME_86A;
		/*車速保持数設定*/
		S_speed_value_count_max		=	VEHICLE_SPEED_VALUE_COUNT_MAX_86A;
		break;
	case TCAN_VEHICLE_INFO_86B:
		/*086B*/
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  0, MT_MSG_ID_040);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  1, MT_MSG_ID_048);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  2, MT_MSG_ID_139);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  3, MT_MSG_ID_13A);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  4, MT_MSG_ID_13B);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  5, MT_MSG_ID_13C);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  6, MT_MSG_ID_138);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  7, MT_MSG_ID_146);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  8, MT_MSG_ID_241);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  9, MT_MSG_ID_345);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 10, MT_MSG_ID_390);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 11, MT_MSG_ID_3AC);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 12, MT_MSG_ID_34A);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 13, MT_MSG_ID_68C);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 14, MT_MSG_ID_6E2);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 15, MT_MSG_ID_328);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 16, MT_MSG_ID_041);
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 17, MT_MSG_ID_04B);
		/*086A用のCANIDも設定する*/
		/*086Aブランドフラグ*/
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 18, MT_MSG_ID_282);

		/*車速保持時間設定*/
		S_speed_value_hold_time		=	VEHICLE_SPEED_HOLD_TIME_86B;
		/*車速保持数設定*/
		S_speed_value_count_max		=	VEHICLE_SPEED_VALUE_COUNT_MAX_86B;
		break;
	default:
		/*不明*/
		/*086Aブランドフラグ*/
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  0, MT_MSG_ID_282);
		/*086Bブランドフラグ*/
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  1, MT_MSG_ID_68C);

		/*車速保持時間設定*/
		S_speed_value_hold_time		=	0;
		/*車速保持数設定*/
		S_speed_value_count_max		=	0;
		break;
	}

}

/**
 * MT ドアSW D席情報取得
 * 
 * param	data		[out]		ドアSW D席(0:閉、1:空)
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2013/12/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTDoorOpen(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	if (MtDDoorSW.first_flg == 0) {
		result = TCAN_CTRL_NO_RECEIVE;
	} else if (MtDDoorSW.fail_flg == 1) {
		result = TCAN_CTRL_ERROR;
	} else {
		result = TCAN_CTRL_OK;
	}
	*data = MtDDoorSW.data;
	
	return result;
}

/**
 * MT ドアSW P席情報取得
 * 
 * param	data		[out]		ドアSW P席(0:閉、1:空)
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTPDoorOpen(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	if (MtPDoorSW.first_flg == 0) {
		result = TCAN_CTRL_NO_RECEIVE;
	} else if (MtPDoorSW.fail_flg == 1) {
		result = TCAN_CTRL_ERROR;
	} else {
		result = TCAN_CTRL_OK;
	}
	*data = MtPDoorSW.data;
	
	return result;
}

/**
 * MT ドアSW トランク情報取得
 * 
 * param	data		[out]		ドアSW トランク(0:閉、1:空)
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTrunkDoorOpen(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	if (MtTrunkDoorSW.first_flg == 0) {
		result = TCAN_CTRL_NO_RECEIVE;
	} else if (MtTrunkDoorSW.fail_flg == 1) {
		result = TCAN_CTRL_ERROR;
	} else {
		result = TCAN_CTRL_OK;
	}
	*data = MtTrunkDoorSW.data;
	
	return result;
}

/**
 * MT アクセル開度率情報取得
 * 
 * param	data		[out]		アクセル開度率(0～100/255)[%]
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2013/12/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTRatioAccel(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	if (MtRatioAccel.first_flg == 0) {
		result = TCAN_CTRL_NO_RECEIVE;
	} else if (MtRatioAccel.fail_flg == 1) {
		result = TCAN_CTRL_ERROR;
	} else {
		result = TCAN_CTRL_OK;
	}
	*data = MtRatioAccel.data;
	
	return result;
}

/**
 * MT エンジン回転数情報取得
 * 
 * param	data		[out]		エンジン回転数(0～16383)[rpm]
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2013/12/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTEngRpm(UI_16 *data)
{
	E_TCAN_RESULT	result;
	
	if (MtEngRpm.first_flg == 0) {
		result = TCAN_CTRL_NO_RECEIVE;
	} else if (MtEngRpm.fail_flg == 1) {
		result = TCAN_CTRL_ERROR;
	} else {
		result = TCAN_CTRL_OK;
	}
	*data = MtEngRpm.data;
	
	return result;
}

/**
 * MT エンジン回転数(タコメータ用)情報取得
 * 
 * param	data		[out]		エンジン回転数(タコメータ用)(0～16383)[rpm]
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2013/12/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTEngRpmTa(UI_16 *data)
{
	E_TCAN_RESULT	result;
	
	if (MtEngRpm_Ta.first_flg == 0) {
		result = TCAN_CTRL_NO_RECEIVE;
	} else if (MtEngRpm_Ta.fail_flg == 1) {
		result = TCAN_CTRL_ERROR;
	} else {
		result = TCAN_CTRL_OK;
	}
	*data = MtEngRpm_Ta.data;
	
	return result;
}

/**
 * MT ギアポジション表示報取得
 * 
 * param	data		[out]		ギアポジション表示
 * 									(0:AT or Uncertain, 1:1st, 2:2nd, 3:3rd, 4:4th, 5:5th, 6:6th, 7:NON)
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2013/12/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTGearEstm(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	if (MtGearEstm.first_flg == 0) {
		result = TCAN_CTRL_NO_RECEIVE;
	} else if (MtGearEstm.fail_flg == 1) {
		result = TCAN_CTRL_ERROR;
	} else {
		result = TCAN_CTRL_OK;
	}
	*data = MtGearEstm.data;
	
	return result;
}

/**
 * MT 駆動輪軸平均車輪速情報取得
 * 
 * param	data		[out]		駆動輪軸平均車輪速(0～300.0375)[0.05625km/h]
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2013/12/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSpWheelAve(UI_16 *data)
{
	E_TCAN_RESULT	result;
	
	if (MtSpWheelAve.first_flg == 0) {
		result = TCAN_CTRL_NO_RECEIVE;
	} else if (MtSpWheelAve.fail_flg == 1) {
		result = TCAN_CTRL_ERROR;
	} else {
		result = TCAN_CTRL_OK;
	}
	*data = MtSpWheelAve.data;
	
	return result;
}

/**
 * MT 車速パルス信号積算値情報取得
 * 
 * param	data		[out]		車速パルス信号積算値(0～63)
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2013/12/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSpPulseInteg(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	if (MtSpPulseInteg.first_flg == 0) {
		result = TCAN_CTRL_NO_RECEIVE;
	} else if (MtSpPulseInteg.fail_flg == 1) {
		result = TCAN_CTRL_ERROR;
	} else {
		result = TCAN_CTRL_OK;
	}
	*data = MtSpPulseInteg.data;
	
	return result;
}

/**
 * MT IGN情報取得
 * 
 * param	data		[out]		IGN情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)atsura
 * date		2016/8/24
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTIgn(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtIgn);
	*data = MtIgn.data;
	
	return result;
}

/**
 * MT ACC情報取得
 * 
 * param	data		[out]		ACC情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)atsura
 * date		2016/8/24
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTAcc(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtAcc);
	*data = MtAcc.data;
	
	return result;
}

/**
 * MT ブレーキランプ情報取得
 * 
 * param	data		[out]		ブレーキランプ情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTBreakLamp(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtBreakLampSW);
	*data = MtBreakLampSW.data;
	
	return result;
}

/**
 * MT パーキングブレーキ情報取得
 * 
 * param	data		[out]		パーキングブレーキ情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTHandBreak(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtHandBreakSW);
	*data = MtHandBreakSW.data;
	
	return result;
}

/**
 * MT リバース情報取得
 * 
 * param	data		[out]		リバース情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTReverse(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtReverseSW);
	*data = MtReverseSW.data;
	
	return result;
	
}

/**
 * MT ヘッドランプHi情報取得
 * 
 * param	data		[out]		ヘッドランプHi情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTHeadLampHi(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtHeadLampHi);
	*data = MtHeadLampHi.data;
	
	return result;
	
}

/**
 * MT ヘッドランプLo情報取得
 * 
 * param	data		[out]		ヘッドランプLo情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTHeadLampLo(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtHeadLampLo);
	*data = MtHeadLampLo.data;
	
	return result;
	
}

/**
 * MT ヘッドランプSmall情報取得
 * 
 * param	data		[out]		ヘッドランプSmall情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTHeadLampSmall(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtHeadLampSmall);
	*data = MtHeadLampSmall.data;
	
	return result;
}

/**
 * MT  表示用外気温情報取得
 * 
 * param	data		[out]		 表示用外気温情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTOutTemp(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtOutTemp);
	*data = MtOutTemp.data;
	
	return result;
}

/**
 * MT シートベルトバックル P席情報取得
 * 
 * param	data		[out]		シートベルトバックル P席情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTPSeatBelt(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtPSeatBeltSW);
	*data = MtPSeatBeltSW.data;
	
	return result;
}

/**
 * MT シートベルトバックル D席情報取得
 * 
 * param	data		[out]		シートベルトバックル D席情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTDSeatBelt(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtDSeatBeltSW);
	*data = MtDSeatBeltSW.data;
	
	return result;
}

/**
 * MT 仕向地情報取得
 * 
 * param	data		[out]		仕向地情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTPlace(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtPlace);
	*data = MtPlace.data;
	
	return result;
}

/**
 * MT ブランドフラグ情報取得
 * 
 * param	data		[out]		ブランドフラグ情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTBrandFlag(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtBrandFlag);
	*data = MtBrandFlag.data;
	
	return result;
}

/**
 * MT エンジン油温情報取得
 * 
 * param	data		[out]		エンジン油温情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTempOil(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtTempOil);
	*data = MtTempOil.data;
	
	return result;
}

/**
 * MT エンジン水温情報取得
 * 
 * param	data		[out]		エンジン水温情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTempCoolant(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtTempCoolant);
	*data = MtTempCoolant.data;
	
	return result;
}

/**
 * MT クルコンセットランプ情報取得
 * 
 * param	data		[out]		クルコンセットランプ情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSetCc(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtSetCc);
	*data = MtSetCc.data;
	
	return result;
}

/**
 * MT 舵角センサ情報取得
 * 
 * param	data		[out]		舵角センサ情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSteerAngle(UI_16 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI16(&MtSteerAngle);
	*data = MtSteerAngle.data;
	
	return result;
}

/**
 * MT ヨーレート情報取得
 * 
 * param	data		[out]		ヨーレート情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTYawRate(UI_16 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI16(&MtYawRate);
	*data = MtYawRate.data;
	
	return result;
}

/**
 * MT 横加速度情報取得
 * 
 * param	data		[out]		横加速度情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTAccLR(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtAccLR);
	*data = MtAccLR.data;
	
	return result;
}

/**
 * MT 前後Gセンサ情報取得
 * 
 * param	data		[out]		前後Gセンサ情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTAccFR(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtAccFR);
	*data = MtAccFR.data;
	
	return result;
}

/**
 * MT トランスミッション情報取得
 * 
 * param	data		[out]		トランスミッション情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2014/10/--
 * note
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTransmissionType(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&MtTrnsType);
	*data = MtTrnsType.data;

	return result;
}

/**
 * MT スポーツシフトギア位置表示情報取得
 *
 * param	data		[out]		スポーツシフトギア位置
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2014/10/--
 * note
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSportGearPosition(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&MtSportGearPos);
	*data = MtSportGearPos.data;

	return result;
}


/**
 * MT シフト位置情報(フェールセーフ処理後)取得
 *
 * param	data		[out]		シフト位置
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2014/10/--
 * note
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTShiftPosition(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&MtShiftPos);
	*data = MtShiftPos.data;

	return result;
}

/**
 * MT TCM変速制御モード情報取得
 *
 * param	data		[out]		TCM変速制御モード情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2014/10/--
 * note
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTModeShiftControl(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&MtModeShiftCtrl);
	*data = MtModeShiftCtrl.data;

	return result;
}

/**
 * MT 目標ギア位置情報取得
 * 
 * param	data		[out]		目標ギア位置情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	H.Watanabe
 * date		2014/5/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTargetGearPos(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtTargetGearPos);
	*data = MtTargetGearPos.data;
	
	return result;
}


/**																
 * MT AT種類取得												
 * 																
 * param	data		[out]		AT種類						
 * return	実行結果											
 * retval	TCAN_CTRL_OK			正常値						
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS												
 * date		2014/10/--											
 * note															
 */																
E_TCAN_RESULT TCAN_Ctrl_GetMTAtType(UI_8 *data)					
{																
	E_TCAN_RESULT	result;										
																
	result = s_tccc_GetValStateUI8(&MtAtType);					
	*data = MtAtType.data;										
																
	return result;												
}																

/**
 * MT スポーツランプ情報取得
 * 
 * param	data		[out]		スポーツランプ情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/8/26
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSportsLamp(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtSportsLamp);
	*data = MtSportsLamp.data;
	
	return result;
}

/**
 * MT スノーホールドランプ情報取得
 * 
 * param	data		[out]		スノーホールドランプ情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/8/26
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSnowHoldLamp(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtSnowHoldLamp);
	*data = MtSnowHoldLamp.data;
	
	return result;
}

/**
 * MT ノードID情報取得
 * 
 * param	data		[out]		ノードID情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/8/26
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTNodeID(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtNodeID);
	*data = MtNodeID.data;
	
	return result;
}

/**
 * MT Sleep Ack情報取得
 * 
 * param	data		[out]		Sleep Ack情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/8/26
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSleepAck(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtSleepAck);
	*data = MtSleepAck.data;
	
	return result;
}

/**
 * MT Sleep Ind情報取得
 * 
 * param	data		[out]		Sleep Ind情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/8/26
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSleepInd(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtSleepInd);
	*data = MtSleepInd.data;
	
	return result;
}

/**
 * MT Limp Home情報取得
 * 
 * param	data		[out]		Limp Home情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/8/26
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTLimpHome(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtLimpHome);
	*data = MtLimpHome.data;
	
	return result;
}

/**
 * MT Ring情報取得
 * 
 * param	data		[out]		Ring情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/8/26
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTRing(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtRing);
	*data = MtRing.data;
	
	return result;
}

/**
 * MT Alive情報取得
 * 
 * param	data		[out]		Alive情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/8/26
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTAlive(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtAlive);
	*data = MtAlive.data;
	
	return result;
}

/**
 * MT Wkup情報取得
 * 
 * param	data		[out]		Wkup情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/8/26
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTWkup(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtWkup);
	*data = MtWkup.data;
	
	return result;
}

/**
 * MT Init情報取得
 * 
 * param	data		[out]		Init情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/8/26
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTInit(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtInit);
	*data = MtInit.data;
	
	return result;
}

/**
 * MT Spec. No.情報取得
 * 
 * param	data		[out]		Spec. No.情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/8/26
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSpecNo(UI_16 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI16(&MtSpecNo);
	*data = MtSpecNo.data;
	
	return result;
}

/**
 * MT 左前輪車輪速情報取得
 * 
 * param	data		[out]		左前輪車輪速情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/9/05
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMtSpdWheelFtlh(UI_16 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI16(&MtSpdWheelFtlh);
	*data = MtSpdWheelFtlh.data;
	
	return result;
}

/**
 * MT 右前輪車輪速情報取得
 * 
 * param	data		[out]		右前輪車輪速情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/9/05
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMtSpdWheelFtrh(UI_16 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI16(&MtSpdWheelFtrh);
	*data = MtSpdWheelFtrh.data;
	
	return result;
}

/**
 * MT 左後輪車輪速情報取得
 * 
 * param	data		[out]		左後輪車輪速情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/9/05
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMtSpdWheelRrlh(UI_16 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI16(&MtSpdWheelRrlh);
	*data = MtSpdWheelRrlh.data;
	
	return result;
}

/**
 * MT 右後輪車輪速情報取得
 * 
 * param	data		[out]		右後輪車輪速情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/9/05
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMtSpdWheelRrrh(UI_16 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI16(&MtSpdWheelRrrh);
	*data = MtSpdWheelRrrh.data;
	
	return result;
}

/**
 * MT ドアロック状態（助手席）情報取得
 * 
 * param	data		[out]		ドアロック状態（助手席）情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/9/05
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMtDoorLockStatPass(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtDoorLockStatPass);
	*data = MtDoorLockStatPass.data;
	
	return result;
}

/**
 * MT ドアロック状態（運転席）情報取得
 * 
 * param	data		[out]		ドアロック状態（運転席）情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/9/05
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMtDoorLockStatDrvr(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtDoorLockStatDrvr);
	*data = MtDoorLockStatDrvr.data;
	
	return result;
}

/**
 * MT ブレーキ圧情情報取得
 * 
 * param	data		[out]		ブレーキ圧情情報
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	(OFT)Katsura
 * date		2016/9/05
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetMtBrakePressure(UI_8 *data)
{  		
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&MtBrakePressure);
	*data = MtBrakePressure.data;
	
	return result;
}

/**
 * 始動後走行距離取得 (HV/MT共通)
 * 
 * param	data		[out]		始動後走行距離[0.01m]
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2013/12/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetOdo(UI_32 *data)
{
	if(S_OdoTrip_enable != 1) {
		/* 車速値が有効でなければCAN未受信 */
		return TCAN_CTRL_NO_RECEIVE;
	}

	*data = OdoTrip;
	return TCAN_CTRL_OK;
}

/**
 * 燃料噴射量積算値 (HV/MT共通)
 * 
 * param	data		[out]		燃料噴射量積算値[0.001ml]
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2013/12/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetFCSum(UI_32 *data)
{
	if(S_FC_Sum_enable != 1) {
		/* 燃料噴射量積算値が有効でなければCAN未受信 */
		return TCAN_CTRL_NO_RECEIVE;
	}

	/* [MT] 単位換算なし */
	*data = FC_Sum;
	return TCAN_CTRL_OK;
}

/**
 * 表示用車速取得
 * 
 * param	data		[out]		表示用車速取得[0.00001km/h]
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2016/09/08
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetVehicleSpeedForDisp(UI_32 *data)
{
	if(S_speed_value_dsp_enable != 1) {
		/* 車速値が有効でなければCAN未受信 */
		return TCAN_CTRL_NO_RECEIVE;
	}

	*data = S_speed_value_dsp;
	return TCAN_CTRL_OK;
}

/**
 * 車速取得
 * 
 * param	data		[out]		車速取得[0.00001km/h]
 * return	実行結果
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2016/10/07
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetVehicleSpeed(UI_32 *data)
{
	E_TCAN_RESULT result;
	UI_16         vso_value;
	
	result = TCAN_Ctrl_GetMTSpWheelAve(&vso_value);
	if(result != TCAN_CTRL_OK) {
		return TCAN_CTRL_ERROR;
	}

	*data = (UI_32)((F_64)vso_value * VSO_RESOLUTION);
	return TCAN_CTRL_OK;
}

/**
 * ブレーキ圧（なまし値）取得
 * 
 * param	data		[out]		
 * return	実行結果				ブレーキ圧（なまし値）取得[0.1305882MPa]
 * retval	TCAN_CTRL_OK			正常値
 * retval	TCAN_CTRL_ERROR			異常値
 * retval	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし
 * author	NSCS
 * date		2016/09/08
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetSmoothingBrakePressure(UI_8 *data)
{
	if(S_brake_press_smooth_enable != 1) {
		/* ブレーキ圧値が有効でなければCAN未受信 */
		return TCAN_CTRL_NO_RECEIVE;
	}

	/*分解能演算前の値を返す*/
	*data = (UI_8)((F_64)S_brake_press_smooth / BRAKE_PRESS_RESOLUTION);
	return TCAN_CTRL_OK;
}


/** 
 * 受信データ設定処理(086A用)
 * 
 * author	OFT
 * param	id		[in]	受信CAN-ID
 * param	data	[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note		受信データの解析処理を行う。
 */
TCAN_COMMON_CTRL_LOCAL void s_setMessage_086A(UI_32 id, const UI_8 len, const UI_8* data)
{

#ifdef TCAN_DEBUG
	OUTLOG(OutputLog( "[TCAN] s_setMessage_086A %04x\n", id );)
#endif /*TCAN_DEBUG*/

	switch (id) {
	case MT_MSG_ID_374:			/* ドアSW */
		s_tccc_jdg_MT_MsgId374_state(len, data);
		break;
		
	case MT_MSG_ID_140:			/* アクセル開度,エンジン回転数 */
		s_tccc_jdg_MT_MsgId140_state(len, data);
		break;
		
	case MT_MSG_ID_141:			/* エンジン回転数(TA) */
		s_tccc_jdg_MT_MsgId141_state(len, data);
		break;
		
	case MT_MSG_ID_144:			/* トランスミッション情報 */
		s_tccc_jdg_MT_MsgId144_state(len, data);
		break;

	case MT_MSG_ID_148:			/* スポーツシフトギア位置等 */
		s_tccc_jdg_MT_MsgId148_state(len, data);
		break;

	case MT_MSG_ID_360:			/* 燃料消費量 */
		s_tccc_jdg_MT_MsgId360_state(len, data);
		break;
		
	case MT_MSG_ID_361:			/* ギアポジション */
		s_tccc_jdg_MT_MsgId361_state(len, data);
		break;
		
	case MT_MSG_ID_368:			/* AT種類 */
		s_tccc_jdg_MT_MsgId368_state(len, data);
		break;

	case MT_MSG_ID_0D1:			/* 駆動輪軸平均車輪速 */
		s_tccc_jdg_MT_MsgId0D1_state(len, data);
		break;
		
	case MT_MSG_ID_0D3:			/* 車速パルス信号積算値 */
		s_tccc_jdg_MT_MsgId0D3_state(len, data);
		break;
		
	case MT_MSG_ID_0D4:			/* ４輪車輪速 */
		s_tccc_jdg_MT_MsgId0D4_state(len, data);
		break;
		
	case MT_MSG_ID_152:			/* ブレーキ/ランプ */
		s_tccc_jdg_MT_MsgId152_state(len, data);
		break;
		
	case MT_MSG_ID_282:			/* バックルSW */
		s_tccc_jdg_MT_MsgId282_state(len, data);
		break;
		
	case MT_MSG_ID_0D0:			/* 舵角センサ等 */
		s_tccc_jdg_MT_MsgId0D0_state(len, data);
		break;
		
	case MT_MSG_ID_375:			/* ドアロック状態	*/
		s_tccc_jdg_MT_MsgId375_state(len, data);
		break;
		
	case MT_MSG_ID_68C:			/*086B:ブランドフラグ */
		s_tccc_jdg_MT_MsgId68C_state(len, data);
		break;

	default:
		break;
	}
}

/** 
 * 受信データ設定処理(086B用)
 * 
 * author	OFT
 * param	id		[in]	受信CAN-ID
 * param	data	[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note		受信データの解析処理を行う。
 */
TCAN_COMMON_CTRL_LOCAL void s_setMessage_086B(UI_32 id, const UI_8 len, const UI_8* data)
{

#ifdef TCAN_DEBUG
	OUTLOG(OutputLog( "[TCAN] s_setMessage_086B %04x\n", id );)
#endif /*TCAN_DEBUG*/

	switch (id) {
		
	case MT_MSG_ID_040:			/* アクセル開度,エンジン回転数 */
		s_tccc_jdg_MT_MsgId040_state(len, data);
		break;
		
	case MT_MSG_ID_146:			/* エンジン回転数(TA) */
		s_tccc_jdg_MT_MsgId146_state(len, data);
		break;
		
	case MT_MSG_ID_345:			/* トランスミッション情報 */
		s_tccc_jdg_MT_MsgId345_state(len, data);
		break;

	case MT_MSG_ID_048:			/* スポーツシフトギア位置等 */
		s_tccc_jdg_MT_MsgId048_state(len, data);
		break;

	case MT_MSG_ID_68C:			/* ブランドフラグ */
		s_tccc_jdg_MT_MsgId68C_state(len, data);
		break;
		
	case MT_MSG_ID_241:			/* ギアポジション */
		s_tccc_jdg_MT_MsgId241_state(len, data);
		break;
		
	case MT_MSG_ID_34A:			/* スポーツ、スノーランプ */
		s_tccc_jdg_MT_MsgId34A_state(len, data);
		break;

	case MT_MSG_ID_139:			/* 駆動輪軸平均車輪速 */
		s_tccc_jdg_MT_MsgId139_state(len, data);
		break;
		
	case MT_MSG_ID_13C:			/* 車速パルス信号積算値 */
		s_tccc_jdg_MT_MsgId13C_state(len, data);
		break;
		
	case MT_MSG_ID_13A:			/* ４輪車輪速 */
		s_tccc_jdg_MT_MsgId13A_state(len, data);
		break;
		
	case MT_MSG_ID_3AC:			/* ブレーキ/ランプ */
		s_tccc_jdg_MT_MsgId3AC_state(len, data);
		break;
		
	case MT_MSG_ID_390:			/* バックルSW */
		s_tccc_jdg_MT_MsgId390_state(len, data);
		break;
		
	case MT_MSG_ID_138:			/* 舵角センサ等 */
		s_tccc_jdg_MT_MsgId138_state(len, data);
		break;
		
	case MT_MSG_ID_13B:			/* 加速度	*/
		s_tccc_jdg_MT_MsgId13B_state(len, data);
		break;
		
	case MT_MSG_ID_652:			/* ドアロック状態	*/
		s_tccc_jdg_MT_MsgId652_state(len, data);
		break;

	case MT_MSG_ID_6E2:			/* タイヤ空気圧	*/
		s_tccc_jdg_MT_MsgId6E2_state(len, data);
		break;

	case MT_MSG_ID_328:			/* VSC信号	*/
		s_tccc_jdg_MT_MsgId328_state(len, data);
		break;

	case MT_MSG_ID_041:			/* 実エンジン軸トルク	*/
		s_tccc_jdg_MT_MsgId041_state(len, data);
		break;

	case MT_MSG_ID_04B:			/*目標ギア段数*/
		s_tccc_jdg_MT_MsgId04B_state(len, data);
		break;
	case MT_MSG_ID_282:			/* 086A:ブランドフラグ */
		s_tccc_jdg_MT_MsgId282_state(len, data);
		break;

	default:
		break;
	}
}
/** 
 * 受信データ設定処理(判定用)
 * 
 * author	OFT
 * param	id		[in]	受信CAN-ID
 * param	data	[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note		受信データの解析処理を行う。
 */
TCAN_COMMON_CTRL_LOCAL void s_setMessage_Unknown(UI_32 id, const UI_8 len, const UI_8* data)
{

	switch (id) {

	case MT_MSG_ID_282:			/* 086A:ブランドフラグ */
		s_tccc_jdg_MT_MsgId282_state(len, data);
		break;

	case MT_MSG_ID_68C:			/*086B:ブランドフラグ */
		s_tccc_jdg_MT_MsgId68C_state(len, data);
		break;
	default:
		break;
	}

}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetVehicleInfo								*/
/* 機能の説明	：	車種情報取得											*/
/* 引数			：	( /)													*/
/* 戻り値		：	E_VEHICLE_TYPE											*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	20/09/-													*/
/* 備考			：	内部判定した状態を返す									*/
/****************************************************************************/
E_TCAN_VEHICLE_INFO TCAN_Ctrl_GetVehicleInfo(void)
{
	/*内部判定状態を返す*/
	return S_vehicleJudgeInfo;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtAirPressFtlh								*/
/* 機能の説明	：	左前輪空気圧情報取得									*/
/* 引数			：	( /O)data	左前輪空気圧情報							*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	19/07/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressFtlh(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&AtAirPressFtlh);
	*data = AtAirPressFtlh.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtAirPressFtrh								*/
/* 機能の説明	：	右前輪空気圧情報取得									*/
/* 引数			：	( /O)data	右前輪空気圧情報							*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	19/07/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressFtrh(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&AtAirPressFtrh);
	*data = AtAirPressFtrh.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtAirPressRrlh								*/
/* 機能の説明	：	左後輪空気圧情報取得									*/
/* 引数			：	( /O)data	左後輪空気圧情報							*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	19/07/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressRrlh(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&AtAirPressRrlh);
	*data = AtAirPressRrlh.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtAirPressRrrh								*/
/* 機能の説明	：	右後輪空気圧情報取得									*/
/* 引数			：	( /O)data	右後輪空気圧情報							*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	19/07/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressRrrh(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&AtAirPressRrrh);
	*data = AtAirPressRrrh.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtAirPressUnit								*/
/* 機能の説明	：	空気圧単位情報取得										*/
/* 引数			：	( /O)data	空気圧単位情報								*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	19/07/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressUnit(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&AtAirPressUnit);
	*data = AtAirPressUnit.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtVscOnOff									*/
/* 機能の説明	：	VSC ON/OFF情報取得										*/
/* 引数			：	( /O)data	VSC ON/OFF情報								*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/02/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscOnOff(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtVscOnOff);
	*data = AtVscOnOff.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtVscAct									*/
/* 機能の説明	：	VSC/TCS作動状態情報取得									*/
/* 引数			：	( /O)data	VSC/TCS作動状態情報							*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/02/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscAct(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtVscAct);
	*data = AtVscAct.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtTrcMode									*/
/* 機能の説明	：	トラクション重視モード情報取得							*/
/* 引数			：	( /O)data	トラクション重視モード情報					*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/02/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtTrcMode(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtTrcMode);
	*data = AtTrcMode.data;

	return result;
}


/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtIntakePress								*/
/* 機能の説明	：	ECBウォーニング情報取得									*/
/* 引数			：	( /O)data	ECBウォーニング情報							*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtEcbWarning(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtEcbWarning);
	*data = AtEcbWarning.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtIntakePress								*/
/* 機能の説明	：	吸入管圧力情報取得										*/
/* 引数			：	( /O)data	吸入管圧力情報								*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtIntakePress(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtIntakePress);
	*data = AtIntakePress.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtAtmosphericPress							*/
/* 機能の説明	：	大気圧情報取得											*/
/* 引数			：	( /O)data	大気圧情報									*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAtmosphericPress(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtAtmosphericPress);
	*data = AtAtmosphericPress.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtBlinkerLeft								*/
/* 機能の説明	：	ウィンカー左情報取得									*/
/* 引数			：	( /O)data	ウィンカー左情報							*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtBlinkerLeft(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtBlinkerLeft);
	*data = AtBlinkerLeft.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtBlinkerRight								*/
/* 機能の説明	：	ウィンカー右情報取得									*/
/* 引数			：	( /O)data	ウィンカー右情報							*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtBlinkerRight(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtBlinkerRight);
	*data = AtBlinkerRight.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetMtHoodDoorSW								*/
/* 機能の説明	：	ドアSW フード情報取得									*/
/* 引数			：	( /O)data	ドアSW フード情報							*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetMtHoodDoorSW(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&MtHoodDoorSW);
	*data = MtHoodDoorSW.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtAbsAct									*/
/* 機能の説明	：	ABS作動状態情報取得										*/
/* 引数			：	( /O)data	ABS作動状態情報								*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/02/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAbsAct(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtAbsAct);
	*data = AtAbsAct.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtVscWarning								*/
/* 機能の説明	：	VSCウォーニング情報取得									*/
/* 引数			：	( /O)data	VSCウォーニング情報							*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscWarning(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtVscWarning);
	*data = AtVscWarning.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtAbsWarning								*/
/* 機能の説明	：	ABSウォーニング情報取得									*/
/* 引数			：	( /O)data	ABSウォーニング情報							*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAbsWarning(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtAbsWarning);
	*data = AtAbsWarning.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtAbsFail									*/
/* 機能の説明	：	ABSシステムフェイル情報取得								*/
/* 引数			：	( /O)data	ABSシステムフェイル							*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAbsFail(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtAbsFail);
	*data = AtAbsFail.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtVscUnderStr								*/
/* 機能の説明	：	VSCアンダー制御状態情報取得								*/
/* 引数			：	( /O)data	VSCアンダー制御状態情報						*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscUnderStr(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtVscUnderStr);
	*data = AtVscUnderStr.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtVscOverStr								*/
/* 機能の説明	：	VSCオーバー制御状態情報取得								*/
/* 引数			：	( /O)data	VSCオーバー制御状態情報						*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscOverStr(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtVscOverStr);
	*data = AtVscOverStr.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtEngTrqUpReqFlg							*/
/* 機能の説明	：	エンジントルクアップ要求フラグ情報取得					*/
/* 引数			：	( /O)data	エンジントルクアップ要求フラグ情報			*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqUpReqFlg(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtEngTrqUpReqFlg);
	*data = AtEngTrqUpReqFlg.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtEngTrqDwnReqFlg							*/
/* 機能の説明	：	エンジントルクダウン要求フラグ情報取得					*/
/* 引数			：	( /O)data	エンジントルクダウン要求フラグ情報			*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqDwnReqFlg(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtEngTrqDwnReqFlg);
	*data = AtEngTrqDwnReqFlg.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtEngTrqUpReqVal							*/
/* 機能の説明	：	エンジントルクアップ要求値情報取得						*/
/* 引数			：	( /O)data	エンジントルクアップ要求値情報				*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqUpReqVal(UI_16 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI16(&AtEngTrqUpReqVal);
	*data = AtEngTrqUpReqVal.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtEngTrqDwnReqVal							*/
/* 機能の説明	：	エンジントルクダウン要求値情報取得						*/
/* 引数			：	( /O)data	エンジントルクダウン要求値情報				*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqDwnReqVal(UI_16 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI16(&AtEngTrqDwnReqVal);
	*data = AtEngTrqDwnReqVal.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtEngTrqShaft								*/
/* 機能の説明	：	実エンジン軸トルク情報取得								*/
/* 引数			：	( /O)data	実エンジン軸トルク情報						*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqShaft(UI_16 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI16(&AtEngTrqShaft);
	*data = AtEngTrqShaft.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtTrcOnOff									*/
/* 機能の説明	：	TRC ON/OFF情報取得										*/
/* 引数			：	( /O)data	VSC ON/OFF情報								*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtTrcOnOff(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtTrcOnOff);
	*data = AtTrcOnOff.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtTrcAct									*/
/* 機能の説明	：	TRC作動状態情報取得										*/
/* 引数			：	( /O)data	TRC作動状態情報								*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/03/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtTrcAct(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtTrcAct);
	*data = AtTrcAct.data;

	return result;
}
/****************************************************************************/
/* 関数名称		：	TCAN_Ctrl_GetAtManualModeSW								*/
/* 機能の説明	：	マニュアルモードSW情報取得								*/
/* 引数			：	( /O)data	マニュアルモードSW情報						*/
/* 戻り値		：	TCAN_CTRL_OK			正常値							*/
/*				：	TCAN_CTRL_ERROR			異常値							*/
/*				：	TCAN_CTRL_NO_RECEIVE	起動後、一度も受信データなし	*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	21/01/-													*/
/* 備考			：															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtManualModeSW(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtManualModeSW);
	*data = AtManualModeSW.data;

	return result;
}

/****************************************************************************/
/* 関数名称		：	s_vehicleInfo_proc										*/
/* 機能の説明	：	車両情報判定処理										*/
/* 引数			：	( / )													*/
/* 戻り値		：	-														*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	20/11/-													*/
/* 備考			：															*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL void s_vehicleInfo_proc(void)
{
	/**	VEHICLEINFO_PROC_INTERVAL間隔で車両情報判を行う*/
	if (Timer_ctrl_TimeCompare1ms(S_vehicle_proc_time, VEHICLEINFO_PROC_INTERVAL) == N_OK)
	{
		/*	時間経過	*/
#ifdef TCAN_DEBUG
		E_TCAN_VEHICLE_INFO info = S_vehicleJudgeInfo;
		OUTLOG(OutputLog( "[TCAN] s_vehicleInfo_proc START !!! \n" );)
#endif /*TCAN_DEBUG*/

		switch (S_vehicleJudgeInfo) {
		case TCAN_VEHICLE_INFO_86A:
			/*086A*/
			s_vehicleInfo_proc_86A();
			break;
		case TCAN_VEHICLE_INFO_86B:
			/*086B*/
			s_vehicleInfo_proc_86B();
			break;
		default:
			/*不明*/
			s_vehicleInfo_proc_Unknown();
			break;
		}
	
#ifdef TCAN_DEBUG
		if (info != S_vehicleJudgeInfo ){
			/*変化あり*/
			OUTLOG(OutputLog( "[TCAN] !!!!!!!!!! VehileInfo Change (%04x -> (%04x) !!!!!!!!!!!\n", info,S_vehicleJudgeInfo );)
		}
#endif /*TCAN_DEBUG*/

		/*	処理時間保持	*/
		S_vehicle_proc_time = Timer_ctrl_GetTime1ms();
	}
	
	
}

/****************************************************************************/
/* 関数名称		：	s_vehicleInfo_proc_86A									*/
/* 機能の説明	：	車両情報判定処理（現在86A）								*/
/* 引数			：	( / )													*/
/* 戻り値		：	-														*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	20/11/-													*/
/* 備考			：															*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL void s_vehicleInfo_proc_86A(void)
{
	/*車種情報 86Aなので86Aから先にチェック*/
	if (is_can_enalbe_86A() == N_TRUE) {
		/*車種情報を判定するための情報クリア*/
		s_clear_vehilceinfo();
		/*車種情報は86A*/
		/*すでに86Aなのでこのまま抜ける*/
		return;
	}
	if (is_can_enalbe_86B() == N_TRUE) {
		/*車種情報を判定するための情報クリア*/
		s_clear_vehilceinfo();
		/*車種情報は86B*/
		S_vehicleJudgeInfo = TCAN_VEHICLE_INFO_86B;
	}
}


/****************************************************************************/
/* 関数名称		：	s_vehicleInfo_proc_86B									*/
/* 機能の説明	：	車両情報判定処理（現在86B）								*/
/* 引数			：	( / )													*/
/* 戻り値		：	-														*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	20/11/-													*/
/* 備考			：															*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL void s_vehicleInfo_proc_86B(void)
{
	/*車種情報 86Bなので86Bから先にチェック*/
	if (is_can_enalbe_86B() == N_TRUE) {
		/*車種情報を判定するための情報クリア*/
		s_clear_vehilceinfo();
		/*車種情報は86B*/
		/*すでに86Bなのでこのまま抜ける*/
		return;
	}

	if (is_can_enalbe_86A() == N_TRUE) {
		/*車種情報を判定するための情報クリア*/
		s_clear_vehilceinfo();
		/*車種情報は86A*/
		S_vehicleJudgeInfo = TCAN_VEHICLE_INFO_86A;
	}
}

/****************************************************************************/
/* 関数名称		：	s_vehicleInfo_proc_Unknown								*/
/* 機能の説明	：	車両情報判定処理（現在Unknown）							*/
/* 引数			：	( / )													*/
/* 戻り値		：	-														*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	20/11/-													*/
/* 備考			：	どちらの車種も確認する									*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL void s_vehicleInfo_proc_Unknown(void)
{
	/*車種情報 不明なので86Aと86B両方チェック*/
	/*同時に2つ同時に有効になることはない*/
	if (is_can_enalbe_86A() == N_TRUE) {
		/*車種情報を判定するための情報クリア*/
		s_clear_vehilceinfo();
		/*車種情報は86A*/
		S_vehicleJudgeInfo = TCAN_VEHICLE_INFO_86A;
		/*同時に2つ同時に有効になることはないがここでける*/
		return;
	}

	if (is_can_enalbe_86B() == N_TRUE) {
		/*車種情報を判定するための情報クリア*/
		s_clear_vehilceinfo();
		/*車種情報は86B*/
		S_vehicleJudgeInfo = TCAN_VEHICLE_INFO_86B;
	}
}


/****************************************************************************/
/* 関数名称		：	is_can_enalbe_86A										*/
/* 機能の説明	：	86A CAN信号有効判定										*/
/* 引数			：	( / )													*/
/* 戻り値		：	N_TRUE:有効/N_FALSE:無効								*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	20/11/-													*/
/* 備考			：															*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL B_8 is_can_enalbe_86A(void)
{
	B_8	ret = N_FALSE;
	UI_32 brad_ave;
	
	if (S_brandflag_receive_count_86A >= BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX) {
		/*86A用ブランドフラグは受信済*/

		/*86A用ブランドフラグの平均受信間隔取得*/
		brad_ave = s_get_receive_interval_average(S_brandflag_receive_interval_86A,S_brandflag_receive_count_86A);
		if ( brad_ave <= BRANDFLAG_RECEIVE_INTERVAL_86A ) {
			/*平均受信間隔が指定値以下となっている*/
			/*車種情報は86A*/
			ret = N_TRUE;
		}
	}
#ifdef TCAN_DEBUG
	OUTLOG(OutputLog( "[TCAN] 86A Check count (%d ) ave ( %d ) result %s \n", S_brandflag_receive_count_86A,brand_ave , (ret ? "true" : "false")); )
#endif /*TCAN_DEBUG*/
	
	return ret;
}


/****************************************************************************/
/* 関数名称		：	is_can_enalbe_86B										*/
/* 機能の説明	：	86B CAN信号有効判定										*/
/* 引数			：	( / )													*/
/* 戻り値		：	N_TRUE:有効/N_FALSE:無効								*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	20/11/-													*/
/* 備考			：															*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL B_8 is_can_enalbe_86B(void)
{
	B_8	ret = N_FALSE;
	UI_32 brad_ave;
	
	if (S_brandflag_receive_count_86B >= BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX) {
		/*86B用ブランドフラグは受信済*/

		/*86B用ブランドフラグの平均受信間隔取得*/
		brad_ave = s_get_receive_interval_average(S_brandflag_receive_interval_86B,S_brandflag_receive_count_86B);
		if ( brad_ave <= BRANDFLAG_RECEIVE_INTERVAL_86B ) {
			/*平均受信間隔が指定値以下となっている*/
			/*車種情報は86B*/
			ret = N_TRUE;
		}
	}
#ifdef TCAN_DEBUG
	OUTLOG(OutputLog( "[TCAN] 86B Check count (%d ) ave ( %d ) result %s \n", S_brandflag_receive_count_86B,brand_ave , (ret ? "true" : "false")); )
#endif /*TCAN_DEBUG*/
		
	return ret;
}


/****************************************************************************/
/* 関数名称		：	s_clear_vehilceinfo_proc_86B							*/
/* 機能の説明	：	車両情報クリア											*/
/* 引数			：	( / )													*/
/* 戻り値		：	-														*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	20/11/-													*/
/* 備考			：	車両情報を判定位するための情報をクリアする				*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL void s_clear_vehilceinfo(void)
{
	
	S_brandflag_receive_lasttime_86A	=	0;
	S_brandflag_receive_lasttime_86B	=	0;

	S_brandflag_receive_count_86A		=	0;
	S_brandflag_receive_count_86B		=	0;

	(void)memset(&S_brandflag_receive_interval_86A,    0, sizeof(S_brandflag_receive_interval_86A));
	(void)memset(&S_brandflag_receive_interval_86B,    0, sizeof(S_brandflag_receive_interval_86B));
		
}

/****************************************************************************/
/* 関数名称		：	get_receive_interval_average							*/
/* 機能の説明	：	受信間隔の平均値取得									*/
/* 引数			：	(I/ )data	データ受信間隔								*/
/* 引数			：	(I/ )count	データ数									*/
/* 戻り値		：	受信間隔の平均値取得									*/
/* 作成者		：	(OFT)Sakata												*/
/* 作成日		：	20/11/-													*/
/* 備考			：															*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL UI_32 s_get_receive_interval_average(const UI_32* data, UI_8 count)
{
	UI_8  i;
	UI_32 sum_value = 0;
	UI_32 ave_value;
	
	if(count == 0) {
		return(0);
	}

	/* バッファ内の全車速値を加算 */
	for(i = 0; i < count; i++) {
		sum_value += data[i];
	}
	
	/* 平均値算出 */
	ave_value = sum_value / count;
	
	return ave_value;
}


/** 
 * 車速積算処理
 * 
 * author	NSCS
 * date		2013/12/--
 * note		始動後走行距離の演算を行う。CAN信号の車速パルス信号積算値の差分を積算するため、CAN受信時処理ではなく定期処理を行って問題なし。
 *			SP1のフェール時は積算処理を行わない、SP1Pのフェール時は処理を継続しても問題なし。
 *
 */
TCAN_COMMON_CTRL_LOCAL void s_odo_proc(void)
{
	UI_16 sp1;					/* 車速[km/h] */
	UI_8 sp1_fail;				/* 車速異常 */
	UI_8 sp1_first;				/* 車速 受信フラグ */
	UI_8 sp1p;					/* 車速パルス信号積算値 */
	UI_8 sp1p_first;			/* 車速パルス信号積算値 受信フラグ */
	UI_16 sp_run_thresh;		/* 1.75[km/h]相当のCAN信号閾値 */
	
	sp1 = MtSpWheelAve.data;
	sp1_fail = MtSpWheelAve.fail_flg;
	sp1_first = MtSpWheelAve.first_flg;
	sp1p = MtSpPulseInteg.data;
	sp1p_first = MtSpPulseInteg.first_flg;
	sp_run_thresh = SP_RUN_TH_MT;

	if ((sp1 > sp_run_thresh) && (sp1_fail == 0))  {
		/* "車速:1.75km/h以上" かつ "車速異常なし" */
		
		/* 車速パルス総積算値 */
		if (sp1p >= Sp1p_old) {
			SpOdo = SpOdo + ((UI_32)sp1p - (UI_32)Sp1p_old);	/* オーバーフロー演算可能（SPODOが最大値（0xFFFFFFFF）を超える場合は、SPODOから最大値（0xFFFFFFFF）を減算する） */
		} else {
			SpOdo = SpOdo + (64 - Sp1p_old) + sp1p;		/* オーバーフロー演算可能（SPODOが最大値（0xFFFFFFFF）を超える場合は、SPODOから最大値（0xFFFFFFFF）を減算する） */
		}
		
		/* 始動後走行距離 */
		if (SpOdo <= SP_MAX_TH) {
			/* ODO_TRIP = SPODO × 10^5 / (2 × 637 × NP) */
			OdoTrip = (UI_32)((F_64)SpOdo * COEF);		/* 被乗数が32bitのため浮動小数点演算を使わざるを得ない、処理が遅くないか要確認■ */
		} else {
			OdoTrip = UI32_MAX;
		}
		/* OdoTrip値有効フラグオン */
		S_OdoTrip_enable = 1;
	}
	else if ((sp1 <= sp_run_thresh) && (sp1_fail == 0) && (sp1_first == 1) && (sp1p_first == 1))  {
		/*停止時で車速、車速パルス受信済み（初回起動時に車速、車速パルスが受信済みならOdoを初期値（0）で有効とさせるため）*/
		/* OdoTrip値有効フラグオン */
		S_OdoTrip_enable = 1;
	}
	else {
		/* nop */
	}
		
	Sp1p_old = sp1p;		/* 前回値保持 */
}

/** 
 * 燃焼噴射量積算処理
 * 
 * author	NSCS
 * date		2013/12/--
 * note		燃料噴射量積算値の演算を行う
 */
TCAN_COMMON_CTRL_LOCAL void s_fuel_proc(void)
{
	UI_32	dlt_fc;
	UI_32	sfc;

	/* [MT] 受信イベントにてSUM_FCを計算し、500ms周期でFCSUMを計算する(Nは有効受信数とする) */
	FuelProcCycleCnt++;
	if (FuelProcCycleCnt >= FUEL_CYCLE) {
		/* DLC_FC[ml/sec] = SUM_FC[×10 cc/sec] ÷ N */
		if (Sum_Fc_N != 0) {
			dlt_fc = Sum_Fc / Sum_Fc_N;
			
			/* SFC[ml] = DLT_FC[ml/sec] × T ÷ 1000 */
			sfc = dlt_fc / 2;
			
			/* FCSUM[ml] = FCSUM[ml] + SFC[ml] */
			FC_Sum += sfc;
			S_FC_Sum_enable = 1;
		}
		
		/* 次回計算用 */
		Sum_Fc = 0;
		Sum_Fc_N = 0;
		FuelProcCycleCnt = 0;
	}
}

/**
 * MT CAN ID(374) ドアSW D席情報の状態判定
 * 
 * author	NSCS
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId374_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/*ドアSW */
	MtDDoorSW.data = data[3] & 0x01;
	MtDDoorSW.first_flg = 1;
	MtDDoorSW.fail_flg = 0;
	
	/*ドアSW P席 */
	MtPDoorSW.data = (data[3] >> 1) & 0x01;
	MtPDoorSW.first_flg = 1;
	MtPDoorSW.fail_flg = 0;
	
	/*ドアSW トランク */
	MtTrunkDoorSW.data = (data[3] >> 5) & 0x01;
	MtTrunkDoorSW.first_flg = 1;
	MtTrunkDoorSW.fail_flg = 0;
}

/**
 * MT CAN ID(140) アクセル開度率、エンジン回転数の状態判定
 * 
 * author	NSCS
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId140_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* アクセル開度率 */
	MtRatioAccel.data = data[0];
	MtRatioAccel.first_flg = 1;
	MtRatioAccel.fail_flg = 0;
	
	/* エンジン回転数 */
	MtEngRpm.data = data[2] + ((UI_16)(data[3] & 0x3F) << 8);
	MtEngRpm.first_flg = 1;
	MtEngRpm.fail_flg = 0;
}

/**
 * MT CAN ID(141) エンジン回転数(タコ)の状態判定
 * 
 * author	NSCS
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId141_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* エンジン回転数(タコ) */
	MtEngRpm_Ta.data = data[4] + ((UI_16)(data[5] & 0x3F) << 8);
	MtEngRpm_Ta.first_flg = 1;
	MtEngRpm_Ta.fail_flg = 0;


}

/*															
 * MT CAN ID(144) トランスミッション情報の状態判定			
 * 															
 * author	NSCS											
 * param	buf		[in]	受信CANデータの先頭アドレス		
 * date		2014/10/--										
 * note														
 */															
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId144_state(const UI_8 len, const UI_8* data)	
{															

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* トランスミッション情報 */
	MtTrnsType.data = (data[6] >> 7);
	MtTrnsType.first_flg = 1;								
	MtTrnsType.fail_flg = 0;								
}															

/*															
 * MT CAN ID(148) スポーツシフトギア位置等の状態判定			
 * 															
 * author	NSCS											
 * param	buf		[in]	受信CANデータの先頭アドレス		
 * date		2014/10/--										
 * note														
 */															
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId148_state(const UI_8 len, const UI_8* data)	
{															

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* スポーツシフトギア位置 */
	MtSportGearPos.data = (data[0] >> 4);
	MtSportGearPos.first_flg = 1;							
	MtSportGearPos.fail_flg = 0;							
															
	/* シフト位置 */
	MtShiftPos.data = data[0] & 0x0F;						
	MtShiftPos.first_flg = 1;								
	MtShiftPos.fail_flg = 0;								
															
	/* TCM変速制御モード */
	MtModeShiftCtrl.data = (data[1] >> 4) & 0x07;			
	MtModeShiftCtrl.first_flg = 1;							
	MtModeShiftCtrl.fail_flg = 0;							

	/* 目標ギア段数 */
	MtTargetGearPos.data = (data[6] & 0x0f);
	MtTargetGearPos.first_flg = 1;
	MtTargetGearPos.fail_flg = 0;
}

/*
 * MT CAN ID(360) 燃費消費量の状態判定
 * 
 * author	NSCS
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId360_state(const UI_8 len, const UI_8* data)
{
	UI_16	fuel;
	
	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* 燃費消費量[0.001cc/sec] */
	fuel = data[0] + ((UI_16)data[1] << 8);
	
	/* SUM_FCの演算 */
	/* 受信イベントにてSUM_FCを計算し、500ms周期でFCSUMを計算する(Nは有効受信数とする) */
	Sum_Fc += fuel;
	Sum_Fc_N++;

	/* エンジン油温 */
	MtTempOil.data = data[2];	
	MtTempOil.first_flg = 1;
	MtTempOil.fail_flg  = 0;

	/* エンジン水温 */
	MtTempCoolant.data = data[3];
	MtTempCoolant.first_flg = 1;
	MtTempCoolant.fail_flg  = 0;
	
	if(MtTempCoolant.data == 0xFF)
	{
		MtTempCoolant.fail_flg = 1;
	}
	
	/* クルコンセットランプ状態 */
	MtSetCc.data = (data[5] >> 5) & 0x01;	
	MtSetCc.first_flg = 1;
	MtSetCc.fail_flg  = 0;
	
}

/*
 * MT CAN ID(361) MTギアポジションの状態判定
 * 
 * author	NSCS
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId361_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* MTギアポジション */
	MtGearEstm.data = data[0] & 0x07;
	MtGearEstm.first_flg = 1;
	MtGearEstm.fail_flg = 0;
	
}

/*															
 * MT CAN ID(368) AT種類の状態判定							
 * 															
 * author	NSCS											
 * param	buf		[in]	受信CANデータの先頭アドレス		
 * date		2014/10/--										
 * note														
 */															
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId368_state(const UI_8 len, const UI_8* data)	
{															

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* AT種類 */
	MtAtType.data = (data[0] >> 2) & 0x07;					
	MtAtType.first_flg = 1;									
	MtAtType.fail_flg = 0;									

	/* スポーツランプ */
	MtSportsLamp.data = ((data[1] >> 4) & 0x03);
	MtSportsLamp.first_flg = 1;
	MtSportsLamp.fail_flg = 0;

	/* スノーホールドランプ */
	MtSnowHoldLamp.data = ((data[2] >> 2) & 0x01);
	MtSnowHoldLamp.first_flg = 1;
	MtSnowHoldLamp.fail_flg = 0;
}

/*
 * MT CAN ID(0D1) 駆動輪軸平均車輪速の状態判定
 * 
 * author	NSCS
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId0D1_state(const UI_8 len, const UI_8* data)
{

	UI_32 time1ms;
	if(len < 4) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* 駆動輪軸平均車輪速 */
	MtSpWheelAve.data = data[0] + ((UI_16)data[1] << 8);
	MtSpWheelAve.first_flg = 1;
	
	if (MtSpWheelAve.data == 0xFFFF) {
		MtSpWheelAve.fail_flg = 1;
	} else {
		MtSpWheelAve.fail_flg = 0;
		
		if (MtSpWheelAve.data < WHEEL_AVE_MIN) {
			MtSpWheelAve.data = 0;
		}
		if (MtSpWheelAve.data > WHEEL_AVE_MAX) {
			MtSpWheelAve.data = WHEEL_AVE_MAX;
		}
	}

	/*時刻取得*/
	time1ms = Timer_ctrl_GetTime1ms();

	MtBrakePressure.data      = data[2];
	MtBrakePressure.first_flg = 1;
	MtBrakePressure.fail_flg  = 0;
	
	/* 車速(0d1)メッセージ最終受信時刻更新 */
	S_can_0d1_received_time = time1ms;
}

#ifdef _FOR_RCF
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId0B4_state(const UI_8* data)
{
	/* 駆動輪軸平均車輪速 */
	MtSpWheelAve.data = data[6] + ((UI_16)data[5] << 8);
	MtSpWheelAve.first_flg = 1;
	
	if (MtSpWheelAve.data == 0xFFFF) {
		MtSpWheelAve.fail_flg = 1;
	} else {
		MtSpWheelAve.fail_flg = 0;
		
		if (MtSpWheelAve.data < WHEEL_AVE_MIN) {
			MtSpWheelAve.data = 0;
		}
		if (MtSpWheelAve.data > WHEEL_AVE_MAX) {
			MtSpWheelAve.data = WHEEL_AVE_MAX;
		}
	}
}
#endif	/* _FOR_RCF */

/*
* MT CAN ID(0D3) 車速パルス信号積算値の状態判定
 * 
 * author	NSCS
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId0D3_state(const UI_8 len, const UI_8* data)
{

	if(len < 7) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* 車速パルス信号積算値 */
	MtSpPulseInteg.data = data[4] >> 2;
	MtSpPulseInteg.first_flg = 1;
	MtSpPulseInteg.fail_flg = 0;
}

/*
* MT CAN ID(152) ブレーキ/ランプの状態判定
 * 
 * author	H.Watanabe
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2013/12/--
 * note	
 */

TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId152_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	MtBreakLampSW.data = ( data[6] >> 4 ) & 0x01;	
	MtBreakLampSW.first_flg = 1;
	MtBreakLampSW.fail_flg = 0;
	
	MtHandBreakSW.data = ( data[6] >> 3 ) & 0x01;	
	MtHandBreakSW.first_flg = 1;
	MtHandBreakSW.fail_flg = 0;
	
	MtReverseSW.data =	(data[6] >> 2) & 0x01;
	MtReverseSW.first_flg = 1;
	MtReverseSW.fail_flg = 0;
	
	MtHeadLampHi.data = ( data[7] >> 4 ) & 0x01;	
	MtHeadLampHi.first_flg = 1;
	MtHeadLampHi.fail_flg = 0;
	
	MtHeadLampLo.data = ( data[7] >> 3 ) & 0x01;
	MtHeadLampLo.first_flg = 1;
	MtHeadLampLo.fail_flg = 0;
	
	MtHeadLampSmall.data = (data[7] >> 2) & 0x01;
	MtHeadLampSmall.first_flg = 1;
	MtHeadLampSmall.fail_flg = 0;

	MtIgn.data = (data[0] >> 6) & 0x01;
	MtIgn.first_flg = 1;
	MtIgn.fail_flg = 0;

	MtAcc.data = (data[0] >> 5) & 0x01;
	MtAcc.first_flg = 1;
	MtAcc.fail_flg = 0;
}

/*
* MT CAN ID(282) バックルSWの状態判定
 * 
 * author	H.Watanabe
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId282_state(const UI_8 len, const UI_8* data)
{
	UI_32 time1ms;

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* 表示用外気温 */
	MtOutTemp.data = data[3];	
	MtOutTemp.first_flg = 1;
	
	if(MtOutTemp.data >= 0xF1)
	{
		MtOutTemp.fail_flg = 1;
	}else{
		MtOutTemp.fail_flg = 0;
	}
	
	/* シートベルトバックルSW P席 */
	MtPSeatBeltSW.data = (data[5] >> 1) & 0x01;	
	MtPSeatBeltSW.first_flg = 1;
	MtPSeatBeltSW.fail_flg = 0;
	
	/* シートベルトバックルSW D席 */
	MtDSeatBeltSW.data = data[5]  & 0x01;
	MtDSeatBeltSW.first_flg = 1;
	MtDSeatBeltSW.fail_flg = 0;
	
	/* 仕向地情報 */
	MtPlace.data = (data[6] >> 4);
	MtPlace.first_flg = 1;
	MtPlace.fail_flg = 0;
	
	/* ブランドフラグ */
	MtBrandFlag.data = data[6] &0x01;
	MtBrandFlag.first_flg = 1;
	MtBrandFlag.fail_flg = 0;

	/*時刻取得*/
	time1ms = Timer_ctrl_GetTime1ms();
	if (S_brandflag_receive_lasttime_86A == 0){
		/* 前回時間未保持*/
		/* 受信間隔保持数０*/
		S_brandflag_receive_count_86A = 0;
	}else if(S_brandflag_receive_count_86A >= BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX) {
		/* 受信間隔保持数最大値以上*/
		/* データを１つ削除（前へ詰める）*/
		(void)memmove(&S_brandflag_receive_interval_86A[0], &S_brandflag_receive_interval_86A[1], sizeof(UI_32) * (BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX - 1));
		/* 前回からの経過時間を設定*/
		S_brandflag_receive_interval_86A[BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX - 1]	=	(time1ms - S_brandflag_receive_lasttime_86A);
	}else{
		/* 受信間隔保持数最大値未満*/
		/* 前回からの経過時間を設定*/
		S_brandflag_receive_interval_86A[S_brandflag_receive_count_86A]	=	(time1ms - S_brandflag_receive_lasttime_86A);
		/* 受信間隔保持数＋１*/
		S_brandflag_receive_count_86A++;
	}
	/* 前回時間更新*/
	S_brandflag_receive_lasttime_86A = time1ms;

}

/*
* MT CAN ID(0D0) 舵角センサの状態判定
 * 
 * author	H.Watanabe
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId0D0_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* 舵角センサ */
	MtSteerAngle.data = (UI_16)data[0] | ((UI_16)data[1] << 8);
	MtSteerAngle.first_flg = 1;
	MtSteerAngle.fail_flg = 0;
	
	if(MtSteerAngle.data == 0x8000)
	{
		MtSteerAngle.fail_flg = 1;
	}
	
	/* ヨーレート */
	MtYawRate.data = (UI_16)data[2] | ((UI_16)data[3] << 8);
	MtYawRate.first_flg = 1;
	MtYawRate.fail_flg = 0;

	/* 横加速度 */
	MtAccLR.data = data[6];
	MtAccLR.first_flg = 1;
	MtAccLR.fail_flg = 0;

	/* 前後Gセンサ */
	MtAccFR.data = 	data[7];
	MtAccFR.first_flg = 1;
	MtAccFR.fail_flg = 0;
	
	if(MtAccFR.data == 0x80)
	{
		MtAccFR.fail_flg = 1;
	}
}


/*
* MT CAN ID(0D4) ４輪車輪速メッセージの状態判定
 * 
 * author	(OFT)Katsura
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2016/08/26
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId0D4_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* 左前輪車輪速 */
	MtSpdWheelFtlh.data      = (UI_16)data[0] | ((UI_16)data[1] << 8);
	MtSpdWheelFtlh.first_flg = 1;
	MtSpdWheelFtlh.fail_flg  = 0;

	/* 右前輪車輪速 */
	MtSpdWheelFtrh.data      = (UI_16)data[2] | ((UI_16)data[3] << 8);
	MtSpdWheelFtrh.first_flg = 1;
	MtSpdWheelFtrh.fail_flg  = 0;

	/* 左後輪車輪速 */
	MtSpdWheelRrlh.data      = (UI_16)data[4] | ((UI_16)data[5] << 8);
	MtSpdWheelRrlh.first_flg = 1;
	MtSpdWheelRrlh.fail_flg  = 0;

	/* 右後輪車輪速 */
	MtSpdWheelRrrh.data      = (UI_16)data[6] | ((UI_16)data[7] << 8);
	MtSpdWheelRrrh.first_flg = 1;
	MtSpdWheelRrrh.fail_flg  = 0;
}

/*
* MT CAN ID(375) ドアロック状態メッセージの状態判定
 * 
 * author	(OFT)Katsura
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2016/08/26
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId375_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* ドアロック状態（助手席）*/
	MtDoorLockStatPass.data  = ((data[0] >> 1) & 0x01);
	MtDoorLockStatPass.first_flg = 1;
	MtDoorLockStatPass.fail_flg  = 0;

	/* ドアロック状態（運転席）*/
	MtDoorLockStatDrvr.data  = (data[0] & 0x01);
	MtDoorLockStatDrvr.first_flg = 1;
	MtDoorLockStatDrvr.fail_flg  = 0;
}


/**
 * MT CAN ID(040) アクセル開度率、エンジン回転数の状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId040_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* アクセル開度率 */
	MtRatioAccel.data = data[4];
	MtRatioAccel.first_flg = 1;
	MtRatioAccel.fail_flg = 0;
	
	/* エンジン回転数 */
	MtEngRpm.data = data[2] + ((UI_16)(data[3] & 0x3F) << 8);
	MtEngRpm.first_flg = 1;
	MtEngRpm.fail_flg = 0;
}

/**
 * MT CAN ID(146) エンジン回転数(タコ)の状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId146_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* エンジン回転数(タコ) */
	MtEngRpm_Ta.data = data[2] + ((UI_16)(data[3] & 0x3F) << 8);
	MtEngRpm_Ta.first_flg = 1;
	MtEngRpm_Ta.fail_flg = 0;


}

/*															
 * MT CAN ID(345) トランスミッション情報の状態判定			
 * 															
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス		
 * date		2020/09/--
 * note														
 */															
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId345_state(const UI_8 len, const UI_8* data)	
{															

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* 吸入管圧力 */
	AtIntakePress.data = data[5];
	AtIntakePress.first_flg = 1;
	AtIntakePress.fail_flg = 0;

	/* 大気圧 */
	AtAtmosphericPress.data = data[6];
	AtAtmosphericPress.fail_flg = 0;
	AtAtmosphericPress.first_flg = 1;

	/* トランスミッション情報 */
	MtTrnsType.data = (data[2] >> 7);
	MtTrnsType.first_flg = 1;
	MtTrnsType.fail_flg = 0;

	/* エンジン油温 */
	MtTempOil.data = data[3];	
	MtTempOil.first_flg = 1;
	MtTempOil.fail_flg  = 0;

	/* エンジン水温 */
	MtTempCoolant.data = data[4];
	MtTempCoolant.first_flg = 1;
	MtTempCoolant.fail_flg  = 0;
	if(MtTempCoolant.data == 0xFF)
	{
		MtTempCoolant.fail_flg = 1;
	}

}

/*                                                          
 * MT CAN ID(048) スポーツシフトギア位置等の状態判定        
 *                                                          
 * author	(OFT)Sakata
 * param    buf     [in]    受信CANデータの先頭アドレス     
 * date		2020/09/--
 * note                                                     
 */                                                         
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId048_state(const UI_8 len, const UI_8* data)  
{                                                           

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* スポーツシフトギア位置 */
	MtSportGearPos.data = (data[3] >> 3) & 0x0F;
	MtSportGearPos.first_flg = 1;
	MtSportGearPos.fail_flg = 0;

	/* シフト位置 */
	MtShiftPos.data = data[3] & 0x07;
	MtShiftPos.first_flg = 1;
	MtShiftPos.fail_flg = 0;

	/* マニュアルモードSW */
	AtManualModeSW.data = (data[4] >> 1) & 0x01;
	AtManualModeSW.first_flg = 1;
	AtManualModeSW.fail_flg = 0;

}

/*
 * MT CAN ID(68C) ブランドフラグの状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId68C_state(const UI_8 len, const UI_8* data)
{
	UI_32 time1ms;
	
	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* ブランドフラグ */
	MtBrandFlag.data = (data[1] >> 5) & 0x01;
	MtBrandFlag.first_flg = 1;
	MtBrandFlag.fail_flg = 0;

	/*時刻取得*/
	time1ms = Timer_ctrl_GetTime1ms();
	if (S_brandflag_receive_lasttime_86B == 0){
		/* 前回時間未保持*/
		/* 受信間隔保持数０*/
		S_brandflag_receive_count_86B = 0;
	}else if(S_brandflag_receive_count_86B >= BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX) {
		/* 受信間隔保持数最大値以上*/
		/* データを１つ削除（前へ詰める）*/
		(void)memmove(&S_brandflag_receive_interval_86B[0], &S_brandflag_receive_interval_86B[1], sizeof(UI_32) * (BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX - 1));
		/* 前回からの経過時間を設定*/
		S_brandflag_receive_interval_86B[BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX - 1]	=	(time1ms - S_brandflag_receive_lasttime_86B);
	}else{
		/* 受信間隔保持数最大値未満*/
		/* 前回からの経過時間を設定*/
		S_brandflag_receive_interval_86B[S_brandflag_receive_count_86B]	=	(time1ms - S_brandflag_receive_lasttime_86B);
		/* 受信間隔保持数＋１*/
		S_brandflag_receive_count_86B++;
	}
	/* 前回時間更新*/
	S_brandflag_receive_lasttime_86B = time1ms;

}

/*
 * MT CAN ID(241) MTギアポジションの状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId241_state(const UI_8 len, const UI_8* data)
{
	UI_16	fuel;

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* クルコンセットランプ状態 */
	MtSetCc.data = (data[6] >> 7);
	MtSetCc.first_flg = 1;
	MtSetCc.fail_flg = 0;
	/* MTギアポジション */
	MtGearEstm.data = (data[4] >> 3) & 0x07;
	MtGearEstm.first_flg = 1;
	MtGearEstm.fail_flg = 0;

	/* 燃費消費量[0.001cc/sec] */
	fuel = data[2] + ((UI_16)data[3] << 8);
	
	/* SUM_FCの演算 */
	/* 受信イベントにてSUM_FCを計算し、500ms周期でFCSUMを計算する(Nは有効受信数とする) */
	Sum_Fc += fuel;
	Sum_Fc_N++;
	
}

/*                                                          
 * MT CAN ID(34A) スポーツランプ,スノーランプの状態判定     
 *                                                          
 * author	(OFT)Sakata
 * param    buf     [in]    受信CANデータの先頭アドレス     
 * date		2020/09/--
 * note                                                     
 */                                                         
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId34A_state(const UI_8 len, const UI_8* data)	
{															

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* スポーツランプ */
	MtSportsLamp.data = ((data[7] >> 6) & 0x01);
	MtSportsLamp.first_flg = 1;
	MtSportsLamp.fail_flg = 0;

	/* スノーホールドランプ */
	MtSnowHoldLamp.data = (data[7] >> 7);
	MtSnowHoldLamp.first_flg = 1;
	MtSnowHoldLamp.fail_flg = 0;
}

/*
 * MT CAN ID(139) 駆動輪軸平均車輪速の状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId139_state(const UI_8 len, const UI_8* data)
{

	UI_32 time1ms;
	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* TRC作動状態 */
	AtTrcAct.data = (data[4] >> 5) & 0x01;
	AtTrcAct.first_flg = 1;
	AtTrcAct.fail_flg = 0;
	/* ABSシステムフェイルフラグ			*/
	AtAbsFail.data = (data[3] >> 7);
	AtAbsFail.first_flg = 1;
	AtAbsFail.fail_flg = 0;
	
	/* ABS作動状態 */
	AtAbsAct.data = (data[4] >> 4) & 0x01;
	AtAbsAct.first_flg = 1;
	AtAbsAct.fail_flg = 0;

	/* VSCアンダー制御状態 */
	AtVscUnderStr.data = (data[4] >> 7);
	AtVscUnderStr.first_flg = 1;
	AtVscUnderStr.fail_flg = 0;

	/* VSCオーバー制御状態 */
	AtVscOverStr.data = (data[4] >> 6) & 0x01;
	AtVscOverStr.first_flg = 1;
	AtVscOverStr.fail_flg = 0;

	/* 駆動輪軸平均車輪速 */
	MtSpWheelAve.data = data[2] + ((UI_16)(data[3] & 0x1F) << 8);
	MtSpWheelAve.first_flg = 1;
	
	if (MtSpWheelAve.data == 0x1FFF) {
		MtSpWheelAve.fail_flg = 1;
	} else {
		MtSpWheelAve.fail_flg = 0;
		
		if (MtSpWheelAve.data < WHEEL_AVE_MIN_86B) {
			MtSpWheelAve.data = 0;
		}
		if (MtSpWheelAve.data > WHEEL_AVE_MAX_86B) {
			MtSpWheelAve.data = WHEEL_AVE_MAX_86B;
		}
	}
	
	/*時刻取得*/
	time1ms = Timer_ctrl_GetTime1ms();
	
	/*ブレーキ圧*/
	MtBrakePressure.data      = data[5];
	MtBrakePressure.first_flg = 1;
	MtBrakePressure.fail_flg  = 0;
	
	/* 車速(0d1)メッセージ最終受信時刻更新 */
	S_can_0d1_received_time = time1ms;
}

/*
* MT CAN ID(13C) 車速パルス信号積算値の状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId13C_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* 車速パルス信号積算値 */
	MtSpPulseInteg.data = data[6] & 0x3F;
	MtSpPulseInteg.first_flg = 1;
	MtSpPulseInteg.fail_flg  = 0;
}

/*
* MT CAN ID(3AC) ブレーキ/ランプの状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note	
 */

TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId3AC_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}
	/* ブレーキランプ */
	MtBreakLampSW.data = ( data[6] >> 6 ) & 0x01;	
	MtBreakLampSW.first_flg = 1;
	MtBreakLampSW.fail_flg = 0;
	
	/* パーキングブレーキ */
	MtHandBreakSW.data = ( data[6] >> 5 ) & 0x01;	
	MtHandBreakSW.first_flg = 1;
	MtHandBreakSW.fail_flg = 0;
	
	/* MTリバース */
	MtReverseSW.data =	(data[6] >> 4) & 0x01;
	MtReverseSW.first_flg = 1;
	MtReverseSW.fail_flg = 0;
	
	/* ヘッドランプ：Hi */
	MtHeadLampHi.data = ( data[7] >> 2 ) & 0x01;	
	MtHeadLampHi.first_flg = 1;
	MtHeadLampHi.fail_flg = 0;
	
	/* ヘッドランプ：Lo */
	MtHeadLampLo.data = ( data[7] >> 1 ) & 0x01;
	MtHeadLampLo.first_flg = 1;
	MtHeadLampLo.fail_flg = 0;
	
	/* ヘッドランプ：スモール */
	MtHeadLampSmall.data = data[7] & 0x01;
	MtHeadLampSmall.first_flg = 1;
	MtHeadLampSmall.fail_flg = 0;

	/* IG */
	MtIgn.data = (data[3] >> 6) & 0x01;
	MtIgn.first_flg = 1;
	MtIgn.fail_flg = 0;

	/* ACC */
	MtAcc.data = (data[3] >> 5) & 0x01;
	MtAcc.first_flg = 1;
	MtAcc.fail_flg = 0;
	
	/*ドアSW D席*/
	MtDDoorSW.data = data[4] & 0x01;
	MtDDoorSW.first_flg = 1;
	MtDDoorSW.fail_flg = 0;
	
	/*ドアSW P席 */
	MtPDoorSW.data = (data[4] >> 1) & 0x01;
	MtPDoorSW.first_flg = 1;
	MtPDoorSW.fail_flg = 0;
	
	/*ドアSW トランク */
	MtTrunkDoorSW.data = (data[4] >> 5) & 0x01;
	MtTrunkDoorSW.first_flg = 1;
	MtTrunkDoorSW.fail_flg = 0;

	/*ドアSW フード */
	MtHoodDoorSW.data = (data[4] >> 6) & 0x01;
	MtHoodDoorSW.first_flg = 1;
	MtHoodDoorSW.fail_flg = 0;
	
}

/*
* MT CAN ID(390) バックルSWの状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId390_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* 表示用外気温 */
	MtOutTemp.data = data[4];	
	MtOutTemp.first_flg = 1;
	if(MtOutTemp.data >= 0xF1)
	{
		MtOutTemp.fail_flg = 1;
	}else{
		MtOutTemp.fail_flg = 0;
	}

	/* ウィンカー右							*/
	AtBlinkerRight.data = (data[6] >> 3) & 0x01;	
	AtBlinkerRight.first_flg = 1;
	AtBlinkerRight.fail_flg = 0;

	/* ウィンカー左							*/
	AtBlinkerLeft.data = (data[6] >> 2) & 0x01;	
	AtBlinkerLeft.first_flg = 1;
	AtBlinkerLeft.fail_flg = 0;
	
	/* シートベルトバックルSW P席 */
	MtPSeatBeltSW.data = (data[6] >> 1) & 0x01;	
	MtPSeatBeltSW.first_flg = 1;
	MtPSeatBeltSW.fail_flg = 0;
	
	/* シートベルトバックルSW D席 */
	MtDSeatBeltSW.data = data[6]  & 0x01;
	MtDSeatBeltSW.first_flg = 1;
	MtDSeatBeltSW.fail_flg = 0;
	
	/* 仕向地情報 */
	MtPlace.data = (data[2] >> 3) & 0x0F;
	MtPlace.first_flg = 1;
	MtPlace.fail_flg = 0;
	
}

/*
* MT CAN ID(138) 舵角センサの状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId138_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* 舵角センサ */
	MtSteerAngle.data = (UI_16)data[2] | ((UI_16)data[3] << 8);
	MtSteerAngle.first_flg = 1;
	MtSteerAngle.fail_flg = 0;
	if(MtSteerAngle.data == 0x8000)
	{
		MtSteerAngle.fail_flg = 1;
	}
	
	/* ヨーレート */
	MtYawRate.data = (UI_16)data[4] | ((UI_16)data[5] << 8);
	MtYawRate.first_flg = 1;
	MtYawRate.fail_flg = 0;

}

/*
* MT CAN ID(13B) リンクメッセージの状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId13B_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* エンジントルクアップ要求フラグ*/
	AtEngTrqUpReqFlg.data = (data[1] >> 6);
	AtEngTrqUpReqFlg.first_flg = 1;
	AtEngTrqUpReqFlg.fail_flg = 0;

	/* エンジントルク減少要求フラグ	*/
	AtEngTrqDwnReqFlg.data = (data[1] >> 5);
	AtEngTrqDwnReqFlg.first_flg = 1;
	AtEngTrqDwnReqFlg.fail_flg = 0;

	/* エンジントルクアップ要求値	*/
	AtEngTrqUpReqVal.data = (UI_16)data[2] | ((UI_16)data[3] << 8);
	AtEngTrqUpReqVal.first_flg = 1;
	AtEngTrqUpReqVal.fail_flg = 0;
	if(AtEngTrqUpReqVal.data == 0x0000)
	{
		AtEngTrqUpReqVal.fail_flg = 1;
	}

	/* エンジントルク減少要求値		*/
	AtEngTrqDwnReqVal.data = (UI_16)data[4] | ((UI_16)data[5] << 8);
	AtEngTrqDwnReqVal.first_flg = 1;
	AtEngTrqDwnReqVal.fail_flg = 0;
	if(AtEngTrqDwnReqVal.data == 0xFFFF)
	{
		AtEngTrqDwnReqVal.fail_flg = 1;
	}

	/* 横加速度 */
	MtAccLR.data = data[6];
	MtAccLR.first_flg = 1;
	MtAccLR.fail_flg = 0;

	/* 前後Gセンサ */
	MtAccFR.data = 	data[7];
	MtAccFR.first_flg = 1;
	MtAccFR.fail_flg = 0;
	
	if(MtAccFR.data == 0x80)
	{
		MtAccFR.fail_flg = 1;
	}

}

/*
* MT CAN ID(13A) ４輪車輪速メッセージの状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId13A_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* 左前輪車輪速:FL */
	MtSpdWheelFtlh.data      = (UI_16)(data[1] >> 4) | ((UI_16)data[2] << 4) | ((UI_16)(data[3] & 0x01) << 12);
	MtSpdWheelFtlh.first_flg = 1;
	MtSpdWheelFtlh.fail_flg  = 0;

	/* 右前輪車輪速:FR */
	MtSpdWheelFtrh.data      = (UI_16)(data[3] >> 1) | ((UI_16)(data[4] & 0x3F)<< 7);
	MtSpdWheelFtrh.first_flg = 1;
	MtSpdWheelFtrh.fail_flg  = 0;

	/* 左後輪車輪速:RL */
	MtSpdWheelRrlh.data      = (UI_16)(data[4] >> 6) | ((UI_16)data[5] << 2) | ((UI_16)(data[6] & 0x07) << 10);
	MtSpdWheelRrlh.first_flg = 1;
	MtSpdWheelRrlh.fail_flg  = 0;

	/* 右後輪車輪速:RR */
	MtSpdWheelRrrh.data      = (UI_16)(data[6] >> 3) | ((UI_16)data[7] << 5);
	MtSpdWheelRrrh.first_flg = 1;
	MtSpdWheelRrrh.fail_flg  = 0;
}


/*
* MT CAN ID(6E2) タイヤ空気圧状態メッセージの状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId6E2_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* 空気圧単位 */
	AtAirPressUnit.data = ((data[2] >> 2) & 0x03);
	AtAirPressUnit.first_flg = 1;
	AtAirPressUnit.fail_flg = 0;
	
	/* 左前輪空気圧 */
	AtAirPressFtlh.data = (data[3] >> 1);
	AtAirPressFtlh.first_flg = 1;
	AtAirPressFtlh.fail_flg = 0;

	/* 右前輪空気圧 */
	AtAirPressFtrh.data = (data[4] >> 1);
	AtAirPressFtrh.first_flg = 1;
	AtAirPressFtrh.fail_flg = 0;

	/* 左後輪空気圧 */
	AtAirPressRrlh.data = (data[5] >> 1);
	AtAirPressRrlh.first_flg = 1;
	AtAirPressRrlh.fail_flg = 0;

	/* 右後輪空気圧 */
	AtAirPressRrrh.data = (data[6] >> 1);
	AtAirPressRrrh.first_flg = 1;
	AtAirPressRrrh.fail_flg = 0;
	
}


/*
* MT CAN ID(04B) 目標ギア段数の状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2021/03/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId04B_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* 目標ギア段数 */
	MtTargetGearPos.data = (data[2] & 0x0f);
	MtTargetGearPos.first_flg = 1;
	MtTargetGearPos.fail_flg = 0;
	
}

/*
* MT CAN ID(041) 実エンジン軸トルク信号状態メッセージの状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2021/03/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId041_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* 実エンジン軸トルク */
	AtEngTrqShaft.data      = (UI_16)data[4] | ((UI_16)(data[5] & 0x7F) << 8);
	AtEngTrqShaft.first_flg = 1;
	AtEngTrqShaft.fail_flg  = 0;
	
}

/*
* MT CAN ID(328) VSC信号状態メッセージの状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2021/02/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId328_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}

	/* TRC ON/OFF */
	AtTrcOnOff.data = (data[5] >> 7);
	AtTrcOnOff.first_flg = 1;
	AtTrcOnOff.fail_flg = 0;
	/* VSCウォーニング*/
	AtVscWarning.data = (data[3] >> 2) & 0x01;
	AtVscWarning.first_flg = 1;
	AtVscWarning.fail_flg = 0;
	
	/* ABSウォーニング*/
	AtAbsWarning.data = (data[3] >> 1) & 0x01;
	AtAbsWarning.first_flg = 1;
	AtAbsWarning.fail_flg = 0;

	/* ECBウォーニング*/
	AtEcbWarning.data = (data[3] & 0x01);
	AtEcbWarning.first_flg = 1;
	AtEcbWarning.fail_flg = 0;

	/* VSC ON/OFF */
	AtVscOnOff.data = (data[4] & 0x01);
	AtVscOnOff.first_flg = 1;
	AtVscOnOff.fail_flg = 0;
	
	/* VSC 作動状態*/
	AtVscAct.data = ((data[4] >> 1) & 0x01);
	AtVscAct.first_flg = 1;
	AtVscAct.fail_flg = 0;
	
	/* トラクション重視モード*/
	AtTrcMode.data = ((data[3] >> 6) & 0x01);
	AtTrcMode.first_flg = 1;
	AtTrcMode.fail_flg = 0;
	
}



/*
* MT CAN ID(652) ドアロック状態メッセージの状態判定
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId652_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* データ長が規定サイズより小さければ読み捨て */
		return;
	}


	/* ドアロック状態（助手席）*/
	MtDoorLockStatPass.data  = ((data[2] >> 1) & 0x01);
	MtDoorLockStatPass.first_flg = 1;
	MtDoorLockStatPass.fail_flg  = 0;

	/* ドアロック状態（運転席）*/
	MtDoorLockStatDrvr.data  = (data[2] & 0x01);
	MtDoorLockStatDrvr.first_flg = 1;
	MtDoorLockStatDrvr.fail_flg  = 0;
	
	
}




/*
* 状態判定 (data:UI_8)
 * 
 * author	H.Watanabe
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL E_TCAN_RESULT s_tccc_GetValStateUI8(const T_CanInfo_UI8* data)
{
	E_TCAN_RESULT result;
	
	if (data->first_flg == 0) {
		result = TCAN_CTRL_NO_RECEIVE;
	} else if (data->fail_flg == 1) {
		result = TCAN_CTRL_ERROR;
	} else {
		result = TCAN_CTRL_OK;
	}
	return result;
}

/*
*  状態判定 (data:UI_16)
 * 
 * author	H.Watanabe
 * param	buf		[in]	受信CANデータの先頭アドレス
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL E_TCAN_RESULT s_tccc_GetValStateUI16(const T_CanInfo_UI16* data)
{
	E_TCAN_RESULT result;
	
	if (data->first_flg == 0) {
		result = TCAN_CTRL_NO_RECEIVE;
	} else if (data->fail_flg == 1) {
		result = TCAN_CTRL_ERROR;
	} else {
		result = TCAN_CTRL_OK;
	}
	return result;
}

/** 
 * ブレーキ圧なまし処理
 * 
 * author	(OFT)Sakata
 * date		2017/07/19
 * note		ブレーキ圧のなまし処理計算を行う
 */
TCAN_COMMON_CTRL_LOCAL void s_brake_proc(void)
{
	UI_32         cur_time;
	UI_8		  brake_press_raw;		/* ブレーキ圧 生値					*/
	UI_32		  brake_press_can;		/* ブレーキ圧 CAN信号値				*/
	UI_32		  brake_press_ave;		/* ブレーキ圧 表示保持時間内平均	*/
	UI_32		  brake_press_lpf;		/* ブレーキ圧 ローパスフィルター処理後の値	*/
	E_TCAN_RESULT result;
	
	/* 未受信時は処理しない */
	if(S_can_0d1_received_time == 0) {
		return;
	}
	
	/* 受信データが既に処理済みなら何もしない */
	if(S_brake_press_proc_data_time >= S_can_0d1_received_time) {
		return;
	}

	/* 現在時刻取得 */
	cur_time = Timer_ctrl_GetTime1ms();

	/* ブレーキ圧生値取得 */
	result = TCAN_Ctrl_GetMtBrakePressure(&brake_press_raw);
	if(result != TCAN_CTRL_OK) {
		brake_press_raw = 0;
	}

	/* 分解能演算 */
	brake_press_can = (UI_32)((F_64)brake_press_raw * BRAKE_PRESS_RESOLUTION);
	
	/* 平均値取得（前回まで） */
	brake_press_ave = get_vehicle_brake_press_average();

	/* 今回の車速をバッファに追加 */
	add_vehicle_brake_press(brake_press_can);
	
	/* ローパスフィルターにかけたブレーキ圧計算 */
	brake_press_lpf = calc_vehicle_brake_press_lpf(brake_press_can, brake_press_ave, S_brake_press_smooth);
	
	/*なまし処理ブレーキ圧更新*/
	S_brake_press_smooth = brake_press_lpf;
	/*なまし処理ブレーキ圧有効*/
	S_brake_press_smooth_enable = 1;

	/*処理時間更新*/
	S_brake_press_proc_data_time = cur_time;
	
}


/** 
 * ブレーキ圧値のバッファへの保存
 * 
 * author	(OFT)Sakata
 * date		2016/09/07
 * note		入力されたブレーキ圧値を平均値計算用バッファへ格納する
 */
TCAN_COMMON_CTRL_LOCAL void add_vehicle_brake_press(UI_32 brake_press)
{
	/* バッファが一杯なら１つづつ前に詰めて、最後尾を空ける */
	if(S_vehicle_brake_press_count >= VEHICLE_BRAKE_PRESS_COUNT_MAX) {
		/* データを前に移動 */
		(void)memmove(&S_vehicle_brake_press_buf[0], &S_vehicle_brake_press_buf[1], sizeof(UI_32) * (VEHICLE_BRAKE_PRESS_COUNT_MAX - 1));
		/* データ数デクリメント */
		S_vehicle_brake_press_count--;
	}
	/* バッファ後尾に格納 */
	S_vehicle_brake_press_buf[S_vehicle_brake_press_count] = brake_press;
	/* データ数インクリメント */
	S_vehicle_brake_press_count++;
}

/** 
 * 表示保持時間内での平均値ブレーキ圧取得
 * 
 * author	(OFT)Sakata
 * date		2017/07/19
 * note		平均値計算用バッファの平均ブレーキ圧値を取得する
 */
TCAN_COMMON_CTRL_LOCAL UI_32 get_vehicle_brake_press_average(void)
{
	UI_8  i;
	UI_32 sum_value = 0;
	UI_32 brake_press_avr;
	
	if(S_vehicle_brake_press_count == 0) {
		return(0);
	}

	/* バッファ内の全車速値を加算 */
	for(i = 0; i < S_vehicle_brake_press_count; i++) {
		sum_value += S_vehicle_brake_press_buf[i];
	}
	
	/* 平均値算出 */
	brake_press_avr = sum_value / S_vehicle_brake_press_count;
	
	return brake_press_avr;
}

/** 
 * ブレーキ圧をローパスフィルターにかける演算
 * 
 * author	(OFT)Sakata
 * date		2017/07/19
 * note		ブレーキ圧をローパスフィルターにかける演算処理
 */
TCAN_COMMON_CTRL_LOCAL UI_32 calc_vehicle_brake_press_lpf(UI_32 brake_press_can, UI_32 brake_press_ave, UI_32 brake_press_smooth)
{
	SI_32 brake_press_lpf;
	
	/*ブレーキ圧（フィルター後）＝ブレーキ圧平均値（前回値）＋（ブレーキ圧CAN値　- なましブレーキ圧）＊係数 */
	brake_press_lpf = (SI_32)((F_64)brake_press_ave + ((F_64)((SI_32)(brake_press_can - brake_press_smooth)) * BRAKE_PRESS_SMOOTHINGCOEFFICIENT));
	/*マイナス値になる場合は０にする*/
	if(brake_press_lpf < 0){
		brake_press_lpf = 0;
	}
	return (UI_32)brake_press_lpf;
}
/** 
 * 表示用車速計算処理
 * 
 * author	(OFT)Katsura
 * date		2016/09/07
 * note		表示用車速の計算を行う
 */
TCAN_COMMON_CTRL_LOCAL void s_speed_proc(void)
{
	UI_32         cur_time;
	UI_16         vso_value;
	UI_32		  speed_value_can;		/* 車速 CAN信号値			*/
	UI_32		  speed_value_ave;		/* 車速 表示保持時間内平均	*/
	UI_32		  speed_value_tol;		/* 車速 指示公差処理後の値	*/
	UI_32		  speed_value_fra;		/* 車速 端数処理後の値		*/
	UI_32		  speed_value_trg;		/* 車速 表示目標値			*/
	UI_32		  diff_value;
	UI_32		  diff_time;
	E_TCAN_RESULT result;
	
	/* 現在時刻取得 */
	cur_time = Timer_ctrl_GetTime1ms();

	/* ①フェール処理 */
	if((S_can_0d1_received_time == 0)
	|| ((cur_time - S_can_0d1_received_time) > TIMEOUT_0D1)) {
		/* 通信途絶の場合は、VSO=0 */
		vso_value = 0;
	}
	else {
		/* 受信データが既に処理済みなら何もしない */
		if(S_speed_proc_data_time >= S_can_0d1_received_time) {
			return;
		}

		/* VSO取得 */
		result = TCAN_Ctrl_GetMTSpWheelAve(&vso_value);
		if(result != TCAN_CTRL_OK) {
			vso_value = 0;
		}
	}
	/* 分解能を掛け合わせて車速取得 */
	speed_value_can = (UI_32)((F_64)vso_value * VSO_RESOLUTION);
	
	/* 有効閾値処理 :300km/h */
	if (speed_value_can > THRESHOLD_SPEED_MAX) {
		speed_value_can = THRESHOLD_SPEED_MAX;
	}

	/* 今回の車速をバッファに追加 */
	add_vehicle_speed_value(speed_value_can);

	/* ②平均処理 */
	speed_value_ave = get_vehicle_speed_average();

	/* ③公差処理 */
	speed_value_tol = calc_vehicle_speed_tolerance(speed_value_ave);

	/* ④端数処理(1) */
	speed_value_fra = calc_vehicle_speed_fraction(speed_value_tol);

	/* ⑤端数処理(2) */
	if(S_speed_value_mem > speed_value_fra) {
		diff_value = S_speed_value_mem - speed_value_fra;
	}
	else {
		diff_value = speed_value_fra - S_speed_value_mem;
	}
	if(diff_value > (1 * VEHICLE_SPEED_RESOLUTION)) {
		S_speed_value_mem = speed_value_fra;
	}

	/* ⑥表示目標値取得 */
	speed_value_trg = calc_vehicle_speed_target(S_speed_value_mem);
#ifdef TCAN_DEBUG
	S_speed_value_trg1 = speed_value_trg;
#endif /* 2021.01.29 S.Sakata */
	
	/* ⑦ゼロ近傍処理 */
	speed_value_trg = calc_vehicle_speed_near0(speed_value_trg, speed_value_fra);

	/* ⑧表示保持時間処理 */
	if(cur_time > S_speed_value_updated_time) {
		diff_time = cur_time - S_speed_value_updated_time;
	}
	else {
		diff_time = cur_time + (0xFFFFFFFF - S_speed_value_updated_time) + 1;
	}
	if(diff_time > S_speed_value_hold_time) {
		S_speed_value_dsp          = speed_value_trg;
		S_speed_value_updated_time = cur_time;
		S_speed_value_dsp_enable   = 1;
	}
	
	S_speed_proc_data_time = cur_time;
}

/** 
 * 車速値のバッファへの保存
 * 
 * author	(OFT)Katsura
 * date		2016/09/07
 * note		入力された車速値を平均値計算用バッファへ格納する
 */
TCAN_COMMON_CTRL_LOCAL void add_vehicle_speed_value(UI_32 speed_value_can)
{
	/* バッファが一杯なら１つづつ前に詰めて、最後尾を空ける */
	if (S_speed_value_count_max == 0){
		/*フェール*/
		return;
	}

	if(S_vehicle_speed_value_count >= S_speed_value_count_max) {
		/* 領域が重なるのでmemmoveへ変更 */
		(void)memmove(&S_vehicle_speed_value_buf[0], &S_vehicle_speed_value_buf[1], sizeof(UI_32) * (S_speed_value_count_max - 1));
		S_vehicle_speed_value_count--;
	}

	/* バッファ後尾に格納 */
	S_vehicle_speed_value_buf[S_vehicle_speed_value_count] = speed_value_can;
	S_vehicle_speed_value_count++;
}

/** 
 * 表示保持時間内での平均値車取得
 * 
 * author	(OFT)Katsura
 * date		2016/09/07
 * note		平均値計算用バッファの平均車速値を取得する
 */
TCAN_COMMON_CTRL_LOCAL UI_32 get_vehicle_speed_average(void)
{
	UI_8  i;
	UI_32 sum_value = 0;
	UI_32 speed_value_avr;
	
	if(S_vehicle_speed_value_count == 0) {
		return(0);
	}

	/* バッファ内の全車速値を加算 */
	for(i = 0; i < S_vehicle_speed_value_count; i++) {
		sum_value += S_vehicle_speed_value_buf[i];
	}
	
	/* 平均値算出 */
	speed_value_avr = sum_value / S_vehicle_speed_value_count;
	return speed_value_avr;
}

/** 
 * 表示用車速の指示公差計算処理
 * 
 * author	(OFT)Katsura
 * date		2016/09/07
 * note		平表示用車速の指示公差計算を行う
 */
TCAN_COMMON_CTRL_LOCAL UI_32 calc_vehicle_speed_tolerance(UI_32 speed_value)
{
	UI_8 place;
	UI_8 i;
	const S_CENTER_TOLE_TBL* 	ctl;
	UI_32 vehicle_speed_tolerance = 0;

	if(TCAN_Ctrl_GetMTPlace(&place) != TCAN_CTRL_OK) {
		return 0;
	}

	if (S_vehicleCanInfo == TCAN_VEHICLE_INFO_86A)
	{
		/*CAN受信状態:86A*/
		ctl = C_center_tole[C_Direction2SP[0][place]];
	}
	else if (S_vehicleCanInfo == TCAN_VEHICLE_INFO_86B)
	{
		/*CAN受信状態:86B*/
		ctl = C_center_tole[C_Direction2SP[1][place]];
	}
	else
	{
		return 0;
	}
	
	/* 指示交差処理後の速度検索 */
	for(i = 0 ; i < TBL_NUM; i++ )
	{
		if(ctl[i].speed >= speed_value)
		{
			/*指示公差処理後の車速*/
			vehicle_speed_tolerance = (UI_32)((SI_32)((F_32)speed_value * ctl[i].slope) + ctl[i].intercept);
			break;
		}
	}

	return vehicle_speed_tolerance;
}

/** 
 * 表示用車示値端数処理
 * 
 * author	(OFT)Katsura
 * date		2016/09/07
 * note		表示用車示値の端数処理を行う
 */
TCAN_COMMON_CTRL_LOCAL UI_32 calc_vehicle_speed_fraction(UI_32 speed_value)
{
	UI_32 speed_value_fra;
	
	/* 小数点以下切り捨て */
	speed_value_fra = speed_value / VEHICLE_SPEED_RESOLUTION * VEHICLE_SPEED_RESOLUTION;

	/* 小数点以下が0.5以上なら、0.5を加算 */
	if((speed_value % VEHICLE_SPEED_RESOLUTION) >= (VEHICLE_SPEED_RESOLUTION / 2)) {
		speed_value_fra += (VEHICLE_SPEED_RESOLUTION / 2);
	}

	return speed_value_fra;
}

/** 
 * 表示目標値算出処理
 * 
 * author	(OFT)Katsura
 * date		2016/09/07
 * note		表示目標値の計算を行う
 */
TCAN_COMMON_CTRL_LOCAL UI_32 calc_vehicle_speed_target(UI_32 speed_value)
{
	UI_32 speed_value_trg;

	/* 与えられた車速値を小数点１ケタで四捨五入する */
	if((speed_value % VEHICLE_SPEED_RESOLUTION) >= (VEHICLE_SPEED_RESOLUTION / 2)) {
		speed_value_trg = ((speed_value / VEHICLE_SPEED_RESOLUTION) + 1) * VEHICLE_SPEED_RESOLUTION;
	}
	else {
		speed_value_trg = (speed_value / VEHICLE_SPEED_RESOLUTION) * VEHICLE_SPEED_RESOLUTION;
	}
	return speed_value_trg;
}

/** 
 * ゼロ付近での動きだし／停止制御処理
 * 
 * author	(OFT)Katsura
 * date		2016/09/07
 * note		ゼロ付近での動きだし／停止時の表示用車速値を決定する
 */
TCAN_COMMON_CTRL_LOCAL UI_32 calc_vehicle_speed_near0(UI_32 speed_value_trg, UI_32 speed_value_fra)
{
	UI_32 vehicle_speed_near0 = 0;
	
	if(S_vehicle_speed_value_count < 2) {
		if(S_vehicle_speed_value_buf[S_vehicle_speed_value_count - 1] == 0) {
			vehicle_speed_near0 = 0;
		}
		else {
			vehicle_speed_near0 = speed_value_trg;
		}
	}
	else {
		/* 減速中かどうかの判定 */
		if(S_vehicle_speed_value_buf[S_vehicle_speed_value_count - 1] > S_vehicle_speed_value_buf[S_vehicle_speed_value_count - 2]) {
			/* 減速中でない場合はそのまま表示目標値 */
			vehicle_speed_near0 = speed_value_trg;
		}
		else {
			if(speed_value_trg > (2 * VEHICLE_SPEED_RESOLUTION)) {
				/* 2km/hより大きければ表示目標値 */
				vehicle_speed_near0 = speed_value_trg;
			}
			else {
				if (S_speed_value_dsp > (2 * VEHICLE_SPEED_RESOLUTION)) {
					/* 前回の表示用車速が2km/hより上なら、2km/h */
					vehicle_speed_near0 = (2 * VEHICLE_SPEED_RESOLUTION);
				}
				else if(S_speed_value_dsp > (1* VEHICLE_SPEED_RESOLUTION)) {
					vehicle_speed_near0 = (1 * VEHICLE_SPEED_RESOLUTION);
				}
				else if(S_speed_value_dsp > 0) {
					/* 前回表示車速が0km/h超～1km/hの場合 */
					if(speed_value_trg < (1 * VEHICLE_SPEED_RESOLUTION)) {
						/* 表示目標値が1km/h未満なら0km/h */
						vehicle_speed_near0 = 0;
					}
					else {
						if(speed_value_fra == 0) {
							/* 平均車速が0なら0m/h */
							vehicle_speed_near0 = 0;
						}
						else {
							/* 平均車速が0超なら1m/h */
							vehicle_speed_near0 = (1 * VEHICLE_SPEED_RESOLUTION);
						}
					}
				}
				else {
					/* 前回表示車速が0km/hなら0m/h */
					vehicle_speed_near0 = 0;
				}
			
			}
		}
	}
	return vehicle_speed_near0;
}
