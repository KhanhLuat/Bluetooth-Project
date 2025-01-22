/************************************************************************************************/
/* 客先名		:	標準モジュール																*/
/* 機種名		:	SSFT																		*/
/* ﾏｲｺﾝｿﾌﾄﾃｰﾏ名	:	PF																			*/
/*==============================================================================================*/
/* 作成ﾌｧｲﾙ名	:	SSFTSTD_FR81_Port_Drv.h														*/
/* 				:	GPIOﾄﾞﾗｲﾊﾞﾍｯﾀﾞ																*/
/*==============================================================================================*/
/* 対象ﾏｲｺﾝ		:	MB91570 Series																*/
/*==============================================================================================*/
/* 作成ﾊﾞｰｼﾞｮﾝ	:	030303																		*/
/* 作成年月日	:	2015.02.09																	*/
/* 作成者		:	R.Hiroi																		*/
/*----------------------------------------------------------------------------------------------*/
/* 変更履歴		:																				*/
/* [010101]		:	新規作成																	*/
/* [010102]		:	P127機能切り替えテーブル修正												*/
/*				:	ポート機能切り替え時のポートに一瞬戻す動作修正								*/
/* [010103]		:	標準IF対応。IF関数の戻り値をSI_8化。引数E_PORTDRV_BYTE_NUM追加（予約設計）	*/
/* [010104]		:	ビットマスク位置の定義箇所変更。セキュリティインジケータのポート修正。		*/
/* [010105]		:	EEPROM、シフトレジスタのポート定義追加										*/
/* [030101]		:	ROM容量と処理時間削減のため、コンパイルSW追加								*/
/* [030102]		:	インクルード構成見直し。													*/
/* [030103]		:	規則対応。																	*/
/* [030301]		:	レジスタの再定義名変更によるドライババージョンの更新						*/
/* [030302]		:	マイコンSLEEP時にポートが入力設定(H.I)になっている							*/
/* [030303]		:	時計ドライバによるWOT出力対応用の外部公開define定義を追加					*/
/************************************************************************************************/
#ifndef __MB91570_PORT_DRV_H__
#define __MB91570_PORT_DRV_H__

/************************************************************************************************/
/*	Include File																				*/
/************************************************************************************************/
#include "SSFTxxx_FR81_Port_Drv_Config.h"		/* GPIO Configヘッダ */

/*==============================================================================*/
/*	定数定義（外部公開）														*/
/*==============================================================================*/

#ifdef	_VERSION_CONST_DEFINE
UI_8 const C_PORTDRV_VER[6] = {	/* GPIOドライバ ドライババージョンNo(030303) */
	0x00,						/* xx "0" */
	0x03,						/* xx "3" */
	0x00,						/* yy "0" */
	0x03,						/* yy "3" */
	0x00,						/* zz "0" */
	0x03						/* zz "3" */
};
#endif	/*	__MB91570_PORT_DRV_H__ 	*/

/* ビットマスク位置テーブル */
#ifdef __MB91570_PORT_DRV_INTERNAL__
const static UI_8 C_PortBitMaskPositionTbl[PORTDRV_PORT_MAX] =
{
	(UI_8)PORT_BIT_MASK_0,		/* P0のビットマスク位置 */
	(UI_8)PORT_BIT_MASK_1,		/* P1のビットマスク位置 */
	(UI_8)PORT_BIT_MASK_2,		/* P2のビットマスク位置 */
	(UI_8)PORT_BIT_MASK_3,		/* P3のビットマスク位置 */
	(UI_8)PORT_BIT_MASK_4,		/* P4のビットマスク位置 */
	(UI_8)PORT_BIT_MASK_5,		/* P5のビットマスク位置 */
	(UI_8)PORT_BIT_MASK_6,		/* P6のビットマスク位置 */
	(UI_8)PORT_BIT_MASK_7,		/* P7のビットマスク位置 */
	(UI_8)PORT_BIT_MASK_8,		/* P8のビットマスク位置 */
	(UI_8)PORT_BIT_MASK_9,		/* P9のビットマスク位置 */
	(UI_8)PORT_BIT_MASK_10,		/* P10のビットマスク位置 */
	(UI_8)PORT_BIT_MASK_11,		/* P11のビットマスク位置 */
	(UI_8)PORT_BIT_MASK_12,		/* P12のビットマスク位置 */
	(UI_8)PORT_BIT_MASK_13		/* P13のビットマスク位置 */
};
#endif /* __MB91570_PORT_DRV_INTERNAL__ */

/*==============================================================================*/
/*	関数ﾌﾟﾛﾄﾀｲﾌﾟ宣言（外部公開）												*/
/*==============================================================================*/

extern SI_8 PortDrv_SetDataByte( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BYTE_NUM byte_num, UI_8 set_byte );
extern SI_8 PortDrv_SetDataBit( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BIT_NUM bit_num, UI_8 set_bit );
extern SI_8 PortDrv_GetDataByte( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BYTE_NUM byte_num, UI_8 *p_get_data );
extern SI_8 PortDrv_GetDataBit( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BIT_NUM bit_num, UI_8 *p_get_bit );

#if (PORT_DRV_FUNC_CHANGE == 1)
extern SI_8 PortDrv_SetFuncBit( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BIT_NUM bit_num, E_PORT_FUNC_CHANGE_ID set_id );
#endif	/* PORT_DRV_FUNC_CHANGE == 1 */

extern void PortDrv_SetSelfCheckStat(SI_8 stat);
extern SI_8 PortDrv_GetPermissionStat(void);
extern SI_8 PortDrv_SetDirectionBit( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BIT_NUM bit_num, UI_8 set_direction );

extern void PortDrv_Init( E_INIT_TYPE req );
extern void PortDrv_Sleep( void );
extern void PortDrv_Refresh( void );

#endif	/*	__MB91570_PORT_DRV_H__ 	*/

