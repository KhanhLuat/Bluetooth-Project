/*--------------------------------------------------------------------------*/
/*	LED�o�͐���																*/
/*--------------------------------------------------------------------------*/
/* �T�v																		*/
/* 				LED�̏o�͏�Ԃ����肷��										*/
/*																			*/
/*	�t�@�C���쐬���̃e�[�}:													*/
/*				ER330 �����A�_�v�^											*/
/*																			*/
/* �쐬			2010/09/24	�֓�											*/
/* 																			*/
/* ���l																		*/
/*				����														*/
/*																			*/
/*--------------------------------------------------------------------------*/
#ifndef	OUTPUT_CONTROL_LED_CTRL_H
#define	OUTPUT_CONTROL_LED_CTRL_H

/*--------------------------------------------------------------------------*/
/*	�O���}�N��																*/
/*--------------------------------------------------------------------------*/
/* 
 ** LED���
 */ 
typedef enum led_type
{
	LED_TYPE_BODY_IND	= 0,		/**< �{�̃C���W�P�[�^			*/
	LED_TYPE_SW_IND,				/**< SW�C���W�P�[�^				*/
	LED_TYPE_SW_BKLT,				/**< SW�o�b�N���C�g				*/
	LED_TYPE_NUM					/**< LED�ʒu��					*/
} LED_TYPE;

/* LED */
#define		LED_ON						(1)					/*  LED�_��(H) */
#define		LED_OFF						(0)					/*  LED����(L) */


/*--------------------------------------------------------------------------*/
/*	�\���̐錾																*/
/*--------------------------------------------------------------------------*/
typedef struct t_led_output {
															/* ���� */
	UI_8		Power;										/* �d��LED�iON/OFF) */
	UI_8		Duty;										/* Duty�����i���邳�w��j */
} T_LED_OUTPUT;


/*--------------------------------------------------------------------------*/
/*	�v���g�^�C�v�錾														*/
/*--------------------------------------------------------------------------*/
void Output_control_led_main(void);						/* LED�o�͏�Ԕ���	[�@�\�֐�] */
void Output_control_led_ram_init(void);					/* RAM������		[�@�\�֐�] */
T_LED_OUTPUT Get_output_led_status(LED_TYPE ledType);	/* LED��Ԃ̎擾	 */
#ifdef _VDV_BOOT
UI_16 output_control_GetLedChangeTime(LED_TYPE ledType);/* LED��ԕω��܂ł̎��Ԏ擾	 */
void output_control_NotifyEndFlashErase(UI_16 eraseTime);/* FLASH�������Ԓʒm */
#endif /* _VDV_BOOT */

#endif		/* OUTPUT_CONTROL_LED_CTRL_H */
