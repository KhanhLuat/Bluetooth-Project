/********************************************************************************************/
/* file defi_uart_prt.c																		*/
/* Defi UART�v���g�R�� 																		*/
/* 																							*/
/* Defi���j�b�g�܂��́A�e�X�g������M�������b�Z�[�W����͂Ɖ������b�Z�[�W�𑗐M����B	*/
/* <ul>																						*/
/* <li>�uDefi UART�f�o�C�X�h���C�o�v�����M�f�[�^���擾���A���b�Z�[�W�v���g�R����͂��s���A*/
/* 		����ȃ��b�Z�[�W���uDefi���j�b�g����v�ɓn���B										*/
/* <li>�uDefi���j�b�g����v����������������b�Z�[�W���uDefi UART�f�o�C�X�h���C�o�v�֓n���A*/
/* 		���M����B																			*/
/* </ul>																					*/
/* 																							*/
/********************************************************************************************/
/* author      Y.Sorimachi																	*/
/* date        2014/4/11																	*/
/********************************************************************************************/
#include <string.h>														/* �W��ͯ�ް̧�� */
#include "type.h"
#include "system.h"
#include "defi_uart_prt.h"												/* Defi UART���ĺ�ͯ�ް̧�� */
#include "SSFTSTD_Macro.h"
#include "SHRVSTD_UART_Drv.h"											/* UART�h���C�o�w�b�_�[�t�@�C�� */
#include "SHRVSTD_Interrupt_Drv.h"
#include "DTO459_VDV_RringBuffer.h"
#include "time_measure.h"

/****************************************************************************/
/*	�����}�N��																*/
/****************************************************************************/
#define DEFIUART_LOCAL	static

#define		UA1_INTRX_BUF_SZ			(1 * 1024)						/**< UART1��M�ޯ̧����(������) 	*/
#define		UA_ANALYZE_WORK_BUF_SZ		(25)							/**< UART�f�[�^��͗p���[�N�o�b�t�@�T�C�Y	*/
#define		UA1_RX_BUF_NUM				(2)								/**< UART1��M�ޯ̧�� 				*/
#define		UA1_RX_BUF_SZ				(35)							/**< UART1��M�ޯ̧���� 			*/
#define		UA1_TX_BUF_SZ				(1 * 1024)						/**< UART1���M�ޯ̧���� 			*/ 
#define		UA_HEAD_7701				(0xFF)							/**< DF07701ͯ�� 					*/

/**
 *	UART1��M���o���ð��
 */
typedef enum e_ua1_status { 
	ST_UA1_WAIT_HEAD1 = 0,												/**< �w�b�_��M�҂�1 */
	ST_UA1_WAIT_HEAD2,													/**< �w�b�_��M�҂�2 */
	ST_UA1_WAIT_MODE,													/**< Ӱ�ގ�M�҂� 	 */
	ST_UA1_WAIT_TESTMODE,												/**< ý�Ӱ�� 2byte�ڂ̎�M�҂� 	*/
	ST_UA1_RECEIVING,													/**< ��M�� 		*/
	ST_UA1_MAX															/**< �v�f�� 		*/
} E_UA1_STATUS;


/****************************************************************************/
/*	�萔�錾																*/
/****************************************************************************/
#ifdef ADVANCE_ENABLE
/**
 *	DF07701�߹�Ē�ð��� 
 */
DEFIUART_LOCAL const UI_8	C_07701_length_tbl[16] = {
	0,								/**< 0x00:--- 			*/
	26,								/**< 0x01:OPENING 		*/
	4,								/**< 0x02:ENDING 		*/
	34,								/**< 0x03:REAL 			*/
	0,								/**< 0x04:--- 			*/
	34,								/**< 0x05:REC 			*/
	34,								/**< 0x06:PLAY 			*/
	0,								/**< 0x07:--- 			*/
	25,								/**< 0x08:SET UP 		*/
	25,								/**< 0x09:CYLINDER SET 	*/
	25,								/**< 0x0A:RESPONSE SET 	*/
	31,								/**< 0x0B:METER SELECT 	*/
	28,								/**< 0x0C:WARNING SET 	*/
	0,								/**< 0x0D:--- 			*/
	0,								/**< 0x0E:--- 			*/
	4								/**< 0x0F:INITIALIZE 	*/
};
#endif /* ADVANCE_ENABLE */

/* �p���e�B�l�ϊ��e�[�u���iDEFI��`�l->UART DRV��`�l�ϊ��p�j*/
/* DEFI_UART_PARITY_xxx�̒�`���ɂ��邱�� */
DEFIUART_LOCAL const E_UARTDRV_PARITY	C_cnv_parity_defi2drv_tbl[] = {
	UARTDRV_PARITY_NONE,					/* DEFI_UART_PARITY_NONE	�p���e�B�Ȃ�				*/
	UARTDRV_PARITY_EVEN,					/* DEFI_UART_PARITY_EVEN	�����p���e�B				*/
	UARTDRV_PARITY_ODD,						/* DEFI_UART_PARITY_ODD		��p���e�B				*/
};

/* �{�[���[�g�l�ϊ��e�[�u���iDEFI��`�l->UART DRV��`�l�ϊ��p�j*/
/* DEFI_UART_BAUD_RATE_xxx�̒�`���ɂ��邱�� */
DEFIUART_LOCAL const E_UARTDRV_BAUD_RATE	C_cnv_brate_defi2drv_tbl[] = {
	UARTDRV_BAUD_RATE_9600,					/* DEFI_UART_BAUD_RATE_9600		9600bps					*/
	UARTDRV_BAUD_RATE_19200,				/* DEFI_UART_BAUD_RATE_19200	19200bps				*/
	UARTDRV_BAUD_RATE_115200,				/* DEFI_UART_BAUD_RATE_115200	115200bps				*/
};

/****************************************************************************/
/*	�\���̐錾																*/
/****************************************************************************/


/****************************************************************************/
/*	�����ϐ���`															*/
/****************************************************************************/
DEFIUART_LOCAL 	T_RINGBUFFER Rx_buf_int1_mng;									/**< UART1��M�ޯ̧�Ǘ��̈� 		*/
DEFIUART_LOCAL 	T_RINGBUFFER Rx_err_inf1_mng;									/**< UART1��M�װ���Ǘ��̈� 		*/
DEFIUART_LOCAL	UI_8	Rx_buf_int1[UA1_INTRX_BUF_SZ];							/**< UART1��M�ޯ̧(������) 		*/
DEFIUART_LOCAL	UI_8	Rx_err_inf1[UA1_INTRX_BUF_SZ];							/**< UART1��M�װ��� 				*/
#ifdef ADVANCE_ENABLE
DEFIUART_LOCAL	UI_8	Rx_buf1[UA1_RX_BUF_NUM][UA1_RX_BUF_SZ];					/**< UART1��M�ޯ̧(��͌��ʊi�[) 	*/
#endif /* ADVANCE_ENABLE */
DEFIUART_LOCAL	UI_8	Rx_len1[UA1_RX_BUF_NUM];								/**< UART1��M�ޯ̧�ް��� 			*/
#ifdef ADVANCE_ENABLE
DEFIUART_LOCAL	UI_8	Rx_idx1;												/**< UART1��M�ް��i�[�ʒu 			*/
DEFIUART_LOCAL	UI_8	Rx_set_pos1;											/**< UART1��M�ڰъi�[�ʒu 			*/
DEFIUART_LOCAL	UI_8	Rx_get_pos1;											/**< UART1��M�ڰю擾�ʒu 			*/
DEFIUART_LOCAL	UI_8	Rx_status1;												/**< UART1��M���o���ð�� 			*/
#endif /* ADVANCE_ENABLE */

DEFIUART_LOCAL 	T_RINGBUFFER Tx_buf_mng;										/**< UART���M�ޯ̧�Ǘ��̈� 			*/
DEFIUART_LOCAL  UI_8	Tx_buf[UA1_TX_BUF_SZ];									/**< UART���M�p�o�b�t�@				*/


/****************************************************************************/
/*	�����֐��v���g�^�C�v�錾												*/
/****************************************************************************/
DEFIUART_LOCAL void uart_buf_init(void);										/* UART�ޯ̧���������� */
#ifdef ADVANCE_ENABLE
DEFIUART_LOCAL void uart1_store_buf_7701(UI_8 rx_data, UI_8 rx_err);			/* UART1��M�ޯ̧�i�[����(DF07701�ڑ�) */
#endif /* ADVANCE_ENABLE */
DEFIUART_LOCAL UI_16 get_uart1_rx_data(UI_8 *rx_data, UI_8 *rx_err, UI_16 data_len);	/* UART1��M�ް��擾���� */
DEFIUART_LOCAL void uartEnable(void);											/* UART����M�����J�n */
DEFIUART_LOCAL void uartDisable(void);											/* UART����M������~ */

/****************************************************************************/
/*	�O���֐�																*/
/****************************************************************************/
#ifdef ADVANCE_ENABLE
/** 
 * RAM������
 * 
 * param	mode		[in]	����������
 *
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		RAM�̏��������s���B
 */ 
void Uart_ram_init(void)
{
}
#endif /* ADVANCE_ENABLE */

/** 
 * UART�ʐM����M�J�n���� 
 * 
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		�ޯ̧�̏�������AUART1�̑���M�J�n�ݒ���s���B 
 */ 
void Uart_start(void)
{
	uart_buf_init();
	/* UART����M�����J�n */
	uartEnable();
}

/** 
 * UART�ʐM����M��~���� 
 * 
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		UART1�̑���M��~�ݒ���s���B
 */ 
void Uart_stop(void)
{
	/* UART����M������~ */
	uartDisable();
}

#ifdef ADVANCE_ENABLE
/** 
 * UART��M��͏���  
 * 
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		UART1�̎�M�ް���͂����s����B
 */ 
void Uart_rx_data_analyze(void)
{
	UI_8	rx_data[UA_ANALYZE_WORK_BUF_SZ];							/* ��M�ް��i�[ܰ� */
	UI_8	rx_err[UA_ANALYZE_WORK_BUF_SZ];								/* �װ���擾ܰ� */
	UI_8	rx_len;														/* ��M�ް��� */
	UI_8	i;
	
	rx_len = (UI_8)get_uart1_rx_data(rx_data, rx_err, sizeof(rx_data));

	i = 0;
	while (rx_len > 0) {
		/* �f�[�^��̓o�b�t�@�iRx_buf1�j�Ƀf�[�^���l�߂� */
		uart1_store_buf_7701(rx_data[i], rx_err[i]);
		rx_len--;
		i++;
	}
}

/** 
 * Defi�ƯĎ�M�ڰю擾���� 
 * 
 * param	*msg		[out]	��M�ڰъi�[�ޯ̧���߲��
 * param	*len		[out]	��M�ڰ��ް����i�[�ϐ����߲��
 *
 * return	��M�ڰїL���i�L:0xFF�ȊO�A��:0xFF�j 
 *
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		Defi�ƯĂ���̎�M�ڰю擾���s���B
 */ 
UI_8 Get_uart_rx_data_defi(UI_8 *msg, UI_8 *len)
{
	UI_8	ret;														/* �߂�l */
	UI_8	rx_get_pos;													/* UART1��M�ڰю擾�ʒu */
	UI_8	rx_set_pos;													/* UART1��M�ڰъi�[�ʒu */

	ret = 0xFF;
	rx_get_pos = Rx_get_pos1;
	rx_set_pos = Rx_set_pos1;

	/* �f�[�^��̓o�b�t�@�iRx_buf1�j�ɂP�t���[�����̃f�[�^���Ȃ���Έȉ��̏������s��Ȃ� */
	if (rx_get_pos != rx_set_pos) {
		(void)memcpy(msg, &Rx_buf1[rx_get_pos][0], (size_t)Rx_len1[rx_get_pos]);
		*len = Rx_len1[rx_get_pos];

		rx_get_pos++;

		if (rx_get_pos >= UA1_RX_BUF_NUM) {
			rx_get_pos = 0;
		}

		if (rx_set_pos >= rx_get_pos) {
			ret = rx_set_pos - rx_get_pos;
		} else {
			ret = UA1_RX_BUF_NUM - rx_get_pos + rx_set_pos;
		}
	}

	Rx_get_pos1 = rx_get_pos;

	return ret;
}
#endif /* ADVANCE_ENABLE */

/****************************************************************************/
/* �֐�����		:	Uart_set_comm_protocol									*/
/* �@�\�̐���	:	�ʐM�v���g�R���ݒ�										*/
/* ����			:	(I/ )protocol	�ʐM�v���g�R��							*/
/* �߂�l		:	N_OK	����											*/
/*				:	N_NG	�ُ�											*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
SI_8 Uart_set_comm_protocol(const T_DEFI_UART_PROTOCOL *protocol)
{
	T_UartDrv_Protocol	drv_protocol;

	if (protocol == NULL) {
		return N_NG;
	}

	drv_protocol.bit_len		= UARTDRV_BIT_LEN_8;
	drv_protocol.stop_bit_len	= UARTDRV_STOP_BIT_LEN_1;
	drv_protocol.direction		= UARTDRV_DIRECTION_LSB_FIRST;

	if (protocol->parity >= DEFI_UART_PARITY_NUM) {
		return N_NG;
	}
	drv_protocol.parity			= C_cnv_parity_defi2drv_tbl[protocol->parity];

	if (protocol->baud_rate >= DEFI_UART_BAUD_RATE_NUM) {
		return N_NG;
	}
	drv_protocol.baud_rate		= C_cnv_brate_defi2drv_tbl[protocol->baud_rate];

	if (UartDrv_ChangeProtocol(UARTDRV_CH_ADVANCE, &drv_protocol) != D_OK) {
		return N_NG;
	}

	return N_OK;
}

/****************************************************************************/
/* �֐�����		:	Uart_get_rx_data										*/
/* �@�\�̐���	:	��M�f�[�^�擾											*/
/* ����			:	( /O)buf	�f�[�^�o�b�t�@								*/
/*				:	(I/ )len	�擾�f�[�^��(byte)							*/
/* �߂�l		:	�擾�ł����f�[�^��(byte)								*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
UI_16 Uart_get_rx_data(UI_8 *buf, UI_16 len)
{
	UI_16	get_len;

	if (buf == NULL) {
		return 0;
	}

	get_len = get_uart1_rx_data(buf, NULL, len);

	return get_len;
}

/****************************************************************************/
/* �֐�����		:	Uart_get_rx_data_len									*/
/* �@�\�̐���	:	��M�f�[�^���擾										*/
/* ����			:	void													*/
/* �߂�l		:	��M�f�[�^��(byte)										*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
UI_16 Uart_get_rx_data_len(void)
{
	UI_16	len;

	/* �ǂݍ��݉\�f�[�^�T�C�Y�擾 */
	len = RingBuffer_GetCanReadSize(&Rx_buf_int1_mng);

	return len;
}

/****************************************************************************/
/* �֐�����		:	Uart_set_tx_data										*/
/* �@�\�̐���	:	���M�f�[�^�ݒ�											*/
/* ����			:	(I/ )buf	�f�[�^�o�b�t�@								*/
/*				:	(I/ )len	���M�f�[�^��(byte)							*/
/* �߂�l		:	N_OK	����											*/
/*				:	N_NG	�ُ�											*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
SI_8 Uart_set_tx_data(UI_8 *buf, UI_16 len)
{
	UI_16	can_tx_len;

	if (buf == NULL) {
		return N_NG;
	}

	/* �ݒ�\�f�[�^�T�C�Y�擾	*/
	can_tx_len = RingBuffer_GetCanWriteSize(&Tx_buf_mng);
	if (can_tx_len < len) {
		/* ���M�f�[�^�o�b�t�@�Ɏw��T�C�Y���̋󂫂��Ȃ�	*/
		return N_NG;
	}

	/* ���M�f�[�^�o�b�t�@�Ƀf�[�^��ݒ�	*/
	(void)RingBuffer_SetArrayData(&Tx_buf_mng, buf, len);

	/* UART�h���C�o���M���荞�݋��i���M�����ݏ����Ńf�[�^���M�����s�����j */
	UartDrv_TxIntrReqEnblCtrl( UARTDRV_CH_ADVANCE, D_ON );

	return N_OK;
}

/****************************************************************************/
/* �֐�����		:	Uart_get_can_set_tx_data_len							*/
/* �@�\�̐���	:	�ݒ�\���M�f�[�^���擾								*/
/* ����			:	void													*/
/* �߂�l		:	�ݒ�\���M�f�[�^��									*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/01/20												*/
/* ���l			:															*/
/****************************************************************************/
UI_16 Uart_get_can_set_tx_data_len(void)
{
	UI_16	can_tx_len;

	/* �ݒ�\�f�[�^�T�C�Y�擾	*/
	can_tx_len = RingBuffer_GetCanWriteSize(&Tx_buf_mng);

	return can_tx_len;
}

/****************************************************************************/
/*	�����֐�																*/
/****************************************************************************/
/** 
 * UART�ޯ̧����������
 * 
 * author	Y.Sorimachi
 * date	2014/4/11
 * note	UART����M�ޯ̧�̏��������s���B
 */ 
DEFIUART_LOCAL void uart_buf_init(void)
{
	/**< UART1��M�ޯ̧(������) 		*/
	RingBuffer_Init(&Rx_buf_int1_mng, Rx_buf_int1, sizeof(Rx_buf_int1));
	/**< UART1��M�װ��� 				*/
	RingBuffer_Init(&Rx_err_inf1_mng, Rx_err_inf1, sizeof(Rx_err_inf1));

	/* UART���M�o�b�t�@������	*/
	RingBuffer_Init(&Tx_buf_mng, Tx_buf, sizeof(Tx_buf));

#ifdef ADVANCE_ENABLE
	Rx_idx1 = 0;
	Rx_set_pos1 = 0;
	Rx_get_pos1 = 0;
	Rx_status1 = 0;
#endif /* ADVANCE_ENABLE */
	(void)memset(Rx_len1, 0, sizeof(Rx_len1));

}

#ifdef ADVANCE_ENABLE
/** 
 * UART1��M�ޯ̧�i�[����(DF07701�ڑ�) 
 * 
 * param	rx_data		[in]	��M�ް�  
 * param	rx_err		[in]	��M�װ���i0:�װ�Ȃ��A1:�װ����j
 *
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		<ul>
 *			<li>��M�ް��̓��o�����s�Ȃ��A�ޯ̧�Ɋi�[����B 
 *			<li>��M�װ���������ꍇ�A���݊i�[���ڰт�j������B
 *			</ul>
 */ 
DEFIUART_LOCAL void uart1_store_buf_7701(UI_8 rx_data, UI_8 rx_err)
{
	UI_8	tbl_pos;													/* ð��وʒu�Z�o�p */
	UI_8	rx_stat;													/* UART1��M���o���ð�� */
	UI_8	rx_set_pos;													/* UART1��M�ڰъi�[�ʒu */
	UI_8	rx_idx;														/* UART1��M�ް��i�[�ʒu */

	tbl_pos = 0;
	rx_stat = Rx_status1;
	rx_set_pos = Rx_set_pos1;
	rx_idx = Rx_idx1;

	if (rx_err == 0) {
		switch (rx_stat) {
		case ST_UA1_WAIT_HEAD2:
			if (rx_data == UA_HEAD_7701) {
				rx_stat = ST_UA1_WAIT_MODE;
			} else {
				rx_stat = ST_UA1_WAIT_HEAD1;
			}
			break;

		case ST_UA1_WAIT_MODE:
			tbl_pos = rx_data & 0x0F;
			if (C_07701_length_tbl[tbl_pos] == 0) {
				/* ���[�h�l�̃f�[�^�T�C�Y���O�̏ꍇ */
				tbl_pos = 0;
			}

			if(tbl_pos != 0) {
				rx_stat = ST_UA1_RECEIVING;
				Rx_buf1[rx_set_pos][0] = UA_HEAD_7701;
				Rx_buf1[rx_set_pos][1] = UA_HEAD_7701;
				Rx_buf1[rx_set_pos][2] = tbl_pos;	/* ���[�h�l */
				Rx_len1[rx_set_pos] = 3;
				rx_idx = 3;
			} else {
				rx_stat = ST_UA1_WAIT_HEAD1;
			}
			break;

		case ST_UA1_RECEIVING:
			Rx_buf1[rx_set_pos][rx_idx] = rx_data;
			Rx_len1[rx_set_pos]++;
			rx_idx++;

			tbl_pos = Rx_buf1[rx_set_pos][2];	/* ���[�h�l */
			if(Rx_len1[rx_set_pos] < C_07701_length_tbl[tbl_pos]) {
				/* �܂���M�������Ă��Ȃ� */
				tbl_pos = 0;
			}

			if(tbl_pos != 0) {
				/* ��M�������� */
				rx_set_pos++;
				rx_idx = 0;

				if (rx_set_pos >= UA1_RX_BUF_NUM) {
					rx_set_pos = 0;
				}

				Rx_len1[rx_set_pos] = 0;
				rx_stat = ST_UA1_WAIT_HEAD1;
			}
			break;

		case ST_UA1_WAIT_HEAD1:
		default:
			if (rx_data == UA_HEAD_7701) {
				rx_stat = ST_UA1_WAIT_HEAD2;
			}
			break;
		}
	} else {
		rx_idx = 0;
		Rx_len1[rx_set_pos] = 0;
		rx_stat = ST_UA1_WAIT_HEAD1;
	}

	Rx_status1 = rx_stat;
	Rx_set_pos1 = rx_set_pos;
	Rx_idx1 = rx_idx;
}
#endif /* ADVANCE_ENABLE */

/** 
 * UART1��M�ް��擾���� 
 * 
 * param	*rx_data	[out]	��M�ް��i�[�ޯ̧���߲�� 
 * param	*rx_err		[out]	��M�װ���i�[�ޯ̧���߲���iNULL�j
 * param	data_len	[in]	�擾����f�[�^���i�ő咷�j
 *
 * return	��M�ް���
 *
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		UART1��M�����݂ɂĊi�[���ꂽ��M�ް��A�װ�����擾����B
 */ 
DEFIUART_LOCAL UI_16 get_uart1_rx_data(UI_8 *rx_data, UI_8 *rx_err, UI_16 data_len)
{
	/* �ǂݍ��݉\�f�[�^�T�C�Y�擾 */
	UI_16 rtn_rx_len = RingBuffer_GetCanReadSize(&Rx_buf_int1_mng);
	if(rtn_rx_len > 0){
		/* �f�[�^����Ȃ炻�ꂼ��f�[�^�擾 */
		if (data_len < rtn_rx_len) {
			rtn_rx_len = data_len;
		}
		/* �r���Ŋ����݂�����ƃf�[�^������邽�߂��̊Ԃ͊����݋֎~���� */
		/* UART�h���C�o��M�����݋֎~ */
		UartDrv_RxIntrReqEnblCtrl( UARTDRV_CH_ADVANCE, D_OFF );
		
		/* ��M�f�[�^�ƃG���[���͓����f�[�^�T�C�Y���i�[����Ă��� */
		(void)RingBuffer_GetArrayData(&Rx_buf_int1_mng, rx_data, rtn_rx_len, RINGBUF_READEND);
		if (rx_err != NULL) {
			(void)RingBuffer_GetArrayData(&Rx_err_inf1_mng, rx_err, rtn_rx_len, RINGBUF_READEND);
		}
		else {
			/* �G���[���i�[�o�b�t�@���w�肳��Ă��Ȃ��ꍇ�͓ǂݎ̂Ă�	*/
			(void)ReadEndRingBuffer(&Rx_err_inf1_mng, rtn_rx_len);
		}

		/* UART�h���C�o��M�����݋��� */
		UartDrv_RxIntrReqEnblCtrl( UARTDRV_CH_ADVANCE, D_ON );
	}

	return rtn_rx_len;
}





/** 
 * UART��M����������
 */ 
__interrupt void Intr_LinUart7_Rx (void)
{
	SI_8 result;
	UI_8 rxFulFlg;
	UI_8 rxData;
	UI_16 can_write_len;
	T_UartDrv_ErrInfo err;
	
	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_ADVANCE_INTR_RX);)

	/* �������݉\�T�C�Y�擾 */
	can_write_len = RingBuffer_GetCanWriteSize(&Rx_buf_int1_mng);
	/* ��M�f�[�^�擾 */
	result = UartDrv_GetErr(UARTDRV_CH_ADVANCE, &err);
	if(result == D_OK)
	{
		if( (err.parity_flg  == D_ON) ||	/* �p���e�B�G���[�t���O */
			(err.overrun_flg == D_ON) ||	/* �I�[�o�����G���[�t���O */
			(err.flaming_flg == D_ON) )		/* �t���[�~���O�G���[�t���O */
		{
			/* �G���[���� */
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
			/* �������݉\�T�C�Y�擾 */
			if(can_write_len > 0){
				/* �������݉\ */
				(void)RingBuffer_SetByteData(&Rx_buf_int1_mng, 0x00);	/* ��M�f�[�^��0x00��ݒ� */
				(void)RingBuffer_SetByteData(&Rx_err_inf1_mng, rxData);
			}
			/* UART����M������~ */
			uartDisable();
			/* UART����M�����J�n */
			uartEnable();
		}
		else
		{
			/* �G���[�Ȃ� */
			/* ��M�f�[�^�t���t���O�̎擾 */
			rxFulFlg = UartDrv_GetRxFullFlag( UARTDRV_CH_ADVANCE );
			if(rxFulFlg == D_ON)
			{
				/* ��M�f�[�^�̎擾 */
				result = UartDrv_GetRxData(UARTDRV_CH_ADVANCE, &rxData);
				if(result == D_OK)
				{
					if(can_write_len > 0){
						/* �������݉\ */
						(void)RingBuffer_SetByteData(&Rx_buf_int1_mng, rxData);
						(void)RingBuffer_SetByteData(&Rx_err_inf1_mng, 0x00);	/* �G���[�f�[�^��0x00��ݒ� */
					}
				}
			}
		}
	}
	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_ADVANCE_INTR_RX);)
	return;
}

/** 
 * UART���M���������݁i���M�o�b�t�@�G���v�e�B�����݁j
 */ 
__interrupt void Intr_LinUart7_Tx (void)
{
	UI_8	data;

	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_ADVANCE_INTR_TX);)

	if (RingBuffer_GetByteData(&Tx_buf_mng, RINGBUF_READEND, &data) == 0) {
		/* ���M�f�[�^���� */
		/* �f�[�^���M */
		(void)UartDrv_SetTxData(UARTDRV_CH_ADVANCE, data);
	}
	else {
		/* ���M�f�[�^�Ȃ� */
		/* UART�h���C�o���M���荞�݋֎~ */
		UartDrv_TxIntrReqEnblCtrl( UARTDRV_CH_ADVANCE, D_OFF );
	}

	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_ADVANCE_INTR_TX);)

	return;
}

/********************************************************/
/* �֐�����		: uartEnable							*/
/* �@�\�̐���	: UART����M�����J�n					*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: 										*/
/********************************************************/
DEFIUART_LOCAL void uartEnable(void)
{
	/* UART�h���C�o��M����(���M���������) */
	(void)UartDrv_ReqRxCtrl( UARTDRV_CH_ADVANCE, D_ENABLE );

	/*  UART�h���C�o��M���荞�݋��� */
	UartDrv_RxIntrReqEnblCtrl( UARTDRV_CH_ADVANCE, D_ON );
}

/********************************************************/
/* �֐�����		: uartDisable							*/
/* �@�\�̐���	: UART����M����~						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: 										*/
/********************************************************/
DEFIUART_LOCAL void uartDisable(void)
{
	/* UART�h���C�o��M�����݋֎~ */
	UartDrv_RxIntrReqEnblCtrl( UARTDRV_CH_ADVANCE, D_OFF );

	/* UART�h���C�o���M�����݋֎~ */
	UartDrv_TxIntrReqEnblCtrl( UARTDRV_CH_ADVANCE, D_OFF );

	/* UART�h���C�o��M�֎~(���M���֎~�����) */
	(void)UartDrv_ReqRxCtrl( UARTDRV_CH_ADVANCE, D_DISABLE );
}
