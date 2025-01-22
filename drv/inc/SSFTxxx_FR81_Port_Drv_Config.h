/************************************************************************************************/
/* 客先名		:	機種依存モジュール															*/
/* 機種名		:	-																			*/
/* ﾏｲｺﾝｿﾌﾄﾃｰﾏ名	:	-																			*/
/*==============================================================================================*/
/* 作成ﾌｧｲﾙ名	:	SSFTxxx_FR81_Port_Drv_Config.h												*/
/* 				:	GPIOﾄﾞﾗｲﾊﾞﾍｯﾀﾞ																*/
/*==============================================================================================*/
/* 対象ﾏｲｺﾝ		:	MB91570 Series																*/
/*==============================================================================================*/
/* 作成ﾊﾞｰｼﾞｮﾝ	:	030302																		*/
/* 作成年月日	:	2014.06.10																	*/
/* 作成者		:	K.Uchiyama																	*/
/*----------------------------------------------------------------------------------------------*/
/* 変更履歴		:																				*/
/* [010103]		:	新規作成																	*/
/* [010104]		:	ビットマスク位置の定義箇所変更。セキュリティインジケータのポート修正。		*/
/* [010105]		:	EEPROM、シフトレジスタ、HWD、LINのポート定義追加							*/
/* [030101]		:	ROM容量と処理時間削減のため、コンパイルSW追加								*/
/* [030301]		:	レジスタの再定義名の変更													*/
/* [030302]		:	マイコンSLEEP時にポートが入力設定(H.I)になっている							*/
/************************************************************************************************/
#ifndef __MB91570_PORT_DRV_CONFIG_H__
#define __MB91570_PORT_DRV_CONFIG_H__

/* 内部定数 */
#ifdef __MB91570_PORT_DRV_INTERNAL__				
/*** START_INC ***/
/********************************************************************************/
/*   Include File                                                               */
/*------------------------------------------------------------------------------*/
/*      ﾍｯﾀﾞｰﾌｧｲﾙのｲﾝｸﾙｰﾄﾞ文は、下記ﾌｧｲﾙに記載すること                          */
/********************************************************************************/
#include "SSFTSTD_FR81_Interrupt_Drv.h"

/*==============================================================================*/
/*	define定義（機種依存）														*/
/*==============================================================================*/
/* ポートデータレジスタ */
#define INIT_PDR0	(0x00)		/* P0のポートデータレジスタの初期値 */
#define INIT_PDR1	(0x00)		/* P1のポートデータレジスタの初期値 */
#define INIT_PDR2	(0x20)		/* P2のポートデータレジスタの初期値 */
#define INIT_PDR3	(0x00)		/* P3のポートデータレジスタの初期値 */
#define INIT_PDR4	(0x00)		/* P4のポートデータレジスタの初期値 */
#define INIT_PDR5	(0x00)		/* P5のポートデータレジスタの初期値 */
#define INIT_PDR6	(0x00)		/* P6のポートデータレジスタの初期値 */
#define INIT_PDR7	(0x00)		/* P7のポートデータレジスタの初期値 */
#define INIT_PDR8	(0x00)		/* P8のポートデータレジスタの初期値 */
#define INIT_PDR9	(0x01)		/* P9のポートデータレジスタの初期値 */
#define INIT_PDR10	(0x00)		/* P10のポートデータレジスタの初期値 */
#define INIT_PDR11	(0x04)		/* P11のポートデータレジスタの初期値 */
#define INIT_PDR12	(0x10)		/* P12のポートデータレジスタの初期値 */
#define INIT_PDR13	(0x00)		/* P13のポートデータレジスタの初期値 */

#define STOP_PDR0	(0x00)		/* P0のポートデータレジスタの停止値 */
#define STOP_PDR1	(0x00)		/* P1のポートデータレジスタの停止値 */
#define STOP_PDR2	(0x20)		/* P2のポートデータレジスタの停止値 */
#define STOP_PDR3	(0x00)		/* P3のポートデータレジスタの停止値 */
#define STOP_PDR4	(0x00)		/* P4のポートデータレジスタの停止値 */
#define STOP_PDR5	(0x00)		/* P5のポートデータレジスタの停止値 */
#define STOP_PDR6	(0x00)		/* P6のポートデータレジスタの停止値 */
#define STOP_PDR7	(0x00)		/* P7のポートデータレジスタの停止値 */
#define STOP_PDR8	(0x00)		/* P8のポートデータレジスタの停止値 */
#define STOP_PDR9	(0x00)		/* P9のポートデータレジスタの停止値 */
								/* AD5V ENはFrameworkで停止する。GPIOは値を保持(P090) */
#define STOP_PDR10	(0x00)		/* P10のポートデータレジスタの停止値 */
#define STOP_PDR11	(0x04)		/* P11のポートデータレジスタの停止値 */
#define STOP_PDR12	(0x70)		/* P12のポートデータレジスタの停止値 */
								/* 自己保持、3.3V EN、AD5V ENはFrameworkで停止する。GPIOは値を保持(P125、P126) */
#define STOP_PDR13	(0x00)		/* P13のポートデータレジスタの停止値 */

/* データ方向レジスタ */
#define INIT_DDR0	(0xFF)		/* P0のデータ方向レジスタの初期値 */
#define INIT_DDR1	(0xA7)		/* P1のデータ方向レジスタの初期値 */
#define INIT_DDR2	(0xA6)		/* P2のデータ方向レジスタの初期値 */
#define INIT_DDR3	(0x65)		/* P3のデータ方向レジスタの初期値 */
#define INIT_DDR4	(0xFB)		/* P4のデータ方向レジスタの初期値 */
#define INIT_DDR5	(0xFF)		/* P5のデータ方向レジスタの初期値 */
#define INIT_DDR6	(0xFF)		/* P6のデータ方向レジスタの初期値 */
#define INIT_DDR7	(0xFF)		/* P7のデータ方向レジスタの初期値 */
#define INIT_DDR8	(0xFF)		/* P8のデータ方向レジスタの初期値 */
#define INIT_DDR9	(0xA7)		/* P9のデータ方向レジスタの初期値 */
#define INIT_DDR10	(0xEE)		/* P10のデータ方向レジスタの初期値 */
#define INIT_DDR11	(0xF5)		/* P11のデータ方向レジスタの初期値 */
#define INIT_DDR12	(0xFF)		/* P12のデータ方向レジスタの初期値 */
#define INIT_DDR13	(0xD7)		/* P13のデータ方向レジスタの初期値 */

#define STOP_DDR0	(0xFF)		/* P0のデータ方向レジスタの停止値 */
#define STOP_DDR1	(0xA7)		/* P1のデータ方向レジスタの停止値 */
#define STOP_DDR2	(0xA6)		/* P2のデータ方向レジスタの停止値 */
#define STOP_DDR3	(0x65)		/* P3のデータ方向レジスタの停止値 */
#define STOP_DDR4	(0xFB)		/* P4のデータ方向レジスタの停止値 */
#define STOP_DDR5	(0xFF)		/* P5のデータ方向レジスタの停止値 */
#define STOP_DDR6	(0xFF)		/* P6のデータ方向レジスタの停止値 */
#define STOP_DDR7	(0xFF)		/* P7のデータ方向レジスタの停止値 */
#define STOP_DDR8	(0xFF)		/* P8のデータ方向レジスタの停止値 */
#define STOP_DDR9	(0xA7)		/* P9のデータ方向レジスタの停止値 */
#define STOP_DDR10	(0xEE)		/* P10のデータ方向レジスタの停止値 */
#define STOP_DDR11	(0xF5)		/* P11のデータ方向レジスタの停止値 */
#define STOP_DDR12	(0xFF)		/* P12のデータ方向レジスタの停止値 */
#define STOP_DDR13	(0xD7)		/* P13のデータ方向レジスタの停止値 */

#define REFRESH_DDR0	(0xFF)		/* P0のデータ方向レジスタの再設定値 */
#define REFRESH_DDR1	(0xA7)		/* P1のデータ方向レジスタの再設定値 */
#define REFRESH_DDR2	(0xA7)		/* P2のデータ方向レジスタの再設定値 */
#define REFRESH_DDR3	(0x65)		/* P3のデータ方向レジスタの再設定値 */
#define REFRESH_DDR4	(0xFB)		/* P4のデータ方向レジスタの再設定値 */
#define REFRESH_DDR5	(0xFF)		/* P5のデータ方向レジスタの再設定値 */
#define REFRESH_DDR6	(0xFF)		/* P6のデータ方向レジスタの再設定値 */
#define REFRESH_DDR7	(0xFF)		/* P7のデータ方向レジスタの再設定値 */
#define REFRESH_DDR8	(0xFF)		/* P8のデータ方向レジスタの再設定値 */
#define REFRESH_DDR9	(0xA7)		/* P9のデータ方向レジスタの再設定値 */
#define REFRESH_DDR10	(0xEE)		/* P10のデータ方向レジスタの再設定値 */
#define REFRESH_DDR11	(0xF5)		/* P11のデータ方向レジスタの再設定値 */
#define REFRESH_DDR12	(0xFF)		/* P12のデータ方向レジスタの再設定値 */
#define REFRESH_DDR13	(0xD7)		/* P13のデータ方向レジスタの再設定値 */

/* ポート機能レジスタ */
#define INIT_PFR0	(0x07)		/* P0のポート機能レジスタの初期値 */
#define INIT_PFR1	(0x00)		/* P1のポート機能レジスタの初期値 */
#define INIT_PFR2	(0x80)		/* P2のポート機能レジスタの初期値 */
#define INIT_PFR3	(0x64)		/* P3のポート機能レジスタの初期値 */
#define INIT_PFR4	(0x01)		/* P4のポート機能レジスタの初期値 */
#define INIT_PFR5	(0x00)		/* P5のポート機能レジスタの初期値 */
#define INIT_PFR6	(0x00)		/* P6のポート機能レジスタの初期値 */
#define INIT_PFR7	(0x00)		/* P7のポート機能レジスタの初期値 */
#define INIT_PFR8	(0x00)		/* P8のポート機能レジスタの初期値 */
#define INIT_PFR9	(0x20)		/* P9のポート機能レジスタの初期値 */
#define INIT_PFR10	(0x00)		/* P10のポート機能レジスタの初期値 */
#define INIT_PFR11	(0x01)		/* P11のポート機能レジスタの初期値 */
#define INIT_PFR12	(0x00)		/* P12のポート機能レジスタの初期値 */
#define INIT_PFR13	(0x00)		/* P13のポート機能レジスタの初期値 */

#define STOP_PFR0	(0x07)		/* P0のポート機能レジスタの停止値 */
#define STOP_PFR1	(0x00)		/* P1のポート機能レジスタの停止値 */
#define STOP_PFR2	(0x80)		/* P2のポート機能レジスタの停止値 */
#define STOP_PFR3	(0x64)		/* P3のポート機能レジスタの停止値 */
#define STOP_PFR4	(0x01)		/* P4のポート機能レジスタの停止値 */
#define STOP_PFR5	(0x00)		/* P5のポート機能レジスタの停止値 */
#define STOP_PFR6	(0x00)		/* P6のポート機能レジスタの停止値 */
#define STOP_PFR7	(0x00)		/* P7のポート機能レジスタの停止値 */
#define STOP_PFR8	(0x00)		/* P8のポート機能レジスタの停止値 */
#define STOP_PFR9	(0x20)		/* P9のポート機能レジスタの停止値 */
#define STOP_PFR10	(0x00)		/* P10のポート機能レジスタの停止値 */
#define STOP_PFR11	(0x01)		/* P11のポート機能レジスタの停止値 */
#define STOP_PFR12	(0x00)		/* P12のポート機能レジスタの停止値 */
#define STOP_PFR13	(0x00)		/* P13のポート機能レジスタの停止値 */

/* 拡張ポート機能レジスタ */
#define INIT_EPFR0	(0x00)		/* P0の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR1	(0x00)		/* P1の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR2	(0xE0)		/* P2の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR3	(0xE0)		/* P3の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR4	(0xE0)		/* P4の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR5	(0xE0)		/* P5の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR6	(0xE0)		/* P6の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR7	(0xE0)		/* P7の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR8	(0xE0)		/* P8の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR9	(0xE0)		/* P9の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR10	(0x89)		/* P10の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR11	(0xC1)		/* P11の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR12	(0xC0)		/* P12の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR13	(0xC0)		/* P13の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR14	(0xC0)		/* P14の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR15	(0x80)		/* P15の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR16	(0x00)		/* P16の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR17	(0x00)		/* P17の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR18	(0x00)		/* P18の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR19	(0x00)		/* P19の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR20	(0x00)		/* P20の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR21	(0x00)		/* P21の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR22	(0x00)		/* P22の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR23	(0x00)		/* P23の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR24	(0xFB)		/* P24の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR25	(0xF8)		/* P25の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR26	(0xF0)		/* P26の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR27	(0xE0)		/* P27の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR28	(0xF0)		/* P28の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR29	(0x00)		/* P29の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR30	(0x00)		/* P30の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR31	(0x00)		/* P31の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR32	(0x00)		/* P32の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR33	(0xF5)		/* P33の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR34	(0xEC)		/* P34の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR35	(0xE0)		/* P35の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR36	(0xE0)		/* P36の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR37	(0x24)		/* P37の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR38	(0xE8)		/* P38の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR39	(0x00)		/* P39の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR40	(0xC0)		/* P40の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR41	(0xF8)		/* P41の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR42	(0xFC)		/* P42の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR43	(0x00)		/* P43の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR44	(0x10)		/* P44の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR45	(0x00)		/* P45の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR46	(0xC0)		/* P46の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR47	(0xFE)		/* P47の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR52	(0xFE)		/* P52の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR53	(0xE0)		/* P53の拡張ポート機能レジスタの初期値 */
#define INIT_EPFR54	(0xF0)		/* P54の拡張ポート機能レジスタの初期値 */

#define STOP_EPFR0	(0x00)		/* P0の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR1	(0x00)		/* P1の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR2	(0xE0)		/* P2の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR3	(0xE0)		/* P3の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR4	(0xE0)		/* P4の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR5	(0xE0)		/* P5の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR6	(0xE0)		/* P6の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR7	(0xE0)		/* P7の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR8	(0xE0)		/* P8の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR9	(0xE0)		/* P9の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR10	(0x80)		/* P10の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR11	(0xC0)		/* P11の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR12	(0xC0)		/* P12の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR13	(0xC0)		/* P13の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR14	(0xC0)		/* P14の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR15	(0x80)		/* P15の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR16	(0x00)		/* P16の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR17	(0x00)		/* P17の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR18	(0x00)		/* P18の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR19	(0x00)		/* P19の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR20	(0x00)		/* P20の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR21	(0x00)		/* P21の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR22	(0x00)		/* P22の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR23	(0x00)		/* P23の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR24	(0xF8)		/* P24の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR25	(0xF8)		/* P25の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR26	(0xF0)		/* P26の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR27	(0xE0)		/* P27の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR28	(0xF0)		/* P28の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR29	(0x00)		/* P29の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR30	(0x00)		/* P30の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR31	(0x00)		/* P31の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR32	(0x00)		/* P32の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR33	(0xE0)		/* P33の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR34	(0xE0)		/* P34の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR35	(0xE0)		/* P35の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR36	(0xE0)		/* P36の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR37	(0x00)		/* P37の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR38	(0xE0)		/* P38の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR39	(0x00)		/* P39の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR40	(0xC0)		/* P40の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR41	(0xF8)		/* P41の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR42	(0xFC)		/* P42の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR43	(0x00)		/* P43の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR44	(0x10)		/* P44の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR45	(0x00)		/* P45の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR46	(0xC0)		/* P46の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR47	(0xFE)		/* P47の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR52	(0xFE)		/* P52の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR53	(0xE0)		/* P53の拡張ポート機能レジスタの停止値 */
#define STOP_EPFR54	(0xF0)		/* P54の拡張ポート機能レジスタの停止値 */

/* プルアップダウン制御レジスタ */
#define INIT_PPCR0	(0xFF)		/* P0のプルアップダウン制御レジスタの初期値 */
#define INIT_PPCR1	(0xFF)		/* P1のプルアップダウン制御レジスタの初期値 */
#define INIT_PPCR2	(0xFF)		/* P2のプルアップダウン制御レジスタの初期値 */
#define INIT_PPCR3	(0xFF)		/* P3のプルアップダウン制御レジスタの初期値 */
#define INIT_PPCR4	(0xFF)		/* P4のプルアップダウン制御レジスタの初期値 */
#define INIT_PPCR5	(0xFF)		/* P5のプルアップダウン制御レジスタの初期値 */
#define INIT_PPCR6	(0xFF)		/* P6のプルアップダウン制御レジスタの初期値 */
#define INIT_PPCR7	(0xFF)		/* P7のプルアップダウン制御レジスタの初期値 */
#define INIT_PPCR8	(0xFF)		/* P8のプルアップダウン制御レジスタの初期値 */
#define INIT_PPCR9	(0xFF)		/* P9のプルアップダウン制御レジスタの初期値 */
#define INIT_PPCR10	(0xFF)		/* P10のプルアップダウン制御レジスタの初期値 */
#define INIT_PPCR11	(0xFF)		/* P11のプルアップダウン制御レジスタの初期値 */
#define INIT_PPCR12	(0xFF)		/* P12のプルアップダウン制御レジスタの初期値 */
#define INIT_PPCR13	(0xFF)		/* P13のプルアップダウン制御レジスタの初期値 */

#define STOP_PPCR0	(0xFF)		/* P0のプルアップダウン制御レジスタの停止値 */
#define STOP_PPCR1	(0xFF)		/* P1のプルアップダウン制御レジスタの停止値 */
#define STOP_PPCR2	(0xFF)		/* P2のプルアップダウン制御レジスタの停止値 */
#define STOP_PPCR3	(0xFF)		/* P3のプルアップダウン制御レジスタの停止値 */
#define STOP_PPCR4	(0xFF)		/* P4のプルアップダウン制御レジスタの停止値 */
#define STOP_PPCR5	(0xFF)		/* P5のプルアップダウン制御レジスタの停止値 */
#define STOP_PPCR6	(0xFF)		/* P6のプルアップダウン制御レジスタの停止値 */
#define STOP_PPCR7	(0xFF)		/* P7のプルアップダウン制御レジスタの停止値 */
#define STOP_PPCR8	(0xFF)		/* P8のプルアップダウン制御レジスタの停止値 */
#define STOP_PPCR9	(0xFF)		/* P9のプルアップダウン制御レジスタの停止値 */
#define STOP_PPCR10	(0xFF)		/* P10のプルアップダウン制御レジスタの停止値 */
#define STOP_PPCR11	(0xFF)		/* P11のプルアップダウン制御レジスタの停止値 */
#define STOP_PPCR12	(0xFF)		/* P12のプルアップダウン制御レジスタの停止値 */
#define STOP_PPCR13	(0xFF)		/* P13のプルアップダウン制御レジスタの停止値 */

/* プルアップダウン許可レジスタ */
#define INIT_PPER0	(0x00)		/* P0のプルアップダウン許可レジスタの初期値 */
#define INIT_PPER1	(0x00)		/* P1のプルアップダウン許可レジスタの初期値 */
#define INIT_PPER2	(0x00)		/* P2のプルアップダウン許可レジスタの初期値 */
#define INIT_PPER3	(0x00)		/* P3のプルアップダウン許可レジスタの初期値 */
#define INIT_PPER4	(0x00)		/* P4のプルアップダウン許可レジスタの初期値 */
#define INIT_PPER5	(0x00)		/* P5のプルアップダウン許可レジスタの初期値 */
#define INIT_PPER6	(0x00)		/* P6のプルアップダウン許可レジスタの初期値 */
#define INIT_PPER7	(0x00)		/* P7のプルアップダウン許可レジスタの初期値 */
#define INIT_PPER8	(0x00)		/* P8のプルアップダウン許可レジスタの初期値 */
#define INIT_PPER9	(0x00)		/* P9のプルアップダウン許可レジスタの初期値 */
#define INIT_PPER10	(0x00)		/* P10のプルアップダウン許可レジスタの初期値 */
#define INIT_PPER11	(0x00)		/* P11のプルアップダウン許可レジスタの初期値 */
#define INIT_PPER12	(0x00)		/* P12のプルアップダウン許可レジスタの初期値 */
#define INIT_PPER13	(0x00)		/* P13のプルアップダウン許可レジスタの初期値 */

#define STOP_PPER0	(0x00)		/* P0のプルアップダウン許可レジスタの停止値 */
#define STOP_PPER1	(0x00)		/* P1のプルアップダウン許可レジスタの停止値 */
#define STOP_PPER2	(0x00)		/* P2のプルアップダウン許可レジスタの停止値 */
#define STOP_PPER3	(0x00)		/* P3のプルアップダウン許可レジスタの停止値 */
#define STOP_PPER4	(0x00)		/* P4のプルアップダウン許可レジスタの停止値 */
#define STOP_PPER5	(0x00)		/* P5のプルアップダウン許可レジスタの停止値 */
#define STOP_PPER6	(0x00)		/* P6のプルアップダウン許可レジスタの停止値 */
#define STOP_PPER7	(0x00)		/* P7のプルアップダウン許可レジスタの停止値 */
#define STOP_PPER8	(0x00)		/* P8のプルアップダウン許可レジスタの停止値 */
#define STOP_PPER9	(0x00)		/* P9のプルアップダウン許可レジスタの停止値 */
#define STOP_PPER10	(0x00)		/* P10のプルアップダウン許可レジスタの停止値 */
#define STOP_PPER11	(0x00)		/* P11のプルアップダウン許可レジスタの停止値 */
#define STOP_PPER12	(0x00)		/* P12のプルアップダウン許可レジスタの停止値 */
#define STOP_PPER13	(0x00)		/* P13のプルアップダウン許可レジスタの停止値 */

/* ポート入力レベル選択レジスタ */
#define INIT_PILR0	(0xFF)		/* P0のポート入力レベル選択レジスタの初期値 */
#define INIT_PILR1	(0xFF)		/* P1のポート入力レベル選択レジスタの初期値 */
#define INIT_PILR2	(0xFF)		/* P2のポート入力レベル選択レジスタの初期値 */
#define INIT_PILR3	(0xFF)		/* P3のポート入力レベル選択レジスタの初期値 */
#define INIT_PILR4	(0xFF)		/* P4のポート入力レベル選択レジスタの初期値 */
#define INIT_PILR5	(0xFF)		/* P5のポート入力レベル選択レジスタの初期値 */
#define INIT_PILR6	(0xFF)		/* P6のポート入力レベル選択レジスタの初期値 */
#define INIT_PILR7	(0xFF)		/* P7のポート入力レベル選択レジスタの初期値 */
#define INIT_PILR8	(0xFF)		/* P8のポート入力レベル選択レジスタの初期値 */
#define INIT_PILR9	(0xFF)		/* P9のポート入力レベル選択レジスタの初期値 */
#define INIT_PILR10	(0xFF)		/* P10のポート入力レベル選択レジスタの初期値 */
#define INIT_PILR11	(0xFF)		/* P11のポート入力レベル選択レジスタの初期値 */
#define INIT_PILR12	(0xFF)		/* P12のポート入力レベル選択レジスタの初期値 */
#define INIT_PILR13	(0xFF)		/* P13のポート入力レベル選択レジスタの初期値 */

#define STOP_PILR0	(0xFF)		/* P0のポート入力レベル選択レジスタの停止値 */
#define STOP_PILR1	(0xFF)		/* P1のポート入力レベル選択レジスタの停止値 */
#define STOP_PILR2	(0xFF)		/* P2のポート入力レベル選択レジスタの停止値 */
#define STOP_PILR3	(0xFF)		/* P3のポート入力レベル選択レジスタの停止値 */
#define STOP_PILR4	(0xFF)		/* P4のポート入力レベル選択レジスタの停止値 */
#define STOP_PILR5	(0xFF)		/* P5のポート入力レベル選択レジスタの停止値 */
#define STOP_PILR6	(0xFF)		/* P6のポート入力レベル選択レジスタの停止値 */
#define STOP_PILR7	(0xFF)		/* P7のポート入力レベル選択レジスタの停止値 */
#define STOP_PILR8	(0xFF)		/* P8のポート入力レベル選択レジスタの停止値 */
#define STOP_PILR9	(0xFF)		/* P9のポート入力レベル選択レジスタの停止値 */
#define STOP_PILR10	(0xFF)		/* P10のポート入力レベル選択レジスタの停止値 */
#define STOP_PILR11	(0xFF)		/* P11のポート入力レベル選択レジスタの停止値 */
#define STOP_PILR12	(0xFF)		/* P12のポート入力レベル選択レジスタの停止値 */
#define STOP_PILR13	(0xFF)		/* P13のポート入力レベル選択レジスタの停止値 */

/* 拡張ポート入力レベル選択レジスタ */
#define INIT_EPILR0		(0x00)		/* P0の拡張ポート入力レベル選択レジスタの初期値 */
#define INIT_EPILR1		(0x00)		/* P1の拡張ポート入力レベル選択レジスタの初期値 */
#define INIT_EPILR2		(0x00)		/* P2の拡張ポート入力レベル選択レジスタの初期値 */
#define INIT_EPILR3		(0x00)		/* P3の拡張ポート入力レベル選択レジスタの初期値 */
#define INIT_EPILR4		(0x00)		/* P4の拡張ポート入力レベル選択レジスタの初期値 */
#define INIT_EPILR5		(0x00)		/* P5の拡張ポート入力レベル選択レジスタの初期値 */
#define INIT_EPILR6		(0x00)		/* P6の拡張ポート入力レベル選択レジスタの初期値 */
#define INIT_EPILR7		(0x00)		/* P7の拡張ポート入力レベル選択レジスタの初期値 */
#define INIT_EPILR8		(0x00)		/* P8の拡張ポート入力レベル選択レジスタの初期値 */
#define INIT_EPILR9		(0x00)		/* P9の拡張ポート入力レベル選択レジスタの初期値 */
#define INIT_EPILR10	(0x00)		/* P10の拡張ポート入力レベル選択レジスタの初期値 */
#define INIT_EPILR11	(0x00)		/* P11の拡張ポート入力レベル選択レジスタの初期値 */
#define INIT_EPILR12	(0x00)		/* P12の拡張ポート入力レベル選択レジスタの初期値 */
#define INIT_EPILR13	(0x00)		/* P13の拡張ポート入力レベル選択レジスタの初期値 */

#define STOP_EPILR0		(0x00)		/* P0の拡張ポート入力レベル選択レジスタの停止値 */
#define STOP_EPILR1		(0x00)		/* P1の拡張ポート入力レベル選択レジスタの停止値 */
#define STOP_EPILR2		(0x00)		/* P2の拡張ポート入力レベル選択レジスタの停止値 */
#define STOP_EPILR3		(0x00)		/* P3の拡張ポート入力レベル選択レジスタの停止値 */
#define STOP_EPILR4		(0x00)		/* P4の拡張ポート入力レベル選択レジスタの停止値 */
#define STOP_EPILR5		(0x00)		/* P5の拡張ポート入力レベル選択レジスタの停止値 */
#define STOP_EPILR6		(0x00)		/* P6の拡張ポート入力レベル選択レジスタの停止値 */
#define STOP_EPILR7		(0x00)		/* P7の拡張ポート入力レベル選択レジスタの停止値 */
#define STOP_EPILR8		(0x00)		/* P8の拡張ポート入力レベル選択レジスタの停止値 */
#define STOP_EPILR9		(0x00)		/* P9の拡張ポート入力レベル選択レジスタの停止値 */
#define STOP_EPILR10	(0x00)		/* P10の拡張ポート入力レベル選択レジスタの停止値 */
#define STOP_EPILR11	(0x00)		/* P11の拡張ポート入力レベル選択レジスタの停止値 */
#define STOP_EPILR12	(0x00)		/* P12の拡張ポート入力レベル選択レジスタの停止値 */
#define STOP_EPILR13	(0x00)		/* P13の拡張ポート入力レベル選択レジスタの停止値 */

/* ポート出力駆動レジスタ */
#define INIT_PODR0		(0xFF)		/* P0のポート出力駆動レジスタの初期値 */
#define INIT_PODR1		(0xFF)		/* P1のポート出力駆動レジスタの初期値 */
#define INIT_PODR2		(0xFF)		/* P2のポート出力駆動レジスタの初期値 */
#define INIT_PODR3		(0xFF)		/* P3のポート出力駆動レジスタの初期値 */
#define INIT_PODR4		(0xFF)		/* P4のポート出力駆動レジスタの初期値 */
#define INIT_PODR5		(0xFF)		/* P5のポート出力駆動レジスタの初期値 */
#define INIT_PODR6		(0xFF)		/* P6のポート出力駆動レジスタの初期値 */
#define INIT_PODR7		(0xFF)		/* P7のポート出力駆動レジスタの初期値 */
#define INIT_PODR8		(0xFF)		/* P8のポート出力駆動レジスタの初期値 */
#define INIT_PODR9		(0xFF)		/* P9のポート出力駆動レジスタの初期値 */
#define INIT_PODR10		(0xFF)		/* P10のポート出力駆動レジスタの初期値 */
#define INIT_PODR11		(0xFF)		/* P11のポート出力駆動レジスタの初期値 */
#define INIT_PODR12		(0xFF)		/* P12のポート出力駆動レジスタの初期値 */
#define INIT_PODR13		(0xFF)		/* P13のポート出力駆動レジスタの初期値 */

#define STOP_PODR0		(0xFF)		/* P0のポート出力駆動レジスタの停止値 */
#define STOP_PODR1		(0xFF)		/* P1のポート出力駆動レジスタの停止値 */
#define STOP_PODR2		(0xFF)		/* P2のポート出力駆動レジスタの停止値 */
#define STOP_PODR3		(0xFF)		/* P3のポート出力駆動レジスタの停止値 */
#define STOP_PODR4		(0xFF)		/* P4のポート出力駆動レジスタの停止値 */
#define STOP_PODR5		(0xFF)		/* P5のポート出力駆動レジスタの停止値 */
#define STOP_PODR6		(0xFF)		/* P6のポート出力駆動レジスタの停止値 */
#define STOP_PODR7		(0xFF)		/* P7のポート出力駆動レジスタの停止値 */
#define STOP_PODR8		(0xFF)		/* P8のポート出力駆動レジスタの停止値 */
#define STOP_PODR9		(0xFF)		/* P9のポート出力駆動レジスタの停止値 */
#define STOP_PODR10		(0xFF)		/* P10のポート出力駆動レジスタの停止値 */
#define STOP_PODR11		(0xFF)		/* P11のポート出力駆動レジスタの停止値 */
#define STOP_PODR12		(0xFF)		/* P12のポート出力駆動レジスタの停止値 */
#define STOP_PODR13		(0xFF)		/* P13のポート出力駆動レジスタの停止値 */

/* 拡張ポート出力駆動レジスタ */
#define INIT_EPODR1		(0x00)		/* P1の拡張ポート出力駆動レジスタの初期値 */
#define INIT_EPODR2		(0x00)		/* P2の拡張ポート出力駆動レジスタの初期値 */
#define INIT_EPODR3		(0x00)		/* P3の拡張ポート出力駆動レジスタの初期値 */
#define INIT_EPODR6		(0x00)		/* P6の拡張ポート出力駆動レジスタの初期値 */
#define INIT_EPODR7		(0x00)		/* P7の拡張ポート出力駆動レジスタの初期値 */
#define INIT_EPODR8		(0x00)		/* P8の拡張ポート出力駆動レジスタの初期値 */

#define STOP_EPODR1		(0x00)		/* P1の拡張ポート出力駆動レジスタの停止値 */
#define STOP_EPODR2		(0x00)		/* P2の拡張ポート出力駆動レジスタの停止値 */
#define STOP_EPODR3		(0x00)		/* P3の拡張ポート出力駆動レジスタの停止値 */
#define STOP_EPODR6		(0x00)		/* P6の拡張ポート出力駆動レジスタの停止値 */
#define STOP_EPODR7		(0x00)		/* P7の拡張ポート出力駆動レジスタの停止値 */
#define STOP_EPODR8		(0x00)		/* P8の拡張ポート出力駆動レジスタの停止値 */

/* ポート入力許可レジスタ */
#define INIT_PORTEN		(0x01)		/* ポート入力許可レジスタの初期値 */

#define STOP_PORTEN		(0x01)		/* ポート入力許可レジスタの停止値 */


#endif								/* _MB91570_PORT_DRV_INTERNAL__ */


/* 外部公開定数 */
/*==============================================================================*/
/*	enum定義（外部非公開：アプリ側で使用する場合、外部公開用に再度定義すること	*/
/*==============================================================================*/
/* ポートデータレジスタ番号 */
typedef enum {
	PORTDRV_PORT_0 = 0,			/* P0のポートデータレジスタ番号 */
	PORTDRV_PORT_1,				/* P1のポートデータレジスタ番号 */
	PORTDRV_PORT_2,				/* P2のポートデータレジスタ番号 */
	PORTDRV_PORT_3,				/* P3のポートデータレジスタ番号 */
	PORTDRV_PORT_4,				/* P4のポートデータレジスタ番号 */
	PORTDRV_PORT_5,				/* P5のポートデータレジスタ番号 */
	PORTDRV_PORT_6,				/* P6のポートデータレジスタ番号 */
	PORTDRV_PORT_7,				/* P7のポートデータレジスタ番号 */
	PORTDRV_PORT_8,				/* P8のポートデータレジスタ番号 */
	PORTDRV_PORT_9,				/* P9のポートデータレジスタ番号 */
	PORTDRV_PORT_10,			/* P10のポートデータレジスタ番号 */
	PORTDRV_PORT_11,			/* P11のポートデータレジスタ番号 */
	PORTDRV_PORT_12,			/* P12のポートデータレジスタ番号 */
	PORTDRV_PORT_13,			/* P13のポートデータレジスタ番号 */
	PORTDRV_PORT_MAX			/* ポートデータレジスタ番号MAX値 */
} E_PORTDRV_PORT_NUM;

/* ポートデータレジスタのBIT番号 */
typedef enum {
	PORTDRV_BIT_0 = 0,			/* ポートデータレジスタのBIT番号0 */
	PORTDRV_BIT_1,				/* ポートデータレジスタのBIT番号1 */
	PORTDRV_BIT_2,				/* ポートデータレジスタのBIT番号2 */
	PORTDRV_BIT_3,				/* ポートデータレジスタのBIT番号3 */
	PORTDRV_BIT_4,				/* ポートデータレジスタのBIT番号4 */
	PORTDRV_BIT_5,				/* ポートデータレジスタのBIT番号5 */
	PORTDRV_BIT_6,				/* ポートデータレジスタのBIT番号6 */
	PORTDRV_BIT_7,				/* ポートデータレジスタのBIT番号7 */
	PORTDRV_BIT_MAX				/* ポートデータレジスタのBIT番号MAX値 */
} E_PORTDRV_BIT_NUM;

/* ポートデータレジスタのBYTE番号 */
typedef enum {
	PORTDRV_BYTE_0 = 0,			/* ポートデータレジスタのBYTE番号0 */
	PORTDRV_BYTE_1,				/* ポートデータレジスタのBYTE番号1 */
	PORTDRV_BYTE_2,				/* ポートデータレジスタのBYTE番号2 */
	PORTDRV_BYTE_3,				/* ポートデータレジスタのBYTE番号3 */
	PORTDRV_BYTE_4,				/* ポートデータレジスタのBYTE番号4 */
	PORTDRV_BYTE_5,				/* ポートデータレジスタのBYTE番号5 */
	PORTDRV_BYTE_6,				/* ポートデータレジスタのBYTE番号6 */
	PORTDRV_BYTE_7,				/* ポートデータレジスタのBYTE番号7 */
	PORTDRV_BYTE_MAX			/* ポートデータレジスタのBYTE番号MAX値 */
} E_PORTDRV_BYTE_NUM;

/* ポート機能切り替え用ID */
typedef enum {
	PORT_IN = 0,				/* 汎用入力ポート */
	PORT_OUT,					/* 汎用出力ポート */
	BUS_DATA_16BIT_IN,			/* 外部バスデータ入力 16bit */
	BUS_DATA_16BIT_OUT,			/* 外部バスデータ出力 16bit */
	LIN_UART_SERIAL_IN,			/* LIN_UART シリアル入力 */
	RELOAD_TIMER_EVENT_IN,		/* リロードタイマイベント入力 */
	PPG_OUT,					/* PPG 出力 */
	BUS_DATA_8BIT_IN,			/* 外部バスデータ入力 8bit */
	BUS_DATA_8BIT_OUT,			/* 外部バスデータ出力 8bit */
	INTTERUPT_REQUEST_IN,		/* 外部割込み要求入力 */
	BUS_DATA_32BIT_IN,			/* 外部バスデータ入力 32bit */
	BUS_DATA_32BIT_OUT,			/* 外部バスデータ出力 32bit */
	LCD_SEG_DUTY_OUT,			/* LCDC セグメント(Duty)出力 */
	BUS_ADDRESS_STROBE_OUT,		/* 外部バスアドレスストローブ出力 */
	ICU_IN,						/* インプットキャプチャ入力 */
	OCU_OUT,					/* アウトプットコンペア出力 */
	BUS_CHIPSET_OUT,			/* 外部バスチップセレクト 出力 */
	BUS_READ_STROBE_OUT,		/* 外部バスリードストローブ出力 */
	BUS_WRITE_STROBE_OUT,		/* 外部バスライトストローブ出力 */
	BUS_ADDRESS_OUT,			/* 外部バスアドレス出力 */
	MULTI_FUNCTION_SERIAL_IN,	/* マルチファンクションシリアル入力 */
	MULTI_FUNCTION_SERIAL_OUT,	/* マルチファンクションシリアル出力 */
	LCD_SEG_STATIC_OUT,			/* LCDC セグメント(Static)出力 */
	UPDOWN_COUNTER_AIN_IN,		/* アップダウンカウンタAIN 入力 */
	UPDOWN_COUNTER_BIN_IN,		/* アップダウンカウンタBIN 入力 */
	SOUND_GENERATOR_SGA_OUT,	/* サウンドジェネレータSGA 出力 */
	UPDOWN_COUNTER_ZIN_IN,		/* アップダウンカウンタZIN 入力 */
	SOUND_GENERATOR_SGO_OUT,	/* サウンドジェネレータSGO 出力 */
	MULTI_FUNCTION_SCK_IN,		/* マルチファンクションシリアルクロック入力 */
	MULTI_FUNCTION_SCK_OUT,		/* マルチファンクションシリアルクロック出力 */
	LCD_SEG_DUTY_OUT_COM_OUT,	/* LCDC セグメント(Duty)コモン出力 */
	FREE_RUN_TIMCLK_IN,			/* フリーランタイマクロック入力 */
	LCD_V_IN,					/* LCDC 基準電圧入力 */
	BUS_RDY_IN,					/* 外部バスRDY 入力 */
	SMC_OUT,					/* SMC 出力 */
	LIN_UART_SCK_IN,			/* LIN_UART シリアルクロック入力 */
	LIN_UART_SCK_OUT,			/* LIN_UART シリアルクロック出力 */
	ADC_TRG_IN,					/* ADC 外部トリガ入力 */
	RELOAD_TIMER_OUT,			/* リロードタイマ出力 */
	CAN_TX_OUT,					/* CAN TX データ出力 */
	CAN_RX_IN,					/* CAN RX データ入力 */
	RTC_OVERFLOW_OUT,			/* RTC オーバフロー出力 */
	DAC_OUT,					/* DAC 出力 */
	PPG_TRG_IN,					/* PPG トリガ入力 */
	BASE_TIMER_IN,				/* ベースタイマ入力 */
	SUB_CLK_OSC_OUT,			/* サブクロックOSC出力 */
	SUB_CLK_OSC_IN,				/* サブクロックOSC 入力 */
	LIN_UART_SERIAL_OUT,		/* LIN_UART シリアル出力 */
	BUS_CLK_OUT,				/* 外部バスクロック出力 */
	ADC_ANALOG_IN,				/* ADC アナログ入力 */
	BASE_TIMER_OUT,				/* ベースタイマ出力 */
	NO_USE,						/* 不使用 */
	PORT_FUNC_CHANGE_ID_MAX		/* MAX値 */
} E_PORT_FUNC_CHANGE_ID;

/*==============================================================================*/
/*	define定義（外部非公開：外部での使用禁止）									*/
/*==============================================================================*/
#define PORT_DRV_FUNC_CHANGE	1	/* ROM容量、処理時間削減のためのコンフィグ(対象関数はPortDrv_SetFuncBit) */
									/* 0:対象関数と対象関数のみ使用しているROMテーブルがコンパイルされない */
									/* 1:対象関数と対象関数のみ使用しているROMテーブルがコンパイルされる */

/*==============================================================================*/
/*	define定義（外部公開）														*/
/*==============================================================================*/
/* ビットマスク位置 */
#define PORT_BIT_MASK_0		(0x00u)	/* P0のビットマスク位置		*/
#define PORT_BIT_MASK_1		(0x00u)	/* P1のビットマスク位置		*/
#define PORT_BIT_MASK_2		(0x00u)	/* P2のビットマスク位置		*/
#define PORT_BIT_MASK_3		(0x00u)	/* P3のビットマスク位置		*/
#define PORT_BIT_MASK_4		(0x00u)	/* P4のビットマスク位置		*/
#define PORT_BIT_MASK_5		(0x00u)	/* P5のビットマスク位置		*/
#define PORT_BIT_MASK_6		(0x00u)	/* P6のビットマスク位置		*/
#define PORT_BIT_MASK_7		(0x00u)	/* P7のビットマスク位置		*/
#define PORT_BIT_MASK_8		(0x00u)	/* P8のビットマスク位置		*/
#define PORT_BIT_MASK_9		(0x01u)	/* P9のビットマスク位置(ADC5V EN)	*/
#define PORT_BIT_MASK_10	(0x00u)	/* P10のビットマスク位置	*/
#define PORT_BIT_MASK_11	(0x00u)	/* P11のビットマスク位置	*/
#define PORT_BIT_MASK_12	(0x60u)	/* P12のビットマスク位置(3.3V EN, 電源自己保持)	*/
#define PORT_BIT_MASK_13	(0x00u)	/* P13のビットマスク位置	*/

#define		R_PO_EEPMW_DI					PDR10_P5			/* EEPROM DI出力latch */
#define		R_PO_EEPMW_SK					PDR10_P6			/* EEPROM SK出力latch */ 
#define		R_PO_EEPMW_CS					PDR10_P7			/* EEPROM CS出力latch */
#define		R_PI_EEPMW_DO					PDR10_P4			/* EEPROM DO入力latch */

#define 	IO_PO_GR_BT_RST					PORTDRV_PORT_2		/* BTモジュールリセット端子 ポートグループ */
#define 	IO_PO_BIT_BT_RST				PORTDRV_BIT_1		/* BTモジュールリセット端子 ポートビット番号 */
#define		IO_PO_LEV_NONACT_BT_RST			(0x00)				/* BTモジュールリセット端子 NonActiveレベル(Low) */
#define		IO_PO_LEV_ACTIVE_BT_RST			(0x01)				/* BTモジュールリセット端子 Activeレベル(High) */

#define 	IO_PO_GR_GNSS_RST				PORTDRV_PORT_2		/* GNSSモジュールリセット端子 ポートグループ */
#define 	IO_PO_BIT_GNSS_RST				PORTDRV_BIT_2		/* GNSSモジュールリセット端子 ポートビット番号 */
#define		IO_PO_LEV_NONACT_GNSS_RST		(0x00)				/* GNSSモジュールリセット端子 NonActiveレベル(Low) */
#define		IO_PO_LEV_ACTIVE_GNSS_RST		(0x01)				/* GNSSモジュールリセット端子 Activeレベル(High) */

#define 	IO_PI_GR_SD_WP					PORTDRV_PORT_2		/* SDカードライトプロテクト端子 ポートグループ */
#define 	IO_PI_BIT_SD_WP					PORTDRV_BIT_3		/* SDカードライトプロテクト端子 ポートビット番号 */
#define		IO_PI_LEV_NONACT_SD_WP			(0x01)				/* SDカードライトプロテクト端子 NonActiveレベル(High) */
#define		IO_PI_LEV_ACTIVE_SD_WP			(0x00)				/* SDカードライトプロテクト端子 Activeレベル(Low) */

#define 	IO_PI_GR_SD_DTCT				PORTDRV_PORT_2		/* SDカードディテクト端子 ポートグループ */
#define 	IO_PI_BIT_SD_DTCT				PORTDRV_BIT_4		/* SDカードディテクト端子 ポートビット番号 */
#define		IO_PI_LEV_NONACT_SD_DTCT		(0x01)				/* SDカードディテクト端子 NonActiveレベル(High) */
#define		IO_PI_LEV_ACTIVE_SD_DTCT		(0x00)				/* SDカードディテクト端子 Activeレベル(Low) */

#define 	IO_PO_GR_SD_SPI_CS				PORTDRV_PORT_2		/* SDカード SPIチップセレクト端子 ポートグループ */
#define 	IO_PO_BIT_SD_SPI_CS				PORTDRV_BIT_5		/* SDカード SPIチップセレクト端子 ポートビット番号 */
#define		IO_PO_LEV_NONACT_SD_SPI_CS		(0x01)				/* SDカード SPIチップセレクト端子 NonActiveレベル(High) */
#define		IO_PO_LEV_ACTIVE_SD_SPI_CS		(0x00)				/* SDカード SPIチップセレクト端子 Activeレベル(Low) */

#define 	IO_PI_GR_ADVANCE_VIGN			PORTDRV_PORT_4		/* ADVANCE VIGN検出端子 ポートグループ */
#define 	IO_PI_BIT_ADVANCE_VIGN			PORTDRV_BIT_2		/* ADVANCE VIGN検出端子 ポートビット番号 */
#define		IO_PI_LEV_NONACT_ADVANCE_VIGN	(0x01)				/* ADVANCE VIGN検出端子 NonActiveレベル(High) */
#define		IO_PI_LEV_ACTIVE_ADVANCE_VIGN	(0x00)				/* ADVANCE VIGN検出端子 Activeレベル(Low) */

#define 	IO_PO_GR_ADC5V_EN				PORTDRV_PORT_9		/* ADC5V EN端子 ポートグループ */
#define 	IO_PO_BIT_ADC5V_EN				PORTDRV_BIT_0		/* ADC5V EN端子 ポートビット番号 */
#define		IO_PO_LEV_NONACT_ADC5V_EN		(0x01)				/* ADC5V EN端子 NonActiveレベル(High) */
#define		IO_PO_LEV_ACTIVE_ADC5V_EN		(0x00)				/* ADC5V EN端子 Activeレベル(Low) */

#define 	IO_PI_GR_MODEL_SW1				PORTDRV_PORT_9		/* 機種切替入力1端子 ポートグループ */
#define 	IO_PI_BIT_MODEL_SW1				PORTDRV_BIT_4		/* 機種切替入力1端子 ポートビット番号 */
#define 	IO_PI_GR_CAN_SW					PORTDRV_PORT_9		/* CAN切替入力端子 ポートグループ */
#define 	IO_PI_BIT_CAN_SW				PORTDRV_BIT_3		/* CAN切替入力端子 ポートビット番号 */
#define		IO_PI_LEV_CAN_SW_ISOCAN			(0x00)				/* CAN切替入力端子 レベルLow(ISO-CAN) */
#define		IO_PI_LEV_CAN_SW_VEHICLECAN		(0x01)				/* CAN切替入力端子 レベルHigh(VEHICLE-CAN) */

#define 	IO_PO_GR_LOCAL_CAN_STB			PORTDRV_PORT_11		/* LOCAL CAN STBモード出力端子 ポートグループ */
#define 	IO_PO_BIT_LOCAL_CAN_STB			PORTDRV_BIT_2		/* LOCAL CAN STBモード出力端子 ポートビット番号 */
#define		IO_PO_LEV_NONACT_LOCAL_CAN_STB	(0x00)				/* LOCAL CAN STBモード出力端子 NonActiveレベル(Low) */
#define		IO_PO_LEV_ACTIVE_LOCAL_CAN_STB	(0x01)				/* LOCAL CAN STBモード出力端子 Activeレベル(High) */

#define 	IO_PO_GR_CAN_STB				PORTDRV_PORT_12		/* 車両CAN STBモード出力端子 ポートグループ */
#define 	IO_PO_BIT_CAN_STB				PORTDRV_BIT_4		/* 車両CAN STBモード出力端子 ポートビット番号 */
#define		IO_PO_LEV_NONACT_CAN_STB		(0x00)				/* 車両CAN STBモード出力端子 NonActiveレベル(Low) */
#define		IO_PO_LEV_ACTIVE_CAN_STB		(0x01)				/* 車両CAN STBモード出力端子 Activeレベル(High) */

#define 	IO_PO_GR_5V_HOLD				PORTDRV_PORT_12		/* 電源自己保持出力端子 ポートグループ */
#define 	IO_PO_BIT_5V_HOLD				PORTDRV_BIT_5		/* 電源自己保持出力端子 ポートビット番号 */
#define		IO_PO_LEV_NONACT_5V_HOLD		(0x00)				/* 電源自己保持出力端子 NonActiveレベル(Low) */
#define		IO_PO_LEV_ACTIVE_5V_HOLD		(0x01)				/* 電源自己保持出力端子 Activeレベル(High) */

#define 	IO_PO_GR_33V_EN					PORTDRV_PORT_12		/* 3.3V EN端子 ポートグループ */
#define 	IO_PO_BIT_33V_EN				PORTDRV_BIT_6		/* 3.3V EN端子 ポートビット番号 */
#define		IO_PO_LEV_NONACT_33V_EN			(0x00)				/* 3.3V EN端子 NonActiveレベル(Low) */
#define		IO_PO_LEV_ACTIVE_33V_EN			(0x01)				/* 3.3V EN端子 Activeレベル(High) */

#define 	IO_PI_GR_ACC_STAT				PORTDRV_PORT_13		/* ACC状態入力端子 ポートグループ */
#define 	IO_PI_BIT_ACC_STAT				PORTDRV_BIT_3		/* ACC状態入力端子 ポートビット番号 */
#define		IO_PI_LEV_NONACT_ACC_STAT		(0x01)				/* ACC状態入力端子 NonActiveレベル(High) */
#define		IO_PI_LEV_ACTIVE_ACC_STAT		(0x00)				/* ACC状態入力端子 Activeレベル(Low) */

#define 	IO_PO_GR_DEFI_CAN_CTRL			PORTDRV_PORT_13		/* Defi-CAN CTRL出力端子 ポートグループ */
#define 	IO_PO_BIT_DEFI_CAN_CTRL			PORTDRV_BIT_4		/* Defi-CAN CTRL出力端子 ポートビット番号 */
#define		IO_PO_LEV_NONACT_DEFI_CAN_CTRL	(0x00)				/* Defi-CAN CTRL出力端子 NonActiveレベル(Low) */
#define		IO_PO_LEV_ACTIVE_DEFI_CAN_CTRL	(0x01)				/* Defi-CAN CTRL出力端子 Activeレベル(High) */

#define 	IO_PO_GR_BLE_DTR						PORTDRV_PORT_1		/* BTモジュール BLE_DTR端子 ポートグループ */
#define 	IO_PO_BIT_BLE_DTR						PORTDRV_BIT_5		/* BTモジュール BLE_DTR端子 ポートビット番号 */
#define		IO_PO_LEV_BLE_DTR_LOW					(0x01)				/* BTモジュール BLE_DTR端子 Low */
#define		IO_PO_LEV_BLE_DTR_HIGH					(0x00)				/* BTモジュール BLE_DTR端子 High */
//#define		IO_PO_LEV_BLE_DTR_LOW					(0x00)				/* BTモジュール BLE_DTR端子 Low */
//#define		IO_PO_LEV_BLE_DTR_HIGH					(0x01)				/* BTモジュール BLE_DTR端子 High */

#define 	IO_PO_GR_BLE_DSR						PORTDRV_PORT_2		/* BTモジュール BLE_DSR端子 ポートグループ */
#define 	IO_PO_BIT_BLE_DSR						PORTDRV_BIT_0		/* BTモジュール BLE_DSR端子 ポートビット番号 */
#define		IO_PO_LEV_BLE_DSR_LOW					(0x01)				/* BTモジュール BLE_DSR端子 Low */
#define		IO_PO_LEV_BLE_DSR_HIGH					(0x00)				/* BTモジュール BLE_DSR端子 High */
//#define		IO_PO_LEV_BLE_DSR_LOW					(0x00)				/* BTモジュール BLE_DSR端子 Low */
//#define		IO_PO_LEV_BLE_DSR_HIGH					(0x01)				/* BTモジュール BLE_DSR端子 High */

#define 	IO_PO_GR_BLE_SW2						PORTDRV_PORT_1		/* BTモジュール BLE_SW2端子 ポートグループ */
#define 	IO_PO_BIT_BLE_SW2						PORTDRV_BIT_3		/* BTモジュール BLE_SW2端子 ポートビット番号 */
#define		IO_PO_LEV_BLE_SW2_LOW					(0x00)				/* BTモジュール BLE_SW2端子 Low */
#define		IO_PO_LEV_BLE_SW2_HIGH					(0x01)				/* BTモジュール BLE_SW2端子 High */

#define 	IO_PO_GR_BLE_SW1						PORTDRV_PORT_1		/* BTモジュール BLE_SW1端子 ポートグループ */
#define 	IO_PO_BIT_BLE_SW1						PORTDRV_BIT_4		/* BTモジュール BLE_SW1端子 ポートビット番号 */
#define		IO_PO_LEV_BLE_SW1_LOW					(0x00)				/* BTモジュール BLE_SW1端子 Low */
#define		IO_PO_LEV_BLE_SW1_HIGH					(0x01)				/* BTモジュール BLE_SW1端子 High */

#endif								/*	__MB91570_PORT_DRV_CONFIG_H__ 	*/

