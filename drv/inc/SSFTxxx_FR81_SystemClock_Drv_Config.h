/************************************************************************************************/
/* �q�於		:	�@��ˑ����W���[��															*/
/* �@�햼		:	-																			*/
/* ϲ�ݿ��ðϖ�	:	-																			*/
/*==============================================================================================*/
/* �쐬̧�ٖ�	:	SSFTxxx_FR81_SystemClock_Drv_Config.h										*/
/* 				:	���Ѹۯ���ײ��ͯ��															*/
/*==============================================================================================*/
/* �Ώ�ϲ��		:	MB91570 Series																*/
/*==============================================================================================*/
/* �쐬�ް�ޮ�	:	010103																		*/
/* �쐬�N����	:	2013.05.23																	*/
/* �쐬��		:	K.Uchiyama																	*/
/*----------------------------------------------------------------------------------------------*/
/* �ύX����		:																				*/
/* [010103]		:	�V�K�쐬																	*/
/************************************************************************************************/
#ifndef __MB91570_SYSTEMCLOCK_DRV_CONFIG_H__
#define __MB91570_SYSTEMCLOCK_DRV_CONFIG_H__

/* �����萔 */
#ifdef __MB91570_SYSTEMCLOCK_DRV_INTERNAL__
/*==============================================================================*/
/*	typedef��`�i�O������J�j													*/
/*==============================================================================*/
/* �Ȃ� */


/*==============================================================================*/
/*	define��`�i�O������J�j													*/
/*==============================================================================*/


#define INIT_DIVR0		(0x00)		/* �����ݒ背�W�X�^0 (���g�p) */
									/*	<07-05>	DIVB	: 000		(�x�[�X�N���b�N�����ݒ�r�b�g ��(�����Ȃ�)) */
									/*	<04-00>	-		: 00000		(�\��bit) */
									/* ------------------------------------------------------------------- */
#define INIT_DIVR1		(0x10)		/* �����ݒ背�W�X�^1 */
									/*	<07>	TSTP	: 0			(�O���o�X�N���b�N��~���r�b�g ��~���Ȃ�) */
									/*	<06-04>	DIVT	: 001		(�O���o�X�N���b�N�����ݒ�r�b�g ��/2(2����)) */
									/*	<03-00>	-		: 0000		(�\��bit) */
									/* ------------------------------------------------------------------- */
#define INIT_DIVR2		(0x40)		/* �����ݒ背�W�X�^2 (���g�p) */
									/*	<07-04>	DIVP	: 0100		(���ӃN���b�N�����ݒ�r�b�g ��/5(5����)) */
									/*	<03-00>	-		: 0000		(�\��bit) */
									/* ------------------------------------------------------------------- */
#define INIT_CSELR		(0x62)		/* �N���b�N�\�[�X�ݒ背�W�X�^ (���g�p) */
									/*	<07>	SCEN	: 0			(�T�u�N���b�N���U�r�b�g ��~) */
									/*	<06>	PCEN	: 1			(PLL ���U�r�b�g ���U) */
									/*	<05>	MCEN	: 1			(���C���N���b�N���U �J�n) */
									/*	<04-02>	-		: 000		(�\��bit) */
									/*	<01-00>	CKS		: 10		(�\�[�X�N���b�N�I���r�b�g PLL/SSCG �N���b�N) */
									/* ------------------------------------------------------------------- */
#define INIT_MTMCR		(0x00)		/* ���C���^�C�}���䃌�W�X�^ (���g�p) */
									/*	<07>	MTIF	: 0			(���C���^�C�}�����݃t���O�r�b�g ��~) */
									/*	<06>	MTIE	: 0			(���C���^�C�}�����݋��r�b�g ��~) */
									/*	<05>	MTC		: 1			(���C���^�C�}�N���A�r�b�g �J�n) */
									/*	<04>	MTE		: 000		(���C���^�C�}���싖�r�b�g ) */
									/*	<03-00>	MTS		: 10		(���C���^�C�}�����I���r�b�g PLL/SSCG �N���b�N) */
									/* ------------------------------------------------------------------- */
#define INIT_STMCR		(0x00)		/* �T�u�^�C�}���䃌�W�X�^ (���g�p) */
									/*	<07>	STIF	: 0			(�T�u�^�C�}�����݃t���O�r�b�g �N���A) */
									/*	<06>	STIE	: 0			(�T�u�^�C�}�����݃r�b�g �����݋֎~(�����l)) */
									/*	<05>	STC		: 0			(�T�u�^�C�}�N���A �������Ȃ�) */
									/*	<04>	STE		: 0			(�T�u�^�C�}���싖�r�b�g ����֎~) */
									/*	<03>	-		: 0			(�\��bit) */
									/*	<02-00>	STS		: 000		(�T�u�^�C�}�����I���r�b�g 2^8*�T�u�N���b�N����) */
									/* ------------------------------------------------------------------- */
#define INIT_PLLCR		(0x00B0)	/* PLL�ݒ背�W�X�^ (���g�p) */
									/* <15-08>	-		: 0			(�\��bit) */
									/* <07-04>	POSW	: 1011		(PLL�N���b�N���U����҂��I���r�b�g 2^12*���C���N���b�N����) */
									/* <03-00>	PDS		: 0000		(PLL���̓N���b�N�����I���r�b�g PLL�����SSCG���̓N���b�N = ���C���N���b�N ) */
									/* ------------------------------------------------------------------- */
#define INIT_CSTBR		(0x0C)		/* ���U����҂��ݒ背�W�X�^ */
									/* <07>		-		: 0			(�\��bit) */
									/* <06-04>	SOSW	: 000		(�T�u�N���b�N���U����҂��I���r�b�g 2^8*�T�u�N���b�N�����i�����l) ) */
									/* <03-00>	MOSW	: 1100		(���C���N���b�N���U����҂��I���r�b�g 2^17*���C���N���b�N����) */
									/* ------------------------------------------------------------------- */
#define INIT_PTMCR		(0x00)		/* PLL�N���b�N���U����҂��^�C�}�ݒ背�W�X�^ (���g�p) */
									/* <07>		PTIF	: 0			(PLL�N���b�N���U����҂��^�C�}�����݃t���O) */
									/* <06>		PTIE	: 0			(PLL�N���b�N���U����҂��^�C�}�����݋��r�b�g �����݋֎~�i�����l�j�j*/
								 	/* <05-00>	-		: 0			(�\��bit ) */
								 	/* ------------------------------------------------------------------- */
#define INIT_CCPSSELR	(0x00)		/* PLL/SSCG�N���b�N�I�����W�X�^ (���g�p) */
									/* <07-01>	-		: 0			(�\��bit) */
									/* <00>		PCSEL	: 0			(PLL/SSCG�N���b�N�\�[�X�I���r�b�g PLL��I��) */
									/* ------------------------------------------------------------------- */
#define INIT_CCPSDIVR	(0x11)		/* PLL/SSCG�o�̓N���b�N�����ݒ背�W�X�^ (���g�p) */
									/* <07>		-		: 0			(�\��bit) */
									/* <06-04>	PODS	: 001		(PLL�}�N�����U�N���b�N������I���r�b�g PLL�N���b�N��PLL�}�N�����U�N���b�N/4) */
									/* <03>		-		: 0			(�\��bit) */
									/* <02-00>	SODS	: 001		(SSCG�}�N�����U�N���b�N������I���r�b�g SSCG�N���b�N��SSCG�}�N�����U�N���b�N/4) */
									/* ------------------------------------------------------------------- */
#define INIT_CCPLLFBR	(0x4F)		/* PLL�t�B�[�h�o�b�N�����ݒ背�W�X�^ (���g�p) */
									/* <07>		-		: 0			(�\��bit) */
									/* <06-00>	IDIV	: 1001111	(PLL�}�N��FB���͕�����ݒ�r�b�g 80) */
									/* ------------------------------------------------------------------- */
#define INIT_CCSSFBR0	(0x09)		/* SSCG�t�B�[�h�o�b�N�����ݒ背�W�X�^0 (���g�p) */
									/* <07-06>	-		: 00		(�\��bit) */
									/* <05-00>	NDIV	: 001001	(SSCG�}�N��FB����N������ݒ�r�b�g 10 ) */
									/* ------------------------------------------------------------------- */
#define INIT_CCSSFBR1	(0x07)		/* SSCG�t�B�[�h�o�b�N�����ݒ背�W�X�^1 (���g�p) */
									/* <07-05>	-		: 0			(�\��bit) */
									/* <04-00>	PDIV	: 00111		(SSCG�}�N��FB����P������ݒ�r�b�g 8) */
									/* ------------------------------------------------------------------- */
#define INIT_CCSSCCR0	(0x00)		/* SSCG�R���t�B�O�ݒ背�W�X�^0 (���g�p) */
									/* <07-04>	-		: 0			(�\��bit) */
									/* <03-02>	SFREQ	: 00		(�g�U���W�����[�V�������g���ݒ�r�b�g DownSpread) */
									/* <01>		SMODE	: 0			(�g�U���W�����[�V�������[�h�I���r�b�g DownSpread) */
									/* <00>		SSEN	: 0			(�g�U�X�y�N�g�����r�b�g �g�U�X�y�N�g��������) */
									/* ------------------------------------------------------------------- */
#define INIT_CCSSCCR1	(0x0000)	/* SSCG�R���t�B�O�ݒ背�W�X�^1 (���g�p) */
									/* <15-13>	RATESEL	: 000		(�g�U���W�����[�V�������[�g�I���r�b�g 0.5%) */
									/* <12-00>	-		: 0			(�\��bit) */
									/* ------------------------------------------------------------------- */
#define INIT_CCCGRCR0	(0x00)		/* �N���b�N�M�A�R���t�B�O�ݒ背�W�X�^0 (���g�p) */
									/* <07-06>	GRSTS	: 00		(�N���b�N�M�A�X�e�[�^�X�t���O�r�b�g ) */
									/* <05-02>	-		: 0			(�\��bit) */
									/* <01>		GRSTR	: 0			(�N���b�N�M�A�X�^�[�g�r�b�g ����ɉe���Ȃ�) */
									/* <00>		GREN	: 0			(�N���b�N�M�A�C�l�[�u���r�b�g �N���b�N�M�A���g�p���Ȃ�) */
									/* ------------------------------------------------------------------- */
#define INIT_CCCGRCR1	(0x00)		/* �N���b�N�M�A�R���t�B�O�ݒ背�W�X�^1 (���g�p) */
									/* <07-06>	GRST	: 00		(�N���b�N�M�A�X�e�b�v�I���r�b�g 1�X�e�b�v) */
									/* <05-00>	GRSTN	: 0			(�N���b�N�M�A�J�n�X�e�b�v�I���r�b�g 0�X�e�b�v) */
									/* ------------------------------------------------------------------- */
#define INIT_CCCGRCR2	(0x00)		/* �N���b�N�M�A�R���t�B�O�ݒ背�W�X�^2 (���g�p) */
									/* <07-00>	GRLP	: 0			(�N���b�N�M�A�J�Ԃ��񐔑I���r�b�g ���[�v1��) */
									/* ------------------------------------------------------------------- */
#define INIT_CCRTSELR	(0x00)		/* RTC/PMU�N���b�N�I�����W�X�^ (���g�p) */
									/* <07>		CST		: 0			(�N���b�N�\�[�X�I���X�e�[�^�X���j�^�r�b�g �N���b�N�؂�ւ�����) */
									/* <06-01>	-		: 0			(�\��bit) */
									/* <00>		CSC		: 0			(�N���b�N�\�[�X�I���r�b�g ���C�����U�N���b�N) */
									/* ------------------------------------------------------------------- */
#define INIT_CCPMUCR0	(0x00)		/* PMU�N���b�N�����ݒ背�W�X�^0 (���g�p) */
									/* <07>		FST		: 0			(F������X�e�[�^�X���j�^�r�b�g �������ݒl�𔽉f�ς�) */
									/* <06-02>	-		: 0			(�\��bit) */
									/* <01-00>	FDOV	: 0			(F������ݒ�r�b�g 128����(�����l)) */
									/* ------------------------------------------------------------------- */
#define INIT_CCPMUCR1	(0x00)		/* PMU�N���b�N�����ݒ背�W�X�^1 (���g�p) */
									/* <07>		GST		: 0			(G������X�e�[�^�X���j�^�r�b�g �������ݒl�𔽉f�ς�) */
									/* <06-05>	-		: 0			(�\��bit) */
									/* <04-00>	GDIV	: 0			(G������ݒ�r�b�g �������Ȃ��i�����l�j�j */
									/* ------------------------------------------------------------------- */
#define INIT_SACR		(0x00)		/* ����/�񓯊��R���g���[�����W�X�^ (���g�p) */
									/* <07-01>	-		: 0			(�\��bit) */
									/* <00>		M		: 0			(���ӃN���b�N(PCLK2)�̓���/�񓯊��ݒ背�W�X�^�r�b�g ����(CPU/���Ӌ���PLL/SSCG �N���b�N)) */
									/* ------------------------------------------------------------------- */
#define INIT_PICD		(0x00)		/* ���ӃC���^�t�F�[�X�N���b�N���� (���g�p) */
									/* <07-04>	-		: 0			(�\��bit) */
									/* <03-00>	PDIV	: 0000		(���ӃN���b�N������ݒ�r�b�g �����Ȃ�) */

#define INIT_CCRTSELR_CSC	(0)		/* RTC/PMU�N���b�N�I�����W�X�^ CSC�r�b�g�����ݒ�l 0:���C���N���b�N��I�� */
#define INIT_CCPMUCR0_FDIV	(0)		/* PMU�N���b�N�����ݒ背�W�X�^0 FDIV �����ݒ�l 00:128���� */
#define INIT_CCPMUCR1_GDIV	(0)		/* PMU�N���b�N�����ݒ背�W�X�^1 GDIV �����ݒ�l 0000:�����Ȃ� */

#define STOP_DIVR0		(0x00)		/* INIT_DIVR0�Ɠ����ݒ�l (���g�p) */
#define STOP_DIVR1		(0x10)		/* �����ݒ背�W�X�^1 */
									/*	<07>	TSTP	: 0			(�O���o�X�N���b�N��~���r�b�g ��~���Ȃ�) */
									/*	<06-04>	DIVT	: 001		(�O���o�X�N���b�N�����ݒ�r�b�g ��/2(2����)) */
									/*	<03-00>	-		: 0000		(�\��bit) */
#define STOP_DIVR2		(0x40)		/* INIT_DIVR2�Ɠ����ݒ�l (���g�p) */
#define STOP_CSELR		(0x20)		/* �N���b�N�\�[�X�ݒ背�W�X�^ (���g�p) */
									/*	<07>	SCEN	: 0			(�T�u�N���b�N���U�r�b�g ��~) */
									/*	<06>	PCEN	: 0			(PLL ���U�r�b�g ��~) */
									/*	<05>	MCEN	: 1			(���C���N���b�N���U �J�n) */
									/*	<04-02>	-		: 000		(�\��bit) */
									/*	<01-00>	CKS		: 00		(�\�[�X�N���b�N�I���r�b�g ���C���N���b�N��2����) */
									/* ------------------------------------------------------------------- */
#define STOP_MTMCR		(0x00)		/* INIT_MTMCR�Ɠ����ݒ�l (���g�p) */
#define STOP_STMCR		(0x00)		/* INIT_STMCR�Ɠ����ݒ�l (���g�p) */
#define STOP_PLLCR		(0x00B0)	/* INIT_PLLCR�Ɠ����ݒ�l (���g�p) */
#define STOP_CSTBR		(0x0C)		/* INIT_CSTBR�Ɠ����ݒ�l */
#define STOP_PTMCR		(0x00)		/* INIT_PTMCR�Ɠ����ݒ�l (���g�p) */
#define STOP_CCPSSELR	(0x00)		/* INIT_CCPSSELR�Ɠ����ݒ�l (���g�p) */
#define STOP_CCPSDIVR	(0x11)		/* INIT_CCPSDIVR�Ɠ����ݒ�l (���g�p) */
#define STOP_CCPLLFBR	(0x4F)		/* INIT_CCPLLFBR�Ɠ����ݒ�l (���g�p) */
#define STOP_CCSSFBR0	(0x09)		/* INIT_CCSSFBR0�Ɠ����ݒ�l (���g�p) */
#define STOP_CCSSFBR1	(0x07)		/* INIT_CCSSFBR1�Ɠ����ݒ�l (���g�p) */
#define STOP_CCSSCCR0	(0x00)		/* INIT_CCSSCCR0�Ɠ����ݒ�l (���g�p) */
#define STOP_CCSSCCR1	(0x0000)	/* INIT_CCSSCCR1�Ɠ����ݒ�l (���g�p) */
#define STOP_CCCGRCR0	(0x00)		/* INIT_CCCGRCR0�Ɠ����ݒ�l (���g�p) */
#define STOP_CCCGRCR1	(0x00)		/* INIT_CCCGRCR1�Ɠ����ݒ�l (���g�p) */
#define STOP_CCCGRCR2	(0x00)		/* INIT_CCCGRCR2�Ɠ����ݒ�l (���g�p) */
#define STOP_CCRTSELR	(0x00)		/* INIT_CCRTSELR�Ɠ����ݒ�l (���g�p) */
#define STOP_CCPMUCR0	(0x00)		/* INIT_CCPMUCR0�Ɠ����ݒ�l (���g�p) */
#define STOP_CCPMUCR1	(0x00)		/* INIT_CCPMUCR1�Ɠ����ݒ�l (���g�p) */
#define STOP_SACR		(0x00)		/* INIT_SACR�Ɠ����ݒ�l (���g�p) */
#define STOP_PICD		(0x00)		/* INIT_PICD�Ɠ����ݒ�l (���g�p) */

#define INIT_PLLCR_POSW					(0xB)		/* PLL/SSCG�N���b�N����҂����Ԃ�ݒ� 1024��s*/
#define INIT_CCPSSELR_PCSEL				(0x0)		/* PLL��I�� */
#define INIT_PLLCR_PDS					(0x0)		/* PLL�̒��{����ݒ�(CAN�AOCD�p) 4MHz*/
#define INIT_CCPSDIVR_PODS				(0x1)		/* PLL�̒��{����ݒ�(CAN�AOCD�p) 80MHz*/
#define INIT_CCPLLFBR_IDIV				(0x4F)		/* PLL�̒��{����ݒ�(CAN�AOCD�p) 320MHz */
#define INIT_CCPSDIVR_SODS				(0x1)		/* SSCG�̒��{����ݒ� 80MHz */
#define INIT_CCSSFBR0_NDIV				(0x8)		/* SSCG�̒��{����ݒ� 288MHz */
#define INIT_CCSSFBR1_PDIV				(0x7)		/* SSCG�̒��{����ݒ� 288MHz */
#define INIT_CCSSCCR0_SFREQ				(0x0)		/* SSCG�̊g�U���@��ݒ� 1/1024 */
#define INIT_CCSSCCR0_SMODE				(0x1)		/* SSCG�̊g�U���@��ݒ� CenterSpread */
#define INIT_CCSSCCR0_SSEN				(0x1)		/* SSCG�̊g�U���@��ݒ� �g�U�X�y�N�g�������� */
#define INIT_CCSSCCR1_RATESEL			(0x6)		/* SSCG�̊g�U���@��ݒ� 5% */
#define INIT_CCCGRCR0_GREN				(0x1)		/* �M�A��L����Ԃɐݒ� */
#define INIT_CCCGRCR1_GRSTP				(0x0)		/* �M�A�X�e�b�v�̐ݒ� �X�e�b�v��1 */
#define INIT_CCCGRCR1_GRSTN				(0x0)		/* �M�A�X�e�b�v�̐ݒ� �J�n�X�e�b�v��0 */
#define INIT_CCCGRCR2_GRLP				(0xF)		/* �M�A�X�e�b�v�̐ݒ� 16�� */
#define INIT_CSELR_PCEN					(0x1)		/* PLL���U */
#define INIT_DIVR0_DIVB					(0x0)		/* �����Ȃ� */
#define INIT_DIVR1_DIVT					(0x1)		/* ��/2 2���� (���g�p) */
#define INIT_DIVR2_DIVP					(0x4)		/* ��/5 5���� */
#define INIT_PICD_PDIV					(0x9)		/* PLL�N���b�N��10����(INIT_SACR_M��񓯊��Ƃ����ꍇ�̂ݗL��) */
#define INIT_SACR_M						(0x0)		/* ����(SSCG���g���ꍇ�̂ݗL��) */
#define INIT_CSELR_CKS_SELECT_PLL_SSCG	(0x2)		/* PLL/SSCG�N���b�N��I�� */
#define INIT_CSELR_CKS_SELECT_MAIN_00	(0x0)		/* ���C���N���b�N��I�� */
#define INIT_CSELR_CKS_SELECT_MAIN_01	(0x1)		/* ���C���N���b�N��I�� (���g�p) */
#define INIT_CSELR_CKS_SELECT_SUB		(0x3)		/* �T�u�N���b�N��I�� (���g�p) */
#define INIT_CCCGRCR0_GRSTR				(0x1)		/* �M�A�J�n */
#define STOP_CCCGRCR0_GRSTR				(0x1)		/* �M�A�J�n (���g�p) */
#define STOP_CSELR_CKS					(0x0)		/* ���C���N���b�N��I�� */
#define STOP_CSELR_PCEN					(0x0)		/* PLL/SSCG�̓�����~�i���U���~) */
#define STOP_DIVR0_DIVB					(0x0)		/* �����Ȃ� */
#define STOP_DIVR1_DIVT					(0x1)		/* ��/2 2���� (���g�p) */
#define STOP_DIVR2_DIVP					(0x4)		/* ��/5 5���� */
#define STOP_SACR_M						(0x0)		/* ���� */

#define DIVR0_INIT_MASK					(0xE0U)			/* �����ݒ背�W�X�^0�����l�}�X�N (���g�p) */
#define DIVR2_INIT_MASK					(0xF0U)			/* �����ݒ背�W�X�^2�����l�}�X�N (���g�p) */ 
#define CSELR_INIT_MASK					(0xE3U)			/* �N���b�N�\�[�X�ݒ背�W�X�^�����l�}�X�N (���g�p) */
#define PLLCR_INIT_MASK					(0x00FF)		/* PLL�ݒ背�W�X�^�����l�}�X�N (���g�p) */
#define CCPSSELR_INIT_MASK				(0x01)			/* PLL/SSCG�N���b�N�I�����W�X�^�����l�}�X�N (���g�p) */
#define CCPSDIVR_INIT_MASK				(0x77)			/* PLL/SSCG�o�̓N���b�N�����ݒ背�W�X�^�����l�}�X�N (���g�p) */
#define CCPLLFBR_INIT_MASK				(0x7F)			/* PLL�t�B�[�h�o�b�N�����ݒ背�W�X�^�����l�}�X�N (���g�p) */
#define CCSSFBR0_INIT_MASK				(0x3F)			/* SSCG�t�B�[�h�o�b�N�����ݒ背�W�X�^0�����l�}�X�N (���g�p) */
#define CCSSFBR1_INIT_MASK				(0x1F)			/* SSCG�t�B�[�h�o�b�N�����ݒ背�W�X�^1�����l�}�X�N (���g�p) */
#define CCSSCCR0_INIT_MASK			(0x0F)			/* SSCG�R���t�B�O�ݒ背�W�X�^0�����l�}�X�N (���g�p) */
#define CCSSCCR1_INIT_MASK			(0xE000U)		/* SSCG�R���t�B�O�ݒ背�W�X�^1�����l�}�X�N (���g�p) */
#define CCCGRCR0_INIT_MASK			(0x03)			/* �N���b�N�M�A�R���t�B�O�ݒ背�W�X�^0�����l�}�X�N (���g�p) */
#define CCCGRCR1_INIT_MASK			(0xFFU)			/* �N���b�N�M�A�R���t�B�O�ݒ背�W�X�^1�����l�}�X�N (���g�p) */
#define CCCGRCR2_INIT_MASK			(0xFFU)			/* �N���b�N�M�A�R���t�B�O�ݒ背�W�X�^2�����l�}�X�N (���g�p) */
#define SACR_INIT_MASK				(0x00)			/* ����/�񓯊��R���g���[�����W�X�^�����l�}�X�N (���g�p) */
#define PICD_INIT_MASK				(0x0F)			/* ���ӃC���^�t�F�[�X�N���b�N���������l�}�X�N (���g�p) */
#define DIVR0_STOP_MASK				(0xE0U)			/* �����ݒ背�W�X�^0��~�l�}�X�N (���g�p) */
#define DIVR2_STOP_MASK				(0xF0U)			/* �����ݒ背�W�X�^2��~�l�}�X�N (���g�p) */
#define CSELR_STOP_MASK				(0xE3U)			/* �N���b�N�\�[�X�ݒ背�W�X�^��~�l�}�X�N (���g�p) */
#define CCCGRCR0_STOP_MASK			(0x02)			/* �N���b�N�M�A�R���t�B�O�ݒ背�W�X�^0��~�l�}�X�N (���g�p) */
#define SACR_STOP_MASK				(0x0)			/* ����/�񓯊��R���g���[�����W�X�^��~�l�}�X�N (���g�p) */
#define CSELR_CKS_MCLK_DIV2			(0x0)			/* �N���b�N�\�[�X�ݒ背�W�X�^�\�[�X�N���b�N�I���F���C���N���b�N(MCLK)��2���� (���g�p) */
#define CSELR_CKS_PLL_SSCG			(0x2)			/* �N���b�N�\�[�X�ݒ背�W�X�^�\�[�X�N���b�N�I���FPLL/SSCG�N���b�N (���g�p) */
#define CMONR_CKM_MCLK_DIV2			(0x0)			/* �N���b�N�\�[�X�Ď����W�X�^�\�[�X�N���b�N�\���F���C���N���b�N(MCLK)��2���� */
#define CMONR_CKM_PLL_SSCG			(0x2)			/* �N���b�N�\�[�X�Ď����W�X�^�\�[�X�N���b�N�\���FPLL/SSCG�N���b�N */
#define CMONR_PCRDY_START			(0x1)			/* �N���b�N�\�[�X�Ď����W�X�^PLL�N���b�N���f�B�F���U�J�n (���g�p) */
#define CCCGRCR0_GRSTS_STOP_LOW		(0x0)			/* �N���b�N�M�A�R���t�B�O�ݒ背�W�X�^0�N���b�N�M�A�X�e�[�^�X�t���O�F�ᑬ�Œ�~ */
#define CCCGRCR0_GRSTS_GEAR_UP		(0x1)			/* �N���b�N�M�A�R���t�B�O�ݒ背�W�X�^0�N���b�N�M�A�X�e�[�^�X�t���O�FGEAR UP ���쒆 */
#define CCCGRCR0_GRSTS_STOP_HIGH	(0x2)			/* �N���b�N�M�A�R���t�B�O�ݒ背�W�X�^0�N���b�N�M�A�X�e�[�^�X�t���O�F�����Œ�~ */
#define CCCGRCR0_GRSTS_GEAR_DOWN	(0x3)			/* �N���b�N�M�A�R���t�B�O�ݒ背�W�X�^0�N���b�N�M�A�X�e�[�^�X�t���O�FGEAR DOWN ���쒆 */
#define D_MAINWAIT_RESET			(12288)			/* ���Z�b�g�N�����̃��C���N���b�N���U����҂�����( 2^15 �~ 4MHz = 8192us ) 1.5�{�_�� */
#define D_MAINWAIT_WAKE_UP			(49152)			/* Wake Up�N�����̃��C���N���b�N���U����҂�����( 2~17 �~ 4MHz = 32768��s ) 1.5�{�_��  */
#define D_PLLWAIT					(1536)			/* PLL���U����҂�����( 2^12 �~ 4MHz = 1024us) 1.5�{�_�� */
#define D_PLL_MAINCLKWAIT			(90)			/* PLL�˃��C���N���b�N�؊��������҂�����(4800cycle / 80MHz = 60us) 1.5�{�_�� */
#define D_MAINCLKWAIT				(3600)			/* ���C���N���b�N�˃��C���N���b�N�؊��������҂�����(4800cycle / (4MHz/2) = 2400us) 1.5�{�_�� */
#define D_PLLCLKWAIT				(3600)			/* PLL/SSCG�N���b�N�؊��������҂�����(4800cycle / (4MHz/2) = 2400us) 1.5�{�_�� */
#define D_GEARWAIT					(1211)			/* �M�A�����҂�����( 807us ) 1.5�{�_�� */
#define D_RTC_PMU_CLKWAIT			(141)			/* RTC/PMU�N���b�N�ؑւ莞��(���C���N���b�N�~��3�T�C�N�� + �T�u�N���b�N�~��3�T�C�N��=93.75��s 1.5�{�_�� */
#define D_PMU_DIV_WAIT				(4)				/* RTC�N���b�N�~��4�T�C�N��+PCLK1�~��4�T�C�N��=1.025��s 1.5�{�_�� */
#define D_SYSCLKWATCH				(1)				/* �V�X�e���N���b�N�Ď�����[us](1us����) */

#endif								/* __MB91570_SYSTEMCLOCK_DRV_INTERNAL__ */

/* DIVide clock configuration Register 1 */
#define SYSCLKDRV_DIVR1_DIVT_1			(0x00u)			/* DIVR1 bit[6-4]:�O���o�X�N���b�N�����ݒ�F�����Ȃ�*/
#define SYSCLKDRV_DIVR1_DIVT_2			(0x01u)			/* DIVR1 bit[6-4]:�O���o�X�N���b�N�����ݒ�F2����	*/
#define SYSCLKDRV_DIVR1_DIVT_3			(0x02u)			/* DIVR1 bit[6-4]:�O���o�X�N���b�N�����ݒ�F3����	*/
#define SYSCLKDRV_DIVR1_DIVT_4			(0x03u)			/* DIVR1 bit[6-4]:�O���o�X�N���b�N�����ݒ�F4����	*/
#define SYSCLKDRV_DIVR1_DIVT_5			(0x04u)			/* DIVR1 bit[6-4]:�O���o�X�N���b�N�����ݒ�F5����	*/
#define SYSCLKDRV_DIVR1_DIVT_6			(0x05u)			/* DIVR1 bit[6-4]:�O���o�X�N���b�N�����ݒ�F6����	*/
#define SYSCLKDRV_DIVR1_DIVT_7			(0x06u)			/* DIVR1 bit[6-4]:�O���o�X�N���b�N�����ݒ�F7����	*/
#define SYSCLKDRV_DIVR1_DIVT_8			(0x07u)			/* DIVR1 bit[6-4]:�O���o�X�N���b�N�����ݒ�F8����	*/

#define SYSCLKDRV_DIVR1_DIVT_MIN		SYSCLKDRV_DIVR1_DIVT_1
#define SYSCLKDRV_DIVR1_DIVT_MAX		SYSCLKDRV_DIVR1_DIVT_8

/* �O�����J�萔 */

/*==============================================================================*/
/*	define��`�i�O�����J�j														*/
/*==============================================================================*/
#define D_SYSCLKDRV_WEIGHT_INIT		 (0x01u)		/*	�N���b�NWeight�W���iMCLK�p�j	*/
#define D_SYSCLKDRV_WEIGHT_PLL		 (0x02u)		/*	�N���b�NWeight�W���iPLLCLK�p�j	*/
#define D_SYSCLKDRV_WEIGHT_SSCG		 (0x01u)		/*	�N���b�NWeight�W���iSSCGCLK�p	*/
#define D_SYSCLKDRV_WEIGHT_SBCLK	 (0x00u)		/*	�N���b�NWeight�W���iSBCLK�͌��ݖ��g�p�B�K�p���ɒl������������	*/
#define D_SYSCLKDRV_ERROR	 		 (0x00u)		/*	�͈͊O		*/

/* �O�����J�}�N�� */
#define EXTBUS_TCLK_LOW		SYSCLKDRV_DIVR1_DIVT_8	/* �ᑬ�ʐM�p�����ݒ�F8����		*/
#define EXTBUS_TCLK_HIGH	SYSCLKDRV_DIVR1_DIVT_2	/* �����ʐM�p�����ݒ�F2����		*/

/* �V�X�e���N���b�N�̒��{�����l�������Ďg�������邱�� */
/* CCLK = 80MHz, 12.5 [ns/Cycle] x 2 [Cycle] = 25ns */
/* CCLK = 72MHz, 13.88..  [ns/Cycle] x 9 [Cycle] = 125ns */
#define D_NOP2()		(void)__wait_nop();(void)__wait_nop()
#define D_NOP3()		(void)__wait_nop();(void)__wait_nop();(void)__wait_nop()

#define D_NOP4()		D_NOP2();D_NOP2()
#define D_NOP5()		D_NOP2();D_NOP3()
#define D_NOP7()		D_NOP3();D_NOP4()
#define D_NOP8()		D_NOP4();D_NOP4()

#define D_NOP9()		D_NOP4();D_NOP5()
#define D_NOP10()		D_NOP5();D_NOP5()
#define D_NOP80()		D_NOP10();D_NOP10();D_NOP10();D_NOP10(); \
						D_NOP10();D_NOP10();D_NOP10();D_NOP10()

#endif	/*	__MB91570_SYSTEMCLOCK_DRV_CONFIG_H__ 	*/

