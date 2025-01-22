/********************************************************/
/* file TCan_comm_Ctrl.h								*/
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

#ifndef	TCAN_COMM_CTRL_H
#define	TCAN_COMM_CTRL_H

#include "type.h"

/********************************************************/
/*	�O���}�N��											*/
/********************************************************/
/**
 *	������
 */
typedef enum e_tcan_result {
	TCAN_CTRL_OK = 0,		/**< ����l		*/
	TCAN_CTRL_ERROR,		/**< �ُ�l		*/
	TCAN_CTRL_NO_RECEIVE	/**< ����M		*/
} E_TCAN_RESULT;


/* �Ԏ���*/
typedef enum e_tcan_vehicle_info {
	TCAN_VEHICLE_INFO_UNKNOWN = 0x0000,		/**< �s��		*/
	TCAN_VEHICLE_INFO_86A	  = 0x0010,		/**< 086A		*/
	TCAN_VEHICLE_INFO_86B	  = 0x0011,		/**< 086B		*/
}E_TCAN_VEHICLE_INFO;

/********************************************************/
/*	�萔�錾											*/
/********************************************************/
#define HV_MSG_ID_620		0x620		/* �J�[�e�V */
#define HV_MSG_ID_0B4		0x0B4		/* �ԑ� */
#define HV_MSG_ID_3BC		0x3BC		/* �V�t�g�|�W�V���� */
#define HV_MSG_ID_245		0x245		/* �A�N�Z���J�x */
#define HV_MSG_ID_247		0x247		/* EV���[�hMSG */
#define HV_MSG_ID_3B6		0x3B6		/* �����o�b�e���c�� */
#define HV_MSG_ID_1C4		0x1C4		/* �G���W����]�� */
#define HV_MSG_ID_3D3		0x3D3		/* �R������� */

#define MT_MSG_ID_374		0x374		/* �h�ASW */
#define MT_MSG_ID_140		0x140		/* �A�N�Z���J�x */
#define MT_MSG_ID_141		0x141		/* �G���W����]�� */
#define MT_MSG_ID_144		0x144		/* �g�����X�~�b�V������� */
#define MT_MSG_ID_148		0x148		/* �X�|�[�c�V�t�g�ޱ�ʒu�\����� */
#define MT_MSG_ID_360		0x360		/* �G���W����]��(TA�p) */
#define MT_MSG_ID_361		0x361		/* �M�A�|�W�V���� */
#define MT_MSG_ID_368		0x368		/* AT��� */
#define MT_MSG_ID_0D1		0x0D1		/* �쓮�֎����ώԗ֑� */
#define MT_MSG_ID_0D3		0x0D3		/* �ԑ��p���X�M���ώZ�l */

#define MT_MSG_ID_152		0x152		/* �u���[�L/�����v */
#define MT_MSG_ID_282		0x282		/* �o�b�N��SW */
#define MT_MSG_ID_0D0		0x0D0		/* �Ǌp�Z���T�� */
#define MT_MSG_ID_0D4		0x0D4		/* �S�֎ԗ֑� */
#define MT_MSG_ID_375		0x375		/* �h�A���b�N��� */

#define MT_MSG_ID_040		0x040		/* �A�N�Z���J�x */
#define MT_MSG_ID_048		0x048		/* �X�|�[�c�V�t�g�ޱ�ʒu�\����� */
#define MT_MSG_ID_139		0x139		/* �쓮�֎����ώԗ֑� */
#define MT_MSG_ID_13A		0x13A		/* �S�֎ԗ֑� */
#define MT_MSG_ID_13B		0x13B		/* �O������x */
#define MT_MSG_ID_13C		0x13C		/* �ԑ��p���X�M���ώZ�l */
#define MT_MSG_ID_138		0x138		/* �Ǌp�Z���T�� */
#define MT_MSG_ID_146		0x146		/* �G���W����]�� */
#define MT_MSG_ID_241		0x241		/* �M�A�|�W�V����,�R������� */
#define MT_MSG_ID_345		0x345		/* �g�����X�~�b�V������� */
#define MT_MSG_ID_390		0x390		/* �o�b�N��SW */
#define MT_MSG_ID_3AC		0x3AC		/* �u���[�L/�����v */
#define MT_MSG_ID_34A		0x34A		/* �X�m�[�A�X�|�[�c�����v */
#define MT_MSG_ID_652		0x652		/* �h�A���b�N��� */
#define MT_MSG_ID_68C		0x68C		/* �u�����h�t���O */
#define MT_MSG_ID_6E2		0x6E2		/* �^�C����C�� */
#define MT_MSG_ID_328		0x328		/* VDC�M�� */
#define MT_MSG_ID_041		0x041		/* ���G���W�����g���N */
#define MT_MSG_ID_04B		0x04B		/* �ڕW�M�A�i�� */




/********************************************************/
/*	�O���֐�											*/
/********************************************************/
void TCAN_Ctrl_Init(void);
void TCAN_Ctrl_Main(void);
void TCAN_Ctrl_SetMessage(UI_32 id, const UI_8 len, const UI_8* data);
void TCAN_Ctrl_SetupCanMessage(E_TCAN_VEHICLE_INFO info);

E_TCAN_RESULT TCAN_Ctrl_GetMTDoorOpen(UI_8 *data);			/* MT �h�ASW D�ȏ��擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTPDoorOpen(UI_8 *data);			/* MT �h�ASW P�ȏ��擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTrunkDoorOpen(UI_8 *data);		/* MT �h�ASW �g�����N���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTRatioAccel(UI_8 *data);		/* MT �A�N�Z���J�x�����擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTEngRpm(UI_16 *data);			/* MT �G���W����]�����擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTEngRpmTa(UI_16 *data);			/* MT �G���W����]��(�^�R���[�^�p)���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTGearEstm(UI_8 *data);			/* MT �M�A�|�W�V�����\����擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSpWheelAve(UI_16 *data);		/* MT �쓮�֎����ώԗ֑����擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSpPulseInteg(UI_8 *data);		/* MT �ԑ��p���X�M���ώZ�l���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTBreakLamp(UI_8 *data);			/* MT �u���[�L�����v���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTHandBreak(UI_8 *data);			/* MT �p�[�L���O�u���[�L���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTReverse(UI_8 *data);			/* MT ���o�[�X���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTHeadLampHi(UI_8 *data);		/* MT �w�b�h�����vHi���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTHeadLampLo(UI_8 *data);		/* MT �w�b�h�����vLo���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTHeadLampSmall(UI_8 *data);		/* MT �w�b�h�����vSmall���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTOutTemp(UI_8 *data);			/* MT �\���p�O�C���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTPSeatBelt(UI_8 *data);			/* MT �V�[�g�x���g�o�b�N�� P�� ���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTDSeatBelt(UI_8 *data);			/* MT �V�[�g�x���g�o�b�N�� D�� ���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTPlace(UI_8 *data);				/* MT �d���n���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTempOil(UI_8 *data);			/* MT �G���W�������擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTempCoolant(UI_8 *data);		/* MT �G���W�������擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSetCc(UI_8 *data);				/* MT �N���R���Z�b�g�����v��Ԏ擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSteerAngle(UI_16 *data);		/* MT �Ǌp�Z���T���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTYawRate(UI_16 *data);			/* MT ���[���[�g���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTAccLR(UI_8 *data);				/* MT �������x���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTAccFR(UI_8 *data);				/* MT �O��G�Z���T���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTransmissionType(UI_8 *data);	/* MT �g�����X�~�b�V�������擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSportGearPosition(UI_8 *data);	/* MT �X�|�[�c�V�t�g�M�A�ʒu�\�����擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTShiftPosition(UI_8 *data);		/* MT �V�t�g�ʒu���(�t�F�[���Z�[�t������)�擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTModeShiftControl(UI_8 *data);	/* MT TCM�ϑ����䃂�[�h���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTAtType(UI_8 *data);			/* MT AT��ގ擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTIgn(UI_8 *data);  				/* MT IGN���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTAcc(UI_8 *data);  				/* MT ACC���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTBrandFlag(UI_8 *data);  		/* MT �u�����h�t���O���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTTargetGearPos(UI_8 *data);		/* MT �ڕW�M�A�ʒu���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSportsLamp(UI_8 *data);		/* MT �X�|�[�c�����v���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSnowHoldLamp(UI_8 *data);		/* MT �X�m�[�z�[���h���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTNodeID(UI_8 *data);			/* MT �m�[�hID���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSleepAck(UI_8 *data);			/* MT Sleep Ack���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSleepInd(UI_8 *data);			/* MT Sleep Ind���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTLimpHome(UI_8 *data);			/* MT Limp Home���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTRing(UI_8 *data);				/* MT Ring���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTAlive(UI_8 *data);				/* MT Alive���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTWkup(UI_8 *data);				/* MT Wkup���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTInit(UI_8 *data);				/* MT Init���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMTSpecNo(UI_16 *data);			/* MT Spec. No.���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMtSpdWheelFtlh(UI_16 *data);		/* MT ���O�֎ԗ֑����擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMtSpdWheelFtrh(UI_16 *data);		/* MT �E�O�֎ԗ֑����擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMtSpdWheelRrlh(UI_16 *data);		/* MT ����֎ԗ֑����擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMtSpdWheelRrrh(UI_16 *data);		/* MT �E��֎ԗ֑����擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMtDoorLockStatPass(UI_8 *data);	/* MT �h�A���b�N��ԁi����ȁj���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMtDoorLockStatDrvr(UI_8 *data);	/* MT �h�A���b�N��ԁi�^�]�ȁj���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetMtBrakePressure(UI_8 *data);		/* MT �u���[�L�����擾 */


E_TCAN_RESULT TCAN_Ctrl_GetOdo(UI_32 *data);				/* �n���㑖�s�����擾 (HV/MT����) */
E_TCAN_RESULT TCAN_Ctrl_GetFCSum(UI_32 *data);				/* �R�����˗ʐώZ�l (HV/MT����) */
E_TCAN_RESULT TCAN_Ctrl_GetVehicleSpeedForDisp(UI_32 *data);/* �\���p�ԑ����擾 */
E_TCAN_RESULT TCAN_Ctrl_GetVehicleSpeed(UI_32 *data);		/* �ԑ����擾 */
E_TCAN_RESULT TCAN_Ctrl_GetSmoothingBrakePressure(UI_8 *data);/* �u���[�L���Ȃ܂��l�擾 */
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressFtlh(UI_8 *data);		/* ���O�֋�C���擾 		*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressFtrh(UI_8 *data);		/* �E�O�֋�C���擾 		*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressRrlh(UI_8 *data);		/* ����֋�C���擾 		*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressRrrh(UI_8 *data);		/* �E��֋�C���擾 		*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAirPressUnit(UI_8 *data);		/* ��C���P�ʎ擾 			*/
E_TCAN_VEHICLE_INFO TCAN_Ctrl_GetVehicleInfo(void);			/* �Ԏ���擾 			*/
E_TCAN_RESULT TCAN_Ctrl_GetAtManualModeSW(UI_8 *data);		/* �}�j���A�����[�hSW���擾 */
E_TCAN_RESULT TCAN_Ctrl_GetAtVscOnOff(UI_8 *data);			/* VSC ON/OFF���擾 				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscAct(UI_8 *data);			/* VSC/TCS�쓮��ԏ��擾 			*/
E_TCAN_RESULT TCAN_Ctrl_GetAtTrcMode(UI_8 *data);			/* �g���N�V�����d�����[�h���擾 	*/
E_TCAN_RESULT TCAN_Ctrl_GetAtTrcOnOff(UI_8 *data);			/* TRC ON/OFF���擾 				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtTrcAct(UI_8 *data);			/* TRC �쓮��ԏ��擾 			*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAbsAct(UI_8 *data);			/* ABS�쓮��ԏ��擾 				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscWarning(UI_8 *data);		/* VSC�E�H�[�j���O���擾 			*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAbsWarning(UI_8 *data);		/* ABS�E�H�[�j���O���擾 			*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAbsFail(UI_8 *data);			/* ABS�V�X�e���t�F�C�����擾 		*/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscUnderStr(UI_8 *data);		/* VSC�A���_�[������				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtVscOverStr(UI_8 *data);		/* VSC�I�[�o�[������				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqUpReqFlg(UI_8 *data);	/* �G���W���g���N�A�b�v�v���t���O	*/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqDwnReqFlg(UI_8 *data);	/* �G���W���g���N�����v���t���O		*/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqUpReqVal(UI_16 *data);	/* �G���W���g���N�A�b�v�v���l		*/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqDwnReqVal(UI_16 *data);	/* �G���W���g���N�����v���l			*/
E_TCAN_RESULT TCAN_Ctrl_GetAtEngTrqShaft(UI_16 *data);		/* ���G���W�����g���N				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtEcbWarning(UI_8 *data);		/* ECB�E�H�[�j���O���擾			*/
E_TCAN_RESULT TCAN_Ctrl_GetAtIntakePress(UI_8 *data);		/* �z���ǈ��͏��擾				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtAtmosphericPress(UI_8 *data);	/* ��C�����擾					*/
E_TCAN_RESULT TCAN_Ctrl_GetAtBlinkerLeft(UI_8 *data);		/* �E�B���J�[�����擾				*/
E_TCAN_RESULT TCAN_Ctrl_GetAtBlinkerRight(UI_8 *data);		/* �E�B���J�[�E���擾				*/
E_TCAN_RESULT TCAN_Ctrl_GetMtHoodDoorSW(UI_8 *data);		/* �h�ASW �t�[�h���擾			*/

#endif	/* TCAN_COMM_CTRL_H */
