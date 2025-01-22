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
/* ���J�M������											*/
/********************************************************/

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include "system.h"
#include <stdlib.h> /* �����֐��𗘗p����̂ɕK�v */
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
/*	�}�N����`											*/
/********************************************************/
#define SIGNAL_LOCAL	static

#define SIGNAL_VERSION	0x00010000UL		/*���J�M�����X�g�o�[�W����*/
/*                          ^^^^^^
                            | | |
                            | | |
                            | | �����e�i�X�o�[�W����
                            | �}�C�i�[�o�[�W����
                            ���W���[�o�[�W����
*/

/* �M���l:���ʒ�` */
#define	SIGNAL_INFO_FUNC_NUM			(UI_16)700			/* �M���֐��̐� */
#define	SIGNAL_ID_MIN					(UI_16)1			/* �M��ID�ŏ��l�i����j */
#define	SIGNAL_ID_MAX					(UI_16)699			/* �M��ID�ő�l�i����j */
#define	SIGNAL_VALUE_IGNORE_32			0xFFFFFFFF			/* �M�������l */
#define	SIGNAL_VALUE_IGNORE_32_SIGNED	(UI_32)0x7FFFFFFF	/* �M�������l */
#define	SIGNAL_VALUE_IGNORE_16			(UI_16)0xFFFF		/* �M�������l */
#define	SIGNAL_VALUE_IGNORE_16_SIGNED	(UI_16)0x7FFF		/* �M�������l */
#define	SIGNAL_VALUE_IGNORE_12			(UI_16)0x0FFF		/* �M�������l */
#define	SIGNAL_VALUE_IGNORE_8			(UI_8)0xFF			/* �M�������l */
#define	SIGNAL_VALUE_IGNORE_8_SIGNED	(UI_8)0x7F			/* �M�������l */
#define	SIGNAL_VALUE_IGNORE_4			(UI_8)0x0F			/* �M�������l */


/* ���[�J�[ID */
#define	SIGNAL_VALUE_MAKERID_INIT			0x00		/*	���[�J�[ID�i000�j: Initial or Other								*/
#define	SIGNAL_VALUE_MAKERID_TOYOTA			0x01		/*	���[�J�[ID�i001�j: TOYOTA										*/
#define	SIGNAL_VALUE_MAKERID_SIMULATOR		0xF4		/*	���[�J�[ID�i244�j: Simulator									*/
#define	SIGNAL_VALUE_MAKERID_IGNORE			0xFF		/*	���[�J�[ID�i255�j: Ignore										*/
/* ���f���ԍ� */
#define	SIGNAL_VALUE_MODEL_NUMBER			0x01		/*	���f���ԍ�: ECU_Model_Number									*/
/* �L���t���O */
#define	SIGNAL_VALUE_VALIDFLAG_INIT			0x00		/*	�L���t���O:000: Initial											*/
#define	SIGNAL_VALUE_VALIDFLAG_VALID		0x01		/*	�L���t���O:001: Valid											*/
#define	SIGNAL_VALUE_VALIDFLAG_INVALID		0x02		/*	�L���t���O:002: Invalid											*/
#define	SIGNAL_VALUE_VALIDFLAG_IGNORE		0xFF		/*	�L���t���O:255: Ignore											*/

/*	CAN-Gateway ECU ID	*/
#define	SIGNAL_VALUE_CANGATEWAYECUID		( (SIGNAL_VALUE_MODEL_NUMBER 					<< 8	)	|\
											  (SIGNAL_VALUE_MAKERID_TOYOTA							)	)

/* �ԗ����� */
#define	SIGNAL_VALUE_VEHICLENAMEHIGHORDER			"86  "	/*	�ԗ�����	Vehicle Name (High-Order)							*/
#define	SIGNAL_VALUE_VEHICLENAMELOWORDER    		"    " 	/*	�ԗ�����	Vehicle Name (Low-Order)							*/
/* �d���n */
#define	SIGNAL_VALUE_DESTINATION		    "    "  	/*	�d�����n(�󔒁j	Destination										*/
/* �h���C�o�[�|�W�V���� */
#define	SIGNAL_VALUE_DRIVERPOSITION_INIT	0x00		/*	�h���C�o�[�|�W�V����(�����l)									*/
#define	SIGNAL_VALUE_DRIVERPOSITION_RIGHT	0x02		/*	�h���C�o�[�|�W�V����(�E)										*/
#define	SIGNAL_VALUE_DRIVERPOSITION_CENTER	0x04		/*	�h���C�o�[�|�W�V����(��)										*/
#define	SIGNAL_VALUE_DRIVERPOSITION_LEFT	0x08		/*	�h���C�o�[�|�W�V����(��)										*/
#define	SIGNAL_VALUE_DRIVERPOSITION_IGNORE	0x0F		/*	�h���C�o�[�|�W�V����(�����l)									*/
/* �G���W���^�� */
#define	SIGNAL_VALUE_ENGINEMODELHIGHORDE_86A     "FA20"	 	/*	�G���W���^��(�擪4����)	Engine Model Code (High-Order)			*/
#define	SIGNAL_VALUE_ENGINEMODELHIGHORDE_86B     "FA24"	 	/*	�G���W���^��(�擪4����)	Engine Model Code (High-Order)			*/
#define	SIGNAL_VALUE_ENGINEMODELHIGHORDE_UNKNOWN "    "	 	/*	�G���W���^��(�擪4����)	Engine Model Code (High-Order)			*/
#define	SIGNAL_VALUE_ENGINEMODELLOWORDER    "    "  	/*	�G���W���^��(���4����)	Engine Model Code (Low-Order)			*/
/* �R����� */
#define	SIGNAL_VALUE_FUELTYPE_GASOLINE		0x01 		/*	�R�����:Gasoline												*/
#define	SIGNAL_VALUE_FUELTYPE_DIESEL		0x02 		/*	�R�����:Diesel													*/
#define	SIGNAL_VALUE_FUELTYPE_ALCOHOL		0x04 		/*	�R�����:Alcohol												*/
#define	SIGNAL_VALUE_FUELTYPE_ELECTRICAL	0x08 		/*	�R�����:Electrical												*/
#define	SIGNAL_VALUE_FUELTYPE_HYDROGEN		0x10 		/*	�R�����:Hydrogen												*/
#define	SIGNAL_VALUE_FUELTYPE_HYBRID		0x80 		/*	�R�����:Hybrid													*/
/* �G���W������ */
#define	SIGNAL_VALUE_ENGINETYPE_4STROKE		0x01		/*	�G���W������	4 Stroke Reciprocating Engine 					*/
#define	SIGNAL_VALUE_ENGINETYPE_2STROKE		0x02		/*	�G���W������	2 Stroke Reciprocating Engine					*/
#define	SIGNAL_VALUE_ENGINETYPE_ROTARY		0x04		/*	�G���W������	Rotary Engine									*/
#define	SIGNAL_VALUE_ENGINETYPE_ELECTRICAL	0x08		/*	�G���W������	Electrical Motor								*/
#define	SIGNAL_VALUE_ENGINETYPE_INWHEEL		0x10		/*	�G���W������	In-wheel Motor									*/
#define	SIGNAL_VALUE_ENGINETYPE_FUELCELL	0x20		/*	�G���W������	Fuel Cell Motor									*/
#define	SIGNAL_VALUE_ENGINETYPE_OTHER		0x40		/*	�G���W������	Other Type Engine								*/
/* �C���� */
#define	SIGNAL_VALUE_NUMBEROFCYLINDERS		0x04		/*	�C����															*/
/* �z�C���� */
#define	SIGNAL_VALUE_INTAKESYSTEM_NATURALASPIRATION		0x01	/*	�z�C����: Natural Aspiration							*/
#define	SIGNAL_VALUE_INTAKESYSTEM_TURBOCHARGER			0x02	/*	�z�C����: Turbocharger									*/
#define	SIGNAL_VALUE_INTAKESYSTEM_SUPERCHARGER			0x03	/*	�z�C����: Supercharger									*/
#define	SIGNAL_VALUE_INTAKESYSTEM_ELECTRICTURBOCHARGER	0x04	/*	�z�C����: Electric Turbocharger							*/
/*	�G���W������	Engine Character	*/
#define	SIGNAL_VALUE_ENGINECHARACTER		( ((UI_32)SIGNAL_VALUE_INTAKESYSTEM_NATURALASPIRATION	<< 20	)	|\
											  ((UI_32)SIGNAL_VALUE_NUMBEROFCYLINDERS  				<< 16	)	|\
											  ((UI_32)SIGNAL_VALUE_ENGINETYPE_4STROKE 				<< 8	)	|\
											  ((UI_32)SIGNAL_VALUE_FUELTYPE_GASOLINE						)	)

/* �g�����X�~�b�V������� */
#define	SIGNAL_VALUE_TRANSMISSION_INITIAL			0x00	/*	�g�����X�~�b�V������� 00: Initial							*/
#define	SIGNAL_VALUE_TRANSMISSION_MANUAL			0x01	/*	�g�����X�~�b�V������� 01: Manual							*/
#define	SIGNAL_VALUE_TRANSMISSION_STEPAUTOMATIC		0x02	/*	�g�����X�~�b�V������� 02: Step Automatic					*/
#define	SIGNAL_VALUE_TRANSMISSION_CVT				0x03	/*	�g�����X�~�b�V������� 03: CVT								*/
#define	SIGNAL_VALUE_TRANSMISSION_ELECTRIC_CVT		0x04	/*	�g�����X�~�b�V������� 04: Electric CVT						*/
#define	SIGNAL_VALUE_TRANSMISSION_MOTOR				0x05	/*	�g�����X�~�b�V������� 05: Motor							*/
#define	SIGNAL_VALUE_TRANSMISSION_IGNORE			0x0F	/*	�g�����X�~�b�V������� 15: Ignore							*/
/* �쓮�� */
#define	SIGNAL_VALUE_DRIVEWHEEL_INITIAL				0x00	/*	�쓮�� 00: Initial											*/
#define	SIGNAL_VALUE_DRIVEWHEEL_FRONT				0x01	/*	�쓮�� 01: Front Drive										*/
#define	SIGNAL_VALUE_DRIVEWHEEL_REAR				0x02	/*	�쓮�� 02: Rear Drive										*/
#define	SIGNAL_VALUE_DRIVEWHEEL_ALLWHEEL_FULLTIME	0x03	/*	�쓮�� 03: All Wheel Drive(Full Time)						*/
#define	SIGNAL_VALUE_DRIVEWHEEL_ALLWHEEL_PARTTIME	0x04	/*	�쓮�� 04: All Wheel Drive(Part Time)						*/
#define	SIGNAL_VALUE_DRIVEWHEEL_IGNORE				0x0F	/*	�쓮�� 15: Ignore											*/
/* �M�A�i�� */
#define	SIGNAL_VALUE_NUMBEROFTHESTEPS				0x06	/* �M�A�i��														*/
/* �^�C�����׏d���a */
#define	SIGNAL_VALUE_DYNAMICXLOADRADIUSOFTIRE		0xFFF	/* �^�C�����׏d���a:����										*/
/*	�h���C�u�g���[��	Drive Train Character						*/
#define	SIGNAL_VALUE_DRIVETRAINCHARACTER	( ((UI_32)SIGNAL_VALUE_DYNAMICXLOADRADIUSOFTIRE		<< 12	)	|\
											  ((UI_32)SIGNAL_VALUE_NUMBEROFTHESTEPS  			<< 8	)	|\
											  ((UI_32)SIGNAL_VALUE_DRIVEWHEEL_REAR 				<< 4	)	)
/* �h�ASW��� */
#define SIGNAL_VALUE_DOOR_CLOSE						0x00	/* �h�ASW���	(0000)2: Initial or Close						*/
#define SIGNAL_VALUE_DOOR_RIGHT_OPEN				0x02	/* �h�ASW���	(--*-)2: Right  1-Open, 0-Close					*/
#define SIGNAL_VALUE_DOOR_LEFT_OPEN					0x04	/* �h�ASW���	(-*--)2: Left  1-Open, 0-Close					*/
#define SIGNAL_VALUE_DOOR_IGNORE					0x0F	/* �h�ASW���	(1111)2: Ignore									*/
#define SIGNAL_VALUE_DOOR_OPEN						0x00	/* �h�A���b�N���	(0000)2: Initial or Open					*/
#define SIGNAL_VALUE_DOOR_RIGHT_LOCK				0x02	/* �h�A���b�N���	(--*-)2: Right  1-Rock, 0-UnRock			*/
#define SIGNAL_VALUE_DOOR_LEFT_LOCK					0x04	/* �h�A���b�N���	(-*--)2: Left  1-Rock, 0-UnRock				*/

/* �V�[�g�x���g��� */
#define SIGNAL_VALUE_SEATBELT_INITIAL				0x00	/* �V�[�g�x���g���	(0000)2: Initial							*/
#define SIGNAL_VALUE_SEATBELT_NOONELOCKED			0x01	/* �V�[�g�x���g���	(0001)2: No one Locked						*/
#define SIGNAL_VALUE_SEATBELT_RIGHT_LOCKED			0x02	/* �V�[�g�x���g���	(--*-)2: Right  1-Locked, 0-Unlocked		*/
#define SIGNAL_VALUE_SEATBELT_CENTER_LOCKED			0x04	/* �V�[�g�x���g���	(-*--)2: Center  1-Locked, 0-Unlocked		*/
#define SIGNAL_VALUE_SEATBELT_LEFT_LOCKED			0x08	/* �V�[�g�x���g���	(*---)2: Left  1-Locked, 0-Unlocked			*/
#define SIGNAL_VALUE_SEATBELT_IGNORE				0x0F	/* �V�[�g�x���g���	(1111)2: Ignore								*/
/* �A�N�Z�������I����� */
#define	SIGNAL_VALUE_ACCEL_CHARACTER_INITIAL		0x00	/* �A�N�Z�������I����� Initial									*/
#define	SIGNAL_VALUE_ACCEL_CHARACTER_NORMAL			0x01	/* �A�N�Z�������I����� Normal									*/
#define	SIGNAL_VALUE_ACCEL_CHARACTER_ECO			0x02	/* �A�N�Z�������I����� ECO										*/
#define	SIGNAL_VALUE_ACCEL_CHARACTER_SNOW			0x03	/* �A�N�Z�������I����� Snow									*/
#define	SIGNAL_VALUE_ACCEL_CHARACTER_POWER			0x04	/* �A�N�Z�������I����� Power									*/
#define	SIGNAL_VALUE_ACCEL_CHARACTER_SPORTS			0x05	/* �A�N�Z�������I����� Sports									*/
#define	SIGNAL_VALUE_ACCEL_CHARACTER_SPECIAL		0x06	/* �A�N�Z�������I����� Special									*/
#define	SIGNAL_VALUE_ACCEL_CHARACTER_IGNORE			0x0F	/* �A�N�Z�������I����� Ignore									*/

/* �V�[�g�ݒ�(1���) */
#define	SIGNAL_VALUE_SEATSETTING_FIRSTROW_RIGHT		0x02	/*	�V�[�g�ݒ�(1���) (**10)2: Right  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_FIRSTROW_CENTER	0x04	/*	�V�[�g�ݒ�(1���) (*1**)2: Center  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_FIRSTROW_LEFT		0x08	/*	�V�[�g�ݒ�(1���) (1***)2: Left  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_FIRSTROW_IGNORE	0x0F	/*	�V�[�g�ݒ�(1���) (1111)2: Ignore							*/
/* �V�[�g�ݒ�(2���) */
#define	SIGNAL_VALUE_SEATSETTING_SECONDROW_RIGHT	0x02	/*	�V�[�g�ݒ�(2���) (**10)2: Right  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_SECONDROW_CENTER	0x04	/*	�V�[�g�ݒ�(2���) (*1**)2: Center  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_SECONDROW_LEFT		0x08	/*	�V�[�g�ݒ�(2���) (1***)2: Left  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_SECONDROW_IGNORE	0x0F	/*	�V�[�g�ݒ�(2���) (1111)2: Ignore							*/
/* �V�[�g�ݒ�(3���) */
#define	SIGNAL_VALUE_SEATSETTING_THIRDROW_RIGHT		0x02	/*	�V�[�g�ݒ�(3���) (**10)2: Right  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_THIRDROW_CENTER	0x04	/*	�V�[�g�ݒ�(3���) (*1**)2: Center  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_THIRDROW_LEFT		0x08	/*	�V�[�g�ݒ�(3���) (1***)2: Left  1-Enable, 0-Disable		*/
#define	SIGNAL_VALUE_SEATSETTING_THIRDROW_IGNORE	0x0F	/*	�V�[�g�ݒ�(3���) (1111)2: Ignore							*/
/*	�V�[�g�ݒ�	Seat Setting										*/
#define	SIGNAL_VALUE_SEATSETTING			( ((SIGNAL_VALUE_SEATSETTING_THIRDROW_IGNORE)											<< 8)	|\
											  ((SIGNAL_VALUE_SEATSETTING_SECONDROW_RIGHT | SIGNAL_VALUE_SEATSETTING_SECONDROW_LEFT) << 4)	|\
											  ((SIGNAL_VALUE_SEATSETTING_FIRSTROW_RIGHT  | SIGNAL_VALUE_SEATSETTING_FIRSTROW_LEFT) 		)	)




/* �A�N�Z������ */
#define	SIGNAL_VALUE_ACCELCHARACTER_INITIAL	0x00		/*	�A�N�Z������:Initial											*/
#define	SIGNAL_VALUE_ACCELCHARACTER_NORMAL	0x01		/*	�A�N�Z������:Normal												*/
#define	SIGNAL_VALUE_ACCELCHARACTER_ECO		0x02		/*	�A�N�Z������:ECO												*/
#define	SIGNAL_VALUE_ACCELCHARACTER_SNOW	0x03		/*	�A�N�Z������:Snow												*/
#define	SIGNAL_VALUE_ACCELCHARACTER_POWER	0x04		/*	�A�N�Z������:Power												*/
#define	SIGNAL_VALUE_ACCELCHARACTER_SPORTS	0x05		/*	�A�N�Z������:Sports												*/
#define	SIGNAL_VALUE_ACCELCHARACTER_SPECIAL	0x06		/*	�A�N�Z������:Special											*/
#define	SIGNAL_VALUE_ACCELCHARACTER_IGNORE	0x15		/*	�A�N�Z������:Ignore												*/
/* �u���[�L�����v */
#define	SIGNAL_VALUE_BRAKELAMP_INIT			0x00		/*	�u���[�L�����v�����l											*/
#define	SIGNAL_VALUE_BRAKELAMP_OFF			0x01		/*	�u���[�L�����vOFF�l												*/
#define	SIGNAL_VALUE_BRAKELAMP_ON			0x02		/*	�u���[�L�����vON�l												*/
#define	SIGNAL_VALUE_BRAKELAMP_IGNORE		0x0F		/*	�u���[�L�����v�����l											*/
/* �p�[�L���O�u���[�L������ */
#define	SIGNAL_VALUE_PARKINGBRAKE_INIT		0x00		/*	�p�[�L���O�u���[�L�����l										*/
#define	SIGNAL_VALUE_PARKINGBRAKE_OFF		0x01		/*	�p�[�L���O�u���[�LOFF�l											*/
#define	SIGNAL_VALUE_PARKINGBRAKE_ON		0x02		/*	�p�[�L���O�u���[�LON�l											*/
#define	SIGNAL_VALUE_PARKINGBRAKE_IGNORE	0x0F		/*	�p�[�L���O�u���[�L�����l										*/
/* AT�V�t�g��� */
#define	SIGNAL_VALUE_ATSHIFT_INIT			0x00		/*	AT�V�t�g���(00000000)2:Initial									*/
#define	SIGNAL_VALUE_ATSHIFT_P				0x01		/*	AT�V�t�g���(00000001)2:P (Parking) 							*/
#define	SIGNAL_VALUE_ATSHIFT_R				0x02		/*	AT�V�t�g���(00000010)2:R (Reverse) 							*/
#define	SIGNAL_VALUE_ATSHIFT_N				0x04		/*	AT�V�t�g���(00000100)2:N (Neutral)								*/
#define	SIGNAL_VALUE_ATSHIFT_D				0x08		/*	AT�V�t�g���(00001000)2:D (Drive)								*/
#define	SIGNAL_VALUE_ATSHIFT_B				0x18		/*	AT�V�t�g���(00011000)2:B (Engine Break Mode)					*/
#define	SIGNAL_VALUE_ATSHIFT_M				0x20		/*	AT�V�t�g���(00100000)2:M (Manual mode)							*/
#define	SIGNAL_VALUE_ATSHIFT_DM				0x28		/*	AT�V�t�g���(00101000)2:DM (Temporary Paddle Control Mode)		*/
#define	SIGNAL_VALUE_ATSHIFT_DS				0x48		/*	AT�V�t�g���(01001000)2:DS (Sporty Manner Drive Mode)			*/
#define	SIGNAL_VALUE_ATSHIFT_IGNORE			0xFF		/*	AT�V�t�g���(11111111)2:Ignore or MT(Manual Transmission) 		*/
/* �M�A�i�� */
#define	SIGNAL_VALUE_GEARPOS_INIT			0x00		/*	�M�A�i��(00000000)2:Initial or not decision						*/
#define	SIGNAL_VALUE_GEARPOS_1ST			0x01		/*	�M�A�i��:1ST													*/
#define	SIGNAL_VALUE_GEARPOS_2ND			0x02		/*	�M�A�i��:2ND													*/
#define	SIGNAL_VALUE_GEARPOS_3RD			0x03		/*	�M�A�i��:3RD													*/
#define	SIGNAL_VALUE_GEARPOS_4TH			0x04		/*	�M�A�i��:4TH													*/
#define	SIGNAL_VALUE_GEARPOS_5TH			0x05		/*	�M�A�i��:5TH													*/
#define	SIGNAL_VALUE_GEARPOS_6TH			0x06		/*	�M�A�i��:6TH													*/
#define	SIGNAL_VALUE_GEARPOS_7TH			0x07		/*	�M�A�i��:7TH													*/
#define	SIGNAL_VALUE_GEARPOS_8TH			0x08		/*	�M�A�i��:8TH													*/
#define	SIGNAL_VALUE_GEARPOS_9TH			0x09		/*	�M�A�i��:9TH													*/
#define	SIGNAL_VALUE_GEARPOS_10TH			0x0A		/*	�M�A�i��:10TH													*/
#define	SIGNAL_VALUE_GEARPOS_REVERSE		0x80		/*	�M�A�i��(1000 0000)2:Reverse									*/
#define	SIGNAL_VALUE_GEARPOS_IGNORE			0xFF		/*	�M�A�i��(1111 1111)2:Ignore										*/
/* �w�b�h���C�g�_���M�� */
#define	SIGNAL_VALUE_HEADLIGHT_INITIAL		0x00		/*	�w�b�h���C�g(0000)2: Initial									*/
#define	SIGNAL_VALUE_HEADLIGHT_OFF			0x01		/*	�w�b�h���C�g(0001)2: Off										*/
#define	SIGNAL_VALUE_HEADLIGHT_SMALL		0x02		/*	�w�b�h���C�g(--10)2: Parking Lamp On(small)						*/
#define	SIGNAL_VALUE_HEADLIGHT_LO			0x04		/*	�w�b�h���C�g(-1--)2: Lo Beam 1-On, 0-Off						*/
#define	SIGNAL_VALUE_HEADLIGHT_HIGHANDLO	0x0C		/*	�w�b�h���C�g(11--)2: High and Lo Beam 1-On, 0-Off				*/
#define	SIGNAL_VALUE_HEADLIGHT_IGNORE		0x0F		/*	�w�b�h���C�g(1111)2: Ignore										*/
/* �N����� */
#define	SIGNAL_VALUE_POWER_INITIAL			0x00		/*	�N�����: (0000 0000)2: Initial									*/
#define	SIGNAL_VALUE_POWER_PLUS_B			0x01		/*	�N�����: (0*** ***1)2: +B										*/
#define	SIGNAL_VALUE_POWER_ACC				0x02		/*	�N�����: (0*** **1*)2: ACC										*/
#define	SIGNAL_VALUE_POWER_IG				0x04		/*	�N�����: (0*** *1**)2: IG										*/
#define	SIGNAL_VALUE_POWER_READY			0x08		/*	�N�����: (0*** 1***)2: READY									*/
#define	SIGNAL_VALUE_POWER_CHANGING 		0x10		/*	�N�����: (0**1 ****)2: Changing								*/
#define	SIGNAL_VALUE_POWER_IGNORE			0xFF		/*	�N�����: (1111 1111)2: Ignore									*/
/* �ԑ��i�����j */
#define	SIGNAL_VALUE_DIRECTION_INITIAL		0x00		/*	����(0000)2: Initial											*/	
#define	SIGNAL_VALUE_DIRECTION_STOP			0x01		/*	����(0001)2: Stop												*/
#define	SIGNAL_VALUE_DIRECTION_FORWARD		0x02		/*	����(0010)2: Forward											*/
#define	SIGNAL_VALUE_DIRECTION_REVERSE		0x04		/*	����(0100)2: Reverse											*/
#define	SIGNAL_VALUE_DIRECTION_IGNORE		0x0F		/*	����(1111)2: Ignore												*/
/* ���[���C�g�o�͕���\ */
#define	SIGNAL_VALUE_YAWRATE_RESOLUTION		0.0005		/* ���[���C�g�o�͕���\											*/
/* GNSS�M�� */
#define	SIGNAL_VALUE_UTC_RESOLUTION			0.001		/*	UTC���ԕ���\												*/
#define	SIGNAL_VALUE_LATITUDE_RESOLUTION	0.0000001	/*	�ܓx����\													*/
#define	SIGNAL_VALUE_LONGTUDE_RESOLUTION	0.0000001	/*	�o�x����\													*/
#define	SIGNAL_VALUE_ALTITUDE_RESOLUTION	0.001		/*	���x����\													*/
#define	SIGNAL_VALUE_GEOID_RESOLUTION		0.001		/*	�W�I�C�h������\											*/

#define	SIGNAL_VALUE_VEHICLESPEED_RESOLUTION		0.01		/*	�����������x����\									*/
#define	SIGNAL_VALUE_HEADING_VEHICLE_RESOLUTION		0.01		/*	���������ړ�����									*/
#define	SIGNAL_VALUE_VDOP_RESOLUTION				0.1			/*	VDOP�o�͕���\										*/
#define	SIGNAL_VALUE_HDOP_RESOLUTION				0.1			/*	HDOP�o�͕���\										*/
#define	SIGNAL_VALUE_PDOP_RESOLUTION				0.1			/*	PDOP�o�͕���\										*/
#define	SIGNAL_VALUE_ACCURACY_RESOLUTION			0.01		/*	���x�o�͕���\										*/

#define	SIGNAL_VALUE_TIREPRESS_RESOLUTION		0.1			/* �^�C����C������\										*/

/* ABS��� */
#define	SIGNAL_VALUE_ABS_INIT						0x00	/* VSC��ԁF�����l												*/
#define	SIGNAL_VALUE_ABS_OFF						0x01	/* VSC��ԁFOFF�i���g�p�j										*/
#define	SIGNAL_VALUE_ABS_SET						0x02	/* VSC��ԁF�L��												*/
#define	SIGNAL_VALUE_ABS_SET_AND_ACTIVE				0x06	/* VSC��ԁF�L��������											*/
#define	SIGNAL_VALUE_ABS_IGNORE						0x0F	/* VSC��ԁF����												*/
/* VSC��� */
#define	SIGNAL_VALUE_VSC_INIT_OR_NORMAL				0x00	/* VSC��ԁF�����l or �m�[�}��									*/
#define	SIGNAL_VALUE_VSC_SET_TRC_OFF				0x01	/* VSC��ԁFTRC OFF												*/
#define	SIGNAL_VALUE_VSC_SET_VSC_OFF				0x02	/* VSC��ԁFVSC OFF												*/
#define	SIGNAL_VALUE_VSC_SET_TRC_VSC_OFF			0x03	/* VSC��ԁFTRC & VSC OFF										*/
#define	SIGNAL_VALUE_VSC_SET_SPECIAL				0x04	/* VSC��ԁFSpecial(TRACK or EXPERT)							*/
#define	SIGNAL_VALUE_VSC_IGNORE						0xFF	/* VSC��ԁF����												*/
#define	SIGNAL_VALUE_VSC_TRC_ACTIVE_BIT				0x08	/* VSC��ԁFTRC���䒆											*/
#define	SIGNAL_VALUE_VSC_VSC_ACTIVE_BIT				0x10	/* VSC��ԁFVSC���䒆											*/
#define	SIGNAL_VALUE_VSC_NOTACTIVE					0x00	/* VSC��ԁFVSC/TRC�񐧌�										*/

#define	SIGNAL_VALUE_NONACTIVE						0x00	/* ABS/VSC/TRC��ԁF�񓮍�										*/
#define	SIGNAL_VALUE_ACTIVE							0x01	/* ABS/VSC/TRC��ԁF����										*/
#define	SIGNAL_VALUE_ENGTRQ_NO_REQUEST				0x00	/* �G���W���g���N�A�b�v�_�E���v���Ȃ�							*/
#define	SIGNAL_VALUE_ENGTRQ_REQUEST					0x01	/* �G���W���g���N�A�b�v�_�E���v������							*/
#define SIGNAL_CONV_MMHG_TO_KPA 					0.13332	/*	�P�ʕϊ�	1mmHg = 0.13332kPa								*/
#define SIGNAL_VALUE_INTAKE_PRESS_RESOLUTION 		0.1		/*	�z���ǈ��́@����\											*/
#define SIGNAL_VALUE_INTAKE_PRESS_OFFSET 			-100	/*	�z���ǈ��́@�I�t�Z�b�g										*/
#define SIGNAL_VALUE_ATM_PRESS_RESOLUTION 			0.01	/*	��C���@����\												*/
#define	SIGNAL_VALUE_TARGET_GEAR_INIT				0x00	/*	�ڕW�M�A�i��:�����l											*/
#define	SIGNAL_VALUE_TARGET_GEAR_1ST				0x01	/*	�ڕW�M�A�i��:1ST											*/
#define	SIGNAL_VALUE_TARGET_GEAR_2ND				0x02	/*	�ڕW�M�A�i��:2ND											*/
#define	SIGNAL_VALUE_TARGET_GEAR_3RD				0x03	/*	�ڕW�M�A�i��:3RD											*/
#define	SIGNAL_VALUE_TARGET_GEAR_4TH				0x04	/*	�ڕW�M�A�i��:4TH											*/
#define	SIGNAL_VALUE_TARGET_GEAR_5TH				0x05	/*	�ڕW�M�A�i��:5TH											*/
#define	SIGNAL_VALUE_TARGET_GEAR_6TH				0x06	/*	�ڕW�M�A�i��:6TH											*/
#define	SIGNAL_VALUE_TARGET_GEAR_P					0x10	/*	�ڕW�M�A�F�V�t�g���(00000001)2:P (Parking) 				*/
#define	SIGNAL_VALUE_TARGET_GEAR_R					0x20	/*	�ڕW�M�A�F�V�t�g���(00000010)2:R (Reverse) 				*/
#define	SIGNAL_VALUE_TARGET_GEAR_N					0x40	/*	�ڕW�M�A�F�V�t�g���(00000100)2:N (Neutral)					*/
#define	SIGNAL_VALUE_TARGET_GEAR_D					0x80	/*	�ڕW�M�A�F�V�t�g���(00001000)2:D (Drive)					*/
#define	SIGNAL_VALUE_TARGET_GEAR_INVALID			0xFF	/*	�ڕW�M�A�F����												*/
#define	SIGNAL_VALUE_BLINKER_INITIAL				0x00	/*	�E�B���J�[���: (0000)2: Initial							*/
#define	SIGNAL_VALUE_BLINKER_OFF					0x01	/*	�E�B���J�[���: (0001)2: Off								*/
#define	SIGNAL_VALUE_BLINKER_TURN_RIGHT				0x02	/*	�E�B���J�[���: (0010)2: Right 								*/
#define	SIGNAL_VALUE_BLINKER_TURN_LEFT				0x04	/*	�E�B���J�[���: (0100)2: Left								*/
#define	SIGNAL_VALUE_BLINKER_INVALID				0x0F	/*	�E�B���J�[���: (1111)2: Invalid							*/
#define	SIGNAL_VALUE_HODD_INITIAL_OR_CLOSE			0x00	/*	�t�[�h���: (0000)2: Initial or Close						*/
#define	SIGNAL_VALUE_HODD_FRONT_OPEN				0x02	/*	�t�[�h���: (0010)2: Front									*/
#define	SIGNAL_VALUE_HODD_REAR_OPEN					0x04	/*	�t�[�h���: (0100)2: Rear 									*/

/* GPS���x */
#define	SIGNAL_VALUE_GSPFIX_NOFIX						0 		/* GPS Fix Type: 0 :���p�ł��Ȃ��A���� 													*/
#define	SIGNAL_VALUE_GSPFIX_GPSFIX						1 		/* GPS Fix Type: 1 : GPS ���� 															*/
#define	SIGNAL_VALUE_GSPFIX_DGPSFIX						2 		/* GPS Fix Type: 2 : DGPS ����															*/
#define	SIGNAL_VALUE_GSPFIX_GPS PPS						3 		/* GPS Fix Type: 3 : GPS-PPS 															*/
#define	SIGNAL_VALUE_GSPFIX_REALTIMEKINEMATIC			4 		/* GPS Fix Type: 4 : Real Time Kinematic. System used in RTK mode with fixed integers 	*/
#define	SIGNAL_VALUE_GSPFIX_FLOATRTK					5 		/* GPS Fix Type: 5 : Float RTK. Satellite system used in RTK mode, floating integers 	*/
#define	SIGNAL_VALUE_GSPFIX_DEADRECKONIG_MODE			6 		/* GPS Fix Type: 6 : Estimated (dead reckoning) mode 									*/
#define	SIGNAL_VALUE_GSPFIX_MANUALINPUTMODE				7 		/* GPS Fix Type: 7 :�}�j���A�����̓��[�h 												*/
#define	SIGNAL_VALUE_GSPFIX_SIMULATORMODE				8 		/* GPS Fix Type: 8 :�V�~�����[�V�������[�h												*/


/* �n����R�����˗ʐώZ�l�o�͕���\ */
#define	SIGNAL_VALUE_FUEL_CONSUMPTION_RESOLUTION	0.0001	/* �n����R�����˗ʐώZ�l����\								*/
/* �n���㑖�s�����ώZ�l�o�͕���\ */
#define	SIGNAL_VALUE_RUNNING_DISTANCE_RESOLUTION	0.01	/* �n���㑖�s�����ώZ�l����\								*/
/* �\���p�ԑ��o�͕���\ */
#define	SIGNAL_VALUE_VEHICLESPEED_DISP_RESOLUTION	0.01	/*	�\���p�ԑ��o�͕���\									*/
/* �S�֎ԑ��o�͕���\ */
#define	SIGNAL_VALUE_WHEELSPEED_RESOLUTION		0.01		/* �S�֎ԑ��o�͕���\										*/
/* �S�֎ԑ��o�̓I�t�Z�b�g */
#define	SIGNAL_VALUE_WHEELSPEED_OFFSET			-100		/* �S�֎ԑ��o�̓I�t�Z�b�g									*/
/* �����x�o�͕���\ */
#define	SIGNAL_VALUE_ACCERERATION_RESOLUTION	0.01		/* �����x�o�͕���\											*/
/* �G���W�������o�̓I�t�Z�b�g */
#define	SIGNAL_VALUE_ENG_TEMP_COOLANT_OFFSET	-50			/* �G���W�������o�̓I�t�Z�b�g								*/
/* �G���W�������o�̓I�t�Z�b�g */
#define	SIGNAL_VALUE_ENG_TEMP_OIL_OFFSET		-50			/* �G���W�������o�̓I�t�Z�b�g								*/
/* �O�C���o�̓I�t�Z�b�g */
#define	SIGNAL_VALUE_OUTSIDE_TEMP_OFFSET		-60			/* �O�C���o�̓I�t�Z�b�g										*/
/* �z�C���x�o�̓I�t�Z�b�g */
#define	SIGNAL_VALUE_INTAKE_MANIFOLD_TEMP_OFFSET_ISOCAN	-40	/* �z�C���x�o�̓I�t�Z�b�g									*/
/* �z�C���o�̓I�t�Z�b�g */
#define	SIGNAL_VALUE_INTAKE_MANIFOLD_PRESS_OFFSET_ISOCAN	-100/* �z�C���o�̓I�t�Z�b�g									*/

/* GPS���x */
#define	SIGNAL_VALUE_QUALITY_IGNORE				0xFF 	/*	GPS���x: Ignore																	*/
#define	SIGNAL_VALUE_QUALITY_FIXNOTAVAILABLE	0x00 	/*	GPS���x: Fix not available or invalid											*/
#define	SIGNAL_VALUE_QUALITY_GPSSPSMODE			0x01 	/*	GPS���x: GPS SPS Mode, fix valid												*/
#define	SIGNAL_VALUE_QUALITY_DIFFERENTIALGPS	0x02 	/*	GPS���x: Differential GPS, SPS Mode, fix valid									*/
#define	SIGNAL_VALUE_QUALITY_GPSPPSMODE			0x03 	/*	GPS���x: GPS PPS Mode, fix valid												*/
#define	SIGNAL_VALUE_QUALITY_REALTIMEKINEMATIC	0x04 	/*	GPS���x: Real Time Kinematic. System used in RTK mode with fixed integers		*/
#define	SIGNAL_VALUE_QUALITY_FLOATRTK			0x05 	/*	GPS���x: Float RTK. Satellite system used in RTK mode, floating integers		*/
#define	SIGNAL_VALUE_QUALITY_DEADRECKONINGMODE	0x06 	/*	GPS���x: Estimated (dead reckoning) Mode										*/
#define	SIGNAL_VALUE_QUALITY_MANUALINPUTMODE	0x07 	/*	GPS���x: Manual Input Mode														*/
#define	SIGNAL_VALUE_QUALITY_SIMULATORMODE		0x08 	/*	GPS���x: Simulator Mode															*/

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EncodeValue(UI_16 req_id, SI_32* signal_value, SI_32 key);	/*	�Í����v�Z�l�擾										*/

SIGNAL_LOCAL UI_16 get_signal_InvalidValue(SI_32 *out);							/*																	*/
SIGNAL_LOCAL UI_16 get_signal_RandomValue(SI_32 *out);							/*	����	Random Value											*/
SIGNAL_LOCAL UI_16 get_signal_CangatewayEcuId(SI_32 *out);						/*	�ʐM�@���ʐM��	CAN-Gateway ECU ID								*/
SIGNAL_LOCAL UI_16 get_signal_VehicleNameHighOrder(SI_32 *out);					/*	�ԗ�����(�擪4����)	Vehicle Name (High-Order)					*/
SIGNAL_LOCAL UI_16 get_signal_VehicleNameLowOrder(SI_32 *out);					/*	�ԗ�����(���4����)	Vehicle Name (Low-Order)					*/
SIGNAL_LOCAL UI_16 get_signal_Destination(SI_32 *out);							/*	�d�����n	Destination											*/
SIGNAL_LOCAL UI_16 get_signal_DriverPosition(SI_32 *out);							/*	�h���C�o�[�|�W�V����	Driver Position							*/
SIGNAL_LOCAL UI_16 get_signal_EngineModelHighOrder(SI_32 *out);					/*	�G���W���^��(�擪4����)	Engine Model Code (High-Order)			*/
SIGNAL_LOCAL UI_16 get_signal_EngineModelLowOrder(SI_32 *out);					/*	�G���W���^��(���4����)	Engine Model Code (Low-Order)			*/
SIGNAL_LOCAL UI_16 get_signal_EngineCharacter(SI_32 *out);						/*	�G���W������	Engine Character								*/
SIGNAL_LOCAL UI_16 get_signal_DriveTrainCharacter(SI_32 *out);					/*	�h���C�u�g���[��	Drive Train Character						*/
SIGNAL_LOCAL UI_16 get_signal_UTCTime(SI_32 *out);								/*	UTC����	UTCTime													*/
SIGNAL_LOCAL UI_16 get_signal_Date(SI_32 *out);									/*	���t	Date													*/
SIGNAL_LOCAL UI_16 get_signal_Latitude(SI_32 *out);								/*	�ܓx	Latitude												*/
SIGNAL_LOCAL UI_16 get_signal_Longitude(SI_32 *out);								/*	�o�x	Longitude												*/
SIGNAL_LOCAL UI_16 get_signal_Altitude(SI_32 *out);								/*	���x	Altitude												*/
SIGNAL_LOCAL UI_16 get_signal_GeoidalSeparation(SI_32 *out);						/*	�W�I�C�h��	Geoidal Separation									*/
SIGNAL_LOCAL UI_16 get_signal_HorizontallySpeed(SI_32 *out);						/*	�Βn���x	Horizontally Speed									*/
SIGNAL_LOCAL UI_16 get_signal_Accuracy(SI_32 *out);								/*	���x	Accuracy 												*/
SIGNAL_LOCAL UI_16 get_signal_DOP(SI_32 *out);									/*	DOP���	DOP														*/
SIGNAL_LOCAL UI_16 get_signal_AcceleratorOperation(SI_32 *out);					/*	�A�N�Z������	Accelerator Operation							*/
SIGNAL_LOCAL UI_16 get_signal_BrakeFlag(SI_32 *out);								/*	�u���[�L����t���O	Brake Flag									*/
SIGNAL_LOCAL UI_16 get_signal_BrakeOperation(SI_32 *out);							/*	�u���[�L�����	Brake Operation									*/
SIGNAL_LOCAL UI_16 get_signal_ShiftOperation(SI_32 *out);							/*	�V�t�g����	Shift Operation										*/
SIGNAL_LOCAL UI_16 get_signal_FuelIndicator(SI_32 *out);						/*	�q���[�G���c��													*/
SIGNAL_LOCAL UI_16 get_signal_Steering(SI_32 *out);								/*	�X�e�A�����O	Steering										*/
SIGNAL_LOCAL UI_16 get_signal_Doors(SI_32 *out);								/*	�h�A	Doors													*/
SIGNAL_LOCAL UI_16 get_signal_AirCondition(SI_32 *out);							/*	�G�A�[�R���f�B�V����	Air Condition							*/
SIGNAL_LOCAL UI_16 get_signal_DrivingSupport(SI_32 *out);						/*	�h���C�r���O�T�|�[�g	Driving Support							*/
SIGNAL_LOCAL UI_16 get_signal_Blinker(SI_32 *out);								/*	�E�B���J�[	Blinker												*/
SIGNAL_LOCAL UI_16 get_signal_SittingandSeatBeltStatus(SI_32 *out);				/*	�V�[�g�A�V�[�g�x���g	Sitting and SeatBelt Status				*/
SIGNAL_LOCAL UI_16 get_signal_LampStatus(SI_32 *out);								/*	�����v	Lamp Status												*/
SIGNAL_LOCAL UI_16 get_signal_Power(SI_32 *out);								/*	�N��	Power													*/
SIGNAL_LOCAL UI_16 get_signal_EngineRev(SI_32 *out);								/*	�G���W���X�s�[�h	Engine Rev									*/
SIGNAL_LOCAL UI_16 get_signal_VehicleSpeed(SI_32 *out);							/*	�ԑ�	Vehicle Speed											*/
SIGNAL_LOCAL UI_16 get_signal_HighResolutionSpeedFront(SI_32 *out);				/*	�����x�ԑ�(�t�����g)	High Resolution Speed (Front)			*/
SIGNAL_LOCAL UI_16 get_signal_HighResolutionSpeedRear(SI_32 *out);				/*	�����x�ԑ�(���A)	High Resolution Speed (Rear)				*/
SIGNAL_LOCAL UI_16 get_signal_AccelerationPlane(SI_32 *out);						/*	�����x(����)	Acceleration(Plane)								*/
SIGNAL_LOCAL UI_16 get_signal_YawandPitchRate(SI_32 *out);						/*	���[���[�g�A�s�b�`���[�g	Yaw and Pitch Rate					*/
SIGNAL_LOCAL UI_16 get_signal_UnitTemperature(SI_32 *out);						/*	���j�b�g���x	Unit Temperature								*/	
SIGNAL_LOCAL UI_16 get_signal_TirePressureFront(SI_32 *out);					/*	�t�����g�^�C����	Tire Pressure (Front)						*/
SIGNAL_LOCAL UI_16 get_signal_TirePressureRear(SI_32 *out);						/*	���A�^�C����	Tire Pressure (Rear)							*/
SIGNAL_LOCAL UI_16 get_signal_FuelConsumption(SI_32 *out);						/*	�n����R�����˗ʐώZ�l	Fuel Consumption						*/
SIGNAL_LOCAL UI_16 get_signal_DistancefromRestart(SI_32 *out);					/*	�n���㑖�s����	Distance from Restart							*/
SIGNAL_LOCAL UI_16 get_signal_OutputTorque(SI_32 *out);							/*	�o�̓g���N	Output Torque										*/
SIGNAL_LOCAL UI_16 get_signal_EngineSensor(SI_32 *out);							/*	�G���W���Z���T�[	Engine Sensor								*/
SIGNAL_LOCAL UI_16 get_signal_AtmosphericPressure(SI_32 *out);					/*	��C��	Atmospheric pressure									*/
SIGNAL_LOCAL UI_16 get_signal_EngineRevRawValue(SI_32 *out);					/*	�G���W����]���i���l�j	Engine Rev (Raw Value)					*/
SIGNAL_LOCAL UI_16 get_signal_OutputUpDwnTorque(SI_32 *out);					/*	�o�̓A�b�v�_�E���g���N	Output Torque UpDwon					*/

#ifdef ADVANCE_ENABLE
SIGNAL_LOCAL UI_16 get_signal_IntakeManifoldPressure(SI_32 *out);					/*	�z�C��	Intake Manifold Pressure								*/
SIGNAL_LOCAL UI_16 get_signal_EngineOilPressure(SI_32 *out);						/*	�G���W������	Engine Oil Pressure								*/
SIGNAL_LOCAL UI_16 get_signal_FuelPressure(SI_32 *out);							/*	�R������	Fuel Pressure										*/
SIGNAL_LOCAL UI_16 get_signal_EngineCoolantTempertureExt1(SI_32 *out);			/*	�G���W�������i�g���j	Engine Coolant Temperture�iExt�j		*/
SIGNAL_LOCAL UI_16 get_signal_EngineCoolantTempertureExt2(SI_32 *out);			/*	�G���W�������i�g���j	Engine Coolant Temperture�iExt�j		*/
SIGNAL_LOCAL UI_16 get_signal_EngineOilTemperture(SI_32 *out);					/*	�G���W������	Engine Oil Temperture							*/
SIGNAL_LOCAL UI_16 get_signal_ExhaustGasTemperture(SI_32 *out);					/*	�r�C���x	Exhaust Gas Temperture								*/
SIGNAL_LOCAL UI_16 get_signal_Volts(SI_32 *out);									/*	�d��	Volts													*/
SIGNAL_LOCAL UI_16 get_signal_AirFuelRatio(SI_32 *out);							/*	��R��	Air-Fuel Ratio											*/
#endif /*ADVANCE_ENABLE*/
#ifdef	ISOCAN_ENABLE
SIGNAL_LOCAL UI_16 get_signal_IsoCanIntakeManifoldPressure(SI_32 *out);			/*	�z�C��	Intake Manifold Pressure								*/
SIGNAL_LOCAL UI_16 get_signal_IsoCanEngineCoolantTemperture(SI_32 *out);			/*	�G���W������	Engine Coolant Temperture						*/
SIGNAL_LOCAL UI_16 get_signal_IsoCanThrottle(SI_32 *out);							/*	�X���b�g���J�x	Throttle										*/
SIGNAL_LOCAL UI_16 get_signal_IsoCanIntakeTemperature(SI_32 *out);				/*	�z�C���x	IntakeTemperature									*/
SIGNAL_LOCAL UI_16 get_signal_IsoCanVehicleSpeed(SI_32 *out);						/*	�ԑ�	Vehicle Speed											*/
SIGNAL_LOCAL UI_16 get_signal_IsoCanEngineRev(SI_32 *out);						/*	�G���W����]��	Engine Rev										*/
SIGNAL_LOCAL UI_16 get_signal_IsoCanVolts(SI_32 *out);							/*	�d��	Volts													*/
#endif	/* ISOCAN_ENABLE */


/*SD�J�[�h�p�Œ�lID*/
SIGNAL_LOCAL const UI_16 signalFixedListTable_SD[] = 
{
	1	,	/*	�ʐM�@���ʐM��	CAN-Gateway ECU ID									*/
	2	,	/*	�ԗ�����(�擪4����)	Vehicle Name (High-Order)						*/
	3	,	/*	�ԗ�����(���4����)	Vehicle Name (Low-Order)						*/
	4	,	/*	�d�����n	Destination												*/
	5	,	/*	�h���C�o�[�|�W�V����	Driver Position								*/
	6	,	/*	�G���W���^��(�擪4����)	Engine Model Code (High-Order)				*/
	7	,	/*	�G���W���^��(���4����)	Engine Model Code (Low-Order)				*/
	8	,	/*	�G���W������	Engine Character									*/
	9	,	/*	�h���C�u�g���[��	Drive Train Character							*/
};

/*SD�J�[�h�p�ϐ��lID*/
SIGNAL_LOCAL const UI_16 signalValiableListTable_SD[] = 
{
	101	,	/*	UTC����	UTCTime														*/
	102	,	/*	���t	Date														*/
	103	,	/*	����	Random Value												*/
	104	,	/*	�ܓx	Latitude													*/
	105	,	/*	�o�x	Longitude													*/
	106	,	/*	���x	Altitude													*/
	107	,	/*	�W�I�C�h��	Geoidal Separation										*/
	108	,	/*	�Βn���x	Horizontally Speed										*/
	110	,	/*	����	Random Value												*/
	111	,	/*	���x	Accuracy 													*/
	112	,	/*	DOP���	DOP															*/
	201	,	/*	�A�N�Z������	Accelerator Operation								*/
	202	,	/*	�u���[�L����t���O	Brake Flag										*/
	203	,	/*	�u���[�L�����	Brake Operation										*/
	204	,	/*	�V�t�g����	Shift Operation											*/
	207	,	/*	����	Random Value												*/
	208	,	/*	�X�e�A�����O	Steering											*/
	209	,	/*	�E�B���J�[	Blinker													*/
	211	,	/*	����	Random Value												*/
	212	,	/*	�G�A�[�R���f�B�V����	Air Condition								*/
	214	,	/*	�h���C�r���O�T�|�[�g	Driving Support								*/
	215	,	/*	�h�A	Doors														*/
	218	,	/*	�V�[�g�A�V�[�g�x���g	Sitting and Seat-belt Status				*/
	219	,	/*	�����v	Lamp Status													*/
	220	,	/*	�N��	Power														*/
	221	,	/*	�G���W���X�s�[�h	Engine Rev										*/
	222	,	/*	�ԑ�	Vehicle Speed												*/
	223	,	/*	�����x�ԑ�(�t�����g)	High Resolution Speed (Front)				*/
	224	,	/*	�����x�ԑ�(���A)	High Resolution Speed (Rear)					*/
	225	,	/*	�����x(����)	Acceleration(Plane)									*/
	227	,	/*	���[���[�g�A�s�b�`���[�g	Yaw and Pitch Rate						*/
	231	,	/*	���j�b�g���x	Unit Temperature									*/
	232	,	/*	�t�����g�^�C����	Tire Pressure (Front)							*/
	233	,	/*	���A�^�C����	Tire Pressure (Rear)								*/
	235	,	/*	�n����R�����˗ʐώZ�l	Fuel Consumption							*/
	240	,	/*	�n���㑖�s����	Distance from Restart								*/
	253	,	/*	�o�̓g���N	Output Torque											*/
	254	,	/*	�G���W���Z���T�[	Engine Sensor									*/
	259	,	/*	�o�̓A�b�v�_�E���g���N	Output Torque UpDwon						*/
	260	,	/*	��C��	Atmospheric pressure										*/
	270	,	/*	�G���W����]���i���l�jEngine Rev(Raw Value)							*/
#ifdef ISOCAN_ENABLE
	640	,	/*	�ԑ�	Vehicle Speed												*/
	641	,	/*	�G���W����]��	Engine Rev											*/
	642	,	/*	�z�C��	Intake Manifold Pressure									*/
	643	,	/*	�G���W������	Engine Coolant Temperture							*/
	644	,	/*	�d��	Volts														*/
	645	,	/*	�X���b�g���J�x	Throttle											*/
	646	,	/*	�z�C���x	IntakeTemperature										*/
#endif /* ISOCAN_ENABLE */
};

/*Bluetooth�p�Œ�lID*/
SIGNAL_LOCAL const UI_16 signalFixedListTable_BT[] = 
{
	1	,	/*	�ʐM�@���ʐM��	CAN-Gateway ECU ID									*/
	2	,	/*	�ԗ�����(�擪4����)	Vehicle Name (High-Order)						*/
	3	,	/*	�ԗ�����(���4����)	Vehicle Name (Low-Order)						*/
	4	,	/*	�d�����n	Destination												*/
	5	,	/*	�h���C�o�[�|�W�V����	Driver Position								*/
	6	,	/*	�G���W���^��(�擪4����)	Engine Model Code (High-Order)				*/
	7	,	/*	�G���W���^��(���4����)	Engine Model Code (Low-Order)				*/
	8	,	/*	�G���W������	Engine Character									*/
	9	,	/*	�h���C�u�g���[��	Drive Train Character							*/
};

/*Bluetooth�p�ϐ��lID*/
SIGNAL_LOCAL const UI_16 signalValiableListTable_BT[] = 
{
	101	,	/*	UTC����	UTCTime														*/
	102	,	/*	���t	Date														*/
	103	,	/*	����	Random Value												*/
	104	,	/*	�ܓx	Latitude													*/
	105	,	/*	�o�x	Longitude													*/
	106	,	/*	���x	Altitude													*/
	107	,	/*	�W�I�C�h��	Geoidal Separation										*/
	108	,	/*	�Βn���x	Horizontally Speed										*/
	110	,	/*	����	Random Value												*/
	111	,	/*	���x	Accuracy 													*/
	112	,	/*	DOP���	DOP															*/
	201	,	/*	�A�N�Z������	Accelerator Operation								*/
	202	,	/*	�u���[�L����t���O	Brake Flag										*/
	203	,	/*	�u���[�L�����	Brake Operation										*/
	204	,	/*	�V�t�g����	Shift Operation											*/
	207	,	/*	����	Random Value												*/
	208	,	/*	�X�e�A�����O	Steering											*/
	209	,	/*	�E�B���J�[	Blinker													*/
	211	,	/*	����	Random Value												*/
	212	,	/*	�G�A�[�R���f�B�V����	Air Condition								*/
	214	,	/*	�h���C�r���O�T�|�[�g	Driving Support								*/
	215	,	/*	�h�A	Doors														*/
	218	,	/*	�V�[�g�A�V�[�g�x���g	Sitting and Seat-belt Status				*/
	219	,	/*	�����v	Lamp Status													*/
	220	,	/*	�N��	Power														*/
	221	,	/*	�G���W���X�s�[�h	Engine Rev										*/
	222	,	/*	�ԑ�	Vehicle Speed												*/
	223	,	/*	�����x�ԑ�(�t�����g)	High Resolution Speed (Front)				*/
	224	,	/*	�����x�ԑ�(���A)	High Resolution Speed (Rear)					*/
	225	,	/*	�����x(����)	Acceleration(Plane)									*/
	227	,	/*	���[���[�g�A�s�b�`���[�g	Yaw and Pitch Rate						*/
	231	,	/*	���j�b�g���x	Unit Temperature									*/
	232	,	/*	�t�����g�^�C����	Tire Pressure (Front)							*/
	233	,	/*	���A�^�C����	Tire Pressure (Rear)								*/
	235	,	/*	�n����R�����˗ʐώZ�l	Fuel Consumption							*/
	240	,	/*	�n���㑖�s����	Distance from Restart								*/
	253	,	/*	�o�̓g���N	Output Torque											*/
	254	,	/*	�G���W���Z���T�[	Engine Sensor									*/
	259	,	/*	�o�̓A�b�v�_�E���g���N	Output Torque UpDwon						*/
	260	,	/*	��C��	Atmospheric pressure										*/
	270	,	/*	�G���W����]���i���l�jEngine Rev(Raw Value)							*/
#ifdef ADVANCE_ENABLE
	601	,	/*	�z�C��	Intake Manifold Pressure									*/
	602	,	/*	�G���W������	Engine Oil Pressure									*/
	603	,	/*	�R������	Fuel Pressure											*/
	604	,	/*	�G���W�������i�g���j	Engine Coolant Temperture�iExt�j			*/
	605	,	/*	�G���W�������i�g���j	Engine Coolant Temperture�iExt�j			*/
	606	,	/*	�G���W������	Engine Oil Temperture								*/
	607	,	/*	�r�C���x	Exhaust Gas Temperture									*/
	608	,	/*	�d��	Volts														*/
	609	,	/*	��R��	Air-Fuel Ratio												*/
#endif /*ADVANCE_ENABLE*/
#ifdef ISOCAN_ENABLE
	640	,	/*	�ԑ�	Vehicle Speed												*/
	641	,	/*	�G���W����]��	Engine Rev											*/
	642	,	/*	�z�C��	Intake Manifold Pressure									*/
	643	,	/*	�G���W������	Engine Coolant Temperture							*/
	644	,	/*	�d��	Volts														*/
	645	,	/*	�X���b�g���J�x	Throttle											*/
	646	,	/*	�z�C���x	IntakeTemperature										*/
#endif /* ISOCAN_ENABLE */
};

typedef UI_16 (* T_SignalInfoFunc) (SI_32 *out);

/*���J�M���擾�p�֐��e�[�u��*/
SIGNAL_LOCAL const T_SignalInfoFunc C_SignalInfoFuncTbl[SIGNAL_INFO_FUNC_NUM][2] = {
	/*	BT											SD											�d�l�Ŋ���U���Ă������						*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	0																	*/
	{	&get_signal_CangatewayEcuId				,	&get_signal_CangatewayEcuId				/*	&get_signal_CangatewayEcuId						*/,},/*	1	�ʐM�@���ʐM��	CAN-Gateway ECU ID								*/
	{	&get_signal_VehicleNameHighOrder		,	&get_signal_VehicleNameHighOrder		/*	&get_signal_VehicleNameHighOrder				*/,},/*	2	�ԗ�����(�擪4����)	Vehicle Name (High-Order)					*/
	{	&get_signal_VehicleNameLowOrder			,	&get_signal_VehicleNameLowOrder			/*	&get_signal_VehicleNameLowOrder					*/,},/*	3	�ԗ�����(���4����)	Vehicle Name (Low-Order)					*/
	{	&get_signal_Destination					,	&get_signal_Destination					/*	&get_signal_Destination							*/,},/*	4	�d�����n	Destination											*/
	{	&get_signal_DriverPosition				,	&get_signal_DriverPosition				/*	&get_signal_DriverPosition						*/,},/*	5	�h���C�o�[�|�W�V����	Driver Position							*/
	{	&get_signal_EngineModelHighOrder		,	&get_signal_EngineModelHighOrder		/*	&get_signal_EngineModelHighOrder				*/,},/*	6	�G���W���^��(�擪4����)	Engine Model Code (High-Order)			*/
	{	&get_signal_EngineModelLowOrder			,	&get_signal_EngineModelLowOrder			/*	&get_signal_EngineModelLowOrder					*/,},/*	7	�G���W���^��(���4����)	Engine Model Code (Low-Order)			*/
	{	&get_signal_EngineCharacter				,	&get_signal_EngineCharacter				/*	&get_signal_EngineCharacter						*/,},/*	8	�G���W������	Engine Character								*/
	{	&get_signal_DriveTrainCharacter			,	&get_signal_DriveTrainCharacter			/*	&get_signal_DriveTrainCharacter					*/,},/*	9	�h���C�u�g���[��	Drive Train Character						*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_SeatSetting							*/,},/*	10	�V�[�g�ݒ�	Seat Setting										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	11																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	12																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	13																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	14	����	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	15																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	16																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	17																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	18	����	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	19																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	20																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	21																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	22																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	23																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	24																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	25	����	Random Value											*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	50	����	Random Value											*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	83	����	Random Value											*/
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
	{	&get_signal_UTCTime						,	&get_signal_UTCTime						/*	&get_signal_UTCTime								*/,},/*	101	UTC����	UTCTime													*/
	{	&get_signal_Date						,	&get_signal_Date						/*	&get_signal_Date								*/,},/*	102	���t	Date													*/
	{	&get_signal_RandomValue					,	&get_signal_RandomValue					/*	&get_signal_RandomValue							*/,},/*	103	����	Random Value											*/
	{	&get_signal_Latitude					,	&get_signal_Latitude					/*	&get_signal_Latitude							*/,},/*	104	�ܓx	Latitude												*/
	{	&get_signal_Longitude					,	&get_signal_Longitude					/*	&get_signal_Longitude							*/,},/*	105	�o�x	Longitude												*/
	{	&get_signal_Altitude					,	&get_signal_Altitude					/*	&get_signal_Altitude							*/,},/*	106	���x	Altitude												*/
	{	&get_signal_GeoidalSeparation			,	&get_signal_GeoidalSeparation			/*	&get_signal_GeoidalSeparation					*/,},/*	107	�W�I�C�h��	Geoidal Separation									*/
	{	&get_signal_HorizontallySpeed			,	&get_signal_HorizontallySpeed			/*	&get_signal_HorizontallySpeed					*/,},/*	108	�Βn���x	Horizontally Speed									*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_VerticalSpeed						*/,},/*	109	�������x	Vertical Speed										*/
	{	&get_signal_RandomValue					,	&get_signal_RandomValue					/*	&get_signal_RandomValue							*/,},/*	110	����	Random Value											*/
	{	&get_signal_Accuracy					,	&get_signal_Accuracy					/*	&get_signal_Accuracy							*/,},/*	111	���x	Accuracy 												*/
	{	&get_signal_DOP							,	&get_signal_DOP							/*	&get_signal_DOP									*/,},/*	112	DOP���	DOP														*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	113	�����x	Acceleration(Plane)										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	114	�����x	Acceleration(Vertical)									*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	115	���[���[�g&�s�b�`���[�g	Yaw Rate & Pitch Rate					*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	116	���[�����[�g	Roll Rate										*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	134	����	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	135																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	136																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	137																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	138																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	139																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	140																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	141																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	142	����	Random Value											*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	188	����	Random Value											*/
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
	{	&get_signal_AcceleratorOperation		,	&get_signal_AcceleratorOperation		/*	&get_signal_AcceleratorOperation				*/,},/*	201	�A�N�Z������	Accelerator Operation							*/
	{	&get_signal_BrakeFlag					,	&get_signal_BrakeFlag					/*	&get_signal_BrakeFlag							*/,},/*	202	�u���[�L����t���O	Brake Flag									*/
	{	&get_signal_BrakeOperation				,	&get_signal_BrakeOperation				/*	&get_signal_BrakeOperation						*/,},/*	203	�u���[�L�����	Brake Operation									*/
	{	&get_signal_ShiftOperation				,	&get_signal_ShiftOperation				/*	&get_signal_ShiftOperation						*/,},/*	204	�V�t�g����	Shift Operation										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_ClutchPedalOperation				*/,},/*	205	�N���b�`����	Clutch Pedal Operation							*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_HybridPowerSelect					*/,},/*	206	�n�C�u���b�h�p���[�I��	Hybrid Power Select						*/
	{	&get_signal_FuelIndicator				,	&get_signal_FuelIndicator				/*	&get_signal_RandomValue							*/,},/*	207	�q���[�G���c��													*/
	{	&get_signal_Steering					,	&get_signal_Steering					/*	&get_signal_Steering							*/,},/*	208	�X�e�A�����O	Steering										*/
	{	&get_signal_Blinker						,	&get_signal_Blinker						/*	&get_signal_Blinker								*/,},/*	209	�E�B���J�[	Blinker												*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_Wiper								*/,},/*	210	���C�p�[	Wiper												*/
	{	&get_signal_RandomValue					,	&get_signal_RandomValue					/*	&get_signal_RandomValue							*/,},/*	211	����	Random Value											*/
	{	&get_signal_AirCondition				,	&get_signal_AirCondition				/*	&get_signal_AirCondition						*/,},/*	212	�G�A�[�R���f�B�V����	Air Condition							*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_HeaterControl						*/,},/*	213	�q�[�^�[�R���g���[��	Heater Control							*/
	{	&get_signal_DrivingSupport				,	&get_signal_DrivingSupport				/*	&get_signal_DrivingSupport						*/,},/*	214	�h���C�r���O�T�|�[�g	Driving Support							*/
	{	&get_signal_Doors						,	&get_signal_Doors						/*	&get_signal_Doors								*/,},/*	215	�h�A	Doors													*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_Lock								*/,},/*	216	���b�N	Lock													*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_Window								*/,},/*	217	�E�B���h�E	Window												*/
	{	&get_signal_SittingandSeatBeltStatus	,	&get_signal_SittingandSeatBeltStatus	/*	&get_signal_SittingandSeatBeltStatus			*/,},/*	218	�V�[�g�A�V�[�g�x���g	Sitting and SeatBelt Status				*/
	{	&get_signal_LampStatus					,	&get_signal_LampStatus					/*	&get_signal_LampStatus							*/,},/*	219	�����v	Lamp Status												*/
	{	&get_signal_Power						,	&get_signal_Power						/*	&get_signal_Power								*/,},/*	220	�N��	Power													*/
	{	&get_signal_EngineRev					,	&get_signal_EngineRev					/*	&get_signal_EngineRev							*/,},/*	221	�G���W���X�s�[�h	Engine Rev									*/
	{	&get_signal_VehicleSpeed				,	&get_signal_VehicleSpeed				/*	&get_signal_VehicleSpeed						*/,},/*	222	�ԑ�	Vehicle Speed											*/
	{	&get_signal_HighResolutionSpeedFront	,	&get_signal_HighResolutionSpeedFront	/*	&get_signal_HighResolutionSpeedFront			*/,},/*	223	�����x�ԑ�(�t�����g)	High Resolution Speed (Front)			*/
	{	&get_signal_HighResolutionSpeedRear		,	&get_signal_HighResolutionSpeedRear		/*	&get_signal_HighResolutionSpeedRear				*/,},/*	224	�����x�ԑ�(���A)	High Resolution Speed (Rear)				*/
	{	&get_signal_AccelerationPlane			,	&get_signal_AccelerationPlane			/*	&get_signal_AccelerationPlane					*/,},/*	225	�����x(����)	Acceleration(Plane)								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_AccelerationVertical				*/,},/*	226	�����x(����)	Acceleration(Vertical)							*/
	{	&get_signal_YawandPitchRate				,	&get_signal_YawandPitchRate				/*	&get_signal_YawandPitchRate						*/,},/*	227	���[���[�g�A�s�b�`���[�g	Yaw and Pitch Rate					*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RollRate							*/,},/*	228	���[�����[�g	Roll Rate										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_YawandPitchAngle					*/,},/*	229	���[�A�s�b�`�p	Yaw and Pitch Angle								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RollAngle							*/,},/*	230	���[���p	Roll Angle											*/
	{	&get_signal_UnitTemperature				,	&get_signal_UnitTemperature				/*	&get_signal_UnitTemperature						*/,},/*	231	���j�b�g���x	Unit Temperature								*/
	{	&get_signal_TirePressureFront			,	&get_signal_TirePressureFront			/*	&get_signal_TirePressureFront					*/,},/*	232	�t�����g�^�C����	Tire Pressure (Front)						*/
	{	&get_signal_TirePressureRear			,	&get_signal_TirePressureRear			/*	&get_signal_TirePressureRear					*/,},/*	233	���A�^�C����	Tire Pressure (Rear)							*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_EnergyIndicator						*/,},/*	234	�G�l���M�C���W�P�[�^	Energy Indicator						*/
	{	&get_signal_FuelConsumption				,	&get_signal_FuelConsumption				/*	&get_signal_FuelConsumption						*/,},/*	235	�n����R�����˗ʐώZ�l	Fuel Consumption						*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_ElectricityConsumptionsincerestart	*/,},/*	236	�n����d�͏���ʐώZ�l	Electricity Consumption (since restart)	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_GaseousFuelConsumption				*/,},/*	237	�n����C�̔R�����˗ʐώZ�l	Gaseous Fuel Consumption			*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_HyblidDriveMode						*/,},/*	238	�n�C�u���b�h���s���	Hyblid Drive Mode						*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_Odometer							*/,},/*	239	ODO���[�^���s����	Odometer									*/
	{	&get_signal_DistancefromRestart			,	&get_signal_DistancefromRestart			/*	&get_signal_DistancefromRestart					*/,},/*	240	�n���㑖�s����	Distance from Restart							*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DistanceofEngineDrive				*/,},/*	241	�G���W�����s����	Distance of Engine Drive					*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DistanceofEVDrive					*/,},/*	242	EV���s����	Distance of EV Drive								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DistanceofGaseousFuel				*/,},/*	243	�K�X�G���W�����s����	Distance of Gaseous Fuel				*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DrivableDistanceALL					*/,},/*	244	���s�\����	Drivable Distance(ALL)							*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DrivableDistanceofFuel				*/,},/*	245	�G���W�����s�\����	Drivable Distance of Fuel				*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DrivableDistanceofElectricity		*/,},/*	246	EV���s�\����	Drivable Distance of Electricity				*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DrivableDistanceofGaseousFuel		*/,},/*	247	�K�X�G���W�����s�\����	Drivable Distance of Gaseous Fuel	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_ChargingLidStatus					*/,},/*	248	�������J���	Charging Lid Status								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_ChargingPlugStatus					*/,},/*	249	�[�d�P�[�u���ڑ����	Charging Lid Status						*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_ChargingandSupplyingStatus			*/,},/*	250	�[���d���	Charging and Supplying Status						*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RaderSignal							*/,},/*	251	�ŋߐڏ�Q������	Rader Signal								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_MinimumTimetoCollision				*/,},/*	252	�Őڋߏ�Q���Փˎ���	Minimum Time to Collision 				*/
	{	&get_signal_OutputTorque				,	&get_signal_OutputTorque				/*	&get_signal_OutputTorque						*/,},/*	253	�o�̓g���N	Output Torque										*/
	{	&get_signal_EngineSensor				,	&get_signal_EngineSensor				/*	&get_signal_InvalidValue						*/,},/*	254 �G���W���Z���T�[ Engine Sensor									*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	255																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	256																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	257																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	258																	*/
	{	&get_signal_OutputUpDwnTorque			,	&get_signal_OutputUpDwnTorque			/*	&get_signal_OutputUpDwnTorque					*/,},/*	259	�o�̓A�b�v�_�E���g���N	Output Torque UpDwon					*/
	{	&get_signal_AtmosphericPressure			,	&get_signal_AtmosphericPressure			/*	&get_signal_InvalidValue						*/,},/*	260 ��C��	Atmospheric pressure									*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	261																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	262																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	263																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	264																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	265																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	266																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	267																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	268																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	269																	*/
	{	&get_signal_EngineRevRawValue			,	&get_signal_EngineRevRawValue			/*	&get_signal_InvalidValue						*/,},/*	270 �G���W����]��(���l)	Engine Rev (Raw Value)					*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	284	����	Random Value											*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	304	����	Random Value											*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	337	����	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	338																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	339																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	340	����	Random Value											*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	356	����	Random Value											*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	370	����	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	371	����	Random Value											*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	399	����	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	400																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	401																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	402																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	403	����	Random Value											*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	422	����	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	423																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	424																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	425																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	426																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	427																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	428																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	429																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	430	����	Random Value											*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	455	����	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	456																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	457																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	458																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	459																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	460																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	461																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	462																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	463																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	464																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	465	����	Random Value											*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_SOCSegmentPrius						*/,},/*	501	SOC�Z�O�����g PRIUS for PriusCUP for TMA	SOC(Segment)		*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_SOCSegment748A						*/,},/*	502	SOC�Z�O�����g 748A��	SOC(Segment)							*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_DiagnosticCode						*/,},/*	503	�_�C�A�O�R�[�h	Diagnostic Code									*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_Warning								*/,},/*	504	���[�j���O	 Warning											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_PlugErr								*/,},/*	505	�v���O�G���[	Plug Err										*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	537	����	Random Value											*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	557	����	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	558																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	559																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	560																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	561																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	562																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	563	����	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	564																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	565	����	Random Value											*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	594	����	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	595																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	596																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	597																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	598																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	599																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	600																	*/
#ifdef ADVANCE_ENABLE
	{	&get_signal_IntakeManifoldPressure		,	&get_signal_InvalidValue				/*	&get_signal_IntakeManifoldPressure				*/,},/*	601	�z�C��	Intake Manifold Pressure								*/
	{	&get_signal_EngineOilPressure			,	&get_signal_InvalidValue				/*	&get_signal_EngineOilPressure					*/,},/*	602	�G���W������	Engine Oil Pressure								*/
	{	&get_signal_FuelPressure				,	&get_signal_InvalidValue				/*	&get_signal_FuelPressure						*/,},/*	603	�R������	Fuel Pressure										*/
	{	&get_signal_EngineCoolantTempertureExt1	,	&get_signal_InvalidValue				/*	&get_signal_EngineCoolantTempertureExt1			*/,},/*	604	�G���W�������i�g���j	Engine Coolant Temperture�iExt�j		*/
	{	&get_signal_EngineCoolantTempertureExt2	,	&get_signal_InvalidValue				/*	&get_signal_EngineCoolantTempertureExt2			*/,},/*	605	�G���W�������i�g���j	Engine Coolant Temperture�iExt�j		*/
	{	&get_signal_EngineOilTemperture			,	&get_signal_InvalidValue				/*	&get_signal_EngineOilTemperture					*/,},/*	606	�G���W������	Engine Oil Temperture							*/
	{	&get_signal_ExhaustGasTemperture		,	&get_signal_InvalidValue				/*	&get_signal_ExhaustGasTemperture				*/,},/*	607	�r�C���x	Exhaust Gas Temperture								*/
	{	&get_signal_Volts						,	&get_signal_InvalidValue				/*	&get_signal_Volts								*/,},/*	608	�d��	Volts													*/
	{	&get_signal_AirFuelRatio				,	&get_signal_InvalidValue				/*	&get_signal_AirFuelRatio						*/,},/*	609	��R��	Air-Fuel Ratio											*/
#else /*ADVANCE_ENABLE*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IntakeManifoldPressure				*/,},/*	601	�z�C��	Intake Manifold Pressure								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_EngineOilPressure					*/,},/*	602	�G���W������	Engine Oil Pressure								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_FuelPressure						*/,},/*	603	�R������	Fuel Pressure										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_EngineCoolantTempertureExt1			*/,},/*	604	�G���W�������i�g���j	Engine Coolant Temperture�iExt�j		*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_EngineCoolantTempertureExt2			*/,},/*	605	�G���W�������i�g���j	Engine Coolant Temperture�iExt�j		*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_EngineOilTemperture					*/,},/*	606	�G���W������	Engine Oil Temperture							*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_ExhaustGasTemperture				*/,},/*	607	�r�C���x	Exhaust Gas Temperture								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_Volts								*/,},/*	608	�d��	Volts													*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_AirFuelRatio						*/,},/*	609	��R��	Air-Fuel Ratio											*/
#endif /*ADVANCE_ENABLE*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	610																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	611																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	612																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	613																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	614																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	615																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	616	����	Random Value											*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	636	����	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	637																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	638																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	639																	*/
#ifdef ISOCAN_ENABLE
	{	&get_signal_IsoCanVehicleSpeed			,	&get_signal_IsoCanVehicleSpeed			/*	&get_signal_IsoCanVehicleSpeed					*/,},/*	640	�ԑ�	Vehicle Speed											*/
	{	&get_signal_IsoCanEngineRev				,	&get_signal_IsoCanEngineRev				/*	&get_signal_IsoCanEngineRev						*/,},/*	641	�G���W����]��	Engine Rev										*/
	{	&get_signal_IsoCanIntakeManifoldPressure,	&get_signal_IsoCanIntakeManifoldPressure/*	&get_signal_IsoCanIntakeManifoldPressure		*/,},/*	642	�z�C��	Intake Manifold Pressure								*/
	{	&get_signal_IsoCanEngineCoolantTemperture,	&get_signal_IsoCanEngineCoolantTemperture/*	&get_signal_IsoCanEngineCoolantTemperture		*/,},/*	643	�G���W������	Engine Coolant Temperture						*/
	{	&get_signal_IsoCanVolts					,	&get_signal_IsoCanVolts					/*	&get_signal_IsoCanVolts							*/,},/*	644	�d��	Volts													*/
	{	&get_signal_IsoCanThrottle				,	&get_signal_IsoCanThrottle				/*	&get_signal_IsoCanThrottle						*/,},/*	645	�X���b�g���J�x	Throttle										*/
	{	&get_signal_IsoCanIntakeTemperature		,	&get_signal_IsoCanIntakeTemperature		/*	&get_signal_IsoCanIntakeTemperature				*/,},/*	646	�z�C���x	IntakeTemperature									*/
#else	/* ISOCAN_ENABLE */
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IsoCanVehicleSpeed					*/,},/*	640	�ԑ�	Vehicle Speed											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IsoCanEngineRev						*/,},/*	641	�G���W����]��	Engine Rev										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IsoCanIntakeManifoldPressure		*/,},/*	642	�z�C��	Intake Manifold Pressure								*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IsoCanEngineCoolantTemperture		*/,},/*	643	�G���W������	Engine Coolant Temperture						*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IsoCanVolts							*/,},/*	644	�d��	Volts													*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IsoCanThrottle						*/,},/*	645	�X���b�g���J�x	Throttle										*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_IsoCanIntakeTemperature				*/,},/*	646	�z�C���x	IntakeTemperature									*/
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
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	672	����	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	673	����	Random Value											*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	674																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	675																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	676																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	677																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	678																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	679																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	680																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_InvalidValue						*/,},/*	681																	*/
	{	&get_signal_InvalidValue				,	&get_signal_InvalidValue				/*	&get_signal_RandomValue							*/,},/*	682	����	Random Value											*/
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
/*�Í����p�֐��e�[�u�� NULL�͈Í����s�v*/
SIGNAL_LOCAL const T_EncodeFunc C_EncodeFuncTbl[SIGNAL_INFO_FUNC_NUM] = {
	NULL										,/*	0																	*/
	NULL										,/*	1	�ʐM�@���ʐM��	CAN-Gateway ECU ID								*/
	NULL										,/*	2	�ԗ�����(�擪4����)	Vehicle Name (High-Order)					*/
	NULL										,/*	3	�ԗ�����(���4����)	Vehicle Name (Low-Order)					*/
	NULL										,/*	4	�d�����n	Destination											*/
	NULL										,/*	5	�h���C�o�[�|�W�V����	Driver Position							*/
	NULL										,/*	6	�G���W���^��(�擪4����)	Engine Model Code (High-Order)			*/
	NULL										,/*	7	�G���W���^��(���4����)	Engine Model Code (Low-Order)			*/
	NULL										,/*	8	�G���W������	Engine Character								*/
	NULL										,/*	9	�h���C�u�g���[��	Drive Train Character						*/
	NULL										,/*	10	�V�[�g�ݒ�	Seat Setting										*/
	NULL										,/*	11																	*/
	NULL										,/*	12																	*/
	NULL										,/*	13																	*/
	NULL										,/*	14	����	Random Value											*/
	NULL										,/*	15																	*/
	NULL										,/*	16																	*/
	NULL										,/*	17																	*/
	NULL										,/*	18	����	Random Value											*/
	NULL										,/*	19																	*/
	NULL										,/*	20																	*/
	NULL										,/*	21																	*/
	NULL										,/*	22																	*/
	NULL										,/*	23																	*/
	NULL										,/*	24																	*/
	NULL										,/*	25	����	Random Value											*/
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
	NULL										,/*	50	����	Random Value											*/
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
	NULL										,/*	83	����	Random Value											*/
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
	NULL										,/*	101	UTC����	UTCTime													*/
	NULL										,/*	102	���t	Date													*/
	NULL										,/*	103	����	Random Value											*/
	NULL										,/*	104	�ܓx	Latitude												*/
	NULL										,/*	105	�o�x	Longitude												*/
	NULL										,/*	106	���x	Altitude												*/
	NULL										,/*	107	�W�I�C�h��	Geoidal Separation									*/
	NULL										,/*	108	�Βn���x	Horizontally Speed									*/
	NULL										,/*	109	�������x	Vertical Speed										*/
	NULL										,/*	110	����	Random Value											*/
	NULL										,/*	111	���x	Accuracy 												*/
	NULL										,/*	112	DOP���	DOP														*/
	NULL										,/*	113	�����x	Acceleration(Plane)										*/
	NULL										,/*	114	�����x	Acceleration(Vertical)									*/
	NULL										,/*	115	���[���[�g&�s�b�`���[�g	Yaw Rate & Pitch Rate					*/
	NULL										,/*	116	���[�����[�g	Roll Rate										*/
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
	NULL										,/*	134	����	Random Value											*/
	NULL										,/*	135																	*/
	NULL										,/*	136																	*/
	NULL										,/*	137																	*/
	NULL										,/*	138																	*/
	NULL										,/*	139																	*/
	NULL										,/*	140																	*/
	NULL										,/*	141																	*/
	NULL										,/*	142	����	Random Value											*/
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
	NULL										,/*	188	����	Random Value											*/
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
	NULL										,/*	201	�A�N�Z������	Accelerator Operation							*/
	NULL										,/*	202	�u���[�L����t���O	Brake Flag									*/
	NULL										,/*	203	�u���[�L�����	Brake Operation									*/
	NULL										,/*	204	�V�t�g����	Shift Operation										*/
	NULL										,/*	205	�N���b�`����	Clutch Pedal Operation							*/
	NULL										,/*	206	�n�C�u���b�h�p���[�I��	Hybrid Power Select						*/
	NULL										,/*	207	����	Random Value											*/
	NULL										,/*	208	�X�e�A�����O	Steering										*/
	NULL										,/*	209	�E�B���J�[	Blinker												*/
	NULL										,/*	210	���C�p�[	Wiper												*/
	NULL										,/*	211	����	Random Value											*/
	NULL										,/*	212	�G�A�[�R���f�B�V����	Air Condition							*/
	NULL										,/*	213	�q�[�^�[�R���g���[��	Heater Control							*/
	NULL										,/*	214	�h���C�r���O�T�|�[�g	Driving Support							*/
	NULL										,/*	215	�h�A	Doors													*/
	NULL										,/*	216	���b�N	Lock													*/
	NULL										,/*	217	�E�B���h�E	Window												*/
	NULL										,/*	218	�V�[�g�A�V�[�g�x���g	Sitting and SeatBelt Status				*/
	NULL										,/*	219	�����v	Lamp Status												*/
	NULL										,/*	220	�N��	Power													*/
	NULL										,/*	221	�G���W���X�s�[�h	Engine Rev									*/
	NULL										,/*	222	�ԑ�	Vehicle Speed											*/
	&get_signal_EncodeValue						,/*	223	�����x�ԑ�(�t�����g)	High Resolution Speed (Front)			*/
	&get_signal_EncodeValue						,/*	224	�����x�ԑ�(���A)	High Resolution Speed (Rear)				*/
	NULL										,/*	225	�����x(����)	Acceleration(Plane)								*/
	NULL										,/*	226	�����x(����)	Acceleration(Vertical)							*/
	NULL										,/*	227	���[���[�g�A�s�b�`���[�g	Yaw and Pitch Rate					*/
	NULL										,/*	228	���[�����[�g	Roll Rate										*/
	NULL										,/*	229	���[�A�s�b�`�p	Yaw and Pitch Angle								*/
	NULL										,/*	230	���[���p	Roll Angle											*/
	NULL										,/*	231	���j�b�g���x	Unit Temperature								*/
	NULL										,/*	232	�t�����g�^�C����	Tire Pressure (Front)						*/
	NULL										,/*	233	���A�^�C����	Tire Pressure (Rear)							*/
	NULL										,/*	234	�G�l���M�C���W�P�[�^	Energy Indicator						*/
	NULL										,/*	235	�n����R�����˗ʐώZ�l	Fuel Consumption						*/
	NULL										,/*	236	�n����d�͏���ʐώZ�l	Electricity Consumption (since restart)	*/
	NULL										,/*	237	�n����C�̔R�����˗ʐώZ�l	Gaseous Fuel Consumption			*/
	NULL										,/*	238	�n�C�u���b�h���s���	Hyblid Drive Mode						*/
	NULL										,/*	239	ODO���[�^���s����	Odometer									*/
	NULL										,/*	240	�n���㑖�s����	Distance from Restart							*/
	NULL										,/*	241	�G���W�����s����	Distance of Engine Drive					*/
	NULL										,/*	242	EV���s����	Distance of EV Drive								*/
	NULL										,/*	243	�K�X�G���W�����s����	Distance of Gaseous Fuel				*/
	NULL										,/*	244	���s�\����	Drivable Distance(ALL)							*/
	NULL										,/*	245	�G���W�����s�\����	Drivable Distance of Fuel				*/
	NULL										,/*	246	EV���s�\����	Drivable Distance of Electricity				*/
	NULL										,/*	247	�K�X�G���W�����s�\����	Drivable Distance of Gaseous Fuel	*/
	NULL										,/*	248	�������J���	Charging Lid Status								*/
	NULL										,/*	249	�[�d�P�[�u���ڑ����	Charging Lid Status						*/
	NULL										,/*	250	�[���d���	Charging and Supplying Status						*/
	NULL										,/*	251	�ŋߐڏ�Q������	Rader Signal								*/
	NULL										,/*	252	�Őڋߏ�Q���Փˎ���	Minimum Time to Collision 				*/
	NULL										,/*	253	�o�̓g���N	Output Torque										*/
	NULL										,/*	254																	*/
	NULL										,/*	255																	*/
	NULL										,/*	256																	*/
	NULL										,/*	257																	*/
	NULL										,/*	258																	*/
	NULL										,/*	259	�o�̓A�b�v�_�E���g���N	Output Torque UpDwon					*/
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
	NULL										,/*	284	����	Random Value											*/
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
	NULL										,/*	304	����	Random Value											*/
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
	NULL										,/*	337	����	Random Value											*/
	NULL										,/*	338																	*/
	NULL										,/*	339																	*/
	NULL										,/*	340	����	Random Value											*/
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
	NULL										,/*	356	����	Random Value											*/
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
	NULL										,/*	370	����	Random Value											*/
	NULL										,/*	371	����	Random Value											*/
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
	NULL										,/*	399	����	Random Value											*/
	NULL										,/*	400																	*/
	NULL										,/*	401																	*/
	NULL										,/*	402																	*/
	NULL										,/*	403	����	Random Value											*/
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
	NULL										,/*	422	����	Random Value											*/
	NULL										,/*	423																	*/
	NULL										,/*	424																	*/
	NULL										,/*	425																	*/
	NULL										,/*	426																	*/
	NULL										,/*	427																	*/
	NULL										,/*	428																	*/
	NULL										,/*	429																	*/
	NULL										,/*	430	����	Random Value											*/
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
	NULL										,/*	455	����	Random Value											*/
	NULL										,/*	456																	*/
	NULL										,/*	457																	*/
	NULL										,/*	458																	*/
	NULL										,/*	459																	*/
	NULL										,/*	460																	*/
	NULL										,/*	461																	*/
	NULL										,/*	462																	*/
	NULL										,/*	463																	*/
	NULL										,/*	464																	*/
	NULL										,/*	465	����	Random Value											*/
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
	NULL										,/*	501	SOC�Z�O�����g PRIUS for PriusCUP for TMA	SOC(Segment)		*/
	NULL										,/*	502	SOC�Z�O�����g 748A��	SOC(Segment)							*/
	NULL										,/*	503	�_�C�A�O�R�[�h	Diagnostic Code									*/
	NULL										,/*	504	���[�j���O	 Warning											*/
	NULL										,/*	505	�v���O�G���[	Plug Err										*/
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
	NULL										,/*	537	����	Random Value											*/
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
	NULL										,/*	557	����	Random Value											*/
	NULL										,/*	558																	*/
	NULL										,/*	559																	*/
	NULL										,/*	560																	*/
	NULL										,/*	561																	*/
	NULL										,/*	562																	*/
	NULL										,/*	563	����	Random Value											*/
	NULL										,/*	564																	*/
	NULL										,/*	565	����	Random Value											*/
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
	NULL										,/*	594	����	Random Value											*/
	NULL										,/*	595																	*/
	NULL										,/*	596																	*/
	NULL										,/*	597																	*/
	NULL										,/*	598																	*/
	NULL										,/*	599																	*/
	NULL										,/*	600																	*/
	NULL										,/*	601	�z�C��	Intake Manifold Pressure								*/
	NULL										,/*	602	�G���W������	Engine Oil Pressure								*/
	NULL										,/*	603	�R������	Fuel Pressure										*/
	NULL										,/*	604	�G���W�������i�g���j	Engine Coolant Temperture�iExt�j		*/
	NULL										,/*	605	�G���W�������i�g���j	Engine Coolant Temperture�iExt�j		*/
	NULL										,/*	606	�G���W������	Engine Oil Temperture							*/
	NULL										,/*	607	�r�C���x	Exhaust Gas Temperture								*/
	NULL										,/*	608	�d��	Volts													*/
	NULL										,/*	609	��R��	Air-Fuel Ratio											*/
	NULL										,/*	610																	*/
	NULL										,/*	611																	*/
	NULL										,/*	612																	*/
	NULL										,/*	613																	*/
	NULL										,/*	614																	*/
	NULL										,/*	615																	*/
	NULL										,/*	616	����	Random Value											*/
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
	NULL										,/*	636	����	Random Value											*/
	NULL										,/*	637																	*/
	NULL										,/*	638																	*/
	NULL										,/*	639																	*/
	NULL										,/*	640	�ԑ�	Vehicle Speed											*/
	NULL										,/*	641	�G���W����]��	Engine Rev										*/
	NULL										,/*	642	�z�C��	Intake Manifold Pressure								*/
	NULL										,/*	643	�G���W������	Engine Coolant Temperture						*/
	NULL										,/*	644	�d��	Volts													*/
	NULL										,/*	645	�X���b�g���J�x	Throttle										*/
	NULL										,/*	646	�z�C���x	IntakeTemperature									*/
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
	NULL										,/*	672	����	Random Value											*/
	NULL										,/*	673	����	Random Value											*/
	NULL										,/*	674																	*/
	NULL										,/*	675																	*/
	NULL										,/*	676																	*/
	NULL										,/*	677																	*/
	NULL										,/*	678																	*/
	NULL										,/*	679																	*/
	NULL										,/*	680																	*/
	NULL										,/*	681																	*/
	NULL										,/*	682	����	Random Value											*/
	NULL										,/*	683																	*/
	NULL										,/*	684																	*/
	NULL										,/*	685																	*/
	NULL										,/*	686																	*/
	NULL										,/*	687																	*/
	NULL										,/*	688																	*/
	NULL										,/*	689																	*/
	NULL										,/*	690	�z�C��	Intake Manifold Pressure								*/
	NULL										,/*	691	�G���W������	Engine Coolant Temperture						*/
	NULL										,/*	692	�X���b�g���J�x	Throttle										*/
	NULL										,/*	693	�z�C���x	IntakeTemperature									*/
	NULL										,/*	694																	*/
	NULL										,/*	695																	*/
	NULL										,/*	696																	*/
	NULL										,/*	697																	*/
	NULL										,/*	698																	*/
	NULL										,/*	699																	*/
};



SIGNAL_LOCAL UI_8	S_RandInitialFlag;					/*����������				*/
SIGNAL_LOCAL SI_32	S_encodeKey[DEVICE_TYPE_NUM];		/*�G���R�[�h�L�[			*/


/********************************************************/
/*	�O���֐�											*/
/********************************************************/

/********************************************************/
/* �֐�����		: Signal_control_Init					*/
/* �@�\�̐���	: ���J�M�����䏉����					*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Signal_control_Init(void)
{
	UI_8 i;
	S_RandInitialFlag = 1;			/*����������*/
	for(i = 0 ; i < DEVICE_TYPE_NUM ; i++)
	{
		S_encodeKey[i] = 0;
	}
}


/********************************************************/
/* �֐�����		: Signal_control_GetSignalVersion		*/
/* �@�\�̐���	: ���J�M���o�[�W����					*/
/* ����			: void									*/
/* �߂�l		: ���J�M���o�[�W����					*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
UI_32 Signal_control_GetSignalVersion(void)
{
	E_SYS_CTRL_STATUS	sys_stat;										/* ������ */
	sys_stat = System_control_GetStatus();
	if(sys_stat == SYS_OPERATION_SIMULATION)
	{
		/*�V�~�����[�V����		*/
		/* SD����o�[�W�����擾 */
		return Sd_control_GetSignalVersion();
	}
	return SIGNAL_VERSION;
}

/********************************************************/
/* �֐�����		: Signal_control_GetSignalInfo			*/
/* �@�\�̐���	: ���J�M���擾							*/
/* ����			: (I/ )deviceType 		�f�o�C�X���	*/
/* 				: (I/ )req_id 			�M��ID			*/
/*				: ( /O)signal_value	�M���l				*/
/* �߂�l		: ����	:N_OK							*/
/*				: �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Signal_control_GetSignalInfo(E_SIGNAL_DEVICE_TYPE deviceType, UI_16 req_id, SI_32 *signal_value)
{
	SI_32 key;
	E_SYS_CTRL_STATUS	sys_stat;										/* ������ */
	E_SIGNAL_DEVICE_TYPE orgDeviceType = deviceType;
	if( (deviceType != DEVICE_TYPE_SD) && (deviceType != DEVICE_TYPE_BT) && (deviceType != DEVICE_TYPE_CAN) )
	{
		return N_NG;
	}
	if( deviceType == DEVICE_TYPE_CAN )
	{
		/*�f�o�C�X�^�C�v��CAN�Ȃ�BT�ɒu��������*/
		deviceType = DEVICE_TYPE_BT;
	}
	/* �����Ԏ擾 */
	sys_stat = System_control_GetStatus();
	if(sys_stat == SYS_OPERATION_SIMULATION)
	{
		/*�V�~�����[�V����		*/
		/* SD����M���擾 */
		return Sd_control_GetSignalsInfo(1, &req_id, signal_value);
	}
	else
	{
		/*�V�~�����[�V�����ȊO	*/
		if( (SIGNAL_ID_MIN <= req_id)  && (req_id <= SIGNAL_ID_MAX) )
		{
			/* �M��ID:�L�� */
			C_SignalInfoFuncTbl[ req_id ][ (UI_8)deviceType ](signal_value);

			if(C_EncodeFuncTbl[ req_id ] != NULL)
			{
				/* �Í����K�v(�u�������O�̈����̃f�o�C�X�^�C�v���g�p�j */
				(void)Signal_control_GetEncodeKey(orgDeviceType, &key);
				C_EncodeFuncTbl[ req_id ](req_id, signal_value, key);
			}
		}
		else
		{
			/* �M��ID:���� */
			(void)get_signal_InvalidValue(signal_value);
		}
	}
	return N_OK;
	
}


/********************************************************/
/* �֐�����		: Signal_control_GetSignalsInfo			*/
/* �@�\�̐���	: ���J�M���擾(�����M��)				*/
/* ����			: (I/ )deviceType 		�f�o�C�X���	*/
/* 				: (I/ )req_id_count	�M��ID��			*/
/* 				: (I/ )req_ids 		�M��ID�i�����j		*/
/*				: ( /O)signal_values	�M���l�i�����j	*/
/* �߂�l		: ����	:N_OK							*/
/*				: �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Signal_control_GetSignalsInfo(E_SIGNAL_DEVICE_TYPE deviceType, UI_8 req_id_count, const UI_16* req_ids, SI_32 *signal_values)
{
	UI_8 i;
	SI_32 key;
	E_SYS_CTRL_STATUS	sys_stat;										/* ������ */
	E_SIGNAL_DEVICE_TYPE orgDeviceType = deviceType;
	if( (deviceType != DEVICE_TYPE_SD) && (deviceType != DEVICE_TYPE_BT) && (deviceType != DEVICE_TYPE_CAN) )
	{
		return N_NG;
	}
	if( deviceType == DEVICE_TYPE_CAN )
	{
		/*�f�o�C�X�^�C�v��CAN�Ȃ�BT�ɒu��������*/
		deviceType = DEVICE_TYPE_BT;
	}
	/* �����Ԏ擾 */
	sys_stat = System_control_GetStatus();
	if(sys_stat == SYS_OPERATION_SIMULATION)
	{
		/*�V�~�����[�V����		*/
		/* SD����M���擾 */
		return Sd_control_GetSignalsInfo(req_id_count, req_ids, signal_values);
	}
	else
	{
		/*�V�~�����[�V�����ȊO	*/
		for( i = 0 ; i < req_id_count ; i ++ )
		{
			if( (SIGNAL_ID_MIN <= req_ids[i])  && (req_ids[i] <= SIGNAL_ID_MAX) )
			{
				/* �M��ID:�L�� */
				C_SignalInfoFuncTbl[ req_ids[i] ][ (UI_8)deviceType ](&signal_values[i]);
				
				if(C_EncodeFuncTbl[ req_ids[i] ] != NULL)
				{
					/* �Í����K�v(�u�������O�̈����̃f�o�C�X�^�C�v���g�p�j */
					(void)Signal_control_GetEncodeKey(orgDeviceType, &key);
					C_EncodeFuncTbl[ req_ids[i] ](req_ids[i], &signal_values[i], key);
				}
			}
			else
			{
				/* �M��ID:���� */
				(void)get_signal_InvalidValue(&signal_values[i]);
			}
		}
	}
	return N_OK;
}


/********************************************************/
/* �֐�����		: Signal_control_GetSignalList			*/
/* �@�\�̐���	: ���J�M��ID�ꗗ�擾					*/
/* ����			: (I/ )deviceType 		�f�o�C�X���	*/
/*				: (I/ )idtype			�M�����		*/
/*				: ( /O)signal_Num		�M����			*/
/*				: ( /O)signal_List		�M�����X�g		*/
/* �߂�l		: ����	:N_OK							*/
/*				: �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Signal_control_GetSignalList(E_SIGNAL_DEVICE_TYPE deviceType, E_SIGNAL_ID_TYPE idtype, UI_8 *signal_Num, const UI_16 **signal_List)
{
	E_SYS_CTRL_STATUS	sys_stat;										/* ������ */
	SI_8 ret = N_NG;
	if( (deviceType != DEVICE_TYPE_SD) && (deviceType != DEVICE_TYPE_BT) && (deviceType != DEVICE_TYPE_CAN) )
	{
		return N_NG;
	}
	if( deviceType == DEVICE_TYPE_CAN )
	{
		/*�f�o�C�X�^�C�v��CAN�Ȃ�BT�ɒu��������*/
		deviceType = DEVICE_TYPE_BT;
	}
	
	/* �����Ԏ擾 */
	sys_stat = System_control_GetStatus();
	if(sys_stat == SYS_OPERATION_SIMULATION)
	{
		/*�V�~�����[�V����		*/
		/* SD����M���擾 */
		return Sd_control_GetSignalList(idtype, signal_Num, signal_List);
	}
	else
	{
		/*�V�~�����[�V�����ȊO	*/
		if( deviceType == DEVICE_TYPE_SD )
		{
			/* SD */
			if( idtype == ID_TYPE_FIXED_ID )
			{
				/* �Œ�l */
				*signal_List = signalFixedListTable_SD;
				*signal_Num = sizeof(signalFixedListTable_SD) / sizeof(UI_16);
				ret = N_OK;
			}
			else if( idtype == ID_TYPE_VARIABLE_ID )
			{
				/* �ϐ��l */
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
				/* �Œ�l */
				*signal_List = signalFixedListTable_BT;
				*signal_Num = sizeof(signalFixedListTable_BT) / sizeof(UI_16);
				ret = N_OK;
			}
			else if( idtype == ID_TYPE_VARIABLE_ID )
			{
				/* �ϐ��l */
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
/* �֐�����		: Signal_control_GetRandomValue			*/
/* �@�\�̐���	: �����擾								*/
/* ����			: void									*/
/* �߂�l		: ����									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_32 Signal_control_GetRandomValue(void)
{
	SI_32 randomValue;
	(void)get_signal_RandomValue(&randomValue);
	return randomValue;
}


/********************************************************/
/* �֐�����		: Signal_control_GetEncodeKey			*/
/* �@�\�̐���	: �Í����L�[�擾						*/
/* ����			: (I/ )deviceType 		�f�o�C�X���	*/
/*				: ( /O)key				�Í����p�L�[	*/
/* �߂�l		: ����	:N_OK							*/
/*				: �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Signal_control_GetEncodeKey(E_SIGNAL_DEVICE_TYPE deviceType, SI_32 *key)
{
	E_SYS_CTRL_STATUS	sys_stat;										/* ������ */
	if( (deviceType != DEVICE_TYPE_SD) && (deviceType != DEVICE_TYPE_BT) && (deviceType != DEVICE_TYPE_CAN) )
	{
		return N_NG;
	}
	sys_stat = System_control_GetStatus();
	if(sys_stat == SYS_OPERATION_SIMULATION)
	{
		/*�V�~�����[�V����		*/
		/* SD����M���擾 */
		return Sd_control_GetEncodeKey(key);
	}
	*key = S_encodeKey[deviceType];

	return N_OK;
}


/********************************************************/
/* �֐�����		: Signal_control_SetEncodeKey			*/
/* �@�\�̐���	: �Í����L�[�ݒ�						*/
/* ����			: (I/ )deviceType 		�f�o�C�X���	*/
/*				: (I/ )key				�Í����p�L�[	*/
/* �߂�l		: ����	:N_OK							*/
/*				: �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
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
/* �֐�����		: Signal_control_isCanEnable			*/
/* �@�\�̐���	: CAN�M����M�ϔ���						*/
/* ����			: void									*/
/* �߂�l		: TRUE:��M��	 FALSE:����M			*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
B_8 Signal_control_isCanEnable(void)
{
#ifdef _VDV_APP
	T_Can_ctrl_tcan_info* pCanInfo;
	/* CAN��M�ς݂����� */
	pCanInfo = Can_control_GetTCanInfo();
	if(	(pCanInfo->en_brand_flag == 1) && 		/*�u�����h�t���O*/
		(pCanInfo->en_destination == 1) && 		/*�d��*/
		(pCanInfo->en_vehicle_info == 1) && 	/*�Ԏ���*/
		(pCanInfo->en_transmission_type == 1) )	/*�g�����X�~�b�V�������*/
	{
		/* CAN��M��*/
		return N_TRUE;
	}
	/* CAN����M */
	return N_FALSE;
#else /* _VDV_APP */
	return N_TRUE;
#endif /* _VDV_APP */
}

/********************************************************/
/* �֐�����		: Signal_control_isGnssEnable			*/
/* �@�\�̐���	: GNSS�M����M�ϔ���					*/
/* ����			: void									*/
/* �߂�l		: TRUE:��M��	 FALSE:����M			*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
B_8 Signal_control_isGnssEnable(void)
{
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo;
	/*NAV-PVT�擾*/	
	pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	/* GNSS��M�ς݂����� */
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:��M�ς� */
		if( (pGnssHnrInfo->gpsFix == GNSS_GSPFIX_2DFIX) || 
			(pGnssHnrInfo->gpsFix == GNSS_GSPFIX_3DFIX) || 
			(pGnssHnrInfo->gpsFix == GNSS_GSPFIX_GPS_AND_DEADRECKONING) ||
			(pGnssHnrInfo->gpsFix == GNSS_GSPFIX_DEADRECKONIN_ONLY)		)
		{
			/* ���� or �f�b�h���R�j���O:�����q�@ */
			/* GNSS��M�� */
			return N_TRUE;
		}
	}
	/* GNSS����M */
	return N_FALSE;
}

/********************************************************/
/* �֐�����		: get_signal_EncodeValue				*/
/* �@�\�̐���	: �Í����v�Z�l�擾						*/
/* ����			: (I/ )req_id 			�M��ID			*/
/*				: (I/O)signal_value	�M���l				*/
/*				: (I/ )key				�Í����L�[		*/
/* �߂�l		: ����	:N_OK							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
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
/* �֐�����		: get_signal_InvalidValue				*/
/* �@�\�̐���	: �����l�擾							*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_InvalidValue(SI_32 *out)
{
	*out = (SI_32)SIGNAL_VALUE_IGNORE_32;
	return 0;
}

/********************************************************/
/* �֐�����		: get_signal_RandomValue				*/
/* �@�\�̐���	: �����l�擾							*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_RandomValue(SI_32 *out)
{
	UI_32 val;
	SI_32 outVal;
	UI_8 shift;
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(S_RandInitialFlag == 1)
	{
		/* �������� */
		if(pGnssHnrInfo != NULL)
		{
			/* NAV-HNR:�L�� */
			srand((UI_16)((pGnssHnrInfo->hour * 10000) + (pGnssHnrInfo->min * 100) + pGnssHnrInfo->sec));
		}
		else
		{
			/* NAV-HNR:���� */
			srand((UI_16)Timer_ctrl_GetTime1ms());
		}
		S_RandInitialFlag = 0;
	}
	do{
		/* ���������irand�Ŏ擾�ł���l��0x7FF�܂łȂ̂łQ��擾�{�r�b�g�V�t�g������j */
		val = ((UI_32)rand() + ((UI_32)rand() << 16));
		shift = (UI_8)(rand() % 32);
		outVal = (SI_32)((val << shift) | (val >> (32 - shift)));
		/* �t�F�[���Ƃ��ė������O�Ȃ��蒼�� */
	}while(outVal == 0);
    *out = outVal;
    
	return 0;
}

/********************************************************/
/* �֐�����		: get_signal_CangatewayEcuId			*/
/* �@�\�̐���	: CANGatewayECUID�擾					*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_CangatewayEcuId(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_brand_flag == 1)
	{
		/* �u�����h�t���O:�L�� */
		if(pCanInfo->brand_flag == CAN_BRAND_FLAG_TMC)
		{
			/*	�u�����h�t���O:TMC												*/
			*out = (SI_32)(SIGNAL_VALUE_CANGATEWAYECUID | ((UI_32)SIGNAL_VALUE_VALIDFLAG_VALID << 24));
		}
		else
		{
			/*	�u�����h�t���O:���̑�											*/
			*out = (SI_32)(SIGNAL_VALUE_CANGATEWAYECUID | ((UI_32)SIGNAL_VALUE_VALIDFLAG_INVALID << 24));
		}
	}
	else
	{
		/* �u�����h�t���O:���� */
		*out = (SI_32)(SIGNAL_VALUE_CANGATEWAYECUID | ((UI_32)SIGNAL_VALUE_VALIDFLAG_INIT << 24));
	}
	return 0;
}

/********************************************************/
/* �֐�����		: get_signal_VehicleNameHighOrder		*/
/* �@�\�̐���	: �ԗ�����(�擪4����)�擾				*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_VehicleNameHighOrder(SI_32 *out)
{
	(void)memcpy(out, SIGNAL_VALUE_VEHICLENAMEHIGHORDER, 4);
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_VehicleNameLowOrder		*/
/* �@�\�̐���	: �ԗ�����(���4����)�擾				*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_VehicleNameLowOrder(SI_32 *out)
{
	(void)memcpy(out, SIGNAL_VALUE_VEHICLENAMELOWORDER, 4);
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_Destination				*/
/* �@�\�̐���	: �d�����n�擾							*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Destination(SI_32 *out)
{
	(void)memcpy(out, SIGNAL_VALUE_DESTINATION, 4);
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_DriverPosition				*/
/* �@�\�̐���	: �h���C�o�[�|�W�V�����擾				*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_DriverPosition(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();

	if(pCanInfo->en_destination == 1)
	{
		switch(pCanInfo->destination)
		{
			case CAN_DESTINATION_JP:	/*	�d����:���{����				*/
			case CAN_DESTINATION_UK:	/*	�d����:�C�M���X				*/
			case CAN_DESTINATION_TH:	/*	�d����:�^�C�E�V���K�|�[��	*/
			case CAN_DESTINATION_AU:	/*	�d����:�I�[�X�g�����A		*/
			case CAN_DESTINATION_KC:	/*	�d����:��A�t���J��			*/
				*out = (SI_32)SIGNAL_VALUE_DRIVERPOSITION_RIGHT;
				break;
			case CAN_DESTINATION_US:	/*	�d����:�č�					*/
			case CAN_DESTINATION_CA:	/*	�d����:�J�i�_				*/
			case CAN_DESTINATION_EU:	/*	�d����:���B					*/
			case CAN_DESTINATION_K4:	/*	�d����:���					*/
			case CAN_DESTINATION_KS:	/*	�d����:���ߓ�				*/
			case CAN_DESTINATION_CN:	/*	�d����:����					*/
			case CAN_DESTINATION_KR:	/*	�d����:�؍�					*/
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
/* �֐�����		: get_signal_EngineModelHighOrder		*/
/* �@�\�̐���	: �G���W���^��(�擪4����)�擾			*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineModelHighOrder(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_vehicle_info == 1)
	{
		/*�Ԏ���F�L��*/
		if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086A)
		{
			/*�Ԏ���F86A*/
			(void)memcpy(out, SIGNAL_VALUE_ENGINEMODELHIGHORDE_86A, 4);
		}
		else if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086B)
		{
			/*�Ԏ���F86B*/
			(void)memcpy(out, SIGNAL_VALUE_ENGINEMODELHIGHORDE_86B, 4);
		}
		else
		{
			/*�Ԏ���F�s��*/
			(void)memcpy(out, SIGNAL_VALUE_ENGINEMODELHIGHORDE_UNKNOWN, 4);
		}
	}
	else
	{
		/*�Ԏ���F����*/
		(void)memcpy(out, SIGNAL_VALUE_ENGINEMODELHIGHORDE_UNKNOWN, 4);
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_EngineModelHighOrder		*/
/* �@�\�̐���	: �G���W���^��(���4����)�擾			*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineModelLowOrder(SI_32 *out)
{
	(void)memcpy(out, SIGNAL_VALUE_ENGINEMODELLOWORDER, 4);
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_EngineCharacter			*/
/* �@�\�̐���	: �G���W�������擾						*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineCharacter(SI_32 *out)
{
	*out = (SI_32)SIGNAL_VALUE_ENGINECHARACTER;
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_DriveTrainCharacter		*/
/* �@�\�̐���	: �h���C�u�g���[���擾					*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_DriveTrainCharacter(SI_32 *out)
{
	UI_8 tm;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();

	if(pCanInfo->en_transmission_type == 1)
	{
		/* �g�����X�~�b�V������ʁAAT:�L�� */
		switch(pCanInfo->transmission_type)
		{
			case CAN_TRANSMISSION_ATORCVT:			/*	�g�����X�~�b�V������� 00: AT or CVT						*/
				
				tm = SIGNAL_VALUE_TRANSMISSION_STEPAUTOMATIC;
				break;
			case CAN_TRANSMISSION_MT:				/*	�g�����X�~�b�V������� 01: MT								*/
				tm = SIGNAL_VALUE_TRANSMISSION_MANUAL;
				break;
			default:
				tm = SIGNAL_VALUE_TRANSMISSION_IGNORE;
				break;
		}
	}
	else
	{
		/* �g�����X�~�b�V�������:���� */
		tm = SIGNAL_VALUE_TRANSMISSION_INITIAL;
	}
	
	*out = (SI_32)(SIGNAL_VALUE_DRIVETRAINCHARACTER | (UI_32)tm);
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_UTCTime					*/
/* �@�\�̐���	: UTC���Ԏ擾							*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_UTCTime(SI_32 *out)
{
	UI_32 utctime;
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:�L�� */
		/* �b�P�ʂ��Ɍv�Z */
		utctime = ((UI_32)pGnssHnrInfo->hour * 3600);
		utctime += ((UI_32)pGnssHnrInfo->min * 60);
		utctime += (UI_32)pGnssHnrInfo->sec;
		/* ����\���Z */
		utctime = (UI_32)((F_64)utctime / SIGNAL_VALUE_UTC_RESOLUTION);
		/* �i�m�b���o�͕���\�i�}�C�N���b�j�ɕϊ����ĉ��Z */
		if(pGnssHnrInfo->nano > 0)
		{
			/* �i�m�b��0���傫���Ȃ�v�Z */
			utctime += (UI_32)((F_64)pGnssHnrInfo->nano * (GNSS_NANOSEC_RESOLUTION / SIGNAL_VALUE_UTC_RESOLUTION));
		}
	}
	else
	{
		/* NAV-HNR:���� */
		utctime = SIGNAL_VALUE_IGNORE_32;
	}
	*out = (SI_32)utctime;

	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_Date						*/
/* �@�\�̐���	: ���t�擾								*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Date(SI_32 *out)
{
	UI_8 day;
	UI_8 month;
	UI_16 year;
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:�L�� */
		day = pGnssHnrInfo->day;
		month = pGnssHnrInfo->month;
		year = 	pGnssHnrInfo->year;
	}
	else
	{
		/* NAV-HNR:���� */
		day = SIGNAL_VALUE_IGNORE_8;
		month = SIGNAL_VALUE_IGNORE_8;
		year = SIGNAL_VALUE_IGNORE_16;
	}
	
	*out = (SI_32)((UI_32)day | ((UI_32)month << 8) | ((UI_32)year << 16));
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_Latitude					*/
/* �@�\�̐���	: �ܓx�擾								*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Latitude(SI_32 *out)
{
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:�L�� */
		*out = pGnssHnrInfo->lat;
	}
	else
	{
		/* NAV-HNR:���� */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_32_SIGNED;
	}
	return 0;
}

/********************************************************/
/* �֐�����		: get_signal_Longitude					*/
/* �@�\�̐���	: �o�x�擾								*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Longitude(SI_32 *out)
{
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:�L�� */
		*out = pGnssHnrInfo->lon;
	}
	else
	{
		/* NAV-HNR:���� */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_32_SIGNED;
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_Altitude					*/
/* �@�\�̐���	: ���x�擾								*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Altitude(SI_32 *out)
{
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:�L�� */
		*out = pGnssHnrInfo->h_msl;
	}
	else
	{
		/* NAV-HNR:���� */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_32_SIGNED;
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_GeoidalSeparation			*/
/* �@�\�̐���	: �W�I�C�h���擾						*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_GeoidalSeparation(SI_32 *out)
{
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:�L�� */
		*out = pGnssHnrInfo->height;
	}
	else
	{
		/* NAV-HNR:���� */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_32_SIGNED;
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_HorizontallySpeed			*/
/* �@�\�̐���	: �����������x�擾						*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_HorizontallySpeed(SI_32 *out)
{
	UI_16 speed;
	UI_16 cource;
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:�L�� */
		/* mm/s -> km/h :(3600 / 1000 x 1000) = 0.0036 */
		speed = (UI_16)((F_64)pGnssHnrInfo->g_speed * (0.0036 / SIGNAL_VALUE_VEHICLESPEED_RESOLUTION));
		cource = (UI_16)((F_64)pGnssHnrInfo->head_veh * (GNSS_HEADING_VEHICLE_RESOLUTION / SIGNAL_VALUE_HEADING_VEHICLE_RESOLUTION));
	}
	else
	{
		/* NAV-HNR:���� */
		speed = SIGNAL_VALUE_IGNORE_16;
		cource = SIGNAL_VALUE_IGNORE_16;
	}
	*out = (SI_32)((UI_32)speed | ((UI_32)cource << 16));
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_Accuracy					*/
/* �@�\�̐���	: �ʒu���x�擾							*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Accuracy(SI_32 *out)
{
	UI_16	accuracy;
	UI_8	quality;
	T_Gnss_ctrl_nav_posecef_info* pGnssNavPosecefInfo;
	T_Gnss_ctrl_nav_pvt_info* pGnssHnrInfo = Gnss_cntrol_GetNavPvtInfo();
	if(pGnssHnrInfo != NULL)
	{
		/* NAV-HNR:�L�� */
		if( (pGnssHnrInfo->gpsFix == GNSS_GSPFIX_2DFIX) || 
			(pGnssHnrInfo->gpsFix == GNSS_GSPFIX_3DFIX) || 
			(pGnssHnrInfo->gpsFix == GNSS_GSPFIX_GPS_AND_DEADRECKONING))
		{
			/* ���� */
			quality = SIGNAL_VALUE_GSPFIX_GPSFIX;
		}
		else if(pGnssHnrInfo->gpsFix == GNSS_GSPFIX_DEADRECKONIN_ONLY)
		{
			/* �f�b�h���R�j���O:�����q�@ */
			quality = SIGNAL_VALUE_GSPFIX_DEADRECKONIG_MODE;
		}
		else if(pGnssHnrInfo->gpsFix == GNSS_GSPFIX_NOFIX)
		{
			/* ������ */
			quality = SIGNAL_VALUE_GSPFIX_NOFIX;
		}
		else
		{
			/* ���̑� */
			quality = SIGNAL_VALUE_VALIDFLAG_IGNORE;
		}
	}
	else
	{
		/* NAV-HNR:���� */
		quality = SIGNAL_VALUE_IGNORE_8;
	}
	
	pGnssNavPosecefInfo = Gnss_cntrol_GetNavPosecefInfo();
	if(pGnssNavPosecefInfo != NULL)
	{
		/* NAV-POSECEF:�L�� */
		accuracy = (UI_16)pGnssNavPosecefInfo->p_acc;
	}
	else
	{
		/* NAV-POSECEF:���� */
		accuracy = SIGNAL_VALUE_IGNORE_16;
	}
	
	*out = (SI_32)((UI_32)accuracy | ((UI_32)quality << 16));

	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_DOP						*/
/* �@�\�̐���	: DOP�擾								*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
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
		/* NAV-DOP:�L�� */
		pdop = (UI_8)((F_64)pGnssNavDopInfo->p_dop * (GNSS_PDOP_RESOLUTION / SIGNAL_VALUE_PDOP_RESOLUTION));
		hdop = (UI_8)((F_64)pGnssNavDopInfo->h_dop * (GNSS_HDOP_RESOLUTION / SIGNAL_VALUE_HDOP_RESOLUTION));
		vdop = (UI_8)((F_64)pGnssNavDopInfo->v_dop * (GNSS_VDOP_RESOLUTION / SIGNAL_VALUE_VDOP_RESOLUTION));
	}
	else
	{
		/* NAV-DOP:���� */
		pdop = SIGNAL_VALUE_IGNORE_8;
		hdop = SIGNAL_VALUE_IGNORE_8;
		vdop = SIGNAL_VALUE_IGNORE_8;
	}

	if(pGnssNavPvtInfo != NULL)
	{
		/* NAV-PVT:�L�� */
		SatellitesNum = pGnssNavPvtInfo->num_sv;
	}
	else
	{
		/* NAV-PVT:���� */
		SatellitesNum = SIGNAL_VALUE_IGNORE_8;
	}
	
	*out = (SI_32)((UI_32)SatellitesNum | ((UI_32)pdop << 8) | ((UI_32)hdop << 16) | ((UI_32)vdop << 24));
	return 0;
}


/********************************************************/
/* �֐�����		: get_signal_AcceleratorOperation		*/
/* �@�\�̐���	: �A�N�Z������擾						*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
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
		/* �A�N�Z���J�x:�L�� */
		accelRatio = (UI_8)(((UI_16)pCanInfo->accel_ratio * 100) / 255);		/*  �P��:[100/255% = 0.392156862745098] */
	}
	else
	{
		/* �A�N�Z���J�x:���� */
		accelRatio = SIGNAL_VALUE_IGNORE_8;
	}
	
	if(pCanInfo->en_transmission_type == 1)
	{
		/* �g�����X�~�b�V�������:�L�� */
		if(pCanInfo->transmission_type == CAN_TRANSMISSION_ATORCVT)
		{
			/* AT or CVT */
			/* �X�|�[�c�����v�A�X�m�[�z�[���h�����v */
			if( (pCanInfo->en_sports_lamp == 1) && (pCanInfo->en_snow_hold_lamp == 1) )
			{
				/* �X�|�[�c�����v�A�X�m�[�z�[���h�����v:�L�� */
				sports_lamp		= pCanInfo->sports_lamp;
				snow_hold_lamp	= pCanInfo->snow_hold_lamp;
				if( (sports_lamp == CAN_SPORTLAMP_TURN_OFF) && (snow_hold_lamp == CAN_SNOWLAMP_OFF) )
				{
					/* �X�|�[�c�����v:OFF�A�X�m�[�z�[���h�����v:OFF */
					accelChar = SIGNAL_VALUE_ACCEL_CHARACTER_NORMAL;
				}
				else if((sports_lamp == CAN_SPORTLAMP_TURN_OFF) && (snow_hold_lamp == CAN_SNOWLAMP_ON))
				{
					/* �X�|�[�c�����v:OFF�A�X�m�[�z�[���h�����v:ON */
					accelChar = SIGNAL_VALUE_ACCEL_CHARACTER_SNOW;
				}
				else
				{
					/* ���̑��i�X�|�[�c�����v:OFF�ȊO�j */
					accelChar = SIGNAL_VALUE_ACCEL_CHARACTER_SPORTS;
				}
			}
			else
			{
				/* �X�|�[�c�����v�A�X�m�[�z�[���h�����v:���� */
				/* �����l */
			}
			
		}
		else
		{
			/* ���̑��iAT or CVT�ȊO�j */
			accelChar = SIGNAL_VALUE_ACCEL_CHARACTER_IGNORE;
		}
	}
	else
	{
		/* �g�����X�~�b�V�������:���� */
		/* �����l */
	}
	*out = (SI_32)((UI_32)accelRatio | ((UI_32)accelChar << 8));
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_BrakeFlag					*/
/* �@�\�̐���	: �u���[�L����t���O�擾				*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_BrakeFlag(SI_32 *out)
{
	UI_16 footbrakeLamp;
	UI_16 parkingBrakeLamp;
	
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	/* �u���[�L�����v */
	if(pCanInfo->en_brake_lamp_sw == 1)
	{
		/* �u���[�L�����v:�L�� */
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
		/* �u���[�L�����v:���� */
		/* �����l */
		footbrakeLamp = (UI_16)SIGNAL_VALUE_BRAKELAMP_INIT;
	}
	/* �p�[�L���O�u���[�L */
	if(pCanInfo->en_hand_brake_sw == 1)
	{
		/* �p�[�L���O�u���[�L:�L�� */
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
		/* �p�[�L���O�u���[�L:���� */
		parkingBrakeLamp = (UI_16)SIGNAL_VALUE_PARKINGBRAKE_INIT;
	}
	*out = (SI_32)(((UI_32)footbrakeLamp) | ((UI_32)parkingBrakeLamp << 4));
	return 0;
}

/********************************************************/
/* �֐�����		: get_signal_BrakeOperation				*/
/* �@�\�̐���	: �u���[�L�y�_������ʎ擾				*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
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
			/*086A�p�u���[�L�ő�l*/
			brakeMax = CAN_BRAKEPRESSURE_MAX_86A;
		}
		else if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086B)
		{
			/*086B�p�u���[�L�ő�l*/
			brakeMax = CAN_BRAKEPRESSURE_MAX_86B;
		}
		else
		{
			/*���̑��̓u���[�L�ő�l0*/
			brakeMax = 0;
		}
		if (brakeMax > 0)
		{
			/*�u���[�L�ő�l����*/
			brake = pCanInfo->smoothing_brake_pressure;
			/* ��ɕ���\���v�Z����Ɓ��̕ω����傫���Ȃ�̂Ō�Ōv�Z���� */
			/* �����_���l������100�{���� */
			if ((brake * 100) >= (UI_16)((brakeMax * 100) / CAN_BRAKEPRESSURE_RESOLUTION))
			{
				/* �ő�l�ȏ�̏ꍇ��100�� */
				*out = 100;
			}
			else
			{
				/* �ő�l�����̏ꍇ�͔䗦���v�Z */
				*out = (SI_32)((((F_64)brake * CAN_BRAKEPRESSURE_RESOLUTION) * 100) / brakeMax);
			}
		}
		else
		{
			/*�u���[�L�ő�l�Ȃ��̂Ƃ��͖����l*/
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
/* �֐�����		: get_signal_ShiftOperation				*/
/* �@�\�̐���	: �V�t�g����擾						*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_ShiftOperation(SI_32 *out)
{
	UI_8 atShift 	= SIGNAL_VALUE_ATSHIFT_INIT;
	UI_8 gearPos 	= SIGNAL_VALUE_GEARPOS_INIT;
	UI_16 gearRatio = SIGNAL_VALUE_IGNORE_16;	/* �ݒ�Ȃ� */
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if((pCanInfo->en_transmission_type == 1) && (pCanInfo->en_vehicle_info == 1))
	{
		/* �g�����X�~�b�V�������:�L�� */
		if(pCanInfo->transmission_type == CAN_TRANSMISSION_ATORCVT)
		{
			/* AT or CVT */
			if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086A){
				/* 86A�ԗ� */
				/* AT�V�t�g�ʒu��� */
				if((pCanInfo->en_shift_pos == 1) && (pCanInfo->en_mode_shift_control == 1) && (pCanInfo->en_sports_gear_pos == 1))
				{
					/* TCM�ϑ����䃂�[�h */
					if (pCanInfo->mode_shift_control == CAN_MODESHIFT_GEARFIXED_AUTORETURN)
					{
						/*TCM�ϑ����䃂�[�h:�M�A�Œ莩�����A���[�h�i�}�j���A���j	*/
						atShift = SIGNAL_VALUE_ATSHIFT_DM;
					}
					else if (pCanInfo->mode_shift_control == CAN_MODESHIFT_GEARFIXED_MODE)
					{
						/*TCM�ϑ����䃂�[�h:�M�A�Œ胂�[�h�i�}�j���A���j			*/
						atShift = SIGNAL_VALUE_ATSHIFT_M;
					}
					else
					{
						/*TCM�ϑ����䃂�[�h:���̑�				*/
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
						/*TCM�ϑ����䃂�[�h:�M�A�Œ莩�����A���[�h�i�}�j���A���j	*/
						/*TCM�ϑ����䃂�[�h:�M�A�Œ胂�[�h�i�}�j���A���j			*/
						switch (pCanInfo->sports_gear_pos)
						{
							case CAN_GEARPOS_1ST:	/*	�M�A�i��:1ST													*/
								gearPos = SIGNAL_VALUE_GEARPOS_1ST;
								break;
							case CAN_GEARPOS_2ND:	/*	�M�A�i��:2ND													*/
								gearPos = SIGNAL_VALUE_GEARPOS_2ND;
								break;
							case CAN_GEARPOS_3RD:	/*	�M�A�i��:3RD													*/
								gearPos = SIGNAL_VALUE_GEARPOS_3RD;
								break;
							case CAN_GEARPOS_4TH:	/*	�M�A�i��:4TH													*/
								gearPos = SIGNAL_VALUE_GEARPOS_4TH;
								break;
							case CAN_GEARPOS_5TH:	/*	�M�A�i��:5TH													*/
								gearPos = SIGNAL_VALUE_GEARPOS_5TH;
								break;
							case CAN_GEARPOS_6TH:	/*	�M�A�i��:6TH													*/
								gearPos = SIGNAL_VALUE_GEARPOS_6TH;
								break;
							default:
								/* ���̑� */
								gearPos = SIGNAL_VALUE_GEARPOS_IGNORE;
								break;
						}
					}
					else if(atShift == SIGNAL_VALUE_ATSHIFT_R )
					{
						/* ���o�[�X */
						gearPos = SIGNAL_VALUE_GEARPOS_REVERSE;
					}
					else{
						/* ���̑� */
						gearPos = SIGNAL_VALUE_GEARPOS_IGNORE;
					}
				}
				else
				{
					/* �����l */
				}
			}else if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086B){
				/* 86B�ԗ� */
				/* AT�V�t�g�ʒu��� */
				if((pCanInfo->en_shift_pos == 1) && (pCanInfo->en_sports_gear_pos == 1) && (pCanInfo->en_manual_mode_sw == 1))
				{
					gearPos = SIGNAL_VALUE_GEARPOS_IGNORE;
					switch (pCanInfo->shift_pos)
					{
						case CAN_ATSHIFT_86B_D	:	/* D */
							
							/*�V�t�gD*/
							switch (pCanInfo->sports_gear_pos)
							{
								case CAN_ATGEAR_86B_MANUAL_1ST:	/*	�M�A�i��:1ST�iM�����W�{D�����W�e���|����M�j				*/
								case CAN_ATGEAR_86B_MANUAL_2ND:	/*	�M�A�i��:2ND�iM�����W�{D�����W�e���|����M�j				*/
								case CAN_ATGEAR_86B_MANUAL_3RD:	/*	�M�A�i��:3RD�iM�����W�{D�����W�e���|����M�j				*/
								case CAN_ATGEAR_86B_MANUAL_4TH:	/*	�M�A�i��:4TH�iM�����W�{D�����W�e���|����M�j				*/
								case CAN_ATGEAR_86B_MANUAL_5TH:	/*	�M�A�i��:5TH�iM�����W�{D�����W�e���|����M�j				*/
								case CAN_ATGEAR_86B_MANUAL_6TH:	/*	�M�A�i��:6TH�iM�����W�{D�����W�e���|����M�j				*/
									if (pCanInfo->manual_mode_sw == CAN_MANUALMODE_SW_ON)
									{
										/*�}�j���A�����[�hSW:ON*/
										/*M�����W*/
										atShift = SIGNAL_VALUE_ATSHIFT_M;
									}
									else
									{
										/*�}�j���A�����[�hSW:OFF*/
										/*DM�����W*/
										atShift = SIGNAL_VALUE_ATSHIFT_DM;
									}
									break;
								case CAN_ATGEAR_86B_DSPORT_1ST:	/*	�M�A�i��:1ST�iD�����W�X�|�[�c���[�h�j					*/
								case CAN_ATGEAR_86B_DSPORT_2ND:	/*	�M�A�i��:2ND�iD�����W�X�|�[�c���[�h�j					*/
								case CAN_ATGEAR_86B_DSPORT_3RD:	/*	�M�A�i��:3RD�iD�����W�X�|�[�c���[�h�j					*/
								case CAN_ATGEAR_86B_DSPORT_4TH:	/*	�M�A�i��:4TH�iD�����W�X�|�[�c���[�h�j					*/
								case CAN_ATGEAR_86B_DSPORT_5TH:	/*	�M�A�i��:5TH�iD�����W�X�|�[�c���[�h�j					*/
								case CAN_ATGEAR_86B_DSPORT_6TH:	/*	�M�A�i��:6TH�iD�����W�X�|�[�c���[�h�j					*/
									/*DM�����W*/
									atShift = SIGNAL_VALUE_ATSHIFT_DM;
									break;
								default:
									/* ���̑�*/
									/*D�����W*/
									atShift = SIGNAL_VALUE_ATSHIFT_D;
									break;
							}

							switch (pCanInfo->sports_gear_pos)
							{
								case CAN_ATGEAR_86B_MANUAL_1ST:	/*	�M�A�i��:1ST�iM�����W�{D�����W�e���|����M�j				*/
								case CAN_ATGEAR_86B_DSPORT_1ST:	/*	�M�A�i��:1ST�iD�����W�X�|�[�c���[�h�j					*/
									gearPos = SIGNAL_VALUE_GEARPOS_1ST;
									break;
								case CAN_ATGEAR_86B_MANUAL_2ND:	/*	�M�A�i��:2ND�iM�����W�{D�����W�e���|����M�j				*/
								case CAN_ATGEAR_86B_DSPORT_2ND:	/*	�M�A�i��:2ND�iD�����W�X�|�[�c���[�h�j					*/
									gearPos = SIGNAL_VALUE_GEARPOS_2ND;
									break;
								case CAN_ATGEAR_86B_MANUAL_3RD:	/*	�M�A�i��:3RD�iM�����W�{D�����W�e���|����M�j				*/
								case CAN_ATGEAR_86B_DSPORT_3RD:	/*	�M�A�i��:3RD�iD�����W�X�|�[�c���[�h�j					*/
									gearPos = SIGNAL_VALUE_GEARPOS_3RD;
									break;
								case CAN_ATGEAR_86B_MANUAL_4TH:	/*	�M�A�i��:4TH�iM�����W�{D�����W�e���|����M�j				*/
								case CAN_ATGEAR_86B_DSPORT_4TH:	/*	�M�A�i��:4TH�iD�����W�X�|�[�c���[�h�j					*/
									gearPos = SIGNAL_VALUE_GEARPOS_4TH;
									break;
								case CAN_ATGEAR_86B_MANUAL_5TH:	/*	�M�A�i��:5TH�iM�����W�{D�����W�e���|����M�j				*/
								case CAN_ATGEAR_86B_DSPORT_5TH:	/*	�M�A�i��:5TH�iD�����W�X�|�[�c���[�h�j					*/
									gearPos = SIGNAL_VALUE_GEARPOS_5TH;
									break;
								case CAN_ATGEAR_86B_MANUAL_6TH:	/*	�M�A�i��:6TH�iM�����W�{D�����W�e���|����M�j				*/
								case CAN_ATGEAR_86B_DSPORT_6TH:	/*	�M�A�i��:6TH�iD�����W�X�|�[�c���[�h�j					*/
									gearPos = SIGNAL_VALUE_GEARPOS_6TH;
									break;
								default:
									/* ���̑�*/
									/* �����Ȃ�*/
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
					/* �����l */
				}
			}else{
				/* �Ԏ�F�s�� */
				/* �����l */
			}
		}
		else if(pCanInfo->transmission_type == CAN_TRANSMISSION_MT)
		{
			/* MT */
			atShift = SIGNAL_VALUE_ATSHIFT_IGNORE;
			if((pCanInfo->en_mt_rev_sw == 1) && (pCanInfo->en_mt_gear_pos == 1))
			{
				/* MT���o�[�X�AMT�M�A:�L�� */
				if(pCanInfo->mt_rev_sw == CAN_MT_REVERSE_ON)
				{
					/* MT���o�[�XON */
					gearPos = SIGNAL_VALUE_GEARPOS_REVERSE;
				}
				else
				{
					/* MT���o�[�XOFF */
					switch (pCanInfo->mt_gear_pos)
					{
						case CAN_GEARPOS_1ST:	/*	�M�A�i��:1ST													*/
							gearPos = SIGNAL_VALUE_GEARPOS_1ST;
							break;
						case CAN_GEARPOS_2ND:	/*	�M�A�i��:2ND													*/
							gearPos = SIGNAL_VALUE_GEARPOS_2ND;
							break;
						case CAN_GEARPOS_3RD:	/*	�M�A�i��:3RD													*/
							gearPos = SIGNAL_VALUE_GEARPOS_3RD;
							break;
						case CAN_GEARPOS_4TH:	/*	�M�A�i��:4TH													*/
							gearPos = SIGNAL_VALUE_GEARPOS_4TH;
							break;
						case CAN_GEARPOS_5TH:	/*	�M�A�i��:5TH													*/
							gearPos = SIGNAL_VALUE_GEARPOS_5TH;
							break;
						case CAN_GEARPOS_6TH:	/*	�M�A�i��:6TH													*/
							gearPos = SIGNAL_VALUE_GEARPOS_6TH;
							break;
						case CAN_GEARPOS_NOT_DECISION:	/*	�M�A�i��:�s��											*/
							gearPos = SIGNAL_VALUE_GEARPOS_INIT;
							break;
						default:
							/* ���̑� */
							gearPos = SIGNAL_VALUE_GEARPOS_IGNORE;
							break;
					}
				}
			}
			else
			{
				/* MT���o�[�X�AMT�M�A:���� */
				/* �����l */
			}
		}
		else
		{
			/* ���̑� */
			atShift = SIGNAL_VALUE_ATSHIFT_IGNORE;
			gearPos = SIGNAL_VALUE_GEARPOS_IGNORE;
		}
	}
	else
	{
		/* �g�����X�~�b�V�������:���� */
		/* �����l */
	}
	*out = (SI_32)(((UI_32)atShift) | ((UI_32)gearRatio << 8) | ((UI_32)gearPos << 24));
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_FuelIndicator				*/
/* �@�\�̐���	: �t���[�G����,VSC��Ԏ擾				*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)S.Sakata							*/
/* �쐬��		: 21/02/-								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_FuelIndicator(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	UI_8 absAct = SIGNAL_VALUE_NONACTIVE;
	UI_8 vscAct = SIGNAL_VALUE_NONACTIVE;
	UI_8 trcAct = SIGNAL_VALUE_NONACTIVE;
	UI_8 trqUpReq = SIGNAL_VALUE_ENGTRQ_NO_REQUEST;
	UI_8 trqDwnReq = SIGNAL_VALUE_ENGTRQ_NO_REQUEST;
	UI_8 vsc = SIGNAL_VALUE_IGNORE_4;										/* VSC���*/
	UI_16 fuelIndicator = SIGNAL_VALUE_IGNORE_12;							/* �t���[�G���� */
	SI_32 randomValue;

	if (pCanInfo->en_vehicle_info == 1){
		/*�Ԏ�L��*/
		if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086B){
			/*�Ԏ�F086B*/
			/*ABS�����ԗL���`�F�b�N*/
			if (pCanInfo->en_absAct == 1)
			{
				/*ABS�����Ԕ���*/
				if (pCanInfo->absAct == CAN_ABSON_ACTIVE)
				{
					/*ABS���䒆*/
					absAct = SIGNAL_VALUE_ACTIVE;
				}
				else
				{
					/*ABS����O*/
					/*�����Ȃ�*/
				}
			}

			/*VSC�����ԗL���`�F�b�N*/
			if (pCanInfo->en_vscAct == 1)
			{
				/*VSC�����Ԕ���*/
				if (pCanInfo->vscAct == CAN_VSCON_ACTIVE)
				{
					/*VSC���䒆*/
					vscAct = SIGNAL_VALUE_ACTIVE;
				}
				else
				{
					/*VSC����O*/
					/*�����Ȃ�*/
				}
			}

			/*TRC�����ԗL���`�F�b�N*/
			if (pCanInfo->en_trcAct == 1)
			{
				/*TRC�����Ԕ���*/
				if (pCanInfo->trcAct == CAN_TRCON_ACTIVE)
				{
					/*TRC���䒆*/
					trcAct = SIGNAL_VALUE_ACTIVE;
				}
				else
				{
					/*TRC����O*/
					/*�����Ȃ�*/
				}
			}
			else
			{
				/*VSCOFF��ԁAVSC�����ԁF����*/
				/*�����Ȃ�*/
			}

			/*�G���W���g���N�A�b�v�v���L���`�F�b�N*/
			if (pCanInfo->en_engTrqUpReqFlag == 1)
			{
				/*�G���W���g���N�A�b�v�v������*/
				if (pCanInfo->engTrqUpReqFlag == CAN_ENGTRQ_REQUEST)
				{
					/*�v������*/
					trqUpReq = SIGNAL_VALUE_ENGTRQ_REQUEST;
				}
				else
				{
					/*�v���Ȃ�*/
					/*�����Ȃ�*/
				}
			}
			else
			{
				/*�G���W���g���N�A�b�v�v���F����*/
				/*�����Ȃ�*/
			}

			/*�G���W���g���N�_�E���v���L���`�F�b�N*/
			if (pCanInfo->en_engTrqDwnReqFlag == 1)
			{
				/*�G���W���g���N�_�E���v������*/
				if (pCanInfo->engTrqDwnReqFlag == CAN_ENGTRQ_REQUEST)
				{
					/*�v������*/
					trqDwnReq = SIGNAL_VALUE_ENGTRQ_REQUEST;
				}
				else
				{
					/*�v���Ȃ�*/
					/*�����Ȃ�*/
				}
			}
			else
			{
				/*�G���W���g���N�_�E���v���F����*/
				/*�����Ȃ�*/
			}
		}
	}

	/* �����擾 */
	(void)get_signal_RandomValue(&randomValue);

	*out = (SI_32)( (UI_32)fuelIndicator | ((UI_32)vsc << 12) | ((UI_32)absAct << 16) | 
					((UI_32)vscAct << 17) | ((UI_32)trcAct << 18) |
					((UI_32)trqUpReq << 19) | ((UI_32)trqDwnReq << 20) | 
					((UI_32)randomValue & 0xFFE00000));
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_Steering					*/
/* �@�\�̐���	: �X�e�A�����O�擾						*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Steering(SI_32 *out)
{
	/*
	�X�e�A�����O�@�@�E���񎞁F�v���X�^�����񎞁F�}�C�i�X
	*/
	SI_16 steer;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_steer_angle == 1)
	{
		/* �X�e�A�����O:�L�� */
		if(pCanInfo->steer_angle != CAN_STEERING_ERRORVALUE)
		{
			/* ����l */
			steer = (SI_16)((F_64)((SI_16)pCanInfo->steer_angle) * CAN_STEERING_RESOLUTION);
			*out = (SI_32)((UI_32)steer & 0x0000FFFF);
		}
		else
		{
			/* �ُ�l */
			*out = SIGNAL_VALUE_IGNORE_16_SIGNED;
		}
	}
	else
	{
		/* �X�e�A�����O:���� */
		*out = SIGNAL_VALUE_IGNORE_16_SIGNED;
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_Blinker					*/
/* �@�\�̐���	: �E�B���J�[�擾						*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Blinker(SI_32 *out)
{

	UI_8 blinker = SIGNAL_VALUE_BLINKER_INITIAL;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	
	/*�Ԏ��ԃ`�F�b�N*/
	if (pCanInfo->en_vehicle_info == 1)
	{
		/*�Ԏ�L��*/
		if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086B)
		{
			/*�Ԏ�F086B*/
			/*�E�B���J�[�L���`�F�b�N*/
			if((pCanInfo->en_blinker_left == 1) && (pCanInfo->en_blinker_right == 1))
			{
				/*�E�B���J�[�F�L��*/
				if( (pCanInfo->blinker_right == CAN_BLINKER_OFF) && (pCanInfo->blinker_left == CAN_BLINKER_OFF) )
				{
					/*�E�B���J�[�F����OFF*/
					blinker = SIGNAL_VALUE_BLINKER_OFF;
				}
				else 
				{
					/*�E�B���J�[�F�ǂ��炩�_��*/
					if(pCanInfo->blinker_right == CAN_BLINKER_ON)
					{
						/*�E�B���J�[�F�E�_��*/
						blinker |= SIGNAL_VALUE_BLINKER_TURN_RIGHT;
					}
					if(pCanInfo->blinker_left == CAN_BLINKER_ON)
					{
						/*�E�B���J�[�F���_��*/
						blinker |= SIGNAL_VALUE_BLINKER_TURN_LEFT;
					}
				}
			}
			else
			{
				/* �E�B���J�[:���� */
				/* �����l*/
			}
		}
		else
		{
			/*�Ԏ�F086B�ȊO*/
			blinker = SIGNAL_VALUE_BLINKER_INVALID;
		}
	}
	else
	{
		/*�Ԏ햳��*/
		/*��������*/
	}
	*out = (SI_32)blinker;
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_AirCondition				*/
/* �@�\�̐���	: �G�A�[�R���f�B�V�����擾				*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
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
		/* �O�C��:�L�� */
		if(pCanInfo->out_temp <= CAN_OUOTSIDE_TEMP_MAX)
		{
			/* ����l */
			outsideTemp = (UI_8)((UI_8)((F_32)pCanInfo->out_temp * CAN_OUOTSIDE_TEMP_RESOLUTION) + CAN_OUTSIDE_TEMP_OFFSET - SIGNAL_VALUE_OUTSIDE_TEMP_OFFSET);
		}
		else
		{
			/* �ُ�l */
			/* �����l */
		}
	}
	else
	{
		/* �O�C��:���� */
		/* �����l */
	}
	*out = (SI_32)((airconStat) | ((UI_32)airconTemp << 4) | ((UI_32)insideTemp << 12) | ((UI_32)outsideTemp << 20));
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_DrivingSupport				*/
/* �@�\�̐���	: �h���C�r���O�T�|�[�g�擾				*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_DrivingSupport(SI_32 *out)
{
	UI_8 absSet = SIGNAL_VALUE_ABS_INIT;					/*ABS�Z�b�g���*/
	UI_8 vscSet = SIGNAL_VALUE_VSC_INIT_OR_NORMAL;			/*VSC�Z�b�g���*/
	UI_32 other = SIGNAL_VALUE_IGNORE_32;	/*���̑��M���F���ׂĖ����l*/
	UI_8 vscMode;			/*VSC�F���[�h*/
	UI_8 vscActive;			/*VSC�FActive���*/

	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	/*�Ԏ��ԃ`�F�b�N*/
	if (pCanInfo->en_vehicle_info == 1){
		/*�Ԏ�L��*/
		if (pCanInfo->vehicle_info == CAN_VEHICLEINFO_086B){
			/*�Ԏ�F086B*/
			/*ABS���`�F�b�N*/
			if ((pCanInfo->en_absAct == 1) || (pCanInfo->en_absFail == 1) || (pCanInfo->en_absWarning == 1))
			{
				/*ABS���L��*/
				if ((pCanInfo->absFail == CAN_ABS_SYSFAIL_ON) || (pCanInfo->absWarning == CAN_ABS_WARNING_ON))
				{
					/*ABS�ُ�܂��̓E�H�[�j���O*/
					absSet = SIGNAL_VALUE_ABS_IGNORE;
				}
				/*VSC�����Ԕ���*/
				else if (pCanInfo->absAct == CAN_ABSON_ACTIVE)
				{
					/*ABS���䒆*/
					absSet = SIGNAL_VALUE_ABS_SET_AND_ACTIVE;
				}
				else
				{
					/*�����Ȃ�*/
				}
			}
			else
			{
				/*ABS��񖳌�*/
				/*��������*/
			}

			/*VSC���`�F�b�N*/
			if ((pCanInfo->en_trcMode == 1) || (pCanInfo->en_trcOnOff == 1) || (pCanInfo->en_trcAct == 1) ||
				(pCanInfo->en_vscUnderStr == 1) || (pCanInfo->en_vscOverStr == 1) || 
				(pCanInfo->en_vscOnOff == 1) || (pCanInfo->en_vscWarning == 1) )
			{
				/*VSC���L��*/
				if (pCanInfo->vscWarning == CAN_VSC_WARNING_ON)
				{
					/*VSC�E�H�[�j���O*/
					vscSet = SIGNAL_VALUE_VSC_IGNORE;
				}
				else
				{
					/*VSC�E�H�[�j���O�ȊO*/
					/*VSC���[�h����*/
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
							/*�m�[�}��*/
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
							/*VSC OFF /TRC ON / TRC�d�����[�hON*/
							vscMode = SIGNAL_VALUE_VSC_SET_SPECIAL;
						}
						else
						{
							/*VSC OFF /TRC ON / TRC�d�����[�hOFF*/
							vscMode = SIGNAL_VALUE_VSC_SET_VSC_OFF;
						}
					}

					/*VSC ����/�񐧌䔻��*/
					if ( (pCanInfo->vscUnderStr == CAN_VSC_CONTROL_STR_ON) || (pCanInfo->vscOverStr == CAN_VSC_CONTROL_STR_ON) )
					{
						/*VSC���䒆(�A���_�[�X�e�A�A�I�[�o�[�X�e�A���䒆)*/
						if (pCanInfo->trcAct == CAN_TRCON_ACTIVE)
						{
							/*VSC & TRC ���䒆*/
							vscActive = SIGNAL_VALUE_VSC_TRC_ACTIVE_BIT | SIGNAL_VALUE_VSC_VSC_ACTIVE_BIT;
						}
						else
						{
							/*VSC���䒆*/
							vscActive = SIGNAL_VALUE_VSC_VSC_ACTIVE_BIT;
						}
					}
					else
					{
						/*VSC�񐧌䒆*/
						if (pCanInfo->trcAct == CAN_TRCON_ACTIVE)
						{
							/*TRC���䒆*/
							vscActive = SIGNAL_VALUE_VSC_TRC_ACTIVE_BIT;
						}
						else
						{
							/*VSC & TRC �񐧌䒆*/
							vscActive = SIGNAL_VALUE_VSC_NOTACTIVE;
						}
					}
					/*VSC��Ԃɐݒ�*/
					vscSet = vscActive + vscMode;
				}
			}
			else
			{
				/*ABS��񖳌�*/
				/*��������*/
			}
		}else{
			/*�Ԏ�F086B�ȊO*/
			absSet = SIGNAL_VALUE_ABS_IGNORE;
			vscSet = SIGNAL_VALUE_VSC_IGNORE;
		}
	}else{
		/*�Ԏ햳��*/
		/*��������*/
	}


	*out = (SI_32)((absSet) | ((UI_32)vscSet << 4) | (other << 12));
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_Doors						*/
/* �@�\�̐���	: �h�A��Ԏ擾							*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
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
		/* �h�ASW P�ȁAD��:�L�� */
		/* �h���C�o�[�ʒu�擾 */
		(void)get_signal_DriverPosition(&driverPos);
		if(driverPos == SIGNAL_VALUE_DRIVERPOSITION_RIGHT)
		{
			/* �E�n���h�� */
			if(pCanInfo->door_sw_seat_d == CAN_DOOR_SW_OPEN)
			{
				/* D�ȃI�[�v���i�E�j */
				firstDoor |= SIGNAL_VALUE_DOOR_RIGHT_OPEN;
			}
			
			if(pCanInfo->door_sw_seat_p == CAN_DOOR_SW_OPEN)
			{
				/* P�ȃI�[�v���i���j */
				firstDoor |= SIGNAL_VALUE_DOOR_LEFT_OPEN;
			}
		}
		else if(driverPos == SIGNAL_VALUE_DRIVERPOSITION_LEFT)
			/* ���n���h�� */
		{
			if(pCanInfo->door_sw_seat_d == CAN_DOOR_SW_OPEN)
			{
				/* D�ȃI�[�v���i���j */
				firstDoor |= SIGNAL_VALUE_DOOR_LEFT_OPEN;
			}
			
			if(pCanInfo->door_sw_seat_p == CAN_DOOR_SW_OPEN)
			{
				/* P�ȃI�[�v���i�E�j */
				firstDoor |= SIGNAL_VALUE_DOOR_RIGHT_OPEN;
			}
		}
		else
		{
			/* ���̑� */
			/* �����l */
		}
	}
	else
	{
		/* �h�ASW P�ȁAD��:���� */
		/* �����l */
	}
	*out = (SI_32)((firstDoor) | ((UI_32)secondDoor << 4) | ((UI_32)hood << 8) | ((UI_32)roof << 12));
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_SittingandSeatBeltStatus	*/
/* �@�\�̐���	: �V�[�g���ȁA�V�[�g�x���g�擾			*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 21/03/-								*/
/* ���l			: 										*/
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
		/* �V�[�g�x���g P�ȁAD��:�L�� */
		if((pCanInfo->seat_belt_sw_d == CAN_SEATBELT_OFF) && (pCanInfo->seat_belt_sw_p == CAN_SEATBELT_OFF))
		{
			/* D�ȁAP�ȂƂ��ɃV�[�g�x���g�񑕒� */
			firstSeatbelt |= SIGNAL_VALUE_SEATBELT_NOONELOCKED;
		}
		else
		{
			/* D�ȁAP�Ȃ����ꂩ���V�[�g�x���g���� */
			/* �h���C�o�[�ʒu�擾 */
			(void)get_signal_DriverPosition(&driverPos);
			if(driverPos == SIGNAL_VALUE_DRIVERPOSITION_RIGHT)
			{
				/* �E�n���h�� */
				if(pCanInfo->seat_belt_sw_d == CAN_SEATBELT_ON)
				{
					/* D�ȃV�[�g�x���g����(�E�j */
					firstSeatbelt |= SIGNAL_VALUE_SEATBELT_RIGHT_LOCKED;
				}
				
				if(pCanInfo->seat_belt_sw_p == CAN_SEATBELT_ON)
				{
					/* P�ȃV�[�g�x���g����(���j */
					firstSeatbelt |= SIGNAL_VALUE_SEATBELT_LEFT_LOCKED;
				}
			}
			else if(driverPos == SIGNAL_VALUE_DRIVERPOSITION_LEFT)
			{
				/* ���n���h�� */
				if(pCanInfo->seat_belt_sw_d == CAN_SEATBELT_ON)
				{
					/* D�ȃV�[�g�x���g����(���j */
					firstSeatbelt |= SIGNAL_VALUE_SEATBELT_LEFT_LOCKED;
				}
				
				if(pCanInfo->seat_belt_sw_p == CAN_SEATBELT_ON)
				{
					/* P�ȃV�[�g�x���g����(�E�j */
					firstSeatbelt |= SIGNAL_VALUE_SEATBELT_RIGHT_LOCKED;
				}
			}
			else
			{
				/*���̑�*/
				/*�����Ȃ��F�����l*/
			}
		}
	}
	else
	{
		/* �V�[�g�x���g P�ȁAD��:���� */
		/* �����l */
	}
	*out =	(SI_32)((firstSitting) 				 | ((UI_32)secondSitting << 4) 	 | ((UI_32)thirdSitting << 8) | 
					((UI_32)firstSeatbelt << 12) | ((UI_32)secondSeatbelt << 16) | ((UI_32)thirdSeatbelt << 20));

	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_LampStatus				*/
/* �@�\�̐���	: �����v��Ԏ擾						*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_LampStatus(SI_32 *out)
{
	UI_8 headLamp = SIGNAL_VALUE_HEADLIGHT_INITIAL;
	UI_8 fogLamp = SIGNAL_VALUE_IGNORE_4;
	UI_8 rooLamp = SIGNAL_VALUE_IGNORE_4;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	
	if((pCanInfo->en_head_lamp_hi == 1) && (pCanInfo->en_head_lamp_lo == 1) && (pCanInfo->en_head_lamp_small == 1))
	{
		/* �w�b�h���C�g:�L�� */
		if((pCanInfo->head_lamp_lo == CAN_LIGHT_TURN_OFF) && (pCanInfo->head_lamp_small == CAN_LIGHT_TURN_OFF) && (pCanInfo->head_lamp_hi == CAN_LIGHT_TURN_OFF))
		{
			/* ���ׂ�OFF */
			headLamp = SIGNAL_VALUE_HEADLIGHT_OFF;
		}
		else{
			/* �����ꂪ��ON */
			if(pCanInfo->head_lamp_small == CAN_LIGHT_TURN_ON)
			{
				/* �w�b�h���C�g:Small */
				headLamp |= SIGNAL_VALUE_HEADLIGHT_SMALL;
			}
			if(pCanInfo->head_lamp_lo == CAN_LIGHT_TURN_ON)
			{
				/* �w�b�h���C�g:Lo */
				headLamp |= SIGNAL_VALUE_HEADLIGHT_LO;
			}
			if((pCanInfo->head_lamp_hi == CAN_LIGHT_TURN_ON) && (pCanInfo->head_lamp_lo == CAN_LIGHT_TURN_ON))
			{
				/* �w�b�h���C�g:Hight and lo */
				headLamp |= SIGNAL_VALUE_HEADLIGHT_HIGHANDLO;
			}
		}
	}
	else
	{
		/* �w�b�h���C�g:���� */
		/* �����l */
	}
	*out = (SI_32)((UI_32)headLamp | ((UI_32)fogLamp << 4) | ((UI_32)rooLamp << 8));
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_Power						*/
/* �@�\�̐���	: �N����Ԏ擾							*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Power(SI_32 *out)
{
	UI_8 power;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if((pCanInfo->en_ign == 1) && (pCanInfo->en_acc == 1))
	{
		/* ACC IGN:�L�� */
		power = SIGNAL_VALUE_POWER_PLUS_B; /* +B�͌Œ��ON */
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
		/* ACC IGN:���� */
		power = SIGNAL_VALUE_POWER_INITIAL;
	}
	*out = (SI_32)power;
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_EngineRev					*/
/* �@�\�̐���	: �G���W����]���擾					*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineRev(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_eng_rpm_taco == 1)
	{
		/* �G���W����]��:�L�� */
		*out = pCanInfo->eng_rpm_taco;
	}
	else
	{
		/* �G���W����]��:���� */
		*out = SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_VehicleSpeed				*/
/* �@�\�̐���	: �ԑ��擾								*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_VehicleSpeed(SI_32 *out)
{
	UI_16 speed = 0;
	UI_8  direction = SIGNAL_VALUE_DIRECTION_IGNORE;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if( pCanInfo->en_vehicle_speed_for_disp == 1 )
	{
		/* �ԑ�:�L�� */
		speed = (UI_16)((F_64)pCanInfo->vehicle_speed_for_disp * (CAN_VEHICLESPEED_DISP_RESOLUTION / SIGNAL_VALUE_VEHICLESPEED_DISP_RESOLUTION));
	}
	else
	{
		/* �ԑ�:���� */
		speed = SIGNAL_VALUE_IGNORE_16;
	}
	
	*out = (SI_32)(((UI_32)direction << 16) | (UI_32)speed);
	
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_HighResolutionSpeedFront	*/
/* �@�\�̐���	: �����x�ԑ�(�t�����g)�擾				*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_HighResolutionSpeedFront(SI_32 *out)
{
	UI_16 fl,fr;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if( (pCanInfo->en_speed_wheel_ftlh == 1) && (pCanInfo->en_speed_wheel_ftrh == 1) )
	{
		/* �ԗ֑�(FL��),�ԗ֑�(FR��):�L�� */
		fl = (UI_16)(((F_64)pCanInfo->speed_wheel_ftlh * (CAN_WHEELSPEED_RESOLUTION / SIGNAL_VALUE_WHEELSPEED_RESOLUTION)) - ((F_64)SIGNAL_VALUE_WHEELSPEED_OFFSET / SIGNAL_VALUE_WHEELSPEED_RESOLUTION));
		fr = (UI_16)(((F_64)pCanInfo->speed_wheel_ftrh * (CAN_WHEELSPEED_RESOLUTION / SIGNAL_VALUE_WHEELSPEED_RESOLUTION)) - ((F_64)SIGNAL_VALUE_WHEELSPEED_OFFSET / SIGNAL_VALUE_WHEELSPEED_RESOLUTION));
	}
	else
	{
		/* �ԗ֑�(FL��),�ԗ֑�(FR��):���� */
		fl = SIGNAL_VALUE_IGNORE_16;
		fr = SIGNAL_VALUE_IGNORE_16;
	}
	*out = (SI_32)((UI_32)fl | ((UI_32)fr << 16));
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_HighResolutionSpeedRear	*/
/* �@�\�̐���	: �����x�ԑ�(���A)�擾					*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_HighResolutionSpeedRear(SI_32 *out)
{
	UI_16 rl,rr;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if( (pCanInfo->en_speed_wheel_rrlh == 1) && (pCanInfo->en_speed_wheel_rrrh == 1) )
	{
		/* �ԗ֑�(RL��),�ԗ֑�(RR��):�L�� */
		rl = (UI_16)(((F_64)pCanInfo->speed_wheel_rrlh * (CAN_WHEELSPEED_RESOLUTION / SIGNAL_VALUE_WHEELSPEED_RESOLUTION)) - ((F_64)SIGNAL_VALUE_WHEELSPEED_OFFSET / SIGNAL_VALUE_WHEELSPEED_RESOLUTION));
		rr = (UI_16)(((F_64)pCanInfo->speed_wheel_rrrh * (CAN_WHEELSPEED_RESOLUTION / SIGNAL_VALUE_WHEELSPEED_RESOLUTION)) - ((F_64)SIGNAL_VALUE_WHEELSPEED_OFFSET / SIGNAL_VALUE_WHEELSPEED_RESOLUTION));
	}
	else
	{
		/* �ԗ֑�(RL��),�ԗ֑�(RR��):���� */
		rl = SIGNAL_VALUE_IGNORE_16;
		rr = SIGNAL_VALUE_IGNORE_16;
	}
	*out = (SI_32)((UI_32)rl | ((UI_32)rr << 16));
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_AccelerationPlane			*/
/* �@�\�̐���	: �����x�i��������:�O��A���j�擾		*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_AccelerationPlane(SI_32 *out)
{
	SI_16 acc_lr,acc_fr;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if( pCanInfo->en_acc_fr == 1 )
	{
		/* �O������x:�L�� */
		acc_fr = (SI_16)((F_64)((SI_8)pCanInfo->acc_fr) * (CAN_ACCERERATION_RESOLUTION_FR / SIGNAL_VALUE_ACCERERATION_RESOLUTION));
	}
	else
	{
		/* �O������x:���� */
		acc_fr = SIGNAL_VALUE_IGNORE_16_SIGNED;
	}
	if( pCanInfo->en_acc_lr == 1 )
	{
		/* �������x:�L�� */
		acc_lr = (SI_16)((F_64)((SI_8)pCanInfo->acc_lr) * (CAN_ACCERERATION_RESOLUTION_LR / SIGNAL_VALUE_ACCERERATION_RESOLUTION));
	}
	else
	{
		/* �������x:���� */
		acc_lr = SIGNAL_VALUE_IGNORE_16_SIGNED;
	}
	*out = (SI_32)(((UI_32)acc_fr & 0x0000FFFF) | ((UI_32)acc_lr << 16));
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_YawandPitchRate			*/
/* �@�\�̐���	: ���[���[�g�A�s�b�`���[�g�擾			*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_YawandPitchRate(SI_32 *out)
{
	SI_16 sYawrate;
	SI_16 yawrate = SIGNAL_VALUE_IGNORE_16_SIGNED;
	SI_16 pitchrate = SIGNAL_VALUE_IGNORE_16_SIGNED; /* �ݒ�Ȃ� */
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_yaw_rate == 1)
	{
		/* ���[���[�g:�L�� */
		sYawrate = (SI_16)pCanInfo->yaw_rate;
		if( (sYawrate <= (SI_16)CAN_YAWRATE_MAX) && (sYawrate >= (SI_16)CAN_YAWRATE_MIN) )
		{
			/* ����l */
			yawrate = (SI_16)((F_64)sYawrate * (CAN_YAWRATE_RESOLUTION_FR / SIGNAL_VALUE_YAWRATE_RESOLUTION));
		}
		else
		{
			/* �ُ�l */
			/* �����l */
		}
	}
	else
	{
		/* ���[���[�g:���� */
		/* �����l */
	}
	
	*out = (SI_32)(((UI_32)yawrate & 0x0000FFFF) | (((UI_32)pitchrate << 16) & 0xFFFF0000));
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_UnitTemperature			*/
/* �@�\�̐���	: ���j�b�g���x�擾						*/
/*				:�i�G���W�������A�G���W�������A�ϑ��@�����j*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_UnitTemperature(SI_32 *out)
{
	UI_16 tmp_value;
	UI_8 eng_temp_coolant = SIGNAL_VALUE_IGNORE_8;
	UI_8 eng_temp_oil = SIGNAL_VALUE_IGNORE_8;
	UI_8 trans_temp_oil = SIGNAL_VALUE_IGNORE_8;	/* �ݒ�Ȃ� */
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_temp_coolant == 1)
	{            
		/* �G���W������:�L�� */
		if(pCanInfo->temp_coolant <= CAN_TEMP_COOLANT_MAX)
		{
			/* ����l */
			tmp_value = pCanInfo->temp_coolant + CAN_ENG_TEMP_COOLANT_OFFSET - SIGNAL_VALUE_ENG_TEMP_COOLANT_OFFSET;
			if(tmp_value >= 0xFF)
			{
				/* �ő�l�𒴂���ꍇ�͊ۂ߂� */
				tmp_value = 0xFE;
			}
			eng_temp_coolant = (UI_8)tmp_value;
		}
		else
		{
			/* �ُ�l */
			/* �����l */
		}
	}
	else
	{
		/* �G���W������:���� */
		/* �����l */
	}
	
	if(pCanInfo->en_temp_oil == 1)
	{
		/* �G���W������:�L�� */
		tmp_value = pCanInfo->temp_oil + CAN_ENG_TEMP_OIL_OFFSET - SIGNAL_VALUE_ENG_TEMP_OIL_OFFSET;
		if(tmp_value >= 0xFF)
		{
			/* �ő�l�𒴂���ꍇ�͊ۂ߂� */
			tmp_value = 0xFE;
		}
		eng_temp_oil = (UI_8)tmp_value;
	}
	else
	{
		/* �G���W������:���� */
		/* �����l */
	}
	
	*out = (SI_32)((UI_32)eng_temp_coolant | ((UI_32)eng_temp_oil << 8) | ((UI_32)trans_temp_oil << 16));
	return 0;
}

/********************************************************/
/* �֐�����		: get_signal_TirePressureFront			*/
/* �@�\�̐���	: �t�����g�^�C�����擾					*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_TirePressureFront(SI_32 *out)
{
	F_64  unit;
	UI_16 fr = SIGNAL_VALUE_IGNORE_16;
	UI_16 fl = SIGNAL_VALUE_IGNORE_16;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if( (pCanInfo->en_air_press_unit == 1) && (pCanInfo->en_air_press_ftlh == 1) && (pCanInfo->en_air_press_ftrh == 1) )
	{
		/* �^�C����C���P�ʁA�^�C����C��(FL��),�^�C����C��(FR��):�L�� */
		if(pCanInfo->air_press_unit == CAN_TIREPRESS_UNIT_KPA)
		{
			/*�P��kPa*/
			unit = CAN_TIREPRESS_RESOLUTION_KPA;
		}
		else if(pCanInfo->air_press_unit == CAN_TIREPRESS_UNIT_PSI)
		{
			/*�P��psi*/
			/*1kPa = 0.145038 psi */
			/*�P��pis -> kPa*/
			unit = CAN_TIREPRESS_RESOLUTION_PSI * (1 / 0.145038);
		}
		else if(pCanInfo->air_press_unit == CAN_TIREPRESS_UNIT_BAR)
		{
			/*�P��bar*/
			/*1kPa = 0.01bar*/
			/*�P��bar -> kPa*/
			unit = CAN_TIREPRESS_RESOLUTION_BAR * (1 / 0.01);
		}
		else
		{
			/*�P�ʕs��*/
			unit = 0;
		}
		if(unit > 0)
		{
			/*�P�ʗL��*/
			if(pCanInfo->air_press_ftlh != CAN_TIREPRESS_STATUS_CHECKING)
			{
				/*FL�փ`�F�b�N����ԂłȂ�*/
				fl = (UI_16)((F_64)pCanInfo->air_press_ftlh * (unit / SIGNAL_VALUE_TIREPRESS_RESOLUTION));
			}
			else
			{
				/* nop */
			}
			if(pCanInfo->air_press_ftrh != CAN_TIREPRESS_STATUS_CHECKING)
			{
				/*FR�փ`�F�b�N����ԂłȂ�*/
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
		/* �^�C����C���P�ʁA�^�C����C��(FL��),�^�C����C��(FR��):�L�� */
		/*�����l*/
	}
	*out = (SI_32)((UI_32)fl | ((UI_32)fr << 16));
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_TirePressureRear			*/
/* �@�\�̐���	: ���A�^�C�����擾						*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_TirePressureRear(SI_32 *out)
{
	F_64  unit;
	UI_16 rr = SIGNAL_VALUE_IGNORE_16;
	UI_16 rl = SIGNAL_VALUE_IGNORE_16;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if( (pCanInfo->en_air_press_unit == 1) && (pCanInfo->en_air_press_rrlh == 1) && (pCanInfo->en_air_press_rrrh == 1) )
	{
		/* �^�C����C���P�ʁA�^�C����C��(RL��),�^�C����C��(RR��):�L�� */
		if(pCanInfo->air_press_unit == CAN_TIREPRESS_UNIT_KPA)
		{
			/*�P��kPa*/
			unit = CAN_TIREPRESS_RESOLUTION_KPA;
		}
		else if(pCanInfo->air_press_unit == CAN_TIREPRESS_UNIT_PSI)
		{
			/*�P��psi*/
			/*1kPa = 0.145038 psi */
			/*�P��pis -> kPa*/
			unit = CAN_TIREPRESS_RESOLUTION_PSI * (1 / 0.145038);
		}
		else if(pCanInfo->air_press_unit == CAN_TIREPRESS_UNIT_BAR)
		{
			/*�P��bar*/
			/*1kPa = 0.01bar*/
			unit = CAN_TIREPRESS_RESOLUTION_BAR * (1 / 0.01);
		}
		else
		{
			/*�P�ʕs��*/
			unit = 0;
		}
		if(unit > 0)
		{
			/*�P�ʗL��*/
			if(pCanInfo->air_press_rrlh != CAN_TIREPRESS_STATUS_CHECKING)
			{
				/*RL�փ`�F�b�N����ԂłȂ�*/
				rl = (UI_16)((F_64)pCanInfo->air_press_rrlh * (unit / SIGNAL_VALUE_TIREPRESS_RESOLUTION));
			}
			else
			{
				/* nop */
			}
			if(pCanInfo->air_press_rrrh != CAN_TIREPRESS_STATUS_CHECKING)
			{
				/*RR�փ`�F�b�N����ԂłȂ�*/
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
		/* �^�C����C���P�ʁA�^�C����C��(R��),�^�C����C��(RR��):�L�� */
		/*�����l*/
	}
	*out = (SI_32)((UI_32)rl | ((UI_32)rr << 16));
	return 0;
}


/********************************************************/
/* �֐�����		: get_signal_FuelConsumption			*/
/* �@�\�̐���	: �R�����˗ʐώZ�l�擾					*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_FuelConsumption(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_fuel_consumption == 1)
	{
		/* �R�����˗ʐώZ�l:�L�� */
		*out = (SI_32)((F_64)pCanInfo->fuel_consumption * (CAN_FUEL_CONSUMPTION_RESOLUTION / SIGNAL_VALUE_FUEL_CONSUMPTION_RESOLUTION)); 
	}
	else
	{
		/* �R�����˗ʐώZ�l:���� */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_32;
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_DistancefromRestart		*/
/* �@�\�̐���	: �n���㑖�s�����擾					*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_DistancefromRestart(SI_32 *out)
{
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	if(pCanInfo->en_running_distance == 1)
	{
		/* �n���㑖�s����:�L�� */
		*out = (SI_32)pCanInfo->running_distance;
	}
	else
	{
		/* �n���㑖�s����:���� */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_32;
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_OutputTorque				*/
/* �@�\�̐���	: �o�̓g���N�擾						*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_OutputTorque(SI_32 *out)
{
	SI_16 engTrq = SIGNAL_VALUE_IGNORE_16_SIGNED;
	SI_16 motTrq = SIGNAL_VALUE_IGNORE_16_SIGNED;

	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	/* �G���W���g���N���L���`�F�b�N */
	if(pCanInfo->en_engTrcShaft == 1)
	{
		/* �G���W���g���N���F�L�� */
		engTrq = (SI_16)(pCanInfo->engTrcShaft + CAN_ENGTRQ_OFFSET_INC_RESOLUTION);
	}

	*out = (SI_32)(((UI_32)engTrq & 0x0000FFFF) | (((UI_32)motTrq << 16) & 0xFFFF0000));
	return 0;
}

/********************************************************/
/* �֐�����		�F get_signal_EngineSensor				*/
/* �@�\�̐���	�F �G���W���Z���T�[						*/
/* ����			�F ( /O)out				�擾�l			*/
/* �߂�l		�F 0									*/
/* �쐬��		�F (OFT)Sakata							*/
/* �쐬��		�F 21/03/-								*/
/* ���l			�F 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineSensor(SI_32 *out)
{
	UI_16 oil_pressure = SIGNAL_VALUE_IGNORE_16;
	UI_16 intake_pressure = SIGNAL_VALUE_IGNORE_16;
	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	/* �z���ǈ��͗L���`�F�b�N */
	if(pCanInfo->en_intake_press == 1)
	{            
		/* �z���ǈ��́F�L�� */
		/*	�o�͒l�@=�@((�z���ǈ��� x ����\ x �P�ʕϊ�) - �I�t�Z�b�g ) / �o�͕���\�i0.1�j */
		/*	�o�͒l�@=�@((�z���ǈ��� x ����\ x �P�ʕϊ�) / �o�͕���\�i0.1)�j- (�I�t�Z�b�g / �o�͕���\�i0.1�j) */
		intake_pressure = (UI_16)((SI_16)(((F_64)(pCanInfo->intake_press * CAN_INTAKE_PRESS_RESOLUTION) * SIGNAL_CONV_MMHG_TO_KPA) / SIGNAL_VALUE_INTAKE_PRESS_RESOLUTION) - (SI_16)((F_64)SIGNAL_VALUE_INTAKE_PRESS_OFFSET / SIGNAL_VALUE_INTAKE_PRESS_RESOLUTION));
	}
	else
	{
		/* �z���ǈ��́F���� */
		/* �����l */
	}
	*out = (SI_32)((UI_32)oil_pressure | ((UI_32)intake_pressure << 16));
	
	return 0;
}

/********************************************************/
/* �֐�����		�F get_signal_AtmosphericPressure		*/
/* �@�\�̐���	�F ��C��								*/
/* ����			�F ( /O)out				�擾�l			*/
/* �߂�l		�F 0									*/
/* �쐬��		�F (OFT)Sakata							*/
/* �쐬��		�F 21/03/-								*/
/* ���l			�F 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_AtmosphericPressure(SI_32 *out)
{
	UI_16 atm_pressure = SIGNAL_VALUE_IGNORE_16;

	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	/* ��C���L���`�F�b�N */
	if(pCanInfo->en_atmospheric_press == 1)
	{            
		/* ��C���F�L�� */
		/*	�o�͒l�@=�@((�z���ǈ��� x ����\ + �I�t�Z�b�g) x �P�ʕϊ�) / �o�͕���\�i0.1�j */
		atm_pressure = (UI_16)(((F_64)((pCanInfo->atmospheric_press * CAN_ATM_PRESS_RESOLUTION) + CAN_ATM_PRESS_OFFSET) * SIGNAL_CONV_MMHG_TO_KPA) / SIGNAL_VALUE_ATM_PRESS_RESOLUTION);
	}
	else
	{
		/* ��C���F���� */
		/* �����l */
	}
	*out = (SI_32)atm_pressure;
	
	return 0;
}

/********************************************************/
/* �֐�����		�F get_signal_EngineRevRawValue			*/
/* �@�\�̐���	�F �G���W����]���i���l�j				*/
/* ����			�F ( /O)out				�擾�l			*/
/* �߂�l		�F 0									*/
/* �쐬��		�F (OFT)Sakata							*/
/* �쐬��		�F 21/03/-								*/
/* ���l			�F 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineRevRawValue(SI_32 *out)
{
	SI_32 randomValue;
	UI_16 engRevRaw = SIGNAL_VALUE_IGNORE_16;
	UI_8  targetGear = SIGNAL_VALUE_TARGET_GEAR_INIT;

	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	/* �G���W����]��(���l)�L���`�F�b�N */
	if(pCanInfo->en_eng_rpm == 1)
	{
		/* �G���W����]��(���l):�L�� */
		engRevRaw = pCanInfo->eng_rpm;
	}
	else
	{
		/* �G���W����]��(���l):���� */
	}
	
	/*�g�����X�~�b�V������ʗL���`�F�b�N*/
	if(pCanInfo->en_transmission_type == 1)
	{
		/* �g�����X�~�b�V�������:�L�� */
		if(pCanInfo->transmission_type == CAN_TRANSMISSION_ATORCVT)
		{
			/*�g�����X�~�b�V�������:AT or CVT*/
			/*�ڕW�M�A�i���L���`�F�b�N*/
			if(pCanInfo->en_target_gear_pos == 1) 
			{
				/*�ڕW�M�A�i���F�L��*/
				switch (pCanInfo->target_gear_pos)
				{
					case	CAN_TARGET_GEAR_N:		/* AT�V�t�g:N	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_N;	
						break;
					case	CAN_TARGET_GEAR_D:		/* AT�V�t�g:D	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_D;	
						break;
					case	CAN_TARGET_GEAR_R:		/* AT�V�t�g:R	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_R;	
						break;
					case	CAN_TARGET_GEAR_P:		/* AT�V�t�g:P	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_P;	
						break;
					case	CAN_TARGET_GEAR1ST:		/* AT�M�A�F1ST	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_1ST;	
						break;
					case	CAN_TARGET_GEAR2ND:		/* AT�M�A�F2ND	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_2ND;	
						break;
					case	CAN_TARGET_GEAR3RD:		/* AT�M�A�F3RD	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_3RD;	
						break;
					case	CAN_TARGET_GEAR4TH:		/* AT�M�A�F4TH	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_4TH;	
						break;
					case	CAN_TARGET_GEAR5TH:		/* AT�M�A�F5TH	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_5TH;	
						break;
					case	CAN_TARGET_GEAR6TH:		/* AT�M�A�F6TH	*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_6TH;	
						break;
					default:
						/*���̑�*/
						targetGear = SIGNAL_VALUE_TARGET_GEAR_INVALID;
						break;
				}
			}
			else
			{
				/*�ڕW�M�A�i���F����*/
				/*�����Ȃ��F�����l*/
			}
		}else{
			/*�g�����X�~�b�V�������:MT*/
			targetGear = SIGNAL_VALUE_TARGET_GEAR_INVALID;
		}
	}
	else
	{
		/*�g�����X�~�b�V�����F����*/
		/*�����Ȃ��F�����l*/
	}
	
	/* �����擾 */
	(void)get_signal_RandomValue(&randomValue);

	*out = (SI_32)(engRevRaw | ((UI_32)targetGear << 16) | ((UI_32)randomValue & 0xFF000000));
	
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_OutputUpDwnTorque			*/
/* �@�\�̐���	: �o�̓A�b�v�_�E���g���N�擾			*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 21/03/-								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_OutputUpDwnTorque(SI_32 *out)
{
	UI_16 engTrqUp = SIGNAL_VALUE_IGNORE_16;
	UI_16 engTrqDwn = SIGNAL_VALUE_IGNORE_16;

	T_Can_ctrl_tcan_info* pCanInfo = Can_control_GetTCanInfo();
	/* �G���W���g���N�A�b�v�v���l�L���`�F�b�N */
	if(pCanInfo->en_engTrqUpReqVal == 1)
	{
		/* �G���W���g���N�A�b�v�v���l�F�L�� */
		if (pCanInfo->engTrqUpReqVal == CAN_ENGTRQ_REQ_MAXVAL)
		{
			/*�ő�l�̏ꍇ�|�P����F�����l�Ƃ��Ȃ�����*/
			engTrqUp = pCanInfo->engTrqUpReqVal - 1;
		}
		else
		{
			/*�ő�l�ȊO�̏ꍇ�A���̂܂ܐݒ�*/
			engTrqUp = pCanInfo->engTrqUpReqVal;
		}
	}

	/* �G���W���g���N�_�E���v���l�L���`�F�b�N */
	if(pCanInfo->en_engTrqDwnReqVal == 1)
	{
		/* �G���W���g���N�_�E���v���l�F�L�� */
		/*���̂܂ܐݒ�i�ő�l�̃`�F�b�N�͕s�v:
		CAN_ENGTRQ_REQ_MAXVAL�̂Ƃ��̓t�F�[�����Ȃ̂�en_engTrqDwnReqVal��0�ƂȂ�j*/
		engTrqDwn = pCanInfo->engTrqDwnReqVal;
	}
	
	*out = (SI_32)(engTrqUp | (((UI_32)engTrqDwn << 16) & 0xFFFF0000));
	return 0;
}
#ifdef ADVANCE_ENABLE
/********************************************************/
/* �֐�����		: get_signal_IntakeManifoldPressure		*/
/* �@�\�̐���	: �z�C���擾							*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IntakeManifoldPressure(SI_32 *out)
{
	T_ADVANCE_CTRL_UNIT_INFO* pAdvanceInfo;
	/* Advance Control Unit�̐M���擾 */
	Advance_control_GetData(&pAdvanceInfo);
	if(pAdvanceInfo->invalidFlag == 1)
	{
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	else
	{
		*out = (SI_32)pAdvanceInfo->TurboPressure;/* �^�[�{��(�ߋ���) */
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_EngineOilPressure			*/
/* �@�\�̐���	: �G���W�������擾						*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineOilPressure(SI_32 *out)
{
	T_ADVANCE_CTRL_UNIT_INFO* pAdvanceInfo;
	/* Advance Control Unit�̐M���擾 */
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
/* �֐�����		: get_signal_FuelPressure				*/
/* �@�\�̐���	: �R�����͎擾							*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_FuelPressure(SI_32 *out)
{
	T_ADVANCE_CTRL_UNIT_INFO* pAdvanceInfo;
	/* Advance Control Unit�̐M���擾 */
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
/* �֐�����		: get_signal_EngineCoolantTempertureExt1*/
/* �@�\�̐���	: �G���W�������i�g��1�j�擾				*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineCoolantTempertureExt1(SI_32 *out)
{
	T_ADVANCE_CTRL_UNIT_INFO* pAdvanceInfo;
	/* Advance Control Unit�̐M���擾 */
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
/* �֐�����		: get_signal_EngineCoolantTempertureExt2*/
/* �@�\�̐���	: �G���W�������i�g��2�j�擾				*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineCoolantTempertureExt2(SI_32 *out)
{
	*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_EngineOilTemperture		*/
/* �@�\�̐���	: �G���W�������擾						*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_EngineOilTemperture(SI_32 *out)
{
	T_ADVANCE_CTRL_UNIT_INFO* pAdvanceInfo;
	/* Advance Control Unit�̐M���擾 */
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
/* �֐�����		: get_signal_ExhaustGasTemperture		*/
/* �@�\�̐���	: �r�C���x�擾							*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_ExhaustGasTemperture(SI_32 *out)
{
	T_ADVANCE_CTRL_UNIT_INFO* pAdvanceInfo;
	/* Advance Control Unit�̐M���擾 */
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
/* �֐�����		: get_signal_Volts						*/
/* �@�\�̐���	: �d���擾								*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_Volts(SI_32 *out)
{
	T_ADVANCE_CTRL_UNIT_INFO* pAdvanceInfo;
	/* Advance Control Unit�̐M���擾 */
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
/* �֐�����		: get_signal_AirFuelRatio				*/
/* �@�\�̐���	: �󈳔�擾							*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_AirFuelRatio(SI_32 *out)
{
	*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	return 0;
}
#endif /*ADVANCE_ENABLE*/
#ifdef	ISOCAN_ENABLE
/********************************************************/
/* �֐�����		: get_signal_IsoCanVehicleSpeed			*/
/* �@�\�̐���	: �ԑ��擾								*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IsoCanVehicleSpeed(SI_32 *out)
{
	T_Can_ctrl_iso_can_info* pIsoCanInfo = Can_control_GetIsoCanInfo();
	if(pIsoCanInfo->en_vehicle_speed == 1)
	{
		/* �ԑ�:�L�� */
		*out = pIsoCanInfo->vehicle_speed;
	}
	else
	{
		/* �ԑ�:���� */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_IsoCanEngineRev			*/
/* �@�\�̐���	: �G���W����]���擾					*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IsoCanEngineRev(SI_32 *out)
{
	T_Can_ctrl_iso_can_info* pIsoCanInfo = Can_control_GetIsoCanInfo();
	if(pIsoCanInfo->en_eng_rpm == 1)
	{
		/* �G���W����]��:�L�� */
		*out = pIsoCanInfo->eng_rpm * 0.25;
	}
	else
	{
		/* �G���W����]��:���� */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_IsoCanIntakeManifoldPressure*/
/* �@�\�̐���	: �z�C���擾							*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IsoCanIntakeManifoldPressure(SI_32 *out)
{
	T_Can_ctrl_iso_can_info* pIsoCanInfo = Can_control_GetIsoCanInfo();
	if(pIsoCanInfo->en_ima_pressure == 1)
	{
		/* �z�C��:�L�� */
		*out = pIsoCanInfo->ima_pressure;
	}
	else
	{
		/* �z�C��:���� */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_IsoCanEngineCoolantTemperture*/
/* �@�\�̐���	: �G���W�������擾						*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IsoCanEngineCoolantTemperture(SI_32 *out)
{
	T_Can_ctrl_iso_can_info* pIsoCanInfo = Can_control_GetIsoCanInfo();
	if(pIsoCanInfo->en_temp_colant == 1)
	{
		/* �G���W������:�L�� */
		*out = pIsoCanInfo->temp_colant + ISOCAN_ENG_TEMP_COOLANT_OFFSET;
	}
	else
	{
		/* �G���W������:���� */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_IsoCanVolts				*/
/* �@�\�̐���	: �d���擾								*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IsoCanVolts(SI_32 *out)
{
	T_Can_ctrl_iso_can_info* pIsoCanInfo = Can_control_GetIsoCanInfo();
	if(pIsoCanInfo->en_volts == 1)
	{
		/* �d��:�L�� */
		*out = (SI_32)pIsoCanInfo->volts * (0.001 / 0.1);
	}
	else
	{
		/* �d��:���� */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_IsoCanThrottle				*/
/* �@�\�̐���	: �X���b�g���J�x�擾					*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IsoCanThrottle(SI_32 *out)
{
	T_Can_ctrl_iso_can_info* pIsoCanInfo = Can_control_GetIsoCanInfo();
	if(pIsoCanInfo->en_throttle_pos == 1)
	{
		/* �X���b�g���J�x:�L�� */
		/* PID�ŐV.xls�Ɍv�Z���@�L�q */
		*out = pIsoCanInfo->throttle_pos * 100 / 255;
	}
	else
	{
		/* �X���b�g���J�x:���� */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
/********************************************************/
/* �֐�����		: get_signal_IsoCanIntakeTemperature	*/
/* �@�\�̐���	: �z�C���x�擾							*/
/* ����			: ( /O)out				�擾�l			*/
/* �߂�l		: 0										*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SIGNAL_LOCAL UI_16 get_signal_IsoCanIntakeTemperature(SI_32 *out)
{
	T_Can_ctrl_iso_can_info* pIsoCanInfo = Can_control_GetIsoCanInfo();
	if(pIsoCanInfo->en_temp_intake_manifold == 1)
	{
		/* �z�C���x:�L�� */
		*out = pIsoCanInfo->temp_intake_manifold;
	}
	else
	{
		/* �z�C���x:���� */
		*out = (SI_32)SIGNAL_VALUE_IGNORE_16;
	}
	return 0;
}
#endif	/* ISOCAN_ENABLE */

