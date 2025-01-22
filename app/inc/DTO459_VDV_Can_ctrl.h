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
/* CAN����												*/
/********************************************************/
#ifndef	DTO459_VDV_CAN_CTRL_H
#define	DTO459_VDV_CAN_CTRL_H

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include "type.h"

/********************************************************/
/*	�}�N����`											*/
/********************************************************/
/* T-CAN�ԗ����\���� */
typedef struct {
									/*[ID:152]***********************************/
	UI_8	en_ign:1;				/* �f�[�^�L���t���O:IGN						*/
	UI_8	en_acc:1;				/* 					 ACC					*/
	UI_8	en_brake_lamp_sw:1;		/* 					 �u���[�L�����vSW		*/
	UI_8	en_hand_brake_sw:1;		/* 					 �p�[�L���O�����vSW		*/
	UI_8	en_mt_rev_sw:1;			/* 					 MT���o�[�XSW			*/
	UI_8	en_head_lamp_hi:1;		/* 					 �w�b�h�����v Hi�r�[��	*/
	UI_8	en_head_lamp_lo:1;		/* 					 �w�b�h�����v Lo�r�[��	*/
	UI_8	en_head_lamp_small:1;	/* 					 �w�b�h�����v �X���[��	*/
									/*------------------------------------------*/
	UI_8	ign:1;					/* �f�[�^����:IGN 							*/
	UI_8	acc:1;					/* 			   ACC 							*/
	UI_8	brake_lamp_sw:1;		/* 			   �u���[�L�����vSW 			*/
	UI_8	hand_brake_sw:1;		/* 			   �p�[�L���O�����vSW 			*/
	UI_8	mt_rev_sw:1;			/* 			   MT���o�[�XSW 				*/
	UI_8	head_lamp_hi:1;			/* 			   �w�b�h�����v Hi�r�[��	 	*/
	UI_8	head_lamp_lo:1;			/* 			   �w�b�h�����v Lo�r�[�� 		*/
	UI_8	head_lamp_small:1;		/* 			   �w�b�h�����v �X���[�� 		*/
									/*											*/
									/*[ID:374]***********************************/
	UI_8	en_door_sw_trunk:1;		/* �f�[�^�L���t���O:�h�ASW �g�����N			*/
	UI_8	en_door_sw_seat_p:1;	/* 					 �h�ASW P��				*/
	UI_8	en_door_sw_seat_d:1;	/* 					 �h�ASW D��				*/
	UI_8	en_door_sw_hood:1;		/* 					 �h�ASW �t�[�h			*/
									/*------------------------------------------*/
	UI_8	door_sw_trunk:1;		/* �f�[�^����:�h�ASW �g�����N				*/
	UI_8	door_sw_seat_p:1;		/* 			   �h�ASW P��					*/
	UI_8	door_sw_seat_d:1;		/* 			   �h�ASW D��					*/
	UI_8	door_sw_hood:1;			/* 			    �h�ASW �t�[�h				*/
									/*											*/
									/*[ID:282]***********************************/
	UI_8	en_out_temp:1;			/* �f�[�^�L���t���O:�\���O�C��				*/
	UI_8	en_seat_belt_sw_p:1;	/* 					 �V�[�g�x���g�o�b�N��SW P��	*/
	UI_8	en_seat_belt_sw_d:1;	/* 					 �V�[�g�x���g�o�b�N��SW D��	*/
	UI_8	en_destination:1;		/* 					 �d���n���				*/
	UI_8	en_brand_flag:1;		/* 					 �u�����h�t���O			*/
	UI_8	en_blinker_left:1;		/* 					 �E�B���J�[��			*/
	UI_8	en_blinker_right:1;		/* 			   		 �E�B���J�[�E			*/
									/*------------------------------------------*/
	UI_8	out_temp:8;				/* �f�[�^����:�\���O�C��					*/
	UI_8	seat_belt_sw_p:1;		/* 			   �V�[�g�x���g�o�b�N��SW P��	*/
	UI_8	seat_belt_sw_d:1;		/* 			   �V�[�g�x���g�o�b�N��SW D��	*/
	UI_8	destination:4;			/* 			   �d���n���					*/
	UI_8	brand_flag:1;			/* 			   �u�����h�t���O				*/
	UI_8	blinker_left:1;			/* 			   �E�B���J�[��					*/
	UI_8	blinker_right:1;		/* 			   �E�B���J�[�E					*/
									/*											*/
									/*[ID:140]***********************************/
	UI_8	en_accel_ratio:1;		/* �f�[�^�L���t���O:�A�N�Z���J�x			*/
	UI_8	en_eng_rpm:1;			/* 					 �G���W����]��			*/
									/*------------------------------------------*/
	UI_8	accel_ratio:8;			/* �f�[�^����:�A�N�Z���J�x					*/
	UI_16	eng_rpm:14;				/* 			   �G���W����]��				*/
									/*											*/
									/*[ID:141]***********************************/
	UI_8	en_eng_rpm_taco:1;		/* �f�[�^�L���t���O:�G���W����]���^�R���[�^�p	*/
									/*------------------------------------------*/
	UI_16	eng_rpm_taco:14;		/* �f�[�^����:�G���W����]���^�R���[�^�p	*/
									/*											*/
									/*[ID:144]***********************************/
	UI_8	en_transmission_type:1;	/* �f�[�^�L���t���O:�g�����X�~�b�V�������	*/
									/*------------------------------------------*/
	UI_8	transmission_type:1;	/* �f�[�^����:�g�����X�~�b�V�������		*/
									/*											*/
									/*[ID:360]***********************************/
	UI_8	en_temp_oil:1;			/* �f�[�^�L���t���O:�G���W������			*/
	UI_8	en_temp_coolant:1;		/* 					 �G���W������			*/
	UI_8	en_cruise_controlrl_set:1;	/* 				 �N���R���Z�b�g�����v	*/
	UI_8	en_intake_press;		/* 					 �z���ǈ���				*/
	UI_8	en_atmospheric_press;	/* 					 ��C��					*/
									/*------------------------------------------*/
	UI_8	temp_oil;				/* �f�[�^����:�G���W������					*/
	UI_8	temp_coolant;			/* 			   �G���W������					*/
	UI_8	cruise_controlrl_set:1;	/* 			   �N���R���Z�b�g�����v			*/
	UI_8	intake_press;			/* 			   �z���ǈ���					*/
	UI_8	atmospheric_press;		/* 			   ��C��						*/
									/*											*/
									/*[ID:361]***********************************/
	UI_8	en_mt_gear_pos:1;		/* �f�[�^�L���t���O:MT�M�A�|�W�V����		*/
									/*------------------------------------------*/
	UI_8	mt_gear_pos:3;			/* �f�[�^����:MT�M�A�|�W�V����				*/
									/*											*/
									/*[ID:148]***********************************/
	UI_8	en_sports_gear_pos:1;	/* �f�[�^�L���t���O:�X�|�[�c�M�A�|�W�V����	*/
	UI_8	en_shift_pos:1;			/* 					 �V�t�g�|�W�V�����i�t�F�[���Z�[�t������j*/
	UI_8	en_mode_shift_control:1;/* 					 TCM�ϑ����䃂�[�h		*/
	UI_8	en_target_gear_pos:1;	/* 					 �ڕW�M�A�ʒu			*/
									/*------------------------------------------*/
	UI_8	sports_gear_pos:4;		/* �f�[�^����:�X�|�[�c�M�A�|�W�V����		*/
	UI_8	shift_pos:4;			/* 			   �V�t�g�|�W�V�����i�t�F�[���Z�[�t������j	*/
	UI_8	mode_shift_control:3;	/* 			   TCM�ϑ����䃂�[�h			*/
	UI_8	target_gear_pos:4;		/* 			   �ڕW�M�A�ʒu					*/
									/*											*/
									/*[ID:368]***********************************/
	UI_8	en_at_type:1;			/* �f�[�^�L���t���O:AT���					*/
	UI_8	en_sports_lamp:1;		/* 					 �X�|�[�c�����v			*/
	UI_8	en_snow_hold_lamp:1;	/* 					 T�X�m�[�z�[���h�����v	*/
									/*------------------------------------------*/
	UI_8	at_type:3;				/* �f�[�^����:AT���						*/
	UI_8	sports_lamp:2;			/* 			   �X�|�[�c�����v				*/
	UI_8	snow_hold_lamp:1;		/* 			   �X�m�[�z�[���h�����v			*/
									/*											*/
									/*[ID:375]***********************************/
	UI_8	en_lock_stat_pass:1;	/* �f�[�^�L���t���O:�h�A���b�N��ԁi����ȁj*/
	UI_8	en_lock_stat_driver:1;	/* 					 �h�A���b�N��ԁi�^�]�ȁj*/
									/*------------------------------------------*/
	UI_8	lock_stat_pass:1;		/* �f�[�^����:�h�A���b�N��ԁi����ȁj		*/
	UI_8	lock_stat_driver:1;		/* 			   �h�A���b�N��ԁi�^�]�ȁj		*/
									/*											*/
									/*[ID:0d0]***********************************/
	UI_8	en_steer_angle:1;		/* �f�[�^�L���t���O:�Ǌp�Z���T				*/
	UI_8	en_yaw_rate:1;			/* 					 ���[���[�g				*/
	UI_8	en_acc_lr:1;			/* 					 �������x				*/
	UI_8	en_acc_fr:1;			/* 					 �O��f�Z���T			*/
									/*------------------------------------------*/
	UI_16	steer_angle;			/* �f�[�^����:�Ǌp�Z���T					*/
	UI_16	yaw_rate;				/* 			   ���[���[�g					*/
	UI_8	acc_lr;					/* 			   �������x						*/
	UI_8	acc_fr;					/* 			   �O��f�Z���T					*/
									/*											*/
									/*[ID:0d1]***********************************/
	UI_8	en_wheel_speed_ave:1;	/* �f�[�^�L���t���O:�쓮�֕��ώԗ֑�		*/
	UI_8	en_brake_pressure:1;	/*					 �u���[�L��				*/
									/*------------------------------------------*/
	UI_16	wheel_speed_ave;		/* �f�[�^����:�쓮�֕��ώԗ֑�				*/
	UI_8	brake_pressure;			/*			   �u���[�L��					*/
									/*											*/
									/*[ID:0d3]***********************************/
	UI_8	en_speed_pulse_integ:1;	/* �f�[�^�L���t���O:�ԑ��p���X�M���ώZ�l	*/
									/*------------------------------------------*/
	UI_32	speed_pulse_integ:6;	/* �f�[�^����:�ԑ��p���X�M���ώZ�l			*/
									/*											*/
									/*[ID:0d4]***********************************/
	UI_8	en_speed_wheel_ftlh:1;	/* �f�[�^�L���t���O:���O�֎ԗ֑�			*/
	UI_8	en_speed_wheel_ftrh:1;	/* 					�E�O�֎ԗ֑�			*/
	UI_8	en_speed_wheel_rrlh:1;	/* 					����֎ԗ֑�			*/
	UI_8	en_speed_wheel_rrrh:1;	/* 					�E�O�֎ԗ֑�			*/
									/*------------------------------------------*/
	UI_16	speed_wheel_ftlh;		/* �f�[�^����:���O�֎ԗ֑�					*/
	UI_16	speed_wheel_ftrh;		/* 			  �E�O�֎ԗ֑�					*/
	UI_16	speed_wheel_rrlh;		/* 			  ����֎ԗ֑�					*/
	UI_16	speed_wheel_rrrh;		/* 			  �E�O�֎ԗ֑�					*/
									/*											*/
									/*[ID:44d]***********************************/
	UI_8	en_node_id:1;			/* �f�[�^�L���t���O:�m�[�hID				*/
	UI_8	en_sleep_ack:1;			/* 				     Sleep Ack				*/
	UI_8	en_sleep_ind:1;			/* 				     Sleep Ind				*/
	UI_8	en_limp_home:1;			/* 				     Limp Home				*/
	UI_8	en_ring:1;				/* 				     Ring					*/
	UI_8	en_alive:1;				/* 				     Alive					*/
	UI_8	en_wkup:1;				/* 				     Wkup					*/
	UI_8	en_init:1;				/* 				     Init					*/
	UI_8	en_spec_no:1;			/* 				     Spec. No.				*/
									/*------------------------------------------*/
	UI_8	node_id;				/* �f�[�^����:�m�[�hID						*/
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
									/* �f�[�^�L���t���O�F						*/
	UI_8	en_air_press_ftlh:1;	/* 			���O�֋�C��					*/
	UI_8	en_air_press_ftrh:1;	/* 			�E�O�֋�C��					*/
	UI_8	en_air_press_rrlh:1;	/* 			����֋�C��					*/
	UI_8	en_air_press_rrrh:1;	/* 			�E��֋�C��					*/
	UI_8	en_air_press_unit:1;	/* 			��C��	�P�ʏ��				*/
									/*------------------------------------------*/
									/* �f�[�^���́F								*/
	UI_8	air_press_ftlh:7;		/* 			���O�֋�C��					*/
	UI_8	air_press_ftrh:7;		/* 			�E�O�֋�C��					*/
	UI_8	air_press_rrlh:7;		/* 			����֋�C��					*/
	UI_8	air_press_rrrh:7;		/* 			�E��֋�C��					*/
	UI_8	air_press_unit:2;		/* 			��C��	�P�ʏ��				*/
									/*											*/
									/*[ID:048]***********************************/
									/* �f�[�^�L���t���O�F						*/
	UI_8	en_manual_mode_sw:1;	/* 			�}�j���A�����[�hSW				*/
									/*------------------------------------------*/
									/* �f�[�^���́F								*/
	UI_8	manual_mode_sw:1;		/* 			�}�j���A�����[�hSW				*/
									/*[ID:139]***********************************/
									/* �f�[�^�L���t���O�F						*/
	UI_8	en_trcAct:1;			/* 			TRC������						*/
	UI_8	en_absAct:1;			/* 			ABS������						*/
	UI_8	en_absFail:1;			/* 			ABS�V�X�e���t�F�C�����			*/
	UI_8	en_vscUnderStr:1;		/* 			VSC�A���_�[�X�e�A���			*/
	UI_8	en_vscOverStr:1;		/* 			VSC�I�[�o�[�X�e�A���			*/
									/*------------------------------------------*/
									/* �f�[�^���́F 							*/
	UI_8	trcAct:1;				/* 			TRC������						*/
	UI_8	absAct:1;				/* 			ABS������						*/
	UI_8	absFail:1;				/* 			ABS�V�X�e���t�F�C�����			*/
	UI_8	vscUnderStr:1;			/* 			VSC�A���_�[�X�e�A���			*/
	UI_8	vscOverStr:1;			/* 			VSC�I�[�o�[�X�e�A���			*/
									/*											*/
									/*[ID:328]***********************************/
									/* �f�[�^�L���t���O�F						*/
	UI_8	en_vscAct:1;			/* 			VSC������						*/
	UI_8	en_vscOnOff:1;			/* 			VSCON/OFF���					*/
	UI_8	en_trcOnOff:1;			/* 			TRCON/OFF���					*/
	UI_8	en_trcMode:1;			/* 			�g���N�V�����d�����[�h���		*/
	UI_8	en_vscWarning:1;		/* 			VSC�E�H�[�j���O���				*/
	UI_8	en_absWarning:1;		/* 			ABS�E�H�[�j���O���				*/
	UI_8	en_ecbWarning:1;		/* 			ECB�E�H�[�j���O���				*/
									/*------------------------------------------*/
									/* �f�[�^���́F 							*/
	UI_8	vscAct:1;				/* 			VSC������						*/
	UI_8	vscOnOff:1;				/* 			VSCON/OFF���					*/
	UI_8	trcOnOff:1;				/* 			TRCON/OFF���					*/
	UI_8	trcMode:1;				/* 			�g���N�V�����d�����[�h���		*/
	UI_8	vscWarning:1;			/* 			VSC�E�H�[�j���O���				*/
	UI_8	absWarning:1;			/* 			ABS�E�H�[�j���O���				*/
	UI_8	ecbWarning:1;			/* 			ECB�E�H�[�j���O���				*/
									/*											*/
									/*[ID:13B]***********************************/
									/* �f�[�^�L���t���O�F						*/
	UI_8	en_engTrqUpReqFlag:1;	/* 			�G���W���g���N�A�b�v�v���t���O	*/
	UI_8	en_engTrqDwnReqFlag:1;	/* 			�G���W���g���N�����v���t���O	*/
	UI_8	en_engTrqUpReqVal:1;	/* 			�G���W���g���N�A�b�v�v���l		*/
	UI_8	en_engTrqDwnReqVal:1;	/* 			�G���W���g���N�����v���l		*/
									/*------------------------------------------*/
									/* �f�[�^���́F 							*/
	UI_8	engTrqUpReqFlag:1;		/* 			�G���W���g���N�A�b�v�v���t���O	*/
	UI_8	engTrqDwnReqFlag:1;		/* 			�G���W���g���N�����v���t���O	*/
	UI_16	engTrqUpReqVal;			/* 			�G���W���g���N�A�b�v�v���l		*/
	UI_16	engTrqDwnReqVal;		/* 			�G���W���g���N�����v���l		*/
									/*[ID:041]***********************************/
									/* �f�[�^�L���t���O�F						*/
	UI_8	en_engTrcShaft:1;		/* 			���G���W�����g���N				*/
									/*------------------------------------------*/
									/* �f�[�^���́F 							*/
	UI_16	engTrcShaft:15;			/* 			���G���W�����g���N				*/
									/*[�����ώZ�l]*******************************/
									/* �f�[�^�L���t���O�i�o�b�N�A�b�v�j�F		*/
	UI_8	en_eep_vehicle_info:1;	/* 			EEPROM�ێ��Ԏ���				*/
									/*------------------------------------------*/
									/* �f�[�^���́F�i�o�b�N�A�b�v�j				*/
	UI_16	eep_vehicle_info;		/* 			�Ԏ���						*/
									/*[�����ώZ�l]*******************************/
	UI_8	en_running_distance:1;	/* �f�[�^�L���t���O:�n���㑖�s�����擾		*/
	UI_8	en_fuel_consumption:1;	/* 					 �R�����˗ʐώZ�l		*/
	UI_8	en_vehicle_speed_for_disp;/*                 �\���p�ԑ�				*/
	UI_8	en_smoothing_brake_pressure:1;/*			 �Ȃ܂��u���[�L��		*/
	UI_8	en_vehicle_info:1;		/* 					 EEPROM�ێ��Ԏ���		*/
									/*------------------------------------------*/
	UI_32	running_distance;		/* �f�[�^����:�n���㑖�s�����擾			*/
	UI_32	fuel_consumption;		/* 			   �R�����˗ʐώZ�l				*/
	UI_32	vehicle_speed_for_disp;	/*			   �\���p�ԑ�(0.00001km/h�P��)	*/
	UI_8	smoothing_brake_pressure;/*			   �Ȃ܂��u���[�L��				*/
	UI_16	vehicle_info;			/* 			   �Ԏ���(EEPROM�Ƃ͒�`�l���قȂ�)*/
} T_Can_ctrl_tcan_info;

/* ISO-CAN�ԗ����\���� */
typedef struct {
	UI_8	en_temp_colant:1;		/* �f�[�^�L���t���O:�G���W������			*/
	UI_8	en_ima_pressure:1;		/* 					 Intake manifold absolute pressure	*/
	UI_8	en_eng_rpm:1;			/* 					 �G���W����]��			*/
	UI_8	en_vehicle_speed:1;		/* 					 �ԑ�					*/
	UI_8	en_throttle_pos:1;		/* 					 �X���b�g���ʒu			*/
	UI_8	en_volts:1;				/* 			 		 �d��					*/
	UI_8	en_temp_intake_manifold:1;/* 				 �z�C���x				*/
									/*------------------------------------------*/
	UI_8	temp_colant;			/* �f�[�^����:�G���W������					*/
	UI_8	ima_pressure;			/* 			   Intake manifold absolute pressure	*/
	UI_16	eng_rpm;				/* 			   �G���W����]�� 				*/
	UI_8	vehicle_speed;			/* 			   �ԑ�	 						*/
	UI_8	throttle_pos;			/* 			   �X���b�g���ʒu				*/
	UI_16	volts;					/* 			   �d��							*/
	UI_8	temp_intake_manifold;	/* 			   �z�C���x						*/
} T_Can_ctrl_iso_can_info;


/* CAN�M��:�u�����h�t���O */
#define CAN_BRAND_FLAG_TMC							0			/*	�u�����h�t���O:TMC												*/
#define CAN_BRAND_FLAG_FHI							1			/*	�u�����h�t���O:FHI												*/
/* CAN�M��:�d������� */
#define CAN_DESTINATION_JP							1			/*	�d����:���{����													*/
#define CAN_DESTINATION_US							2			/*	�d����:�č�														*/
#define CAN_DESTINATION_CA							3			/*	�d����:�J�i�_													*/
#define CAN_DESTINATION_EU							4			/*	�d����:���B														*/
#define CAN_DESTINATION_K4							5			/*	�d����:���														*/
#define CAN_DESTINATION_KS							6			/*	�d����:���ߓ�													*/
#define CAN_DESTINATION_UK							7			/*	�d����:�C�M���X													*/
#define CAN_DESTINATION_TH							8			/*	�d����:�^�C�E�V���K�|�[��										*/
#define CAN_DESTINATION_AU							9			/*	�d����:�I�[�X�g�����A											*/
#define CAN_DESTINATION_CN							10			/*	�d����:����														*/
#define CAN_DESTINATION_KR							11			/*	�d����:�؍�														*/
#define CAN_DESTINATION_KC							12			/*	�d����:��A�t���J��												*/
/* CAN�M��:�g�����X�~�b�V������� */
#define	CAN_TRANSMISSION_ATORCVT					0x00		/*	�g�����X�~�b�V������� 00: AT or CVT							*/
#define	CAN_TRANSMISSION_MT							0x01		/*	�g�����X�~�b�V������� 01: MT									*/
/* CAN�M��:AT��� */
#define	CAN_AT_TYPE_CVT								0x01		/*	AT���:CVT														*/
#define	CAN_AT_TYPE_4AT								0x02		/*	AT���:4AT														*/
#define	CAN_AT_TYPE_5AT								0x03		/*	AT���:5AT														*/
#define	CAN_AT_TYPE_6AT								0x04		/*	AT���:6AT														*/
/* CAN�M��:�h�A��� */
#define CAN_DOOR_SW_OPEN							0x01		/* �h�ASW��ԃI�[�v��												*/
#define CAN_DOOR_SW_CLOSE							0x00		/* �h�ASW��ԃN���[�Y												*/
#define CAN_DOOR_LOCK								0x00		/* �h�A���b�N��ԃ��b�N												*/
#define CAN_DOOR_UNLOCK								0x01		/* �h�A���b�N��ԃA�����b�N											*/
/* CAN�M��:�V�[�g�x���g��� */
#define CAN_SEATBELT_ON								0x00		/* �V�[�g�x���g��ԑ���												*/
#define CAN_SEATBELT_OFF							0x01		/* �V�[�g�x���g��Ԕ񑕒�											*/
/* CAN�M��:�X�|�[�c�����v��� */
#define	CAN_SPORTLAMP_TURN_OFF						0			/* �X�|�[�c�����v :����												*/
#define	CAN_SPORTLAMP_TURN_ON						1			/* �X�|�[�c�����v :�_��												*/
#define	CAN_SPORTLAMP_BLINK_2Hz						2			/* �X�|�[�c�����v :2Hz�_��											*/
#define	CAN_SPORTLAMP_BLINK_4Hz						3			/* �X�|�[�c�����v :4Hz�_��											*/
/* CAN�M��:�X�m�[�z�[���h�����v��� */
#define	CAN_SNOWLAMP_ON								1			/* �X�m�[�����v :ON													*/
#define	CAN_SNOWLAMP_OFF							0			/* �X�m�[�����v :OFF												*/
/* CAN�M��:�u���[�L�����vON/OFF */
#define	CAN_BRAKE_LAMP_ON							1			/*	�u���[�L�����v: ON												*/
#define	CAN_BRAKE_LAMP_OFF							0			/*	�u���[�L�����v: OFF												*/
/* CAN�M��:�w�b�h���C�gON/OFF */
#define	CAN_LIGHT_TURN_ON							1			/*	�w�b�h���C�g: ON												*/
#define	CAN_LIGHT_TURN_OFF							0			/*	�w�b�h���C�g: OFF												*/
/* CAN�M��:MT���o�[�XSW */
#define	CAN_MT_REVERSE_ON							1			/*	MT���o�[�X: ON�i���o�[�X��ԁj									*/
#define	CAN_MT_REVERSE_OFF							0			/*	MT���o�[�X: OFF													*/
/* CAN�M��:IG,ACC */
#define	CAN_POWER_ON								1			/*	�d���iACC,IG): ON												*/
#define	CAN_POWER_OFF								0			/*	�d���iACC,IG): OFF												*/
/* CAN�M��:����ԑ� */
#define	CAN_WHEELSPEED_RESOLUTION					0.05625		/*	�S�֎ԑ�����\�ϊ��p:0.05625�i���Z��A����\1�j					*/
#define	CAN_ACCERERATION_RESOLUTION_LR				0.1923		/*	�����������x����\�i���Z��A����\1�j							*/
#define	CAN_ACCERERATION_RESOLUTION_FR				0.1			/*	�O����������x����\�i���Z��A����\1�j							*/
/* CAN�M��:�X�e�A�����O����\ */
#define	CAN_STEERING_RESOLUTION						0.1			/*	�X�e�A�����O����\�i���Z��A����\1�j							*/
/* CAN�M��:�X�e�A�����O�ُ�l */
#define	CAN_STEERING_ERRORVALUE						0x8000		/*	�X�e�A�����O�ُ�l												*/
/* CAN�M��:�u���[�L������\ */
#define	CAN_BRAKEPRESSURE_RESOLUTION				0.1305882	/*	�u���[�L������\ 0.1305882�i���Z��A����\1�j					*/
#define	CAN_BRAKEPRESSURE_MAX_86A					11.4		/*	�u���[�L�����ő�l�̃Z���T�[����l19Mpa��60%					*/
#define	CAN_BRAKEPRESSURE_MAX_86B					13.5			/*	�u���[�L�����ő�l�̃Z���T�[����l15Mpa��90%					*/
/* CAN�M��:���[���C�g */
#define	CAN_YAWRATE_RESOLUTION_FR					0.0047358	/* ���[���C�g����\�i���Z��A����\1�j								*/
#define	CAN_YAWRATE_MAX								0x01FF		/* ���[���C�g �ő�l												*/
#define	CAN_YAWRATE_MIN								0xFE00		/* ���[���C�g �ŏ��l												*/
/* CAN�M��:AT�V�t�g */
#define	CAN_ATSHIFT_DS								0x0A		/* AT�V�t�g:D-S														*/
#define	CAN_ATSHIFT_D								0x0B		/* AT�V�t�g:D														*/
#define	CAN_ATSHIFT_N								0x0C		/* AT�V�t�g:N														*/
#define	CAN_ATSHIFT_R								0x0D		/* AT�V�t�g:R														*/
#define	CAN_ATSHIFT_P								0x0E		/* AT�V�t�g:P														*/
#define	CAN_ATSHIFT_1								0x01		/* AT�V�t�g:1�����W													*/
#define	CAN_ATSHIFT_2								0x02		/* AT�V�t�g:21�����W												*/
#define	CAN_ATSHIFT_3								0x03		/* AT�V�t�g:3�����W													*/
#define	CAN_ATSHIFT_ERR								0x0F		/* AT�V�t�g:�G���[													*/
/* 86B�v��` */
/* CAN�M��:AT�V�t�g */
#define	CAN_ATSHIFT_86B_D							0x01		/* AT�V�t�g:D														*/
#define	CAN_ATSHIFT_86B_N							0x02		/* AT�V�t�g:N														*/
#define	CAN_ATSHIFT_86B_R							0x03		/* AT�V�t�g:R														*/
#define	CAN_ATSHIFT_86B_P							0x04		/* AT�V�t�g:P														*/
/* CAN�M��AT�M�A�i�� */
#define	CAN_ATGEAR_86B_NOT_D						0x00		/* AT�M�A�FD�����W�ȊO												*/
#define	CAN_ATGEAR_86B_MANUAL_1ST					0x01		/* AT�M�A�F1ST(MӰ�ށA������MӰ��)									*/
#define	CAN_ATGEAR_86B_MANUAL_2ND					0x02		/* AT�M�A�F2ND(MӰ�ށA������MӰ��)									*/
#define	CAN_ATGEAR_86B_MANUAL_3RD					0x03		/* AT�M�A�F3RD(MӰ�ށA������MӰ��)									*/
#define	CAN_ATGEAR_86B_MANUAL_4TH					0x04		/* AT�M�A�F4TH(MӰ�ށA������MӰ��)									*/
#define	CAN_ATGEAR_86B_MANUAL_5TH					0x05		/* AT�M�A�F5TH(MӰ�ށA������MӰ��)									*/
#define	CAN_ATGEAR_86B_MANUAL_6TH					0x06		/* AT�M�A�F6TH(MӰ�ށA������MӰ��)									*/
#define	CAN_ATGEAR_86B_DSPORT_1ST					0x07		/* AT�M�A�F1ST(��߰�Ӱ��)											*/
#define	CAN_ATGEAR_86B_DSPORT_2ND					0x08		/* AT�M�A�F2ND(��߰�Ӱ��)											*/
#define	CAN_ATGEAR_86B_DSPORT_3RD					0x09		/* AT�M�A�F3RD(��߰�Ӱ��)											*/
#define	CAN_ATGEAR_86B_DSPORT_4TH					0x0A		/* AT�M�A�F4TH(��߰�Ӱ��)											*/
#define	CAN_ATGEAR_86B_DSPORT_5TH					0x0B		/* AT�M�A�F5TH(��߰�Ӱ��)											*/
#define	CAN_ATGEAR_86B_DSPORT_6TH					0x0C		/* AT�M�A�F6TH(��߰�Ӱ��)											*/
#define	CAN_ATGEAR_86B_D							0x0F		/* AT�M�A�FD�����W													*/
/* CAN�M��:TCM�ϑ����䃂�[�h */
#define	CAN_MODESHIFT_D_RANGE_AUTO					0x01		/*	TCM�ϑ����䃂�[�h:1 �c�����W�����ϑ����[�h						*/
#define	CAN_MODESHIFT_GEARFIXED_AUTORETURN			0x03		/*	TCM�ϑ����䃂�[�h:3 �M�A�Œ莩�����A���[�h�i�}�j���A���j		*/
#define	CAN_MODESHIFT_GEARFIXED_MODE				0x05		/*	TCM�ϑ����䃂�[�h:5 �M�A�Œ胂�[�h�i�}�j���A���j				*/
/* CAN�M��AT/MT�M�A�i�� */
#define	CAN_GEARPOS_NOT_DECISION					0x00		/*	�M�A�i��:�s��													*/
#define	CAN_GEARPOS_1ST								0x01		/*	�M�A�i��:1ST													*/
#define	CAN_GEARPOS_2ND								0x02		/*	�M�A�i��:2ND													*/
#define	CAN_GEARPOS_3RD								0x03		/*	�M�A�i��:3RD													*/
#define	CAN_GEARPOS_4TH								0x04		/*	�M�A�i��:4TH													*/
#define	CAN_GEARPOS_5TH								0x05		/*	�M�A�i��:5TH													*/
#define	CAN_GEARPOS_6TH								0x06		/*	�M�A�i��:6TH													*/
#define	CAN_GEARPOS_7TH								0x07		/*	�M�A�i��:7TH													*/
#define	CAN_GEARPOS_8TH								0x08		/*	�M�A�i��:8TH													*/
#define	CAN_GEARPOS_9TH								0x09		/*	�M�A�i��:9TH													*/
#define	CAN_GEARPOS_10TH							0x0A		/*	�M�A�i��:10TH													*/
/* CAN�M���F�^�C����C�� */
#define	CAN_TIREPRESS_STATUS_CHECKING				0x7F		/*	�^�C����C���`�F�b�N�����										*/
#define	CAN_TIREPRESS_RESOLUTION_KPA				5			/*	�^�C����C��kPa����\�i���Z��A����\1�j						*/
#define	CAN_TIREPRESS_RESOLUTION_PSI				1			/*	�^�C����C��psi����\�i���Z��A����\1�j						*/
#define	CAN_TIREPRESS_RESOLUTION_BAR				0.1			/*	�^�C����C��bar����\�i���Z��A����\1�j						*/
/* CAN�M���F�^�C����C���P�� */
#define	CAN_TIREPRESS_UNIT_KPA						0			/*	�^�C����C���P��kPa												*/
#define	CAN_TIREPRESS_UNIT_PSI						1			/*	�^�C����C���P��psi												*/
#define	CAN_TIREPRESS_UNIT_BAR						2			/*	�^�C����C���P��bar												*/
#define	CAN_VEHICLEINFO_UNKNOWN						0			/*	�ԗ���ʁF�s��													*/
#define	CAN_VEHICLEINFO_086A						1			/*	�ԗ���ʁF086A													*/
#define	CAN_VEHICLEINFO_086B						2			/*	�ԗ���ʁF086B													*/
#define	CAN_MANUALMODE_SW_ON						1			/*	�}�j���A�����[�hSW�FON											*/
#define	CAN_MANUALMODE_SW_OFF						0			/*	�}�j���A�����[�hSW�FOFF											*/
/*VSCOFF���*/ /*CAN�̐M����OFF�����v�_��/�������*/
#define CAN_VSC_OFF									1			/* VSCOFF�iVSCOFF�����v�_���j										*/
#define CAN_VSC_ON									0			/* VSCON�iVSCOFF�����v�����j										*/
/*VSC������*/
#define CAN_VSCON_ACTIVE							1			/* VSC�����ԁF���䒆												*/
#define CAN_VSCON_NOTACTIVE							0			/* VSC�����ԁF����O												*/
/*TRCOFF���*/ /*CAN�̐M����OFF�����v�_��/�������*/
#define CAN_TRC_OFF									1			/* TRCOFF�iTRCOFF�����v�_���j										*/
#define CAN_TRC_ON									0			/* TRCON�iTRCOFF�����v�����j										*/
/*TRC������*/
#define CAN_TRCON_ACTIVE							1			/* TRC�����ԁF���䒆												*/
#define CAN_TRCON_NOTACTIVE							0			/* TRC�����ԁF����O												*/
/*ABS������*/
#define CAN_ABSON_ACTIVE							1			/* ABS�����ԁF���䒆												*/
#define CAN_ABSON_NOTACTIVE							0			/* ABS�����ԁF����O												*/
/*ABS�V�X�e���t�F�C�����*/
#define CAN_ABS_SYSFAIL_OFF							1			/* ABS�V�X�e���t�F�C������											*/
#define CAN_ABS_SYSFAIL_ON							0			/* ABS�V�X�e���t�F�C���ُ�											*/
/*ABS�E�H�[�j���O���*/
#define CAN_ABS_WARNING_ON							1			/* ABS�E�H�[�j���OON�F�i�E�H�[�j���O�����v�_���j					*/
#define CAN_ABS_WARNING_OFF							0			/* ABS�E�H�[�j���OOFF�F�i�E�H�[�j���O�����v�����j					*/
/*TRC�E�H�[�j���O���*/
#define CAN_TRC_WARNING_ON							1			/* TRC�E�H�[�j���OON�F�i�E�H�[�j���O�����v�_���j					*/
#define CAN_TRC_WARNING_OFF							0			/* TRC�E�H�[�j���OOFF�F�i�E�H�[�j���O�����v�����j					*/
/*VSC�E�H�[�j���O���*/
#define CAN_VSC_WARNING_ON							1			/* VSC�E�H�[�j���OON�F�i�E�H�[�j���O�����v�_���j					*/
#define CAN_VSC_WARNING_OFF							0			/* VSC�E�H�[�j���OOFF�F�i�E�H�[�j���O�����v�����j					*/
/*�g���N�V�����d�����*/
#define CAN_TRC_MODE_ON								1			/* �g���N�V�����d�����[�h											*/
#define CAN_TRC_MODE_OFF							0			/* �g���N�V�����d�����[�h											*/
/*�G���W���g���N�v��(�A�b�v�_�E������)*/
#define CAN_ENGTRQ_NO_REQUEST						0			/* �G���W���g���N�A�b�v�_�E���v���Ȃ�								*/
#define CAN_ENGTRQ_REQUEST							1			/* �G���W���g���N�A�b�v�_�E���v������								*/
/*�G���W���g���N�i�v���l�Ƌ��ʁj*/
#define CAN_ENGTRQ_OFFSET							-1000		/* �G���W���g���N�I�t�Z�b�g�i�v���l�Ƌ��ʁj							*/
#define CAN_ENGTRQ_RESOLUTION						0.1			/* �G���W���g���N����\�i���Z��A����\1�j�i�v���l�Ƌ��ʁj			*/
#define CAN_ENGTRQ_OFFSET_INC_RESOLUTION			-10000		/* �G���W���g���N�I�t�Z�b�g�F����\���Z�O�i�v���l�Ƌ��ʁj			*/
#define CAN_ENGTRQ_REQ_MAXVAL						0xFFFF		/* �G���W���g���N�v���ő�l											*/
/*VSC�A���_�[�I�[�o�[�X�e�A�쓮�����*/
#define CAN_VSC_CONTROL_STR_ON						1			/* VSC�A���_�[�A�I�[�o�[�X�e�A���䒆								*/
#define CAN_VSC_CONTROL_STR_OFF						0			/* VSC�A���_�[�A�I�[�o�[�X�e�A���䒆�łȂ�							*/
/*EBD�E�H�[�j���O���*/
#define CAN_EBD_WARNING_ON							1			/* EBD�E�H�[�j���OON�F�i�E�H�[�j���O�����v�_���j					*/
#define CAN_EBD_WARNING_OFF							0			/* EBD�E�H�[�j���OOFF�F�i�E�H�[�j���O�����v�����j					*/
/* CAN�M��:�g�����N�J��� */
#define CAN_TRUNK_SW_OPEN							0x01		/* �g�����NSW��ԃI�[�v��											*/
#define CAN_TRUNK_SW_CLOSE							0x00		/* �g�����NSW��ԃN���[�Y											*/
/* CAN�M��:�t�[�h�J��� */
#define CAN_HOOD_SW_OPEN							0x00		/* �t�[�hSW��ԃI�[�v��												*/
#define CAN_HOOD_SW_CLOSE							0x01		/* �t�[�hSW��ԃN���[�Y												*/
/* CAN�M��:�N���[�Y�R���g���[�� */
#define CAN_CRUISE_ON								0x01		/* �N���[�Y�R���g���[��ON											*/
#define CAN_CRUISE_OFF								0x00		/* �N���[�Y�R���g���[��OFF											*/
/* CAN�M��:�z���ǈ��� */
#define CAN_INTAKE_PRESS_RESOLUTION					15			/* �z���ǈ��͕���\�i���Z��A����\�P�j								*/
/* CAN�M��:��C�� */
#define CAN_ATM_PRESS_RESOLUTION					3			/* ��C�� ����\�i���Z��A����\�P�j								*/
#define CAN_ATM_PRESS_OFFSET						100			/* ��C�� �I�t�Z�b�g												*/
/* CAN�M��:�E�B���J�[ */
#define CAN_BLINKER_ON								1			/* �E�B���J�[ON														*/
#define CAN_BLINKER_OFF								0			/* �E�B���J�[OFF													*/
/* CAN�M���F�ڕW�M�A�i�� */
#define	CAN_TARGET_GEAR_N							0x00		/* AT�V�t�g:N														*/
#define	CAN_TARGET_GEAR_D							0x0D		/* AT�V�t�g:D														*/
#define	CAN_TARGET_GEAR_R							0x0E		/* AT�V�t�g:R														*/
#define	CAN_TARGET_GEAR_P							0x0F		/* AT�V�t�g:P														*/
#define	CAN_TARGET_GEAR1ST							0x01		/* AT�M�A�F1ST														*/
#define	CAN_TARGET_GEAR2ND							0x02		/* AT�M�A�F2ND														*/
#define	CAN_TARGET_GEAR3RD							0x03		/* AT�M�A�F3RD														*/
#define	CAN_TARGET_GEAR4TH							0x04		/* AT�M�A�F4TH														*/
#define	CAN_TARGET_GEAR5TH							0x05		/* AT�M�A�F5TH														*/
#define	CAN_TARGET_GEAR6TH							0x06		/* AT�M�A�F6TH														*/

/* �n����R�����˗ʐώZ�l����\ */
#define	CAN_FUEL_CONSUMPTION_RESOLUTION			0.001			/*	�n����R�����˗ʐώZ�l����\�i���Z��A����\1�j					*/
/* �n���㑖�s�����ώZ�l����\ */
#define	CAN_RUNNING_DISTANCE_RESOLUTION			0.01			/*	�n���㑖�s�����ώZ�l����\�i���Z��A����\1�j					*/
/* �\���p�ԑ�����\ */
#define	CAN_VEHICLESPEED_DISP_RESOLUTION		0.00001			/*	�\���p�ԑ�����\												*/
/* CAN�M��:�G���W������ */
#define	CAN_TEMP_COOLANT_MAX					0xFE			/* �G���W������ �ő�l												*/
/* CAN�M��:�G���W�������I�t�Z�b�g */
#define	CAN_ENG_TEMP_COOLANT_OFFSET				-40				/* �G���W�������I�t�Z�b�g											*/
/* CAN�M��:�G���W�������I�t�Z�b�g */
#define	CAN_ENG_TEMP_OIL_OFFSET					-40				/* �G���W�������I�t�Z�b�g											*/
/* CAN�M��:�O�C���ő�l */
#define	CAN_OUOTSIDE_TEMP_MAX					0xF0			/* �O�C���ő�l														*/
/* CAN�M��:�O�C������\ */
#define	CAN_OUOTSIDE_TEMP_RESOLUTION			0.5				/* �O�C������\														*/
/* CAN�M��:�O�C���I�t�Z�b�g */
#define	CAN_OUTSIDE_TEMP_OFFSET					-40				/* �O�C���I�t�Z�b�g													*/
/* ISOCAN�M��:�z�C���x�I�t�Z�b�g */
#define	ISOCAN_INTAKE_MANIFOLD_TEMP_OFFSET		-40				/* �z�C���x�I�t�Z�b�g												*/
/* ISOCAN�M��:�G���W�������I�t�Z�b�g */
#define	ISOCAN_ENG_TEMP_COOLANT_OFFSET			-40				/* �G���W�������I�t�Z�b�g											*/


/* CAN����CH	*/
typedef enum e_can_inspection_can_ch
{
	CAN_INSPECTION_CAN_CH0	= 0,
	CAN_INSPECTION_CAN_CH1
} E_CAN_INSPECTION_CAN_CH;


#define CAN_INSPECTION_RESULT_NG				(0x00)
#define CAN_INSPECTION_RESULT_OK				(0x01)

/* �����R�}���h */
#define CAN_INSPECTION_CMD_START				(0xF1)		/* �����J�n			 					*/
#define CAN_INSPECTION_CMD_STOP					(0xF2)		/* ������~			 					*/
#define CAN_INSPECTION_CMD_SERIAL_NO_WR			(0x10)		/* �V���A��No��������					*/
#define CAN_INSPECTION_CMD_SERIAL_NO_RD			(0x11)		/* �V���A��No�ǂݏo��					*/
#define CAN_INSPECTION_CMD_BT_PAIRING			(0x20)		/* Bluetooth�y�A�����O					*/
#define CAN_INSPECTION_CMD_BT_CONNECT			(0x21)		/* Bluetooth�ʐM�m�F					*/
#define CAN_INSPECTION_CMD_SD					(0x30)		/* SD�J�[�h����							*/
#define CAN_INSPECTION_CMD_CAN0					(0x40)		/* CAN0�m�F								*/
#define CAN_INSPECTION_CMD_CAN1					(0x50)		/* CAN1�m�F								*/
#define CAN_INSPECTION_CMD_UART					(0x60)		/* UART�m�F								*/
#define CAN_INSPECTION_CMD_SW					(0x70)		/* SW�m�F								*/
#define CAN_INSPECTION_CMD_GNSS					(0x80)		/* GNSS����								*/

/* �����R�}���h	*/
typedef struct t_can_inspection_cmd_info {
	UI_8		cmd;			/* �R�}���h(INSPEC_CMD_xxx)				*/
	UI_8		result;			/* ����(CAN_INSPECTION_RESULT_xx)		*/
	UI_8		data1;			/* �f�[�^�P(1byte)						*/
	UI_8		data2;			/* �f�[�^�Q(1byte)						*/
	UI_32		data3;			/* �f�[�^�R(4byte)						*/
} T_Can_inspection_cmd_info;



/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/

/********************************************************/
/*	�O���֐�											*/
/********************************************************/

/********************************************************/
/* �֐�����		: Can_control_Init						*/
/* �@�\�̐���	: CAN���䏉����							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Can_control_Init(void);

/********************************************************/
/* �֐�����		: Can_control_Main						*/
/* �@�\�̐���	: CAN���䃁�C��							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Can_control_Main(void);

/********************************************************/
/* �֐�����		: Can_control_RcvInspectionCmd			*/
/* �@�\�̐���	: �����R�}���h��M						*/
/* ����			: (I/ )cmdinfo	�����R�}���h			*/
/* �߂�l		: N_OK		�R�}���h����M����			*/
/* 				: N_NG		�R�}���h����M�܂��̓G���[	*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Can_control_RcvInspectionCmd(T_Can_inspection_cmd_info *cmdinfo);

/********************************************************/
/* �֐�����		: Can_control_SndInspectionCmd			*/
/* �@�\�̐���	: �����R�}���h���M						*/
/* ����			: (I/ )cmdinfo	�����R�}���h			*/
/* �߂�l		: N_OK		����						*/
/* 				: N_NG		�ُ�						*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Can_control_SndInspectionCmd(const T_Can_inspection_cmd_info *cmdinfo);

/********************************************************/
/* �֐�����		: Can_control_ReqInspection				*/
/* �@�\�̐���	: CAN����								*/
/* ����			: ch	CAN CH							*/
/* �߂�l		: N_OK		����						*/
/* 				: N_NG		�ُ�						*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Can_control_ReqInspection(E_CAN_INSPECTION_CAN_CH ch);

/********************************************************/
/* �֐�����		: Can_control_GetTCanInfo				*/
/* �@�\�̐���	: T-CAN���J�����g�l���擾����			*/
/* ����			: void									*/
/* �߂�l		: T-CAN���J�����g�l�i�[�̈�ptr		*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
T_Can_ctrl_tcan_info *Can_control_GetTCanInfo(void);

/********************************************************/
/* �֐�����		: Can_control_GetVehicleSpeed			*/
/* �@�\�̐���	: �ԑ��J�����g�l���擾����				*/
/* ����			: (O)vehicle_speed �ԑ��J�����g�l		*/
/* �߂�l		: �擾����								*/
/*				   E_RC_OK:�擾����, E_RC_NG:�擾�s��	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/*   �Ԃ����ԑ��́A0.00001km/h��1�P�ʂƂ���l�B		*/
/********************************************************/
E_RC Can_control_GetVehicleSpeed(UI_32 *vehicle_speed);

/********************************************************/
/* �֐�����		: Can_control_GetIsoCanInfo				*/
/* �@�\�̐���	: ISO-CAN���J�����g�l���擾����		*/
/* ����			: void									*/
/* �߂�l		: ISO-CAN���J�����g�l�i�[�̈�ptr		*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/11/08								*/
/* ���l			: 										*/
/********************************************************/
T_Can_ctrl_iso_can_info *Can_control_GetIsoCanInfo(void);
#endif	/*DTO459_VDV_CAN_CTRL_H*/
