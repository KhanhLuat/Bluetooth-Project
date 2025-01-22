/*==================================================================================================*/
/* @file	bt_mod_drv.c																			*/
/* @brief	Bluetooth Module Driver																	*/
/*==================================================================================================*/
/* @par Copyright																					*/
/*	Copyright (c) 2015 by N.S. Ltd. All rights reserved.											*/
/*																									*/
/* @par File History																				*/
/*   | Date       | Summary                                  | Source Repository	 | Revision |	*/
/*   | ---------- | ---------------------------------------- | -------------------- | -------- |	*/
/*   | 2015-xx-xx | Created for XXXXXXX                      | N/A                  | N/A      |	*/
/*==================================================================================================*/


#include "DTO459_VDV_bt_mod_drv.h"
#include "SHRVSTD_Port_Drv.h"
#include "SHRVSTD_UART_Drv.h"
#include "SHRVSTD_CPU_Drv.h"
#include "SSFTSTD_Macro.h"
#include <string.h>
#include "dbglog.h"
#include "time_measure.h"

#define	BTMODDRV_LOCAL	static
/********************************************************/
/*	pragma�錾											*/
/********************************************************/
/**
 *	���荞��
**/
BTMODDRV_LOCAL void S_Intr_BT_mod_drv_Send( void );
BTMODDRV_LOCAL void S_Intr_BT_mod_drv_Receive( void );
BTMODDRV_LOCAL void S_Intr_BT_mod_drv_Err( void );

/********************************************************/
/*	�����}�N��											*/
/********************************************************/
/**
 * GPIO�֌W�}�N�� 
**/
#define D_BTMODDRV_RESET_WAIT_USEC				(200U)		/**< ���Z�b�g�҂�����(usec)				*/

/**
 * �ėp�萔
**/
#ifndef N_TRUE
#define N_TRUE			(1U)	/* �^:TRUE */
#endif /* N_TRUE */
#ifndef N_FALSE
#define N_FALSE			(0U)	/* �U:FALSE */
#endif /* N_FALSE */

/********************************************************/
/*	�萔�錾											*/
/********************************************************/

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
BTMODDRV_LOCAL UI_8 S_send_permission_state;				/**< ���M����ԁiN_TRUE�F���M���AN_FALSE�F���M�֎~�j*/
BTMODDRV_LOCAL UI_8 S_bt_mod_buf_rx[BT_MOD_DRV_RX_BUF];		/**< ��M�f�[�^�i�[�p�o�b�t�@ */
BTMODDRV_LOCAL UI_8 S_bt_mod_buf_tx[BT_MOD_DRV_TX_BUF];		/**< ���M�f�[�^�i�[�p�o�b�t�@ */
BTMODDRV_LOCAL UI_8 S_bt_mod_buf_err[BT_MOD_DRV_RX_BUF];	/**< �G���[�f�[�^�i�[�p�o�b�t�@ */
BTMODDRV_LOCAL UI_16 S_bt_mod_set_rx_data_pos;				/**< ��M�f�[�^�i�[�ʒu */
BTMODDRV_LOCAL UI_16 S_bt_mod_get_rx_data_pos;				/**< ��M�f�[�^�擾�ʒu */
BTMODDRV_LOCAL UI_16 S_bt_mod_set_tx_data_pos;				/**< ���M�f�[�^�i�[�ʒu */
BTMODDRV_LOCAL UI_16 S_bt_mod_get_tx_data_pos;				/**< ���M�f�[�^�擾�ʒu */

/********************************************************/
/*	�v���g�^�C�v�錾									*/
/********************************************************/
BTMODDRV_LOCAL UI_16 S_Bt_mod_get_data_size(void);
BTMODDRV_LOCAL UI_16 S_Bt_mod_send_data_size(void);
BTMODDRV_LOCAL void S_Bt_mod_UartEnable( void );
BTMODDRV_LOCAL void S_Bt_mod_UartDisable( void );

/********************************************************/
/*	�O���֐�											*/
/********************************************************/

void Bt_mod_drv_Init( void ) 
{	
	/**	<ol> */
	/**	<li>�����ϐ��̏����� */
	S_send_permission_state = N_TRUE;
	(void)memset(S_bt_mod_buf_rx, 0, sizeof(S_bt_mod_buf_rx));
	(void)memset(S_bt_mod_buf_tx, 0, sizeof(S_bt_mod_buf_tx));
	(void)memset(S_bt_mod_buf_err, 0, sizeof(S_bt_mod_buf_err));
	S_bt_mod_set_rx_data_pos = 0;
	S_bt_mod_get_rx_data_pos = 0;
	S_bt_mod_set_tx_data_pos = 0;
	S_bt_mod_get_tx_data_pos = 0;
	/**	</ol> */
	return;
}

void Bt_mod_drv_StartBtModule( void )
{
	/* �����Ȃ�(POWER�s���Ȃ�) */
	return;
}

void Bt_mod_drv_StopBtModule( void )
{
	/* �����Ȃ�(POWER�s���Ȃ�) */
	return;
}

void Bt_mod_drv_StartComm( void )
{
	/** UART�h���C�o����M�������� */
	S_Bt_mod_UartEnable();
	return;
}

void Bt_mod_drv_StopComm( void )
{	
	/** UART�h���C�o����M���֎~���� */
	S_Bt_mod_UartDisable();
	return;
}

void Bt_mod_drv_Reset( void )
{
	UI_32 i;
	/** ���Z�b�g���s */
	(void)PortDrv_SetDataBit( IO_PO_GR_BT_RST, IO_PO_BIT_BT_RST, IO_PO_LEV_ACTIVE_BT_RST );
	
	/** 200us�E�F�C�g */
	for( i = 0 ; i < D_BTMODDRV_RESET_WAIT_USEC ; i++){
		/* 1�ʂ��E�F�C�g */
		D_NOP80();
	}

	/** ���Z�b�g���� */
	(void)PortDrv_SetDataBit( IO_PO_GR_BT_RST, IO_PO_BIT_BT_RST, IO_PO_LEV_NONACT_BT_RST );
	return;
}


void Bt_mod_drv_ResetON( void )
{
	/** ���Z�b�g���s */
	(void)PortDrv_SetDataBit( IO_PO_GR_BT_RST, IO_PO_BIT_BT_RST, IO_PO_LEV_ACTIVE_BT_RST );
	return;
}
void Bt_mod_drv_ResetOFF( void )
{
	/** ���Z�b�g���� */
	(void)PortDrv_SetDataBit( IO_PO_GR_BT_RST, IO_PO_BIT_BT_RST, IO_PO_LEV_NONACT_BT_RST );
	return;
}


E_BT_MOD_DRV_RESULT Bt_mod_drv_GetRxData(UI_8* buf, UI_16* len)
{
	UI_16 i;
	UI_8 tmp;
	UI_16 index;
	UI_16 get_data_size;
	index = 0;
	
	/** <ol> */
	/** <li>�����`�F�b�N */
	/** <li>�������s����������ABT_MOD_DRV_ERR_INVALIDARG��Ԃ� */
	if ((buf == NULL) || (len == NULL))
	{
		return BT_MOD_DRV_ERR_INVALIDARG;
	}
	/**	<li> ��M�m�F�f�[�^���Ȃ���΁ABT_MOD_DRV_ERR_NOT_RECIEVE_DATA��Ԃ� */
	get_data_size = S_Bt_mod_get_data_size();
	if (get_data_size <= 0)
	{
		return BT_MOD_DRV_ERR_NOT_RECIEVE_DATA;
	}
	/** <li>��M�f�[�^�̎擾 */
	for (i = 0; i < get_data_size; i++)
	{
		/* �G���[�f�[�^���m�F */
		tmp = S_bt_mod_buf_err[S_bt_mod_get_rx_data_pos];
		if(tmp == 0x00)
		{
			/* �G���[�Ȃ� */
			/* �f�[�^���R�s�[ */
			buf[index] = S_bt_mod_buf_rx[S_bt_mod_get_rx_data_pos];
			index++;
		}
		else
		{
			/* �G���[���� */
			/* �R�s�[�����f�[�^��j�� */
			index = 0;
		}
		S_bt_mod_get_rx_data_pos++;
		S_bt_mod_get_rx_data_pos %= BT_MOD_DRV_RX_BUF;
	}
	/** <li>��M�f�[�^���̎擾 */
	*len = index;
	/** </ol> */
	return BT_MOD_DRV_OK;
}

E_BT_MOD_DRV_RESULT Bt_mod_drv_SetTxData(const UI_8* buf, UI_16 len)
{
	UI_16 i;
	UI_16 send_data_size;

	/** <ol> */
	/** <li>�����`�F�b�N */
	/** <li>�������s����������ABT_MOD_DRV_ERR_INVALIDARG��Ԃ� */
	if ((buf == NULL) || (len <= 0))
	{
		return BT_MOD_DRV_ERR_INVALIDARG;
	}
	/** <li>�o�b�t�@�T�C�Y�𒴂��Ă���΁ABT_MOD_DRV_ERR_BUFFERFULL��Ԃ� */
	send_data_size = S_Bt_mod_send_data_size();
	if ((len + send_data_size) > BT_MOD_DRV_TX_BUF)
	{
		return BT_MOD_DRV_ERR_BUFFERFULL;
	}
	/** <li>���M�o�b�t�@�ɑ��M����f�[�^��ݒ肷�� */
	for (i = 0; i < len; i++)
	{
		S_bt_mod_buf_tx[S_bt_mod_set_tx_data_pos++] = buf[i];
		S_bt_mod_set_tx_data_pos %= BT_MOD_DRV_TX_BUF;
	}
	/** <li>���M����Ԃ�������Ă���ꍇ(���M���łȂ����) */
	if (S_send_permission_state == N_TRUE)
	{
		/** <ul> */
		/** <li>���M����Ԃ��֎~�ɂ��� */
		S_send_permission_state = N_FALSE;
		/* UART�h���C�o���M���荞�݋��� */
		UartDrv_TxIntrReqEnblCtrl( UARTDRV_CH_BT, D_ON );
		/* �o�b�t�@�G���v�e�B�����݂�����̂ł����ł͑��M���Ȃ� */
		/**	</ul> */
	}
	/**	</ol> */
	return BT_MOD_DRV_OK;
}


UI_16 Bt_mod_drv_GetTxEmptyBufferSize(void)
{
	UI_16 empty_tx_buf_size;
	/**	<ol> */
	/** <li>���M�o�b�t�@�̋󂫃T�C�Y���m�F���� */
	empty_tx_buf_size = BT_MOD_DRV_TX_BUF - S_Bt_mod_send_data_size();
	/**	</ol> */
	return empty_tx_buf_size;
}


E_BT_MOD_DRV_RESULT Bt_mod_drv_SetSystemState( E_BT_SYS_STATE_TYPE_OUT type, UI_8 level )
{
	/* �s�v */
	return BT_MOD_DRV_OK;
}

/********************************************************/
/*	�����݊֐�											*/
/********************************************************/
/** 
 * @brief	��M������
 *
 * @author	
 * @date	2015/-/-
 * @note	UART��M�����݂ɂ����s����A��M�f�[�^���o�b�t�@�Ɋi�[����B
 */ 
BTMODDRV_LOCAL void S_Intr_BT_mod_drv_Receive( void )
{
	SI_8 result;
	UI_8 rxFlg;
	UI_8 rxData;
	/* ��M�f�[�^�t���t���O�̎擾 */
	rxFlg = UartDrv_GetRxFullFlag( UARTDRV_CH_BT );
	if(rxFlg == D_ON)
	{
		/* ��M�f�[�^�̎擾 */
		result = UartDrv_GetRxData(UARTDRV_CH_BT, &rxData);
		if(result == D_OK)
		{
			/** <li>��M�o�b�t�@�ɋ󂫂������ */
			if (S_Bt_mod_get_data_size() < (BT_MOD_DRV_RX_BUF - 1))
			{
				/** <ul> */
				/** <li> ��M�f�[�^���o�b�t�@�ɓ���� */
				S_bt_mod_buf_rx[S_bt_mod_set_rx_data_pos] = rxData;
				S_bt_mod_buf_err[S_bt_mod_set_rx_data_pos] = 0x00;	/* �G���[�Ȃ� */
				S_bt_mod_set_rx_data_pos++;
				S_bt_mod_set_rx_data_pos %= BT_MOD_DRV_RX_BUF;
				/** </ul> */
			}
		}
	}
}

/** 
 * @brief	���M������
 *
 * @author	
 * @date	2015/-/-
 * @note	UART���M���������݂ɂ����s����A�c���M�f�[�^�𑗐M����B
 */ 
BTMODDRV_LOCAL void S_Intr_BT_mod_drv_Send( void )
{
	UI_8 txData;
	if (S_Bt_mod_send_data_size() > 0)
	{
		/** <li>�o�b�t�@���瑗�M�f�[�^���������� */
		txData =  S_bt_mod_buf_tx[S_bt_mod_get_tx_data_pos++];
		S_bt_mod_get_tx_data_pos %= BT_MOD_DRV_TX_BUF;
		(void)UartDrv_SetTxData(UARTDRV_CH_BT, txData);
	}
	/** <li>�����M�f�[�^�c�����Ȃ���� */
	else
	{
		/** <ul> */
		/* UART�h���C�o���M���荞�݋֎~ */
		UartDrv_TxIntrReqEnblCtrl( UARTDRV_CH_BT, D_OFF );
		/** <li>���M����Ԃ����ɂ��� */
		S_send_permission_state = N_TRUE;
		/** </ul> */
	}
}

/** 
 * @brief	�G���[������
 *
 * @author	
 * @date	2015/-/-
 * @note	��M�G���[�ɂ���Ď��s����A�G���[�����i�����ݏ����̉����j���s���B
 */ 
BTMODDRV_LOCAL void S_Intr_BT_mod_drv_Err( void )
{
		/** UART�h���C�o����M���֎~���� */
	S_Bt_mod_UartDisable();
	/** UART�h���C�o����M�������� */
	S_Bt_mod_UartEnable();
}

/********************************************************/
/*	�����֐�											*/
/********************************************************/
/** 
 * @brief	��M�f�[�^�T�C�Y�擾
 *
 * @return	��M�f�[�^�T�C�Y
 * @note	��M�o�b�t�@�ɕۑ�����Ă���f�[�^�T�C�Y��Ԃ��B
 */ 
 
BTMODDRV_LOCAL UI_16 S_Bt_mod_get_data_size(void)
{
	UI_16 rtn_size;
	UI_16 tmp_set_rx_data_pos;
	UI_16 tmp_get_rx_data_pos;
	/** <ol> */
	/** <li>��M�f�[�^�i�[�ʒu�Ǝ�M�f�[�^�擾�ʒu���ꎞ�̈�ɕۑ����� */
	tmp_set_rx_data_pos = S_bt_mod_set_rx_data_pos;
	tmp_get_rx_data_pos = S_bt_mod_get_rx_data_pos;
	/** <li>�������݈ʒu�Ɠǂݍ��݈ʒu�����M�o�b�t�@�̃f�[�^����Ԃ� */
	if(tmp_get_rx_data_pos <= tmp_set_rx_data_pos)
	{
		rtn_size = tmp_set_rx_data_pos - tmp_get_rx_data_pos;
	}
	else
	{
		rtn_size = BT_MOD_DRV_RX_BUF - tmp_get_rx_data_pos + tmp_set_rx_data_pos;
	}
	/** </ol> */
	return rtn_size;
}
/** 
 * @brief	�����M�f�[�^�T�C�Y�擾
 *
 * @return	�����M�f�[�^�T�C�Y
 * @note	���M�o�b�t�@�ɕۑ�����Ă���f�[�^�T�C�Y��Ԃ��B
 */
BTMODDRV_LOCAL UI_16 S_Bt_mod_send_data_size(void)
{
		UI_16 rtn_size;
	UI_16 tmp_set_tx_data_pos;
	UI_16 tmp_get_tx_data_pos;
	/** <ol> */
	/** <li>���M�f�[�^�i�[�ʒu�Ƒ��M�f�[�^�擾�ʒu���ꎞ�̈�ɕۑ����� */
	tmp_set_tx_data_pos = S_bt_mod_set_tx_data_pos;
	tmp_get_tx_data_pos = S_bt_mod_get_tx_data_pos;
	/** <li>�������݈ʒu�Ɠǂݍ��݈ʒu���瑗�M�o�b�t�@�̃f�[�^����Ԃ� */
	if (tmp_get_tx_data_pos <= tmp_set_tx_data_pos)
	{
		rtn_size = tmp_set_tx_data_pos - tmp_get_tx_data_pos;
	}
	else
	{
		rtn_size = BT_MOD_DRV_TX_BUF - tmp_get_tx_data_pos + tmp_set_tx_data_pos;
	}
	/** </ol> */
	return rtn_size;
}

/** 
 * @brief	UART����M��������
 *
 * @note	������ S_bt_uart_status �� UART_STATUS_TX_ENABLED �ɂȂ�
 */
BTMODDRV_LOCAL void S_Bt_mod_UartEnable( void )
{
	/** UART�h���C�o��M���� */
	(void)UartDrv_ReqRxCtrl( UARTDRV_CH_BT, D_ENABLE );
	/* UART�h���C�o��M���荞�݋��� */
	UartDrv_RxIntrReqEnblCtrl( UARTDRV_CH_BT, D_ON );
}

/** 
 * @brief	UART����M���֎~����
 *
 * @note	������ S_bt_uart_status �� UART_STATUS_DISABLED �ɂȂ�
 */
BTMODDRV_LOCAL void S_Bt_mod_UartDisable( void )
{
	/* UART�h���C�o���M���荞�݋֎~ */
	UartDrv_TxIntrReqEnblCtrl( UARTDRV_CH_BT, D_OFF );
	/* UART�h���C�o��M���荞�݋֎~ */
	UartDrv_RxIntrReqEnblCtrl( UARTDRV_CH_BT, D_OFF );
	/** UART�h���C�o��M�֎~ */
	(void)UartDrv_ReqRxCtrl( UARTDRV_CH_BT, D_DISABLE );
}

/** 
 * UART��M����������
 */ 
__interrupt void Intr_LinUart6_Rx (void)
{
	UI_8 rxData;
	SI_8 errResult;
	T_UartDrv_ErrInfo err;
	
	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_BT_INTR_RX);)

	/* ��M�f�[�^�擾 */
	errResult = UartDrv_GetErr(UARTDRV_CH_BT, &err);
	if(errResult == D_OK)
	{
		if( (err.parity_flg  == D_ON) ||	/* �p���e�B�G���[�t���O */
			(err.overrun_flg == D_ON) ||	/* �I�[�o�����G���[�t���O */
			(err.flaming_flg == D_ON) )		/* �t���[�~���O�G���[�t���O */
		{
			rxData = 0x00;
			if (err.parity_flg  == D_ON)
			{
				rxData |= 0x01;
			}
			if (err.overrun_flg == D_ON)
			{
				rxData |= 0x02;
			}
			if (err.flaming_flg == D_ON)
			{
				rxData |= 0x04;
			}
			S_bt_mod_buf_rx[S_bt_mod_set_rx_data_pos] = 0x00;		/* ��M�f�[�^��0�Ƃ��� */
			S_bt_mod_buf_err[S_bt_mod_set_rx_data_pos] = rxData;	
			S_bt_mod_set_rx_data_pos++;
			S_bt_mod_set_rx_data_pos %= BT_MOD_DRV_RX_BUF;
			/* ��M�G���[ */
			S_Intr_BT_mod_drv_Err();
		}
		else
		{
			/* ��MOK */
			S_Intr_BT_mod_drv_Receive();
		}
	}
	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_BT_INTR_RX);)
}

/** 
 * UART���M���������݁i���M�o�b�t�@�G���v�e�B�����݁j
 */ 
__interrupt void Intr_LinUart6_Tx (void)
{
	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_BT_INTR_TX);)
	S_Intr_BT_mod_drv_Send();
	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_BT_INTR_TX);)
}
