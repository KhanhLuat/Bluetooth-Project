/********************************************************/
/* file TCan_comm_Ctrl.h								*/
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

#ifndef	TCAN_COMM_CTRL_H
#define	TCAN_COMM_CTRL_H

#include "type.h"

/********************************************************/
/*	外部マクロ											*/
/********************************************************/
/**
 *	動作状態
 */
typedef enum e_tcan_result {
	TCAN_CTRL_OK = 0,		/**< 正常値		*/
	TCAN_CTRL_ERROR,		/**< 異常値		*/
	TCAN_CTRL_NO_RECEIVE	/**< 未受信		*/
} E_TCAN_RESULT;


/* 車種情報*/
typedef enum e_tcan_vehicle_info {
	TCAN_VEHICLE_INFO_UNKNOWN = 0x0000,		/**< 不明		*/
	TCAN_VEHICLE_INFO_86A	  = 0x0010,		/**< 086A		*/
	TCAN_VEHICLE_INFO_86B	  = 0x0011,		/**< 086B		*/
}E_TCAN_VEHICLE_INFO;

/********************************************************/
/*	定数宣言											*/
/********************************************************/
#define HV_MSG_ID_620		0x620		/* カーテシ */
#define HV_MSG_ID_0B4		0x0B4		/* 車速 */
#define HV_MSG_ID_3BC		0x3BC		/* シフトポジション */
#define HV_MSG_ID_245		0x245		/* アクセル開度 */
#define HV_MSG_ID_247		0x247		/* EVモードMSG */
#define HV_MSG_ID_3B6		0x3B6		/* 高圧バッテリ残量 */
#define HV_MSG_ID_1C4		0x1C4		/* エンジン回転数 */
#define HV_MSG_ID_3D3		0x3D3		/* 燃料消費量 */

#define MT_MSG_ID_374		0x374		/* ドアSW */
#define MT_MSG_ID_140		0x140		/* アクセル開度 */
#define MT_MSG_ID_141		0x141		/* エンジン回転数 */
#define MT_MSG_ID_144		0x144		/* トランスミッション情報 */
#define MT_MSG_ID_148		0x148		/* スポーツシフトｷﾞｱ位置表示情報等 */
#define MT_MSG_ID_360		0x360		/* エンジン回転数(TA用) */
#define MT_MSG_ID_361		0x361		/* ギアポジション */
#define MT_MSG_ID_368		0x368		/* AT種類 */
#define MT_MSG_ID_0D1		0x0D1		/* 駆動輪軸平均車輪速 */
#define MT_MSG_ID_0D3		0x0D3		/* 車速パルス信号積算値 */

#define MT_MSG_ID_152		0x152		/* ブレーキ/ランプ */
#define MT_MSG_ID_282		0x282		/* バックルSW */
#define MT_MSG_ID_0D0		0x0D0		/* 舵角センサ等 */
#define MT_MSG_ID_0D4		0x0D4		/* ４輪車輪速 */
#define MT_MSG_ID_375		0x375		/* ドアロック状態 */

#define MT_MSG_ID_040		0x040		/* アクセル開度 */
#define MT_MSG_ID_048		0x048		/* スポーツシフトｷﾞｱ位置表示情報等 */
#define MT_MSG_ID_139		0x139		/* 駆動輪軸平均車輪速 */
#define MT_MSG_ID_13A		0x13A		/* ４輪車輪速 */
#define MT_MSG_ID_13B		0x13B		/* 前後加速度 */
#define MT_MSG_ID_13C		0x13C		/* 車速パルス信号積算値 */
#define MT_MSG_ID_138		0x138		/* 舵角センサ等 */
#define MT_MSG_ID_146		0x146		/* エンジン回転数 */
#define MT_MSG_ID_241		0x241		/* ギアポジション,燃料消費量 */
#define MT_MSG_ID_345		0x345		/* トランスミッション情報 */
#define MT_MSG_ID_390		0x390		/* バックルSW */
#define MT_MSG_ID_3AC		0x3AC		/* ブレーキ/ランプ */
#define MT_MSG_ID_34A		0x34A		/* スノー、スポーツランプ */
#define MT_MSG_ID_652		0x652		/* ドアロック状態 */
#define MT_MSG_ID_68C		0x68C		/* ブランドフラグ */
#define MT_MSG_ID_6E2		0x6E2		/* タイヤ空気圧 */
#define MT_MSG_ID_328		0x328		/* VDC信号 */
#define MT_MSG_ID_041		0x041		/* 実エンジン軸トルク */
#define MT_MSG_ID_04B		0x04B		/* 目標ギア段数 */




/********************************************************/
/*	外部関数											*/
/********************************************************/
void TCAN_Ctrl_Init(void);
void TCAN_Ctrl_Main(void);
void TCAN_Ctrl_SetMessage(UI_32 id, const UI_8 len, const UI_8* data);
void TCAN_Ctrl_SetupCanMessage(E_TCAN_VEHICLE_INFO info);

E_TCAN_RESULT TCAN_Ctrl_GetMTDoorOpen(UI_8 *data);			/* MT ドアSW D席情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTPDoorOpen(UI_8 *data);			/* MT ドアSW P席情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTrunkDoorOpen(UI_8 *data);		/* MT ドアSW トランク情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTRatioAccel(UI_8 *data);		/* MT アクセル開度率情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTEngRpm(UI_16 *data);			/* MT エンジン回転数情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTEngRpmTa(UI_16 *data);			/* MT エンジン回転数(タコメータ用)情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTGearEstm(UI_8 *data);			/* MT ギアポジション表示報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSpWheelAve(UI_16 *data);		/* MT 駆動輪軸平均車輪速情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSpPulseInteg(UI_8 *data);		/* MT 車速パルス信号積算値情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTBreakLamp(UI_8 *data);			/* MT ブレーキランプ情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTHandBreak(UI_8 *data);			/* MT パーキングブレーキ情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTReverse(UI_8 *data);			/* MT リバース情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTHeadLampHi(UI_8 *data);		/* MT ヘッドランプHi情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTHeadLampLo(UI_8 *data);		/* MT ヘッドランプLo情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTHeadLampSmall(UI_8 *data);		/* MT ヘッドランプSmall情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTOutTemp(UI_8 *data);			/* MT 表示用外気温取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTPSeatBelt(UI_8 *data);			/* MT シートベルトバックル P席 情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTDSeatBelt(UI_8 *data);			/* MT シートベルトバックル D席 情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTPlace(UI_8 *data);				/* MT 仕向地情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTempOil(UI_8 *data);			/* MT エンジン油温取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTempCoolant(UI_8 *data);		/* MT エンジン水温取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSetCc(UI_8 *data);				/* MT クルコンセットランプ状態取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSteerAngle(UI_16 *data);		/* MT 舵角センサ情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTYawRate(UI_16 *data);			/* MT ヨーレート情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTAccLR(UI_8 *data);				/* MT 横加速度情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTAccFR(UI_8 *data);				/* MT 前後Gセンサ情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTransmissionType(UI_8 *data);	/* MT トランスミッション情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSportGearPosition(UI_8 *data);	/* MT スポーツシフトギア位置表示情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTShiftPosition(UI_8 *data);		/* MT シフト位置情報(フェールセーフ処理後)取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTModeShiftControl(UI_8 *data);	/* MT TCM変速制御モード情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTAtType(UI_8 *data);			/* MT AT種類取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTIgn(UI_8 *data);  				/* MT IGN情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTAcc(UI_8 *data);  				/* MT ACC情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTBrandFlag(UI_8 *data);  		/* MT ブランドフラグ情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTargetGearPos(UI_8 *data);		/* MT 目標ギア位置情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSportsLamp(UI_8 *data);		/* MT スポーツランプ情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSnowHoldLamp(UI_8 *data);		/* MT スノーホールド情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTNodeID(UI_8 *data);			/* MT ノードID情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSleepAck(UI_8 *data);			/* MT Sleep Ack情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSleepInd(UI_8 *data);			/* MT Sleep Ind情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTLimpHome(UI_8 *data);			/* MT Limp Home情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTRing(UI_8 *data);				/* MT Ring情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTAlive(UI_8 *data);				/* MT Alive情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTWkup(UI_8 *data);				/* MT Wkup情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTInit(UI_8 *data);				/* MT Init情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSpecNo(UI_16 *data);			/* MT Spec. No.情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMtSpdWheelFtlh(UI_16 *data);		/* MT 左前輪車輪速情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMtSpdWheelFtrh(UI_16 *data);		/* MT 右前輪車輪速情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMtSpdWheelRrlh(UI_16 *data);		/* MT 左後輪車輪速情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMtSpdWheelRrrh(UI_16 *data);		/* MT 右後輪車輪速情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMtDoorLockStatPass(UI_8 *data);	/* MT ドアロック状態（助手席）情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMtDoorLockStatDrvr(UI_8 *data);	/* MT ドアロック状態（運転席）情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetMtBrakePressure(UI_8 *data);		/* MT ブレーキ圧情報取得 */


E_TCAN_RESULT TCAN_Ctrl_GetOdo(UI_32 *data);				/* 始動後走行距離取得 (HV/MT共通) */
E_TCAN_RESULT TCAN_Ctrl_GetFCSum(UI_32 *data);				/* 燃料噴射量積算値 (HV/MT共通) */
E_TCAN_RESULT TCAN_Ctrl_GetVehicleSpeedForDisp(UI_32 *data);/* 表示用車速情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetVehicleSpeed(UI_32 *data);		/* 車速情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetSmoothingBrakePressure(UI_8 *data);/* ブレーキ圧なまし値取得 */
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressFtlh(UI_8 *data);		/* 左前輪空気圧取得 		*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressFtrh(UI_8 *data);		/* 右前輪空気圧取得 		*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressRrlh(UI_8 *data);		/* 左後輪空気圧取得 		*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressRrrh(UI_8 *data);		/* 右後輪空気圧取得 		*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressUnit(UI_8 *data);		/* 空気圧単位取得 			*/
E_TCAN_VEHICLE_INFO TCAN_Ctrl_GetVehicleInfo(void);			/* 車種情報取得 			*/
E_TCAN_RESULT TCAN_Ctrl_GetAtManualModeSW(UI_8 *data);		/* マニュアルモードSW情報取得 */
E_TCAN_RESULT TCAN_Ctrl_GetAtVscOnOff(UI_8 *data);			/* VSC ON/OFF情報取得 				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscAct(UI_8 *data);			/* VSC/TCS作動状態情報取得 			*/
E_TCAN_RESULT TCAN_Ctrl_GetAtTrcMode(UI_8 *data);			/* トラクション重視モード情報取得 	*/
E_TCAN_RESULT TCAN_Ctrl_GetAtTrcOnOff(UI_8 *data);			/* TRC ON/OFF情報取得 				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtTrcAct(UI_8 *data);			/* TRC 作動状態情報取得 			*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAbsAct(UI_8 *data);			/* ABS作動状態情報取得 				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscWarning(UI_8 *data);		/* VSCウォーニング情報取得 			*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAbsWarning(UI_8 *data);		/* ABSウォーニング情報取得 			*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAbsFail(UI_8 *data);			/* ABSシステムフェイル情報取得 		*/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscUnderStr(UI_8 *data);		/* VSCアンダー制御状態				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscOverStr(UI_8 *data);		/* VSCオーバー制御状態				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqUpReqFlg(UI_8 *data);	/* エンジントルクアップ要求フラグ	*/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqDwnReqFlg(UI_8 *data);	/* エンジントルク減少要求フラグ		*/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqUpReqVal(UI_16 *data);	/* エンジントルクアップ要求値		*/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqDwnReqVal(UI_16 *data);	/* エンジントルク減少要求値			*/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqShaft(UI_16 *data);		/* 実エンジン軸トルク				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtEcbWarning(UI_8 *data);		/* ECBウォーニング情報取得			*/
E_TCAN_RESULT TCAN_Ctrl_GetAtIntakePress(UI_8 *data);		/* 吸入管圧力情報取得				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAtmosphericPress(UI_8 *data);	/* 大気圧情報取得					*/
E_TCAN_RESULT TCAN_Ctrl_GetAtBlinkerLeft(UI_8 *data);		/* ウィンカー左情報取得				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtBlinkerRight(UI_8 *data);		/* ウィンカー右情報取得				*/
E_TCAN_RESULT TCAN_Ctrl_GetMtHoodDoorSW(UI_8 *data);		/* ドアSW フード情報取得			*/

#endif	/* TCAN_COMM_CTRL_H */
