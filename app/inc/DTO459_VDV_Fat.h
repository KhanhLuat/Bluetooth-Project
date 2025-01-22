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
/* FATファイルシステム														*/
/****************************************************************************/

#ifndef _DTO459_VDV_FAT_H
#define _DTO459_VDV_FAT_H


#include "type.h"
#include "DTO459_VDV_Disk_Drv.h"

/********************************************************************/
/*	マクロ定義														*/
/********************************************************************/
/* ドライブ名、区切り文字 */
#define	FS_DRV_NAME_SD				"SD"			/* SDドライブ名 */
#define	FS_SEP_DRV					":"				/* ドライブの区切り文字 */
#define	FS_SEP_DIR					"\\"			/* ディレクトリの区切り文字 */

/* パス名 */
#define	FS_DRV_PATH_SD		FS_DRV_NAME_SD FS_SEP_DRV FS_SEP_DIR		/* SDドライブパス */

/* ファイル名長 */
#define	FS_FILENAME_LEN_MAX			(32)
/* ファイル名長は以下の範囲で設定すること */
/* 12 <= FS_FILENAME_LEN_MAX <= 255 */
/* VDVでアクセスするファイル名（パス含む）が32バイト以上となる場合はここを増やすこと */
/* システムで使用するメモリ（スタック）使用量に影響するためメモリ使用量も確認すること */
/* (FS_FILENAME_LEN_MAX以上のファイル名のファイルが格納されていてもVDVでアクセスしない場合は動作に影響はない) */

/* 空きクラスタ数（FileSystem_GetFreeで取得する値） */
#define	FS_FREE_CLUST_NUM_UNKNOWN	(0xFFFFFFFFUL)	/* 空きクラスタ数不明 */

/* ファイルアクセスモード (FileSystem_Open関数で指定する) */
/* (DTO459_VDVFat.cにも内部用定義あり。内部用定義とかぶらないこと) */
#define	FS_FA_MODE_OPEN_EXISTING	(0x00)			/* 既存のファイルを開く。ファイルが無いときはエラー。 */
#define	FS_FA_MODE_READ				(0x01)			/* 読み出しモードで開く。読み書きする場合はFS_FA_MODE_WRITEと共に指定する。 */
#define	FS_FA_MODE_WRITE			(0x02)			/* 書き込みモードで開く。読み書きする場合はFS_FA_MODE_READと共に指定する。 */
#define	FS_FA_MODE_CREATE_NEW		(0x04)			/* ファイルを作成する。同名のファイルがある場合は戻り値FS_RESULT_ERR_EXISTが返る。 */
#define	FS_FA_MODE_CREATE_ALWAYS	(0x08)			/* ファイルを作成する。同名のファイルがある場合はサイズを0にして開く。 */
#define	FS_FA_MODE_OPEN_ALWAYS		(0x10)			/* 既存のファイルを開く。ファイルが無い場合はファイルを作成する。 */
#define	FS_FA_MODE_OPEN_APPEND		(0x30)			/* FS_FA_MODE_OPEN_ALWAYSと同じでリード/ライトポインタはファイルの最後尾にセットされる。 */
													/* FS_FA_MODE_OPEN_ALWAYSとFS_FA_MODE_SEEKEND(内部用定義)の組み合わせ */

/* 処理結果 */
typedef enum {
	FS_RESULT_OK = 0,								/* (0)  正常終了 */
	FS_RESULT_ERR_DISK,								/* (1)  ファイルシステムの下位レイヤ(ドライブ層)で回復不能なエラーが発生した */
	FS_RESULT_ERR_INTERNAL,							/* (2)  内部エラー */
	FS_RESULT_ERR_NOT_READY,						/* (3)  ファイルシステムの下位レイヤ(ドライブ層)でアクセス不能（メディアがセットされていないなど） */
	FS_RESULT_ERR_NO_FILE,							/* (4)  指定されたファイルが見つからない */
	FS_RESULT_ERR_NO_PATH,							/* (5)  指定されたパスが見つからない */
	FS_RESULT_ERR_INVALID_NAME,						/* (6)  不正なパス名が指定された */
	FS_RESULT_ERR_DENIED,							/* (7)  アクセスが拒否された */
	FS_RESULT_ERR_EXIST,							/* (8)  新しく作成しようとしたオブジェクトと同じ名前のオブジェクトが既に存在する */
	FS_RESULT_ERR_INVALID_OBJECT,					/* (9)  指定されたファイル オブジェクトやディレクトリ オブジェクトが無効 */
	FS_RESULT_ERR_WRITE_PROTECTED,					/* (10) 物理ドライブが書き込み禁止状態 */
	FS_RESULT_ERR_INVALID_DRIVE,					/* (11) パス名中に指定されたドライブが無効 */
	FS_RESULT_ERR_NO_FILESYSTEM,					/* (12) 物理ドライブ上に有効なFATボリュームがない */
	FS_RESULT_ERR_LOCKED,							/* (13) 多重アクセス排他制御によりアクセスが拒否された */
	FS_RESULT_ERR_TOO_MANY_OPEN_FILES,				/* (14) 同時オープン可能なファイル数を越えてファイルを開こうとした */
	FS_RESULT_ERR_INVALID_PARAM						/* (15) 入力パラメタ不正 */
} E_FS_RESULT;

/********************************************************************/
/*	構造体定義														*/
/********************************************************************/
/* ファイル情報 */
typedef struct {
	UI_32				fsize;							/* ファイルサイズ[byte] */
	UI_16				fdate;							/* ファイル更新日 */
	UI_16				ftime;							/* ファイル更新時間 */
	UI_8				fattrib;						/* ファイル属性 */
	UI_8				altname[13];					/* 8.3形式のファイル名 */
	UI_8				fname[FS_FILENAME_LEN_MAX + 1];	/* ファイル名 */
} FS_FILE_INFO;


/********************************************************************/
/*	外部関数														*/
/********************************************************************/
/* 初期化 */
void	FileSystem_Init(void);

/* ボリューム管理 */
E_FS_RESULT	FileSystem_Mount(const UI_8 *path);
E_FS_RESULT	FileSystem_Unmount(const UI_8 *path);
E_FS_RESULT FileSystem_GetFree(const UI_8 *path, UI_32 *nclst, UI_16 *sectPerClst);

/*ファイルアクセス */
E_FS_RESULT FileSystem_Open(const UI_8 *path, UI_8 mode, UI_8 *fileID);
E_FS_RESULT FileSystem_Close(UI_8 fileID);
E_FS_RESULT FileSystem_Read(UI_8 fileID, UI_8 *buff, UI_32 btr, UI_32 *br);
E_FS_RESULT FileSystem_Write(UI_8 fileID, const UI_8 *buff, UI_32 btw, UI_32 *bw);
E_FS_RESULT FileSystem_Seek(UI_8 fileID, UI_32 ofs);
E_FS_RESULT FileSystem_Tell(UI_8 fileID, UI_32 *curOfs);
E_FS_RESULT FileSystem_Size(UI_8 fileID, UI_32 *size);
E_FS_RESULT FileSystem_Sync(UI_8 fileID);
E_FS_RESULT FileSystem_Stat(const UI_8 *path, FS_FILE_INFO *finfo);

/* ディレクトリアクセス */
E_FS_RESULT FileSystem_OpenDir(const UI_8 *path, UI_8 *dirID);
E_FS_RESULT FileSystem_CloseDir(UI_8 dirID);
E_FS_RESULT FileSystem_ReadDir(UI_8 dirID, FS_FILE_INFO *finfo);
E_FS_RESULT FileSystem_MakeDir(const UI_8 *path);
E_FS_RESULT FileSystem_FindFirst(UI_8 *dirID, FS_FILE_INFO *finfo, const UI_8 *path, const UI_8 *pattern);
E_FS_RESULT FileSystem_FindNext(UI_8 dirID, FS_FILE_INFO *finfo);

#endif /* _DTO459_VDV_FAT_H */
