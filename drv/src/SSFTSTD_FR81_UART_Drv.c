/********************************************************************************/
/* �q�於		:	�W�����W���[��												*/
/* �@�햼		:	SSFT														*/
/* ϲ�ݿ��ðϖ�	:	PF															*/
/*==============================================================================*/
/* �쐬̧�ٖ�	:	SSFTSTD_FR81_UART_Drv.c										*/
/* 				:	UART��ײ��Ӽޭ��											*/
/*==============================================================================*/
/* �Ώ�ϲ��		:	MB91570 Series												*/
/*==============================================================================*/
/* �쐬�ް�ޮ�	:	020201														*/
/* �쐬�N����	:	2015.1.29													*/
/* �쐬��		:	k.uchiyama													*/
/*------------------------------------------------------------------------------*/
/* �ύX����		:																*/
/* [010101]		:	�V�K�쐬													*/
/* [010102]		:	�W��IF�Ή��BIF�֐��̖߂�lSI_8���B�֐����̕ύX�B			*/
/*				:	���M�����ݗv�������Z�b�g/�N���A�����̓���				*/
/*				:	��M�����ݗv�������Z�b�g/�N���A�����̓���				*/
/* [010104]		:	�����֐����̕ύX											*/
/* [020101]		:	IF�̒ǉ��i������, �ʐM�����ؑ�, �f�[�^���W�X�^�A�h���X�擾)	*/
/* [020201]		:	LCD�N���b�N�o�͋@�\��ǉ�����								*/
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

#define __MB91570_UART_DRV_INTERNAL__
#include "SSFTSTD_FR81_UART_Drv.h"


/********************************************************************************/
/*	�����}�N����`																*/
/********************************************************************************/
#define SSR_PARITY_MASK			(0x80U)		/* �p���e�B�G���[�t���O���o�}�X�N */
#define SSR_OVERRUN_MASK		(0x40U)		/* �I�[�o�[�����G���[�t���O���o�}�X�N */
#define SSR_FLAMING_MASK		(0x20U)		/* �t���[�~���O�G���[�t���O���o�}�X�N */
#define SSR_RDRF_MASK			(0x10U)		/* ��M�f�[�^�t���t���O���o�}�X�N */
#define SSR_TDRE_MASK			(0x08U)		/* ���M�f�[�^�G���v�e�B�t���O���o�}�X�N */
#define SSR_TIE_SET				(0x01U)		/* ���M�����ݗv�����Z�b�g�f�[�^ */
#define SSR_TIE_CLR				(0xFEU)		/* ���M�����ݗv�����N���A�f�[�^ */
#define SSR_RIE_SET				(0x02U)		/* ��M�����ݗv�����Z�b�g�f�[�^ */
#define SSR_RIE_CLR				(0xFDU)		/* ��M�����ݗv�����N���A�f�[�^ */
#define SMR_UPCL_SET			(0x04U)		/* �v���O���}�u���N���A�r�b�g�Z�b�g�f�[�^ */
#define SMR_MD_MASK				(0xC0U)		/* ���샂�[�h���o�}�X�N */
#define DUMMY_WRITE				(0x00U)		/* UART���M�p�_�~�[���C�g�f�[�^ */
#define SCR_RX_ERR_CLR_BIT_ON	(0x04U)		/* ��M�G���[�t���O�N���A�r�b�g */
#define UARTDRV_MODE_UART		(0x00U)		/* Uart�h���C�o:�񓯊����[�h(���샂�[�h0)�Ŏg�p */
#define UARTDRV_MODE_CLOCK		(0x80U)		/* Uart�h���C�o:�������[�h(�N���b�N�o��)(���샂�[�h2)�Ŏg�p */

#define SCR_PEN_CLR				(0x7FU)		/* �p���e�B���N���A�f�[�^(�p���e�B�Ȃ�) */
#define SCR_PEN_SET				(0x80U)		/* �p���e�B���Z�b�g�f�[�^(�p���e�B����) */
#define SCR_P_CLR				(0xBFU)		/* �p���e�B�I���N���A�f�[�^(�����p���e�B) */
#define SCR_P_SET				(0x40U)		/* �p���e�B�I���Z�b�g�f�[�^(��p���e�B) */
#define SCR_SBL_CLR				(0xDFU)		/* �X�g�b�v�r�b�g���I���N���A�f�[�^(1�r�b�g) */
#define SCR_SBL_SET				(0x20U)		/* �X�g�b�v�r�b�g���I���Z�b�g�f�[�^(2�r�b�g) */
#define SCR_CL_CLR				(0xEFU)		/* �f�[�^���I���N���A�f�[�^(7�r�b�g) */
#define SCR_CL_SET				(0x10U)		/* �f�[�^���I���Z�b�g�f�[�^(8�r�b�g) */
#define SSR_BDS_CLR				(0xFBU)		/* �]�������I���N���A�f�[�^(LSB�t�@�[�X�g) */
#define SSR_BDS_SET				(0x04U)		/* �]�������I���Z�b�g�f�[�^(MSB�t�@�[�X�g) */

/*==============================================================================*/
/*	�֐��������ߐ錾�i�O������J)												*/
/*==============================================================================*/
static SI_8 uart_drv_init( E_UARTDRV_CH ch );/* UART�h���C�o�֘A���W�X�^���������� */
static SI_8 uart_drv_stop( E_UARTDRV_CH ch );/* UART�h���C�o�֘A���W�X�^��~���� */
#if (UART_DRV_TX == 1U) /* ���M���� */
static void uart_drv_txdrive( E_UARTDRV_CH ch );	/* UART�h���C�o���M�N������ */
static void uart_drv_txstop( E_UARTDRV_CH ch );		/* UART�h���C�o���M��~���� */
#endif /* UART_DRV_TX */
#if (UART_DRV_RX == 1U) /* ��M���� */
static void uart_drv_rxdrive( E_UARTDRV_CH ch );	/* UART�h���C�o��M�N������ */
static void uart_drv_rxstop( E_UARTDRV_CH ch );		/* UART�h���C�o��M��~���� */
#endif /* UART_DRV_RX */
static UI_8 uart_drv_get_scr_act_value( E_UARTDRV_CH ch );
static UI_8 uart_drv_get_ssr_act_value( E_UARTDRV_CH ch );
static UI_16 uart_drv_get_bgr_act_value( E_UARTDRV_CH ch );

/*==============================================================================*/
/*	�����萔																	*/
/*==============================================================================*/
/* �V���A���R���g���[�����W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *volatile const C_SerialCtrlReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_SCR2),						/* SCR2�̃A�h���X */
	&(LIN_UART3_SCR3),						/* SCR3�̃A�h���X */
	&(LIN_UART4_SCR4),						/* SCR4�̃A�h���X */
	&(LIN_UART5_SCR5),						/* SCR5�̃A�h���X */
	&(LIN_UART6_SCR6),						/* SCR6�̃A�h���X */
	&(LIN_UART7_SCR7)						/* SCR7�̃A�h���X */
};

/* �V���A�����[�h���W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *volatile const C_SerialModeReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_SMR2),						/* SMR2�̃A�h���X */
	&(LIN_UART3_SMR3),						/* SMR3�̃A�h���X */
	&(LIN_UART4_SMR4),						/* SMR4�̃A�h���X */
	&(LIN_UART5_SMR5),						/* SMR5�̃A�h���X */
	&(LIN_UART6_SMR6),						/* SMR6�̃A�h���X */
	&(LIN_UART7_SMR7)						/* SMR7�̃A�h���X */
};

/* �V���A���X�e�[�^�X���W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *volatile const C_SerialStatReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_SSR2),						/* SSR2�̃A�h���X */
	&(LIN_UART3_SSR3),						/* SSR3�̃A�h���X */
	&(LIN_UART4_SSR4),						/* SSR4�̃A�h���X */
	&(LIN_UART5_SSR5),						/* SSR5�̃A�h���X */
	&(LIN_UART6_SSR6),						/* SSR6�̃A�h���X */
	&(LIN_UART7_SSR7)						/* SSR7�̃A�h���X */
};

/* ��M�f�[�^���W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *volatile const C_ReceiveDataReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_RDR2),						/* RDR2�̃A�h���X */
	&(LIN_UART3_RDR3),						/* RDR3�̃A�h���X */
	&(LIN_UART4_RDR4),						/* RDR4�̃A�h���X */
	&(LIN_UART5_RDR5),						/* RDR5�̃A�h���X */
	&(LIN_UART6_RDR6),						/* RDR6�̃A�h���X */
	&(LIN_UART7_RDR7)						/* RDR7�̃A�h���X */
};

/* ���M�f�[�^���W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *volatile const C_TransDataReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_TDR2),						/* TDR2�̃A�h���X */
	&(LIN_UART3_TDR3),						/* TDR3�̃A�h���X */
	&(LIN_UART4_TDR4),						/* TDR4�̃A�h���X */
	&(LIN_UART5_TDR5),						/* TDR5�̃A�h���X */
	&(LIN_UART6_TDR6),						/* TDR6�̃A�h���X */
	&(LIN_UART7_TDR7)						/* TDR7�̃A�h���X */
};
	
/* �g���X�e�[�^�X���䃌�W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *volatile const C_ExtendCtrlReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_ESCR2),						/* ESCR2�̃A�h���X */
	&(LIN_UART3_ESCR3),						/* ESCR3�̃A�h���X */
	&(LIN_UART4_ESCR4),						/* ESCR4�̃A�h���X */
	&(LIN_UART5_ESCR5),						/* ESCR5�̃A�h���X */
	&(LIN_UART6_ESCR6),						/* ESCR6�̃A�h���X */
	&(LIN_UART7_ESCR7)						/* ESCR7�̃A�h���X */
};

/* �g���ʐM�R���g���[�����W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *volatile const C_ExtendCommunicationReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_ECCR2),						/* ECCR2�̃A�h���X */
	&(LIN_UART3_ECCR3),						/* ECCR3�̃A�h���X */
	&(LIN_UART4_ECCR4),						/* ECCR4�̃A�h���X */
	&(LIN_UART5_ECCR5),						/* ECCR5�̃A�h���X */
	&(LIN_UART6_ECCR6),						/* ECCR6�̃A�h���X */
	&(LIN_UART7_ECCR7)						/* ECCR7�̃A�h���X */
};

/* �{�[���[�g�W�F�l���[�^���W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_16 *volatile const C_BaudRateReg[UARTDRV_CH_NUMBER] =
{
	&(LIN_UART2_BGR2),						/* BGR2�̃A�h���X */
	&(LIN_UART3_BGR3),						/* BGR3�̃A�h���X */
	&(LIN_UART4_BGR4),						/* BGR4�̃A�h���X */
	&(LIN_UART5_BGR5),						/* BGR5�̃A�h���X */
	&(LIN_UART6_BGR6),						/* BGR6�̃A�h���X */
	&(LIN_UART7_BGR7)						/* BGR7�̃A�h���X */
};

/* �V���A���R���g���[�����W�X�^�h���C�o�������e�[�u�� */
static UI_8 const C_SerialCtrlRegInitTbl[UARTDRV_CH_NUMBER] =
{
	INIT_SCR2,								/* SCR2�̃h���C�o�����ݒ�l */
	INIT_SCR3,								/* SCR3�̃h���C�o�����ݒ�l */
	INIT_SCR4,								/* SCR4�̃h���C�o�����ݒ�l */
	INIT_SCR5,								/* SCR5�̃h���C�o�����ݒ�l */
	INIT_SCR6,								/* SCR6�̃h���C�o�����ݒ�l */
	INIT_SCR7								/* SCR7�̃h���C�o�����ݒ�l */
};

/* �V���A���R���g���[�����W�X�^�h���C�o�N���ݒ�e�[�u�� */
static UI_8 const C_SerialCtrlRegDrvActTbl[UARTDRV_CH_NUMBER] =
{
	DRV_ACT_SCR2,							/* SCR2�̃h���C�o�N���ݒ�l */
	DRV_ACT_SCR3,							/* SCR3�̃h���C�o�N���ݒ�l */
	DRV_ACT_SCR4,							/* SCR4�̃h���C�o�N���ݒ�l */
	DRV_ACT_SCR5,							/* SCR5�̃h���C�o�N���ݒ�l */
	DRV_ACT_SCR6,							/* SCR6�̃h���C�o�N���ݒ�l */
	DRV_ACT_SCR7							/* SCR7�̃h���C�o�N���ݒ�l */
};

/* �V���A���R���g���[�����W�X�^�h���C�o��~�ݒ�e�[�u�� */
static UI_8 const C_SerialCtrlRegDrvStopTbl[UARTDRV_CH_NUMBER] =
{
	DRV_STOP_SCR2,							/* SCR2�̃h���C�o��~�ݒ�l */
	DRV_STOP_SCR3,							/* SCR3�̃h���C�o��~�ݒ�l */
	DRV_STOP_SCR4,							/* SCR4�̃h���C�o��~�ݒ�l */
	DRV_STOP_SCR5,							/* SCR5�̃h���C�o��~�ݒ�l */
	DRV_STOP_SCR6,							/* SCR6�̃h���C�o��~�ݒ�l */
	DRV_STOP_SCR7							/* SCR7�̃h���C�o��~�ݒ�l */
};

/* �V���A���R���g���[�����W�X�^�h���C�o��~�e�[�u�� */
static UI_8 const C_SerialCtrlRegStopTbl[UARTDRV_CH_NUMBER] =
{
	STOP_SCR2,								/* SCR2�̃h���C�o��~�ݒ�l */
	STOP_SCR3,								/* SCR3�̃h���C�o��~�ݒ�l */
	STOP_SCR4,								/* SCR4�̃h���C�o��~�ݒ�l */
	STOP_SCR5,								/* SCR5�̃h���C�o��~�ݒ�l */
	STOP_SCR6,								/* SCR6�̃h���C�o��~�ݒ�l */
	STOP_SCR7								/* SCR7�̃h���C�o��~�ݒ�l */
};

/* �V���A�����[�h���W�X�^�h���C�o�������e�[�u�� */
static UI_8 const C_SerialModeRegInitTbl[UARTDRV_CH_NUMBER] =
{
	INIT_SMR2,								/* SMR2�̃h���C�o�����ݒ�l */
	INIT_SMR3,								/* SMR3�̃h���C�o�����ݒ�l */
	INIT_SMR4,								/* SMR4�̃h���C�o�����ݒ�l */
	INIT_SMR5,								/* SMR5�̃h���C�o�����ݒ�l */
	INIT_SMR6,								/* SMR6�̃h���C�o�����ݒ�l */
	INIT_SMR7								/* SMR7�̃h���C�o�����ݒ�l */
};

/* �V���A�����[�h���W�X�^�h���C�o�N���ݒ�e�[�u�� */
static UI_8 const C_SerialModeRegDrvActTbl[UARTDRV_CH_NUMBER] =
{
	DRV_ACT_SMR2,							/* SMR2�̃h���C�o�N���ݒ�l */
	DRV_ACT_SMR3,							/* SMR3�̃h���C�o�N���ݒ�l */
	DRV_ACT_SMR4,							/* SMR4�̃h���C�o�N���ݒ�l */
	DRV_ACT_SMR5,							/* SMR5�̃h���C�o�N���ݒ�l */
	DRV_ACT_SMR6,							/* SMR6�̃h���C�o�N���ݒ�l */
	DRV_ACT_SMR7							/* SMR7�̃h���C�o�N���ݒ�l */
};

/* �V���A�����[�h���W�X�^�h���C�o��~�ݒ�e�[�u�� */
static UI_8 const C_SerialModeRegDrvStopTbl[UARTDRV_CH_NUMBER] =
{
	DRV_STOP_SMR2,							/* SMR2�̃h���C�o��~�ݒ�l */
	DRV_STOP_SMR3,							/* SMR3�̃h���C�o��~�ݒ�l */
	DRV_STOP_SMR4,							/* SMR4�̃h���C�o��~�ݒ�l */
	DRV_STOP_SMR5,							/* SMR5�̃h���C�o��~�ݒ�l */
	DRV_STOP_SMR6,							/* SMR6�̃h���C�o��~�ݒ�l */
	DRV_STOP_SMR7							/* SMR7�̃h���C�o��~�ݒ�l */
};

/* �V���A�����[�h���W�X�^�h���C�o��~�e�[�u�� */
static UI_8 const C_SerialModeRegStopTbl[UARTDRV_CH_NUMBER] =
{
	STOP_SMR2,								/* SMR2�̃h���C�o��~�ݒ�l */
	STOP_SMR3,								/* SMR3�̃h���C�o��~�ݒ�l */
	STOP_SMR4,								/* SMR4�̃h���C�o��~�ݒ�l */
	STOP_SMR5,								/* SMR5�̃h���C�o��~�ݒ�l */
	STOP_SMR6,								/* SMR6�̃h���C�o��~�ݒ�l */
	STOP_SMR7								/* SMR7�̃h���C�o��~�ݒ�l */
};

/* �V���A���X�e�[�^�X���W�X�^�h���C�o�������e�[�u�� */
static UI_8 const C_SerialStatRegInitTbl[UARTDRV_CH_NUMBER] =
{
	INIT_SSR2,								/* SSR2�̃h���C�o�����ݒ�l */
	INIT_SSR3,								/* SSR3�̃h���C�o�����ݒ�l */
	INIT_SSR4,								/* SSR4�̃h���C�o�����ݒ�l */
	INIT_SSR5,								/* SSR5�̃h���C�o�����ݒ�l */
	INIT_SSR6,								/* SSR6�̃h���C�o�����ݒ�l */
	INIT_SSR7								/* SSR7�̃h���C�o�����ݒ�l */
};

/* �V���A���X�e�[�^�X���W�X�^�h���C�o�N���ݒ�e�[�u�� */
static UI_8 const C_SerialStatRegDrvActTbl[UARTDRV_CH_NUMBER] =
{
	DRV_ACT_SSR2,							/* SSR2�̃h���C�o�N���ݒ�l */
	DRV_ACT_SSR3,							/* SSR3�̃h���C�o�N���ݒ�l */
	DRV_ACT_SSR4,							/* SSR4�̃h���C�o�N���ݒ�l */
	DRV_ACT_SSR5,							/* SSR5�̃h���C�o�N���ݒ�l */
	DRV_ACT_SSR6,							/* SSR6�̃h���C�o�N���ݒ�l */
	DRV_ACT_SSR7							/* SSR7�̃h���C�o�N���ݒ�l */
};

/* �V���A���X�e�[�^�X���W�X�^�h���C�o��~�ݒ�e�[�u�� */
static UI_8 const C_SerialStatRegDrvStopTbl[UARTDRV_CH_NUMBER] =
{
	DRV_STOP_SSR2,							/* SSR2�̃h���C�o��~�ݒ�l */
	DRV_STOP_SSR3,							/* SSR3�̃h���C�o��~�ݒ�l */
	DRV_STOP_SSR4,							/* SSR4�̃h���C�o��~�ݒ�l */
	DRV_STOP_SSR5,							/* SSR5�̃h���C�o��~�ݒ�l */
	DRV_STOP_SSR6,							/* SSR6�̃h���C�o��~�ݒ�l */
	DRV_STOP_SSR7							/* SSR7�̃h���C�o��~�ݒ�l */
};

/* �V���A���X�e�[�^�X���W�X�^�h���C�o��~�e�[�u�� */
static UI_8 const C_SerialStatRegStopTbl[UARTDRV_CH_NUMBER] =
{
	STOP_SSR2,								/* SSR2�̃h���C�o��~�ݒ�l */
	STOP_SSR3,								/* SSR3�̃h���C�o��~�ݒ�l */
	STOP_SSR4,								/* SSR4�̃h���C�o��~�ݒ�l */
	STOP_SSR5,								/* SSR5�̃h���C�o��~�ݒ�l */
	STOP_SSR6,								/* SSR6�̃h���C�o��~�ݒ�l */
	STOP_SSR7								/* SSR7�̃h���C�o��~�ݒ�l */
};

/* �g���X�e�[�^�X���䃌�W�X�^�h���C�o�������e�[�u�� */
static UI_8 const C_ExtendCtrlRegInitTbl[UARTDRV_CH_NUMBER] =
{
	INIT_ESCR2,								/* ESCR2�̃h���C�o�����ݒ�l */
	INIT_ESCR3,								/* ESCR3�̃h���C�o�����ݒ�l */
	INIT_ESCR4,								/* ESCR4�̃h���C�o�����ݒ�l */
	INIT_ESCR5,								/* ESCR5�̃h���C�o�����ݒ�l */
	INIT_ESCR6,								/* ESCR6�̃h���C�o�����ݒ�l */
	INIT_ESCR7								/* ESCR7�̃h���C�o�����ݒ�l */
};

/* �g���X�e�[�^�X���䃌�W�X�^�h���C�o�N���ݒ�e�[�u�� */
static UI_8 const C_ExtendCtrlRegDrvActTbl[UARTDRV_CH_NUMBER] =
{
	DRV_ACT_ESCR2,							/* ESCR2�̃h���C�o�N���ݒ�l */
	DRV_ACT_ESCR3,							/* ESCR3�̃h���C�o�N���ݒ�l */
	DRV_ACT_ESCR4,							/* ESCR4�̃h���C�o�N���ݒ�l */
	DRV_ACT_ESCR5,							/* ESCR5�̃h���C�o�N���ݒ�l */
	DRV_ACT_ESCR6,							/* ESCR6�̃h���C�o�N���ݒ�l */
	DRV_ACT_ESCR7							/* ESCR7�̃h���C�o�N���ݒ�l */
};

/* �g���X�e�[�^�X���䃌�W�X�^�h���C�o��~�ݒ�e�[�u�� */
static UI_8 const C_ExtendCtrlRegDrvStopTbl[UARTDRV_CH_NUMBER] =
{
	DRV_STOP_ESCR2,							/* ESCR2�̃h���C�o��~�ݒ�l */
	DRV_STOP_ESCR3,							/* ESCR3�̃h���C�o��~�ݒ�l */
	DRV_STOP_ESCR4,							/* ESCR4�̃h���C�o��~�ݒ�l */
	DRV_STOP_ESCR5,							/* ESCR5�̃h���C�o��~�ݒ�l */
	DRV_STOP_ESCR6,							/* ESCR6�̃h���C�o��~�ݒ�l */
	DRV_STOP_ESCR7							/* ESCR7�̃h���C�o��~�ݒ�l */
};

/* �g���X�e�[�^�X���䃌�W�X�^�h���C�o��~�e�[�u�� */
static UI_8 const C_ExtendCtrlRegStopTbl[UARTDRV_CH_NUMBER] =
{
	STOP_ESCR2,								/* ESCR2�̃h���C�o��~�ݒ�l */
	STOP_ESCR3,								/* ESCR3�̃h���C�o��~�ݒ�l */
	STOP_ESCR4,								/* ESCR4�̃h���C�o��~�ݒ�l */
	STOP_ESCR5,								/* ESCR5�̃h���C�o��~�ݒ�l */
	STOP_ESCR6,								/* ESCR6�̃h���C�o��~�ݒ�l */
	STOP_ESCR7								/* ESCR7�̃h���C�o��~�ݒ�l */
};

/* �g���ʐM�R���g���[�����W�X�^�h���C�o�������e�[�u�� */
static UI_8 const C_ExtendCommunicationRegInitTbl[UARTDRV_CH_NUMBER] =
{
	INIT_ECCR2,								/* ECCR2�̃h���C�o�����ݒ�l */
	INIT_ECCR3,								/* ECCR3�̃h���C�o�����ݒ�l */
	INIT_ECCR4,								/* ECCR4�̃h���C�o�����ݒ�l */
	INIT_ECCR5,								/* ECCR5�̃h���C�o�����ݒ�l */
	INIT_ECCR6,								/* ECCR6�̃h���C�o�����ݒ�l */
	INIT_ECCR7								/* ECCR7�̃h���C�o�����ݒ�l */
};

/* �g���ʐM�R���g���[�����W�X�^�h���C�o�N���ݒ�e�[�u�� */
static UI_8 const C_ExtendCommunicationRegDrvAct[UARTDRV_CH_NUMBER] =
{
	DRV_ACT_ECCR2,							/* ECCR2�̃h���C�o�N���ݒ�l */
	DRV_ACT_ECCR3,							/* ECCR3�̃h���C�o�N���ݒ�l */
	DRV_ACT_ECCR4,							/* ECCR4�̃h���C�o�N���ݒ�l */
	DRV_ACT_ECCR5,							/* ECCR5�̃h���C�o�N���ݒ�l */
	DRV_ACT_ECCR6,							/* ECCR6�̃h���C�o�N���ݒ�l */
	DRV_ACT_ECCR7							/* ECCR7�̃h���C�o�N���ݒ�l */
};

/* �g���ʐM�R���g���[�����W�X�^�h���C�o��~�ݒ�e�[�u�� */
static UI_8 const C_ExtendCommunicationRegDrvStop[UARTDRV_CH_NUMBER] =
{
	DRV_STOP_ECCR2,							/* ECCR2�̃h���C�o��~�ݒ�l */
	DRV_STOP_ECCR3,							/* ECCR3�̃h���C�A��~�ݒ�l */
	DRV_STOP_ECCR4,							/* ECCR4�̃h���C�A��~�ݒ�l */
	DRV_STOP_ECCR5,							/* ECCR5�̃h���C�A��~�ݒ�l */
	DRV_STOP_ECCR6,							/* ECCR6�̃h���C�A��~�ݒ�l */
	DRV_STOP_ECCR7							/* ECCR7�̃h���C�A��~�ݒ�l */
};

/* �g���ʐM�R���g���[�����W�X�^�h���C�o��~�e�[�u�� */
static UI_8 const C_ExtendCommunicationRegStopTbl[UARTDRV_CH_NUMBER] =
{
	STOP_ECCR2,								/* ECCR2�̃h���C�o��~�ݒ�l */
	STOP_ECCR3,								/* ECCR3�̃h���C�A��~�ݒ�l */
	STOP_ECCR4,								/* ECCR4�̃h���C�A��~�ݒ�l */
	STOP_ECCR5,								/* ECCR5�̃h���C�A��~�ݒ�l */
	STOP_ECCR6,								/* ECCR6�̃h���C�A��~�ݒ�l */
	STOP_ECCR7								/* ECCR7�̃h���C�A��~�ݒ�l */
};

/* �{�[���[�g�W�F�l���[�^���W�X�^�h���C�o�������e�[�u�� */
static UI_16 const C_BaudRateRegInitTbl[UARTDRV_CH_NUMBER] =
{
	INIT_BGR2,								/* BGR2�̃h���C�o�����ݒ�l */
	INIT_BGR3,								/* BGR3�̃h���C�o�����ݒ�l */
	INIT_BGR4,								/* BGR4�̃h���C�o�����ݒ�l */
	INIT_BGR5,								/* BGR5�̃h���C�o�����ݒ�l */
	INIT_BGR6,								/* BGR6�̃h���C�o�����ݒ�l */
	INIT_BGR7								/* BGR7�̃h���C�o�����ݒ�l */
};

/* �{�[���[�g�W�F�l���[�^���W�X�^�h���C�o�N���ݒ�e�[�u�� */
static UI_16 const C_BaudRateRegDrvActTbl[UARTDRV_CH_NUMBER] =
{
	DRV_ACT_BGR2,							/* BGR2�̃h���C�o�N���ݒ�l */
	DRV_ACT_BGR3,							/* BGR3�̃h���C�o�N���ݒ�l */
	DRV_ACT_BGR4,							/* BGR4�̃h���C�o�N���ݒ�l */
	DRV_ACT_BGR5,							/* BGR5�̃h���C�o�N���ݒ�l */
	DRV_ACT_BGR6,							/* BGR6�̃h���C�o�N���ݒ�l */
	DRV_ACT_BGR7							/* BGR7�̃h���C�o�N���ݒ�l */
};

/* �{�[���[�g�W�F�l���[�^���W�X�^�h���C�o��~�ݒ�e�[�u�� */
static UI_16 const C_BaudRateRegDrvStopTbl[UARTDRV_CH_NUMBER] =
{
	DRV_STOP_BGR2,							/* BGR2�̃h���C�o��~�ݒ�l */
	DRV_STOP_BGR3,							/* BGR3�̃h���C�o��~�ݒ�l */
	DRV_STOP_BGR4,							/* BGR4�̃h���C�o��~�ݒ�l */
	DRV_STOP_BGR5,							/* BGR5�̃h���C�o��~�ݒ�l */
	DRV_STOP_BGR6,							/* BGR6�̃h���C�o��~�ݒ�l */
	DRV_STOP_BGR7							/* BGR7�̃h���C�o��~�ݒ�l */
};

/* �{�[���[�g�W�F�l���[�^���W�X�^�h���C�o��~�e�[�u�� */
static UI_16 const C_BaudRateRegStopTbl[UARTDRV_CH_NUMBER] =
{
	STOP_BGR2,								/* BGR2�̃h���C�o��~�ݒ�l */
	STOP_BGR3,								/* BGR3�̃h���C�o��~�ݒ�l */
	STOP_BGR4,								/* BGR4�̃h���C�o��~�ݒ�l */
	STOP_BGR5,								/* BGR5�̃h���C�o��~�ݒ�l */
	STOP_BGR6,								/* BGR6�̃h���C�o��~�ݒ�l */
	STOP_BGR7								/* BGR7�̃h���C�o��~�ݒ�l */
};

typedef struct {
	B_8						is_valid;
	T_UartDrv_Protocol		protocol;
}T_UartDrv_Protocol_config;			/* �ʐM�ݒ��� */

static T_UartDrv_Protocol_config	S_uart_drv_protocol[UARTDRV_CH_NUMBER];

/****************************************************************************************/
/*	[���W���[����]	uart_drv_init		[����]	UART�h���C�o�֘A���W�X�^����������(����)*/
/*======================================================================================*/
/*	[�����T�v]	���W�X�^�����ݒ莞�ɖ{�֐����ĂсA���������s���B						*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 uart_drv_init( E_UARTDRV_CH ch)									*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH			ch				: ����ُ��						*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8	tmp_rc		: ����(D_OK)�A�ُ�(D_NG)��Ԃ�						*/
/*======================================================================================*/
/*	[ ��  �l ]	����M�Ƃ��̊����݋��r�b�g���֎~�ɐݒ肵����Ԃ�						*/
/*				�v���O���}�u���N���A�i�\�t�g�E�F�A���Z�b�g�j���s��						*/
/****************************************************************************************/
static SI_8 uart_drv_init( E_UARTDRV_CH ch )
{
	SI_8	tmp_rc;		/* �߂�l */
	UI_8	smr_work;	/* �V���A�����[�h���W�X�^���Z�pwork�ϐ� */
	UI_8	uart_mode;	/* UART�h���C�o�̓��샂�[�h */
	volatile UI_8	dummy_rx_buf;

	if ( ch < UARTDRV_CH_NUMBER ) {
		/* UART�h���C�o���W�X�^�����ݒ菈�� */
		dummy_rx_buf				= *C_ReceiveDataReg[ch];		/* ��M�f�[�^�̓ǂݏo���ɂ�銄���ݗv���t���O�N���A */

		/* ���[�h�ݒ�i�vUPCL���Z�b�g�j */
		uart_mode					= (C_SerialModeRegInitTbl[ch] & SMR_MD_MASK);	/* ���샂�[�h�̐ݒ�l�𒊏o */
		smr_work					= *C_SerialModeReg[ch];
		smr_work					|= uart_mode;
		*C_SerialModeReg[ch]		= smr_work;										/* ���샂�[�h�ݒ� */

		/* �������[�h�F�X�^�[�g/�X�g�b�v�r�b�g�̕t���ݒ�i�vUPCL���Z�b�g�j */
		*C_ExtendCommunicationReg[ch]	= C_ExtendCommunicationRegInitTbl[ch];	/* �g���ʐM�R���g���[�����W�X�^�ݒ� */

		if ( (uart_mode & SMR_MD_MASK) == UARTDRV_MODE_CLOCK ) {				/* UPCL���Z�b�g�F�������[�h�̏ꍇ */
			smr_work					= *C_SerialModeReg[ch];
			smr_work					|= SMR_UPCL_SET;
			*C_SerialModeReg[ch]		= smr_work;				/* ���[�h�E�X�^�[�g/�X�g�b�v�r�b�g�̕t���ύX�ɂ��UPCL���Z�b�g */
		}

		*C_ExtendCtrlReg[ch]		= C_ExtendCtrlRegInitTbl[ch];	/* �g���X�e�[�^�X���䃌�W�X�^�ݒ�i�N���b�N���]���AUPCL���Z�b�gNG�j */
		*C_SerialCtrlReg[ch]		= C_SerialCtrlRegInitTbl[ch];	/* �V���A���R���g���[�����W�X�^�F�f�[�^�t�H�[�}�b�g�ݒ�i�vUPCL���Z�b�g�j */
		*C_SerialModeReg[ch]		= C_SerialModeRegInitTbl[ch];	/* �V���A�����[�h���W�X�^�F���[�h�ݒ� */
		*C_SerialStatReg[ch]		= C_SerialStatRegInitTbl[ch];	/* �V���A���X�e�[�^�X���W�X�^�ݒ� */
		*C_BaudRateReg[ch]			= C_BaudRateRegInitTbl[ch];		/* �{�[���[�g�W�F�l���[�^���W�X�^�ݒ� */

		if ( (uart_mode & SMR_MD_MASK) == UARTDRV_MODE_UART ) {					/* UPCL���Z�b�g�F�񓯊����[�h�̏ꍇ */
			/* �\�t�g���Z�b�g: ����M�̒�~�Ɗ����ݗv���̃N���A */
			smr_work					= *C_SerialModeReg[ch];
			smr_work					|= SMR_UPCL_SET;
			*C_SerialModeReg[ch]		= smr_work;				/* ���[�h�ύX�A�f�[�^�t�H�[�}�b�g�ύX�ɂ��UPCL���Z�b�g */
		}

		S_uart_drv_protocol[ch].is_valid = D_FALSE;

		tmp_rc = D_OK;
	} else {
		tmp_rc = D_NG;
	}
	return tmp_rc;
}

/****************************************************************************************/
/*	[���W���[����]	uart_drv_stop		[����]	UART�h���C�o�֘A���W�X�^��~����(����)	*/
/*======================================================================================*/
/*	[�����T�v]	���W�X�^�����ݒ莞�ɖ{�֐����ĂсA��~���s���B							*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 uart_drv_stop( E_UARTDRV_CH ch)									*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH			ch				: ����ُ��						*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8	tmp_rc		: ����(D_OK)�A�ُ�(D_NG)��Ԃ�						*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static SI_8 uart_drv_stop( E_UARTDRV_CH ch )
{
	SI_8	tmp_rc;		/* Ch.�͈͊O�`�F�b�N */
	volatile UI_8	dummy_rx_buf;

	if ( ch < UARTDRV_CH_NUMBER ) {
		/* UART�h���C�o���W�X�^��~�ݒ菈�� */
		dummy_rx_buf				= *C_ReceiveDataReg[ch];	/* ��M�f�[�^�̓ǂݏo���ɂ�銄���ݗv���t���O�N���A */
		*C_ExtendCtrlReg[ch]		= C_ExtendCtrlRegStopTbl[ch];	/* �g���X�e�[�^�X���䃌�W�X�^�ݒ� */
		*C_SerialCtrlReg[ch]		= C_SerialCtrlRegStopTbl[ch];	/* �V���A���R���g���[�����W�X�^�F�f�[�^�t�H�[�}�b�g�ݒ� */
		*C_SerialModeReg[ch]		= C_SerialModeRegStopTbl[ch];	/* �V���A�����[�h���W�X�^�F���[�h�ݒ� */
		*C_SerialStatReg[ch]		= C_SerialStatRegStopTbl[ch];	/* �V���A���X�e�[�^�X���W�X�^�ݒ� */
		*C_ExtendCommunicationReg[ch]	= C_ExtendCommunicationRegStopTbl[ch];/* �g���ʐM�R���g���[�����W�X�^�ݒ� */
		*C_BaudRateReg[ch]			= C_BaudRateRegStopTbl[ch];		/* �{�[���[�g�W�F�l���[�^���W�X�^�ݒ� */
		tmp_rc = D_OK;
	} else {
		tmp_rc = D_NG;
	}
	return tmp_rc;
}

#if (UART_DRV_TX == 1U) /* ���M���� */
/****************************************************************************************/
/*	[���W���[����]	uart_drv_txdrive		[����]	UART��ײ�ޑ��M�N������				*/
/*======================================================================================*/
/*	[�����T�v]	LIN-UARTڼ޽�����ײ�ޑ��M�N���ݒ�ɂ���									*/
/*======================================================================================*/
/*	[�L�q�`��]	static void uart_drv_txdrive( E_UARTDRV_CH ch )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH				ch					: ����ُ��				*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	�V���A���R���g���[�����W�X�^�A�V���A���X�e�[�^�X���W�X�^��				*/
/*				�h���C�o���M�N���ɐݒ肷��O�ɁA���M�f�[�^���W�X�^�Ƀ_�~�[���C�g���s��	*/
/*				�v���O���}�u���N���A�i�\�t�g�E�F�A���Z�b�g�j���s���Ă��܂��B			*/
/****************************************************************************************/
static void uart_drv_txdrive( E_UARTDRV_CH ch )
{
	UI_8 smr_work; /* �V���A�����[�h���W�X�^���Z�pwork�ϐ� */
	UI_8 uart_mode; /* UART�h���C�o�̓��샂�[�h */
	UI_16 bgr_act_value;
	UI_8 scr_act_value;
	UI_8 ssr_act_value;

	*C_ExtendCtrlReg[ch]		= C_ExtendCtrlRegDrvActTbl[ch];			/* �g���X�e�[�^�X���䃌�W�X�^�A�h���X�e�[�u�� */
	*C_ExtendCommunicationReg[ch]	= C_ExtendCommunicationRegDrvAct[ch];	/* �g���ʐM�R���g���[�����W�X�^�A�h���X�e�[�u�� */
	bgr_act_value				= uart_drv_get_bgr_act_value(ch);
	*C_BaudRateReg[ch]			= bgr_act_value;						/* �{�[���[�g�W�F�l���[�^���W�X�^�A�h���X�e�[�u�� */
	*C_SerialModeReg[ch]		= C_SerialModeRegDrvActTbl[ch];			/* �V���A�����[�h���W�X�^�A�h���X�e�[�u�� */
	*C_TransDataReg[ch]			= DUMMY_WRITE;							/* ���M�f�[�^���W�X�^�A�h���X�e�[�u�� �� UART���M�p�_�~�[���C�g�f�[�^ */

	uart_mode					= (C_SerialModeRegDrvActTbl[ch] & SMR_MD_MASK);	/* ���샂�[�h�̐ݒ�l�𒊏o */
	if ( (uart_mode & SMR_MD_MASK) == UARTDRV_MODE_UART  ) {			/* �񓯊����[�h�̏ꍇ */
		smr_work					= *C_SerialModeReg[ch];
		smr_work					|= SMR_UPCL_SET;
		*C_SerialModeReg[ch]		= smr_work;							/* �V���A�����[�h���W�X�^�A�h���X�e�[�u�� | �v���O���}�u���N���A�r�b�g�Z�b�g�f�[�^ */
	}

	scr_act_value				= uart_drv_get_scr_act_value(ch);
	ssr_act_value				= uart_drv_get_ssr_act_value(ch);
	*C_SerialCtrlReg[ch]		= scr_act_value;						/* �V���A���R���g���[�����W�X�^�A�h���X�e�[�u�� */
	*C_SerialStatReg[ch]		= ssr_act_value;						/* �V���A���X�e�[�^�X���W�X�^�A�h���X�e�[�u�� */
}
#endif /* UART_DRV_TX */

#if (UART_DRV_TX == 1U) /* ���M���� */
/****************************************************************************************/
/*	[���W���[����]	uart_drv_txstop			[����]	UART��ײ�ޑ��M��~����				*/
/*======================================================================================*/
/*	[�����T�v]	LIN-UART���W�X�^���h���C�o���M��~�ݒ�ɂ���B							*/
/*======================================================================================*/
/*	[�L�q�`��]	static void uart_drv_txstop( E_UARTDRV_CH ch )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH				ch					: ����ُ��				*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static void uart_drv_txstop( E_UARTDRV_CH ch )
{
	*C_ExtendCtrlReg[ch]		= C_ExtendCtrlRegDrvStopTbl[ch];		/* �g���X�e�[�^�X���䃌�W�X�^�A�h���X�e�[�u�� */
	*C_SerialStatReg[ch]		= C_SerialStatRegDrvStopTbl[ch];		/* �V���A���X�e�[�^�X���W�X�^�A�h���X�e�[�u�� */
	*C_SerialCtrlReg[ch]		= C_SerialCtrlRegDrvStopTbl[ch];		/* �V���A���R���g���[�����W�X�^�A�h���X�e�[�u�� */
	*C_SerialModeReg[ch]		= C_SerialModeRegDrvStopTbl[ch];		/* �V���A�����[�h���W�X�^�A�h���X�e�[�u�� */
	*C_BaudRateReg[ch]			= C_BaudRateRegDrvStopTbl[ch];			/* �{�[���[�g�W�F�l���[�^���W�X�^�A�h���X�e�[�u�� */
	*C_ExtendCommunicationReg[ch]	= C_ExtendCommunicationRegDrvStop[ch];	/* �g���ʐM�R���g���[�����W�X�^�A�h���X�e�[�u�� */
}
#endif /* UART_DRV_TX */

#if (UART_DRV_RX == 1U) /* ��M���� */
/****************************************************************************************/
/*	[���W���[����]	uart_drv_rxdrive		[����]	UART��ײ�ގ�M�N������				*/
/*======================================================================================*/
/*	[�����T�v]	LIN-UART���W�X�^���h���C�o��M�N���ݒ�ɂ���B							*/
/*======================================================================================*/
/*	[�L�q�`��]	static void uart_drv_rxdrive( E_UARTDRV_CH ch )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH				ch					: ����ُ��				*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static void uart_drv_rxdrive( E_UARTDRV_CH ch )
{
	volatile UI_8 dummy_rx_buf;
	UI_8 smr_work; /* �V���A�����[�h���W�X�^���Z�pwork�ϐ� */
	UI_16 bgr_act_value;
	UI_8 scr_act_value;
	UI_8 ssr_act_value;
	
	dummy_rx_buf				= *C_ReceiveDataReg[ch];				/* UART��M�p�_�~�[�o�b�t�@ �� ��M�f�[�^���W�X�^�A�h���X�e�[�u�� */
	*C_ExtendCtrlReg[ch]		= C_ExtendCtrlRegDrvActTbl[ch];			/* �g���X�e�[�^�X���䃌�W�X�^�A�h���X�e�[�u�� */
	*C_ExtendCommunicationReg[ch]	= C_ExtendCommunicationRegDrvAct[ch];	/* �g���ʐM�R���g���[�����W�X�^�A�h���X�e�[�u�� */
	bgr_act_value				= uart_drv_get_bgr_act_value(ch);
	*C_BaudRateReg[ch]			= bgr_act_value;						/* �{�[���[�g�W�F�l���[�^���W�X�^�A�h���X�e�[�u�� */
	smr_work					= *C_SerialModeReg[ch];
	smr_work					|= SMR_UPCL_SET;
	*C_SerialModeReg[ch]		= smr_work;								/* �V���A�����[�h���W�X�^�A�h���X�e�[�u�� | �v���O���}�u���N���A�r�b�g�Z�b�g�f�[�^ */
	scr_act_value				= uart_drv_get_scr_act_value(ch);
	ssr_act_value				= uart_drv_get_ssr_act_value(ch);
	*C_SerialCtrlReg[ch]		= scr_act_value;						/* �V���A���R���g���[�����W�X�^�A�h���X�e�[�u�� */
	*C_SerialStatReg[ch]		= ssr_act_value;						/* �V���A���X�e�[�^�X���W�X�^�A�h���X�e�[�u�� */
}
#endif /* UART_DRV_RX */

#if (UART_DRV_RX == 1U) /* ��M���� */
/****************************************************************************************/
/*	[���W���[����]	uart_drv_rxstop			[����]	UART��ײ�ގ�M��~����				*/
/*======================================================================================*/
/*	[�����T�v]	LIN-UART���W�X�^���h���C�o��M��~�ݒ�ɂ���B							*/
/*======================================================================================*/
/*	[�L�q�`��]	static void uart_drv_rxstop( E_UARTDRV_CH ch )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH				ch					: ����ُ��				*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static void uart_drv_rxstop( E_UARTDRV_CH ch )
{
	volatile UI_8 dummy_rx_buf;
	
	*C_ExtendCtrlReg[ch]		= C_ExtendCtrlRegDrvStopTbl[ch];		/* �g���X�e�[�^�X���䃌�W�X�^�A�h���X�e�[�u�� */
	*C_SerialStatReg[ch]		= C_SerialStatRegDrvStopTbl[ch];		/* �V���A���X�e�[�^�X���W�X�^�A�h���X�e�[�u�� */
	*C_SerialCtrlReg[ch]		= C_SerialCtrlRegDrvStopTbl[ch];		/* �V���A���R���g���[�����W�X�^�A�h���X�e�[�u�� */
	*C_SerialModeReg[ch]		= C_SerialModeRegDrvStopTbl[ch];		/* �V���A�����[�h���W�X�^�A�h���X�e�[�u�� */
	*C_BaudRateReg[ch]			= C_BaudRateRegDrvStopTbl[ch];			/* �{�[���[�g�W�F�l���[�^���W�X�^�A�h���X�e�[�u�� */
	*C_ExtendCommunicationReg[ch]	= C_ExtendCommunicationRegDrvStop[ch];	/* �g���ʐM�R���g���[�����W�X�^�A�h���X�e�[�u�� */
	dummy_rx_buf				= *C_ReceiveDataReg[ch];				/* UART��M�p�_�~�[�o�b�t�@ �� ��M�f�[�^���W�X�^�A�h���X�e�[�u�� */
}
#endif /* UART_DRV_RX */

/****************************************************************************************/
/*	[���W���[����]	uart_drv_get_scr_act_value	[����]	�V���A���R���g���[�����W�X�^�N���l�擾	*/
/*======================================================================================*/
/*	[�����T�v]	�V���A���R���g���[�����W�X�^�N���l���擾����B							*/
/*======================================================================================*/
/*	[�L�q�`��]	static UI_8 uart_drv_get_scr_act_value( E_UARTDRV_CH ch )				*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH				ch					: ����ُ��				*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	UI_8	�V���A���R���g���[�����W�X�^�N���l								*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static UI_8 uart_drv_get_scr_act_value( E_UARTDRV_CH ch )
{

	UI_8 scr_act_value;

	/* �V���A���R���g���[�����W�X�^�h���C�o�N���ݒ�e�[�u������擾	*/
	scr_act_value = C_SerialCtrlRegDrvActTbl[ch];

	if (S_uart_drv_protocol[ch].is_valid == D_TRUE) {
		/* �ʐM�������ݒ肳��Ă���ꍇ �ݒ肳��Ă���l�ōX�V����	*/

		/* �p���e�B�ݒ�	*/
		if (S_uart_drv_protocol[ch].protocol.parity == UARTDRV_PARITY_EVEN) {
			/* �����p���e�B	*/
			scr_act_value |= SCR_PEN_SET;
			scr_act_value &= SCR_P_CLR;
		}
		else if (S_uart_drv_protocol[ch].protocol.parity == UARTDRV_PARITY_ODD) {
			/* ��p���e�B	*/
			scr_act_value |= SCR_PEN_SET;
			scr_act_value |= SCR_P_SET;
		}
		else if (S_uart_drv_protocol[ch].protocol.parity == UARTDRV_PARITY_NONE) {
			/* �p���e�B�Ȃ�	*/
			scr_act_value &= SCR_PEN_CLR;
		}
		else {
			/* nop */
		}

		/* �X�g�b�v�r�b�g���ݒ�	*/
		if (S_uart_drv_protocol[ch].protocol.stop_bit_len == UARTDRV_STOP_BIT_LEN_1) {
			/* 1�r�b�g	*/
			scr_act_value &= SCR_SBL_CLR;
		}
		else if (S_uart_drv_protocol[ch].protocol.stop_bit_len == UARTDRV_STOP_BIT_LEN_2) {
			/* 2�r�b�g	*/
			scr_act_value |= SCR_SBL_SET;
		}
		else {
			/* nop */
		}

		/* �f�[�^���ݒ�	*/
		if (S_uart_drv_protocol[ch].protocol.bit_len == UARTDRV_BIT_LEN_7) {
			/* 7�r�b�g	*/
			scr_act_value &= SCR_CL_CLR;
		}
		else if (S_uart_drv_protocol[ch].protocol.bit_len == UARTDRV_BIT_LEN_8) {
			/* 8�r�b�g	*/
			scr_act_value |= SCR_CL_SET;
		}
		else {
			/* nop */
		}
	}

	return scr_act_value;

}

/****************************************************************************************/
/*	[���W���[����]	uart_drv_get_ssr_act_value	[����]	�V���A���X�e�[�^�X���W�X�^�N���l�擾	*/
/*======================================================================================*/
/*	[�����T�v]	�V���A���X�e�[�^�X���W�X�^�N���l���擾����B							*/
/*======================================================================================*/
/*	[�L�q�`��]	static UI_8 uart_drv_get_ssr_act_value( E_UARTDRV_CH ch )				*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH				ch					: ����ُ��				*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	UI_8	�V���A���X�e�[�^�X���W�X�^�N���l								*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static UI_8 uart_drv_get_ssr_act_value( E_UARTDRV_CH ch )
{

	UI_8 ssr_act_value;

	/* �V���A���X�e�[�^�X���W�X�^�h���C�o�N���ݒ�e�[�u������擾	*/
	ssr_act_value = C_SerialStatRegDrvActTbl[ch];

	if (S_uart_drv_protocol[ch].is_valid == D_TRUE) {
		/* �ʐM�������ݒ肳��Ă���ꍇ �ݒ肳��Ă���l�ōX�V����	*/

		/* �]�������ݒ�	*/
		if (S_uart_drv_protocol[ch].protocol.direction == UARTDRV_DIRECTION_LSB_FIRST) {
			/* LSB�t�@�[�X�g	*/
			ssr_act_value &= SSR_BDS_CLR;
		}
		else if (S_uart_drv_protocol[ch].protocol.direction == UARTDRV_DIRECTION_MSB_FIRST) {
			/* MSB�t�@�[�X�g	*/
			ssr_act_value |= SSR_BDS_SET;
		}
		else {
			/* nop */
		}
	}

	return ssr_act_value;

}

/****************************************************************************************/
/*	[���W���[����]	uart_drv_get_bgr_act_value	[����]	�{�[���[�g�W�F�l���[�^���W�X�^�N���l�擾	*/
/*======================================================================================*/
/*	[�����T�v]	�{�[���[�g�W�F�l���[�^���W�X�^�N���l���擾����B						*/
/*======================================================================================*/
/*	[�L�q�`��]	static UI_8 uart_drv_get_bgr_act_value( E_UARTDRV_CH ch )				*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH				ch					: ����ُ��				*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	UI_16	�{�[���[�g�W�F�l���[�^���W�X�^�N���l							*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static UI_16 uart_drv_get_bgr_act_value( E_UARTDRV_CH ch )
{

	UI_16 bgr_act_value;

	/* �{�[���[�g�W�F�l���[�^���W�X�^�h���C�o�N���ݒ�e�[�u������擾	*/
	bgr_act_value = C_BaudRateRegDrvActTbl[ch];

	if (S_uart_drv_protocol[ch].is_valid == D_TRUE) {
		/* �ʐM�������ݒ肳��Ă���ꍇ �ݒ肳��Ă���l�ōX�V����	*/
		/* ���W�X�^�ݒ�l��(�����N���b�N16MHz/�{�[���[�g�j- 1	*/
		switch (S_uart_drv_protocol[ch].protocol.baud_rate) {
		case UARTDRV_BAUD_RATE_9600:
			/* 9600bps		*/
			bgr_act_value = 1666;
			break;

		case UARTDRV_BAUD_RATE_14400:
			/* 14400bps		*/
			bgr_act_value = 1110;
			break;

		case UARTDRV_BAUD_RATE_19200:
			/* 19200bps		*/
			bgr_act_value = 832;
			break;

		case UARTDRV_BAUD_RATE_38400:
			/* 38400bps		*/
			bgr_act_value = 416;
			break;

		case UARTDRV_BAUD_RATE_57600:
			/* 57600bps		*/
			bgr_act_value = 277;
			break;

		case UARTDRV_BAUD_RATE_115200:
			/* 115200bps	*/
			bgr_act_value = 138;
			break;

		default:
			break;
		}
	}

	return bgr_act_value;

}

/****************************************************************************************/
/*	[���W���[����]	UartDrv_InitRequest	[����]	UART�h���C�o�֘A���W�X�^�������v������	*/
/*======================================================================================*/
/*	[�����T�v]	�������v�����ɖ{�֐����ĂсA���W�X�^�������֐��̌Ăяo�����s���B		*/
/*======================================================================================*/
/*	[�L�q�`��]	void UartDrv_InitRequest( E_UARTDRV_CH ch )								*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH			ch				: ����ُ��						*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void UartDrv_InitRequest( E_UARTDRV_CH ch )
{
	/* UART�h���C�o���W�X�^�����ݒ菈�� */
	(void)uart_drv_init( ch );
}

#if (UART_DRV_TX == 1U) /* ���M���� */
/****************************************************************************************/
/*	[���W���[����]	UartDrv_ReqTxCtrl		[����]	�رْʐM��ײ�ޑ��M���䏈��			*/
/*======================================================================================*/
/*	[�����T�v]	�V���A���ʐM�h���C�o�̑��M������s���B									*/
/*				UART���s��Ԃ��N���Ȃ�AUART�h���C�o���M�N���������Ăяo���B			*/
/*				UART���s��Ԃ���~�Ȃ�AUART�h���C�o���M��~�������Ăяo���B			*/
/*				�߂�l�ɁA�w��`���l���̎��s�����X�e�[�^�X��Ԃ��B						*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 UartDrv_ReqTxCtrl( E_UARTDRV_CH ch, UI_8 act )						*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH		ch				: ����ُ��							*/
/*				UI_8				act				: UART���s���						*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8				stat			: �w�����ق̎��s�����ð��			*/
/*======================================================================================*/
/*	[ ��  �l ]	�����`���l���ɑ��M�Ǝ�M�𓯎��Ɋ��蓖�Ă��Ȃ����߁A					*/
/*				�V���A���ʐM�h���C�o��M���䏈���Ɠ����ɌĂяo���Ȃ����ƁB				*/
/****************************************************************************************/
SI_8 UartDrv_ReqTxCtrl( E_UARTDRV_CH ch, UI_8 act )
{
	SI_8 stat;
	
	stat = D_OK;
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		if ( act == D_ENABLE ) {					/* �w��CH��UART�̎��s��� */
			/* UART���s��Ԃ��N���Ȃ�� */
			uart_drv_txdrive(ch);					/* UART�h���C�o���M�N������ */
		} else {
			/* UART���s��Ԃ���~�Ȃ�� */
			uart_drv_txstop(ch);					/* UART�h���C�o���M��~���� */
		}
	} else {
		stat = D_NG;
	}
	
	return stat;
}
#endif /* UART_DRV_TX */

#if (UART_DRV_RX == 1U) /* ��M���� */
/****************************************************************************************/
/*	[���W���[����]	UartDrv_ReqRxCtrl	[����]	�رْʐM��ײ�ގ�M���䏈��				*/
/*======================================================================================*/
/*	[�����T�v]	�V���A���ʐM�h���C�o�̎�M������s���B									*/
/*				UART���s��Ԃ��N���Ȃ�AUART�h���C�o��M�N���������Ăяo���B			*/
/*				UART���s��Ԃ���~�Ȃ�AUART�h���C�o��M��~�������Ăяo���B			*/
/*				�߂�l�ɁA�w��`���l���̎��s�����X�e�[�^�X��Ԃ��B						*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 UartDrv_ReqRxCtrl( E_UARTDRV_CH ch, UI_8 act )						*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH			ch			: ����ُ��							*/
/*				UI_8					act			: UART���s���						*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8					stat		: �w�����ق̎��s�����ð��			*/
/*======================================================================================*/
/*	[ ��  �l ]	�����`���l���ɑ��M�Ǝ�M�𓯎��Ɋ��蓖�Ă��Ȃ����߁A					*/
/*				�V���A���ʐM�h���C�o���M���䏈���Ɠ����ɌĂяo���Ȃ����ƁB				*/
/****************************************************************************************/
SI_8 UartDrv_ReqRxCtrl( E_UARTDRV_CH ch, UI_8 act )
{
	SI_8 stat;
	
	stat = D_OK;
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		if ( act == D_ENABLE ) {				/* �w��CH��UART�̎��s��� */
			/* UART���s��Ԃ��N���Ȃ�� */
			uart_drv_rxdrive(ch);				/* UART�h���C�o��M�N������ */
		} else {
			/* UART���s��Ԃ���~�Ȃ�� */
			uart_drv_rxstop(ch);				/* UART�h���C�o��M��~���� */
		}
	} else {
		stat = D_NG;
	}
	
	return stat;
}
#endif /* UART_DRV_RX */

#if (UART_DRV_TX == 1U) /* ���M���� */
/****************************************************************************************/
/*	[���W���[����]	UartDrv_SetTxData	[����]	�رْʐM���M�ް��ݒ菈��				*/
/*======================================================================================*/
/*	[�����T�v]	���M�f�[�^�̐ݒ���s���B												*/
/*				���M�f�[�^���W�X�^�ɑ��M�f�[�^��ݒ肵�܂��B							*/
/*				�߂�l�ɁA�w��`���l���̎��s�����X�e�[�^�X��Ԃ��܂��B					*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 UartDrv_SetTxData( E_UARTDRV_CH ch, UI_8 tx_data)					*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH			ch			: ����ُ��							*/
/*				UI_8					tx_data		: ���M�ް�							*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8					stat		: �w�����ق̎��s�����ð��			*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
SI_8 UartDrv_SetTxData( E_UARTDRV_CH ch, UI_8 tx_data )
{
	SI_8 stat;
	
	stat = D_OK;
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		*C_TransDataReg[ch] = tx_data;			/* ���M�f�[�^���W�X�^ �� ���M�f�[�^ */
	} else {
		stat = D_NG;
	}
		
	return stat;
}
#endif /* UART_DRV_TX */

/****************************************************************************************/
/*	[���W���[����]	UartDrv_GetTransDataAddress	[����]	UART���M�f�[�^�A�h���X�擾		*/
/*======================================================================================*/
/*	[�����T�v]	UART�̑��M�f�[�^�A�h���X���擾����B									*/
/*======================================================================================*/
/*	[�L�q�`��]	T_UartTransDataAddress UartDrv_GetTransDataAddress( E_UARTDRV_CH ch )	*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH			ch				: ����ُ��						*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	T_UartTransDataAddress	tx_data_addr		: ���M�f�[�^�A�h���X��Ԃ�	*/
/*======================================================================================*/
/*	[ ��  �l ]	�֐��̌Ăяo�����ŕK��ch�͈̔͊O�`�F�b�N�����鎖						*/
/*				���M/��M�̃f�[�^���W�X�^�A�h���X�������ׁA������̏ꍇ�ɂ��g�p�\�B	*/
/****************************************************************************************/
T_UartTransDataAddress UartDrv_GetTransDataAddress( E_UARTDRV_CH ch )
{
	T_UartTransDataAddress	tx_data_addr;

	tx_data_addr = (T_UartTransDataAddress)C_TransDataReg[ch];

	return tx_data_addr;
}

/****************************************************************************************/
/*	[���W���[����]	UartDrv_ChangeProtocol	[����]	UART�h���C�o�ʐM�����ύX����		*/
/*======================================================================================*/
/*	[�����T�v]	�ʐM�����̓��I�ؑւ��s���B												*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 UartDrv_ChangeProtocol(E_UARTDRV_CH ch,							*/
/*													const T_UartDrv_Protocol* protocol)	*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH			ch				: ����ُ��						*/
/*				const T_UartDrv_Protocol	*protocol	: �ʐM�ݒ���					*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8									: �w�����ق̎��s�����ð��		*/
/*======================================================================================*/
/*	[ ��  �l ]	�ύX�����ʐM�ݒ�́AUartDrv_ReqTxCtrl�܂���UartDrv_ReqRxCtrl�ɂ��		*/
/*				�N�������^�C�~���O�ŗL���ɂȂ�											*/
/****************************************************************************************/
SI_8 UartDrv_ChangeProtocol(E_UARTDRV_CH ch, const T_UartDrv_Protocol* protocol)
{
	SI_8 stat;
	
	stat = D_OK;
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		S_uart_drv_protocol[ch].protocol = *protocol;
		S_uart_drv_protocol[ch].is_valid = D_TRUE;
	} else {
		stat = D_NG;
	}
	
	return stat;

}

#if (UART_DRV_RX == 1U) /* ��M���� */
/****************************************************************************************/
/*	[���W���[����]	UartDrv_GetRxData	[����]	�رْʐM��M�ް��擾����				*/
/*======================================================================================*/
/*	[�����T�v]	��M�f�[�^�̎擾���s���B												*/
/*				��M�f�[�^�R�s�[��A�h���X�Ɏw��`���l���̎�M�f�[�^���W�X�^�̃f�[�^��	*/
/*				�擾���܂��B															*/
/*				�߂�l�ɁA�w��`���l���̎��s�����X�e�[�^�X��Ԃ��܂��B					*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 UartDrv_GetRxData( E_UARTDRV_CH ch, UI_8 *rx_data )				*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH			ch					: ����ُ��					*/
/*				UI_8 *					rx_data				: ��M�ް���߰����ڽ		*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8					stat				: �w�����ق̎��s�����ð��	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
SI_8 UartDrv_GetRxData( E_UARTDRV_CH ch, UI_8 *rx_data )
{
	SI_8 stat;
	
	stat = D_OK;
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		*rx_data = *C_ReceiveDataReg[ch];		/* ��M�f�[�^�R�s�[��A�h���X �� ��M�f�[�^���W�X�^ */
	} else {
		stat = D_NG;
	}
	
	return stat;
}
#endif /* UART_DRV_RX */

#if (UART_DRV_TX == 1U) /* ���M���� */
/****************************************************************************************/
/*	[���W���[����]	UartDrv_TxIntrReqEnblCtrl[����]	���M�����ݗv�������ď���		*/
/*======================================================================================*/
/*	[�����T�v]	�V���A���ʐM�̑��M�����ݗv���������Z�b�g/�N���A����B				*/
/*======================================================================================*/
/*	[�L�q�`��]	void UartDrv_TxIntrReqEnblCtrl( E_UARTDRV_CH ch, UI_8 act )				*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH			ch			: ����ُ��							*/
/*				UI_8					act			: ������(D_ON/D_OFF)				*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void UartDrv_TxIntrReqEnblCtrl( E_UARTDRV_CH ch, UI_8 act )
{
	UI_8 ssr_work; /* �V���A���X�e�[�^�X���W�X�^���Z�pwork�ϐ� */

	if ( ch < UARTDRV_CH_NUMBER ) {
		ssr_work = *C_SerialStatReg[ch];
		if ( act == D_ON ) {	/* �Z�b�g�� */
			ssr_work |= SSR_TIE_SET;	/* ���M�����ݗv���������Z�b�g */
		} else {
			ssr_work &= SSR_TIE_CLR;	/* ���M�����ݗv���������N���A */
		}
		*C_SerialStatReg[ch] = ssr_work;
	}
}
#endif /* UART_DRV_TX */

#if (UART_DRV_RX == 1U) /* ��M���� */
/****************************************************************************************/
/*	[���W���[����]	UartDrv_RxIntrReqEnblCtrl[����]	��M�����ݗv�������ď���		*/
/*======================================================================================*/
/*	[�����T�v]	�V���A���ʐM�̎�M�����ݗv���������Z�b�g/�N���A����					*/
/*======================================================================================*/
/*	[�L�q�`��]	void UartDrv_RxIntrReqEnblCtrl( E_UARTDRV_CH ch, UI_8 act )				*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH			ch			: ����ُ��							*/
/*				UI_8					act			: ������(D_ON/D_OFF)				*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void UartDrv_RxIntrReqEnblCtrl( E_UARTDRV_CH ch, UI_8 act )
{
	UI_8 ssr_work; /* �V���A���X�e�[�^�X���W�X�^���Z�pwork�ϐ� */

	if ( ch < UARTDRV_CH_NUMBER ) {
		ssr_work = *C_SerialStatReg[ch];
		if ( act == D_ON ) {	/* �Z�b�g�� */
			ssr_work |= SSR_RIE_SET;	/* ��M�����ݗv���������Z�b�g */
		} else {
			ssr_work &= SSR_RIE_CLR;	/* ��M�����ݗv���������N���A */
		}
		*C_SerialStatReg[ch] = ssr_work;
	}
}
#endif /* UART_DRV_RX */

#if (UART_DRV_TX == 1U) /* ���M���� */
/****************************************************************************************/
/*	[���W���[����]	UartDrv_GetTxDataEmpty	[����]	���M�ް�����è���擾����			*/
/*======================================================================================*/
/*	[�����T�v]	�V���A���ʐM�ŁA���M�f�[�^�̃G���v�e�B�������J����B					*/
/*				LIN-UART���ق̎w�肪UARTDRV_CH_2�`UARTDRV_CH_7�ȊO�̏ꍇ�́A			*/
/*				�K�����M�f�[�^�Ȃ���Ԃ��܂��B											*/
/*				�w��`���l���̑��M�f�[�^�G���v�e�B�t���O�𔻒肵�A						*/
/*				OFF�Ȃ瑗�M�f�[�^����AON�Ȃ瑗�M�f�[�^�Ȃ���Ԃ��܂��B					*/
/*======================================================================================*/
/*	[�L�q�`��]	UI_8 UartDrv_GetTxDataEmpty(E_UARTDRV_CH ch )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH			ch						: ����ُ��				*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	UI_8					empty_info		: �w�����ق̑��M�ް�����è���	*/
/*======================================================================================*/
/*	[ ��  �l ]																			*/
/*																						*/
/****************************************************************************************/
UI_8 UartDrv_GetTxDataEmpty( E_UARTDRV_CH ch )
{
	UI_8 empty_info;
	UI_8 ssr_temp; /* �V���A���X�e�[�^�X���W�X�^�i�[�p�ϐ� */
	
	empty_info = D_OFF;					/* ���M�f�[�^�Ȃ� */
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		ssr_temp = *C_SerialStatReg[ch];
		if ( (ssr_temp & SSR_TDRE_MASK) == D_OFF ) {
			empty_info = D_ON;			/* ���M�f�[�^���� */
		}
	}
	
	return empty_info;
}
#endif /* UART_DRV_TX */

#if (UART_DRV_RX == 1U) /* ��M���� */
/****************************************************************************************/
/*	[���W���[����]	UartDrv_GetErr		[����]	��M�װ���擾����						*/
/*======================================================================================*/
/*	[�����T�v]	��M�G���[���̎擾���s���B											*/
/*				LIN-UART�`���l���̎w�肪UARTDRV_CH_2�`UARTDRV_CH_7�ȊO�̏ꍇ�́A		*/
/*				�G���[�X�e�[�^�X���G���[�Ȃ��ɂ��A���ُ��͈̔͐����Ԃ��܂��B		*/
/*				�w��`���l���̃G���[�t���O�ɉ����āA�G���[���̊Y���r�b�g��1�ɂ��܂��B	*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 UartDrv_GetErr( E_UARTDRV_CH ch, T_UartDrv_ErrInfo err )			*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH			ch					: ����ُ��					*/
/*				T_UartDrv_ErrInfo *		err					: �װ���					*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8					stat				: �w�����ق̎��s�����ð��	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
SI_8 UartDrv_GetErr( E_UARTDRV_CH ch, T_UartDrv_ErrInfo *err )
{
	SI_8 stat;
	UI_8 ssr_work;	/* �V���A���X�e�[�^�X���W�X�^��Ɨp */
	
	stat = D_OK;				/* ���ُ�񐳏� */
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		/* �V���A���X�e�[�^�X���W�X�^�̐ݒ�l���擾 */
		ssr_work = *C_SerialStatReg[ch];
		
		/* �G���[���̏����� */
		err->parity_flg = D_OFF;
		err->overrun_flg = D_OFF;
		err->flaming_flg = D_OFF;
		
		/* �p���e�B�G���[ */
		if ( (ssr_work & SSR_PARITY_MASK) != D_OFF ) {
			err->parity_flg = D_ON;
		}
		/* �I�[�o�[�����G���[ */
		if ( (ssr_work & SSR_OVERRUN_MASK) != D_OFF ) {
			err->overrun_flg = D_ON;
		}
		/* �t���[�~���O�G���[ */
		if ( (ssr_work & SSR_FLAMING_MASK) != D_OFF ) {
			err->flaming_flg = D_ON;
		}
	} else {
		stat = D_NG;				/* ���ُ��ُ� */
	}
	return stat;
}
#endif /* UART_DRV_RX */

#if (UART_DRV_RX == 1U) /* ��M���� */
/****************************************************************************************/
/*	[���W���[����]	UartDrv_ReqErrClr		[����]	��M�װ���N���A����				*/
/*======================================================================================*/
/*	[�����T�v]	��M�G���[���N���A�������s���B										*/
/*======================================================================================*/
/*	[�L�q�`��]	void UartDrv_ReqErrClr( E_UARTDRV_CH ch )								*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH			ch						: ����ُ��				*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void UartDrv_ReqErrClr( E_UARTDRV_CH ch )
{
	UI_8 serial_ctrl_reg;
	
	if ( ch < UARTDRV_CH_NUMBER ) {							/* �w��ch���͈͓� */
		serial_ctrl_reg = *C_SerialCtrlReg[ch];				/* �V���A���R���g���[�����W�X�^��Ǐo�� */
		serial_ctrl_reg |= SCR_RX_ERR_CLR_BIT_ON;			/* ��M�G���[�t���O�N���Abit��ON�ɂ��� */
		*C_SerialCtrlReg[ch] = serial_ctrl_reg;
	}
}
#endif /* UART_DRV_RX */

#if (UART_DRV_RX == 1U) /* ��M���� */
/****************************************************************************************/
/*	[���W���[����]	UartDrv_GetRxFullFlag	[����]	��M�ް����׸ގ擾����				*/
/*======================================================================================*/
/*	[�����T�v]	��M�f�[�^�t���t���O�̎擾���s���B										*/
/*======================================================================================*/
/*	[�L�q�`��]	UI_8 UartDrv_GetRxFullFlag( E_UARTDRV_CH ch )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_UARTDRV_CH			ch						: ����ُ��				*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	UI_8				rx_full_flag			: �w�����ق̎�M�ް����׸�	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
UI_8 UartDrv_GetRxFullFlag( E_UARTDRV_CH ch )
{
	UI_8 rx_full_flag;
	UI_8 ssr_temp; /* �V���A���X�e�[�^�X���W�X�^�i�[�p�ϐ� */
	
	rx_full_flag = D_OFF;				/* ��M�f�[�^�t���t���OOFF */
	
	if ( ch < UARTDRV_CH_NUMBER ) {
		ssr_temp = *C_SerialStatReg[ch];
		if ( (ssr_temp & SSR_RDRF_MASK) != D_OFF ) {
			rx_full_flag = D_ON;		/* ��M�f�[�^�t���t���OON */
		}
	}
	
	return rx_full_flag;
}
#endif /* UART_DRV_RX */

/****************************************************************************************/
/*	[���W���[����]	UartDrv_Init	[����]	UARTڼ޽������ݒ菈��						*/
/*======================================================================================*/
/*	[�����T�v]	���W�X�^�����ݒ莞�ɖ{�֐����ĂсA���������s���B						*/
/*======================================================================================*/
/*	[�L�q�`��]	void UartDrv_Init( E_INIT_TYPE req  )									*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_INIT_TYPE				req		�F�N�����								*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void UartDrv_Init( E_INIT_TYPE req )
{
	UI_8 cnt;

	switch ( req ) {
	case E_INIT_RESET:			/* CPU���Z�b�g�ɂ��N�� */
	case E_INIT_WAKEUP:			/* �ȓd�͉����iWAKEUP�j�ɂ��N�� */
		/* UART���W�X�^�����ݒ菈�� */
		for ( cnt = 0U; cnt < UARTDRV_USEDCH_NUMBER; cnt++) {
			(void)uart_drv_init( C_UartUsedChTbl[cnt] );
		}
		break;
	case E_INIT_IGN_ON:			/* IGN ON */
		break;
	case E_INIT_RET_NORMAL_VOL:	/* ��d�����A */
		break;
	case E_INIT_INTERVAL_WAKEUP:	/* �Ԍ��N���E�F�C�N�A�b�v */
		break;
	default:					/* ��L�ȊO */
		break;
	}
	
}

/****************************************************************************************/
/*	[���W���[����]	UartDrv_Sleep	[����]	UARTڼ޽���~�ݒ菈��						*/
/*======================================================================================*/
/*	[�����T�v]	�ȓd�̓��[�h�J�ڎ��ɖ{�֐����ĂсA���������s���B						*/
/*======================================================================================*/
/*	[�L�q�`��]	void UartDrv_Sleep( void )												*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void UartDrv_Sleep( void )
{
	UI_8 cnt;

	/* UART���W�X�^��~�ݒ菈�� */
	for ( cnt = 0U; cnt < UARTDRV_USEDCH_NUMBER; cnt++) {
		(void)uart_drv_stop( C_UartUsedChTbl[cnt] );
	}
}
/****************************************************************************************/
/*	[���W���[����]	UartDrv_Refresh	[����]	UARTڼ޽��Đݒ菈��							*/
/*======================================================================================*/
/*	[�����T�v]	���W�X�^�Đݒ莞�ɖ{�֐����ĂсA���������s���B							*/
/*======================================================================================*/
/*	[�L�q�`��]	void UartDrv_Refresh( void )											*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void UartDrv_Refresh( void )
{
	
	/* ��֐� */
	
}

