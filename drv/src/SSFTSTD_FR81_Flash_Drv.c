/********************************************************************************/
/* �q�於		:	�W�����W���[��												*/
/* �@�햼		:	SSFT														*/
/* ϲ�ݿ��ðϖ�	:	PF															*/
/*==============================================================================*/
/* �쐬̧�ٖ�	:	SSFTSTD_FR81_Flash_Drv.c									*/
/* 				:	�ׯ������������ײ��											*/
/*==============================================================================*/
/* �Ώ�ϲ��		:	MB91570 Series												*/
/*==============================================================================*/
/* �쐬�ް�ޮ�	:	010101														*/
/* �쐬�N����	:	2012.07.13													*/
/* �쐬��		:	K.Horikawa													*/
/*------------------------------------------------------------------------------*/
/* �ύX����		:																*/
/* [040101]		:	�f�[�^�t���b�V���h���C�o�쐬�ɔ����A						*/
/*					���[�N�t���b�V���p�̃��W�X�^�A�N�Z�X�̏������폜			*/
/********************************************************************************/

/*** START_INC ***/
/********************************************************************************/
/*   Include File                                                               */
/*------------------------------------------------------------------------------*/
/*      ͯ�ް̧�ق̲ݸٰ�ޕ��́A���Ļ�قɋL�ڂ��邱��                          */
/********************************************************************************/
#include "SSFTSTD_Type.h"
#include "SSFTSTD_Macro.h"
#include "mb91570.h"

#define	 __MB91570_FLASH_DRV_INTERNAL__
#include "SSFTSTD_FR81_Flash_Drv.h"

/*==============================================================================*/
/*	typedef��`�i�O������J�j													*/
/*==============================================================================*/
#ifdef FLASH_DRV_REPRGM_ENABLE
/* �ׯ��������̍\���� */
typedef struct t_flashdrv_t {
	E_FLASHDRV_STATE		state;					/* �ð��					*/
	T_FlashDrv_Addr			addr;					/* ��������/�������ڽ		*/
	UI_16					wrdata;					/* ���������ް�				*/
}T_FlashDrv;
#endif	/* FLASH_DRV_REPRGM_ENABLE */

/*==============================================================================*/
/*	�֐��������ߐ錾�i�O������J)												*/
/*==============================================================================*/
#ifdef FLASH_DRV_REPRGM_ENABLE
static E_FLASHDRV_STATE flash_drv_write_polling( void );
static E_FLASHDRV_STATE flash_drv_erase_toggle( void );
static void flash_drv_reset( E_FLASHDRV_STATE state );
#endif	/* FLASH_DRV_REPRGM_ENABLE */

/*==============================================================================*/
/*	�����萔																	*/
/*==============================================================================*/
/* �Ȃ� */

/*==============================================================================*/
/*	�����}�N��																	*/
/*==============================================================================*/
#define MASK_LAST_HALFWORD			(0x00000002U)	/* �������ݱ��ڽϽ�:�㔼��ʰ�ܰ��	*/
#define BIT_SHIFT_8					(8U)			/* �ޯļ�ėp				*/

/*==============================================================================*/
/*	�����ϐ�																	*/
/*==============================================================================*/
#ifdef FLASH_DRV_REPRGM_ENABLE
static T_FlashDrv				FlashDrvInfo;
#endif	/* FLASH_DRV_REPRGM_ENABLE */

/****************************************************************************************/
/*	[���W���[����]	FlashDrv_Init		[����]	�ׯ������������ײ��ڼ޽������ݒ菈��	*/
/*======================================================================================*/
/*	[�����T�v]	ڼ޽������ݒ莞�ɖ{�֐����ĂсA���������s���B							*/
/*======================================================================================*/
/*	[�L�q�`��]	void FlashDrv_Init( E_INIT_TYPE req  )									*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_INIT_TYPE				req		�F�N�����								*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void FlashDrv_Init( E_INIT_TYPE req )
{
	
	switch ( req ) {
	case E_INIT_RESET:			/* CPUؾ�Ăɂ��N�� */
	case E_INIT_WAKEUP:			/* �ȓd�͉����iWAKEUP�j�ɂ��N�� */
		/* �ׯ������������ײ��ڼ޽��̏����ݒ� */
		FCTLR		= INIT_FCTLR;
		FSTR		= INIT_FSTR;
		FLIFCTLR	= INIT_FLIFCTLR;

#ifdef FLASH_DRV_REPRGM_ENABLE
		/* �ׯ��������̏����ݒ� */
		FlashDrvInfo.state	= E_FLASHDRV_IDLE;
		FlashDrvInfo.addr	= 0U;
		FlashDrvInfo.wrdata	= 0U;
#endif	/* FLASH_DRV_REPRGM_ENABLE */

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
/*	[���W���[����]	FlashDrv_Sleep	[����]	�ׯ������������ײ��ڼ޽���~�ݒ菈��		*/
/*======================================================================================*/
/*	[�����T�v]	�ȓd��Ӱ�ޑJ�ڎ��ɖ{�֐����ĂсA���������s���B							*/
/*======================================================================================*/
/*	[�L�q�`��]	void FlashDrv_Sleep( void )												*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void FlashDrv_Sleep( void )
{
	
	/* �ׯ������������ײ��ڼ޽��̒�~�ݒ菈�� */
	FCTLR		= STOP_FCTLR;
	FSTR		= STOP_FSTR;
	FLIFCTLR	= STOP_FLIFCTLR;
	
}

/****************************************************************************************/
/*	[���W���[����]	FlashDrv_Refresh	[����]	�ׯ������������ײ��ڼ޽��Đݒ菈��		*/
/*======================================================================================*/
/*	[�����T�v]	ڼ޽��Đݒ莞�ɖ{�֐����ĂсA���������s���B								*/
/*======================================================================================*/
/*	[�L�q�`��]	void FlashDrv_Refresh( void )											*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void FlashDrv_Refresh( void )
{
	
	/* ��֐� */
	
}

#ifdef FLASH_DRV_REPRGM_ENABLE
/****************************************************************************************/
/*	[���W���[����]	FlashDrv_Read		[����]	�ׯ������������ײ���ׯ���ǂݍ��ݏ���	*/
/*======================================================================================*/
/*	[�����T�v]	�w�肳�ꂽ���ڽ�����ް���ǂݍ��ށB										*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 FlashDrv_Read( T_FlashDrv_Addr read_addr,							*/
/*									 UI_8 *read_data, UI_16 data_size )					*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	T_FlashDrv_Addr		read_addr	�F�ǂݍ��ݱ��ڽ							*/
/*				UI_8				*read_data	�F�ǂݍ����ް�							*/
/*				UI_16				data_size	�F�ǂݍ����ް��̻���(byte)				*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8				ret			�F�ǂݍ���OK(D_OK)						*/
/*												�F�ǂݍ���NG(D_NG)						*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
SI_8 FlashDrv_Read( T_FlashDrv_Addr read_addr, UI_8 *read_data, UI_16 data_size )
{
	UI_8	count;
	UI_8	*temp_addr;
	SI_8	ret = D_NG;

	/* �ǂݍ��ݱ��ڽ�͈́A�ײ��ނ��ް����ނ��������� */
	if ( ( ( FLASH_START_ADDR <= read_addr ) && ( read_addr <= FLASH_END_ADDR ) ) &&
		 ( ( read_addr % D_FLASHDRV_DATASIZE ) == 0U ) &&
		 ( data_size == D_FLASHDRV_DATASIZE ) ) {

		temp_addr = (UI_8 *)read_addr;

		for ( count = 0U; count < D_FLASHDRV_DATASIZE; count++ ) {
			/* �w����ڽ���ް���ǂݍ��� */
			read_data[count] = temp_addr[count];
		}

		ret = D_OK;
	}

	return ret;
}

/****************************************************************************************/
/*	[���W���[����]	FlashDrv_Write		[����]	�ׯ������������ײ���ׯ���������ݏ���	*/
/*======================================================================================*/
/*	[�����T�v]	�w�肳�ꂽ���ڽ���ް����������ށB										*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 FlashDrv_Write( T_FlashDrv_Addr write_addr,						*/
/*									 UI_8 *write_data, UI_16 data_size )				*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	T_FlashDrv_Addr		write_addr	�F�������ݱ��ڽ							*/
/*				UI_8				*write_data	�F���������ް�							*/
/*				UI_16				data_size	�F���������ް��̻���(byte)				*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8				ret			�F��������OK(D_OK)						*/
/*												�F��������NG(D_NG)						*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
SI_8 FlashDrv_Write( T_FlashDrv_Addr write_addr, const UI_8 *write_data, UI_16 data_size )
{
	T_FlashDrv_Addr		flash_algo1_addr;
	T_FlashDrv_Addr		flash_algo2_addr;
	T_FlashDrv_Addr		flash_algo3_addr;
	T_FlashDrv_Addr		temp_addr;
	UI_16				temp_data;
	UI_8				dummy;
	SI_8				ret = D_NG;

	/* �������ݱ��ڽ�͈́A�ײ��ނ��ް����ނ��������� */
	if ( ( FLASH_START_ADDR <= write_addr ) && ( write_addr <= FLASH_END_ADDR ) &&
		 ( ( write_addr % D_FLASHDRV_DATASIZE ) == 0U ) &&
		 ( data_size == D_FLASHDRV_DATASIZE ) ) {
		/* CPUӰ�ނƏ������݋����ޯĂ��������� */
		if ( ( FCTLR_FWE == D_ON ) && ( FSTR_FRDY == D_ON ) ) {
			/* 1ܰ���ް��������݊J�n����ECC�װ�����׸ނ�ر���� */
			if ( ( write_addr & MASK_LAST_HALFWORD ) != MASK_LAST_HALFWORD ) {
				/* �O����ʰ�ܰ�ނ̏ꍇ�́AECC�װ�����׸ނ�ر���� */
				FSTR_FECCERR = D_OFF;
			}

			/* �������ݺ���ޱ��ڽ���Z�o���� */
			temp_data = (UI_16)((UI_16)(((UI_16)write_data[0]) << BIT_SHIFT_8) + write_data[1]);
			temp_addr = write_addr & FLASH_ALGO_OFFSET_MASK;
			flash_algo1_addr	= temp_addr + FLASH_ALGO1_OFFSET;
			flash_algo2_addr	= temp_addr + FLASH_ALGO2_OFFSET;
			flash_algo3_addr	= temp_addr + FLASH_ALGO3_OFFSET;

			/* ʰ�ܰ�ނ̏������� */
			(*(volatile UI_16*)flash_algo1_addr)	= FLASH_ALGO1_VALUE;	/* ��1�������ݻ��� */
			(*(volatile UI_16*)flash_algo2_addr)	= FLASH_ALGO2_VALUE;	/* ��2�������ݻ��� */
			(*(volatile UI_16*)flash_algo3_addr)	= FLASH_ALGOW_VALUE;	/* ��3�������ݻ��� */
			(*(volatile UI_16*)write_addr)			= temp_data;			/* data�������ݻ��� */

			/* �ׯ��������̊i�[ */
			FlashDrvInfo.state	= E_FLASHDRV_WRITE;
			FlashDrvInfo.addr	= write_addr;
			FlashDrvInfo.wrdata	= temp_data;

			/* FSTRڼ޽���аذ�� */
			dummy = FSTR;

			ret = D_OK;
		}
	}

	return ret;
}

/****************************************************************************************/
/*	[���W���[����]	FlashDrv_Erase		[����]	�ׯ������������ײ���ׯ����������		*/
/*======================================================================================*/
/*	[�����T�v]	�w�肳�ꂽ���ڽ���ް�����������B										*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 FlashDrv_Erase( T_FlashDrv_Addr erase_addr )						*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	T_FlashDrv_Addr		erase_addr	�F�������ڽ								*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8				ret			�F����OK(D_OK)�A����NG(D_NG)			*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
SI_8 FlashDrv_Erase( T_FlashDrv_Addr erase_addr )
{
	T_FlashDrv_Addr	flash_algo1_addr;
	T_FlashDrv_Addr	flash_algo2_addr;
	T_FlashDrv_Addr	flash_algo3_addr;
	T_FlashDrv_Addr	flash_algo4_addr;
	T_FlashDrv_Addr	flash_algo5_addr;
	T_FlashDrv_Addr	flash_algo6_addr;
	T_FlashDrv_Addr	temp_addr;
	UI_8			dummy;
	SI_8			ret = D_NG;

	/* �������ڽ�͈͂��������� */
	if ( ( FLASH_START_ADDR <= erase_addr ) && ( erase_addr <= FLASH_END_ADDR ) ) {
		/* CPUӰ�ނƏ��������ޯĂ��������� */
		if ( ( FCTLR_FWE == D_ON ) && ( FSTR_FRDY == D_ON ) ) {
			/* �����ٺ�ؽ�я�������ޱ��ڽ���Z�o���� */
			temp_addr = erase_addr & FLASH_ALGO_OFFSET_MASK;
			flash_algo1_addr	= temp_addr + FLASH_ALGO1_OFFSET;
			flash_algo2_addr	= temp_addr + FLASH_ALGO2_OFFSET;
			flash_algo3_addr	= temp_addr + FLASH_ALGO3_OFFSET;
			flash_algo4_addr	= temp_addr + FLASH_ALGO4_OFFSET;
			flash_algo5_addr	= temp_addr + FLASH_ALGO5_OFFSET;
			flash_algo6_addr	= erase_addr;

			/* �w����ۯ��̑O������Ɏ����ٺ�ؽ�я�������ނ����s���� */
			(*(volatile UI_16*)flash_algo1_addr)	= FLASH_ALGO1_VALUE;
			(*(volatile UI_16*)flash_algo2_addr)	= FLASH_ALGO2_VALUE;
			(*(volatile UI_16*)flash_algo3_addr)	= FLASH_ALGO3_VALUE;
			(*(volatile UI_16*)flash_algo4_addr)	= FLASH_ALGO4_VALUE;
			(*(volatile UI_16*)flash_algo5_addr)	= FLASH_ALGO5_VALUE;
			(*(volatile UI_16*)flash_algo6_addr)	= FLASH_ALGOE_VALUE;

			flash_algo6_addr = erase_addr + FLASH_SC_UNIT;
			/* �w����ۯ��̌㔼����Ɏ����ٺ�ؽ�я�������ނ����s���� */
			if ( ( (*(volatile UI_16*)flash_algo6_addr) & FLASH_SETI ) == FLASH_SETI ) {
				/* �w����ۯ��̑O������̏������Ċ��Ԃ𒴉߂��Ă��� */
				(*(volatile UI_16*)flash_algo1_addr)	= FLASH_ALGO1_VALUE;
				(*(volatile UI_16*)flash_algo2_addr)	= FLASH_ALGO2_VALUE;
				(*(volatile UI_16*)flash_algo3_addr)	= FLASH_ALGO3_VALUE;
				(*(volatile UI_16*)flash_algo4_addr)	= FLASH_ALGO4_VALUE;
				(*(volatile UI_16*)flash_algo5_addr)	= FLASH_ALGO5_VALUE;
				(*(volatile UI_16*)flash_algo6_addr)	= FLASH_ALGOE_VALUE;
			} else {
				/* �w����ۯ��̑O������̏������Ċ��Ԓ� */
				(*(volatile UI_16*)flash_algo6_addr)	= FLASH_ALGOE_VALUE;
			}

			/* �ׯ��������̊i�[ */
			FlashDrvInfo.state	= E_FLASHDRV_ERASE;
			FlashDrvInfo.addr	= erase_addr;

			/* FSTRڼ޽���аذ�� */
			dummy = FSTR;

			ret = D_OK;
		}
	}

	return ret;
}

/****************************************************************************************/
/*	[���W���[����]	FlashDrv_CheckStatus	[����]	�ׯ������������ײ���ׯ���ð������	*/
/*======================================================================================*/
/*	[�����T�v]	�ׯ������������ײ�ނ̓���ɂ���āA�ׯ���ð������������B				*/
/*======================================================================================*/
/*	[�L�q�`��]	E_FLASHDRV_STATE FlashDrv_CheckStatus( void )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	E_FLASHDRV_STATE	ret	�F												*/
/*					�ׯ����ײ�ނ̽ð��	�F	�����(E_FLASHDRV_IDLE)						*/
/*										�F	�������ݒ�(E_FLASHDRV_WRITE)				*/
/*										�F	�������ݎ��s(E_FLASHDRV_WRITE_ERROR)		*/
/*										�F	������(E_FLASHDRV_ERASE)					*/
/*										�F	�������s(E_FLASHDRV_ERASE_ERROR)			*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
E_FLASHDRV_STATE FlashDrv_CheckStatus( void )
{
	switch ( FlashDrvInfo.state ) {
	case E_FLASHDRV_WRITE:					/* �ׯ���������݌���ް��߰�ݸ� */
		/* �ׯ�������݂���������������������*/
		FlashDrvInfo.state = flash_drv_write_polling();

		/* �ׯ�������݂����s�����ꍇ�Aؾ�ĺ���ނ𔭍s���� */
		flash_drv_reset(FlashDrvInfo.state);

		break;
	case E_FLASHDRV_ERASE:					/* �ׯ���������ĸ������ */
		/* �ׯ����������������������������*/
		FlashDrvInfo.state = flash_drv_erase_toggle();

		/* �ׯ�����������s�����ꍇ�Aؾ�ĺ���ނ𔭍s���� */
		flash_drv_reset(FlashDrvInfo.state);

		break;
	default:								/* ��L�ȊO */
		break;
	}

	return FlashDrvInfo.state;
}

/****************************************************************************************/
/*	[���W���[����]	FlashDrv_Enable		[����]	�ׯ������������ײ���ׯ����������		*/
/*======================================================================================*/
/*	[�����T�v]	FCTLRڼ޽���FWE�ޯĂ�ݒ肵�āA�ׯ��������������B					*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 FlashDrv_Enable( void )											*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8				ret			�F��������OK(D_OK)�A��������NG(D_NG)	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
SI_8 FlashDrv_Enable( void )
{
	SI_8	ret = D_NG;

	/* CPUӰ�ނ��ׯ��HANG�װ���������� */
	if ( ( FCTLR_FWE == D_OFF ) && ( FSTR_FHANG == D_OFF ) ) {
		/* �ׯ�������̋���ݒ肷�� */
		FCTLR_FWE	= D_ON;

		if ( FSTR_FRDY == D_ON ) {
			/* �ׯ���������ݒ萬�� */
			ret = D_OK;
		}
	}

	return ret;
}

/****************************************************************************************/
/*	[���W���[����]	FlashDrv_Disable	[����]	�ׯ������������ײ���ׯ�������֎~		*/
/*======================================================================================*/
/*	[�����T�v]	FCTLRڼ޽���FWE�ޯĂ�ݒ肵�āA�ׯ���������֎~����B					*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 FlashDrv_Disable( void )											*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8				ret			�F�����֎~OK(D_OK)�A�����֎~NG(D_NG)	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
SI_8 FlashDrv_Disable( void )
{
	SI_8	ret = D_NG;

	/* CPUӰ�ނƏ�������/���������ޯĂ��������� */
	if ( ( FCTLR_FWE == D_ON ) && ( FSTR_FRDY == D_ON ) ) {
		/* �ׯ�������̋֎~��ݒ肷�� */
		FCTLR_FWE	= D_OFF;

		/* �ׯ�������֎~�ݒ萬�� */
		ret = D_OK;
	}

	return ret;
}

/****************************************************************************************/
/*	[���W���[����]	flash_drv_write_polling	[����]	�ׯ���������݌���ް��߰�ݸ�		*/
/*======================================================================================*/
/*	[�����T�v]	�ׯ�����ް��������݌�ADPOLL�ޯĂ���������B							*/
/*======================================================================================*/
/*	[�L�q�`��]	E_FLASHDRV_STATE flash_drv_write_polling( void )						*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	E_FLASHDRV_STATE	ret_value	�F										*/
/*					�ׯ����ײ�ނ̽ð��	�F	�����(E_FLASHDRV_IDLE)						*/
/*										�F	�������ݒ�(E_FLASHDRV_WRITE)				*/
/*										�F	�������ݎ��s(E_FLASHDRV_WRITE_ERROR)		*/
/*										�F	������(E_FLASHDRV_ERASE)					*/
/*										�F	�������s(E_FLASHDRV_ERASE_ERROR)			*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static E_FLASHDRV_STATE flash_drv_write_polling( void )
{
	E_FLASHDRV_STATE	ret_value;
	UI_32				dummy_data;
	volatile UI_16		hd_seq_f1;

	/* �߂�l������������ */
	ret_value = E_FLASHDRV_WRITE;

	/* ʰ�޳������޼��ݽ��Ԏ擾 */
	hd_seq_f1	= *(volatile UI_16*)(FlashDrvInfo.addr);

	/* DPOLL�ޯ����� */
	if (( hd_seq_f1 & FLASH_DPOLL ) == ( ( FlashDrvInfo.wrdata ) & FLASH_DPOLL ) ) {
		/* ����޼��ݽ����I�� */
		ret_value = E_FLASHDRV_IDLE;
	} else {
		/* TLOV�ޯ����� */
		if ( ( hd_seq_f1 & FLASH_TLOV ) == FLASH_TLOV ) {
			/* �ēxʰ�޳������޼��ݽ��Ԏ擾 */
			hd_seq_f1	= *(volatile UI_16*)(FlashDrvInfo.addr);

			/* DPOLL�ޯ����� */
			if ( ( hd_seq_f1 & FLASH_DPOLL ) == ( ( FlashDrvInfo.wrdata ) & FLASH_DPOLL ) ) {
				/* ����޼��ݽ����I�� */
				ret_value = E_FLASHDRV_IDLE;
			} else {
				/* ����޼��ݽ���s���s */
				ret_value = E_FLASHDRV_WRITE_ERROR;
			}
		}
	}

	/* �����݂��������������������s�� */
	if (ret_value == E_FLASHDRV_IDLE) {
		/* �㔼��ʰ�ܰ�ނ̏ꍇ�̂�ECC�װ�����׸ނ��������s�� */
		if ( (FlashDrvInfo.addr & MASK_LAST_HALFWORD) == MASK_LAST_HALFWORD ) {
			/* 1ܰ�ޏ����݌���ް���ǂݏo�� */
			dummy_data = *(volatile UI_32*)(FlashDrvInfo.addr - D_FLASHDRV_DATASIZE);

			/* ECC�װ�����׸ނ���Ă���Ă��邩���������s�� */
			if (FSTR_FECCERR == D_ON) {
				ret_value = E_FLASHDRV_WRITE_ERROR;
			}
		}
	}

	return ret_value;
}

/****************************************************************************************/
/*	[���W���[����]	flash_drv_erase_toggle	[����]	�ׯ����������ް�ĸ������			*/
/*======================================================================================*/
/*	[�����T�v]	�ׯ��������ATOGG1�ޯĂ���������B										*/
/*======================================================================================*/
/*	[�L�q�`��]	E_FLASHDRV_STATE flash_drv_erase_toggle( void )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	E_FLASHDRV_STATE	ret_value	�F										*/
/*					�ׯ����ײ�ނ̽ð��	�F	�����(E_FLASHDRV_IDLE)						*/
/*										�F	�������ݒ�(E_FLASHDRV_WRITE)				*/
/*										�F	�������ݎ��s(E_FLASHDRV_WRITE_ERROR)		*/
/*										�F	������(E_FLASHDRV_ERASE)					*/
/*										�F	�������s(E_FLASHDRV_ERASE_ERROR)			*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static E_FLASHDRV_STATE flash_drv_erase_toggle( void )
{
	E_FLASHDRV_STATE	ret_value;
	volatile UI_16		hd_seq_f1;
	volatile UI_16		hd_seq_f2;
	UI_8				count;

	/* �߂�l������������ */
	ret_value = E_FLASHDRV_ERASE;

	/* ٰ�߂���ۯ��������ʂ��������� */
	for ( count = 0U; ( ( count < FLASH_TOGCHK_LOOPMAX ) &&
					 ( ret_value == E_FLASHDRV_ERASE ) ); count++ ) {
		/* ���ʰ�޳������޼��ݽ��Ԏ擾 */
		hd_seq_f1	= *(volatile UI_16*)(FlashDrvInfo.addr);
		hd_seq_f2	= *(volatile UI_16*)(FlashDrvInfo.addr);

		/* ĸ���׸��ޯĂ����� */
		if ( ( hd_seq_f1 & FLASH_TOGG1 ) == (hd_seq_f2 & FLASH_TOGG1 ) ) {
			/* �w����ڽ���� */
			if ( ( FlashDrvInfo.addr & FLASH_SC_UNIT ) == FLASH_SC_UNIT ) {
				/* �㔼����������� */
				ret_value = E_FLASHDRV_IDLE;
			} else {
				/* �O������������� */
				/* �������ڽ:�O����� -> �㔼��� */
				FlashDrvInfo.addr += FLASH_SC_UNIT;
			}
		} else if ( ( hd_seq_f2 & FLASH_TLOV ) == FLASH_TLOV ) {
			/* �ēx���ʰ�޳������޼��ݽ��Ԏ擾 */
			hd_seq_f1	= *(volatile UI_16*)(FlashDrvInfo.addr);
			hd_seq_f2	= *(volatile UI_16*)(FlashDrvInfo.addr);

			/* ĸ���׸��ޯĂƎw����ڽ���������� */
			if ( ( ( hd_seq_f1 & FLASH_TOGG1 ) == ( hd_seq_f2 & FLASH_TOGG1 ) ) &&
				 ( ( FlashDrvInfo.addr & FLASH_SC_UNIT ) == FLASH_SC_UNIT ) ) {
				/* �㔼����������� */
				ret_value = E_FLASHDRV_IDLE;
			} else if ( ( ( hd_seq_f1 & FLASH_TOGG1 ) == ( hd_seq_f2 & FLASH_TOGG1 ) ) &&
					  ( ( FlashDrvInfo.addr & FLASH_SC_UNIT ) != FLASH_SC_UNIT ) ) {
				/* �O������������� */
				/* �������ڽ:�O����� -> �㔼��� */
				FlashDrvInfo.addr += FLASH_SC_UNIT;
			} else {
				/* ����޼��ݽ���s���s */
				ret_value = E_FLASHDRV_ERASE_ERROR;
			}
		} else {
			/* ����޼��ݽ���s�� */
			break;
		}
	}

	return ret_value;
}

/****************************************************************************************/
/*	[���W���[����]	flash_drv_reset			[����]	�ׯ��̱͂����ُ펞�Aؾ�ĺ���ޔ��s	*/
/*======================================================================================*/
/*	[�����T�v]	�ׯ��̱͂����ُ펞�Aؾ�ĺ���ނ𔭍s����B								*/
/*======================================================================================*/
/*	[�L�q�`��]	void flash_drv_reset( E_FLASHDRV_STATE state )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static void flash_drv_reset( E_FLASHDRV_STATE state )
{
	T_FlashDrv_Addr		flash_algor_addr;
	UI_8				dummy;

	if ( ( state == E_FLASHDRV_WRITE_ERROR ) ||
		 ( state == E_FLASHDRV_ERASE_ERROR ) ) {
		/* ؾ�ĺ���ނ̱��ڽ��ݒ肷�� */
		flash_algor_addr = FLASH_START_ADDR;

		/* ؾ�ĺ���ނ𔭍s���� */
		(*(volatile UI_16*)flash_algor_addr) = FLASH_ALGOR_VALUE;

		/* FSTRڼ޽���аذ�� */
		dummy = FSTR;
	}

}
#endif	/* FLASH_DRV_REPRGM_ENABLE */
