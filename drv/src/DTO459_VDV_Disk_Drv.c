/********************************************************************************/
/* Customer			: Customer_XXXX												*/
/* Model(Thema No.)	: Model_XXXX												*/
/*------------------------------------------------------------------------------*/
/* CPU				: FR81S														*/
/* Date				: 16/08/18													*/
/*------------------------------------------------------------------------------*/
/* Programmed by	:(OFT)Toguchi												*/
/* Copyrights Nippon Seiki Co.,Ltd												*/
/*------------------------------------------------------------------------------*/
/* update by		:															*/
/* date				:															*/
/********************************************************************************/
/********************************************************************************/
/* file DTO459_VDV_Disk_Drv.c													*/
/* Disk�h���C�o																	*/
/********************************************************************************/
#include "SSFTSTD_Macro.h"
#include "system.h"
#include "timer_ctrl.h"
#include "DTO459_VDV_Disk_Drv.h"
#include "DTO459_VDV_Sd_Drv.h"
#include "dbglog.h"


/********************************************************************************/
/*	�萔��`																	*/
/********************************************************************************/
#define DISKDRV_SD_SECTOR_SIZE		(512U)				/* SD�h���C�u�̃Z�N�^�T�C�Y(byte)	*/
#define DISKDRV_SD_RETRY_NUM		(2)					/* SD�h���C�u�A�N�Z�X�̃��g���C��	*/
#define DISKDRV_SD_INIT_WAIT_TIME	(10)				/* SD�h���C�u�������J�n�҂����� */
#define DISKDRV_SD_INIT_RETRY_TIME	(10)				/* SD�h���C�u���������g���C���� */
#define DISKDRV_SD_INIT_RETRY_NUM	(5)					/* SD�h���C�u���������g���C�� */

/********************************************************************************/
/*	�O���֐�																	*/
/********************************************************************************/
/********************************************************************************/
/* �֐�����		: DiskDrv_get_status											*/
/* �@�\�̐���	: �f�B�X�N��Ԏ擾												*/
/* ����			: [I/ ]:E_DISKDRV_DRIVE_NO	drvno	�h���C�u�ԍ�				*/
/* �߂�l		: void															*/
/* �쐬��		: (OFT)Toguchi													*/
/* �쐬��		: 16/08/18														*/
/* ���l			: 																*/
/********************************************************************************/
DISKDRV_STATUS DiskDrv_get_status (
	E_DISKDRV_DRIVE_NO drvno
)
{
	DISKDRV_STATUS	stat;

	if (drvno == DISKDRV_NO_SD) {
		T_CARD_INFO		SdCardInfo;
		E_SDDRRET		sdRet;
		sdRet = SDCardDrv_GetCardInfo(&SdCardInfo);
		if (sdRet == SDCARDDRV_NOERROR) {
			if (SdCardInfo.ulTotal_Secotors == 0UL) {
				stat = DISKDRV_STATUS_NOINIT;
			}
			else {
				if (SdCardInfo.uProtectState == D_ON) {
					stat = DISKDRV_STATUS_PROTECT;
				}
				else {
					stat = DISKDRV_STATUS_NORMAL;
				}
			}
		}
		else {
			stat = (DISKDRV_STATUS_NOINIT | DISKDRV_STATUS_NODISK);
		}
	}
	else {
		stat = DISKDRV_STATUS_NOINIT;
	}
	return stat;
}

/********************************************************************************/
/* �֐�����		: DiskDrv_init													*/
/* �@�\�̐���	: �f�B�X�N������												*/
/* ����			: [I/ ]:E_DISKDRV_DRIVE_NO	drvno	�h���C�u�ԍ�				*/
/* �߂�l		: void															*/
/* �쐬��		: (OFT)Toguchi													*/
/* �쐬��		: 16/08/18														*/
/* ���l			: 																*/
/********************************************************************************/
DISKDRV_STATUS DiskDrv_init(
	E_DISKDRV_DRIVE_NO drvno
)
{
	DISKDRV_STATUS	stat;

	if (drvno == DISKDRV_NO_SD) {
		E_SDDRRET		sdRet;
		UI_8			i;
		UI_32			tm;

		/* SD�������J�n�҂� */
		tm = Timer_ctrl_GetTime1ms();
		while (Timer_ctrl_TimeCompare1ms( tm, DISKDRV_SD_INIT_WAIT_TIME ) == N_NG) {
		}
		for (i = 0; i < DISKDRV_SD_RETRY_NUM; i++) {
			sdRet = SDCardDrv_SdInit();
			if ((sdRet == SDCARDDRV_NOERROR)) {
				stat = DiskDrv_get_status(drvno);
				break;
			}
			else if ((sdRet == SDCARDDRV_NOCARD)) {
				stat = DISKDRV_STATUS_NOINIT | DISKDRV_STATUS_NODISK;
				OUTLOG(OutputLog("[DISK]Init ERR!! no card\n");)
				break;
			}
			else {
				stat = DISKDRV_STATUS_NOINIT;
				OUTLOG(OutputLog("[DISK]Init ERR!! sd_res:%d\n", sdRet);)
				if (i < (DISKDRV_SD_RETRY_NUM - 1)) {
					/* ���g���C�҂� */
					tm = Timer_ctrl_GetTime1ms();
					while (Timer_ctrl_TimeCompare1ms( tm, DISKDRV_SD_INIT_RETRY_TIME ) == N_NG) {
					}
				}
			}
		}
	}
	else {
		stat = DISKDRV_STATUS_NOINIT;
	}
	return stat;
}

/********************************************************************************/
/* �֐�����		: DiskDrv_read													*/
/* �@�\�̐���	: �f�B�X�N���[�h												*/
/* ����			: [I/ ]:E_DISKDRV_DRIVE_NO	drvno	�h���C�u�ԍ�				*/
/*				: [ /O]:UI_8*				buff	�ǂݍ��݃f�[�^�o�b�t�@		*/
/*				: [I/ ]:UI_32				sector	�ǂݍ��݃Z�N�^�ʒu			*/
/*				: [I/ ]:UI_32				count	�ǂݍ��݃Z�N�^��			*/
/* �߂�l		: E_DISKDRV_RESULT		DISKDRV_RES_xxx							*/
/* �쐬��		: (OFT)Toguchi													*/
/* �쐬��		: 16/08/18														*/
/* ���l			: 																*/
/********************************************************************************/
E_DISKDRV_RESULT DiskDrv_read(
	E_DISKDRV_DRIVE_NO drvno,
	UI_8 *buff,
	UI_32 sector,
	UI_32 count
)
{
	E_DISKDRV_RESULT result;

	if (drvno == DISKDRV_NO_SD) {
		E_SDDRRET	sdRet;
		UI_8		i;
		for (i = 0; i < DISKDRV_SD_RETRY_NUM; i++) {
			sdRet = SDCardDrv_Read(buff, sector, count);
			if (sdRet == SDCARDDRV_NOERROR) {
				result = DISKDRV_RES_OK;
				break;
			}
			else if (sdRet == SDCARDDRV_NOCARD) {
				result = DISKDRV_RES_NOTRDY;
				OUTLOG(OutputLog("[DISK]Read ERR!! no card\n");)
				break;
			}
			else {
				result = DISKDRV_RES_ERROR;
				OUTLOG(OutputLog("[DISK]Read ERR!! sd_res:%d\n", sdRet);)
			}
		}
	}
	else {
		result = DISKDRV_RES_PARERR;
	}

	return result;
}

/********************************************************************************/
/* �֐�����		: DiskDrv_write													*/
/* �@�\�̐���	: �f�B�X�N���C�g												*/
/* ����			: [I/ ]:E_DISKDRV_DRIVE_NO	drvno	�h���C�u�ԍ�				*/
/*				: [I/ ]:UI_8*				buff	�������݃f�[�^�o�b�t�@		*/
/*				: [I/ ]:UI_32				sector	�������݃Z�N�^�ʒu			*/
/*				: [I/ ]:UI_32				count	�������݃Z�N�^��			*/
/* �߂�l		: E_DISKDRV_RESULT		DISKDRV_RES_xxx							*/
/* �쐬��		: (OFT)Toguchi													*/
/* �쐬��		: 16/08/18														*/
/* ���l			: 																*/
/********************************************************************************/
E_DISKDRV_RESULT DiskDrv_write(
	E_DISKDRV_DRIVE_NO drvno,
	const UI_8 *buff,
	UI_32 sector,
	UI_32 count
)
{
	E_DISKDRV_RESULT result = DISKDRV_RES_OK;

	if (drvno == DISKDRV_NO_SD) {
		E_SDDRRET	sdRet;
		UI_8		i;
		for (i = 0; i < DISKDRV_SD_RETRY_NUM; i++) {
			sdRet = SDCardDrv_Write(buff, sector, count);
			if (sdRet == SDCARDDRV_NOERROR) {
				result = DISKDRV_RES_OK;
				break;
			}
			else if (sdRet == SDCARDDRV_NOCARD) {
				result = DISKDRV_RES_NOTRDY;
				OUTLOG(OutputLog("[DISK]Write ERR!! no card\n");)
				break;
			}
			else if (sdRet == SDCARDDRV_WRITEPROHIB) {
				result = DISKDRV_RES_WRPRT;
/*					OUTLOG(OutputLog("[DISK]Write ERR!! write protect\n");) */
				break;
			}
			else {
				result = DISKDRV_RES_ERROR;
				OUTLOG(OutputLog("[DISK]Write ERR!! sd_res:%d\n", sdRet);)
			}
		}
	}
	else {
		result = DISKDRV_RES_PARERR;
	}

	return result;
}

/********************************************************************************/
/* �֐�����		: DiskDrv_ioctl													*/
/* �@�\�̐���	: �f�B�X�N����													*/
/* ����			: [I/ ]:E_DISKDRV_DRIVE_NO	drvno	�h���C�u�ԍ�				*/
/*				: [I/ ]:UI_8				cmd		����R�}���h				*/
/*				: [I/O]:void*				buff	����p�f�[�^�o�b�t�@		*/
/* �߂�l		: E_DISKDRV_RESULT		DISKDRV_RES_xxx							*/
/* �쐬��		: (OFT)Toguchi													*/
/* �쐬��		: 16/08/18														*/
/* ���l			: 	�R�}���h:DISKDRV_CTRL_SYNC									*/
/*						���C�g�p�L���b�V���Ȃǂ������Ă���ꍇ�͏������܂��	*/
/*						���Ȃ��f�[�^���������ށB								*/
/*				 	�R�}���h:GET_SECTOR_SIZE									*/
/*						UI_16�^�̕ϐ�buff�ɃZ�N�^�T�C�Y��Ԃ��B					*/
/*						�L���l��512�A1024�A2048�A4096							*/
/********************************************************************************/
E_DISKDRV_RESULT DiskDrv_ioctl(
	E_DISKDRV_DRIVE_NO drvno,
	UI_8 cmd,
	void *buff
)
{
	E_DISKDRV_RESULT	result;

	if (drvno == DISKDRV_NO_SD) {
		switch (cmd) {
		case DISKDRV_CTRL_SYNC:
			result = DISKDRV_RES_OK;	/* SD�ɃL���b�V���͂Ȃ����߂Ȃɂ����Ȃ� */
			break;

		case DISKDRV_GET_SECTOR_SIZE:
			*(UI_16*)buff = DISKDRV_SD_SECTOR_SIZE;
			result = DISKDRV_RES_OK;
			break;

		default:
			result = DISKDRV_RES_PARERR;
			break;
		}
	}
	else {
		result = DISKDRV_RES_PARERR;
	}

	return result;
}
