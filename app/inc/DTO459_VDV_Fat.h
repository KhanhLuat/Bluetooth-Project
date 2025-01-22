/****************************************************************************/
/* Customer			: Customer_XXXX											*/
/* Model(Thema No.)	: Model_XXXX											*/
/*--------------------------------------------------------------------------*/
/* CPU				: FR81S													*/
/* Date				: 16/10/05												*/
/*--------------------------------------------------------------------------*/
/* Programmed by	: (OFT)Toguch											*/
/* Copyrights Nippon Seiki Co.,Ltd											*/
/*--------------------------------------------------------------------------*/
/* update by		:														*/
/* date				:														*/
/****************************************************************************/
/****************************************************************************/
/* file DTO459_VDV_Fat.h													*/
/* FAT�t�@�C���V�X�e��														*/
/****************************************************************************/

#ifndef _DTO459_VDV_FAT_H
#define _DTO459_VDV_FAT_H


#include "type.h"
#include "DTO459_VDV_Disk_Drv.h"

/********************************************************************/
/*	�}�N����`														*/
/********************************************************************/
/* �h���C�u���A��؂蕶�� */
#define	FS_DRV_NAME_SD				"SD"			/* SD�h���C�u�� */
#define	FS_SEP_DRV					":"				/* �h���C�u�̋�؂蕶�� */
#define	FS_SEP_DIR					"\\"			/* �f�B���N�g���̋�؂蕶�� */

/* �p�X�� */
#define	FS_DRV_PATH_SD		FS_DRV_NAME_SD FS_SEP_DRV FS_SEP_DIR		/* SD�h���C�u�p�X */

/* �t�@�C������ */
#define	FS_FILENAME_LEN_MAX			(32)
/* �t�@�C�������͈ȉ��͈̔͂Őݒ肷�邱�� */
/* 12 <= FS_FILENAME_LEN_MAX <= 255 */
/* VDV�ŃA�N�Z�X����t�@�C�����i�p�X�܂ށj��32�o�C�g�ȏ�ƂȂ�ꍇ�͂����𑝂₷���� */
/* �V�X�e���Ŏg�p���郁�����i�X�^�b�N�j�g�p�ʂɉe�����邽�߃������g�p�ʂ��m�F���邱�� */
/* (FS_FILENAME_LEN_MAX�ȏ�̃t�@�C�����̃t�@�C�����i�[����Ă��Ă�VDV�ŃA�N�Z�X���Ȃ��ꍇ�͓���ɉe���͂Ȃ�) */

/* �󂫃N���X�^���iFileSystem_GetFree�Ŏ擾����l�j */
#define	FS_FREE_CLUST_NUM_UNKNOWN	(0xFFFFFFFFUL)	/* �󂫃N���X�^���s�� */

/* �t�@�C���A�N�Z�X���[�h (FileSystem_Open�֐��Ŏw�肷��) */
/* (DTO459_VDVFat.c�ɂ������p��`����B�����p��`�Ƃ��Ԃ�Ȃ�����) */
#define	FS_FA_MODE_OPEN_EXISTING	(0x00)			/* �����̃t�@�C�����J���B�t�@�C���������Ƃ��̓G���[�B */
#define	FS_FA_MODE_READ				(0x01)			/* �ǂݏo�����[�h�ŊJ���B�ǂݏ�������ꍇ��FS_FA_MODE_WRITE�Ƌ��Ɏw�肷��B */
#define	FS_FA_MODE_WRITE			(0x02)			/* �������݃��[�h�ŊJ���B�ǂݏ�������ꍇ��FS_FA_MODE_READ�Ƌ��Ɏw�肷��B */
#define	FS_FA_MODE_CREATE_NEW		(0x04)			/* �t�@�C�����쐬����B�����̃t�@�C��������ꍇ�͖߂�lFS_RESULT_ERR_EXIST���Ԃ�B */
#define	FS_FA_MODE_CREATE_ALWAYS	(0x08)			/* �t�@�C�����쐬����B�����̃t�@�C��������ꍇ�̓T�C�Y��0�ɂ��ĊJ���B */
#define	FS_FA_MODE_OPEN_ALWAYS		(0x10)			/* �����̃t�@�C�����J���B�t�@�C���������ꍇ�̓t�@�C�����쐬����B */
#define	FS_FA_MODE_OPEN_APPEND		(0x30)			/* FS_FA_MODE_OPEN_ALWAYS�Ɠ����Ń��[�h/���C�g�|�C���^�̓t�@�C���̍Ō���ɃZ�b�g�����B */
													/* FS_FA_MODE_OPEN_ALWAYS��FS_FA_MODE_SEEKEND(�����p��`)�̑g�ݍ��킹 */

/* �������� */
typedef enum {
	FS_RESULT_OK = 0,								/* (0)  ����I�� */
	FS_RESULT_ERR_DISK,								/* (1)  �t�@�C���V�X�e���̉��ʃ��C��(�h���C�u�w)�ŉ񕜕s�\�ȃG���[���������� */
	FS_RESULT_ERR_INTERNAL,							/* (2)  �����G���[ */
	FS_RESULT_ERR_NOT_READY,						/* (3)  �t�@�C���V�X�e���̉��ʃ��C��(�h���C�u�w)�ŃA�N�Z�X�s�\�i���f�B�A���Z�b�g����Ă��Ȃ��Ȃǁj */
	FS_RESULT_ERR_NO_FILE,							/* (4)  �w�肳�ꂽ�t�@�C����������Ȃ� */
	FS_RESULT_ERR_NO_PATH,							/* (5)  �w�肳�ꂽ�p�X��������Ȃ� */
	FS_RESULT_ERR_INVALID_NAME,						/* (6)  �s���ȃp�X�����w�肳�ꂽ */
	FS_RESULT_ERR_DENIED,							/* (7)  �A�N�Z�X�����ۂ��ꂽ */
	FS_RESULT_ERR_EXIST,							/* (8)  �V�����쐬���悤�Ƃ����I�u�W�F�N�g�Ɠ������O�̃I�u�W�F�N�g�����ɑ��݂��� */
	FS_RESULT_ERR_INVALID_OBJECT,					/* (9)  �w�肳�ꂽ�t�@�C�� �I�u�W�F�N�g��f�B���N�g�� �I�u�W�F�N�g������ */
	FS_RESULT_ERR_WRITE_PROTECTED,					/* (10) �����h���C�u���������݋֎~��� */
	FS_RESULT_ERR_INVALID_DRIVE,					/* (11) �p�X�����Ɏw�肳�ꂽ�h���C�u������ */
	FS_RESULT_ERR_NO_FILESYSTEM,					/* (12) �����h���C�u��ɗL����FAT�{�����[�����Ȃ� */
	FS_RESULT_ERR_LOCKED,							/* (13) ���d�A�N�Z�X�r������ɂ��A�N�Z�X�����ۂ��ꂽ */
	FS_RESULT_ERR_TOO_MANY_OPEN_FILES,				/* (14) �����I�[�v���\�ȃt�@�C�������z���ăt�@�C�����J�����Ƃ��� */
	FS_RESULT_ERR_INVALID_PARAM						/* (15) ���̓p�����^�s�� */
} E_FS_RESULT;

/********************************************************************/
/*	�\���̒�`														*/
/********************************************************************/
/* �t�@�C����� */
typedef struct {
	UI_32				fsize;							/* �t�@�C���T�C�Y[byte] */
	UI_16				fdate;							/* �t�@�C���X�V�� */
	UI_16				ftime;							/* �t�@�C���X�V���� */
	UI_8				fattrib;						/* �t�@�C������ */
	UI_8				altname[13];					/* 8.3�`���̃t�@�C���� */
	UI_8				fname[FS_FILENAME_LEN_MAX + 1];	/* �t�@�C���� */
} FS_FILE_INFO;


/********************************************************************/
/*	�O���֐�														*/
/********************************************************************/
/* ������ */
void	FileSystem_Init(void);

/* �{�����[���Ǘ� */
E_FS_RESULT	FileSystem_Mount(const UI_8 *path);
E_FS_RESULT	FileSystem_Unmount(const UI_8 *path);
E_FS_RESULT FileSystem_GetFree(const UI_8 *path, UI_32 *nclst, UI_16 *sectPerClst);

/*�t�@�C���A�N�Z�X */
E_FS_RESULT FileSystem_Open(const UI_8 *path, UI_8 mode, UI_8 *fileID);
E_FS_RESULT FileSystem_Close(UI_8 fileID);
E_FS_RESULT FileSystem_Read(UI_8 fileID, UI_8 *buff, UI_32 btr, UI_32 *br);
E_FS_RESULT FileSystem_Write(UI_8 fileID, const UI_8 *buff, UI_32 btw, UI_32 *bw);
E_FS_RESULT FileSystem_Seek(UI_8 fileID, UI_32 ofs);
E_FS_RESULT FileSystem_Tell(UI_8 fileID, UI_32 *curOfs);
E_FS_RESULT FileSystem_Size(UI_8 fileID, UI_32 *size);
E_FS_RESULT FileSystem_Sync(UI_8 fileID);
E_FS_RESULT FileSystem_Stat(const UI_8 *path, FS_FILE_INFO *finfo);

/* �f�B���N�g���A�N�Z�X */
E_FS_RESULT FileSystem_OpenDir(const UI_8 *path, UI_8 *dirID);
E_FS_RESULT FileSystem_CloseDir(UI_8 dirID);
E_FS_RESULT FileSystem_ReadDir(UI_8 dirID, FS_FILE_INFO *finfo);
E_FS_RESULT FileSystem_MakeDir(const UI_8 *path);
E_FS_RESULT FileSystem_FindFirst(UI_8 *dirID, FS_FILE_INFO *finfo, const UI_8 *path, const UI_8 *pattern);
E_FS_RESULT FileSystem_FindNext(UI_8 dirID, FS_FILE_INFO *finfo);

#endif /* _DTO459_VDV_FAT_H */
