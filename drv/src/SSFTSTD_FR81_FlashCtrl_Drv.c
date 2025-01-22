/********************************************************************************/
/* �q�於		:	�W�����W���[��												*/
/* �@�햼		:	SSFT														*/
/* ϲ�ݿ��ðϖ�	:	PF															*/
/*==============================================================================*/
/* �쐬̧�ٖ�	:	SSFTSTD_FR81_FlashCtrl_Drv.c								*/
/* 				:	�ׯ�����۰���ײ��											*/
/*==============================================================================*/
/* �Ώ�ϲ��		:	MB91570 Series												*/
/*==============================================================================*/
/* �쐬�ް�ޮ�	:	010101														*/
/* �쐬�N����	:	13/11/20(��) 15:21:53										*/
/* �쐬��		:	Litong														*/
/*------------------------------------------------------------------------------*/
/* �ύX����		:																*/
/* [020101]		:	�t���b�V���̈�̃T�C�Y�ȏ�̏����v���������ۂɁA�̈�O		*/
/* 				:	�A�N�Z�X�����Ă��܂��s��̏C��							*/
/********************************************************************************/

/*** START_INC ***/
/************************************************************************************************/
/*   Include File                                                                               */
/*----------------------------------------------------------------------------------------------*/
/*      ͯ�ް̧�ق̲ݸٰ�ޕ��́A���Ļ�قɋL�ڂ��邱��                                          */
/************************************************************************************************/
#include "SSFTSTD_Type.h"
#include "SSFTSTD_Macro.h"

#define __MB91570_FLASHCTRL_DRV_INTERNAL__
#include "SSFTSTD_FR81_FlashCtrl_Drv.h"

/*==============================================================================*/
/*	typedef��`�i�O������J�j													*/
/*==============================================================================*/
/* �ׯ�����۰���ײ�ނ̓����֐��̌^ */
typedef SI_8 (*P_FlashCtrlDrvActFunc)(void);	/* �ׯ�����۰���ײ�ނ̓��������֐�	*/
typedef SI_8 (*P_FlashCtrlDrvJdgFunc)(void);	/* �ׯ�����۰���ײ�ނ̓�������֐�	*/

/* �ׯ�����۰ُ��\���� */
typedef struct t_flashctrldrv_t {
	E_FLASHCTRLDRV_STATE	flashctrldrv_state;	/* �ׯ�����۰���ײ�޽ð�		*/
	E_FLASHDRV_STATE		flashdrv_status;	/* �ׯ������������ײ�޽ð��		*/
	T_FlashDrv_Addr			access_addr;		/* ��������/�������ڽ			*/
	T_FlashDrv_Addr			access_end_addr;	/* ��������/�����I�����ڽ		*/
	const UI_8				*write_area;		/* ���������ް��̊i�[��			*/
	UI_8					blknum;				/* ��ۯ��ԍ�					*/
}T_FlashCtrlDrv;

/* �ׯ�����۰ٓ���PCB�\����(Process Control Block) */
typedef struct t_flashctrldrv_pcb_t {
	P_FlashCtrlDrvActFunc	act_func;			/* ��������/���������֐�		*/
	P_FlashCtrlDrvJdgFunc	jdg_func;			/* ��������/������������֐�	*/
	E_FLASHCTRLDRV_STATE	err_state;			/* ��������/�������s���		*/
}T_FlashCtrlDrv_PCB;

/*==============================================================================*/
/*	�����}�N��																	*/
/*==============================================================================*/
/* �Ȃ� */

/*==============================================================================*/
/*	�����ϐ�																	*/
/*==============================================================================*/
static T_FlashCtrlDrv	FlashCtrlDrv_Info;			/* �ׯ�����۰ُ��			*/

/*==============================================================================*/
/*	�����萔																	*/
/*==============================================================================*/
/* �Ȃ� */

/*==============================================================================*/
/*	�֐��������ߐ錾(�O������J)												*/
/*==============================================================================*/
static SI_8 flash_ctrl_drv_write( void );
static SI_8 flash_ctrl_drv_write_complete( void );
static SI_8 flash_ctrl_drv_erase( void );
static SI_8 flash_ctrl_drv_erase_complete( void );
static SI_8 flash_ctrl_drv_wait_chk( void );

/*==============================================================================*/
/*	�萔��`																	*/
/*==============================================================================*/
/* �ׯ�����۰���ײ�ޓ���PCBð��� */
static const T_FlashCtrlDrv_PCB C_FlashCtrlDrvPcbTbl[] =
{
	/*	�����֐�				��������֐�					���s���					*/

	/* 	��						��								�����						 */
	{	D_NULL,					D_NULL,							E_FLASHCTRLDRV_IDLE	},
	/* 	�������ݏ���			�������݊������菈��			�������ݎ��s				 */
	{	&flash_ctrl_drv_write,	&flash_ctrl_drv_write_complete,	E_FLASHCTRLDRV_WRITE_ERROR	},
	/* 	��������				�����������菈��				�������s					 */
	{	&flash_ctrl_drv_erase,	&flash_ctrl_drv_erase_complete,	E_FLASHCTRLDRV_ERASE_ERROR	},
	/* 	��						��								�����						 */
	{	D_NULL,					D_NULL,							E_FLASHCTRLDRV_IDLE	},
	/* 	��						��								�����						 */
	{	D_NULL,					D_NULL,							E_FLASHCTRLDRV_IDLE	}
};

/****************************************************************************************/
/*	[���W���[����]	FlashCtrlDrv_Init			[����]	�ׯ�����۰���ײ�ޏ���������		*/
/*======================================================================================*/
/*	[�����T�v]	�ׯ�����۰ُ��̏������������s���B										*/
/*======================================================================================*/
/*	[�L�q�`��]	void FlashCtrlDrv_Init( E_INIT_TYPE req )								*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_INIT_TYPE				req		�F�N�����								*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void FlashCtrlDrv_Init( E_INIT_TYPE req )
{
	
	switch ( req ) {
	case E_INIT_RESET:			/* CPUؾ�Ăɂ��N�� */
	case E_INIT_WAKEUP:			/* �ȓd�͉����iWAKEUP�j�ɂ��N�� */
		/* �ׯ�����۰ُ��̏����ݒ� */
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
	case E_INIT_RET_NORMAL_VOL:	/* ��d�����A */
		break;
	case E_INIT_INTERVAL_WAKEUP:	/* �Ԍ��N���������� */
		break;
	default:					/* ��L�ȊO */
		break;
	}
	
}

/****************************************************************************************/
/*	[���W���[����]	FlashCtrlDrv_Write			[����]	�ׯ���������ݗv����t			*/
/*======================================================================================*/
/*	[�����T�v]	�w�肳�ꂽ���ڽ�ւ��ް��������ݗv������t����B							*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 FlashCtrlDrv_Write( T_FlashDrv_Addr write_addr,					*/
/*										 UI_8 *write_data, UI_16 data_size )			*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	T_FlashDrv_Addr		write_addr	�F�������ݱ��ڽ							*/
/*				UI_8				*write_data	�F���������ް�							*/
/*				UI_16				data_size	�F���������ް��̻���					*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8				ret			�F�������ݎ�t����(D_OK)				*/
/*												�F��������busy/�͈͊O(D_NG)				*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
SI_8 FlashCtrlDrv_Write( T_FlashDrv_Addr write_addr, const UI_8 *write_data, UI_16 data_size )
{
	SI_8	ret = D_NG;

	/* �ׯ�����۰ُ��:�������ݒ��A�������͏������ł͂Ȃ���� */
	if ( ( FlashCtrlDrv_Info.flashctrldrv_state != E_FLASHCTRLDRV_WRITE ) &&
		( FlashCtrlDrv_Info.flashctrldrv_state != E_FLASHCTRLDRV_ERASE ) ) {
		/* �������ݗv������t���āA�ׯ�����۰ُ����X�V���� */
		FlashCtrlDrv_Info.access_addr = write_addr;
		FlashCtrlDrv_Info.access_end_addr = write_addr + data_size;
		FlashCtrlDrv_Info.write_area = write_data;
		FlashCtrlDrv_Info.flashctrldrv_state = E_FLASHCTRLDRV_WRITE;

		/* �ׯ���������ݗv���̎�t���� */
		ret = D_OK;
	}

	return ret;
}

/****************************************************************************************/
/*	[���W���[����]	FlashCtrlDrv_Erase			[����]	�ׯ�������v����t				*/
/*======================================================================================*/
/*	[�����T�v]	�w�肳�ꂽ���ڽ���ް������v������t����B								*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 FlashCtrlDrv_Erase( T_FlashDrv_Addr erase_addr, UI_32 data_size )	*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	T_FlashDrv_Addr		erase_addr	�F�������ڽ								*/
/*				UI_32				data_size	�F��������								*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8				ret			�F������t����(D_OK)					*/
/*												�F����busy/�͈͊O(D_NG)					*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
SI_8 FlashCtrlDrv_Erase( T_FlashDrv_Addr erase_addr, UI_32 data_size )
{
	SI_8	ret = D_NG;
	UI_8	count;

	/* �ׯ�����۰ُ��:�������ݒ��A�������͏������ł͂Ȃ���� */
	if ( ( FlashCtrlDrv_Info.flashctrldrv_state != E_FLASHCTRLDRV_WRITE ) &&
		( FlashCtrlDrv_Info.flashctrldrv_state != E_FLASHCTRLDRV_ERASE ) ) {
		/* ������ۯ����� */
		for ( count = 0U; count < FLASH_MAX_BLKNUM; count++ ) {
			/* ������ۯ��J�n���ڽ����������ۯ��ԍ����擾���� */
			if ( erase_addr == C_FlashBlkTbl[count].start_addr ) {
				/* �����v������t���āA�ׯ�����۰ُ����X�V���� */
				FlashCtrlDrv_Info.access_addr = erase_addr;
				FlashCtrlDrv_Info.access_end_addr = ( erase_addr + data_size ) - 1U;
				FlashCtrlDrv_Info.blknum = count;
				FlashCtrlDrv_Info.flashctrldrv_state = E_FLASHCTRLDRV_ERASE;

				/* �ׯ�������v���̎�t���� */
				ret = D_OK;

				break;
			}
		}
	}

	return ret;
}

/****************************************************************************************/
/*	[���W���[����]	FlashCtrlDrv_CheckStatus	[����]	�ׯ���v����������				*/
/*======================================================================================*/
/*	[�����T�v]	��t������������/�����v���������������s���B								*/
/*======================================================================================*/
/*	[�L�q�`��]	E_FLASHCTRLDRV_STATE FlashCtrlDrv_CheckStatus( void )					*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	E_FLASHCTRLDRV_STATE	ret	�F											*/
/*				�ׯ�����۰���ײ�ނ̽ð��	�F�����(E_FLASHCTRLDRV_IDLE)				*/
/*											�F�������ݒ�(E_FLASHCTRLDRV_WRITE)			*/
/*											�F������(E_FLASHCTRLDRV_ERASE)				*/
/*											�F�������ݎ��s(E_FLASHCTRLDRV_WRITE_ERROR)	*/
/*											�F�������s(E_FLASHCTRLDRV_ERASE_ERROR)		*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
E_FLASHCTRLDRV_STATE FlashCtrlDrv_CheckStatus( void )
{
	return FlashCtrlDrv_Info.flashctrldrv_state;
}

/****************************************************************************************/
/*	[���W���[����]	FlashCtrlDrv_Proc	[����]		�ׯ�����۰���ײ��Ҳݏ���			*/
/*======================================================================================*/
/*	[�����T�v]	�ׯ�����۰���ײ��Ҳݏ������s���B										*/
/*======================================================================================*/
/*	[�L�q�`��]	void FlashCtrlDrv_Proc( void )											*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void FlashCtrlDrv_Proc( void )
{
#ifndef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
	UI_16					mask_level;						/* ���荞��Ͻ�����			*/
#endif
	SI_8					sub_ret;						/* �Ăяo���֐��̖߂�l		*/

	switch ( FlashCtrlDrv_Info.flashctrldrv_state ) {
	case E_FLASHCTRLDRV_WRITE:								/* �ׯ���������ݗv�����s	*/
	case E_FLASHCTRLDRV_ERASE:								/* �ׯ�������v�����s		*/

#ifndef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
		/* ���݂̊��荞�����ق��擾���A���荞�݋֎~�ɂ��� */
		(void)IntrDrv_GetMaskLevel( &mask_level );
		(void)IntrDrv_SetMaskLevel( FLASH_ACCESS_INTR_LEVEL );
#endif
		
		/* ��������/���������s */
		sub_ret = C_FlashCtrlDrvPcbTbl[FlashCtrlDrv_Info.flashctrldrv_state].act_func();

#ifndef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
		/* ���荞�����ق𕜋A���� */
		(void)IntrDrv_SetMaskLevel( mask_level );
#endif

		/* ��������/�����������̏��� */
		if ( sub_ret == D_OK ) {

			/* ��������/���������������ꍇ�́A����̏�������/�����v������������ */
			sub_ret = C_FlashCtrlDrvPcbTbl[FlashCtrlDrv_Info.flashctrldrv_state].jdg_func();
			if ( sub_ret == D_OK ) {

				/* �S�Ă̏�������/�������I�������ꍇ�́A�����Ƃ��� */
				FlashCtrlDrv_Info.flashctrldrv_state = E_FLASHCTRLDRV_IDLE;
			}
		} else {
			/* ��������/���������s�����ꍇ�́A�G���[�Ƃ��� */
			FlashCtrlDrv_Info.flashctrldrv_state = C_FlashCtrlDrvPcbTbl[FlashCtrlDrv_Info.flashctrldrv_state].err_state;
		}

		break;
	default:									/* ��L�ȊO */
		break;	
	}
}

/****************************************************************************************/
/*	[���W���[����]	flash_ctrl_drv_write	[����]	��ײ��Ҳݏ����̏������ݏ���			*/
/*======================================================================================*/
/*	[�����T�v]	�ׯ�����۰���ײ��Ҳݏ����̏������ݏ������s���B							*/
/*======================================================================================*/
/*	[�L�q�`��]	static SI_8 flash_ctrl_drv_write( void )								*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8		write_result	:����(D_OK)�A�ُ�(D_NG)��Ԃ�				*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static SI_8 flash_ctrl_drv_write( void )
{
	SI_8					write_result = D_NG;
	SI_8					ret;
	SI_8					sub_ret;

#ifndef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
	/* �ׯ������۸���ݸ�Ӱ�ނɂ��� */
	ret = FlashDrv_Enable();
	if ( ret == D_OK ) {
#endif
		/* �ׯ����ײ�ނ֑O����ʰ�ܰ�ޏ������ݗv���𔭍s���� */
		sub_ret = FlashDrv_Write( FlashCtrlDrv_Info.access_addr, FlashCtrlDrv_Info.write_area, D_FLASHDRV_DATASIZE );
		if ( sub_ret == D_OK ) {
			/* �������݂���������܂ŁA�`�F�b�N�����{����*/
			write_result = flash_ctrl_drv_wait_chk();
		}

		if ( write_result == D_OK ) {
			/* ���񏑂����݂̱��ڽ���ް����ڽ���Z�o���� */
			FlashCtrlDrv_Info.access_addr += D_FLASHDRV_DATASIZE;
			FlashCtrlDrv_Info.write_area += D_FLASHDRV_DATASIZE;

			/* �ׯ����ײ�ނ֌㔼��ʰ�ܰ�ޏ������ݗv���𔭍s���� */
			sub_ret = FlashDrv_Write( FlashCtrlDrv_Info.access_addr, FlashCtrlDrv_Info.write_area, D_FLASHDRV_DATASIZE );
			if ( sub_ret == D_OK ) {
				/* �������݂���������܂ŁA�`�F�b�N�����{����*/
				write_result = flash_ctrl_drv_wait_chk();
			} else {
				write_result = D_NG;
			}
		}
#ifndef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
		/* �ׯ����CPUӰ�ނɂ��� */
		ret = FlashDrv_Disable();
		if ( ret != D_OK ) {
			/* CPU���[�h�ɕ��A�ł��Ȃ������ꍇ�͎��s�Ƃ��� */
			write_result = D_NG;
		}
	}
#endif

	return write_result;
}

/****************************************************************************************/
/*	[���W���[����]	flash_ctrl_drv_write_complete[����]	��ײ��Ҳݏ����̏������݊�������	*/
/*======================================================================================*/
/*	[�����T�v]	�ׯ�����۰���ײ��Ҳݏ����̏������݊���������s��						*/
/*======================================================================================*/
/*	[�L�q�`��]	static SI_8 flash_ctrl_drv_write_complete(void)							*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8		ret			: �S�̈�̏���������(D_OK)�A������������(D_NG)	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static SI_8 flash_ctrl_drv_write_complete( void )
{
	SI_8					ret = D_NG;

	/* ���񏑂����݂̱��ڽ���Z�o���� */
	FlashCtrlDrv_Info.access_addr += D_FLASHDRV_DATASIZE;

	/* �������ݗv�����ފ������������� */
	if ( FlashCtrlDrv_Info.access_addr < FlashCtrlDrv_Info.access_end_addr ) {
		/* ���񏑂������ް����ڽ���Z�o���� */
		FlashCtrlDrv_Info.write_area += D_FLASHDRV_DATASIZE;
	} else {
		/* �������݂����������ꍇ��OK��Ԃ� */
		ret = D_OK;
	}
	
	return ret;
}

/****************************************************************************************/
/*	[���W���[����]	flash_ctrl_drv_erase	[����]	��ײ��Ҳݏ����̏�������				*/
/*======================================================================================*/
/*	[�����T�v]	�ׯ�����۰���ײ��Ҳݏ����̏����������s��								*/
/*======================================================================================*/
/*	[�L�q�`��]	static SI_8 flash_ctrl_drv_erase( void )								*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8		erase_result	:����(D_OK)�A�ُ�(D_NG)��Ԃ�				*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static SI_8 flash_ctrl_drv_erase( void )
{
	SI_8					erase_result = D_NG;
	SI_8					ret;
#ifndef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
	/* �ׯ������۸���ݸ�Ӱ�ނɂ��� */
	ret = FlashDrv_Enable();

	if ( ret == D_OK ) {
		/* �ׯ����ײ�ނ���ۯ������v����ݒ肷�� */
		ret = FlashDrv_Erase( FlashCtrlDrv_Info.access_addr );
		if ( ret == D_OK ) {
			/* ��������������܂ŁA�`�F�b�N�����{���� */
			erase_result = flash_ctrl_drv_wait_chk();
		}
		/* �ׯ����CPUӰ�ނɂ��� */
		ret = FlashDrv_Disable();
		if ( ret != D_OK ) {
			/* CPU���[�h�ɕ��A�ł��Ȃ������ꍇ�͎��s�Ƃ��� */
			erase_result = D_NG;
		}
	}
#else
	UI_16					mask_level;
	
	/* ���݂̊��荞�����ق��擾���A���荞�݋֎~�ɂ��� */
	(void)IntrDrv_GetMaskLevel( &mask_level );
	(void)IntrDrv_SetMaskLevel( FLASH_ACCESS_INTR_LEVEL );
	/* �ׯ����ײ�ނ���ۯ������v����ݒ肷�� */
	ret = FlashDrv_Erase( FlashCtrlDrv_Info.access_addr );
	/* ���荞�����ق𕜋A���� */
	(void)IntrDrv_SetMaskLevel( mask_level );
	if ( ret == D_OK ) {
		/* ��������������܂ŁA�`�F�b�N�����{���� */
		erase_result = flash_ctrl_drv_wait_chk();
	}
#endif
	return erase_result;
}

/****************************************************************************************/
/*	[���W���[����]	flash_ctrl_drv_erase_complete	[����]��ײ��Ҳݏ����̏����I������	*/
/*======================================================================================*/
/*	[�����T�v]	�ׯ�����۰���ײ��Ҳݏ����̏����I��������s���B							*/
/*======================================================================================*/
/*	[�L�q�`��]	static SI_8 flash_ctrl_drv_erase_complete( void )						*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8		ret			: �S�̈�̏�������(D_OK)�A����������(D_NG)		*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static SI_8 flash_ctrl_drv_erase_complete( void )
{
	SI_8					ret = D_NG;

	if ( FlashCtrlDrv_Info.access_end_addr <= C_FlashBlkTbl[FlashCtrlDrv_Info.blknum].end_addr ) {
		/* �S�Ă̏��������������ꍇ��OK��Ԃ� */
		ret = D_OK;
	} else {
		/* ���������ۯ��ԍ����Z�o���� */
		FlashCtrlDrv_Info.blknum++;

		if ( FlashCtrlDrv_Info.blknum < FLASH_MAX_BLKNUM ) {
			/* �S�Ă̏����������A���̏�����ۯ��J�n���ڽ���擾���� */
			FlashCtrlDrv_Info.access_addr = C_FlashBlkTbl[FlashCtrlDrv_Info.blknum].start_addr;
		} else {
			/* �ׯ����؂͈̔͂𒴂���ꍇ�͏����I���Ƃ��� */
			ret = D_OK;
		}
	}

	return ret;
}

/****************************************************************************************/
/*	[���W���[����]	flash_ctrl_drv_wait_chk		[����]����/�������ݏ�����������			*/
/*======================================================================================*/
/*	[�����T�v]	�ׯ����ײ�ނ̏���/�������ݏ�������������s���B							*/
/*======================================================================================*/
/*	[�L�q�`��]	static SI_8 flash_ctrl_drv_wait_chk( void )								*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8		ret			: ����I��(D_OK)�A�ُ�I��(D_NG)��Ԃ�			*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static SI_8 flash_ctrl_drv_wait_chk( void )
{
	E_FLASHDRV_STATE		status;							/* �ׯ����ײ�޽ð�� */
	UI_32					overflow_count = 0U;			/* ���ް�۰���o�p���� */
	UI_8					ovf_info = D_OFF;				/* ���ް�۰��� */
	SI_8					ret = D_NG;

	do {
#ifndef FLASH_DRV_REPRGM_EXECUTION_AREA_ALL_RAM
		WdtDrv_ReqClr();				/* S/W WDT �ر		 */
		WdtDrv_ReqClrCpu();				/* H/W WDT �ر		 */
		HWWdtDrv_ReqCrl();				/* �O��WDT(HWWDT)�ر */
#endif

		/* �ׯ����ײ�ނ̏�Ԃ��擾���� */
		status = FlashDrv_CheckStatus();
		switch ( status ) {
		case E_FLASHDRV_IDLE:
			/* ���������������ꍇ */
			ovf_info = D_ON;							/* �����I����				*/
			ret = D_OK;									/* ����I���Ƃ���			*/
			break;
		case E_FLASHDRV_WRITE:
			/* �������ݒ��̏ꍇ */
			if ( overflow_count <= FLASH_WAIT_TM_FOR_WRITE ) {
				D_NOP10();								/* 10�T�C�N���E�G�C�g����	*/
				++overflow_count;
			} else {
				ovf_info = D_ON;						/* �����I����				*/
				ret = D_NG;								/* �ُ�I���Ƃ���			*/
			}
			break;
		case E_FLASHDRV_ERASE:
			/* �������̏ꍇ */
			if ( overflow_count <= FLASH_WAIT_TM_FOR_ERASE ) {
				D_NOP10();								/* 10�T�C�N���E�G�C�g����	*/
				++overflow_count;
			} else {
				ovf_info = D_ON;						/* �����I����				*/
				ret = D_NG;								/* �ُ�I���Ƃ���			*/
			}
			break;
		case E_FLASHDRV_WRITE_ERROR:
		case E_FLASHDRV_ERASE_ERROR:
		default:
			/* �ُ킪���������ꍇ */
			ovf_info = D_ON;							/* �����I����				*/
			ret = D_NG;									/* �ُ�I���Ƃ���			*/
			break;
		}
		
	} while ( ovf_info == D_OFF );

	return ret;
}
