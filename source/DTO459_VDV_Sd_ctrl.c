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
/* file DTO459_VDV_Sd_ctrl.c							*/
/* SD����												*/
/********************************************************/

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include <string.h>
#include "system.h"
#include "SSFTSTD_Macro.h"
#include "System_control_TASK.h"
#include "SHRVSTD_Interrupt_Drv.h"
#include "SHRVSTD_Port_Drv.h"
#include "timer_ctrl.h"
#include "DTO459_VDV_RringBuffer.h"
#include "DTO459_VDV_Sd_Drv.h"
#include "DTO459_VDV_Sw_ctrl.h"
#include "DTO459_VDV_File_io.h"
#include "DTO459_VDV_Gnss_ctrl.h"
#include "DTO459_VDV_Can_ctrl.h"
#include "DTO459_VDV_Sd_ctrl.h"
#include "DTO459_VDV_Eeprom_ctrl.h"
#include "DTO459_VDV_Common.h"
#include "DTO459_VDV_Inspection_ctrl.h"
#include "dbglog.h"



/********************************************************/
/*	�萔��`											*/
/********************************************************/
/* SD�����ԁiSD���W���[�������̐����ԁj */
typedef enum e_sd_ctrl_stat {
	SD_CTRL_STAT_INIT = 0L,								/* [S00]�������						*/
	SD_CTRL_STAT_IDLE,									/* [S01]�A�C�h��						*/
	SD_CTRL_STAT_GET_SYSMODE,							/* [S02]�V�X�e�����샂�[�h�擾			*/
	SD_CTRL_STAT_REC,									/* [S03]REC��							*/
	SD_CTRL_STAT_WAIT_REC_END,							/* [S04]REC�I���҂�						*/
	SD_CTRL_STAT_SIMULATION,							/* [S05]�V�~�����[�V������				*/
	SD_CTRL_STAT_INSPECTION,							/* [S06]SD����							*/
	SD_CTRL_STAT_WAIT_SHUTDOWN,							/* [S07]SD���W���[����~�҂�			*/
	SD_CTRL_STAT_SHUTDOWN,								/* [S08]SD���W���[����~				*/
	/* ����Ԃ͂�������ɒǉ����邱�� */
	SD_CTRL_STAT_COUNT									/*	��Ԓ�`��	*/
} E_SD_CTRL_STAT;

/* �V�~�����[�V���������� */
typedef enum e_sd_sim_stat {
	SD_SIM_STAT_NONE = 0L,	
	SD_SIM_STAT_SEARCH_SIM_FILE,						/* SIM�t�@�C��������		 				*/
	SD_SIM_STAT_WAIT_HEADER,							/* �t�@�C���w�b�_��M�҂� 					*/
	SD_SIM_STAT_WAIT_FIX_ID_LIST,						/* �Œ�M��ID��M�҂� 						*/
	SD_SIM_STAT_WAIT_FIX_VALUES,						/* �Œ�ԗ�����M�҂�						*/
	SD_SIM_STAT_WAIT_VAR_ID_LIST,						/* �ϐ��M��ID��M�҂�						*/
	SD_SIM_STAT_WAIT_VAR_VALUES,						/* �ԗ�����M�҂� 						*/
	SD_SIM_STAT_WAIT_STOP,								/* ��~�҂�									*/
	SD_SIM_STAT_WAIT_STOP_ACC_OFF,						/* ACC OFF�ɂ���~�҂�					*/
	SD_SIM_STAT_ACC_OFF,								/* ACC OFF									*/
	/* ����Ԃ͂�������ɒǉ����邱�� */
	SD_SIM_STAT_COUNT									/*	��Ԓ�`��	*/
} E_SD_SIM_STAT;

typedef enum e_sd_ctrl_sys_mode {
	SD_CTRL_SYS_MODE_UNKNOWN = 0,						/* ���[�h�s��(�����l)									*/
	SD_CTRL_SYS_MODE_NOMAL,								/* �ʏ�(SD���}������Ă��Ȃ��A�܂���SD�Ƀ��[�h�t�@�C�������݂��Ȃ��ꍇ�ɐݒ�)	*/
	SD_CTRL_SYS_MODE_SIMULATION,						/* �V�~�����[�V����										*/
	SD_CTRL_SYS_MODE_EEPROM,							/* EEPROM��������										*/
	SD_CTRL_SYS_MODE_FIRMWARE,							/* �t�@�[���E�F�A��������								*/
	SD_CTRL_SYS_MODE_INSPECTION,						/* ����													*/
	SD_CTRL_SYS_MODE_BT_COMM,							/* BT���W���[���ʐM										*/
	SD_CTRL_SYS_MODE_GNSS_COMM							/* GNSS���W���[���ʐM									*/
} E_SD_CTRL_SYS_MODE;

/* �V�~�����[�V�������䏉�������[�h */
typedef enum e_sd_sim_init_mode {
	SD_SIM_INIT_MODE_ENTER_SIM = 0,						/* ��ʑJ�ڕ\����V�~�����[�V�������[�h�ɓ���		*/
	SD_SIM_INIT_MODE_REWIND_SIM,						/* ���ʑJ�ڕ\��SIM�t�@�C���̊J�n����ъ����߂�		*/
	SD_SIM_INIT_MODE_CONTINUE							/* �t�@�C���I�[���瑱���ēǂ݂��ނƂ�				*/
} E_SD_SIM_INIT_MODE;

/* �V�~�����[�V�������䎞�Ԏ�� */
typedef enum e_sd_sim_time_kind {
	SD_SIM_TIME_KIND_LAST_SEARCH_START = 0,				/* �Ō�Ɍ������J�n��������							*/
	/* ���������ɒǉ����邱�� */
	SD_SIM_TIME_KIND_COUNT
} E_SD_SIM_TIME_KIND;

/* �V�~�����[�V�����t�@�C��������`		*/
typedef enum e_sim_file_part {
	SIM_FILE_PART_HEADER	= 0,						/* �t�@�C���w�b�_							*/
	SIM_FILE_PART_FIX_ID_LIST,							/* �Œ�ID���X�g								*/
	SIM_FILE_PART_FIX_VALUES,							/* �Œ�ԗ����								*/
	SIM_FILE_PART_VAR_ID_LIST,							/* �ϐ�ID���X�g								*/
	SIM_FILE_PART_VAR_VALUES,							/* �ϐ��ԗ����								*/
	/* ���������ɒǉ����邱�� */
	SIM_FILE_PART_COUNT
} E_SIM_FILE_PART;

/* SD����C�x���g */
typedef UI_32			SD_CTRL_EVENT; 
#define SD_CTRL_EVENT_NONE						(0L)			/* �C�x���g�Ȃ�								*/
#define SD_CTRL_EVENT_REQ_STARTUP				(1UL << 1)		/* [E01]SD���W���[���N���v��				*/
#define SD_CTRL_EVENT_REQ_SHUTDOWN				(1UL << 2)		/* [E02]SD���W���[����~�v��				*/
#define SD_CTRL_EVENT_REQ_GET_SYSMODE			(1UL << 3)		/* [E03]�V�X�e�����샂�[�h�擾�v��			*/
#define SD_CTRL_EVENT_NOTIFY_GET_SYSMODE		(1UL << 4)		/* [E04]�V�X�e�����샂�[�h�擾�����ʒm		*/
#define SD_CTRL_EVENT_REQ_START_REC				(1UL << 5)		/* [E05]REC�J�n�v��							*/
#define SD_CTRL_EVENT_REQ_WRITE_REC_DATA		(1UL << 6)		/* [E06]REC�f�[�^�������ݗv��				*/
#define SD_CTRL_EVENT_NOTIFY_WRITE_RESULT		(1UL << 7)		/* [E07]REC�f�[�^�������݌��ʒʒm			*/
#define SD_CTRL_EVENT_REQ_STOP_REC				(1UL << 8)		/* [E08]REC��~�v��							*/
#define SD_CTRL_EVENT_REQ_START_SIM				(1UL << 9)		/* [E09]�V�~�����[�V�����J�n�v��			*/
#define SD_CTRL_EVENT_NOTIFY_STOP_REC			(1UL << 10)		/* [E10]REC��~�����ʒm						*/
#define SD_CTRL_EVENT_NOTIFY_SHUTDOWN			(1UL << 11)		/* [E11]SD���W���[����~�����ʒm			*/
#define SD_CTRL_EVENT_REQ_INSPECTION			(1UL << 12)		/* [E12]SD�����v��							*/
#define SD_CTRL_EVENT_REQ_INSPECTION_START		(1UL << 13)		/* [E13]SD�����J�n							*/
#define SD_CTRL_EVENT_NOTIFY_INSPECTION_SD_RES	(1UL << 14)		/* [E14]SD�����pSD�A�N�Z�X����				*/


/* SD�������ݐ��� */
/*���f�[�^��1�t���[���̃T�C�Y
�@�w�b�_�F4byte
�@�f�[�^�F6+(�f�[�^����4)byte
�@�t�b�^�F3byte
�@���f�[�^���t���[���T�C�Y��13 + (�f�[�^����4)
�@
�@��jID40�Ȃ� 13 + (40��4) ��173 byte�ƂȂ�
�@25ms�Ԋu�ŏo�͂��邽��1000ms�Ԃ�40�t���[���K�v
*/
#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata */
/*25ms�Ԋu�ŏo�͂���ꍇ�A1000ms�Ԃ�40�t���[��*/
/*1�t���[��165byte x 40�t���[�� = 6600 byte) 6.5k x 1024 = 6656byte�Ƃ���*/
#define SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE	(6656)						/* REC�f�[�^��SD�ɏ������ރT�C�Y	*/
#elif defined _VDV_YARIS  /* 2020.02.05 S.Sakata */
/*25ms�Ԋu�ŏo�͂���ꍇ�A1000ms�Ԃ�40�t���[��*/
/*1�t���[��169byte x 40�t���[�� = 6760 byte) */
#define SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE	(6760)						/* REC�f�[�^��SD�ɏ������ރT�C�Y	*/
#elif defined _VDV_86B  /* 2021.02.18 S.Sakata */
/*ID����41  13 + (41��4) ��177 byte*/
/*1�t���[��177byte x 40�t���[�� = 7080 byte) 7.0k x 1024 = 7168byte*/
#define SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE	(7168)						/* REC�f�[�^��SD�ɏ������ރT�C�Y	*/
#else
#define SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE	(5 * 1024)					/* REC�f�[�^��SD�ɏ������ރT�C�Y	*/
#endif /* _VDV_SUPRA�@2018.12.14 S.Sakata */
#define SD_CTRL_REC_DATA_WRITE_BLOCK_NUM	(3)							/* REC�f�[�^�̃o�b�t�@�ʐ�			*/
#define	SD_CTRL_REC_TIME_MAX				(8UL * 3600UL * 1000UL)		/* REC�ő厞��[ms]�i8���ԁj 		*/
#define SD_CTRL_FIO_WRITE_NOTFY_NUM			(SD_CTRL_REC_DATA_WRITE_BLOCK_NUM + 2)	/* FileIo����̏������݊����ʒm��	*/
													/* FileIo����̏������݊����ʒm����FileIo�ɑ΂���ő�v����	*/
													/*	��REC�f�[�^�̏������ݗv�����i�������O�o�b�t�@�ʐ��j		*/
													/*		�{�w�b�_�������ݗv�����i��2�i����ƍŌ�̍X�V�j�j	*/
#define SD_CTRL_MAX_FIX_ID_COUNT			(30U)						/* �Œ�ID�̍ő吔�i�����l���j	*/
#define SD_CTRL_MAX_VAR_ID_COUNT			(60U)						/* �ϐ�ID�̍ő吔�i�����l���j	*/
#define SD_CTRL_MAX_ERRCODE_COUNT			(10U)						/*	�ő�G���[�R�[�h��			*/

#define SD_CTRL_FILE_HEADER_PART_SIZE_MAX		(90U + (2U * SD_CTRL_MAX_FIX_ID_COUNT) + (2U * SD_CTRL_MAX_VAR_ID_COUNT))	/* �t�@�C���w�b�_���ő�T�C�Y		*/
#define SD_CTRL_FIX_IDINFO_DATA_PART_SIZE_MAX	(8U + (2U * SD_CTRL_MAX_FIX_ID_COUNT))										/* �Œ�ID��񕔍ő�T�C�Y			*/
#define SD_CTRL_VAR_IDINFO_DATA_PART_SIZE_MAX	(8U + (2U * SD_CTRL_MAX_VAR_ID_COUNT))										/* �ϐ�ID��񕔍ő�T�C�Y			*/
#define SD_CTRL_FIX_SIGNAL_DATA_PART_SIZE_MAX	(13U + (6U * SD_CTRL_MAX_ERRCODE_COUNT) + (4U * SD_CTRL_MAX_FIX_ID_COUNT))	/* �Œ�ID �ԗ���񕔍ő�T�C�Y		*/
#define SD_CTRL_VAR_SIGNAL_DATA_PART_SIZE_MAX	(13U + (6U * SD_CTRL_MAX_ERRCODE_COUNT) + (4U * SD_CTRL_MAX_VAR_ID_COUNT))	/* �ϐ�ID �ԗ���񕔍ő�T�C�Y		*/

#define SD_CTRL_REC_DATA_HEAD_SIZE_MAX			(SD_CTRL_FILE_HEADER_PART_SIZE_MAX + SD_CTRL_FIX_IDINFO_DATA_PART_SIZE_MAX + SD_CTRL_FIX_SIGNAL_DATA_PART_SIZE_MAX + SD_CTRL_VAR_IDINFO_DATA_PART_SIZE_MAX)
																															/* REC�w�b�_�ő�T�C�Y				*/
#define SD_CTRL_REC_HEAD_BUF_SIZE				(((SD_CTRL_REC_DATA_HEAD_SIZE_MAX + 3) / 4) * 4)							/* REC�w�b�_�o�b�t�@�T�C�Y(4byte�A���C�����g����)	*/
#define SD_CTRL_REC_DATA_BUF_SIZE				(((SD_CTRL_VAR_SIGNAL_DATA_PART_SIZE_MAX + 3) / 4) * 4)						/* REC�f�[�^�o�b�t�@�T�C�Y(4byte�A���C�����g����)	*/
#if (SD_CTRL_REC_HEAD_BUF_SIZE > SD_CTRL_REC_DATA_BUF_SIZE)
#define SD_CTRL_REC_MAX_PACKET_SIZE				SD_CTRL_REC_HEAD_BUF_SIZE
#else
#define SD_CTRL_REC_MAX_PACKET_SIZE				SD_CTRL_REC_DATA_BUF_SIZE
#endif

/* �V�X�e�����샂�[�h */
#define SD_CTRL_SYS_MODE_FILE_NAME			"mode.inf"
#define SD_CTRL_SYS_MODE_FILE_PATH			FS_DRV_PATH_SD SD_CTRL_SYS_MODE_FILE_NAME
#define SD_CTRL_SYS_MODE_FILE_SIZE			(8)

/* �V�X�e�����샂�[�h�t�@�C�����̃��[�h�l */
#define	SD_CTRL_SYS_MODE_FILE_VALUE_SIMU	(0xF0)
#define	SD_CTRL_SYS_MODE_FILE_VALUE_EEPROM	(0xF1)
#define	SD_CTRL_SYS_MODE_FILE_VALUE_FIRM	(0xF2)
#define	SD_CTRL_SYS_MODE_FILE_VALUE_INSPECTION	(0xF3)
#define	SD_CTRL_SYS_MODE_FILE_VALUE_BT_COMM		(0xF4)
#define	SD_CTRL_SYS_MODE_FILE_VALUE_GNSS_COMM	(0xF5)

#define	SD_CTRL_REC_FILE_EXTENSION			".vdv"

/*	VDV SD�f�[�^�t�H�[�}�b�g	*/
#define SD_CTRL_FORMAT_VERSION				(0x00010000)	/*	�t�H�[�}�b�g�o�[�W����	*/
/*												 ^^^^^^
												  | | |
												  | | |
												  | | �����e�i�X�o�[�W����
												  | �}�C�i�[�o�[�W����
												  ���W���[�o�[�W����
*/

/* �w�b�_ */
/* START	1byte */
#define SD_CTRL_FMT_HEADER_START			(0x7EU)			/*	START						*/
/* LEN		2byte */
/* TYPE		1byte */
#define SD_CTRL_FMT_HEADER_BYTES			(4U)			/*	�w�b�_�T�C�Y				*/
#define SD_CTRL_FMT_UTC_LEN					(12U)			/*	UTC�����\�L��(YYMMDDHHmmSS)	*/

/* TYPE: �f�[�^��� */
#define SD_CTRL_FMT_TYPE_HEADER				(0x20U)			/*	�ԗ��M���ꗗ�iSD�J�[�h�p�j	*/
#define SD_CTRL_FMT_TYPE_ID_INFO_DATA		(0x21U)			/*	�ԗ����ID�iSD�J�[�h�p�j	*/
#define SD_CTRL_FMT_TYPE_SIGNAL_DATA		(0x22U)			/*	�ԗ����iSD�J�[�h�p�j		*/
#define SD_CTRL_FMT_TYPE_INVALID			(0xFFU)			/*	�s���ȃf�[�^���			*/

/* �t�b�^ */
/* CRC		2byte */
/* END		1byte */
#define SD_CTRL_FMT_FOOTER_END				(0x7FU)			/*	END							*/
#define SD_CTRL_FMT_FOOTER_BYTES			(3U)			/*	�t�b�^�T�C�Y				*/

#define SD_CTRL_FMT_BYTES_FIX				(SD_CTRL_FMT_HEADER_BYTES + SD_CTRL_FMT_FOOTER_BYTES)

#define SD_SIM_SIGNAL_INVALID_VALUE			(0xFFFFFFFFL)

/* �V�~�����[�V��������C�x���g */
typedef UI_32			SD_SIM_EVENT; 
#define SD_SIM_EVENT_NONE					(0L)			/* �C�x���g�Ȃ�								*/
#define SD_SIM_EVENT_FOUND_SIM_FILE			(1UL << 0)		/* SIM�t�@�C����������						*/
#define SD_SIM_EVENT_READ_ERROR				(1UL << 1)		/* �t�@�C���ǂ݃G���[����					*/
#define SD_SIM_EVENT_RECV_HEADER			(1UL << 2)		/* �t�@�C���w�b�_��M						*/
#define SD_SIM_EVENT_RECV_FIX_ID_LIST		(1UL << 3)		/* �Œ�ID����M							*/
#define SD_SIM_EVENT_RECV_FIX_VALUES		(1UL << 4)		/* �Œ�ԗ�����M							*/
#define SD_SIM_EVENT_RECV_VAR_ID_LIST		(1UL << 5)		/* �ϐ�ID����M							*/
#define SD_SIM_EVENT_RECV_VAR_VALUES		(1UL << 6)		/* �ϐ��ԗ�����M							*/
#define SD_SIM_EVENT_REQ_REWIND				(1UL << 7)		/* �V�~�����[�V�������o���v��				*/
#define SD_SIM_EVENT_END_READ				(1UL << 8)		/* �t�@�C���ǂݏI��							*/
#define SD_SIM_EVENT_ACC_OFF				(1UL << 9)		/* ACC OFF									*/

/* �V�~�����[�V�����i�s�t���O				*/
#define SD_SIM_FLAG_FOUND_SIM_FILE			(1UL << 0)		/* SIM�t�@�C��������			*/
#define SD_SIM_FLAG_GET_FILE_SIZE			(1UL << 1)		/* SIM�t�@�C���T�C�Y�擾��		*/
#define SD_SIM_FLAG_RECV_HEADER				(1UL << 2)		/* �w�b�_��M��					*/
#define SD_SIM_FLAG_RECV_FIX_ID_LIST		(1UL << 3)		/* �Œ�ID���X�g�擾��			*/
#define SD_SIM_FLAG_RECV_FIX_VALUES			(1UL << 4)		/* �Œ�ԗ����擾��			*/
#define SD_SIM_FLAG_RECV_VAR_ID_LIST		(1UL << 5)		/* �ϐ�ID���X�g�擾��			*/
#define SD_SIM_FLAG_RECV_VAR_VALUES			(1UL << 6)		/* �ϐ��ԗ����擾��			*/
#define SD_SIM_FLAG_READ_ERROR				(1UL << 7)		/* READ�G���[����				*/
#define SD_SIM_FLAG_CHANGE_VAR_ID_LIST		(1UL << 8)		/* �ϐ�ID���X�g�ύX���o			*/
#define SD_SIM_FLAG_REQ_REWIND				(1UL << 9)		/* ���o���v��					*/
/* �V�~�����[�V���������t���O				*/
#define SD_SIM_FLAG_SEARCHING_FILE			(1UL << 16)		/* �t�@�C��SEARCH��				*/
#define SD_SIM_FLAG_READING_FILE			(1UL << 17)		/* �t�@�C��READ��				*/

/* ���Ԓ�` */
#define SD_SIM_SEARCH_INTERVAL_MSEC			(1000UL)		/* �t�@�C���������s�Ԋu(msec)	*/

/* ���O�f�[�^���t���O */
#define SD_CTRL_LOG_DATA_TIME_ENABLE		(1UL << 0)		/* �������L��					*/
#define SD_CTRL_LOG_DATA_POS_ENABLE			(1UL << 1)		/* ���W���L��					*/

/* �w�b�_�X�V */
#define SD_CTRL_UPDATE_HEADER_CYCLE_MSEC	(30 * 1000)		/* �w�b�_�X�V�Ԋu				*/

#define SD_CTRL_INSPECTION_DIR					FS_DRV_PATH_SD "VDV\\test"	/* �����f�B���N�g��		*/
#define SD_CTRL_INSPECTION_DATA_SIZE			(2)								/* �����f�[�^�T�C�Y		*/

#define SD_CTRL_REC_STOP_TIMEOUT			(2UL * 60UL * 1000UL)	/* REC��~�^�C���A�E�g[ms] (2��)	*/

/* SD�����X�e�b�v */
typedef enum e_sd_inspection_step {
	SD_INSPECTION_STEP_MKDIR = 0,					/* �����p�f�B���N�g���쐬		*/
	SD_INSPECTION_STEP_WRITE,						/* �������݌���					*/
	SD_INSPECTION_STEP_READ,						/* �ǂݍ��݌���					*/
	SD_INSPECTION_STEP_INVALID						/* �����l						*/
} E_SD_INSPECTION_STEP;

/********************************************************/
/*	�\���̒�`											*/
/********************************************************/
/* REC�f�[�^�������݊����ʒm��� */
typedef struct {

	/* �������ݗv���̓��e		*/
	UI_32				bytes;						/* �������݃o�C�g��						*/

	/* �������݌���				*/
	E_FIO_RESULT		result;						/* �����݌���							*/
	UI_32				bytesWritten;				/* �������񂾃o�C�g��					*/
} T_SD_CTRL_WRITE_END_NOTIFY_INFO;

/* ���O�f�[�^��� */
typedef struct {
	SI_32				latitudeNorthernMost;		/* �Ŗk�[�ܓx 							*/
	SI_32				longtudeWesternMost;		/* �Ő��[�ܓx 							*/
	SI_32				latitudeSouthernMost;		/* �œ�[�ܓx 							*/
	SI_32				longtudeEasternMost;		/* �œ��[�o�x 							*/
	UI_8				timeLogStart[SD_CTRL_FMT_UTC_LEN+4];	/* ���O�������݊J�n���� 	*/
	UI_8				timeLogEnd[SD_CTRL_FMT_UTC_LEN+4];		/* ���O�������ݏI������		*/
						/* �����̃t�H�[�}�b�g�iUTC) YYMMDDHHMMSS(��151110154322�j */
	UI_32				flags;						/* ���O��񏉊���/�X�V��Ԃ������t���O	*/
} T_SD_CTRL_LOG_DATA_INFO;

/* �p�P�b�g��� */
typedef struct {
	UI_16				packet_bytes;					/*	�p�P�b�g�S�̂̒��� 7E=>7F		*/
	UI_16				length;							/*	�w�b�_���璊�o���� Length		*/
	UI_8				type;							/*	�w�b�_���璊�o���� Type			*/
	UI_8				rsv[3];
	UI_8*				header_part;					/*	�o�b�t�@���̃w�b�_�擪			*/
	UI_8*				data_part;						/*	�o�b�t�@���̃f�[�^�擪			*/
	UI_8*				footer_part;					/*	�o�b�t�@���̃t�b�^�擪			*/
	UI_8				packet[SD_CTRL_REC_MAX_PACKET_SIZE];	
														/*	�p�P�b�g�o�b�t�@				*/
} T_SD_CTRL_PACKET_INFO;

/* �V�~�����[�V���������� */
typedef struct t_sd_sim_ctrl_info {
	UI_32					flags;								/* �V�~�����[�V�����i�s�t���O				*/
	T_SD_CTRL_PACKET_INFO	packet_info;						/* �p�P�b�g���i�V�~�����[�V�����f�[�^�j	*/
	UI_32					time_flags;							/* �L���Ȏ��ԃt���O							*/
	UI_32					time[SD_SIM_TIME_KIND_COUNT];		/* ��ʖ��̎��ԋL�^�̈�						*/
	UI_32					sim_total_write_size;				/* �o�b�t�@�������ݍσT�C�Y�i�ʎZ�j			*/
	UI_32					sim_total_read_size;				/* �o�b�t�@�ǂݍ��ݍσT�C�Y�i�ʎZ�j			*/
	UI_32					sim_file_size;						/* �V�~�����[�V�����t�@�C���S�̃T�C�Y		*/
	UI_32					sim_file_read_size;					/* �P�t�@�C������READ�σT�C�Y				*/
	UI_32					sim_file_part_size[SIM_FILE_PART_COUNT];
																/* �V�~�����[�V�����t�@�C�������T�C�Y		*/
	UI_8					sim_file_path[((((FIO_PATH_LEN_MAX+1) + 3) / 4) * 4)];
																/* �V�~�����[�V�����t�@�C���p�X(4byte�A���C�����g����) */
} T_SD_SIM_CTRL_INFO;

/* �V�~�����[�V�����f�[�^ID��� */
typedef struct t_sd_ctrl_sim_id_info {
	UI_32				randomValue;							/* ����						*/
	UI_32				signalListVersion;						/* �M�����X�g�o�[�W����		*/
	UI_8				fixIdNum;								/* �Œ�ID�̐�				*/
	UI_8				varIdNum;								/* �ϐ�ID�̐�				*/
	UI_16				fixIdList[SD_CTRL_MAX_FIX_ID_COUNT];	/* �Œ�ID���X�g				*/
	UI_16				varIdList[SD_CTRL_MAX_VAR_ID_COUNT];	/* �ϐ�ID���X�g				*/
} T_SD_CTRL_SIM_ID_INFO;

/* SD������� */
typedef struct t_sd_ctrl_inspection_info {
	UI_8					insp_req_flg;								/* �����v���t���O				*/
	UI_8					fio_req_flg;								/* �t�@�C���A�N�Z�X�v���t���O	*/
	UI_16					serialno;									/* �V���A��No					*/
	E_SD_INSPECTION_STEP	step;										/* �����X�e�b�v					*/
	UI_8					filepath[FIO_PATH_LEN_MAX];					/* �t�@�C���p�X					*/
	UI_8					write_buf[SD_CTRL_INSPECTION_DATA_SIZE];	/* SD�������ݗp�o�b�t�@			*/
	UI_8					read_buf[SD_CTRL_INSPECTION_DATA_SIZE];		/* SD�ǂݍ��ݗp�o�b�t�@			*/

	E_FIO_RESULT			fio_result;									/* �t�@�C���A�N�Z�X����			*/
	UI_32					fio_bytes;									/* �t�@�C���ɏ������߂��T�C�Y	*/
} T_SD_CTRL_INSPECTION_INFO;


/* ��Ԋ֐� */
typedef E_SD_CTRL_STAT (*PFN_SD_STATE_PROC)( SD_CTRL_EVENT event );
typedef E_SD_SIM_STAT (*PFN_SD_SIM_STATE_PROC)( SD_SIM_EVENT event, B_8* fcontinue );

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
/* SD������(SD���W���[�������̏��) */
static E_SD_CTRL_STAT	S_sd_stat;
#ifdef _VDV_APP
static SD_PROCESS_TYPE		S_processType;
#endif /* _VDV_APP */

/* �t�@�C��IO����̒ʒm�t���O */
static UI_8	S_sd_fio_notify_get_mode;		/* ���[�h�t�@�C���擾�����ʒm�t���O */
static UI_8	S_sd_fio_notify_write;			/* REC�f�[�^�������݊����ʒm�t���O */
static UI_8	S_sd_fio_notify_inspection;		/* SD�����p�t�@�C���A�N�Z�X�����ʒm�t���O */

/* �V�X�e�����샂�[�h */
static UI_8 S_sd_sys_mode_req_flg;
static UI_8 S_sd_sys_mode_data[SD_CTRL_SYS_MODE_FILE_SIZE];
static E_SD_CTRL_SYS_MODE S_sd_sys_mode;
#ifdef _VDV_APP
/* REC�{�^��ON/OFF�ω��J�E���g */
static UI_32 S_sd_recBtnOnOffCnt;

/* REC�f�[�^�o�b�t�@ */
static UI_8 S_sd_rec_head[SD_CTRL_REC_HEAD_BUF_SIZE];
static UI_8 S_sd_rec_data[SD_CTRL_REC_DATA_BUF_SIZE];
static SI_32 S_sd_signal_buff[SD_CTRL_MAX_FIX_ID_COUNT+SD_CTRL_MAX_VAR_ID_COUNT];
static T_RINGBUFFER S_sd_rec_data_mng;							/* REC�f�[�^�����O�o�b�t�@�Ǘ����	*/
static UI_8 S_sd_rec_data_buff[(SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE * SD_CTRL_REC_DATA_WRITE_BLOCK_NUM)];
																/* REC�f�[�^�����O�o�b�t�@			*/
static UI_32 S_sd_ring_write_size;								/* �����O�o�b�t�@�ɏ�������REC�f�[�^�T�C�Y		*/
static UI_32 S_sd_ring_flush_size;								/* �����O�o�b�t�@����SD�ɏ������񂾃T�C�Y			*/

/* REC�f�[�^�������݉�	*/
static UI_32 S_sd_rec_write_req_cnt;							/* �������ݗv����					*/
static UI_32 S_sd_rec_write_notify_cnt;							/* �������ݒʒm��					*/

/* REC�f�[�^�������݌��ʒʒm���o�b�t�@ */
static T_RINGBUFFER S_sd_rec_write_notify_mng;
static T_SD_CTRL_WRITE_END_NOTIFY_INFO S_sd_rec_write_notify_buff[(sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO) * SD_CTRL_FIO_WRITE_NOTFY_NUM)];
static T_SD_CTRL_WRITE_END_NOTIFY_INFO S_sd_notifyinfo_set_buff;
static T_SD_CTRL_WRITE_END_NOTIFY_INFO S_sd_notifyinfo_get_buff;

/* REC�f�[�^�������݊Ǘ����	*/
static UI_32 S_sd_write_start_time;								/* REC�J�n����				*/
static UI_32 S_sd_update_header_time;							/* �w�b�_�X�V����(LAST)		*/

/* REC�f�[�^�t�@�C���p�X(4byte�A���C�����g����) */
static UI_8 S_sd_rec_filepath[((((FIO_PATH_LEN_MAX+1) + 3) / 4) * 4)];

/* ���O�f�[�^��� */
static T_SD_CTRL_LOG_DATA_INFO	S_sd_log_data_info;

/* �V�~�����[�V�����֘A��� */
static E_SD_SIM_STAT			S_sd_sim_status;					/* �V�~�����[�V�������						*/
static T_SD_SIM_CTRL_INFO		S_sd_sim_ctrl_info;					/* �V�~�����[�V����������					*/
static T_SD_CTRL_SIM_ID_INFO	S_sd_sim_id_info;					/* �V�~�����[�V�����f�[�^ID���				*/
/* �ԗ����i�M���f�[�^�j���̂� S_sd_signal_buff �ɋL�^ */
/*	S_sd_signal_buff[0]�`[S_sd_sim_fix_id_num - 1] */
/*		�� �Œ�ԗ���� */
/*	S_sd_signal_buff[S_sd_sim_fix_id_num]�`[S_sd_sim_fix_id_num + S_sd_sim_var_id_num - 1] */
/*		�� �ϐ��ԗ���� */

/* SD������� */
static T_SD_CTRL_INSPECTION_INFO	S_sd_inspection_info;

static UI_32	S_sd_running_distance;								/* ���s����					*/
static UI_32	S_sd_speed_stop_time;								/* ��Ԏ���					*/

#endif /* _VDV_APP */

/********************************************************/
/*	�����֐��錾										*/
/********************************************************/
/* �C�x���g����֐�	*/
static void sd_ctrl_state_MakeEvent( SD_PROCESS_TYPE processType, SD_CTRL_EVENT *event );

/* ��ԑJ�ڎ��֐� */
static void sd_ctrl_state_ProcOnEnter( E_SD_CTRL_STAT stat );
static void sd_ctrl_state_ProcOnLeave( E_SD_CTRL_STAT stat );

/* ��Ԋ֐�	*/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcInit( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcIdle( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcGetSysMode( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcRec( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcWaitRecEnd( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcSimulation( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcInspection( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcWaitShutdown( SD_CTRL_EVENT event );
static E_SD_CTRL_STAT sd_ctrl_stat_ProcShutdown( SD_CTRL_EVENT event );

/* �������֐�	*/
static void sd_ctrl_InitModuleParam( void );
#ifdef _VDV_APP
static void sd_ctrl_InitRecParam( void );
static void sd_ctrl_InitSimParam( void );
#endif /* _VDV_APP */

/* �V�X�e�����샂�[�h�擾�R�[���o�b�N�֐�	*/
static void sd_ctrl_SysModeCallback( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo );

#ifdef _VDV_APP
/* REC�֘A�֐�	*/
static void sd_ctrl_RecDataWriteCallback( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo );
static B_8 sd_ctrl_CheckWriteStartCnd( void );
static B_8 sd_ctrl_CheckWriteEndCnd( void );
static void sd_ctrl_MakeRecFilepath( void );
static void sd_ctrl_MakeRecHead( UI_8 *headBuff, UI_16 buffSize, UI_16 *headSize, B_8 init );
static void sd_ctrl_MakeRecData( UI_8 *dataBuff, UI_16 buffSize, UI_16 *dataSize );
static void sd_ctrl_MakeFileHeaderPart( UI_8 *headBuff, UI_16 buffSize, UI_16 *headSize );
static void sd_ctrl_MakeIdInfoDataPart( UI_8 signalNum, const UI_16 *idList, UI_8 *dataBuff, UI_16 buffSize, UI_16 *dataSize );
static void sd_ctrl_MakeSignalDataPart( UI_8 signalNum, const SI_32 *signalValues, UI_8 *dataBuff, UI_16 buffSize, UI_16 *dataSize );
static void sd_ctrl_UpdateLogDataInfo( B_8 init );
static UI_32 sd_ctrl_CheckGnssHnrInfo( const T_Gnss_ctrl_nav_pvt_info* hnr_info );
static void sd_ctrl_WriteRecData( UI_8 *dataBuff, UI_16 dataSize );
static void sd_ctrl_FlushRecData( B_8 allFlush );
static void sd_ctrl_UpdateHeader( void );
static void sd_ctrl_EndRec( void );
static UI_16 sd_ctrl_GetFileHeaderPartSize( UI_8 fixedSignalNum, UI_8 vehicleSignalNum );
static UI_16 sd_ctrl_GetIdInfoDataPartSize( UI_8 signalNum );
static UI_16 sd_ctrl_GetSignalDataPartSize( UI_8 signalNum, UI_8 errNum );

/* �V�~�����[�V��������֐�	*/
static void sd_sim_state_MainProc( void );
static void sd_sim_state_MakeEvent( SD_SIM_EVENT* sim_event );
static void sd_sim_state_ProcOnEnter( E_SD_SIM_STAT stat );
static void sd_sim_state_ProcOnLeave( E_SD_SIM_STAT stat );

/* �V�~�����[�V��������֐� */
static void sd_sim_InitSimCtrlInfo( E_SD_SIM_INIT_MODE mode );
static void sd_sim_SearchSimFile( void );
static void sd_sim_SearchSimFileCB( const T_COMMON_SRC_VDVF_RET *retinfo );
static void sd_sim_ReadSimFile( void );
static void sd_sim_ReadSimFileCB( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo );
static B_8 sd_sim_GetReadBlock( UI_8** block, UI_16* block_size );
static void sd_sim_RegTime( E_SD_SIM_TIME_KIND kind );
static void sd_sim_ClearTime( E_SD_SIM_TIME_KIND kind );
static SI_32 sd_sim_CheckTimeout( E_SD_SIM_TIME_KIND kind, UI_32 msec );
static void sd_sim_ClearReadError( void );

/* ��Ԋ֐�(�V�~�����[�V����)	*/
static E_SD_SIM_STAT sd_sim_stat_None( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_SearchSimFile( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_WaitHeader( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_WaitFixIdList( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_WaitFixValues( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_WaitVarIdList( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_WaitVarValues( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_WaitStop( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_WaitStopAccOff( SD_SIM_EVENT sim_event, B_8* fcontinue );
static E_SD_SIM_STAT sd_sim_stat_AccOff( SD_SIM_EVENT sim_event, B_8* fcontinue );

/* ��M�f�[�^��͊֐� */
static E_RC sd_sim_data_AnalyzeRecvData( T_SD_CTRL_PACKET_INFO* packet_info );
static SI_32 sd_sim_data_SkipToSTART( void );
static void sd_sim_data_ClearRecvPacket( T_SD_CTRL_PACKET_INFO* packet_info );
static SI_32 sd_sim_data_GetPacketInfo( T_SD_CTRL_PACKET_INFO* packet_info );
static UI_16 sd_sim_data_GetRecvDataSize( void );
static E_RC sd_sim_data_ReadRecvData( UI_16 size, UI_8* buff, UI_8 fclear );
static SI_32 sd_sim_data_GetIDList( T_SD_CTRL_PACKET_INFO* packet_info, UI_16* idList, UI_8 max_idNum );
static SI_32 sd_sim_data_GetValues( T_SD_CTRL_PACKET_INFO* packet_info, SI_32* values, UI_8 max_valueNum );

/* �M���f�[�^���� */
static void sd_sim_signal_ClearBuffer( B_8 clear_all );
static SI_32 sd_sim_signal_AnalyzeHeader( const T_SD_CTRL_PACKET_INFO* packet_info );
static SI_32 sd_sim_signal_SetIDList( B_8 fix, T_SD_CTRL_PACKET_INFO* packet_info );
static SI_32 sd_sim_signal_SetValues( B_8 fix, T_SD_CTRL_PACKET_INFO* packet_info );
static UI_32 sd_sim_signal_GetFixPartSize( void );

static void sd_ctrl_InitInspectionInfo(void);
static UI_8 sd_ctrl_Inspect(void);
static void sd_ctrl_InspectionFioCallback( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo );
static void sd_ctrl_InspectionDirCallback( const T_FIO_DIR_REQ_INFO *reqinfo, const T_FIO_DIR_RET_INFO *retinfo );


#endif /* _VDV_APP */

/* ��Ԋ֐��e�[�u��	*/
static const PFN_SD_STATE_PROC S_sd_state_proc[] = {
	sd_ctrl_stat_ProcInit,					/*	�������					*/
	sd_ctrl_stat_ProcIdle,					/*	�A�C�h��					*/
	sd_ctrl_stat_ProcGetSysMode,			/*	�V�X�e�����샂�[�h�擾		*/
	sd_ctrl_stat_ProcRec,					/*	REC��						*/
	sd_ctrl_stat_ProcWaitRecEnd,			/*	REC�I���҂�					*/
	sd_ctrl_stat_ProcSimulation,			/*	�V�~�����[�V������			*/
	sd_ctrl_stat_ProcInspection,			/*	SD����						*/
	sd_ctrl_stat_ProcWaitShutdown,			/*	SD���W���[����~�҂�		*/
	sd_ctrl_stat_ProcShutdown,				/*	SD���W���[����~			*/
};

#ifdef _VDV_APP
/* ��Ԋ֐��e�[�u��(�V�~�����[�V����)	*/
static const PFN_SD_SIM_STATE_PROC S_sd_sim_state_proc[] = {
	sd_sim_stat_None,
	sd_sim_stat_SearchSimFile,				/*	SIM�t�@�C��������		 	*/
	sd_sim_stat_WaitHeader,					/*	�t�@�C���w�b�_��M�҂�		*/
	sd_sim_stat_WaitFixIdList,				/*	�Œ�M��ID��M�҂�			*/
	sd_sim_stat_WaitFixValues,				/*	�Œ�ԗ�����M�҂�		*/
	sd_sim_stat_WaitVarIdList,				/*	�ϐ��M��ID��M�҂�			*/
	sd_sim_stat_WaitVarValues,				/*	�ԗ�����M�҂�			*/
	sd_sim_stat_WaitStop,					/*	��~�҂�					*/
	sd_sim_stat_WaitStopAccOff,				/*	ACC OFF�ɂ���~�҂�		*/
	sd_sim_stat_AccOff,						/*	ACC OFF						*/
};
#endif /* _VDV_APP */

/********************************************************/
/*	�O���֐�											*/
/********************************************************/

/********************************************************/
/* �֐�����		: Sd_control_Init						*/
/* �@�\�̐���	: SD���䏉����							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Sd_control_Init(void)
{
	/* ���W���[���Ŏg�p����ϐ������� */
	sd_ctrl_InitModuleParam();

	/* ������Ԑݒ�	*/
	S_sd_stat = SD_CTRL_STAT_INIT;

	/* SD�J�[�h�h���C�o������ */
	SDCardDrv_Init();
}

/********************************************************/
/* �֐�����		: Sd_control_Main						*/
/* �@�\�̐���	: SD���䃁�C��							*/
/* ����			: (I/ )processType �v���Z�X���			*/
/* 					SD_PROCESS_MAIN:�ʏ폈��	10ms��	*/
/* 					SD_PROCESS_SUB :�T�u����	25ms��	*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Sd_control_Main(SD_PROCESS_TYPE processType)
{
	SD_CTRL_EVENT		event;
	E_SD_CTRL_STAT		nextStat;
	
	/* �C�x���g�쐬 */
	sd_ctrl_state_MakeEvent( processType, &event );

#ifdef _VDV_APP
	/* �v���Z�X��ʂ��L�^ */
	S_processType = processType;
#endif /* _VDV_APP */

	/* ��Ԋ֐��R�[�� */
	nextStat = S_sd_state_proc[S_sd_stat]( event );
	if (S_sd_stat != nextStat) {			/* ��ԑJ�ڂ���		*/
		OUTLOG(OutputLog("[SdCtrl]stat:%d->%d\n", S_sd_stat, nextStat);)
		sd_ctrl_state_ProcOnLeave(S_sd_stat);
		S_sd_stat = nextStat;
		sd_ctrl_state_ProcOnEnter(nextStat);
	}

	return;
}

/************************************************************/
/* �֐�����		: Sd_control_GetRecStatus					*/
/* �@�\�̐���	: SD REC��Ԏ擾							*/
/* ����			: void										*/
/* �߂�l		: �������� E_SD_CONTROL_REC_STAT			*/
/* �쐬��		: (OFT)Sakata								*/
/* �쐬��		: 16/08/18									*/
/* ���l			: 											*/
/************************************************************/
E_SD_CONTROL_REC_STAT Sd_control_GetRecStatus(void)
{
	E_SD_CONTROL_REC_STAT recStat;

	if (S_sd_stat == SD_CTRL_STAT_INIT) {
		recStat = SD_CONTROL_REC_STAT_UNKNOWN;
	}
	else if ((S_sd_stat == SD_CTRL_STAT_REC) ||
			 (S_sd_stat == SD_CTRL_STAT_WAIT_REC_END)) {
		recStat = SD_CONTROL_REC_STAT_RECORDING;
	}
	else {
		recStat = SD_CONTROL_REC_STAT_UNRECORDING;
	}
	return recStat;
}

/********************************************************/
/* �֐�����		: Sd_control_GetSignalList				*/
/* �@�\�̐���	: ���J�M��ID�ꗗ�擾					*/
/* ����			: (I/ )idtype			�M�����		*/
/*				  ( /O)signal_Num		�M����			*/
/*				  ( /O)signal_List		�M�����X�g		*/
/* �߂�l		: ����	:N_OK							*/
/*				  �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Sd_control_GetSignalList(E_SIGNAL_ID_TYPE idtype, UI_8 *signal_Num, const UI_16 **signal_List)
{
#ifdef _VDV_APP
	SI_8	ret = N_NG;

	if( (NULL != signal_Num) && (NULL != signal_List) ) {
		*signal_Num = 0;
		*signal_List = NULL;
		if( (0 < S_sd_sim_id_info.fixIdNum) && (0 < S_sd_sim_id_info.varIdNum) ) {
			switch( idtype ) {
			case ID_TYPE_FIXED_ID:		/**< �M�����:�Œ�l	*/
				*signal_Num = S_sd_sim_id_info.fixIdNum;
				*signal_List = S_sd_sim_id_info.fixIdList;
				ret = N_OK;
				break;

			case ID_TYPE_VARIABLE_ID:	/**< �M�����:�ϐ��l	*/
				*signal_Num = S_sd_sim_id_info.varIdNum;
				*signal_List = S_sd_sim_id_info.varIdList;
				ret = N_OK;
				break;

			default:
				break;
			}
		}
	}
	return ret;
#else /* _VDV_APP */
	return N_NG;
#endif /* _VDV_APP */
}

/********************************************************/
/* �֐�����		: Sd_control_GetEncodeKey				*/
/* �@�\�̐���	: �Í����L�[�擾						*/
/* ����			: ( /O)key				�Í����p�L�[	*/
/* �߂�l		: ����	:N_OK							*/
/*				  �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Sd_control_GetEncodeKey(SI_32 *key)
{
#ifdef _VDV_APP
	*key = (SI_32)S_sd_sim_id_info.randomValue;
	return N_OK;
#else /* _VDV_APP */
	*key = 0;
	return N_NG;
#endif /* _VDV_APP */
}


/********************************************************/
/* �֐�����		: Sd_control_GetSignalVersion			*/
/* �@�\�̐���	: ���J�M���o�[�W����					*/
/* ����			: void									*/
/* �߂�l		: ���J�M���o�[�W����					*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
UI_32 Sd_control_GetSignalVersion(void)
{
#ifdef _VDV_APP
	return S_sd_sim_id_info.signalListVersion;
#else /* _VDV_APP */
	return 0;
#endif /* _VDV_APP */
}

/********************************************************/
/* �֐�����		: Sd_control_GetSignalsInfo				*/
/* �@�\�̐���	: ���J�M���擾(�����M��)				*/
/* ����			: (I/ )req_id_count	�M��ID��			*/
/* 				: (I/ )req_ids 		�M��ID�i�����j		*/
/*				  ( /O)signal_values	�M���l�i�����j	*/
/* �߂�l		: ����	:N_OK							*/
/*				  �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
SI_8 Sd_control_GetSignalsInfo( UI_8 req_id_count, const UI_16* req_ids, SI_32 *signal_values)
{
#ifdef _VDV_APP
	SI_8			ret = N_NG;
	UI_8			i = 0U;
	UI_8			j = 0U;
	UI_16			req_id = 0U;
	UI_16			fixIdNum = 0U;
	UI_16			varIdNum = 0U;
	const UI_16*	fixIdList = NULL;
	const UI_16*	varIdList = NULL;
	B_8				find = N_FALSE;
	SI_32			value = 0L;

	if( (NULL != req_ids) && (NULL != signal_values) ) {
		fixIdNum = S_sd_sim_id_info.fixIdNum;
		varIdNum = S_sd_sim_id_info.varIdNum;
		fixIdList = S_sd_sim_id_info.fixIdList;
		varIdList = S_sd_sim_id_info.varIdList;

		/* �v���M��ID���̏������s�� */
		for( i = 0U; i < req_id_count; i++ ) {
			find = N_FALSE;
			req_id = req_ids[i];
			value = (SI_32)SD_SIM_SIGNAL_INVALID_VALUE;

			/* �Œ�ID�z������� */
			for( j = 0U; j < fixIdNum; j++ ) {
				if( fixIdList[j] == req_id ) {
					/* ID����v���� */
					value = S_sd_signal_buff[j];
					find = N_TRUE;
					break;
				}
			}
			if( N_TRUE != find ) {
				/* �ϐ�ID�z������� */
				for( j = 0U; j < varIdNum; j++ ) {
					if( varIdList[j] == req_id ) {
						/* ID����v���� */
						value = S_sd_signal_buff[fixIdNum + j];
						break;
					}
				}
			}
			signal_values[i] = value;
		}
		ret = N_OK;
	}
	return ret;
#else /* _VDV_APP */
	return N_NG;
#endif /* _VDV_APP */
}

#ifdef _VDV_APP
/****************************************************************************/
/* �֐�����		: Sd_control_GetInspectionVariation							*/
/* �@�\�̐���	: �������[�h�̃o���G�[�V�������擾						*/
/* ����			: variation	�o���G�[�V�������								*/
/* �߂�l		: ����	:N_OK												*/
/*				  �ُ�	:N_NG												*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 17/01/20													*/
/* ���l			: 															*/
/****************************************************************************/
SI_8 Sd_control_GetInspectionVariation(UI_8 *variation)
{
	if (variation == NULL) {
		return N_NG;
	}

	if (S_sd_sys_mode_data[0] != SD_CTRL_SYS_MODE_FILE_VALUE_INSPECTION) {
		return N_NG;
	}

	*variation = S_sd_sys_mode_data[1];

	return N_OK;
}

/****************************************************************************/
/* �֐�����		: Sd_control_ReqInspection									*/
/* �@�\�̐���	: SD�����v��												*/
/* ����			: serialno		�V���A��No									*/
/* �߂�l		: ����	:N_OK												*/
/*				  �ُ�	:N_NG												*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 17/01/20													*/
/* ���l			: �{�֐�������I�������ꍇ�A�������䃂�W���[���ɑ΂���		*/
/*				  Insp_notify_inspection_result�ɂ�茟�����ʂ�ʒm����		*/
/****************************************************************************/
SI_8 Sd_control_ReqInspection(UI_16 serialno)
{
	E_SYS_CTRL_STATUS	sysStat;
	UI_32	filepathlen;

	/* �V�X�e����Ԏ擾	*/
	sysStat = System_control_GetStatus();
	if (sysStat != SYS_OPERATION_INSPECTION) {
		/* ������Ԃł͂Ȃ�	*/
		return N_NG;
	}

	if (S_sd_inspection_info.insp_req_flg == N_ON) {
		/* �������s��	*/
		return N_NG;
	}

	/* ��������ݒ�	*/
	sd_ctrl_InitInspectionInfo();
	S_sd_inspection_info.insp_req_flg = N_ON;
	S_sd_inspection_info.serialno = serialno;
	S_sd_inspection_info.filepath[0] = '\0';
	(void)strcpy((CH_8*)S_sd_inspection_info.filepath, SD_CTRL_INSPECTION_DIR);
	(void)strcat((CH_8*)S_sd_inspection_info.filepath, "\\");
	filepathlen = strlen((const CH_8*)S_sd_inspection_info.filepath);
	Common_utoa((UI_32)serialno, &S_sd_inspection_info.filepath[filepathlen]);
	(void)strcat((CH_8*)S_sd_inspection_info.filepath, ".dat");

	return N_OK;
}
#endif /* _VDV_APP */

/****************************************************************************/
/*	�����֐�																*/
/****************************************************************************/
/****************************************************************************/
/* �֐�����		:	sd_ctrl_state_MakeEvent									*/
/* �@�\�̐���	:	�C�x���g�쐬											*/
/* ����			:	(I/ )processType	�v���Z�X���						*/
/* 							SD_PROCESS_MAIN:�ʏ폈��	10ms��				*/
/* 							SD_PROCESS_SUB :�T�u����	25ms��				*/
/*					( /O)event			�C�x���g							*/
/* �߂�l		:	SD_CTRL_STAT_xxx	�J�ڐ���							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_ctrl_state_MakeEvent( SD_PROCESS_TYPE processType, SD_CTRL_EVENT *event )
{
	E_SYS_CTRL_STATUS	sysStat;

	if (event == NULL) {
		return;
	}

	*event = SD_CTRL_EVENT_NONE;

	sysStat = System_control_GetStatus();

	switch ( sysStat ) {
	case SYS_OPERATION_WAIT_ACC_ON:				/* �V�X�e��������:ACC ON�҂�	--------------------*/
		/* �������Ȃ� */
		break;

	case SYS_OPERATION_ACC_ON_UNKNOWN_STAT:		/* �V�X�e��������:ACC ON�i��Ԕ���j	------------*/
		if ((S_sd_stat == SD_CTRL_STAT_INIT) ||			/* SD������:�������				*/
			(S_sd_stat == SD_CTRL_STAT_SHUTDOWN)) {		/* SD������:SD���W���[����~		*/
			/* SD���W���[���N���v��	*/
			*event |= SD_CTRL_EVENT_REQ_STARTUP;
		}
		else if (S_sd_stat == SD_CTRL_STAT_IDLE) {			/* SD������:�A�C�h��				*/
			if (S_sd_sys_mode == SD_CTRL_SYS_MODE_UNKNOWN) {			/* �V�X�e�����샂�[�h�s���i���[�h�t�@�C�����擾�j	*/
				if (FileIO_Get_MountExecStat((const UI_8*)FS_DRV_PATH_SD) == N_TRUE) {	/* SD�}�E���g���s�ς�								*/
					/* �V�X�e�����샂�[�h�擾�v��	*/
					*event |= SD_CTRL_EVENT_REQ_GET_SYSMODE;
				}
			}
		}
		else if (S_sd_stat == SD_CTRL_STAT_GET_SYSMODE) {	/* SD������:�V�X�e�����샂�[�h�擾	*/
			/* �������Ȃ�	*/
		}
		else if (S_sd_stat == SD_CTRL_STAT_WAIT_SHUTDOWN) {	/* SD������:SD���W���[����~�҂�	*/
#ifdef _VDV_APP
			if ((S_sd_sys_mode_req_flg == N_OFF) &&							/* �V�X�e�����샂�[�h�擾���ł͂Ȃ�	*/
				(S_sd_inspection_info.fio_req_flg == N_OFF) &&				/* SD�����t�@�C���A�N�Z�X���ł͂Ȃ�	*/
				(S_sd_rec_write_req_cnt <= S_sd_rec_write_notify_cnt)) {	/* REC���ł͂Ȃ�					*/
#else /* _VDV_APP */
			if (S_sd_sys_mode_req_flg == N_OFF) {							/* �V�X�e�����샂�[�h�擾���ł͂Ȃ�	*/
#endif /* _VDV_APP */
				/* SD���W���[���N���v��	*/
				*event |= SD_CTRL_EVENT_REQ_STARTUP;
			}
		}
#ifdef _VDV_APP
		else if (S_sd_stat == SD_CTRL_STAT_SIMULATION) {	/* SD������:�V�~�����[�V������	*/
			/* �������Ȃ�	*/
		}
#endif /* _VDV_APP */
		else {
			/* ��ԕs�� */
			OUTLOG(OutputLog("[SdCtrl]Stat ERR!! SysStat:%d SdStat:%d\n", sysStat, S_sd_stat);)
			/* SD���W���[������x��~���ĕ��A������	*/
			/* SD���W���[����~�v��	*/
			*event |= SD_CTRL_EVENT_REQ_SHUTDOWN;
		}
		break;

#ifdef _VDV_APP
	case SYS_OPERATION_NORMAL_OFF:				/* �V�X�e��������:�ʏ�:OFF���[�h	----------------*/
		if (S_sd_stat == SD_CTRL_STAT_REC) {				/* SD������:REC��				*/
			/* REC��~�v��	*/
			*event |= SD_CTRL_EVENT_REQ_STOP_REC;
		}
		else if (S_sd_stat == SD_CTRL_STAT_IDLE) {			/* SD������:�A�C�h��				*/
			/* �������Ȃ�	*/
		}
		else if (S_sd_stat == SD_CTRL_STAT_WAIT_REC_END) {	/* SD������:REC�I���҂�			*/
			if (S_sd_rec_write_req_cnt <= S_sd_rec_write_notify_cnt) {	/* REC����		*/
				/* REC��~����	*/
				*event |= SD_CTRL_EVENT_NOTIFY_STOP_REC;
			}
		}
		else if ((S_sd_stat == SD_CTRL_STAT_INIT) ||		/* SD������:�������				*/
				(S_sd_stat == SD_CTRL_STAT_SHUTDOWN)) {		/* SD������:SD���W���[����~		*/
			/* ��ԕs�� */
			OUTLOG(OutputLog("[SdCtrl]Stat ERR!! SysStat:%d SdStat:%d\n", sysStat, S_sd_stat);)
			/* SD���W���[�����N������	*/
			/* SD���W���[���N���v��	*/
			*event |= SD_CTRL_EVENT_REQ_STARTUP;
		}
		else if (S_sd_stat == SD_CTRL_STAT_WAIT_SHUTDOWN) {	/* SD������:SD���W���[����~�҂�	*/
			/* ��ԕs�� */
			OUTLOG(OutputLog("[SdCtrl]Stat ERR!! SysStat:%d SdStat:%d\n", sysStat, S_sd_stat);)
			if ((S_sd_sys_mode_req_flg == N_OFF) &&							/* �V�X�e�����샂�[�h�擾���ł͂Ȃ�	*/
				(S_sd_rec_write_req_cnt <= S_sd_rec_write_notify_cnt)) {	/* REC���ł͂Ȃ�					*/
				/* SD���W���[���N���v��	*/
				*event |= SD_CTRL_EVENT_REQ_STARTUP;
			}
			else {
				/* �V�X�e�����샂�[�h�擾 �܂��� REC�����܂ő҂��߂����ł͉������Ȃ� */
			}
		}
		else {
			/* ��ԕs�� */
			OUTLOG(OutputLog("[SdCtrl]Stat ERR!! SysStat:%d SdStat:%d\n", sysStat, S_sd_stat);)
			/* SD���W���[������x��~���ĕ��A������	*/
			/* SD���W���[����~�v��	*/
			*event |= SD_CTRL_EVENT_REQ_SHUTDOWN;
		}
		break;

	case SYS_OPERATION_NORMAL_AUTO:				/* �V�X�e��������:�ʏ�:�I�[�gREC	----------------*/
	case SYS_OPERATION_NORMAL_MANUAL:			/* �V�X�e��������:�ʏ�:�}�j���A��REC	------------*/
		if (S_sd_stat == SD_CTRL_STAT_IDLE) {				/* SD������:�A�C�h��				*/
			if (sd_ctrl_CheckWriteStartCnd() == N_TRUE) {	/* SD�������݊J�n�����Ɉ�v����	*/
				/* REC�J�n�v��	*/
				*event |= SD_CTRL_EVENT_REQ_START_REC;
			}
		}
		else if (S_sd_stat == SD_CTRL_STAT_REC) {			/* SD������:REC��				*/
			if (sd_ctrl_CheckWriteEndCnd() == N_TRUE) {		/* SD�������ݒ�~�����Ɉ�v����	*/
				/* REC��~�v��	*/
				*event |= SD_CTRL_EVENT_REQ_STOP_REC;
			}
			else {
				if (processType == SD_PROCESS_SUB) {
					/* REC�f�[�^�������ݗv��	*/
					*event |= SD_CTRL_EVENT_REQ_WRITE_REC_DATA;
				}
			}
		}
		else if (S_sd_stat == SD_CTRL_STAT_WAIT_REC_END) {	/* SD������:REC�I���҂�			*/
			if (S_sd_rec_write_req_cnt <= S_sd_rec_write_notify_cnt) {	/* REC����		*/
				/* REC��~����	*/
				*event |= SD_CTRL_EVENT_NOTIFY_STOP_REC;
			}
		}
		else if ((S_sd_stat == SD_CTRL_STAT_INIT) ||		/* SD������:�������				*/
				(S_sd_stat == SD_CTRL_STAT_SHUTDOWN)) {		/* SD������:SD���W���[����~		*/
			/* ��ԕs�� */
			OUTLOG(OutputLog("[SdCtrl]Stat ERR!! SysStat:%d SdStat:%d\n", sysStat, S_sd_stat);)
			/* SD���W���[�����N������	*/
			/* SD���W���[���N���v��	*/
			*event |= SD_CTRL_EVENT_REQ_STARTUP;
		}
		else if (S_sd_stat == SD_CTRL_STAT_WAIT_SHUTDOWN) {	/* SD������:SD���W���[����~�҂�	*/
			/* ��ԕs�� */
			OUTLOG(OutputLog("[SdCtrl]Stat ERR!! SysStat:%d SdStat:%d\n", sysStat, S_sd_stat);)
			/* SD���W���[�����N������	*/
			if ((S_sd_sys_mode_req_flg == N_OFF) &&							/* �V�X�e�����샂�[�h�擾���ł͂Ȃ�	*/
				(S_sd_rec_write_req_cnt <= S_sd_rec_write_notify_cnt)) {	/* REC���ł͂Ȃ�					*/
				/* SD���W���[���N���v��	*/
				*event |= SD_CTRL_EVENT_REQ_STARTUP;
			}
		}
		else {
			/* ��ԕs�� */
			OUTLOG(OutputLog("[SdCtrl]Stat ERR!! SysStat:%d SdStat:%d\n", sysStat, S_sd_stat);)
			/* SD���W���[������x��~���ĕ��A������	*/
			/* SD���W���[����~�v��	*/
			*event |= SD_CTRL_EVENT_REQ_SHUTDOWN;
		}
		break;
#endif /* _VDV_APP */

	case SYS_OPERATION_ERROR:					/* �V�X�e��������:�ُ�	------------------------*/
#ifdef _VDV_APP
		if (S_sd_stat == SD_CTRL_STAT_REC) {				/* SD������:REC��				*/
			/* REC��~�v��	*/
			*event |= SD_CTRL_EVENT_REQ_STOP_REC;
		}
#endif /* _VDV_APP */
		break;

#ifdef _VDV_APP
	case SYS_OPERATION_SIMULATION:				/* �V�X�e��������:�V�~�����[�V����	------------*/
		if (S_sd_stat == SD_CTRL_STAT_IDLE) {				/* SD������:�A�C�h��				*/
			/* �V�~�����[�V�����J�n�v��	*/
			*event |= SD_CTRL_EVENT_REQ_START_SIM;
		}
		break;

	case SYS_OPERATION_INSPECTION:				/* �V�X�e��������:����				------------*/
		if (S_sd_stat == SD_CTRL_STAT_IDLE) {				/* SD������:�A�C�h��				*/
			if (S_sd_inspection_info.insp_req_flg == N_ON) {
				/* SD�����v��	*/
				*event |= SD_CTRL_EVENT_REQ_INSPECTION;
			}
		}
		else if (S_sd_stat == SD_CTRL_STAT_INSPECTION) {	/* SD������:SD����				*/
			if (S_sd_inspection_info.step == SD_INSPECTION_STEP_INVALID) {
				/* SD�����J�n�v��	*/
				*event |= SD_CTRL_EVENT_REQ_INSPECTION_START;
			}
		}
		else {
			/* nop */
		}
		break;

	case SYS_OPERATION_EEPROM_WRITE:			/* �V�X�e��������:EEPROM������	----------------*/
		/* �������Ȃ� */
		break;

	case SYS_OPERATION_EEPROM_WRITE_COMP:		/* �V�X�e��������:EEPROM��������	----------------*/
		/* �������Ȃ� */
		break;
#endif /* _VDV_APP */

	case SYS_OPERATION_FIRMWARE_WRITE:			/* �V�X�e��������:�t�@�[���E�F�A������	--------*/
		/* �������Ȃ� */
		break;

	case SYS_OPERATION_FIRMWARE_WRITE_COMP:		/* �V�X�e��������:�t�@�[���E�F�A��������	--------*/
		/* �������Ȃ� */
		break;

	case SYS_OPERATION_PRE_ACC_OFF:				/* �V�X�e��������:ACC OFF�ڍs	--------*/
		if ((S_sd_stat == SD_CTRL_STAT_IDLE) ||				/* SD������:�A�C�h��					*/
			(S_sd_stat == SD_CTRL_STAT_GET_SYSMODE) ||		/* SD������:�V�X�e�����샂�[�h�擾	*/
			(S_sd_stat == SD_CTRL_STAT_REC) ||				/* SD������:REC��					*/
			(S_sd_stat == SD_CTRL_STAT_WAIT_REC_END) ||		/* SD������:REC�I���҂�				*/
			(S_sd_stat == SD_CTRL_STAT_INSPECTION) ||		/* SD������:SD����					*/
			(S_sd_stat == SD_CTRL_STAT_SIMULATION)) {		/* SD������:�V�~�����[�V������		*/
			/* SD���W���[����~�v��	*/
			*event |= SD_CTRL_EVENT_REQ_SHUTDOWN;
		}
		else if (S_sd_stat == SD_CTRL_STAT_WAIT_SHUTDOWN) {	/* SD������:SD���W���[����~�҂�		*/
#ifdef _VDV_APP
			if ((S_sd_sys_mode_req_flg == N_OFF) &&							/* �V�X�e�����샂�[�h�擾���ł͂Ȃ�	*/
				(S_sd_inspection_info.fio_req_flg == N_OFF) &&				/* SD�����t�@�C���A�N�Z�X���ł͂Ȃ�	*/
				(S_sd_rec_write_req_cnt <= S_sd_rec_write_notify_cnt)) {	/* REC���ł͂Ȃ�					*/
#else /* _VDV_APP */
			if (S_sd_sys_mode_req_flg == N_OFF) {							/* �V�X�e�����샂�[�h�擾���ł͂Ȃ�	*/
#endif /* _VDV_APP */
				/* SD���W���[����~����	*/
				*event |= SD_CTRL_EVENT_NOTIFY_SHUTDOWN;
			}
		}
		else {
			/* nop */
		}
		break;

	case SYS_OPERATION_ACC_OFF:					/* �V�X�e��������:ACC OFF	--------*/
		/* �������Ȃ� */
		break;
		
	default:
		/* �������Ȃ� */
		break;
	}

	if (S_sd_fio_notify_get_mode == N_ON) {	/* ���샂�[�h�擾�����ʒm���� */
		/* �V�X�e�����샂�[�h�擾�����ʒm */
		*event |= SD_CTRL_EVENT_NOTIFY_GET_SYSMODE;
		S_sd_fio_notify_get_mode = N_OFF;
	}
	if (S_sd_fio_notify_write == N_ON) {	/* REC�f�[�^�������݊����ʒm���� */
		/* REC�f�[�^�������݌��ʒʒm	*/
		*event |= SD_CTRL_EVENT_NOTIFY_WRITE_RESULT;
		S_sd_fio_notify_write = N_OFF;
	}
	if (S_sd_fio_notify_inspection == N_ON) {	/* SD�����p�t�@�C���A�N�Z�X�����ʒm���� */
		/* SD�����p�A�N�Z�X���ʒʒm	*/
		*event |= SD_CTRL_EVENT_NOTIFY_INSPECTION_SD_RES;
		S_sd_fio_notify_inspection = N_OFF;
	}

	return;
}

/****************************************************************************/
/* �֐�����		:	btc_state_ProcOnEnter									*/
/* �@�\�̐���	:	��Ԃɓ���Ƃ��̏���									*/
/* ����			:	���ꂩ�������										*/
/* �߂�l		:	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_ctrl_state_ProcOnEnter( E_SD_CTRL_STAT stat )
{

	switch( stat ) {
	case SD_CTRL_STAT_IDLE:					/*	�A�C�h��				*/
		break;

	case SD_CTRL_STAT_GET_SYSMODE:			/*	�V�X�e�����샂�[�h�擾	*/
		break;

	case SD_CTRL_STAT_REC:					/*	REC��					*/
#ifdef _VDV_APP
		/* REC�֘A�ϐ������� */
		sd_ctrl_InitRecParam();
#endif /* _VDV_APP */
		break;

	case SD_CTRL_STAT_WAIT_REC_END:			/*	REC�I���҂�				*/
		break;

	case SD_CTRL_STAT_SIMULATION:			/*	�V�~�����[�V������		*/
#ifdef _VDV_APP
		/* �V�~�����[�V�����֘A�ϐ������� */
		sd_ctrl_InitSimParam();
#endif /* _VDV_APP */
		break;

	case SD_CTRL_STAT_WAIT_SHUTDOWN:		/*	SD���W���[����~�҂�	*/
		break;

	case SD_CTRL_STAT_SHUTDOWN:				/*	SD���W���[����~		*/
		break;
	
	default:
		break;
	}

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_state_ProcOnLeave								*/
/* �@�\�̐���	:	��Ԃ���o��Ƃ��̏���									*/
/* ����			:	���ꂩ��o����										*/
/* �߂�l		:	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_ctrl_state_ProcOnLeave( E_SD_CTRL_STAT stat )
{

	switch( stat ) {
	case SD_CTRL_STAT_IDLE:					/*	�A�C�h��				*/
		break;

	case SD_CTRL_STAT_GET_SYSMODE:			/*	�V�X�e�����샂�[�h�擾	*/
		break;

	case SD_CTRL_STAT_REC:					/*	REC��					*/
#ifdef _VDV_APP
		/* Rec�I������	*/
		sd_ctrl_EndRec();
#endif /* _VDV_APP */
		break;

	case SD_CTRL_STAT_WAIT_REC_END:			/*	REC�I���҂�				*/
		break;

	case SD_CTRL_STAT_SIMULATION:			/*	�V�~�����[�V������		*/
		break;

	case SD_CTRL_STAT_WAIT_SHUTDOWN:		/*	SD���W���[����~�҂�	*/
		/* �p�����^������	*/
		sd_ctrl_InitModuleParam();
		break;

	case SD_CTRL_STAT_SHUTDOWN:				/*	SD���W���[����~		*/
		/* �p�����^������	*/
		sd_ctrl_InitModuleParam();
		break;
	
	default:
		break;
	}

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_stat_ProcInit									*/
/* �@�\�̐���	:	��Ԋ֐�:SD_CTRL_STAT_INIT								*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	SD_CTRL_STAT_xxx	�J�ڐ���							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcInit( SD_CTRL_EVENT event )
{

	E_SD_CTRL_STAT nextStat = SD_CTRL_STAT_INIT;

	if ((event & SD_CTRL_EVENT_REQ_STARTUP) != 0) {			/* SD���W���[���N���v��				*/
		/* �J�ڐ���:�A�C�h��	*/
		nextStat = SD_CTRL_STAT_IDLE;
	}

	return nextStat;

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_stat_ProcIdle									*/
/* �@�\�̐���	:	��Ԋ֐�:SD_CTRL_STAT_IDLE								*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	SD_CTRL_STAT_xxx	�J�ڐ���							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcIdle( SD_CTRL_EVENT event )
{

	E_SD_CTRL_STAT	nextStat = SD_CTRL_STAT_IDLE;
	E_FIO_RESULT	fioRet;
	UI_32			reqNo;

	if ((event & SD_CTRL_EVENT_REQ_SHUTDOWN) != 0) {			/* SD���W���[����~�v��				*/
		/* �J�ڐ���:SD���W���[����~	*/
		nextStat = SD_CTRL_STAT_WAIT_SHUTDOWN;
	}
	else if ((event & SD_CTRL_EVENT_REQ_GET_SYSMODE) != 0) {	/* �V�X�e�����샂�[�h�擾�v��		*/

		S_sd_fio_notify_get_mode = N_OFF;		/* �V�X�e�����샂�[�h�擾�����ʒm�t���O�N���A	*/

		/* �V�X�e�����샂�[�h�t�@�C���ǂݍ��� */
		fioRet = FileIO_Read( (const UI_8*)SD_CTRL_SYS_MODE_FILE_PATH, S_sd_sys_mode_data, 0, SD_CTRL_SYS_MODE_FILE_SIZE, sd_ctrl_SysModeCallback, &reqNo );
		if (fioRet != FIO_RESULT_OK) {								/* ���[�h�t�@�C���ǂݍ��ݎ��s	*/

			/* ���[�h:�ʏ� */
			S_sd_sys_mode = SD_CTRL_SYS_MODE_NOMAL;

			/* �V�X�e�����샂�[�h�擾�����ʒm�����ݒ� */
			S_sd_fio_notify_get_mode = N_ON;

			/* �V�X�e�����샂�[�h�擾�v���t���OOFF	*/
			S_sd_sys_mode_req_flg = N_OFF;
		}
		else {
			/* �V�X�e�����샂�[�h�擾�v���t���OON	*/
			S_sd_sys_mode_req_flg = N_ON;
		}

		/* �J�ڐ���:�V�X�e�����샂�[�h�擾	*/
		nextStat = SD_CTRL_STAT_GET_SYSMODE;
	}
	else if ((event & SD_CTRL_EVENT_REQ_START_REC) != 0) {	/* REC�J�n�v��						*/
		/* �J�ڐ���:REC��	*/
		nextStat = SD_CTRL_STAT_REC;
	}
	else if ((event & SD_CTRL_EVENT_REQ_START_SIM) != 0) {	/* �V�~�����[�V�����J�n�v��			*/
		/* �J�ڐ���:�V�~�����[�V������	*/
		nextStat = SD_CTRL_STAT_SIMULATION;
	}
	else if ((event & SD_CTRL_EVENT_REQ_INSPECTION) != 0) {	/* SD����							*/
		/* �J�ڐ���:SD����	*/
		nextStat = SD_CTRL_STAT_INSPECTION;
	}
	else {
		/* nop */
	}

	return nextStat;

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_stat_ProcGetSysMode								*/
/* �@�\�̐���	:	��Ԋ֐�:SD_CTRL_STAT_GET_SYSMODE						*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	SD_CTRL_STAT_xxx	�J�ڐ���							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcGetSysMode( SD_CTRL_EVENT event )
{

	E_SD_CTRL_STAT			nextStat = SD_CTRL_STAT_GET_SYSMODE;
	E_SYS_CTRL_EVENT_PARAM	eventParam;

	if ((event & SD_CTRL_EVENT_REQ_SHUTDOWN) != 0) {			/* SD���W���[����~�v��				*/
		/* �J�ڐ���:SD���W���[����~	*/
		nextStat = SD_CTRL_STAT_WAIT_SHUTDOWN;
	}
	else if ((event & SD_CTRL_EVENT_NOTIFY_GET_SYSMODE) != 0) {		/* �V�X�e�����샂�[�h�擾�����ʒm			*/
		/* �V�X�e������ɓ��샂�[�h��ʒm���� */
		switch (S_sd_sys_mode) {
		case SD_CTRL_SYS_MODE_NOMAL:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_NORMAL;			/* ���샂�[�h:�ʏ�					*/
			break;
		case SD_CTRL_SYS_MODE_SIMULATION:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_SIMULATION;		/* ���샂�[�h:�V�~�����[�V����		*/
			break;
		case SD_CTRL_SYS_MODE_EEPROM:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_EEPROM;			/* ���샂�[�h:EEPROM����			*/
			break;
		case SD_CTRL_SYS_MODE_FIRMWARE:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_FIRMWARE;			/* ���샂�[�h:�t�@�[���E�F�A����	*/
			break;
		case SD_CTRL_SYS_MODE_INSPECTION:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_INSPECTION;		/* ���샂�[�h:����					*/
			break;
		case SD_CTRL_SYS_MODE_BT_COMM:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_BT_COMM;			/* ���샂�[�h:BT���W���[���ʐM		*/
			break;
		case SD_CTRL_SYS_MODE_GNSS_COMM:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_GNSS_COM;			/* ���샂�[�h:GNSS���W���[���ʐM	*/
			break;
		default:
			eventParam = SYS_EVENT_PARAM_ACT_MODE_NORMAL;			/* ���샂�[�h:�ʏ�					*/
			OUTLOG(OutputLog("[SdCtrl]SysMode ERR!!! mode:%d\n", S_sd_sys_mode);)
			break;
		}
		(void)System_control_NotifyEvent( SYS_EVENT_DECISION_ACTION_MODE, eventParam );

		/* �J�ڐ���:�A�C�h��	*/
		nextStat = SD_CTRL_STAT_IDLE;
	}
	else {
		/* nop */
	}

	return nextStat;

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_stat_ProcRec									*/
/* �@�\�̐���	:	��Ԋ֐�:SD_CTRL_STAT_REC								*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	SD_CTRL_STAT_xxx	�J�ڐ���							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcRec( SD_CTRL_EVENT event )
{

#ifdef _VDV_APP
	E_SD_CTRL_STAT		nextStat = SD_CTRL_STAT_REC;
	E_FIO_RESULT		fioRet;
	UI_32				reqNo;
	UI_32				time1ms;
	UI_16				headSize;
	UI_16				dataSize;
	UI_8				i;

	if ((event & SD_CTRL_EVENT_REQ_SHUTDOWN) != 0) {		/* SD���W���[����~�v��				*/
		/* �J�ڐ���:SD���W���[����~�҂�	*/
		nextStat = SD_CTRL_STAT_WAIT_SHUTDOWN;
	}
	else if ((event & SD_CTRL_EVENT_REQ_STOP_REC) != 0) {	/* REC��~�v��						*/
		/* �J�ڐ���:REC�I���҂�	*/
		nextStat = SD_CTRL_STAT_WAIT_REC_END;
	}
	else {
		if ((event & SD_CTRL_EVENT_NOTIFY_WRITE_RESULT) != 0) {		/* REC�f�[�^�������݌��ʒʒm					*/
			/* �������݌��ʂ��擾	*/
			for (i = 0; i < SD_CTRL_FIO_WRITE_NOTFY_NUM; i++) {
				if (RingBuffer_GetCanReadSize( &S_sd_rec_write_notify_mng ) < sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO)) {
					break;
				}
				(void)RingBuffer_GetArrayData( &S_sd_rec_write_notify_mng, (UI_8*)&S_sd_notifyinfo_get_buff, sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO), RINGBUF_READEND );
				if (S_sd_notifyinfo_get_buff.result == FIO_RESULT_OK) {										/* �������ݐ���									*/
					if ( S_sd_notifyinfo_get_buff.bytes > S_sd_notifyinfo_get_buff.bytesWritten ) {			/* �v���T�C�Y���������߂Ă��Ȃ��iSD�󂫂Ȃ��j	*/
						/* �J�ڐ���:REC�I���҂�	*/
						nextStat = SD_CTRL_STAT_WAIT_REC_END;
					}
				}
				else {											/* �������݃G���[����							*/
					/* �J�ڐ���:REC�I���҂�	*/
					nextStat = SD_CTRL_STAT_WAIT_REC_END;
				}
			}
		}

		if (((event & SD_CTRL_EVENT_REQ_WRITE_REC_DATA) != 0) &&		/* REC�f�[�^�������ݗv��						*/
			(nextStat == SD_CTRL_STAT_REC)) {							/* �J�ڐ���:REC���							*/

			if ( S_sd_rec_write_req_cnt == 0 ) {						/* ���񏑂�����		*/
				/* REC�J�n���Ԃ�ێ�	*/
				S_sd_write_start_time = Timer_ctrl_GetTime1ms();

				/* REC�t�@�C���p�X�쐬	*/
				sd_ctrl_MakeRecFilepath();

				/* ���O�������������� */
				sd_ctrl_UpdateLogDataInfo( N_TRUE );

				/* REC�t�@�C���̃w�b�_���쐬	*/
				sd_ctrl_MakeRecHead( S_sd_rec_head, sizeof(S_sd_rec_head), &headSize, N_TRUE );

				/* REC�t�@�C���̃w�b�_������������	*/
				fioRet = FileIO_Write( FIO_WRITE_TYPE_FILE_CREATE, S_sd_rec_filepath, S_sd_rec_head, 0, (UI_32)headSize, sd_ctrl_RecDataWriteCallback, &reqNo );
				if (fioRet == FIO_RESULT_OK) {
					S_sd_rec_write_req_cnt++;
					S_sd_update_header_time = Timer_ctrl_GetTime1ms();
				}
				else {											/* �������݃G���[����							*/
					/* �J�ڐ���:REC�I���҂�	*/
					nextStat = SD_CTRL_STAT_WAIT_REC_END;
				}
			}

			if (nextStat == SD_CTRL_STAT_REC) {
				time1ms = Timer_ctrl_GetTime1ms();
				if ((time1ms - S_sd_write_start_time) > SD_CTRL_REC_TIME_MAX) {	/* REC�ő厞�ԃI�[�o�[	*/
					/* �J�ڐ���:REC�I���҂�	*/
					nextStat = SD_CTRL_STAT_WAIT_REC_END;
				}
				else {
					/* ���O�����X�V���� */
					sd_ctrl_UpdateLogDataInfo( N_FALSE );
					/* ����w�b�_�X�V����(�����Ŏ��Ԕ��肠��) */
					sd_ctrl_UpdateHeader();

					/* REC�f�[�^�쐬	*/
					sd_ctrl_MakeRecData( S_sd_rec_data, sizeof(S_sd_rec_data), &dataSize );

					/* REC�f�[�^����������	*/
					sd_ctrl_WriteRecData( S_sd_rec_data, dataSize );
				}
			}
		}
	}

	return nextStat;
#else /* _VDV_APP */
	return SD_CTRL_STAT_IDLE;
#endif /* _VDV_APP */

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_stat_ProcWaitRecEnd								*/
/* �@�\�̐���	:	��Ԋ֐�:SD_CTRL_STAT_WAIT_REC_END						*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	SD_CTRL_STAT_xxx	�J�ڐ���							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcWaitRecEnd( SD_CTRL_EVENT event )
{

#ifdef _VDV_APP
	E_SD_CTRL_STAT nextStat = SD_CTRL_STAT_WAIT_REC_END;
	UI_8	i;

	if ((event & SD_CTRL_EVENT_NOTIFY_WRITE_RESULT) != 0) {	/* REC�f�[�^�������݌��ʒʒm			*/
		for (i = 0; i < SD_CTRL_FIO_WRITE_NOTFY_NUM; i++) {
			/* REC�f�[�^�������݌��ʒʒm���o�b�t�@��ǂݐi�߂�	*/
			if (RingBuffer_GetCanReadSize( &S_sd_rec_write_notify_mng ) < sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO)) {
				break;
			}
			(void)ReadEndRingBuffer( &S_sd_rec_write_notify_mng, sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO) );
		}
	}

	if ((event & SD_CTRL_EVENT_REQ_SHUTDOWN) != 0) {			/* SD���W���[����~�v��				*/
		/* �J�ڐ���:SD���W���[����~	*/
		nextStat = SD_CTRL_STAT_WAIT_SHUTDOWN;
	}
	else if ((event & SD_CTRL_EVENT_NOTIFY_STOP_REC) != 0) {	/* REC��~����						*/
		/* �J�ڐ���:�A�C�h��	*/
		nextStat = SD_CTRL_STAT_IDLE;
	}
	else {
		/* nop */
	}

	return nextStat;
#else /* _VDV_APP */
	return SD_CTRL_STAT_IDLE;
#endif /* _VDV_APP */

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_stat_ProcSimulation								*/
/* �@�\�̐���	:	��Ԋ֐�:SD_CTRL_STAT_SIMULATION						*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	SD_CTRL_STAT_xxx	�J�ڐ���							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcSimulation( SD_CTRL_EVENT event )
{

#ifdef _VDV_APP
	E_SD_CTRL_STAT nextStat = SD_CTRL_STAT_SIMULATION;

	/* �T�u��ԑJ�ڂ̃��C���������Ăяo�� */
	sd_sim_state_MainProc();

	if( SD_SIM_STAT_ACC_OFF == S_sd_sim_status ) {
		/* ACC OFF �ɂ���~���m�F���Ă���J�� */
		nextStat = SD_CTRL_STAT_WAIT_SHUTDOWN;
	}
	return nextStat;
#else /* _VDV_APP */
	return SD_CTRL_STAT_IDLE;
#endif /* _VDV_APP */

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_stat_ProcInspection								*/
/* �@�\�̐���	:	��Ԋ֐�:SD_CTRL_STAT_INSPECTION						*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	SD_CTRL_STAT_xxx	�J�ڐ���							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcInspection( SD_CTRL_EVENT event )
{

#ifdef _VDV_APP
	E_SD_CTRL_STAT	nextStat = SD_CTRL_STAT_INSPECTION;

	if ((event & SD_CTRL_EVENT_REQ_SHUTDOWN) != 0) {					/* SD���W���[����~�v��				*/
		/* �J�ڐ���:SD���W���[����~�҂�	*/
		nextStat = SD_CTRL_STAT_WAIT_SHUTDOWN;
	}
	else if (((event & SD_CTRL_EVENT_REQ_INSPECTION_START) != 0) ||		/* SD�����J�n						*/
			 ((event & SD_CTRL_EVENT_NOTIFY_INSPECTION_SD_RES) != 0)) {	/* SD�����p�A�N�Z�X���ʒʒm			*/
		/* SD����	*/
		if (sd_ctrl_Inspect() == N_OFF) {
			/* �J�ڐ���:�A�C�h��	*/
			nextStat = SD_CTRL_STAT_IDLE;
		}
	}
	else {
		/* nop */
	}

	return nextStat;

#else /* _VDV_APP */
	return SD_CTRL_STAT_IDLE;
#endif /* _VDV_APP */

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_stat_ProcWaitShutdown							*/
/* �@�\�̐���	:	��Ԋ֐�:SD_CTRL_STAT_WAIT_SHUTDOWN						*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	SD_CTRL_STAT_xxx	�J�ڐ���							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcWaitShutdown( SD_CTRL_EVENT event )
{

#ifdef _VDV_APP
	E_SD_CTRL_STAT nextStat = SD_CTRL_STAT_WAIT_SHUTDOWN;
	UI_8	i;

	if ((event & SD_CTRL_EVENT_NOTIFY_WRITE_RESULT) != 0) {	/* REC�f�[�^�������݌��ʒʒm			*/
		/* REC�f�[�^�������݌��ʒʒm���o�b�t�@��ǂݐi�߂�	*/
		for (i = 0; i < SD_CTRL_FIO_WRITE_NOTFY_NUM; i++) {
			if (RingBuffer_GetCanReadSize( &S_sd_rec_write_notify_mng ) < sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO)) {
				break;
			}
			(void)ReadEndRingBuffer( &S_sd_rec_write_notify_mng, sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO) );
		}
	}

	if ((event & SD_CTRL_EVENT_NOTIFY_INSPECTION_SD_RES) != 0) {	/* SD�����p�A�N�Z�X���ʒʒm		*/
		S_sd_inspection_info.fio_req_flg = N_OFF;
	}

	if ((event & SD_CTRL_EVENT_NOTIFY_SHUTDOWN) != 0) {		/* SD���W���[����~����					*/
		/* �V�X�e����Ԑ����SD�I������������ʒm	*/
		(void)System_control_NotifyEvent( SYS_EVENT_END_PROC_COMP_SD, SYS_EVENT_PARAM_NO );

		/* �J�ڐ���:SD���W���[����~	*/
		nextStat = SD_CTRL_STAT_SHUTDOWN;
	}
	else if ((event & SD_CTRL_EVENT_REQ_STARTUP) != 0) {	/* SD���W���[���N���v��					*/
		/* �J�ڐ���:�A�C�h��	*/
		nextStat = SD_CTRL_STAT_IDLE;
	}
	else {
		/* nop */
	}

	return nextStat;
#else /* _VDV_APP */
	/* �V�X�e����Ԑ����SD�I������������ʒm	*/
	(void)System_control_NotifyEvent( SYS_EVENT_END_PROC_COMP_SD, SYS_EVENT_PARAM_NO );

	return SD_CTRL_STAT_SHUTDOWN;
#endif /* _VDV_APP */

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_stat_ProcShutdown								*/
/* �@�\�̐���	:	��Ԋ֐�:SD_CTRL_STAT_SHUTDOWN							*/
/* ����			:	(I/ )event	�C�x���g									*/
/* �߂�l		: 	SD_CTRL_STAT_xxx	�J�ڐ���							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_CTRL_STAT sd_ctrl_stat_ProcShutdown( SD_CTRL_EVENT event )
{

	E_SD_CTRL_STAT nextStat = SD_CTRL_STAT_SHUTDOWN;

	if ((event & SD_CTRL_EVENT_REQ_STARTUP) != 0) {			/* SD���W���[���N���v��				*/
		/* �J�ڐ���:�A�C�h��	*/
		nextStat = SD_CTRL_STAT_IDLE;
	}

	return nextStat;

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_InitModuleParam									*/
/* �@�\�̐���	:	���W���[���Ŏg�p����ϐ�������							*/
/* ����			:	�Ȃ�													*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_ctrl_InitModuleParam( void )
{

	S_sd_sys_mode = SD_CTRL_SYS_MODE_UNKNOWN;
	S_sd_fio_notify_get_mode = N_OFF;
	S_sd_fio_notify_write = N_OFF;
	S_sd_fio_notify_inspection = N_OFF;
	S_sd_sys_mode_req_flg = N_OFF;
	(void)memset(S_sd_sys_mode_data, 0, sizeof(S_sd_sys_mode_data));

#ifdef _VDV_APP
	S_processType = SD_PROCESS_MAIN;
	S_sd_recBtnOnOffCnt = 0;
	sd_ctrl_InitRecParam();
	sd_ctrl_InitSimParam();
	sd_ctrl_InitInspectionInfo();

	sd_ctrl_UpdateLogDataInfo( N_TRUE );
#endif /* _VDV_APP */

	return;

}

#ifdef _VDV_APP
/****************************************************************************/
/* �֐�����		:	sd_ctrl_InitRecParam									*/
/* ����			:	REC�֘A�ϐ�������										*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_ctrl_InitRecParam( void )
{

	/* REC�f�[�^�o�b�t�@������	*/
	(void)memset(S_sd_rec_head, 0, sizeof(S_sd_rec_head));
	(void)memset(S_sd_rec_data, 0, sizeof(S_sd_rec_data));
	(void)memset(S_sd_rec_data_buff, 0, sizeof(S_sd_rec_data_buff));
	RingBuffer_Init( &S_sd_rec_data_mng, S_sd_rec_data_buff, (UI_16)sizeof(S_sd_rec_data_buff) );
	S_sd_ring_write_size = 0;
	S_sd_ring_flush_size = 0;

	/* REC�f�[�^�������݉񐔏�����	*/
	S_sd_rec_write_req_cnt = 0;
	S_sd_rec_write_notify_cnt = 0;

	/* REC�f�[�^�������݌��ʒʒm���o�b�t�@������	*/
	(void)memset(S_sd_rec_write_notify_buff, 0, sizeof(S_sd_rec_write_notify_buff));
	RingBuffer_Init( &S_sd_rec_write_notify_mng, (UI_8*)S_sd_rec_write_notify_buff, (UI_16)sizeof(S_sd_rec_write_notify_buff) );
	(void)memset(&S_sd_notifyinfo_set_buff, 0, sizeof(S_sd_notifyinfo_set_buff));
	(void)memset(&S_sd_notifyinfo_get_buff, 0, sizeof(S_sd_notifyinfo_get_buff));

	/* REC�f�[�^�������݊Ǘ���񏉊���	*/
	S_sd_write_start_time = 0;
	S_sd_update_header_time = 0;

	/* REC�f�[�^�t�@�C���p�X������	*/
	(void)memset(S_sd_rec_filepath, 0, sizeof(S_sd_rec_filepath));

	return ;

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_InitSimParam									*/
/* ����			:	�V�~�����[�V�����֘A�ϐ�������							*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_ctrl_InitSimParam( void )
{

	sd_sim_InitSimCtrlInfo( SD_SIM_INIT_MODE_ENTER_SIM );

	return ;

}
#endif /* _VDV_APP */

/****************************************************************************/
/* �֐�����		:	sd_ctrl_SysModeCallback									*/
/* �@�\�̐���	:	�V�X�e�����샂�[�h�擾�R�[���o�b�N						*/
/* ����			:	(I/ )reqinfo	�v�����								*/
/* 				:	(I/ )retinfo	����									*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_ctrl_SysModeCallback( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo )
{

	/* ���[�h��ݒ� */
	S_sd_sys_mode = SD_CTRL_SYS_MODE_NOMAL;			/* ���[�h:�ʏ�			*/
	if (retinfo->result == FIO_RESULT_OK) {		/* �V�X�e�����샂�[�h�t�@�C����ǂ߂�		*/
		if (S_sd_sys_mode_data[0] == SD_CTRL_SYS_MODE_FILE_VALUE_SIMU) {
			S_sd_sys_mode = SD_CTRL_SYS_MODE_SIMULATION;	/* ���[�h:�V�~�����[�V����			*/
		}
		else if (S_sd_sys_mode_data[0] == SD_CTRL_SYS_MODE_FILE_VALUE_EEPROM) {
			S_sd_sys_mode = SD_CTRL_SYS_MODE_EEPROM;		/* ���[�h:EEPROM��������			*/
		}
		else if (S_sd_sys_mode_data[0] == SD_CTRL_SYS_MODE_FILE_VALUE_FIRM) {
			S_sd_sys_mode = SD_CTRL_SYS_MODE_FIRMWARE;		/* ���[�h:�t�@�[���E�F�A��������	*/
		}
		else if (S_sd_sys_mode_data[0] == SD_CTRL_SYS_MODE_FILE_VALUE_INSPECTION) {
			S_sd_sys_mode = SD_CTRL_SYS_MODE_INSPECTION;	/* ���[�h:����						*/
		}
#ifdef ADVANCE_ENABLE
		else if (S_sd_sys_mode_data[0] == SD_CTRL_SYS_MODE_FILE_VALUE_BT_COMM) {
			S_sd_sys_mode = SD_CTRL_SYS_MODE_BT_COMM;		/* ���[�h:BT���W���[���ʐM			*/
		}
		else if (S_sd_sys_mode_data[0] == SD_CTRL_SYS_MODE_FILE_VALUE_GNSS_COMM) {
			S_sd_sys_mode = SD_CTRL_SYS_MODE_GNSS_COMM;		/* ���[�h:GNSS���W���[���ʐM		*/
		}
#endif /* ADVANCE_ENABLE */
		else {
			/* nop */
		}
	}

	/* �V�X�e�����샂�[�h�擾�ʒm�t���O��ON */
	S_sd_fio_notify_get_mode = N_ON;

	/* �V�X�e�����샂�[�h�擾�v���t���OOFF	*/
	S_sd_sys_mode_req_flg = N_OFF;

	return;

}

#ifdef _VDV_APP
/****************************************************************************/
/* �֐�����		:	sd_ctrl_RecDataWriteCallback							*/
/* �@�\�̐���	:	REC�f�[�^�������݃R�[���o�b�N							*/
/* ����			:	(I/ )reqinfo	�v�����								*/
/* 				:	(I/ )retinfo	����									*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_ctrl_RecDataWriteCallback( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo )
{

	/* REC�f�[�^�������݌��ʒʒm���ɃZ�b�g���� */
	if ((reqinfo != NULL) && (retinfo != NULL)) {
		if (RingBuffer_GetCanWriteSize( &S_sd_rec_write_notify_mng ) >= sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO) ) {
			S_sd_notifyinfo_set_buff.result = retinfo->result;
			S_sd_notifyinfo_set_buff.bytes = reqinfo->bytes;
			S_sd_notifyinfo_set_buff.bytesWritten = retinfo->bytes;
			(void)RingBuffer_SetArrayData( &S_sd_rec_write_notify_mng, (UI_8*)&S_sd_notifyinfo_set_buff, sizeof(T_SD_CTRL_WRITE_END_NOTIFY_INFO) );
		}
	}

	if ((reqinfo != NULL) &&
		(S_sd_rec_data_buff <= reqinfo->buff) &&
		(reqinfo->buff <= (S_sd_rec_data_buff + sizeof(S_sd_rec_data_buff) - 1))) {	/* REC�����O�o�b�t�@�̃f�[�^�ɑ΂��鏑�����݊����ʒm */
		/* REC�����O�o�b�t�@�̃��[�h�ʒu��i�߂� */
		(void)ReadEndRingBuffer( &S_sd_rec_data_mng, (UI_16)reqinfo->bytes );
	}

	/* REC�f�[�^�������݌��ʒʒm�񐔂��C���N�������g	*/
	S_sd_rec_write_notify_cnt++;

	/* REC�f�[�^�������݌��ʒʒm�t���O��ON */
	S_sd_fio_notify_write = N_ON;

	return;

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_CheckWriteStartCnd								*/
/* �@�\�̐���	:	SD�������݊J�n�����Ɉ�v���邩�`�F�b�N����				*/
/* ����			:	�Ȃ�													*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static B_8 sd_ctrl_CheckWriteStartCnd( void )
{

	E_SYS_CTRL_STATUS		sysStat;
	B_8						btnOn;
	B_8						speedOn;
	E_FIO_RESULT			fioRet;
	UI_32					freeSectNum;
	E_SW_REC_BTN_STATUS		recBtnStat;
	UI_8					btnOnOffCnt;
	T_Can_ctrl_tcan_info	*canInfo;
	UI_16					speed;
	UI_8					portLev;

	sysStat = System_control_GetStatus();

	/* �V�X�e�������ԃ`�F�b�N	*/
	if ((sysStat != SYS_OPERATION_NORMAL_AUTO) &&
		(sysStat != SYS_OPERATION_NORMAL_MANUAL)) {		/*�u�I�[�gREC�v�A�u�}�j���A��REC�v�ȊO	*/
		return N_FALSE;
	}

	/* REC�{�^��/�ԑ����`�F�b�N	*/
	btnOn = N_FALSE;
	speedOn = N_FALSE;
	/* REC�{�^���������ꂽ���`�F�b�N*/
	recBtnStat = Sw_control_GetRecButtonStatus();
	if ((recBtnStat == SYS_REC_BTN_ON_SHORTPRESS) ||
		(recBtnStat == SYS_REC_BTN_ON_LONGPRESS)) {		/* �{�^���������ꂽ		*/
		btnOnOffCnt = Sw_control_GetRecButtonOnOffChangeCount();
		if (S_sd_recBtnOnOffCnt != btnOnOffCnt) {	/* �O���REC�{�^���J�E���g�ƈقȂ�	*/
			/* �`���^�����O�z���̂���ON/OFF������Ԃ����C����������Ԋu��蒷���B	*/
			/* 1��̃{�^������������������ꂽ�Ɣ��f���Ă��܂�Ȃ�����ON/OFF�̕ω������肷��B*/
			/* �ێ����Ă���{�^���J�E���g�ƈقȂ�ꍇ�͉����ꂽ�Ɣ��f����	*/
			S_sd_recBtnOnOffCnt = btnOnOffCnt;
			btnOn = N_TRUE;
		}
		else {
			/* �ێ����Ă���{�^���J�E���g�Ɠ����ꍇ�͉����ꂽ��Ԃ��p�����Ă���i������͂�����Ă��Ȃ��j�Ɣ��f����	*/
			/* �������Ȃ�	*/
		}
	}
	/* �I�[�gREC�̏ꍇ�͎ԑ����`�F�b�N����	*/
	if (sysStat == SYS_OPERATION_NORMAL_AUTO) {
		canInfo = Can_control_GetTCanInfo();
		if (canInfo != NULL) {
			if( canInfo->en_vehicle_speed_for_disp == 1 ) {	/* CAN���ԑ�����M���Ă���	*/
				speed = (UI_16)((F_64)canInfo->vehicle_speed_for_disp * CAN_VEHICLESPEED_DISP_RESOLUTION);
				if(speed >= 1){									/* �ԑ���1Km/h�ȏ�			*/
					speedOn = N_TRUE;
				}
			}
		}
	}
	if ((btnOn == N_FALSE) && (speedOn == N_FALSE)) {
		return N_FALSE;
	}

	/* GNSS�����ԃ`�F�b�N	*/
#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata */
	if (Gnss_control_iSGnssFixedAtOneTime() == N_FALSE) {	/* GNSS1��ł��⑫�ς�	*/
#else
	if (Signal_control_isGnssEnable() == N_FALSE) {		/* GNSS����M			*/
#endif /* _VDV_SUPRA�@2018.12.14 S.Sakata */
		OUTLOG(OutputLog("[SdCtrl]RecStartCheck GnssDisable\n");)
		return N_FALSE;
	}

	/* CAN�����ԃ`�F�b�N	*/
	if (Signal_control_isCanEnable() == N_FALSE) {		/* CAN����M			*/
/*		OUTLOG(OutputLog("[SdCtrl]RecStartCheck CanDisable\n");) */
		return N_FALSE;
	}


	/* SD�ɋ󂫂����邩�`�F�b�N	*/
	fioRet = FileIO_Get_FreeSectNum( (const UI_8*)FS_DRV_PATH_SD, &freeSectNum );
	if (fioRet != FIO_RESULT_OK) {						/* SD�󂫗e�ʂ��擾�ł��Ȃ��iSD�g�p�\��Ԃł͂Ȃ��j	*/
/*		OUTLOG(OutputLog("[SdCtrl]RecStartCheck SdNotReady\n");) */
		return N_FALSE;
	}
	else {
		if (freeSectNum == 0) {							/* SD�ɋ󂫂�����		*/
/*			OUTLOG(OutputLog("[SdCtrl]RecStartCheck SdNotEmpty\n");) */
			return N_FALSE;
		}
	}

	/* ���C�g�v���e�N�g��Ԃ��`�F�b�N */
	if (PortDrv_GetDataBit(IO_PI_GR_SD_WP, IO_PI_BIT_SD_WP, &portLev) == D_OK) {
		if (portLev == IO_PI_LEV_NONACT_SD_WP) {
			/* �v���e�N�g�L */
/*			OUTLOG(OutputLog("[SdCtrl]RecStartCheck SdWriteProtect\n");) */
			return N_FALSE;
		}
	}

	/* ���s�������N���A	*/
	S_sd_running_distance = 0;
	/* CAN���擾 */
	canInfo = Can_control_GetTCanInfo();
	if (canInfo != NULL) {
		/* ���s������ێ� */
		S_sd_running_distance = canInfo->running_distance;
	}
	/* ��Ԏ��ԂɌ��ݎ������Z�b�g */
	S_sd_speed_stop_time = Timer_ctrl_GetTime1ms();

	return N_TRUE;

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_CheckWriteEndCnd								*/
/* �@�\�̐���	:	SD�������ݒ�~�����Ɉ�v���邩�`�F�b�N����				*/
/* ����			:	�Ȃ�													*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static B_8 sd_ctrl_CheckWriteEndCnd( void )
{

	E_SW_REC_BTN_STATUS		recBtnStat;
	UI_8					btnOnOffCnt;
	T_Can_ctrl_tcan_info	*canInfo;

	/* REC�{�^���������ꂽ���`�F�b�N*/
	recBtnStat = Sw_control_GetRecButtonStatus();
	if ((recBtnStat == SYS_REC_BTN_ON_SHORTPRESS) ||
		(recBtnStat == SYS_REC_BTN_ON_LONGPRESS)) {		/* �{�^���������ꂽ		*/
		btnOnOffCnt = Sw_control_GetRecButtonOnOffChangeCount();
		if (S_sd_recBtnOnOffCnt != btnOnOffCnt) {	/* �O���REC�{�^���J�E���g�ƈقȂ�	*/
			/* �`���^�����O�z���̂���ON/OFF������Ԃ����C����������Ԋu��蒷���B	*/
			/* 1��̃{�^������������������ꂽ�Ɣ��f���Ă��܂�Ȃ�����ON/OFF�̕ω������肷��B*/
			/* �ێ����Ă���{�^���J�E���g�ƈقȂ�ꍇ�͉����ꂽ�Ɣ��f����	*/
			S_sd_recBtnOnOffCnt = btnOnOffCnt;
			return N_TRUE;
		}
		else {
			/* �ێ����Ă���{�^���J�E���g�Ɠ����ꍇ�͉����ꂽ��Ԃ��p�����Ă���i������͂�����Ă��Ȃ��j�Ɣ��f����	*/
			/* �������Ȃ�	*/
		}
	}

	/* �K�莞�Ԉȏ��Ԃ��Ă���ꍇ��REC���~ */
	/* CAN���擾 */
	canInfo = Can_control_GetTCanInfo();
	if (canInfo != NULL) {
		/* ��Ԃ��Ă��邩�`�F�b�N	*/
		if ((SYS_OPERATION_NORMAL_AUTO == System_control_GetStatus()) &&	/* �I�[�g���[�h */
			(canInfo->wheel_speed_ave == 0) &&								/* �ԑ���0				*/
			(canInfo->running_distance == S_sd_running_distance)) {			/* ���s�����ɕω��Ȃ�	*/
			/* ��Ԃ��ċK�莞�Ԍo�߂������`�F�b�N	*/
			if (Timer_ctrl_TimeCompare1ms(S_sd_speed_stop_time, SD_CTRL_REC_STOP_TIMEOUT) == N_OK) {	
				/* ��Ԃ��ċK�莞�Ԍo��	*/
				return N_TRUE;
			}
		}
		else {																/* ��Ԃ��Ă��Ȃ�		*/
			/* ���s�������X�V	*/
			S_sd_running_distance = canInfo->running_distance;
			/* ��Ԏ��Ԃ����ݎ����ōX�V */
			S_sd_speed_stop_time = Timer_ctrl_GetTime1ms();
		}
	}

	return N_FALSE;

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_MakeRecFilepath									*/
/* �@�\�̐���	:	REC�f�[�^�t�@�C���p�X�쐬								*/
/* ����			:	�Ȃ�													*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_ctrl_MakeRecFilepath( void )
{

	T_COMMON_DATE_TIME	dtm;
	UI_32				pathlen;
	T_Gnss_ctrl_nav_pvt_info *hnrinf;
	hnrinf = Gnss_cntrol_GetNavPvtInfo();

	(void)memset(&dtm, 0, sizeof(dtm));
	if (hnrinf != NULL) {
		dtm.year = hnrinf->year;
		dtm.month = hnrinf->month;
		dtm.day = hnrinf->day;
		dtm.hour = hnrinf->hour;
		dtm.min = hnrinf->min;
		dtm.sec = hnrinf->sec;
	}

	/* REC�f�[�^�t�@�C���p�X��SD�h���C�u�̃p�X��ݒ� */
	S_sd_rec_filepath[0] = '\0';
	(void)strcpy((CH_8*)S_sd_rec_filepath, FS_DRV_PATH_SD);

	/* REC�f�[�^�t�@�C���p�X�ɓ�����ǉ� */
	pathlen = strlen((const CH_8*)S_sd_rec_filepath);
	Common_MakeDateTimeStr(&dtm, COMMON_DATE_TIME_STR_TYPE_YEAR_4, &S_sd_rec_filepath[pathlen]);

	/* REC�f�[�^�t�@�C���p�X�Ɋg���q��ǉ� */
	(void)strcat((CH_8*)S_sd_rec_filepath, SD_CTRL_REC_FILE_EXTENSION);

	return;

}

/****************************************************************************/
/* �֐�����		: sd_ctrl_MakeRecHead										*/
/* �@�\�̐���	: �t�@�C���w�b�_�쐬										*/
/* ����			: ( /O)headBuff	�o�b�t�@�擪�A�h���X						*/
/*				  (I/ )buffSize	�o�b�t�@�T�C�Y								*/
/*				  ( /O)headSize	�쐬�����w�b�_�T�C�Y						*/
/*				  (I/ )init		������(N_TRUE:������/����ȊO:�X�V)			*/
/* �߂�l		: void														*/
/* �쐬��		: (OFT)T.Kamakura											*/
/* �쐬��		: 16/10/01													*/
/* ���l			: init �ɂ�蓮�삪�قȂ�B									*/
/*	���������[�h	�Í��L�[�����o�^���s���t�@�C���擪�̌Œ蕔���쐬����B	*/
/*	�X�V���[�h		�t�@�C���w�b�_�݂̂��쐬����B							*/
/****************************************************************************/
static void sd_ctrl_MakeRecHead( UI_8 *headBuff, UI_16 buffSize, UI_16 *headSize, B_8 init )
{

	UI_16			fileHeaderPartSize = 0U;
	UI_16			fixIdInfoPartSize = 0U;
	UI_16			fixSignalPartSize = 0U;
	UI_16			varIdInfoPartSize = 0U;
	UI_8*			pBuff = NULL;
	UI_16			restSize = 0U;
	UI_8			fixedSignalNum = 0U;
	UI_16*			fixedSignalIdList = 0U;
	UI_8			vehicleSignalNum = 0U;
	UI_16*			vehicleSignalIdList = 0U;

	if( (NULL != headBuff) && (NULL != headSize) ) {
		/* �o�b�t�@�擪����i�[ */
		pBuff = headBuff;
		restSize = buffSize;

		if( N_TRUE == init ) {
			/*==============================================================*/
			/* �������[�h�ł̓G���R�[�h�L�[�쐬���ĐM������֐ݒ�			*/
			/*==============================================================*/
			(void)Signal_control_SetEncodeKey( DEVICE_TYPE_SD, Signal_control_GetRandomValue() );
		}
		/* �t�@�C���w�b�_�����쐬 */
		sd_ctrl_MakeFileHeaderPart( pBuff, restSize, &fileHeaderPartSize );
		pBuff += fileHeaderPartSize;
		restSize -= fileHeaderPartSize;

		if( N_TRUE == init ) {
			/*==============================================================*/
			/* �������[�h�ł̓w�b�_���̌�̌Œ蕔�������쐬					*/
			/*==============================================================*/
			/* �M�����䂩��ID����擾 */
			(void)Signal_control_GetSignalList( DEVICE_TYPE_SD, ID_TYPE_FIXED_ID, &fixedSignalNum, (const UI_16**)&fixedSignalIdList );
			(void)Signal_control_GetSignalList( DEVICE_TYPE_SD, ID_TYPE_VARIABLE_ID, &vehicleSignalNum, (const UI_16**)&vehicleSignalIdList );

			/* �Œ�M���f�[�^�z����擾 */
			(void)Signal_control_GetSignalsInfo( DEVICE_TYPE_SD, fixedSignalNum, fixedSignalIdList, S_sd_signal_buff );

			/* �Œ�ID��񕔂��쐬 */
			sd_ctrl_MakeIdInfoDataPart( fixedSignalNum, fixedSignalIdList, pBuff, restSize, &fixIdInfoPartSize );
			pBuff += fixIdInfoPartSize;
			restSize -= fixIdInfoPartSize;

			/* �Œ�ԗ���񕔂��쐬 */
			sd_ctrl_MakeSignalDataPart( fixedSignalNum, S_sd_signal_buff, pBuff, restSize, &fixSignalPartSize );
			pBuff += fixSignalPartSize;
			restSize -= fixSignalPartSize;

			/* �ϐ�ID��񕔂��쐬 */
			sd_ctrl_MakeIdInfoDataPart( vehicleSignalNum, vehicleSignalIdList, pBuff, restSize, &varIdInfoPartSize );
			pBuff += fixIdInfoPartSize;
			restSize -= fixIdInfoPartSize;
		}
		/* �w�b�_�S�̃T�C�Y�i�[ */
		*headSize = (fileHeaderPartSize + fixIdInfoPartSize + fixSignalPartSize + varIdInfoPartSize);
/*		OUTLOG(OutputLog("[SD] Header(init=%d) size=[%d]\n", init, *headSize);) */
	}
	return;

}

/****************************************************************************/
/* �֐�����		: sd_ctrl_MakeRecData										*/
/* �@�\�̐���	: �L�^�f�[�^���쐬											*/
/* ����			: ( /O)dataBuff	�o�b�t�@�擪�A�h���X						*/
/*				  (I/ )buffSize	�o�b�t�@�T�C�Y								*/
/*				  ( /O)dataSize	�쐬�����f�[�^�T�C�Y						*/
/* �߂�l		: void														*/
/* �쐬��		: (OFT)T.Kamakura											*/
/* �쐬��		: 16/10/01													*/
/* ���l			: 															*/
/****************************************************************************/
static void sd_ctrl_MakeRecData( UI_8 *dataBuff, UI_16 buffSize, UI_16 *dataSize )
{

	UI_16			varSignalPartSize = 0U;
	UI_8*			pBuff = NULL;
	UI_16			restSize = 0U;
	UI_8			vehicleSignalNum = 0U;
	UI_16*			vehicleSignalIdList = 0U;

	if( (NULL != dataBuff) && (NULL != dataSize) ) {
		/* �o�b�t�@�擪����i�[ */
		pBuff = dataBuff;
		restSize = buffSize;

		/* �M�����䂩��ID����擾 */
		(void)Signal_control_GetSignalList( DEVICE_TYPE_SD, ID_TYPE_VARIABLE_ID, &vehicleSignalNum, (const UI_16**)&vehicleSignalIdList );

		/* �ϐ��M���f�[�^�z����擾 */
		(void)Signal_control_GetSignalsInfo( DEVICE_TYPE_SD, vehicleSignalNum, vehicleSignalIdList, S_sd_signal_buff );

		/* �Œ�ԗ���񕔂��쐬 */
		sd_ctrl_MakeSignalDataPart( vehicleSignalNum, S_sd_signal_buff, pBuff, restSize, &varSignalPartSize );
		pBuff += varSignalPartSize;
		restSize -= varSignalPartSize;

		/* �f�[�^���S�̃T�C�Y�i�[ */
		*dataSize = varSignalPartSize;
	}
	return;

}

/****************************************************************************/
/* �֐�����		: sd_ctrl_MakeFileHeaderPart								*/
/* �@�\�̐���	: �t�@�C���w�b�_���iFile Header�j���쐬					*/
/* ����			: ( /O)headBuff	�o�b�t�@�擪�A�h���X						*/
/*				  (I/ )buffSize	�o�b�t�@�T�C�Y								*/
/*				  ( /O)headSize	�쐬�����w�b�_�T�C�Y						*/
/* �߂�l		: void														*/
/* �쐬��		: (OFT)T.Kamakura											*/
/* �쐬��		: 16/10/01													*/
/* ���l			: 															*/
/****************************************************************************/
static void sd_ctrl_MakeFileHeaderPart( UI_8 *headBuff, UI_16 buffSize, UI_16 *headSize )
{

	UI_8*			p = NULL;
	UI_16			totalSize = 0U;
	UI_16			length = 0U;
	UI_16			firmwareVersion = 0U;
	UI_16			crc = 0UL;
	UI_16*			fixedSignalIdList = 0U;
	UI_16*			vehicleSignalIdList = 0U;
	SI_32			sdEncodeKey = 0UL;
	UI_8			fixedSignalNum = 0U;
	UI_8			vehicleSignalNum = 0U;
	UI_8			i = 0U;

	if( (NULL != headBuff) && (NULL != headSize) ) {
		/* �M�����䂩��ID����擾 */
		(void)Signal_control_GetSignalList( DEVICE_TYPE_SD, ID_TYPE_FIXED_ID, &fixedSignalNum, (const UI_16**)&fixedSignalIdList );
		(void)Signal_control_GetSignalList( DEVICE_TYPE_SD, ID_TYPE_VARIABLE_ID, &vehicleSignalNum, (const UI_16**)&vehicleSignalIdList );

		/* �o�͏����� */
		(void)memset(headBuff, 0, buffSize);
		*headSize = 0;

		/* �S�̃T�C�Y�v�Z */
		totalSize = sd_ctrl_GetFileHeaderPartSize( fixedSignalNum, vehicleSignalNum );
		length = totalSize - (SD_CTRL_FMT_HEADER_BYTES + SD_CTRL_FMT_FOOTER_BYTES) + 1U;
/*		OUTLOG(OutputLog("[SD] sd_ctrl_MakeFileHeaderPart(%d,%d)[%d,%d]\n", fixedSignalNum, vehicleSignalNum, totalSize, length);) */

		/* �G���R�[�h�L�[�擾 */
		(void)Signal_control_GetEncodeKey( DEVICE_TYPE_SD, &sdEncodeKey );

		if( totalSize > buffSize ) {
			/* �o�b�t�@�s�� */
		}
		else {
			/* �f�[�^�쐬�J�n */
			p = headBuff;

			/*==================================================================*/
			/* �w�b�_�쐬														*/
			/*==================================================================*/
			*p = SD_CTRL_FMT_HEADER_START;
			p++;
			Common_Set16( p, length );
			p += 2;
			*p = SD_CTRL_FMT_TYPE_HEADER;
			p++;

			/*==================================================================*/
			/* �f�[�^���쐬														*/
			/*==================================================================*/
			/* �ő̎��ʔԍ�(16�o�C�g) */
			/* 00�Œ� */
			p += 16;

			/* �Ԏ�R�[�h(2�o�C�g) */
			/* 00�Œ� */
			p += 2;

			/* �t�H�[�}�b�g�o�[�W����(3�o�C�g) */
			Common_Set24( p, SD_CTRL_FORMAT_VERSION );
			p += 3;

			/* �M�����X�g�o�[�W����(3�o�C�g) */
			Common_Set24( p, Signal_control_GetSignalVersion() );
			p += 3;

			/* �t�@�[���E�F�A�o�[�W����(3�o�C�g) */
			(void)Eeprom_control_GetData( EEPROM_DATA_FIRMWARE_VER, &firmwareVersion );

			/* 16bit�̃o�[�W������O�l�߂ŋL�^ */
			Common_Set24( p, ((UI_32)firmwareVersion) << 8 );
			p += 3;

			/* �R�����g(10�o�C�g) */
			/* 00�Œ� */
			p += 10;

			/* �擾�\�Ȏԗ��M�����i�Œ���j */
			*p = fixedSignalNum;
			p++;
			for( i = 0; i < fixedSignalNum; i++ ) {
				/* �擾�\�Ȏԗ��M��ID�i�Œ���j */
				Common_Set16( p, fixedSignalIdList[i] );
				p += 2;
			}
			/* �擾�\�Ȏԗ��M�����i�ԗ����j */
			*p = vehicleSignalNum;
			p++;
			for( i = 0; i < vehicleSignalNum; i++ ) {
				/* �擾�\�Ȏԗ��M��ID�i�ԗ����j */
				Common_Set16( p, vehicleSignalIdList[i] );
				p += 2;
			}
			/* ���O�������݊J�n���ԁiUTC) YYMMDDHHMMSS(��151110154322�j	*/
			(void)memcpy( p, S_sd_log_data_info.timeLogStart, SD_CTRL_FMT_UTC_LEN );
			p += SD_CTRL_FMT_UTC_LEN;
			/* ���O�������ݏI�����ԁiUTC) YYMMDDHHMMSS�i��151110161515�j */
			(void)memcpy( p, S_sd_log_data_info.timeLogEnd, SD_CTRL_FMT_UTC_LEN );
			p += SD_CTRL_FMT_UTC_LEN;

			/* �Ŗk�[�ܓx */
			Common_Set32( p, (UI_32)S_sd_log_data_info.latitudeNorthernMost );
			p += 4;
			/* �Ő��[�ܓx */
			Common_Set32( p, (UI_32)S_sd_log_data_info.longtudeWesternMost );
			p += 4;
			/* �œ�[�ܓx */
			Common_Set32( p, (UI_32)S_sd_log_data_info.latitudeSouthernMost );
			p += 4;
			/* �œ��[�o�x */
			Common_Set32( p, (UI_32)S_sd_log_data_info.longtudeEasternMost );
			p += 4;

			/* ����(4�o�C�g) */
			Common_Set32( p, (UI_32)sdEncodeKey );
			p += 4;

			/*==================================================================*/
			/* �t�b�^�쐬														*/
			/*==================================================================*/
			crc = Common_CalcCRC( headBuff, 1U, length + 2U );
			Common_Set16( p, crc );
			p += 2;

			*p = SD_CTRL_FMT_FOOTER_END;
			p++;

			/* �f�[�^�����i�[ */
			*headSize = totalSize;
		}
	}

}

/****************************************************************************/
/* �֐�����		: sd_ctrl_MakeIdInfoDataPart								*/
/* �@�\�̐���	: ID���iID Info Data�j���쐬								*/
/* ����			: (I/ )signalNum	�M����									*/
/*				  (I/ )idList		ID���X�g								*/
/*				  ( /O)dataBuff	�o�b�t�@�擪�A�h���X						*/
/*				  (I/ )buffSize	�o�b�t�@�T�C�Y								*/
/*				  ( /O)dataSize	�쐬�����f�[�^�T�C�Y						*/
/* �߂�l		: void														*/
/* �쐬��		: (OFT)T.Kamakura											*/
/* �쐬��		: 16/10/01													*/
/* ���l			: 															*/
/****************************************************************************/
static void sd_ctrl_MakeIdInfoDataPart( UI_8 signalNum, const UI_16 *idList, UI_8 *dataBuff, UI_16 buffSize, UI_16 *dataSize )
{

	UI_8*			p = NULL;
	UI_16			totalSize = 0U;
	UI_16			length = 0U;
	UI_16			crc = 0U;
	UI_8			i = 0U;

	if( (NULL != idList) && (NULL != dataBuff) && (NULL != dataSize) ) {
		/* �o�͏����� */
		(void)memset(dataBuff, 0, buffSize);
		*dataSize = 0;

		/* �S�̃T�C�Y�v�Z */
		totalSize = sd_ctrl_GetIdInfoDataPartSize( signalNum );
		length = totalSize - (SD_CTRL_FMT_HEADER_BYTES + SD_CTRL_FMT_FOOTER_BYTES) + 1U;

		if( totalSize > buffSize ) {
			/* �o�b�t�@�s�� */
		}
		else {
			/* �f�[�^�쐬�J�n */
			p = dataBuff;

			/*==================================================================*/
			/* �w�b�_�쐬														*/
			/*==================================================================*/
			*p = SD_CTRL_FMT_HEADER_START;
			p++;
			Common_Set16( p, length );
			p += 2;
			*p = SD_CTRL_FMT_TYPE_ID_INFO_DATA;
			p++;

			/*==================================================================*/
			/* �f�[�^���쐬														*/
			/*==================================================================*/
			/* �������ݎԗ��M���� */
			*p = signalNum;
			p++;

			/* ��������ԗ��M��ID */
			for( i = 0U; i < signalNum; i++ ) {
				/* �ԗ��M��ID */
				Common_Set16( p, idList[i] );
				p += 2;
			}
			/*==================================================================*/
			/* �t�b�^�쐬														*/
			/*==================================================================*/
			crc = Common_CalcCRC( dataBuff, 1U, length + 2U );
			Common_Set16( p, crc );
			p += 2;

			*p = SD_CTRL_FMT_FOOTER_END;
			p++;

			/* �f�[�^�����i�[ */
			*dataSize = totalSize;
		}
	}

}

/****************************************************************************/
/* �֐�����		: sd_ctrl_MakeSignalDataPart								*/
/* �@�\�̐���	: �ԗ����iSignal Data�j���쐬								*/
/* ����			: (I/ )signalNum		�M����								*/
/*				  (I/ )signalValues	�M�����X�g								*/
/*				  ( /O)dataBuff	�o�b�t�@�擪�A�h���X						*/
/*				  (I/ )buffSize	�o�b�t�@�T�C�Y								*/
/*				  ( /O)dataSize	�쐬�����f�[�^�T�C�Y						*/
/* �߂�l		: void														*/
/* �쐬��		: (OFT)T.Kamakura											*/
/* �쐬��		: 16/10/01													*/
/* ���l			: 															*/
/****************************************************************************/
static void sd_ctrl_MakeSignalDataPart( UI_8 signalNum, const SI_32 *signalValues, UI_8 *dataBuff, UI_16 buffSize, UI_16 *dataSize )
{

	UI_8*			p = NULL;
	UI_16			totalSize = 0U;
	UI_16			length = 0U;
	UI_16			crc = 0U;
	UI_8			i = 0U;

	if( (NULL != signalValues) && (NULL != dataBuff) && (NULL != dataSize) ) {
		/* �o�͏����� */
		(void)memset(dataBuff, 0, buffSize);
		*dataSize = 0;

		/* �S�̃T�C�Y�v�Z */
		totalSize = sd_ctrl_GetSignalDataPartSize( signalNum, 0 );
		length = totalSize - (SD_CTRL_FMT_HEADER_BYTES + SD_CTRL_FMT_FOOTER_BYTES) + 1U;

		if( totalSize > buffSize ) {
			/* �o�b�t�@�s�� */
		}
		else {
			/* �f�[�^�쐬�J�n */
			p = dataBuff;

			/*==================================================================*/
			/* �w�b�_�쐬														*/
			/*==================================================================*/
			*p = SD_CTRL_FMT_HEADER_START;
			p++;
			Common_Set16( p, length );
			p += 2;
			*p = SD_CTRL_FMT_TYPE_SIGNAL_DATA;
			p++;

			/*==================================================================*/
			/* �f�[�^���쐬														*/
			/*==================================================================*/
			/* �d��ON����̃^�C���X�^���v(ms) */
			Common_Set32( p, Timer_ctrl_GetTime1ms() );
			p += 4;

			/* �G���[�R�[�h�� */
			*p = 0U;
			p++;
			/* �\��G���[�R�[�h�͌��݊i�[���Ȃ� */
			
			/* �������ݎԗ��M���� */
			*p = signalNum;
			p++;

			/* �������ݎԗ��M���̒l */
			for( i = 0U; i < signalNum; i++ ) {
				/* �M���l */
				Common_Set32( p, (UI_32)signalValues[i] );
				p += 4;
			}
			/*==================================================================*/
			/* �t�b�^�쐬														*/
			/*==================================================================*/
			crc = Common_CalcCRC( dataBuff, 1U, length + 2U );
			Common_Set16( p, crc );
			p += 2;

			*p = SD_CTRL_FMT_FOOTER_END;
			p++;

			/* �f�[�^�����i�[ */
			*dataSize = totalSize;
		}
	}

}

/****************************************************************************/
/* �֐�����		: sd_ctrl_UpdateLogDataInfo									*/
/* �@�\�̐���	: ���O���X�V												*/
/* ����			: (I/ )init	�������t���O(N_TRUE:�������^����ȊO:�X�V)		*/
/* �߂�l		: void														*/
/* �쐬��		: (OFT)T.Kamakura											*/
/* �쐬��		: 16/10/01													*/
/* ���l			: 															*/
/****************************************************************************/
static void sd_ctrl_UpdateLogDataInfo( B_8 init )
{

	T_Gnss_ctrl_nav_pvt_info*	hnr_info = NULL;
	UI_32						check_flags = 0UL;
	T_COMMON_DATE_TIME	dtm;

	if( N_TRUE == init ) {
		/*==================================================================*/
		/* ���������쎞														*/
		/*==================================================================*/
		(void)memset(&S_sd_log_data_info, 0, sizeof(S_sd_log_data_info));
	}
	/* GNSS������擾 */
	hnr_info = Gnss_cntrol_GetNavPvtInfo();

	/* �擾���������`�F�b�N */
	check_flags = sd_ctrl_CheckGnssHnrInfo( hnr_info );

	if( 0UL != (check_flags & SD_CTRL_LOG_DATA_POS_ENABLE) ) {
		/*==================================================================*/
		/* GNSS ���W�L��													*/
		/*==================================================================*/
		if( 0UL == (S_sd_log_data_info.flags & SD_CTRL_LOG_DATA_POS_ENABLE ) ) {
			/*==============================================================*/
			/* ���W���������Ȃ珉����										*/
			/*==============================================================*/
			/* �ܓx�o�x�͈͂����݂̒n�_���W�ŏ����� */
			S_sd_log_data_info.latitudeNorthernMost = hnr_info->lat;
			S_sd_log_data_info.longtudeWesternMost = hnr_info->lon;
			S_sd_log_data_info.latitudeSouthernMost = hnr_info->lat;
			S_sd_log_data_info.longtudeEasternMost = hnr_info->lon;

			/* �������σt���O�Z�b�g */
			S_sd_log_data_info.flags |= SD_CTRL_LOG_DATA_POS_ENABLE;

			OUTLOG(OutputLog("[SD][%08d] sd_ctrl_UpdateLogDataInfo(init=%d) SD_CTRL_LOG_DATA_POS_ENABLE\n", Timer_ctrl_GetTime1ms(), init );)
		}
		else {
			/*==============================================================*/
			/* ���W�����ςȂ�̈�g��										*/
			/*==============================================================*/
			/* �ܓx�o�x�͈͂����݂̒n�_���W�ōX�V */
			if( S_sd_log_data_info.latitudeNorthernMost < hnr_info->lat ) {
				/* �k�[�g�� */
				S_sd_log_data_info.latitudeNorthernMost = hnr_info->lat;
			}
			if( S_sd_log_data_info.latitudeSouthernMost > hnr_info->lat ) {
				/* ��[�g�� */
				S_sd_log_data_info.latitudeSouthernMost = hnr_info->lat;
			}
			if( S_sd_log_data_info.longtudeEasternMost < hnr_info->lon ) {
				/* ���[�g�� */
				S_sd_log_data_info.longtudeEasternMost = hnr_info->lon;
			}
			if( S_sd_log_data_info.longtudeWesternMost > hnr_info->lon ) {
				/* ���[�g�� */
				S_sd_log_data_info.longtudeWesternMost = hnr_info->lon;
			}
		}
	}

	if( 0UL != (check_flags & SD_CTRL_LOG_DATA_TIME_ENABLE) ) {
		/*==================================================================*/
		/* GNSS �����L��													*/
		/*==================================================================*/
		dtm.year	= hnr_info->year;
		dtm.month	= hnr_info->month;
		dtm.day		= hnr_info->day;
		dtm.hour	= hnr_info->hour;
		dtm.min		= hnr_info->min;
		dtm.sec		= hnr_info->sec;
		if( 0UL == (S_sd_log_data_info.flags & SD_CTRL_LOG_DATA_TIME_ENABLE ) ) {
			/*==============================================================*/
			/* �������������Ȃ珉����										*/
			/*==============================================================*/
			/* ���O�������݊J�n���� = ���O�������ݏI������ */
			Common_MakeDateTimeStr(&dtm, COMMON_DATE_TIME_STR_TYPE_YEAR_2, S_sd_log_data_info.timeLogStart);	/* �N��2���ō쐬 */
			(void)memcpy( S_sd_log_data_info.timeLogEnd, S_sd_log_data_info.timeLogStart, sizeof(S_sd_log_data_info.timeLogEnd) );

			/* �������σt���O�Z�b�g */
			S_sd_log_data_info.flags |= SD_CTRL_LOG_DATA_TIME_ENABLE;

			OUTLOG(OutputLog("[SD][%08d] sd_ctrl_UpdateLogDataInfo(init=%d) SD_CTRL_LOG_DATA_TIME_ENABLE\n", Timer_ctrl_GetTime1ms(), init );)
		}
		else {
			/*==============================================================*/
			/* ������������													*/
			/*==============================================================*/
			/* ���O�������ݏI�����Ԃ��X�V */
			Common_MakeDateTimeStr(&dtm, COMMON_DATE_TIME_STR_TYPE_YEAR_2, S_sd_log_data_info.timeLogEnd);	/* �N��2���ō쐬 */
		}
	}

}

/****************************************************************************/
/* �֐�����		: sd_ctrl_CheckGnssHnrInfo									*/
/* �@�\�̐���	: GNSS NAV-HNR or HNR-PVT��� ���`�F�b�N����				*/
/* ����			: (I/ )hnr_info	NAV-HNR or HNR-PVT���						*/
/* �߂�l		: bit�t���O�̘_���a											*/
/*				  SD_CTRL_LOG_DATA_TIME_ENABLE:	�������擾�ł��Ă���		*/
/*				  SD_CTRL_LOG_DATA_POS_ENABLE:		���W���擾�ł��Ă���	*/
/* �쐬��		: (OFT)T.Kamakura											*/
/* �쐬��		: 16/11/07													*/
/* ���l			: 															*/
/****************************************************************************/
static UI_32 sd_ctrl_CheckGnssHnrInfo( const T_Gnss_ctrl_nav_pvt_info* hnr_info )
{

	UI_32	flags = 0UL;

	if( NULL != hnr_info ) {
		switch( hnr_info->gpsFix ) {
		case GNSS_GSPFIX_NOFIX:					/* No Fix */
			break;
		case GNSS_GSPFIX_DEADRECKONIN_ONLY:		/* Dead Reckoning only */
		case GNSS_GSPFIX_2DFIX:					/* 2D-Fix */
		case GNSS_GSPFIX_3DFIX:					/* 3D-Fix */
		case GNSS_GSPFIX_GPS_AND_DEADRECKONING:	/* GPS + dead reckoning combined */
			/* ���W�������������� */
			flags = SD_CTRL_LOG_DATA_TIME_ENABLE | SD_CTRL_LOG_DATA_POS_ENABLE;
			break;
		case GNSS_GSPFIX_TIME_ONLY_FIX:			/* Time only fix */
			/* �����̂ݐ����� */
			flags = SD_CTRL_LOG_DATA_TIME_ENABLE;
			break;
		default:
			break;
		}
	}
	return flags;

}

/****************************************************************************/
/* �֐�����		: sd_ctrl_WriteRecData										*/
/* �@�\�̐���	: REC�f�[�^��������											*/
/* ����			: (I/ )dataBuff	�f�[�^�o�b�t�@								*/
/*				   (I/ )dataSize	�f�[�^�T�C�Y							*/
/* �߂�l		: void														*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 16/10/08													*/
/* ���l			: �����O�o�b�t�@�Ƀf�[�^����������							*/
/*				  �����O�o�b�t�@�̃f�[�^��SD�������݃T�C�Y�ɒB�����ꍇ��	*/
/*				  �����O�o�b�t�@�ɂ��܂����f�[�^��SD�ɏ�������				*/
/****************************************************************************/
static void sd_ctrl_WriteRecData( UI_8 *dataBuff, UI_16 dataSize )
{

	/* �����O�o�b�t�@�ւ̏�������	*/
	if (RingBuffer_GetCanWriteSize(&S_sd_rec_data_mng) >= dataSize) {
		/* �����O�o�b�t�@�ɏ�������	*/
		(void)RingBuffer_SetArrayData(&S_sd_rec_data_mng, dataBuff, dataSize);
		S_sd_ring_write_size += dataSize;
	}

	/* �����O�o�b�t�@�̃f�[�^���u���b�N�P�ʂ�SD�ɏ�������	*/
	sd_ctrl_FlushRecData(N_FALSE);

	return;

}

/****************************************************************************/
/* �֐�����		: sd_ctrl_FlushRecData										*/
/* �@�\�̐���	: �����O�o�b�t�@�̃f�[�^��SD�ɏ�������						*/
/* ����			: (I/ )allFlush	�S�f�[�^�������݃t���O						*/
/*							N_TRUE:�S�f�[�^��������							*/
/*							N_FALSE:�u���b�N��؂�܂ŏ�������				*/
/* �߂�l		: void														*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 16/10/08													*/
/* ���l			: 															*/
/****************************************************************************/
static void sd_ctrl_FlushRecData( B_8 allFlush )
{

	E_FIO_RESULT	fioRet;
	UI_32			reqNo;
	UI_32			writeSize;
	UI_8			*pBuff;

	/* �u���b�N�̓r�����疢�������݂̃f�[�^������ꍇ�̏��� */
	if ((S_sd_ring_flush_size % SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE) != 0) {	/* �u���b�N�r������̖��������݃f�[�^����	*/
		/* �u���b�N�r������u���b�N��؂�܂ł̃f�[�^��SD�ɏ�������	*/

		/* SD�ɏ������ރT�C�Y���Z�o�i���������݃f�[�^�擪����u���b�N��؂�ʒu�܂ł̃T�C�Y�j	*/
		writeSize = (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE - (S_sd_ring_flush_size % (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE);
		if ((S_sd_ring_write_size - S_sd_ring_flush_size) >= writeSize) {		/* ���������݃f�[�^��SD�������݃T�C�Y�ȏ゠��	*/
			/* SD�ɏ������ރo�b�t�@�ʒu���Z�o	*/
			pBuff = S_sd_rec_data_buff + (S_sd_ring_flush_size % ((UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE * (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_NUM));

			/* SD�ɏ������� */
			fioRet = FileIO_Write( FIO_WRITE_TYPE_FILE_OPEN, S_sd_rec_filepath, pBuff, FIO_OFS_EOF, writeSize, sd_ctrl_RecDataWriteCallback, &reqNo );
			if (fioRet != FIO_RESULT_OK) {
				/* SD�������ݎ��s���͉������Ȃ�	*/
				return;
			}
			S_sd_ring_flush_size += writeSize;
			S_sd_rec_write_req_cnt++;
		}
	}

	/* �u���b�N��؂�擪����u���b�N�P�ʂ�SD�ɏ������ޏ���	*/
	while ((S_sd_ring_write_size - S_sd_ring_flush_size) >= SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE) {	/* ���������݃f�[�^���u���b�N�T�C�Y�ȏ゠��	*/
		/* SD�ɏ������ރo�b�t�@�ʒu���Z�o	*/
		pBuff = S_sd_rec_data_buff + (S_sd_ring_flush_size % ((UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE * (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_NUM));

		/* SD�ɏ������� */
		fioRet = FileIO_Write( FIO_WRITE_TYPE_FILE_OPEN, S_sd_rec_filepath, pBuff, FIO_OFS_EOF, SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE, sd_ctrl_RecDataWriteCallback, &reqNo );
		if (fioRet != FIO_RESULT_OK) {
			/* SD�������ݎ��s���͉������Ȃ�	*/
			return;
		}
		S_sd_ring_flush_size += (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE;
		S_sd_rec_write_req_cnt++;
	}

	/* �u���b�N��؂�擪����u���b�N�P�ʂɖ����Ȃ��T�C�Y�̃f�[�^���������ޏ���	*/
	if (allFlush == N_TRUE) {				/* �����O�o�b�t�@�S�f�[�^�������݂��w�肳��Ă���	*/

		/* SD�ɏ������ރT�C�Y���Z�o	*/
		writeSize = S_sd_ring_write_size - S_sd_ring_flush_size;
		if (writeSize > 0) {

			/* SD�ɏ������ރo�b�t�@�ʒu���Z�o	*/
			pBuff = S_sd_rec_data_buff + (S_sd_ring_flush_size % ((UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE * (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_NUM));

			/* SD�ɏ������� */
			fioRet = FileIO_Write( FIO_WRITE_TYPE_FILE_OPEN, S_sd_rec_filepath, pBuff, FIO_OFS_EOF, writeSize, sd_ctrl_RecDataWriteCallback, &reqNo );
			if (fioRet != FIO_RESULT_OK) {
				/* SD�������ݎ��s���͉������Ȃ�	*/
				return;
			}
			S_sd_ring_flush_size += writeSize;
			S_sd_rec_write_req_cnt++;
		}
	}
	return;

}

/****************************************************************************/
/* �֐�����		: sd_ctrl_UpdateHeader										*/
/* �@�\�̐���	: ����w�b�_�X�V����										*/
/* ����			: void														*/
/* �߂�l		: void														*/
/* �쐬��		: (OFT)T.Kamakura											*/
/* �쐬��		: 16/11/07													*/
/* ���l			: 															*/
/****************************************************************************/
static void sd_ctrl_UpdateHeader( void )
{

	E_FIO_RESULT		fioRet;
	UI_32				reqNo;
	UI_16				headSize;
	UI_32				curTime = 0UL;

	if( S_sd_rec_write_req_cnt > 0 ) {
		/* ���ݎ����擾 */
		curTime = Timer_ctrl_GetTime1ms();
		if( SD_CTRL_UPDATE_HEADER_CYCLE_MSEC <= (curTime - S_sd_update_header_time) ) {
			/*==============================================================*/
			/* �X�V�������Ԍo��												*/
			/*==============================================================*/
			/* REC�t�@�C���̃w�b�_���쐬	*/
			sd_ctrl_MakeRecHead( S_sd_rec_head, sizeof(S_sd_rec_head), &headSize, N_FALSE );

			/* REC�t�@�C���̃w�b�_�����������ށy�X�V�z*/
			fioRet = FileIO_Write( FIO_WRITE_TYPE_FILE_OPEN, S_sd_rec_filepath, S_sd_rec_head, 0, (UI_32)headSize, sd_ctrl_RecDataWriteCallback, &reqNo );
			if( fioRet == FIO_RESULT_OK ) {
				S_sd_rec_write_req_cnt++;
				S_sd_update_header_time = curTime;
			}
/*			OUTLOG(OutputLog("[SD][%08d] sd_ctrl_UpdateHeader()\n", curTime );) */
		}
	}

}

/****************************************************************************/
/* �֐�����		: sd_ctrl_EndRec											*/
/* �@�\�̐���	: REC�I������												*/
/* ����			: void														*/
/* �߂�l		: void														*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 16/10/08													*/
/* ���l			: 															*/
/****************************************************************************/
static void sd_ctrl_EndRec( void )
{

	E_FIO_RESULT		fioRet;
	UI_32				reqNo;
	UI_16				headSize;

	if (S_sd_rec_write_req_cnt > 0) {
		/* �����O�o�b�t�@�Ɏc���Ă���f�[�^��SD�ɏ�������	*/
		sd_ctrl_FlushRecData( N_TRUE );

		/* REC�t�@�C���̃w�b�_���쐬	*/
		sd_ctrl_MakeRecHead( S_sd_rec_head, sizeof(S_sd_rec_head), &headSize, N_FALSE );

		/* REC�t�@�C���̃w�b�_�����������ށy�X�V�z*/
		fioRet = FileIO_Write( FIO_WRITE_TYPE_FILE_OPEN, S_sd_rec_filepath, S_sd_rec_head, 0, (UI_32)headSize, sd_ctrl_RecDataWriteCallback, &reqNo );
		if (fioRet == FIO_RESULT_OK) {
			S_sd_rec_write_req_cnt++;
		}
	}

}

/****************************************************************************/
/* �֐�����		: sd_ctrl_GetFileHeaderPartSize								*/
/* �@�\�̐���	: �t�@�C���w�b�_���T�C�Y�擾								*/
/* ����			: (I/ )fixedSignalNum		�Œ�ID�̐�						*/
/* 				: (I/ )vehicleSignalNum		�ϐ�ID�̐�						*/
/* �߂�l		: �t�@�C���w�b�_���T�C�Y									*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 17/08/28													*/
/* ���l			: 															*/
/****************************************************************************/
static UI_16 sd_ctrl_GetFileHeaderPartSize( UI_8 fixedSignalNum, UI_8 vehicleSignalNum )
{
	return (UI_16)(90U + (2U * (UI_16)fixedSignalNum) + (2U * (UI_16)vehicleSignalNum));

}

/****************************************************************************/
/* �֐�����		: sd_ctrl_GetIdInfoDataPartSize								*/
/* �@�\�̐���	: ID��񕔃T�C�Y�擾										*/
/* ����			: (I/ )signalNum			ID�̐�							*/
/* �߂�l		: ID��񕔃T�C�Y											*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 17/08/28													*/
/* ���l			: 															*/
/****************************************************************************/
static UI_16 sd_ctrl_GetIdInfoDataPartSize( UI_8 signalNum )
{
	return (UI_16)(8U + (2U * (UI_16)signalNum));

}

/****************************************************************************/
/* �֐�����		: sd_ctrl_GetSignalDataPartSize								*/
/* �@�\�̐���	: �ԗ���񕔃T�C�Y�擾										*/
/* ����			: (I/ )signalNum			ID�̐�							*/
/* 				: (I/ )errNum				�G���[�R�[�h�̐�				*/
/* �߂�l		: �ԗ���񕔃T�C�Y											*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 17/08/28													*/
/* ���l			: 															*/
/****************************************************************************/
static UI_16 sd_ctrl_GetSignalDataPartSize( UI_8 signalNum, UI_8 errNum )
{
	return (UI_16)((13U + (6U * (UI_16)errNum)) + (4U * (UI_16)signalNum));

}
#endif /* _VDV_APP */

#ifdef _VDV_APP
/*==========================================================================*/
/* �V�~�����[�V��������֐�													*/
/*==========================================================================*/
/****************************************************************************/
/* �֐�����		:	sd_sim_state_MainProc									*/
/* �@�\�̐���	:	�V�~�����[�V�������䃁�C��								*/
/* ����			:	void													*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/03												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_sim_state_MainProc( void )
{

	PFN_SD_SIM_STATE_PROC	state_proc = NULL;
	SD_SIM_EVENT			sim_event = SD_SIM_EVENT_NONE;
	E_SD_SIM_STAT			next_status = SD_SIM_STAT_NONE;
	B_8						fcontinue = N_TRUE;

	if( SD_SIM_STAT_COUNT > S_sd_sim_status ) {
		state_proc = S_sd_sim_state_proc[S_sd_sim_status];

		/* ��Ԋ֐����ŘA���ďo�����v�����ꂽ�ꍇ�͘A�� */
		while( (NULL != state_proc) && (N_TRUE == fcontinue) ) {
			/* �C�x���g�쐬 */
			sd_sim_state_MakeEvent( &sim_event );

			/* �f�t�H���g�͂P��ďo�� */
			fcontinue = N_FALSE;

			/* ��Ԋ֐��Ăяo�� */
			next_status = state_proc( sim_event, &fcontinue );
			if( next_status != SD_SIM_STAT_NONE ) {
				/* ��ԑJ�ڔ��� */
				sd_sim_state_ProcOnLeave( S_sd_sim_status );
				OUTLOG(OutputLog( "[SDSIM] STATUS %d->%d [EV:%08x][%08d]\n", S_sd_sim_status, next_status, sim_event, Timer_ctrl_GetTime1ms() );)
				S_sd_sim_status = next_status;
				sd_sim_state_ProcOnEnter( next_status );
			}
			state_proc = S_sd_sim_state_proc[S_sd_sim_status];
		}
		/* �o�b�N�O���E���h���� */
		switch( S_sd_sim_status ) {
		case SD_SIM_STAT_SEARCH_SIM_FILE:			/* SIM�t�@�C��������		*/
			/* �V�~�����[�V�����t�@�C�������i�J�n�j*/
			sd_sim_SearchSimFile();
			break;
		case SD_SIM_STAT_WAIT_HEADER:				/* �t�@�C���w�b�_��M�҂� 	*/
		case SD_SIM_STAT_WAIT_FIX_ID_LIST:			/* �Œ�M��ID��M�҂� 		*/
		case SD_SIM_STAT_WAIT_FIX_VALUES:			/* �Œ�ԗ�����M�҂�		*/
		case SD_SIM_STAT_WAIT_VAR_ID_LIST:			/* �ϐ��M��ID��M�҂�		*/
		case SD_SIM_STAT_WAIT_VAR_VALUES:			/* �ԗ�����M�҂� 		*/
			/* �V�~�����[�V�����t�@�C������ǂ݁i�J�n�j*/
			sd_sim_ReadSimFile();
			break;
		default:
			break;
		}
	}

}
/****************************************************************************/
/* �֐�����		:	sd_sim_state_MakeEvent									*/
/* �@�\�̐���	:	�C�x���g�쐬											*/
/* ����			:	( /O)sim_event	�쐬�����C�x���g�̊i�[��				*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/03												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_sim_state_MakeEvent( SD_SIM_EVENT* sim_event )
{

	E_SW_REC_BTN_STATUS rec_btn_status = SYS_REC_BTN_UNKNOWN;
	UI_32				work_sim_event = 0UL;
	UI_32				flags = 0UL;

	if( NULL != sim_event ) {
		flags = S_sd_sim_ctrl_info.flags;

		/* ������� */
		if( 0 != (SD_SIM_FLAG_FOUND_SIM_FILE & flags ) ) {
			/* SIM�t�@�C���������� */
			work_sim_event |= (UI_32)SD_SIM_EVENT_FOUND_SIM_FILE;
		}
		/* ��M�p�P�b�g */
		(void)sd_sim_data_AnalyzeRecvData( &S_sd_sim_ctrl_info.packet_info );
		switch( S_sd_sim_ctrl_info.packet_info.type ) {
		case SD_CTRL_FMT_TYPE_HEADER:
			work_sim_event |= (UI_32)SD_SIM_EVENT_RECV_HEADER;
			break;
		case SD_CTRL_FMT_TYPE_ID_INFO_DATA:
			if( 0 == (SD_SIM_FLAG_RECV_FIX_ID_LIST & flags) ) {
				/* �Œ�ID��񖢎�M */
				work_sim_event |= (UI_32)SD_SIM_EVENT_RECV_FIX_ID_LIST;
			}
			else {
				/* �Œ�ID����M�� */
				work_sim_event |= (UI_32)SD_SIM_EVENT_RECV_VAR_ID_LIST;
			}
			break;
		case SD_CTRL_FMT_TYPE_SIGNAL_DATA:
			if( 0 == (SD_SIM_FLAG_RECV_FIX_VALUES & flags) ) {
				/* �Œ�ԗ���񖢎�M */
				work_sim_event |= (UI_32)SD_SIM_EVENT_RECV_FIX_VALUES;
			}
			else {
				/* �Œ�ԗ�����M�� */
				if (SD_PROCESS_SUB == S_processType) {
					/* �Œ�ԗ����̓T�u�����Ŏ�M */
					work_sim_event |= (UI_32)SD_SIM_EVENT_RECV_VAR_VALUES;
				}
			}
			break;
		default:
			break;
		}

		if( 0 != (SD_SIM_FLAG_READ_ERROR & flags) ) {
			/* �t�@�C���ǂ݃G���[���� */
			work_sim_event |= (UI_32)SD_SIM_EVENT_READ_ERROR;
		}
		
		/* REC�{�^����� */
		rec_btn_status = Sw_control_GetRecButtonStatus();
		if( SYS_REC_BTN_ON_SHORTPRESS == rec_btn_status ) {
			/* �V�~�����[�V�������o���v�� */
			work_sim_event |= (UI_32)SD_SIM_EVENT_REQ_REWIND;
		}

		if( 0 == ((SD_SIM_FLAG_SEARCHING_FILE | SD_SIM_FLAG_READING_FILE)& flags) ) {
			/* �t�@�C���ǂݏI�� */
			work_sim_event |= (UI_32)SD_SIM_EVENT_END_READ;
		}

		if( SYS_OPERATION_PRE_ACC_OFF == System_control_GetStatus() ) {
			/* ACC OFF */
			work_sim_event |= (UI_32)SD_SIM_EVENT_ACC_OFF;
		}

		/* �C�x���g�i�[ */
		*sim_event = work_sim_event;
	}

}
/****************************************************************************/
/* �֐�����		:	sd_sim_state_ProcOnEnter								*/
/* �@�\�̐���	:	��ԑJ�ڎ�����:��Ԃɓ���Ƃ�							*/
/* ����			:	stat	�J�ڐ���										*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/03												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_sim_state_ProcOnEnter( E_SD_SIM_STAT stat )
{

	switch( stat ) {
	case SD_SIM_STAT_SEARCH_SIM_FILE:
		/* �V�~�����[�V���������񏉊��� */
		sd_sim_InitSimCtrlInfo( SD_SIM_INIT_MODE_REWIND_SIM );
		sd_sim_ClearTime( SD_SIM_TIME_KIND_LAST_SEARCH_START );
		break;
	case SD_SIM_STAT_WAIT_HEADER:				/* �t�@�C���w�b�_��M�҂� 	*/
		break;
	case SD_SIM_STAT_WAIT_FIX_ID_LIST:			/* �Œ�M��ID��M�҂� 		*/
		break;
	case SD_SIM_STAT_WAIT_FIX_VALUES:			/* �Œ�ԗ�����M�҂�		*/
		break;
	case SD_SIM_STAT_WAIT_VAR_ID_LIST:			/* �ϐ��M��ID��M�҂�		*/
		break;
	case SD_SIM_STAT_WAIT_VAR_VALUES:			/* �ԗ�����M�҂� 		*/
		/* �ϐ��ԗ����o�b�t�@�N���A */
		sd_sim_signal_ClearBuffer( N_FALSE );
		break;
	case SD_SIM_STAT_WAIT_STOP:					/* ��~�҂�					*/
		break;
	case SD_SIM_STAT_WAIT_STOP_ACC_OFF:			/* ACC OFF�ɂ���~�҂�	*/
		break;
	case SD_SIM_STAT_ACC_OFF:					/* ACC OFF					*/
		break;

	default:
		break;
	}

}
/****************************************************************************/
/* �֐�����		:	sd_sim_state_ProcOnLeave								*/
/* �@�\�̐���	:	��ԑJ�ڎ�����:��Ԃ��o��Ƃ�							*/
/* ����			:	stat	�J�ڌ����										*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/03												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_sim_state_ProcOnLeave( E_SD_SIM_STAT stat )
{

	switch( stat ) {
	case SD_SIM_STAT_SEARCH_SIM_FILE:
		break;
	case SD_SIM_STAT_WAIT_HEADER:				/* �t�@�C���w�b�_��M�҂� 	*/
		break;
	case SD_SIM_STAT_WAIT_FIX_ID_LIST:			/* �Œ�M��ID��M�҂� 		*/
		break;
	case SD_SIM_STAT_WAIT_FIX_VALUES:			/* �Œ�ԗ�����M�҂�		*/
		break;
	case SD_SIM_STAT_WAIT_VAR_ID_LIST:			/* �ϐ��M��ID��M�҂�		*/
		break;
	case SD_SIM_STAT_WAIT_VAR_VALUES:			/* �ԗ�����M�҂� 		*/
		break;
	case SD_SIM_STAT_WAIT_STOP:					/* ��~�҂�					*/
		break;
	case SD_SIM_STAT_WAIT_STOP_ACC_OFF:			/* ACC OFF�ɂ���~�҂�	*/
		break;
	case SD_SIM_STAT_ACC_OFF:					/* ACC OFF					*/
		break;
	default:
		break;
	}

}
/*==========================================================================*/
/* �V�~�����[�V��������֐�													*/
/*==========================================================================*/
/****************************************************************************/
/* �֐�����		:	sd_sim_InitSimCtrlInfo									*/
/* �@�\�̐���	:	�V�~�����[�V�������䏉����								*/
/* ����			:	mode	���������[�h									*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_sim_InitSimCtrlInfo( E_SD_SIM_INIT_MODE mode )
{

	switch( mode ) {
	case SD_SIM_INIT_MODE_ENTER_SIM:
		/*==================================================================*/
		/* �V�~�����[�V�������[�h�ɓ���Ƃ��̏�����							*/
		/*==================================================================*/
		/* �������0�ŏ����� */
		(void)memset(&S_sd_sim_ctrl_info, 0, sizeof(S_sd_sim_ctrl_info));
		/* ID����0�ŏ����� */
		(void)memset(&S_sd_sim_id_info, 0, sizeof(S_sd_sim_id_info));

		/* SD_SIM_STAT_NONE �ɂ��ċN�������𑖂点�� */
		S_sd_sim_status = SD_SIM_STAT_NONE;
		break;

	case SD_SIM_INIT_MODE_REWIND_SIM:
		/*==================================================================*/
		/* �V�~�����[�V�������o�����̏�����									*/
		/*==================================================================*/
		/* �������0�ŏ����� */
		(void)memset(&S_sd_sim_ctrl_info, 0, sizeof(S_sd_sim_ctrl_info));
		break;

	case SD_SIM_INIT_MODE_CONTINUE:
		/*==================================================================*/
		/* ����ǂ݌p�����̏�����											*/
		/*==================================================================*/
		/* �t�@�C���ǂݐi�s�󋵂̃t���O�𗎂Ƃ� */
		S_sd_sim_ctrl_info.flags &= (~(
			SD_SIM_FLAG_RECV_HEADER			|	/* �w�b�_��M��				*/
			SD_SIM_FLAG_RECV_FIX_ID_LIST	|	/* �Œ�ID���X�g�擾��		*/
			SD_SIM_FLAG_RECV_FIX_VALUES		|	/* �Œ�ԗ����擾��		*/
			SD_SIM_FLAG_RECV_VAR_ID_LIST	|	/* �ϐ�ID���X�g�擾��		*/
			SD_SIM_FLAG_RECV_VAR_VALUES		|	/* �ϐ��ԗ����擾��		*/
			SD_SIM_FLAG_READ_ERROR			|	/* READ�G���[����			*/
			0UL));

		/* �t�@�C���ǂ݃T�C�Y�������� */
		S_sd_sim_ctrl_info.sim_file_read_size = 0UL;
		break;

	default:
		break;
	}

}
/****************************************************************************/
/* �֐�����		:	sd_sim_SearchSimFile									*/
/* �@�\�̐���	:	�V�~�����[�V�����t�@�C�������i�J�n�j					*/
/* ����			:	void													*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_sim_SearchSimFile( void )
{

	E_COMMON_RESULT	res;

	if( 0 == (S_sd_sim_ctrl_info.flags & (SD_SIM_FLAG_SEARCHING_FILE|SD_SIM_FLAG_FOUND_SIM_FILE)) ) {
		/*==================================================================*/
		/* �t�@�C���������ł͂Ȃ��A�t�@�C���p�X������						*/
		/*==================================================================*/
		/* ���܂�p�ɂɂ��Ȃ����߂Ƀ^�C���A�E�g�`�F�b�N */
		if( 0 != sd_sim_CheckTimeout( SD_SIM_TIME_KIND_LAST_SEARCH_START, SD_SIM_SEARCH_INTERVAL_MSEC ) ) {
			/*==============================================================*/
			/* �Ԋu�󂢂Ă���Ό���OK										*/
			/*==============================================================*/
			/* �t�@�C���p�X�o�b�t�@�N���A */
			(void)memset(S_sd_sim_ctrl_info.sim_file_path, 0, sizeof(S_sd_sim_ctrl_info.sim_file_path));

			/* �t�@�C�������J�n */
			res = Common_SerchVdvFile( COMMON_VDV_FILE_TYPE_SIM, sd_sim_SearchSimFileCB );
			if( COMMON_RESULT_OK == res ) {
				/* �t�@�C�������J�n�������L�^ */
				sd_sim_RegTime( SD_SIM_TIME_KIND_LAST_SEARCH_START );

				/* �������t���O�𗧂Ă� */
				S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_SEARCHING_FILE;
			}
		}
	}

}
/* �R�[���o�b�N�֐� */
/****************************************************************************/
/* �֐�����		:	sd_sim_SearchSimFileCB									*/
/* �@�\�̐���	:	�V�~�����[�V�����t�@�C�������R�[���o�b�N�֐�			*/
/* ����			:	(I/ )retinfo	��������								*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/10/19												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_sim_SearchSimFileCB( const T_COMMON_SRC_VDVF_RET *retinfo )
{

	if( NULL != retinfo ) {
		if( N_TRUE == retinfo->exist ) {
			/* �p�X���擾�ł��� */
			(void)strcpy( (CH_8*)S_sd_sim_ctrl_info.sim_file_path, (const CH_8*)retinfo->filepath );
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_FOUND_SIM_FILE;

			OUTLOG(OutputLog("[SD] found path=[%s]\n", S_sd_sim_ctrl_info.sim_file_path);)
		}
		/* �������t���OOFF */
		S_sd_sim_ctrl_info.flags &= (~SD_SIM_FLAG_SEARCHING_FILE);
	}
	return;

}

/****************************************************************************/
/* �֐�����		:	sd_sim_ReadSimFile										*/
/* �@�\�̐���	:	�V�~�����[�V�����t�@�C������ǂ݁i�J�n�j				*/
/* ����			:	void													*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_sim_ReadSimFile( void )
{

	E_FIO_RESULT	res;
	UI_32			reqno = 0UL;
	UI_32			ofs = 0UL;
	UI_16			block_size = 0U;
	UI_8*			block = NULL;
	B_8				bret = N_FALSE;

	if( 0 != (S_sd_sim_ctrl_info.flags & SD_SIM_FLAG_FOUND_SIM_FILE) ) {
		/* �V�~�����[�V�����t�@�C�������� */
		if( 0 == (S_sd_sim_ctrl_info.flags & SD_SIM_FLAG_READING_FILE) ) {
			/*==============================================================*/
			/* READ���ł͂Ȃ�												*/
			/*==============================================================*/
			if( 0 != (S_sd_sim_ctrl_info.flags & SD_SIM_FLAG_REQ_REWIND ) ) {
				/*==========================================================*/
				/* ���o���v������											*/
				/*==========================================================*/
				S_sd_sim_ctrl_info.flags &= (~SD_SIM_FLAG_REQ_REWIND);

				/* ��M�o�b�t�@�N���A */
				(void)sd_sim_data_ReadRecvData( sd_sim_data_GetRecvDataSize(), NULL, N_TRUE );

				/* �t�@�C���̓��o���i�Œ蕔�̒���܂Ŗ߂��j*/
				S_sd_sim_ctrl_info.sim_file_read_size = sd_sim_signal_GetFixPartSize();

				/* �e���M�ςݏ��N���A(�Œ蕔�ǂݍ��ݍς݂��l��) */
				S_sd_sim_ctrl_info.flags &= ~(SD_SIM_FLAG_CHANGE_VAR_ID_LIST );
			}
			/* �V�~�����[�V�����f�[�^�ǂ݃u���b�N�擾 */
			bret = sd_sim_GetReadBlock( &block, &block_size );
			if( (N_TRUE == bret) && (NULL != block) && (0U < block_size) ) {
				/*==========================================================*/
				/* �ǂݍ��݉\�ȋ󂫃u���b�N������							*/
				/*==========================================================*/
				/* �ǂݍ��ݍσt�@�C���T�C�Y�̑�������ǂ� */
				ofs = S_sd_sim_ctrl_info.sim_file_read_size;
				res = FileIO_Read( S_sd_sim_ctrl_info.sim_file_path, block, ofs, (UI_32)block_size, sd_sim_ReadSimFileCB, &reqno );
				if( FIO_RESULT_OK == res ) {
					/* READ���t���O�𗧂Ă� */
					S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_READING_FILE;
				}
			}
		}
	}

}
/* �R�[���o�b�N�֐� */
/****************************************************************************/
/* �֐�����		:	sd_sim_ReadSimFileCB									*/
/* �@�\�̐���	:	�V�~�����[�V�����t�@�C���ǂݍ��݃R�[���o�b�N�֐�		*/
/* ����			:	(I/ )reqinfo	�v�����								*/
/* 				:	(I/ )retinfo	��������								*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_sim_ReadSimFileCB( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo )
{

	UI_32	fix_part_size = 0L;
	UI_16	mask_level;

	if( (NULL != reqinfo) && (NULL != retinfo) ) {
		/* ���݂̊��荞�݃��x�����擾�����C������(SD����̃��C������)�̊��荞�݂��֎~�ɂ��� */
		(void)IntrDrv_GetMaskLevel(&mask_level);
		(void)IntrDrv_SetMaskLevel( MAIN_INTR_LEVEL );

		if( FIO_RESULT_OK == retinfo->result ) {
			/*==============================================================*/
			/* READ����														*/
			/*==============================================================*/
			if( 0 == (S_sd_sim_ctrl_info.flags & SD_SIM_FLAG_GET_FILE_SIZE) ) {
				/* �V�~�����[�V�����t�@�C���T�C�Y���擾�Ȃ�擾 */
				S_sd_sim_ctrl_info.sim_file_size = retinfo->fsize;
				S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_GET_FILE_SIZE;
			}
			/* �����ݍσT�C�Y�X�V�i�ʎZ�j */
			S_sd_sim_ctrl_info.sim_total_write_size += retinfo->bytes;

			/* READ�σT�C�Y�X�V�i���̃t�@�C���j */
			S_sd_sim_ctrl_info.sim_file_read_size += retinfo->bytes;
			if( S_sd_sim_ctrl_info.sim_file_read_size >= S_sd_sim_ctrl_info.sim_file_size ) {
				fix_part_size = sd_sim_signal_GetFixPartSize();
				if( 0 < fix_part_size ) {
					/* �t�@�C���I�[�ɒB������Œ蕔�̒���܂Ŗ߂� */
					S_sd_sim_ctrl_info.sim_file_read_size = fix_part_size;

					/* �e���M�ςݏ��N���A(�Œ蕔�ǂݍ��ݍς݂��l��) */
					S_sd_sim_ctrl_info.flags &= ~(SD_SIM_FLAG_CHANGE_VAR_ID_LIST );
				}
				else {
					/* �t�@�C���I�[�ɒB������擪�ɖ߂� */
					S_sd_sim_ctrl_info.sim_file_read_size = 0;

					/* �e���M�ςݏ��N���A */
					S_sd_sim_ctrl_info.flags &= ~(SD_SIM_FLAG_RECV_HEADER | SD_SIM_FLAG_RECV_FIX_ID_LIST | SD_SIM_FLAG_RECV_FIX_VALUES | SD_SIM_FLAG_RECV_VAR_ID_LIST | SD_SIM_FLAG_RECV_VAR_VALUES | SD_SIM_FLAG_CHANGE_VAR_ID_LIST );
				}
			}
		}
		else {
			/*==============================================================*/
			/* READ�G���[													*/
			/*==============================================================*/
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_READ_ERROR;
		}
		/* READ���t���O�𗎂Ƃ� */
		S_sd_sim_ctrl_info.flags &= (~SD_SIM_FLAG_READING_FILE);

		/* ���荞�݃��x���𕜋A���� */
		(void)IntrDrv_SetMaskLevel( mask_level );
	}

}
/****************************************************************************/
/* �֐�����		:	sd_sim_GetReadBlock										*/
/* �@�\�̐���	:	�V�~�����[�V�����f�[�^�ǂ݃u���b�N�擾					*/
/* ����			:	( /O)block		�ǂݍ��݃u���b�N�擪�A�h���X			*/
/*					( /O)block_size	�ǂݍ��݃u���b�N�T�C�Y					*/
/* �߂�l		:	N_TRUE: ����^N_FALSE: �ُ�								*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static B_8 sd_sim_GetReadBlock( UI_8** block, UI_16* block_size )
{

	UI_16	block_idx = 0U;
	UI_16	last_block_size = 0U;
	UI_16	temp_size = 0U;
	B_8		bret = N_FALSE;
	UI_16	empty_size = 0U;

	if( (NULL != block) && (NULL != block_size) ) {
		/* ���ʂ������� */
		*block = NULL;
		*block_size = 0U;

		/* �󂫃T�C�Y���擾 */
		empty_size = sizeof(S_sd_rec_data_buff) - sd_sim_data_GetRecvDataSize();

		/* ���ݏ������܂�Ă���I�[�ʒu���v�Z */
		block_idx = (UI_16)((S_sd_sim_ctrl_info.sim_total_write_size / (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE) % (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_NUM);
		last_block_size = (UI_16)(S_sd_sim_ctrl_info.sim_total_write_size % (UI_32)SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE);

		if( 0U == last_block_size ) {
			/* �u���b�N�T�C�Y��������ǂ݂��܂�Ă���ꍇ */
			if( SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE <= empty_size ) {
				/* �P�u���b�N�󂢂Ă��� */
				*block = (S_sd_rec_data_buff + (SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE * block_idx));
				*block_size = SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE;
			}
		}
		else {
			/* ���܂�u���b�N������ꍇ�u���b�N�̎c��𖄂߂� */
			temp_size = SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE - last_block_size;
			if( temp_size <= empty_size ) {
				/* �u���b�N�c�蕪�󂢂Ă��� */
				*block = (S_sd_rec_data_buff + ((SD_CTRL_REC_DATA_WRITE_BLOCK_SIZE * block_idx) + last_block_size));
				*block_size = temp_size;
			}
		}
		bret = N_TRUE;
	}
	return bret;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_RegTime											*/
/* �@�\�̐���	:	���ԓo�^												*/
/* ����			:	(I/ )kind	���Ԏ��									*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_sim_RegTime( E_SD_SIM_TIME_KIND kind )
{

	UI_32	flag = 1UL << kind;

	if( SD_SIM_TIME_KIND_COUNT > kind ) {
		S_sd_sim_ctrl_info.time[kind] = Timer_ctrl_GetTime1ms();
		S_sd_sim_ctrl_info.time_flags |= flag;
	}

}
/****************************************************************************/
/* �֐�����		:	sd_sim_ClearTime										*/
/* �@�\�̐���	:	���ԃN���A												*/
/* ����			:	(I/ )kind	���Ԏ��									*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_sim_ClearTime( E_SD_SIM_TIME_KIND kind )
{

	UI_32	flag = 1UL << kind;

	if( SD_SIM_TIME_KIND_COUNT > kind ) {
		S_sd_sim_ctrl_info.time_flags &= (~flag);
	}

}
/****************************************************************************/
/* �֐�����		:	sd_sim_CheckTimeout										*/
/* �@�\�̐���	:	�^�C���A�E�g�`�F�b�N									*/
/* ����			:	(I/ )kind	���Ԏ��									*/
/*					(I/ )msec	�^�C���A�E�g����(msec)						*/
/* �߂�l		:	1	�^�C���A�E�g���Ԍo��								*/
/*					0	�^�C���A�E�g���Ԍo�߂��Ă��Ȃ�						*/
/*					-1	���ԓo�^����Ă��Ȃ�								*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static SI_32 sd_sim_CheckTimeout( E_SD_SIM_TIME_KIND kind, UI_32 msec )
{

	SI_32	ret = -1;
	UI_32	flag = 1UL << kind;
	UI_32	now = 0UL;

	if( SD_SIM_TIME_KIND_COUNT > kind ) {
		if( 0 != (S_sd_sim_ctrl_info.time_flags & flag) ) {
			now = Timer_ctrl_GetTime1ms();
			if( (now - S_sd_sim_ctrl_info.time[kind]) >= msec ) {
				/* �^�C���A�E�g���o */
				ret = 1L;
			}
			else {
				/* �^�C���A�E�g�Ȃ� */
				ret = 0L;
			}
		}
	}
	return ret;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_ClearReadError									*/
/* �@�\�̐���	:	���[�h�G���[�N���A										*/
/* ����			:	void													*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_sim_ClearReadError( void )
{

	S_sd_sim_ctrl_info.flags &= (~SD_SIM_FLAG_READ_ERROR);

}
/*==========================================================================*/
/* ��Ԋ֐�(�V�~�����[�V����)												*/
/*==========================================================================*/
/****************************************************************************/
/* �֐�����		:	sd_sim_stat_None										*/
/* �@�\�̐���	:	��Ԋ֐�	SD_SIM_STAT_NONE							*/
/* ����			:	(I/ )sim_event	�V�~�����[�V�����C�x���g				*/
/* 				:	( /O)fcontinue	�����p���t���O							*/
/* �߂�l		:	SD_SIM_STAT_XXXX	�J�ڐ���							*/
/*					SD_SIM_STAT_NONE	��ԑJ�ڂȂ�						*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_None( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;

	if( NULL != fcontinue ) {
		*fcontinue = N_FALSE;
	}

	return next_stat;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_stat_SearchSimFile								*/
/* �@�\�̐���	:	��Ԋ֐�	SD_SIM_STAT_SEARCH_SIM_FILE					*/
/* ����			:	(I/ )sim_event	�V�~�����[�V�����C�x���g				*/
/* 				:	( /O)fcontinue	�����p���t���O							*/
/* �߂�l		:	SD_SIM_STAT_XXXX	�J�ڐ���							*/
/*					SD_SIM_STAT_NONE	��ԑJ�ڂȂ�						*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_SearchSimFile( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_FOUND_SIM_FILE & sim_event) ) {
			/* SIM�t�@�C���������� */

			next_stat = SD_SIM_STAT_WAIT_HEADER;
		}
		else if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* �t�@�C���ǂ݃G���[���� */

			/* READ�G���[�N���A */
			sd_sim_ClearReadError();
		}
		else if( 0 != (SD_SIM_EVENT_ACC_OFF & sim_event) ) {
			/* ACC OFF */

			next_stat = SD_SIM_STAT_WAIT_STOP_ACC_OFF;
		}
		else {
			/* �������Ȃ� */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_stat_WaitHeader									*/
/* �@�\�̐���	:	��Ԋ֐�	SD_SIM_STAT_WAIT_HEADER						*/
/* ����			:	(I/ )sim_event	�V�~�����[�V�����C�x���g				*/
/* 				:	( /O)fcontinue	�����p���t���O							*/
/* �߂�l		:	SD_SIM_STAT_XXXX	�J�ڐ���							*/
/*					SD_SIM_STAT_NONE	��ԑJ�ڂȂ�						*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_WaitHeader( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* �t�@�C���ǂ݃G���[���� */

			/* READ�G���[�N���A */
			sd_sim_ClearReadError();

			next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_HEADER & sim_event) ) {
			/* �t�@�C���w�b�_��M */
			
			/* �w�b�_��� */
			(void)sd_sim_signal_AnalyzeHeader( &S_sd_sim_ctrl_info.packet_info );
			/* �p�P�b�g�N���A */
			sd_sim_data_ClearRecvPacket( &S_sd_sim_ctrl_info.packet_info );

			/* �t�H�[�}�b�g��͂͘A�����s�ō������� */
			*fcontinue = N_TRUE;

			next_stat = SD_SIM_STAT_WAIT_FIX_ID_LIST;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_ID_LIST & sim_event) ) {
			/* �Œ�ID����M */

			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_VALUES & sim_event) ) {
			/* �Œ�ԗ�����M */

			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_ID_LIST & sim_event) ) {
			/* �ϐ�ID����M */

			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_VALUES & sim_event) ) {
			/* �ϐ��ԗ�����M */

			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_ACC_OFF & sim_event) ) {
			/* ACC OFF */

			next_stat = SD_SIM_STAT_WAIT_STOP_ACC_OFF;
		}
		else {
			/* �������Ȃ� */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_stat_WaitFixIdList								*/
/* �@�\�̐���	:	��Ԋ֐�	SD_SIM_STAT_WAIT_FIX_ID_LIST				*/
/* ����			:	(I/ )sim_event	�V�~�����[�V�����C�x���g				*/
/* 				:	( /O)fcontinue	�����p���t���O							*/
/* �߂�l		:	SD_SIM_STAT_XXXX	�J�ڐ���							*/
/*					SD_SIM_STAT_NONE	��ԑJ�ڂȂ�						*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_WaitFixIdList( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* �t�@�C���ǂ݃G���[���� */

			/* READ�G���[�N���A */
			sd_sim_ClearReadError();

			next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_HEADER & sim_event) ) {
			/* �t�@�C���w�b�_��M */
			
			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_ID_LIST & sim_event) ) {
			/* �Œ�ID����M */

			/* �Œ�M��ID�i�[ */
			(void)sd_sim_signal_SetIDList( N_TRUE, &S_sd_sim_ctrl_info.packet_info );
			/* �p�P�b�g�N���A */
			sd_sim_data_ClearRecvPacket( &S_sd_sim_ctrl_info.packet_info );

			/* �t�H�[�}�b�g��͂͘A�����s�ō������� */
			*fcontinue = N_TRUE;

			next_stat = SD_SIM_STAT_WAIT_FIX_VALUES;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_VALUES & sim_event) ) {
			/* �Œ�ԗ�����M */

			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_ID_LIST & sim_event) ) {
			/* �ϐ�ID����M */

			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_VALUES & sim_event) ) {
			/* �ϐ��ԗ�����M */

			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_ACC_OFF & sim_event) ) {
			/* ACC OFF */

			next_stat = SD_SIM_STAT_WAIT_STOP_ACC_OFF;
		}
		else {
			/* �������Ȃ� */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_stat_WaitFixValues								*/
/* �@�\�̐���	:	��Ԋ֐�	SD_SIM_STAT_WAIT_FIX_VALUES					*/
/* ����			:	(I/ )sim_event	�V�~�����[�V�����C�x���g				*/
/* 				:	( /O)fcontinue	�����p���t���O							*/
/* �߂�l		:	SD_SIM_STAT_XXXX	�J�ڐ���							*/
/*					SD_SIM_STAT_NONE	��ԑJ�ڂȂ�						*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_WaitFixValues( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* �t�@�C���ǂ݃G���[���� */

			/* READ�G���[�N���A */
			sd_sim_ClearReadError();

			next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_HEADER & sim_event) ) {
			/* �t�@�C���w�b�_��M */
			
			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_ID_LIST & sim_event) ) {
			/* �Œ�ID����M */

			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_VALUES & sim_event) ) {
			/* �Œ�ԗ�����M */

			/* �Œ�ԗ����i�[ */
			(void)sd_sim_signal_SetValues( N_TRUE, &S_sd_sim_ctrl_info.packet_info );
			/* �p�P�b�g�N���A */
			sd_sim_data_ClearRecvPacket( &S_sd_sim_ctrl_info.packet_info );

			/* �t�H�[�}�b�g��͂͘A�����s�ō������� */
			*fcontinue = N_TRUE;

			next_stat = SD_SIM_STAT_WAIT_VAR_ID_LIST;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_ID_LIST & sim_event) ) {
			/* �ϐ�ID����M */

			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_VALUES & sim_event) ) {
			/* �ϐ��ԗ�����M */

			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_ACC_OFF & sim_event) ) {
			/* ACC OFF */

			next_stat = SD_SIM_STAT_WAIT_STOP_ACC_OFF;
		}
		else {
			/* �������Ȃ� */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_stat_WaitVarIdList								*/
/* �@�\�̐���	:	��Ԋ֐�	SD_SIM_STAT_WAIT_VAR_ID_LIST				*/
/* ����			:	(I/ )sim_event	�V�~�����[�V�����C�x���g				*/
/* 				:	( /O)fcontinue	�����p���t���O							*/
/* �߂�l		:	SD_SIM_STAT_XXXX	�J�ڐ���							*/
/*					SD_SIM_STAT_NONE	��ԑJ�ڂȂ�						*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_WaitVarIdList( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* �t�@�C���ǂ݃G���[���� */

			/* READ�G���[�N���A */
			sd_sim_ClearReadError();

			next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_HEADER & sim_event) ) {
			/* �t�@�C���w�b�_��M */
			
			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_ID_LIST & sim_event) ) {
			/* �Œ�ID����M */

			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_VALUES & sim_event) ) {
			/* �Œ�ԗ�����M */

			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_ID_LIST & sim_event) ) {
			/* �ϐ�ID����M */

			/* �ϐ��M��ID�i�[ */
			(void)sd_sim_signal_SetIDList( N_FALSE, &S_sd_sim_ctrl_info.packet_info );
			/* �p�P�b�g�N���A */
			sd_sim_data_ClearRecvPacket( &S_sd_sim_ctrl_info.packet_info );

			/* �t�H�[�}�b�g��͂͘A�����s�ō������� */
			*fcontinue = N_TRUE;

			next_stat = SD_SIM_STAT_WAIT_VAR_VALUES;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_VALUES & sim_event) ) {
			/* �ϐ��ԗ�����M */

			/* �t�H�[�}�b�g�s���̂��ߒ�~��҂��čŏ�������Ȃ��� */
			next_stat = SD_SIM_STAT_WAIT_STOP;
		}
		else if( 0 != (SD_SIM_EVENT_ACC_OFF & sim_event) ) {
			/* ACC OFF */

			next_stat = SD_SIM_STAT_WAIT_STOP_ACC_OFF;
		}
		else {
			/* �������Ȃ� */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_stat_WaitVarValues								*/
/* �@�\�̐���	:	��Ԋ֐�	SD_SIM_STAT_WAIT_VAR_VALUES					*/
/* ����			:	(I/ )sim_event	�V�~�����[�V�����C�x���g				*/
/* 				:	( /O)fcontinue	�����p���t���O							*/
/* �߂�l		:	SD_SIM_STAT_XXXX	�J�ڐ���							*/
/*					SD_SIM_STAT_NONE	��ԑJ�ڂȂ�						*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_WaitVarValues( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* �t�@�C���ǂ݃G���[���� */

			/* READ�G���[�N���A */
			sd_sim_ClearReadError();

			next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_HEADER & sim_event) ) {
			/* �t�@�C���w�b�_��M */
			
			/* �p�P�b�g�N���A */
			OUTLOG(OutputLog("[SD] SKIP HEADER(%d)\n", S_sd_sim_ctrl_info.packet_info.packet_bytes);)
			sd_sim_data_ClearRecvPacket(&S_sd_sim_ctrl_info.packet_info);

			/* ��M��񂾂��ێ� */
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_HEADER;

			/* �������ăX�L�b�v */
			*fcontinue = N_TRUE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_ID_LIST & sim_event) ) {
			/* �Œ�ID����M */

			/* �p�P�b�g�N���A */
			OUTLOG(OutputLog("[SD] SKIP FIX ID LIST(%d)\n", S_sd_sim_ctrl_info.packet_info.packet_bytes);)
			sd_sim_data_ClearRecvPacket(&S_sd_sim_ctrl_info.packet_info);

			/* ��M��񂾂��ێ� */
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_FIX_ID_LIST;

			/* �������ăX�L�b�v */
			*fcontinue = N_TRUE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_FIX_VALUES & sim_event) ) {
			/* �Œ�ԗ�����M */

			/* �p�P�b�g�N���A */
			OUTLOG(OutputLog("[SD] SKIP FIX VALUES(%d)\n", S_sd_sim_ctrl_info.packet_info.packet_bytes);)
			sd_sim_data_ClearRecvPacket(&S_sd_sim_ctrl_info.packet_info);

			/* ��M��񂾂��ێ� */
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_FIX_VALUES;

			/* �������ăX�L�b�v */
			*fcontinue = N_TRUE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_ID_LIST & sim_event) ) {
			/* �ϐ�ID����M */

			/* �ϐ��M��ID�i�[ */
			(void)sd_sim_signal_SetIDList( N_FALSE, &S_sd_sim_ctrl_info.packet_info );
			/* �ϐ�ID���X�g�̕ύX���L�^ */
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_CHANGE_VAR_ID_LIST;

			/* �p�P�b�g�N���A */
			sd_sim_data_ClearRecvPacket( &S_sd_sim_ctrl_info.packet_info );
			/* �ϐ��ԗ����o�b�t�@�N���A */
			sd_sim_signal_ClearBuffer( N_FALSE );

			/* �t�H�[�}�b�g��͂͘A�����s�ō������� */
			*fcontinue = N_TRUE;
		}
		else if( 0 != (SD_SIM_EVENT_RECV_VAR_VALUES & sim_event) ) {
			/* �ϐ��ԗ�����M */

			/* �ϐ��ԗ����i�[ */
			(void)sd_sim_signal_SetValues( N_FALSE, &S_sd_sim_ctrl_info.packet_info );
			/* �p�P�b�g�N���A */
			sd_sim_data_ClearRecvPacket( &S_sd_sim_ctrl_info.packet_info );
		}
		else if( 0 != (SD_SIM_EVENT_REQ_REWIND & sim_event) ) {
			/* �V�~�����[�V�������o���v�� */

			/* ���o���v���t���O���L�^ */
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_REQ_REWIND;
		}
		else if( 0 != (SD_SIM_EVENT_ACC_OFF & sim_event) ) {
			/* ACC OFF */

			next_stat = SD_SIM_STAT_WAIT_STOP_ACC_OFF;
		}
		else {
			/* �������Ȃ� */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_stat_WaitStop									*/
/* �@�\�̐���	:	��Ԋ֐�	SD_SIM_STAT_WAIT_STOP						*/
/* ����			:	(I/ )sim_event	�V�~�����[�V�����C�x���g				*/
/* 				:	( /O)fcontinue	�����p���t���O							*/
/* �߂�l		:	SD_SIM_STAT_XXXX	�J�ڐ���							*/
/*					SD_SIM_STAT_NONE	��ԑJ�ڂȂ�						*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_WaitStop( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* �t�@�C���ǂ݃G���[���� */

			/* READ�G���[�N���A */
			sd_sim_ClearReadError();

			next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;
		}
		else if( 0 != (SD_SIM_EVENT_END_READ & sim_event) ) {
			/* �t�@�C���ǂݏI�� */

			/* �t�@�C�������肩��ĊJ */
			next_stat = SD_SIM_STAT_SEARCH_SIM_FILE;
		}
		else if( 0 != (SD_SIM_EVENT_ACC_OFF & sim_event) ) {
			/* ACC OFF */

			next_stat = SD_SIM_STAT_WAIT_STOP_ACC_OFF;
		}
		else {
			/* �������Ȃ� */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_stat_WaitStopAccOff								*/
/* �@�\�̐���	:	��Ԋ֐�	SD_SIM_STAT_WAIT_STOP_ACC_OFF				*/
/* ����			:	(I/ )sim_event	�V�~�����[�V�����C�x���g				*/
/* 				:	( /O)fcontinue	�����p���t���O							*/
/* �߂�l		:	SD_SIM_STAT_XXXX	�J�ڐ���							*/
/*					SD_SIM_STAT_NONE	��ԑJ�ڂȂ�						*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_WaitStopAccOff( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

		if( 0 != (SD_SIM_EVENT_READ_ERROR & sim_event) ) {
			/* �t�@�C���ǂ݃G���[���� */

			/* READ�G���[�N���A */
			sd_sim_ClearReadError();

			next_stat = SD_SIM_STAT_ACC_OFF;
		}
		else if( 0 != (SD_SIM_EVENT_END_READ & sim_event) ) {
			/* �t�@�C���ǂݏI�� */

			next_stat = SD_SIM_STAT_ACC_OFF;
		}
		else {
			/* �������Ȃ� */
		}
	}
	return next_stat;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_stat_AccOff										*/
/* �@�\�̐���	:	��Ԋ֐�	SD_SIM_STAT_ACC_OFF							*/
/* ����			:	(I/ )sim_event	�V�~�����[�V�����C�x���g				*/
/* 				:	( /O)fcontinue	�����p���t���O							*/
/* �߂�l		:	SD_SIM_STAT_XXXX	�J�ڐ���							*/
/*					SD_SIM_STAT_NONE	��ԑJ�ڂȂ�						*/
/* �쐬��		:	(OFT)T.Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static E_SD_SIM_STAT sd_sim_stat_AccOff( SD_SIM_EVENT sim_event, B_8* fcontinue )
{

	E_SD_SIM_STAT	next_stat = SD_SIM_STAT_NONE;

	if( NULL != fcontinue ) {

		*fcontinue = N_FALSE;

	}
	return next_stat;

}
/*==========================================================================*/
/* ��M�f�[�^��͊֐�														*/
/*==========================================================================*/
/****************************************************************************/
/* �֐�����		:	sd_sim_data_AnalyzeRecvData								*/
/* �@�\�̐���	:	��M�f�[�^���											*/
/* ����			:	( /O)packet_info	��͂œ����p�P�b�g�̏��			*/
/* �߂�l		:	E_RC	��������										*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/03												*/
/* ���l			:	��M�f�[�^�Ȃ��ꍇ SD_CTRL_FMT_TYPE_INVALID ���o�͂���	*/
/****************************************************************************/
static E_RC sd_sim_data_AnalyzeRecvData( T_SD_CTRL_PACKET_INFO* packet_info )
{

	E_RC	rc = E_RC_OK;
	SI_32	ret;

	if( NULL == packet_info ) {
		rc = E_RC_NG_PARAM;
	}
	else {
		/* �p�P�b�g���𖳌��l�ŏ����� */
		(void)memset( packet_info, 0, sizeof(T_SD_CTRL_PACKET_INFO) );
		packet_info->type = SD_CTRL_FMT_TYPE_INVALID;

		/* ��M�o�b�t�@�� START �}�[�N�܂œǂݔ�΂� */
		ret = sd_sim_data_SkipToSTART();
		/* 1: START ���� */
		/* 0: START �Ȃ� */
		while( 1L == ret ) {
			/* �p�P�b�g�����P���o�� */
			ret = sd_sim_data_GetPacketInfo( packet_info );
			if( 1L == ret ) {
				/* 1: �p�P�b�g���� */
				break;
			}
			else if( 0L == ret ) {
				/* 0: �p�P�b�g�ł͂Ȃ��i�ُ�p�P�b�g���܂ށj */
				/* START�}�[�N���΂��Ď��̉�� */
				(void)sd_sim_data_ReadRecvData( 1U, NULL, N_TRUE ); 
			}
			else {
				/* -1: �f�[�^�s���Ŕ���s�\ */
				rc = E_RC_NG;
				break;
			}
			/* ��M�o�b�t�@�� START �}�[�N�܂œǂݔ�΂� */
			ret = sd_sim_data_SkipToSTART();
		}
	}
	return rc;

}
/****************************************************************************/
/* �֐�����		:	btc_data_SkipToSTART									*/
/* �@�\�̐���	:	��M�o�b�t�@�� START �}�[�N�܂œǂݔ�΂�				*/
/* ����			:	�Ȃ�													*/
/* �߂�l		:	1: START ����											*/
/* �߂�l		:	0: START �Ȃ�											*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/03												*/
/* ���l			:															*/
/****************************************************************************/
static SI_32 sd_sim_data_SkipToSTART( void )
{

	SI_32	ret = 0L;
	UI_16	data_bytes = 0L;
	UI_16	skip_bytes = 0L;
	UI_8	buf[16];	/* �K���ȃu���b�N�T�C�Y�ŏ��� */
	UI_16	buf_bytes =	sizeof(buf);

	/* �c��f�[�^�o�C�g���擾 */
	data_bytes = sd_sim_data_GetRecvDataSize();
	while( 0U < data_bytes ) {
		if( data_bytes > buf_bytes ) {
			data_bytes = buf_bytes;
		}
		/* �o�b�t�@�Q�� */
		(void)sd_sim_data_ReadRecvData( data_bytes, buf, N_FALSE );

		/* START �}�[�N��T�� */
		skip_bytes = 0U;
		while( skip_bytes < data_bytes ) {
			if( SD_CTRL_FMT_HEADER_START == buf[skip_bytes] ) {
				/* ���� */
				ret = 1L;
				break;
			}
			skip_bytes++;
		}
		if( 0U < skip_bytes ) {
			/* �݂���Ȃ������f�[�^���o�b�t�@����̂Ă� */
			(void)sd_sim_data_ReadRecvData( skip_bytes, NULL, N_TRUE );
		}
		if( 1L == ret ) {
			/* ���[�v�𔲂��ďI�� */
			break;
		}
		/* �c��f�[�^�o�C�g���擾 */
		data_bytes = sd_sim_data_GetRecvDataSize();
	}
	return ret;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_data_ClearRecvPacket								*/
/* �@�\�̐���	:	��M�o�b�t�@����͂��ăp�P�b�g���𓾂�				*/
/* ����			:	(I/O)packet_info	��͂œ����p�P�b�g�̏��			*/
/* �߂�l		:	1:  �p�P�b�g����										*/
/* �߂�l		:	0:  �p�P�b�g�ł͂Ȃ��i�ُ�p�P�b�g���܂ށj				*/
/* �߂�l		:	-1: �f�[�^�s���Ŕ���s�\								*/
/* �߂�l		:	-2:	�����s��											*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/03												*/
/* ���l			:	�p�P�b�g���Ȃ��ꍇ SD_CTRL_FMT_TYPE_INVALID ���o�͂���	*/
/****************************************************************************/
static void sd_sim_data_ClearRecvPacket( T_SD_CTRL_PACKET_INFO* packet_info )
{

	if( NULL != packet_info ) {
		/* ��M�p�P�b�g���o�b�t�@�����菜�� */
		(void)sd_sim_data_ReadRecvData( packet_info->packet_bytes, NULL, N_TRUE );
	
		(void)memset(packet_info, 0, sizeof(T_SD_CTRL_PACKET_INFO));
	}

}
/****************************************************************************/
/* �֐�����		:	sd_sim_data_GetPacketInfo								*/
/* �@�\�̐���	:	��M�o�b�t�@����͂��ăp�P�b�g���𓾂�				*/
/* ����			:	( /O)packet_info	��͂œ����p�P�b�g�̏��			*/
/* �߂�l		:	1:  �p�P�b�g����										*/
/* �߂�l		:	0:  �p�P�b�g�ł͂Ȃ��i�ُ�p�P�b�g���܂ށj				*/
/* �߂�l		:	-1: �f�[�^�s���Ŕ���s�\								*/
/* �߂�l		:	-2:	�����s��											*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/03												*/
/* ���l			:	�p�P�b�g���Ȃ��ꍇ SD_CTRL_FMT_TYPE_INVALID ���o�͂���	*/
/****************************************************************************/
static SI_32 sd_sim_data_GetPacketInfo( T_SD_CTRL_PACKET_INFO* packet_info )
{

	SI_32	ret = 0L;
	UI_16	data_bytes = 0U;
	UI_16	buf_bytes = sizeof(packet_info->packet);
	UI_8*	p = NULL;
	UI_8*	header_part = NULL;
	UI_8*	data_part = NULL;
	UI_8*	footer_part = NULL;
	UI_16	packet_bytes = 0;
	UI_16	length = 0U;
	UI_16	crc = 0U;
	UI_16	crc_calc = 0U;
	UI_8	type = 0U;

	if( NULL == packet_info ) {
		ret = -2L;
	}
	else {
		/* �p�P�b�g���𖳌��l�ŏ����� */
		(void)memset( packet_info, 0, sizeof(T_SD_CTRL_PACKET_INFO) );
		packet_info->type = SD_CTRL_FMT_TYPE_INVALID;

		/* �����O�o�b�t�@�̃f�[�^���Q�� */
		data_bytes = sd_sim_data_GetRecvDataSize();
		if( data_bytes > buf_bytes ) {
			/* �p�P�b�g�o�b�t�@�̍ő吔���܂Ńf�[�^�Q�Ƃ��� */
			data_bytes = buf_bytes;
		}
		if( SD_CTRL_FMT_BYTES_FIX > data_bytes ) {
			/* �Œ蕔���̃T�C�Y�ɂ������Ȃ� */
			ret = -1L;
			goto END_ANALYZE;
		}
		(void)sd_sim_data_ReadRecvData( data_bytes, packet_info->packet, N_FALSE );

		/* �p�P�b�g��� */
		p = packet_info->packet;

		/*==================================================================*/
		/* �w�b�_�̉��														*/
		/*==================================================================*/
		header_part = p;
		/* START */
		if( SD_CTRL_FMT_HEADER_START != *p ) {
			goto END_ANALYZE;
		}
		p++;
		/* LENGTH */
		length = Common_Get16( p );
		p += 2;
		/* TYPE */
		type = *p;
		p++;

		/* �p�P�b�g�S�̂̒������v�Z */
		packet_bytes = SD_CTRL_FMT_BYTES_FIX + length - 1U;
		if( packet_bytes > data_bytes ) {
			/* �p�P�b�g�S�̂̃T�C�Y�ɖ����Ȃ� */
			ret = -1L;
			goto END_ANALYZE;
		}
		/*==================================================================*/
		/* �f�[�^�̉��														*/
		/*==================================================================*/
		switch( type ) {
		case SD_CTRL_FMT_TYPE_HEADER:			/*	�ԗ��M���ꗗ�iSD�J�[�h�p�j	*/
			/* �f�[�^���`�F�b�N */
			/* �ȗ� */
			break;
		case SD_CTRL_FMT_TYPE_ID_INFO_DATA:		/*	�ԗ����ID�iSD�J�[�h�p�j	*/
			/* �f�[�^���`�F�b�N */
			/* �ȗ� */
			break;
		case SD_CTRL_FMT_TYPE_SIGNAL_DATA:		/*	�ԗ����iSD�J�[�h�p�j		*/
			/* �f�[�^���`�F�b�N */
			/* �ȗ� */
			break;
		default:
			/* �s���ȃf�[�^��� */
			goto END_ANALYZE;
		}
		data_part = p;
		p += (length - 1U);

		/*==================================================================*/
		/* �t�b�^�̉��														*/
		/*==================================================================*/
		footer_part = p;
		/* CRC */
		crc = Common_Get16( p );
		p += 2;
		/* END */
		if( SD_CTRL_FMT_FOOTER_END != *p ) {
			goto END_ANALYZE;
		}
		p++;

		/* CRC�v�Z */
		crc_calc = Common_CalcCRC( header_part, 1U, length + 2 );
		if( crc_calc != crc ) {
			/* CRC �s��v */
			goto END_ANALYZE;
		}
		/*==================================================================*/
		/* ��͌��ʂ��i�[													*/
		/*==================================================================*/
		packet_info->packet_bytes = packet_bytes;
		packet_info->length = length;
		packet_info->type = type;
		packet_info->header_part = header_part;
		packet_info->data_part = data_part;
		packet_info->footer_part = footer_part;

		/* �p�P�b�g���݂����� */
		ret = 1L;
	}
END_ANALYZE:
	return ret;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_data_GetRecvDataSize								*/
/* �@�\�̐���	:	��M�o�b�t�@�ɂ���f�[�^�̃T�C�Y���擾����				*/
/* ����			:	void													*/
/* �߂�l		:	��M�f�[�^�o�C�g��										*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/03												*/
/* ���l			:															*/
/****************************************************************************/
static UI_16 sd_sim_data_GetRecvDataSize( void )
{

	UI_32	recv_data_size;
	UI_32	buff_size = sizeof(S_sd_rec_data_buff);

	recv_data_size = S_sd_sim_ctrl_info.sim_total_write_size - S_sd_sim_ctrl_info.sim_total_read_size;
	if( recv_data_size > buff_size ) {
		/* ���肦�Ȃ�:�t�F�[���Z�[�t */
		recv_data_size = buff_size;
	}
	return (UI_16)recv_data_size;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_data_ReadRecvData								*/
/* �@�\�̐���	:	��M�o�b�t�@�f�[�^�ǂ�									*/
/* ����			:	(I/ )size	�ǂݍ��݃T�C�Y								*/
/*					( /O)buff	�ǂݍ��݃o�b�t�@	NULL��					*/
/*					(I/ )fclear	�擾�����f�[�^���N���A���邩�H				*/
/* �߂�l		:	E_RC_OK			size���擾����							*/
/*					E_RC_NG_PARAM	�p�����^�s���isize���s�������j			*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/03												*/
/* ���l			:															*/
/****************************************************************************/
static E_RC sd_sim_data_ReadRecvData( UI_16 size, UI_8* buff, UI_8 fclear )
{

	E_RC	rc = E_RC_NG;
	UI_32	read_pos = 0UL;
	UI_32	rest_size = 0UL;
	UI_32	buff_size = sizeof(S_sd_rec_data_buff);

	if( 0 == size ) {
		rc = E_RC_NG_PARAM;
	}
	else if( size > sd_sim_data_GetRecvDataSize() ) {
		rc = E_RC_NG_PARAM;
	}
	else {
		/* �ǂݏo���J�n�ʒu���v�Z */
		read_pos = S_sd_sim_ctrl_info.sim_total_read_size % buff_size;
		
		if( NULL != buff ) {
			/* ���ۂɓǂݏo���ꍇ */
			rest_size = buff_size - read_pos;
			if( size <= rest_size ) {
				/* �܂Ƃߓǂ݉\ */
				(void)memcpy( buff, &S_sd_rec_data_buff[read_pos], size );
			}
			else {
				/* �܂�Ԃ��ǂݕK�v */
				(void)memcpy( buff, &S_sd_rec_data_buff[read_pos], (size_t)rest_size );
				(void)memcpy( buff + rest_size, S_sd_rec_data_buff, (size_t)(size - rest_size) );
			}
		}
		if( N_TRUE == fclear ) {
			/* �o�b�t�@�N���A����ꍇ */
			S_sd_sim_ctrl_info.sim_total_read_size += size;
		}
		rc = E_RC_OK;
	}
	return rc;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_data_GetIDList									*/
/* �@�\�̐���	:	ID���X�g���p�P�b�g���璊�o								*/
/* ����			:	(I/ )packet_info	�p�P�b�g���						*/
/*					( /O)idList		ID���X�g	NULL��						*/
/*					(I/ )max_idNum	�ő�ID��								*/
/* �߂�l		:	0 <=	���o����ID��									*/
/*					-1		�ȏ�i�����s���^�t�H�[�}�b�g�s���j				*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static SI_32 sd_sim_data_GetIDList( T_SD_CTRL_PACKET_INFO* packet_info, UI_16* idList, UI_8 max_idNum )
{

	SI_32	count = -1;
	UI_8	idNum = 0U;
	UI_8	i = 0U;
	UI_8*	p = NULL;

	if( NULL != packet_info ) {
		if( SD_CTRL_FMT_TYPE_ID_INFO_DATA == packet_info->type ) {
			/* ID�����擾 */
			idNum = *packet_info->data_part;
			p = packet_info->data_part + 1;
			if( idNum <= max_idNum ) {
				if( NULL != idList ) {
					/* ID���X�g���擾 */
					for( i = 0; i < idNum; i++ ) {
						idList[i] = Common_Get16( p );
						p += 2;
					}
				}
				count = (SI_32)idNum;
			}
		}
	}
	return count;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_data_GetValues									*/
/* �@�\�̐���	:	�M���z����p�P�b�g���璊�o								*/
/* ����			:	(I/ )packet_info		�p�P�b�g���					*/
/*					( /O)values			�M���z��	NULL��					*/
/*					(I/ )max_valueNum	�ő�M����							*/
/* �߂�l		:	0 <=	���o�����M����									*/
/*					-1		�ȏ�i�����s���^�t�H�[�}�b�g�s���j				*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static SI_32 sd_sim_data_GetValues( T_SD_CTRL_PACKET_INFO* packet_info, SI_32* values, UI_8 max_valueNum )
{

	SI_32	count = -1;
	UI_8	errorNum = 0U;
	UI_8	valueNum = 0U;
	UI_8	i = 0U;
	UI_8*	p = NULL;

	if( NULL != packet_info ) {
		if( SD_CTRL_FMT_TYPE_SIGNAL_DATA == packet_info->type ) {
			/* �G���[�R�[�h������Q�� */
			p = packet_info->data_part + 4;
			errorNum = *p;
			p++;
			if( SD_CTRL_MAX_ERRCODE_COUNT >= errorNum ) {
				/* �G���[�R�[�h��ǂݔ�΂� */
				p += (6 * errorNum);

				/* �M�������擾 */
				valueNum = *p;
				p++;
				if( valueNum <= max_valueNum ) {
					if( NULL != values ) {
						/* �M���z����擾 */
						for( i = 0; i < valueNum; i++ ) {
							values[i] = (SI_32)Common_Get32( p );
							p += 4;
						}
					}
					count = (SI_32)valueNum;
				}
			}
		}
	}
	return count;

}
/*==========================================================================*/
/* �M���f�[�^����															*/
/*==========================================================================*/
/****************************************************************************/
/* �֐�����		:	sd_sim_signal_ClearBuffer								*/
/* �@�\�̐���	:	�M���o�b�t�@�N���A										*/
/* ����			:	(I/ )clear_all	N_TRUE:�S�M���N���A�^N_FALSE:�ϐ��M���̂݃N���A	*/
/* �߂�l		:	void													*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/03												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_sim_signal_ClearBuffer( B_8 clear_all )
{

	UI_8	fixIdNum;

	if( N_TRUE == clear_all ) {
		/* �S�M���N���A */
		(void)memset(S_sd_signal_buff, 0xFF, sizeof(S_sd_signal_buff));
	}
	else {
		/* �ϐ��M���̂݃N���A */
		fixIdNum = S_sd_sim_id_info.fixIdNum;
		(void)memset(S_sd_signal_buff + fixIdNum, 0xFF, sizeof(S_sd_signal_buff)-(fixIdNum * sizeof(SI_32)));
	}

}
/****************************************************************************/
/* �֐�����		:	sd_sim_signal_AnalyzeHeader								*/
/* �@�\�̐���	:	�w�b�_���												*/
/* ����			:	(I/ )packet_info	�p�P�b�g���i�t�@�C���w�b�_�j		*/
/* �߂�l		:	0 	����												*/
/*					-1	�p�����^�s��										*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static SI_32 sd_sim_signal_AnalyzeHeader( const T_SD_CTRL_PACKET_INFO* packet_info )
{

	SI_32	ret = -1;
	UI_8	varIdNum;
	UI_8	fixIdNum;
	UI_8	fixIdNumIndex;
	UI_8	varIdNumIndex;
	UI_8	randomValIndex;
	UI_8	signalListVerIndex;

	if( NULL != packet_info ) {
		if( SD_CTRL_FMT_TYPE_HEADER == packet_info->type ) {
			S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_HEADER;
			S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_HEADER] = packet_info->packet_bytes;
			signalListVerIndex = 21;
			/*�M�����X�g�o�[�W�����ێ�*/
			S_sd_sim_id_info.signalListVersion = Common_Get24( &packet_info->data_part[signalListVerIndex] );
			/*�Œ�ID���C���f�b�N�X*/
			fixIdNumIndex = 37;
			fixIdNum = packet_info->data_part[fixIdNumIndex];

			/*�ϐ�ID���C���f�b�N�X*/
			varIdNumIndex = fixIdNumIndex + 1 + (fixIdNum * sizeof(UI_16));
			varIdNum = packet_info->data_part[varIdNumIndex];
			
			/*�����C���f�b�N�X*/
			randomValIndex = varIdNumIndex + 1 + (varIdNum * sizeof(UI_16)) + 40;
			
			/*�����ێ�*/
			S_sd_sim_id_info.randomValue = Common_Get32( &packet_info->data_part[randomValIndex] );
			/*�yMEMO�z
				��������ł͊eID��񃊃X�g���璊�o����ID���X�g��
				ID�ꗗ�Ƃ��ď�ʂɕԂ��Ă��邪�A�{���̓w�b�_����擾�������̂�
				�Ԃ��悤�ɂ����ق����悢��������Ȃ��B
				������݌v�ł͂ǂ���������l�ł��邽�ߊȒP�̂��߂������Ă���B
			*/
			ret = 0;
		}
	}
	return ret;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_signal_SetIDList									*/
/* �@�\�̐���	:	ID���X�g�ݒ�											*/
/* ����			:	(I/ )fix			N_TRUE:�Œ�ID���X�g�^N_FALSE:�ϐ�ID���X�g	*/
/*					(I/ )packet_info	�p�P�b�g���iID���X�g�j			*/
/* �߂�l		:	0 <=	�Z�b�g����ID�̐�								*/
/*					-1		�p�����^�s��									*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static SI_32 sd_sim_signal_SetIDList( B_8 fix, T_SD_CTRL_PACKET_INFO* packet_info )
{

	SI_32	count = -1;

	if( NULL != packet_info ) {
		if( N_TRUE == fix ) {
			count = sd_sim_data_GetIDList( packet_info, S_sd_sim_id_info.fixIdList, SD_CTRL_MAX_FIX_ID_COUNT );
			if( 0 <= count ) {
				S_sd_sim_id_info.fixIdNum = (UI_8)count;
				S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_FIX_ID_LIST;
				S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_FIX_ID_LIST] = packet_info->packet_bytes;
			}
		}
		else {
			count = sd_sim_data_GetIDList( packet_info, S_sd_sim_id_info.varIdList, SD_CTRL_MAX_VAR_ID_COUNT );
			if( 0 <= count ) {
				S_sd_sim_id_info.varIdNum = (UI_8)count;
				S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_VAR_ID_LIST;
				S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_VAR_ID_LIST] = packet_info->packet_bytes;
			}
		}
	}
	return count;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_signal_SetValues									*/
/* �@�\�̐���	:	�M���z��ݒ�											*/
/* ����			:	(I/ )fix			N_TRUE:�Œ�M���z��^N_FALSE:�ϐ��M���z��	*/
/*					(I/ )packet_info	�p�P�b�g���iID���X�g�j			*/
/* �߂�l		:	0 <=	�Z�b�g�����l�̐�								*/
/*					-1		�p�����^�s��									*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static SI_32 sd_sim_signal_SetValues( B_8 fix, T_SD_CTRL_PACKET_INFO* packet_info )
{

	SI_32	count = -1;

	if( NULL != packet_info ) {
		if( N_TRUE == fix ) {
			count = sd_sim_data_GetValues( packet_info, S_sd_signal_buff, SD_CTRL_MAX_FIX_ID_COUNT );
			if( 0 <= count ) {
				S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_FIX_VALUES;
				S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_FIX_VALUES] = packet_info->packet_bytes;
			}
		}
		else {
			count = sd_sim_data_GetValues( packet_info, S_sd_signal_buff + S_sd_sim_id_info.fixIdNum, SD_CTRL_MAX_VAR_ID_COUNT );
			if( 0 <= count ) {
				S_sd_sim_ctrl_info.flags |= SD_SIM_FLAG_RECV_VAR_VALUES;
				S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_VAR_VALUES] = packet_info->packet_bytes;
			}
		}
	}
	return count;

}
/****************************************************************************/
/* �֐�����		:	sd_sim_signal_GetFixPartSize							*/
/* �@�\�̐���	:	�V�~�����[�V�����t�@�C���Œ蕔�T�C�Y�擾				*/
/* ����			:	void													*/
/* �߂�l		:	0 <	�V�~�����[�V�����t�@�C���Œ蕔�T�C�Y				*/
/*					0		�擾�s��										*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static UI_32 sd_sim_signal_GetFixPartSize( void )
{

	UI_32	mask = SD_SIM_FLAG_RECV_HEADER | SD_SIM_FLAG_RECV_FIX_ID_LIST | SD_SIM_FLAG_RECV_FIX_VALUES | SD_SIM_FLAG_RECV_VAR_ID_LIST;
	UI_32	fix_part_size = 0UL;

	if( mask == (S_sd_sim_ctrl_info.flags & mask) ) {
		/* �w�b�_���{�Œ�ID���X�g�{�Œ�ԗ����͊m�� */
		fix_part_size = S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_HEADER] + 
			S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_FIX_ID_LIST] + 
			S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_FIX_VALUES];

		if( 0 == (S_sd_sim_ctrl_info.flags & SD_SIM_FLAG_CHANGE_VAR_ID_LIST) ) {
			/* �ϐ�ID���X�g�ύX�����o���Ă��Ȃ��Ȃ�ϐ�ID���X�g�܂ŌŒ� */
			fix_part_size += S_sd_sim_ctrl_info.sim_file_part_size[SIM_FILE_PART_VAR_ID_LIST];
		}
	}
	return fix_part_size;

}

#ifdef _VDV_APP
/****************************************************************************/
/* �֐�����		: sd_ctrl_InitInspectionInfo								*/
/* �@�\�̐���	: SD������񏉊���											*/
/* ����			: void														*/
/* �߂�l		: void														*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 17/01/20													*/
/* ���l			: 															*/
/****************************************************************************/
static void sd_ctrl_InitInspectionInfo(void)
{

	(void)memset(&S_sd_inspection_info, 0, sizeof(S_sd_inspection_info));
	S_sd_inspection_info.insp_req_flg = N_OFF;
	S_sd_inspection_info.fio_req_flg = N_OFF;
	S_sd_inspection_info.step = SD_INSPECTION_STEP_INVALID;

	return;

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_Inspect											*/
/* �@�\�̐���	:	SD����													*/
/* ����			:	void													*/
/* �߂�l		: 	N_ON	�������p������									*/
/*				: 	N_OFF	�������p�����Ȃ��i�����I���j					*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
static UI_8 sd_ctrl_Inspect(void)
{

	E_FIO_RESULT	fioRes;
	UI_32			reqNo;
	UI_8			contflg;		/* �����p���t���O N_ON:�����p��  N_OFF:�����I��		*/
	E_INSP_INSPECTION_RESULT	inspectRes;
	T_INSP_INSPECTION_PARAM		inspectParam;
	
	if (S_sd_inspection_info.insp_req_flg == N_OFF) {
		return N_OFF;
	}

	contflg = N_OFF;
	inspectRes = INSP_INSPECTION_RESULT_NG;
	S_sd_inspection_info.fio_req_flg = N_OFF;

	if (S_sd_inspection_info.step == SD_INSPECTION_STEP_MKDIR) {	/* �����X�e�b�v:�����p�f�B���N�g���쐬	*/

		/* �f�B���N�g���쐬���ʂ��`�F�b�N	*/
		if ((S_sd_inspection_info.fio_result == FIO_RESULT_OK) ||
			(S_sd_inspection_info.fio_result == FIO_RESULT_ERR_EXIST)) {
			/* �f�B�e�N�g���쐬����(�f�B���N�g�������݂��Ă����ꍇ��OK)	*/

			/* �����X�e�b�v�Ɂu�������݌����v��ݒ�	*/
			S_sd_inspection_info.step = SD_INSPECTION_STEP_WRITE;

			/* SD�ɃV���A��No����������	*/
			Common_Set16(S_sd_inspection_info.write_buf, S_sd_inspection_info.serialno);
			fioRes = FileIO_Write( FIO_WRITE_TYPE_FILE_CREATE, S_sd_inspection_info.filepath,
									S_sd_inspection_info.write_buf, 0, SD_CTRL_INSPECTION_DATA_SIZE,
									sd_ctrl_InspectionFioCallback, &reqNo );
			if (fioRes == FIO_RESULT_OK) {
				S_sd_inspection_info.fio_req_flg = N_ON;
				contflg = N_ON;
			}
		}
	}
	else if (S_sd_inspection_info.step == SD_INSPECTION_STEP_WRITE) {	/* �����X�e�b�v:�������݌���			*/

		/* �������݌��ʂ��`�F�b�N	*/
		if ((S_sd_inspection_info.fio_result == FIO_RESULT_OK) &&
			(S_sd_inspection_info.fio_bytes == SD_CTRL_INSPECTION_DATA_SIZE)) {
			/* �������ݐ���	*/
			/* �����X�e�b�v�Ɂu�ǂݍ��݌����v��ݒ�	*/
			S_sd_inspection_info.step = SD_INSPECTION_STEP_READ;

			/* SD����V���A��No��ǂݍ���	*/
			fioRes = FileIO_Read( S_sd_inspection_info.filepath,
								S_sd_inspection_info.read_buf, 0, SD_CTRL_INSPECTION_DATA_SIZE,
								sd_ctrl_InspectionFioCallback, &reqNo );
			if (fioRes == FIO_RESULT_OK) {
				S_sd_inspection_info.fio_req_flg = N_ON;
				contflg = N_ON;
			}
		}
	}
	else if (S_sd_inspection_info.step == SD_INSPECTION_STEP_READ) {	/* �����X�e�b�v:�ǂݍ��݌���			*/

		/* �ǂݍ��݌��ʂ��`�F�b�N	*/
		if ((S_sd_inspection_info.fio_result == FIO_RESULT_OK) &&
			(S_sd_inspection_info.fio_bytes == SD_CTRL_INSPECTION_DATA_SIZE)) {
			if (memcmp(S_sd_inspection_info.write_buf, S_sd_inspection_info.read_buf, SD_CTRL_INSPECTION_DATA_SIZE) == 0) {
				/* SD����OK	*/
				inspectRes = INSP_INSPECTION_RESULT_OK;
			}
		}
	}
	else {																/* �����X�e�b�v:����(���������{���)	*/

		/* �����X�e�b�v�Ɂu�����p�f�B���N�g���쐬�v��ݒ�	*/
		S_sd_inspection_info.step = SD_INSPECTION_STEP_MKDIR;

		/* SD�Ɍ����p�f�B���N�g�����쐬����	*/
		fioRes = FileIO_MakeDir((const UI_8*)SD_CTRL_INSPECTION_DIR, sd_ctrl_InspectionDirCallback, &reqNo);
		if (fioRes == FIO_RESULT_OK) {
			S_sd_inspection_info.fio_req_flg = N_ON;
			contflg = N_ON;
		}
	}

	if (contflg == N_OFF) {				/* SD�����I��		*/
		/* �������ʃp�����^�쐬	*/
		(void)memset(&inspectParam, 0, sizeof(inspectParam));
		inspectParam.data1 = (UI_8)(S_sd_inspection_info.serialno >> 8);
		inspectParam.data2 = (UI_8)(S_sd_inspection_info.serialno & 0x00FF);

		/* �������䃂�W���[���Ɍ��ʂ�ʒm����	*/
		Insp_notify_inspection_result(INSP_INSPECTION_TYPE_SD, &inspectParam, inspectRes);

		/* SD���������N���A	*/
		sd_ctrl_InitInspectionInfo();
	}

	return contflg;

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_InspectionFioCallback							*/
/* �@�\�̐���	:	SD�����t�@�C���A�N�Z�X�R�[���o�b�N						*/
/* ����			:	(I/ )reqinfo	�v�����								*/
/* 				:	(I/ )retinfo	����									*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_ctrl_InspectionFioCallback( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo )
{

	if ((reqinfo != NULL) && (retinfo != NULL)) {
		/* SD�������Ɍ����t�@�C���A�N�Z�X���ʂ��Z�b�g���� */
		S_sd_inspection_info.fio_result = retinfo->result;
		S_sd_inspection_info.fio_bytes = retinfo->bytes;

		/* �t�@�C��IO����̒ʒm�iSD�����p�ʒm�j�t���O��ON */
		S_sd_fio_notify_inspection = N_ON;
	}

	return;

}

/****************************************************************************/
/* �֐�����		:	sd_ctrl_InspectionFioCallback							*/
/* �@�\�̐���	:	SD�����f�B���N�g������R�[���o�b�N						*/
/* ����			:	(I/ )reqinfo	�v�����								*/
/* 				:	(I/ )retinfo	����									*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
static void sd_ctrl_InspectionDirCallback( const T_FIO_DIR_REQ_INFO *reqinfo, const T_FIO_DIR_RET_INFO *retinfo )
{

	if ((reqinfo != NULL) && (retinfo != NULL)) {
		/* SD�������Ɍ����t�@�C���A�N�Z�X���ʂ��Z�b�g���� */
		S_sd_inspection_info.fio_result = retinfo->result;
		S_sd_inspection_info.fio_bytes = 0;

		/* �t�@�C��IO����̒ʒm�iSD�����p�ʒm�j�t���O��ON */
		S_sd_fio_notify_inspection = N_ON;
	}

	return;

}
#endif /* _VDV_APP */


#endif /* _VDV_APP */
