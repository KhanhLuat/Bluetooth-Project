/************************************************************************************************/
/* �q�於		:	�W�����W���[��																*/
/* �@�햼		:	SSFT																		*/
/* ϲ�ݿ��ðϖ�	:	PF																			*/
/*==============================================================================================*/
/* �쐬̧�ٖ�	:	SSFTSTD_FR81_ReloadTimer_Drv.h												*/
/* 				:	�۰�������ײ��ͯ��															*/
/*==============================================================================================*/
/* �Ώ�ϲ��		:	MB91570 Series																*/
/*==============================================================================================*/
/* �쐬�ް�ޮ�	:	020201																		*/
/* �쐬�N����	:	2015.12.09																	*/
/* �쐬��		:	R.Hiroi																		*/
/*----------------------------------------------------------------------------------------------*/
/* �ύX����		:																				*/
/* [010101]		:	�V�K�쐬																	*/
/*				:	J36A�Ή��B���W�X�^�ݒ�l�ύX�B												*/
/* [010102]		:	�W��IF�Ή��B�N����ʂ̖��̕ύX												*/
/*				:	�W��IF�Ή��̎w�E������													*/
/* 2013.11.19	:	�A���_�[�t���[�t���O�擾IF�ǉ�												*/
/* [020101]		:	��L�u�A���_�[�t���[�t���O�擾IF�ǉ��v�𐳎���VM�o�^						*/
/* [020201]		:	�^�C�}�l�擾�֐��̏C��														*/
/************************************************************************************************/
#ifndef __MB91570_RELOADTIMER_DRV_H__
#define __MB91570_RELOADTIMER_DRV_H__

/*** START_INC ***/
/********************************************************************************/
/*   Include File                                                               */
/*------------------------------------------------------------------------------*/
/*      ͯ�ް̧�ق̲ݸٰ�ޕ��́A���Ļ�قɋL�ڂ��邱��                          */
/********************************************************************************/
#include "SSFTxxx_FR81_ReloadTimer_Drv_Config.h"

/*==============================================================================*/
/*	typedef��`�i�O�����J�j														*/
/*==============================================================================*/
typedef UI_16	T_ReloadTimeDrv_Cnt;	/* �}�C�R���ɍ��킹�� T_ReloadTimeDrv_Cnt���`     */
										/* FR81�}�C�R���̃V���O�����[�h�́AUI_16�Ƃ��Ďg�p  */
typedef UI_32	T_ReloadTimeDrv_ReloadRegAddr;
										/* �}�C�R���ɍ��킹�� T_ReloadTimeDrv_ReloadRegAddr���`     */
										/* FR81�}�C�R���ł́AUI_32�Ƃ��Ďg�p  */

/*==============================================================================*/
/*	�萔��`�i�O�����J�j														*/
/*==============================================================================*/
#ifdef	_VERSION_CONST_DEFINE
UI_8 const C_RELOADTIMERDRV_VER[6] = {		/* �����[�h�^�C�}�h���C�o �h���C�o�o�[�W����No(020201) */
	0x00,							/* xx"0" */
	0x02,							/* xx"2" */
	0x00,							/* yy"0" */
	0x02,							/* yy"2" */
	0x00,							/* zz"0" */
	0x01,							/* zz"1" */
};
#endif	/*	_VERSION_CONST_DEFINE 	*/
/*==============================================================================*/
/*	�֐��������ߐ錾�i�O�����J�j												*/
/*==============================================================================*/

extern void ReloadTmDrv_Ctrl( E_RELOADTIMERDRV_CH ch, T_ReloadTimeDrv_Cnt cycle, UI_8 act );
extern void ReloadTmDrv_ReqUnderFlowClr( E_RELOADTIMERDRV_CH ch );
extern UI_8 ReloadTmDrv_ReqUnderFlowGet( E_RELOADTIMERDRV_CH ch );
extern T_ReloadTimeDrv_Cnt ReloadTmDrv_GetTm( E_RELOADTIMERDRV_CH ch );

extern void ReloadTmDrv_Init( E_INIT_TYPE req );
extern void ReloadTmDrv_Sleep( void );
extern void ReloadTmDrv_Refresh( void );
extern UI_8 ReloadTmDrv_GetTmCntEnaStat( E_RELOADTIMERDRV_CH ch );
extern T_ReloadTimeDrv_ReloadRegAddr ReloadTmDrv_GetReloadRegAddr( E_RELOADTIMERDRV_CH ch );

#endif	/*	__MB91570_RELOADTIMER_DRV_H__ */

