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
/* ファイルIO														*/
/********************************************************************/
#ifndef	DTO459_VDV_FILE_IO_H
#define	DTO459_VDV_FILE_IO_H

/********************************************************************/
/*	インクルードファイル											*/
/********************************************************************/
#include "type.h"
#include "DTO459_VDV_Fat.h"

/********************************************************************/
/*	マクロ定義														*/
/********************************************************************/
/* パス名長 */ 
#define FIO_PATH_LEN_MAX			FS_FILENAME_LEN_MAX

/* Writeのオフセット指定 */
#define FIO_OFS_EOF					(0xFFFFFFFFUL)	/* ファイル終端 */

/* 空きセクタ数（FileIO_Get_FreeSectNumで取得する値）	*/
#define	FIO_FREE_SECT_NUM_UNKNOWN	(0xFFFFFFFFUL)	/* 空きセクタ数不明	*/

/* 書き込み種別 */ 
typedef enum e_fio_write_type
{
	FIO_WRITE_TYPE_FILE_OPEN	= 0,		/* 既存のファイルを開いて書き込む				*/
											/* 既存のファイルが無い場合は新規作成する		*/
	FIO_WRITE_TYPE_FILE_CREATE				/* 新規ファイルを作成して書き込む				*/
											/* 既存のファイルがある場合は破棄して書き込む	*/
} E_FIO_WRITE_TYPE;

/* 要求種別 */ 
typedef enum e_fio_req_type
{
	FIO_REQ_TYPE_READ	= 0,				/* リード要求				*/
	FIO_REQ_TYPE_WRITE						/* ライト要求				*/
} E_FIO_REQ_TYPE;

/* 処理結果 */
typedef enum e_fio_result {
	FIO_RESULT_OK = 0,						/* 正常終了						*/
	FIO_RESULT_ERR_FATAL,					/* 異常終了						*/
	FIO_RESULT_ERR_PARAM,					/* 入力パラメタ不正				*/
	FIO_RESULT_ERR_DISK_NOT_READY,			/* ディスク使用不可				*/
	FIO_RESULT_ERR_INVALID_DRIVE,			/* ドライブ名不正				*/
	FIO_RESULT_ERR_NO_FILE,					/* 指定されたファイルがない		*/
	FIO_RESULT_ERR_NO_PATH,					/* 指定されたパスがない			*/
	FIO_RESULT_ERR_SNDMSG,					/* メッセージ送信エラー			*/
	FIO_RESULT_ERR_EXIST					/* 同名のオブジェクトが存在する	*/
} E_FIO_RESULT;

/********************************************************************/
/*	構造体定義														*/
/********************************************************************/
/* 要求情報 */
typedef struct {
	UI_32				reqno;						/* 要求No								*/
	E_FIO_REQ_TYPE		reqtype;					/* 要求種別								*/
	UI_8				path[FIO_PATH_LEN_MAX+1];	/* ファイルパス							*/
	UI_8				*buff;						/* バッファ								*/
	UI_32				ofs;						/* ファイル先頭からのオフセット(byte)	*/
	UI_32				bytes;						/* サイズ(byte)							*/
} T_FIO_REQ_INFO;

/* 処理結果情報 */
typedef struct {
	E_FIO_RESULT		result;						/* 処理結果								*/
	UI_32				bytes;						/* サイズ(byte)							*/
													/* reqtype=FIO_REQ_TYPE_READの場合:読み込んだサイズ		*/
													/* reqtype=FIO_REQ_TYPE_WRITEの場合:書き込んだサイズ	*/
	UI_32				fsize;						/* ファイルサイズ(byte)					*/
} T_FIO_RET_INFO;

/* 要求情報（ファイル検索用） */
typedef struct {
	UI_32				reqno;							/* 要求No								*/
	UI_8				path[FIO_PATH_LEN_MAX+1];		/* 検索するパス							*/
	UI_8				filename[FIO_PATH_LEN_MAX+1];	/* 検索ファイル名						*/
} T_FIO_SRCF_REQ_INFO;

/* 処理結果情報（ファイル検索用） */
typedef struct {
	E_FIO_RESULT		result;							/* 処理結果								*/
	UI_8				filename[FIO_PATH_LEN_MAX+1];	/* 検索したファイル名					*/
} T_FIO_SRCF_RET_INFO;

/* 要求情報（ディレクトリ操作用） */
typedef struct {
	UI_32				reqno;							/* 要求No								*/
	UI_8				path[FIO_PATH_LEN_MAX+1];		/* 検索するパス							*/
} T_FIO_DIR_REQ_INFO;

/* 処理結果情報（ディレクトリ操作用） */
typedef struct {
	E_FIO_RESULT		result;							/* 処理結果								*/
} T_FIO_DIR_RET_INFO;

/* コールバック関数 */
typedef void (*PFN_FIO_CB_NOTIFY)( const T_FIO_REQ_INFO *reqinfo, const T_FIO_RET_INFO *retinfo );
													/* Read/Write用							*/
typedef B_8 (*PFN_FIO_SRCF_CB_NOTIFY)( const T_FIO_SRCF_REQ_INFO *reqinfo, const T_FIO_SRCF_RET_INFO *retinfo );
													/* ファイル検索用						*/
typedef void (*PFN_FIO_DIR_CB_NOTIFY)( const T_FIO_DIR_REQ_INFO *reqinfo, const T_FIO_DIR_RET_INFO *retinfo );
													/* ディレクトリ操作用					*/


/********************************************************************/
/*	外部関数														*/
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
