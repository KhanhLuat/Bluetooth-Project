/********************************************************************/
/* Customer			: Customer_XXXX									*/
/* Model(Thema No.)	: Model_XXXX									*/
/*------------------------------------------------------------------*/
/* CPU				: FR81S											*/
/* Date				: 16/08/18										*/
/*------------------------------------------------------------------*/
/* Programmed by	: (OFT)Toguch									*/
/* Copyrights Nippon Seiki Co.,Ltd									*/
/*------------------------------------------------------------------*/
/* update by		:												*/
/* date				:												*/
/********************************************************************/
/********************************************************************/
/* file DTO459_VDV_File_io.h										*/
/* �t�@�C��IO														*/
/********************************************************************/
#ifndef	DTO459_VDV_FILE_IO_H
#define	DTO459_VDV_FILE_IO_H

/********************************************************************/
/*	�C���N���[�h�t�@�C��											*/
/********************************************************************/
#include "type.h"
#include "DTO459_VDV_Fat.h"

/********************************************************************/
/*	�}�N����`														*/
/********************************************************************/
/* �p�X���� */ 
#define FIO_PATH_LEN_MAX			FS_FILENAME_LEN_MAX

/* Write�̃I�t�Z�b�g�w�� */
#define FIO_OFS_EOF					(0xFFFFFFFFUL)	/* �t�@�C���I�[ */

/* �󂫃Z�N�^���iFileIO_Get_FreeSectNum�Ŏ擾����l�j	*/
#define	FIO_FREE_SECT_NUM_UNKNOWN	(0xFFFFFFFFUL)	/* �󂫃Z�N�^���s��	*/

/* �������ݎ�� */ 
typedef enum e_fio_write_type
{
	FIO_WRITE_TYPE_FILE_OPEN	= 0,		/* �����̃t�@�C�����J���ď�������				*/
											/* �����̃t�@�C���������ꍇ�͐V�K�쐬����		*/
	FIO_WRITE_TYPE_FILE_CREATE				/* �V�K�t�@�C�����쐬���ď�������				*/
											/* �����̃t�@�C��������ꍇ�͔j�����ď�������	*/
} E_FIO_WRITE_TYPE;

/* �v����� */ 
typedef enum e_fio_req_type
{
	FIO_REQ_TYPE_READ	= 0,				/* ���[�h�v��				*/
	FIO_REQ_TYPE_WRITE						/* ���C�g�v��				*/
} E_FIO_REQ_TYPE;

/* �������� */
typedef enum e_fio_result {
	FIO_RESULT_OK = 0,						/* ����I��						*/
	FIO_RESULT_ERR_FATAL,					/* �ُ�I��						*/
	FIO_RESULT_ERR_PARAM,					/* ���̓p�����^�s��				*/
	FIO_RESULT_ERR_DISK_NOT_READY,			/* �f�B�X�N�g�p�s��				*/
	FIO_RESULT_ERR_INVALID_DRIVE,			/* �h���C�u���s��				*/
	FIO_RESULT_ERR_NO_FILE,					/* �w�肳�ꂽ�t�@�C�����Ȃ�		*/
	FIO_RESULT_ERR_NO_PATH,					/* �w�肳�ꂽ�p�X���Ȃ�			*/
	FIO_RESULT_ERR_SNDMSG,					/* ���b�Z�[�W���M�G���[			*/
	FIO_RESULT_ERR_EXIST					/* �����̃I�u�W�F�N�g�����݂���	*/
} E_FIO_RESULT;

/********************************************************************/
/*	�\���̒�`														*/
/********************************************************************/
/* �v����� */
typedef struct {
	UI_32				reqno;						/* �v��No								*/
	E_FIO_REQ_TYPE		reqtype;					/* �v�����								*/
	UI_8				path[FIO_PATH_LEN_MAX+1];	/* �t�@�C���p�X							*/
	UI_8				*buff;						/* �o�b�t�@								*/
	UI_32				ofs;						/* �t�@�C���擪����̃I�t�Z�b�g(byte)	*/
	UI_32				bytes;						/* �T�C�Y(byte)							*/
} T_FIO_REQ_INFO;

/* �������ʏ�� */
typedef struct {
	E_FIO_RESULT		result;						/* ��������								*/
	UI_32				bytes;						/* �T�C�Y(byte)							*/
													/* reqtype=FIO_REQ_TYPE_READ�̏ꍇ:�ǂݍ��񂾃T�C�Y		*/
													/* reqtype=FIO_REQ_TYPE_WRITE�̏ꍇ:�������񂾃T�C�Y	*/
	UI_32				fsize;						/* �t�@�C���T�C�Y(byte)					*/
} T_FIO_RET_INFO;

/* �v�����i�t�@�C�������p�j */
typedef struct {
	UI_32				reqno;							/* �v��No								*/
	UI_8				path[FIO_PATH_LEN_MAX+1];		/* ��������p�X							*/
	UI_8				filename[FIO_PATH_LEN_MAX+1];	/* �����t�@�C����						*/
} T_FIO_SRCF_REQ_INFO;

/* �������ʏ��i�t�@�C�������p�j */
typedef struct {
	E_FIO_RESULT		result;							/* ��������								*/
	UI_8				filename[FIO_PATH_LEN_MAX+1];	/* ���������t�@�C����					*/
} T_FIO_SRCF_RET_INFO;

/* �v�����i�f�B���N�g������p�j */
typedef struct {
	UI_32				reqno;							/* �v��No								*/
	UI_8				path[FIO_PATH_LEN_MAX+1];		/* ��������p�X							*/
} T_FIO_DIR_REQ_INFO;

/* �������ʏ��i�f�B���N�g������p�j */
typedef struct {
	E_FIO_RESULT		result;							/* ��������								*/
} T_FIO_DIR_RET_INFO;

/* �R�[���o�b�N�֐� */
typedef void (*PFN_FIO_CB_NOTIFY)( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo );
													/* Read/Write�p							*/
typedef B_8 (*PFN_FIO_SRCF_CB_NOTIFY)( const T_FIO_SRCF_REQ_INFO *reqinfo, const T_FIO_SRCF_RET_INFO *retinfo );
													/* �t�@�C�������p						*/
typedef void (*PFN_FIO_DIR_CB_NOTIFY)( const T_FIO_DIR_REQ_INFO *reqinfo, const T_FIO_DIR_RET_INFO *retinfo );
													/* �f�B���N�g������p					*/


/********************************************************************/
/*	�O���֐�														*/
/********************************************************************/
E_FIO_RESULT FileIO_Read(const UI_8 *path, UI_8 *buff, UI_32 ofs, UI_32 bytes, PFN_FIO_CB_NOTIFY cbfnc, UI_32 *reqno);
E_FIO_RESULT FileIO_Write(E_FIO_WRITE_TYPE wtype, const UI_8 *path, const UI_8 *buff, UI_32 ofs, UI_32 bytes, PFN_FIO_CB_NOTIFY cbfnc, UI_32 *reqno);
E_FIO_RESULT FileIO_SearchFile(const UI_8 *path, const UI_8 *filename, PFN_FIO_SRCF_CB_NOTIFY cbfnc, UI_32 *reqno);
E_FIO_RESULT FileIO_MakeDir(const UI_8 *path, PFN_FIO_DIR_CB_NOTIFY cbfnc, UI_32 *reqno);
E_FIO_RESULT FileIO_Get_FreeSectNum(const UI_8 *drvpath, UI_32 *freesect);
B_8 FileIO_Get_MountExecStat(const UI_8 *drvpath);
void FileIO_Init(void);
void FileIO_Main(void);

#endif	/*DTO459_VDV_FILE_IO_H*/
