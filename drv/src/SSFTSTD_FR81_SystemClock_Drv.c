/********************************************************************************/
/* �q�於		:	�W�����W���[��												*/
/* �@�햼		:	SSFT														*/
/* ϲ�ݿ��ðϖ�	:	PF															*/
/*==============================================================================*/
/* �쐬̧�ٖ�	:	SSFTSTD_FR81_SystemClock_Drv.c								*/
/* 				:	���Ѹۯ���ײ��Ӽޭ��										*/
/*==============================================================================*/
/* �Ώ�ϲ��		:	MB91570 Series												*/
/*==============================================================================*/
/* �쐬�ް�ޮ�	:	020102														*/
/* �쐬�N����	:	2014.01.23													*/
/* �쐬��		:	K.Uchiyama													*/
/*------------------------------------------------------------------------------*/
/* �ύX����		:																*/
/* [010101]		:	�V�K�쐬													*/
/* [010102]		:	���W�X�^�����ݒ肩��PLL�N���b�N�؂�ւ��𕪗�				*/
/*				:	������A�����Ƒ����^�C�~���O�Őݒ���s���K�v������������	*/
/* [010103]		:	�W��IF�Ή��B�N����ʂ̖��̕ύX								*/
/* [020101]		:	�O�o�X����(TCLK)�ݒ�p�̊O��IF�ǉ�							*/
/* [020102]		:	QAC�Ή��B													*/
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

#define	 __MB91570_SYSTEMCLOCK_DRV_INTERNAL__
#include "SSFTSTD_FR81_SystemClock_Drv.h"

/*==============================================================================*/
/*	typedef��`�i�O������J�j													*/
/*==============================================================================*/
/* �Ȃ� */

/*==============================================================================*/
/*	�֐��������ߐ錾�i�O������J)												*/
/*==============================================================================*/
static void sys_clk_drv_main_to_pll(E_INIT_TYPE req);							/* ���C����PLL �N���b�N�؊������� */
static void sys_clk_drv_pll_to_main(void);										/* PLL�����C�� �N���b�N�؊������� */
static void sys_clk_drv_main_to_sub(void);										/* ���C�����T�u �N���b�N�؊������� */
static void sys_clk_drv_sub_to_main(void);										/* �T�u�����C�� �N���b�N�؊������� */
static void sys_clk_drv_rtc_pmu_set(void);										/* RTC/PMU�N���b�N�ݒ菈�� */

/*==============================================================================*/
/*	�����萔																	*/
/*==============================================================================*/
#define		MASK_CSVCR_MM			(0x40U)										/* CSVCR.MM �I���}�X�N */

/****************************************************************************************/
/*	[���W���[����]	SysClkDrv_Init		[����]	���Ѹۯ���ײ��ڼ޽��������ݒ菈��		*/
/*======================================================================================*/
/*	[�����T�v]	���W�X�^�����ݒ莞�ɖ{�֐����ĂсA���������s���B						*/
/*======================================================================================*/
/*	[�L�q�`��]	void SysClkDrv_Init( E_INIT_TYPE req  )									*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	E_INIT_TYPE				req		�F�N�����								*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void SysClkDrv_Init( E_INIT_TYPE req )
{
	
	switch ( req ) {
	case E_INIT_RESET:															/* CPU���Z�b�g�ɂ��N�� */
	case E_INIT_WAKEUP:															/* �ȓd�͉����iWAKEUP�j�ɂ��N�� */
		sys_clk_drv_main_to_pll(req);											/* Ҳ݁�PLL �ۯ��؊������� */
		break;
	case E_INIT_IGN_ON:															/* IGN ON */
		break;
	case E_INIT_RET_NORMAL_VOL:													/* ��d�����A */
		break;
	case E_INIT_INTERVAL_WAKEUP:												/* �Ԍ��N���E�F�C�N�A�b�v */
		break;
	default:																	/* ��L�ȊO */
		break;
	}
	
}

/****************************************************************************************/
/*	[���W���[����]	SysClkDrv_Sleep	[����]	���Ѹۯ���ײ��ڼ޽���~�ݒ菈��				*/
/*======================================================================================*/
/*	[�����T�v]	�ȓd�̓��[�h�J�ڎ��ɖ{�֐����ĂсA���������s���B						*/
/*======================================================================================*/
/*	[�L�q�`��]	void SysClkDrv_Sleep( void )											*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void SysClkDrv_Sleep( void )
{
	sys_clk_drv_pll_to_main();													/* PLL��Ҳ� �ۯ��؊������� */
}


/****************************************************************************************/
/*	[���W���[����]	SysClkDrv_Refresh	[����]	���Ѹۯ���ײ��ڼ޽��Đݒ�ݒ菈��		*/
/*======================================================================================*/
/*	[�����T�v]	���W�X�^�Đݒ莞�ɖ{�֐����ĂсA���������s���B							*/
/*======================================================================================*/
/*	[�L�q�`��]	void SysClkDrv_Refresh( void )											*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
void SysClkDrv_Refresh( void )
{
	
	/* ��֐� */
	
}


/****************************************************************************************/
/*	[���W���[����]	sys_clk_drv_main_to_pll	[����] Ҳ݁�PLL �ۯ��؊�������				*/
/*======================================================================================*/
/*	[�����T�v]	�\�[�X�N���b�N�����C���N���b�N��PLL/SSCG�N���b�N�ɐ؂芷����B			*/
/*======================================================================================*/
/*	[�L�q�`��]	static void sys_clk_drv_main_to_pll(E_INIT_TYPE req)					*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static void sys_clk_drv_main_to_pll(E_INIT_TYPE req)
{
	UI_16				main_clk_osc_wait;										/* ���C���N���b�N���U����҂����� */
	UI_16				main_clk_osc_tm;										/* ���C���N���b�N���U����҂��J�E���^ */
	UI_16				main_tmp;												/* ���C���N���b�N�ؑւ��҂����ԃJ�E���^ */
	UI_16				pll_tmp;												/* PLL���U����҂��J�E���^ */
	UI_16				clk_tmp;												/* PLL�ؑւ��҂����ԃJ�E���^ */
	UI_16				gear_tmp;												/* �M�A�����҂����ԃJ�E���^ */
	UI_8				flg_exit;												/* ���W�X�^�l�ω��҂��t���O */
	UI_8				csvcr;													/* �N���b�N�X�[�p�o�C�U���䃌�W�X�^ */
	SI_8				main_clk_osc_stat;										/* ���C���N���b�N���U��� */
	
	if (req == E_INIT_WAKEUP) {
		main_clk_osc_wait = D_MAINWAIT_WAKE_UP;									/* Weak Up�N�����́A�d������҂��ݒ背�W�X�^�l�Ɉˑ� */
	} else {
		main_clk_osc_wait = D_MAINWAIT_RESET;									/* ���Z�b�g�N�����́A2~15�~4MHz�̑҂����� */
	}
	
	main_clk_osc_tm = 0;
	main_tmp = 0;
	pll_tmp = 0;
	clk_tmp = 0;
	flg_exit = D_OFF;
	main_clk_osc_stat = D_OK;

	while (flg_exit == D_OFF) {													/* �I���t���O��OFF�̊Ԃ̓��[�v */
		if (CLKG_CMONR_MCRDY == D_ON) {											/* ���C���N���b�N�����U���肵���H*/
			flg_exit = D_ON;													/* ���[�v�I�� */
			main_clk_osc_stat = D_OK;											/* ���C���N���b�N���U���:OK */
		} else if(main_clk_osc_tm >= main_clk_osc_wait) {						/* ���U����҂����ԁA�o�߁H */
			flg_exit = D_ON;													/* ���[�v�I�� */
			main_clk_osc_stat = D_NG;											/* ���C���N���b�N���U���:NG */
		} else {
			;
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* �ҋ@���� */
		main_clk_osc_tm++;
	}
	
	CLKG_CSELR_CKS = INIT_CSELR_CKS_SELECT_MAIN_00;								/* ���C���N���b�N(00b)�ɐݒ� */
	
	flg_exit = D_OFF;
	
	while (flg_exit == D_OFF) {													/* �I���t���O��OFF�̊Ԃ̓��[�v */
		if ( (CLKG_CMONR_CKM == INIT_CSELR_CKS_SELECT_MAIN_00)					/* �\�[�X�N���b�N�����C���N���b�N�ɐ؂�ւ�����H */
		|| (main_tmp >= D_MAINCLKWAIT) ) {										/* �܂��́A�\�[�X�N���b�N�̐؂�ւ莞�ԑҋ@����? */
			flg_exit = D_ON;													/* ���[�v�I�� */
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* �ҋ@���� */
		main_tmp++;
	}
	CLKG_CSTBR = INIT_CSTBR;													/* ���U����҂��ݒ背�W�X�^ */
	CLKG_PLLCR_POSW = INIT_PLLCR_POSW;											/* PLL/SSCG�N���b�N����҂����Ԃ�ݒ� */

	CCTL_CCPSSELR_PCSEL = INIT_CCPSSELR_PCSEL;									/* PLL/SSCG�N���b�N�\�[�X�I�� */
	
	/* PLL�̒��{����ݒ�(CAN�AOCD�p) */
	CLKG_PLLCR_PDS = INIT_PLLCR_PDS;											/* PLL���̓N���b�N�����I�� */
	CCTL_CCPLLFBR_IDIV = INIT_CCPLLFBR_IDIV;									/* PLL�}�N��FB���͕�����ݒ� */
	CCTL_CCPSDIVR_PODS = INIT_CCPSDIVR_PODS;									/* PLL�}�N�����U�N���b�N������I�� */

	/* SSCG�̒��{����ݒ� */
	CCTL_CCSSFBR0_NDIV = INIT_CCSSFBR0_NDIV;									/* SSCG�}�N��FB����N������ݒ� */
	CCTL_CCSSFBR1_PDIV = INIT_CCSSFBR1_PDIV;									/* SSCG�}�N��FB����P������ݒ� */
	CCTL_CCPSDIVR_SODS = INIT_CCPSDIVR_SODS;									/* SSCG�}�N�����U�N���b�N������I�� */
	
	/* SSCG�̊g�U���@��ݒ� */
	CCTL_CCSSCCR0_SFREQ = INIT_CCSSCCR0_SFREQ;									/* ���W�����[�V�������g�� */
	CCTL_CCSSCCR0_SMODE = INIT_CCSSCCR0_SMODE;									/* �g�U���W�����[�V�������[�h�I�� */
	CCTL_CCSSCCR0_SSEN = INIT_CCSSCCR0_SSEN;									/* �g�U�X�y�N�g������ */
	CCTL_CCSSCCR1_RATESEL = INIT_CCSSCCR1_RATESEL;								/* �g�U���W�����[�V�������[�g�I�� */
	
	/* �M�A��L����Ԃɐݒ� */
	CCTL_CCCGRCR0_GREN = INIT_CCCGRCR0_GREN;									/* �N���b�N�M�A�C�l�[�u�� */
	
	/* �M�A�X�e�b�v�̐ݒ� */
	CCTL_CCCGRCR1_GRSTP = INIT_CCCGRCR1_GRSTP;									/* �N���b�N�M�A�X�e�b�v�I�� */
	CCTL_CCCGRCR1_GRSTN = INIT_CCCGRCR1_GRSTN;									/* �N���b�N�M�A�J�n�X�e�b�v�I�� */
	CCTL_CCCGRCR2_GRLP = INIT_CCCGRCR2_GRLP;									/* �N���b�N�M�A�J�Ԃ��� */
	
	csvcr = CSVCR;																/* �N���b�N�X�[�p�o�C�U���䃌�W�X�^�l */
	
	if ( (main_clk_osc_stat == D_OK) &&											/* ���C���N���b�N�̔��U������m�F�ł����H */
	     ((csvcr & MASK_CSVCR_MM) == 0) ) {										/* ���A�N���b�N�X�[�p�o�C�U��MM bit�����C���N���b�N���쒆�H */
		CLKG_CSELR_PCEN = INIT_CSELR_PCEN;										/* PLL���U������ */	
	
		flg_exit = D_OFF;
	
		while (flg_exit == D_OFF) {												/* �I���t���O��OFF�̊Ԃ̓��[�v */
			if ( (CLKG_CMONR_PCRDY == D_ON) 									/* PLL���U�����肵���H */
			  || (pll_tmp >= D_PLLWAIT) ) {										/* �܂��͔��U����҂����ԑҋ@�����H */
				flg_exit = D_ON;												/* ���[�v�I�� */
			}
			SysClkDrv_UsecWait(D_SYSCLKWATCH);									/* �ҋ@���� */
			pll_tmp++;
		}
	
		/* �e��N���b�N(CPU/����)�̕�����ݒ� */
		DIVR0_DIVB = INIT_DIVR0_DIVB;											/* �\�[�X�N���b�N�𕪎����x�[�X�N���b�N�ACPU�N���b�N(CCLK)�A�I���`�b�v�o�X�N���b�N(HCLK) */
		DIVR1      = INIT_DIVR1;												/* �x�[�X�N���b�N�𕪎����O���o�X�N���b�N(TCLK) */
		DIVR2_DIVP = INIT_DIVR2_DIVP;											/* �x�[�X�N���b�N�𕪎������ӃN���b�N(PCLK1) */
	
		/* �񓯊����ӃN���b�N�̕�����ݒ� */
		PICD_PDIV = INIT_PICD_PDIV;												/* PLL�N���b�N�𕪎������ӃN���b�N(PCLK2) */
		/* CPU/���ӃN���b�N�̊֌W��񓯊��ɐݒ� */
		SACR_M = INIT_SACR_M;													/* ����/�񓯊��ݒ� */
	
		/* PLL/SSCG�N���b�N�ւ̐ؑւ� */
		CLKG_CSELR_CKS = INIT_CSELR_CKS_SELECT_PLL_SSCG;						/* �\�[�X�N���b�N�I�� */
	
		flg_exit = D_OFF;
	
		while (flg_exit == D_OFF) {												/* �I���t���O��OFF�̊Ԃ̓��[�v */
			if ( (CLKG_CMONR_CKM == INIT_CSELR_CKS_SELECT_PLL_SSCG) 			/* �\�[�X�N���b�N���؂�ւ����H */
			  || (clk_tmp >= D_PLLCLKWAIT) ) {									/* �܂��͐؂�ւ莞�ԑҋ@�����H */
				flg_exit = D_ON;												/* ���[�v�I�� */
			}
			SysClkDrv_UsecWait(D_SYSCLKWATCH);									/* �ҋ@���� */
			clk_tmp++;
		}
	
		/* GEAR UP ���쒆 */
		if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_GEAR_UP ) {
			/* �M�A�������m�F */
			flg_exit = D_OFF;
			gear_tmp = 0;
			while (flg_exit == D_OFF) {											/* �I���t���O��OFF�̊Ԃ̓��[�v */
				if ( (CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_HIGH)			/* �N���b�N�M�A��������~�� */
				  || (gear_tmp >= D_GEARWAIT) ) {								/* �܂��́A�M�A�������Ԋ��� */
					flg_exit = D_ON;											/* ���[�v�I�� */
				}
				SysClkDrv_UsecWait(D_SYSCLKWATCH);								/* �ҋ@���� */
				gear_tmp++;
			}
		}
		/* �N���b�N�������Œ�~ */
		if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_HIGH ) {
			/* �M�A�J�n */
			CCTL_CCCGRCR0_GRSTR = INIT_CCCGRCR0_GRSTR;							/* �N���b�N�M�A�X�^�[�g */
		}
	
		/* GEAR DOWN ���쒆 */
		if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_GEAR_DOWN ) {
			/* �M�A�������m�F */
			flg_exit = D_OFF;
			gear_tmp = 0;
			while (flg_exit == D_OFF) {											/* �I���t���O��OFF�̊Ԃ̓��[�v */
				if ( (CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_LOW)			/* �N���b�N�M�A���ᑬ��~�� */
				  || (gear_tmp >= D_GEARWAIT) ) {								/* �܂��́A�M�A�������Ԋ��� */
					flg_exit = D_ON;											/* ���[�v�I�� */
				}
				SysClkDrv_UsecWait(D_SYSCLKWATCH);								/* �ҋ@���� */
				gear_tmp++;
			}
		}
		/* �N���b�N���ᑬ�Œ�~ */
		if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_LOW ) {
			/* �M�A�J�n */
			CCTL_CCCGRCR0_GRSTR = INIT_CCCGRCR0_GRSTR;							/* �N���b�N�M�A�X�^�[�g */
		}
	
		/* �M�A�������m�F */
		flg_exit = D_OFF;
		gear_tmp = 0;
		while (flg_exit == D_OFF) {												/* �I���t���O��OFF�̊Ԃ̓��[�v */
			if ( (CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_HIGH)				/* �N���b�N�M�A��������~�� */
			  || (gear_tmp >= D_GEARWAIT) ) {									/* �܂��́A�M�A�������Ԋ��� */
				flg_exit = D_ON;												/* ���[�v�I�� */
			}
			SysClkDrv_UsecWait(D_SYSCLKWATCH);									/* �ҋ@���� */
			gear_tmp++;
		}

		sys_clk_drv_rtc_pmu_set();												/* RTC/PMU�N���b�N�ݒ菈�� */
	}
}


/****************************************************************************************/
/*	[���W���[����]	sys_clk_drv_pll_to_main	[����] PLL��Ҳ� �ۯ��؊�������				*/
/*======================================================================================*/
/*	[�����T�v]	�\�[�X�N���b�N��PLL/SSCG�N���b�N�����C���N���b�N�ɐ؂芷����B			*/
/*======================================================================================*/
/*	[�L�q�`��]	static void sys_clk_drv_pll_to_main(void)								*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static void sys_clk_drv_pll_to_main(void)
{
	UI_16				clk_tmp;												/* �N���b�N�؊����҂����ԃJ�E���^ */
	UI_16				gear_tmp;												/* �M�A�����҂����ԃJ�E���^ */
	UI_8				flg_exit;												/* ���W�X�^�l�ω��҂��t���O */
	
	clk_tmp = 0;
	flg_exit = D_OFF;
	
	CLKG_CSTBR = STOP_CSTBR;													/* ���U����҂��ݒ背�W�X�^ */
	
	/* PLL/SSCG�N���b�N�E���[�h���m�F */
	while (flg_exit == D_OFF) {
		if ( (CLKG_CMONR_CKM == CMONR_CKM_PLL_SSCG)
		  || (clk_tmp >= D_PLLCLKWAIT) ) {
			flg_exit = D_ON;
		}
		clk_tmp++;
		SysClkDrv_UsecWait(D_SYSCLKWATCH);
	}
	
	/* GEAR DOWN ���쒆 */
	if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_GEAR_DOWN ) {
		/* �M�A�������m�F */
		flg_exit = D_OFF;
		gear_tmp = 0;
		while (flg_exit == D_OFF) {												/* �I���t���O��OFF�̊Ԃ̓��[�v */
			if ( (CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_LOW)				/* �N���b�N�M�A���ᑬ��~�� */
			  || (gear_tmp >= D_GEARWAIT) ) {									/* �܂��́A�M�A�������Ԋ��� */
				flg_exit = D_ON;												/* ���[�v�I�� */
			}
			SysClkDrv_UsecWait(D_SYSCLKWATCH);									/* �ҋ@���� */
			gear_tmp++;
		}
	}
	/* �N���b�N���ᑬ�Œ�~ */
	if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_LOW ) {
		/* �M�A�J�n */
		CCTL_CCCGRCR0_GRSTR = INIT_CCCGRCR0_GRSTR;								/* �N���b�N�M�A�X�^�[�g */
	}
	/* GEAR UP ���쒆 */
	if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_GEAR_UP ) {
		/* �M�A�������m�F */
		flg_exit = D_OFF;
		gear_tmp = 0;
		while (flg_exit == D_OFF) {												/* �I���t���O��OFF�̊Ԃ̓��[�v */
			if ( (CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_HIGH)				/* �N���b�N�M�A��������~�� */
			  || (gear_tmp >= D_GEARWAIT) ) {									/* �܂��́A�M�A�������Ԋ��� */
				flg_exit = D_ON;												/* ���[�v�I�� */
			}
			SysClkDrv_UsecWait(D_SYSCLKWATCH);									/* �ҋ@���� */
			gear_tmp++;
		}
	}
	/* �N���b�N�������Œ�~ */
	if ( CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_HIGH ) {
		/* �M�A�J�n */
		CCTL_CCCGRCR0_GRSTR = INIT_CCCGRCR0_GRSTR;								/* �N���b�N�M�A�X�^�[�g */
	}
	
	/* �M�A�������m�F */
	flg_exit = D_OFF;
	gear_tmp = 0;
	while (flg_exit == D_OFF) {													/* �I���t���O��OFF�̊Ԃ̓��[�v */
		if ( (CCTL_CCCGRCR0_GRSTS == CCCGRCR0_GRSTS_STOP_LOW)					/* �N���b�N�M�A���ᑬ��~�� */
		  || (gear_tmp >= D_GEARWAIT) ) {										/* �܂��́A�M�A�������Ԋ��� */
			flg_exit = D_ON;													/* ���[�v�I�� */
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* �ҋ@���� */
		gear_tmp++;
	}

	/* ���C���N���b�N�ւ̐ؑւ� */
	CLKG_CSELR_CKS = STOP_CSELR_CKS;											/* �\�[�X�N���b�N�I�� */
	
	clk_tmp = 0;
	flg_exit = D_OFF;
	/* ���C���N���b�N�ɐؑւ���Ă��邱�Ƃ��m�F */
	while (flg_exit == D_OFF) {
		if ( (CLKG_CMONR_CKM == CMONR_CKM_MCLK_DIV2)
		  || (clk_tmp >= D_PLL_MAINCLKWAIT) ) {
			flg_exit = D_ON;
		}
		clk_tmp++;
		SysClkDrv_UsecWait(D_SYSCLKWATCH);
	}
	
	/* PLL/SSCG�̓�����~ */
	CLKG_CSELR_PCEN = STOP_CSELR_PCEN;											/* PLL���U */
	/* �e��N���b�N(CPU/����)�̕�����ݒ� */
	DIVR0_DIVB = STOP_DIVR0_DIVB;												/* �\�[�X�N���b�N�𕪎����x�[�X�N���b�N�ACPU�N���b�N(CCLK)�A�I���`�b�v�o�X�N���b�N(HCLK) */
	DIVR1      = STOP_DIVR1;													/* �x�[�X�N���b�N�𕪎����O���o�X�N���b�N(TCLK) */
	DIVR2_DIVP = STOP_DIVR2_DIVP;												/* �x�[�X�N���b�N�𕪎������ӃN���b�N(PCLK1) */
	/* CPU/���ӃN���b�N�̊֌W��񓯊��ɐݒ� */
	SACR_M = STOP_SACR_M;														/* ����/�񓯊��ݒ� */
}

/****************************************************************************************/
/*	[���W���[����]	sys_clk_drv_main_to_sub	[����] Ҳ݁���� �ۯ��؊�������				*/
/*======================================================================================*/
/*	[�����T�v]	�\�[�X�N���b�N�����C���N���b�N���T�u�N���b�N�ɐ؂芷����B				*/
/*======================================================================================*/
/*	[�L�q�`��]	static void sys_clk_drv_main_to_sub(void)								*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static void sys_clk_drv_main_to_sub(void)
{
	
	/* ��֐� */
	
}

/****************************************************************************************/
/*	[���W���[����]	sys_clk_drv_sub_to_main	[����] ��ށ�Ҳ� �ۯ��؊�������				*/
/*======================================================================================*/
/*	[�����T�v]	�\�[�X�N���b�N���T�u�N���b�N�����C���N���b�N�ɐ؂芷����B				*/
/*======================================================================================*/
/*	[�L�q�`��]	static void sys_clk_drv_sub_to_main(void)								*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static void sys_clk_drv_sub_to_main(void)
{
	
	/* ��֐� */
	
}

/****************************************************************************************/
/*	[���W���[����]	sys_clk_drv_rtc_pmu_set	[����] RTC/PMU�N���b�N�ݒ菈��				*/
/*======================================================================================*/
/*	[�����T�v]	RTC/PMU�N���b�N�̃\�[�X�N���b�N�ƕ������ݒ肷��B						*/
/*======================================================================================*/
/*	[�L�q�`��]	static void sys_clk_drv_rtc_pmu_set(void)								*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	����																	*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	����																	*/
/****************************************************************************************/
static void sys_clk_drv_rtc_pmu_set(void)
{
	UI_16				src_clk_change_time;
	UI_16				f_div_refrect_time;
	UI_16				g_div_refrect_time;
	UI_8				flg_exit;
	
	src_clk_change_time = 0;
	f_div_refrect_time = 0;
	g_div_refrect_time = 0;
	flg_exit = D_OFF;
	
	/* RTC/PMU�N���b�N�I�����W�X�^ CST=0�łȂ��ƁA�������ł��Ȃ��̂ŁA�Ď� */
	while(flg_exit == D_OFF) {
		if ( (CCTL_CCRTSELR_CST == 0)
		  || (src_clk_change_time >= D_RTC_PMU_CLKWAIT) ) {
			flg_exit = D_ON;
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* �ҋ@���� */
		src_clk_change_time++;
	}
	
	CCTL_CCRTSELR_CSC = INIT_CCRTSELR_CSC;
	
	/* �\�[�X�N���b�N�̐ؑւ�҂� */
	src_clk_change_time = 0;
	flg_exit = D_OFF;
	while(flg_exit == D_OFF) {
		if ( (CCTL_CCRTSELR_CST == 0)
		  || (src_clk_change_time >= D_RTC_PMU_CLKWAIT) ) {
			flg_exit = D_ON;
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* �ҋ@���� */
		src_clk_change_time++;
	}
	
	flg_exit = D_OFF;
	while(flg_exit == D_OFF) {
		if ( (CCTL_CCPMUCR0_FST == 0)
		  || (f_div_refrect_time >= D_PMU_DIV_WAIT) ) {
			flg_exit = D_ON;
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* �ҋ@���� */
		f_div_refrect_time++;
	}
	
	CCTL_CCPMUCR0_FDIV = INIT_CCPMUCR0_FDIV;
	
	f_div_refrect_time = 0;
	flg_exit = D_OFF;
	while(flg_exit == D_OFF) {
		if ( (CCTL_CCPMUCR0_FST == 0)
		  || (f_div_refrect_time >= D_PMU_DIV_WAIT) ) {
			flg_exit = D_ON;
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* �ҋ@���� */
		f_div_refrect_time++;
	}
	
	flg_exit = D_OFF;
	while(flg_exit == D_OFF) {
		if ( (CCTL_CCPMUCR1_GST == 0)
		  || (g_div_refrect_time >= D_PMU_DIV_WAIT) ) {
			flg_exit = D_ON;
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* �ҋ@���� */
		g_div_refrect_time++;
	}
	
	CCTL_CCPMUCR1_GDIV = INIT_CCPMUCR1_GDIV;
	
	g_div_refrect_time = 0;
	flg_exit = D_OFF;
	while(flg_exit == D_OFF) {
		if ( (CCTL_CCPMUCR1_GST == 0)
		  || (g_div_refrect_time >= D_PMU_DIV_WAIT) ) {
			flg_exit = D_ON;
		}
		SysClkDrv_UsecWait(D_SYSCLKWATCH);										/* �ҋ@���� */
		g_div_refrect_time++;
	}
}

/****************************************************************************************/
/*	[���W���[����]	SysClkDrv_UsecWait	[����] �E�F�C�g����								*/
/*======================================================================================*/
/*	[�����T�v]	�w�肳�ꂽ�ݒ�l�ɑΉ��������ԕ��E�F�C�g���s�Ȃ��B						*/
/*======================================================================================*/
/*	[�L�q�`��]	void SysClkDrv_UsecWait(void)											*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	wait_time:	wait����(����=1�̂Ƃ� 80MHz��1[us])							*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	������clock_weight�ݒ�l�ɂ��Ă̓��W���[���d�l���Q�Ƃ̂���			*/
/****************************************************************************************/
void SysClkDrv_UsecWait(UI_16 wait_time)
{
	volatile UI_16	wk1;			/* ���[�v1�p										*/
	volatile UI_16	wk2;			/* ���[�v2�p					    				*/
	volatile UI_16	clock_weight;	/* �V�X�e���N���b�N�ʗp�ɏd�ݕt�� 					*/

	switch( CLKG_CMONR_CKM ) {		/* Clock Monitor Register (�\�[�X�N���b�N�\���j		*/
		case E_SYSCLK_INIT:			/* Initial Value�i���C���N���b�N��2�����j 		 	*/
		case E_SYSCLK_MCLK_DV2:		/* MCLK devided by 2�i���C���N���b�N��2�����j		*/
			clock_weight = D_SYSCLKDRV_WEIGHT_INIT;				/* 4MHz/2 �����̏d�ݕt��*/
			break;

		case E_SYSCLKDRV_PS_SEL:	/* PLL/SSCG CLK.�iPLL/SSCG�N���b�N�j				*/
			if ( CCTL_CCPSSELR_PCSEL == D_ON ) {	/* PLL/SSCG�N���b�N SSCG�L�F1		*/
				clock_weight = D_SYSCLKDRV_WEIGHT_SSCG;			/*  SSCG�����̏d�ݕt�� 	*/
			} else {
				clock_weight = D_SYSCLKDRV_WEIGHT_PLL;			/*  PLL�����̏d�ݕt��	*/
			}
			break;

		case E_SYSCLKDRV_SBCLK:		/* SBCLK.�i�T�u�N���b�N�j							*/
			clock_weight = D_SYSCLKDRV_WEIGHT_SBCLK;			/*  SBCLK�����̏d�ݕt�� */
			break;

		default:					/* �͈͊O 											*/
			clock_weight = D_SYSCLKDRV_ERROR;					/*  �͈͊O�����̏d�ݕt��*/
			break;
	}

	for ( wk1 = 1 ; wk1 < wait_time ; wk1++ ) { 
		for ( wk2 = 1 ; wk2 < clock_weight; wk2++ ) {
			(void) __wait_nop();						/* NOP		 1[cycle] 			*/
		}
	}
	return;
}

/****************************************************************************************/
/*	[���W���[����]	SysClkDrv_SetTCLK	[����] Ext-Bus Clock (TCLK)�����ݒ菈��			*/
/*======================================================================================*/
/*	[�����T�v]	�w�肳�ꂽ�ݒ�l�ɑΉ�����TCLK�����ݒ���s�Ȃ��B						*/
/*======================================================================================*/
/*	[�L�q�`��]	void SysClkDrv_SetTCLK(UI_8 select_clk)									*/
/*--------------------------------------------------------------------------------------*/
/*	[ ��  �� ]	select_clk:	�O���o�X�N���b�N�����l										*/
/*--------------------------------------------------------------------------------------*/
/*	[ �߂�l ]	����																	*/
/*======================================================================================*/
/*	[ ��  �l ]	�{API�͊O���o�X�ʐM��~��(CS��������)�Ɏg�p���Ă��������B				*/
/****************************************************************************************/
void SysClkDrv_SetTCLK(UI_8 select_clk)
{
	if (select_clk <= SYSCLKDRV_DIVR1_DIVT_MAX) {
		DIVR1_DIVT = select_clk;
	}
}
