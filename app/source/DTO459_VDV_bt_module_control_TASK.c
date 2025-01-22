/*--------------------------------------------------------------------------*/
/*	BT���W���[������														*/
/*--------------------------------------------------------------------------*/
/* �T�v																		*/
/* 				BT���W���[������											*/
/*																			*/
/*	�t�@�C���쐬���̃e�[�}:													*/
/*				ER330 �����A�_�v�^											*/
/*																			*/
/* �쐬			2010/09/24	����											*/
/* 																			*/
/* �C��			2012/03/23	���{											*/
/*				REQ-001�F�X�^���o�C���[�h����ԑJ�ڂ���폜					*/
/*				REQ-011�F�d��ON����UART�V�[�P���X�ύX						*/
/*				REQ-013�FBluetooth�ڑ��V�[�P���X�ύX						*/
/*				REQ-014�F�e�X�g���[�h�������쐬								*/
/*				REQ-015�FDUT���[�h�������쐬								*/
/*				REQ-016�F�F�؃��[�h�������쐬								*/
/* �C��			2020/08/25	(OFT)Kamakura									*/
/*				BLE����FANNA-B112����Ή�									*/
/*																			*/
/* ���l																		*/
/*				����														*/
/*																			*/
/*--------------------------------------------------------------------------*/
#include <string.h>														/* �W��ͯ�ް̧�� */
#include "System.h"
#include "Timer.h"
#include "timer_ctrl.h"
#include "SSFTSTD_Macro.h"
#include "DTO459_VDV_bt_module_control_TASK.h"							/* BTӼޭ�ِ���ͯ�ް̧�� */
#include "DTO459_VDV_bt_mod_drv.h"
#include "SHRVSTD_Port_Drv.h"
#include "DTO459_VDV_RringBuffer.h"
#include "dbglog.h"

#define	BTMDL_LOCAL	static
#define BT_SOFT_FLOW_CONTROL
/*--------------------------------------------------------------------------*/
/*	�����}�N��																*/
/*--------------------------------------------------------------------------*/
/**
 *	BT���W���[�����
 */
typedef enum e_bt_module_status {						/* BT���W���[����� 						add REQ-015, 016 */
	STATUS_BT_IDLE = 0,									/* �A�C�h����� */
	STATUS_BT_STARTUP,									/* �X�^�[�g�A�b�v��� */
	STATUS_BT_COMM_MODE,								/* �ڑ��\��� */
	STATUS_BT_PRE_PAIRING_MODE,							/* �y�A�����O�ڍs��� */
	STATUS_BT_PAIRING_MODE,								/* �y�A�����O��� */
	STATUS_BT_SHUTDOWN,									/* �I����������� */
	STATUS_BT_ERROR,									/* �ُ��� */
	STATUS_BT_NUM
} E_BT_MODULE_STATUS;

/**
 *	�R�}���hID
 */
typedef enum e_bmc_cmd {
	BMC_CMD_NONE = 0,									/**< �R�}���h�Ȃ�			*/
	BMC_CMD_STARTUP,									/**< BT���W���[���N��		*/
	BMC_CMD_SHUTDOWN,									/**< BT���W���[����~		*/
	BMC_CMD_COMM,										/**< �ʐM���[�h�J�n			*/
	BMC_CMD_PAIR,										/**< �y�A�����O���[�h�J�n	*/
	BMC_CMD_NUM											/**< �R�}���h��				*/
} E_BMC_CMD;

/**
 *	�y�A�����O�v���Z�X
 */
typedef enum e_bmc_pairing_proc {
	PAIRING_PROC_NONE = 0,								/**< �v���Z�X�Ȃ�			*/
	PAIRING_PROC_REQ_DISCOVERABLE,						/**< �����\�v��			*/
	PAIRING_PROC_DISCOVERABLE,							/**< �����\				*/
	PAIRING_PROC_WAIT_PAIRING,							/**< �y�A�����O�����҂�		*/
	PAIRING_PROC_PAIRING,								/**< �y�A�����O��			*/
	PAIRING_PROC_PAIRING_COMP							/**< �y�A�����O����			*/
} E_BMC_PAIRING_PROC;

/**
 *	�y�A�����O�����`�F�b�N���[�h
 */
typedef enum e_bmc_check_pairing_finished_mode {
	CHECK_PAIRING_FINISHED_MODE_NOT_CMD_READ_END = 0,	/**< �`�F�b�N���ʂ��������̏ꍇ�̓R�}���h�f�[�^��ǂݎ̂ĂȂ�	*/
	CHECK_PAIRING_FINISHED_MODE_CMD_READ_END			/**< �R�}���h�f�[�^��ǂݎ̂Ă�									*/
} E_BMC_CHECK_PAIRING_FINISHED_MODE;

#define		BT_PACKET_SZ						(46)					/* BT���W���[���ʐM�p�P�b�g�T�C�Y */
#define		BT_RX_BUFF_SZ						(512)					/* BT���W���[����M�f�[�^�o�b�t�@�T�C�Y */
#define		BT_STARTUP_INTERVAL					(199)					/* BT���W���[���N�����g���C�C���^�[�o��(2�b) */
#define		BT_AT_CMD_INTERVAL					(99)					/* BT���W���[��AT�R�}���h���M�C���^�[�o��(1�b) */
#define		BT_AT_CMD_RETRYCOUNT				(1)						/* BT���W���[��AT�R�}���h���M���g���C�� */
#define		BT_AT_SEQ_STAT_PROCESSING			(0)						/* AT�R�}���h�V�[�P���X�F������ */
#define		BT_AT_SEQ_STAT_COMPLETE				(1)						/* AT�R�}���h�V�[�P���X�F���� */
#define		BT_AT_SEQ_STAT_FAILED				(2)						/* AT�R�}���h�V�[�P���X�F���s�F�@�\�I�Ȏ��s�E�ے� */
#define		BT_AT_SEQ_STAT_ERROR				(9)						/* AT�R�}���h�V�[�P���X�F�ُ�F�ʏ�N���肦�Ȃ����s */

#define 	FRAME_NOTFOUND	 					(0)						/* �P�t���[������M */
#define 	FRAME_FOUND 						(1)						/* �P�t���[����M */

#define 	SEARCH_HEADER_CR					(0)						/* �w�b�_CR���� */
#define 	SEARCH_HEADER_LF					(1)						/* �w�b�_LF���� */
#define 	SEARCH_FOOTER_CR					(2)						/* �t�b�^CR���� */
#define 	SEARCH_FOOTER_LF					(3)						/* �t�b�^CR���� */

#define 	AT_CMD_TERMINATIONCODE				"\r\n"					/* AT�R�}���h�F�I�[�R�[�h*/
#define 	AT_CMD_TERMINATIONLEN				(2)						/* AT�R�}���h�F�I�[�R�[�h�� */
#define		PAIRING_WAIT_COUNT					(49)					/* �y�A�����O�葱�������J�E���^(500ms)	*/

#define 	DEVNAMESIZE							(6)						/*�f�o�C�X���T�C�Y*/

#ifdef BT_SOFT_FLOW_CONTROL	/* BLE���� T.Kamakura */
#define		BT_FC_BUF_BYTES						(512)					/* �t���[����o�b�t�@�T�C�Y	*/
#define		BT_FC_BYTES_PER_TRIGGER				(10000 / 100)			/* 10msec������̃o�C�g��	*/
#endif	/* BT_SOFT_FLOW_CONTROL */

/*--------------------------------------------------------------------------*/
/*	�\���̐錾																*/
/*--------------------------------------------------------------------------*/
/**
 * AT�R�}���h�V�[�P���X�f�[�^
 */
typedef struct {
	const UI_8	tx_len;						/* ���M�f�[�^��	*/
	const UI_8*	tx_data;					/* ���M�f�[�^�i�k���I�[�j	*/
	const UI_8	rx_len;						/* ��M�i�����j�f�[�^��	*/
	const UI_8*	rx_data;					/* ��M�i�����j�f�[�^�i�k���I�[�j	*/
	const UI_8	rx_timeout;					/* ��M�i�����j�^�C���A�E�g�i0:�^�C���A�E�g�����^0<:�C���^�[�o��)	*/
} S_BT_AT_SEQ_DATA;

/**
 * AT�R�}���h�V�[�P���X���
 */
typedef struct {
	UI_8		status;						/* �X�e�[�^�X	*/
	UI_8		retry_count;				/* ���g���C�� */
	UI_8		interval;					/* ���g���C�Ԋu */
} S_BT_AT_SEQ_INFO;

/*--------------------------------------------------------------------------*/
/*	�萔�錾																*/
/*--------------------------------------------------------------------------*/
/**
 *	BLE���W���[�����������
 */
typedef enum {
	STATUS_BLE_INIT_WAIT_STARTUP = 0,
	STATUS_BLE_INIT_DISCOVER_MODE_1,
	STATUS_BLE_INIT_CONNECT_MODE_1,
	STATUS_BLE_INIT_SECURITY_MODE_OFF,
	STATUS_BLE_INIT_STORE_0,
	STATUS_BLE_INIT_RESTART,

	STATUS_BLE_INIT_WAIT_STARTUP_2,
	STATUS_BLE_INIT_ECHO_OFF,
	STATUS_BLE_INIT_MANUFACTURER,
	STATUS_BLE_INIT_MODEL_NUM,
	STATUS_BLE_INIT_FIRMWARE_VER,
	STATUS_BLE_INIT_SOFTWARE_VER,
	STATUS_BLE_INIT_SECURITY_TYPE,
	STATUS_BLE_INIT_SECURITY_MODE,
	STATUS_BLE_INIT_LOCAL_NAME,
	STATUS_BLE_INIT_DTR_BEHAVIOR,
	STATUS_BLE_INIT_DSR_OVERRIDE,
	STATUS_BLE_INIT_DSR_CONFIGURATION,
	STATUS_BLE_INIT_BLE_CFG_04,
	STATUS_BLE_INIT_BLE_CFG_05,
	STATUS_BLE_INIT_BLE_CFG_06,
	STATUS_BLE_INIT_BLE_CFG_26,
	STATUS_BLE_INIT_STORE,
	STATUS_BLE_INIT_RESTART_2,

	STATUS_BLE_INIT_WAIT_STARTUP_3,
	STATUS_BLE_INIT_DISCOVER_MODE_2STATUS_BLE_INIT_DISCOVER_MODE_2,
	STATUS_BLE_INIT_CONNECT_MODE_2,
	STATUS_BLE_INIT_PAIRING_MODE,
	STATUS_BLE_INIT_BLE_ROLE,
/* ��ERROR�ɂȂ邽�ߐݒ肵�Ȃ��i�H��o�ׂƓ����l��ݒ肷��̂�NG�j */
/*	STATUS_BLE_INIT_SERVER_CONFIGURATION, */
	STATUS_BLE_INIT_ENTER_DATA_MODE,
	/* �� */
	STATUS_BLE_INIT_NUM
} E_STATUS_BLE_INIT;

/* BT���W���[���ݒ著�M�f�[�^(������) */
BTMDL_LOCAL const S_BT_AT_SEQ_DATA	C_bt_at_seq_data_tbl_Init[STATUS_BLE_INIT_NUM] = {
	{ 0,	(const UI_8*)"",						8,	(const UI_8*)"+STARTUP",	BT_STARTUP_INTERVAL	},	/* �N���ҋ@ */
	{ 10,	(const UI_8*)"AT+UBTDM=1",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* ���o�\���[�h�̐ݒ�:GAP�񌟏o���[�h */
	{ 10,	(const UI_8*)"AT+UBTCM=1",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* �ڑ����[�h�̐ݒ�:GAP��ڑ����[�h */
	{ 10,	(const UI_8*)"AT+UBTSM=1",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* �Z�L�����e�B�̐ݒ�:�H��o�׏�Ԃɖ߂� */
	{ 4,	(const UI_8*)"AT&W",					2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* ���݂̐ݒ��ۑ� */
	{ 11,	(const UI_8*)"AT+CPWROFF",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* ���f�̂��ߍċN�� */

	{ 0,	(const UI_8*)"",						8,	(const UI_8*)"+STARTUP",	BT_STARTUP_INTERVAL	},	/* �N���ҋ@ */
	{ 4,	(const UI_8*)"ATE0",					2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* �G�R�[OFF */
	{ 38,	(const UI_8*)"AT+UBTLEDIS=1,\"NIPPON SEIKI CO., LTD.\"",	2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* �}�j���t�@�N�`���̐ݒ� */
	{ 23,	(const UI_8*)"AT+UBTLEDIS=2,\"OP-0459\"",2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* ���f���ԍ� */
	{ 21,	(const UI_8*)"AT+UBTLEDIS=3,\"1.0.0\"",	2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* �t�@�[���E�F�A�ԍ� */
	{ 21,	(const UI_8*)"AT+UBTLEDIS=4,\"1.0.0\"",	2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* �\�t�g�E�F�A�ԍ� */
	{ 10,	(const UI_8*)"AT+UBTST=0",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* �Z�L�����e�B�^�C�v:�V���v���y�A�����O���[�h */
	{ 10,	(const UI_8*)"AT+UBTSM=2",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* �Z�L�����e�B�̐ݒ�:�Z�L�����e�B���L�� */
	{ 21,	(const UI_8*)"AT+UBTLN=\"GRRecorder\"", 2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* ���[�J�����̐ݒ� */
	{ 5,	(const UI_8*)"AT&D1",					2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* DTR�̐�����@�̑I��:�R�}���h���[�h�ؑ֐��� */
	{ 5,	(const UI_8*)"AT&S2",					2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* DSR�̐�����@�̑I��:DSR�͐ڑ���Ԃ����� */
	{ 12,	(const UI_8*)"AT+UDCFG=3,2",			2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* DSR�̕\���ݒ�:BLE bond �σf�o�C�X�ڑ���Ԃ�\�� */
	{ 15,	(const UI_8*)"AT+UBTLECFG=4,6",			2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* BLE�ݒ�:Connection interval minimum */
	{ 15,	(const UI_8*)"AT+UBTLECFG=5,6",			2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* BLE�ݒ�:Connection interval maximum */
	{ 15,	(const UI_8*)"AT+UBTLECFG=6,0",			2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* BLE�ݒ�:Connection slave latency */
	{ 16,	(const UI_8*)"AT+UBTLECFG=26,0",		2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* BLE�ݒ�:LL PDU payload size (Data Length Extension) and ATT MTU size negotiation */
	{ 4,	(const UI_8*)"AT&W",					2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* ���݂̐ݒ��ۑ� */
	{ 11,	(const UI_8*)"AT+CPWROFF",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* ���f�̂��ߍċN�� */

	{ 0,	(const UI_8*)"",						8,	(const UI_8*)"+STARTUP",	BT_STARTUP_INTERVAL	},	/* �N���ҋ@ */
	{ 10,	(const UI_8*)"AT+UBTDM=1",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* ���o�\���[�h�̐ݒ�:GAP�񌟏o���[�h */
	{ 10,	(const UI_8*)"AT+UBTCM=2",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* �ڑ����[�h�̐ݒ�:GAP�ڑ����[�h */
	{ 10,	(const UI_8*)"AT+UBTPM=2",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* �y�A�����O���[�h�̐ݒ�:GAP�y�A�����O���[�h */
	{ 10,	(const UI_8*)"AT+UBTLE=2",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* �Z���g����/�y���t�F�����̐ݒ�:�y���t�F���� */
/* ��ERROR�ɂȂ邽�ߐݒ肵�Ȃ��i�H��o�ׂƓ����l��ݒ肷��̂�NG�j*/
/*	{ 11,	(const UI_8*)"AT+UDSC=0,6",				2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	*//* �V���A���|�[�g�T�[�r�X�̐ݒ�:�T�[�oID:0 �^�C�v:SPS */
	{ 4,	(const UI_8*)"ATO1",					2,	(const UI_8*)"OK",			BT_AT_CMD_INTERVAL	},	/* �f�[�^���[�h�ɐ؂�ւ� */
};

/**
 *	�����\�v�����
 */
typedef enum  {
	STATUS_STA_DSCV_WAIT_COMMAND_MODE = 0,
	STATUS_STA_DSCV_DISCOVERABILITY_MODE,
	STATUS_STA_DSCV_CONNECTABILITY_MODE,
	STATUS_STA_DSCV_LOCAL_NAME,
	/* �� */
	STATUS_STA_DSCV_NUM
} E_STATUS_STA_DSCV;

/* BT���W���[���ݒ著�M�f�[�^(�����\�v��) */
BTMDL_LOCAL const S_BT_AT_SEQ_DATA	C_bt_at_seq_data_tbl_sta_dscv[STATUS_STA_DSCV_NUM] = {
	{ 0,	(const UI_8*)"",						2,	(const UI_8*)"OK",	BT_AT_CMD_INTERVAL	},	/* �R�}���h���[�h�ڍs�ҋ@ */
	{ 10,	(const UI_8*)"AT+UBTDM=3",				2,	(const UI_8*)"OK",	BT_AT_CMD_INTERVAL	},	/* ���o�\���[�h�̐ݒ�:GAP��ʌ��o�\���[�h */
	{ 10,	(const UI_8*)"AT+UBTCM=2",				2,	(const UI_8*)"OK",	BT_AT_CMD_INTERVAL	},	/* �ڑ����[�h�̐ݒ�:GAP�ڑ����[�h */
	{ 21,	(const UI_8*)"AT+UBTLN=\"GRRecorder\"",	2,	(const UI_8*)"OK",	BT_AT_CMD_INTERVAL	},	/* ���[�J�����̐ݒ� */
};

/**
 *	�����s�\�v�����
 */
typedef enum  {
	STATUS_END_DSCV_DUMMY = 0,
	STATUS_END_DSCV_DISCOVERABILITY_MODE,
	STATUS_END_DSCV_CONNECTABILITY_MODE,
	STATUS_END_DSCV_ENTER_DATA_MODE,
	/* �� */
	STATUS_END_DSCV_NUM
} E_STATUS_END_DSCV;

/* BT���W���[���ݒ著�M�f�[�^(�����s�\�v��) */
BTMDL_LOCAL const S_BT_AT_SEQ_DATA	C_bt_at_seq_data_tbl_end_dscv[STATUS_END_DSCV_NUM] = {
	{ 0,	(const UI_8*)"",			0,	(const UI_8*)"",	0					},	/* �_�~�[��M����J�n */
	{ 10,	(const UI_8*)"AT+UBTDM=1",	2,	(const UI_8*)"OK",	BT_AT_CMD_INTERVAL	},	/* ���o�\���[�h�̐ݒ�:GAP�񌟏o���[�h */
	{ 10,	(const UI_8*)"AT+UBTCM=2",	2,	(const UI_8*)"OK",	BT_AT_CMD_INTERVAL	},	/* �ڑ����[�h�̐ݒ�:GAP�ڑ����[�h */
	{ 4,	(const UI_8*)"ATO1",		2,	(const UI_8*)"OK",	BT_AT_CMD_INTERVAL	},	/* �f�[�^���[�h�ɐ؂�ւ� */
};

/**
 *	�������-�O����ԕϊ��e�[�u��
 */
BTMDL_LOCAL const E_BT_COMM_MDL_STATE C_bmc_state_in2out[STATUS_BT_NUM] = {
	BT_COMM_MDL_STATE_IDLE,				/* STATUS_BT_IDLE,				�A�C�h����� */
	BT_COMM_MDL_STATE_STARTUP,			/* STATUS_BT_STARTUP,			�N����������� */
	BT_COMM_MDL_STATE_COMM_MODE,		/* STATUS_BT_COMM_MODE,			�ڑ��\��� */
	BT_COMM_MDL_STATE_PRE,				/* STATUS_BT_PRE_PAIRING_MODE,	�y�A�����O�ڍs��� */
	BT_COMM_MDL_STATE_PAIRING_MODE,		/* STATUS_BT_PAIRING_MODE,		�y�A�����O��� */
	BT_COMM_MDL_STATE_SHUTDOWN,			/* STATUS_BT_SHUTDOWN,			�I����������� */
	BT_COMM_MDL_STATE_ERROR,			/* STATUS_BT_ERROR,				�ُ��� */
};

/*--------------------------------------------------------------------------*/
/*	�����֐�																*/
/*--------------------------------------------------------------------------*/
/* ��Ԋ֐��Q */
BTMDL_LOCAL void S_bmc_state_idle( E_BMC_CMD cmd );
BTMDL_LOCAL void S_bmc_state_startup( E_BMC_CMD cmd );
BTMDL_LOCAL void S_bmc_state_pre_pairing_mode( E_BMC_CMD cmd );
BTMDL_LOCAL void S_bmc_state_pairing_mode( E_BMC_CMD cmd );
BTMDL_LOCAL void S_bmc_state_comm_mode( E_BMC_CMD cmd );
BTMDL_LOCAL void S_bmc_state_shutdown( E_BMC_CMD cmd );
BTMDL_LOCAL void S_bmc_state_error( E_BMC_CMD cmd );
/* �����֐��Q */
BTMDL_LOCAL void S_bmc_imp_shutdown(void);
BTMDL_LOCAL UI_8 S_bmc_module_init(void);
BTMDL_LOCAL UI_8 S_bmc_set_discoverable(B_8 b_discoverable);
BTMDL_LOCAL UI_8 S_bmc_chk_pairing_finished_cmd1( E_BMC_CHECK_PAIRING_FINISHED_MODE mode );
BTMDL_LOCAL UI_8 S_bmc_chk_pairing_finished_cmd2( E_BMC_CHECK_PAIRING_FINISHED_MODE mode );
BTMDL_LOCAL UI_8 S_bmc_at_command_sequence( const S_BT_AT_SEQ_DATA* at_seq_data_tbl, UI_8 status_count, S_BT_AT_SEQ_INFO* seq_info );
BTMDL_LOCAL UI_8 S_bmc_check_frame(const UI_8* p_data, UI_16 data_len, UI_16* p_frame_index, UI_16* p_frame_len);
#ifdef BT_SOFT_FLOW_CONTROL	/* BLE���� T.Kamakura */
BTMDL_LOCAL void S_bmc_fc_Init( void );
BTMDL_LOCAL UI_16 S_bmc_fc_GetDataBytes( void );
BTMDL_LOCAL UI_16 S_bmc_fc_SetData(const UI_8* data, UI_16 len);
BTMDL_LOCAL void S_bmc_fc_TriggerProc( void );
#endif	/* BT_SOFT_FLOW_CONTROL */

/* ��Ԋ֐��e�[�u�� */
BTMDL_LOCAL void ( *const C_bmc_state_func[STATUS_BT_NUM] )( E_BMC_CMD cmd ) = {
	S_bmc_state_idle,					/* STATUS_BT_IDLE, */
	S_bmc_state_startup,				/* STATUS_BT_STARTUP, */
	S_bmc_state_comm_mode,				/* STATUS_BT_COMM_MODE, */
	S_bmc_state_pre_pairing_mode,		/* STATUS_BT_PRE_PAIRING_MODE, */
	S_bmc_state_pairing_mode,			/* STATUS_BT_PAIRING_MODE, */
	S_bmc_state_shutdown,				/* STATUS_BT_SHUTDOWN, */
	S_bmc_state_error,					/* STATUS_BT_ERROR, */
};

/*--------------------------------------------------------------------------*/
/*	�����ϐ���`																*/
/*--------------------------------------------------------------------------*/
BTMDL_LOCAL	E_BT_MODULE_STATUS						S_Bt_module_status;				/* ���W���[�������� */
BTMDL_LOCAL	E_BT_COMM_MDL_PEER_STATUS 				S_Bt_peer_status;				/* �s�A�ڑ���� */
BTMDL_LOCAL	S_BT_AT_SEQ_INFO						S_Bt_at_seq_info_init;			/* ���W���[�����������g���C��� */
BTMDL_LOCAL	S_BT_AT_SEQ_INFO						S_Bt_at_seq_info_sta_dscv;		/* ���W���[�����������g���C��� */
BTMDL_LOCAL	S_BT_AT_SEQ_INFO						S_Bt_at_seq_info_end_dscv;		/* ���W���[�����������g���C��� */

BTMDL_LOCAL	UI_8									S_Bt_send_data[BT_PACKET_SZ];	/* UART���M�f�[�^ */
BTMDL_LOCAL	UI_8									S_Bt_tmp_buf[BT_RX_BUFF_SZ];	/* ��M�ް��擾�p�e���|�����ޯ̧ */
BTMDL_LOCAL	UI_8									S_Bt_rx_buf[BT_RX_BUFF_SZ];		/* ��M�ް��i�[�ޯ̧ */
BTMDL_LOCAL	T_RINGBUFFER 							S_Bt_rx_buf_mng;				/* ��M�ް��p�����O�o�b�t�@�Ǘ��̈� */

BTMDL_LOCAL	P_BT_COMM_MDL_FUNC_RECEIVE_DATA			S_bpcc_cb_receive_data;			/* �f�[�^��M�R�[���o�b�N�֐� */
BTMDL_LOCAL	E_BMC_CMD								S_bmc_cmd;						/* �v���R�}���h */

BTMDL_LOCAL	E_BMC_PAIRING_PROC						S_Bt_Pairing_Proc;				/* BT �y�A�����O�v���Z�X */
BTMDL_LOCAL B_8										S_bmc_f_first_init;				/* ���񏉊����t���O */

BTMDL_LOCAL UI_8									S_Bt_Pairing_WaitCount;			/* �y�A�����O�����҂��J�E���^	*/
#ifdef BT_SOFT_FLOW_CONTROL	/* BLE���� T.Kamakura */
BTMDL_LOCAL UI_8									S_fc_buf[BT_FC_BUF_BYTES];		/* �t���[����p�����O�o�b�t�@	*/
BTMDL_LOCAL UI_16									S_fc_buf_w_pos;					/* �����O�o�b�t�@�������݈ʒu	*/
BTMDL_LOCAL UI_16									S_fc_buf_r_pos;					/* �����O�o�b�t�@�ǂݏo���ʒu	*/
BTMDL_LOCAL UI_8									S_fc_send_buff[BT_FC_BYTES_PER_TRIGGER];	/* �P�^�X�N�����ő��M����o�b�t�@	*/
#endif	/* BT_SOFT_FLOW_CONTROL */
/*--------------------------------------------------------------------------*/
/* RAM������ 																*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	Bt_comm_mdl_Init										*/
/* �@�\�̐���	�F	������������ݸގ��ɏ����l��ݒ肷��B					*/
/* �ߒl			�F	�Ȃ�													*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
void Bt_comm_mdl_Init( void )
{
	S_Bt_module_status = STATUS_BT_IDLE;									/* ���W���[�������� ������ */

	(void)memset( &S_Bt_at_seq_info_init, 0, sizeof(S_Bt_at_seq_info_init) );			/* ���g���C��񏉊����F���W���[�������� */
	(void)memset( &S_Bt_at_seq_info_sta_dscv, 0, sizeof(S_Bt_at_seq_info_sta_dscv) );	/* ���g���C��񏉊����F�����\�v�� */
	(void)memset( &S_Bt_at_seq_info_end_dscv, 0, sizeof(S_Bt_at_seq_info_end_dscv) );	/* ���g���C��񏉊����F�����s�\�v�� */

	S_Bt_peer_status = BT_COMM_MDL_PEER_STATUS_DISCONNECTED;				/* �s�A�ڑ���� ������ */

	RingBuffer_Init(&S_Bt_rx_buf_mng, S_Bt_rx_buf, sizeof(S_Bt_rx_buf));	/* ��M�o�b�t�@������ */
	S_Bt_Pairing_Proc = PAIRING_PROC_NONE;									/* BT �y�A�����O�v���Z�X������ */
	S_bmc_f_first_init = D_TRUE;

	S_bpcc_cb_receive_data  = NULL;											/* �f�[�^��M�R�[���o�b�N�֐� */
	S_bmc_cmd				= BMC_CMD_NONE;									/* �R�}���h������ */

	(void)PortDrv_SetDataBit( IO_PO_GR_BT_RST, 		IO_PO_BIT_BT_RST, 		IO_PO_LEV_ACTIVE_BT_RST );	/* ���Z�b�g���s */
	Bt_mod_drv_Init();														/* BT���W���[���h���C�o������ */
#ifdef BT_SOFT_FLOW_CONTROL	/* BLE���� T.Kamakura */
	/* �t���[���� */
	S_bmc_fc_Init();
#endif	/* BT_SOFT_FLOW_CONTROL */
}


/*--------------------------------------------------------------------------*/
/* BT���W���[����Ԑ��䏈�� 												*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	Bt_comm_mdl_Main										*/
/* �@�\�̐���	�F	BT���W���[���̓����Ԑ�����s�� 						*/
/* 				�F		�d���I�t��� 										*/
/* 				�F		�R�}���h��� 										*/
/* 				�F		�X�L������� 										*/
/* 				�F		�I�����C����� 										*/
/* 				�F		DUT��� 						add REQ-015			*/
/* 				�F		�F�؏�� 						add REQ-016			*/
/* ����			�F	�Ȃ�													*/
/* �ߒl			�F	�Ȃ�													*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
void Bt_comm_mdl_Main( void )
{
	UI_8 	port_data;
	UI_16 	data_len = 0;
	
	/*	��M�f�[�^�擾	*/
	(void)Bt_mod_drv_GetRxData(S_Bt_tmp_buf, &data_len);
	
	if (data_len > 0)
	{
		/* �����O�o�b�t�@�Ƀf�[�^��ݒ�	*/
		(void)RingBuffer_SetArrayData(&S_Bt_rx_buf_mng, S_Bt_tmp_buf, data_len);
	}
#ifdef BT_SOFT_FLOW_CONTROL	/* BLE���� T.Kamakura */
	/* �t���[���� */
	S_bmc_fc_TriggerProc();
#endif	/* BT_SOFT_FLOW_CONTROL */

	switch( S_Bt_module_status ) {
	case STATUS_BT_COMM_MODE:
	case STATUS_BT_PRE_PAIRING_MODE:
	case STATUS_BT_PAIRING_MODE:
		/*�ڑ���Ԃ��|�[�g����擾:DSR*/
		(void)PortDrv_GetDataBit( IO_PO_GR_BLE_DSR, IO_PO_BIT_BLE_DSR, &port_data );
		if(port_data == IO_PO_LEV_BLE_DSR_HIGH)
		{
			/*�ڑ���*/
			S_Bt_peer_status = BT_COMM_MDL_PEER_STATUS_CONNECTED;
		}
		else
		{
			/*�ؒf*/
			S_Bt_peer_status = BT_COMM_MDL_PEER_STATUS_DISCONNECTED;
		}
		break;
	default:
		/*�ؒf*/
		S_Bt_peer_status = BT_COMM_MDL_PEER_STATUS_DISCONNECTED;
		break;
	}

	/**	��Ԋ֐����s	*/
	C_bmc_state_func[S_Bt_module_status]( S_bmc_cmd );


	
}


/*--------------------------------------------------------------------------*/
/* �y�A�����O��Ԃ̎擾														*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	Bt_comm_mdl_GetPairedStatus								*/
/* �@�\�̐���	�F	�y�A�����O��Ԃ̎擾									*/
/* ����			�F	�Ȃ�													*/
/* �ߒl			�F	E_BT_COMM_MDL_PAIRED_STATUS			�y�A�����O��� 		*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_PAIRED_STATUS Bt_comm_mdl_GetPairedStatus( void )
{
	E_BT_COMM_MDL_PAIRED_STATUS status;
	if (S_Bt_Pairing_Proc == PAIRING_PROC_PAIRING_COMP)
	{
		/* �y�A�����O������ԁF�y�A�����O�f�o�C�X���� */
		status = BT_COMM_MDL_PAIRED_STATUS_EXIST;
	}
	else
	{
		/* �y�A�����O������ԈȊO:�y�A�����O�f�o�C�X�Ȃ� */
		status = BT_COMM_MDL_PAIRED_STATUS_NONE;
	}
	return status;
}

/*--------------------------------------------------------------------------*/
/* BT���W���[����������Ԃ̎擾												*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	Get_Bt_module_status									*/
/* �@�\�̐���	�F	BT���W���[����������Ԃ̎擾							*/
/* ����			�F	�Ȃ�													*/
/* �ߒl			�F	E_BT_COMM_MDL_STATE			BT���W���[����������� 		*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_STATE Bt_comm_mdl_GetState( void )
{
	return C_bmc_state_in2out[S_Bt_module_status];
}

/*--------------------------------------------------------------------------*/
/* BT���W���[��Peer�ڑ���Ԃ̎擾											*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	Bt_comm_mdl_GetPeerState								*/
/* �@�\�̐���	�F	BT���W���[��Peer�ڑ���Ԃ̎擾 							*/
/* ����			�F	�Ȃ�													*/
/* �ߒl			�F	E_BT_COMM_MDL_PEER_STATUS	BT���W���[��Peer�ڑ����	*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_PEER_STATUS Bt_comm_mdl_GetPeerState( void )
{
	return S_Bt_peer_status;
}

/*--------------------------------------------------------------------------*/
/* BT���W���[���X�^�[�g�A�b�v�v��											*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	Bt_comm_mdl_Startup										*/
/* �@�\�̐���	�F	BT���W���[���X�^�[�g�A�b�v�v�� 							*/
/* ����			�F	�Ȃ�													*/
/* �ߒl			�F	E_BT_COMM_MDL_RESULT	�v������						*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_RESULT Bt_comm_mdl_Startup( void )
{
	E_BT_COMM_MDL_STATE status = Bt_comm_mdl_GetState();
	if(status != BT_COMM_MDL_STATE_IDLE)
	{
		return BT_COMM_MDL_RESULT_ERR_INVALID_STATE;
	}
	/**	<li>�R�}���h�o�^	*/
	S_bmc_cmd = BMC_CMD_STARTUP;
	return BT_COMM_MDL_RESULT_OK;
}

/*--------------------------------------------------------------------------*/
/* BT���W���[���V���b�g�_�E���v��											*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	Bt_comm_mdl_Shutdown									*/
/* �@�\�̐���	�F	BT���W���[���V���b�g�_�E���v��							*/
/* ����			�F	�Ȃ�													*/
/* �ߒl			�F	E_BT_COMM_MDL_RESULT	�v������						*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_RESULT Bt_comm_mdl_Shutdown( void )
{
	E_BT_COMM_MDL_STATE status = Bt_comm_mdl_GetState();
	if(status == BT_COMM_MDL_STATE_IDLE)
	{
		return BT_COMM_MDL_RESULT_ERR_INVALID_STATE;
	}
	/**	<li>�R�}���h�o�^	*/
	S_bmc_cmd = BMC_CMD_SHUTDOWN;
	return BT_COMM_MDL_RESULT_OK;
}

/*--------------------------------------------------------------------------*/
/* �f�[�^���M�v��															*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	Bt_comm_mdl_SendData									*/
/* �@�\�̐���	�F	�f�[�^���M�v��											*/
/* ����			�F	���M�f�[�^�A���M�T�C�Y									*/
/* �ߒl			�F	E_BT_COMM_MDL_RESULT	�v������						*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_RESULT Bt_comm_mdl_SendData( const UI_8 *buf, UI_16 len )
{
	if ( buf == NULL )
	{
		return BT_COMM_MDL_RESULT_ERR_INVALID_ARG;
	}
	/**	�s�A��ԃ`�F�b�N	*/
	if ( S_Bt_peer_status != BT_COMM_MDL_PEER_STATUS_CONNECTED )
	{
		return BT_COMM_MDL_RESULT_ERR_INVALID_STATE;
	}
#ifdef BT_SOFT_FLOW_CONTROL	/* BLE���� T.Kamakura */
	if( S_bmc_fc_SetData( buf, len ) != 1 ) {
		return BT_COMM_MDL_RESULT_ERR_BUSY;
	}
#else	/* BT_SOFT_FLOW_CONTROL */
	/**	�f�[�^���m�F	*/
	if ( Bt_mod_drv_GetTxEmptyBufferSize() < len )
	{
		return BT_COMM_MDL_RESULT_ERR_BUSY;
	}
	/**	���M	*/
	if ( Bt_mod_drv_SetTxData( buf, len ) != BT_MOD_DRV_OK )
	{
		return BT_COMM_MDL_RESULT_ERR_BUSY;
	}
#endif	/* BT_SOFT_FLOW_CONTROL */
	return BT_COMM_MDL_RESULT_OK;
}


/*--------------------------------------------------------------------------*/
/* �ʐM���[�h�ڍs�v���v��													*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	Bt_comm_mdl_EnterCommMode								*/
/* �@�\�̐���	�F	�ʐM���[�h�ڍs�v���v��									*/
/* ����			�F	�Ȃ�													*/
/* �ߒl			�F	E_BT_COMM_MDL_RESULT	�v������						*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_RESULT Bt_comm_mdl_EnterCommMode( void )
{
	E_BT_COMM_MDL_STATE status = Bt_comm_mdl_GetState();
	if (( status == BT_COMM_MDL_STATE_IDLE ) 		|| 
		( status == BT_COMM_MDL_STATE_STARTUP )		|| 
		( status == BT_COMM_MDL_STATE_COMM_MODE )	|| 
		( status == BT_COMM_MDL_STATE_SHUTDOWN )	|| 
		( status == BT_COMM_MDL_STATE_ERROR ) )
	{
		return BT_COMM_MDL_RESULT_ERR_INVALID_STATE;
	}
	/**	<li>�R�}���h�o�^	*/
	S_bmc_cmd = BMC_CMD_COMM;
	return BT_COMM_MDL_RESULT_OK;
}

/*--------------------------------------------------------------------------*/
/* �y�A�����O���[�h�ڍs�v��													*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	Bt_comm_mdl_EnterPairingMode							*/
/* �@�\�̐���	�F	�y�A�����O���[�h�ڍs�v��								*/
/* ����			�F	�Ȃ�													*/
/* �ߒl			�F	E_BT_COMM_MDL_RESULT	�v������						*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_RESULT Bt_comm_mdl_EnterPairingMode( void )
{
	E_BT_COMM_MDL_STATE status = Bt_comm_mdl_GetState();
	if ( status != BT_COMM_MDL_STATE_COMM_MODE )
	{
		return BT_COMM_MDL_RESULT_ERR_INVALID_STATE;
	}
	/**	<li>�R�}���h�o�^	*/
	S_bmc_cmd = BMC_CMD_PAIR;
	return BT_COMM_MDL_RESULT_OK;
}


/*--------------------------------------------------------------------------*/
/* iAP�ڑ����擾�v��														*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	Bt_comm_mdl_GetIapAuthInfo								*/
/* �@�\�̐���	�F	iAP�ڑ����擾�v��										*/
/* ����			�F	iAP�ڑ����												*/
/* �ߒl			�F	E_BT_COMM_MDL_RESULT	�v������						*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
E_BT_COMM_MDL_RESULT Bt_comm_mdl_GetIapAuthInfo( const S_IAP_CONNECTON_INFO** info )
{
	return BT_COMM_MDL_RESULT_ERR_FATAL;
}

/*--------------------------------------------------------------------------*/
/* �f�[�^��M�R�[���o�b�N�֐��o�^											*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	Bt_comm_mdl_RegisterCbReceiveData						*/
/* �@�\�̐���	�F	�f�[�^��M�R�[���o�b�N�֐��o�^							*/
/* ����			�F	�R�[���o�b�N�֐�										*/
/* �ߒl			�F	�Ȃ�													*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
void Bt_comm_mdl_RegisterCbReceiveData( const P_BT_COMM_MDL_FUNC_RECEIVE_DATA cb )
{
	S_bpcc_cb_receive_data = cb;
}

/*--------------------------------------------------------------------------*/
/* BLE���W���[��(ANNA-B112)�����ݒ菈�� 									*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	S_bmc_module_init										*/
/* �@�\�̐���	�F	BLE���W���[��(ANNA-B112)�̏����ݒ菈�������s����B 		*/
/* ����			�F	void													*/
/* �ߒl			�F	UI_8	�����ݒ菈������ 								*/
/*				�F			BT_AT_SEQ_STAT_PROCESSING						*/
/*				�F			BT_AT_SEQ_STAT_COMPLETE							*/
/*				�F			BT_AT_SEQ_STAT_ERROR							*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
BTMDL_LOCAL UI_8 S_bmc_module_init(void)
{
	UI_8	ret;
	
	ret = S_bmc_at_command_sequence( C_bt_at_seq_data_tbl_Init, STATUS_BLE_INIT_NUM, &S_Bt_at_seq_info_init );

	return ret;
}

/*--------------------------------------------------------------------------*/
/* �����\��Ԑؑ�															*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	S_bmc_set_discoverable									*/
/* �@�\�̐���	�F	�����\�ۏ�Ԃ�AT�R�}���h�V�[�P���X�Ő؂�ւ���B	*/
/* ����			�F	B_8		������(D_TRUE:������/D_FALSE:�����s��)		*/
/* �ߒl			�F	UI_8	BLE�����\��Ԑݒ���							*/
/*				�F			BT_AT_SEQ_STAT_PROCESSING						*/
/*				�F			BT_AT_SEQ_STAT_COMPLETE							*/
/*				�F			BT_AT_SEQ_STAT_ERROR							*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
BTMDL_LOCAL UI_8 S_bmc_set_discoverable(B_8 b_discoverable)
{
	UI_8	ret;
	
	if( D_TRUE == b_discoverable ) {
		/* �����ɐݒ肷�� */
		ret = S_bmc_at_command_sequence( C_bt_at_seq_data_tbl_sta_dscv, STATUS_STA_DSCV_NUM, &S_Bt_at_seq_info_sta_dscv );
	}
	else {
		/* �����s�ɐݒ肷�� */
		ret = S_bmc_at_command_sequence( C_bt_at_seq_data_tbl_end_dscv, STATUS_END_DSCV_NUM, &S_Bt_at_seq_info_end_dscv );
	}
	return ret;
}

/*--------------------------------------------------------------------------*/
/* �y�A�����O��������P														*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	S_bmc_chk_pairing_finished_cmd1						*/
/* �@�\�̐���	�F	�y�A�����O�̊�����AT�R�}���h�V�[�P���X�Ŋm�F����B		*/
/* ����			�F	mode	�`�F�b�N���[�h									*/
/* �ߒl			�F	UI_8	�y�A�����O����������							*/
/*				�F			BT_AT_SEQ_STAT_PROCESSING						*/
/*				�F			BT_AT_SEQ_STAT_COMPLETE							*/
/*				�F			BT_AT_SEQ_STAT_FAILED							*/
/*				�F			BT_AT_SEQ_STAT_ERROR							*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
BTMDL_LOCAL UI_8 S_bmc_chk_pairing_finished_cmd1( E_BMC_CHECK_PAIRING_FINISHED_MODE mode )
{
	UI_16						data_len;
	UI_16 						frame_len;									/* 1�t���[���� */
	UI_16 						cmd_data_len;								/* �R�}���h�f�[�^�� */
	UI_16						frame_index;								/* �o�b�t�@���̃t���[���擪�C���f�b�N�X */
	UI_8						frame_chkeck_result = FRAME_NOTFOUND;		/* �t���[���`�F�b�N���� */
	UI_8*						p_data;
	const UI_8*					cmd_header = (const UI_8*)"+UUBTB:";
	const UI_16					cmd_header_len = 7;
	UI_8						ret = BT_AT_SEQ_STAT_PROCESSING;

	/* +UUBTB:<�ڑ��@���BT�A�h���X>,0:���� ��ҋ@����	*/
	/* +UUBTB:<�ڑ��@���BT�A�h���X>,2:�Ȃǂ͎��s		*/

	/* �����O�o�b�t�@����ǂݍ��݉\�ȃf�[�^�T�C�Y���擾 */
	data_len = RingBuffer_GetCanReadSize(&S_Bt_rx_buf_mng);
	if( 0 < data_len ) {
		/*�����O�o�b�t�@���f�[�^�擾 */
		(void)RingBuffer_GetArrayData(&S_Bt_rx_buf_mng, S_Bt_tmp_buf, data_len, RINGBUF_ONLY_READ);
		/*1�t���[�����̃f�[�^�����邩�`�F�b�N */
		frame_chkeck_result = S_bmc_check_frame(S_Bt_tmp_buf, data_len, &frame_index, &frame_len);
		if( FRAME_FOUND == frame_chkeck_result ) {
			/* �t���[������ */
			if( (AT_CMD_TERMINATIONLEN * 2) == frame_len ) {
				/* [CRLF][CRLF]��M���̃t�F�[���i�ʏ�͂��肦�Ȃ��j*/
				/*�����O�o�b�t�@�̃f�[�^�ǂݎ̂� */
				(void)ReadEndRingBuffer(&S_Bt_rx_buf_mng, frame_index + AT_CMD_TERMINATIONLEN);
			}
			else {
		
				cmd_data_len = frame_len - (AT_CMD_TERMINATIONLEN * 2);						/* �擪�I�[��\r\n�����Z */
				p_data = &S_Bt_tmp_buf[frame_index + AT_CMD_TERMINATIONLEN];	/* ���f�[�^�擪�A�h���X�ێ� */
				if( cmd_data_len >= (cmd_header_len + 3) ) { /* [*,0] �Œ�3�o�C�g */
					/* �������ҋ@���������ɍ��v */
					if( 0 == memcmp( p_data, cmd_header, (size_t)cmd_header_len ) ) {
						/* �R�}���h�w�b�_�܂Ŋm�F������X�e�[�^�X�m�F�J�n */
						ret = BT_AT_SEQ_STAT_ERROR;	/* ���̒l�̂܂܏I������̂͑z��O�F�R�}���h��͏������m�F���邱�� */

						/* �J���}���m�F */
						if( (UI_8)',' == *(p_data + cmd_data_len - 2) ) {
							/* �X�e�[�^�X���m�F */
							if( (UI_8)'0' == *(p_data + cmd_data_len - 1) ) {
								/* �������m�F */
								ret = BT_AT_SEQ_STAT_COMPLETE;
							}
							else {
								ret = BT_AT_SEQ_STAT_FAILED;
							}
						}
						else {
						}
					}
				}
				if (( CHECK_PAIRING_FINISHED_MODE_CMD_READ_END == mode ) || ( BT_AT_SEQ_STAT_PROCESSING != ret )) {
					/*�����O�o�b�t�@�̃f�[�^�ǂݎ̂� */
					(void)ReadEndRingBuffer(&S_Bt_rx_buf_mng, frame_index + frame_len);
				}
			}
		}
	}
	return ret;
}

/*--------------------------------------------------------------------------*/
/* �y�A�����O��������														*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	S_bmc_chk_pairing_finished_cmd2						*/
/* �@�\�̐���	�F	�y�A�����O�̊�����AT�R�}���h�V�[�P���X�Ŋm�F����B		*/
/* ����			�F	mode	�`�F�b�N���[�h									*/
/* �ߒl			�F	UI_8	�y�A�����O����������							*/
/*				�F			BT_AT_SEQ_STAT_PROCESSING						*/
/*				�F			BT_AT_SEQ_STAT_COMPLETE							*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
BTMDL_LOCAL UI_8 S_bmc_chk_pairing_finished_cmd2( E_BMC_CHECK_PAIRING_FINISHED_MODE mode )
{
	UI_16						data_len;
	UI_16 						frame_len;									/* 1�t���[���� */
	UI_16 						cmd_data_len;								/* �R�}���h�f�[�^�� */
	UI_16						frame_index;								/* �o�b�t�@���̃t���[���擪�C���f�b�N�X */
	UI_8						frame_chkeck_result = FRAME_NOTFOUND;		/* �t���[���`�F�b�N���� */
	UI_8*						p_data;
	const UI_8*					cmd_header = (const UI_8*)"+UUDPC:";
	const UI_16					cmd_header_len = 7;
	UI_8						ret = BT_AT_SEQ_STAT_PROCESSING;

	/* +UUDPC:<�n���h��>,<�^�C�v>,<�v���t�@�C��>,<�A�h���X>,<�t���[���T�C�Y> ��ҋ@����	*/

	/* �����O�o�b�t�@����ǂݍ��݉\�ȃf�[�^�T�C�Y���擾 */
	data_len = RingBuffer_GetCanReadSize(&S_Bt_rx_buf_mng);
	if( 0 < data_len ) {
		/*�����O�o�b�t�@���f�[�^�擾 */
		(void)RingBuffer_GetArrayData(&S_Bt_rx_buf_mng, S_Bt_tmp_buf, data_len, RINGBUF_ONLY_READ);
		/*1�t���[�����̃f�[�^�����邩�`�F�b�N */
		frame_chkeck_result = S_bmc_check_frame(S_Bt_tmp_buf, data_len, &frame_index, &frame_len);
		if( FRAME_FOUND == frame_chkeck_result ) {
			/* �t���[������ */
			if( (AT_CMD_TERMINATIONLEN * 2) == frame_len ) {
				/* [CRLF][CRLF]��M���̃t�F�[���i�ʏ�͂��肦�Ȃ��j */
				/*�����O�o�b�t�@�̃f�[�^�ǂݎ̂� */
				(void)ReadEndRingBuffer(&S_Bt_rx_buf_mng, frame_index + AT_CMD_TERMINATIONLEN);
			}
			else {
				cmd_data_len = frame_len - (AT_CMD_TERMINATIONLEN * 2);						/* �擪�I�[��\r\n�����Z */
				p_data = &S_Bt_tmp_buf[frame_index + AT_CMD_TERMINATIONLEN];	/* ���f�[�^�擪�A�h���X�ێ� */
				if( cmd_data_len >= (cmd_header_len + 9) ) { /* [*,*,*,*,*] �Œ�9�o�C�g */
					/* �������ҋ@���������ɍ��v */
					if( 0 == memcmp( p_data, cmd_header, (size_t)cmd_header_len ) ) {
						/* �ҋ@�������ɍ��v */
						ret = BT_AT_SEQ_STAT_COMPLETE;
					}
				}
				if (( CHECK_PAIRING_FINISHED_MODE_CMD_READ_END == mode ) || ( BT_AT_SEQ_STAT_PROCESSING != ret )) {
					/*�����O�o�b�t�@�̃f�[�^�ǂݎ̂� */
					(void)ReadEndRingBuffer(&S_Bt_rx_buf_mng, frame_index + frame_len);
				}
			}
		}
	}
	return ret;
}

/*--------------------------------------------------------------------------*/
/* �ėpAT�R�}���h�V�[�P���X���s�֐�											*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	S_bmc_at_command_sequence								*/
/* �@�\�̐���	�F	AT�R�}���h�V�[�P���X�����s����B						*/
/* ����			�F	�Ȃ�													*/
/* �ߒl			�F	UI_8	�y�A�����O����������							*/
/*				�F			BT_AT_SEQ_STAT_PROCESSING						*/
/*				�F			BT_AT_SEQ_STAT_COMPLETE							*/
/*				�F			BT_AT_SEQ_STAT_ERROR							*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
BTMDL_LOCAL UI_8 S_bmc_at_command_sequence( const S_BT_AT_SEQ_DATA* at_seq_data_tbl, UI_8 status_count, S_BT_AT_SEQ_INFO* seq_info )
{
	const S_BT_AT_SEQ_DATA*		at_seq_data;								/* AT�R�}���h�V�[�P���X�f�[�^	*/
	UI_16						data_len;
	UI_16 						frame_len;									/* 1�t���[���� */
	UI_16						frame_index;								/* �o�b�t�@���̃t���[���擪�C���f�b�N�X */
	UI_8						frame_chkeck_result = FRAME_NOTFOUND;		/* �t���[���`�F�b�N���� */
	UI_8*						p_data;
	UI_8						Bt_send_len;								/* UART���M�f�[�^�� */
	B_8							f_recv = D_FALSE;							/* UART��M�t���O */
	B_8							f_send = D_FALSE;							/* UART���M�t���O */
	B_8							f_echo = D_FALSE;							/* ECHO��M�t���O */
	UI_8						ret = BT_AT_SEQ_STAT_PROCESSING;			/* �����ݒ菈���̖������ݒ� */

	/* �J�����g��Ԃ�AT�R�}���h�V�[�P���X���擾 */
	at_seq_data = &at_seq_data_tbl[seq_info->status];

	if( 0 < at_seq_data->rx_len ) {
		/* �����O�o�b�t�@����ǂݍ��݉\�ȃf�[�^�T�C�Y���擾 */
		data_len = RingBuffer_GetCanReadSize(&S_Bt_rx_buf_mng);
		if( 0 < data_len ) {
			/*�����O�o�b�t�@���f�[�^�擾 */
			(void)RingBuffer_GetArrayData(&S_Bt_rx_buf_mng, S_Bt_tmp_buf, data_len, RINGBUF_ONLY_READ);

			if( (0 < at_seq_data->tx_len) && (data_len >= (at_seq_data->tx_len + AT_CMD_TERMINATIONLEN)) ) {
				/* �G�R�[���L�����Z�� */
				if( 0 == memcmp( S_Bt_tmp_buf, at_seq_data->tx_data, (size_t)at_seq_data->tx_len ) ) {
					if( 0 == memcmp( S_Bt_tmp_buf + at_seq_data->tx_len, AT_CMD_TERMINATIONCODE, AT_CMD_TERMINATIONLEN ) ) {
						/*�����O�o�b�t�@�̃f�[�^�ǂݎ̂� */
						(void)ReadEndRingBuffer(&S_Bt_rx_buf_mng, (UI_16)at_seq_data->tx_len + AT_CMD_TERMINATIONLEN);
						f_echo = D_TRUE;
					}
				}
			}
			if( D_FALSE == f_echo ) {
				/*1�t���[�����̃f�[�^�����邩�`�F�b�N */
				frame_chkeck_result = S_bmc_check_frame(S_Bt_tmp_buf, data_len, &frame_index, &frame_len);
				if( FRAME_FOUND == frame_chkeck_result ) {
					/* �t���[������ */
					if( (AT_CMD_TERMINATIONLEN * 2) == frame_len ) {
						/* [CRLF][CRLF]��M���̃t�F�[���i�ʏ�͂��肦�Ȃ��j */
						/*�����O�o�b�t�@�̃f�[�^�ǂݎ̂� */
						(void)ReadEndRingBuffer(&S_Bt_rx_buf_mng, frame_index + AT_CMD_TERMINATIONLEN);
					}
					else {
						/*�����O�o�b�t�@�̃f�[�^�ǂݎ̂� */
						(void)ReadEndRingBuffer(&S_Bt_rx_buf_mng, frame_index + frame_len);
		
						frame_len -= (AT_CMD_TERMINATIONLEN * 2);						/* �擪�I�[��\r\n�����Z */
						p_data = &S_Bt_tmp_buf[frame_index + AT_CMD_TERMINATIONLEN];	/* ���f�[�^�擪�A�h���X�ێ� */
						if( frame_len == at_seq_data->rx_len ) {
							/* �������ҋ@���������Ɠ����� */
							if( 0 == memcmp( p_data, at_seq_data->rx_data, (size_t)frame_len ) ) {
								/* �ҋ@�������o������X�e�[�^�X�X�V */
								seq_info->status++;
								f_recv = D_TRUE;
							}
						}
					}
				}
			}
		}
	}
	else {
		/* �ҋ@�R�}���h�Ȃ��ꍇ�͎�M�������Ƃɂ��ăX�e�[�^�X�X�V */
		seq_info->status++;
		f_recv = D_TRUE;
	}

	if( D_TRUE == f_recv ) {
		/* ��M�����瑗�M */
		f_send = D_TRUE;
	}
	else {
		/* ��M���Ȃ��Ԃ̓^�C���A�E�g���� */
		if( 0 < seq_info->interval ) {
			seq_info->interval--;
		}
		else {
			/* �^�C���A�E�g */
			if( 0 < at_seq_data->rx_timeout ) {
				/* ���g���C�񐔍X�V */
				seq_info->retry_count++;
				if( BT_AT_CMD_RETRYCOUNT >= seq_info->retry_count ) {
					/* ���g���C�񐔐����𒴂��Ă��Ȃ��Ȃ�đ� */
					f_send = D_TRUE;
				}
				else {
					/* �^�C���A�E�g */
					ret = BT_AT_SEQ_STAT_ERROR;
				}
			}
		}
	}

	if( D_TRUE == f_send ) {
		if( seq_info->status < status_count ) {
			at_seq_data = &at_seq_data_tbl[seq_info->status];
			if( 0 < at_seq_data->tx_len ) {
				/* ��M�o�b�t�@��j�� */
				RingBuffer_Clear(&S_Bt_rx_buf_mng);

				/* ���M�R�}���h�g�ݗ��� */
				(void)memset( S_Bt_send_data, 0, sizeof(S_Bt_send_data) );
				(void)memcpy( S_Bt_send_data, at_seq_data->tx_data, (size_t)at_seq_data->tx_len );
				(void)memcpy( S_Bt_send_data + at_seq_data->tx_len, AT_CMD_TERMINATIONCODE, AT_CMD_TERMINATIONLEN );
				Bt_send_len = at_seq_data->tx_len + AT_CMD_TERMINATIONLEN;

				(void)Bt_mod_drv_SetTxData( S_Bt_send_data, (UI_16)Bt_send_len );		/* UART�R�}���h���M */
			}
			/* ���g���C���M�J�E���^�ɃC���^�[�o�����Ԑݒ� */
			seq_info->interval = at_seq_data->rx_timeout;
		}
		else {
			/* �I�[��Ԃɓ��B�F���M�R�}���h�Ȃ� */
			ret = BT_AT_SEQ_STAT_COMPLETE;
		}
	}
	return ret;
}

/*--------------------------------------------------------------------------*/
/* 1�t���[�����̎�M�f�[�^������											*/
/*--------------------------------------------------------------------------*/
/* �֐�����		�F	S_bmc_check_frame										*/
/* �@�\�̐���	�F	BT���W���[��(BT401AP2)�����M�����f�[�^����			*/
/*				�F	1�t���[������M���Ă��邩���肷�� 						*/
/* ����			�F	UI_8*				��M�f�[�^�o�b�t�@					*/
/* ����			�F	UI_16				��M�f�[�^��						*/
/* ����			�F	UI_16*				�t���[���擪�C���f�b�N�X			*/
/* ����			�F	UI_16*				�t���[����							*/
/* �ߒl			�F	UI_8				FRAME_FOUND		:�f�[�^���� 		*/
/*				�F						FRAME_NOTFOUND	:�f�[�^�Ȃ�			*/
/* ���l			�F															*/
/*--------------------------------------------------------------------------*/
BTMDL_LOCAL UI_8 S_bmc_check_frame(const UI_8* p_data, UI_16 data_len, UI_16* p_frame_index, UI_16* p_frame_len)
{
	UI_16	i;
	SI_16 	frameTopIndex;
	SI_16 	frameBottomIndex;
	UI_8	result = FRAME_NOTFOUND;
	UI_8	search_procNo = SEARCH_HEADER_CR;

	if ((p_data == NULL) || (p_frame_index == NULL) || (p_frame_len == NULL) || (data_len <= 4))
	{
		return result;
	}
	
	for( i = 0 ; (i < data_len) && (result != FRAME_FOUND) ; i++ )
	{
		switch(search_procNo)
		{
			case SEARCH_HEADER_CR:		/* �w�b�_1�`�F�b�N'\r' */
				if (p_data[i] == '\r') 
				{
					search_procNo++;
					/* �擪�C���f�b�N�X�ێ� */
					frameTopIndex = (SI_16)i;
				}
				break;
			case SEARCH_HEADER_LF:		/* �w�b�_2�`�F�b�N'\n' */
				if (p_data[i] == '\n') 
				{
					search_procNo++;
				}
				else
				{
					search_procNo = SEARCH_HEADER_CR;
					frameTopIndex = -1;
				}
				break;
			case SEARCH_FOOTER_CR:		/* �t�b�^1�`�F�b�N'\r' */
				if (p_data[i] == '\r') 
				{
					search_procNo++;
				}
				break;
			case SEARCH_FOOTER_LF:		/* �t�b�^2�`�F�b�N'\n' */
				if (p_data[i] == '\n') 
				{
					/*�P�t���[������������ */
					search_procNo++;
					/* �I�[�C���f�b�N�X�ێ� */
					frameBottomIndex = (SI_16)i;
					/* ���ʂ��� */
					result = FRAME_FOUND;
					
					/* 1�t���[���̐擪�C���f�b�N�X */
					*p_frame_index = (UI_16)frameTopIndex;
					/* 1�t���[���̃f�[�^���Z�o */
					*p_frame_len = (UI_16)((frameBottomIndex - frameTopIndex) + 1);
				}
				else
				{
					search_procNo = SEARCH_HEADER_CR;
					frameTopIndex = -1;
				}
				break;
			default:
				break;
		}
	}
	return result;
}

/**
 *
 * ��Ԋ֐��FIdle
 *
 * @param	[in]	cmd	�o�^�R�}���h
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_state_idle( E_BMC_CMD cmd )
{
	UI_16 data_len;
	
	switch ( cmd )
	{
		case BMC_CMD_STARTUP:	/**< BT���W���[���N��						*/
			(void)PortDrv_SetDataBit( IO_PO_GR_BT_RST, 		IO_PO_BIT_BT_RST, 		IO_PO_LEV_NONACT_BT_RST );			/* ���Z�b�g���� */
			(void)PortDrv_SetDataBit( IO_PO_GR_BLE_SW1,		IO_PO_BIT_BLE_SW1,		IO_PO_LEV_BLE_SW1_LOW );			/* SW1:LOW�Œ� */
			(void)PortDrv_SetDataBit( IO_PO_GR_BLE_SW2,		IO_PO_BIT_BLE_SW2,		IO_PO_LEV_BLE_SW2_LOW );			/* SW2:LOW�Œ� */
			(void)PortDrv_SetDataBit( IO_PO_GR_BLE_DTR,		IO_PO_BIT_BLE_DTR,		IO_PO_LEV_BLE_DTR_HIGH );			/* DTR:HIGH�ŊJ�n */
			(void)memset( &S_Bt_at_seq_info_init, 0, sizeof(S_Bt_at_seq_info_init) );	/* ���g���C��񏉊����F���W���[�������� */
			if( D_TRUE == S_bmc_f_first_init ) {
				S_Bt_at_seq_info_init.status = STATUS_BLE_INIT_WAIT_STARTUP;				/* ���W���[����������� ������ */
				S_bmc_f_first_init = D_FALSE;
			}
			else {
				S_Bt_at_seq_info_init.status = STATUS_BLE_INIT_WAIT_STARTUP_3;				/* ���W���[����������� ������ */
			}
			S_Bt_at_seq_info_init.interval = C_bt_at_seq_data_tbl_Init[S_Bt_at_seq_info_init.status].rx_timeout;
			/* �N���O��BT�h���C�o�̎�M�o�b�t�@���c���Ă���i���O�̒ʐM�f�[�^�j�ꍇ������̂Ŏ�M�f�[�^�����ׂĎ擾���Ĕj������ */
			do{
				data_len = 0;
				(void)Bt_mod_drv_GetRxData(S_Bt_tmp_buf, &data_len);
			}while(data_len > 0);
			/* �����O�o�b�t�@���j�� */
			RingBuffer_Clear(&S_Bt_rx_buf_mng);
			
			/**	BT���W���[���N��	*/
			Bt_mod_drv_StartBtModule();
			/**	�ʐM�J�n	*/
			Bt_mod_drv_StartComm();
			
			S_Bt_module_status = STATUS_BT_STARTUP;									/* �N����������Ԃ֏�ԑJ�� */

			break;
		case BMC_CMD_SHUTDOWN:	/**< BT���W���[����~						*/
		case BMC_CMD_COMM:		/**< �ʐM���[�h�J�n							*/
		case BMC_CMD_PAIR:		/**< �y�A�����O���[�h�J�n					*/
		default:
			S_bmc_cmd = BMC_CMD_NONE;
			break;
	}
}


/**
 *
 * ��Ԋ֐��Fstartup
 *
 * @param	[in]	cmd	�o�^�R�}���h
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_state_startup( E_BMC_CMD cmd )
{
	UI_8	init_stat;				/* �����ݒ��� */
	switch ( cmd )
	{
		case BMC_CMD_STARTUP:	/**< BT���W���[���N��						*/
			init_stat = S_bmc_module_init();							/* BT���W���[���̏����ݒ菈�� */
			if (init_stat == BT_AT_SEQ_STAT_COMPLETE) {					/* �����ݒ菈�������H */
#ifdef BT_SOFT_FLOW_CONTROL	/* BLE���� T.Kamakura */
				/* �t���[���䏉���� */
				S_bmc_fc_Init();
#endif	/* BT_SOFT_FLOW_CONTROL */
				S_Bt_module_status = STATUS_BT_COMM_MODE;				/* �ʐM�\��Ԃ֑J�� */
				
				S_bmc_cmd = BMC_CMD_NONE;
			}
			else if (init_stat == BT_AT_SEQ_STAT_ERROR) {				/* �����ݒ菈���ُ� */
				S_Bt_module_status = STATUS_BT_ERROR;					/* �ُ��Ԃ֑J�� */
				
				S_bmc_cmd = BMC_CMD_NONE;
			}
			else {
				/* nop */
			}
			break;
		case BMC_CMD_SHUTDOWN:	/**< BT���W���[����~						*/
			
			S_bmc_imp_shutdown();										/* �V���b�g�_�E������ */
			S_Bt_module_status = STATUS_BT_SHUTDOWN;					/* �I����������Ԃ֑J�� */
			
			break;
		case BMC_CMD_COMM:		/**< �ʐM���[�h�J�n							*/
		case BMC_CMD_PAIR:		/**< �y�A�����O���[�h�J�n					*/
		default:
			S_bmc_cmd = BMC_CMD_NONE;
			break;
	}
}


/**
 *
 * ��Ԋ֐��Fcomm_mode
 *
 * @param	[in]	cmd	�o�^�R�}���h
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_state_comm_mode( E_BMC_CMD cmd )
{
	UI_16 	data_len;
	
	switch ( cmd )
	{
		case BMC_CMD_SHUTDOWN:	/**< BT���W���[����~						*/

			S_bmc_imp_shutdown();										/* �V���b�g�_�E������ */
			S_Bt_module_status = STATUS_BT_SHUTDOWN;					/* �I����������Ԃ֑J�� */
			
			break;
		case BMC_CMD_PAIR:		/**< �y�A�����O���[�h�J�n					*/
			/* �R�}���h���[�h�ɓ���O�Ɏ�M�o�b�t�@��j�� */
			RingBuffer_Clear(&S_Bt_rx_buf_mng);
#ifdef BT_SOFT_FLOW_CONTROL	/* BLE���� T.Kamakura */
			/* �t���[���䏉���� */
			S_bmc_fc_Init();
#endif	/* BT_SOFT_FLOW_CONTROL */

			/* �R�}���h���[�h�ɓ���: DTR(HIGH��LOW) */
			(void)PortDrv_SetDataBit( IO_PO_GR_BLE_DTR, IO_PO_BIT_BLE_DTR, IO_PO_LEV_BLE_DTR_LOW );
			/* ��DTR��LOW�̂܂܏�ԑJ�ڂ���iHIGH�֖߂��̂̓y�A�����O��ԂŁj*/

			(void)memset( &S_Bt_at_seq_info_sta_dscv, 0, sizeof(S_Bt_at_seq_info_sta_dscv) );	/* ���g���C��񏉊��� */
			S_Bt_at_seq_info_sta_dscv.status = STATUS_STA_DSCV_WAIT_COMMAND_MODE;
			S_Bt_at_seq_info_sta_dscv.interval = C_bt_at_seq_data_tbl_sta_dscv[S_Bt_at_seq_info_sta_dscv.status].rx_timeout;

			(void)memset( &S_Bt_at_seq_info_end_dscv, 0, sizeof(S_Bt_at_seq_info_end_dscv) );	/* ���g���C��񏉊��� */
			S_Bt_at_seq_info_end_dscv.status = STATUS_END_DSCV_DUMMY;
			S_Bt_at_seq_info_end_dscv.interval = C_bt_at_seq_data_tbl_end_dscv[S_Bt_at_seq_info_end_dscv.status].rx_timeout;

			S_Bt_module_status  = STATUS_BT_PRE_PAIRING_MODE;			/* �y�A�����O�ڍs����Ԃ֑J�� */
			S_Bt_Pairing_Proc = PAIRING_PROC_REQ_DISCOVERABLE;			/* �y�A�����O�v���Z�X�F�����\�v����� */
			break;
		case BMC_CMD_STARTUP:	/**< BT���W���[���N��						*/
		case BMC_CMD_COMM:		/**< �ʐM���[�h�J�n							*/
		default:
			S_bmc_cmd = BMC_CMD_NONE;
			break;
	}

	/* �����O�o�b�t�@�̃f�[�^�����擾 */
	data_len = RingBuffer_GetCanReadSize(&S_Bt_rx_buf_mng);
	if (data_len > 0)
	{
		/* �����O�o�b�t�@���f�[�^�擾 */
		(void)RingBuffer_GetArrayData(&S_Bt_rx_buf_mng, S_Bt_tmp_buf, data_len, RINGBUF_READEND);
		/* ��M�f�[�^���� */
		if (S_bpcc_cb_receive_data != NULL)
		{
			/* �R�[���o�b�N�֐��o�^�ς� */
			S_bpcc_cb_receive_data( S_Bt_tmp_buf, data_len );
		}
	}
}


/**
 *
 * ��Ԋ֐��Fpre_pairing_mode
 *
 * @param	[in]	cmd	�o�^�R�}���h
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_state_pre_pairing_mode( E_BMC_CMD cmd )
{
	UI_8 set_discoverable_stat;
	switch ( cmd )
	{
		case BMC_CMD_SHUTDOWN:	/**< BT���W���[����~						*/
			
			S_bmc_imp_shutdown();							/* �V���b�g�_�E������ */
			S_Bt_module_status = STATUS_BT_SHUTDOWN;		/* �I����������Ԃ֑J�� */
			
			break;
		case BMC_CMD_COMM:		/**< �ʐM���[�h�J�n							*/
		
			S_bmc_imp_shutdown();							/* �V���b�g�_�E������ */
			S_Bt_module_status = STATUS_BT_SHUTDOWN;		/* �I����������Ԃ֑J�� */
			
			break;
		case BMC_CMD_PAIR:		/**< �y�A�����O���[�h�J�n					*/
			/* �y�A�����O�v���Z�X�F�����\�v����� */
			set_discoverable_stat = S_bmc_set_discoverable(D_TRUE);
			if( BT_AT_SEQ_STAT_COMPLETE == set_discoverable_stat ) {
				/* BLE�����\��Ԑݒ�F���� */
				S_Bt_module_status = STATUS_BT_PAIRING_MODE;		/* �y�A�����O��Ԃ֑J�� */
				S_Bt_Pairing_Proc = PAIRING_PROC_DISCOVERABLE;		/* �����\��� */
			}
			else if( BT_AT_SEQ_STAT_ERROR == set_discoverable_stat ) {
				/* BLE�����\��Ԑݒ�F�ُ� */
				S_Bt_module_status = STATUS_BT_ERROR;				/* �ُ��Ԃ֑J�� */
			}
			else {
				/* BLE�����\��Ԑݒ�F������ */
				;
			}
			break;
		case BMC_CMD_STARTUP:	/**< BT���W���[���N��						*/
		default:
			S_bmc_cmd = BMC_CMD_NONE;
			break;
	}
	if(S_Bt_module_status != STATUS_BT_PRE_PAIRING_MODE) {
		/* ��Ԃ���o��Ƃ� */
		(void)PortDrv_SetDataBit( IO_PO_GR_BLE_DTR, IO_PO_BIT_BLE_DTR, IO_PO_LEV_BLE_DTR_HIGH );	/* DTR(LOW��HIGH) */

		if(S_Bt_module_status != STATUS_BT_PAIRING_MODE) {
			/* �y�A�����O���[�h�ȊO�̏�ԂɂȂ�ꍇ */
			S_Bt_Pairing_Proc = PAIRING_PROC_NONE;		/* BT �y�A�����O�v���Z�X������ */
		}
	}
}


/**
 *
 * ��Ԋ֐��Fpairing_mode
 *
 * @param	[in]	cmd	�o�^�R�}���h
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_state_pairing_mode( E_BMC_CMD cmd )
{
	UI_8 pairing_finished_stat;
	UI_8 set_discoverable_stat;
	switch ( cmd )
	{
		case BMC_CMD_SHUTDOWN:	/**< BT���W���[����~						*/
			
			S_bmc_imp_shutdown();							/* �V���b�g�_�E������ */
			S_Bt_module_status = STATUS_BT_SHUTDOWN;		/* �I����������Ԃ֑J�� */
			
			break;
		case BMC_CMD_COMM:		/**< �ʐM���[�h�J�n							*/
		
			S_bmc_imp_shutdown();							/* �V���b�g�_�E������ */
			S_Bt_module_status = STATUS_BT_SHUTDOWN;		/* �I����������Ԃ֑J�� */
			
			break;
		case BMC_CMD_PAIR:		/**< �y�A�����O���[�h�J�n					*/
			if( PAIRING_PROC_DISCOVERABLE == S_Bt_Pairing_Proc ) {
				/* �y�A�����O�v���Z�X�F�y�A�����O�������� */
				pairing_finished_stat = S_bmc_chk_pairing_finished_cmd1( CHECK_PAIRING_FINISHED_MODE_NOT_CMD_READ_END );
				if( BT_AT_SEQ_STAT_COMPLETE == pairing_finished_stat ) {
					/* �y�A�����O��������F���� */
					S_Bt_Pairing_WaitCount = PAIRING_WAIT_COUNT;
					S_Bt_Pairing_Proc = PAIRING_PROC_WAIT_PAIRING;		/* �y�A�����O�����҂���� */
				}
				else if( BT_AT_SEQ_STAT_FAILED == pairing_finished_stat ) {
					/* �y�A�����O��������F���s */
					/* ���̂܂ܐڑ�������҂� */
				}
				else if( BT_AT_SEQ_STAT_ERROR == pairing_finished_stat ) {
					/* �y�A�����O��������F�ُ� */
					S_Bt_module_status = STATUS_BT_ERROR;				/* �ُ��Ԃ֑J�� */
				}
				else {
					pairing_finished_stat = S_bmc_chk_pairing_finished_cmd2( CHECK_PAIRING_FINISHED_MODE_CMD_READ_END );
					if( BT_AT_SEQ_STAT_COMPLETE == pairing_finished_stat ) {
						/* �y�A�����O��������F���� */
						S_Bt_Pairing_WaitCount = PAIRING_WAIT_COUNT;
						S_Bt_Pairing_Proc = PAIRING_PROC_WAIT_PAIRING;		/* �y�A�����O�����҂���� */
					}
					else {
						/* �y�A�����O��������F������ */
						;
					}
				}
			}
			else if( PAIRING_PROC_WAIT_PAIRING == S_Bt_Pairing_Proc ) {
				/* �y�A�����O������ɗ���ʒm�R�}���h�����΂炭�ҋ@���� */
				S_Bt_Pairing_WaitCount--;
				if( 0 == S_Bt_Pairing_WaitCount ) {
					/* �����O�o�b�t�@�N���A */
					RingBuffer_Clear( &S_Bt_rx_buf_mng );

					S_Bt_Pairing_Proc = PAIRING_PROC_PAIRING;			/* �y�A�����O��� */
				}
			}
			else if( PAIRING_PROC_PAIRING == S_Bt_Pairing_Proc ) {
				/* �y�A�����O�v���Z�X�F�����s�\�v����� */
				set_discoverable_stat = S_bmc_set_discoverable(D_FALSE);
				if( BT_AT_SEQ_STAT_COMPLETE == set_discoverable_stat ) {
					/* BLE�����s�\��Ԑݒ�F���� */
					S_Bt_Pairing_Proc = PAIRING_PROC_PAIRING_COMP;		/* �y�A�����O������� */
					S_Bt_module_status = STATUS_BT_COMM_MODE;			/* �ʐM�\��Ԃ֑J�� */
					S_bmc_cmd = BMC_CMD_NONE;
				}
				else if( BT_AT_SEQ_STAT_ERROR == set_discoverable_stat ) {
					/* BLE�����s�\��Ԑݒ�F�ُ� */
					S_Bt_module_status = STATUS_BT_ERROR;				/* �ُ��Ԃ֑J�� */
				}
				else {
					/* BLE�����\��Ԑݒ�F������ */
					;
				}
			}
			else {
				/* ��Ԉُ� */
				S_Bt_module_status = STATUS_BT_ERROR;					/* �ُ��Ԃ֑J�� */
			}
			break;
		case BMC_CMD_STARTUP:	/**< BT���W���[���N��						*/
		default:
			S_bmc_cmd = BMC_CMD_NONE;
			break;
	}
	if(S_Bt_module_status != STATUS_BT_PAIRING_MODE) {
		if(	S_Bt_Pairing_Proc != PAIRING_PROC_PAIRING_COMP) {
			/* �y�A�����O������ԈȊO�Ŕ�����ꍇ�͏����� */
			S_Bt_Pairing_Proc = PAIRING_PROC_NONE;						/* BT �y�A�����O�v���Z�X������ */
		}
	}
}




/**
 *
 * ��Ԋ֐��Fshutdown
 *
 * @param	[in]	cmd	�o�^�R�}���h
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_state_shutdown( E_BMC_CMD cmd )
{
	switch ( cmd )
	{
		case BMC_CMD_SHUTDOWN:	/**< BT���W���[����~						*/

			S_bmc_cmd = BMC_CMD_NONE;
			S_Bt_module_status = STATUS_BT_IDLE;			/* �A�C�h����Ԃ֑J�� */
			
			break;
		case BMC_CMD_COMM:		/**< �ʐM���[�h�J�n							*/
			
			S_bmc_cmd = BMC_CMD_STARTUP;					/* BT���W���[���N���R�}���h���s */
			S_Bt_module_status = STATUS_BT_IDLE;			/* �A�C�h����Ԃ֑J�� */
			break;
		case BMC_CMD_STARTUP:	/**< BT���W���[���N��						*/
		case BMC_CMD_PAIR:		/**< �y�A�����O���[�h�J�n					*/
		default:
			S_bmc_cmd = BMC_CMD_NONE;
			break;
	}
}


/**
 *
 * ��Ԋ֐��Ferror
 *
 * @param	[in]	cmd	�o�^�R�}���h
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_state_error( E_BMC_CMD cmd )
{
	switch ( cmd )
	{
		case BMC_CMD_SHUTDOWN:	/**< BT���W���[����~						*/
			
			S_bmc_imp_shutdown();							/* �V���b�g�_�E������ */
			S_Bt_module_status = STATUS_BT_SHUTDOWN;		/* �I����������Ԃ֑J�� */
			
			break;
		case BMC_CMD_STARTUP:	/**< BT���W���[���N��						*/
		case BMC_CMD_COMM:		/**< �ʐM���[�h�J�n							*/
		case BMC_CMD_PAIR:		/**< �y�A�����O���[�h�J�n					*/
		default:
			S_bmc_cmd = BMC_CMD_NONE;
			break;
	}
}

/**
 *
 * �����֐��Fshutdown
 *
 * @param  none
 *
 * @return none
 *
 * @note
 *
 */
BTMDL_LOCAL void S_bmc_imp_shutdown(void)
{
	/**	�ʐM��~	*/
	Bt_mod_drv_StopComm();
	/**	BT���W���[����~	*/
	Bt_mod_drv_StopBtModule();
	
	(void)PortDrv_SetDataBit( IO_PO_GR_BT_RST, 		IO_PO_BIT_BT_RST, 		IO_PO_LEV_ACTIVE_BT_RST );			/* ���Z�b�g���s */
}

#ifdef BT_SOFT_FLOW_CONTROL	/* BLE���� T.Kamakura */
/**
 * 
 * �t���[����F������
 * 
 * @param	�Ȃ�
 * @return	�Ȃ�
 *
 * @note
 */
BTMDL_LOCAL void S_bmc_fc_Init( void )
{
	(void)memset( S_fc_buf, 0, sizeof(S_fc_buf) );
}
#endif	/* BT_SOFT_FLOW_CONTROL */

#ifdef BT_SOFT_FLOW_CONTROL	/* BLE���� T.Kamakura */
/**
 * 
 * �t���[����F�����M�f�[�^�T�C�Y�擾
 * 
 * @param	�Ȃ�
 * @return	���M�f�[�^�T�C�Y(�o�C�g�P��)
 *
 * @note
 */
BTMDL_LOCAL UI_16 S_bmc_fc_GetDataBytes( void )
{
	if( S_fc_buf_w_pos >= S_fc_buf_r_pos ) {
		return S_fc_buf_w_pos - S_fc_buf_r_pos;
	}
	else {
		return BT_FC_BUF_BYTES + S_fc_buf_w_pos - S_fc_buf_r_pos;
	}
}
#endif	/* BT_SOFT_FLOW_CONTROL */

#ifdef BT_SOFT_FLOW_CONTROL	/* BLE���� T.Kamakura */
/**
 * 
 * �t���[����F���M�f�[�^�ݒ�
 * 
 * @param	data	���M�f�[�^�擪�A�h���X
 * @param	len		���M�f�[�^�T�C�Y(�o�C�g�P��)
 * @return	0	�o�b�t�@�t��
 * @return	1	���M�f�[�^�ݒ萬��
 *
 * @note
 */
BTMDL_LOCAL UI_16 S_bmc_fc_SetData(const UI_8* data, UI_16 len)
{
	UI_16	i;
	UI_16	empty_bytes;

	empty_bytes = BT_FC_BUF_BYTES - S_bmc_fc_GetDataBytes();
	if( len >= empty_bytes ) {
		return 0;
	}
	for( i = 0; i < len; i++ ) {
		S_fc_buf[S_fc_buf_w_pos] = data[i];
		S_fc_buf_w_pos++;
		S_fc_buf_w_pos %= BT_FC_BUF_BYTES;
	}
	return 1;
}
#endif	/* BT_SOFT_FLOW_CONTROL */

#ifdef BT_SOFT_FLOW_CONTROL	/* BLE���� T.Kamakura */
/**
 * 
 * �t���[����F���M�g���K����
 * �P�g���K�i�^�X�N�����j���Ƀ����O�o�b�t�@�ɂ���f�[�^�̂������M�T�C�Y���������M����B
 * @param	�Ȃ�
 * @return	�Ȃ�
 *
 * @note
 */
BTMDL_LOCAL void S_bmc_fc_TriggerProc( void )
{
	UI_16	i;
	UI_16	data_bytes;
	UI_16	len;

	data_bytes = S_bmc_fc_GetDataBytes();
	if( 0 < data_bytes ) {
		if( BT_FC_BYTES_PER_TRIGGER < data_bytes ) {
			len = BT_FC_BYTES_PER_TRIGGER;
		}
		else {
			len = data_bytes;
		}
		if( Bt_mod_drv_GetTxEmptyBufferSize() >= len ) {
			/* �����Ńt���[����ς̑��M�f�[�^�𑗐M */
			for( i = 0; i < len; i++ ) {
				S_fc_send_buff[i] = S_fc_buf[S_fc_buf_r_pos];
				S_fc_buf_r_pos++;
				S_fc_buf_r_pos %= BT_FC_BUF_BYTES;
			}
			(void)Bt_mod_drv_SetTxData( S_fc_send_buff, len );
		}
	}
}
#endif	/* BT_SOFT_FLOW_CONTROL */

