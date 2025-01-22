/****************************************************************************/
/* Customer			: Customer_XXXX											*/
/* Model(Thema No.)	: Model_XXXX											*/
/*--------------------------------------------------------------------------*/
/* CPU				: FR81S													*/
/* Date				: 16/08/18												*/
/*--------------------------------------------------------------------------*/
/* Programmed by	: (OFT)Toguch											*/
/* Copyrights Nippon Seiki Co.,Ltd											*/
/*--------------------------------------------------------------------------*/
/* update by		:														*/
/* date				:														*/
/****************************************************************************/
/****************************************************************************/
/* file DTO459_VDV_File_io.c												*/
/* �t�@�C��IO																*/
/****************************************************************************/


/****************************************************************************/
/*	�C���N���[�h�t�@�C��													*/
/****************************************************************************/
#include <string.h>
#include "system.h"
#include "SHRVSTD_Port_Drv.h"
#include "DTO459_VDV_Sd_Drv.h"
#include "System_control_TASK.h"
#include "chatt_absorber.h"
#include "DTO459_VDV_RringBuffer.h"
#include "DTO459_VDV_Fat.h"
#include "DTO459_VDV_File_io.h"
#include "dbglog.h"



/****************************************************************************/
/*	�}�N����`																*/
/****************************************************************************/
/* ���b�Z�[�WID */ 
typedef enum e_fio_msg_id
{
	FIO_MSG_ID_READ_REQ	= 1,				/* ���[�h�v��				*/
	FIO_MSG_ID_WRITE_REQ,					/* ���C�g�v��				*/
	FIO_MSG_ID_SRC_FILE_REQ,				/* �t�@�C�������v��			*/
	FIO_MSG_ID_MKDIR_REQ					/* �f�B���N�g���쐬�v��		*/
} E_FIO_MSG_ID;

/* �t�@�C���V�X�e�����Ή�����h���C�uID	*/
typedef enum e_fio_drv_id
{
	FIO_DRV_ID_SD		= 0,				/* SD				*/

	/* ��������ɒǉ����邱��	*/
	/* �ǉ������ꍇ��C_fio_drv_name�Ƀh���C�u����ǉ����邱��	*/
	FIO_DRV_ID_NUM,							/* �h���C�u��		*/
	FIO_DRV_ID_INVALID						/* �����l			*/
} E_FIO_DRV_ID;

/* ���b�Z�[�W�L���[�̒i�� */
#define FIO_MSGQUE_NUM			16

/* �}�E���g�����̃��g���C�� */
#define FIO_MOUNT_RETRY_NUM		2

/****************************************************************************/
/*	�\���̒�`																*/
/****************************************************************************/
/* ���[�h�p�����^ */
typedef struct {
	UI_8				path[FIO_PATH_LEN_MAX+1];	/* �t�@�C���p�X							*/
	UI_8				*buff;						/* �o�b�t�@								*/
	UI_32				ofs;						/* �t�@�C���擪����̃I�t�Z�b�g(byte)	*/
	UI_32				bytes;						/* �T�C�Y(byte)							*/
	PFN_FIO_CB_NOTIFY	cbfnc;						/* �R�[���o�b�N�֐�						*/
} T_FIO_RD_PARAM;

/* ���C�g�p�����^ */
typedef struct {
	E_FIO_WRITE_TYPE	wtype;						/* �������ݎ��(FIO_WRITE_TYPE_xxx)		*/
	UI_8				path[FIO_PATH_LEN_MAX+1];	/* �t�@�C���p�X							*/
	const UI_8			*buff;						/* �o�b�t�@								*/
	UI_32				ofs;						/* �t�@�C���擪����̃I�t�Z�b�g(byte)	*/
	UI_32				bytes;						/* �T�C�Y(byte)							*/
	PFN_FIO_CB_NOTIFY	cbfnc;						/* �R�[���o�b�N�֐�						*/
} T_FIO_WR_PARAM;

/* �t�@�C�������p�����^ */
typedef struct {
	UI_8					path[FIO_PATH_LEN_MAX+1];		/* �����f�B���N�g���̃p�X		*/
	UI_8					filename[FIO_PATH_LEN_MAX+1];	/* ��������t�@�C����			*/
	PFN_FIO_SRCF_CB_NOTIFY	cbfnc;							/* �R�[���o�b�N�֐�				*/
} T_FIO_SRCF_PARAM;

/* �f�B���N�g���쐬�p�����^ */
typedef struct {
	UI_8					path[FIO_PATH_LEN_MAX+1];	/* �f�B���N�g���p�X						*/
	PFN_FIO_DIR_CB_NOTIFY	cbfnc;						/* �R�[���o�b�N�֐�						*/
} T_FIO_DIR_PARAM;

/* �v���p�����^ */
typedef	union {
	T_FIO_RD_PARAM		read;						/* ���[�h�p�����^						*/
	T_FIO_WR_PARAM		write;						/* ���C�g�p�����^						*/
	T_FIO_SRCF_PARAM	srcf;						/* �t�@�C�������p�����^					*/
	T_FIO_DIR_PARAM		dir;						/* �f�B���N�g���쐬�p�����^				*/
} U_FIO_REQ_PARAM;

/* �v�����b�Z�[�W */
typedef struct {
	E_FIO_MSG_ID		msgid;						/* ���b�Z�[�WID							*/
	UI_32				reqno;						/* �v��No								*/
	U_FIO_REQ_PARAM		param;						/* �v���p�����^							*/
} T_FIO_MSG;

/* �h���C�u���	*/
typedef struct {
	const UI_8			*drvname;					/* �h���C�u��							*/
	B_8					mountExecuted;				/* �}�E���g���s��� N_TRUE:�}�E���g���������s����	*/
	B_8					mount;						/* �}�E���g��� TRUE:�}�E���g�ς�		*/
	UI_8				resv[2];
	UI_32				free_sect_num;				/* �󂫃Z�N�^��							*/
} T_FIO_DRV_INFO;

/****************************************************************************/
/*	�萔�e�[�u��															*/
/****************************************************************************/
/* �h���C�u��	*/
static const UI_8 *C_fio_drv_name[FIO_DRV_ID_NUM] =
{
	(const UI_8*)FS_DRV_PATH_SD,								/* SD:FIO_DRV_ID_SD	*/
	/* �h���C�uID���Œǉ����邱�� */
};

/****************************************************************************/
/*	�����ϐ���`															*/
/****************************************************************************/
static UI_32 S_fio_reqno;									/* ���N�G�X�g�ԍ�(�O��IF�ŕԂ��V�[�P���V�����Ȕԍ�) */
static T_RINGBUFFER S_fio_msgbuff;							/* �v�����b�Z�[�W�o�b�t�@�Ǘ� */
static T_FIO_MSG S_fio_msg_data[FIO_MSGQUE_NUM];			/* �v�����b�Z�[�W�o�b�t�@ */
static UI_8 S_fio_sd_detect_in;								/* SD�}�����o�t���O */
static UI_8 S_fio_sd_detect_out;							/* SD�r�o���o�t���O */
static T_FIO_MSG S_fio_sndmsg_work;							/* ���M���b�Z�[�W�p���[�N�o�b�t�@ */
static T_FIO_MSG S_fio_rcvmsg_work;							/* ��M���b�Z�[�W�p���[�N�o�b�t�@ */
static T_FIO_REQ_INFO S_fio_cbparam_work;					/* �R�[���o�b�N�p�����^�p���[�N�o�b�t�@ */
static T_FIO_SRCF_REQ_INFO S_fio_srcf_cb_reqinfo_work;		/* �t�@�C�������̗v�����p���[�N�o�b�t�@ */
static T_FIO_SRCF_RET_INFO S_fio_srcf_cb_retinfo_work;		/* �t�@�C�������̌��ʏ��p���[�N�o�b�t�@ */
static T_FIO_DIR_REQ_INFO S_fio_dir_cb_reqinfo_work;		/* �f�B���N�g���쐬�̗v�����p���[�N�o�b�t�@ */
static UI_8 S_fio_dir_path_work[FIO_PATH_LEN_MAX+1];		/* �f�B���N�g���p�X���[�N�o�b�t�@ */
static FS_FILE_INFO S_fio_fsfinfo_work;						/* �t�@�C����񃏁[�N�o�b�t�@ */
static T_FIO_DRV_INFO S_fio_drv_info[FIO_DRV_ID_NUM];		/* �h���C�u��� */

/****************************************************************************/
/*	�����֐��錾															*/
/****************************************************************************/
static UI_32 fio_generate_reqno(void);
static E_FIO_RESULT fio_snd_msg(const T_FIO_MSG *msg);
static B_8 fio_rcv_msg(T_FIO_MSG *msg);
static void fio_make_reqinfo(T_FIO_MSG *msg, T_FIO_REQ_INFO *reqinfo);
static void fio_make_srcf_reqinfo(T_FIO_MSG *msg, T_FIO_SRCF_REQ_INFO *reqinfo);
static void fio_make_dir_reqinfo(T_FIO_MSG *msg, T_FIO_DIR_REQ_INFO *reqinfo);
static E_FIO_RESULT fio_read_proc(T_FIO_MSG *msg);
static E_FIO_RESULT fio_write_proc(T_FIO_MSG *msg);
static E_FIO_RESULT fio_searchfile_proc(T_FIO_MSG *msg);
static E_FIO_RESULT fio_makedir_proc(T_FIO_MSG *msg);
static UI_32 fio_get_dir_hierarchy_num(const UI_8 *path);
static SI_8 fio_get_dir_path(const UI_8 *path_src, UI_32 hnum, UI_8 *path_dst);
static void fio_cnv_res_fs2fio(E_FS_RESULT result_fs, E_FIO_RESULT *result_fio);
static void fio_mount_ctrl(void);
static void fio_mount_ctrl_sd(void);
static E_FIO_DRV_ID fio_path2drvid( const UI_8 *path );
static void fio_sdc_dtct_cb( UI_8 portlev );
static void fio_strcpy(UI_8 *dstBuff, UI_32 dstBuffSize, const UI_8 *srcBuff);


/****************************************************************************/
/*	�O���֐�																*/
/****************************************************************************/
/****************************************************************************/
/* �֐�����		:	FileIO_Read												*/
/* �@�\�̐���	:	�t�@�C���ǂݍ���(�񓯊�)								*/
/* ����			:	[I/ ]path	�t�@�C���p�X								*/
/*								(�{�֐��I����A�̈�����)					*/
/*					[I/ ]buff	�ǂݍ��݃f�[�^�o�b�t�@						*/
/*								(�R�[���o�b�N�܂ŗ̈��ۏ؂��邱��)		*/
/*					[I/ ]ofs	�t�@�C���擪����̃I�t�Z�b�g(byte)			*/
/*					[I/ ]bytes	�ǂݍ��ރo�C�g��							*/
/*					[I/ ]cbfnc	�R�[���o�b�N�֐�							*/
/*								(�{�֐�������I���̏ꍇ�ɃR�[���o�b�N����)	*/
/*					[ /O]reqno	�v��No										*/
/*								(�R�[���o�b�N�œ���̗v��No��Ԃ�)			*/
/* �߂�l		: 	FIO_RESULT_OK		����I��							*/
/*					FIO_RESULT_ERR_xxx	�ُ�I��							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/08/18												*/
/* ���l			:	�X���b�h�Z�[�t�ł͂Ȃ����ߓ���v���Z�X����Ăяo������	*/
/****************************************************************************/
E_FIO_RESULT FileIO_Read(
	const UI_8				*path,
	UI_8					*buff,
	UI_32					ofs,
	UI_32					bytes,
	PFN_FIO_CB_NOTIFY		cbfnc,
	UI_32					*reqno
)
{
	E_FIO_RESULT		result;
	T_FIO_RD_PARAM		*param;
	UI_32	pathlen;

	/* ���̓p�����^�`�F�b�N */
	if ((path == (UI_8*)NULL) ||
		(buff == (UI_8*)NULL) ||
		(bytes == 0) ||
		(cbfnc == (PFN_FIO_CB_NOTIFY)NULL) ||
		(reqno == (UI_32*)NULL)) {
		OUTLOG(OutputLog("[FIO]ReadIf Param ERR!!\n");)
		return FIO_RESULT_ERR_PARAM;
	}
	pathlen = strlen((const CH_8*)path);
	if (pathlen > FIO_PATH_LEN_MAX) {
		OUTLOG(OutputLog("[FIO]ReadIf Param pathlen ERR!!\n");)
		return FIO_RESULT_ERR_PARAM;
	}

	/* ���b�Z�[�W�쐬 */
	*reqno = fio_generate_reqno();
	S_fio_sndmsg_work.msgid = FIO_MSG_ID_READ_REQ;
	S_fio_sndmsg_work.reqno = *reqno;

	param = &S_fio_sndmsg_work.param.read;
	fio_strcpy(param->path, sizeof(param->path), path);
	param->buff = buff;
	param->ofs = ofs;
	param->bytes = bytes;
	param->cbfnc = cbfnc;

	/* ���b�Z�[�W���M */
	result = fio_snd_msg(&S_fio_sndmsg_work);
	if (result != FIO_RESULT_OK) {
		OUTLOG(OutputLog("[FIO]ReadIf SndMsg ERR!!\n");)
	}

	return result;
}

/****************************************************************************/
/* �֐�����		:	FileIO_Write											*/
/* �@�\�̐���	:	�t�@�C����������(�񓯊�)								*/
/* ����			:	[I/ ]wtype	�������ݎ��(FIO_WRITE_TYPE_xxx)			*/
/* 					[I/ ]path	�t�@�C���p�X								*/
/*								(�{�֐��I����A�̈�����)					*/
/*					[I/ ]buff	�������݃f�[�^�o�b�t�@						*/
/*								(�R�[���o�b�N�܂ŗ̈��ۏ؂��邱��)		*/
/*					[I/ ]ofs	�t�@�C���擪����̃I�t�Z�b�g(byte)			*/
/*					[I/ ]bytes	�������ރo�C�g��							*/
/*					[I/ ]cbfnc	�R�[���o�b�N�֐�							*/
/*								(�{�֐�������I���̏ꍇ�ɃR�[���o�b�N����)	*/
/*					[ /O]reqno	�v��No										*/
/*								(�R�[���o�b�N�œ���̗v��No��Ԃ�)			*/
/* �߂�l		: 	FIO_RESULT_OK		����I��							*/
/*					FIO_RESULT_ERR_xxx	�ُ�I��							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/08/18												*/
/* ���l			:	�X���b�h�Z�[�t�ł͂Ȃ����ߓ���v���Z�X����Ăяo������	*/
/****************************************************************************/
E_FIO_RESULT FileIO_Write(
	E_FIO_WRITE_TYPE		wtype,
	const UI_8				*path,
	const UI_8				*buff,
	UI_32					ofs,
	UI_32					bytes,
	PFN_FIO_CB_NOTIFY		cbfnc,
	UI_32					*reqno
)
{
	E_FIO_RESULT		result;
	T_FIO_WR_PARAM		*param;
	UI_32	pathlen;

	/* ���̓p�����^�`�F�b�N */
	if ((path == (UI_8*)NULL) ||
		(buff == (UI_8*)NULL) ||
		(bytes == 0) ||
		(cbfnc == (PFN_FIO_CB_NOTIFY)NULL) ||
		(reqno == (UI_32*)NULL)) {
		OUTLOG(OutputLog("[FIO]WriteIf Param ERR!!\n");)
		return FIO_RESULT_ERR_PARAM;
	}
	pathlen = strlen((const CH_8*)path);
	if (pathlen > FIO_PATH_LEN_MAX) {
		OUTLOG(OutputLog("[FIO]WriteIf Param pathlen ERR!!\n");)
		return FIO_RESULT_ERR_PARAM;
	}

	/* ���b�Z�[�W�쐬 */
	*reqno = fio_generate_reqno();
	S_fio_sndmsg_work.msgid = FIO_MSG_ID_WRITE_REQ;
	S_fio_sndmsg_work.reqno = *reqno;

	param = &S_fio_sndmsg_work.param.write;
	param->wtype = wtype;
	fio_strcpy(param->path, sizeof(param->path), path);
	param->buff = buff;
	param->ofs = ofs;
	param->bytes = bytes;
	param->cbfnc = cbfnc;

	/* ���b�Z�[�W���M */
	result = fio_snd_msg(&S_fio_sndmsg_work);
	if (result != FIO_RESULT_OK) {
		OUTLOG(OutputLog("[FIO]WriteIf SndMsg ERR!!\n");)
	}

	return result;
}

/****************************************************************************/
/* �֐�����		:	FileIO_SearchFile										*/
/* �@�\�̐���	:	�t�@�C������(�񓯊�)									*/
/* ����			:	[I/ ]path		�����f�B���N�g���̃p�X					*/
/*									(�{�֐��I����A�̈�����)				*/
/* 				:	[I/ ]filename	��������t�@�C����						*/
/*									(�{�֐��I����A�̈�����)				*/
/*					[I/ ]cbfnc		�R�[���o�b�N�֐�						*/
/*								(�{�֐�������I���̏ꍇ�ɃR�[���o�b�N����)	*/
/*					[ /O]reqno		�v��No									*/
/*									(�R�[���o�b�N�œ���̗v��No��Ԃ�)		*/
/* �߂�l		: 	FIO_RESULT_OK		����I��							*/
/*					FIO_RESULT_ERR_xxx	�ُ�I��							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/08/18												*/
/* ���l			:	�t�@�C�����Ƀ��C���h�J�[�h����'?'�܂���'*'�̎w�肪�\�B*/
/*					'?'�͔C�ӂ�1�����ɁA'*'��0�����ȏ�̔C�ӂ̕������		*/
/*					�}�b�`����B											*/
/*					�}�b�`�����t�@�C�����������閈�ɃR�[���o�b�N��ʒm����B*/
/*					�R�[���o�b�N�ʒm�̍ő�񐔂̓}�b�`�����t�@�C�����{1��	*/
/*					�i�����I���������j�ƂȂ�B								*/
/*					�R�[���o�b�N�ʒm�̃t�@�C�����ϐ��Ɍ��������t�@�C������	*/
/*					�i�[����B�����I���̓t�@�C�����ϐ���NULL�������i�[����B*/
/*					�t�@�C���̒ʒm�͏��s���B�i�����ł������j				*/
/*					�R�[���o�b�N�֐��̖߂�l��N_TRUE��Ԃ����ꍇ�͌�����	*/
/*					�p������B												*/
/*					�R�[���o�b�N�֐��̖߂�l��N_FALSE��Ԃ����ꍇ�͌�����	*/
/*					�I������B�i�ȍ~�R�[���o�b�N�͒ʒm���Ȃ��j				*/
/*					�R�[���o�b�N�ʒm�̌������ʂ��G���[�̏ꍇ�͈ȍ~�R�[��	*/
/*					�o�b�N�͒ʒm���Ȃ��B									*/
/****************************************************************************/
E_FIO_RESULT FileIO_SearchFile(
	const UI_8				*path,
	const UI_8				*filename,
	PFN_FIO_SRCF_CB_NOTIFY	cbfnc,
	UI_32					*reqno
)
{
	E_FIO_RESULT		result;
	T_FIO_SRCF_PARAM	*param;
	UI_32				pathlen;
	UI_32				filelen;

	/* ���̓p�����^�`�F�b�N */
	if ((path == (UI_8*)NULL) ||
		(filename == (UI_8*)NULL) ||
		(cbfnc == (PFN_FIO_SRCF_CB_NOTIFY)NULL) ||
		(reqno == (UI_32*)NULL)) {
		OUTLOG(OutputLog("[FIO]SrcFileIf Param ERR!!\n");)
		return FIO_RESULT_ERR_PARAM;
	}
	pathlen = strlen((const CH_8*)path);
	if (pathlen > FIO_PATH_LEN_MAX) {
		OUTLOG(OutputLog("[FIO]SrcFileIf Param pathlen ERR!!\n");)
			return FIO_RESULT_ERR_PARAM;
	}
	filelen = strlen((const CH_8*)filename);
	if (filelen > FIO_PATH_LEN_MAX) {
		OUTLOG(OutputLog("[FIO]SrcFileIf Param filenamelen ERR!!\n");)
			return FIO_RESULT_ERR_PARAM;
	}

	/* ���b�Z�[�W�쐬 */
	*reqno = fio_generate_reqno();
	S_fio_sndmsg_work.msgid = FIO_MSG_ID_SRC_FILE_REQ;
	S_fio_sndmsg_work.reqno = *reqno;

	param = &S_fio_sndmsg_work.param.srcf;
	fio_strcpy(param->path, sizeof(param->path), path);
	fio_strcpy(param->filename, sizeof(param->filename), filename);
	param->cbfnc = cbfnc;

	/* ���b�Z�[�W���M */
	result = fio_snd_msg(&S_fio_sndmsg_work);
	if (result != FIO_RESULT_OK) {
		OUTLOG(OutputLog("[FIO]SrcFileIf SndMsg ERR!!\n");)
	}

	return result;
}

/****************************************************************************/
/* �֐�����		:	FileIO_MakeDir											*/
/* �@�\�̐���	:	�f�B���N�g���쐬(�񓯊�)								*/
/* ����				[I/ ]path	�f�B���N�g���p�X							*/
/*								(�{�֐��I����A�̈�����)					*/
/*					[I/ ]cbfnc	�R�[���o�b�N�֐�							*/
/*								(�{�֐�������I���̏ꍇ�ɃR�[���o�b�N����)	*/
/*					[ /O]reqno	�v��No										*/
/*								(�R�[���o�b�N�œ���̗v��No��Ԃ�)			*/
/* �߂�l		: 	FIO_RESULT_OK		����I��							*/
/*					FIO_RESULT_ERR_xxx	�ُ�I��							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:	�X���b�h�Z�[�t�ł͂Ȃ����ߓ���v���Z�X����Ăяo������	*/
/****************************************************************************/
E_FIO_RESULT FileIO_MakeDir(
	const UI_8				*path,
	PFN_FIO_DIR_CB_NOTIFY	cbfnc,
	UI_32					*reqno
)
{
	E_FIO_RESULT	result;
	T_FIO_DIR_PARAM	*param;
	UI_32	pathlen;

	/* ���̓p�����^�`�F�b�N */
	if ((path == (UI_8*)NULL) ||
		(cbfnc == (PFN_FIO_DIR_CB_NOTIFY)NULL) ||
		(reqno == (UI_32*)NULL)) {
		OUTLOG(OutputLog("[FIO]MakeDirIf Param ERR!!\n");)
		return FIO_RESULT_ERR_PARAM;
	}
	pathlen = strlen((const CH_8*)path);
	if (pathlen > FIO_PATH_LEN_MAX) {
		OUTLOG(OutputLog("[FIO]MakeDirIf Param pathlen ERR!!\n");)
		return FIO_RESULT_ERR_PARAM;
	}

	/* ���b�Z�[�W�쐬 */
	*reqno = fio_generate_reqno();
	S_fio_sndmsg_work.msgid = FIO_MSG_ID_MKDIR_REQ;
	S_fio_sndmsg_work.reqno = *reqno;

	param = &S_fio_sndmsg_work.param.dir;
	fio_strcpy(param->path, sizeof(param->path), path);
	param->cbfnc = cbfnc;

	/* ���b�Z�[�W���M */
	result = fio_snd_msg(&S_fio_sndmsg_work);
	if (result != FIO_RESULT_OK) {
		OUTLOG(OutputLog("[FIO]MakeDirIf SndMsg ERR!!\n");)
	}

	return result;
}

/****************************************************************************/
/* �֐�����		: FileIO_Get_FreeSectNum									*/
/* �@�\�̐���	: �󂫃Z�N�^���擾											*/
/* ����			: [I/ ]drvpath			�h���C�u�̃p�X						*/
/*				   [ /O]freesect		�󂫃Z�N�^��						*/
/* �߂�l		: 	FIO_RESULT_OK		����I��							*/
/*					FIO_RESULT_ERR_xxx	�ُ�I��							*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 16/08/18													*/
/* ���l			: 															*/
/****************************************************************************/
E_FIO_RESULT FileIO_Get_FreeSectNum(
	const UI_8			*drvpath,
	UI_32				*freesect
)
{
	E_FIO_DRV_ID	drvid;

	/* ���̓p�����^�`�F�b�N */
	if ((drvpath == NULL) || (freesect == NULL)) {
		return FIO_RESULT_ERR_PARAM;
	}

	drvid = fio_path2drvid( drvpath );
	if (drvid == FIO_DRV_ID_INVALID) {		/* �h���C�u���s��	*/
		return FIO_RESULT_ERR_INVALID_DRIVE;
	}
	if (S_fio_drv_info[drvid].mount == N_FALSE) {	/* �}�E���g���Ă��Ȃ�	*/
		return FIO_RESULT_ERR_DISK_NOT_READY;
	}

	*freesect = S_fio_drv_info[drvid].free_sect_num;

	return FIO_RESULT_OK;
}

/****************************************************************************/
/* �֐�����		: FileIO_Get_MountExecStat									*/
/* �@�\�̐���	: �}�E���g���s��Ԏ擾										*/
/* ����			: [I/ ]drvpath			�h���C�u�̃p�X						*/
/* �߂�l		: 	N_TRUE		�}�E���g�����s����							*/
/*					N_FALSE		�}�E���g�����s���Ă��Ȃ�					*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 16/11/11													*/
/* ���l			: 															*/
/****************************************************************************/
B_8 FileIO_Get_MountExecStat(
	const UI_8			*drvpath
)
{
	E_FIO_DRV_ID	drvid;

	/* ���̓p�����^�`�F�b�N */
	if (drvpath == NULL) {
		return N_FALSE;
	}

	drvid = fio_path2drvid( drvpath );
	if (drvid == FIO_DRV_ID_INVALID) {		/* �h���C�u���s��	*/
		return N_FALSE;
	}

	return S_fio_drv_info[drvid].mountExecuted;
}

/****************************************************************************/
/* �֐�����		: FileIO_Init												*/
/* �@�\�̐���	: �t�@�C��IO������											*/
/* ����			: void														*/
/* �߂�l		: void														*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 16/08/18													*/
/* ���l			: 															*/
/****************************************************************************/
void FileIO_Init(void)
{
	UI_8	i;

	/* �ϐ������� */
	S_fio_reqno = 0;
	S_fio_sd_detect_in = N_OFF;
	S_fio_sd_detect_out = N_OFF;
	RingBuffer_Init(&S_fio_msgbuff, (UI_8*)S_fio_msg_data, (UI_16)sizeof(S_fio_msg_data));

	for (i = 0; i < FIO_DRV_ID_NUM; i++) {
		S_fio_drv_info[i].drvname = C_fio_drv_name[i];
		S_fio_drv_info[i].mountExecuted = N_FALSE;
		S_fio_drv_info[i].mount = N_FALSE;
		S_fio_drv_info[i].free_sect_num = 0;
	}

	/* SD�J�[�hIN/OUT���o�̃R�[���o�b�N��o�^ */
	(void)Chatt_absorber_ReqPortData(fio_sdc_dtct_cb, CHATT_PORT_SDC, CHATT_ABS_MOD_FIO);

	return;
}

/****************************************************************************/
/*	�t�@�C��IO���C���i������s�����j										*/
/****************************************************************************/
/****************************************************************************/
/* �֐�����		: FileIO_Main												*/
/* �@�\�̐���	: �t�@�C��IO���C������										*/
/* ����			: void														*/
/* �߂�l		: void														*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 16/08/18													*/
/* ���l			: 															*/
/****************************************************************************/
void FileIO_Main(void)
{
	/* �h���C�u�}�E���g���� */
	fio_mount_ctrl();


	/* �t�@�C���A�N�Z�X���b�Z�[�W��M */
	if (fio_rcv_msg(&S_fio_rcvmsg_work) == N_TRUE) {

		switch (S_fio_rcvmsg_work.msgid) {
		case FIO_MSG_ID_READ_REQ:				/* ���[�h�v�� */
			(void)fio_read_proc(&S_fio_rcvmsg_work);
			break;

		case FIO_MSG_ID_WRITE_REQ:				/* ���C�g�v�� */
			(void)fio_write_proc(&S_fio_rcvmsg_work);
			break;

		case FIO_MSG_ID_SRC_FILE_REQ:			/* �t�@�C�������v�� */
			(void)fio_searchfile_proc(&S_fio_rcvmsg_work);
			break;

		case FIO_MSG_ID_MKDIR_REQ:				/* �f�B���N�g���쐬�v�� */
			(void)fio_makedir_proc(&S_fio_rcvmsg_work);
			break;

		default:
			break;
		}

	}

	return;
}

/****************************************************************************/
/*	�����֐�																*/
/****************************************************************************/
/****************************************************************************/
/* �֐�����		: fio_generate_reqno										*/
/* �@�\�̐���	: �v��No����												*/
/* ����			: �Ȃ�														*/
/* �߂�l		: �v��No													*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 16/08/18													*/
/* ���l			: 															*/
/****************************************************************************/
static UI_32 fio_generate_reqno(void)
{

	S_fio_reqno++;
	if (S_fio_reqno == 0) {
		S_fio_reqno = 1;
	}
	return S_fio_reqno;

}

/****************************************************************************/
/* �֐�����		: fio_snd_msg												*/
/* �@�\�̐���	: ���b�Z�[�W���M											*/
/* ����			: [I/ ]msg		���M���b�Z�[�W								*/
/* �߂�l		: 	FIO_RESULT_OK		����I��							*/
/*					FIO_RESULT_ERR_xxx	�ُ�I��							*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 16/08/18													*/
/* ���l			: 															*/
/****************************************************************************/
static E_FIO_RESULT fio_snd_msg(
	const T_FIO_MSG *msg
)
{

	E_FIO_RESULT result;
	UI_16 bytes;

	bytes = RingBuffer_GetCanWriteSize(&S_fio_msgbuff);
	if (bytes >= sizeof(T_FIO_MSG)) {
		(void)RingBuffer_SetArrayData(&S_fio_msgbuff, (UI_8*)msg, (UI_16)sizeof(T_FIO_MSG));
		result = FIO_RESULT_OK;
	}
	else {
		result = FIO_RESULT_ERR_SNDMSG;
	}

	return result;

}

/****************************************************************************/
/* �֐�����		: fio_rcv_msg												*/
/* �@�\�̐���	: ���b�Z�[�W��M											*/
/* ����			: [ /O]msg		��M���b�Z�[�W								*/
/* �߂�l		: 	N_TRUE		��M���b�Z�[�W����							*/
/* 				 	N_FALSE		��M���b�Z�[�W�Ȃ�							*/
/* �쐬��		: (OFT)Toguchi												*/
/* �쐬��		: 16/08/18													*/
/* ���l			: 															*/
/****************************************************************************/
static B_8 fio_rcv_msg(
	T_FIO_MSG *msg
)
{

	B_8 result;
	UI_16 bytes;

	bytes = RingBuffer_GetCanReadSize(&S_fio_msgbuff);
	if (bytes >= sizeof(T_FIO_MSG)) {
		(void)RingBuffer_GetArrayData(&S_fio_msgbuff, (UI_8*)msg, (UI_16)sizeof(T_FIO_MSG), RINGBUF_READEND);
		result = N_TRUE;
	}
	else {
		result = N_FALSE;
	}

	return result;

}

/****************************************************************************/
/* �֐�����		:	fio_make_reqinfo										*/
/* �@�\�̐���	:	�v�����쐬											*/
/* ����			:	[I/ ]msg		���b�Z�[�W								*/
/*					[ /O]reqinfo	�v�����								*/
/* �߂�l		:	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/08/18												*/
/* ���l			: 															*/
/****************************************************************************/
static void fio_make_reqinfo(
	T_FIO_MSG			*msg,
	T_FIO_REQ_INFO		*reqinfo
)
{

	T_FIO_RD_PARAM	*rd_param;
	T_FIO_WR_PARAM	*wr_param;

	if (msg->msgid == FIO_MSG_ID_READ_REQ) {
		reqinfo->reqtype = FIO_REQ_TYPE_READ;
		reqinfo->reqno = msg->reqno;
		rd_param = &msg->param.read;
		fio_strcpy(reqinfo->path, sizeof(reqinfo->path), rd_param->path);
		reqinfo->buff = rd_param->buff;
		reqinfo->ofs = rd_param->ofs;
		reqinfo->bytes = rd_param->bytes;
	}
	else if (msg->msgid == FIO_MSG_ID_WRITE_REQ) {
		reqinfo->reqtype = FIO_REQ_TYPE_WRITE;
		reqinfo->reqno = msg->reqno;
		wr_param = &msg->param.write;
		fio_strcpy(reqinfo->path, sizeof(reqinfo->path), wr_param->path);
		reqinfo->buff = (UI_8*)wr_param->buff;
		reqinfo->ofs = wr_param->ofs;
		reqinfo->bytes = wr_param->bytes;
	}
	else {
		(void)memset(reqinfo, 0, sizeof(T_FIO_REQ_INFO));
	}

	return;

}

/****************************************************************************/
/* �֐�����		:	fio_make_srcf_reqinfo									*/
/* �@�\�̐���	:	�v�����쐬(�t�@�C�������p)							*/
/* ����			:	[I/ ]msg		���b�Z�[�W								*/
/*					[ /O]reqinfo	�v�����								*/
/* �߂�l		:	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/08/18												*/
/* ���l			: 															*/
/****************************************************************************/
static void fio_make_srcf_reqinfo(
	T_FIO_MSG			*msg,
	T_FIO_SRCF_REQ_INFO	*reqinfo
)
{

	T_FIO_SRCF_PARAM	*param;

	param = &msg->param.srcf;
	fio_strcpy(reqinfo->path, sizeof(reqinfo->path), param->path);
	fio_strcpy(reqinfo->filename, sizeof(reqinfo->filename), param->filename);
	reqinfo->reqno = msg->reqno;

	return;

}

/****************************************************************************/
/* �֐�����		:	fio_make_dir_reqinfo									*/
/* �@�\�̐���	:	�v�����쐬(�f�B���N�g������p)						*/
/* ����			:	[I/ ]msg		���b�Z�[�W								*/
/*					[ /O]reqinfo	�v�����								*/
/* �߂�l		:	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/08/18												*/
/* ���l			: 															*/
/****************************************************************************/
static void fio_make_dir_reqinfo(
	T_FIO_MSG			*msg,
	T_FIO_DIR_REQ_INFO	*reqinfo
)
{

	T_FIO_DIR_PARAM	*param;

	param = &msg->param.dir;
	fio_strcpy(reqinfo->path, sizeof(reqinfo->path), param->path);
	reqinfo->reqno = msg->reqno;

	return;

}

/****************************************************************************/
/* �֐�����		:	fio_read_proc											*/
/* �@�\�̐���	:	�t�@�C���ǂݍ���										*/
/* ����			:	[I/ ]msg		���b�Z�[�W								*/
/* �߂�l		: 	FIO_RESULT_OK		����I��							*/
/*					FIO_RESULT_ERR_xxx	�ُ�I��							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/08/18												*/
/* ���l			:															*/
/****************************************************************************/
static E_FIO_RESULT fio_read_proc(
	T_FIO_MSG		*msg
)
{

	E_FIO_RESULT	result;
	E_FS_RESULT		result_fs;
	UI_8			fid;
	UI_32			bytesRead;
	UI_32			fSize;
	T_FIO_RET_INFO	retinfo;
	E_FIO_DRV_ID	drvid;

	fid = 0;
	bytesRead = 0;
	fSize = 0;

	/* �h���C�u�̃}�E���g��Ԃ��`�F�b�N	*/
	drvid = fio_path2drvid( msg->param.read.path );
	if (drvid == FIO_DRV_ID_INVALID) {		/* �h���C�u���s��	*/
		result = FIO_RESULT_ERR_INVALID_DRIVE;
		OUTLOG(OutputLog("[FIO]Read drive name ERR!!\n");)
		goto PROC_END;
	}
	if (S_fio_drv_info[drvid].mount == N_FALSE) {	/* �}�E���g���Ă��Ȃ�	*/
		result = FIO_RESULT_ERR_DISK_NOT_READY;
		OUTLOG(OutputLog("[FIO]Read mount ERR!!\n");)
		goto PROC_END;
	}

	/* �t�@�C���I�[�v�� */
	result_fs = FileSystem_Open(msg->param.read.path, FS_FA_MODE_READ, &fid);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Read FsOpen ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}

	/* ���̓I�t�Z�b�g�l�`�F�b�N */
	result_fs = FileSystem_Size(fid, &fSize);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Read FsSize ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}
	if (fSize < msg->param.read.ofs) {			/* �I�t�Z�b�g���t�@�C���T�C�Y���傫�� */
		result = FIO_RESULT_ERR_PARAM;
		goto PROC_END;
	}

	/* �ǂݍ��݈ʒu�Ɉړ� */
	result_fs = FileSystem_Seek(fid, msg->param.read.ofs);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Read FsSeek ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}

	/* �t�@�C���ǂݍ��� */
	result_fs = FileSystem_Read(fid, msg->param.read.buff, msg->param.read.bytes, &bytesRead);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Read FsRead ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}

	result = FIO_RESULT_OK;		/* ����I��	*/

PROC_END:

	/* �t�@�C���N���[�Y */
	if (fid > 0) {
		(void)FileSystem_Close(fid);
	}

	/* �R�[���o�b�N�ʒm */
	if (msg->param.read.cbfnc != NULL) {
		retinfo.result = result;
		retinfo.bytes = bytesRead;
		retinfo.fsize = fSize;
		fio_make_reqinfo(msg, &S_fio_cbparam_work);

		msg->param.read.cbfnc(&S_fio_cbparam_work, &retinfo);
	}

	return result;

}

/****************************************************************************/
/* �֐�����		:	fio_write_proc											*/
/* �@�\�̐���	:	�t�@�C����������										*/
/* ����			:	[I/ ]msg		���b�Z�[�W								*/
/* �߂�l		: 	FIO_RESULT_OK		����I��							*/
/*					FIO_RESULT_ERR_xxx	�ُ�I��							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	2016/MM/DD												*/
/* ���l			:															*/
/****************************************************************************/
static E_FIO_RESULT fio_write_proc(
	T_FIO_MSG		*msg
)
{

	E_FIO_RESULT	result;
	E_FS_RESULT		result_fs;
	UI_8			fid;
	UI_8			mode;
	UI_32			ofs_w;
	UI_32			bytesWritten;
	UI_32			fSize;
	T_FIO_RET_INFO	retinfo;
	E_FIO_DRV_ID	drvid;
	UI_16			sectPerClst;
	UI_32			freeClst;

	fid = 0;
	bytesWritten = 0;
	fSize = 0;

	/* �h���C�u�̃}�E���g��Ԃ��`�F�b�N	*/
	drvid = fio_path2drvid( msg->param.write.path );
	if (drvid == FIO_DRV_ID_INVALID) {		/* �h���C�u���s��	*/
		result = FIO_RESULT_ERR_INVALID_DRIVE;
		OUTLOG(OutputLog("[FIO]Write drive name ERR!!\n");)
		goto PROC_END;
	}
	if (S_fio_drv_info[drvid].mount == N_FALSE) {	/* �}�E���g���Ă��Ȃ�	*/
		result = FIO_RESULT_ERR_DISK_NOT_READY;
	/*	OUTLOG(OutputLog("[FIO]Write mount ERR!!\n");)	*/
		goto PROC_END;
	}

	/* �t�@�C���I�[�v�� */
	if (msg->param.write.wtype == FIO_WRITE_TYPE_FILE_CREATE) {
		mode = (FS_FA_MODE_CREATE_ALWAYS | FS_FA_MODE_WRITE);
	}
	else {
		mode = (FS_FA_MODE_OPEN_ALWAYS | FS_FA_MODE_WRITE);
	}
	result_fs = FileSystem_Open(msg->param.write.path, mode, &fid);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Write FsOpen ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}

	/* ���̓I�t�Z�b�g�l�`�F�b�N */
	result_fs = FileSystem_Size(fid, &fSize);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Write FsSize ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}
	if ((msg->param.write.ofs != FIO_OFS_EOF) && (fSize < msg->param.write.ofs)) {	/* �I�t�Z�b�g���t�@�C���T�C�Y���傫�� */
		result = FIO_RESULT_ERR_PARAM;
		goto PROC_END;
	}

	/* �����݈ʒu�Ɉړ� */
	if (msg->param.write.ofs == FIO_OFS_EOF) {
		ofs_w = fSize;							/* �t�@�C���I�[�Ɉړ�		*/
	}
	else {
		ofs_w = msg->param.write.ofs;			/* �w��I�t�Z�b�g�Ɉړ�		*/
	}
	result_fs = FileSystem_Seek(fid, ofs_w);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Write FsSeek ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}

	/* �t�@�C���������� */
	result_fs = FileSystem_Write(fid, msg->param.write.buff, msg->param.write.bytes, &bytesWritten);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Write FsWrite ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}
	else {
		if (bytesWritten < msg->param.write.bytes) {	/* �v���T�C�Y���������߂Ă��Ȃ�	*/
			/* �󂫂Ȃ��i�󂫃Z�N�^��:0�j */
			S_fio_drv_info[drvid].free_sect_num = 0;
			OUTLOG(OutputLog("[FIO]Write FsWrite memory full!!\n");)
		}
		else {
			/* �󂫃Z�N�^���擾 */
			result_fs = FileSystem_GetFree(msg->param.write.path, &freeClst, &sectPerClst);
			if (result_fs == FS_RESULT_OK) {
				if (freeClst == FS_FREE_CLUST_NUM_UNKNOWN) {
					S_fio_drv_info[drvid].free_sect_num = FIO_FREE_SECT_NUM_UNKNOWN;
				}
				else {
					S_fio_drv_info[drvid].free_sect_num = freeClst * sectPerClst;
				}
			}
			else {
				S_fio_drv_info[drvid].free_sect_num = FIO_FREE_SECT_NUM_UNKNOWN;
			}
		}
	}

	/* �t�@�C���T�C�Y�擾 */
	result_fs = FileSystem_Size(fid, &fSize);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Write FsSize 2 ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}

	result = FIO_RESULT_OK;		/* ����I��	*/

PROC_END:

	/* �t�@�C���N���[�Y */
	if (fid > 0) {
		(void)FileSystem_Close(fid);
	}

	/* �R�[���o�b�N�ʒm */
	if (msg->param.write.cbfnc != NULL) {
		retinfo.result = result;
		retinfo.bytes = bytesWritten;
		retinfo.fsize = fSize;
		fio_make_reqinfo(msg, &S_fio_cbparam_work);

		msg->param.write.cbfnc(&S_fio_cbparam_work, &retinfo);
	}

	return result;

}

/****************************************************************************/
/* �֐�����		:	fio_searchfile_proc										*/
/* �@�\�̐���	:	�t�@�C������											*/
/* ����			:	[I/ ]msg		���b�Z�[�W								*/
/* �߂�l		: 	FIO_RESULT_OK		����I��							*/
/*					FIO_RESULT_ERR_xxx	�ُ�I��							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	2016/MM/DD												*/
/* ���l			:															*/
/****************************************************************************/
static E_FIO_RESULT fio_searchfile_proc(
	T_FIO_MSG		*msg
)
{

	E_FIO_RESULT	result;
	E_FS_RESULT		result_fs;
	B_8				file_exist;
	B_8				ret_callback;
	E_FIO_DRV_ID	drvid;
	UI_8			did;
	B_8				find_first;

	if (msg->param.srcf.cbfnc == NULL) {
		return FIO_RESULT_ERR_FATAL;
	}

	/* �R�[���o�b�N�p�����^�쐬	*/
	fio_make_srcf_reqinfo(msg, &S_fio_srcf_cb_reqinfo_work);

	/* �h���C�u�̃}�E���g��Ԃ��`�F�b�N	*/
	drvid = fio_path2drvid( msg->param.srcf.path );
	if (drvid == FIO_DRV_ID_INVALID) {		/* �h���C�u���s��	*/
		result = FIO_RESULT_ERR_INVALID_DRIVE;
		OUTLOG(OutputLog("[FIO]SrcFile drive name ERR!!\n");)
		goto ERR_END;
	}
	if (S_fio_drv_info[drvid].mount == N_FALSE) {	/* �}�E���g���Ă��Ȃ�	*/
		result = FIO_RESULT_ERR_DISK_NOT_READY;
		OUTLOG(OutputLog("[FIO]SrcFile mount ERR!!\n");)
		goto ERR_END;
	}

	/* �擪�t�@�C������	*/
	find_first = N_FALSE;
	did = 0;
	(void)memset(&S_fio_srcf_cb_retinfo_work, 0, sizeof(S_fio_srcf_cb_retinfo_work));
	result_fs = FileSystem_FindFirst(&did, &S_fio_fsfinfo_work, msg->param.srcf.path, msg->param.srcf.filename);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
	}
	else {
		result = FIO_RESULT_OK;
		fio_strcpy(S_fio_srcf_cb_retinfo_work.filename, sizeof(S_fio_srcf_cb_retinfo_work.filename), S_fio_fsfinfo_work.fname);
		if (S_fio_fsfinfo_work.fname[0] == '\0') {
			result = FIO_RESULT_ERR_NO_FILE;
		}
		else {
			find_first = N_TRUE;
		}
	}

	/* �R�[���o�b�N�ʒm(�擪�t�@�C���������ʒʒm) */
	S_fio_srcf_cb_retinfo_work.result = result;
	ret_callback = msg->param.srcf.cbfnc(&S_fio_srcf_cb_reqinfo_work, &S_fio_srcf_cb_retinfo_work);

	if (find_first == N_TRUE) {
		/* �Q�Ԗڈȍ~�̃t�@�C������	*/
		file_exist = N_TRUE;
		while ((file_exist == N_TRUE) && (ret_callback == N_TRUE)) {
			(void)memset(&S_fio_srcf_cb_retinfo_work, 0, sizeof(S_fio_srcf_cb_retinfo_work));
			result_fs = FileSystem_FindNext(did, &S_fio_fsfinfo_work);
			if (result_fs != FS_RESULT_OK) {
				fio_cnv_res_fs2fio(result_fs, &result);
				file_exist = N_FALSE;
			}
			else {
				result = FIO_RESULT_OK;
				fio_strcpy(S_fio_srcf_cb_retinfo_work.filename, sizeof(S_fio_srcf_cb_retinfo_work.filename), S_fio_fsfinfo_work.fname);
				if (S_fio_fsfinfo_work.fname[0] == '\0') {
					result = FIO_RESULT_ERR_NO_FILE;
					file_exist = N_FALSE;
				}
			}

			/* �R�[���o�b�N�ʒm(2�Ԗڈȍ~�̃t�@�C���������ʒʒm) */
			S_fio_srcf_cb_retinfo_work.result = result;
			ret_callback = msg->param.srcf.cbfnc(&S_fio_srcf_cb_reqinfo_work, &S_fio_srcf_cb_retinfo_work);
		}
	}

	/* �f�B���N�g���N���[�Y */
	if (did > 0) {
		(void)FileSystem_CloseDir(did);
	}

	if ((result == FIO_RESULT_ERR_NO_FILE) && (find_first == N_TRUE)) {
		/* �擪�t�@�C���̂݌����ł����ꍇ�͖߂�l:�����Ԃ� */
		result = FIO_RESULT_OK;
	}

	return result;

ERR_END:

	/* �R�[���o�b�N�ʒm */
	S_fio_srcf_cb_retinfo_work.result = result;
	S_fio_srcf_cb_retinfo_work.filename[0] = '\0';
	msg->param.srcf.cbfnc(&S_fio_srcf_cb_reqinfo_work, &S_fio_srcf_cb_retinfo_work);

	return result;

}

/****************************************************************************/
/* �֐�����		:	fio_makedir_proc										*/
/* �@�\�̐���	:	�f�B���N�g���쐬										*/
/* ����			:	[I/ ]msg		���b�Z�[�W								*/
/* �߂�l		: 	FIO_RESULT_OK		����I��							*/
/*					FIO_RESULT_ERR_xxx	�ُ�I��							*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	2017/01/20												*/
/* ���l			:															*/
/****************************************************************************/
static E_FIO_RESULT fio_makedir_proc(
	T_FIO_MSG		*msg
)
{

	E_FIO_RESULT		result;
	E_FS_RESULT			result_fs;
	T_FIO_DIR_RET_INFO	retinfo;
	E_FIO_DRV_ID		drvid;
	UI_32				pathlen;
	UI_32				hierarchy_num;
	UI_32				i;

	/* �h���C�u�̃}�E���g��Ԃ��`�F�b�N	*/
	drvid = fio_path2drvid( msg->param.dir.path );
	if (drvid == FIO_DRV_ID_INVALID) {		/* �h���C�u���s��	*/
		result = FIO_RESULT_ERR_INVALID_DRIVE;
		OUTLOG(OutputLog("[FIO]MakeDir drive name ERR!!\n");)
		goto PROC_END;
	}
	if (S_fio_drv_info[drvid].mount == N_FALSE) {	/* �}�E���g���Ă��Ȃ�	*/
		result = FIO_RESULT_ERR_DISK_NOT_READY;
/*		OUTLOG(OutputLog("[FIO]MakeDir mount ERR!!\n");) */
		goto PROC_END;
	}

	pathlen = strlen((const CH_8*)msg->param.dir.path);
	if ((msg->param.dir.path[(pathlen - 1)] == '\\') || (msg->param.dir.path[(pathlen - 1)] == '/')){
		/* �p�X�̏I�[���Z�p���[�^�̏ꍇ�̓G���[	*/
		result = FIO_RESULT_ERR_NO_PATH;
		OUTLOG(OutputLog("[FIO]MakeDir path sepa ERR!!\n");)
		goto PROC_END;
	}

	/* �f�B���N�g���K�w�����擾 */
	hierarchy_num = fio_get_dir_hierarchy_num(msg->param.dir.path);
	if (hierarchy_num == 0) {
		result = FIO_RESULT_ERR_NO_PATH;
		OUTLOG(OutputLog("[FIO]MakeDir not dir ERR!!\n");)
		goto PROC_END;
	}

	/* �K�w�����̃f�B���N�g�����쐬���� */
	for (i = 0; i < hierarchy_num; i++) {
		if (fio_get_dir_path(msg->param.dir.path, (i + 1), S_fio_dir_path_work) != N_OK) {
			result = FIO_RESULT_ERR_FATAL;
			OUTLOG(OutputLog("[FIO]MakeDir get path ERR!!\n");)
			goto PROC_END;
		}

		/* �f�B���N�g���쐬	*/
		result_fs = FileSystem_MakeDir(S_fio_dir_path_work);
		if ((result_fs != FS_RESULT_OK) &&
			(result_fs != FS_RESULT_ERR_EXIST)) {
			/* �f�B���N�g���쐬���s	*/
			break;
		}
	}
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
/*		OUTLOG(OutputLog("[FIO]MakeDir FsMakeDir ERR!! fs_res:%d\n", result_fs);) */
		goto PROC_END;
	}

	result = FIO_RESULT_OK;		/* ����I��	*/

PROC_END:

	/* �R�[���o�b�N�ʒm */
	if (msg->param.dir.cbfnc != NULL) {
		retinfo.result = result;
		fio_make_dir_reqinfo(msg, &S_fio_dir_cb_reqinfo_work);

		msg->param.dir.cbfnc(&S_fio_dir_cb_reqinfo_work, &retinfo);
	}

	return result;

}

/****************************************************************************/
/* �֐�����		:	fio_get_dir_hierarchy_num								*/
/* �@�\�̐���	:	�f�B���N�g���K�w���擾									*/
/* ����			:	[I/ ]path		�p�X									*/
/* �߂�l		: 	�f�B���N�g���K�w��										*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	2017/01/20												*/
/* ���l			:	path�̓h���C�u������n�܂镶�����w�肷�邱�ƁB			*/
/*					�h���C�u���̎��̃f�B���N�g����1�K�w�ڂɂȂ�B			*/
/****************************************************************************/
static UI_32 fio_get_dir_hierarchy_num(const UI_8 *path)
{

	UI_32	i;
	UI_32	pathlen;
	UI_32	hcnt;

	if (path == NULL) {
		return 0;
	}

	pathlen = strlen((const CH_8*)path);
	if (pathlen == 0) {
		return 0;
	}

	hcnt = 0;
	for (i = 0; i < pathlen; i++) {
		if ((path[i] == '\\') || (path[i] == '/')) {
			hcnt++;
		}
	}
	if ((path[(pathlen - 1)] == '\\') || (path[(pathlen - 1)] == '/')) {
		/* �Ō�̕������Z�p���[�^�̏ꍇ �K�w����-1����	*/
		hcnt--;
	}

	return hcnt;

}

/****************************************************************************/
/* �֐�����		:	fio_get_dir_path										*/
/* �@�\�̐���	:	�f�B���N�g���p�X�擾									*/
/* ����			:	[I/ ]path_src	���̓p�X								*/
/*					[I/ ]hnum		�K�w��									*/
/*					[ /O]path_dst	�w��K�w�̃p�X							*/
/* �߂�l		: 	N_OK	����											*/
/* 				: 	N_NG	�ُ�											*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	2017/01/20												*/
/* ���l			:	path�̓h���C�u������n�܂镶�����w�肷�邱�ƁB			*/
/*					�h���C�u���̎��̃f�B���N�g����1�K�w�ڂɂȂ�B			*/
/****************************************************************************/
static SI_8 fio_get_dir_path(const UI_8 *path_src, UI_32 hnum, UI_8 *path_dst)
{

	UI_32	i;
	UI_32	pathlen;
	UI_32	hcnt;

	if ((path_src == NULL) || (path_dst == NULL)) {
		return N_NG;
	}

	pathlen = strlen((const CH_8*)path_src);
	if (pathlen == 0) {
		return N_NG;
	}

	hcnt = 0;
	for (i = 0; i < pathlen; i++) {
		if ((path_src[i] == '\\') || (path_src[i] == '/')){
			if (hcnt == hnum) {
				break;
			}
			hcnt++;
		}
		path_dst[i] = path_src[i];
	}
	path_dst[i] = '\0';

	if (hcnt != hnum) {
		return N_NG;
	}

	return N_OK;

}

/****************************************************************************/
/* �֐�����		:	fio_cnv_res_fs2fio										*/
/* �@�\�̐���	:	�������ʕϊ�(FAT����->FileIO����)						*/
/* ����			:	[I/ ]result_fs	FAT�̏�������							*/
/*					[ /O]result_fio	FileIO�̏�������						*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/08/18												*/
/* ���l			:															*/
/****************************************************************************/
static void fio_cnv_res_fs2fio(
	E_FS_RESULT		result_fs,
	E_FIO_RESULT	*result_fio
)
{

	switch (result_fs) {
	case FS_RESULT_OK:
		*result_fio = FIO_RESULT_OK;
		break;

	case FS_RESULT_ERR_NOT_READY:
		*result_fio = FIO_RESULT_ERR_DISK_NOT_READY;
		break;

	case FS_RESULT_ERR_INVALID_DRIVE:
		*result_fio = FIO_RESULT_ERR_INVALID_DRIVE;
		break;

	case FS_RESULT_ERR_NO_FILE:
		*result_fio = FIO_RESULT_ERR_NO_FILE;
		break;

	case FS_RESULT_ERR_NO_PATH:
		*result_fio = FIO_RESULT_ERR_NO_PATH;
		break;

	case FS_RESULT_ERR_EXIST:
		*result_fio = FIO_RESULT_ERR_EXIST;
		break;

	default:
		*result_fio = FIO_RESULT_ERR_FATAL;
		break;
	}
	return;

}

/****************************************************************************/
/* �֐�����		:	fio_mount_ctrl											*/
/* �@�\�̐���	:	�t�@�C���V�X�e���̃}�E���g/�A���}�E���g����				*/
/* ����			:	�Ȃ�													*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/08/18												*/
/* ���l			:															*/
/****************************************************************************/
static void fio_mount_ctrl(void)
{

	E_SYS_CTRL_STATUS	SysStatus;

	/* �V�X�e�������Ԏ擾 */
	SysStatus = System_control_GetStatus();

	switch (SysStatus) {
	case SYS_OPERATION_WAIT_ACC_ON:			/**< ACC ON�҂�				*/
	case SYS_OPERATION_PRE_ACC_OFF:			/**< ACC OFF�ڍs			*/
	case SYS_OPERATION_ACC_OFF:				/**< ACC OFF				*/
		/* �������Ȃ� */
		break;

	default:
		fio_mount_ctrl_sd();
		break;
	}

	return;

}

/****************************************************************************/
/* �֐�����		:	fio_mount_ctrl_sd										*/
/* �@�\�̐���	:	SD�̃}�E���g/�A���}�E���g����							*/
/* ����			:	�Ȃ�													*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/08/18												*/
/* ���l			:															*/
/****************************************************************************/
static void fio_mount_ctrl_sd(void)
{

	E_FS_RESULT	result_fs;
	UI_16		sectPerClst;
	UI_32		freeClst;
	UI_8		i;

	/* SD���������`�F�b�N(�����ꂽ���̏������Ɏ��s����) */
	if (S_fio_sd_detect_out == N_ON) {		/* SD OUT�����o */
		S_fio_sd_detect_out = N_OFF;
		/* SD�h���C�u���A���}�E���g���� */
		(void)FileSystem_Unmount((const UI_8 *)FS_DRV_PATH_SD);
		S_fio_drv_info[FIO_DRV_ID_SD].mount = N_FALSE;		/* �A���}�E���g���͍ŏ��Ƀ}�E���g��Ԃ��X�V���� */
		S_fio_drv_info[FIO_DRV_ID_SD].free_sect_num = 0;
	}

	if (S_fio_sd_detect_in == N_ON) {		/* SD IN�����o */
		S_fio_sd_detect_in = N_OFF;

		/* SD�h���C�u���}�E���g����(SD�������̏ꍇ�̓��g���C����) */
		for (i = 0; i < FIO_MOUNT_RETRY_NUM; i++) {
			result_fs = FileSystem_Mount((const UI_8 *)FS_DRV_PATH_SD);
			if (result_fs != FS_RESULT_ERR_NOT_READY) {
				break;
			}
		}
		if (result_fs == FS_RESULT_OK) {	/* �}�E���g���� */

			/* �󂫃Z�N�^���擾 */
			result_fs = FileSystem_GetFree((const UI_8 *)FS_DRV_PATH_SD, &freeClst, &sectPerClst);
			if (result_fs == FS_RESULT_OK) {
				if (freeClst == FS_FREE_CLUST_NUM_UNKNOWN) {
					S_fio_drv_info[FIO_DRV_ID_SD].free_sect_num = FIO_FREE_SECT_NUM_UNKNOWN;
				}
				else {
					S_fio_drv_info[FIO_DRV_ID_SD].free_sect_num = freeClst * sectPerClst;
				}
			}
			else {
				S_fio_drv_info[FIO_DRV_ID_SD].free_sect_num = FIO_FREE_SECT_NUM_UNKNOWN;
			}

			S_fio_drv_info[FIO_DRV_ID_SD].mount = N_TRUE;		/* �}�E���g���͍Ō�Ƀ}�E���g��Ԃ��X�V���� */
		}
		else {								/* �}�E���g���s */
			/* SD�h���C�u���A���}�E���g���� */
			(void)FileSystem_Unmount((const UI_8 *)FS_DRV_PATH_SD);
			S_fio_drv_info[FIO_DRV_ID_SD].mount = N_FALSE;		/* �A���}�E���g���͍ŏ��Ƀ}�E���g��Ԃ��X�V���� */
			S_fio_drv_info[FIO_DRV_ID_SD].free_sect_num = 0;
		}
	}

	S_fio_drv_info[FIO_DRV_ID_SD].mountExecuted = N_TRUE;	/* �Ō�Ƀ}�E���g���s��Ԃ�ݒ肷�� */

	return;

}

/****************************************************************************/
/* �֐�����		:	fio_path2drvid											*/
/* �@�\�̐���	:	�p�X������h���C�uID���擾								*/
/* ����			:	[I/ ]path	�h���C�u�̃p�X(�p�X�̌��Ƀt�@�C���������Ă���)	*/
/* �߂�l		: 	����	�h���C�uID�iFIO_DRV_ID_xxx�j					*/
/*				    �ُ�	FIO_DRV_ID_INVALID								*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
static E_FIO_DRV_ID fio_path2drvid( const UI_8 *path )
{

	SI_32	pathlen;
	SI_32	drvlen;
	UI_8	i;

	pathlen = (SI_32)strlen((const CH_8*)path);

	for (i = 0; i < FIO_DRV_ID_NUM; i++) {
		drvlen = (SI_32)strlen((const CH_8*)S_fio_drv_info[i].drvname);
		if (drvlen <= pathlen) {
			if (memcmp(path, S_fio_drv_info[i].drvname, (size_t)drvlen) == 0) {
				return (E_FIO_DRV_ID)i;
			}
		}
	}

	return FIO_DRV_ID_INVALID;

}

/****************************************************************************/
/* �֐�����		:	fio_sdc_dtct_cb											*/
/* �@�\�̐���	:	SD�J�[�hIN/OUT���o�R�[���o�b�N							*/
/* ����			:	[I/ ]portlev	�|�[�g���x��							*/
/*				:				IO_PI_LEV_ACTIVE_SD_DTCT : SD�J�[�hIN		*/
/*				:				IO_PI_LEV_NONACT_SD_DTCT : SD�J�[�hOUT		*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/08/18												*/
/* ���l			:															*/
/****************************************************************************/
static void fio_sdc_dtct_cb( UI_8 portlev )
{

	if (portlev == IO_PI_LEV_ACTIVE_SD_DTCT) {	/* SD�J�[�hIN		*/
		S_fio_sd_detect_in = N_ON;
	}
	else {										/* SD�J�[�hOUT		*/
		S_fio_sd_detect_out = N_ON;
	}
	return;

}

/****************************************************************************/
/* �֐�����		:	fio_strcpy												*/
/* �@�\�̐���	:	������R�s�[											*/
/* ����			:	[ /O]dstBuff		�R�s�[��o�b�t�@					*/
/*				:	[I/ ]dstBuffSize	�R�s�[��o�b�t�@�T�C�Y				*/
/*				:	[I/ ]srcBuff		�R�s�[���o�b�t�@					*/
/* �߂�l		: 	�Ȃ�													*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	16/08/18												*/
/* ���l			:															*/
/****************************************************************************/
static void fio_strcpy(
	UI_8		*dstBuff,
	UI_32		dstBuffSize,
	const UI_8	*srcBuff
)
{

	(void)strncpy((CH_8*)dstBuff, (const CH_8*)srcBuff, (size_t)(dstBuffSize - 1));
	dstBuff[dstBuffSize - 1] = '\0';
	return;

}




