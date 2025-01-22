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
#include "DTO459_VDV_Can_Drv.h"
#include "SSFTxxx_FR81_SystemClock_Drv_Config.h"
#include "SSFTSTD_FR81_Port_drv.h"
#include "timer_ctrl.h"
#include "dbglog.h"
#include "time_measure.h"
#define CAN_INT_LEVEL			0xf2	/* CAN�����݃��x��		*/

#define MIN_MESSAGE_NO			 1		/* �ŏ����b�Z�[�W�ԍ�   */
#define MAX_MESSAGE_NO			32		/* �ő僁�b�Z�[�W�ԍ�	*/

#define CAN_DRV_LOCAL	static

/* CAN���W�X�^ */
typedef struct {
	union {
		__HWORD	hword;
		struct {
			__BYTE	CTRLRH;
			__BYTE	CTRLRL;
		} byte;
		struct {
			__HWORD	RESV00:1;
			__HWORD	RESV01:1;
			__HWORD	RESV02:1;
			__HWORD	RESV03:1;
			__HWORD	RESV04:1;
			__HWORD	RESV05:1;
			__HWORD	RESV06:1;
			__HWORD	RESV07:1;
			__HWORD	Test:1;
			__HWORD	CCE:1;
			__HWORD	DAR:1;
			__HWORD	RESV011:1;
			__HWORD	EIE:1;
			__HWORD	SIE:1;
			__HWORD	IE:1;
			__HWORD	Init:1;
		} bit;
	} CTRLR;
	union {
		__HWORD	hword;
		struct {
			__BYTE	STATRH;
			__BYTE	STATRL;
		} byte;
		struct {
			__HWORD	RESV20:1;
			__HWORD	RESV21:1;
			__HWORD	RESV22:1;
			__HWORD	RESV23:1;
			__HWORD	RESV24:1;
			__HWORD	RESV25:1;
			__HWORD	RESV26:1;
			__HWORD	RESV27:1;
			__HWORD	BOff:1;
			__HWORD	EWarn:1;
			__HWORD	EPass:1;
			__HWORD	RxOk:1;
			__HWORD	TxOk:1;
			__HWORD	LEC:3;
		} bit;
	} STATR;
	const union {
		__HWORD	hword;
		struct {
			__BYTE	ERRCNTH;
			__BYTE	ERRCNTL;
		} byte;
		struct {
			__HWORD	RP:1;
			__HWORD	REC:7;
			__HWORD	TEC:8;
		} bit;
	} ERRCNT;
	union {
		__HWORD	hword;
		struct {
			__BYTE	BTRH;
			__BYTE	BTRL;
		} byte;
		struct {
			__HWORD	RESV60:1;
			__HWORD	TSeg2:3;
			__HWORD	TSeg1:4;
			__HWORD	SJW:2;
			__HWORD	BRP:6;
		} bit;
	} BTR;
	const union {
		__HWORD	hword;
		struct {
			__BYTE	INTRH;
			__BYTE	INTRL;
		} byte;
	} INTR;
	union {
		__HWORD	hword;
		struct {
			__BYTE	TESTRH;
			__BYTE	TESTRL;
		} byte;
		struct {
			__HWORD	RESV100:1;
			__HWORD	RESV101:1;
			__HWORD	RESV102:1;
			__HWORD	RESV103:1;
			__HWORD	RESV104:1;
			__HWORD	RESV105:1;
			__HWORD	RESV106:1;
			__HWORD	RESV107:1;
			__HWORD	Rx:1;
			__HWORD	Tx:2;
			__HWORD	LBack:1;
			__HWORD	Silent:1;
			__HWORD	Basic:1;
			__HWORD	RESV1013:1;
			__HWORD	RESV1014:1;
		} bit;
	} TESTR;
	union {
		__HWORD	hword;
		struct {
			__BYTE	BRPERH;
			__BYTE	BRPERL;
		} byte;
		struct {
			__HWORD	RESV120:1;
			__HWORD	RESV121:1;
			__HWORD	RESV122:1;
			__HWORD	RESV123:1;
			__HWORD	RESV124:1;
			__HWORD	RESV125:1;
			__HWORD	RESV126:1;
			__HWORD	RESV127:1;
			__HWORD	RESV128:1;
			__HWORD	RESV129:1;
			__HWORD	RESV1210:1;
			__HWORD	RESV1211:1;
			__HWORD	BRPE:4;
		} bit;
	} BRPER;
	__HWORD	IO_RESV1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1CREQH;
			__BYTE	IF1CREQL;
		} byte;
		struct {
			__HWORD	BUSY:1;
			__HWORD	RESV161:1;
			__HWORD	RESV162:1;
			__HWORD	RESV163:1;
			__HWORD	RESV164:1;
			__HWORD	RESV165:1;
			__HWORD	RESV166:1;
			__HWORD	RESV167:1;
			__HWORD	MN:8;
		} bit;
	} IF1CREQ;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1CMSKH;
			__BYTE	IF1CMSKL;
		} byte;
		struct {
			__HWORD	RESV180:1;
			__HWORD	RESV181:1;
			__HWORD	RESV182:1;
			__HWORD	RESV183:1;
			__HWORD	RESV184:1;
			__HWORD	RESV185:1;
			__HWORD	RESV186:1;
			__HWORD	RESV187:1;
			__HWORD	WR_RD:1;
			__HWORD	Mask:1;
			__HWORD	Arb:1;
			__HWORD	Control:1;
			__HWORD	CIP:1;
			__HWORD	TxReq:1;
			__HWORD	DataA:1;
			__HWORD	DataB:1;
		} bit;
	} IF1CMSK;
	union {
		__WORD	word;
		struct {
			__HWORD	IF1MSK2;
			__HWORD	IF1MSK1;
		} hword;
		struct {
			__BYTE	IF1MSK2H;
			__BYTE	IF1MSK2L;
			__BYTE	IF1MSK1H;
			__BYTE	IF1MSK1L;
		} byte;
		struct {
			__WORD	MXtd:1;
			__WORD	MDir:1;
			__WORD	RESV202:1;
			__WORD	Msk:29;
		} bit;
	} IF1MSK;
	union {
		__WORD	word;
		struct {
			__HWORD	IF1ARB2;
			__HWORD	IF1ARB1;
		} hword;
		struct {
			__BYTE	IF1ARB2H;
			__BYTE	IF1ARB2L;
			__BYTE	IF1ARB1H;
			__BYTE	IF1ARB1L;
		} byte;
		struct {
			__WORD	MsgVal:1;
			__WORD	Xtd:1;
			__WORD	Dir:1;
			__WORD	ID:29;
		} bit;
	} IF1ARB;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1MCTRH;
			__BYTE	IF1MCTRL;
		} byte;
		struct {
			__HWORD	NewDat:1;
			__HWORD	MsgLst:1;
			__HWORD	IntPnd:1;
			__HWORD	UMask:1;
			__HWORD	TxIE:1;
			__HWORD	RxIE:1;
			__HWORD	RmtEn:1;
			__HWORD	TxRqst:1;
			__HWORD	EoB:1;
			__HWORD	RESV289:1;
			__HWORD	RESV2810:1;
			__HWORD	RESV2811:1;
			__HWORD	DLC:4;
		} bit;
	} IF1MCTR;
	__HWORD	IO_RESV2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTA1H;
			__BYTE	IF1DTA1L;
		} byte;
	} IF1DTA1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTA2H;
			__BYTE	IF1DTA2L;
		} byte;
	} IF1DTA2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTB1H;
			__BYTE	IF1DTB1L;
		} byte;
	} IF1DTB1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTB2H;
			__BYTE	IF1DTB2L;
		} byte;
	} IF1DTB2;
	__WORD	IO_RESV3[CONSTANT_2];
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTA2_H;
			__BYTE	IF1DTA2_L;
		} byte;
	} IF1DTA2_;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTA1_H;
			__BYTE	IF1DTA1_L;
		} byte;
	} IF1DTA1_;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTB2_H;
			__BYTE	IF1DTB2_L;
		} byte;
	} IF1DTB2_;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTB1_H;
			__BYTE	IF1DTB1_L;
		} byte;
	} IF1DTB1_;
	__WORD	IO_RESV4[CONSTANT_2];
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2CREQH;
			__BYTE	IF2CREQL;
		} byte;
		struct {
			__HWORD	BUSY:1;
			__HWORD	RESV641:1;
			__HWORD	RESV642:1;
			__HWORD	RESV643:1;
			__HWORD	RESV644:1;
			__HWORD	RESV645:1;
			__HWORD	RESV646:1;
			__HWORD	RESV647:1;
			__HWORD	MN:8;
		} bit;
	} IF2CREQ;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2CMSKH;
			__BYTE	IF2CMSKL;
		} byte;
		struct {
			__HWORD	RESV660:1;
			__HWORD	RESV661:1;
			__HWORD	RESV662:1;
			__HWORD	RESV663:1;
			__HWORD	RESV664:1;
			__HWORD	RESV665:1;
			__HWORD	RESV666:1;
			__HWORD	RESV667:1;
			__HWORD	WR_RD:1;
			__HWORD	Mask:1;
			__HWORD	Arb:1;
			__HWORD	Control:1;
			__HWORD	CIP:1;
			__HWORD	TxReq:1;
			__HWORD	DataA:1;
			__HWORD	DataB:1;
		} bit;
	} IF2CMSK;
	union {
		__WORD	word;
		struct {
			__HWORD	IF2MSK2;
			__HWORD	IF2MSK1;
		} hword;
		struct {
			__BYTE	IF2MSK2H;
			__BYTE	IF2MSK2L;
			__BYTE	IF2MSK1H;
			__BYTE	IF2MSK1L;
		} byte;
		struct {
			__WORD	MXtd:1;
			__WORD	MDir:1;
			__WORD	RESV682:1;
			__WORD	Msk:29;
		} bit;
	} IF2MSK;
	union {
		__WORD	word;
		struct {
			__HWORD	IF2ARB2;
			__HWORD	IF2ARB1;
		} hword;
		struct {
			__BYTE	IF2ARB2H;
			__BYTE	IF2ARB2L;
			__BYTE	IF2ARB1H;
			__BYTE	IF2ARB1L;
		} byte;
		struct {
			__WORD	MsgVal:1;
			__WORD	Xtd:1;
			__WORD	Dir:1;
			__WORD	ID:29;
		} bit;
	} IF2ARB;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2MCTRH;
			__BYTE	IF2MCTRL;
		} byte;
		struct {
			__HWORD	NewDat:1;
			__HWORD	MsgLst:1;
			__HWORD	IntPnd:1;
			__HWORD	UMask:1;
			__HWORD	TxIE:1;
			__HWORD	RxIE:1;
			__HWORD	RmtEn:1;
			__HWORD	TxRqst:1;
			__HWORD	EoB:1;
			__HWORD	RESV769:1;
			__HWORD	RESV7610:1;
			__HWORD	RESV7611:1;
			__HWORD	DLC:4;
		} bit;
	} IF2MCTR;
	__HWORD	IO_RESV5;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTA1H;
			__BYTE	IF2DTA1L;
		} byte;
	} IF2DTA1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTA2H;
			__BYTE	IF2DTA2L;
		} byte;
	} IF2DTA2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTB1H;
			__BYTE	IF2DTB1L;
		} byte;
	} IF2DTB1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTB2H;
			__BYTE	IF2DTB2L;
		} byte;
	} IF2DTB2;
	__WORD	IO_RESV6[CONSTANT_2];
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTA2_H;
			__BYTE	IF2DTA2_L;
		} byte;
	} IF2DTA2_;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTA1_H;
			__BYTE	IF2DTA1_L;
		} byte;
	} IF2DTA1_;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTB2_H;
			__BYTE	IF2DTB2_L;
		} byte;
	} IF2DTB2_;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTB1_H;
			__BYTE	IF2DTB1_L;
		} byte;
	} IF2DTB1_;
	__WORD	IO_RESV7[CONSTANT_6];
	const union {
		__WORD	word;
		struct {
			__HWORD	TREQR2;
			__HWORD	TREQR1;
		} hword;
		struct {
			__BYTE	TREQR2H;
			__BYTE	TREQR2L;
			__BYTE	TREQR1H;
			__BYTE	TREQR1L;
		} byte;
	} TREQR12;
	const union {
		__WORD	word;
		struct {
			__HWORD	TREQR4;
			__HWORD	TREQR3;
		} hword;
		struct {
			__BYTE	TREQR4H;
			__BYTE	TREQR4L;
			__BYTE	TREQR3H;
			__BYTE	TREQR3L;
		} byte;
	} TREQR34;
	const union {
		__WORD	word;
		struct {
			__HWORD	TREQR6;
			__HWORD	TREQR5;
		} hword;
		struct {
			__BYTE	TREQR6H;
			__BYTE	TREQR6L;
			__BYTE	TREQR5H;
			__BYTE	TREQR5L;
		} byte;
	} TREQR56;
	const union {
		__WORD	word;
		struct {
			__HWORD	TREQR8;
			__HWORD	TREQR7;
		} hword;
		struct {
			__BYTE	TREQR8H;
			__BYTE	TREQR8L;
			__BYTE	TREQR7H;
			__BYTE	TREQR7L;
		} byte;
	} TREQR78;
	const union {
		__WORD	word;
		struct {
			__HWORD	NEWDT2;
			__HWORD	NEWDT1;
		} hword;
		struct {
			__BYTE	NEWDT2H;
			__BYTE	NEWDT2L;
			__BYTE	NEWDT1H;
			__BYTE	NEWDT1L;
		} byte;
	} NEWDT12;
	const union {
		__WORD	word;
		struct {
			__HWORD	NEWDT4;
			__HWORD	NEWDT3;
		} hword;
		struct {
			__BYTE	NEWDT4H;
			__BYTE	NEWDT4L;
			__BYTE	NEWDT3H;
			__BYTE	NEWDT3L;
		} byte;
	} NEWDT34;
	const union {
		__WORD	word;
		struct {
			__HWORD	NEWDT6;
			__HWORD	NEWDT5;
		} hword;
		struct {
			__BYTE	NEWDT6H;
			__BYTE	NEWDT6L;
			__BYTE	NEWDT5H;
			__BYTE	NEWDT5L;
		} byte;
	} NEWDT56;
	const union {
		__WORD	word;
		struct {
			__HWORD	NEWDT8;
			__HWORD	NEWDT7;
		} hword;
		struct {
			__BYTE	NEWDT8H;
			__BYTE	NEWDT8L;
			__BYTE	NEWDT7H;
			__BYTE	NEWDT7L;
		} byte;
	} NEWDT78;
	const union {
		__WORD	word;
		struct {
			__HWORD	INTPND2;
			__HWORD	INTPND1;
		} hword;
		struct {
			__BYTE	INTPND2H;
			__BYTE	INTPND2L;
			__BYTE	INTPND1H;
			__BYTE	INTPND1L;
		} byte;
	} INTPND12;
	const union {
		__WORD	word;
		struct {
			__HWORD	INTPND4;
			__HWORD	INTPND3;
		} hword;
		struct {
			__BYTE	INTPND4H;
			__BYTE	INTPND4L;
			__BYTE	INTPND3H;
			__BYTE	INTPND3L;
		} byte;
	} INTPND34;
	const union {
		__WORD	word;
		struct {
			__HWORD	INTPND6;
			__HWORD	INTPND5;
		} hword;
		struct {
			__BYTE	INTPND6H;
			__BYTE	INTPND6L;
			__BYTE	INTPND5H;
			__BYTE	INTPND5L;
		} byte;
	} INTPND56;
	const union {
		__WORD	word;
		struct {
			__HWORD	INTPND8;
			__HWORD	INTPND7;
		} hword;
		struct {
			__BYTE	INTPND8H;
			__BYTE	INTPND8L;
			__BYTE	INTPND7H;
			__BYTE	INTPND7L;
		} byte;
	} INTPND78;
	const union {
		__WORD	word;
		struct {
			__HWORD	MSGVAL2;
			__HWORD	MSGVAL1;
		} hword;
		struct {
			__BYTE	MSGVAL2H;
			__BYTE	MSGVAL2L;
			__BYTE	MSGVAL1H;
			__BYTE	MSGVAL1L;
		} byte;
	} MSGVAL12;
	const union {
		__WORD	word;
		struct {
			__HWORD	MSGVAL4;
			__HWORD	MSGVAL3;
		} hword;
		struct {
			__BYTE	MSGVAL4H;
			__BYTE	MSGVAL4L;
			__BYTE	MSGVAL3H;
			__BYTE	MSGVAL3L;
		} byte;
	} MSGVAL34;
	const union {
		__WORD	word;
		struct {
			__HWORD	MSGVAL6;
			__HWORD	MSGVAL5;
		} hword;
		struct {
			__BYTE	MSGVAL6H;
			__BYTE	MSGVAL6L;
			__BYTE	MSGVAL5H;
			__BYTE	MSGVAL5L;
		} byte;
	} MSGVAL56;
	const union {
		__WORD	word;
		struct {
			__HWORD	MSGVAL8;
			__HWORD	MSGVAL7;
		} hword;
		struct {
			__BYTE	MSGVAL8H;
			__BYTE	MSGVAL8L;
			__BYTE	MSGVAL7H;
			__BYTE	MSGVAL7L;
		} byte;
	} MSGVAL78;
} T_Can_reg;


/* CAN���W�X�^ */
CAN_DRV_LOCAL __io volatile T_Can_reg *const C_Can_reg[CAN_CH_COUNT] = {
	(volatile T_Can_reg*)&IO_CAN0, (volatile T_Can_reg*)&IO_CAN1, (volatile T_Can_reg*)&IO_CAN2
};

#define CAN_RING_BUF_MAX		50					/* CAN��M�f�[�^�����O�o�b�t�@��				*/

typedef struct {
	T_Can_data	can_data[CAN_RING_BUF_MAX];			/* CAN��M�f�[�^�����O�o�b�t�@					*/
	UI_32		count_w;							/* CAN��M�f�[�^�����O�o�b�t�@Write�J�E���g		*/
	UI_32		count_r;							/* CAN��M�f�[�^�����O�o�b�t�@Read�J�E���g		*/
} T_Rx_buffer;

/* �����ϐ���` */
CAN_DRV_LOCAL T_Rx_buffer S_Rx_buffer[CAN_CH_COUNT];		/* CAN��M�f�[�^�����O�o�b�t�@					*/
CAN_DRV_LOCAL UI_8 S_can_drv_initialized = 0;					/* CAN�h���C�o�������ς݃t���O					*/

/* �����֐��v���g�^�C�v�錾 */
CAN_DRV_LOCAL void init_message_buffer(E_CAN_CH_NO can_ch_no, UI_8 send_buf_cnt);
CAN_DRV_LOCAL void interrupt_common(E_CAN_CH_NO can_ch_no);
CAN_DRV_LOCAL void interrupt_status(E_CAN_CH_NO can_ch_no);
CAN_DRV_LOCAL void interrupt_tx_rx(E_CAN_CH_NO can_ch_no);
CAN_DRV_LOCAL E_RC save_rx_data(E_CAN_CH_NO can_ch_no, UI_32 msg_id, const UI_16 *rx_data, UI_8 rx_data_len);

/********************************************************/
/* �֐�����		: CAN�h���C�o������						*/
/* �@�\�̐���	: CAN�h���C�o������������				*/
/*				  ����									*/
/* ����			: void									*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/11								*/
/* ���l			: 										*/
/********************************************************/
void Can_drv_init(void)
{
	UI_8 p112;
	UI_8 p124;

	/* Local CAN �X�^���o�C���� */
	(void)PortDrv_GetDataBit( PORTDRV_PORT_11,PORTDRV_BIT_2, &p112);
	if(p112 != 0) {
		(void)PortDrv_SetDataBit( PORTDRV_PORT_11,PORTDRV_BIT_2, 0);
	}

	/* �ԗ�CAN �X�^���o�C���� */
	(void)PortDrv_GetDataBit( PORTDRV_PORT_12,PORTDRV_BIT_4, &p124);
	if(p124 != 0) {
		(void)PortDrv_SetDataBit( PORTDRV_PORT_12,PORTDRV_BIT_4, 0);
	}

    /* �v���X�P�[���ݒ� */
    IO_CANPRE.byte = 0x0C;       /* CAN_Interface_Prescale 1/5 (16MHz = 80MHz/5) */

	S_can_drv_initialized = 1;
}

/********************************************************/
/* �֐�����		: CAN�`�����l��������					*/
/* �@�\�̐���	: �w��`�����l��������������B			*/
/* ����			: (I)can_ch_no:CAN�`�����l���ԍ�		*/
/*				  (I)send_buf_cnt:���M�o�b�t�@�� 		*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/11								*/
/* ���l			: 										*/
/*   send_buffer_count�ɂ͑��M�Ɏg�p����o�b�t�@����	*/
/*   �w�肷��B											*/
/*   �o�b�t�@���͑S����32�̂��߁A(32 - send_buf_cnt)��*/
/*   ��M�p�o�b�t�@���ƂȂ�B 							*/
/********************************************************/
void Can_drv_ch_init(E_CAN_CH_NO can_ch_no, UI_8 send_buf_cnt)
{
	/* CAN�h���C�o���������Ȃ珉�������s�� */
	if(S_can_drv_initialized != 1) {
		Can_drv_init();
	}

	/* ��M�f�[�^�����O�o�b�t�@������ */
	(void)memset(&S_Rx_buffer[can_ch_no], 0x00, sizeof(T_Rx_buffer));

	/* CAN�R���g���[����~ */
	C_Can_reg[can_ch_no]->CTRLR.bit.Init = 1;

	/* ���b�Z�[�W�o�b�t�@�̏����ݒ� */
	init_message_buffer(can_ch_no, send_buf_cnt);

	/* CAN�o�X�̐ݒ� */
    C_Can_reg[can_ch_no]->CTRLR.hword = 0x0041;        /* BTR/BRPE �������݋��� */
    C_Can_reg[can_ch_no]->BTR.hword   = 0x2B41;        /* 500kbps */
    C_Can_reg[can_ch_no]->BRPER.hword = 0x0000;        
    C_Can_reg[can_ch_no]->CTRLR.hword = 0x0001;        /* BTR/BRPE �������݋֎~ */

	C_Can_reg[can_ch_no]->CTRLR.bit.EIE  = 1;			/* �G���[���荞�݋���	*/
	C_Can_reg[can_ch_no]->CTRLR.bit.IE   = 1;			/* ���荞�݋���			*/

	C_Can_reg[can_ch_no]->CTRLR.bit.DAR   = 0;			/* �����đ�����			*/


	C_Can_reg[can_ch_no]->CTRLR.bit.Init = 1;			/* ������				*/

    /* CAN�R���g���[���J�n */
	C_Can_reg[can_ch_no]->CTRLR.bit.Init = 0;
	while(C_Can_reg[can_ch_no]->CTRLR.bit.Init == 1) {
		/* nop */
	}
	return;
}

/********************************************************/
/* �֐�����		: ���b�Z�[�W�o�b�t�@����������			*/
/* �@�\�̐���	: CAN�R���g���[���̃��b�Z�[�W�o�b�t�@��	*/
/*				  ����������B							*/
/* ����			: (I)can_ch_no:CAN�`�����l���ԍ�		*/
/*				  (I)send_buf_cnt:���M�o�b�t�@�� 		*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/11								*/
/* ���l			: 										*/
/*	���M�p��IF1�A���b�Z�[�W�ԍ�1�`send_buf_cnt���g�p����B*/
/*	��M�p��IF2�A���b�Z�[�W�ԍ�(send_buf_cnt + 1)�`32��	*/
/*  �g�p����B											*/
/********************************************************/
CAN_DRV_LOCAL void init_message_buffer(E_CAN_CH_NO can_ch_no, UI_8 send_buf_cnt)
{

	UI_8 msg_no;	/* ���b�Z�[�W�ԍ� */

	/* ���M�o�b�t�@���̎w��t�F�[���Z�[�t */
	if(send_buf_cnt > MAX_MESSAGE_NO) {
		send_buf_cnt = MAX_MESSAGE_NO;
	}

	/********************************/
	/* ���M���b�Z�[�W�o�b�t�@�̐ݒ� */
	/********************************/

	for(msg_no = 1; msg_no <= send_buf_cnt; msg_no++) {
	   	/* �R�}���h�}�X�N���W�X�^�ݒ� */
		C_Can_reg[can_ch_no]->IF1CMSK.hword = 0x0000;
		C_Can_reg[can_ch_no]->IF1CMSK.bit.WR_RD   = 1;
		C_Can_reg[can_ch_no]->IF1CMSK.bit.Mask    = 1;
		C_Can_reg[can_ch_no]->IF1CMSK.bit.Arb     = 1;

		/* �A�[�r�g���[�V�������W�X�^�ݒ� */
		C_Can_reg[can_ch_no]->IF1ARB.word = 0x00000000;
		C_Can_reg[can_ch_no]->IF1ARB.bit.MsgVal = 1;
		C_Can_reg[can_ch_no]->IF1ARB.bit.Dir    = 1;

	    /* �f�[�^���W�X�^�N���A */
		C_Can_reg[can_ch_no]->IF1DTA1.hword = 0x0000;
		C_Can_reg[can_ch_no]->IF1DTA2.hword = 0x0000;
		C_Can_reg[can_ch_no]->IF1DTB1.hword = 0x0000;
		C_Can_reg[can_ch_no]->IF1DTB2.hword = 0x0000;

	    /* ���b�Z�[�W�ԍ��ݒ� */
	    C_Can_reg[can_ch_no]->IF1CREQ.hword = msg_no;
	}
	
	/********************************/
	/* ��M���b�Z�[�W�o�b�t�@�̐ݒ� */
	/********************************/

	for(msg_no = send_buf_cnt + 1; msg_no <= MAX_MESSAGE_NO; msg_no++) {
    	/* �R�}���h�}�X�N���W�X�^�ݒ� */
    	C_Can_reg[can_ch_no]->IF2CMSK.hword = 0x0000;
		C_Can_reg[can_ch_no]->IF2CMSK.bit.WR_RD   = 1;
		C_Can_reg[can_ch_no]->IF2CMSK.bit.Mask    = 1;
		C_Can_reg[can_ch_no]->IF2CMSK.bit.Arb     = 1;
		C_Can_reg[can_ch_no]->IF2CMSK.bit.Control = 1;
    
		/* �}�X�N���W�X�^�ݒ� */
    	C_Can_reg[can_ch_no]->IF2MSK.word = 0x00000000;
    	C_Can_reg[can_ch_no]->IF2MSK.bit.MXtd    = 1;
    	C_Can_reg[can_ch_no]->IF2MSK.bit.MDir    = 1;
    	C_Can_reg[can_ch_no]->IF2MSK.bit.RESV682 = 1;
		C_Can_reg[can_ch_no]->IF2MSK.word        = ((C_Can_reg[can_ch_no]->IF2MSK.word & 0xE0000000UL) | 0x1FFC0000UL); /* MID28-18 �I�[��1 */
   	
		/* �A�[�r�g���[�V�������W�X�^�ݒ� */
    	C_Can_reg[can_ch_no]->IF2ARB.word = 0x00000000;
		C_Can_reg[can_ch_no]->IF2ARB.bit.MsgVal = 0;
		C_Can_reg[can_ch_no]->IF2ARB.bit.ID     = 0;			/* ���b�Z�[�WID�͌ォ��Đݒ肷�� */
    
		/* ���b�Z�[�W���䃌�W�X�^�ݒ� */
    	C_Can_reg[can_ch_no]->IF2MCTR.hword = 0x0000;
    	C_Can_reg[can_ch_no]->IF2MCTR.bit.UMask = 1;
    	C_Can_reg[can_ch_no]->IF2MCTR.bit.RxIE  = 1;
    	C_Can_reg[can_ch_no]->IF2MCTR.bit.EoB   = 1;
    	C_Can_reg[can_ch_no]->IF2MCTR.bit.DLC   = 8;			/* �f�[�^�t���[����8�o�C�g	*/
    	
    	/* ���b�Z�[�W�ԍ��������݁i�R�}���h�v���j */
    	C_Can_reg[can_ch_no]->IF2CREQ.hword = msg_no;
	}

	return;
}

/********************************************************/
/* �֐�����		: ���b�Z�[�W�o�b�t�@�̑Ώۃ��b�Z�[�WID	*/
/*				  �ύX									*/
/* �@�\�̐���	: msg_num�Ŏw�肳��郁�b�Z�[�W�o�b�t�@*/
/*				  �̎�e���b�Z�[�WID��ύX����B		*/
/* ����			: (I)can_ch_no:CAN�`�����l���ԍ�		*/
/*   			  (I)msg_no :�ύX�Ώۃ��b�Z�[�W�ԍ�		*/
/*   			  (I)msg_id :��e���b�Z�[�WID			*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/11								*/
/* ���l			: 										*/
/*	�i�Ȃ��j											*/
/********************************************************/
void Can_drv_change_message_buffer(E_CAN_CH_NO can_ch_no, UI_8 msg_no, UI_32 msg_id)
{
	/* �R�}���h�}�X�N���W�X�^�ݒ� */
	C_Can_reg[can_ch_no]->IF2CMSK.hword = 0x0000;
	C_Can_reg[can_ch_no]->IF2CMSK.bit.WR_RD = 1;
	C_Can_reg[can_ch_no]->IF2CMSK.bit.Arb   = 1;
	C_Can_reg[can_ch_no]->IF2CMSK.bit.Mask  = 1;

	/* �A�[�r�g���[�V�������W�X�^�ݒ� */
	C_Can_reg[can_ch_no]->IF2ARB.word = 0x00000000;
	C_Can_reg[can_ch_no]->IF2ARB.bit.MsgVal = 1;
	if((msg_id & 0xFFFF0000UL) == 0){
		/* �W���t���[����ID28-ID18���g�p���� */
		msg_id = msg_id << 18;
		C_Can_reg[can_ch_no]->IF2MSK.word    = ((C_Can_reg[can_ch_no]->IF2MSK.word & 0xE0000000UL) | 0x1FFC0000UL); /* MID28-18 �I�[��1 */
		C_Can_reg[can_ch_no]->IF2ARB.bit.Xtd = 0;
	}
	else {
		C_Can_reg[can_ch_no]->IF2MSK.word    = ((C_Can_reg[can_ch_no]->IF2MSK.word & 0xE0000000UL) | 0x1FFFFFFFUL); /* MID28-0 �I�[��1 */
		C_Can_reg[can_ch_no]->IF2ARB.bit.Xtd = 1;
	}
	C_Can_reg[can_ch_no]->IF2ARB.bit.ID = msg_id & 0x1FFFFFFFUL;	/* ���b�Z�[�WID�ݒ� */
	
	/* ���b�Z�[�W�ԍ��������݁i�R�}���h�v���j */
	C_Can_reg[can_ch_no]->IF2CREQ.hword = msg_no;
}

/********************************************************/
/* �֐�����		: ���b�Z�[�W�o�b�t�@�̑Ώۃ��b�Z�[�WID	*/
/*				  �ύX(���b�Z�[�WID�̃}�X�N�w���)		*/
/* �@�\�̐���	: msg_num�Ŏw�肳��郁�b�Z�[�W�o�b�t�@	*/
/*				  �̎�e���b�Z�[�WID��ύX����B		*/
/* ����			: (I)can_ch_no:CAN�`�����l���ԍ�		*/
/*   			  (I)msg_no :�ύX�Ώۃ��b�Z�[�W�ԍ�		*/
/*   			  (I)msg_id :��e���b�Z�[�WID			*/
/*   			  (I)mask   :�}�X�N						*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 16/08/11								*/
/* ���l			: 										*/
/*	�i�Ȃ��j											*/
/********************************************************/
void Can_drv_change_message_buffer_mask(E_CAN_CH_NO can_ch_no, UI_8 msg_no, UI_32 msg_id, UI_32 mask)
{
	/* �R�}���h�}�X�N���W�X�^�ݒ� */
	C_Can_reg[can_ch_no]->IF2CMSK.hword = 0x0000;
	C_Can_reg[can_ch_no]->IF2CMSK.bit.WR_RD = 1;
	C_Can_reg[can_ch_no]->IF2CMSK.bit.Arb   = 1;
	C_Can_reg[can_ch_no]->IF2CMSK.bit.Mask  = 1;

	/* �A�[�r�g���[�V�������W�X�^�ݒ� */
	C_Can_reg[can_ch_no]->IF2ARB.word = 0x00000000;
	if (msg_id == 0) {
		/* �w�肳�ꂽ���b�Z�[�W�ԍ��𖢎g�p�ɂ��� */
		C_Can_reg[can_ch_no]->IF2ARB.bit.MsgVal = 0;
		C_Can_reg[can_ch_no]->IF2ARB.bit.ID = 0;
	}
	else {
		C_Can_reg[can_ch_no]->IF2ARB.bit.MsgVal = 1;
		if((msg_id & 0xFFFF0000UL) == 0){
			/* �W���t���[����ID28-ID18���g�p���� */
			msg_id = msg_id << 18;
			mask = mask << 18;
			C_Can_reg[can_ch_no]->IF2MSK.bit.Msk     = 0x1FFC0000UL & mask;	/* MID28-18 ���}�X�N */
			C_Can_reg[can_ch_no]->IF2ARB.bit.Xtd = 0;
		}
		else {
	    	C_Can_reg[can_ch_no]->IF2MSK.bit.Msk     = 0x1FFFFFFFUL & mask;	/* MID28-0 ���}�X�N */
			C_Can_reg[can_ch_no]->IF2ARB.bit.Xtd = 1;
		}
		C_Can_reg[can_ch_no]->IF2ARB.bit.ID = msg_id & 0x1FFFFFFFUL;	/* ���b�Z�[�WID�ݒ� */
	}

	/* ���b�Z�[�W�ԍ��������݁i�R�}���h�v���j */
	C_Can_reg[can_ch_no]->IF2CREQ.hword = msg_no;
}

/********************************************************/
/* �֐�����		: ���M�f�[�^�̐ݒ�						*/
/* �@�\�̐���	: ���M����f�[�^�𑗐M�p�o�b�t�@�ɐݒ�	*/
/*				  ���A����𑗐M����B					*/
/* ����			: (I)can_ch_no:CAN�`�����l���ԍ�		*/
/*   			  (I)msg_no   :���M���b�Z�[�W�ԍ�		*/
/*   			  (I)msg_id   :���M���b�Z�[�WID			*/
/*   			  (I)msg_data :���M�f�[�^				*/
/*   			  (I)msg_data_len :���M�f�[�^��			*/
/* �߂�l		: ��������(E_RC_OK/E_RC_NG)				*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/23								*/
/* ���l			: 										*/
/*	�i�Ȃ��j											*/
/********************************************************/
E_RC Can_drv_set_tx_data(E_CAN_CH_NO can_ch_no, UI_8 msg_no, UI_32 msg_id, const UI_8 *msg_data, UI_8 msg_data_len)
{
	UI_8 i;
	
	if(msg_data_len > 8) {
		return E_RC_NG;
	}

	/* �R�}���h�}�X�N���W�X�^�ݒ� */
	C_Can_reg[can_ch_no]->IF1CMSK.hword = 0x0000;
	C_Can_reg[can_ch_no]->IF1CMSK.bit.WR_RD   = 1;
	C_Can_reg[can_ch_no]->IF1CMSK.bit.Arb     = 1;
	C_Can_reg[can_ch_no]->IF1CMSK.bit.Control = 1;
	C_Can_reg[can_ch_no]->IF1CMSK.bit.TxReq   = 1;
	C_Can_reg[can_ch_no]->IF1CMSK.bit.DataA   = 1;
	C_Can_reg[can_ch_no]->IF1CMSK.bit.DataB   = 1;

	/* �A�[�r�g���[�V�������W�X�^�̐ݒ� */
	C_Can_reg[can_ch_no]->IF1ARB.bit.MsgVal = 1;
	C_Can_reg[can_ch_no]->IF1ARB.bit.Dir    = 1;
	if((msg_id & 0xFFFF0000UL) == 0){
		/* �W���t���[����ID28-ID18���g�p���� */
		msg_id = msg_id << 18;
		C_Can_reg[can_ch_no]->IF1ARB.bit.Xtd = 0;
	}
	else {
		C_Can_reg[can_ch_no]->IF1ARB.bit.Xtd = 1;
	}
	C_Can_reg[can_ch_no]->IF1ARB.bit.ID = msg_id & 0x1FFFFFFFUL;	/* ���b�Z�[�WID�ݒ� */
	
    /* �f�[�^���W�X�^�ݒ� */
    for(i = 0 ; i < msg_data_len; i++) {
	    switch(i) {
	    case 0:
			C_Can_reg[can_ch_no]->IF1DTA1.byte.IF1DTA1H = msg_data[i];
			break;
	    case 1:
			C_Can_reg[can_ch_no]->IF1DTA1.byte.IF1DTA1L = msg_data[i];
			break;
	    case 2:
			C_Can_reg[can_ch_no]->IF1DTA2.byte.IF1DTA2H = msg_data[i];
			break;
	    case 3:
			C_Can_reg[can_ch_no]->IF1DTA2.byte.IF1DTA2L = msg_data[i];
			break;
	    case 4:
			C_Can_reg[can_ch_no]->IF1DTB1.byte.IF1DTB1H = msg_data[i];
			break;
	    case 5:
			C_Can_reg[can_ch_no]->IF1DTB1.byte.IF1DTB1L = msg_data[i];
			break;
	    case 6:
			C_Can_reg[can_ch_no]->IF1DTB2.byte.IF1DTB2H = msg_data[i];
			break;
	    case 7:
			C_Can_reg[can_ch_no]->IF1DTB2.byte.IF1DTB2L = msg_data[i];
			break;
		default:
			break;
		}
	}
	/* ���b�Z�[�W���䃌�W�X�^�ݒ� */
    C_Can_reg[can_ch_no]->IF1MCTR.hword = 0x0000;
    C_Can_reg[can_ch_no]->IF1MCTR.bit.IntPnd = 1;
    C_Can_reg[can_ch_no]->IF1MCTR.bit.TxIE   = 1;
    C_Can_reg[can_ch_no]->IF1MCTR.bit.TxRqst = 1;
    C_Can_reg[can_ch_no]->IF1MCTR.bit.EoB    = 1;
    C_Can_reg[can_ch_no]->IF1MCTR.bit.DLC    = msg_data_len;		/* �f�[�^���ݒ� */

	/* ���b�Z�[�W�ԍ��������݁i�R�}���h�v���j */
    C_Can_reg[can_ch_no]->IF1CREQ.bit.MN = msg_no;

	return E_RC_OK;
}

/********************************************************/
/* �֐�����		: CAN 0 �����݊֐�						*/
/* �@�\�̐���	: CAN 0�̑���M�����݊֐�				*/
/* ����			: �Ȃ�									*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/23								*/
/* ���l			: 										*/
/*	�i�Ȃ��j											*/
/********************************************************/
void __interrupt CanDrv_interrupt_can_0(void)
{

	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_CAN_INTR_CH0);)

	/* �����݋��ʊ֐��R�[�� */
	interrupt_common(CAN_CH_0);

	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_CAN_INTR_CH0);)

}

/********************************************************/
/* �֐�����		: CAN 1�����݊֐�						*/
/* �@�\�̐���	: CAN 1�̑���M�����݊֐�				*/
/* ����			: �Ȃ�									*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/23								*/
/* ���l			: 										*/
/*	�i�Ȃ��j											*/
/********************************************************/
void __interrupt CanDrv_interrupt_can_1(void)
{

	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_CAN_INTR_CH1);)

	/* �����݋��ʊ֐��R�[�� */
	interrupt_common(CAN_CH_1);

	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_CAN_INTR_CH1);)

}

/********************************************************/
/* �֐�����		: CAN 2�����݊֐�						*/
/* �@�\�̐���	: CAN 2����M�����݊֐�					*/
/* ����			: �Ȃ�									*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/23								*/
/* ���l			: 										*/
/*	�i�Ȃ��j											*/
/********************************************************/
void __interrupt CanDrv_interrupt_can_2(void)
{

	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_CAN_INTR_CH2);)

	/* �����݋��ʊ֐��R�[�� */
	interrupt_common(CAN_CH_2);

	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_CAN_INTR_CH2);)

}

/********************************************************/
/* �֐�����		: CAN �����݋��ʊ֐�					*/
/* �@�\�̐���	: �eCAN�`�����l���̑���M�����݋��ʊ֐�	*/
/* ����			: (I)can_ch_no:CAN�`�����l���ԍ�		*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/23								*/
/* ���l			: 										*/
/*	�����݃��W�X�^�̒l����X�e�[�^�X�����݂�����M������*/
/*  ���𔻕ʂ��āA�����̐U�蕪�����s���B				*/
/********************************************************/
CAN_DRV_LOCAL void interrupt_common(E_CAN_CH_NO can_ch_no)
{

	/* �X�e�[�^�X�ω��ɂ�銄���݂��ǂ����̔��� */
    if(C_Can_reg[can_ch_no]->INTR.hword == 0x8000) {
        interrupt_status(can_ch_no);
    }
    else if((C_Can_reg[can_ch_no]->INTR.hword >= MIN_MESSAGE_NO)
         && (C_Can_reg[can_ch_no]->INTR.hword <= MAX_MESSAGE_NO)) {
		/* ���b�Z�[�W����M�����ݏ��� */
       interrupt_tx_rx(can_ch_no);
    }
    else {
        (void)__wait_nop();
        (void)__wait_nop();
    }

}

/********************************************************/
/* �֐�����		: �X�e�[�^�X�ω������ݏ���				*/
/* �@�\�̐���	: �X�e�[�^�X�ω��ɂ�銄���݂ɑ΂���	*/
/*				  ����									*/
/* ����			: (I)can_ch_no:CAN�`�����l���ԍ�		*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/23								*/
/* ���l			: 										*/
/*	�o�X�I�t��Ԃ��ǂ����𔻒肵�āA�o�X�I�t��Ԃ̏ꍇ  */
/*  �ɂ�CAN�R���g���[�����ċN������B					*/
/********************************************************/
CAN_DRV_LOCAL void interrupt_status(E_CAN_CH_NO can_ch_no)
{

	/* �o�X�I�t��Ԃ��ǂ����̔��� */
    if(C_Can_reg[can_ch_no]->STATR.bit.BOff==0x01) {
        /* CAN�R���g���[�����ċN������ */
        C_Can_reg[can_ch_no]->CTRLR.bit.Init = 0;

		/* �G���[�J�E���g���N���A�����̂�҂� */
        while((C_Can_reg[can_ch_no]->ERRCNT.bit.TEC != 0)
           || (C_Can_reg[can_ch_no]->ERRCNT.bit.REC != 0)) {
        }
    }

}

/********************************************************/
/* �֐�����		: ���M�^��M�����ݏ���					*/
/* �@�\�̐���	: ���M�^��M�����݂ɑ΂��鏈��			*/
/* ����			: (I)can_ch_no:CAN�`�����l���ԍ�		*/
/* �߂�l		: �Ȃ�									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/23								*/
/* ���l			: 										*/
/*	��M�����݂̏ꍇ�A��M�f�[�^����������O�o�b�t�@��	*/
/*  �i�[����B���M�����݂̏ꍇ�A���M�Ɏg�p����RAM���N��	*/
/*  �A����B											*/
/********************************************************/
CAN_DRV_LOCAL void interrupt_tx_rx(E_CAN_CH_NO can_ch_no)
{

	UI_8  msg_no;
	UI_32 msg_val;
	UI_16 rx_data[4];
	UI_32 msg_id;
	UI_8  data_len;
	
	/* �����ݗv���̃��b�Z�[�W�ԍ��擾 */
	msg_no = (UI_8)C_Can_reg[can_ch_no]->INTR.hword;
    
    /* ���b�Z�[�W�ԍ�����MsgVal�r�b�g�쐬 */
	msg_val = 0x00000001UL << (msg_no - 1);
    
	/* ���̃��b�Z�[�W���L���Ŋ������݃y���f�B���O���ł��邩�ǂ����`�F�b�N */
	if(((C_Can_reg[can_ch_no]->MSGVAL12.word & msg_val) != 0)
	&& ((C_Can_reg[can_ch_no]->INTPND12.word & msg_val) != 0)) {
		/* ���̃��b�Z�[�W����M�f�[�^���ǂ����̔��� */
		if((C_Can_reg[can_ch_no]->NEWDT12.word & msg_val) != 0 ) {
                /* �R�}���h�}�X�N���W�X�^�ݒ� */
				C_Can_reg[can_ch_no]->IF2CMSK.hword = 0x0000;
				C_Can_reg[can_ch_no]->IF2CMSK.bit.Mask    = 1;
				C_Can_reg[can_ch_no]->IF2CMSK.bit.Arb     = 1;
				C_Can_reg[can_ch_no]->IF2CMSK.bit.Control = 1;
				C_Can_reg[can_ch_no]->IF2CMSK.bit.CIP     = 1;
				C_Can_reg[can_ch_no]->IF2CMSK.bit.TxReq   = 1;
				C_Can_reg[can_ch_no]->IF2CMSK.bit.DataA   = 1;
				C_Can_reg[can_ch_no]->IF2CMSK.bit.DataB   = 1;

				/* ���b�Z�[�W�ԍ��������݁i���b�Z�[�WRAM�̓��e�����W�X�^�Ƀ��[�h�j */
				C_Can_reg[can_ch_no]->IF2CREQ.bit.MN = msg_no;

				/* ���b�Z�[�W���X�g�`�F�b�N */
				if(C_Can_reg[can_ch_no]->IF2MCTR.bit.MsgLst == 1) {
					/* ���b�Z�[�W���X�g */
					(void)__wait_nop();
					
					/* ���b�Z�[�W���䃌�W�X�^�Đݒ� */
			    	C_Can_reg[can_ch_no]->IF2MCTR.hword = 0x0000;
			    	C_Can_reg[can_ch_no]->IF2MCTR.bit.UMask = 1;
			    	C_Can_reg[can_ch_no]->IF2MCTR.bit.RxIE  = 1;
			    	C_Can_reg[can_ch_no]->IF2MCTR.bit.EoB   = 1;

					/* �R�}���h�}�X�N���W�X�^�Đݒ� */
					C_Can_reg[can_ch_no]->IF2CMSK.hword       = 0x0000;
					C_Can_reg[can_ch_no]->IF2CMSK.bit.WR_RD   = 1;
					C_Can_reg[can_ch_no]->IF2CMSK.bit.Control = 1;

					/* ���b�Z�[�W�o�b�t�@�Đݒ� */
                    C_Can_reg[can_ch_no]->IF2CREQ.bit.MN = msg_no;
				}
				else {
					/* ���b�Z�[�WID�擾 */
					msg_id = C_Can_reg[can_ch_no]->IF2ARB.bit.ID;
					if(C_Can_reg[can_ch_no]->IF2ARB.bit.Xtd == 0) {
						msg_id = msg_id >>18;
					}
				
					/* ��M�f�[�^�擾 */
					rx_data[0] = C_Can_reg[can_ch_no]->IF2DTA1.hword;
					rx_data[1] = C_Can_reg[can_ch_no]->IF2DTA2.hword;
					rx_data[2] = C_Can_reg[can_ch_no]->IF2DTB1.hword;
					rx_data[3] = C_Can_reg[can_ch_no]->IF2DTB2.hword;

					/* ��M�f�[�^���擾 */
					data_len = C_Can_reg[can_ch_no]->IF2MCTR.bit.DLC;

					/* ��M�f�[�^�������O�o�b�t�@�Ɋi�[ */
					(void)save_rx_data(can_ch_no, msg_id, rx_data, data_len);
				}
		}
		/* ���̃��b�Z�[�W�����M�f�[�^���ǂ����̔��� */
		else if((C_Can_reg[can_ch_no]->TREQR12.word & msg_val) == 0 ) {
			/* ���b�Z�[�W���䃌�W�X�^�Đݒ� */
			C_Can_reg[can_ch_no]->IF1MCTR.hword = 0x0000;
			C_Can_reg[can_ch_no]->IF1MCTR.bit.TxIE  = 1;
			C_Can_reg[can_ch_no]->IF1MCTR.bit.EoB   = 1;
			C_Can_reg[can_ch_no]->IF1MCTR.bit.DLC   = 8;			/* �f�[�^�t���[����8�o�C�g	*/

		   	/* �R�}���h�}�X�N���W�X�^�Đݒ� */
			C_Can_reg[can_ch_no]->IF1CMSK.hword = 0x0000;
			C_Can_reg[can_ch_no]->IF1CMSK.bit.WR_RD   = 1;
			C_Can_reg[can_ch_no]->IF1CMSK.bit.Control = 1;

			/* ���b�Z�[�W�ԍ��������݁i�R�}���h�v���j */
			C_Can_reg[can_ch_no]->IF1CREQ.bit.MN = msg_no;
		}
		else {
			/* ���M�G���[���ǂ����̔��� */
			if (C_Can_reg[can_ch_no]->STATR.bit.LEC != 0) {

				/* ���M�G���[�̏ꍇ�̓��b�Z�[�W���䃌�W�X�^�Đݒ�(���M�v�����N���A) */
				C_Can_reg[can_ch_no]->IF1MCTR.hword = 0x0000;
				C_Can_reg[can_ch_no]->IF1MCTR.bit.TxIE  = 1;
				C_Can_reg[can_ch_no]->IF1MCTR.bit.EoB   = 1;
				C_Can_reg[can_ch_no]->IF1MCTR.bit.DLC   = 8;			/* �f�[�^�t���[����8�o�C�g	*/

				/* �R�}���h�}�X�N���W�X�^�Đݒ�(���M�v�����N���A) */
				C_Can_reg[can_ch_no]->IF1CMSK.hword = 0x0000;
				C_Can_reg[can_ch_no]->IF1CMSK.bit.WR_RD   = 1;
				C_Can_reg[can_ch_no]->IF1CMSK.bit.Control = 1;

				/* ���b�Z�[�W�ԍ��������݁i�R�}���h�v���j */
				C_Can_reg[can_ch_no]->IF1CREQ.bit.MN = msg_no;
			}
		}
	}

}

/********************************************************/
/* �֐�����		: ��M�f�[�^�Z�[�u����					*/
/* �@�\�̐���	: ��M�f�[�^����������O�o�b�t�@�֊i�[	*/
/*				  ����B								*/
/* ����			: (I)can_ch_no  :CAN�`�����l���ԍ�		*/
/* 				: (I)msg_id     :���b�Z�[�WID			*/
/* 				: (I)rx_data    :��M�f�[�^�z��|�C���^	*/
/* 				: (I)rx_data_len:��M�f�[�^��			*/
/* �߂�l		: ��������(E_RC_OK/E_RC_NG)				*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/23								*/
/* ���l			: 										*/
/*	��M�����݂̏ꍇ�A��M�f�[�^����������O�o�b�t�@��	*/
/*  �i�[����B���M�����݂̏ꍇ�A���M�Ɏg�p����RAM���N��	*/
/*  �A����B											*/
/********************************************************/
CAN_DRV_LOCAL E_RC save_rx_data(E_CAN_CH_NO can_ch_no, UI_32 msg_id, const UI_16 *rx_data, UI_8 rx_data_len)
{

	E_RC ret = E_RC_OK;
	T_Rx_buffer *rx_buffer;
	T_Can_data   *save_ptr;
	UI_8         dst_index;
	
	/* ����Ώۂ̃����O�o�b�t�@�m�� */
	rx_buffer = &S_Rx_buffer[can_ch_no];

	if((rx_buffer->count_w -  rx_buffer->count_r) >= CAN_RING_BUF_MAX) {
		/* �����O�o�b�t�@����t */
/* 		OUTLOG(OutputLog("CAN receive buffer overflow!!! msg_id = <%08x>\n", msg_id);) */
		return E_RC_NG;
	}

	/* �f�[�^�Z�[�u�挈�� */
	save_ptr = &rx_buffer->can_data[rx_buffer->count_w % CAN_RING_BUF_MAX];

	/* �f�[�^�Z�[�u */
	save_ptr->id = msg_id;
	save_ptr->len = rx_data_len;
	for(dst_index = 0; dst_index < rx_data_len; dst_index++) {
		if((dst_index % 2) == 0) {
			save_ptr->data[dst_index] = (UI_8)((rx_data[dst_index / 2] & 0xff00) >> 8);
		}
		else {
			save_ptr->data[dst_index] = (UI_8)((rx_data[dst_index / 2] & 0x00ff));
		}
	}

	rx_buffer->count_w++;

	return ret;
}

/********************************************************/
/* �֐�����		: ��M�f�[�^�擾����					*/
/* �@�\�̐���	: �����O�o�b�t�@�Ɋi�[����Ă����M	*/
/*				  �f�[�^���擾����B					*/
/* ����			: (I)can_ch_no:CAN�`�����l���ԍ�		*/
/* 				: (O)can_data :�擾�f�[�^�i�[��ptr		*/
/* �߂�l		: ��������(E_RC_OK/E_RC_NG)				*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/24								*/
/* ���l			: 										*/
/*	���������O�o�b�t�@�ɕێ�����Ă����M�f�[�^�����	*/
/*  �o���B�����O�o�b�t�@����̏ꍇ�ɂ́AE_RC_NG�����^�[	*/
/*	������B											*/
/********************************************************/
E_RC Can_drv_get_rx_data(E_CAN_CH_NO can_ch_no, T_Can_data *can_data)
{

	E_RC ret = E_RC_OK;
	T_Rx_buffer *rx_buffer;
	
	/* ����Ώۂ̃����O�o�b�t�@�m�� */
	rx_buffer = &S_Rx_buffer[can_ch_no];

	/* �����O�o�b�t�@���󂩂ǂ����`�F�b�N */
	if((rx_buffer->count_w - rx_buffer->count_r) == 0) {
		return E_RC_NG;
	}

	/* �f�[�^���[�h */
	(void)memcpy(can_data, &rx_buffer->can_data[rx_buffer->count_r % CAN_RING_BUF_MAX], sizeof(T_Can_data));

	rx_buffer->count_r++;

	return ret;

}

/********************************************************/
/* �֐�����		: ��M�f�[�^�N���A����					*/
/* �@�\�̐���	: �����O�o�b�t�@�Ɋi�[����Ă����M	*/
/*				  �f�[�^���N���A����B					*/
/* ����			: (I)can_ch_no:CAN�`�����l���ԍ�		*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/11/11								*/
/* ���l			: 										*/
/********************************************************/
void Can_drv_clear_rx_data(E_CAN_CH_NO can_ch_no)
{
	T_Rx_buffer *rx_buffer;
	
	/* ����Ώۂ̃����O�o�b�t�@�m�� */
	rx_buffer = &S_Rx_buffer[can_ch_no];

	/* �ǂݍ��݃J�E���^���������݃J�E���^�ɍ��킹�� */
	rx_buffer->count_r = rx_buffer->count_w;
}
