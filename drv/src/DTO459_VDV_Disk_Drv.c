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
/* Diskドライバ																	*/
/********************************************************************************/
#include "SSFTSTD_Macro.h"
#include "system.h"
#include "timer_ctrl.h"
#include "DTO459_VDV_Disk_Drv.h"
#include "DTO459_VDV_Sd_Drv.h"
#include "dbglog.h"


/********************************************************************************/
/*	定数定義																	*/
/********************************************************************************/
#define DISKDRV_SD_SECTOR_SIZE		(512U)				/* SDドライブのセクタサイズ(byte)	*/
#define DISKDRV_SD_RETRY_NUM		(2)					/* SDドライブアクセスのリトライ回数	*/
#define DISKDRV_SD_INIT_WAIT_TIME	(10)				/* SDドライブ初期化開始待ち時間 */
#define DISKDRV_SD_INIT_RETRY_TIME	(10)				/* SDドライブ初期化リトライ時間 */
#define DISKDRV_SD_INIT_RETRY_NUM	(5)					/* SDドライブ初期化リトライ回数 */

/********************************************************************************/
/*	外部関数																	*/
/********************************************************************************/
/********************************************************************************/
/* 関数名称		: DiskDrv_get_status											*/
/* 機能の説明	: ディスク状態取得												*/
/* 引数			: [I/ ]:E_DISKDRV_DRIVE_NO	drvno	ドライブ番号				*/
/* 戻り値		: void															*/
/* 作成者		: (OFT)Toguchi													*/
/* 作成日		: 16/08/18														*/
/* 備考			: 																*/
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
/* 関数名称		: DiskDrv_init													*/
/* 機能の説明	: ディスク初期化												*/
/* 引数			: [I/ ]:E_DISKDRV_DRIVE_NO	drvno	ドライブ番号				*/
/* 戻り値		: void															*/
/* 作成者		: (OFT)Toguchi													*/
/* 作成日		: 16/08/18														*/
/* 備考			: 																*/
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

		/* SD初期化開始待ち */
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
					/* リトライ待ち */
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
/* 関数名称		: DiskDrv_read													*/
/* 機能の説明	: ディスクリード												*/
/* 引数			: [I/ ]:E_DISKDRV_DRIVE_NO	drvno	ドライブ番号				*/
/*				: [ /O]:UI_8*				buff	読み込みデータバッファ		*/
/*				: [I/ ]:UI_32				sector	読み込みセクタ位置			*/
/*				: [I/ ]:UI_32				count	読み込みセクタ数			*/
/* 戻り値		: E_DISKDRV_RESULT		DISKDRV_RES_xxx							*/
/* 作成者		: (OFT)Toguchi													*/
/* 作成日		: 16/08/18														*/
/* 備考			: 																*/
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
/* 関数名称		: DiskDrv_write													*/
/* 機能の説明	: ディスクライト												*/
/* 引数			: [I/ ]:E_DISKDRV_DRIVE_NO	drvno	ドライブ番号				*/
/*				: [I/ ]:UI_8*				buff	書き込みデータバッファ		*/
/*				: [I/ ]:UI_32				sector	書き込みセクタ位置			*/
/*				: [I/ ]:UI_32				count	書き込みセクタ数			*/
/* 戻り値		: E_DISKDRV_RESULT		DISKDRV_RES_xxx							*/
/* 作成者		: (OFT)Toguchi													*/
/* 作成日		: 16/08/18														*/
/* 備考			: 																*/
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
/* 関数名称		: DiskDrv_ioctl													*/
/* 機能の説明	: ディスク制御													*/
/* 引数			: [I/ ]:E_DISKDRV_DRIVE_NO	drvno	ドライブ番号				*/
/*				: [I/ ]:UI_8				cmd		制御コマンド				*/
/*				: [I/O]:void*				buff	制御用データバッファ		*/
/* 戻り値		: E_DISKDRV_RESULT		DISKDRV_RES_xxx							*/
/* 作成者		: (OFT)Toguchi													*/
/* 作成日		: 16/08/18														*/
/* 備考			: 	コマンド:DISKDRV_CTRL_SYNC									*/
/*						ライト用キャッシュなどを持っている場合は書き込まれて	*/
/*						いないデータを書き込む。								*/
/*				 	コマンド:GET_SECTOR_SIZE									*/
/*						UI_16型の変数buffにセクタサイズを返す。					*/
/*						有効値は512、1024、2048、4096							*/
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
			result = DISKDRV_RES_OK;	/* SDにキャッシュはないためなにもしない */
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
