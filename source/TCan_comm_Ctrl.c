/********************************************************/
/* file TCan_comm_Ctrl.c								*/
/* T-CAN����											*/
/* 														*/
/* T-CAN�̃��b�Z�[�W�̒��o�A�ώZ���s���B				*/
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
/*	�����}�N��											*/
/********************************************************/
#define TCAN_COMMON_CTRL_LOCAL	static

#define		SP_RUN_TH_MT		(31)				/* 1.75[km/h]�����l(MT)  ����\:0.05625km/h */
#define		SP_RUN_TH_HV		(175)				/* 1.75[km/h]�����l(HV)  ����\:0.01km/h */

#define		COEF				(19.623233909f)		/* �n���㑖�s�����ϊ��W��:100000��(2�~637�~NP[4]) */
#define		SP_MAX_TH			(218871528UL)		/* SP-ODO�ő�l:(2^32)��COEF) */

#define		FUEL_CYCLE			(50)				/* �R�����˗ʐώZ�l �Z�o����:500ms */

#define		UI32_MAX			(4294967295UL)		

#define		WHEEL_AVE_MIN		(0x0018)			/* �쓮�֎����ώԗ֑� �ŏ� */
#define		WHEEL_AVE_MAX		(0x14D6)			/* �쓮�֎����ώԗ֑� �ő� */

#define		WHEEL_AVE_MIN_86B	(0x000D)			/* �쓮�֎����ώԗ֑� �ŏ� */
#define		WHEEL_AVE_MAX_86B	(0x14D5)			/* �쓮�֎����ώԗ֑� �ő� */

#define 	CAN_CH_TCAN							CAN_CH_0
#define 	VEHICLE_SPEED_RESOLUTION			100000
#ifdef _FOR_RCF
#define 	VSO_RESOLUTION						(0.01 * VEHICLE_SPEED_RESOLUTION)
#else	/* _FOR_RCF */
#define 	VSO_RESOLUTION						(0.05625 * VEHICLE_SPEED_RESOLUTION)
#endif	/* _FOR_RCF */
#define 	TIMEOUT_0D1							120
#define 	VEHICLE_SPEED_HOLD_TIME_86A				280
#define 	VEHICLE_SPEED_VALUE_COUNT_MAX_86A		14 /*14�񕪕ێ�x20ms = 280ms*/
#define 	VEHICLE_SPEED_HOLD_TIME_86B				300/*300ms�Ԋu�ōX�V*/
#define 	VEHICLE_SPEED_VALUE_COUNT_MAX_86B		10 /*10�񕪕ێ����ĕ��ω�*/
#define 	VEHICLE_SPEED_VALUE_COUNT_MAX			VEHICLE_SPEED_VALUE_COUNT_MAX_86A /*�ԑ��ێ����F86A,86B�ŕێ����̑傫������ݒ�*/
#define 	VEHICLE_BRAKE_PRESS_COUNT_MAX		6				/*�u���[�L���ێ���*/
#define 	THRESHOLD_SPEED_MAX					(300 * VEHICLE_SPEED_RESOLUTION)
#define 	VEHICLEINFO_NUM						2				/* �d���n���x�Z�o�e�[�u���Ԏ�z�� */
#define 	TBL_NUM								15				/* �d���n���x�Z�o�e�[�u���z�� */
#define 	DIRECTION_TBL_NUM					16
#define 	TIMEOUT_SUM_FC						1000			/* �R������ʐώZ�����^�C���A�E�g�l(ms) */

#define 	BRAKE_PRESS_SMOOTHINGCOEFFICIENT	(0.2)		/* �u���[�L���Ȃ܂��W��					*/
#define 	SMOOTHING_BRAKE_PRESS_RESOLUTION	10000		/* �Ȃ܂������u���[�L������\			*/
#define 	BRAKE_PRESS_RESOLUTION				(0.1305882 * SMOOTHING_BRAKE_PRESS_RESOLUTION)	/* �u���[�L������\	*/

#define 	BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX	10				/*�u�����h�t���O�ێ���*/
#define 	BRANDFLAG_RECEIVE_INTERVAL_86A			100				/*86A�u�����h�t���O��M�Ԋums*/
#define 	BRANDFLAG_RECEIVE_INTERVAL_86B			1500			/*86B�u�����h�t���O��M�Ԋums*/
#define 	VEHICLEINFO_PROC_INTERVAL				10000			/*�ԗ�������Ԋums*/

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/

/********************************************************/
/*	�萔�錾											*/
/********************************************************/

/********************************************************/
/*	�\���̐錾											*/
/********************************************************/
/* CAN���(UI_8�p) */
typedef struct t_can_info_ui8 {
	UI_8	data;				/* �f�[�^ */
	UI_8	first_flg;			/* �����M�t���O */
	UI_8	fail_flg;			/* �t�F�[���t���O */
} T_CanInfo_UI8;

/* CAN���(UI_16�p) */
typedef struct t_can_info_ui16 {
	UI_16	data;				/* �f�[�^ */
	UI_8	first_flg;			/* �����M�t���O */
	UI_8	fail_flg;			/* �t�F�[���t���O */
} T_CanInfo_UI16;

/*�����w���e�[�u���̌^*/
typedef struct{
	UI_32		speed;		/* �ԑ�(km/h) * 100000 */
	F_32		slope;		/* �X�� */
	SI_32		intercept;	/* �ؕ�(km/h) * 100000 */
}S_CENTER_TOLE_TBL;

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
/* �n���㑖�s���� */
TCAN_COMMON_CTRL_LOCAL	UI_32	OdoTrip;				/* �n���㑖�s����[0.01m] */
TCAN_COMMON_CTRL_LOCAL	UI_32	SpOdo;					/* �ԑ��p���X���ώZ�l */
TCAN_COMMON_CTRL_LOCAL	UI_8	Sp1p_old;				/* �ԑ��p���X�M���ώZ�l �O��l */

/* �R�����ːώZ�l */
TCAN_COMMON_CTRL_LOCAL	UI_32	Sum_Fc;					/* �R�����˗ʐώZ�l �����ԐώZ�l */
TCAN_COMMON_CTRL_LOCAL	UI_16	Sum_Fc_N;				/* �R�����˗ʐώZ�l �ώZ�� */
TCAN_COMMON_CTRL_LOCAL	UI_8	FuelProcCycleCnt;		/* �R�����˗ʐώZ�l �Z�o�����J�E���^ */
TCAN_COMMON_CTRL_LOCAL	UI_32	FC_Sum;					/* �R�����˗ʐώZ�l (�ŏI�m��l) */
TCAN_COMMON_CTRL_LOCAL UI_8  S_OdoTrip_enable;			/* OdoTrip�l�̗L���t���O				*/
TCAN_COMMON_CTRL_LOCAL UI_8  S_FC_Sum_enable;			/* Sum_Fc�l�̗L���t���O					*/

/* MT */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtDDoorSW;			/* �h�ASW D�� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtPDoorSW;			/* �h�ASW P�� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtTrunkDoorSW;		/* �h�ASW �g�����N */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtRatioAccel;		/* �A�N�Z���J�x�� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtEngRpm;			/* �G���W����]�� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtEngRpm_Ta;		/* �G���W����]��(�^�R���[�^�p) */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtGearEstm;			/* MT�M�A�|�W�V�����\����� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtSpWheelAve;		/* �쓮�֎����ώԗ֑�(VSO) */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtSpPulseInteg;		/* �ԑ��p���X�M���ώZ�l(SP1P) */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtBreakLampSW;		/* �u���[�L�����vSW */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtHandBreakSW;		/* �p�[�L���O�u���[�LSW */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtReverseSW;		/* ���o�[�XSW */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtHeadLampHi;		/* �w�b�h�����vHi */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtHeadLampLo;		/* �w�b�h�����vLo */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtHeadLampSmall;	/* �w�b�h�����vSmarll */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtOutTemp;			/* �\���p�O�C�� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtPSeatBeltSW;		/* �V�[�g�x���g�o�b�N��SW P�� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtDSeatBeltSW;		/* �V�[�g�x���g�o�b�N��SW D�� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtTempOil;			/* �G���W������ */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtTempCoolant;		/* �G���W������ */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtSetCc;			/* �N���R���Z�b�g�����v��� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtSteerAngle;		/* �Ǌp�Z���T */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtYawRate;			/* ���[���[�g */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtAccLR;			/* �������x */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtAccFR;			/* �O��G�Z���T */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtTrnsType;			/* �g�����X�~�b�V������� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtSportGearPos;		/* �X�|�[�c�V�t�g�M�A�ʒu */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtShiftPos;			/* �V�t�g�ʒu */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtModeShiftCtrl;	/* TCM�ϑ����䃂�[�h��� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtAtType;			/* AT��� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtPlace;			/* �d���n��� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtIgn;				/* IGN						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtAcc;				/* ACC						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtBrandFlag;		/* �u�����h�t���O			*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtTargetGearPos;	/* �ڕW�M�A�ʒu				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtSportsLamp;		/* �X�|�[�c�����v			*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtSnowHoldLamp;		/* �X�m�[�z�[���h�����v		*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtNodeID;			/* �m�[�hID					*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtSleepAck;			/* Sleep Ack				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtSleepInd;			/* Sleep Ind				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtLimpHome;			/* Limp Home				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtRing;				/* Ring						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtAlive;			/* Alive					*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtWkup;				/* Wkup						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtInit;				/* Init						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtSpecNo;			/* Spec. No.				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtSpdWheelFtlh;		/* ���O�֎ԗ֑�				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtSpdWheelFtrh;		/* �E�O�֎ԗ֑�				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtSpdWheelRrlh;		/* ����֎ԗ֑�				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	MtSpdWheelRrrh;		/* �E��֎ԗ֑�				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtDoorLockStatPass;	/* �h�A���b�N��ԁi����ȁj	*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtDoorLockStatDrvr;	/* �h�A���b�N��ԁi�^�]�ȁj	*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtBrakePressure;	/* �u���[�L��				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAirPressFtlh;		/* ���O�֋�C�� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAirPressFtrh;		/* �E�O�֋�C�� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAirPressRrlh;		/* ����֋�C�� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAirPressRrrh;		/* �E��֋�C�� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAirPressUnit;		/* ��C���P�� */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtManualModeSW;		/* �}�j���A�����[�hSW */
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtVscOnOff;			/* VSC ON/OFF���iVSC OFF�����v��ԁj 	*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtVscAct;			/* VSC�쓮���(VSC,TRC����)				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtTrcMode;			/* �g���N�V�����d�����[�h 				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtTrcOnOff;			/* TRC ON/OFF���iTRC OFF�����v��ԁj 	*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtTrcAct;			/* TRC�쓮��� 							*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAbsAct;			/* ABS�쓮��� 							*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtVscWarning;		/* VSC�E�H�[�j���O						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAbsWarning;		/* ABS�E�H�[�j���O 						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAbsFail;			/* ABS�V�X�e���t�F�C���t���O			*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtVscUnderStr;		/* VSC�A���_�[������					*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtVscOverStr;		/* VSC�I�[�o�[������					*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtEngTrqUpReqFlg;	/* �G���W���g���N�A�b�v�v���t���O		*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtEngTrqDwnReqFlg;	/* �G���W���g���N�����v���t���O			*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	AtEngTrqUpReqVal;	/* �G���W���g���N�A�b�v�v���l			*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	AtEngTrqDwnReqVal;	/* �G���W���g���N�����v���l				*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI16	AtEngTrqShaft;		/* ���G���W�����g���N					*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtEcbWarning;		/* ECB�E�H�[�j���O						*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtIntakePress;		/* �z���ǈ���							*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtAtmosphericPress;	/* ��C��								*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtBlinkerLeft;		/* �E�B���J�[��							*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	AtBlinkerRight;		/* �E�B���J�[�E							*/
TCAN_COMMON_CTRL_LOCAL	T_CanInfo_UI8	MtHoodDoorSW;		/* �h�ASW �t�[�h						*/

TCAN_COMMON_CTRL_LOCAL  UI_32	S_can_0d1_received_time;				/* 0D1���b�Z�[�W�ŏI��M����	*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_vehicle_speed_value_buf[VEHICLE_SPEED_VALUE_COUNT_MAX];	/* �ԑ��l�ێ��o�b�t�@	*/
TCAN_COMMON_CTRL_LOCAL  UI_8  	S_vehicle_speed_value_count;			/* �ԑ��l�ێ��o�b�t�@�f�[�^��		*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_speed_value_mem;						/* �ԑ��������ێ��l					*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_speed_value_dsp;						/* �ԑ��\���l						*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_speed_proc_data_time;					/* �\���p�ԑ��v�Z�����O�񏈗�����	*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_speed_value_updated_time;				/* �ԑ��\���l�O��X�V����			*/
TCAN_COMMON_CTRL_LOCAL  UI_8    S_speed_value_dsp_enable;				/* S_speed_value_dsp�l�̗L���t���O	*/

TCAN_COMMON_CTRL_LOCAL  UI_32 	S_brake_press_smooth;					/* �Ȃ܂��u���[�L���l							*/
TCAN_COMMON_CTRL_LOCAL  UI_8    S_brake_press_smooth_enable;			/* �Ȃ܂��u���[�L���l�̗L���t���O				*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_brake_press_proc_data_time;			/* �u���[�L���O�񏈗�����						*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_vehicle_brake_press_buf[VEHICLE_BRAKE_PRESS_COUNT_MAX];	/* �u���[�L���ێ��o�b�t�@	*/
TCAN_COMMON_CTRL_LOCAL  UI_8  	S_vehicle_brake_press_count;			/* �u���[�L���ێ��o�b�t�@�f�[�^��				*/

TCAN_COMMON_CTRL_LOCAL  UI_32 	S_vehicle_proc_time;								/*�ԗ���񔻒莞��					*/
TCAN_COMMON_CTRL_LOCAL E_TCAN_VEHICLE_INFO S_vehicleJudgeInfo;						/*�ԗ����:��������*/
TCAN_COMMON_CTRL_LOCAL E_TCAN_VEHICLE_INFO S_vehicleCanInfo;						/*�ԗ����:CAN��M���*/
/*
CAN��M���	�F��ʃ��W���[������w�肳���B���̏�Ԃɉ�����CANID����M����
			  ���Z�����͂��̏�Ԃɉ����Ď��{����BUnknown��Ԏ��͐M�����Z���Ȃ��B
			  �N�����ɂ͕K����ʂ���w�肳���B
��������	�FTCan���W���[�����Ŕ��肵����ԁB��ʂ���CAN��M��Ԃ��ݒ肳���Ɩ{��Ԃ����킹�ĕύX����B
			  ��������ł�86A/86B�݂̂�Unknown�ɂȂ邱�Ƃ͂Ȃ��B�i������Ԃ������j
			  ���������Ԃ��ω�����Ə�ʂ���CAN��M��Ԃ̐ݒ�(TCAN_Ctrl_SetupCanMessage)���s����B
----------------------------------------------------------
CAN��M���			��������				�M�����Z
(S_vehicleCanInfo�j	(S_vehicleJudgeInfo)	
------------------------------------------------------------
Unknown				Unknown					�Ԏ픻��̂�
					86A						�Ԏ픻��̂�
					86B						�Ԏ픻��̂�
------------------------------------------------------------
86A					-(�Ȃ��j				-
					86A						86A
					86B						86A
------------------------------------------------------------
86B					-(�Ȃ��j				-
					86A						86B
					86B						86B
------------------------------------------------------------*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_brandflag_receive_interval_86A[BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX];	/*86A�u�����h�t���O��M�Ԋu				*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_brandflag_receive_interval_86B[BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX];	/*86B�u�����h�t���O��M�Ԋu				*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_brandflag_receive_lasttime_86A;										/*86A�u�����h�t���O�O���M����			*/
TCAN_COMMON_CTRL_LOCAL  UI_32 	S_brandflag_receive_lasttime_86B;										/*86B�u�����h�t���O�O���M����			*/
TCAN_COMMON_CTRL_LOCAL  UI_8 	S_brandflag_receive_count_86A;											/*86A�u�����h�t���O��M��				*/
TCAN_COMMON_CTRL_LOCAL  UI_8 	S_brandflag_receive_count_86B;											/*86B�u�����h�t���O��M��				*/
TCAN_COMMON_CTRL_LOCAL  UI_16   S_speed_value_hold_time;							/* �\���p�ԑ��ێ�����	*/
TCAN_COMMON_CTRL_LOCAL  UI_8    S_speed_value_count_max;							/* �ԑ��ێ���			*/


/* �d���n�e�[�u���C���f�N�X*/
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

/* �d���n����Ώۂ̑��x�Z�o�e�[�u���C���f�N�X�ɕϊ�*/ 
TCAN_COMMON_CTRL_LOCAL const E_SP_DIRECTION C_Direction2SP[VEHICLEINFO_NUM][DIRECTION_TBL_NUM] = 
{
	{/*086A*/
		SP_DIRECTION_EC_86A,/* NO_DIRECTION (�s���JPN) */
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
	{/*086B 4�p�^�[���̂� JP,U4,C0,EK */
		SP_DIRECTION_EC_86B,/* NO_DIRECTION (�s���JPN) */
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



/* ���x�Z�o�e�[�u�� */
/*�@�č��K�i SP_DIRECTION_U4 */
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
	{/*086B MPH->km/h�ɕϊ����Ă��邽�ߎԑ���20km/h�P�ʂł͂Ȃ�*/
		{	1600000		,1.03125f 	, 0		},/* 10MPH��km/h�ɕϊ� */
		{	3200000		,1.00625f 	, 40000	},/* 20MPH��km/h�ɕϊ� */
		{	6400000		,1.003125f	, 50000	},/* 40MPH��km/h�ɕϊ� */
		{	9600000		,1.00625f	, 30000	},/* 60MPH��km/h�ɕϊ� */
		{	12800000	,1.003125f	, 60000	},/* 80MPH��km/h�ɕϊ� */
		{	16000000	,1.00625f 	, 20000	},/* 100MPH��km/h�ɕϊ� */
		{	19300000	,1.003030303f,71515	},/* 120MPH��km/h�ɕϊ� */
		{	22500000	,1.003125f 	, 69687	},/* 140MPH��km/h�ɕϊ� */
		{	25700000	,1.00625f 	, -625	},/* 160MPH��km/h�ɕϊ� */
		{	28900000	,1.003125f 	, 79687	},/* 180MPH��km/h�ɕϊ� */
		{	29600000	,1.003125f 	, 79687	},/* ����ȏ�͂Ȃ��̂�180MPH�̒l�����̂܂܎g�p(���𑵂��邽�ߎԑ���300km/h�͈͓̔��Őݒ�)*/
		{	29700000	,1.003125f 	, 79687	},/* ����ȏ�͂Ȃ��̂�180MPH�̒l�����̂܂܎g�p(���𑵂��邽�ߎԑ���300km/h�͈͓̔��Őݒ�)*/
		{	29800000	,1.003125f 	, 79687	},/* ����ȏ�͂Ȃ��̂�180MPH�̒l�����̂܂܎g�p(���𑵂��邽�ߎԑ���300km/h�͈͓̔��Őݒ�)*/
		{	29900000	,1.003125f 	, 79687	},/* ����ȏ�͂Ȃ��̂�180MPH�̒l�����̂܂܎g�p(���𑵂��邽�ߎԑ���300km/h�͈͓̔��Őݒ�)*/
		{	30000000	,1.003125f 	, 79687	},/* ����ȏ�͂Ȃ��̂�180MPH�̒l�����̂܂܎g�p(���𑵂��邽�ߎԑ���300km/h�͈͓̔��Őݒ�)*/
	}
};

/*�A�J�i�_�K�i SP_DIRECTION_C0*/
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
		{	30000000	,1.005f ,	30000	},/* ����ȏ�͂Ȃ��̂�280km/h�̒l�����̂܂܎g�p*/
	}
};

/*�B���B�K�i SP_DIRECTION_KA */
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
	{/*086B:86B�ł͂��̎d���͖����Ȃ̂�SP_DIRECTION_EC�Ɠ����l��ݒ�*/
		{	 2000000	,1.04f	,	 0		},/*SP_DIRECTION_EC�̒l��ݒ�*/
		{	 4000000	,1.02f	,40000		},/*SP_DIRECTION_EC�̒l��ݒ�*/
		{	 6000000	,1.02f	,40000		},/*SP_DIRECTION_EC�̒l��ݒ�*/
		{	 8000000	,1.02f	,40000		},/*SP_DIRECTION_EC�̒l��ݒ�*/
		{	10000000	,1.02f	,40000		},/*SP_DIRECTION_EC�̒l��ݒ�*/
		{	12000000	,1.02f	,40000		},/*SP_DIRECTION_EC�̒l��ݒ�*/
		{	14000000	,1.02f	,40000		},/*SP_DIRECTION_EC�̒l��ݒ�*/
		{	16000000	,1.02f	,40000		},/*SP_DIRECTION_EC�̒l��ݒ�*/
		{	18000000	,1.02f	,40000		},/*SP_DIRECTION_EC�̒l��ݒ�*/
		{	20000000	,1.02f	,40000		},/*SP_DIRECTION_EC�̒l��ݒ�*/
		{	22000000	,1.02f	,40000		},/*SP_DIRECTION_EC�̒l��ݒ�*/
		{	24000000	,1.02f	,40000		},/*SP_DIRECTION_EC�̒l��ݒ�*/
		{	26000000	,1.02f	,40000		},/*SP_DIRECTION_EC�̒l��ݒ�*/
		{	28000000	,1.02f	,40000		},/*SP_DIRECTION_EC�̒l��ݒ�*/
		{	30000000	,1.02f	,40000		},/*SP_DIRECTION_EC�̒l��ݒ�*/
	}
};

/*�CECE39�ikm/h�j�Ή��K�i�i���B�ȊO�jSP_DIRECTION_EC */
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
		{	28000000	,1.02f	,40000		},/* ����ȏ�͂Ȃ��̂�260km/h�̒l�����̂܂܎g�p*/
		{	30000000	,1.02f	,40000		},/* ����ȏ�͂Ȃ��̂�260km/h�̒l�����̂܂܎g�p*/
	}
};

/*�DECE39�iMPH�j�Ή��K�i�i���B�ȊO�jSP_DIRECTION_EK */
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
		{	1600000		,1.025f 	, 		0	},/* 10MPH��km/h�ɕϊ� */
		{	3200000		,1.0125f 	, 	20000	},/* 20MPH��km/h�ɕϊ� */
		{	6400000		,1.0125f	, 	20000	},/* 40MPH��km/h�ɕϊ� */
		{	9600000		,1.009375f	, 	40000	},/* 60MPH��km/h�ɕϊ� */
		{	12800000	,1.0125f	, 	10000	},/* 80MPH��km/h�ɕϊ� */
		{	16000000	,1.0125f	 ,	10000	},/* 100MPH��km/h�ɕϊ� */
		{	19300000	,1.012121212f,	16060	},/* 120MPH��km/h�ɕϊ� */
		{	22500000	,1.0125f 	, 	8750	},/* 140MPH��km/h�ɕϊ� */
		{	25700000	,1.009375f 	, 	79062	},/* 160MPH��km/h�ɕϊ� */
		{	28900000	,1.0125f 	, 	-1250	},/* 180MPH��km/h�ɕϊ� */
		{	29600000	,1.0125f 	, 	-1250	},/* ����ȏ�͂Ȃ��̂�180MPH�̒l�����̂܂܎g�p(���𑵂��邽�ߎԑ���300km/h�͈͓̔��Őݒ�)*/
		{	29700000	,1.0125f 	, 	-1250	},/* ����ȏ�͂Ȃ��̂�180MPH�̒l�����̂܂܎g�p(���𑵂��邽�ߎԑ���300km/h�͈͓̔��Őݒ�)*/
		{	29800000	,1.0125f 	, 	-1250	},/* ����ȏ�͂Ȃ��̂�180MPH�̒l�����̂܂܎g�p(���𑵂��邽�ߎԑ���300km/h�͈͓̔��Őݒ�)*/
		{	29900000	,1.0125f 	, 	-1250	},/* ����ȏ�͂Ȃ��̂�180MPH�̒l�����̂܂܎g�p(���𑵂��邽�ߎԑ���300km/h�͈͓̔��Őݒ�)*/
		{	30000000	,1.0125f 	, 	-1250	},/* ����ȏ�͂Ȃ��̂�180MPH�̒l�����̂܂܎g�p(���𑵂��邽�ߎԑ���300km/h�͈͓̔��Őݒ�)*/
	}
};

/* ���x�Z�o�e�[�u�����d���n�ƑΉ�������e�[�u�� */
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
/*	�����֐��v���g�^�C�v�錾							*/
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
 * T-CAN���C������������
 * 
 * author	NSCS
 * date		2013/12/--
 * note		�ԗ���ʂɂ���M�\ID�̐ݒ�A�ϐ��̏�����
 */
void TCAN_Ctrl_Init(void)
{
#ifdef TCAN_DEBUG
	OUTLOG(OutputLog( "[TCAN] TCAN_Ctrl_Init \n");)
#endif /*TCAN_DEBUG*/
	/* �ϐ������� */
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
	/*�Ԏ���𔻒肷�邽�߂̏��N���A*/
	s_clear_vehilceinfo();

}

/** 
 * T-CAN���C������
 * 
 * author	NSCS
 * date		2013/12/--
 * note		10ms�����ŃR�[�������B��ɐώZ�������s���B
 */
void TCAN_Ctrl_Main(void)
{
	if(S_vehicleCanInfo != TCAN_VEHICLE_INFO_UNKNOWN)
	{
		/*CAN��M��Ԃ��s���ȊO*/
		s_odo_proc();
		s_fuel_proc();
		s_speed_proc();			/* �\���p�ԑ��v�Z����	*/
		s_brake_proc();			/* �u���[�L���Ȃ܂�����	*/
	}	
	s_vehicleInfo_proc();	/* �ԗ���񔻒菈��	*/
}


/** 
 * ��M�f�[�^�ݒ菈��
 * 
 * author	NSCS
 * param	id		[in]	��MCAN-ID
 * param	data	[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2013/12/--
 * note		��M�f�[�^�̉�͏������s���B
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
		/*�s��*/
		s_setMessage_Unknown(id, len, data);
		break;
	}
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_SetupCanMessage								*/
/* �@�\�̐���	�F	�Ԏ���ɉ�������M���b�Z�[�W�̐ݒ�					*/
/* ����			�F	(I/ )vehicleInfo	�Ԏ���							*/
/* 				�F	(I/ )data	��MCAN�f�[�^�̐擪�A�h���X					*/
/* �߂�l		�F 	void													*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	20/11/--												*/
/* ���l			�F	�Ԏ���ɉ�����CANID����M����ݒ���s��				*/
/****************************************************************************/
void TCAN_Ctrl_SetupCanMessage(E_TCAN_VEHICLE_INFO info)
{
#ifdef TCAN_DEBUG
	OUTLOG(OutputLog( "[TCAN] SetupCanMessage (%04x)\n", info );)
#endif /*TCAN_DEBUG*/

	/* �Ԏ���X:���������� */
	S_vehicleJudgeInfo = info;

	/* �Ԏ���X:CAN��M��� */
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
		/*086B�p��CANID���ݒ肷��*/
		/*086B�u�����h�t���O*/
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 14, MT_MSG_ID_68C);

		/*�ԑ��ێ����Ԑݒ�*/
		S_speed_value_hold_time		=	VEHICLE_SPEED_HOLD_TIME_86A;
		/*�ԑ��ێ����ݒ�*/
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
		/*086A�p��CANID���ݒ肷��*/
		/*086A�u�����h�t���O*/
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN + 18, MT_MSG_ID_282);

		/*�ԑ��ێ����Ԑݒ�*/
		S_speed_value_hold_time		=	VEHICLE_SPEED_HOLD_TIME_86B;
		/*�ԑ��ێ����ݒ�*/
		S_speed_value_count_max		=	VEHICLE_SPEED_VALUE_COUNT_MAX_86B;
		break;
	default:
		/*�s��*/
		/*086A�u�����h�t���O*/
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  0, MT_MSG_ID_282);
		/*086B�u�����h�t���O*/
		Can_drv_change_message_buffer(CAN_CH_TCAN, CAN_MSG_ID_RX_MIN +  1, MT_MSG_ID_68C);

		/*�ԑ��ێ����Ԑݒ�*/
		S_speed_value_hold_time		=	0;
		/*�ԑ��ێ����ݒ�*/
		S_speed_value_count_max		=	0;
		break;
	}

}

/**
 * MT �h�ASW D�ȏ��擾
 * 
 * param	data		[out]		�h�ASW D��(0:�A1:��)
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �h�ASW P�ȏ��擾
 * 
 * param	data		[out]		�h�ASW P��(0:�A1:��)
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �h�ASW �g�����N���擾
 * 
 * param	data		[out]		�h�ASW �g�����N(0:�A1:��)
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �A�N�Z���J�x�����擾
 * 
 * param	data		[out]		�A�N�Z���J�x��(0�`100/255)[%]
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �G���W����]�����擾
 * 
 * param	data		[out]		�G���W����]��(0�`16383)[rpm]
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �G���W����]��(�^�R���[�^�p)���擾
 * 
 * param	data		[out]		�G���W����]��(�^�R���[�^�p)(0�`16383)[rpm]
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �M�A�|�W�V�����\����擾
 * 
 * param	data		[out]		�M�A�|�W�V�����\��
 * 									(0:AT or Uncertain, 1:1st, 2:2nd, 3:3rd, 4:4th, 5:5th, 6:6th, 7:NON)
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �쓮�֎����ώԗ֑����擾
 * 
 * param	data		[out]		�쓮�֎����ώԗ֑�(0�`300.0375)[0.05625km/h]
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �ԑ��p���X�M���ώZ�l���擾
 * 
 * param	data		[out]		�ԑ��p���X�M���ώZ�l(0�`63)
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT IGN���擾
 * 
 * param	data		[out]		IGN���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT ACC���擾
 * 
 * param	data		[out]		ACC���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �u���[�L�����v���擾
 * 
 * param	data		[out]		�u���[�L�����v���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �p�[�L���O�u���[�L���擾
 * 
 * param	data		[out]		�p�[�L���O�u���[�L���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT ���o�[�X���擾
 * 
 * param	data		[out]		���o�[�X���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �w�b�h�����vHi���擾
 * 
 * param	data		[out]		�w�b�h�����vHi���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �w�b�h�����vLo���擾
 * 
 * param	data		[out]		�w�b�h�����vLo���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �w�b�h�����vSmall���擾
 * 
 * param	data		[out]		�w�b�h�����vSmall���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT  �\���p�O�C�����擾
 * 
 * param	data		[out]		 �\���p�O�C�����
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �V�[�g�x���g�o�b�N�� P�ȏ��擾
 * 
 * param	data		[out]		�V�[�g�x���g�o�b�N�� P�ȏ��
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �V�[�g�x���g�o�b�N�� D�ȏ��擾
 * 
 * param	data		[out]		�V�[�g�x���g�o�b�N�� D�ȏ��
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �d���n���擾
 * 
 * param	data		[out]		�d���n���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �u�����h�t���O���擾
 * 
 * param	data		[out]		�u�����h�t���O���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �G���W���������擾
 * 
 * param	data		[out]		�G���W���������
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �G���W���������擾
 * 
 * param	data		[out]		�G���W���������
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �N���R���Z�b�g�����v���擾
 * 
 * param	data		[out]		�N���R���Z�b�g�����v���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �Ǌp�Z���T���擾
 * 
 * param	data		[out]		�Ǌp�Z���T���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT ���[���[�g���擾
 * 
 * param	data		[out]		���[���[�g���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �������x���擾
 * 
 * param	data		[out]		�������x���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �O��G�Z���T���擾
 * 
 * param	data		[out]		�O��G�Z���T���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �g�����X�~�b�V�������擾
 * 
 * param	data		[out]		�g�����X�~�b�V�������
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �X�|�[�c�V�t�g�M�A�ʒu�\�����擾
 *
 * param	data		[out]		�X�|�[�c�V�t�g�M�A�ʒu
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �V�t�g�ʒu���(�t�F�[���Z�[�t������)�擾
 *
 * param	data		[out]		�V�t�g�ʒu
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT TCM�ϑ����䃂�[�h���擾
 *
 * param	data		[out]		TCM�ϑ����䃂�[�h���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �ڕW�M�A�ʒu���擾
 * 
 * param	data		[out]		�ڕW�M�A�ʒu���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT AT��ގ擾												
 * 																
 * param	data		[out]		AT���						
 * return	���s����											
 * retval	TCAN_CTRL_OK			����l						
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �X�|�[�c�����v���擾
 * 
 * param	data		[out]		�X�|�[�c�����v���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �X�m�[�z�[���h�����v���擾
 * 
 * param	data		[out]		�X�m�[�z�[���h�����v���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �m�[�hID���擾
 * 
 * param	data		[out]		�m�[�hID���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT Sleep Ack���擾
 * 
 * param	data		[out]		Sleep Ack���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT Sleep Ind���擾
 * 
 * param	data		[out]		Sleep Ind���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT Limp Home���擾
 * 
 * param	data		[out]		Limp Home���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT Ring���擾
 * 
 * param	data		[out]		Ring���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT Alive���擾
 * 
 * param	data		[out]		Alive���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT Wkup���擾
 * 
 * param	data		[out]		Wkup���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT Init���擾
 * 
 * param	data		[out]		Init���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT Spec. No.���擾
 * 
 * param	data		[out]		Spec. No.���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT ���O�֎ԗ֑����擾
 * 
 * param	data		[out]		���O�֎ԗ֑����
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �E�O�֎ԗ֑����擾
 * 
 * param	data		[out]		�E�O�֎ԗ֑����
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT ����֎ԗ֑����擾
 * 
 * param	data		[out]		����֎ԗ֑����
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �E��֎ԗ֑����擾
 * 
 * param	data		[out]		�E��֎ԗ֑����
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �h�A���b�N��ԁi����ȁj���擾
 * 
 * param	data		[out]		�h�A���b�N��ԁi����ȁj���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �h�A���b�N��ԁi�^�]�ȁj���擾
 * 
 * param	data		[out]		�h�A���b�N��ԁi�^�]�ȁj���
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * MT �u���[�L������擾
 * 
 * param	data		[out]		�u���[�L������
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * �n���㑖�s�����擾 (HV/MT����)
 * 
 * param	data		[out]		�n���㑖�s����[0.01m]
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
 * author	NSCS
 * date		2013/12/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetOdo(UI_32 *data)
{
	if(S_OdoTrip_enable != 1) {
		/* �ԑ��l���L���łȂ����CAN����M */
		return TCAN_CTRL_NO_RECEIVE;
	}

	*data = OdoTrip;
	return TCAN_CTRL_OK;
}

/**
 * �R�����˗ʐώZ�l (HV/MT����)
 * 
 * param	data		[out]		�R�����˗ʐώZ�l[0.001ml]
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
 * author	NSCS
 * date		2013/12/--
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetFCSum(UI_32 *data)
{
	if(S_FC_Sum_enable != 1) {
		/* �R�����˗ʐώZ�l���L���łȂ����CAN����M */
		return TCAN_CTRL_NO_RECEIVE;
	}

	/* [MT] �P�ʊ��Z�Ȃ� */
	*data = FC_Sum;
	return TCAN_CTRL_OK;
}

/**
 * �\���p�ԑ��擾
 * 
 * param	data		[out]		�\���p�ԑ��擾[0.00001km/h]
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
 * author	NSCS
 * date		2016/09/08
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetVehicleSpeedForDisp(UI_32 *data)
{
	if(S_speed_value_dsp_enable != 1) {
		/* �ԑ��l���L���łȂ����CAN����M */
		return TCAN_CTRL_NO_RECEIVE;
	}

	*data = S_speed_value_dsp;
	return TCAN_CTRL_OK;
}

/**
 * �ԑ��擾
 * 
 * param	data		[out]		�ԑ��擾[0.00001km/h]
 * return	���s����
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
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
 * �u���[�L���i�Ȃ܂��l�j�擾
 * 
 * param	data		[out]		
 * return	���s����				�u���[�L���i�Ȃ܂��l�j�擾[0.1305882MPa]
 * retval	TCAN_CTRL_OK			����l
 * retval	TCAN_CTRL_ERROR			�ُ�l
 * retval	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�
 * author	NSCS
 * date		2016/09/08
 * note	
 */
E_TCAN_RESULT TCAN_Ctrl_GetSmoothingBrakePressure(UI_8 *data)
{
	if(S_brake_press_smooth_enable != 1) {
		/* �u���[�L���l���L���łȂ����CAN����M */
		return TCAN_CTRL_NO_RECEIVE;
	}

	/*����\���Z�O�̒l��Ԃ�*/
	*data = (UI_8)((F_64)S_brake_press_smooth / BRAKE_PRESS_RESOLUTION);
	return TCAN_CTRL_OK;
}


/** 
 * ��M�f�[�^�ݒ菈��(086A�p)
 * 
 * author	OFT
 * param	id		[in]	��MCAN-ID
 * param	data	[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note		��M�f�[�^�̉�͏������s���B
 */
TCAN_COMMON_CTRL_LOCAL void s_setMessage_086A(UI_32 id, const UI_8 len, const UI_8* data)
{

#ifdef TCAN_DEBUG
	OUTLOG(OutputLog( "[TCAN] s_setMessage_086A %04x\n", id );)
#endif /*TCAN_DEBUG*/

	switch (id) {
	case MT_MSG_ID_374:			/* �h�ASW */
		s_tccc_jdg_MT_MsgId374_state(len, data);
		break;
		
	case MT_MSG_ID_140:			/* �A�N�Z���J�x,�G���W����]�� */
		s_tccc_jdg_MT_MsgId140_state(len, data);
		break;
		
	case MT_MSG_ID_141:			/* �G���W����]��(TA) */
		s_tccc_jdg_MT_MsgId141_state(len, data);
		break;
		
	case MT_MSG_ID_144:			/* �g�����X�~�b�V������� */
		s_tccc_jdg_MT_MsgId144_state(len, data);
		break;

	case MT_MSG_ID_148:			/* �X�|�[�c�V�t�g�M�A�ʒu�� */
		s_tccc_jdg_MT_MsgId148_state(len, data);
		break;

	case MT_MSG_ID_360:			/* �R������� */
		s_tccc_jdg_MT_MsgId360_state(len, data);
		break;
		
	case MT_MSG_ID_361:			/* �M�A�|�W�V���� */
		s_tccc_jdg_MT_MsgId361_state(len, data);
		break;
		
	case MT_MSG_ID_368:			/* AT��� */
		s_tccc_jdg_MT_MsgId368_state(len, data);
		break;

	case MT_MSG_ID_0D1:			/* �쓮�֎����ώԗ֑� */
		s_tccc_jdg_MT_MsgId0D1_state(len, data);
		break;
		
	case MT_MSG_ID_0D3:			/* �ԑ��p���X�M���ώZ�l */
		s_tccc_jdg_MT_MsgId0D3_state(len, data);
		break;
		
	case MT_MSG_ID_0D4:			/* �S�֎ԗ֑� */
		s_tccc_jdg_MT_MsgId0D4_state(len, data);
		break;
		
	case MT_MSG_ID_152:			/* �u���[�L/�����v */
		s_tccc_jdg_MT_MsgId152_state(len, data);
		break;
		
	case MT_MSG_ID_282:			/* �o�b�N��SW */
		s_tccc_jdg_MT_MsgId282_state(len, data);
		break;
		
	case MT_MSG_ID_0D0:			/* �Ǌp�Z���T�� */
		s_tccc_jdg_MT_MsgId0D0_state(len, data);
		break;
		
	case MT_MSG_ID_375:			/* �h�A���b�N���	*/
		s_tccc_jdg_MT_MsgId375_state(len, data);
		break;
		
	case MT_MSG_ID_68C:			/*086B:�u�����h�t���O */
		s_tccc_jdg_MT_MsgId68C_state(len, data);
		break;

	default:
		break;
	}
}

/** 
 * ��M�f�[�^�ݒ菈��(086B�p)
 * 
 * author	OFT
 * param	id		[in]	��MCAN-ID
 * param	data	[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note		��M�f�[�^�̉�͏������s���B
 */
TCAN_COMMON_CTRL_LOCAL void s_setMessage_086B(UI_32 id, const UI_8 len, const UI_8* data)
{

#ifdef TCAN_DEBUG
	OUTLOG(OutputLog( "[TCAN] s_setMessage_086B %04x\n", id );)
#endif /*TCAN_DEBUG*/

	switch (id) {
		
	case MT_MSG_ID_040:			/* �A�N�Z���J�x,�G���W����]�� */
		s_tccc_jdg_MT_MsgId040_state(len, data);
		break;
		
	case MT_MSG_ID_146:			/* �G���W����]��(TA) */
		s_tccc_jdg_MT_MsgId146_state(len, data);
		break;
		
	case MT_MSG_ID_345:			/* �g�����X�~�b�V������� */
		s_tccc_jdg_MT_MsgId345_state(len, data);
		break;

	case MT_MSG_ID_048:			/* �X�|�[�c�V�t�g�M�A�ʒu�� */
		s_tccc_jdg_MT_MsgId048_state(len, data);
		break;

	case MT_MSG_ID_68C:			/* �u�����h�t���O */
		s_tccc_jdg_MT_MsgId68C_state(len, data);
		break;
		
	case MT_MSG_ID_241:			/* �M�A�|�W�V���� */
		s_tccc_jdg_MT_MsgId241_state(len, data);
		break;
		
	case MT_MSG_ID_34A:			/* �X�|�[�c�A�X�m�[�����v */
		s_tccc_jdg_MT_MsgId34A_state(len, data);
		break;

	case MT_MSG_ID_139:			/* �쓮�֎����ώԗ֑� */
		s_tccc_jdg_MT_MsgId139_state(len, data);
		break;
		
	case MT_MSG_ID_13C:			/* �ԑ��p���X�M���ώZ�l */
		s_tccc_jdg_MT_MsgId13C_state(len, data);
		break;
		
	case MT_MSG_ID_13A:			/* �S�֎ԗ֑� */
		s_tccc_jdg_MT_MsgId13A_state(len, data);
		break;
		
	case MT_MSG_ID_3AC:			/* �u���[�L/�����v */
		s_tccc_jdg_MT_MsgId3AC_state(len, data);
		break;
		
	case MT_MSG_ID_390:			/* �o�b�N��SW */
		s_tccc_jdg_MT_MsgId390_state(len, data);
		break;
		
	case MT_MSG_ID_138:			/* �Ǌp�Z���T�� */
		s_tccc_jdg_MT_MsgId138_state(len, data);
		break;
		
	case MT_MSG_ID_13B:			/* �����x	*/
		s_tccc_jdg_MT_MsgId13B_state(len, data);
		break;
		
	case MT_MSG_ID_652:			/* �h�A���b�N���	*/
		s_tccc_jdg_MT_MsgId652_state(len, data);
		break;

	case MT_MSG_ID_6E2:			/* �^�C����C��	*/
		s_tccc_jdg_MT_MsgId6E2_state(len, data);
		break;

	case MT_MSG_ID_328:			/* VSC�M��	*/
		s_tccc_jdg_MT_MsgId328_state(len, data);
		break;

	case MT_MSG_ID_041:			/* ���G���W�����g���N	*/
		s_tccc_jdg_MT_MsgId041_state(len, data);
		break;

	case MT_MSG_ID_04B:			/*�ڕW�M�A�i��*/
		s_tccc_jdg_MT_MsgId04B_state(len, data);
		break;
	case MT_MSG_ID_282:			/* 086A:�u�����h�t���O */
		s_tccc_jdg_MT_MsgId282_state(len, data);
		break;

	default:
		break;
	}
}
/** 
 * ��M�f�[�^�ݒ菈��(����p)
 * 
 * author	OFT
 * param	id		[in]	��MCAN-ID
 * param	data	[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note		��M�f�[�^�̉�͏������s���B
 */
TCAN_COMMON_CTRL_LOCAL void s_setMessage_Unknown(UI_32 id, const UI_8 len, const UI_8* data)
{

	switch (id) {

	case MT_MSG_ID_282:			/* 086A:�u�����h�t���O */
		s_tccc_jdg_MT_MsgId282_state(len, data);
		break;

	case MT_MSG_ID_68C:			/*086B:�u�����h�t���O */
		s_tccc_jdg_MT_MsgId68C_state(len, data);
		break;
	default:
		break;
	}

}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetVehicleInfo								*/
/* �@�\�̐���	�F	�Ԏ���擾											*/
/* ����			�F	( /)													*/
/* �߂�l		�F	E_VEHICLE_TYPE											*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	20/09/-													*/
/* ���l			�F	�������肵����Ԃ�Ԃ�									*/
/****************************************************************************/
E_TCAN_VEHICLE_INFO TCAN_Ctrl_GetVehicleInfo(void)
{
	/*���������Ԃ�Ԃ�*/
	return S_vehicleJudgeInfo;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtAirPressFtlh								*/
/* �@�\�̐���	�F	���O�֋�C�����擾									*/
/* ����			�F	( /O)data	���O�֋�C�����							*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	19/07/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressFtlh(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&AtAirPressFtlh);
	*data = AtAirPressFtlh.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtAirPressFtrh								*/
/* �@�\�̐���	�F	�E�O�֋�C�����擾									*/
/* ����			�F	( /O)data	�E�O�֋�C�����							*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	19/07/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressFtrh(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&AtAirPressFtrh);
	*data = AtAirPressFtrh.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtAirPressRrlh								*/
/* �@�\�̐���	�F	����֋�C�����擾									*/
/* ����			�F	( /O)data	����֋�C�����							*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	19/07/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressRrlh(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&AtAirPressRrlh);
	*data = AtAirPressRrlh.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtAirPressRrrh								*/
/* �@�\�̐���	�F	�E��֋�C�����擾									*/
/* ����			�F	( /O)data	�E��֋�C�����							*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	19/07/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressRrrh(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&AtAirPressRrrh);
	*data = AtAirPressRrrh.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtAirPressUnit								*/
/* �@�\�̐���	�F	��C���P�ʏ��擾										*/
/* ����			�F	( /O)data	��C���P�ʏ��								*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	19/07/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressUnit(UI_8 *data)
{
	E_TCAN_RESULT	result;
	
	result = s_tccc_GetValStateUI8(&AtAirPressUnit);
	*data = AtAirPressUnit.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtVscOnOff									*/
/* �@�\�̐���	�F	VSC ON/OFF���擾										*/
/* ����			�F	( /O)data	VSC ON/OFF���								*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/02/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscOnOff(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtVscOnOff);
	*data = AtVscOnOff.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtVscAct									*/
/* �@�\�̐���	�F	VSC/TCS�쓮��ԏ��擾									*/
/* ����			�F	( /O)data	VSC/TCS�쓮��ԏ��							*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/02/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscAct(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtVscAct);
	*data = AtVscAct.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtTrcMode									*/
/* �@�\�̐���	�F	�g���N�V�����d�����[�h���擾							*/
/* ����			�F	( /O)data	�g���N�V�����d�����[�h���					*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/02/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtTrcMode(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtTrcMode);
	*data = AtTrcMode.data;

	return result;
}


/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtIntakePress								*/
/* �@�\�̐���	�F	ECB�E�H�[�j���O���擾									*/
/* ����			�F	( /O)data	ECB�E�H�[�j���O���							*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtEcbWarning(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtEcbWarning);
	*data = AtEcbWarning.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtIntakePress								*/
/* �@�\�̐���	�F	�z���ǈ��͏��擾										*/
/* ����			�F	( /O)data	�z���ǈ��͏��								*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtIntakePress(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtIntakePress);
	*data = AtIntakePress.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtAtmosphericPress							*/
/* �@�\�̐���	�F	��C�����擾											*/
/* ����			�F	( /O)data	��C�����									*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAtmosphericPress(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtAtmosphericPress);
	*data = AtAtmosphericPress.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtBlinkerLeft								*/
/* �@�\�̐���	�F	�E�B���J�[�����擾									*/
/* ����			�F	( /O)data	�E�B���J�[�����							*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtBlinkerLeft(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtBlinkerLeft);
	*data = AtBlinkerLeft.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtBlinkerRight								*/
/* �@�\�̐���	�F	�E�B���J�[�E���擾									*/
/* ����			�F	( /O)data	�E�B���J�[�E���							*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtBlinkerRight(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtBlinkerRight);
	*data = AtBlinkerRight.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetMtHoodDoorSW								*/
/* �@�\�̐���	�F	�h�ASW �t�[�h���擾									*/
/* ����			�F	( /O)data	�h�ASW �t�[�h���							*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetMtHoodDoorSW(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&MtHoodDoorSW);
	*data = MtHoodDoorSW.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtAbsAct									*/
/* �@�\�̐���	�F	ABS�쓮��ԏ��擾										*/
/* ����			�F	( /O)data	ABS�쓮��ԏ��								*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/02/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAbsAct(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtAbsAct);
	*data = AtAbsAct.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtVscWarning								*/
/* �@�\�̐���	�F	VSC�E�H�[�j���O���擾									*/
/* ����			�F	( /O)data	VSC�E�H�[�j���O���							*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscWarning(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtVscWarning);
	*data = AtVscWarning.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtAbsWarning								*/
/* �@�\�̐���	�F	ABS�E�H�[�j���O���擾									*/
/* ����			�F	( /O)data	ABS�E�H�[�j���O���							*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAbsWarning(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtAbsWarning);
	*data = AtAbsWarning.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtAbsFail									*/
/* �@�\�̐���	�F	ABS�V�X�e���t�F�C�����擾								*/
/* ����			�F	( /O)data	ABS�V�X�e���t�F�C��							*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtAbsFail(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtAbsFail);
	*data = AtAbsFail.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtVscUnderStr								*/
/* �@�\�̐���	�F	VSC�A���_�[�����ԏ��擾								*/
/* ����			�F	( /O)data	VSC�A���_�[�����ԏ��						*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscUnderStr(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtVscUnderStr);
	*data = AtVscUnderStr.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtVscOverStr								*/
/* �@�\�̐���	�F	VSC�I�[�o�[�����ԏ��擾								*/
/* ����			�F	( /O)data	VSC�I�[�o�[�����ԏ��						*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscOverStr(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtVscOverStr);
	*data = AtVscOverStr.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtEngTrqUpReqFlg							*/
/* �@�\�̐���	�F	�G���W���g���N�A�b�v�v���t���O���擾					*/
/* ����			�F	( /O)data	�G���W���g���N�A�b�v�v���t���O���			*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqUpReqFlg(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtEngTrqUpReqFlg);
	*data = AtEngTrqUpReqFlg.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtEngTrqDwnReqFlg							*/
/* �@�\�̐���	�F	�G���W���g���N�_�E���v���t���O���擾					*/
/* ����			�F	( /O)data	�G���W���g���N�_�E���v���t���O���			*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqDwnReqFlg(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtEngTrqDwnReqFlg);
	*data = AtEngTrqDwnReqFlg.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtEngTrqUpReqVal							*/
/* �@�\�̐���	�F	�G���W���g���N�A�b�v�v���l���擾						*/
/* ����			�F	( /O)data	�G���W���g���N�A�b�v�v���l���				*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqUpReqVal(UI_16 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI16(&AtEngTrqUpReqVal);
	*data = AtEngTrqUpReqVal.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtEngTrqDwnReqVal							*/
/* �@�\�̐���	�F	�G���W���g���N�_�E���v���l���擾						*/
/* ����			�F	( /O)data	�G���W���g���N�_�E���v���l���				*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqDwnReqVal(UI_16 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI16(&AtEngTrqDwnReqVal);
	*data = AtEngTrqDwnReqVal.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtEngTrqShaft								*/
/* �@�\�̐���	�F	���G���W�����g���N���擾								*/
/* ����			�F	( /O)data	���G���W�����g���N���						*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqShaft(UI_16 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI16(&AtEngTrqShaft);
	*data = AtEngTrqShaft.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtTrcOnOff									*/
/* �@�\�̐���	�F	TRC ON/OFF���擾										*/
/* ����			�F	( /O)data	VSC ON/OFF���								*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtTrcOnOff(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtTrcOnOff);
	*data = AtTrcOnOff.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtTrcAct									*/
/* �@�\�̐���	�F	TRC�쓮��ԏ��擾										*/
/* ����			�F	( /O)data	TRC�쓮��ԏ��								*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/03/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtTrcAct(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtTrcAct);
	*data = AtTrcAct.data;

	return result;
}
/****************************************************************************/
/* �֐�����		�F	TCAN_Ctrl_GetAtManualModeSW								*/
/* �@�\�̐���	�F	�}�j���A�����[�hSW���擾								*/
/* ����			�F	( /O)data	�}�j���A�����[�hSW���						*/
/* �߂�l		�F	TCAN_CTRL_OK			����l							*/
/*				�F	TCAN_CTRL_ERROR			�ُ�l							*/
/*				�F	TCAN_CTRL_NO_RECEIVE	�N����A��x����M�f�[�^�Ȃ�	*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	21/01/-													*/
/* ���l			�F															*/
/****************************************************************************/
E_TCAN_RESULT TCAN_Ctrl_GetAtManualModeSW(UI_8 *data)
{
	E_TCAN_RESULT	result;

	result = s_tccc_GetValStateUI8(&AtManualModeSW);
	*data = AtManualModeSW.data;

	return result;
}

/****************************************************************************/
/* �֐�����		�F	s_vehicleInfo_proc										*/
/* �@�\�̐���	�F	�ԗ���񔻒菈��										*/
/* ����			�F	( / )													*/
/* �߂�l		�F	-														*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	20/11/-													*/
/* ���l			�F															*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL void s_vehicleInfo_proc(void)
{
	/**	VEHICLEINFO_PROC_INTERVAL�Ԋu�Ŏԗ���񔻂��s��*/
	if (Timer_ctrl_TimeCompare1ms(S_vehicle_proc_time, VEHICLEINFO_PROC_INTERVAL) == N_OK)
	{
		/*	���Ԍo��	*/
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
			/*�s��*/
			s_vehicleInfo_proc_Unknown();
			break;
		}
	
#ifdef TCAN_DEBUG
		if (info != S_vehicleJudgeInfo ){
			/*�ω�����*/
			OUTLOG(OutputLog( "[TCAN] !!!!!!!!!! VehileInfo Change (%04x -> (%04x) !!!!!!!!!!!\n", info,S_vehicleJudgeInfo );)
		}
#endif /*TCAN_DEBUG*/

		/*	�������ԕێ�	*/
		S_vehicle_proc_time = Timer_ctrl_GetTime1ms();
	}
	
	
}

/****************************************************************************/
/* �֐�����		�F	s_vehicleInfo_proc_86A									*/
/* �@�\�̐���	�F	�ԗ���񔻒菈���i����86A�j								*/
/* ����			�F	( / )													*/
/* �߂�l		�F	-														*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	20/11/-													*/
/* ���l			�F															*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL void s_vehicleInfo_proc_86A(void)
{
	/*�Ԏ��� 86A�Ȃ̂�86A�����Ƀ`�F�b�N*/
	if (is_can_enalbe_86A() == N_TRUE) {
		/*�Ԏ���𔻒肷�邽�߂̏��N���A*/
		s_clear_vehilceinfo();
		/*�Ԏ����86A*/
		/*���ł�86A�Ȃ̂ł��̂܂ܔ�����*/
		return;
	}
	if (is_can_enalbe_86B() == N_TRUE) {
		/*�Ԏ���𔻒肷�邽�߂̏��N���A*/
		s_clear_vehilceinfo();
		/*�Ԏ����86B*/
		S_vehicleJudgeInfo = TCAN_VEHICLE_INFO_86B;
	}
}


/****************************************************************************/
/* �֐�����		�F	s_vehicleInfo_proc_86B									*/
/* �@�\�̐���	�F	�ԗ���񔻒菈���i����86B�j								*/
/* ����			�F	( / )													*/
/* �߂�l		�F	-														*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	20/11/-													*/
/* ���l			�F															*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL void s_vehicleInfo_proc_86B(void)
{
	/*�Ԏ��� 86B�Ȃ̂�86B�����Ƀ`�F�b�N*/
	if (is_can_enalbe_86B() == N_TRUE) {
		/*�Ԏ���𔻒肷�邽�߂̏��N���A*/
		s_clear_vehilceinfo();
		/*�Ԏ����86B*/
		/*���ł�86B�Ȃ̂ł��̂܂ܔ�����*/
		return;
	}

	if (is_can_enalbe_86A() == N_TRUE) {
		/*�Ԏ���𔻒肷�邽�߂̏��N���A*/
		s_clear_vehilceinfo();
		/*�Ԏ����86A*/
		S_vehicleJudgeInfo = TCAN_VEHICLE_INFO_86A;
	}
}

/****************************************************************************/
/* �֐�����		�F	s_vehicleInfo_proc_Unknown								*/
/* �@�\�̐���	�F	�ԗ���񔻒菈���i����Unknown�j							*/
/* ����			�F	( / )													*/
/* �߂�l		�F	-														*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	20/11/-													*/
/* ���l			�F	�ǂ���̎Ԏ���m�F����									*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL void s_vehicleInfo_proc_Unknown(void)
{
	/*�Ԏ��� �s���Ȃ̂�86A��86B�����`�F�b�N*/
	/*������2�����ɗL���ɂȂ邱�Ƃ͂Ȃ�*/
	if (is_can_enalbe_86A() == N_TRUE) {
		/*�Ԏ���𔻒肷�邽�߂̏��N���A*/
		s_clear_vehilceinfo();
		/*�Ԏ����86A*/
		S_vehicleJudgeInfo = TCAN_VEHICLE_INFO_86A;
		/*������2�����ɗL���ɂȂ邱�Ƃ͂Ȃ��������ł���*/
		return;
	}

	if (is_can_enalbe_86B() == N_TRUE) {
		/*�Ԏ���𔻒肷�邽�߂̏��N���A*/
		s_clear_vehilceinfo();
		/*�Ԏ����86B*/
		S_vehicleJudgeInfo = TCAN_VEHICLE_INFO_86B;
	}
}


/****************************************************************************/
/* �֐�����		�F	is_can_enalbe_86A										*/
/* �@�\�̐���	�F	86A CAN�M���L������										*/
/* ����			�F	( / )													*/
/* �߂�l		�F	N_TRUE:�L��/N_FALSE:����								*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	20/11/-													*/
/* ���l			�F															*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL B_8 is_can_enalbe_86A(void)
{
	B_8	ret = N_FALSE;
	UI_32 brad_ave;
	
	if (S_brandflag_receive_count_86A >= BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX) {
		/*86A�p�u�����h�t���O�͎�M��*/

		/*86A�p�u�����h�t���O�̕��ώ�M�Ԋu�擾*/
		brad_ave = s_get_receive_interval_average(S_brandflag_receive_interval_86A,S_brandflag_receive_count_86A);
		if ( brad_ave <= BRANDFLAG_RECEIVE_INTERVAL_86A ) {
			/*���ώ�M�Ԋu���w��l�ȉ��ƂȂ��Ă���*/
			/*�Ԏ����86A*/
			ret = N_TRUE;
		}
	}
#ifdef TCAN_DEBUG
	OUTLOG(OutputLog( "[TCAN] 86A Check count (%d ) ave ( %d ) result %s \n", S_brandflag_receive_count_86A,brand_ave , (ret ? "true" : "false")); )
#endif /*TCAN_DEBUG*/
	
	return ret;
}


/****************************************************************************/
/* �֐�����		�F	is_can_enalbe_86B										*/
/* �@�\�̐���	�F	86B CAN�M���L������										*/
/* ����			�F	( / )													*/
/* �߂�l		�F	N_TRUE:�L��/N_FALSE:����								*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	20/11/-													*/
/* ���l			�F															*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL B_8 is_can_enalbe_86B(void)
{
	B_8	ret = N_FALSE;
	UI_32 brad_ave;
	
	if (S_brandflag_receive_count_86B >= BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX) {
		/*86B�p�u�����h�t���O�͎�M��*/

		/*86B�p�u�����h�t���O�̕��ώ�M�Ԋu�擾*/
		brad_ave = s_get_receive_interval_average(S_brandflag_receive_interval_86B,S_brandflag_receive_count_86B);
		if ( brad_ave <= BRANDFLAG_RECEIVE_INTERVAL_86B ) {
			/*���ώ�M�Ԋu���w��l�ȉ��ƂȂ��Ă���*/
			/*�Ԏ����86B*/
			ret = N_TRUE;
		}
	}
#ifdef TCAN_DEBUG
	OUTLOG(OutputLog( "[TCAN] 86B Check count (%d ) ave ( %d ) result %s \n", S_brandflag_receive_count_86B,brand_ave , (ret ? "true" : "false")); )
#endif /*TCAN_DEBUG*/
		
	return ret;
}


/****************************************************************************/
/* �֐�����		�F	s_clear_vehilceinfo_proc_86B							*/
/* �@�\�̐���	�F	�ԗ����N���A											*/
/* ����			�F	( / )													*/
/* �߂�l		�F	-														*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	20/11/-													*/
/* ���l			�F	�ԗ����𔻒�ʂ��邽�߂̏����N���A����				*/
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
/* �֐�����		�F	get_receive_interval_average							*/
/* �@�\�̐���	�F	��M�Ԋu�̕��ϒl�擾									*/
/* ����			�F	(I/ )data	�f�[�^��M�Ԋu								*/
/* ����			�F	(I/ )count	�f�[�^��									*/
/* �߂�l		�F	��M�Ԋu�̕��ϒl�擾									*/
/* �쐬��		�F	(OFT)Sakata												*/
/* �쐬��		�F	20/11/-													*/
/* ���l			�F															*/
/****************************************************************************/
TCAN_COMMON_CTRL_LOCAL UI_32 s_get_receive_interval_average(const UI_32* data, UI_8 count)
{
	UI_8  i;
	UI_32 sum_value = 0;
	UI_32 ave_value;
	
	if(count == 0) {
		return(0);
	}

	/* �o�b�t�@���̑S�ԑ��l�����Z */
	for(i = 0; i < count; i++) {
		sum_value += data[i];
	}
	
	/* ���ϒl�Z�o */
	ave_value = sum_value / count;
	
	return ave_value;
}


/** 
 * �ԑ��ώZ����
 * 
 * author	NSCS
 * date		2013/12/--
 * note		�n���㑖�s�����̉��Z���s���BCAN�M���̎ԑ��p���X�M���ώZ�l�̍�����ώZ���邽�߁ACAN��M�������ł͂Ȃ�����������s���Ė��Ȃ��B
 *			SP1�̃t�F�[�����͐ώZ�������s��Ȃ��ASP1P�̃t�F�[�����͏������p�����Ă����Ȃ��B
 *
 */
TCAN_COMMON_CTRL_LOCAL void s_odo_proc(void)
{
	UI_16 sp1;					/* �ԑ�[km/h] */
	UI_8 sp1_fail;				/* �ԑ��ُ� */
	UI_8 sp1_first;				/* �ԑ� ��M�t���O */
	UI_8 sp1p;					/* �ԑ��p���X�M���ώZ�l */
	UI_8 sp1p_first;			/* �ԑ��p���X�M���ώZ�l ��M�t���O */
	UI_16 sp_run_thresh;		/* 1.75[km/h]������CAN�M��臒l */
	
	sp1 = MtSpWheelAve.data;
	sp1_fail = MtSpWheelAve.fail_flg;
	sp1_first = MtSpWheelAve.first_flg;
	sp1p = MtSpPulseInteg.data;
	sp1p_first = MtSpPulseInteg.first_flg;
	sp_run_thresh = SP_RUN_TH_MT;

	if ((sp1 > sp_run_thresh) && (sp1_fail == 0))  {
		/* "�ԑ�:1.75km/h�ȏ�" ���� "�ԑ��ُ�Ȃ�" */
		
		/* �ԑ��p���X���ώZ�l */
		if (sp1p >= Sp1p_old) {
			SpOdo = SpOdo + ((UI_32)sp1p - (UI_32)Sp1p_old);	/* �I�[�o�[�t���[���Z�\�iSPODO���ő�l�i0xFFFFFFFF�j�𒴂���ꍇ�́ASPODO����ő�l�i0xFFFFFFFF�j�����Z����j */
		} else {
			SpOdo = SpOdo + (64 - Sp1p_old) + sp1p;		/* �I�[�o�[�t���[���Z�\�iSPODO���ő�l�i0xFFFFFFFF�j�𒴂���ꍇ�́ASPODO����ő�l�i0xFFFFFFFF�j�����Z����j */
		}
		
		/* �n���㑖�s���� */
		if (SpOdo <= SP_MAX_TH) {
			/* ODO_TRIP = SPODO �~ 10^5 / (2 �~ 637 �~ NP) */
			OdoTrip = (UI_32)((F_64)SpOdo * COEF);		/* ��搔��32bit�̂��ߕ��������_���Z���g�킴��𓾂Ȃ��A�������x���Ȃ����v�m�F�� */
		} else {
			OdoTrip = UI32_MAX;
		}
		/* OdoTrip�l�L���t���O�I�� */
		S_OdoTrip_enable = 1;
	}
	else if ((sp1 <= sp_run_thresh) && (sp1_fail == 0) && (sp1_first == 1) && (sp1p_first == 1))  {
		/*��~���Ŏԑ��A�ԑ��p���X��M�ς݁i����N�����Ɏԑ��A�ԑ��p���X����M�ς݂Ȃ�Odo�������l�i0�j�ŗL���Ƃ����邽�߁j*/
		/* OdoTrip�l�L���t���O�I�� */
		S_OdoTrip_enable = 1;
	}
	else {
		/* nop */
	}
		
	Sp1p_old = sp1p;		/* �O��l�ێ� */
}

/** 
 * �R�ĕ��˗ʐώZ����
 * 
 * author	NSCS
 * date		2013/12/--
 * note		�R�����˗ʐώZ�l�̉��Z���s��
 */
TCAN_COMMON_CTRL_LOCAL void s_fuel_proc(void)
{
	UI_32	dlt_fc;
	UI_32	sfc;

	/* [MT] ��M�C�x���g�ɂ�SUM_FC���v�Z���A500ms������FCSUM���v�Z����(N�͗L����M���Ƃ���) */
	FuelProcCycleCnt++;
	if (FuelProcCycleCnt >= FUEL_CYCLE) {
		/* DLC_FC[ml/sec] = SUM_FC[�~10 cc/sec] �� N */
		if (Sum_Fc_N != 0) {
			dlt_fc = Sum_Fc / Sum_Fc_N;
			
			/* SFC[ml] = DLT_FC[ml/sec] �~ T �� 1000 */
			sfc = dlt_fc / 2;
			
			/* FCSUM[ml] = FCSUM[ml] + SFC[ml] */
			FC_Sum += sfc;
			S_FC_Sum_enable = 1;
		}
		
		/* ����v�Z�p */
		Sum_Fc = 0;
		Sum_Fc_N = 0;
		FuelProcCycleCnt = 0;
	}
}

/**
 * MT CAN ID(374) �h�ASW D�ȏ��̏�Ԕ���
 * 
 * author	NSCS
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId374_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/*�h�ASW */
	MtDDoorSW.data = data[3] & 0x01;
	MtDDoorSW.first_flg = 1;
	MtDDoorSW.fail_flg = 0;
	
	/*�h�ASW P�� */
	MtPDoorSW.data = (data[3] >> 1) & 0x01;
	MtPDoorSW.first_flg = 1;
	MtPDoorSW.fail_flg = 0;
	
	/*�h�ASW �g�����N */
	MtTrunkDoorSW.data = (data[3] >> 5) & 0x01;
	MtTrunkDoorSW.first_flg = 1;
	MtTrunkDoorSW.fail_flg = 0;
}

/**
 * MT CAN ID(140) �A�N�Z���J�x���A�G���W����]���̏�Ԕ���
 * 
 * author	NSCS
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId140_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �A�N�Z���J�x�� */
	MtRatioAccel.data = data[0];
	MtRatioAccel.first_flg = 1;
	MtRatioAccel.fail_flg = 0;
	
	/* �G���W����]�� */
	MtEngRpm.data = data[2] + ((UI_16)(data[3] & 0x3F) << 8);
	MtEngRpm.first_flg = 1;
	MtEngRpm.fail_flg = 0;
}

/**
 * MT CAN ID(141) �G���W����]��(�^�R)�̏�Ԕ���
 * 
 * author	NSCS
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId141_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �G���W����]��(�^�R) */
	MtEngRpm_Ta.data = data[4] + ((UI_16)(data[5] & 0x3F) << 8);
	MtEngRpm_Ta.first_flg = 1;
	MtEngRpm_Ta.fail_flg = 0;


}

/*															
 * MT CAN ID(144) �g�����X�~�b�V�������̏�Ԕ���			
 * 															
 * author	NSCS											
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X		
 * date		2014/10/--										
 * note														
 */															
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId144_state(const UI_8 len, const UI_8* data)	
{															

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �g�����X�~�b�V������� */
	MtTrnsType.data = (data[6] >> 7);
	MtTrnsType.first_flg = 1;								
	MtTrnsType.fail_flg = 0;								
}															

/*															
 * MT CAN ID(148) �X�|�[�c�V�t�g�M�A�ʒu���̏�Ԕ���			
 * 															
 * author	NSCS											
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X		
 * date		2014/10/--										
 * note														
 */															
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId148_state(const UI_8 len, const UI_8* data)	
{															

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �X�|�[�c�V�t�g�M�A�ʒu */
	MtSportGearPos.data = (data[0] >> 4);
	MtSportGearPos.first_flg = 1;							
	MtSportGearPos.fail_flg = 0;							
															
	/* �V�t�g�ʒu */
	MtShiftPos.data = data[0] & 0x0F;						
	MtShiftPos.first_flg = 1;								
	MtShiftPos.fail_flg = 0;								
															
	/* TCM�ϑ����䃂�[�h */
	MtModeShiftCtrl.data = (data[1] >> 4) & 0x07;			
	MtModeShiftCtrl.first_flg = 1;							
	MtModeShiftCtrl.fail_flg = 0;							

	/* �ڕW�M�A�i�� */
	MtTargetGearPos.data = (data[6] & 0x0f);
	MtTargetGearPos.first_flg = 1;
	MtTargetGearPos.fail_flg = 0;
}

/*
 * MT CAN ID(360) �R�����ʂ̏�Ԕ���
 * 
 * author	NSCS
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId360_state(const UI_8 len, const UI_8* data)
{
	UI_16	fuel;
	
	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �R������[0.001cc/sec] */
	fuel = data[0] + ((UI_16)data[1] << 8);
	
	/* SUM_FC�̉��Z */
	/* ��M�C�x���g�ɂ�SUM_FC���v�Z���A500ms������FCSUM���v�Z����(N�͗L����M���Ƃ���) */
	Sum_Fc += fuel;
	Sum_Fc_N++;

	/* �G���W������ */
	MtTempOil.data = data[2];	
	MtTempOil.first_flg = 1;
	MtTempOil.fail_flg  = 0;

	/* �G���W������ */
	MtTempCoolant.data = data[3];
	MtTempCoolant.first_flg = 1;
	MtTempCoolant.fail_flg  = 0;
	
	if(MtTempCoolant.data == 0xFF)
	{
		MtTempCoolant.fail_flg = 1;
	}
	
	/* �N���R���Z�b�g�����v��� */
	MtSetCc.data = (data[5] >> 5) & 0x01;	
	MtSetCc.first_flg = 1;
	MtSetCc.fail_flg  = 0;
	
}

/*
 * MT CAN ID(361) MT�M�A�|�W�V�����̏�Ԕ���
 * 
 * author	NSCS
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId361_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* MT�M�A�|�W�V���� */
	MtGearEstm.data = data[0] & 0x07;
	MtGearEstm.first_flg = 1;
	MtGearEstm.fail_flg = 0;
	
}

/*															
 * MT CAN ID(368) AT��ނ̏�Ԕ���							
 * 															
 * author	NSCS											
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X		
 * date		2014/10/--										
 * note														
 */															
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId368_state(const UI_8 len, const UI_8* data)	
{															

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* AT��� */
	MtAtType.data = (data[0] >> 2) & 0x07;					
	MtAtType.first_flg = 1;									
	MtAtType.fail_flg = 0;									

	/* �X�|�[�c�����v */
	MtSportsLamp.data = ((data[1] >> 4) & 0x03);
	MtSportsLamp.first_flg = 1;
	MtSportsLamp.fail_flg = 0;

	/* �X�m�[�z�[���h�����v */
	MtSnowHoldLamp.data = ((data[2] >> 2) & 0x01);
	MtSnowHoldLamp.first_flg = 1;
	MtSnowHoldLamp.fail_flg = 0;
}

/*
 * MT CAN ID(0D1) �쓮�֎����ώԗ֑��̏�Ԕ���
 * 
 * author	NSCS
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId0D1_state(const UI_8 len, const UI_8* data)
{

	UI_32 time1ms;
	if(len < 4) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �쓮�֎����ώԗ֑� */
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

	/*�����擾*/
	time1ms = Timer_ctrl_GetTime1ms();

	MtBrakePressure.data      = data[2];
	MtBrakePressure.first_flg = 1;
	MtBrakePressure.fail_flg  = 0;
	
	/* �ԑ�(0d1)���b�Z�[�W�ŏI��M�����X�V */
	S_can_0d1_received_time = time1ms;
}

#ifdef _FOR_RCF
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId0B4_state(const UI_8* data)
{
	/* �쓮�֎����ώԗ֑� */
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
* MT CAN ID(0D3) �ԑ��p���X�M���ώZ�l�̏�Ԕ���
 * 
 * author	NSCS
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId0D3_state(const UI_8 len, const UI_8* data)
{

	if(len < 7) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �ԑ��p���X�M���ώZ�l */
	MtSpPulseInteg.data = data[4] >> 2;
	MtSpPulseInteg.first_flg = 1;
	MtSpPulseInteg.fail_flg = 0;
}

/*
* MT CAN ID(152) �u���[�L/�����v�̏�Ԕ���
 * 
 * author	H.Watanabe
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2013/12/--
 * note	
 */

TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId152_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
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
* MT CAN ID(282) �o�b�N��SW�̏�Ԕ���
 * 
 * author	H.Watanabe
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId282_state(const UI_8 len, const UI_8* data)
{
	UI_32 time1ms;

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �\���p�O�C�� */
	MtOutTemp.data = data[3];	
	MtOutTemp.first_flg = 1;
	
	if(MtOutTemp.data >= 0xF1)
	{
		MtOutTemp.fail_flg = 1;
	}else{
		MtOutTemp.fail_flg = 0;
	}
	
	/* �V�[�g�x���g�o�b�N��SW P�� */
	MtPSeatBeltSW.data = (data[5] >> 1) & 0x01;	
	MtPSeatBeltSW.first_flg = 1;
	MtPSeatBeltSW.fail_flg = 0;
	
	/* �V�[�g�x���g�o�b�N��SW D�� */
	MtDSeatBeltSW.data = data[5]  & 0x01;
	MtDSeatBeltSW.first_flg = 1;
	MtDSeatBeltSW.fail_flg = 0;
	
	/* �d���n��� */
	MtPlace.data = (data[6] >> 4);
	MtPlace.first_flg = 1;
	MtPlace.fail_flg = 0;
	
	/* �u�����h�t���O */
	MtBrandFlag.data = data[6] &0x01;
	MtBrandFlag.first_flg = 1;
	MtBrandFlag.fail_flg = 0;

	/*�����擾*/
	time1ms = Timer_ctrl_GetTime1ms();
	if (S_brandflag_receive_lasttime_86A == 0){
		/* �O�񎞊Ԗ��ێ�*/
		/* ��M�Ԋu�ێ����O*/
		S_brandflag_receive_count_86A = 0;
	}else if(S_brandflag_receive_count_86A >= BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX) {
		/* ��M�Ԋu�ێ����ő�l�ȏ�*/
		/* �f�[�^���P�폜�i�O�֋l�߂�j*/
		(void)memmove(&S_brandflag_receive_interval_86A[0], &S_brandflag_receive_interval_86A[1], sizeof(UI_32) * (BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX - 1));
		/* �O�񂩂�̌o�ߎ��Ԃ�ݒ�*/
		S_brandflag_receive_interval_86A[BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX - 1]	=	(time1ms - S_brandflag_receive_lasttime_86A);
	}else{
		/* ��M�Ԋu�ێ����ő�l����*/
		/* �O�񂩂�̌o�ߎ��Ԃ�ݒ�*/
		S_brandflag_receive_interval_86A[S_brandflag_receive_count_86A]	=	(time1ms - S_brandflag_receive_lasttime_86A);
		/* ��M�Ԋu�ێ����{�P*/
		S_brandflag_receive_count_86A++;
	}
	/* �O�񎞊ԍX�V*/
	S_brandflag_receive_lasttime_86A = time1ms;

}

/*
* MT CAN ID(0D0) �Ǌp�Z���T�̏�Ԕ���
 * 
 * author	H.Watanabe
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2013/12/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId0D0_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �Ǌp�Z���T */
	MtSteerAngle.data = (UI_16)data[0] | ((UI_16)data[1] << 8);
	MtSteerAngle.first_flg = 1;
	MtSteerAngle.fail_flg = 0;
	
	if(MtSteerAngle.data == 0x8000)
	{
		MtSteerAngle.fail_flg = 1;
	}
	
	/* ���[���[�g */
	MtYawRate.data = (UI_16)data[2] | ((UI_16)data[3] << 8);
	MtYawRate.first_flg = 1;
	MtYawRate.fail_flg = 0;

	/* �������x */
	MtAccLR.data = data[6];
	MtAccLR.first_flg = 1;
	MtAccLR.fail_flg = 0;

	/* �O��G�Z���T */
	MtAccFR.data = 	data[7];
	MtAccFR.first_flg = 1;
	MtAccFR.fail_flg = 0;
	
	if(MtAccFR.data == 0x80)
	{
		MtAccFR.fail_flg = 1;
	}
}


/*
* MT CAN ID(0D4) �S�֎ԗ֑����b�Z�[�W�̏�Ԕ���
 * 
 * author	(OFT)Katsura
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2016/08/26
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId0D4_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* ���O�֎ԗ֑� */
	MtSpdWheelFtlh.data      = (UI_16)data[0] | ((UI_16)data[1] << 8);
	MtSpdWheelFtlh.first_flg = 1;
	MtSpdWheelFtlh.fail_flg  = 0;

	/* �E�O�֎ԗ֑� */
	MtSpdWheelFtrh.data      = (UI_16)data[2] | ((UI_16)data[3] << 8);
	MtSpdWheelFtrh.first_flg = 1;
	MtSpdWheelFtrh.fail_flg  = 0;

	/* ����֎ԗ֑� */
	MtSpdWheelRrlh.data      = (UI_16)data[4] | ((UI_16)data[5] << 8);
	MtSpdWheelRrlh.first_flg = 1;
	MtSpdWheelRrlh.fail_flg  = 0;

	/* �E��֎ԗ֑� */
	MtSpdWheelRrrh.data      = (UI_16)data[6] | ((UI_16)data[7] << 8);
	MtSpdWheelRrrh.first_flg = 1;
	MtSpdWheelRrrh.fail_flg  = 0;
}

/*
* MT CAN ID(375) �h�A���b�N��ԃ��b�Z�[�W�̏�Ԕ���
 * 
 * author	(OFT)Katsura
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2016/08/26
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId375_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �h�A���b�N��ԁi����ȁj*/
	MtDoorLockStatPass.data  = ((data[0] >> 1) & 0x01);
	MtDoorLockStatPass.first_flg = 1;
	MtDoorLockStatPass.fail_flg  = 0;

	/* �h�A���b�N��ԁi�^�]�ȁj*/
	MtDoorLockStatDrvr.data  = (data[0] & 0x01);
	MtDoorLockStatDrvr.first_flg = 1;
	MtDoorLockStatDrvr.fail_flg  = 0;
}


/**
 * MT CAN ID(040) �A�N�Z���J�x���A�G���W����]���̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId040_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �A�N�Z���J�x�� */
	MtRatioAccel.data = data[4];
	MtRatioAccel.first_flg = 1;
	MtRatioAccel.fail_flg = 0;
	
	/* �G���W����]�� */
	MtEngRpm.data = data[2] + ((UI_16)(data[3] & 0x3F) << 8);
	MtEngRpm.first_flg = 1;
	MtEngRpm.fail_flg = 0;
}

/**
 * MT CAN ID(146) �G���W����]��(�^�R)�̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId146_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �G���W����]��(�^�R) */
	MtEngRpm_Ta.data = data[2] + ((UI_16)(data[3] & 0x3F) << 8);
	MtEngRpm_Ta.first_flg = 1;
	MtEngRpm_Ta.fail_flg = 0;


}

/*															
 * MT CAN ID(345) �g�����X�~�b�V�������̏�Ԕ���			
 * 															
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X		
 * date		2020/09/--
 * note														
 */															
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId345_state(const UI_8 len, const UI_8* data)	
{															

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �z���ǈ��� */
	AtIntakePress.data = data[5];
	AtIntakePress.first_flg = 1;
	AtIntakePress.fail_flg = 0;

	/* ��C�� */
	AtAtmosphericPress.data = data[6];
	AtAtmosphericPress.fail_flg = 0;
	AtAtmosphericPress.first_flg = 1;

	/* �g�����X�~�b�V������� */
	MtTrnsType.data = (data[2] >> 7);
	MtTrnsType.first_flg = 1;
	MtTrnsType.fail_flg = 0;

	/* �G���W������ */
	MtTempOil.data = data[3];	
	MtTempOil.first_flg = 1;
	MtTempOil.fail_flg  = 0;

	/* �G���W������ */
	MtTempCoolant.data = data[4];
	MtTempCoolant.first_flg = 1;
	MtTempCoolant.fail_flg  = 0;
	if(MtTempCoolant.data == 0xFF)
	{
		MtTempCoolant.fail_flg = 1;
	}

}

/*                                                          
 * MT CAN ID(048) �X�|�[�c�V�t�g�M�A�ʒu���̏�Ԕ���        
 *                                                          
 * author	(OFT)Sakata
 * param    buf     [in]    ��MCAN�f�[�^�̐擪�A�h���X     
 * date		2020/09/--
 * note                                                     
 */                                                         
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId048_state(const UI_8 len, const UI_8* data)  
{                                                           

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �X�|�[�c�V�t�g�M�A�ʒu */
	MtSportGearPos.data = (data[3] >> 3) & 0x0F;
	MtSportGearPos.first_flg = 1;
	MtSportGearPos.fail_flg = 0;

	/* �V�t�g�ʒu */
	MtShiftPos.data = data[3] & 0x07;
	MtShiftPos.first_flg = 1;
	MtShiftPos.fail_flg = 0;

	/* �}�j���A�����[�hSW */
	AtManualModeSW.data = (data[4] >> 1) & 0x01;
	AtManualModeSW.first_flg = 1;
	AtManualModeSW.fail_flg = 0;

}

/*
 * MT CAN ID(68C) �u�����h�t���O�̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId68C_state(const UI_8 len, const UI_8* data)
{
	UI_32 time1ms;
	
	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �u�����h�t���O */
	MtBrandFlag.data = (data[1] >> 5) & 0x01;
	MtBrandFlag.first_flg = 1;
	MtBrandFlag.fail_flg = 0;

	/*�����擾*/
	time1ms = Timer_ctrl_GetTime1ms();
	if (S_brandflag_receive_lasttime_86B == 0){
		/* �O�񎞊Ԗ��ێ�*/
		/* ��M�Ԋu�ێ����O*/
		S_brandflag_receive_count_86B = 0;
	}else if(S_brandflag_receive_count_86B >= BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX) {
		/* ��M�Ԋu�ێ����ő�l�ȏ�*/
		/* �f�[�^���P�폜�i�O�֋l�߂�j*/
		(void)memmove(&S_brandflag_receive_interval_86B[0], &S_brandflag_receive_interval_86B[1], sizeof(UI_32) * (BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX - 1));
		/* �O�񂩂�̌o�ߎ��Ԃ�ݒ�*/
		S_brandflag_receive_interval_86B[BRANDFLAG_RECEIVE_INTERVAL_COUNT_MAX - 1]	=	(time1ms - S_brandflag_receive_lasttime_86B);
	}else{
		/* ��M�Ԋu�ێ����ő�l����*/
		/* �O�񂩂�̌o�ߎ��Ԃ�ݒ�*/
		S_brandflag_receive_interval_86B[S_brandflag_receive_count_86B]	=	(time1ms - S_brandflag_receive_lasttime_86B);
		/* ��M�Ԋu�ێ����{�P*/
		S_brandflag_receive_count_86B++;
	}
	/* �O�񎞊ԍX�V*/
	S_brandflag_receive_lasttime_86B = time1ms;

}

/*
 * MT CAN ID(241) MT�M�A�|�W�V�����̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId241_state(const UI_8 len, const UI_8* data)
{
	UI_16	fuel;

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �N���R���Z�b�g�����v��� */
	MtSetCc.data = (data[6] >> 7);
	MtSetCc.first_flg = 1;
	MtSetCc.fail_flg = 0;
	/* MT�M�A�|�W�V���� */
	MtGearEstm.data = (data[4] >> 3) & 0x07;
	MtGearEstm.first_flg = 1;
	MtGearEstm.fail_flg = 0;

	/* �R������[0.001cc/sec] */
	fuel = data[2] + ((UI_16)data[3] << 8);
	
	/* SUM_FC�̉��Z */
	/* ��M�C�x���g�ɂ�SUM_FC���v�Z���A500ms������FCSUM���v�Z����(N�͗L����M���Ƃ���) */
	Sum_Fc += fuel;
	Sum_Fc_N++;
	
}

/*                                                          
 * MT CAN ID(34A) �X�|�[�c�����v,�X�m�[�����v�̏�Ԕ���     
 *                                                          
 * author	(OFT)Sakata
 * param    buf     [in]    ��MCAN�f�[�^�̐擪�A�h���X     
 * date		2020/09/--
 * note                                                     
 */                                                         
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId34A_state(const UI_8 len, const UI_8* data)	
{															

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �X�|�[�c�����v */
	MtSportsLamp.data = ((data[7] >> 6) & 0x01);
	MtSportsLamp.first_flg = 1;
	MtSportsLamp.fail_flg = 0;

	/* �X�m�[�z�[���h�����v */
	MtSnowHoldLamp.data = (data[7] >> 7);
	MtSnowHoldLamp.first_flg = 1;
	MtSnowHoldLamp.fail_flg = 0;
}

/*
 * MT CAN ID(139) �쓮�֎����ώԗ֑��̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId139_state(const UI_8 len, const UI_8* data)
{

	UI_32 time1ms;
	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* TRC�쓮��� */
	AtTrcAct.data = (data[4] >> 5) & 0x01;
	AtTrcAct.first_flg = 1;
	AtTrcAct.fail_flg = 0;
	/* ABS�V�X�e���t�F�C���t���O			*/
	AtAbsFail.data = (data[3] >> 7);
	AtAbsFail.first_flg = 1;
	AtAbsFail.fail_flg = 0;
	
	/* ABS�쓮��� */
	AtAbsAct.data = (data[4] >> 4) & 0x01;
	AtAbsAct.first_flg = 1;
	AtAbsAct.fail_flg = 0;

	/* VSC�A���_�[������ */
	AtVscUnderStr.data = (data[4] >> 7);
	AtVscUnderStr.first_flg = 1;
	AtVscUnderStr.fail_flg = 0;

	/* VSC�I�[�o�[������ */
	AtVscOverStr.data = (data[4] >> 6) & 0x01;
	AtVscOverStr.first_flg = 1;
	AtVscOverStr.fail_flg = 0;

	/* �쓮�֎����ώԗ֑� */
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
	
	/*�����擾*/
	time1ms = Timer_ctrl_GetTime1ms();
	
	/*�u���[�L��*/
	MtBrakePressure.data      = data[5];
	MtBrakePressure.first_flg = 1;
	MtBrakePressure.fail_flg  = 0;
	
	/* �ԑ�(0d1)���b�Z�[�W�ŏI��M�����X�V */
	S_can_0d1_received_time = time1ms;
}

/*
* MT CAN ID(13C) �ԑ��p���X�M���ώZ�l�̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void s_tccc_jdg_MT_MsgId13C_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �ԑ��p���X�M���ώZ�l */
	MtSpPulseInteg.data = data[6] & 0x3F;
	MtSpPulseInteg.first_flg = 1;
	MtSpPulseInteg.fail_flg  = 0;
}

/*
* MT CAN ID(3AC) �u���[�L/�����v�̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note	
 */

TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId3AC_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}
	/* �u���[�L�����v */
	MtBreakLampSW.data = ( data[6] >> 6 ) & 0x01;	
	MtBreakLampSW.first_flg = 1;
	MtBreakLampSW.fail_flg = 0;
	
	/* �p�[�L���O�u���[�L */
	MtHandBreakSW.data = ( data[6] >> 5 ) & 0x01;	
	MtHandBreakSW.first_flg = 1;
	MtHandBreakSW.fail_flg = 0;
	
	/* MT���o�[�X */
	MtReverseSW.data =	(data[6] >> 4) & 0x01;
	MtReverseSW.first_flg = 1;
	MtReverseSW.fail_flg = 0;
	
	/* �w�b�h�����v�FHi */
	MtHeadLampHi.data = ( data[7] >> 2 ) & 0x01;	
	MtHeadLampHi.first_flg = 1;
	MtHeadLampHi.fail_flg = 0;
	
	/* �w�b�h�����v�FLo */
	MtHeadLampLo.data = ( data[7] >> 1 ) & 0x01;
	MtHeadLampLo.first_flg = 1;
	MtHeadLampLo.fail_flg = 0;
	
	/* �w�b�h�����v�F�X���[�� */
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
	
	/*�h�ASW D��*/
	MtDDoorSW.data = data[4] & 0x01;
	MtDDoorSW.first_flg = 1;
	MtDDoorSW.fail_flg = 0;
	
	/*�h�ASW P�� */
	MtPDoorSW.data = (data[4] >> 1) & 0x01;
	MtPDoorSW.first_flg = 1;
	MtPDoorSW.fail_flg = 0;
	
	/*�h�ASW �g�����N */
	MtTrunkDoorSW.data = (data[4] >> 5) & 0x01;
	MtTrunkDoorSW.first_flg = 1;
	MtTrunkDoorSW.fail_flg = 0;

	/*�h�ASW �t�[�h */
	MtHoodDoorSW.data = (data[4] >> 6) & 0x01;
	MtHoodDoorSW.first_flg = 1;
	MtHoodDoorSW.fail_flg = 0;
	
}

/*
* MT CAN ID(390) �o�b�N��SW�̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId390_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �\���p�O�C�� */
	MtOutTemp.data = data[4];	
	MtOutTemp.first_flg = 1;
	if(MtOutTemp.data >= 0xF1)
	{
		MtOutTemp.fail_flg = 1;
	}else{
		MtOutTemp.fail_flg = 0;
	}

	/* �E�B���J�[�E							*/
	AtBlinkerRight.data = (data[6] >> 3) & 0x01;	
	AtBlinkerRight.first_flg = 1;
	AtBlinkerRight.fail_flg = 0;

	/* �E�B���J�[��							*/
	AtBlinkerLeft.data = (data[6] >> 2) & 0x01;	
	AtBlinkerLeft.first_flg = 1;
	AtBlinkerLeft.fail_flg = 0;
	
	/* �V�[�g�x���g�o�b�N��SW P�� */
	MtPSeatBeltSW.data = (data[6] >> 1) & 0x01;	
	MtPSeatBeltSW.first_flg = 1;
	MtPSeatBeltSW.fail_flg = 0;
	
	/* �V�[�g�x���g�o�b�N��SW D�� */
	MtDSeatBeltSW.data = data[6]  & 0x01;
	MtDSeatBeltSW.first_flg = 1;
	MtDSeatBeltSW.fail_flg = 0;
	
	/* �d���n��� */
	MtPlace.data = (data[2] >> 3) & 0x0F;
	MtPlace.first_flg = 1;
	MtPlace.fail_flg = 0;
	
}

/*
* MT CAN ID(138) �Ǌp�Z���T�̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId138_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �Ǌp�Z���T */
	MtSteerAngle.data = (UI_16)data[2] | ((UI_16)data[3] << 8);
	MtSteerAngle.first_flg = 1;
	MtSteerAngle.fail_flg = 0;
	if(MtSteerAngle.data == 0x8000)
	{
		MtSteerAngle.fail_flg = 1;
	}
	
	/* ���[���[�g */
	MtYawRate.data = (UI_16)data[4] | ((UI_16)data[5] << 8);
	MtYawRate.first_flg = 1;
	MtYawRate.fail_flg = 0;

}

/*
* MT CAN ID(13B) �����N���b�Z�[�W�̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId13B_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �G���W���g���N�A�b�v�v���t���O*/
	AtEngTrqUpReqFlg.data = (data[1] >> 6);
	AtEngTrqUpReqFlg.first_flg = 1;
	AtEngTrqUpReqFlg.fail_flg = 0;

	/* �G���W���g���N�����v���t���O	*/
	AtEngTrqDwnReqFlg.data = (data[1] >> 5);
	AtEngTrqDwnReqFlg.first_flg = 1;
	AtEngTrqDwnReqFlg.fail_flg = 0;

	/* �G���W���g���N�A�b�v�v���l	*/
	AtEngTrqUpReqVal.data = (UI_16)data[2] | ((UI_16)data[3] << 8);
	AtEngTrqUpReqVal.first_flg = 1;
	AtEngTrqUpReqVal.fail_flg = 0;
	if(AtEngTrqUpReqVal.data == 0x0000)
	{
		AtEngTrqUpReqVal.fail_flg = 1;
	}

	/* �G���W���g���N�����v���l		*/
	AtEngTrqDwnReqVal.data = (UI_16)data[4] | ((UI_16)data[5] << 8);
	AtEngTrqDwnReqVal.first_flg = 1;
	AtEngTrqDwnReqVal.fail_flg = 0;
	if(AtEngTrqDwnReqVal.data == 0xFFFF)
	{
		AtEngTrqDwnReqVal.fail_flg = 1;
	}

	/* �������x */
	MtAccLR.data = data[6];
	MtAccLR.first_flg = 1;
	MtAccLR.fail_flg = 0;

	/* �O��G�Z���T */
	MtAccFR.data = 	data[7];
	MtAccFR.first_flg = 1;
	MtAccFR.fail_flg = 0;
	
	if(MtAccFR.data == 0x80)
	{
		MtAccFR.fail_flg = 1;
	}

}

/*
* MT CAN ID(13A) �S�֎ԗ֑����b�Z�[�W�̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId13A_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* ���O�֎ԗ֑�:FL */
	MtSpdWheelFtlh.data      = (UI_16)(data[1] >> 4) | ((UI_16)data[2] << 4) | ((UI_16)(data[3] & 0x01) << 12);
	MtSpdWheelFtlh.first_flg = 1;
	MtSpdWheelFtlh.fail_flg  = 0;

	/* �E�O�֎ԗ֑�:FR */
	MtSpdWheelFtrh.data      = (UI_16)(data[3] >> 1) | ((UI_16)(data[4] & 0x3F)<< 7);
	MtSpdWheelFtrh.first_flg = 1;
	MtSpdWheelFtrh.fail_flg  = 0;

	/* ����֎ԗ֑�:RL */
	MtSpdWheelRrlh.data      = (UI_16)(data[4] >> 6) | ((UI_16)data[5] << 2) | ((UI_16)(data[6] & 0x07) << 10);
	MtSpdWheelRrlh.first_flg = 1;
	MtSpdWheelRrlh.fail_flg  = 0;

	/* �E��֎ԗ֑�:RR */
	MtSpdWheelRrrh.data      = (UI_16)(data[6] >> 3) | ((UI_16)data[7] << 5);
	MtSpdWheelRrrh.first_flg = 1;
	MtSpdWheelRrrh.fail_flg  = 0;
}


/*
* MT CAN ID(6E2) �^�C����C����ԃ��b�Z�[�W�̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId6E2_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* ��C���P�� */
	AtAirPressUnit.data = ((data[2] >> 2) & 0x03);
	AtAirPressUnit.first_flg = 1;
	AtAirPressUnit.fail_flg = 0;
	
	/* ���O�֋�C�� */
	AtAirPressFtlh.data = (data[3] >> 1);
	AtAirPressFtlh.first_flg = 1;
	AtAirPressFtlh.fail_flg = 0;

	/* �E�O�֋�C�� */
	AtAirPressFtrh.data = (data[4] >> 1);
	AtAirPressFtrh.first_flg = 1;
	AtAirPressFtrh.fail_flg = 0;

	/* ����֋�C�� */
	AtAirPressRrlh.data = (data[5] >> 1);
	AtAirPressRrlh.first_flg = 1;
	AtAirPressRrlh.fail_flg = 0;

	/* �E��֋�C�� */
	AtAirPressRrrh.data = (data[6] >> 1);
	AtAirPressRrrh.first_flg = 1;
	AtAirPressRrrh.fail_flg = 0;
	
}


/*
* MT CAN ID(04B) �ڕW�M�A�i���̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2021/03/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId04B_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* �ڕW�M�A�i�� */
	MtTargetGearPos.data = (data[2] & 0x0f);
	MtTargetGearPos.first_flg = 1;
	MtTargetGearPos.fail_flg = 0;
	
}

/*
* MT CAN ID(041) ���G���W�����g���N�M����ԃ��b�Z�[�W�̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2021/03/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId041_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* ���G���W�����g���N */
	AtEngTrqShaft.data      = (UI_16)data[4] | ((UI_16)(data[5] & 0x7F) << 8);
	AtEngTrqShaft.first_flg = 1;
	AtEngTrqShaft.fail_flg  = 0;
	
}

/*
* MT CAN ID(328) VSC�M����ԃ��b�Z�[�W�̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2021/02/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId328_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}

	/* TRC ON/OFF */
	AtTrcOnOff.data = (data[5] >> 7);
	AtTrcOnOff.first_flg = 1;
	AtTrcOnOff.fail_flg = 0;
	/* VSC�E�H�[�j���O*/
	AtVscWarning.data = (data[3] >> 2) & 0x01;
	AtVscWarning.first_flg = 1;
	AtVscWarning.fail_flg = 0;
	
	/* ABS�E�H�[�j���O*/
	AtAbsWarning.data = (data[3] >> 1) & 0x01;
	AtAbsWarning.first_flg = 1;
	AtAbsWarning.fail_flg = 0;

	/* ECB�E�H�[�j���O*/
	AtEcbWarning.data = (data[3] & 0x01);
	AtEcbWarning.first_flg = 1;
	AtEcbWarning.fail_flg = 0;

	/* VSC ON/OFF */
	AtVscOnOff.data = (data[4] & 0x01);
	AtVscOnOff.first_flg = 1;
	AtVscOnOff.fail_flg = 0;
	
	/* VSC �쓮���*/
	AtVscAct.data = ((data[4] >> 1) & 0x01);
	AtVscAct.first_flg = 1;
	AtVscAct.fail_flg = 0;
	
	/* �g���N�V�����d�����[�h*/
	AtTrcMode.data = ((data[3] >> 6) & 0x01);
	AtTrcMode.first_flg = 1;
	AtTrcMode.fail_flg = 0;
	
}



/*
* MT CAN ID(652) �h�A���b�N��ԃ��b�Z�[�W�̏�Ԕ���
 * 
 * author	(OFT)Sakata
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
 * date		2020/09/--
 * note	
 */
TCAN_COMMON_CTRL_LOCAL void	s_tccc_jdg_MT_MsgId652_state(const UI_8 len, const UI_8* data)
{

	if(len < 8) {
		/* �f�[�^�����K��T�C�Y��菬������Γǂݎ̂� */
		return;
	}


	/* �h�A���b�N��ԁi����ȁj*/
	MtDoorLockStatPass.data  = ((data[2] >> 1) & 0x01);
	MtDoorLockStatPass.first_flg = 1;
	MtDoorLockStatPass.fail_flg  = 0;

	/* �h�A���b�N��ԁi�^�]�ȁj*/
	MtDoorLockStatDrvr.data  = (data[2] & 0x01);
	MtDoorLockStatDrvr.first_flg = 1;
	MtDoorLockStatDrvr.fail_flg  = 0;
	
	
}




/*
* ��Ԕ��� (data:UI_8)
 * 
 * author	H.Watanabe
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
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
*  ��Ԕ��� (data:UI_16)
 * 
 * author	H.Watanabe
 * param	buf		[in]	��MCAN�f�[�^�̐擪�A�h���X
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
 * �u���[�L���Ȃ܂�����
 * 
 * author	(OFT)Sakata
 * date		2017/07/19
 * note		�u���[�L���̂Ȃ܂������v�Z���s��
 */
TCAN_COMMON_CTRL_LOCAL void s_brake_proc(void)
{
	UI_32         cur_time;
	UI_8		  brake_press_raw;		/* �u���[�L�� ���l					*/
	UI_32		  brake_press_can;		/* �u���[�L�� CAN�M���l				*/
	UI_32		  brake_press_ave;		/* �u���[�L�� �\���ێ����ԓ�����	*/
	UI_32		  brake_press_lpf;		/* �u���[�L�� ���[�p�X�t�B���^�[������̒l	*/
	E_TCAN_RESULT result;
	
	/* ����M���͏������Ȃ� */
	if(S_can_0d1_received_time == 0) {
		return;
	}
	
	/* ��M�f�[�^�����ɏ����ς݂Ȃ牽�����Ȃ� */
	if(S_brake_press_proc_data_time >= S_can_0d1_received_time) {
		return;
	}

	/* ���ݎ����擾 */
	cur_time = Timer_ctrl_GetTime1ms();

	/* �u���[�L�����l�擾 */
	result = TCAN_Ctrl_GetMtBrakePressure(&brake_press_raw);
	if(result != TCAN_CTRL_OK) {
		brake_press_raw = 0;
	}

	/* ����\���Z */
	brake_press_can = (UI_32)((F_64)brake_press_raw * BRAKE_PRESS_RESOLUTION);
	
	/* ���ϒl�擾�i�O��܂Łj */
	brake_press_ave = get_vehicle_brake_press_average();

	/* ����̎ԑ����o�b�t�@�ɒǉ� */
	add_vehicle_brake_press(brake_press_can);
	
	/* ���[�p�X�t�B���^�[�ɂ������u���[�L���v�Z */
	brake_press_lpf = calc_vehicle_brake_press_lpf(brake_press_can, brake_press_ave, S_brake_press_smooth);
	
	/*�Ȃ܂������u���[�L���X�V*/
	S_brake_press_smooth = brake_press_lpf;
	/*�Ȃ܂������u���[�L���L��*/
	S_brake_press_smooth_enable = 1;

	/*�������ԍX�V*/
	S_brake_press_proc_data_time = cur_time;
	
}


/** 
 * �u���[�L���l�̃o�b�t�@�ւ̕ۑ�
 * 
 * author	(OFT)Sakata
 * date		2016/09/07
 * note		���͂��ꂽ�u���[�L���l�𕽋ϒl�v�Z�p�o�b�t�@�֊i�[����
 */
TCAN_COMMON_CTRL_LOCAL void add_vehicle_brake_press(UI_32 brake_press)
{
	/* �o�b�t�@����t�Ȃ�P�ÂO�ɋl�߂āA�Ō�����󂯂� */
	if(S_vehicle_brake_press_count >= VEHICLE_BRAKE_PRESS_COUNT_MAX) {
		/* �f�[�^��O�Ɉړ� */
		(void)memmove(&S_vehicle_brake_press_buf[0], &S_vehicle_brake_press_buf[1], sizeof(UI_32) * (VEHICLE_BRAKE_PRESS_COUNT_MAX - 1));
		/* �f�[�^���f�N�������g */
		S_vehicle_brake_press_count--;
	}
	/* �o�b�t�@����Ɋi�[ */
	S_vehicle_brake_press_buf[S_vehicle_brake_press_count] = brake_press;
	/* �f�[�^���C���N�������g */
	S_vehicle_brake_press_count++;
}

/** 
 * �\���ێ����ԓ��ł̕��ϒl�u���[�L���擾
 * 
 * author	(OFT)Sakata
 * date		2017/07/19
 * note		���ϒl�v�Z�p�o�b�t�@�̕��σu���[�L���l���擾����
 */
TCAN_COMMON_CTRL_LOCAL UI_32 get_vehicle_brake_press_average(void)
{
	UI_8  i;
	UI_32 sum_value = 0;
	UI_32 brake_press_avr;
	
	if(S_vehicle_brake_press_count == 0) {
		return(0);
	}

	/* �o�b�t�@���̑S�ԑ��l�����Z */
	for(i = 0; i < S_vehicle_brake_press_count; i++) {
		sum_value += S_vehicle_brake_press_buf[i];
	}
	
	/* ���ϒl�Z�o */
	brake_press_avr = sum_value / S_vehicle_brake_press_count;
	
	return brake_press_avr;
}

/** 
 * �u���[�L�������[�p�X�t�B���^�[�ɂ����鉉�Z
 * 
 * author	(OFT)Sakata
 * date		2017/07/19
 * note		�u���[�L�������[�p�X�t�B���^�[�ɂ����鉉�Z����
 */
TCAN_COMMON_CTRL_LOCAL UI_32 calc_vehicle_brake_press_lpf(UI_32 brake_press_can, UI_32 brake_press_ave, UI_32 brake_press_smooth)
{
	SI_32 brake_press_lpf;
	
	/*�u���[�L���i�t�B���^�[��j���u���[�L�����ϒl�i�O��l�j�{�i�u���[�L��CAN�l�@- �Ȃ܂��u���[�L���j���W�� */
	brake_press_lpf = (SI_32)((F_64)brake_press_ave + ((F_64)((SI_32)(brake_press_can - brake_press_smooth)) * BRAKE_PRESS_SMOOTHINGCOEFFICIENT));
	/*�}�C�i�X�l�ɂȂ�ꍇ�͂O�ɂ���*/
	if(brake_press_lpf < 0){
		brake_press_lpf = 0;
	}
	return (UI_32)brake_press_lpf;
}
/** 
 * �\���p�ԑ��v�Z����
 * 
 * author	(OFT)Katsura
 * date		2016/09/07
 * note		�\���p�ԑ��̌v�Z���s��
 */
TCAN_COMMON_CTRL_LOCAL void s_speed_proc(void)
{
	UI_32         cur_time;
	UI_16         vso_value;
	UI_32		  speed_value_can;		/* �ԑ� CAN�M���l			*/
	UI_32		  speed_value_ave;		/* �ԑ� �\���ێ����ԓ�����	*/
	UI_32		  speed_value_tol;		/* �ԑ� �w������������̒l	*/
	UI_32		  speed_value_fra;		/* �ԑ� �[��������̒l		*/
	UI_32		  speed_value_trg;		/* �ԑ� �\���ڕW�l			*/
	UI_32		  diff_value;
	UI_32		  diff_time;
	E_TCAN_RESULT result;
	
	/* ���ݎ����擾 */
	cur_time = Timer_ctrl_GetTime1ms();

	/* �@�t�F�[������ */
	if((S_can_0d1_received_time == 0)
	|| ((cur_time - S_can_0d1_received_time) > TIMEOUT_0D1)) {
		/* �ʐM�r��̏ꍇ�́AVSO=0 */
		vso_value = 0;
	}
	else {
		/* ��M�f�[�^�����ɏ����ς݂Ȃ牽�����Ȃ� */
		if(S_speed_proc_data_time >= S_can_0d1_received_time) {
			return;
		}

		/* VSO�擾 */
		result = TCAN_Ctrl_GetMTSpWheelAve(&vso_value);
		if(result != TCAN_CTRL_OK) {
			vso_value = 0;
		}
	}
	/* ����\���|�����킹�Ďԑ��擾 */
	speed_value_can = (UI_32)((F_64)vso_value * VSO_RESOLUTION);
	
	/* �L��臒l���� :300km/h */
	if (speed_value_can > THRESHOLD_SPEED_MAX) {
		speed_value_can = THRESHOLD_SPEED_MAX;
	}

	/* ����̎ԑ����o�b�t�@�ɒǉ� */
	add_vehicle_speed_value(speed_value_can);

	/* �A���Ϗ��� */
	speed_value_ave = get_vehicle_speed_average();

	/* �B�������� */
	speed_value_tol = calc_vehicle_speed_tolerance(speed_value_ave);

	/* �C�[������(1) */
	speed_value_fra = calc_vehicle_speed_fraction(speed_value_tol);

	/* �D�[������(2) */
	if(S_speed_value_mem > speed_value_fra) {
		diff_value = S_speed_value_mem - speed_value_fra;
	}
	else {
		diff_value = speed_value_fra - S_speed_value_mem;
	}
	if(diff_value > (1 * VEHICLE_SPEED_RESOLUTION)) {
		S_speed_value_mem = speed_value_fra;
	}

	/* �E�\���ڕW�l�擾 */
	speed_value_trg = calc_vehicle_speed_target(S_speed_value_mem);
#ifdef TCAN_DEBUG
	S_speed_value_trg1 = speed_value_trg;
#endif /* 2021.01.29 S.Sakata */
	
	/* �F�[���ߖT���� */
	speed_value_trg = calc_vehicle_speed_near0(speed_value_trg, speed_value_fra);

	/* �G�\���ێ����ԏ��� */
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
 * �ԑ��l�̃o�b�t�@�ւ̕ۑ�
 * 
 * author	(OFT)Katsura
 * date		2016/09/07
 * note		���͂��ꂽ�ԑ��l�𕽋ϒl�v�Z�p�o�b�t�@�֊i�[����
 */
TCAN_COMMON_CTRL_LOCAL void add_vehicle_speed_value(UI_32 speed_value_can)
{
	/* �o�b�t�@����t�Ȃ�P�ÂO�ɋl�߂āA�Ō�����󂯂� */
	if (S_speed_value_count_max == 0){
		/*�t�F�[��*/
		return;
	}

	if(S_vehicle_speed_value_count >= S_speed_value_count_max) {
		/* �̈悪�d�Ȃ�̂�memmove�֕ύX */
		(void)memmove(&S_vehicle_speed_value_buf[0], &S_vehicle_speed_value_buf[1], sizeof(UI_32) * (S_speed_value_count_max - 1));
		S_vehicle_speed_value_count--;
	}

	/* �o�b�t�@����Ɋi�[ */
	S_vehicle_speed_value_buf[S_vehicle_speed_value_count] = speed_value_can;
	S_vehicle_speed_value_count++;
}

/** 
 * �\���ێ����ԓ��ł̕��ϒl�Ԏ擾
 * 
 * author	(OFT)Katsura
 * date		2016/09/07
 * note		���ϒl�v�Z�p�o�b�t�@�̕��ώԑ��l���擾����
 */
TCAN_COMMON_CTRL_LOCAL UI_32 get_vehicle_speed_average(void)
{
	UI_8  i;
	UI_32 sum_value = 0;
	UI_32 speed_value_avr;
	
	if(S_vehicle_speed_value_count == 0) {
		return(0);
	}

	/* �o�b�t�@���̑S�ԑ��l�����Z */
	for(i = 0; i < S_vehicle_speed_value_count; i++) {
		sum_value += S_vehicle_speed_value_buf[i];
	}
	
	/* ���ϒl�Z�o */
	speed_value_avr = sum_value / S_vehicle_speed_value_count;
	return speed_value_avr;
}

/** 
 * �\���p�ԑ��̎w�������v�Z����
 * 
 * author	(OFT)Katsura
 * date		2016/09/07
 * note		���\���p�ԑ��̎w�������v�Z���s��
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
		/*CAN��M���:86A*/
		ctl = C_center_tole[C_Direction2SP[0][place]];
	}
	else if (S_vehicleCanInfo == TCAN_VEHICLE_INFO_86B)
	{
		/*CAN��M���:86B*/
		ctl = C_center_tole[C_Direction2SP[1][place]];
	}
	else
	{
		return 0;
	}
	
	/* �w������������̑��x���� */
	for(i = 0 ; i < TBL_NUM; i++ )
	{
		if(ctl[i].speed >= speed_value)
		{
			/*�w������������̎ԑ�*/
			vehicle_speed_tolerance = (UI_32)((SI_32)((F_32)speed_value * ctl[i].slope) + ctl[i].intercept);
			break;
		}
	}

	return vehicle_speed_tolerance;
}

/** 
 * �\���p�Ԏ��l�[������
 * 
 * author	(OFT)Katsura
 * date		2016/09/07
 * note		�\���p�Ԏ��l�̒[���������s��
 */
TCAN_COMMON_CTRL_LOCAL UI_32 calc_vehicle_speed_fraction(UI_32 speed_value)
{
	UI_32 speed_value_fra;
	
	/* �����_�ȉ��؂�̂� */
	speed_value_fra = speed_value / VEHICLE_SPEED_RESOLUTION * VEHICLE_SPEED_RESOLUTION;

	/* �����_�ȉ���0.5�ȏ�Ȃ�A0.5�����Z */
	if((speed_value % VEHICLE_SPEED_RESOLUTION) >= (VEHICLE_SPEED_RESOLUTION / 2)) {
		speed_value_fra += (VEHICLE_SPEED_RESOLUTION / 2);
	}

	return speed_value_fra;
}

/** 
 * �\���ڕW�l�Z�o����
 * 
 * author	(OFT)Katsura
 * date		2016/09/07
 * note		�\���ڕW�l�̌v�Z���s��
 */
TCAN_COMMON_CTRL_LOCAL UI_32 calc_vehicle_speed_target(UI_32 speed_value)
{
	UI_32 speed_value_trg;

	/* �^����ꂽ�ԑ��l�������_�P�P�^�Ŏl�̌ܓ����� */
	if((speed_value % VEHICLE_SPEED_RESOLUTION) >= (VEHICLE_SPEED_RESOLUTION / 2)) {
		speed_value_trg = ((speed_value / VEHICLE_SPEED_RESOLUTION) + 1) * VEHICLE_SPEED_RESOLUTION;
	}
	else {
		speed_value_trg = (speed_value / VEHICLE_SPEED_RESOLUTION) * VEHICLE_SPEED_RESOLUTION;
	}
	return speed_value_trg;
}

/** 
 * �[���t�߂ł̓��������^��~���䏈��
 * 
 * author	(OFT)Katsura
 * date		2016/09/07
 * note		�[���t�߂ł̓��������^��~���̕\���p�ԑ��l�����肷��
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
		/* ���������ǂ����̔��� */
		if(S_vehicle_speed_value_buf[S_vehicle_speed_value_count - 1] > S_vehicle_speed_value_buf[S_vehicle_speed_value_count - 2]) {
			/* �������łȂ��ꍇ�͂��̂܂ܕ\���ڕW�l */
			vehicle_speed_near0 = speed_value_trg;
		}
		else {
			if(speed_value_trg > (2 * VEHICLE_SPEED_RESOLUTION)) {
				/* 2km/h���傫����Ε\���ڕW�l */
				vehicle_speed_near0 = speed_value_trg;
			}
			else {
				if (S_speed_value_dsp > (2 * VEHICLE_SPEED_RESOLUTION)) {
					/* �O��̕\���p�ԑ���2km/h����Ȃ�A2km/h */
					vehicle_speed_near0 = (2 * VEHICLE_SPEED_RESOLUTION);
				}
				else if(S_speed_value_dsp > (1* VEHICLE_SPEED_RESOLUTION)) {
					vehicle_speed_near0 = (1 * VEHICLE_SPEED_RESOLUTION);
				}
				else if(S_speed_value_dsp > 0) {
					/* �O��\���ԑ���0km/h���`1km/h�̏ꍇ */
					if(speed_value_trg < (1 * VEHICLE_SPEED_RESOLUTION)) {
						/* �\���ڕW�l��1km/h�����Ȃ�0km/h */
						vehicle_speed_near0 = 0;
					}
					else {
						if(speed_value_fra == 0) {
							/* ���ώԑ���0�Ȃ�0m/h */
							vehicle_speed_near0 = 0;
						}
						else {
							/* ���ώԑ���0���Ȃ�1m/h */
							vehicle_speed_near0 = (1 * VEHICLE_SPEED_RESOLUTION);
						}
					}
				}
				else {
					/* �O��\���ԑ���0km/h�Ȃ�0m/h */
					vehicle_speed_near0 = 0;
				}
			
			}
		}
	}
	return vehicle_speed_near0;
}
