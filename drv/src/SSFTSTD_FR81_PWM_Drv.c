/********************************************************************************/
/* �q�於		:	�W�����W���[��												*/
/* �@�햼		:	SSFT														*/
/* ϲ�ݿ��ðϖ�	:	PF															*/
/*==============================================================================*/
/* �쐬̧�ٖ�	:	SSFTSTD_FR81_PWM_Drv.c										*/
/* 				:	PWM���W���[��												*/
/*==============================================================================*/
/* �Ώ�ϲ��		:	MB91570 Series												*/
/*==============================================================================*/
/* �쐬�ް�ޮ�	:	010104														*/
/* �쐬�N����	:	2013.05.23													*/
/* �쐬��		:	K.Uchiyama													*/
/*------------------------------------------------------------------------------*/
/* �ύX����		:																*/
/* [010101]		:	�V�K�쐬													*/
/* [010102]		:																*/
/*	12.11.27	:	�ɐ��A�g���K�[�Ȃǂ�PWM�ݒ�̕ύX�Ȃ�����					*/
/*				:		PWM���ăX�^�[�g�ƂȂ�Ȃ��悤�ɑΉ�						*/
/* [010103]		:																*/
/* 	13.2.8		:	FR81S�Ή�													*/
/* [010104]		:	�W��IF�Ή��B�N����ʂ̖��̕ύX								*/
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

#define	 __MB91570_PWM_DRV_INTERNAL__
#include "SSFTSTD_FR81_PWM_Drv.h"

/*==============================================================================*/
/*	typedef��`�i�O������J�j													*/
/*==============================================================================*/



/*==============================================================================*/
/*	�֐��������ߐ錾�i�O������J)												*/
/*==============================================================================*/


/*==============================================================================*/
/*	�����萔																	*/
/*==============================================================================*/
/* PPG����X�e�[�^�X���W�X�^�EOSEL �r�b�g�ʒu�}�X�N */
#define PCN_OSEL_BIT_MASK		(0x0001U)
/* PPG����X�e�[�^�X���W�X�^�EIRS �r�b�g�ʒu�}�X�N */
#define PCN_IRS_BIT_MASK		(0x000CU)
/* PPG����X�e�[�^�X���W�X�^�EPGMS �r�b�g�ʒu�}�X�N */
#define PCN_PGMS_BIT_MASK		(0x0200U)
/* PPG����X�e�[�^�X���W�X�^�ESTRG �r�b�g�ʒu�}�X�N */
#define PCN_STRG_BIT_MASK		(0x4000U)
/* PPG����X�e�[�^�X���W�X�^�ECNTE �r�b�g�ʒu�}�X�N */
#define PCN_CNTE_BIT_MASK		(0x8000U)
/* PPG����X�e�[�^�X���W�X�^�ECKS �r�b�g�ʒu�}�X�N */
#define PCN_CKS_BIT_MASK		(0x0C00U)
/* PPG����X�e�[�^�X���W�X�^�EIRQF �r�b�g�N���A�p�}�X�N */
#define PCN_IRQF_CLR_MASK		(0xFFEFU)
/* PPG����X�e�[�^�X���W�X�^�EIREN,IRQF �r�b�g�N���A�p�}�X�N */
#define PCN_IREN_IRQF_CLR_MASK	(0xFFCFU)
/* PPG����X�e�[�^�X���W�X�^�EIREN�r�b�g�Z�b�g */
#define PCN_IREN_SET			(0x0020U)
/* PPG����X�e�[�^�X���W�X�^ PWM�i�o�̓}�X�N�����j���̋N���ݒ� */
#define PCN_RUN_WITH_PWM		( PCN_CNTE_BIT_MASK | PCN_STRG_BIT_MASK )
/* PPG����X�e�[�^�X���W�X�^ �Œ�o�́i�o�̓}�X�N�j���̋N���ݒ� */
#define PCN_RUN_WITH_MASK		( PCN_CNTE_BIT_MASK | PCN_STRG_BIT_MASK  | PCN_PGMS_BIT_MASK )
/* PPG����X�e�[�^�X���W�X�^ �L���r�b�g�}�X�N */
#define PCN_USED_BIT_MASK		( PCN_CNTE_BIT_MASK | PCN_STRG_BIT_MASK  | PCN_PGMS_BIT_MASK  | PCN_CKS_BIT_MASK | PCN_OSEL_BIT_MASK | PCN_IRS_BIT_MASK)
/* PPG����X�e�[�^�X���W�X�^�ݒ�l */
#define PCN_SET_VALUE			(0xFFFFU)
/* �����ݒ�l����萔 */
#define CYCLE_JUDGMENT			(0x0001)
/* Duty�ݒ�l����萔 */
#define DUTY_JUDGMENT			(0x0000)
/* ���Z�萔 */
#define PWM_SUBTRACTION			(0x0001)
/* PPG�f���[�e�B�ݒ背�W�X�^�N���A�萔 */
#define DUTY_CLEA				(0x0000)


/* PPG����X�e�[�^�X���W�X�^�N���ݒ�e�[�u�� */
static UI_16 const C_PcnSettingTbl[PPG_CH_NUMBER] = 
{
	INIT_PCN_CH0,	/* PPG����X�e�[�^�X���W�X�^0 �����ݒ�l */
	INIT_PCN_CH1,	/* PPG����X�e�[�^�X���W�X�^1 �����ݒ�l */
	INIT_PCN_CH2,	/* PPG����X�e�[�^�X���W�X�^2 �����ݒ�l */
	INIT_PCN_CH3,	/* PPG����X�e�[�^�X���W�X�^3 �����ݒ�l */
	INIT_PCN_CH4,	/* PPG����X�e�[�^�X���W�X�^4 �����ݒ�l */
	INIT_PCN_CH5,	/* PPG����X�e�[�^�X���W�X�^5 �����ݒ�l */
	INIT_PCN_CH6,	/* PPG����X�e�[�^�X���W�X�^6 �����ݒ�l */
	INIT_PCN_CH7,	/* PPG����X�e�[�^�X���W�X�^7 �����ݒ�l */
	INIT_PCN_CH8,	/* PPG����X�e�[�^�X���W�X�^8 �����ݒ�l */
	INIT_PCN_CH9,	/* PPG����X�e�[�^�X���W�X�^9 �����ݒ�l */
	INIT_PCN_CH10,	/* PPG����X�e�[�^�X���W�X�^10 �����ݒ�l */
	INIT_PCN_CH11,	/* PPG����X�e�[�^�X���W�X�^11 �����ݒ�l */
	INIT_PCN_CH12,	/* PPG����X�e�[�^�X���W�X�^12 �����ݒ�l */
	INIT_PCN_CH13,	/* PPG����X�e�[�^�X���W�X�^13 �����ݒ�l */
	INIT_PCN_CH14,	/* PPG����X�e�[�^�X���W�X�^14 �����ݒ�l */
	INIT_PCN_CH15,	/* PPG����X�e�[�^�X���W�X�^15 �����ݒ�l */
	INIT_PCN_CH16,	/* PPG����X�e�[�^�X���W�X�^16 �����ݒ�l */
	INIT_PCN_CH17,	/* PPG����X�e�[�^�X���W�X�^17 �����ݒ�l */
	INIT_PCN_CH18,	/* PPG����X�e�[�^�X���W�X�^18 �����ݒ�l */
	INIT_PCN_CH19,	/* PPG����X�e�[�^�X���W�X�^19 �����ݒ�l */
	INIT_PCN_CH20,	/* PPG����X�e�[�^�X���W�X�^20 �����ݒ�l */
	INIT_PCN_CH21,	/* PPG����X�e�[�^�X���W�X�^21 �����ݒ�l */
	INIT_PCN_CH22,	/* PPG����X�e�[�^�X���W�X�^22 �����ݒ�l */
	INIT_PCN_CH23	/* PPG����X�e�[�^�X���W�X�^23 �����ݒ�l */
};

/* PPG����X�e�[�^�X���W�X�^��~�ݒ�e�[�u�� */
static UI_16 const C_PcnCtrlStopRegTbl[PPG_CH_NUMBER] = 
{
	STOP_PCN_CH0,	/* PPG����X�e�[�^�X���W�X�^0 ��~�ݒ�l */
	STOP_PCN_CH1,	/* PPG����X�e�[�^�X���W�X�^1 ��~�ݒ�l */
	STOP_PCN_CH2,	/* PPG����X�e�[�^�X���W�X�^2 ��~�ݒ�l */
	STOP_PCN_CH3,	/* PPG����X�e�[�^�X���W�X�^3 ��~�ݒ�l */
	STOP_PCN_CH4,	/* PPG����X�e�[�^�X���W�X�^4 ��~�ݒ�l */
	STOP_PCN_CH5,	/* PPG����X�e�[�^�X���W�X�^5 ��~�ݒ�l */
	STOP_PCN_CH6,	/* PPG����X�e�[�^�X���W�X�^6 ��~�ݒ�l */
	STOP_PCN_CH7,	/* PPG����X�e�[�^�X���W�X�^7 ��~�ݒ�l */
	STOP_PCN_CH8,	/* PPG����X�e�[�^�X���W�X�^8 ��~�ݒ�l */
	STOP_PCN_CH9,	/* PPG����X�e�[�^�X���W�X�^9 ��~�ݒ�l */
	STOP_PCN_CH10,	/* PPG����X�e�[�^�X���W�X�^10 ��~�ݒ�l */
	STOP_PCN_CH11,	/* PPG����X�e�[�^�X���W�X�^11 ��~�ݒ�l */
	STOP_PCN_CH12,	/* PPG����X�e�[�^�X���W�X�^12 ��~�ݒ�l */
	STOP_PCN_CH13,	/* PPG����X�e�[�^�X���W�X�^13 ��~�ݒ�l */
	STOP_PCN_CH14,	/* PPG����X�e�[�^�X���W�X�^14 ��~�ݒ�l */
	STOP_PCN_CH15,	/* PPG����X�e�[�^�X���W�X�^15 ��~�ݒ�l */
	STOP_PCN_CH16,	/* PPG����X�e�[�^�X���W�X�^16 ��~�ݒ�l */
	STOP_PCN_CH17,	/* PPG����X�e�[�^�X���W�X�^17 ��~�ݒ�l */
	STOP_PCN_CH18,	/* PPG����X�e�[�^�X���W�X�^18 ��~�ݒ�l */
	STOP_PCN_CH19,	/* PPG����X�e�[�^�X���W�X�^19 ��~�ݒ�l */
	STOP_PCN_CH20,	/* PPG����X�e�[�^�X���W�X�^20 ��~�ݒ�l */
	STOP_PCN_CH21,	/* PPG����X�e�[�^�X���W�X�^21 ��~�ݒ�l */
	STOP_PCN_CH22,	/* PPG����X�e�[�^�X���W�X�^22 ��~�ݒ�l */
	STOP_PCN_CH23	/* PPG����X�e�[�^�X���W�X�^23 ��~�ݒ�l */
};

/* PPG����X�e�[�^�X���W�X�^�o�͊J�n�ݒ�e�[�u�� */
static UI_16 const C_PcnSettingActTbl[PPG_CH_NUMBER] = 
{
	PWM_DRV_ACT_PCN_CH0,			/* PPG����X�e�[�^�X���W�X�^0 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH1,			/* PPG����X�e�[�^�X���W�X�^1 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH2,			/* PPG����X�e�[�^�X���W�X�^2 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH3,			/* PPG����X�e�[�^�X���W�X�^3 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH4,			/* PPG����X�e�[�^�X���W�X�^4 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH5,			/* PPG����X�e�[�^�X���W�X�^5 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH6,			/* PPG����X�e�[�^�X���W�X�^6 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH7,			/* PPG����X�e�[�^�X���W�X�^7 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH8,			/* PPG����X�e�[�^�X���W�X�^8 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH9,			/* PPG����X�e�[�^�X���W�X�^9 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH10,			/* PPG����X�e�[�^�X���W�X�^10 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH11,			/* PPG����X�e�[�^�X���W�X�^11 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH12,			/* PPG����X�e�[�^�X���W�X�^12 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH13,			/* PPG����X�e�[�^�X���W�X�^13 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH14,			/* PPG����X�e�[�^�X���W�X�^14 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH15,			/* PPG����X�e�[�^�X���W�X�^15 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH16,			/* PPG����X�e�[�^�X���W�X�^16 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH17,			/* PPG����X�e�[�^�X���W�X�^17 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH18,			/* PPG����X�e�[�^�X���W�X�^18 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH19,			/* PPG����X�e�[�^�X���W�X�^19 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH20,			/* PPG����X�e�[�^�X���W�X�^20 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH21,			/* PPG����X�e�[�^�X���W�X�^21 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH22,			/* PPG����X�e�[�^�X���W�X�^22 �h���C�o�N���ݒ�l */
	PWM_DRV_ACT_PCN_CH23			/* PPG����X�e�[�^�X���W�X�^23 �h���C�o�N���ݒ�l */
};

/* PPG����X�e�[�^�X���W�X�^�o�͒�~�ݒ�e�[�u�� */
static UI_16 const C_PcnSettingStopTbl[PPG_CH_NUMBER] = 
{
	PWM_DRV_STOP_PCN_CH0,			/* PPG����X�e�[�^�X���W�X�^0 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH1,			/* PPG����X�e�[�^�X���W�X�^1 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH2,			/* PPG����X�e�[�^�X���W�X�^2 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH3,			/* PPG����X�e�[�^�X���W�X�^3 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH4,			/* PPG����X�e�[�^�X���W�X�^4 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH5,			/* PPG����X�e�[�^�X���W�X�^5 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH6,			/* PPG����X�e�[�^�X���W�X�^6 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH7,			/* PPG����X�e�[�^�X���W�X�^7 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH8,			/* PPG����X�e�[�^�X���W�X�^8 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH9,			/* PPG����X�e�[�^�X���W�X�^9 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH10,			/* PPG����X�e�[�^�X���W�X�^10 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH11,			/* PPG����X�e�[�^�X���W�X�^11 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH12,			/* PPG����X�e�[�^�X���W�X�^12 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH13,			/* PPG����X�e�[�^�X���W�X�^13 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH14,			/* PPG����X�e�[�^�X���W�X�^14 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH15,			/* PPG����X�e�[�^�X���W�X�^15 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH16,			/* PPG����X�e�[�^�X���W�X�^16 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH17,			/* PPG����X�e�[�^�X���W�X�^17 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH18,			/* PPG����X�e�[�^�X���W�X�^18 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH19,			/* PPG����X�e�[�^�X���W�X�^19 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH20,			/* PPG����X�e�[�^�X���W�X�^20 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH21,			/* PPG����X�e�[�^�X���W�X�^21 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH22,			/* PPG����X�e�[�^�X���W�X�^22 �h���C�o��~�ݒ�l */
	PWM_DRV_STOP_PCN_CH23			/* PPG����X�e�[�^�X���W�X�^23 �h���C�o��~�ݒ�l */
};

/* PPG�����ݒ背�W�X�^�N���ݒ�e�[�u�� */
static UI_16 const	C_PcsrSettingTbl[PPG_CH_NUMBER] = 
{
	INIT_PCSR_CH0,	/* PPG�����ݒ背�W�X�^0 �����ݒ�l */
	INIT_PCSR_CH1,	/* PPG�����ݒ背�W�X�^1 �����ݒ�l */
	INIT_PCSR_CH2,	/* PPG�����ݒ背�W�X�^2 �����ݒ�l */
	INIT_PCSR_CH3,	/* PPG�����ݒ背�W�X�^3 �����ݒ�l */
	INIT_PCSR_CH4,	/* PPG�����ݒ背�W�X�^4 �����ݒ�l */
	INIT_PCSR_CH5,	/* PPG�����ݒ背�W�X�^5 �����ݒ�l */
	INIT_PCSR_CH6,	/* PPG�����ݒ背�W�X�^6 �����ݒ�l */
	INIT_PCSR_CH7,	/* PPG�����ݒ背�W�X�^7 �����ݒ�l */
	INIT_PCSR_CH8,	/* PPG�����ݒ背�W�X�^8 �����ݒ�l */
	INIT_PCSR_CH9,	/* PPG�����ݒ背�W�X�^9 �����ݒ�l */
	INIT_PCSR_CH10,	/* PPG�����ݒ背�W�X�^10 �����ݒ�l */
	INIT_PCSR_CH11, /* PPG�����ݒ背�W�X�^11 �����ݒ�l */
	INIT_PCSR_CH12,	/* PPG�����ݒ背�W�X�^12 �����ݒ�l */
	INIT_PCSR_CH13,	/* PPG�����ݒ背�W�X�^13 �����ݒ�l */
	INIT_PCSR_CH14,	/* PPG�����ݒ背�W�X�^14 �����ݒ�l */
	INIT_PCSR_CH15,	/* PPG�����ݒ背�W�X�^15 �����ݒ�l */
	INIT_PCSR_CH16,	/* PPG�����ݒ背�W�X�^16 �����ݒ�l */
	INIT_PCSR_CH17,	/* PPG�����ݒ背�W�X�^17 �����ݒ�l */
	INIT_PCSR_CH18,	/* PPG�����ݒ背�W�X�^18 �����ݒ�l */
	INIT_PCSR_CH19,	/* PPG�����ݒ背�W�X�^19 �����ݒ�l */
	INIT_PCSR_CH20,	/* PPG�����ݒ背�W�X�^20 �����ݒ�l */
	INIT_PCSR_CH21,	/* PPG�����ݒ背�W�X�^21 �����ݒ�l */
	INIT_PCSR_CH22,	/* PPG�����ݒ背�W�X�^22 �����ݒ�l */
	INIT_PCSR_CH23	/* PPG�����ݒ背�W�X�^23 �����ݒ�l */
};

/* PPG�����ݒ背�W�X�^��~�ݒ�e�[�u�� */
static UI_16 const	C_PcsrRegStopTbl[PPG_CH_NUMBER] = 
{
	STOP_PCSR_CH0,	/* PPG�����ݒ背�W�X�^0 ��~�ݒ�l */
	STOP_PCSR_CH1,	/* PPG�����ݒ背�W�X�^1 ��~�ݒ�l */
	STOP_PCSR_CH2,	/* PPG�����ݒ背�W�X�^2 ��~�ݒ�l */
	STOP_PCSR_CH3,	/* PPG�����ݒ背�W�X�^3 ��~�ݒ�l */
	STOP_PCSR_CH4,	/* PPG�����ݒ背�W�X�^4 ��~�ݒ�l */
	STOP_PCSR_CH5,	/* PPG�����ݒ背�W�X�^5 ��~�ݒ�l */
	STOP_PCSR_CH6,	/* PPG�����ݒ背�W�X�^6 ��~�ݒ�l */
	STOP_PCSR_CH7,	/* PPG�����ݒ背�W�X�^7 ��~�ݒ�l */
	STOP_PCSR_CH8,	/* PPG�����ݒ背�W�X�^8 ��~�ݒ�l */
	STOP_PCSR_CH9,	/* PPG�����ݒ背�W�X�^9 ��~�ݒ�l */
	STOP_PCSR_CH10,	/* PPG�����ݒ背�W�X�^10 ��~�ݒ�l */
	STOP_PCSR_CH11, /* PPG�����ݒ背�W�X�^11 ��~�ݒ�l */
	STOP_PCSR_CH12,	/* PPG�����ݒ背�W�X�^12 ��~�ݒ�l */
	STOP_PCSR_CH13,	/* PPG�����ݒ背�W�X�^13 ��~�ݒ�l */
	STOP_PCSR_CH14,	/* PPG�����ݒ背�W�X�^14 ��~�ݒ�l */
	STOP_PCSR_CH15,	/* PPG�����ݒ背�W�X�^15 ��~�ݒ�l */
	STOP_PCSR_CH16,	/* PPG�����ݒ背�W�X�^16 ��~�ݒ�l */
	STOP_PCSR_CH17,	/* PPG�����ݒ背�W�X�^17 ��~�ݒ�l */
	STOP_PCSR_CH18,	/* PPG�����ݒ背�W�X�^18 ��~�ݒ�l */
	STOP_PCSR_CH19,	/* PPG�����ݒ背�W�X�^19 ��~�ݒ�l */
	STOP_PCSR_CH20,	/* PPG�����ݒ背�W�X�^20 ��~�ݒ�l */
	STOP_PCSR_CH21,	/* PPG�����ݒ背�W�X�^21 ��~�ݒ�l */
	STOP_PCSR_CH22,	/* PPG�����ݒ背�W�X�^22 ��~�ݒ�l */
	STOP_PCSR_CH23	/* PPG�����ݒ背�W�X�^23 ��~�ݒ�l */
};

/* PPG�f���[�e�B�ݒ背�W�X�^�N���ݒ�e�[�u�� */
static UI_16 const	C_PdutSettingTbl[PPG_CH_NUMBER] = 
{
	INIT_PDUT_CH0,	/* PPG�f���[�e�B�ݒ背�W�X�^0 �����ݒ�l */
	INIT_PDUT_CH1,	/* PPG�f���[�e�B�ݒ背�W�X�^1 �����ݒ�l */
	INIT_PDUT_CH2,	/* PPG�f���[�e�B�ݒ背�W�X�^2 �����ݒ�l */
	INIT_PDUT_CH3,	/* PPG�f���[�e�B�ݒ背�W�X�^3 �����ݒ�l */
	INIT_PDUT_CH4,	/* PPG�f���[�e�B�ݒ背�W�X�^4 �����ݒ�l */
	INIT_PDUT_CH5,	/* PPG�f���[�e�B�ݒ背�W�X�^5 �����ݒ�l */
	INIT_PDUT_CH6,	/* PPG�f���[�e�B�ݒ背�W�X�^6 �����ݒ�l */
	INIT_PDUT_CH7,	/* PPG�f���[�e�B�ݒ背�W�X�^7 �����ݒ�l */
	INIT_PDUT_CH8,	/* PPG�f���[�e�B�ݒ背�W�X�^8 �����ݒ�l */
	INIT_PDUT_CH9,	/* PPG�f���[�e�B�ݒ背�W�X�^9 �����ݒ�l */
	INIT_PDUT_CH10,	/* PPG�f���[�e�B�ݒ背�W�X�^10 �����ݒ�l */
	INIT_PDUT_CH11,	/* PPG�f���[�e�B�ݒ背�W�X�^11 �����ݒ�l */
	INIT_PDUT_CH12,	/* PPG�f���[�e�B�ݒ背�W�X�^12 �����ݒ�l */
	INIT_PDUT_CH13,	/* PPG�f���[�e�B�ݒ背�W�X�^13 �����ݒ�l */
	INIT_PDUT_CH14,	/* PPG�f���[�e�B�ݒ背�W�X�^14 �����ݒ�l */
	INIT_PDUT_CH15,	/* PPG�f���[�e�B�ݒ背�W�X�^15 �����ݒ�l */
	INIT_PDUT_CH16,	/* PPG�f���[�e�B�ݒ背�W�X�^16 �����ݒ�l */
	INIT_PDUT_CH17,	/* PPG�f���[�e�B�ݒ背�W�X�^17 �����ݒ�l */
	INIT_PDUT_CH18,	/* PPG�f���[�e�B�ݒ背�W�X�^18 �����ݒ�l */
	INIT_PDUT_CH19,	/* PPG�f���[�e�B�ݒ背�W�X�^19 �����ݒ�l */
	INIT_PDUT_CH20,	/* PPG�f���[�e�B�ݒ背�W�X�^20 �����ݒ�l */
	INIT_PDUT_CH21,	/* PPG�f���[�e�B�ݒ背�W�X�^21 �����ݒ�l */
	INIT_PDUT_CH22,	/* PPG�f���[�e�B�ݒ背�W�X�^22 �����ݒ�l */
	INIT_PDUT_CH23	/* PPG�f���[�e�B�ݒ背�W�X�^23 �����ݒ�l */
};

/* PPG�f���[�e�B�ݒ背�W�X�^��~�ݒ�e�[�u�� */
static UI_16 const	C_PdutRegStopTbl[PPG_CH_NUMBER] = 
{
	STOP_PDUT_CH0,	/* PPG�f���[�e�B�ݒ背�W�X�^0 ��~�ݒ�l */
	STOP_PDUT_CH1,	/* PPG�f���[�e�B�ݒ背�W�X�^1 ��~�ݒ�l */
	STOP_PDUT_CH2,	/* PPG�f���[�e�B�ݒ背�W�X�^2 ��~�ݒ�l */
	STOP_PDUT_CH3,	/* PPG�f���[�e�B�ݒ背�W�X�^3 ��~�ݒ�l */
	STOP_PDUT_CH4,	/* PPG�f���[�e�B�ݒ背�W�X�^4 ��~�ݒ�l */
	STOP_PDUT_CH5,	/* PPG�f���[�e�B�ݒ背�W�X�^5 ��~�ݒ�l */
	STOP_PDUT_CH6,	/* PPG�f���[�e�B�ݒ背�W�X�^6 ��~�ݒ�l */
	STOP_PDUT_CH7,	/* PPG�f���[�e�B�ݒ背�W�X�^7 ��~�ݒ�l */
	STOP_PDUT_CH8,	/* PPG�f���[�e�B�ݒ背�W�X�^8 ��~�ݒ�l */
	STOP_PDUT_CH9,	/* PPG�f���[�e�B�ݒ背�W�X�^9 ��~�ݒ�l */
	STOP_PDUT_CH10,	/* PPG�f���[�e�B�ݒ背�W�X�^10 ��~�ݒ�l */
	STOP_PDUT_CH11,	/* PPG�f���[�e�B�ݒ背�W�X�^11 ��~�ݒ�l */
	STOP_PDUT_CH12,	/* PPG�f���[�e�B�ݒ背�W�X�^12 ��~�ݒ�l */
	STOP_PDUT_CH13,	/* PPG�f���[�e�B�ݒ背�W�X�^13 ��~�ݒ�l */
	STOP_PDUT_CH14,	/* PPG�f���[�e�B�ݒ背�W�X�^14 ��~�ݒ�l */
	STOP_PDUT_CH15,	/* PPG�f���[�e�B�ݒ背�W�X�^15 ��~�ݒ�l */
	STOP_PDUT_CH16,	/* PPG�f���[�e�B�ݒ背�W�X�^16 ��~�ݒ�l */
	STOP_PDUT_CH17,	/* PPG�f���[�e�B�ݒ背�W�X�^17 ��~�ݒ�l */
	STOP_PDUT_CH18,	/* PPG�f���[�e�B�ݒ背�W�X�^18 ��~�ݒ�l */
	STOP_PDUT_CH19,	/* PPG�f���[�e�B�ݒ背�W�X�^19 ��~�ݒ�l */
	STOP_PDUT_CH20,	/* PPG�f���[�e�B�ݒ背�W�X�^20 ��~�ݒ�l */
	STOP_PDUT_CH21,	/* PPG�f���[�e�B�ݒ背�W�X�^21 ��~�ݒ�l */
	STOP_PDUT_CH22,	/* PPG�f���[�e�B�ݒ背�W�X�^22 ��~�ݒ�l */
	STOP_PDUT_CH23	/* PPG�f���[�e�B�ݒ背�W�X�^23 ��~�ݒ�l */
};

/* PPG����X�e�[�^�X���W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_16 *const C_PcnReg[PPG_CH_NUMBER] = 
{
	&(PCN0),	/* PPG����X�e�[�^�X���W�X�^0 */
	&(PCN1),	/* PPG����X�e�[�^�X���W�X�^1 */
	&(PCN2),	/* PPG����X�e�[�^�X���W�X�^2 */
	&(PCN3),	/* PPG����X�e�[�^�X���W�X�^3 */
	&(PCN4),	/* PPG����X�e�[�^�X���W�X�^4 */
	&(PCN5),	/* PPG����X�e�[�^�X���W�X�^5 */
	&(PCN6),	/* PPG����X�e�[�^�X���W�X�^6 */
	&(PCN7),	/* PPG����X�e�[�^�X���W�X�^7 */
	&(PCN8),	/* PPG����X�e�[�^�X���W�X�^8 */
	&(PCN9),	/* PPG����X�e�[�^�X���W�X�^9 */
	&(PCN10),	/* PPG����X�e�[�^�X���W�X�^10 */
	&(PCN11),	/* PPG����X�e�[�^�X���W�X�^11 */
	&(PCN12),	/* PPG����X�e�[�^�X���W�X�^12 */
	&(PCN13),	/* PPG����X�e�[�^�X���W�X�^13 */
	&(PCN14),	/* PPG����X�e�[�^�X���W�X�^14 */
	&(PCN15),	/* PPG����X�e�[�^�X���W�X�^15 */
	&(PCN16),	/* PPG����X�e�[�^�X���W�X�^16 */
	&(PCN17),	/* PPG����X�e�[�^�X���W�X�^17 */
	&(PCN18),	/* PPG����X�e�[�^�X���W�X�^18 */
	&(PCN19),	/* PPG����X�e�[�^�X���W�X�^19 */
	&(PCN20),	/* PPG����X�e�[�^�X���W�X�^20 */
	&(PCN21),	/* PPG����X�e�[�^�X���W�X�^21 */
	&(PCN22),	/* PPG����X�e�[�^�X���W�X�^22 */
	&(PCN23)	/* PPG����X�e�[�^�X���W�X�^23 */
};

/* PPG�����ݒ背�W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_16 *const C_PcsrReg[PPG_CH_NUMBER] = 
{
	&(PCSR0_),	/* PPG�����ݒ背�W�X�^0 */
	&(PCSR1_),	/* PPG�����ݒ背�W�X�^1 */
	&(PCSR2),	/* PPG�����ݒ背�W�X�^2 */
	&(PCSR3),	/* PPG�����ݒ背�W�X�^3 */
	&(PCSR4),	/* PPG�����ݒ背�W�X�^4 */
	&(PCSR5),	/* PPG�����ݒ背�W�X�^5 */
	&(PCSR6),	/* PPG�����ݒ背�W�X�^6 */
	&(PCSR7),	/* PPG�����ݒ背�W�X�^7 */
	&(PCSR8),	/* PPG�����ݒ背�W�X�^8 */
	&(PCSR9),	/* PPG�����ݒ背�W�X�^9 */
	&(PCSR10),	/* PPG�����ݒ背�W�X�^10 */
	&(PCSR11),	/* PPG�����ݒ背�W�X�^11 */
	&(PCSR12),	/* PPG�����ݒ背�W�X�^12 */
	&(PCSR13),	/* PPG�����ݒ背�W�X�^13 */
	&(PCSR14),	/* PPG�����ݒ背�W�X�^14 */
	&(PCSR15),	/* PPG�����ݒ背�W�X�^15 */
	&(PCSR16),	/* PPG�����ݒ背�W�X�^16 */
	&(PCSR17),	/* PPG�����ݒ背�W�X�^17 */
	&(PCSR18),	/* PPG�����ݒ背�W�X�^18 */
	&(PCSR19),	/* PPG�����ݒ背�W�X�^19 */
	&(PCSR20),	/* PPG�����ݒ背�W�X�^20 */
	&(PCSR21),	/* PPG�����ݒ背�W�X�^21 */
	&(PCSR22),	/* PPG�����ݒ背�W�X�^22 */
	&(PCSR23)	/* PPG�����ݒ背�W�X�^23 */
};

/* PPG�f���[�e�B�ݒ背�W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_16 *const C_PdutReg[PPG_CH_NUMBER] = 
{
	&(PDUT0_),	/* PPG�f���[�e�B�ݒ背�W�X�^0 */
	&(PDUT1_),	/* PPG�f���[�e�B�ݒ背�W�X�^1 */
	&(PDUT2),	/* PPG�f���[�e�B�ݒ背�W�X�^2 */
	&(PDUT3),	/* PPG�f���[�e�B�ݒ背�W�X�^3 */
	&(PDUT4),	/* PPG�f���[�e�B�ݒ背�W�X�^4 */
	&(PDUT5),	/* PPG�f���[�e�B�ݒ背�W�X�^5 */
	&(PDUT6),	/* PPG�f���[�e�B�ݒ背�W�X�^6 */
	&(PDUT7),	/* PPG�f���[�e�B�ݒ背�W�X�^7 */
	&(PDUT8),	/* PPG�f���[�e�B�ݒ背�W�X�^8 */
	&(PDUT9),	/* PPG�f���[�e�B�ݒ背�W�X�^9 */
	&(PDUT10),	/* PPG�f���[�e�B�ݒ背�W�X�^10 */
	&(PDUT11),	/* PPG�f���[�e�B�ݒ背�W�X�^11 */
	&(PDUT12),	/* PPG�f���[�e�B�ݒ背�W�X�^12 */
	&(PDUT13),	/* PPG�f���[�e�B�ݒ背�W�X�^13 */
	&(PDUT14),	/* PPG�f���[�e�B�ݒ背�W�X�^14 */
	&(PDUT15),	/* PPG�f���[�e�B�ݒ背�W�X�^15 */
	&(PDUT16),	/* PPG�f���[�e�B�ݒ背�W�X�^16 */
	&(PDUT17),	/* PPG�f���[�e�B�ݒ背�W�X�^17 */
	&(PDUT18),	/* PPG�f���[�e�B�ݒ背�W�X�^18 */
	&(PDUT19),	/* PPG�f���[�e�B�ݒ背�W�X�^19 */
	&(PDUT20),	/* PPG�f���[�e�B�ݒ背�W�X�^20 */
	&(PDUT21),	/* PPG�f���[�e�B�ݒ背�W�X�^21 */
	&(PDUT22),	/* PPG�f���[�e�B�ݒ背�W�X�^22 */
	&(PDUT23)	/* PPG�f���[�e�B�ݒ背�W�X�^23 */
};


/****************************************************************************************/
/*	[���W���[����]	PWMDrv_SetDutyCycle		[����]	����/DUTY�ݒ菈��					*/
/*======================================================================================*/
/*	[�����T�v]	�����Ŏw�肳�ꂽ�`���l���ɑ΂��A�w�肳�ꂽ�����ADUTY��PPG�o�͂��s���B	*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 PWMDrv_SetDutyCycle(E_PWM_DRV_PPG_CH ch,UI_16 cycle,UI_16 duty)	*/
/*																						*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_PWM_DRV_PPG_CH		ch		�FPPG�`���l��							*/
/*				UI_16					cycle	�F�����ݒ�l							*/
/*				UI_16					duty	�FDuty�ݒ�l							*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8		rc_out	: ����(D_OK)�A�ُ�(D_NG)��Ԃ�						*/
/*======================================================================================*/
/*	[ ��  �l ]																			*/
/*	�E������1�ȉ��A�܂��͎�������Duty�̒l���傫���ꍇ�ُ͈�Ȑݒ�Ɣ��f���āA			*/
/*    ��~�E����������																	*/
/*  �E100���o�͉\																		*/
/*	�E0��(Duty=0)���w�肵���ꍇ�A�o�̓}�X�N��1������0���o�͐ݒ���s���A				*/
/*    ��~�E����������																	*/
/*	�E�`���l���ԈႢ(���݂��Ȃ��`���l����I��)�͏������s�킸�I������					*/
/****************************************************************************************/
SI_8 PWMDrv_SetDutyCycle( E_PWM_DRV_PPG_CH ch, UI_16 cycle, UI_16 duty )
{
	SI_8  rc_out;           /* �߂�l */
	UI_16 pcn_val;          /* PPG����X�e�[�^�X���W�X�^�ɐݒ肷��l */
	UI_16 pcn_crr;         	/* PPG����X�e�[�^�X���W�X�^�̌��݂̐ݒ�l(���W�X�^����l��ݒ�) */

	pcn_val = PCN_SET_VALUE;			
	
	/* �`���l������ */	
	if ( ch < (UI_8)PPG_CH_NUMBER ) {
		
		/* �����ADuty������͈͓��ł��� */
		if ( ( cycle > CYCLE_JUDGMENT ) && ( cycle >= duty ) ) {
			pcn_val=C_PcnSettingTbl[ch];
			
			/* Duty=0%�̏ꍇ */
			if ( duty == DUTY_JUDGMENT ) {
				/* PPG����X�e�[�^�X���W�X�^ �Œ�o�́i�o�̓}�X�N�j���̋N���ݒ�  */
				pcn_val |= PCN_RUN_WITH_MASK;
			} else {
				/* PPG����X�e�[�^�X���W�X�^ PWM�i�o�̓}�X�N�����j���̋N���ݒ� */
				pcn_val |= PCN_RUN_WITH_PWM;
			}
			
		}
		
		/* ���݂̃��W�X�^�ݒ�l���i�[�ݒ�l */
		pcn_crr = *C_PcnReg[ch];
		/* �L���r�b�g�}�X�N�ݒ�(�^�C�}���싖�A�\�t�g�E�F�A�g���K�A�J�E���^�N���b�N�I���APPG �o�̓}�X�N�I���A�����ݗv���I���APPG �o�͋ɐ�) */
		pcn_crr &= PCN_USED_BIT_MASK;
		/* PPG����X�e�[�^�X���W�X�^ �Œ�o�́i�o�̓}�X�N�j���̋N���ݒ� */
		pcn_crr |= PCN_RUN_WITH_PWM;
			
		if ( ( pcn_val != pcn_crr ) || ( pcn_val == PCN_SET_VALUE ) ) {
			/* PPG����X�e�[�^�X���W�X�^��~�ݒ� */
			*C_PcnReg[ch] = C_PcnCtrlStopRegTbl[ch];
			/* PPG�����ݒ背�W�X�^��~�ݒ� */
			*C_PcsrReg[ch] = C_PcsrRegStopTbl[ch];
			/* PPG�f���[�e�B�ݒ背�W�X�^ */
			*C_PdutReg[ch] = C_PdutRegStopTbl[ch];
		}
		
		/* Output enabled? */
		if ( pcn_val != PCN_SET_VALUE ) {
			*C_PcsrReg[ch] = ( cycle - PWM_SUBTRACTION );
			
			if ( duty > DUTY_JUDGMENT ) {
				*C_PdutReg[ch] = ( duty - PWM_SUBTRACTION );
			} else {
				*C_PdutReg[ch] = DUTY_CLEA;
			}
			
			*C_PcnReg[ch] = pcn_val;
			rc_out = D_OK;
			
		} else {
			rc_out = D_NG;
		}
		
		
	} else {
		rc_out = D_NG;
	}
	return rc_out;
}

/****************************************************************************************/
/*	[���W���[����]	PWMDrv_SetDutyCycleSync	[����]	����/DUTY�ݒ菈��(����)				*/
/*======================================================================================*/
/*	[�����T�v]	�����Ŏw�肳�ꂽ2�̃`���l���ɑ΂��A�w�肳�ꂽ�����A					*/
/*              DUTY��PPG�o�͂𓯊������čs���B											*/
/*======================================================================================*/
/*	[�L�q�`��]	UI_8 PWMDrv_SetDutyCycleSync(UI_8 ch1,UI_16 cycle,						*/
/*	                                         UI_16 duty1,UI_8 ch2,UI_16 duty2)			*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_PWM_DRV_PPG_CH	ch1		�FPPG�`���l��								*/
/*				UI_16				cycle	�F�����ݒ�l								*/
/*				UI_16				duty1	�FDuty�ݒ�l								*/
/*				E_PWM_DRV_PPG_CH	ch2	    �FPPG�`���l��								*/
/*				UI_16				duty2	�FDuty�ݒ�									*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8		rc_out	: ����(D_OK)�A�ُ�(D_NG)��Ԃ�						*/
/*======================================================================================*/
/*	[ ��  �l ]																			*/
/*																						*/
/****************************************************************************************/
SI_8 PWMDrv_SetDutyCycleSync( E_PWM_DRV_PPG_CH ch1, UI_16 cycle, UI_16 duty1, E_PWM_DRV_PPG_CH ch2, UI_16 duty2)
{
	SI_8  rc_out;
	UI_16 mask_level;
	UI_16 pcn_val1;
	UI_16 pcn_val2;
	
	/* �`���l���w�肪���� */
	if ( (ch1 < PPG_CH_NUMBER) && (ch2 < PPG_CH_NUMBER) && (ch1 != ch2) ) {
		rc_out = D_OK;
		if ((cycle > 1) && (cycle >= duty1) && (cycle >= duty2) && (duty1 > 0) && (duty2 > 0)) {
			/* PPG����X�e�[�^�X���W�X�^:�J�E���g�J�n/�g���KON��ݒ� */
			pcn_val1 = C_PcnSettingActTbl[ch1] | PCN_RUN_WITH_PWM;
			pcn_val2 = C_PcnSettingActTbl[ch2] | PCN_RUN_WITH_PWM;
			
			/* �ʑ������ŏ��ɂ��邽�߂ɁA�����݋֎~�ݒ���s�� */
			(void)IntrDrv_GetMaskLevel( &mask_level );				/* ���荞�݃}�X�N���x���̎擾 */
			(void)IntrDrv_SetMaskLevel( INTR_DISABLE_LEVEL );		/* ���荞�݃}�X�N���x���̐ݒ� */
			
			/* ch1 ����/�f���[�e�B�̐ݒ� */
			*C_PcsrReg[ch1] = ( cycle - PWM_SUBTRACTION );
			*C_PdutReg[ch1] = ( duty1 - PWM_SUBTRACTION );
			
			/* ch2 ����/�f���[�e�B�̐ݒ� */
			*C_PcsrReg[ch2] = ( cycle - PWM_SUBTRACTION );
			*C_PdutReg[ch2] = ( duty2 - PWM_SUBTRACTION );
			
			/* PPG����X�e�[�^�X���W�X�^��ݒ� */
			*C_PcnReg[ch1] = pcn_val1;
			*C_PcnReg[ch2] = pcn_val2;
			
			(void)IntrDrv_SetMaskLevel( mask_level );				/* ���荞�݃}�X�N���x���̐ݒ� */
			
		} else {
			
			*C_PcnReg[ch1] = C_PcnSettingStopTbl[ch1];
			*C_PcnReg[ch2] = C_PcnSettingStopTbl[ch2];
			
			/* ch1 ����/�f���[�e�B�̐ݒ� */
			*C_PcsrReg[ch1] = 0;
			*C_PdutReg[ch1] = 0;
			
			/* ch2 ����/�f���[�e�B�̐ݒ� */
			*C_PcsrReg[ch2] = 0;
			*C_PdutReg[ch2] = 0;
			
		}
	} else {
		rc_out = D_NG;
	}
	
	
	return rc_out;
}

/****************************************************************************************/
/*	[���W���[����]	PWMDrv_ReqIntrClr	[����]	PWM�h���C�o�̊��荞�ݗv���N���A����		*/
/*======================================================================================*/
/*	[�����T�v]	�����Ŏw�肳�ꂽ�`���l���̊����ݗv�����N���A����						*/
/*              																		*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 PWMDrv_ReqIntrClr(E_PWM_DRV_PPG_CH ch)								*/
/*	                                     												*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_PWM_DRV_PPG_CH		ch		�FPPG�`���l��						    */
/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8		rc_out	: ����(D_OK)�A�ُ�(D_NG)��Ԃ�						*/
/*======================================================================================*/
/*	[ ��  �l ]																			*/
/*		�`���l���ԈႢ(���݂��Ȃ��`���l����I��)�͏������s�킸�I������					*/
/****************************************************************************************/
SI_8 PWMDrv_ReqIntrClr( E_PWM_DRV_PPG_CH ch )
{
	SI_8  rc_out;   /* �߂�l */
	UI_16 pcn_work; /* PPG����X�e�[�^�X���W�X�^���Z�pwork�ϐ� */
	
	/* �`���l������̏ꍇ */
	if ( ch < (UI_8)PPG_CH_NUMBER ) {
		pcn_work = *C_PcnReg[ch];
		pcn_work &= PCN_IRQF_CLR_MASK;
		*C_PcnReg[ch] = pcn_work;
		rc_out = D_OK;
	} else {
	/* �`���l���ُ�̏ꍇ */
		rc_out = D_NG;
	}
	
	return rc_out;
}

/****************************************************************************************/
/*	[���W���[����]	PWMDrv_ReqIntrDisable	[����]	PWM�h���C�o�̊��荞�݋֎~����		*/
/*======================================================================================*/
/*	[�����T�v]	�����Ŏw�肳�ꂽ�`���l���̊��荞�ݗv���N���A�A�����݋֎~���s��			*/
/*              																		*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 PWMDrv_ReqIntrDisable(E_PWM_DRV_PPG_CH  ch)						*/
/*	                                     												*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_PWM_DRV_PPG_CH 		ch		�FPPG�`���l��						    */
/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8		rc_out	: ����(D_OK)�A�ُ�(D_NG)��Ԃ�						*/
/*======================================================================================*/
/*	[ ��  �l ]																			*/
/*		�`���l���ԈႢ(���݂��Ȃ��`���l����I��)�͏������s�킸�I������					*/
/****************************************************************************************/
SI_8 PWMDrv_ReqIntrDisable( E_PWM_DRV_PPG_CH  ch )
{
	SI_8  rc_out;   /* �߂�l */
	UI_16 pcn_work; /* PPG����X�e�[�^�X���W�X�^���Z�pwork�ϐ� */
	
	/* �`���l������̏ꍇ */
	if ( ch < ( UI_8 )PPG_CH_NUMBER ) {
		pcn_work = *C_PcnReg[ch];
		pcn_work &= PCN_IREN_IRQF_CLR_MASK;
		*C_PcnReg[ch] = pcn_work;
		rc_out = D_OK;
	} else {
	/* �`���l���ُ�̏ꍇ */
		rc_out = D_NG;
	}
	
	return rc_out;
}

/****************************************************************************************/
/*	[���W���[����]	PWMDrv_ReqIntrEnable	[����]	PWM�h���C�o�̊��荞�݋�����		*/
/*======================================================================================*/
/*	[�����T�v]	�����Ŏw�肳�ꂽ�`���l���̊��荞�ݗv���̊����݋����s��				*/
/*              																		*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 PWMDrv_ReqIntrEnable(E_PWM_DRV_PPG_CH  ch)							*/
/*	                                     												*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_PWM_DRV_PPG_CH 		ch		�FPPG�`���l��						    */
/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8		rc_out	: ����(D_OK)�A�ُ�(D_NG)��Ԃ�						*/
/*======================================================================================*/
/*	[ ��  �l ]																			*/
/*		�`���l���ԈႢ(���݂��Ȃ��`���l����I��)�͏������s�킸�I������					*/
/****************************************************************************************/
SI_8 PWMDrv_ReqIntrEnable( E_PWM_DRV_PPG_CH  ch )
{
	SI_8  rc_out;   /* �߂�l */
	UI_16 pcn_work; /* PPG����X�e�[�^�X���W�X�^���Z�pwork�ϐ� */
	
	/* �`���l������̏ꍇ */
	if ( ch < ( UI_8 )PPG_CH_NUMBER ) {
		pcn_work = *C_PcnReg[ch];
		pcn_work |= PCN_IREN_SET;
		*C_PcnReg[ch] = pcn_work;
		rc_out = D_OK;
	} else {
	/* �`���l���ُ�̏ꍇ */
		rc_out = D_NG;
	}
	
	return rc_out;
}


/****************************************************************************************/
/*	[���W���[����]	PWM_Drv_Init		[����]PWM�h���C�o���W�X�^�����ݒ菈��			*/
/*======================================================================================*/
/*	[�����T�v]	���W�X�^�����ݒ莞�ɖ{�֐����ĂсA���������s���B						*/
/*======================================================================================*/
/*	[�L�q�`��]	void PWM_Drv_Init( E_INIT_TYPE req  )									*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_INIT_TYPE				req		�F�N�����								*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void PWMDrv_Init( E_INIT_TYPE req )
{
	UI_8	ch;	/* PPG�`���l���ԍ� */
	
	switch ( req ) {
	case E_INIT_RESET:			/* CPU���Z�b�g�ɂ��N�� */
	case E_INIT_WAKEUP:			/* �ȓd�͉����iWAKEUP�j�ɂ��N�� */
		
		/* PPG�����ݒ背�W�X�^,PPG�f���[�e�B�ݒ背�W�X�^,PPG����X�e�[�^�X���W�X�^ �����ݒ� */
		for ( ch = 0; ch < PPG_CH_NUMBER; ch++ ) {
			*C_PcsrReg[ch] = C_PcsrSettingTbl[ch];
			*C_PdutReg[ch] = C_PdutSettingTbl[ch];
			*C_PcnReg[ch] = C_PcnSettingTbl[ch];
		}
		
		PPGDIV = INIT_PPGDIV;	/* PPG0�o�͕����ݒ背�W�X�^ �����ݒ� */
		
		GCN10 = INIT_GCN10;	/* �W�F�l�������䃌�W�X�^10 �����ݒ� */
		GCN11 = INIT_GCN11;	/* �W�F�l�������䃌�W�X�^11 �����ݒ� */
		GCN12 = INIT_GCN12;	/* �W�F�l�������䃌�W�X�^12 �����ݒ� */
		GCN13 = INIT_GCN13;	/* �W�F�l�������䃌�W�X�^13 �����ݒ� */
		GCN14 = INIT_GCN14;	/* �W�F�l�������䃌�W�X�^14 �����ݒ� */
		GCN15 = INIT_GCN15;	/* �W�F�l�������䃌�W�X�^15 �����ݒ� */
		GCN20 = INIT_GCN20;	/* �W�F�l�������䃌�W�X�^20 �����ݒ� */
		GCN21 = INIT_GCN21;	/* �W�F�l�������䃌�W�X�^21 �����ݒ� */
		GCN22 = INIT_GCN22;	/* �W�F�l�������䃌�W�X�^22 �����ݒ� */
		GCN23 = INIT_GCN23;	/* �W�F�l�������䃌�W�X�^23 �����ݒ� */
		GCN24 = INIT_GCN24;	/* �W�F�l�������䃌�W�X�^24 �����ݒ� */
		GCN25 = INIT_GCN25;	/* �W�F�l�������䃌�W�X�^25 �����ݒ� */
		
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
/*	[���W���[����]	PWMDrv_Sleep	[����]	PWM�h���C�o���W�X�^��~�ݒ菈��				*/
/*======================================================================================*/
/*	[�����T�v]	�ȓd�̓��[�h�J�ڎ��ɖ{�֐����ĂсA���������s���B						*/
/*======================================================================================*/
/*	[�L�q�`��]	void PWMDrv_Sleep( void )												*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void PWMDrv_Sleep( void )
{
	UI_8	ch;	/* PPG�`���l���ԍ� */
	
	/* PPG����X�e�[�^�X���W�X�^,PPG�����ݒ背�W�X�^,PPG�f���[�e�B�ݒ背�W�X�^ ��~�ݒ� */
	for ( ch = 0; ch < PPG_CH_NUMBER; ch++ ) {
		*C_PcnReg[ch] = C_PcnCtrlStopRegTbl[ch];
		*C_PcsrReg[ch] = C_PcsrRegStopTbl[ch];
		*C_PdutReg[ch] = C_PdutRegStopTbl[ch];
	}
	
	PPGDIV = STOP_PPGDIV;	/* PPG0�o�͕����ݒ背�W�X�^ ��~�ݒ� */
	
	GCN10 = STOP_GCN10;	/* �W�F�l�������䃌�W�X�^10 ��~�ݒ� */
	GCN11 = STOP_GCN11;	/* �W�F�l�������䃌�W�X�^11 ��~�ݒ� */
	GCN12 = STOP_GCN12;	/* �W�F�l�������䃌�W�X�^12 ��~�ݒ� */
	GCN13 = STOP_GCN13;	/* �W�F�l�������䃌�W�X�^13 ��~�ݒ� */
	GCN14 = STOP_GCN14;	/* �W�F�l�������䃌�W�X�^14 ��~�ݒ� */
	GCN15 = STOP_GCN15;	/* �W�F�l�������䃌�W�X�^15 ��~�ݒ� */
	GCN20 = STOP_GCN20;	/* �W�F�l�������䃌�W�X�^20 ��~�ݒ� */
	GCN21 = STOP_GCN21;	/* �W�F�l�������䃌�W�X�^21 ��~�ݒ� */
	GCN22 = STOP_GCN22;	/* �W�F�l�������䃌�W�X�^22 ��~�ݒ� */
	GCN23 = STOP_GCN23;	/* �W�F�l�������䃌�W�X�^23 ��~�ݒ� */
	GCN24 = STOP_GCN24;	/* �W�F�l�������䃌�W�X�^24 ��~�ݒ� */
	GCN25 = STOP_GCN25;	/* �W�F�l�������䃌�W�X�^25 ��~�ݒ� */
}

/****************************************************************************************/
/*	[���W���[����]	PWMDrv_Refresh	[����]	PWM�h���C�o���W�X�^�Đݒ菈��				*/
/*======================================================================================*/
/*	[�����T�v]	���W�X�^�Đݒ莞�ɖ{�֐����ĂсA���������s���B							*/
/*======================================================================================*/
/*	[�L�q�`��]	void PWMDrv_Refresh( void )												*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void PWMDrv_Refresh( void )
{
	/* ��֐� */
}

