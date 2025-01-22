/********************************************************/
/* file reg_setting.h									*/
/* ���W�X�^�ݒ�p�w�b�_�t�@�C��							*/
/* 														*/
/* ���W�X�^�̐ݒ������B								*/
/* <ul>													*/
/* <li>���W�X�^�̏��������s��							*/
/* <li>���W�X�^�̍Đݒ���s��							*/
/* <li>10msec�����̃^�C�}�����݂��J�n����				*/
/* <li>�E�H�b�`�h�b�O�^�C�}�����Z�b�g����				*/
/* </ul>												*/
/* 														*/
/********************************************************/
/* author      Y.Sorimachi								*/
/* date        2013/--/--								*/
/********************************************************/
#include "reg_setting.h"
#include "SSFTSTD_Type.h"
#include "SSFTSTD_Macro.h"
#include "SHRVSTD_CPU_Drv.h"
#include "SHRVSTD_Interrupt_Drv.h"
#include "SHRVSTD_Wdt_Drv.h"
#include "SHRVSTD_Timer_Drv.h"
#include "SHRVSTD_Port_Drv.h"
#include "SHRVSTD_UART_Drv.h"
#include "SHRVSTD_ADC_Drv.h"
#include "SHRVSTD_PWM_Drv.h"

/********************************************************/
/*	�����}�N��											*/
/********************************************************/
#define D_1MTIMER_RELOADTIMER_CYCLE			(999U)		/* 1msec���̃T�C�N����-1 (1cycle=1us, 1msec:1000cycle-1=999(999�J�n->0�I���̂���-1����)) */
#define D_MAIN_TASK_RELOADTIMER_CYCLE		(4999U)		/* 5msec���̃T�C�N����-1 (1cycle=1us, 5msec:5000cycle-1=4999(4999�J�n->0�I���̂���-1����)) */

/********************************************************/
/*	�����֐��v���g�^�C�v�錾							*/
/********************************************************/

/********************************************************/
/*	�萔�錾											*/
/********************************************************/

/********************************************************/
/*	�\���̐錾											*/
/********************************************************/

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/

/********************************************************/
/*	�O���֐�											*/
/********************************************************/

/** 
 * /// ���W�X�^������
 *
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		���W�X�^�̏������������s���B
 */ 
void Reg_setting_Init( void )
{	
	/* �������֐��̈�����E_INIT_RESET�ł悢�i�d��OFF��Ԃ���̋N���݂̂ŏȓd�̓��[�h����̕��A�Ȃǂ͂Ȃ����߁j */

	/* �V�X�e���N���b�N�h���C�o���W�X�^������ */
	SysClkDrv_Init(E_INIT_RESET);	

	/* ���Z�b�g�h���C�o���W�X�^������ */
	RstDrv_Init(E_INIT_RESET);

	/* �����@�\�h���C�o���W�X�^������ */
	IntrDrv_Init(E_INIT_RESET);

	/* WDT�h���C�o������ */
	WdtDrv_Init(E_INIT_RESET);

	/* �����[�h�^�C�}����ڼ޽������� */
	ReloadTmDrv_Init(E_INIT_RESET);

	/* GPIO�h���C�o���W�X�^������ */
	PortDrv_Init(E_INIT_RESET);

#ifdef _VDV_BOOT
	/* �t���b�V���h���C�o���W�X�^������ */
	FlashDrv_Init(E_INIT_RESET);
	FlashCtrlDrv_Init(E_INIT_RESET);
#endif /* _VDV_BOOT */

#ifdef _VDV_APP
	/* UART���W�X�^������ */
	UartDrv_Init(E_INIT_RESET);

	/* A/D�h���C�o���W�X�^������ */
	ADCDrv_Init(E_INIT_RESET);
#endif /* _VDV_APP */

	/* PWM�h���C�o���W�X�^���� */
	PWMDrv_Init(E_INIT_RESET);

#ifdef _VDV_APP
#endif	/* _VDV_APP */

	return;
}

/********************************************************/
/* �֐�����		: Reg_setting_Term						*/
/* �@�\�̐���	: ���W�X�^�I���ݒ�						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 16/10/21								*/
/* ���l			: 										*/
/********************************************************/
void Reg_setting_Term( void )
{	
	/* PWM�h���C�o��~ */
	PWMDrv_Sleep();

#ifdef _VDV_APP
	/* A/D�h���C�o��~ */
	ADCDrv_Sleep();

	/* UART�h���C�o��~ */
	UartDrv_Sleep();
#endif /* _VDV_APP */

#ifdef _VDV_BOOT
	/* �t���b�V���h���C�o��~ */
	FlashDrv_Sleep();
#endif /* _VDV_BOOT */

	/* GPIO�h���C�o��~ */
	PortDrv_Sleep();

	/* �����[�h�^�C�}�h���C�o��~ */
	ReloadTmDrv_Sleep();

	/* WDT�h���C�o��~ */
	WdtDrv_Sleep();

	/* �����@�\�h���C�o��~ */
	IntrDrv_Sleep();

	/* ���Z�b�g�h���C�o��~ */
	RstDrv_Sleep();

	/* �V�X�e���N���b�N�h���C�o��~ */
	SysClkDrv_Sleep();	

	return;
}

/** 
 * /// �^�C�}�����݊J�n
 *
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		10msec�����̃^�C�}�����݂��J�n����B
 */ 
void Reg_setting_TimerStart( void )
{
	ReloadTmDrv_Ctrl(RELOADTIMER_CH_1M_TIME_COUNT, D_1MTIMER_RELOADTIMER_CYCLE, D_ENABLE);		/* �o�ߎ����J�E���g�p�^�C�}�i1ms�j�N�� */
	ReloadTmDrv_Ctrl(RELOADTIMER_CH_MAIN_TASK, D_MAIN_TASK_RELOADTIMER_CYCLE, D_ENABLE);		/* ���C����������p�^�C�}�i5ms�j�N�� */

	return;
}

/** 
 * /// �E�H�b�`�h�b�O�^�C�}���Z�b�g
 *
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		�E�H�b�`�h�b�O�^�C�}�����Z�b�g����B
 */ 
void Reg_setting_WtdRestart( void )
{
	/**	<ol>	*/
	/**	<li>HW�E�H�b�`�h�b�O�^�C�}�����Z�b�g����	*/
	WdtDrv_ReqClrCpu();

	return;
}

/** 
 * /// �����݃}�X�N���ݒ�
 */ 
void Reg_setting_EnableIntMask(void)
{
	(void)IntrDrv_SetMaskLevel(INTR_ENABLE_LEVEL);
}

/** 
 * /// �����݃}�X�N�֎~�ݒ�
 */ 
void Reg_setting_DisableIntMask(void)
{
	(void)IntrDrv_SetMaskLevel(INTR_DISABLE_LEVEL);
}
