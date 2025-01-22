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
#define CAN_INT_LEVEL			0xf2	/* CAN割込みレベル		*/

#define MIN_MESSAGE_NO			 1		/* 最小メッセージ番号   */
#define MAX_MESSAGE_NO			32		/* 最大メッセージ番号	*/

#define CAN_DRV_LOCAL	static

/* CANレジスタ */
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


/* CANレジスタ */
CAN_DRV_LOCAL __io volatile T_Can_reg *const C_Can_reg[CAN_CH_COUNT] = {
	(volatile T_Can_reg*)&IO_CAN0, (volatile T_Can_reg*)&IO_CAN1, (volatile T_Can_reg*)&IO_CAN2
};

#define CAN_RING_BUF_MAX		50					/* CAN受信データリングバッファ数				*/

typedef struct {
	T_Can_data	can_data[CAN_RING_BUF_MAX];			/* CAN受信データリングバッファ					*/
	UI_32		count_w;							/* CAN受信データリングバッファWriteカウント		*/
	UI_32		count_r;							/* CAN受信データリングバッファReadカウント		*/
} T_Rx_buffer;

/* 内部変数定義 */
CAN_DRV_LOCAL T_Rx_buffer S_Rx_buffer[CAN_CH_COUNT];		/* CAN受信データリングバッファ					*/
CAN_DRV_LOCAL UI_8 S_can_drv_initialized = 0;					/* CANドライバ初期化済みフラグ					*/

/* 内部関数プロトタイプ宣言 */
CAN_DRV_LOCAL void init_message_buffer(E_CAN_CH_NO can_ch_no, UI_8 send_buf_cnt);
CAN_DRV_LOCAL void interrupt_common(E_CAN_CH_NO can_ch_no);
CAN_DRV_LOCAL void interrupt_status(E_CAN_CH_NO can_ch_no);
CAN_DRV_LOCAL void interrupt_tx_rx(E_CAN_CH_NO can_ch_no);
CAN_DRV_LOCAL E_RC save_rx_data(E_CAN_CH_NO can_ch_no, UI_32 msg_id, const UI_16 *rx_data, UI_8 rx_data_len);

/********************************************************/
/* 関数名称		: CANドライバ初期化						*/
/* 機能の説明	: CANドライバを初期化する				*/
/*				  する									*/
/* 引数			: void									*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/11								*/
/* 備考			: 										*/
/********************************************************/
void Can_drv_init(void)
{
	UI_8 p112;
	UI_8 p124;

	/* Local CAN スタンバイ解除 */
	(void)PortDrv_GetDataBit( PORTDRV_PORT_11,PORTDRV_BIT_2, &p112);
	if(p112 != 0) {
		(void)PortDrv_SetDataBit( PORTDRV_PORT_11,PORTDRV_BIT_2, 0);
	}

	/* 車両CAN スタンバイ解除 */
	(void)PortDrv_GetDataBit( PORTDRV_PORT_12,PORTDRV_BIT_4, &p124);
	if(p124 != 0) {
		(void)PortDrv_SetDataBit( PORTDRV_PORT_12,PORTDRV_BIT_4, 0);
	}

    /* プレスケーラ設定 */
    IO_CANPRE.byte = 0x0C;       /* CAN_Interface_Prescale 1/5 (16MHz = 80MHz/5) */

	S_can_drv_initialized = 1;
}

/********************************************************/
/* 関数名称		: CANチャンネル初期化					*/
/* 機能の説明	: 指定チャンネルを初期化する。			*/
/* 引数			: (I)can_ch_no:CANチャンネル番号		*/
/*				  (I)send_buf_cnt:送信バッファ数 		*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/11								*/
/* 備考			: 										*/
/*   send_buffer_countには送信に使用するバッファ数を	*/
/*   指定する。											*/
/*   バッファ数は全部で32個のため、(32 - send_buf_cnt)が*/
/*   受信用バッファ数となる。 							*/
/********************************************************/
void Can_drv_ch_init(E_CAN_CH_NO can_ch_no, UI_8 send_buf_cnt)
{
	/* CANドライバ未初期化なら初期化を行う */
	if(S_can_drv_initialized != 1) {
		Can_drv_init();
	}

	/* 受信データリングバッファ初期化 */
	(void)memset(&S_Rx_buffer[can_ch_no], 0x00, sizeof(T_Rx_buffer));

	/* CANコントローラ停止 */
	C_Can_reg[can_ch_no]->CTRLR.bit.Init = 1;

	/* メッセージバッファの初期設定 */
	init_message_buffer(can_ch_no, send_buf_cnt);

	/* CANバスの設定 */
    C_Can_reg[can_ch_no]->CTRLR.hword = 0x0041;        /* BTR/BRPE 書き込み許可 */
    C_Can_reg[can_ch_no]->BTR.hword   = 0x2B41;        /* 500kbps */
    C_Can_reg[can_ch_no]->BRPER.hword = 0x0000;        
    C_Can_reg[can_ch_no]->CTRLR.hword = 0x0001;        /* BTR/BRPE 書き込み禁止 */

	C_Can_reg[can_ch_no]->CTRLR.bit.EIE  = 1;			/* エラー割り込み許可	*/
	C_Can_reg[can_ch_no]->CTRLR.bit.IE   = 1;			/* 割り込み許可			*/

	C_Can_reg[can_ch_no]->CTRLR.bit.DAR   = 0;			/* 自動再送許可			*/


	C_Can_reg[can_ch_no]->CTRLR.bit.Init = 1;			/* 初期化				*/

    /* CANコントローラ開始 */
	C_Can_reg[can_ch_no]->CTRLR.bit.Init = 0;
	while(C_Can_reg[can_ch_no]->CTRLR.bit.Init == 1) {
		/* nop */
	}
	return;
}

/********************************************************/
/* 関数名称		: メッセージバッファ初期化処理			*/
/* 機能の説明	: CANコントローラのメッセージバッファを	*/
/*				  初期化する。							*/
/* 引数			: (I)can_ch_no:CANチャンネル番号		*/
/*				  (I)send_buf_cnt:送信バッファ数 		*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/11								*/
/* 備考			: 										*/
/*	送信用にIF1、メッセージ番号1～send_buf_cntを使用する。*/
/*	受信用にIF2、メッセージ番号(send_buf_cnt + 1)～32を	*/
/*  使用する。											*/
/********************************************************/
CAN_DRV_LOCAL void init_message_buffer(E_CAN_CH_NO can_ch_no, UI_8 send_buf_cnt)
{

	UI_8 msg_no;	/* メッセージ番号 */

	/* 送信バッファ数の指定フェールセーフ */
	if(send_buf_cnt > MAX_MESSAGE_NO) {
		send_buf_cnt = MAX_MESSAGE_NO;
	}

	/********************************/
	/* 送信メッセージバッファの設定 */
	/********************************/

	for(msg_no = 1; msg_no <= send_buf_cnt; msg_no++) {
	   	/* コマンドマスクレジスタ設定 */
		C_Can_reg[can_ch_no]->IF1CMSK.hword = 0x0000;
		C_Can_reg[can_ch_no]->IF1CMSK.bit.WR_RD   = 1;
		C_Can_reg[can_ch_no]->IF1CMSK.bit.Mask    = 1;
		C_Can_reg[can_ch_no]->IF1CMSK.bit.Arb     = 1;

		/* アービトレーションレジスタ設定 */
		C_Can_reg[can_ch_no]->IF1ARB.word = 0x00000000;
		C_Can_reg[can_ch_no]->IF1ARB.bit.MsgVal = 1;
		C_Can_reg[can_ch_no]->IF1ARB.bit.Dir    = 1;

	    /* データレジスタクリア */
		C_Can_reg[can_ch_no]->IF1DTA1.hword = 0x0000;
		C_Can_reg[can_ch_no]->IF1DTA2.hword = 0x0000;
		C_Can_reg[can_ch_no]->IF1DTB1.hword = 0x0000;
		C_Can_reg[can_ch_no]->IF1DTB2.hword = 0x0000;

	    /* メッセージ番号設定 */
	    C_Can_reg[can_ch_no]->IF1CREQ.hword = msg_no;
	}
	
	/********************************/
	/* 受信メッセージバッファの設定 */
	/********************************/

	for(msg_no = send_buf_cnt + 1; msg_no <= MAX_MESSAGE_NO; msg_no++) {
    	/* コマンドマスクレジスタ設定 */
    	C_Can_reg[can_ch_no]->IF2CMSK.hword = 0x0000;
		C_Can_reg[can_ch_no]->IF2CMSK.bit.WR_RD   = 1;
		C_Can_reg[can_ch_no]->IF2CMSK.bit.Mask    = 1;
		C_Can_reg[can_ch_no]->IF2CMSK.bit.Arb     = 1;
		C_Can_reg[can_ch_no]->IF2CMSK.bit.Control = 1;
    
		/* マスクレジスタ設定 */
    	C_Can_reg[can_ch_no]->IF2MSK.word = 0x00000000;
    	C_Can_reg[can_ch_no]->IF2MSK.bit.MXtd    = 1;
    	C_Can_reg[can_ch_no]->IF2MSK.bit.MDir    = 1;
    	C_Can_reg[can_ch_no]->IF2MSK.bit.RESV682 = 1;
		C_Can_reg[can_ch_no]->IF2MSK.word        = ((C_Can_reg[can_ch_no]->IF2MSK.word & 0xE0000000UL) | 0x1FFC0000UL); /* MID28-18 オール1 */
   	
		/* アービトレーションレジスタ設定 */
    	C_Can_reg[can_ch_no]->IF2ARB.word = 0x00000000;
		C_Can_reg[can_ch_no]->IF2ARB.bit.MsgVal = 0;
		C_Can_reg[can_ch_no]->IF2ARB.bit.ID     = 0;			/* メッセージIDは後から再設定する */
    
		/* メッセージ制御レジスタ設定 */
    	C_Can_reg[can_ch_no]->IF2MCTR.hword = 0x0000;
    	C_Can_reg[can_ch_no]->IF2MCTR.bit.UMask = 1;
    	C_Can_reg[can_ch_no]->IF2MCTR.bit.RxIE  = 1;
    	C_Can_reg[can_ch_no]->IF2MCTR.bit.EoB   = 1;
    	C_Can_reg[can_ch_no]->IF2MCTR.bit.DLC   = 8;			/* データフレーム長8バイト	*/
    	
    	/* メッセージ番号書き込み（コマンド要求） */
    	C_Can_reg[can_ch_no]->IF2CREQ.hword = msg_no;
	}

	return;
}

/********************************************************/
/* 関数名称		: メッセージバッファの対象メッセージID	*/
/*				  変更									*/
/* 機能の説明	: msg_numで指定されるメッセージバッファ*/
/*				  の受容メッセージIDを変更する。		*/
/* 引数			: (I)can_ch_no:CANチャンネル番号		*/
/*   			  (I)msg_no :変更対象メッセージ番号		*/
/*   			  (I)msg_id :受容メッセージID			*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/11								*/
/* 備考			: 										*/
/*	（なし）											*/
/********************************************************/
void Can_drv_change_message_buffer(E_CAN_CH_NO can_ch_no, UI_8 msg_no, UI_32 msg_id)
{
	/* コマンドマスクレジスタ設定 */
	C_Can_reg[can_ch_no]->IF2CMSK.hword = 0x0000;
	C_Can_reg[can_ch_no]->IF2CMSK.bit.WR_RD = 1;
	C_Can_reg[can_ch_no]->IF2CMSK.bit.Arb   = 1;
	C_Can_reg[can_ch_no]->IF2CMSK.bit.Mask  = 1;

	/* アービトレーションレジスタ設定 */
	C_Can_reg[can_ch_no]->IF2ARB.word = 0x00000000;
	C_Can_reg[can_ch_no]->IF2ARB.bit.MsgVal = 1;
	if((msg_id & 0xFFFF0000UL) == 0){
		/* 標準フレームはID28-ID18を使用する */
		msg_id = msg_id << 18;
		C_Can_reg[can_ch_no]->IF2MSK.word    = ((C_Can_reg[can_ch_no]->IF2MSK.word & 0xE0000000UL) | 0x1FFC0000UL); /* MID28-18 オール1 */
		C_Can_reg[can_ch_no]->IF2ARB.bit.Xtd = 0;
	}
	else {
		C_Can_reg[can_ch_no]->IF2MSK.word    = ((C_Can_reg[can_ch_no]->IF2MSK.word & 0xE0000000UL) | 0x1FFFFFFFUL); /* MID28-0 オール1 */
		C_Can_reg[can_ch_no]->IF2ARB.bit.Xtd = 1;
	}
	C_Can_reg[can_ch_no]->IF2ARB.bit.ID = msg_id & 0x1FFFFFFFUL;	/* メッセージID設定 */
	
	/* メッセージ番号書き込み（コマンド要求） */
	C_Can_reg[can_ch_no]->IF2CREQ.hword = msg_no;
}

/********************************************************/
/* 関数名称		: メッセージバッファの対象メッセージID	*/
/*				  変更(メッセージIDのマスク指定可)		*/
/* 機能の説明	: msg_numで指定されるメッセージバッファ	*/
/*				  の受容メッセージIDを変更する。		*/
/* 引数			: (I)can_ch_no:CANチャンネル番号		*/
/*   			  (I)msg_no :変更対象メッセージ番号		*/
/*   			  (I)msg_id :受容メッセージID			*/
/*   			  (I)mask   :マスク						*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 16/08/11								*/
/* 備考			: 										*/
/*	（なし）											*/
/********************************************************/
void Can_drv_change_message_buffer_mask(E_CAN_CH_NO can_ch_no, UI_8 msg_no, UI_32 msg_id, UI_32 mask)
{
	/* コマンドマスクレジスタ設定 */
	C_Can_reg[can_ch_no]->IF2CMSK.hword = 0x0000;
	C_Can_reg[can_ch_no]->IF2CMSK.bit.WR_RD = 1;
	C_Can_reg[can_ch_no]->IF2CMSK.bit.Arb   = 1;
	C_Can_reg[can_ch_no]->IF2CMSK.bit.Mask  = 1;

	/* アービトレーションレジスタ設定 */
	C_Can_reg[can_ch_no]->IF2ARB.word = 0x00000000;
	if (msg_id == 0) {
		/* 指定されたメッセージ番号を未使用にする */
		C_Can_reg[can_ch_no]->IF2ARB.bit.MsgVal = 0;
		C_Can_reg[can_ch_no]->IF2ARB.bit.ID = 0;
	}
	else {
		C_Can_reg[can_ch_no]->IF2ARB.bit.MsgVal = 1;
		if((msg_id & 0xFFFF0000UL) == 0){
			/* 標準フレームはID28-ID18を使用する */
			msg_id = msg_id << 18;
			mask = mask << 18;
			C_Can_reg[can_ch_no]->IF2MSK.bit.Msk     = 0x1FFC0000UL & mask;	/* MID28-18 をマスク */
			C_Can_reg[can_ch_no]->IF2ARB.bit.Xtd = 0;
		}
		else {
	    	C_Can_reg[can_ch_no]->IF2MSK.bit.Msk     = 0x1FFFFFFFUL & mask;	/* MID28-0 をマスク */
			C_Can_reg[can_ch_no]->IF2ARB.bit.Xtd = 1;
		}
		C_Can_reg[can_ch_no]->IF2ARB.bit.ID = msg_id & 0x1FFFFFFFUL;	/* メッセージID設定 */
	}

	/* メッセージ番号書き込み（コマンド要求） */
	C_Can_reg[can_ch_no]->IF2CREQ.hword = msg_no;
}

/********************************************************/
/* 関数名称		: 送信データの設定						*/
/* 機能の説明	: 送信するデータを送信用バッファに設定	*/
/*				  し、これを送信する。					*/
/* 引数			: (I)can_ch_no:CANチャンネル番号		*/
/*   			  (I)msg_no   :送信メッセージ番号		*/
/*   			  (I)msg_id   :送信メッセージID			*/
/*   			  (I)msg_data :送信データ				*/
/*   			  (I)msg_data_len :送信データ長			*/
/* 戻り値		: 処理結果(E_RC_OK/E_RC_NG)				*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/23								*/
/* 備考			: 										*/
/*	（なし）											*/
/********************************************************/
E_RC Can_drv_set_tx_data(E_CAN_CH_NO can_ch_no, UI_8 msg_no, UI_32 msg_id, const UI_8 *msg_data, UI_8 msg_data_len)
{
	UI_8 i;
	
	if(msg_data_len > 8) {
		return E_RC_NG;
	}

	/* コマンドマスクレジスタ設定 */
	C_Can_reg[can_ch_no]->IF1CMSK.hword = 0x0000;
	C_Can_reg[can_ch_no]->IF1CMSK.bit.WR_RD   = 1;
	C_Can_reg[can_ch_no]->IF1CMSK.bit.Arb     = 1;
	C_Can_reg[can_ch_no]->IF1CMSK.bit.Control = 1;
	C_Can_reg[can_ch_no]->IF1CMSK.bit.TxReq   = 1;
	C_Can_reg[can_ch_no]->IF1CMSK.bit.DataA   = 1;
	C_Can_reg[can_ch_no]->IF1CMSK.bit.DataB   = 1;

	/* アービトレーションレジスタの設定 */
	C_Can_reg[can_ch_no]->IF1ARB.bit.MsgVal = 1;
	C_Can_reg[can_ch_no]->IF1ARB.bit.Dir    = 1;
	if((msg_id & 0xFFFF0000UL) == 0){
		/* 標準フレームはID28-ID18を使用する */
		msg_id = msg_id << 18;
		C_Can_reg[can_ch_no]->IF1ARB.bit.Xtd = 0;
	}
	else {
		C_Can_reg[can_ch_no]->IF1ARB.bit.Xtd = 1;
	}
	C_Can_reg[can_ch_no]->IF1ARB.bit.ID = msg_id & 0x1FFFFFFFUL;	/* メッセージID設定 */
	
    /* データレジスタ設定 */
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
	/* メッセージ制御レジスタ設定 */
    C_Can_reg[can_ch_no]->IF1MCTR.hword = 0x0000;
    C_Can_reg[can_ch_no]->IF1MCTR.bit.IntPnd = 1;
    C_Can_reg[can_ch_no]->IF1MCTR.bit.TxIE   = 1;
    C_Can_reg[can_ch_no]->IF1MCTR.bit.TxRqst = 1;
    C_Can_reg[can_ch_no]->IF1MCTR.bit.EoB    = 1;
    C_Can_reg[can_ch_no]->IF1MCTR.bit.DLC    = msg_data_len;		/* データ長設定 */

	/* メッセージ番号書き込み（コマンド要求） */
    C_Can_reg[can_ch_no]->IF1CREQ.bit.MN = msg_no;

	return E_RC_OK;
}

/********************************************************/
/* 関数名称		: CAN 0 割込み関数						*/
/* 機能の説明	: CAN 0の送受信割込み関数				*/
/* 引数			: なし									*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/23								*/
/* 備考			: 										*/
/*	（なし）											*/
/********************************************************/
void __interrupt CanDrv_interrupt_can_0(void)
{

	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_CAN_INTR_CH0);)

	/* 割込み共通関数コール */
	interrupt_common(CAN_CH_0);

	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_CAN_INTR_CH0);)

}

/********************************************************/
/* 関数名称		: CAN 1割込み関数						*/
/* 機能の説明	: CAN 1の送受信割込み関数				*/
/* 引数			: なし									*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/23								*/
/* 備考			: 										*/
/*	（なし）											*/
/********************************************************/
void __interrupt CanDrv_interrupt_can_1(void)
{

	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_CAN_INTR_CH1);)

	/* 割込み共通関数コール */
	interrupt_common(CAN_CH_1);

	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_CAN_INTR_CH1);)

}

/********************************************************/
/* 関数名称		: CAN 2割込み関数						*/
/* 機能の説明	: CAN 2送受信割込み関数					*/
/* 引数			: なし									*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/23								*/
/* 備考			: 										*/
/*	（なし）											*/
/********************************************************/
void __interrupt CanDrv_interrupt_can_2(void)
{

	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_CAN_INTR_CH2);)

	/* 割込み共通関数コール */
	interrupt_common(CAN_CH_2);

	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_CAN_INTR_CH2);)

}

/********************************************************/
/* 関数名称		: CAN 割込み共通関数					*/
/* 機能の説明	: 各CANチャンネルの送受信割込み共通関数	*/
/* 引数			: (I)can_ch_no:CANチャンネル番号		*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/23								*/
/* 備考			: 										*/
/*	割込みレジスタの値からステータス割込みか送受信割込み*/
/*  かを判別して、処理の振り分けを行う。				*/
/********************************************************/
CAN_DRV_LOCAL void interrupt_common(E_CAN_CH_NO can_ch_no)
{

	/* ステータス変化による割込みかどうかの判定 */
    if(C_Can_reg[can_ch_no]->INTR.hword == 0x8000) {
        interrupt_status(can_ch_no);
    }
    else if((C_Can_reg[can_ch_no]->INTR.hword >= MIN_MESSAGE_NO)
         && (C_Can_reg[can_ch_no]->INTR.hword <= MAX_MESSAGE_NO)) {
		/* メッセージ送受信割込み処理 */
       interrupt_tx_rx(can_ch_no);
    }
    else {
        (void)__wait_nop();
        (void)__wait_nop();
    }

}

/********************************************************/
/* 関数名称		: ステータス変化割込み処理				*/
/* 機能の説明	: ステータス変化による割込みに対する	*/
/*				  処理									*/
/* 引数			: (I)can_ch_no:CANチャンネル番号		*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/23								*/
/* 備考			: 										*/
/*	バスオフ状態かどうかを判定して、バスオフ状態の場合  */
/*  にはCANコントローラを再起動する。					*/
/********************************************************/
CAN_DRV_LOCAL void interrupt_status(E_CAN_CH_NO can_ch_no)
{

	/* バスオフ状態かどうかの判定 */
    if(C_Can_reg[can_ch_no]->STATR.bit.BOff==0x01) {
        /* CANコントローラを再起動する */
        C_Can_reg[can_ch_no]->CTRLR.bit.Init = 0;

		/* エラーカウントがクリアされるのを待つ */
        while((C_Can_reg[can_ch_no]->ERRCNT.bit.TEC != 0)
           || (C_Can_reg[can_ch_no]->ERRCNT.bit.REC != 0)) {
        }
    }

}

/********************************************************/
/* 関数名称		: 送信／受信割込み処理					*/
/* 機能の説明	: 送信／受信割込みに対する処理			*/
/* 引数			: (I)can_ch_no:CANチャンネル番号		*/
/* 戻り値		: なし									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/23								*/
/* 備考			: 										*/
/*	受信割込みの場合、受信データを内部リングバッファに	*/
/*  格納する。送信割込みの場合、送信に使用したRAMをクリ	*/
/*  アする。											*/
/********************************************************/
CAN_DRV_LOCAL void interrupt_tx_rx(E_CAN_CH_NO can_ch_no)
{

	UI_8  msg_no;
	UI_32 msg_val;
	UI_16 rx_data[4];
	UI_32 msg_id;
	UI_8  data_len;
	
	/* 割込み要因のメッセージ番号取得 */
	msg_no = (UI_8)C_Can_reg[can_ch_no]->INTR.hword;
    
    /* メッセージ番号からMsgValビット作成 */
	msg_val = 0x00000001UL << (msg_no - 1);
    
	/* このメッセージが有効で且つ割込みペンディング中であるかどうかチェック */
	if(((C_Can_reg[can_ch_no]->MSGVAL12.word & msg_val) != 0)
	&& ((C_Can_reg[can_ch_no]->INTPND12.word & msg_val) != 0)) {
		/* このメッセージが受信データかどうかの判定 */
		if((C_Can_reg[can_ch_no]->NEWDT12.word & msg_val) != 0 ) {
                /* コマンドマスクレジスタ設定 */
				C_Can_reg[can_ch_no]->IF2CMSK.hword = 0x0000;
				C_Can_reg[can_ch_no]->IF2CMSK.bit.Mask    = 1;
				C_Can_reg[can_ch_no]->IF2CMSK.bit.Arb     = 1;
				C_Can_reg[can_ch_no]->IF2CMSK.bit.Control = 1;
				C_Can_reg[can_ch_no]->IF2CMSK.bit.CIP     = 1;
				C_Can_reg[can_ch_no]->IF2CMSK.bit.TxReq   = 1;
				C_Can_reg[can_ch_no]->IF2CMSK.bit.DataA   = 1;
				C_Can_reg[can_ch_no]->IF2CMSK.bit.DataB   = 1;

				/* メッセージ番号書き込み（メッセージRAMの内容をレジスタにロード） */
				C_Can_reg[can_ch_no]->IF2CREQ.bit.MN = msg_no;

				/* メッセージロストチェック */
				if(C_Can_reg[can_ch_no]->IF2MCTR.bit.MsgLst == 1) {
					/* メッセージロスト */
					(void)__wait_nop();
					
					/* メッセージ制御レジスタ再設定 */
			    	C_Can_reg[can_ch_no]->IF2MCTR.hword = 0x0000;
			    	C_Can_reg[can_ch_no]->IF2MCTR.bit.UMask = 1;
			    	C_Can_reg[can_ch_no]->IF2MCTR.bit.RxIE  = 1;
			    	C_Can_reg[can_ch_no]->IF2MCTR.bit.EoB   = 1;

					/* コマンドマスクレジスタ再設定 */
					C_Can_reg[can_ch_no]->IF2CMSK.hword       = 0x0000;
					C_Can_reg[can_ch_no]->IF2CMSK.bit.WR_RD   = 1;
					C_Can_reg[can_ch_no]->IF2CMSK.bit.Control = 1;

					/* メッセージバッファ再設定 */
                    C_Can_reg[can_ch_no]->IF2CREQ.bit.MN = msg_no;
				}
				else {
					/* メッセージID取得 */
					msg_id = C_Can_reg[can_ch_no]->IF2ARB.bit.ID;
					if(C_Can_reg[can_ch_no]->IF2ARB.bit.Xtd == 0) {
						msg_id = msg_id >>18;
					}
				
					/* 受信データ取得 */
					rx_data[0] = C_Can_reg[can_ch_no]->IF2DTA1.hword;
					rx_data[1] = C_Can_reg[can_ch_no]->IF2DTA2.hword;
					rx_data[2] = C_Can_reg[can_ch_no]->IF2DTB1.hword;
					rx_data[3] = C_Can_reg[can_ch_no]->IF2DTB2.hword;

					/* 受信データ長取得 */
					data_len = C_Can_reg[can_ch_no]->IF2MCTR.bit.DLC;

					/* 受信データをリングバッファに格納 */
					(void)save_rx_data(can_ch_no, msg_id, rx_data, data_len);
				}
		}
		/* このメッセージが送信データかどうかの判定 */
		else if((C_Can_reg[can_ch_no]->TREQR12.word & msg_val) == 0 ) {
			/* メッセージ制御レジスタ再設定 */
			C_Can_reg[can_ch_no]->IF1MCTR.hword = 0x0000;
			C_Can_reg[can_ch_no]->IF1MCTR.bit.TxIE  = 1;
			C_Can_reg[can_ch_no]->IF1MCTR.bit.EoB   = 1;
			C_Can_reg[can_ch_no]->IF1MCTR.bit.DLC   = 8;			/* データフレーム長8バイト	*/

		   	/* コマンドマスクレジスタ再設定 */
			C_Can_reg[can_ch_no]->IF1CMSK.hword = 0x0000;
			C_Can_reg[can_ch_no]->IF1CMSK.bit.WR_RD   = 1;
			C_Can_reg[can_ch_no]->IF1CMSK.bit.Control = 1;

			/* メッセージ番号書き込み（コマンド要求） */
			C_Can_reg[can_ch_no]->IF1CREQ.bit.MN = msg_no;
		}
		else {
			/* 送信エラーかどうかの判定 */
			if (C_Can_reg[can_ch_no]->STATR.bit.LEC != 0) {

				/* 送信エラーの場合はメッセージ制御レジスタ再設定(送信要求をクリア) */
				C_Can_reg[can_ch_no]->IF1MCTR.hword = 0x0000;
				C_Can_reg[can_ch_no]->IF1MCTR.bit.TxIE  = 1;
				C_Can_reg[can_ch_no]->IF1MCTR.bit.EoB   = 1;
				C_Can_reg[can_ch_no]->IF1MCTR.bit.DLC   = 8;			/* データフレーム長8バイト	*/

				/* コマンドマスクレジスタ再設定(送信要求をクリア) */
				C_Can_reg[can_ch_no]->IF1CMSK.hword = 0x0000;
				C_Can_reg[can_ch_no]->IF1CMSK.bit.WR_RD   = 1;
				C_Can_reg[can_ch_no]->IF1CMSK.bit.Control = 1;

				/* メッセージ番号書き込み（コマンド要求） */
				C_Can_reg[can_ch_no]->IF1CREQ.bit.MN = msg_no;
			}
		}
	}

}

/********************************************************/
/* 関数名称		: 受信データセーブ処理					*/
/* 機能の説明	: 受信データを内部リングバッファへ格納	*/
/*				  する。								*/
/* 引数			: (I)can_ch_no  :CANチャンネル番号		*/
/* 				: (I)msg_id     :メッセージID			*/
/* 				: (I)rx_data    :受信データ配列ポインタ	*/
/* 				: (I)rx_data_len:受信データ長			*/
/* 戻り値		: 処理結果(E_RC_OK/E_RC_NG)				*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/23								*/
/* 備考			: 										*/
/*	受信割込みの場合、受信データを内部リングバッファに	*/
/*  格納する。送信割込みの場合、送信に使用したRAMをクリ	*/
/*  アする。											*/
/********************************************************/
CAN_DRV_LOCAL E_RC save_rx_data(E_CAN_CH_NO can_ch_no, UI_32 msg_id, const UI_16 *rx_data, UI_8 rx_data_len)
{

	E_RC ret = E_RC_OK;
	T_Rx_buffer *rx_buffer;
	T_Can_data   *save_ptr;
	UI_8         dst_index;
	
	/* 操作対象のリングバッファ確定 */
	rx_buffer = &S_Rx_buffer[can_ch_no];

	if((rx_buffer->count_w -  rx_buffer->count_r) >= CAN_RING_BUF_MAX) {
		/* リングバッファが一杯 */
/* 		OUTLOG(OutputLog("CAN receive buffer overflow!!! msg_id = <%08x>\n", msg_id);) */
		return E_RC_NG;
	}

	/* データセーブ先決定 */
	save_ptr = &rx_buffer->can_data[rx_buffer->count_w % CAN_RING_BUF_MAX];

	/* データセーブ */
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
/* 関数名称		: 受信データ取得処理					*/
/* 機能の説明	: リングバッファに格納されている受信	*/
/*				  データを取得する。					*/
/* 引数			: (I)can_ch_no:CANチャンネル番号		*/
/* 				: (O)can_data :取得データ格納先ptr		*/
/* 戻り値		: 処理結果(E_RC_OK/E_RC_NG)				*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/24								*/
/* 備考			: 										*/
/*	内部リングバッファに保持されている受信データを取り	*/
/*  出す。リングバッファが空の場合には、E_RC_NGをリター	*/
/*	ンする。											*/
/********************************************************/
E_RC Can_drv_get_rx_data(E_CAN_CH_NO can_ch_no, T_Can_data *can_data)
{

	E_RC ret = E_RC_OK;
	T_Rx_buffer *rx_buffer;
	
	/* 操作対象のリングバッファ確定 */
	rx_buffer = &S_Rx_buffer[can_ch_no];

	/* リングバッファが空かどうかチェック */
	if((rx_buffer->count_w - rx_buffer->count_r) == 0) {
		return E_RC_NG;
	}

	/* データロード */
	(void)memcpy(can_data, &rx_buffer->can_data[rx_buffer->count_r % CAN_RING_BUF_MAX], sizeof(T_Can_data));

	rx_buffer->count_r++;

	return ret;

}

/********************************************************/
/* 関数名称		: 受信データクリア処理					*/
/* 機能の説明	: リングバッファに格納されている受信	*/
/*				  データをクリアする。					*/
/* 引数			: (I)can_ch_no:CANチャンネル番号		*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/11/11								*/
/* 備考			: 										*/
/********************************************************/
void Can_drv_clear_rx_data(E_CAN_CH_NO can_ch_no)
{
	T_Rx_buffer *rx_buffer;
	
	/* 操作対象のリングバッファ確定 */
	rx_buffer = &S_Rx_buffer[can_ch_no];

	/* 読み込みカウンタを書き込みカウンタに合わせる */
	rx_buffer->count_r = rx_buffer->count_w;
}
