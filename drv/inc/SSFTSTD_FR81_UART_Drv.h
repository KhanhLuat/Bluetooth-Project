/************************************************************************************************/
/* �q�於		:	�W�����W���[��																*/
/* �@�햼		:	SSFT																		*/
/* ϲ�ݿ��ðϖ�	:	PF																			*/
/*==============================================================================================*/
/* �쐬̧�ٖ�	:	SSFTSTD_FR81_UART_Drv.h														*/
/* 				:	UART��ײ��ͯ��																*/
/*==============================================================================================*/
/* �Ώ�ϲ��		:	MB91570 Series																*/
/*==============================================================================================*/
/* �쐬�ް�ޮ�	:	020201																		*/
/* �쐬�N����	:	2015.1.29																	*/
/* �쐬��		:	k.uchiyama																	*/
/*----------------------------------------------------------------------------------------------*/
/* �ύX����		:																				*/
/* [010101]		:	�V�K�쐬																	*/
/* 12.09.28		:	�`���l��2 �V�t�^�[�C���W�P�[�^UART����Ή�									*/
/* 12.10.11		:	�`���l��3 �C���W�P�[�^ASSY UART����Ή�										*/
/* 12.11.10		:	�`���l��7 HUD ���[�^ASSY UART����Ή�										*/
/* [010102]		:	�W��IF�Ή��BIF�֐��̖߂�lSI_8���B�֐����̕ύX�B							*/
/*				:	���M�����ݗv�������Z�b�g/�N���A�����̓���								*/
/*				:	��M�����ݗv�������Z�b�g/�N���A�����̓���								*/
/* [010104]		:	�����֐����̕ύX�ɔ����h���C�o�o�[�W�����̍X�V								*/
/* [020101]		:	IF�̒ǉ��i������, �ʐM�����ؑ�, �f�[�^���W�X�^�A�h���X�擾)					*/
/* [020201]		:	LCD�N���b�N�o�͋@�\��ǉ�����												*/
/************************************************************************************************/
#ifndef __MB91570_UART_DRV_H__
#define __MB91570_UART_DRV_H__

/*** START_INC ***/
/********************************************************************************/
/*   Include File                                                               */
/*------------------------------------------------------------------------------*/
/*      ͯ�ް̧�ق̲ݸٰ�ޕ��́A���Ļ�قɋL�ڂ��邱��                          */
/********************************************************************************/
#include "SSFTxxx_FR81_UART_Drv_Config.h"

/*==============================================================================*/
/*	enum��`�i�O�����J)															*/
/*==============================================================================*/
/* �f�[�^�r�b�g��	*/
typedef enum {
	UARTDRV_BIT_LEN_7,						/* 7�r�b�g					*/
	UARTDRV_BIT_LEN_8						/* 8�r�b�g					*/
}E_UARTDRV_BIT_LEN;

/* �p���e�B	*/
typedef enum {
	UARTDRV_PARITY_NONE,					/* �p���e�B�Ȃ�				*/
	UARTDRV_PARITY_EVEN,					/* �����p���e�B				*/
	UARTDRV_PARITY_ODD						/* ��p���e�B				*/
}E_UARTDRV_PARITY;

/* �X�g�b�v�r�b�g��	*/
typedef enum {
	UARTDRV_STOP_BIT_LEN_1,					/* 1�r�b�g					*/
	UARTDRV_STOP_BIT_LEN_2					/* 2�r�b�g					*/
}E_UARTDRV_STOP_BIT_LEN;

/* �]������	*/
typedef enum {
	UARTDRV_DIRECTION_LSB_FIRST,			/* LSB�t�@�[�X�g			*/
	UARTDRV_DIRECTION_MSB_FIRST				/* MSB�t�@�[�X�g			*/
}E_UARTDRV_DIRECTION;

/* �{�[���[�g	*/
typedef enum {
	UARTDRV_BAUD_RATE_9600,					/* 9600bps					*/
	UARTDRV_BAUD_RATE_14400,				/* 14400bps					*/
	UARTDRV_BAUD_RATE_19200,				/* 19200bps					*/
	UARTDRV_BAUD_RATE_38400,				/* 38400bps					*/
	UARTDRV_BAUD_RATE_57600,				/* 57600bps					*/
	UARTDRV_BAUD_RATE_115200				/* 115200bps				*/
}E_UARTDRV_BAUD_RATE;

/*==============================================================================*/
/*	typedef��`�i�O�����J�j														*/
/*==============================================================================*/
typedef struct {
	UI_8	parity_flg;				/* �p���e�B�G���[�t���O */
	UI_8	overrun_flg;			/* �I�[�o�����G���[�t���O */
	UI_8	flaming_flg;			/* �t���[�~���O�G���[�t���O */
}T_UartDrv_ErrInfo;					/* �G���[��� */

typedef struct {
	E_UARTDRV_BIT_LEN			bit_len;			/* �r�b�g��				*/
	E_UARTDRV_PARITY			parity;				/* �p���e�B				*/
	E_UARTDRV_STOP_BIT_LEN		stop_bit_len;		/* �X�g�b�v�r�b�g��		*/
	E_UARTDRV_DIRECTION			direction;			/* �]������				*/
	E_UARTDRV_BAUD_RATE			baud_rate;			/* �{�[���[�g			*/
}T_UartDrv_Protocol;				/* �ʐM�ݒ��� */

/* FR81�}�C�R���p�^��`	*/
typedef UI_32 T_UartTransDataAddress;


/*==============================================================================*/
/*	�萔��`�i�O�����J�j														*/
/*==============================================================================*/
#ifdef	_VERSION_CONST_DEFINE
UI_8 const C_UARTDRV_VER[6] = {		/* UART�h���C�o �h���C�o�o�[�W����No(020201) */
	0x00U,							/* xx"0" */
	0x02U,							/* xx"2" */
	0x00U,							/* yy"0" */
	0x02U,							/* yy"2" */
	0x00U,							/* zz"0" */
	0x01U							/* zz"1" */
};
#endif	/*	_VERSION_CONST_DEFINE	*/

/*==============================================================================*/
/*	�֐��������ߐ錾�i�O�����J�j												*/
/*==============================================================================*/

#if (UART_DRV_TX == 1U) /* ���M���� */
extern SI_8 UartDrv_ReqTxCtrl( E_UARTDRV_CH ch, UI_8 act );
extern SI_8 UartDrv_SetTxData( E_UARTDRV_CH ch, UI_8 tx_data );
extern void UartDrv_TxIntrReqEnblCtrl( E_UARTDRV_CH ch, UI_8 act );
extern UI_8 UartDrv_GetTxDataEmpty( E_UARTDRV_CH ch );
#endif /* UART_DRV_TX */

#if (UART_DRV_RX == 1U) /* ��M���� */
extern SI_8 UartDrv_ReqRxCtrl( E_UARTDRV_CH ch, UI_8 act );
extern SI_8 UartDrv_GetRxData( E_UARTDRV_CH ch, UI_8 *rx_data );
extern void UartDrv_RxIntrReqEnblCtrl( E_UARTDRV_CH ch, UI_8 act );
extern SI_8 UartDrv_GetErr( E_UARTDRV_CH ch, T_UartDrv_ErrInfo *err );
extern void UartDrv_ReqErrClr( E_UARTDRV_CH ch );
extern UI_8 UartDrv_GetRxFullFlag( E_UARTDRV_CH ch );
#endif /* UART_DRV_RX */

extern SI_8 UartDrv_ChangeProtocol(E_UARTDRV_CH ch, const T_UartDrv_Protocol* protocol);
extern T_UartTransDataAddress UartDrv_GetTransDataAddress( E_UARTDRV_CH ch );
extern void UartDrv_InitRequest( E_UARTDRV_CH ch );
extern void UartDrv_Init( E_INIT_TYPE req );
extern void UartDrv_Sleep( void );
extern void UartDrv_Refresh( void );

#endif	/*	__MB91570_UART_DRV_H__	*/

