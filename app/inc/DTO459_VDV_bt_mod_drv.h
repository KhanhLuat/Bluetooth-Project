/*==================================================================================================*/
/* @file	bt_mod_drv.h																			*/
/* @brief	Bluetooth Module Driver																	*/
/*==================================================================================================*/
/* @par Copyright																					*/
/*	Copyright (c) 2014 by N.S. Ltd. All rights reserved.											*/
/*																									*/
/* @par File History																				*/
/*   | Date       | Summary                                  | Source Repository	 | Revision |	*/
/*   | ---------- | ---------------------------------------- | -------------------- | -------- |	*/
/*   | 2014-xx-xx | Created for XXXXXXX                      | N/A                  | N/A      |	*/
/*==================================================================================================*/

#ifndef __BT_MOD_DRV_H__
#define __BT_MOD_DRV_H__

/*==================================================================================================*/
/*	Include files																					*/
/*==================================================================================================*/

#include "type.h"

/*==================================================================================================*/
/*	Constant, Macro definitions																		*/
/*==================================================================================================*/
#define BT_MOD_DRV_RX_BUF	(1000)	/**< ��M�o�b�t�@�̃o�C�g��	*/
#define BT_MOD_DRV_TX_BUF	(512)	/**< ���M�o�b�t�@�̃o�C�g��	*/

/*==================================================================================================*/
/*	Structure, Union, Type definitions																*/
/*==================================================================================================*/

/**
 * ��������
 */
typedef enum e_bt_mod_dev_result
{
	BT_MOD_DRV_OK = 0,					/**< ����I��			*/
	BT_MOD_DRV_ERR_INVALIDARG,			/**< �����s��			*/
	BT_MOD_DRV_ERR_BUFFERFULL,			/**< �o�b�t�@�t��		*/
	BT_MOD_DRV_ERR_NOT_RECIEVE_DATA		/**< ��M�f�[�^�Ȃ�		*/
} E_BT_MOD_DRV_RESULT;

/*
 ** �{�[���[�g
 */
typedef enum e_bt_baudrate
{
	BT_BAUDRATE_115200_BPS = 0,			/**< 115200bps			*/
	BT_BAUDRATE_230400_BPS,				/**< 230400bps			*/
	BT_BAUDRATE_NUM						/**< �{�[���[�g��		*/
} E_BT_BAUDRATE;

/*
 ** �V�X�e����Ԏ�ʁi���́j
 */
typedef enum e_bt_sys_state_type_in
{
	BT_SYS_STATE_TYPE_IN_DSR = 0,		/**< UART_DSR							*/
	BT_SYS_STATE_TYPE_IN_NUM			/**< �V�X�e����Ԏ�ʁi���́j��			*/
} E_BT_SYS_STATE_TYPE_IN;

/*
 ** �V�X�e����Ԏ�ʁi�o�́j
 */
typedef enum e_bt_sys_state_type_out
{
	BT_SYS_STATE_TYPE_OUT_DTR = 0,		/**< UART_DTR							*/
	BT_SYS_STATE_TYPE_OUT_NUM			/**< �V�X�e����Ԏ�ʁi�o�́j��			*/
} E_BT_SYS_STATE_TYPE_OUT;


/*==================================================================================================*/
/*	Variable declarations																			*/
/*==================================================================================================*/

/*==================================================================================================*/
/*	Function prototypes																				*/
/*==================================================================================================*/
/** 
 * @brief	������
 *
 * @note	<ul>
 *			<li>Bluetooth���W���[���h���C�o�Ŏg�p����RAM�̏������������s���B
 *			<li>Bluetooth���W���[���h���C�o�Ŏg�p���郌�W�X�^�̐ݒ���s���B
 *			</ul>
 */ 
void Bt_mod_drv_Init( void );

/** 
 * @brief	BT���W���[���N��
 * 
 * @note	
 */ 
void Bt_mod_drv_StartBtModule( void );

/** 
 * @brief	BT���W���[����~
 * 
 * @note	
 */ 
void Bt_mod_drv_StopBtModule( void );

/** 
 * @brief	�ʐM�J�n
 * 
 * @note	Bluetooth���W���[���Ƃ�UART�ɂ�鑗��M���J�n����B
 */ 
void Bt_mod_drv_StartComm( void );

/** 
 * @brief	�ʐM��~
 * 
 * @note	Bluetooth���W���[���Ƃ�UART�ɂ�鑗��M���~����B
 */ 
void Bt_mod_drv_StopComm( void );

/** 
 * @brief	BT���W���[�����Z�b�g
 * 
 * @note	Bluetooth���W���[���ɑ΂��āA���Z�b�g��������B
 */ 
void Bt_mod_drv_Reset( void );

/** 
 * @brief	BT���W���[�����Z�b�gON
 * 
 * @note	Bluetooth���W���[���ɑ΂��āA���Z�b�gON��������B
 */ 
void Bt_mod_drv_ResetON( void );

/** 
 * @brief	BT���W���[�����Z�b�gOFF
 * 
 * @note	Bluetooth���W���[���ɑ΂��āA���Z�b�gOFF��������B
 */ 
void Bt_mod_drv_ResetOFF( void );

/** 
 * @brief	��M�f�[�^�擾
 * 
 * @param	buf		[out]	��M�����f�[�^���i�[����o�b�t�@�ւ̃|�C���^
 * @param	len		[out]	��M�����f�[�^�̃o�C�g��
 *
 * @return	���s����
 * @retval	BT_MOD_DRV_OK						����I��
 * @retval	BT_MOD_DRV_ERR_INVALIDARG			�����s��
 * @retval	BT_MOD_DRV_ERR_NOT_RECIEVE_DATA		��M�f�[�^�Ȃ�
 *
 * @note	��M�����f�[�^���擾����B
 */ 
E_BT_MOD_DRV_RESULT Bt_mod_drv_GetRxData(UI_8* buf, UI_16* len);

/** 
 * @brief	���M�f�[�^�ݒ�
 * 
 * @param	*buf	[out]	���M����f�[�^���i�[����Ă���o�b�t�@�ւ̃|�C���^
 * @param	len		[in]	���M����f�[�^�̃o�C�g��
 *
 * @return	���s����
 * @retval	BT_MOD_DRV_OK				����I��
 * @retval	BT_MOD_DRV_ERR_INVALIDARG	�����s��
 * @retval	BT_MOD_DRV_ERR_BUFFERFULL	�o�b�t�@�t��
 *
 * @note	���M����f�[�^��ݒ肷��B
 */ 
E_BT_MOD_DRV_RESULT Bt_mod_drv_SetTxData(const UI_8* buf, UI_16 len);

/** 
 * @brief	���M�p�o�b�t�@�󂫃T�C�Y�擾
 *
 * @return	���M�p�o�b�t�@�󂫃T�C�Y
 * @note	<ul>
 *			<li>���M�p�o�b�t�@�̋󂫃T�C�Y���擾����B
 *			</ul>
 */ 
UI_16 Bt_mod_drv_GetTxEmptyBufferSize( void );

/** 
 * @brief	�V�X�e����Ԑݒ�
 *
 * @param	type	[in]	�V�X�e����Ԏ�ʁi�o�́j
 * @param	level	[in]	�o�̓��x���iN_HIGH or N_LOW�j
 *
 * @return	���s���ʁiE_BT_MOD_DRV_RESULT�j
 * @retval	BT_MOD_DRV_OK				����I��
 * @retval	BT_MOD_DRV_ERR_INVALIDARG	�����s��
 *
 * @note
 */ 
E_BT_MOD_DRV_RESULT Bt_mod_drv_SetSystemState( E_BT_SYS_STATE_TYPE_OUT type, UI_8 level );

/** 
 * UART��M����������
 */ 
__interrupt void Intr_LinUart6_Rx (void);

/** 
 * UART���M���������݁i���M�o�b�t�@�G���v�e�B�����݁j
 */ 
__interrupt void Intr_LinUart6_Tx (void);

#endif	/* __BT_MOD_DRV_H__ */
