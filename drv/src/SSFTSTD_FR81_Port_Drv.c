/************************************************************************************************/
/* �q�於		:	�W�����W���[��																*/
/* �@�햼		:	SSFT																		*/
/* ϲ�ݿ��ðϖ�	:	PF																			*/
/*==============================================================================================*/
/* �쐬̧�ٖ�	:	SSFTSTD_FR81_Port_Drv.c														*/
/* 				:	GPIO��ײ��Ӽޭ��															*/
/*==============================================================================================*/
/* �Ώ�ϲ��		:	MB91570 Series																*/
/*==============================================================================================*/
/* �쐬�ް�ޮ�	:	030103																		*/
/* �쐬�N����	:	2014.01.23																	*/
/* �쐬��		:	K.Uchiyama																	*/
/*----------------------------------------------------------------------------------------------*/
/* �ύX����		:																				*/
/* [010101]		:	�V�K�쐬																	*/
/* [010102]		:	P127�@�\�؂�ւ��e�[�u���C��												*/
/*				:	�|�[�g�@�\�؂�ւ����̃|�[�g�Ɉ�u�߂�����C��								*/
/* [010103]		:	�W��IF�Ή��BIF�֐��̖߂�l��SI_8���B����E_PORTDRV_BYTE_NUM�ǉ��i�\��݌v�j	*/
/* [010104]		:	�r�b�g�}�X�N�ʒu�̒�`�ӏ��ύX�B�Z�L�����e�B�C���W�P�[�^�̃|�[�g�C���B		*/
/* [010105]		:	EEPROM�A�V�t�g���W�X�^�̃|�[�g��`�ǉ�										*/
/* [030101]		:	ROM�e�ʂƏ������ԍ팸�̂��߁A�R���p�C��SW�ǉ��A���g�p�̃e�[�u���폜			*/
/* [030102]		:	�C���N���[�h�\���������B													*/
/* [030103]		:	�K���Ή��B																	*/
/************************************************************************************************/

/*** START_INC ***/
/********************************************************************************/
/*   Include File                                                               */
/*------------------------------------------------------------------------------*/
/*      ͯ�ް̧�ق̲ݸٰ�ޕ��́A���Ļ�قɋL�ڂ��邱��                          */
/********************************************************************************/
#include "SSFTSTD_Type.h"
#include "SSFTSTD_Macro.h"
#include "mb91570.h"

#define	 __MB91570_PORT_DRV_INTERNAL__
#include "SSFTSTD_FR81_Port_Drv.h"

/*==============================================================================*/
/*	�����ÓI�ϐ�																*/
/*==============================================================================*/
static SI_8 S_SelfcheckState;			/* �o�͕������W�X�^���t���b�V�������̃Z���t�`�F�b�N��� */
static SI_8 S_ChkSetReg;				/* �D��x�̍��������ݓ��ł̃��W�X�^�ݒ�̋֎~/���� */


/*==============================================================================*/
/*	typedef��`�i�O������J�j													*/
/*==============================================================================*/
/* �Ȃ� */

/*==============================================================================*/
/*	�֐��������ߐ錾�i�O������J)												*/
/*==============================================================================*/
/* �Ȃ� */

/*==============================================================================*/
/*	�����萔																	*/
/*==============================================================================*/

/*�ydefine��`�z*/

#define	PORT_FUNC_CHANGE_NUM0	(14)	/* P0�̋@�\���W�X�^�� */
#define	PORT_FUNC_CHANGE_NUM1	(5)		/* P1�̋@�\���W�X�^�� */
#define	PORT_FUNC_CHANGE_NUM2	(8)		/* P2�̋@�\���W�X�^�� */
#define	PORT_FUNC_CHANGE_NUM3	(9)		/* P3�̋@�\���W�X�^�� */
#define	PORT_FUNC_CHANGE_NUM4	(10)	/* P4�̋@�\���W�X�^�� */
#define	PORT_FUNC_CHANGE_NUM5	(5)		/* P5�̋@�\���W�X�^�� */
#define	PORT_FUNC_CHANGE_NUM6	(8)		/* P6�̋@�\���W�X�^�� */
#define	PORT_FUNC_CHANGE_NUM7	(10)	/* P7�̋@�\���W�X�^�� */
#define	PORT_FUNC_CHANGE_NUM8	(9)		/* P8�̋@�\���W�X�^�� */
#define	PORT_FUNC_CHANGE_NUM9	(16)	/* P9�̋@�\���W�X�^�� */
#define	PORT_FUNC_CHANGE_NUM10	(17)	/* P10�̋@�\���W�X�^�� */
#define	PORT_FUNC_CHANGE_NUM11	(16)	/* P11�̋@�\���W�X�^�� */
#define	PORT_FUNC_CHANGE_NUM12	(18)	/* P12�̋@�\���W�X�^�� */
#define	PORT_FUNC_CHANGE_NUM13	(10)	/* P13�̋@�\���W�X�^�� */

#define	PORT_FUNC_CHANGE0	(PORT_FUNC_CHANGE_NUM0 + PORT_FUNC)		/* P0�̋@�\���W�X�^�ݒ萔 */
#define	PORT_FUNC_CHANGE1	(PORT_FUNC_CHANGE_NUM1 + PORT_FUNC)		/* P1�̋@�\���W�X�^�ݒ萔 */
#define	PORT_FUNC_CHANGE2	(PORT_FUNC_CHANGE_NUM2 + PORT_FUNC)		/* P2�̋@�\���W�X�^�ݒ萔 */
#define	PORT_FUNC_CHANGE3	(PORT_FUNC_CHANGE_NUM3 + PORT_FUNC)		/* P3�̋@�\���W�X�^�ݒ萔 */
#define	PORT_FUNC_CHANGE4	(PORT_FUNC_CHANGE_NUM4 + PORT_FUNC)		/* P4�̋@�\���W�X�^�ݒ萔 */
#define	PORT_FUNC_CHANGE5	(PORT_FUNC_CHANGE_NUM5 + PORT_FUNC)		/* P5�̋@�\���W�X�^�ݒ萔 */
#define	PORT_FUNC_CHANGE6	(PORT_FUNC_CHANGE_NUM6 + PORT_FUNC)		/* P6�̋@�\���W�X�^�ݒ萔 */
#define	PORT_FUNC_CHANGE7	(PORT_FUNC_CHANGE_NUM7 + PORT_FUNC)		/* P7�̋@�\���W�X�^�ݒ萔 */
#define	PORT_FUNC_CHANGE8	(PORT_FUNC_CHANGE_NUM8 + PORT_FUNC)		/* P8�̋@�\���W�X�^�ݒ萔 */
#define	PORT_FUNC_CHANGE9	(PORT_FUNC_CHANGE_NUM9 + PORT_FUNC)		/* P9�̋@�\���W�X�^�ݒ萔 */
#define	PORT_FUNC_CHANGE10	(PORT_FUNC_CHANGE_NUM10 + PORT_FUNC)	/* P10�̋@�\���W�X�^�ݒ萔 */
#define	PORT_FUNC_CHANGE11	(PORT_FUNC_CHANGE_NUM11 + PORT_FUNC)	/* P11�̋@�\���W�X�^�ݒ萔 */
#define	PORT_FUNC_CHANGE12	(PORT_FUNC_CHANGE_NUM12 + PORT_FUNC)	/* P12�̋@�\���W�X�^�ݒ萔 */
#define	PORT_FUNC_CHANGE13	(PORT_FUNC_CHANGE_NUM13 + PORT_FUNC)	/* P13�̋@�\���W�X�^�ݒ萔 */

#define	PORT_TABLE_DDR		(0)		/* �f�[�^���� */
#define	PORT_TABLE_PFR		(1)		/* �|�[�g�@�\ */
#define	PORT_TABLE_EPFR		(2)		/* �g���|�[�g�@�\�ݒ�J�n�l */

#define	PORT_FUNC_CHANGE_ID_NUM_3	(3)		/* �@�\�ؑ֐ݒ�l�e�[�u���̋@�\��(��v�f)3 */
#define	PORT_FUNC_CHANGE_ID_NUM_4	(4)		/* �@�\�ؑ֐ݒ�l�e�[�u���̋@�\��(��v�f)4 */
#define	PORT_FUNC_CHANGE_ID_NUM_5	(5)		/* �@�\�ؑ֐ݒ�l�e�[�u���̋@�\��(��v�f)5 */
#define	PORT_FUNC_CHANGE_ID_NUM_6	(6)		/* �@�\�ؑ֐ݒ�l�e�[�u���̋@�\��(��v�f)6 */
#define	PORT_FUNC_CHANGE_ID_NUM_7	(7)		/* �@�\�ؑ֐ݒ�l�e�[�u���̋@�\��(��v�f)7 */
#define	PORT_FUNC_CHANGE_ID_NUM_8	(8)		/* �@�\�ؑ֐ݒ�l�e�[�u���̋@�\��(��v�f)8 */
#define	PORT_FUNC_CHANGE_ID_NUM_9	(9)		/* �@�\�ؑ֐ݒ�l�e�[�u���̋@�\��(��v�f)9 */
#define	PORT_FUNC_CHANGE_ID_NUM_10	(10)	/* �@�\�ؑ֐ݒ�l�e�[�u���̋@�\��(��v�f)10 */
#define	PORT_FUNC_CHANGE_ID_NUM_11	(11)	/* �@�\�ؑ֐ݒ�l�e�[�u���̋@�\��(��v�f)11 */

#define DATA_MIN			(0)			/* �f�[�^�͈͍ŏ��l */
#define PORT_FUNC			(1)			/* �e�[�u���v�fID�ǉ��p */
#define PORTDRV_EPFR_MAX	(51)		/* �g���|�[�g�@�\���W�X�^�� */
#define BIT_GET				(0x0700u)	/* �r�b�g�ʒu���o�p */
#define BIT_SHIFT			(8)			/* �r�b�g�V�t�g�p */

#define	NO_MASK			(0x8000u)	/* �}�X�N�Ȃ� */

/* �S�� */
#define D_LOW_PXX0		(0x40FEu)	/* �r�b�g0�Z�b�g�lLOW */
#define D_LOW_PXX1		(0x40FDu)	/* �r�b�g1�Z�b�g�lLOW */
#define D_LOW_PXX2		(0x40FBu)	/* �r�b�g2�Z�b�g�lLOW */
#define D_LOW_PXX3		(0x40F7u)	/* �r�b�g3�Z�b�g�lLOW */
#define D_LOW_PXX4		(0x40EFu)	/* �r�b�g4�Z�b�g�lLOW */
#define D_LOW_PXX5		(0x40DFu)	/* �r�b�g5�Z�b�g�lLOW */
#define D_LOW_PXX6		(0x40BFu)	/* �r�b�g6�Z�b�g�lLOW */
#define D_LOW_PXX7		(0x407Fu)	/* �r�b�g7�Z�b�g�lLOW */

#define D_HIGH_PXX0		(0x2001u)	/* �r�b�g0�Z�b�g�lHIGH */
#define D_HIGH_PXX1		(0x2002u)	/* �r�b�g1�Z�b�g�lHIGH */
#define D_HIGH_PXX2		(0x2004u)	/* �r�b�g2�Z�b�g�lHIGH */
#define D_HIGH_PXX3		(0x2008u)	/* �r�b�g3�Z�b�g�lHIGH */
#define D_HIGH_PXX4		(0x2010u)	/* �r�b�g4�Z�b�g�lHIGH */
#define D_HIGH_PXX5		(0x2020u)	/* �r�b�g5�Z�b�g�lHIGH */
#define D_HIGH_PXX6		(0x2040u)	/* �r�b�g6�Z�b�g�lHIGH */
#define D_HIGH_PXX7		(0x2080u)	/* �r�b�g7�Z�b�g�lHIGH */

/* <<P00>> */
	
/* <P000> */
#define EPFR02_TIN0E_10		(0x11FCu)	/* EPFR02_TIN0E�̃Z�b�g�l10 */
#define EPFR06_SIN2E_HIGH	(0x2001u)	/* EPFR06_SIN2E�̃Z�b�g�lHIGH */
#define EPFR10_PPG0E_XX0	(0x40FEu)	/* EPFR10_PPG0E�̃Z�b�g�lXX0 */
#define EPFR10_PPG0E_XX1	(0x2001u)	/* EPFR10_PPG0E�̃Z�b�g�lXX1 */
#define EPFR43_INT0E_HIGH	(0x2001u)	/* EPFR43_INT0E�̃Z�b�g�lHIGH */
#define EPFR47_EBDSWPE_LOW	(0x40FEu)	/* EPFR47_EBDSWPE�̃Z�b�g�lLOW */
#define EPFR47_EBDSWPE_HIGH	(0x2001u)	/* EPFR47_EBDSWPE�̃Z�b�g�lHIGH */

/* <P001> */
#define EPFR03_TIN1E_10		(0x11FCu)	/* EPFR03_TIN1E�̃Z�b�g�l10 */
#define EPFR06_SOT2E_0X		(0x40EFu)	/* EPFR06_SOT2E�̃Z�b�g�l0X */
#define EPFR06_SOT2E_1X		(0x2010u)	/* EPFR06_SOT2E�̃Z�b�g�l1X */
#define EPFR10_PPG1E_XXX0	(0x40F7u)	/* EPFR10_PPG1E�̃Z�b�g�lXXX0 */
#define EPFR10_PPG1E_XXX1	(0x2008u)	/* EPFR10_PPG1E�̃Z�b�g�lXXX1 */
#define EPFR43_INT1E_HIGH	(0x2002u)	/* EPFR43_INT1E�̃Z�b�g�lHIGH */

/* <P002> */
#define EPFR04_TIN2E_10		(0x11FCu)	/* EPFR04_TIN2E�̃Z�b�g�l10 */
#define EPFR06_SCK2E_0X		(0x40FBu)	/* EPFR06_SCK2E�̃Z�b�g�l0X */
#define EPFR06_SCK2E_10		(0x12F9u)	/* EPFR06_SCK2E�̃Z�b�g�l10 */
#define EPFR11_PPG2E_XX0	(0x40FEu)	/* EPFR11_PPG2E�̃Z�b�g�lXX0 */
#define EPFR11_PPG2E_XX1	(0x2001u)	/* EPFR11_PPG2E�̃Z�b�g�lXX1 */
#define EPFR43_INT2E_HIGH	(0x2004u)	/* EPFR43_INT2E�̃Z�b�g�lHIGH */

/* <P003> */
#define EPFR05_TIN3E_10		(0x11FCu)	/* EPFR05_TIN3E�̃Z�b�g�l10 */
#define EPFR07_SIN3E_HIGH	(0x2001u)	/* EPFR07_SIN3E�̃Z�b�g�lHIGH */
#define EPFR11_PPG3E_XX0	(0x40F7u)	/* EPFR11_PPG3E�̃Z�b�g�lXX0 */
#define EPFR11_PPG3E_XX1	(0x2008u)	/* EPFR11_PPG3E�̃Z�b�g�lXX1 */
#define EPFR43_INT3E_HIGH	(0x2008u)	/* EPFR43_INT3E�̃Z�b�g�lHIGH */

/* <P004> */
#define EPFR02_TOT0E_0XX	(0x40EFu)	/* EPFR02_TOT0E�̃Z�b�g�l0XX */
#define EPFR02_TOT0E_1XX	(0x2010u)	/* EPFR02_TOT0E�̃Z�b�g�l1XX */
#define EPFR07_SOT3E_0X		(0x40EFu)	/* EPFR07_SOT3E�̃Z�b�g�l0X */
#define EPFR07_SOT3E_1X		(0x2010u)	/* EPFR07_SOT3E�̃Z�b�g�l1X */
#define EPFR12_PPG4E_XX0	(0x40FEu)	/* EPFR12_PPG4E�̃Z�b�g�lXX0 */
#define EPFR12_PPG4E_XX1	(0x2001u)	/* EPFR12_PPG4E�̃Z�b�g�lXX1 */
#define EPFR43_INT4E_HIGH	(0x2010u)	/* EPFR43_INT4E�̃Z�b�g�lHIGH */

/* <P005> */
#define EPFR03_TOT1E_0XX	(0x40EFu)	/* EPFR03_TOT1E�̃Z�b�g�l0XX */
#define EPFR03_TOT1E_1XX	(0x2010u)	/* EPFR03_TOT1E�̃Z�b�g�l1XX */
#define EPFR07_SCK3E_0X		(0x40FBu)	/* EPFR07_SCK3E�̃Z�b�g�l0X */
#define EPFR07_SCK3E_10		(0x12F9u)	/* EPFR07_SCK3E�̃Z�b�g�l10 */
#define EPFR12_PPG5E_XX0	(0x40F7u)	/* EPFR12_PPG5E�̃Z�b�g�lXX0 */
#define EPFR12_PPG5E_XX1	(0x2008u)	/* EPFR12_PPG5E�̃Z�b�g�lXX1 */
#define EPFR43_INT5E_HIGH	(0x2020u)	/* EPFR43_INT5E�̃Z�b�g�lHIGH */

/* <P006> */
#define EPFR04_TOT2E_0XX	(0x40EFu)	/* EPFR04_TOT2E�̃Z�b�g�l0XX */
#define EPFR04_TOT2E_1XX	(0x2010u)	/* EPFR04_TOT2E�̃Z�b�g�l1XX */
#define EPFR13_PPG6E_XX0	(0x40FEu)	/* EPFR13_PPG6E�̃Z�b�g�lXX0 */
#define EPFR13_PPG6E_XX1	(0x2001u)	/* EPFR13_PPG6E�̃Z�b�g�lXX1 */
#define EPFR43_INT6E_HIGH	(0x2040u)	/* EPFR43_INT6E�̃Z�b�g�lHIGH */

/* <P007> */
#define EPFR05_TOT3E_0XX	(0x40EFu)	/* EPFR05_TOT3E�̃Z�b�g�l0XX */
#define EPFR05_TOT3E_1XX	(0x2010u)	/* EPFR05_TOT3E�̃Z�b�g�l1XX */
#define EPFR13_PPG7E_XX0	(0x40F7u)	/* EPFR13_PPG7E�̃Z�b�g�lXX0 */
#define EPFR13_PPG7E_XX1	(0x2008u)	/* EPFR13_PPG7E�̃Z�b�g�lXX1 */
#define EPFR43_INT7E_HIGH	(0x2080u)	/* EPFR43_INT7E�̃Z�b�g�lHIGH */

/* <<P01>> */

/* <P010> */
#define EPFR16_SEG0E_LOW	(0x40FEu)	/* EPFR16_SEG0E�̃Z�b�g�lLOW */
#define EPFR16_SEG0E_HIGH	(0x2001u)	/* EPFR16_SEG0E�̃Z�b�g�lHIGH */
#define EPFR44_INT8E_HIGH	(0x2001u)	/* EPFR44_INT8E�̃Z�b�g�lHIGH */

/* <P011> */
#define EPFR16_SEG1E_LOW	(0x40FDu)	/* EPFR16_SEG1E�̃Z�b�g�lLOW */
#define EPFR16_SEG1E_HIGH	(0x2002u)	/* EPFR16_SEG1E�̃Z�b�g�lHIGH */
#define EPFR44_INT9E_HIGH	(0x2002u)	/* EPFR44_INT9E�̃Z�b�g�lHIGH */

/* <P012> */
#define EPFR16_SEG2E_LOW	(0x40FBu)	/* EPFR16_SEG2E�̃Z�b�g�lLOW */
#define EPFR16_SEG2E_HIGH	(0x2004u)	/* EPFR16_SEG2E�̃Z�b�g�lHIGH */
#define EPFR44_INT10E_HIGH	(0x2004u)	/* EPFR44_INT10E�̃Z�b�g�lHIGH */

/* <P013> */
#define EPFR16_SEG3E_LOW	(0x40F7u)	/* EPFR16_SEG3E�̃Z�b�g�lLOW */
#define EPFR16_SEG3E_HIGH	(0x2008u)	/* EPFR16_SEG3E�̃Z�b�g�lHIGH */
#define EPFR44_INT11E_HIGH	(0x2008u)	/* EPFR44_INT11E�̃Z�b�g�lHIGH */

/* <P014> */
#define EPFR16_SEG4E_LOW	(0x40EFu)	/* EPFR16_SEG4E�̃Z�b�g�lLOW */
#define EPFR16_SEG4E_HIGH	(0x2010u)	/* EPFR16_SEG4E�̃Z�b�g�lHIGH */
#define EPFR44_INT12E_HIGH	(0x2010u)	/* EPFR44_INT12E�̃Z�b�g�lHIGH */

/* <P015> */
#define EPFR16_SEG5E_LOW	(0x40DFu)	/* EPFR16_SEG5E�̃Z�b�g�lLOW */
#define EPFR16_SEG5E_HIGH	(0x2020u)	/* EPFR16_SEG5E�̃Z�b�g�lHIGH */
#define EPFR44_INT13E_HIGH	(0x2020u)	/* EPFR44_INT13E�̃Z�b�g�lHIGH */

/* <P016> */
#define EPFR16_SEG6E_LOW	(0x40BFu)	/* EPFR16_SEG6E�̃Z�b�g�lLOW */
#define EPFR16_SEG6E_HIGH	(0x2040u)	/* EPFR16_SEG6E�̃Z�b�g�lHIGH */
#define EPFR44_INT14E_HIGH	(0x2040u)	/* EPFR44_INT14E�̃Z�b�g�lHIGH */

/* <P017> */
#define EPFR16_SEG7E_LOW	(0x407Fu)	/* EPFR16_SEG7E�̃Z�b�g�lLOW */
#define EPFR16_SEG7E_HIGH	(0x2080u)	/* EPFR16_SEG7E�̃Z�b�g�lHIGH */
#define EPFR44_INT15E_HIGH	(0x2080u)	/* EPFR44_INT15E�̃Z�b�g�lHIGH */

/* <<P02>> */

/* <P020> */
#define EPFR01_ICU6E_00		(0x40CFu)	/* EPFR01_ICU6E�̃Z�b�g�l00 */
#define EPFR17_SEG8E_LOW	(0x40FEu)	/* EPFR17_SEG8E�̃Z�b�g�lLOW */
#define EPFR17_SEG8E_HIGH	(0x2001u)	/* EPFR17_SEG8E�̃Z�b�g�lHIGH */
#define EPFR29_OCU0E_0X		(0x40FDu)	/* EPFR29_OCU0E�̃Z�b�g�l0X */
#define EPFR29_OCU0E_10		(0x11FCu)	/* EPFR29_OCU0E�̃Z�b�g�l10 */

/* <P021> */
#define EPFR01_ICU7E_00		(0x403Fu)	/* EPFR01_ICU7E�̃Z�b�g�l00 */
#define EPFR17_SEG9E_LOW	(0x40FDu)	/* EPFR17_SEG9E�̃Z�b�g�lLOW */
#define EPFR17_SEG9E_HIGH	(0x2002u)	/* EPFR17_SEG9E�̃Z�b�g�lHIGH */
#define EPFR29_OCU1E_0X		(0x40F7u)	/* EPFR29_OCU1E�̃Z�b�g�l0X */
#define EPFR29_OCU1E_10		(0x13F3u)	/* EPFR29_OCU1E�̃Z�b�g�l10 */

/* <P022> */
#define EPFR17_SEG10E_LOW	(0x40FBu)	/* EPFR17_SEG10E�̃Z�b�g�lLOW */
#define EPFR17_SEG10E_HIGH	(0x2004u)	/* EPFR17_SEG10E�̃Z�b�g�lHIGH */
#define EPFR29_OCU2E_0X		(0x40DFu)	/* EPFR29_OCU2E�̃Z�b�g�l0X */
#define EPFR29_OCU2E_10		(0x15CFu)	/* EPFR29_OCU2E�̃Z�b�g�l10 */
#define EPFR39_ICU8E_00		(0x40FCu)	/* EPFR39_ICU8E�̃Z�b�g�l00 */

/* <P023> */
#define EPFR17_SEG11E_LOW	(0x40F7u)	/* EPFR17_SEG11E�̃Z�b�g�lLOW */
#define EPFR17_SEG11E_HIGH	(0x2008u)	/* EPFR17_SEG11E�̃Z�b�g�lHIGH */
#define EPFR29_OCU3E_0X		(0x407Fu)	/* EPFR29_OCU3E�̃Z�b�g�l0X */
#define EPFR29_OCU3E_10		(0x173Fu)	/* EPFR29_OCU3E�̃Z�b�g�l10 */
#define EPFR39_ICU9E_00		(0x40F3u)	/* EPFR39_ICU9E�̃Z�b�g�l00 */

/* <P024> */
#define EPFR17_SEG12E_LOW	(0x40EFu)	/* EPFR17_SEG12E�̃Z�b�g�lLOW */
#define EPFR17_SEG12E_HIGH	(0x2010u)	/* EPFR17_SEG12E�̃Z�b�g�lHIGH */
#define EPFR32_OCU11E_X0	(0x40BFu)	/* EPFR32_OCU11E�̃Z�b�g�lX0 */
#define EPFR32_OCU11E_01	(0x163Fu)	/* EPFR32_OCU11E�̃Z�b�g�l01 */
#define EPFR39_ICU10E_00	(0x40CFu)	/* EPFR39_ICU10E�̃Z�b�g�l00 */

/* <P025> */
#define EPFR17_SEG13E_LOW	(0x40DFu)	/* EPFR17_SEG13E�̃Z�b�g�lLOW */
#define EPFR17_SEG13E_HIGH	(0x2020u)	/* EPFR17_SEG13E�̃Z�b�g�lHIGH */
#define EPFR32_OCU10E_X0	(0x40EFu)	/* EPFR32_OCU10E�̃Z�b�g�l00 */
#define EPFR32_OCU10E_01	(0x14CFu)	/* EPFR32_OCU10E�̃Z�b�g�l01 */
#define EPFR39_ICU11E_00	(0x403Fu)	/* EPFR39_ICU11E�̃Z�b�g�l00 */

/* <P026> */
#define EPFR17_SEG14E_LOW	(0x40BFu)	/* EPFR17_SEG14E�̃Z�b�g�lLOW */
#define EPFR17_SEG14E_HIGH	(0x2040u)	/* EPFR17_SEG14E�̃Z�b�g�lHIGH */
#define EPFR32_OCU9E_X0		(0x40FBu)	/* EPFR32_OCU9E�̃Z�b�g�lX0 */
#define EPFR32_OCU9E_01		(0x12F3u)	/* EPFR32_OCU9E�̃Z�b�g�l01 */
#define EPFR33_SIN6E_HIGH	(0x2001u)	/* EPFR33_SIN6E�̃Z�b�g�lHIGH */

/* <P027> */
#define EPFR17_SEG15E_LOW	(0x407Fu)	/* EPFR17_SEG15E�̃Z�b�g�lLOW */
#define EPFR17_SEG15E_HIGH	(0x2080u)	/* EPFR17_SEG15E�̃Z�b�g�lHIGH */
#define EPFR32_OCU8E_X0		(0x40FEu)	/* EPFR32_OCU8E�̃Z�b�g�lX0 */
#define EPFR32_OCU8E_01		(0x10FCu)	/* EPFR32_OCU8E�̃Z�b�g�l01 */
#define EPFR33_SOT6E_0X		(0x40EFu)	/* EPFR33_SOT6E�̃Z�b�g�l0X */
#define EPFR33_SOT6E_1X		(0x2010u)	/* EPFR33_SOT6E�̃Z�b�g�l1X */

/* <<P03>> */

/* <P030> */
#define EPFR18_SEG16E_LOW	(0x40FEu)	/* EPFR18_SEG16E�̃Z�b�g�lLOW */
#define EPFR18_SEG16E_HIGH	(0x2001u)	/* EPFR18_SEG16E�̃Z�b�g�lHIGH */
#define EPFR33_SCK6E_0X		(0x40FBu)	/* EPFR33_SCK6E�̃Z�b�g�l0X */ 
#define EPFR33_SCK6E_10		(0x12F9u)	/* EPFR33_SCK6E�̃Z�b�g�l10 */

/* <P031> */
#define EPFR18_SEG17E_LOW	(0x40FDu)	/* EPFR18_SEG17E�̃Z�b�g�lLOW */
#define EPFR18_SEG17E_HIGH	(0x2002u)	/* EPFR18_SEG17E�̃Z�b�g�lHIGH */
#define EPFR38_SIN9E_LOW	(0x40FEu)	/* EPFR38_SIN9E�̃Z�b�g�lLOW */

/* <P032> */
#define EPFR18_SEG18E_LOW	(0x40BFu)	/* EPFR18_SEG18E�̃Z�b�g�lLOW */
#define EPFR18_SEG18E_HIGH	(0x2004u)	/* EPFR18_SEG18E�̃Z�b�g�lHIGH */
#define EPFR31_OCU7E_X0		(0x40BFu)	/* EPFR31_OCU7E�̃Z�b�g�lX0 */
#define EPFR31_OCU7E_01		(0x163Fu)	/* EPFR31_OCU7E�̃Z�b�g�l01 */
#define EPFR38_SOT9E_X0		(0x40F7u)	/* EPFR38_SOT9E�̃Z�b�g�lX0 */
#define EPFR38_SOT9E_01		(0x13E7u)	/* EPFR38_SOT9E�̃Z�b�g�l01 */

/* <P033> */
#define EPFR18_SEG19E_LOW	(0x40F7u)	/* EPFR18_SEG19E�̃Z�b�g�lLOW */
#define EPFR18_SEG19E_HIGH	(0x2008u)	/* EPFR18_SEG19E�̃Z�b�g�lHIGH */
#define EPFR31_OCU6E_X0		(0x40EFu)	/* EPFR31_OCU6E�̃Z�b�g�lX0 */
#define EPFR31_OCU6E_01		(0x14CFu)	/* EPFR31_OCU6E�̃Z�b�g�l01 */
#define EPFR38_SCK9E_X0		(0x40FDu)	/* EPFR38_SCK9E�̃Z�b�g�lX0 */
#define EPFR38_SCK9E_01		(0x11F9u)	/* EPFR38_SCK9E�̃Z�b�g�l01 */

/* <P034> */
#define EPFR18_SEG20E_LOW	(0x40EFu)	/* EPFR18_SEG20E�̃Z�b�g�lLOW */
#define EPFR18_SEG20E_HIGH	(0x2010u)	/* EPFR18_SEG20E�̃Z�b�g�lHIGH */
#define EPFR31_OCU5E_0X		(0x40F7u)	/* EPFR31_OCU5E�̃Z�b�g�l0X */
#define EPFR31_OCU5E_10		(0x13F3u)	/* EPFR31_OCU5E�̃Z�b�g�l10 */
#define EPFR37_SIN8E_00		(0x40FCu)	/* EPFR37_SIN8E�̃Z�b�g�l00 */

/* <P035> */
#define EPFR18_SEG21E_LOW	(0x40DFu)	/* EPFR18_SEG21E�̃Z�b�g�lLOW */
#define EPFR18_SEG21E_HIGH	(0x2020u)	/* EPFR18_SEG21E�̃Z�b�g�lHIGH */
#define EPFR31_OCU4E_0X		(0x40FDu)	/* EPFR31_OCU4E�̃Z�b�g�l0X */
#define EPFR31_OCU4E_10		(0x11FCu)	/* EPFR31_OCU4E�̃Z�b�g�l10 */
#define EPFR37_SOT8E_XX0	(0x40DFu)	/* EPFR37_SOT8E�̃Z�b�g�lXX0 */
#define EPFR37_SOT8E_001	(0x151Fu)	/* EPFR37_SOT8E�̃Z�b�g�l001 */

/* <P036> */
#define EPFR15_PPG11E_X0	(0x40F7u)	/* EPFR15_PPG11E�̃Z�b�g�lX0 */
#define EPFR15_PPG11E_01	(0x13E7u)	/* EPFR15_PPG11E�̃Z�b�g�l01 */
#define EPFR18_SEG22E_LOW	(0x40BFu)	/* EPFR18_SEG22E�̃Z�b�g�lLOW */
#define EPFR18_SEG22E_HIGH	(0x2040u)	/* EPFR18_SEG22E�̃Z�b�g�lHIGH */
#define EPFR37_SCK8E_XX0	(0x40FBu)	/* EPFR37_SCK8E�̃Z�b�g�lXX0 */
#define EPFR37_SCK8E_001	(0x12E3u)	/* EPFR37_SCK8E�̃Z�b�g�l001 */

/* <P037> */
#define EPFR15_PPG12E_X0	(0x40BFu)	/* EPFR15_PPG12E�̃Z�b�g�lX0 */
#define EPFR15_PPG12E_01	(0x159Fu)	/* EPFR15_PPG12E�̃Z�b�g�l01 */
#define EPFR18_SEG23E_LOW	(0x407Fu)	/* EPFR18_SEG23E�̃Z�b�g�lLOW */
#define EPFR18_SEG23E_HIGH	(0x2080u)	/* EPFR18_SEG23E�̃Z�b�g�lHIGH */
#define EPFR34_SIN7E_LOW	(0x40FEu)	/* EPFR34_SIN7E�̃Z�b�g�lLOW */

/* <<P04>> */

/* <P040> */
#define EPFR19_SEG24E_LOW	(0x40FEu)	/* EPFR19_SEG24E�̃Z�b�g�lLOW */
#define EPFR19_SEG24E_HIGH	(0x2001u)	/* EPFR19_SEG24E�̃Z�b�g�lHIGH */
#define EPFR34_SOT7E_X0		(0x40F7u)	/* EPFR34_SOT7E�̃Z�b�g�lX0 */
#define EPFR34_SOT7E_X1		(0x2008u)	/* EPFR34_SOT7E�̃Z�b�g�lX1 */
#define EPFR45_PPG13E_X0	(0x40FEu)	/* EPFR45_PPG13E�̃Z�b�g�lX0 */
#define EPFR45_PPG13E_01	(0x10FCu)	/* EPFR45_PPG13E�̃Z�b�g�l01 */

/* <P041> */
#define EPFR19_SEG25E_LOW	(0x40FDu)	/* EPFR19_SEG25E�̃Z�b�g�lLOW */
#define EPFR19_SEG25E_HIGH	(0x2002u)	/* EPFR19_SEG25E�̃Z�b�g�lHIGH */
#define EPFR34_SCK7E_X0		(0x40FDu)	/* EPFR34_SCK7E�̃Z�b�g�lX0 */
#define EPFR34_SCK7E_01		(0x11F9u)	/* EPFR34_SCK7E�̃Z�b�g�l01 */
#define EPFR45_PPG14E_X0	(0x40FBu)	/* EPFR45_PPG14E�̃Z�b�g�lX0 */
#define EPFR45_PPG14E_01	(0x12F3u)	/* EPFR45_PPG14E�̃Z�b�g�l01 */

/* <P042> */
#define EPFR19_SEG26E_LOW	(0x40FBu)	/* EPFR19_SEG26E�̃Z�b�g�lLOW */
#define EPFR19_SEG26E_HIGH	(0x2004u)	/* EPFR19_SEG26E�̃Z�b�g�lHIGH */
#define EPFR40_AIN0E_00		(0x40FCu)	/* EPFR40_AIN0E�̃Z�b�g�l00 */
#define EPFR45_PPG15E_X0	(0x40EFu)	/* EPFR45_PPG15E�̃Z�b�g�lX0 */
#define EPFR45_PPG15E_01	(0x14CFu)	/* EPFR45_PPG15E�̃Z�b�g�l01 */

/* <P043> */
#define EPFR19_SEG27E_LOW	(0x40F7u)	/* EPFR19_SEG27E�̃Z�b�g�lLOW */
#define EPFR19_SEG27E_HIGH	(0x2008u)	/* EPFR19_SEG27E�̃Z�b�g�lHIGH */
#define EPFR30_SGA4E_X0		(0x40EFu)	/* EPFR30_SGA4E�̃Z�b�g�lX0 */
#define EPFR30_SGA4E_01		(0x14CFu)	/* EPFR30_SGA4E�̃Z�b�g�l01 */
#define EPFR31_OCU6E_0X		(0x40DFu)	/* EPFR31_OCU6E�̃Z�b�g�l0X */
#define EPFR31_OCU6E_10		(0x15CFu)	/* EPFR31_OCU6E�̃Z�b�g�l10 */
#define EPFR40_BIN0E_00		(0x40F3u)	/* EPFR40_BIN0E�̃Z�b�g�l00 */

/* <P044> */
#define EPFR19_SEG28E_LOW	(0x40EFu)	/* EPFR19_SEG28E�̃Z�b�g�lLOW */
#define EPFR19_SEG28E_HIGH	(0x2010u)	/* EPFR19_SEG28E�̃Z�b�g�lHIGH */
#define EPFR30_SGO4E_X0		(0x40BFu)	/* EPFR30_SGO4E�̃Z�b�g�lX0 */
#define EPFR30_SGO4E_01		(0x163Fu)	/* EPFR30_SGO4E�̃Z�b�g�l01 */
#define EPFR31_OCU7E_0X		(0x407Fu)	/* EPFR31_OCU7E�̃Z�b�g�l0X */
#define EPFR31_OCU7E_10		(0x173Fu)	/* EPFR31_OCU7E�̃Z�b�g�l10 */
#define EPFR40_ZIN0E_00		(0x40CFu)	/* EPFR40_ZIN0E�̃Z�b�g�l00 */

/* <P045> */
#define EPFR19_SEG29E_LOW	(0x40DFu)	/* EPFR19_SEG29E�̃Z�b�g�lLOW */
#define EPFR19_SEG29E_HIGH	(0x2020u)	/* EPFR19_SEG29E�̃Z�b�g�lHIGH */
#define EPFR37_SIN8E_10		(0x11FCu)	/* EPFR37_SIN8E�̃Z�b�g�l10 */
#define EPFR41_AIN1E_LOW	(0x40FEu)	/* EPFR41_AIN1E�̃Z�b�g�lLOW */

/* <P046> */
#define EPFR19_SEG30E_LOW	(0x40BFu)	/* EPFR19_SEG30E�̃Z�b�g�lLOW */
#define EPFR19_SEG30E_HIGH	(0x2040u)	/* EPFR19_SEG30E�̃Z�b�g�lHIGH */
#define EPFR37_SOT8E_0XX	(0x407Fu)	/* EPFR37_SOT8E�̃Z�b�g�l0XX */
#define EPFR37_SOT8E_100	(0x171Fu)	/* EPFR37_SOT8E�̃Z�b�g�l100 */
#define EPFR41_BIN1E_LOW	(0x40FDu)	/* EPFR41_BIN1E�̃Z�b�g�lLOW */

/* <P047> */
#define EPFR19_SEG31E_LOW	(0x407Fu)	/* EPFR19_SEG31E�̃Z�b�g�lLOW */
#define EPFR19_SEG31E_HIGH	(0x2080u)	/* EPFR19_SEG31E�̃Z�b�g�lHIGH */
#define EPFR37_SCK8E_0XX	(0x40EFu)	/* EPFR37_SCK8E�̃Z�b�g�l0XX */
#define EPFR37_SCK8E_100	(0x14E3u)	/* EPFR37_SCK8E�̃Z�b�g�l100 */
#define EPFR41_ZIN1E_LOW	(0x40FBu)	/* EPFR41_ZIN1E�̃Z�b�g�lLOW */

/* <<P05>> */

/* <P050> */
#define EPFR20_COM0E_LOW	(0x40FEu)	/* EPFR20_COM0E�̃Z�b�g�lLOW */
#define EPFR20_COM0E_HIGH	(0x2001u)	/* EPFR20_COM0E�̃Z�b�g�lHIGH */
#define EPFR32_OCU8E_0X		(0x40FDu)	/* EPFR32_OCU8E�̃Z�b�g�l0X */
#define EPFR32_OCU8E_10		(0x11FCu)	/* EPFR32_OCU8E�̃Z�b�g�l10 */

/* <P051> */
#define EPFR20_COM1E_LOW	(0x40FDu)	/* EPFR20_COM1E�̃Z�b�g�lLOW */
#define EPFR20_COM1E_HIGH	(0x2002u)	/* EPFR20_COM1E�̃Z�b�g�lHIGH */
#define EPFR32_OCU9E_0X		(0x40F7u)	/* EPFR32_OCU9E�̃Z�b�g�l0X */
#define EPFR32_OCU9E_10		(0x13F3u)	/* EPFR32_OCU9E�̃Z�b�g�l10 */

/* <P052> */
#define EPFR20_COM2E_LOW	(0x40FBu)	/* EPFR20_COM2E�̃Z�b�g�lLOW */
#define EPFR20_COM2E_HIGH	(0x2004u)	/* EPFR20_COM2E�̃Z�b�g�lHIGH */
#define EPFR32_OCU10E_0X	(0x40DFu)	/* EPFR32_OCU10E�̃Z�b�g�l0X */
#define EPFR32_OCU10E_10	(0x15CFu)	/* EPFR32_OCU10E�̃Z�b�g�l10 */

/* <P053> */
#define EPFR20_COM3E_LOW	(0x40F7u)	/* EPFR20_COM3E�̃Z�b�g�lLOW */
#define EPFR20_COM3E_HIGH	(0x2008u)	/* EPFR20_COM3E�̃Z�b�g�lHIGH */
#define EPFR32_OCU11E_0X	(0x407Fu)	/* EPFR32_OCU11E�̃Z�b�g�l0X */
#define EPFR32_OCU11E_10	(0x173Fu)	/* EPFR32_OCU11E�̃Z�b�g�l10 */

/* <P054> */
#define EPFR20_V0E_LOW		(0x40EFu)	/* EPFR20_V0E�̃Z�b�g�lLOW */
#define EPFR20_V0E_HIGH		(0x2010u)	/* EPFR20_V0E�̃Z�b�g�lHIGH */
#define EPFR28_FRCK0E_HIGH	(0x2001u)	/* EPFR28_FRCK0E�̃Z�b�g�lHIGH */

/* <P055> */
#define EPFR20_V1E_LOW		(0x40DFu)	/* EPFR20_V1E�̃Z�b�g�lLOW */
#define EPFR20_V1E_HIGH		(0x2020u)	/* EPFR20_V1E�̃Z�b�g�lHIGH */
#define EPFR28_FRCK1E_HIGH	(0x2002u)	/* EPFR28_FRCK1E�̃Z�b�g�lHIGH */

/* <P056> */
#define EPFR20_V2E_LOW		(0x40BFu)	/* EPFR20_V2E�̃Z�b�g�lLOW */
#define EPFR20_V2E_HIGH		(0x2040u)	/* EPFR20_V2E�̃Z�b�g�lHIGH */
#define EPFR28_FRCK2E_HIGH	(0x2004u)	/* EPFR28_FRCK2E�̃Z�b�g�lHIGH */

/* <P057> */
#define EPFR20_V3E_LOW		(0x407Fu)	/* EPFR20_V3E�̃Z�b�g�lLOW */
#define EPFR20_V3E_HIGH		(0x2080u)	/* EPFR20_V3E�̃Z�b�g�lHIGH */
#define EPFR28_FRCK3E_HIGH	(0x2008u)	/* EPFR28_FRCK3E�̃Z�b�g�lHIGH */

/* <<P06>> */

/* <P060> */
#define EPFR21_PWM1P0E_LOW	(0x40FEu)	/* EPFR21_PWM1P0E�̃Z�b�g�lLOW */
#define EPFR21_PWM1P0E_HIGH	(0x2001u)	/* EPFR21_PWM1P0E�̃Z�b�g�lHIGH */

/* <P061> */
#define EPFR21_PWM1M0E_LOW	(0x40FDu)	/* EPFR21_PWM1M0E�̃Z�b�g�lLOW */
#define EPFR21_PWM1M0E_HIGH	(0x2002u)	/* EPFR21_PWM1M0E�̃Z�b�g�lHIGH */
#define EPFR36_SIN1E_HIGH	(0x2001u)	/* EPFR36_SIN1E�̃Z�b�g�lHIGH */

/* <P062> */
#define EPFR21_PWM2P0E_LOW	(0x40FBu)	/* EPFR21_PWM2P0E�̃Z�b�g�lLOW */
#define EPFR21_PWM2P0E_HIGH	(0x2004u)	/* EPFR21_PWM2P0E�̃Z�b�g�lHIGH */
#define EPFR36_SOT1E_0X		(0x40EFu)	/* EPFR36_SOT1E�̃Z�b�g�l0X */
#define EPFR36_SOT1E_10		(0x14E7u)	/* EPFR36_SOT1E�̃Z�b�g�l10 */
#define EPFR41_ZIN1E_HIGH	(0x2004u)	/* EPFR41_ZIN1E�̃Z�b�g�lHIGH */

/* <P063> */
#define EPFR21_PWM2M0E_LOW	(0x40F7u)	/* EPFR21_PWM2M0E�̃Z�b�g�lLOW */
#define EPFR21_PWM2M0E_HIGH	(0x2008u)	/* EPFR21_PWM2M0E�̃Z�b�g�lHIGH */
#define EPFR36_SCK1E_0X		(0x40FBu)	/* EPFR36_SCK1E�̃Z�b�g�l0X */
#define EPFR36_SCK1E_10		(0x12F9u)	/* EPFR36_SCK1E�̃Z�b�g�l10 */
#define EPFR41_BIN1E_HIGH	(0x2002u)	/* EPFR41_BIN1E�̃Z�b�g�lHIGH */

/* <P064> */
#define EPFR21_PWM1P1E_LOW	(0x40EFu)	/* EPFR21_PWM1P1E�̃Z�b�g�lLOW */
#define EPFR21_PWM1P1E_HIGH	(0x2010u)	/* EPFR21_PWM1P1E�̃Z�b�g�lHIGH */
#define EPFR35_SIN0E_HIGH	(0x2001u)	/* EPFR35_SIN0E�̃Z�b�g�lHIGH */
#define EPFR41_AIN1E_HIGH	(0x2001u)	/* EPFR41_AIN1E�̃Z�b�g�lHIGH */

/* <P65> */
#define EPFR21_PWM1M1E_LOW	(0x40DFu)	/* EPFR21_PWM1M1E�̃Z�b�g�lLOW */
#define EPFR21_PWM1M1E_HIGH	(0x2020u)	/* EPFR21_PWM1M1E�̃Z�b�g�lHIGH */
#define EPFR35_SOT0E_0X		(0x40EFu)	/* EPFR35_SOT0E�̃Z�b�g�l0X */
#define EPFR35_SOT0E_10		(0x14E7u)	/* EPFR35_SOT0E�̃Z�b�g�l10 */
#define EPFR40_ZIN0E_01		(0x14CFu)	/* EPFR40_ZIN0E�̃Z�b�g�l01 */

/* <P66> */
#define EPFR21_PWM2P1E_LOW	(0x40BFu)	/* EPFR21_PWM2P1E�̃Z�b�g�lLOW */
#define EPFR21_PWM2P1E_HIGH	(0x2040u)	/* EPFR21_PWM2P1E�̃Z�b�g�lHIGH */
#define EPFR35_SCK0E_0X		(0x40FBu)	/* EPFR35_SCK0E�̃Z�b�g�l0X */
#define EPFR35_SCK0E_10		(0x12F9u)	/* EPFR35_SCK0E�̃Z�b�g�l10 */
#define EPFR40_BIN0E_01		(0x12F3u)	/* EPFR40_BIN0E�̃Z�b�g�l01 */

/* <P67> */
#define EPFR21_PWM2M1E_LOW	(0x407Fu)	/* EPFR21_PWM2M1E�̃Z�b�g�lLOW */
#define EPFR21_PWM2M1E_HIGH	(0x2080u)	/* EPFR21_PWM2M1E�̃Z�b�g�lHIGH */
#define EPFR38_SIN9E_HIGH	(0x2001u)	/* EPFR38_SIN9E�̃Z�b�g�lHIGH */
#define EPFR40_AIN0E_01		(0x10FCu)	/* EPFR40_AIN0E�̃Z�b�g�l01 */

/* <<P07>> */

/* <P70> */
#define EPFR22_PWM1P2E_LOW	(0x40FEu)	/* EPFR22_PWM1P2E�̃Z�b�g�lLOW */
#define EPFR22_PWM1P2E_HIGH	(0x2001u)	/* EPFR22_PWM1P2E�̃Z�b�g�lHIGH */
#define EPFR38_SOT9E_0X		(0x40EFu)	/* EPFR38_SOT9E�̃Z�b�g�l0X */
#define EPFR38_SOT9E_10		(0x14E7u)	/* EPFR38_SOT9E�̃Z�b�g�l10 */

/* <P71> */
#define EPFR22_PWM1M2E_LOW	(0x40FDu)	/* EPFR22_PWM1M2E�̃Z�b�g�lLOW */
#define EPFR22_PWM1M2E_HIGH	(0x2002u)	/* EPFR22_PWM1M2E�̃Z�b�g�lHIGH */
#define EPFR38_SCK9E_0X		(0x40FBu)	/* EPFR38_SCK9E�̃Z�b�g�l0X */
#define EPFR38_SCK9E_10		(0x12F9u)	/* EPFR38_SCK9E�̃Z�b�g�l10 */

/* <P72> */
#define EPFR22_PWM2P2E_LOW	(0x40FBu)	/* EPFR22_PWM2P2E�̃Z�b�g�lLOW */
#define EPFR22_PWM2P2E_HIGH	(0x2004u)	/* EPFR22_PWM2P2E�̃Z�b�g�lHIGH */
#define EPFR37_SIN8E_01		(0x10FCu)	/* EPFR37_SIN8E�̃Z�b�g�l01 */
#define EPFR39_ICU11E_01	(0x163Fu)	/* EPFR39_ICU11E�̃Z�b�g�l01 */

/* <P73> */
#define EPFR22_PWM2M2E_LOW	(0x40F7u)	/* EPFR22_PWM2M2E�̃Z�b�g�lLOW */
#define EPFR22_PWM2M2E_HIGH	(0x2008u)	/* EPFR22_PWM2M2E�̃Z�b�g�lHIGH */
#define EPFR37_SOT8E_X0X	(0x40BFu)	/* EPFR37_SOT8E�̃Z�b�g�lX0X */
#define EPFR37_SOT8E_010	(0x161Fu)	/* EPFR37_SOT8E�̃Z�b�g�l010 */
#define EPFR39_ICU10E_01	(0x14CFu)	/* EPFR39_ICU10E�̃Z�b�g�l01 */

/* <P74> */
#define EPFR15_PPG12E_0X	(0x40BFu)	/* EPFR15_PPG12E�̃Z�b�g�l0X */
#define EPFR15_PPG12E_1X	(0x2040u)	/* EPFR15_PPG12E�̃Z�b�g�l1X */
#define EPFR22_PWM1P3E_LOW	(0x40EFu)	/* EPFR22_PWM1P3E�̃Z�b�g�lLOW */
#define EPFR22_PWM1P3E_HIGH	(0x2010u)	/* EPFR22_PWM1P3E�̃Z�b�g�lHIGH */
#define EPFR37_SCK8E_X0X	(0x40F7u)	/* EPFR37_SCK8E�̃Z�b�g�lX0X */
#define EPFR37_SCK8E_010	(0x13E3u)	/*  EPFR37_SCK8E�̃Z�b�g�l010 */
#define EPFR39_ICU9E_01		(0x12F3u)	/* EPFR39_ICU9E�̃Z�b�g�l01 */

/* <P75> */
#define EPFR22_PWM1M3E_LOW	(0x40DFu)	/* EPFR22_PWM1M3E�̃Z�b�g�lLOW */
#define EPFR22_PWM1M3E_HIGH	(0x2020u)	/* EPFR22_PWM1M3E�̃Z�b�g�lHIGH */
#define EPFR34_SIN7E_HIGH	(0x2001u)	/* EPFR34_SIN7E�̃Z�b�g�lHIGH */
#define EPFR39_ICU8E_01		(0x10FCu)	/* EPFR39_ICU8E�̃Z�b�g�l01 */
#define EPFR45_PPG13E_0X	(0x40FDu)	/* EPFR45_PPG13E�̃Z�b�g�l0X */
#define EPFR45_PPG13E_1X	(0x2002u)	/* EPFR45_PPG13E�̃Z�b�g�l1X */

/* <P76> */
#define EPFR01_ICU7E_01		(0x163Fu)	/* EPFR01_ICU7E�̃Z�b�g�l01 */
#define EPFR22_PWM2P3E_LOW	(0x40BFu)	/* EPFR22_PWM2P3E�̃Z�b�g�lLOW */
#define EPFR22_PWM2P3E_HIGH	(0x2040u)	/* EPFR22_PWM2P3E�̃Z�b�g�lHIGH */
#define EPFR34_SOT7E_0X		(0x40EFu)	/* EPFR34_SOT7E�̃Z�b�g�l0X */
#define EPFR34_SOT7E_1X		(0x2010u)	/* EPFR34_SOT7E�̃Z�b�g�l1X */
#define EPFR45_PPG14E_0X	(0x40F7u)	/* EPFR45_PPG14E�̃Z�b�g�l0X */
#define EPFR45_PPG14E_1X	(0x2008u)	/* EPFR45_PPG14E�̃Z�b�g�l1X */

/* <P77> */
#define EPFR01_ICU6E_01		(0x14CFu)	/* EPFR01_ICU6E�̃Z�b�g�l01 */
#define EPFR22_PWM2M3E_LOW	(0x407Fu)	/* EPFR22_PWM2M3E�̃Z�b�g�lLOW */
#define EPFR22_PWM2M3E_HIGH	(0x2080u)	/* EPFR22_PWM2M3E�̃Z�b�g�lHIGH */
#define EPFR34_SCK7E_0X		(0x40FBu)	/* EPFR34_SCK7E�̃Z�b�g�l0X */
#define EPFR34_SCK7E_10		(0x12F9u)	/* EPFR34_SCK7E�̃Z�b�g�l10 */
#define EPFR45_PPG15E_0X	(0x40DFu)	/* EPFR45_PPG15E�̃Z�b�g�l0X */
#define EPFR45_PPG15E_1X	(0x2020u)	/* EPFR45_PPG15E�̃Z�b�g�l1X */

/* <P08>> */

/* <P80> */
#define EPFR23_PWM1P4E_LOW	(0x40FEu)	/* EPFR23_PWM1P4E�̃Z�b�g�lLOW */
#define EPFR23_PWM1P4E_HIGH	(0x2001u)	/* EPFR23_PWM1P4E�̃Z�b�g�lHIGH */
#define EPFR33_SIN6E_LOW	(0x40FEu)	/* EPFR33_SIN6E�̃Z�b�g�lLOW */
#define EPFR40_AIN0E_10		(0x11FCu)	/* EPFR40_AIN0E�̃Z�b�g�l10 */
#define EPFR45_PPG16E_LOW	(0x40BFu)	/* EPFR45_PPG16E�̃Z�b�g�lLOW */
#define EPFR45_PPG16E_HIGH	(0x2040u)	/* EPFR45_PPG16E�̃Z�b�g�lHIGH */

/* <P81> */
#define EPFR23_PWM1M4E_LOW	(0x40FDu)	/* EPFR23_PWM1M4E�̃Z�b�g�lLOW */
#define EPFR23_PWM1M4E_HIGH	(0x2002u)	/* EPFR23_PWM1M4E�̃Z�b�g�lHIGH */
#define EPFR33_SOT6E_X0		(0x40F7u)	/* EPFR33_SOT6E�̃Z�b�g�lX0 */
#define EPFR33_SOT6E_X1		(0x2008u)	/* EPFR33_SOT6E�̃Z�b�g�lX1 */
#define EPFR40_BIN0E_10		(0x13F3u)	/* EPFR40_BIN0E�̃Z�b�g�l10 */
#define EPFR45_PPG17E_LOW	(0x407Fu)	/* EPFR45_PPG17E�̃Z�b�g�lLOW */
#define EPFR45_PPG17E_HIGH	(0x2080u)	/* EPFR45_PPG17E�̃Z�b�g�lHIGH */

/* <P82> */
#define EPFR23_PWM2P4E_LOW	(0x40FBu)	/* EPFR23_PWM2P4E�̃Z�b�g�lLOW */
#define EPFR23_PWM2P4E_HIGH	(0x2004u)	/* EPFR23_PWM2P4E�̃Z�b�g�lHIGH */
#define EPFR33_SCK6E_X0		(0x40FDu)	/* EPFR33_SCK6E�̃Z�b�g�lX0 */
#define EPFR33_SCK6E_01		(0x11F9u)	/* EPFR33_SCK6E�̃Z�b�g�l01 */
#define EPFR40_ZIN0E_10		(0x15CFu)	/* EPFR40_ZIN0E�̃Z�b�g�l10 */
#define EPFR46_PPG18E_LOW	(0x40FEu)	/* EPFR46_PPG18E�̃Z�b�g�lLOW */
#define EPFR46_PPG18E_HIGH	(0x2001u)	/* EPFR46_PPG18E�̃Z�b�g�lHIGH */

/* <P83> */
#define EPFR00_ICU0E_10		(0x11FCu)	/* EPFR00_ICU0E�̃Z�b�g�l10 */
#define EPFR23_PWM2M4E_LOW	(0x40F7u)	/* EPFR23_PWM2M4E�̃Z�b�g�lLOW */
#define EPFR23_PWM2M4E_HIGH	(0x2008u)	/* EPFR23_PWM2M4E�̃Z�b�g�lHIGH */
#define EPFR46_PPG19E_LOW	(0x40FDu)	/* EPFR46_PPG19E�̃Z�b�g�lLOW */
#define EPFR46_PPG19E_HIGH	(0x2002u)	/* EPFR46_PPG19E�̃Z�b�g�lHIGH */

/* <P84> */
#define EPFR00_ICU1E_10		(0x13F3u)	/* EPFR00_ICU1E�̃Z�b�g�l10 */
#define EPFR23_PWM1P5E_LOW	(0x40EFu)	/* EPFR23_PWM1P5E�̃Z�b�g�lLOW */
#define EPFR23_PWM1P5E_HIGH	(0x2010u)	/* EPFR23_PWM1P5E�̃Z�b�g�lHIGH */
#define EPFR46_PPG20E_LOW	(0x40FBu)	/* EPFR46_PPG20E�̃Z�b�g�lLOW */
#define EPFR46_PPG20E_HIGH	(0x2004u)	/* EPFR46_PPG20E�̃Z�b�g�lHIGH */

/* <P85> */
#define EPFR00_ICU2E_10		(0x15CFu)	/* EPFR00_ICU2E�̃Z�b�g�l10 */
#define EPFR23_PWM1M5E_LOW	(0x40DFu)	/* EPFR23_PWM1M5E�̃Z�b�g�lLOW */
#define EPFR23_PWM1M5E_HIGH	(0x2020u)	/* EPFR23_PWM1M5E�̃Z�b�g�lHIGH */
#define EPFR46_PPG21E_LOW	(0x40F7u)	/* EPFR46_PPG21E�̃Z�b�g�lLOW */
#define EPFR46_PPG21E_HIGH	(0x2008u)	/* EPFR46_PPG21E�̃Z�b�g�lHIGH */

/* <P86> */
#define EPFR00_ICU3E_10		(0x173Fu)	/* EPFR00_ICU3E�̃Z�b�g�l10 */
#define EPFR23_PWM2P5E_LOW	(0x40BFu)	/* EPFR23_PWM2P5E�̃Z�b�g�lLOW */
#define EPFR23_PWM2P5E_HIGH	(0x2040u)	/* EPFR23_PWM2P5E�̃Z�b�g�lHIGH */
#define EPFR46_PPG22E_LOW	(0x40EFu)	/* EPFR46_PPG22E�̃Z�b�g�lLOW */
#define EPFR46_PPG22E_HIGH	(0x2010u)	/* EPFR46_PPG22E�̃Z�b�g�lHIGH */

/* <P87> */
#define EPFR01_ICU4E_10		(0x11FCu)	/* EPFR01_ICU4E�̃Z�b�g�l10 */
#define EPFR23_PWM2M5E_LOW	(0x407Fu)	/* EPFR23_PWM2M5E�̃Z�b�g�lLOW */
#define EPFR23_PWM2M5E_HIGH	(0x2080u)	/* EPFR23_PWM2M5E�̃Z�b�g�lHIGH */
#define EPFR46_PPG23E_LOW	(0x40DFu)	/* EPFR46_PPG23E�̃Z�b�g�lLOW */
#define EPFR46_PPG23E_HIGH	(0x2020u)	/* EPFR46_PPG23E�̃Z�b�g�lHIGH */

/* <<P09>> */

/* <P90> */
#define EPFR10_PPG0E_0XX	(0x40FBu)	/* EPFR10_PPG0E�̃Z�b�g�l0XX */
#define EPFR10_PPG0E_1XX	(0x2004u)	/* EPFR10_PPG0E�̃Z�b�g�l1XX */

/* <P91> */
#define EPFR00_ICU2E_01		(0x14CFu)	/* EPFR00_ICU2E�̃Z�b�g�l01 */
#define EPFR04_TOT2E_X0X	(0x40F7u)	/* EPFR04_TOT2E�̃Z�b�g�lX0X */
#define EPFR04_TOT2E_X1X	(0x2008u)	/* EPFR04_TOT2E�̃Z�b�g�lX1X */
#define EPFR06_SIN2E_LOW	(0x40FEu)	/* EPFR06_SIN2E�̃Z�b�g�lLOW */
#define EPFR13_PPG6E_X0X	(0x40FDu)	/* EPFR13_PPG6E�̃Z�b�g�lX0X */
#define EPFR13_PPG6E_X1X	(0x2002u)	/* EPFR13_PPG6E�̃Z�b�g�lX1X */
#define EPFR27_SGA0E_LOW	(0x40FEu)	/* EPFR27_SGA0E�̃Z�b�g�lLOW */
#define EPFR27_SGA0E_HIGH	(0x2001u)	/* EPFR27_SGA0E�̃Z�b�g�lHIGH */
#define EPFR44_INT12E_LOW	(0x40EFu)	/* EPFR44_INT12E�̃Z�b�g�lLOW */

/* <P92> */
#define EPFR00_ICU0E_01		(0x10FCu)	/* EPFR00_ICU0E�̃Z�b�g�l01 */
#define EPFR05_TOT3E_X0X	(0x40F7u)	/* EPFR05_TOT3E�̃Z�b�g�lX0X */
#define EPFR05_TOT3E_X1X	(0x2008u)	/* EPFR05_TOT3E�̃Z�b�g�lX1X */
#define EPFR06_SCK2E_X0		(0x40FDu)	/* EPFR06_SCK2E�̃Z�b�g�lX0 */
#define EPFR06_SCK2E_01		(0x11F9u)	/* EPFR06_SCK2E�̃Z�b�g�l01 */
#define EPFR13_PPG7E_X0X	(0x40EFu)	/* EPFR13_PPG7E�̃Z�b�g�lX0X */
#define EPFR13_PPG7E_X1X	(0x2010u)	/* EPFR13_PPG7E�̃Z�b�g�lX1X */
#define EPFR27_SGO0E_LOW	(0x40FDu)	/* EPFR27_SGO0E�̃Z�b�g�lLOW */
#define EPFR27_SGO0E_HIGH	(0x2002u)	/* EPFR27_SGO0E�̃Z�b�g�lHIGH */
#define EPFR44_INT13E_LOW	(0x40DFu)	/* EPFR44_INT13E�̃Z�b�g�lLOW */

/* <P93> */
#define EPFR00_ICU3E_01		(0x163Fu)	/* EPFR00_ICU3E�̃Z�b�g�l01 */
#define EPFR06_SOT2E_X0		(0x40F7u)	/* EPFR06_SOT2E�̃Z�b�g�lX0 */
#define EPFR06_SOT2E_X1		(0x2008u)	/* EPFR06_SOT2E�̃Z�b�g�lX1 */
#define EPFR14_PPG8E_X0X	(0x40FDu)	/* EPFR14_PPG8E�̃Z�b�g�lX0X */
#define EPFR14_PPG8E_X1X	(0x2002u)	/* EPFR14_PPG8E�̃Z�b�g�lX1X */
#define EPFR27_SGA1E_LOW	(0x40FBu)	/* EPFR27_SGA1E�̃Z�b�g�lLOW */
#define EPFR27_SGA1E_HIGH	(0x2004u)	/* EPFR27_SGA1E�̃Z�b�g�lHIGH */
#define EPFR44_INT14E_LOW	(0x40BFu)	/* EPFR44_INT14E�̃Z�b�g�lLOW */

/* <P94> */
#define EPFR00_ICU1E_01		(0x12F3u)	/* EPFR00_ICU1E�̃Z�b�g�l01 */
#define EPFR07_SIN3E_LOW	(0x40FEu)	/* EPFR07_SIN3E�̃Z�b�g�lLOW */
#define EPFR14_PPG9E_X0X	(0x40EFu)	/* EPFR14_PPG9E�̃Z�b�g�lX0X */
#define EPFR14_PPG9E_X1X	(0x2010u)	/* EPFR14_PPG9E�̃Z�b�g�lX1X */
#define EPFR27_SGO1E_LOW	(0x40F7u)	/* EPFR27_SGO1E�̃Z�b�g�lLOW */
#define EPFR27_SGO1E_HIGH	(0x2008u)	/* EPFR27_SGO1E�̃Z�b�g�lHIGH */
#define EPFR44_INT15E_LOW	(0x407Fu)	/* EPFR44_INT15E�̃Z�b�g�lLOW */

/* <P95> */
#define EPFR15_PPG10E_X0X	(0x40FDu)	/* EPFR15_PPG10E�̃Z�b�g�lX0X */
#define EPFR15_PPG10E_X1X	(0x2002u)	/* EPFR15_PPG10E�̃Z�b�g�lX1X */
#define EPFR24_TX0E_LOW		(0x40FEu)	/* EPFR24_TX0E�̃Z�b�g�lLOW */
#define EPFR24_TX0E_HIGH	(0x2001u)	/* EPFR24_TX0E�̃Z�b�g�lHIGH */

/* <P96> */
#define EPFR44_INT9E_LOW	(0x40FDu)	/* EPFR44_INT9E�̃Z�b�g�lLOW */

/* <P97> */
#define EPFR01_ICU4E_01		(0x10FCu)	/* EPFR01_ICU4E�̃Z�b�g�l01 */
#define EPFR02_TIN0E_00		(0x40FCu)	/* EPFR02_TIN0E�̃Z�b�g�l00 */
#define EPFR07_SOT3E_X0		(0x40F7u)	/* EPFR07_SOT3E�̃Z�b�g�lX0 */
#define EPFR07_SOT3E_X1		(0x2008u)	/* EPFR07_SOT3E�̃Z�b�g�lX1 */
#define EPFR10_PPG0E_X0X	(0x40FDu)	/* EPFR10_PPG0E�̃Z�b�g�lX0X */
#define EPFR10_PPG0E_X1X	(0x2002u)	/* EPFR10_PPG0E�̃Z�b�g�lX1X */
#define EPFR27_WOTE_LOW		(0x40EFu)	/* EPFR27_WOTE�̃Z�b�g�lLOW */
#define EPFR27_WOTE_HIGH	(0x2010u)	/* EPFR27_WOTE�̃Z�b�g�lHIGH */
#define EPFR44_INT8E_LOW	(0x40FEu)	/* EPFR44_INT8E�̃Z�b�g�lLOW */

/* <<P10>> */

/* <P100> */
#define EPFR02_TIN0E_01		(0x10FCu)	/* EPFR02_TIN0E�̃Z�b�g�l01 */
#define EPFR08_SIN4E_HIGH	(0x2001u)	/* EPFR08_SIN4E�̃Z�b�g�lHIGH */
#define EPFR14_PPG8E_XX0	(0x40FEu)	/* EPFR14_PPG8E�̃Z�b�g�lXX0 */
#define EPFR14_PPG8E_XX1	(0x2001u)	/* EPFR14_PPG8E�̃Z�b�g�lXX1 */

/* <P101> */
#define EPFR03_TIN1E_01		(0x10FCu)	/* EPFR03_TIN1E�̃Z�b�g�l01 */
#define EPFR08_SOT4E_0X		(0x40EFu)	/* EPFR08_SOT4E�̃Z�b�g�l0X */
#define EPFR08_SOT4E_1X		(0x2010u)	/* EPFR08_SOT4E�̃Z�b�g�l1X */
#define EPFR14_PPG9E_XX0	(0x40F7u)	/* EPFR14_PPG9E�̃Z�b�g�lXX0 */
#define EPFR14_PPG9E_XX1	(0x2008u)	/* EPFR14_PPG9E�̃Z�b�g�lXX1 */

/* <P102> */
#define EPFR01_ICU6E_10		(0x15CFu)	/* EPFR01_ICU6E�̃Z�b�g�l10 */
#define EPFR04_TIN2E_01		(0x10FCu)	/* EPFR04_TIN2E�̃Z�b�g�l01 */
#define EPFR08_SCK4E_0X		(0x40FBu)	/* EPFR08_SCK4E�̃Z�b�g�l0X */
#define EPFR08_SCK4E_10		(0x12F9u)	/* EPFR08_SCK4E�̃Z�b�g�l10 */
#define EPFR15_PPG10E_XX0	(0x40FEu)	/* EPFR15_PPG10E�̃Z�b�g�lXX0 */
#define EPFR15_PPG10E_XX1	(0x2001u)	/* EPFR15_PPG10E�̃Z�b�g�lXX1 */

/* <P103> */
#define EPFR01_ICU7E_10		(0x173Fu)	/* EPFR01_ICU7E�̃Z�b�g�l10 */
#define EPFR05_TIN3E_01		(0x10FCu)	/* EPFR05_TIN3E�̃Z�b�g�l01 */
#define EPFR09_SIN5E_HIGH	(0x2001u)	/* EPFR09_SIN5E�̃Z�b�g�lHIGH */
#define EPFR10_PPG1E_XX0X	(0x40EFu)	/* EPFR10_PPG1E�̃Z�b�g�lXX0X */
#define EPFR10_PPG1E_XX1X	(0x2010u)	/* EPFR10_PPG1E�̃Z�b�g�lXX1X */

/* <P104> */
#define EPFR02_TOT0E_X0X	(0x40F7u)	/* EPFR02_TOT0E�̃Z�b�g�lX0X */
#define EPFR02_TOT0E_X1X	(0x2008u)	/* EPFR02_TOT0E�̃Z�b�g�lX1X */
#define EPFR09_SOT5E_0X		(0x40EFu)	/* EPFR09_SOT5E�̃Z�b�g�l0X */
#define EPFR09_SOT5E_1X		(0x2010u)	/* EPFR09_SOT5E�̃Z�b�g�l1X */
#define EPFR11_PPG2E_X0X	(0x40FDu)	/* EPFR11_PPG2E�̃Z�b�g�lX0X */
#define EPFR11_PPG2E_X1X	(0x2002u)	/* EPFR11_PPG2E�̃Z�b�g�lX1X */
#define EPFR39_ICU8E_10		(0x11FCu)	/* EPFR39_ICU8E�̃Z�b�g�l10 */

/* <P105> */
#define EPFR03_TOT1E_X0X	(0x40F7u)	/* EPFR03_TOT1E�̃Z�b�g�lX0X */
#define EPFR03_TOT1E_X1X	(0x2008u)	/* EPFR03_TOT1E�̃Z�b�g�lX1X */
#define EPFR09_SCK5E_0X		(0x40FBu)	/* EPFR09_SCK5E�̃Z�b�g�l0X */
#define EPFR09_SCK5E_10		(0x12F9u)	/* EPFR09_SCK5E�̃Z�b�g�l10 */
#define EPFR11_PPG3E_X0X	(0x40EFu)	/* EPFR11_PPG3E�̃Z�b�g�lX0X */
#define EPFR11_PPG3E_X1X	(0x2010u)	/* EPFR11_PPG3E�̃Z�b�g�lX1X */
#define EPFR39_ICU9E_10		(0x13F3u)	/* EPFR39_ICU9E�̃Z�b�g�l10 */

/* <P106> */
#define EPFR12_PPG4E_X0X	(0x40FDu)	/* EPFR12_PPG4E�̃Z�b�g�lX0X */
#define EPFR12_PPG4E_X1X	(0x2002u)	/* EPFR12_PPG4E�̃Z�b�g�lX1X */
#define EPFR30_SGA4E_0X		(0x40DFu)	/* EPFR30_SGA4E�̃Z�b�g�l0X */
#define EPFR30_SGA4E_10		(0x15CFu)	/* EPFR30_SGA4E�̃Z�b�g�l10 */
#define EPFR39_ICU10E_10	(0x15CFu)	/* EPFR39_ICU10E�̃Z�b�g�l10 */

/* <P107> */
#define EPFR12_PPG5E_X0X	(0x40EFu)	/* EPFR12_PPG5E�̃Z�b�g�lX0X */
#define EPFR12_PPG5E_X1X	(0x2010u)	/* EPFR12_PPG5E�̃Z�b�g�lX1X */
#define EPFR30_SGO4E_0X		(0x407Fu)	/* EPFR30_SGO4E�̃Z�b�g�l0X */
#define EPFR30_SGO4E_10		(0x173Fu)	/* EPFR30_SGO4E�̃Z�b�g�l10 */
#define EPFR39_ICU11E_10	(0x173Fu)	/* EPFR39_ICU11E�̃Z�b�g�l10 */
#define EPFR42_DAS1_LOW		(0x40FDu)	/* EPFR42_DAS1�̃Z�b�g�lLOW */
#define EPFR42_DAS1_HIGH	(0x2002u)	/* EPFR42_DAS1�̃Z�b�g�lHIGH */

/* <<P11>> */

/* <P110> */
#define EPFR10_PPG1E_X0XX	(0x40DFu)	/* EPFR10_PPG1E�̃Z�b�g�lX0XX */
#define EPFR10_PPG1E_X1XX	(0x2020u)	/* EPFR10_PPG1E�̃Z�b�g�lX1XX */
#define EPFR24_TX1E_LOW		(0x40FDu)	/* EPFR24_TX1E�̃Z�b�g�lLOW */
#define EPFR24_TX1E_HIGH	(0x2002u)	/* EPFR24_TX1E�̃Z�b�g�lHIGH */

/* <P111> */
#define EPFR11_PPG2E_0XX	(0x40FBu)	/* EPFR11_PPG2E�̃Z�b�g�l0XX */
#define EPFR11_PPG2E_1XX	(0x2004u)	/* EPFR11_PPG2E�̃Z�b�g�l1XX */
#define EPFR44_INT10E_LOW	(0x40FBu)	/* EPFR44_INT10E�̃Z�b�g�lLOW */

/* <P112> */
#define EPFR11_PPG3E_0XX	(0x40DFu)	/* EPFR11_PPG3E�̃Z�b�g�l0XX */
#define EPFR11_PPG3E_1XX	(0x2020u)	/* EPFR11_PPG3E�̃Z�b�g�l1XX */
#define EPFR24_TX2E_LOW		(0x40FBu)	/* EPFR24_TX2E�̃Z�b�g�lLOW */
#define EPFR24_TX2E_HIGH	(0x2004u)	/* EPFR24_TX2E�̃Z�b�g�lHIGH */

/* <P113> */
#define EPFR12_PPG4E_0XX	(0x40FBu)	/* EPFR12_PPG4E�̃Z�b�g�l0XX */
#define EPFR12_PPG4E_1XX	(0x2004u)	/* EPFR12_PPG4E�̃Z�b�g�l1XX */
#define EPFR44_INT11E_LOW	(0x40F7u)	/* EPFR44_INT11E�̃Z�b�g�lLOW */

/* <P114> */
#define EPFR01_ICU5E_01		(0x12F3u)	/* EPFR01_ICU5E�̃Z�b�g�l01 */
#define EPFR03_TIN1E_00		(0x40FCu)	/* EPFR03_TIN1E�̃Z�b�g�l00 */
#define EPFR07_SCK3E_X0		(0x40FDu)	/* EPFR07_SCK3E�̃Z�b�g�lX0 */
#define EPFR07_SCK3E_01		(0x11F9u)	/* EPFR07_SCK3E�̃Z�b�g�l01 */
#define EPFR30_SGA2E_LOW	(0x40FEu)	/* EPFR30_SGA2E�̃Z�b�g�lLOW */
#define EPFR30_SGA2E_HIGH	(0x2001u)	/* EPFR30_SGA2E�̃Z�b�g�lHIGH */

/* <P115> */
#define EPFR04_TIN2E_00		(0x40FCu)	/* EPFR04_TIN2E�̃Z�b�g�l00 */
#define EPFR08_SIN4E_LOW	(0x40FEu)	/* EPFR08_SIN4E�̃Z�b�g�lLOW */
#define EPFR30_SGO2E_LOW	(0x40FDu)	/* EPFR30_SGO2E�̃Z�b�g�lLOW */
#define EPFR30_SGO2E_HIGH	(0x2002u)	/* EPFR30_SGO2E�̃Z�b�g�lHIGH */

/* <P116> */
#define EPFR05_TIN3E_00		(0x40FCu)	/* EPFR05_TIN3E�̃Z�b�g�l00 */
#define EPFR08_SOT4E_X0		(0x40F7u)	/* EPFR08_SOT4E�̃Z�b�g�lX0 */
#define EPFR08_SOT4E_X1		(0x2008u)	/* EPFR08_SOT4E�̃Z�b�g�lX1 */
#define EPFR28_FRCK3E_LOW	(0x40F7u)	/* EPFR28_FRCK3E�̃Z�b�g�lLOW */
#define EPFR30_SGA3E_LOW	(0x40FBu)	/* EPFR30_SGA3E�̃Z�b�g�lLOW */
#define EPFR30_SGA3E_HIGH	(0x2004u)	/* EPFR30_SGA3E�̃Z�b�g�lHIGH */

/* <P117> */
#define EPFR02_TOT0E_XX0	(0x40FBu)	/* EPFR02_TOT0E�̃Z�b�g�lXX0 */
#define EPFR02_TOT0E_XX1	(0x2004u)	/* EPFR02_TOT0E�̃Z�b�g�lXX1 */
#define EPFR08_SCK4E_X0		(0x40FDu)	/* EPFR08_SCK4E�̃Z�b�g�lX0 */
#define EPFR08_SCK4E_01		(0x11F9u)	/* EPFR08_SCK4E�̃Z�b�g�l01 */
#define EPFR28_FRCK2E_LOW	(0x40FBu)	/* EPFR28_FRCK2E�̃Z�b�g�lLOW */
#define EPFR30_SGO3E_LOW	(0x40F7u)	/* EPFR30_SGO3E�̃Z�b�g�lLOW */
#define EPFR30_SGO3E_HIGH	(0x2008u)	/* EPFR30_SGO3E�̃Z�b�g�lHIGH */

/* <<P12>> */

/* <P120> */
#define EPFR03_TOT1E_XX0	(0x40FBu)	/* EPFR03_TOT1E�̃Z�b�g�lXX0 */
#define EPFR03_TOT1E_XX1	(0x2004u)	/* EPFR03_TOT1E�̃Z�b�g�lXX1 */
#define EPFR09_SIN5E_LOW	(0x40FEu)	/* EPFR09_SIN5E�̃Z�b�g�lLOW */
#define EPFR12_PPG5E_0XX	(0x40DFu)	/* EPFR12_PPG5E�̃Z�b�g�l0XX */
#define EPFR12_PPG5E_1XX	(0x2020u)	/* EPFR12_PPG5E�̃Z�b�g�l1XX */
#define EPFR28_FRCK1E_LOW	(0x40FDu)	/* EPFR28_FRCK1E�̃Z�b�g�lLOW */
#define EPFR43_INT6E_LOW	(0x40BFu)	/* EPFR43_INT6E�̃Z�b�g�lLOW */

/* <P121> */
#define EPFR04_TOT2E_XX0	(0x40FBu)	/* EPFR04_TOT2E�̃Z�b�g�lXX0 */
#define EPFR04_TOT2E_XX1	(0x2004u)	/* EPFR04_TOT2E�̃Z�b�g�lXX1 */
#define EPFR09_SOT5E_X0		(0x40F7u)	/* EPFR09_SOT5E�̃Z�b�g�lX0 */
#define EPFR09_SOT5E_X1		(0x2008u)	/* EPFR09_SOT5E�̃Z�b�g�lX1 */
#define EPFR13_PPG6E_0XX	(0x40FBu)	/* EPFR13_PPG6E�̃Z�b�g�l0XX */
#define EPFR13_PPG6E_1XX	(0x2004u)	/* EPFR13_PPG6E�̃Z�b�g�l1XX */
#define EPFR28_FRCK0E_LOW	(0x40FEu)	/* EPFR28_FRCK0E�̃Z�b�g�lLOW */
#define EPFR43_INT7E_LOW	(0x407Fu)	/* EPFR43_INT7E�̃Z�b�g�lLOW */

/* <P122> */
#define EPFR05_TOT3E_XX0	(0x40FBu)	/* EPFR05_TOT3E�̃Z�b�g�lXX0 */
#define EPFR05_TOT3E_XX1	(0x2004u)	/* EPFR05_TOT3E�̃Z�b�g�lXX1 */
#define EPFR09_SCK5E_X0		(0x40FDu)	/* EPFR09_SCK5E�̃Z�b�g�lX0 */
#define EPFR09_SCK5E_01		(0x11F9u)	/* EPFR09_SCK5E�̃Z�b�g�l01 */
#define EPFR13_PPG7E_0XX	(0x40DFu)	/* EPFR13_PPG7E�̃Z�b�g�l0XX */
#define EPFR13_PPG7E_1XX	(0x2020u)	/* EPFR13_PPG7E�̃Z�b�g�l1XX */
#define EPFR29_OCU0E_X0		(0x40FBu)	/* EPFR29_OCU0E�̃Z�b�g�lX0 */
#define EPFR29_OCU0E_01		(0x10FCu)	/* EPFR29_OCU0E�̃Z�b�g�l01 */

/* <P123> */
#define EPFR14_PPG8E_0XX	(0x40FBu)	/* EPFR14_PPG8E�̃Z�b�g�l0XX */
#define EPFR14_PPG8E_1XX	(0x2004u)	/* EPFR14_PPG8E�̃Z�b�g�l1XX */
#define EPFR29_OCU1E_X0		(0x40FBu)	/* EPFR29_OCU1E�̃Z�b�g�lX0 */
#define EPFR29_OCU1E_01		(0x12F3u)	/* EPFR29_OCU1E�̃Z�b�g�l01 */
#define EPFR42_DAS0_LOW		(0x40FEu)	/* EPFR42_DAS0�̃Z�b�g�lLOW */
#define EPFR42_DAS0_HIGH	(0x2001u)	/* EPFR42_DAS0�̃Z�b�g�lHIGH */

/* <P124> */
#define EPFR01_ICU5E_10		(0x13F3u)	/* EPFR01_ICU5E�̃Z�b�g�l10 */
#define EPFR14_PPG9E_0XX	(0x40DFu)	/* EPFR14_PPG9E�̃Z�b�g�l0XX */
#define EPFR14_PPG9E_1XX	(0x2020u)	/* EPFR14_PPG9E�̃Z�b�g�l1XX */
#define EPFR29_OCU2E_X0		(0x40EFu)	/* EPFR31_OCU5E�̃Z�b�g�lX0 */
#define EPFR29_OCU2E_01		(0x14CFu)	/* EPFR31_OCU5E�̃Z�b�g�l01 */

/* <P125> */
#define EPFR00_ICU0E_00		(0x40FCu)	/* EPFR00_ICU0E�̃Z�b�g�l00 */
#define EPFR15_PPG10E_0XX	(0x40FBu)	/* EPFR15_PPG10E�̃Z�b�g�l0XX */
#define EPFR15_PPG10E_1XX	(0x2004u)	/* EPFR15_PPG10E�̃Z�b�g�l1XX */
#define EPFR29_OCU3E_X0		(0x40BFu)	/* EPFR29_OCU3E�̃Z�b�g�lX0 */
#define EPFR29_OCU3E_01		(0x163Fu)	/* EPFR29_OCU3E�̃Z�b�g�l01 */

/* <P126> */
#define EPFR31_OCU4E_X0		(0x40FEu)	/* EPFR31_OCU4E�̃Z�b�g�lX0 */
#define EPFR31_OCU4E_01		(0x10FCu)	/* EPFR31_OCU4E�̃Z�b�g�l01 */
#define EPFR35_SIN0E_LOW	(0x40FEu)	/* EPFR35_SIN0E�̃Z�b�g�lLOW */
#define EPFR43_INT1E_LOW	(0x40FDu)	/* EPFR43_INT1E�̃Z�b�g�lLOW */

/* <P127> */
#define EPFR31_OCU5E_X0		(0x40FBu)	/* EPFR31_OCU5E�̃Z�b�g�lX0 */
#define EPFR31_OCU5E_01		(0x12F3u)	/* EPFR31_OCU5E�̃Z�b�g�l01 */
#define EPFR35_SOT0E_X0		(0x40E7u)	/* EPFR35_SOT0E�̃Z�b�g�lX0 */
#define EPFR35_SOT0E_01		(0x13E7u)	/* EPFR35_SOT0E�̃Z�b�g�l01 */

/* <<P13>> */

/* <P130> */
#define EPFR00_ICU1E_00		(0x40FCu)	/* EPFR00_ICU1E�̃Z�b�g�l00 */
#define EPFR26_TIA0E_LOW	(0x40FEu)	/* EPFR26_TIA0E�̃Z�b�g�lLOW */
#define EPFR26_TIA0E_HIGH	(0x2001u)	/* EPFR26_TIA0E�̃Z�b�g�lHIGH */
#define EPFR35_SCK0E_X0		(0x40FDu)	/* EPFR35_SCK0E�̃Z�b�g�lX0 */
#define EPFR35_SCK0E_01		(0x11F9u)	/* EPFR35_SCK0E�̃Z�b�g�l01 */
#define EPFR43_INT0E_LOW	(0x40FEu)	/* EPFR43_INT0E�̃Z�b�g�lLOW */

/* <P131> */
#define EPFR00_ICU2E_00		(0x40CFu)	/* EPFR00_ICU2E�̃Z�b�g�l00 */
#define EPFR26_TIA1E_LOW	(0x40FDu)	/* EPFR26_TIA1E�̃Z�b�g�lLOW */
#define EPFR26_TIA1E_HIGH	(0x2002u)	/* EPFR26_TIA1E�̃Z�b�g�lHIGH */
#define EPFR36_SIN1E_LOW	(0x40FEu)	/* EPFR36_SIN1E�̃Z�b�g�lLOW */
#define EPFR43_INT4E_LOW	(0x40EFu)	/* EPFR43_INT4E�̃Z�b�g�lLOW */

/* <P132> */
#define EPFR00_ICU3E_00		(0x403Fu)	/* EPFR00_ICU3E�̃Z�b�g�l00 */
#define EPFR26_TIB0E_LOW	(0x40FBu)	/* EPFR26_TIB0E�̃Z�b�g�lLOW */
#define EPFR26_TIB0E_HIGH	(0x2004u)	/* EPFR26_TIB0E�̃Z�b�g�lHIGH */
#define EPFR36_SOT1E_X0		(0x40F7u)	/* EPFR36_SOT1E�̃Z�b�g�lX0 */
#define EPFR36_SOT1E_01		(0x13E7u)	/* EPFR36_SOT1E�̃Z�b�g�l01 */
#define EPFR43_INT2E_LOW	(0x40FBu)	/* EPFR43_INT2E�̃Z�b�g�lLOW */

/* <P133> */
#define EPFR01_ICU4E_00		(0x40FCu)	/* EPFR01_ICU4E�̃Z�b�g�l00 */
#define EPFR15_PPG11E_0X	(0x40EFu)	/* EPFR15_PPG11E�̃Z�b�g�l0X */
#define EPFR15_PPG11E_1X	(0x2010u)	/* EPFR15_PPG11E�̃Z�b�g�l1X */
#define EPFR26_TIB1E_LOW	(0x40F7u)	/* EPFR26_TIB1E�̃Z�b�g�lLOW */
#define EPFR26_TIB1E_HIGH	(0x2008u)	/* EPFR26_TIB1E�̃Z�b�g�lHIGH */
#define EPFR36_SCK1E_X0		(0x40FDu)	/* EPFR36_SCK1E�̃Z�b�g�lX0 */
#define EPFR36_SCK1E_01		(0x11F9u)	/* EPFR36_SCK1E�̃Z�b�g�l01 */
#define EPFR43_INT3E_LOW	(0x40F7u)	/* EPFR43_INT3E�̃Z�b�g�lLOW */

/* <P134> */
#define EPFR01_ICU5E_00		(0x40F3u)	/* EPFR01_ICU5E�̃Z�b�g�l00 */
#define EPFR10_PPG1E_0XXX	(0x40BFu)	/* EPFR10_PPG1E�̃Z�b�g�l0XXX */
#define EPFR10_PPG1E_1XXX	(0x2040u)	/* EPFR10_PPG1E�̃Z�b�g�l1XXX */
#define EPFR43_INT5E_LOW	(0x40DFu)	/* EPFR43_INT5E�̃Z�b�g�lLOW */

/* <P135> */
/* �Ȃ� */

/* <P136> */
/* �Ȃ� */

/* <P137> */
/* �Ȃ� */

/* �yenum��`�z */
	
/* �r�b�g�ԍ� */
enum {
	BIT_NUM_0 = 0,			/* BIT�ԍ�0 */
	BIT_NUM_1,				/* BIT�ԍ�1 */
	BIT_NUM_2,				/* BIT�ԍ�2 */
	BIT_NUM_3,				/* BIT�ԍ�3 */
	BIT_NUM_4,				/* BIT�ԍ�4 */
	BIT_NUM_5,				/* BIT�ԍ�5 */
	BIT_NUM_6,				/* BIT�ԍ�6 */
	BIT_NUM_7,				/* BIT�ԍ�7 */
	BIT_MAX					/* BIT�ԍ��ő吔 */
};

/* �@�\���W�X�^�r�b�g�ԍ� */
enum {
	BIT_FUNC_NUM_8 = 0,		/* BIT�ԍ�8  */
	BIT_FUNC_NUM_9,			/* BIT�ԍ�9  */
	BIT_FUNC_NUM_10,		/* BIT�ԍ�10 */
	BIT_FUNC_NUM_11,		/* BIT�ԍ�11 */
	BIT_FUNC_NUM_12,		/* BIT�ԍ�12 */
	BIT_FUNC_NUM_13,		/* BIT�ԍ�13 */
	BIT_FUNC_NUM_14,		/* BIT�ԍ�14 */
	BIT_FUNC_NUM_15,		/* BIT�ԍ�15 */
	BIT_FUNC_MAX			/* BIT�ԍ��ő吔 */
};
	
/* �|�[�g���W�X�^�� */
enum {
	PORT_NUM_0 = 0,			/* PORT�ԍ�0 */
	PORT_NUM_1,				/* PORT�ԍ�1 */
	PORT_NUM_2,				/* PORT�ԍ�2 */
	PORT_NUM_3,				/* PORT�ԍ�3 */
	PORT_NUM_4,				/* PORT�ԍ�4 */
	PORT_NUM_5,				/* PORT�ԍ�5 */
	PORT_NUM_6,				/* PORT�ԍ�6 */
	PORT_NUM_7,				/* PORT�ԍ�7 */
	PORT_NUM_8,				/* PORT�ԍ�8 */
	PORT_NUM_9,				/* PORT�ԍ�9 */
	PORT_NUM_10,			/* PORT�ԍ�10 */
	PORT_NUM_11,			/* PORT�ԍ�11 */
	PORT_NUM_12,			/* PORT�ԍ�12 */
	PORT_NUM_13,			/* PORT�ԍ�13 */
	PORT_MAX				/* PORT�ԍ��ő吔 */
};

/* �yconst��`�z */

/* �r�b�g�ԍ��e�[�u�� */
const static UI_8 C_PortCheckBitTbl[BIT_MAX] =
{
	0x01u,			/* BIT0 */
	0x02u,			/* BIT1 */
	0x04u,			/* BIT2 */
	0x08u,			/* BIT3 */
	0x10u,			/* BIT4 */
	0x20u,			/* BIT5 */
	0x40u,			/* BIT6 */
	0x80u			/* BIT7 */
};

#if (PORT_DRV_FUNC_CHANGE == 1)
/* �@�\���W�X�^�r�b�g�ԍ��e�[�u�� */
const static UI_16 C_PortFuncCheckBitTbl[BIT_MAX] =
{
	0x0100u,		/* BIT8  */
	0x0200u,		/* BIT9  */
	0x0400u,		/* BIT10 */
	0x0800u,		/* BIT11 */
	0x1000u,		/* BIT12 */
	0x2000u,		/* BIT13 */
	0x4000u,		/* BIT14 */
	0x8000u			/* BIT15 */
};
#endif	/* PORT_DRV_FUNC_CHANGE == 1 */

/* �|�[�g�f�[�^���W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *const C_PortDataTbl[PORTDRV_PORT_MAX] =
{
	&(PDR00),	/* P0�̃|�[�g�f�[�^���W�X�^�A�h���X */
	&(PDR01),	/* P1�̃|�[�g�f�[�^���W�X�^�A�h���X */
	&(PDR02),	/* P2�̃|�[�g�f�[�^���W�X�^�A�h���X */
	&(PDR03),	/* P3�̃|�[�g�f�[�^���W�X�^�A�h���X */
	&(PDR04),	/* P4�̃|�[�g�f�[�^���W�X�^�A�h���X */
	&(PDR05),	/* P5�̃|�[�g�f�[�^���W�X�^�A�h���X */
	&(PDR06),	/* P6�̃|�[�g�f�[�^���W�X�^�A�h���X */
	&(PDR07),	/* P7�̃|�[�g�f�[�^���W�X�^�A�h���X */
	&(PDR08),	/* P8�̃|�[�g�f�[�^���W�X�^�A�h���X */
	&(PDR09),	/* P9�̃|�[�g�f�[�^���W�X�^�A�h���X */
	&(PDR10),	/* P10�̃|�[�g�f�[�^���W�X�^�A�h���X */
	&(PDR11),	/* P11�̃|�[�g�f�[�^���W�X�^�A�h���X */
	&(PDR12),	/* P12�̃|�[�g�f�[�^���W�X�^�A�h���X */
	&(PDR13)	/* P13�̃|�[�g�f�[�^���W�X�^�A�h���X */
};

/* ���̓f�[�^�_�C���N�g���[�h���W�X�^�A�h���X�e�[�u�� */
static __io volatile const UI_8 *const C_PortDataDirectTbl[PORT_MAX] =
{
	&(PDDR00),	/* P0�̓��̓f�[�^�_�C���N�g���[�h���W�X�^ */
	&(PDDR01),	/* P1�̓��̓f�[�^�_�C���N�g���[�h���W�X�^ */
	&(PDDR02),	/* P2�̓��̓f�[�^�_�C���N�g���[�h���W�X�^ */
	&(PDDR03),	/* P3�̓��̓f�[�^�_�C���N�g���[�h���W�X�^ */
	&(PDDR04),	/* P4�̓��̓f�[�^�_�C���N�g���[�h���W�X�^ */
	&(PDDR05),	/* P5�̓��̓f�[�^�_�C���N�g���[�h���W�X�^ */
	&(PDDR06),	/* P6�̓��̓f�[�^�_�C���N�g���[�h���W�X�^ */
	&(PDDR07),	/* P7�̓��̓f�[�^�_�C���N�g���[�h���W�X�^ */
	&(PDDR08),	/* P8�̓��̓f�[�^�_�C���N�g���[�h���W�X�^ */
	&(PDDR09),	/* P9�̓��̓f�[�^�_�C���N�g���[�h���W�X�^ */
	&(PDDR10),	/* P10�̓��̓f�[�^�_�C���N�g���[�h���W�X�^ */
	&(PDDR11),	/* P11�̓��̓f�[�^�_�C���N�g���[�h���W�X�^ */
	&(PDDR12),	/* P12�̓��̓f�[�^�_�C���N�g���[�h���W�X�^ */
	&(PDDR13)	/* P13�̓��̓f�[�^�_�C���N�g���[�h���W�X�^ */
};

/* �f�[�^�������W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *const C_DataDirectionTbl[PORT_MAX] =
{
	&(DDR00),	/* P0�̃f�[�^�������W�X�^�A�h���X */
	&(DDR01),	/* P1�̃f�[�^�������W�X�^�A�h���X */
	&(DDR02),	/* P2�̃f�[�^�������W�X�^�A�h���X */
	&(DDR03),	/* P3�̃f�[�^�������W�X�^�A�h���X */
	&(DDR04),	/* P4�̃f�[�^�������W�X�^�A�h���X */
	&(DDR05),	/* P5�̃f�[�^�������W�X�^�A�h���X */
	&(DDR06),	/* P6�̃f�[�^�������W�X�^�A�h���X */
	&(DDR07),	/* P7�̃f�[�^�������W�X�^�A�h���X */
	&(DDR08),	/* P8�̃f�[�^�������W�X�^�A�h���X */
	&(DDR09),	/* P9�̃f�[�^�������W�X�^�A�h���X */
	&(DDR10),	/* P10�̃f�[�^�������W�X�^�A�h���X */
	&(DDR11),	/* P11�̃f�[�^�������W�X�^�A�h���X */
	&(DDR12),	/* P12�̃f�[�^�������W�X�^�A�h���X */
	&(DDR13)	/* P13�̃f�[�^�������W�X�^�A�h���X */
};

/* �|�[�g�@�\���W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *const C_PortFunctionTbl[PORT_MAX] =
{
	&(PFR00),	/* P0�̃|�[�g�@�\���W�X�^�A�h���X */
	&(PFR01),	/* P1�̃|�[�g�@�\���W�X�^�A�h���X */
	&(PFR02),	/* P2�̃|�[�g�@�\���W�X�^�A�h���X */
	&(PFR03),	/* P3�̃|�[�g�@�\���W�X�^�A�h���X */
	&(PFR04),	/* P4�̃|�[�g�@�\���W�X�^�A�h���X */
	&(PFR05),	/* P5�̃|�[�g�@�\���W�X�^�A�h���X */
	&(PFR06),	/* P6�̃|�[�g�@�\���W�X�^�A�h���X */
	&(PFR07),	/* P7�̃|�[�g�@�\���W�X�^�A�h���X */
	&(PFR08),	/* P8�̃|�[�g�@�\���W�X�^�A�h���X */
	&(PFR09),	/* P9�̃|�[�g�@�\���W�X�^�A�h���X */
	&(PFR10),	/* P10�̃|�[�g�@�\���W�X�^�A�h���X */
	&(PFR11),	/* P11�̃|�[�g�@�\���W�X�^�A�h���X */
	&(PFR12),	/* P12�̃|�[�g�@�\���W�X�^�A�h���X */
	&(PFR13)	/* P13�̃|�[�g�@�\���W�X�^�A�h���X */
};

/* �g���|�[�g�@�\���W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *const C_ExtendedPortFunctionTbl[PORTDRV_EPFR_MAX] =
{
	&(EPFR00),	/* P0�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR01),	/* P1�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR02),	/* P2�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR03),	/* P3�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR04),	/* P4�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR05),	/* P5�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR06),	/* P6�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR07),	/* P7�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR08),	/* P8�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR09),	/* P9�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR10),	/* P10�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR11),	/* P11�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR12),	/* P12�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR13),	/* P13�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR14),	/* P14�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR15),	/* P15�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR16),	/* P16�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR17),	/* P17�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR18),	/* P18�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR19),	/* P19�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR20),	/* P20�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR21),	/* P21�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR22),	/* P22�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR23),	/* P23�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR24),	/* P24�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR25),	/* P25�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR26),	/* P26�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR27),	/* P27�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR28),	/* P28�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR29),	/* P29�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR30),	/* P30�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR31),	/* P31�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR32),	/* P32�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR33),	/* P33�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR34),	/* P34�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR35),	/* P35�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR36),	/* P36�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR37),	/* P37�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR38),	/* P38�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR39),	/* P39�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR40),	/* P40�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR41),	/* P41�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR42),	/* P42�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR43),	/* P43�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR44),	/* P44�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR45),	/* P45�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR46),	/* P46�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR47),	/* P47�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR52),	/* P52�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR53),	/* P53�̊g���|�[�g�@�\���W�X�^�A�h���X */
	&(EPFR54)	/* P54�̊g���|�[�g�@�\���W�X�^�A�h���X */
};

/* �v���A�b�v�_�E�����䃌�W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *const C_PullControlTbl[PORT_MAX] =
{
	&(PPCR00),	/* P0�̃v���A�b�v�_�E�����䃌�W�X�^�A�h���X */
	&(PPCR01),	/* P1�̃v���A�b�v�_�E�����䃌�W�X�^�A�h���X */
	&(PPCR02),	/* P2�̃v���A�b�v�_�E�����䃌�W�X�^�A�h���X */
	&(PPCR03),	/* P3�̃v���A�b�v�_�E�����䃌�W�X�^�A�h���X */
	&(PPCR04),	/* P4�̃v���A�b�v�_�E�����䃌�W�X�^�A�h���X */
	&(PPCR05),	/* P5�̃v���A�b�v�_�E�����䃌�W�X�^�A�h���X */
	&(PPCR06),	/* P6�̃v���A�b�v�_�E�����䃌�W�X�^�A�h���X */
	&(PPCR07),	/* P7�̃v���A�b�v�_�E�����䃌�W�X�^�A�h���X */
	&(PPCR08),	/* P8�̃v���A�b�v�_�E�����䃌�W�X�^�A�h���X */
	&(PPCR09),	/* P9�̃v���A�b�v�_�E�����䃌�W�X�^�A�h���X */
	&(PPCR10),	/* P10�̃v���A�b�v�_�E�����䃌�W�X�^�A�h���X */
	&(PPCR11),	/* P11�̃v���A�b�v�_�E�����䃌�W�X�^�A�h���X */
	&(PPCR12),	/* P12�̃v���A�b�v�_�E�����䃌�W�X�^�A�h���X */
	&(PPCR13)	/* P13�̃v���A�b�v�_�E�����䃌�W�X�^�A�h���X */
};

/* �v���A�b�v�_�E�������W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *const C_PullEnableTbl [PORT_MAX] =
{
	&(PPER00),	/* P0�̃v���A�b�v�_�E�������W�X�^�A�h���X */
	&(PPER01),	/* P1�̃v���A�b�v�_�E�������W�X�^�A�h���X */
	&(PPER02),	/* P2�̃v���A�b�v�_�E�������W�X�^�A�h���X */
	&(PPER03),	/* P3�̃v���A�b�v�_�E�������W�X�^�A�h���X */
	&(PPER04),	/* P4�̃v���A�b�v�_�E�������W�X�^�A�h���X */
	&(PPER05),	/* P5�̃v���A�b�v�_�E�������W�X�^�A�h���X */
	&(PPER06),	/* P6�̃v���A�b�v�_�E�������W�X�^�A�h���X */
	&(PPER07),	/* P7�̃v���A�b�v�_�E�������W�X�^�A�h���X */
	&(PPER08),	/* P8�̃v���A�b�v�_�E�������W�X�^�A�h���X */
	&(PPER09),	/* P9�̃v���A�b�v�_�E�������W�X�^�A�h���X */
	&(PPER10),	/* P10�̃v���A�b�v�_�E�������W�X�^�A�h���X */
	&(PPER11),	/* P11�̃v���A�b�v�_�E�������W�X�^�A�h���X */
	&(PPER12),	/* P12�̃v���A�b�v�_�E�������W�X�^�A�h���X */
	&(PPER13)	/* P13�̃v���A�b�v�_�E�������W�X�^�A�h���X */
};

/* �|�[�g���̓��x���I�����W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *const C_PortInputLevelTbl[PORT_MAX] =
{
	&(PILR00),	/* P0�̃|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(PILR01),	/* P1�̃|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(PILR02),	/* P2�̃|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(PILR03),	/* P3�̃|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(PILR04),	/* P4�̃|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(PILR05),	/* P5�̃|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(PILR06),	/* P6�̃|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(PILR07),	/* P7�̃|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(PILR08),	/* P8�̃|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(PILR09),	/* P9�̃|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(PILR10),	/* P10�̃|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(PILR11),	/* P11�̃|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(PILR12),	/* P12�̃|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(PILR13)	/* P13�̃|�[�g���̓��x���I�����W�X�^�A�h���X */
};

/* �g���|�[�g���̓��x���I�����W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *const C_ExtendedPortInputLevelTbl[PORT_MAX] =
{
	&(EPILR00),	/* P0�̊g���|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(EPILR01),	/* P1�̊g���|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(EPILR02),	/* P2�̊g���|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(EPILR03),	/* P3�̊g���|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(EPILR04),	/* P4�̊g���|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(EPILR05),	/* P5�̊g���|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(EPILR06),	/* P6�̊g���|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(EPILR07),	/* P7�̊g���|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(EPILR08),	/* P8�̊g���|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(EPILR09),	/* P9�̊g���|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(EPILR10),	/* P10�̊g���|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(EPILR11),	/* P11�̊g���|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(EPILR12),	/* P12�̊g���|�[�g���̓��x���I�����W�X�^�A�h���X */
	&(EPILR13)	/* P13�̊g���|�[�g���̓��x���I�����W�X�^�A�h���X */
};

/* �|�[�g�o�͋쓮���W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *const C_PortOutputDriveTbl[PORT_MAX] =
{
	&(PODR00),		/* P0�̃|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(PODR01),		/* P1�̃|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(PODR02),		/* P2�̃|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(PODR03),		/* P3�̃|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(PODR04),		/* P4�̃|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(PODR05),		/* P5�̃|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(PODR06),		/* P6�̃|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(PODR07),		/* P7�̃|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(PODR08),		/* P8�̃|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(PODR09),		/* P9�̃|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(PODR10),		/* P10�̃|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(PODR11),		/* P11�̃|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(PODR12),		/* P12�̃|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(PODR13)		/* P13�̃|�[�g�o�͋쓮���W�X�^�A�h���X */
};

/* �g���|�[�g�o�͋쓮���W�X�^�A�h���X�e�[�u�� */
static __io volatile UI_8 *const C_ExtendedPortOutputDriveTbl[PORT_MAX] =
{
	D_NULL,			/* ���W�X�^�ݒ�Ȃ� */
	&(EPODR01),		/* P1�̊g���|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(EPODR02),		/* P2�̊g���|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(EPODR03),		/* P3�̊g���|�[�g�o�͋쓮���W�X�^�A�h���X */
	D_NULL,			/* ���W�X�^�ݒ�Ȃ� */
	D_NULL,			/* ���W�X�^�ݒ�Ȃ� */
	&(EPODR06),		/* P6�̊g���|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(EPODR07),		/* P7�̊g���|�[�g�o�͋쓮���W�X�^�A�h���X */
	&(EPODR08),		/* P8�̊g���|�[�g�o�͋쓮���W�X�^�A�h���X */
	D_NULL,			/* ���W�X�^�ݒ�Ȃ� */
	D_NULL,			/* ���W�X�^�ݒ�Ȃ� */
	D_NULL,			/* ���W�X�^�ݒ�Ȃ� */
	D_NULL,			/* ���W�X�^�ݒ�Ȃ� */
	D_NULL,			/* ���W�X�^�ݒ�Ȃ� */
};

/* �|�[�g�f�[�^���W�X�^�����l�e�[�u�� */
const static UI_8 C_PortInitTbl[PORTDRV_PORT_MAX] =
{
	INIT_PDR0,	/* P0�̃|�[�g�f�[�^���W�X�^�̏����l */
	INIT_PDR1,	/* P1�̃|�[�g�f�[�^���W�X�^�̏����l */
	INIT_PDR2,	/* P2�̃|�[�g�f�[�^���W�X�^�̏����l */
	INIT_PDR3,	/* P3�̃|�[�g�f�[�^���W�X�^�̏����l */
	INIT_PDR4,	/* P4�̃|�[�g�f�[�^���W�X�^�̏����l */
	INIT_PDR5,	/* P5�̃|�[�g�f�[�^���W�X�^�̏����l */
	INIT_PDR6,	/* P6�̃|�[�g�f�[�^���W�X�^�̏����l */
	INIT_PDR7,	/* P7�̃|�[�g�f�[�^���W�X�^�̏����l */
	INIT_PDR8,	/* P8�̃|�[�g�f�[�^���W�X�^�̏����l */
	INIT_PDR9,	/* P9�̃|�[�g�f�[�^���W�X�^�̏����l */
	INIT_PDR10,	/* P10�̃|�[�g�f�[�^���W�X�^�̏����l */
	INIT_PDR11,	/* P11�̃|�[�g�f�[�^���W�X�^�̏����l */
	INIT_PDR12,	/* P12�̃|�[�g�f�[�^���W�X�^�̏����l */
	INIT_PDR13	/* P13�̃|�[�g�f�[�^���W�X�^�̏����l */
};

/* �|�[�g�f�[�^���W�X�^��~�l�e�[�u�� */
const static UI_8 C_PortStopTbl[PORTDRV_PORT_MAX] =
{
	STOP_PDR0,	/* P0�̃|�[�g�f�[�^���W�X�^�̒�~�l */
	STOP_PDR1,	/* P1�̃|�[�g�f�[�^���W�X�^�̒�~�l */
	STOP_PDR2,	/* P2�̃|�[�g�f�[�^���W�X�^�̒�~�l */
	STOP_PDR3,	/* P3�̃|�[�g�f�[�^���W�X�^�̒�~�l */
	STOP_PDR4,	/* P4�̃|�[�g�f�[�^���W�X�^�̒�~�l */
	STOP_PDR5,	/* P5�̃|�[�g�f�[�^���W�X�^�̒�~�l */
	STOP_PDR6,	/* P6�̃|�[�g�f�[�^���W�X�^�̒�~�l */
	STOP_PDR7,	/* P7�̃|�[�g�f�[�^���W�X�^�̒�~�l */
	STOP_PDR8,	/* P8�̃|�[�g�f�[�^���W�X�^�̒�~�l */
	STOP_PDR9,	/* P9�̃|�[�g�f�[�^���W�X�^�̒�~�l */
	STOP_PDR10,	/* P10�̃|�[�g�f�[�^���W�X�^�̒�~�l */
	STOP_PDR11,	/* P11�̃|�[�g�f�[�^���W�X�^�̒�~�l */
	STOP_PDR12,	/* P12�̃|�[�g�f�[�^���W�X�^�̒�~�l */
	STOP_PDR13	/* P13�̃|�[�g�f�[�^���W�X�^�̒�~�l */
};

/* �f�[�^�������W�X�^�����l�e�[�u�� */
const static UI_8 C_DataDirectionInitTbl[PORT_MAX] =
{
	INIT_DDR0,	/* P0�̃f�[�^�������W�X�^�̏����l */
	INIT_DDR1,	/* P1�̃f�[�^�������W�X�^�̏����l */
	INIT_DDR2,	/* P2�̃f�[�^�������W�X�^�̏����l */
	INIT_DDR3,	/* P3�̃f�[�^�������W�X�^�̏����l */
	INIT_DDR4,	/* P4�̃f�[�^�������W�X�^�̏����l */
	INIT_DDR5,	/* P5�̃f�[�^�������W�X�^�̏����l */
	INIT_DDR6,	/* P6�̃f�[�^�������W�X�^�̏����l */
	INIT_DDR7,	/* P7�̃f�[�^�������W�X�^�̏����l */
	INIT_DDR8,	/* P8�̃f�[�^�������W�X�^�̏����l */
	INIT_DDR9,	/* P9�̃f�[�^�������W�X�^�̏����l */
	INIT_DDR10,	/* P10�̃f�[�^�������W�X�^�̏����l */
	INIT_DDR11,	/* P11�̃f�[�^�������W�X�^�̏����l */
	INIT_DDR12,	/* P12�̃f�[�^�������W�X�^�̏����l */
	INIT_DDR13	/* P13�̃f�[�^�������W�X�^�̏����l */
};

/* �f�[�^�������W�X�^��~�l�e�[�u�� */
const static UI_8 C_DataDirectionStopTbl[PORT_MAX] =
{
	STOP_DDR0,	/* P0�̃f�[�^�������W�X�^�̒�~�l */
	STOP_DDR1,	/* P1�̃f�[�^�������W�X�^�̒�~�l */
	STOP_DDR2,	/* P2�̃f�[�^�������W�X�^�̒�~�l */
	STOP_DDR3,	/* P3�̃f�[�^�������W�X�^�̒�~�l */
	STOP_DDR4,	/* P4�̃f�[�^�������W�X�^�̒�~�l */
	STOP_DDR5,	/* P5�̃f�[�^�������W�X�^�̒�~�l */
	STOP_DDR6,	/* P6�̃f�[�^�������W�X�^�̒�~�l */
	STOP_DDR7,	/* P7�̃f�[�^�������W�X�^�̒�~�l */
	STOP_DDR8,	/* P8�̃f�[�^�������W�X�^�̒�~�l */
	STOP_DDR9,	/* P9�̃f�[�^�������W�X�^�̒�~�l */
	STOP_DDR10,	/* P10�̃f�[�^�������W�X�^�̒�~�l */
	STOP_DDR11,	/* P11�̃f�[�^�������W�X�^�̒�~�l */
	STOP_DDR12,	/* P12�̃f�[�^�������W�X�^�̒�~�l */
	STOP_DDR13	/* P13�̃f�[�^�������W�X�^�̒�~�l */
};

/* �f�[�^�������W�X�^�Đݒ�l�e�[�u�� */
const static UI_8 C_DataDirectionRefreshTbl[PORT_MAX] =
{
	REFRESH_DDR0,	/* P0�̃f�[�^�������W�X�^�̍Đݒ�l */
	REFRESH_DDR1,	/* P1�̃f�[�^�������W�X�^�̍Đݒ�l */
	REFRESH_DDR2,	/* P2�̃f�[�^�������W�X�^�̍Đݒ�l */
	REFRESH_DDR3,	/* P3�̃f�[�^�������W�X�^�̍Đݒ�l */
	REFRESH_DDR4,	/* P4�̃f�[�^�������W�X�^�̍Đݒ�l */
	REFRESH_DDR5,	/* P5�̃f�[�^�������W�X�^�̍Đݒ�l */
	REFRESH_DDR6,	/* P6�̃f�[�^�������W�X�^�̍Đݒ�l */
	REFRESH_DDR7,	/* P7�̃f�[�^�������W�X�^�̍Đݒ�l */
	REFRESH_DDR8,	/* P8�̃f�[�^�������W�X�^�̍Đݒ�l */
	REFRESH_DDR9,	/* P9�̃f�[�^�������W�X�^�̍Đݒ�l */
	REFRESH_DDR10,	/* P10�̃f�[�^�������W�X�^�̍Đݒ�l */
	REFRESH_DDR11,	/* P11�̃f�[�^�������W�X�^�̍Đݒ�l */
	REFRESH_DDR12,	/* P12�̃f�[�^�������W�X�^�̍Đݒ�l */
	REFRESH_DDR13	/* P13�̃f�[�^�������W�X�^�̍Đݒ�l */
};

/* �|�[�g�@�\���W�X�^�����l�e�[�u�� */
const static UI_8 C_PortFunctionInitTbl[PORT_MAX] =
{
	INIT_PFR0,	/* P0�̃|�[�g�@�\���W�X�^�̏����l */
	INIT_PFR1,	/* P1�̃|�[�g�@�\���W�X�^�̏����l */
	INIT_PFR2,	/* P2�̃|�[�g�@�\���W�X�^�̏����l */
	INIT_PFR3,	/* P3�̃|�[�g�@�\���W�X�^�̏����l */
	INIT_PFR4,	/* P4�̃|�[�g�@�\���W�X�^�̏����l */
	INIT_PFR5,	/* P5�̃|�[�g�@�\���W�X�^�̏����l */
	INIT_PFR6,	/* P6�̃|�[�g�@�\���W�X�^�̏����l */
	INIT_PFR7,	/* P7�̃|�[�g�@�\���W�X�^�̏����l */
	INIT_PFR8,	/* P8�̃|�[�g�@�\���W�X�^�̏����l */
	INIT_PFR9,	/* P9�̃|�[�g�@�\���W�X�^�̏����l */
	INIT_PFR10,	/* P10�̃|�[�g�@�\���W�X�^�̏����l */
	INIT_PFR11,	/* P11�̃|�[�g�@�\���W�X�^�̏����l */
	INIT_PFR12,	/* P12�̃|�[�g�@�\���W�X�^�̏����l */
	INIT_PFR13	/* P13�̃|�[�g�@�\���W�X�^�̏����l */
};

/* �|�[�g�@�\���W�X�^��~�l�e�[�u�� */
const static UI_8 C_PortFunctionStopTbl[PORT_MAX] =
{
	STOP_PFR0,	/* P0�̃|�[�g�@�\���W�X�^�̒�~�l */
	STOP_PFR1,	/* P1�̃|�[�g�@�\���W�X�^�̒�~�l */
	STOP_PFR2,	/* P2�̃|�[�g�@�\���W�X�^�̒�~�l */
	STOP_PFR3,	/* P3�̃|�[�g�@�\���W�X�^�̒�~�l */
	STOP_PFR4,	/* P4�̃|�[�g�@�\���W�X�^�̒�~�l */
	STOP_PFR5,	/* P5�̃|�[�g�@�\���W�X�^�̒�~�l */
	STOP_PFR6,	/* P6�̃|�[�g�@�\���W�X�^�̒�~�l */
	STOP_PFR7,	/* P7�̃|�[�g�@�\���W�X�^�̒�~�l */
	STOP_PFR8,	/* P8�̃|�[�g�@�\���W�X�^�̒�~�l */
	STOP_PFR9,	/* P9�̃|�[�g�@�\���W�X�^�̒�~�l */
	STOP_PFR10,	/* P10�̃|�[�g�@�\���W�X�^�̒�~�l */
	STOP_PFR11,	/* P11�̃|�[�g�@�\���W�X�^�̒�~�l */
	STOP_PFR12,	/* P12�̃|�[�g�@�\���W�X�^�̒�~�l */
	STOP_PFR13	/* P13�̃|�[�g�@�\���W�X�^�̒�~�l */
};

/* �g���|�[�g�@�\���W�X�^�����l�e�[�u�� */
const static UI_8 C_ExtendedPortFunctionInitTbl[PORTDRV_EPFR_MAX] =
{
	INIT_EPFR0,		/* P0�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR1,		/* P1�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR2,		/* P2�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR3,		/* P3�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR4,		/* P4�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR5,		/* P5�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR6,		/* P6�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR7,		/* P7�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR8,		/* P8�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR9,		/* P9�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR10,	/* P10�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR11,	/* P11�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR12,	/* P12�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR13,	/* P13�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR14,	/* P14�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR15,	/* P15�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR16,	/* P16�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR17,	/* P17�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR18,	/* P18�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR19,	/* P19�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR20,	/* P20�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR21,	/* P21�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR22,	/* P22�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR23,	/* P23�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR24,	/* P24�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR25,	/* P25�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR26,	/* P26�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR27,	/* P27�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR28,	/* P28�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR29,	/* P29�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR30,	/* P30�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR31,	/* P31�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR32,	/* P32�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR33,	/* P33�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR34,	/* P34�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR35,	/* P35�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR36,	/* P36�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR37,	/* P37�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR38,	/* P38�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR39,	/* P39�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR40,	/* P40�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR41,	/* P41�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR42,	/* P42�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR43,	/* P43�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR44,	/* P44�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR45,	/* P45�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR46,	/* P46�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR47,	/* P47�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR52,	/* P52�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR53,	/* P53�̊g���|�[�g�@�\���W�X�^�̏����l */
	INIT_EPFR54		/* P54�̊g���|�[�g�@�\���W�X�^�̏����l */
};

/* �g���|�[�g�@�\���W�X�^��~�l�e�[�u�� */
const static UI_8 C_ExtendedPortFunctionStopTbl[PORTDRV_EPFR_MAX] =
{
	STOP_EPFR0,		/* P0�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR1,		/* P1�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR2,		/* P2�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR3,		/* P3�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR4,		/* P4�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR5,		/* P5�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR6,		/* P6�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR7,		/* P7�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR8,		/* P8�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR9,		/* P9�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR10,	/* P10�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR11,	/* P11�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR12,	/* P12�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR13,	/* P13�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR14,	/* P14�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR15,	/* P15�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR16,	/* P16�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR17,	/* P17�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR18,	/* P18�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR19,	/* P19�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR20,	/* P20�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR21,	/* P21�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR22,	/* P22�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR23,	/* P23�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR24,	/* P24�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR25,	/* P25�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR26,	/* P26�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR27,	/* P27�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR28,	/* P28�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR29,	/* P29�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR30,	/* P30�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR31,	/* P31�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR32,	/* P32�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR33,	/* P33�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR34,	/* P34�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR35,	/* P35�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR36,	/* P36�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR37,	/* P37�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR38,	/* P38�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR39,	/* P39�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR40,	/* P40�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR41,	/* P41�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR42,	/* P42�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR43,	/* P43�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR44,	/* P44�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR45,	/* P45�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR46,	/* P46�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR47,	/* P47�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR52,	/* P52�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR53,	/* P53�̊g���|�[�g�@�\���W�X�^�̒�~�l */
	STOP_EPFR54	/* P54�̊g���|�[�g�@�\���W�X�^�̒�~�l */
};

/* �v���A�b�v�_�E�����䃌�W�X�^�����l�e�[�u�� */
const static UI_8 C_PullControlInitTbl[PORT_MAX] =
{
	INIT_PPCR0,		/* P0�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
	INIT_PPCR1,		/* P1�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
	INIT_PPCR2,		/* P2�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
	INIT_PPCR3,		/* P3�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
	INIT_PPCR4,		/* P4�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
	INIT_PPCR5,		/* P5�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
	INIT_PPCR6,		/* P6�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
	INIT_PPCR7,		/* P7�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
	INIT_PPCR8,		/* P8�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
	INIT_PPCR9,		/* P9�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
	INIT_PPCR10,	/* P10�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
	INIT_PPCR11,	/* P11�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
	INIT_PPCR12,	/* P12�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
	INIT_PPCR13		/* P13�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
};

/* �v���A�b�v�_�E�����䃌�W�X�^��~�l�e�[�u�� */
const static UI_8 C_PullControlStopTbl[PORT_MAX] =
{
	STOP_PPCR0,		/* P0�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
	STOP_PPCR1,		/* P1�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
	STOP_PPCR2,		/* P2�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
	STOP_PPCR3,		/* P3�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
	STOP_PPCR4,		/* P4�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
	STOP_PPCR5,		/* P5�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
	STOP_PPCR6,		/* P6�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
	STOP_PPCR7,		/* P7�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
	STOP_PPCR8,		/* P8�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
	STOP_PPCR9,		/* P9�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
	STOP_PPCR10,	/* P10�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
	STOP_PPCR11,	/* P11�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
	STOP_PPCR12,	/* P12�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
	STOP_PPCR13		/* P13�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
};

/* �v���A�b�v�_�E�������W�X�^�����l�e�[�u�� */
const static UI_8 C_PullEnableInitTbl[PORT_MAX] =
{
	INIT_PPER0,		/* P0�̃v���A�b�v�_�E�������W�X�^�̏����l */
	INIT_PPER1,		/* P1�̃v���A�b�v�_�E�������W�X�^�̏����l */
	INIT_PPER2,		/* P2�̃v���A�b�v�_�E�������W�X�^�̏����l */
	INIT_PPER3,		/* P3�̃v���A�b�v�_�E�������W�X�^�̏����l */
	INIT_PPER4,		/* P4�̃v���A�b�v�_�E�������W�X�^�̏����l */
	INIT_PPER5,		/* P5�̃v���A�b�v�_�E�������W�X�^�̏����l */
	INIT_PPER6,		/* P6�̃v���A�b�v�_�E�������W�X�^�̏����l */
	INIT_PPER7,		/* P7�̃v���A�b�v�_�E�������W�X�^�̏����l */
	INIT_PPER8,		/* P8�̃v���A�b�v�_�E�������W�X�^�̏����l */
	INIT_PPER9,		/* P9�̃v���A�b�v�_�E�������W�X�^�̏����l */
	INIT_PPER10,	/* P10�̃v���A�b�v�_�E�������W�X�^�̏����l */
	INIT_PPER11,	/* P11�̃v���A�b�v�_�E�������W�X�^�̏����l */
	INIT_PPER12,	/* P12�̃v���A�b�v�_�E�������W�X�^�̏����l */
	INIT_PPER13		/* P13�̃v���A�b�v�_�E�������W�X�^�̏����l */
};

/* �v���A�b�v�_�E�������W�X�^��~�l�e�[�u�� */
const static UI_8 C_PullEnableStopTbl[PORT_MAX] =
{
	STOP_PPER0,		/* P0�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
	STOP_PPER1,		/* P1�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
	STOP_PPER2,		/* P2�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
	STOP_PPER3,		/* P3�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
	STOP_PPER4,		/* P4�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
	STOP_PPER5,		/* P5�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
	STOP_PPER6,		/* P6�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
	STOP_PPER7,		/* P7�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
	STOP_PPER8,		/* P8�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
	STOP_PPER9,		/* P9�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
	STOP_PPER10,	/* P10�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
	STOP_PPER11,	/* P11�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
	STOP_PPER12,	/* P12�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
	STOP_PPER13		/* P13�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
};

/* �|�[�g���̓��x���I�����W�X�^�����l�e�[�u�� */
const static UI_8 C_PortInputLevelInitTbl[PORT_MAX] =
{
	INIT_PILR0,		/* P0�̃|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_PILR1,		/* P1�̃|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_PILR2,		/* P2�̃|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_PILR3,		/* P3�̃|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_PILR4,		/* P4�̃|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_PILR5,		/* P5�̃|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_PILR6,		/* P6�̃|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_PILR7,		/* P7�̃|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_PILR8,		/* P8�̃|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_PILR9,		/* P9�̃|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_PILR10,	/* P10�̃|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_PILR11,	/* P11�̃|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_PILR12,	/* P12�̃|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_PILR13		/* P13�̃|�[�g���̓��x���I�����W�X�^�̏����l */
};

/* �|�[�g���̓��x���I�����W�X�^��~�l�e�[�u�� */
const static UI_8 C_PortInputLevelStopTbl[PORT_MAX] =
{
	STOP_PILR0,		/* P0�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_PILR1,		/* P1�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_PILR2,		/* P2�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_PILR3,		/* P3�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_PILR4,		/* P4�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_PILR5,		/* P5�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_PILR6,		/* P6�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_PILR7,		/* P7�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_PILR8,		/* P8�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_PILR9,		/* P9�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_PILR10,	/* P10�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_PILR11,	/* P11�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_PILR12,	/* P12�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_PILR13		/* P13�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
};

/* �g���|�[�g���̓��x���I�����W�X�^�����l�e�[�u�� */
const static UI_8 C_ExtendedPortInputLevelInitTbl[PORT_MAX] =
{
	INIT_EPILR0,	/* P0�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_EPILR1,	/* P1�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_EPILR2,	/* P2�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_EPILR3,	/* P3�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_EPILR4,	/* P4�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_EPILR5,	/* P5�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_EPILR6,	/* P6�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_EPILR7,	/* P7�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_EPILR8,	/* P8�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_EPILR9,	/* P9�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_EPILR10,	/* P10�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_EPILR11,	/* P11�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_EPILR12,	/* P12�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
	INIT_EPILR13	/* P13�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
};

/* �g���|�[�g���̓��x���I�����W�X�^��~�l�e�[�u�� */
const static UI_8 C_ExtendedPortInputLevelStopTbl[PORT_MAX] =
{
	STOP_EPILR0,	/* P0�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_EPILR1,	/* P1�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_EPILR2,	/* P2�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_EPILR3,	/* P3�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_EPILR4,	/* P4�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_EPILR5,	/* P5�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_EPILR6,	/* P6�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_EPILR7,	/* P7�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_EPILR8,	/* P8�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_EPILR9,	/* P9�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_EPILR10,	/* P10�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_EPILR11,	/* P11�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_EPILR12,	/* P12�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
	STOP_EPILR13	/* P13�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
};

/* �|�[�g�o�͋쓮���W�X�^�����l�e�[�u�� */
const static UI_8 C_PortOutputDriveInitTbl[PORT_MAX] =
{
	INIT_PODR0,		/* P0�̃|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_PODR1,		/* P1�̃|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_PODR2,		/* P2�̃|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_PODR3,		/* P3�̃|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_PODR4,		/* P4�̃|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_PODR5,		/* P5�̃|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_PODR6,		/* P6�̃|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_PODR7,		/* P7�̃|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_PODR8,		/* P8�̃|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_PODR9,		/* P9�̃|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_PODR10,	/* P10�̃|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_PODR11,	/* P11�̃|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_PODR12,	/* P12�̃|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_PODR13		/* P13�̃|�[�g�o�͋쓮���W�X�^�̏����l */
};

/* �|�[�g�o�͋쓮���W�X�^��~�l�e�[�u�� */
const static UI_8 C_PortOutputDriveStopTbl[PORT_MAX] =
{
	STOP_PODR0,		/* P0�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_PODR1,		/* P1�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_PODR2,		/* P2�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_PODR3,		/* P3�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_PODR4,		/* P4�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_PODR5,		/* P5�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_PODR6,		/* P6�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_PODR7,		/* P7�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_PODR8,		/* P8�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_PODR9,		/* P9�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_PODR10,	/* P10�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_PODR11,	/* P11�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_PODR12,	/* P12�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_PODR13		/* P13�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
};

/* �g���|�[�g�o�͋쓮���W�X�^�����l�e�[�u�� */
const static UI_8 C_ExtendedPortOutputDriveInitTbl[PORT_MAX] =
{
	D_NULL,			/* ���W�X�^�ݒ�Ȃ�*/
	INIT_EPODR1,	/* P1�̊g���|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_EPODR2,	/* P2�̊g���|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_EPODR3,	/* P3�̊g���|�[�g�o�͋쓮���W�X�^�̏����l */
	D_NULL,			/* ���W�X�^�ݒ�Ȃ�*/
	D_NULL,			/* ���W�X�^�ݒ�Ȃ�*/
	INIT_EPODR6,	/* P6�̊g���|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_EPODR7,	/* P7�̊g���|�[�g�o�͋쓮���W�X�^�̏����l */
	INIT_EPODR8,	/* P8�̊g���|�[�g�o�͋쓮���W�X�^�̏����l */
	D_NULL,			/* ���W�X�^�ݒ�Ȃ�*/
	D_NULL,			/* ���W�X�^�ݒ�Ȃ�*/
	D_NULL,			/* ���W�X�^�ݒ�Ȃ�*/
	D_NULL,			/* ���W�X�^�ݒ�Ȃ�*/
	D_NULL			/* ���W�X�^�ݒ�Ȃ�*/
};

/* �g���|�[�g�o�͋쓮���W�X�^��~�l�e�[�u�� */
const static UI_8 C_ExtendedPortOutputDriveStopTbl[PORT_MAX] =
{
	D_NULL,			/* ���W�X�^�ݒ�Ȃ�*/
	STOP_EPODR1,	/* P1�̊g���|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_EPODR2,	/* P2�̊g���|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_EPODR3,	/* P3�̊g���|�[�g�o�͋쓮���W�X�^�̒�~�l */
	D_NULL,			/* ���W�X�^�ݒ�Ȃ�*/
	D_NULL,			/* ���W�X�^�ݒ�Ȃ�*/
	STOP_EPODR6,	/* P6�̊g���|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_EPODR7,	/* P7�̊g���|�[�g�o�͋쓮���W�X�^�̒�~�l */
	STOP_EPODR8,	/* P8�̊g���|�[�g�o�͋쓮���W�X�^�̒�~�l */
	D_NULL,			/* ���W�X�^�ݒ�Ȃ�*/
	D_NULL,			/* ���W�X�^�ݒ�Ȃ�*/
	D_NULL,			/* ���W�X�^�ݒ�Ȃ�*/
	D_NULL,			/* ���W�X�^�ݒ�Ȃ�*/
	D_NULL			/* ���W�X�^�ݒ�Ȃ�*/
};

#if (PORT_DRV_FUNC_CHANGE == 1)
/* �@�\���W�X�^�ݒ萔�e�[�u�� */
const static UI_8 C_PortFuncChangeTbl[PORT_MAX] =
{
	PORT_FUNC_CHANGE0,		/* P0�̋@�\���W�X�^�ݒ萔 */
	PORT_FUNC_CHANGE1,		/* P1�̋@�\���W�X�^�ݒ萔 */
	PORT_FUNC_CHANGE2,		/* P2�̋@�\���W�X�^�ݒ萔 */
	PORT_FUNC_CHANGE3,		/* P3�̋@�\���W�X�^�ݒ萔 */
	PORT_FUNC_CHANGE4,		/* P4�̋@�\���W�X�^�ݒ萔 */
	PORT_FUNC_CHANGE5,		/* P5�̋@�\���W�X�^�ݒ萔 */
	PORT_FUNC_CHANGE6,		/* P6�̋@�\���W�X�^�ݒ萔 */
	PORT_FUNC_CHANGE7,		/* P7�̋@�\���W�X�^�ݒ萔 */
	PORT_FUNC_CHANGE8,		/* P8�̋@�\���W�X�^�ݒ萔 */
	PORT_FUNC_CHANGE9,		/* P9�̋@�\���W�X�^�ݒ萔 */
	PORT_FUNC_CHANGE10,		/* P10�̋@�\���W�X�^�ݒ萔 */
	PORT_FUNC_CHANGE11,		/* P11�̋@�\���W�X�^�ݒ萔 */
	PORT_FUNC_CHANGE12,		/* P12�̋@�\���W�X�^�ݒ萔 */
	PORT_FUNC_CHANGE13		/* P13�̋@�\���W�X�^�ݒ萔 */
};

/* �@�\���W�X�^�ݒ萔�e�[�u�� */
const static UI_8 C_PortFuncChangeNumTbl[PORT_MAX] =
{
	PORT_FUNC_CHANGE_NUM0,		/* P0�̋@�\���W�X�^�� */
	PORT_FUNC_CHANGE_NUM1,		/* P1�̋@�\���W�X�^�� */
	PORT_FUNC_CHANGE_NUM2,		/* P2�̋@�\���W�X�^�� */
	PORT_FUNC_CHANGE_NUM3,		/* P3�̋@�\���W�X�^�� */
	PORT_FUNC_CHANGE_NUM4,		/* P4�̋@�\���W�X�^�� */
	PORT_FUNC_CHANGE_NUM5,		/* P5�̋@�\���W�X�^�� */
	PORT_FUNC_CHANGE_NUM6,		/* P6�̋@�\���W�X�^�� */
	PORT_FUNC_CHANGE_NUM7,		/* P7�̋@�\���W�X�^�� */
	PORT_FUNC_CHANGE_NUM8,		/* P8�̋@�\���W�X�^�� */
	PORT_FUNC_CHANGE_NUM9,		/* P9�̋@�\���W�X�^�� */
	PORT_FUNC_CHANGE_NUM10,		/* P10�̋@�\���W�X�^�� */
	PORT_FUNC_CHANGE_NUM11,		/* P11�̋@�\���W�X�^�� */
	PORT_FUNC_CHANGE_NUM12,		/* P12�̋@�\���W�X�^�� */
	PORT_FUNC_CHANGE_NUM13		/* P13�̋@�\���W�X�^�� */
};

/* �@�\�ؑ֐ݒ�lID���e�[�u�� */
const static UI_8 C_PortFuncChangeIdNumTbl[PORT_MAX][BIT_MAX] =
{
	/* PXX0							PXX1							PXX2							PXX3							PXX4							PXX5							PXX6							PXX7 */
	{ PORT_FUNC_CHANGE_ID_NUM_10,	PORT_FUNC_CHANGE_ID_NUM_10,		PORT_FUNC_CHANGE_ID_NUM_11,		PORT_FUNC_CHANGE_ID_NUM_10, 	PORT_FUNC_CHANGE_ID_NUM_10,		PORT_FUNC_CHANGE_ID_NUM_11,		PORT_FUNC_CHANGE_ID_NUM_9,		PORT_FUNC_CHANGE_ID_NUM_9	},		/* P000�̋@�\�� */
	{ PORT_FUNC_CHANGE_ID_NUM_8,	PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8	},		/* P010�̋@�\�� */
	{ PORT_FUNC_CHANGE_ID_NUM_6,	PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_7	},		/* P020�̋@�\�� */
	{ PORT_FUNC_CHANGE_ID_NUM_7,	PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_7	},		/* P030�̋@�\�� */
	{ PORT_FUNC_CHANGE_ID_NUM_7,	PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_8	},		/* P040�̋@�\�� */
	{ PORT_FUNC_CHANGE_ID_NUM_5,	PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_4	},		/* P050�̋@�\�� */
	{ PORT_FUNC_CHANGE_ID_NUM_4,	PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_6	},		/* P060�̋@�\�� */
	{ PORT_FUNC_CHANGE_ID_NUM_5,	PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_8	},		/* P070�̋@�\�� */
	{ PORT_FUNC_CHANGE_ID_NUM_7,	PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_6	},		/* P080�̋@�\�� */
	{ PORT_FUNC_CHANGE_ID_NUM_4,	PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_9,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_4,		PORT_FUNC_CHANGE_ID_NUM_4,		PORT_FUNC_CHANGE_ID_NUM_8	},		/* P090�̋@�\�� */
	{ PORT_FUNC_CHANGE_ID_NUM_6,	PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_7	},		/* P100�̋@�\�� */
	{ PORT_FUNC_CHANGE_ID_NUM_5,	PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_4,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_9,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_9	},		/* P110�̋@�\�� */
	{ PORT_FUNC_CHANGE_ID_NUM_8,	PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_5,		PORT_FUNC_CHANGE_ID_NUM_6,		PORT_FUNC_CHANGE_ID_NUM_4	},		/* P120�̋@�\�� */
	{ PORT_FUNC_CHANGE_ID_NUM_8,	PORT_FUNC_CHANGE_ID_NUM_8,		PORT_FUNC_CHANGE_ID_NUM_7,		PORT_FUNC_CHANGE_ID_NUM_10,		PORT_FUNC_CHANGE_ID_NUM_6,		D_NULL,							PORT_FUNC_CHANGE_ID_NUM_3,		PORT_FUNC_CHANGE_ID_NUM_3	}		/* P130�̋@�\�� */
};
	
/*�sP00 �@�\���W�X�^�A�h���X�e�[�u���t*/
static __io volatile UI_8 * const C_PortFuncTbl_P00[PORT_FUNC_CHANGE_NUM0] =
{
	&(DDR00),		/* �f�[�^�������W�X�^00�̃A�h���X */
	&(PFR00),		/* �|�[�g�@�\���W�X�^00�̃A�h���X */
	&(EPFR02),		/* �g���|�[�g�@�\���W�X�^02�̃A�h���X */
	&(EPFR03),		/* �g���|�[�g�@�\���W�X�^03�̃A�h���X */
	&(EPFR04),		/* �g���|�[�g�@�\���W�X�^04�̃A�h���X */
	&(EPFR05),		/* �g���|�[�g�@�\���W�X�^05�̃A�h���X */
	&(EPFR06),		/* �g���|�[�g�@�\���W�X�^06�̃A�h���X */
	&(EPFR07),		/* �g���|�[�g�@�\���W�X�^07�̃A�h���X */
	&(EPFR10),		/* �g���|�[�g�@�\���W�X�^10�̃A�h���X */
	&(EPFR11),		/* �g���|�[�g�@�\���W�X�^11�̃A�h���X */
	&(EPFR12),		/* �g���|�[�g�@�\���W�X�^12�̃A�h���X */
	&(EPFR13),		/* �g���|�[�g�@�\���W�X�^13�̃A�h���X */
	&(EPFR43),		/* �g���|�[�g�@�\���W�X�^43�̃A�h���X */
	&(EPFR47)		/* �g���|�[�g�@�\���W�X�^47�̃A�h���X */
};

/*�sP01 �@�\���W�X�^�A�h���X�e�[�u���t*/
static __io volatile UI_8 * const C_PortFuncTbl_P01[PORT_FUNC_CHANGE_NUM1] =
{
	&(DDR01),		/* �f�[�^�������W�X�^01�̃A�h���X */
	&(PFR01),		/* �|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR16),		/* �g���|�[�g�@�\���W�X�^16�̃A�h���X */
	&(EPFR44),		/* �g���|�[�g�@�\���W�X�^44�̃A�h���X */
	&(EPFR47)		/* �g���|�[�g�@�\���W�X�^47�̃A�h���X */
};

/* �sP02 �@�\���W�X�^�A�h���X�e�[�u���t */
static __io volatile UI_8 * const C_PortFuncTbl_P02[PORT_FUNC_CHANGE_NUM2] =
{
	&(DDR02),		/* �f�[�^�������W�X�^02�̃A�h���X */
	&(PFR02),		/* �|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR01),		/* �g���|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR17),		/* �g���|�[�g�@�\���W�X�^17�̃A�h���X */
	&(EPFR29),		/* �g���|�[�g�@�\���W�X�^29�̃A�h���X */
	&(EPFR32),		/* �g���|�[�g�@�\���W�X�^32�̃A�h���X */
	&(EPFR33),		/* �g���|�[�g�@�\���W�X�^33�̃A�h���X */
	&(EPFR39)		/* �g���|�[�g�@�\���W�X�^39�̃A�h���X */
};

/* �sP03 �@�\���W�X�^�A�h���X�e�[�u���t */
static __io volatile UI_8 * const C_PortFuncTbl_P03[PORT_FUNC_CHANGE_NUM3] =
{
	&(DDR03),		/* �f�[�^�������W�X�^03�̃A�h���X */
	&(PFR03),		/* �|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR15),		/* �g���|�[�g�@�\���W�X�^15�̃A�h���X */
	&(EPFR18),		/* �g���|�[�g�@�\���W�X�^18�̃A�h���X */
	&(EPFR31),		/* �g���|�[�g�@�\���W�X�^31�̃A�h���X */
	&(EPFR33),		/* �g���|�[�g�@�\���W�X�^33�̃A�h���X */
	&(EPFR34),		/* �g���|�[�g�@�\���W�X�^34�̃A�h���X */
	&(EPFR37),		/* �g���|�[�g�@�\���W�X�^37�̃A�h���X */
	&(EPFR38)		/* �g���|�[�g�@�\���W�X�^38�̃A�h���X */
};

/* �sP04 �@�\���W�X�^�A�h���X�e�[�u���t*/
static __io volatile UI_8 * const C_PortFuncTbl_P04[PORT_FUNC_CHANGE_NUM4] =
{
	&(DDR04),		/* �f�[�^�������W�X�^03�̃A�h���X */
	&(PFR04),		/* �|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR19),		/* �g���|�[�g�@�\���W�X�^19�̃A�h���X */
	&(EPFR30),		/* �g���|�[�g�@�\���W�X�^30�̃A�h���X */
	&(EPFR31),		/* �g���|�[�g�@�\���W�X�^31�̃A�h���X */
	&(EPFR34),		/* �g���|�[�g�@�\���W�X�^34�̃A�h���X */
	&(EPFR37),		/* �g���|�[�g�@�\���W�X�^37�̃A�h���X */
	&(EPFR40),		/* �g���|�[�g�@�\���W�X�^40�̃A�h���X */
	&(EPFR41),		/* �g���|�[�g�@�\���W�X�^41�̃A�h���X */
	&(EPFR45)		/* �g���|�[�g�@�\���W�X�^45�̃A�h���X */
};

/* �sP05 �@�\���W�X�^�A�h���X�e�[�u���t*/
static __io volatile UI_8 * const C_PortFuncTbl_P05[PORT_FUNC_CHANGE_NUM5] =
{	
	&(DDR05),		/* �f�[�^�������W�X�^03�̃A�h���X */
	&(PFR05),		/* �|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR20),		/* �g���|�[�g�@�\���W�X�^20�̃A�h���X */
	&(EPFR28),		/* �g���|�[�g�@�\���W�X�^28�̃A�h���X */
	&(EPFR32)		/* �g���|�[�g�@�\���W�X�^32�̃A�h���X */
};

/* �sP06 �@�\���W�X�^�A�h���X�e�[�u���t*/
static __io volatile UI_8 * const C_PortFuncTbl_P06[PORT_FUNC_CHANGE_NUM6] =
{
	&(DDR06),		/* �f�[�^�������W�X�^03�̃A�h���X */
	&(PFR06),		/* �|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR21),		/* �g���|�[�g�@�\���W�X�^21�̃A�h���X */
	&(EPFR35),		/* �g���|�[�g�@�\���W�X�^35�̃A�h���X */
	&(EPFR36),		/* �g���|�[�g�@�\���W�X�^36�̃A�h���X */
	&(EPFR38),		/* �g���|�[�g�@�\���W�X�^38�̃A�h���X */
	&(EPFR40),		/* �g���|�[�g�@�\���W�X�^40�̃A�h���X */
	&(EPFR41)		/* �g���|�[�g�@�\���W�X�^41�̃A�h���X */
};

/* �sP07 �@�\���W�X�^�A�h���X�e�[�u���t*/
static __io volatile UI_8 * const C_PortFuncTbl_P07[PORT_FUNC_CHANGE_NUM7] =
{
	&(DDR07),		/* �f�[�^�������W�X�^03�̃A�h���X */
	&(PFR07),		/* �|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR01),		/* �g���|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR15),		/* �g���|�[�g�@�\���W�X�^15�̃A�h���X */
	&(EPFR22),		/* �g���|�[�g�@�\���W�X�^22�̃A�h���X */
	&(EPFR34),		/* �g���|�[�g�@�\���W�X�^34�̃A�h���X */
	&(EPFR37),		/* �g���|�[�g�@�\���W�X�^37�̃A�h���X */
	&(EPFR38),		/* �g���|�[�g�@�\���W�X�^38�̃A�h���X */
	&(EPFR39),		/* �g���|�[�g�@�\���W�X�^39�̃A�h���X */
	&(EPFR45)		/* �g���|�[�g�@�\���W�X�^45�̃A�h���X */
};

/* �sP08 �@�\���W�X�^�A�h���X�e�[�u���t*/
static __io volatile UI_8 * const C_PortFuncTbl_P08[PORT_FUNC_CHANGE_NUM8] =
{
	&(DDR08),		/* �f�[�^�������W�X�^03�̃A�h���X */
	&(PFR08),		/* �|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR00),		/* �g���|�[�g�@�\���W�X�^00�̃A�h���X */
	&(EPFR01),		/* �g���|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR23),		/* �g���|�[�g�@�\���W�X�^23�̃A�h���X */
	&(EPFR33),		/* �g���|�[�g�@�\���W�X�^33�̃A�h���X */
	&(EPFR40),		/* �g���|�[�g�@�\���W�X�^40�̃A�h���X */
	&(EPFR45),		/* �g���|�[�g�@�\���W�X�^45�̃A�h���X */
	&(EPFR46)		/* �g���|�[�g�@�\���W�X�^46�̃A�h���X */
};

/* �sP09 �@�\���W�X�^�A�h���X�e�[�u���t*/
static __io volatile UI_8 * const C_PortFuncTbl_P09[PORT_FUNC_CHANGE_NUM9] =
{
	&(DDR09),		/* �f�[�^�������W�X�^09�̃A�h���X */
	&(PFR09),		/* �|�[�g�@�\���W�X�^09�̃A�h���X */
	&(EPFR00),		/* �g���|�[�g�@�\���W�X�^00�̃A�h���X */
	&(EPFR01),		/* �g���|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR02),		/* �g���|�[�g�@�\���W�X�^02�̃A�h���X */
	&(EPFR04),		/* �g���|�[�g�@�\���W�X�^04�̃A�h���X */
	&(EPFR05),		/* �g���|�[�g�@�\���W�X�^05�̃A�h���X */
	&(EPFR06),		/* �g���|�[�g�@�\���W�X�^06�̃A�h���X */
	&(EPFR07),		/* �g���|�[�g�@�\���W�X�^07�̃A�h���X */
	&(EPFR10),		/* �g���|�[�g�@�\���W�X�^10�̃A�h���X */
	&(EPFR13),		/* �g���|�[�g�@�\���W�X�^13�̃A�h���X */
	&(EPFR14),		/* �g���|�[�g�@�\���W�X�^14�̃A�h���X */
	&(EPFR15),		/* �g���|�[�g�@�\���W�X�^15�̃A�h���X */
	&(EPFR24),		/* �g���|�[�g�@�\���W�X�^24�̃A�h���X */
	&(EPFR27),		/* �g���|�[�g�@�\���W�X�^27�̃A�h���X */
	&(EPFR44)		/* �g���|�[�g�@�\���W�X�^44�̃A�h���X */
};

/* �sP10 �@�\���W�X�^�A�h���X�e�[�u���t*/
static __io volatile UI_8 * const C_PortFuncTbl_P10[PORT_FUNC_CHANGE_NUM10] =
{
	&(DDR10),		/* �f�[�^�������W�X�^09�̃A�h���X */
	&(PFR10),		/* �|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR01),		/* �g���|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR02),		/* �g���|�[�g�@�\���W�X�^02�̃A�h���X */
	&(EPFR03),		/* �g���|�[�g�@�\���W�X�^03�̃A�h���X */
	&(EPFR04),		/* �g���|�[�g�@�\���W�X�^04�̃A�h���X */
	&(EPFR05),		/* �g���|�[�g�@�\���W�X�^05�̃A�h���X */
	&(EPFR08),		/* �g���|�[�g�@�\���W�X�^08�̃A�h���X */
	&(EPFR09),		/* �g���|�[�g�@�\���W�X�^09�̃A�h���X */
	&(EPFR10),		/* �g���|�[�g�@�\���W�X�^10�̃A�h���X */
	&(EPFR11),		/* �g���|�[�g�@�\���W�X�^11�̃A�h���X */
	&(EPFR12),		/* �g���|�[�g�@�\���W�X�^12�̃A�h���X */
	&(EPFR14),		/* �g���|�[�g�@�\���W�X�^14�̃A�h���X */
	&(EPFR15),		/* �g���|�[�g�@�\���W�X�^15�̃A�h���X */
	&(EPFR30),		/* �g���|�[�g�@�\���W�X�^30�̃A�h���X */
	&(EPFR39),		/* �g���|�[�g�@�\���W�X�^39�̃A�h���X */
	&(EPFR42)		/* �g���|�[�g�@�\���W�X�^42�̃A�h���X */
};

/* �sP11�@�\���W�X�^�A�h���X�e�[�u���t*/
static __io volatile UI_8 * const C_PortFuncTbl_P11[PORT_FUNC_CHANGE_NUM11] =
{
	&(DDR11),		/* �f�[�^�������W�X�^09�̃A�h���X */
	&(PFR11),		/* �|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR01),		/* �g���|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR02),		/* �g���|�[�g�@�\���W�X�^02�̃A�h���X */
	&(EPFR03),		/* �g���|�[�g�@�\���W�X�^03�̃A�h���X */
	&(EPFR04),		/* �g���|�[�g�@�\���W�X�^04�̃A�h���X */
	&(EPFR05),		/* �g���|�[�g�@�\���W�X�^05�̃A�h���X */
	&(EPFR07),		/* �g���|�[�g�@�\���W�X�^07�̃A�h���X */
	&(EPFR08),		/* �g���|�[�g�@�\���W�X�^08�̃A�h���X */
	&(EPFR10),		/* �g���|�[�g�@�\���W�X�^10�̃A�h���X */
	&(EPFR11),		/* �g���|�[�g�@�\���W�X�^11�̃A�h���X */
	&(EPFR12),		/* �g���|�[�g�@�\���W�X�^12�̃A�h���X */
	&(EPFR24),		/* �g���|�[�g�@�\���W�X�^24�̃A�h���X */
	&(EPFR28),		/* �g���|�[�g�@�\���W�X�^28�̃A�h���X */
	&(EPFR30),		/* �g���|�[�g�@�\���W�X�^30�̃A�h���X */
	&(EPFR44)		/* �g���|�[�g�@�\���W�X�^44�̃A�h���X */
};

/* �sP12 �@�\���W�X�^�A�h���X�e�[�u���t*/
static __io volatile UI_8* const C_PortFuncTbl_P12[PORT_FUNC_CHANGE_NUM12] =
{
	&(DDR12),		/* �f�[�^�������W�X�^09�̃A�h���X */
	&(PFR12),		/* �|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR00),		/* �g���|�[�g�@�\���W�X�^00�̃A�h���X */
	&(EPFR01),		/* �g���|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR03),		/* �g���|�[�g�@�\���W�X�^03�̃A�h���X */
	&(EPFR04),		/* �g���|�[�g�@�\���W�X�^04�̃A�h���X */
	&(EPFR05),		/* �g���|�[�g�@�\���W�X�^05�̃A�h���X */
	&(EPFR09),		/* �g���|�[�g�@�\���W�X�^09�̃A�h���X */
	&(EPFR12),		/* �g���|�[�g�@�\���W�X�^12�̃A�h���X */
	&(EPFR13),		/* �g���|�[�g�@�\���W�X�^13�̃A�h���X */
	&(EPFR14),		/* �g���|�[�g�@�\���W�X�^14�̃A�h���X */
	&(EPFR15),		/* �g���|�[�g�@�\���W�X�^15�̃A�h���X */
	&(EPFR28),		/* �g���|�[�g�@�\���W�X�^28�̃A�h���X */
	&(EPFR29),		/* �g���|�[�g�@�\���W�X�^29�̃A�h���X */
	&(EPFR31),		/* �g���|�[�g�@�\���W�X�^31�̃A�h���X */
	&(EPFR35),		/* �g���|�[�g�@�\���W�X�^35�̃A�h���X */
	&(EPFR42),		/* �g���|�[�g�@�\���W�X�^42�̃A�h���X */
	&(EPFR43)		/* �g���|�[�g�@�\���W�X�^43�̃A�h���X */
};

/* �sP13 �@�\���W�X�^�A�h���X�e�[�u���t*/
static __io volatile UI_8* const C_PortFuncTbl_P13[PORT_FUNC_CHANGE_NUM13] =
{
	&(DDR13),		/* �f�[�^�������W�X�^09�̃A�h���X */
	&(PFR13),		/* �|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR00),		/* �g���|�[�g�@�\���W�X�^00�̃A�h���X */
	&(EPFR01),		/* �g���|�[�g�@�\���W�X�^01�̃A�h���X */
	&(EPFR10),		/* �g���|�[�g�@�\���W�X�^10�̃A�h���X */
	&(EPFR15),		/* �g���|�[�g�@�\���W�X�^15�̃A�h���X */
	&(EPFR26),		/* �g���|�[�g�@�\���W�X�^26�̃A�h���X */
	&(EPFR35),		/* �g���|�[�g�@�\���W�X�^35�̃A�h���X */
	&(EPFR36),		/* �g���|�[�g�@�\���W�X�^36�̃A�h���X */
	&(EPFR43)		/* �g���|�[�g�@�\���W�X�^43�̃A�h���X */
};

/* �@�\���W�X�^�|�[�g�I���e�[�u�� */
static __io volatile UI_8* const * const C_PortFuncTbl[PORT_MAX] =
{
	&C_PortFuncTbl_P00[0],	/* �@�\���W�X�^�A�h���X�e�[�u���ւ̃|�C���^P00 */
	&C_PortFuncTbl_P01[0],	/* �@�\���W�X�^�A�h���X�e�[�u���ւ̃|�C���^P01 */
	&C_PortFuncTbl_P02[0],	/* �@�\���W�X�^�A�h���X�e�[�u���ւ̃|�C���^P02 */
	&C_PortFuncTbl_P03[0],	/* �@�\���W�X�^�A�h���X�e�[�u���ւ̃|�C���^P03 */
	&C_PortFuncTbl_P04[0],	/* �@�\���W�X�^�A�h���X�e�[�u���ւ̃|�C���^P04 */
	&C_PortFuncTbl_P05[0],	/* �@�\���W�X�^�A�h���X�e�[�u���ւ̃|�C���^P05 */
	&C_PortFuncTbl_P06[0],	/* �@�\���W�X�^�A�h���X�e�[�u���ւ̃|�C���^P06 */
	&C_PortFuncTbl_P07[0],	/* �@�\���W�X�^�A�h���X�e�[�u���ւ̃|�C���^P07 */
	&C_PortFuncTbl_P08[0],	/* �@�\���W�X�^�A�h���X�e�[�u���ւ̃|�C���^P08 */
	&C_PortFuncTbl_P09[0],	/* �@�\���W�X�^�A�h���X�e�[�u���ւ̃|�C���^P09 */
	&C_PortFuncTbl_P10[0],	/* �@�\���W�X�^�A�h���X�e�[�u���ւ̃|�C���^P10 */
	&C_PortFuncTbl_P11[0],	/* �@�\���W�X�^�A�h���X�e�[�u���ւ̃|�C���^P11 */
	&C_PortFuncTbl_P12[0],	/* �@�\���W�X�^�A�h���X�e�[�u���ւ̃|�C���^P12 */
	&C_PortFuncTbl_P13[0]	/* �@�\���W�X�^�A�h���X�e�[�u���ւ̃|�C���^P13 */
};

/* �wP000�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P000[PORT_FUNC_CHANGE_ID_NUM_10][PORT_FUNC_CHANGE0] =
{
	/* DDR00 			PFR00			EPFR02				EPFR03		EPFR04		EPFR05		EPFR06				EPFR07		EPFR10				EPFR11		EPFR12		EPFR13		EPFR43				EPFR47					ID */
	{ D_LOW_PXX0, 		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX0, 		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_LOW_PXX0,		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR06_SIN2E_HIGH,	D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SERIAL_IN		},		/* LIN_UART �V���A������ch.2 �����P�[�V����1 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR02_TIN0E_10,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				RELOAD_TIMER_EVENT_IN	},		/* �����[�h�^�C�}�C�x���g����ch.0 �����P�[�V����2 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX1,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG �o��ch.0 �����P�[�V����0 */
	{ D_LOW_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX0,		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR10_PPG0E_XX0,	D_NULL,		D_NULL,		D_NULL,		EPFR43_INT0E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.0 �����P�[�V����1 */
};

/* �wP001�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P001[PORT_FUNC_CHANGE_ID_NUM_10][PORT_FUNC_CHANGE0] =
{
	/* DDR00 			PFR00			EPFR02		EPFR03				EPFR04		EPFR05		EPFR06				EPFR07		EPFR10				EPFR11		EPFR12		EPFR13		EPFR43				EPFR47					ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX1,		D_HIGH_PXX1,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_1X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SERIAL_IN		},		/* LIN_UART �V���A���o��ch.2 �����P�[�V����1 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		EPFR03_TIN1E_10,	D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				RELOAD_TIMER_EVENT_IN	},		/* �����[�h�^�C�}�C�x���g����ch.1 �����P�[�V����2 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX1,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG �o��ch.1 �����P�[�V����0 */
	{ D_LOW_PXX1,		D_HIGH_PXX1,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR06_SOT2E_0X,	D_NULL,		EPFR10_PPG1E_XXX0,	D_NULL,		D_NULL,		D_NULL,		EPFR43_INT1E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.1 �����P�[�V����1 */
};

/* �wP002�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P002[PORT_FUNC_CHANGE_ID_NUM_11][PORT_FUNC_CHANGE0] =
{
	/* DDR00			PFR00			EPFR02		EPFR03		EPFR04				EPFR05		EPFR06				EPFR07		EPFR10		EPFR11				EPFR12		EPFR13		EPFR43				EPFR47					ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_10,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SCK_IN			},		/* LIN_UART �V���A���N���b�N����ch.2 �����P�[�V����1 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_10,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SCK_OUT		},		/* LIN_UART �V���A���N���b�N�o��ch.2 �����P�[�V����1 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		EPFR04_TIN2E_10,	D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				RELOAD_TIMER_EVENT_IN	},		/* �����[�h�^�C�}�C�x���g����ch.2 �����P�[�V����2 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX1,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG �o��ch.1 �����P�[�V����0 */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR06_SCK2E_0X,	D_NULL,		D_NULL,		EPFR11_PPG2E_XX0,	D_NULL,		D_NULL,		EPFR43_INT2E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.2 �����P�[�V����1 */
};

/* �wP003�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P003[PORT_FUNC_CHANGE_ID_NUM_10][PORT_FUNC_CHANGE0] =
{
	/* DDR00 			PFR00			EPFR02		EPFR03		EPFR04		EPFR05				EPFR06		EPFR07				EPFR10		EPFR11				EPFR12		EPFR13		EPFR43				EPFR47					ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR07_SIN3E_HIGH,	D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SERIAL_IN		},		/* LIN_UART �V���A������ch.3 �����P�[�V����1 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TIN3E_10,	D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				RELOAD_TIMER_EVENT_IN	},		/* �����[�h�^�C�}�C�x���g����ch.3 �����P�[�V����2 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX1,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG �o��ch.3 �����P�[�V����0 */
	{ D_LOW_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		EPFR11_PPG3E_XX0,	D_NULL,		D_NULL,		EPFR43_INT3E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.3 �����P�[�V����1 */
};

/* �wP004�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P004[PORT_FUNC_CHANGE_ID_NUM_10][PORT_FUNC_CHANGE0] =
{
	/* DDR00 			PFR00			EPFR02				EPFR03		EPFR04		EPFR05		EPFR06		EPFR07				EPFR10		EPFR11		EPFR12				EPFR13		EPFR43				EPFR47					ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX4,		D_HIGH_PXX4,	EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_1X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SERIAL_OUT		},		/* LIN_UART �V���A���o��ch.3 �����P�[�V����1 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR02_TOT0E_1XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				RELOAD_TIMER_EVENT_IN	},		/* �����[�h�^�C�}�o��ch.0 �����P�[�V����2 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX1,	D_NULL,		NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG �o��ch.4 �����P�[�V����0 */
	{ D_LOW_PXX4,		D_HIGH_PXX4,	EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR02_TOT0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG4E_XX0,	D_NULL,		EPFR43_INT4E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.4 �����P�[�V����1 */
};

/* �wP005�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P005[PORT_FUNC_CHANGE_ID_NUM_11][PORT_FUNC_CHANGE0] =
{
	/* DDR00 			PFR00			EPFR02		EPFR03				EPFR04		EPFR05		EPFR06		EPFR07				EPFR10		EPFR11		EPFR12				EPFR13		EPFR43				EPFR47					ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_LOW_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_10,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SCK_IN			},		/* LIN_UART �V���A���N���b�N����ch.3 �����P�[�V����1 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_10,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				LIN_UART_SCK_OUT		},		/* LIN_UART �V���A���N���b�N�o��ch.3 �����P�[�V����1 */	
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_1XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			NO_MASK,				RELOAD_TIMER_EVENT_IN	},		/* �����[�h�^�C�}�o��ch.1 �����P�[�V����2 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX1,	D_NULL,		NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG �o��ch.5 �����P�[�V����0 */
	{ D_LOW_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		EPFR03_TOT1E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SCK3E_0X,	D_NULL,		D_NULL,		EPFR12_PPG5E_XX0,	D_NULL,		EPFR43_INT5E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.5 �����P�[�V����1 */
};

/* �wP006�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P006[PORT_FUNC_CHANGE_ID_NUM_9][PORT_FUNC_CHANGE0] =
{
	/* DDR00 			PFR00			EPFR02		EPFR03		EPFR04				EPFR05		EPFR06		EPFR07		EPFR10		EPFR11		EPFR12		EPFR13				EPFR43				EPFR47					ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		EPFR04_TOT2E_1XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	NO_MASK,			NO_MASK,				RELOAD_TIMER_OUT		},		/* �����[�h�^�C�}�o��ch.2 �����P�[�V����2 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX1,	NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG �o��ch.6 �����P�[�V����0 */
	{ D_LOW_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		EPFR04_TOT2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG6E_XX0,	EPFR43_INT6E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.6 �����P�[�V����1 */
};

/* �wP007�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P007[PORT_FUNC_CHANGE_ID_NUM_9][PORT_FUNC_CHANGE0] =
{
	/* DDR00 			PFR00			EPFR02		EPFR03		EPFR04		EPFR05				EPFR06		EPFR07		EPFR10		EPFR11		EPFR12		EPFR13				EPFR43				EPFR47					ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_1XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	NO_MASK,			NO_MASK,				RELOAD_TIMER_OUT		},		/* �����[�h�^�C�}�o��ch.3 �����P�[�V����2 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX1,	NO_MASK,			NO_MASK,				PPG_OUT					},		/* PPG �o��ch.7 �����P�[�V����0 */
	{ D_LOW_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR13_PPG7E_XX0,	EPFR43_INT7E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.7 �����P�[�V����1 */
};

/* �wP010�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P010[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					EPFR44				EPFR47					ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR16_SEG0E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		EPFR16_SEG0E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX0,		D_HIGH_PXX0,	EPFR16_SEG0E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR16_SEG0E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR16_SEG0E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_LOW_PXX0,		D_HIGH_PXX0,	EPFR16_SEG0E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR16_SEG0E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR16_SEG0E_LOW,		EPFR44_INT8E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.8 �����P�[�V����1 */	
};

/* �wP011�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P011[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					PFR44				EPFR47					ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR16_SEG1E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		EPFR16_SEG1E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX1,		D_HIGH_PXX1,	EPFR16_SEG1E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR16_SEG1E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR16_SEG1E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_LOW_PXX1,		D_HIGH_PXX1,	EPFR16_SEG1E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR16_SEG1E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR16_SEG1E_LOW,		EPFR44_INT9E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.9 �����P�[�V����1 */	
};

/* �wP012�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P012[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					PFR44				EPFR47					ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR16_SEG2E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		EPFR16_SEG2E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	EPFR16_SEG2E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR16_SEG2E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR16_SEG2E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	EPFR16_SEG2E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR16_SEG2E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR16_SEG2E_LOW,		EPFR44_INT10E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.10 �����P�[�V����1 */	
};

/* �wP013�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P013[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					PFR44				EPFR47					ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR16_SEG3E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		EPFR16_SEG3E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX3,		D_HIGH_PXX3,	EPFR16_SEG3E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR16_SEG3E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR16_SEG3E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_LOW_PXX3,		D_HIGH_PXX3,	EPFR16_SEG3E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���o�� 32bit */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR16_SEG3E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^���o�� 32bit */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR16_SEG3E_LOW,		EPFR44_INT11E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.11 �����P�[�V����1 */	
};

/* �wP014�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P014[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					PFR44				EPFR47					ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR16_SEG4E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		EPFR16_SEG5E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX4,		D_HIGH_PXX4,	EPFR16_SEG4E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR16_SEG4E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR16_SEG4E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_LOW_PXX4,		D_HIGH_PXX4,	EPFR16_SEG4E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR16_SEG4E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR16_SEG4E_LOW,		EPFR44_INT12E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.12 �����P�[�V����1 */	
};

/* �wP015�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P015[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					PFR44				EPFR47					ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR16_SEG5E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		EPFR16_SEG5E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX5,		D_HIGH_PXX5,	EPFR16_SEG5E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR16_SEG5E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR16_SEG5E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_LOW_PXX5,		D_HIGH_PXX5,	EPFR16_SEG5E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR16_SEG5E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR16_SEG5E_LOW,		EPFR44_INT13E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.13 �����P�[�V����1 */	
};

/* �wP016�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P016[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					PFR44				EPFR47					ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR16_SEG6E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		EPFR16_SEG6E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX6,		D_HIGH_PXX6,	EPFR16_SEG6E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR16_SEG6E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR16_SEG6E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_LOW_PXX6,		D_HIGH_PXX6,	EPFR16_SEG6E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR16_SEG6E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR16_SEG6E_LOW,		EPFR44_INT14E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.14 �����P�[�V����1 */	
};

/* �wP017�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P017[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE1] =
{
	/* DDR01 			PFR01			EPFR16					PFR44				EPFR47					ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR16_SEG7E_LOW,		NO_MASK,			NO_MASK,				PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		EPFR16_SEG7E_LOW,		NO_MASK,			NO_MASK,				PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX7,		D_HIGH_PXX7,	EPFR16_SEG7E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_IN		},		/* �O���o�X�f�[�^���� 16bit */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR16_SEG7E_LOW,		NO_MASK,			EPFR47_EBDSWPE_LOW,		BUS_DATA_16BIT_OUT		},		/* �O���o�X�f�[�^�o�� 16bit */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR16_SEG7E_HIGH,		NO_MASK,			NO_MASK,				LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_LOW_PXX7,		D_HIGH_PXX7,	EPFR16_SEG7E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_IN		},		/* �O���o�X�f�[�^���� 32bit */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR16_SEG7E_LOW,		NO_MASK,			EPFR47_EBDSWPE_HIGH,	BUS_DATA_32BIT_OUT		},		/* �O���o�X�f�[�^�o�� 32bit */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR16_SEG7E_LOW,		EPFR44_INT15E_HIGH,	NO_MASK,				INTTERUPT_REQUEST_IN	}		/* �O�������ݗv������ch.15 �����P�[�V����1 */	
};

/* �wP020�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P020[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE2] =
{
	/* DDR02 		PFR02			EPFR01				EPFR17				EPFR29				EPFR32		EPFR33		EPFR39		ID */
	{ D_LOW_PXX0,	D_LOW_PXX0,		NO_MASK,			EPFR17_SEG8E_LOW,	EPFR29_OCU0E_0X,	D_NULL,		D_NULL,		D_NULL,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX0,	D_LOW_PXX0,		NO_MASK,			EPFR17_SEG8E_LOW,	EPFR29_OCU0E_0X,	D_NULL,		D_NULL,		D_NULL,		PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX0,	D_HIGH_PXX0,	NO_MASK,			EPFR17_SEG8E_LOW,	EPFR29_OCU0E_0X,	D_NULL,		D_NULL,		D_NULL,		BUS_ADDRESS_STROBE_OUT	},		/* �O���o�X�A�h���X�X�g���[�u�o�� */
	{ D_HIGH_PXX0,	D_HIGH_PXX0,	NO_MASK,			EPFR17_SEG8E_HIGH,	EPFR29_OCU0E_0X,	D_NULL,		D_NULL,		D_NULL,		LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_LOW_PXX0,	D_LOW_PXX0,		EPFR01_ICU6E_00,	EPFR17_SEG8E_LOW,	EPFR29_OCU0E_0X,	D_NULL,		D_NULL,		D_NULL,		ICU_IN					},		/* �C���v�b�g�L���v�`������ch.6 �����P�[�V����0 */
	{ D_HIGH_PXX0,	D_HIGH_PXX0,	NO_MASK,			EPFR17_SEG8E_LOW,	EPFR29_OCU0E_10,	D_NULL,		D_NULL,		D_NULL,		OCU_OUT					}		/* �A�E�g�v�b�g�R���y�A�o��ch.0 �����P�[�V����1 */
};

/* �wP021�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P021[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE2] =
{	
	/* DDR02 		PFR02			EPFR01				EPFR17				EPFR29				EPFR32		EPFR33		EPFR39		ID */
	{ D_LOW_PXX1,	D_LOW_PXX1,		NO_MASK,			EPFR17_SEG9E_LOW,	EPFR29_OCU1E_0X,	D_NULL,		D_NULL,		D_NULL,		PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX1,	D_LOW_PXX1,		NO_MASK,			EPFR17_SEG9E_LOW,	EPFR29_OCU1E_0X,	D_NULL,		D_NULL,		D_NULL,		PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX1,	D_HIGH_PXX1,	NO_MASK,			EPFR17_SEG9E_LOW,	EPFR29_OCU1E_0X,	D_NULL,		D_NULL,		D_NULL,		BUS_CHIPSET_OUT		},		/* �O���o�X�`�b�v�Z���N�g0 �o�� */
	{ D_HIGH_PXX1,	D_HIGH_PXX1,	NO_MASK,			EPFR17_SEG9E_HIGH,	EPFR29_OCU1E_0X,	D_NULL,		D_NULL,		D_NULL,		LCD_SEG_DUTY_OUT	},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_LOW_PXX1,	D_LOW_PXX1,		EPFR01_ICU7E_00,	EPFR17_SEG9E_LOW,	EPFR29_OCU1E_0X,	D_NULL,		D_NULL,		D_NULL,		ICU_IN				},		/* �C���v�b�g�L���v�`������ch.7 �����P�[�V����0 */
	{ D_HIGH_PXX1,	D_HIGH_PXX1,	NO_MASK,			EPFR17_SEG9E_LOW,	EPFR29_OCU1E_10,	D_NULL,		D_NULL,		D_NULL,		OCU_OUT				}		/* �A�E�g�v�b�g�R���y�A�o��ch.1 �����P�[�V����1 */
};

/* �wP022�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P022[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE2] =
{
	/* DDR02 		PFR02			EPFR01		EPFR17					EPFR29				EPFR32		EPFR33		EPFR39				ID */
	{ D_LOW_PXX2,	D_LOW_PXX2,		D_NULL,		EPFR17_SEG10E_LOW,		EPFR29_OCU2E_0X,	D_NULL,		D_NULL,		NO_MASK,			PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX2,	D_LOW_PXX2,		D_NULL,		EPFR17_SEG10E_LOW,		EPFR29_OCU2E_0X,	D_NULL,		D_NULL,		NO_MASK,			PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX2,	D_HIGH_PXX2,	D_NULL,		EPFR17_SEG10E_LOW,		EPFR29_OCU2E_0X,	D_NULL,		D_NULL,		NO_MASK,			BUS_CHIPSET_OUT		},		/* �O���o�X�`�b�v�Z���N�g1 �o�� */
	{ D_HIGH_PXX2,	D_HIGH_PXX2,	D_NULL,		EPFR17_SEG10E_HIGH,		EPFR29_OCU2E_0X,	D_NULL,		D_NULL,		NO_MASK,			LCD_SEG_DUTY_OUT	},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_LOW_PXX2,	D_LOW_PXX2,		D_NULL,		EPFR17_SEG10E_LOW,		EPFR29_OCU2E_0X,	D_NULL,		D_NULL,		EPFR39_ICU8E_00,	ICU_IN				},		/* �C���v�b�g�L���v�`������ch.8 �����P�[�V����0 */
	{ D_HIGH_PXX2,	D_HIGH_PXX2,	D_NULL,		EPFR17_SEG10E_LOW,		EPFR29_OCU2E_10,	D_NULL,		D_NULL,		NO_MASK,			OCU_OUT				}		/* �A�E�g�v�b�g�R���y�A�o��ch.2 �����P�[�V����1 */
};

/* �wP023�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P023[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE2] =
{
	/* DDR02 		PFR02			EPFR01		EPFR17					EPFR29				EPFR32		EPFR33		EPFR39				ID */
	{ D_LOW_PXX3,	D_LOW_PXX3,		D_NULL,		EPFR17_SEG11E_LOW,		EPFR29_OCU3E_0X,	D_NULL,		D_NULL,		NO_MASK,			PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX3,	D_LOW_PXX3,		D_NULL,		EPFR17_SEG11E_LOW,		EPFR29_OCU3E_0X,	D_NULL,		D_NULL,		NO_MASK,			PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX3,	D_HIGH_PXX3,	D_NULL,		EPFR17_SEG11E_LOW,		EPFR29_OCU3E_0X,	D_NULL,		D_NULL,		NO_MASK,			BUS_READ_STROBE_OUT	},		/* �O���o�X���[�h�X�g���[�u�o�� */
	{ D_HIGH_PXX3,	D_HIGH_PXX3,	D_NULL,		EPFR17_SEG11E_HIGH,		EPFR29_OCU3E_0X,	D_NULL,		D_NULL,		NO_MASK,			LCD_SEG_DUTY_OUT	},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_LOW_PXX3,	D_LOW_PXX3,		D_NULL,		EPFR17_SEG11E_LOW,		EPFR29_OCU3E_0X,	D_NULL,		D_NULL,		EPFR39_ICU9E_00,	ICU_IN				},		/* �C���v�b�g�L���v�`������ch.9 �����P�[�V����0 */
	{ D_HIGH_PXX3,	D_HIGH_PXX3,	D_NULL,		EPFR17_SEG11E_LOW,		EPFR29_OCU3E_10,	D_NULL,		D_NULL,		NO_MASK,			OCU_OUT				}		/* �A�E�g�v�b�g�R���y�A�o��ch.3 �����P�[�V����1 */
};

/* �wP024�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P024[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE2] =
{
	/* DDR02 		PFR02			EPFR01		EPFR17					EPFR29		EPFR32					EPFR33		EPFR39				ID */
	{ D_LOW_PXX4,	D_LOW_PXX4,		D_NULL,		EPFR17_SEG12E_LOW,		D_NULL,		EPFR32_OCU11E_X0,		D_NULL,		NO_MASK,			PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX4,	D_LOW_PXX4,		D_NULL,		EPFR17_SEG12E_LOW,		D_NULL,		EPFR32_OCU11E_X0,		D_NULL,		NO_MASK,			PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX4,	D_HIGH_PXX4,	D_NULL,		EPFR17_SEG12E_LOW,		D_NULL,		EPFR32_OCU11E_X0,		D_NULL,		NO_MASK,			BUS_WRITE_STROBE_OUT	},		/* �O���o�X���C�g�X�g���[�u0 �o�� */
	{ D_HIGH_PXX4,	D_HIGH_PXX4,	D_NULL,		EPFR17_SEG12E_HIGH,		D_NULL,		EPFR32_OCU11E_X0,		D_NULL,		NO_MASK,			LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_LOW_PXX4,	D_LOW_PXX4,		D_NULL,		EPFR17_SEG12E_LOW,		D_NULL,		EPFR32_OCU11E_X0,		D_NULL,		EPFR39_ICU10E_00,	ICU_IN					},		/* �C���v�b�g�L���v�`������ch.10 �����P�[�V����0 */
	{ D_HIGH_PXX4,	D_HIGH_PXX4,	D_NULL,		EPFR17_SEG12E_LOW,		D_NULL,		EPFR32_OCU11E_01,		D_NULL,		NO_MASK,			OCU_OUT					}		/* �A�E�g�v�b�g�R���y�A�o��ch.11 �����P�[�V����0 */
};

/* �wP025�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P025[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE2] =
{
	/* DDR02 		PFR02			EPFR01		EPFR17					EPFR29		EPFR32					EPFR33		EPFR39				ID */
	{ D_LOW_PXX5,	D_LOW_PXX5,		D_NULL,		EPFR17_SEG13E_LOW,		D_NULL,		EPFR32_OCU10E_X0,		D_NULL,		NO_MASK,			PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX5,	D_LOW_PXX5,		D_NULL,		EPFR17_SEG13E_LOW,		D_NULL,		EPFR32_OCU10E_X0,		D_NULL,		NO_MASK,			PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX5,	D_HIGH_PXX5,	D_NULL,		EPFR17_SEG13E_LOW,		D_NULL,		EPFR32_OCU10E_X0,		D_NULL,		NO_MASK,			BUS_WRITE_STROBE_OUT	},		/* �O���o�X���C�g�X�g���[�u1 �o�� */
	{ D_HIGH_PXX5,	D_HIGH_PXX5,	D_NULL,		EPFR17_SEG13E_HIGH,		D_NULL,		EPFR32_OCU10E_X0,		D_NULL,		NO_MASK,			LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_LOW_PXX5,	D_LOW_PXX5,		D_NULL,		EPFR17_SEG13E_LOW,		D_NULL,		EPFR32_OCU10E_X0,		D_NULL,		EPFR39_ICU11E_00,	ICU_IN					},		/* �C���v�b�g�L���v�`������ch.11 �����P�[�V����0 */
	{ D_HIGH_PXX5,	D_HIGH_PXX5,	D_NULL,		EPFR17_SEG13E_LOW,		D_NULL,		EPFR32_OCU10E_01,		D_NULL,		NO_MASK,			OCU_OUT					}		/* �A�E�g�v�b�g�R���y�A�o��ch.10 �����P�[�V����0 */
};

/* �wP026�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P026[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE2] =
{
	/* DDR02 		PFR02			EPFR01		EPFR17					EPFR29		EPFR32					EPFR33				EPFR39		ID */
	{ D_LOW_PXX6,	D_LOW_PXX6,		D_NULL,		EPFR17_SEG14E_LOW,		D_NULL,		EPFR32_OCU9E_X0,		NO_MASK,			D_NULL,		PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX6,	D_LOW_PXX6,		D_NULL,		EPFR17_SEG14E_LOW,		D_NULL,		EPFR32_OCU9E_X0,		NO_MASK,			D_NULL,		PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX6,	D_HIGH_PXX6,	D_NULL,		EPFR17_SEG14E_LOW,		D_NULL,		EPFR32_OCU9E_X0,		NO_MASK,			D_NULL,		BUS_ADDRESS_OUT		},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX6,	D_HIGH_PXX6,	D_NULL,		EPFR17_SEG14E_HIGH,		D_NULL,		EPFR32_OCU9E_X0,		NO_MASK,			D_NULL,		LCD_SEG_DUTY_OUT	},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_NULL,		D_NULL,			D_NULL,		D_NULL,					D_NULL,		D_NULL,					D_NULL,				D_NULL,		NO_USE				},		/* HS_SPI SSEL3 �o�� */
	{ D_LOW_PXX6,	D_LOW_PXX6,		D_NULL,		EPFR17_SEG14E_LOW,		D_NULL,		EPFR32_OCU9E_X0,		EPFR33_SIN6E_HIGH,	D_NULL,		LIN_UART_SERIAL_IN	},		/* LIN_UART �V���A������ch.6 �����P�[�V����1 */
	{ D_HIGH_PXX6,	D_HIGH_PXX6,	D_NULL,		EPFR17_SEG14E_LOW,		D_NULL,		EPFR32_OCU9E_01,		NO_MASK,			D_NULL,		OCU_OUT				}		/* �A�E�g�v�b�g�R���y�A�o��ch.9 �����P�[�V����0 */
};

/* �wP027�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P027[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE2] =
{
	/* DDR02 		PFR02			EPFR01		EPFR17					EPFR29		EPFR32					EPFR33				EPFR39		ID */
	{ D_LOW_PXX7,	D_LOW_PXX7,		D_NULL,		EPFR17_SEG15E_LOW,		D_NULL,		EPFR32_OCU8E_X0,		EPFR33_SOT6E_0X,	D_NULL,		PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX7,	D_LOW_PXX7,		D_NULL,		EPFR17_SEG15E_LOW,		D_NULL,		EPFR32_OCU8E_X0,		EPFR33_SOT6E_0X,	D_NULL,		PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX7,	D_HIGH_PXX7,	D_NULL,		EPFR17_SEG15E_LOW,		D_NULL,		EPFR32_OCU8E_X0,		EPFR33_SOT6E_0X,	D_NULL,		BUS_ADDRESS_OUT		},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX7,	D_HIGH_PXX7,	D_NULL,		EPFR17_SEG15E_HIGH,		D_NULL,		EPFR32_OCU8E_X0,		EPFR33_SOT6E_0X,	D_NULL,		LCD_SEG_DUTY_OUT	},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_NULL,		D_NULL,			D_NULL,		D_NULL,					D_NULL,		D_NULL,					D_NULL,				D_NULL,		NO_USE				},		/* HS_SPI SSEL3 �o�� */
	{ D_HIGH_PXX7,	D_HIGH_PXX7,	D_NULL,		EPFR17_SEG15E_LOW,		D_NULL,		EPFR32_OCU8E_X0,		EPFR33_SOT6E_1X,	D_NULL,		LIN_UART_SERIAL_OUT	},		/* LIN_UART �V���A���o��ch.6 �����P�[�V����1 */
	{ D_HIGH_PXX7,	D_HIGH_PXX7,	D_NULL,		EPFR17_SEG15E_LOW,		D_NULL,		EPFR32_OCU8E_01,		EPFR33_SOT6E_0X,	D_NULL,		OCU_OUT				}		/* �A�E�g�v�b�g�R���y�A�o��ch.8 �����P�[�V����0 */
};

/* �wP030�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P030[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03		EPFR015		EPFR18					EPFR31		EPFR33				EPFR34		EPFR37		EPFR38		ID */
	{ D_LOW_PXX0,	D_LOW_PXX0,		D_NULL,		EPFR18_SEG16E_LOW,		D_NULL,		EPFR33_SCK6E_0X,	D_NULL,		D_NULL,		D_NULL,		PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX0,	D_LOW_PXX0,		D_NULL,		EPFR18_SEG16E_LOW,		D_NULL,		EPFR33_SCK6E_0X,	D_NULL,		D_NULL,		D_NULL,		PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX0,	D_HIGH_PXX0,	D_NULL,		EPFR18_SEG16E_LOW,		D_NULL,		EPFR33_SCK6E_0X,	D_NULL,		D_NULL,		D_NULL,		BUS_ADDRESS_OUT		},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX0,	D_HIGH_PXX0,	D_NULL,		EPFR18_SEG16E_HIGH,		D_NULL,		EPFR33_SCK6E_0X,	D_NULL,		D_NULL,		D_NULL,		LCD_SEG_DUTY_OUT	},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_NULL,		D_NULL,			D_NULL,		D_NULL,					D_NULL,		D_NULL,				D_NULL,		D_NULL,		D_NULL,		NO_USE				},		/* HS_SPI SCLK ���o�� */
	{ D_LOW_PXX0,	D_HIGH_PXX0,	D_NULL,		EPFR18_SEG16E_LOW,		D_NULL,		EPFR33_SCK6E_10,	D_NULL,		D_NULL,		D_NULL,		LIN_UART_SCK_IN		},		/* LIN_UART �V���A���N���b�N����ch.6 �����P�[�V����1 */
	{ D_HIGH_PXX0,	D_HIGH_PXX0,	D_NULL,		EPFR18_SEG16E_LOW,		D_NULL,		EPFR33_SCK6E_10,	D_NULL,		D_NULL,		D_NULL,		LIN_UART_SCK_OUT	}		/* LIN_UART �V���A���N���b�N�o��ch.6 �����P�[�V����1 */
};

/* �wP031�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P031[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03		EPFR015		EPFR18					EPFR31		EPFR33		EPFR34		EPFR37		EPFR38				ID */
	{ D_LOW_PXX1,	D_LOW_PXX1,		D_NULL,		EPFR18_SEG17E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX1,	D_LOW_PXX1,		D_NULL,		EPFR18_SEG17E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX1,	D_HIGH_PXX1,	D_NULL,		EPFR18_SEG17E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			BUS_ADDRESS_OUT				},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX1,	D_HIGH_PXX1,	D_NULL,		EPFR18_SEG17E_HIGH,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			LCD_SEG_DUTY_OUT			},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_NULL,		D_NULL,			D_NULL,		D_NULL,					D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,				NO_USE						},		/* HS_SPI SCLK ���o�� */
	{ D_LOW_PXX1,	D_LOW_PXX1,		D_NULL,		EPFR18_SEG17E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR38_SIN9E_LOW,	MULTI_FUNCTION_SERIAL_IN	}		/* �}���`�t�@���N�V�����V���A������ch.9 �����P�[�V����0 */
};

/* �wP032�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P032[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03			EPFR015		EPFR18					EPFR31				EPFR33		EPFR34		EPFR37		EPFR38				ID	 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		D_NULL,		EPFR18_SEG18E_LOW,		EPFR31_OCU7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SOT9E_X0,	PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		D_NULL,		EPFR18_SEG18E_LOW,		EPFR31_OCU7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SOT9E_X0,	PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		EPFR18_SEG18E_LOW,		EPFR31_OCU7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SOT9E_X0,	BUS_ADDRESS_OUT				},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		EPFR18_SEG18E_HIGH,		EPFR31_OCU7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SOT9E_X0,	LCD_SEG_DUTY_OUT			},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_NULL,			D_NULL,			D_NULL,		D_NULL,					D_NULL,				D_NULL,		D_NULL,		D_NULL,		D_NULL,				NO_USE						},		/* HS_SPI SCLK ���o�� */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		EPFR18_SEG18E_LOW,		EPFR31_OCU7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SOT9E_01,	MULTI_FUNCTION_SERIAL_OUT	},		/* �}���`�t�@���N�V�����V���A���o��ch.9 �����P�[�V����0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		EPFR18_SEG18E_LOW,		EPFR31_OCU7E_01,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SOT9E_X0,	OCU_OUT						}		/* �A�E�g�v�b�g�R���y�A�o��ch.7 �����P�[�V����0 */
};

/* �wP033�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P033[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03			EPFR015		EPFR18					EPFR31				EPFR33		EPFR34		EPFR37		EPFR38				ID	 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		EPFR18_SEG19E_LOW,		EPFR31_OCU6E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SCK9E_X0,	PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		D_NULL,		EPFR18_SEG19E_LOW,		EPFR31_OCU6E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SCK9E_X0,	PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		EPFR18_SEG19E_LOW,		EPFR31_OCU6E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SCK9E_X0,	BUS_ADDRESS_OUT			},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		EPFR18_SEG19E_HIGH,		EPFR31_OCU6E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SCK9E_X0,	LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_NULL,			D_NULL,			D_NULL,		D_NULL,					D_NULL,				D_NULL,		D_NULL,		D_NULL,		D_NULL,				NO_USE					},		/* HS_SPI SCLK ���o�� */
	{ D_LOW_PXX3,		D_HIGH_PXX3,	D_NULL,		EPFR18_SEG19E_LOW,		EPFR31_OCU6E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SCK9E_01,	MULTI_FUNCTION_SCK_IN	},		/* �}���`�t�@���N�V�����V���A���N���b�N����ch.9 �����P�[�V����0 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		EPFR18_SEG19E_LOW,		EPFR31_OCU6E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SCK9E_01,	MULTI_FUNCTION_SCK_OUT	},		/* �}���`�t�@���N�V�����V���A���N���b�N�o��ch.9 �����P�[�V����0 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		EPFR18_SEG19E_LOW,		EPFR31_OCU6E_01,	D_NULL,		D_NULL,		D_NULL,		EPFR38_SCK9E_X0,	OCU_OUT					}		/* �A�E�g�v�b�g�R���y�A�o��ch.6 �����P�[�V����0 */
};

/* �wP034�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P034[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03			EPFR015		EPFR18					EPFR31				EPFR33		EPFR34		EPFR37				EPFR38		ID	 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR18_SEG20E_LOW,		EPFR31_OCU5E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR18_SEG20E_LOW,		EPFR31_OCU5E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		EPFR18_SEG20E_LOW,		EPFR31_OCU5E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		BUS_ADDRESS_OUT				},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		EPFR18_SEG20E_HIGH,		EPFR31_OCU5E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		LCD_SEG_DUTY_OUT			},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_NULL,			D_NULL,			D_NULL,		D_NULL,					D_NULL,				D_NULL,		D_NULL,		D_NULL,				D_NULL,		NO_USE						},		/* HS_SPI SCLK ���o�� */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR18_SEG20E_LOW,		EPFR31_OCU5E_0X,	D_NULL,		D_NULL,		EPFR37_SIN8E_00,	D_NULL,		MULTI_FUNCTION_SERIAL_IN	},		/* �}���`�t�@���N�V�����V���A������ch.8 �����P�[�V����0 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		EPFR18_SEG20E_LOW,		EPFR31_OCU5E_10,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		OCU_OUT						}		/* �A�E�g�v�b�g�R���y�A�o��ch.5 �����P�[�V����1 */
};

/* �wP035�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P035[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03			EPFR015		EPFR18					EPFR31				EPFR33		EPFR34		EPFR37				EPFR38		ID	 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		EPFR18_SEG21E_LOW,		EPFR31_OCU4E_0X,	D_NULL,		D_NULL,		EPFR37_SOT8E_XX0,	D_NULL,		PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		D_NULL,		EPFR18_SEG21E_LOW,		EPFR31_OCU4E_0X,	D_NULL,		D_NULL,		EPFR37_SOT8E_XX0,	D_NULL,		PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR18_SEG21E_LOW,		EPFR31_OCU4E_0X,	D_NULL,		D_NULL,		EPFR37_SOT8E_XX0,	D_NULL,		BUS_ADDRESS_OUT				},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR18_SEG21E_HIGH,		EPFR31_OCU4E_0X,	D_NULL,		D_NULL,		EPFR37_SOT8E_XX0,	D_NULL,		LCD_SEG_DUTY_OUT			},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_NULL,			D_NULL,			D_NULL,		D_NULL,					D_NULL,				D_NULL,		D_NULL,		D_NULL,				D_NULL,		NO_USE						},		/* HS_SPI SCLK ���o�� */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR18_SEG21E_LOW,		EPFR31_OCU4E_0X,	D_NULL,		D_NULL,		EPFR37_SOT8E_001,	D_NULL,		MULTI_FUNCTION_SERIAL_OUT	},		/* �}���`�t�@���N�V�����V���A���o��ch.8 �����P�[�V����0 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		EPFR18_SEG21E_LOW,		EPFR31_OCU4E_10,	D_NULL,		D_NULL,		EPFR37_SOT8E_XX0,	D_NULL,		OCU_OUT						}		/* �A�E�g�v�b�g�R���y�A�o��ch.4 �����P�[�V����1 */
};

/* �wP036�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P036[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03			EPFR015					EPFR18					EPFR31		EPFR33		EPFR34		EPFR37				EPFR38		ID	 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR15_PPG11E_X0,		EPFR18_SEG22E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_XX0,	D_NULL,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		EPFR15_PPG11E_X0,		EPFR18_SEG22E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_XX0,	D_NULL,		PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR15_PPG11E_X0,		EPFR18_SEG22E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_XX0,	D_NULL,		BUS_ADDRESS_OUT			},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR15_PPG11E_X0,		EPFR18_SEG22E_HIGH,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_XX0,	D_NULL,		LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR15_PPG11E_01,		EPFR18_SEG22E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_XX0,	D_NULL,		PPG_OUT					},		/* PPG �o��ch.11 �����P�[�V����0 */
	{ D_NULL,			D_NULL,			D_NULL,					D_NULL,					D_NULL,		D_NULL,		D_NULL,		D_NULL,				D_NULL,		NO_USE					},		/* HS_SPI SCLK ���o�� */
	{ D_LOW_PXX6,		D_HIGH_PXX6,	EPFR15_PPG11E_X0,		EPFR18_SEG22E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_001,	D_NULL,		MULTI_FUNCTION_SCK_IN	},		/* �}���`�t�@���N�V�����V���A���N���b�N����ch.8 �����P�[�V����0 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR15_PPG11E_X0,		EPFR18_SEG22E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_001,	D_NULL,		MULTI_FUNCTION_SCK_OUT	}		/* �}���`�t�@���N�V�����V���A���N���b�N�o��ch.8 �����P�[�V����0 */
};

/* �wP037�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P037[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE3] =
{
	/* DDR03			PFR03			EPFR015					EPFR18					EPFR31		EPFR33		EPFR34				EPFR37		EPFR38		ID	 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR15_PPG12E_X0,		EPFR18_SEG23E_LOW,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		EPFR15_PPG12E_X0,		EPFR18_SEG23E_LOW,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR15_PPG12E_X0,		EPFR18_SEG23E_LOW,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		BUS_ADDRESS_OUT		},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR15_PPG12E_X0,		EPFR18_SEG23E_HIGH,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		LCD_SEG_DUTY_OUT	},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR15_PPG12E_X0,		EPFR18_SEG23E_HIGH,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		LCD_SEG_STATIC_OUT	},		/* LCDC �Z�O�����g(Static)�o�� */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR15_PPG12E_01,		EPFR18_SEG23E_LOW,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		PPG_OUT				},		/* PPG �o��ch.12 �����P�[�V����0 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR15_PPG12E_X0,		EPFR18_SEG23E_LOW,		D_NULL,		D_NULL,		EPFR34_SIN7E_LOW,	D_NULL,		D_NULL,		LIN_UART_SERIAL_IN	}		/* LIN_UART �V���A������ch.7 �����P�[�V����0 */
};

/* �wP040�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P040[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30		EPFR31		EPFR34				EPFR37		EPFR40		EPFR41		EPFR45				ID	 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR19_SEG24E_LOW,		D_NULL,		D_NULL,		EPFR34_SOT7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG13E_X0,	PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		EPFR19_SEG24E_LOW,		D_NULL,		D_NULL,		EPFR34_SOT7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG13E_X0,	PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR19_SEG24E_LOW,		D_NULL,		D_NULL,		EPFR34_SOT7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG13E_X0,	BUS_ADDRESS_OUT			},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR19_SEG24E_HIGH,		D_NULL,		D_NULL,		EPFR34_SOT7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG13E_X0,	LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR19_SEG24E_HIGH,		D_NULL,		D_NULL,		EPFR34_SOT7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG13E_X0,	LCD_SEG_STATIC_OUT		},		/* LCDC �Z�O�����g(Static)�o�� */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR19_SEG24E_LOW,		D_NULL,		D_NULL,		EPFR34_SOT7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG13E_01,	PPG_OUT					},		/* PPG �o��ch.13 �����P�[�V����0 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR19_SEG24E_LOW,		D_NULL,		D_NULL,		EPFR34_SOT7E_X1,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG13E_X0,	LIN_UART_SERIAL_OUT		}		/* LIN_UART �V���A���o��ch.7 �����P�[�V����0 */
};

/* �wP041�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P041[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30		EPFR31		EPFR34				EPFR37		EPFR40		EPFR41		EPFR45					ID	 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR19_SEG25E_LOW,		D_NULL,		D_NULL,		EPFR34_SCK7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_X0,		PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		EPFR19_SEG25E_LOW,		D_NULL,		D_NULL,		EPFR34_SCK7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_X0,		PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR19_SEG25E_LOW,		D_NULL,		D_NULL,		EPFR34_SCK7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_X0,		BUS_ADDRESS_OUT		},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR19_SEG25E_HIGH,		D_NULL,		D_NULL,		EPFR34_SCK7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_X0,		LCD_SEG_DUTY_OUT	},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR19_SEG25E_HIGH,		D_NULL,		D_NULL,		EPFR34_SCK7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_X0,		LCD_SEG_STATIC_OUT	},		/* LCDC �Z�O�����g(Static)�o�� */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR19_SEG25E_LOW,		D_NULL,		D_NULL,		EPFR34_SCK7E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_01,		PPG_OUT				},		/* PPG �o��ch.14 �����P�[�V����0 */
	{ D_LOW_PXX1,		D_HIGH_PXX1,	EPFR19_SEG25E_LOW,		D_NULL,		D_NULL,		EPFR34_SCK7E_01,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_X0,		LIN_UART_SCK_IN		},		/* LIN_UART �V���A���N���b�N����ch.7 �����P�[�V����0 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR19_SEG25E_LOW,		D_NULL,		D_NULL,		EPFR34_SCK7E_01,	D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_X0,		LIN_UART_SCK_OUT	}		/* LIN_UART �V���A���N���b�N�o��ch.7 �����P�[�V����0 */
};

/* �wP042�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P042[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30		EPFR31		EPFR34		EPFR37		EPFR40				EPFR41		EPFR45					ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR19_SEG26E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR45_PPG15E_X0,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		EPFR19_SEG26E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR45_PPG15E_X0,		PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR19_SEG26E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR45_PPG15E_X0,		BUS_ADDRESS_OUT			},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR19_SEG26E_HIGH,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR45_PPG15E_X0,		LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR19_SEG26E_HIGH,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR45_PPG15E_X0,		LCD_SEG_STATIC_OUT		},		/* LCDC �Z�O�����g(Static)�o�� */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR19_SEG26E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR45_PPG15E_01,		PPG_OUT					},		/* PPG �o��ch.15 �����P�[�V����0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR19_SEG26E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR40_AIN0E_00,	D_NULL,		EPFR45_PPG15E_X0,		UPDOWN_COUNTER_AIN_IN	}		/* �A�b�v�_�E���J�E���^AIN ����ch.0 �����P�[�V����0 */
};

/* �wP043�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P043[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30					EPFR31					EPFR34		EPFR37		EPFR40				EPFR41		EPFR45		ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR19_SEG27E_LOW,		EPFR30_SGA4E_X0,		EPFR31_OCU6E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		EPFR19_SEG27E_LOW,		EPFR30_SGA4E_X0,		EPFR31_OCU6E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR19_SEG27E_LOW,		EPFR30_SGA4E_X0,		EPFR31_OCU6E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		BUS_ADDRESS_OUT			},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR19_SEG27E_HIGH,		EPFR30_SGA4E_X0,		EPFR31_OCU6E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR19_SEG27E_HIGH,		EPFR30_SGA4E_X0,		EPFR31_OCU6E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		LCD_SEG_STATIC_OUT		},		/* LCDC �Z�O�����g(Static)�o�� */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR19_SEG27E_LOW,		EPFR30_SGA4E_X0,		EPFR31_OCU6E_0X,		D_NULL,		D_NULL,		EPFR40_BIN0E_00,	D_NULL,		D_NULL,		UPDOWN_COUNTER_BIN_IN	},		/* �A�b�v�_�E���J�E���^BIN ����ch.0 �����P�[�V����0 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR19_SEG27E_LOW,		EPFR30_SGA4E_01,		EPFR31_OCU6E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		SOUND_GENERATOR_SGA_OUT	},		/* �T�E���h�W�F�l���[�^SGA �o��ch.4 �����P�[�V����0 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR19_SEG27E_LOW,		EPFR30_SGA4E_X0,		EPFR31_OCU6E_10,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		OCU_OUT					}		/* �A�E�g�v�b�g�R���y�A�o��ch.6 �����P�[�V����1 */
};

/* �wP044�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P044[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30					EPFR31				EPFR34		EPFR37		EPFR40				EPFR41		EPFR45		ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR19_SEG28E_LOW,		EPFR30_SGO4E_X0,		EPFR31_OCU7E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		EPFR19_SEG28E_LOW,		EPFR30_SGO4E_X0,		EPFR31_OCU7E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR19_SEG28E_LOW,		EPFR30_SGO4E_X0,		EPFR31_OCU7E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		BUS_ADDRESS_OUT			},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR19_SEG28E_HIGH,		EPFR30_SGO4E_X0,		EPFR31_OCU7E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR19_SEG28E_HIGH,		EPFR30_SGO4E_X0,		EPFR31_OCU7E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		LCD_SEG_STATIC_OUT		},		/* LCDC �Z�O�����g(Static)�o�� */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR19_SEG28E_LOW,		EPFR30_SGO4E_X0,		EPFR31_OCU7E_0X,	D_NULL,		D_NULL,		EPFR40_ZIN0E_00,	D_NULL,		D_NULL,		UPDOWN_COUNTER_ZIN_IN	},		/* �A�b�v�_�E���J�E���^ZIN ����ch.0 �����P�[�V����0 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR19_SEG28E_LOW,		EPFR30_SGO4E_01,		EPFR31_OCU7E_0X,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		SOUND_GENERATOR_SGO_OUT	},		/* �T�E���h�W�F�l���[�^SGA �o��ch.4 �����P�[�V����0 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR19_SEG28E_LOW,		EPFR30_SGO4E_X0,		EPFR31_OCU7E_10,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		OCU_OUT					}		/* �A�E�g�v�b�g�R���y�A�o��ch.7 �����P�[�V����1 */
};

/* �wP045�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P045[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30		EPFR31		EPFR34		EPFR37				EPFR40		EPFR41				EPFR45		ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR19_SEG29E_LOW,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		EPFR19_SEG29E_LOW,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR19_SEG29E_LOW,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		BUS_ADDRESS_OUT				},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR19_SEG29E_HIGH,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		LCD_SEG_DUTY_OUT			},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR19_SEG29E_HIGH,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		LCD_SEG_STATIC_OUT			},		/* LCDC �Z�O�����g(Static)�o�� */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR19_SEG29E_LOW,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR41_AIN1E_LOW,	D_NULL,		UPDOWN_COUNTER_AIN_IN		},		/* �A�b�v�_�E���J�E���^AIN ����ch.1 �����P�[�V����0 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR19_SEG29E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SIN8E_10,	D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SERIAL_IN	}		/* �}���`�t�@���N�V�����V���A������ch.8 �����P�[�V����2 */
};

/* �wP046�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P046[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30		EPFR31		EPFR34		EPFR37				EPFR40		EPFR41				EPFR45		ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR19_SEG30E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SOT8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		EPFR19_SEG30E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SOT8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR19_SEG30E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SOT8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		BUS_ADDRESS_OUT				},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR19_SEG30E_HIGH,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SOT8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		LCD_SEG_DUTY_OUT			},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR19_SEG30E_HIGH,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SOT8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		LCD_SEG_STATIC_OUT			},		/* LCDC �Z�O�����g(Static)�o�� */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR19_SEG30E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SOT8E_0XX,	D_NULL,		EPFR41_BIN1E_LOW,	D_NULL,		UPDOWN_COUNTER_BIN_IN		},		/* �A�b�v�_�E���J�E���^BIN ����ch.1 �����P�[�V����0 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR19_SEG30E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SOT8E_100,	D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SERIAL_OUT	}		/* �}���`�t�@���N�V�����V���A���o��ch.8 �����P�[�V����2 */
};

/* �wP047�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P047[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE4] =
{
	/* DDR04			PFR04			EPFR19					EPFR30		EPFR31		EPFR34		EPFR37				EPFR40		EPFR41				EPFR45		ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR19_SEG31E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		EPFR19_SEG31E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR19_SEG31E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		BUS_ADDRESS_OUT			},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR19_SEG31E_HIGH,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		LCD_SEG_DUTY_OUT		},		/* LCDC �Z�O�����g(Duty)�o�� */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR19_SEG31E_HIGH,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_0XX,	D_NULL,		NO_MASK,			D_NULL,		LCD_SEG_STATIC_OUT		},		/* LCDC �Z�O�����g(Static)�o�� */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR19_SEG31E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_0XX,	D_NULL,		EPFR41_ZIN1E_LOW,	D_NULL,		UPDOWN_COUNTER_ZIN_IN	},		/* �A�b�v�_�E���J�E���^ZIN ����ch.1 �����P�[�V����0 */
	{ D_LOW_PXX7,		D_HIGH_PXX7,	EPFR19_SEG31E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_100,	D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SCK_IN	},		/* �}���`�t�@���N�V�����V���A���N���b�N����ch.8 �����P�[�V����2 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR19_SEG31E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR37_SCK8E_100,	D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SCK_OUT	}		/* �}���`�t�@���N�V�����V���A���N���b�N�o��ch.8 �����P�[�V����2 */
};

/* �wP050�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P050[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20					EPFR28		EPFR32				ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR20_COM0E_LOW,		D_NULL,		EPFR32_OCU8E_0X,	PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		EPFR20_COM0E_LOW,		D_NULL,		EPFR32_OCU8E_0X,	PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR20_COM0E_LOW,		D_NULL,		EPFR32_OCU8E_0X,	BUS_ADDRESS_OUT				},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR20_COM0E_HIGH,		D_NULL,		EPFR32_OCU8E_0X,	LCD_SEG_DUTY_OUT_COM_OUT	},		/* LCDC �Z�O�����g(Duty)�R�����o�� */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR20_COM0E_LOW,		D_NULL,		EPFR32_OCU8E_10,	OCU_OUT						}		/* �A�E�g�v�b�g�R���y�A�o��ch.8 �����P�[�V����1 */
};

/* �wP051�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P051[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20					EPFR28		EPFR32				ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR20_COM1E_LOW,		D_NULL,		EPFR32_OCU9E_0X,	PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		EPFR20_COM1E_LOW,		D_NULL,		EPFR32_OCU9E_0X,	PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR20_COM1E_LOW,		D_NULL,		EPFR32_OCU9E_0X,	BUS_ADDRESS_OUT				},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR20_COM1E_HIGH,		D_NULL,		EPFR32_OCU9E_0X,	LCD_SEG_DUTY_OUT_COM_OUT	},		/* LCDC �Z�O�����g(Duty)�R�����o�� */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR20_COM1E_LOW,		D_NULL,		EPFR32_OCU9E_10,	OCU_OUT						}		/* �A�E�g�v�b�g�R���y�A�o��ch.9 �����P�[�V����1 */
};

/* �wP052�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P052[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20					EPFR28		EPFR32				ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR20_COM2E_LOW,		D_NULL,		EPFR32_OCU10E_0X,	PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		EPFR20_COM2E_LOW,		D_NULL,		EPFR32_OCU10E_0X,	PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR20_COM2E_LOW,		D_NULL,		EPFR32_OCU10E_0X,	BUS_ADDRESS_OUT				},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR20_COM2E_HIGH,		D_NULL,		EPFR32_OCU10E_0X,	LCD_SEG_DUTY_OUT_COM_OUT	},		/* LCDC �Z�O�����g(Duty)�R�����o�� */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR20_COM2E_LOW,		D_NULL,		EPFR32_OCU10E_10,	OCU_OUT						}		/* �A�E�g�v�b�g�R���y�A�o��ch.10 �����P�[�V����1 */
};

/* �wP053�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P053[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20					EPFR28		EPFR32				ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR20_COM3E_LOW,		D_NULL,		EPFR32_OCU11E_0X,	PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		EPFR20_COM3E_LOW,		D_NULL,		EPFR32_OCU11E_0X,	PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR20_COM3E_LOW,		D_NULL,		EPFR32_OCU11E_0X,	BUS_ADDRESS_OUT				},		/* �O���o�X�A�h���X�o�� */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR20_COM3E_HIGH,		D_NULL,		EPFR32_OCU11E_0X,	LCD_SEG_DUTY_OUT_COM_OUT	},		/* LCDC �Z�O�����g(Duty)�R�����o�� */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR20_COM3E_LOW,		D_NULL,		EPFR32_OCU11E_10,	OCU_OUT						}		/* �A�E�g�v�b�g�R���y�A�o��ch.11 �����P�[�V����1 */
};

/* �wP054�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P054[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20				EPFR28					EPFR32		ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR20_V0E_LOW,		NO_MASK,				D_NULL,		PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		EPFR20_V0E_LOW,		NO_MASK,				D_NULL,		PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR20_V0E_LOW,		NO_MASK,				D_NULL,		BUS_CLK_OUT			},		/* �O���o�X�N���b�N�o�� */
	{ D_LOW_PXX4,		D_HIGH_PXX4,	EPFR20_V0E_HIGH,	NO_MASK,				D_NULL,		LCD_V_IN			},		/* LCDC ��d��V0 ���� */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR20_V0E_LOW,		EPFR28_FRCK0E_HIGH,		D_NULL,		FREE_RUN_TIMCLK_IN	}		/* �t���[�����^�C�}�N���b�N����ch.0 �����P�[�V����1 */
};

/* �wP055�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P055[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20				EPFR28					EPFR32		ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR20_V1E_LOW,		NO_MASK,				D_NULL,		PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		EPFR20_V1E_LOW,		NO_MASK,				D_NULL,		PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR20_V1E_LOW,		NO_MASK,				D_NULL,		BUS_CHIPSET_OUT		},		/* �O���o�X�`�b�v�Z���N�g2 �o�� */
	{ D_LOW_PXX5,		D_HIGH_PXX5,	EPFR20_V1E_HIGH,	NO_MASK,				D_NULL,		LCD_V_IN			},		/* LCDC ��d��V1 ���� */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR20_V1E_LOW,		EPFR28_FRCK1E_HIGH,		D_NULL,		FREE_RUN_TIMCLK_IN	}		/* �t���[�����^�C�}�N���b�N����ch.1 �����P�[�V����1 */
};

/* �wP056�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P056[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20				EPFR28					EPFR32		ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR20_V2E_LOW,		NO_MASK,				D_NULL,		PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		EPFR20_V2E_LOW,		NO_MASK,				D_NULL,		PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR20_V2E_LOW,		NO_MASK,				D_NULL,		BUS_CHIPSET_OUT		},		/* �O���o�X�`�b�v�Z���N�g3 �o�� */
	{ D_LOW_PXX6,		D_HIGH_PXX6,	EPFR20_V2E_HIGH,	NO_MASK,				D_NULL,		LCD_V_IN			},		/* LCDC ��d��V2 ���� */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR20_V2E_LOW,		EPFR28_FRCK2E_HIGH,		D_NULL,		FREE_RUN_TIMCLK_IN	}		/* �t���[�����^�C�}�N���b�N����ch.2 �����P�[�V����1 */
};

/* �wP057�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P057[PORT_FUNC_CHANGE_ID_NUM_4][PORT_FUNC_CHANGE5] =
{
	/* DDR05			PFR05			EPFR20				EPFR28					EPFR32		ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR20_V3E_LOW,		NO_MASK,				D_NULL,		PORT_IN				},		/* �ėp���o�̓|�[�g (���͂̂� �o�͂Ȃ�) */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR20_V3E_LOW,		NO_MASK,				D_NULL,		BUS_RDY_IN			},		/* �O���o�XRDY ���� */
	{ D_LOW_PXX7,		D_HIGH_PXX7,	EPFR20_V3E_HIGH,	NO_MASK,				D_NULL,		LCD_V_IN			},		/* LCDC ��d��V3 ���� */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR20_V3E_LOW,		EPFR28_FRCK3E_HIGH,		D_NULL,		FREE_RUN_TIMCLK_IN	}		/* �t���[�����^�C�}�N���b�N����ch.3 �����P�[�V����1 */
};

/* �wP060�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P060[PORT_FUNC_CHANGE_ID_NUM_4][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35		EPFR36		EPFR38		EPFR40		EPFR41		ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR21_PWM1P0E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN			},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		EPFR21_PWM1P0E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT		},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	EPFR21_PWM1P0E_HIGH,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		SMC_OUT			},		/* SMC �o��ch.0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR21_PWM1P0E_LOW,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ADC_ANALOG_IN	}		/* ADC �A�i���O����ch.8 */
};

/* �wP061�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P061[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35		EPFR36					EPFR38		EPFR40		EPFR41		ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR21_PWM1M0E_LOW,		D_NULL,		NO_MASK,				D_NULL,		D_NULL,		D_NULL,		PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		EPFR21_PWM1M0E_LOW,		D_NULL,		NO_MASK,				D_NULL,		D_NULL,		D_NULL,		PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	EPFR21_PWM1M0E_HIGH,	D_NULL,		NO_MASK,				D_NULL,		D_NULL,		D_NULL,		SMC_OUT						},		/* SMC �o��ch.0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR21_PWM1M0E_LOW,		D_NULL,		NO_MASK,				D_NULL,		D_NULL,		D_NULL,		ADC_ANALOG_IN				},		/* ADC �A�i���O����ch.9 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR21_PWM1M0E_LOW,		D_NULL,		EPFR36_SIN1E_HIGH,		D_NULL,		D_NULL,		D_NULL,		MULTI_FUNCTION_SERIAL_IN	}		/* �}���`�t�@���N�V�����V���A������ch.1 �����P�[�V����1 */
};

/* �wP062�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P062[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35		EPFR36					EPFR38		EPFR40		EPFR41				ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR21_PWM2P0E_LOW,		D_NULL,		EPFR36_SOT1E_0X,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		EPFR21_PWM2P0E_LOW,		D_NULL,		EPFR36_SOT1E_0X,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR21_PWM2P0E_HIGH,	D_NULL,		EPFR36_SOT1E_0X,		D_NULL,		D_NULL,		NO_MASK,			SMC_OUT						},		/* SMC �o��ch.0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR21_PWM2P0E_LOW,		D_NULL,		EPFR36_SOT1E_0X,		D_NULL,		D_NULL,		NO_MASK,			ADC_ANALOG_IN				},		/* ADC �A�i���O����ch.10 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR21_PWM2P0E_LOW,		D_NULL,		EPFR36_SOT1E_0X,		D_NULL,		D_NULL,		EPFR41_ZIN1E_HIGH,	UPDOWN_COUNTER_ZIN_IN		},		/* �A�b�v�_�E���J�E���^ZIN ����ch.1 �����P�[�V����1 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	EPFR21_PWM2P0E_LOW,		D_NULL,		EPFR36_SOT1E_10,		D_NULL,		D_NULL,		NO_MASK,			MULTI_FUNCTION_SERIAL_OUT	}		/* �}���`�t�@���N�V�����V���A���o��ch.1 �����P�[�V����1 */
};

/* �wP063�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P063[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35		EPFR36					EPFR38		EPFR40		EPFR41				ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR21_PWM2M0E_LOW,		D_NULL,		EPFR36_SCK1E_0X,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		EPFR21_PWM2M0E_LOW,		D_NULL,		EPFR36_SCK1E_0X,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR21_PWM2M0E_HIGH,	D_NULL,		EPFR36_SCK1E_0X,		D_NULL,		D_NULL,		NO_MASK,			SMC_OUT					},		/* SMC �o��ch.0 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR21_PWM2M0E_LOW,		D_NULL,		EPFR36_SCK1E_0X,		D_NULL,		D_NULL,		NO_MASK,			ADC_ANALOG_IN			},		/* ADC �A�i���O����ch.11 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR21_PWM2M0E_LOW,		D_NULL,		EPFR36_SCK1E_0X,		D_NULL,		D_NULL,		EPFR41_BIN1E_HIGH,	UPDOWN_COUNTER_BIN_IN	},		/* �A�b�v�_�E���J�E���^BIN ����ch.1 �����P�[�V����1 */
	{ D_LOW_PXX3,		D_HIGH_PXX3,	EPFR21_PWM2M0E_LOW,		D_NULL,		EPFR36_SCK1E_10,		D_NULL,		D_NULL,		NO_MASK,			MULTI_FUNCTION_SCK_IN	},		/* �}���`�t�@���N�V�����V���A���N���b�N����ch.1 �����P�[�V����1 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	EPFR21_PWM2M0E_LOW,		D_NULL,		EPFR36_SCK1E_10,		D_NULL,		D_NULL,		NO_MASK,			MULTI_FUNCTION_SCK_OUT	}		/* �}���`�t�@���N�V�����V���A���N���b�N�o��ch.1 �����P�[�V����1 */
};

/* �wP064�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P064[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35				EPFR36		EPFR38		EPFR40		EPFR41				ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR21_PWM1P1E_LOW,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		EPFR21_PWM1P1E_LOW,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	EPFR21_PWM1P1E_HIGH,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			SMC_OUT						},		/* SMC �o��ch.1 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR21_PWM1P1E_LOW,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			ADC_ANALOG_IN				},		/* ADC �A�i���O����ch.12 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR21_PWM1P1E_LOW,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR41_AIN1E_HIGH,	UPDOWN_COUNTER_AIN_IN		},		/* �A�b�v�_�E���J�E���^AIN ����ch.1 �����P�[�V����1 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR21_PWM1P1E_LOW,		EPFR35_SIN0E_HIGH,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			MULTI_FUNCTION_SERIAL_IN	}		/* �}���`�t�@���N�V�����V���A������ch.0 �����P�[�V����1 */
};

/* �wP065�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P065[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35					EPFR36		EPFR38		EPFR40				EPFR41		ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR21_PWM1M1E_LOW,		EPFR35_SOT0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		EPFR21_PWM1M1E_LOW,		EPFR35_SOT0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR21_PWM1M1E_HIGH,	EPFR35_SOT0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		SMC_OUT						},		/* SMC �o��ch.1 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR21_PWM1M1E_LOW,		EPFR35_SOT0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		ADC_ANALOG_IN				},		/* ADC �A�i���O����ch.13 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR21_PWM1M1E_LOW,		EPFR35_SOT0E_0X,		D_NULL,		D_NULL,		EPFR40_ZIN0E_01,	D_NULL,		UPDOWN_COUNTER_ZIN_IN		},		/* �A�b�v�_�E���J�E���^ZIN ����ch.0 �����P�[�V����1 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	EPFR21_PWM1M1E_LOW,		EPFR35_SOT0E_10,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SERIAL_OUT	}		/* �}���`�t�@���N�V�����V���A���o��ch.0 �����P�[�V����1 */
};

/* �wP066�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P066[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35					EPFR36		EPFR38		EPFR40				EPFR41		ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR21_PWM2P1E_LOW,		EPFR35_SCK0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		EPFR21_PWM2P1E_LOW,		EPFR35_SCK0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR21_PWM2P1E_HIGH,	EPFR35_SCK0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		SMC_OUT					},		/* SMC �o��ch.1 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR21_PWM2P1E_LOW,		EPFR35_SCK0E_0X,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		ADC_ANALOG_IN			},		/* ADC �A�i���O����ch.14 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR21_PWM2P1E_LOW,		EPFR35_SCK0E_0X,		D_NULL,		D_NULL,		EPFR40_BIN0E_01,	D_NULL,		UPDOWN_COUNTER_BIN_IN	},		/* �A�b�v�_�E���J�E���^BIN ����ch.0 �����P�[�V����1 */
	{ D_LOW_PXX6,		D_HIGH_PXX6,	EPFR21_PWM2P1E_LOW,		EPFR35_SCK0E_10,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SCK_IN	},		/* �}���`�t�@���N�V�����V���A���N���b�N����ch.0 �����P�[�V����1 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	EPFR21_PWM2P1E_LOW,		EPFR35_SCK0E_10,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SCK_OUT	}		/* �}���`�t�@���N�V�����V���A���N���b�N�o��ch.0 �����P�[�V����1 */
};

/* �wP067�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P067[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE6] =
{
	/* DDR06			PFR06			EPFR21					EPFR35		EPFR36		EPFR38				EPFR40				EPFR41		ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR21_PWM2M1E_LOW,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		EPFR21_PWM2M1E_LOW,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	EPFR21_PWM2M1E_HIGH,	D_NULL,		D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		SMC_OUT						},		/* SMC �o��ch.1 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR21_PWM2M1E_LOW,		D_NULL,		D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		ADC_ANALOG_IN				},		/* ADC �A�i���O����ch.15 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR21_PWM2M1E_LOW,		D_NULL,		D_NULL,		NO_MASK,			EPFR40_AIN0E_01,	D_NULL,		UPDOWN_COUNTER_AIN_IN		},		/* �A�b�v�_�E���J�E���^AIN ����ch.0 �����P�[�V����1 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		EPFR21_PWM2M1E_LOW,		D_NULL,		D_NULL,		EPFR38_SIN9E_HIGH,	NO_MASK,			D_NULL,		MULTI_FUNCTION_SERIAL_IN	}		/* �}���`�t�@���N�V�����V���A������ch.9 �����P�[�V����1 */
};

/* �wP070�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P070[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07			EPFR01		EPFR15		EPFR22					EPFR34		EPFR37		EPFR38					EPFR39		EPFR45		ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR22_PWM1P2E_LOW,		D_NULL,		D_NULL,		EPFR38_SOT9E_0X,		D_NULL,		D_NULL,		PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR22_PWM1P2E_LOW,		D_NULL,		D_NULL,		EPFR38_SOT9E_0X,		D_NULL,		D_NULL,		PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		D_NULL,		EPFR22_PWM1P2E_HIGH,	D_NULL,		D_NULL,		EPFR38_SOT9E_0X,		D_NULL,		D_NULL,		SMC_OUT						},		/* SMC �o��ch.2 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR22_PWM1P2E_LOW,		D_NULL,		D_NULL,		EPFR38_SOT9E_0X,		D_NULL,		D_NULL,		ADC_ANALOG_IN				},		/* ADC �A�i���O����ch.16 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		D_NULL,		EPFR22_PWM1P2E_LOW,		D_NULL,		D_NULL,		EPFR38_SOT9E_10,		D_NULL,		D_NULL,		MULTI_FUNCTION_SERIAL_OUT	}		/* �}���`�t�@���N�V�����V���A���o��ch.9 �����P�[�V����1 */
};

/* �wP071�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P071[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07				EPFR01		EPFR15		EPFR22					EPFR34		EPFR37		EPFR38					EPFR39		EPFR45		ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,			D_NULL,		D_NULL,		EPFR22_PWM1M2E_LOW,		D_NULL,		D_NULL,		EPFR38_SCK9E_0X,		D_NULL,		D_NULL,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX1,		D_LOW_PXX1,			D_NULL,		D_NULL,		EPFR22_PWM1M2E_LOW,		D_NULL,		D_NULL,		EPFR38_SCK9E_0X,		D_NULL,		D_NULL,		PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,		D_NULL,		D_NULL,		EPFR22_PWM1M2E_HIGH,	D_NULL,		D_NULL,		EPFR38_SCK9E_0X,		D_NULL,		D_NULL,		SMC_OUT					},		/* SMC �o��ch.2 */
	{ D_LOW_PXX1,		D_LOW_PXX1,			D_NULL,		D_NULL,		EPFR22_PWM1M2E_LOW,		D_NULL,		D_NULL,		EPFR38_SCK9E_0X,		D_NULL,		D_NULL,		ADC_ANALOG_IN			},		/* ADC �A�i���O����ch.17 */
	{ D_LOW_PXX1,		D_HIGH_PXX1,		D_NULL,		D_NULL,		EPFR22_PWM1M2E_LOW,		D_NULL,		D_NULL,		EPFR38_SCK9E_10,		D_NULL,		D_NULL,		MULTI_FUNCTION_SCK_IN	},		/* �}���`�t�@���N�V�����V���A���N���b�N����ch.9 �����P�[�V����1 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,		D_NULL,		D_NULL,		EPFR22_PWM1M2E_LOW,		D_NULL,		D_NULL,		EPFR38_SCK9E_10,		D_NULL,		D_NULL,		MULTI_FUNCTION_SCK_OUT	}		/* �}���`�t�@���N�V�����V���A���N���b�N�o��ch.9 �����P�[�V����1 */
};

/* �wP072�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P072[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07				EPFR01		EPFR15		EPFR22					EPFR34		EPFR37				EPFR38		EPFR39					EPFR45		ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR22_PWM2P2E_LOW,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,				D_NULL,		PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR22_PWM2P2E_LOW,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,				D_NULL,		PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,		D_NULL,		D_NULL,		EPFR22_PWM2P2E_HIGH,	D_NULL,		NO_MASK,			D_NULL,		NO_MASK,				D_NULL,		SMC_OUT						},		/* SMC �o��ch.2 */
	{ D_LOW_PXX2,		D_LOW_PXX2	,		D_NULL,		D_NULL,		EPFR22_PWM2P2E_LOW,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,				D_NULL,		ADC_ANALOG_IN				},		/* ADC �A�i���O����ch.18 */
	{ D_LOW_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR22_PWM2P2E_LOW,		D_NULL,		NO_MASK,			D_NULL,		EPFR39_ICU11E_01,		D_NULL,		ICU_IN						},		/* �C���v�b�g�L���v�`������ch.11 �����P�[�V����1 */
	{ D_LOW_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR22_PWM2P2E_LOW,		D_NULL,		EPFR37_SIN8E_01,	D_NULL,		NO_MASK,				D_NULL,		MULTI_FUNCTION_SERIAL_IN	}		/* �}���`�t�@���N�V�����V���A������ch.8 �����P�[�V����1 */
};

/* �wP073�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P073[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07				EPFR01		EPFR15		EPFR22					EPFR34		EPFR37					EPFR38		EPFR39					EPFR45		ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,			D_NULL,		D_NULL,		EPFR22_PWM2M2E_LOW,		D_NULL,		EPFR37_SOT8E_X0X,		D_NULL,		NO_MASK,				D_NULL,		PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX3,		D_LOW_PXX3,			D_NULL,		D_NULL,		EPFR22_PWM2M2E_LOW,		D_NULL,		EPFR37_SOT8E_X0X,		D_NULL,		NO_MASK,				D_NULL,		PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,		D_NULL,		D_NULL,		EPFR22_PWM2M2E_HIGH,	D_NULL,		EPFR37_SOT8E_X0X,		D_NULL,		NO_MASK,				D_NULL,		SMC_OUT						},		/* SMC �o��ch.2 */
	{ D_LOW_PXX3,		D_LOW_PXX3,			D_NULL,		D_NULL,		EPFR22_PWM2M2E_LOW,		D_NULL,		EPFR37_SOT8E_X0X,		D_NULL,		NO_MASK,				D_NULL,		ADC_ANALOG_IN				},		/* ADC �A�i���O����ch.19 */
	{ D_LOW_PXX3,		D_LOW_PXX3,			D_NULL,		D_NULL,		EPFR22_PWM2M2E_LOW,		D_NULL,		EPFR37_SOT8E_X0X,		D_NULL,		EPFR39_ICU10E_01,		D_NULL,		ICU_IN						},		/* �C���v�b�g�L���v�`������ch.10 �����P�[�V����1 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,		D_NULL,		D_NULL,		EPFR22_PWM2M2E_LOW,		D_NULL,		EPFR37_SOT8E_010,		D_NULL,		NO_MASK,				D_NULL,		MULTI_FUNCTION_SERIAL_OUT	}		/* �}���`�t�@���N�V�����V���A���o��ch.8 �����P�[�V����1 */
};

/* �wP074�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P074[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07				EPFR01		EPFR15					EPFR22						EPFR34		EPFR37					EPFR38		EPFR39				EPFR45		ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,			D_NULL,		EPFR15_PPG12E_0X,		EPFR22_PWM1P3E_LOW,			D_NULL,		EPFR37_SCK8E_X0X,		D_NULL,		NO_MASK,			D_NULL,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX4,		D_LOW_PXX4,			D_NULL,		EPFR15_PPG12E_0X,		EPFR22_PWM1P3E_LOW,			D_NULL,		EPFR37_SCK8E_X0X,		D_NULL,		NO_MASK,			D_NULL,		PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,		D_NULL,		EPFR15_PPG12E_0X,		EPFR22_PWM1P3E_HIGH,		D_NULL,		EPFR37_SCK8E_X0X,		D_NULL,		NO_MASK,			D_NULL,		SMC_OUT					},		/* SMC �o��ch.3 */
	{ D_LOW_PXX4,		D_LOW_PXX4,			D_NULL,		EPFR15_PPG12E_0X,		EPFR22_PWM1P3E_LOW,			D_NULL,		EPFR37_SCK8E_X0X,		D_NULL,		NO_MASK,			D_NULL,		ADC_ANALOG_IN			},		/* ADC �A�i���O����ch.20 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,		D_NULL,		EPFR15_PPG12E_1X,		EPFR22_PWM1P3E_LOW,			D_NULL,		EPFR37_SCK8E_X0X,		D_NULL,		NO_MASK,			D_NULL,		PPG_OUT					},		/* PPG �o��ch.12 �����P�[�V����1 */
	{ D_LOW_PXX4,		D_LOW_PXX4,			D_NULL,		EPFR15_PPG12E_0X,		EPFR22_PWM1P3E_LOW,			D_NULL,		EPFR37_SCK8E_X0X,		D_NULL,		EPFR39_ICU9E_01,	D_NULL,		ICU_IN					},		/* �C���v�b�g�L���v�`������ch.9 �����P�[�V����1 */
	{ D_LOW_PXX4,		D_HIGH_PXX4,		D_NULL,		EPFR15_PPG12E_0X,		EPFR22_PWM1P3E_LOW,			D_NULL,		EPFR37_SCK8E_010,		D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SCK_IN	},		/* �}���`�t�@���N�V�����V���A���N���b�N����ch.8 �����P�[�V����1 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,		D_NULL,		EPFR15_PPG12E_0X,		EPFR22_PWM1P3E_LOW,			D_NULL,		EPFR37_SCK8E_010,		D_NULL,		NO_MASK,			D_NULL,		MULTI_FUNCTION_SCK_OUT	},		/* �}���`�t�@���N�V�����V���A���N���b�N�o��ch.8 �����P�[�V����1 */
};

/* �wP075�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P075[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07				EPFR01		EPFR15		EPFR22					EPFR34				EPFR37		EPFR38		EPFR39				EPFR45					ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,			D_NULL,		D_NULL,		EPFR22_PWM1M3E_LOW,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			EPFR45_PPG13E_0X,		PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX5,		D_LOW_PXX5,			D_NULL,		D_NULL,		EPFR22_PWM1M3E_LOW,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			EPFR45_PPG13E_0X,		PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,		D_NULL,		D_NULL,		EPFR22_PWM1M3E_HIGH,	NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			EPFR45_PPG13E_0X,		SMC_OUT				},		/* SMC �o��ch.3 */
	{ D_LOW_PXX5,		D_LOW_PXX5,			D_NULL,		D_NULL,		EPFR22_PWM1M3E_LOW,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			EPFR45_PPG13E_0X,		ADC_ANALOG_IN		},		/* ADC �A�i���O����ch.21 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,		D_NULL,		D_NULL,		EPFR22_PWM1M3E_LOW,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			EPFR45_PPG13E_1X,		PPG_OUT				},		/* PPG �o��ch.13 �����P�[�V����1 */
	{ D_LOW_PXX5,		D_LOW_PXX5,			D_NULL,		D_NULL,		EPFR22_PWM1M3E_LOW,		NO_MASK,			D_NULL,		D_NULL,		EPFR39_ICU8E_01,	EPFR45_PPG13E_0X,		ICU_IN				},		/* �C���v�b�g�L���v�`������ch.8 �����P�[�V����1 */
	{ D_LOW_PXX5,		D_LOW_PXX5,			D_NULL,		D_NULL,		EPFR22_PWM1M3E_LOW,		EPFR34_SIN7E_HIGH,	D_NULL,		D_NULL,		NO_MASK,			EPFR45_PPG13E_0X,		LIN_UART_SERIAL_IN	}		/* LIN_UART �V���A������ch.7 �����P�[�V����1 */
};

/* �wP076�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P076[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07				EPFR01				EPFR15		EPFR22					EPFR34					EPFR37		EPFR38		EPFR39		EPFR45					ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,			NO_MASK,			D_NULL,		EPFR22_PWM2P3E_LOW,		EPFR34_SOT7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_0X,		PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX6,		D_LOW_PXX6,			NO_MASK,			D_NULL,		EPFR22_PWM2P3E_LOW,		EPFR34_SOT7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_0X,		PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,		NO_MASK,			D_NULL,		EPFR22_PWM2P3E_HIGH,	EPFR34_SOT7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_0X,		SMC_OUT				},		/* SMC �o��ch.3 */
	{ D_LOW_PXX6,		D_LOW_PXX6,			NO_MASK,			D_NULL,		EPFR22_PWM2P3E_LOW,		EPFR34_SOT7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_0X,		ADC_ANALOG_IN		},		/* ADC �A�i���O����ch.22 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,		NO_MASK,			D_NULL,		EPFR22_PWM2P3E_LOW,		EPFR34_SOT7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_1X,		PPG_OUT				},		/* PPG �o��ch.14 �����P�[�V����1 */
	{ D_LOW_PXX6,		D_LOW_PXX6,			EPFR01_ICU7E_01,	D_NULL,		EPFR22_PWM2P3E_LOW,		EPFR34_SOT7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_0X,		ICU_IN				},		/* �C���v�b�g�L���v�`������ch.7 �����P�[�V����1 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,		NO_MASK,			D_NULL,		EPFR22_PWM2P3E_LOW,		EPFR34_SOT7E_1X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG14E_0X,		LIN_UART_SERIAL_OUT	}		/* LIN_UART �V���A���o��ch.7 �����P�[�V����1 */
};

/* �wP077�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P077[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE7] =
{
	/* DDR07			PFR07				EPFR01				EPFR15		EPFR22					EPFR34					EPFR37		EPFR38		EPFR39		EPFR45					ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,			NO_MASK,			D_NULL,		EPFR22_PWM2M3E_LOW,		EPFR34_SCK7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_0X,		PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX7,		D_LOW_PXX7,			NO_MASK,			D_NULL,		EPFR22_PWM2M3E_LOW,		EPFR34_SCK7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_0X,		PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,		NO_MASK,			D_NULL,		EPFR22_PWM2M3E_HIGH,	EPFR34_SCK7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_0X,		SMC_OUT				},		/* SMC �o��ch.3 */
	{ D_LOW_PXX7,		D_LOW_PXX7,			NO_MASK,			D_NULL,		EPFR22_PWM2M3E_LOW,		EPFR34_SCK7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_0X,		ADC_ANALOG_IN		},		/* ADC �A�i���O����ch.23 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,		NO_MASK,			D_NULL,		EPFR22_PWM2M3E_LOW,		EPFR34_SCK7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_1X,		PPG_OUT				},		/* PPG �o��ch.15 �����P�[�V����1 */
	{ D_LOW_PXX7,		D_LOW_PXX7,			EPFR01_ICU6E_01,	D_NULL,		EPFR22_PWM2M3E_LOW,		EPFR34_SCK7E_0X,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_0X,		ICU_IN				},		/* �C���v�b�g�L���v�`������ch.6 �����P�[�V����1 */
	{ D_LOW_PXX7,		D_HIGH_PXX7,		NO_MASK,			D_NULL,		EPFR22_PWM2M3E_LOW,		EPFR34_SCK7E_10,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_0X,		LIN_UART_SCK_IN		},		/* LIN_UART �V���A���N���b�N����ch.7 �����P�[�V����1 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,		NO_MASK,			D_NULL,		EPFR22_PWM2M3E_LOW,		EPFR34_SCK7E_10,		D_NULL,		D_NULL,		D_NULL,		EPFR45_PPG15E_0X,		LIN_UART_SCK_OUT	}		/* LIN_UART �V���A���N���b�N�o��ch.7 �����P�[�V����1 */
};

/* �wP080�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P080[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08				EPFR00		EPFR01		EPFR23					EPFR33				EPFR40				EPFR45					EPFR46		ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,			D_NULL,		D_NULL,		EPFR23_PWM1P4E_LOW,		NO_MASK,			NO_MASK,			EPFR45_PPG16E_LOW,		D_NULL,		PORT_IN 				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX0,		D_LOW_PXX0,			D_NULL,		D_NULL,		EPFR23_PWM1P4E_LOW,		NO_MASK,			NO_MASK,			EPFR45_PPG16E_LOW,		D_NULL,		PORT_OUT 				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,		D_NULL,		D_NULL,		EPFR23_PWM1P4E_HIGH,	NO_MASK,			NO_MASK,			EPFR45_PPG16E_LOW,		D_NULL,		SMC_OUT 				},		/* SMC �o��ch.4 */
	{ D_LOW_PXX0,		D_LOW_PXX0,			D_NULL,		D_NULL,		EPFR23_PWM1P4E_LOW,		NO_MASK,			NO_MASK,			EPFR45_PPG16E_LOW,		D_NULL,		ADC_ANALOG_IN			},		/* ADC �A�i���O����ch.24 */
	{ D_LOW_PXX0,		D_LOW_PXX0,			D_NULL,		D_NULL,		EPFR23_PWM1P4E_LOW,		EPFR33_SIN6E_LOW,	NO_MASK,			EPFR45_PPG16E_LOW,		D_NULL,		LIN_UART_SERIAL_IN		},		/* LIN_UART �V���A������ch.6 �����P�[�V����0 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,		D_NULL,		D_NULL,		EPFR23_PWM1P4E_LOW,		NO_MASK,			NO_MASK,			EPFR45_PPG16E_HIGH,		D_NULL,		PPG_OUT					},		/* PPG �o��ch.16 �����P�[�V����0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,			D_NULL,		D_NULL,		EPFR23_PWM1P4E_LOW,		NO_MASK,			EPFR40_AIN0E_10,	EPFR45_PPG16E_LOW,		D_NULL,		UPDOWN_COUNTER_AIN_IN	}		/* �A�b�v�_�E���J�E���^AIN ����ch.0 �����P�[�V����2 */
};

/* �wP081�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P081[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08				EPFR00		EPFR01		EPFR23					EPFR33				EPFR40				EPFR45					EPFR46		ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,			D_NULL,		D_NULL,		EPFR23_PWM1M4E_LOW,		EPFR33_SOT6E_X0,	NO_MASK,			EPFR45_PPG17E_LOW,		D_NULL,		PORT_IN				 	},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX1,		D_LOW_PXX1,			D_NULL,		D_NULL,		EPFR23_PWM1M4E_LOW,		EPFR33_SOT6E_X0,	NO_MASK,			EPFR45_PPG17E_LOW,		D_NULL,		PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,		D_NULL,		D_NULL,		EPFR23_PWM1M4E_HIGH,	EPFR33_SOT6E_X0,	NO_MASK,			EPFR45_PPG17E_LOW,		D_NULL,		SMC_OUT					},		/* SMC �o��ch.4 */
	{ D_LOW_PXX1,		D_LOW_PXX1,			D_NULL,		D_NULL,		EPFR23_PWM1M4E_LOW,		EPFR33_SOT6E_X0,	NO_MASK,			EPFR45_PPG17E_LOW,		D_NULL,		ADC_ANALOG_IN 			},		/* ADC �A�i���O����ch.25 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,		D_NULL,		D_NULL,		EPFR23_PWM1M4E_LOW,		EPFR33_SOT6E_X1,	NO_MASK,			EPFR45_PPG17E_LOW,		D_NULL,		LIN_UART_SERIAL_OUT		},		/* LIN_UART �V���A���o��ch.6 �����P�[�V����0 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,		D_NULL,		D_NULL,		EPFR23_PWM1M4E_LOW,		EPFR33_SOT6E_X0,	NO_MASK,			EPFR45_PPG17E_HIGH,		D_NULL,		PPG_OUT					},		/* PPG �o��ch.17 �����P�[�V����0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,			D_NULL,		D_NULL,		EPFR23_PWM1M4E_LOW,		EPFR33_SOT6E_X0,	EPFR40_BIN0E_10,	EPFR45_PPG17E_LOW,		D_NULL,		UPDOWN_COUNTER_BIN_IN	}		/* �A�b�v�_�E���J�E���^BIN ����ch.0 �����P�[�V����2 */
};

/* �wP082�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P082[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08				EPFR00		EPFR01		EPFR23					EPFR33				EPFR40				EPFR45		EPFR46					ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR23_PWM2P4E_LOW,		EPFR33_SCK6E_X0,	NO_MASK,			D_NULL,		EPFR46_PPG18E_LOW,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR23_PWM2P4E_LOW,		EPFR33_SCK6E_X0,	NO_MASK,			D_NULL,		EPFR46_PPG18E_LOW,		PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,		D_NULL,		D_NULL,		EPFR23_PWM2P4E_HIGH,	EPFR33_SCK6E_X0,	NO_MASK,			D_NULL,		EPFR46_PPG18E_LOW,		SMC_OUT					},		/* SMC �o��ch.4 */
	{ D_LOW_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR23_PWM2P4E_LOW,		EPFR33_SCK6E_X0,	NO_MASK,			D_NULL,		EPFR46_PPG18E_LOW,		ADC_ANALOG_IN			},		/* ADC �A�i���O����ch.26 */
	{ D_LOW_PXX2,		D_HIGH_PXX2,		D_NULL,		D_NULL,		EPFR23_PWM2P4E_LOW,		EPFR33_SCK6E_01,	NO_MASK,			D_NULL,		EPFR46_PPG18E_LOW,		LIN_UART_SCK_IN			},		/* LIN_UART �V���A���N���b�N����ch.6 �����P�[�V����0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,		D_NULL,		D_NULL,		EPFR23_PWM2P4E_LOW,		EPFR33_SCK6E_01,	NO_MASK,			D_NULL,		EPFR46_PPG18E_LOW,		LIN_UART_SCK_OUT		},		/* LIN_UART �V���A���N���b�N�o��ch.6 �����P�[�V����0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,		D_NULL,		D_NULL,		EPFR23_PWM2P4E_LOW,		EPFR33_SCK6E_X0,	NO_MASK,			D_NULL,		EPFR46_PPG18E_HIGH,		PPG_OUT					},		/* PPG �o��ch.18 �����P�[�V����0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,			D_NULL,		D_NULL,		EPFR23_PWM2P4E_LOW,		EPFR33_SCK6E_X0,	EPFR40_ZIN0E_10,	D_NULL,		EPFR46_PPG18E_LOW,		UPDOWN_COUNTER_ZIN_IN	}		/* �A�b�v�_�E���J�E���^ZIN ����ch.0 �����P�[�V����2 */
};

/* �wP083�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P083[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08			EPFR00				EPFR01		EPFR23					EPFR33		EPFR40		EPFR45		EPFR46					ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		EPFR23_PWM2M4E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG19E_LOW,		PORT_IN			},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		EPFR23_PWM2M4E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG19E_LOW,		PORT_OUT		},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	NO_MASK,			D_NULL,		EPFR23_PWM2M4E_HIGH,	D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG19E_LOW,		SMC_OUT			},		/* SMC �o��ch.4 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		EPFR23_PWM2M4E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG19E_LOW,		ADC_ANALOG_IN	},		/* ADC �A�i���O����ch.27 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR00_ICU0E_10,	D_NULL,		EPFR23_PWM2M4E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG19E_LOW,		ICU_IN			},		/* �C���v�b�g�L���v�`������ch.0 �����P�[�V����2 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	NO_MASK,			D_NULL,		EPFR23_PWM2M4E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG19E_HIGH,		PPG_OUT			}		/* PPG �o��ch.19 �����P�[�V����0 */
};

/* �wP084�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P084[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08			EPFR00				EPFR01		EPFR23					EPFR33		EPFR40		EPFR45		EPFR46					ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		EPFR23_PWM1P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG20E_LOW,		PORT_IN			},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		EPFR23_PWM1P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG20E_LOW,		PORT_OUT		},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	NO_MASK,			D_NULL,		EPFR23_PWM1P5E_HIGH,	D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG20E_LOW,		SMC_OUT			},		/* SMC �o��ch.5 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		EPFR23_PWM1P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG20E_LOW,		ADC_ANALOG_IN	},		/* ADC �A�i���O����ch.28 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR00_ICU1E_10,	D_NULL,		EPFR23_PWM1P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG20E_LOW,		ICU_IN			},		/* �C���v�b�g�L���v�`������ch.1 �����P�[�V����2 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	NO_MASK,			D_NULL,		EPFR23_PWM1P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG20E_HIGH,		PPG_OUT			}		/* PPG �o��ch.20 �����P�[�V����0 */
};

/* �wP085�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P085[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08				EPFR00				EPFR01		EPFR23					EPFR33		EPFR40		EPFR45		EPFR46					ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,			NO_MASK,			D_NULL,		EPFR23_PWM1M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG21E_LOW,		PORT_IN			},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX5,		D_LOW_PXX5,			NO_MASK,			D_NULL,		EPFR23_PWM1M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG21E_LOW,		PORT_OUT		},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,		NO_MASK,			D_NULL,		EPFR23_PWM1M5E_HIGH,	D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG21E_LOW,		SMC_OUT			},		/* SMC �o��ch.5 */
	{ D_LOW_PXX5,		D_LOW_PXX5,			NO_MASK,			D_NULL,		EPFR23_PWM1M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG21E_LOW,		ADC_ANALOG_IN	},		/* ADC �A�i���O����ch.29 */
	{ D_LOW_PXX5,		D_LOW_PXX5,			EPFR00_ICU2E_10,	D_NULL,		EPFR23_PWM1M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG21E_LOW,		ICU_IN			},		/* �C���v�b�g�L���v�`������ch.2 �����P�[�V����2 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,		NO_MASK,			D_NULL,		EPFR23_PWM1M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG21E_HIGH,		PPG_OUT			}		/* PPG �o��ch.21 �����P�[�V����0 */
};

/* �wP086�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P086[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08			EPFR00				EPFR01		EPFR23					EPFR33		EPFR40		EPFR45		EPFR46					ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		NO_MASK,			D_NULL,		EPFR23_PWM2P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG22E_LOW,		PORT_IN			},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX6	,	D_LOW_PXX6,		NO_MASK,			D_NULL,		EPFR23_PWM2P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG22E_LOW,		PORT_OUT		},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	NO_MASK,			D_NULL,		EPFR23_PWM2P5E_HIGH,	D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG22E_LOW,		SMC_OUT			},		/* SMC �o��ch.5 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		NO_MASK,			D_NULL,		EPFR23_PWM2P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG22E_LOW,		ADC_ANALOG_IN	},		/* ADC �A�i���O����ch.30 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		EPFR00_ICU3E_10,	D_NULL,		EPFR23_PWM2P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG22E_LOW,		ICU_IN			},		/* �C���v�b�g�L���v�`������ch.3 �����P�[�V����2 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	NO_MASK,			D_NULL,		EPFR23_PWM2P5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG22E_HIGH,		PPG_OUT			}		/* PPG �o��ch.22 �����P�[�V����0 */
};

/* �wP087�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P087[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE8] =
{
	/* DDR08			PFR08			EPFR00		EPFR01				EPFR23					EPFR33		EPFR40		EPFR45		EPFR46					ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		NO_MASK,			EPFR23_PWM2M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG23E_LOW,		PORT_IN			},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		D_NULL,		NO_MASK,			EPFR23_PWM2M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG23E_LOW,		PORT_OUT		},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		NO_MASK,			EPFR23_PWM2M5E_HIGH,	D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG23E_LOW,		SMC_OUT			},		/* SMC �o��ch.5 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		NO_MASK,			EPFR23_PWM2M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG23E_LOW,		ADC_ANALOG_IN	},		/* ADC �A�i���O����ch.30 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		EPFR01_ICU4E_10,	EPFR23_PWM2M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG23E_LOW,		ICU_IN			},		/* �C���v�b�g�L���v�`������ch.3 �����P�[�V����2 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		NO_MASK,			EPFR23_PWM2M5E_LOW,		D_NULL,		D_NULL,		D_NULL,		EPFR46_PPG23E_HIGH,		PPG_OUT			}		/* PPG �o��ch.22 �����P�[�V����0 */
};

/* �wP090�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P090[PORT_FUNC_CHANGE_ID_NUM_4][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00		EPFR01		EPFR02		EPFR04		EPFR05		EPFR06		EPFR07		EPFR10				EPFR13		EPFR14		EPFR15		EPFR24		EPFR27		EPFR44		ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN 	},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT	},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG0E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ADC_TRG_IN	},		/* ADC �O���g���K���� */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG0E_1XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT		}		/* PPG �o��ch.0 �����P�[�V����2 */
};

/* �wP091�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P091[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00				EPFR01		EPFR02		EPFR04				EPFR05		EPFR06				EPFR07		EPFR10		EPFR13					EPFR14		EPFR15		EPFR24		EPFR27					EPFR44			ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		EPFR04_TOT2E_X0X,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR13_PPG6E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_LOW,		NO_MASK,			PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		EPFR04_TOT2E_X0X,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR13_PPG6E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_LOW,		NO_MASK,			PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	NO_MASK,			D_NULL,		D_NULL,		EPFR04_TOT2E_X0X,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR13_PPG6E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_HIGH,		NO_MASK,			SOUND_GENERATOR_SGA_OUT	},		/* �T�E���h�W�F�l���[�^SGA �o��ch.0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		EPFR04_TOT2E_X0X,	D_NULL,		EPFR06_SIN2E_LOW,	D_NULL,		D_NULL,		EPFR13_PPG6E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_LOW,		NO_MASK,			LIN_UART_SERIAL_IN		},		/* LIN_UART �V���A������ch.2 �����P�[�V����0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		EPFR04_TOT2E_X0X,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR13_PPG6E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_LOW,		EPFR44_INT12E_LOW,	INTTERUPT_REQUEST_IN	},		/* �O�������ݗv������ch.12 �����P�[�V����0 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	NO_MASK,			D_NULL,		D_NULL,		EPFR04_TOT2E_X1X,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR13_PPG6E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_LOW,		NO_MASK,			RELOAD_TIMER_OUT		},		/* �����[�h�^�C�}�o��ch.2 �����P�[�V����1 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR00_ICU2E_01,	D_NULL,		D_NULL,		EPFR04_TOT2E_X0X,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR13_PPG6E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_LOW,		NO_MASK,			ICU_IN					},		/* �C���v�b�g�L���v�`������ch.2 �����P�[�V����1 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	NO_MASK,			D_NULL,		D_NULL,		EPFR04_TOT2E_X0X,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR13_PPG6E_X1X,		D_NULL,		D_NULL,		D_NULL,		EPFR27_SGA0E_LOW,		NO_MASK,			PPG_OUT					}		/* PPG �o��ch.6 �����P�[�V����1 */
};

/* �wP092�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P092[PORT_FUNC_CHANGE_ID_NUM_9][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00				EPFR01		EPFR02		EPFR04		EPFR05				EPFR06					EPFR07		EPFR10		EPFR13				EPFR14		EPFR15		EPFR24		EPFR27					EPFR44			ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_X0,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		NO_MASK,			PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_X0,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		NO_MASK,			PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_X0,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_HIGH,		NO_MASK,			SOUND_GENERATOR_SGO_OUT	},		/* �T�E���h�W�F�l���[�^SGO �o��ch.0 */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_01,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		NO_MASK,			LIN_UART_SCK_IN			},		/* LIN_UART �V���A���N���b�N����ch.2 �����P�[�V����0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_01,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		NO_MASK,			LIN_UART_SCK_OUT		},		/* LIN_UART �V���A���N���b�N�o��ch.2 �����P�[�V����0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_X0,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		EPFR44_INT13E_LOW,	INTTERUPT_REQUEST_IN	},		/* �O�������ݗv������ch.13 �����P�[�V����0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X1X,	EPFR06_SCK2E_X0,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		NO_MASK,			RELOAD_TIMER_OUT		},		/* �����[�h�^�C�}�o��ch.3 �����P�[�V����1 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR00_ICU0E_01,	D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_X0,		D_NULL,		D_NULL,		EPFR13_PPG7E_X0X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		NO_MASK,			ICU_IN					},		/* �C���v�b�g�L���v�`������ch.0 �����P�[�V����1 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_X0X,	EPFR06_SCK2E_X0,		D_NULL,		D_NULL,		EPFR13_PPG7E_X1X,	D_NULL,		D_NULL,		D_NULL,		EPFR27_SGO0E_LOW,		NO_MASK,			PPG_OUT					}		/* PPG �o��ch.7 �����P�[�V����1 */
};

/* �wP093�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P093[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00				EPFR01		EPFR02		EPFR04		EPFR05		EPFR06				EPFR07		EPFR10		EPFR13		EPFR14					EPFR15		EPFR24		EPFR27					EPFR44				ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR06_SOT2E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_X0X,		D_NULL,		D_NULL,		EPFR27_SGA1E_LOW,		NO_MASK,			PORT_IN	 					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR06_SOT2E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_X0X,		D_NULL,		D_NULL,		EPFR27_SGA1E_LOW,		NO_MASK,			PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR06_SOT2E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_X0X,		D_NULL,		D_NULL,		EPFR27_SGA1E_HIGH,		NO_MASK,			SOUND_GENERATOR_SGA_OUT		},		/* �T�E���h�W�F�l���[�^SGA �o��ch.1 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR06_SOT2E_X1,	D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_X0X,		D_NULL,		D_NULL,		EPFR27_SGA1E_LOW,		NO_MASK,			LIN_UART_SERIAL_OUT			},		/* LIN_UART �V���A���o��ch.2 �����P�[�V����0 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR06_SOT2E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_X0X,		D_NULL,		D_NULL,		EPFR27_SGA1E_LOW,		EPFR44_INT14E_LOW,	INTTERUPT_REQUEST_IN		},		/* �O�������ݗv������ch.14 �����P�[�V����0 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR00_ICU3E_01,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR06_SOT2E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_X0X,		D_NULL,		D_NULL,		EPFR27_SGA1E_LOW,		NO_MASK,			ICU_IN						},		/* �C���v�b�g�L���v�`������ch.3 �����P�[�V����1 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR06_SOT2E_X0,	D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_X1X,		D_NULL,		D_NULL,		EPFR27_SGA1E_LOW,		NO_MASK,			PPG_OUT						}		/* PPG �o��ch.8 �����P�[�V����1 */
};

/* �wP094�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P094[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00				EPFR01		EPFR02		EPFR04		EPFR05		EPFR06		EPFR07				EPFR10		EPFR13		EPFR14					EPFR15		EPFR24		EPFR27					EPFR44				ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR14_PPG9E_X0X,		D_NULL,		D_NULL,		EPFR27_SGO1E_LOW,		NO_MASK,			PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR14_PPG9E_X0X,		D_NULL,		D_NULL,		EPFR27_SGO1E_LOW,		NO_MASK,			PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR14_PPG9E_X0X,		D_NULL,		D_NULL,		EPFR27_SGO1E_HIGH,		NO_MASK,			SOUND_GENERATOR_SGO_OUT	},		/* �T�E���h�W�F�l���[�^SGO �o��ch.1 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR07_SIN3E_LOW,	D_NULL,		D_NULL,		EPFR14_PPG9E_X0X,		D_NULL,		D_NULL,		EPFR27_SGO1E_LOW,		NO_MASK,			LIN_UART_SERIAL_IN		},		/* LIN_UART �V���A������ch.3 �����P�[�V����0 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR14_PPG9E_X0X,		D_NULL,		D_NULL,		EPFR27_SGO1E_LOW,		EPFR44_INT15E_LOW,	INTTERUPT_REQUEST_IN	},		/* �O�������ݗv������ch.15 �����P�[�V����0 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR00_ICU1E_01,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR14_PPG9E_X0X,		D_NULL,		D_NULL,		EPFR27_SGO1E_LOW,		NO_MASK,			ICU_IN					},		/* �C���v�b�g�L���v�`������ch.1 �����P�[�V����1 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR14_PPG9E_X1X,		D_NULL,		D_NULL,		EPFR27_SGO1E_LOW,		NO_MASK,			PPG_OUT					}		/* PPG �o��ch.9 �����P�[�V����1 */
};

/* �wP095�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P095[PORT_FUNC_CHANGE_ID_NUM_4][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00		EPFR01		EPFR02		EPFR04		EPFR05		EPFR06		EPFR07		EPFR10		EPFR13		EPFR14		EPFR15					EPFR24					EPFR27		EPFR44		ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_X0X,		EPFR24_TX0E_LOW,		D_NULL,		D_NULL,		PORT_IN 	},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_X0X,		EPFR24_TX0E_LOW,		D_NULL,		D_NULL,		PORT_OUT	},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_X0X,		EPFR24_TX0E_HIGH,		D_NULL,		D_NULL,		CAN_TX_OUT	},		/* CAN TX �f�[�^�o��ch.0 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_X1X,		EPFR24_TX0E_LOW,		D_NULL,		D_NULL,		PPG_OUT 	}		/* PPG �o��ch.10 �����P�[�V����1 */
};

/* �wP096�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P096[PORT_FUNC_CHANGE_ID_NUM_4][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00		EPFR01		EPFR02		EPFR04		EPFR05		EPFR06		EPFR07		EPFR10		EPFR13		EPFR14		EPFR15		EPFR24		EPFR27		EPFR44				ID	 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN				 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT			 },		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			CAN_RX_IN			 },		/* CAN RX �f�[�^����ch.0 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR44_INT9E_LOW,	INTTERUPT_REQUEST_IN }		/* �O�������ݗv������ch.9 �����P�[�V����0 */
};	

/* �wP097�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P097[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE9] =
{
	/* DDR09			PFR09			EPFR00		EPFR01				EPFR02				EPFR04		EPFR05		EPFR06		EPFR07					EPFR10					EPFR13		EPFR14		EPFR15		EPFR24		EPFR27					EPFR44			ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X0,		EPFR10_PPG0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_LOW,		NO_MASK,			PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X0,		EPFR10_PPG0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_LOW,		NO_MASK,			PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X0,		EPFR10_PPG0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_HIGH,		NO_MASK,			RTC_OVERFLOW_OUT		},		/* RTC �I�[�o�t���[�o�� */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X1,		EPFR10_PPG0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_LOW,		NO_MASK,			LIN_UART_SERIAL_OUT		},		/* LIN_UART �V���A���o��ch.3 �����P�[�V����0 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X0,		EPFR10_PPG0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_LOW,		EPFR44_INT8E_LOW,	INTTERUPT_REQUEST_IN	},		/* �O�������ݗv������ch.8 �����P�[�V����0 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		NO_MASK,			EPFR02_TIN0E_00,	D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X0,		EPFR10_PPG0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_LOW,		NO_MASK,			RELOAD_TIMER_EVENT_IN	},		/* �����[�h�^�C�}�C�x���g����ch.0 �����P�[�V����0 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		EPFR01_ICU4E_01,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X0,		EPFR10_PPG0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_LOW,		NO_MASK,			ICU_IN					},		/* �C���v�b�g�L���v�`������ch.4 �����P�[�V����1 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		NO_MASK,			NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR07_SOT3E_X0,		EPFR10_PPG0E_X1X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR27_WOTE_LOW,		NO_MASK,			PPG_OUT					},		/* PPG �o��ch.0 �����P�[�V����1 */
};

/* �wP100�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P100[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01		EPFR02				EPFR03		EPFR04		EPFR05		EPFR08				EPFR09		EPFR10		EPFR11		EPFR12		EPFR14					EPFR15		EPFR30		EPFR39		EPFR42		ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT 				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR08_SIN4E_HIGH,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		LIN_UART_SERIAL_IN 		},		/* LIN_UART �V���A������ch.4 �����P�[�V����1 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ADC_ANALOG_IN			},		/* ADC �A�i���O����ch.0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		EPFR02_TIN0E_01,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		RELOAD_TIMER_EVENT_IN	},		/* �����[�h�^�C�}�C�x���g����ch.0 �����P�[�V����1 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_XX1,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT					}		/* PPG �o��ch.8 �����P�[�V����0 */
};

/* �wP101�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P101[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01		EPFR02		EPFR03				EPFR04		EPFR05		EPFR08				EPFR09		EPFR10		EPFR11		EPFR12		EPFR14					EPFR15		EPFR30		EPFR39		EPFR42		ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR08_SOT4E_0X,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR08_SOT4E_0X,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR08_SOT4E_1X,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		LIN_UART_SERIAL_IN		},		/* LIN_UART �V���A���o��ch.4 �����P�[�V����1 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR08_SOT4E_0X,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ADC_ANALOG_IN		 	},		/* ADC �A�i���O����ch.1 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		EPFR03_TIN1E_01,	D_NULL,		D_NULL,		EPFR08_SOT4E_0X,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_XX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		RELOAD_TIMER_EVENT_IN 	},		/* �����[�h�^�C�}�C�x���g����ch.1 �����P�[�V����1 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR08_SOT4E_0X,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_XX1,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT				 	}		/* PPG �o��ch.8 �����P�[�V����0 */
};

/* �wP102�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P102[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01				EPFR02		EPFR03		EPFR04				EPFR05		EPFR08					EPFR09		EPFR10		EPFR11		EPFR12		EPFR14		EPFR15					EPFR30		EPFR39		EPFR42	ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SCK4E_0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX0,		D_NULL,		D_NULL,		D_NULL,		PORT_IN 				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SCK4E_0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX0,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT 				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SCK4E_10,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX0,		D_NULL,		D_NULL,		D_NULL,		LIN_UART_SERIAL_IN  	},		/* LIN_UART �V���A������ch.4 �����P�[�V����1 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SCK4E_10,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX0,		D_NULL,		D_NULL,		D_NULL,		LIN_UART_SERIAL_OUT  	},		/* LIN_UART �V���A���o��ch.4 �����P�[�V����1 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SCK4E_0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX0,		D_NULL,		D_NULL,		D_NULL,		ADC_ANALOG_IN			},		/* ADC �A�i���O����ch.2 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		EPFR04_TIN2E_01,	D_NULL,		EPFR08_SCK4E_0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX0,		D_NULL,		D_NULL,		D_NULL,		RELOAD_TIMER_EVENT_IN	},		/* �����[�h�^�C�}�C�x���g����ch.2 �����P�[�V����1 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SCK4E_0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX1,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT					},		/* PPG �o��ch.10 �����P�[�V����0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR01_ICU6E_10,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SCK4E_0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_XX0,		D_NULL,		D_NULL,		D_NULL,		ICU_IN					}		/* �C���v�b�g�L���v�`������ch.6 �����P�[�V����2 */
};

/* �wP103�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P103[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01				EPFR02		EPFR03		EPFR04		EPFR05				EPFR08		EPFR09				EPFR10					EPFR11		EPFR12		EPFR14		EPFR15		EPFR30		EPFR39		EPFR42		ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			EPFR10_PPG1E_XX0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN 				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			EPFR10_PPG1E_XX0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT 				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR09_SIN5E_HIGH,	EPFR10_PPG1E_XX0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		LIN_UART_SERIAL_IN		},		/* LIN_UART �V���A������ch.5 �����P�[�V����1 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			EPFR10_PPG1E_XX0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ADC_ANALOG_IN 			},		/* ADC �A�i���O����ch.3 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR05_TIN3E_01,	D_NULL,		NO_MASK,			EPFR10_PPG1E_XX0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		RELOAD_TIMER_EVENT_IN	},		/* �����[�h�^�C�}�C�x���g����ch.3 �����P�[�V����1 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			EPFR10_PPG1E_XX1X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT					},		/* PPG �o��ch.1 �����P�[�V����1 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		EPFR01_ICU7E_10,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		NO_MASK,			EPFR10_PPG1E_XX0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ICU_IN					}		/* �C���v�b�g�L���v�`������ch.7 �����P�[�V����2 */
};

/* �wP104�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P104[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01		EPFR02					EPFR03		EPFR04		EPFR05		EPFR08		EPFR09					EPFR10		EPFR11					EPFR12		EPFR14		EPFR15		EPFR30		EPFR39				EPFR42		ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR02_TOT0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SOT5E_0X,		D_NULL,		EPFR11_PPG2E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_IN 			 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR02_TOT0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SOT5E_0X,		D_NULL,		EPFR11_PPG2E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_OUT			 },		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		EPFR02_TOT0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SOT5E_1X,		D_NULL,		EPFR11_PPG2E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		LIN_UART_SERIAL_OUT  },		/* LIN_UART �V���A���o��ch.5 �����P�[�V����1 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR02_TOT0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SOT5E_0X,		D_NULL,		EPFR11_PPG2E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		ADC_ANALOG_IN		 },		/* ADC �A�i���O����ch.4 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		EPFR02_TOT0E_X1X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SOT5E_0X,		D_NULL,		EPFR11_PPG2E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		RELOAD_TIMER_OUT	 },		/* �����[�h�^�C�}�o��ch.0 �����P�[�V����1 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		EPFR02_TOT0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SOT5E_0X,		D_NULL,		EPFR11_PPG2E_X1X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PPG_OUT 			 },		/* PPG �o��ch.2 �����P�[�V����1 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR02_TOT0E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SOT5E_0X,		D_NULL,		EPFR11_PPG2E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR39_ICU8E_10,	D_NULL,		ICU_IN				 }		/* �C���v�b�g�L���v�`������ ch.8 �����P�[�V����2 */
};

/* �wP105�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P105[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01		EPFR02		EPFR03					EPFR04		EPFR05		EPFR08		EPFR09					EPFR10		EPFR11					EPFR12		EPFR14		EPFR15		EPFR30		EPFR39				EPFR42		ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		EPFR03_TOT1E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_0X,		D_NULL,		EPFR11_PPG3E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_IN 		 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		EPFR03_TOT1E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_0X,		D_NULL,		EPFR11_PPG3E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PORT_OUT		 },		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX5,		D_HIGH_PXX5,	D_NULL,		D_NULL,		EPFR03_TOT1E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_10,		D_NULL,		EPFR11_PPG3E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		LIN_UART_SCK_IN  },		/* LIN_UART �V���A���N���b�N����ch.5 �����P�[�V����1 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		D_NULL,		EPFR03_TOT1E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_10,		D_NULL,		EPFR11_PPG3E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		LIN_UART_SCK_OUT },		/* LIN_UART �V���A���N���b�N�o��ch.5 �����P�[�V����1 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		EPFR03_TOT1E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_0X,		D_NULL,		EPFR11_PPG3E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		ADC_ANALOG_IN 	 },		/* ADC �A�i���O����ch.5 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		D_NULL,		EPFR03_TOT1E_X1X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_0X,		D_NULL,		EPFR11_PPG3E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		RELOAD_TIMER_OUT },		/* �����[�h�^�C�}�o��ch.1 �����P�[�V����1 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		D_NULL,		EPFR03_TOT1E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_0X,		D_NULL,		EPFR11_PPG3E_X1X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		PPG_OUT			 },		/* PPG �o��ch.3 �����P�[�V����1 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		EPFR03_TOT1E_X0X,		D_NULL,		D_NULL,		D_NULL,		EPFR09_SCK5E_0X,		D_NULL,		EPFR11_PPG3E_X0X,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR39_ICU9E_10,	D_NULL,		ICU_IN			 }		/* �C���v�b�g�L���v�`������ch.9 �����P�[�V����2 */
};

/* �wP106�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P106[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01		EPFR02		EPFR03		EPFR04		EPFR05		EPFR08		EPFR09		EPFR10		EPFR11		EPFR12					EPFR14		EPFR15		EPFR30					EPFR39				EPFR42		ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_X0X,		D_NULL,		D_NULL,		EPFR30_SGA4E_0X,		NO_MASK,			D_NULL,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_X0X,		D_NULL,		D_NULL,		EPFR30_SGA4E_0X,		NO_MASK,			D_NULL,		PORT_OUT 				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_X0X,		D_NULL,		D_NULL,		EPFR30_SGA4E_0X,		NO_MASK,			D_NULL,		ADC_ANALOG_IN			},		/* ADC �A�i���O����ch.6 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_X1X,		D_NULL,		D_NULL,		EPFR30_SGA4E_0X,		NO_MASK,			D_NULL,		PPG_OUT					},		/* PPG �o��ch.4 �����P�[�V����1 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_X0X,		D_NULL,		D_NULL,		EPFR30_SGA4E_0X,		EPFR39_ICU10E_10,	D_NULL,		ICU_IN 					},		/* �C���v�b�g�L���v�`������ch.10 �����P�[�V����2 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_X0X,		D_NULL,		D_NULL,		EPFR30_SGA4E_10,		NO_MASK,			D_NULL,		SOUND_GENERATOR_SGA_OUT }		/* �T�E���h�W�F�l���[�^SGA �o��ch.4 �����P�[�V����1 */
};

/* �wP107�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P107[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE10] =
{
	/* DDR10			PFR10			EPFR01		EPFR02		EPFR03		EPFR04		EPFR05		EPFR08		EPFR09		EPFR10		EPFR11		EPFR12					EPFR14		EPFR15		EPFR30					EPFR39				EPFR42				ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG5E_X0X,		D_NULL,		D_NULL,		EPFR30_SGO4E_0X,		NO_MASK,			EPFR42_DAS1_LOW,	PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG5E_X0X,		D_NULL,		D_NULL,		EPFR30_SGO4E_0X,		NO_MASK,			EPFR42_DAS1_LOW,	PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG5E_X0X,		D_NULL,		D_NULL,		EPFR30_SGO4E_0X,		NO_MASK,			EPFR42_DAS1_LOW,	ADC_ANALOG_IN			},		/* ADC �A�i���O����ch.7 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG5E_X1X,		D_NULL,		D_NULL,		EPFR30_SGO4E_0X,		NO_MASK,			EPFR42_DAS1_LOW,	PPG_OUT					},		/* PPG �o��ch.5 �����P�[�V����1 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG5E_X0X,		D_NULL,		D_NULL,		EPFR30_SGO4E_0X,		NO_MASK,			EPFR42_DAS1_HIGH,	DAC_OUT					},		/* DAC �o��ch.1 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG5E_X0X,		D_NULL,		D_NULL,		EPFR30_SGO4E_0X,		EPFR39_ICU11E_10,	EPFR42_DAS1_LOW,	ICU_IN					},		/* �C���v�b�g�L���v�`������ch.11 �����P�[�V����2 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG5E_X0X,		D_NULL,		D_NULL,		EPFR30_SGO4E_10,		NO_MASK,			EPFR42_DAS1_LOW,	SOUND_GENERATOR_SGO_OUT	}		/* �T�E���h�W�F�l���[�^SGO �o��ch.4 �����P�[�V����1 */
};

/* �wP110�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P110[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR01		EPFR02		EPFR03		EPFR04		EPFR05		EPFR07		EPFR08		EPFR10					EPFR11		EPFR12		EPFR24				EPFR28		EPFR30		EPFR44		ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG1E_X0XX,		D_NULL,		D_NULL,		EPFR24_TX1E_LOW,	D_NULL,		D_NULL,		D_NULL,		PORT_IN				},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG1E_X0XX,		D_NULL,		D_NULL,		EPFR24_TX1E_LOW,	D_NULL,		D_NULL,		D_NULL,		PORT_OUT			},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG1E_X0XX,		D_NULL,		D_NULL,		EPFR24_TX1E_HIGH,	D_NULL,		D_NULL,		D_NULL,		CAN_TX_OUT 			},		/* CAN TX �f�[�^�o��ch.1 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG1E_X1XX,		D_NULL,		D_NULL,		EPFR24_TX1E_LOW,	D_NULL,		D_NULL,		D_NULL,		PPG_OUT				},		/* PPG �o��ch.1 �����P�[�V����2 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR10_PPG1E_X0XX,		D_NULL,		D_NULL,		EPFR24_TX1E_LOW,	D_NULL,		D_NULL,		D_NULL,		FREE_RUN_TIMCLK_IN	}		/* �t���[�����^�C�}�N���b�N����ch.5 �����P�[�V����0 */
};

/* �wP111�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P111[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR01		EPFR02		EPFR03		EPFR04		EPFR05		EPFR07		EPFR08		EPFR10		EPFR11				EPFR12		EPFR24		EPFR28		EPFR30		EPFR44					ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,				PORT_IN				 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,				PORT_OUT			 },		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,				CAN_RX_IN			 },		/* CAN RX �f�[�^����ch.1 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG2E_0XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR44_INT10E_LOW,		INTTERUPT_REQUEST_IN },		/* �O�������ݗv������ch.10 �����P�[�V����0 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG2E_1XX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,				PPG_OUT				 }		/* PPG �o��ch.2 �����P�[�V����2 */
};

/* �wP112�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P112[PORT_FUNC_CHANGE_ID_NUM_4][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR01		EPFR02		EPFR03		EPFR04		EPFR05		EPFR07		EPFR08		EPFR10		EPFR11				EPFR12		EPFR24					EPFR28		EPFR30		EPFR44		ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG3E_0XX,	D_NULL,		EPFR24_TX2E_LOW,		D_NULL,		D_NULL,		D_NULL,		PORT_IN 	},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG3E_0XX,	D_NULL,		EPFR24_TX2E_LOW,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT 	},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG3E_0XX,	D_NULL,		EPFR24_TX2E_HIGH,		D_NULL,		D_NULL,		D_NULL,		CAN_TX_OUT	},		/* CAN TX �f�[�^�o��ch.2 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR11_PPG3E_1XX,	D_NULL,		EPFR24_TX2E_LOW,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT		}		/* PPG �o��ch.3 �����P�[�V����2 */
};

/* �wP113�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P113[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR01		EPFR02		EPFR03		EPFR04		EPFR05		EPFR07		EPFR08		EPFR10		EPFR11		EPFR12				EPFR24		EPFR28		EPFR30		EPFR44				ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN				 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT			 },		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			CAN_RX_IN			 },		/* CAN RX �f�[�^����ch.2 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR44_INT11E_LOW,	INTTERUPT_REQUEST_IN },		/* �O�������ݗv������ch.11 �����P�[�V����0 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR12_PPG4E_1XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PPG_OUT				 }		/* PPG �o��ch.4 �����P�[�V����2 */
};

/* �wP114�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P114[PORT_FUNC_CHANGE_ID_NUM_9][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR01				EPFR02		EPFR03				EPFR04		EPFR05		EPFR07				EPFR08		EPFR10		EPFR11		EPFR12		EPFR24		EPFR28		EPFR30					EPFR44		ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		PORT_IN					 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		PORT_OUT				 },		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX4,		D_HIGH_PXX4,	NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_01,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		LIN_UART_SCK_IN 		 },		/* LIN_UART �V���A���N���b�N����ch.3 �����P�[�V����0 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_01,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		LIN_UART_SCK_OUT		 },		/* LIN_UART �V���A���N���b�N�o��ch.3 �����P�[�V����0 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		EPFR03_TIN1E_00,	D_NULL,		D_NULL,		EPFR07_SCK3E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		RELOAD_TIMER_EVENT_IN	 },		/* �����[�h�^�C�}�C�x���g����ch.1 �����P�[�V����0 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		EPFR01_ICU5E_01,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		ICU_IN 				 	 },		/* �C���v�b�g�L���v�`������ch.5 �����P�[�V����1 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_HIGH,		D_NULL,		SOUND_GENERATOR_SGA_OUT	 },		/* �T�E���h�W�F�l���[�^SGA �o��ch.2 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		PPG_TRG_IN 				 },		/* PPG �g���K����3 (ch.12-ch.15) */
	{ D_LOW_PXX4,		D_LOW_PXX4,		NO_MASK,			D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR07_SCK3E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGA2E_LOW,		D_NULL,		ADC_ANALOG_IN			 }		/* ADC �A�i���O����ch.32 */
};

/* �wP115�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P115[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR0		EPFR02		EPFR03		EPFR04				EPFR05		EPFR07		EPFR08				EPFR10		EPFR11		EPFR12		EPFR24		EPFR28		EPFR30					EPFR44		ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGO2E_LOW,		D_NULL,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGO2E_LOW,		D_NULL,		PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		EPFR08_SIN4E_LOW,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGO2E_LOW,		D_NULL,		LIN_UART_SERIAL_IN 		},		/* LIN_UART �V���A������ch.4 �����P�[�V����0 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		EPFR04_TIN2E_00,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGO2E_LOW,		D_NULL,		RELOAD_TIMER_EVENT_IN 	},		/* �����[�h�^�C�}�C�x���g����ch.2 �����P�[�V����0 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGO2E_HIGH,		D_NULL,		SOUND_GENERATOR_SGO_OUT },		/* �T�E���h�W�F�l���[�^SGO �o��ch.2 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGO2E_LOW,		D_NULL,		FREE_RUN_TIMCLK_IN 		},		/* �t���[�����^�C�}�N���b�N����ch.4 �����P�[�V����0 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR30_SGO2E_LOW,		D_NULL,		ADC_ANALOG_IN 			}		/* ADC �A�i���O����ch.33 */
};

/* �wP116�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P116[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR0		EPFR02		EPFR03		EPFR04		EPFR05				EPFR07		EPFR08				EPFR10		EPFR11		EPFR12		EPFR24		EPFR28				EPFR30				EPFR44		ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SOT4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGA3E_LOW,	D_NULL,		PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SOT4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGA3E_LOW,	D_NULL,		PORT_OUT 				},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SOT4E_X1,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGA3E_LOW,	D_NULL,		LIN_UART_SERIAL_OUT		},		/* LIN_UART �V���A���o��ch.4 �����P�[�V����0 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TIN3E_00,	D_NULL,		EPFR08_SOT4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGA3E_LOW,	D_NULL,		RELOAD_TIMER_EVENT_IN 	},		/* �����[�h�^�C�}�C�x���g����ch.3 �����P�[�V����0 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SOT4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGA3E_HIGH,	D_NULL,		SOUND_GENERATOR_SGA_OUT },		/* �T�E���h�W�F�l���[�^SGA �o��ch.3 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SOT4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR28_FRCK3E_LOW,	EPFR30_SGA3E_LOW,	D_NULL,		FREE_RUN_TIMCLK_IN 		},		/* �t���[�����^�C�}�N���b�N����ch.3 �����P�[�V����0 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		EPFR08_SOT4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGA3E_LOW,	D_NULL,		ADC_ANALOG_IN 			}		/* ADC �A�i���O����ch.34 */
};

/* �wP117�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P117[PORT_FUNC_CHANGE_ID_NUM_9][PORT_FUNC_CHANGE11] =
{
	/* DDR11			PFR11			EPFR0		EPFR02				EPFR03		EPFR04		EPFR05		EPFR07		EPFR08				EPFR10		EPFR11		EPFR12		EPFR24		EPFR28				EPFR30				EPFR44		ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_LOW,	D_NULL,		PORT_IN					 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_LOW,	D_NULL,		PORT_OUT				 },		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX7,		D_HIGH_PXX7,	D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_01,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_LOW,	D_NULL,		LIN_UART_SCK_IN			 },		/* LIN_UART �V���A���N���b�N����ch.4 �����P�[�V����0 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_01,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_LOW,	D_NULL,		LIN_UART_SCK_OUT		 },		/* LIN_UART �V���A���N���b�N�o��ch.4 �����P�[�V����0 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		EPFR02_TOT0E_XX1,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_LOW,	D_NULL,		RELOAD_TIMER_OUT		 },		/* �����[�h�^�C�}�o��ch.0 �����P�[�V����0 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_HIGH,	D_NULL,		SOUND_GENERATOR_SGO_OUT  },		/* �T�E���h�W�F�l���[�^SGO �o��ch.3 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_LOW,	D_NULL,		PPG_TRG_IN 				 },		/* PPG �g���K����4 (ch.16-ch.19) */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR28_FRCK2E_LOW,	EPFR30_SGO3E_LOW,	D_NULL,		FREE_RUN_TIMCLK_IN		 },		/* �t���[�����^�C�}�N���b�N����ch.2 �����P�[�V����0 */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		EPFR02_TOT0E_XX0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR08_SCK4E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			EPFR30_SGO3E_LOW,	D_NULL,		ADC_ANALOG_IN 			 }		/* ADC �A�i���O����ch.35 */
};

/* �wP120�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P120[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE12] =
{
	/* DDR12			PFR12			EPFR00		EPFR01		EPFR03				EPFR04		EPFR05		EPFR09				EPFR12				EPFR13		EPFR14		EPFR15		EPFR28				EPFR29		EPFR31		EPFR35		EPFR42		EPFR43				ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR03_TOT1E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR12_PPG5E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN				 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR03_TOT1E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR12_PPG5E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT			 },		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR03_TOT1E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR12_PPG5E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR28_FRCK1E_LOW,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			FREE_RUN_TIMCLK_IN	 },		/* �t���[�����^�C�}�N���b�N����ch.1 �����P�[�V����0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR03_TOT1E_XX0,	D_NULL,		D_NULL,		EPFR09_SIN5E_LOW,	EPFR12_PPG5E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			LIN_UART_SERIAL_IN	 },		/* LIN_UART �V���A������ch.5 �����P�[�V����0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR03_TOT1E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR12_PPG5E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR43_INT6E_LOW,	INTTERUPT_REQUEST_IN },		/* �O�������ݗv������ch.6 �����P�[�V����0 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		D_NULL,		EPFR03_TOT1E_XX1,	D_NULL,		D_NULL,		NO_MASK,			EPFR12_PPG5E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			RELOAD_TIMER_OUT	 },		/* �����[�h�^�C�}�o��ch.1 �����P�[�V����0 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	D_NULL,		D_NULL,		EPFR03_TOT1E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR12_PPG5E_1XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PPG_OUT				 },		/* PPG �o��ch.5 �����P�[�V����2 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		D_NULL,		D_NULL,		EPFR03_TOT1E_XX0,	D_NULL,		D_NULL,		NO_MASK,			EPFR12_PPG5E_0XX,	D_NULL,		D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			ADC_ANALOG_IN		 }		/* ADC �A�i���O����ch.36 */
};

/* �wP121�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P121[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE12] =
{
	/* DDR12			PFR12			EPFR00		EPFR01		EPFR03		EPFR04				EPFR05		EPFR09				EPFR12		EPFR13				EPFR14		EPFR15		EPFR28				EPFR29		EPFR31		EPFR35		EPFR42		EPFR43				ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX0,	D_NULL,		EPFR09_SOT5E_X0,	D_NULL,		EPFR13_PPG6E_0XX,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN				 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX0,	D_NULL,		EPFR09_SOT5E_X0,	D_NULL,		EPFR13_PPG6E_0XX,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT			 },		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX0,	D_NULL,		EPFR09_SOT5E_X0,	D_NULL,		EPFR13_PPG6E_0XX,	D_NULL,		D_NULL,		EPFR28_FRCK0E_LOW,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			FREE_RUN_TIMCLK_IN	 },		/* �t���[�����^�C�}�N���b�N����ch.0 �����P�[�V����0 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX0,	D_NULL,		EPFR09_SOT5E_X1,	D_NULL,		EPFR13_PPG6E_0XX,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			LIN_UART_SERIAL_OUT	 },		/* LIN_UART �V���A���o��ch.5 �����P�[�V����0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX0,	D_NULL,		EPFR09_SOT5E_X0,	D_NULL,		EPFR13_PPG6E_0XX,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR43_INT7E_LOW,	INTTERUPT_REQUEST_IN },		/* �O�������ݗv������ch.7 �����P�[�V����0 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX1,	D_NULL,		EPFR09_SOT5E_X0,	D_NULL,		EPFR13_PPG6E_0XX,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			RELOAD_TIMER_OUT	 },		/* �����[�h�^�C�}�o��ch.2 �����P�[�V����0 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX0,	D_NULL,		EPFR09_SOT5E_X0,	D_NULL,		EPFR13_PPG6E_1XX,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PPG_OUT				 },		/* PPG �o��ch.6 �����P�[�V����2 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		D_NULL,		D_NULL,		D_NULL,		EPFR04_TOT2E_XX0,	D_NULL,		EPFR09_SOT5E_X0,	D_NULL,		EPFR13_PPG6E_0XX,	D_NULL,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			ADC_ANALOG_IN		 }		/* ADC �A�i���O����ch.37 */
};

/* �wP122�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P122[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE12] =
{
	/* DDR12			PFR12			EPFR00		EPFR01		EPFR03		EPFR04		EPFR05				EPFR09				EPFR12		EPFR13				EPFR14		EPFR15		EPFR28		EPFR29					EPFR31		EPFR35		EPFR42		EPFR43		ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX0,	EPFR09_SCK5E_X0,	D_NULL,		EPFR13_PPG7E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN			 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX0,	EPFR09_SCK5E_X0,	D_NULL,		EPFR13_PPG7E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT		 },		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX0,	EPFR09_SCK5E_X0,	D_NULL,		EPFR13_PPG7E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_01,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		OCU_OUT 		 },		/* �A�E�g�v�b�g�R���y�A�o��ch.0 �����P�[�V����0 */
	{ D_LOW_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX0,	EPFR09_SCK5E_01,	D_NULL,		EPFR13_PPG7E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		LIN_UART_SCK_IN  },		/* LIN_UART �V���A���N���b�N����ch.5 �����P�[�V����0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX0,	EPFR09_SCK5E_01,	D_NULL,		EPFR13_PPG7E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		LIN_UART_SCK_OUT },		/* LIN_UART �V���A���N���b�N�o��ch.5 �����P�[�V����0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX1,	EPFR09_SCK5E_X0,	D_NULL,		EPFR13_PPG7E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		RELOAD_TIMER_OUT },		/* �����[�h�^�C�}�o��ch.3 �����P�[�V����0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX0,	EPFR09_SCK5E_X0,	D_NULL,		EPFR13_PPG7E_1XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT 		 },		/* PPG �o��ch.7 �����P�[�V����2 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR05_TOT3E_XX0,	EPFR09_SCK5E_X0,	D_NULL,		EPFR13_PPG7E_0XX,	D_NULL,		D_NULL,		D_NULL,		EPFR29_OCU0E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ADC_ANALOG_IN	 }		/* ADC �A�i���O����ch.38 */
};

/* �wP123�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P123[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE12] =
{
	/* DDR12			PFR12			EPFR00		EPFR01		EPFR03		EPFR04		EPFR05		EPFR09		EPFR12		EPFR13		EPFR14				EPFR15		EPFR28		EPFR29					EPFR31		EPFR35		EPFR42				EPFR43		ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU1E_X0,		D_NULL,		D_NULL,		EPFR42_DAS0_LOW,	D_NULL,		PORT_IN 		},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU1E_X0,		D_NULL,		D_NULL,		EPFR42_DAS0_LOW,	D_NULL,		PORT_OUT 		},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU1E_01,		D_NULL,		D_NULL,		EPFR42_DAS0_LOW,	D_NULL,		OCU_OUT 		},		/* �A�E�g�v�b�g�R���y�A�o��ch.1 �����P�[�V����0 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_1XX,	D_NULL,		D_NULL,		EPFR29_OCU1E_X0,		D_NULL,		D_NULL,		EPFR42_DAS0_LOW,	D_NULL,		PPG_OUT			},		/* PPG �o��ch.8 �����P�[�V����2 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU1E_X0,		D_NULL,		D_NULL,		EPFR42_DAS0_HIGH,	D_NULL,		DAC_OUT			},		/* DAC �o��ch.0 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG8E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU1E_X0,		D_NULL,		D_NULL,		EPFR42_DAS0_LOW,	D_NULL,		ADC_ANALOG_IN 	}		/* ADC �A�i���O����ch.39 */
};

/* �wP124�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P124[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE12] =
{
	/* DDR12			PFR12			EPFR00		EPFR01				EPFR03		EPFR04		EPFR05		EPFR09		EPFR12		EPFR13		EPFR14				EPFR15		EPFR28		EPFR29				EPFR31		EPFR35		EPFR42		EPFR43		ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU2E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN	 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU2E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT },		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU2E_01,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		OCU_OUT	 },		/* �A�E�g�v�b�g�R���y�A�o��ch.2 �����P�[�V����0 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR01_ICU5E_10,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_0XX,	D_NULL,		D_NULL,		EPFR29_OCU2E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		ICU_IN	 },		/* �C���v�b�g�L���v�`������ch.5 �����P�[�V����2 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR14_PPG9E_1XX,	D_NULL,		D_NULL,		EPFR29_OCU2E_X0,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT	 }		/* PPG �o��ch.9 �����P�[�V����2 */
};

/* �wP125�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P125[PORT_FUNC_CHANGE_ID_NUM_5][PORT_FUNC_CHANGE12] =
{
	/* DDR12			PFR12			EPFR00				EPFR01		EPFR03		EPFR04		EPFR05		EPFR09		EPFR12		EPFR13		EPFR14		EPFR15					EPFR28		EPFR29					EPFR31		EPFR35		EPFR42		EPFR43		ID */
	{ D_LOW_PXX5,		D_LOW_PXX5,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_0XX,		D_NULL,		EPFR29_OCU3E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN	 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX5,		D_LOW_PXX5,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_0XX,		D_NULL,		EPFR29_OCU3E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT },		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_0XX,		D_NULL,		EPFR29_OCU3E_01,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		OCU_OUT	 },		/* �A�E�g�v�b�g�R���y�A�o��ch.3 �����P�[�V����0 */
	{ D_LOW_PXX5,		D_LOW_PXX5,		EPFR00_ICU0E_00,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_0XX,		D_NULL,		EPFR29_OCU3E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		ICU_IN 	 },		/* �C���v�b�g�L���v�`������ch.0 �����P�[�V����0 */
	{ D_HIGH_PXX5,		D_HIGH_PXX5,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR15_PPG10E_1XX,		D_NULL,		EPFR29_OCU3E_X0,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PPG_OUT	 }		/* PPG �o��ch.10 �����P�[�V����2 */
};

/* �wP126�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P126[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE12] =
{
	/* DDR12			PFR12			EPFR00		EPFR01		EPFR03		EPFR04		EPFR05		EPFR09		EPFR12		EPFR13		EPFR14		EPFR15		EPFR28		EPFR29		EPFR31					EPFR35				EPFR42		EPFR43				ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU4E_X0,		NO_MASK,			D_NULL,		NO_MASK,			PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU4E_X0,		NO_MASK,			D_NULL,		NO_MASK,			PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU4E_X0,		NO_MASK,			D_NULL,		NO_MASK,			PPG_TRG_IN					},		/* PPG �g���K����0 (ch.0-ch.3) */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU4E_X0,		EPFR35_SIN0E_LOW,	D_NULL,		NO_MASK,			MULTI_FUNCTION_SERIAL_IN	},		/* �}���`�t�@���N�V�����V���A������ch.0 �����P�[�V����0 */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU4E_X0,		NO_MASK,			D_NULL,		EPFR43_INT1E_LOW,	INTTERUPT_REQUEST_IN		},		/* �O�������ݗv������ch.1 �����P�[�V����0 */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU4E_01,		NO_MASK,			D_NULL,		NO_MASK,			OCU_OUT						}		/* �A�E�g�v�b�g�R���y�A�o��ch.4 �����P�[�V����0 */
};

/* �wP127�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P127[PORT_FUNC_CHANGE_ID_NUM_4][PORT_FUNC_CHANGE12] =
{	
	/* DDR12			PFR12			EPFR00		EPFR01		EPFR03		EPFR04		EPFR05		EPFR09		EPFR12		EPFR13		EPFR14		EPFR15		EPFR28		EPFR29		EPFR31					EPFR35				EPFR42		EPFR43		ID */
	{ D_LOW_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU5E_X0,		EPFR35_SOT0E_X0,	D_NULL,		D_NULL,		PORT_IN 				 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX7,		D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU5E_X0,		EPFR35_SOT0E_X0,	D_NULL,		D_NULL,		PORT_OUT  				 },		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU5E_X0,		EPFR35_SOT0E_01,	D_NULL,		D_NULL,		MULTI_FUNCTION_SERIAL_OUT },	/* �}���`�t�@���N�V�����V���A���o��ch.0 �����P�[�V����0 */
	{ D_HIGH_PXX7,		D_HIGH_PXX7,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR31_OCU5E_01,		EPFR35_SOT0E_X0,	D_NULL,		D_NULL,		OCU_OUT					 }		/* �A�E�g�v�b�g�R���y�A�o��ch.5 �����P�[�V����0*/
};

/* �wP130�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P130[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE13] =
{
	/* DDR13			PFR13			EPFR00				EPFR01		EPFR10		EPFR15		EPFR26					EPFR35				EPFR36		EPFR43				ID */
	{ D_LOW_PXX0,		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_LOW,		EPFR35_SCK0E_X0,	D_NULL,		NO_MASK,			PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX0,		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_LOW,		EPFR35_SCK0E_X0,	D_NULL,		NO_MASK,			PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_LOW,		EPFR35_SCK0E_01,	D_NULL,		NO_MASK,			MULTI_FUNCTION_SCK_IN	},		/* �}���`�t�@���N�V�����V���A���N���b�N����ch.0 �����P�[�V����0 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_LOW,		EPFR35_SCK0E_01,	D_NULL,		NO_MASK,			MULTI_FUNCTION_SCK_OUT	},		/* �}���`�t�@���N�V�����V���A���N���b�N�o��ch.0 �����P�[�V����0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_LOW,		EPFR35_SCK0E_X0,	D_NULL,		EPFR43_INT0E_LOW,	INTTERUPT_REQUEST_IN	},		/* �O�������ݗv������ch.0 �����P�[�V����0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		EPFR00_ICU1E_00,	D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_LOW,		EPFR35_SCK0E_X0,	D_NULL,		NO_MASK,			ICU_IN					},		/* �C���v�b�g�L���v�`������ch.1 �����P�[�V����0 */
	{ D_LOW_PXX0,		D_LOW_PXX0,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_LOW,		EPFR35_SCK0E_X0,	D_NULL,		NO_MASK,			BASE_TIMER_IN			},		/* �x�[�X�^�C�}����ch.0 */
	{ D_HIGH_PXX0,		D_HIGH_PXX0,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA0E_HIGH,		EPFR35_SCK0E_X0,	D_NULL,		NO_MASK,			BASE_TIMER_OUT			}		/* �x�[�X�^�C�}�o��ch.0 */
};

/* �wP131�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P131[PORT_FUNC_CHANGE_ID_NUM_8][PORT_FUNC_CHANGE13] =
{
	/* DDR13			PFR13			EPFR00				EPFR01		EPFR10		EPFR15		EPFR26					EPFR35		EPFR36				EPFR43				ID */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_LOW,		D_NULL,		NO_MASK,			NO_MASK,			PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_LOW,		D_NULL,		NO_MASK,			NO_MASK,			PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_LOW,		D_NULL,		NO_MASK,			NO_MASK,			PPG_TRG_IN					},		/* PPG �g���K����1 (ch.4-ch.7) */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_LOW,		D_NULL,		EPFR36_SIN1E_LOW,	NO_MASK,			MULTI_FUNCTION_SERIAL_IN	},		/* �}���`�t�@���N�V�����V���A������ch.1 �����P�[�V����0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_LOW,		D_NULL,		NO_MASK,			EPFR43_INT4E_LOW,	INTTERUPT_REQUEST_IN		},		/* �O�������ݗv������ch.4 �����P�[�V����0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		EPFR00_ICU2E_00,	D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_LOW,		D_NULL,		NO_MASK,			NO_MASK,			ICU_IN						},		/* �C���v�b�g�L���v�`������ch.2 �����P�[�V����0 */
	{ D_LOW_PXX1,		D_LOW_PXX1,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_LOW,		D_NULL,		NO_MASK,			NO_MASK,			BASE_TIMER_IN				},		/* �x�[�X�^�C�}����ch.1 */
	{ D_HIGH_PXX1,		D_HIGH_PXX1,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIA1E_HIGH,		D_NULL,		NO_MASK,			NO_MASK,			BASE_TIMER_OUT				}		/* �x�[�X�^�C�}�o��ch.1 */
};

/* �wP132�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P132[PORT_FUNC_CHANGE_ID_NUM_7][PORT_FUNC_CHANGE13] =
{
	/* DDR13			PFR13			EPFR00				EPFR01		EPFR10		EPFR15		EPFR26					EPFR35		EPFR36					EPFR43				ID */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIB0E_LOW,		D_NULL,		EPFR36_SOT1E_X0,		NO_MASK,			PORT_IN						},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIB0E_LOW,		D_NULL,		EPFR36_SOT1E_X0,		NO_MASK,			PORT_OUT					},		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIB0E_LOW,		D_NULL,		EPFR36_SOT1E_01,		NO_MASK,			MULTI_FUNCTION_SERIAL_OUT	},		/* �}���`�t�@���N�V�����V���A���o��ch.1 �����P�[�V����0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIB0E_LOW,		D_NULL,		EPFR36_SOT1E_X0,		EPFR43_INT2E_LOW,	INTTERUPT_REQUEST_IN		},		/* �O�������ݗv������ch.2 �����P�[�V����0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		EPFR00_ICU3E_00,	D_NULL,		D_NULL,		D_NULL,		EPFR26_TIB0E_LOW,		D_NULL,		EPFR36_SOT1E_X0,		NO_MASK,			ICU_IN						},		/* �C���v�b�g�L���v�`������ch.3 �����P�[�V����0 */
	{ D_LOW_PXX2,		D_LOW_PXX2,		NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIB0E_LOW,		D_NULL,		EPFR36_SOT1E_X0,		NO_MASK,			BASE_TIMER_IN				},		/* �x�[�X�^�C�}����ch.0 */
	{ D_HIGH_PXX2,		D_HIGH_PXX2,	NO_MASK,			D_NULL,		D_NULL,		D_NULL,		EPFR26_TIB0E_HIGH,		D_NULL,		EPFR36_SOT1E_X0,		NO_MASK,			BASE_TIMER_OUT				}		/* �x�[�X�^�C�}�o��ch.0 */
};

/* �wP133�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P133[PORT_FUNC_CHANGE_ID_NUM_10][PORT_FUNC_CHANGE13] =
{
	/* DDR13			PFR13			EPFR00		EPFR01				EPFR10		EPFR15				EPFR26					EPFR35		EPFR36					EPFR43				ID */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_X0,		NO_MASK,			PORT_IN					},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX3,		D_LOW_PXX3,		D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_X0,		NO_MASK,			PORT_OUT				},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX3,		D_HIGH_PXX3,	D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_01,		NO_MASK,			MULTI_FUNCTION_SCK_IN	},		/* �}���`�t�@���N�V�����V���A���N���b�N����ch.1 �����P�[�V����0 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_01,		NO_MASK,			MULTI_FUNCTION_SCK_OUT	},		/* �}���`�t�@���N�V�����V���A���N���b�N�o��ch.1 �����P�[�V����0 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_X0,		EPFR43_INT3E_LOW,	INTTERUPT_REQUEST_IN	},		/* �O�������ݗv������ch.3 �����P�[�V����0 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		EPFR01_ICU4E_00,	D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_X0,		NO_MASK,			ICU_IN					},		/* �C���v�b�g�L���v�`������ch.4 �����P�[�V����0 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_X0,		NO_MASK,			BASE_TIMER_IN			},		/* �x�[�X�^�C�}����ch.1 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_HIGH,		D_NULL,		EPFR36_SCK1E_X0,		NO_MASK,			BASE_TIMER_OUT			},		/* �x�[�X�^�C�}�o��ch.1 */
	{ D_HIGH_PXX3,		D_HIGH_PXX3,	D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_1X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_X0,		NO_MASK,			PPG_OUT					},		/* PPG �o��ch.11 �����P�[�V����1 */
	{ D_LOW_PXX3,		D_LOW_PXX3,		D_NULL,		NO_MASK,			D_NULL,		EPFR15_PPG11E_0X,	EPFR26_TIB1E_LOW,		D_NULL,		EPFR36_SCK1E_X0,		NO_MASK,			PPG_TRG_IN				}		/* PPG �g���K����5 (ch.20-ch.23) */
};

/* �wP134�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P134[PORT_FUNC_CHANGE_ID_NUM_6][PORT_FUNC_CHANGE13] =
{
	/* DDR13			PFR13			EPFR00		EPFR01				EPFR10				EPFR15		EPFR26		EPFR35		EPFR36		EPFR43				ID */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		NO_MASK,			EPFR10_PPG1E_0XXX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_IN				 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX4,		D_LOW_PXX4,		D_NULL,		NO_MASK,			EPFR10_PPG1E_0XXX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PORT_OUT			 },		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		NO_MASK,			EPFR10_PPG1E_0XXX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PPG_TRG_IN			 },		/* PPG �g���K����2 (ch.8-ch.11) */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		NO_MASK,			EPFR10_PPG1E_0XXX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		EPFR43_INT5E_LOW,	INTTERUPT_REQUEST_IN },		/* �O�������ݗv������ch.5 �����P�[�V����0 */
	{ D_LOW_PXX4,		D_LOW_PXX4,		D_NULL,		EPFR01_ICU5E_00,	EPFR10_PPG1E_0XXX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			ICU_IN				 },		/* �C���v�b�g�L���v�`������ch.5 �����P�[�V����0 */
	{ D_HIGH_PXX4,		D_HIGH_PXX4,	D_NULL,		NO_MASK,			EPFR10_PPG1E_1XXX,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		NO_MASK,			PPG_OUT				 }		/* PPG �o��ch.1 �����P�[�V����3 */
};

/* �wP136�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P136[PORT_FUNC_CHANGE_ID_NUM_3][PORT_FUNC_CHANGE13] =
{
	/* DDR13			PFR13			EPFR00		EPFR0		EPFR10		EPFR15		EPFR26		EPFR35		EPFR36		EPFR43		ID */
	{ D_LOW_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN			 },		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX6,		D_LOW_PXX6,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT		 },		/* �ėp�o�̓|�[�g */
	{ D_HIGH_PXX6,		D_HIGH_PXX6,	D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		SUB_CLK_OSC_OUT	 }		/* �T�u�N���b�NOSC �o��(�T�u�N���b�N���ڕi��̂�) */
};

/* �wP137�@�\�ؑ֐ݒ�l�e�[�u���x*/
const static UI_16 C_FuncOperateTbl_P137[PORT_FUNC_CHANGE_ID_NUM_3][PORT_FUNC_CHANGE13] =
{
	/* DDR13		PFR13			EPFR00		EPFR0		EPFR10		EPFR15		EPFR26		EPFR35		EPFR36		EPFR43		ID */
	{ D_LOW_PXX7,	D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_IN			},		/* �ėp���̓|�[�g */
	{ D_HIGH_PXX7,	D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		PORT_OUT		},		/* �ėp�o�̓|�[�g */
	{ D_LOW_PXX7,	D_LOW_PXX7,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		D_NULL,		SUB_CLK_OSC_IN	}		/* �T�u�N���b�NOSC ����(�T�u�N���b�N���ڕi��̂�) */
};

/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u��P00 */
static const UI_16* const C_FuncOperateTbl_P00[BIT_MAX] =
{
	&C_FuncOperateTbl_P000[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P000 */
	&C_FuncOperateTbl_P001[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P001 */
	&C_FuncOperateTbl_P002[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P002 */
	&C_FuncOperateTbl_P003[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P003 */
	&C_FuncOperateTbl_P004[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P004 */
	&C_FuncOperateTbl_P005[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P005 */
	&C_FuncOperateTbl_P006[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P006 */
	&C_FuncOperateTbl_P007[0][0]		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P007 */
};

/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u��P01 */
static const UI_16* const C_FuncOperateTbl_P01[BIT_MAX] =
{
	&C_FuncOperateTbl_P010[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P010 */
	&C_FuncOperateTbl_P011[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P011 */
	&C_FuncOperateTbl_P012[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P012 */
	&C_FuncOperateTbl_P013[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P013 */
	&C_FuncOperateTbl_P014[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P014 */
	&C_FuncOperateTbl_P015[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P015 */
	&C_FuncOperateTbl_P016[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P016 */
	&C_FuncOperateTbl_P017[0][0]		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P017 */
};

/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u��P02 */
static const UI_16* const C_FuncOperateTbl_P02[BIT_MAX] =
{
	&C_FuncOperateTbl_P020[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P020 */
	&C_FuncOperateTbl_P021[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P021 */
	&C_FuncOperateTbl_P022[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P022 */
	&C_FuncOperateTbl_P023[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P023 */
	&C_FuncOperateTbl_P024[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P024 */
	&C_FuncOperateTbl_P025[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P025 */
	&C_FuncOperateTbl_P026[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P026 */
	&C_FuncOperateTbl_P027[0][0]		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P027 */
};

/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u��P03 */
static const UI_16* const C_FuncOperateTbl_P03[BIT_MAX] =
{
	&C_FuncOperateTbl_P030[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P030 */
	&C_FuncOperateTbl_P031[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P031 */
	&C_FuncOperateTbl_P032[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P032 */
	&C_FuncOperateTbl_P033[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P033 */
	&C_FuncOperateTbl_P034[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P034 */
	&C_FuncOperateTbl_P035[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P035 */
	&C_FuncOperateTbl_P036[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P036 */
	&C_FuncOperateTbl_P037[0][0]		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P037 */
};

/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u��P04 */
static const UI_16* const C_FuncOperateTbl_P04[BIT_MAX] =
{
	&C_FuncOperateTbl_P040[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P040 */
	&C_FuncOperateTbl_P041[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P041 */
	&C_FuncOperateTbl_P042[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P042 */
	&C_FuncOperateTbl_P043[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P043 */
	&C_FuncOperateTbl_P044[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P044 */
	&C_FuncOperateTbl_P045[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P045 */
	&C_FuncOperateTbl_P046[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P046 */
	&C_FuncOperateTbl_P047[0][0]		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P047 */
};

/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u��P05 */
static const UI_16* const C_FuncOperateTbl_P05[BIT_MAX] =
{
	&C_FuncOperateTbl_P050[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P050 */
	&C_FuncOperateTbl_P051[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P051 */
	&C_FuncOperateTbl_P052[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P052 */
	&C_FuncOperateTbl_P053[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P053 */
	&C_FuncOperateTbl_P054[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P054 */
	&C_FuncOperateTbl_P055[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P055 */
	&C_FuncOperateTbl_P056[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P056 */
	&C_FuncOperateTbl_P057[0][0]		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P057 */
};

/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u��P06 */
static const UI_16* const C_FuncOperateTbl_P06[BIT_MAX] =
{
	&C_FuncOperateTbl_P060[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P060 */
	&C_FuncOperateTbl_P061[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P061 */
	&C_FuncOperateTbl_P062[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P062 */
	&C_FuncOperateTbl_P063[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P063 */
	&C_FuncOperateTbl_P064[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P064 */
	&C_FuncOperateTbl_P065[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P065 */
	&C_FuncOperateTbl_P066[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P066 */
	&C_FuncOperateTbl_P067[0][0]		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P067 */
};

/*  �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u��P07 */
static const UI_16* const C_FuncOperateTbl_P07[BIT_MAX] =
{
	&C_FuncOperateTbl_P070[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P070 */
	&C_FuncOperateTbl_P071[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P071 */
	&C_FuncOperateTbl_P072[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P072 */
	&C_FuncOperateTbl_P073[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P073 */
	&C_FuncOperateTbl_P074[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P074 */
	&C_FuncOperateTbl_P075[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P075 */
	&C_FuncOperateTbl_P076[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P076 */
	&C_FuncOperateTbl_P077[0][0]		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P077 */
};

/*  �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u��P08 */
static const UI_16* const C_FuncOperateTbl_P08[BIT_MAX] =
{
	&C_FuncOperateTbl_P080[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P080 */
	&C_FuncOperateTbl_P081[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P081 */
	&C_FuncOperateTbl_P082[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P082 */
	&C_FuncOperateTbl_P083[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P083 */
	&C_FuncOperateTbl_P084[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P084 */
	&C_FuncOperateTbl_P085[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P085 */
	&C_FuncOperateTbl_P086[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P086 */
	&C_FuncOperateTbl_P087[0][0]		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P087 */
};

/*  �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u��P09 */
static const UI_16* const C_FuncOperateTbl_P09[BIT_MAX] =
{
	&C_FuncOperateTbl_P090[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P090 */
	&C_FuncOperateTbl_P091[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P091 */
	&C_FuncOperateTbl_P092[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P092 */
	&C_FuncOperateTbl_P093[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P093 */
	&C_FuncOperateTbl_P094[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P094 */
	&C_FuncOperateTbl_P095[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P095 */
	&C_FuncOperateTbl_P096[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P096 */
	&C_FuncOperateTbl_P097[0][0]		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P097 */
};

/*  �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u��P10 */
static const UI_16* const C_FuncOperateTbl_P10[BIT_MAX] =
{
	&C_FuncOperateTbl_P100[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P100 */
	&C_FuncOperateTbl_P101[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P101 */
	&C_FuncOperateTbl_P102[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P102 */
	&C_FuncOperateTbl_P103[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P103 */
	&C_FuncOperateTbl_P104[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P104 */
	&C_FuncOperateTbl_P105[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P105 */
	&C_FuncOperateTbl_P106[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P106 */
	&C_FuncOperateTbl_P107[0][0]		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P107 */
};

/*  �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u��P11 */
static const UI_16* const C_FuncOperateTbl_P11[BIT_MAX] =
{
	&C_FuncOperateTbl_P110[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P110 */
	&C_FuncOperateTbl_P111[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P111 */
	&C_FuncOperateTbl_P112[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P112 */
	&C_FuncOperateTbl_P113[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P113 */
	&C_FuncOperateTbl_P114[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P114 */
	&C_FuncOperateTbl_P115[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P115 */
	&C_FuncOperateTbl_P116[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P116 */
	&C_FuncOperateTbl_P117[0][0]		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P117 */
};

/*  �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u��P12 */
static const UI_16* const C_FuncOperateTbl_P12[BIT_MAX] =
{
	&C_FuncOperateTbl_P120[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P120 */
	&C_FuncOperateTbl_P121[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P121 */
	&C_FuncOperateTbl_P122[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P122 */
	&C_FuncOperateTbl_P123[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P123 */
	&C_FuncOperateTbl_P124[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P124 */
	&C_FuncOperateTbl_P125[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P125 */
	&C_FuncOperateTbl_P126[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P126 */
	&C_FuncOperateTbl_P127[0][0]		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P127 */
};

/*  �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u��P13 */
static const UI_16* const C_FuncOperateTbl_P13[BIT_MAX] =
{
	&C_FuncOperateTbl_P130[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P130 */
	&C_FuncOperateTbl_P131[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P131 */
	&C_FuncOperateTbl_P132[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P132 */
	&C_FuncOperateTbl_P133[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P133 */
	&C_FuncOperateTbl_P134[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P134 */
	D_NULL,								/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P135 */
	&C_FuncOperateTbl_P136[0][0],		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P136 */
	&C_FuncOperateTbl_P137[0][0]		/* �@�\�ؑ֐ݒ�l�e�[�u���ւ̃|�C���^P137 */
};

/* �@�\�ؑ֐ݒ�l�|�[�g�I���e�[�u�� */
static const UI_16* const * const C_FuncOperateTbl[PORT_MAX] =
{
	&C_FuncOperateTbl_P00[0],		/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u���ւ̃|�C���^P00 */
	&C_FuncOperateTbl_P01[0],		/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u���ւ̃|�C���^P01 */
	&C_FuncOperateTbl_P02[0],		/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u���ւ̃|�C���^P02 */
	&C_FuncOperateTbl_P03[0],		/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u���ւ̃|�C���^P03 */
	&C_FuncOperateTbl_P04[0],		/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u���ւ̃|�C���^P04 */
	&C_FuncOperateTbl_P05[0],		/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u���ւ̃|�C���^P05 */
	&C_FuncOperateTbl_P06[0],		/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u���ւ̃|�C���^P06 */
	&C_FuncOperateTbl_P07[0],		/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u���ւ̃|�C���^P07 */
	&C_FuncOperateTbl_P08[0],		/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u���ւ̃|�C���^P08 */
	&C_FuncOperateTbl_P09[0],		/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u���ւ̃|�C���^P09 */
	&C_FuncOperateTbl_P10[0],		/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u���ւ̃|�C���^P10 */
	&C_FuncOperateTbl_P11[0],		/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u���ւ̃|�C���^P11 */
	&C_FuncOperateTbl_P12[0],		/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u���ւ̃|�C���^P12 */
	&C_FuncOperateTbl_P13[0]		/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u���ւ̃|�C���^P13 */
};
#endif	/* PORT_DRV_FUNC_CHANGE == 1 */

/****************************************************************************************/
/*	[���W���[����]	PortDrv_SetDataByte	[����]	�w��|�[�g��Byte�f�[�^�ݒ�				*/
/*======================================================================================*/
/*	[�����T�v]	�w��|�[�g�̃|�[�g�f�[�^���W�X�^�Ɏw��Byte�f�[�^��ݒ肷��				*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 PortDrv_SetDataByte( E_PORTDRV_PORT_NUM port_num, 					*/
/* 				, E_PORTDRV_BYTE_NUM byte_num, UI_8 set_byte )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_PORTDRV_PORT_NUM port_num	�w��|�[�g�ԍ�	E_PORTDRV_PORT_NUM:enum��`	*/
/*				E_PORTDRV_BYTE_NUM byte_num	16bitϲ�݂Ƃ�IF�pByte�ݒ���				*/
/*				UI_8 set_byte	�ݒ���												*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8 state;	�������s�X�e�[�^�X											*/
/*======================================================================================*/
/*	[ ��  �l ]																			*/
/****************************************************************************************/
SI_8 PortDrv_SetDataByte( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BYTE_NUM byte_num, UI_8 set_byte )
{
	SI_8 state;			/* �������s�X�e�[�^�X */
	
	/* ����1�F�w��|�[�g�ԍ��̃f�[�^�͈͔��� */
	if ( port_num < PORTDRV_PORT_MAX ) {
		state = D_OK;	/* ���� */
		*C_PortDataTbl[port_num] = set_byte;	/* Byte�f�[�^���|�[�g�f�[�^���W�X�^�ɐݒ� */
	} else {
		state = D_NG; 	/* �ُ� */
	}
	
	return state;	/* �߂�l:�������s�X�e�[�^�X */
}

/****************************************************************************************/
/*	[���W���[����]	PortDrv_SetDataBit	[����]	�w��|�[�g�̎w��Bit�f�[�^�ݒ�			*/
/*======================================================================================*/
/*	[�����T�v]	�w��|�[�g�̃|�[�g�f�[�^���W�X�^�̎w��Bit��Bit�f�[�^��ݒ肷��			*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 PortDrv_SetDataBit( E_PORTDRV_PORT_NUM port_num, 					*/
/*										E_PORTDRV_BIT_NUM bit_num, UI_8 set_bit);		*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_PORTDRV_PORT_NUM port_num	�w��|�[�g�ԍ�	E_PORTDRV_PORT_NUM:enum��`	*/
/*				E_PORTDRV_BIT_NUM bit_num	�w��r�b�g�ʒu	E_PORTDRV_BIT_NUM:enum��`	*/
/*				UI_8 set_bit	�ݒ���												*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8 state;	�������s�X�e�[�^�X											*/
/*======================================================================================*/
/*	[ ��  �l ]																			*/
/****************************************************************************************/
SI_8 PortDrv_SetDataBit( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BIT_NUM bit_num, UI_8 set_bit )
{
	UI_8 pdr_work;				/* �|�[�g�f�[�^���W�X�^���Z�pwork�ϐ� */
	SI_8 state; 				/* �������s�X�e�[�^�X */
	UI_16 mask_level; 			/* �}�X�N���x���i�[�ϐ� */
	
	/* ����1�F�w��|�[�g�ԍ��̃f�[�^�͈͔��� */
	if ( port_num < PORTDRV_PORT_MAX ) {
		
		/* ����2�F�w��r�b�g�ʒu�̃f�[�^�͈͔��� */
		if ( bit_num < PORTDRV_BIT_MAX ) {
			
			/* ����3�F�ݒ���̃f�[�^�͈͔��� */
			if ( ( set_bit == D_LOW ) || ( set_bit == D_HIGH ) ) {
				state = D_OK;	/* ���� */	
			} else {
				state = D_NG; 	/* �ُ� */
			}
		} else {
			state = D_NG; 		/* �ُ� */
		}
	} else {
		state = D_NG; 			/* �ُ� */
	}
	
	/* �������s�X�e�[�^�X�̐��픻�� */
	if ( state == D_OK ) {
		
		(void) IntrDrv_GetMaskLevel( &mask_level );
		(void) IntrDrv_SetMaskLevel( PORT_INTR_LEVEL );			/* �����݋֎~ */
		pdr_work = *C_PortDataTbl[port_num];
		/* �ݒ���Hi���x���𔻒� */
		if ( ( set_bit == D_HIGH ) ) {
			pdr_work |= C_PortCheckBitTbl[bit_num];	/* �|�[�g�f�[�^���W�X�^�̎w��r�b�g�ݒ� */
		} else {
			pdr_work &= (UI_8)~C_PortCheckBitTbl[bit_num];	 /* �|�[�g�f�[�^���W�X�^�̎w��r�b�g�ݒ� */
		}
		*C_PortDataTbl[port_num] = pdr_work;
		(void) IntrDrv_SetMaskLevel( mask_level );						 /* �����ݕ��A */
	} else {
		/* �����Ȃ� */
	}
	
	return state;	/* �߂�l:�������s�X�e�[�^�X */
}

/****************************************************************************************/
/*	[���W���[����]	PortDrv_GetDataByte	[����]	�w��|�[�g��Byte�f�[�^�擾				*/
/*======================================================================================*/
/*	[�����T�v]	�w��|�[�g�̃|�[�g�f�[�^���W�X�^�Ɏw��Byte�f�[�^���擾����				*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 PortDrv_GetDataByte( E_PORTDRV_PORT_NUM port_num,					*/
/* 				, E_PORTDRV_BYTE_NUM byte_num, UI_8 *p_get_data )						*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_PORTDRV_PORT_NUM port_num	�w��|�[�g�ԍ�	E_PORTDRV_PORT_NUM:enum��`	*/
/*				E_PORTDRV_BYTE_NUM byte_num	16bitϲ�݂Ƃ�IF�pByte�ݒ���				*/
/*				UI_8 *p_get_data	�|�[�g���R�s�[��A�h���X							*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8 state;	�������s�X�e�[�^�X											*/
/*======================================================================================*/
/*	[ ��  �l ]	�|�[�g�͈͊O�̂Ƃ���Lo ���x���̃f�[�^��ԋp����							*/
/****************************************************************************************/
SI_8 PortDrv_GetDataByte( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BYTE_NUM byte_num, UI_8 *p_get_data )
{
	SI_8 state;				/* �������s�X�e�[�^�X */
	
	/* ����2�F�|�[�g���R�s�[��A�h���X���� */
	if ( p_get_data != D_NULL ) {

		/* ����1�F�w��|�[�g�ԍ��̃f�[�^�͈͔��� */
		if ( port_num < PORTDRV_PORT_MAX ) {
		
			state = D_OK;									/* ���� */
			*p_get_data = *C_PortDataDirectTbl[port_num];	/* ���̓f�[�^�_�C���N�g���[�h���W�X�^��Byte�f�[�^�擾 */
		} else {
			state = D_NG;			/* �ُ� */
			*p_get_data = D_LOW;	/* �|�[�g�ُ͈͈펞�A�ݒ��� Lo �Œ� */
		}
	} else {
		state = D_NG; 				/* �ُ� */
	}
	
	return state;	/* �߂�l:�������s�X�e�[�^�X */
}

/****************************************************************************************/
/*	[���W���[����]	PortDrv_GetDataBit	[����]	�w��|�[�g�̎w��Bit�f�[�^�擾			*/
/*======================================================================================*/
/*	[�����T�v]	�w��|�[�g�̃|�[�g�f�[�^���W�X�^�̎w��Bit��Bit�f�[�^���擾����			*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 PortDrv_GetDataBit( E_PORTDRV_PORT_NUM port_num, 					*/
/*										E_PORTDRV_BIT_NUM bit_num, UI_8 *p_get_bit )	*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_PORTDRV_PORT_NUM port_num	�w��|�[�g�ԍ�	E_PORTDRV_PORT_NUM:enum��`	*/
/*				E_PORTDRV_BIT_NUM bit_num	�w��r�b�g�ʒu	E_PORTDRV_BIT_NUM:enum��`	*/
/*				UI_8 *p_get_bit		�|�[�g���R�s�[��A�h���X							*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8 state;	�������s�X�e�[�^�X											*/
/*======================================================================================*/
/*	[ ��  �l ]	�|�[�g�͈͊O�̂Ƃ��� Lo���x���̃f�[�^��ԋp����							*/
/****************************************************************************************/
SI_8 PortDrv_GetDataBit( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BIT_NUM bit_num, UI_8 *p_get_bit )
{
	UI_8 pddr_temp;					/*  ���̓f�[�^�_�C���N�g���[�h���W�X�^�l�i�[�p */
	SI_8 state; 					/* �������s�X�e�[�^�X */
	
	/* ����3�F�|�[�g���R�s�[��A�h���X���� */
	if ( p_get_bit != D_NULL ) {

		/* ����1�F�w��|�[�g�ԍ��̃f�[�^�͈͔��� */
		if ( port_num < PORTDRV_PORT_MAX ) {
				
			/* ����2�F�w��r�b�g�ʒu�̃f�[�^�͈͔��� */
			if ( bit_num < PORTDRV_BIT_MAX ) {
				state = D_OK;		/* ���� */	
			} else {
				state = D_NG; 		/* �ُ� */
				*p_get_bit = D_LOW;	/* �|�[�g�ُ͈͈펞�A�ݒ��� Lo �Œ� */
			}
		} else {
			state = D_NG; 			/* �ُ� */
			*p_get_bit = D_LOW;		/* �|�[�g�ُ͈͈펞�A�ݒ��� Lo �Œ� */
		}
	} else {
		state = D_NG; 				/* �ُ� */
	}
	
	/* �������s�X�e�[�^�X�̐��픻�� */
	if ( state == D_OK ) {
		
		pddr_temp = *C_PortDataDirectTbl[port_num];
		/* �ݒ��� Hi���x���𔻒� */
		if ( ( pddr_temp & C_PortCheckBitTbl[bit_num] ) == C_PortCheckBitTbl[bit_num] ) {
			*p_get_bit = D_HIGH;	/* Hi���x�� */
		} else {
			*p_get_bit = D_LOW;		/* Lo���x�� */
		}
	} else {
		/* �����Ȃ� */  
	}
	
	return state;	/* �߂�l:�������s�X�e�[�^�X */
}

#if (PORT_DRV_FUNC_CHANGE == 1)
/****************************************************************************************/
/*	[���W���[����]	PortDrv_SetFuncBit	[����]	�@�\���W�X�^�ؑ֏���					*/
/*======================================================================================*/
/*	[�����T�v]	�@�\���W�X�^�̐ؑւ��s���BID�ɏ]���āA���W�X�^�ݒ�l��ύX����B		*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 PortDrv_SetFuncBit( E_PORTDRV_PORT_NUM port_num, 					*/
/*							E_PORTDRV_BIT_NUM bit_num, E_PORT_FUNC_CHANGE_ID set_id )	*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_PORTDRV_PORT_NUM port_num	�w��|�[�g�ԍ�	E_PORTDRV_PORT_NUM:enum��`	*/
/*				E_PORTDRV_BIT_NUM bit_num	�w��r�b�g�ʒu	E_PORTDRV_BIT_NUM:enum��`	*/
/*				E_PORT_FUNC_CHANGE_ID set_id	�w��ID									*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8 state;	�������s�X�e�[�^�X											*/
/*======================================================================================*/
/*	[ ��  �l ]																			*/
/****************************************************************************************/
SI_8 PortDrv_SetFuncBit( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BIT_NUM bit_num, E_PORT_FUNC_CHANGE_ID set_id )
{
	UI_8 i;								/* ���[�v�J�E���^ */
	__io volatile UI_8 *const *table;	/* �@�\���W�X�^�e�[�u���|�C���^�i�[�ϐ� */
	const UI_16 *const *set1;			/* �@�\���W�X�^�ݒ�l�|�C���^�i�[�ϐ�1 */
	const UI_16 *set2;					/* �@�\���W�X�^�ݒ�l�|�C���^�i�[�ϐ�2 */
	UI_8 set_data1;						/* �@�\���W�X�^�ݒ�l�i�[�ϐ�1 */
	const UI_16 *set_data2;				/* �@�\���W�X�^�ݒ�l�i�[�ϐ�2 */
	UI_16 set_data3;					/* �@�\���W�X�^�ݒ�l�i�[�ϐ�3 */
	UI_16 set_data4;					/* �f�[�^����&�|�[�g�@�\���W�X�^�ݒ�l�i�[�ϐ� */
	SI_8 state;							/* �������s�X�e�[�^�X */
	UI_16 mask_level;					/* �}�X�N���x���i�[�ϐ� */
	UI_8 mask;							/* �}�X�N�p�ϐ� */
	UI_8 shift;							/* �r�b�g�V�t�g�p�ϐ� */
	UI_8 func;							/* �I���@�\ */
	UI_8 work;							/* ���W�X�^���Z�pwork�ϐ� */
	UI_8 ddr_work;						/* �f�[�^�������W�X�^���Z�pwork�ϐ� */
	UI_8 pfr_work;						/* �|�[�g�@�\���W�X�^���Z�pwork�ϐ� */
	
	state = D_NG; 						/* �ُ� */
	func = 0;							/* �@�\������ */
	
	/* ����1�F�w��|�[�g�ԍ��̃f�[�^�͈͔��� */
	if ( port_num < PORTDRV_PORT_MAX ) {
		
		/* ����2�F�w��r�b�g�ʒu�̃f�[�^�͈͔��� */
		if ( bit_num < PORTDRV_BIT_MAX ) {
			
			/* ����3�F�w��ID�̋@�\���Y���|�[�g�ɑ��݂��邩���� */
			set1 = C_FuncOperateTbl[port_num];	/* �@�\�ؑ֐ݒ�l�|�[�g�I���e�[�u�������� */
			set2 = set1[bit_num];				/* �@�\�ؑ֐ݒ�l�r�b�g�I���e�[�u�������� */
			for ( i = 0; i < C_PortFuncChangeIdNumTbl[port_num][bit_num]; i++ ) {
				
				set_data1 = (UI_8)(*(set2 + ((C_PortFuncChangeTbl[port_num] * i) 
										+ (C_PortFuncChangeTbl[port_num] - 1))));	/* �@�\���W�X�^�ݒ�l�l�Z�o */
				
				/* �w��ID���@�\�ؑ֐ݒ�l�e�[�u���̋@�\�ɑ��� */
				if ( ( set_id == set_data1 ) && ( set_id != NO_USE ) ) {
					state = D_OK;		/* ���� */
					func = i;
				}
			}
		} else {
			state = D_NG; 				/* �ُ� */
		}
	} else {
		state = D_NG; 					/* �ُ� */
	}
	
	/* �������s�X�e�[�^�X�̐��픻�� */
	if ( state == D_OK ) {
		
		(void) IntrDrv_GetMaskLevel( &mask_level );
		(void) IntrDrv_SetMaskLevel( PORT_INTR_LEVEL );					/* �����݋֎~ */
		
		/* �@�\�֑ؑO���� */
		table = C_PortFuncTbl[port_num];								/* �@�\���W�X�^�A�h���X�e�[�u�������� */
		ddr_work = *table[PORT_TABLE_DDR];
		ddr_work |= C_PortCheckBitTbl[bit_num];			/* �f�[�^�������o�͂ɐݒ� */
		*table[PORT_TABLE_DDR] = ddr_work;

		pfr_work = *table[PORT_TABLE_PFR];
		pfr_work &= (UI_8)~C_PortCheckBitTbl[bit_num];	/* �|�[�g�@�\���|�[�g���ɐݒ�*/
		*table[PORT_TABLE_PFR] = pfr_work;
		/* �@�\�ؑ֏��� */
		for ( i = PORT_TABLE_EPFR; i < C_PortFuncChangeNumTbl[port_num]; i++ ) {
			
			set_data2 = set2 + (C_PortFuncChangeTbl[port_num] * func);	/* �@�\���W�X�^�ݒ�l�Z�o */
			set_data3 = *(set_data2 + i);								/* �@�\���W�X�^�ݒ�l�Z�o */
			
			/* �ݒ�l������ */
			if ( set_data3 != D_NULL ) {
				
				/* �}�X�N�����Ȃ�(���̃��W�X�^�l����ύX�Ȃ�)�̏ꍇ */
				if ( (set_data3 & C_PortFuncCheckBitTbl[BIT_FUNC_NUM_15]) == C_PortFuncCheckBitTbl[BIT_FUNC_NUM_15] ) {
					/* �����Ȃ� */
				} else if ( (set_data3 & C_PortFuncCheckBitTbl[BIT_FUNC_NUM_14]) == C_PortFuncCheckBitTbl[BIT_FUNC_NUM_14] ) {
				/* AND�}�X�N����(���W�X�^�l�Ɛݒ�l��&���擾)�̏ꍇ */
					work = *table[i];
					work &= (UI_8)set_data3;	/* �@�\�ؑփ��W�X�^�ɒl��ݒ� */
					*table[i] = work;
				} else if ( (set_data3 & C_PortFuncCheckBitTbl[BIT_FUNC_NUM_13]) == C_PortFuncCheckBitTbl[BIT_FUNC_NUM_13] ) {
				/* OR�}�X�N����(���W�X�^�l�Ɛݒ�l��|���擾)�̏ꍇ */
					work = *table[i];
					work |= (UI_8)set_data3;	/* �@�\�ؑփ��W�X�^�ɒl��ݒ� */
					*table[i] = work;
				} else if ( (set_data3 & C_PortFuncCheckBitTbl[BIT_FUNC_NUM_12]) == C_PortFuncCheckBitTbl[BIT_FUNC_NUM_12] ) {
				/* AND/OR�}�X�N����(���W�X�^�l�Ɛݒ�l��"&"����"|"�����̏���)�̏ꍇ */
					mask = *table[i] & (UI_8)set_data3;						/* �Y���r�b�g��0�Ń}�X�N */
					shift = (UI_8)((BIT_GET & set_data3) >> BIT_SHIFT);		/* 1�Ń}�X�N����r�b�g�ʒu�̒��o */
					mask |= C_PortCheckBitTbl[shift];						/* �Y���r�b�g��1�Ń}�X�N */
					*table[i] = mask;										/* �@�\�ؑփ��W�X�^�ɒl��ݒ� */
				} else {
					/* �����Ȃ� */
				}
			}
		}
		
		/* �f�[�^����&�|�[�g�@�\�ݒ菈�� */
		for ( i = 0;	i < PORT_TABLE_EPFR; i++ ) {
			
			set_data4 = *(set_data2 + i);			/* �@�\���W�X�^�ݒ�l�Z�o */
		
			/* AND�}�X�N����(���W�X�^�l�Ɛݒ�l��&���擾)�̏ꍇ */
			if ( (set_data4 & C_PortFuncCheckBitTbl[BIT_FUNC_NUM_14]) == C_PortFuncCheckBitTbl[BIT_FUNC_NUM_14] ) {
				work = *table[i];
				work &= (UI_8)set_data4;		/* i = 0:�f�[�^�������W�X�^�Ai = 1:�|�[�g�@�\���W�X�^�̐ݒ� */
				*table[i] = work;
			} else if ( (set_data4 & C_PortFuncCheckBitTbl[BIT_FUNC_NUM_13]) == C_PortFuncCheckBitTbl[BIT_FUNC_NUM_13] ) {
			/* OR�}�X�N����(���W�X�^�l�Ɛݒ�l��|���擾)�̏ꍇ */
				work = *table[i];
				work |= (UI_8)set_data4;		/* i = 0:�f�[�^�������W�X�^�Ai = 1:�|�[�g�@�\���W�X�^�̐ݒ� */
				*table[i] = work;
			} else {
				/* �����Ȃ� */
			}
		}
		
		(void) IntrDrv_SetMaskLevel( mask_level );	/* �����ݕ��A */
		
	} else {
		/* �����Ȃ� */
	}

	return state;	/* �߂�l:�������s�X�e�[�^�X */
}
#endif	/* PORT_DRV_FUNC_CHANGE == 1 */

/****************************************************************************************/
/*	[���W���[����]	PortDrv_Init		[����]	GPIO��ײ��ڼ޽������ݒ菈��				*/
/*======================================================================================*/
/*	[�����T�v]	���W�X�^�����ݒ莞�ɖ{�֐����ĂсA���������s���B						*/
/*======================================================================================*/
/*	[�L�q�`��]	void PortDrv_Init( E_INIT_TYPE req  )									*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_INIT_TYPE				req		�F�N�����								*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void PortDrv_Init( E_INIT_TYPE req )
{
	UI_8 i;						/* ���[�v�J�E���^ */
	UI_8 dummy;					/* ���W�X�^�l�i�[�ϐ� */

	/* RAM������ */
	S_SelfcheckState = D_OK;	/* ���t���b�V�����쒆 */
	S_ChkSetReg = D_NG;			/* ���W�X�^�ݒ�̋֎~ */

	/* �|�[�g�f�[�^���W�X�^�����ݒ� */
	if (req == E_INIT_RESET) {			/* CPU���Z�b�g�ɂ��N�� */
		for ( i = 0; i < PORTDRV_PORT_MAX; i++ ) {	/* ���[�v����: ���[�v�J�E���^ < �|�[�g�f�[�^���W�X�^�ԍ�MAX�l */
			*C_PortDataTbl[i] = C_PortInitTbl[i];		/* �|�[�g�f�[�^���W�X�^�̏����ݒ� */
		}
	} else if (req == E_INIT_WAKEUP) {			/* �ȓd�͉����iWAKEUP�j�ɂ��N�� */
		for ( i = 0; i < PORTDRV_PORT_MAX; i++ ) {	/* ���[�v����: ���[�v�J�E���^ < �|�[�g�f�[�^���W�X�^�ԍ�MAX�l */
			if ( C_PortBitMaskPositionTbl[i] != D_NULL ) {	/* �r�b�g�}�X�N���s���r�b�g������ */
				dummy = *C_PortDataDirectTbl[i];			/* ���̓f�[�^�_�C���N�g���[�h���W�X�^�l�擾 */
				dummy &= C_PortBitMaskPositionTbl[i];		/* �r�b�g�}�X�N */
				dummy |= ( UI_8 )( C_PortInitTbl[i] & (UI_8)~C_PortBitMaskPositionTbl[i] );		/* �}�X�N�ӏ��ȊO�̐ݒ� */
				*C_PortDataTbl[i] = dummy;					/* �|�[�g�f�[�^���W�X�^�̏����ݒ� */
			} else {
				*C_PortDataTbl[i] = C_PortInitTbl[i];		/* �|�[�g�f�[�^���W�X�^�̏����ݒ� */
			}
		}
	} else {	/* �r�b�g�}�X�N�����̑ΏۊO */
		/* �������Ȃ� */
	}

	switch ( req ) {
	case E_INIT_RESET:			/* CPU���Z�b�g�ɂ��N�� */
	case E_INIT_WAKEUP:			/* �ȓd�͉����iWAKEUP�j�ɂ��N�� */
		
		/* ���W�X�^�����ݒ� */
		for ( i = 0; i < PORT_MAX; i++ ) {	/* ���[�v����: ���[�v�J�E���^ < �|�[�g�f�[�^���W�X�^�ԍ�MAX�l */
			*C_PortFunctionTbl[i] = 0;								/* �|�[�g�@�\���W�X�^�����ݒ� */
			*C_PullControlTbl[i] = C_PullControlInitTbl[i];			/* �v���A�b�v�_�E�����䃌�W�X�^�����ݒ� */
			*C_PullEnableTbl[i] = C_PullEnableInitTbl[i];			/* �v���A�b�v�_�E�������W�X�^�����ݒ� */
			*C_PortInputLevelTbl[i] = C_PortInputLevelInitTbl[i];	/* �|�[�g���̓��x���I�����W�X�^�����ݒ� */
			*C_ExtendedPortInputLevelTbl[i] = C_ExtendedPortInputLevelInitTbl[i];	/* �g���|�[�g���̓��x���I�����W�X�^�����ݒ� */
			*C_PortOutputDriveTbl[i] = C_PortOutputDriveInitTbl[i];	/* �|�[�g�o�͋쓮���W�X�^�����ݒ� */
			switch ( i ) {											/* ���[�v�J�E���^�̔��� */
			case PORT_NUM_1:	/* ���[�v�J�E���^�F1 */
			case PORT_NUM_2:	/* ���[�v�J�E���^�F2 */
			case PORT_NUM_3:	/* ���[�v�J�E���^�F3 */
			case PORT_NUM_6:	/* ���[�v�J�E���^�F6 */
			case PORT_NUM_7:	/* ���[�v�J�E���^�F7 */
			case PORT_NUM_8:	/* ���[�v�J�E���^�F8 */
				*C_ExtendedPortOutputDriveTbl[i] = C_ExtendedPortOutputDriveInitTbl[i];	/* �g���|�[�g�o�͋쓮���W�X�^�����ݒ� */
				break;
			default:			/* ��L�ȊO */
				break;
			}
		}

		/* �g���|�[�g�@�\���W�X�^�����ݒ� */
		for ( i = 0; i < PORTDRV_EPFR_MAX; i++ ) {	/* ���[�v����: ���[�v�J�E���^ < �|�[�g�f�[�^���W�X�^�ԍ�MAX�l */
			*C_ExtendedPortFunctionTbl[i] = C_ExtendedPortFunctionInitTbl[i];	/* �g���|�[�g�@�\���W�X�^�����ݒ� */
		}

		/* ���W�X�^�����ݒ� */
		for ( i = 0; i < PORT_MAX; i++ ) {	/* ���[�v����: ���[�v�J�E���^ < �|�[�g�f�[�^���W�X�^�ԍ�MAX�l */
			*C_DataDirectionTbl[i] = C_DataDirectionInitTbl[i];		/* �f�[�^�������W�X�^�����ݒ� */
			*C_PortFunctionTbl[i] = C_PortFunctionInitTbl[i];		/* �|�[�g�@�\���W�X�^�����ݒ� */
		}

		PORTEN_GPORTEN = INIT_PORTEN;		/* �|�[�g���͋����W�X�^�����ݒ� */

		break;
		
	case E_INIT_IGN_ON:			/* IGN ON */
		break;
	case E_INIT_RET_NORMAL_VOL:	/* ��d�����A */
		break;
	case E_INIT_INTERVAL_WAKEUP:	/* �Ԍ��N���E�F�C�N�A�b�v */
		PortDrv_Sleep();			/* ���W�X�^������������Ă���̂ōŒ���|�[�g�̏��������K�v */
		break;
	default:					/* ��L�ȊO */
		break;
	}
	
}

/****************************************************************************************/
/*	[���W���[����]	PortDrv_Sleep	[����]	GPIO��ײ��ڼ޽���~�ݒ菈��					*/
/*======================================================================================*/
/*	[�����T�v]	�ȓd�̓��[�h�J�ڎ��ɖ{�֐����ĂсA���������s���B						*/
/*======================================================================================*/
/*	[�L�q�`��]	void PortDrv_Sleep( void )												*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void PortDrv_Sleep( void )
{
	UI_8 i;						/* ���[�v�J�E���^ */
	UI_8 dummy;					/* ���W�X�^�l�i�[�ϐ� */
	
	/* �|�[�g�f�[�^���W�X�^��~�ݒ� */
	for ( i = 0; i < PORTDRV_PORT_MAX; i++ ) {	/* ���[�v����: ���[�v�J�E���^ < �|�[�g�f�[�^���W�X�^�ԍ�MAX�l */
		if ( C_PortBitMaskPositionTbl[i] != D_NULL ) {		/* �r�b�g�}�X�N���s���r�b�g������ */
			dummy = *C_PortDataDirectTbl[i];				/* ���̓f�[�^�_�C���N�g���[�h���W�X�^�l�擾 */
			dummy &= C_PortBitMaskPositionTbl[i];			/* �r�b�g�}�X�N */
			dummy |= ( UI_8 )( C_PortStopTbl[i] & (UI_8)~C_PortBitMaskPositionTbl[i] );		/* �}�X�N�ӏ��ȊO�̐ݒ� */
			*C_PortDataTbl[i] = dummy;						/* �|�[�g�f�[�^���W�X�^�̒�~�ݒ� */
		} else {
			*C_PortDataTbl[i] = C_PortStopTbl[i];			/* �|�[�g�f�[�^���W�X�^�̒�~�ݒ� */
		}
	}

	/* ���W�X�^��~�ݒ� */
	for ( i = 0; i < PORT_MAX; i++ ) {	/* ���[�v����: ���[�v�J�E���^ < �|�[�g�f�[�^���W�X�^�ԍ�MAX�l */
		*C_PortFunctionTbl[i] = 0;								/* �|�[�g�@�\���W�X�^�����ݒ� */
		*C_PullControlTbl[i] = C_PullControlStopTbl[i];			/* �v���A�b�v�_�E�����䃌�W�X�^��~�ݒ� */
		*C_PullEnableTbl[i] = C_PullEnableStopTbl[i];			/* �v���A�b�v�_�E�������W�X�^��~�ݒ� */
		*C_PortInputLevelTbl[i] = C_PortInputLevelStopTbl[i];	/* �|�[�g���̓��x���I�����W�X�^��~�ݒ� */
		*C_ExtendedPortInputLevelTbl[i] = C_ExtendedPortInputLevelStopTbl[i];	/* �g���|�[�g���̓��x���I�����W�X�^��~�ݒ� */
		*C_PortOutputDriveTbl[i] = C_PortOutputDriveStopTbl[i];	/* �|�[�g�o�͋쓮���W�X�^��~�ݒ� */
		switch ( i ) {											/* ���[�v�J�E���^�̔��� */
		case PORT_NUM_1:		/* ���[�v�J�E���^�F1 */
		case PORT_NUM_2:		/* ���[�v�J�E���^�F2 */
		case PORT_NUM_3:		/* ���[�v�J�E���^�F3 */
		case PORT_NUM_6:		/* ���[�v�J�E���^�F6 */
		case PORT_NUM_7:		/* ���[�v�J�E���^�F7 */
		case PORT_NUM_8:		/* ���[�v�J�E���^�F8 */
			*C_ExtendedPortOutputDriveTbl[i] = C_ExtendedPortOutputDriveStopTbl[i];	/* �g���|�[�g�o�͋쓮���W�X�^��~�ݒ� */
			break;
		default:				/* ��L�ȊO */
			break;
		}
	}

	/* �g���|�[�g�@�\���W�X�^��~�ݒ� */
	for ( i = 0; i < PORTDRV_EPFR_MAX; i++ ) {	/* ���[�v����: ���[�v�J�E���^ < �g���|�[�g�@�\���W�X�^�� */
		*C_ExtendedPortFunctionTbl[i] = C_ExtendedPortFunctionStopTbl[i];	/* �g���|�[�g�@�\���W�X�^��~�ݒ� */
	}

	/* ���W�X�^��~�ݒ� */
	for ( i = 0; i < PORT_MAX; i++ ) {	/* ���[�v����: ���[�v�J�E���^ < �|�[�g�f�[�^���W�X�^�ԍ�MAX�l */
		*C_DataDirectionTbl[i] = C_DataDirectionStopTbl[i];		/* �f�[�^�������W�X�^��~�ݒ� */
		*C_PortFunctionTbl[i] = C_PortFunctionStopTbl[i];		/* �|�[�g�@�\���W�X�^��~�ݒ� */
	}
	
	PORTEN_GPORTEN = STOP_PORTEN;		/* �|�[�g���͋����W�X�^��~�ݒ� */

}

/****************************************************************************************/
/*	[���W���[����]	PortDrv_Refresh	[����]												*/
/*======================================================================================*/
/*	[�����T�v]	���W�X�^�Đݒ莞�ɖ{�֐����ĂсA���������s���B							*/
/*======================================================================================*/
/*	[�L�q�`��]	void PortDrv_Refresh( void )											*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void PortDrv_Refresh( void )
{
	UI_8 i;						/* ���[�v�J�E���^ */
	
	if ( S_SelfcheckState != D_NG) {/* �Z���t�`�F�b�N���쒆? */
		S_ChkSetReg = D_NG;	/* ���W�X�^�ݒ�̋֎~ */
		for ( i = 0; i < PORT_MAX; i++ ) {	/* ���[�v����: ���[�v�J�E���^ < �|�[�g�f�[�^���W�X�^�ԍ�MAX�l */
			*C_DataDirectionTbl[i] = C_DataDirectionRefreshTbl[i];	/* �f�[�^�������W�X�^�Đݒ� */
		}
	} else {
		S_ChkSetReg = D_OK;	/* ���W�X�^�ݒ�̋��� */
	}
}

/****************************************************************************************/
/*	[���W���[����]	PortDrv_SetSelfCheckStat	[����]	�Z���t�`�F�b�N��Ԑݒ菈��		*/
/*======================================================================================*/
/*	[�����T�v]	���C���e�X�g�@�\������Z���t�`�F�b�N��Ԃ��擾���ARAM�֊i�[����			*/
/*======================================================================================*/
/*	[�L�q�`��]	void PortDrv_SetSelfCheckStat( SI_8 stat )								*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	SI_8		stat	���W�X�^���t���b�V��������						*/
/*		�iD_NG :���t���b�V������NG(�Z���t�`�F�b�N�쓮) / D_NG�ȊO :���t���b�V�����쒆�j	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	�ݒ肳��Ȃ����RAM�������ɂ�胊�t���b�V�����쒆�i0 : D_OK�����j�ƂȂ�	*/
/****************************************************************************************/
void PortDrv_SetSelfCheckStat(SI_8 stat)
{
	S_SelfcheckState = stat;		/* �Z���t�`�F�b�N��Ԃ�ݒ�  */
}

/****************************************************************************************/
/*	[���W���[����]	PortDrv_GetPermissionStat	[����]	GPIO�̍Đݒ苖�֎~��Ԏ�		*/
/*======================================================================================*/
/*	[�����T�v]	���W�X�^�ݒ苖�E�֎~��Ԃ��O���@�\�֌��J����							*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 PortDrv_GetPermissionStat( void )									*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8		���W�X�^�ݒ�̏�ԁiD_NG�F�֎~/D_OK�F���j					*/
/*======================================================================================*/
/*	[ ��  �l ]	���C�����ōs�Ȃ���PortDrv_Refresh�����̓r���ŁA�Z���t�`�F�b�N��Ԑݒ�	*/
/*	�����i10mss�����j���Ă΂ꂽ��A���C�����s�Ȃ���O�ɑ����ăf�[�^�o�͂��s�Ȃ���	*/
/*�i�j�]����10ms�������A2��A���ōs�Ȃ���j�P�[�X��������邽�߁A�ݒ�̊m�F���s�Ȃ��B	*/
/****************************************************************************************/
SI_8 PortDrv_GetPermissionStat(void)
{
	return S_ChkSetReg;			/* ���W�X�^�ݒ�̋֎~/�����  */
}

/****************************************************************************************/
/*	[���W���[����]	PortDrv_SetDirectionBit	[����]	�w��|�[�g�̎w��Bit�����ݒ�			*/
/*======================================================================================*/
/*	[�����T�v]	�w��|�[�g�̎w��Bit�̃f�[�^������ݒ肷��								*/
/*======================================================================================*/
/*	[�L�q�`��]	SI_8 PortDrv_SetDirectionBit( E_PORTDRV_PORT_NUM port_num, 				*/
/*				E_PORTDRV_BIT_NUM bit_num, UI_8 set_direction )							*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_PORTDRV_PORT_NUM port_num	�w��|�[�g�ԍ�	E_PORTDRV_PORT_NUM:enum��`	*/
/*				E_PORTDRV_BIT_NUM bit_num	�w��r�b�g�ʒu	E_PORTDRV_BIT_NUM:enum��`	*/
/*				UI_8 set_direction	�ݒ���											*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	SI_8 state;	�������s�X�e�[�^�X											*/
/*======================================================================================*/
/*	[ ��  �l ]	�Ȃ�																	*/
/****************************************************************************************/
SI_8 PortDrv_SetDirectionBit( E_PORTDRV_PORT_NUM port_num, E_PORTDRV_BIT_NUM bit_num, UI_8 set_direction )
{
	UI_8 ddr_work;				/* �f�[�^�������W�X�^���Z�pwork�ϐ� */
	SI_8 state; 				/* �������s�X�e�[�^�X */
	UI_16 mask_level; 			/* �}�X�N���x���i�[�ϐ� */
	
	/* ����1�F�w��|�[�g�ԍ��̃f�[�^�͈͔��� */
	if ( port_num < PORTDRV_PORT_MAX ) {
		
		/* ����2�F�w��r�b�g�ʒu�̃f�[�^�͈͔��� */
		if ( bit_num < PORTDRV_BIT_MAX ) {
			
			/* ����3�F�ݒ���̃f�[�^�͈͔��� */
			if ( ( set_direction == D_LOW ) || ( set_direction == D_HIGH ) ) {
				state = D_OK;	/* ���� */	
			} else {
				state = D_NG; 	/* �ُ� */
			}
		} else {
			state = D_NG; 		/* �ُ� */
		}
	} else {
		state = D_NG; 			/* �ُ� */
	}
	
	/* �������s�X�e�[�^�X�̐��픻�� */
	if ( state == D_OK ) {
		

		(void) IntrDrv_GetMaskLevel( &mask_level );
		(void) IntrDrv_SetMaskLevel( PORT_INTR_LEVEL );			/* �����݋֎~ */
		ddr_work = *C_DataDirectionTbl[port_num];
		/* �ݒ���Hi���x���𔻒� */
		if ( ( set_direction == D_HIGH ) ) {
			ddr_work |= C_PortCheckBitTbl[bit_num];	/* �|�[�g�������W�X�^�̎w��r�b�g�ݒ� */
		} else {
			ddr_work &= (UI_8)~C_PortCheckBitTbl[bit_num];	 /* �|�[�g�������W�X�^�̎w��r�b�g�ݒ� */
		}
		*C_DataDirectionTbl[port_num] = ddr_work;
		(void) IntrDrv_SetMaskLevel( mask_level );				/* �����ݕ��A */
		
	} else {
		/* �����Ȃ� */
	}
	
	return state;	/* �߂�l:�������s�X�e�[�^�X */
}
