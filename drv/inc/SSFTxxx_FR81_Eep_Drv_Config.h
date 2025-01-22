/********************************************************************************/
/* �q�於		:	�@��ˑ����W���[��											*/
/* �@�햼		:	-															*/
/* ϲ�ݿ��ðϖ�	:	-															*/
/*==============================================================================*/
/* �쐬̧�ٖ�	:	SSFTxxx_FR81_Eep_Drv_Config.h								*/
/* 				:	EEPROM��ײ�޺�̨��ͯ���										*/
/*==============================================================================*/
/* �Ώ�ϲ��		:	MB91570 Series												*/
/*==============================================================================*/
/* �쐬�ް�ޮ�	:	050101														*/
/* �쐬�N����	:	2015.01.12													*/
/* �쐬��		:	M.Koide														*/
/*------------------------------------------------------------------------------*/
/* �ύX����		:																*/
/* [010103]		:	�V�K�쐬													*/
/* [010202]		:	�E�R�����g�A�E�g�C��										*/
/* [020101]		:	128K-bit�Ή�(Wait���Ԓ���)									*/
/* [020201]		:	#define�ݒ�l�C��											*/
/* [030101]		:	SPI�ʐM(DMA)�Ή�											*/
/* [030201]		:	32bit�Ή�,SDES�G���[�Ή�									*/
/* [040101]		:	SPI�A�N�Z�X�J�nIF�֐��ǉ�									*/
/* [050101]		:	�A�����[�hIF�֐��ύX�ASPI�h���C�oIF�֐����C��				*/
/********************************************************************************/
#ifndef __MB91570_EEP_DRV_CONFIG_H__
#define	__MB91570_EEP_DRV_CONFIG_H__

#ifdef __MB91570_EEP_DRV_INTERNAL__

/*** START_INC ***/
/********************************************************************************/
/*   Include File                                                               */
/*------------------------------------------------------------------------------*/
/*      ͯ�ް̧�ق̲ݸٰ�ޕ��́A���Ļ�قɋL�ڂ��邱��                          */
/********************************************************************************/
#include	"SSFTSTD_FR81_Port_Drv.h"
#include	"SSFTSTD_FR81_SystemClock_Drv.h"
//#include	"SSFTSTD_FR81_Spi_Drv.h"
#include	"SSFTSTD_FR81_Dmac_Drv.h"

/*==============================================================================*/
/*	define��`�i�@��ˑ��j														*/
/*==============================================================================*/
/* �����萔 */
/*==============================================================================*/
/* EEPROM�̃A�N�Z�X�V�[�P���X�ɉ����āA���L�̃E�F�C�g���Ԃ�ύX���邱��			*/
/*==============================================================================*/
#define		EEPROM_WAIT_TCSS()
#define		EEPROM_WAIT_TSKH_CONTINUE()		D_NOP10();D_NOP10();D_NOP10();D_NOP10();D_NOP10();D_NOP2()
#define		EEPROM_WAIT_TSKL_CONTINUE()		D_NOP10();D_NOP10();D_NOP10();D_NOP10();D_NOP10()
#define		EEPROM_WAIT_TSKH_PREREAD()		D_NOP10();D_NOP10();D_NOP10();D_NOP10();D_NOP10();D_NOP2()
#define		EEPROM_WAIT_TSKL_PREREAD()		D_NOP10();D_NOP10();D_NOP10();D_NOP10();D_NOP10()

#define		EEPROM_WAIT_TSKH()				D_NOP10();D_NOP10();D_NOP10();D_NOP10();D_NOP10();D_NOP2()
#define		EEPROM_WAIT_TSKL()				D_NOP10();D_NOP10();D_NOP10();D_NOP10();D_NOP10()

#define		EEPROM_WAIT_TDIS()
#define		EEPROM_WAIT_TDIH()
#define		EEPROM_WAIT_TSV()				D_NOP10();D_NOP10();D_NOP10();D_NOP10();D_NOP10()

#define		EEP_SPI_WAIT_MAXBLOCK			55190U	/* EEPROM-SPI-DMA�ʐM(�A�����[�h�ő�T�C�Y)�^�C���A�E�g����(55.19ms) */
/* �ő�u���b�N�T�C�Y��EEPROM-SPI�ʐM�ʐM���Ԃ��Z�o���鎖 */
/* �Q�l EEPROM-SPI-DMA�ʐM(16K-BYTE)�ʐM���� */
/* 27595[us] = 16K * 8[bit] * (1/4.758[MHz]) */		/* ��SPI�ʐM���g�� 4.758[MHz] */
/* �^�C���A�E�g���Ԃ͒ʐM���Ԃ�2�{�̃}�[�W�� */
/* 55190[us] = 27595[us] * 2 */

#define		EEP_SPI_WAIT_1BYTE				4U		/* EEPROM-SPI�ʐM(8bit)�^�C���A�E�g����(4us) */
/* EEPROM-SPI�ʐM(8bit)�ʐM���� */
/* 1.68[us] =  8[bit] * (1/4.758[MHz]) */			/* ��SPI�ʐM���g�� 4.758[MHz] */
/* �^�C���A�E�g���Ԃ͒ʐM���Ԃ�2�{�̃}�[�W�� */
/* 3.36[us] = 1.68[us] * 2 */

/* ��SPI�ʐM���g�� = ���ӃN���b�N���g�� / 4 */
/* 4.758[MHz] = 19.03325 / 4 */

#define		EEP_SPI_WATCH					1U		/* EEPROM-SPI�ʐM(8bit)�Ď�����[��s](1��s����) */

#endif /* __MB91570_EEP_DRV_INTERNAL__ */

/* �O�����J�萔  */
/*==============================================================================*/
/* EEPROM�ő�A�h���X(word�P��)�̓}�C�R���A���Ӄf�o�C�X�ˑ��̂��ߕύX���Ȃ����� */
/*==============================================================================*/
/* EEPROM�ő�A�h���X(word�P��) */
#define		EEPROM_ADDR_MAX_1K		(0x003FU)	/*	64[word]	*/
#define		EEPROM_ADDR_MAX_2K		(0x007FU)	/* 128[word]	*/
#define		EEPROM_ADDR_MAX_4K		(0x00FFU)	/* 256[word]	*/
#define		EEPROM_ADDR_MAX_8K		(0x01FFU)	/* 512[word]	*/
#define		EEPROM_ADDR_MAX_16K		(0x03FFU)	/* 1024[word]	*/
#define		EEPROM_ADDR_MAX_32K		(0x07FFU)	/* 2048[word]	*/
#define		EEPROM_ADDR_MAX_64K		(0x0FFFU)	/* 4096[word]	*/
#define		EEPROM_ADDR_MAX_128K	(0x1FFFU)	/* 8192[word]	*/

/*==============================================================================*/
/* EEPROM�A�N�Z�X��@															*/
/*==============================================================================*/
#define EEPROM_ACCESS_PORT		0U
#define EEPROM_ACCESS_SPI		1U
#define EEPROM_ACCESS_SPI_FAST	2U

/* EEPROM�̃A�N�Z�X��@�ɉ����ď�L����I�����邱��							*/
/* �A���AEEPROM_ACCESS_SPI,EEPROM_ACCESS_SPI_FAST��EEPROM�ő�A�h���X��32K-BIT�ȏ�̂ݑI���\ */
#define EEPROM_ACCESS_TYPE EEPROM_ACCESS_PORT

/*==============================================================================*/
/* EEPROM���g�p������ɉ����ď�L����I�����邱��								*/
/*==============================================================================*/
#define		EEPROM_ADR_MAX	EEPROM_ADDR_MAX_16K		/* 16K�I�� */


#endif	/* __MB91570_EEP_DRV_CONFIG_H__	*/
