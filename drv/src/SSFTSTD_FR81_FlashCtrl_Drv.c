/********************************************************************************/
/* 客先名		:	標準モジュール												*/
/* 機種名		:	SSFT														*/
/* ﾏｲｺﾝｿﾌﾄﾃｰﾏ名	:	PF															*/
/*==============================================================================*/
/* 作成ﾌｧｲﾙ名	:	SSFTSTD_FR81_FlashCtrl_Drv.c								*/
/* 				:	ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞ											*/
/*==============================================================================*/
/* 対象ﾏｲｺﾝ		:	MB91570 Series												*/
/*==============================================================================*/
/* 作成ﾊﾞｰｼﾞｮﾝ	:	010101														*/
/* 作成年月日	:	13/11/20(水) 15:21:53										*/
/* 作成者		:	Litong														*/
/*------------------------------------------------------------------------------*/
/* 変更履歴		:																*/
/* [020101]		:	フラッシュ領域のサイズ以上の消去要求が来た際に、領域外		*/
/* 				:	アクセスをしてしまう不具合の修正							*/
/********************************************************************************/

/*** START_INC ***/
/************************************************************************************************/
/*   Include File                                                                               */
/*----------------------------------------------------------------------------------------------*/
/*      ﾍｯﾀﾞｰﾌｧｲﾙのｲﾝｸﾙｰﾄﾞ文は、下記ﾌｧｲﾙに記載すること                                          */
/************************************************************************************************/
#include "SSFTSTD_Type.h"
#include "SSFTSTD_Macro.h"

#define __MB91570_FLASHCTRL_DRV_INTERNAL__
#include "SSFTSTD_FR81_FlashCtrl_Drv.h"

/*==============================================================================*/
/*	typedef定義（外部非公開）													*/
/*==============================================================================*/
/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞの内部関数の型 */
typedef SI_8 (*P_FlashCtrlDrvActFunc)(void);	/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞの内部処理関数	*/
typedef SI_8 (*P_FlashCtrlDrvJdgFunc)(void);	/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞの内部判定関数	*/

/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙ情報構造体 */
typedef struct t_flashctrldrv_t {
	E_FLASHCTRLDRV_STATE	flashctrldrv_state;	/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞｽﾃｰﾄ		*/
	E_FLASHDRV_STATE		flashdrv_status;	/* ﾌﾗｯｼｭ書き換えﾄﾞﾗｲﾊﾞｽﾃｰﾀｽ		*/
	T_FlashDrv_Addr			access_addr;		/* 書き込み/消去ｱﾄﾞﾚｽ			*/
	T_FlashDrv_Addr			access_end_addr;	/* 書き込み/消去終了ｱﾄﾞﾚｽ		*/
	const UI_8				*write_area;		/* 書き込みﾃﾞｰﾀの格納先			*/
	UI_8					blknum;				/* ﾌﾞﾛｯｸ番号					*/
}T_FlashCtrlDrv;

/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙ動作PCB構造体(Process Control Block) */
typedef struct t_flashctrldrv_pcb_t {
	P_FlashCtrlDrvActFunc	act_func;			/* 書き込み/消去処理関数		*/
	P_FlashCtrlDrvJdgFunc	jdg_func;			/* 書き込み/消去完了判定関数	*/
	E_FLASHCTRLDRV_STATE	err_state;			/* 書き込み/消去失敗状態		*/
}T_FlashCtrlDrv_PCB;

/*==============================================================================*/
/*	内部マクロ																	*/
/*==============================================================================*/
/* なし */

/*==============================================================================*/
/*	内部変数																	*/
/*==============================================================================*/
static T_FlashCtrlDrv	FlashCtrlDrv_Info;			/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙ情報			*/

/*==============================================================================*/
/*	内部定数																	*/
/*==============================================================================*/
/* なし */

/*==============================================================================*/
/*	関数ﾌﾟﾛﾄﾀｲﾌﾟ宣言(外部非公開)												*/
/*==============================================================================*/
static SI_8 flash_ctrl_drv_write( void );
static SI_8 flash_ctrl_drv_write_complete( void );
static SI_8 flash_ctrl_drv_erase( void );
static SI_8 flash_ctrl_drv_erase_complete( void );
static SI_8 flash_ctrl_drv_wait_chk( void );

/*==============================================================================*/
/*	定数定義																	*/
/*==============================================================================*/
/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞ動作PCBﾃｰﾌﾞﾙ */
static const T_FlashCtrlDrv_PCB C_FlashCtrlDrvPcbTbl[] =
{
	/*	処理関数				完了判定関数					失敗状態					*/

	/* 	ﾇﾙ						ﾇﾙ								ｱｲﾄﾞﾙ						 */
	{	D_NULL,					D_NULL,							E_FLASHCTRLDRV_IDLE	},
	/* 	書き込み処理			書き込み完了判定処理			書き込み失敗				 */
	{	&flash_ctrl_drv_write,	&flash_ctrl_drv_write_complete,	E_FLASHCTRLDRV_WRITE_ERROR	},
	/* 	消去処理				消去完了判定処理				消去失敗					 */
	{	&flash_ctrl_drv_erase,	&flash_ctrl_drv_erase_complete,	E_FLASHCTRLDRV_ERASE_ERROR	},
	/* 	ﾇﾙ						ﾇﾙ								ｱｲﾄﾞﾙ						 */
	{	D_NULL,					D_NULL,							E_FLASHCTRLDRV_IDLE	},
	/* 	ﾇﾙ						ﾇﾙ								ｱｲﾄﾞﾙ						 */
	{	D_NULL,					D_NULL,							E_FLASHCTRLDRV_IDLE	}
};

/****************************************************************************************/
/*	[モジュール名]	FlashCtrlDrv_Init			[名称]	ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞ初期化処理		*/
/*======================================================================================*/
/*	[処理概要]	ﾌﾗｯｼｭｺﾝﾄﾛｰﾙ情報の初期化処理を行う。										*/
/*======================================================================================*/
/*	[記述形式]	void FlashCtrlDrv_Init( E_INIT_TYPE req )								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	E_INIT_TYPE				req		：起動種別								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void FlashCtrlDrv_Init( E_INIT_TYPE req )
{
	
	switch ( req ) {
	case E_INIT_RESET:			/* CPUﾘｾｯﾄによる起動 */
	case E_INIT_WAKEUP:			/* 省電力解除（WAKEUP）による起動 */
		/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙ情報の初期設定 */
		FlashCtrlDrv_Info.flashctrldrv_state = E_FLASHCTRLDRV_IDLE;
		FlashCtrlDrv_Info.access_addr = 0U;
		FlashCtrlDrv_Info.access_end_addr = 0U;
		FlashCtrlDrv_Info.write_area = 0U;
		FlashCtrlDrv_Info.blknum = 0U;
		FlashCtrlDrv_Info.flashdrv_status = FlashDrv_CheckStatus();
		
#ifdef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
		(void)FlashDrv_Enable();
#endif

		break;
	case E_INIT_IGN_ON:			/* IGN ON */
		break;
	case E_INIT_RET_NORMAL_VOL:	/* 低電圧復帰 */
		break;
	case E_INIT_INTERVAL_WAKEUP:	/* 間欠起動ｳｪｲｸｱｯﾌﾟ */
		break;
	default:					/* 上記以外 */
		break;
	}
	
}

/****************************************************************************************/
/*	[モジュール名]	FlashCtrlDrv_Write			[名称]	ﾌﾗｯｼｭ書き込み要求受付			*/
/*======================================================================================*/
/*	[処理概要]	指定されたｱﾄﾞﾚｽへのﾃﾞｰﾀ書き込み要求を受付ける。							*/
/*======================================================================================*/
/*	[記述形式]	SI_8 FlashCtrlDrv_Write( T_FlashDrv_Addr write_addr,					*/
/*										 UI_8 *write_data, UI_16 data_size )			*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	T_FlashDrv_Addr		write_addr	：書き込みｱﾄﾞﾚｽ							*/
/*				UI_8				*write_data	：書き込みﾃﾞｰﾀ							*/
/*				UI_16				data_size	：書き込むﾃﾞｰﾀのｻｲｽﾞ					*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8				ret			：書き込み受付成功(D_OK)				*/
/*												：書き込みbusy/範囲外(D_NG)				*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
SI_8 FlashCtrlDrv_Write( T_FlashDrv_Addr write_addr, const UI_8 *write_data, UI_16 data_size )
{
	SI_8	ret = D_NG;

	/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙ情報:書き込み中、もしくは消去中ではない状態 */
	if ( ( FlashCtrlDrv_Info.flashctrldrv_state != E_FLASHCTRLDRV_WRITE ) &&
		( FlashCtrlDrv_Info.flashctrldrv_state != E_FLASHCTRLDRV_ERASE ) ) {
		/* 書き込み要求を受付して、ﾌﾗｯｼｭｺﾝﾄﾛｰﾙ情報を更新する */
		FlashCtrlDrv_Info.access_addr = write_addr;
		FlashCtrlDrv_Info.access_end_addr = write_addr + data_size;
		FlashCtrlDrv_Info.write_area = write_data;
		FlashCtrlDrv_Info.flashctrldrv_state = E_FLASHCTRLDRV_WRITE;

		/* ﾌﾗｯｼｭ書き込み要求の受付成功 */
		ret = D_OK;
	}

	return ret;
}

/****************************************************************************************/
/*	[モジュール名]	FlashCtrlDrv_Erase			[名称]	ﾌﾗｯｼｭ消去要求受付				*/
/*======================================================================================*/
/*	[処理概要]	指定されたｱﾄﾞﾚｽのﾃﾞｰﾀ消去要求を受付ける。								*/
/*======================================================================================*/
/*	[記述形式]	SI_8 FlashCtrlDrv_Erase( T_FlashDrv_Addr erase_addr, UI_32 data_size )	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	T_FlashDrv_Addr		erase_addr	：消去ｱﾄﾞﾚｽ								*/
/*				UI_32				data_size	：消去ｻｲｽﾞ								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8				ret			：消去受付成功(D_OK)					*/
/*												：消去busy/範囲外(D_NG)					*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
SI_8 FlashCtrlDrv_Erase( T_FlashDrv_Addr erase_addr, UI_32 data_size )
{
	SI_8	ret = D_NG;
	UI_8	count;

	/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙ情報:書き込み中、もしくは消去中ではない状態 */
	if ( ( FlashCtrlDrv_Info.flashctrldrv_state != E_FLASHCTRLDRV_WRITE ) &&
		( FlashCtrlDrv_Info.flashctrldrv_state != E_FLASHCTRLDRV_ERASE ) ) {
		/* 消去ﾌﾞﾛｯｸﾁｪｯｸ */
		for ( count = 0U; count < FLASH_MAX_BLKNUM; count++ ) {
			/* 消去ﾌﾞﾛｯｸ開始ｱﾄﾞﾚｽが属するﾌﾞﾛｯｸ番号を取得する */
			if ( erase_addr == C_FlashBlkTbl[count].start_addr ) {
				/* 消去要求を受付して、ﾌﾗｯｼｭｺﾝﾄﾛｰﾙ情報を更新する */
				FlashCtrlDrv_Info.access_addr = erase_addr;
				FlashCtrlDrv_Info.access_end_addr = ( erase_addr + data_size ) - 1U;
				FlashCtrlDrv_Info.blknum = count;
				FlashCtrlDrv_Info.flashctrldrv_state = E_FLASHCTRLDRV_ERASE;

				/* ﾌﾗｯｼｭ消去要求の受付成功 */
				ret = D_OK;

				break;
			}
		}
	}

	return ret;
}

/****************************************************************************************/
/*	[モジュール名]	FlashCtrlDrv_CheckStatus	[名称]	ﾌﾗｯｼｭ要求ﾁｪｯｸ処理				*/
/*======================================================================================*/
/*	[処理概要]	受付けた書き込み/消去要求のﾁｪｯｸ処理を行う。								*/
/*======================================================================================*/
/*	[記述形式]	E_FLASHCTRLDRV_STATE FlashCtrlDrv_CheckStatus( void )					*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	E_FLASHCTRLDRV_STATE	ret	：											*/
/*				ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞのｽﾃｰﾀｽ	：ｱｲﾄﾞﾙ(E_FLASHCTRLDRV_IDLE)				*/
/*											：書き込み中(E_FLASHCTRLDRV_WRITE)			*/
/*											：消去中(E_FLASHCTRLDRV_ERASE)				*/
/*											：書き込み失敗(E_FLASHCTRLDRV_WRITE_ERROR)	*/
/*											：消去失敗(E_FLASHCTRLDRV_ERASE_ERROR)		*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
E_FLASHCTRLDRV_STATE FlashCtrlDrv_CheckStatus( void )
{
	return FlashCtrlDrv_Info.flashctrldrv_state;
}

/****************************************************************************************/
/*	[モジュール名]	FlashCtrlDrv_Proc	[名称]		ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞﾒｲﾝ処理			*/
/*======================================================================================*/
/*	[処理概要]	ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞﾒｲﾝ処理を行う。										*/
/*======================================================================================*/
/*	[記述形式]	void FlashCtrlDrv_Proc( void )											*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	無し																	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
void FlashCtrlDrv_Proc( void )
{
#ifndef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
	UI_16					mask_level;						/* 割り込みﾏｽｸﾚﾍﾞﾙ			*/
#endif
	SI_8					sub_ret;						/* 呼び出し関数の戻り値		*/

	switch ( FlashCtrlDrv_Info.flashctrldrv_state ) {
	case E_FLASHCTRLDRV_WRITE:								/* ﾌﾗｯｼｭ書き込み要求実行	*/
	case E_FLASHCTRLDRV_ERASE:								/* ﾌﾗｯｼｭ消去要求実行		*/

#ifndef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
		/* 現在の割り込みﾚﾍﾞﾙを取得し、割り込み禁止にする */
		(void)IntrDrv_GetMaskLevel( &mask_level );
		(void)IntrDrv_SetMaskLevel( FLASH_ACCESS_INTR_LEVEL );
#endif
		
		/* 書き込み/消去を実行 */
		sub_ret = C_FlashCtrlDrvPcbTbl[FlashCtrlDrv_Info.flashctrldrv_state].act_func();

#ifndef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
		/* 割り込みﾚﾍﾞﾙを復帰する */
		(void)IntrDrv_SetMaskLevel( mask_level );
#endif

		/* 書き込み/消去が完了の処理 */
		if ( sub_ret == D_OK ) {

			/* 書き込み/消去が成功した場合は、次回の書き込み/消去要求を準備する */
			sub_ret = C_FlashCtrlDrvPcbTbl[FlashCtrlDrv_Info.flashctrldrv_state].jdg_func();
			if ( sub_ret == D_OK ) {

				/* 全ての書き込み/消去が終了した場合は、完了とする */
				FlashCtrlDrv_Info.flashctrldrv_state = E_FLASHCTRLDRV_IDLE;
			}
		} else {
			/* 書き込み/消去が失敗した場合は、エラーとする */
			FlashCtrlDrv_Info.flashctrldrv_state = C_FlashCtrlDrvPcbTbl[FlashCtrlDrv_Info.flashctrldrv_state].err_state;
		}

		break;
	default:									/* 上記以外 */
		break;	
	}
}

/****************************************************************************************/
/*	[モジュール名]	flash_ctrl_drv_write	[名称]	ﾄﾞﾗｲﾊﾞﾒｲﾝ処理の書き込み処理			*/
/*======================================================================================*/
/*	[処理概要]	ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞﾒｲﾝ処理の書き込み処理を行う。							*/
/*======================================================================================*/
/*	[記述形式]	static SI_8 flash_ctrl_drv_write( void )								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8		write_result	:正常(D_OK)、異常(D_NG)を返す				*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static SI_8 flash_ctrl_drv_write( void )
{
	SI_8					write_result = D_NG;
	SI_8					ret;
	SI_8					sub_ret;

#ifndef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
	/* ﾌﾗｯｼｭのﾌﾟﾛｸﾞﾗﾐﾝｸﾞﾓｰﾄﾞにする */
	ret = FlashDrv_Enable();
	if ( ret == D_OK ) {
#endif
		/* ﾌﾗｯｼｭﾄﾞﾗｲﾊﾞへ前半のﾊｰﾌﾜｰﾄﾞ書き込み要求を発行する */
		sub_ret = FlashDrv_Write( FlashCtrlDrv_Info.access_addr, FlashCtrlDrv_Info.write_area, D_FLASHDRV_DATASIZE );
		if ( sub_ret == D_OK ) {
			/* 書き込みが完了するまで、チェックを実施する*/
			write_result = flash_ctrl_drv_wait_chk();
		}

		if ( write_result == D_OK ) {
			/* 次回書き込みのｱﾄﾞﾚｽとﾃﾞｰﾀｱﾄﾞﾚｽを算出する */
			FlashCtrlDrv_Info.access_addr += D_FLASHDRV_DATASIZE;
			FlashCtrlDrv_Info.write_area += D_FLASHDRV_DATASIZE;

			/* ﾌﾗｯｼｭﾄﾞﾗｲﾊﾞへ後半のﾊｰﾌﾜｰﾄﾞ書き込み要求を発行する */
			sub_ret = FlashDrv_Write( FlashCtrlDrv_Info.access_addr, FlashCtrlDrv_Info.write_area, D_FLASHDRV_DATASIZE );
			if ( sub_ret == D_OK ) {
				/* 書き込みが完了するまで、チェックを実施する*/
				write_result = flash_ctrl_drv_wait_chk();
			} else {
				write_result = D_NG;
			}
		}
#ifndef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
		/* ﾌﾗｯｼｭをCPUﾓｰﾄﾞにする */
		ret = FlashDrv_Disable();
		if ( ret != D_OK ) {
			/* CPUモードに復帰できなかった場合は失敗とする */
			write_result = D_NG;
		}
	}
#endif

	return write_result;
}

/****************************************************************************************/
/*	[モジュール名]	flash_ctrl_drv_write_complete[名称]	ﾄﾞﾗｲﾊﾞﾒｲﾝ処理の書き込み完了判定	*/
/*======================================================================================*/
/*	[処理概要]	ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞﾒｲﾝ処理の書き込み完了判定を行う						*/
/*======================================================================================*/
/*	[記述形式]	static SI_8 flash_ctrl_drv_write_complete(void)							*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8		ret			: 全領域の書換え完了(D_OK)、書換え未完了(D_NG)	*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static SI_8 flash_ctrl_drv_write_complete( void )
{
	SI_8					ret = D_NG;

	/* 次回書き込みのｱﾄﾞﾚｽを算出する */
	FlashCtrlDrv_Info.access_addr += D_FLASHDRV_DATASIZE;

	/* 書き込み要求ｻｲｽﾞ完了をﾁｪｯｸする */
	if ( FlashCtrlDrv_Info.access_addr < FlashCtrlDrv_Info.access_end_addr ) {
		/* 次回書き込みﾃﾞｰﾀｱﾄﾞﾚｽを算出する */
		FlashCtrlDrv_Info.write_area += D_FLASHDRV_DATASIZE;
	} else {
		/* 書き込みが完了した場合はOKを返す */
		ret = D_OK;
	}
	
	return ret;
}

/****************************************************************************************/
/*	[モジュール名]	flash_ctrl_drv_erase	[名称]	ﾄﾞﾗｲﾊﾞﾒｲﾝ処理の消去処理				*/
/*======================================================================================*/
/*	[処理概要]	ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞﾒｲﾝ処理の消去処理を行う								*/
/*======================================================================================*/
/*	[記述形式]	static SI_8 flash_ctrl_drv_erase( void )								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8		erase_result	:正常(D_OK)、異常(D_NG)を返す				*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static SI_8 flash_ctrl_drv_erase( void )
{
	SI_8					erase_result = D_NG;
	SI_8					ret;
#ifndef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
	/* ﾌﾗｯｼｭのﾌﾟﾛｸﾞﾗﾐﾝｸﾞﾓｰﾄﾞにする */
	ret = FlashDrv_Enable();

	if ( ret == D_OK ) {
		/* ﾌﾗｯｼｭﾄﾞﾗｲﾊﾞへﾌﾞﾛｯｸ消去要求を設定する */
		ret = FlashDrv_Erase( FlashCtrlDrv_Info.access_addr );
		if ( ret == D_OK ) {
			/* 消去が完了するまで、チェックを実施する */
			erase_result = flash_ctrl_drv_wait_chk();
		}
		/* ﾌﾗｯｼｭをCPUﾓｰﾄﾞにする */
		ret = FlashDrv_Disable();
		if ( ret != D_OK ) {
			/* CPUモードに復帰できなかった場合は失敗とする */
			erase_result = D_NG;
		}
	}
#else
	UI_16					mask_level;
	
	/* 現在の割り込みﾚﾍﾞﾙを取得し、割り込み禁止にする */
	(void)IntrDrv_GetMaskLevel( &mask_level );
	(void)IntrDrv_SetMaskLevel( FLASH_ACCESS_INTR_LEVEL );
	/* ﾌﾗｯｼｭﾄﾞﾗｲﾊﾞへﾌﾞﾛｯｸ消去要求を設定する */
	ret = FlashDrv_Erase( FlashCtrlDrv_Info.access_addr );
	/* 割り込みﾚﾍﾞﾙを復帰する */
	(void)IntrDrv_SetMaskLevel( mask_level );
	if ( ret == D_OK ) {
		/* 消去が完了するまで、チェックを実施する */
		erase_result = flash_ctrl_drv_wait_chk();
	}
#endif
	return erase_result;
}

/****************************************************************************************/
/*	[モジュール名]	flash_ctrl_drv_erase_complete	[名称]ﾄﾞﾗｲﾊﾞﾒｲﾝ処理の消去終了判定	*/
/*======================================================================================*/
/*	[処理概要]	ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞﾒｲﾝ処理の消去終了判定を行う。							*/
/*======================================================================================*/
/*	[記述形式]	static SI_8 flash_ctrl_drv_erase_complete( void )						*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8		ret			: 全領域の消去完了(D_OK)、消去未完了(D_NG)		*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static SI_8 flash_ctrl_drv_erase_complete( void )
{
	SI_8					ret = D_NG;

	if ( FlashCtrlDrv_Info.access_end_addr <= C_FlashBlkTbl[FlashCtrlDrv_Info.blknum].end_addr ) {
		/* 全ての消去が完了した場合はOKを返す */
		ret = D_OK;
	} else {
		/* 次回消去ﾌﾞﾛｯｸ番号を算出する */
		FlashCtrlDrv_Info.blknum++;

		if ( FlashCtrlDrv_Info.blknum < FLASH_MAX_BLKNUM ) {
			/* 全ての消去が未完、次の消去ﾌﾞﾛｯｸ開始ｱﾄﾞﾚｽを取得する */
			FlashCtrlDrv_Info.access_addr = C_FlashBlkTbl[FlashCtrlDrv_Info.blknum].start_addr;
		} else {
			/* ﾌﾗｯｼｭﾒﾓﾘの範囲を超える場合は消去終了とする */
			ret = D_OK;
		}
	}

	return ret;
}

/****************************************************************************************/
/*	[モジュール名]	flash_ctrl_drv_wait_chk		[名称]消去/書き込み処理完了判定			*/
/*======================================================================================*/
/*	[処理概要]	ﾌﾗｯｼｭﾄﾞﾗｲﾊﾞの消去/書き込み処理完了判定を行う。							*/
/*======================================================================================*/
/*	[記述形式]	static SI_8 flash_ctrl_drv_wait_chk( void )								*/
/*--------------------------------------------------------------------------------------*/
/*	[ 引  数 ]	無し																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ 戻り値 ]	SI_8		ret			: 正常終了(D_OK)、異常終了(D_NG)を返す			*/
/*======================================================================================*/
/*	[ 備  考 ]	無し																	*/
/****************************************************************************************/
static SI_8 flash_ctrl_drv_wait_chk( void )
{
	E_FLASHDRV_STATE		status;							/* ﾌﾗｯｼｭﾄﾞﾗｲﾊﾞｽﾃｰﾀｽ */
	UI_32					overflow_count = 0U;			/* ｵｰﾊﾞｰﾌﾛｰ検出用ｶｳﾝﾀ */
	UI_8					ovf_info = D_OFF;				/* ｵｰﾊﾞｰﾌﾛｰ情報 */
	SI_8					ret = D_NG;

	do {
#ifndef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
		WdtDrv_ReqClr();				/* S/W WDT ｸﾘｱ		 */
		WdtDrv_ReqClrCpu();				/* H/W WDT ｸﾘｱ		 */
		HWWdtDrv_ReqCrl();				/* 外部WDT(HWWDT)ｸﾘｱ */
#endif

		/* ﾌﾗｯｼｭﾄﾞﾗｲﾊﾞの状態を取得する */
		status = FlashDrv_CheckStatus();
		switch ( status ) {
		case E_FLASHDRV_IDLE:
			/* 処理が完了した場合 */
			ovf_info = D_ON;							/* 処理終了で				*/
			ret = D_OK;									/* 正常終了とする			*/
			break;
		case E_FLASHDRV_WRITE:
			/* 書き込み中の場合 */
			if ( overflow_count <= FLASH_WAIT_TM_FOR_WRITE ) {
				D_NOP10();								/* 10サイクルウエイトする	*/
				++overflow_count;
			} else {
				ovf_info = D_ON;						/* 処理終了で				*/
				ret = D_NG;								/* 異常終了とする			*/
			}
			break;
		case E_FLASHDRV_ERASE:
			/* 消去中の場合 */
			if ( overflow_count <= FLASH_WAIT_TM_FOR_ERASE ) {
				D_NOP10();								/* 10サイクルウエイトする	*/
				++overflow_count;
			} else {
				ovf_info = D_ON;						/* 処理終了で				*/
				ret = D_NG;								/* 異常終了とする			*/
			}
			break;
		case E_FLASHDRV_WRITE_ERROR:
		case E_FLASHDRV_ERASE_ERROR:
		default:
			/* 異常が完了した場合 */
			ovf_info = D_ON;							/* 処理終了で				*/
			ret = D_NG;									/* 異常終了とする			*/
			break;
		}
		
	} while ( ovf_info == D_OFF );

	return ret;
}
