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
/* file DTO459_VDV_Eeprom_ctrl.c						*/
/* EEPROM����											*/
/********************************************************/

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include <string.h>
#include "DTO459_VDV_Eeprom_ctrl.h"
#include "System.h"
#include "SSFTSTD_Macro.h"
#include "SSFTSTD_FR81_Eep_Drv.h"
#include "System_control_TASK.h"
#include "DTO459_VDV_File_io.h"
#include "DTO459_VDV_Common.h"
#include "DTO459_VDV_RringBuffer.h"
#include "timer_ctrl.h"
#include "Timer.h"
#include "dbglog.h"

/********************************************************/
/*	�}�N����`											*/
/********************************************************/
#define EEP_LOCAL	static

#define EEPROM_ADR_FIRMWARE_VER		0x03FE	/**< �t�@�[���E�F�A�o�[�W����	*/
#define EEPROM_ADR_EEPROM_VER		0x03FF	/**< EEPROM�E�F�A�o�[�W����		*/
#define EEPROM_ADR_VARIATION		0x0000	/**< �d�������					*/
#define EEPROM_ADR_BAT				0x0005	/**< �o�b�e���[�d���␳�l		*/
#define EEPROM_ADR_SERIAL_NO		0x000A	/**< �V���A��No					*/
#if defined _VDV_SUPRA /* 2018.09.11 S.Sakata */
#define EEPROM_ADR_VEHICLETYPE		0x000F	/**< �ԗ��^						*/
#endif /* defined _VDV_SUPRA 2018.09.11 S.Sakata */
#define EEPROM_ADR_VEHICLEINFO		0x0014	/**< �ԗ����					*/
#define EEPROM_ADR_INVALID			0xFFFFU	/**< ����						*/

#define EEPROM_DEFLT_VAL_FIRMWARE_VER	0x0000	/**< �����l:�t�@�[���E�F�A�o�[�W����	*/
#define EEPROM_DEFLT_VAL_EEPROM_VER		0x0000	/**< �����l:EEPROM�E�F�A�o�[�W����		*/
#define EEPROM_DEFLT_VAL_VARIATION		0x0000	/**< �����l:�d�������					*/
#define EEPROM_DEFLT_VAL_BAT			0x000A	/**< �����l:�o�b�e���[�d���␳�l		*/
#define EEPROM_DEFLT_VAL_SERIAL_NO		0x0000	/**< �����l:�V���A��No					*/
#if defined _VDV_SUPRA /* 2018.09.11 S.Sakata */
#define EEPROM_DEFLT_VAL_VEHICLETYPE	0x81FF	/**< �����l:�ԗ��^						*/
												/**< bit0-3:�C�����F�����l				*/
												/**< bit4-7:�n���h���ʒu�F�����l		*/
												/**< bit8-16:�ԗ��^���F�X�[�v��		*/
#endif /* defined _VDV_SUPRA 2018.09.11 S.Sakata */
#define EEPROM_DEFLT_VAL_VEHICLEJUDGE	0x0011	/**< �����l:�ԗ�������				*/
												/**< bit0-15:�ԗ�������F086B	��`�lTCAN_VEHICLE_INFO_86B�ɂ��킹�邱��	*/
#define EEPROM_ADR_INVALID			0xFFFFU	/**< ����						*/

#define EEPROM_READ_BUFSIZE			64		/**< �ǂݍ��݃o�b�t�@�T�C�Y		*/
#define EEPROM_RING_BUFSIZE			128		/**< �����O�o�b�t�@�T�C�Y		*/
#define EEPROM_RETRY_START			1000	/**< ���g���C�J�n�^�C�}			*/

/* �R�[���o�b�N�C�x���g�p��` */
#define EEPROM_FILE_SEARCH_EXIST	0x01	/**< �t�@�C������:�t�@�C������	*/
#define EEPROM_FILE_SEARCH_NOTEXIST	0x02	/**< �t�@�C������:�t�@�C���Ȃ�	*/
#define EEPROM_FILE_READ_OK			0x01	/**< �t�@�C���Ǎ�:�ǂݍ��ݐ���	*/
#define EEPROM_FILE_READ_NG			0x02	/**< �t�@�C���Ǎ�:�ǂݍ��ُ݈�	*/
#define EEPROM_FILE_READ_COMP		0x03	/**< �t�@�C���Ǎ�:�ǂݍ��݊���	*/

/* �p���e�B�`�F�b�N�p��` */
#define PARITY_NORMAL		0x00		/*  �p���e�B�`�F�b�N���ʐ��� */
#define PARITY_ERROR_1BIT	0x01		/*  �p���e�B�`�F�b�N���ʈُ�i1�r�b�g�j */
#define PARITY_ERROR_2BIT	0x02		/*  �p���e�B�`�F�b�N���ʈُ�i2�r�b�g�ȏ�j */

/* 
 ** �V�X�e��������
 */ 
typedef enum e_eeprom_ctrl_status
{
	EEPROM_STAT_WRITE_NOT = 0,				/**< EEPROM����������				*/
	EEPROM_STAT_WRITE_NOW,					/**< EEPROM�������ݒ�				*/
	EEPROM_STAT_WRITE_COMPLETE,				/**< EEPROM�������ݏI��			*/
	EEPROM_STAT_WRITE_SUSPEND,				/**< EEPROM�������ݒ��f			*/
	EEPROM_STAT_NUM							/**< EEPROM�����Ԑ�				*/
} E_EEPROM_CTRL_STATUS;

/* 
 ** �C�x���g
 */ 
typedef enum e_eeprom_ctrl_event
{
	EEPROM_EVENT_EEPROM_MODE_DETECT	= 0,	/**< EEPROM�������[�h���o					*/
	EEPROM_EVENT_FILE_SEARCH,				/**< �t�@�C����������						*/
	EEPROM_EVENT_FILE_READ,					/**< �t�@�C���Ǎ�����						*/
	EEPROM_EVENT_EEPROM_WRITE,				/**< EEPROM������							*/
	EEPROM_EVENT_ACC_OFF_DETECT,			/**< ACC OFF���o							*/
	EEPROM_EVENT_ACC_ON_DETECT,				/**< ACC ON���o�iACC OFF�ڍs��Ԃ���́j	*/
	EEPROM_EVENT_RETRY_TIMEOUT,				/**< ���g���C�^�C���A�E�g					*/
	EEPROM_EVENT_NUM						/**< �C�x���g��								*/
} E_EEPROM_CTRL_EVENT;

/* 
 ** �C�x���g�p�����^
 */ 
typedef enum e_eeprom_ctrl_event_param
{
	EEPROM_EVENT_PARAM_NO		= 0,		/**< �p�����^�Ȃ�												*/
	EEPROM_EVENT_PARAM_FILE_SRC_EXIST,		/**< �t�@�C������:�t�@�C������									*/
	EEPROM_EVENT_PARAM_FILE_SRC_NOTEXIST,	/**< �t�@�C������:�t�@�C���Ȃ�									*/

	EEPROM_EVENT_PARAM_FILE_READ_OK,		/**< �t�@�C���Ǎ�:�ǂݍ��ݐ���									*/
	EEPROM_EVENT_PARAM_FILE_READ_NG			/**< �t�@�C���Ǎ�:�ǂݍ��ُ݈�									*/

} E_EEPROM_CTRL_EVENT_PARAM;


/********************************************************/
/*	�\���̒�`											*/
/********************************************************/
/* EEPROM�f�[�^�ݒ��� */
typedef struct {
	E_EEPROM_DATA_TYPE		type;
	UI_16					data;
	PFN_EEPROM_SETDATA_CB	cbfnc;
} T_EEPROM_SET_DATA;

/* EEPROM�f�[�^�ݒ�Ǘ� */
typedef struct {
	UI_16					eep_adr;
	UI_8					data_num;
	UI_8					write_num;
	UI_16					data[5];
	T_EEPROM_SET_DATA		set_data;
} T_EEPROM_SET_DATA_MNG;

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
EEP_LOCAL	E_EEPROM_CTRL_STATUS		S_eeprom_ctrl_state;						/**< EEPROM������		*/
EEP_LOCAL	E_SYS_CTRL_STATUS			S_eeprom_prev_system_state;					/**< �O�̃V�X�e�����	*/
EEP_LOCAL	UI_8						S_eeprom_event_state[EEPROM_EVENT_NUM];		/**< �C�x���g�ʒm��� 	*/
EEP_LOCAL	E_EEPROM_CTRL_EVENT_PARAM	S_eeprom_event_param[EEPROM_EVENT_NUM];		/**< �C�x���g�p�����^ 	*/
EEP_LOCAL	UI_8						S_eeprom_event_fileread_stat;				/**< �C�x���g�������(N_OFF�ȊO:�����AN_OFF:������) */
EEP_LOCAL	UI_8						S_eeprom_event_filesearch_stat;				/**< �C�x���g�������(N_OFF�ȊO:�����AN_OFF:������) */

EEP_LOCAL	UI_8						S_eeprom_filepath[COMMON_VDV_FILE_PATH_LEN];/**< ���������t�@�C���p�X��(�f�B���N�g��+�t�@�C����)	*/
EEP_LOCAL	UI_8						S_eeprom_readBuff[EEPROM_READ_BUFSIZE];		/**< �t�@�C���ǂݍ��ݗp�o�b�t�@							*/
EEP_LOCAL	UI_16						S_eeprom_readFileSize;						/**< �t�@�C���T�C�Y										*/
EEP_LOCAL	UI_16						S_eeprom_readSizeTotal;						/**< �t�@�C���ǂݍ��ݍ��v�T�C�Y							*/
EEP_LOCAL	UI_16						S_eeprom_readSize;							/**< �t�@�C���ǂݍ��݃T�C�Y�i���߁j						*/
EEP_LOCAL	UI_16						S_eeprom_fileAccessWaitCB;					/**< �t�@�C���A�N�Z�X�R�[���o�b�N�҂��t���O				*/
EEP_LOCAL	UI_16						S_eeprom_writeSizeTotal;					/**< EEPROM�������ݍ��v�T�C�Y							*/

EEP_LOCAL	UI_16						S_eeprom_retry_timerCount;					/**< ���g���C�^�C�}�[									*/

EEP_LOCAL 	T_RINGBUFFER 				S_eeprom_ringBuffMng;						/**< �t�@�C���ǂݍ��݃f�[�^�ێ��p�����O�o�b�t�@�Ǘ��̈� */
EEP_LOCAL	UI_8						S_eeprom_ringBuff[EEPROM_RING_BUFSIZE];		/**< �t�@�C���ǂݍ��݃f�[�^�ێ��p�����O�o�b�t�@			*/

EEP_LOCAL	T_EEPROM_SET_DATA_MNG		S_eeprom_setdata_mng;						/**< EEPROM�ݒ�f�[�^�Ǘ�								*/
EEP_LOCAL 	T_RINGBUFFER 				S_eeprom_setdata_ringBuffMng;				/**< EEPROM�f�[�^�ݒ�p�����O�o�b�t�@�Ǘ��̈�			*/
EEP_LOCAL	UI_8						S_eeprom_setdata_ringBuff[(sizeof(T_EEPROM_SET_DATA) * 4)];
																				/**< EEPROM�f�[�^�ݒ�p�����O�o�b�t�@					*/

/********************************************************/
/*	�����֐��v���g�^�C�v�錾							*/
/********************************************************/
EEP_LOCAL void S_eeprom_setDataProc(void);
EEP_LOCAL UI_8 S_eeprom_checkEventIssue(E_SYS_CTRL_STATUS system_state);
EEP_LOCAL UI_8 S_eeprom_createEvent(E_EEPROM_CTRL_EVENT event, E_EEPROM_CTRL_EVENT_PARAM param);
EEP_LOCAL void S_eeprom_event_eeprom_mode_detect( UI_8 param );
EEP_LOCAL void S_eeprom_event_file_search( UI_8 param );
EEP_LOCAL void S_eeprom_event_file_read( UI_8 param );
EEP_LOCAL void S_eeprom_event_eeprom_write( UI_8 param );
EEP_LOCAL void S_eeprom_event_acc_off_detect( UI_8 param );
EEP_LOCAL void S_eeprom_event_acc_on_detect( UI_8 param );
EEP_LOCAL void S_eeprom_event_retry_timeout( UI_8 param );
EEP_LOCAL void S_eeprom_setStatus(E_EEPROM_CTRL_STATUS status);
EEP_LOCAL void S_eeprom_initData(void);
EEP_LOCAL void S_eeprom_writeData(UI_16 adr, const UI_8* pData, UI_16 dataSize);
EEP_LOCAL SI_8 S_eeprom_readData(UI_16 adr, UI_16* pData);
EEP_LOCAL void S_eeprom_ReadEepromFileCB( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo );
EEP_LOCAL void S_eeprom_SearchEepromFileCB( const T_COMMON_SRC_VDVF_RET *pSearchResult );
EEP_LOCAL UI_16 parity8(UI_16 *p, SI_8 n);
EEP_LOCAL UI_8 parity_check8(UI_16 *p, SI_8 n);


/*
 *	�C�x���g�����֐��e�[�u��
 */
EEP_LOCAL void ( * const C_eeprom_event_func[EEPROM_EVENT_NUM] ) ( UI_8 ) = {
	S_eeprom_event_eeprom_mode_detect,					/**< EEPROM_EVENT_EEPROM_MODE_DETECT	*/
	S_eeprom_event_file_search,							/**< EEPROM_EVENT_FILE_SEARCH			*/
	S_eeprom_event_file_read,							/**< EEPROM_EVENT_FILE_READ				*/
	S_eeprom_event_eeprom_write,						/**< EEPROM_EVENT_EEPROM_WRITE			*/
	S_eeprom_event_acc_off_detect,						/**< EEPROM_EVENT_ACC_OFF_DETECT		*/
	S_eeprom_event_acc_on_detect,						/**< EEPROM_EVENT_ACC_ON_DETECT			*/
	S_eeprom_event_retry_timeout,						/**< EEPROM_EVENT_RETRY_TIMEOUT			*/
};


/********************************************************/
/*	�O���֐�											*/
/********************************************************/

/********************************************************/
/* �֐�����		: Eeprom_control_Init					*/
/* �@�\�̐���	: EEPROM���䏉����						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Eeprom_control_Init(void)
{
	UI_8 i;
	S_eeprom_setStatus(EEPROM_STAT_WRITE_NOT);
	S_eeprom_prev_system_state	= SYS_OPERATION_NUM;
	/* �����O�o�b�t�@������ */
	RingBuffer_Init(&S_eeprom_ringBuffMng, S_eeprom_ringBuff, sizeof(S_eeprom_ringBuff));
	/* �����f�[�^������ */
	S_eeprom_initData();

	/* EEPROM�ݒ�f�[�^������ */
	RingBuffer_Init(&S_eeprom_setdata_ringBuffMng, S_eeprom_setdata_ringBuff, sizeof(S_eeprom_setdata_ringBuff));
	(void)memset(&S_eeprom_setdata_mng, 0, sizeof(S_eeprom_setdata_mng));
	for (i = 0; i < EEPROM_EVENT_NUM; i++)
	{
		S_eeprom_event_state[i] = N_OFF;
		S_eeprom_event_param[i] = EEPROM_EVENT_PARAM_NO;
	}
}

/********************************************************/
/* �֐�����		: Eeprom_control_Main					*/
/* �@�\�̐���	: EEPROM���䃁�C��						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Eeprom_control_Main(void)
{
	UI_8	i;
	E_SYS_CTRL_STATUS system_state;
	E_EEPROM_CTRL_EVENT_PARAM	param;
	UI_8 eventFlag;


	/**	<li>�V�X�e����Ԃ��擾	*/
	system_state = System_control_GetStatus();
	switch (system_state)
	{
		case (SYS_OPERATION_ERROR):					/**< �ُ�					*/
			/* �����̃��[�h�ł͉������Ȃ� */
			return;

		case (SYS_OPERATION_NORMAL_OFF):			/**< �ʏ�:OFF���[�h			*/
		case (SYS_OPERATION_NORMAL_AUTO):			/**< �ʏ�:�I�[�gREC			*/
		case (SYS_OPERATION_NORMAL_MANUAL):			/**< �ʏ�:�}�j���A��REC		*/
		case (SYS_OPERATION_SIMULATION):			/**< �V�~�����[�V����		*/
		case (SYS_OPERATION_FIRMWARE_WRITE):		/**< �t�@�[���E�F�A������	*/
		case (SYS_OPERATION_FIRMWARE_WRITE_COMP):	/**< �t�@�[���E�F�A��������	*/
		case (SYS_OPERATION_INSPECTION):			/**< �������[�h				*/
		case (SYS_OPERATION_INSPECTION_COMP):		/**< ��������				*/
		case (SYS_OPERATION_BT_COMM):				/**< BT���W���[���ʐM���[�h	*/
		case (SYS_OPERATION_GNSS_COMM):				/**< GNSS���W���[���ʐM���[�h*/
			/* EEPROM�f�[�^�ݒ菈�����s */
			S_eeprom_setDataProc();
			return;

		default:
			break;
	}
	
	/* �C�x���g�����`�F�b�N */
	eventFlag = S_eeprom_checkEventIssue(system_state);
	/**	<li>�C�x���g�̗L���𔻒�	*/
	if ( eventFlag == N_ON )
	{
		/**	<li>�C�x���g�����������������s	*/
		for ( i = 0; i < EEPROM_EVENT_NUM; i++ )
		{
			if ( S_eeprom_event_state[i] == N_ON )
			{
				param = S_eeprom_event_param[i];
				C_eeprom_event_func[i]((UI_8)param);
				S_eeprom_event_state[i] = N_OFF;
				S_eeprom_event_param[i] = EEPROM_EVENT_PARAM_NO;
			}
		}
	}
}


/********************************************************/
/* �֐�����		: Eeprom_control_GetData				*/
/* �@�\�̐���	: EEPROM����f�[�^�擾					*/
/* ����			: (I/ )�f�[�^���  E_EEPROM_DATA_TYPE	*/
/* 				: ( /O)�擾�f�[�^  						*/
/* �߂�l		: �������� E_EEPROM_CONTROL_RESULT		*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
E_EEPROM_CONTROL_RESULT Eeprom_control_GetData(E_EEPROM_DATA_TYPE type, UI_16* pData)
{
	UI_16 adr;
	/* �f�[�^�ǂݍ��� */
	switch(type)
	{
		case EEPROM_DATA_FIRMWARE_VER:	/**< �t�@�[���E�F�A�o�[�W����	*/
			adr = EEPROM_ADR_FIRMWARE_VER;
			break;
		case EEPROM_DATA_EEPROM_VER:	/**< EEPROM�E�F�A�o�[�W����		*/
			adr = EEPROM_ADR_EEPROM_VER;
			break;
		case EEPROM_DATA_VARIATION:		/**< �d�������					*/
			adr = EEPROM_ADR_VARIATION;
			break;
		case EEPROM_DATA_BAT:			/**< �o�b�e���[�d���␳�l		*/
			adr = EEPROM_ADR_BAT;
			break;
		case EEPROM_DATA_SERIAL_NO:		/**< �V���A��No					*/
			adr = EEPROM_ADR_SERIAL_NO;
			break;
#if defined _VDV_SUPRA /* 2018.09.11 S.Sakata */
		case EEPROM_DATA_VEHICLETYPE:
			adr = EEPROM_ADR_VEHICLETYPE;
			break;
#endif /* defined _VDV_SUPRA 2018.09.11 S.Sakata */
		case EEPROM_DATA_VEHICLEINFO:	/**< �Ԏ���					*/
			adr = EEPROM_ADR_VEHICLEINFO;
			break;
		default:
			adr = EEPROM_ADR_INVALID;	/**< �ُ�						*/
			break;
	}
	if(adr == EEPROM_ADR_INVALID)
	{
		/* �����s�� */
		return EEPROM_CONTROL_RESULT_ERR_INVALID_ARG;
	}
	/* EEPROM�ǂݍ��� */
	if (S_eeprom_readData(adr,pData) != N_OK) {
		return EEPROM_CONTROL_RESULT_ERR_FATAL;
	}

	return EEPROM_CONTROL_RESULT_OK;
}

/********************************************************/
/* �֐�����		: Eeprom_control_SetData				*/
/* �@�\�̐���	: EEPROM�փf�[�^�ݒ�					*/
/* ����			: (I/ )�f�[�^���  E_EEPROM_DATA_TYPE	*/
/* 				: (I/ )�ݒ�f�[�^  						*/
/*				: (I/ )�������݊����R�[���o�b�N�֐�		*/
/* �߂�l		: �������� E_EEPROM_CONTROL_RESULT		*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
E_EEPROM_CONTROL_RESULT Eeprom_control_SetData(E_EEPROM_DATA_TYPE type, UI_16 data, PFN_EEPROM_SETDATA_CB cbfnc)
{
	T_EEPROM_SET_DATA	setdata;

	if (type >= EEPROM_DATA_NUM) {
		/* �����s�� */
		return EEPROM_CONTROL_RESULT_ERR_INVALID_ARG;
	}

	/* �f�[�^�ݒ�p�����O�o�b�t�@�������݉\�T�C�Y�擾	*/
	if (RingBuffer_GetCanWriteSize(&S_eeprom_setdata_ringBuffMng) < sizeof(setdata)) {
		return EEPROM_CONTROL_RESULT_ERR_BUSY;
	}

	/* �f�[�^�ݒ�p�����O�o�b�t�@�Ɋi�[	*/
	(void)memset(&setdata, 0, sizeof(setdata));
	setdata.type = type;
	setdata.data = data;
	setdata.cbfnc = cbfnc;
	(void)RingBuffer_SetArrayData(&S_eeprom_setdata_ringBuffMng, (UI_8*)&setdata, sizeof(setdata));

	return EEPROM_CONTROL_RESULT_OK;
}

/********************************************************/
/* �֐�����		: S_eeprom_setDataProc					*/
/* �@�\�̐���	: EEPROM�փf�[�^�ݒ�					*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/02/09								*/
/* ���l			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_setDataProc(void)
{
	UI_16	adr;
	UI_16	data;
	UI_8	i;

	if (S_eeprom_setdata_mng.data_num == 0) {
		/* EEPROM�������ݒ��̃f�[�^�������ꍇ�̓����O�o�b�t�@����ݒ�f�[�^���擾���� */
		if(RingBuffer_GetCanReadSize(&S_eeprom_setdata_ringBuffMng) >= sizeof(T_EEPROM_SET_DATA)) {

			(void)memset(&S_eeprom_setdata_mng, 0, sizeof(S_eeprom_setdata_mng));

			/* �ݒ�f�[�^���擾 */
			(void)RingBuffer_GetArrayData(&S_eeprom_setdata_ringBuffMng, (UI_8*)&S_eeprom_setdata_mng.set_data, sizeof(T_EEPROM_SET_DATA), RINGBUF_READEND);

			/* �ݒ�f�[�^����EEPROM�ɏ������ރf�[�^�ɕϊ� */
			switch (S_eeprom_setdata_mng.set_data.type) {

				case EEPROM_DATA_FIRMWARE_VER:	/**< �t�@�[���E�F�A�o�[�W����	*/
					S_eeprom_setdata_mng.eep_adr = EEPROM_ADR_FIRMWARE_VER;
					S_eeprom_setdata_mng.data_num = 1;
					S_eeprom_setdata_mng.data[0] = S_eeprom_setdata_mng.set_data.data;
					break;
				case EEPROM_DATA_EEPROM_VER:	/**< EEPROM�E�F�A�o�[�W����		*/
					S_eeprom_setdata_mng.eep_adr = EEPROM_ADR_EEPROM_VER;
					S_eeprom_setdata_mng.data_num = 1;
					S_eeprom_setdata_mng.data[0] = S_eeprom_setdata_mng.set_data.data;
					break;
				case EEPROM_DATA_VARIATION:		/**< �d�������					*/
					S_eeprom_setdata_mng.eep_adr = EEPROM_ADR_VARIATION;
					S_eeprom_setdata_mng.data_num = 5;
					S_eeprom_setdata_mng.data[0] = S_eeprom_setdata_mng.set_data.data;
					break;
				case EEPROM_DATA_BAT:			/**< �o�b�e���[�d���␳�l		*/
					S_eeprom_setdata_mng.eep_adr = EEPROM_ADR_BAT;
					S_eeprom_setdata_mng.data_num = 5;
					/* bit8-15�Ȃ̂�8bit�V�t�g */
					S_eeprom_setdata_mng.data[0] = (S_eeprom_setdata_mng.set_data.data << 8);
					break;
				case EEPROM_DATA_SERIAL_NO:		/**< �V���A��No					*/
					S_eeprom_setdata_mng.eep_adr = EEPROM_ADR_SERIAL_NO;
					S_eeprom_setdata_mng.data_num = 5;
					S_eeprom_setdata_mng.data[0] = S_eeprom_setdata_mng.set_data.data;
					break;
#if defined _VDV_SUPRA /* 2018.09.11 S.Sakata */
				case EEPROM_DATA_VEHICLETYPE:	/**< �ԗ��^���					*/
					S_eeprom_setdata_mng.eep_adr = EEPROM_ADR_VEHICLETYPE;
					S_eeprom_setdata_mng.data_num = 5;
					S_eeprom_setdata_mng.data[0] = S_eeprom_setdata_mng.set_data.data;
					break;
#endif /* defined _VDV_SUPRA 2018.09.11 S.Sakata */
				case EEPROM_DATA_VEHICLEINFO:	/**< �ԗ����					*/
					S_eeprom_setdata_mng.eep_adr = EEPROM_ADR_VEHICLEINFO;
					S_eeprom_setdata_mng.data_num = 5;
					S_eeprom_setdata_mng.data[0] = S_eeprom_setdata_mng.set_data.data;
					break;
				default:						/**< �ُ�						*/
					break;
			}
			if (S_eeprom_setdata_mng.data_num == 0) {
				/* �������ُ݈�R�[���o�b�N */
				if (S_eeprom_setdata_mng.set_data.cbfnc != NULL) {
					S_eeprom_setdata_mng.set_data.cbfnc(S_eeprom_setdata_mng.set_data.type, S_eeprom_setdata_mng.set_data.data, N_NG);
				}
			}
			else if (S_eeprom_setdata_mng.data_num == 5) {
				/* �p���e�B�܂߂��������݃f�[�^���쐬���� */
				for (i = 1; i < 4; i++) {
					S_eeprom_setdata_mng.data[i] = EepDrv_ReadDataWord( S_eeprom_setdata_mng.eep_adr + i);
				}
				S_eeprom_setdata_mng.data[4] = parity8(S_eeprom_setdata_mng.data, 4);
			}
			else {
				/* nop */
			}
		}
	}

	if (S_eeprom_setdata_mng.data_num > 0) {
		/* EEPROM�������݃f�[�^������ꍇEEPROM�ɏ������� */
		/* EEPROM�h���C�o�L�� */
		EepDrv_Enable();
	
		/* EEPROM�f�[�^�������� */
		adr = S_eeprom_setdata_mng.eep_adr + S_eeprom_setdata_mng.write_num;
		data = S_eeprom_setdata_mng.data[S_eeprom_setdata_mng.write_num];
		EepDrv_WriteDataWord(adr, data);
		S_eeprom_setdata_mng.write_num++;

		/* EEPROM�h���C�o���� */
		EepDrv_Disable();

		if (S_eeprom_setdata_mng.data_num <= S_eeprom_setdata_mng.write_num) {
			/* �������݊��� */
			/* �R�[���o�b�N�ʒm */
			if (S_eeprom_setdata_mng.set_data.cbfnc != NULL) {
				S_eeprom_setdata_mng.set_data.cbfnc(S_eeprom_setdata_mng.set_data.type, S_eeprom_setdata_mng.set_data.data, N_OK);
			}

			/* �������݃f�[�^�N���A */
			(void)memset(&S_eeprom_setdata_mng, 0, sizeof(S_eeprom_setdata_mng));
		}
	}

	return;
}

/************************************************************/
/* �֐�����		: S_eeprom_checkEventIssue					*/
/* �@�\�̐���	: �C�x���g���s�`�F�b�N						*/
/* ����			: system_state:�V�X�e��������				*/
/* �߂�l		: N_ON:�C�x���g���� N_OFF:�C�x���g�Ȃ�		*/
/* �쐬��		: (OFT)Sakata								*/
/* �쐬��		: 16/10/12									*/
/* ���l			: 											*/
/************************************************************/
EEP_LOCAL UI_8 S_eeprom_checkEventIssue(E_SYS_CTRL_STATUS system_state)
{
	E_EEPROM_CTRL_EVENT_PARAM param;
	UI_8 ret = N_OFF;

	if(system_state != S_eeprom_prev_system_state)
	{
		/**	<li>�V�X�e����Ԃ�ω�����	*/
		switch (system_state)
		{
		case (SYS_OPERATION_EEPROM_WRITE):			/*EEPROM������			*/
			/* EEPROM�������[�h���o */
			(void)S_eeprom_createEvent(EEPROM_EVENT_EEPROM_MODE_DETECT, EEPROM_EVENT_PARAM_NO);
			ret = N_ON;
			break;
		case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF�ڍs			*/
			/* ACC OFF���o */
			(void)S_eeprom_createEvent(EEPROM_EVENT_ACC_OFF_DETECT, EEPROM_EVENT_PARAM_NO);
			ret = N_ON;
			break;
		case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON���o			*/
			if(S_eeprom_prev_system_state == SYS_OPERATION_PRE_ACC_OFF)
			{
				/* ACC OFF�ڍs -> ACC ON���o */
				(void)S_eeprom_createEvent(EEPROM_EVENT_ACC_ON_DETECT, EEPROM_EVENT_PARAM_NO);
				ret = N_ON;
			}
			break;
		default:
			break;
		}
		S_eeprom_prev_system_state = system_state;
	}
	/* �C�x���g�����t���O�Z�b�g */
	if(S_eeprom_event_fileread_stat != N_OFF)
	{
		/* �t�@�C���ǂ݌��ʂ��� */
		if(S_eeprom_event_fileread_stat == EEPROM_FILE_READ_OK)
		{
			param = EEPROM_EVENT_PARAM_FILE_READ_OK;
		}
		else if(S_eeprom_event_fileread_stat == EEPROM_FILE_READ_NG)
		{
			param = EEPROM_EVENT_PARAM_FILE_READ_NG;
		}
		else
		{
			/* �t�F�[�� */
			param = EEPROM_EVENT_PARAM_NO;
		}
		S_eeprom_event_fileread_stat = N_OFF;
		/* �ǂݍ��݌��� */
		(void)S_eeprom_createEvent(EEPROM_EVENT_FILE_READ, param);
		ret = N_ON;
	}
	if(S_eeprom_event_filesearch_stat != N_OFF)
	{
		/* �t�@�C���������ʂ��� */
		if(S_eeprom_event_filesearch_stat == EEPROM_FILE_SEARCH_EXIST)
		{
			param = EEPROM_EVENT_PARAM_FILE_SRC_EXIST;
		}
		else if(S_eeprom_event_filesearch_stat == EEPROM_FILE_SEARCH_NOTEXIST)
		{
			param = EEPROM_EVENT_PARAM_FILE_SRC_NOTEXIST;
		}
		else
		{
			/* �t�F�[�� */
			param = EEPROM_EVENT_PARAM_NO;
		}
		S_eeprom_event_filesearch_stat = N_OFF;
		(void)S_eeprom_createEvent(EEPROM_EVENT_FILE_SEARCH, param);
		ret = N_ON;
	}
	if(RingBuffer_GetCanReadSize(&S_eeprom_ringBuffMng) >= sizeof(UI_16))
	{
		/* EEPROM������ */
		(void)S_eeprom_createEvent(EEPROM_EVENT_EEPROM_WRITE, EEPROM_EVENT_PARAM_NO);
		ret = N_ON;
	}
	if(S_eeprom_retry_timerCount > 0)
	{
		/* ���g���C�^�C�}�[�N���� */
		/**	���g���C�^�C�}�[�^�C���A�E�g�������`�F�b�N	*/
		if (Timer_ctrl_TimeCompare10ms(S_eeprom_retry_timerCount, EEPROM_RETRY_START/TIME_MAIN) == N_OK)
		{
			S_eeprom_retry_timerCount = 0;
			(void)S_eeprom_createEvent(EEPROM_EVENT_RETRY_TIMEOUT, EEPROM_EVENT_PARAM_NO);
			ret = N_ON;
		}
	}
	
	return ret;
}

/************************************************************/
/* �֐�����		: S_eeprom_createEvent						*/
/* �@�\�̐���	: �C�x���g�ʒm								*/
/* ����			: (I/ )�C�x���g								*/
/* ����			: (I/ )�C�x���g�p�����^						*/
/* �߂�l		: �����ُ�		EEPROM_CONTROL_RESULT_ERR_INVALID_ARG*/
/* 				: ����			EEPROM_CONTROL_RESULT_OK	*/
/* �쐬��		: (OFT)Sakata								*/
/* �쐬��		: 16/10/12									*/
/* ���l			: 											*/
/************************************************************/
EEP_LOCAL UI_8 S_eeprom_createEvent(E_EEPROM_CTRL_EVENT event, E_EEPROM_CTRL_EVENT_PARAM param)
{
	
	/**	<ol>	*/
	/**	<li>�����`�F�b�N	*/
	if ( event >= EEPROM_EVENT_NUM )
	{
		return EEPROM_CONTROL_RESULT_ERR_INVALID_ARG;
	}
	
	/**	<li>�C�x���g������ʒm	*/
	if ( S_eeprom_event_state[event] == N_OFF)
	{
		S_eeprom_event_state[event] = N_ON;
		S_eeprom_event_param[event] = param;
	}
	/**	</ol>	*/
	return EEPROM_CONTROL_RESULT_OK;
}


/********************************************************/
/* �֐�����		: S_eeprom_event_eeprom_mode_detect		*/
/* �@�\�̐���	: �C�x���g�����֐�:EEPROM���샂�[�h���o	*/
/* ����			: (I/)param:�C�x���g�p�����^			*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/10/12								*/
/* ���l			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_event_eeprom_mode_detect( UI_8 param )
{
	E_COMMON_RESULT ret;
	switch(S_eeprom_ctrl_state)
	{
		case EEPROM_STAT_WRITE_NOT:				/**< EEPROM����������			*/
			/*�t�@�C�������J�n*/
			ret = Common_SerchVdvFile	(COMMON_VDV_FILE_TYPE_EEP, S_eeprom_SearchEepromFileCB);
			if(COMMON_RESULT_OK == ret)
			{
				/* �t�@�C���������� */
				/* �R�[���o�b�N�҂� */
				S_eeprom_fileAccessWaitCB = N_ON;
			}
			else
			{
				/* �t�@�C�������ُ� */
				/* ��ԕύX */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
				/* �V�X�e�������EEPROM���������ʒm */
				(void)System_control_NotifyEvent(SYS_EVENT_END_EEPROM_WRITE, SYS_EVENT_PARAM_NO);
			}
		break;
		case EEPROM_STAT_WRITE_NOW:				/**< EEPROM�������ݒ�			*/
		break;
		case EEPROM_STAT_WRITE_COMPLETE:		/**< EEPROM�������ݏI��		*/
		break;
		case EEPROM_STAT_WRITE_SUSPEND:			/**< EEPROM�������ݒ��f		*/
		break;
		default:
		break;
	}
}


/********************************************************/
/* �֐�����		: S_eeprom_event_file_search			*/
/* �@�\�̐���	: �C�x���g�����֐�:�t�@�C����������		*/
/* ����			: (I/)param:�C�x���g�p�����^			*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/10/12								*/
/* ���l			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_event_file_search( UI_8 param )
{
	UI_32 reqno;
	E_FIO_RESULT	ret;
	/* �R�[���o�b�N�҂� */
	S_eeprom_fileAccessWaitCB = N_OFF;
	switch(S_eeprom_ctrl_state)
	{
		case EEPROM_STAT_WRITE_NOT:				/**< EEPROM����������			*/
			if(param == EEPROM_EVENT_PARAM_FILE_SRC_EXIST)
			{
				/**< �t�@�C������:�t�@�C������									*/
				/* �t�@�C���ǂݍ��݊J�n */
				ret = FileIO_Read( S_eeprom_filepath, S_eeprom_readBuff, (UI_32)S_eeprom_readSizeTotal, sizeof(S_eeprom_readBuff), S_eeprom_ReadEepromFileCB, &reqno );
				if (ret == FIO_RESULT_OK) 
				{
					/* �t�@�C���ǂݍ��݊J�n:���� */
					/* �R�[���o�b�N�҂� */
					S_eeprom_fileAccessWaitCB = N_ON;
					/* ��ԕύX */
					S_eeprom_setStatus(EEPROM_STAT_WRITE_NOW);
				}
				else
				{
					/* �t�@�C���ǂݍ��݊J�n:�ُ� */
					/* ���g���C�^�C�}�[�J�n���Ԏ擾 */
					S_eeprom_retry_timerCount = Timer_ctrl_GetTime10ms();
					/* ��ԕύX */
					S_eeprom_setStatus(EEPROM_STAT_WRITE_SUSPEND);
				}
			}
			else
			{
				/**< �t�@�C������:�t�@�C���Ȃ�									*/
				/* ��ԕύX */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
				/* �t�@�C�����Ȃ����EEPROM�����I���Ƃ��� */
				/* �V�X�e�������EEPROM���������ʒm */
				(void)System_control_NotifyEvent(SYS_EVENT_END_EEPROM_WRITE, SYS_EVENT_PARAM_NO);
			}
		break;
		case EEPROM_STAT_WRITE_SUSPEND:			/**< EEPROM�������ݒ��f		*/
			/* �V�X�e�������EEPROM�I���ʒm */
			(void)System_control_NotifyEvent(SYS_EVENT_END_PROC_COMP_EEPROM, SYS_EVENT_PARAM_NO);
			/* ��ԕύX */
			S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
		break;
		default:
		break;
	}
}


/********************************************************/
/* �֐�����		: S_eeprom_event_file_read				*/
/* �@�\�̐���	: �C�x���g�����֐�:�t�@�C���Ǎ�����		*/
/* ����			: (I/)param:�C�x���g�p�����^			*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/10/12								*/
/* ���l			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_event_file_read( UI_8 param )
{
	UI_32 reqno;
	E_FIO_RESULT	ret;
	UI_16 can_write_len;
	/* �R�[���o�b�N�҂� */
	S_eeprom_fileAccessWaitCB = N_OFF;
	switch(S_eeprom_ctrl_state)
	{
		case EEPROM_STAT_WRITE_NOW:				/**< EEPROM�������ݒ�			*/
			if(param == EEPROM_EVENT_PARAM_FILE_READ_OK)
			{
				/**< �t�@�C���Ǎ�:�ǂݍ��ݐ���									*/
				if(S_eeprom_readSize > 0)
				{
					/* �����O�o�b�t�@�փf�[�^�i�[ */
					(void)RingBuffer_SetArrayData(&S_eeprom_ringBuffMng, S_eeprom_readBuff, S_eeprom_readSize);
				}
				/* ���ǂݍ��݃T�C�Y���Z */
				S_eeprom_readSizeTotal += S_eeprom_readSize;
				if(S_eeprom_readSizeTotal < S_eeprom_readFileSize)
				{
					/* �܂��Ō�܂œǂݍ���ł��Ȃ� */
					/* �������݉\�T�C�Y�擾 */
					can_write_len = RingBuffer_GetCanWriteSize(&S_eeprom_ringBuffMng);
					if(can_write_len >= sizeof(S_eeprom_readBuff))
					{
						/* �܂��󂫂�����΃f�[�^�ǂ� */
						ret = FileIO_Read( S_eeprom_filepath, S_eeprom_readBuff, (UI_32)S_eeprom_readSizeTotal, sizeof(S_eeprom_readBuff), S_eeprom_ReadEepromFileCB, &reqno );
						if (ret == FIO_RESULT_OK) 
						{
							/* �t�@�C���ǂݍ��݊J�n:���� */
							/* �R�[���o�b�N�҂� */
							S_eeprom_fileAccessWaitCB = N_ON;
						}
						else
						{
							/* �t�@�C���ǂݍ��݊J�n:�ُ� */
							/* ���g���C�^�C�}�[�J�n���Ԏ擾 */
							S_eeprom_retry_timerCount = Timer_ctrl_GetTime10ms();
							/* ��ԕύX */
							S_eeprom_setStatus(EEPROM_STAT_WRITE_SUSPEND);
						}
					}
				}
			}
			else
			{
				/**< �t�@�C���Ǎ�:�ǂݍ��ُ݈�									*/
				/* ���g���C�^�C�}�[�J�n���Ԏ擾 */
				S_eeprom_retry_timerCount = Timer_ctrl_GetTime10ms();
				/* ��ԕύX */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_SUSPEND);
			}
		break;
		case EEPROM_STAT_WRITE_SUSPEND:			/**< EEPROM�������ݒ��f		*/
			/* �V�X�e�������EEPROM�I���ʒm */
			(void)System_control_NotifyEvent(SYS_EVENT_END_PROC_COMP_EEPROM, SYS_EVENT_PARAM_NO);
			/* ��ԕύX */
			S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
		break;
		default:
		break;
	}
}


/********************************************************/
/* �֐�����		: S_eeprom_event_eeprom_write			*/
/* �@�\�̐���	: �C�x���g�����֐�:EEPROM�֏�������		*/
/* ����			: (I/)param:�C�x���g�p�����^			*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/10/12								*/
/* ���l			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_event_eeprom_write( UI_8 param )
{
	UI_8  buf[2];
	UI_16 can_write_len;
	UI_32 reqno;
	E_FIO_RESULT	ret;

	if(S_eeprom_ctrl_state != EEPROM_STAT_WRITE_NOW)
	{
		/**< EEPROM�������ݒ��ȊO�Ȃ甲����	*/
		return;
	}

	if(RingBuffer_GetCanReadSize(&S_eeprom_ringBuffMng) >= sizeof(UI_16))
	{
		/* �����O�o�b�t�@����f�[�^�擾 */
		(void)RingBuffer_GetArrayData(&S_eeprom_ringBuffMng, buf, sizeof(buf), RINGBUF_READEND);

		/* EEPROM�փf�[�^�������� */
		S_eeprom_writeData(S_eeprom_writeSizeTotal / sizeof(UI_16), buf, sizeof(buf));
		
		/* EEPROM�������ݍ��v���Z */
		S_eeprom_writeSizeTotal+= sizeof(UI_16);

	}
	if(S_eeprom_writeSizeTotal >= S_eeprom_readFileSize)
	{
		/* ���ׂď������݊��� */
		/* ��ԕύX */
		S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
		/* �V�X�e�������EEPROM���������ʒm */
		(void)System_control_NotifyEvent(SYS_EVENT_END_EEPROM_WRITE, SYS_EVENT_PARAM_NO);
	}
	else if(S_eeprom_readSizeTotal < S_eeprom_readFileSize)
	{
		/* �܂��Ō�܂œǂݍ���ł��Ȃ� */
		if(S_eeprom_fileAccessWaitCB == N_OFF){
			/* �t�@�C�����A�N�Z�X */
			/* �������݉\�T�C�Y�擾 */
			can_write_len = RingBuffer_GetCanWriteSize(&S_eeprom_ringBuffMng);
			if(can_write_len >= sizeof(S_eeprom_readBuff))
			{
				/* �܂��󂫂�����΃f�[�^�ǂ� */
				ret = FileIO_Read( S_eeprom_filepath, S_eeprom_readBuff, (UI_32)S_eeprom_readSizeTotal, sizeof(S_eeprom_readBuff), S_eeprom_ReadEepromFileCB, &reqno );
				if (ret == FIO_RESULT_OK) 
				{
					/* �t�@�C���ǂݍ��݊J�n:���� */
					/* �R�[���o�b�N�҂� */
					S_eeprom_fileAccessWaitCB = N_ON;
				}
				else
				{
					/* �t�@�C���ǂݍ��݊J�n:�ُ� */
					/* ���g���C�^�C�}�[�J�n���Ԏ擾 */
					S_eeprom_retry_timerCount = Timer_ctrl_GetTime10ms();
					/* ��ԕύX */
					S_eeprom_setStatus(EEPROM_STAT_WRITE_SUSPEND);
				}
			}
		}
	}
	else
	{
		/* nop */
	}
}

/********************************************************/
/* �֐�����		: S_eeprom_event_acc_off_detect			*/
/* �@�\�̐���	: �C�x���g�����֐�:ACC OFF���o			*/
/* ����			: (I/)param:�C�x���g�p�����^			*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/10/12								*/
/* ���l			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_event_acc_off_detect( UI_8 param )
{
	switch(S_eeprom_ctrl_state)
	{
		case EEPROM_STAT_WRITE_NOT:				/**< EEPROM����������			*/
		case EEPROM_STAT_WRITE_NOW:				/**< EEPROM�������ݒ�			*/
			if(S_eeprom_fileAccessWaitCB == N_ON)
			{
				/* �R�[���o�b�N�҂� */
				/* ��ԕύX */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_SUSPEND);
			}
			else
			{
				/* �R�[���o�b�N�҂��łȂ� */
				/* ��ԕύX */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
				/* �V�X�e�������EEPROM�I���ʒm */
				(void)System_control_NotifyEvent(SYS_EVENT_END_PROC_COMP_EEPROM, SYS_EVENT_PARAM_NO);
			}	
		break;
		case EEPROM_STAT_WRITE_COMPLETE:		/**< EEPROM�������ݏI��		*/
			/* �V�X�e�������EEPROM�I���ʒm */
			(void)System_control_NotifyEvent(SYS_EVENT_END_PROC_COMP_EEPROM, SYS_EVENT_PARAM_NO);
		break;
		case EEPROM_STAT_WRITE_SUSPEND:			/**< EEPROM�������ݒ��f		*/
			if(S_eeprom_fileAccessWaitCB == N_ON)
			{
				/* �R�[���o�b�N�҂� */
				/* �����Ȃ� */
			}
			else
			{
				/* �R�[���o�b�N�҂��łȂ� */
				/* ���g���C�^�C�}�[�J�n���Ԏ擾 */
				S_eeprom_retry_timerCount = 0;
				/* ��ԕύX */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
				/* �V�X�e�������EEPROM�I���ʒm */
				(void)System_control_NotifyEvent(SYS_EVENT_END_PROC_COMP_EEPROM, SYS_EVENT_PARAM_NO);
			}
		break;
		default:
		break;
	}
}




/********************************************************/
/* �֐�����		: S_eeprom_event_acc_on_detect			*/
/* �@�\�̐���	: �C�x���g�����֐�:ACC ON���o			*/
/* ����			: (I/)param:�C�x���g�p�����^			*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/10/12								*/
/* ���l			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_event_acc_on_detect( UI_8 param )
{
	switch(S_eeprom_ctrl_state)
	{
		case EEPROM_STAT_WRITE_NOT:				/**< EEPROM����������			*/
		case EEPROM_STAT_WRITE_NOW:				/**< EEPROM�������ݒ�			*/
		case EEPROM_STAT_WRITE_COMPLETE:		/**< EEPROM�������ݏI��		*/
		case EEPROM_STAT_WRITE_SUSPEND:			/**< EEPROM�������ݒ��f		*/
			/* ���������� */
			S_eeprom_initData();
			/* ��ԕύX */
			S_eeprom_setStatus(EEPROM_STAT_WRITE_NOT);
		break;
		default:
		break;
	}
}


/********************************************************/
/* �֐�����		: S_eeprom_event_retry_timeout			*/
/* �@�\�̐���	: �C�x���g�����֐�:���g���C�^�C���A�E�g	*/
/* ����			: (I/)param:�C�x���g�p�����^			*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/10/12								*/
/* ���l			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_event_retry_timeout( UI_8 param )
{
	E_COMMON_RESULT ret;
	switch(S_eeprom_ctrl_state)
	{
		case EEPROM_STAT_WRITE_NOT:				/**< EEPROM����������			*/
		break;
		case EEPROM_STAT_WRITE_NOW:				/**< EEPROM�������ݒ�			*/
		break;
		case EEPROM_STAT_WRITE_COMPLETE:		/**< EEPROM�������ݏI��		*/
		break;
		case EEPROM_STAT_WRITE_SUSPEND:			/**< EEPROM�������ݒ��f		*/
			/*�t�@�C�������J�n*/
			ret = Common_SerchVdvFile	(COMMON_VDV_FILE_TYPE_EEP, S_eeprom_SearchEepromFileCB);
			if(COMMON_RESULT_OK == ret)
			{
				/* �t�@�C���������� */
				/* �R�[���o�b�N�҂� */
				S_eeprom_fileAccessWaitCB = N_ON;
				/* ���������� */
				S_eeprom_initData();
				/* ��ԕύX */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_NOT);
			}
			else
			{
				/* �t�@�C�������ُ� */
				/* ��ԕύX */
				S_eeprom_setStatus(EEPROM_STAT_WRITE_COMPLETE);
				/* �V�X�e�������EEPROM���������ʒm */
				(void)System_control_NotifyEvent(SYS_EVENT_END_EEPROM_WRITE, SYS_EVENT_PARAM_NO);
			}
		break;
		default:
		break;
	}
}


/********************************************************/
/* �֐�����		: S_eeprom_ReadEepromFileCB				*/
/* �@�\�̐���	: �t�@�C���Ǎ��R�[���o�b�N�֐�			*/
/* ����			: (I/)reqinfo:�v�����					*/
/* 				: (I/)retinfo:�������ʏ��				*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/10/12								*/
/* ���l			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_ReadEepromFileCB( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo )
{
	if( (NULL != reqinfo) && (NULL != retinfo) ) {
		if( FIO_RESULT_OK == retinfo->result ) 
		{
			/* �Ǎ�����	*/
			S_eeprom_readSize = (UI_16)retinfo->bytes;
			S_eeprom_readFileSize = (UI_16)retinfo->fsize;
			S_eeprom_event_fileread_stat = EEPROM_FILE_READ_OK;
		}
		else 
		{
			/* �Ǎ��ُ�	*/
			S_eeprom_event_fileread_stat = EEPROM_FILE_READ_NG;
		}
	}
	else
	{
		/* �Ǎ����s */
		S_eeprom_event_fileread_stat = EEPROM_FILE_READ_NG;
	}

}



/********************************************************/
/* �֐�����		: S_eeprom_SearchEepromFileCB			*/
/* �@�\�̐���	: �t�@�C�������R�[���o�b�N�֐�			*/
/* ����			: (I/)pSearchResult:V�t�@�C����������	*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/10/12								*/
/* ���l			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_SearchEepromFileCB( const T_COMMON_SRC_VDVF_RET *pSearchResult )
{
	if(pSearchResult == NULL)
	{
		/* �����ُ� */
		S_eeprom_event_filesearch_stat = EEPROM_FILE_SEARCH_NOTEXIST;
		return;
	}
	if(pSearchResult->exist == N_TRUE)
	{
		/* �t�@�C������ */
		/* �t�@�C���p�X�R�s�[ */
		(void)memcpy(S_eeprom_filepath, pSearchResult->filepath, sizeof(pSearchResult->filepath));
		S_eeprom_event_filesearch_stat = EEPROM_FILE_SEARCH_EXIST;
	}
	else
	{
		/* �t�@�C���Ȃ� */
		S_eeprom_event_filesearch_stat = EEPROM_FILE_SEARCH_NOTEXIST;
	}
	
}


/********************************************************/
/* �֐�����		: S_eeprom_setStatus					*/
/* �@�\�̐���	: ��Ԑݒ�								*/
/* ����			: status:���							*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/10/12								*/
/* ���l			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_setStatus(E_EEPROM_CTRL_STATUS status)
{
	S_eeprom_ctrl_state = status;
	if(status == EEPROM_STAT_WRITE_SUSPEND)
	{
		/* �����O�o�b�t�@�N���A */
		RingBuffer_Clear(&S_eeprom_ringBuffMng);
	}
}


/********************************************************/
/* �֐�����		: S_eeprom_initData						*/
/* �@�\�̐���	: �����f�[�^������						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/10/12								*/
/* ���l			: 										*/
/********************************************************/
EEP_LOCAL void S_eeprom_initData(void)
{
	S_eeprom_writeSizeTotal = 0;
	S_eeprom_readSizeTotal = 0;
	S_eeprom_readFileSize = 0;
	S_eeprom_readSize = 0;
	/* �R�[���o�b�N�҂� */
	S_eeprom_fileAccessWaitCB = N_OFF;

	S_eeprom_event_fileread_stat = N_OFF;
	S_eeprom_event_filesearch_stat = N_OFF;
	
	S_eeprom_retry_timerCount = 0;
	
	/* �����O�o�b�t�@�N���A */
	RingBuffer_Clear(&S_eeprom_ringBuffMng);
	
}

/********************************************************/
/* �֐�����		: S_eeprom_writeData					*/
/* �@�\�̐���	: EEPROM�փf�[�^��������				*/
/* ����			: adr:�������݊J�n�A�h���X				*/
/* 				: 		�iEEPROM�������}�b�v�̃A�h���X	*/
/* 				: pData:�������݃f�[�^�A�h���X			*/
/* 				: dataSize:�������݃T�C�Y				*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/10/12								*/
/* ���l			: �������݃T�C�Y��2byte�P�ʂ̃f�[�^��ݒ肷�邱��*/
/********************************************************/
EEP_LOCAL void S_eeprom_writeData(UI_16 adr, const UI_8* pData, UI_16 dataSize)
{
	UI_8 i;
	UI_16 writeData;
	UI_8 writeCount = (UI_8)(dataSize / sizeof(UI_16));
	
	/* EEPROM�h���C�o�L�� */
	EepDrv_Enable();
	
	for(i = 0 ; i < writeCount ; i++)
	{
		/* �o�C�g�z�񂩂�16bit�̃f�[�^�擾 */
		writeData = Common_Get16(pData);
		/* EEPROM�f�[�^�������� */
		EepDrv_WriteDataWord(adr,writeData);
		pData += sizeof(UI_16);
		adr++;
	}
	/* EEPROM�h���C�o���� */
	EepDrv_Disable();
	
	
	
}


/********************************************************/
/* �֐�����		: S_eeprom_readData						*/
/* �@�\�̐���	: EEPROM����f�[�^�ǂݍ���				*/
/* ����			: adr:�������݊J�n�A�h���X				*/
/* 				: 		�iEEPROM�������}�b�v�̃A�h���X	*/
/* 				: pData:�ǂݍ��݃f�[�^�A�h���X			*/
/* �߂�l		: N_OK	:����							*/
/*				: N_NG	:�ُ�							*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/10/12								*/
/* ���l			: 										*/
/********************************************************/
EEP_LOCAL SI_8 S_eeprom_readData(UI_16 adr, UI_16* pData)
{
	UI_8 i;
	UI_16 data[5];
	UI_16 wordData;
	UI_8  parityResult;
	SI_8  ret = N_OK;

	/* �f�[�^�擾 */
	wordData = EepDrv_ReadDataWord(adr);
	if((adr == EEPROM_ADR_VARIATION) ||
#if defined _VDV_SUPRA /* 2018.09.11 S.Sakata */
	   (adr == EEPROM_ADR_VEHICLETYPE) ||
#endif /* defined _VDV_SUPRA 2018.09.11 S.Sakata */
	   (adr == EEPROM_ADR_VEHICLEINFO) ||
		(adr == EEPROM_ADR_SERIAL_NO))
	{
		/* �d����� */
		/* �p���e�B�v�Z�p�Ƀf�[�^�擾 */
		for (i = 0 ; i < (sizeof(data) / sizeof(UI_16)) ; i++)
		{
			data[i] = EepDrv_ReadDataWord( adr + i);
		}
		/* �p���e�B�`�F�b�N */
		parityResult = parity_check8(data, (sizeof(data) / sizeof(UI_16)));
		if(parityResult == PARITY_NORMAL)
		{
			*pData = wordData;
		}
		else
		{
			if (adr == EEPROM_ADR_VARIATION) {
				*pData = EEPROM_DEFLT_VAL_VARIATION;
			}
#if defined _VDV_SUPRA /* 2018.09.11 S.Sakata */
			else if (adr == EEPROM_ADR_VEHICLETYPE) {
				*pData = EEPROM_DEFLT_VAL_VEHICLETYPE;
			}
#endif /* defined _VDV_SUPRA 2018.09.11 S.Sakata */
			else if (adr == EEPROM_ADR_VEHICLEINFO) {
				*pData = EEPROM_DEFLT_VAL_VEHICLEJUDGE;
			}
			else {
				*pData = EEPROM_DEFLT_VAL_SERIAL_NO;
			}
			ret = N_NG;
		}
	}
	else if(adr == EEPROM_ADR_BAT)
	{
		/* �o�b�e���[�d�� */
		/* �p���e�B�v�Z�p�Ƀf�[�^�擾 */
		for (i = 0 ; i < (sizeof(data) / sizeof(UI_16)) ; i++)
		{
			data[i] = EepDrv_ReadDataWord( adr + i);
		}
		/* �p���e�B�`�F�b�N */
		parityResult = parity_check8(data, (sizeof(data) / sizeof(UI_16)));
		if(parityResult == PARITY_NORMAL)
		{
			/* bit8-15�Ȃ̂�8bit�V�t�g */
			*pData = (wordData >> 8);
		}
		else
		{
			*pData = EEPROM_DEFLT_VAL_BAT;
			ret = N_NG;
		}
	}
	else if(adr == EEPROM_ADR_FIRMWARE_VER)
	{
		if (wordData == 0xFFFFU)
		{
			*pData = EEPROM_DEFLT_VAL_FIRMWARE_VER;
		}
		else
		{
			*pData = wordData;
		}
	}
	else if(adr == EEPROM_ADR_EEPROM_VER)
	{
		if (wordData == 0xFFFFU)
		{
			*pData = EEPROM_DEFLT_VAL_EEPROM_VER;
		}
		else
		{
			*pData = wordData;
		}
	}
	else
	{
		*pData = wordData;
	}

	return ret;
}


/*==========================================================================*/
/*	8bit�p���e�B�Z�o/�␳����												*/
/*--------------------------------------------------------------------------*/
/*	UI_16 parity8(UI_16 *p, SI_8 n)											*/
/*--------------------------------------------------------------------------*/
/*	����:	UI_16	*p		parity�Z�o�Ώۃf�[�^�̐擪�A�h���X				*/
/*			SI_8	n		�f�[�^�̃��[�h��(1�`4)							*/
/*	�ߒl:	parity�Z�o����													*/
/*--------------------------------------------------------------------------*/
/*	�@�\:	endian�Ɉˑ����Ȃ�8bit�P�ʂ�parity�v�Z							*/
/*																			*/
/*		�f�[�^�ɑ΂���parity�Z�o���ʂ͈ȉ��̂Ƃ���							*/
/*																			*/
/*				(b7-0)														*/
/*				 b15-8 b7-0													*/
/*		addr	+-----+-----+												*/
/*		+0		|  H  |  L  |												*/
/*				+-----+-----+												*/
/*		+1		|  H  |  L  |												*/
/*				+-----+-----+												*/
/*		�`																	*/
/*				+-----+-----+												*/
/*		+(n-1)	|  H  |  L  |												*/
/*				+-----+-----+												*/
/*																			*/
/*																			*/
/*	|b15 b14 b13 b12 b11 b10 b9  b8 | b7 b6  b5  b4  b3  b2  b1  b0 |		*/
/*	|			����parity			|			����parity			|		*/
/*	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+		*/
/*	|+3L|+3H|+2L|+2H|+1L|+1H|+0L|+0H| b7| b6| b5| b4| b3| b2| b1| b0|		*/
/*	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+		*/
/*																			*/
/*--------------------------------------------------------------------------*/
/*	���l:	�f�[�^���[�h�� n �Ɉȉ��̒l���w�肵���ꍇ�̓���͕ۏ؂��܂���	*/
/*				1��菬�����l												*/
/*				4���傫���l												*/
/*--------------------------------------------------------------------------*/
/*	author:	2002/07/25	YAMAZAKI Iwao										*/
/*==========================================================================*/
EEP_LOCAL UI_16 parity8(UI_16 *p, SI_8 n)
{
	UI_16 *q;
	UI_16 t;
	UI_16 v, h;

	q = p + (n-1);						/* �Z�o�Ώۃu���b�N�ŏI�ʒu			*/

	h = 0x0000;							/* ����parity ����					*/
	v = 0x00ff;							/* ����parity �					*/
										/*   �����l 0x0000: ����parity		*/
										/*          0x00ff: �parity		*/
	do {
		t = *q;							/* ���[�h�f�[�^�擾					*/
		v ^= t;							/* ����parity�Z�o					*/
		t ^= (UI_16)(t >> 4);			/* ����parity�Z�o					*/
		t ^= (UI_16)(t >> 2);
		t ^= (UI_16)(t >> 1);
		t &= 0x0101;					/* 0x0101�̈ʒu��parity���o��		*/
		h = (UI_16)((h<<1) | (t<<8));	/* ����byte�̐���parity				*/
		h = (UI_16)((h<<1) | t);		/* ���byte�̐���parity				*/
		q--;							/* �擪�Ɍ������ăf�[�^�X�V			*/
	} while (q >= p);					/* �f�[�^�擪�܂Ōv�Z�𑱂���		*/
	v = (UI_16)((v>>8) ^ v);			/* ���,����byte�̐���parity������	*/

	return (UI_16)((h & 0xff00U) | (v & 0x00ffU));
}

/*==========================================================================*/
/*	8bit�p���e�B�`�F�b�N/�␳�����i���g�p�̈斳���j							*/
/*--------------------------------------------------------------------------*/
/*	UI_8 parity_check8(UI_16 *p, SI_8 n)									*/
/*--------------------------------------------------------------------------*/
/*	����:	UI_16	*p		parity�`�F�b�N�Ώۃu���b�N�̐擪�A�h���X		*/
/*			UI_8	n		�u���b�N�̃��[�h��(2�`5)						*/
/*	�ߒl:	parity�`�F�b�N����	= PARITY_NORMAL		:	����				*/
/*								= PARITY_ERROR_1BIT	:	1bit�ُ�			*/
/*								= PARITY_ERROR_2BIT :	�ُ�				*/
/*--------------------------------------------------------------------------*/
/*	�@�\:	parity�̖��g�p�̈���f�[�^�Ƃ��Ďg�p���Ȃ��^�C�v��				*/
/*			endian�Ɉˑ����Ȃ�8bit�P�ʂ�parity�`�F�b�N						*/
/*																			*/
/*		�ΏۂƂ���u���b�N�̍\���͈ȉ��̂Ƃ���								*/
/*																			*/
/*				(b7-0)														*/
/*				 b15-8 b7-0													*/
/*		addr	+-----+-----+												*/
/*		+0		|  H  |  L  |												*/
/*				+-----+-----+												*/
/*		+1		|  H  |  L  |												*/
/*				+-----+-----+												*/
/*		�`																	*/
/*				+-----+-----+												*/
/*		+(n-2)	|  H  |  L  |												*/
/*				+-+---+-----+												*/
/*		+(n-1)	|/|PH | PV  |												*/
/*				+-+---+-----+												*/
/*																			*/
/*																			*/
/*	|b15 b14 b13 b12 b11 b10 b9  b8 | b7 b6  b5  b4  b3  b2  b1  b0 |		*/
/*	|			����parity			|			����parity			|		*/
/*	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+		*/
/*	|+3L|+3H|+2L|+2H|+1L|+1H|+0L|+0H| b7| b6| b5| b4| b3| b2| b1| b0|		*/
/*	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+		*/
/*																			*/
/*--------------------------------------------------------------------------*/
/*	���l:	�u���b�N���[�h�� n �Ɉȉ��̒l���w�肵���ꍇ�̓���͕ۏ؂��܂���	*/
/*				2��菬�����l												*/
/*				5���傫���l												*/
/*--------------------------------------------------------------------------*/
/*	author:	2002/07/25	YAMAZAKI Iwao										*/
/*	modify:	2010/11/15	YASHIRO Hidetaka									*/
/*==========================================================================*/
EEP_LOCAL UI_8 parity_check8(UI_16 *p, SI_8 n)
{
	UI_16 v, h;
	UI_8 ret;
	static const UI_16 mask[] = {
		0x03ffU, 0x0fffU, 0x3fffU, 0xffffU,
	};

	v = (UI_16)(p[n-1] & mask[n-2]);	/* parity�̋L���l�擾				*/
	v ^= parity8(p, (SI_8)(n-1));		/* parity�̎Z�o�l�ƋL���l���r		*/

	h = (UI_16)(v >> 8);				/* ����parity�̑���					*/
	v = (UI_16)(v & 0x00ff);			/* ����parity�̑���					*/

	if (((v & (v-1)) != 0U) || ((h & (h-1)) != 0U)){
		ret = PARITY_ERROR_2BIT;		/* 2bit�ȏ�̑��ၨ�ُ�				*/
	}else if ((v == 0) && (h == 0)){
		ret = PARITY_NORMAL;			/* �ُ�Ȃ�							*/
	}else if ((v == 0) || (h == 0)){
		ret = PARITY_ERROR_1BIT;		/* 1bit�̑��ၨparity���̌̏�		*/
	}else{

		/*----------------------------------*/
		/* �ȉ������A����parity�ɂ��ꂼ��	*/
		/* 1bit�̑��ၨ�f�[�^����1bit�̏�	*/
		/* �̏�bit�̕␳���s��				*/
		/*----------------------------------*/
		n = 0;
		if ((h & 0xcc) != 0U){
			n += 1;							/* ����parity�̑��Ⴉ��̏�bit�����o*/
		}
		if ((h & 0xf0) != 0U){
			n += 2;
		}
		if ((h & 0x55) != 0U){
			v <<= 8;						/* ���byte�̏ꍇ�A��ʂɈړ�		*/
		}
		p[n] ^= v;							/* �̏�bit�̕␳					*/

		ret = PARITY_ERROR_1BIT;
	}
	return ret;
}





