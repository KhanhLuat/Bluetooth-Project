/************************************************************************************************/
/* 客先名		:	標準モジュール																*/
/* 機種名		:	SSFT																		*/
/* ﾏｲｺﾝｿﾌﾄﾃｰﾏ名	:	PF																			*/
/*==============================================================================================*/
/* 作成ﾌｧｲﾙ名	:	SSFTSTD_FR81_FlashCtrl_Drv.h												*/
/* 				:	ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞﾍｯﾀﾞ														*/
/*==============================================================================================*/
/* 対象ﾏｲｺﾝ		:	MB91570 Series																*/
/*==============================================================================================*/
/* 作成ﾊﾞｰｼﾞｮﾝ	:	010101																		*/
/* 作成年月日	:	13/11/20(水) 15:22:06														*/
/* 作成者		:	Litong																		*/
/*----------------------------------------------------------------------------------------------*/
/* 変更履歴		:																				*/
/* [020101]		:	フラッシュ領域のサイズ以上の消去要求が来た際に、領域外						*/
/* 				:	アクセスをしてしまう不具合の修正											*/
/************************************************************************************************/

#ifndef __MB91570_FLASHCTRL_DRV_H__
#define __MB91570_FLASHCTRL_DRV_H__

#include "SSFTSTD_FR81_Flash_Drv.h"

/*==============================================================================*/
/*	外部定数																	*/
/*==============================================================================*/
#ifdef	_VERSION_CONST_DEFINE
UI_8 const C_FLASHCTRLDRV_VER[6] = {	/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞﾊﾞｰｼﾞｮﾝ No(020301) */
	0x00U,								/* xx "0" */
	0x02U,								/* xx "2" */
	0x00U,								/* yy "0" */
	0x03U,								/* yy "3" */
	0x00U,								/* zz "0" */
	0x01U								/* zz "1" */
};
#endif	/*	_VERSION_CONST_DEFINE 	*/

/*==============================================================================*/
/*	typedef定義（外部公開）														*/
/*==============================================================================*/
/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞのｽﾃｰﾀｽのenum列 */
typedef enum e_flashctrldrv_state{
	E_FLASHCTRLDRV_IDLE			= 0	,		/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞのｽﾃｰﾀｽ:ｱｲﾄﾞﾙ			 */
	E_FLASHCTRLDRV_WRITE			,		/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞのｽﾃｰﾀｽ:書き込み中		 */
	E_FLASHCTRLDRV_ERASE			,		/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞのｽﾃｰﾀｽ:消去中			 */
	E_FLASHCTRLDRV_WRITE_ERROR		,		/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞのｽﾃｰﾀｽ:書き込み失敗	 */
	E_FLASHCTRLDRV_ERASE_ERROR				/* ﾌﾗｯｼｭｺﾝﾄﾛｰﾙﾄﾞﾗｲﾊﾞのｽﾃｰﾀｽ:消去失敗		 */
}E_FLASHCTRLDRV_STATE;

/*==============================================================================*/
/*	関数ﾌﾟﾛﾄﾀｲﾌﾟ宣言（外部公開）												*/
/*==============================================================================*/
extern void FlashCtrlDrv_Init( E_INIT_TYPE req );
extern SI_8 FlashCtrlDrv_Write( T_FlashDrv_Addr write_addr, const UI_8 *write_data, UI_16 data_size );
extern SI_8 FlashCtrlDrv_Erase( T_FlashDrv_Addr erase_addr, UI_32 data_size );
extern void FlashCtrlDrv_Proc( void );
extern E_FLASHCTRLDRV_STATE FlashCtrlDrv_CheckStatus( void );

#endif /* __MB91570_FLASHCTRL_DRV_H__ */
