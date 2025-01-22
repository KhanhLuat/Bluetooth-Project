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
/* GNSS����												*/
/********************************************************/
#ifndef	DTO459_VDV_GNSS_CTRL_H
#define	DTO459_VDV_GNSS_CTRL_H

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include "type.h"
#include "System_control_TASK.h"


/********************************************************/
/*	�}�N����`											*/
/********************************************************/
/* GNSS�M�� */
#define	GNSS_NANOSEC_RESOLUTION					0.000000001		/*	�i�m�b����\													*/
#define	GNSS_LATITUDE_RESOLUTION				0.0000001		/*	�ܓx����\														*/
#define	GNSS_LONGTUDE_RESOLUTION				0.0000001		/*	�o�x����\														*/
#define	GNSS_ALTITUDE_RESOLUTION				0.001			/*	���x����\														*/
#define	GNSS_GEOID_RESOLUTION					0.001			/*	�W�I�C�h������\												*/
#define	GNSS_HEADING_VEHICLE_RESOLUTION			0.00001			/*	���������ړ�����												*/
#define	GNSS_VDOP_RESOLUTION					0.01			/*	VDOP����\														*/
#define	GNSS_HDOP_RESOLUTION					0.01			/*	HDOP����\														*/
#define	GNSS_PDOP_RESOLUTION					0.01			/*	PDOP����\														*/
#define	GNSS_ACCURACY_RESOLUTION				0.01			/*	���x����\														*/
#define	GNSS_GSPFIX_NOFIX						0x00 			/*	No Fix															*/
#define	GNSS_GSPFIX_DEADRECKONIN_ONLY			0x01 			/*	Dead Reckoning only												*/
#define	GNSS_GSPFIX_2DFIX						0x02 			/*	2D-Fix															*/
#define	GNSS_GSPFIX_3DFIX						0x03 			/*	3D-Fix															*/
#define	GNSS_GSPFIX_GPS_AND_DEADRECKONING		0x04 			/*	GPS + dead reckoning combined									*/
#define	GNSS_GSPFIX_TIME_ONLY_FIX				0x05 			/*	Time only fix													*/

//�����x
#define	GNSS_ACCERERATION_RESOLUTION_FR			0.0009765625	/*	(2^(-10))�O����������x����\�i���Z��A����\1�j				*/
#define	GNSS_ACCERERATION_RESOLUTION_LR			0.0009765625	/*	(2^(-10))�����������x����\�i���Z��A����\1�j					*/
#define	GNSS_ACCERERATION_RESOLUTION_VER		0.0009765625	/*	(2^(-10))�������������x����\�i���Z��A����\1�j				*/
//���[�A�s�b�`�A���[��
#define	GNSS_GYROSCOPE_RESOLUTION_YAW			0.00024414062	/*	(2^(-12))���[���[�g����\�i���Z��A����\1�j					*/
#define	GNSS_GYROSCOPE_RESOLUTION_PITCH			0.00024414062	/*	(2^(-12))�s�b�`���[�g����\�i���Z��A����\1�j					*/
#define	GNSS_GYROSCOPE_RESOLUTION_ROLL			0.00024414062	/*	(2^(-12))���[�����[�g����\�i���Z��A����\1�j					*/

/********************************************************/
/*	�\���̐錾											*/
/********************************************************/

/* GNSS HNR-PVT���\���� */
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

/* GNSS NAV-PVT���\���� */
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

/* GNSS NAV-DOP���\���� */
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

/* GNSS NAV-POSECEF�\���� */
typedef struct {
	UI_32		i_tow;				/* GPS time of week				*/
	SI_32		ecef_x;				/* ECEF X coordinate			*/
	SI_32		ecef_y;				/* ECEF Y coordinate			*/
	SI_32		ecef_z;				/* ECEF Z coordinate			*/
	UI_32		p_acc;				/* Position Accuracy Estimate	*/
} T_Gnss_ctrl_nav_posecef_info;

/* GNSS ESF-RAW���\���� */
typedef struct {
	UI_32		x_axis_gyro;		/* Gyroscope x-axis 			*/
	UI_32		y_axis_gyro;		/* Gyroscope y-axis 			*/
	UI_32		z_axis_gyro;		/* Gyroscope z-axis 			*/
	UI_32		x_axis_accel;		/* Accelerometer x-axis			*/
	UI_32		y_axis_accel;		/* Accelerometer y-axis			*/
	UI_32		z_axis_accel;		/* Accelerometer z-axis			*/
	UI_32		gyro_temp;			/* Gyroscope  Temperture		*/
} T_Gnss_ctrl_esf_raw_info;

/* GNSS ESF-MEAS���\���� */
typedef struct {
	UI_8		en_x_axis_gyro:1;	/* Gyroscope x-axis�L���t���O	���[��		*/
	UI_8		en_y_axis_gyro:1;	/* Gyroscope y-axis�L���t���O	�s�b�`		*/
	UI_8		en_z_axis_gyro:1;	/* Gyroscope z-axis�L���t���O	���[		*/
	UI_8		en_x_axis_accel:1;	/* Accelerometer x-axis�L���t���O	*/
	UI_8		en_y_axis_accel:1;	/* Accelerometer y-axis�L���t���O	*/
	UI_8		en_z_axis_accel:1;	/* Accelerometer z-axis�L���t���O	*/
	UI_8		en_gyro_temp:1;		/* Gyroscope  Temperture�L���t���O	*/

	UI_32		x_axis_gyro;		/* Gyroscope x-axis ���[��			*/
	UI_32		y_axis_gyro;		/* Gyroscope y-axis �s�b�`			*/
	UI_32		z_axis_gyro;		/* Gyroscope z-axis ���[			*/
	UI_32		x_axis_accel;		/* Accelerometer x-axis			*/
	UI_32		y_axis_accel;		/* Accelerometer y-axis			*/
	UI_32		z_axis_accel;		/* Accelerometer z-axis			*/
	UI_32		gyro_temp;			/* Gyroscope  Temperture		*/
} T_Gnss_ctrl_esf_meas_info;



/********************************************************/
/*	�O���֐�											*/
/********************************************************/

/********************************************************/
/* �֐�����		: Gnss_control_Init						*/
/* �@�\�̐���	: GNSS���䏉����						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Gnss_control_Init(void);

/********************************************************/
/* �֐�����		: Gnss_control_Main						*/
/* �@�\�̐���	: GNSS���䃁�C��						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Gnss_control_Main(void);

/********************************************************/
/* �֐�����		: Gnss_cntrol_GetHnrPvtInfo				*/
/* �@�\�̐���	: GNSS HNR-PVT���J�����g�l���擾����	*/
/* ����			: void									*/
/* �߂�l		: T_Gnss_ctrl_hnr_pvt_info :			*/
/*									HNR-PVT���i�[Ptr	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/02								*/
/* ���l			: 										*/
/********************************************************/
T_Gnss_ctrl_hnr_pvt_info *Gnss_cntrol_GetHnrPvtInfo(void);

/********************************************************/
/* �֐�����		: Gnss_cntrol_GetNavPvtInfo				*/
/* �@�\�̐���	: GNSS NAV-PVT���J�����g�l���擾����	*/
/* ����			: void									*/
/* �߂�l		: T_Gnss_ctrl_nav_pvt_info :			*/
/*									NAV-PVT���i�[Ptr	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/28								*/
/* ���l			: 										*/
/********************************************************/
T_Gnss_ctrl_nav_pvt_info *Gnss_cntrol_GetNavPvtInfo(void);

/********************************************************/
/* �֐�����		: Gnss_control_GetNavDopInfo			*/
/* �@�\�̐���	: GNSS NAV-DOP���J�����g�l���擾����	*/
/* ����			: void									*/
/* �߂�l		: T_Gnss_ctrl_nav_dop_info :			*/
/*									NAV-DOP���i�[Ptr	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/02								*/
/* ���l			: 										*/
/********************************************************/
T_Gnss_ctrl_nav_dop_info *Gnss_control_GetNavDopInfo(void);

/********************************************************/
/* �֐�����		: Gnss_cntrol_GetNavPosecefInfo			*/
/* �@�\�̐���	: GNSS NAV-POSECEF���J�����g�l���擾����	*/
/* ����			: void									*/
/* �߂�l		: T_Gnss_ctrl_nav_posecef_info :		*/
/*								NAV-POSECEF���i�[Ptr	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/28								*/
/* ���l			: 										*/
/*   �L����NAV-PVT��񂪑��݂��Ȃ��ꍇ�ANULL�����^�[��	*/
/*   ����B												*/
/********************************************************/
T_Gnss_ctrl_nav_posecef_info *Gnss_cntrol_GetNavPosecefInfo(void);

/********************************************************/
/* �֐�����		: Gnss_control_GetNavPosecefInfo		*/
/* �@�\�̐���	: GNSS NAV-POSECEF���J�����g�l���擾����	*/
/* ����			: void									*/
/* �߂�l		: T_Gnss_ctrl_nav_posecef_info :		*/
/*								NAV-POSECEF���i�[Ptr	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/28								*/
/* ���l			: 										*/
/********************************************************/
T_Gnss_ctrl_nav_posecef_info *Gnss_control_GetNavPosecefInfo(void);

/********************************************************/
/* �֐�����		: Gnss_control_GetEsfRawInfo			*/
/* �@�\�̐���	: GNSS ESF-RAW���J�����g�l���擾����	*/
/* ����			: void									*/
/* �߂�l		: T_Gnss_ctrl_esf_raw_info :			*/
/*									ESF-RAW���i�[Ptr	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/02								*/
/* ���l			: 										*/
/********************************************************/
T_Gnss_ctrl_esf_raw_info *Gnss_control_GetEsfRawInfo(void);

/********************************************************/
/* �֐�����		�F Gnss_control_GetMeasRawInfo			*/
/* �@�\�̐���	�F GNSS ESF-MEAS���J�����g�l���擾����*/
/* ����			�F void									*/
/* �߂�l		�F T_Gnss_ctrl_esf_meas_info :			*/
/*									ESF-MEAS���i�[Ptr	*/
/* �쐬��		�F (OFT)Katsura							*/
/* �쐬��		�F 16/09/02								*/
/* ���l			�F 										*/
/********************************************************/
T_Gnss_ctrl_esf_meas_info *Gnss_control_GetEsfMeasInfo(void);


/********************************************************/
/* �֐�����		: Gnss_control_GetGpsFixType			*/
/* �@�\�̐���	: GPS Fix Type�l���擾����				*/
/* ����			: (O)gps_fix_type : GPS Fix Type�l		*/
/* �߂�l		: E_N_RC : �擾����(N_RC_OK, N_RC_NG)	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/07								*/
/* ���l			: 										*/
/********************************************************/
E_N_RC Gnss_control_GetGpsFixType(UI_8 *gps_fix_type);

#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata */
/********************************************************/
/* �֐�����		: Gnss_control_iSGnssFixedAtOneTime		*/
/* �@�\�̐���	: GPSFixType�l���ߋ��ɂP�x�ł���M�ς�	*/
/* 				: �ƂȂ���������						*/
/* ����			: void									*/
/* �߂�l		: N_TRUE:��M�ς� / N_FALSE:����M		*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 18/12/14								*/
/* ���l			: 										*/
/********************************************************/
B_8 Gnss_control_iSGnssFixedAtOneTime(void);
#endif /* _VDV_SUPRA�@2018.12.14 S.Sakata */

/********************************************************/
/* �֐�����		: Gnss_control_getUartData				*/
/* �@�\�̐���	:	UART�f�[�^�擾						*/
/* ����			:	(I/ )buf	�f�[�^�o�b�t�@			*/
/*				:	(I/ )len	�擾�f�[�^��(byte)		*/
/* �߂�l		:	�擾�ł����f�[�^��(byte)			*/
/* �쐬��		:	(OFT)Toguchi						*/
/* �쐬��		:	17/01/20							*/
/* ���l			: 										*/
/********************************************************/
UI_16 Gnss_control_getUartData(UI_8 *buf, UI_16 len);
#endif	/*DTO459_VDV_GNSS_CTRL_H*/
