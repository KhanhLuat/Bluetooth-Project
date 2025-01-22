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
/* file DTO459_VDV_Can_ctrl.c							*/
/* CAN����												*/
/********************************************************/

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/

#include <type.h>
#include <system.h>
#include <string.h>
#include "DTO459_VDV_Common.h"
#include "timer_ctrl.h"
#include "DTO459_VDV_Inspection_ctrl.h"
#include "dbglog.h"
#include "DTO459_VDV_Can_ctrl.h"
#include "System_control_TASK.h"
#include "TCan_comm_Ctrl.h"
#include "DTO459_VDV_Can_Drv.h"
#include "DTO459_VDV_local_can_ctrl.h"
#ifdef ISOCAN_ENABLE
#include "DTO459_VDV_Iso_can_ctrl.h"
#endif /* ISOCAN_ENABLE */
#include "SSFTSTD_FR81_Port_drv.h"
#include "DTO459_VDV_Eeprom_ctrl.h"

/********************************************************/
/*	�}�N����`											*/
/********************************************************/
#define CAN_CTRL_LOCAL	static



#define CAN_CH_TCAN		CAN_CH_0		/* CAN�`�����l�� : T-CAN */

#define CAN_TCAN_DATA_RCV_TIMEOUT			(10000)		/* T-CAN�f�[�^��M�^�C���A�E�g(10000ms)	*/

#define CAN_INSPECTION_SEND_BUFFER_COUNT	(5)			/* �����pCAN�̑��M�o�b�t�@��			*/
#define CAN_INSPECTION_MSG_ID_RX_MIN		(CAN_INSPECTION_SEND_BUFFER_COUNT + 1)
														/* �����pCAN�̎�M�p���b�Z�[�WID�ŏ��l	*/

#define CAN_INSPECTION_RCV_CMD_CAN_ID		(0x100)		/* �����R�}���h��M�pCAN ID				*/
#define CAN_INSPECTION_SND_CMD_CAN_ID		(0x101)		/* �����R�}���h���M�pCAN ID				*/

#define CAN_INSPECTION_TIMEOUT				(3000)		/* CAN�����^�C���A�E�g(3000ms)			*/
#define CAN_INSPECTION_MSG_NUM				(3)			/* CAN�������b�Z�[�W��					*/


/* ������� */
typedef enum {
	CAN_INSPECTION_STAT_IDLE = 0,			/* �A�C�h��								*/
	CAN_INSPECTION_STAT_START,				/* �����J�n								*/
	CAN_INSPECTION_STAT_INSPECT				/* ������								*/
} E_CAN_INSPECTION_STAT;

/* CAN������ */
typedef enum {
	CAN_STAT_IDLE = 0,					/* �A�C�h��								*/
	CAN_STAT_PRE_WORKING,				/* ���쏀����							*/
	CAN_STAT_INSPECTION,				/* �������쒆							*/
	CAN_STAT_WORKING,					/* �ʏ퓮�쒆							*/
	CAN_STAT_ERROR						/* �G���[���							*/
} E_CAN_STAT;

/* CAN�^�C�v */
typedef enum {
	CAN_TYPE_TCAN = 0,					/* T-CAN								*/
	CAN_TYPE_ISO						/* ISO CAN								*/
} E_CAN_TYPE;

/********************************************************/
/*	�\���̒�`											*/
/********************************************************/
typedef struct t_can_inspection_info {
	UI_8						reqflg;
	UI_8						msg_cnt;
	UI_32						start_time;
	E_CAN_INSPECTION_CAN_CH		ch;
} T_CAN_INSPECTION_INFO;

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
CAN_CTRL_LOCAL E_CAN_STAT           S_can_stat  = CAN_STAT_IDLE;	/* CAN������					*/	
CAN_CTRL_LOCAL T_Can_ctrl_tcan_info S_Can_ctrl_tcan_info;			/* �O���񋟗pT-CAN�ԗ����		*/
CAN_CTRL_LOCAL T_Can_ctrl_iso_can_info S_Can_ctrl_iso_can_info;		/* �O���񋟗pISO CAN�ԗ����	*/
#ifdef ISOCAN_ENABLE
CAN_CTRL_LOCAL E_CAN_TYPE			S_can_type;
#endif /* ISOCAN_ENABLE */
CAN_CTRL_LOCAL T_CAN_INSPECTION_INFO	S_can_inspection_info;		/* CAN�������				*/

/********************************************************/
/*	�����֐�											*/
/********************************************************/
CAN_CTRL_LOCAL void proc_for_idle(void);
CAN_CTRL_LOCAL void proc_for_pre_working(void);
CAN_CTRL_LOCAL void proc_for_inspection(void);
CAN_CTRL_LOCAL void proc_for_working(void);
CAN_CTRL_LOCAL void change_stat(E_CAN_STAT can_stat);
CAN_CTRL_LOCAL void readEepData(void);
CAN_CTRL_LOCAL void writeEepData(void);

CAN_CTRL_LOCAL void update_tcan_info_all(void);
CAN_CTRL_LOCAL void update_tcan_info_086A(void);
CAN_CTRL_LOCAL void update_tcan_info_id_152(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_374(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_282(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_140(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_141(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_144(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_360(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_361(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_148(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_368(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_0d0(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_0d1(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_0d3(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_0d4(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_44d(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_375(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_calc(T_Can_ctrl_tcan_info *tcan_info);

CAN_CTRL_LOCAL void update_tcan_info_086B(void);
CAN_CTRL_LOCAL void update_tcan_info_id_040(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_048(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_139(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_13A(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_13B(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_13C(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_138(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_146(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_241(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_345(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_390(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_3AC(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_34A(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_652(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_68C(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_6E2(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_328(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_041(T_Can_ctrl_tcan_info *tcan_info);
CAN_CTRL_LOCAL void update_tcan_info_id_04B(T_Can_ctrl_tcan_info *tcan_info);

#ifdef ISOCAN_ENABLE
CAN_CTRL_LOCAL void update_iso_can_info_all(void);
#endif /* ISOCAN_ENABLE */

CAN_CTRL_LOCAL B_8 receive_tcan_data(void);

CAN_CTRL_LOCAL void can_init_inspection_info(void);
CAN_CTRL_LOCAL void can_inspect_can0(void);
CAN_CTRL_LOCAL void can_inspect_can1(void);
CAN_CTRL_LOCAL void can_inspect_stop(void);

/********************************************************/
/* �֐�����		: Can_control_Init						*/
/* �@�\�̐���	: CAN���䏉����							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Can_control_Init(void)
{
	/* T-CAN�ԗ���񏉊��� */
	(void)memset(&S_Can_ctrl_tcan_info, 0x00, sizeof(S_Can_ctrl_tcan_info));
	
	/* ISO-CAN�ԗ���񏉊��� */
	(void)memset(&S_Can_ctrl_iso_can_info, 0x00, sizeof(S_Can_ctrl_iso_can_info));


	/* CAN������񏉊���	*/
	can_init_inspection_info();
}

/********************************************************/
/* �֐�����		: Can_control_Main						*/
/* �@�\�̐���	: CAN���䃁�C��							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Can_control_Main(void)
{
	switch(S_can_stat) {
	case CAN_STAT_IDLE:
		/* �A�C�h����Ԃł̏��� */
		proc_for_idle();
		break;
	case CAN_STAT_PRE_WORKING:
		/* ���쏀�����̏��� */
		proc_for_pre_working();
		break;
	case CAN_STAT_INSPECTION:
		/* �������쒆�̏��� */
		proc_for_inspection();
		break;
	case CAN_STAT_WORKING:
		/* �ʏ퓮�쒆�̏��� */
		proc_for_working();
		break;
	default:
		break;
	}
}


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
SI_8 Can_control_RcvInspectionCmd(T_Can_inspection_cmd_info *cmdinfo)
{
	T_Can_data	candata;
	E_RC		ret;

	if (S_can_stat != CAN_STAT_INSPECTION) {
		/* CAN��Ԃ��u��������v�ȊO	*/
		return N_NG;
	}

	if (cmdinfo == NULL) {
		return N_NG;
	}

	ret = Can_drv_get_rx_data(CAN_CH_1, &candata);
	if (ret != E_RC_OK) {
		/* ���b�Z�[�W����M	*/
		return N_NG;
	}
	if (candata.id != CAN_INSPECTION_RCV_CMD_CAN_ID) {
		return N_NG;
	}

	cmdinfo->cmd	= candata.data[0];
	cmdinfo->result	= candata.data[1];
	cmdinfo->data1	= candata.data[2];
	cmdinfo->data2	= candata.data[3];
	cmdinfo->data3	= Common_Get32(&candata.data[4]);

	return N_OK;
}

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
SI_8 Can_control_SndInspectionCmd(const T_Can_inspection_cmd_info *cmdinfo)
{
	UI_8  candata[8];
	E_RC  ret;

	if (S_can_stat != CAN_STAT_INSPECTION) {
		/* CAN��Ԃ��u��������v�ȊO	*/
		return N_NG;
	}

	if (cmdinfo == NULL) {
		return N_NG;
	}

	candata[0] = cmdinfo->cmd;	
	candata[1] = cmdinfo->result;
	candata[2] = cmdinfo->data1;
	candata[3] = cmdinfo->data2;
	Common_Set32( &candata[4], cmdinfo->data3 );

	ret = Can_drv_set_tx_data(CAN_CH_1, 1, CAN_INSPECTION_SND_CMD_CAN_ID, candata, 8);
	if (ret != E_RC_OK) {
		return N_NG;
	}

	return N_OK;
}

/****************************************************************************/
/* �֐�����		: Can_control_ReqInspection									*/
/* �@�\�̐���	: CAN�����v��												*/
/* ����			: ch	CAN CH												*/
/* �߂�l		: N_OK		����											*/
/* 				: N_NG		�ُ�											*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 17/01/20													*/
/* ���l			: �{�֐�������I�������ꍇ�A�������䃂�W���[���ɑ΂���		*/
/*				   Insp_notify_inspection_result�ɂ�茟�����ʂ�ʒm����	*/
/****************************************************************************/
SI_8 Can_control_ReqInspection(E_CAN_INSPECTION_CAN_CH ch)
{
	if ((ch != CAN_INSPECTION_CAN_CH0) &&
		(ch != CAN_INSPECTION_CAN_CH1)) {				/* CAN CH�s��	*/
		return N_NG;
	}

	if (S_can_stat != CAN_STAT_INSPECTION) {		/* CAN��Ԃ��u��������v�ȊO	*/
		return N_NG;
	}

	if (S_can_inspection_info.reqflg == N_ON) {		/* CAN������	*/
		return N_NG;
	}

	/* �������Z�b�g	*/
	can_init_inspection_info();
	S_can_inspection_info.ch = ch;
	S_can_inspection_info.start_time = Timer_ctrl_GetTime1ms();
	S_can_inspection_info.reqflg = N_ON;

	return N_OK;
}

/********************************************************/
/* �֐�����		: proc_for_idle							*/
/* �@�\�̐���	: �A�C�h����Ԏ��������				*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void proc_for_idle(void)
{
	E_SYS_CTRL_STATUS sys_ctrl_status;
	
	sys_ctrl_status = System_control_GetStatus();

	switch(sys_ctrl_status) {
	case SYS_OPERATION_WAIT_ACC_ON:
		/* ACC ON�҂���ԂȂ牽�����Ȃ� */
		break;
	case SYS_OPERATION_ACC_ON_UNKNOWN_STAT:
		/* ACC ON�i��Ԗ��m��j�Ȃ瓮�쏀�����֑J�� */
		change_stat(CAN_STAT_PRE_WORKING);
		break;
	default:
		break;
	}
}

/********************************************************/
/* �֐�����		: change_stat							*/
/* �@�\�̐���	: ��ԑJ�ڏ���							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/*   ��Ԓl�̍X�V�Ƌ��ɏ����t�F�[�Y�J�E���^�̃��Z�b�g��	*/
/*	 �s���B												*/
/********************************************************/
CAN_CTRL_LOCAL void change_stat(E_CAN_STAT can_stat)
{
#ifdef ISOCAN_ENABLE
	UI_8	port_data;
#endif /* ISOCAN_ENABLE */
	UI_32	msg_id;
	UI_8	i;

	S_can_stat  = can_stat;

	switch(S_can_stat) {
	case CAN_STAT_PRE_WORKING:
		(void)System_control_NotifyEvent(SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_OK_CAN);
		break;

	case CAN_STAT_INSPECTION:
		/* CAN������񏉊���	*/
		can_init_inspection_info();

		/* CAN�h���C�o CH0 ������*/
		Can_drv_ch_init(CAN_CH_0, CAN_INSPECTION_SEND_BUFFER_COUNT);
		for (i = 0; i < 6; i++) {
			if (i < 3) {
				msg_id = 0x7FF;			/* 11bit���b�Z�[�WID */
			}
			else {
				msg_id = 0x1FFFFFFFUL;	/* 29bit���b�Z�[�WID */
			}
			Can_drv_change_message_buffer_mask(CAN_CH_0, (CAN_INSPECTION_MSG_ID_RX_MIN + i), msg_id, 0);
		}

		/* CAN�h���C�o CH1 ������*/
		Can_drv_ch_init(CAN_CH_1, CAN_INSPECTION_SEND_BUFFER_COUNT);
		Can_drv_change_message_buffer(CAN_CH_1, CAN_INSPECTION_MSG_ID_RX_MIN, CAN_INSPECTION_RCV_CMD_CAN_ID);

		break;

	case CAN_STAT_WORKING:
#ifdef ISOCAN_ENABLE
		PortDrv_GetDataBit(IO_PI_GR_CAN_SW, IO_PI_BIT_CAN_SW, &port_data);
		if (port_data == IO_PI_LEV_CAN_SW_ISOCAN) {
			/* ISO CAN���L�� */
			S_can_type = CAN_TYPE_ISO;

			/* ISO CAN�R���g���[��������*/
			Iso_can_ctrl_Init();
		}
		else {
			/* T-CAN���L�� */
			S_can_type = CAN_TYPE_TCAN;

			/* T-CAN�pCAN�`�����l�������� */
			Can_drv_ch_init(CAN_CH_TCAN, 0);

			/* T-CAN�R���g���[�������� */
			TCAN_Ctrl_Init();
		}
#else /* ISOCAN_ENABLE  */
		/* T-CAN�pCAN�`�����l�������� */
		Can_drv_ch_init(CAN_CH_TCAN, 0);

		/* T-CAN�R���g���[�������� */
		TCAN_Ctrl_Init();
#endif /* ISOCAN_ENABLE  */

		/* Local CAN�R���g���[�������� */
		Local_can_ctrl_Init();
		/*�s��������������ԗ���񖢓ǂݍ��݃`�F�b�N*/
		if(S_Can_ctrl_tcan_info.en_eep_vehicle_info == 0)
		{
			/*���ǂݍ��݂Ȃ�s��������������f�[�^�ǂݍ���*/
			readEepData();
			/*�Ԏ���ɉ�����CAN���b�Z�[�W�̃Z�b�g�A�b�v*/
			TCAN_Ctrl_SetupCanMessage((E_TCAN_VEHICLE_INFO)S_Can_ctrl_tcan_info.eep_vehicle_info);
		}
		break;

	case CAN_STAT_ERROR:
		(void)System_control_NotifyEvent(SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_ATT_NG_CAN);
		break;

	default:
		break;
	}
}

/********************************************************/
/* �֐�����		: proc_for_pre_working					*/
/* �@�\�̐���	: ���쏀������Ԏ��������				*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void proc_for_pre_working(void)
{
	E_SYS_CTRL_STATUS sys_ctrl_status;
	
	sys_ctrl_status = System_control_GetStatus();

	switch(sys_ctrl_status) {
	case SYS_OPERATION_PRE_ACC_OFF:
		change_stat(CAN_STAT_IDLE);
		break;

	case SYS_OPERATION_NORMAL_OFF:
	case SYS_OPERATION_NORMAL_AUTO:
	case SYS_OPERATION_NORMAL_MANUAL:
	case SYS_OPERATION_SIMULATION:
		change_stat(CAN_STAT_WORKING);
		break;

	case SYS_OPERATION_INSPECTION:
		change_stat(CAN_STAT_INSPECTION);
		break;

	default:
		break;
	}
}

/********************************************************/
/* �֐�����		: proc_for_inspection					*/
/* �@�\�̐���	: �������쒆����						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void proc_for_inspection(void)
{
	E_SYS_CTRL_STATUS sys_ctrl_status;

	sys_ctrl_status = System_control_GetStatus();
	switch(sys_ctrl_status) {
	case SYS_OPERATION_INSPECTION:
		if (S_can_inspection_info.reqflg == N_ON) {
			if (S_can_inspection_info.ch == CAN_INSPECTION_CAN_CH0) {
				can_inspect_can0();
			}
			else {
				can_inspect_can1();
			}
		}
		break;

	case SYS_OPERATION_INSPECTION_COMP:
	case SYS_OPERATION_PRE_ACC_OFF:
		if (S_can_inspection_info.reqflg == N_ON) {
			can_inspect_stop();
		}
		change_stat(CAN_STAT_IDLE);
		break;
	
	default:
		break;
	}

}

/********************************************************/
/* �֐�����		: proc_for_working						*/
/* �@�\�̐���	: CAN����ʏ퓮�쒆����					*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)katsura							*/
/* �쐬��		: 16/09/28								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void proc_for_working(void)
{
	E_SYS_CTRL_STATUS sys_ctrl_status;

	sys_ctrl_status = System_control_GetStatus();
	if (sys_ctrl_status == SYS_OPERATION_PRE_ACC_OFF) {
		/* ACC OFF�ڍs�Ȃ�A�C�h���֑J�� */
		change_stat(CAN_STAT_IDLE);
	}

#ifdef ISOCAN_ENABLE
	if (S_can_type == CAN_TYPE_ISO) {
		/* ISO CAN�R���g���[�����C�������R�[�� */
		Iso_can_ctrl_Main();

		/* ISO-CAN�J�����g���̍X�V */
		update_iso_can_info_all();
	}
	else {
#endif /* ISOCAN_ENABLE */

		/* T-CAN�f�[�^��M */
		(void)receive_tcan_data();

		/* T-CAN�R���g���[�����C�������R�[�� */
		TCAN_Ctrl_Main();

		/* T-CAN�J�����g���̍X�V */
		update_tcan_info_all();
#ifdef ISOCAN_ENABLE
	}
#endif /* ISOCAN_ENABLE */

	/* Local CAN�R���g���[�����C�������R�[�� */
	Local_can_ctrl_Main();

}

/********************************************************/
/* �֐�����		: Can_control_GetTCanInfo				*/
/* �@�\�̐���	: T-CAN���J�����g�l���擾����			*/
/* ����			: void									*/
/* �߂�l		: T-CAN���J�����g�l�i�[�̈�ptr		*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
T_Can_ctrl_tcan_info *Can_control_GetTCanInfo(void)
{
	return(&S_Can_ctrl_tcan_info);
}

/********************************************************/
/* �֐�����		: Can_control_GetIsoCanInfo				*/
/* �@�\�̐���	: ISO-CAN���J�����g�l���擾����		*/
/* ����			: void									*/
/* �߂�l		: ISO-CAN���J�����g�l�i�[�̈�ptr		*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/11/08								*/
/* ���l			: 										*/
/********************************************************/
T_Can_ctrl_iso_can_info *Can_control_GetIsoCanInfo(void)
{
	return(&S_Can_ctrl_iso_can_info);
}

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
E_RC Can_control_GetVehicleSpeed(UI_32 *vehicle_speed)
{
	E_TCAN_RESULT result;
	
	result = TCAN_Ctrl_GetVehicleSpeed(vehicle_speed);
	if(result != TCAN_CTRL_OK) {
		return E_RC_NG;
	}
	else {
		return E_RC_OK;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_all					*/
/* �@�\�̐���	: T-CAN�J�����g�����X�V����			*/
/* ����			: �Ȃ�									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_all(void)
{
	E_TCAN_VEHICLE_INFO vehicleInfo; 
	/*�Ԏ���擾*/
	vehicleInfo = TCAN_Ctrl_GetVehicleInfo();

	if ( (S_Can_ctrl_tcan_info.en_eep_vehicle_info == 1) &&
		 (S_Can_ctrl_tcan_info.eep_vehicle_info != (UI_16)vehicleInfo) ){
		/*EEPROM�ێ��l�ƕω�����*/
#ifdef CAN_DEBUG
		OUTLOG(OutputLog( "[CAN] @@@@@@@@@ VehileInfo Change (%04x) @@@@@@@@ \n", vehicleInfo );)
#endif /*CAN_DEBUG*/
		/* T-CAN�pCAN�`�����l�������� */
		Can_drv_ch_init(CAN_CH_TCAN, 0);
		/* T-CAN�R���g���[�������� */
		TCAN_Ctrl_Init();
		/*�Ԏ���ɉ�����CAN���b�Z�[�W�̃Z�b�g�A�b�v*/
		TCAN_Ctrl_SetupCanMessage(vehicleInfo);
		/*EEPROM�ێ��l�X�V*/
		S_Can_ctrl_tcan_info.eep_vehicle_info = (UI_16)vehicleInfo;
		/*�s�����������փf�[�^��������*/
		writeEepData();
	}

	/*�ԗ���ʍX�V*/
	S_Can_ctrl_tcan_info.en_vehicle_info = 1;
	/*�ԗ���ʃ`�F�b�N*/
	switch (vehicleInfo) {
	case TCAN_VEHICLE_INFO_86A:
		/*086A*/
		S_Can_ctrl_tcan_info.vehicle_info = CAN_VEHICLEINFO_086A;
		update_tcan_info_086A();
		break;
	case TCAN_VEHICLE_INFO_86B:
		/*086B*/
		S_Can_ctrl_tcan_info.vehicle_info = CAN_VEHICLEINFO_086B;
		update_tcan_info_086B();
		break;
	default:
		/*�s��*/
		S_Can_ctrl_tcan_info.vehicle_info = CAN_VEHICLEINFO_UNKNOWN;
		break;
	}

}

/********************************************************/
/* �֐�����		: update_tcan_info_086A					*/
/* �@�\�̐���	: T-CAN�J�����g�����X�V����			*/
/* ����			: �Ȃ�									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_086A(void)
{
	/* ID=152���X�V */
	update_tcan_info_id_152(&S_Can_ctrl_tcan_info);

	/* ID=374���X�V */
	update_tcan_info_id_374(&S_Can_ctrl_tcan_info);

	/* ID=282���X�V */
	update_tcan_info_id_282(&S_Can_ctrl_tcan_info);

	/* ID=140���X�V */
	update_tcan_info_id_140(&S_Can_ctrl_tcan_info);

	/* ID=141���X�V */
	update_tcan_info_id_141(&S_Can_ctrl_tcan_info);

	/* ID=144���X�V */
	update_tcan_info_id_144(&S_Can_ctrl_tcan_info);

	/* ID=360���X�V */
	update_tcan_info_id_360(&S_Can_ctrl_tcan_info);

	/* ID=361���X�V */
	update_tcan_info_id_361(&S_Can_ctrl_tcan_info);

	/* ID=148���X�V */
	update_tcan_info_id_148(&S_Can_ctrl_tcan_info);

	/* ID=368���X�V */
	update_tcan_info_id_368(&S_Can_ctrl_tcan_info);

	/* ID=0D0���X�V */
	update_tcan_info_id_0d0(&S_Can_ctrl_tcan_info);
	
	/* ID=0D1���X�V */
	update_tcan_info_id_0d1(&S_Can_ctrl_tcan_info);

	/* ID=0D3���X�V */
	update_tcan_info_id_0d3(&S_Can_ctrl_tcan_info);

	/* ID=0D4���X�V */
	update_tcan_info_id_0d4(&S_Can_ctrl_tcan_info);

	/* ID=44D���X�V */
	update_tcan_info_id_44d(&S_Can_ctrl_tcan_info);

	/* ID=375���X�V */
	update_tcan_info_id_375(&S_Can_ctrl_tcan_info);

	/* �����Z�o���X�V */
	update_tcan_info_calc(&S_Can_ctrl_tcan_info);
}


/********************************************************/
/* �֐�����		: update_tcan_info_086B					*/
/* �@�\�̐���	: T-CAN�J�����g�����X�V����			*/
/* ����			: �Ȃ�									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_086B(void)
{

	/* ID=040���X�V */
	update_tcan_info_id_040(&S_Can_ctrl_tcan_info);

	/* ID=048���X�V */
	update_tcan_info_id_048(&S_Can_ctrl_tcan_info);

	/* ID=139���X�V */
	update_tcan_info_id_139(&S_Can_ctrl_tcan_info);

	/* ID=13A���X�V */
	update_tcan_info_id_13A(&S_Can_ctrl_tcan_info);

	/* ID=13B���X�V */
	update_tcan_info_id_13B(&S_Can_ctrl_tcan_info);

	/* ID=13C���X�V */
	update_tcan_info_id_13C(&S_Can_ctrl_tcan_info);

	/* ID=138���X�V */
	update_tcan_info_id_138(&S_Can_ctrl_tcan_info);

	/* ID=146���X�V */
	update_tcan_info_id_146(&S_Can_ctrl_tcan_info);

	/* ID=241���X�V */
	update_tcan_info_id_241(&S_Can_ctrl_tcan_info);

	/* ID=345���X�V */
	update_tcan_info_id_345(&S_Can_ctrl_tcan_info);

	/* ID=390���X�V */
	update_tcan_info_id_390(&S_Can_ctrl_tcan_info);

	/* ID=3AC���X�V */
	update_tcan_info_id_3AC(&S_Can_ctrl_tcan_info);

	/* ID=34A���X�V */
	update_tcan_info_id_34A(&S_Can_ctrl_tcan_info);

	/* ID=652���X�V */
	update_tcan_info_id_652(&S_Can_ctrl_tcan_info);

	/* ID=68C���X�V */
	update_tcan_info_id_68C(&S_Can_ctrl_tcan_info);

	/* ID=6E2���X�V */
	update_tcan_info_id_6E2(&S_Can_ctrl_tcan_info);

	/* ID=328���X�V */
	update_tcan_info_id_328(&S_Can_ctrl_tcan_info);

	/* ID=041���X�V */
	update_tcan_info_id_041(&S_Can_ctrl_tcan_info);

	/* ID=04B���X�V */
	update_tcan_info_id_04B(&S_Can_ctrl_tcan_info);

	/* �����Z�o���X�V */
	update_tcan_info_calc(&S_Can_ctrl_tcan_info);

}


/********************************************************/
/* �֐�����		: update_tcan_info_id_040				*/
/* �@�\�̐���	: T-CAN ID=040�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_040(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* �A�N�Z���J�x���擾 */
	result = TCAN_Ctrl_GetMTRatioAccel(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->accel_ratio    = data_8;
		tcan_info->en_accel_ratio = 1;
	}
	else {
		tcan_info->en_accel_ratio = 0;
	}

	/* �G���W����]�����擾 */
	result = TCAN_Ctrl_GetMTEngRpm(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->eng_rpm    = data_16;
		tcan_info->en_eng_rpm = 1;
	}
	else {
		tcan_info->en_eng_rpm = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_048				*/
/* �@�\�̐���	: T-CAN ID=048�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_048(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* �X�|�[�c�V�t�g�M�A�ʒu�\�����擾 */
	result = TCAN_Ctrl_GetMTSportGearPosition(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->sports_gear_pos    = (data_8 & 0x0f);
		tcan_info->en_sports_gear_pos = 1;
	}
	else {
		tcan_info->en_sports_gear_pos = 0;
	}

	/* �V�t�g�ʒu���(�t�F�[���Z�[�t������)���擾 */
	result = TCAN_Ctrl_GetMTShiftPosition(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->shift_pos    = (data_8 & 0x07);
		tcan_info->en_shift_pos = 1;
	}
	else {
		tcan_info->en_shift_pos = 0;
	}

	/* �}�j���A�����[�hSW���擾 */
	result = TCAN_Ctrl_GetAtManualModeSW(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->manual_mode_sw    = (data_8 & 0x01);
		tcan_info->en_manual_mode_sw = 1;
	}
	else {
		tcan_info->en_manual_mode_sw = 0;
	}

}




/********************************************************/
/* �֐�����		: update_tcan_info_id_146				*/
/* �@�\�̐���	: T-CAN ID=146�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_146(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* �G���W����]���^�R���[�^�p���擾 */
	result = TCAN_Ctrl_GetMTEngRpmTa(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->eng_rpm_taco    = data_16;
		tcan_info->en_eng_rpm_taco = 1;
	}
	else {
		tcan_info->en_eng_rpm_taco = 0;
	}
}


/********************************************************/
/* �֐�����		: update_tcan_info_id_68C				*/
/* �@�\�̐���	: T-CAN ID=68C�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_68C(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* �u�����h�t���O���擾 */
	result = TCAN_Ctrl_GetMTBrandFlag(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->brand_flag    = (data & 0x01);
		tcan_info->en_brand_flag = 1;
	}
	else {
		tcan_info->en_brand_flag = 0;
	}

}

/********************************************************/
/* �֐�����		: update_tcan_info_id_241				*/
/* �@�\�̐���	: T-CAN ID=241�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_241(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* MT�M�A�|�W�V�����\�����擾 */
	result = TCAN_Ctrl_GetMTGearEstm(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->mt_gear_pos    = (data_8 & 0x07);
		tcan_info->en_mt_gear_pos = 1;
	}
	else {
		tcan_info->en_mt_gear_pos = 0;
	}

	/* �N���R���Z�b�g�����v���擾 */
	result = TCAN_Ctrl_GetMTSetCc(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->cruise_controlrl_set    = (data_8 & 0x01);
		tcan_info->en_cruise_controlrl_set = 1;
	}
	else {
		tcan_info->en_cruise_controlrl_set = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_34A				*/
/* �@�\�̐���	: T-CAN ID=34A�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_34A(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;


	/* �X�|�[�c�����v���擾 */
	result = TCAN_Ctrl_GetMTSportsLamp(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->sports_lamp    = (data_8 & 0x01);
		tcan_info->en_sports_lamp = 1;
	}
	else {
		tcan_info->en_sports_lamp = 0;
	}

	/* �X�m�[�z�[���h�����v���擾 */
	result = TCAN_Ctrl_GetMTSnowHoldLamp(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->snow_hold_lamp    = (data_8 & 0x01);
		tcan_info->en_snow_hold_lamp = 1;
	}
	else {
		tcan_info->en_snow_hold_lamp = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_139				*/
/* �@�\�̐���	: T-CAN ID=139�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_139(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16         data_16;
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* �쓮�֕��ώԗ֑����擾 */
	result = TCAN_Ctrl_GetMTSpWheelAve(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->wheel_speed_ave    = data_16;
		tcan_info->en_wheel_speed_ave = 1;
	}
	else {
		tcan_info->en_wheel_speed_ave = 0;
	}

	/* �u���[�L�����擾 */
	result = TCAN_Ctrl_GetMtBrakePressure(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->brake_pressure    = data_8;
		tcan_info->en_brake_pressure = 1;
	}
	else {
		tcan_info->en_brake_pressure = 0;
	}

	/* TRC�쓮��ԏ��擾 */
	result = TCAN_Ctrl_GetAtTrcAct(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->trcAct    = (data_8 & 0x01);
		tcan_info->en_trcAct = 1;
	}
	else {
		tcan_info->en_trcAct = 0;
	}
	/* ABS�쓮��ԏ��擾 				*/
	result = TCAN_Ctrl_GetAtAbsAct(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->absAct    = (data_8 & 0x01);
		tcan_info->en_absAct = 1;
	}
	else {
		tcan_info->en_absAct = 0;
	}
	
	/* ABS�V�X�e���t�F�C�����擾 		*/
	result = TCAN_Ctrl_GetAtAbsFail(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->absFail    = (data_8 & 0x01);
		tcan_info->en_absFail = 1;
	}
	else {
		tcan_info->en_absFail = 0;
	}
	
	/* VSC�A���_�[�X�e�A��� */
	result = TCAN_Ctrl_GetAtVscUnderStr(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->vscUnderStr    = (data_8 & 0x01);
		tcan_info->en_vscUnderStr = 1;
	}
	else {
		tcan_info->en_vscUnderStr = 0;
	}

	/* VSC�I�[�o�[�X�e�A��� */
	result = TCAN_Ctrl_GetAtVscOverStr(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->vscOverStr    = (data_8 & 0x01);
		tcan_info->en_vscOverStr = 1;
	}
	else {
		tcan_info->en_vscOverStr = 0;
	}

}

/********************************************************/
/* �֐�����		: update_tcan_info_id_13C				*/
/* �@�\�̐���	: T-CAN ID=13C�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_13C(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* �ԑ��p���X�ώZ�l���擾 */
	result = TCAN_Ctrl_GetMTSpPulseInteg(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_pulse_integ    = (data_8 & 0x3f);
		tcan_info->en_speed_pulse_integ = 1;
	}
	else {
		tcan_info->en_speed_pulse_integ = 0;
	}
}


/********************************************************/
/* �֐�����		: update_tcan_info_id_3AC				*/
/* �@�\�̐���	: T-CAN ID=3AC�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_3AC(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* IGN���擾 */
	result = TCAN_Ctrl_GetMTIgn(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->ign    = (data & 0x01);
		tcan_info->en_ign = 1;
	}
	else {
		tcan_info->en_ign = 0;
	}
	
	/* ACC���擾 */
	result= TCAN_Ctrl_GetMTAcc(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->acc    = (data & 0x01);
		tcan_info->en_acc = 1;
	}
	else {
		tcan_info->en_acc = 0;
	}
	
	/* �u���[�L�����vSW���擾 */
	result= TCAN_Ctrl_GetMTBreakLamp(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->brake_lamp_sw     = (data & 0x01);
		tcan_info->en_brake_lamp_sw  = 1;
	}
	else {
		tcan_info->en_brake_lamp_sw = 0;
	}

	/* �p�[�L���O�����vSW���擾 */
	result= TCAN_Ctrl_GetMTHandBreak(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->hand_brake_sw    = (data & 0x01);
		tcan_info->en_hand_brake_sw = 1;
	}
	else {
		tcan_info->en_hand_brake_sw = 0;
	}

	/* MT���o�[�XSW���擾 */
	result= TCAN_Ctrl_GetMTReverse(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->mt_rev_sw    = (data & 0x01);
		tcan_info->en_mt_rev_sw = 1;
	}
	else {
		tcan_info->en_mt_rev_sw = 0;
	}

	/* �w�b�h�����v Hi�r�[�����擾 */
	result= TCAN_Ctrl_GetMTHeadLampHi(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->head_lamp_hi    = (data & 0x01);
		tcan_info->en_head_lamp_hi = 1;
	}
	else {
		tcan_info->en_head_lamp_hi = 0;
	}

	/* �w�b�h�����v Lo�r�[�����擾 */
	result= TCAN_Ctrl_GetMTHeadLampLo(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->head_lamp_lo    = (data & 0x01);
		tcan_info->en_head_lamp_lo = 1;
	}
	else {
		tcan_info->en_head_lamp_lo = 0;
	}

	/* �w�b�h�����v �X���[�����擾 */
	result= TCAN_Ctrl_GetMTHeadLampSmall(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->head_lamp_small    = (data & 0x01);
		tcan_info->en_head_lamp_small = 1;
	}
	else {
		tcan_info->en_head_lamp_small = 0;
	}
	
	/* �h�ASW �g�����N���擾 */
	result = TCAN_Ctrl_GetMTTrunkDoorOpen(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->door_sw_trunk    = (data & 0x01);
		tcan_info->en_door_sw_trunk = 1;
	}
	else {
		tcan_info->en_door_sw_trunk = 0;
	}

	/* �h�ASW P�ȏ��擾 */
	result = TCAN_Ctrl_GetMTPDoorOpen(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->door_sw_seat_p    = (data & 0x01);
		tcan_info->en_door_sw_seat_p = 1;
	}
	else {
		tcan_info->en_door_sw_seat_p = 0;
	}

	/* �h�ASW D�ȏ��擾 */
	result = TCAN_Ctrl_GetMTDoorOpen(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->door_sw_seat_d    = (data & 0x01);
		tcan_info->en_door_sw_seat_d = 1;
	}
	else {
		tcan_info->en_door_sw_seat_d = 0;
	}

	/* �h�ASW �t�[�h���擾 */
	result = TCAN_Ctrl_GetMtHoodDoorSW(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->door_sw_hood    = (data & 0x01);
		tcan_info->en_door_sw_hood = 1;
	}
	else {
		tcan_info->en_door_sw_hood = 0;
	}
}


/********************************************************/
/* �֐�����		: update_tcan_info_id_345				*/
/* �@�\�̐���	: T-CAN ID=345�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_345(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* �g�����X�~�b�V�������擾 */
	result = TCAN_Ctrl_GetMTTransmissionType(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->transmission_type    = (data & 0x01);
		tcan_info->en_transmission_type = 1;
	}
	else {
		tcan_info->en_transmission_type = 0;
	}
	
	/* �G���W���������擾 */
	result = TCAN_Ctrl_GetMTTempOil(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->temp_oil    = data;
		tcan_info->en_temp_oil = 1;
	}
	else {
		tcan_info->en_temp_oil = 0;
	}

	/* �G���W���������擾 */
	result = TCAN_Ctrl_GetMTTempCoolant(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->temp_coolant    = data;
		tcan_info->en_temp_coolant = 1;
	}
	else {
		tcan_info->en_temp_coolant = 0;
	}
	
	/* �z���ǈ��͏��擾 */
	result = TCAN_Ctrl_GetAtIntakePress(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->intake_press    = data;
		tcan_info->en_intake_press = 1;
	}
	else {
		tcan_info->en_intake_press = 0;
	}

	/* ��C�����擾 */
	result = TCAN_Ctrl_GetAtAtmosphericPress(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->atmospheric_press    = data;
		tcan_info->en_atmospheric_press = 1;
	}
	else {
		tcan_info->en_atmospheric_press = 0;
	}
	
}


/********************************************************/
/* �֐�����		: update_tcan_info_id_390				*/
/* �@�\�̐���	: T-CAN ID=390�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_390(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* �\���O�C�����擾 */
	result = TCAN_Ctrl_GetMTOutTemp(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->out_temp    = data;
		tcan_info->en_out_temp = 1;
	}
	else {
		tcan_info->en_out_temp = 0;
	}

	/* �V�[�g�x���gSW P�ȏ��擾 */
	result = TCAN_Ctrl_GetMTPSeatBelt(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->seat_belt_sw_p    = (data & 0x01);
		tcan_info->en_seat_belt_sw_p = 1;
	}
	else {
		tcan_info->en_seat_belt_sw_p = 0;
	}

	/* �V�[�g�x���gSW D�ȏ��擾 */
	result = TCAN_Ctrl_GetMTDSeatBelt(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->seat_belt_sw_d    = (data & 0x01);
		tcan_info->en_seat_belt_sw_d = 1;
	}
	else {
		tcan_info->en_seat_belt_sw_d = 0;
	}

	/* �d���n���擾 */
	result = TCAN_Ctrl_GetMTPlace(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->destination    = (data & 0x0f);
		tcan_info->en_destination = 1;
	}
	else {
		tcan_info->en_destination = 0;
	}

	/* �E�B���J�[�����擾 */
	result = TCAN_Ctrl_GetAtBlinkerLeft(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->blinker_left    = (data & 0x0f);
		tcan_info->en_blinker_left = 1;
	}
	else {
		tcan_info->en_blinker_left = 0;
	}

	/* �E�B���J�[�E���擾 */
	result = TCAN_Ctrl_GetAtBlinkerRight(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->blinker_right    = (data & 0x0f);
		tcan_info->en_blinker_right = 1;
	}
	else {
		tcan_info->en_blinker_right = 0;
	}

}


/********************************************************/
/* �֐�����		: update_tcan_info_id_138				*/
/* �@�\�̐���	: T-CAN ID=138�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_138(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* �Ǌp�Z���T���擾 */
	result = TCAN_Ctrl_GetMTSteerAngle(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->steer_angle    = data_16;
		tcan_info->en_steer_angle = 1;
	}
	else {
		tcan_info->en_steer_angle = 0;
	}

	/* ���[���[�g���擾 */
	result = TCAN_Ctrl_GetMTYawRate(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->yaw_rate    = data_16;
		tcan_info->en_yaw_rate = 1;
	}
	else {
		tcan_info->en_yaw_rate = 0;
	}

}

/********************************************************/
/* �֐�����		: update_tcan_info_id_13B				*/
/* �@�\�̐���	: T-CAN ID=13B�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_13B(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16          data_16;
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* �������x���擾 */
	result = TCAN_Ctrl_GetMTAccLR(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->acc_lr    = data_8;
		tcan_info->en_acc_lr = 1;
	}
	else {
		tcan_info->en_acc_lr = 0;
	}

	/* �O��f�Z���T���擾 */
	result = TCAN_Ctrl_GetMTAccFR(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->acc_fr    = data_8;
		tcan_info->en_acc_fr = 1;
	}
	else {
		tcan_info->en_acc_fr = 0;
	}

	/*�G���W���g���N�A�b�v�v���t���O���擾*/	
	result = TCAN_Ctrl_GetAtEngTrqUpReqFlg(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->engTrqUpReqFlag    = (data_8 & 0x01);
		tcan_info->en_engTrqUpReqFlag = 1;
	}
	else {
		tcan_info->en_engTrqUpReqFlag = 0;
	}
	
	/*�G���W���g���N�����v���t���O���擾*/	
	result = TCAN_Ctrl_GetAtEngTrqDwnReqFlg(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->engTrqDwnReqFlag    = (data_8 & 0x01);
		tcan_info->en_engTrqDwnReqFlag = 1;
	}
	else {
		tcan_info->en_engTrqDwnReqFlag = 0;
	}

	/*�G���W���g���N�A�b�v�v���l���擾*/	
	result = TCAN_Ctrl_GetAtEngTrqUpReqVal(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->engTrqUpReqVal    = data_16;
		tcan_info->en_engTrqUpReqVal = 1;
	}
	else {
		tcan_info->en_engTrqUpReqVal = 0;
	}

	/*�G���W���g���N�����v���l���擾*/	
	result = TCAN_Ctrl_GetAtEngTrqDwnReqVal(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->engTrqDwnReqVal    = data_16;
		tcan_info->en_engTrqDwnReqVal = 1;
	}
	else {
		tcan_info->en_engTrqDwnReqVal = 0;
	}
}


/********************************************************/
/* �֐�����		: update_tcan_info_id_13A				*/
/* �@�\�̐���	: T-CAN ID=13A�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_13A(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16          data_16;
	E_TCAN_RESULT result;

	/* ���O�֎ԗ֑����擾 */
	result = TCAN_Ctrl_GetMtSpdWheelFtlh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_ftlh    = data_16;
		tcan_info->en_speed_wheel_ftlh = 1;
	}
	else {
		tcan_info->en_speed_wheel_ftlh = 0;
	}

	/* �E�O�֎ԗ֑����擾 */
	result = TCAN_Ctrl_GetMtSpdWheelFtrh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_ftrh    = data_16;
		tcan_info->en_speed_wheel_ftrh = 1;
	}
	else {
		tcan_info->en_speed_wheel_ftrh = 0;
	}

	/* ����֎ԗ֑����擾 */
	result = TCAN_Ctrl_GetMtSpdWheelRrlh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_rrlh    = data_16;
		tcan_info->en_speed_wheel_rrlh = 1;
	}
	else {
		tcan_info->en_speed_wheel_rrlh = 0;
	}

	/* �E��֎ԗ֑����擾 */
	result = TCAN_Ctrl_GetMtSpdWheelRrrh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_rrrh    = data_16;
		tcan_info->en_speed_wheel_rrrh = 1;
	}
	else {
		tcan_info->en_speed_wheel_rrrh = 0;
	}
}


/********************************************************/
/* �֐�����		: update_tcan_info_id_6E2				*/
/* �@�\�̐���	: T-CAN ID=6E2�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_6E2(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8         data_8;
	E_TCAN_RESULT result;

	/* ���O�֋�C�����擾 */
	result = TCAN_Ctrl_GetAtAirPressFtlh(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->air_press_ftlh    = data_8;
		tcan_info->en_air_press_ftlh = 1;
	}
	else {
		tcan_info->en_air_press_ftlh = 0;
	}

	/* �E�O�֋�C�����擾 */
	result = TCAN_Ctrl_GetAtAirPressFtrh(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->air_press_ftrh    = data_8;
		tcan_info->en_air_press_ftrh = 1;
	}
	else {
		tcan_info->en_air_press_ftrh = 0;
	}

	/* ����֋�C�����擾 */
	result = TCAN_Ctrl_GetAtAirPressRrlh(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->air_press_rrlh    = data_8;
		tcan_info->en_air_press_rrlh = 1;
	}
	else {
		tcan_info->en_air_press_rrlh = 0;
	}

	/* �E��֋�C�����擾 */
	result = TCAN_Ctrl_GetAtAirPressRrrh(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->air_press_rrrh    = data_8;
		tcan_info->en_air_press_rrrh = 1;
	}
	else {
		tcan_info->en_air_press_rrrh = 0;
	}
	
	/* ��C���P�ʏ��擾 */
	result = TCAN_Ctrl_GetAtAirPressUnit(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->air_press_unit    = data_8;
		tcan_info->en_air_press_unit = 1;
	}
	else {
		tcan_info->en_air_press_unit = 0;
	}	

}


/********************************************************/
/* �֐�����		: update_tcan_info_id_652				*/
/* �@�\�̐���	: T-CAN ID=652�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/09/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_652(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* �h�A���b�N��ԁi����ȁj���擾 */
	result = TCAN_Ctrl_GetMtDoorLockStatPass(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->lock_stat_pass    = data_8;
		tcan_info->en_lock_stat_pass = 1;
	}
	else {
		tcan_info->en_lock_stat_pass = 0;
	}

	/* �h�A���b�N��ԁi�^�]�ȁj���擾 */
	result = TCAN_Ctrl_GetMtDoorLockStatDrvr(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->lock_stat_driver    = data_8;
		tcan_info->en_lock_stat_driver = 1;
	}
	else {
		tcan_info->en_lock_stat_driver = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_328				*/
/* �@�\�̐���	: T-CAN ID=328�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 21/02/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_328(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* TRC ON/OFF���擾 */
	result = TCAN_Ctrl_GetAtTrcOnOff(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->trcOnOff =  data_8 & 0x01;
		tcan_info->en_trcOnOff = 1;
	}
	else {
		tcan_info->en_trcOnOff = 0;
	}
	/* ECB�E�H�[�j���O���擾 			*/
	result = TCAN_Ctrl_GetAtEcbWarning(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->ecbWarning =  data_8 & 0x01;
		tcan_info->en_ecbWarning = 1;
	}
	else {
		tcan_info->en_vscWarning = 0;
	}
	/* VSC�E�H�[�j���O���擾 			*/
	result = TCAN_Ctrl_GetAtVscWarning(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->vscWarning =  data_8 & 0x01;
		tcan_info->en_vscWarning = 1;
	}
	else {
		tcan_info->en_vscWarning = 0;
	}

	/* ABS�E�H�[�j���O���擾 			*/
	result = TCAN_Ctrl_GetAtAbsWarning(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->absWarning =  data_8 & 0x01;
		tcan_info->en_absWarning = 1;
	}
	else {
		tcan_info->en_absWarning = 0;
	}

	/* VSC ON/OFF���擾 */
	result = TCAN_Ctrl_GetAtVscOnOff(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->vscOnOff =  data_8 & 0x01;
		tcan_info->en_vscOnOff = 1;
	}
	else {
		tcan_info->en_vscOnOff = 0;
	}

	/* VSC�����Ԏ擾 */
	result = TCAN_Ctrl_GetAtVscAct(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->vscAct    =  data_8 & 0x01;
		tcan_info->en_vscAct = 1;
	}
	else {
		tcan_info->en_vscAct = 0;
	}

	/* �g���N�V�����d�����[�h��Ԏ擾 */
	result = TCAN_Ctrl_GetAtTrcMode(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->trcMode    =  data_8 & 0x01;
		tcan_info->en_trcMode = 1;
	}
	else {
		tcan_info->en_trcMode = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_04B				*/
/* �@�\�̐���	: T-CAN ID=141�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 21/03/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_04B(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* �ڕW�M�A�ʒu���擾 */
	result = TCAN_Ctrl_GetMTTargetGearPos(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->target_gear_pos = (data_8 & 0x0f);
		tcan_info->en_target_gear_pos = 1;
	}
	else {
		tcan_info->en_target_gear_pos = 0;
	}

}
/********************************************************/
/* �֐�����		: update_tcan_info_id_041				*/
/* �@�\�̐���	: T-CAN ID=141�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 21/03/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_041(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16          data_16;
	E_TCAN_RESULT result;

	/* ���G���W�����g���N���擾 */
	result = TCAN_Ctrl_GetAtEngTrqShaft(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->engTrcShaft = (data_16 & 0x7FFF);
		tcan_info->en_engTrcShaft = 1;
	}
	else {
		tcan_info->en_engTrcShaft = 0;
	}

}

/********************************************************/
/* �֐�����		: update_tcan_info_id_152				*/
/* �@�\�̐���	: T-CAN ID=152�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_152(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* IGN���擾 */
	result = TCAN_Ctrl_GetMTIgn(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->ign    = (data & 0x01);
		tcan_info->en_ign = 1;
	}
	else {
		tcan_info->en_ign = 0;
	}
	
	/* ACC���擾 */
	result= TCAN_Ctrl_GetMTAcc(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->acc    = (data & 0x01);
		tcan_info->en_acc = 1;
	}
	else {
		tcan_info->en_acc = 0;
	}
	
	/* �u���[�L�����vSW���擾 */
	result= TCAN_Ctrl_GetMTBreakLamp(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->brake_lamp_sw     = (data & 0x01);
		tcan_info->en_brake_lamp_sw  = 1;
	}
	else {
		tcan_info->en_brake_lamp_sw = 0;
	}

	/* �p�[�L���O�����vSW���擾 */
	result= TCAN_Ctrl_GetMTHandBreak(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->hand_brake_sw    = (data & 0x01);
		tcan_info->en_hand_brake_sw = 1;
	}
	else {
		tcan_info->en_hand_brake_sw = 0;
	}

	/* MT���o�[�XSW���擾 */
	result= TCAN_Ctrl_GetMTReverse(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->mt_rev_sw    = (data & 0x01);
		tcan_info->en_mt_rev_sw = 1;
	}
	else {
		tcan_info->en_mt_rev_sw = 0;
	}

	/* �w�b�h�����v Hi�r�[�����擾 */
	result= TCAN_Ctrl_GetMTHeadLampHi(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->head_lamp_hi    = (data & 0x01);
		tcan_info->en_head_lamp_hi = 1;
	}
	else {
		tcan_info->en_head_lamp_hi = 0;
	}

	/* �w�b�h�����v Lo�r�[�����擾 */
	result= TCAN_Ctrl_GetMTHeadLampLo(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->head_lamp_lo    = (data & 0x01);
		tcan_info->en_head_lamp_lo = 1;
	}
	else {
		tcan_info->en_head_lamp_lo = 0;
	}

	/* �w�b�h�����v �X���[�����擾 */
	result= TCAN_Ctrl_GetMTHeadLampSmall(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->head_lamp_small    = (data & 0x01);
		tcan_info->en_head_lamp_small = 1;
	}
	else {
		tcan_info->en_head_lamp_small = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_374				*/
/* �@�\�̐���	: T-CAN ID=374�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_374(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* �h�ASW �g�����N���擾 */
	result = TCAN_Ctrl_GetMTTrunkDoorOpen(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->door_sw_trunk    = (data & 0x01);
		tcan_info->en_door_sw_trunk = 1;
	}
	else {
		tcan_info->en_door_sw_trunk = 0;
	}

	/* �h�ASW P�ȏ��擾 */
	result = TCAN_Ctrl_GetMTPDoorOpen(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->door_sw_seat_p    = (data & 0x01);
		tcan_info->en_door_sw_seat_p = 1;
	}
	else {
		tcan_info->en_door_sw_seat_p = 0;
	}

	/* �h�ASW D�ȏ��擾 */
	result = TCAN_Ctrl_GetMTDoorOpen(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->door_sw_seat_d    = (data & 0x01);
		tcan_info->en_door_sw_seat_d = 1;
	}
	else {
		tcan_info->en_door_sw_seat_d = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_282				*/
/* �@�\�̐���	: T-CAN ID=282�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_282(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* �\���O�C�����擾 */
	result = TCAN_Ctrl_GetMTOutTemp(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->out_temp    = data;
		tcan_info->en_out_temp = 1;
	}
	else {
		tcan_info->en_out_temp = 0;
	}

	/* �V�[�g�x���gSW P�ȏ��擾 */
	result = TCAN_Ctrl_GetMTPSeatBelt(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->seat_belt_sw_p    = (data & 0x01);
		tcan_info->en_seat_belt_sw_p = 1;
	}
	else {
		tcan_info->en_seat_belt_sw_p = 0;
	}

	/* �V�[�g�x���gSW D�ȏ��擾 */
	result = TCAN_Ctrl_GetMTDSeatBelt(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->seat_belt_sw_d    = (data & 0x01);
		tcan_info->en_seat_belt_sw_d = 1;
	}
	else {
		tcan_info->en_seat_belt_sw_d = 0;
	}

	/* �d���n���擾 */
	result = TCAN_Ctrl_GetMTPlace(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->destination    = (data & 0x0f);
		tcan_info->en_destination = 1;
	}
	else {
		tcan_info->en_destination = 0;
	}

	/* �u�����h�t���O���擾 */
	result = TCAN_Ctrl_GetMTBrandFlag(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->brand_flag    = (data & 0x01);
		tcan_info->en_brand_flag = 1;
	}
	else {
		tcan_info->en_brand_flag = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_140				*/
/* �@�\�̐���	: T-CAN ID=140�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_140(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* �A�N�Z���J�x���擾 */
	result = TCAN_Ctrl_GetMTRatioAccel(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->accel_ratio    = data_8;
		tcan_info->en_accel_ratio = 1;
	}
	else {
		tcan_info->en_accel_ratio = 0;
	}

	/* �G���W����]�����擾 */
	result = TCAN_Ctrl_GetMTEngRpm(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->eng_rpm    = data_16;
		tcan_info->en_eng_rpm = 1;
	}
	else {
		tcan_info->en_eng_rpm = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_141				*/
/* �@�\�̐���	: T-CAN ID=141�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_141(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* �G���W����]���^�R���[�^�p���擾 */
	result = TCAN_Ctrl_GetMTEngRpmTa(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->eng_rpm_taco    = data_16;
		tcan_info->en_eng_rpm_taco = 1;
	}
	else {
		tcan_info->en_eng_rpm_taco = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_144				*/
/* �@�\�̐���	: T-CAN ID=152�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_144(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data;
	E_TCAN_RESULT result;

	/* �g�����X�~�b�V�������擾 */
	result = TCAN_Ctrl_GetMTTransmissionType(&data);
	if(result == TCAN_CTRL_OK) {
		tcan_info->transmission_type    = (data & 0x01);
		tcan_info->en_transmission_type = 1;
	}
	else {
		tcan_info->en_transmission_type = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_360				*/
/* �@�\�̐���	: T-CAN ID=360�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_360(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* �G���W���������擾 */
	result = TCAN_Ctrl_GetMTTempOil(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->temp_oil    = data_8;
		tcan_info->en_temp_oil = 1;
	}
	else {
		tcan_info->en_temp_oil = 0;
	}

	/* �G���W���������擾 */
	result = TCAN_Ctrl_GetMTTempCoolant(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->temp_coolant    = data_8;
		tcan_info->en_temp_coolant = 1;
	}
	else {
		tcan_info->en_temp_coolant = 0;
	}

	/* �N���R���Z�b�g�����v���擾 */
	result = TCAN_Ctrl_GetMTSetCc(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->cruise_controlrl_set    = (data_8 & 0x01);
		tcan_info->en_cruise_controlrl_set = 1;
	}
	else {
		tcan_info->en_cruise_controlrl_set = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_361				*/
/* �@�\�̐���	: T-CAN ID=361�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_361(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* MT�M�A�|�W�V�����\�����擾 */
	result = TCAN_Ctrl_GetMTGearEstm(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->mt_gear_pos    = (data_8 & 0x07);
		tcan_info->en_mt_gear_pos = 1;
	}
	else {
		tcan_info->en_mt_gear_pos = 0;
	}
}
/********************************************************/
/* �֐�����		: update_tcan_info_id_148				*/
/* �@�\�̐���	: T-CAN ID=148�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_148(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* �X�|�[�c�V�t�g�M�A�ʒu�\�����擾 */
	result = TCAN_Ctrl_GetMTSportGearPosition(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->sports_gear_pos    = (data_8 & 0x0f);
		tcan_info->en_sports_gear_pos = 1;
	}
	else {
		tcan_info->en_sports_gear_pos = 0;
	}

	/* �V�t�g�ʒu���(�t�F�[���Z�[�t������)���擾 */
	result = TCAN_Ctrl_GetMTShiftPosition(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->shift_pos    = (data_8 & 0x0f);
		tcan_info->en_shift_pos = 1;
	}
	else {
		tcan_info->en_shift_pos = 0;
	}

	/* TCM�ϑ����䃂�[�h���擾 */
	result = TCAN_Ctrl_GetMTModeShiftControl(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->mode_shift_control    = (data_8 & 0x07);
		tcan_info->en_mode_shift_control = 1;
	}
	else {
		tcan_info->en_mode_shift_control = 0;
	}

	/* �ڕW�M�A�ʒu���擾 */
	result = TCAN_Ctrl_GetMTTargetGearPos(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->target_gear_pos = (data_8 & 0x0f);
		tcan_info->en_target_gear_pos = 1;
	}
	else {
		tcan_info->en_target_gear_pos = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_368				*/
/* �@�\�̐���	: T-CAN ID=368�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_368(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* AT��ޏ��擾 */
	result = TCAN_Ctrl_GetMTAtType(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->at_type    = (data_8 & 0x07);
		tcan_info->en_at_type = 1;
	}
	else {
		tcan_info->en_at_type = 0;
	}

	/* �X�|�[�c�����v���擾 */
	result = TCAN_Ctrl_GetMTSportsLamp(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->sports_lamp    = (data_8 & 0x03);
		tcan_info->en_sports_lamp = 1;
	}
	else {
		tcan_info->en_sports_lamp = 0;
	}

	/* �X�m�[�z�[���h�����v���擾 */
	result = TCAN_Ctrl_GetMTSnowHoldLamp(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->snow_hold_lamp    = (data_8 & 0x03);
		tcan_info->en_snow_hold_lamp = 1;
	}
	else {
		tcan_info->en_snow_hold_lamp = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_0d0				*/
/* �@�\�̐���	: T-CAN ID=0d0�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_0d0(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* �Ǌp�Z���T���擾 */
	result = TCAN_Ctrl_GetMTSteerAngle(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->steer_angle    = data_16;
		tcan_info->en_steer_angle = 1;
	}
	else {
		tcan_info->en_steer_angle = 0;
	}

	/* ���[���[�g���擾 */
	result = TCAN_Ctrl_GetMTYawRate(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->yaw_rate    = data_16;
		tcan_info->en_yaw_rate = 1;
	}
	else {
		tcan_info->en_yaw_rate = 0;
	}

	/* �������x���擾 */
	result = TCAN_Ctrl_GetMTAccLR(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->acc_lr    = data_8;
		tcan_info->en_acc_lr = 1;
	}
	else {
		tcan_info->en_acc_lr = 0;
	}

	/* �O��f�Z���T���擾 */
	result = TCAN_Ctrl_GetMTAccFR(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->acc_fr    = data_8;
		tcan_info->en_acc_fr = 1;
	}
	else {
		tcan_info->en_acc_fr = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_0d1				*/
/* �@�\�̐���	: T-CAN ID=0d1�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_0d1(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16         data_16;
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* �쓮�֕��ώԗ֑����擾 */
	result = TCAN_Ctrl_GetMTSpWheelAve(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->wheel_speed_ave    = data_16;
		tcan_info->en_wheel_speed_ave = 1;
	}
	else {
		tcan_info->en_wheel_speed_ave = 0;
	}

	/* �u���[�L�����擾 */
	result = TCAN_Ctrl_GetMtBrakePressure(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->brake_pressure    = data_8;
		tcan_info->en_brake_pressure = 1;
	}
	else {
		tcan_info->en_brake_pressure = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_0d3				*/
/* �@�\�̐���	: T-CAN ID=0d3�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_0d3(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* �ԑ��p���X�ώZ�l���擾 */
	result = TCAN_Ctrl_GetMTSpPulseInteg(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_pulse_integ    = (data_8 & 0x3f);
		tcan_info->en_speed_pulse_integ = 1;
	}
	else {
		tcan_info->en_speed_pulse_integ = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_0d4				*/
/* �@�\�̐���	: T-CAN ID=0d4�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/05								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_0d4(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16          data_16;
	E_TCAN_RESULT result;

	/* ���O�֎ԗ֑����擾 */
	result = TCAN_Ctrl_GetMtSpdWheelFtlh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_ftlh    = data_16;
		tcan_info->en_speed_wheel_ftlh = 1;
	}
	else {
		tcan_info->en_speed_wheel_ftlh = 0;
	}

	/* �E�O�֎ԗ֑����擾 */
	result = TCAN_Ctrl_GetMtSpdWheelFtrh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_ftrh    = data_16;
		tcan_info->en_speed_wheel_ftrh = 1;
	}
	else {
		tcan_info->en_speed_wheel_ftrh = 0;
	}

	/* ����֎ԗ֑����擾 */
	result = TCAN_Ctrl_GetMtSpdWheelRrlh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_rrlh    = data_16;
		tcan_info->en_speed_wheel_rrlh = 1;
	}
	else {
		tcan_info->en_speed_wheel_rrlh = 0;
	}

	/* �E��֎ԗ֑����擾 */
	result = TCAN_Ctrl_GetMtSpdWheelRrrh(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->speed_wheel_rrrh    = data_16;
		tcan_info->en_speed_wheel_rrrh = 1;
	}
	else {
		tcan_info->en_speed_wheel_rrrh = 0;
	}
}

#ifdef _FOR_RCF
CAN_CTRL_LOCAL void update_tcan_info_id_0b4(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* �쓮�֕��ώԗ֑����擾 */
	result = TCAN_Ctrl_GetMTSpWheelAve(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->wheel_speed_ave    = data_16;
		tcan_info->en_wheel_speed_ave = 1;
	}
	else {
		tcan_info->en_wheel_speed_ave = 0;
	}
}
#endif	/* _FOR_RCF */

/********************************************************/
/* �֐�����		: update_tcan_info_id_44d				*/
/* �@�\�̐���	: T-CAN ID=44D�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/26								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_44d(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	UI_16         data_16;
	E_TCAN_RESULT result;

	/* �m�[�hID���擾 */
	result = TCAN_Ctrl_GetMTNodeID(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->node_id  = data_8;
		tcan_info->en_node_id = 1;
	}
	else {
		tcan_info->en_node_id = 0;
	}

	/* Sleep Ack���擾 */
	result = TCAN_Ctrl_GetMTSleepAck(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->sleep_ack    = (data_8 & 0x01);
		tcan_info->en_sleep_ack = 1;
	}
	else {
		tcan_info->en_sleep_ack = 0;
	}

	/* Sleep Ind���擾 */
	result = TCAN_Ctrl_GetMTSleepInd(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->sleep_ind    = (data_8 & 0x01);
		tcan_info->en_sleep_ind = 1;
	}
	else {
		tcan_info->en_sleep_ind = 0;
	}

	/* Limp Home���擾 */
	result = TCAN_Ctrl_GetMTLimpHome(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->limp_home    = (data_8 & 0x01);
		tcan_info->en_limp_home = 1;
	}
	else {
		tcan_info->en_limp_home = 0;
	}

	/* Ring���擾 */
	result = TCAN_Ctrl_GetMTRing(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->ring    = (data_8 & 0x01);
		tcan_info->en_ring = 1;
	}
	else {
		tcan_info->en_ring = 0;
	}

	/* Alive���擾 */
	result = TCAN_Ctrl_GetMTAlive(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->alive    = (data_8 & 0x01);
		tcan_info->en_alive = 1;
	}
	else {
		tcan_info->en_alive = 0;
	}

	/* Wkup���擾 */
	result = TCAN_Ctrl_GetMTWkup(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->wkup    = (data_8 & 0x01);
		tcan_info->en_wkup = 1;
	}
	else {
		tcan_info->en_wkup = 0;
	}

	/* Init���擾 */
	result = TCAN_Ctrl_GetMTInit(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->init    = (data_8 & 0x01);
		tcan_info->en_init = 1;
	}
	else {
		tcan_info->en_init = 0;
	}

	/* Spec. No.���擾 */
	result = TCAN_Ctrl_GetMTSpecNo(&data_16);
	if(result == TCAN_CTRL_OK) {
		tcan_info->spec_no    = data_16;
		tcan_info->en_spec_no = 1;
	}
	else {
		tcan_info->en_spec_no = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_id_375				*/
/* �@�\�̐���	: T-CAN ID=375�̃J�����g�����X�V����	*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/11/03								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_id_375(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8          data_8;
	E_TCAN_RESULT result;

	/* �h�A���b�N��ԁi����ȁj���擾 */
	result = TCAN_Ctrl_GetMtDoorLockStatPass(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->lock_stat_pass    = data_8;
		tcan_info->en_lock_stat_pass = 1;
	}
	else {
		tcan_info->en_lock_stat_pass = 0;
	}

	/* �h�A���b�N��ԁi�^�]�ȁj���擾 */
	result = TCAN_Ctrl_GetMtDoorLockStatDrvr(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->lock_stat_driver    = data_8;
		tcan_info->en_lock_stat_driver = 1;
	}
	else {
		tcan_info->en_lock_stat_driver = 0;
	}
}

/********************************************************/
/* �֐�����		: update_tcan_info_calc					*/
/* �@�\�̐���	: T-CAN �����Z�o�̃J�����g�����X�V����*/
/* ����			: ( /O)tcan_info:T-CAN�J�����g��� 		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_tcan_info_calc(T_Can_ctrl_tcan_info *tcan_info)
{
	UI_8         data_8;
	UI_32         data_32;
	E_TCAN_RESULT result;

	/* �n���㑖�s�������擾 */
	result = TCAN_Ctrl_GetOdo(&data_32);
	if(result == TCAN_CTRL_OK) {
		tcan_info->running_distance    = data_32;
		tcan_info->en_running_distance = 1;
	}
	else {
		tcan_info->en_running_distance = 0;
	}

	/* �R�����˗ʐώZ�l���擾 */
	result = TCAN_Ctrl_GetFCSum(&data_32);
	if(result == TCAN_CTRL_OK) {
		tcan_info->fuel_consumption    =  data_32;
		tcan_info->en_fuel_consumption = 1;
	}
	else {
		tcan_info->en_fuel_consumption = 0;
	}

	/* �\���p�ԑ����擾 */
	result = TCAN_Ctrl_GetVehicleSpeedForDisp(&data_32);
	if(result == TCAN_CTRL_OK) {
		tcan_info->vehicle_speed_for_disp    =  data_32;
		tcan_info->en_vehicle_speed_for_disp = 1;
	}
	else {
		tcan_info->en_vehicle_speed_for_disp = 0;
	}

	/* �Ȃ܂��u���[�L���擾 */
	result = TCAN_Ctrl_GetSmoothingBrakePressure(&data_8);
	if(result == TCAN_CTRL_OK) {
		tcan_info->smoothing_brake_pressure    =  data_8;
		tcan_info->en_smoothing_brake_pressure = 1;
	}
	else {
		tcan_info->en_smoothing_brake_pressure = 0;
	}
}

#ifdef ISOCAN_ENABLE
/********************************************************/
/* �֐�����		: update_iso_can_info_all				*/
/* �@�\�̐���	: ISO-CAN�J�����g�����X�V����			*/
/* ����			: �Ȃ�									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/11/08								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void update_iso_can_info_all(void)
{
	E_ISO_CAN_RESULT result;
	UI_8  data_8;
	UI_16 data_16;
	
	/* �G���W���������擾 */
	result = Iso_can_ctrl_get_eng_cool_temp(&data_8);
	if(result == ISO_CAN_CTRL_OK) {
		S_Can_ctrl_iso_can_info.temp_colant    =  data_8;
		S_Can_ctrl_iso_can_info.en_temp_colant = 1;
	}
	else {
		S_Can_ctrl_iso_can_info.en_temp_colant = 0;
	}

	/* �z�C�}�j�t�H�[���h�����擾 */
	result = Iso_can_ctrl_get_im_abs_press(&data_8);
	if(result == ISO_CAN_CTRL_OK) {
		S_Can_ctrl_iso_can_info.ima_pressure    =  data_8;
		S_Can_ctrl_iso_can_info.en_ima_pressure = 1;
	}
	else {
		S_Can_ctrl_iso_can_info.en_ima_pressure = 0;
	}

	/* �G���W����]�����擾 */
	result = Iso_can_ctrl_get_eng_rpm(&data_16);
	if(result == ISO_CAN_CTRL_OK) {
		S_Can_ctrl_iso_can_info.eng_rpm    =  data_16;
		S_Can_ctrl_iso_can_info.en_eng_rpm = 1;
	}
	else {
		S_Can_ctrl_iso_can_info.en_eng_rpm = 0;
	}

	/* �ԑ����擾 */
	result = Iso_can_ctrl_get_vehicle_speed(&data_8);
	if(result == ISO_CAN_CTRL_OK) {
		S_Can_ctrl_iso_can_info.vehicle_speed    =  data_8;
		S_Can_ctrl_iso_can_info.en_vehicle_speed = 1;
	}
	else {
		S_Can_ctrl_iso_can_info.en_vehicle_speed = 0;
	}

	/* �X���b�g���ʒu��擾 */
	result = Iso_can_ctrl_get_throttle_pos(&data_8);
	if(result == ISO_CAN_CTRL_OK) {
		S_Can_ctrl_iso_can_info.throttle_pos    =  data_8;
		S_Can_ctrl_iso_can_info.en_throttle_pos = 1;
	}
	else {
		S_Can_ctrl_iso_can_info.en_throttle_pos = 0;
	}

	/* �d�����擾 */
	result = Iso_can_ctrl_get_ctrl_mod_volt(&data_16);
	if(result == ISO_CAN_CTRL_OK) {
		S_Can_ctrl_iso_can_info.volts    =  data_16;
		S_Can_ctrl_iso_can_info.en_volts = 1;
	}
	else {
		S_Can_ctrl_iso_can_info.en_volts = 0;
	}

	/* �z�C���x���擾 */
	result = Iso_can_ctrl_get_intake_air_temp(&data_8);
	if(result == ISO_CAN_CTRL_OK) {
		S_Can_ctrl_iso_can_info.temp_intake_manifold    =  data_8;
		S_Can_ctrl_iso_can_info.en_temp_intake_manifold = 1;
	}
	else {
		S_Can_ctrl_iso_can_info.en_temp_intake_manifold = 0;
	}

}
#endif /* ISOCAN_ENABLE */

/********************************************************/
/* �֐�����		: receive_tcan_data						*/
/* �@�\�̐���	: T-CAN�f�[�^��M����					*/
/* ����			: �Ȃ�									*/
/* �߂�l		: N_TRUE	��M�f�[�^����				*/
/* 				: N_FALSE	��M�f�[�^�Ȃ�				*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL B_8 receive_tcan_data(void)
{
	E_RC       result;
	T_Can_data can_data;
	B_8        rcv_flg = N_FALSE;
	
	/* CAN�h���C�o�����M�ς݃f�[�^�擾 */
	result = Can_drv_get_rx_data(CAN_CH_TCAN, &can_data);
	while (result == E_RC_OK) {
		rcv_flg = N_TRUE;

		/* T-CAN�R���g���[���ł��̃f�[�^������ */
		TCAN_Ctrl_SetMessage(can_data.id, can_data.len, can_data.data);

		/* CAN�h���C�o�����M�ς݃f�[�^�擾 */
		result = Can_drv_get_rx_data(CAN_CH_TCAN, &can_data);
	}

	return rcv_flg;

}

/********************************************************/
/* �֐�����		: can_init_inspection_info				*/
/* �@�\�̐���	: ������񏉊���						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void can_init_inspection_info(void)
{
	(void)memset(&S_can_inspection_info, 0, sizeof(S_can_inspection_info));
	S_can_inspection_info.reqflg = N_OFF;
	return;
}

/********************************************************/
/* �֐�����		: can_inspect_can0						*/
/* �@�\�̐���	: CAN CH0����							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void can_inspect_can0(void)
{
	E_INSP_INSPECTION_RESULT	inspect_res = INSP_INSPECTION_RESULT_INVALID;
	T_Can_data		candata;
	E_RC			ret;

	if (Timer_ctrl_TimeCompare1ms( S_can_inspection_info.start_time, CAN_INSPECTION_TIMEOUT ) == N_OK) {
		/* CAN�����^�C���A�E�g	*/
		inspect_res = INSP_INSPECTION_RESULT_NG;
#ifdef CAN_DEBUG
		OUTLOG(OutputLog("[Can]insp can0 timeout!!\n");)
#endif /*CAN_DEBUG*/
	}
	else {
		for ( ; S_can_inspection_info.msg_cnt < CAN_INSPECTION_MSG_NUM; S_can_inspection_info.msg_cnt++) {
			/* CAN0���烁�b�Z�[�W����M	*/
			ret = Can_drv_get_rx_data(CAN_CH_0, &candata);
			if (ret != E_RC_OK) {
				/* ���b�Z�[�W����M�̏ꍇ�͏����𔲂���	*/
				break;
			}

#ifdef CAN_DEBUG
			OUTLOG(OutputLog("[Can]insp can0 rcv id:%X len:%u data:%02X %02X %02X %02X %02X %02X %02X %02X\n",
						candata.id, candata.len,
						candata.data[0], candata.data[1], candata.data[2], candata.data[3],
						candata.data[4], candata.data[5], candata.data[6], candata.data[7]);)
#endif /*CAN_DEBUG*/

			/* ��M�������b�Z�[�W��CAN0�ACAN1�ɑ��M����	*/
			ret = Can_drv_set_tx_data(CAN_CH_0, ((S_can_inspection_info.msg_cnt % CAN_INSPECTION_SEND_BUFFER_COUNT) + 1), candata.id, candata.data, candata.len);
			if (ret != E_RC_OK) {
				/* CAN0 ���M�ُ�	*/
				inspect_res = INSP_INSPECTION_RESULT_NG;
#ifdef CAN_DEBUG
				OUTLOG(OutputLog("[Can]insp can0 can0 snd err!!\n");)
#endif /*CAN_DEBUG*/
				break;
			}
			ret = Can_drv_set_tx_data(CAN_CH_1, ((S_can_inspection_info.msg_cnt % CAN_INSPECTION_SEND_BUFFER_COUNT) + 1), candata.id, candata.data, candata.len);
			if (ret != E_RC_OK) {
				/* CAN1 ���M�ُ�	*/
				inspect_res = INSP_INSPECTION_RESULT_NG;
#ifdef CAN_DEBUG
				OUTLOG(OutputLog("[Can]insp can0 can1 snd err!!\n");)
#endif /*CAN_DEBUG*/
				break;
			}
		}
		if (S_can_inspection_info.msg_cnt == CAN_INSPECTION_MSG_NUM) {
			/* CAN���� ����	*/
			inspect_res = INSP_INSPECTION_RESULT_OK;
		}
	}

	if (inspect_res != INSP_INSPECTION_RESULT_INVALID) {
		/* �������䃂�W���[���Ɍ��ʂ�ʒm����	*/
		Insp_notify_inspection_result(INSP_INSPECTION_TYPE_CAN0, NULL, inspect_res);

		/* ���������N���A	*/
		can_init_inspection_info();
	}

	return;
}

/********************************************************/
/* �֐�����		: can_inspect_can1						*/
/* �@�\�̐���	: CAN CH1����							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void can_inspect_can1(void)
{
	/* CAN1�͌����Ȃ�	*/
	/* �������䃂�W���[���Ɍ���OK��ʒm����		*/
	Insp_notify_inspection_result(INSP_INSPECTION_TYPE_CAN1, NULL, INSP_INSPECTION_RESULT_OK);

	/* ���������N���A	*/
	can_init_inspection_info();

	return;
}

/********************************************************/
/* �֐�����		: can_inspect_stop						*/
/* �@�\�̐���	: CAN������~							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void can_inspect_stop(void)
{
	E_INSP_INSPECTION_TYPE	type;

	if (S_can_inspection_info.ch == CAN_INSPECTION_CAN_CH0) {
		type = INSP_INSPECTION_TYPE_CAN0;
	}
	else if (S_can_inspection_info.ch == CAN_INSPECTION_CAN_CH1) {
		type = INSP_INSPECTION_TYPE_CAN1;
	}
	else {
		type = INSP_INSPECTION_TYPE_INVLID;
	}

	if (type != INSP_INSPECTION_TYPE_INVLID) {
		/* �������䃂�W���[���Ɍ���NG��ʒm����		*/
		Insp_notify_inspection_result(type, NULL, INSP_INSPECTION_RESULT_NG);
	}

	/* ���������N���A	*/
	can_init_inspection_info();

	return;
}


/********************************************************/
/* �֐�����		: readEepData							*/
/* �@�\�̐���	: �s��������������f�[�^�ǂݍ���		*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/11/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void readEepData(void)
{
	UI_16 vehicleInfo = 0x0000;	
	/* EEPROM����Ԏ����ǂ݂���	*/
	(void)Eeprom_control_GetData(EEPROM_DATA_VEHICLEINFO, &vehicleInfo);
	/*�ԗ�������*/
	S_Can_ctrl_tcan_info.en_eep_vehicle_info = 1;	
	if ( (vehicleInfo == (UI_16)TCAN_VEHICLE_INFO_86A) || (vehicleInfo == (UI_16)TCAN_VEHICLE_INFO_86B) ){
		/*86A �܂��� 86B*/
		S_Can_ctrl_tcan_info.eep_vehicle_info = vehicleInfo;
	}else{
		/*�s��*/
		/*�ň��s����Ԃ���ω����Ȃ��ƍ���̂ŕs������86B�Ƃ���*/
		S_Can_ctrl_tcan_info.eep_vehicle_info = (UI_16)TCAN_VEHICLE_INFO_86B;
	}
#ifdef CAN_DEBUG
	OUTLOG(OutputLog( "[CAN] EEPROM Read (%04x)\n", S_Can_ctrl_tcan_info.eep_vehicle_info );)
#endif /*CAN_DEBUG*/
}

/********************************************************/
/* �֐�����		: writeEepData							*/
/* �@�\�̐���	: �s�����������փf�[�^��������			*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 20/11/-								*/
/* ���l			: 										*/
/********************************************************/
CAN_CTRL_LOCAL void writeEepData(void)
{
	UI_16 vehicleInfo;

#ifdef CAN_DEBUG
	OUTLOG(OutputLog( "[CAN] EEPROM Write (%04x)\n", S_Can_ctrl_tcan_info.eep_vehicle_info );)
#endif /*CAN_DEBUG*/
	/*�Ԏ���*/
	vehicleInfo = S_Can_ctrl_tcan_info.eep_vehicle_info;
	/* EEPROM�֎Ԏ���������o��	*/
	(void)Eeprom_control_SetData(EEPROM_DATA_VEHICLEINFO, vehicleInfo, NULL);
}
