/************************************************************************************************/
/* �q�於		:	�W�����W���[��																*/
/* �@�햼		:	SSFT																		*/
/* ϲ�ݿ��ðϖ�	:	PF																			*/
/*==============================================================================================*/
/* �쐬̧�ٖ�	:	SSFTSTD_FR81_Port_Drv.h														*/
/* 				:	GPIO��ײ��ͯ��																*/
/*==============================================================================================*/
/* �Ώ�ϲ��		:	MB91570 Series																*/
/*==============================================================================================*/
/* �쐬�ް�ޮ�	:	030303																		*/
/* �쐬�N����	:	2015.02.09																	*/
/* �쐬��		:	R.Hiroi																		*/
/*----------------------------------------------------------------------------------------------*/
/* �ύX����		:																				*/
/* [010101]		:	�V�K�쐬																	*/
/* [010102]		:	P127�@�\�؂�ւ��e�[�u���C��												*/
/*				:	�|�[�g�@�\�؂�ւ����̃|�[�g�Ɉ�u�߂�����C��								*/
/* [010103]		:	�W��IF�Ή��BIF�֐��̖߂�l��SI_8���B����E_PORTDRV_BYTE_NUM�ǉ��i�\��݌v�j	*/
/* [010104]		:	�r�b�g�}�X�N�ʒu�̒�`�ӏ��ύX�B�Z�L�����e�B�C���W�P�[�^�̃|�[�g�C���B		*/
/* [010105]		:	EEPROM�A�V�t�g���W�X�^�̃|�[�g��`�ǉ�										*/
/* [030101]		:	ROM�e�ʂƏ������ԍ팸�̂��߁A�R���p�C��SW�ǉ�								*/
/* [030102]		:	�C���N���[�h�\���������B													*/
/* [030103]		:	�K���Ή��B																	*/
/* [030301]		:	���W�X�^�̍Ē�`���ύX�ɂ��h���C�o�o�[�W�����̍X�V						*/
/* [030302]		:	�}�C�R��SLEEP���Ƀ|�[�g�����͐ݒ�(H.I)�ɂȂ��Ă���							*/
/* [030303]		:	���v�h���C�o�ɂ��WOT�o�͑Ή��p�̊O�����Jdefine��`��ǉ�					*/
/************************************************************************************************/
#ifndef __MB91570_PORT_DRV_H__
#define __MB91570_PORT_DRV_H__

/************************************************************************************************/
/*	Include File																				*/
/************************************************************************************************/
#include "SSFTxxx_FR81_Port_Drv_Config.h"		/* GPIO Config�w�b�_ */

/*==============================================================================*/
/*	�萔��`�i�O�����J�j														*/
/*==============================================================================*/

#ifdef	_VERSION_CONST_DEFINE
UI_8 const C_PORTDRV_VER[6] = {	/* GPIO�h���C�o �h���C�o�o�[�W����No(030303) */
	0x00,						/* xx "0" */
	0x03,						/* xx "3" */
	0x00,						/* yy "0" */
	0x03,						/* yy "3" */
	0x00,						/* zz "0" */
	0x03						/* zz "3" */
};
#endif	/*	__MB91570_PORT_DRV_H__ 	*/

/* �r�b�g�}�X�N�ʒu�e�[�u�� */
#ifdef __MB91570_PORT_DRV_INTERNAL__
const static UI_8 C_PortBitMaskPositionTbl[PORTDRV_PORT_MAX] =
{
	(UI_8)PORT_BIT_MASK_0,		/* P0�̃r�b�g�}�X�N�ʒu */
	(UI_8)PORT_BIT_MASK_1,		/* P1�̃r�b�g�}�X�N�ʒu */
	(UI_8)PORT_BIT_MASK_2,		/* P2�̃r�b�g�}�X�N�ʒu */
	(UI_8)PORT_BIT_MASK_3,		/* P3�̃r�b�g�}�X�N�ʒu */
	(UI_8)PORT_BIT_MASK_4,		/* P4�̃r�b�g�}�X�N�ʒu */
	(UI_8)PORT_BIT_MASK_5,		/* P5�̃r�b�g�}�X�N�ʒu */
	(UI_8)PORT_BIT_MASK_6,		/* P6�̃r�b�g�}�X�N�ʒu */
	(UI_8)PORT_BIT_MASK_7,		/* P7�̃r�b�g�}�X�N�ʒu */
	(UI_8)PORT_BIT_MASK_8,		/* P8�̃r�b�g�}�X�N�ʒu */
	(UI_8)PORT_BIT_MASK_9,		/* P9�̃r�b�g�}�X�N�ʒu */
	(UI_8)PORT_BIT_MASK_10,		/* P10�̃r�b�g�}�X�N�ʒu */
	(UI_8)PORT_BIT_MASK_11,		/* P11�̃r�b�g�}�X�N�ʒu */
	(UI_8)PORT_BIT_MASK_12,		/* P12�̃r�b�g�}�X�N�ʒu */
	(UI_8)PORT_BIT_MASK_13		/* P13�̃r�b�g�}�X�N�ʒu */
};
#endif /* __MB91570_PORT_DRV_INTERNAL__ */

/*==============================================================================*/
/*	�֐��������ߐ錾�i�O�����J�j												*/
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

