/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: FR81S								*/
/* Date				: 17/01/20							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Toguchi						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		: 									*/
/* date				: 									*/
/********************************************************/
/********************************************************/
/* file DTO459_VDV_Inspection_ctrl.c					*/
/* SD����												*/
/********************************************************/

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include <string.h>
#include "system.h"
#include "SSFTSTD_Macro.h"
#include "System_control_TASK.h"
#include "timer_ctrl.h"
#include "DTO459_VDV_Sw_ctrl.h"
#include "DTO459_VDV_Bt_ctrl.h"
#include "DTO459_VDV_Gnss_ctrl.h"
#include "DTO459_VDV_Can_ctrl.h"
#include "DTO459_VDV_Sd_ctrl.h"
#include "DTO459_VDV_Eeprom_ctrl.h"
#include "DTO459_VDV_Advance_ctrl.h"
#include "DTO459_VDV_Common.h"
#include "DTO459_VDV_Inspection_ctrl.h"
#include "dbglog.h"

/********************************************************/
/*	�萔��`											*/
/********************************************************/
/* �������W���[�������� */
typedef enum e_insp_stat {
	INSP_STAT_INIT = 0,								/* [S00]�������						*/
	INSP_STAT_WAIT_INSPECTION_START,				/* [S01]�����J�n�҂�					*/
	INSP_STAT_WAIT_INSPECTION_REQ,					/* [S02]�����v���҂�					*/
	INSP_STAT_INSPECTION,							/* [S03]������							*/
	INSP_STAT_INSPECTION_END,						/* [S04]�����I��						*/
	INSP_STAT_SHUTDOWN								/* [S05]���W���[���I��					*/
} E_INSP_STAT;

/* �������W���[������C�x���g */
typedef enum e_insp_event {
	INSP_EVENT_NONE = 0,
	INSP_EVENT_STARTUP,								/* [E01]���W���[���N��					*/
	INSP_EVENT_INSPECTION_START,					/* [E02]�����J�n						*/
	INSP_EVENT_INSPECTION_REQ,						/* [E03]�����v��						*/
	INSP_EVENT_INSPECTION_RES,						/* [E04]��������						*/
	INSP_EVENT_INSPECTION_END,						/* [E05]�����I��						*/
	INSP_EVENT_INSPECTION_PERIOD_PROC,				/* [E06]�����������					*/
	INSP_EVENT_PRE_ACC_OFF,							/* [E07]ACC OFF�ڍs						*/
	INSP_EVENT_SHUTDOWN								/* [S08]���W���[���I��					*/
} E_INSP_EVENT;

#define INSP_GNSS_LAT					(356807870L)	/* �����n�_�̈ܓx:�k��35.680787		*/
#define INSP_GNSS_LON					(1397660930L)	/* �����n�_�̌o�x:���o139.766093	*/
#define INSP_GNSS_LAT_TOLERANCE			(2000L)			/* �ܓx�̋��e�덷:+-0.0002(��20m)	*/
#define INSP_GNSS_LON_TOLERANCE			(2000L)			/* �o�x�̋��e�덷:+-0.0002(��20m)	*/
#define INSP_GNSS_LAT_TOLERANCE_MIN		(INSP_GNSS_LAT - INSP_GNSS_LAT_TOLERANCE)	/* ���e�ܓx�̍ŏ��l	*/
#define INSP_GNSS_LAT_TOLERANCE_MAX		(INSP_GNSS_LAT + INSP_GNSS_LAT_TOLERANCE)	/* ���e�ܓx�̍ő�l	*/
#define INSP_GNSS_LON_TOLERANCE_MIN		(INSP_GNSS_LON - INSP_GNSS_LON_TOLERANCE)	/* ���e�o�x�̍ŏ��l	*/
#define INSP_GNSS_LON_TOLERANCE_MAX		(INSP_GNSS_LON + INSP_GNSS_LON_TOLERANCE)	/* ���e�o�x�̍ő�l	*/

#define INSP_BT_PAIRING_TIMEOUT			(10000)			/* BT�y�A�����O�����^�C���A�E�g(10000ms)	*/

#if defined _VDV_MARKX
/*MarkX*/
#define INSP_VDV_MODEL_INFO  (0x03)		/*�@����*/
#define INSP_VDV_FIRM_INFO	 (0x10)		/*�t�@�[���E�F�A���*/
#elif defined _VDV_RCF
/*RCF*/
#define INSP_VDV_MODEL_INFO  (0x04)		/*�@����*/
#define INSP_VDV_FIRM_INFO	 (0x10)		/*�t�@�[���E�F�A���*/
#elif defined _VDV_SUPRA
/*SUPRA*/
#define INSP_VDV_MODEL_INFO  (0x05)		/*�@����*/
#define INSP_VDV_FIRM_INFO	 (0x10)		/*�t�@�[���E�F�A���*/
#elif defined _VDV_LC
/*LC*/
#define INSP_VDV_MODEL_INFO  (0x06)		/*�@����*/
#define INSP_VDV_FIRM_INFO	 (0x10)		/*�t�@�[���E�F�A���*/
#elif defined _VDV_YARIS
/*YARIS*/
#define INSP_VDV_MODEL_INFO  (0x07)		/*�@����*/
#define INSP_VDV_FIRM_INFO	 (0x10)		/*�t�@�[���E�F�A���*/
#elif defined _VDV_86B
/*86B*/
#define INSP_VDV_MODEL_INFO  (0xA1)		/*�@����*/
#define INSP_VDV_FIRM_INFO	 (0x10)		/*�t�@�[���E�F�A���*/
#else
/*���̑��iVitz,86�܂ށj�Ԏ�*/
#define INSP_VDV_MODEL_INFO  (0x00)		/*�@����*/
#define INSP_VDV_FIRM_INFO	 (0x00)		/*�t�@�[���E�F�A���*/
#endif

/* �f�o�b�O�p��` */
#define INSP_LOCAL	static

/********************************************************/
/*	�\���̒�`											*/
/********************************************************/
/* ������ʁ|�R�}���h���	*/
typedef struct t_insp_type_cmd_info {
	E_INSP_INSPECTION_TYPE		type;
	UI_8						cmd;
} T_INSP_TYPE_CMD_INFO;

/* �������	*/
typedef struct t_insp_inspection_info {
	E_INSP_INSPECTION_TYPE		type;				/* �������								*/
	T_INSP_INSPECTION_PARAM		param;				/* �p�����^								*/
	E_INSP_INSPECTION_RESULT	result;				/* ��������								*/
} T_INSP_INSPECTION_INFO;

/********************************************************/
/*	�����萔�e�[�u��									*/
/********************************************************/
/* ������ʁ|�R�}���h���e�[�u�� */
INSP_LOCAL const T_INSP_TYPE_CMD_INFO C_insp_type_cmd_table[INSP_INSPECTION_TYPE_NUM] = {
	/*	�������,							�R�}���h				*/
	{	INSP_INSPECTION_TYPE_START,			CAN_INSPECTION_CMD_START				},	/* �����J�n								*/
	{	INSP_INSPECTION_TYPE_STOP,			CAN_INSPECTION_CMD_STOP					},	/* ������~								*/
	{	INSP_INSPECTION_TYPE_SERIAL_NO_WR,	CAN_INSPECTION_CMD_SERIAL_NO_WR			},	/* �V���A��No��������					*/
	{	INSP_INSPECTION_TYPE_SERIAL_NO_RD,	CAN_INSPECTION_CMD_SERIAL_NO_RD			},	/* �V���A��No�ǂݏo��					*/
	{	INSP_INSPECTION_TYPE_BT_PAIRING,	CAN_INSPECTION_CMD_BT_PAIRING			},	/* Bluetooth�y�A�����O					*/
	{	INSP_INSPECTION_TYPE_BT_CONNECT,	CAN_INSPECTION_CMD_BT_CONNECT			},	/* Bluetooth�ʐM						*/
	{	INSP_INSPECTION_TYPE_SD,			CAN_INSPECTION_CMD_SD					},	/* SD�J�[�h								*/
	{	INSP_INSPECTION_TYPE_CAN0,			CAN_INSPECTION_CMD_CAN0					},	/* CAN0									*/
	{	INSP_INSPECTION_TYPE_CAN1,			CAN_INSPECTION_CMD_CAN1					},	/* CAN1									*/
	{	INSP_INSPECTION_TYPE_UART,			CAN_INSPECTION_CMD_UART					},	/* UART									*/
	{	INSP_INSPECTION_TYPE_SW,			CAN_INSPECTION_CMD_SW					},	/* SW									*/
	{	INSP_INSPECTION_TYPE_GNSS,			CAN_INSPECTION_CMD_GNSS					},	/* GNSS									*/
};

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
INSP_LOCAL E_INSP_STAT				S_insp_stat;					/* �������W���[��������		*/
INSP_LOCAL T_INSP_INSPECTION_INFO	S_insp_inspection_info;			/* ���݌������̏��				*/
INSP_LOCAL T_INSP_INSPECTION_INFO	S_insp_inspection_info_prev;	/* �O��̌������				*/
INSP_LOCAL E_INSP_LED_MODE			S_insp_led_mode;				/* LED���[�h					*/
INSP_LOCAL UI_32					S_insp_bt_pairing_time;			/* BT�y�A�����O�����J�n����		*/

/********************************************************/
/*	�����֐��錾										*/
/********************************************************/
/* �C�x���g����֐�	*/
INSP_LOCAL void insp_make_event( E_INSP_EVENT *event );

/* ��ԑJ�ڎ��֐� */
INSP_LOCAL void insp_state_on_enter( E_INSP_STAT stat );
INSP_LOCAL void insp_state_on_leave( E_INSP_STAT stat );

/* ��Ԋ֐�	*/
INSP_LOCAL E_INSP_STAT insp_stat_init( E_INSP_EVENT event );
INSP_LOCAL E_INSP_STAT insp_stat_wait_start( E_INSP_EVENT event );
INSP_LOCAL E_INSP_STAT insp_stat_wait_inspection_req( E_INSP_EVENT event );
INSP_LOCAL E_INSP_STAT insp_stat_inspection( E_INSP_EVENT event );
INSP_LOCAL E_INSP_STAT insp_stat_end( E_INSP_EVENT event );
INSP_LOCAL E_INSP_STAT insp_stat_shutdown( E_INSP_EVENT event );

/* ��Ԋ֐��e�[�u��	*/
typedef E_INSP_STAT (*PFN_INSP_STATE_PROC)( E_INSP_EVENT event );
INSP_LOCAL const PFN_INSP_STATE_PROC S_insp_state_proc[] = {
	insp_stat_init,							/*	�������					*/
	insp_stat_wait_start,					/*	�����J�n�҂�				*/
	insp_stat_wait_inspection_req,			/*	�����v���҂�				*/
	insp_stat_inspection,					/*	������						*/
	insp_stat_end,							/*	�����I��					*/
	insp_stat_shutdown,						/*	���W���[���I��				*/
};

/* �������֐�	*/
INSP_LOCAL void insp_init_param( void );
INSP_LOCAL void insp_init_inspection_info(T_INSP_INSPECTION_INFO *insp_info);

/* �����֐�	*/
INSP_LOCAL SI_8 insp_inspection_start(void);
INSP_LOCAL SI_8 insp_inspection_end(void);
INSP_LOCAL SI_8 insp_inspection_req(E_INSP_INSPECTION_TYPE type, const T_INSP_INSPECTION_PARAM *param);
INSP_LOCAL SI_8 insp_inspection_res(const T_INSP_INSPECTION_INFO *inspinfo);
INSP_LOCAL void insp_inspection_serial_no_read(void);
INSP_LOCAL void insp_inspection_bt_pairing(void);
INSP_LOCAL SI_8 insp_inspection_bt_connect(void);
INSP_LOCAL void insp_inspection_gnss(void);

/* �R�[���o�b�N�֐�	*/
INSP_LOCAL void insp_eeprom_setdata_cb(E_EEPROM_DATA_TYPE type, UI_16 data, SI_8 result);

/* �����R�}���h����M�֐�	*/
INSP_LOCAL SI_8 insp_rcv_inspection_reqcmd(T_INSP_INSPECTION_INFO *inspinfo);
INSP_LOCAL SI_8 insp_snd_inspection_rescmd(const T_INSP_INSPECTION_INFO *inspinfo);
INSP_LOCAL SI_8 insp_type2cmd(E_INSP_INSPECTION_TYPE type, UI_8 *cmd);
INSP_LOCAL SI_8 insp_cmd2type(UI_8 cmd, E_INSP_INSPECTION_TYPE *type);

INSP_LOCAL void insp_upd_led_mode(void);

/****************************************************************************/
/*	�O���֐�																*/
/****************************************************************************/
/****************************************************************************/
/* �֐�����		: Insp_control_Init											*/
/* �@�\�̐���	: �������[�h���䏉����										*/
/* ����			:	void													*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
void Insp_control_Init(void)
{
	/* ���W���[���Ŏg�p����ϐ������� */
	insp_init_param();

	/* ��������LED���[�h:�ʏ�_��	*/
	S_insp_led_mode = INSP_LED_MODE_NOMAL;

	/* ������Ԑݒ�	*/
	S_insp_stat = INSP_STAT_INIT;

	return;
}

/****************************************************************************/
/* �֐�����		: Insp_control_Main											*/
/* �@�\�̐���	: �������[�h���䃁�C��										*/
/* ����			:	void													*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
void Insp_control_Main(void)
{
	E_INSP_EVENT	event;
	E_INSP_STAT		nextstat;
	
	/* �C�x���g�쐬 */
	insp_make_event( &event );

	/* ��Ԋ֐��R�[�� */
	if (event != INSP_EVENT_NONE) {
		nextstat = S_insp_state_proc[S_insp_stat]( event );
		if (S_insp_stat != nextstat) {			/* ��ԑJ�ڂ���		*/
			OUTLOG(OutputLog("[Insp]stat:%d->%d\n", S_insp_stat, nextstat);)
			insp_state_on_leave(S_insp_stat);
			S_insp_stat = nextstat;
			insp_state_on_enter(nextstat);
		}
	}

	/* LED���[�h�X�V	*/
	insp_upd_led_mode();

	return;
}

/****************************************************************************/
/* �֐�����		:	Insp_notify_inspection_result							*/
/* �@�\�̐���	:	�������ʒʒm											*/
/* ����			:	(I/ )type			�������							*/
/* 				:	(I/ )param			�p�����^							*/
/* 				:	(I/ )result			��������							*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
void Insp_notify_inspection_result(E_INSP_INSPECTION_TYPE type, const T_INSP_INSPECTION_PARAM *param, E_INSP_INSPECTION_RESULT result)
{

	/* �������ʂ�ݒ� */
	if (S_insp_inspection_info.type == INSP_INSPECTION_TYPE_INVLID) {
		/* �������ł͂Ȃ�	*/
		OUTLOG(OutputLog("[Insp]notify res insptype invalid!! type:%u restype:%u result:%u\n", S_insp_inspection_info.type, type, result);)
		return;
	}

	if (S_insp_inspection_info.type != type) {
		/* �������Ƃ͈قȂ�ʒm	*/
		OUTLOG(OutputLog("[Insp]notify res insptype diff!! type:%u restype:%u result:%u\n", S_insp_inspection_info.type, type, result);)
		return;
	}

	/* �p�����^���R�}���h���Ɋi�[ */
	if (param != NULL) {
		S_insp_inspection_info.param = *param;
	}

	/* ���ʂ��R�}���h���Ɋi�[ */
	if (result == INSP_INSPECTION_RESULT_OK) {
		S_insp_inspection_info.result = INSP_INSPECTION_RESULT_OK;
	}
	else {
		S_insp_inspection_info.result = INSP_INSPECTION_RESULT_NG;
	}

	OUTLOG(OutputLog("[Insp]notify res type:%u res:%u para:%02X %02X %08X\n",
						S_insp_inspection_info.type, S_insp_inspection_info.result,
						S_insp_inspection_info.param.data1, S_insp_inspection_info.param.data2, S_insp_inspection_info.param.data3);)

	return;

}

/****************************************************************************/
/* �֐�����		:	Insp_get_led_mode										*/
/* �@�\�̐���	:	����LED���[�h�擾										*/
/* ����			:	void													*/
/* �߂�l		:	LED���[�h(E_INSP_LED_MODE_xxx)							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
E_INSP_LED_MODE Insp_get_led_mode(void)
{
	return S_insp_led_mode;
}

/****************************************************************************/
/*	�����֐�																*/
/****************************************************************************/
/****************************************************************************/
/* �֐�����		:	insp_make_event											*/
/* �@�\�̐���	:	�C�x���g�쐬											*/
/* ����				( /O)event			�C�x���g							*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL void insp_make_event( E_INSP_EVENT *event )
{

	E_SYS_CTRL_STATUS		sysStat;
	T_INSP_INSPECTION_INFO	inspinfo;
	SI_8					ret;

	if (event == NULL) {
		return;
	}

	*event = INSP_EVENT_NONE;

	sysStat = System_control_GetStatus();

	switch ( sysStat ) {
	case SYS_OPERATION_INSPECTION:				/* �V�X�e��������:�������[�h	----------------*/
		if ((S_insp_stat == INSP_STAT_INIT) ||						/* �������W���[��������:�������				*/
			(S_insp_stat == INSP_STAT_INSPECTION_END)) {			/* �������W���[��������:�����I��				*/
			/* ���W���[���N���C�x���g	*/
			*event = INSP_EVENT_STARTUP;
		}
		else if (S_insp_stat == INSP_STAT_WAIT_INSPECTION_START) {	/* �������W���[��������:�����J�n�҂�			*/
			/* �����v���R�}���h��M	*/
			ret = insp_rcv_inspection_reqcmd( &inspinfo );
			if ((ret == N_OK) && (inspinfo.type == INSP_INSPECTION_TYPE_START)) {
				/* �����J�n�C�x���g */
				*event = INSP_EVENT_INSPECTION_START;
			}
		}
		else if (S_insp_stat == INSP_STAT_WAIT_INSPECTION_REQ) {	/* �������W���[��������:�����v���҂�			*/
			/* �����v���R�}���h��M	*/
			ret = insp_rcv_inspection_reqcmd( &inspinfo );
			if (ret == N_OK) {
				if (inspinfo.type == INSP_INSPECTION_TYPE_STOP) {
					/* �����I���C�x���g */
					*event = INSP_EVENT_INSPECTION_END;
				}
				else if (inspinfo.type == INSP_INSPECTION_TYPE_START) {
					/* �����J�n�͎󂯕t���Ȃ�	*/
					/* nop */
				}
				else {
					/* �����v������ݒ� */
					S_insp_inspection_info.type		= inspinfo.type;
					S_insp_inspection_info.param	= inspinfo.param;
					S_insp_inspection_info.result	= INSP_INSPECTION_RESULT_INVALID;

					/* �����v���C�x���g */
					*event = INSP_EVENT_INSPECTION_REQ;
				}
			}
		}
		else if (S_insp_stat == INSP_STAT_INSPECTION) {				/* �������W���[��������:������				*/
			/* �������ʂ��`�F�b�N	*/
			if (S_insp_inspection_info.result != INSP_INSPECTION_RESULT_INVALID) {	/* �������ʂ��i�[����Ă���	*/
				/* ���������C�x���g */
				*event = INSP_EVENT_INSPECTION_RES;
			}
			else {
				/* ������������C�x���g */
				*event = INSP_EVENT_INSPECTION_PERIOD_PROC;
			}
		}
		else {
			/* nop */
		}
		break;

	case SYS_OPERATION_PRE_ACC_OFF:				/* �V�X�e��������:ACC OFF�ڍs	----------------*/
		if (S_insp_stat != INSP_STAT_INSPECTION_END) {				/* �������W���[��������:�����I���ȊO					*/
			/* ACC OFF�ڍs�C�x���g */
			*event = INSP_EVENT_PRE_ACC_OFF;
		}
		break;

	case SYS_OPERATION_ACC_OFF:					/* �V�X�e��������:ACC OFF	----------------*/
		if (S_insp_stat != INSP_STAT_SHUTDOWN) {					/* �������W���[��������:���W���[���I���ȊO			*/
			/* ���W���[���I���C�x���g */
			*event = INSP_EVENT_SHUTDOWN;
		}
		break;

	default:
		break;
	}

	return;

}

/****************************************************************************/
/* �֐�����		:	insp_state_on_enter										*/
/* �@�\�̐���	:	��Ԃɓ���Ƃ��̏���									*/
/* ����				(I/ )stat	:���ꂩ�������							*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL void insp_state_on_enter( E_INSP_STAT stat )
{

	switch (stat) {
	case INSP_STAT_INIT:					/*	�������				*/
		insp_init_inspection_info(&S_insp_inspection_info);
		insp_init_inspection_info(&S_insp_inspection_info_prev);
		break;

	case INSP_STAT_WAIT_INSPECTION_START:	/*	�����J�n�҂�			*/
	case INSP_STAT_WAIT_INSPECTION_REQ:		/*	�����v���҂�			*/
		insp_init_inspection_info(&S_insp_inspection_info);
		break;

	case INSP_STAT_INSPECTION_END:			/*	�����I��				*/
		insp_init_inspection_info(&S_insp_inspection_info);
		insp_init_inspection_info(&S_insp_inspection_info_prev);

		/* �V�X�e������Ɍ���������ʒm	*/
		(void)System_control_NotifyEvent(SYS_EVENT_END_INSPECTION, SYS_EVENT_PARAM_NO);
		break;

	default:
		break;
	}

	return;

}

/****************************************************************************/
/* �֐�����		:	insp_state_on_leave										*/
/* �@�\�̐���	:	��Ԃ���o��Ƃ��̏���									*/
/* ����			:	(I/ )stat	:���ꂩ��o����							*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL void insp_state_on_leave( E_INSP_STAT stat )
{

	if (stat == INSP_STAT_INSPECTION) {				/* ������					*/
		/* ����̌�������O��̌������ɃR�s�[	*/
		S_insp_inspection_info_prev = S_insp_inspection_info;
	}

	return;

}

/****************************************************************************/
/* �֐�����		:	insp_stat_init											*/
/* �@�\�̐���	:	��Ԋ֐�:INSP_STAT_INIT									*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	INSP_STAT_xxx	�J�ڐ���								*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL E_INSP_STAT insp_stat_init( E_INSP_EVENT event )
{

	E_INSP_STAT	nextStat = INSP_STAT_INIT;
	UI_8			variation;

	if (event == INSP_EVENT_STARTUP) {					/* ���W���[���N��				*/

		/* �o���G�[�V���������擾����	*/
		if (Sd_control_GetInspectionVariation(&variation) == N_OK) {

			OUTLOG(OutputLog("[Insp]variation write :%02X\n", variation);)

			/* �o���G�[�V��������EEPROM�ɏ�������	*/
			if (Eeprom_control_SetData(EEPROM_DATA_VARIATION, (UI_16)variation, NULL) == EEPROM_CONTROL_RESULT_OK) {
				/* �J�ڐ���:�����J�n�҂�	*/
				nextStat = INSP_STAT_WAIT_INSPECTION_START;
			}
			else {
				/* �J�ڐ���:�����I��	*/
				nextStat = INSP_STAT_INSPECTION_END;
			}
		}
		else {
			/* �J�ڐ���:�����I��	*/
			nextStat = INSP_STAT_INSPECTION_END;
		}
	}
	else if (event == INSP_EVENT_PRE_ACC_OFF) {			/* ACC OFF�ڍs				*/
		/* �J�ڐ���:�����I��	*/
		nextStat = INSP_STAT_INSPECTION_END;
	}
	else {
		/* nop */
	}

	return nextStat;

}

/****************************************************************************/
/* �֐�����		:	insp_stat_wait_start									*/
/* �@�\�̐���	:	��Ԋ֐�:INSP_STAT_WAIT_INSPECTION_START				*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	INSP_STAT_xxx	�J�ڐ���								*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL E_INSP_STAT insp_stat_wait_start( E_INSP_EVENT event )
{

	E_INSP_STAT	nextStat = INSP_STAT_WAIT_INSPECTION_START;

	if (event == INSP_EVENT_INSPECTION_START) {			/* �����J�n						*/
		/* �����J�n���� */
		(void)insp_inspection_start();

		/* �J�ڐ���:�����v���҂�	*/
		nextStat = INSP_STAT_WAIT_INSPECTION_REQ;
	}
	else if (event == INSP_EVENT_PRE_ACC_OFF) {			/* ACC OFF�ڍs					*/
		/* �J�ڐ���:�����I��	*/
		nextStat = INSP_STAT_INSPECTION_END;
	}
	else {
		/* nop */
	}

	return nextStat;

}

/****************************************************************************/
/* �֐�����		:	insp_stat_wait_inspection_req							*/
/* �@�\�̐���	:	��Ԋ֐�:INSP_STAT_WAIT_INSPECTION_REQ					*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	INSP_STAT_xxx	�J�ڐ���								*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL E_INSP_STAT insp_stat_wait_inspection_req( E_INSP_EVENT event )
{

	E_INSP_STAT	nextStat = INSP_STAT_WAIT_INSPECTION_REQ;
	SI_8			ret;

	if (event == INSP_EVENT_INSPECTION_REQ) {				/* �����v��						*/
		/* �����v�� */
		ret = insp_inspection_req(S_insp_inspection_info.type, &S_insp_inspection_info.param);
		if (ret != N_OK) {
			/* �����v�����󂯕t�����Ȃ��ꍇ�̓G���[��ʒm */
			Insp_notify_inspection_result(S_insp_inspection_info.type, NULL, INSP_INSPECTION_RESULT_NG);
		}

		/* �J�ڐ���:������	*/
		nextStat = INSP_STAT_INSPECTION;
	}
	else if (event == INSP_EVENT_INSPECTION_END) {			/* �����I��						*/
		/* �����I������ */
		(void)insp_inspection_end();

		/* �J�ڐ���:�����I��	*/
		nextStat = INSP_STAT_INSPECTION_END;
	}
	else if (event == INSP_EVENT_PRE_ACC_OFF) {				/* ACC OFF�ڍs					*/
		/* �J�ڐ���:�����I��	*/
		nextStat = INSP_STAT_INSPECTION_END;
	}
	else {
		/* nop */
	}

	return nextStat;

}

/****************************************************************************/
/* �֐�����		:	insp_stat_inspection									*/
/* �@�\�̐���	:	��Ԋ֐�:INSP_STAT_INSPECTION							*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	INSP_STAT_xxx	�J�ڐ���								*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL E_INSP_STAT insp_stat_inspection( E_INSP_EVENT event )
{

	E_INSP_STAT			nextStat = INSP_STAT_INSPECTION;

	if (event == INSP_EVENT_INSPECTION_RES) {				/* ��������						*/
		/* �������������� */
		(void)insp_inspection_res(&S_insp_inspection_info);

		/* �J�ڐ���:�����v���҂�	*/
		nextStat = INSP_STAT_WAIT_INSPECTION_REQ;
	}
	else if (event == INSP_EVENT_INSPECTION_PERIOD_PROC) {	/* �����������					*/
		if (S_insp_inspection_info.type == INSP_INSPECTION_TYPE_BT_PAIRING) {		/* ���s���̌���:BT�y�A�����O	*/
			/* BT�y�A�����O����	*/
			insp_inspection_bt_pairing();
		}
		else if (S_insp_inspection_info.type == INSP_INSPECTION_TYPE_GNSS) {		/* ���s���̌���:GNSS			*/
			/* GNSS����	*/
			insp_inspection_gnss();
		}
		else {
			/* nop */
		}
	}
	else if (event == INSP_EVENT_PRE_ACC_OFF) {				/* ACC OFF�ڍs					*/
		/* �J�ڐ���:�����I��	*/
		nextStat = INSP_STAT_INSPECTION_END;
	}
	else {
		/* nop */
	}

	return nextStat;

}

/****************************************************************************/
/* �֐�����		:	insp_stat_end											*/
/* �@�\�̐���	:	��Ԋ֐�:INSP_STAT_INSPECTION_END						*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	INSP_STAT_xxx	�J�ڐ���								*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL E_INSP_STAT insp_stat_end( E_INSP_EVENT event )
{

	E_INSP_STAT	nextStat = INSP_STAT_INSPECTION_END;

	if (event == INSP_EVENT_STARTUP) {					/* ���W���[���N��				*/
		/* �J�ڐ���:�������	*/
		nextStat = INSP_STAT_INIT;
	}
	else if (event == INSP_EVENT_SHUTDOWN) {			/* ���W���[���I��				*/
		/* �J�ڐ���:���W���[���I��	*/
		nextStat = INSP_STAT_SHUTDOWN;
	}
	else {
		/* nop */
	}

	return nextStat;

}

/****************************************************************************/
/* �֐�����		:	insp_stat_shutdown										*/
/* �@�\�̐���	:	��Ԋ֐�:INSP_STAT_SHUTDOWN								*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	INSP_STAT_xxx	�J�ڐ���								*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL E_INSP_STAT insp_stat_shutdown( E_INSP_EVENT event )
{

	E_INSP_STAT	nextStat = INSP_STAT_SHUTDOWN;

	/* NOP */

	return nextStat;

}

/****************************************************************************/
/* �֐�����		:	insp_init_param											*/
/* �@�\�̐���	:	�p�����^������											*/
/* ����			:	void													*/
/* �߂�l		: 	void													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:	���W���[����Ԃ͍X�V���Ȃ�								*/
/****************************************************************************/
INSP_LOCAL void insp_init_param( void )
{

	/* ���W���[�����ϐ�������	*/
	S_insp_led_mode = INSP_LED_MODE_OFF;
	insp_init_inspection_info(&S_insp_inspection_info);
	insp_init_inspection_info(&S_insp_inspection_info_prev);
	S_insp_bt_pairing_time = 0;

	return;

}

/****************************************************************************/
/* �֐�����		:	insp_init_inspection_info								*/
/* �@�\�̐���	:	������񏉊���											*/
/* ����			:	( /O)insp_info		�������							*/
/* �߂�l		: 	void													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL void insp_init_inspection_info(T_INSP_INSPECTION_INFO *insp_info)
{

	if (insp_info != NULL) {
		(void)memset(insp_info, 0, sizeof(T_INSP_INSPECTION_INFO));
		insp_info->type		= INSP_INSPECTION_TYPE_INVLID;
		insp_info->result	= INSP_INSPECTION_RESULT_INVALID;
	}

	return;

}

/****************************************************************************/
/* �֐�����		:	insp_inspection_start									*/
/* �@�\�̐���	:	�����J�n����											*/
/* ����			:	void													*/
/* �߂�l		:	����:N_OK												*/
/*				:	�ُ�:N_NG												*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_inspection_start(void)
{

	T_INSP_INSPECTION_INFO	inspinfo;
	SI_8	ret;

	/* �����J�n�����R�}���h���M	*/
	(void)memset(&inspinfo, 0, sizeof(inspinfo));
	inspinfo.type = INSP_INSPECTION_TYPE_START;
	inspinfo.result = INSP_INSPECTION_RESULT_OK;
	inspinfo.param.data1 = INSP_VDV_MODEL_INFO;
	inspinfo.param.data2 = INSP_VDV_FIRM_INFO;
	ret = insp_snd_inspection_rescmd(&inspinfo);

	return ret;

}

/****************************************************************************/
/* �֐�����		:	insp_inspection_end										*/
/* �@�\�̐���	:	�����I������											*/
/* ����			:	void													*/
/* �߂�l		:	����:N_OK												*/
/*				:	�ُ�:N_NG												*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_inspection_end(void)
{

	T_INSP_INSPECTION_INFO	inspinfo;
	SI_8	ret;

	/* ������~�����R�}���h���M	*/
	(void)memset(&inspinfo, 0, sizeof(inspinfo));
	inspinfo.type = INSP_INSPECTION_TYPE_STOP;
	inspinfo.result = INSP_INSPECTION_RESULT_OK;
	ret = insp_snd_inspection_rescmd(&inspinfo);

	return ret;

}

/****************************************************************************/
/* �֐�����		:	insp_inspection_req										*/
/* �@�\�̐���	:	�����v������											*/
/* ����			:	(I/ )type		�������								*/
/* 				:	(I/ )param			�p�����^							*/
/* �߂�l		:	����:N_OK												*/
/*				:	�ُ�:N_NG												*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:	�߂�l������(N_OK)�̏ꍇ�A�����v�����󂯕t�����		*/
/*					�������ʂ�S_insp_inspection_info�ɐݒ肳���			*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_inspection_req(E_INSP_INSPECTION_TYPE type, const T_INSP_INSPECTION_PARAM *param)
{

	UI_16	serial_no;
	SI_8	ret = N_OK;


	switch(type) {
	case INSP_INSPECTION_TYPE_SERIAL_NO_WR:
		/* �V���A��No�������݌���	*/
		if (param != NULL) {
			serial_no = (((UI_16)param->data1) << 8) | ((UI_16)param->data2);
			if (Eeprom_control_SetData(EEPROM_DATA_SERIAL_NO, serial_no, insp_eeprom_setdata_cb) != EEPROM_CONTROL_RESULT_OK) {
				ret = N_NG;
			}
		}
		else {
			ret = N_NG;
		}
		break;

	case INSP_INSPECTION_TYPE_SERIAL_NO_RD:
		/* �V���A��No�ǂݏo������	*/
		insp_inspection_serial_no_read();
		break;

	case INSP_INSPECTION_TYPE_BT_PAIRING:
		/* Bluetooth�y�A�����O����	*/
		(void)Bt_control_reqPairingInspection();
		S_insp_bt_pairing_time = Timer_ctrl_GetTime1ms();
		break;

	case INSP_INSPECTION_TYPE_BT_CONNECT:
		/* Bluetooth�ʐM����	*/
		ret = insp_inspection_bt_connect();
		break;

	case INSP_INSPECTION_TYPE_SD:
		/* SD�J�[�h����	*/
		if (param != NULL) {
			serial_no = (((UI_16)param->data1) << 8) | ((UI_16)param->data2);
			ret = Sd_control_ReqInspection(serial_no);
		}
		else {
			ret = N_NG;
		}
		break;

	case INSP_INSPECTION_TYPE_CAN0:
		/* CAN0����	*/
		ret = Can_control_ReqInspection(CAN_INSPECTION_CAN_CH0);
		break;

	case INSP_INSPECTION_TYPE_CAN1:
		/* CAN1����	*/
		ret = Can_control_ReqInspection(CAN_INSPECTION_CAN_CH1);
		break;

	case INSP_INSPECTION_TYPE_UART:
		/* UART����	*/
		ret = Advance_control_ReqInspection();
		break;

	case INSP_INSPECTION_TYPE_SW:
		/* SW����	*/
		ret = Sw_control_ReqInspection();
		break;

	case INSP_INSPECTION_TYPE_GNSS:
		/* GNSS����	*/
		insp_inspection_gnss();
		break;

	default:
		ret = N_NG;
		break;
	}

	if (ret != N_OK) {
		OUTLOG(OutputLog("[Insp]insp req err!! type:%u\n", type);)
	}

	return ret;

}

/****************************************************************************/
/* �֐�����		:	insp_inspection_res										*/
/* �@�\�̐���	:	������������											*/
/* ����			:	(I/ )inspinfo		�������							*/
/* �߂�l		:	����:N_OK												*/
/*				:	�ُ�:N_NG												*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_inspection_res(const T_INSP_INSPECTION_INFO *inspinfo)
{

	SI_8	ret;

	if (inspinfo == NULL) {
		return N_NG;
	}

	/* ���������R�}���h���M */
	ret = insp_snd_inspection_rescmd(inspinfo);
	if (ret != N_OK) {
		return N_NG;
	}

	return N_OK;

}

/****************************************************************************/
/* �֐�����		:	insp_inspection_serial_no_read							*/
/* �@�\�̐���	:	�V���A��No�ǂݏo������									*/
/* ����			:	void													*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL void insp_inspection_serial_no_read(void)
{

	E_EEPROM_CONTROL_RESULT		eep_res;
	E_INSP_INSPECTION_RESULT	insp_res;
	T_INSP_INSPECTION_PARAM		param;
	UI_16	serial_no;

	(void)memset(&param, 0, sizeof(param));

	/* EEPROM����V���A��No��ǂ݂���	*/
	eep_res = Eeprom_control_GetData(EEPROM_DATA_SERIAL_NO, &serial_no);
	if (eep_res == EEPROM_CONTROL_RESULT_OK) {
		param.data1 = (UI_8)(serial_no >> 8);
		param.data2 = (UI_8)(serial_no & 0x00FF);
		insp_res = INSP_INSPECTION_RESULT_OK;
	}
	else {
		insp_res = INSP_INSPECTION_RESULT_NG;
	}

	/* �ǂݏo�����ʂ�ʒm	*/
	Insp_notify_inspection_result(INSP_INSPECTION_TYPE_SERIAL_NO_RD, &param, insp_res);

	return;

}

/****************************************************************************/
/* �֐�����		:	insp_inspection_bt_pairing								*/
/* �@�\�̐���	:	Bluetooth�y�A�����O����									*/
/* ����			:	void													*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL void insp_inspection_bt_pairing(void)
{

	E_BT_CONTROL_STAT			btStat;
	E_BT_COMM_MDL_STATE			btMdlStat;

	/* BT��Ԏ擾	*/
	btStat = Bt_control_GetStatus();
	if (btStat == BT_CONTROL_STAT_PAIRING) {					/* BT���:�y�A�����O	*/

		/* BT�~�h����Ԃ��擾	*/
		btMdlStat = Bt_control_getMdlState();
		if (btMdlStat == BT_COMM_MDL_STATE_PAIRING_MODE) {		/* BT�~�h�����:�y�A�����O	*/


				/* BT�y�A�����O�������� �����ʒm	*/
				Insp_notify_inspection_result(INSP_INSPECTION_TYPE_BT_PAIRING, NULL, INSP_INSPECTION_RESULT_OK);
				return;
		}
	}

	if (Timer_ctrl_TimeCompare1ms(S_insp_bt_pairing_time, INSP_BT_PAIRING_TIMEOUT) == N_OK) {	/* BT�y�A�����O�����^�C���A�E�g	*/

		/* BT�y�A�����O�������� �ُ��ʒm	*/
		Insp_notify_inspection_result(INSP_INSPECTION_TYPE_BT_PAIRING, NULL, INSP_INSPECTION_RESULT_NG);
	}

	return;

}

/****************************************************************************/
/* �֐�����		:	insp_inspection_bt_connect								*/
/* �@�\�̐���	:	Bluetooth�ʐM����										*/
/* ����			:	void													*/
/* �߂�l		:	����:N_OK												*/
/*				:	�ُ�:N_NG												*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_inspection_bt_connect(void)
{

	E_BT_CONTROL_STAT			btStat;

	/* Bluetooth��Ԏ擾	*/
	btStat = Bt_control_GetStatus();
	if ((btStat == BT_CONTROL_STAT_COMM_ILDE) || (btStat == BT_CONTROL_STAT_COMM_SENDING)) {
		/* BT�ڑ��ς�	*/
		/* BT�ʐM�������� �����ʒm	*/
		Insp_notify_inspection_result(INSP_INSPECTION_TYPE_BT_CONNECT, NULL, INSP_INSPECTION_RESULT_OK);

		/* BT�ʐM�J�n	*/
		(void)Bt_control_reqCommunicationInspection();
	}
	else {
		/* BT�ʐM�������� �ُ��ʒm	*/
		Insp_notify_inspection_result(INSP_INSPECTION_TYPE_BT_CONNECT, NULL, INSP_INSPECTION_RESULT_NG);
	}

	return N_OK;

}

/****************************************************************************/
/* �֐�����		:	insp_inspection_gnss									*/
/* �@�\�̐���	:	GNSS����												*/
/* ����			:	void													*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL void insp_inspection_gnss(void)
{

	UI_8	gps_fix_type;
	E_N_RC	gnss_res;
	T_Gnss_ctrl_nav_pvt_info 	*hnr_info;


	E_INSP_INSPECTION_RESULT	insp_res;

	/* GPS Fix Type�l���擾	*/
	gnss_res = Gnss_control_GetGpsFixType(&gps_fix_type);
	if ((gnss_res == N_RC_OK) &&
		((gps_fix_type == GNSS_GSPFIX_3DFIX) || (gps_fix_type == GNSS_GSPFIX_GPS_AND_DEADRECKONING))) {
		/* 3D���ʒ��ɂȂ����̂Ō�������	*/

		/* GNSS NAV-HNR���J�����g�l���擾	*/
		hnr_info = Gnss_cntrol_GetNavPvtInfo();
		if (hnr_info != NULL) {
			/* �ܓx�o�x���`�F�b�N����	*/
			if ((INSP_GNSS_LAT_TOLERANCE_MIN <= hnr_info->lat) && (hnr_info->lat <= INSP_GNSS_LAT_TOLERANCE_MAX) &&
				(INSP_GNSS_LON_TOLERANCE_MIN <= hnr_info->lon) && (hnr_info->lon <= INSP_GNSS_LON_TOLERANCE_MAX)) {
				/* �ܓx�o�x�������n�_�̋��e�͈͓��ɂ���	*/
				insp_res = INSP_INSPECTION_RESULT_OK;
			}
			else {
				insp_res = INSP_INSPECTION_RESULT_NG;
			}

			/* GNSS�������ʂ�ʒm����	*/
			Insp_notify_inspection_result(INSP_INSPECTION_TYPE_GNSS, NULL, insp_res);
		}
	}

	return;

}

/****************************************************************************/
/* �֐�����		:	insp_eeprom_setdata_cb									*/
/* �@�\�̐���	:	eeprom�������݃R�[���o�b�N								*/
/* ����			: (I/ )type	�f�[�^���  E_EEPROM_DATA_TYPE					*/
/* 				: (I/ )data	�ݒ�f�[�^  									*/
/*				: (I/ )result	�������݌���(N_OK:����, N_NG:�ُ�)			*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/02/09												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL void insp_eeprom_setdata_cb(E_EEPROM_DATA_TYPE type, UI_16 data, SI_8 result)
{

	E_INSP_INSPECTION_RESULT	insp_res;

	if (S_insp_inspection_info.type == INSP_INSPECTION_TYPE_SERIAL_NO_WR) {		/* �������:�V���A��No��������	*/
		if (type == EEPROM_DATA_SERIAL_NO) {			/* EEPROM�f�[�^���:�V���A��No		*/

			if (result == N_OK) {
				insp_res = INSP_INSPECTION_RESULT_OK;
			}
			else {
				insp_res = INSP_INSPECTION_RESULT_NG;
			}

			/* �V���A��No�������݌��ʂ�ʒm	*/
			Insp_notify_inspection_result(INSP_INSPECTION_TYPE_SERIAL_NO_WR, NULL, insp_res);
		}
	}

	return;

}

/****************************************************************************/
/* �֐�����		:	insp_rcv_inspection_reqcmd								*/
/* �@�\�̐���	:	�����v���R�}���h��M									*/
/* ����			:	( /O)inspinfo		�������							*/
/* �߂�l		:	N_OK		�����v������M����							*/
/* 				:	N_NG		�����v������M�܂��̓G���[					*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_rcv_inspection_reqcmd(T_INSP_INSPECTION_INFO *inspinfo)
{

	T_Can_inspection_cmd_info	cmdinfo;
	E_INSP_INSPECTION_TYPE		type;
	SI_8	ret;

	if (inspinfo == NULL) {
		return N_NG;
	}

	/* CAN���猟���R�}���h����M����	*/
	ret = Can_control_RcvInspectionCmd(&cmdinfo);
	if (ret != N_OK) {
		return N_NG;
	}

	OUTLOG(OutputLog("[Insp]rcv can cmd cmd:%02X res:%02X data:%02X %02X %08X\n",
						cmdinfo.cmd, cmdinfo.result, cmdinfo.data1, cmdinfo.data2, cmdinfo.data3);)

	/* CAN�����R�}���h��������ʂɕϊ� */
	ret = insp_cmd2type(cmdinfo.cmd, &type);
	if (ret != N_OK) {
		OUTLOG(OutputLog("[Insp]rcv can cmd invalid cmd:%02X\n", cmdinfo.cmd);)
		return N_NG;
	}

	/* �������ݒ� */
	inspinfo->type = type;
	inspinfo->result = INSP_INSPECTION_RESULT_INVALID;
	inspinfo->param.data1 = cmdinfo.data1;
	inspinfo->param.data2 = cmdinfo.data2;
	inspinfo->param.data3 = cmdinfo.data3;

	return N_OK;

}

/****************************************************************************/
/* �֐�����		:	insp_snd_inspection_rescmd								*/
/* �@�\�̐���	:	���������R�}���h���M									*/
/* ����			:	(I/ )inspinfo		�������							*/
/* �߂�l		:	����:N_OK												*/
/*				:	�ُ�:N_NG												*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_snd_inspection_rescmd(const T_INSP_INSPECTION_INFO *inspinfo)
{

	T_Can_inspection_cmd_info	cmdinfo;
	SI_8	ret;

	if (inspinfo == NULL) {
		return N_NG;
	}

	/* ������ʂ�CAN�����R�}���h�ɕϊ� */
	ret = insp_type2cmd(inspinfo->type, &cmdinfo.cmd);
	if (ret != N_OK) {
		return N_NG;
	}

	/* CAN�R�}���h�ݒ� */
	if (inspinfo->result == INSP_INSPECTION_RESULT_OK) {
		cmdinfo.result = CAN_INSPECTION_RESULT_OK;
	}
	else {
		cmdinfo.result = CAN_INSPECTION_RESULT_NG;
	}
	cmdinfo.data1 = inspinfo->param.data1;
	cmdinfo.data2 = inspinfo->param.data2;
	cmdinfo.data3 = inspinfo->param.data3;

	OUTLOG(OutputLog("[Insp]snd can cmd cmd:%02X res:%02X data:%02X %02X %08X\n",
						cmdinfo.cmd, cmdinfo.result, cmdinfo.data1, cmdinfo.data2, cmdinfo.data3);)

	/* CAN�Ō����R�}���h�𑗐M����	*/
	ret = Can_control_SndInspectionCmd(&cmdinfo);
	if (ret != N_OK) {
		return N_NG;
	}

	return N_OK;

}

/****************************************************************************/
/* �֐�����		:	insp_type2cmd											*/
/* �@�\�̐���	:	�������->�R�}���h�ϊ�									*/
/* ����			:	(I/ )type			�������							*/
/* 				: 	( /O)cmd			�R�}���h							*/
/* �߂�l		:	����:N_OK												*/
/*				:	�ُ�:N_NG												*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_type2cmd(E_INSP_INSPECTION_TYPE type, UI_8 *cmd)
{

	UI_8	i;

	if (cmd == NULL) {
		return N_NG;
	}

	for (i = 0; i < INSP_INSPECTION_TYPE_NUM; i++) {
		if (C_insp_type_cmd_table[i].type == type) {
			*cmd = C_insp_type_cmd_table[i].cmd;
			return N_OK;
		}
	}
	return N_NG;

}

/****************************************************************************/
/* �֐�����		:	insp_cmd2type											*/
/* �@�\�̐���	:	�R�}���h->������ʕϊ�									*/
/* ����			: 	(I/ )cmd			�R�}���h							*/
/* 				:	( /O)type			�������							*/
/* �߂�l		:	����:N_OK												*/
/*				:	�ُ�:N_NG												*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL SI_8 insp_cmd2type(UI_8 cmd, E_INSP_INSPECTION_TYPE *type)
{

	UI_8	i;

	if (type == NULL) {
		return N_NG;
	}

	for (i = 0; i < INSP_INSPECTION_TYPE_NUM; i++) {
		if (C_insp_type_cmd_table[i].cmd == cmd) {
			*type = C_insp_type_cmd_table[i].type;
			return N_OK;
		}
	}
	return N_NG;

}

/****************************************************************************/
/* �֐�����		:	insp_upd_led_mode										*/
/* �@�\�̐���	:	LED���[�h�X�V											*/
/* ����			:	void													*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
INSP_LOCAL void insp_upd_led_mode(void)
{

	E_INSP_INSPECTION_TYPE		type;
	E_INSP_INSPECTION_RESULT	result;
	E_BT_COMM_MDL_STATE			btMdlStat;

	switch (S_insp_stat) {
	case INSP_STAT_WAIT_INSPECTION_REQ:
	case INSP_STAT_INSPECTION:
		if (S_insp_inspection_info.type != INSP_INSPECTION_TYPE_INVLID) {			/* ���݌�����							*/
			type	= S_insp_inspection_info.type;
			result	= S_insp_inspection_info.result;
		}
		else if (S_insp_inspection_info_prev.type != INSP_INSPECTION_TYPE_INVLID) {	/* ���ݖ�����  �O��̌������ʂ���		*/
			type	= S_insp_inspection_info_prev.type;
			result	= S_insp_inspection_info_prev.result;
		}
		else {
			type	= INSP_INSPECTION_TYPE_INVLID;
			result	= INSP_INSPECTION_RESULT_INVALID;
		}

		if (type == INSP_INSPECTION_TYPE_BT_PAIRING) {		/* �������:Bluetooth�y�A�����O	*/
			/* BT�~�h����Ԃ��擾	*/
			btMdlStat = Bt_control_getMdlState();
			if (btMdlStat == BT_COMM_MDL_STATE_PAIRING_MODE) {	/* BT�~�h�����:�y�A�����O	*/
				/* ��������LED���[�h:Buletooth�y�A�����O	*/
				S_insp_led_mode = INSP_LED_MODE_BT_PAIRING;
			}
			else {
				/* ��������LED���[�h:�ʏ�_��	*/
				S_insp_led_mode = INSP_LED_MODE_NOMAL;
			}
		}
		else if (type == INSP_INSPECTION_TYPE_SW) {			/* �������:SW����					*/
			if (result == INSP_INSPECTION_RESULT_OK) {			/* SW��������I��	*/
				/* ��������LED���[�h:�ʏ�_��	*/
				S_insp_led_mode = INSP_LED_MODE_NOMAL;
			}
			else {												/* SW�������܂��ُ͈�I��	*/
				/* ��������LED���[�h:SW����	*/
				S_insp_led_mode = INSP_LED_MODE_SW_INSPECT;
			}
		}
		else {
			/* ��������LED���[�h:�ʏ�_��	*/
			S_insp_led_mode = INSP_LED_MODE_NOMAL;
		}
		break;

	case INSP_STAT_SHUTDOWN:
		/* ��������LED���[�h:����	*/
		S_insp_led_mode = INSP_LED_MODE_OFF;
		break;

	default:
		/* ��������LED���[�h:�ʏ�_��	*/
		S_insp_led_mode = INSP_LED_MODE_NOMAL;
		break;
	}

	return;

}

