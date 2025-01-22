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
/* file DTO459_VDV_Gnss_ctrl.h							*/
/* GNSS制御												*/
/********************************************************/
#ifndef	DTO459_VDV_GNSS_CTRL_H
#define	DTO459_VDV_GNSS_CTRL_H

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include "type.h"
#include "System_control_TASK.h"


/********************************************************/
/*	マクロ定義											*/
/********************************************************/
/* GNSS信号 */
#define	GNSS_NANOSEC_RESOLUTION					0.000000001		/*	ナノ秒分解能													*/
#define	GNSS_LATITUDE_RESOLUTION				0.0000001		/*	緯度分解能														*/
#define	GNSS_LONGTUDE_RESOLUTION				0.0000001		/*	経度分解能														*/
#define	GNSS_ALTITUDE_RESOLUTION				0.001			/*	高度分解能														*/
#define	GNSS_GEOID_RESOLUTION					0.001			/*	ジオイド高分解能												*/
#define	GNSS_HEADING_VEHICLE_RESOLUTION			0.00001			/*	水平方向移動方向												*/
#define	GNSS_VDOP_RESOLUTION					0.01			/*	VDOP分解能														*/
#define	GNSS_HDOP_RESOLUTION					0.01			/*	HDOP分解能														*/
#define	GNSS_PDOP_RESOLUTION					0.01			/*	PDOP分解能														*/
#define	GNSS_ACCURACY_RESOLUTION				0.01			/*	精度分解能														*/
#define	GNSS_GSPFIX_NOFIX						0x00 			/*	No Fix															*/
#define	GNSS_GSPFIX_DEADRECKONIN_ONLY			0x01 			/*	Dead Reckoning only												*/
#define	GNSS_GSPFIX_2DFIX						0x02 			/*	2D-Fix															*/
#define	GNSS_GSPFIX_3DFIX						0x03 			/*	3D-Fix															*/
#define	GNSS_GSPFIX_GPS_AND_DEADRECKONING		0x04 			/*	GPS + dead reckoning combined									*/
#define	GNSS_GSPFIX_TIME_ONLY_FIX				0x05 			/*	Time only fix													*/

//加速度
#define	GNSS_ACCERERATION_RESOLUTION_FR			0.0009765625	/*	(2^(-10))前後方向加速度分解能（演算後、分解能1）				*/
#define	GNSS_ACCERERATION_RESOLUTION_LR			0.0009765625	/*	(2^(-10))横方向加速度分解能（演算後、分解能1）					*/
#define	GNSS_ACCERERATION_RESOLUTION_VER		0.0009765625	/*	(2^(-10))垂直方向加速度分解能（演算後、分解能1）				*/
//ヨー、ピッチ、ロール
#define	GNSS_GYROSCOPE_RESOLUTION_YAW			0.00024414062	/*	(2^(-12))ヨーレート分解能（演算後、分解能1）					*/
#define	GNSS_GYROSCOPE_RESOLUTION_PITCH			0.00024414062	/*	(2^(-12))ピッチレート分解能（演算後、分解能1）					*/
#define	GNSS_GYROSCOPE_RESOLUTION_ROLL			0.00024414062	/*	(2^(-12))ロールレート分解能（演算後、分解能1）					*/

/********************************************************/
/*	構造体宣言											*/
/********************************************************/

/* GNSS HNR-PVT情報構造体 */
typedef struct {
	UI_32		tow;				/* GPS time of week				*/
	UI_16		year;				/* Year(UTC)					*/
	UI_8		month;				/* Month(1-12)(UTC)				*/
	UI_8		day;				/* Day(1-31)(UTC)				*/
	UI_8		hour;				/* Hour(0-23)(UTC)				*/
	UI_8		min;				/* Minute(0-59)(UTC)			*/
	UI_8		sec;				/* Second(0-60)(UTC)			*/
	UI_8		valid;				/* Validity Flags				*/
	SI_32		nano;				/* Fraction of second(UTC)		*/
	UI_8		gpsFix;				/* GPSfix Type					*/
	UI_8		flags;				/* Fix Status Flags				*/
	SI_32		lon;				/* Longitude					*/
	SI_32		lat;				/* Latitude						*/
	SI_32		height;				/* Height above Ellipsolid		*/
	SI_32		h_msl;				/* Height above mean sea level	*/
	SI_32		g_speed;			/* Ground Speed(2-D)			*/
	SI_32		speed;				/* Speed(3-D)					*/
	SI_32		head_mot;			/* Heading of motion(2-D)		*/
	SI_32		head_veh;			/* Heading of vehicle(2-D)		*/
} T_Gnss_ctrl_hnr_pvt_info;

/* GNSS NAV-PVT情報構造体 */
typedef struct {
	UI_32		tow;				/* GPS time of week				*/
	UI_16		year;				/* Year(UTC)					*/
	UI_8		month;				/* Month(1-12)(UTC)				*/
	UI_8		day;				/* Day(1-31)(UTC)				*/
	UI_8		hour;				/* Hour(0-23)(UTC)				*/
	UI_8		min;				/* Minute(0-59)(UTC)			*/
	UI_8		sec;				/* Second(0-60)(UTC)			*/
	UI_8		valid;				/* Validity Flags				*/
	UI_32		t_acc;				/* Time Accuract Estimate(UTC)	*/
	SI_32		nano;				/* Fraction of second(UTC)		*/
	UI_8		gpsFix;				/* GPSfix Type					*/
	UI_8		flags;				/* Fix Status Flags				*/
	UI_8		num_sv;				/* Number of satellites used in Nav solution */
	SI_32		lon;				/* Longitude					*/
	SI_32		lat;				/* Latitude						*/
	SI_32		height;				/* Height above Ellipsolid		*/
	SI_32		h_msl;				/* Height above mean sea level	*/
	UI_32		h_acc;				/* Horizontal accuracy estimate	*/
	UI_32		v_acc;				/* Vertical accuracy estimate	*/
	SI_32		vel_n;				/* NED north velocity			*/
	SI_32		vel_e;				/* NED east velocity			*/
	SI_32		vel_d;				/* NED down velocity			*/
	SI_32		g_speed;			/* Ground Speed(2-D)			*/
	SI_32		head_mot;			/* Heading of motion(2-D)		*/
	UI_32		s_acc;				/* Speed accuracy estimate		*/
	UI_32		head_acc;			/* Heading accuracy estimate	*/
	UI_16		p_dop;				/* Position DOP					*/
	SI_32		head_veh;			/* Heading of vehicle(2-D)		*/
} T_Gnss_ctrl_nav_pvt_info;

/* GNSS NAV-DOP情報構造体 */
typedef struct {
	UI_32		i_tow;				/* GPS time of week				*/
	UI_16		g_dop;				/* Geometric DOP				*/
	UI_16		p_dop;				/* Position DOP					*/
	UI_16		t_dop;				/* Time DOP						*/
	UI_16		v_dop;				/* Vertical DOP					*/
	UI_16		h_dop;				/* Horizontal DOP				*/
	UI_16		n_dop;				/* Northing DOP					*/
	UI_16		e_dop;				/* Easting DOP					*/
} T_Gnss_ctrl_nav_dop_info;

/* GNSS NAV-POSECEF構造体 */
typedef struct {
	UI_32		i_tow;				/* GPS time of week				*/
	SI_32		ecef_x;				/* ECEF X coordinate			*/
	SI_32		ecef_y;				/* ECEF Y coordinate			*/
	SI_32		ecef_z;				/* ECEF Z coordinate			*/
	UI_32		p_acc;				/* Position Accuracy Estimate	*/
} T_Gnss_ctrl_nav_posecef_info;

/* GNSS ESF-RAW情報構造体 */
typedef struct {
	UI_32		x_axis_gyro;		/* Gyroscope x-axis 			*/
	UI_32		y_axis_gyro;		/* Gyroscope y-axis 			*/
	UI_32		z_axis_gyro;		/* Gyroscope z-axis 			*/
	UI_32		x_axis_accel;		/* Accelerometer x-axis			*/
	UI_32		y_axis_accel;		/* Accelerometer y-axis			*/
	UI_32		z_axis_accel;		/* Accelerometer z-axis			*/
	UI_32		gyro_temp;			/* Gyroscope  Temperture		*/
} T_Gnss_ctrl_esf_raw_info;

/* GNSS ESF-MEAS情報構造体 */
typedef struct {
	UI_8		en_x_axis_gyro:1;	/* Gyroscope x-axis有効フラグ	ロール		*/
	UI_8		en_y_axis_gyro:1;	/* Gyroscope y-axis有効フラグ	ピッチ		*/
	UI_8		en_z_axis_gyro:1;	/* Gyroscope z-axis有効フラグ	ヨー		*/
	UI_8		en_x_axis_accel:1;	/* Accelerometer x-axis有効フラグ	*/
	UI_8		en_y_axis_accel:1;	/* Accelerometer y-axis有効フラグ	*/
	UI_8		en_z_axis_accel:1;	/* Accelerometer z-axis有効フラグ	*/
	UI_8		en_gyro_temp:1;		/* Gyroscope  Temperture有効フラグ	*/

	UI_32		x_axis_gyro;		/* Gyroscope x-axis ロール			*/
	UI_32		y_axis_gyro;		/* Gyroscope y-axis ピッチ			*/
	UI_32		z_axis_gyro;		/* Gyroscope z-axis ヨー			*/
	UI_32		x_axis_accel;		/* Accelerometer x-axis			*/
	UI_32		y_axis_accel;		/* Accelerometer y-axis			*/
	UI_32		z_axis_accel;		/* Accelerometer z-axis			*/
	UI_32		gyro_temp;			/* Gyroscope  Temperture		*/
} T_Gnss_ctrl_esf_meas_info;



/********************************************************/
/*	外部関数											*/
/********************************************************/

/********************************************************/
/* 関数名称		: Gnss_control_Init						*/
/* 機能の説明	: GNSS制御初期化						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Gnss_control_Init(void);

/********************************************************/
/* 関数名称		: Gnss_control_Main						*/
/* 機能の説明	: GNSS制御メイン						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Gnss_control_Main(void);

/********************************************************/
/* 関数名称		: Gnss_cntrol_GetHnrPvtInfo				*/
/* 機能の説明	: GNSS HNR-PVT情報カレント値を取得する	*/
/* 引数			: void									*/
/* 戻り値		: T_Gnss_ctrl_hnr_pvt_info :			*/
/*									HNR-PVT情報格納Ptr	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/02								*/
/* 備考			: 										*/
/********************************************************/
T_Gnss_ctrl_hnr_pvt_info *Gnss_cntrol_GetHnrPvtInfo(void);

/********************************************************/
/* 関数名称		: Gnss_cntrol_GetNavPvtInfo				*/
/* 機能の説明	: GNSS NAV-PVT情報カレント値を取得する	*/
/* 引数			: void									*/
/* 戻り値		: T_Gnss_ctrl_nav_pvt_info :			*/
/*									NAV-PVT情報格納Ptr	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/28								*/
/* 備考			: 										*/
/********************************************************/
T_Gnss_ctrl_nav_pvt_info *Gnss_cntrol_GetNavPvtInfo(void);

/********************************************************/
/* 関数名称		: Gnss_control_GetNavDopInfo			*/
/* 機能の説明	: GNSS NAV-DOP情報カレント値を取得する	*/
/* 引数			: void									*/
/* 戻り値		: T_Gnss_ctrl_nav_dop_info :			*/
/*									NAV-DOP情報格納Ptr	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/02								*/
/* 備考			: 										*/
/********************************************************/
T_Gnss_ctrl_nav_dop_info *Gnss_control_GetNavDopInfo(void);

/********************************************************/
/* 関数名称		: Gnss_cntrol_GetNavPosecefInfo			*/
/* 機能の説明	: GNSS NAV-POSECEF情報カレント値を取得する	*/
/* 引数			: void									*/
/* 戻り値		: T_Gnss_ctrl_nav_posecef_info :		*/
/*								NAV-POSECEF情報格納Ptr	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/28								*/
/* 備考			: 										*/
/*   有効なNAV-PVT情報が存在しない場合、NULLをリターン	*/
/*   する。												*/
/********************************************************/
T_Gnss_ctrl_nav_posecef_info *Gnss_cntrol_GetNavPosecefInfo(void);

/********************************************************/
/* 関数名称		: Gnss_control_GetNavPosecefInfo		*/
/* 機能の説明	: GNSS NAV-POSECEF情報カレント値を取得する	*/
/* 引数			: void									*/
/* 戻り値		: T_Gnss_ctrl_nav_posecef_info :		*/
/*								NAV-POSECEF情報格納Ptr	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/28								*/
/* 備考			: 										*/
/********************************************************/
T_Gnss_ctrl_nav_posecef_info *Gnss_control_GetNavPosecefInfo(void);

/********************************************************/
/* 関数名称		: Gnss_control_GetEsfRawInfo			*/
/* 機能の説明	: GNSS ESF-RAW情報カレント値を取得する	*/
/* 引数			: void									*/
/* 戻り値		: T_Gnss_ctrl_esf_raw_info :			*/
/*									ESF-RAW情報格納Ptr	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/02								*/
/* 備考			: 										*/
/********************************************************/
T_Gnss_ctrl_esf_raw_info *Gnss_control_GetEsfRawInfo(void);

/********************************************************/
/* 関数名称		： Gnss_control_GetMeasRawInfo			*/
/* 機能の説明	： GNSS ESF-MEAS情報カレント値を取得する*/
/* 引数			： void									*/
/* 戻り値		： T_Gnss_ctrl_esf_meas_info :			*/
/*									ESF-MEAS情報格納Ptr	*/
/* 作成者		： (OFT)Katsura							*/
/* 作成日		： 16/09/02								*/
/* 備考			： 										*/
/********************************************************/
T_Gnss_ctrl_esf_meas_info *Gnss_control_GetEsfMeasInfo(void);


/********************************************************/
/* 関数名称		: Gnss_control_GetGpsFixType			*/
/* 機能の説明	: GPS Fix Type値を取得する				*/
/* 引数			: (O)gps_fix_type : GPS Fix Type値		*/
/* 戻り値		: E_N_RC : 取得結果(N_RC_OK, N_RC_NG)	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/07								*/
/* 備考			: 										*/
/********************************************************/
E_N_RC Gnss_control_GetGpsFixType(UI_8 *gps_fix_type);

#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata */
/********************************************************/
/* 関数名称		: Gnss_control_iSGnssFixedAtOneTime		*/
/* 機能の説明	: GPSFixType値が過去に１度でも受信済み	*/
/* 				: となったか判定						*/
/* 引数			: void									*/
/* 戻り値		: N_TRUE:受信済み / N_FALSE:未受信		*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 18/12/14								*/
/* 備考			: 										*/
/********************************************************/
B_8 Gnss_control_iSGnssFixedAtOneTime(void);
#endif /* _VDV_SUPRA　2018.12.14 S.Sakata */

/********************************************************/
/* 関数名称		: Gnss_control_getUartData				*/
/* 機能の説明	:	UARTデータ取得						*/
/* 引数			:	(I/ )buf	データバッファ			*/
/*				:	(I/ )len	取得データ長(byte)		*/
/* 戻り値		:	取得できたデータ長(byte)			*/
/* 作成者		:	(OFT)Toguchi						*/
/* 作成日		:	17/01/20							*/
/* 備考			: 										*/
/********************************************************/
UI_16 Gnss_control_getUartData(UI_8 *buf, UI_16 len);
#endif	/*DTO459_VDV_GNSS_CTRL_H*/
