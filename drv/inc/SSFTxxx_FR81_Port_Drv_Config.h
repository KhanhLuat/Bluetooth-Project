/************************************************************************************************/
/* �q�於		:	�@��ˑ����W���[��															*/
/* �@�햼		:	-																			*/
/* ϲ�ݿ��ðϖ�	:	-																			*/
/*==============================================================================================*/
/* �쐬̧�ٖ�	:	SSFTxxx_FR81_Port_Drv_Config.h												*/
/* 				:	GPIO��ײ��ͯ��																*/
/*==============================================================================================*/
/* �Ώ�ϲ��		:	MB91570 Series																*/
/*==============================================================================================*/
/* �쐬�ް�ޮ�	:	030302																		*/
/* �쐬�N����	:	2014.06.10																	*/
/* �쐬��		:	K.Uchiyama																	*/
/*----------------------------------------------------------------------------------------------*/
/* �ύX����		:																				*/
/* [010103]		:	�V�K�쐬																	*/
/* [010104]		:	�r�b�g�}�X�N�ʒu�̒�`�ӏ��ύX�B�Z�L�����e�B�C���W�P�[�^�̃|�[�g�C���B		*/
/* [010105]		:	EEPROM�A�V�t�g���W�X�^�AHWD�ALIN�̃|�[�g��`�ǉ�							*/
/* [030101]		:	ROM�e�ʂƏ������ԍ팸�̂��߁A�R���p�C��SW�ǉ�								*/
/* [030301]		:	���W�X�^�̍Ē�`���̕ύX													*/
/* [030302]		:	�}�C�R��SLEEP���Ƀ|�[�g�����͐ݒ�(H.I)�ɂȂ��Ă���							*/
/************************************************************************************************/
#ifndef __MB91570_PORT_DRV_CONFIG_H__
#define __MB91570_PORT_DRV_CONFIG_H__

/* �����萔 */
#ifdef __MB91570_PORT_DRV_INTERNAL__				
/*** START_INC ***/
/********************************************************************************/
/*   Include File                                                               */
/*------------------------------------------------------------------------------*/
/*      ͯ�ް̧�ق̲ݸٰ�ޕ��́A���Ļ�قɋL�ڂ��邱��                          */
/********************************************************************************/
#include "SSFTSTD_FR81_Interrupt_Drv.h"

/*==============================================================================*/
/*	define��`�i�@��ˑ��j														*/
/*==============================================================================*/
/* �|�[�g�f�[�^���W�X�^ */
#define INIT_PDR0	(0x00)		/* P0�̃|�[�g�f�[�^���W�X�^�̏����l */
#define INIT_PDR1	(0x00)		/* P1�̃|�[�g�f�[�^���W�X�^�̏����l */
#define INIT_PDR2	(0x20)		/* P2�̃|�[�g�f�[�^���W�X�^�̏����l */
#define INIT_PDR3	(0x00)		/* P3�̃|�[�g�f�[�^���W�X�^�̏����l */
#define INIT_PDR4	(0x00)		/* P4�̃|�[�g�f�[�^���W�X�^�̏����l */
#define INIT_PDR5	(0x00)		/* P5�̃|�[�g�f�[�^���W�X�^�̏����l */
#define INIT_PDR6	(0x00)		/* P6�̃|�[�g�f�[�^���W�X�^�̏����l */
#define INIT_PDR7	(0x00)		/* P7�̃|�[�g�f�[�^���W�X�^�̏����l */
#define INIT_PDR8	(0x00)		/* P8�̃|�[�g�f�[�^���W�X�^�̏����l */
#define INIT_PDR9	(0x01)		/* P9�̃|�[�g�f�[�^���W�X�^�̏����l */
#define INIT_PDR10	(0x00)		/* P10�̃|�[�g�f�[�^���W�X�^�̏����l */
#define INIT_PDR11	(0x04)		/* P11�̃|�[�g�f�[�^���W�X�^�̏����l */
#define INIT_PDR12	(0x10)		/* P12�̃|�[�g�f�[�^���W�X�^�̏����l */
#define INIT_PDR13	(0x00)		/* P13�̃|�[�g�f�[�^���W�X�^�̏����l */

#define STOP_PDR0	(0x00)		/* P0�̃|�[�g�f�[�^���W�X�^�̒�~�l */
#define STOP_PDR1	(0x00)		/* P1�̃|�[�g�f�[�^���W�X�^�̒�~�l */
#define STOP_PDR2	(0x20)		/* P2�̃|�[�g�f�[�^���W�X�^�̒�~�l */
#define STOP_PDR3	(0x00)		/* P3�̃|�[�g�f�[�^���W�X�^�̒�~�l */
#define STOP_PDR4	(0x00)		/* P4�̃|�[�g�f�[�^���W�X�^�̒�~�l */
#define STOP_PDR5	(0x00)		/* P5�̃|�[�g�f�[�^���W�X�^�̒�~�l */
#define STOP_PDR6	(0x00)		/* P6�̃|�[�g�f�[�^���W�X�^�̒�~�l */
#define STOP_PDR7	(0x00)		/* P7�̃|�[�g�f�[�^���W�X�^�̒�~�l */
#define STOP_PDR8	(0x00)		/* P8�̃|�[�g�f�[�^���W�X�^�̒�~�l */
#define STOP_PDR9	(0x00)		/* P9�̃|�[�g�f�[�^���W�X�^�̒�~�l */
								/* AD5V EN��Framework�Œ�~����BGPIO�͒l��ێ�(P090) */
#define STOP_PDR10	(0x00)		/* P10�̃|�[�g�f�[�^���W�X�^�̒�~�l */
#define STOP_PDR11	(0x04)		/* P11�̃|�[�g�f�[�^���W�X�^�̒�~�l */
#define STOP_PDR12	(0x70)		/* P12�̃|�[�g�f�[�^���W�X�^�̒�~�l */
								/* ���ȕێ��A3.3V EN�AAD5V EN��Framework�Œ�~����BGPIO�͒l��ێ�(P125�AP126) */
#define STOP_PDR13	(0x00)		/* P13�̃|�[�g�f�[�^���W�X�^�̒�~�l */

/* �f�[�^�������W�X�^ */
#define INIT_DDR0	(0xFF)		/* P0�̃f�[�^�������W�X�^�̏����l */
#define INIT_DDR1	(0xA7)		/* P1�̃f�[�^�������W�X�^�̏����l */
#define INIT_DDR2	(0xA6)		/* P2�̃f�[�^�������W�X�^�̏����l */
#define INIT_DDR3	(0x65)		/* P3�̃f�[�^�������W�X�^�̏����l */
#define INIT_DDR4	(0xFB)		/* P4�̃f�[�^�������W�X�^�̏����l */
#define INIT_DDR5	(0xFF)		/* P5�̃f�[�^�������W�X�^�̏����l */
#define INIT_DDR6	(0xFF)		/* P6�̃f�[�^�������W�X�^�̏����l */
#define INIT_DDR7	(0xFF)		/* P7�̃f�[�^�������W�X�^�̏����l */
#define INIT_DDR8	(0xFF)		/* P8�̃f�[�^�������W�X�^�̏����l */
#define INIT_DDR9	(0xA7)		/* P9�̃f�[�^�������W�X�^�̏����l */
#define INIT_DDR10	(0xEE)		/* P10�̃f�[�^�������W�X�^�̏����l */
#define INIT_DDR11	(0xF5)		/* P11�̃f�[�^�������W�X�^�̏����l */
#define INIT_DDR12	(0xFF)		/* P12�̃f�[�^�������W�X�^�̏����l */
#define INIT_DDR13	(0xD7)		/* P13�̃f�[�^�������W�X�^�̏����l */

#define STOP_DDR0	(0xFF)		/* P0�̃f�[�^�������W�X�^�̒�~�l */
#define STOP_DDR1	(0xA7)		/* P1�̃f�[�^�������W�X�^�̒�~�l */
#define STOP_DDR2	(0xA6)		/* P2�̃f�[�^�������W�X�^�̒�~�l */
#define STOP_DDR3	(0x65)		/* P3�̃f�[�^�������W�X�^�̒�~�l */
#define STOP_DDR4	(0xFB)		/* P4�̃f�[�^�������W�X�^�̒�~�l */
#define STOP_DDR5	(0xFF)		/* P5�̃f�[�^�������W�X�^�̒�~�l */
#define STOP_DDR6	(0xFF)		/* P6�̃f�[�^�������W�X�^�̒�~�l */
#define STOP_DDR7	(0xFF)		/* P7�̃f�[�^�������W�X�^�̒�~�l */
#define STOP_DDR8	(0xFF)		/* P8�̃f�[�^�������W�X�^�̒�~�l */
#define STOP_DDR9	(0xA7)		/* P9�̃f�[�^�������W�X�^�̒�~�l */
#define STOP_DDR10	(0xEE)		/* P10�̃f�[�^�������W�X�^�̒�~�l */
#define STOP_DDR11	(0xF5)		/* P11�̃f�[�^�������W�X�^�̒�~�l */
#define STOP_DDR12	(0xFF)		/* P12�̃f�[�^�������W�X�^�̒�~�l */
#define STOP_DDR13	(0xD7)		/* P13�̃f�[�^�������W�X�^�̒�~�l */

#define REFRESH_DDR0	(0xFF)		/* P0�̃f�[�^�������W�X�^�̍Đݒ�l */
#define REFRESH_DDR1	(0xA7)		/* P1�̃f�[�^�������W�X�^�̍Đݒ�l */
#define REFRESH_DDR2	(0xA7)		/* P2�̃f�[�^�������W�X�^�̍Đݒ�l */
#define REFRESH_DDR3	(0x65)		/* P3�̃f�[�^�������W�X�^�̍Đݒ�l */
#define REFRESH_DDR4	(0xFB)		/* P4�̃f�[�^�������W�X�^�̍Đݒ�l */
#define REFRESH_DDR5	(0xFF)		/* P5�̃f�[�^�������W�X�^�̍Đݒ�l */
#define REFRESH_DDR6	(0xFF)		/* P6�̃f�[�^�������W�X�^�̍Đݒ�l */
#define REFRESH_DDR7	(0xFF)		/* P7�̃f�[�^�������W�X�^�̍Đݒ�l */
#define REFRESH_DDR8	(0xFF)		/* P8�̃f�[�^�������W�X�^�̍Đݒ�l */
#define REFRESH_DDR9	(0xA7)		/* P9�̃f�[�^�������W�X�^�̍Đݒ�l */
#define REFRESH_DDR10	(0xEE)		/* P10�̃f�[�^�������W�X�^�̍Đݒ�l */
#define REFRESH_DDR11	(0xF5)		/* P11�̃f�[�^�������W�X�^�̍Đݒ�l */
#define REFRESH_DDR12	(0xFF)		/* P12�̃f�[�^�������W�X�^�̍Đݒ�l */
#define REFRESH_DDR13	(0xD7)		/* P13�̃f�[�^�������W�X�^�̍Đݒ�l */

/* �|�[�g�@�\���W�X�^ */
#define INIT_PFR0	(0x07)		/* P0�̃|�[�g�@�\���W�X�^�̏����l */
#define INIT_PFR1	(0x00)		/* P1�̃|�[�g�@�\���W�X�^�̏����l */
#define INIT_PFR2	(0x80)		/* P2�̃|�[�g�@�\���W�X�^�̏����l */
#define INIT_PFR3	(0x64)		/* P3�̃|�[�g�@�\���W�X�^�̏����l */
#define INIT_PFR4	(0x01)		/* P4�̃|�[�g�@�\���W�X�^�̏����l */
#define INIT_PFR5	(0x00)		/* P5�̃|�[�g�@�\���W�X�^�̏����l */
#define INIT_PFR6	(0x00)		/* P6�̃|�[�g�@�\���W�X�^�̏����l */
#define INIT_PFR7	(0x00)		/* P7�̃|�[�g�@�\���W�X�^�̏����l */
#define INIT_PFR8	(0x00)		/* P8�̃|�[�g�@�\���W�X�^�̏����l */
#define INIT_PFR9	(0x20)		/* P9�̃|�[�g�@�\���W�X�^�̏����l */
#define INIT_PFR10	(0x00)		/* P10�̃|�[�g�@�\���W�X�^�̏����l */
#define INIT_PFR11	(0x01)		/* P11�̃|�[�g�@�\���W�X�^�̏����l */
#define INIT_PFR12	(0x00)		/* P12�̃|�[�g�@�\���W�X�^�̏����l */
#define INIT_PFR13	(0x00)		/* P13�̃|�[�g�@�\���W�X�^�̏����l */

#define STOP_PFR0	(0x07)		/* P0�̃|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_PFR1	(0x00)		/* P1�̃|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_PFR2	(0x80)		/* P2�̃|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_PFR3	(0x64)		/* P3�̃|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_PFR4	(0x01)		/* P4�̃|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_PFR5	(0x00)		/* P5�̃|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_PFR6	(0x00)		/* P6�̃|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_PFR7	(0x00)		/* P7�̃|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_PFR8	(0x00)		/* P8�̃|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_PFR9	(0x20)		/* P9�̃|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_PFR10	(0x00)		/* P10�̃|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_PFR11	(0x01)		/* P11�̃|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_PFR12	(0x00)		/* P12�̃|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_PFR13	(0x00)		/* P13�̃|�[�g�@�\���W�X�^�̒�~�l */

/* �g���|�[�g�@�\���W�X�^ */
#define INIT_EPFR0	(0x00)		/* P0�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR1	(0x00)		/* P1�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR2	(0xE0)		/* P2�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR3	(0xE0)		/* P3�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR4	(0xE0)		/* P4�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR5	(0xE0)		/* P5�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR6	(0xE0)		/* P6�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR7	(0xE0)		/* P7�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR8	(0xE0)		/* P8�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR9	(0xE0)		/* P9�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR10	(0x89)		/* P10�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR11	(0xC1)		/* P11�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR12	(0xC0)		/* P12�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR13	(0xC0)		/* P13�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR14	(0xC0)		/* P14�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR15	(0x80)		/* P15�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR16	(0x00)		/* P16�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR17	(0x00)		/* P17�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR18	(0x00)		/* P18�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR19	(0x00)		/* P19�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR20	(0x00)		/* P20�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR21	(0x00)		/* P21�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR22	(0x00)		/* P22�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR23	(0x00)		/* P23�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR24	(0xFB)		/* P24�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR25	(0xF8)		/* P25�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR26	(0xF0)		/* P26�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR27	(0xE0)		/* P27�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR28	(0xF0)		/* P28�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR29	(0x00)		/* P29�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR30	(0x00)		/* P30�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR31	(0x00)		/* P31�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR32	(0x00)		/* P32�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR33	(0xF5)		/* P33�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR34	(0xEC)		/* P34�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR35	(0xE0)		/* P35�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR36	(0xE0)		/* P36�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR37	(0x24)		/* P37�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR38	(0xE8)		/* P38�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR39	(0x00)		/* P39�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR40	(0xC0)		/* P40�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR41	(0xF8)		/* P41�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR42	(0xFC)		/* P42�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR43	(0x00)		/* P43�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR44	(0x10)		/* P44�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR45	(0x00)		/* P45�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR46	(0xC0)		/* P46�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR47	(0xFE)		/* P47�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR52	(0xFE)		/* P52�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR53	(0xE0)		/* P53�̊g���|�[�g�@�\���W�X�^�̏����l */
#define INIT_EPFR54	(0xF0)		/* P54�̊g���|�[�g�@�\���W�X�^�̏����l */

#define STOP_EPFR0	(0x00)		/* P0�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR1	(0x00)		/* P1�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR2	(0xE0)		/* P2�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR3	(0xE0)		/* P3�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR4	(0xE0)		/* P4�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR5	(0xE0)		/* P5�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR6	(0xE0)		/* P6�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR7	(0xE0)		/* P7�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR8	(0xE0)		/* P8�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR9	(0xE0)		/* P9�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR10	(0x80)		/* P10�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR11	(0xC0)		/* P11�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR12	(0xC0)		/* P12�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR13	(0xC0)		/* P13�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR14	(0xC0)		/* P14�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR15	(0x80)		/* P15�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR16	(0x00)		/* P16�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR17	(0x00)		/* P17�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR18	(0x00)		/* P18�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR19	(0x00)		/* P19�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR20	(0x00)		/* P20�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR21	(0x00)		/* P21�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR22	(0x00)		/* P22�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR23	(0x00)		/* P23�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR24	(0xF8)		/* P24�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR25	(0xF8)		/* P25�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR26	(0xF0)		/* P26�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR27	(0xE0)		/* P27�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR28	(0xF0)		/* P28�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR29	(0x00)		/* P29�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR30	(0x00)		/* P30�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR31	(0x00)		/* P31�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR32	(0x00)		/* P32�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR33	(0xE0)		/* P33�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR34	(0xE0)		/* P34�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR35	(0xE0)		/* P35�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR36	(0xE0)		/* P36�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR37	(0x00)		/* P37�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR38	(0xE0)		/* P38�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR39	(0x00)		/* P39�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR40	(0xC0)		/* P40�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR41	(0xF8)		/* P41�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR42	(0xFC)		/* P42�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR43	(0x00)		/* P43�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR44	(0x10)		/* P44�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR45	(0x00)		/* P45�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR46	(0xC0)		/* P46�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR47	(0xFE)		/* P47�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR52	(0xFE)		/* P52�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR53	(0xE0)		/* P53�̊g���|�[�g�@�\���W�X�^�̒�~�l */
#define STOP_EPFR54	(0xF0)		/* P54�̊g���|�[�g�@�\���W�X�^�̒�~�l */

/* �v���A�b�v�_�E�����䃌�W�X�^ */
#define INIT_PPCR0	(0xFF)		/* P0�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
#define INIT_PPCR1	(0xFF)		/* P1�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
#define INIT_PPCR2	(0xFF)		/* P2�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
#define INIT_PPCR3	(0xFF)		/* P3�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
#define INIT_PPCR4	(0xFF)		/* P4�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
#define INIT_PPCR5	(0xFF)		/* P5�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
#define INIT_PPCR6	(0xFF)		/* P6�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
#define INIT_PPCR7	(0xFF)		/* P7�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
#define INIT_PPCR8	(0xFF)		/* P8�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
#define INIT_PPCR9	(0xFF)		/* P9�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
#define INIT_PPCR10	(0xFF)		/* P10�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
#define INIT_PPCR11	(0xFF)		/* P11�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
#define INIT_PPCR12	(0xFF)		/* P12�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */
#define INIT_PPCR13	(0xFF)		/* P13�̃v���A�b�v�_�E�����䃌�W�X�^�̏����l */

#define STOP_PPCR0	(0xFF)		/* P0�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
#define STOP_PPCR1	(0xFF)		/* P1�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
#define STOP_PPCR2	(0xFF)		/* P2�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
#define STOP_PPCR3	(0xFF)		/* P3�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
#define STOP_PPCR4	(0xFF)		/* P4�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
#define STOP_PPCR5	(0xFF)		/* P5�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
#define STOP_PPCR6	(0xFF)		/* P6�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
#define STOP_PPCR7	(0xFF)		/* P7�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
#define STOP_PPCR8	(0xFF)		/* P8�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
#define STOP_PPCR9	(0xFF)		/* P9�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
#define STOP_PPCR10	(0xFF)		/* P10�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
#define STOP_PPCR11	(0xFF)		/* P11�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
#define STOP_PPCR12	(0xFF)		/* P12�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */
#define STOP_PPCR13	(0xFF)		/* P13�̃v���A�b�v�_�E�����䃌�W�X�^�̒�~�l */

/* �v���A�b�v�_�E�������W�X�^ */
#define INIT_PPER0	(0x00)		/* P0�̃v���A�b�v�_�E�������W�X�^�̏����l */
#define INIT_PPER1	(0x00)		/* P1�̃v���A�b�v�_�E�������W�X�^�̏����l */
#define INIT_PPER2	(0x00)		/* P2�̃v���A�b�v�_�E�������W�X�^�̏����l */
#define INIT_PPER3	(0x00)		/* P3�̃v���A�b�v�_�E�������W�X�^�̏����l */
#define INIT_PPER4	(0x00)		/* P4�̃v���A�b�v�_�E�������W�X�^�̏����l */
#define INIT_PPER5	(0x00)		/* P5�̃v���A�b�v�_�E�������W�X�^�̏����l */
#define INIT_PPER6	(0x00)		/* P6�̃v���A�b�v�_�E�������W�X�^�̏����l */
#define INIT_PPER7	(0x00)		/* P7�̃v���A�b�v�_�E�������W�X�^�̏����l */
#define INIT_PPER8	(0x00)		/* P8�̃v���A�b�v�_�E�������W�X�^�̏����l */
#define INIT_PPER9	(0x00)		/* P9�̃v���A�b�v�_�E�������W�X�^�̏����l */
#define INIT_PPER10	(0x00)		/* P10�̃v���A�b�v�_�E�������W�X�^�̏����l */
#define INIT_PPER11	(0x00)		/* P11�̃v���A�b�v�_�E�������W�X�^�̏����l */
#define INIT_PPER12	(0x00)		/* P12�̃v���A�b�v�_�E�������W�X�^�̏����l */
#define INIT_PPER13	(0x00)		/* P13�̃v���A�b�v�_�E�������W�X�^�̏����l */

#define STOP_PPER0	(0x00)		/* P0�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
#define STOP_PPER1	(0x00)		/* P1�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
#define STOP_PPER2	(0x00)		/* P2�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
#define STOP_PPER3	(0x00)		/* P3�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
#define STOP_PPER4	(0x00)		/* P4�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
#define STOP_PPER5	(0x00)		/* P5�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
#define STOP_PPER6	(0x00)		/* P6�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
#define STOP_PPER7	(0x00)		/* P7�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
#define STOP_PPER8	(0x00)		/* P8�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
#define STOP_PPER9	(0x00)		/* P9�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
#define STOP_PPER10	(0x00)		/* P10�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
#define STOP_PPER11	(0x00)		/* P11�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
#define STOP_PPER12	(0x00)		/* P12�̃v���A�b�v�_�E�������W�X�^�̒�~�l */
#define STOP_PPER13	(0x00)		/* P13�̃v���A�b�v�_�E�������W�X�^�̒�~�l */

/* �|�[�g���̓��x���I�����W�X�^ */
#define INIT_PILR0	(0xFF)		/* P0�̃|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_PILR1	(0xFF)		/* P1�̃|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_PILR2	(0xFF)		/* P2�̃|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_PILR3	(0xFF)		/* P3�̃|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_PILR4	(0xFF)		/* P4�̃|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_PILR5	(0xFF)		/* P5�̃|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_PILR6	(0xFF)		/* P6�̃|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_PILR7	(0xFF)		/* P7�̃|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_PILR8	(0xFF)		/* P8�̃|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_PILR9	(0xFF)		/* P9�̃|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_PILR10	(0xFF)		/* P10�̃|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_PILR11	(0xFF)		/* P11�̃|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_PILR12	(0xFF)		/* P12�̃|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_PILR13	(0xFF)		/* P13�̃|�[�g���̓��x���I�����W�X�^�̏����l */

#define STOP_PILR0	(0xFF)		/* P0�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_PILR1	(0xFF)		/* P1�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_PILR2	(0xFF)		/* P2�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_PILR3	(0xFF)		/* P3�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_PILR4	(0xFF)		/* P4�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_PILR5	(0xFF)		/* P5�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_PILR6	(0xFF)		/* P6�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_PILR7	(0xFF)		/* P7�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_PILR8	(0xFF)		/* P8�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_PILR9	(0xFF)		/* P9�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_PILR10	(0xFF)		/* P10�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_PILR11	(0xFF)		/* P11�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_PILR12	(0xFF)		/* P12�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_PILR13	(0xFF)		/* P13�̃|�[�g���̓��x���I�����W�X�^�̒�~�l */

/* �g���|�[�g���̓��x���I�����W�X�^ */
#define INIT_EPILR0		(0x00)		/* P0�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_EPILR1		(0x00)		/* P1�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_EPILR2		(0x00)		/* P2�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_EPILR3		(0x00)		/* P3�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_EPILR4		(0x00)		/* P4�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_EPILR5		(0x00)		/* P5�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_EPILR6		(0x00)		/* P6�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_EPILR7		(0x00)		/* P7�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_EPILR8		(0x00)		/* P8�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_EPILR9		(0x00)		/* P9�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_EPILR10	(0x00)		/* P10�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_EPILR11	(0x00)		/* P11�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_EPILR12	(0x00)		/* P12�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */
#define INIT_EPILR13	(0x00)		/* P13�̊g���|�[�g���̓��x���I�����W�X�^�̏����l */

#define STOP_EPILR0		(0x00)		/* P0�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_EPILR1		(0x00)		/* P1�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_EPILR2		(0x00)		/* P2�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_EPILR3		(0x00)		/* P3�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_EPILR4		(0x00)		/* P4�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_EPILR5		(0x00)		/* P5�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_EPILR6		(0x00)		/* P6�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_EPILR7		(0x00)		/* P7�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_EPILR8		(0x00)		/* P8�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_EPILR9		(0x00)		/* P9�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_EPILR10	(0x00)		/* P10�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_EPILR11	(0x00)		/* P11�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_EPILR12	(0x00)		/* P12�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */
#define STOP_EPILR13	(0x00)		/* P13�̊g���|�[�g���̓��x���I�����W�X�^�̒�~�l */

/* �|�[�g�o�͋쓮���W�X�^ */
#define INIT_PODR0		(0xFF)		/* P0�̃|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_PODR1		(0xFF)		/* P1�̃|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_PODR2		(0xFF)		/* P2�̃|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_PODR3		(0xFF)		/* P3�̃|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_PODR4		(0xFF)		/* P4�̃|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_PODR5		(0xFF)		/* P5�̃|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_PODR6		(0xFF)		/* P6�̃|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_PODR7		(0xFF)		/* P7�̃|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_PODR8		(0xFF)		/* P8�̃|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_PODR9		(0xFF)		/* P9�̃|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_PODR10		(0xFF)		/* P10�̃|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_PODR11		(0xFF)		/* P11�̃|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_PODR12		(0xFF)		/* P12�̃|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_PODR13		(0xFF)		/* P13�̃|�[�g�o�͋쓮���W�X�^�̏����l */

#define STOP_PODR0		(0xFF)		/* P0�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_PODR1		(0xFF)		/* P1�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_PODR2		(0xFF)		/* P2�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_PODR3		(0xFF)		/* P3�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_PODR4		(0xFF)		/* P4�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_PODR5		(0xFF)		/* P5�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_PODR6		(0xFF)		/* P6�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_PODR7		(0xFF)		/* P7�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_PODR8		(0xFF)		/* P8�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_PODR9		(0xFF)		/* P9�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_PODR10		(0xFF)		/* P10�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_PODR11		(0xFF)		/* P11�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_PODR12		(0xFF)		/* P12�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_PODR13		(0xFF)		/* P13�̃|�[�g�o�͋쓮���W�X�^�̒�~�l */

/* �g���|�[�g�o�͋쓮���W�X�^ */
#define INIT_EPODR1		(0x00)		/* P1�̊g���|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_EPODR2		(0x00)		/* P2�̊g���|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_EPODR3		(0x00)		/* P3�̊g���|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_EPODR6		(0x00)		/* P6�̊g���|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_EPODR7		(0x00)		/* P7�̊g���|�[�g�o�͋쓮���W�X�^�̏����l */
#define INIT_EPODR8		(0x00)		/* P8�̊g���|�[�g�o�͋쓮���W�X�^�̏����l */

#define STOP_EPODR1		(0x00)		/* P1�̊g���|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_EPODR2		(0x00)		/* P2�̊g���|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_EPODR3		(0x00)		/* P3�̊g���|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_EPODR6		(0x00)		/* P6�̊g���|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_EPODR7		(0x00)		/* P7�̊g���|�[�g�o�͋쓮���W�X�^�̒�~�l */
#define STOP_EPODR8		(0x00)		/* P8�̊g���|�[�g�o�͋쓮���W�X�^�̒�~�l */

/* �|�[�g���͋����W�X�^ */
#define INIT_PORTEN		(0x01)		/* �|�[�g���͋����W�X�^�̏����l */

#define STOP_PORTEN		(0x01)		/* �|�[�g���͋����W�X�^�̒�~�l */


#endif								/* _MB91570_PORT_DRV_INTERNAL__ */


/* �O�����J�萔 */
/*==============================================================================*/
/*	enum��`�i�O������J�F�A�v�����Ŏg�p����ꍇ�A�O�����J�p�ɍēx��`���邱��	*/
/*==============================================================================*/
/* �|�[�g�f�[�^���W�X�^�ԍ� */
typedef enum {
	PORTDRV_PORT_0 = 0,			/* P0�̃|�[�g�f�[�^���W�X�^�ԍ� */
	PORTDRV_PORT_1,				/* P1�̃|�[�g�f�[�^���W�X�^�ԍ� */
	PORTDRV_PORT_2,				/* P2�̃|�[�g�f�[�^���W�X�^�ԍ� */
	PORTDRV_PORT_3,				/* P3�̃|�[�g�f�[�^���W�X�^�ԍ� */
	PORTDRV_PORT_4,				/* P4�̃|�[�g�f�[�^���W�X�^�ԍ� */
	PORTDRV_PORT_5,				/* P5�̃|�[�g�f�[�^���W�X�^�ԍ� */
	PORTDRV_PORT_6,				/* P6�̃|�[�g�f�[�^���W�X�^�ԍ� */
	PORTDRV_PORT_7,				/* P7�̃|�[�g�f�[�^���W�X�^�ԍ� */
	PORTDRV_PORT_8,				/* P8�̃|�[�g�f�[�^���W�X�^�ԍ� */
	PORTDRV_PORT_9,				/* P9�̃|�[�g�f�[�^���W�X�^�ԍ� */
	PORTDRV_PORT_10,			/* P10�̃|�[�g�f�[�^���W�X�^�ԍ� */
	PORTDRV_PORT_11,			/* P11�̃|�[�g�f�[�^���W�X�^�ԍ� */
	PORTDRV_PORT_12,			/* P12�̃|�[�g�f�[�^���W�X�^�ԍ� */
	PORTDRV_PORT_13,			/* P13�̃|�[�g�f�[�^���W�X�^�ԍ� */
	PORTDRV_PORT_MAX			/* �|�[�g�f�[�^���W�X�^�ԍ�MAX�l */
} E_PORTDRV_PORT_NUM;

/* �|�[�g�f�[�^���W�X�^��BIT�ԍ� */
typedef enum {
	PORTDRV_BIT_0 = 0,			/* �|�[�g�f�[�^���W�X�^��BIT�ԍ�0 */
	PORTDRV_BIT_1,				/* �|�[�g�f�[�^���W�X�^��BIT�ԍ�1 */
	PORTDRV_BIT_2,				/* �|�[�g�f�[�^���W�X�^��BIT�ԍ�2 */
	PORTDRV_BIT_3,				/* �|�[�g�f�[�^���W�X�^��BIT�ԍ�3 */
	PORTDRV_BIT_4,				/* �|�[�g�f�[�^���W�X�^��BIT�ԍ�4 */
	PORTDRV_BIT_5,				/* �|�[�g�f�[�^���W�X�^��BIT�ԍ�5 */
	PORTDRV_BIT_6,				/* �|�[�g�f�[�^���W�X�^��BIT�ԍ�6 */
	PORTDRV_BIT_7,				/* �|�[�g�f�[�^���W�X�^��BIT�ԍ�7 */
	PORTDRV_BIT_MAX				/* �|�[�g�f�[�^���W�X�^��BIT�ԍ�MAX�l */
} E_PORTDRV_BIT_NUM;

/* �|�[�g�f�[�^���W�X�^��BYTE�ԍ� */
typedef enum {
	PORTDRV_BYTE_0 = 0,			/* �|�[�g�f�[�^���W�X�^��BYTE�ԍ�0 */
	PORTDRV_BYTE_1,				/* �|�[�g�f�[�^���W�X�^��BYTE�ԍ�1 */
	PORTDRV_BYTE_2,				/* �|�[�g�f�[�^���W�X�^��BYTE�ԍ�2 */
	PORTDRV_BYTE_3,				/* �|�[�g�f�[�^���W�X�^��BYTE�ԍ�3 */
	PORTDRV_BYTE_4,				/* �|�[�g�f�[�^���W�X�^��BYTE�ԍ�4 */
	PORTDRV_BYTE_5,				/* �|�[�g�f�[�^���W�X�^��BYTE�ԍ�5 */
	PORTDRV_BYTE_6,				/* �|�[�g�f�[�^���W�X�^��BYTE�ԍ�6 */
	PORTDRV_BYTE_7,				/* �|�[�g�f�[�^���W�X�^��BYTE�ԍ�7 */
	PORTDRV_BYTE_MAX			/* �|�[�g�f�[�^���W�X�^��BYTE�ԍ�MAX�l */
} E_PORTDRV_BYTE_NUM;

/* �|�[�g�@�\�؂�ւ��pID */
typedef enum {
	PORT_IN = 0,				/* �ėp���̓|�[�g */
	PORT_OUT,					/* �ėp�o�̓|�[�g */
	BUS_DATA_16BIT_IN,			/* �O���o�X�f�[�^���� 16bit */
	BUS_DATA_16BIT_OUT,			/* �O���o�X�f�[�^�o�� 16bit */
	LIN_UART_SERIAL_IN,			/* LIN_UART �V���A������ */
	RELOAD_TIMER_EVENT_IN,		/* �����[�h�^�C�}�C�x���g���� */
	PPG_OUT,					/* PPG �o�� */
	BUS_DATA_8BIT_IN,			/* �O���o�X�f�[�^���� 8bit */
	BUS_DATA_8BIT_OUT,			/* �O���o�X�f�[�^�o�� 8bit */
	INTTERUPT_REQUEST_IN,		/* �O�������ݗv������ */
	BUS_DATA_32BIT_IN,			/* �O���o�X�f�[�^���� 32bit */
	BUS_DATA_32BIT_OUT,			/* �O���o�X�f�[�^�o�� 32bit */
	LCD_SEG_DUTY_OUT,			/* LCDC �Z�O�����g(Duty)�o�� */
	BUS_ADDRESS_STROBE_OUT,		/* �O���o�X�A�h���X�X�g���[�u�o�� */
	ICU_IN,						/* �C���v�b�g�L���v�`������ */
	OCU_OUT,					/* �A�E�g�v�b�g�R���y�A�o�� */
	BUS_CHIPSET_OUT,			/* �O���o�X�`�b�v�Z���N�g �o�� */
	BUS_READ_STROBE_OUT,		/* �O���o�X���[�h�X�g���[�u�o�� */
	BUS_WRITE_STROBE_OUT,		/* �O���o�X���C�g�X�g���[�u�o�� */
	BUS_ADDRESS_OUT,			/* �O���o�X�A�h���X�o�� */
	MULTI_FUNCTION_SERIAL_IN,	/* �}���`�t�@���N�V�����V���A������ */
	MULTI_FUNCTION_SERIAL_OUT,	/* �}���`�t�@���N�V�����V���A���o�� */
	LCD_SEG_STATIC_OUT,			/* LCDC �Z�O�����g(Static)�o�� */
	UPDOWN_COUNTER_AIN_IN,		/* �A�b�v�_�E���J�E���^AIN ���� */
	UPDOWN_COUNTER_BIN_IN,		/* �A�b�v�_�E���J�E���^BIN ���� */
	SOUND_GENERATOR_SGA_OUT,	/* �T�E���h�W�F�l���[�^SGA �o�� */
	UPDOWN_COUNTER_ZIN_IN,		/* �A�b�v�_�E���J�E���^ZIN ���� */
	SOUND_GENERATOR_SGO_OUT,	/* �T�E���h�W�F�l���[�^SGO �o�� */
	MULTI_FUNCTION_SCK_IN,		/* �}���`�t�@���N�V�����V���A���N���b�N���� */
	MULTI_FUNCTION_SCK_OUT,		/* �}���`�t�@���N�V�����V���A���N���b�N�o�� */
	LCD_SEG_DUTY_OUT_COM_OUT,	/* LCDC �Z�O�����g(Duty)�R�����o�� */
	FREE_RUN_TIMCLK_IN,			/* �t���[�����^�C�}�N���b�N���� */
	LCD_V_IN,					/* LCDC ��d������ */
	BUS_RDY_IN,					/* �O���o�XRDY ���� */
	SMC_OUT,					/* SMC �o�� */
	LIN_UART_SCK_IN,			/* LIN_UART �V���A���N���b�N���� */
	LIN_UART_SCK_OUT,			/* LIN_UART �V���A���N���b�N�o�� */
	ADC_TRG_IN,					/* ADC �O���g���K���� */
	RELOAD_TIMER_OUT,			/* �����[�h�^�C�}�o�� */
	CAN_TX_OUT,					/* CAN TX �f�[�^�o�� */
	CAN_RX_IN,					/* CAN RX �f�[�^���� */
	RTC_OVERFLOW_OUT,			/* RTC �I�[�o�t���[�o�� */
	DAC_OUT,					/* DAC �o�� */
	PPG_TRG_IN,					/* PPG �g���K���� */
	BASE_TIMER_IN,				/* �x�[�X�^�C�}���� */
	SUB_CLK_OSC_OUT,			/* �T�u�N���b�NOSC�o�� */
	SUB_CLK_OSC_IN,				/* �T�u�N���b�NOSC ���� */
	LIN_UART_SERIAL_OUT,		/* LIN_UART �V���A���o�� */
	BUS_CLK_OUT,				/* �O���o�X�N���b�N�o�� */
	ADC_ANALOG_IN,				/* ADC �A�i���O���� */
	BASE_TIMER_OUT,				/* �x�[�X�^�C�}�o�� */
	NO_USE,						/* �s�g�p */
	PORT_FUNC_CHANGE_ID_MAX		/* MAX�l */
} E_PORT_FUNC_CHANGE_ID;

/*==============================================================================*/
/*	define��`�i�O������J�F�O���ł̎g�p�֎~�j									*/
/*==============================================================================*/
#define PORT_DRV_FUNC_CHANGE	1	/* ROM�e�ʁA�������ԍ팸�̂��߂̃R���t�B�O(�Ώۊ֐���PortDrv_SetFuncBit) */
									/* 0:�Ώۊ֐��ƑΏۊ֐��̂ݎg�p���Ă���ROM�e�[�u�����R���p�C������Ȃ� */
									/* 1:�Ώۊ֐��ƑΏۊ֐��̂ݎg�p���Ă���ROM�e�[�u�����R���p�C������� */

/*==============================================================================*/
/*	define��`�i�O�����J�j														*/
/*==============================================================================*/
/* �r�b�g�}�X�N�ʒu */
#define PORT_BIT_MASK_0		(0x00u)	/* P0�̃r�b�g�}�X�N�ʒu		*/
#define PORT_BIT_MASK_1		(0x00u)	/* P1�̃r�b�g�}�X�N�ʒu		*/
#define PORT_BIT_MASK_2		(0x00u)	/* P2�̃r�b�g�}�X�N�ʒu		*/
#define PORT_BIT_MASK_3		(0x00u)	/* P3�̃r�b�g�}�X�N�ʒu		*/
#define PORT_BIT_MASK_4		(0x00u)	/* P4�̃r�b�g�}�X�N�ʒu		*/
#define PORT_BIT_MASK_5		(0x00u)	/* P5�̃r�b�g�}�X�N�ʒu		*/
#define PORT_BIT_MASK_6		(0x00u)	/* P6�̃r�b�g�}�X�N�ʒu		*/
#define PORT_BIT_MASK_7		(0x00u)	/* P7�̃r�b�g�}�X�N�ʒu		*/
#define PORT_BIT_MASK_8		(0x00u)	/* P8�̃r�b�g�}�X�N�ʒu		*/
#define PORT_BIT_MASK_9		(0x01u)	/* P9�̃r�b�g�}�X�N�ʒu(ADC5V EN)	*/
#define PORT_BIT_MASK_10	(0x00u)	/* P10�̃r�b�g�}�X�N�ʒu	*/
#define PORT_BIT_MASK_11	(0x00u)	/* P11�̃r�b�g�}�X�N�ʒu	*/
#define PORT_BIT_MASK_12	(0x60u)	/* P12�̃r�b�g�}�X�N�ʒu(3.3V EN, �d�����ȕێ�)	*/
#define PORT_BIT_MASK_13	(0x00u)	/* P13�̃r�b�g�}�X�N�ʒu	*/

#define		R_PO_EEPMW_DI					PDR10_P5			/* EEPROM DI�o��latch */
#define		R_PO_EEPMW_SK					PDR10_P6			/* EEPROM SK�o��latch */ 
#define		R_PO_EEPMW_CS					PDR10_P7			/* EEPROM CS�o��latch */
#define		R_PI_EEPMW_DO					PDR10_P4			/* EEPROM DO����latch */

#define 	IO_PO_GR_BT_RST					PORTDRV_PORT_2		/* BT���W���[�����Z�b�g�[�q �|�[�g�O���[�v */
#define 	IO_PO_BIT_BT_RST				PORTDRV_BIT_1		/* BT���W���[�����Z�b�g�[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PO_LEV_NONACT_BT_RST			(0x00)				/* BT���W���[�����Z�b�g�[�q NonActive���x��(Low) */
#define		IO_PO_LEV_ACTIVE_BT_RST			(0x01)				/* BT���W���[�����Z�b�g�[�q Active���x��(High) */

#define 	IO_PO_GR_GNSS_RST				PORTDRV_PORT_2		/* GNSS���W���[�����Z�b�g�[�q �|�[�g�O���[�v */
#define 	IO_PO_BIT_GNSS_RST				PORTDRV_BIT_2		/* GNSS���W���[�����Z�b�g�[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PO_LEV_NONACT_GNSS_RST		(0x00)				/* GNSS���W���[�����Z�b�g�[�q NonActive���x��(Low) */
#define		IO_PO_LEV_ACTIVE_GNSS_RST		(0x01)				/* GNSS���W���[�����Z�b�g�[�q Active���x��(High) */

#define 	IO_PI_GR_SD_WP					PORTDRV_PORT_2		/* SD�J�[�h���C�g�v���e�N�g�[�q �|�[�g�O���[�v */
#define 	IO_PI_BIT_SD_WP					PORTDRV_BIT_3		/* SD�J�[�h���C�g�v���e�N�g�[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PI_LEV_NONACT_SD_WP			(0x01)				/* SD�J�[�h���C�g�v���e�N�g�[�q NonActive���x��(High) */
#define		IO_PI_LEV_ACTIVE_SD_WP			(0x00)				/* SD�J�[�h���C�g�v���e�N�g�[�q Active���x��(Low) */

#define 	IO_PI_GR_SD_DTCT				PORTDRV_PORT_2		/* SD�J�[�h�f�B�e�N�g�[�q �|�[�g�O���[�v */
#define 	IO_PI_BIT_SD_DTCT				PORTDRV_BIT_4		/* SD�J�[�h�f�B�e�N�g�[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PI_LEV_NONACT_SD_DTCT		(0x01)				/* SD�J�[�h�f�B�e�N�g�[�q NonActive���x��(High) */
#define		IO_PI_LEV_ACTIVE_SD_DTCT		(0x00)				/* SD�J�[�h�f�B�e�N�g�[�q Active���x��(Low) */

#define 	IO_PO_GR_SD_SPI_CS				PORTDRV_PORT_2		/* SD�J�[�h SPI�`�b�v�Z���N�g�[�q �|�[�g�O���[�v */
#define 	IO_PO_BIT_SD_SPI_CS				PORTDRV_BIT_5		/* SD�J�[�h SPI�`�b�v�Z���N�g�[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PO_LEV_NONACT_SD_SPI_CS		(0x01)				/* SD�J�[�h SPI�`�b�v�Z���N�g�[�q NonActive���x��(High) */
#define		IO_PO_LEV_ACTIVE_SD_SPI_CS		(0x00)				/* SD�J�[�h SPI�`�b�v�Z���N�g�[�q Active���x��(Low) */

#define 	IO_PI_GR_ADVANCE_VIGN			PORTDRV_PORT_4		/* ADVANCE VIGN���o�[�q �|�[�g�O���[�v */
#define 	IO_PI_BIT_ADVANCE_VIGN			PORTDRV_BIT_2		/* ADVANCE VIGN���o�[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PI_LEV_NONACT_ADVANCE_VIGN	(0x01)				/* ADVANCE VIGN���o�[�q NonActive���x��(High) */
#define		IO_PI_LEV_ACTIVE_ADVANCE_VIGN	(0x00)				/* ADVANCE VIGN���o�[�q Active���x��(Low) */

#define 	IO_PO_GR_ADC5V_EN				PORTDRV_PORT_9		/* ADC5V EN�[�q �|�[�g�O���[�v */
#define 	IO_PO_BIT_ADC5V_EN				PORTDRV_BIT_0		/* ADC5V EN�[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PO_LEV_NONACT_ADC5V_EN		(0x01)				/* ADC5V EN�[�q NonActive���x��(High) */
#define		IO_PO_LEV_ACTIVE_ADC5V_EN		(0x00)				/* ADC5V EN�[�q Active���x��(Low) */

#define 	IO_PI_GR_MODEL_SW1				PORTDRV_PORT_9		/* �@��ؑ֓���1�[�q �|�[�g�O���[�v */
#define 	IO_PI_BIT_MODEL_SW1				PORTDRV_BIT_4		/* �@��ؑ֓���1�[�q �|�[�g�r�b�g�ԍ� */
#define 	IO_PI_GR_CAN_SW					PORTDRV_PORT_9		/* CAN�ؑ֓��͒[�q �|�[�g�O���[�v */
#define 	IO_PI_BIT_CAN_SW				PORTDRV_BIT_3		/* CAN�ؑ֓��͒[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PI_LEV_CAN_SW_ISOCAN			(0x00)				/* CAN�ؑ֓��͒[�q ���x��Low(ISO-CAN) */
#define		IO_PI_LEV_CAN_SW_VEHICLECAN		(0x01)				/* CAN�ؑ֓��͒[�q ���x��High(VEHICLE-CAN) */

#define 	IO_PO_GR_LOCAL_CAN_STB			PORTDRV_PORT_11		/* LOCAL CAN STB���[�h�o�͒[�q �|�[�g�O���[�v */
#define 	IO_PO_BIT_LOCAL_CAN_STB			PORTDRV_BIT_2		/* LOCAL CAN STB���[�h�o�͒[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PO_LEV_NONACT_LOCAL_CAN_STB	(0x00)				/* LOCAL CAN STB���[�h�o�͒[�q NonActive���x��(Low) */
#define		IO_PO_LEV_ACTIVE_LOCAL_CAN_STB	(0x01)				/* LOCAL CAN STB���[�h�o�͒[�q Active���x��(High) */

#define 	IO_PO_GR_CAN_STB				PORTDRV_PORT_12		/* �ԗ�CAN STB���[�h�o�͒[�q �|�[�g�O���[�v */
#define 	IO_PO_BIT_CAN_STB				PORTDRV_BIT_4		/* �ԗ�CAN STB���[�h�o�͒[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PO_LEV_NONACT_CAN_STB		(0x00)				/* �ԗ�CAN STB���[�h�o�͒[�q NonActive���x��(Low) */
#define		IO_PO_LEV_ACTIVE_CAN_STB		(0x01)				/* �ԗ�CAN STB���[�h�o�͒[�q Active���x��(High) */

#define 	IO_PO_GR_5V_HOLD				PORTDRV_PORT_12		/* �d�����ȕێ��o�͒[�q �|�[�g�O���[�v */
#define 	IO_PO_BIT_5V_HOLD				PORTDRV_BIT_5		/* �d�����ȕێ��o�͒[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PO_LEV_NONACT_5V_HOLD		(0x00)				/* �d�����ȕێ��o�͒[�q NonActive���x��(Low) */
#define		IO_PO_LEV_ACTIVE_5V_HOLD		(0x01)				/* �d�����ȕێ��o�͒[�q Active���x��(High) */

#define 	IO_PO_GR_33V_EN					PORTDRV_PORT_12		/* 3.3V EN�[�q �|�[�g�O���[�v */
#define 	IO_PO_BIT_33V_EN				PORTDRV_BIT_6		/* 3.3V EN�[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PO_LEV_NONACT_33V_EN			(0x00)				/* 3.3V EN�[�q NonActive���x��(Low) */
#define		IO_PO_LEV_ACTIVE_33V_EN			(0x01)				/* 3.3V EN�[�q Active���x��(High) */

#define 	IO_PI_GR_ACC_STAT				PORTDRV_PORT_13		/* ACC��ԓ��͒[�q �|�[�g�O���[�v */
#define 	IO_PI_BIT_ACC_STAT				PORTDRV_BIT_3		/* ACC��ԓ��͒[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PI_LEV_NONACT_ACC_STAT		(0x01)				/* ACC��ԓ��͒[�q NonActive���x��(High) */
#define		IO_PI_LEV_ACTIVE_ACC_STAT		(0x00)				/* ACC��ԓ��͒[�q Active���x��(Low) */

#define 	IO_PO_GR_DEFI_CAN_CTRL			PORTDRV_PORT_13		/* Defi-CAN CTRL�o�͒[�q �|�[�g�O���[�v */
#define 	IO_PO_BIT_DEFI_CAN_CTRL			PORTDRV_BIT_4		/* Defi-CAN CTRL�o�͒[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PO_LEV_NONACT_DEFI_CAN_CTRL	(0x00)				/* Defi-CAN CTRL�o�͒[�q NonActive���x��(Low) */
#define		IO_PO_LEV_ACTIVE_DEFI_CAN_CTRL	(0x01)				/* Defi-CAN CTRL�o�͒[�q Active���x��(High) */

#define 	IO_PO_GR_BLE_DTR						PORTDRV_PORT_1		/* BT���W���[�� BLE_DTR�[�q �|�[�g�O���[�v */
#define 	IO_PO_BIT_BLE_DTR						PORTDRV_BIT_5		/* BT���W���[�� BLE_DTR�[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PO_LEV_BLE_DTR_LOW					(0x01)				/* BT���W���[�� BLE_DTR�[�q Low */
#define		IO_PO_LEV_BLE_DTR_HIGH					(0x00)				/* BT���W���[�� BLE_DTR�[�q High */
//#define		IO_PO_LEV_BLE_DTR_LOW					(0x00)				/* BT���W���[�� BLE_DTR�[�q Low */
//#define		IO_PO_LEV_BLE_DTR_HIGH					(0x01)				/* BT���W���[�� BLE_DTR�[�q High */

#define 	IO_PO_GR_BLE_DSR						PORTDRV_PORT_2		/* BT���W���[�� BLE_DSR�[�q �|�[�g�O���[�v */
#define 	IO_PO_BIT_BLE_DSR						PORTDRV_BIT_0		/* BT���W���[�� BLE_DSR�[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PO_LEV_BLE_DSR_LOW					(0x01)				/* BT���W���[�� BLE_DSR�[�q Low */
#define		IO_PO_LEV_BLE_DSR_HIGH					(0x00)				/* BT���W���[�� BLE_DSR�[�q High */
//#define		IO_PO_LEV_BLE_DSR_LOW					(0x00)				/* BT���W���[�� BLE_DSR�[�q Low */
//#define		IO_PO_LEV_BLE_DSR_HIGH					(0x01)				/* BT���W���[�� BLE_DSR�[�q High */

#define 	IO_PO_GR_BLE_SW2						PORTDRV_PORT_1		/* BT���W���[�� BLE_SW2�[�q �|�[�g�O���[�v */
#define 	IO_PO_BIT_BLE_SW2						PORTDRV_BIT_3		/* BT���W���[�� BLE_SW2�[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PO_LEV_BLE_SW2_LOW					(0x00)				/* BT���W���[�� BLE_SW2�[�q Low */
#define		IO_PO_LEV_BLE_SW2_HIGH					(0x01)				/* BT���W���[�� BLE_SW2�[�q High */

#define 	IO_PO_GR_BLE_SW1						PORTDRV_PORT_1		/* BT���W���[�� BLE_SW1�[�q �|�[�g�O���[�v */
#define 	IO_PO_BIT_BLE_SW1						PORTDRV_BIT_4		/* BT���W���[�� BLE_SW1�[�q �|�[�g�r�b�g�ԍ� */
#define		IO_PO_LEV_BLE_SW1_LOW					(0x00)				/* BT���W���[�� BLE_SW1�[�q Low */
#define		IO_PO_LEV_BLE_SW1_HIGH					(0x01)				/* BT���W���[�� BLE_SW1�[�q High */

#endif								/*	__MB91570_PORT_DRV_CONFIG_H__ 	*/

