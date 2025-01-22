/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: FR81S								*/
/* Date				: YY/MM/DD							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Katsura						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		:									*/
/* date				:									*/
/********************************************************/

#include <type.h>
#include <system.h>
#include <string.h>
#include "SSFTSTD_Type.h"
#include "SSFTSTD_Macro.h"
#include "mb91570.h"
#include "DTO459_VDV_Gnss_Uart_Drv.h"
#include "dbglog.h"
#include "time_measure.h"

#define GNSS_UART_DRV_LOCAL	static

typedef struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	UPCL:1;
			__BYTE	RESV01:1;
			__BYTE	RESV02:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
			__BYTE	TBIE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} SCR;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:3;
			__BYTE	RESV11:1;
			__BYTE	SBL:1;
			__BYTE	BDS:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} SMR;
	union {
		__BYTE	byte;
		struct {
			__BYTE	REC:1;
			__BYTE	RESV21:1;
			__BYTE	PE:1;
			__BYTE	FRE:1;
			__BYTE	ORE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	TBI:1;
		} bit;
	} SSR;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV30:1;
			__BYTE	ESBL:1;
			__BYTE	INV:1;
			__BYTE	PEN:1;
			__BYTE	P:1;
			__BYTE	L:3;
		} bit;
	} ESCR;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDRL;
		} byte;
		struct {
			__HWORD	RESV1:7;
			__HWORD	D:9;
		} bit9;
		struct {
			__HWORD	RESV1:9;
			__HWORD	D:7;
		} bit7;
		struct {
			__HWORD	RESV1:10;
			__HWORD	D:6;
		} bit6;
		struct {
			__HWORD	RESV1:11;
			__HWORD	D:5;
		} bit5;
	} RDR;
	union {
		__HWORD	hword;
		struct {
			__HWORD	EXT:1;
			__HWORD	BGR:15;
		} bit;
	} BGR;
	__WORD	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV120:1;
			__BYTE	RESV121:1;
			__BYTE	RESV122:1;
			__BYTE	FLSTE:1;
			__BYTE	FRIIE:1;
			__BYTE	FDRQ:1;
			__BYTE	FTIE:1;
			__BYTE	FSEL:1;
		} bit;
	} FCR1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV130:1;
			__BYTE	FLST:1;
			__BYTE	FLD:1;
			__BYTE	FSET:1;
			__BYTE	FCL2:1;
			__BYTE	FCL1:1;
			__BYTE	FE2:1;
			__BYTE	FE1:1;
		} bit;
	} FCR0;
	__HWORD	FBYTE;
} T_Uart_reg;


#define GNSS_BAUDRATE_DEFAULT	GNSS_BAUDRATE_9600_BPS		/* �f�t�H���g�{�[���[�g		*/

#define PCLK_FREQ				((UI_32)16 * 1000000)		/* ���ӃN���b�N���g��		*/

/* �eUART�`�����l�����Ƃ�UART���W�X�^ */
GNSS_UART_DRV_LOCAL volatile T_Uart_reg *const C_Uart_reg[GNSS_UART_DRV_UART_CH_COUNT] = {
	(volatile T_Uart_reg*)&IO_UART0, (volatile T_Uart_reg*)&IO_UART1, (volatile T_Uart_reg*)&IO_UART8, (volatile T_Uart_reg*)&IO_UART9
};

/* �eUART�`�����l�����Ƃ̃{�[���[�g */
GNSS_UART_DRV_LOCAL E_GNSS_BAUDRATE S_Uart_baudrate[GNSS_UART_DRV_UART_CH_COUNT] = {
	GNSS_BAUDRATE_DEFAULT, GNSS_BAUDRATE_DEFAULT, GNSS_BAUDRATE_DEFAULT, GNSS_BAUDRATE_DEFAULT
};

/* �{�[���[�g�W�F�l���[�^�ݒ�l */
GNSS_UART_DRV_LOCAL UI_16 const C_Bgr_value[] = {
	(UI_16)1666, /* (PCLK_FREQ /   9600 - 1), */	/*   9,600bps			*/
	(UI_16)((PCLK_FREQ /  19200) - 1),				/*  19,200bps			*/
	(UI_16)((PCLK_FREQ /  38400) - 1),				/*  38,400bps			*/
	(UI_16)((PCLK_FREQ /  57600) - 1),				/*  57,600bps			*/
	(UI_16)138 /* (PCLK_FREQ / 115200 - 1) */		/* 115,200bps			*/
};


/* ����M�o�b�t�@�֘A��� */
GNSS_UART_DRV_LOCAL T_Uart_buf_info uart_buf_info_tbl[GNSS_UART_DRV_UART_CH_COUNT];							/* �e�`�����l�����Ƃ̑���M�o�b�t�@���	*/
GNSS_UART_DRV_LOCAL UI_32           uart_rx_save_count[GNSS_UART_DRV_UART_CH_COUNT];							/* ��M�o�b�t�@�f�[�^�i�[�J�E���g		*/
GNSS_UART_DRV_LOCAL UI_32           uart_rx_load_count[GNSS_UART_DRV_UART_CH_COUNT];							/* ��M�o�b�t�@�f�[�^�Ǐo�J�E���g		*/
GNSS_UART_DRV_LOCAL UI_32           uart_tx_save_count[GNSS_UART_DRV_UART_CH_COUNT];							/* ���M�o�b�t�@�f�[�^�i�[�J�E���g		*/
GNSS_UART_DRV_LOCAL UI_32           uart_tx_load_count[GNSS_UART_DRV_UART_CH_COUNT];							/* ���M�o�b�t�@�f�[�^�Ǐo�J�E���g			*/

/* �����֐��v���g�^�C�v�錾 */
GNSS_UART_DRV_LOCAL void interrupt_rx_common(E_GNSS_UART_DRV_UART_CH uart_ch);
GNSS_UART_DRV_LOCAL void interrupt_tx_common(E_GNSS_UART_DRV_UART_CH uart_ch);
GNSS_UART_DRV_LOCAL void clear_rx_buf(E_GNSS_UART_DRV_UART_CH uart_ch);

/********************************************************/
/* �֐�����		: GNSS UART�h���C�o������				*/
/* �@�\�̐���	: �w��`�����l����UART�h���C�o��������	*/
/*				  ����									*/
/* ����			: (I)uart_ch :UART�`�����l��			*/
/*				  (I)uart_buf_info:����M�o�b�t�@�֘A���*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/26								*/
/* ���l			: 										*/
/********************************************************/
void Gnss_uart_drv_Init(E_GNSS_UART_DRV_UART_CH uart_ch, const T_Uart_buf_info *uart_buf_info)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];

	/* ���̃`�����l���̑���M�o�b�t�@���ݒ� */
	(void)memcpy(&uart_buf_info_tbl[uart_ch], uart_buf_info, sizeof(T_Uart_buf_info));
	uart_rx_save_count[uart_ch] = 0;
	uart_rx_load_count[uart_ch] = 0;
	uart_tx_save_count[uart_ch] = 0;
	uart_tx_load_count[uart_ch] = 0;

    /* UART���Z�b�g */
    uart_reg->SCR.bit.UPCL = 1;
    
	/* ���샂�[�h�ݒ� */
    uart_reg->SMR.bit.MD  = 0;		/* �񓯊����[�h				*/
    uart_reg->SMR.bit.SBL = 0;		/* �X�g�b�v�r�b�g:1bit		*/
    uart_reg->SMR.bit.BDS = 0;		/* LSB�t�@�[�X�g			*/
    uart_reg->SMR.bit.SOE = 1;		/* �o�͋���					*/

    /* �g���ʐM���䃌�W�X�^�̐ݒ� */
    uart_reg->ESCR.bit.ESBL = 0;	/* �X�g�b�v�r�b�g:1bit		*/
    uart_reg->ESCR.bit.INV  = 0;	/* ���]�Ȃ�					*/
    uart_reg->ESCR.bit.PEN  = 0;	/* �p���e�B�Ȃ�				*/
    uart_reg->ESCR.bit.L    = 0;	/* �f�[�^��:8bit			*/
}

/********************************************************/
/* �֐�����		: GNSS UART�h���C�o�J�n����				*/
/* �@�\�̐���	: �w��`�����l����UART�h���C�o�̒ʐM	*/
/*				  �J�n�ݒ���s���B						*/
/* ����			: (I)uart_ch:UART�`�����l��				*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/26								*/
/* ���l			: 										*/
/********************************************************/
void Gnss_uart_drv_Start(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];
	
	/* �{�[���[�g�ݒ� */
	Gnss_uart_drv_Set_bdrate(uart_ch, S_Uart_baudrate[uart_ch]);

	GNSS_uart_drv_Clr_err_stat(uart_ch);

	/* ���䃌�W�X�^�ݒ� */
    uart_reg->SCR.bit.RIE = 1;		/* ��M�����݋���			*/
    uart_reg->SCR.bit.TXE = 1;		/* ���M����					*/

    /* UART���Z�b�g */
    uart_reg->SCR.bit.UPCL = 1;
}

/********************************************************/
/* �֐�����		: GNSS UART�h���C�o��~����				*/
/* �@�\�̐���	: �w��`�����l����UART�h���C�o�̒ʐM	*/
/*				  ��~�ݒ���s���B						*/
/* ����			: (I)uart_ch:UART�`�����l��				*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/26								*/
/* ���l			: 										*/
/********************************************************/
void Gnss_uart_drv_Stop(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];

	/* ���䃌�W�X�^�ݒ� */
    uart_reg->SCR.bit.RIE = 0;		/* ��M�����݋֎~			*/
    uart_reg->SCR.bit.TIE = 0;		/* ���M�����݋֎~			*/
    uart_reg->SCR.bit.RXE = 0;		/* ��M�֎~					*/
    uart_reg->SCR.bit.TXE = 0;		/* ���M�֎~					*/
}

/********************************************************/
/* �֐�����		: ��M��~����							*/
/* �@�\�̐���	: �w��`�����l���̎�M���~����B		*/
/* ����			: (I)uart_ch:UART�`�����l��				*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/26								*/
/* ���l			: 										*/
/********************************************************/
void Gnss_uart_drv_Stop_rx(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];
    uart_reg->SCR.bit.RXE = 0;		/* ��M�֎~					*/
}
/********************************************************/
/* �֐�����		: ��M�J�n����							*/
/* �@�\�̐���	: �w��`�����l���̎�M���J�n����B		*/
/* ����			: (I)uart_ch:UART�`�����l��				*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/26								*/
/* ���l			: 										*/
/********************************************************/
void Gnss_uart_drv_Start_rx(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];
    uart_reg->SCR.bit.RXE = 1;		/* ��M����					*/
}

/********************************************************/
/* �֐�����		: GNSS UART�h���C�o �{�[���[�g�ݒ菈��	*/
/* �@�\�̐���	: �w��`�����l����UART�h���C�o�̒ʐM	*/
/*				  �{�[���[�g�̐ݒ���s���B				*/
/* ����			: (I)uart_ch:UART�`�����l��				*/
/* 				: (I)baudrate:�{�[���[�g				*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/26								*/
/* ���l			: 										*/
/********************************************************/
void Gnss_uart_drv_Set_bdrate(E_GNSS_UART_DRV_UART_CH uart_ch, E_GNSS_BAUDRATE baudrate)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];
	
	Gnss_uart_drv_Stop(uart_ch);

	/* �{�[���[�g�W�F�l���[�^�ݒ� */
	uart_reg->BGR.hword = C_Bgr_value[baudrate];

    /* UART���Z�b�g */
    uart_reg->SCR.bit.UPCL = 1;

	GNSS_uart_drv_Clr_err_stat(uart_ch);

	/* ���䃌�W�X�^�ݒ� */
    uart_reg->SCR.bit.RIE = 1;		/* ��M�����݋���			*/
    uart_reg->SCR.bit.TXE = 1;		/* ���M����					*/

	/* ���݂̃{�[���[�g��ێ����Ă��� */
	S_Uart_baudrate[uart_ch] = baudrate;
}

/********************************************************/
/* �֐�����		: GNSS UART�h���C�o ��M�f�[�^�擾		*/
/* �@�\�̐���	: �w��`�����l���̎�M�ς݃f�[�^���擾	*/
/*				  ����B								*/
/* ����			: (I)uart_ch :UART�`�����l��			*/
/* 				: (O)data_buf:��M�ς݃f�[�^��			*/
/* 				: (O)err_buf :��M�G���[����(NULL��)	*/
/* 				: (I/O)data_len:��M�f�[�^�T�C�Y		*/
/* �߂�l		: ��������								*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/*   len�ɂ�data_buf�����err_buf�̃o�b�t�@�T�C�Y���w�� */
/*	 ���邱�ƁB���^�[�����Alen�ɂ�data_buf�����err_buf */
/*   �Ɋi�[�����f�[�^�T�C�Y���ݒ肳���B				*/
/*   err_buf�ɂ́Adata_buf�Ɋi�[�����e��M�f�[�^���Ƃ�  */
/*   �G���[���(E_GNSS_UART_ERR_STAT)���i�[�����B		*/
/********************************************************/
E_GNSS_UART_DRV_RESULT Gnss_uart_drv_Get_rx_data(E_GNSS_UART_DRV_UART_CH uart_ch, UI_8 *data_buf, UI_8 *err_buf, UI_16 *data_len)
{
	T_Uart_buf_info *buf_info = &uart_buf_info_tbl[uart_ch];
	UI_16 return_size;
	UI_16 cpy_size[2];
	UI_32 data_count;
	UI_16 load_index;
	
	/* ��M�ς݃f�[�^�L���`�F�b�N */
	data_count = (uart_rx_save_count[uart_ch] - uart_rx_load_count[uart_ch]);
	if(data_count == 0) {
		/* ��M�f�[�^�Ȃ� */
		return GNSS_UART_DRV_ERR_NOT_RECIEVE_DATA;
	}
	

	/* ��M�o�b�t�@�t���`�F�b�N */
	if(data_count > (UI_32)buf_info->rx_buf_size) {
		/* ��M�o�b�t�@�N���A */
		clear_rx_buf(uart_ch);
		return GNSS_UART_DRV_ERR_BUFFERFULL;
	}

	/* ����R�[�����ɕԂ��f�[�^�T�C�Y���� */
	return_size = (UI_16)data_count;
	if(return_size > *data_len) {
		return_size = *data_len;
	}

	/* �z�������K�v���ǂ����̔��� */
	load_index = (UI_16)(uart_rx_load_count[uart_ch] % buf_info->rx_buf_size);
	if((load_index + return_size) <= buf_info->rx_buf_size) {
		/* �s�v�Ȃ���memcpy1��œ]�� */
		(void)memcpy(data_buf, &buf_info->rx_buf[load_index], return_size);
		if (err_buf != NULL) {
			(void)memcpy(err_buf,  &buf_info->er_buf[load_index], return_size);
		}
	}
	else {
		/* �o�b�t�@�I�[�܂łƐ擪�����2���memcpy�œ]�� */
		cpy_size[0] = buf_info->rx_buf_size - load_index;
		cpy_size[1] = return_size - cpy_size[0];
		(void)memcpy(data_buf,               &buf_info->rx_buf[load_index], cpy_size[0]);
		(void)memcpy(&data_buf[cpy_size[0]], &buf_info->rx_buf[0],          cpy_size[1]);
		if (err_buf != NULL) {
			(void)memcpy(err_buf,                &buf_info->er_buf[load_index], cpy_size[0]);
			(void)memcpy(&err_buf[cpy_size[0]],  &buf_info->er_buf[0],          cpy_size[1]);
		}
	}
	
	/* ��M�f�[�^�ǂݏo���J�E���g�X�V */
	uart_rx_load_count[uart_ch] += return_size;

	/* �f�[�^�T�C�Y�����^�[�� */
	*data_len = return_size;
	return GNSS_UART_DRV_OK;
}

/********************************************************/
/* �֐�����		: GNSS UART�h���C�o ��M�f�[�^�N���A����*/
/* �@�\�̐���	: �w��`�����l���Ɏ�M�ς݃f�[�^���N���A*/
/*                ����B								*/
/* ����			: (I)uart_ch :UART�`�����l��			*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
void Gnss_uart_drv_Clear_rx_data(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	uart_rx_load_count[uart_ch] = uart_rx_save_count[uart_ch];
}

/********************************************************/
/* �֐�����		: GNSS UART�h���C�o ���M�\�f�[�^���擾*/
/* �@�\�̐���	: �w��`�����l���̑��M�\�ȃf�[�^����	*/
/*				  �擾����B							*/
/* ����			: (I)uart_ch :UART�`�����l��			*/
/* �߂�l		: ���M�\�f�[�^��						*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: 										*/
/********************************************************/
UI_16 Gnss_uart_drv_get_can_set_tx_data_len(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	UI_16	data_count;
	UI_16	data_len;

	/* ���M�f�[�^�o�b�t�@�ɐݒ肳��Ă���f�[�^�����擾 */
	data_count = (UI_16)(uart_tx_save_count[uart_ch] - uart_tx_load_count[uart_ch]);

	/* ���M�\�f�[�^�����擾	*/
	data_len = uart_buf_info_tbl[uart_ch].tx_buf_size - data_count;
	
	return data_len;
}

/********************************************************/
/* �֐�����		: GNSS UART�h���C�o ���M�f�[�^�ݒ�		*/
/* �@�\�̐���	: �w��`�����l���ɑ��M����f�[�^��ݒ�	*/
/*				  ����B								*/
/* ����			: (I)uart_ch :UART�`�����l��			*/
/* 				: (I)data_buf:���M�f�[�^��				*/
/* 				: (I)data_len:���M�f�[�^�T�C�Y			*/
/* �߂�l		: ��������								*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
E_GNSS_UART_DRV_RESULT Gnss_uart_drv_Set_tx_data(E_GNSS_UART_DRV_UART_CH uart_ch, const UI_8 *data_buf, UI_8 data_len)
{
	/* ���̃`�����l���̃��W�X�^���ƃo�b�t�@��񌈒� */
	volatile T_Uart_reg      *uart_reg = C_Uart_reg[uart_ch];
	T_Uart_buf_info *buf_info = &uart_buf_info_tbl[uart_ch];
	UI_16 cpy_size[2];
	UI_32 data_count;
	UI_16 save_index;

	if(data_len > buf_info->tx_buf_size) {
		/* �o�b�t�@�T�C�Y�𒴂��Ă���̂ő��M�s�� */
		return GNSS_UART_DRV_ERR_DATA_SEND;
	}

	/* ���݂̃f�[�^���Ɗi�[�C���f�b�N�X���� */
	data_count = uart_tx_save_count[uart_ch] - uart_tx_load_count[uart_ch];
	save_index = (UI_16)(uart_tx_save_count[uart_ch] % buf_info->tx_buf_size);


	/* ���M�o�b�t�@�I�[�o�[�t���[�`�F�b�N */
	if((data_count + data_len) > (UI_32)buf_info->tx_buf_size) {
		return GNSS_UART_DRV_ERR_BUFFERFULL;
	}
	/* �z�������K�v���ǂ����̔��� */
	if((save_index + data_len) <= buf_info->tx_buf_size) {
		/* �s�v�Ȃ���memcpy1��œ]�� */
		(void)memcpy(&buf_info->tx_buf[save_index], data_buf, (size_t)data_len);
	}
	else {
		/* �o�b�t�@�I�[�܂łƐ擪�����2���memcpy�œ]�� */
		cpy_size[0] = buf_info->tx_buf_size - save_index;
		cpy_size[1] = data_len - cpy_size[0];
		(void)memcpy(&buf_info->tx_buf[save_index], &data_buf[0],           cpy_size[0]);
		(void)memcpy(&buf_info->tx_buf[0],          &data_buf[cpy_size[0]], cpy_size[1]);
	}

	/* ���M�f�[�^�i�[���X�V */
	uart_tx_save_count[uart_ch] += data_len;

    uart_reg->SCR.bit.RXE = 1;      /* ��M����				*/
	uart_reg->SCR.bit.TIE = 1;		/* ���M���荞�݋���		*/
	
	return GNSS_UART_DRV_OK;
}

/********************************************************/
/* �֐�����		: �G���[��Ԏ擾						*/
/* �@�\�̐���	: �w��`�����l���̃G���[��Ԃ��擾����B*/
/* ����			: (I)uart_ch :UART�`�����l��			*/
/* �߂�l		: �G���[���							*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
E_GNSS_UART_ERR_STAT Gnss_uart_drv_Get_err_stat(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];
	
	/* �p���e�B�G���[�L���`�F�b�N */
	if(uart_reg->SSR.bit.PE == 1) {
		return GNSS_UART_ERR_STAT_PE;
	}

	/* �t���[�~���O�G���[�L���`�F�b�N */
	if(uart_reg->SSR.bit.FRE == 1) {
		return GNSS_UART_ERR_STAT_FRE;
	}

	/* �I�[�o�[�����G���[�L���`�F�b�N */
	if(uart_reg->SSR.bit.ORE == 1) {
		return GNSS_UART_ERR_STAT_ORE;
	}

	return GNSS_UART_ERR_STAT_NONE;
}

/********************************************************/
/* �֐�����		: �G���[�t���O�N���A					*/
/* �@�\�̐���	: �w��`�����l���̃G���[�t���O���N���A	*/
/*				  ����B								*/
/* ����			: (I)uart_ch :UART�`�����l��			*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
void GNSS_uart_drv_Clr_err_stat(E_GNSS_UART_DRV_UART_CH uart_ch)
{
	volatile T_Uart_reg *uart_reg = C_Uart_reg[uart_ch];

	/* �G���[�t���O�N���A */
	uart_reg->SSR.bit.REC = 1;
}

/********************************************************/
/* �֐�����		: ��M�o�b�t�@�N���A					*/
/* �@�\�̐���	: ��M�o�b�t�@���N���A����B			*/
/* ����			: (I)uart_ch :UART�`�����l��			*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
void clear_rx_buf(E_GNSS_UART_DRV_UART_CH uart_ch)
{

	/* �ǂݏo�������i�[���ɍ��킹�� */
	uart_rx_load_count[uart_ch] = uart_rx_save_count[uart_ch];

}

/********************************************************/
/* �֐�����		: UART CH0 ��M�����݊֐�				*/
/* �@�\�̐���	: UART CH0�̎�M�����ݏ����֐�			*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_rx0(void)
{
	/* �`�����l�����ʊ֐��R�[�� */
	interrupt_rx_common(GNSS_UART_DRV_UART_CH_0);
}
/********************************************************/
/* �֐�����		: UART CH0 ���M�����݊֐�				*/
/* �@�\�̐���	: UART CH0�̑��M�����ݏ����֐�			*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_tx0(void)
{
	/* �`�����l�����ʊ֐��R�[�� */
	interrupt_tx_common(GNSS_UART_DRV_UART_CH_0);
}

/********************************************************/
/* �֐�����		: UART CH1 ��M�����݊֐�				*/
/* �@�\�̐���	: UART CH1�̎�M�����ݏ����֐�			*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_rx1(void)
{
	/* �`�����l�����ʊ֐��R�[�� */
	interrupt_rx_common(GNSS_UART_DRV_UART_CH_1);
}
/********************************************************/
/* �֐�����		: UART CH1 ���M�����݊֐�				*/
/* �@�\�̐���	: UART CH1�̑��M�����ݏ����֐�			*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_tx1(void)
{
	/* �`�����l�����ʊ֐��R�[�� */
	interrupt_tx_common(GNSS_UART_DRV_UART_CH_1);
}

/********************************************************/
/* �֐�����		: UART CH8 ��M�����݊֐�				*/
/* �@�\�̐���	: UART CH8�̎�M�����ݏ����֐�			*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_rx8(void)
{
	/* �`�����l�����ʊ֐��R�[�� */
	interrupt_rx_common(GNSS_UART_DRV_UART_CH_8);
}
/********************************************************/
/* �֐�����		: UART CH8 ���M�����݊֐�				*/
/* �@�\�̐���	: UART CH8�̑��M�����ݏ����֐�			*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_tx8(void)
{
	/* �`�����l�����ʊ֐��R�[�� */
	interrupt_tx_common(GNSS_UART_DRV_UART_CH_8);
}

/********************************************************/
/* �֐�����		: UART CH9 ��M�����݊֐�				*/
/* �@�\�̐���	: UART CH9�̎�M�����ݏ����֐�			*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_rx9(void)
{
	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_GNSS_INTR_RX);)

	/* �`�����l�����ʊ֐��R�[�� */
	interrupt_rx_common(GNSS_UART_DRV_UART_CH_9);

	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_GNSS_INTR_RX);)
}
/********************************************************/
/* �֐�����		: UART CH9 ���M�����݊֐�				*/
/* �@�\�̐���	: UART CH9�̑��M�����ݏ����֐�			*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
void __interrupt Gnss_uart_drv_Interrupt_tx9(void)
{
	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_GNSS_INTR_TX);)

	/* �`�����l�����ʊ֐��R�[�� */
	interrupt_tx_common(GNSS_UART_DRV_UART_CH_9);

	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_GNSS_INTR_TX);)
}

/********************************************************/
/* �֐�����		: UART�`�����l�����ʎ�M�����݊֐�		*/
/* �@�\�̐���	: UART�e�`�����l�����ʂ̎�M�����ݏ���	*/
/*				  �֐�									*/
/* ����			: (I)uart_ch :UART�`�����l��			*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
void interrupt_rx_common(E_GNSS_UART_DRV_UART_CH uart_ch)
{

	/* ���̃`�����l���̃��W�X�^���ƃo�b�t�@��񌈒� */
	volatile T_Uart_reg      *uart_reg = C_Uart_reg[uart_ch];
	T_Uart_buf_info *buf_info = &uart_buf_info_tbl[uart_ch];
	UI_32 data_count;
	UI_16 save_index;
	volatile UI_8 data_tmp;
	
	/* �f�[�^���Ɗi�[�C���f�b�N�X���� */
	data_count = uart_rx_save_count[uart_ch] - uart_rx_load_count[uart_ch];
	save_index = (UI_16)(uart_rx_save_count[uart_ch] % buf_info->rx_buf_size);

	/* ���Ɏ�M�o�b�t�@�������ς� */
	if(data_count >= (UI_32)buf_info->rx_buf_size) {
		if((uart_reg->SSR.bit.ORE == 1) || (uart_reg->SSR.bit.FRE == 1) || (uart_reg->SSR.bit.PE == 1)) {
			/* �G���[�t���O�N���A */
			uart_reg->SSR.bit.REC = 1;
		}
		else {
			/* ��M�f�[�^�ǂݎ̂�	*/
			data_tmp = uart_reg->RDR.byte.RDRL;
		}
		return;
	}

	/* �I�[�o�[�����G���[�L���`�F�b�N */
	if((uart_reg->SSR.bit.ORE == 1) || (uart_reg->SSR.bit.FRE == 1) || (uart_reg->SSR.bit.PE == 1)) {
		/* �G���[������M�G���[�o�b�t�@�֊i�[ */
		if(uart_reg->SSR.bit.ORE == 1) {
			buf_info->er_buf[save_index] = GNSS_UART_ERR_STAT_ORE;
		}
		else if(uart_reg->SSR.bit.FRE == 1) {
			buf_info->er_buf[save_index] = GNSS_UART_ERR_STAT_FRE;
		}
		else if(uart_reg->SSR.bit.PE == 1) {
			buf_info->er_buf[save_index] = GNSS_UART_ERR_STAT_PE;
		}
		else {
			/* nop */
		}
		buf_info->rx_buf[save_index]  = 0;

		/* �G���[�t���O�N���A */
		uart_reg->SSR.bit.REC = 1;
	}
	else {
		/* ��M�f�[�^�擾���ē����o�b�t�@�֊i�[ */
		buf_info->rx_buf[save_index] = uart_reg->RDR.byte.RDRL;
		buf_info->er_buf[save_index] = GNSS_UART_ERR_STAT_NONE;
	}

	/* ��M�f�[�^�i�[���C���N�������g */
	uart_rx_save_count[uart_ch]++;

}

/********************************************************/
/* �֐�����		: UART�`�����l�����ʑ��M�����݊֐�		*/
/* �@�\�̐���	: UART�e�`�����l�����ʂ̑��M�����ݏ���	*/
/*				  �֐�									*/
/* ����			: (I)uart_ch :UART�`�����l��			*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
void interrupt_tx_common(E_GNSS_UART_DRV_UART_CH uart_ch)
{

	UI_8  byte_data;
	UI_32 data_count;
	UI_16 load_index;
	
	/* ���̃`�����l���̃��W�X�^���ƃo�b�t�@��񌈒� */
	volatile T_Uart_reg      *uart_reg = C_Uart_reg[uart_ch];
	T_Uart_buf_info *buf_info = &uart_buf_info_tbl[uart_ch];

	data_count = uart_tx_save_count[uart_ch] - uart_tx_load_count[uart_ch];
	load_index = (UI_16)(uart_tx_load_count[uart_ch] % buf_info->tx_buf_size);
	
	/* ���M�҂��f�[�^�����݂��邩�ǂ����̔��� */
	if(data_count > 0) {
		/* ���M�f�[�^�𑗐M�f�[�^���W�X�^�ɐݒ� */
		byte_data = buf_info->tx_buf[load_index];

		/* ���M�҂��f�[�^�Ǐo�����X�V */
		uart_tx_load_count[uart_ch]++;

		uart_reg->RDR.byte.RDRL = byte_data;
	}
	else {
    	uart_reg->SCR.bit.TIE = 0;		/* ���M�֎~					*/
	}

}
