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
/* ファイルIO																*/
/****************************************************************************/


/****************************************************************************/
/*	インクルードファイル													*/
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
/*	マクロ定義																*/
/****************************************************************************/
/* メッセージID */ 
typedef enum e_fio_msg_id
{
	FIO_MSG_ID_READ_REQ	= 1,				/* リード要求				*/
	FIO_MSG_ID_WRITE_REQ,					/* ライト要求				*/
	FIO_MSG_ID_SRC_FILE_REQ,				/* ファイル検索要求			*/
	FIO_MSG_ID_MKDIR_REQ					/* ディレクトリ作成要求		*/
} E_FIO_MSG_ID;

/* ファイルシステムが対応するドライブID	*/
typedef enum e_fio_drv_id
{
	FIO_DRV_ID_SD		= 0,				/* SD				*/

	/* ここより上に追加すること	*/
	/* 追加した場合はC_fio_drv_nameにドライブ名を追加すること	*/
	FIO_DRV_ID_NUM,							/* ドライブ数		*/
	FIO_DRV_ID_INVALID						/* 無効値			*/
} E_FIO_DRV_ID;

/* メッセージキューの段数 */
#define FIO_MSGQUE_NUM			16

/* マウント処理のリトライ回数 */
#define FIO_MOUNT_RETRY_NUM		2

/****************************************************************************/
/*	構造体定義																*/
/****************************************************************************/
/* リードパラメタ */
typedef struct {
	UI_8				path[FIO_PATH_LEN_MAX+1];	/* ファイルパス							*/
	UI_8				*buff;						/* バッファ								*/
	UI_32				ofs;						/* ファイル先頭からのオフセット(byte)	*/
	UI_32				bytes;						/* サイズ(byte)							*/
	PFN_FIO_CB_NOTIFY	cbfnc;						/* コールバック関数						*/
} T_FIO_RD_PARAM;

/* ライトパラメタ */
typedef struct {
	E_FIO_WRITE_TYPE	wtype;						/* 書き込み種別(FIO_WRITE_TYPE_xxx)		*/
	UI_8				path[FIO_PATH_LEN_MAX+1];	/* ファイルパス							*/
	const UI_8			*buff;						/* バッファ								*/
	UI_32				ofs;						/* ファイル先頭からのオフセット(byte)	*/
	UI_32				bytes;						/* サイズ(byte)							*/
	PFN_FIO_CB_NOTIFY	cbfnc;						/* コールバック関数						*/
} T_FIO_WR_PARAM;

/* ファイル検索パラメタ */
typedef struct {
	UI_8					path[FIO_PATH_LEN_MAX+1];		/* 検索ディレクトリのパス		*/
	UI_8					filename[FIO_PATH_LEN_MAX+1];	/* 検索するファイル名			*/
	PFN_FIO_SRCF_CB_NOTIFY	cbfnc;							/* コールバック関数				*/
} T_FIO_SRCF_PARAM;

/* ディレクトリ作成パラメタ */
typedef struct {
	UI_8					path[FIO_PATH_LEN_MAX+1];	/* ディレクトリパス						*/
	PFN_FIO_DIR_CB_NOTIFY	cbfnc;						/* コールバック関数						*/
} T_FIO_DIR_PARAM;

/* 要求パラメタ */
typedef	union {
	T_FIO_RD_PARAM		read;						/* リードパラメタ						*/
	T_FIO_WR_PARAM		write;						/* ライトパラメタ						*/
	T_FIO_SRCF_PARAM	srcf;						/* ファイル検索パラメタ					*/
	T_FIO_DIR_PARAM		dir;						/* ディレクトリ作成パラメタ				*/
} U_FIO_REQ_PARAM;

/* 要求メッセージ */
typedef struct {
	E_FIO_MSG_ID		msgid;						/* メッセージID							*/
	UI_32				reqno;						/* 要求No								*/
	U_FIO_REQ_PARAM		param;						/* 要求パラメタ							*/
} T_FIO_MSG;

/* ドライブ情報	*/
typedef struct {
	const UI_8			*drvname;					/* ドライブ名							*/
	B_8					mountExecuted;				/* マウント実行状態 N_TRUE:マウント処理を実行した	*/
	B_8					mount;						/* マウント状態 TRUE:マウント済み		*/
	UI_8				resv[2];
	UI_32				free_sect_num;				/* 空きセクタ数							*/
} T_FIO_DRV_INFO;

/****************************************************************************/
/*	定数テーブル															*/
/****************************************************************************/
/* ドライブ名	*/
static const UI_8 *C_fio_drv_name[FIO_DRV_ID_NUM] =
{
	(const UI_8*)FS_DRV_PATH_SD,								/* SD:FIO_DRV_ID_SD	*/
	/* ドライブID順で追加すること */
};

/****************************************************************************/
/*	内部変数定義															*/
/****************************************************************************/
static UI_32 S_fio_reqno;									/* リクエスト番号(外部IFで返すシーケンシャルな番号) */
static T_RINGBUFFER S_fio_msgbuff;							/* 要求メッセージバッファ管理 */
static T_FIO_MSG S_fio_msg_data[FIO_MSGQUE_NUM];			/* 要求メッセージバッファ */
static UI_8 S_fio_sd_detect_in;								/* SD挿入検出フラグ */
static UI_8 S_fio_sd_detect_out;							/* SD排出検出フラグ */
static T_FIO_MSG S_fio_sndmsg_work;							/* 送信メッセージ用ワークバッファ */
static T_FIO_MSG S_fio_rcvmsg_work;							/* 受信メッセージ用ワークバッファ */
static T_FIO_REQ_INFO S_fio_cbparam_work;					/* コールバックパラメタ用ワークバッファ */
static T_FIO_SRCF_REQ_INFO S_fio_srcf_cb_reqinfo_work;		/* ファイル検索の要求情報用ワークバッファ */
static T_FIO_SRCF_RET_INFO S_fio_srcf_cb_retinfo_work;		/* ファイル検索の結果情報用ワークバッファ */
static T_FIO_DIR_REQ_INFO S_fio_dir_cb_reqinfo_work;		/* ディレクトリ作成の要求情報用ワークバッファ */
static UI_8 S_fio_dir_path_work[FIO_PATH_LEN_MAX+1];		/* ディレクトリパスワークバッファ */
static FS_FILE_INFO S_fio_fsfinfo_work;						/* ファイル情報ワークバッファ */
static T_FIO_DRV_INFO S_fio_drv_info[FIO_DRV_ID_NUM];		/* ドライブ情報 */

/****************************************************************************/
/*	内部関数宣言															*/
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
/*	外部関数																*/
/****************************************************************************/
/****************************************************************************/
/* 関数名称		:	FileIO_Read												*/
/* 機能の説明	:	ファイル読み込み(非同期)								*/
/* 引数			:	[I/ ]path	ファイルパス								*/
/*								(本関数終了後、領域解放可)					*/
/*					[I/ ]buff	読み込みデータバッファ						*/
/*								(コールバックまで領域を保証すること)		*/
/*					[I/ ]ofs	ファイル先頭からのオフセット(byte)			*/
/*					[I/ ]bytes	読み込むバイト数							*/
/*					[I/ ]cbfnc	コールバック関数							*/
/*								(本関数が正常終了の場合にコールバックする)	*/
/*					[ /O]reqno	要求No										*/
/*								(コールバックで同一の要求Noを返す)			*/
/* 戻り値		: 	FIO_RESULT_OK		正常終了							*/
/*					FIO_RESULT_ERR_xxx	異常終了							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/08/18												*/
/* 備考			:	スレッドセーフではないため同一プロセスから呼び出すこと	*/
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

	/* 入力パラメタチェック */
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

	/* メッセージ作成 */
	*reqno = fio_generate_reqno();
	S_fio_sndmsg_work.msgid = FIO_MSG_ID_READ_REQ;
	S_fio_sndmsg_work.reqno = *reqno;

	param = &S_fio_sndmsg_work.param.read;
	fio_strcpy(param->path, sizeof(param->path), path);
	param->buff = buff;
	param->ofs = ofs;
	param->bytes = bytes;
	param->cbfnc = cbfnc;

	/* メッセージ送信 */
	result = fio_snd_msg(&S_fio_sndmsg_work);
	if (result != FIO_RESULT_OK) {
		OUTLOG(OutputLog("[FIO]ReadIf SndMsg ERR!!\n");)
	}

	return result;
}

/****************************************************************************/
/* 関数名称		:	FileIO_Write											*/
/* 機能の説明	:	ファイル書き込み(非同期)								*/
/* 引数			:	[I/ ]wtype	書き込み種別(FIO_WRITE_TYPE_xxx)			*/
/* 					[I/ ]path	ファイルパス								*/
/*								(本関数終了後、領域解放可)					*/
/*					[I/ ]buff	書き込みデータバッファ						*/
/*								(コールバックまで領域を保証すること)		*/
/*					[I/ ]ofs	ファイル先頭からのオフセット(byte)			*/
/*					[I/ ]bytes	書き込むバイト数							*/
/*					[I/ ]cbfnc	コールバック関数							*/
/*								(本関数が正常終了の場合にコールバックする)	*/
/*					[ /O]reqno	要求No										*/
/*								(コールバックで同一の要求Noを返す)			*/
/* 戻り値		: 	FIO_RESULT_OK		正常終了							*/
/*					FIO_RESULT_ERR_xxx	異常終了							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/08/18												*/
/* 備考			:	スレッドセーフではないため同一プロセスから呼び出すこと	*/
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

	/* 入力パラメタチェック */
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

	/* メッセージ作成 */
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

	/* メッセージ送信 */
	result = fio_snd_msg(&S_fio_sndmsg_work);
	if (result != FIO_RESULT_OK) {
		OUTLOG(OutputLog("[FIO]WriteIf SndMsg ERR!!\n");)
	}

	return result;
}

/****************************************************************************/
/* 関数名称		:	FileIO_SearchFile										*/
/* 機能の説明	:	ファイル検索(非同期)									*/
/* 引数			:	[I/ ]path		検索ディレクトリのパス					*/
/*									(本関数終了後、領域解放可)				*/
/* 				:	[I/ ]filename	検索するファイル名						*/
/*									(本関数終了後、領域解放可)				*/
/*					[I/ ]cbfnc		コールバック関数						*/
/*								(本関数が正常終了の場合にコールバックする)	*/
/*					[ /O]reqno		要求No									*/
/*									(コールバックで同一の要求Noを返す)		*/
/* 戻り値		: 	FIO_RESULT_OK		正常終了							*/
/*					FIO_RESULT_ERR_xxx	異常終了							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/08/18												*/
/* 備考			:	ファイル名にワイルドカード文字'?'または'*'の指定が可能。*/
/*					'?'は任意の1文字に、'*'は0文字以上の任意の文字列に		*/
/*					マッチする。											*/
/*					マッチしたファイルを検索する毎にコールバックを通知する。*/
/*					コールバック通知の最大回数はマッチしたファイル数＋1回	*/
/*					（検索終了を示す）となる。								*/
/*					コールバック通知のファイル名変数に検索したファイル名を	*/
/*					格納する。検索終了はファイル名変数にNULL文字を格納する。*/
/*					ファイルの通知は順不同。（検索できた順）				*/
/*					コールバック関数の戻り値でN_TRUEを返した場合は検索を	*/
/*					継続する。												*/
/*					コールバック関数の戻り値でN_FALSEを返した場合は検索を	*/
/*					終了する。（以降コールバックは通知しない）				*/
/*					コールバック通知の検索結果がエラーの場合は以降コール	*/
/*					バックは通知しない。									*/
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

	/* 入力パラメタチェック */
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

	/* メッセージ作成 */
	*reqno = fio_generate_reqno();
	S_fio_sndmsg_work.msgid = FIO_MSG_ID_SRC_FILE_REQ;
	S_fio_sndmsg_work.reqno = *reqno;

	param = &S_fio_sndmsg_work.param.srcf;
	fio_strcpy(param->path, sizeof(param->path), path);
	fio_strcpy(param->filename, sizeof(param->filename), filename);
	param->cbfnc = cbfnc;

	/* メッセージ送信 */
	result = fio_snd_msg(&S_fio_sndmsg_work);
	if (result != FIO_RESULT_OK) {
		OUTLOG(OutputLog("[FIO]SrcFileIf SndMsg ERR!!\n");)
	}

	return result;
}

/****************************************************************************/
/* 関数名称		:	FileIO_MakeDir											*/
/* 機能の説明	:	ディレクトリ作成(非同期)								*/
/* 引数				[I/ ]path	ディレクトリパス							*/
/*								(本関数終了後、領域解放可)					*/
/*					[I/ ]cbfnc	コールバック関数							*/
/*								(本関数が正常終了の場合にコールバックする)	*/
/*					[ /O]reqno	要求No										*/
/*								(コールバックで同一の要求Noを返す)			*/
/* 戻り値		: 	FIO_RESULT_OK		正常終了							*/
/*					FIO_RESULT_ERR_xxx	異常終了							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	17/01/20												*/
/* 備考			:	スレッドセーフではないため同一プロセスから呼び出すこと	*/
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

	/* 入力パラメタチェック */
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

	/* メッセージ作成 */
	*reqno = fio_generate_reqno();
	S_fio_sndmsg_work.msgid = FIO_MSG_ID_MKDIR_REQ;
	S_fio_sndmsg_work.reqno = *reqno;

	param = &S_fio_sndmsg_work.param.dir;
	fio_strcpy(param->path, sizeof(param->path), path);
	param->cbfnc = cbfnc;

	/* メッセージ送信 */
	result = fio_snd_msg(&S_fio_sndmsg_work);
	if (result != FIO_RESULT_OK) {
		OUTLOG(OutputLog("[FIO]MakeDirIf SndMsg ERR!!\n");)
	}

	return result;
}

/****************************************************************************/
/* 関数名称		: FileIO_Get_FreeSectNum									*/
/* 機能の説明	: 空きセクタ数取得											*/
/* 引数			: [I/ ]drvpath			ドライブのパス						*/
/*				   [ /O]freesect		空きセクタ数						*/
/* 戻り値		: 	FIO_RESULT_OK		正常終了							*/
/*					FIO_RESULT_ERR_xxx	異常終了							*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 16/08/18													*/
/* 備考			: 															*/
/****************************************************************************/
E_FIO_RESULT FileIO_Get_FreeSectNum(
	const UI_8			*drvpath,
	UI_32				*freesect
)
{
	E_FIO_DRV_ID	drvid;

	/* 入力パラメタチェック */
	if ((drvpath == NULL) || (freesect == NULL)) {
		return FIO_RESULT_ERR_PARAM;
	}

	drvid = fio_path2drvid( drvpath );
	if (drvid == FIO_DRV_ID_INVALID) {		/* ドライブ名不正	*/
		return FIO_RESULT_ERR_INVALID_DRIVE;
	}
	if (S_fio_drv_info[drvid].mount == N_FALSE) {	/* マウントしていない	*/
		return FIO_RESULT_ERR_DISK_NOT_READY;
	}

	*freesect = S_fio_drv_info[drvid].free_sect_num;

	return FIO_RESULT_OK;
}

/****************************************************************************/
/* 関数名称		: FileIO_Get_MountExecStat									*/
/* 機能の説明	: マウント実行状態取得										*/
/* 引数			: [I/ ]drvpath			ドライブのパス						*/
/* 戻り値		: 	N_TRUE		マウントを実行した							*/
/*					N_FALSE		マウントを実行していない					*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 16/11/11													*/
/* 備考			: 															*/
/****************************************************************************/
B_8 FileIO_Get_MountExecStat(
	const UI_8			*drvpath
)
{
	E_FIO_DRV_ID	drvid;

	/* 入力パラメタチェック */
	if (drvpath == NULL) {
		return N_FALSE;
	}

	drvid = fio_path2drvid( drvpath );
	if (drvid == FIO_DRV_ID_INVALID) {		/* ドライブ名不正	*/
		return N_FALSE;
	}

	return S_fio_drv_info[drvid].mountExecuted;
}

/****************************************************************************/
/* 関数名称		: FileIO_Init												*/
/* 機能の説明	: ファイルIO初期化											*/
/* 引数			: void														*/
/* 戻り値		: void														*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 16/08/18													*/
/* 備考			: 															*/
/****************************************************************************/
void FileIO_Init(void)
{
	UI_8	i;

	/* 変数初期化 */
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

	/* SDカードIN/OUT検出のコールバックを登録 */
	(void)Chatt_absorber_ReqPortData(fio_sdc_dtct_cb, CHATT_PORT_SDC, CHATT_ABS_MOD_FIO);

	return;
}

/****************************************************************************/
/*	ファイルIOメイン（定期実行処理）										*/
/****************************************************************************/
/****************************************************************************/
/* 関数名称		: FileIO_Main												*/
/* 機能の説明	: ファイルIOメイン処理										*/
/* 引数			: void														*/
/* 戻り値		: void														*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 16/08/18													*/
/* 備考			: 															*/
/****************************************************************************/
void FileIO_Main(void)
{
	/* ドライブマウント制御 */
	fio_mount_ctrl();


	/* ファイルアクセスメッセージ受信 */
	if (fio_rcv_msg(&S_fio_rcvmsg_work) == N_TRUE) {

		switch (S_fio_rcvmsg_work.msgid) {
		case FIO_MSG_ID_READ_REQ:				/* リード要求 */
			(void)fio_read_proc(&S_fio_rcvmsg_work);
			break;

		case FIO_MSG_ID_WRITE_REQ:				/* ライト要求 */
			(void)fio_write_proc(&S_fio_rcvmsg_work);
			break;

		case FIO_MSG_ID_SRC_FILE_REQ:			/* ファイル検索要求 */
			(void)fio_searchfile_proc(&S_fio_rcvmsg_work);
			break;

		case FIO_MSG_ID_MKDIR_REQ:				/* ディレクトリ作成要求 */
			(void)fio_makedir_proc(&S_fio_rcvmsg_work);
			break;

		default:
			break;
		}

	}

	return;
}

/****************************************************************************/
/*	内部関数																*/
/****************************************************************************/
/****************************************************************************/
/* 関数名称		: fio_generate_reqno										*/
/* 機能の説明	: 要求No生成												*/
/* 引数			: なし														*/
/* 戻り値		: 要求No													*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 16/08/18													*/
/* 備考			: 															*/
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
/* 関数名称		: fio_snd_msg												*/
/* 機能の説明	: メッセージ送信											*/
/* 引数			: [I/ ]msg		送信メッセージ								*/
/* 戻り値		: 	FIO_RESULT_OK		正常終了							*/
/*					FIO_RESULT_ERR_xxx	異常終了							*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 16/08/18													*/
/* 備考			: 															*/
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
/* 関数名称		: fio_rcv_msg												*/
/* 機能の説明	: メッセージ受信											*/
/* 引数			: [ /O]msg		受信メッセージ								*/
/* 戻り値		: 	N_TRUE		受信メッセージあり							*/
/* 				 	N_FALSE		受信メッセージなし							*/
/* 作成者		: (OFT)Toguchi												*/
/* 作成日		: 16/08/18													*/
/* 備考			: 															*/
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
/* 関数名称		:	fio_make_reqinfo										*/
/* 機能の説明	:	要求情報作成											*/
/* 引数			:	[I/ ]msg		メッセージ								*/
/*					[ /O]reqinfo	要求情報								*/
/* 戻り値		:	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/08/18												*/
/* 備考			: 															*/
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
/* 関数名称		:	fio_make_srcf_reqinfo									*/
/* 機能の説明	:	要求情報作成(ファイル検索用)							*/
/* 引数			:	[I/ ]msg		メッセージ								*/
/*					[ /O]reqinfo	要求情報								*/
/* 戻り値		:	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/08/18												*/
/* 備考			: 															*/
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
/* 関数名称		:	fio_make_dir_reqinfo									*/
/* 機能の説明	:	要求情報作成(ディレクトリ操作用)						*/
/* 引数			:	[I/ ]msg		メッセージ								*/
/*					[ /O]reqinfo	要求情報								*/
/* 戻り値		:	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/08/18												*/
/* 備考			: 															*/
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
/* 関数名称		:	fio_read_proc											*/
/* 機能の説明	:	ファイル読み込み										*/
/* 引数			:	[I/ ]msg		メッセージ								*/
/* 戻り値		: 	FIO_RESULT_OK		正常終了							*/
/*					FIO_RESULT_ERR_xxx	異常終了							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/08/18												*/
/* 備考			:															*/
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

	/* ドライブのマウント状態をチェック	*/
	drvid = fio_path2drvid( msg->param.read.path );
	if (drvid == FIO_DRV_ID_INVALID) {		/* ドライブ名不正	*/
		result = FIO_RESULT_ERR_INVALID_DRIVE;
		OUTLOG(OutputLog("[FIO]Read drive name ERR!!\n");)
		goto PROC_END;
	}
	if (S_fio_drv_info[drvid].mount == N_FALSE) {	/* マウントしていない	*/
		result = FIO_RESULT_ERR_DISK_NOT_READY;
		OUTLOG(OutputLog("[FIO]Read mount ERR!!\n");)
		goto PROC_END;
	}

	/* ファイルオープン */
	result_fs = FileSystem_Open(msg->param.read.path, FS_FA_MODE_READ, &fid);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Read FsOpen ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}

	/* 入力オフセット値チェック */
	result_fs = FileSystem_Size(fid, &fSize);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Read FsSize ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}
	if (fSize < msg->param.read.ofs) {			/* オフセットがファイルサイズより大きい */
		result = FIO_RESULT_ERR_PARAM;
		goto PROC_END;
	}

	/* 読み込み位置に移動 */
	result_fs = FileSystem_Seek(fid, msg->param.read.ofs);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Read FsSeek ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}

	/* ファイル読み込み */
	result_fs = FileSystem_Read(fid, msg->param.read.buff, msg->param.read.bytes, &bytesRead);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Read FsRead ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}

	result = FIO_RESULT_OK;		/* 正常終了	*/

PROC_END:

	/* ファイルクローズ */
	if (fid > 0) {
		(void)FileSystem_Close(fid);
	}

	/* コールバック通知 */
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
/* 関数名称		:	fio_write_proc											*/
/* 機能の説明	:	ファイル書き込み										*/
/* 引数			:	[I/ ]msg		メッセージ								*/
/* 戻り値		: 	FIO_RESULT_OK		正常終了							*/
/*					FIO_RESULT_ERR_xxx	異常終了							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	2016/MM/DD												*/
/* 備考			:															*/
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

	/* ドライブのマウント状態をチェック	*/
	drvid = fio_path2drvid( msg->param.write.path );
	if (drvid == FIO_DRV_ID_INVALID) {		/* ドライブ名不正	*/
		result = FIO_RESULT_ERR_INVALID_DRIVE;
		OUTLOG(OutputLog("[FIO]Write drive name ERR!!\n");)
		goto PROC_END;
	}
	if (S_fio_drv_info[drvid].mount == N_FALSE) {	/* マウントしていない	*/
		result = FIO_RESULT_ERR_DISK_NOT_READY;
	/*	OUTLOG(OutputLog("[FIO]Write mount ERR!!\n");)	*/
		goto PROC_END;
	}

	/* ファイルオープン */
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

	/* 入力オフセット値チェック */
	result_fs = FileSystem_Size(fid, &fSize);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Write FsSize ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}
	if ((msg->param.write.ofs != FIO_OFS_EOF) && (fSize < msg->param.write.ofs)) {	/* オフセットがファイルサイズより大きい */
		result = FIO_RESULT_ERR_PARAM;
		goto PROC_END;
	}

	/* 書込み位置に移動 */
	if (msg->param.write.ofs == FIO_OFS_EOF) {
		ofs_w = fSize;							/* ファイル終端に移動		*/
	}
	else {
		ofs_w = msg->param.write.ofs;			/* 指定オフセットに移動		*/
	}
	result_fs = FileSystem_Seek(fid, ofs_w);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Write FsSeek ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}

	/* ファイル書き込み */
	result_fs = FileSystem_Write(fid, msg->param.write.buff, msg->param.write.bytes, &bytesWritten);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Write FsWrite ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}
	else {
		if (bytesWritten < msg->param.write.bytes) {	/* 要求サイズ分書き込めていない	*/
			/* 空きなし（空きセクタ数:0） */
			S_fio_drv_info[drvid].free_sect_num = 0;
			OUTLOG(OutputLog("[FIO]Write FsWrite memory full!!\n");)
		}
		else {
			/* 空きセクタ数取得 */
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

	/* ファイルサイズ取得 */
	result_fs = FileSystem_Size(fid, &fSize);
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
		OUTLOG(OutputLog("[FIO]Write FsSize 2 ERR!! fs_res:%d\n", result_fs);)
		goto PROC_END;
	}

	result = FIO_RESULT_OK;		/* 正常終了	*/

PROC_END:

	/* ファイルクローズ */
	if (fid > 0) {
		(void)FileSystem_Close(fid);
	}

	/* コールバック通知 */
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
/* 関数名称		:	fio_searchfile_proc										*/
/* 機能の説明	:	ファイル検索											*/
/* 引数			:	[I/ ]msg		メッセージ								*/
/* 戻り値		: 	FIO_RESULT_OK		正常終了							*/
/*					FIO_RESULT_ERR_xxx	異常終了							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	2016/MM/DD												*/
/* 備考			:															*/
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

	/* コールバックパラメタ作成	*/
	fio_make_srcf_reqinfo(msg, &S_fio_srcf_cb_reqinfo_work);

	/* ドライブのマウント状態をチェック	*/
	drvid = fio_path2drvid( msg->param.srcf.path );
	if (drvid == FIO_DRV_ID_INVALID) {		/* ドライブ名不正	*/
		result = FIO_RESULT_ERR_INVALID_DRIVE;
		OUTLOG(OutputLog("[FIO]SrcFile drive name ERR!!\n");)
		goto ERR_END;
	}
	if (S_fio_drv_info[drvid].mount == N_FALSE) {	/* マウントしていない	*/
		result = FIO_RESULT_ERR_DISK_NOT_READY;
		OUTLOG(OutputLog("[FIO]SrcFile mount ERR!!\n");)
		goto ERR_END;
	}

	/* 先頭ファイル検索	*/
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

	/* コールバック通知(先頭ファイル検索結果通知) */
	S_fio_srcf_cb_retinfo_work.result = result;
	ret_callback = msg->param.srcf.cbfnc(&S_fio_srcf_cb_reqinfo_work, &S_fio_srcf_cb_retinfo_work);

	if (find_first == N_TRUE) {
		/* ２番目以降のファイル検索	*/
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

			/* コールバック通知(2番目以降のファイル検索結果通知) */
			S_fio_srcf_cb_retinfo_work.result = result;
			ret_callback = msg->param.srcf.cbfnc(&S_fio_srcf_cb_reqinfo_work, &S_fio_srcf_cb_retinfo_work);
		}
	}

	/* ディレクトリクローズ */
	if (did > 0) {
		(void)FileSystem_CloseDir(did);
	}

	if ((result == FIO_RESULT_ERR_NO_FILE) && (find_first == N_TRUE)) {
		/* 先頭ファイルのみ検索できた場合は戻り値:正常を返す */
		result = FIO_RESULT_OK;
	}

	return result;

ERR_END:

	/* コールバック通知 */
	S_fio_srcf_cb_retinfo_work.result = result;
	S_fio_srcf_cb_retinfo_work.filename[0] = '\0';
	msg->param.srcf.cbfnc(&S_fio_srcf_cb_reqinfo_work, &S_fio_srcf_cb_retinfo_work);

	return result;

}

/****************************************************************************/
/* 関数名称		:	fio_makedir_proc										*/
/* 機能の説明	:	ディレクトリ作成										*/
/* 引数			:	[I/ ]msg		メッセージ								*/
/* 戻り値		: 	FIO_RESULT_OK		正常終了							*/
/*					FIO_RESULT_ERR_xxx	異常終了							*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	2017/01/20												*/
/* 備考			:															*/
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

	/* ドライブのマウント状態をチェック	*/
	drvid = fio_path2drvid( msg->param.dir.path );
	if (drvid == FIO_DRV_ID_INVALID) {		/* ドライブ名不正	*/
		result = FIO_RESULT_ERR_INVALID_DRIVE;
		OUTLOG(OutputLog("[FIO]MakeDir drive name ERR!!\n");)
		goto PROC_END;
	}
	if (S_fio_drv_info[drvid].mount == N_FALSE) {	/* マウントしていない	*/
		result = FIO_RESULT_ERR_DISK_NOT_READY;
/*		OUTLOG(OutputLog("[FIO]MakeDir mount ERR!!\n");) */
		goto PROC_END;
	}

	pathlen = strlen((const CH_8*)msg->param.dir.path);
	if ((msg->param.dir.path[(pathlen - 1)] == '\\') || (msg->param.dir.path[(pathlen - 1)] == '/')){
		/* パスの終端がセパレータの場合はエラー	*/
		result = FIO_RESULT_ERR_NO_PATH;
		OUTLOG(OutputLog("[FIO]MakeDir path sepa ERR!!\n");)
		goto PROC_END;
	}

	/* ディレクトリ階層数を取得 */
	hierarchy_num = fio_get_dir_hierarchy_num(msg->param.dir.path);
	if (hierarchy_num == 0) {
		result = FIO_RESULT_ERR_NO_PATH;
		OUTLOG(OutputLog("[FIO]MakeDir not dir ERR!!\n");)
		goto PROC_END;
	}

	/* 階層数分のディレクトリを作成する */
	for (i = 0; i < hierarchy_num; i++) {
		if (fio_get_dir_path(msg->param.dir.path, (i + 1), S_fio_dir_path_work) != N_OK) {
			result = FIO_RESULT_ERR_FATAL;
			OUTLOG(OutputLog("[FIO]MakeDir get path ERR!!\n");)
			goto PROC_END;
		}

		/* ディレクトリ作成	*/
		result_fs = FileSystem_MakeDir(S_fio_dir_path_work);
		if ((result_fs != FS_RESULT_OK) &&
			(result_fs != FS_RESULT_ERR_EXIST)) {
			/* ディレクトリ作成失敗	*/
			break;
		}
	}
	if (result_fs != FS_RESULT_OK) {
		fio_cnv_res_fs2fio(result_fs, &result);
/*		OUTLOG(OutputLog("[FIO]MakeDir FsMakeDir ERR!! fs_res:%d\n", result_fs);) */
		goto PROC_END;
	}

	result = FIO_RESULT_OK;		/* 正常終了	*/

PROC_END:

	/* コールバック通知 */
	if (msg->param.dir.cbfnc != NULL) {
		retinfo.result = result;
		fio_make_dir_reqinfo(msg, &S_fio_dir_cb_reqinfo_work);

		msg->param.dir.cbfnc(&S_fio_dir_cb_reqinfo_work, &retinfo);
	}

	return result;

}

/****************************************************************************/
/* 関数名称		:	fio_get_dir_hierarchy_num								*/
/* 機能の説明	:	ディレクトリ階層数取得									*/
/* 引数			:	[I/ ]path		パス									*/
/* 戻り値		: 	ディレクトリ階層数										*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	2017/01/20												*/
/* 備考			:	pathはドライブ名から始まる文字を指定すること。			*/
/*					ドライブ名の次のディレクトリが1階層目になる。			*/
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
		/* 最後の文字がセパレータの場合 階層数を-1する	*/
		hcnt--;
	}

	return hcnt;

}

/****************************************************************************/
/* 関数名称		:	fio_get_dir_path										*/
/* 機能の説明	:	ディレクトリパス取得									*/
/* 引数			:	[I/ ]path_src	入力パス								*/
/*					[I/ ]hnum		階層数									*/
/*					[ /O]path_dst	指定階層のパス							*/
/* 戻り値		: 	N_OK	正常											*/
/* 				: 	N_NG	異常											*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	2017/01/20												*/
/* 備考			:	pathはドライブ名から始まる文字を指定すること。			*/
/*					ドライブ名の次のディレクトリが1階層目になる。			*/
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
/* 関数名称		:	fio_cnv_res_fs2fio										*/
/* 機能の説明	:	処理結果変換(FAT結果->FileIO結果)						*/
/* 引数			:	[I/ ]result_fs	FATの処理結果							*/
/*					[ /O]result_fio	FileIOの処理結果						*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/08/18												*/
/* 備考			:															*/
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
/* 関数名称		:	fio_mount_ctrl											*/
/* 機能の説明	:	ファイルシステムのマウント/アンマウント制御				*/
/* 引数			:	なし													*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/08/18												*/
/* 備考			:															*/
/****************************************************************************/
static void fio_mount_ctrl(void)
{

	E_SYS_CTRL_STATUS	SysStatus;

	/* システム動作状態取得 */
	SysStatus = System_control_GetStatus();

	switch (SysStatus) {
	case SYS_OPERATION_WAIT_ACC_ON:			/**< ACC ON待ち				*/
	case SYS_OPERATION_PRE_ACC_OFF:			/**< ACC OFF移行			*/
	case SYS_OPERATION_ACC_OFF:				/**< ACC OFF				*/
		/* 何もしない */
		break;

	default:
		fio_mount_ctrl_sd();
		break;
	}

	return;

}

/****************************************************************************/
/* 関数名称		:	fio_mount_ctrl_sd										*/
/* 機能の説明	:	SDのマウント/アンマウント制御							*/
/* 引数			:	なし													*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/08/18												*/
/* 備考			:															*/
/****************************************************************************/
static void fio_mount_ctrl_sd(void)
{

	E_FS_RESULT	result_fs;
	UI_16		sectPerClst;
	UI_32		freeClst;
	UI_8		i;

	/* SD抜き差しチェック(抜かれた時の処理を先に実行する) */
	if (S_fio_sd_detect_out == N_ON) {		/* SD OUTを検出 */
		S_fio_sd_detect_out = N_OFF;
		/* SDドライブをアンマウントする */
		(void)FileSystem_Unmount((const UI_8 *)FS_DRV_PATH_SD);
		S_fio_drv_info[FIO_DRV_ID_SD].mount = N_FALSE;		/* アンマウント時は最初にマウント状態を更新する */
		S_fio_drv_info[FIO_DRV_ID_SD].free_sect_num = 0;
	}

	if (S_fio_sd_detect_in == N_ON) {		/* SD INを検出 */
		S_fio_sd_detect_in = N_OFF;

		/* SDドライブをマウントする(SD準備中の場合はリトライする) */
		for (i = 0; i < FIO_MOUNT_RETRY_NUM; i++) {
			result_fs = FileSystem_Mount((const UI_8 *)FS_DRV_PATH_SD);
			if (result_fs != FS_RESULT_ERR_NOT_READY) {
				break;
			}
		}
		if (result_fs == FS_RESULT_OK) {	/* マウント成功 */

			/* 空きセクタ数取得 */
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

			S_fio_drv_info[FIO_DRV_ID_SD].mount = N_TRUE;		/* マウント時は最後にマウント状態を更新する */
		}
		else {								/* マウント失敗 */
			/* SDドライブをアンマウントする */
			(void)FileSystem_Unmount((const UI_8 *)FS_DRV_PATH_SD);
			S_fio_drv_info[FIO_DRV_ID_SD].mount = N_FALSE;		/* アンマウント時は最初にマウント状態を更新する */
			S_fio_drv_info[FIO_DRV_ID_SD].free_sect_num = 0;
		}
	}

	S_fio_drv_info[FIO_DRV_ID_SD].mountExecuted = N_TRUE;	/* 最後にマウント実行状態を設定する */

	return;

}

/****************************************************************************/
/* 関数名称		:	fio_path2drvid											*/
/* 機能の説明	:	パス名からドライブIDを取得								*/
/* 引数			:	[I/ ]path	ドライブのパス(パスの後ろにファイル名がついても可)	*/
/* 戻り値		: 	正常	ドライブID（FIO_DRV_ID_xxx）					*/
/*				    異常	FIO_DRV_ID_INVALID								*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/10/07												*/
/* 備考			:															*/
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
/* 関数名称		:	fio_sdc_dtct_cb											*/
/* 機能の説明	:	SDカードIN/OUT検出コールバック							*/
/* 引数			:	[I/ ]portlev	ポートレベル							*/
/*				:				IO_PI_LEV_ACTIVE_SD_DTCT : SDカードIN		*/
/*				:				IO_PI_LEV_NONACT_SD_DTCT : SDカードOUT		*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/08/18												*/
/* 備考			:															*/
/****************************************************************************/
static void fio_sdc_dtct_cb( UI_8 portlev )
{

	if (portlev == IO_PI_LEV_ACTIVE_SD_DTCT) {	/* SDカードIN		*/
		S_fio_sd_detect_in = N_ON;
	}
	else {										/* SDカードOUT		*/
		S_fio_sd_detect_out = N_ON;
	}
	return;

}

/****************************************************************************/
/* 関数名称		:	fio_strcpy												*/
/* 機能の説明	:	文字列コピー											*/
/* 引数			:	[ /O]dstBuff		コピー先バッファ					*/
/*				:	[I/ ]dstBuffSize	コピー先バッファサイズ				*/
/*				:	[I/ ]srcBuff		コピー元バッファ					*/
/* 戻り値		: 	なし													*/
/* 作成者		:	(OFT)Toguchi											*/
/* 作成日		:	16/08/18												*/
/* 備考			:															*/
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




