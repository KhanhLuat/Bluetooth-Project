/*
 MB91570 Series I/O register declaration file V01L02
 ALL RIGHTS RESERVED, COPYRIGHT (C) FUJITSU SEMICONDUCTOR LIMITED 2011
 LICENSED MATERIAL - PROGRAM PROPERTY OF FUJITSU SEMICONDUCTOR LIMITED
*/
#ifdef  __IO_NEAR
#ifdef  __IO_FAR
#error "__IO_NEAR and __IO_FAR must not be defined at the same time"
#else
#define ___IOWIDTH __near
#endif
#else
#ifdef __IO_FAR
#define ___IOWIDTH __far
#else                               /* specified by memory model */
#define ___IOWIDTH
#endif
#endif

#ifdef  __IO_DEFINE
#define __IO_EXTERN __io
#define __IO_EXTENDED volatile ___IOWIDTH
#else
#define __IO_EXTERN   extern __io      /* for data, which can have __io */
#define __IO_EXTENDED extern volatile ___IOWIDTH
#endif

typedef unsigned char        __BYTE;
typedef unsigned short       __HWORD;
typedef unsigned long        __WORD;
typedef const unsigned short __HWORD_READ;

#define	CONSTANT_0	(0)
#define	CONSTANT_1	(1)
#define	CONSTANT_2	(2)
#define	CONSTANT_3	(3)
#define	CONSTANT_4	(4)
#define	CONSTANT_5	(5)
#define	CONSTANT_6	(6)
#define	CONSTANT_7	(7)
#define	CONSTANT_8	(8)
#define	CONSTANT_9	(9)
#define	CONSTANT_10	(10)
#define	CONSTANT_11	(11)
#define	CONSTANT_12	(12)
#define	CONSTANT_13	(13)
#define	CONSTANT_14	(14)
#define	CONSTANT_15	(15)
#define	CONSTANT_16	(16)
#define	CONSTANT_17	(17)
#define	CONSTANT_18	(18)
#define	CONSTANT_19	(19)
#define	CONSTANT_20	(20)
#define	CONSTANT_21	(21)
#define	CONSTANT_22	(22)
#define	CONSTANT_23	(23)
#define	CONSTANT_24	(24)
#define	CONSTANT_25	(25)
#define	CONSTANT_26	(26)
#define	CONSTANT_27	(27)
#define	CONSTANT_28	(28)
#define	CONSTANT_29	(29)
#define	CONSTANT_30	(30)
#define	CONSTANT_31	(31)
#define	CONSTANT_32	(32)
#define	CONSTANT_33	(33)
#define	CONSTANT_34	(34)
#define	CONSTANT_35	(35)
#define	CONSTANT_36	(36)
#define	CONSTANT_37	(37)
#define	CONSTANT_38	(38)
#define	CONSTANT_39	(39)
#define	CONSTANT_40	(40)
#define	CONSTANT_41	(41)
#define	CONSTANT_42	(42)
#define	CONSTANT_43	(43)
#define	CONSTANT_44	(44)
#define	CONSTANT_45	(45)
#define	CONSTANT_46	(46)
#define	CONSTANT_47	(47)
#define	CONSTANT_48	(48)
#define	CONSTANT_49	(49)
#define	CONSTANT_50	(50)
#define	CONSTANT_51	(51)
#define	CONSTANT_52	(52)
#define	CONSTANT_53	(53)
#define	CONSTANT_54	(54)
#define	CONSTANT_55	(55)
#define	CONSTANT_56	(56)
#define	CONSTANT_57	(57)
#define	CONSTANT_58	(58)
#define	CONSTANT_59	(59)
#define	CONSTANT_60	(60)
#define	CONSTANT_61	(61)
#define	CONSTANT_62	(62)
#define	CONSTANT_63	(63)


#ifdef __IO_DEFINE
#pragma segment IO=IO_PDR00, locate=0x0
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDR00STR;

__IO_EXTERN	  PDR00STR	IO_PDR00;
#define	_pdr00		(IO_PDR00)
#define	PDR00		(IO_PDR00.byte)
#define	PDR00_P7  	(IO_PDR00.bit.P7)
#define	PDR00_P6  	(IO_PDR00.bit.P6)
#define	PDR00_P5  	(IO_PDR00.bit.P5)
#define	PDR00_P4  	(IO_PDR00.bit.P4)
#define	PDR00_P3  	(IO_PDR00.bit.P3)
#define	PDR00_P2  	(IO_PDR00.bit.P2)
#define	PDR00_P1  	(IO_PDR00.bit.P1)
#define	PDR00_P0  	(IO_PDR00.bit.P0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_PDR01, locate=0x1
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDR01STR;

__IO_EXTERN	  PDR01STR	IO_PDR01;
#define	_pdr01		(IO_PDR01)
#define	PDR01		(IO_PDR01.byte)
#define	PDR01_P7  	(IO_PDR01.bit.P7)
#define	PDR01_P6  	(IO_PDR01.bit.P6)
#define	PDR01_P5  	(IO_PDR01.bit.P5)
#define	PDR01_P4  	(IO_PDR01.bit.P4)
#define	PDR01_P3  	(IO_PDR01.bit.P3)
#define	PDR01_P2  	(IO_PDR01.bit.P2)
#define	PDR01_P1  	(IO_PDR01.bit.P1)
#define	PDR01_P0  	(IO_PDR01.bit.P0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_PDR02, locate=0x2
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDR02STR;

__IO_EXTERN	  PDR02STR	IO_PDR02;
#define	_pdr02		(IO_PDR02)
#define	PDR02		(IO_PDR02.byte)
#define	PDR02_P7  	(IO_PDR02.bit.P7)
#define	PDR02_P6  	(IO_PDR02.bit.P6)
#define	PDR02_P5  	(IO_PDR02.bit.P5)
#define	PDR02_P4  	(IO_PDR02.bit.P4)
#define	PDR02_P3  	(IO_PDR02.bit.P3)
#define	PDR02_P2  	(IO_PDR02.bit.P2)
#define	PDR02_P1  	(IO_PDR02.bit.P1)
#define	PDR02_P0  	(IO_PDR02.bit.P0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_PDR03, locate=0x3
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDR03STR;

__IO_EXTERN	  PDR03STR	IO_PDR03;
#define	_pdr03		(IO_PDR03)
#define	PDR03		(IO_PDR03.byte)
#define	PDR03_P7  	(IO_PDR03.bit.P7)
#define	PDR03_P6  	(IO_PDR03.bit.P6)
#define	PDR03_P5  	(IO_PDR03.bit.P5)
#define	PDR03_P4  	(IO_PDR03.bit.P4)
#define	PDR03_P3  	(IO_PDR03.bit.P3)
#define	PDR03_P2  	(IO_PDR03.bit.P2)
#define	PDR03_P1  	(IO_PDR03.bit.P1)
#define	PDR03_P0  	(IO_PDR03.bit.P0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_PDR04, locate=0x4
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDR04STR;

__IO_EXTERN	  PDR04STR	IO_PDR04;
#define	_pdr04		(IO_PDR04)
#define	PDR04		(IO_PDR04.byte)
#define	PDR04_P7  	(IO_PDR04.bit.P7)
#define	PDR04_P6  	(IO_PDR04.bit.P6)
#define	PDR04_P5  	(IO_PDR04.bit.P5)
#define	PDR04_P4  	(IO_PDR04.bit.P4)
#define	PDR04_P3  	(IO_PDR04.bit.P3)
#define	PDR04_P2  	(IO_PDR04.bit.P2)
#define	PDR04_P1  	(IO_PDR04.bit.P1)
#define	PDR04_P0  	(IO_PDR04.bit.P0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_PDR05, locate=0x5
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDR05STR;

__IO_EXTERN	  PDR05STR	IO_PDR05;
#define	_pdr05		(IO_PDR05)
#define	PDR05		(IO_PDR05.byte)
#define	PDR05_P7  	(IO_PDR05.bit.P7)
#define	PDR05_P6  	(IO_PDR05.bit.P6)
#define	PDR05_P5  	(IO_PDR05.bit.P5)
#define	PDR05_P4  	(IO_PDR05.bit.P4)
#define	PDR05_P3  	(IO_PDR05.bit.P3)
#define	PDR05_P2  	(IO_PDR05.bit.P2)
#define	PDR05_P1  	(IO_PDR05.bit.P1)
#define	PDR05_P0  	(IO_PDR05.bit.P0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_PDR06, locate=0x6
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDR06STR;

__IO_EXTERN	  PDR06STR	IO_PDR06;
#define	_pdr06		(IO_PDR06)
#define	PDR06		(IO_PDR06.byte)
#define	PDR06_P7  	(IO_PDR06.bit.P7)
#define	PDR06_P6  	(IO_PDR06.bit.P6)
#define	PDR06_P5  	(IO_PDR06.bit.P5)
#define	PDR06_P4  	(IO_PDR06.bit.P4)
#define	PDR06_P3  	(IO_PDR06.bit.P3)
#define	PDR06_P2  	(IO_PDR06.bit.P2)
#define	PDR06_P1  	(IO_PDR06.bit.P1)
#define	PDR06_P0  	(IO_PDR06.bit.P0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_PDR07, locate=0x7
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDR07STR;

__IO_EXTERN	  PDR07STR	IO_PDR07;
#define	_pdr07		(IO_PDR07)
#define	PDR07		(IO_PDR07.byte)
#define	PDR07_P7  	(IO_PDR07.bit.P7)
#define	PDR07_P6  	(IO_PDR07.bit.P6)
#define	PDR07_P5  	(IO_PDR07.bit.P5)
#define	PDR07_P4  	(IO_PDR07.bit.P4)
#define	PDR07_P3  	(IO_PDR07.bit.P3)
#define	PDR07_P2  	(IO_PDR07.bit.P2)
#define	PDR07_P1  	(IO_PDR07.bit.P1)
#define	PDR07_P0  	(IO_PDR07.bit.P0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_PDR08, locate=0x8
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDR08STR;

__IO_EXTERN	  PDR08STR	IO_PDR08;
#define	_pdr08		(IO_PDR08)
#define	PDR08		(IO_PDR08.byte)
#define	PDR08_P7  	(IO_PDR08.bit.P7)
#define	PDR08_P6  	(IO_PDR08.bit.P6)
#define	PDR08_P5  	(IO_PDR08.bit.P5)
#define	PDR08_P4  	(IO_PDR08.bit.P4)
#define	PDR08_P3  	(IO_PDR08.bit.P3)
#define	PDR08_P2  	(IO_PDR08.bit.P2)
#define	PDR08_P1  	(IO_PDR08.bit.P1)
#define	PDR08_P0  	(IO_PDR08.bit.P0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_PDR09, locate=0x9
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDR09STR;

__IO_EXTERN	  PDR09STR	IO_PDR09;
#define	_pdr09		(IO_PDR09)
#define	PDR09		(IO_PDR09.byte)
#define	PDR09_P7  	(IO_PDR09.bit.P7)
#define	PDR09_P6  	(IO_PDR09.bit.P6)
#define	PDR09_P5  	(IO_PDR09.bit.P5)
#define	PDR09_P4  	(IO_PDR09.bit.P4)
#define	PDR09_P3  	(IO_PDR09.bit.P3)
#define	PDR09_P2  	(IO_PDR09.bit.P2)
#define	PDR09_P1  	(IO_PDR09.bit.P1)
#define	PDR09_P0  	(IO_PDR09.bit.P0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_PDR10, locate=0xA
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDR10STR;

__IO_EXTERN	  PDR10STR	IO_PDR10;
#define	_pdr10		(IO_PDR10)
#define	PDR10		(IO_PDR10.byte)
#define	PDR10_P7  	(IO_PDR10.bit.P7)
#define	PDR10_P6  	(IO_PDR10.bit.P6)
#define	PDR10_P5  	(IO_PDR10.bit.P5)
#define	PDR10_P4  	(IO_PDR10.bit.P4)
#define	PDR10_P3  	(IO_PDR10.bit.P3)
#define	PDR10_P2  	(IO_PDR10.bit.P2)
#define	PDR10_P1  	(IO_PDR10.bit.P1)
#define	PDR10_P0  	(IO_PDR10.bit.P0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_PDR11, locate=0xB
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDR11STR;

__IO_EXTERN	  PDR11STR	IO_PDR11;
#define	_pdr11		(IO_PDR11)
#define	PDR11		(IO_PDR11.byte)
#define	PDR11_P7  	(IO_PDR11.bit.P7)
#define	PDR11_P6  	(IO_PDR11.bit.P6)
#define	PDR11_P5  	(IO_PDR11.bit.P5)
#define	PDR11_P4  	(IO_PDR11.bit.P4)
#define	PDR11_P3  	(IO_PDR11.bit.P3)
#define	PDR11_P2  	(IO_PDR11.bit.P2)
#define	PDR11_P1  	(IO_PDR11.bit.P1)
#define	PDR11_P0  	(IO_PDR11.bit.P0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_PDR12, locate=0xC
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDR12STR;

__IO_EXTERN	  PDR12STR	IO_PDR12;
#define	_pdr12		(IO_PDR12)
#define	PDR12		(IO_PDR12.byte)
#define	PDR12_P7  	(IO_PDR12.bit.P7)
#define	PDR12_P6  	(IO_PDR12.bit.P6)
#define	PDR12_P5  	(IO_PDR12.bit.P5)
#define	PDR12_P4  	(IO_PDR12.bit.P4)
#define	PDR12_P3  	(IO_PDR12.bit.P3)
#define	PDR12_P2  	(IO_PDR12.bit.P2)
#define	PDR12_P1  	(IO_PDR12.bit.P1)
#define	PDR12_P0  	(IO_PDR12.bit.P0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_PDR13, locate=0xD
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	RESV2:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDR13STR;

__IO_EXTERN	  PDR13STR	IO_PDR13;
#define	_pdr13		(IO_PDR13)
#define	PDR13		(IO_PDR13.byte)
#define	PDR13_P7  	(IO_PDR13.bit.P7)
#define	PDR13_P6  	(IO_PDR13.bit.P6)
#define	PDR13_P4  	(IO_PDR13.bit.P4)
#define	PDR13_P3  	(IO_PDR13.bit.P3)
#define	PDR13_P2  	(IO_PDR13.bit.P2)
#define	PDR13_P1  	(IO_PDR13.bit.P1)
#define	PDR13_P0  	(IO_PDR13.bit.P0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_WDTCR0, locate=0x3C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RSTP:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	WT:4;
	} bit;
} WDTCR0STR;

__IO_EXTERN	  WDTCR0STR	IO_WDTCR0;
#define	_wdtcr0		(IO_WDTCR0)
#define	WDTCR0		(IO_WDTCR0.byte)
#define	WDTCR0_RSTP  	(IO_WDTCR0.bit.RSTP)
#define	WDTCR0_WT  	(IO_WDTCR0.bit.WT)

#ifdef __IO_DEFINE
#pragma segment IO=IO_WDTCPR0, locate=0x3D
#endif

__IO_EXTERN	__BYTE	IO_WDTCPR0;
#define	_wdtcpr0		(IO_WDTCPR0)
#define	WDTCPR0	(_wdtcpr0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_WDTCR1, locate=0x3E
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	WT:4;
	} bit;
} WDTCR1STR;

__IO_EXTERN	 const  WDTCR1STR	IO_WDTCR1;
#define	_wdtcr1		(IO_WDTCR1)
#define	WDTCR1		(IO_WDTCR1.byte)
#define	WDTCR1_WT  	(IO_WDTCR1.bit.WT)

#ifdef __IO_DEFINE
#pragma segment IO=IO_WDTCPR1, locate=0x3F
#endif

__IO_EXTERN	__BYTE	IO_WDTCPR1;
#define	_wdtcpr1		(IO_WDTCPR1)
#define	WDTCPR1	(_wdtcpr1)

#ifdef __IO_DEFINE
#pragma segment IO=IO_DICR, locate=0x44
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	DLYI:1;
	} bit;
} DICRSTR;

__IO_EXTERN	  DICRSTR	IO_DICR;
#define	_dicr		(IO_DICR)
#define	DICR		(IO_DICR.byte)
#define	DICR_DLYI  	(IO_DICR.bit.DLYI)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMRLRA4, locate=0x48
#endif

__IO_EXTERN	__HWORD	IO_TMRLRA4;
#define	_tmrlra4		(IO_TMRLRA4)
#define	TMRLRA4	(_tmrlra4)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMR4, locate=0x4A
#endif

__IO_EXTERN	const __HWORD	IO_TMR4;
#define	_tmr4		(IO_TMR4)
#define	TMR4	(_tmr4)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMRLRB4, locate=0x4C
#endif

__IO_EXTERN	__HWORD	IO_TMRLRB4;
#define	_tmrlrb4		(IO_TMRLRB4)
#define	TMRLRB4	(_tmrlrb4)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMCSR4, locate=0x4E
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	TMCSR4H;
		__BYTE	TMCSR4L;
	} byte;
	struct {
			__HWORD	MOD:2;
			__HWORD	TRGM:2;
			__HWORD	CSL:3;
			__HWORD	GATE:1;
			__HWORD	EF:1;
			__HWORD	RESV5:1;
			__HWORD	OUTL:1;
			__HWORD	RELD:1;
			__HWORD	INTE:1;
			__HWORD	UF:1;
			__HWORD	CNTE:1;
			__HWORD	TRG:1;
	} bit;
} TMCSR4STR;

__IO_EXTERN	  TMCSR4STR	IO_TMCSR4;
#define	_tmcsr4		(IO_TMCSR4)
#define	TMCSR4		(IO_TMCSR4.hword)
#define	TMCSR4_TMCSR4H	(IO_TMCSR4.byte.TMCSR4H)
#define	TMCSR4_TMCSR4L	(IO_TMCSR4.byte.TMCSR4L)
#define	TMCSR4_MOD  	(IO_TMCSR4.bit.MOD)
#define	TMCSR4_TRGM  	(IO_TMCSR4.bit.TRGM)
#define	TMCSR4_CSL  	(IO_TMCSR4.bit.CSL)
#define	TMCSR4_GATE  	(IO_TMCSR4.bit.GATE)
#define	TMCSR4_EF  	(IO_TMCSR4.bit.EF)
#define	TMCSR4_OUTL  	(IO_TMCSR4.bit.OUTL)
#define	TMCSR4_RELD  	(IO_TMCSR4.bit.RELD)
#define	TMCSR4_INTE  	(IO_TMCSR4.bit.INTE)
#define	TMCSR4_UF  	(IO_TMCSR4.bit.UF)
#define	TMCSR4_CNTE  	(IO_TMCSR4.bit.CNTE)
#define	TMCSR4_TRG  	(IO_TMCSR4.bit.TRG)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMRLRA5, locate=0x50
#endif

__IO_EXTERN	__HWORD	IO_TMRLRA5;
#define	_tmrlra5		(IO_TMRLRA5)
#define	TMRLRA5	(_tmrlra5)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMR5, locate=0x52
#endif

__IO_EXTERN	const __HWORD	IO_TMR5;
#define	_tmr5		(IO_TMR5)
#define	TMR5	(_tmr5)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMRLRB5, locate=0x54
#endif

__IO_EXTERN	__HWORD	IO_TMRLRB5;
#define	_tmrlrb5		(IO_TMRLRB5)
#define	TMRLRB5	(_tmrlrb5)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMCSR5, locate=0x56
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	TMCSR5H;
		__BYTE	TMCSR5L;
	} byte;
	struct {
			__HWORD	MOD:2;
			__HWORD	TRGM:2;
			__HWORD	CSL:3;
			__HWORD	GATE:1;
			__HWORD	EF:1;
			__HWORD	RESV5:1;
			__HWORD	OUTL:1;
			__HWORD	RELD:1;
			__HWORD	INTE:1;
			__HWORD	UF:1;
			__HWORD	CNTE:1;
			__HWORD	TRG:1;
	} bit;
} TMCSR5STR;

__IO_EXTERN	  TMCSR5STR	IO_TMCSR5;
#define	_tmcsr5		(IO_TMCSR5)
#define	TMCSR5		(IO_TMCSR5.hword)
#define	TMCSR5_TMCSR5H	(IO_TMCSR5.byte.TMCSR5H)
#define	TMCSR5_TMCSR5L	(IO_TMCSR5.byte.TMCSR5L)
#define	TMCSR5_MOD  	(IO_TMCSR5.bit.MOD)
#define	TMCSR5_TRGM  	(IO_TMCSR5.bit.TRGM)
#define	TMCSR5_CSL  	(IO_TMCSR5.bit.CSL)
#define	TMCSR5_GATE  	(IO_TMCSR5.bit.GATE)
#define	TMCSR5_EF  	(IO_TMCSR5.bit.EF)
#define	TMCSR5_OUTL  	(IO_TMCSR5.bit.OUTL)
#define	TMCSR5_RELD  	(IO_TMCSR5.bit.RELD)
#define	TMCSR5_INTE  	(IO_TMCSR5.bit.INTE)
#define	TMCSR5_UF  	(IO_TMCSR5.bit.UF)
#define	TMCSR5_CNTE  	(IO_TMCSR5.bit.CNTE)
#define	TMCSR5_TRG  	(IO_TMCSR5.bit.TRG)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMRLRA6, locate=0x58
#endif

__IO_EXTERN	__HWORD	IO_TMRLRA6;
#define	_tmrlra6		(IO_TMRLRA6)
#define	TMRLRA6	(_tmrlra6)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMR6, locate=0x5A
#endif

__IO_EXTERN	const __HWORD	IO_TMR6;
#define	_tmr6		(IO_TMR6)
#define	TMR6	(_tmr6)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMRLRB6, locate=0x5C
#endif

__IO_EXTERN	__HWORD	IO_TMRLRB6;
#define	_tmrlrb6		(IO_TMRLRB6)
#define	TMRLRB6	(_tmrlrb6)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMCSR6, locate=0x5E
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	TMCSR6H;
		__BYTE	TMCSR6L;
	} byte;
	struct {
			__HWORD	MOD:2;
			__HWORD	TRGM:2;
			__HWORD	CSL:3;
			__HWORD	GATE:1;
			__HWORD	EF:1;
			__HWORD	RESV5:1;
			__HWORD	OUTL:1;
			__HWORD	RELD:1;
			__HWORD	INTE:1;
			__HWORD	UF:1;
			__HWORD	CNTE:1;
			__HWORD	TRG:1;
	} bit;
} TMCSR6STR;

__IO_EXTERN	  TMCSR6STR	IO_TMCSR6;
#define	_tmcsr6		(IO_TMCSR6)
#define	TMCSR6		(IO_TMCSR6.hword)
#define	TMCSR6_TMCSR6H	(IO_TMCSR6.byte.TMCSR6H)
#define	TMCSR6_TMCSR6L	(IO_TMCSR6.byte.TMCSR6L)
#define	TMCSR6_MOD  	(IO_TMCSR6.bit.MOD)
#define	TMCSR6_TRGM  	(IO_TMCSR6.bit.TRGM)
#define	TMCSR6_CSL  	(IO_TMCSR6.bit.CSL)
#define	TMCSR6_GATE  	(IO_TMCSR6.bit.GATE)
#define	TMCSR6_EF  	(IO_TMCSR6.bit.EF)
#define	TMCSR6_OUTL  	(IO_TMCSR6.bit.OUTL)
#define	TMCSR6_RELD  	(IO_TMCSR6.bit.RELD)
#define	TMCSR6_INTE  	(IO_TMCSR6.bit.INTE)
#define	TMCSR6_UF  	(IO_TMCSR6.bit.UF)
#define	TMCSR6_CNTE  	(IO_TMCSR6.bit.CNTE)
#define	TMCSR6_TRG  	(IO_TMCSR6.bit.TRG)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMRLRA0, locate=0x60
#endif

__IO_EXTERN	__HWORD	IO_TMRLRA0;
#define	_tmrlra0		(IO_TMRLRA0)
#define	TMRLRA0	(_tmrlra0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMR0, locate=0x62
#endif

__IO_EXTERN	const __HWORD	IO_TMR0;
#define	_tmr0		(IO_TMR0)
#define	TMR0_	(_tmr0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMRLRB0, locate=0x64
#endif

__IO_EXTERN	__HWORD	IO_TMRLRB0;
#define	_tmrlrb0		(IO_TMRLRB0)
#define	TMRLRB0	(_tmrlrb0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_TMCSR0, locate=0x66
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	TMCSR0H;
		__BYTE	TMCSR0L;
	} byte;
	struct {
			__HWORD	MOD:2;
			__HWORD	TRGM:2;
			__HWORD	CSL:3;
			__HWORD	GATE:1;
			__HWORD	EF:1;
			__HWORD	RESV5:1;
			__HWORD	OUTL:1;
			__HWORD	RELD:1;
			__HWORD	INTE:1;
			__HWORD	UF:1;
			__HWORD	CNTE:1;
			__HWORD	TRG:1;
	} bit;
} TMCSR0STR;

__IO_EXTERN	  TMCSR0STR	IO_TMCSR0;
#define	_tmcsr0		(IO_TMCSR0)
#define	TMCSR0		(IO_TMCSR0.hword)
#define	TMCSR0_TMCSR0H	(IO_TMCSR0.byte.TMCSR0H)
#define	TMCSR0_TMCSR0L	(IO_TMCSR0.byte.TMCSR0L)
#define	TMCSR0_MOD  	(IO_TMCSR0.bit.MOD)
#define	TMCSR0_TRGM  	(IO_TMCSR0.bit.TRGM)
#define	TMCSR0_CSL  	(IO_TMCSR0.bit.CSL)
#define	TMCSR0_GATE  	(IO_TMCSR0.bit.GATE)
#define	TMCSR0_EF  	(IO_TMCSR0.bit.EF)
#define	TMCSR0_OUTL  	(IO_TMCSR0.bit.OUTL)
#define	TMCSR0_RELD  	(IO_TMCSR0.bit.RELD)
#define	TMCSR0_INTE  	(IO_TMCSR0.bit.INTE)
#define	TMCSR0_UF  	(IO_TMCSR0.bit.UF)
#define	TMCSR0_CNTE  	(IO_TMCSR0.bit.CNTE)
#define	TMCSR0_TRG  	(IO_TMCSR0.bit.TRG)

#ifdef __IO_DEFINE
#pragma segment IO=IO_BT0, locate=0x80
#endif

typedef union {

struct {
	const __HWORD	TMR0;
	union {
		__HWORD	hword;
		struct {
			__HWORD	RESV20:1;
			__HWORD	CKS:3;
			__HWORD	RESV22:1;
			__HWORD	RESV23:1;
			__HWORD	EGS:2;
			__HWORD	T32:1;
			__HWORD	FMD:3;
			__HWORD	OSEL:1;
			__HWORD	MDSE:1;
			__HWORD	CTEN:1;
			__HWORD	STRG:1;
		} bit;
	} TMCR0;
	__BYTE	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV50:1;
			__BYTE	TGIE:1;
			__BYTE	RESV52:1;
			__BYTE	UDIE:1;
			__BYTE	RESV54:1;
			__BYTE	TGIR:1;
			__BYTE	RESV56:1;
			__BYTE	UDIR:1;
		} bit;
	} STC0;
	__HWORD	IO_RESV2;
	__HWORD	PCSR0;
	__HWORD	IO_RESV3;
} RT0;

struct {
	const __HWORD	TMR0;
	union {
		__HWORD	hword;
		struct {
			__HWORD	RESV20:1;
			__HWORD	CKS:3;
			__HWORD	RESV22:1;
			__HWORD	EGS:3;
			__HWORD	T32:1;
			__HWORD	FMD:3;
			__HWORD	RESV26:1;
			__HWORD	MDSE:1;
			__HWORD	CTEN:1;
			__HWORD	STRG:1;
		} bit;
	} TMCR0;
	__BYTE	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	ERR:1;
			__BYTE	EDIE:1;
			__BYTE	RESV52:1;
			__BYTE	OVIE:1;
			__BYTE	RESV54:1;
			__BYTE	EDIR:1;
			__BYTE	RESV56:1;
			__BYTE	OVIR:1;
		} bit;
	} STC0;
	__HWORD	IO_RESV2;
	__HWORD	IO_RESV3;
	__HWORD	DTBF0;
} PWC0;

struct {
	const __HWORD	TMR0;
	union {
		__HWORD	hword;
		struct {
			__HWORD	RESV20:1;
			__HWORD	CKS:3;
			__HWORD	RTGEN:1;
			__HWORD	PMSK:1;
			__HWORD	EGS:2;
			__HWORD	RESV25:1;
			__HWORD	FMD:3;
			__HWORD	OSEL:1;
			__HWORD	MDSE:1;
			__HWORD	CTEN:1;
			__HWORD	STRG:1;
		} bit;
	} TMCR0;
	__BYTE	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV50:1;
			__BYTE	TGIE:1;
			__BYTE	DTIE:1;
			__BYTE	UDIE:1;
			__BYTE	RESV54:1;
			__BYTE	TGIR:1;
			__BYTE	DTIR:1;
			__BYTE	UDIR:1;
		} bit;
	} STC0;
	__HWORD	IO_RESV2;
	__HWORD	PCSR0;
	__HWORD	PDUT0;
} PWM0;

struct {
	const __HWORD	TMR0;
	union {
		__HWORD	hword;
		struct {
			__HWORD	RESV20:1;
			__HWORD	CKS:3;
			__HWORD	RTGEN:1;
			__HWORD	PMSK:1;
			__HWORD	EGS:2;
			__HWORD	RESV25:1;
			__HWORD	FMD:3;
			__HWORD	OSEL:1;
			__HWORD	MDSE:1;
			__HWORD	CTEN:1;
			__HWORD	STRG:1;
		} bit;
	} TMCR0;
	__BYTE	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV50:1;
			__BYTE	TGIE:1;
			__BYTE	RESV52:1;
			__BYTE	UDIE:1;
			__BYTE	RESV54:1;
			__BYTE	TGIR:1;
			__BYTE	RESV56:1;
			__BYTE	UDIR:1;
		} bit;
	} STC0;
	__HWORD	IO_RESV2;
	__HWORD	PRLL0;
	__HWORD	PRLH0;
} PPG0;

} BT0STR;

__IO_EXTERN	BT0STR	IO_BT0;

#define	BT0_RT0_TMR0	(IO_BT0.RT0.TMR0)
#define	BT0_RT0_TMCR0			(IO_BT0.RT0.TMCR0.hword)
#define	BT0_RT0_TMCR0_CKS    	(IO_BT0.RT0.TMCR0.bit.CKS)
#define	BT0_RT0_TMCR0_EGS    	(IO_BT0.RT0.TMCR0.bit.EGS)
#define	BT0_RT0_TMCR0_T32    	(IO_BT0.RT0.TMCR0.bit.T32)
#define	BT0_RT0_TMCR0_FMD    	(IO_BT0.RT0.TMCR0.bit.FMD)
#define	BT0_RT0_TMCR0_OSEL    	(IO_BT0.RT0.TMCR0.bit.OSEL)
#define	BT0_RT0_TMCR0_MDSE    	(IO_BT0.RT0.TMCR0.bit.MDSE)
#define	BT0_RT0_TMCR0_CTEN    	(IO_BT0.RT0.TMCR0.bit.CTEN)
#define	BT0_RT0_TMCR0_STRG    	(IO_BT0.RT0.TMCR0.bit.STRG)
#define	BT0_RT0_STC0			(IO_BT0.RT0.STC0.byte)
#define	BT0_RT0_STC0_TGIE    	(IO_BT0.RT0.STC0.bit.TGIE)
#define	BT0_RT0_STC0_UDIE    	(IO_BT0.RT0.STC0.bit.UDIE)
#define	BT0_RT0_STC0_TGIR    	(IO_BT0.RT0.STC0.bit.TGIR)
#define	BT0_RT0_STC0_UDIR    	(IO_BT0.RT0.STC0.bit.UDIR)
#define	BT0_RT0_PCSR0	(IO_BT0.RT0.PCSR0)

#define	BT0_PWC0_TMR0	(IO_BT0.PWC0.TMR0)
#define	BT0_PWC0_TMCR0			(IO_BT0.PWC0.TMCR0.hword)
#define	BT0_PWC0_TMCR0_CKS    	(IO_BT0.PWC0.TMCR0.bit.CKS)
#define	BT0_PWC0_TMCR0_EGS    	(IO_BT0.PWC0.TMCR0.bit.EGS)
#define	BT0_PWC0_TMCR0_T32    	(IO_BT0.PWC0.TMCR0.bit.T32)
#define	BT0_PWC0_TMCR0_FMD    	(IO_BT0.PWC0.TMCR0.bit.FMD)
#define	BT0_PWC0_TMCR0_MDSE    	(IO_BT0.PWC0.TMCR0.bit.MDSE)
#define	BT0_PWC0_TMCR0_CTEN    	(IO_BT0.PWC0.TMCR0.bit.CTEN)
#define	BT0_PWC0_TMCR0_STRG    	(IO_BT0.PWC0.TMCR0.bit.STRG)
#define	BT0_PWC0_STC0			(IO_BT0.PWC0.STC0.byte)
#define	BT0_PWC0_STC0_ERR    	(IO_BT0.PWC0.STC0.bit.ERR)
#define	BT0_PWC0_STC0_EDIE    	(IO_BT0.PWC0.STC0.bit.EDIE)
#define	BT0_PWC0_STC0_OVIE    	(IO_BT0.PWC0.STC0.bit.OVIE)
#define	BT0_PWC0_STC0_EDIR    	(IO_BT0.PWC0.STC0.bit.EDIR)
#define	BT0_PWC0_STC0_OVIR    	(IO_BT0.PWC0.STC0.bit.OVIR)
#define	BT0_PWC0_DTBF0	(IO_BT0.PWC0.DTBF0)

#define	BT0_PWM0_TMR0	(IO_BT0.PWM0.TMR0)
#define	BT0_PWM0_TMCR0			(IO_BT0.PWM0.TMCR0.hword)
#define	BT0_PWM0_TMCR0_CKS    	(IO_BT0.PWM0.TMCR0.bit.CKS)
#define	BT0_PWM0_TMCR0_RTGEN    	(IO_BT0.PWM0.TMCR0.bit.RTGEN)
#define	BT0_PWM0_TMCR0_PMSK    	(IO_BT0.PWM0.TMCR0.bit.PMSK)
#define	BT0_PWM0_TMCR0_EGS    	(IO_BT0.PWM0.TMCR0.bit.EGS)
#define	BT0_PWM0_TMCR0_FMD    	(IO_BT0.PWM0.TMCR0.bit.FMD)
#define	BT0_PWM0_TMCR0_OSEL    	(IO_BT0.PWM0.TMCR0.bit.OSEL)
#define	BT0_PWM0_TMCR0_MDSE    	(IO_BT0.PWM0.TMCR0.bit.MDSE)
#define	BT0_PWM0_TMCR0_CTEN    	(IO_BT0.PWM0.TMCR0.bit.CTEN)
#define	BT0_PWM0_TMCR0_STRG    	(IO_BT0.PWM0.TMCR0.bit.STRG)
#define	BT0_PWM0_STC0			(IO_BT0.PWM0.STC0.byte)
#define	BT0_PWM0_STC0_TGIE    	(IO_BT0.PWM0.STC0.bit.TGIE)
#define	BT0_PWM0_STC0_DTIE    	(IO_BT0.PWM0.STC0.bit.DTIE)
#define	BT0_PWM0_STC0_UDIE    	(IO_BT0.PWM0.STC0.bit.UDIE)
#define	BT0_PWM0_STC0_TGIR    	(IO_BT0.PWM0.STC0.bit.TGIR)
#define	BT0_PWM0_STC0_DTIR    	(IO_BT0.PWM0.STC0.bit.DTIR)
#define	BT0_PWM0_STC0_UDIR    	(IO_BT0.PWM0.STC0.bit.UDIR)
#define	BT0_PWM0_PCSR0	(IO_BT0.PWM0.PCSR0)
#define	BT0_PWM0_PDUT0	(IO_BT0.PWM0.PDUT0)

#define	BT0_PPG0_TMR0	(IO_BT0.PPG0.TMR0)
#define	BT0_PPG0_TMCR0			(IO_BT0.PPG0.TMCR0.hword)
#define	BT0_PPG0_TMCR0_CKS    	(IO_BT0.PPG0.TMCR0.bit.CKS)
#define	BT0_PPG0_TMCR0_RTGEN    	(IO_BT0.PPG0.TMCR0.bit.RTGEN)
#define	BT0_PPG0_TMCR0_PMSK    	(IO_BT0.PPG0.TMCR0.bit.PMSK)
#define	BT0_PPG0_TMCR0_EGS    	(IO_BT0.PPG0.TMCR0.bit.EGS)
#define	BT0_PPG0_TMCR0_FMD    	(IO_BT0.PPG0.TMCR0.bit.FMD)
#define	BT0_PPG0_TMCR0_OSEL    	(IO_BT0.PPG0.TMCR0.bit.OSEL)
#define	BT0_PPG0_TMCR0_MDSE    	(IO_BT0.PPG0.TMCR0.bit.MDSE)
#define	BT0_PPG0_TMCR0_CTEN    	(IO_BT0.PPG0.TMCR0.bit.CTEN)
#define	BT0_PPG0_TMCR0_STRG    	(IO_BT0.PPG0.TMCR0.bit.STRG)
#define	BT0_PPG0_STC0			(IO_BT0.PPG0.STC0.byte)
#define	BT0_PPG0_STC0_TGIE    	(IO_BT0.PPG0.STC0.bit.TGIE)
#define	BT0_PPG0_STC0_UDIE    	(IO_BT0.PPG0.STC0.bit.UDIE)
#define	BT0_PPG0_STC0_TGIR    	(IO_BT0.PPG0.STC0.bit.TGIR)
#define	BT0_PPG0_STC0_UDIR    	(IO_BT0.PPG0.STC0.bit.UDIR)
#define	BT0_PPG0_PRLL0	(IO_BT0.PPG0.PRLL0)
#define	BT0_PPG0_PRLH0	(IO_BT0.PPG0.PRLH0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_BT1, locate=0x90
#endif

typedef union {

struct {
	const __HWORD	TMR1;
	union {
		__HWORD	hword;
		struct {
			__HWORD	RESV20:1;
			__HWORD	CKS:3;
			__HWORD	RESV22:1;
			__HWORD	RESV23:1;
			__HWORD	EGS:2;
			__HWORD	T32:1;
			__HWORD	FMD:3;
			__HWORD	OSEL:1;
			__HWORD	MDSE:1;
			__HWORD	CTEN:1;
			__HWORD	STRG:1;
		} bit;
	} TMCR1;
	__BYTE	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV50:1;
			__BYTE	TGIE:1;
			__BYTE	RESV52:1;
			__BYTE	UDIE:1;
			__BYTE	RESV54:1;
			__BYTE	TGIR:1;
			__BYTE	RESV56:1;
			__BYTE	UDIR:1;
		} bit;
	} STC1;
	__HWORD	IO_RESV2;
	__HWORD	PCSR1;
	__HWORD	IO_RESV3;
} RT1;

struct {
	const __HWORD	TMR1;
	union {
		__HWORD	hword;
		struct {
			__HWORD	RESV20:1;
			__HWORD	CKS:3;
			__HWORD	RESV22:1;
			__HWORD	EGS:3;
			__HWORD	T32:1;
			__HWORD	FMD:3;
			__HWORD	RESV26:1;
			__HWORD	MDSE:1;
			__HWORD	CTEN:1;
			__HWORD	STRG:1;
		} bit;
	} TMCR1;
	__BYTE	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	ERR:1;
			__BYTE	EDIE:1;
			__BYTE	RESV52:1;
			__BYTE	OVIE:1;
			__BYTE	RESV54:1;
			__BYTE	EDIR:1;
			__BYTE	RESV56:1;
			__BYTE	OVIR:1;
		} bit;
	} STC1;
	__HWORD	IO_RESV2;
	__HWORD	IO_RESV3;
	__HWORD	DTBF1;
} PWC1;

struct {
	const __HWORD	TMR1;
	union {
		__HWORD	hword;
		struct {
			__HWORD	RESV20:1;
			__HWORD	CKS:3;
			__HWORD	RTGEN:1;
			__HWORD	PMSK:1;
			__HWORD	EGS:2;
			__HWORD	RESV25:1;
			__HWORD	FMD:3;
			__HWORD	OSEL:1;
			__HWORD	MDSE:1;
			__HWORD	CTEN:1;
			__HWORD	STRG:1;
		} bit;
	} TMCR1;
	__BYTE	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV50:1;
			__BYTE	TGIE:1;
			__BYTE	DTIE:1;
			__BYTE	UDIE:1;
			__BYTE	RESV54:1;
			__BYTE	TGIR:1;
			__BYTE	DTIR:1;
			__BYTE	UDIR:1;
		} bit;
	} STC1;
	__HWORD	IO_RESV2;
	__HWORD	PCSR1;
	__HWORD	PDUT1;
} PWM1;

struct {
	const __HWORD	TMR1;
	union {
		__HWORD	hword;
		struct {
			__HWORD	RESV20:1;
			__HWORD	CKS:3;
			__HWORD	RTGEN:1;
			__HWORD	PMSK:1;
			__HWORD	EGS:2;
			__HWORD	RESV25:1;
			__HWORD	FMD:3;
			__HWORD	OSEL:1;
			__HWORD	MDSE:1;
			__HWORD	CTEN:1;
			__HWORD	STRG:1;
		} bit;
	} TMCR1;
	__BYTE	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV50:1;
			__BYTE	TGIE:1;
			__BYTE	RESV52:1;
			__BYTE	UDIE:1;
			__BYTE	RESV54:1;
			__BYTE	TGIR:1;
			__BYTE	RESV56:1;
			__BYTE	UDIR:1;
		} bit;
	} STC1;
	__HWORD	IO_RESV2;
	__HWORD	PRLL1;
	__HWORD	PRLH1;
} PPG1;

} BT1STR;

__IO_EXTERN	BT1STR	IO_BT1;

#define	BT1_RT1_TMR1	(IO_BT1.RT1.TMR1)
#define	BT1_RT1_TMCR1			(IO_BT1.RT1.TMCR1.hword)
#define	BT1_RT1_TMCR1_CKS    	(IO_BT1.RT1.TMCR1.bit.CKS)
#define	BT1_RT1_TMCR1_EGS    	(IO_BT1.RT1.TMCR1.bit.EGS)
#define	BT1_RT1_TMCR1_T32    	(IO_BT1.RT1.TMCR1.bit.T32)
#define	BT1_RT1_TMCR1_FMD    	(IO_BT1.RT1.TMCR1.bit.FMD)
#define	BT1_RT1_TMCR1_OSEL    	(IO_BT1.RT1.TMCR1.bit.OSEL)
#define	BT1_RT1_TMCR1_MDSE    	(IO_BT1.RT1.TMCR1.bit.MDSE)
#define	BT1_RT1_TMCR1_CTEN    	(IO_BT1.RT1.TMCR1.bit.CTEN)
#define	BT1_RT1_TMCR1_STRG    	(IO_BT1.RT1.TMCR1.bit.STRG)
#define	BT1_RT1_STC1			(IO_BT1.RT1.STC1.byte)
#define	BT1_RT1_STC1_TGIE    	(IO_BT1.RT1.STC1.bit.TGIE)
#define	BT1_RT1_STC1_UDIE    	(IO_BT1.RT1.STC1.bit.UDIE)
#define	BT1_RT1_STC1_TGIR    	(IO_BT1.RT1.STC1.bit.TGIR)
#define	BT1_RT1_STC1_UDIR    	(IO_BT1.RT1.STC1.bit.UDIR)
#define	BT1_RT1_PCSR1	(IO_BT1.RT1.PCSR1)

#define	BT1_PWC1_TMR1	(IO_BT1.PWC1.TMR1)
#define	BT1_PWC1_TMCR1			(IO_BT1.PWC1.TMCR1.hword)
#define	BT1_PWC1_TMCR1_CKS    	(IO_BT1.PWC1.TMCR1.bit.CKS)
#define	BT1_PWC1_TMCR1_EGS    	(IO_BT1.PWC1.TMCR1.bit.EGS)
#define	BT1_PWC1_TMCR1_T32    	(IO_BT1.PWC1.TMCR1.bit.T32)
#define	BT1_PWC1_TMCR1_FMD    	(IO_BT1.PWC1.TMCR1.bit.FMD)
#define	BT1_PWC1_TMCR1_MDSE    	(IO_BT1.PWC1.TMCR1.bit.MDSE)
#define	BT1_PWC1_TMCR1_CTEN    	(IO_BT1.PWC1.TMCR1.bit.CTEN)
#define	BT1_PWC1_TMCR1_STRG    	(IO_BT1.PWC1.TMCR1.bit.STRG)
#define	BT1_PWC1_STC1			(IO_BT1.PWC1.STC1.byte)
#define	BT1_PWC1_STC1_ERR    	(IO_BT1.PWC1.STC1.bit.ERR)
#define	BT1_PWC1_STC1_EDIE    	(IO_BT1.PWC1.STC1.bit.EDIE)
#define	BT1_PWC1_STC1_OVIE    	(IO_BT1.PWC1.STC1.bit.OVIE)
#define	BT1_PWC1_STC1_EDIR    	(IO_BT1.PWC1.STC1.bit.EDIR)
#define	BT1_PWC1_STC1_OVIR    	(IO_BT1.PWC1.STC1.bit.OVIR)
#define	BT1_PWC1_DTBF1	(IO_BT1.PWC1.DTBF1)

#define	BT1_PWM1_TMR1	(IO_BT1.PWM1.TMR1)
#define	BT1_PWM1_TMCR1			(IO_BT1.PWM1.TMCR1.hword)
#define	BT1_PWM1_TMCR1_CKS    	(IO_BT1.PWM1.TMCR1.bit.CKS)
#define	BT1_PWM1_TMCR1_RTGEN    	(IO_BT1.PWM1.TMCR1.bit.RTGEN)
#define	BT1_PWM1_TMCR1_PMSK    	(IO_BT1.PWM1.TMCR1.bit.PMSK)
#define	BT1_PWM1_TMCR1_EGS    	(IO_BT1.PWM1.TMCR1.bit.EGS)
#define	BT1_PWM1_TMCR1_FMD    	(IO_BT1.PWM1.TMCR1.bit.FMD)
#define	BT1_PWM1_TMCR1_OSEL    	(IO_BT1.PWM1.TMCR1.bit.OSEL)
#define	BT1_PWM1_TMCR1_MDSE    	(IO_BT1.PWM1.TMCR1.bit.MDSE)
#define	BT1_PWM1_TMCR1_CTEN    	(IO_BT1.PWM1.TMCR1.bit.CTEN)
#define	BT1_PWM1_TMCR1_STRG    	(IO_BT1.PWM1.TMCR1.bit.STRG)
#define	BT1_PWM1_STC1			(IO_BT1.PWM1.STC1.byte)
#define	BT1_PWM1_STC1_TGIE    	(IO_BT1.PWM1.STC1.bit.TGIE)
#define	BT1_PWM1_STC1_DTIE    	(IO_BT1.PWM1.STC1.bit.DTIE)
#define	BT1_PWM1_STC1_UDIE    	(IO_BT1.PWM1.STC1.bit.UDIE)
#define	BT1_PWM1_STC1_TGIR    	(IO_BT1.PWM1.STC1.bit.TGIR)
#define	BT1_PWM1_STC1_DTIR    	(IO_BT1.PWM1.STC1.bit.DTIR)
#define	BT1_PWM1_STC1_UDIR    	(IO_BT1.PWM1.STC1.bit.UDIR)
#define	BT1_PWM1_PCSR1	(IO_BT1.PWM1.PCSR1)
#define	BT1_PWM1_PDUT1	(IO_BT1.PWM1.PDUT1)

#define	BT1_PPG1_TMR1	(IO_BT1.PPG1.TMR1)
#define	BT1_PPG1_TMCR1			(IO_BT1.PPG1.TMCR1.hword)
#define	BT1_PPG1_TMCR1_CKS    	(IO_BT1.PPG1.TMCR1.bit.CKS)
#define	BT1_PPG1_TMCR1_RTGEN    	(IO_BT1.PPG1.TMCR1.bit.RTGEN)
#define	BT1_PPG1_TMCR1_PMSK    	(IO_BT1.PPG1.TMCR1.bit.PMSK)
#define	BT1_PPG1_TMCR1_EGS    	(IO_BT1.PPG1.TMCR1.bit.EGS)
#define	BT1_PPG1_TMCR1_FMD    	(IO_BT1.PPG1.TMCR1.bit.FMD)
#define	BT1_PPG1_TMCR1_OSEL    	(IO_BT1.PPG1.TMCR1.bit.OSEL)
#define	BT1_PPG1_TMCR1_MDSE    	(IO_BT1.PPG1.TMCR1.bit.MDSE)
#define	BT1_PPG1_TMCR1_CTEN    	(IO_BT1.PPG1.TMCR1.bit.CTEN)
#define	BT1_PPG1_TMCR1_STRG    	(IO_BT1.PPG1.TMCR1.bit.STRG)
#define	BT1_PPG1_STC1			(IO_BT1.PPG1.STC1.byte)
#define	BT1_PPG1_STC1_TGIE    	(IO_BT1.PPG1.STC1.bit.TGIE)
#define	BT1_PPG1_STC1_UDIE    	(IO_BT1.PPG1.STC1.bit.UDIE)
#define	BT1_PPG1_STC1_TGIR    	(IO_BT1.PPG1.STC1.bit.TGIR)
#define	BT1_PPG1_STC1_UDIR    	(IO_BT1.PPG1.STC1.bit.UDIR)
#define	BT1_PPG1_PRLL1	(IO_BT1.PPG1.PRLL1)
#define	BT1_PPG1_PRLH1	(IO_BT1.PPG1.PRLH1)

#ifdef __IO_DEFINE
#pragma segment IO=IO_BTSEL01, locate=0x9C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	SEL01:4;
	} bit;
} BTSEL01STR;

__IO_EXTERN	  BTSEL01STR	IO_BTSEL01;
#define	_btsel01		(IO_BTSEL01)
#define	BTSEL01		(IO_BTSEL01.byte)
#define	BTSEL01_SEL01  	(IO_BTSEL01.bit.SEL01)

#ifdef __IO_DEFINE
#pragma segment IO=IO_BTSSSR, locate=0x9E
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	BTSSSRH;
		__BYTE	BTSSSRL;
	} byte;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	RESV13:1;
			__HWORD	SSSR1:1;
			__HWORD	SSSR0:1;
	} bit;
} BTSSSRSTR;

__IO_EXTERN	  BTSSSRSTR	IO_BTSSSR;
#define	_btsssr		(IO_BTSSSR)
#define	BTSSSR		(IO_BTSSSR.hword)
#define	BTSSSR_BTSSSRH	(IO_BTSSSR.byte.BTSSSRH)
#define	BTSSSR_BTSSSRL	(IO_BTSSSR.byte.BTSSSRL)
#define	BTSSSR_SSSR1  	(IO_BTSSSR.bit.SSSR1)
#define	BTSSSR_SSSR0  	(IO_BTSSSR.bit.SSSR0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_AD, locate=0xA0
#endif

__IO_EXTERN	struct {
	union {
		__WORD	word;
		struct {
			__HWORD	ADERH;
			__HWORD	ADERL;
		} hword;
		struct {
			__BYTE	ADERHH;
			__BYTE	ADERHL;
			__BYTE	ADERLH;
			__BYTE	ADERLL;
		} byte;
		struct {
			__WORD	ADE31:1;
			__WORD	ADE30:1;
			__WORD	ADE29:1;
			__WORD	ADE28:1;
			__WORD	ADE27:1;
			__WORD	ADE26:1;
			__WORD	ADE25:1;
			__WORD	ADE24:1;
			__WORD	ADE23:1;
			__WORD	ADE22:1;
			__WORD	ADE21:1;
			__WORD	ADE20:1;
			__WORD	ADE19:1;
			__WORD	ADE18:1;
			__WORD	ADE17:1;
			__WORD	ADE16:1;
			__WORD	ADE15:1;
			__WORD	ADE14:1;
			__WORD	ADE13:1;
			__WORD	ADE12:1;
			__WORD	ADE11:1;
			__WORD	ADE10:1;
			__WORD	ADE9:1;
			__WORD	ADE8:1;
			__WORD	ADE7:1;
			__WORD	ADE6:1;
			__WORD	ADE5:1;
			__WORD	ADE4:1;
			__WORD	ADE3:1;
			__WORD	ADE2:1;
			__WORD	ADE1:1;
			__WORD	ADE0:1;
		} bit;
	} IO_ADER;
	union {
		__BYTE	byte;
		struct {
			__BYTE	BUSY:1;
			__BYTE	INT:1;
			__BYTE	INTE:1;
			__BYTE	PAUS:1;
			__BYTE	STS:2;
			__BYTE	STRT:1;
			__BYTE	RESV46:1;
		} bit;
	} IO_ADCS1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:2;
			__BYTE	S10:1;
			__BYTE	RESV52:1;
			__BYTE	RESV53:1;
			__BYTE	RESV54:1;
			__BYTE	RESV55:1;
			__BYTE	RESV56:1;
		} bit;
	} IO_ADCS0;
	const union {
		__HWORD	hword;
		struct {
			__BYTE	ADCR1;
			__BYTE	ADCR0;
		} byte;
		struct {
			__HWORD	RESV60:1;
			__HWORD	RESV61:1;
			__HWORD	RESV62:1;
			__HWORD	RESV63:1;
			__HWORD	RESV64:1;
			__HWORD	RESV65:1;
			__HWORD	D:10;
		} bit;
	} IO_ADCR01;
	union {
		__HWORD	hword;
		struct {
			__BYTE	ADCT1;
			__BYTE	ADCT0;
		} byte;
		struct {
			__HWORD	CT:6;
			__HWORD	ST:10;
		} bit;
	} IO_ADCT01;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV100:1;
			__BYTE	RESV101:1;
			__BYTE	ANS:6;
		} bit;
	} IO_ADSCH;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV110:1;
			__BYTE	RESV111:1;
			__BYTE	ANE:6;
		} bit;
	} IO_ADECH;
	__BYTE	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	ADE39:1;
			__BYTE	ADE38:1;
			__BYTE	ADE37:1;
			__BYTE	ADE36:1;
			__BYTE	ADE35:1;
			__BYTE	ADE34:1;
			__BYTE	ADE33:1;
			__BYTE	ADE32:1;
		} bit;
	} IO_EADERLL;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV140:1;
			__BYTE	RESV141:1;
			__BYTE	ACH:6;
		} bit;
	} IO_EADCS;
	__BYTE	IO_RESV2;
} IO_AD;

#define	AD_ADER			(IO_AD.IO_ADER.word)
#define	AD_ADER_ADERH	(IO_AD.IO_ADER.hword.ADERH)
#define	AD_ADER_ADERL	(IO_AD.IO_ADER.hword.ADERL)
#define	AD_ADER_ADERHH	(IO_AD.IO_ADER.byte.ADERHH)
#define	AD_ADER_ADERHL	(IO_AD.IO_ADER.byte.ADERHL)
#define	AD_ADER_ADERLH	(IO_AD.IO_ADER.byte.ADERLH)
#define	AD_ADER_ADERLL	(IO_AD.IO_ADER.byte.ADERLL)
#define	AD_ADER_ADE31    	(IO_AD.IO_ADER.bit.ADE31)
#define	AD_ADER_ADE30    	(IO_AD.IO_ADER.bit.ADE30)
#define	AD_ADER_ADE29    	(IO_AD.IO_ADER.bit.ADE29)
#define	AD_ADER_ADE28    	(IO_AD.IO_ADER.bit.ADE28)
#define	AD_ADER_ADE27    	(IO_AD.IO_ADER.bit.ADE27)
#define	AD_ADER_ADE26    	(IO_AD.IO_ADER.bit.ADE26)
#define	AD_ADER_ADE25    	(IO_AD.IO_ADER.bit.ADE25)
#define	AD_ADER_ADE24    	(IO_AD.IO_ADER.bit.ADE24)
#define	AD_ADER_ADE23    	(IO_AD.IO_ADER.bit.ADE23)
#define	AD_ADER_ADE22    	(IO_AD.IO_ADER.bit.ADE22)
#define	AD_ADER_ADE21    	(IO_AD.IO_ADER.bit.ADE21)
#define	AD_ADER_ADE20    	(IO_AD.IO_ADER.bit.ADE20)
#define	AD_ADER_ADE19    	(IO_AD.IO_ADER.bit.ADE19)
#define	AD_ADER_ADE18    	(IO_AD.IO_ADER.bit.ADE18)
#define	AD_ADER_ADE17    	(IO_AD.IO_ADER.bit.ADE17)
#define	AD_ADER_ADE16    	(IO_AD.IO_ADER.bit.ADE16)
#define	AD_ADER_ADE15    	(IO_AD.IO_ADER.bit.ADE15)
#define	AD_ADER_ADE14    	(IO_AD.IO_ADER.bit.ADE14)
#define	AD_ADER_ADE13    	(IO_AD.IO_ADER.bit.ADE13)
#define	AD_ADER_ADE12    	(IO_AD.IO_ADER.bit.ADE12)
#define	AD_ADER_ADE11    	(IO_AD.IO_ADER.bit.ADE11)
#define	AD_ADER_ADE10    	(IO_AD.IO_ADER.bit.ADE10)
#define	AD_ADER_ADE9    	(IO_AD.IO_ADER.bit.ADE9)
#define	AD_ADER_ADE8    	(IO_AD.IO_ADER.bit.ADE8)
#define	AD_ADER_ADE7    	(IO_AD.IO_ADER.bit.ADE7)
#define	AD_ADER_ADE6    	(IO_AD.IO_ADER.bit.ADE6)
#define	AD_ADER_ADE5    	(IO_AD.IO_ADER.bit.ADE5)
#define	AD_ADER_ADE4    	(IO_AD.IO_ADER.bit.ADE4)
#define	AD_ADER_ADE3    	(IO_AD.IO_ADER.bit.ADE3)
#define	AD_ADER_ADE2    	(IO_AD.IO_ADER.bit.ADE2)
#define	AD_ADER_ADE1    	(IO_AD.IO_ADER.bit.ADE1)
#define	AD_ADER_ADE0    	(IO_AD.IO_ADER.bit.ADE0)
#define	AD_ADCS1			(IO_AD.IO_ADCS1.byte)
#define	AD_ADCS1_BUSY    	(IO_AD.IO_ADCS1.bit.BUSY)
#define	AD_ADCS1_INT    	(IO_AD.IO_ADCS1.bit.INT)
#define	AD_ADCS1_INTE    	(IO_AD.IO_ADCS1.bit.INTE)
#define	AD_ADCS1_PAUS    	(IO_AD.IO_ADCS1.bit.PAUS)
#define	AD_ADCS1_STS    	(IO_AD.IO_ADCS1.bit.STS)
#define	AD_ADCS1_STRT    	(IO_AD.IO_ADCS1.bit.STRT)
#define	AD_ADCS0			(IO_AD.IO_ADCS0.byte)
#define	AD_ADCS0_MD    	(IO_AD.IO_ADCS0.bit.MD)
#define	AD_ADCS0_S10    	(IO_AD.IO_ADCS0.bit.S10)
#define	AD_ADCR01			(IO_AD.IO_ADCR01.hword)
#define	AD_ADCR01_ADCR1			(IO_AD.IO_ADCR01.byte.ADCR1)
#define	AD_ADCR01_ADCR0			(IO_AD.IO_ADCR01.byte.ADCR0)
#define	AD_ADCR01_D    	(IO_AD.IO_ADCR01.bit.D)
#define	AD_ADCT01			(IO_AD.IO_ADCT01.hword)
#define	AD_ADCT01_ADCT1			(IO_AD.IO_ADCT01.byte.ADCT1)
#define	AD_ADCT01_ADCT0			(IO_AD.IO_ADCT01.byte.ADCT0)
#define	AD_ADCT01_CT    	(IO_AD.IO_ADCT01.bit.CT)
#define	AD_ADCT01_ST    	(IO_AD.IO_ADCT01.bit.ST)
#define	AD_ADSCH			(IO_AD.IO_ADSCH.byte)
#define	AD_ADSCH_ANS    	(IO_AD.IO_ADSCH.bit.ANS)
#define	AD_ADECH			(IO_AD.IO_ADECH.byte)
#define	AD_ADECH_ANE    	(IO_AD.IO_ADECH.bit.ANE)
#define	AD_EADERLL			(IO_AD.IO_EADERLL.byte)
#define	AD_EADERLL_ADE39    	(IO_AD.IO_EADERLL.bit.ADE39)
#define	AD_EADERLL_ADE38    	(IO_AD.IO_EADERLL.bit.ADE38)
#define	AD_EADERLL_ADE37    	(IO_AD.IO_EADERLL.bit.ADE37)
#define	AD_EADERLL_ADE36    	(IO_AD.IO_EADERLL.bit.ADE36)
#define	AD_EADERLL_ADE35    	(IO_AD.IO_EADERLL.bit.ADE35)
#define	AD_EADERLL_ADE34    	(IO_AD.IO_EADERLL.bit.ADE34)
#define	AD_EADERLL_ADE33    	(IO_AD.IO_EADERLL.bit.ADE33)
#define	AD_EADERLL_ADE32    	(IO_AD.IO_EADERLL.bit.ADE32)
#define	AD_EADCS			(IO_AD.IO_EADCS.byte)
#define	AD_EADCS_ACH    	(IO_AD.IO_EADCS.bit.ACH)

#ifdef __IO_DEFINE
#pragma segment IO=IO_UART0, locate=0xB0
#endif

typedef union {

struct {
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
	} SCR0;
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
	} SMR0;
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
	} SSR0;
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
	} ESCR0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR0L;
		} byte;
		struct {
			struct {
				__HWORD	RESV1:7;
				__HWORD	D:9;
			} bit;
		} bit9;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	D:7;
			} bit;
		} bit7;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	D:6;
			} bit;
		} bit6;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	RESV3:1;
				__BYTE	D:5;
			} bit;
		} bit5;
	} RDR0;
	union {
		__HWORD	hword;
		struct {
			__HWORD	EXT:1;
			__HWORD	BGR:15;
		} bit;
	} BGR0;
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
	} FCR10;
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
	} FCR00;
	__HWORD	FBYTE0;
} UART0;

struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	UPCL:1;
			__BYTE	MS:1;
			__BYTE	SPI:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
			__BYTE	TBIE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} SCR0;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:3;
			__BYTE	RESV11:1;
			__BYTE	SCINV:1;
			__BYTE	BDS:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} SMR0;
	union {
		__BYTE	byte;
		struct {
			__BYTE	REC:1;
			__BYTE	RESV21:1;
			__BYTE	RESV22:1;
			__BYTE	RESV23:1;
			__BYTE	ORE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	TBI:1;
		} bit;
	} SSR0;
	union {
		__BYTE	byte;
		struct {
			__BYTE	SOP:1;
			__BYTE	RESV31:1;
			__BYTE	RESV32:1;
			__BYTE	WT:2;
			__BYTE	L:3;
		} bit;
	} ESCR0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR0L;
		} byte;
		struct {
			struct {
				__HWORD	RESV1:7;
				__HWORD	D:9;
			} bit;
		} bit9;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	D:7;
			} bit;
		} bit7;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	D:6;
			} bit;
		} bit6;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	RESV3:1;
				__BYTE	D:5;
			} bit;
		} bit5;
	} RDR0;
	__HWORD	BGR0;
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
	} FCR10;
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
	} FCR00;
	__HWORD	FBYTE0;
} CSIO0;

struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	UPCL:1;
			__BYTE	MS:1;
			__BYTE	LBR:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
			__BYTE	TBIE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} SCR0;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:3;
			__BYTE	RESV11:1;
			__BYTE	SBL:1;
			__BYTE	RESV13:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} SMR0;
	union {
		__BYTE	byte;
		struct {
			__BYTE	REC:1;
			__BYTE	RESV21:1;
			__BYTE	LBD:1;
			__BYTE	FRE:1;
			__BYTE	ORE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	TBI:1;
		} bit;
	} SSR0;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV30:1;
			__BYTE	ESBL:1;
			__BYTE	RESV32:1;
			__BYTE	LBIE:1;
			__BYTE	LBL:2;
			__BYTE	DEL:2;
		} bit;
	} ESCR0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR0L;
		} byte;
	} RDR0;
	union {
		__HWORD	hword;
		struct {
			__HWORD	EXT:1;
			__HWORD	BGR:15;
		} bit;
	} BGR0;
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
	} FCR10;
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
	} FCR00;
	__HWORD	FBYTE0;
} LINUART0;

struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	MSS:1;
			__BYTE	ACT:1;
			__BYTE	ACKE:1;
			__BYTE	WSEL:1;
			__BYTE	CNDE:1;
			__BYTE	INTE:1;
			__BYTE	BER:1;
			__BYTE	INT:1;
		} bit;
	} IBCR0;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:3;
			__BYTE	RESV11:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
			__BYTE	RESV1:1;
			__BYTE	RESV0:1;
		} bit;
	} SMR0;
	union {
		__BYTE	byte;
		struct {
			__BYTE	REC:1;
			__BYTE	TSET:1;
			__BYTE	DMA:1;
			__BYTE	TBIE:1;
			__BYTE	ORE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	TBI:1;
		} bit;
	} SSR0;
	union {
		__BYTE	byte;
		struct {
			__BYTE	FBT:1;
			__BYTE	RACK:1;
			__BYTE	RSA:1;
			__BYTE	TRX:1;
			__BYTE	AL:1;
			__BYTE	RSC:1;
			__BYTE	SPC:1;
			__BYTE	BB:1;
		} bit;
	} IBSR0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR0L;
		} byte;
	} RDR0;
	__HWORD	BGR0;
	union {
		__BYTE	byte;
		struct {
			__BYTE	EN:1;
			__BYTE	SM:7;
		} bit;
	} ISMK0;
	union {
		__BYTE	byte;
		struct {
			__BYTE	SAEN:1;
			__BYTE	SA:7;
		} bit;
	} ISBA0;
	__HWORD	IO_RESV1;
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
	} FCR10;
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
	} FCR00;
	__HWORD	FBYTE0;
} I2C0;

} UART0STR;

__IO_EXTERN	UART0STR	IO_UART0;

#define	UART0_UART0_SCR0			(IO_UART0.UART0.SCR0.byte)
#define	UART0_UART0_SCR0_UPCL    	(IO_UART0.UART0.SCR0.bit.UPCL)
#define	UART0_UART0_SCR0_RIE    	(IO_UART0.UART0.SCR0.bit.RIE)
#define	UART0_UART0_SCR0_TIE    	(IO_UART0.UART0.SCR0.bit.TIE)
#define	UART0_UART0_SCR0_TBIE    	(IO_UART0.UART0.SCR0.bit.TBIE)
#define	UART0_UART0_SCR0_RXE    	(IO_UART0.UART0.SCR0.bit.RXE)
#define	UART0_UART0_SCR0_TXE    	(IO_UART0.UART0.SCR0.bit.TXE)
#define	UART0_UART0_SMR0			(IO_UART0.UART0.SMR0.byte)
#define	UART0_UART0_SMR0_MD    	(IO_UART0.UART0.SMR0.bit.MD)
#define	UART0_UART0_SMR0_SBL    	(IO_UART0.UART0.SMR0.bit.SBL)
#define	UART0_UART0_SMR0_BDS    	(IO_UART0.UART0.SMR0.bit.BDS)
#define	UART0_UART0_SMR0_SCKE    	(IO_UART0.UART0.SMR0.bit.SCKE)
#define	UART0_UART0_SMR0_SOE    	(IO_UART0.UART0.SMR0.bit.SOE)
#define	UART0_UART0_SSR0			(IO_UART0.UART0.SSR0.byte)
#define	UART0_UART0_SSR0_REC    	(IO_UART0.UART0.SSR0.bit.REC)
#define	UART0_UART0_SSR0_PE    	(IO_UART0.UART0.SSR0.bit.PE)
#define	UART0_UART0_SSR0_FRE    	(IO_UART0.UART0.SSR0.bit.FRE)
#define	UART0_UART0_SSR0_ORE    	(IO_UART0.UART0.SSR0.bit.ORE)
#define	UART0_UART0_SSR0_RDRF    	(IO_UART0.UART0.SSR0.bit.RDRF)
#define	UART0_UART0_SSR0_TDRE    	(IO_UART0.UART0.SSR0.bit.TDRE)
#define	UART0_UART0_SSR0_TBI    	(IO_UART0.UART0.SSR0.bit.TBI)
#define	UART0_UART0_ESCR0			(IO_UART0.UART0.ESCR0.byte)
#define	UART0_UART0_ESCR0_ESBL    	(IO_UART0.UART0.ESCR0.bit.ESBL)
#define	UART0_UART0_ESCR0_INV    	(IO_UART0.UART0.ESCR0.bit.INV)
#define	UART0_UART0_ESCR0_PEN    	(IO_UART0.UART0.ESCR0.bit.PEN)
#define	UART0_UART0_ESCR0_P    	(IO_UART0.UART0.ESCR0.bit.P)
#define	UART0_UART0_ESCR0_L    	(IO_UART0.UART0.ESCR0.bit.L)
#define	UART0_UART0_RDR0	(IO_UART0.UART0.RDR0.hword)
#define	UART0_UART0_RDR0_RDR0L	(IO_UART0.UART0.RDR0.byte.RDR0L)
#define	UART0_UART0_RDR0_bit9	(IO_UART0.UART0.RDR0.bit9.bit.D)
#define	UART0_UART0_RDR0_bit7	(IO_UART0.UART0.RDR0.bit7.bit.D)
#define	UART0_UART0_RDR0_bit6	(IO_UART0.UART0.RDR0.bit6.bit.D)
#define	UART0_UART0_RDR0_bit5	(IO_UART0.UART0.RDR0.bit5.bit.D)
#define	UART0_UART0_TDR0	(IO_UART0.UART0.RDR0.hword)
#define	UART0_UART0_TDR0_TDR0L	(IO_UART0.UART0.RDR0.byte.RDR0L)
#define	UART0_UART0_TDR0_bit9	(IO_UART0.UART0.RDR0.bit9.bit.D)
#define	UART0_UART0_TDR0_bit7	(IO_UART0.UART0.RDR0.bit7.bit.D)
#define	UART0_UART0_TDR0_bit6	(IO_UART0.UART0.RDR0.bit6.bit.D)
#define	UART0_UART0_TDR0_bit5	(IO_UART0.UART0.RDR0.bit5.bit.D)
#define	UART0_UART0_BGR0			(IO_UART0.UART0.BGR0.hword)
#define	UART0_UART0_BGR0_EXT    	(IO_UART0.UART0.BGR0.bit.EXT)
#define	UART0_UART0_BGR0_BGR    	(IO_UART0.UART0.BGR0.bit.BGR)
#define	UART0_UART0_FCR10			(IO_UART0.UART0.FCR10.byte)
#define	UART0_UART0_FCR10_FLSTE    	(IO_UART0.UART0.FCR10.bit.FLSTE)
#define	UART0_UART0_FCR10_FRIIE    	(IO_UART0.UART0.FCR10.bit.FRIIE)
#define	UART0_UART0_FCR10_FDRQ    	(IO_UART0.UART0.FCR10.bit.FDRQ)
#define	UART0_UART0_FCR10_FTIE    	(IO_UART0.UART0.FCR10.bit.FTIE)
#define	UART0_UART0_FCR10_FSEL    	(IO_UART0.UART0.FCR10.bit.FSEL)
#define	UART0_UART0_FCR00			(IO_UART0.UART0.FCR00.byte)
#define	UART0_UART0_FCR00_FLST    	(IO_UART0.UART0.FCR00.bit.FLST)
#define	UART0_UART0_FCR00_FLD    	(IO_UART0.UART0.FCR00.bit.FLD)
#define	UART0_UART0_FCR00_FSET    	(IO_UART0.UART0.FCR00.bit.FSET)
#define	UART0_UART0_FCR00_FCL2    	(IO_UART0.UART0.FCR00.bit.FCL2)
#define	UART0_UART0_FCR00_FCL1    	(IO_UART0.UART0.FCR00.bit.FCL1)
#define	UART0_UART0_FCR00_FE2    	(IO_UART0.UART0.FCR00.bit.FE2)
#define	UART0_UART0_FCR00_FE1    	(IO_UART0.UART0.FCR00.bit.FE1)
#define	UART0_UART0_FBYTE0	(IO_UART0.UART0.FBYTE0)

#define	UART0_CSIO0_SCR0			(IO_UART0.CSIO0.SCR0.byte)
#define	UART0_CSIO0_SCR0_UPCL    	(IO_UART0.CSIO0.SCR0.bit.UPCL)
#define	UART0_CSIO0_SCR0_MS    	(IO_UART0.CSIO0.SCR0.bit.MS)
#define	UART0_CSIO0_SCR0_SPI    	(IO_UART0.CSIO0.SCR0.bit.SPI)
#define	UART0_CSIO0_SCR0_RIE    	(IO_UART0.CSIO0.SCR0.bit.RIE)
#define	UART0_CSIO0_SCR0_TIE    	(IO_UART0.CSIO0.SCR0.bit.TIE)
#define	UART0_CSIO0_SCR0_TBIE    	(IO_UART0.CSIO0.SCR0.bit.TBIE)
#define	UART0_CSIO0_SCR0_RXE    	(IO_UART0.CSIO0.SCR0.bit.RXE)
#define	UART0_CSIO0_SCR0_TXE    	(IO_UART0.CSIO0.SCR0.bit.TXE)
#define	UART0_CSIO0_SMR0			(IO_UART0.CSIO0.SMR0.byte)
#define	UART0_CSIO0_SMR0_MD    	(IO_UART0.CSIO0.SMR0.bit.MD)
#define	UART0_CSIO0_SMR0_SCINV    	(IO_UART0.CSIO0.SMR0.bit.SCINV)
#define	UART0_CSIO0_SMR0_BDS    	(IO_UART0.CSIO0.SMR0.bit.BDS)
#define	UART0_CSIO0_SMR0_SCKE    	(IO_UART0.CSIO0.SMR0.bit.SCKE)
#define	UART0_CSIO0_SMR0_SOE    	(IO_UART0.CSIO0.SMR0.bit.SOE)
#define	UART0_CSIO0_SSR0			(IO_UART0.CSIO0.SSR0.byte)
#define	UART0_CSIO0_SSR0_REC    	(IO_UART0.CSIO0.SSR0.bit.REC)
#define	UART0_CSIO0_SSR0_ORE    	(IO_UART0.CSIO0.SSR0.bit.ORE)
#define	UART0_CSIO0_SSR0_RDRF    	(IO_UART0.CSIO0.SSR0.bit.RDRF)
#define	UART0_CSIO0_SSR0_TDRE    	(IO_UART0.CSIO0.SSR0.bit.TDRE)
#define	UART0_CSIO0_SSR0_TBI    	(IO_UART0.CSIO0.SSR0.bit.TBI)
#define	UART0_CSIO0_ESCR0			(IO_UART0.CSIO0.ESCR0.byte)
#define	UART0_CSIO0_ESCR0_SOP    	(IO_UART0.CSIO0.ESCR0.bit.SOP)
#define	UART0_CSIO0_ESCR0_WT    	(IO_UART0.CSIO0.ESCR0.bit.WT)
#define	UART0_CSIO0_ESCR0_L    	(IO_UART0.CSIO0.ESCR0.bit.L)
#define	UART0_CSIO0_RDR0	(IO_UART0.CSIO0.RDR0.hword)
#define	UART0_CSIO0_RDR0_RDR0L	(IO_UART0.CSIO0.RDR0.byte.RDR0L)
#define	UART0_CSIO0_RDR0_bit9	(IO_UART0.CSIO0.RDR0.bit9.bit.D)
#define	UART0_CSIO0_RDR0_bit7	(IO_UART0.CSIO0.RDR0.bit7.bit.D)
#define	UART0_CSIO0_RDR0_bit6	(IO_UART0.CSIO0.RDR0.bit6.bit.D)
#define	UART0_CSIO0_RDR0_bit5	(IO_UART0.CSIO0.RDR0.bit5.bit.D)
#define	UART0_CSIO0_TDR0	(IO_UART0.CSIO0.RDR0.hword)
#define	UART0_CSIO0_TDR0_TDR0L	(IO_UART0.CSIO0.RDR0.byte.RDR0L)
#define	UART0_CSIO0_TDR0_bit9	(IO_UART0.CSIO0.RDR0.bit9.bit.D)
#define	UART0_CSIO0_TDR0_bit7	(IO_UART0.CSIO0.RDR0.bit7.bit.D)
#define	UART0_CSIO0_TDR0_bit6	(IO_UART0.CSIO0.RDR0.bit6.bit.D)
#define	UART0_CSIO0_TDR0_bit5	(IO_UART0.CSIO0.RDR0.bit5.bit.D)
#define	UART0_CSIO0_BGR0	(IO_UART0.CSIO0.BGR0)
#define	UART0_CSIO0_FCR10			(IO_UART0.CSIO0.FCR10.byte)
#define	UART0_CSIO0_FCR10_FLSTE    	(IO_UART0.CSIO0.FCR10.bit.FLSTE)
#define	UART0_CSIO0_FCR10_FRIIE    	(IO_UART0.CSIO0.FCR10.bit.FRIIE)
#define	UART0_CSIO0_FCR10_FDRQ    	(IO_UART0.CSIO0.FCR10.bit.FDRQ)
#define	UART0_CSIO0_FCR10_FTIE    	(IO_UART0.CSIO0.FCR10.bit.FTIE)
#define	UART0_CSIO0_FCR10_FSEL    	(IO_UART0.CSIO0.FCR10.bit.FSEL)
#define	UART0_CSIO0_FCR00			(IO_UART0.CSIO0.FCR00.byte)
#define	UART0_CSIO0_FCR00_FLST    	(IO_UART0.CSIO0.FCR00.bit.FLST)
#define	UART0_CSIO0_FCR00_FLD    	(IO_UART0.CSIO0.FCR00.bit.FLD)
#define	UART0_CSIO0_FCR00_FSET    	(IO_UART0.CSIO0.FCR00.bit.FSET)
#define	UART0_CSIO0_FCR00_FCL2    	(IO_UART0.CSIO0.FCR00.bit.FCL2)
#define	UART0_CSIO0_FCR00_FCL1    	(IO_UART0.CSIO0.FCR00.bit.FCL1)
#define	UART0_CSIO0_FCR00_FE2    	(IO_UART0.CSIO0.FCR00.bit.FE2)
#define	UART0_CSIO0_FCR00_FE1    	(IO_UART0.CSIO0.FCR00.bit.FE1)
#define	UART0_CSIO0_FBYTE0	(IO_UART0.CSIO0.FBYTE0)

#define	UART0_LINUART0_SCR0			(IO_UART0.LINUART0.SCR0.byte)
#define	UART0_LINUART0_SCR0_UPCL    	(IO_UART0.LINUART0.SCR0.bit.UPCL)
#define	UART0_LINUART0_SCR0_MS    	(IO_UART0.LINUART0.SCR0.bit.MS)
#define	UART0_LINUART0_SCR0_LBR    	(IO_UART0.LINUART0.SCR0.bit.LBR)
#define	UART0_LINUART0_SCR0_RIE    	(IO_UART0.LINUART0.SCR0.bit.RIE)
#define	UART0_LINUART0_SCR0_TIE    	(IO_UART0.LINUART0.SCR0.bit.TIE)
#define	UART0_LINUART0_SCR0_TBIE    	(IO_UART0.LINUART0.SCR0.bit.TBIE)
#define	UART0_LINUART0_SCR0_RXE    	(IO_UART0.LINUART0.SCR0.bit.RXE)
#define	UART0_LINUART0_SCR0_TXE    	(IO_UART0.LINUART0.SCR0.bit.TXE)
#define	UART0_LINUART0_SMR0			(IO_UART0.LINUART0.SMR0.byte)
#define	UART0_LINUART0_SMR0_MD    	(IO_UART0.LINUART0.SMR0.bit.MD)
#define	UART0_LINUART0_SMR0_SBL    	(IO_UART0.LINUART0.SMR0.bit.SBL)
#define	UART0_LINUART0_SMR0_SCKE    	(IO_UART0.LINUART0.SMR0.bit.SCKE)
#define	UART0_LINUART0_SMR0_SOE    	(IO_UART0.LINUART0.SMR0.bit.SOE)
#define	UART0_LINUART0_SSR0			(IO_UART0.LINUART0.SSR0.byte)
#define	UART0_LINUART0_SSR0_REC    	(IO_UART0.LINUART0.SSR0.bit.REC)
#define	UART0_LINUART0_SSR0_LBD    	(IO_UART0.LINUART0.SSR0.bit.LBD)
#define	UART0_LINUART0_SSR0_FRE    	(IO_UART0.LINUART0.SSR0.bit.FRE)
#define	UART0_LINUART0_SSR0_ORE    	(IO_UART0.LINUART0.SSR0.bit.ORE)
#define	UART0_LINUART0_SSR0_RDRF    	(IO_UART0.LINUART0.SSR0.bit.RDRF)
#define	UART0_LINUART0_SSR0_TDRE    	(IO_UART0.LINUART0.SSR0.bit.TDRE)
#define	UART0_LINUART0_SSR0_TBI    	(IO_UART0.LINUART0.SSR0.bit.TBI)
#define	UART0_LINUART0_ESCR0			(IO_UART0.LINUART0.ESCR0.byte)
#define	UART0_LINUART0_ESCR0_ESBL    	(IO_UART0.LINUART0.ESCR0.bit.ESBL)
#define	UART0_LINUART0_ESCR0_LBIE    	(IO_UART0.LINUART0.ESCR0.bit.LBIE)
#define	UART0_LINUART0_ESCR0_LBL    	(IO_UART0.LINUART0.ESCR0.bit.LBL)
#define	UART0_LINUART0_ESCR0_DEL    	(IO_UART0.LINUART0.ESCR0.bit.DEL)
#define	UART0_LINUART0_RDR0	(IO_UART0.LINUART0.RDR0.hword)
#define	UART0_LINUART0_RDR0_RDR0L	(IO_UART0.LINUART0.RDR0.byte.RDR0L)
#define	UART0_LINUART0_TDR0	(IO_UART0.LINUART0.RDR0.hword)
#define	UART0_LINUART0_TDR0_TDR0L	(IO_UART0.LINUART0.RDR0.byte.RDR0L)
#define	UART0_LINUART0_BGR0			(IO_UART0.LINUART0.BGR0.hword)
#define	UART0_LINUART0_BGR0_EXT    	(IO_UART0.LINUART0.BGR0.bit.EXT)
#define	UART0_LINUART0_BGR0_BGR    	(IO_UART0.LINUART0.BGR0.bit.BGR)
#define	UART0_LINUART0_FCR10			(IO_UART0.LINUART0.FCR10.byte)
#define	UART0_LINUART0_FCR10_FLSTE    	(IO_UART0.LINUART0.FCR10.bit.FLSTE)
#define	UART0_LINUART0_FCR10_FRIIE    	(IO_UART0.LINUART0.FCR10.bit.FRIIE)
#define	UART0_LINUART0_FCR10_FDRQ    	(IO_UART0.LINUART0.FCR10.bit.FDRQ)
#define	UART0_LINUART0_FCR10_FTIE    	(IO_UART0.LINUART0.FCR10.bit.FTIE)
#define	UART0_LINUART0_FCR10_FSEL    	(IO_UART0.LINUART0.FCR10.bit.FSEL)
#define	UART0_LINUART0_FCR00			(IO_UART0.LINUART0.FCR00.byte)
#define	UART0_LINUART0_FCR00_FLST    	(IO_UART0.LINUART0.FCR00.bit.FLST)
#define	UART0_LINUART0_FCR00_FLD    	(IO_UART0.LINUART0.FCR00.bit.FLD)
#define	UART0_LINUART0_FCR00_FSET    	(IO_UART0.LINUART0.FCR00.bit.FSET)
#define	UART0_LINUART0_FCR00_FCL2    	(IO_UART0.LINUART0.FCR00.bit.FCL2)
#define	UART0_LINUART0_FCR00_FCL1    	(IO_UART0.LINUART0.FCR00.bit.FCL1)
#define	UART0_LINUART0_FCR00_FE2    	(IO_UART0.LINUART0.FCR00.bit.FE2)
#define	UART0_LINUART0_FCR00_FE1    	(IO_UART0.LINUART0.FCR00.bit.FE1)
#define	UART0_LINUART0_FBYTE0	(IO_UART0.LINUART0.FBYTE0)

#define	UART0_I2C0_IBCR0			(IO_UART0.I2C0.IBCR0.byte)
#define	UART0_I2C0_IBCR0_MSS    	(IO_UART0.I2C0.IBCR0.bit.MSS)
#define	UART0_I2C0_IBCR0_ACT    	(IO_UART0.I2C0.IBCR0.bit.ACT)
#define	UART0_I2C0_IBCR0_ACKE    	(IO_UART0.I2C0.IBCR0.bit.ACKE)
#define	UART0_I2C0_IBCR0_WSEL    	(IO_UART0.I2C0.IBCR0.bit.WSEL)
#define	UART0_I2C0_IBCR0_CNDE    	(IO_UART0.I2C0.IBCR0.bit.CNDE)
#define	UART0_I2C0_IBCR0_INTE    	(IO_UART0.I2C0.IBCR0.bit.INTE)
#define	UART0_I2C0_IBCR0_BER    	(IO_UART0.I2C0.IBCR0.bit.BER)
#define	UART0_I2C0_IBCR0_INT    	(IO_UART0.I2C0.IBCR0.bit.INT)
#define	UART0_I2C0_SMR0			(IO_UART0.I2C0.SMR0.byte)
#define	UART0_I2C0_SMR0_MD    	(IO_UART0.I2C0.SMR0.bit.MD)
#define	UART0_I2C0_SMR0_RIE    	(IO_UART0.I2C0.SMR0.bit.RIE)
#define	UART0_I2C0_SMR0_TIE    	(IO_UART0.I2C0.SMR0.bit.TIE)
#define	UART0_I2C0_SSR0			(IO_UART0.I2C0.SSR0.byte)
#define	UART0_I2C0_SSR0_REC    	(IO_UART0.I2C0.SSR0.bit.REC)
#define	UART0_I2C0_SSR0_TSET    	(IO_UART0.I2C0.SSR0.bit.TSET)
#define	UART0_I2C0_SSR0_DMA    	(IO_UART0.I2C0.SSR0.bit.DMA)
#define	UART0_I2C0_SSR0_TBIE    	(IO_UART0.I2C0.SSR0.bit.TBIE)
#define	UART0_I2C0_SSR0_ORE    	(IO_UART0.I2C0.SSR0.bit.ORE)
#define	UART0_I2C0_SSR0_RDRF    	(IO_UART0.I2C0.SSR0.bit.RDRF)
#define	UART0_I2C0_SSR0_TDRE    	(IO_UART0.I2C0.SSR0.bit.TDRE)
#define	UART0_I2C0_SSR0_TBI    	(IO_UART0.I2C0.SSR0.bit.TBI)
#define	UART0_I2C0_IBSR0			(IO_UART0.I2C0.IBSR0.byte)
#define	UART0_I2C0_IBSR0_FBT    	(IO_UART0.I2C0.IBSR0.bit.FBT)
#define	UART0_I2C0_IBSR0_RACK    	(IO_UART0.I2C0.IBSR0.bit.RACK)
#define	UART0_I2C0_IBSR0_RSA    	(IO_UART0.I2C0.IBSR0.bit.RSA)
#define	UART0_I2C0_IBSR0_TRX    	(IO_UART0.I2C0.IBSR0.bit.TRX)
#define	UART0_I2C0_IBSR0_AL    	(IO_UART0.I2C0.IBSR0.bit.AL)
#define	UART0_I2C0_IBSR0_RSC    	(IO_UART0.I2C0.IBSR0.bit.RSC)
#define	UART0_I2C0_IBSR0_SPC    	(IO_UART0.I2C0.IBSR0.bit.SPC)
#define	UART0_I2C0_IBSR0_BB    	(IO_UART0.I2C0.IBSR0.bit.BB)
#define	UART0_I2C0_RDR0	(IO_UART0.I2C0.RDR0.hword)
#define	UART0_I2C0_RDR0_RDR0L	(IO_UART0.I2C0.RDR0.byte.RDR0L)
#define	UART0_I2C0_TDR0	(IO_UART0.I2C0.RDR0.hword)
#define	UART0_I2C0_TDR0_TDR0L	(IO_UART0.I2C0.RDR0.byte.RDR0L)
#define	UART0_I2C0_BGR0	(IO_UART0.I2C0.BGR0)
#define	UART0_I2C0_ISMK0			(IO_UART0.I2C0.ISMK0.byte)
#define	UART0_I2C0_ISMK0_EN    	(IO_UART0.I2C0.ISMK0.bit.EN)
#define	UART0_I2C0_ISMK0_SM    	(IO_UART0.I2C0.ISMK0.bit.SM)
#define	UART0_I2C0_ISBA0			(IO_UART0.I2C0.ISBA0.byte)
#define	UART0_I2C0_ISBA0_SAEN    	(IO_UART0.I2C0.ISBA0.bit.SAEN)
#define	UART0_I2C0_ISBA0_SA    	(IO_UART0.I2C0.ISBA0.bit.SA)
#define	UART0_I2C0_FCR10			(IO_UART0.I2C0.FCR10.byte)
#define	UART0_I2C0_FCR10_FLSTE    	(IO_UART0.I2C0.FCR10.bit.FLSTE)
#define	UART0_I2C0_FCR10_FRIIE    	(IO_UART0.I2C0.FCR10.bit.FRIIE)
#define	UART0_I2C0_FCR10_FDRQ    	(IO_UART0.I2C0.FCR10.bit.FDRQ)
#define	UART0_I2C0_FCR10_FTIE    	(IO_UART0.I2C0.FCR10.bit.FTIE)
#define	UART0_I2C0_FCR10_FSEL    	(IO_UART0.I2C0.FCR10.bit.FSEL)
#define	UART0_I2C0_FCR00			(IO_UART0.I2C0.FCR00.byte)
#define	UART0_I2C0_FCR00_FLST    	(IO_UART0.I2C0.FCR00.bit.FLST)
#define	UART0_I2C0_FCR00_FLD    	(IO_UART0.I2C0.FCR00.bit.FLD)
#define	UART0_I2C0_FCR00_FSET    	(IO_UART0.I2C0.FCR00.bit.FSET)
#define	UART0_I2C0_FCR00_FCL2    	(IO_UART0.I2C0.FCR00.bit.FCL2)
#define	UART0_I2C0_FCR00_FCL1    	(IO_UART0.I2C0.FCR00.bit.FCL1)
#define	UART0_I2C0_FCR00_FE2    	(IO_UART0.I2C0.FCR00.bit.FE2)
#define	UART0_I2C0_FCR00_FE1    	(IO_UART0.I2C0.FCR00.bit.FE1)
#define	UART0_I2C0_FBYTE0	(IO_UART0.I2C0.FBYTE0)

#ifdef __IO_DEFINE
#pragma segment IO=IO_UART1, locate=0xC0
#endif

typedef union {

struct {
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
	} SCR1;
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
	} SMR1;
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
	} SSR1;
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
	} ESCR1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR1L;
		} byte;
		struct {
			struct {
				__HWORD	RESV1:7;
				__HWORD	D:9;
			} bit;
		} bit9;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	D:7;
			} bit;
		} bit7;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	D:6;
			} bit;
		} bit6;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	RESV3:1;
				__BYTE	D:5;
			} bit;
		} bit5;
	} RDR1;
	union {
		__HWORD	hword;
		struct {
			__HWORD	EXT:1;
			__HWORD	BGR:15;
		} bit;
	} BGR1;
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
	} FCR11;
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
	} FCR01;
	__HWORD	FBYTE1;
} UART1;

struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	UPCL:1;
			__BYTE	MS:1;
			__BYTE	SPI:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
			__BYTE	TBIE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} SCR1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:3;
			__BYTE	RESV11:1;
			__BYTE	SCINV:1;
			__BYTE	BDS:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} SMR1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	REC:1;
			__BYTE	RESV21:1;
			__BYTE	RESV22:1;
			__BYTE	RESV23:1;
			__BYTE	ORE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	TBI:1;
		} bit;
	} SSR1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	SOP:1;
			__BYTE	RESV31:1;
			__BYTE	RESV32:1;
			__BYTE	WT:2;
			__BYTE	L:3;
		} bit;
	} ESCR1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR1L;
		} byte;
		struct {
			struct {
				__HWORD	RESV1:7;
				__HWORD	D:9;
			} bit;
		} bit9;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	D:7;
			} bit;
		} bit7;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	D:6;
			} bit;
		} bit6;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	RESV3:1;
				__BYTE	D:5;
			} bit;
		} bit5;
	} RDR1;
	__HWORD	BGR1;
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
	} FCR11;
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
	} FCR01;
	__HWORD	FBYTE1;
} CSIO1;

struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	UPCL:1;
			__BYTE	MS:1;
			__BYTE	LBR:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
			__BYTE	TBIE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} SCR1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:3;
			__BYTE	RESV11:1;
			__BYTE	SBL:1;
			__BYTE	RESV13:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} SMR1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	REC:1;
			__BYTE	RESV21:1;
			__BYTE	LBD:1;
			__BYTE	FRE:1;
			__BYTE	ORE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	TBI:1;
		} bit;
	} SSR1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV30:1;
			__BYTE	ESBL:1;
			__BYTE	RESV32:1;
			__BYTE	LBIE:1;
			__BYTE	LBL:2;
			__BYTE	DEL:2;
		} bit;
	} ESCR1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR1L;
		} byte;
	} RDR1;
	union {
		__HWORD	hword;
		struct {
			__HWORD	EXT:1;
			__HWORD	BGR:15;
		} bit;
	} BGR1;
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
	} FCR11;
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
	} FCR01;
	__HWORD	FBYTE1;
} LINUART1;

struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	MSS:1;
			__BYTE	ACT:1;
			__BYTE	ACKE:1;
			__BYTE	WSEL:1;
			__BYTE	CNDE:1;
			__BYTE	INTE:1;
			__BYTE	BER:1;
			__BYTE	INT:1;
		} bit;
	} IBCR1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:3;
			__BYTE	RESV11:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
			__BYTE	RESV1:1;
			__BYTE	RESV0:1;
		} bit;
	} SMR1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	REC:1;
			__BYTE	TSET:1;
			__BYTE	DMA:1;
			__BYTE	TBIE:1;
			__BYTE	ORE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	TBI:1;
		} bit;
	} SSR1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	FBT:1;
			__BYTE	RACK:1;
			__BYTE	RSA:1;
			__BYTE	TRX:1;
			__BYTE	AL:1;
			__BYTE	RSC:1;
			__BYTE	SPC:1;
			__BYTE	BB:1;
		} bit;
	} IBSR1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR1L;
		} byte;
	} RDR1;
	__HWORD	BGR1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	EN:1;
			__BYTE	SM:7;
		} bit;
	} ISMK1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	SAEN:1;
			__BYTE	SA:7;
		} bit;
	} ISBA1;
	__HWORD	IO_RESV1;
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
	} FCR11;
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
	} FCR01;
	__HWORD	FBYTE1;
} I2C1;

} UART1STR;

__IO_EXTERN	UART1STR	IO_UART1;

#define	UART1_UART1_SCR1			(IO_UART1.UART1.SCR1.byte)
#define	UART1_UART1_SCR1_UPCL    	(IO_UART1.UART1.SCR1.bit.UPCL)
#define	UART1_UART1_SCR1_RIE    	(IO_UART1.UART1.SCR1.bit.RIE)
#define	UART1_UART1_SCR1_TIE    	(IO_UART1.UART1.SCR1.bit.TIE)
#define	UART1_UART1_SCR1_TBIE    	(IO_UART1.UART1.SCR1.bit.TBIE)
#define	UART1_UART1_SCR1_RXE    	(IO_UART1.UART1.SCR1.bit.RXE)
#define	UART1_UART1_SCR1_TXE    	(IO_UART1.UART1.SCR1.bit.TXE)
#define	UART1_UART1_SMR1			(IO_UART1.UART1.SMR1.byte)
#define	UART1_UART1_SMR1_MD    	(IO_UART1.UART1.SMR1.bit.MD)
#define	UART1_UART1_SMR1_SBL    	(IO_UART1.UART1.SMR1.bit.SBL)
#define	UART1_UART1_SMR1_BDS    	(IO_UART1.UART1.SMR1.bit.BDS)
#define	UART1_UART1_SMR1_SCKE    	(IO_UART1.UART1.SMR1.bit.SCKE)
#define	UART1_UART1_SMR1_SOE    	(IO_UART1.UART1.SMR1.bit.SOE)
#define	UART1_UART1_SSR1			(IO_UART1.UART1.SSR1.byte)
#define	UART1_UART1_SSR1_REC    	(IO_UART1.UART1.SSR1.bit.REC)
#define	UART1_UART1_SSR1_PE    	(IO_UART1.UART1.SSR1.bit.PE)
#define	UART1_UART1_SSR1_FRE    	(IO_UART1.UART1.SSR1.bit.FRE)
#define	UART1_UART1_SSR1_ORE    	(IO_UART1.UART1.SSR1.bit.ORE)
#define	UART1_UART1_SSR1_RDRF    	(IO_UART1.UART1.SSR1.bit.RDRF)
#define	UART1_UART1_SSR1_TDRE    	(IO_UART1.UART1.SSR1.bit.TDRE)
#define	UART1_UART1_SSR1_TBI    	(IO_UART1.UART1.SSR1.bit.TBI)
#define	UART1_UART1_ESCR1			(IO_UART1.UART1.ESCR1.byte)
#define	UART1_UART1_ESCR1_ESBL    	(IO_UART1.UART1.ESCR1.bit.ESBL)
#define	UART1_UART1_ESCR1_INV    	(IO_UART1.UART1.ESCR1.bit.INV)
#define	UART1_UART1_ESCR1_PEN    	(IO_UART1.UART1.ESCR1.bit.PEN)
#define	UART1_UART1_ESCR1_P    	(IO_UART1.UART1.ESCR1.bit.P)
#define	UART1_UART1_ESCR1_L    	(IO_UART1.UART1.ESCR1.bit.L)
#define	UART1_UART1_RDR1	(IO_UART1.UART1.RDR1.hword)
#define	UART1_UART1_RDR1_RDR1L	(IO_UART1.UART1.RDR1.byte.RDR1L)
#define	UART1_UART1_RDR1_bit9	(IO_UART1.UART1.RDR1.bit9.bit.D)
#define	UART1_UART1_RDR1_bit7	(IO_UART1.UART1.RDR1.bit7.bit.D)
#define	UART1_UART1_RDR1_bit6	(IO_UART1.UART1.RDR1.bit6.bit.D)
#define	UART1_UART1_RDR1_bit5	(IO_UART1.UART1.RDR1.bit5.bit.D)
#define	UART1_UART1_TDR1	(IO_UART1.UART1.RDR1.hword)
#define	UART1_UART1_TDR1_TDR1L	(IO_UART1.UART1.RDR1.byte.RDR1L)
#define	UART1_UART1_TDR1_bit9	(IO_UART1.UART1.RDR1.bit9.bit.D)
#define	UART1_UART1_TDR1_bit7	(IO_UART1.UART1.RDR1.bit7.bit.D)
#define	UART1_UART1_TDR1_bit6	(IO_UART1.UART1.RDR1.bit6.bit.D)
#define	UART1_UART1_TDR1_bit5	(IO_UART1.UART1.RDR1.bit5.bit.D)
#define	UART1_UART1_BGR1			(IO_UART1.UART1.BGR1.hword)
#define	UART1_UART1_BGR1_EXT    	(IO_UART1.UART1.BGR1.bit.EXT)
#define	UART1_UART1_BGR1_BGR    	(IO_UART1.UART1.BGR1.bit.BGR)
#define	UART1_UART1_FCR11			(IO_UART1.UART1.FCR11.byte)
#define	UART1_UART1_FCR11_FLSTE    	(IO_UART1.UART1.FCR11.bit.FLSTE)
#define	UART1_UART1_FCR11_FRIIE    	(IO_UART1.UART1.FCR11.bit.FRIIE)
#define	UART1_UART1_FCR11_FDRQ    	(IO_UART1.UART1.FCR11.bit.FDRQ)
#define	UART1_UART1_FCR11_FTIE    	(IO_UART1.UART1.FCR11.bit.FTIE)
#define	UART1_UART1_FCR11_FSEL    	(IO_UART1.UART1.FCR11.bit.FSEL)
#define	UART1_UART1_FCR01			(IO_UART1.UART1.FCR01.byte)
#define	UART1_UART1_FCR01_FLST    	(IO_UART1.UART1.FCR01.bit.FLST)
#define	UART1_UART1_FCR01_FLD    	(IO_UART1.UART1.FCR01.bit.FLD)
#define	UART1_UART1_FCR01_FSET    	(IO_UART1.UART1.FCR01.bit.FSET)
#define	UART1_UART1_FCR01_FCL2    	(IO_UART1.UART1.FCR01.bit.FCL2)
#define	UART1_UART1_FCR01_FCL1    	(IO_UART1.UART1.FCR01.bit.FCL1)
#define	UART1_UART1_FCR01_FE2    	(IO_UART1.UART1.FCR01.bit.FE2)
#define	UART1_UART1_FCR01_FE1    	(IO_UART1.UART1.FCR01.bit.FE1)
#define	UART1_UART1_FBYTE1	(IO_UART1.UART1.FBYTE1)

#define	UART1_CSIO1_SCR1			(IO_UART1.CSIO1.SCR1.byte)
#define	UART1_CSIO1_SCR1_UPCL    	(IO_UART1.CSIO1.SCR1.bit.UPCL)
#define	UART1_CSIO1_SCR1_MS    	(IO_UART1.CSIO1.SCR1.bit.MS)
#define	UART1_CSIO1_SCR1_SPI    	(IO_UART1.CSIO1.SCR1.bit.SPI)
#define	UART1_CSIO1_SCR1_RIE    	(IO_UART1.CSIO1.SCR1.bit.RIE)
#define	UART1_CSIO1_SCR1_TIE    	(IO_UART1.CSIO1.SCR1.bit.TIE)
#define	UART1_CSIO1_SCR1_TBIE    	(IO_UART1.CSIO1.SCR1.bit.TBIE)
#define	UART1_CSIO1_SCR1_RXE    	(IO_UART1.CSIO1.SCR1.bit.RXE)
#define	UART1_CSIO1_SCR1_TXE    	(IO_UART1.CSIO1.SCR1.bit.TXE)
#define	UART1_CSIO1_SMR1			(IO_UART1.CSIO1.SMR1.byte)
#define	UART1_CSIO1_SMR1_MD    	(IO_UART1.CSIO1.SMR1.bit.MD)
#define	UART1_CSIO1_SMR1_SCINV    	(IO_UART1.CSIO1.SMR1.bit.SCINV)
#define	UART1_CSIO1_SMR1_BDS    	(IO_UART1.CSIO1.SMR1.bit.BDS)
#define	UART1_CSIO1_SMR1_SCKE    	(IO_UART1.CSIO1.SMR1.bit.SCKE)
#define	UART1_CSIO1_SMR1_SOE    	(IO_UART1.CSIO1.SMR1.bit.SOE)
#define	UART1_CSIO1_SSR1			(IO_UART1.CSIO1.SSR1.byte)
#define	UART1_CSIO1_SSR1_REC    	(IO_UART1.CSIO1.SSR1.bit.REC)
#define	UART1_CSIO1_SSR1_ORE    	(IO_UART1.CSIO1.SSR1.bit.ORE)
#define	UART1_CSIO1_SSR1_RDRF    	(IO_UART1.CSIO1.SSR1.bit.RDRF)
#define	UART1_CSIO1_SSR1_TDRE    	(IO_UART1.CSIO1.SSR1.bit.TDRE)
#define	UART1_CSIO1_SSR1_TBI    	(IO_UART1.CSIO1.SSR1.bit.TBI)
#define	UART1_CSIO1_ESCR1			(IO_UART1.CSIO1.ESCR1.byte)
#define	UART1_CSIO1_ESCR1_SOP    	(IO_UART1.CSIO1.ESCR1.bit.SOP)
#define	UART1_CSIO1_ESCR1_WT    	(IO_UART1.CSIO1.ESCR1.bit.WT)
#define	UART1_CSIO1_ESCR1_L    	(IO_UART1.CSIO1.ESCR1.bit.L)
#define	UART1_CSIO1_RDR1	(IO_UART1.CSIO1.RDR1.hword)
#define	UART1_CSIO1_RDR1_RDR1L	(IO_UART1.CSIO1.RDR1.byte.RDR1L)
#define	UART1_CSIO1_RDR1_bit9	(IO_UART1.CSIO1.RDR1.bit9.bit.D)
#define	UART1_CSIO1_RDR1_bit7	(IO_UART1.CSIO1.RDR1.bit7.bit.D)
#define	UART1_CSIO1_RDR1_bit6	(IO_UART1.CSIO1.RDR1.bit6.bit.D)
#define	UART1_CSIO1_RDR1_bit5	(IO_UART1.CSIO1.RDR1.bit5.bit.D)
#define	UART1_CSIO1_TDR1	(IO_UART1.CSIO1.RDR1.hword)
#define	UART1_CSIO1_TDR1_TDR1L	(IO_UART1.CSIO1.RDR1.byte.RDR1L)
#define	UART1_CSIO1_TDR1_bit9	(IO_UART1.CSIO1.RDR1.bit9.bit.D)
#define	UART1_CSIO1_TDR1_bit7	(IO_UART1.CSIO1.RDR1.bit7.bit.D)
#define	UART1_CSIO1_TDR1_bit6	(IO_UART1.CSIO1.RDR1.bit6.bit.D)
#define	UART1_CSIO1_TDR1_bit5	(IO_UART1.CSIO1.RDR1.bit5.bit.D)
#define	UART1_CSIO1_BGR1	(IO_UART1.CSIO1.BGR1)
#define	UART1_CSIO1_FCR11			(IO_UART1.CSIO1.FCR11.byte)
#define	UART1_CSIO1_FCR11_FLSTE    	(IO_UART1.CSIO1.FCR11.bit.FLSTE)
#define	UART1_CSIO1_FCR11_FRIIE    	(IO_UART1.CSIO1.FCR11.bit.FRIIE)
#define	UART1_CSIO1_FCR11_FDRQ    	(IO_UART1.CSIO1.FCR11.bit.FDRQ)
#define	UART1_CSIO1_FCR11_FTIE    	(IO_UART1.CSIO1.FCR11.bit.FTIE)
#define	UART1_CSIO1_FCR11_FSEL    	(IO_UART1.CSIO1.FCR11.bit.FSEL)
#define	UART1_CSIO1_FCR01			(IO_UART1.CSIO1.FCR01.byte)
#define	UART1_CSIO1_FCR01_FLST    	(IO_UART1.CSIO1.FCR01.bit.FLST)
#define	UART1_CSIO1_FCR01_FLD    	(IO_UART1.CSIO1.FCR01.bit.FLD)
#define	UART1_CSIO1_FCR01_FSET    	(IO_UART1.CSIO1.FCR01.bit.FSET)
#define	UART1_CSIO1_FCR01_FCL2    	(IO_UART1.CSIO1.FCR01.bit.FCL2)
#define	UART1_CSIO1_FCR01_FCL1    	(IO_UART1.CSIO1.FCR01.bit.FCL1)
#define	UART1_CSIO1_FCR01_FE2    	(IO_UART1.CSIO1.FCR01.bit.FE2)
#define	UART1_CSIO1_FCR01_FE1    	(IO_UART1.CSIO1.FCR01.bit.FE1)
#define	UART1_CSIO1_FBYTE1	(IO_UART1.CSIO1.FBYTE1)

#define	UART1_LINUART1_SCR1			(IO_UART1.LINUART1.SCR1.byte)
#define	UART1_LINUART1_SCR1_UPCL    	(IO_UART1.LINUART1.SCR1.bit.UPCL)
#define	UART1_LINUART1_SCR1_MS    	(IO_UART1.LINUART1.SCR1.bit.MS)
#define	UART1_LINUART1_SCR1_LBR    	(IO_UART1.LINUART1.SCR1.bit.LBR)
#define	UART1_LINUART1_SCR1_RIE    	(IO_UART1.LINUART1.SCR1.bit.RIE)
#define	UART1_LINUART1_SCR1_TIE    	(IO_UART1.LINUART1.SCR1.bit.TIE)
#define	UART1_LINUART1_SCR1_TBIE    	(IO_UART1.LINUART1.SCR1.bit.TBIE)
#define	UART1_LINUART1_SCR1_RXE    	(IO_UART1.LINUART1.SCR1.bit.RXE)
#define	UART1_LINUART1_SCR1_TXE    	(IO_UART1.LINUART1.SCR1.bit.TXE)
#define	UART1_LINUART1_SMR1			(IO_UART1.LINUART1.SMR1.byte)
#define	UART1_LINUART1_SMR1_MD    	(IO_UART1.LINUART1.SMR1.bit.MD)
#define	UART1_LINUART1_SMR1_SBL    	(IO_UART1.LINUART1.SMR1.bit.SBL)
#define	UART1_LINUART1_SMR1_SCKE    	(IO_UART1.LINUART1.SMR1.bit.SCKE)
#define	UART1_LINUART1_SMR1_SOE    	(IO_UART1.LINUART1.SMR1.bit.SOE)
#define	UART1_LINUART1_SSR1			(IO_UART1.LINUART1.SSR1.byte)
#define	UART1_LINUART1_SSR1_REC    	(IO_UART1.LINUART1.SSR1.bit.REC)
#define	UART1_LINUART1_SSR1_LBD    	(IO_UART1.LINUART1.SSR1.bit.LBD)
#define	UART1_LINUART1_SSR1_FRE    	(IO_UART1.LINUART1.SSR1.bit.FRE)
#define	UART1_LINUART1_SSR1_ORE    	(IO_UART1.LINUART1.SSR1.bit.ORE)
#define	UART1_LINUART1_SSR1_RDRF    	(IO_UART1.LINUART1.SSR1.bit.RDRF)
#define	UART1_LINUART1_SSR1_TDRE    	(IO_UART1.LINUART1.SSR1.bit.TDRE)
#define	UART1_LINUART1_SSR1_TBI    	(IO_UART1.LINUART1.SSR1.bit.TBI)
#define	UART1_LINUART1_ESCR1			(IO_UART1.LINUART1.ESCR1.byte)
#define	UART1_LINUART1_ESCR1_ESBL    	(IO_UART1.LINUART1.ESCR1.bit.ESBL)
#define	UART1_LINUART1_ESCR1_LBIE    	(IO_UART1.LINUART1.ESCR1.bit.LBIE)
#define	UART1_LINUART1_ESCR1_LBL    	(IO_UART1.LINUART1.ESCR1.bit.LBL)
#define	UART1_LINUART1_ESCR1_DEL    	(IO_UART1.LINUART1.ESCR1.bit.DEL)
#define	UART1_LINUART1_RDR1	(IO_UART1.LINUART1.RDR1.hword)
#define	UART1_LINUART1_RDR1_RDR1L	(IO_UART1.LINUART1.RDR1.byte.RDR1L)
#define	UART1_LINUART1_TDR1	(IO_UART1.LINUART1.RDR1.hword)
#define	UART1_LINUART1_TDR1_TDR1L	(IO_UART1.LINUART1.RDR1.byte.RDR1L)
#define	UART1_LINUART1_BGR1			(IO_UART1.LINUART1.BGR1.hword)
#define	UART1_LINUART1_BGR1_EXT    	(IO_UART1.LINUART1.BGR1.bit.EXT)
#define	UART1_LINUART1_BGR1_BGR    	(IO_UART1.LINUART1.BGR1.bit.BGR)
#define	UART1_LINUART1_FCR11			(IO_UART1.LINUART1.FCR11.byte)
#define	UART1_LINUART1_FCR11_FLSTE    	(IO_UART1.LINUART1.FCR11.bit.FLSTE)
#define	UART1_LINUART1_FCR11_FRIIE    	(IO_UART1.LINUART1.FCR11.bit.FRIIE)
#define	UART1_LINUART1_FCR11_FDRQ    	(IO_UART1.LINUART1.FCR11.bit.FDRQ)
#define	UART1_LINUART1_FCR11_FTIE    	(IO_UART1.LINUART1.FCR11.bit.FTIE)
#define	UART1_LINUART1_FCR11_FSEL    	(IO_UART1.LINUART1.FCR11.bit.FSEL)
#define	UART1_LINUART1_FCR01			(IO_UART1.LINUART1.FCR01.byte)
#define	UART1_LINUART1_FCR01_FLST    	(IO_UART1.LINUART1.FCR01.bit.FLST)
#define	UART1_LINUART1_FCR01_FLD    	(IO_UART1.LINUART1.FCR01.bit.FLD)
#define	UART1_LINUART1_FCR01_FSET    	(IO_UART1.LINUART1.FCR01.bit.FSET)
#define	UART1_LINUART1_FCR01_FCL2    	(IO_UART1.LINUART1.FCR01.bit.FCL2)
#define	UART1_LINUART1_FCR01_FCL1    	(IO_UART1.LINUART1.FCR01.bit.FCL1)
#define	UART1_LINUART1_FCR01_FE2    	(IO_UART1.LINUART1.FCR01.bit.FE2)
#define	UART1_LINUART1_FCR01_FE1    	(IO_UART1.LINUART1.FCR01.bit.FE1)
#define	UART1_LINUART1_FBYTE1	(IO_UART1.LINUART1.FBYTE1)

#define	UART1_I2C1_IBCR1			(IO_UART1.I2C1.IBCR1.byte)
#define	UART1_I2C1_IBCR1_MSS    	(IO_UART1.I2C1.IBCR1.bit.MSS)
#define	UART1_I2C1_IBCR1_ACT    	(IO_UART1.I2C1.IBCR1.bit.ACT)
#define	UART1_I2C1_IBCR1_ACKE    	(IO_UART1.I2C1.IBCR1.bit.ACKE)
#define	UART1_I2C1_IBCR1_WSEL    	(IO_UART1.I2C1.IBCR1.bit.WSEL)
#define	UART1_I2C1_IBCR1_CNDE    	(IO_UART1.I2C1.IBCR1.bit.CNDE)
#define	UART1_I2C1_IBCR1_INTE    	(IO_UART1.I2C1.IBCR1.bit.INTE)
#define	UART1_I2C1_IBCR1_BER    	(IO_UART1.I2C1.IBCR1.bit.BER)
#define	UART1_I2C1_IBCR1_INT    	(IO_UART1.I2C1.IBCR1.bit.INT)
#define	UART1_I2C1_SMR1			(IO_UART1.I2C1.SMR1.byte)
#define	UART1_I2C1_SMR1_MD    	(IO_UART1.I2C1.SMR1.bit.MD)
#define	UART1_I2C1_SMR1_RIE    	(IO_UART1.I2C1.SMR1.bit.RIE)
#define	UART1_I2C1_SMR1_TIE    	(IO_UART1.I2C1.SMR1.bit.TIE)
#define	UART1_I2C1_SSR1			(IO_UART1.I2C1.SSR1.byte)
#define	UART1_I2C1_SSR1_REC    	(IO_UART1.I2C1.SSR1.bit.REC)
#define	UART1_I2C1_SSR1_TSET    	(IO_UART1.I2C1.SSR1.bit.TSET)
#define	UART1_I2C1_SSR1_DMA    	(IO_UART1.I2C1.SSR1.bit.DMA)
#define	UART1_I2C1_SSR1_TBIE    	(IO_UART1.I2C1.SSR1.bit.TBIE)
#define	UART1_I2C1_SSR1_ORE    	(IO_UART1.I2C1.SSR1.bit.ORE)
#define	UART1_I2C1_SSR1_RDRF    	(IO_UART1.I2C1.SSR1.bit.RDRF)
#define	UART1_I2C1_SSR1_TDRE    	(IO_UART1.I2C1.SSR1.bit.TDRE)
#define	UART1_I2C1_SSR1_TBI    	(IO_UART1.I2C1.SSR1.bit.TBI)
#define	UART1_I2C1_IBSR1			(IO_UART1.I2C1.IBSR1.byte)
#define	UART1_I2C1_IBSR1_FBT    	(IO_UART1.I2C1.IBSR1.bit.FBT)
#define	UART1_I2C1_IBSR1_RACK    	(IO_UART1.I2C1.IBSR1.bit.RACK)
#define	UART1_I2C1_IBSR1_RSA    	(IO_UART1.I2C1.IBSR1.bit.RSA)
#define	UART1_I2C1_IBSR1_TRX    	(IO_UART1.I2C1.IBSR1.bit.TRX)
#define	UART1_I2C1_IBSR1_AL    	(IO_UART1.I2C1.IBSR1.bit.AL)
#define	UART1_I2C1_IBSR1_RSC    	(IO_UART1.I2C1.IBSR1.bit.RSC)
#define	UART1_I2C1_IBSR1_SPC    	(IO_UART1.I2C1.IBSR1.bit.SPC)
#define	UART1_I2C1_IBSR1_BB    	(IO_UART1.I2C1.IBSR1.bit.BB)
#define	UART1_I2C1_RDR1	(IO_UART1.I2C1.RDR1.hword)
#define	UART1_I2C1_RDR1_RDR1L	(IO_UART1.I2C1.RDR1.byte.RDR1L)
#define	UART1_I2C1_TDR1	(IO_UART1.I2C1.RDR1.hword)
#define	UART1_I2C1_TDR1_TDR1L	(IO_UART1.I2C1.RDR1.byte.RDR1L)
#define	UART1_I2C1_BGR1	(IO_UART1.I2C1.BGR1)
#define	UART1_I2C1_ISMK1			(IO_UART1.I2C1.ISMK1.byte)
#define	UART1_I2C1_ISMK1_EN    	(IO_UART1.I2C1.ISMK1.bit.EN)
#define	UART1_I2C1_ISMK1_SM    	(IO_UART1.I2C1.ISMK1.bit.SM)
#define	UART1_I2C1_ISBA1			(IO_UART1.I2C1.ISBA1.byte)
#define	UART1_I2C1_ISBA1_SAEN    	(IO_UART1.I2C1.ISBA1.bit.SAEN)
#define	UART1_I2C1_ISBA1_SA    	(IO_UART1.I2C1.ISBA1.bit.SA)
#define	UART1_I2C1_FCR11			(IO_UART1.I2C1.FCR11.byte)
#define	UART1_I2C1_FCR11_FLSTE    	(IO_UART1.I2C1.FCR11.bit.FLSTE)
#define	UART1_I2C1_FCR11_FRIIE    	(IO_UART1.I2C1.FCR11.bit.FRIIE)
#define	UART1_I2C1_FCR11_FDRQ    	(IO_UART1.I2C1.FCR11.bit.FDRQ)
#define	UART1_I2C1_FCR11_FTIE    	(IO_UART1.I2C1.FCR11.bit.FTIE)
#define	UART1_I2C1_FCR11_FSEL    	(IO_UART1.I2C1.FCR11.bit.FSEL)
#define	UART1_I2C1_FCR01			(IO_UART1.I2C1.FCR01.byte)
#define	UART1_I2C1_FCR01_FLST    	(IO_UART1.I2C1.FCR01.bit.FLST)
#define	UART1_I2C1_FCR01_FLD    	(IO_UART1.I2C1.FCR01.bit.FLD)
#define	UART1_I2C1_FCR01_FSET    	(IO_UART1.I2C1.FCR01.bit.FSET)
#define	UART1_I2C1_FCR01_FCL2    	(IO_UART1.I2C1.FCR01.bit.FCL2)
#define	UART1_I2C1_FCR01_FCL1    	(IO_UART1.I2C1.FCR01.bit.FCL1)
#define	UART1_I2C1_FCR01_FE2    	(IO_UART1.I2C1.FCR01.bit.FE2)
#define	UART1_I2C1_FCR01_FE1    	(IO_UART1.I2C1.FCR01.bit.FE1)
#define	UART1_I2C1_FBYTE1	(IO_UART1.I2C1.FBYTE1)

#ifdef __IO_DEFINE
#pragma segment IO=IO_LIN_UART2, locate=0xD0
#endif

__IO_EXTERN	struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	PEN:1;
			__BYTE	P:1;
			__BYTE	SBL:1;
			__BYTE	CL:1;
			__BYTE	AD:1;
			__BYTE	CRE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} IO_SCR2;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:2;
			__BYTE	OTO:1;
			__BYTE	EXT:1;
			__BYTE	REST:1;
			__BYTE	UPCL:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} IO_SMR2;
	union {
		__BYTE	byte;
		struct {
			__BYTE	PE:1;
			__BYTE	ORE:1;
			__BYTE	FRE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	BDS:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
		} bit;
	} IO_SSR2;
	__BYTE	IO_RDR2;
	union {
		__BYTE	byte;
		struct {
			__BYTE	LBIE:1;
			__BYTE	LBD:1;
			__BYTE	LBL:2;
			__BYTE	SOPE:1;
			__BYTE	SIOP:1;
			__BYTE	CCO:1;
			__BYTE	SCES:1;
		} bit;
	} IO_ESCR2;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV50:1;
			__BYTE	LBR:1;
			__BYTE	MS:1;
			__BYTE	SCDE:1;
			__BYTE	SSM:1;
			__BYTE	RESV55:1;
			__BYTE	RBI:1;
			__BYTE	TBI:1;
		} bit;
	} IO_ECCR2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	BGR012;
			__BYTE	BGR002;
		} byte;
		struct {
			__HWORD	RESV0:1;
			__HWORD	BGR:15;
		} bit;
	} IO_BGR2;
} IO_LIN_UART2;

#define	LIN_UART2_SCR2			(IO_LIN_UART2.IO_SCR2.byte)
#define	LIN_UART2_SCR2_PEN    	(IO_LIN_UART2.IO_SCR2.bit.PEN)
#define	LIN_UART2_SCR2_P    	(IO_LIN_UART2.IO_SCR2.bit.P)
#define	LIN_UART2_SCR2_SBL    	(IO_LIN_UART2.IO_SCR2.bit.SBL)
#define	LIN_UART2_SCR2_CL    	(IO_LIN_UART2.IO_SCR2.bit.CL)
#define	LIN_UART2_SCR2_AD    	(IO_LIN_UART2.IO_SCR2.bit.AD)
#define	LIN_UART2_SCR2_CRE    	(IO_LIN_UART2.IO_SCR2.bit.CRE)
#define	LIN_UART2_SCR2_RXE    	(IO_LIN_UART2.IO_SCR2.bit.RXE)
#define	LIN_UART2_SCR2_TXE    	(IO_LIN_UART2.IO_SCR2.bit.TXE)
#define	LIN_UART2_SMR2			(IO_LIN_UART2.IO_SMR2.byte)
#define	LIN_UART2_SMR2_MD    	(IO_LIN_UART2.IO_SMR2.bit.MD)
#define	LIN_UART2_SMR2_OTO    	(IO_LIN_UART2.IO_SMR2.bit.OTO)
#define	LIN_UART2_SMR2_EXT    	(IO_LIN_UART2.IO_SMR2.bit.EXT)
#define	LIN_UART2_SMR2_REST    	(IO_LIN_UART2.IO_SMR2.bit.REST)
#define	LIN_UART2_SMR2_UPCL    	(IO_LIN_UART2.IO_SMR2.bit.UPCL)
#define	LIN_UART2_SMR2_SCKE    	(IO_LIN_UART2.IO_SMR2.bit.SCKE)
#define	LIN_UART2_SMR2_SOE    	(IO_LIN_UART2.IO_SMR2.bit.SOE)
#define	LIN_UART2_SSR2			(IO_LIN_UART2.IO_SSR2.byte)
#define	LIN_UART2_SSR2_PE    	(IO_LIN_UART2.IO_SSR2.bit.PE)
#define	LIN_UART2_SSR2_ORE    	(IO_LIN_UART2.IO_SSR2.bit.ORE)
#define	LIN_UART2_SSR2_FRE    	(IO_LIN_UART2.IO_SSR2.bit.FRE)
#define	LIN_UART2_SSR2_RDRF    	(IO_LIN_UART2.IO_SSR2.bit.RDRF)
#define	LIN_UART2_SSR2_TDRE    	(IO_LIN_UART2.IO_SSR2.bit.TDRE)
#define	LIN_UART2_SSR2_BDS    	(IO_LIN_UART2.IO_SSR2.bit.BDS)
#define	LIN_UART2_SSR2_RIE    	(IO_LIN_UART2.IO_SSR2.bit.RIE)
#define	LIN_UART2_SSR2_TIE    	(IO_LIN_UART2.IO_SSR2.bit.TIE)
#define	LIN_UART2_RDR2	(IO_LIN_UART2.IO_RDR2)
#define	LIN_UART2_TDR2	(IO_LIN_UART2.IO_RDR2)
#define	LIN_UART2_ESCR2			(IO_LIN_UART2.IO_ESCR2.byte)
#define	LIN_UART2_ESCR2_LBIE    	(IO_LIN_UART2.IO_ESCR2.bit.LBIE)
#define	LIN_UART2_ESCR2_LBD    	(IO_LIN_UART2.IO_ESCR2.bit.LBD)
#define	LIN_UART2_ESCR2_LBL    	(IO_LIN_UART2.IO_ESCR2.bit.LBL)
#define	LIN_UART2_ESCR2_SOPE    	(IO_LIN_UART2.IO_ESCR2.bit.SOPE)
#define	LIN_UART2_ESCR2_SIOP    	(IO_LIN_UART2.IO_ESCR2.bit.SIOP)
#define	LIN_UART2_ESCR2_CCO    	(IO_LIN_UART2.IO_ESCR2.bit.CCO)
#define	LIN_UART2_ESCR2_SCES    	(IO_LIN_UART2.IO_ESCR2.bit.SCES)
#define	LIN_UART2_ECCR2			(IO_LIN_UART2.IO_ECCR2.byte)
#define	LIN_UART2_ECCR2_LBR    	(IO_LIN_UART2.IO_ECCR2.bit.LBR)
#define	LIN_UART2_ECCR2_MS    	(IO_LIN_UART2.IO_ECCR2.bit.MS)
#define	LIN_UART2_ECCR2_SCDE    	(IO_LIN_UART2.IO_ECCR2.bit.SCDE)
#define	LIN_UART2_ECCR2_SSM    	(IO_LIN_UART2.IO_ECCR2.bit.SSM)
#define	LIN_UART2_ECCR2_RBI    	(IO_LIN_UART2.IO_ECCR2.bit.RBI)
#define	LIN_UART2_ECCR2_TBI    	(IO_LIN_UART2.IO_ECCR2.bit.TBI)
#define	LIN_UART2_BGR2	(IO_LIN_UART2.IO_BGR2.hword)
#define	LIN_UART2_BGR2_BGR012	(IO_LIN_UART2.IO_BGR2.byte.BGR012)
#define	LIN_UART2_BGR2_BGR002	(IO_LIN_UART2.IO_BGR2.byte.BGR002)
#define	LIN_UART2_BGR2_BGR	(IO_LIN_UART2.IO_BGR2.bit.BGR)

#ifdef __IO_DEFINE
#pragma segment IO=IO_LIN_UART3, locate=0xD8
#endif

__IO_EXTERN	struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	PEN:1;
			__BYTE	P:1;
			__BYTE	SBL:1;
			__BYTE	CL:1;
			__BYTE	AD:1;
			__BYTE	CRE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} IO_SCR3;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:2;
			__BYTE	OTO:1;
			__BYTE	EXT:1;
			__BYTE	REST:1;
			__BYTE	UPCL:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} IO_SMR3;
	union {
		__BYTE	byte;
		struct {
			__BYTE	PE:1;
			__BYTE	ORE:1;
			__BYTE	FRE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	BDS:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
		} bit;
	} IO_SSR3;
	__BYTE	IO_RDR3;
	union {
		__BYTE	byte;
		struct {
			__BYTE	LBIE:1;
			__BYTE	LBD:1;
			__BYTE	LBL:2;
			__BYTE	SOPE:1;
			__BYTE	SIOP:1;
			__BYTE	CCO:1;
			__BYTE	SCES:1;
		} bit;
	} IO_ESCR3;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV50:1;
			__BYTE	LBR:1;
			__BYTE	MS:1;
			__BYTE	SCDE:1;
			__BYTE	SSM:1;
			__BYTE	RESV55:1;
			__BYTE	RBI:1;
			__BYTE	TBI:1;
		} bit;
	} IO_ECCR3;
	union {
		__HWORD	hword;
		struct {
			__BYTE	BGR013;
			__BYTE	BGR003;
		} byte;
		struct {
			__HWORD	RESV0:1;
			__HWORD	BGR:15;
		} bit;
	} IO_BGR3;
} IO_LIN_UART3;

#define	LIN_UART3_SCR3			(IO_LIN_UART3.IO_SCR3.byte)
#define	LIN_UART3_SCR3_PEN    	(IO_LIN_UART3.IO_SCR3.bit.PEN)
#define	LIN_UART3_SCR3_P    	(IO_LIN_UART3.IO_SCR3.bit.P)
#define	LIN_UART3_SCR3_SBL    	(IO_LIN_UART3.IO_SCR3.bit.SBL)
#define	LIN_UART3_SCR3_CL    	(IO_LIN_UART3.IO_SCR3.bit.CL)
#define	LIN_UART3_SCR3_AD    	(IO_LIN_UART3.IO_SCR3.bit.AD)
#define	LIN_UART3_SCR3_CRE    	(IO_LIN_UART3.IO_SCR3.bit.CRE)
#define	LIN_UART3_SCR3_RXE    	(IO_LIN_UART3.IO_SCR3.bit.RXE)
#define	LIN_UART3_SCR3_TXE    	(IO_LIN_UART3.IO_SCR3.bit.TXE)
#define	LIN_UART3_SMR3			(IO_LIN_UART3.IO_SMR3.byte)
#define	LIN_UART3_SMR3_MD    	(IO_LIN_UART3.IO_SMR3.bit.MD)
#define	LIN_UART3_SMR3_OTO    	(IO_LIN_UART3.IO_SMR3.bit.OTO)
#define	LIN_UART3_SMR3_EXT    	(IO_LIN_UART3.IO_SMR3.bit.EXT)
#define	LIN_UART3_SMR3_REST    	(IO_LIN_UART3.IO_SMR3.bit.REST)
#define	LIN_UART3_SMR3_UPCL    	(IO_LIN_UART3.IO_SMR3.bit.UPCL)
#define	LIN_UART3_SMR3_SCKE    	(IO_LIN_UART3.IO_SMR3.bit.SCKE)
#define	LIN_UART3_SMR3_SOE    	(IO_LIN_UART3.IO_SMR3.bit.SOE)
#define	LIN_UART3_SSR3			(IO_LIN_UART3.IO_SSR3.byte)
#define	LIN_UART3_SSR3_PE    	(IO_LIN_UART3.IO_SSR3.bit.PE)
#define	LIN_UART3_SSR3_ORE    	(IO_LIN_UART3.IO_SSR3.bit.ORE)
#define	LIN_UART3_SSR3_FRE    	(IO_LIN_UART3.IO_SSR3.bit.FRE)
#define	LIN_UART3_SSR3_RDRF    	(IO_LIN_UART3.IO_SSR3.bit.RDRF)
#define	LIN_UART3_SSR3_TDRE    	(IO_LIN_UART3.IO_SSR3.bit.TDRE)
#define	LIN_UART3_SSR3_BDS    	(IO_LIN_UART3.IO_SSR3.bit.BDS)
#define	LIN_UART3_SSR3_RIE    	(IO_LIN_UART3.IO_SSR3.bit.RIE)
#define	LIN_UART3_SSR3_TIE    	(IO_LIN_UART3.IO_SSR3.bit.TIE)
#define	LIN_UART3_RDR3	(IO_LIN_UART3.IO_RDR3)
#define	LIN_UART3_TDR3	(IO_LIN_UART3.IO_RDR3)
#define	LIN_UART3_ESCR3			(IO_LIN_UART3.IO_ESCR3.byte)
#define	LIN_UART3_ESCR3_LBIE    	(IO_LIN_UART3.IO_ESCR3.bit.LBIE)
#define	LIN_UART3_ESCR3_LBD    	(IO_LIN_UART3.IO_ESCR3.bit.LBD)
#define	LIN_UART3_ESCR3_LBL    	(IO_LIN_UART3.IO_ESCR3.bit.LBL)
#define	LIN_UART3_ESCR3_SOPE    	(IO_LIN_UART3.IO_ESCR3.bit.SOPE)
#define	LIN_UART3_ESCR3_SIOP    	(IO_LIN_UART3.IO_ESCR3.bit.SIOP)
#define	LIN_UART3_ESCR3_CCO    	(IO_LIN_UART3.IO_ESCR3.bit.CCO)
#define	LIN_UART3_ESCR3_SCES    	(IO_LIN_UART3.IO_ESCR3.bit.SCES)
#define	LIN_UART3_ECCR3			(IO_LIN_UART3.IO_ECCR3.byte)
#define	LIN_UART3_ECCR3_LBR    	(IO_LIN_UART3.IO_ECCR3.bit.LBR)
#define	LIN_UART3_ECCR3_MS    	(IO_LIN_UART3.IO_ECCR3.bit.MS)
#define	LIN_UART3_ECCR3_SCDE    	(IO_LIN_UART3.IO_ECCR3.bit.SCDE)
#define	LIN_UART3_ECCR3_SSM    	(IO_LIN_UART3.IO_ECCR3.bit.SSM)
#define	LIN_UART3_ECCR3_RBI    	(IO_LIN_UART3.IO_ECCR3.bit.RBI)
#define	LIN_UART3_ECCR3_TBI    	(IO_LIN_UART3.IO_ECCR3.bit.TBI)
#define	LIN_UART3_BGR3	(IO_LIN_UART3.IO_BGR3.hword)
#define	LIN_UART3_BGR3_BGR013	(IO_LIN_UART3.IO_BGR3.byte.BGR013)
#define	LIN_UART3_BGR3_BGR003	(IO_LIN_UART3.IO_BGR3.byte.BGR003)
#define	LIN_UART3_BGR3_BGR	(IO_LIN_UART3.IO_BGR3.bit.BGR)

#ifdef __IO_DEFINE
#pragma segment IO=IO_LIN_UART4, locate=0xE0
#endif

__IO_EXTERN	struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	PEN:1;
			__BYTE	P:1;
			__BYTE	SBL:1;
			__BYTE	CL:1;
			__BYTE	AD:1;
			__BYTE	CRE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} IO_SCR4;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:2;
			__BYTE	OTO:1;
			__BYTE	EXT:1;
			__BYTE	REST:1;
			__BYTE	UPCL:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} IO_SMR4;
	union {
		__BYTE	byte;
		struct {
			__BYTE	PE:1;
			__BYTE	ORE:1;
			__BYTE	FRE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	BDS:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
		} bit;
	} IO_SSR4;
	__BYTE	IO_RDR4;
	union {
		__BYTE	byte;
		struct {
			__BYTE	LBIE:1;
			__BYTE	LBD:1;
			__BYTE	LBL:2;
			__BYTE	SOPE:1;
			__BYTE	SIOP:1;
			__BYTE	CCO:1;
			__BYTE	SCES:1;
		} bit;
	} IO_ESCR4;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV50:1;
			__BYTE	LBR:1;
			__BYTE	MS:1;
			__BYTE	SCDE:1;
			__BYTE	SSM:1;
			__BYTE	RESV55:1;
			__BYTE	RBI:1;
			__BYTE	TBI:1;
		} bit;
	} IO_ECCR4;
	union {
		__HWORD	hword;
		struct {
			__BYTE	BGR014;
			__BYTE	BGR004;
		} byte;
		struct {
			__HWORD	RESV0:1;
			__HWORD	BGR:15;
		} bit;
	} IO_BGR4;
} IO_LIN_UART4;

#define	LIN_UART4_SCR4			(IO_LIN_UART4.IO_SCR4.byte)
#define	LIN_UART4_SCR4_PEN    	(IO_LIN_UART4.IO_SCR4.bit.PEN)
#define	LIN_UART4_SCR4_P    	(IO_LIN_UART4.IO_SCR4.bit.P)
#define	LIN_UART4_SCR4_SBL    	(IO_LIN_UART4.IO_SCR4.bit.SBL)
#define	LIN_UART4_SCR4_CL    	(IO_LIN_UART4.IO_SCR4.bit.CL)
#define	LIN_UART4_SCR4_AD    	(IO_LIN_UART4.IO_SCR4.bit.AD)
#define	LIN_UART4_SCR4_CRE    	(IO_LIN_UART4.IO_SCR4.bit.CRE)
#define	LIN_UART4_SCR4_RXE    	(IO_LIN_UART4.IO_SCR4.bit.RXE)
#define	LIN_UART4_SCR4_TXE    	(IO_LIN_UART4.IO_SCR4.bit.TXE)
#define	LIN_UART4_SMR4			(IO_LIN_UART4.IO_SMR4.byte)
#define	LIN_UART4_SMR4_MD    	(IO_LIN_UART4.IO_SMR4.bit.MD)
#define	LIN_UART4_SMR4_OTO    	(IO_LIN_UART4.IO_SMR4.bit.OTO)
#define	LIN_UART4_SMR4_EXT    	(IO_LIN_UART4.IO_SMR4.bit.EXT)
#define	LIN_UART4_SMR4_REST    	(IO_LIN_UART4.IO_SMR4.bit.REST)
#define	LIN_UART4_SMR4_UPCL    	(IO_LIN_UART4.IO_SMR4.bit.UPCL)
#define	LIN_UART4_SMR4_SCKE    	(IO_LIN_UART4.IO_SMR4.bit.SCKE)
#define	LIN_UART4_SMR4_SOE    	(IO_LIN_UART4.IO_SMR4.bit.SOE)
#define	LIN_UART4_SSR4			(IO_LIN_UART4.IO_SSR4.byte)
#define	LIN_UART4_SSR4_PE    	(IO_LIN_UART4.IO_SSR4.bit.PE)
#define	LIN_UART4_SSR4_ORE    	(IO_LIN_UART4.IO_SSR4.bit.ORE)
#define	LIN_UART4_SSR4_FRE    	(IO_LIN_UART4.IO_SSR4.bit.FRE)
#define	LIN_UART4_SSR4_RDRF    	(IO_LIN_UART4.IO_SSR4.bit.RDRF)
#define	LIN_UART4_SSR4_TDRE    	(IO_LIN_UART4.IO_SSR4.bit.TDRE)
#define	LIN_UART4_SSR4_BDS    	(IO_LIN_UART4.IO_SSR4.bit.BDS)
#define	LIN_UART4_SSR4_RIE    	(IO_LIN_UART4.IO_SSR4.bit.RIE)
#define	LIN_UART4_SSR4_TIE    	(IO_LIN_UART4.IO_SSR4.bit.TIE)
#define	LIN_UART4_RDR4	(IO_LIN_UART4.IO_RDR4)
#define	LIN_UART4_TDR4	(IO_LIN_UART4.IO_RDR4)
#define	LIN_UART4_ESCR4			(IO_LIN_UART4.IO_ESCR4.byte)
#define	LIN_UART4_ESCR4_LBIE    	(IO_LIN_UART4.IO_ESCR4.bit.LBIE)
#define	LIN_UART4_ESCR4_LBD    	(IO_LIN_UART4.IO_ESCR4.bit.LBD)
#define	LIN_UART4_ESCR4_LBL    	(IO_LIN_UART4.IO_ESCR4.bit.LBL)
#define	LIN_UART4_ESCR4_SOPE    	(IO_LIN_UART4.IO_ESCR4.bit.SOPE)
#define	LIN_UART4_ESCR4_SIOP    	(IO_LIN_UART4.IO_ESCR4.bit.SIOP)
#define	LIN_UART4_ESCR4_CCO    	(IO_LIN_UART4.IO_ESCR4.bit.CCO)
#define	LIN_UART4_ESCR4_SCES    	(IO_LIN_UART4.IO_ESCR4.bit.SCES)
#define	LIN_UART4_ECCR4			(IO_LIN_UART4.IO_ECCR4.byte)
#define	LIN_UART4_ECCR4_LBR    	(IO_LIN_UART4.IO_ECCR4.bit.LBR)
#define	LIN_UART4_ECCR4_MS    	(IO_LIN_UART4.IO_ECCR4.bit.MS)
#define	LIN_UART4_ECCR4_SCDE    	(IO_LIN_UART4.IO_ECCR4.bit.SCDE)
#define	LIN_UART4_ECCR4_SSM    	(IO_LIN_UART4.IO_ECCR4.bit.SSM)
#define	LIN_UART4_ECCR4_RBI    	(IO_LIN_UART4.IO_ECCR4.bit.RBI)
#define	LIN_UART4_ECCR4_TBI    	(IO_LIN_UART4.IO_ECCR4.bit.TBI)
#define	LIN_UART4_BGR4	(IO_LIN_UART4.IO_BGR4.hword)
#define	LIN_UART4_BGR4_BGR014	(IO_LIN_UART4.IO_BGR4.byte.BGR014)
#define	LIN_UART4_BGR4_BGR004	(IO_LIN_UART4.IO_BGR4.byte.BGR004)
#define	LIN_UART4_BGR4_BGR	(IO_LIN_UART4.IO_BGR4.bit.BGR)

#ifdef __IO_DEFINE
#pragma segment IO=IO_LIN_UART5, locate=0xE8
#endif

__IO_EXTERN	struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	PEN:1;
			__BYTE	P:1;
			__BYTE	SBL:1;
			__BYTE	CL:1;
			__BYTE	AD:1;
			__BYTE	CRE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} IO_SCR5;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:2;
			__BYTE	OTO:1;
			__BYTE	EXT:1;
			__BYTE	REST:1;
			__BYTE	UPCL:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} IO_SMR5;
	union {
		__BYTE	byte;
		struct {
			__BYTE	PE:1;
			__BYTE	ORE:1;
			__BYTE	FRE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	BDS:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
		} bit;
	} IO_SSR5;
	__BYTE	IO_RDR5;
	union {
		__BYTE	byte;
		struct {
			__BYTE	LBIE:1;
			__BYTE	LBD:1;
			__BYTE	LBL:2;
			__BYTE	SOPE:1;
			__BYTE	SIOP:1;
			__BYTE	CCO:1;
			__BYTE	SCES:1;
		} bit;
	} IO_ESCR5;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV50:1;
			__BYTE	LBR:1;
			__BYTE	MS:1;
			__BYTE	SCDE:1;
			__BYTE	SSM:1;
			__BYTE	RESV55:1;
			__BYTE	RBI:1;
			__BYTE	TBI:1;
		} bit;
	} IO_ECCR5;
	union {
		__HWORD	hword;
		struct {
			__BYTE	BGR015;
			__BYTE	BGR005;
		} byte;
		struct {
			__HWORD	RESV0:1;
			__HWORD	BGR:15;
		} bit;
	} IO_BGR5;
} IO_LIN_UART5;

#define	LIN_UART5_SCR5			(IO_LIN_UART5.IO_SCR5.byte)
#define	LIN_UART5_SCR5_PEN    	(IO_LIN_UART5.IO_SCR5.bit.PEN)
#define	LIN_UART5_SCR5_P    	(IO_LIN_UART5.IO_SCR5.bit.P)
#define	LIN_UART5_SCR5_SBL    	(IO_LIN_UART5.IO_SCR5.bit.SBL)
#define	LIN_UART5_SCR5_CL    	(IO_LIN_UART5.IO_SCR5.bit.CL)
#define	LIN_UART5_SCR5_AD    	(IO_LIN_UART5.IO_SCR5.bit.AD)
#define	LIN_UART5_SCR5_CRE    	(IO_LIN_UART5.IO_SCR5.bit.CRE)
#define	LIN_UART5_SCR5_RXE    	(IO_LIN_UART5.IO_SCR5.bit.RXE)
#define	LIN_UART5_SCR5_TXE    	(IO_LIN_UART5.IO_SCR5.bit.TXE)
#define	LIN_UART5_SMR5			(IO_LIN_UART5.IO_SMR5.byte)
#define	LIN_UART5_SMR5_MD    	(IO_LIN_UART5.IO_SMR5.bit.MD)
#define	LIN_UART5_SMR5_OTO    	(IO_LIN_UART5.IO_SMR5.bit.OTO)
#define	LIN_UART5_SMR5_EXT    	(IO_LIN_UART5.IO_SMR5.bit.EXT)
#define	LIN_UART5_SMR5_REST    	(IO_LIN_UART5.IO_SMR5.bit.REST)
#define	LIN_UART5_SMR5_UPCL    	(IO_LIN_UART5.IO_SMR5.bit.UPCL)
#define	LIN_UART5_SMR5_SCKE    	(IO_LIN_UART5.IO_SMR5.bit.SCKE)
#define	LIN_UART5_SMR5_SOE    	(IO_LIN_UART5.IO_SMR5.bit.SOE)
#define	LIN_UART5_SSR5			(IO_LIN_UART5.IO_SSR5.byte)
#define	LIN_UART5_SSR5_PE    	(IO_LIN_UART5.IO_SSR5.bit.PE)
#define	LIN_UART5_SSR5_ORE    	(IO_LIN_UART5.IO_SSR5.bit.ORE)
#define	LIN_UART5_SSR5_FRE    	(IO_LIN_UART5.IO_SSR5.bit.FRE)
#define	LIN_UART5_SSR5_RDRF    	(IO_LIN_UART5.IO_SSR5.bit.RDRF)
#define	LIN_UART5_SSR5_TDRE    	(IO_LIN_UART5.IO_SSR5.bit.TDRE)
#define	LIN_UART5_SSR5_BDS    	(IO_LIN_UART5.IO_SSR5.bit.BDS)
#define	LIN_UART5_SSR5_RIE    	(IO_LIN_UART5.IO_SSR5.bit.RIE)
#define	LIN_UART5_SSR5_TIE    	(IO_LIN_UART5.IO_SSR5.bit.TIE)
#define	LIN_UART5_RDR5	(IO_LIN_UART5.IO_RDR5)
#define	LIN_UART5_TDR5	(IO_LIN_UART5.IO_RDR5)
#define	LIN_UART5_ESCR5			(IO_LIN_UART5.IO_ESCR5.byte)
#define	LIN_UART5_ESCR5_LBIE    	(IO_LIN_UART5.IO_ESCR5.bit.LBIE)
#define	LIN_UART5_ESCR5_LBD    	(IO_LIN_UART5.IO_ESCR5.bit.LBD)
#define	LIN_UART5_ESCR5_LBL    	(IO_LIN_UART5.IO_ESCR5.bit.LBL)
#define	LIN_UART5_ESCR5_SOPE    	(IO_LIN_UART5.IO_ESCR5.bit.SOPE)
#define	LIN_UART5_ESCR5_SIOP    	(IO_LIN_UART5.IO_ESCR5.bit.SIOP)
#define	LIN_UART5_ESCR5_CCO    	(IO_LIN_UART5.IO_ESCR5.bit.CCO)
#define	LIN_UART5_ESCR5_SCES    	(IO_LIN_UART5.IO_ESCR5.bit.SCES)
#define	LIN_UART5_ECCR5			(IO_LIN_UART5.IO_ECCR5.byte)
#define	LIN_UART5_ECCR5_LBR    	(IO_LIN_UART5.IO_ECCR5.bit.LBR)
#define	LIN_UART5_ECCR5_MS    	(IO_LIN_UART5.IO_ECCR5.bit.MS)
#define	LIN_UART5_ECCR5_SCDE    	(IO_LIN_UART5.IO_ECCR5.bit.SCDE)
#define	LIN_UART5_ECCR5_SSM    	(IO_LIN_UART5.IO_ECCR5.bit.SSM)
#define	LIN_UART5_ECCR5_RBI    	(IO_LIN_UART5.IO_ECCR5.bit.RBI)
#define	LIN_UART5_ECCR5_TBI    	(IO_LIN_UART5.IO_ECCR5.bit.TBI)
#define	LIN_UART5_BGR5	(IO_LIN_UART5.IO_BGR5.hword)
#define	LIN_UART5_BGR5_BGR015	(IO_LIN_UART5.IO_BGR5.byte.BGR015)
#define	LIN_UART5_BGR5_BGR005	(IO_LIN_UART5.IO_BGR5.byte.BGR005)
#define	LIN_UART5_BGR5_BGR	(IO_LIN_UART5.IO_BGR5.bit.BGR)

#ifdef __IO_DEFINE
#pragma segment IO=IO_LIN_UART6, locate=0xF0
#endif

__IO_EXTERN	struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	PEN:1;
			__BYTE	P:1;
			__BYTE	SBL:1;
			__BYTE	CL:1;
			__BYTE	AD:1;
			__BYTE	CRE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} IO_SCR6;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:2;
			__BYTE	OTO:1;
			__BYTE	EXT:1;
			__BYTE	REST:1;
			__BYTE	UPCL:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} IO_SMR6;
	union {
		__BYTE	byte;
		struct {
			__BYTE	PE:1;
			__BYTE	ORE:1;
			__BYTE	FRE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	BDS:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
		} bit;
	} IO_SSR6;
	__BYTE	IO_RDR6;
	union {
		__BYTE	byte;
		struct {
			__BYTE	LBIE:1;
			__BYTE	LBD:1;
			__BYTE	LBL:2;
			__BYTE	SOPE:1;
			__BYTE	SIOP:1;
			__BYTE	CCO:1;
			__BYTE	SCES:1;
		} bit;
	} IO_ESCR6;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV50:1;
			__BYTE	LBR:1;
			__BYTE	MS:1;
			__BYTE	SCDE:1;
			__BYTE	SSM:1;
			__BYTE	RESV55:1;
			__BYTE	RBI:1;
			__BYTE	TBI:1;
		} bit;
	} IO_ECCR6;
	union {
		__HWORD	hword;
		struct {
			__BYTE	BGR016;
			__BYTE	BGR006;
		} byte;
		struct {
			__HWORD	RESV0:1;
			__HWORD	BGR:15;
		} bit;
	} IO_BGR6;
} IO_LIN_UART6;

#define	LIN_UART6_SCR6			(IO_LIN_UART6.IO_SCR6.byte)
#define	LIN_UART6_SCR6_PEN    	(IO_LIN_UART6.IO_SCR6.bit.PEN)
#define	LIN_UART6_SCR6_P    	(IO_LIN_UART6.IO_SCR6.bit.P)
#define	LIN_UART6_SCR6_SBL    	(IO_LIN_UART6.IO_SCR6.bit.SBL)
#define	LIN_UART6_SCR6_CL    	(IO_LIN_UART6.IO_SCR6.bit.CL)
#define	LIN_UART6_SCR6_AD    	(IO_LIN_UART6.IO_SCR6.bit.AD)
#define	LIN_UART6_SCR6_CRE    	(IO_LIN_UART6.IO_SCR6.bit.CRE)
#define	LIN_UART6_SCR6_RXE    	(IO_LIN_UART6.IO_SCR6.bit.RXE)
#define	LIN_UART6_SCR6_TXE    	(IO_LIN_UART6.IO_SCR6.bit.TXE)
#define	LIN_UART6_SMR6			(IO_LIN_UART6.IO_SMR6.byte)
#define	LIN_UART6_SMR6_MD    	(IO_LIN_UART6.IO_SMR6.bit.MD)
#define	LIN_UART6_SMR6_OTO    	(IO_LIN_UART6.IO_SMR6.bit.OTO)
#define	LIN_UART6_SMR6_EXT    	(IO_LIN_UART6.IO_SMR6.bit.EXT)
#define	LIN_UART6_SMR6_REST    	(IO_LIN_UART6.IO_SMR6.bit.REST)
#define	LIN_UART6_SMR6_UPCL    	(IO_LIN_UART6.IO_SMR6.bit.UPCL)
#define	LIN_UART6_SMR6_SCKE    	(IO_LIN_UART6.IO_SMR6.bit.SCKE)
#define	LIN_UART6_SMR6_SOE    	(IO_LIN_UART6.IO_SMR6.bit.SOE)
#define	LIN_UART6_SSR6			(IO_LIN_UART6.IO_SSR6.byte)
#define	LIN_UART6_SSR6_PE    	(IO_LIN_UART6.IO_SSR6.bit.PE)
#define	LIN_UART6_SSR6_ORE    	(IO_LIN_UART6.IO_SSR6.bit.ORE)
#define	LIN_UART6_SSR6_FRE    	(IO_LIN_UART6.IO_SSR6.bit.FRE)
#define	LIN_UART6_SSR6_RDRF    	(IO_LIN_UART6.IO_SSR6.bit.RDRF)
#define	LIN_UART6_SSR6_TDRE    	(IO_LIN_UART6.IO_SSR6.bit.TDRE)
#define	LIN_UART6_SSR6_BDS    	(IO_LIN_UART6.IO_SSR6.bit.BDS)
#define	LIN_UART6_SSR6_RIE    	(IO_LIN_UART6.IO_SSR6.bit.RIE)
#define	LIN_UART6_SSR6_TIE    	(IO_LIN_UART6.IO_SSR6.bit.TIE)
#define	LIN_UART6_RDR6	(IO_LIN_UART6.IO_RDR6)
#define	LIN_UART6_TDR6	(IO_LIN_UART6.IO_RDR6)
#define	LIN_UART6_ESCR6			(IO_LIN_UART6.IO_ESCR6.byte)
#define	LIN_UART6_ESCR6_LBIE    	(IO_LIN_UART6.IO_ESCR6.bit.LBIE)
#define	LIN_UART6_ESCR6_LBD    	(IO_LIN_UART6.IO_ESCR6.bit.LBD)
#define	LIN_UART6_ESCR6_LBL    	(IO_LIN_UART6.IO_ESCR6.bit.LBL)
#define	LIN_UART6_ESCR6_SOPE    	(IO_LIN_UART6.IO_ESCR6.bit.SOPE)
#define	LIN_UART6_ESCR6_SIOP    	(IO_LIN_UART6.IO_ESCR6.bit.SIOP)
#define	LIN_UART6_ESCR6_CCO    	(IO_LIN_UART6.IO_ESCR6.bit.CCO)
#define	LIN_UART6_ESCR6_SCES    	(IO_LIN_UART6.IO_ESCR6.bit.SCES)
#define	LIN_UART6_ECCR6			(IO_LIN_UART6.IO_ECCR6.byte)
#define	LIN_UART6_ECCR6_LBR    	(IO_LIN_UART6.IO_ECCR6.bit.LBR)
#define	LIN_UART6_ECCR6_MS    	(IO_LIN_UART6.IO_ECCR6.bit.MS)
#define	LIN_UART6_ECCR6_SCDE    	(IO_LIN_UART6.IO_ECCR6.bit.SCDE)
#define	LIN_UART6_ECCR6_SSM    	(IO_LIN_UART6.IO_ECCR6.bit.SSM)
#define	LIN_UART6_ECCR6_RBI    	(IO_LIN_UART6.IO_ECCR6.bit.RBI)
#define	LIN_UART6_ECCR6_TBI    	(IO_LIN_UART6.IO_ECCR6.bit.TBI)
#define	LIN_UART6_BGR6	(IO_LIN_UART6.IO_BGR6.hword)
#define	LIN_UART6_BGR6_BGR016	(IO_LIN_UART6.IO_BGR6.byte.BGR016)
#define	LIN_UART6_BGR6_BGR006	(IO_LIN_UART6.IO_BGR6.byte.BGR006)
#define	LIN_UART6_BGR6_BGR	(IO_LIN_UART6.IO_BGR6.bit.BGR)

#ifdef __IO_DEFINE
#pragma segment IO=IO_LIN_UART7, locate=0xF8
#endif

__IO_EXTERN	struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	PEN:1;
			__BYTE	P:1;
			__BYTE	SBL:1;
			__BYTE	CL:1;
			__BYTE	AD:1;
			__BYTE	CRE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} IO_SCR7;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:2;
			__BYTE	OTO:1;
			__BYTE	EXT:1;
			__BYTE	REST:1;
			__BYTE	UPCL:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} IO_SMR7;
	union {
		__BYTE	byte;
		struct {
			__BYTE	PE:1;
			__BYTE	ORE:1;
			__BYTE	FRE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	BDS:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
		} bit;
	} IO_SSR7;
	__BYTE	IO_RDR7;
	union {
		__BYTE	byte;
		struct {
			__BYTE	LBIE:1;
			__BYTE	LBD:1;
			__BYTE	LBL:2;
			__BYTE	SOPE:1;
			__BYTE	SIOP:1;
			__BYTE	CCO:1;
			__BYTE	SCES:1;
		} bit;
	} IO_ESCR7;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV50:1;
			__BYTE	LBR:1;
			__BYTE	MS:1;
			__BYTE	SCDE:1;
			__BYTE	SSM:1;
			__BYTE	RESV55:1;
			__BYTE	RBI:1;
			__BYTE	TBI:1;
		} bit;
	} IO_ECCR7;
	union {
		__HWORD	hword;
		struct {
			__BYTE	BGR017;
			__BYTE	BGR007;
		} byte;
		struct {
			__HWORD	RESV0:1;
			__HWORD	BGR:15;
		} bit;
	} IO_BGR7;
} IO_LIN_UART7;

#define	LIN_UART7_SCR7			(IO_LIN_UART7.IO_SCR7.byte)
#define	LIN_UART7_SCR7_PEN    	(IO_LIN_UART7.IO_SCR7.bit.PEN)
#define	LIN_UART7_SCR7_P    	(IO_LIN_UART7.IO_SCR7.bit.P)
#define	LIN_UART7_SCR7_SBL    	(IO_LIN_UART7.IO_SCR7.bit.SBL)
#define	LIN_UART7_SCR7_CL    	(IO_LIN_UART7.IO_SCR7.bit.CL)
#define	LIN_UART7_SCR7_AD    	(IO_LIN_UART7.IO_SCR7.bit.AD)
#define	LIN_UART7_SCR7_CRE    	(IO_LIN_UART7.IO_SCR7.bit.CRE)
#define	LIN_UART7_SCR7_RXE    	(IO_LIN_UART7.IO_SCR7.bit.RXE)
#define	LIN_UART7_SCR7_TXE    	(IO_LIN_UART7.IO_SCR7.bit.TXE)
#define	LIN_UART7_SMR7			(IO_LIN_UART7.IO_SMR7.byte)
#define	LIN_UART7_SMR7_MD    	(IO_LIN_UART7.IO_SMR7.bit.MD)
#define	LIN_UART7_SMR7_OTO    	(IO_LIN_UART7.IO_SMR7.bit.OTO)
#define	LIN_UART7_SMR7_EXT    	(IO_LIN_UART7.IO_SMR7.bit.EXT)
#define	LIN_UART7_SMR7_REST    	(IO_LIN_UART7.IO_SMR7.bit.REST)
#define	LIN_UART7_SMR7_UPCL    	(IO_LIN_UART7.IO_SMR7.bit.UPCL)
#define	LIN_UART7_SMR7_SCKE    	(IO_LIN_UART7.IO_SMR7.bit.SCKE)
#define	LIN_UART7_SMR7_SOE    	(IO_LIN_UART7.IO_SMR7.bit.SOE)
#define	LIN_UART7_SSR7			(IO_LIN_UART7.IO_SSR7.byte)
#define	LIN_UART7_SSR7_PE    	(IO_LIN_UART7.IO_SSR7.bit.PE)
#define	LIN_UART7_SSR7_ORE    	(IO_LIN_UART7.IO_SSR7.bit.ORE)
#define	LIN_UART7_SSR7_FRE    	(IO_LIN_UART7.IO_SSR7.bit.FRE)
#define	LIN_UART7_SSR7_RDRF    	(IO_LIN_UART7.IO_SSR7.bit.RDRF)
#define	LIN_UART7_SSR7_TDRE    	(IO_LIN_UART7.IO_SSR7.bit.TDRE)
#define	LIN_UART7_SSR7_BDS    	(IO_LIN_UART7.IO_SSR7.bit.BDS)
#define	LIN_UART7_SSR7_RIE    	(IO_LIN_UART7.IO_SSR7.bit.RIE)
#define	LIN_UART7_SSR7_TIE    	(IO_LIN_UART7.IO_SSR7.bit.TIE)
#define	LIN_UART7_RDR7	(IO_LIN_UART7.IO_RDR7)
#define	LIN_UART7_TDR7	(IO_LIN_UART7.IO_RDR7)
#define	LIN_UART7_ESCR7			(IO_LIN_UART7.IO_ESCR7.byte)
#define	LIN_UART7_ESCR7_LBIE    	(IO_LIN_UART7.IO_ESCR7.bit.LBIE)
#define	LIN_UART7_ESCR7_LBD    	(IO_LIN_UART7.IO_ESCR7.bit.LBD)
#define	LIN_UART7_ESCR7_LBL    	(IO_LIN_UART7.IO_ESCR7.bit.LBL)
#define	LIN_UART7_ESCR7_SOPE    	(IO_LIN_UART7.IO_ESCR7.bit.SOPE)
#define	LIN_UART7_ESCR7_SIOP    	(IO_LIN_UART7.IO_ESCR7.bit.SIOP)
#define	LIN_UART7_ESCR7_CCO    	(IO_LIN_UART7.IO_ESCR7.bit.CCO)
#define	LIN_UART7_ESCR7_SCES    	(IO_LIN_UART7.IO_ESCR7.bit.SCES)
#define	LIN_UART7_ECCR7			(IO_LIN_UART7.IO_ECCR7.byte)
#define	LIN_UART7_ECCR7_LBR    	(IO_LIN_UART7.IO_ECCR7.bit.LBR)
#define	LIN_UART7_ECCR7_MS    	(IO_LIN_UART7.IO_ECCR7.bit.MS)
#define	LIN_UART7_ECCR7_SCDE    	(IO_LIN_UART7.IO_ECCR7.bit.SCDE)
#define	LIN_UART7_ECCR7_SSM    	(IO_LIN_UART7.IO_ECCR7.bit.SSM)
#define	LIN_UART7_ECCR7_RBI    	(IO_LIN_UART7.IO_ECCR7.bit.RBI)
#define	LIN_UART7_ECCR7_TBI    	(IO_LIN_UART7.IO_ECCR7.bit.TBI)
#define	LIN_UART7_BGR7	(IO_LIN_UART7.IO_BGR7.hword)
#define	LIN_UART7_BGR7_BGR017	(IO_LIN_UART7.IO_BGR7.byte.BGR017)
#define	LIN_UART7_BGR7_BGR007	(IO_LIN_UART7.IO_BGR7.byte.BGR007)
#define	LIN_UART7_BGR7_BGR	(IO_LIN_UART7.IO_BGR7.bit.BGR)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_TMRLRA1,attr=IO,locate=0x100
#endif

__IO_EXTENDED	__HWORD	IO_TMRLRA1;
#define	_tmrlra1		(IO_TMRLRA1)
#define	TMRLRA1	(_tmrlra1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_TMR1,attr=IO,locate=0x102
#endif

__IO_EXTENDED	const __HWORD	IO_TMR1;
#define	_tmr1		(IO_TMR1)
#define	TMR1_	(_tmr1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_TMRLRB1,attr=IO,locate=0x104
#endif

__IO_EXTENDED	__HWORD	IO_TMRLRB1;
#define	_tmrlrb1		(IO_TMRLRB1)
#define	TMRLRB1	(_tmrlrb1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_TMCSR1,attr=IO,locate=0x106
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	TMCSR1H;
		__BYTE	TMCSR1L;
	} byte;
	struct {
			__HWORD	MOD:2;
			__HWORD	TRGM:2;
			__HWORD	CSL:3;
			__HWORD	GATE:1;
			__HWORD	EF:1;
			__HWORD	RESV5:1;
			__HWORD	OUTL:1;
			__HWORD	RELD:1;
			__HWORD	INTE:1;
			__HWORD	UF:1;
			__HWORD	CNTE:1;
			__HWORD	TRG:1;
	} bit;
} TMCSR1STR;

__IO_EXTENDED	  TMCSR1STR	IO_TMCSR1;
#define	_tmcsr1		(IO_TMCSR1)
#define	TMCSR1		(IO_TMCSR1.hword)
#define	TMCSR1_TMCSR1H	(IO_TMCSR1.byte.TMCSR1H)
#define	TMCSR1_TMCSR1L	(IO_TMCSR1.byte.TMCSR1L)
#define	TMCSR1_MOD  	(IO_TMCSR1.bit.MOD)
#define	TMCSR1_TRGM  	(IO_TMCSR1.bit.TRGM)
#define	TMCSR1_CSL  	(IO_TMCSR1.bit.CSL)
#define	TMCSR1_GATE  	(IO_TMCSR1.bit.GATE)
#define	TMCSR1_EF  	(IO_TMCSR1.bit.EF)
#define	TMCSR1_OUTL  	(IO_TMCSR1.bit.OUTL)
#define	TMCSR1_RELD  	(IO_TMCSR1.bit.RELD)
#define	TMCSR1_INTE  	(IO_TMCSR1.bit.INTE)
#define	TMCSR1_UF  	(IO_TMCSR1.bit.UF)
#define	TMCSR1_CNTE  	(IO_TMCSR1.bit.CNTE)
#define	TMCSR1_TRG  	(IO_TMCSR1.bit.TRG)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_TMRLRA2,attr=IO,locate=0x108
#endif

__IO_EXTENDED	__HWORD	IO_TMRLRA2;
#define	_tmrlra2		(IO_TMRLRA2)
#define	TMRLRA2	(_tmrlra2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_TMR2,attr=IO,locate=0x10A
#endif

__IO_EXTENDED	const __HWORD	IO_TMR2;
#define	_tmr2		(IO_TMR2)
#define	TMR2_	(_tmr2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_TMRLRB2,attr=IO,locate=0x10C
#endif

__IO_EXTENDED	__HWORD	IO_TMRLRB2;
#define	_tmrlrb2		(IO_TMRLRB2)
#define	TMRLRB2	(_tmrlrb2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_TMCSR2,attr=IO,locate=0x10E
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	TMCSR2H;
		__BYTE	TMCSR2L;
	} byte;
	struct {
			__HWORD	MOD:2;
			__HWORD	TRGM:2;
			__HWORD	CSL:3;
			__HWORD	GATE:1;
			__HWORD	EF:1;
			__HWORD	RESV5:1;
			__HWORD	OUTL:1;
			__HWORD	RELD:1;
			__HWORD	INTE:1;
			__HWORD	UF:1;
			__HWORD	CNTE:1;
			__HWORD	TRG:1;
	} bit;
} TMCSR2STR;

__IO_EXTENDED	  TMCSR2STR	IO_TMCSR2;
#define	_tmcsr2		(IO_TMCSR2)
#define	TMCSR2		(IO_TMCSR2.hword)
#define	TMCSR2_TMCSR2H	(IO_TMCSR2.byte.TMCSR2H)
#define	TMCSR2_TMCSR2L	(IO_TMCSR2.byte.TMCSR2L)
#define	TMCSR2_MOD  	(IO_TMCSR2.bit.MOD)
#define	TMCSR2_TRGM  	(IO_TMCSR2.bit.TRGM)
#define	TMCSR2_CSL  	(IO_TMCSR2.bit.CSL)
#define	TMCSR2_GATE  	(IO_TMCSR2.bit.GATE)
#define	TMCSR2_EF  	(IO_TMCSR2.bit.EF)
#define	TMCSR2_OUTL  	(IO_TMCSR2.bit.OUTL)
#define	TMCSR2_RELD  	(IO_TMCSR2.bit.RELD)
#define	TMCSR2_INTE  	(IO_TMCSR2.bit.INTE)
#define	TMCSR2_UF  	(IO_TMCSR2.bit.UF)
#define	TMCSR2_CNTE  	(IO_TMCSR2.bit.CNTE)
#define	TMCSR2_TRG  	(IO_TMCSR2.bit.TRG)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_TMRLRA3,attr=IO,locate=0x110
#endif

__IO_EXTENDED	__HWORD	IO_TMRLRA3;
#define	_tmrlra3		(IO_TMRLRA3)
#define	TMRLRA3	(_tmrlra3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_TMR3,attr=IO,locate=0x112
#endif

__IO_EXTENDED	const __HWORD	IO_TMR3;
#define	_tmr3		(IO_TMR3)
#define	TMR3_	(_tmr3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_TMRLRB3,attr=IO,locate=0x114
#endif

__IO_EXTENDED	__HWORD	IO_TMRLRB3;
#define	_tmrlrb3		(IO_TMRLRB3)
#define	TMRLRB3	(_tmrlrb3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_TMCSR3,attr=IO,locate=0x116
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	TMCSR3H;
		__BYTE	TMCSR3L;
	} byte;
	struct {
			__HWORD	MOD:2;
			__HWORD	TRGM:2;
			__HWORD	CSL:3;
			__HWORD	GATE:1;
			__HWORD	EF:1;
			__HWORD	RESV5:1;
			__HWORD	OUTL:1;
			__HWORD	RELD:1;
			__HWORD	INTE:1;
			__HWORD	UF:1;
			__HWORD	CNTE:1;
			__HWORD	TRG:1;
	} bit;
} TMCSR3STR;

__IO_EXTENDED	  TMCSR3STR	IO_TMCSR3;
#define	_tmcsr3		(IO_TMCSR3)
#define	TMCSR3		(IO_TMCSR3.hword)
#define	TMCSR3_TMCSR3H	(IO_TMCSR3.byte.TMCSR3H)
#define	TMCSR3_TMCSR3L	(IO_TMCSR3.byte.TMCSR3L)
#define	TMCSR3_MOD  	(IO_TMCSR3.bit.MOD)
#define	TMCSR3_TRGM  	(IO_TMCSR3.bit.TRGM)
#define	TMCSR3_CSL  	(IO_TMCSR3.bit.CSL)
#define	TMCSR3_GATE  	(IO_TMCSR3.bit.GATE)
#define	TMCSR3_EF  	(IO_TMCSR3.bit.EF)
#define	TMCSR3_OUTL  	(IO_TMCSR3.bit.OUTL)
#define	TMCSR3_RELD  	(IO_TMCSR3.bit.RELD)
#define	TMCSR3_INTE  	(IO_TMCSR3.bit.INTE)
#define	TMCSR3_UF  	(IO_TMCSR3.bit.UF)
#define	TMCSR3_CNTE  	(IO_TMCSR3.bit.CNTE)
#define	TMCSR3_TRG  	(IO_TMCSR3.bit.TRG)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCCP6,attr=IO,locate=0x120
#endif

__IO_EXTENDED	__WORD	IO_OCCP6;
#define	_occp6		(IO_OCCP6)
#define	OCCP6	(_occp6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCCP7,attr=IO,locate=0x124
#endif

__IO_EXTENDED	__WORD	IO_OCCP7;
#define	_occp7		(IO_OCCP7)
#define	OCCP7	(_occp7)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCFS67,attr=IO,locate=0x128
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SEL7:1;
			__BYTE	SEL6:1;
	} bit;
} OCFS67STR;

__IO_EXTENDED	  OCFS67STR	IO_OCFS67;
#define	_ocfs67		(IO_OCFS67)
#define	OCFS67		(IO_OCFS67.byte)
#define	OCFS67_SEL7  	(IO_OCFS67.bit.SEL7)
#define	OCFS67_SEL6  	(IO_OCFS67.bit.SEL6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCS67,attr=IO,locate=0x12A
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	OCSH67;
		__BYTE	OCSL67;
	} byte;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	CMOD:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	OTD7:1;
			__HWORD	OTD6:1;
			__HWORD	IOP7:1;
			__HWORD	IOP6:1;
			__HWORD	IOE7:1;
			__HWORD	IOE6:1;
			__HWORD	RESV12:1;
			__HWORD	RESV13:1;
			__HWORD	CST7:1;
			__HWORD	CST6:1;
	} bit;
} OCS67STR;

__IO_EXTENDED	  OCS67STR	IO_OCS67;
#define	_ocs67		(IO_OCS67)
#define	OCS67		(IO_OCS67.hword)
#define	OCS67_OCSH67	(IO_OCS67.byte.OCSH67)
#define	OCS67_OCSL67	(IO_OCS67.byte.OCSL67)
#define	OCS67_CMOD  	(IO_OCS67.bit.CMOD)
#define	OCS67_OTD7  	(IO_OCS67.bit.OTD7)
#define	OCS67_OTD6  	(IO_OCS67.bit.OTD6)
#define	OCS67_IOP7  	(IO_OCS67.bit.IOP7)
#define	OCS67_IOP6  	(IO_OCS67.bit.IOP6)
#define	OCS67_IOE7  	(IO_OCS67.bit.IOE7)
#define	OCS67_IOE6  	(IO_OCS67.bit.IOE6)
#define	OCS67_CST7  	(IO_OCS67.bit.CST7)
#define	OCS67_CST6  	(IO_OCS67.bit.CST6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCCP8,attr=IO,locate=0x12C
#endif

__IO_EXTENDED	__WORD	IO_OCCP8;
#define	_occp8		(IO_OCCP8)
#define	OCCP8	(_occp8)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCCP9,attr=IO,locate=0x130
#endif

__IO_EXTENDED	__WORD	IO_OCCP9;
#define	_occp9		(IO_OCCP9)
#define	OCCP9	(_occp9)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCFS89,attr=IO,locate=0x134
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SEL9:1;
			__BYTE	SEL8:1;
	} bit;
} OCFS89STR;

__IO_EXTENDED	  OCFS89STR	IO_OCFS89;
#define	_ocfs89		(IO_OCFS89)
#define	OCFS89		(IO_OCFS89.byte)
#define	OCFS89_SEL9  	(IO_OCFS89.bit.SEL9)
#define	OCFS89_SEL8  	(IO_OCFS89.bit.SEL8)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCS89,attr=IO,locate=0x136
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	OCSH89;
		__BYTE	OCSL89;
	} byte;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	CMOD:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	OTD9:1;
			__HWORD	OTD8:1;
			__HWORD	IOP9:1;
			__HWORD	IOP8:1;
			__HWORD	IOE9:1;
			__HWORD	IOE8:1;
			__HWORD	RESV12:1;
			__HWORD	RESV13:1;
			__HWORD	CST9:1;
			__HWORD	CST8:1;
	} bit;
} OCS89STR;

__IO_EXTENDED	  OCS89STR	IO_OCS89;
#define	_ocs89		(IO_OCS89)
#define	OCS89		(IO_OCS89.hword)
#define	OCS89_OCSH89	(IO_OCS89.byte.OCSH89)
#define	OCS89_OCSL89	(IO_OCS89.byte.OCSL89)
#define	OCS89_CMOD  	(IO_OCS89.bit.CMOD)
#define	OCS89_OTD9  	(IO_OCS89.bit.OTD9)
#define	OCS89_OTD8  	(IO_OCS89.bit.OTD8)
#define	OCS89_IOP9  	(IO_OCS89.bit.IOP9)
#define	OCS89_IOP8  	(IO_OCS89.bit.IOP8)
#define	OCS89_IOE9  	(IO_OCS89.bit.IOE9)
#define	OCS89_IOE8  	(IO_OCS89.bit.IOE8)
#define	OCS89_CST9  	(IO_OCS89.bit.CST9)
#define	OCS89_CST8  	(IO_OCS89.bit.CST8)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCCP10,attr=IO,locate=0x138
#endif

__IO_EXTENDED	__WORD	IO_OCCP10;
#define	_occp10		(IO_OCCP10)
#define	OCCP10	(_occp10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCCP11,attr=IO,locate=0x13C
#endif

__IO_EXTENDED	__WORD	IO_OCCP11;
#define	_occp11		(IO_OCCP11)
#define	OCCP11	(_occp11)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCFS1011,attr=IO,locate=0x140
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SEL11:1;
			__BYTE	SEL10:1;
	} bit;
} OCFS1011STR;

__IO_EXTENDED	  OCFS1011STR	IO_OCFS1011;
#define	_ocfs1011		(IO_OCFS1011)
#define	OCFS1011		(IO_OCFS1011.byte)
#define	OCFS1011_SEL11  	(IO_OCFS1011.bit.SEL11)
#define	OCFS1011_SEL10  	(IO_OCFS1011.bit.SEL10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCS1011,attr=IO,locate=0x142
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	OCSH1011;
		__BYTE	OCSL1011;
	} byte;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	CMOD:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	OTD11:1;
			__HWORD	OTD10:1;
			__HWORD	IOP11:1;
			__HWORD	IOP10:1;
			__HWORD	IOE11:1;
			__HWORD	IOE10:1;
			__HWORD	RESV12:1;
			__HWORD	RESV13:1;
			__HWORD	CST11:1;
			__HWORD	CST10:1;
	} bit;
} OCS1011STR;

__IO_EXTENDED	  OCS1011STR	IO_OCS1011;
#define	_ocs1011		(IO_OCS1011)
#define	OCS1011		(IO_OCS1011.hword)
#define	OCS1011_OCSH1011	(IO_OCS1011.byte.OCSH1011)
#define	OCS1011_OCSL1011	(IO_OCS1011.byte.OCSL1011)
#define	OCS1011_CMOD  	(IO_OCS1011.bit.CMOD)
#define	OCS1011_OTD11  	(IO_OCS1011.bit.OTD11)
#define	OCS1011_OTD10  	(IO_OCS1011.bit.OTD10)
#define	OCS1011_IOP11  	(IO_OCS1011.bit.IOP11)
#define	OCS1011_IOP10  	(IO_OCS1011.bit.IOP10)
#define	OCS1011_IOE11  	(IO_OCS1011.bit.IOE11)
#define	OCS1011_IOE10  	(IO_OCS1011.bit.IOE10)
#define	OCS1011_CST11  	(IO_OCS1011.bit.CST11)
#define	OCS1011_CST10  	(IO_OCS1011.bit.CST10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_GCN13,attr=IO,locate=0x144
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	TSEL3:4;
			__HWORD	TSEL2:4;
			__HWORD	TSEL1:4;
			__HWORD	TSEL0:4;
	} bit;
} GCN13STR;

__IO_EXTENDED	  GCN13STR	IO_GCN13;
#define	_gcn13		(IO_GCN13)
#define	GCN13		(IO_GCN13.hword)
#define	GCN13_TSEL3  	(IO_GCN13.bit.TSEL3)
#define	GCN13_TSEL2  	(IO_GCN13.bit.TSEL2)
#define	GCN13_TSEL1  	(IO_GCN13.bit.TSEL1)
#define	GCN13_TSEL0  	(IO_GCN13.bit.TSEL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_GCN23,attr=IO,locate=0x147
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	EN3:1;
			__BYTE	EN2:1;
			__BYTE	EN1:1;
			__BYTE	EN0:1;
	} bit;
} GCN23STR;

__IO_EXTENDED	  GCN23STR	IO_GCN23;
#define	_gcn23		(IO_GCN23)
#define	GCN23		(IO_GCN23.byte)
#define	GCN23_EN3  	(IO_GCN23.bit.EN3)
#define	GCN23_EN2  	(IO_GCN23.bit.EN2)
#define	GCN23_EN1  	(IO_GCN23.bit.EN1)
#define	GCN23_EN0  	(IO_GCN23.bit.EN0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_GCN14,attr=IO,locate=0x148
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	TSEL3:4;
			__HWORD	TSEL2:4;
			__HWORD	TSEL1:4;
			__HWORD	TSEL0:4;
	} bit;
} GCN14STR;

__IO_EXTENDED	  GCN14STR	IO_GCN14;
#define	_gcn14		(IO_GCN14)
#define	GCN14		(IO_GCN14.hword)
#define	GCN14_TSEL3  	(IO_GCN14.bit.TSEL3)
#define	GCN14_TSEL2  	(IO_GCN14.bit.TSEL2)
#define	GCN14_TSEL1  	(IO_GCN14.bit.TSEL1)
#define	GCN14_TSEL0  	(IO_GCN14.bit.TSEL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_GCN24,attr=IO,locate=0x14B
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	EN3:1;
			__BYTE	EN2:1;
			__BYTE	EN1:1;
			__BYTE	EN0:1;
	} bit;
} GCN24STR;

__IO_EXTENDED	  GCN24STR	IO_GCN24;
#define	_gcn24		(IO_GCN24)
#define	GCN24		(IO_GCN24.byte)
#define	GCN24_EN3  	(IO_GCN24.bit.EN3)
#define	GCN24_EN2  	(IO_GCN24.bit.EN2)
#define	GCN24_EN1  	(IO_GCN24.bit.EN1)
#define	GCN24_EN0  	(IO_GCN24.bit.EN0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_GCN15,attr=IO,locate=0x14C
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	TSEL3:4;
			__HWORD	TSEL2:4;
			__HWORD	TSEL1:4;
			__HWORD	TSEL0:4;
	} bit;
} GCN15STR;

__IO_EXTENDED	  GCN15STR	IO_GCN15;
#define	_gcn15		(IO_GCN15)
#define	GCN15		(IO_GCN15.hword)
#define	GCN15_TSEL3  	(IO_GCN15.bit.TSEL3)
#define	GCN15_TSEL2  	(IO_GCN15.bit.TSEL2)
#define	GCN15_TSEL1  	(IO_GCN15.bit.TSEL1)
#define	GCN15_TSEL0  	(IO_GCN15.bit.TSEL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_GCN25,attr=IO,locate=0x14F
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	EN3:1;
			__BYTE	EN2:1;
			__BYTE	EN1:1;
			__BYTE	EN0:1;
	} bit;
} GCN25STR;

__IO_EXTENDED	  GCN25STR	IO_GCN25;
#define	_gcn25		(IO_GCN25)
#define	GCN25		(IO_GCN25.byte)
#define	GCN25_EN3  	(IO_GCN25.bit.EN3)
#define	GCN25_EN2  	(IO_GCN25.bit.EN2)
#define	GCN25_EN1  	(IO_GCN25.bit.EN1)
#define	GCN25_EN0  	(IO_GCN25.bit.EN0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR11,attr=IO,locate=0x150
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR11;
#define	_ptmr11		(IO_PTMR11)
#define	PTMR11	(_ptmr11)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR11,attr=IO,locate=0x152
#endif

__IO_EXTENDED	__HWORD	IO_PCSR11;
#define	_pcsr11		(IO_PCSR11)
#define	PCSR11	(_pcsr11)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT11,attr=IO,locate=0x154
#endif

__IO_EXTENDED	__HWORD	IO_PDUT11;
#define	_pdut11		(IO_PDUT11)
#define	PDUT11	(_pdut11)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN11,attr=IO,locate=0x156
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN11STR;

__IO_EXTENDED	  PCN11STR	IO_PCN11;
#define	_pcn11		(IO_PCN11)
#define	PCN11		(IO_PCN11.hword)
#define	PCN11_CNTE  	(IO_PCN11.bit.CNTE)
#define	PCN11_STGR  	(IO_PCN11.bit.STGR)
#define	PCN11_MDSE  	(IO_PCN11.bit.MDSE)
#define	PCN11_RTRG  	(IO_PCN11.bit.RTRG)
#define	PCN11_CKS  	(IO_PCN11.bit.CKS)
#define	PCN11_PGMS  	(IO_PCN11.bit.PGMS)
#define	PCN11_EGS  	(IO_PCN11.bit.EGS)
#define	PCN11_IREN  	(IO_PCN11.bit.IREN)
#define	PCN11_IRQF  	(IO_PCN11.bit.IRQF)
#define	PCN11_IRS  	(IO_PCN11.bit.IRS)
#define	PCN11_OSEL  	(IO_PCN11.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR12,attr=IO,locate=0x158
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR12;
#define	_ptmr12		(IO_PTMR12)
#define	PTMR12	(_ptmr12)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR12,attr=IO,locate=0x15A
#endif

__IO_EXTENDED	__HWORD	IO_PCSR12;
#define	_pcsr12		(IO_PCSR12)
#define	PCSR12	(_pcsr12)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT12,attr=IO,locate=0x15C
#endif

__IO_EXTENDED	__HWORD	IO_PDUT12;
#define	_pdut12		(IO_PDUT12)
#define	PDUT12	(_pdut12)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN12,attr=IO,locate=0x15E
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN12STR;

__IO_EXTENDED	  PCN12STR	IO_PCN12;
#define	_pcn12		(IO_PCN12)
#define	PCN12		(IO_PCN12.hword)
#define	PCN12_CNTE  	(IO_PCN12.bit.CNTE)
#define	PCN12_STGR  	(IO_PCN12.bit.STGR)
#define	PCN12_MDSE  	(IO_PCN12.bit.MDSE)
#define	PCN12_RTRG  	(IO_PCN12.bit.RTRG)
#define	PCN12_CKS  	(IO_PCN12.bit.CKS)
#define	PCN12_PGMS  	(IO_PCN12.bit.PGMS)
#define	PCN12_EGS  	(IO_PCN12.bit.EGS)
#define	PCN12_IREN  	(IO_PCN12.bit.IREN)
#define	PCN12_IRQF  	(IO_PCN12.bit.IRQF)
#define	PCN12_IRS  	(IO_PCN12.bit.IRS)
#define	PCN12_OSEL  	(IO_PCN12.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR13,attr=IO,locate=0x160
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR13;
#define	_ptmr13		(IO_PTMR13)
#define	PTMR13	(_ptmr13)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR13,attr=IO,locate=0x162
#endif

__IO_EXTENDED	__HWORD	IO_PCSR13;
#define	_pcsr13		(IO_PCSR13)
#define	PCSR13	(_pcsr13)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT13,attr=IO,locate=0x164
#endif

__IO_EXTENDED	__HWORD	IO_PDUT13;
#define	_pdut13		(IO_PDUT13)
#define	PDUT13	(_pdut13)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN13,attr=IO,locate=0x166
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN13STR;

__IO_EXTENDED	  PCN13STR	IO_PCN13;
#define	_pcn13		(IO_PCN13)
#define	PCN13		(IO_PCN13.hword)
#define	PCN13_CNTE  	(IO_PCN13.bit.CNTE)
#define	PCN13_STGR  	(IO_PCN13.bit.STGR)
#define	PCN13_MDSE  	(IO_PCN13.bit.MDSE)
#define	PCN13_RTRG  	(IO_PCN13.bit.RTRG)
#define	PCN13_CKS  	(IO_PCN13.bit.CKS)
#define	PCN13_PGMS  	(IO_PCN13.bit.PGMS)
#define	PCN13_EGS  	(IO_PCN13.bit.EGS)
#define	PCN13_IREN  	(IO_PCN13.bit.IREN)
#define	PCN13_IRQF  	(IO_PCN13.bit.IRQF)
#define	PCN13_IRS  	(IO_PCN13.bit.IRS)
#define	PCN13_OSEL  	(IO_PCN13.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR14,attr=IO,locate=0x168
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR14;
#define	_ptmr14		(IO_PTMR14)
#define	PTMR14	(_ptmr14)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR14,attr=IO,locate=0x16A
#endif

__IO_EXTENDED	__HWORD	IO_PCSR14;
#define	_pcsr14		(IO_PCSR14)
#define	PCSR14	(_pcsr14)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT14,attr=IO,locate=0x16C
#endif

__IO_EXTENDED	__HWORD	IO_PDUT14;
#define	_pdut14		(IO_PDUT14)
#define	PDUT14	(_pdut14)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN14,attr=IO,locate=0x16E
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN14STR;

__IO_EXTENDED	  PCN14STR	IO_PCN14;
#define	_pcn14		(IO_PCN14)
#define	PCN14		(IO_PCN14.hword)
#define	PCN14_CNTE  	(IO_PCN14.bit.CNTE)
#define	PCN14_STGR  	(IO_PCN14.bit.STGR)
#define	PCN14_MDSE  	(IO_PCN14.bit.MDSE)
#define	PCN14_RTRG  	(IO_PCN14.bit.RTRG)
#define	PCN14_CKS  	(IO_PCN14.bit.CKS)
#define	PCN14_PGMS  	(IO_PCN14.bit.PGMS)
#define	PCN14_EGS  	(IO_PCN14.bit.EGS)
#define	PCN14_IREN  	(IO_PCN14.bit.IREN)
#define	PCN14_IRQF  	(IO_PCN14.bit.IRQF)
#define	PCN14_IRS  	(IO_PCN14.bit.IRS)
#define	PCN14_OSEL  	(IO_PCN14.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR15,attr=IO,locate=0x170
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR15;
#define	_ptmr15		(IO_PTMR15)
#define	PTMR15	(_ptmr15)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR15,attr=IO,locate=0x172
#endif

__IO_EXTENDED	__HWORD	IO_PCSR15;
#define	_pcsr15		(IO_PCSR15)
#define	PCSR15	(_pcsr15)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT15,attr=IO,locate=0x174
#endif

__IO_EXTENDED	__HWORD	IO_PDUT15;
#define	_pdut15		(IO_PDUT15)
#define	PDUT15	(_pdut15)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN15,attr=IO,locate=0x176
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN15STR;

__IO_EXTENDED	  PCN15STR	IO_PCN15;
#define	_pcn15		(IO_PCN15)
#define	PCN15		(IO_PCN15.hword)
#define	PCN15_CNTE  	(IO_PCN15.bit.CNTE)
#define	PCN15_STGR  	(IO_PCN15.bit.STGR)
#define	PCN15_MDSE  	(IO_PCN15.bit.MDSE)
#define	PCN15_RTRG  	(IO_PCN15.bit.RTRG)
#define	PCN15_CKS  	(IO_PCN15.bit.CKS)
#define	PCN15_PGMS  	(IO_PCN15.bit.PGMS)
#define	PCN15_EGS  	(IO_PCN15.bit.EGS)
#define	PCN15_IREN  	(IO_PCN15.bit.IREN)
#define	PCN15_IRQF  	(IO_PCN15.bit.IRQF)
#define	PCN15_IRS  	(IO_PCN15.bit.IRS)
#define	PCN15_OSEL  	(IO_PCN15.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR16,attr=IO,locate=0x178
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR16;
#define	_ptmr16		(IO_PTMR16)
#define	PTMR16	(_ptmr16)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR16,attr=IO,locate=0x17A
#endif

__IO_EXTENDED	__HWORD	IO_PCSR16;
#define	_pcsr16		(IO_PCSR16)
#define	PCSR16	(_pcsr16)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT16,attr=IO,locate=0x17C
#endif

__IO_EXTENDED	__HWORD	IO_PDUT16;
#define	_pdut16		(IO_PDUT16)
#define	PDUT16	(_pdut16)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN16,attr=IO,locate=0x17E
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN16STR;

__IO_EXTENDED	  PCN16STR	IO_PCN16;
#define	_pcn16		(IO_PCN16)
#define	PCN16		(IO_PCN16.hword)
#define	PCN16_CNTE  	(IO_PCN16.bit.CNTE)
#define	PCN16_STGR  	(IO_PCN16.bit.STGR)
#define	PCN16_MDSE  	(IO_PCN16.bit.MDSE)
#define	PCN16_RTRG  	(IO_PCN16.bit.RTRG)
#define	PCN16_CKS  	(IO_PCN16.bit.CKS)
#define	PCN16_PGMS  	(IO_PCN16.bit.PGMS)
#define	PCN16_EGS  	(IO_PCN16.bit.EGS)
#define	PCN16_IREN  	(IO_PCN16.bit.IREN)
#define	PCN16_IRQF  	(IO_PCN16.bit.IRQF)
#define	PCN16_IRS  	(IO_PCN16.bit.IRS)
#define	PCN16_OSEL  	(IO_PCN16.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR17,attr=IO,locate=0x180
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR17;
#define	_ptmr17		(IO_PTMR17)
#define	PTMR17	(_ptmr17)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR17,attr=IO,locate=0x182
#endif

__IO_EXTENDED	__HWORD	IO_PCSR17;
#define	_pcsr17		(IO_PCSR17)
#define	PCSR17	(_pcsr17)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT17,attr=IO,locate=0x184
#endif

__IO_EXTENDED	__HWORD	IO_PDUT17;
#define	_pdut17		(IO_PDUT17)
#define	PDUT17	(_pdut17)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN17,attr=IO,locate=0x186
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN17STR;

__IO_EXTENDED	  PCN17STR	IO_PCN17;
#define	_pcn17		(IO_PCN17)
#define	PCN17		(IO_PCN17.hword)
#define	PCN17_CNTE  	(IO_PCN17.bit.CNTE)
#define	PCN17_STGR  	(IO_PCN17.bit.STGR)
#define	PCN17_MDSE  	(IO_PCN17.bit.MDSE)
#define	PCN17_RTRG  	(IO_PCN17.bit.RTRG)
#define	PCN17_CKS  	(IO_PCN17.bit.CKS)
#define	PCN17_PGMS  	(IO_PCN17.bit.PGMS)
#define	PCN17_EGS  	(IO_PCN17.bit.EGS)
#define	PCN17_IREN  	(IO_PCN17.bit.IREN)
#define	PCN17_IRQF  	(IO_PCN17.bit.IRQF)
#define	PCN17_IRS  	(IO_PCN17.bit.IRS)
#define	PCN17_OSEL  	(IO_PCN17.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR18,attr=IO,locate=0x188
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR18;
#define	_ptmr18		(IO_PTMR18)
#define	PTMR18	(_ptmr18)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR18,attr=IO,locate=0x18A
#endif

__IO_EXTENDED	__HWORD	IO_PCSR18;
#define	_pcsr18		(IO_PCSR18)
#define	PCSR18	(_pcsr18)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT18,attr=IO,locate=0x18C
#endif

__IO_EXTENDED	__HWORD	IO_PDUT18;
#define	_pdut18		(IO_PDUT18)
#define	PDUT18	(_pdut18)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN18,attr=IO,locate=0x18E
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN18STR;

__IO_EXTENDED	  PCN18STR	IO_PCN18;
#define	_pcn18		(IO_PCN18)
#define	PCN18		(IO_PCN18.hword)
#define	PCN18_CNTE  	(IO_PCN18.bit.CNTE)
#define	PCN18_STGR  	(IO_PCN18.bit.STGR)
#define	PCN18_MDSE  	(IO_PCN18.bit.MDSE)
#define	PCN18_RTRG  	(IO_PCN18.bit.RTRG)
#define	PCN18_CKS  	(IO_PCN18.bit.CKS)
#define	PCN18_PGMS  	(IO_PCN18.bit.PGMS)
#define	PCN18_EGS  	(IO_PCN18.bit.EGS)
#define	PCN18_IREN  	(IO_PCN18.bit.IREN)
#define	PCN18_IRQF  	(IO_PCN18.bit.IRQF)
#define	PCN18_IRS  	(IO_PCN18.bit.IRS)
#define	PCN18_OSEL  	(IO_PCN18.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR19,attr=IO,locate=0x190
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR19;
#define	_ptmr19		(IO_PTMR19)
#define	PTMR19	(_ptmr19)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR19,attr=IO,locate=0x192
#endif

__IO_EXTENDED	__HWORD	IO_PCSR19;
#define	_pcsr19		(IO_PCSR19)
#define	PCSR19	(_pcsr19)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT19,attr=IO,locate=0x194
#endif

__IO_EXTENDED	__HWORD	IO_PDUT19;
#define	_pdut19		(IO_PDUT19)
#define	PDUT19	(_pdut19)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN19,attr=IO,locate=0x196
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN19STR;

__IO_EXTENDED	  PCN19STR	IO_PCN19;
#define	_pcn19		(IO_PCN19)
#define	PCN19		(IO_PCN19.hword)
#define	PCN19_CNTE  	(IO_PCN19.bit.CNTE)
#define	PCN19_STGR  	(IO_PCN19.bit.STGR)
#define	PCN19_MDSE  	(IO_PCN19.bit.MDSE)
#define	PCN19_RTRG  	(IO_PCN19.bit.RTRG)
#define	PCN19_CKS  	(IO_PCN19.bit.CKS)
#define	PCN19_PGMS  	(IO_PCN19.bit.PGMS)
#define	PCN19_EGS  	(IO_PCN19.bit.EGS)
#define	PCN19_IREN  	(IO_PCN19.bit.IREN)
#define	PCN19_IRQF  	(IO_PCN19.bit.IRQF)
#define	PCN19_IRS  	(IO_PCN19.bit.IRS)
#define	PCN19_OSEL  	(IO_PCN19.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR20,attr=IO,locate=0x198
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR20;
#define	_ptmr20		(IO_PTMR20)
#define	PTMR20	(_ptmr20)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR20,attr=IO,locate=0x19A
#endif

__IO_EXTENDED	__HWORD	IO_PCSR20;
#define	_pcsr20		(IO_PCSR20)
#define	PCSR20	(_pcsr20)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT20,attr=IO,locate=0x19C
#endif

__IO_EXTENDED	__HWORD	IO_PDUT20;
#define	_pdut20		(IO_PDUT20)
#define	PDUT20	(_pdut20)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN20,attr=IO,locate=0x19E
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN20STR;

__IO_EXTENDED	  PCN20STR	IO_PCN20;
#define	_pcn20		(IO_PCN20)
#define	PCN20		(IO_PCN20.hword)
#define	PCN20_CNTE  	(IO_PCN20.bit.CNTE)
#define	PCN20_STGR  	(IO_PCN20.bit.STGR)
#define	PCN20_MDSE  	(IO_PCN20.bit.MDSE)
#define	PCN20_RTRG  	(IO_PCN20.bit.RTRG)
#define	PCN20_CKS  	(IO_PCN20.bit.CKS)
#define	PCN20_PGMS  	(IO_PCN20.bit.PGMS)
#define	PCN20_EGS  	(IO_PCN20.bit.EGS)
#define	PCN20_IREN  	(IO_PCN20.bit.IREN)
#define	PCN20_IRQF  	(IO_PCN20.bit.IRQF)
#define	PCN20_IRS  	(IO_PCN20.bit.IRS)
#define	PCN20_OSEL  	(IO_PCN20.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR21,attr=IO,locate=0x1A0
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR21;
#define	_ptmr21		(IO_PTMR21)
#define	PTMR21	(_ptmr21)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR21,attr=IO,locate=0x1A2
#endif

__IO_EXTENDED	__HWORD	IO_PCSR21;
#define	_pcsr21		(IO_PCSR21)
#define	PCSR21	(_pcsr21)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT21,attr=IO,locate=0x1A4
#endif

__IO_EXTENDED	__HWORD	IO_PDUT21;
#define	_pdut21		(IO_PDUT21)
#define	PDUT21	(_pdut21)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN21,attr=IO,locate=0x1A6
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN21STR;

__IO_EXTENDED	  PCN21STR	IO_PCN21;
#define	_pcn21		(IO_PCN21)
#define	PCN21		(IO_PCN21.hword)
#define	PCN21_CNTE  	(IO_PCN21.bit.CNTE)
#define	PCN21_STGR  	(IO_PCN21.bit.STGR)
#define	PCN21_MDSE  	(IO_PCN21.bit.MDSE)
#define	PCN21_RTRG  	(IO_PCN21.bit.RTRG)
#define	PCN21_CKS  	(IO_PCN21.bit.CKS)
#define	PCN21_PGMS  	(IO_PCN21.bit.PGMS)
#define	PCN21_EGS  	(IO_PCN21.bit.EGS)
#define	PCN21_IREN  	(IO_PCN21.bit.IREN)
#define	PCN21_IRQF  	(IO_PCN21.bit.IRQF)
#define	PCN21_IRS  	(IO_PCN21.bit.IRS)
#define	PCN21_OSEL  	(IO_PCN21.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR22,attr=IO,locate=0x1A8
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR22;
#define	_ptmr22		(IO_PTMR22)
#define	PTMR22	(_ptmr22)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR22,attr=IO,locate=0x1AA
#endif

__IO_EXTENDED	__HWORD	IO_PCSR22;
#define	_pcsr22		(IO_PCSR22)
#define	PCSR22	(_pcsr22)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT22,attr=IO,locate=0x1AC
#endif

__IO_EXTENDED	__HWORD	IO_PDUT22;
#define	_pdut22		(IO_PDUT22)
#define	PDUT22	(_pdut22)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN22,attr=IO,locate=0x1AE
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN22STR;

__IO_EXTENDED	  PCN22STR	IO_PCN22;
#define	_pcn22		(IO_PCN22)
#define	PCN22		(IO_PCN22.hword)
#define	PCN22_CNTE  	(IO_PCN22.bit.CNTE)
#define	PCN22_STGR  	(IO_PCN22.bit.STGR)
#define	PCN22_MDSE  	(IO_PCN22.bit.MDSE)
#define	PCN22_RTRG  	(IO_PCN22.bit.RTRG)
#define	PCN22_CKS  	(IO_PCN22.bit.CKS)
#define	PCN22_PGMS  	(IO_PCN22.bit.PGMS)
#define	PCN22_EGS  	(IO_PCN22.bit.EGS)
#define	PCN22_IREN  	(IO_PCN22.bit.IREN)
#define	PCN22_IRQF  	(IO_PCN22.bit.IRQF)
#define	PCN22_IRS  	(IO_PCN22.bit.IRS)
#define	PCN22_OSEL  	(IO_PCN22.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR23,attr=IO,locate=0x1B0
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR23;
#define	_ptmr23		(IO_PTMR23)
#define	PTMR23	(_ptmr23)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR23,attr=IO,locate=0x1B2
#endif

__IO_EXTENDED	__HWORD	IO_PCSR23;
#define	_pcsr23		(IO_PCSR23)
#define	PCSR23	(_pcsr23)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT23,attr=IO,locate=0x1B4
#endif

__IO_EXTENDED	__HWORD	IO_PDUT23;
#define	_pdut23		(IO_PDUT23)
#define	PDUT23	(_pdut23)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN23,attr=IO,locate=0x1B6
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN23STR;

__IO_EXTENDED	  PCN23STR	IO_PCN23;
#define	_pcn23		(IO_PCN23)
#define	PCN23		(IO_PCN23.hword)
#define	PCN23_CNTE  	(IO_PCN23.bit.CNTE)
#define	PCN23_STGR  	(IO_PCN23.bit.STGR)
#define	PCN23_MDSE  	(IO_PCN23.bit.MDSE)
#define	PCN23_RTRG  	(IO_PCN23.bit.RTRG)
#define	PCN23_CKS  	(IO_PCN23.bit.CKS)
#define	PCN23_PGMS  	(IO_PCN23.bit.PGMS)
#define	PCN23_EGS  	(IO_PCN23.bit.EGS)
#define	PCN23_IREN  	(IO_PCN23.bit.IREN)
#define	PCN23_IRQF  	(IO_PCN23.bit.IRQF)
#define	PCN23_IRS  	(IO_PCN23.bit.IRS)
#define	PCN23_OSEL  	(IO_PCN23.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC20,attr=IO,locate=0x200
#endif

__IO_EXTENDED	__HWORD	IO_PWC20;
#define	_pwc20		(IO_PWC20)
#define	PWC20	(_pwc20)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC10,attr=IO,locate=0x202
#endif

__IO_EXTENDED	__HWORD	IO_PWC10;
#define	_pwc10		(IO_PWC10)
#define	PWC10	(_pwc10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC0,attr=IO,locate=0x205
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	P:3;
			__BYTE	CE:1;
			__BYTE	SC:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
	} bit;
} PWC0STR;

__IO_EXTENDED	  PWC0STR	IO_PWC0;
#define	_pwc0		(IO_PWC0)
#define	PWC0_		(IO_PWC0.byte)
#define	PWC0_P  	(IO_PWC0.bit.P)
#define	PWC0_CE  	(IO_PWC0.bit.CE)
#define	PWC0_SC  	(IO_PWC0.bit.SC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWS20,attr=IO,locate=0x206
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	BS:1;
			__BYTE	P:3;
			__BYTE	M:3;
	} bit;
} PWS20STR;

__IO_EXTENDED	  PWS20STR	IO_PWS20;
#define	_pws20		(IO_PWS20)
#define	PWS20		(IO_PWS20.byte)
#define	PWS20_BS  	(IO_PWS20.bit.BS)
#define	PWS20_P  	(IO_PWS20.bit.P)
#define	PWS20_M  	(IO_PWS20.bit.M)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWS10,attr=IO,locate=0x207
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	P:3;
			__BYTE	M:3;
	} bit;
} PWS10STR;

__IO_EXTENDED	  PWS10STR	IO_PWS10;
#define	_pws10		(IO_PWS10)
#define	PWS10		(IO_PWS10.byte)
#define	PWS10_P  	(IO_PWS10.bit.P)
#define	PWS10_M  	(IO_PWS10.bit.M)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC21,attr=IO,locate=0x208
#endif

__IO_EXTENDED	__HWORD	IO_PWC21;
#define	_pwc21		(IO_PWC21)
#define	PWC21	(_pwc21)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC11,attr=IO,locate=0x20A
#endif

__IO_EXTENDED	__HWORD	IO_PWC11;
#define	_pwc11		(IO_PWC11)
#define	PWC11	(_pwc11)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC1,attr=IO,locate=0x20D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	P:3;
			__BYTE	CE:1;
			__BYTE	SC:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
	} bit;
} PWC1STR;

__IO_EXTENDED	  PWC1STR	IO_PWC1;
#define	_pwc1		(IO_PWC1)
#define	PWC1_		(IO_PWC1.byte)
#define	PWC1_P  	(IO_PWC1.bit.P)
#define	PWC1_CE  	(IO_PWC1.bit.CE)
#define	PWC1_SC  	(IO_PWC1.bit.SC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWS21,attr=IO,locate=0x20E
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	BS:1;
			__BYTE	P:3;
			__BYTE	M:3;
	} bit;
} PWS21STR;

__IO_EXTENDED	  PWS21STR	IO_PWS21;
#define	_pws21		(IO_PWS21)
#define	PWS21		(IO_PWS21.byte)
#define	PWS21_BS  	(IO_PWS21.bit.BS)
#define	PWS21_P  	(IO_PWS21.bit.P)
#define	PWS21_M  	(IO_PWS21.bit.M)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWS11,attr=IO,locate=0x20F
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	P:3;
			__BYTE	M:3;
	} bit;
} PWS11STR;

__IO_EXTENDED	  PWS11STR	IO_PWS11;
#define	_pws11		(IO_PWS11)
#define	PWS11		(IO_PWS11.byte)
#define	PWS11_P  	(IO_PWS11.bit.P)
#define	PWS11_M  	(IO_PWS11.bit.M)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC22,attr=IO,locate=0x210
#endif

__IO_EXTENDED	__HWORD	IO_PWC22;
#define	_pwc22		(IO_PWC22)
#define	PWC22	(_pwc22)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC12,attr=IO,locate=0x212
#endif

__IO_EXTENDED	__HWORD	IO_PWC12;
#define	_pwc12		(IO_PWC12)
#define	PWC12	(_pwc12)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC2,attr=IO,locate=0x215
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	P:3;
			__BYTE	CE:1;
			__BYTE	SC:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
	} bit;
} PWC2STR;

__IO_EXTENDED	  PWC2STR	IO_PWC2;
#define	_pwc2		(IO_PWC2)
#define	PWC2		(IO_PWC2.byte)
#define	PWC2_P  	(IO_PWC2.bit.P)
#define	PWC2_CE  	(IO_PWC2.bit.CE)
#define	PWC2_SC  	(IO_PWC2.bit.SC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWS22,attr=IO,locate=0x216
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	BS:1;
			__BYTE	P:3;
			__BYTE	M:3;
	} bit;
} PWS22STR;

__IO_EXTENDED	  PWS22STR	IO_PWS22;
#define	_pws22		(IO_PWS22)
#define	PWS22		(IO_PWS22.byte)
#define	PWS22_BS  	(IO_PWS22.bit.BS)
#define	PWS22_P  	(IO_PWS22.bit.P)
#define	PWS22_M  	(IO_PWS22.bit.M)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWS12,attr=IO,locate=0x217
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	P:3;
			__BYTE	M:3;
	} bit;
} PWS12STR;

__IO_EXTENDED	  PWS12STR	IO_PWS12;
#define	_pws12		(IO_PWS12)
#define	PWS12		(IO_PWS12.byte)
#define	PWS12_P  	(IO_PWS12.bit.P)
#define	PWS12_M  	(IO_PWS12.bit.M)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC23,attr=IO,locate=0x218
#endif

__IO_EXTENDED	__HWORD	IO_PWC23;
#define	_pwc23		(IO_PWC23)
#define	PWC23	(_pwc23)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC13,attr=IO,locate=0x21A
#endif

__IO_EXTENDED	__HWORD	IO_PWC13;
#define	_pwc13		(IO_PWC13)
#define	PWC13	(_pwc13)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC3,attr=IO,locate=0x21D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	P:3;
			__BYTE	CE:1;
			__BYTE	SC:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
	} bit;
} PWC3STR;

__IO_EXTENDED	  PWC3STR	IO_PWC3;
#define	_pwc3		(IO_PWC3)
#define	PWC3		(IO_PWC3.byte)
#define	PWC3_P  	(IO_PWC3.bit.P)
#define	PWC3_CE  	(IO_PWC3.bit.CE)
#define	PWC3_SC  	(IO_PWC3.bit.SC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWS23,attr=IO,locate=0x21E
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	BS:1;
			__BYTE	P:3;
			__BYTE	M:3;
	} bit;
} PWS23STR;

__IO_EXTENDED	  PWS23STR	IO_PWS23;
#define	_pws23		(IO_PWS23)
#define	PWS23		(IO_PWS23.byte)
#define	PWS23_BS  	(IO_PWS23.bit.BS)
#define	PWS23_P  	(IO_PWS23.bit.P)
#define	PWS23_M  	(IO_PWS23.bit.M)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWS13,attr=IO,locate=0x21F
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	P:3;
			__BYTE	M:3;
	} bit;
} PWS13STR;

__IO_EXTENDED	  PWS13STR	IO_PWS13;
#define	_pws13		(IO_PWS13)
#define	PWS13		(IO_PWS13.byte)
#define	PWS13_P  	(IO_PWS13.bit.P)
#define	PWS13_M  	(IO_PWS13.bit.M)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC24,attr=IO,locate=0x220
#endif

__IO_EXTENDED	__HWORD	IO_PWC24;
#define	_pwc24		(IO_PWC24)
#define	PWC24	(_pwc24)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC14,attr=IO,locate=0x222
#endif

__IO_EXTENDED	__HWORD	IO_PWC14;
#define	_pwc14		(IO_PWC14)
#define	PWC14	(_pwc14)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC4,attr=IO,locate=0x225
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	P:3;
			__BYTE	CE:1;
			__BYTE	SC:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
	} bit;
} PWC4STR;

__IO_EXTENDED	  PWC4STR	IO_PWC4;
#define	_pwc4		(IO_PWC4)
#define	PWC4		(IO_PWC4.byte)
#define	PWC4_P  	(IO_PWC4.bit.P)
#define	PWC4_CE  	(IO_PWC4.bit.CE)
#define	PWC4_SC  	(IO_PWC4.bit.SC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWS24,attr=IO,locate=0x226
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	BS:1;
			__BYTE	P:3;
			__BYTE	M:3;
	} bit;
} PWS24STR;

__IO_EXTENDED	  PWS24STR	IO_PWS24;
#define	_pws24		(IO_PWS24)
#define	PWS24		(IO_PWS24.byte)
#define	PWS24_BS  	(IO_PWS24.bit.BS)
#define	PWS24_P  	(IO_PWS24.bit.P)
#define	PWS24_M  	(IO_PWS24.bit.M)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWS14,attr=IO,locate=0x227
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	P:3;
			__BYTE	M:3;
	} bit;
} PWS14STR;

__IO_EXTENDED	  PWS14STR	IO_PWS14;
#define	_pws14		(IO_PWS14)
#define	PWS14		(IO_PWS14.byte)
#define	PWS14_P  	(IO_PWS14.bit.P)
#define	PWS14_M  	(IO_PWS14.bit.M)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC25,attr=IO,locate=0x228
#endif

__IO_EXTENDED	__HWORD	IO_PWC25;
#define	_pwc25		(IO_PWC25)
#define	PWC25	(_pwc25)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC15,attr=IO,locate=0x22A
#endif

__IO_EXTENDED	__HWORD	IO_PWC15;
#define	_pwc15		(IO_PWC15)
#define	PWC15	(_pwc15)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWC5,attr=IO,locate=0x22D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	P:3;
			__BYTE	CE:1;
			__BYTE	SC:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
	} bit;
} PWC5STR;

__IO_EXTENDED	  PWC5STR	IO_PWC5;
#define	_pwc5		(IO_PWC5)
#define	PWC5		(IO_PWC5.byte)
#define	PWC5_P  	(IO_PWC5.bit.P)
#define	PWC5_CE  	(IO_PWC5.bit.CE)
#define	PWC5_SC  	(IO_PWC5.bit.SC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWS25,attr=IO,locate=0x22E
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	BS:1;
			__BYTE	P:3;
			__BYTE	M:3;
	} bit;
} PWS25STR;

__IO_EXTENDED	  PWS25STR	IO_PWS25;
#define	_pws25		(IO_PWS25)
#define	PWS25		(IO_PWS25.byte)
#define	PWS25_BS  	(IO_PWS25.bit.BS)
#define	PWS25_P  	(IO_PWS25.bit.P)
#define	PWS25_M  	(IO_PWS25.bit.M)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWS15,attr=IO,locate=0x22F
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	P:3;
			__BYTE	M:3;
	} bit;
} PWS15STR;

__IO_EXTENDED	  PWS15STR	IO_PWS15;
#define	_pws15		(IO_PWS15)
#define	PWS15		(IO_PWS15.byte)
#define	PWS15_P  	(IO_PWS15.bit.P)
#define	PWS15_M  	(IO_PWS15.bit.M)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DACR0,attr=IO,locate=0x23C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	DAE:1;
	} bit;
} DACR0STR;

__IO_EXTENDED	  DACR0STR	IO_DACR0;
#define	_dacr0		(IO_DACR0)
#define	DACR0		(IO_DACR0.byte)
#define	DACR0_DAE  	(IO_DACR0.bit.DAE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DADR0,attr=IO,locate=0x23D
#endif

__IO_EXTENDED	__BYTE	IO_DADR0;
#define	_dadr0		(IO_DADR0)
#define	DADR0	(_dadr0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DACR1,attr=IO,locate=0x23E
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	DAE:1;
	} bit;
} DACR1STR;

__IO_EXTENDED	  DACR1STR	IO_DACR1;
#define	_dacr1		(IO_DACR1)
#define	DACR1		(IO_DACR1.byte)
#define	DACR1_DAE  	(IO_DACR1.bit.DAE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DADR1,attr=IO,locate=0x23F
#endif

__IO_EXTENDED	__BYTE	IO_DADR1;
#define	_dadr1		(IO_DADR1)
#define	DADR1	(_dadr1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_FT0,attr=IO,locate=0x240
#endif

__IO_EXTENDED	struct {
	__WORD	IO_CPCLR0;
	__WORD	IO_TCDT0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	TCCSH0;
			__BYTE	TCCSL0;
		} byte;
		struct {
			__HWORD	ECKE:1;
			__HWORD	RESV81:1;
			__HWORD	RESV82:1;
			__HWORD	RESV83:1;
			__HWORD	RESV84:1;
			__HWORD	RESV85:1;
			__HWORD	ICLR:1;
			__HWORD	ICRE:1;
			__HWORD	RESV88:1;
			__HWORD	STOP:1;
			__HWORD	RESV810:1;
			__HWORD	SCLR:1;
			__HWORD	CLK:4;
		} bit;
	} IO_TCCS0;
} IO_FT0;

#define	FT0_CPCLR0	(IO_FT0.IO_CPCLR0)
#define	FT0_TCDT0	(IO_FT0.IO_TCDT0)
#define	FT0_TCCS0			(IO_FT0.IO_TCCS0.hword)
#define	FT0_TCCS0_TCCSH0    	(IO_FT0.IO_TCCS0.byte.TCCSH0)
#define	FT0_TCCS0_TCCSL0    	(IO_FT0.IO_TCCS0.byte.TCCSL0)
#define	FT0_TCCS0_ECKE    	(IO_FT0.IO_TCCS0.bit.ECKE)
#define	FT0_TCCS0_ICLR    	(IO_FT0.IO_TCCS0.bit.ICLR)
#define	FT0_TCCS0_ICRE    	(IO_FT0.IO_TCCS0.bit.ICRE)
#define	FT0_TCCS0_STOP    	(IO_FT0.IO_TCCS0.bit.STOP)
#define	FT0_TCCS0_SCLR    	(IO_FT0.IO_TCCS0.bit.SCLR)
#define	FT0_TCCS0_CLK    	(IO_FT0.IO_TCCS0.bit.CLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_FT1,attr=IO,locate=0x24C
#endif

__IO_EXTENDED	struct {
	__WORD	IO_CPCLR1;
	__WORD	IO_TCDT1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	TCCSH1;
			__BYTE	TCCSL1;
		} byte;
		struct {
			__HWORD	ECKE:1;
			__HWORD	RESV81:1;
			__HWORD	RESV82:1;
			__HWORD	RESV83:1;
			__HWORD	RESV84:1;
			__HWORD	RESV85:1;
			__HWORD	ICLR:1;
			__HWORD	ICRE:1;
			__HWORD	RESV88:1;
			__HWORD	STOP:1;
			__HWORD	RESV810:1;
			__HWORD	SCLR:1;
			__HWORD	CLK:4;
		} bit;
	} IO_TCCS1;
} IO_FT1;

#define	FT1_CPCLR1	(IO_FT1.IO_CPCLR1)
#define	FT1_TCDT1	(IO_FT1.IO_TCDT1)
#define	FT1_TCCS1			(IO_FT1.IO_TCCS1.hword)
#define	FT1_TCCS1_TCCSH1    	(IO_FT1.IO_TCCS1.byte.TCCSH1)
#define	FT1_TCCS1_TCCSL1    	(IO_FT1.IO_TCCS1.byte.TCCSL1)
#define	FT1_TCCS1_ECKE    	(IO_FT1.IO_TCCS1.bit.ECKE)
#define	FT1_TCCS1_ICLR    	(IO_FT1.IO_TCCS1.bit.ICLR)
#define	FT1_TCCS1_ICRE    	(IO_FT1.IO_TCCS1.bit.ICRE)
#define	FT1_TCCS1_STOP    	(IO_FT1.IO_TCCS1.bit.STOP)
#define	FT1_TCCS1_SCLR    	(IO_FT1.IO_TCCS1.bit.SCLR)
#define	FT1_TCCS1_CLK    	(IO_FT1.IO_TCCS1.bit.CLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_GCN10,attr=IO,locate=0x25C
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	TSEL3:4;
			__HWORD	TSEL2:4;
			__HWORD	TSEL1:4;
			__HWORD	TSEL0:4;
	} bit;
} GCN10STR;

__IO_EXTENDED	  GCN10STR	IO_GCN10;
#define	_gcn10		(IO_GCN10)
#define	GCN10		(IO_GCN10.hword)
#define	GCN10_TSEL3  	(IO_GCN10.bit.TSEL3)
#define	GCN10_TSEL2  	(IO_GCN10.bit.TSEL2)
#define	GCN10_TSEL1  	(IO_GCN10.bit.TSEL1)
#define	GCN10_TSEL0  	(IO_GCN10.bit.TSEL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_GCN20,attr=IO,locate=0x25F
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	EN3:1;
			__BYTE	EN2:1;
			__BYTE	EN1:1;
			__BYTE	EN0:1;
	} bit;
} GCN20STR;

__IO_EXTENDED	  GCN20STR	IO_GCN20;
#define	_gcn20		(IO_GCN20)
#define	GCN20		(IO_GCN20.byte)
#define	GCN20_EN3  	(IO_GCN20.bit.EN3)
#define	GCN20_EN2  	(IO_GCN20.bit.EN2)
#define	GCN20_EN1  	(IO_GCN20.bit.EN1)
#define	GCN20_EN0  	(IO_GCN20.bit.EN0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_GCN11,attr=IO,locate=0x260
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	TSEL3:4;
			__HWORD	TSEL2:4;
			__HWORD	TSEL1:4;
			__HWORD	TSEL0:4;
	} bit;
} GCN11STR;

__IO_EXTENDED	  GCN11STR	IO_GCN11;
#define	_gcn11		(IO_GCN11)
#define	GCN11		(IO_GCN11.hword)
#define	GCN11_TSEL3  	(IO_GCN11.bit.TSEL3)
#define	GCN11_TSEL2  	(IO_GCN11.bit.TSEL2)
#define	GCN11_TSEL1  	(IO_GCN11.bit.TSEL1)
#define	GCN11_TSEL0  	(IO_GCN11.bit.TSEL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_GCN21,attr=IO,locate=0x263
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	EN3:1;
			__BYTE	EN2:1;
			__BYTE	EN1:1;
			__BYTE	EN0:1;
	} bit;
} GCN21STR;

__IO_EXTENDED	  GCN21STR	IO_GCN21;
#define	_gcn21		(IO_GCN21)
#define	GCN21		(IO_GCN21.byte)
#define	GCN21_EN3  	(IO_GCN21.bit.EN3)
#define	GCN21_EN2  	(IO_GCN21.bit.EN2)
#define	GCN21_EN1  	(IO_GCN21.bit.EN1)
#define	GCN21_EN0  	(IO_GCN21.bit.EN0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_GCN12,attr=IO,locate=0x264
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	TSEL3:4;
			__HWORD	TSEL2:4;
			__HWORD	TSEL1:4;
			__HWORD	TSEL0:4;
	} bit;
} GCN12STR;

__IO_EXTENDED	  GCN12STR	IO_GCN12;
#define	_gcn12		(IO_GCN12)
#define	GCN12		(IO_GCN12.hword)
#define	GCN12_TSEL3  	(IO_GCN12.bit.TSEL3)
#define	GCN12_TSEL2  	(IO_GCN12.bit.TSEL2)
#define	GCN12_TSEL1  	(IO_GCN12.bit.TSEL1)
#define	GCN12_TSEL0  	(IO_GCN12.bit.TSEL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_GCN22,attr=IO,locate=0x267
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	EN3:1;
			__BYTE	EN2:1;
			__BYTE	EN1:1;
			__BYTE	EN0:1;
	} bit;
} GCN22STR;

__IO_EXTENDED	  GCN22STR	IO_GCN22;
#define	_gcn22		(IO_GCN22)
#define	GCN22		(IO_GCN22.byte)
#define	GCN22_EN3  	(IO_GCN22.bit.EN3)
#define	GCN22_EN2  	(IO_GCN22.bit.EN2)
#define	GCN22_EN1  	(IO_GCN22.bit.EN1)
#define	GCN22_EN0  	(IO_GCN22.bit.EN0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPGDIV,attr=IO,locate=0x26B
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	DIV:2;
	} bit;
} PPGDIVSTR;

__IO_EXTENDED	  PPGDIVSTR	IO_PPGDIV;
#define	_ppgdiv		(IO_PPGDIV)
#define	PPGDIV		(IO_PPGDIV.byte)
#define	PPGDIV_DIV  	(IO_PPGDIV.bit.DIV)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR0,attr=IO,locate=0x26C
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR0;
#define	_ptmr0		(IO_PTMR0)
#define	PTMR0	(_ptmr0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR0,attr=IO,locate=0x26E
#endif

__IO_EXTENDED	__HWORD	IO_PCSR0;
#define	_pcsr0		(IO_PCSR0)
#define	PCSR0_	(_pcsr0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT0,attr=IO,locate=0x270
#endif

__IO_EXTENDED	__HWORD	IO_PDUT0;
#define	_pdut0		(IO_PDUT0)
#define	PDUT0_	(_pdut0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN0,attr=IO,locate=0x272
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	PCN0H;
		__BYTE	PCN0L;
	} byte;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN0STR;

__IO_EXTENDED	  PCN0STR	IO_PCN0;
#define	_pcn0		(IO_PCN0)
#define	PCN0		(IO_PCN0.hword)
#define	PCN0_PCN0H	(IO_PCN0.byte.PCN0H)
#define	PCN0_PCN0L	(IO_PCN0.byte.PCN0L)
#define	PCN0_CNTE  	(IO_PCN0.bit.CNTE)
#define	PCN0_STGR  	(IO_PCN0.bit.STGR)
#define	PCN0_MDSE  	(IO_PCN0.bit.MDSE)
#define	PCN0_RTRG  	(IO_PCN0.bit.RTRG)
#define	PCN0_CKS  	(IO_PCN0.bit.CKS)
#define	PCN0_PGMS  	(IO_PCN0.bit.PGMS)
#define	PCN0_EGS  	(IO_PCN0.bit.EGS)
#define	PCN0_IREN  	(IO_PCN0.bit.IREN)
#define	PCN0_IRQF  	(IO_PCN0.bit.IRQF)
#define	PCN0_IRS  	(IO_PCN0.bit.IRS)
#define	PCN0_OSEL  	(IO_PCN0.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR1,attr=IO,locate=0x274
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR1;
#define	_ptmr1		(IO_PTMR1)
#define	PTMR1	(_ptmr1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR1,attr=IO,locate=0x276
#endif

__IO_EXTENDED	__HWORD	IO_PCSR1;
#define	_pcsr1		(IO_PCSR1)
#define	PCSR1_	(_pcsr1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT1,attr=IO,locate=0x278
#endif

__IO_EXTENDED	__HWORD	IO_PDUT1;
#define	_pdut1		(IO_PDUT1)
#define	PDUT1_	(_pdut1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN1,attr=IO,locate=0x27A
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	PCN1H;
		__BYTE	PCN1L;
	} byte;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN1STR;

__IO_EXTENDED	  PCN1STR	IO_PCN1;
#define	_pcn1		(IO_PCN1)
#define	PCN1		(IO_PCN1.hword)
#define	PCN1_PCN1H	(IO_PCN1.byte.PCN1H)
#define	PCN1_PCN1L	(IO_PCN1.byte.PCN1L)
#define	PCN1_CNTE  	(IO_PCN1.bit.CNTE)
#define	PCN1_STGR  	(IO_PCN1.bit.STGR)
#define	PCN1_MDSE  	(IO_PCN1.bit.MDSE)
#define	PCN1_RTRG  	(IO_PCN1.bit.RTRG)
#define	PCN1_CKS  	(IO_PCN1.bit.CKS)
#define	PCN1_PGMS  	(IO_PCN1.bit.PGMS)
#define	PCN1_EGS  	(IO_PCN1.bit.EGS)
#define	PCN1_IREN  	(IO_PCN1.bit.IREN)
#define	PCN1_IRQF  	(IO_PCN1.bit.IRQF)
#define	PCN1_IRS  	(IO_PCN1.bit.IRS)
#define	PCN1_OSEL  	(IO_PCN1.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR2,attr=IO,locate=0x27C
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR2;
#define	_ptmr2		(IO_PTMR2)
#define	PTMR2	(_ptmr2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR2,attr=IO,locate=0x27E
#endif

__IO_EXTENDED	__HWORD	IO_PCSR2;
#define	_pcsr2		(IO_PCSR2)
#define	PCSR2 	(_pcsr2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT2,attr=IO,locate=0x280
#endif

__IO_EXTENDED	__HWORD	IO_PDUT2;
#define	_pdut2		(IO_PDUT2)
#define	PDUT2	(_pdut2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN2,attr=IO,locate=0x282
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	PCN2H;
		__BYTE	PCN2L;
	} byte;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN2STR;

__IO_EXTENDED	  PCN2STR	IO_PCN2;
#define	_pcn2		(IO_PCN2)
#define	PCN2		(IO_PCN2.hword)
#define	PCN2_PCN2H	(IO_PCN2.byte.PCN2H)
#define	PCN2_PCN2L	(IO_PCN2.byte.PCN2L)
#define	PCN2_CNTE  	(IO_PCN2.bit.CNTE)
#define	PCN2_STGR  	(IO_PCN2.bit.STGR)
#define	PCN2_MDSE  	(IO_PCN2.bit.MDSE)
#define	PCN2_RTRG  	(IO_PCN2.bit.RTRG)
#define	PCN2_CKS  	(IO_PCN2.bit.CKS)
#define	PCN2_PGMS  	(IO_PCN2.bit.PGMS)
#define	PCN2_EGS  	(IO_PCN2.bit.EGS)
#define	PCN2_IREN  	(IO_PCN2.bit.IREN)
#define	PCN2_IRQF  	(IO_PCN2.bit.IRQF)
#define	PCN2_IRS  	(IO_PCN2.bit.IRS)
#define	PCN2_OSEL  	(IO_PCN2.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR3,attr=IO,locate=0x284
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR3;
#define	_ptmr3		(IO_PTMR3)
#define	PTMR3	(_ptmr3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR3,attr=IO,locate=0x286
#endif

__IO_EXTENDED	__HWORD	IO_PCSR3;
#define	_pcsr3		(IO_PCSR3)
#define	PCSR3 	(_pcsr3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT3,attr=IO,locate=0x288
#endif

__IO_EXTENDED	__HWORD	IO_PDUT3;
#define	_pdut3		(IO_PDUT3)
#define	PDUT3	(_pdut3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN3,attr=IO,locate=0x28A
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	PCN3H;
		__BYTE	PCN3L;
	} byte;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN3STR;

__IO_EXTENDED	  PCN3STR	IO_PCN3;
#define	_pcn3		(IO_PCN3)
#define	PCN3		(IO_PCN3.hword)
#define	PCN3_PCN3H	(IO_PCN3.byte.PCN3H)
#define	PCN3_PCN3L	(IO_PCN3.byte.PCN3L)
#define	PCN3_CNTE  	(IO_PCN3.bit.CNTE)
#define	PCN3_STGR  	(IO_PCN3.bit.STGR)
#define	PCN3_MDSE  	(IO_PCN3.bit.MDSE)
#define	PCN3_RTRG  	(IO_PCN3.bit.RTRG)
#define	PCN3_CKS  	(IO_PCN3.bit.CKS)
#define	PCN3_PGMS  	(IO_PCN3.bit.PGMS)
#define	PCN3_EGS  	(IO_PCN3.bit.EGS)
#define	PCN3_IREN  	(IO_PCN3.bit.IREN)
#define	PCN3_IRQF  	(IO_PCN3.bit.IRQF)
#define	PCN3_IRS  	(IO_PCN3.bit.IRS)
#define	PCN3_OSEL  	(IO_PCN3.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR4,attr=IO,locate=0x28C
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR4;
#define	_ptmr4		(IO_PTMR4)
#define	PTMR4	(_ptmr4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR4,attr=IO,locate=0x28E
#endif

__IO_EXTENDED	__HWORD	IO_PCSR4;
#define	_pcsr4		(IO_PCSR4)
#define	PCSR4 	(_pcsr4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT4,attr=IO,locate=0x290
#endif

__IO_EXTENDED	__HWORD	IO_PDUT4;
#define	_pdut4		(IO_PDUT4)
#define	PDUT4	(_pdut4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN4,attr=IO,locate=0x292
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	PCN4H;
		__BYTE	PCN4L;
	} byte;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN4STR;

__IO_EXTENDED	  PCN4STR	IO_PCN4;
#define	_pcn4		(IO_PCN4)
#define	PCN4		(IO_PCN4.hword)
#define	PCN4_PCN4H	(IO_PCN4.byte.PCN4H)
#define	PCN4_PCN4L	(IO_PCN4.byte.PCN4L)
#define	PCN4_CNTE  	(IO_PCN4.bit.CNTE)
#define	PCN4_STGR  	(IO_PCN4.bit.STGR)
#define	PCN4_MDSE  	(IO_PCN4.bit.MDSE)
#define	PCN4_RTRG  	(IO_PCN4.bit.RTRG)
#define	PCN4_CKS  	(IO_PCN4.bit.CKS)
#define	PCN4_PGMS  	(IO_PCN4.bit.PGMS)
#define	PCN4_EGS  	(IO_PCN4.bit.EGS)
#define	PCN4_IREN  	(IO_PCN4.bit.IREN)
#define	PCN4_IRQF  	(IO_PCN4.bit.IRQF)
#define	PCN4_IRS  	(IO_PCN4.bit.IRS)
#define	PCN4_OSEL  	(IO_PCN4.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR5,attr=IO,locate=0x294
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR5;
#define	_ptmr5		(IO_PTMR5)
#define	PTMR5	(_ptmr5)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR5,attr=IO,locate=0x296
#endif

__IO_EXTENDED	__HWORD	IO_PCSR5;
#define	_pcsr5		(IO_PCSR5)
#define	PCSR5 	(_pcsr5)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT5,attr=IO,locate=0x298
#endif

__IO_EXTENDED	__HWORD	IO_PDUT5;
#define	_pdut5		(IO_PDUT5)
#define	PDUT5	(_pdut5)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN5,attr=IO,locate=0x29A
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	PCN5H;
		__BYTE	PCN5L;
	} byte;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN5STR;

__IO_EXTENDED	  PCN5STR	IO_PCN5;
#define	_pcn5		(IO_PCN5)
#define	PCN5		(IO_PCN5.hword)
#define	PCN5_PCN5H	(IO_PCN5.byte.PCN5H)
#define	PCN5_PCN5L	(IO_PCN5.byte.PCN5L)
#define	PCN5_CNTE  	(IO_PCN5.bit.CNTE)
#define	PCN5_STGR  	(IO_PCN5.bit.STGR)
#define	PCN5_MDSE  	(IO_PCN5.bit.MDSE)
#define	PCN5_RTRG  	(IO_PCN5.bit.RTRG)
#define	PCN5_CKS  	(IO_PCN5.bit.CKS)
#define	PCN5_PGMS  	(IO_PCN5.bit.PGMS)
#define	PCN5_EGS  	(IO_PCN5.bit.EGS)
#define	PCN5_IREN  	(IO_PCN5.bit.IREN)
#define	PCN5_IRQF  	(IO_PCN5.bit.IRQF)
#define	PCN5_IRS  	(IO_PCN5.bit.IRS)
#define	PCN5_OSEL  	(IO_PCN5.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR6,attr=IO,locate=0x29C
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR6;
#define	_ptmr6		(IO_PTMR6)
#define	PTMR6	(_ptmr6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR6,attr=IO,locate=0x29E
#endif

__IO_EXTENDED	__HWORD	IO_PCSR6;
#define	_pcsr6		(IO_PCSR6)
#define	PCSR6 	(_pcsr6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT6,attr=IO,locate=0x2A0
#endif

__IO_EXTENDED	__HWORD	IO_PDUT6;
#define	_pdut6		(IO_PDUT6)
#define	PDUT6	(_pdut6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN6,attr=IO,locate=0x2A2
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	PCN6H;
		__BYTE	PCN6L;
	} byte;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN6STR;

__IO_EXTENDED	  PCN6STR	IO_PCN6;
#define	_pcn6		(IO_PCN6)
#define	PCN6		(IO_PCN6.hword)
#define	PCN6_PCN6H	(IO_PCN6.byte.PCN6H)
#define	PCN6_PCN6L	(IO_PCN6.byte.PCN6L)
#define	PCN6_CNTE  	(IO_PCN6.bit.CNTE)
#define	PCN6_STGR  	(IO_PCN6.bit.STGR)
#define	PCN6_MDSE  	(IO_PCN6.bit.MDSE)
#define	PCN6_RTRG  	(IO_PCN6.bit.RTRG)
#define	PCN6_CKS  	(IO_PCN6.bit.CKS)
#define	PCN6_PGMS  	(IO_PCN6.bit.PGMS)
#define	PCN6_EGS  	(IO_PCN6.bit.EGS)
#define	PCN6_IREN  	(IO_PCN6.bit.IREN)
#define	PCN6_IRQF  	(IO_PCN6.bit.IRQF)
#define	PCN6_IRS  	(IO_PCN6.bit.IRS)
#define	PCN6_OSEL  	(IO_PCN6.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR7,attr=IO,locate=0x2A4
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR7;
#define	_ptmr7		(IO_PTMR7)
#define	PTMR7	(_ptmr7)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR7,attr=IO,locate=0x2A6
#endif

__IO_EXTENDED	__HWORD	IO_PCSR7;
#define	_pcsr7		(IO_PCSR7)
#define	PCSR7 	(_pcsr7)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT7,attr=IO,locate=0x2A8
#endif

__IO_EXTENDED	__HWORD	IO_PDUT7;
#define	_pdut7		(IO_PDUT7)
#define	PDUT7	(_pdut7)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN7,attr=IO,locate=0x2AA
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	PCN7H;
		__BYTE	PCN7L;
	} byte;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN7STR;

__IO_EXTENDED	  PCN7STR	IO_PCN7;
#define	_pcn7		(IO_PCN7)
#define	PCN7		(IO_PCN7.hword)
#define	PCN7_PCN7H	(IO_PCN7.byte.PCN7H)
#define	PCN7_PCN7L	(IO_PCN7.byte.PCN7L)
#define	PCN7_CNTE  	(IO_PCN7.bit.CNTE)
#define	PCN7_STGR  	(IO_PCN7.bit.STGR)
#define	PCN7_MDSE  	(IO_PCN7.bit.MDSE)
#define	PCN7_RTRG  	(IO_PCN7.bit.RTRG)
#define	PCN7_CKS  	(IO_PCN7.bit.CKS)
#define	PCN7_PGMS  	(IO_PCN7.bit.PGMS)
#define	PCN7_EGS  	(IO_PCN7.bit.EGS)
#define	PCN7_IREN  	(IO_PCN7.bit.IREN)
#define	PCN7_IRQF  	(IO_PCN7.bit.IRQF)
#define	PCN7_IRS  	(IO_PCN7.bit.IRS)
#define	PCN7_OSEL  	(IO_PCN7.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR8,attr=IO,locate=0x2AC
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR8;
#define	_ptmr8		(IO_PTMR8)
#define	PTMR8	(_ptmr8)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR8,attr=IO,locate=0x2AE
#endif

__IO_EXTENDED	__HWORD	IO_PCSR8;
#define	_pcsr8		(IO_PCSR8)
#define	PCSR8 	(_pcsr8)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT8,attr=IO,locate=0x2B0
#endif

__IO_EXTENDED	__HWORD	IO_PDUT8;
#define	_pdut8		(IO_PDUT8)
#define	PDUT8	(_pdut8)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN8,attr=IO,locate=0x2B2
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	PCN8H;
		__BYTE	PCN8L;
	} byte;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN8STR;

__IO_EXTENDED	  PCN8STR	IO_PCN8;
#define	_pcn8		(IO_PCN8)
#define	PCN8		(IO_PCN8.hword)
#define	PCN8_PCN8H	(IO_PCN8.byte.PCN8H)
#define	PCN8_PCN8L	(IO_PCN8.byte.PCN8L)
#define	PCN8_CNTE  	(IO_PCN8.bit.CNTE)
#define	PCN8_STGR  	(IO_PCN8.bit.STGR)
#define	PCN8_MDSE  	(IO_PCN8.bit.MDSE)
#define	PCN8_RTRG  	(IO_PCN8.bit.RTRG)
#define	PCN8_CKS  	(IO_PCN8.bit.CKS)
#define	PCN8_PGMS  	(IO_PCN8.bit.PGMS)
#define	PCN8_EGS  	(IO_PCN8.bit.EGS)
#define	PCN8_IREN  	(IO_PCN8.bit.IREN)
#define	PCN8_IRQF  	(IO_PCN8.bit.IRQF)
#define	PCN8_IRS  	(IO_PCN8.bit.IRS)
#define	PCN8_OSEL  	(IO_PCN8.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR9,attr=IO,locate=0x2B4
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR9;
#define	_ptmr9		(IO_PTMR9)
#define	PTMR9	(_ptmr9)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR9,attr=IO,locate=0x2B6
#endif

__IO_EXTENDED	__HWORD	IO_PCSR9;
#define	_pcsr9		(IO_PCSR9)
#define	PCSR9 	(_pcsr9)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT9,attr=IO,locate=0x2B8
#endif

__IO_EXTENDED	__HWORD	IO_PDUT9;
#define	_pdut9		(IO_PDUT9)
#define	PDUT9	(_pdut9)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN9,attr=IO,locate=0x2BA
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	PCN9H;
		__BYTE	PCN9L;
	} byte;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN9STR;

__IO_EXTENDED	  PCN9STR	IO_PCN9;
#define	_pcn9		(IO_PCN9)
#define	PCN9		(IO_PCN9.hword)
#define	PCN9_PCN9H	(IO_PCN9.byte.PCN9H)
#define	PCN9_PCN9L	(IO_PCN9.byte.PCN9L)
#define	PCN9_CNTE  	(IO_PCN9.bit.CNTE)
#define	PCN9_STGR  	(IO_PCN9.bit.STGR)
#define	PCN9_MDSE  	(IO_PCN9.bit.MDSE)
#define	PCN9_RTRG  	(IO_PCN9.bit.RTRG)
#define	PCN9_CKS  	(IO_PCN9.bit.CKS)
#define	PCN9_PGMS  	(IO_PCN9.bit.PGMS)
#define	PCN9_EGS  	(IO_PCN9.bit.EGS)
#define	PCN9_IREN  	(IO_PCN9.bit.IREN)
#define	PCN9_IRQF  	(IO_PCN9.bit.IRQF)
#define	PCN9_IRS  	(IO_PCN9.bit.IRS)
#define	PCN9_OSEL  	(IO_PCN9.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PTMR10,attr=IO,locate=0x2BC
#endif

__IO_EXTENDED	const __HWORD	IO_PTMR10;
#define	_ptmr10		(IO_PTMR10)
#define	PTMR10	(_ptmr10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR10,attr=IO,locate=0x2BE
#endif

__IO_EXTENDED	__HWORD	IO_PCSR10;
#define	_pcsr10		(IO_PCSR10)
#define	PCSR10 	(_pcsr10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDUT10,attr=IO,locate=0x2C0
#endif

__IO_EXTENDED	__HWORD	IO_PDUT10;
#define	_pdut10		(IO_PDUT10)
#define	PDUT10	(_pdut10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCN10,attr=IO,locate=0x2C2
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	PCN10H;
		__BYTE	PCN10L;
	} byte;
	struct {
			__HWORD	CNTE:1;
			__HWORD	STGR:1;
			__HWORD	MDSE:1;
			__HWORD	RTRG:1;
			__HWORD	CKS:2;
			__HWORD	PGMS:1;
			__HWORD	RESV6:1;
			__HWORD	EGS:2;
			__HWORD	IREN:1;
			__HWORD	IRQF:1;
			__HWORD	IRS:2;
			__HWORD	RESV11:1;
			__HWORD	OSEL:1;
	} bit;
} PCN10STR;

__IO_EXTENDED	  PCN10STR	IO_PCN10;
#define	_pcn10		(IO_PCN10)
#define	PCN10		(IO_PCN10.hword)
#define	PCN10_PCN10H	(IO_PCN10.byte.PCN10H)
#define	PCN10_PCN10L	(IO_PCN10.byte.PCN10L)
#define	PCN10_CNTE  	(IO_PCN10.bit.CNTE)
#define	PCN10_STGR  	(IO_PCN10.bit.STGR)
#define	PCN10_MDSE  	(IO_PCN10.bit.MDSE)
#define	PCN10_RTRG  	(IO_PCN10.bit.RTRG)
#define	PCN10_CKS  	(IO_PCN10.bit.CKS)
#define	PCN10_PGMS  	(IO_PCN10.bit.PGMS)
#define	PCN10_EGS  	(IO_PCN10.bit.EGS)
#define	PCN10_IREN  	(IO_PCN10.bit.IREN)
#define	PCN10_IRQF  	(IO_PCN10.bit.IRQF)
#define	PCN10_IRS  	(IO_PCN10.bit.IRS)
#define	PCN10_OSEL  	(IO_PCN10.bit.OSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IPCP0,attr=IO,locate=0x2C4
#endif

__IO_EXTENDED	const __WORD	IO_IPCP0;
#define	_ipcp0		(IO_IPCP0)
#define	IPCP0	(_ipcp0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IPCP1,attr=IO,locate=0x2C8
#endif

__IO_EXTENDED	const __WORD	IO_IPCP1;
#define	_ipcp1		(IO_IPCP1)
#define	IPCP1	(_ipcp1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICFS01,attr=IO,locate=0x2CC
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SEL1:1;
			__BYTE	SEL0:1;
	} bit;
} ICFS01STR;

__IO_EXTENDED	  ICFS01STR	IO_ICFS01;
#define	_icfs01		(IO_ICFS01)
#define	ICFS01		(IO_ICFS01.byte)
#define	ICFS01_SEL1  	(IO_ICFS01.bit.SEL1)
#define	ICFS01_SEL0  	(IO_ICFS01.bit.SEL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_LSYNS0,attr=IO,locate=0x2CE
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	LSYN5:1;
			__BYTE	LSYN4:1;
			__BYTE	LSYN3:1;
			__BYTE	LSYN2:1;
			__BYTE	LSYN1:1;
			__BYTE	LSYN0:1;
	} bit;
} LSYNS0STR;

__IO_EXTENDED	  LSYNS0STR	IO_LSYNS0;
#define	_lsyns0		(IO_LSYNS0)
#define	LSYNS0		(IO_LSYNS0.byte)
#define	LSYNS0_LSYN5  	(IO_LSYNS0.bit.LSYN5)
#define	LSYNS0_LSYN4  	(IO_LSYNS0.bit.LSYN4)
#define	LSYNS0_LSYN3  	(IO_LSYNS0.bit.LSYN3)
#define	LSYNS0_LSYN2  	(IO_LSYNS0.bit.LSYN2)
#define	LSYNS0_LSYN1  	(IO_LSYNS0.bit.LSYN1)
#define	LSYNS0_LSYN0  	(IO_LSYNS0.bit.LSYN0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICS01,attr=IO,locate=0x2CF
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICP1:1;
			__BYTE	ICP0:1;
			__BYTE	ICE1:1;
			__BYTE	ICE0:1;
			__BYTE	EG1:2;
			__BYTE	EG0:2;
	} bit;
} ICS01STR;

__IO_EXTENDED	  ICS01STR	IO_ICS01;
#define	_ics01		(IO_ICS01)
#define	ICS01		(IO_ICS01.byte)
#define	ICS01_ICP1  	(IO_ICS01.bit.ICP1)
#define	ICS01_ICP0  	(IO_ICS01.bit.ICP0)
#define	ICS01_ICE1  	(IO_ICS01.bit.ICE1)
#define	ICS01_ICE0  	(IO_ICS01.bit.ICE0)
#define	ICS01_EG1  	(IO_ICS01.bit.EG1)
#define	ICS01_EG0  	(IO_ICS01.bit.EG0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IPCP2,attr=IO,locate=0x2D0
#endif

__IO_EXTENDED	const __WORD	IO_IPCP2;
#define	_ipcp2		(IO_IPCP2)
#define	IPCP2	(_ipcp2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IPCP3,attr=IO,locate=0x2D4
#endif

__IO_EXTENDED	const __WORD	IO_IPCP3;
#define	_ipcp3		(IO_IPCP3)
#define	IPCP3	(_ipcp3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICFS23,attr=IO,locate=0x2D8
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SEL3:1;
			__BYTE	SEL2:1;
	} bit;
} ICFS23STR;

__IO_EXTENDED	  ICFS23STR	IO_ICFS23;
#define	_icfs23		(IO_ICFS23)
#define	ICFS23		(IO_ICFS23.byte)
#define	ICFS23_SEL3  	(IO_ICFS23.bit.SEL3)
#define	ICFS23_SEL2  	(IO_ICFS23.bit.SEL2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICS23,attr=IO,locate=0x2DB
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICP3:1;
			__BYTE	ICP2:1;
			__BYTE	ICE3:1;
			__BYTE	ICE2:1;
			__BYTE	EG3:2;
			__BYTE	EG2:2;
	} bit;
} ICS23STR;

__IO_EXTENDED	  ICS23STR	IO_ICS23;
#define	_ics23		(IO_ICS23)
#define	ICS23		(IO_ICS23.byte)
#define	ICS23_ICP3  	(IO_ICS23.bit.ICP3)
#define	ICS23_ICP2  	(IO_ICS23.bit.ICP2)
#define	ICS23_ICE3  	(IO_ICS23.bit.ICE3)
#define	ICS23_ICE2  	(IO_ICS23.bit.ICE2)
#define	ICS23_EG3  	(IO_ICS23.bit.EG3)
#define	ICS23_EG2  	(IO_ICS23.bit.EG2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IPCP4,attr=IO,locate=0x2DC
#endif

__IO_EXTENDED	const __WORD	IO_IPCP4;
#define	_ipcp4		(IO_IPCP4)
#define	IPCP4	(_ipcp4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IPCP5,attr=IO,locate=0x2E0
#endif

__IO_EXTENDED	const __WORD	IO_IPCP5;
#define	_ipcp5		(IO_IPCP5)
#define	IPCP5	(_ipcp5)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICFS45,attr=IO,locate=0x2E4
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SEL5:1;
			__BYTE	SEL4:1;
	} bit;
} ICFS45STR;

__IO_EXTENDED	  ICFS45STR	IO_ICFS45;
#define	_icfs45		(IO_ICFS45)
#define	ICFS45		(IO_ICFS45.byte)
#define	ICFS45_SEL5  	(IO_ICFS45.bit.SEL5)
#define	ICFS45_SEL4  	(IO_ICFS45.bit.SEL4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICS45,attr=IO,locate=0x2E7
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICP5:1;
			__BYTE	ICP4:1;
			__BYTE	ICE5:1;
			__BYTE	ICE4:1;
			__BYTE	EG5:2;
			__BYTE	EG4:2;
	} bit;
} ICS45STR;

__IO_EXTENDED	  ICS45STR	IO_ICS45;
#define	_ics45		(IO_ICS45)
#define	ICS45		(IO_ICS45.byte)
#define	ICS45_ICP5  	(IO_ICS45.bit.ICP5)
#define	ICS45_ICP4  	(IO_ICS45.bit.ICP4)
#define	ICS45_ICE5  	(IO_ICS45.bit.ICE5)
#define	ICS45_ICE4  	(IO_ICS45.bit.ICE4)
#define	ICS45_EG5  	(IO_ICS45.bit.EG5)
#define	ICS45_EG4  	(IO_ICS45.bit.EG4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCCP0,attr=IO,locate=0x2E8
#endif

__IO_EXTENDED	__WORD	IO_OCCP0;
#define	_occp0		(IO_OCCP0)
#define	OCCP0	(_occp0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCCP1,attr=IO,locate=0x2EC
#endif

__IO_EXTENDED	__WORD	IO_OCCP1;
#define	_occp1		(IO_OCCP1)
#define	OCCP1	(_occp1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCFS01,attr=IO,locate=0x2F0
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SEL1:1;
			__BYTE	SEL0:1;
	} bit;
} OCFS01STR;

__IO_EXTENDED	  OCFS01STR	IO_OCFS01;
#define	_ocfs01		(IO_OCFS01)
#define	OCFS01		(IO_OCFS01.byte)
#define	OCFS01_SEL1  	(IO_OCFS01.bit.SEL1)
#define	OCFS01_SEL0  	(IO_OCFS01.bit.SEL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCS01,attr=IO,locate=0x2F2
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	OCSH01;
		__BYTE	OCSL01;
	} byte;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	CMOD:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	OTD1:1;
			__HWORD	OTD0:1;
			__HWORD	IOP1:1;
			__HWORD	IOP0:1;
			__HWORD	IOE1:1;
			__HWORD	IOE0:1;
			__HWORD	RESV12:1;
			__HWORD	RESV13:1;
			__HWORD	CST1:1;
			__HWORD	CST0:1;
	} bit;
} OCS01STR;

__IO_EXTENDED	  OCS01STR	IO_OCS01;
#define	_ocs01		(IO_OCS01)
#define	OCS01		(IO_OCS01.hword)
#define	OCS01_OCSH01	(IO_OCS01.byte.OCSH01)
#define	OCS01_OCSL01	(IO_OCS01.byte.OCSL01)
#define	OCS01_CMOD  	(IO_OCS01.bit.CMOD)
#define	OCS01_OTD1  	(IO_OCS01.bit.OTD1)
#define	OCS01_OTD0  	(IO_OCS01.bit.OTD0)
#define	OCS01_IOP1  	(IO_OCS01.bit.IOP1)
#define	OCS01_IOP0  	(IO_OCS01.bit.IOP0)
#define	OCS01_IOE1  	(IO_OCS01.bit.IOE1)
#define	OCS01_IOE0  	(IO_OCS01.bit.IOE0)
#define	OCS01_CST1  	(IO_OCS01.bit.CST1)
#define	OCS01_CST0  	(IO_OCS01.bit.CST0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCCP2,attr=IO,locate=0x2F4
#endif

__IO_EXTENDED	__WORD	IO_OCCP2;
#define	_occp2		(IO_OCCP2)
#define	OCCP2	(_occp2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCCP3,attr=IO,locate=0x2F8
#endif

__IO_EXTENDED	__WORD	IO_OCCP3;
#define	_occp3		(IO_OCCP3)
#define	OCCP3	(_occp3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCFS23,attr=IO,locate=0x2FC
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SEL3:1;
			__BYTE	SEL2:1;
	} bit;
} OCFS23STR;

__IO_EXTENDED	  OCFS23STR	IO_OCFS23;
#define	_ocfs23		(IO_OCFS23)
#define	OCFS23		(IO_OCFS23.byte)
#define	OCFS23_SEL3  	(IO_OCFS23.bit.SEL3)
#define	OCFS23_SEL2  	(IO_OCFS23.bit.SEL2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCS23,attr=IO,locate=0x2FE
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	OCSH23;
		__BYTE	OCSL23;
	} byte;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	CMOD:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	OTD3:1;
			__HWORD	OTD2:1;
			__HWORD	IOP3:1;
			__HWORD	IOP2:1;
			__HWORD	IOE3:1;
			__HWORD	IOE2:1;
			__HWORD	RESV12:1;
			__HWORD	RESV13:1;
			__HWORD	CST3:1;
			__HWORD	CST2:1;
	} bit;
} OCS23STR;

__IO_EXTENDED	  OCS23STR	IO_OCS23;
#define	_ocs23		(IO_OCS23)
#define	OCS23		(IO_OCS23.hword)
#define	OCS23_OCSH23	(IO_OCS23.byte.OCSH23)
#define	OCS23_OCSL23	(IO_OCS23.byte.OCSL23)
#define	OCS23_CMOD  	(IO_OCS23.bit.CMOD)
#define	OCS23_OTD3  	(IO_OCS23.bit.OTD3)
#define	OCS23_OTD2  	(IO_OCS23.bit.OTD2)
#define	OCS23_IOP3  	(IO_OCS23.bit.IOP3)
#define	OCS23_IOP2  	(IO_OCS23.bit.IOP2)
#define	OCS23_IOE3  	(IO_OCS23.bit.IOE3)
#define	OCS23_IOE2  	(IO_OCS23.bit.IOE2)
#define	OCS23_CST3  	(IO_OCS23.bit.CST3)
#define	OCS23_CST2  	(IO_OCS23.bit.CST2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_MPU0,attr=IO,locate=0x310
#endif

__IO_EXTENDED	struct {
	__HWORD	IO_RESV1;
	union {
		__HWORD	hword;
		struct {
			__HWORD	PIE:1;
			__HWORD	PRE:1;
			__HWORD	PWE:1;
			__HWORD	UIE:1;
			__HWORD	URE:1;
			__HWORD	UWE:1;
			__HWORD	RESV26:1;
			__HWORD	BE:1;
			__HWORD	RESV28:1;
			__HWORD	RESV29:1;
			__HWORD	RESV210:1;
			__HWORD	RESV211:1;
			__HWORD	PAN:2;
			__HWORD	DEE:1;
			__HWORD	MPE:1;
		} bit;
	} IO_MPUCR;
	__HWORD	IO_RESV2;
	__HWORD	IO_RESV3;
	__WORD	IO_RESV15;
	__HWORD	IO_RESV4;
	__HWORD	IO_RESV16;
	const __WORD	IO_DPVAR;
	__HWORD	IO_RESV5;
	union {
		__HWORD	hword;
		struct {
			__HWORD	RESV220:1;
			__HWORD	RESV221:1;
			__HWORD	RESV222:1;
			__HWORD	RESV223:1;
			__HWORD	RESV224:1;
			__HWORD	RESV225:1;
			__HWORD	RESV226:1;
			__HWORD	RESV227:1;
			__HWORD	RW:2;
			__HWORD	SZ:2;
			__HWORD	MD:1;
			__HWORD	RESV2211:1;
			__HWORD	RESV2212:1;
			__HWORD	DPV:1;
		} bit;
	} IO_DPVSR;
	const __WORD	IO_DEAR;
	__HWORD	IO_RESV6;
	union {
		__HWORD	hword;
		struct {
			__HWORD	RESV300:1;
			__HWORD	RESV301:1;
			__HWORD	RESV302:1;
			__HWORD	RESV303:1;
			__HWORD	RESV304:1;
			__HWORD	RESV305:1;
			__HWORD	RESV306:1;
			__HWORD	RESV307:1;
			__HWORD	RW:2;
			__HWORD	SZ:2;
			__HWORD	MD:1;
			__HWORD	RESV3011:1;
			__HWORD	RESV3012:1;
			__HWORD	DAE:1;
		} bit;
	} IO_DESR;
	__WORD	IO_PABR0;
	__HWORD	IO_RESV7;
	union {
		__HWORD	hword;
		struct {
			__HWORD	PIE:1;
			__HWORD	PRE:1;
			__HWORD	PWE:1;
			__HWORD	UIE:1;
			__HWORD	URE:1;
			__HWORD	UWE:1;
			__HWORD	RESV386:1;
			__HWORD	BE:1;
			__HWORD	ASZ:5;
			__HWORD	RESV389:1;
			__HWORD	RESV3810:1;
			__HWORD	PAE:1;
		} bit;
	} IO_PACR0;
	__WORD	IO_PABR1;
	__HWORD	IO_RESV8;
	union {
		__HWORD	hword;
		struct {
			__HWORD	PIE:1;
			__HWORD	PRE:1;
			__HWORD	PWE:1;
			__HWORD	UIE:1;
			__HWORD	URE:1;
			__HWORD	UWE:1;
			__HWORD	RESV466:1;
			__HWORD	BE:1;
			__HWORD	ASZ:5;
			__HWORD	RESV469:1;
			__HWORD	RESV4610:1;
			__HWORD	PAE:1;
		} bit;
	} IO_PACR1;
	__WORD	IO_PABR2;
	__HWORD	IO_RESV9;
	union {
		__HWORD	hword;
		struct {
			__HWORD	PIE:1;
			__HWORD	PRE:1;
			__HWORD	PWE:1;
			__HWORD	UIE:1;
			__HWORD	URE:1;
			__HWORD	UWE:1;
			__HWORD	RESV546:1;
			__HWORD	BE:1;
			__HWORD	ASZ:5;
			__HWORD	RESV549:1;
			__HWORD	RESV5410:1;
			__HWORD	PAE:1;
		} bit;
	} IO_PACR2;
	__WORD	IO_PABR3;
	__HWORD	IO_RESV10;
	union {
		__HWORD	hword;
		struct {
			__HWORD	PIE:1;
			__HWORD	PRE:1;
			__HWORD	PWE:1;
			__HWORD	UIE:1;
			__HWORD	URE:1;
			__HWORD	UWE:1;
			__HWORD	RESV626:1;
			__HWORD	BE:1;
			__HWORD	ASZ:5;
			__HWORD	RESV629:1;
			__HWORD	RESV6210:1;
			__HWORD	PAE:1;
		} bit;
	} IO_PACR3;
	__WORD	IO_PABR4;
	__HWORD	IO_RESV11;
	union {
		__HWORD	hword;
		struct {
			__HWORD	PIE:1;
			__HWORD	PRE:1;
			__HWORD	PWE:1;
			__HWORD	UIE:1;
			__HWORD	URE:1;
			__HWORD	UWE:1;
			__HWORD	RESV706:1;
			__HWORD	BE:1;
			__HWORD	ASZ:5;
			__HWORD	RESV709:1;
			__HWORD	RESV7010:1;
			__HWORD	PAE:1;
		} bit;
	} IO_PACR4;
	__WORD	IO_PABR5;
	__HWORD	IO_RESV12;
	union {
		__HWORD	hword;
		struct {
			__HWORD	PIE:1;
			__HWORD	PRE:1;
			__HWORD	PWE:1;
			__HWORD	UIE:1;
			__HWORD	URE:1;
			__HWORD	UWE:1;
			__HWORD	RESV786:1;
			__HWORD	BE:1;
			__HWORD	ASZ:5;
			__HWORD	RESV789:1;
			__HWORD	RESV7810:1;
			__HWORD	PAE:1;
		} bit;
	} IO_PACR5;
	__WORD	IO_PABR6;
	__HWORD	IO_RESV13;
	union {
		__HWORD	hword;
		struct {
			__HWORD	PIE:1;
			__HWORD	PRE:1;
			__HWORD	PWE:1;
			__HWORD	UIE:1;
			__HWORD	URE:1;
			__HWORD	UWE:1;
			__HWORD	RESV866:1;
			__HWORD	BE:1;
			__HWORD	ASZ:5;
			__HWORD	RESV869:1;
			__HWORD	RESV8610:1;
			__HWORD	PAE:1;
		} bit;
	} IO_PACR6;
	__WORD	IO_PABR7;
	__HWORD	IO_RESV14;
	union {
		__HWORD	hword;
		struct {
			__HWORD	PIE:1;
			__HWORD	PRE:1;
			__HWORD	PWE:1;
			__HWORD	UIE:1;
			__HWORD	URE:1;
			__HWORD	UWE:1;
			__HWORD	RESV946:1;
			__HWORD	BE:1;
			__HWORD	ASZ:5;
			__HWORD	RESV949:1;
			__HWORD	RESV9410:1;
			__HWORD	PAE:1;
		} bit;
	} IO_PACR7;
} IO_MPU0;

#define	MPU0_MPUCR			(IO_MPU0.IO_MPUCR.hword)
#define	MPU0_MPUCR_PIE    	(IO_MPU0.IO_MPUCR.bit.PIE)
#define	MPU0_MPUCR_PRE    	(IO_MPU0.IO_MPUCR.bit.PRE)
#define	MPU0_MPUCR_PWE    	(IO_MPU0.IO_MPUCR.bit.PWE)
#define	MPU0_MPUCR_UIE    	(IO_MPU0.IO_MPUCR.bit.UIE)
#define	MPU0_MPUCR_URE    	(IO_MPU0.IO_MPUCR.bit.URE)
#define	MPU0_MPUCR_UWE    	(IO_MPU0.IO_MPUCR.bit.UWE)
#define	MPU0_MPUCR_BE    	(IO_MPU0.IO_MPUCR.bit.BE)
#define	MPU0_MPUCR_PAN    	(IO_MPU0.IO_MPUCR.bit.PAN)
#define	MPU0_MPUCR_DEE    	(IO_MPU0.IO_MPUCR.bit.DEE)
#define	MPU0_MPUCR_MPE    	(IO_MPU0.IO_MPUCR.bit.MPE)
#define	MPU0_DPVAR	(IO_MPU0.IO_DPVAR)
#define	MPU0_DPVSR			(IO_MPU0.IO_DPVSR.hword)
#define	MPU0_DPVSR_RW    	(IO_MPU0.IO_DPVSR.bit.RW)
#define	MPU0_DPVSR_SZ    	(IO_MPU0.IO_DPVSR.bit.SZ)
#define	MPU0_DPVSR_MD    	(IO_MPU0.IO_DPVSR.bit.MD)
#define	MPU0_DPVSR_DPV    	(IO_MPU0.IO_DPVSR.bit.DPV)
#define	MPU0_DEAR	(IO_MPU0.IO_DEAR)
#define	MPU0_DESR			(IO_MPU0.IO_DESR.hword)
#define	MPU0_DESR_RW    	(IO_MPU0.IO_DESR.bit.RW)
#define	MPU0_DESR_SZ    	(IO_MPU0.IO_DESR.bit.SZ)
#define	MPU0_DESR_MD    	(IO_MPU0.IO_DESR.bit.MD)
#define	MPU0_DESR_DAE    	(IO_MPU0.IO_DESR.bit.DAE)
#define	MPU0_PABR0	(IO_MPU0.IO_PABR0)
#define	MPU0_PACR0			(IO_MPU0.IO_PACR0.hword)
#define	MPU0_PACR0_PIE    	(IO_MPU0.IO_PACR0.bit.PIE)
#define	MPU0_PACR0_PRE    	(IO_MPU0.IO_PACR0.bit.PRE)
#define	MPU0_PACR0_PWE    	(IO_MPU0.IO_PACR0.bit.PWE)
#define	MPU0_PACR0_UIE    	(IO_MPU0.IO_PACR0.bit.UIE)
#define	MPU0_PACR0_URE    	(IO_MPU0.IO_PACR0.bit.URE)
#define	MPU0_PACR0_UWE    	(IO_MPU0.IO_PACR0.bit.UWE)
#define	MPU0_PACR0_BE    	(IO_MPU0.IO_PACR0.bit.BE)
#define	MPU0_PACR0_ASZ    	(IO_MPU0.IO_PACR0.bit.ASZ)
#define	MPU0_PACR0_PAE    	(IO_MPU0.IO_PACR0.bit.PAE)
#define	MPU0_PABR1	(IO_MPU0.IO_PABR1)
#define	MPU0_PACR1			(IO_MPU0.IO_PACR1.hword)
#define	MPU0_PACR1_PIE    	(IO_MPU0.IO_PACR1.bit.PIE)
#define	MPU0_PACR1_PRE    	(IO_MPU0.IO_PACR1.bit.PRE)
#define	MPU0_PACR1_PWE    	(IO_MPU0.IO_PACR1.bit.PWE)
#define	MPU0_PACR1_UIE    	(IO_MPU0.IO_PACR1.bit.UIE)
#define	MPU0_PACR1_URE    	(IO_MPU0.IO_PACR1.bit.URE)
#define	MPU0_PACR1_UWE    	(IO_MPU0.IO_PACR1.bit.UWE)
#define	MPU0_PACR1_BE    	(IO_MPU0.IO_PACR1.bit.BE)
#define	MPU0_PACR1_ASZ    	(IO_MPU0.IO_PACR1.bit.ASZ)
#define	MPU0_PACR1_PAE    	(IO_MPU0.IO_PACR1.bit.PAE)
#define	MPU0_PABR2	(IO_MPU0.IO_PABR2)
#define	MPU0_PACR2			(IO_MPU0.IO_PACR2.hword)
#define	MPU0_PACR2_PIE    	(IO_MPU0.IO_PACR2.bit.PIE)
#define	MPU0_PACR2_PRE    	(IO_MPU0.IO_PACR2.bit.PRE)
#define	MPU0_PACR2_PWE    	(IO_MPU0.IO_PACR2.bit.PWE)
#define	MPU0_PACR2_UIE    	(IO_MPU0.IO_PACR2.bit.UIE)
#define	MPU0_PACR2_URE    	(IO_MPU0.IO_PACR2.bit.URE)
#define	MPU0_PACR2_UWE    	(IO_MPU0.IO_PACR2.bit.UWE)
#define	MPU0_PACR2_BE    	(IO_MPU0.IO_PACR2.bit.BE)
#define	MPU0_PACR2_ASZ    	(IO_MPU0.IO_PACR2.bit.ASZ)
#define	MPU0_PACR2_PAE    	(IO_MPU0.IO_PACR2.bit.PAE)
#define	MPU0_PABR3	(IO_MPU0.IO_PABR3)
#define	MPU0_PACR3			(IO_MPU0.IO_PACR3.hword)
#define	MPU0_PACR3_PIE    	(IO_MPU0.IO_PACR3.bit.PIE)
#define	MPU0_PACR3_PRE    	(IO_MPU0.IO_PACR3.bit.PRE)
#define	MPU0_PACR3_PWE    	(IO_MPU0.IO_PACR3.bit.PWE)
#define	MPU0_PACR3_UIE    	(IO_MPU0.IO_PACR3.bit.UIE)
#define	MPU0_PACR3_URE    	(IO_MPU0.IO_PACR3.bit.URE)
#define	MPU0_PACR3_UWE    	(IO_MPU0.IO_PACR3.bit.UWE)
#define	MPU0_PACR3_BE    	(IO_MPU0.IO_PACR3.bit.BE)
#define	MPU0_PACR3_ASZ    	(IO_MPU0.IO_PACR3.bit.ASZ)
#define	MPU0_PACR3_PAE    	(IO_MPU0.IO_PACR3.bit.PAE)
#define	MPU0_PABR4	(IO_MPU0.IO_PABR4)
#define	MPU0_PACR4			(IO_MPU0.IO_PACR4.hword)
#define	MPU0_PACR4_PIE    	(IO_MPU0.IO_PACR4.bit.PIE)
#define	MPU0_PACR4_PRE    	(IO_MPU0.IO_PACR4.bit.PRE)
#define	MPU0_PACR4_PWE    	(IO_MPU0.IO_PACR4.bit.PWE)
#define	MPU0_PACR4_UIE    	(IO_MPU0.IO_PACR4.bit.UIE)
#define	MPU0_PACR4_URE    	(IO_MPU0.IO_PACR4.bit.URE)
#define	MPU0_PACR4_UWE    	(IO_MPU0.IO_PACR4.bit.UWE)
#define	MPU0_PACR4_BE    	(IO_MPU0.IO_PACR4.bit.BE)
#define	MPU0_PACR4_ASZ    	(IO_MPU0.IO_PACR4.bit.ASZ)
#define	MPU0_PACR4_PAE    	(IO_MPU0.IO_PACR4.bit.PAE)
#define	MPU0_PABR5	(IO_MPU0.IO_PABR5)
#define	MPU0_PACR5			(IO_MPU0.IO_PACR5.hword)
#define	MPU0_PACR5_PIE    	(IO_MPU0.IO_PACR5.bit.PIE)
#define	MPU0_PACR5_PRE    	(IO_MPU0.IO_PACR5.bit.PRE)
#define	MPU0_PACR5_PWE    	(IO_MPU0.IO_PACR5.bit.PWE)
#define	MPU0_PACR5_UIE    	(IO_MPU0.IO_PACR5.bit.UIE)
#define	MPU0_PACR5_URE    	(IO_MPU0.IO_PACR5.bit.URE)
#define	MPU0_PACR5_UWE    	(IO_MPU0.IO_PACR5.bit.UWE)
#define	MPU0_PACR5_BE    	(IO_MPU0.IO_PACR5.bit.BE)
#define	MPU0_PACR5_ASZ    	(IO_MPU0.IO_PACR5.bit.ASZ)
#define	MPU0_PACR5_PAE    	(IO_MPU0.IO_PACR5.bit.PAE)
#define	MPU0_PABR6	(IO_MPU0.IO_PABR6)
#define	MPU0_PACR6			(IO_MPU0.IO_PACR6.hword)
#define	MPU0_PACR6_PIE    	(IO_MPU0.IO_PACR6.bit.PIE)
#define	MPU0_PACR6_PRE    	(IO_MPU0.IO_PACR6.bit.PRE)
#define	MPU0_PACR6_PWE    	(IO_MPU0.IO_PACR6.bit.PWE)
#define	MPU0_PACR6_UIE    	(IO_MPU0.IO_PACR6.bit.UIE)
#define	MPU0_PACR6_URE    	(IO_MPU0.IO_PACR6.bit.URE)
#define	MPU0_PACR6_UWE    	(IO_MPU0.IO_PACR6.bit.UWE)
#define	MPU0_PACR6_BE    	(IO_MPU0.IO_PACR6.bit.BE)
#define	MPU0_PACR6_ASZ    	(IO_MPU0.IO_PACR6.bit.ASZ)
#define	MPU0_PACR6_PAE    	(IO_MPU0.IO_PACR6.bit.PAE)
#define	MPU0_PABR7	(IO_MPU0.IO_PABR7)
#define	MPU0_PACR7			(IO_MPU0.IO_PACR7.hword)
#define	MPU0_PACR7_PIE    	(IO_MPU0.IO_PACR7.bit.PIE)
#define	MPU0_PACR7_PRE    	(IO_MPU0.IO_PACR7.bit.PRE)
#define	MPU0_PACR7_PWE    	(IO_MPU0.IO_PACR7.bit.PWE)
#define	MPU0_PACR7_UIE    	(IO_MPU0.IO_PACR7.bit.UIE)
#define	MPU0_PACR7_URE    	(IO_MPU0.IO_PACR7.bit.URE)
#define	MPU0_PACR7_UWE    	(IO_MPU0.IO_PACR7.bit.UWE)
#define	MPU0_PACR7_BE    	(IO_MPU0.IO_PACR7.bit.BE)
#define	MPU0_PACR7_ASZ    	(IO_MPU0.IO_PACR7.bit.ASZ)
#define	MPU0_PACR7_PAE    	(IO_MPU0.IO_PACR7.bit.PAE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL0,attr=IO,locate=0x400
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	EISEL:3;
	} bit;
} ICSEL0STR;

__IO_EXTENDED	  ICSEL0STR	IO_ICSEL0;
#define	_icsel0		(IO_ICSEL0)
#define	ICSEL0		(IO_ICSEL0.byte)
#define	ICSEL0_EISEL  	(IO_ICSEL0.bit.EISEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL1,attr=IO,locate=0x401
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	EISEL:3;
	} bit;
} ICSEL1STR;

__IO_EXTENDED	  ICSEL1STR	IO_ICSEL1;
#define	_icsel1		(IO_ICSEL1)
#define	ICSEL1		(IO_ICSEL1.byte)
#define	ICSEL1_EISEL  	(IO_ICSEL1.bit.EISEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL2,attr=IO,locate=0x402
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RTSEL0:1;
	} bit;
} ICSEL2STR;

__IO_EXTENDED	  ICSEL2STR	IO_ICSEL2;
#define	_icsel2		(IO_ICSEL2)
#define	ICSEL2		(IO_ICSEL2.byte)
#define	ICSEL2_RTSEL0  	(IO_ICSEL2.bit.RTSEL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL3,attr=IO,locate=0x403
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RTSEL1:1;
	} bit;
} ICSEL3STR;

__IO_EXTENDED	  ICSEL3STR	IO_ICSEL3;
#define	_icsel3		(IO_ICSEL3)
#define	ICSEL3		(IO_ICSEL3.byte)
#define	ICSEL3_RTSEL1  	(IO_ICSEL3.bit.RTSEL1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL4,attr=IO,locate=0x404
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	SG_RX_SEL0:1;
	} bit;
} ICSEL4STR;

__IO_EXTENDED	  ICSEL4STR	IO_ICSEL4;
#define	_icsel4		(IO_ICSEL4)
#define	ICSEL4		(IO_ICSEL4.byte)
#define	ICSEL4_SG_RX_SEL0  	(IO_ICSEL4.bit.SG_RX_SEL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL5,attr=IO,locate=0x405
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	SG_RX_SEL1:1;
	} bit;
} ICSEL5STR;

__IO_EXTENDED	  ICSEL5STR	IO_ICSEL5;
#define	_icsel5		(IO_ICSEL5)
#define	ICSEL5		(IO_ICSEL5.byte)
#define	ICSEL5_SG_RX_SEL1  	(IO_ICSEL5.bit.SG_RX_SEL1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL6,attr=IO,locate=0x406
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	PPGSEL0:3;
	} bit;
} ICSEL6STR;

__IO_EXTENDED	  ICSEL6STR	IO_ICSEL6;
#define	_icsel6		(IO_ICSEL6)
#define	ICSEL6		(IO_ICSEL6.byte)
#define	ICSEL6_PPGSEL0  	(IO_ICSEL6.bit.PPGSEL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL7,attr=IO,locate=0x407
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	PPGSEL1:3;
	} bit;
} ICSEL7STR;

__IO_EXTENDED	  ICSEL7STR	IO_ICSEL7;
#define	_icsel7		(IO_ICSEL7)
#define	ICSEL7		(IO_ICSEL7.byte)
#define	ICSEL7_PPGSEL1  	(IO_ICSEL7.bit.PPGSEL1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL8,attr=IO,locate=0x408
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	PPGSEL2:3;
	} bit;
} ICSEL8STR;

__IO_EXTENDED	  ICSEL8STR	IO_ICSEL8;
#define	_icsel8		(IO_ICSEL8)
#define	ICSEL8		(IO_ICSEL8.byte)
#define	ICSEL8_PPGSEL2  	(IO_ICSEL8.bit.PPGSEL2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL9,attr=IO,locate=0x409
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	PPGSEL3:3;
	} bit;
} ICSEL9STR;

__IO_EXTENDED	  ICSEL9STR	IO_ICSEL9;
#define	_icsel9		(IO_ICSEL9)
#define	ICSEL9		(IO_ICSEL9.byte)
#define	ICSEL9_PPGSEL3  	(IO_ICSEL9.bit.PPGSEL3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL10,attr=IO,locate=0x40A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	PPGSEL4:3;
	} bit;
} ICSEL10STR;

__IO_EXTENDED	  ICSEL10STR	IO_ICSEL10;
#define	_icsel10		(IO_ICSEL10)
#define	ICSEL10		(IO_ICSEL10.byte)
#define	ICSEL10_PPGSEL4  	(IO_ICSEL10.bit.PPGSEL4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL11,attr=IO,locate=0x40B
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	PMSTSEL:2;
	} bit;
} ICSEL11STR;

__IO_EXTENDED	  ICSEL11STR	IO_ICSEL11;
#define	_icsel11		(IO_ICSEL11)
#define	ICSEL11		(IO_ICSEL11.byte)
#define	ICSEL11_PMSTSEL  	(IO_ICSEL11.bit.PMSTSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL12,attr=IO,locate=0x40C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SG_RX_SEL:2;
	} bit;
} ICSEL12STR;

__IO_EXTENDED	  ICSEL12STR	IO_ICSEL12;
#define	_icsel12		(IO_ICSEL12)
#define	ICSEL12		(IO_ICSEL12.byte)
#define	ICSEL12_SG_RX_SEL  	(IO_ICSEL12.bit.SG_RX_SEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL13,attr=IO,locate=0x40D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	ICUSEL0:1;
	} bit;
} ICSEL13STR;

__IO_EXTENDED	  ICSEL13STR	IO_ICSEL13;
#define	_icsel13		(IO_ICSEL13)
#define	ICSEL13		(IO_ICSEL13.byte)
#define	ICSEL13_ICUSEL0  	(IO_ICSEL13.bit.ICUSEL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL14,attr=IO,locate=0x40E
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	ICUSEL1:1;
	} bit;
} ICSEL14STR;

__IO_EXTENDED	  ICSEL14STR	IO_ICSEL14;
#define	_icsel14		(IO_ICSEL14)
#define	ICSEL14		(IO_ICSEL14.byte)
#define	ICSEL14_ICUSEL1  	(IO_ICSEL14.bit.ICUSEL1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL15,attr=IO,locate=0x40F
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	ICUSEL2:1;
	} bit;
} ICSEL15STR;

__IO_EXTENDED	  ICSEL15STR	IO_ICSEL15;
#define	_icsel15		(IO_ICSEL15)
#define	ICSEL15		(IO_ICSEL15.byte)
#define	ICSEL15_ICUSEL2  	(IO_ICSEL15.bit.ICUSEL2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL16,attr=IO,locate=0x410
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	ICUSEL3:1;
	} bit;
} ICSEL16STR;

__IO_EXTENDED	  ICSEL16STR	IO_ICSEL16;
#define	_icsel16		(IO_ICSEL16)
#define	ICSEL16		(IO_ICSEL16.byte)
#define	ICSEL16_ICUSEL3  	(IO_ICSEL16.bit.ICUSEL3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL17,attr=IO,locate=0x411
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	ICUSEL4:1;
	} bit;
} ICSEL17STR;

__IO_EXTENDED	  ICSEL17STR	IO_ICSEL17;
#define	_icsel17		(IO_ICSEL17)
#define	ICSEL17		(IO_ICSEL17.byte)
#define	ICSEL17_ICUSEL4  	(IO_ICSEL17.bit.ICUSEL4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL18,attr=IO,locate=0x412
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	ICUSEL5:1;
	} bit;
} ICSEL18STR;

__IO_EXTENDED	  ICSEL18STR	IO_ICSEL18;
#define	_icsel18		(IO_ICSEL18)
#define	ICSEL18		(IO_ICSEL18.byte)
#define	ICSEL18_ICUSEL5  	(IO_ICSEL18.bit.ICUSEL5)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL19,attr=IO,locate=0x413
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	OCUSEL0:3;
	} bit;
} ICSEL19STR;

__IO_EXTENDED	  ICSEL19STR	IO_ICSEL19;
#define	_icsel19		(IO_ICSEL19)
#define	ICSEL19		(IO_ICSEL19.byte)
#define	ICSEL19_OCUSEL0  	(IO_ICSEL19.bit.OCUSEL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL20,attr=IO,locate=0x414
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	OCUSEL1:3;
	} bit;
} ICSEL20STR;

__IO_EXTENDED	  ICSEL20STR	IO_ICSEL20;
#define	_icsel20		(IO_ICSEL20)
#define	ICSEL20		(IO_ICSEL20.byte)
#define	ICSEL20_OCUSEL1  	(IO_ICSEL20.bit.OCUSEL1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL21,attr=IO,locate=0x415
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	BT_SG_SEL0:2;
	} bit;
} ICSEL21STR;

__IO_EXTENDED	  ICSEL21STR	IO_ICSEL21;
#define	_icsel21		(IO_ICSEL21)
#define	ICSEL21		(IO_ICSEL21.byte)
#define	ICSEL21_BT_SG_SEL0  	(IO_ICSEL21.bit.BT_SG_SEL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICSEL22,attr=IO,locate=0x416
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	BT_SG_SEL1:2;
	} bit;
} ICSEL22STR;

__IO_EXTENDED	  ICSEL22STR	IO_ICSEL22;
#define	_icsel22		(IO_ICSEL22)
#define	ICSEL22		(IO_ICSEL22.byte)
#define	ICSEL22_BT_SG_SEL1  	(IO_ICSEL22.bit.BT_SG_SEL1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR0H,attr=IO,locate=0x418
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RTIR0:1;
			__BYTE	RTIR1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR0HSTR;

__IO_EXTENDED	 const  IRPR0HSTR	IO_IRPR0H;
#define	_irpr0h		(IO_IRPR0H)
#define	IRPR0H		(IO_IRPR0H.byte)
#define	IRPR0H_RTIR0  	(IO_IRPR0H.bit.RTIR0)
#define	IRPR0H_RTIR1  	(IO_IRPR0H.bit.RTIR1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR0L,attr=IO,locate=0x419
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RTIR2:1;
			__BYTE	RTIR3:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR0LSTR;

__IO_EXTENDED	 const  IRPR0LSTR	IO_IRPR0L;
#define	_irpr0l		(IO_IRPR0L)
#define	IRPR0L		(IO_IRPR0L.byte)
#define	IRPR0L_RTIR2  	(IO_IRPR0L.bit.RTIR2)
#define	IRPR0L_RTIR3  	(IO_IRPR0L.bit.RTIR3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR1H,attr=IO,locate=0x41A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RXIR0:1;
			__BYTE	ISIR0:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR1HSTR;

__IO_EXTENDED	 const  IRPR1HSTR	IO_IRPR1H;
#define	_irpr1h		(IO_IRPR1H)
#define	IRPR1H		(IO_IRPR1H.byte)
#define	IRPR1H_RXIR0  	(IO_IRPR1H.bit.RXIR0)
#define	IRPR1H_ISIR0  	(IO_IRPR1H.bit.ISIR0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR1L,attr=IO,locate=0x41B
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RXIR1:1;
			__BYTE	ISIR1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR1LSTR;

__IO_EXTENDED	 const  IRPR1LSTR	IO_IRPR1L;
#define	_irpr1l		(IO_IRPR1L)
#define	IRPR1L		(IO_IRPR1L.byte)
#define	IRPR1L_RXIR1  	(IO_IRPR1L.bit.RXIR1)
#define	IRPR1L_ISIR1  	(IO_IRPR1L.bit.ISIR1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR2H,attr=IO,locate=0x41C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	SGIR0:1;
			__BYTE	RXIR7:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR2HSTR;

__IO_EXTENDED	 const  IRPR2HSTR	IO_IRPR2H;
#define	_irpr2h		(IO_IRPR2H)
#define	IRPR2H		(IO_IRPR2H.byte)
#define	IRPR2H_SGIR0  	(IO_IRPR2H.bit.SGIR0)
#define	IRPR2H_RXIR7  	(IO_IRPR2H.bit.RXIR7)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR2L,attr=IO,locate=0x41D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	SGIR1:1;
			__BYTE	TXIR7:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR2LSTR;

__IO_EXTENDED	 const  IRPR2LSTR	IO_IRPR2L;
#define	_irpr2l		(IO_IRPR2L)
#define	IRPR2L		(IO_IRPR2L.byte)
#define	IRPR2L_SGIR1  	(IO_IRPR2L.bit.SGIR1)
#define	IRPR2L_TXIR7  	(IO_IRPR2L.bit.TXIR7)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR3H,attr=IO,locate=0x41E
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	PPGIR0:1;
			__BYTE	PPGIR1:1;
			__BYTE	PPGIR10:1;
			__BYTE	PPGIR11:1;
			__BYTE	PPGIR20:1;
			__BYTE	PPGIR21:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR3HSTR;

__IO_EXTENDED	 const  IRPR3HSTR	IO_IRPR3H;
#define	_irpr3h		(IO_IRPR3H)
#define	IRPR3H		(IO_IRPR3H.byte)
#define	IRPR3H_PPGIR0  	(IO_IRPR3H.bit.PPGIR0)
#define	IRPR3H_PPGIR1  	(IO_IRPR3H.bit.PPGIR1)
#define	IRPR3H_PPGIR10  	(IO_IRPR3H.bit.PPGIR10)
#define	IRPR3H_PPGIR11  	(IO_IRPR3H.bit.PPGIR11)
#define	IRPR3H_PPGIR20  	(IO_IRPR3H.bit.PPGIR20)
#define	IRPR3H_PPGIR21  	(IO_IRPR3H.bit.PPGIR21)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR3L,attr=IO,locate=0x41F
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	PPGIR2:1;
			__BYTE	PPGIR3:1;
			__BYTE	PPGIR12:1;
			__BYTE	PPGIR13:1;
			__BYTE	PPGIR22:1;
			__BYTE	PPGIR23:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR3LSTR;

__IO_EXTENDED	 const  IRPR3LSTR	IO_IRPR3L;
#define	_irpr3l		(IO_IRPR3L)
#define	IRPR3L		(IO_IRPR3L.byte)
#define	IRPR3L_PPGIR2  	(IO_IRPR3L.bit.PPGIR2)
#define	IRPR3L_PPGIR3  	(IO_IRPR3L.bit.PPGIR3)
#define	IRPR3L_PPGIR12  	(IO_IRPR3L.bit.PPGIR12)
#define	IRPR3L_PPGIR13  	(IO_IRPR3L.bit.PPGIR13)
#define	IRPR3L_PPGIR22  	(IO_IRPR3L.bit.PPGIR22)
#define	IRPR3L_PPGIR23  	(IO_IRPR3L.bit.PPGIR23)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR4H,attr=IO,locate=0x420
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	PPGIR4:1;
			__BYTE	PPGIR5:1;
			__BYTE	PPGIR14:1;
			__BYTE	PPGIR15:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR4HSTR;

__IO_EXTENDED	 const  IRPR4HSTR	IO_IRPR4H;
#define	_irpr4h		(IO_IRPR4H)
#define	IRPR4H		(IO_IRPR4H.byte)
#define	IRPR4H_PPGIR4  	(IO_IRPR4H.bit.PPGIR4)
#define	IRPR4H_PPGIR5  	(IO_IRPR4H.bit.PPGIR5)
#define	IRPR4H_PPGIR14  	(IO_IRPR4H.bit.PPGIR14)
#define	IRPR4H_PPGIR15  	(IO_IRPR4H.bit.PPGIR15)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR4L,attr=IO,locate=0x421
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	PPGIR6:1;
			__BYTE	PPGIR7:1;
			__BYTE	PPGIR16:1;
			__BYTE	PPGIR17:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR4LSTR;

__IO_EXTENDED	 const  IRPR4LSTR	IO_IRPR4L;
#define	_irpr4l		(IO_IRPR4L)
#define	IRPR4L		(IO_IRPR4L.byte)
#define	IRPR4L_PPGIR6  	(IO_IRPR4L.bit.PPGIR6)
#define	IRPR4L_PPGIR7  	(IO_IRPR4L.bit.PPGIR7)
#define	IRPR4L_PPGIR16  	(IO_IRPR4L.bit.PPGIR16)
#define	IRPR4L_PPGIR17  	(IO_IRPR4L.bit.PPGIR17)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR5H,attr=IO,locate=0x422
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	PPGIR8:1;
			__BYTE	PPGIR9:1;
			__BYTE	PPGIR18:1;
			__BYTE	PPGIR19:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR5HSTR;

__IO_EXTENDED	 const  IRPR5HSTR	IO_IRPR5H;
#define	_irpr5h		(IO_IRPR5H)
#define	IRPR5H		(IO_IRPR5H.byte)
#define	IRPR5H_PPGIR8  	(IO_IRPR5H.bit.PPGIR8)
#define	IRPR5H_PPGIR9  	(IO_IRPR5H.bit.PPGIR9)
#define	IRPR5H_PPGIR18  	(IO_IRPR5H.bit.PPGIR18)
#define	IRPR5H_PPGIR19  	(IO_IRPR5H.bit.PPGIR19)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR5L,attr=IO,locate=0x423
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	CANIR2:1;
			__BYTE	UDCIR0:1;
			__BYTE	UDCIR1:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR5LSTR;

__IO_EXTENDED	 const  IRPR5LSTR	IO_IRPR5L;
#define	_irpr5l		(IO_IRPR5L)
#define	IRPR5L		(IO_IRPR5L.byte)
#define	IRPR5L_CANIR2  	(IO_IRPR5L.bit.CANIR2)
#define	IRPR5L_UDCIR0  	(IO_IRPR5L.bit.UDCIR0)
#define	IRPR5L_UDCIR1  	(IO_IRPR5L.bit.UDCIR1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR6H,attr=IO,locate=0x424
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RXIR8:1;
			__BYTE	ISIR8:1;
			__BYTE	HS_RIR:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR6HSTR;

__IO_EXTENDED	 const  IRPR6HSTR	IO_IRPR6H;
#define	_irpr6h		(IO_IRPR6H)
#define	IRPR6H		(IO_IRPR6H.byte)
#define	IRPR6H_RXIR8  	(IO_IRPR6H.bit.RXIR8)
#define	IRPR6H_ISIR8  	(IO_IRPR6H.bit.ISIR8)
#define	IRPR6H_HS_RIR  	(IO_IRPR6H.bit.HS_RIR)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR6L,attr=IO,locate=0x425
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	MTIR:1;
			__BYTE	STIR:1;
			__BYTE	PTIR:1;
			__BYTE	TXIR8:1;
			__BYTE	HS_TIR:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR6LSTR;

__IO_EXTENDED	 const  IRPR6LSTR	IO_IRPR6L;
#define	_irpr6l		(IO_IRPR6L)
#define	IRPR6L		(IO_IRPR6L.byte)
#define	IRPR6L_MTIR  	(IO_IRPR6L.bit.MTIR)
#define	IRPR6L_STIR  	(IO_IRPR6L.bit.STIR)
#define	IRPR6L_PTIR  	(IO_IRPR6L.bit.PTIR)
#define	IRPR6L_TXIR8  	(IO_IRPR6L.bit.TXIR8)
#define	IRPR6L_HS_TIR  	(IO_IRPR6L.bit.HS_TIR)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR7H,attr=IO,locate=0x426
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	SUBIR:1;
			__BYTE	SGIR4:1;
			__BYTE	RXIR9:1;
			__BYTE	ISIR9:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR7HSTR;

__IO_EXTENDED	 const  IRPR7HSTR	IO_IRPR7H;
#define	_irpr7h		(IO_IRPR7H)
#define	IRPR7H		(IO_IRPR7H.byte)
#define	IRPR7H_SUBIR  	(IO_IRPR7H.bit.SUBIR)
#define	IRPR7H_SGIR4  	(IO_IRPR7H.bit.SGIR4)
#define	IRPR7H_RXIR9  	(IO_IRPR7H.bit.RXIR9)
#define	IRPR7H_ISIR9  	(IO_IRPR7H.bit.ISIR9)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR7L,attr=IO,locate=0x427
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	CRIR:1;
			__BYTE	TXIR9:1;
	} bit;
} IRPR7LSTR;

__IO_EXTENDED	 const  IRPR7LSTR	IO_IRPR7L;
#define	_irpr7l		(IO_IRPR7L)
#define	IRPR7L		(IO_IRPR7L.byte)
#define	IRPR7L_CRIR  	(IO_IRPR7L.bit.CRIR)
#define	IRPR7L_TXIR9  	(IO_IRPR7L.bit.TXIR9)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR8H,attr=IO,locate=0x428
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	FRTIR0:1;
			__BYTE	FRTIR2:1;
			__BYTE	FRTIR4:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR8HSTR;

__IO_EXTENDED	 const  IRPR8HSTR	IO_IRPR8H;
#define	_irpr8h		(IO_IRPR8H)
#define	IRPR8H		(IO_IRPR8H.byte)
#define	IRPR8H_FRTIR0  	(IO_IRPR8H.bit.FRTIR0)
#define	IRPR8H_FRTIR2  	(IO_IRPR8H.bit.FRTIR2)
#define	IRPR8H_FRTIR4  	(IO_IRPR8H.bit.FRTIR4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR8L,attr=IO,locate=0x429
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	FRTIR1:1;
			__BYTE	FRTIR3:1;
			__BYTE	FRTIR5:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR8LSTR;

__IO_EXTENDED	 const  IRPR8LSTR	IO_IRPR8L;
#define	_irpr8l		(IO_IRPR8L)
#define	IRPR8L		(IO_IRPR8L.byte)
#define	IRPR8L_FRTIR1  	(IO_IRPR8L.bit.FRTIR1)
#define	IRPR8L_FRTIR3  	(IO_IRPR8L.bit.FRTIR3)
#define	IRPR8L_FRTIR5  	(IO_IRPR8L.bit.FRTIR5)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR9H,attr=IO,locate=0x42A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICUIR0:1;
			__BYTE	ICUIR6:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR9HSTR;

__IO_EXTENDED	 const  IRPR9HSTR	IO_IRPR9H;
#define	_irpr9h		(IO_IRPR9H)
#define	IRPR9H		(IO_IRPR9H.byte)
#define	IRPR9H_ICUIR0  	(IO_IRPR9H.bit.ICUIR0)
#define	IRPR9H_ICUIR6  	(IO_IRPR9H.bit.ICUIR6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR9L,attr=IO,locate=0x42B
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICUIR1:1;
			__BYTE	ICUIR7:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR9LSTR;

__IO_EXTENDED	 const  IRPR9LSTR	IO_IRPR9L;
#define	_irpr9l		(IO_IRPR9L)
#define	IRPR9L		(IO_IRPR9L.byte)
#define	IRPR9L_ICUIR1  	(IO_IRPR9L.bit.ICUIR1)
#define	IRPR9L_ICUIR7  	(IO_IRPR9L.bit.ICUIR7)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR10H,attr=IO,locate=0x42C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICUIR2:1;
			__BYTE	ICUIR8:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR10HSTR;

__IO_EXTENDED	 const  IRPR10HSTR	IO_IRPR10H;
#define	_irpr10h		(IO_IRPR10H)
#define	IRPR10H		(IO_IRPR10H.byte)
#define	IRPR10H_ICUIR2  	(IO_IRPR10H.bit.ICUIR2)
#define	IRPR10H_ICUIR8  	(IO_IRPR10H.bit.ICUIR8)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR10L,attr=IO,locate=0x42D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICUIR3:1;
			__BYTE	ICUIR9:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR10LSTR;

__IO_EXTENDED	 const  IRPR10LSTR	IO_IRPR10L;
#define	_irpr10l		(IO_IRPR10L)
#define	IRPR10L		(IO_IRPR10L.byte)
#define	IRPR10L_ICUIR3  	(IO_IRPR10L.bit.ICUIR3)
#define	IRPR10L_ICUIR9  	(IO_IRPR10L.bit.ICUIR9)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR11H,attr=IO,locate=0x42E
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICUIR4:1;
			__BYTE	ICUIR10:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR11HSTR;

__IO_EXTENDED	 const  IRPR11HSTR	IO_IRPR11H;
#define	_irpr11h		(IO_IRPR11H)
#define	IRPR11H		(IO_IRPR11H.byte)
#define	IRPR11H_ICUIR4  	(IO_IRPR11H.bit.ICUIR4)
#define	IRPR11H_ICUIR10  	(IO_IRPR11H.bit.ICUIR10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR11L,attr=IO,locate=0x42F
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICUIR5:1;
			__BYTE	ICUIR11:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR11LSTR;

__IO_EXTENDED	 const  IRPR11LSTR	IO_IRPR11L;
#define	_irpr11l		(IO_IRPR11L)
#define	IRPR11L		(IO_IRPR11L.byte)
#define	IRPR11L_ICUIR5  	(IO_IRPR11L.bit.ICUIR5)
#define	IRPR11L_ICUIR11  	(IO_IRPR11L.bit.ICUIR11)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR12H,attr=IO,locate=0x430
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	OCUIR0:1;
			__BYTE	OCUIR1:1;
			__BYTE	OCUIR6:1;
			__BYTE	OCUIR7:1;
			__BYTE	OCUIR10:1;
			__BYTE	OCUIR11:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR12HSTR;

__IO_EXTENDED	 const  IRPR12HSTR	IO_IRPR12H;
#define	_irpr12h		(IO_IRPR12H)
#define	IRPR12H		(IO_IRPR12H.byte)
#define	IRPR12H_OCUIR0  	(IO_IRPR12H.bit.OCUIR0)
#define	IRPR12H_OCUIR1  	(IO_IRPR12H.bit.OCUIR1)
#define	IRPR12H_OCUIR6  	(IO_IRPR12H.bit.OCUIR6)
#define	IRPR12H_OCUIR7  	(IO_IRPR12H.bit.OCUIR7)
#define	IRPR12H_OCUIR10  	(IO_IRPR12H.bit.OCUIR10)
#define	IRPR12H_OCUIR11  	(IO_IRPR12H.bit.OCUIR11)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR12L,attr=IO,locate=0x431
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	OCUIR2:1;
			__BYTE	OCUIR3:1;
			__BYTE	OCUIR4:1;
			__BYTE	OCUIR5:1;
			__BYTE	OCUIR8:1;
			__BYTE	OCUIR9:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR12LSTR;

__IO_EXTENDED	 const  IRPR12LSTR	IO_IRPR12L;
#define	_irpr12l		(IO_IRPR12L)
#define	IRPR12L		(IO_IRPR12L.byte)
#define	IRPR12L_OCUIR2  	(IO_IRPR12L.bit.OCUIR2)
#define	IRPR12L_OCUIR3  	(IO_IRPR12L.bit.OCUIR3)
#define	IRPR12L_OCUIR4  	(IO_IRPR12L.bit.OCUIR4)
#define	IRPR12L_OCUIR5  	(IO_IRPR12L.bit.OCUIR5)
#define	IRPR12L_OCUIR8  	(IO_IRPR12L.bit.OCUIR8)
#define	IRPR12L_OCUIR9  	(IO_IRPR12L.bit.OCUIR9)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR13H,attr=IO,locate=0x432
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	BT0IR0:1;
			__BYTE	BT0IR1:1;
			__BYTE	SGIR2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR13HSTR;

__IO_EXTENDED	 const  IRPR13HSTR	IO_IRPR13H;
#define	_irpr13h		(IO_IRPR13H)
#define	IRPR13H		(IO_IRPR13H.byte)
#define	IRPR13H_BT0IR0  	(IO_IRPR13H.bit.BT0IR0)
#define	IRPR13H_BT0IR1  	(IO_IRPR13H.bit.BT0IR1)
#define	IRPR13H_SGIR2  	(IO_IRPR13H.bit.SGIR2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR13L,attr=IO,locate=0x433
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	BT1IR0:1;
			__BYTE	BT1IR1:1;
			__BYTE	SGIR3:1;
			__BYTE	XB_ECC_SE:1;
			__BYTE	BR_ECC_SE:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR13LSTR;

__IO_EXTENDED	 const  IRPR13LSTR	IO_IRPR13L;
#define	_irpr13l		(IO_IRPR13L)
#define	IRPR13L		(IO_IRPR13L.byte)
#define	IRPR13L_BT1IR0  	(IO_IRPR13L.bit.BT1IR0)
#define	IRPR13L_BT1IR1  	(IO_IRPR13L.bit.BT1IR1)
#define	IRPR13L_SGIR3  	(IO_IRPR13L.bit.SGIR3)
#define	IRPR13L_XB_ECC_SE  	(IO_IRPR13L.bit.XB_ECC_SE)
#define	IRPR13L_BR_ECC_SE  	(IO_IRPR13L.bit.BR_ECC_SE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR14H,attr=IO,locate=0x434
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	DMAC0IR:1;
			__BYTE	DMAC1IR:1;
			__BYTE	DMAC2IR:1;
			__BYTE	DMAC3IR:1;
			__BYTE	DMAC4IR:1;
			__BYTE	DMAC5IR:1;
			__BYTE	DMAC6IR:1;
			__BYTE	DMAC7IR:1;
	} bit;
} IRPR14HSTR;

__IO_EXTENDED	 const  IRPR14HSTR	IO_IRPR14H;
#define	_irpr14h		(IO_IRPR14H)
#define	IRPR14H		(IO_IRPR14H.byte)
#define	IRPR14H_DMAC0IR  	(IO_IRPR14H.bit.DMAC0IR)
#define	IRPR14H_DMAC1IR  	(IO_IRPR14H.bit.DMAC1IR)
#define	IRPR14H_DMAC2IR  	(IO_IRPR14H.bit.DMAC2IR)
#define	IRPR14H_DMAC3IR  	(IO_IRPR14H.bit.DMAC3IR)
#define	IRPR14H_DMAC4IR  	(IO_IRPR14H.bit.DMAC4IR)
#define	IRPR14H_DMAC5IR  	(IO_IRPR14H.bit.DMAC5IR)
#define	IRPR14H_DMAC6IR  	(IO_IRPR14H.bit.DMAC6IR)
#define	IRPR14H_DMAC7IR  	(IO_IRPR14H.bit.DMAC7IR)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR14L,attr=IO,locate=0x435
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	DMAC8IR:1;
			__BYTE	DMAC9IR:1;
			__BYTE	DMAC10IR:1;
			__BYTE	DMAC11IR:1;
			__BYTE	DMAC12IR:1;
			__BYTE	DMAC13IR:1;
			__BYTE	DMAC14IR:1;
			__BYTE	DMAC15IR:1;
	} bit;
} IRPR14LSTR;

__IO_EXTENDED	 const  IRPR14LSTR	IO_IRPR14L;
#define	_irpr14l		(IO_IRPR14L)
#define	IRPR14L		(IO_IRPR14L.byte)
#define	IRPR14L_DMAC8IR  	(IO_IRPR14L.bit.DMAC8IR)
#define	IRPR14L_DMAC9IR  	(IO_IRPR14L.bit.DMAC9IR)
#define	IRPR14L_DMAC10IR  	(IO_IRPR14L.bit.DMAC10IR)
#define	IRPR14L_DMAC11IR  	(IO_IRPR14L.bit.DMAC11IR)
#define	IRPR14L_DMAC12IR  	(IO_IRPR14L.bit.DMAC12IR)
#define	IRPR14L_DMAC13IR  	(IO_IRPR14L.bit.DMAC13IR)
#define	IRPR14L_DMAC14IR  	(IO_IRPR14L.bit.DMAC14IR)
#define	IRPR14L_DMAC15IR  	(IO_IRPR14L.bit.DMAC15IR)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IRPR15H,attr=IO,locate=0x436
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	EXTNMI:1;
			__BYTE	XB_ECC_DE:1;
			__BYTE	BR_ECC_DE:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} IRPR15HSTR;

__IO_EXTENDED	 const  IRPR15HSTR	IO_IRPR15H;
#define	_irpr15h		(IO_IRPR15H)
#define	IRPR15H		(IO_IRPR15H.byte)
#define	IRPR15H_EXTNMI  	(IO_IRPR15H.bit.EXTNMI)
#define	IRPR15H_XB_ECC_DE  	(IO_IRPR15H.bit.XB_ECC_DE)
#define	IRPR15H_BR_ECC_DE  	(IO_IRPR15H.bit.BR_ECC_DE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICR,attr=IO,locate=0x440
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	IL:5;
	} bit;
} ICRSTR;

__IO_EXTENDED	ICRSTR	IO_ICR[CONSTANT_48];

#define	ICR0	(IO_ICR[CONSTANT_0].byte)
#define	ICR0_IL  	(IO_ICR[CONSTANT_0].bit.IL)
#define	ICR1	(IO_ICR[CONSTANT_1].byte)
#define	ICR1_IL  	(IO_ICR[CONSTANT_1].bit.IL)
#define	ICR2	(IO_ICR[CONSTANT_2].byte)
#define	ICR2_IL  	(IO_ICR[CONSTANT_2].bit.IL)
#define	ICR3	(IO_ICR[CONSTANT_3].byte)
#define	ICR3_IL  	(IO_ICR[CONSTANT_3].bit.IL)
#define	ICR4	(IO_ICR[CONSTANT_4].byte)
#define	ICR4_IL  	(IO_ICR[CONSTANT_4].bit.IL)
#define	ICR5	(IO_ICR[CONSTANT_5].byte)
#define	ICR5_IL  	(IO_ICR[CONSTANT_5].bit.IL)
#define	ICR6	(IO_ICR[CONSTANT_6].byte)
#define	ICR6_IL  	(IO_ICR[CONSTANT_6].bit.IL)
#define	ICR7	(IO_ICR[CONSTANT_7].byte)
#define	ICR7_IL  	(IO_ICR[CONSTANT_7].bit.IL)
#define	ICR8	(IO_ICR[CONSTANT_8].byte)
#define	ICR8_IL  	(IO_ICR[CONSTANT_8].bit.IL)
#define	ICR9	(IO_ICR[CONSTANT_9].byte)
#define	ICR9_IL  	(IO_ICR[CONSTANT_9].bit.IL)
#define	ICR10	(IO_ICR[CONSTANT_10].byte)
#define	ICR10_IL  	(IO_ICR[CONSTANT_10].bit.IL)
#define	ICR11	(IO_ICR[CONSTANT_11].byte)
#define	ICR11_IL  	(IO_ICR[CONSTANT_11].bit.IL)
#define	ICR12	(IO_ICR[CONSTANT_12].byte)
#define	ICR12_IL  	(IO_ICR[CONSTANT_12].bit.IL)
#define	ICR13	(IO_ICR[CONSTANT_13].byte)
#define	ICR13_IL  	(IO_ICR[CONSTANT_13].bit.IL)
#define	ICR14	(IO_ICR[CONSTANT_14].byte)
#define	ICR14_IL  	(IO_ICR[CONSTANT_14].bit.IL)
#define	ICR15	(IO_ICR[CONSTANT_15].byte)
#define	ICR15_IL  	(IO_ICR[CONSTANT_15].bit.IL)
#define	ICR16	(IO_ICR[CONSTANT_16].byte)
#define	ICR16_IL  	(IO_ICR[CONSTANT_16].bit.IL)
#define	ICR17	(IO_ICR[CONSTANT_17].byte)
#define	ICR17_IL  	(IO_ICR[CONSTANT_17].bit.IL)
#define	ICR18	(IO_ICR[CONSTANT_18].byte)
#define	ICR18_IL  	(IO_ICR[CONSTANT_18].bit.IL)
#define	ICR19	(IO_ICR[CONSTANT_19].byte)
#define	ICR19_IL  	(IO_ICR[CONSTANT_19].bit.IL)
#define	ICR20	(IO_ICR[CONSTANT_20].byte)
#define	ICR20_IL  	(IO_ICR[CONSTANT_20].bit.IL)
#define	ICR21	(IO_ICR[CONSTANT_21].byte)
#define	ICR21_IL  	(IO_ICR[CONSTANT_21].bit.IL)
#define	ICR22	(IO_ICR[CONSTANT_22].byte)
#define	ICR22_IL  	(IO_ICR[CONSTANT_22].bit.IL)
#define	ICR23	(IO_ICR[CONSTANT_23].byte)
#define	ICR23_IL  	(IO_ICR[CONSTANT_23].bit.IL)
#define	ICR24	(IO_ICR[CONSTANT_24].byte)
#define	ICR24_IL  	(IO_ICR[CONSTANT_24].bit.IL)
#define	ICR25	(IO_ICR[CONSTANT_25].byte)
#define	ICR25_IL  	(IO_ICR[CONSTANT_25].bit.IL)
#define	ICR26	(IO_ICR[CONSTANT_26].byte)
#define	ICR26_IL  	(IO_ICR[CONSTANT_26].bit.IL)
#define	ICR27	(IO_ICR[CONSTANT_27].byte)
#define	ICR27_IL  	(IO_ICR[CONSTANT_27].bit.IL)
#define	ICR28	(IO_ICR[CONSTANT_28].byte)
#define	ICR28_IL  	(IO_ICR[CONSTANT_28].bit.IL)
#define	ICR29	(IO_ICR[CONSTANT_29].byte)
#define	ICR29_IL  	(IO_ICR[CONSTANT_29].bit.IL)
#define	ICR30	(IO_ICR[CONSTANT_30].byte)
#define	ICR30_IL  	(IO_ICR[CONSTANT_30].bit.IL)
#define	ICR31	(IO_ICR[CONSTANT_31].byte)
#define	ICR31_IL  	(IO_ICR[CONSTANT_31].bit.IL)
#define	ICR32	(IO_ICR[CONSTANT_32].byte)
#define	ICR32_IL  	(IO_ICR[CONSTANT_32].bit.IL)
#define	ICR33	(IO_ICR[CONSTANT_33].byte)
#define	ICR33_IL  	(IO_ICR[CONSTANT_33].bit.IL)
#define	ICR34	(IO_ICR[CONSTANT_34].byte)
#define	ICR34_IL  	(IO_ICR[CONSTANT_34].bit.IL)
#define	ICR35	(IO_ICR[CONSTANT_35].byte)
#define	ICR35_IL  	(IO_ICR[CONSTANT_35].bit.IL)
#define	ICR36	(IO_ICR[CONSTANT_36].byte)
#define	ICR36_IL  	(IO_ICR[CONSTANT_36].bit.IL)
#define	ICR37	(IO_ICR[CONSTANT_37].byte)
#define	ICR37_IL  	(IO_ICR[CONSTANT_37].bit.IL)
#define	ICR38	(IO_ICR[CONSTANT_38].byte)
#define	ICR38_IL  	(IO_ICR[CONSTANT_38].bit.IL)
#define	ICR39	(IO_ICR[CONSTANT_39].byte)
#define	ICR39_IL  	(IO_ICR[CONSTANT_39].bit.IL)
#define	ICR40	(IO_ICR[CONSTANT_40].byte)
#define	ICR40_IL  	(IO_ICR[CONSTANT_40].bit.IL)
#define	ICR41	(IO_ICR[CONSTANT_41].byte)
#define	ICR41_IL  	(IO_ICR[CONSTANT_41].bit.IL)
#define	ICR42	(IO_ICR[CONSTANT_42].byte)
#define	ICR42_IL  	(IO_ICR[CONSTANT_42].bit.IL)
#define	ICR43	(IO_ICR[CONSTANT_43].byte)
#define	ICR43_IL  	(IO_ICR[CONSTANT_43].bit.IL)
#define	ICR44	(IO_ICR[CONSTANT_44].byte)
#define	ICR44_IL  	(IO_ICR[CONSTANT_44].bit.IL)
#define	ICR45	(IO_ICR[CONSTANT_45].byte)
#define	ICR45_IL  	(IO_ICR[CONSTANT_45].bit.IL)
#define	ICR46	(IO_ICR[CONSTANT_46].byte)
#define	ICR46_IL  	(IO_ICR[CONSTANT_46].bit.IL)
#define	ICR47	(IO_ICR[CONSTANT_47].byte)
#define	ICR47_IL  	(IO_ICR[CONSTANT_47].bit.IL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_RSTRR,attr=IO,locate=0x480
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	IRRST:1;
			__BYTE	ERST:1;
			__BYTE	WDG1:1;
			__BYTE	WDG0:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SCRT:1;
			__BYTE	SRST:1;
	} bit;
} RSTRRSTR;

__IO_EXTENDED	 const  RSTRRSTR	IO_RSTRR;
#define	_rstrr		(IO_RSTRR)
#define	RSTRR		(IO_RSTRR.byte)
#define	RSTRR_IRRST  	(IO_RSTRR.bit.IRRST)
#define	RSTRR_ERST  	(IO_RSTRR.bit.ERST)
#define	RSTRR_WDG1  	(IO_RSTRR.bit.WDG1)
#define	RSTRR_WDG0  	(IO_RSTRR.bit.WDG0)
#define	RSTRR_SCRT  	(IO_RSTRR.bit.SCRT)
#define	RSTRR_SRST  	(IO_RSTRR.bit.SRST)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_RSTCR,attr=IO,locate=0x481
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RDLY:3;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	SRST:1;
	} bit;
} RSTCRSTR;

__IO_EXTENDED	  RSTCRSTR	IO_RSTCR;
#define	_rstcr		(IO_RSTCR)
#define	RSTCR		(IO_RSTCR.byte)
#define	RSTCR_RDLY  	(IO_RSTCR.bit.RDLY)
#define	RSTCR_SRST  	(IO_RSTCR.bit.SRST)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_STBCR,attr=IO,locate=0x482
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	STOP:1;
			__BYTE	TIMER:1;
			__BYTE	SLEEP:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SLVL:2;
	} bit;
} STBCRSTR;

__IO_EXTENDED	  STBCRSTR	IO_STBCR;
#define	_stbcr		(IO_STBCR)
#define	STBCR		(IO_STBCR.byte)
#define	STBCR_STOP  	(IO_STBCR.bit.STOP)
#define	STBCR_TIMER  	(IO_STBCR.bit.TIMER)
#define	STBCR_SLEEP  	(IO_STBCR.bit.SLEEP)
#define	STBCR_SLVL  	(IO_STBCR.bit.SLVL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DIVR0,attr=IO,locate=0x488
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	DIVB:3;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
	} bit;
} DIVR0STR;

__IO_EXTENDED	  DIVR0STR	IO_DIVR0;
#define	_divr0		(IO_DIVR0)
#define	DIVR0		(IO_DIVR0.byte)
#define	DIVR0_DIVB  	(IO_DIVR0.bit.DIVB)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DIVR1,attr=IO,locate=0x489
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	TSTP:1;
			__BYTE	DIVT:3;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
	} bit;
} DIVR1STR;

__IO_EXTENDED	  DIVR1STR	IO_DIVR1;
#define	_divr1		(IO_DIVR1)
#define	DIVR1		(IO_DIVR1.byte)
#define	DIVR1_TSTP  	(IO_DIVR1.bit.TSTP)
#define	DIVR1_DIVT  	(IO_DIVR1.bit.DIVT)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DIVR2,attr=IO,locate=0x48A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	DIVP:4;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
	} bit;
} DIVR2STR;

__IO_EXTENDED	  DIVR2STR	IO_DIVR2;
#define	_divr2		(IO_DIVR2)
#define	DIVR2		(IO_DIVR2.byte)
#define	DIVR2_DIVP  	(IO_DIVR2.bit.DIVP)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR00,attr=IO,locate=0x490
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR00STR;

__IO_EXTENDED	  IORR00STR	IO_IORR00;
#define	_iorr00		(IO_IORR00)
#define	IORR00		(IO_IORR00.byte)
#define	IORR00_IOE  	(IO_IORR00.bit.IOE)
#define	IORR00_IOS  	(IO_IORR00.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR01,attr=IO,locate=0x491
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR01STR;

__IO_EXTENDED	  IORR01STR	IO_IORR01;
#define	_iorr01		(IO_IORR01)
#define	IORR01		(IO_IORR01.byte)
#define	IORR01_IOE  	(IO_IORR01.bit.IOE)
#define	IORR01_IOS  	(IO_IORR01.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR02,attr=IO,locate=0x492
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR02STR;

__IO_EXTENDED	  IORR02STR	IO_IORR02;
#define	_iorr02		(IO_IORR02)
#define	IORR02		(IO_IORR02.byte)
#define	IORR02_IOE  	(IO_IORR02.bit.IOE)
#define	IORR02_IOS  	(IO_IORR02.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR03,attr=IO,locate=0x493
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR03STR;

__IO_EXTENDED	  IORR03STR	IO_IORR03;
#define	_iorr03		(IO_IORR03)
#define	IORR03		(IO_IORR03.byte)
#define	IORR03_IOE  	(IO_IORR03.bit.IOE)
#define	IORR03_IOS  	(IO_IORR03.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR04,attr=IO,locate=0x494
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR04STR;

__IO_EXTENDED	  IORR04STR	IO_IORR04;
#define	_iorr04		(IO_IORR04)
#define	IORR04		(IO_IORR04.byte)
#define	IORR04_IOE  	(IO_IORR04.bit.IOE)
#define	IORR04_IOS  	(IO_IORR04.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR05,attr=IO,locate=0x495
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR05STR;

__IO_EXTENDED	  IORR05STR	IO_IORR05;
#define	_iorr05		(IO_IORR05)
#define	IORR05		(IO_IORR05.byte)
#define	IORR05_IOE  	(IO_IORR05.bit.IOE)
#define	IORR05_IOS  	(IO_IORR05.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR06,attr=IO,locate=0x496
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR06STR;

__IO_EXTENDED	  IORR06STR	IO_IORR06;
#define	_iorr06		(IO_IORR06)
#define	IORR06		(IO_IORR06.byte)
#define	IORR06_IOE  	(IO_IORR06.bit.IOE)
#define	IORR06_IOS  	(IO_IORR06.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR07,attr=IO,locate=0x497
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR07STR;

__IO_EXTENDED	  IORR07STR	IO_IORR07;
#define	_iorr07		(IO_IORR07)
#define	IORR07		(IO_IORR07.byte)
#define	IORR07_IOE  	(IO_IORR07.bit.IOE)
#define	IORR07_IOS  	(IO_IORR07.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR08,attr=IO,locate=0x498
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR08STR;

__IO_EXTENDED	  IORR08STR	IO_IORR08;
#define	_iorr08		(IO_IORR08)
#define	IORR08		(IO_IORR08.byte)
#define	IORR08_IOE  	(IO_IORR08.bit.IOE)
#define	IORR08_IOS  	(IO_IORR08.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR09,attr=IO,locate=0x499
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR09STR;

__IO_EXTENDED	  IORR09STR	IO_IORR09;
#define	_iorr09		(IO_IORR09)
#define	IORR09		(IO_IORR09.byte)
#define	IORR09_IOE  	(IO_IORR09.bit.IOE)
#define	IORR09_IOS  	(IO_IORR09.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR10,attr=IO,locate=0x49A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR10STR;

__IO_EXTENDED	  IORR10STR	IO_IORR10;
#define	_iorr10		(IO_IORR10)
#define	IORR10		(IO_IORR10.byte)
#define	IORR10_IOE  	(IO_IORR10.bit.IOE)
#define	IORR10_IOS  	(IO_IORR10.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR11,attr=IO,locate=0x49B
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR11STR;

__IO_EXTENDED	  IORR11STR	IO_IORR11;
#define	_iorr11		(IO_IORR11)
#define	IORR11		(IO_IORR11.byte)
#define	IORR11_IOE  	(IO_IORR11.bit.IOE)
#define	IORR11_IOS  	(IO_IORR11.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR12,attr=IO,locate=0x49C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR12STR;

__IO_EXTENDED	  IORR12STR	IO_IORR12;
#define	_iorr12		(IO_IORR12)
#define	IORR12		(IO_IORR12.byte)
#define	IORR12_IOE  	(IO_IORR12.bit.IOE)
#define	IORR12_IOS  	(IO_IORR12.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR13,attr=IO,locate=0x49D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR13STR;

__IO_EXTENDED	  IORR13STR	IO_IORR13;
#define	_iorr13		(IO_IORR13)
#define	IORR13		(IO_IORR13.byte)
#define	IORR13_IOE  	(IO_IORR13.bit.IOE)
#define	IORR13_IOS  	(IO_IORR13.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR14,attr=IO,locate=0x49E
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR14STR;

__IO_EXTENDED	  IORR14STR	IO_IORR14;
#define	_iorr14		(IO_IORR14)
#define	IORR14		(IO_IORR14.byte)
#define	IORR14_IOE  	(IO_IORR14.bit.IOE)
#define	IORR14_IOS  	(IO_IORR14.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IORR15,attr=IO,locate=0x49F
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	IOE:1;
			__BYTE	IOS:6;
	} bit;
} IORR15STR;

__IO_EXTENDED	  IORR15STR	IO_IORR15;
#define	_iorr15		(IO_IORR15)
#define	IORR15		(IO_IORR15.byte)
#define	IORR15_IOE  	(IO_IORR15.bit.IOE)
#define	IORR15_IOS  	(IO_IORR15.bit.IOS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CANPRE,attr=IO,locate=0x4A4
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	CANPRE:4;
	} bit;
} CANPRESTR;

__IO_EXTENDED	  CANPRESTR	IO_CANPRE;
#define	_canpre		(IO_CANPRE)
#define	CANPRE_		(IO_CANPRE.byte)
#define	CANPRE_CANPRE  	(IO_CANPRE.bit.CANPRE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CUCR0,attr=IO,locate=0x4B8
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	CUCR0H;
		__BYTE	CUCR0L;
	} byte;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	STRT:1;
			__HWORD	RESV12:1;
			__HWORD	RESV13:1;
			__HWORD	INT:1;
			__HWORD	INTEN:1;
	} bit;
} CUCR0STR;

__IO_EXTENDED	  CUCR0STR	IO_CUCR0;
#define	_cucr0		(IO_CUCR0)
#define	CUCR0		(IO_CUCR0.hword)
#define	CUCR0_CUCR0H	(IO_CUCR0.byte.CUCR0H)
#define	CUCR0_CUCR0L	(IO_CUCR0.byte.CUCR0L)
#define	CUCR0_STRT  	(IO_CUCR0.bit.STRT)
#define	CUCR0_INT  	(IO_CUCR0.bit.INT)
#define	CUCR0_INTEN  	(IO_CUCR0.bit.INTEN)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CUTD0,attr=IO,locate=0x4BA
#endif

__IO_EXTENDED	union {
	__HWORD	hword;
	struct {
		__BYTE	CUTD0H;
		__BYTE	CUTD0L;
	} byte;
} IO_CUTD0;

#define	CUTD0_		(IO_CUTD0.hword)
#define	CUTD0_CUTD0H	(IO_CUTD0.byte.CUTD0H)
#define	CUTD0_CUTD0L	(IO_CUTD0.byte.CUTD0L)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CUTR0,attr=IO,locate=0x4BC
#endif

__IO_EXTENDED	const	union {
	__WORD	word;
	struct {
		__HWORD	CUTR0H;
		__HWORD	CUTR0L;
	} hword;
	struct {
		__BYTE	CUTR0HH;
		__BYTE	CUTR0HL;
		__BYTE	CUTR0LH;
		__BYTE	CUTR0LL;
	} byte;
} IO_CUTR0;

#define	CUTR0_		(IO_CUTR0.word)
#define	CUTR0_CUTR0H	(IO_CUTR0.hword.CUTR0H)
#define	CUTR0_CUTR0L	(IO_CUTR0.hword.CUTR0L)
#define	CUTR0_CUTR0HH	(IO_CUTR0.byte.CUTR0HH)
#define	CUTR0_CUTR0HL	(IO_CUTR0.byte.CUTR0HL)
#define	CUTR0_CUTR0LH	(IO_CUTR0.byte.CUTR0LH)
#define	CUTR0_CUTR0LL	(IO_CUTR0.byte.CUTR0LL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CUCR1,attr=IO,locate=0x4C4
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	CUCR1H;
		__BYTE	CUCR1L;
	} byte;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	STRT:1;
			__HWORD	RESV12:1;
			__HWORD	RESV13:1;
			__HWORD	INT:1;
			__HWORD	INTEN:1;
	} bit;
} CUCR1STR;

__IO_EXTENDED	  CUCR1STR	IO_CUCR1;
#define	_cucr1		(IO_CUCR1)
#define	CUCR1		(IO_CUCR1.hword)
#define	CUCR1_CUCR1H	(IO_CUCR1.byte.CUCR1H)
#define	CUCR1_CUCR1L	(IO_CUCR1.byte.CUCR1L)
#define	CUCR1_STRT  	(IO_CUCR1.bit.STRT)
#define	CUCR1_INT  	(IO_CUCR1.bit.INT)
#define	CUCR1_INTEN  	(IO_CUCR1.bit.INTEN)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CUTD1,attr=IO,locate=0x4C6
#endif

__IO_EXTENDED	union {
	__HWORD	hword;
	struct {
		__BYTE	CUTD1H;
		__BYTE	CUTD1L;
	} byte;
} IO_CUTD1;

#define	CUTD1_		(IO_CUTD1.hword)
#define	CUTD1_CUTD1H	(IO_CUTD1.byte.CUTD1H)
#define	CUTD1_CUTD1L	(IO_CUTD1.byte.CUTD1L)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CUTR1,attr=IO,locate=0x4C8
#endif

__IO_EXTENDED	const	union {
	__WORD	word;
	struct {
		__BYTE	CUTR1H;
		__BYTE	CUTR1L;
	} hword;
	struct {
		__BYTE	CUTR1HH;
		__BYTE	CUTR1HL;
		__BYTE	CUTR1LH;
		__BYTE	CUTR1LL;
	} byte;
} IO_CUTR1;

#define	CUTR1_		(IO_CUTR1.word)
#define	CUTR1_CUTR1H	(IO_CUTR1.hword.CUTR1H)
#define	CUTR1_CUTR1L	(IO_CUTR1.hword.CUTR1L)
#define	CUTR1_CUTR1HH	(IO_CUTR1.byte.CUTR1HH)
#define	CUTR1_CUTR1HL	(IO_CUTR1.byte.CUTR1HL)
#define	CUTR1_CUTR1LH	(IO_CUTR1.byte.CUTR1LH)
#define	CUTR1_CUTR1LL	(IO_CUTR1.byte.CUTR1LL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CRTR,attr=IO,locate=0x4CC
#endif

__IO_EXTENDED	__BYTE	IO_CRTR;
#define	_crtr		(IO_CRTR)
#define	CRTR	(_crtr)

#ifdef __IO_DEFINE
#pragma segment DATA=IO_UART8,attr=IO,locate=0x4E0
#endif

typedef union {

struct {
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
	} SCR8;
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
	} SMR8;
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
	} SSR8;
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
	} ESCR8;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR8L;
		} byte;
		struct {
			struct {
				__HWORD	RESV1:7;
				__HWORD	D:9;
			} bit;
		} bit9;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	D:7;
			} bit;
		} bit7;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	D:6;
			} bit;
		} bit6;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	RESV3:1;
				__BYTE	D:5;
			} bit;
		} bit5;
	} RDR8;
	union {
		__HWORD	hword;
		struct {
			__HWORD	EXT:1;
			__HWORD	BGR:15;
		} bit;
	} BGR8;
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
	} FCR18;
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
	} FCR08;
	__HWORD	FBYTE8;
} UART8;

struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	UPCL:1;
			__BYTE	MS:1;
			__BYTE	SPI:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
			__BYTE	TBIE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} SCR8;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:3;
			__BYTE	RESV11:1;
			__BYTE	SCINV:1;
			__BYTE	BDS:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} SMR8;
	union {
		__BYTE	byte;
		struct {
			__BYTE	REC:1;
			__BYTE	RESV21:1;
			__BYTE	RESV22:1;
			__BYTE	RESV23:1;
			__BYTE	ORE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	TBI:1;
		} bit;
	} SSR8;
	union {
		__BYTE	byte;
		struct {
			__BYTE	SOP:1;
			__BYTE	RESV31:1;
			__BYTE	RESV32:1;
			__BYTE	WT:2;
			__BYTE	L:3;
		} bit;
	} ESCR8;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR8L;
		} byte;
		struct {
			struct {
				__HWORD	RESV1:7;
				__HWORD	D:9;
			} bit;
		} bit9;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	D:7;
			} bit;
		} bit7;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	D:6;
			} bit;
		} bit6;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	RESV3:1;
				__BYTE	D:5;
			} bit;
		} bit5;
	} RDR8;
	__HWORD	BGR8;
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
	} FCR18;
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
	} FCR08;
	__HWORD	FBYTE8;
} CSIO8;

struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	UPCL:1;
			__BYTE	MS:1;
			__BYTE	LBR:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
			__BYTE	TBIE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} SCR8;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:3;
			__BYTE	RESV11:1;
			__BYTE	SBL:1;
			__BYTE	RESV13:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} SMR8;
	union {
		__BYTE	byte;
		struct {
			__BYTE	REC:1;
			__BYTE	RESV21:1;
			__BYTE	LBD:1;
			__BYTE	FRE:1;
			__BYTE	ORE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	TBI:1;
		} bit;
	} SSR8;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV30:1;
			__BYTE	ESBL:1;
			__BYTE	RESV32:1;
			__BYTE	LBIE:1;
			__BYTE	LBL:2;
			__BYTE	DEL:2;
		} bit;
	} ESCR8;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR8L;
		} byte;
	} RDR8;
	union {
		__HWORD	hword;
		struct {
			__HWORD	EXT:1;
			__HWORD	BGR:15;
		} bit;
	} BGR8;
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
	} FCR18;
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
	} FCR08;
	__HWORD	FBYTE8;
} LINUART8;

struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	MSS:1;
			__BYTE	ACT:1;
			__BYTE	ACKE:1;
			__BYTE	WSEL:1;
			__BYTE	CNDE:1;
			__BYTE	INTE:1;
			__BYTE	BER:1;
			__BYTE	INT:1;
		} bit;
	} IBCR8;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:3;
			__BYTE	RESV11:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
			__BYTE	RESV1:1;
			__BYTE	RESV0:1;
		} bit;
	} SMR8;
	union {
		__BYTE	byte;
		struct {
			__BYTE	REC:1;
			__BYTE	TSET:1;
			__BYTE	DMA:1;
			__BYTE	TBIE:1;
			__BYTE	ORE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	TBI:1;
		} bit;
	} SSR8;
	union {
		__BYTE	byte;
		struct {
			__BYTE	FBT:1;
			__BYTE	RACK:1;
			__BYTE	RSA:1;
			__BYTE	TRX:1;
			__BYTE	AL:1;
			__BYTE	RSC:1;
			__BYTE	SPC:1;
			__BYTE	BB:1;
		} bit;
	} IBSR8;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR8L;
		} byte;
	} RDR8;
	__HWORD	BGR8;
	union {
		__BYTE	byte;
		struct {
			__BYTE	EN:1;
			__BYTE	SM:7;
		} bit;
	} ISMK8;
	union {
		__BYTE	byte;
		struct {
			__BYTE	SAEN:1;
			__BYTE	SA:7;
		} bit;
	} ISBA8;
	__HWORD	IO_RESV1;
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
	} FCR18;
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
	} FCR08;
	__HWORD	FBYTE8;
} I2C8;

} UART8STR;

__IO_EXTENDED	UART8STR	IO_UART8;

#define	UART8_UART8_SCR8			(IO_UART8.UART8.SCR8.byte)
#define	UART8_UART8_SCR8_UPCL    	(IO_UART8.UART8.SCR8.bit.UPCL)
#define	UART8_UART8_SCR8_RIE    	(IO_UART8.UART8.SCR8.bit.RIE)
#define	UART8_UART8_SCR8_TIE    	(IO_UART8.UART8.SCR8.bit.TIE)
#define	UART8_UART8_SCR8_TBIE    	(IO_UART8.UART8.SCR8.bit.TBIE)
#define	UART8_UART8_SCR8_RXE    	(IO_UART8.UART8.SCR8.bit.RXE)
#define	UART8_UART8_SCR8_TXE    	(IO_UART8.UART8.SCR8.bit.TXE)
#define	UART8_UART8_SMR8			(IO_UART8.UART8.SMR8.byte)
#define	UART8_UART8_SMR8_MD    	(IO_UART8.UART8.SMR8.bit.MD)
#define	UART8_UART8_SMR8_SBL    	(IO_UART8.UART8.SMR8.bit.SBL)
#define	UART8_UART8_SMR8_BDS    	(IO_UART8.UART8.SMR8.bit.BDS)
#define	UART8_UART8_SMR8_SCKE    	(IO_UART8.UART8.SMR8.bit.SCKE)
#define	UART8_UART8_SMR8_SOE    	(IO_UART8.UART8.SMR8.bit.SOE)
#define	UART8_UART8_SSR8			(IO_UART8.UART8.SSR8.byte)
#define	UART8_UART8_SSR8_REC    	(IO_UART8.UART8.SSR8.bit.REC)
#define	UART8_UART8_SSR8_PE    	(IO_UART8.UART8.SSR8.bit.PE)
#define	UART8_UART8_SSR8_FRE    	(IO_UART8.UART8.SSR8.bit.FRE)
#define	UART8_UART8_SSR8_ORE    	(IO_UART8.UART8.SSR8.bit.ORE)
#define	UART8_UART8_SSR8_RDRF    	(IO_UART8.UART8.SSR8.bit.RDRF)
#define	UART8_UART8_SSR8_TDRE    	(IO_UART8.UART8.SSR8.bit.TDRE)
#define	UART8_UART8_SSR8_TBI    	(IO_UART8.UART8.SSR8.bit.TBI)
#define	UART8_UART8_ESCR8			(IO_UART8.UART8.ESCR8.byte)
#define	UART8_UART8_ESCR8_ESBL    	(IO_UART8.UART8.ESCR8.bit.ESBL)
#define	UART8_UART8_ESCR8_INV    	(IO_UART8.UART8.ESCR8.bit.INV)
#define	UART8_UART8_ESCR8_PEN    	(IO_UART8.UART8.ESCR8.bit.PEN)
#define	UART8_UART8_ESCR8_P    	(IO_UART8.UART8.ESCR8.bit.P)
#define	UART8_UART8_ESCR8_L    	(IO_UART8.UART8.ESCR8.bit.L)
#define	UART8_UART8_RDR8	(IO_UART8.UART8.RDR8.hword)
#define	UART8_UART8_RDR8_RDR8L	(IO_UART8.UART8.RDR8.byte.RDR8L)
#define	UART8_UART8_RDR8_bit9	(IO_UART8.UART8.RDR8.bit9.bit.D)
#define	UART8_UART8_RDR8_bit7	(IO_UART8.UART8.RDR8.bit7.bit.D)
#define	UART8_UART8_RDR8_bit6	(IO_UART8.UART8.RDR8.bit6.bit.D)
#define	UART8_UART8_RDR8_bit5	(IO_UART8.UART8.RDR8.bit5.bit.D)
#define	UART8_UART8_TDR8	(IO_UART8.UART8.RDR8.hword)
#define	UART8_UART8_TDR8_TDR8L	(IO_UART8.UART8.RDR8.byte.RDR8L)
#define	UART8_UART8_TDR8_bit9	(IO_UART8.UART8.RDR8.bit9.bit.D)
#define	UART8_UART8_TDR8_bit7	(IO_UART8.UART8.RDR8.bit7.bit.D)
#define	UART8_UART8_TDR8_bit6	(IO_UART8.UART8.RDR8.bit6.bit.D)
#define	UART8_UART8_TDR8_bit5	(IO_UART8.UART8.RDR8.bit5.bit.D)
#define	UART8_UART8_BGR8			(IO_UART8.UART8.BGR8.hword)
#define	UART8_UART8_BGR8_EXT    	(IO_UART8.UART8.BGR8.bit.EXT)
#define	UART8_UART8_BGR8_BGR    	(IO_UART8.UART8.BGR8.bit.BGR)
#define	UART8_UART8_FCR18			(IO_UART8.UART8.FCR18.byte)
#define	UART8_UART8_FCR18_FLSTE    	(IO_UART8.UART8.FCR18.bit.FLSTE)
#define	UART8_UART8_FCR18_FRIIE    	(IO_UART8.UART8.FCR18.bit.FRIIE)
#define	UART8_UART8_FCR18_FDRQ    	(IO_UART8.UART8.FCR18.bit.FDRQ)
#define	UART8_UART8_FCR18_FTIE    	(IO_UART8.UART8.FCR18.bit.FTIE)
#define	UART8_UART8_FCR18_FSEL    	(IO_UART8.UART8.FCR18.bit.FSEL)
#define	UART8_UART8_FCR08			(IO_UART8.UART8.FCR08.byte)
#define	UART8_UART8_FCR08_FLST    	(IO_UART8.UART8.FCR08.bit.FLST)
#define	UART8_UART8_FCR08_FLD    	(IO_UART8.UART8.FCR08.bit.FLD)
#define	UART8_UART8_FCR08_FSET    	(IO_UART8.UART8.FCR08.bit.FSET)
#define	UART8_UART8_FCR08_FCL2    	(IO_UART8.UART8.FCR08.bit.FCL2)
#define	UART8_UART8_FCR08_FCL1    	(IO_UART8.UART8.FCR08.bit.FCL1)
#define	UART8_UART8_FCR08_FE2    	(IO_UART8.UART8.FCR08.bit.FE2)
#define	UART8_UART8_FCR08_FE1    	(IO_UART8.UART8.FCR08.bit.FE1)
#define	UART8_UART8_FBYTE8	(IO_UART8.UART8.FBYTE8)

#define	UART8_CSIO8_SCR8			(IO_UART8.CSIO8.SCR8.byte)
#define	UART8_CSIO8_SCR8_UPCL    	(IO_UART8.CSIO8.SCR8.bit.UPCL)
#define	UART8_CSIO8_SCR8_MS    	(IO_UART8.CSIO8.SCR8.bit.MS)
#define	UART8_CSIO8_SCR8_SPI    	(IO_UART8.CSIO8.SCR8.bit.SPI)
#define	UART8_CSIO8_SCR8_RIE    	(IO_UART8.CSIO8.SCR8.bit.RIE)
#define	UART8_CSIO8_SCR8_TIE    	(IO_UART8.CSIO8.SCR8.bit.TIE)
#define	UART8_CSIO8_SCR8_TBIE    	(IO_UART8.CSIO8.SCR8.bit.TBIE)
#define	UART8_CSIO8_SCR8_RXE    	(IO_UART8.CSIO8.SCR8.bit.RXE)
#define	UART8_CSIO8_SCR8_TXE    	(IO_UART8.CSIO8.SCR8.bit.TXE)
#define	UART8_CSIO8_SMR8			(IO_UART8.CSIO8.SMR8.byte)
#define	UART8_CSIO8_SMR8_MD    	(IO_UART8.CSIO8.SMR8.bit.MD)
#define	UART8_CSIO8_SMR8_SCINV    	(IO_UART8.CSIO8.SMR8.bit.SCINV)
#define	UART8_CSIO8_SMR8_BDS    	(IO_UART8.CSIO8.SMR8.bit.BDS)
#define	UART8_CSIO8_SMR8_SCKE    	(IO_UART8.CSIO8.SMR8.bit.SCKE)
#define	UART8_CSIO8_SMR8_SOE    	(IO_UART8.CSIO8.SMR8.bit.SOE)
#define	UART8_CSIO8_SSR8			(IO_UART8.CSIO8.SSR8.byte)
#define	UART8_CSIO8_SSR8_REC    	(IO_UART8.CSIO8.SSR8.bit.REC)
#define	UART8_CSIO8_SSR8_ORE    	(IO_UART8.CSIO8.SSR8.bit.ORE)
#define	UART8_CSIO8_SSR8_RDRF    	(IO_UART8.CSIO8.SSR8.bit.RDRF)
#define	UART8_CSIO8_SSR8_TDRE    	(IO_UART8.CSIO8.SSR8.bit.TDRE)
#define	UART8_CSIO8_SSR8_TBI    	(IO_UART8.CSIO8.SSR8.bit.TBI)
#define	UART8_CSIO8_ESCR8			(IO_UART8.CSIO8.ESCR8.byte)
#define	UART8_CSIO8_ESCR8_SOP    	(IO_UART8.CSIO8.ESCR8.bit.SOP)
#define	UART8_CSIO8_ESCR8_WT    	(IO_UART8.CSIO8.ESCR8.bit.WT)
#define	UART8_CSIO8_ESCR8_L    	(IO_UART8.CSIO8.ESCR8.bit.L)
#define	UART8_CSIO8_RDR8	(IO_UART8.CSIO8.RDR8.hword)
#define	UART8_CSIO8_RDR8_RDR8L	(IO_UART8.CSIO8.RDR8.byte.RDR8L)
#define	UART8_CSIO8_RDR8_bit9	(IO_UART8.CSIO8.RDR8.bit9.bit.D)
#define	UART8_CSIO8_RDR8_bit7	(IO_UART8.CSIO8.RDR8.bit7.bit.D)
#define	UART8_CSIO8_RDR8_bit6	(IO_UART8.CSIO8.RDR8.bit6.bit.D)
#define	UART8_CSIO8_RDR8_bit5	(IO_UART8.CSIO8.RDR8.bit5.bit.D)
#define	UART8_CSIO8_TDR8	(IO_UART8.CSIO8.RDR8.hword)
#define	UART8_CSIO8_TDR8_TDR8L	(IO_UART8.CSIO8.RDR8.byte.RDR8L)
#define	UART8_CSIO8_TDR8_bit9	(IO_UART8.CSIO8.RDR8.bit9.bit.D)
#define	UART8_CSIO8_TDR8_bit7	(IO_UART8.CSIO8.RDR8.bit7.bit.D)
#define	UART8_CSIO8_TDR8_bit6	(IO_UART8.CSIO8.RDR8.bit6.bit.D)
#define	UART8_CSIO8_TDR8_bit5	(IO_UART8.CSIO8.RDR8.bit5.bit.D)
#define	UART8_CSIO8_BGR8	(IO_UART8.CSIO8.BGR8)
#define	UART8_CSIO8_FCR18			(IO_UART8.CSIO8.FCR18.byte)
#define	UART8_CSIO8_FCR18_FLSTE    	(IO_UART8.CSIO8.FCR18.bit.FLSTE)
#define	UART8_CSIO8_FCR18_FRIIE    	(IO_UART8.CSIO8.FCR18.bit.FRIIE)
#define	UART8_CSIO8_FCR18_FDRQ    	(IO_UART8.CSIO8.FCR18.bit.FDRQ)
#define	UART8_CSIO8_FCR18_FTIE    	(IO_UART8.CSIO8.FCR18.bit.FTIE)
#define	UART8_CSIO8_FCR18_FSEL    	(IO_UART8.CSIO8.FCR18.bit.FSEL)
#define	UART8_CSIO8_FCR08			(IO_UART8.CSIO8.FCR08.byte)
#define	UART8_CSIO8_FCR08_FLST    	(IO_UART8.CSIO8.FCR08.bit.FLST)
#define	UART8_CSIO8_FCR08_FLD    	(IO_UART8.CSIO8.FCR08.bit.FLD)
#define	UART8_CSIO8_FCR08_FSET    	(IO_UART8.CSIO8.FCR08.bit.FSET)
#define	UART8_CSIO8_FCR08_FCL2    	(IO_UART8.CSIO8.FCR08.bit.FCL2)
#define	UART8_CSIO8_FCR08_FCL1    	(IO_UART8.CSIO8.FCR08.bit.FCL1)
#define	UART8_CSIO8_FCR08_FE2    	(IO_UART8.CSIO8.FCR08.bit.FE2)
#define	UART8_CSIO8_FCR08_FE1    	(IO_UART8.CSIO8.FCR08.bit.FE1)
#define	UART8_CSIO8_FBYTE8	(IO_UART8.CSIO8.FBYTE8)

#define	UART8_LINUART8_SCR8			(IO_UART8.LINUART8.SCR8.byte)
#define	UART8_LINUART8_SCR8_UPCL    	(IO_UART8.LINUART8.SCR8.bit.UPCL)
#define	UART8_LINUART8_SCR8_MS    	(IO_UART8.LINUART8.SCR8.bit.MS)
#define	UART8_LINUART8_SCR8_LBR    	(IO_UART8.LINUART8.SCR8.bit.LBR)
#define	UART8_LINUART8_SCR8_RIE    	(IO_UART8.LINUART8.SCR8.bit.RIE)
#define	UART8_LINUART8_SCR8_TIE    	(IO_UART8.LINUART8.SCR8.bit.TIE)
#define	UART8_LINUART8_SCR8_TBIE    	(IO_UART8.LINUART8.SCR8.bit.TBIE)
#define	UART8_LINUART8_SCR8_RXE    	(IO_UART8.LINUART8.SCR8.bit.RXE)
#define	UART8_LINUART8_SCR8_TXE    	(IO_UART8.LINUART8.SCR8.bit.TXE)
#define	UART8_LINUART8_SMR8			(IO_UART8.LINUART8.SMR8.byte)
#define	UART8_LINUART8_SMR8_MD    	(IO_UART8.LINUART8.SMR8.bit.MD)
#define	UART8_LINUART8_SMR8_SBL    	(IO_UART8.LINUART8.SMR8.bit.SBL)
#define	UART8_LINUART8_SMR8_SCKE    	(IO_UART8.LINUART8.SMR8.bit.SCKE)
#define	UART8_LINUART8_SMR8_SOE    	(IO_UART8.LINUART8.SMR8.bit.SOE)
#define	UART8_LINUART8_SSR8			(IO_UART8.LINUART8.SSR8.byte)
#define	UART8_LINUART8_SSR8_REC    	(IO_UART8.LINUART8.SSR8.bit.REC)
#define	UART8_LINUART8_SSR8_LBD    	(IO_UART8.LINUART8.SSR8.bit.LBD)
#define	UART8_LINUART8_SSR8_FRE    	(IO_UART8.LINUART8.SSR8.bit.FRE)
#define	UART8_LINUART8_SSR8_ORE    	(IO_UART8.LINUART8.SSR8.bit.ORE)
#define	UART8_LINUART8_SSR8_RDRF    	(IO_UART8.LINUART8.SSR8.bit.RDRF)
#define	UART8_LINUART8_SSR8_TDRE    	(IO_UART8.LINUART8.SSR8.bit.TDRE)
#define	UART8_LINUART8_SSR8_TBI    	(IO_UART8.LINUART8.SSR8.bit.TBI)
#define	UART8_LINUART8_ESCR8			(IO_UART8.LINUART8.ESCR8.byte)
#define	UART8_LINUART8_ESCR8_ESBL    	(IO_UART8.LINUART8.ESCR8.bit.ESBL)
#define	UART8_LINUART8_ESCR8_LBIE    	(IO_UART8.LINUART8.ESCR8.bit.LBIE)
#define	UART8_LINUART8_ESCR8_LBL    	(IO_UART8.LINUART8.ESCR8.bit.LBL)
#define	UART8_LINUART8_ESCR8_DEL    	(IO_UART8.LINUART8.ESCR8.bit.DEL)
#define	UART8_LINUART8_RDR8	(IO_UART8.LINUART8.RDR8.hword)
#define	UART8_LINUART8_RDR8_RDR8L	(IO_UART8.LINUART8.RDR8.byte.RDR8L)
#define	UART8_LINUART8_TDR8	(IO_UART8.LINUART8.RDR8.hword)
#define	UART8_LINUART8_TDR8_TDR8L	(IO_UART8.LINUART8.RDR8.byte.RDR8L)
#define	UART8_LINUART8_BGR8			(IO_UART8.LINUART8.BGR8.hword)
#define	UART8_LINUART8_BGR8_EXT    	(IO_UART8.LINUART8.BGR8.bit.EXT)
#define	UART8_LINUART8_BGR8_BGR    	(IO_UART8.LINUART8.BGR8.bit.BGR)
#define	UART8_LINUART8_FCR18			(IO_UART8.LINUART8.FCR18.byte)
#define	UART8_LINUART8_FCR18_FLSTE    	(IO_UART8.LINUART8.FCR18.bit.FLSTE)
#define	UART8_LINUART8_FCR18_FRIIE    	(IO_UART8.LINUART8.FCR18.bit.FRIIE)
#define	UART8_LINUART8_FCR18_FDRQ    	(IO_UART8.LINUART8.FCR18.bit.FDRQ)
#define	UART8_LINUART8_FCR18_FTIE    	(IO_UART8.LINUART8.FCR18.bit.FTIE)
#define	UART8_LINUART8_FCR18_FSEL    	(IO_UART8.LINUART8.FCR18.bit.FSEL)
#define	UART8_LINUART8_FCR08			(IO_UART8.LINUART8.FCR08.byte)
#define	UART8_LINUART8_FCR08_FLST    	(IO_UART8.LINUART8.FCR08.bit.FLST)
#define	UART8_LINUART8_FCR08_FLD    	(IO_UART8.LINUART8.FCR08.bit.FLD)
#define	UART8_LINUART8_FCR08_FSET    	(IO_UART8.LINUART8.FCR08.bit.FSET)
#define	UART8_LINUART8_FCR08_FCL2    	(IO_UART8.LINUART8.FCR08.bit.FCL2)
#define	UART8_LINUART8_FCR08_FCL1    	(IO_UART8.LINUART8.FCR08.bit.FCL1)
#define	UART8_LINUART8_FCR08_FE2    	(IO_UART8.LINUART8.FCR08.bit.FE2)
#define	UART8_LINUART8_FCR08_FE1    	(IO_UART8.LINUART8.FCR08.bit.FE1)
#define	UART8_LINUART8_FBYTE8	(IO_UART8.LINUART8.FBYTE8)

#define	UART8_I2C8_IBCR8			(IO_UART8.I2C8.IBCR8.byte)
#define	UART8_I2C8_IBCR8_MSS    	(IO_UART8.I2C8.IBCR8.bit.MSS)
#define	UART8_I2C8_IBCR8_ACT    	(IO_UART8.I2C8.IBCR8.bit.ACT)
#define	UART8_I2C8_IBCR8_ACKE    	(IO_UART8.I2C8.IBCR8.bit.ACKE)
#define	UART8_I2C8_IBCR8_WSEL    	(IO_UART8.I2C8.IBCR8.bit.WSEL)
#define	UART8_I2C8_IBCR8_CNDE    	(IO_UART8.I2C8.IBCR8.bit.CNDE)
#define	UART8_I2C8_IBCR8_INTE    	(IO_UART8.I2C8.IBCR8.bit.INTE)
#define	UART8_I2C8_IBCR8_BER    	(IO_UART8.I2C8.IBCR8.bit.BER)
#define	UART8_I2C8_IBCR8_INT    	(IO_UART8.I2C8.IBCR8.bit.INT)
#define	UART8_I2C8_SMR8			(IO_UART8.I2C8.SMR8.byte)
#define	UART8_I2C8_SMR8_MD    	(IO_UART8.I2C8.SMR8.bit.MD)
#define	UART8_I2C8_SMR8_RIE    	(IO_UART8.I2C8.SMR8.bit.RIE)
#define	UART8_I2C8_SMR8_TIE    	(IO_UART8.I2C8.SMR8.bit.TIE)
#define	UART8_I2C8_SSR8			(IO_UART8.I2C8.SSR8.byte)
#define	UART8_I2C8_SSR8_REC    	(IO_UART8.I2C8.SSR8.bit.REC)
#define	UART8_I2C8_SSR8_TSET    	(IO_UART8.I2C8.SSR8.bit.TSET)
#define	UART8_I2C8_SSR8_DMA    	(IO_UART8.I2C8.SSR8.bit.DMA)
#define	UART8_I2C8_SSR8_TBIE    	(IO_UART8.I2C8.SSR8.bit.TBIE)
#define	UART8_I2C8_SSR8_ORE    	(IO_UART8.I2C8.SSR8.bit.ORE)
#define	UART8_I2C8_SSR8_RDRF    	(IO_UART8.I2C8.SSR8.bit.RDRF)
#define	UART8_I2C8_SSR8_TDRE    	(IO_UART8.I2C8.SSR8.bit.TDRE)
#define	UART8_I2C8_SSR8_TBI    	(IO_UART8.I2C8.SSR8.bit.TBI)
#define	UART8_I2C8_IBSR8			(IO_UART8.I2C8.IBSR8.byte)
#define	UART8_I2C8_IBSR8_FBT    	(IO_UART8.I2C8.IBSR8.bit.FBT)
#define	UART8_I2C8_IBSR8_RACK    	(IO_UART8.I2C8.IBSR8.bit.RACK)
#define	UART8_I2C8_IBSR8_RSA    	(IO_UART8.I2C8.IBSR8.bit.RSA)
#define	UART8_I2C8_IBSR8_TRX    	(IO_UART8.I2C8.IBSR8.bit.TRX)
#define	UART8_I2C8_IBSR8_AL    	(IO_UART8.I2C8.IBSR8.bit.AL)
#define	UART8_I2C8_IBSR8_RSC    	(IO_UART8.I2C8.IBSR8.bit.RSC)
#define	UART8_I2C8_IBSR8_SPC    	(IO_UART8.I2C8.IBSR8.bit.SPC)
#define	UART8_I2C8_IBSR8_BB    	(IO_UART8.I2C8.IBSR8.bit.BB)
#define	UART8_I2C8_RDR8	(IO_UART8.I2C8.RDR8.hword)
#define	UART8_I2C8_RDR8_RDR8L	(IO_UART8.I2C8.RDR8.byte.RDR8L)
#define	UART8_I2C8_TDR8	(IO_UART8.I2C8.RDR8.hword)
#define	UART8_I2C8_TDR8_TDR8L	(IO_UART8.I2C8.RDR8.byte.RDR8L)
#define	UART8_I2C8_BGR8	(IO_UART8.I2C8.BGR8)
#define	UART8_I2C8_ISMK8			(IO_UART8.I2C8.ISMK8.byte)
#define	UART8_I2C8_ISMK8_EN    	(IO_UART8.I2C8.ISMK8.bit.EN)
#define	UART8_I2C8_ISMK8_SM    	(IO_UART8.I2C8.ISMK8.bit.SM)
#define	UART8_I2C8_ISBA8			(IO_UART8.I2C8.ISBA8.byte)
#define	UART8_I2C8_ISBA8_SAEN    	(IO_UART8.I2C8.ISBA8.bit.SAEN)
#define	UART8_I2C8_ISBA8_SA    	(IO_UART8.I2C8.ISBA8.bit.SA)
#define	UART8_I2C8_FCR18			(IO_UART8.I2C8.FCR18.byte)
#define	UART8_I2C8_FCR18_FLSTE    	(IO_UART8.I2C8.FCR18.bit.FLSTE)
#define	UART8_I2C8_FCR18_FRIIE    	(IO_UART8.I2C8.FCR18.bit.FRIIE)
#define	UART8_I2C8_FCR18_FDRQ    	(IO_UART8.I2C8.FCR18.bit.FDRQ)
#define	UART8_I2C8_FCR18_FTIE    	(IO_UART8.I2C8.FCR18.bit.FTIE)
#define	UART8_I2C8_FCR18_FSEL    	(IO_UART8.I2C8.FCR18.bit.FSEL)
#define	UART8_I2C8_FCR08			(IO_UART8.I2C8.FCR08.byte)
#define	UART8_I2C8_FCR08_FLST    	(IO_UART8.I2C8.FCR08.bit.FLST)
#define	UART8_I2C8_FCR08_FLD    	(IO_UART8.I2C8.FCR08.bit.FLD)
#define	UART8_I2C8_FCR08_FSET    	(IO_UART8.I2C8.FCR08.bit.FSET)
#define	UART8_I2C8_FCR08_FCL2    	(IO_UART8.I2C8.FCR08.bit.FCL2)
#define	UART8_I2C8_FCR08_FCL1    	(IO_UART8.I2C8.FCR08.bit.FCL1)
#define	UART8_I2C8_FCR08_FE2    	(IO_UART8.I2C8.FCR08.bit.FE2)
#define	UART8_I2C8_FCR08_FE1    	(IO_UART8.I2C8.FCR08.bit.FE1)
#define	UART8_I2C8_FBYTE8	(IO_UART8.I2C8.FBYTE8)

#ifdef __IO_DEFINE
#pragma segment DATA=IO_UART9,attr=IO,locate=0x4F0
#endif

typedef union {

struct {
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
	} SCR9;
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
	} SMR9;
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
	} SSR9;
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
	} ESCR9;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR9L;
		} byte;
		struct {
			struct {
				__HWORD	RESV1:7;
				__HWORD	D:9;
			} bit;
		} bit9;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	D:7;
			} bit;
		} bit7;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	D:6;
			} bit;
		} bit6;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	RESV3:1;
				__BYTE	D:5;
			} bit;
		} bit5;
	} RDR9;
	union {
		__HWORD	hword;
		struct {
			__HWORD	EXT:1;
			__HWORD	BGR:15;
		} bit;
	} BGR9;
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
	} FCR19;
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
	} FCR09;
	__HWORD	FBYTE9;
} UART9;

struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	UPCL:1;
			__BYTE	MS:1;
			__BYTE	SPI:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
			__BYTE	TBIE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} SCR9;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:3;
			__BYTE	RESV11:1;
			__BYTE	SCINV:1;
			__BYTE	BDS:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} SMR9;
	union {
		__BYTE	byte;
		struct {
			__BYTE	REC:1;
			__BYTE	RESV21:1;
			__BYTE	RESV22:1;
			__BYTE	RESV23:1;
			__BYTE	ORE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	TBI:1;
		} bit;
	} SSR9;
	union {
		__BYTE	byte;
		struct {
			__BYTE	SOP:1;
			__BYTE	RESV31:1;
			__BYTE	RESV32:1;
			__BYTE	WT:2;
			__BYTE	L:3;
		} bit;
	} ESCR9;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR9L;
		} byte;
		struct {
			struct {
				__HWORD	RESV1:7;
				__HWORD	D:9;
			} bit;
		} bit9;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	D:7;
			} bit;
		} bit7;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	D:6;
			} bit;
		} bit6;
		struct {
			__BYTE	RESV1;
			struct {
				__BYTE	RESV1:1;
				__BYTE	RESV2:1;
				__BYTE	RESV3:1;
				__BYTE	D:5;
			} bit;
		} bit5;
	} RDR9;
	__HWORD	BGR9;
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
	} FCR19;
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
	} FCR09;
	__HWORD	FBYTE9;
} CSIO9;

struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	UPCL:1;
			__BYTE	MS:1;
			__BYTE	LBR:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
			__BYTE	TBIE:1;
			__BYTE	RXE:1;
			__BYTE	TXE:1;
		} bit;
	} SCR9;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:3;
			__BYTE	RESV11:1;
			__BYTE	SBL:1;
			__BYTE	RESV13:1;
			__BYTE	SCKE:1;
			__BYTE	SOE:1;
		} bit;
	} SMR9;
	union {
		__BYTE	byte;
		struct {
			__BYTE	REC:1;
			__BYTE	RESV21:1;
			__BYTE	LBD:1;
			__BYTE	FRE:1;
			__BYTE	ORE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	TBI:1;
		} bit;
	} SSR9;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV30:1;
			__BYTE	ESBL:1;
			__BYTE	RESV32:1;
			__BYTE	LBIE:1;
			__BYTE	LBL:2;
			__BYTE	DEL:2;
		} bit;
	} ESCR9;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR9L;
		} byte;
	} RDR9;
	union {
		__HWORD	hword;
		struct {
			__HWORD	EXT:1;
			__HWORD	BGR:15;
		} bit;
	} BGR9;
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
	} FCR19;
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
	} FCR09;
	__HWORD	FBYTE9;
} LINUART9;

struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	MSS:1;
			__BYTE	ACT:1;
			__BYTE	ACKE:1;
			__BYTE	WSEL:1;
			__BYTE	CNDE:1;
			__BYTE	INTE:1;
			__BYTE	BER:1;
			__BYTE	INT:1;
		} bit;
	} IBCR9;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MD:3;
			__BYTE	RESV11:1;
			__BYTE	RIE:1;
			__BYTE	TIE:1;
			__BYTE	RESV1:1;
			__BYTE	RESV0:1;
		} bit;
	} SMR9;
	union {
		__BYTE	byte;
		struct {
			__BYTE	REC:1;
			__BYTE	TSET:1;
			__BYTE	DMA:1;
			__BYTE	TBIE:1;
			__BYTE	ORE:1;
			__BYTE	RDRF:1;
			__BYTE	TDRE:1;
			__BYTE	TBI:1;
		} bit;
	} SSR9;
	union {
		__BYTE	byte;
		struct {
			__BYTE	FBT:1;
			__BYTE	RACK:1;
			__BYTE	RSA:1;
			__BYTE	TRX:1;
			__BYTE	AL:1;
			__BYTE	RSC:1;
			__BYTE	SPC:1;
			__BYTE	BB:1;
		} bit;
	} IBSR9;
	union {
		__HWORD	hword;
		struct {
			__BYTE	RESV1;
			__BYTE	RDR9L;
		} byte;
	} RDR9;
	__HWORD	BGR9;
	union {
		__BYTE	byte;
		struct {
			__BYTE	EN:1;
			__BYTE	SM:7;
		} bit;
	} ISMK9;
	union {
		__BYTE	byte;
		struct {
			__BYTE	SAEN:1;
			__BYTE	SA:7;
		} bit;
	} ISBA9;
	__HWORD	IO_RESV1;
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
	} FCR19;
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
	} FCR09;
	__HWORD	FBYTE9;
} I2C9;

} UART9STR;

__IO_EXTENDED	UART9STR	IO_UART9;

#define	UART9_UART9_SCR9			(IO_UART9.UART9.SCR9.byte)
#define	UART9_UART9_SCR9_UPCL    	(IO_UART9.UART9.SCR9.bit.UPCL)
#define	UART9_UART9_SCR9_RIE    	(IO_UART9.UART9.SCR9.bit.RIE)
#define	UART9_UART9_SCR9_TIE    	(IO_UART9.UART9.SCR9.bit.TIE)
#define	UART9_UART9_SCR9_TBIE    	(IO_UART9.UART9.SCR9.bit.TBIE)
#define	UART9_UART9_SCR9_RXE    	(IO_UART9.UART9.SCR9.bit.RXE)
#define	UART9_UART9_SCR9_TXE    	(IO_UART9.UART9.SCR9.bit.TXE)
#define	UART9_UART9_SMR9			(IO_UART9.UART9.SMR9.byte)
#define	UART9_UART9_SMR9_MD    	(IO_UART9.UART9.SMR9.bit.MD)
#define	UART9_UART9_SMR9_SBL    	(IO_UART9.UART9.SMR9.bit.SBL)
#define	UART9_UART9_SMR9_BDS    	(IO_UART9.UART9.SMR9.bit.BDS)
#define	UART9_UART9_SMR9_SCKE    	(IO_UART9.UART9.SMR9.bit.SCKE)
#define	UART9_UART9_SMR9_SOE    	(IO_UART9.UART9.SMR9.bit.SOE)
#define	UART9_UART9_SSR9			(IO_UART9.UART9.SSR9.byte)
#define	UART9_UART9_SSR9_REC    	(IO_UART9.UART9.SSR9.bit.REC)
#define	UART9_UART9_SSR9_PE    	(IO_UART9.UART9.SSR9.bit.PE)
#define	UART9_UART9_SSR9_FRE    	(IO_UART9.UART9.SSR9.bit.FRE)
#define	UART9_UART9_SSR9_ORE    	(IO_UART9.UART9.SSR9.bit.ORE)
#define	UART9_UART9_SSR9_RDRF    	(IO_UART9.UART9.SSR9.bit.RDRF)
#define	UART9_UART9_SSR9_TDRE    	(IO_UART9.UART9.SSR9.bit.TDRE)
#define	UART9_UART9_SSR9_TBI    	(IO_UART9.UART9.SSR9.bit.TBI)
#define	UART9_UART9_ESCR9			(IO_UART9.UART9.ESCR9.byte)
#define	UART9_UART9_ESCR9_ESBL    	(IO_UART9.UART9.ESCR9.bit.ESBL)
#define	UART9_UART9_ESCR9_INV    	(IO_UART9.UART9.ESCR9.bit.INV)
#define	UART9_UART9_ESCR9_PEN    	(IO_UART9.UART9.ESCR9.bit.PEN)
#define	UART9_UART9_ESCR9_P    	(IO_UART9.UART9.ESCR9.bit.P)
#define	UART9_UART9_ESCR9_L    	(IO_UART9.UART9.ESCR9.bit.L)
#define	UART9_UART9_RDR9	(IO_UART9.UART9.RDR9.hword)
#define	UART9_UART9_RDR9_RDR9L	(IO_UART9.UART9.RDR9.byte.RDR9L)
#define	UART9_UART9_RDR9_bit9	(IO_UART9.UART9.RDR9.bit9.bit.D)
#define	UART9_UART9_RDR9_bit7	(IO_UART9.UART9.RDR9.bit7.bit.D)
#define	UART9_UART9_RDR9_bit6	(IO_UART9.UART9.RDR9.bit6.bit.D)
#define	UART9_UART9_RDR9_bit5	(IO_UART9.UART9.RDR9.bit5.bit.D)
#define	UART9_UART9_TDR9	(IO_UART9.UART9.RDR9.hword)
#define	UART9_UART9_TDR9_TDR9L	(IO_UART9.UART9.RDR9.byte.RDR9L)
#define	UART9_UART9_TDR9_bit9	(IO_UART9.UART9.RDR9.bit9.bit.D)
#define	UART9_UART9_TDR9_bit7	(IO_UART9.UART9.RDR9.bit7.bit.D)
#define	UART9_UART9_TDR9_bit6	(IO_UART9.UART9.RDR9.bit6.bit.D)
#define	UART9_UART9_TDR9_bit5	(IO_UART9.UART9.RDR9.bit5.bit.D)
#define	UART9_UART9_BGR9			(IO_UART9.UART9.BGR9.hword)
#define	UART9_UART9_BGR9_EXT    	(IO_UART9.UART9.BGR9.bit.EXT)
#define	UART9_UART9_BGR9_BGR    	(IO_UART9.UART9.BGR9.bit.BGR)
#define	UART9_UART9_FCR19			(IO_UART9.UART9.FCR19.byte)
#define	UART9_UART9_FCR19_FLSTE    	(IO_UART9.UART9.FCR19.bit.FLSTE)
#define	UART9_UART9_FCR19_FRIIE    	(IO_UART9.UART9.FCR19.bit.FRIIE)
#define	UART9_UART9_FCR19_FDRQ    	(IO_UART9.UART9.FCR19.bit.FDRQ)
#define	UART9_UART9_FCR19_FTIE    	(IO_UART9.UART9.FCR19.bit.FTIE)
#define	UART9_UART9_FCR19_FSEL    	(IO_UART9.UART9.FCR19.bit.FSEL)
#define	UART9_UART9_FCR09			(IO_UART9.UART9.FCR09.byte)
#define	UART9_UART9_FCR09_FLST    	(IO_UART9.UART9.FCR09.bit.FLST)
#define	UART9_UART9_FCR09_FLD    	(IO_UART9.UART9.FCR09.bit.FLD)
#define	UART9_UART9_FCR09_FSET    	(IO_UART9.UART9.FCR09.bit.FSET)
#define	UART9_UART9_FCR09_FCL2    	(IO_UART9.UART9.FCR09.bit.FCL2)
#define	UART9_UART9_FCR09_FCL1    	(IO_UART9.UART9.FCR09.bit.FCL1)
#define	UART9_UART9_FCR09_FE2    	(IO_UART9.UART9.FCR09.bit.FE2)
#define	UART9_UART9_FCR09_FE1    	(IO_UART9.UART9.FCR09.bit.FE1)
#define	UART9_UART9_FBYTE9	(IO_UART9.UART9.FBYTE9)

#define	UART9_CSIO9_SCR9			(IO_UART9.CSIO9.SCR9.byte)
#define	UART9_CSIO9_SCR9_UPCL    	(IO_UART9.CSIO9.SCR9.bit.UPCL)
#define	UART9_CSIO9_SCR9_MS    	(IO_UART9.CSIO9.SCR9.bit.MS)
#define	UART9_CSIO9_SCR9_SPI    	(IO_UART9.CSIO9.SCR9.bit.SPI)
#define	UART9_CSIO9_SCR9_RIE    	(IO_UART9.CSIO9.SCR9.bit.RIE)
#define	UART9_CSIO9_SCR9_TIE    	(IO_UART9.CSIO9.SCR9.bit.TIE)
#define	UART9_CSIO9_SCR9_TBIE    	(IO_UART9.CSIO9.SCR9.bit.TBIE)
#define	UART9_CSIO9_SCR9_RXE    	(IO_UART9.CSIO9.SCR9.bit.RXE)
#define	UART9_CSIO9_SCR9_TXE    	(IO_UART9.CSIO9.SCR9.bit.TXE)
#define	UART9_CSIO9_SMR9			(IO_UART9.CSIO9.SMR9.byte)
#define	UART9_CSIO9_SMR9_MD    	(IO_UART9.CSIO9.SMR9.bit.MD)
#define	UART9_CSIO9_SMR9_SCINV    	(IO_UART9.CSIO9.SMR9.bit.SCINV)
#define	UART9_CSIO9_SMR9_BDS    	(IO_UART9.CSIO9.SMR9.bit.BDS)
#define	UART9_CSIO9_SMR9_SCKE    	(IO_UART9.CSIO9.SMR9.bit.SCKE)
#define	UART9_CSIO9_SMR9_SOE    	(IO_UART9.CSIO9.SMR9.bit.SOE)
#define	UART9_CSIO9_SSR9			(IO_UART9.CSIO9.SSR9.byte)
#define	UART9_CSIO9_SSR9_REC    	(IO_UART9.CSIO9.SSR9.bit.REC)
#define	UART9_CSIO9_SSR9_ORE    	(IO_UART9.CSIO9.SSR9.bit.ORE)
#define	UART9_CSIO9_SSR9_RDRF    	(IO_UART9.CSIO9.SSR9.bit.RDRF)
#define	UART9_CSIO9_SSR9_TDRE    	(IO_UART9.CSIO9.SSR9.bit.TDRE)
#define	UART9_CSIO9_SSR9_TBI    	(IO_UART9.CSIO9.SSR9.bit.TBI)
#define	UART9_CSIO9_ESCR9			(IO_UART9.CSIO9.ESCR9.byte)
#define	UART9_CSIO9_ESCR9_SOP    	(IO_UART9.CSIO9.ESCR9.bit.SOP)
#define	UART9_CSIO9_ESCR9_WT    	(IO_UART9.CSIO9.ESCR9.bit.WT)
#define	UART9_CSIO9_ESCR9_L    	(IO_UART9.CSIO9.ESCR9.bit.L)
#define	UART9_CSIO9_RDR9	(IO_UART9.CSIO9.RDR9.hword)
#define	UART9_CSIO9_RDR9_RDR9L	(IO_UART9.CSIO9.RDR9.byte.RDR9L)
#define	UART9_CSIO9_RDR9_bit9	(IO_UART9.CSIO9.RDR9.bit9.bit.D)
#define	UART9_CSIO9_RDR9_bit7	(IO_UART9.CSIO9.RDR9.bit7.bit.D)
#define	UART9_CSIO9_RDR9_bit6	(IO_UART9.CSIO9.RDR9.bit6.bit.D)
#define	UART9_CSIO9_RDR9_bit5	(IO_UART9.CSIO9.RDR9.bit5.bit.D)
#define	UART9_CSIO9_TDR9	(IO_UART9.CSIO9.RDR9.hword)
#define	UART9_CSIO9_TDR9_TDR9L	(IO_UART9.CSIO9.RDR9.byte.RDR9L)
#define	UART9_CSIO9_TDR9_bit9	(IO_UART9.CSIO9.RDR9.bit9.bit.D)
#define	UART9_CSIO9_TDR9_bit7	(IO_UART9.CSIO9.RDR9.bit7.bit.D)
#define	UART9_CSIO9_TDR9_bit6	(IO_UART9.CSIO9.RDR9.bit6.bit.D)
#define	UART9_CSIO9_TDR9_bit5	(IO_UART9.CSIO9.RDR9.bit5.bit.D)
#define	UART9_CSIO9_BGR9	(IO_UART9.CSIO9.BGR9)
#define	UART9_CSIO9_FCR19			(IO_UART9.CSIO9.FCR19.byte)
#define	UART9_CSIO9_FCR19_FLSTE    	(IO_UART9.CSIO9.FCR19.bit.FLSTE)
#define	UART9_CSIO9_FCR19_FRIIE    	(IO_UART9.CSIO9.FCR19.bit.FRIIE)
#define	UART9_CSIO9_FCR19_FDRQ    	(IO_UART9.CSIO9.FCR19.bit.FDRQ)
#define	UART9_CSIO9_FCR19_FTIE    	(IO_UART9.CSIO9.FCR19.bit.FTIE)
#define	UART9_CSIO9_FCR19_FSEL    	(IO_UART9.CSIO9.FCR19.bit.FSEL)
#define	UART9_CSIO9_FCR09			(IO_UART9.CSIO9.FCR09.byte)
#define	UART9_CSIO9_FCR09_FLST    	(IO_UART9.CSIO9.FCR09.bit.FLST)
#define	UART9_CSIO9_FCR09_FLD    	(IO_UART9.CSIO9.FCR09.bit.FLD)
#define	UART9_CSIO9_FCR09_FSET    	(IO_UART9.CSIO9.FCR09.bit.FSET)
#define	UART9_CSIO9_FCR09_FCL2    	(IO_UART9.CSIO9.FCR09.bit.FCL2)
#define	UART9_CSIO9_FCR09_FCL1    	(IO_UART9.CSIO9.FCR09.bit.FCL1)
#define	UART9_CSIO9_FCR09_FE2    	(IO_UART9.CSIO9.FCR09.bit.FE2)
#define	UART9_CSIO9_FCR09_FE1    	(IO_UART9.CSIO9.FCR09.bit.FE1)
#define	UART9_CSIO9_FBYTE9	(IO_UART9.CSIO9.FBYTE9)

#define	UART9_LINUART9_SCR9			(IO_UART9.LINUART9.SCR9.byte)
#define	UART9_LINUART9_SCR9_UPCL    	(IO_UART9.LINUART9.SCR9.bit.UPCL)
#define	UART9_LINUART9_SCR9_MS    	(IO_UART9.LINUART9.SCR9.bit.MS)
#define	UART9_LINUART9_SCR9_LBR    	(IO_UART9.LINUART9.SCR9.bit.LBR)
#define	UART9_LINUART9_SCR9_RIE    	(IO_UART9.LINUART9.SCR9.bit.RIE)
#define	UART9_LINUART9_SCR9_TIE    	(IO_UART9.LINUART9.SCR9.bit.TIE)
#define	UART9_LINUART9_SCR9_TBIE    	(IO_UART9.LINUART9.SCR9.bit.TBIE)
#define	UART9_LINUART9_SCR9_RXE    	(IO_UART9.LINUART9.SCR9.bit.RXE)
#define	UART9_LINUART9_SCR9_TXE    	(IO_UART9.LINUART9.SCR9.bit.TXE)
#define	UART9_LINUART9_SMR9			(IO_UART9.LINUART9.SMR9.byte)
#define	UART9_LINUART9_SMR9_MD    	(IO_UART9.LINUART9.SMR9.bit.MD)
#define	UART9_LINUART9_SMR9_SBL    	(IO_UART9.LINUART9.SMR9.bit.SBL)
#define	UART9_LINUART9_SMR9_SCKE    	(IO_UART9.LINUART9.SMR9.bit.SCKE)
#define	UART9_LINUART9_SMR9_SOE    	(IO_UART9.LINUART9.SMR9.bit.SOE)
#define	UART9_LINUART9_SSR9			(IO_UART9.LINUART9.SSR9.byte)
#define	UART9_LINUART9_SSR9_REC    	(IO_UART9.LINUART9.SSR9.bit.REC)
#define	UART9_LINUART9_SSR9_LBD    	(IO_UART9.LINUART9.SSR9.bit.LBD)
#define	UART9_LINUART9_SSR9_FRE    	(IO_UART9.LINUART9.SSR9.bit.FRE)
#define	UART9_LINUART9_SSR9_ORE    	(IO_UART9.LINUART9.SSR9.bit.ORE)
#define	UART9_LINUART9_SSR9_RDRF    	(IO_UART9.LINUART9.SSR9.bit.RDRF)
#define	UART9_LINUART9_SSR9_TDRE    	(IO_UART9.LINUART9.SSR9.bit.TDRE)
#define	UART9_LINUART9_SSR9_TBI    	(IO_UART9.LINUART9.SSR9.bit.TBI)
#define	UART9_LINUART9_ESCR9			(IO_UART9.LINUART9.ESCR9.byte)
#define	UART9_LINUART9_ESCR9_ESBL    	(IO_UART9.LINUART9.ESCR9.bit.ESBL)
#define	UART9_LINUART9_ESCR9_LBIE    	(IO_UART9.LINUART9.ESCR9.bit.LBIE)
#define	UART9_LINUART9_ESCR9_LBL    	(IO_UART9.LINUART9.ESCR9.bit.LBL)
#define	UART9_LINUART9_ESCR9_DEL    	(IO_UART9.LINUART9.ESCR9.bit.DEL)
#define	UART9_LINUART9_RDR9	(IO_UART9.LINUART9.RDR9.hword)
#define	UART9_LINUART9_RDR9_RDR9L	(IO_UART9.LINUART9.RDR9.byte.RDR9L)
#define	UART9_LINUART9_TDR9	(IO_UART9.LINUART9.RDR9.hword)
#define	UART9_LINUART9_TDR9_TDR9L	(IO_UART9.LINUART9.RDR9.byte.RDR9L)
#define	UART9_LINUART9_BGR9			(IO_UART9.LINUART9.BGR9.hword)
#define	UART9_LINUART9_BGR9_EXT    	(IO_UART9.LINUART9.BGR9.bit.EXT)
#define	UART9_LINUART9_BGR9_BGR    	(IO_UART9.LINUART9.BGR9.bit.BGR)
#define	UART9_LINUART9_FCR19			(IO_UART9.LINUART9.FCR19.byte)
#define	UART9_LINUART9_FCR19_FLSTE    	(IO_UART9.LINUART9.FCR19.bit.FLSTE)
#define	UART9_LINUART9_FCR19_FRIIE    	(IO_UART9.LINUART9.FCR19.bit.FRIIE)
#define	UART9_LINUART9_FCR19_FDRQ    	(IO_UART9.LINUART9.FCR19.bit.FDRQ)
#define	UART9_LINUART9_FCR19_FTIE    	(IO_UART9.LINUART9.FCR19.bit.FTIE)
#define	UART9_LINUART9_FCR19_FSEL    	(IO_UART9.LINUART9.FCR19.bit.FSEL)
#define	UART9_LINUART9_FCR09			(IO_UART9.LINUART9.FCR09.byte)
#define	UART9_LINUART9_FCR09_FLST    	(IO_UART9.LINUART9.FCR09.bit.FLST)
#define	UART9_LINUART9_FCR09_FLD    	(IO_UART9.LINUART9.FCR09.bit.FLD)
#define	UART9_LINUART9_FCR09_FSET    	(IO_UART9.LINUART9.FCR09.bit.FSET)
#define	UART9_LINUART9_FCR09_FCL2    	(IO_UART9.LINUART9.FCR09.bit.FCL2)
#define	UART9_LINUART9_FCR09_FCL1    	(IO_UART9.LINUART9.FCR09.bit.FCL1)
#define	UART9_LINUART9_FCR09_FE2    	(IO_UART9.LINUART9.FCR09.bit.FE2)
#define	UART9_LINUART9_FCR09_FE1    	(IO_UART9.LINUART9.FCR09.bit.FE1)
#define	UART9_LINUART9_FBYTE9	(IO_UART9.LINUART9.FBYTE9)

#define	UART9_I2C9_IBCR9			(IO_UART9.I2C9.IBCR9.byte)
#define	UART9_I2C9_IBCR9_MSS    	(IO_UART9.I2C9.IBCR9.bit.MSS)
#define	UART9_I2C9_IBCR9_ACT    	(IO_UART9.I2C9.IBCR9.bit.ACT)
#define	UART9_I2C9_IBCR9_ACKE    	(IO_UART9.I2C9.IBCR9.bit.ACKE)
#define	UART9_I2C9_IBCR9_WSEL    	(IO_UART9.I2C9.IBCR9.bit.WSEL)
#define	UART9_I2C9_IBCR9_CNDE    	(IO_UART9.I2C9.IBCR9.bit.CNDE)
#define	UART9_I2C9_IBCR9_INTE    	(IO_UART9.I2C9.IBCR9.bit.INTE)
#define	UART9_I2C9_IBCR9_BER    	(IO_UART9.I2C9.IBCR9.bit.BER)
#define	UART9_I2C9_IBCR9_INT    	(IO_UART9.I2C9.IBCR9.bit.INT)
#define	UART9_I2C9_SMR9			(IO_UART9.I2C9.SMR9.byte)
#define	UART9_I2C9_SMR9_MD    	(IO_UART9.I2C9.SMR9.bit.MD)
#define	UART9_I2C9_SMR9_RIE    	(IO_UART9.I2C9.SMR9.bit.RIE)
#define	UART9_I2C9_SMR9_TIE    	(IO_UART9.I2C9.SMR9.bit.TIE)
#define	UART9_I2C9_SSR9			(IO_UART9.I2C9.SSR9.byte)
#define	UART9_I2C9_SSR9_REC    	(IO_UART9.I2C9.SSR9.bit.REC)
#define	UART9_I2C9_SSR9_TSET    	(IO_UART9.I2C9.SSR9.bit.TSET)
#define	UART9_I2C9_SSR9_DMA    	(IO_UART9.I2C9.SSR9.bit.DMA)
#define	UART9_I2C9_SSR9_TBIE    	(IO_UART9.I2C9.SSR9.bit.TBIE)
#define	UART9_I2C9_SSR9_ORE    	(IO_UART9.I2C9.SSR9.bit.ORE)
#define	UART9_I2C9_SSR9_RDRF    	(IO_UART9.I2C9.SSR9.bit.RDRF)
#define	UART9_I2C9_SSR9_TDRE    	(IO_UART9.I2C9.SSR9.bit.TDRE)
#define	UART9_I2C9_SSR9_TBI    	(IO_UART9.I2C9.SSR9.bit.TBI)
#define	UART9_I2C9_IBSR9			(IO_UART9.I2C9.IBSR9.byte)
#define	UART9_I2C9_IBSR9_FBT    	(IO_UART9.I2C9.IBSR9.bit.FBT)
#define	UART9_I2C9_IBSR9_RACK    	(IO_UART9.I2C9.IBSR9.bit.RACK)
#define	UART9_I2C9_IBSR9_RSA    	(IO_UART9.I2C9.IBSR9.bit.RSA)
#define	UART9_I2C9_IBSR9_TRX    	(IO_UART9.I2C9.IBSR9.bit.TRX)
#define	UART9_I2C9_IBSR9_AL    	(IO_UART9.I2C9.IBSR9.bit.AL)
#define	UART9_I2C9_IBSR9_RSC    	(IO_UART9.I2C9.IBSR9.bit.RSC)
#define	UART9_I2C9_IBSR9_SPC    	(IO_UART9.I2C9.IBSR9.bit.SPC)
#define	UART9_I2C9_IBSR9_BB    	(IO_UART9.I2C9.IBSR9.bit.BB)
#define	UART9_I2C9_RDR9	(IO_UART9.I2C9.RDR9.hword)
#define	UART9_I2C9_RDR9_RDR9L	(IO_UART9.I2C9.RDR9.byte.RDR9L)
#define	UART9_I2C9_TDR9	(IO_UART9.I2C9.RDR9.hword)
#define	UART9_I2C9_TDR9_TDR9L	(IO_UART9.I2C9.RDR9.byte.RDR9L)
#define	UART9_I2C9_BGR9	(IO_UART9.I2C9.BGR9)
#define	UART9_I2C9_ISMK9			(IO_UART9.I2C9.ISMK9.byte)
#define	UART9_I2C9_ISMK9_EN    	(IO_UART9.I2C9.ISMK9.bit.EN)
#define	UART9_I2C9_ISMK9_SM    	(IO_UART9.I2C9.ISMK9.bit.SM)
#define	UART9_I2C9_ISBA9			(IO_UART9.I2C9.ISBA9.byte)
#define	UART9_I2C9_ISBA9_SAEN    	(IO_UART9.I2C9.ISBA9.bit.SAEN)
#define	UART9_I2C9_ISBA9_SA    	(IO_UART9.I2C9.ISBA9.bit.SA)
#define	UART9_I2C9_FCR19			(IO_UART9.I2C9.FCR19.byte)
#define	UART9_I2C9_FCR19_FLSTE    	(IO_UART9.I2C9.FCR19.bit.FLSTE)
#define	UART9_I2C9_FCR19_FRIIE    	(IO_UART9.I2C9.FCR19.bit.FRIIE)
#define	UART9_I2C9_FCR19_FDRQ    	(IO_UART9.I2C9.FCR19.bit.FDRQ)
#define	UART9_I2C9_FCR19_FTIE    	(IO_UART9.I2C9.FCR19.bit.FTIE)
#define	UART9_I2C9_FCR19_FSEL    	(IO_UART9.I2C9.FCR19.bit.FSEL)
#define	UART9_I2C9_FCR09			(IO_UART9.I2C9.FCR09.byte)
#define	UART9_I2C9_FCR09_FLST    	(IO_UART9.I2C9.FCR09.bit.FLST)
#define	UART9_I2C9_FCR09_FLD    	(IO_UART9.I2C9.FCR09.bit.FLD)
#define	UART9_I2C9_FCR09_FSET    	(IO_UART9.I2C9.FCR09.bit.FSET)
#define	UART9_I2C9_FCR09_FCL2    	(IO_UART9.I2C9.FCR09.bit.FCL2)
#define	UART9_I2C9_FCR09_FCL1    	(IO_UART9.I2C9.FCR09.bit.FCL1)
#define	UART9_I2C9_FCR09_FE2    	(IO_UART9.I2C9.FCR09.bit.FE2)
#define	UART9_I2C9_FCR09_FE1    	(IO_UART9.I2C9.FCR09.bit.FE1)
#define	UART9_I2C9_FBYTE9	(IO_UART9.I2C9.FBYTE9)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CLKG,attr=IO,locate=0x510
#endif

__IO_EXTENDED	struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	SCEN:1;
			__BYTE	PCEN:1;
			__BYTE	MCEN:1;
			__BYTE	RESV03:1;
			__BYTE	RESV04:1;
			__BYTE	RESV05:1;
			__BYTE	CKS:2;
		} bit;
	} IO_CSELR;
	const union {
		__BYTE	byte;
		struct {
			__BYTE	SCRDY:1;
			__BYTE	PCRDY:1;
			__BYTE	MCRDY:1;
			__BYTE	RESV13:1;
			__BYTE	RESV14:1;
			__BYTE	RESV15:1;
			__BYTE	CKM:2;
		} bit;
	} IO_CMONR;
	union {
		__BYTE	byte;
		struct {
			__BYTE	MTIF:1;
			__BYTE	MTIE:1;
			__BYTE	MTC:1;
			__BYTE	MTE:1;
			__BYTE	MTS:4;
		} bit;
	} IO_MTMCR;
	union {
		__BYTE	byte;
		struct {
			__BYTE	STIF:1;
			__BYTE	STIE:1;
			__BYTE	STC:1;
			__BYTE	STE:1;
			__BYTE	RESV34:1;
			__BYTE	STS:3;
		} bit;
	} IO_STMCR;
	union {
		__HWORD	hword;
		struct {
			__BYTE	PLLCRH;
			__BYTE	PLLCRL;
		} byte;
		struct {
			__HWORD	RESV40:1;
			__HWORD	RESV41:1;
			__HWORD	RESV42:1;
			__HWORD	RESV43:1;
			__HWORD	RESV44:1;
			__HWORD	RESV45:1;
			__HWORD	RESV46:1;
			__HWORD	RESV47:1;
			__HWORD	POSW:4;
			__HWORD	PDS:4;
		} bit;
	} IO_PLLCR;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV60:1;
			__BYTE	SOSW:3;
			__BYTE	MOSW:4;
		} bit;
	} IO_CSTBR;
	union {
		__BYTE	byte;
		struct {
			__BYTE	PTIF:1;
			__BYTE	PTIE:1;
			__BYTE	RESV72:1;
			__BYTE	RESV73:1;
			__BYTE	RESV74:1;
			__BYTE	RESV75:1;
			__BYTE	RESV76:1;
			__BYTE	RESV77:1;
		} bit;
	} IO_PTMCR;
} IO_CLKG;

#define	CLKG_CSELR			(IO_CLKG.IO_CSELR.byte)
#define	CLKG_CSELR_SCEN    	(IO_CLKG.IO_CSELR.bit.SCEN)
#define	CLKG_CSELR_PCEN    	(IO_CLKG.IO_CSELR.bit.PCEN)
#define	CLKG_CSELR_MCEN    	(IO_CLKG.IO_CSELR.bit.MCEN)
#define	CLKG_CSELR_CKS    	(IO_CLKG.IO_CSELR.bit.CKS)
#define	CLKG_CMONR			(IO_CLKG.IO_CMONR.byte)
#define	CLKG_CMONR_SCRDY    	(IO_CLKG.IO_CMONR.bit.SCRDY)
#define	CLKG_CMONR_PCRDY    	(IO_CLKG.IO_CMONR.bit.PCRDY)
#define	CLKG_CMONR_MCRDY    	(IO_CLKG.IO_CMONR.bit.MCRDY)
#define	CLKG_CMONR_CKM    	(IO_CLKG.IO_CMONR.bit.CKM)
#define	CLKG_MTMCR			(IO_CLKG.IO_MTMCR.byte)
#define	CLKG_MTMCR_MTIF    	(IO_CLKG.IO_MTMCR.bit.MTIF)
#define	CLKG_MTMCR_MTIE    	(IO_CLKG.IO_MTMCR.bit.MTIE)
#define	CLKG_MTMCR_MTC    	(IO_CLKG.IO_MTMCR.bit.MTC)
#define	CLKG_MTMCR_MTE    	(IO_CLKG.IO_MTMCR.bit.MTE)
#define	CLKG_MTMCR_MTS    	(IO_CLKG.IO_MTMCR.bit.MTS)
#define	CLKG_STMCR			(IO_CLKG.IO_STMCR.byte)
#define	CLKG_STMCR_STIF    	(IO_CLKG.IO_STMCR.bit.STIF)
#define	CLKG_STMCR_STIE    	(IO_CLKG.IO_STMCR.bit.STIE)
#define	CLKG_STMCR_STC    	(IO_CLKG.IO_STMCR.bit.STC)
#define	CLKG_STMCR_STE    	(IO_CLKG.IO_STMCR.bit.STE)
#define	CLKG_STMCR_STS    	(IO_CLKG.IO_STMCR.bit.STS)
#define	CLKG_PLLCR			(IO_CLKG.IO_PLLCR.hword)
#define	CLKG_PLLCR_PLLCRH			(IO_CLKG.IO_PLLCR.byte.PLLCRH)
#define	CLKG_PLLCR_PLLCRL			(IO_CLKG.IO_PLLCR.byte.PLLCRL)
#define	CLKG_PLLCR_POSW    	(IO_CLKG.IO_PLLCR.bit.POSW)
#define	CLKG_PLLCR_PDS    	(IO_CLKG.IO_PLLCR.bit.PDS)
#define	CLKG_CSTBR			(IO_CLKG.IO_CSTBR.byte)
#define	CLKG_CSTBR_SOSW    	(IO_CLKG.IO_CSTBR.bit.SOSW)
#define	CLKG_CSTBR_MOSW    	(IO_CLKG.IO_CSTBR.bit.MOSW)
#define	CLKG_PTMCR			(IO_CLKG.IO_PTMCR.byte)
#define	CLKG_PTMCR_PTIF    	(IO_CLKG.IO_PTMCR.bit.PTIF)
#define	CLKG_PTMCR_PTIE    	(IO_CLKG.IO_PTMCR.bit.PTIE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CPUAR,attr=IO,locate=0x51A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	PSTRE:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	PDLF:1;
			__BYTE	PMDF:1;
			__BYTE	PSTF:1;
			__BYTE	HWDF:1;
	} bit;
} CPUARSTR;

__IO_EXTENDED	  CPUARSTR	IO_CPUAR;
#define	_cpuar		(IO_CPUAR)
#define	CPUAR		(IO_CPUAR.byte)
#define	CPUAR_PSTRE  	(IO_CPUAR.bit.PSTRE)
#define	CPUAR_PDLF  	(IO_CPUAR.bit.PDLF)
#define	CPUAR_PMDF  	(IO_CPUAR.bit.PMDF)
#define	CPUAR_PSTF  	(IO_CPUAR.bit.PSTF)
#define	CPUAR_HWDF  	(IO_CPUAR.bit.HWDF)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CCTL,attr=IO,locate=0x520
#endif

__IO_EXTENDED	struct {
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV00:1;
			__BYTE	RESV01:1;
			__BYTE	RESV02:1;
			__BYTE	RESV03:1;
			__BYTE	RESV04:1;
			__BYTE	RESV05:1;
			__BYTE	RESV06:1;
			__BYTE	PCSEL:1;
		} bit;
	} IO_CCPSSELR;
	__BYTE	IO_RESV1;
	__BYTE	IO_RESV2;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV30:1;
			__BYTE	PODS:3;
			__BYTE	RESV32:1;
			__BYTE	SODS:3;
		} bit;
	} IO_CCPSDIVR;
	__BYTE	IO_RESV3;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV50:1;
			__BYTE	IDIV:7;
		} bit;
	} IO_CCPLLFBR;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV60:1;
			__BYTE	RESV61:1;
			__BYTE	NDIV:6;
		} bit;
	} IO_CCSSFBR0;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV70:1;
			__BYTE	RESV71:1;
			__BYTE	RESV72:1;
			__BYTE	PDIV:5;
		} bit;
	} IO_CCSSFBR1;
	__BYTE	IO_RESV4;
	union {
		__BYTE	byte;
		struct {
			__BYTE	RESV90:1;
			__BYTE	RESV91:1;
			__BYTE	RESV92:1;
			__BYTE	RESV93:1;
			__BYTE	SFREQ:2;
			__BYTE	SMODE:1;
			__BYTE	SSEN:1;
		} bit;
	} IO_CCSSCCR0;
	union {
		__HWORD	hword;
		struct {
			__HWORD	RATESEL:3;
			__HWORD	RESV101:1;
			__HWORD	RESV102:1;
			__HWORD	RESV103:1;
			__HWORD	RESV104:1;
			__HWORD	RESV105:1;
			__HWORD	RESV106:1;
			__HWORD	RESV107:1;
			__HWORD	RESV108:1;
			__HWORD	RESV109:1;
			__HWORD	RESV1010:1;
			__HWORD	RESV1011:1;
			__HWORD	RESV1012:1;
			__HWORD	RESV1013:1;
		} bit;
	} IO_CCSSCCR1;
	__BYTE	IO_RESV5;
	union {
		__BYTE	byte;
		struct {
			__BYTE	GRSTS:2;
			__BYTE	RESV131:1;
			__BYTE	RESV132:1;
			__BYTE	RESV133:1;
			__BYTE	RESV134:1;
			__BYTE	GRSTR:1;
			__BYTE	GREN:1;
		} bit;
	} IO_CCCGRCR0;
	union {
		__BYTE	byte;
		struct {
			__BYTE	GRSTP:2;
			__BYTE	GRSTN:6;
		} bit;
	} IO_CCCGRCR1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	GRLP:8;
		} bit;
	} IO_CCCGRCR2;
	union {
		__BYTE	byte;
		struct {
			__BYTE	CST:1;
			__BYTE	RESV161:1;
			__BYTE	RESV162:1;
			__BYTE	RESV163:1;
			__BYTE	RESV164:1;
			__BYTE	RESV165:1;
			__BYTE	RESV166:1;
			__BYTE	CSC:1;
		} bit;
	} IO_CCRTSELR;
	__BYTE	IO_RESV6;
	union {
		__BYTE	byte;
		struct {
			__BYTE	FST:1;
			__BYTE	RESV181:1;
			__BYTE	RESV182:1;
			__BYTE	RESV183:1;
			__BYTE	RESV184:1;
			__BYTE	RESV185:1;
			__BYTE	FDIV:2;
		} bit;
	} IO_CCPMUCR0;
	union {
		__BYTE	byte;
		struct {
			__BYTE	GST:1;
			__BYTE	RESV191:1;
			__BYTE	RESV192:1;
			__BYTE	GDIV:5;
		} bit;
	} IO_CCPMUCR1;
	__WORD	IO_RESV7;
	__WORD	IO_RESV8;
	__WORD	IO_RESV9;
} IO_CCTL;

#define	CCTL_CCPSSELR			(IO_CCTL.IO_CCPSSELR.byte)
#define	CCTL_CCPSSELR_PCSEL    	(IO_CCTL.IO_CCPSSELR.bit.PCSEL)
#define	CCTL_CCPSDIVR			(IO_CCTL.IO_CCPSDIVR.byte)
#define	CCTL_CCPSDIVR_PODS    	(IO_CCTL.IO_CCPSDIVR.bit.PODS)
#define	CCTL_CCPSDIVR_SODS    	(IO_CCTL.IO_CCPSDIVR.bit.SODS)
#define	CCTL_CCPLLFBR			(IO_CCTL.IO_CCPLLFBR.byte)
#define	CCTL_CCPLLFBR_IDIV    	(IO_CCTL.IO_CCPLLFBR.bit.IDIV)
#define	CCTL_CCSSFBR0			(IO_CCTL.IO_CCSSFBR0.byte)
#define	CCTL_CCSSFBR0_NDIV    	(IO_CCTL.IO_CCSSFBR0.bit.NDIV)
#define	CCTL_CCSSFBR1			(IO_CCTL.IO_CCSSFBR1.byte)
#define	CCTL_CCSSFBR1_PDIV    	(IO_CCTL.IO_CCSSFBR1.bit.PDIV)
#define	CCTL_CCSSCCR0			(IO_CCTL.IO_CCSSCCR0.byte)
#define	CCTL_CCSSCCR0_SFREQ    	(IO_CCTL.IO_CCSSCCR0.bit.SFREQ)
#define	CCTL_CCSSCCR0_SMODE    	(IO_CCTL.IO_CCSSCCR0.bit.SMODE)
#define	CCTL_CCSSCCR0_SSEN    	(IO_CCTL.IO_CCSSCCR0.bit.SSEN)
#define	CCTL_CCSSCCR1			(IO_CCTL.IO_CCSSCCR1.hword)
#define	CCTL_CCSSCCR1_RATESEL    	(IO_CCTL.IO_CCSSCCR1.bit.RATESEL)
#define	CCTL_CCCGRCR0			(IO_CCTL.IO_CCCGRCR0.byte)
#define	CCTL_CCCGRCR0_GRSTS    	(IO_CCTL.IO_CCCGRCR0.bit.GRSTS)
#define	CCTL_CCCGRCR0_GRSTR    	(IO_CCTL.IO_CCCGRCR0.bit.GRSTR)
#define	CCTL_CCCGRCR0_GREN    	(IO_CCTL.IO_CCCGRCR0.bit.GREN)
#define	CCTL_CCCGRCR1			(IO_CCTL.IO_CCCGRCR1.byte)
#define	CCTL_CCCGRCR1_GRSTP    	(IO_CCTL.IO_CCCGRCR1.bit.GRSTP)
#define	CCTL_CCCGRCR1_GRSTN    	(IO_CCTL.IO_CCCGRCR1.bit.GRSTN)
#define	CCTL_CCCGRCR2			(IO_CCTL.IO_CCCGRCR2.byte)
#define	CCTL_CCCGRCR2_GRLP    	(IO_CCTL.IO_CCCGRCR2.bit.GRLP)
#define	CCTL_CCRTSELR			(IO_CCTL.IO_CCRTSELR.byte)
#define	CCTL_CCRTSELR_CST    	(IO_CCTL.IO_CCRTSELR.bit.CST)
#define	CCTL_CCRTSELR_CSC    	(IO_CCTL.IO_CCRTSELR.bit.CSC)
#define	CCTL_CCPMUCR0			(IO_CCTL.IO_CCPMUCR0.byte)
#define	CCTL_CCPMUCR0_FST    	(IO_CCTL.IO_CCPMUCR0.bit.FST)
#define	CCTL_CCPMUCR0_FDIV    	(IO_CCTL.IO_CCPMUCR0.bit.FDIV)
#define	CCTL_CCPMUCR1			(IO_CCTL.IO_CCPMUCR1.byte)
#define	CCTL_CCPMUCR1_GST    	(IO_CCTL.IO_CCPMUCR1.bit.GST)
#define	CCTL_CCPMUCR1_GDIV    	(IO_CCTL.IO_CCPMUCR1.bit.GDIV)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EIRR0,attr=IO,locate=0x550
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ER7:1;
			__BYTE	ER6:1;
			__BYTE	ER5:1;
			__BYTE	ER4:1;
			__BYTE	ER3:1;
			__BYTE	ER2:1;
			__BYTE	ER1:1;
			__BYTE	ER0:1;
	} bit;
} EIRR0STR;

__IO_EXTENDED	  EIRR0STR	IO_EIRR0;
#define	_eirr0		(IO_EIRR0)
#define	EIRR0		(IO_EIRR0.byte)
#define	EIRR0_ER7  	(IO_EIRR0.bit.ER7)
#define	EIRR0_ER6  	(IO_EIRR0.bit.ER6)
#define	EIRR0_ER5  	(IO_EIRR0.bit.ER5)
#define	EIRR0_ER4  	(IO_EIRR0.bit.ER4)
#define	EIRR0_ER3  	(IO_EIRR0.bit.ER3)
#define	EIRR0_ER2  	(IO_EIRR0.bit.ER2)
#define	EIRR0_ER1  	(IO_EIRR0.bit.ER1)
#define	EIRR0_ER0  	(IO_EIRR0.bit.ER0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ENIR0,attr=IO,locate=0x551
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	EN7:1;
			__BYTE	EN6:1;
			__BYTE	EN5:1;
			__BYTE	EN4:1;
			__BYTE	EN3:1;
			__BYTE	EN2:1;
			__BYTE	EN1:1;
			__BYTE	EN0:1;
	} bit;
} ENIR0STR;

__IO_EXTENDED	  ENIR0STR	IO_ENIR0;
#define	_enir0		(IO_ENIR0)
#define	ENIR0		(IO_ENIR0.byte)
#define	ENIR0_EN7  	(IO_ENIR0.bit.EN7)
#define	ENIR0_EN6  	(IO_ENIR0.bit.EN6)
#define	ENIR0_EN5  	(IO_ENIR0.bit.EN5)
#define	ENIR0_EN4  	(IO_ENIR0.bit.EN4)
#define	ENIR0_EN3  	(IO_ENIR0.bit.EN3)
#define	ENIR0_EN2  	(IO_ENIR0.bit.EN2)
#define	ENIR0_EN1  	(IO_ENIR0.bit.EN1)
#define	ENIR0_EN0  	(IO_ENIR0.bit.EN0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ELVR0,attr=IO,locate=0x552
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	LBLA7:2;
			__HWORD	LBLA6:2;
			__HWORD	LBLA5:2;
			__HWORD	LBLA4:2;
			__HWORD	LBLA3:2;
			__HWORD	LBLA2:2;
			__HWORD	LBLA1:2;
			__HWORD	LBLA0:2;
	} bit;
} ELVR0STR;

__IO_EXTENDED	  ELVR0STR	IO_ELVR0;
#define	_elvr0		(IO_ELVR0)
#define	ELVR0		(IO_ELVR0.hword)
#define	ELVR0_LBLA7  	(IO_ELVR0.bit.LBLA7)
#define	ELVR0_LBLA6  	(IO_ELVR0.bit.LBLA6)
#define	ELVR0_LBLA5  	(IO_ELVR0.bit.LBLA5)
#define	ELVR0_LBLA4  	(IO_ELVR0.bit.LBLA4)
#define	ELVR0_LBLA3  	(IO_ELVR0.bit.LBLA3)
#define	ELVR0_LBLA2  	(IO_ELVR0.bit.LBLA2)
#define	ELVR0_LBLA1  	(IO_ELVR0.bit.LBLA1)
#define	ELVR0_LBLA0  	(IO_ELVR0.bit.LBLA0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EIRR1,attr=IO,locate=0x554
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ER7:1;
			__BYTE	ER6:1;
			__BYTE	ER5:1;
			__BYTE	ER4:1;
			__BYTE	ER3:1;
			__BYTE	ER2:1;
			__BYTE	ER1:1;
			__BYTE	ER0:1;
	} bit;
} EIRR1STR;

__IO_EXTENDED	  EIRR1STR	IO_EIRR1;
#define	_eirr1		(IO_EIRR1)
#define	EIRR1		(IO_EIRR1.byte)
#define	EIRR1_ER7  	(IO_EIRR1.bit.ER7)
#define	EIRR1_ER6  	(IO_EIRR1.bit.ER6)
#define	EIRR1_ER5  	(IO_EIRR1.bit.ER5)
#define	EIRR1_ER4  	(IO_EIRR1.bit.ER4)
#define	EIRR1_ER3  	(IO_EIRR1.bit.ER3)
#define	EIRR1_ER2  	(IO_EIRR1.bit.ER2)
#define	EIRR1_ER1  	(IO_EIRR1.bit.ER1)
#define	EIRR1_ER0  	(IO_EIRR1.bit.ER0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ENIR1,attr=IO,locate=0x555
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	EN7:1;
			__BYTE	EN6:1;
			__BYTE	EN5:1;
			__BYTE	EN4:1;
			__BYTE	EN3:1;
			__BYTE	EN2:1;
			__BYTE	EN1:1;
			__BYTE	EN0:1;
	} bit;
} ENIR1STR;

__IO_EXTENDED	  ENIR1STR	IO_ENIR1;
#define	_enir1		(IO_ENIR1)
#define	ENIR1		(IO_ENIR1.byte)
#define	ENIR1_EN7  	(IO_ENIR1.bit.EN7)
#define	ENIR1_EN6  	(IO_ENIR1.bit.EN6)
#define	ENIR1_EN5  	(IO_ENIR1.bit.EN5)
#define	ENIR1_EN4  	(IO_ENIR1.bit.EN4)
#define	ENIR1_EN3  	(IO_ENIR1.bit.EN3)
#define	ENIR1_EN2  	(IO_ENIR1.bit.EN2)
#define	ENIR1_EN1  	(IO_ENIR1.bit.EN1)
#define	ENIR1_EN0  	(IO_ENIR1.bit.EN0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ELVR1,attr=IO,locate=0x556
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	LBLA7:2;
			__HWORD	LBLA6:2;
			__HWORD	LBLA5:2;
			__HWORD	LBLA4:2;
			__HWORD	LBLA3:2;
			__HWORD	LBLA2:2;
			__HWORD	LBLA1:2;
			__HWORD	LBLA0:2;
	} bit;
} ELVR1STR;

__IO_EXTENDED	  ELVR1STR	IO_ELVR1;
#define	_elvr1		(IO_ELVR1)
#define	ELVR1		(IO_ELVR1.hword)
#define	ELVR1_LBLA7  	(IO_ELVR1.bit.LBLA7)
#define	ELVR1_LBLA6  	(IO_ELVR1.bit.LBLA6)
#define	ELVR1_LBLA5  	(IO_ELVR1.bit.LBLA5)
#define	ELVR1_LBLA4  	(IO_ELVR1.bit.LBLA4)
#define	ELVR1_LBLA3  	(IO_ELVR1.bit.LBLA3)
#define	ELVR1_LBLA2  	(IO_ELVR1.bit.LBLA2)
#define	ELVR1_LBLA1  	(IO_ELVR1.bit.LBLA1)
#define	ELVR1_LBLA0  	(IO_ELVR1.bit.LBLA0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_WTDR,attr=IO,locate=0x55E
#endif

__IO_EXTENDED	__HWORD	IO_WTDR;
#define	_wtdr		(IO_WTDR)
#define	WTDR	(_wtdr)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_WTCRH,attr=IO,locate=0x561
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	INTE4:1;
			__BYTE	INT4:1;
	} bit;
} WTCRHSTR;

__IO_EXTENDED	  WTCRHSTR	IO_WTCRH;
#define	_wtcrh		(IO_WTCRH)
#define	WTCRH		(IO_WTCRH.byte)
#define	WTCRH_INTE4  	(IO_WTCRH.bit.INTE4)
#define	WTCRH_INT4  	(IO_WTCRH.bit.INT4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_WTCRM,attr=IO,locate=0x562
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	INTE3:1;
			__BYTE	INT3:1;
			__BYTE	INTE2:1;
			__BYTE	INT2:1;
			__BYTE	INTE1:1;
			__BYTE	INT1:1;
			__BYTE	INTE0:1;
			__BYTE	INT0:1;
	} bit;
} WTCRMSTR;

__IO_EXTENDED	  WTCRMSTR	IO_WTCRM;
#define	_wtcrm		(IO_WTCRM)
#define	WTCRM		(IO_WTCRM.byte)
#define	WTCRM_INTE3  	(IO_WTCRM.bit.INTE3)
#define	WTCRM_INT3  	(IO_WTCRM.bit.INT3)
#define	WTCRM_INTE2  	(IO_WTCRM.bit.INTE2)
#define	WTCRM_INT2  	(IO_WTCRM.bit.INT2)
#define	WTCRM_INTE1  	(IO_WTCRM.bit.INTE1)
#define	WTCRM_INT1  	(IO_WTCRM.bit.INT1)
#define	WTCRM_INTE0  	(IO_WTCRM.bit.INTE0)
#define	WTCRM_INT0  	(IO_WTCRM.bit.INT0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_WTCRL,attr=IO,locate=0x563
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RUN:1;
			__BYTE	UPDT:1;
			__BYTE	RESV6:1;
			__BYTE	ST:1;
	} bit;
} WTCRLSTR;

__IO_EXTENDED	  WTCRLSTR	IO_WTCRL;
#define	_wtcrl		(IO_WTCRL)
#define	WTCRL		(IO_WTCRL.byte)
#define	WTCRL_RUN  	(IO_WTCRL.bit.RUN)
#define	WTCRL_UPDT  	(IO_WTCRL.bit.UPDT)
#define	WTCRL_ST  	(IO_WTCRL.bit.ST)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_WTBRH,attr=IO,locate=0x565
#endif

__IO_EXTENDED	__BYTE	IO_WTBRH;
#define	_wtbrh		(IO_WTBRH)
#define	WTBRH	(_wtbrh)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_WTBRM,attr=IO,locate=0x566
#endif

__IO_EXTENDED	__BYTE	IO_WTBRM;
#define	_wtbrm		(IO_WTBRM)
#define	WTBRM	(_wtbrm)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_WTBRL,attr=IO,locate=0x567
#endif

__IO_EXTENDED	__BYTE	IO_WTBRL;
#define	_wtbrl		(IO_WTBRL)
#define	WTBRL	(_wtbrl)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_WTHR,attr=IO,locate=0x568
#endif

__IO_EXTENDED	__BYTE	IO_WTHR;
#define	_wthr		(IO_WTHR)
#define	WTHR	(_wthr)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_WTMR,attr=IO,locate=0x569
#endif

__IO_EXTENDED	__BYTE	IO_WTMR;
#define	_wtmr		(IO_WTMR)
#define	WTMR	(_wtmr)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_WTSR,attr=IO,locate=0x56A
#endif

__IO_EXTENDED	__BYTE	IO_WTSR;
#define	_wtsr		(IO_WTSR)
#define	WTSR	(_wtsr)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CSVCR,attr=IO,locate=0x56D
#endif

__IO_EXTENDED	__BYTE	IO_CSVCR;
#define	_csvcr		(IO_CSVCR)
#define	CSVCR	(_csvcr)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_REGSEL,attr=IO,locate=0x580
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	MRSEL:2;
			__BYTE	SRSEL:2;
			__BYTE	STRSEL:3;
			__BYTE	RESV3:1;
	} bit;
} REGSELSTR;

__IO_EXTENDED	  REGSELSTR	IO_REGSEL;
#define	_regsel		(IO_REGSEL)
#define	REGSEL		(IO_REGSEL.byte)
#define	REGSEL_MRSEL  	(IO_REGSEL.bit.MRSEL)
#define	REGSEL_SRSEL  	(IO_REGSEL.bit.SRSEL)
#define	REGSEL_STRSEL  	(IO_REGSEL.bit.STRSEL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_LVD5R,attr=IO,locate=0x584
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	LVD5R_F:1;
	} bit;
} LVD5RSTR;

__IO_EXTENDED	  LVD5RSTR	IO_LVD5R;
#define	_lvd5r		(IO_LVD5R)
#define	LVD5R		(IO_LVD5R.byte)
#define	LVD5R_LVD5R_F  	(IO_LVD5R.bit.LVD5R_F)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_LVD5F,attr=IO,locate=0x585
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	LVD5F_PD:1;
			__BYTE	RESV1:1;
			__BYTE	LVD5F_SEL:2;
			__BYTE	LVD5F_OE:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	LVD5F_F:1;
	} bit;
} LVD5FSTR;

__IO_EXTENDED	  LVD5FSTR	IO_LVD5F;
#define	_lvd5f		(IO_LVD5F)
#define	LVD5F		(IO_LVD5F.byte)
#define	LVD5F_LVD5F_PD  	(IO_LVD5F.bit.LVD5F_PD)
#define	LVD5F_LVD5F_SEL  	(IO_LVD5F.bit.LVD5F_SEL)
#define	LVD5F_LVD5F_OE  	(IO_LVD5F.bit.LVD5F_OE)
#define	LVD5F_LVD5F_F  	(IO_LVD5F.bit.LVD5F_F)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_LVD,attr=IO,locate=0x586
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	LVD_PD:1;
			__BYTE	LVD_SEL:3;
			__BYTE	LVD_OE:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	LVD_F:1;
	} bit;
} LVDSTR;

__IO_EXTENDED	  LVDSTR	IO_LVD;
#define	_lvd		(IO_LVD)
#define	LVD		(IO_LVD.byte)
#define	LVD_LVD_PD  	(IO_LVD.bit.LVD_PD)
#define	LVD_LVD_SEL  	(IO_LVD.bit.LVD_SEL)
#define	LVD_LVD_OE  	(IO_LVD.bit.LVD_OE)
#define	LVD_LVD_F  	(IO_LVD.bit.LVD_F)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PMUSTR,attr=IO,locate=0x590
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	PMUST:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} PMUSTRSTR;

__IO_EXTENDED	  PMUSTRSTR	IO_PMUSTR;
#define	_pmustr		(IO_PMUSTR)
#define	PMUSTR		(IO_PMUSTR.byte)
#define	PMUSTR_PMUST  	(IO_PMUSTR.bit.PMUST)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PMUCTLR,attr=IO,locate=0x591
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	SHDE:1;
			__BYTE	RESV1:1;
			__BYTE	IOCTMD:1;
			__BYTE	IOCT:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} PMUCTLRSTR;

__IO_EXTENDED	  PMUCTLRSTR	IO_PMUCTLR;
#define	_pmuctlr		(IO_PMUCTLR)
#define	PMUCTLR		(IO_PMUCTLR.byte)
#define	PMUCTLR_SHDE  	(IO_PMUCTLR.bit.SHDE)
#define	PMUCTLR_IOCTMD  	(IO_PMUCTLR.bit.IOCTMD)
#define	PMUCTLR_IOCT  	(IO_PMUCTLR.bit.IOCT)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PWRTMCTL,attr=IO,locate=0x592
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	PTC:3;
	} bit;
} PWRTMCTLSTR;

__IO_EXTENDED	  PWRTMCTLSTR	IO_PWRTMCTL;
#define	_pwrmctl		(IO_PWRTMCTL)
#define	PWRTMCTL		(IO_PWRTMCTL.byte)
#define	PWRTMCTL_PTC  	(IO_PWRTMCTL.bit.PTC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PMUINTF0,attr=IO,locate=0x594
#endif

__IO_EXTENDED	__BYTE	IO_PMUINTF0;
#define	_pmuintf0		(IO_PMUINTF0)
#define	PMUINTF0	(_pmuintf0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PMUINTF1,attr=IO,locate=0x595
#endif

__IO_EXTENDED	__BYTE	IO_PMUINTF1;
#define	_pmuintf1		(IO_PMUINTF1)
#define	PMUINTF1	(_pmuintf1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PMUINTF2,attr=IO,locate=0x596
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RIF:1;
			__BYTE	NIF:1;
			__BYTE	MTIF:1;
			__BYTE	STIF:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} PMUINTF2STR;

__IO_EXTENDED	  PMUINTF2STR	IO_PMUINTF2;
#define	_pmuintf2		(IO_PMUINTF2)
#define	PMUINTF2		(IO_PMUINTF2.byte)
#define	PMUINTF2_RIF  	(IO_PMUINTF2.bit.RIF)
#define	PMUINTF2_NIF  	(IO_PMUINTF2.bit.NIF)
#define	PMUINTF2_MTIF  	(IO_PMUINTF2.bit.MTIF)
#define	PMUINTF2_STIF  	(IO_PMUINTF2.bit.STIF)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_LCDCMR,attr=IO,locate=0x5A8
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	DTCH:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	RESV7:1;
	} bit;
} LCDCMRSTR;

__IO_EXTENDED	  LCDCMRSTR	IO_LCDCMR;
#define	_lcdcmr		(IO_LCDCMR)
#define	LCDCMR		(IO_LCDCMR.byte)
#define	LCDCMR_DTCH  	(IO_LCDCMR.bit.DTCH)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_LCRS,attr=IO,locate=0x5A9
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	SCSS:1;
			__BYTE	LCSEN:1;
			__BYTE	LCS:4;
			__BYTE	FPS:2;
	} bit;
} LCRSSTR;

__IO_EXTENDED	  LCRSSTR	IO_LCRS;
#define	_lcrs		(IO_LCRS)
#define	LCRS		(IO_LCRS.byte)
#define	LCRS_SCSS  	(IO_LCRS.bit.SCSS)
#define	LCRS_LCSEN  	(IO_LCRS.bit.LCSEN)
#define	LCRS_LCS  	(IO_LCRS.bit.LCS)
#define	LCRS_FPS  	(IO_LCRS.bit.FPS)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_LCR0,attr=IO,locate=0x5AA
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	CSS:1;
			__BYTE	LCEN:1;
			__BYTE	VSEL:1;
			__BYTE	BK:1;
			__BYTE	MS:2;
			__BYTE	FP:2;
	} bit;
} LCR0STR;

__IO_EXTENDED	  LCR0STR	IO_LCR0;
#define	_lcr0		(IO_LCR0)
#define	LCR0		(IO_LCR0.byte)
#define	LCR0_CSS  	(IO_LCR0.bit.CSS)
#define	LCR0_LCEN  	(IO_LCR0.bit.LCEN)
#define	LCR0_VSEL  	(IO_LCR0.bit.VSEL)
#define	LCR0_BK  	(IO_LCR0.bit.BK)
#define	LCR0_MS  	(IO_LCR0.bit.MS)
#define	LCR0_FP  	(IO_LCR0.bit.FP)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_LCR1,attr=IO,locate=0x5AB
#endif

__IO_EXTENDED	__BYTE	IO_LCR1;
#define	_lcr1		(IO_LCR1)
#define	LCR1	(_lcr1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM0,attr=IO,locate=0x5AC
#endif

__IO_EXTENDED	__BYTE	IO_VRAM0;
#define	_vram0		(IO_VRAM0)
#define	VRAM0	(_vram0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM1,attr=IO,locate=0x5AD
#endif

__IO_EXTENDED	__BYTE	IO_VRAM1;
#define	_vram1		(IO_VRAM1)
#define	VRAM1	(_vram1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM2,attr=IO,locate=0x5AE
#endif

__IO_EXTENDED	__BYTE	IO_VRAM2;
#define	_vram2		(IO_VRAM2)
#define	VRAM2	(_vram2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM3,attr=IO,locate=0x5AF
#endif

__IO_EXTENDED	__BYTE	IO_VRAM3;
#define	_vram3		(IO_VRAM3)
#define	VRAM3	(_vram3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM4,attr=IO,locate=0x5B0
#endif

__IO_EXTENDED	__BYTE	IO_VRAM4;
#define	_vram4		(IO_VRAM4)
#define	VRAM4	(_vram4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM5,attr=IO,locate=0x5B1
#endif

__IO_EXTENDED	__BYTE	IO_VRAM5;
#define	_vram5		(IO_VRAM5)
#define	VRAM5	(_vram5)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM6,attr=IO,locate=0x5B2
#endif

__IO_EXTENDED	__BYTE	IO_VRAM6;
#define	_vram6		(IO_VRAM6)
#define	VRAM6	(_vram6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM7,attr=IO,locate=0x5B3
#endif

__IO_EXTENDED	__BYTE	IO_VRAM7;
#define	_vram7		(IO_VRAM7)
#define	VRAM7	(_vram7)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM8,attr=IO,locate=0x5B4
#endif

__IO_EXTENDED	__BYTE	IO_VRAM8;
#define	_vram8		(IO_VRAM8)
#define	VRAM8	(_vram8)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM9,attr=IO,locate=0x5B5
#endif

__IO_EXTENDED	__BYTE	IO_VRAM9;
#define	_vram9		(IO_VRAM9)
#define	VRAM9	(_vram9)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM10,attr=IO,locate=0x5B6
#endif

__IO_EXTENDED	__BYTE	IO_VRAM10;
#define	_vram10		(IO_VRAM10)
#define	VRAM10	(_vram10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM11,attr=IO,locate=0x5B7
#endif

__IO_EXTENDED	__BYTE	IO_VRAM11;
#define	_vram11		(IO_VRAM11)
#define	VRAM11	(_vram11)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM12,attr=IO,locate=0x5B8
#endif

__IO_EXTENDED	__BYTE	IO_VRAM12;
#define	_vram12		(IO_VRAM12)
#define	VRAM12	(_vram12)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM13,attr=IO,locate=0x5B9
#endif

__IO_EXTENDED	__BYTE	IO_VRAM13;
#define	_vram13		(IO_VRAM13)
#define	VRAM13	(_vram13)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM14,attr=IO,locate=0x5BA
#endif

__IO_EXTENDED	__BYTE	IO_VRAM14;
#define	_vram14		(IO_VRAM14)
#define	VRAM14	(_vram14)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_VRAM15,attr=IO,locate=0x5BB
#endif

__IO_EXTENDED	__BYTE	IO_VRAM15;
#define	_vram15		(IO_VRAM15)
#define	VRAM15	(_vram15)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_LDR,attr=IO,locate=0x5BC
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	ST:9;
	} bit;
} LDRSTR;

__IO_EXTENDED	  LDRSTR	IO_LDR;
#define	_ldr		(IO_LDR)
#define	LDR		(IO_LDR.hword)
#define	LDR_ST  	(IO_LDR.bit.ST)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ASR0,attr=IO,locate=0x600
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	SADR1:16;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	RESV5:1;
			__WORD	RESV6:1;
			__WORD	RESV7:1;
			__WORD	RESV8:1;
			__WORD	ASZ:4;
			__WORD	RESV10:1;
			__WORD	WREN:1;
			__WORD	LEDN:1;
			__WORD	CSEN:1;
	} bit;
} ASR0STR;

__IO_EXTENDED	  ASR0STR	IO_ASR0;
#define	_asr0		(IO_ASR0)
#define	ASR0		(IO_ASR0.word)
#define	ASR0_SADR1  	(IO_ASR0.bit.SADR1)
#define	ASR0_ASZ  	(IO_ASR0.bit.ASZ)
#define	ASR0_WREN  	(IO_ASR0.bit.WREN)
#define	ASR0_LEDN  	(IO_ASR0.bit.LEDN)
#define	ASR0_CSEN  	(IO_ASR0.bit.CSEN)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ASR1,attr=IO,locate=0x604
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	SADR1:16;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	RESV5:1;
			__WORD	RESV6:1;
			__WORD	RESV7:1;
			__WORD	RESV8:1;
			__WORD	ASZ:4;
			__WORD	RESV10:1;
			__WORD	WREN:1;
			__WORD	LEDN:1;
			__WORD	CSEN:1;
	} bit;
} ASR1STR;

__IO_EXTENDED	  ASR1STR	IO_ASR1;
#define	_asr1		(IO_ASR1)
#define	ASR1		(IO_ASR1.word)
#define	ASR1_SADR1  	(IO_ASR1.bit.SADR1)
#define	ASR1_ASZ  	(IO_ASR1.bit.ASZ)
#define	ASR1_WREN  	(IO_ASR1.bit.WREN)
#define	ASR1_LEDN  	(IO_ASR1.bit.LEDN)
#define	ASR1_CSEN  	(IO_ASR1.bit.CSEN)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ASR2,attr=IO,locate=0x608
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	SADR1:16;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	RESV5:1;
			__WORD	RESV6:1;
			__WORD	RESV7:1;
			__WORD	RESV8:1;
			__WORD	ASZ:4;
			__WORD	RESV10:1;
			__WORD	WREN:1;
			__WORD	LEDN:1;
			__WORD	CSEN:1;
	} bit;
} ASR2STR;

__IO_EXTENDED	  ASR2STR	IO_ASR2;
#define	_asr2		(IO_ASR2)
#define	ASR2		(IO_ASR2.word)
#define	ASR2_SADR1  	(IO_ASR2.bit.SADR1)
#define	ASR2_ASZ  	(IO_ASR2.bit.ASZ)
#define	ASR2_WREN  	(IO_ASR2.bit.WREN)
#define	ASR2_LEDN  	(IO_ASR2.bit.LEDN)
#define	ASR2_CSEN  	(IO_ASR2.bit.CSEN)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ASR3,attr=IO,locate=0x60C
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	SADR1:16;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	RESV5:1;
			__WORD	RESV6:1;
			__WORD	RESV7:1;
			__WORD	RESV8:1;
			__WORD	ASZ:4;
			__WORD	RESV10:1;
			__WORD	WREN:1;
			__WORD	LEDN:1;
			__WORD	CSEN:1;
	} bit;
} ASR3STR;

__IO_EXTENDED	  ASR3STR	IO_ASR3;
#define	_asr3		(IO_ASR3)
#define	ASR3		(IO_ASR3.word)
#define	ASR3_SADR1  	(IO_ASR3.bit.SADR1)
#define	ASR3_ASZ  	(IO_ASR3.bit.ASZ)
#define	ASR3_WREN  	(IO_ASR3.bit.WREN)
#define	ASR3_LEDN  	(IO_ASR3.bit.LEDN)
#define	ASR3_CSEN  	(IO_ASR3.bit.CSEN)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ACR0,attr=IO,locate=0x640
#endif

__IO_EXTENDED	__WORD	IO_ACR0;
#define	_acr0		(IO_ACR0)
#define	ACR0	(_acr0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ACR1,attr=IO,locate=0x644
#endif

__IO_EXTENDED	__WORD	IO_ACR1;
#define	_acr1		(IO_ACR1)
#define	ACR1	(_acr1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ACR2,attr=IO,locate=0x648
#endif

__IO_EXTENDED	__WORD	IO_ACR2;
#define	_acr2		(IO_ACR2)
#define	ACR2	(_acr2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ACR3,attr=IO,locate=0x64C
#endif

__IO_EXTENDED	__WORD	IO_ACR3;
#define	_acr3		(IO_ACR3)
#define	ACR3	(_acr3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_AWR0,attr=IO,locate=0x680
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	RESV0:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RWT:4;
			__WORD	WWT:4;
			__WORD	RIDL:2;
			__WORD	WRCV:2;
			__WORD	CSRD:2;
			__WORD	RDCS:2;
			__WORD	CSWR:2;
			__WORD	WRCS:2;
			__WORD	ADCY:2;
			__WORD	ACS:2;
			__WORD	ASCY:1;
			__WORD	RESV15:1;
			__WORD	RDYE:1;
			__WORD	RESV17:1;
	} bit;
} AWR0STR;

__IO_EXTENDED	  AWR0STR	IO_AWR0;
#define	_awr0		(IO_AWR0)
#define	AWR0		(IO_AWR0.word)
#define	AWR0_RWT  	(IO_AWR0.bit.RWT)
#define	AWR0_WWT  	(IO_AWR0.bit.WWT)
#define	AWR0_RIDL  	(IO_AWR0.bit.RIDL)
#define	AWR0_WRCV  	(IO_AWR0.bit.WRCV)
#define	AWR0_CSRD  	(IO_AWR0.bit.CSRD)
#define	AWR0_RDCS  	(IO_AWR0.bit.RDCS)
#define	AWR0_CSWR  	(IO_AWR0.bit.CSWR)
#define	AWR0_WRCS  	(IO_AWR0.bit.WRCS)
#define	AWR0_ADCY  	(IO_AWR0.bit.ADCY)
#define	AWR0_ACS  	(IO_AWR0.bit.ACS)
#define	AWR0_ASCY  	(IO_AWR0.bit.ASCY)
#define	AWR0_RDYE  	(IO_AWR0.bit.RDYE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_AWR1,attr=IO,locate=0x684
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	RESV0:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RWT:4;
			__WORD	WWT:4;
			__WORD	RIDL:2;
			__WORD	WRCV:2;
			__WORD	CSRD:2;
			__WORD	RDCS:2;
			__WORD	CSWR:2;
			__WORD	WRCS:2;
			__WORD	ADCY:2;
			__WORD	ACS:2;
			__WORD	ASCY:1;
			__WORD	RESV15:1;
			__WORD	RDYE:1;
			__WORD	RESV17:1;
	} bit;
} AWR1STR;

__IO_EXTENDED	  AWR1STR	IO_AWR1;
#define	_awr1		(IO_AWR1)
#define	AWR1		(IO_AWR1.word)
#define	AWR1_RWT  	(IO_AWR1.bit.RWT)
#define	AWR1_WWT  	(IO_AWR1.bit.WWT)
#define	AWR1_RIDL  	(IO_AWR1.bit.RIDL)
#define	AWR1_WRCV  	(IO_AWR1.bit.WRCV)
#define	AWR1_CSRD  	(IO_AWR1.bit.CSRD)
#define	AWR1_RDCS  	(IO_AWR1.bit.RDCS)
#define	AWR1_CSWR  	(IO_AWR1.bit.CSWR)
#define	AWR1_WRCS  	(IO_AWR1.bit.WRCS)
#define	AWR1_ADCY  	(IO_AWR1.bit.ADCY)
#define	AWR1_ACS  	(IO_AWR1.bit.ACS)
#define	AWR1_ASCY  	(IO_AWR1.bit.ASCY)
#define	AWR1_RDYE  	(IO_AWR1.bit.RDYE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_AWR2,attr=IO,locate=0x688
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	RESV0:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RWT:4;
			__WORD	WWT:4;
			__WORD	RIDL:2;
			__WORD	WRCV:2;
			__WORD	CSRD:2;
			__WORD	RDCS:2;
			__WORD	CSWR:2;
			__WORD	WRCS:2;
			__WORD	ADCY:2;
			__WORD	ACS:2;
			__WORD	ASCY:1;
			__WORD	RESV15:1;
			__WORD	RDYE:1;
			__WORD	RESV17:1;
	} bit;
} AWR2STR;

__IO_EXTENDED	  AWR2STR	IO_AWR2;
#define	_awr2		(IO_AWR2)
#define	AWR2		(IO_AWR2.word)
#define	AWR2_RWT  	(IO_AWR2.bit.RWT)
#define	AWR2_WWT  	(IO_AWR2.bit.WWT)
#define	AWR2_RIDL  	(IO_AWR2.bit.RIDL)
#define	AWR2_WRCV  	(IO_AWR2.bit.WRCV)
#define	AWR2_CSRD  	(IO_AWR2.bit.CSRD)
#define	AWR2_RDCS  	(IO_AWR2.bit.RDCS)
#define	AWR2_CSWR  	(IO_AWR2.bit.CSWR)
#define	AWR2_WRCS  	(IO_AWR2.bit.WRCS)
#define	AWR2_ADCY  	(IO_AWR2.bit.ADCY)
#define	AWR2_ACS  	(IO_AWR2.bit.ACS)
#define	AWR2_ASCY  	(IO_AWR2.bit.ASCY)
#define	AWR2_RDYE  	(IO_AWR2.bit.RDYE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_AWR3,attr=IO,locate=0x68C
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	RESV0:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RWT:4;
			__WORD	WWT:4;
			__WORD	RIDL:2;
			__WORD	WRCV:2;
			__WORD	CSRD:2;
			__WORD	RDCS:2;
			__WORD	CSWR:2;
			__WORD	WRCS:2;
			__WORD	ADCY:2;
			__WORD	ACS:2;
			__WORD	ASCY:1;
			__WORD	RESV15:1;
			__WORD	RDYE:1;
			__WORD	RESV17:1;
	} bit;
} AWR3STR;

__IO_EXTENDED	  AWR3STR	IO_AWR3;
#define	_awr3		(IO_AWR3)
#define	AWR3		(IO_AWR3.word)
#define	AWR3_RWT  	(IO_AWR3.bit.RWT)
#define	AWR3_WWT  	(IO_AWR3.bit.WWT)
#define	AWR3_RIDL  	(IO_AWR3.bit.RIDL)
#define	AWR3_WRCV  	(IO_AWR3.bit.WRCV)
#define	AWR3_CSRD  	(IO_AWR3.bit.CSRD)
#define	AWR3_RDCS  	(IO_AWR3.bit.RDCS)
#define	AWR3_CSWR  	(IO_AWR3.bit.CSWR)
#define	AWR3_WRCS  	(IO_AWR3.bit.WRCS)
#define	AWR3_ADCY  	(IO_AWR3.bit.ADCY)
#define	AWR3_ACS  	(IO_AWR3.bit.ACS)
#define	AWR3_ASCY  	(IO_AWR3.bit.ASCY)
#define	AWR3_RDYE  	(IO_AWR3.bit.RDYE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_BPCCRA,attr=IO,locate=0x710
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	FUNC:2;
			__BYTE	MST:4;
			__BYTE	SLV:2;
	} bit;
} BPCCRASTR;

__IO_EXTENDED	  BPCCRASTR	IO_BPCCRA;
#define	_bpccra		(IO_BPCCRA)
#define	BPCCRA		(IO_BPCCRA.byte)
#define	BPCCRA_FUNC  	(IO_BPCCRA.bit.FUNC)
#define	BPCCRA_MST  	(IO_BPCCRA.bit.MST)
#define	BPCCRA_SLV  	(IO_BPCCRA.bit.SLV)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_BPCCRB,attr=IO,locate=0x711
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	FUNC:2;
			__BYTE	MST:4;
			__BYTE	SLV:2;
	} bit;
} BPCCRBSTR;

__IO_EXTENDED	  BPCCRBSTR	IO_BPCCRB;
#define	_bpccrb		(IO_BPCCRB)
#define	BPCCRB		(IO_BPCCRB.byte)
#define	BPCCRB_FUNC  	(IO_BPCCRB.bit.FUNC)
#define	BPCCRB_MST  	(IO_BPCCRB.bit.MST)
#define	BPCCRB_SLV  	(IO_BPCCRB.bit.SLV)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_BPCCRC,attr=IO,locate=0x712
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	FUNC:2;
			__BYTE	MST:4;
			__BYTE	SLV:2;
	} bit;
} BPCCRCSTR;

__IO_EXTENDED	  BPCCRCSTR	IO_BPCCRC;
#define	_bpccrc		(IO_BPCCRC)
#define	BPCCRC		(IO_BPCCRC.byte)
#define	BPCCRC_FUNC  	(IO_BPCCRC.bit.FUNC)
#define	BPCCRC_MST  	(IO_BPCCRC.bit.MST)
#define	BPCCRC_SLV  	(IO_BPCCRC.bit.SLV)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_BPCTRA,attr=IO,locate=0x714
#endif

__IO_EXTENDED	__WORD	IO_BPCTRA;
#define	_bpctra		(IO_BPCTRA)
#define	BPCTRA	(_bpctra)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_BPCTRB,attr=IO,locate=0x718
#endif

__IO_EXTENDED	__WORD	IO_BPCTRB;
#define	_bpctrb		(IO_BPCTRB)
#define	BPCTRB	(_bpctrb)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_BPCTRC,attr=IO,locate=0x71C
#endif

__IO_EXTENDED	__WORD	IO_BPCTRC;
#define	_bpctrc		(IO_BPCTRC)
#define	BPCTRC	(_bpctrc)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_BMODR,attr=IO,locate=0x7FC
#endif

__IO_EXTENDED	const __BYTE	IO_BMODR;
#define	_bmodr		(IO_BMODR)
#define	BMODR	(_bmodr)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_FCTLR,attr=IO,locate=0x840
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	RESV0:1;
			__HWORD	FWE:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	FSZ:2;
			__HWORD	FAW:2;
			__HWORD	FDSBL:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RDYF:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	RESV13:1;
	} bit;
} FCTLRSTR;

__IO_EXTENDED	  FCTLRSTR	IO_FCTLR;
#define	_fctlr		(IO_FCTLR)
#define	FCTLR		(IO_FCTLR.hword)
#define	FCTLR_FWE  	(IO_FCTLR.bit.FWE)
#define	FCTLR_FSZ  	(IO_FCTLR.bit.FSZ)
#define	FCTLR_FAW  	(IO_FCTLR.bit.FAW)
#define	FCTLR_FDSBL  	(IO_FCTLR.bit.FDSBL)
#define	FCTLR_RDYF  	(IO_FCTLR.bit.RDYF)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_FSTR,attr=IO,locate=0x843
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	FECCERR:1;
			__BYTE	FHANG:1;
			__BYTE	FRDY:1;
	} bit;
} FSTRSTR;

__IO_EXTENDED	  FSTRSTR	IO_FSTR;
#define	_fstr		(IO_FSTR)
#define	FSTR		(IO_FSTR.byte)
#define	FSTR_FECCERR  	(IO_FSTR.bit.FECCERR)
#define	FSTR_FHANG  	(IO_FSTR.bit.FHANG)
#define	FSTR_FRDY  	(IO_FSTR.bit.FRDY)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_WREN,attr=IO,locate=0x85A
#endif

__IO_EXTENDED	__HWORD	IO_WREN;
#define	_wren		(IO_WREN)
#define	WREN_	(_wren)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_WR,attr=IO,locate=0x880
#endif

typedef struct {
		__WORD	WRAR;
		__WORD	WRDR;
} WRSTR;

__IO_EXTENDED	  WRSTR	IO_WR[CONSTANT_16];
#define	_wr		(IO_WR)
#define	WR0_WRAR  	(IO_WR[CONSTANT_0].WRAR)
#define	WR1_WRAR  	(IO_WR[CONSTANT_1].WRAR)
#define	WR2_WRAR  	(IO_WR[CONSTANT_2].WRAR)
#define	WR3_WRAR  	(IO_WR[CONSTANT_3].WRAR)
#define	WR4_WRAR  	(IO_WR[CONSTANT_4].WRAR)
#define	WR5_WRAR  	(IO_WR[CONSTANT_5].WRAR)
#define	WR6_WRAR  	(IO_WR[CONSTANT_6].WRAR)
#define	WR7_WRAR  	(IO_WR[CONSTANT_7].WRAR)
#define	WR8_WRAR  	(IO_WR[CONSTANT_8].WRAR)
#define	WR9_WRAR  	(IO_WR[CONSTANT_9].WRAR)
#define	WR10_WRAR  	(IO_WR[CONSTANT_10].WRAR)
#define	WR11_WRAR  	(IO_WR[CONSTANT_11].WRAR)
#define	WR12_WRAR  	(IO_WR[CONSTANT_12].WRAR)
#define	WR13_WRAR  	(IO_WR[CONSTANT_13].WRAR)
#define	WR14_WRAR  	(IO_WR[CONSTANT_14].WRAR)
#define	WR15_WRAR  	(IO_WR[CONSTANT_15].WRAR)
#define	WR0_WRDR  	(IO_WR[CONSTANT_0].WRDR)
#define	WR1_WRDR  	(IO_WR[CONSTANT_1].WRDR)
#define	WR2_WRDR  	(IO_WR[CONSTANT_2].WRDR)
#define	WR3_WRDR  	(IO_WR[CONSTANT_3].WRDR)
#define	WR4_WRDR  	(IO_WR[CONSTANT_4].WRDR)
#define	WR5_WRDR  	(IO_WR[CONSTANT_5].WRDR)
#define	WR6_WRDR  	(IO_WR[CONSTANT_6].WRDR)
#define	WR7_WRDR  	(IO_WR[CONSTANT_7].WRDR)
#define	WR8_WRDR  	(IO_WR[CONSTANT_8].WRDR)
#define	WR9_WRDR  	(IO_WR[CONSTANT_9].WRDR)
#define	WR10_WRDR  	(IO_WR[CONSTANT_10].WRDR)
#define	WR11_WRDR  	(IO_WR[CONSTANT_11].WRDR)
#define	WR12_WRDR  	(IO_WR[CONSTANT_12].WRDR)
#define	WR13_WRDR  	(IO_WR[CONSTANT_13].WRDR)
#define	WR14_WRDR  	(IO_WR[CONSTANT_14].WRDR)
#define	WR15_WRDR  	(IO_WR[CONSTANT_15].WRDR)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_UER,attr=IO,locate=0xBFE
#endif

__IO_EXTENDED	__HWORD	IO_UER;
#define	_uer		(IO_UER)
#define	UER	(_uer)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR0,attr=IO,locate=0xC00
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR0STR;

__IO_EXTENDED	  DCCR0STR	IO_DCCR0;
#define	_dccr0		(IO_DCCR0)
#define	DCCR0		(IO_DCCR0.word)
#define	DCCR0_CE  	(IO_DCCR0.bit.CE)
#define	DCCR0_AIE  	(IO_DCCR0.bit.AIE)
#define	DCCR0_SIE  	(IO_DCCR0.bit.SIE)
#define	DCCR0_NIE  	(IO_DCCR0.bit.NIE)
#define	DCCR0_RS  	(IO_DCCR0.bit.RS)
#define	DCCR0_TM  	(IO_DCCR0.bit.TM)
#define	DCCR0_ST  	(IO_DCCR0.bit.ST)
#define	DCCR0_SAR  	(IO_DCCR0.bit.SAR)
#define	DCCR0_SAC  	(IO_DCCR0.bit.SAC)
#define	DCCR0_DT  	(IO_DCCR0.bit.DT)
#define	DCCR0_DAR  	(IO_DCCR0.bit.DAR)
#define	DCCR0_DAC  	(IO_DCCR0.bit.DAC)
#define	DCCR0_TCR  	(IO_DCCR0.bit.TCR)
#define	DCCR0_TS  	(IO_DCCR0.bit.TS)
#define	DCCR0_BLK  	(IO_DCCR0.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR0,attr=IO,locate=0xC04
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR0STR;

__IO_EXTENDED	  DCSR0STR	IO_DCSR0;
#define	_dcsr0		(IO_DCSR0)
#define	DCSR0		(IO_DCSR0.hword)
#define	DCSR0_CA  	(IO_DCSR0.bit.CA)
#define	DCSR0_AC  	(IO_DCSR0.bit.AC)
#define	DCSR0_SP  	(IO_DCSR0.bit.SP)
#define	DCSR0_NC  	(IO_DCSR0.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR0,attr=IO,locate=0xC06
#endif

__IO_EXTENDED	__HWORD	IO_DTCR0;
#define	_dtcr0		(IO_DTCR0)
#define	DTCR0	(_dtcr0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR0,attr=IO,locate=0xC08
#endif

__IO_EXTENDED	__WORD	IO_DSAR0;
#define	_dsar0		(IO_DSAR0)
#define	DSAR0	(_dsar0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR0,attr=IO,locate=0xC0C
#endif

__IO_EXTENDED	__WORD	IO_DDAR0;
#define	_ddar0		(IO_DDAR0)
#define	DDAR0	(_ddar0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR1,attr=IO,locate=0xC10
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR1STR;

__IO_EXTENDED	  DCCR1STR	IO_DCCR1;
#define	_dccr1		(IO_DCCR1)
#define	DCCR1		(IO_DCCR1.word)
#define	DCCR1_CE  	(IO_DCCR1.bit.CE)
#define	DCCR1_AIE  	(IO_DCCR1.bit.AIE)
#define	DCCR1_SIE  	(IO_DCCR1.bit.SIE)
#define	DCCR1_NIE  	(IO_DCCR1.bit.NIE)
#define	DCCR1_RS  	(IO_DCCR1.bit.RS)
#define	DCCR1_TM  	(IO_DCCR1.bit.TM)
#define	DCCR1_ST  	(IO_DCCR1.bit.ST)
#define	DCCR1_SAR  	(IO_DCCR1.bit.SAR)
#define	DCCR1_SAC  	(IO_DCCR1.bit.SAC)
#define	DCCR1_DT  	(IO_DCCR1.bit.DT)
#define	DCCR1_DAR  	(IO_DCCR1.bit.DAR)
#define	DCCR1_DAC  	(IO_DCCR1.bit.DAC)
#define	DCCR1_TCR  	(IO_DCCR1.bit.TCR)
#define	DCCR1_TS  	(IO_DCCR1.bit.TS)
#define	DCCR1_BLK  	(IO_DCCR1.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR1,attr=IO,locate=0xC14
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR1STR;

__IO_EXTENDED	  DCSR1STR	IO_DCSR1;
#define	_dcsr1		(IO_DCSR1)
#define	DCSR1		(IO_DCSR1.hword)
#define	DCSR1_CA  	(IO_DCSR1.bit.CA)
#define	DCSR1_AC  	(IO_DCSR1.bit.AC)
#define	DCSR1_SP  	(IO_DCSR1.bit.SP)
#define	DCSR1_NC  	(IO_DCSR1.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR1,attr=IO,locate=0xC16
#endif

__IO_EXTENDED	__HWORD	IO_DTCR1;
#define	_dtcr1		(IO_DTCR1)
#define	DTCR1	(_dtcr1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR1,attr=IO,locate=0xC18
#endif

__IO_EXTENDED	__WORD	IO_DSAR1;
#define	_dsar1		(IO_DSAR1)
#define	DSAR1	(_dsar1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR1,attr=IO,locate=0xC1C
#endif

__IO_EXTENDED	__WORD	IO_DDAR1;
#define	_ddar1		(IO_DDAR1)
#define	DDAR1	(_ddar1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR2,attr=IO,locate=0xC20
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR2STR;

__IO_EXTENDED	  DCCR2STR	IO_DCCR2;
#define	_dccr2		(IO_DCCR2)
#define	DCCR2		(IO_DCCR2.word)
#define	DCCR2_CE  	(IO_DCCR2.bit.CE)
#define	DCCR2_AIE  	(IO_DCCR2.bit.AIE)
#define	DCCR2_SIE  	(IO_DCCR2.bit.SIE)
#define	DCCR2_NIE  	(IO_DCCR2.bit.NIE)
#define	DCCR2_RS  	(IO_DCCR2.bit.RS)
#define	DCCR2_TM  	(IO_DCCR2.bit.TM)
#define	DCCR2_ST  	(IO_DCCR2.bit.ST)
#define	DCCR2_SAR  	(IO_DCCR2.bit.SAR)
#define	DCCR2_SAC  	(IO_DCCR2.bit.SAC)
#define	DCCR2_DT  	(IO_DCCR2.bit.DT)
#define	DCCR2_DAR  	(IO_DCCR2.bit.DAR)
#define	DCCR2_DAC  	(IO_DCCR2.bit.DAC)
#define	DCCR2_TCR  	(IO_DCCR2.bit.TCR)
#define	DCCR2_TS  	(IO_DCCR2.bit.TS)
#define	DCCR2_BLK  	(IO_DCCR2.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR2,attr=IO,locate=0xC24
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR2STR;

__IO_EXTENDED	  DCSR2STR	IO_DCSR2;
#define	_dcsr2		(IO_DCSR2)
#define	DCSR2		(IO_DCSR2.hword)
#define	DCSR2_CA  	(IO_DCSR2.bit.CA)
#define	DCSR2_AC  	(IO_DCSR2.bit.AC)
#define	DCSR2_SP  	(IO_DCSR2.bit.SP)
#define	DCSR2_NC  	(IO_DCSR2.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR2,attr=IO,locate=0xC26
#endif

__IO_EXTENDED	__HWORD	IO_DTCR2;
#define	_dtcr2		(IO_DTCR2)
#define	DTCR2	(_dtcr2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR2,attr=IO,locate=0xC28
#endif

__IO_EXTENDED	__WORD	IO_DSAR2;
#define	_dsar2		(IO_DSAR2)
#define	DSAR2	(_dsar2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR2,attr=IO,locate=0xC2C
#endif

__IO_EXTENDED	__WORD	IO_DDAR2;
#define	_ddar2		(IO_DDAR2)
#define	DDAR2	(_ddar2)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR3,attr=IO,locate=0xC30
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR3STR;

__IO_EXTENDED	  DCCR3STR	IO_DCCR3;
#define	_dccr3		(IO_DCCR3)
#define	DCCR3		(IO_DCCR3.word)
#define	DCCR3_CE  	(IO_DCCR3.bit.CE)
#define	DCCR3_AIE  	(IO_DCCR3.bit.AIE)
#define	DCCR3_SIE  	(IO_DCCR3.bit.SIE)
#define	DCCR3_NIE  	(IO_DCCR3.bit.NIE)
#define	DCCR3_RS  	(IO_DCCR3.bit.RS)
#define	DCCR3_TM  	(IO_DCCR3.bit.TM)
#define	DCCR3_ST  	(IO_DCCR3.bit.ST)
#define	DCCR3_SAR  	(IO_DCCR3.bit.SAR)
#define	DCCR3_SAC  	(IO_DCCR3.bit.SAC)
#define	DCCR3_DT  	(IO_DCCR3.bit.DT)
#define	DCCR3_DAR  	(IO_DCCR3.bit.DAR)
#define	DCCR3_DAC  	(IO_DCCR3.bit.DAC)
#define	DCCR3_TCR  	(IO_DCCR3.bit.TCR)
#define	DCCR3_TS  	(IO_DCCR3.bit.TS)
#define	DCCR3_BLK  	(IO_DCCR3.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR3,attr=IO,locate=0xC34
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR3STR;

__IO_EXTENDED	  DCSR3STR	IO_DCSR3;
#define	_dcsr3		(IO_DCSR3)
#define	DCSR3		(IO_DCSR3.hword)
#define	DCSR3_CA  	(IO_DCSR3.bit.CA)
#define	DCSR3_AC  	(IO_DCSR3.bit.AC)
#define	DCSR3_SP  	(IO_DCSR3.bit.SP)
#define	DCSR3_NC  	(IO_DCSR3.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR3,attr=IO,locate=0xC36
#endif

__IO_EXTENDED	__HWORD	IO_DTCR3;
#define	_dtcr3		(IO_DTCR3)
#define	DTCR3	(_dtcr3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR3,attr=IO,locate=0xC38
#endif

__IO_EXTENDED	__WORD	IO_DSAR3;
#define	_dsar3		(IO_DSAR3)
#define	DSAR3	(_dsar3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR3,attr=IO,locate=0xC3C
#endif

__IO_EXTENDED	__WORD	IO_DDAR3;
#define	_ddar3		(IO_DDAR3)
#define	DDAR3	(_ddar3)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR4,attr=IO,locate=0xC40
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR4STR;

__IO_EXTENDED	  DCCR4STR	IO_DCCR4;
#define	_dccr4		(IO_DCCR4)
#define	DCCR4		(IO_DCCR4.word)
#define	DCCR4_CE  	(IO_DCCR4.bit.CE)
#define	DCCR4_AIE  	(IO_DCCR4.bit.AIE)
#define	DCCR4_SIE  	(IO_DCCR4.bit.SIE)
#define	DCCR4_NIE  	(IO_DCCR4.bit.NIE)
#define	DCCR4_RS  	(IO_DCCR4.bit.RS)
#define	DCCR4_TM  	(IO_DCCR4.bit.TM)
#define	DCCR4_ST  	(IO_DCCR4.bit.ST)
#define	DCCR4_SAR  	(IO_DCCR4.bit.SAR)
#define	DCCR4_SAC  	(IO_DCCR4.bit.SAC)
#define	DCCR4_DT  	(IO_DCCR4.bit.DT)
#define	DCCR4_DAR  	(IO_DCCR4.bit.DAR)
#define	DCCR4_DAC  	(IO_DCCR4.bit.DAC)
#define	DCCR4_TCR  	(IO_DCCR4.bit.TCR)
#define	DCCR4_TS  	(IO_DCCR4.bit.TS)
#define	DCCR4_BLK  	(IO_DCCR4.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR4,attr=IO,locate=0xC44
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR4STR;

__IO_EXTENDED	  DCSR4STR	IO_DCSR4;
#define	_dcsr4		(IO_DCSR4)
#define	DCSR4		(IO_DCSR4.hword)
#define	DCSR4_CA  	(IO_DCSR4.bit.CA)
#define	DCSR4_AC  	(IO_DCSR4.bit.AC)
#define	DCSR4_SP  	(IO_DCSR4.bit.SP)
#define	DCSR4_NC  	(IO_DCSR4.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR4,attr=IO,locate=0xC46
#endif

__IO_EXTENDED	__HWORD	IO_DTCR4;
#define	_dtcr4		(IO_DTCR4)
#define	DTCR4	(_dtcr4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR4,attr=IO,locate=0xC48
#endif

__IO_EXTENDED	__WORD	IO_DSAR4;
#define	_dsar4		(IO_DSAR4)
#define	DSAR4	(_dsar4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR4,attr=IO,locate=0xC4C
#endif

__IO_EXTENDED	__WORD	IO_DDAR4;
#define	_ddar4		(IO_DDAR4)
#define	DDAR4	(_ddar4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR5,attr=IO,locate=0xC50
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR5STR;

__IO_EXTENDED	  DCCR5STR	IO_DCCR5;
#define	_dccr5		(IO_DCCR5)
#define	DCCR5		(IO_DCCR5.word)
#define	DCCR5_CE  	(IO_DCCR5.bit.CE)
#define	DCCR5_AIE  	(IO_DCCR5.bit.AIE)
#define	DCCR5_SIE  	(IO_DCCR5.bit.SIE)
#define	DCCR5_NIE  	(IO_DCCR5.bit.NIE)
#define	DCCR5_RS  	(IO_DCCR5.bit.RS)
#define	DCCR5_TM  	(IO_DCCR5.bit.TM)
#define	DCCR5_ST  	(IO_DCCR5.bit.ST)
#define	DCCR5_SAR  	(IO_DCCR5.bit.SAR)
#define	DCCR5_SAC  	(IO_DCCR5.bit.SAC)
#define	DCCR5_DT  	(IO_DCCR5.bit.DT)
#define	DCCR5_DAR  	(IO_DCCR5.bit.DAR)
#define	DCCR5_DAC  	(IO_DCCR5.bit.DAC)
#define	DCCR5_TCR  	(IO_DCCR5.bit.TCR)
#define	DCCR5_TS  	(IO_DCCR5.bit.TS)
#define	DCCR5_BLK  	(IO_DCCR5.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR5,attr=IO,locate=0xC54
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR5STR;

__IO_EXTENDED	  DCSR5STR	IO_DCSR5;
#define	_dcsr5		(IO_DCSR5)
#define	DCSR5		(IO_DCSR5.hword)
#define	DCSR5_CA  	(IO_DCSR5.bit.CA)
#define	DCSR5_AC  	(IO_DCSR5.bit.AC)
#define	DCSR5_SP  	(IO_DCSR5.bit.SP)
#define	DCSR5_NC  	(IO_DCSR5.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR5,attr=IO,locate=0xC56
#endif

__IO_EXTENDED	__HWORD	IO_DTCR5;
#define	_dtcr5		(IO_DTCR5)
#define	DTCR5	(_dtcr5)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR5,attr=IO,locate=0xC58
#endif

__IO_EXTENDED	__WORD	IO_DSAR5;
#define	_dsar5		(IO_DSAR5)
#define	DSAR5	(_dsar5)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR5,attr=IO,locate=0xC5C
#endif

__IO_EXTENDED	__WORD	IO_DDAR5;
#define	_ddar5		(IO_DDAR5)
#define	DDAR5	(_ddar5)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR6,attr=IO,locate=0xC60
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR6STR;

__IO_EXTENDED	  DCCR6STR	IO_DCCR6;
#define	_dccr6		(IO_DCCR6)
#define	DCCR6		(IO_DCCR6.word)
#define	DCCR6_CE  	(IO_DCCR6.bit.CE)
#define	DCCR6_AIE  	(IO_DCCR6.bit.AIE)
#define	DCCR6_SIE  	(IO_DCCR6.bit.SIE)
#define	DCCR6_NIE  	(IO_DCCR6.bit.NIE)
#define	DCCR6_RS  	(IO_DCCR6.bit.RS)
#define	DCCR6_TM  	(IO_DCCR6.bit.TM)
#define	DCCR6_ST  	(IO_DCCR6.bit.ST)
#define	DCCR6_SAR  	(IO_DCCR6.bit.SAR)
#define	DCCR6_SAC  	(IO_DCCR6.bit.SAC)
#define	DCCR6_DT  	(IO_DCCR6.bit.DT)
#define	DCCR6_DAR  	(IO_DCCR6.bit.DAR)
#define	DCCR6_DAC  	(IO_DCCR6.bit.DAC)
#define	DCCR6_TCR  	(IO_DCCR6.bit.TCR)
#define	DCCR6_TS  	(IO_DCCR6.bit.TS)
#define	DCCR6_BLK  	(IO_DCCR6.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR6,attr=IO,locate=0xC64
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR6STR;

__IO_EXTENDED	  DCSR6STR	IO_DCSR6;
#define	_dcsr6		(IO_DCSR6)
#define	DCSR6		(IO_DCSR6.hword)
#define	DCSR6_CA  	(IO_DCSR6.bit.CA)
#define	DCSR6_AC  	(IO_DCSR6.bit.AC)
#define	DCSR6_SP  	(IO_DCSR6.bit.SP)
#define	DCSR6_NC  	(IO_DCSR6.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR6,attr=IO,locate=0xC66
#endif

__IO_EXTENDED	__HWORD	IO_DTCR6;
#define	_dtcr6		(IO_DTCR6)
#define	DTCR6	(_dtcr6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR6,attr=IO,locate=0xC68
#endif

__IO_EXTENDED	__WORD	IO_DSAR6;
#define	_dsar6		(IO_DSAR6)
#define	DSAR6	(_dsar6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR6,attr=IO,locate=0xC6C
#endif

__IO_EXTENDED	__WORD	IO_DDAR6;
#define	_ddar6		(IO_DDAR6)
#define	DDAR6	(_ddar6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR7,attr=IO,locate=0xC70
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR7STR;

__IO_EXTENDED	  DCCR7STR	IO_DCCR7;
#define	_dccr7		(IO_DCCR7)
#define	DCCR7		(IO_DCCR7.word)
#define	DCCR7_CE  	(IO_DCCR7.bit.CE)
#define	DCCR7_AIE  	(IO_DCCR7.bit.AIE)
#define	DCCR7_SIE  	(IO_DCCR7.bit.SIE)
#define	DCCR7_NIE  	(IO_DCCR7.bit.NIE)
#define	DCCR7_RS  	(IO_DCCR7.bit.RS)
#define	DCCR7_TM  	(IO_DCCR7.bit.TM)
#define	DCCR7_ST  	(IO_DCCR7.bit.ST)
#define	DCCR7_SAR  	(IO_DCCR7.bit.SAR)
#define	DCCR7_SAC  	(IO_DCCR7.bit.SAC)
#define	DCCR7_DT  	(IO_DCCR7.bit.DT)
#define	DCCR7_DAR  	(IO_DCCR7.bit.DAR)
#define	DCCR7_DAC  	(IO_DCCR7.bit.DAC)
#define	DCCR7_TCR  	(IO_DCCR7.bit.TCR)
#define	DCCR7_TS  	(IO_DCCR7.bit.TS)
#define	DCCR7_BLK  	(IO_DCCR7.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR7,attr=IO,locate=0xC74
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR7STR;

__IO_EXTENDED	  DCSR7STR	IO_DCSR7;
#define	_dcsr7		(IO_DCSR7)
#define	DCSR7		(IO_DCSR7.hword)
#define	DCSR7_CA  	(IO_DCSR7.bit.CA)
#define	DCSR7_AC  	(IO_DCSR7.bit.AC)
#define	DCSR7_SP  	(IO_DCSR7.bit.SP)
#define	DCSR7_NC  	(IO_DCSR7.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR7,attr=IO,locate=0xC76
#endif

__IO_EXTENDED	__HWORD	IO_DTCR7;
#define	_dtcr7		(IO_DTCR7)
#define	DTCR7	(_dtcr7)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR7,attr=IO,locate=0xC78
#endif

__IO_EXTENDED	__WORD	IO_DSAR7;
#define	_dsar7		(IO_DSAR7)
#define	DSAR7	(_dsar7)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR7,attr=IO,locate=0xC7C
#endif

__IO_EXTENDED	__WORD	IO_DDAR7;
#define	_ddar7		(IO_DDAR7)
#define	DDAR7	(_ddar7)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR8,attr=IO,locate=0xC80
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR8STR;

__IO_EXTENDED	  DCCR8STR	IO_DCCR8;
#define	_dccr8		(IO_DCCR8)
#define	DCCR8		(IO_DCCR8.word)
#define	DCCR8_CE  	(IO_DCCR8.bit.CE)
#define	DCCR8_AIE  	(IO_DCCR8.bit.AIE)
#define	DCCR8_SIE  	(IO_DCCR8.bit.SIE)
#define	DCCR8_NIE  	(IO_DCCR8.bit.NIE)
#define	DCCR8_RS  	(IO_DCCR8.bit.RS)
#define	DCCR8_TM  	(IO_DCCR8.bit.TM)
#define	DCCR8_ST  	(IO_DCCR8.bit.ST)
#define	DCCR8_SAR  	(IO_DCCR8.bit.SAR)
#define	DCCR8_SAC  	(IO_DCCR8.bit.SAC)
#define	DCCR8_DT  	(IO_DCCR8.bit.DT)
#define	DCCR8_DAR  	(IO_DCCR8.bit.DAR)
#define	DCCR8_DAC  	(IO_DCCR8.bit.DAC)
#define	DCCR8_TCR  	(IO_DCCR8.bit.TCR)
#define	DCCR8_TS  	(IO_DCCR8.bit.TS)
#define	DCCR8_BLK  	(IO_DCCR8.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR8,attr=IO,locate=0xC84
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR8STR;

__IO_EXTENDED	  DCSR8STR	IO_DCSR8;
#define	_dcsr8		(IO_DCSR8)
#define	DCSR8		(IO_DCSR8.hword)
#define	DCSR8_CA  	(IO_DCSR8.bit.CA)
#define	DCSR8_AC  	(IO_DCSR8.bit.AC)
#define	DCSR8_SP  	(IO_DCSR8.bit.SP)
#define	DCSR8_NC  	(IO_DCSR8.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR8,attr=IO,locate=0xC86
#endif

__IO_EXTENDED	__HWORD	IO_DTCR8;
#define	_dtcr8		(IO_DTCR8)
#define	DTCR8	(_dtcr8)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR8,attr=IO,locate=0xC88
#endif

__IO_EXTENDED	__WORD	IO_DSAR8;
#define	_dsar8		(IO_DSAR8)
#define	DSAR8	(_dsar8)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR8,attr=IO,locate=0xC8C
#endif

__IO_EXTENDED	__WORD	IO_DDAR8;
#define	_ddar8		(IO_DDAR8)
#define	DDAR8	(_ddar8)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR9,attr=IO,locate=0xC90
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR9STR;

__IO_EXTENDED	  DCCR9STR	IO_DCCR9;
#define	_dccr9		(IO_DCCR9)
#define	DCCR9		(IO_DCCR9.word)
#define	DCCR9_CE  	(IO_DCCR9.bit.CE)
#define	DCCR9_AIE  	(IO_DCCR9.bit.AIE)
#define	DCCR9_SIE  	(IO_DCCR9.bit.SIE)
#define	DCCR9_NIE  	(IO_DCCR9.bit.NIE)
#define	DCCR9_RS  	(IO_DCCR9.bit.RS)
#define	DCCR9_TM  	(IO_DCCR9.bit.TM)
#define	DCCR9_ST  	(IO_DCCR9.bit.ST)
#define	DCCR9_SAR  	(IO_DCCR9.bit.SAR)
#define	DCCR9_SAC  	(IO_DCCR9.bit.SAC)
#define	DCCR9_DT  	(IO_DCCR9.bit.DT)
#define	DCCR9_DAR  	(IO_DCCR9.bit.DAR)
#define	DCCR9_DAC  	(IO_DCCR9.bit.DAC)
#define	DCCR9_TCR  	(IO_DCCR9.bit.TCR)
#define	DCCR9_TS  	(IO_DCCR9.bit.TS)
#define	DCCR9_BLK  	(IO_DCCR9.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR9,attr=IO,locate=0xC94
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR9STR;

__IO_EXTENDED	  DCSR9STR	IO_DCSR9;
#define	_dcsr9		(IO_DCSR9)
#define	DCSR9		(IO_DCSR9.hword)
#define	DCSR9_CA  	(IO_DCSR9.bit.CA)
#define	DCSR9_AC  	(IO_DCSR9.bit.AC)
#define	DCSR9_SP  	(IO_DCSR9.bit.SP)
#define	DCSR9_NC  	(IO_DCSR9.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR9,attr=IO,locate=0xC96
#endif

__IO_EXTENDED	__HWORD	IO_DTCR9;
#define	_dtcr9		(IO_DTCR9)
#define	DTCR9	(_dtcr9)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR9,attr=IO,locate=0xC98
#endif

__IO_EXTENDED	__WORD	IO_DSAR9;
#define	_dsar9		(IO_DSAR9)
#define	DSAR9	(_dsar9)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR9,attr=IO,locate=0xC9C
#endif

__IO_EXTENDED	__WORD	IO_DDAR9;
#define	_ddar9		(IO_DDAR9)
#define	DDAR9	(_ddar9)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR10,attr=IO,locate=0xCA0
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR10STR;

__IO_EXTENDED	  DCCR10STR	IO_DCCR10;
#define	_dccr10		(IO_DCCR10)
#define	DCCR10		(IO_DCCR10.word)
#define	DCCR10_CE  	(IO_DCCR10.bit.CE)
#define	DCCR10_AIE  	(IO_DCCR10.bit.AIE)
#define	DCCR10_SIE  	(IO_DCCR10.bit.SIE)
#define	DCCR10_NIE  	(IO_DCCR10.bit.NIE)
#define	DCCR10_RS  	(IO_DCCR10.bit.RS)
#define	DCCR10_TM  	(IO_DCCR10.bit.TM)
#define	DCCR10_ST  	(IO_DCCR10.bit.ST)
#define	DCCR10_SAR  	(IO_DCCR10.bit.SAR)
#define	DCCR10_SAC  	(IO_DCCR10.bit.SAC)
#define	DCCR10_DT  	(IO_DCCR10.bit.DT)
#define	DCCR10_DAR  	(IO_DCCR10.bit.DAR)
#define	DCCR10_DAC  	(IO_DCCR10.bit.DAC)
#define	DCCR10_TCR  	(IO_DCCR10.bit.TCR)
#define	DCCR10_TS  	(IO_DCCR10.bit.TS)
#define	DCCR10_BLK  	(IO_DCCR10.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR10,attr=IO,locate=0xCA4
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR10STR;

__IO_EXTENDED	  DCSR10STR	IO_DCSR10;
#define	_dcsr10		(IO_DCSR10)
#define	DCSR10		(IO_DCSR10.hword)
#define	DCSR10_CA  	(IO_DCSR10.bit.CA)
#define	DCSR10_AC  	(IO_DCSR10.bit.AC)
#define	DCSR10_SP  	(IO_DCSR10.bit.SP)
#define	DCSR10_NC  	(IO_DCSR10.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR10,attr=IO,locate=0xCA6
#endif

__IO_EXTENDED	__HWORD	IO_DTCR10;
#define	_dtcr10		(IO_DTCR10)
#define	DTCR10	(_dtcr10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR10,attr=IO,locate=0xCA8
#endif

__IO_EXTENDED	__WORD	IO_DSAR10;
#define	_dsar10		(IO_DSAR10)
#define	DSAR10	(_dsar10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR10,attr=IO,locate=0xCAC
#endif

__IO_EXTENDED	__WORD	IO_DDAR10;
#define	_ddar10		(IO_DDAR10)
#define	DDAR10	(_ddar10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR11,attr=IO,locate=0xCB0
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR11STR;

__IO_EXTENDED	  DCCR11STR	IO_DCCR11;
#define	_dccr11		(IO_DCCR11)
#define	DCCR11		(IO_DCCR11.word)
#define	DCCR11_CE  	(IO_DCCR11.bit.CE)
#define	DCCR11_AIE  	(IO_DCCR11.bit.AIE)
#define	DCCR11_SIE  	(IO_DCCR11.bit.SIE)
#define	DCCR11_NIE  	(IO_DCCR11.bit.NIE)
#define	DCCR11_RS  	(IO_DCCR11.bit.RS)
#define	DCCR11_TM  	(IO_DCCR11.bit.TM)
#define	DCCR11_ST  	(IO_DCCR11.bit.ST)
#define	DCCR11_SAR  	(IO_DCCR11.bit.SAR)
#define	DCCR11_SAC  	(IO_DCCR11.bit.SAC)
#define	DCCR11_DT  	(IO_DCCR11.bit.DT)
#define	DCCR11_DAR  	(IO_DCCR11.bit.DAR)
#define	DCCR11_DAC  	(IO_DCCR11.bit.DAC)
#define	DCCR11_TCR  	(IO_DCCR11.bit.TCR)
#define	DCCR11_TS  	(IO_DCCR11.bit.TS)
#define	DCCR11_BLK  	(IO_DCCR11.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR11,attr=IO,locate=0xCB4
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR11STR;

__IO_EXTENDED	  DCSR11STR	IO_DCSR11;
#define	_dcsr11		(IO_DCSR11)
#define	DCSR11		(IO_DCSR11.hword)
#define	DCSR11_CA  	(IO_DCSR11.bit.CA)
#define	DCSR11_AC  	(IO_DCSR11.bit.AC)
#define	DCSR11_SP  	(IO_DCSR11.bit.SP)
#define	DCSR11_NC  	(IO_DCSR11.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR11,attr=IO,locate=0xCB6
#endif

__IO_EXTENDED	__HWORD	IO_DTCR11;
#define	_dtcr11		(IO_DTCR11)
#define	DTCR11	(_dtcr11)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR11,attr=IO,locate=0xCB8
#endif

__IO_EXTENDED	__WORD	IO_DSAR11;
#define	_dsar11		(IO_DSAR11)
#define	DSAR11	(_dsar11)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR11,attr=IO,locate=0xCBC
#endif

__IO_EXTENDED	__WORD	IO_DDAR11;
#define	_ddar11		(IO_DDAR11)
#define	DDAR11	(_ddar11)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR12,attr=IO,locate=0xCC0
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR12STR;

__IO_EXTENDED	  DCCR12STR	IO_DCCR12;
#define	_dccr12		(IO_DCCR12)
#define	DCCR12		(IO_DCCR12.word)
#define	DCCR12_CE  	(IO_DCCR12.bit.CE)
#define	DCCR12_AIE  	(IO_DCCR12.bit.AIE)
#define	DCCR12_SIE  	(IO_DCCR12.bit.SIE)
#define	DCCR12_NIE  	(IO_DCCR12.bit.NIE)
#define	DCCR12_RS  	(IO_DCCR12.bit.RS)
#define	DCCR12_TM  	(IO_DCCR12.bit.TM)
#define	DCCR12_ST  	(IO_DCCR12.bit.ST)
#define	DCCR12_SAR  	(IO_DCCR12.bit.SAR)
#define	DCCR12_SAC  	(IO_DCCR12.bit.SAC)
#define	DCCR12_DT  	(IO_DCCR12.bit.DT)
#define	DCCR12_DAR  	(IO_DCCR12.bit.DAR)
#define	DCCR12_DAC  	(IO_DCCR12.bit.DAC)
#define	DCCR12_TCR  	(IO_DCCR12.bit.TCR)
#define	DCCR12_TS  	(IO_DCCR12.bit.TS)
#define	DCCR12_BLK  	(IO_DCCR12.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR12,attr=IO,locate=0xCC4
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR12STR;

__IO_EXTENDED	  DCSR12STR	IO_DCSR12;
#define	_dcsr12		(IO_DCSR12)
#define	DCSR12		(IO_DCSR12.hword)
#define	DCSR12_CA  	(IO_DCSR12.bit.CA)
#define	DCSR12_AC  	(IO_DCSR12.bit.AC)
#define	DCSR12_SP  	(IO_DCSR12.bit.SP)
#define	DCSR12_NC  	(IO_DCSR12.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR12,attr=IO,locate=0xCC6
#endif

__IO_EXTENDED	__HWORD	IO_DTCR12;
#define	_dtcr12		(IO_DTCR12)
#define	DTCR12	(_dtcr12)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR12,attr=IO,locate=0xCC8
#endif

__IO_EXTENDED	__WORD	IO_DSAR12;
#define	_dsar12		(IO_DSAR12)
#define	DSAR12	(_dsar12)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR12,attr=IO,locate=0xCCC
#endif

__IO_EXTENDED	__WORD	IO_DDAR12;
#define	_ddar12		(IO_DDAR12)
#define	DDAR12	(_ddar12)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR13,attr=IO,locate=0xCD0
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR13STR;

__IO_EXTENDED	  DCCR13STR	IO_DCCR13;
#define	_dccr13		(IO_DCCR13)
#define	DCCR13		(IO_DCCR13.word)
#define	DCCR13_CE  	(IO_DCCR13.bit.CE)
#define	DCCR13_AIE  	(IO_DCCR13.bit.AIE)
#define	DCCR13_SIE  	(IO_DCCR13.bit.SIE)
#define	DCCR13_NIE  	(IO_DCCR13.bit.NIE)
#define	DCCR13_RS  	(IO_DCCR13.bit.RS)
#define	DCCR13_TM  	(IO_DCCR13.bit.TM)
#define	DCCR13_ST  	(IO_DCCR13.bit.ST)
#define	DCCR13_SAR  	(IO_DCCR13.bit.SAR)
#define	DCCR13_SAC  	(IO_DCCR13.bit.SAC)
#define	DCCR13_DT  	(IO_DCCR13.bit.DT)
#define	DCCR13_DAR  	(IO_DCCR13.bit.DAR)
#define	DCCR13_DAC  	(IO_DCCR13.bit.DAC)
#define	DCCR13_TCR  	(IO_DCCR13.bit.TCR)
#define	DCCR13_TS  	(IO_DCCR13.bit.TS)
#define	DCCR13_BLK  	(IO_DCCR13.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR13,attr=IO,locate=0xCD4
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR13STR;

__IO_EXTENDED	  DCSR13STR	IO_DCSR13;
#define	_dcsr13		(IO_DCSR13)
#define	DCSR13		(IO_DCSR13.hword)
#define	DCSR13_CA  	(IO_DCSR13.bit.CA)
#define	DCSR13_AC  	(IO_DCSR13.bit.AC)
#define	DCSR13_SP  	(IO_DCSR13.bit.SP)
#define	DCSR13_NC  	(IO_DCSR13.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR13,attr=IO,locate=0xCD6
#endif

__IO_EXTENDED	__HWORD	IO_DTCR13;
#define	_dtcr13		(IO_DTCR13)
#define	DTCR13	(_dtcr13)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR13,attr=IO,locate=0xCD8
#endif

__IO_EXTENDED	__WORD	IO_DSAR13;
#define	_dsar13		(IO_DSAR13)
#define	DSAR13	(_dsar13)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR13,attr=IO,locate=0xCDC
#endif

__IO_EXTENDED	__WORD	IO_DDAR13;
#define	_ddar13		(IO_DDAR13)
#define	DDAR13	(_ddar13)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR14,attr=IO,locate=0xCE0
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR14STR;

__IO_EXTENDED	  DCCR14STR	IO_DCCR14;
#define	_dccr14		(IO_DCCR14)
#define	DCCR14		(IO_DCCR14.word)
#define	DCCR14_CE  	(IO_DCCR14.bit.CE)
#define	DCCR14_AIE  	(IO_DCCR14.bit.AIE)
#define	DCCR14_SIE  	(IO_DCCR14.bit.SIE)
#define	DCCR14_NIE  	(IO_DCCR14.bit.NIE)
#define	DCCR14_RS  	(IO_DCCR14.bit.RS)
#define	DCCR14_TM  	(IO_DCCR14.bit.TM)
#define	DCCR14_ST  	(IO_DCCR14.bit.ST)
#define	DCCR14_SAR  	(IO_DCCR14.bit.SAR)
#define	DCCR14_SAC  	(IO_DCCR14.bit.SAC)
#define	DCCR14_DT  	(IO_DCCR14.bit.DT)
#define	DCCR14_DAR  	(IO_DCCR14.bit.DAR)
#define	DCCR14_DAC  	(IO_DCCR14.bit.DAC)
#define	DCCR14_TCR  	(IO_DCCR14.bit.TCR)
#define	DCCR14_TS  	(IO_DCCR14.bit.TS)
#define	DCCR14_BLK  	(IO_DCCR14.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR14,attr=IO,locate=0xCE4
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR14STR;

__IO_EXTENDED	  DCSR14STR	IO_DCSR14;
#define	_dcsr14		(IO_DCSR14)
#define	DCSR14		(IO_DCSR14.hword)
#define	DCSR14_CA  	(IO_DCSR14.bit.CA)
#define	DCSR14_AC  	(IO_DCSR14.bit.AC)
#define	DCSR14_SP  	(IO_DCSR14.bit.SP)
#define	DCSR14_NC  	(IO_DCSR14.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR14,attr=IO,locate=0xCE6
#endif

__IO_EXTENDED	__HWORD	IO_DTCR14;
#define	_dtcr14		(IO_DTCR14)
#define	DTCR14	(_dtcr14)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR14,attr=IO,locate=0xCE8
#endif

__IO_EXTENDED	__WORD	IO_DSAR14;
#define	_dsar14		(IO_DSAR14)
#define	DSAR14	(_dsar14)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR14,attr=IO,locate=0xCEC
#endif

__IO_EXTENDED	__WORD	IO_DDAR14;
#define	_ddar14		(IO_DDAR14)
#define	DDAR14	(_ddar14)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCCR15,attr=IO,locate=0xCF0
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	CE:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	AIE:1;
			__WORD	SIE:1;
			__WORD	NIE:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RS:2;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	TM:2;
			__WORD	ST:1;
			__WORD	SAR:1;
			__WORD	SAC:2;
			__WORD	DT:1;
			__WORD	DAR:1;
			__WORD	DAC:2;
			__WORD	TCR:1;
			__WORD	RESV21:1;
			__WORD	TS:2;
			__WORD	BLK:4;
	} bit;
} DCCR15STR;

__IO_EXTENDED	  DCCR15STR	IO_DCCR15;
#define	_dccr15		(IO_DCCR15)
#define	DCCR15		(IO_DCCR15.word)
#define	DCCR15_CE  	(IO_DCCR15.bit.CE)
#define	DCCR15_AIE  	(IO_DCCR15.bit.AIE)
#define	DCCR15_SIE  	(IO_DCCR15.bit.SIE)
#define	DCCR15_NIE  	(IO_DCCR15.bit.NIE)
#define	DCCR15_RS  	(IO_DCCR15.bit.RS)
#define	DCCR15_TM  	(IO_DCCR15.bit.TM)
#define	DCCR15_ST  	(IO_DCCR15.bit.ST)
#define	DCCR15_SAR  	(IO_DCCR15.bit.SAR)
#define	DCCR15_SAC  	(IO_DCCR15.bit.SAC)
#define	DCCR15_DT  	(IO_DCCR15.bit.DT)
#define	DCCR15_DAR  	(IO_DCCR15.bit.DAR)
#define	DCCR15_DAC  	(IO_DCCR15.bit.DAC)
#define	DCCR15_TCR  	(IO_DCCR15.bit.TCR)
#define	DCCR15_TS  	(IO_DCCR15.bit.TS)
#define	DCCR15_BLK  	(IO_DCCR15.bit.BLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DCSR15,attr=IO,locate=0xCF4
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	CA:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	AC:1;
			__HWORD	SP:1;
			__HWORD	NC:1;
	} bit;
} DCSR15STR;

__IO_EXTENDED	  DCSR15STR	IO_DCSR15;
#define	_dcsr15		(IO_DCSR15)
#define	DCSR15		(IO_DCSR15.hword)
#define	DCSR15_CA  	(IO_DCSR15.bit.CA)
#define	DCSR15_AC  	(IO_DCSR15.bit.AC)
#define	DCSR15_SP  	(IO_DCSR15.bit.SP)
#define	DCSR15_NC  	(IO_DCSR15.bit.NC)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DTCR15,attr=IO,locate=0xCF6
#endif

__IO_EXTENDED	__HWORD	IO_DTCR15;
#define	_dtcr15		(IO_DTCR15)
#define	DTCR15	(_dtcr15)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSAR15,attr=IO,locate=0xCF8
#endif

__IO_EXTENDED	__WORD	IO_DSAR15;
#define	_dsar15		(IO_DSAR15)
#define	DSAR15	(_dsar15)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDAR15,attr=IO,locate=0xCFC
#endif

__IO_EXTENDED	__WORD	IO_DDAR15;
#define	_ddar15		(IO_DDAR15)
#define	DDAR15	(_ddar15)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DNMIR,attr=IO,locate=0xDF6
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	NMIH:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	NMIHD:1;
	} bit;
} DNMIRSTR;

__IO_EXTENDED	  DNMIRSTR	IO_DNMIR;
#define	_dnmir		(IO_DNMIR)
#define	DNMIR		(IO_DNMIR.byte)
#define	DNMIR_NMIH  	(IO_DNMIR.bit.NMIH)
#define	DNMIR_NMIHD  	(IO_DNMIR.bit.NMIHD)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DILVR,attr=IO,locate=0xDF7
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	LVL:5;
	} bit;
} DILVRSTR;

__IO_EXTENDED	  DILVRSTR	IO_DILVR;
#define	_dilvr		(IO_DILVR)
#define	DILVR		(IO_DILVR.byte)
#define	DILVR_LVL  	(IO_DILVR.bit.LVL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DMACR,attr=IO,locate=0xDF8
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	DME:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	RESV5:1;
			__WORD	RESV6:1;
			__WORD	RESV7:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RESV10:1;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	RESV13:1;
			__WORD	RESV14:1;
			__WORD	RESV15:1;
			__WORD	AT:1;
			__WORD	RESV17:1;
			__WORD	RESV18:1;
			__WORD	RESV19:1;
			__WORD	RESV20:1;
			__WORD	RESV21:1;
			__WORD	RESV22:1;
			__WORD	RESV23:1;
			__WORD	RESV24:1;
			__WORD	RESV25:1;
			__WORD	RESV26:1;
			__WORD	RESV27:1;
			__WORD	RESV28:1;
			__WORD	RESV29:1;
			__WORD	RESV30:1;
			__WORD	RESV31:1;
	} bit;
} DMACRSTR;

__IO_EXTENDED	  DMACRSTR	IO_DMACR;
#define	_dmacr		(IO_DMACR)
#define	DMACR		(IO_DMACR.word)
#define	DMACR_DME  	(IO_DMACR.bit.DME)
#define	DMACR_AT  	(IO_DMACR.bit.AT)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDR00,attr=IO,locate=0xE00
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} DDR00STR;

__IO_EXTENDED	  DDR00STR	IO_DDR00;
#define	_ddr00		(IO_DDR00)
#define	DDR00		(IO_DDR00.byte)
#define	DDR00_P7  	(IO_DDR00.bit.P7)
#define	DDR00_P6  	(IO_DDR00.bit.P6)
#define	DDR00_P5  	(IO_DDR00.bit.P5)
#define	DDR00_P4  	(IO_DDR00.bit.P4)
#define	DDR00_P3  	(IO_DDR00.bit.P3)
#define	DDR00_P2  	(IO_DDR00.bit.P2)
#define	DDR00_P1  	(IO_DDR00.bit.P1)
#define	DDR00_P0  	(IO_DDR00.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDR01,attr=IO,locate=0xE01
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} DDR01STR;

__IO_EXTENDED	  DDR01STR	IO_DDR01;
#define	_ddr01		(IO_DDR01)
#define	DDR01		(IO_DDR01.byte)
#define	DDR01_P7  	(IO_DDR01.bit.P7)
#define	DDR01_P6  	(IO_DDR01.bit.P6)
#define	DDR01_P5  	(IO_DDR01.bit.P5)
#define	DDR01_P4  	(IO_DDR01.bit.P4)
#define	DDR01_P3  	(IO_DDR01.bit.P3)
#define	DDR01_P2  	(IO_DDR01.bit.P2)
#define	DDR01_P1  	(IO_DDR01.bit.P1)
#define	DDR01_P0  	(IO_DDR01.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDR02,attr=IO,locate=0xE02
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} DDR02STR;

__IO_EXTENDED	  DDR02STR	IO_DDR02;
#define	_ddr02		(IO_DDR02)
#define	DDR02		(IO_DDR02.byte)
#define	DDR02_P7  	(IO_DDR02.bit.P7)
#define	DDR02_P6  	(IO_DDR02.bit.P6)
#define	DDR02_P5  	(IO_DDR02.bit.P5)
#define	DDR02_P4  	(IO_DDR02.bit.P4)
#define	DDR02_P3  	(IO_DDR02.bit.P3)
#define	DDR02_P2  	(IO_DDR02.bit.P2)
#define	DDR02_P1  	(IO_DDR02.bit.P1)
#define	DDR02_P0  	(IO_DDR02.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDR03,attr=IO,locate=0xE03
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} DDR03STR;

__IO_EXTENDED	  DDR03STR	IO_DDR03;
#define	_ddr03		(IO_DDR03)
#define	DDR03		(IO_DDR03.byte)
#define	DDR03_P7  	(IO_DDR03.bit.P7)
#define	DDR03_P6  	(IO_DDR03.bit.P6)
#define	DDR03_P5  	(IO_DDR03.bit.P5)
#define	DDR03_P4  	(IO_DDR03.bit.P4)
#define	DDR03_P3  	(IO_DDR03.bit.P3)
#define	DDR03_P2  	(IO_DDR03.bit.P2)
#define	DDR03_P1  	(IO_DDR03.bit.P1)
#define	DDR03_P0  	(IO_DDR03.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDR04,attr=IO,locate=0xE04
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} DDR04STR;

__IO_EXTENDED	  DDR04STR	IO_DDR04;
#define	_ddr04		(IO_DDR04)
#define	DDR04		(IO_DDR04.byte)
#define	DDR04_P7  	(IO_DDR04.bit.P7)
#define	DDR04_P6  	(IO_DDR04.bit.P6)
#define	DDR04_P5  	(IO_DDR04.bit.P5)
#define	DDR04_P4  	(IO_DDR04.bit.P4)
#define	DDR04_P3  	(IO_DDR04.bit.P3)
#define	DDR04_P2  	(IO_DDR04.bit.P2)
#define	DDR04_P1  	(IO_DDR04.bit.P1)
#define	DDR04_P0  	(IO_DDR04.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDR05,attr=IO,locate=0xE05
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} DDR05STR;

__IO_EXTENDED	  DDR05STR	IO_DDR05;
#define	_ddr05		(IO_DDR05)
#define	DDR05		(IO_DDR05.byte)
#define	DDR05_P7  	(IO_DDR05.bit.P7)
#define	DDR05_P6  	(IO_DDR05.bit.P6)
#define	DDR05_P5  	(IO_DDR05.bit.P5)
#define	DDR05_P4  	(IO_DDR05.bit.P4)
#define	DDR05_P3  	(IO_DDR05.bit.P3)
#define	DDR05_P2  	(IO_DDR05.bit.P2)
#define	DDR05_P1  	(IO_DDR05.bit.P1)
#define	DDR05_P0  	(IO_DDR05.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDR06,attr=IO,locate=0xE06
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} DDR06STR;

__IO_EXTENDED	  DDR06STR	IO_DDR06;
#define	_ddr06		(IO_DDR06)
#define	DDR06		(IO_DDR06.byte)
#define	DDR06_P7  	(IO_DDR06.bit.P7)
#define	DDR06_P6  	(IO_DDR06.bit.P6)
#define	DDR06_P5  	(IO_DDR06.bit.P5)
#define	DDR06_P4  	(IO_DDR06.bit.P4)
#define	DDR06_P3  	(IO_DDR06.bit.P3)
#define	DDR06_P2  	(IO_DDR06.bit.P2)
#define	DDR06_P1  	(IO_DDR06.bit.P1)
#define	DDR06_P0  	(IO_DDR06.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDR07,attr=IO,locate=0xE07
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} DDR07STR;

__IO_EXTENDED	  DDR07STR	IO_DDR07;
#define	_ddr07		(IO_DDR07)
#define	DDR07		(IO_DDR07.byte)
#define	DDR07_P7  	(IO_DDR07.bit.P7)
#define	DDR07_P6  	(IO_DDR07.bit.P6)
#define	DDR07_P5  	(IO_DDR07.bit.P5)
#define	DDR07_P4  	(IO_DDR07.bit.P4)
#define	DDR07_P3  	(IO_DDR07.bit.P3)
#define	DDR07_P2  	(IO_DDR07.bit.P2)
#define	DDR07_P1  	(IO_DDR07.bit.P1)
#define	DDR07_P0  	(IO_DDR07.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDR08,attr=IO,locate=0xE08
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} DDR08STR;

__IO_EXTENDED	  DDR08STR	IO_DDR08;
#define	_ddr08		(IO_DDR08)
#define	DDR08		(IO_DDR08.byte)
#define	DDR08_P7  	(IO_DDR08.bit.P7)
#define	DDR08_P6  	(IO_DDR08.bit.P6)
#define	DDR08_P5  	(IO_DDR08.bit.P5)
#define	DDR08_P4  	(IO_DDR08.bit.P4)
#define	DDR08_P3  	(IO_DDR08.bit.P3)
#define	DDR08_P2  	(IO_DDR08.bit.P2)
#define	DDR08_P1  	(IO_DDR08.bit.P1)
#define	DDR08_P0  	(IO_DDR08.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDR09,attr=IO,locate=0xE09
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} DDR09STR;

__IO_EXTENDED	  DDR09STR	IO_DDR09;
#define	_ddr09		(IO_DDR09)
#define	DDR09		(IO_DDR09.byte)
#define	DDR09_P7  	(IO_DDR09.bit.P7)
#define	DDR09_P6  	(IO_DDR09.bit.P6)
#define	DDR09_P5  	(IO_DDR09.bit.P5)
#define	DDR09_P4  	(IO_DDR09.bit.P4)
#define	DDR09_P3  	(IO_DDR09.bit.P3)
#define	DDR09_P2  	(IO_DDR09.bit.P2)
#define	DDR09_P1  	(IO_DDR09.bit.P1)
#define	DDR09_P0  	(IO_DDR09.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDR10,attr=IO,locate=0xE0A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} DDR10STR;

__IO_EXTENDED	  DDR10STR	IO_DDR10;
#define	_ddr10		(IO_DDR10)
#define	DDR10		(IO_DDR10.byte)
#define	DDR10_P7  	(IO_DDR10.bit.P7)
#define	DDR10_P6  	(IO_DDR10.bit.P6)
#define	DDR10_P5  	(IO_DDR10.bit.P5)
#define	DDR10_P4  	(IO_DDR10.bit.P4)
#define	DDR10_P3  	(IO_DDR10.bit.P3)
#define	DDR10_P2  	(IO_DDR10.bit.P2)
#define	DDR10_P1  	(IO_DDR10.bit.P1)
#define	DDR10_P0  	(IO_DDR10.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDR11,attr=IO,locate=0xE0B
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} DDR11STR;

__IO_EXTENDED	  DDR11STR	IO_DDR11;
#define	_ddr11		(IO_DDR11)
#define	DDR11		(IO_DDR11.byte)
#define	DDR11_P7  	(IO_DDR11.bit.P7)
#define	DDR11_P6  	(IO_DDR11.bit.P6)
#define	DDR11_P5  	(IO_DDR11.bit.P5)
#define	DDR11_P4  	(IO_DDR11.bit.P4)
#define	DDR11_P3  	(IO_DDR11.bit.P3)
#define	DDR11_P2  	(IO_DDR11.bit.P2)
#define	DDR11_P1  	(IO_DDR11.bit.P1)
#define	DDR11_P0  	(IO_DDR11.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDR12,attr=IO,locate=0xE0C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} DDR12STR;

__IO_EXTENDED	  DDR12STR	IO_DDR12;
#define	_ddr12		(IO_DDR12)
#define	DDR12		(IO_DDR12.byte)
#define	DDR12_P7  	(IO_DDR12.bit.P7)
#define	DDR12_P6  	(IO_DDR12.bit.P6)
#define	DDR12_P5  	(IO_DDR12.bit.P5)
#define	DDR12_P4  	(IO_DDR12.bit.P4)
#define	DDR12_P3  	(IO_DDR12.bit.P3)
#define	DDR12_P2  	(IO_DDR12.bit.P2)
#define	DDR12_P1  	(IO_DDR12.bit.P1)
#define	DDR12_P0  	(IO_DDR12.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DDR13,attr=IO,locate=0xE0D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	RESV2:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} DDR13STR;

__IO_EXTENDED	  DDR13STR	IO_DDR13;
#define	_ddr13		(IO_DDR13)
#define	DDR13		(IO_DDR13.byte)
#define	DDR13_P7  	(IO_DDR13.bit.P7)
#define	DDR13_P6  	(IO_DDR13.bit.P6)
#define	DDR13_P4  	(IO_DDR13.bit.P4)
#define	DDR13_P3  	(IO_DDR13.bit.P3)
#define	DDR13_P2  	(IO_DDR13.bit.P2)
#define	DDR13_P1  	(IO_DDR13.bit.P1)
#define	DDR13_P0  	(IO_DDR13.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PFR00,attr=IO,locate=0xE20
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PFR00STR;

__IO_EXTENDED	  PFR00STR	IO_PFR00;
#define	_pfr00		(IO_PFR00)
#define	PFR00		(IO_PFR00.byte)
#define	PFR00_P7  	(IO_PFR00.bit.P7)
#define	PFR00_P6  	(IO_PFR00.bit.P6)
#define	PFR00_P5  	(IO_PFR00.bit.P5)
#define	PFR00_P4  	(IO_PFR00.bit.P4)
#define	PFR00_P3  	(IO_PFR00.bit.P3)
#define	PFR00_P2  	(IO_PFR00.bit.P2)
#define	PFR00_P1  	(IO_PFR00.bit.P1)
#define	PFR00_P0  	(IO_PFR00.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PFR01,attr=IO,locate=0xE21
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PFR01STR;

__IO_EXTENDED	  PFR01STR	IO_PFR01;
#define	_pfr01		(IO_PFR01)
#define	PFR01		(IO_PFR01.byte)
#define	PFR01_P7  	(IO_PFR01.bit.P7)
#define	PFR01_P6  	(IO_PFR01.bit.P6)
#define	PFR01_P5  	(IO_PFR01.bit.P5)
#define	PFR01_P4  	(IO_PFR01.bit.P4)
#define	PFR01_P3  	(IO_PFR01.bit.P3)
#define	PFR01_P2  	(IO_PFR01.bit.P2)
#define	PFR01_P1  	(IO_PFR01.bit.P1)
#define	PFR01_P0  	(IO_PFR01.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PFR02,attr=IO,locate=0xE22
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PFR02STR;

__IO_EXTENDED	  PFR02STR	IO_PFR02;
#define	_pfr02		(IO_PFR02)
#define	PFR02		(IO_PFR02.byte)
#define	PFR02_P7  	(IO_PFR02.bit.P7)
#define	PFR02_P6  	(IO_PFR02.bit.P6)
#define	PFR02_P5  	(IO_PFR02.bit.P5)
#define	PFR02_P4  	(IO_PFR02.bit.P4)
#define	PFR02_P3  	(IO_PFR02.bit.P3)
#define	PFR02_P2  	(IO_PFR02.bit.P2)
#define	PFR02_P1  	(IO_PFR02.bit.P1)
#define	PFR02_P0  	(IO_PFR02.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PFR03,attr=IO,locate=0xE23
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PFR03STR;

__IO_EXTENDED	  PFR03STR	IO_PFR03;
#define	_pfr03		(IO_PFR03)
#define	PFR03		(IO_PFR03.byte)
#define	PFR03_P7  	(IO_PFR03.bit.P7)
#define	PFR03_P6  	(IO_PFR03.bit.P6)
#define	PFR03_P5  	(IO_PFR03.bit.P5)
#define	PFR03_P4  	(IO_PFR03.bit.P4)
#define	PFR03_P3  	(IO_PFR03.bit.P3)
#define	PFR03_P2  	(IO_PFR03.bit.P2)
#define	PFR03_P1  	(IO_PFR03.bit.P1)
#define	PFR03_P0  	(IO_PFR03.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PFR04,attr=IO,locate=0xE24
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PFR04STR;

__IO_EXTENDED	  PFR04STR	IO_PFR04;
#define	_pfr04		(IO_PFR04)
#define	PFR04		(IO_PFR04.byte)
#define	PFR04_P7  	(IO_PFR04.bit.P7)
#define	PFR04_P6  	(IO_PFR04.bit.P6)
#define	PFR04_P5  	(IO_PFR04.bit.P5)
#define	PFR04_P4  	(IO_PFR04.bit.P4)
#define	PFR04_P3  	(IO_PFR04.bit.P3)
#define	PFR04_P2  	(IO_PFR04.bit.P2)
#define	PFR04_P1  	(IO_PFR04.bit.P1)
#define	PFR04_P0  	(IO_PFR04.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PFR05,attr=IO,locate=0xE25
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PFR05STR;

__IO_EXTENDED	  PFR05STR	IO_PFR05;
#define	_pfr05		(IO_PFR05)
#define	PFR05		(IO_PFR05.byte)
#define	PFR05_P7  	(IO_PFR05.bit.P7)
#define	PFR05_P6  	(IO_PFR05.bit.P6)
#define	PFR05_P5  	(IO_PFR05.bit.P5)
#define	PFR05_P4  	(IO_PFR05.bit.P4)
#define	PFR05_P3  	(IO_PFR05.bit.P3)
#define	PFR05_P2  	(IO_PFR05.bit.P2)
#define	PFR05_P1  	(IO_PFR05.bit.P1)
#define	PFR05_P0  	(IO_PFR05.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PFR06,attr=IO,locate=0xE26
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PFR06STR;

__IO_EXTENDED	  PFR06STR	IO_PFR06;
#define	_pfr06		(IO_PFR06)
#define	PFR06		(IO_PFR06.byte)
#define	PFR06_P7  	(IO_PFR06.bit.P7)
#define	PFR06_P6  	(IO_PFR06.bit.P6)
#define	PFR06_P5  	(IO_PFR06.bit.P5)
#define	PFR06_P4  	(IO_PFR06.bit.P4)
#define	PFR06_P3  	(IO_PFR06.bit.P3)
#define	PFR06_P2  	(IO_PFR06.bit.P2)
#define	PFR06_P1  	(IO_PFR06.bit.P1)
#define	PFR06_P0  	(IO_PFR06.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PFR07,attr=IO,locate=0xE27
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PFR07STR;

__IO_EXTENDED	  PFR07STR	IO_PFR07;
#define	_pfr07		(IO_PFR07)
#define	PFR07		(IO_PFR07.byte)
#define	PFR07_P7  	(IO_PFR07.bit.P7)
#define	PFR07_P6  	(IO_PFR07.bit.P6)
#define	PFR07_P5  	(IO_PFR07.bit.P5)
#define	PFR07_P4  	(IO_PFR07.bit.P4)
#define	PFR07_P3  	(IO_PFR07.bit.P3)
#define	PFR07_P2  	(IO_PFR07.bit.P2)
#define	PFR07_P1  	(IO_PFR07.bit.P1)
#define	PFR07_P0  	(IO_PFR07.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PFR08,attr=IO,locate=0xE28
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PFR08STR;

__IO_EXTENDED	  PFR08STR	IO_PFR08;
#define	_pfr08		(IO_PFR08)
#define	PFR08		(IO_PFR08.byte)
#define	PFR08_P7  	(IO_PFR08.bit.P7)
#define	PFR08_P6  	(IO_PFR08.bit.P6)
#define	PFR08_P5  	(IO_PFR08.bit.P5)
#define	PFR08_P4  	(IO_PFR08.bit.P4)
#define	PFR08_P3  	(IO_PFR08.bit.P3)
#define	PFR08_P2  	(IO_PFR08.bit.P2)
#define	PFR08_P1  	(IO_PFR08.bit.P1)
#define	PFR08_P0  	(IO_PFR08.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PFR09,attr=IO,locate=0xE29
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	RESV1:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PFR09STR;

__IO_EXTENDED	  PFR09STR	IO_PFR09;
#define	_pfr09		(IO_PFR09)
#define	PFR09		(IO_PFR09.byte)
#define	PFR09_P7  	(IO_PFR09.bit.P7)
#define	PFR09_P5  	(IO_PFR09.bit.P5)
#define	PFR09_P4  	(IO_PFR09.bit.P4)
#define	PFR09_P3  	(IO_PFR09.bit.P3)
#define	PFR09_P2  	(IO_PFR09.bit.P2)
#define	PFR09_P1  	(IO_PFR09.bit.P1)
#define	PFR09_P0  	(IO_PFR09.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PFR10,attr=IO,locate=0xE2A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PFR10STR;

__IO_EXTENDED	  PFR10STR	IO_PFR10;
#define	_pfr10		(IO_PFR10)
#define	PFR10		(IO_PFR10.byte)
#define	PFR10_P7  	(IO_PFR10.bit.P7)
#define	PFR10_P6  	(IO_PFR10.bit.P6)
#define	PFR10_P5  	(IO_PFR10.bit.P5)
#define	PFR10_P4  	(IO_PFR10.bit.P4)
#define	PFR10_P3  	(IO_PFR10.bit.P3)
#define	PFR10_P2  	(IO_PFR10.bit.P2)
#define	PFR10_P1  	(IO_PFR10.bit.P1)
#define	PFR10_P0  	(IO_PFR10.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PFR11,attr=IO,locate=0xE2B
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PFR11STR;

__IO_EXTENDED	  PFR11STR	IO_PFR11;
#define	_pfr11		(IO_PFR11)
#define	PFR11		(IO_PFR11.byte)
#define	PFR11_P7  	(IO_PFR11.bit.P7)
#define	PFR11_P6  	(IO_PFR11.bit.P6)
#define	PFR11_P5  	(IO_PFR11.bit.P5)
#define	PFR11_P4  	(IO_PFR11.bit.P4)
#define	PFR11_P3  	(IO_PFR11.bit.P3)
#define	PFR11_P2  	(IO_PFR11.bit.P2)
#define	PFR11_P1  	(IO_PFR11.bit.P1)
#define	PFR11_P0  	(IO_PFR11.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PFR12,attr=IO,locate=0xE2C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PFR12STR;

__IO_EXTENDED	  PFR12STR	IO_PFR12;
#define	_pfr12		(IO_PFR12)
#define	PFR12		(IO_PFR12.byte)
#define	PFR12_P7  	(IO_PFR12.bit.P7)
#define	PFR12_P6  	(IO_PFR12.bit.P6)
#define	PFR12_P5  	(IO_PFR12.bit.P5)
#define	PFR12_P4  	(IO_PFR12.bit.P4)
#define	PFR12_P3  	(IO_PFR12.bit.P3)
#define	PFR12_P2  	(IO_PFR12.bit.P2)
#define	PFR12_P1  	(IO_PFR12.bit.P1)
#define	PFR12_P0  	(IO_PFR12.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PFR13,attr=IO,locate=0xE2D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	RESV2:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PFR13STR;

__IO_EXTENDED	  PFR13STR	IO_PFR13;
#define	_pfr13		(IO_PFR13)
#define	PFR13		(IO_PFR13.byte)
#define	PFR13_P7  	(IO_PFR13.bit.P7)
#define	PFR13_P6  	(IO_PFR13.bit.P6)
#define	PFR13_P4  	(IO_PFR13.bit.P4)
#define	PFR13_P3  	(IO_PFR13.bit.P3)
#define	PFR13_P2  	(IO_PFR13.bit.P2)
#define	PFR13_P1  	(IO_PFR13.bit.P1)
#define	PFR13_P0  	(IO_PFR13.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDDR00,attr=IO,locate=0xE40
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDDR00STR;

__IO_EXTENDED	 const  PDDR00STR	IO_PDDR00;
#define	_pddr00		(IO_PDDR00)
#define	PDDR00		(IO_PDDR00.byte)
#define	PDDR00_P7  	(IO_PDDR00.bit.P7)
#define	PDDR00_P6  	(IO_PDDR00.bit.P6)
#define	PDDR00_P5  	(IO_PDDR00.bit.P5)
#define	PDDR00_P4  	(IO_PDDR00.bit.P4)
#define	PDDR00_P3  	(IO_PDDR00.bit.P3)
#define	PDDR00_P2  	(IO_PDDR00.bit.P2)
#define	PDDR00_P1  	(IO_PDDR00.bit.P1)
#define	PDDR00_P0  	(IO_PDDR00.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDDR01,attr=IO,locate=0xE41
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDDR01STR;

__IO_EXTENDED	 const  PDDR01STR	IO_PDDR01;
#define	_pddr01		(IO_PDDR01)
#define	PDDR01		(IO_PDDR01.byte)
#define	PDDR01_P7  	(IO_PDDR01.bit.P7)
#define	PDDR01_P6  	(IO_PDDR01.bit.P6)
#define	PDDR01_P5  	(IO_PDDR01.bit.P5)
#define	PDDR01_P4  	(IO_PDDR01.bit.P4)
#define	PDDR01_P3  	(IO_PDDR01.bit.P3)
#define	PDDR01_P2  	(IO_PDDR01.bit.P2)
#define	PDDR01_P1  	(IO_PDDR01.bit.P1)
#define	PDDR01_P0  	(IO_PDDR01.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDDR02,attr=IO,locate=0xE42
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDDR02STR;

__IO_EXTENDED	 const  PDDR02STR	IO_PDDR02;
#define	_pddr02		(IO_PDDR02)
#define	PDDR02		(IO_PDDR02.byte)
#define	PDDR02_P7  	(IO_PDDR02.bit.P7)
#define	PDDR02_P6  	(IO_PDDR02.bit.P6)
#define	PDDR02_P5  	(IO_PDDR02.bit.P5)
#define	PDDR02_P4  	(IO_PDDR02.bit.P4)
#define	PDDR02_P3  	(IO_PDDR02.bit.P3)
#define	PDDR02_P2  	(IO_PDDR02.bit.P2)
#define	PDDR02_P1  	(IO_PDDR02.bit.P1)
#define	PDDR02_P0  	(IO_PDDR02.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDDR03,attr=IO,locate=0xE43
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDDR03STR;

__IO_EXTENDED	 const  PDDR03STR	IO_PDDR03;
#define	_pddr03		(IO_PDDR03)
#define	PDDR03		(IO_PDDR03.byte)
#define	PDDR03_P7  	(IO_PDDR03.bit.P7)
#define	PDDR03_P6  	(IO_PDDR03.bit.P6)
#define	PDDR03_P5  	(IO_PDDR03.bit.P5)
#define	PDDR03_P4  	(IO_PDDR03.bit.P4)
#define	PDDR03_P3  	(IO_PDDR03.bit.P3)
#define	PDDR03_P2  	(IO_PDDR03.bit.P2)
#define	PDDR03_P1  	(IO_PDDR03.bit.P1)
#define	PDDR03_P0  	(IO_PDDR03.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDDR04,attr=IO,locate=0xE44
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDDR04STR;

__IO_EXTENDED	 const  PDDR04STR	IO_PDDR04;
#define	_pddr04		(IO_PDDR04)
#define	PDDR04		(IO_PDDR04.byte)
#define	PDDR04_P7  	(IO_PDDR04.bit.P7)
#define	PDDR04_P6  	(IO_PDDR04.bit.P6)
#define	PDDR04_P5  	(IO_PDDR04.bit.P5)
#define	PDDR04_P4  	(IO_PDDR04.bit.P4)
#define	PDDR04_P3  	(IO_PDDR04.bit.P3)
#define	PDDR04_P2  	(IO_PDDR04.bit.P2)
#define	PDDR04_P1  	(IO_PDDR04.bit.P1)
#define	PDDR04_P0  	(IO_PDDR04.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDDR05,attr=IO,locate=0xE45
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDDR05STR;

__IO_EXTENDED	 const  PDDR05STR	IO_PDDR05;
#define	_pddr05		(IO_PDDR05)
#define	PDDR05		(IO_PDDR05.byte)
#define	PDDR05_P7  	(IO_PDDR05.bit.P7)
#define	PDDR05_P6  	(IO_PDDR05.bit.P6)
#define	PDDR05_P5  	(IO_PDDR05.bit.P5)
#define	PDDR05_P4  	(IO_PDDR05.bit.P4)
#define	PDDR05_P3  	(IO_PDDR05.bit.P3)
#define	PDDR05_P2  	(IO_PDDR05.bit.P2)
#define	PDDR05_P1  	(IO_PDDR05.bit.P1)
#define	PDDR05_P0  	(IO_PDDR05.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDDR06,attr=IO,locate=0xE46
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDDR06STR;

__IO_EXTENDED	 const  PDDR06STR	IO_PDDR06;
#define	_pddr06		(IO_PDDR06)
#define	PDDR06		(IO_PDDR06.byte)
#define	PDDR06_P7  	(IO_PDDR06.bit.P7)
#define	PDDR06_P6  	(IO_PDDR06.bit.P6)
#define	PDDR06_P5  	(IO_PDDR06.bit.P5)
#define	PDDR06_P4  	(IO_PDDR06.bit.P4)
#define	PDDR06_P3  	(IO_PDDR06.bit.P3)
#define	PDDR06_P2  	(IO_PDDR06.bit.P2)
#define	PDDR06_P1  	(IO_PDDR06.bit.P1)
#define	PDDR06_P0  	(IO_PDDR06.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDDR07,attr=IO,locate=0xE47
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDDR07STR;

__IO_EXTENDED	 const  PDDR07STR	IO_PDDR07;
#define	_pddr07		(IO_PDDR07)
#define	PDDR07		(IO_PDDR07.byte)
#define	PDDR07_P7  	(IO_PDDR07.bit.P7)
#define	PDDR07_P6  	(IO_PDDR07.bit.P6)
#define	PDDR07_P5  	(IO_PDDR07.bit.P5)
#define	PDDR07_P4  	(IO_PDDR07.bit.P4)
#define	PDDR07_P3  	(IO_PDDR07.bit.P3)
#define	PDDR07_P2  	(IO_PDDR07.bit.P2)
#define	PDDR07_P1  	(IO_PDDR07.bit.P1)
#define	PDDR07_P0  	(IO_PDDR07.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDDR08,attr=IO,locate=0xE48
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDDR08STR;

__IO_EXTENDED	 const  PDDR08STR	IO_PDDR08;
#define	_pddr08		(IO_PDDR08)
#define	PDDR08		(IO_PDDR08.byte)
#define	PDDR08_P7  	(IO_PDDR08.bit.P7)
#define	PDDR08_P6  	(IO_PDDR08.bit.P6)
#define	PDDR08_P5  	(IO_PDDR08.bit.P5)
#define	PDDR08_P4  	(IO_PDDR08.bit.P4)
#define	PDDR08_P3  	(IO_PDDR08.bit.P3)
#define	PDDR08_P2  	(IO_PDDR08.bit.P2)
#define	PDDR08_P1  	(IO_PDDR08.bit.P1)
#define	PDDR08_P0  	(IO_PDDR08.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDDR09,attr=IO,locate=0xE49
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDDR09STR;

__IO_EXTENDED	 const  PDDR09STR	IO_PDDR09;
#define	_pddr09		(IO_PDDR09)
#define	PDDR09		(IO_PDDR09.byte)
#define	PDDR09_P7  	(IO_PDDR09.bit.P7)
#define	PDDR09_P6  	(IO_PDDR09.bit.P6)
#define	PDDR09_P5  	(IO_PDDR09.bit.P5)
#define	PDDR09_P4  	(IO_PDDR09.bit.P4)
#define	PDDR09_P3  	(IO_PDDR09.bit.P3)
#define	PDDR09_P2  	(IO_PDDR09.bit.P2)
#define	PDDR09_P1  	(IO_PDDR09.bit.P1)
#define	PDDR09_P0  	(IO_PDDR09.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDDR10,attr=IO,locate=0xE4A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDDR10STR;

__IO_EXTENDED	 const  PDDR10STR	IO_PDDR10;
#define	_pddr10		(IO_PDDR10)
#define	PDDR10		(IO_PDDR10.byte)
#define	PDDR10_P7  	(IO_PDDR10.bit.P7)
#define	PDDR10_P6  	(IO_PDDR10.bit.P6)
#define	PDDR10_P5  	(IO_PDDR10.bit.P5)
#define	PDDR10_P4  	(IO_PDDR10.bit.P4)
#define	PDDR10_P3  	(IO_PDDR10.bit.P3)
#define	PDDR10_P2  	(IO_PDDR10.bit.P2)
#define	PDDR10_P1  	(IO_PDDR10.bit.P1)
#define	PDDR10_P0  	(IO_PDDR10.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDDR11,attr=IO,locate=0xE4B
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDDR11STR;

__IO_EXTENDED	 const  PDDR11STR	IO_PDDR11;
#define	_pddr11		(IO_PDDR11)
#define	PDDR11		(IO_PDDR11.byte)
#define	PDDR11_P7  	(IO_PDDR11.bit.P7)
#define	PDDR11_P6  	(IO_PDDR11.bit.P6)
#define	PDDR11_P5  	(IO_PDDR11.bit.P5)
#define	PDDR11_P4  	(IO_PDDR11.bit.P4)
#define	PDDR11_P3  	(IO_PDDR11.bit.P3)
#define	PDDR11_P2  	(IO_PDDR11.bit.P2)
#define	PDDR11_P1  	(IO_PDDR11.bit.P1)
#define	PDDR11_P0  	(IO_PDDR11.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDDR12,attr=IO,locate=0xE4C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDDR12STR;

__IO_EXTENDED	 const  PDDR12STR	IO_PDDR12;
#define	_pddr12		(IO_PDDR12)
#define	PDDR12		(IO_PDDR12.byte)
#define	PDDR12_P7  	(IO_PDDR12.bit.P7)
#define	PDDR12_P6  	(IO_PDDR12.bit.P6)
#define	PDDR12_P5  	(IO_PDDR12.bit.P5)
#define	PDDR12_P4  	(IO_PDDR12.bit.P4)
#define	PDDR12_P3  	(IO_PDDR12.bit.P3)
#define	PDDR12_P2  	(IO_PDDR12.bit.P2)
#define	PDDR12_P1  	(IO_PDDR12.bit.P1)
#define	PDDR12_P0  	(IO_PDDR12.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PDDR13,attr=IO,locate=0xE4D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	RESV2:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PDDR13STR;

__IO_EXTENDED	 const  PDDR13STR	IO_PDDR13;
#define	_pddr13		(IO_PDDR13)
#define	PDDR13		(IO_PDDR13.byte)
#define	PDDR13_P7  	(IO_PDDR13.bit.P7)
#define	PDDR13_P6  	(IO_PDDR13.bit.P6)
#define	PDDR13_P4  	(IO_PDDR13.bit.P4)
#define	PDDR13_P3  	(IO_PDDR13.bit.P3)
#define	PDDR13_P2  	(IO_PDDR13.bit.P2)
#define	PDDR13_P1  	(IO_PDDR13.bit.P1)
#define	PDDR13_P0  	(IO_PDDR13.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR00,attr=IO,locate=0xE60
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICU3E:2;
			__BYTE	ICU2E:2;
			__BYTE	ICU1E:2;
			__BYTE	ICU0E:2;
	} bit;
} EPFR00STR;

__IO_EXTENDED	  EPFR00STR	IO_EPFR00;
#define	_epfr00		(IO_EPFR00)
#define	EPFR00		(IO_EPFR00.byte)
#define	EPFR00_ICU3E  	(IO_EPFR00.bit.ICU3E)
#define	EPFR00_ICU2E  	(IO_EPFR00.bit.ICU2E)
#define	EPFR00_ICU1E  	(IO_EPFR00.bit.ICU1E)
#define	EPFR00_ICU0E  	(IO_EPFR00.bit.ICU0E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR01,attr=IO,locate=0xE61
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICU7E:2;
			__BYTE	ICU6E:2;
			__BYTE	ICU5E:2;
			__BYTE	ICU4E:2;
	} bit;
} EPFR01STR;

__IO_EXTENDED	  EPFR01STR	IO_EPFR01;
#define	_epfr01		(IO_EPFR01)
#define	EPFR01		(IO_EPFR01.byte)
#define	EPFR01_ICU7E  	(IO_EPFR01.bit.ICU7E)
#define	EPFR01_ICU6E  	(IO_EPFR01.bit.ICU6E)
#define	EPFR01_ICU5E  	(IO_EPFR01.bit.ICU5E)
#define	EPFR01_ICU4E  	(IO_EPFR01.bit.ICU4E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR02,attr=IO,locate=0xE62
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	TOT0E:3;
			__BYTE	TIN0E:2;
	} bit;
} EPFR02STR;

__IO_EXTENDED	  EPFR02STR	IO_EPFR02;
#define	_epfr02		(IO_EPFR02)
#define	EPFR02		(IO_EPFR02.byte)
#define	EPFR02_TOT0E  	(IO_EPFR02.bit.TOT0E)
#define	EPFR02_TIN0E  	(IO_EPFR02.bit.TIN0E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR03,attr=IO,locate=0xE63
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	TOT1E:3;
			__BYTE	TIN1E:2;
	} bit;
} EPFR03STR;

__IO_EXTENDED	  EPFR03STR	IO_EPFR03;
#define	_epfr03		(IO_EPFR03)
#define	EPFR03		(IO_EPFR03.byte)
#define	EPFR03_TOT1E  	(IO_EPFR03.bit.TOT1E)
#define	EPFR03_TIN1E  	(IO_EPFR03.bit.TIN1E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR04,attr=IO,locate=0xE64
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	TOT2E:3;
			__BYTE	TIN2E:2;
	} bit;
} EPFR04STR;

__IO_EXTENDED	  EPFR04STR	IO_EPFR04;
#define	_epfr04		(IO_EPFR04)
#define	EPFR04		(IO_EPFR04.byte)
#define	EPFR04_TOT2E  	(IO_EPFR04.bit.TOT2E)
#define	EPFR04_TIN2E  	(IO_EPFR04.bit.TIN2E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR05,attr=IO,locate=0xE65
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	TOT3E:3;
			__BYTE	TIN3E:2;
	} bit;
} EPFR05STR;

__IO_EXTENDED	  EPFR05STR	IO_EPFR05;
#define	_epfr05		(IO_EPFR05)
#define	EPFR05		(IO_EPFR05.byte)
#define	EPFR05_TOT3E  	(IO_EPFR05.bit.TOT3E)
#define	EPFR05_TIN3E  	(IO_EPFR05.bit.TIN3E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR06,attr=IO,locate=0xE66
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	SOT2E:2;
			__BYTE	SCK2E:2;
			__BYTE	SIN2E:1;
	} bit;
} EPFR06STR;

__IO_EXTENDED	  EPFR06STR	IO_EPFR06;
#define	_epfr06		(IO_EPFR06)
#define	EPFR06		(IO_EPFR06.byte)
#define	EPFR06_SOT2E  	(IO_EPFR06.bit.SOT2E)
#define	EPFR06_SCK2E  	(IO_EPFR06.bit.SCK2E)
#define	EPFR06_SIN2E  	(IO_EPFR06.bit.SIN2E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR07,attr=IO,locate=0xE67
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	SOT3E:2;
			__BYTE	SCK3E:2;
			__BYTE	SIN3E:1;
	} bit;
} EPFR07STR;

__IO_EXTENDED	  EPFR07STR	IO_EPFR07;
#define	_epfr07		(IO_EPFR07)
#define	EPFR07		(IO_EPFR07.byte)
#define	EPFR07_SOT3E  	(IO_EPFR07.bit.SOT3E)
#define	EPFR07_SCK3E  	(IO_EPFR07.bit.SCK3E)
#define	EPFR07_SIN3E  	(IO_EPFR07.bit.SIN3E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR08,attr=IO,locate=0xE68
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	SOT4E:2;
			__BYTE	SCK4E:2;
			__BYTE	SIN4E:1;
	} bit;
} EPFR08STR;

__IO_EXTENDED	  EPFR08STR	IO_EPFR08;
#define	_epfr08		(IO_EPFR08)
#define	EPFR08		(IO_EPFR08.byte)
#define	EPFR08_SOT4E  	(IO_EPFR08.bit.SOT4E)
#define	EPFR08_SCK4E  	(IO_EPFR08.bit.SCK4E)
#define	EPFR08_SIN4E  	(IO_EPFR08.bit.SIN4E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR09,attr=IO,locate=0xE69
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	SOT5E:2;
			__BYTE	SCK5E:2;
			__BYTE	SIN5E:1;
	} bit;
} EPFR09STR;

__IO_EXTENDED	  EPFR09STR	IO_EPFR09;
#define	_epfr09		(IO_EPFR09)
#define	EPFR09		(IO_EPFR09.byte)
#define	EPFR09_SOT5E  	(IO_EPFR09.bit.SOT5E)
#define	EPFR09_SCK5E  	(IO_EPFR09.bit.SCK5E)
#define	EPFR09_SIN5E  	(IO_EPFR09.bit.SIN5E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR10,attr=IO,locate=0xE6A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	PPG1E:4;
			__BYTE	PPG0E:3;
	} bit;
} EPFR10STR;

__IO_EXTENDED	  EPFR10STR	IO_EPFR10;
#define	_epfr10		(IO_EPFR10)
#define	EPFR10		(IO_EPFR10.byte)
#define	EPFR10_PPG1E  	(IO_EPFR10.bit.PPG1E)
#define	EPFR10_PPG0E  	(IO_EPFR10.bit.PPG0E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR11,attr=IO,locate=0xE6B
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	PPG3E:3;
			__BYTE	PPG2E:3;
	} bit;
} EPFR11STR;

__IO_EXTENDED	  EPFR11STR	IO_EPFR11;
#define	_epfr11		(IO_EPFR11)
#define	EPFR11		(IO_EPFR11.byte)
#define	EPFR11_PPG3E  	(IO_EPFR11.bit.PPG3E)
#define	EPFR11_PPG2E  	(IO_EPFR11.bit.PPG2E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR12,attr=IO,locate=0xE6C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	PPG5E:3;
			__BYTE	PPG4E:3;
	} bit;
} EPFR12STR;

__IO_EXTENDED	  EPFR12STR	IO_EPFR12;
#define	_epfr12		(IO_EPFR12)
#define	EPFR12		(IO_EPFR12.byte)
#define	EPFR12_PPG5E  	(IO_EPFR12.bit.PPG5E)
#define	EPFR12_PPG4E  	(IO_EPFR12.bit.PPG4E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR13,attr=IO,locate=0xE6D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	PPG7E:3;
			__BYTE	PPG6E:3;
	} bit;
} EPFR13STR;

__IO_EXTENDED	  EPFR13STR	IO_EPFR13;
#define	_epfr13		(IO_EPFR13)
#define	EPFR13		(IO_EPFR13.byte)
#define	EPFR13_PPG7E  	(IO_EPFR13.bit.PPG7E)
#define	EPFR13_PPG6E  	(IO_EPFR13.bit.PPG6E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR14,attr=IO,locate=0xE6E
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	PPG9E:3;
			__BYTE	PPG8E:3;
	} bit;
} EPFR14STR;

__IO_EXTENDED	  EPFR14STR	IO_EPFR14;
#define	_epfr14		(IO_EPFR14)
#define	EPFR14		(IO_EPFR14.byte)
#define	EPFR14_PPG9E  	(IO_EPFR14.bit.PPG9E)
#define	EPFR14_PPG8E  	(IO_EPFR14.bit.PPG8E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR15,attr=IO,locate=0xE6F
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	PPG12E:2;
			__BYTE	PPG11E:2;
			__BYTE	PPG10E:3;
	} bit;
} EPFR15STR;

__IO_EXTENDED	  EPFR15STR	IO_EPFR15;
#define	_epfr15		(IO_EPFR15)
#define	EPFR15		(IO_EPFR15.byte)
#define	EPFR15_PPG12E  	(IO_EPFR15.bit.PPG12E)
#define	EPFR15_PPG11E  	(IO_EPFR15.bit.PPG11E)
#define	EPFR15_PPG10E  	(IO_EPFR15.bit.PPG10E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR16,attr=IO,locate=0xE70
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	SEG7E:1;
			__BYTE	SEG6E:1;
			__BYTE	SEG5E:1;
			__BYTE	SEG4E:1;
			__BYTE	SEG3E:1;
			__BYTE	SEG2E:1;
			__BYTE	SEG1E:1;
			__BYTE	SEG0E:1;
	} bit;
} EPFR16STR;

__IO_EXTENDED	  EPFR16STR	IO_EPFR16;
#define	_epfr16		(IO_EPFR16)
#define	EPFR16		(IO_EPFR16.byte)
#define	EPFR16_SEG7E  	(IO_EPFR16.bit.SEG7E)
#define	EPFR16_SEG6E  	(IO_EPFR16.bit.SEG6E)
#define	EPFR16_SEG5E  	(IO_EPFR16.bit.SEG5E)
#define	EPFR16_SEG4E  	(IO_EPFR16.bit.SEG4E)
#define	EPFR16_SEG3E  	(IO_EPFR16.bit.SEG3E)
#define	EPFR16_SEG2E  	(IO_EPFR16.bit.SEG2E)
#define	EPFR16_SEG1E  	(IO_EPFR16.bit.SEG1E)
#define	EPFR16_SEG0E  	(IO_EPFR16.bit.SEG0E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR17,attr=IO,locate=0xE71
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	SEG15E:1;
			__BYTE	SEG14E:1;
			__BYTE	SEG13E:1;
			__BYTE	SEG12E:1;
			__BYTE	SEG11E:1;
			__BYTE	SEG10E:1;
			__BYTE	SEG9E:1;
			__BYTE	SEG8E:1;
	} bit;
} EPFR17STR;

__IO_EXTENDED	  EPFR17STR	IO_EPFR17;
#define	_epfr17		(IO_EPFR17)
#define	EPFR17		(IO_EPFR17.byte)
#define	EPFR17_SEG15E  	(IO_EPFR17.bit.SEG15E)
#define	EPFR17_SEG14E  	(IO_EPFR17.bit.SEG14E)
#define	EPFR17_SEG13E  	(IO_EPFR17.bit.SEG13E)
#define	EPFR17_SEG12E  	(IO_EPFR17.bit.SEG12E)
#define	EPFR17_SEG11E  	(IO_EPFR17.bit.SEG11E)
#define	EPFR17_SEG10E  	(IO_EPFR17.bit.SEG10E)
#define	EPFR17_SEG9E  	(IO_EPFR17.bit.SEG9E)
#define	EPFR17_SEG8E  	(IO_EPFR17.bit.SEG8E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR18,attr=IO,locate=0xE72
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	SEG23E:1;
			__BYTE	SEG22E:1;
			__BYTE	SEG21E:1;
			__BYTE	SEG20E:1;
			__BYTE	SEG19E:1;
			__BYTE	SEG18E:1;
			__BYTE	SEG17E:1;
			__BYTE	SEG16E:1;
	} bit;
} EPFR18STR;

__IO_EXTENDED	  EPFR18STR	IO_EPFR18;
#define	_epfr18		(IO_EPFR18)
#define	EPFR18		(IO_EPFR18.byte)
#define	EPFR18_SEG23E  	(IO_EPFR18.bit.SEG23E)
#define	EPFR18_SEG22E  	(IO_EPFR18.bit.SEG22E)
#define	EPFR18_SEG21E  	(IO_EPFR18.bit.SEG21E)
#define	EPFR18_SEG20E  	(IO_EPFR18.bit.SEG20E)
#define	EPFR18_SEG19E  	(IO_EPFR18.bit.SEG19E)
#define	EPFR18_SEG18E  	(IO_EPFR18.bit.SEG18E)
#define	EPFR18_SEG17E  	(IO_EPFR18.bit.SEG17E)
#define	EPFR18_SEG16E  	(IO_EPFR18.bit.SEG16E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR19,attr=IO,locate=0xE73
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	SEG31E:1;
			__BYTE	SEG30E:1;
			__BYTE	SEG29E:1;
			__BYTE	SEG28E:1;
			__BYTE	SEG27E:1;
			__BYTE	SEG26E:1;
			__BYTE	SEG25E:1;
			__BYTE	SEG24E:1;
	} bit;
} EPFR19STR;

__IO_EXTENDED	  EPFR19STR	IO_EPFR19;
#define	_epfr19		(IO_EPFR19)
#define	EPFR19		(IO_EPFR19.byte)
#define	EPFR19_SEG31E  	(IO_EPFR19.bit.SEG31E)
#define	EPFR19_SEG30E  	(IO_EPFR19.bit.SEG30E)
#define	EPFR19_SEG29E  	(IO_EPFR19.bit.SEG29E)
#define	EPFR19_SEG28E  	(IO_EPFR19.bit.SEG28E)
#define	EPFR19_SEG27E  	(IO_EPFR19.bit.SEG27E)
#define	EPFR19_SEG26E  	(IO_EPFR19.bit.SEG26E)
#define	EPFR19_SEG25E  	(IO_EPFR19.bit.SEG25E)
#define	EPFR19_SEG24E  	(IO_EPFR19.bit.SEG24E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR20,attr=IO,locate=0xE74
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	V3E:1;
			__BYTE	V2E:1;
			__BYTE	V1E:1;
			__BYTE	V0E:1;
			__BYTE	COM3E:1;
			__BYTE	COM2E:1;
			__BYTE	COM1E:1;
			__BYTE	COM0E:1;
	} bit;
} EPFR20STR;

__IO_EXTENDED	  EPFR20STR	IO_EPFR20;
#define	_epfr20		(IO_EPFR20)
#define	EPFR20		(IO_EPFR20.byte)
#define	EPFR20_V3E  	(IO_EPFR20.bit.V3E)
#define	EPFR20_V2E  	(IO_EPFR20.bit.V2E)
#define	EPFR20_V1E  	(IO_EPFR20.bit.V1E)
#define	EPFR20_V0E  	(IO_EPFR20.bit.V0E)
#define	EPFR20_COM3E  	(IO_EPFR20.bit.COM3E)
#define	EPFR20_COM2E  	(IO_EPFR20.bit.COM2E)
#define	EPFR20_COM1E  	(IO_EPFR20.bit.COM1E)
#define	EPFR20_COM0E  	(IO_EPFR20.bit.COM0E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR21,attr=IO,locate=0xE75
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	PWM2M1E:1;
			__BYTE	PWM2P1E:1;
			__BYTE	PWM1M1E:1;
			__BYTE	PWM1P1E:1;
			__BYTE	PWM2M0E:1;
			__BYTE	PWM2P0E:1;
			__BYTE	PWM1M0E:1;
			__BYTE	PWM1P0E:1;
	} bit;
} EPFR21STR;

__IO_EXTENDED	  EPFR21STR	IO_EPFR21;
#define	_epfr21		(IO_EPFR21)
#define	EPFR21		(IO_EPFR21.byte)
#define	EPFR21_PWM2M1E  	(IO_EPFR21.bit.PWM2M1E)
#define	EPFR21_PWM2P1E  	(IO_EPFR21.bit.PWM2P1E)
#define	EPFR21_PWM1M1E  	(IO_EPFR21.bit.PWM1M1E)
#define	EPFR21_PWM1P1E  	(IO_EPFR21.bit.PWM1P1E)
#define	EPFR21_PWM2M0E  	(IO_EPFR21.bit.PWM2M0E)
#define	EPFR21_PWM2P0E  	(IO_EPFR21.bit.PWM2P0E)
#define	EPFR21_PWM1M0E  	(IO_EPFR21.bit.PWM1M0E)
#define	EPFR21_PWM1P0E  	(IO_EPFR21.bit.PWM1P0E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR22,attr=IO,locate=0xE76
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	PWM2M3E:1;
			__BYTE	PWM2P3E:1;
			__BYTE	PWM1M3E:1;
			__BYTE	PWM1P3E:1;
			__BYTE	PWM2M2E:1;
			__BYTE	PWM2P2E:1;
			__BYTE	PWM1M2E:1;
			__BYTE	PWM1P2E:1;
	} bit;
} EPFR22STR;

__IO_EXTENDED	  EPFR22STR	IO_EPFR22;
#define	_epfr22		(IO_EPFR22)
#define	EPFR22		(IO_EPFR22.byte)
#define	EPFR22_PWM2M3E  	(IO_EPFR22.bit.PWM2M3E)
#define	EPFR22_PWM2P3E  	(IO_EPFR22.bit.PWM2P3E)
#define	EPFR22_PWM1M3E  	(IO_EPFR22.bit.PWM1M3E)
#define	EPFR22_PWM1P3E  	(IO_EPFR22.bit.PWM1P3E)
#define	EPFR22_PWM2M2E  	(IO_EPFR22.bit.PWM2M2E)
#define	EPFR22_PWM2P2E  	(IO_EPFR22.bit.PWM2P2E)
#define	EPFR22_PWM1M2E  	(IO_EPFR22.bit.PWM1M2E)
#define	EPFR22_PWM1P2E  	(IO_EPFR22.bit.PWM1P2E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR23,attr=IO,locate=0xE77
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	PWM2M5E:1;
			__BYTE	PWM2P5E:1;
			__BYTE	PWM1M5E:1;
			__BYTE	PWM1P5E:1;
			__BYTE	PWM2M4E:1;
			__BYTE	PWM2P4E:1;
			__BYTE	PWM1M4E:1;
			__BYTE	PWM1P4E:1;
	} bit;
} EPFR23STR;

__IO_EXTENDED	  EPFR23STR	IO_EPFR23;
#define	_epfr23		(IO_EPFR23)
#define	EPFR23		(IO_EPFR23.byte)
#define	EPFR23_PWM2M5E  	(IO_EPFR23.bit.PWM2M5E)
#define	EPFR23_PWM2P5E  	(IO_EPFR23.bit.PWM2P5E)
#define	EPFR23_PWM1M5E  	(IO_EPFR23.bit.PWM1M5E)
#define	EPFR23_PWM1P5E  	(IO_EPFR23.bit.PWM1P5E)
#define	EPFR23_PWM2M4E  	(IO_EPFR23.bit.PWM2M4E)
#define	EPFR23_PWM2P4E  	(IO_EPFR23.bit.PWM2P4E)
#define	EPFR23_PWM1M4E  	(IO_EPFR23.bit.PWM1M4E)
#define	EPFR23_PWM1P4E  	(IO_EPFR23.bit.PWM1P4E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR24,attr=IO,locate=0xE78
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	TX2E:1;
			__BYTE	TX1E:1;
			__BYTE	TX0E:1;
	} bit;
} EPFR24STR;

__IO_EXTENDED	  EPFR24STR	IO_EPFR24;
#define	_epfr24		(IO_EPFR24)
#define	EPFR24		(IO_EPFR24.byte)
#define	EPFR24_TX2E  	(IO_EPFR24.bit.TX2E)
#define	EPFR24_TX1E  	(IO_EPFR24.bit.TX1E)
#define	EPFR24_TX0E  	(IO_EPFR24.bit.TX0E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR25,attr=IO,locate=0xE79
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	EPFR25D:3;
	} bit;
} EPFR25STR;

__IO_EXTENDED	  EPFR25STR	IO_EPFR25;
#define	_epfr25		(IO_EPFR25)
#define	EPFR25		(IO_EPFR25.byte)
#define	EPFR25_EPFR25D  	(IO_EPFR25.bit.EPFR25D)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR26,attr=IO,locate=0xE7A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	TIB1E:1;
			__BYTE	TIB0E:1;
			__BYTE	TIA1E:1;
			__BYTE	TIA0E:1;
	} bit;
} EPFR26STR;

__IO_EXTENDED	  EPFR26STR	IO_EPFR26;
#define	_epfr26		(IO_EPFR26)
#define	EPFR26		(IO_EPFR26.byte)
#define	EPFR26_TIB1E  	(IO_EPFR26.bit.TIB1E)
#define	EPFR26_TIB0E  	(IO_EPFR26.bit.TIB0E)
#define	EPFR26_TIA1E  	(IO_EPFR26.bit.TIA1E)
#define	EPFR26_TIA0E  	(IO_EPFR26.bit.TIA0E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR27,attr=IO,locate=0xE7B
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	WOTE:1;
			__BYTE	SGO1E:1;
			__BYTE	SGA1E:1;
			__BYTE	SGO0E:1;
			__BYTE	SGA0E:1;
	} bit;
} EPFR27STR;

__IO_EXTENDED	  EPFR27STR	IO_EPFR27;
#define	_epfr27		(IO_EPFR27)
#define	EPFR27		(IO_EPFR27.byte)
#define	EPFR27_WOTE  	(IO_EPFR27.bit.WOTE)
#define	EPFR27_SGO1E  	(IO_EPFR27.bit.SGO1E)
#define	EPFR27_SGA1E  	(IO_EPFR27.bit.SGA1E)
#define	EPFR27_SGO0E  	(IO_EPFR27.bit.SGO0E)
#define	EPFR27_SGA0E  	(IO_EPFR27.bit.SGA0E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR28,attr=IO,locate=0xE7C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	FRCK3E:1;
			__BYTE	FRCK2E:1;
			__BYTE	FRCK1E:1;
			__BYTE	FRCK0E:1;
	} bit;
} EPFR28STR;

__IO_EXTENDED	  EPFR28STR	IO_EPFR28;
#define	_epfr28		(IO_EPFR28)
#define	EPFR28		(IO_EPFR28.byte)
#define	EPFR28_FRCK3E  	(IO_EPFR28.bit.FRCK3E)
#define	EPFR28_FRCK2E  	(IO_EPFR28.bit.FRCK2E)
#define	EPFR28_FRCK1E  	(IO_EPFR28.bit.FRCK1E)
#define	EPFR28_FRCK0E  	(IO_EPFR28.bit.FRCK0E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR29,attr=IO,locate=0xE7D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	OCU3E:2;
			__BYTE	OCU2E:2;
			__BYTE	OCU1E:2;
			__BYTE	OCU0E:2;
	} bit;
} EPFR29STR;

__IO_EXTENDED	  EPFR29STR	IO_EPFR29;
#define	_epfr29		(IO_EPFR29)
#define	EPFR29		(IO_EPFR29.byte)
#define	EPFR29_OCU3E  	(IO_EPFR29.bit.OCU3E)
#define	EPFR29_OCU2E  	(IO_EPFR29.bit.OCU2E)
#define	EPFR29_OCU1E  	(IO_EPFR29.bit.OCU1E)
#define	EPFR29_OCU0E  	(IO_EPFR29.bit.OCU0E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR30,attr=IO,locate=0xE7E
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	SGO4E:2;
			__BYTE	SGA4E:2;
			__BYTE	SGO3E:1;
			__BYTE	SGA3E:1;
			__BYTE	SGO2E:1;
			__BYTE	SGA2E:1;
	} bit;
} EPFR30STR;

__IO_EXTENDED	  EPFR30STR	IO_EPFR30;
#define	_epfr30		(IO_EPFR30)
#define	EPFR30		(IO_EPFR30.byte)
#define	EPFR30_SGO4E  	(IO_EPFR30.bit.SGO4E)
#define	EPFR30_SGA4E  	(IO_EPFR30.bit.SGA4E)
#define	EPFR30_SGO3E  	(IO_EPFR30.bit.SGO3E)
#define	EPFR30_SGA3E  	(IO_EPFR30.bit.SGA3E)
#define	EPFR30_SGO2E  	(IO_EPFR30.bit.SGO2E)
#define	EPFR30_SGA2E  	(IO_EPFR30.bit.SGA2E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR31,attr=IO,locate=0xE7F
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	OCU7E:2;
			__BYTE	OCU6E:2;
			__BYTE	OCU5E:2;
			__BYTE	OCU4E:2;
	} bit;
} EPFR31STR;

__IO_EXTENDED	  EPFR31STR	IO_EPFR31;
#define	_epfr31		(IO_EPFR31)
#define	EPFR31		(IO_EPFR31.byte)
#define	EPFR31_OCU7E  	(IO_EPFR31.bit.OCU7E)
#define	EPFR31_OCU6E  	(IO_EPFR31.bit.OCU6E)
#define	EPFR31_OCU5E  	(IO_EPFR31.bit.OCU5E)
#define	EPFR31_OCU4E  	(IO_EPFR31.bit.OCU4E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR32,attr=IO,locate=0xE80
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	OCU11E:2;
			__BYTE	OCU10E:2;
			__BYTE	OCU9E:2;
			__BYTE	OCU8E:2;
	} bit;
} EPFR32STR;

__IO_EXTENDED	  EPFR32STR	IO_EPFR32;
#define	_epfr32		(IO_EPFR32)
#define	EPFR32		(IO_EPFR32.byte)
#define	EPFR32_OCU11E  	(IO_EPFR32.bit.OCU11E)
#define	EPFR32_OCU10E  	(IO_EPFR32.bit.OCU10E)
#define	EPFR32_OCU9E  	(IO_EPFR32.bit.OCU9E)
#define	EPFR32_OCU8E  	(IO_EPFR32.bit.OCU8E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR33,attr=IO,locate=0xE81
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	SOT6E:2;
			__BYTE	SCK6E:2;
			__BYTE	SIN6E:1;
	} bit;
} EPFR33STR;

__IO_EXTENDED	  EPFR33STR	IO_EPFR33;
#define	_epfr33		(IO_EPFR33)
#define	EPFR33		(IO_EPFR33.byte)
#define	EPFR33_SOT6E  	(IO_EPFR33.bit.SOT6E)
#define	EPFR33_SCK6E  	(IO_EPFR33.bit.SCK6E)
#define	EPFR33_SIN6E  	(IO_EPFR33.bit.SIN6E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR34,attr=IO,locate=0xE82
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	SOT7E:2;
			__BYTE	SCK7E:2;
			__BYTE	SIN7E:1;
	} bit;
} EPFR34STR;

__IO_EXTENDED	  EPFR34STR	IO_EPFR34;
#define	_epfr34		(IO_EPFR34)
#define	EPFR34		(IO_EPFR34.byte)
#define	EPFR34_SOT7E  	(IO_EPFR34.bit.SOT7E)
#define	EPFR34_SCK7E  	(IO_EPFR34.bit.SCK7E)
#define	EPFR34_SIN7E  	(IO_EPFR34.bit.SIN7E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR35,attr=IO,locate=0xE83
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	SOT0E:2;
			__BYTE	SCK0E:2;
			__BYTE	SIN0E:1;
	} bit;
} EPFR35STR;

__IO_EXTENDED	  EPFR35STR	IO_EPFR35;
#define	_epfr35		(IO_EPFR35)
#define	EPFR35		(IO_EPFR35.byte)
#define	EPFR35_SOT0E  	(IO_EPFR35.bit.SOT0E)
#define	EPFR35_SCK0E  	(IO_EPFR35.bit.SCK0E)
#define	EPFR35_SIN0E  	(IO_EPFR35.bit.SIN0E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR36,attr=IO,locate=0xE84
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	SOT1E:2;
			__BYTE	SCK1E:2;
			__BYTE	SIN1E:1;
	} bit;
} EPFR36STR;

__IO_EXTENDED	  EPFR36STR	IO_EPFR36;
#define	_epfr36		(IO_EPFR36)
#define	EPFR36		(IO_EPFR36.byte)
#define	EPFR36_SOT1E  	(IO_EPFR36.bit.SOT1E)
#define	EPFR36_SCK1E  	(IO_EPFR36.bit.SCK1E)
#define	EPFR36_SIN1E  	(IO_EPFR36.bit.SIN1E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR37,attr=IO,locate=0xE85
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	SOT8E:3;
			__BYTE	SCK8E:3;
			__BYTE	SIN8E:2;
	} bit;
} EPFR37STR;

__IO_EXTENDED	  EPFR37STR	IO_EPFR37;
#define	_epfr37		(IO_EPFR37)
#define	EPFR37		(IO_EPFR37.byte)
#define	EPFR37_SOT8E  	(IO_EPFR37.bit.SOT8E)
#define	EPFR37_SCK8E  	(IO_EPFR37.bit.SCK8E)
#define	EPFR37_SIN8E  	(IO_EPFR37.bit.SIN8E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR38,attr=IO,locate=0xE86
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	SOT9E:2;
			__BYTE	SCK9E:2;
			__BYTE	SIN9E:1;
	} bit;
} EPFR38STR;

__IO_EXTENDED	  EPFR38STR	IO_EPFR38;
#define	_epfr38		(IO_EPFR38)
#define	EPFR38		(IO_EPFR38.byte)
#define	EPFR38_SOT9E  	(IO_EPFR38.bit.SOT9E)
#define	EPFR38_SCK9E  	(IO_EPFR38.bit.SCK9E)
#define	EPFR38_SIN9E  	(IO_EPFR38.bit.SIN9E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR39,attr=IO,locate=0xE87
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICU11E:2;
			__BYTE	ICU10E:2;
			__BYTE	ICU9E:2;
			__BYTE	ICU8E:2;
	} bit;
} EPFR39STR;

__IO_EXTENDED	  EPFR39STR	IO_EPFR39;
#define	_epfr39		(IO_EPFR39)
#define	EPFR39		(IO_EPFR39.byte)
#define	EPFR39_ICU11E  	(IO_EPFR39.bit.ICU11E)
#define	EPFR39_ICU10E  	(IO_EPFR39.bit.ICU10E)
#define	EPFR39_ICU9E  	(IO_EPFR39.bit.ICU9E)
#define	EPFR39_ICU8E  	(IO_EPFR39.bit.ICU8E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR40,attr=IO,locate=0xE88
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	ZIN0E:2;
			__BYTE	BIN0E:2;
			__BYTE	AIN0E:2;
	} bit;
} EPFR40STR;

__IO_EXTENDED	  EPFR40STR	IO_EPFR40;
#define	_epfr40		(IO_EPFR40)
#define	EPFR40		(IO_EPFR40.byte)
#define	EPFR40_ZIN0E  	(IO_EPFR40.bit.ZIN0E)
#define	EPFR40_BIN0E  	(IO_EPFR40.bit.BIN0E)
#define	EPFR40_AIN0E  	(IO_EPFR40.bit.AIN0E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR41,attr=IO,locate=0xE89
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	ZIN1E:1;
			__BYTE	BIN1E:1;
			__BYTE	AIN1E:1;
	} bit;
} EPFR41STR;

__IO_EXTENDED	  EPFR41STR	IO_EPFR41;
#define	_epfr41		(IO_EPFR41)
#define	EPFR41		(IO_EPFR41.byte)
#define	EPFR41_ZIN1E  	(IO_EPFR41.bit.ZIN1E)
#define	EPFR41_BIN1E  	(IO_EPFR41.bit.BIN1E)
#define	EPFR41_AIN1E  	(IO_EPFR41.bit.AIN1E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR42,attr=IO,locate=0xE8A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	DAS1:1;
			__BYTE	DAS0:1;
	} bit;
} EPFR42STR;

__IO_EXTENDED	  EPFR42STR	IO_EPFR42;
#define	_epfr42		(IO_EPFR42)
#define	EPFR42		(IO_EPFR42.byte)
#define	EPFR42_DAS1  	(IO_EPFR42.bit.DAS1)
#define	EPFR42_DAS0  	(IO_EPFR42.bit.DAS0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR43,attr=IO,locate=0xE8B
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	INT7E:1;
			__BYTE	INT6E:1;
			__BYTE	INT5E:1;
			__BYTE	INT4E:1;
			__BYTE	INT3E:1;
			__BYTE	INT2E:1;
			__BYTE	INT1E:1;
			__BYTE	INT0E:1;
	} bit;
} EPFR43STR;

__IO_EXTENDED	  EPFR43STR	IO_EPFR43;
#define	_epfr43		(IO_EPFR43)
#define	EPFR43		(IO_EPFR43.byte)
#define	EPFR43_INT7E  	(IO_EPFR43.bit.INT7E)
#define	EPFR43_INT6E  	(IO_EPFR43.bit.INT6E)
#define	EPFR43_INT5E  	(IO_EPFR43.bit.INT5E)
#define	EPFR43_INT4E  	(IO_EPFR43.bit.INT4E)
#define	EPFR43_INT3E  	(IO_EPFR43.bit.INT3E)
#define	EPFR43_INT2E  	(IO_EPFR43.bit.INT2E)
#define	EPFR43_INT1E  	(IO_EPFR43.bit.INT1E)
#define	EPFR43_INT0E  	(IO_EPFR43.bit.INT0E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR44,attr=IO,locate=0xE8C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	INT15E:1;
			__BYTE	INT14E:1;
			__BYTE	INT13E:1;
			__BYTE	INT12E:1;
			__BYTE	INT11E:1;
			__BYTE	INT10E:1;
			__BYTE	INT9E:1;
			__BYTE	INT8E:1;
	} bit;
} EPFR44STR;

__IO_EXTENDED	  EPFR44STR	IO_EPFR44;
#define	_epfr44		(IO_EPFR44)
#define	EPFR44		(IO_EPFR44.byte)
#define	EPFR44_INT15E  	(IO_EPFR44.bit.INT15E)
#define	EPFR44_INT14E  	(IO_EPFR44.bit.INT14E)
#define	EPFR44_INT13E  	(IO_EPFR44.bit.INT13E)
#define	EPFR44_INT12E  	(IO_EPFR44.bit.INT12E)
#define	EPFR44_INT11E  	(IO_EPFR44.bit.INT11E)
#define	EPFR44_INT10E  	(IO_EPFR44.bit.INT10E)
#define	EPFR44_INT9E  	(IO_EPFR44.bit.INT9E)
#define	EPFR44_INT8E  	(IO_EPFR44.bit.INT8E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR45,attr=IO,locate=0xE8D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	PPG17E:1;
			__BYTE	PPG16E:1;
			__BYTE	PPG15E:2;
			__BYTE	PPG14E:2;
			__BYTE	PPG13E:2;
	} bit;
} EPFR45STR;

__IO_EXTENDED	  EPFR45STR	IO_EPFR45;
#define	_epfr45		(IO_EPFR45)
#define	EPFR45		(IO_EPFR45.byte)
#define	EPFR45_PPG17E  	(IO_EPFR45.bit.PPG17E)
#define	EPFR45_PPG16E  	(IO_EPFR45.bit.PPG16E)
#define	EPFR45_PPG15E  	(IO_EPFR45.bit.PPG15E)
#define	EPFR45_PPG14E  	(IO_EPFR45.bit.PPG14E)
#define	EPFR45_PPG13E  	(IO_EPFR45.bit.PPG13E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR46,attr=IO,locate=0xE8E
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	PPG23E:1;
			__BYTE	PPG22E:1;
			__BYTE	PPP21E:1;
			__BYTE	PPG20E:1;
			__BYTE	PPG19E:1;
			__BYTE	PPG18E:1;
	} bit;
} EPFR46STR;

__IO_EXTENDED	  EPFR46STR	IO_EPFR46;
#define	_epfr46		(IO_EPFR46)
#define	EPFR46		(IO_EPFR46.byte)
#define	EPFR46_PPG23E  	(IO_EPFR46.bit.PPG23E)
#define	EPFR46_PPG22E  	(IO_EPFR46.bit.PPG22E)
#define	EPFR46_PPP21E  	(IO_EPFR46.bit.PPP21E)
#define	EPFR46_PPG20E  	(IO_EPFR46.bit.PPG20E)
#define	EPFR46_PPG19E  	(IO_EPFR46.bit.PPG19E)
#define	EPFR46_PPG18E  	(IO_EPFR46.bit.PPG18E)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR47,attr=IO,locate=0xE8F
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	EBDSWPE:1;
	} bit;
} EPFR47STR;

__IO_EXTENDED	  EPFR47STR	IO_EPFR47;
#define	_epfr47		(IO_EPFR47)
#define	EPFR47		(IO_EPFR47.byte)
#define	EPFR47_EBDSWPE  	(IO_EPFR47.bit.EBDSWPE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR52,attr=IO,locate=0xE94
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	HSSWAP:1;
	} bit;
} EPFR52STR;

__IO_EXTENDED	  EPFR52STR	IO_EPFR52;
#define	_epfr52		(IO_EPFR52)
#define	EPFR52		(IO_EPFR52.byte)
#define	EPFR52_HSSWAP  	(IO_EPFR52.bit.HSSWAP)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR53,attr=IO,locate=0xE95
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	HSCKE:1;
			__BYTE	HSDE:4;
	} bit;
} EPFR53STR;

__IO_EXTENDED	  EPFR53STR	IO_EPFR53;
#define	_epfr53		(IO_EPFR53)
#define	EPFR53		(IO_EPFR53.byte)
#define	EPFR53_HSCKE  	(IO_EPFR53.bit.HSCKE)
#define	EPFR53_HSDE  	(IO_EPFR53.bit.HSDE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPFR54,attr=IO,locate=0xE96
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	HSSELE:4;
	} bit;
} EPFR54STR;

__IO_EXTENDED	  EPFR54STR	IO_EPFR54;
#define	_epfr54		(IO_EPFR54)
#define	EPFR54		(IO_EPFR54.byte)
#define	EPFR54_HSSELE  	(IO_EPFR54.bit.HSSELE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPCR00,attr=IO,locate=0xEA0
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPCR00STR;

__IO_EXTENDED	  PPCR00STR	IO_PPCR00;
#define	_ppcr00		(IO_PPCR00)
#define	PPCR00		(IO_PPCR00.byte)
#define	PPCR00_P7  	(IO_PPCR00.bit.P7)
#define	PPCR00_P6  	(IO_PPCR00.bit.P6)
#define	PPCR00_P5  	(IO_PPCR00.bit.P5)
#define	PPCR00_P4  	(IO_PPCR00.bit.P4)
#define	PPCR00_P3  	(IO_PPCR00.bit.P3)
#define	PPCR00_P2  	(IO_PPCR00.bit.P2)
#define	PPCR00_P1  	(IO_PPCR00.bit.P1)
#define	PPCR00_P0  	(IO_PPCR00.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPCR01,attr=IO,locate=0xEA1
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPCR01STR;

__IO_EXTENDED	  PPCR01STR	IO_PPCR01;
#define	_ppcr01		(IO_PPCR01)
#define	PPCR01		(IO_PPCR01.byte)
#define	PPCR01_P7  	(IO_PPCR01.bit.P7)
#define	PPCR01_P6  	(IO_PPCR01.bit.P6)
#define	PPCR01_P5  	(IO_PPCR01.bit.P5)
#define	PPCR01_P4  	(IO_PPCR01.bit.P4)
#define	PPCR01_P3  	(IO_PPCR01.bit.P3)
#define	PPCR01_P2  	(IO_PPCR01.bit.P2)
#define	PPCR01_P1  	(IO_PPCR01.bit.P1)
#define	PPCR01_P0  	(IO_PPCR01.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPCR02,attr=IO,locate=0xEA2
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPCR02STR;

__IO_EXTENDED	  PPCR02STR	IO_PPCR02;
#define	_ppcr02		(IO_PPCR02)
#define	PPCR02		(IO_PPCR02.byte)
#define	PPCR02_P7  	(IO_PPCR02.bit.P7)
#define	PPCR02_P6  	(IO_PPCR02.bit.P6)
#define	PPCR02_P5  	(IO_PPCR02.bit.P5)
#define	PPCR02_P4  	(IO_PPCR02.bit.P4)
#define	PPCR02_P3  	(IO_PPCR02.bit.P3)
#define	PPCR02_P2  	(IO_PPCR02.bit.P2)
#define	PPCR02_P1  	(IO_PPCR02.bit.P1)
#define	PPCR02_P0  	(IO_PPCR02.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPCR03,attr=IO,locate=0xEA3
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPCR03STR;

__IO_EXTENDED	  PPCR03STR	IO_PPCR03;
#define	_ppcr03		(IO_PPCR03)
#define	PPCR03		(IO_PPCR03.byte)
#define	PPCR03_P7  	(IO_PPCR03.bit.P7)
#define	PPCR03_P6  	(IO_PPCR03.bit.P6)
#define	PPCR03_P5  	(IO_PPCR03.bit.P5)
#define	PPCR03_P4  	(IO_PPCR03.bit.P4)
#define	PPCR03_P3  	(IO_PPCR03.bit.P3)
#define	PPCR03_P2  	(IO_PPCR03.bit.P2)
#define	PPCR03_P1  	(IO_PPCR03.bit.P1)
#define	PPCR03_P0  	(IO_PPCR03.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPCR04,attr=IO,locate=0xEA4
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPCR04STR;

__IO_EXTENDED	  PPCR04STR	IO_PPCR04;
#define	_ppcr04		(IO_PPCR04)
#define	PPCR04		(IO_PPCR04.byte)
#define	PPCR04_P7  	(IO_PPCR04.bit.P7)
#define	PPCR04_P6  	(IO_PPCR04.bit.P6)
#define	PPCR04_P5  	(IO_PPCR04.bit.P5)
#define	PPCR04_P4  	(IO_PPCR04.bit.P4)
#define	PPCR04_P3  	(IO_PPCR04.bit.P3)
#define	PPCR04_P2  	(IO_PPCR04.bit.P2)
#define	PPCR04_P1  	(IO_PPCR04.bit.P1)
#define	PPCR04_P0  	(IO_PPCR04.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPCR05,attr=IO,locate=0xEA5
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPCR05STR;

__IO_EXTENDED	  PPCR05STR	IO_PPCR05;
#define	_ppcr05		(IO_PPCR05)
#define	PPCR05		(IO_PPCR05.byte)
#define	PPCR05_P7  	(IO_PPCR05.bit.P7)
#define	PPCR05_P6  	(IO_PPCR05.bit.P6)
#define	PPCR05_P5  	(IO_PPCR05.bit.P5)
#define	PPCR05_P4  	(IO_PPCR05.bit.P4)
#define	PPCR05_P3  	(IO_PPCR05.bit.P3)
#define	PPCR05_P2  	(IO_PPCR05.bit.P2)
#define	PPCR05_P1  	(IO_PPCR05.bit.P1)
#define	PPCR05_P0  	(IO_PPCR05.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPCR06,attr=IO,locate=0xEA6
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPCR06STR;

__IO_EXTENDED	  PPCR06STR	IO_PPCR06;
#define	_ppcr06		(IO_PPCR06)
#define	PPCR06		(IO_PPCR06.byte)
#define	PPCR06_P7  	(IO_PPCR06.bit.P7)
#define	PPCR06_P6  	(IO_PPCR06.bit.P6)
#define	PPCR06_P5  	(IO_PPCR06.bit.P5)
#define	PPCR06_P4  	(IO_PPCR06.bit.P4)
#define	PPCR06_P3  	(IO_PPCR06.bit.P3)
#define	PPCR06_P2  	(IO_PPCR06.bit.P2)
#define	PPCR06_P1  	(IO_PPCR06.bit.P1)
#define	PPCR06_P0  	(IO_PPCR06.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPCR07,attr=IO,locate=0xEA7
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPCR07STR;

__IO_EXTENDED	  PPCR07STR	IO_PPCR07;
#define	_ppcr07		(IO_PPCR07)
#define	PPCR07		(IO_PPCR07.byte)
#define	PPCR07_P7  	(IO_PPCR07.bit.P7)
#define	PPCR07_P6  	(IO_PPCR07.bit.P6)
#define	PPCR07_P5  	(IO_PPCR07.bit.P5)
#define	PPCR07_P4  	(IO_PPCR07.bit.P4)
#define	PPCR07_P3  	(IO_PPCR07.bit.P3)
#define	PPCR07_P2  	(IO_PPCR07.bit.P2)
#define	PPCR07_P1  	(IO_PPCR07.bit.P1)
#define	PPCR07_P0  	(IO_PPCR07.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPCR08,attr=IO,locate=0xEA8
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPCR08STR;

__IO_EXTENDED	  PPCR08STR	IO_PPCR08;
#define	_ppcr08		(IO_PPCR08)
#define	PPCR08		(IO_PPCR08.byte)
#define	PPCR08_P7  	(IO_PPCR08.bit.P7)
#define	PPCR08_P6  	(IO_PPCR08.bit.P6)
#define	PPCR08_P5  	(IO_PPCR08.bit.P5)
#define	PPCR08_P4  	(IO_PPCR08.bit.P4)
#define	PPCR08_P3  	(IO_PPCR08.bit.P3)
#define	PPCR08_P2  	(IO_PPCR08.bit.P2)
#define	PPCR08_P1  	(IO_PPCR08.bit.P1)
#define	PPCR08_P0  	(IO_PPCR08.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPCR09,attr=IO,locate=0xEA9
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPCR09STR;

__IO_EXTENDED	  PPCR09STR	IO_PPCR09;
#define	_ppcr09		(IO_PPCR09)
#define	PPCR09		(IO_PPCR09.byte)
#define	PPCR09_P7  	(IO_PPCR09.bit.P7)
#define	PPCR09_P6  	(IO_PPCR09.bit.P6)
#define	PPCR09_P5  	(IO_PPCR09.bit.P5)
#define	PPCR09_P4  	(IO_PPCR09.bit.P4)
#define	PPCR09_P3  	(IO_PPCR09.bit.P3)
#define	PPCR09_P2  	(IO_PPCR09.bit.P2)
#define	PPCR09_P1  	(IO_PPCR09.bit.P1)
#define	PPCR09_P0  	(IO_PPCR09.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPCR10,attr=IO,locate=0xEAA
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPCR10STR;

__IO_EXTENDED	  PPCR10STR	IO_PPCR10;
#define	_ppcr10		(IO_PPCR10)
#define	PPCR10		(IO_PPCR10.byte)
#define	PPCR10_P7  	(IO_PPCR10.bit.P7)
#define	PPCR10_P6  	(IO_PPCR10.bit.P6)
#define	PPCR10_P5  	(IO_PPCR10.bit.P5)
#define	PPCR10_P4  	(IO_PPCR10.bit.P4)
#define	PPCR10_P3  	(IO_PPCR10.bit.P3)
#define	PPCR10_P2  	(IO_PPCR10.bit.P2)
#define	PPCR10_P1  	(IO_PPCR10.bit.P1)
#define	PPCR10_P0  	(IO_PPCR10.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPCR11,attr=IO,locate=0xEAB
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPCR11STR;

__IO_EXTENDED	  PPCR11STR	IO_PPCR11;
#define	_ppcr11		(IO_PPCR11)
#define	PPCR11		(IO_PPCR11.byte)
#define	PPCR11_P7  	(IO_PPCR11.bit.P7)
#define	PPCR11_P6  	(IO_PPCR11.bit.P6)
#define	PPCR11_P5  	(IO_PPCR11.bit.P5)
#define	PPCR11_P4  	(IO_PPCR11.bit.P4)
#define	PPCR11_P3  	(IO_PPCR11.bit.P3)
#define	PPCR11_P2  	(IO_PPCR11.bit.P2)
#define	PPCR11_P1  	(IO_PPCR11.bit.P1)
#define	PPCR11_P0  	(IO_PPCR11.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPCR12,attr=IO,locate=0xEAC
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPCR12STR;

__IO_EXTENDED	  PPCR12STR	IO_PPCR12;
#define	_ppcr12		(IO_PPCR12)
#define	PPCR12		(IO_PPCR12.byte)
#define	PPCR12_P7  	(IO_PPCR12.bit.P7)
#define	PPCR12_P6  	(IO_PPCR12.bit.P6)
#define	PPCR12_P5  	(IO_PPCR12.bit.P5)
#define	PPCR12_P4  	(IO_PPCR12.bit.P4)
#define	PPCR12_P3  	(IO_PPCR12.bit.P3)
#define	PPCR12_P2  	(IO_PPCR12.bit.P2)
#define	PPCR12_P1  	(IO_PPCR12.bit.P1)
#define	PPCR12_P0  	(IO_PPCR12.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPCR13,attr=IO,locate=0xEAD
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	RESV2:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPCR13STR;

__IO_EXTENDED	  PPCR13STR	IO_PPCR13;
#define	_ppcr13		(IO_PPCR13)
#define	PPCR13		(IO_PPCR13.byte)
#define	PPCR13_P7  	(IO_PPCR13.bit.P7)
#define	PPCR13_P6  	(IO_PPCR13.bit.P6)
#define	PPCR13_P4  	(IO_PPCR13.bit.P4)
#define	PPCR13_P3  	(IO_PPCR13.bit.P3)
#define	PPCR13_P2  	(IO_PPCR13.bit.P2)
#define	PPCR13_P1  	(IO_PPCR13.bit.P1)
#define	PPCR13_P0  	(IO_PPCR13.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPER00,attr=IO,locate=0xEC0
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPER00STR;

__IO_EXTENDED	  PPER00STR	IO_PPER00;
#define	_pper00		(IO_PPER00)
#define	PPER00		(IO_PPER00.byte)
#define	PPER00_P7  	(IO_PPER00.bit.P7)
#define	PPER00_P6  	(IO_PPER00.bit.P6)
#define	PPER00_P5  	(IO_PPER00.bit.P5)
#define	PPER00_P4  	(IO_PPER00.bit.P4)
#define	PPER00_P3  	(IO_PPER00.bit.P3)
#define	PPER00_P2  	(IO_PPER00.bit.P2)
#define	PPER00_P1  	(IO_PPER00.bit.P1)
#define	PPER00_P0  	(IO_PPER00.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPER01,attr=IO,locate=0xEC1
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPER01STR;

__IO_EXTENDED	  PPER01STR	IO_PPER01;
#define	_pper01		(IO_PPER01)
#define	PPER01		(IO_PPER01.byte)
#define	PPER01_P7  	(IO_PPER01.bit.P7)
#define	PPER01_P6  	(IO_PPER01.bit.P6)
#define	PPER01_P5  	(IO_PPER01.bit.P5)
#define	PPER01_P4  	(IO_PPER01.bit.P4)
#define	PPER01_P3  	(IO_PPER01.bit.P3)
#define	PPER01_P2  	(IO_PPER01.bit.P2)
#define	PPER01_P1  	(IO_PPER01.bit.P1)
#define	PPER01_P0  	(IO_PPER01.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPER02,attr=IO,locate=0xEC2
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPER02STR;

__IO_EXTENDED	  PPER02STR	IO_PPER02;
#define	_pper02		(IO_PPER02)
#define	PPER02		(IO_PPER02.byte)
#define	PPER02_P7  	(IO_PPER02.bit.P7)
#define	PPER02_P6  	(IO_PPER02.bit.P6)
#define	PPER02_P5  	(IO_PPER02.bit.P5)
#define	PPER02_P4  	(IO_PPER02.bit.P4)
#define	PPER02_P3  	(IO_PPER02.bit.P3)
#define	PPER02_P2  	(IO_PPER02.bit.P2)
#define	PPER02_P1  	(IO_PPER02.bit.P1)
#define	PPER02_P0  	(IO_PPER02.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPER03,attr=IO,locate=0xEC3
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPER03STR;

__IO_EXTENDED	  PPER03STR	IO_PPER03;
#define	_pper03		(IO_PPER03)
#define	PPER03		(IO_PPER03.byte)
#define	PPER03_P7  	(IO_PPER03.bit.P7)
#define	PPER03_P6  	(IO_PPER03.bit.P6)
#define	PPER03_P5  	(IO_PPER03.bit.P5)
#define	PPER03_P4  	(IO_PPER03.bit.P4)
#define	PPER03_P3  	(IO_PPER03.bit.P3)
#define	PPER03_P2  	(IO_PPER03.bit.P2)
#define	PPER03_P1  	(IO_PPER03.bit.P1)
#define	PPER03_P0  	(IO_PPER03.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPER04,attr=IO,locate=0xEC4
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPER04STR;

__IO_EXTENDED	  PPER04STR	IO_PPER04;
#define	_pper04		(IO_PPER04)
#define	PPER04		(IO_PPER04.byte)
#define	PPER04_P7  	(IO_PPER04.bit.P7)
#define	PPER04_P6  	(IO_PPER04.bit.P6)
#define	PPER04_P5  	(IO_PPER04.bit.P5)
#define	PPER04_P4  	(IO_PPER04.bit.P4)
#define	PPER04_P3  	(IO_PPER04.bit.P3)
#define	PPER04_P2  	(IO_PPER04.bit.P2)
#define	PPER04_P1  	(IO_PPER04.bit.P1)
#define	PPER04_P0  	(IO_PPER04.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPER05,attr=IO,locate=0xEC5
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPER05STR;

__IO_EXTENDED	  PPER05STR	IO_PPER05;
#define	_pper05		(IO_PPER05)
#define	PPER05		(IO_PPER05.byte)
#define	PPER05_P7  	(IO_PPER05.bit.P7)
#define	PPER05_P6  	(IO_PPER05.bit.P6)
#define	PPER05_P5  	(IO_PPER05.bit.P5)
#define	PPER05_P4  	(IO_PPER05.bit.P4)
#define	PPER05_P3  	(IO_PPER05.bit.P3)
#define	PPER05_P2  	(IO_PPER05.bit.P2)
#define	PPER05_P1  	(IO_PPER05.bit.P1)
#define	PPER05_P0  	(IO_PPER05.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPER06,attr=IO,locate=0xEC6
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPER06STR;

__IO_EXTENDED	  PPER06STR	IO_PPER06;
#define	_pper06		(IO_PPER06)
#define	PPER06		(IO_PPER06.byte)
#define	PPER06_P7  	(IO_PPER06.bit.P7)
#define	PPER06_P6  	(IO_PPER06.bit.P6)
#define	PPER06_P5  	(IO_PPER06.bit.P5)
#define	PPER06_P4  	(IO_PPER06.bit.P4)
#define	PPER06_P3  	(IO_PPER06.bit.P3)
#define	PPER06_P2  	(IO_PPER06.bit.P2)
#define	PPER06_P1  	(IO_PPER06.bit.P1)
#define	PPER06_P0  	(IO_PPER06.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPER07,attr=IO,locate=0xEC7
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPER07STR;

__IO_EXTENDED	  PPER07STR	IO_PPER07;
#define	_pper07		(IO_PPER07)
#define	PPER07		(IO_PPER07.byte)
#define	PPER07_P7  	(IO_PPER07.bit.P7)
#define	PPER07_P6  	(IO_PPER07.bit.P6)
#define	PPER07_P5  	(IO_PPER07.bit.P5)
#define	PPER07_P4  	(IO_PPER07.bit.P4)
#define	PPER07_P3  	(IO_PPER07.bit.P3)
#define	PPER07_P2  	(IO_PPER07.bit.P2)
#define	PPER07_P1  	(IO_PPER07.bit.P1)
#define	PPER07_P0  	(IO_PPER07.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPER08,attr=IO,locate=0xEC8
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPER08STR;

__IO_EXTENDED	  PPER08STR	IO_PPER08;
#define	_pper08		(IO_PPER08)
#define	PPER08		(IO_PPER08.byte)
#define	PPER08_P7  	(IO_PPER08.bit.P7)
#define	PPER08_P6  	(IO_PPER08.bit.P6)
#define	PPER08_P5  	(IO_PPER08.bit.P5)
#define	PPER08_P4  	(IO_PPER08.bit.P4)
#define	PPER08_P3  	(IO_PPER08.bit.P3)
#define	PPER08_P2  	(IO_PPER08.bit.P2)
#define	PPER08_P1  	(IO_PPER08.bit.P1)
#define	PPER08_P0  	(IO_PPER08.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPER09,attr=IO,locate=0xEC9
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPER09STR;

__IO_EXTENDED	  PPER09STR	IO_PPER09;
#define	_pper09		(IO_PPER09)
#define	PPER09		(IO_PPER09.byte)
#define	PPER09_P7  	(IO_PPER09.bit.P7)
#define	PPER09_P6  	(IO_PPER09.bit.P6)
#define	PPER09_P5  	(IO_PPER09.bit.P5)
#define	PPER09_P4  	(IO_PPER09.bit.P4)
#define	PPER09_P3  	(IO_PPER09.bit.P3)
#define	PPER09_P2  	(IO_PPER09.bit.P2)
#define	PPER09_P1  	(IO_PPER09.bit.P1)
#define	PPER09_P0  	(IO_PPER09.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPER10,attr=IO,locate=0xECA
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPER10STR;

__IO_EXTENDED	  PPER10STR	IO_PPER10;
#define	_pper10		(IO_PPER10)
#define	PPER10		(IO_PPER10.byte)
#define	PPER10_P7  	(IO_PPER10.bit.P7)
#define	PPER10_P6  	(IO_PPER10.bit.P6)
#define	PPER10_P5  	(IO_PPER10.bit.P5)
#define	PPER10_P4  	(IO_PPER10.bit.P4)
#define	PPER10_P3  	(IO_PPER10.bit.P3)
#define	PPER10_P2  	(IO_PPER10.bit.P2)
#define	PPER10_P1  	(IO_PPER10.bit.P1)
#define	PPER10_P0  	(IO_PPER10.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPER11,attr=IO,locate=0xECB
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPER11STR;

__IO_EXTENDED	  PPER11STR	IO_PPER11;
#define	_pper11		(IO_PPER11)
#define	PPER11		(IO_PPER11.byte)
#define	PPER11_P7  	(IO_PPER11.bit.P7)
#define	PPER11_P6  	(IO_PPER11.bit.P6)
#define	PPER11_P5  	(IO_PPER11.bit.P5)
#define	PPER11_P4  	(IO_PPER11.bit.P4)
#define	PPER11_P3  	(IO_PPER11.bit.P3)
#define	PPER11_P2  	(IO_PPER11.bit.P2)
#define	PPER11_P1  	(IO_PPER11.bit.P1)
#define	PPER11_P0  	(IO_PPER11.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPER12,attr=IO,locate=0xECC
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPER12STR;

__IO_EXTENDED	  PPER12STR	IO_PPER12;
#define	_pper12		(IO_PPER12)
#define	PPER12		(IO_PPER12.byte)
#define	PPER12_P7  	(IO_PPER12.bit.P7)
#define	PPER12_P6  	(IO_PPER12.bit.P6)
#define	PPER12_P5  	(IO_PPER12.bit.P5)
#define	PPER12_P4  	(IO_PPER12.bit.P4)
#define	PPER12_P3  	(IO_PPER12.bit.P3)
#define	PPER12_P2  	(IO_PPER12.bit.P2)
#define	PPER12_P1  	(IO_PPER12.bit.P1)
#define	PPER12_P0  	(IO_PPER12.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PPER13,attr=IO,locate=0xECD
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	RESV2:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PPER13STR;

__IO_EXTENDED	  PPER13STR	IO_PPER13;
#define	_pper13		(IO_PPER13)
#define	PPER13		(IO_PPER13.byte)
#define	PPER13_P7  	(IO_PPER13.bit.P7)
#define	PPER13_P6  	(IO_PPER13.bit.P6)
#define	PPER13_P4  	(IO_PPER13.bit.P4)
#define	PPER13_P3  	(IO_PPER13.bit.P3)
#define	PPER13_P2  	(IO_PPER13.bit.P2)
#define	PPER13_P1  	(IO_PPER13.bit.P1)
#define	PPER13_P0  	(IO_PPER13.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PILR00,attr=IO,locate=0xEE0
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PILR00STR;

__IO_EXTENDED	  PILR00STR	IO_PILR00;
#define	_pilr00		(IO_PILR00)
#define	PILR00		(IO_PILR00.byte)
#define	PILR00_P7  	(IO_PILR00.bit.P7)
#define	PILR00_P6  	(IO_PILR00.bit.P6)
#define	PILR00_P5  	(IO_PILR00.bit.P5)
#define	PILR00_P4  	(IO_PILR00.bit.P4)
#define	PILR00_P3  	(IO_PILR00.bit.P3)
#define	PILR00_P2  	(IO_PILR00.bit.P2)
#define	PILR00_P1  	(IO_PILR00.bit.P1)
#define	PILR00_P0  	(IO_PILR00.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PILR01,attr=IO,locate=0xEE1
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PILR01STR;

__IO_EXTENDED	  PILR01STR	IO_PILR01;
#define	_pilr01		(IO_PILR01)
#define	PILR01		(IO_PILR01.byte)
#define	PILR01_P7  	(IO_PILR01.bit.P7)
#define	PILR01_P6  	(IO_PILR01.bit.P6)
#define	PILR01_P5  	(IO_PILR01.bit.P5)
#define	PILR01_P4  	(IO_PILR01.bit.P4)
#define	PILR01_P3  	(IO_PILR01.bit.P3)
#define	PILR01_P2  	(IO_PILR01.bit.P2)
#define	PILR01_P1  	(IO_PILR01.bit.P1)
#define	PILR01_P0  	(IO_PILR01.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PILR02,attr=IO,locate=0xEE2
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PILR02STR;

__IO_EXTENDED	  PILR02STR	IO_PILR02;
#define	_pilr02		(IO_PILR02)
#define	PILR02		(IO_PILR02.byte)
#define	PILR02_P7  	(IO_PILR02.bit.P7)
#define	PILR02_P6  	(IO_PILR02.bit.P6)
#define	PILR02_P5  	(IO_PILR02.bit.P5)
#define	PILR02_P4  	(IO_PILR02.bit.P4)
#define	PILR02_P3  	(IO_PILR02.bit.P3)
#define	PILR02_P2  	(IO_PILR02.bit.P2)
#define	PILR02_P1  	(IO_PILR02.bit.P1)
#define	PILR02_P0  	(IO_PILR02.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PILR03,attr=IO,locate=0xEE3
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PILR03STR;

__IO_EXTENDED	  PILR03STR	IO_PILR03;
#define	_pilr03		(IO_PILR03)
#define	PILR03		(IO_PILR03.byte)
#define	PILR03_P7  	(IO_PILR03.bit.P7)
#define	PILR03_P6  	(IO_PILR03.bit.P6)
#define	PILR03_P5  	(IO_PILR03.bit.P5)
#define	PILR03_P4  	(IO_PILR03.bit.P4)
#define	PILR03_P3  	(IO_PILR03.bit.P3)
#define	PILR03_P2  	(IO_PILR03.bit.P2)
#define	PILR03_P1  	(IO_PILR03.bit.P1)
#define	PILR03_P0  	(IO_PILR03.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PILR04,attr=IO,locate=0xEE4
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PILR04STR;

__IO_EXTENDED	  PILR04STR	IO_PILR04;
#define	_pilr04		(IO_PILR04)
#define	PILR04		(IO_PILR04.byte)
#define	PILR04_P7  	(IO_PILR04.bit.P7)
#define	PILR04_P6  	(IO_PILR04.bit.P6)
#define	PILR04_P5  	(IO_PILR04.bit.P5)
#define	PILR04_P4  	(IO_PILR04.bit.P4)
#define	PILR04_P3  	(IO_PILR04.bit.P3)
#define	PILR04_P2  	(IO_PILR04.bit.P2)
#define	PILR04_P1  	(IO_PILR04.bit.P1)
#define	PILR04_P0  	(IO_PILR04.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PILR05,attr=IO,locate=0xEE5
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PILR05STR;

__IO_EXTENDED	  PILR05STR	IO_PILR05;
#define	_pilr05		(IO_PILR05)
#define	PILR05		(IO_PILR05.byte)
#define	PILR05_P7  	(IO_PILR05.bit.P7)
#define	PILR05_P6  	(IO_PILR05.bit.P6)
#define	PILR05_P5  	(IO_PILR05.bit.P5)
#define	PILR05_P4  	(IO_PILR05.bit.P4)
#define	PILR05_P3  	(IO_PILR05.bit.P3)
#define	PILR05_P2  	(IO_PILR05.bit.P2)
#define	PILR05_P1  	(IO_PILR05.bit.P1)
#define	PILR05_P0  	(IO_PILR05.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PILR06,attr=IO,locate=0xEE6
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PILR06STR;

__IO_EXTENDED	  PILR06STR	IO_PILR06;
#define	_pilr06		(IO_PILR06)
#define	PILR06		(IO_PILR06.byte)
#define	PILR06_P7  	(IO_PILR06.bit.P7)
#define	PILR06_P6  	(IO_PILR06.bit.P6)
#define	PILR06_P5  	(IO_PILR06.bit.P5)
#define	PILR06_P4  	(IO_PILR06.bit.P4)
#define	PILR06_P3  	(IO_PILR06.bit.P3)
#define	PILR06_P2  	(IO_PILR06.bit.P2)
#define	PILR06_P1  	(IO_PILR06.bit.P1)
#define	PILR06_P0  	(IO_PILR06.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PILR07,attr=IO,locate=0xEE7
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PILR07STR;

__IO_EXTENDED	  PILR07STR	IO_PILR07;
#define	_pilr07		(IO_PILR07)
#define	PILR07		(IO_PILR07.byte)
#define	PILR07_P7  	(IO_PILR07.bit.P7)
#define	PILR07_P6  	(IO_PILR07.bit.P6)
#define	PILR07_P5  	(IO_PILR07.bit.P5)
#define	PILR07_P4  	(IO_PILR07.bit.P4)
#define	PILR07_P3  	(IO_PILR07.bit.P3)
#define	PILR07_P2  	(IO_PILR07.bit.P2)
#define	PILR07_P1  	(IO_PILR07.bit.P1)
#define	PILR07_P0  	(IO_PILR07.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PILR08,attr=IO,locate=0xEE8
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PILR08STR;

__IO_EXTENDED	  PILR08STR	IO_PILR08;
#define	_pilr08		(IO_PILR08)
#define	PILR08		(IO_PILR08.byte)
#define	PILR08_P7  	(IO_PILR08.bit.P7)
#define	PILR08_P6  	(IO_PILR08.bit.P6)
#define	PILR08_P5  	(IO_PILR08.bit.P5)
#define	PILR08_P4  	(IO_PILR08.bit.P4)
#define	PILR08_P3  	(IO_PILR08.bit.P3)
#define	PILR08_P2  	(IO_PILR08.bit.P2)
#define	PILR08_P1  	(IO_PILR08.bit.P1)
#define	PILR08_P0  	(IO_PILR08.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PILR09,attr=IO,locate=0xEE9
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PILR09STR;

__IO_EXTENDED	  PILR09STR	IO_PILR09;
#define	_pilr09		(IO_PILR09)
#define	PILR09		(IO_PILR09.byte)
#define	PILR09_P7  	(IO_PILR09.bit.P7)
#define	PILR09_P6  	(IO_PILR09.bit.P6)
#define	PILR09_P5  	(IO_PILR09.bit.P5)
#define	PILR09_P4  	(IO_PILR09.bit.P4)
#define	PILR09_P3  	(IO_PILR09.bit.P3)
#define	PILR09_P2  	(IO_PILR09.bit.P2)
#define	PILR09_P1  	(IO_PILR09.bit.P1)
#define	PILR09_P0  	(IO_PILR09.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PILR10,attr=IO,locate=0xEEA
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PILR10STR;

__IO_EXTENDED	  PILR10STR	IO_PILR10;
#define	_pilr10		(IO_PILR10)
#define	PILR10		(IO_PILR10.byte)
#define	PILR10_P7  	(IO_PILR10.bit.P7)
#define	PILR10_P6  	(IO_PILR10.bit.P6)
#define	PILR10_P5  	(IO_PILR10.bit.P5)
#define	PILR10_P4  	(IO_PILR10.bit.P4)
#define	PILR10_P3  	(IO_PILR10.bit.P3)
#define	PILR10_P2  	(IO_PILR10.bit.P2)
#define	PILR10_P1  	(IO_PILR10.bit.P1)
#define	PILR10_P0  	(IO_PILR10.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PILR11,attr=IO,locate=0xEEB
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PILR11STR;

__IO_EXTENDED	  PILR11STR	IO_PILR11;
#define	_pilr11		(IO_PILR11)
#define	PILR11		(IO_PILR11.byte)
#define	PILR11_P7  	(IO_PILR11.bit.P7)
#define	PILR11_P6  	(IO_PILR11.bit.P6)
#define	PILR11_P5  	(IO_PILR11.bit.P5)
#define	PILR11_P4  	(IO_PILR11.bit.P4)
#define	PILR11_P3  	(IO_PILR11.bit.P3)
#define	PILR11_P2  	(IO_PILR11.bit.P2)
#define	PILR11_P1  	(IO_PILR11.bit.P1)
#define	PILR11_P0  	(IO_PILR11.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PILR12,attr=IO,locate=0xEEC
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PILR12STR;

__IO_EXTENDED	  PILR12STR	IO_PILR12;
#define	_pilr12		(IO_PILR12)
#define	PILR12		(IO_PILR12.byte)
#define	PILR12_P7  	(IO_PILR12.bit.P7)
#define	PILR12_P6  	(IO_PILR12.bit.P6)
#define	PILR12_P5  	(IO_PILR12.bit.P5)
#define	PILR12_P4  	(IO_PILR12.bit.P4)
#define	PILR12_P3  	(IO_PILR12.bit.P3)
#define	PILR12_P2  	(IO_PILR12.bit.P2)
#define	PILR12_P1  	(IO_PILR12.bit.P1)
#define	PILR12_P0  	(IO_PILR12.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PILR13,attr=IO,locate=0xEED
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	RESV2:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PILR13STR;

__IO_EXTENDED	  PILR13STR	IO_PILR13;
#define	_pilr13		(IO_PILR13)
#define	PILR13		(IO_PILR13.byte)
#define	PILR13_P7  	(IO_PILR13.bit.P7)
#define	PILR13_P6  	(IO_PILR13.bit.P6)
#define	PILR13_P4  	(IO_PILR13.bit.P4)
#define	PILR13_P3  	(IO_PILR13.bit.P3)
#define	PILR13_P2  	(IO_PILR13.bit.P2)
#define	PILR13_P1  	(IO_PILR13.bit.P1)
#define	PILR13_P0  	(IO_PILR13.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPILR00,attr=IO,locate=0xF00
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPILR00STR;

__IO_EXTENDED	  EPILR00STR	IO_EPILR00;
#define	_epilr00		(IO_EPILR00)
#define	EPILR00		(IO_EPILR00.byte)
#define	EPILR00_P7  	(IO_EPILR00.bit.P7)
#define	EPILR00_P6  	(IO_EPILR00.bit.P6)
#define	EPILR00_P5  	(IO_EPILR00.bit.P5)
#define	EPILR00_P4  	(IO_EPILR00.bit.P4)
#define	EPILR00_P3  	(IO_EPILR00.bit.P3)
#define	EPILR00_P2  	(IO_EPILR00.bit.P2)
#define	EPILR00_P1  	(IO_EPILR00.bit.P1)
#define	EPILR00_P0  	(IO_EPILR00.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPILR01,attr=IO,locate=0xF01
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPILR01STR;

__IO_EXTENDED	  EPILR01STR	IO_EPILR01;
#define	_epilr01		(IO_EPILR01)
#define	EPILR01		(IO_EPILR01.byte)
#define	EPILR01_P7  	(IO_EPILR01.bit.P7)
#define	EPILR01_P6  	(IO_EPILR01.bit.P6)
#define	EPILR01_P5  	(IO_EPILR01.bit.P5)
#define	EPILR01_P4  	(IO_EPILR01.bit.P4)
#define	EPILR01_P3  	(IO_EPILR01.bit.P3)
#define	EPILR01_P2  	(IO_EPILR01.bit.P2)
#define	EPILR01_P1  	(IO_EPILR01.bit.P1)
#define	EPILR01_P0  	(IO_EPILR01.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPILR02,attr=IO,locate=0xF02
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPILR02STR;

__IO_EXTENDED	  EPILR02STR	IO_EPILR02;
#define	_epilr02		(IO_EPILR02)
#define	EPILR02		(IO_EPILR02.byte)
#define	EPILR02_P7  	(IO_EPILR02.bit.P7)
#define	EPILR02_P6  	(IO_EPILR02.bit.P6)
#define	EPILR02_P5  	(IO_EPILR02.bit.P5)
#define	EPILR02_P4  	(IO_EPILR02.bit.P4)
#define	EPILR02_P3  	(IO_EPILR02.bit.P3)
#define	EPILR02_P2  	(IO_EPILR02.bit.P2)
#define	EPILR02_P1  	(IO_EPILR02.bit.P1)
#define	EPILR02_P0  	(IO_EPILR02.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPILR03,attr=IO,locate=0xF03
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPILR03STR;

__IO_EXTENDED	  EPILR03STR	IO_EPILR03;
#define	_epilr03		(IO_EPILR03)
#define	EPILR03		(IO_EPILR03.byte)
#define	EPILR03_P7  	(IO_EPILR03.bit.P7)
#define	EPILR03_P6  	(IO_EPILR03.bit.P6)
#define	EPILR03_P5  	(IO_EPILR03.bit.P5)
#define	EPILR03_P4  	(IO_EPILR03.bit.P4)
#define	EPILR03_P3  	(IO_EPILR03.bit.P3)
#define	EPILR03_P2  	(IO_EPILR03.bit.P2)
#define	EPILR03_P1  	(IO_EPILR03.bit.P1)
#define	EPILR03_P0  	(IO_EPILR03.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPILR04,attr=IO,locate=0xF04
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPILR04STR;

__IO_EXTENDED	  EPILR04STR	IO_EPILR04;
#define	_epilr04		(IO_EPILR04)
#define	EPILR04		(IO_EPILR04.byte)
#define	EPILR04_P7  	(IO_EPILR04.bit.P7)
#define	EPILR04_P6  	(IO_EPILR04.bit.P6)
#define	EPILR04_P5  	(IO_EPILR04.bit.P5)
#define	EPILR04_P4  	(IO_EPILR04.bit.P4)
#define	EPILR04_P3  	(IO_EPILR04.bit.P3)
#define	EPILR04_P2  	(IO_EPILR04.bit.P2)
#define	EPILR04_P1  	(IO_EPILR04.bit.P1)
#define	EPILR04_P0  	(IO_EPILR04.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPILR05,attr=IO,locate=0xF05
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPILR05STR;

__IO_EXTENDED	  EPILR05STR	IO_EPILR05;
#define	_epilr05		(IO_EPILR05)
#define	EPILR05		(IO_EPILR05.byte)
#define	EPILR05_P7  	(IO_EPILR05.bit.P7)
#define	EPILR05_P6  	(IO_EPILR05.bit.P6)
#define	EPILR05_P5  	(IO_EPILR05.bit.P5)
#define	EPILR05_P4  	(IO_EPILR05.bit.P4)
#define	EPILR05_P3  	(IO_EPILR05.bit.P3)
#define	EPILR05_P2  	(IO_EPILR05.bit.P2)
#define	EPILR05_P1  	(IO_EPILR05.bit.P1)
#define	EPILR05_P0  	(IO_EPILR05.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPILR06,attr=IO,locate=0xF06
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPILR06STR;

__IO_EXTENDED	  EPILR06STR	IO_EPILR06;
#define	_epilr06		(IO_EPILR06)
#define	EPILR06		(IO_EPILR06.byte)
#define	EPILR06_P7  	(IO_EPILR06.bit.P7)
#define	EPILR06_P6  	(IO_EPILR06.bit.P6)
#define	EPILR06_P5  	(IO_EPILR06.bit.P5)
#define	EPILR06_P4  	(IO_EPILR06.bit.P4)
#define	EPILR06_P3  	(IO_EPILR06.bit.P3)
#define	EPILR06_P2  	(IO_EPILR06.bit.P2)
#define	EPILR06_P1  	(IO_EPILR06.bit.P1)
#define	EPILR06_P0  	(IO_EPILR06.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPILR07,attr=IO,locate=0xF07
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPILR07STR;

__IO_EXTENDED	  EPILR07STR	IO_EPILR07;
#define	_epilr07		(IO_EPILR07)
#define	EPILR07		(IO_EPILR07.byte)
#define	EPILR07_P7  	(IO_EPILR07.bit.P7)
#define	EPILR07_P6  	(IO_EPILR07.bit.P6)
#define	EPILR07_P5  	(IO_EPILR07.bit.P5)
#define	EPILR07_P4  	(IO_EPILR07.bit.P4)
#define	EPILR07_P3  	(IO_EPILR07.bit.P3)
#define	EPILR07_P2  	(IO_EPILR07.bit.P2)
#define	EPILR07_P1  	(IO_EPILR07.bit.P1)
#define	EPILR07_P0  	(IO_EPILR07.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPILR08,attr=IO,locate=0xF08
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPILR08STR;

__IO_EXTENDED	  EPILR08STR	IO_EPILR08;
#define	_epilr08		(IO_EPILR08)
#define	EPILR08		(IO_EPILR08.byte)
#define	EPILR08_P7  	(IO_EPILR08.bit.P7)
#define	EPILR08_P6  	(IO_EPILR08.bit.P6)
#define	EPILR08_P5  	(IO_EPILR08.bit.P5)
#define	EPILR08_P4  	(IO_EPILR08.bit.P4)
#define	EPILR08_P3  	(IO_EPILR08.bit.P3)
#define	EPILR08_P2  	(IO_EPILR08.bit.P2)
#define	EPILR08_P1  	(IO_EPILR08.bit.P1)
#define	EPILR08_P0  	(IO_EPILR08.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPILR09,attr=IO,locate=0xF09
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPILR09STR;

__IO_EXTENDED	  EPILR09STR	IO_EPILR09;
#define	_epilr09		(IO_EPILR09)
#define	EPILR09		(IO_EPILR09.byte)
#define	EPILR09_P7  	(IO_EPILR09.bit.P7)
#define	EPILR09_P6  	(IO_EPILR09.bit.P6)
#define	EPILR09_P5  	(IO_EPILR09.bit.P5)
#define	EPILR09_P4  	(IO_EPILR09.bit.P4)
#define	EPILR09_P3  	(IO_EPILR09.bit.P3)
#define	EPILR09_P2  	(IO_EPILR09.bit.P2)
#define	EPILR09_P1  	(IO_EPILR09.bit.P1)
#define	EPILR09_P0  	(IO_EPILR09.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPILR10,attr=IO,locate=0xF0A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPILR10STR;

__IO_EXTENDED	  EPILR10STR	IO_EPILR10;
#define	_epilr10		(IO_EPILR10)
#define	EPILR10		(IO_EPILR10.byte)
#define	EPILR10_P7  	(IO_EPILR10.bit.P7)
#define	EPILR10_P6  	(IO_EPILR10.bit.P6)
#define	EPILR10_P5  	(IO_EPILR10.bit.P5)
#define	EPILR10_P4  	(IO_EPILR10.bit.P4)
#define	EPILR10_P3  	(IO_EPILR10.bit.P3)
#define	EPILR10_P2  	(IO_EPILR10.bit.P2)
#define	EPILR10_P1  	(IO_EPILR10.bit.P1)
#define	EPILR10_P0  	(IO_EPILR10.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPILR11,attr=IO,locate=0xF0B
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPILR11STR;

__IO_EXTENDED	  EPILR11STR	IO_EPILR11;
#define	_epilr11		(IO_EPILR11)
#define	EPILR11		(IO_EPILR11.byte)
#define	EPILR11_P7  	(IO_EPILR11.bit.P7)
#define	EPILR11_P6  	(IO_EPILR11.bit.P6)
#define	EPILR11_P5  	(IO_EPILR11.bit.P5)
#define	EPILR11_P4  	(IO_EPILR11.bit.P4)
#define	EPILR11_P3  	(IO_EPILR11.bit.P3)
#define	EPILR11_P2  	(IO_EPILR11.bit.P2)
#define	EPILR11_P1  	(IO_EPILR11.bit.P1)
#define	EPILR11_P0  	(IO_EPILR11.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPILR12,attr=IO,locate=0xF0C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPILR12STR;

__IO_EXTENDED	  EPILR12STR	IO_EPILR12;
#define	_epilr12		(IO_EPILR12)
#define	EPILR12		(IO_EPILR12.byte)
#define	EPILR12_P7  	(IO_EPILR12.bit.P7)
#define	EPILR12_P6  	(IO_EPILR12.bit.P6)
#define	EPILR12_P5  	(IO_EPILR12.bit.P5)
#define	EPILR12_P4  	(IO_EPILR12.bit.P4)
#define	EPILR12_P3  	(IO_EPILR12.bit.P3)
#define	EPILR12_P2  	(IO_EPILR12.bit.P2)
#define	EPILR12_P1  	(IO_EPILR12.bit.P1)
#define	EPILR12_P0  	(IO_EPILR12.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPILR13,attr=IO,locate=0xF0D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	RESV2:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPILR13STR;

__IO_EXTENDED	  EPILR13STR	IO_EPILR13;
#define	_epilr13		(IO_EPILR13)
#define	EPILR13		(IO_EPILR13.byte)
#define	EPILR13_P7  	(IO_EPILR13.bit.P7)
#define	EPILR13_P6  	(IO_EPILR13.bit.P6)
#define	EPILR13_P4  	(IO_EPILR13.bit.P4)
#define	EPILR13_P3  	(IO_EPILR13.bit.P3)
#define	EPILR13_P2  	(IO_EPILR13.bit.P2)
#define	EPILR13_P1  	(IO_EPILR13.bit.P1)
#define	EPILR13_P0  	(IO_EPILR13.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PODR00,attr=IO,locate=0xF20
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PODR00STR;

__IO_EXTENDED	  PODR00STR	IO_PODR00;
#define	_podr00		(IO_PODR00)
#define	PODR00		(IO_PODR00.byte)
#define	PODR00_P7  	(IO_PODR00.bit.P7)
#define	PODR00_P6  	(IO_PODR00.bit.P6)
#define	PODR00_P5  	(IO_PODR00.bit.P5)
#define	PODR00_P4  	(IO_PODR00.bit.P4)
#define	PODR00_P3  	(IO_PODR00.bit.P3)
#define	PODR00_P2  	(IO_PODR00.bit.P2)
#define	PODR00_P1  	(IO_PODR00.bit.P1)
#define	PODR00_P0  	(IO_PODR00.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PODR01,attr=IO,locate=0xF21
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PODR01STR;

__IO_EXTENDED	  PODR01STR	IO_PODR01;
#define	_podr01		(IO_PODR01)
#define	PODR01		(IO_PODR01.byte)
#define	PODR01_P7  	(IO_PODR01.bit.P7)
#define	PODR01_P6  	(IO_PODR01.bit.P6)
#define	PODR01_P5  	(IO_PODR01.bit.P5)
#define	PODR01_P4  	(IO_PODR01.bit.P4)
#define	PODR01_P3  	(IO_PODR01.bit.P3)
#define	PODR01_P2  	(IO_PODR01.bit.P2)
#define	PODR01_P1  	(IO_PODR01.bit.P1)
#define	PODR01_P0  	(IO_PODR01.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PODR02,attr=IO,locate=0xF22
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PODR02STR;

__IO_EXTENDED	  PODR02STR	IO_PODR02;
#define	_podr02		(IO_PODR02)
#define	PODR02		(IO_PODR02.byte)
#define	PODR02_P7  	(IO_PODR02.bit.P7)
#define	PODR02_P6  	(IO_PODR02.bit.P6)
#define	PODR02_P5  	(IO_PODR02.bit.P5)
#define	PODR02_P4  	(IO_PODR02.bit.P4)
#define	PODR02_P3  	(IO_PODR02.bit.P3)
#define	PODR02_P2  	(IO_PODR02.bit.P2)
#define	PODR02_P1  	(IO_PODR02.bit.P1)
#define	PODR02_P0  	(IO_PODR02.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PODR03,attr=IO,locate=0xF23
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PODR03STR;

__IO_EXTENDED	  PODR03STR	IO_PODR03;
#define	_podr03		(IO_PODR03)
#define	PODR03		(IO_PODR03.byte)
#define	PODR03_P7  	(IO_PODR03.bit.P7)
#define	PODR03_P6  	(IO_PODR03.bit.P6)
#define	PODR03_P5  	(IO_PODR03.bit.P5)
#define	PODR03_P4  	(IO_PODR03.bit.P4)
#define	PODR03_P3  	(IO_PODR03.bit.P3)
#define	PODR03_P2  	(IO_PODR03.bit.P2)
#define	PODR03_P1  	(IO_PODR03.bit.P1)
#define	PODR03_P0  	(IO_PODR03.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PODR04,attr=IO,locate=0xF24
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PODR04STR;

__IO_EXTENDED	  PODR04STR	IO_PODR04;
#define	_podr04		(IO_PODR04)
#define	PODR04		(IO_PODR04.byte)
#define	PODR04_P7  	(IO_PODR04.bit.P7)
#define	PODR04_P6  	(IO_PODR04.bit.P6)
#define	PODR04_P5  	(IO_PODR04.bit.P5)
#define	PODR04_P4  	(IO_PODR04.bit.P4)
#define	PODR04_P3  	(IO_PODR04.bit.P3)
#define	PODR04_P2  	(IO_PODR04.bit.P2)
#define	PODR04_P1  	(IO_PODR04.bit.P1)
#define	PODR04_P0  	(IO_PODR04.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PODR05,attr=IO,locate=0xF25
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PODR05STR;

__IO_EXTENDED	  PODR05STR	IO_PODR05;
#define	_podr05		(IO_PODR05)
#define	PODR05		(IO_PODR05.byte)
#define	PODR05_P7  	(IO_PODR05.bit.P7)
#define	PODR05_P6  	(IO_PODR05.bit.P6)
#define	PODR05_P5  	(IO_PODR05.bit.P5)
#define	PODR05_P4  	(IO_PODR05.bit.P4)
#define	PODR05_P3  	(IO_PODR05.bit.P3)
#define	PODR05_P2  	(IO_PODR05.bit.P2)
#define	PODR05_P1  	(IO_PODR05.bit.P1)
#define	PODR05_P0  	(IO_PODR05.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PODR06,attr=IO,locate=0xF26
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PODR06STR;

__IO_EXTENDED	  PODR06STR	IO_PODR06;
#define	_podr06		(IO_PODR06)
#define	PODR06		(IO_PODR06.byte)
#define	PODR06_P7  	(IO_PODR06.bit.P7)
#define	PODR06_P6  	(IO_PODR06.bit.P6)
#define	PODR06_P5  	(IO_PODR06.bit.P5)
#define	PODR06_P4  	(IO_PODR06.bit.P4)
#define	PODR06_P3  	(IO_PODR06.bit.P3)
#define	PODR06_P2  	(IO_PODR06.bit.P2)
#define	PODR06_P1  	(IO_PODR06.bit.P1)
#define	PODR06_P0  	(IO_PODR06.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PODR07,attr=IO,locate=0xF27
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PODR07STR;

__IO_EXTENDED	  PODR07STR	IO_PODR07;
#define	_podr07		(IO_PODR07)
#define	PODR07		(IO_PODR07.byte)
#define	PODR07_P7  	(IO_PODR07.bit.P7)
#define	PODR07_P6  	(IO_PODR07.bit.P6)
#define	PODR07_P5  	(IO_PODR07.bit.P5)
#define	PODR07_P4  	(IO_PODR07.bit.P4)
#define	PODR07_P3  	(IO_PODR07.bit.P3)
#define	PODR07_P2  	(IO_PODR07.bit.P2)
#define	PODR07_P1  	(IO_PODR07.bit.P1)
#define	PODR07_P0  	(IO_PODR07.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PODR08,attr=IO,locate=0xF28
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PODR08STR;

__IO_EXTENDED	  PODR08STR	IO_PODR08;
#define	_podr08		(IO_PODR08)
#define	PODR08		(IO_PODR08.byte)
#define	PODR08_P7  	(IO_PODR08.bit.P7)
#define	PODR08_P6  	(IO_PODR08.bit.P6)
#define	PODR08_P5  	(IO_PODR08.bit.P5)
#define	PODR08_P4  	(IO_PODR08.bit.P4)
#define	PODR08_P3  	(IO_PODR08.bit.P3)
#define	PODR08_P2  	(IO_PODR08.bit.P2)
#define	PODR08_P1  	(IO_PODR08.bit.P1)
#define	PODR08_P0  	(IO_PODR08.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PODR09,attr=IO,locate=0xF29
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PODR09STR;

__IO_EXTENDED	  PODR09STR	IO_PODR09;
#define	_podr09		(IO_PODR09)
#define	PODR09		(IO_PODR09.byte)
#define	PODR09_P7  	(IO_PODR09.bit.P7)
#define	PODR09_P6  	(IO_PODR09.bit.P6)
#define	PODR09_P5  	(IO_PODR09.bit.P5)
#define	PODR09_P4  	(IO_PODR09.bit.P4)
#define	PODR09_P3  	(IO_PODR09.bit.P3)
#define	PODR09_P2  	(IO_PODR09.bit.P2)
#define	PODR09_P1  	(IO_PODR09.bit.P1)
#define	PODR09_P0  	(IO_PODR09.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PODR10,attr=IO,locate=0xF2A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PODR10STR;

__IO_EXTENDED	  PODR10STR	IO_PODR10;
#define	_podr10		(IO_PODR10)
#define	PODR10		(IO_PODR10.byte)
#define	PODR10_P7  	(IO_PODR10.bit.P7)
#define	PODR10_P6  	(IO_PODR10.bit.P6)
#define	PODR10_P5  	(IO_PODR10.bit.P5)
#define	PODR10_P4  	(IO_PODR10.bit.P4)
#define	PODR10_P3  	(IO_PODR10.bit.P3)
#define	PODR10_P2  	(IO_PODR10.bit.P2)
#define	PODR10_P1  	(IO_PODR10.bit.P1)
#define	PODR10_P0  	(IO_PODR10.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PODR11,attr=IO,locate=0xF2B
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PODR11STR;

__IO_EXTENDED	  PODR11STR	IO_PODR11;
#define	_podr11		(IO_PODR11)
#define	PODR11		(IO_PODR11.byte)
#define	PODR11_P7  	(IO_PODR11.bit.P7)
#define	PODR11_P6  	(IO_PODR11.bit.P6)
#define	PODR11_P5  	(IO_PODR11.bit.P5)
#define	PODR11_P4  	(IO_PODR11.bit.P4)
#define	PODR11_P3  	(IO_PODR11.bit.P3)
#define	PODR11_P2  	(IO_PODR11.bit.P2)
#define	PODR11_P1  	(IO_PODR11.bit.P1)
#define	PODR11_P0  	(IO_PODR11.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PODR12,attr=IO,locate=0xF2C
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PODR12STR;

__IO_EXTENDED	  PODR12STR	IO_PODR12;
#define	_podr12		(IO_PODR12)
#define	PODR12		(IO_PODR12.byte)
#define	PODR12_P7  	(IO_PODR12.bit.P7)
#define	PODR12_P6  	(IO_PODR12.bit.P6)
#define	PODR12_P5  	(IO_PODR12.bit.P5)
#define	PODR12_P4  	(IO_PODR12.bit.P4)
#define	PODR12_P3  	(IO_PODR12.bit.P3)
#define	PODR12_P2  	(IO_PODR12.bit.P2)
#define	PODR12_P1  	(IO_PODR12.bit.P1)
#define	PODR12_P0  	(IO_PODR12.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PODR13,attr=IO,locate=0xF2D
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	RESV2:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} PODR13STR;

__IO_EXTENDED	  PODR13STR	IO_PODR13;
#define	_podr13		(IO_PODR13)
#define	PODR13		(IO_PODR13.byte)
#define	PODR13_P7  	(IO_PODR13.bit.P7)
#define	PODR13_P6  	(IO_PODR13.bit.P6)
#define	PODR13_P4  	(IO_PODR13.bit.P4)
#define	PODR13_P3  	(IO_PODR13.bit.P3)
#define	PODR13_P2  	(IO_PODR13.bit.P2)
#define	PODR13_P1  	(IO_PODR13.bit.P1)
#define	PODR13_P0  	(IO_PODR13.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPODR01,attr=IO,locate=0xF35
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPODR01STR;

__IO_EXTENDED	  EPODR01STR	IO_EPODR01;
#define	_epodr01		(IO_EPODR01)
#define	EPODR01		(IO_EPODR01.byte)
#define	EPODR01_P7  	(IO_EPODR01.bit.P7)
#define	EPODR01_P6  	(IO_EPODR01.bit.P6)
#define	EPODR01_P5  	(IO_EPODR01.bit.P5)
#define	EPODR01_P4  	(IO_EPODR01.bit.P4)
#define	EPODR01_P3  	(IO_EPODR01.bit.P3)
#define	EPODR01_P2  	(IO_EPODR01.bit.P2)
#define	EPODR01_P1  	(IO_EPODR01.bit.P1)
#define	EPODR01_P0  	(IO_EPODR01.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPODR02,attr=IO,locate=0xF36
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPODR02STR;

__IO_EXTENDED	  EPODR02STR	IO_EPODR02;
#define	_epodr02		(IO_EPODR02)
#define	EPODR02		(IO_EPODR02.byte)
#define	EPODR02_P7  	(IO_EPODR02.bit.P7)
#define	EPODR02_P6  	(IO_EPODR02.bit.P6)
#define	EPODR02_P5  	(IO_EPODR02.bit.P5)
#define	EPODR02_P4  	(IO_EPODR02.bit.P4)
#define	EPODR02_P3  	(IO_EPODR02.bit.P3)
#define	EPODR02_P2  	(IO_EPODR02.bit.P2)
#define	EPODR02_P1  	(IO_EPODR02.bit.P1)
#define	EPODR02_P0  	(IO_EPODR02.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPODR03,attr=IO,locate=0xF37
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPODR03STR;

__IO_EXTENDED	  EPODR03STR	IO_EPODR03;
#define	_epodr03		(IO_EPODR03)
#define	EPODR03		(IO_EPODR03.byte)
#define	EPODR03_P6  	(IO_EPODR03.bit.P6)
#define	EPODR03_P5  	(IO_EPODR03.bit.P5)
#define	EPODR03_P4  	(IO_EPODR03.bit.P4)
#define	EPODR03_P3  	(IO_EPODR03.bit.P3)
#define	EPODR03_P2  	(IO_EPODR03.bit.P2)
#define	EPODR03_P1  	(IO_EPODR03.bit.P1)
#define	EPODR03_P0  	(IO_EPODR03.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPODR06,attr=IO,locate=0xF38
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPODR06STR;

__IO_EXTENDED	  EPODR06STR	IO_EPODR06;
#define	_epodr06		(IO_EPODR06)
#define	EPODR06		(IO_EPODR06.byte)
#define	EPODR06_P7  	(IO_EPODR06.bit.P7)
#define	EPODR06_P6  	(IO_EPODR06.bit.P6)
#define	EPODR06_P5  	(IO_EPODR06.bit.P5)
#define	EPODR06_P4  	(IO_EPODR06.bit.P4)
#define	EPODR06_P3  	(IO_EPODR06.bit.P3)
#define	EPODR06_P2  	(IO_EPODR06.bit.P2)
#define	EPODR06_P1  	(IO_EPODR06.bit.P1)
#define	EPODR06_P0  	(IO_EPODR06.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPODR07,attr=IO,locate=0xF39
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPODR07STR;

__IO_EXTENDED	  EPODR07STR	IO_EPODR07;
#define	_epodr07		(IO_EPODR07)
#define	EPODR07		(IO_EPODR07.byte)
#define	EPODR07_P7  	(IO_EPODR07.bit.P7)
#define	EPODR07_P6  	(IO_EPODR07.bit.P6)
#define	EPODR07_P5  	(IO_EPODR07.bit.P5)
#define	EPODR07_P4  	(IO_EPODR07.bit.P4)
#define	EPODR07_P3  	(IO_EPODR07.bit.P3)
#define	EPODR07_P2  	(IO_EPODR07.bit.P2)
#define	EPODR07_P1  	(IO_EPODR07.bit.P1)
#define	EPODR07_P0  	(IO_EPODR07.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EPODR08,attr=IO,locate=0xF3A
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	P7:1;
			__BYTE	P6:1;
			__BYTE	P5:1;
			__BYTE	P4:1;
			__BYTE	P3:1;
			__BYTE	P2:1;
			__BYTE	P1:1;
			__BYTE	P0:1;
	} bit;
} EPODR08STR;

__IO_EXTENDED	  EPODR08STR	IO_EPODR08;
#define	_epodr08		(IO_EPODR08)
#define	EPODR08		(IO_EPODR08.byte)
#define	EPODR08_P7  	(IO_EPODR08.bit.P7)
#define	EPODR08_P6  	(IO_EPODR08.bit.P6)
#define	EPODR08_P5  	(IO_EPODR08.bit.P5)
#define	EPODR08_P4  	(IO_EPODR08.bit.P4)
#define	EPODR08_P3  	(IO_EPODR08.bit.P3)
#define	EPODR08_P2  	(IO_EPODR08.bit.P2)
#define	EPODR08_P1  	(IO_EPODR08.bit.P1)
#define	EPODR08_P0  	(IO_EPODR08.bit.P0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PORTEN,attr=IO,locate=0xF40
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	GPORTEN:1;
	} bit;
} PORTENSTR;

__IO_EXTENDED	  PORTENSTR	IO_PORTEN;
#define	_porten		(IO_PORTEN)
#define	PORTEN		(IO_PORTEN.byte)
#define	PORTEN_GPORTEN  	(IO_PORTEN.bit.GPORTEN)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_RCR0,attr=IO,locate=0xF70
#endif

__IO_EXTENDED	union {
	__HWORD	hword;
	struct {
		__BYTE	RCRH0;
		__BYTE	RCRL0;
	} byte;
} IO_RCR0;

#define	RCR0		(IO_RCR0.hword)
#define	RCR0_RCRH0	(IO_RCR0.byte.RCRH0)
#define	RCR0_RCRL0	(IO_RCR0.byte.RCRL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_UDCR0,attr=IO,locate=0xF72
#endif

__IO_EXTENDED	union {
	__HWORD	hword;
	struct {
		__BYTE	UDCRH0;
		__BYTE	UDCRL0;
	} byte;
} IO_UDCR0;

#define	UDCR0		(IO_UDCR0.hword)
#define	UDCR0_UDCRH0	(IO_UDCR0.byte.UDCRH0)
#define	UDCR0_UDCRL0	(IO_UDCR0.byte.UDCRL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CCR0,attr=IO,locate=0xF74
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	M16E:1;
			__HWORD	CDCF:1;
			__HWORD	CFIE:1;
			__HWORD	CLKS:1;
			__HWORD	CMS:2;
			__HWORD	CES:2;
			__HWORD	RESV6:1;
			__HWORD	CTUT:1;
			__HWORD	UCRE:1;
			__HWORD	RLDE:1;
			__HWORD	UDCC:1;
			__HWORD	CGSC:1;
			__HWORD	CGE:2;
	} bit;
} CCR0STR;

__IO_EXTENDED	  CCR0STR	IO_CCR0;
#define	_ccr0		(IO_CCR0)
#define	CCR0		(IO_CCR0.hword)
#define	CCR0_M16E  	(IO_CCR0.bit.M16E)
#define	CCR0_CDCF  	(IO_CCR0.bit.CDCF)
#define	CCR0_CFIE  	(IO_CCR0.bit.CFIE)
#define	CCR0_CLKS  	(IO_CCR0.bit.CLKS)
#define	CCR0_CMS  	(IO_CCR0.bit.CMS)
#define	CCR0_CES  	(IO_CCR0.bit.CES)
#define	CCR0_CTUT  	(IO_CCR0.bit.CTUT)
#define	CCR0_UCRE  	(IO_CCR0.bit.UCRE)
#define	CCR0_RLDE  	(IO_CCR0.bit.RLDE)
#define	CCR0_UDCC  	(IO_CCR0.bit.UDCC)
#define	CCR0_CGSC  	(IO_CCR0.bit.CGSC)
#define	CCR0_CGE  	(IO_CCR0.bit.CGE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CSR0,attr=IO,locate=0xF77
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	CSTR:1;
			__BYTE	CITE:1;
			__BYTE	UDIE:1;
			__BYTE	CMPF:1;
			__BYTE	OVFF:1;
			__BYTE	UDFF:1;
			__BYTE	UDF:2;
	} bit;
} CSR0STR;

__IO_EXTENDED	  CSR0STR	IO_CSR0;
#define	_csr0		(IO_CSR0)
#define	CSR0		(IO_CSR0.byte)
#define	CSR0_CSTR  	(IO_CSR0.bit.CSTR)
#define	CSR0_CITE  	(IO_CSR0.bit.CITE)
#define	CSR0_UDIE  	(IO_CSR0.bit.UDIE)
#define	CSR0_CMPF  	(IO_CSR0.bit.CMPF)
#define	CSR0_OVFF  	(IO_CSR0.bit.OVFF)
#define	CSR0_UDFF  	(IO_CSR0.bit.UDFF)
#define	CSR0_UDF  	(IO_CSR0.bit.UDF)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_RCR1,attr=IO,locate=0xF80
#endif

__IO_EXTENDED	union {
	__HWORD	hword;
	struct {
		__BYTE	RCRH1;
		__BYTE	RCRL1;
	} byte;
} IO_RCR1;

#define	RCR1		(IO_RCR1.hword)
#define	RCR1_RCRH1	(IO_RCR1.byte.RCRH1)
#define	RCR1_RCRL1	(IO_RCR1.byte.RCRL1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_UDCR1,attr=IO,locate=0xF82
#endif

__IO_EXTENDED	union {
	__HWORD	hword;
	struct {
		__BYTE	UDCRH1;
		__BYTE	UDCRL1;
	} byte;
} IO_UDCR1;

#define	UDCR1		(IO_UDCR1.hword)
#define	UDCR1_UDCRH1	(IO_UDCR1.byte.UDCRH1)
#define	UDCR1_UDCRL1	(IO_UDCR1.byte.UDCRL1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CCR1,attr=IO,locate=0xF84
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	M16E:1;
			__HWORD	CDCF:1;
			__HWORD	CFIE:1;
			__HWORD	CLKS:1;
			__HWORD	CMS:2;
			__HWORD	CES:2;
			__HWORD	RESV6:1;
			__HWORD	CTUT:1;
			__HWORD	UCRE:1;
			__HWORD	RLDE:1;
			__HWORD	UDCC:1;
			__HWORD	CGSC:1;
			__HWORD	CGE:2;
	} bit;
} CCR1STR;

__IO_EXTENDED	  CCR1STR	IO_CCR1;
#define	_ccr1		(IO_CCR1)
#define	CCR1		(IO_CCR1.hword)
#define	CCR1_M16E  	(IO_CCR1.bit.M16E)
#define	CCR1_CDCF  	(IO_CCR1.bit.CDCF)
#define	CCR1_CFIE  	(IO_CCR1.bit.CFIE)
#define	CCR1_CLKS  	(IO_CCR1.bit.CLKS)
#define	CCR1_CMS  	(IO_CCR1.bit.CMS)
#define	CCR1_CES  	(IO_CCR1.bit.CES)
#define	CCR1_CTUT  	(IO_CCR1.bit.CTUT)
#define	CCR1_UCRE  	(IO_CCR1.bit.UCRE)
#define	CCR1_RLDE  	(IO_CCR1.bit.RLDE)
#define	CCR1_UDCC  	(IO_CCR1.bit.UDCC)
#define	CCR1_CGSC  	(IO_CCR1.bit.CGSC)
#define	CCR1_CGE  	(IO_CCR1.bit.CGE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CSR1,attr=IO,locate=0xF87
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	CSTR:1;
			__BYTE	CITE:1;
			__BYTE	UDIE:1;
			__BYTE	CMPF:1;
			__BYTE	OVFF:1;
			__BYTE	UDFF:1;
			__BYTE	UDF:2;
	} bit;
} CSR1STR;

__IO_EXTENDED	  CSR1STR	IO_CSR1;
#define	_csr1		(IO_CSR1)
#define	CSR1		(IO_CSR1.byte)
#define	CSR1_CSTR  	(IO_CSR1.bit.CSTR)
#define	CSR1_CITE  	(IO_CSR1.bit.CITE)
#define	CSR1_UDIE  	(IO_CSR1.bit.UDIE)
#define	CSR1_CMPF  	(IO_CSR1.bit.CMPF)
#define	CSR1_OVFF  	(IO_CSR1.bit.OVFF)
#define	CSR1_UDFF  	(IO_CSR1.bit.UDFF)
#define	CSR1_UDF  	(IO_CSR1.bit.UDF)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCCP4,attr=IO,locate=0xF90
#endif

__IO_EXTENDED	__WORD	IO_OCCP4;
#define	_occp4		(IO_OCCP4)
#define	OCCP4	(_occp4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCCP5,attr=IO,locate=0xF94
#endif

__IO_EXTENDED	__WORD	IO_OCCP5;
#define	_occp5		(IO_OCCP5)
#define	OCCP5	(_occp5)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCFS45,attr=IO,locate=0xF98
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SEL5:1;
			__BYTE	SEL4:1;
	} bit;
} OCFS45STR;

__IO_EXTENDED	  OCFS45STR	IO_OCFS45;
#define	_ocfs45		(IO_OCFS45)
#define	OCFS45		(IO_OCFS45.byte)
#define	OCFS45_SEL5  	(IO_OCFS45.bit.SEL5)
#define	OCFS45_SEL4  	(IO_OCFS45.bit.SEL4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_OCS45,attr=IO,locate=0xF9A
#endif

typedef union {
	__HWORD	hword;
	struct {
		__BYTE	OCSH45;
		__BYTE	OCSL45;
	} byte;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	CMOD:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	OTD5:1;
			__HWORD	OTD4:1;
			__HWORD	IOP5:1;
			__HWORD	IOP4:1;
			__HWORD	IOE5:1;
			__HWORD	IOE4:1;
			__HWORD	RESV12:1;
			__HWORD	RESV13:1;
			__HWORD	CST5:1;
			__HWORD	CST4:1;
	} bit;
} OCS45STR;

__IO_EXTENDED	  OCS45STR	IO_OCS45;
#define	_ocs45		(IO_OCS45)
#define	OCS45		(IO_OCS45.hword)
#define	OCS45_OCSH45	(IO_OCS45.byte.OCSH45)
#define	OCS45_OCSL45	(IO_OCS45.byte.OCSL45)
#define	OCS45_CMOD  	(IO_OCS45.bit.CMOD)
#define	OCS45_OTD5  	(IO_OCS45.bit.OTD5)
#define	OCS45_OTD4  	(IO_OCS45.bit.OTD4)
#define	OCS45_IOP5  	(IO_OCS45.bit.IOP5)
#define	OCS45_IOP4  	(IO_OCS45.bit.IOP4)
#define	OCS45_IOE5  	(IO_OCS45.bit.IOE5)
#define	OCS45_IOE4  	(IO_OCS45.bit.IOE4)
#define	OCS45_CST5  	(IO_OCS45.bit.CST5)
#define	OCS45_CST4  	(IO_OCS45.bit.CST4)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_FT2,attr=IO,locate=0xFA0
#endif

__IO_EXTENDED	struct {
	__WORD	IO_CPCLR2;
	__WORD	IO_TCDT2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	TCCSH2;
			__BYTE	TCCSL2;
		} byte;
		struct {
			__HWORD	ECKE:1;
			__HWORD	RESV81:1;
			__HWORD	RESV82:1;
			__HWORD	RESV83:1;
			__HWORD	RESV84:1;
			__HWORD	RESV85:1;
			__HWORD	ICLR:1;
			__HWORD	ICRE:1;
			__HWORD	RESV88:1;
			__HWORD	STOP:1;
			__HWORD	RESV810:1;
			__HWORD	SCLR:1;
			__HWORD	CLK:4;
		} bit;
	} IO_TCCS2;
} IO_FT2;

#define	FT2_CPCLR2	(IO_FT2.IO_CPCLR2)
#define	FT2_TCDT2	(IO_FT2.IO_TCDT2)
#define	FT2_TCCS2			(IO_FT2.IO_TCCS2.hword)
#define	FT2_TCCS2_TCCSH2    	(IO_FT2.IO_TCCS2.byte.TCCSH2)
#define	FT2_TCCS2_TCCSL2    	(IO_FT2.IO_TCCS2.byte.TCCSL2)
#define	FT2_TCCS2_ECKE    	(IO_FT2.IO_TCCS2.bit.ECKE)
#define	FT2_TCCS2_ICLR    	(IO_FT2.IO_TCCS2.bit.ICLR)
#define	FT2_TCCS2_ICRE    	(IO_FT2.IO_TCCS2.bit.ICRE)
#define	FT2_TCCS2_STOP    	(IO_FT2.IO_TCCS2.bit.STOP)
#define	FT2_TCCS2_SCLR    	(IO_FT2.IO_TCCS2.bit.SCLR)
#define	FT2_TCCS2_CLK    	(IO_FT2.IO_TCCS2.bit.CLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_FT3,attr=IO,locate=0xFAC
#endif

__IO_EXTENDED	struct {
	__WORD	IO_CPCLR3;
	__WORD	IO_TCDT3;
	union {
		__HWORD	hword;
		struct {
			__BYTE	TCCSH3;
			__BYTE	TCCSL3;
		} byte;
		struct {
			__HWORD	ECKE:1;
			__HWORD	RESV40041:1;
			__HWORD	RESV40042:1;
			__HWORD	RESV40043:1;
			__HWORD	RESV40044:1;
			__HWORD	RESV40045:1;
			__HWORD	ICLR:1;
			__HWORD	ICRE:1;
			__HWORD	RESV40048:1;
			__HWORD	STOP:1;
			__HWORD	RESV400410:1;
			__HWORD	SCLR:1;
			__HWORD	CLK:4;
		} bit;
	} IO_TCCS3;
} IO_FT3;

#define	FT3_CPCLR3	(IO_FT3.IO_CPCLR3)
#define	FT3_TCDT3	(IO_FT3.IO_TCDT3)
#define	FT3_TCCS3			(IO_FT3.IO_TCCS3.hword)
#define	FT3_TCCS3_TCCSH3    	(IO_FT3.IO_TCCS3.byte.TCCSH3)
#define	FT3_TCCS3_TCCSL3    	(IO_FT3.IO_TCCS3.byte.TCCSL3)
#define	FT3_TCCS3_ECKE    	(IO_FT3.IO_TCCS3.bit.ECKE)
#define	FT3_TCCS3_ICLR    	(IO_FT3.IO_TCCS3.bit.ICLR)
#define	FT3_TCCS3_ICRE    	(IO_FT3.IO_TCCS3.bit.ICRE)
#define	FT3_TCCS3_STOP    	(IO_FT3.IO_TCCS3.bit.STOP)
#define	FT3_TCCS3_SCLR    	(IO_FT3.IO_TCCS3.bit.SCLR)
#define	FT3_TCCS3_CLK    	(IO_FT3.IO_TCCS3.bit.CLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_FT4,attr=IO,locate=0xFB8
#endif

__IO_EXTENDED	struct {
	__WORD	IO_CPCLR4;
	__WORD	IO_TCDT4;
	union {
		__HWORD	hword;
		struct {
			__BYTE	TCCSH4;
			__BYTE	TCCSL4;
		} byte;
		struct {
			__HWORD	ECKE:1;
			__HWORD	RESV91:1;
			__HWORD	RESV92:1;
			__HWORD	RESV93:1;
			__HWORD	RESV94:1;
			__HWORD	RESV95:1;
			__HWORD	ICLR:1;
			__HWORD	ICRE:1;
			__HWORD	RESV98:1;
			__HWORD	STOP:1;
			__HWORD	RESV910:1;
			__HWORD	SCLR:1;
			__HWORD	CLK:4;
		} bit;
	} IO_TCCS4;
} IO_FT4;

#define	FT4_CPCLR4	(IO_FT4.IO_CPCLR4)
#define	FT4_TCDT4	(IO_FT4.IO_TCDT4)
#define	FT4_TCCS4			(IO_FT4.IO_TCCS4.hword)
#define	FT4_TCCS4_TCCSH4    	(IO_FT4.IO_TCCS4.byte.TCCSH4)
#define	FT4_TCCS4_TCCSL4    	(IO_FT4.IO_TCCS4.byte.TCCSL4)
#define	FT4_TCCS4_ECKE    	(IO_FT4.IO_TCCS4.bit.ECKE)
#define	FT4_TCCS4_ICLR    	(IO_FT4.IO_TCCS4.bit.ICLR)
#define	FT4_TCCS4_ICRE    	(IO_FT4.IO_TCCS4.bit.ICRE)
#define	FT4_TCCS4_STOP    	(IO_FT4.IO_TCCS4.bit.STOP)
#define	FT4_TCCS4_SCLR    	(IO_FT4.IO_TCCS4.bit.SCLR)
#define	FT4_TCCS4_CLK    	(IO_FT4.IO_TCCS4.bit.CLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_FT5,attr=IO,locate=0xFC4
#endif

__IO_EXTENDED	struct {
	__WORD	IO_CPCLR5;
	__WORD	IO_TCDT5;
	union {
		__HWORD	hword;
		struct {
			__BYTE	TCCSH5;
			__BYTE	TCCSL5;
		} byte;
		struct {
			__HWORD	ECKE:1;
			__HWORD	RESV91:1;
			__HWORD	RESV92:1;
			__HWORD	RESV93:1;
			__HWORD	RESV94:1;
			__HWORD	RESV95:1;
			__HWORD	ICLR:1;
			__HWORD	ICRE:1;
			__HWORD	RESV98:1;
			__HWORD	STOP:1;
			__HWORD	RESV910:1;
			__HWORD	SCLR:1;
			__HWORD	CLK:4;
		} bit;
	} IO_TCCS5;
} IO_FT5;

#define	FT5_CPCLR5	(IO_FT5.IO_CPCLR5)
#define	FT5_TCDT5	(IO_FT5.IO_TCDT5)
#define	FT5_TCCS5			(IO_FT5.IO_TCCS5.hword)
#define	FT5_TCCS5_TCCSH5    	(IO_FT5.IO_TCCS5.byte.TCCSH5)
#define	FT5_TCCS5_TCCSL5    	(IO_FT5.IO_TCCS5.byte.TCCSL5)
#define	FT5_TCCS5_ECKE    	(IO_FT5.IO_TCCS5.bit.ECKE)
#define	FT5_TCCS5_ICLR    	(IO_FT5.IO_TCCS5.bit.ICLR)
#define	FT5_TCCS5_ICRE    	(IO_FT5.IO_TCCS5.bit.ICRE)
#define	FT5_TCCS5_STOP    	(IO_FT5.IO_TCCS5.bit.STOP)
#define	FT5_TCCS5_SCLR    	(IO_FT5.IO_TCCS5.bit.SCLR)
#define	FT5_TCCS5_CLK    	(IO_FT5.IO_TCCS5.bit.CLK)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IPCP6,attr=IO,locate=0xFD0
#endif

__IO_EXTENDED	__WORD	IO_IPCP6;
#define	_ipcp6		(IO_IPCP6)
#define	IPCP6	(_ipcp6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IPCP7,attr=IO,locate=0xFD4
#endif

__IO_EXTENDED	__WORD	IO_IPCP7;
#define	_ipcp7		(IO_IPCP7)
#define	IPCP7	(_ipcp7)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICFS67,attr=IO,locate=0xFD8
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SEL7:1;
			__BYTE	SEL6:1;
	} bit;
} ICFS67STR;

__IO_EXTENDED	  ICFS67STR	IO_ICFS67;
#define	_icfs67		(IO_ICFS67)
#define	ICFS67		(IO_ICFS67.byte)
#define	ICFS67_SEL7  	(IO_ICFS67.bit.SEL7)
#define	ICFS67_SEL6  	(IO_ICFS67.bit.SEL6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_LSYNS1,attr=IO,locate=0xFDA
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	LSYN9:1;
			__BYTE	LSYN8:1;
			__BYTE	LSYN7:1;
			__BYTE	LSYN6:1;
	} bit;
} LSYNS1STR;

__IO_EXTENDED	  LSYNS1STR	IO_LSYNS1;
#define	_lsyns1		(IO_LSYNS1)
#define	LSYNS1		(IO_LSYNS1.byte)
#define	LSYNS1_LSYN9  	(IO_LSYNS1.bit.LSYN9)
#define	LSYNS1_LSYN8  	(IO_LSYNS1.bit.LSYN8)
#define	LSYNS1_LSYN7  	(IO_LSYNS1.bit.LSYN7)
#define	LSYNS1_LSYN6  	(IO_LSYNS1.bit.LSYN6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICS67,attr=IO,locate=0xFDB
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICP7:1;
			__BYTE	ICP6:1;
			__BYTE	ICE7:1;
			__BYTE	ICE6:1;
			__BYTE	EG7:2;
			__BYTE	EG6:2;
	} bit;
} ICS67STR;

__IO_EXTENDED	  ICS67STR	IO_ICS67;
#define	_ics67		(IO_ICS67)
#define	ICS67		(IO_ICS67.byte)
#define	ICS67_ICP7  	(IO_ICS67.bit.ICP7)
#define	ICS67_ICP6  	(IO_ICS67.bit.ICP6)
#define	ICS67_ICE7  	(IO_ICS67.bit.ICE7)
#define	ICS67_ICE6  	(IO_ICS67.bit.ICE6)
#define	ICS67_EG7  	(IO_ICS67.bit.EG7)
#define	ICS67_EG6  	(IO_ICS67.bit.EG6)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IPCP8,attr=IO,locate=0xFDC
#endif

__IO_EXTENDED	const __WORD	IO_IPCP8;
#define	_ipcp8		(IO_IPCP8)
#define	IPCP8	(_ipcp8)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IPCP9,attr=IO,locate=0xFE0
#endif

__IO_EXTENDED	const __WORD	IO_IPCP9;
#define	_ipcp9		(IO_IPCP9)
#define	IPCP9	(_ipcp9)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICFS89,attr=IO,locate=0xFE4
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SEL9:1;
			__BYTE	SEL8:1;
	} bit;
} ICFS89STR;

__IO_EXTENDED	  ICFS89STR	IO_ICFS89;
#define	_icfs89		(IO_ICFS89)
#define	ICFS89		(IO_ICFS89.byte)
#define	ICFS89_SEL9  	(IO_ICFS89.bit.SEL9)
#define	ICFS89_SEL8  	(IO_ICFS89.bit.SEL8)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICS89,attr=IO,locate=0xFE7
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICP9:1;
			__BYTE	ICP8:1;
			__BYTE	ICE9:1;
			__BYTE	ICE8:1;
			__BYTE	EG9:2;
			__BYTE	EG8:2;
	} bit;
} ICS89STR;

__IO_EXTENDED	  ICS89STR	IO_ICS89;
#define	_ics89		(IO_ICS89)
#define	ICS89		(IO_ICS89.byte)
#define	ICS89_ICP9  	(IO_ICS89.bit.ICP9)
#define	ICS89_ICP8  	(IO_ICS89.bit.ICP8)
#define	ICS89_ICE9  	(IO_ICS89.bit.ICE9)
#define	ICS89_ICE8  	(IO_ICS89.bit.ICE8)
#define	ICS89_EG9  	(IO_ICS89.bit.EG9)
#define	ICS89_EG8  	(IO_ICS89.bit.EG8)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IPCP10,attr=IO,locate=0xFE8
#endif

__IO_EXTENDED	const __WORD	IO_IPCP10;
#define	_ipcp10		(IO_IPCP10)
#define	IPCP10	(_ipcp10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_IPCP11,attr=IO,locate=0xFEC
#endif

__IO_EXTENDED	const __WORD	IO_IPCP11;
#define	_ipcp11		(IO_IPCP11)
#define	IPCP11	(_ipcp11)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICFS1011,attr=IO,locate=0xFF0
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	SEL11:1;
			__BYTE	SEL10:1;
	} bit;
} ICFS1011STR;

__IO_EXTENDED	  ICFS1011STR	IO_ICFS1011;
#define	_icfs1011		(IO_ICFS1011)
#define	ICFS1011		(IO_ICFS1011.byte)
#define	ICFS1011_SEL11  	(IO_ICFS1011.bit.SEL11)
#define	ICFS1011_SEL10  	(IO_ICFS1011.bit.SEL10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_ICS1011,attr=IO,locate=0xFF3
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	ICP11:1;
			__BYTE	ICP10:1;
			__BYTE	ICE11:1;
			__BYTE	ICE10:1;
			__BYTE	EG11:2;
			__BYTE	EG10:2;
	} bit;
} ICS1011STR;

__IO_EXTENDED	  ICS1011STR	IO_ICS1011;
#define	_ics1011		(IO_ICS1011)
#define	ICS1011		(IO_ICS1011.byte)
#define	ICS1011_ICP11  	(IO_ICS1011.bit.ICP11)
#define	ICS1011_ICP10  	(IO_ICS1011.bit.ICP10)
#define	ICS1011_ICE11  	(IO_ICS1011.bit.ICE11)
#define	ICS1011_ICE10  	(IO_ICS1011.bit.ICE10)
#define	ICS1011_EG11  	(IO_ICS1011.bit.EG11)
#define	ICS1011_EG10  	(IO_ICS1011.bit.EG10)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_SACR,attr=IO,locate=0x1000
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	RESV6:1;
			__BYTE	M:1;
	} bit;
} SACRSTR;

__IO_EXTENDED	  SACRSTR	IO_SACR;
#define	_sacr		(IO_SACR)
#define	SACR		(IO_SACR.byte)
#define	SACR_M  	(IO_SACR.bit.M)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PICD,attr=IO,locate=0x1001
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	PDIV:5;
	} bit;
} PICDSTR;

__IO_EXTENDED	  PICDSTR	IO_PICD;
#define	_picd		(IO_PICD)
#define	PICD		(IO_PICD.byte)
#define	PICD_PDIV  	(IO_PICD.bit.PDIV)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_SGR0,attr=IO,locate=0x1040
#endif

__IO_EXTENDED	struct {
	__BYTE	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	ARE1:1;
			__BYTE	ARE0:1;
			__BYTE	FRE:1;
			__BYTE	NRE:1;
			__BYTE	TCRE:1;
			__BYTE	IDRE:1;
			__BYTE	PCRE1:1;
			__BYTE	PCRE0:1;
		} bit;
	} IO_SGDER0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGCR0H;
			__BYTE	SGCR0L;
		} byte;
		struct {
			__HWORD	RESV20:1;
			__HWORD	SRST:1;
			__HWORD	DMA:1;
			__HWORD	GID:1;
			__HWORD	GEN:1;
			__HWORD	RESV25:1;
			__HWORD	BUSY:1;
			__HWORD	RESV27:1;
			__HWORD	S:2;
			__HWORD	TONE:1;
			__HWORD	RESV211:1;
			__HWORD	RESV212:1;
			__HWORD	INTE:1;
			__HWORD	INT:1;
			__HWORD	ST:1;
		} bit;
	} IO_SGCR0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGAR0H;
			__BYTE	SGAR0L;
		} byte;
	} IO_SGAR0;
	__BYTE	IO_SGFR0;
	__BYTE	IO_SGNR0;
	__BYTE	IO_SGTCR0;
	__BYTE	IO_SGIDR0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGPCR0H;
			__BYTE	SGPCR0L;
		} byte;
	} IO_SGPCR0;
	union {
		__WORD	word;
		struct {
			__HWORD	SGDMAR0H;
			__HWORD	SGDMAR0L;
		} hword;
		struct {
			__BYTE	SGDMAR0HH;
			__BYTE	SGDMAR0HL;
			__BYTE	SGDMAR0LH;
			__BYTE	SGDMAR0LL;
		} byte;
	} IO_SGDMAR0;
} IO_SGR0;

#define	SGR0_SGDER0			(IO_SGR0.IO_SGDER0.byte)
#define	SGR0_SGDER0_ARE1    	(IO_SGR0.IO_SGDER0.bit.ARE1)
#define	SGR0_SGDER0_ARE0    	(IO_SGR0.IO_SGDER0.bit.ARE0)
#define	SGR0_SGDER0_FRE    	(IO_SGR0.IO_SGDER0.bit.FRE)
#define	SGR0_SGDER0_NRE    	(IO_SGR0.IO_SGDER0.bit.NRE)
#define	SGR0_SGDER0_TCRE    	(IO_SGR0.IO_SGDER0.bit.TCRE)
#define	SGR0_SGDER0_IDRE    	(IO_SGR0.IO_SGDER0.bit.IDRE)
#define	SGR0_SGDER0_PCRE1    	(IO_SGR0.IO_SGDER0.bit.PCRE1)
#define	SGR0_SGDER0_PCRE0    	(IO_SGR0.IO_SGDER0.bit.PCRE0)
#define	SGR0_SGCR0			(IO_SGR0.IO_SGCR0.hword)
#define	SGR0_SGCR0_SGCR0H	(IO_SGR0.IO_SGCR0.byte.SGCR0H)
#define	SGR0_SGCR0_SGCR0L	(IO_SGR0.IO_SGCR0.byte.SGCR0L)
#define	SGR0_SGCR0_SRST    	(IO_SGR0.IO_SGCR0.bit.SRST)
#define	SGR0_SGCR0_DMA    	(IO_SGR0.IO_SGCR0.bit.DMA)
#define	SGR0_SGCR0_GID    	(IO_SGR0.IO_SGCR0.bit.GID)
#define	SGR0_SGCR0_GEN    	(IO_SGR0.IO_SGCR0.bit.GEN)
#define	SGR0_SGCR0_BUSY    	(IO_SGR0.IO_SGCR0.bit.BUSY)
#define	SGR0_SGCR0_S    	(IO_SGR0.IO_SGCR0.bit.S)
#define	SGR0_SGCR0_TONE    	(IO_SGR0.IO_SGCR0.bit.TONE)
#define	SGR0_SGCR0_INTE    	(IO_SGR0.IO_SGCR0.bit.INTE)
#define	SGR0_SGCR0_INT    	(IO_SGR0.IO_SGCR0.bit.INT)
#define	SGR0_SGCR0_ST    	(IO_SGR0.IO_SGCR0.bit.ST)
#define	SGR0_SGAR0		(IO_SGR0.IO_SGAR0.hword)
#define	SGR0_SGAR0_SGAR0H	(IO_SGR0.IO_SGAR0.byte.SGAR0H)
#define	SGR0_SGAR0_SGAR0L	(IO_SGR0.IO_SGAR0.byte.SGAR0L)
#define	SGR0_SGFR0	(IO_SGR0.IO_SGFR0)
#define	SGR0_SGNR0	(IO_SGR0.IO_SGNR0)
#define	SGR0_SGTCR0	(IO_SGR0.IO_SGTCR0)
#define	SGR0_SGIDR0	(IO_SGR0.IO_SGIDR0)
#define	SGR0_SGPCR0		(IO_SGR0.IO_SGPCR0.hword)
#define	SGR0_SGPCR0_SGPCR0H	(IO_SGR0.IO_SGPCR0.byte.SGPCR0H)
#define	SGR0_SGPCR0_SGPCR0L	(IO_SGR0.IO_SGPCR0.byte.SGPCR0L)
#define	SGR0_SGDMAR0		(IO_SGR0.IO_SGDMAR0.word)
#define	SGR0_SGDMAR0_SGDMAR0H	(IO_SGR0.IO_SGDMAR0.hword.SGDMAR0H)
#define	SGR0_SGDMAR0_SGDMAR0L	(IO_SGR0.IO_SGDMAR0.hword.SGDMAR0L)
#define	SGR0_SGDMAR0_SGDMAR0HH	(IO_SGR0.IO_SGDMAR0.byte.SGDMAR0HH)
#define	SGR0_SGDMAR0_SGDMAR0HL	(IO_SGR0.IO_SGDMAR0.byte.SGDMAR0HL)
#define	SGR0_SGDMAR0_SGDMAR0LH	(IO_SGR0.IO_SGDMAR0.byte.SGDMAR0LH)
#define	SGR0_SGDMAR0_SGDMAR0LL	(IO_SGR0.IO_SGDMAR0.byte.SGDMAR0LL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_SGR1,attr=IO,locate=0x1060
#endif

__IO_EXTENDED	struct {
	__BYTE	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	ARE1:1;
			__BYTE	ARE0:1;
			__BYTE	FRE:1;
			__BYTE	NRE:1;
			__BYTE	TCRE:1;
			__BYTE	IDRE:1;
			__BYTE	PCRE1:1;
			__BYTE	PCRE0:1;
		} bit;
	} IO_SGDER1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGCR1H;
			__BYTE	SGCR1L;
		} byte;
		struct {
			__HWORD	RESV20:1;
			__HWORD	SRST:1;
			__HWORD	DMA:1;
			__HWORD	GID:1;
			__HWORD	GEN:1;
			__HWORD	RESV25:1;
			__HWORD	BUSY:1;
			__HWORD	RESV27:1;
			__HWORD	S:2;
			__HWORD	TONE:1;
			__HWORD	RESV210:1;
			__HWORD	RESV211:1;
			__HWORD	INTE:1;
			__HWORD	INT:1;
			__HWORD	ST:1;
		} bit;
	} IO_SGCR1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGAR1H;
			__BYTE	SGAR1L;
		} byte;
	} IO_SGAR1;
	__BYTE	IO_SGFR1;
	__BYTE	IO_SGNR1;
	__BYTE	IO_SGTCR1;
	__BYTE	IO_SGIDR1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGPCR1H;
			__BYTE	SGPCR1L;
		} byte;
	} IO_SGPCR1;
	union {
		__WORD	word;
		struct {
			__HWORD	SGDMAR1H;
			__HWORD	SGDMAR1L;
		} hword;
		struct {
			__BYTE	SGDMAR1HH;
			__BYTE	SGDMAR1HL;
			__BYTE	SGDMAR1LH;
			__BYTE	SGDMAR1LL;
		} byte;
	} IO_SGDMAR1;
} IO_SGR1;

#define	SGR1_SGDER1			(IO_SGR1.IO_SGDER1.byte)
#define	SGR1_SGDER1_ARE1    	(IO_SGR1.IO_SGDER1.bit.ARE1)
#define	SGR1_SGDER1_ARE0    	(IO_SGR1.IO_SGDER1.bit.ARE0)
#define	SGR1_SGDER1_FRE    	(IO_SGR1.IO_SGDER1.bit.FRE)
#define	SGR1_SGDER1_NRE    	(IO_SGR1.IO_SGDER1.bit.NRE)
#define	SGR1_SGDER1_TCRE    	(IO_SGR1.IO_SGDER1.bit.TCRE)
#define	SGR1_SGDER1_IDRE    	(IO_SGR1.IO_SGDER1.bit.IDRE)
#define	SGR1_SGDER1_PCRE1    	(IO_SGR1.IO_SGDER1.bit.PCRE1)
#define	SGR1_SGDER1_PCRE0    	(IO_SGR1.IO_SGDER1.bit.PCRE0)
#define	SGR1_SGCR1			(IO_SGR1.IO_SGCR1.hword)
#define	SGR1_SGCR1_SGCR1H	(IO_SGR1.IO_SGCR1.byte.SGCR1H)
#define	SGR1_SGCR1_SGCR1L	(IO_SGR1.IO_SGCR1.byte.SGCR1L)
#define	SGR1_SGCR1_SRST    	(IO_SGR1.IO_SGCR1.bit.SRST)
#define	SGR1_SGCR1_DMA    	(IO_SGR1.IO_SGCR1.bit.DMA)
#define	SGR1_SGCR1_GID    	(IO_SGR1.IO_SGCR1.bit.GID)
#define	SGR1_SGCR1_GEN    	(IO_SGR1.IO_SGCR1.bit.GEN)
#define	SGR1_SGCR1_BUSY    	(IO_SGR1.IO_SGCR1.bit.BUSY)
#define	SGR1_SGCR1_S    	(IO_SGR1.IO_SGCR1.bit.S)
#define	SGR1_SGCR1_TONE    	(IO_SGR1.IO_SGCR1.bit.TONE)
#define	SGR1_SGCR1_INTE    	(IO_SGR1.IO_SGCR1.bit.INTE)
#define	SGR1_SGCR1_INT    	(IO_SGR1.IO_SGCR1.bit.INT)
#define	SGR1_SGCR1_ST    	(IO_SGR1.IO_SGCR1.bit.ST)
#define	SGR1_SGAR1		(IO_SGR1.IO_SGAR1.hword)
#define	SGR1_SGAR1_SGAR1H	(IO_SGR1.IO_SGAR1.byte.SGAR1H)
#define	SGR1_SGAR1_SGAR1L	(IO_SGR1.IO_SGAR1.byte.SGAR1L)
#define	SGR1_SGFR1	(IO_SGR1.IO_SGFR1)
#define	SGR1_SGNR1	(IO_SGR1.IO_SGNR1)
#define	SGR1_SGTCR1	(IO_SGR1.IO_SGTCR1)
#define	SGR1_SGIDR1	(IO_SGR1.IO_SGIDR1)
#define	SGR1_SGPCR1	(IO_SGR1.IO_SGPCR1.hword)
#define	SGR1_SGPCR1_SGPCR1H	(IO_SGR1.IO_SGPCR1.byte.SGPCR1H)
#define	SGR1_SGPCR1_SGPCR1L	(IO_SGR1.IO_SGPCR1.byte.SGPCR1L)
#define	SGR1_SGDMAR1		(IO_SGR1.IO_SGDMAR1.word)
#define	SGR1_SGDMAR1_SGDMAR1H	(IO_SGR1.IO_SGDMAR1.hword.SGDMAR1H)
#define	SGR1_SGDMAR1_SGDMAR1L	(IO_SGR1.IO_SGDMAR1.hword.SGDMAR1L)
#define	SGR1_SGDMAR1_SGDMAR1HH	(IO_SGR1.IO_SGDMAR1.byte.SGDMAR1HH)
#define	SGR1_SGDMAR1_SGDMAR1HL	(IO_SGR1.IO_SGDMAR1.byte.SGDMAR1HL)
#define	SGR1_SGDMAR1_SGDMAR1LH	(IO_SGR1.IO_SGDMAR1.byte.SGDMAR1LH)
#define	SGR1_SGDMAR1_SGDMAR1LL	(IO_SGR1.IO_SGDMAR1.byte.SGDMAR1LL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_SGR2,attr=IO,locate=0x1080
#endif

__IO_EXTENDED	struct {
	__BYTE	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	ARE1:1;
			__BYTE	ARE0:1;
			__BYTE	FRE:1;
			__BYTE	NRE:1;
			__BYTE	TCRE:1;
			__BYTE	IDRE:1;
			__BYTE	PCRE1:1;
			__BYTE	PCRE0:1;
		} bit;
	} IO_SGDER2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGCR2H;
			__BYTE	SGCR2L;
		} byte;
		struct {
			__HWORD	RESV20:1;
			__HWORD	SRST:1;
			__HWORD	DMA:1;
			__HWORD	GID:1;
			__HWORD	GEN:1;
			__HWORD	RESV25:1;
			__HWORD	BUSY:1;
			__HWORD	RESV27:1;
			__HWORD	S:2;
			__HWORD	TONE:1;
			__HWORD	RESV210:1;
			__HWORD	RESV211:1;
			__HWORD	INTE:1;
			__HWORD	INT:1;
			__HWORD	ST:1;
		} bit;
	} IO_SGCR2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGAR2H;
			__BYTE	SGAR2L;
		} byte;
	} IO_SGAR2;
	__BYTE	IO_SGFR2;
	__BYTE	IO_SGNR2;
	__BYTE	IO_SGTCR2;
	__BYTE	IO_SGIDR2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGPCR2H;
			__BYTE	SGPCR2L;
		} byte;
	} IO_SGPCR2;
	union {
		__WORD	word;
		struct {
			__HWORD	SGDMAR2H;
			__HWORD	SGDMAR2L;
		} hword;
		struct {
			__BYTE	SGDMAR2HH;
			__BYTE	SGDMAR2HL;
			__BYTE	SGDMAR2LH;
			__BYTE	SGDMAR2LL;
		} byte;
	} IO_SGDMAR2;
} IO_SGR2;

#define	SGR2_SGDER2			(IO_SGR2.IO_SGDER2.byte)
#define	SGR2_SGDER2_ARE1    	(IO_SGR2.IO_SGDER2.bit.ARE1)
#define	SGR2_SGDER2_ARE0    	(IO_SGR2.IO_SGDER2.bit.ARE0)
#define	SGR2_SGDER2_FRE    	(IO_SGR2.IO_SGDER2.bit.FRE)
#define	SGR2_SGDER2_NRE    	(IO_SGR2.IO_SGDER2.bit.NRE)
#define	SGR2_SGDER2_TCRE    	(IO_SGR2.IO_SGDER2.bit.TCRE)
#define	SGR2_SGDER2_IDRE    	(IO_SGR2.IO_SGDER2.bit.IDRE)
#define	SGR2_SGDER2_PCRE1    	(IO_SGR2.IO_SGDER2.bit.PCRE1)
#define	SGR2_SGDER2_PCRE0    	(IO_SGR2.IO_SGDER2.bit.PCRE0)
#define	SGR2_SGCR2			(IO_SGR2.IO_SGCR2.hword)
#define	SGR2_SGCR2_SGCR2H	(IO_SGR2.IO_SGCR2.byte.SGCR2H)
#define	SGR2_SGCR2_SGCR2L	(IO_SGR2.IO_SGCR2.byte.SGCR2L)
#define	SGR2_SGCR2_SRST    	(IO_SGR2.IO_SGCR2.bit.SRST)
#define	SGR2_SGCR2_DMA    	(IO_SGR2.IO_SGCR2.bit.DMA)
#define	SGR2_SGCR2_GID    	(IO_SGR2.IO_SGCR2.bit.GID)
#define	SGR2_SGCR2_GEN    	(IO_SGR2.IO_SGCR2.bit.GEN)
#define	SGR2_SGCR2_BUSY    	(IO_SGR2.IO_SGCR2.bit.BUSY)
#define	SGR2_SGCR2_S    	(IO_SGR2.IO_SGCR2.bit.S)
#define	SGR2_SGCR2_TONE    	(IO_SGR2.IO_SGCR2.bit.TONE)
#define	SGR2_SGCR2_INTE    	(IO_SGR2.IO_SGCR2.bit.INTE)
#define	SGR2_SGCR2_INT    	(IO_SGR2.IO_SGCR2.bit.INT)
#define	SGR2_SGCR2_ST    	(IO_SGR2.IO_SGCR2.bit.ST)
#define	SGR2_SGAR2		(IO_SGR2.IO_SGAR2.hword)
#define	SGR2_SGAR2_SGAR2H	(IO_SGR2.IO_SGAR2.byte.SGAR2H)
#define	SGR2_SGAR2_SGAR2L	(IO_SGR2.IO_SGAR2.byte.SGAR2L)
#define	SGR2_SGFR2	(IO_SGR2.IO_SGFR2)
#define	SGR2_SGNR2	(IO_SGR2.IO_SGNR2)
#define	SGR2_SGTCR2	(IO_SGR2.IO_SGTCR2)
#define	SGR2_SGIDR2	(IO_SGR2.IO_SGIDR2)
#define	SGR2_SGPCR2	(IO_SGR2.IO_SGPCR2.hword)
#define	SGR2_SGPCR2_SGPCR2H	(IO_SGR2.IO_SGPCR2.byte.SGPCR2H)
#define	SGR2_SGPCR2_SGPCR2L	(IO_SGR2.IO_SGPCR2.byte.SGPCR2L)
#define	SGR2_SGDMAR2		(IO_SGR2.IO_SGDMAR2.word)
#define	SGR2_SGDMAR2_SGDMAR2H	(IO_SGR2.IO_SGDMAR2.hword.SGDMAR2H)
#define	SGR2_SGDMAR2_SGDMAR2L	(IO_SGR2.IO_SGDMAR2.hword.SGDMAR2L)
#define	SGR2_SGDMAR2_SGDMAR2HH	(IO_SGR2.IO_SGDMAR2.byte.SGDMAR2HH)
#define	SGR2_SGDMAR2_SGDMAR2HL	(IO_SGR2.IO_SGDMAR2.byte.SGDMAR2HL)
#define	SGR2_SGDMAR2_SGDMAR2LH	(IO_SGR2.IO_SGDMAR2.byte.SGDMAR2LH)
#define	SGR2_SGDMAR2_SGDMAR2LL	(IO_SGR2.IO_SGDMAR2.byte.SGDMAR2LL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_SGR3,attr=IO,locate=0x10A0
#endif

__IO_EXTENDED	struct {
	__BYTE	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	ARE1:1;
			__BYTE	ARE0:1;
			__BYTE	FRE:1;
			__BYTE	NRE:1;
			__BYTE	TCRE:1;
			__BYTE	IDRE:1;
			__BYTE	PCRE1:1;
			__BYTE	PCRE0:1;
		} bit;
	} IO_SGDER3;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGCR3H;
			__BYTE	SGCR3L;
		} byte;
		struct {
			__HWORD	RESV20:1;
			__HWORD	SRST:1;
			__HWORD	DMA:1;
			__HWORD	GID:1;
			__HWORD	GEN:1;
			__HWORD	RESV25:1;
			__HWORD	BUSY:1;
			__HWORD	RESV27:1;
			__HWORD	S:2;
			__HWORD	TONE:1;
			__HWORD	RESV210:1;
			__HWORD	RESV211:1;
			__HWORD	INTE:1;
			__HWORD	INT:1;
			__HWORD	ST:1;
		} bit;
	} IO_SGCR3;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGAR3H;
			__BYTE	SGAR3L;
		} byte;
	} IO_SGAR3;
	__BYTE	IO_SGFR3;
	__BYTE	IO_SGNR3;
	__BYTE	IO_SGTCR3;
	__BYTE	IO_SGIDR3;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGPCR3H;
			__BYTE	SGPCR3L;
		} byte;
	} IO_SGPCR3;
	union {
		__WORD	word;
		struct {
			__HWORD	SGDMAR3H;
			__HWORD	SGDMAR3L;
		} hword;
		struct {
			__BYTE	SGDMAR3HH;
			__BYTE	SGDMAR3HL;
			__BYTE	SGDMAR3LH;
			__BYTE	SGDMAR3LL;
		} byte;
	} IO_SGDMAR3;
} IO_SGR3;

#define	SGR3_SGDER3			(IO_SGR3.IO_SGDER3.byte)
#define	SGR3_SGDER3_ARE1    	(IO_SGR3.IO_SGDER3.bit.ARE1)
#define	SGR3_SGDER3_ARE0    	(IO_SGR3.IO_SGDER3.bit.ARE0)
#define	SGR3_SGDER3_FRE    	(IO_SGR3.IO_SGDER3.bit.FRE)
#define	SGR3_SGDER3_NRE    	(IO_SGR3.IO_SGDER3.bit.NRE)
#define	SGR3_SGDER3_TCRE    	(IO_SGR3.IO_SGDER3.bit.TCRE)
#define	SGR3_SGDER3_IDRE    	(IO_SGR3.IO_SGDER3.bit.IDRE)
#define	SGR3_SGDER3_PCRE1    	(IO_SGR3.IO_SGDER3.bit.PCRE1)
#define	SGR3_SGDER3_PCRE0    	(IO_SGR3.IO_SGDER3.bit.PCRE0)
#define	SGR3_SGCR3			(IO_SGR3.IO_SGCR3.hword)
#define	SGR3_SGCR3_SGCR3H	(IO_SGR3.IO_SGCR3.byte.SGCR3H)
#define	SGR3_SGCR3_SGCR3L	(IO_SGR3.IO_SGCR3.byte.SGCR3L)
#define	SGR3_SGCR3_SRST    	(IO_SGR3.IO_SGCR3.bit.SRST)
#define	SGR3_SGCR3_DMA    	(IO_SGR3.IO_SGCR3.bit.DMA)
#define	SGR3_SGCR3_GID    	(IO_SGR3.IO_SGCR3.bit.GID)
#define	SGR3_SGCR3_GEN    	(IO_SGR3.IO_SGCR3.bit.GEN)
#define	SGR3_SGCR3_BUSY    	(IO_SGR3.IO_SGCR3.bit.BUSY)
#define	SGR3_SGCR3_S    	(IO_SGR3.IO_SGCR3.bit.S)
#define	SGR3_SGCR3_TONE    	(IO_SGR3.IO_SGCR3.bit.TONE)
#define	SGR3_SGCR3_INTE    	(IO_SGR3.IO_SGCR3.bit.INTE)
#define	SGR3_SGCR3_INT    	(IO_SGR3.IO_SGCR3.bit.INT)
#define	SGR3_SGCR3_ST    	(IO_SGR3.IO_SGCR3.bit.ST)
#define	SGR3_SGAR3		(IO_SGR3.IO_SGAR3.hword)
#define	SGR3_SGAR3_SGAR3H	(IO_SGR3.IO_SGAR3.byte.SGAR3H)
#define	SGR3_SGAR3_SGAR3L	(IO_SGR3.IO_SGAR3.byte.SGAR3L)
#define	SGR3_SGFR3	(IO_SGR3.IO_SGFR3)
#define	SGR3_SGNR3	(IO_SGR3.IO_SGNR3)
#define	SGR3_SGTCR3	(IO_SGR3.IO_SGTCR3)
#define	SGR3_SGIDR3	(IO_SGR3.IO_SGIDR3)
#define	SGR3_SGPCR3	(IO_SGR3.IO_SGPCR3.hword)
#define	SGR3_SGPCR3_SGPCR3H	(IO_SGR3.IO_SGPCR3.byte.SGPCR3H)
#define	SGR3_SGPCR3_SGPCR3L	(IO_SGR3.IO_SGPCR3.byte.SGPCR3L)
#define	SGR3_SGDMAR3		(IO_SGR3.IO_SGDMAR3.word)
#define	SGR3_SGDMAR3_SGDMAR3H	(IO_SGR3.IO_SGDMAR3.hword.SGDMAR3H)
#define	SGR3_SGDMAR3_SGDMAR3L	(IO_SGR3.IO_SGDMAR3.hword.SGDMAR3L)
#define	SGR3_SGDMAR3_SGDMAR3HH	(IO_SGR3.IO_SGDMAR3.byte.SGDMAR3HH)
#define	SGR3_SGDMAR3_SGDMAR3HL	(IO_SGR3.IO_SGDMAR3.byte.SGDMAR3HL)
#define	SGR3_SGDMAR3_SGDMAR3LH	(IO_SGR3.IO_SGDMAR3.byte.SGDMAR3LH)
#define	SGR3_SGDMAR3_SGDMAR3LL	(IO_SGR3.IO_SGDMAR3.byte.SGDMAR3LL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_SGR4,attr=IO,locate=0x10C0
#endif

__IO_EXTENDED	struct {
	__BYTE	IO_RESV1;
	union {
		__BYTE	byte;
		struct {
			__BYTE	ARE1:1;
			__BYTE	ARE0:1;
			__BYTE	FRE:1;
			__BYTE	NRE:1;
			__BYTE	TCRE:1;
			__BYTE	IDRE:1;
			__BYTE	PCRE1:1;
			__BYTE	PCRE0:1;
		} bit;
	} IO_SGDER4;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGCR4H;
			__BYTE	SGCR4L;
		} byte;
		struct {
			__HWORD	RESV20:1;
			__HWORD	SRST:1;
			__HWORD	DMA:1;
			__HWORD	GID:1;
			__HWORD	GEN:1;
			__HWORD	RESV25:1;
			__HWORD	BUSY:1;
			__HWORD	RESV27:1;
			__HWORD	S:2;
			__HWORD	TONE:1;
			__HWORD	RESV210:1;
			__HWORD	RESV211:1;
			__HWORD	INTE:1;
			__HWORD	INT:1;
			__HWORD	ST:1;
		} bit;
	} IO_SGCR4;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGAR4H;
			__BYTE	SGAR4L;
		} byte;
	} IO_SGAR4;
	__BYTE	IO_SGFR4;
	__BYTE	IO_SGNR4;
	__BYTE	IO_SGTCR4;
	__BYTE	IO_SGIDR4;
	union {
		__HWORD	hword;
		struct {
			__BYTE	SGPCR4H;
			__BYTE	SGPCR4L;
		} byte;
	} IO_SGPCR4;
	union {
		__WORD	word;
		struct {
			__HWORD	SGDMAR4H;
			__HWORD	SGDMAR4L;
		} hword;
		struct {
			__BYTE	SGDMAR4HH;
			__BYTE	SGDMAR4HL;
			__BYTE	SGDMAR4LH;
			__BYTE	SGDMAR4LL;
		} byte;
	} IO_SGDMAR4;
} IO_SGR4;

#define	SGR4_SGDER4			(IO_SGR4.IO_SGDER4.byte)
#define	SGR4_SGDER4_ARE1    	(IO_SGR4.IO_SGDER4.bit.ARE1)
#define	SGR4_SGDER4_ARE0    	(IO_SGR4.IO_SGDER4.bit.ARE0)
#define	SGR4_SGDER4_FRE    	(IO_SGR4.IO_SGDER4.bit.FRE)
#define	SGR4_SGDER4_NRE    	(IO_SGR4.IO_SGDER4.bit.NRE)
#define	SGR4_SGDER4_TCRE    	(IO_SGR4.IO_SGDER4.bit.TCRE)
#define	SGR4_SGDER4_IDRE    	(IO_SGR4.IO_SGDER4.bit.IDRE)
#define	SGR4_SGDER4_PCRE1    	(IO_SGR4.IO_SGDER4.bit.PCRE1)
#define	SGR4_SGDER4_PCRE0    	(IO_SGR4.IO_SGDER4.bit.PCRE0)
#define	SGR4_SGCR4			(IO_SGR4.IO_SGCR4.hword)
#define	SGR4_SGCR4_SGCR4H	(IO_SGR4.IO_SGCR4.byte.SGCR4H)
#define	SGR4_SGCR4_SGCR4L	(IO_SGR4.IO_SGCR4.byte.SGCR4L)
#define	SGR4_SGCR4_SRST    	(IO_SGR4.IO_SGCR4.bit.SRST)
#define	SGR4_SGCR4_DMA    	(IO_SGR4.IO_SGCR4.bit.DMA)
#define	SGR4_SGCR4_GID    	(IO_SGR4.IO_SGCR4.bit.GID)
#define	SGR4_SGCR4_GEN    	(IO_SGR4.IO_SGCR4.bit.GEN)
#define	SGR4_SGCR4_BUSY    	(IO_SGR4.IO_SGCR4.bit.BUSY)
#define	SGR4_SGCR4_S    	(IO_SGR4.IO_SGCR4.bit.S)
#define	SGR4_SGCR4_TONE    	(IO_SGR4.IO_SGCR4.bit.TONE)
#define	SGR4_SGCR4_INTE    	(IO_SGR4.IO_SGCR4.bit.INTE)
#define	SGR4_SGCR4_INT    	(IO_SGR4.IO_SGCR4.bit.INT)
#define	SGR4_SGCR4_ST    	(IO_SGR4.IO_SGCR4.bit.ST)
#define	SGR4_SGAR4		(IO_SGR4.IO_SGAR4.hword)
#define	SGR4_SGAR4_SGAR4H	(IO_SGR4.IO_SGAR4.byte.SGAR4H)
#define	SGR4_SGAR4_SGAR4L	(IO_SGR4.IO_SGAR4.byte.SGAR4L)
#define	SGR4_SGFR4	(IO_SGR4.IO_SGFR4)
#define	SGR4_SGNR4	(IO_SGR4.IO_SGNR4)
#define	SGR4_SGTCR4	(IO_SGR4.IO_SGTCR4)
#define	SGR4_SGIDR4	(IO_SGR4.IO_SGIDR4)
#define	SGR4_SGPCR4	(IO_SGR4.IO_SGPCR4.hword)
#define	SGR4_SGPCR4_SGPCR4H	(IO_SGR4.IO_SGPCR4.byte.SGPCR4H)
#define	SGR4_SGPCR4_SGPCR4L	(IO_SGR4.IO_SGPCR4.byte.SGPCR4L)
#define	SGR4_SGDMAR4		(IO_SGR4.IO_SGDMAR4.word)
#define	SGR4_SGDMAR4_SGDMAR4H	(IO_SGR4.IO_SGDMAR4.hword.SGDMAR4H)
#define	SGR4_SGDMAR4_SGDMAR4L	(IO_SGR4.IO_SGDMAR4.hword.SGDMAR4L)
#define	SGR4_SGDMAR4_SGDMAR4HH	(IO_SGR4.IO_SGDMAR4.byte.SGDMAR4HH)
#define	SGR4_SGDMAR4_SGDMAR4HL	(IO_SGR4.IO_SGDMAR4.byte.SGDMAR4HL)
#define	SGR4_SGDMAR4_SGDMAR4LH	(IO_SGR4.IO_SGDMAR4.byte.SGDMAR4LH)
#define	SGR4_SGDMAR4_SGDMAR4LL	(IO_SGR4.IO_SGDMAR4.byte.SGDMAR4LL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CRCCR,attr=IO,locate=0x1133
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	FXOR:1;
			__BYTE	CRCLSF:1;
			__BYTE	CRCLTE:1;
			__BYTE	LSBFST:1;
			__BYTE	LTLEND:1;
			__BYTE	CRC32:1;
			__BYTE	INIT:1;
	} bit;
} CRCCRSTR;

__IO_EXTENDED	  CRCCRSTR	IO_CRCCR;
#define	_crccr		(IO_CRCCR)
#define	CRCCR		(IO_CRCCR.byte)
#define	CRCCR_FXOR  	(IO_CRCCR.bit.FXOR)
#define	CRCCR_CRCLSF  	(IO_CRCCR.bit.CRCLSF)
#define	CRCCR_CRCLTE  	(IO_CRCCR.bit.CRCLTE)
#define	CRCCR_LSBFST  	(IO_CRCCR.bit.LSBFST)
#define	CRCCR_LTLEND  	(IO_CRCCR.bit.LTLEND)
#define	CRCCR_CRC32  	(IO_CRCCR.bit.CRC32)
#define	CRCCR_INIT  	(IO_CRCCR.bit.INIT)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CRCINIT,attr=IO,locate=0x1134
#endif

__IO_EXTENDED	union {
	__WORD	word;
	struct {
		__HWORD	CRCINITH;
		__HWORD	CRCINITL;
	} hword;
	struct {
		__BYTE	CRCINITHH;
		__BYTE	CRCINITHL;
		__BYTE	CRCINITLH;
		__BYTE	CRCINITLL;
	} byte;
} IO_CRCINIT;

#define	CRCINIT		(IO_CRCINIT.word)
#define	CRCINIT_CRCINITH	(IO_CRCINIT.hword.CRCINITH)
#define	CRCINIT_CRCINITL	(IO_CRCINIT.hword.CRCINITL)
#define	CRCINIT_CRCINITHH	(IO_CRCINIT.byte.CRCINITHH)
#define	CRCINIT_CRCINITHL	(IO_CRCINIT.byte.CRCINITHL)
#define	CRCINIT_CRCINITLH	(IO_CRCINIT.byte.CRCINITLH)
#define	CRCINIT_CRCINITLL	(IO_CRCINIT.byte.CRCINITLL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CRCIN,attr=IO,locate=0x1138
#endif

__IO_EXTENDED	union {
	__WORD	word;
	struct {
		__HWORD	CRCINH;
		__HWORD	CRCINL;
	} hword;
	struct {
		__BYTE	CRCINHH;
		__BYTE	CRCINHL;
		__BYTE	CRCINLH;
		__BYTE	CRCINLL;
	} byte;
} IO_CRCIN;

#define	CRCIN		(IO_CRCIN.word)
#define	CRCIN_CRCINH	(IO_CRCIN.hword.CRCINH)
#define	CRCIN_CRCINL	(IO_CRCIN.hword.CRCINL)
#define	CRCIN_CRCINHH	(IO_CRCIN.byte.CRCINHH)
#define	CRCIN_CRCINHL	(IO_CRCIN.byte.CRCINHL)
#define	CRCIN_CRCINLH	(IO_CRCIN.byte.CRCINLH)
#define	CRCIN_CRCINLL	(IO_CRCIN.byte.CRCINLL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CRCR,attr=IO,locate=0x113C
#endif

__IO_EXTENDED	const	union {
	__WORD	word;
	struct {
		__HWORD	CRCRH;
		__HWORD	CRCRL;
	} hword;
	struct {
		__BYTE	CRCRHH;
		__BYTE	CRCRHL;
		__BYTE	CRCRLH;
		__BYTE	CRCRLL;
	} byte;
} IO_CRCR;

#define	CRCR		(IO_CRCR.word)
#define	CRCR_CRCRH	(IO_CRCR.hword.CRCRH)
#define	CRCR_CRCRL	(IO_CRCR.hword.CRCRL)
#define	CRCR_CRCRHH	(IO_CRCR.byte.CRCRHH)
#define	CRCR_CRCRHL	(IO_CRCR.byte.CRCRHL)
#define	CRCR_CRCRLH	(IO_CRCR.byte.CRCRLH)
#define	CRCR_CRCRLL	(IO_CRCR.byte.CRCRLL)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CAN0,attr=IO,locate=0x2000
#endif

__IO_EXTENDED	struct {
	union {
		__HWORD	hword;
		struct {
			__BYTE	CTRLR0H;
			__BYTE	CTRLR0L;
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
	} IO_CTRLR0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	STATR0H;
			__BYTE	STATR0L;
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
	} IO_STATR0;
	const union {
		__HWORD	hword;
		struct {
			__BYTE	ERRCNT0H;
			__BYTE	ERRCNT0L;
		} byte;
		struct {
			__HWORD	RP:1;
			__HWORD	REC:7;
			__HWORD	TEC:8;
		} bit;
	} IO_ERRCNT0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	BTR0H;
			__BYTE	BTR0L;
		} byte;
		struct {
			__HWORD	RESV60:1;
			__HWORD	TSeg2:3;
			__HWORD	TSeg1:4;
			__HWORD	SJW:2;
			__HWORD	BRP:6;
		} bit;
	} IO_BTR0;
	const union {
		__HWORD	hword;
		struct {
			__BYTE	INTR0H;
			__BYTE	INTR0L;
		} byte;
	} IO_INTR0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	TESTR0H;
			__BYTE	TESTR0L;
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
	} IO_TESTR0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	BRPER0H;
			__BYTE	BRPER0L;
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
	} IO_BRPER0;
	__HWORD	IO_RESV1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1CREQ0H;
			__BYTE	IF1CREQ0L;
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
	} IO_IF1CREQ0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1CMSK0H;
			__BYTE	IF1CMSK0L;
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
	} IO_IF1CMSK0;
	union {
		__WORD	word;
		struct {
			__HWORD	IF1MSK20;
			__HWORD	IF1MSK10;
		} hword;
		struct {
			__BYTE	IF1MSK20H;
			__BYTE	IF1MSK20L;
			__BYTE	IF1MSK10H;
			__BYTE	IF1MSK10L;
		} byte;
		struct {
			__WORD	MXtd:1;
			__WORD	MDir:1;
			__WORD	RESV202:1;
			__WORD	Msk:29;
		} bit;
	} IO_IF1MSK0;
	union {
		__WORD	word;
		struct {
			__HWORD	IF1ARB20;
			__HWORD	IF1ARB10;
		} hword;
		struct {
			__BYTE	IF1ARB20H;
			__BYTE	IF1ARB20L;
			__BYTE	IF1ARB10H;
			__BYTE	IF1ARB10L;
		} byte;
		struct {
			__WORD	MsgVal:1;
			__WORD	Xtd:1;
			__WORD	Dir:1;
			__WORD	ID:29;
		} bit;
	} IO_IF1ARB0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1MCTR0H;
			__BYTE	IF1MCTR0L;
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
	} IO_IF1MCTR0;
	__HWORD	IO_RESV2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTA10H;
			__BYTE	IF1DTA10L;
		} byte;
	} IO_IF1DTA10;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTA20H;
			__BYTE	IF1DTA20L;
		} byte;
	} IO_IF1DTA20;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTB10H;
			__BYTE	IF1DTB10L;
		} byte;
	} IO_IF1DTB10;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTB20H;
			__BYTE	IF1DTB20L;
		} byte;
	} IO_IF1DTB20;
	__WORD	IO_RESV3[CONSTANT_6];
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2CREQ0H;
			__BYTE	IF2CREQ0L;
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
	} IO_IF2CREQ0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2CMSK0H;
			__BYTE	IF2CMSK0L;
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
	} IO_IF2CMSK0;
	union {
		__WORD	word;
		struct {
			__HWORD	IF2MSK20;
			__HWORD	IF2MSK10;
		} hword;
		struct {
			__BYTE	IF2MSK20H;
			__BYTE	IF2MSK20L;
			__BYTE	IF2MSK10H;
			__BYTE	IF2MSK10L;
		} byte;
		struct {
			__WORD	MXtd:1;
			__WORD	MDir:1;
			__WORD	RESV682:1;
			__WORD	Msk:29;
		} bit;
	} IO_IF2MSK0;
	union {
		__WORD	word;
		struct {
			__HWORD	IF2ARB20;
			__HWORD	IF2ARB10;
		} hword;
		struct {
			__BYTE	IF2ARB20H;
			__BYTE	IF2ARB20L;
			__BYTE	IF2ARB10H;
			__BYTE	IF2ARB10L;
		} byte;
		struct {
			__WORD	MsgVal:1;
			__WORD	Xtd:1;
			__WORD	Dir:1;
			__WORD	ID:29;
		} bit;
	} IO_IF2ARB0;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2MCTR0H;
			__BYTE	IF2MCTR0L;
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
	} IO_IF2MCTR0;
	__HWORD	IO_RESV4;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTA10H;
			__BYTE	IF2DTA10L;
		} byte;
	} IO_IF2DTA10;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTA20H;
			__BYTE	IF2DTA20L;
		} byte;
	} IO_IF2DTA20;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTB10H;
			__BYTE	IF2DTB10L;
		} byte;
	} IO_IF2DTB10;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTB20H;
			__BYTE	IF2DTB20L;
		} byte;
	} IO_IF2DTB20;
	__WORD	IO_RESV5[CONSTANT_10];
	const union {
		__WORD	word;
		struct {
			__HWORD	TREQR20;
			__HWORD	TREQR10;
		} hword;
		struct {
			__BYTE	TREQR20H;
			__BYTE	TREQR20L;
			__BYTE	TREQR10H;
			__BYTE	TREQR10L;
		} byte;
	} IO_TREQR0_12;
	const union {
		__WORD	word;
		struct {
			__HWORD	TREQR40;
			__HWORD	TREQR30;
		} hword;
		struct {
			__BYTE	TREQR40H;
			__BYTE	TREQR40L;
			__BYTE	TREQR30H;
			__BYTE	TREQR30L;
		} byte;
	} IO_TREQR0_34;
	__WORD	IO_RESV6;
	__WORD	IO_RESV7;
	const union {
		__WORD	word;
		struct {
			__HWORD	NEWDT20;
			__HWORD	NEWDT10;
		} hword;
		struct {
			__BYTE	NEWDT20H;
			__BYTE	NEWDT20L;
			__BYTE	NEWDT10H;
			__BYTE	NEWDT10L;
		} byte;
	} IO_NEWDT0_12;
	const union {
		__WORD	word;
		struct {
			__HWORD	NEWDT40;
			__HWORD	NEWDT30;
		} hword;
		struct {
			__BYTE	NEWDT40H;
			__BYTE	NEWDT40L;
			__BYTE	NEWDT30H;
			__BYTE	NEWDT30L;
		} byte;
	} IO_NEWDT0_34;
	__WORD	IO_RESV8;
	__WORD	IO_RESV9;
	const union {
		__WORD	word;
		struct {
			__HWORD	INTPND20;
			__HWORD	INTPND10;
		} hword;
		struct {
			__BYTE	INTPND20H;
			__BYTE	INTPND20L;
			__BYTE	INTPND10H;
			__BYTE	INTPND10L;
		} byte;
	} IO_INTPND0_12;
	const union {
		__WORD	word;
		struct {
			__HWORD	INTPND40;
			__HWORD	INTPND30;
		} hword;
		struct {
			__BYTE	INTPND40H;
			__BYTE	INTPND40L;
			__BYTE	INTPND30H;
			__BYTE	INTPND30L;
		} byte;
	} IO_INTPND0_34;
	__WORD	IO_RESV10;
	__WORD	IO_RESV11;
	const union {
		__WORD	word;
		struct {
			__HWORD	MSGVAL20;
			__HWORD	MSGVAL10;
		} hword;
		struct {
			__BYTE	MSGVAL20H;
			__BYTE	MSGVAL20L;
			__BYTE	MSGVAL10H;
			__BYTE	MSGVAL10L;
		} byte;
	} IO_MSGVAL0_12;
	const union {
		__WORD	word;
		struct {
			__HWORD	MSGVAL40;
			__HWORD	MSGVAL30;
		} hword;
		struct {
			__BYTE	MSGVAL40H;
			__BYTE	MSGVAL40L;
			__BYTE	MSGVAL30H;
			__BYTE	MSGVAL30L;
		} byte;
	} IO_MSGVAL0_34;
	__WORD	IO_RESV12;
	__WORD	IO_RESV13;
} IO_CAN0;

#define	CAN0_CTRLR0			(IO_CAN0.IO_CTRLR0.hword)
#define	CAN0_CTRLR0_CTRLR0H		(IO_CAN0.IO_CTRLR0.byte.CTRLR0H)
#define	CAN0_CTRLR0_CTRLR0L		(IO_CAN0.IO_CTRLR0.byte.CTRLR0L)
#define	CAN0_CTRLR0_Test    	(IO_CAN0.IO_CTRLR0.bit.Test)
#define	CAN0_CTRLR0_CCE    	(IO_CAN0.IO_CTRLR0.bit.CCE)
#define	CAN0_CTRLR0_DAR    	(IO_CAN0.IO_CTRLR0.bit.DAR)
#define	CAN0_CTRLR0_EIE    	(IO_CAN0.IO_CTRLR0.bit.EIE)
#define	CAN0_CTRLR0_SIE    	(IO_CAN0.IO_CTRLR0.bit.SIE)
#define	CAN0_CTRLR0_IE    	(IO_CAN0.IO_CTRLR0.bit.IE)
#define	CAN0_CTRLR0_Init    	(IO_CAN0.IO_CTRLR0.bit.Init)
#define	CAN0_STATR0			(IO_CAN0.IO_STATR0.hword)
#define	CAN0_STATR0_STATR0H		(IO_CAN0.IO_STATR0.byte.STATR0H)
#define	CAN0_STATR0_STATR0L		(IO_CAN0.IO_STATR0.byte.STATR0L)
#define	CAN0_STATR0_BOff    	(IO_CAN0.IO_STATR0.bit.BOff)
#define	CAN0_STATR0_EWarn    	(IO_CAN0.IO_STATR0.bit.EWarn)
#define	CAN0_STATR0_EPass    	(IO_CAN0.IO_STATR0.bit.EPass)
#define	CAN0_STATR0_RxOk    	(IO_CAN0.IO_STATR0.bit.RxOk)
#define	CAN0_STATR0_TxOk    	(IO_CAN0.IO_STATR0.bit.TxOk)
#define	CAN0_STATR0_LEC    	(IO_CAN0.IO_STATR0.bit.LEC)
#define	CAN0_ERRCNT0			(IO_CAN0.IO_ERRCNT0.hword)
#define	CAN0_ERRCNT0_ERRCNT0H	(IO_CAN0.IO_ERRCNT0.byte.ERRCNT0H)
#define	CAN0_ERRCNT0_ERRCNT0L	(IO_CAN0.IO_ERRCNT0.byte.ERRCNT0L)
#define	CAN0_ERRCNT0_RP    	(IO_CAN0.IO_ERRCNT0.bit.RP)
#define	CAN0_ERRCNT0_REC    	(IO_CAN0.IO_ERRCNT0.bit.REC)
#define	CAN0_ERRCNT0_TEC    	(IO_CAN0.IO_ERRCNT0.bit.TEC)
#define	CAN0_BTR0			(IO_CAN0.IO_BTR0.hword)
#define	CAN0_BTR0_BTR0H	(IO_CAN0.IO_BTR0.byte.BTR0H)
#define	CAN0_BTR0_BTR0L	(IO_CAN0.IO_BTR0.byte.BTR0L)
#define	CAN0_BTR0_TSeg2    	(IO_CAN0.IO_BTR0.bit.TSeg2)
#define	CAN0_BTR0_TSeg1    	(IO_CAN0.IO_BTR0.bit.TSeg1)
#define	CAN0_BTR0_SJW    	(IO_CAN0.IO_BTR0.bit.SJW)
#define	CAN0_BTR0_BRP    	(IO_CAN0.IO_BTR0.bit.BRP)
#define	CAN0_INTR0			(IO_CAN0.IO_INTR0.hword)
#define	CAN0_INTR0_INTR0H	(IO_CAN0.IO_INTR0.byte.INTR0H)
#define	CAN0_INTR0_INTR0L	(IO_CAN0.IO_INTR0.byte.INTR0L)
#define	CAN0_TESTR0			(IO_CAN0.IO_TESTR0.hword)
#define	CAN0_TESTR0_TESTR0H	(IO_CAN0.IO_TESTR0.byte.TESTR0H)
#define	CAN0_TESTR0_TESTR0L	(IO_CAN0.IO_TESTR0.byte.TESTR0L)
#define	CAN0_TESTR0_Rx    	(IO_CAN0.IO_TESTR0.bit.Rx)
#define	CAN0_TESTR0_Tx    	(IO_CAN0.IO_TESTR0.bit.Tx)
#define	CAN0_TESTR0_LBack    	(IO_CAN0.IO_TESTR0.bit.LBack)
#define	CAN0_TESTR0_Silent    	(IO_CAN0.IO_TESTR0.bit.Silent)
#define	CAN0_TESTR0_Basic    	(IO_CAN0.IO_TESTR0.bit.Basic)
#define	CAN0_BRPER0			(IO_CAN0.IO_BRPER0.hword)
#define	CAN0_BRPER0_BRPER0H	(IO_CAN0.IO_BRPER0.byte.BRPER0H)
#define	CAN0_BRPER0_BRPER0L	(IO_CAN0.IO_BRPER0.byte.BRPER0L)
#define	CAN0_BRPER0_BRPE    	(IO_CAN0.IO_BRPER0.bit.BRPE)
#define	CAN0_IF1CREQ0			(IO_CAN0.IO_IF1CREQ0.hword)
#define	CAN0_IF1CREQ0_IF1CREQ0H		(IO_CAN0.IO_IF1CREQ0.byte.IF1CREQ0H)
#define	CAN0_IF1CREQ0_IF1CREQ0L		(IO_CAN0.IO_IF1CREQ0.byte.IF1CREQ0L)
#define	CAN0_IF1CREQ0_BUSY    	(IO_CAN0.IO_IF1CREQ0.bit.BUSY)
#define	CAN0_IF1CREQ0_MN    	(IO_CAN0.IO_IF1CREQ0.bit.MN)
#define	CAN0_IF1CMSK0			(IO_CAN0.IO_IF1CMSK0.hword)
#define	CAN0_IF1CMSK0_IF1CMSK0H		(IO_CAN0.IO_IF1CMSK0.byte.IF1CMSK0H)
#define	CAN0_IF1CMSK0_IF1CMSK0L		(IO_CAN0.IO_IF1CMSK0.byte.IF1CMSK0L)
#define	CAN0_IF1CMSK0_WR_RD    	(IO_CAN0.IO_IF1CMSK0.bit.WR_RD)
#define	CAN0_IF1CMSK0_Mask    	(IO_CAN0.IO_IF1CMSK0.bit.Mask)
#define	CAN0_IF1CMSK0_Arb    	(IO_CAN0.IO_IF1CMSK0.bit.Arb)
#define	CAN0_IF1CMSK0_Control    	(IO_CAN0.IO_IF1CMSK0.bit.Control)
#define	CAN0_IF1CMSK0_CIP    	(IO_CAN0.IO_IF1CMSK0.bit.CIP)
#define	CAN0_IF1CMSK0_TxReq    	(IO_CAN0.IO_IF1CMSK0.bit.TxReq)
#define	CAN0_IF1CMSK0_DataA    	(IO_CAN0.IO_IF1CMSK0.bit.DataA)
#define	CAN0_IF1CMSK0_DataB    	(IO_CAN0.IO_IF1CMSK0.bit.DataB)
#define	CAN0_IF1MSK0			(IO_CAN0.IO_IF1MSK0.word)
#define	CAN0_IF1MSK0_IF1MSK20		(IO_CAN0.IO_IF1MSK0.hword.IF1MSK20)
#define	CAN0_IF1MSK0_IF1MSK10		(IO_CAN0.IO_IF1MSK0.hword.IF1MSK10)
#define	CAN0_IF1MSK0_IF1MSK20H		(IO_CAN0.IO_IF1MSK0.byte.IF1MSK20H)
#define	CAN0_IF1MSK0_IF1MSK20L		(IO_CAN0.IO_IF1MSK0.byte.IF1MSK20L)
#define	CAN0_IF1MSK0_IF1MSK10H		(IO_CAN0.IO_IF1MSK0.byte.IF1MSK10H)
#define	CAN0_IF1MSK0_IF1MSK10L		(IO_CAN0.IO_IF1MSK0.byte.IF1MSK10L)
#define	CAN0_IF1MSK0_MXtd    	(IO_CAN0.IO_IF1MSK0.bit.MXtd)
#define	CAN0_IF1MSK0_MDir    	(IO_CAN0.IO_IF1MSK0.bit.MDir)
#define	CAN0_IF1MSK0_Msk    	(IO_CAN0.IO_IF1MSK0.bit.Msk)
#define	CAN0_IF1ARB0			(IO_CAN0.IO_IF1ARB0.word)
#define	CAN0_IF1ARB0_IF1ARB20		(IO_CAN0.IO_IF1ARB0.hword.IF1ARB20)
#define	CAN0_IF1ARB0_IF1ARB10		(IO_CAN0.IO_IF1ARB0.hword.IF1ARB10)
#define	CAN0_IF1ARB0_IF1ARB20H		(IO_CAN0.IO_IF1ARB0.byte.IF1ARB20H)
#define	CAN0_IF1ARB0_IF1ARB20L		(IO_CAN0.IO_IF1ARB0.byte.IF1ARB20L)
#define	CAN0_IF1ARB0_IF1ARB10H		(IO_CAN0.IO_IF1ARB0.byte.IF1ARB10H)
#define	CAN0_IF1ARB0_IF1ARB10L		(IO_CAN0.IO_IF1ARB0.byte.IF1ARB10L)
#define	CAN0_IF1ARB0_MsgVal    	(IO_CAN0.IO_IF1ARB0.bit.MsgVal)
#define	CAN0_IF1ARB0_Xtd    	(IO_CAN0.IO_IF1ARB0.bit.Xtd)
#define	CAN0_IF1ARB0_Dir    	(IO_CAN0.IO_IF1ARB0.bit.Dir)
#define	CAN0_IF1ARB0_ID    	(IO_CAN0.IO_IF1ARB0.bit.ID)
#define	CAN0_IF1MCTR0			(IO_CAN0.IO_IF1MCTR0.hword)
#define	CAN0_IF1MCTR0_IF1MCTR0H		(IO_CAN0.IO_IF1MCTR0.byte.IF1MCTR0H)
#define	CAN0_IF1MCTR0_IF1MCTR0L		(IO_CAN0.IO_IF1MCTR0.byte.IF1MCTR0L)
#define	CAN0_IF1MCTR0_NewDat    	(IO_CAN0.IO_IF1MCTR0.bit.NewDat)
#define	CAN0_IF1MCTR0_MsgLst    	(IO_CAN0.IO_IF1MCTR0.bit.MsgLst)
#define	CAN0_IF1MCTR0_IntPnd    	(IO_CAN0.IO_IF1MCTR0.bit.IntPnd)
#define	CAN0_IF1MCTR0_UMask    	(IO_CAN0.IO_IF1MCTR0.bit.UMask)
#define	CAN0_IF1MCTR0_TxIE    	(IO_CAN0.IO_IF1MCTR0.bit.TxIE)
#define	CAN0_IF1MCTR0_RxIE    	(IO_CAN0.IO_IF1MCTR0.bit.RxIE)
#define	CAN0_IF1MCTR0_RmtEn    	(IO_CAN0.IO_IF1MCTR0.bit.RmtEn)
#define	CAN0_IF1MCTR0_TxRqst    	(IO_CAN0.IO_IF1MCTR0.bit.TxRqst)
#define	CAN0_IF1MCTR0_EoB    	(IO_CAN0.IO_IF1MCTR0.bit.EoB)
#define	CAN0_IF1MCTR0_DLC    	(IO_CAN0.IO_IF1MCTR0.bit.DLC)
#define	CAN0_IF1DTA10	(IO_CAN0.IO_IF1DTA10.hword)
#define	CAN0_IF1DTA10_IF1DTA10H	(IO_CAN0.IO_IF1DTA10.byte.IF1DTA10H)
#define	CAN0_IF1DTA10_IF1DTA10L	(IO_CAN0.IO_IF1DTA10.byte.IF1DTA10L)
#define	CAN0_IF1DTA20	(IO_CAN0.IO_IF1DTA20.hword)
#define	CAN0_IF1DTA20_IF1DTA20H	(IO_CAN0.IO_IF1DTA20.byte.IF1DTA20H)
#define	CAN0_IF1DTA20_IF1DTA20L	(IO_CAN0.IO_IF1DTA20.byte.IF1DTA20L)
#define	CAN0_IF1DTB10	(IO_CAN0.IO_IF1DTB10.hword)
#define	CAN0_IF1DTB10_IF1DTB10H	(IO_CAN0.IO_IF1DTB10.byte.IF1DTB10H)
#define	CAN0_IF1DTB10_IF1DTB10L	(IO_CAN0.IO_IF1DTB10.byte.IF1DTB10L)
#define	CAN0_IF1DTB20	(IO_CAN0.IO_IF1DTB20.hword)
#define	CAN0_IF1DTB20_IF1DTB20H	(IO_CAN0.IO_IF1DTB20.byte.IF1DTB20H)
#define	CAN0_IF1DTB20_IF1DTB20L	(IO_CAN0.IO_IF1DTB20.byte.IF1DTB20L)
#define	CAN0_RESV3_0  	(IO_CAN0.RESV3[CONSTANT_0])
#define	CAN0_RESV3_1  	(IO_CAN0.RESV3[CONSTANT_1])
#define	CAN0_RESV3_2  	(IO_CAN0.RESV3[CONSTANT_2])
#define	CAN0_RESV3_3  	(IO_CAN0.RESV3[CONSTANT_3])
#define	CAN0_RESV3_4  	(IO_CAN0.RESV3[CONSTANT_4])
#define	CAN0_RESV3_5  	(IO_CAN0.RESV3[CONSTANT_5])
#define	CAN0_RESV3_6  	(IO_CAN0.RESV3[CONSTANT_6])
#define	CAN0_RESV3_7  	(IO_CAN0.RESV3[CONSTANT_7])
#define	CAN0_RESV3_8  	(IO_CAN0.RESV3[CONSTANT_8])
#define	CAN0_RESV3_9  	(IO_CAN0.RESV3[CONSTANT_9])
#define	CAN0_RESV3_10  	(IO_CAN0.RESV3[CONSTANT_10])
#define	CAN0_RESV3_11  	(IO_CAN0.RESV3[CONSTANT_11])
#define	CAN0_RESV3_12  	(IO_CAN0.RESV3[CONSTANT_12])
#define	CAN0_RESV3_13  	(IO_CAN0.RESV3[CONSTANT_13])
#define	CAN0_RESV3_14  	(IO_CAN0.RESV3[CONSTANT_14])
#define	CAN0_RESV3_15  	(IO_CAN0.RESV3[CONSTANT_15])
#define	CAN0_RESV3_16  	(IO_CAN0.RESV3[CONSTANT_16])
#define	CAN0_RESV3_17  	(IO_CAN0.RESV3[CONSTANT_17])
#define	CAN0_RESV3_18  	(IO_CAN0.RESV3[CONSTANT_18])
#define	CAN0_RESV3_19  	(IO_CAN0.RESV3[CONSTANT_19])
#define	CAN0_RESV3_20  	(IO_CAN0.RESV3[CONSTANT_20])
#define	CAN0_RESV3_21  	(IO_CAN0.RESV3[CONSTANT_21])
#define	CAN0_RESV3_22  	(IO_CAN0.RESV3[CONSTANT_22])
#define	CAN0_RESV3_23  	(IO_CAN0.RESV3[CONSTANT_23])
#define	CAN0_IF2CREQ0			(IO_CAN0.IO_IF2CREQ0.hword)
#define	CAN0_IF2CREQ0_IF2CREQ0H		(IO_CAN0.IO_IF2CREQ0.byte.IF2CREQ0H)
#define	CAN0_IF2CREQ0_IF2CREQ0L		(IO_CAN0.IO_IF2CREQ0.byte.IF2CREQ0L)
#define	CAN0_IF2CREQ0_BUSY    	(IO_CAN0.IO_IF2CREQ0.bit.BUSY)
#define	CAN0_IF2CREQ0_MN    	(IO_CAN0.IO_IF2CREQ0.bit.MN)
#define	CAN0_IF2CMSK0			(IO_CAN0.IO_IF2CMSK0.hword)
#define	CAN0_IF2CMSK0_IF2CMSK0H		(IO_CAN0.IO_IF2CMSK0.byte.IF2CMSK0H)
#define	CAN0_IF2CMSK0_IF2CMSK0L		(IO_CAN0.IO_IF2CMSK0.byte.IF2CMSK0L)
#define	CAN0_IF2CMSK0_WR_RD    	(IO_CAN0.IO_IF2CMSK0.bit.WR_RD)
#define	CAN0_IF2CMSK0_Mask    	(IO_CAN0.IO_IF2CMSK0.bit.Mask)
#define	CAN0_IF2CMSK0_Arb    	(IO_CAN0.IO_IF2CMSK0.bit.Arb)
#define	CAN0_IF2CMSK0_Control    	(IO_CAN0.IO_IF2CMSK0.bit.Control)
#define	CAN0_IF2CMSK0_CIP    	(IO_CAN0.IO_IF2CMSK0.bit.CIP)
#define	CAN0_IF2CMSK0_TxReq    	(IO_CAN0.IO_IF2CMSK0.bit.TxReq)
#define	CAN0_IF2CMSK0_DataA    	(IO_CAN0.IO_IF2CMSK0.bit.DataA)
#define	CAN0_IF2CMSK0_DataB    	(IO_CAN0.IO_IF2CMSK0.bit.DataB)
#define	CAN0_IF2MSK0			(IO_CAN0.IO_IF2MSK0.word)
#define	CAN0_IF2MSK0_IF2MSK20		(IO_CAN0.IO_IF2MSK0.hword.IF2MSK20)
#define	CAN0_IF2MSK0_IF2MSK10		(IO_CAN0.IO_IF2MSK0.hword.IF2MSK10)
#define	CAN0_IF2MSK0_IF2MSK20H		(IO_CAN0.IO_IF2MSK0.byte.IF2MSK20H)
#define	CAN0_IF2MSK0_IF2MSK20L		(IO_CAN0.IO_IF2MSK0.byte.IF2MSK20L)
#define	CAN0_IF2MSK0_IF2MSK10H		(IO_CAN0.IO_IF2MSK0.byte.IF2MSK10H)
#define	CAN0_IF2MSK0_IF2MSK10L		(IO_CAN0.IO_IF2MSK0.byte.IF2MSK10L)
#define	CAN0_IF2MSK0_MXtd    	(IO_CAN0.IO_IF2MSK0.bit.MXtd)
#define	CAN0_IF2MSK0_MDir    	(IO_CAN0.IO_IF2MSK0.bit.MDir)
#define	CAN0_IF2MSK0_Msk    	(IO_CAN0.IO_IF2MSK0.bit.Msk)
#define	CAN0_IF2ARB0			(IO_CAN0.IO_IF2ARB0.word)
#define	CAN0_IF2ARB0_IF2ARB20		(IO_CAN0.IO_IF2ARB0.hword.IF2ARB20)
#define	CAN0_IF2ARB0_IF2ARB10		(IO_CAN0.IO_IF2ARB0.hword.IF2ARB10)
#define	CAN0_IF2ARB0_IF2ARB20H		(IO_CAN0.IO_IF2ARB0.byte.IF2ARB20H)
#define	CAN0_IF2ARB0_IF2ARB20L		(IO_CAN0.IO_IF2ARB0.byte.IF2ARB20L)
#define	CAN0_IF2ARB0_IF2ARB10H		(IO_CAN0.IO_IF2ARB0.byte.IF2ARB10H)
#define	CAN0_IF2ARB0_IF2ARB10L		(IO_CAN0.IO_IF2ARB0.byte.IF2ARB10L)
#define	CAN0_IF2ARB0_MsgVal    	(IO_CAN0.IO_IF2ARB0.bit.MsgVal)
#define	CAN0_IF2ARB0_Xtd    	(IO_CAN0.IO_IF2ARB0.bit.Xtd)
#define	CAN0_IF2ARB0_Dir    	(IO_CAN0.IO_IF2ARB0.bit.Dir)
#define	CAN0_IF2ARB0_ID    	(IO_CAN0.IO_IF2ARB0.bit.ID)
#define	CAN0_IF2MCTR0			(IO_CAN0.IO_IF2MCTR0.hword)
#define	CAN0_IF2MCTR0_IF2MCTR0H		(IO_CAN0.IO_IF2MCTR0.byte.IF2MCTR0H)
#define	CAN0_IF2MCTR0_IF2MCTR0L		(IO_CAN0.IO_IF2MCTR0.byte.IF2MCTR0L)
#define	CAN0_IF2MCTR0_NewDat    	(IO_CAN0.IO_IF2MCTR0.bit.NewDat)
#define	CAN0_IF2MCTR0_MsgLst    	(IO_CAN0.IO_IF2MCTR0.bit.MsgLst)
#define	CAN0_IF2MCTR0_IntPnd    	(IO_CAN0.IO_IF2MCTR0.bit.IntPnd)
#define	CAN0_IF2MCTR0_UMask    	(IO_CAN0.IO_IF2MCTR0.bit.UMask)
#define	CAN0_IF2MCTR0_TxIE    	(IO_CAN0.IO_IF2MCTR0.bit.TxIE)
#define	CAN0_IF2MCTR0_RxIE    	(IO_CAN0.IO_IF2MCTR0.bit.RxIE)
#define	CAN0_IF2MCTR0_RmtEn    	(IO_CAN0.IO_IF2MCTR0.bit.RmtEn)
#define	CAN0_IF2MCTR0_TxRqst    	(IO_CAN0.IO_IF2MCTR0.bit.TxRqst)
#define	CAN0_IF2MCTR0_EoB    	(IO_CAN0.IO_IF2MCTR0.bit.EoB)
#define	CAN0_IF2MCTR0_DLC    	(IO_CAN0.IO_IF2MCTR0.bit.DLC)
#define	CAN0_IF2DTA10	(IO_CAN0.IO_IF2DTA10.hword)
#define	CAN0_IF2DTA10_IF2DTA10H	(IO_CAN0.IO_IF2DTA10.byte.IF2DTA10H)
#define	CAN0_IF2DTA10_IF2DTA10L	(IO_CAN0.IO_IF2DTA10.byte.IF2DTA10L)
#define	CAN0_IF2DTA20	(IO_CAN0.IO_IF2DTA20.hword)
#define	CAN0_IF2DTA20_IF2DTA20H	(IO_CAN0.IO_IF2DTA20.byte.IF2DTA20H)
#define	CAN0_IF2DTA20_IF2DTA20L	(IO_CAN0.IO_IF2DTA20.byte.IF2DTA20L)
#define	CAN0_IF2DTB10	(IO_CAN0.IO_IF2DTB10.hword)
#define	CAN0_IF2DTB10_IF2DTB10H	(IO_CAN0.IO_IF2DTB10.byte.IF2DTB10H)
#define	CAN0_IF2DTB10_IF2DTB10L	(IO_CAN0.IO_IF2DTB10.byte.IF2DTB10L)
#define	CAN0_IF2DTB20	(IO_CAN0.IO_IF2DTB20.hword)
#define	CAN0_IF2DTB20_IF2DTB20H	(IO_CAN0.IO_IF2DTB20.byte.IF2DTB20H)
#define	CAN0_IF2DTB20_IF2DTB20L	(IO_CAN0.IO_IF2DTB20.byte.IF2DTB20L)
#define	CAN0_RESV5_0  	(IO_CAN0.RESV5[CONSTANT_0])
#define	CAN0_RESV5_1  	(IO_CAN0.RESV5[CONSTANT_1])
#define	CAN0_RESV5_2  	(IO_CAN0.RESV5[CONSTANT_2])
#define	CAN0_RESV5_3  	(IO_CAN0.RESV5[CONSTANT_3])
#define	CAN0_RESV5_4  	(IO_CAN0.RESV5[CONSTANT_4])
#define	CAN0_RESV5_5  	(IO_CAN0.RESV5[CONSTANT_5])
#define	CAN0_RESV5_6  	(IO_CAN0.RESV5[CONSTANT_6])
#define	CAN0_RESV5_7  	(IO_CAN0.RESV5[CONSTANT_7])
#define	CAN0_RESV5_8  	(IO_CAN0.RESV5[CONSTANT_8])
#define	CAN0_RESV5_9  	(IO_CAN0.RESV5[CONSTANT_9])
#define	CAN0_RESV5_10  	(IO_CAN0.RESV5[CONSTANT_10])
#define	CAN0_RESV5_11  	(IO_CAN0.RESV5[CONSTANT_11])
#define	CAN0_RESV5_12  	(IO_CAN0.RESV5[CONSTANT_12])
#define	CAN0_RESV5_13  	(IO_CAN0.RESV5[CONSTANT_13])
#define	CAN0_RESV5_14  	(IO_CAN0.RESV5[CONSTANT_14])
#define	CAN0_RESV5_15  	(IO_CAN0.RESV5[CONSTANT_15])
#define	CAN0_RESV5_16  	(IO_CAN0.RESV5[CONSTANT_16])
#define	CAN0_RESV5_17  	(IO_CAN0.RESV5[CONSTANT_17])
#define	CAN0_RESV5_18  	(IO_CAN0.RESV5[CONSTANT_18])
#define	CAN0_RESV5_19  	(IO_CAN0.RESV5[CONSTANT_19])
#define	CAN0_RESV5_20  	(IO_CAN0.RESV5[CONSTANT_20])
#define	CAN0_RESV5_21  	(IO_CAN0.RESV5[CONSTANT_21])
#define	CAN0_RESV5_22  	(IO_CAN0.RESV5[CONSTANT_22])
#define	CAN0_RESV5_23  	(IO_CAN0.RESV5[CONSTANT_23])
#define	CAN0_RESV5_24  	(IO_CAN0.RESV5[CONSTANT_24])
#define	CAN0_RESV5_25  	(IO_CAN0.RESV5[CONSTANT_25])
#define	CAN0_RESV5_26  	(IO_CAN0.RESV5[CONSTANT_26])
#define	CAN0_RESV5_27  	(IO_CAN0.RESV5[CONSTANT_27])
#define	CAN0_RESV5_28  	(IO_CAN0.RESV5[CONSTANT_28])
#define	CAN0_RESV5_29  	(IO_CAN0.RESV5[CONSTANT_29])
#define	CAN0_RESV5_30  	(IO_CAN0.RESV5[CONSTANT_30])
#define	CAN0_RESV5_31  	(IO_CAN0.RESV5[CONSTANT_31])
#define	CAN0_RESV5_32  	(IO_CAN0.RESV5[CONSTANT_32])
#define	CAN0_RESV5_33  	(IO_CAN0.RESV5[CONSTANT_33])
#define	CAN0_RESV5_34  	(IO_CAN0.RESV5[CONSTANT_34])
#define	CAN0_RESV5_35  	(IO_CAN0.RESV5[CONSTANT_35])
#define	CAN0_RESV5_36  	(IO_CAN0.RESV5[CONSTANT_36])
#define	CAN0_RESV5_37  	(IO_CAN0.RESV5[CONSTANT_37])
#define	CAN0_RESV5_38  	(IO_CAN0.RESV5[CONSTANT_38])
#define	CAN0_RESV5_39  	(IO_CAN0.RESV5[CONSTANT_39])
#define	CAN0_TREQR0_12			(IO_CAN0.IO_TREQR0_12.word)
#define	CAN0_TREQR0_12_TREQR20		(IO_CAN0.IO_TREQR0_12.hword.TREQR20)
#define	CAN0_TREQR0_12_TREQR10		(IO_CAN0.IO_TREQR0_12.hword.TREQR10)
#define	CAN0_TREQR0_12_TREQR20H		(IO_CAN0.IO_TREQR0_12.byte.TREQR20H)
#define	CAN0_TREQR0_12_TREQR20L		(IO_CAN0.IO_TREQR0_12.byte.TREQR20L)
#define	CAN0_TREQR0_12_TREQR10H		(IO_CAN0.IO_TREQR0_12.byte.TREQR10H)
#define	CAN0_TREQR0_12_TREQR10L		(IO_CAN0.IO_TREQR0_12.byte.TREQR10L)
#define	CAN0_TREQR0_34			(IO_CAN0.IO_TREQR0_34.word)
#define	CAN0_TREQR0_34_TREQR40		(IO_CAN0.IO_TREQR0_34.hword.TREQR40)
#define	CAN0_TREQR0_34_TREQR30		(IO_CAN0.IO_TREQR0_34.hword.TREQR30)
#define	CAN0_TREQR0_34_TREQR40H		(IO_CAN0.IO_TREQR0_34.byte.TREQR40H)
#define	CAN0_TREQR0_34_TREQR40L		(IO_CAN0.IO_TREQR0_34.byte.TREQR40L)
#define	CAN0_TREQR0_34_TREQR30H		(IO_CAN0.IO_TREQR0_34.byte.TREQR30H)
#define	CAN0_TREQR0_34_TREQR30L		(IO_CAN0.IO_TREQR0_34.byte.TREQR30L)
#define	CAN0_NEWDT0_12			(IO_CAN0.IO_NEWDT0_12.word)
#define	CAN0_NEWDT0_12_NEWDT20		(IO_CAN0.IO_NEWDT0_12.hword.NEWDT20)
#define	CAN0_NEWDT0_12_NEWDT10		(IO_CAN0.IO_NEWDT0_12.hword.NEWDT10)
#define	CAN0_NEWDT0_12_NEWDT20H		(IO_CAN0.IO_NEWDT0_12.byte.NEWDT20H)
#define	CAN0_NEWDT0_12_NEWDT20L		(IO_CAN0.IO_NEWDT0_12.byte.NEWDT20L)
#define	CAN0_NEWDT0_12_NEWDT10H		(IO_CAN0.IO_NEWDT0_12.byte.NEWDT10H)
#define	CAN0_NEWDT0_12_NEWDT10L		(IO_CAN0.IO_NEWDT0_12.byte.NEWDT10L)
#define	CAN0_NEWDT0_34			(IO_CAN0.IO_NEWDT0_34.word)
#define	CAN0_NEWDT0_34_NEWDT40		(IO_CAN0.IO_NEWDT0_34.hword.NEWDT40)
#define	CAN0_NEWDT0_34_NEWDT30		(IO_CAN0.IO_NEWDT0_34.hword.NEWDT30)
#define	CAN0_NEWDT0_34_NEWDT40H		(IO_CAN0.IO_NEWDT0_34.byte.NEWDT40H)
#define	CAN0_NEWDT0_34_NEWDT40L		(IO_CAN0.IO_NEWDT0_34.byte.NEWDT40L)
#define	CAN0_NEWDT0_34_NEWDT30H		(IO_CAN0.IO_NEWDT0_34.byte.NEWDT30H)
#define	CAN0_NEWDT0_34_NEWDT30L		(IO_CAN0.IO_NEWDT0_34.byte.NEWDT30L)
#define	CAN0_INTPND0_12			(IO_CAN0.IO_INTPND0_12.word)
#define	CAN0_INTPND0_12_INTPND20		(IO_CAN0.IO_INTPND0_12.hword.INTPND20)
#define	CAN0_INTPND0_12_INTPND10		(IO_CAN0.IO_INTPND0_12.hword.INTPND10)
#define	CAN0_INTPND0_12_INTPND20H	(IO_CAN0.IO_INTPND0_12.byte.INTPND20H)
#define	CAN0_INTPND0_12_INTPND20L	(IO_CAN0.IO_INTPND0_12.byte.INTPND20L)
#define	CAN0_INTPND0_12_INTPND10H	(IO_CAN0.IO_INTPND0_12.byte.INTPND10H)
#define	CAN0_INTPND0_12_INTPND10L	(IO_CAN0.IO_INTPND0_12.byte.INTPND10L)
#define	CAN0_INTPND0_34			(IO_CAN0.IO_INTPND0_34.word)
#define	CAN0_INTPND0_34_INTPND40		(IO_CAN0.IO_INTPND0_34.hword.INTPND40)
#define	CAN0_INTPND0_34_INTPND30		(IO_CAN0.IO_INTPND0_34.hword.INTPND30)
#define	CAN0_INTPND0_34_INTPND40H	(IO_CAN0.IO_INTPND0_34.byte.INTPND40H)
#define	CAN0_INTPND0_34_INTPND40L	(IO_CAN0.IO_INTPND0_34.byte.INTPND40L)
#define	CAN0_INTPND0_34_INTPND30H	(IO_CAN0.IO_INTPND0_34.byte.INTPND30H)
#define	CAN0_INTPND0_34_INTPND30L	(IO_CAN0.IO_INTPND0_34.byte.INTPND30L)
#define	CAN0_MSGVAL0_12			(IO_CAN0.IO_MSGVAL0_12.word)
#define	CAN0_MSGVAL0_12_MSGVAL20		(IO_CAN0.IO_MSGVAL0_12.hword.MSGVAL20)
#define	CAN0_MSGVAL0_12_MSGVAL10		(IO_CAN0.IO_MSGVAL0_12.hword.MSGVAL10)
#define	CAN0_MSGVAL0_12_MSGVAL20H	(IO_CAN0.IO_MSGVAL0_12.byte.MSGVAL20H)
#define	CAN0_MSGVAL0_12_MSGVAL20L	(IO_CAN0.IO_MSGVAL0_12.byte.MSGVAL20L)
#define	CAN0_MSGVAL0_12_MSGVAL10H	(IO_CAN0.IO_MSGVAL0_12.byte.MSGVAL10H)
#define	CAN0_MSGVAL0_12_MSGVAL10L	(IO_CAN0.IO_MSGVAL0_12.byte.MSGVAL10L)
#define	CAN0_MSGVAL0_34			(IO_CAN0.IO_MSGVAL0_34.word)
#define	CAN0_MSGVAL0_34_MSGVAL40		(IO_CAN0.IO_MSGVAL0_34.hword.MSGVAL40)
#define	CAN0_MSGVAL0_34_MSGVAL30		(IO_CAN0.IO_MSGVAL0_34.hword.MSGVAL30)
#define	CAN0_MSGVAL0_34_MSGVAL40H	(IO_CAN0.IO_MSGVAL0_34.byte.MSGVAL40H)
#define	CAN0_MSGVAL0_34_MSGVAL40L	(IO_CAN0.IO_MSGVAL0_34.byte.MSGVAL40L)
#define	CAN0_MSGVAL0_34_MSGVAL30H	(IO_CAN0.IO_MSGVAL0_34.byte.MSGVAL30H)
#define	CAN0_MSGVAL0_34_MSGVAL30L	(IO_CAN0.IO_MSGVAL0_34.byte.MSGVAL30L)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CAN1,attr=IO,locate=0x2100
#endif

__IO_EXTENDED	struct {
	union {
		__HWORD	hword;
		struct {
			__BYTE	CTRLR1H;
			__BYTE	CTRLR1L;
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
	} IO_CTRLR1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	STATR1H;
			__BYTE	STATR1L;
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
	} IO_STATR1;
	const union {
		__HWORD	hword;
		struct {
			__BYTE	ERRCNT1H;
			__BYTE	ERRCNT1L;
		} byte;
		struct {
			__HWORD	RP:1;
			__HWORD	REC:7;
			__HWORD	TEC:8;
		} bit;
	} IO_ERRCNT1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	BTR1H;
			__BYTE	BTR1L;
		} byte;
		struct {
			__HWORD	RESV60:1;
			__HWORD	TSeg2:3;
			__HWORD	TSeg1:4;
			__HWORD	SJW:2;
			__HWORD	BRP:6;
		} bit;
	} IO_BTR1;
	const union {
		__HWORD	hword;
		struct {
			__BYTE	INTR1H;
			__BYTE	INTR1L;
		} byte;
	} IO_INTR1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	TESTR1H;
			__BYTE	TESTR1L;
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
	} IO_TESTR1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	BRPER1H;
			__BYTE	BRPER1L;
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
	} IO_BRPER1;
	__HWORD	IO_RESV1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1CREQ1H;
			__BYTE	IF1CREQ1L;
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
	} IO_IF1CREQ1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1CMSK1H;
			__BYTE	IF1CMSK1L;
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
	} IO_IF1CMSK1;
	union {
		__WORD	word;
		struct {
			__HWORD	IF1MSK21;
			__HWORD	IF1MSK11;
		} hword;
		struct {
			__BYTE	IF1MSK21H;
			__BYTE	IF1MSK21L;
			__BYTE	IF1MSK11H;
			__BYTE	IF1MSK11L;
		} byte;
		struct {
			__WORD	MXtd:1;
			__WORD	MDir:1;
			__WORD	RESV202:1;
			__WORD	Msk:29;
		} bit;
	} IO_IF1MSK1;
	union {
		__WORD	word;
		struct {
			__HWORD	IF1ARB21;
			__HWORD	IF1ARB11;
		} hword;
		struct {
			__BYTE	IF1ARB21H;
			__BYTE	IF1ARB21L;
			__BYTE	IF1ARB11H;
			__BYTE	IF1ARB11L;
		} byte;
		struct {
			__WORD	MsgVal:1;
			__WORD	Xtd:1;
			__WORD	Dir:1;
			__WORD	ID:29;
		} bit;
	} IO_IF1ARB1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1MCTR1H;
			__BYTE	IF1MCTR1L;
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
	} IO_IF1MCTR1;
	__HWORD	IO_RESV2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTA11H;
			__BYTE	IF1DTA11L;
		} byte;
	} IO_IF1DTA11;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTA21H;
			__BYTE	IF1DTA21L;
		} byte;
	} IO_IF1DTA21;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTB11H;
			__BYTE	IF1DTB11L;
		} byte;
	} IO_IF1DTB11;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTB21H;
			__BYTE	IF1DTB21L;
		} byte;
	} IO_IF1DTB21;
	__WORD	IO_RESV3[CONSTANT_6];
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2CREQ1H;
			__BYTE	IF2CREQ1L;
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
	} IO_IF2CREQ1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2CMSK1H;
			__BYTE	IF2CMSK1L;
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
	} IO_IF2CMSK1;
	union {
		__WORD	word;
		struct {
			__HWORD	IF2MSK21;
			__HWORD	IF2MSK11;
		} hword;
		struct {
			__BYTE	IF2MSK21H;
			__BYTE	IF2MSK21L;
			__BYTE	IF2MSK11H;
			__BYTE	IF2MSK11L;
		} byte;
		struct {
			__WORD	MXtd:1;
			__WORD	MDir:1;
			__WORD	RESV682:1;
			__WORD	Msk:29;
		} bit;
	} IO_IF2MSK1;
	union {
		__WORD	word;
		struct {
			__HWORD	IF2ARB21;
			__HWORD	IF2ARB11;
		} hword;
		struct {
			__BYTE	IF2ARB21H;
			__BYTE	IF2ARB21L;
			__BYTE	IF2ARB11H;
			__BYTE	IF2ARB11L;
		} byte;
		struct {
			__WORD	MsgVal:1;
			__WORD	Xtd:1;
			__WORD	Dir:1;
			__WORD	ID:29;
		} bit;
	} IO_IF2ARB1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2MCTR1H;
			__BYTE	IF2MCTR1L;
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
	} IO_IF2MCTR1;
	__HWORD	IO_RESV4;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTA11H;
			__BYTE	IF2DTA11L;
		} byte;
	} IO_IF2DTA11;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTA21H;
			__BYTE	IF2DTA21L;
		} byte;
	} IO_IF2DTA21;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTB11H;
			__BYTE	IF2DTB11L;
		} byte;
	} IO_IF2DTB11;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTB21H;
			__BYTE	IF2DTB21L;
		} byte;
	} IO_IF2DTB21;
	__WORD	IO_RESV5[CONSTANT_10];
	const union {
		__WORD	word;
		struct {
			__HWORD	TREQR21;
			__HWORD	TREQR11;
		} hword;
		struct {
			__BYTE	TREQR21H;
			__BYTE	TREQR21L;
			__BYTE	TREQR11H;
			__BYTE	TREQR11L;
		} byte;
	} IO_TREQR1_12;
	__WORD	IO_RESV6;
	__WORD	IO_RESV7;
	__WORD	IO_RESV8;
	const union {
		__WORD	word;
		struct {
			__HWORD	NEWDT21;
			__HWORD	NEWDT11;
		} hword;
		struct {
			__BYTE	NEWDT21H;
			__BYTE	NEWDT21L;
			__BYTE	NEWDT11H;
			__BYTE	NEWDT11L;
		} byte;
	} IO_NEWDT1_12;
	__WORD	IO_RESV9;
	__WORD	IO_RESV10;
	__WORD	IO_RESV11;
	const union {
		__WORD	word;
		struct {
			__HWORD	INTPND21;
			__HWORD	INTPND11;
		} hword;
		struct {
			__BYTE	INTPND21H;
			__BYTE	INTPND21L;
			__BYTE	INTPND11H;
			__BYTE	INTPND11L;
		} byte;
	} IO_INTPND1_12;
	__WORD	IO_RESV12;
	__WORD	IO_RESV13;
	__WORD	IO_RESV14;
	const union {
		__WORD	word;
		struct {
			__HWORD	MSGVAL21;
			__HWORD	MSGVAL11;
		} hword;
		struct {
			__BYTE	MSGVAL21H;
			__BYTE	MSGVAL21L;
			__BYTE	MSGVAL11H;
			__BYTE	MSGVAL11L;
		} byte;
	} IO_MSGVAL1_12;
	__WORD	IO_RESV15;
	__WORD	IO_RESV16;
	__WORD	IO_RESV17;
} IO_CAN1;

#define	CAN1_CTRLR1			(IO_CAN1.IO_CTRLR1.hword)
#define	CAN1_CTRLR1_CTRLR1H		(IO_CAN1.IO_CTRLR1.byte.CTRLR1H)
#define	CAN1_CTRLR1_CTRLR1L		(IO_CAN1.IO_CTRLR1.byte.CTRLR1L)
#define	CAN1_CTRLR1_Test    	(IO_CAN1.IO_CTRLR1.bit.Test)
#define	CAN1_CTRLR1_CCE    	(IO_CAN1.IO_CTRLR1.bit.CCE)
#define	CAN1_CTRLR1_DAR    	(IO_CAN1.IO_CTRLR1.bit.DAR)
#define	CAN1_CTRLR1_EIE    	(IO_CAN1.IO_CTRLR1.bit.EIE)
#define	CAN1_CTRLR1_SIE    	(IO_CAN1.IO_CTRLR1.bit.SIE)
#define	CAN1_CTRLR1_IE    	(IO_CAN1.IO_CTRLR1.bit.IE)
#define	CAN1_CTRLR1_Init    	(IO_CAN1.IO_CTRLR1.bit.Init)
#define	CAN1_STATR1			(IO_CAN1.IO_STATR1.hword)
#define	CAN1_STATR1_STATR1H		(IO_CAN1.IO_STATR1.byte.STATR1H)
#define	CAN1_STATR1_STATR1L		(IO_CAN1.IO_STATR1.byte.STATR1L)
#define	CAN1_STATR1_BOff    	(IO_CAN1.IO_STATR1.bit.BOff)
#define	CAN1_STATR1_EWarn    	(IO_CAN1.IO_STATR1.bit.EWarn)
#define	CAN1_STATR1_EPass    	(IO_CAN1.IO_STATR1.bit.EPass)
#define	CAN1_STATR1_RxOk    	(IO_CAN1.IO_STATR1.bit.RxOk)
#define	CAN1_STATR1_TxOk    	(IO_CAN1.IO_STATR1.bit.TxOk)
#define	CAN1_STATR1_LEC    	(IO_CAN1.IO_STATR1.bit.LEC)
#define	CAN1_ERRCNT1			(IO_CAN1.IO_ERRCNT1.hword)
#define	CAN1_ERRCNT1_ERRCNT1H	(IO_CAN1.IO_ERRCNT1.byte.ERRCNT1H)
#define	CAN1_ERRCNT1_ERRCNT1L	(IO_CAN1.IO_ERRCNT1.byte.ERRCNT1L)
#define	CAN1_ERRCNT1_RP    	(IO_CAN1.IO_ERRCNT1.bit.RP)
#define	CAN1_ERRCNT1_REC    	(IO_CAN1.IO_ERRCNT1.bit.REC)
#define	CAN1_ERRCNT1_TEC    	(IO_CAN1.IO_ERRCNT1.bit.TEC)
#define	CAN1_BTR1			(IO_CAN1.IO_BTR1.hword)
#define	CAN1_BTR1_BTR1H	(IO_CAN1.IO_BTR1.byte.BTR1H)
#define	CAN1_BTR1_BTR1L	(IO_CAN1.IO_BTR1.byte.BTR1L)
#define	CAN1_BTR1_TSeg2    	(IO_CAN1.IO_BTR1.bit.TSeg2)
#define	CAN1_BTR1_TSeg1    	(IO_CAN1.IO_BTR1.bit.TSeg1)
#define	CAN1_BTR1_SJW    	(IO_CAN1.IO_BTR1.bit.SJW)
#define	CAN1_BTR1_BRP    	(IO_CAN1.IO_BTR1.bit.BRP)
#define	CAN1_INTR1			(IO_CAN1.IO_INTR1.hword)
#define	CAN1_INTR1_INTR1H	(IO_CAN1.IO_INTR1.byte.INTR1H)
#define	CAN1_INTR1_INTR1L	(IO_CAN1.IO_INTR1.byte.INTR1L)
#define	CAN1_TESTR1			(IO_CAN1.IO_TESTR1.hword)
#define	CAN1_TESTR1_TESTR1H	(IO_CAN1.IO_TESTR1.byte.TESTR1H)
#define	CAN1_TESTR1_TESTR1L	(IO_CAN1.IO_TESTR1.byte.TESTR1L)
#define	CAN1_TESTR1_Rx    	(IO_CAN1.IO_TESTR1.bit.Rx)
#define	CAN1_TESTR1_Tx    	(IO_CAN1.IO_TESTR1.bit.Tx)
#define	CAN1_TESTR1_LBack    	(IO_CAN1.IO_TESTR1.bit.LBack)
#define	CAN1_TESTR1_Silent    	(IO_CAN1.IO_TESTR1.bit.Silent)
#define	CAN1_TESTR1_Basic    	(IO_CAN1.IO_TESTR1.bit.Basic)
#define	CAN1_BRPER1			(IO_CAN1.IO_BRPER1.hword)
#define	CAN1_BRPER1_BRPER1H	(IO_CAN1.IO_BRPER1.byte.BRPER1H)
#define	CAN1_BRPER1_BRPER1L	(IO_CAN1.IO_BRPER1.byte.BRPER1L)
#define	CAN1_BRPER1_BRPE    	(IO_CAN1.IO_BRPER1.bit.BRPE)
#define	CAN1_IF1CREQ1			(IO_CAN1.IO_IF1CREQ1.hword)
#define	CAN1_IF1CREQ1_IF1CREQ1H		(IO_CAN1.IO_IF1CREQ1.byte.IF1CREQ1H)
#define	CAN1_IF1CREQ1_IF1CREQ1L		(IO_CAN1.IO_IF1CREQ1.byte.IF1CREQ1L)
#define	CAN1_IF1CREQ1_BUSY    	(IO_CAN1.IO_IF1CREQ1.bit.BUSY)
#define	CAN1_IF1CREQ1_MN    	(IO_CAN1.IO_IF1CREQ1.bit.MN)
#define	CAN1_IF1CMSK1			(IO_CAN1.IO_IF1CMSK1.hword)
#define	CAN1_IF1CMSK1_IF1CMSK1H		(IO_CAN1.IO_IF1CMSK1.byte.IF1CMSK1H)
#define	CAN1_IF1CMSK1_IF1CMSK1L		(IO_CAN1.IO_IF1CMSK1.byte.IF1CMSK1L)
#define	CAN1_IF1CMSK1_WR_RD    	(IO_CAN1.IO_IF1CMSK1.bit.WR_RD)
#define	CAN1_IF1CMSK1_Mask    	(IO_CAN1.IO_IF1CMSK1.bit.Mask)
#define	CAN1_IF1CMSK1_Arb    	(IO_CAN1.IO_IF1CMSK1.bit.Arb)
#define	CAN1_IF1CMSK1_Control    	(IO_CAN1.IO_IF1CMSK1.bit.Control)
#define	CAN1_IF1CMSK1_CIP    	(IO_CAN1.IO_IF1CMSK1.bit.CIP)
#define	CAN1_IF1CMSK1_TxReq    	(IO_CAN1.IO_IF1CMSK1.bit.TxReq)
#define	CAN1_IF1CMSK1_DataA    	(IO_CAN1.IO_IF1CMSK1.bit.DataA)
#define	CAN1_IF1CMSK1_DataB    	(IO_CAN1.IO_IF1CMSK1.bit.DataB)
#define	CAN1_IF1MSK1			(IO_CAN1.IO_IF1MSK1.word)
#define	CAN1_IF1MSK1_IF1MSK21		(IO_CAN1.IO_IF1MSK1.hword.IF1MSK21)
#define	CAN1_IF1MSK1_IF1MSK11		(IO_CAN1.IO_IF1MSK1.hword.IF1MSK11)
#define	CAN1_IF1MSK1_IF1MSK21H		(IO_CAN1.IO_IF1MSK1.byte.IF1MSK21H)
#define	CAN1_IF1MSK1_IF1MSK21L		(IO_CAN1.IO_IF1MSK1.byte.IF1MSK21L)
#define	CAN1_IF1MSK1_IF1MSK11H		(IO_CAN1.IO_IF1MSK1.byte.IF1MSK11H)
#define	CAN1_IF1MSK1_IF1MSK11L		(IO_CAN1.IO_IF1MSK1.byte.IF1MSK11L)
#define	CAN1_IF1MSK1_MXtd    	(IO_CAN1.IO_IF1MSK1.bit.MXtd)
#define	CAN1_IF1MSK1_MDir    	(IO_CAN1.IO_IF1MSK1.bit.MDir)
#define	CAN1_IF1MSK1_Msk    	(IO_CAN1.IO_IF1MSK1.bit.Msk)
#define	CAN1_IF1ARB1			(IO_CAN1.IO_IF1ARB1.word)
#define	CAN1_IF1ARB1_IF1ARB21		(IO_CAN1.IO_IF1ARB1.hword.IF1ARB21)
#define	CAN1_IF1ARB1_IF1ARB11		(IO_CAN1.IO_IF1ARB1.hword.IF1ARB11)
#define	CAN1_IF1ARB1_IF1ARB21H		(IO_CAN1.IO_IF1ARB1.byte.IF1ARB21H)
#define	CAN1_IF1ARB1_IF1ARB21L		(IO_CAN1.IO_IF1ARB1.byte.IF1ARB21L)
#define	CAN1_IF1ARB1_IF1ARB11H		(IO_CAN1.IO_IF1ARB1.byte.IF1ARB11H)
#define	CAN1_IF1ARB1_IF1ARB11L		(IO_CAN1.IO_IF1ARB1.byte.IF1ARB11L)
#define	CAN1_IF1ARB1_MsgVal    	(IO_CAN1.IO_IF1ARB1.bit.MsgVal)
#define	CAN1_IF1ARB1_Xtd    	(IO_CAN1.IO_IF1ARB1.bit.Xtd)
#define	CAN1_IF1ARB1_Dir    	(IO_CAN1.IO_IF1ARB1.bit.Dir)
#define	CAN1_IF1ARB1_ID    	(IO_CAN1.IO_IF1ARB1.bit.ID)
#define	CAN1_IF1MCTR1			(IO_CAN1.IO_IF1MCTR1.hword)
#define	CAN1_IF1MCTR1_IF1MCTR1H		(IO_CAN1.IO_IF1MCTR1.byte.IF1MCTR1H)
#define	CAN1_IF1MCTR1_IF1MCTR1L		(IO_CAN1.IO_IF1MCTR1.byte.IF1MCTR1L)
#define	CAN1_IF1MCTR1_NewDat    	(IO_CAN1.IO_IF1MCTR1.bit.NewDat)
#define	CAN1_IF1MCTR1_MsgLst    	(IO_CAN1.IO_IF1MCTR1.bit.MsgLst)
#define	CAN1_IF1MCTR1_IntPnd    	(IO_CAN1.IO_IF1MCTR1.bit.IntPnd)
#define	CAN1_IF1MCTR1_UMask    	(IO_CAN1.IO_IF1MCTR1.bit.UMask)
#define	CAN1_IF1MCTR1_TxIE    	(IO_CAN1.IO_IF1MCTR1.bit.TxIE)
#define	CAN1_IF1MCTR1_RxIE    	(IO_CAN1.IO_IF1MCTR1.bit.RxIE)
#define	CAN1_IF1MCTR1_RmtEn    	(IO_CAN1.IO_IF1MCTR1.bit.RmtEn)
#define	CAN1_IF1MCTR1_TxRqst    	(IO_CAN1.IO_IF1MCTR1.bit.TxRqst)
#define	CAN1_IF1MCTR1_EoB    	(IO_CAN1.IO_IF1MCTR1.bit.EoB)
#define	CAN1_IF1MCTR1_DLC    	(IO_CAN1.IO_IF1MCTR1.bit.DLC)
#define	CAN1_IF1DTA11	(IO_CAN1.IO_IF1DTA11.hword)
#define	CAN1_IF1DTA11_IF1DTA11H	(IO_CAN1.IO_IF1DTA11.byte.IF1DTA11H)
#define	CAN1_IF1DTA11_IF1DTA11L	(IO_CAN1.IO_IF1DTA11.byte.IF1DTA11L)
#define	CAN1_IF1DTA21	(IO_CAN1.IO_IF1DTA21.hword)
#define	CAN1_IF1DTA21_IF1DTA21H	(IO_CAN1.IO_IF1DTA21.byte.IF1DTA21H)
#define	CAN1_IF1DTA21_IF1DTA21L	(IO_CAN1.IO_IF1DTA21.byte.IF1DTA21L)
#define	CAN1_IF1DTB11	(IO_CAN1.IO_IF1DTB11.hword)
#define	CAN1_IF1DTB11_IF1DTB11H	(IO_CAN1.IO_IF1DTB11.byte.IF1DTB11H)
#define	CAN1_IF1DTB11_IF1DTB11L	(IO_CAN1.IO_IF1DTB11.byte.IF1DTB11L)
#define	CAN1_IF1DTB21	(IO_CAN1.IO_IF1DTB21.hword)
#define	CAN1_IF1DTB21_IF1DTB21H	(IO_CAN1.IO_IF1DTB21.byte.IF1DTB21H)
#define	CAN1_IF1DTB21_IF1DTB21L	(IO_CAN1.IO_IF1DTB21.byte.IF1DTB21L)
#define	CAN1_RESV3_0  	(IO_CAN1.RESV3[CONSTANT_0])
#define	CAN1_RESV3_1  	(IO_CAN1.RESV3[CONSTANT_1])
#define	CAN1_RESV3_2  	(IO_CAN1.RESV3[CONSTANT_2])
#define	CAN1_RESV3_3  	(IO_CAN1.RESV3[CONSTANT_3])
#define	CAN1_RESV3_4  	(IO_CAN1.RESV3[CONSTANT_4])
#define	CAN1_RESV3_5  	(IO_CAN1.RESV3[CONSTANT_5])
#define	CAN1_RESV3_6  	(IO_CAN1.RESV3[CONSTANT_6])
#define	CAN1_RESV3_7  	(IO_CAN1.RESV3[CONSTANT_7])
#define	CAN1_RESV3_8  	(IO_CAN1.RESV3[CONSTANT_8])
#define	CAN1_RESV3_9  	(IO_CAN1.RESV3[CONSTANT_9])
#define	CAN1_RESV3_10  	(IO_CAN1.RESV3[CONSTANT_10])
#define	CAN1_RESV3_11  	(IO_CAN1.RESV3[CONSTANT_11])
#define	CAN1_RESV3_12  	(IO_CAN1.RESV3[CONSTANT_12])
#define	CAN1_RESV3_13  	(IO_CAN1.RESV3[CONSTANT_13])
#define	CAN1_RESV3_14  	(IO_CAN1.RESV3[CONSTANT_14])
#define	CAN1_RESV3_15  	(IO_CAN1.RESV3[CONSTANT_15])
#define	CAN1_RESV3_16  	(IO_CAN1.RESV3[CONSTANT_16])
#define	CAN1_RESV3_17  	(IO_CAN1.RESV3[CONSTANT_17])
#define	CAN1_RESV3_18  	(IO_CAN1.RESV3[CONSTANT_18])
#define	CAN1_RESV3_19  	(IO_CAN1.RESV3[CONSTANT_19])
#define	CAN1_RESV3_20  	(IO_CAN1.RESV3[CONSTANT_20])
#define	CAN1_RESV3_21  	(IO_CAN1.RESV3[CONSTANT_21])
#define	CAN1_RESV3_22  	(IO_CAN1.RESV3[CONSTANT_22])
#define	CAN1_RESV3_23  	(IO_CAN1.RESV3[CONSTANT_23])
#define	CAN1_IF2CREQ1			(IO_CAN1.IO_IF2CREQ1.hword)
#define	CAN1_IF2CREQ1_IF2CREQ1H		(IO_CAN1.IO_IF2CREQ1.byte.IF2CREQ1H)
#define	CAN1_IF2CREQ1_IF2CREQ1L		(IO_CAN1.IO_IF2CREQ1.byte.IF2CREQ1L)
#define	CAN1_IF2CREQ1_BUSY    	(IO_CAN1.IO_IF2CREQ1.bit.BUSY)
#define	CAN1_IF2CREQ1_MN    	(IO_CAN1.IO_IF2CREQ1.bit.MN)
#define	CAN1_IF2CMSK1			(IO_CAN1.IO_IF2CMSK1.hword)
#define	CAN1_IF2CMSK1_IF2CMSK1H		(IO_CAN1.IO_IF2CMSK1.byte.IF2CMSK1H)
#define	CAN1_IF2CMSK1_IF2CMSK1L		(IO_CAN1.IO_IF2CMSK1.byte.IF2CMSK1L)
#define	CAN1_IF2CMSK1_WR_RD    	(IO_CAN1.IO_IF2CMSK1.bit.WR_RD)
#define	CAN1_IF2CMSK1_Mask    	(IO_CAN1.IO_IF2CMSK1.bit.Mask)
#define	CAN1_IF2CMSK1_Arb    	(IO_CAN1.IO_IF2CMSK1.bit.Arb)
#define	CAN1_IF2CMSK1_Control    	(IO_CAN1.IO_IF2CMSK1.bit.Control)
#define	CAN1_IF2CMSK1_CIP    	(IO_CAN1.IO_IF2CMSK1.bit.CIP)
#define	CAN1_IF2CMSK1_TxReq    	(IO_CAN1.IO_IF2CMSK1.bit.TxReq)
#define	CAN1_IF2CMSK1_DataA    	(IO_CAN1.IO_IF2CMSK1.bit.DataA)
#define	CAN1_IF2CMSK1_DataB    	(IO_CAN1.IO_IF2CMSK1.bit.DataB)
#define	CAN1_IF2MSK1			(IO_CAN1.IO_IF2MSK1.word)
#define	CAN1_IF2MSK1_IF2MSK21		(IO_CAN1.IO_IF2MSK1.hword.IF2MSK21)
#define	CAN1_IF2MSK1_IF2MSK11		(IO_CAN1.IO_IF2MSK1.hword.IF2MSK11)
#define	CAN1_IF2MSK1_IF2MSK21H		(IO_CAN1.IO_IF2MSK1.byte.IF2MSK21H)
#define	CAN1_IF2MSK1_IF2MSK21L		(IO_CAN1.IO_IF2MSK1.byte.IF2MSK21L)
#define	CAN1_IF2MSK1_IF2MSK11H		(IO_CAN1.IO_IF2MSK1.byte.IF2MSK11H)
#define	CAN1_IF2MSK1_IF2MSK11L		(IO_CAN1.IO_IF2MSK1.byte.IF2MSK11L)
#define	CAN1_IF2MSK1_MXtd    	(IO_CAN1.IO_IF2MSK1.bit.MXtd)
#define	CAN1_IF2MSK1_MDir    	(IO_CAN1.IO_IF2MSK1.bit.MDir)
#define	CAN1_IF2MSK1_Msk    	(IO_CAN1.IO_IF2MSK1.bit.Msk)
#define	CAN1_IF2ARB1			(IO_CAN1.IO_IF2ARB1.word)
#define	CAN1_IF2ARB1_IF2ARB21		(IO_CAN1.IO_IF2ARB1.hword.IF2ARB21)
#define	CAN1_IF2ARB1_IF2ARB11		(IO_CAN1.IO_IF2ARB1.hword.IF2ARB11)
#define	CAN1_IF2ARB1_IF2ARB21H		(IO_CAN1.IO_IF2ARB1.byte.IF2ARB21H)
#define	CAN1_IF2ARB1_IF2ARB21L		(IO_CAN1.IO_IF2ARB1.byte.IF2ARB21L)
#define	CAN1_IF2ARB1_IF2ARB11H		(IO_CAN1.IO_IF2ARB1.byte.IF2ARB11H)
#define	CAN1_IF2ARB1_IF2ARB11L		(IO_CAN1.IO_IF2ARB1.byte.IF2ARB11L)
#define	CAN1_IF2ARB1_MsgVal    	(IO_CAN1.IO_IF2ARB1.bit.MsgVal)
#define	CAN1_IF2ARB1_Xtd    	(IO_CAN1.IO_IF2ARB1.bit.Xtd)
#define	CAN1_IF2ARB1_Dir    	(IO_CAN1.IO_IF2ARB1.bit.Dir)
#define	CAN1_IF2ARB1_ID    	(IO_CAN1.IO_IF2ARB1.bit.ID)
#define	CAN1_IF2MCTR1			(IO_CAN1.IO_IF2MCTR1.hword)
#define	CAN1_IF2MCTR1_IF2MCTR1H		(IO_CAN1.IO_IF2MCTR1.byte.IF2MCTR1H)
#define	CAN1_IF2MCTR1_IF2MCTR1L		(IO_CAN1.IO_IF2MCTR1.byte.IF2MCTR1L)
#define	CAN1_IF2MCTR1_NewDat    	(IO_CAN1.IO_IF2MCTR1.bit.NewDat)
#define	CAN1_IF2MCTR1_MsgLst    	(IO_CAN1.IO_IF2MCTR1.bit.MsgLst)
#define	CAN1_IF2MCTR1_IntPnd    	(IO_CAN1.IO_IF2MCTR1.bit.IntPnd)
#define	CAN1_IF2MCTR1_UMask    	(IO_CAN1.IO_IF2MCTR1.bit.UMask)
#define	CAN1_IF2MCTR1_TxIE    	(IO_CAN1.IO_IF2MCTR1.bit.TxIE)
#define	CAN1_IF2MCTR1_RxIE    	(IO_CAN1.IO_IF2MCTR1.bit.RxIE)
#define	CAN1_IF2MCTR1_RmtEn    	(IO_CAN1.IO_IF2MCTR1.bit.RmtEn)
#define	CAN1_IF2MCTR1_TxRqst    	(IO_CAN1.IO_IF2MCTR1.bit.TxRqst)
#define	CAN1_IF2MCTR1_EoB    	(IO_CAN1.IO_IF2MCTR1.bit.EoB)
#define	CAN1_IF2MCTR1_DLC    	(IO_CAN1.IO_IF2MCTR1.bit.DLC)
#define	CAN1_IF2DTA11	(IO_CAN1.IO_IF2DTA11.hword)
#define	CAN1_IF2DTA11_IF2DTA11H	(IO_CAN1.IO_IF2DTA11.byte.IF2DTA11H)
#define	CAN1_IF2DTA11_IF2DTA11L	(IO_CAN1.IO_IF2DTA11.byte.IF2DTA11L)
#define	CAN1_IF2DTA21	(IO_CAN1.IO_IF2DTA21.hword)
#define	CAN1_IF2DTA21_IF2DTA21H	(IO_CAN1.IO_IF2DTA21.byte.IF2DTA21H)
#define	CAN1_IF2DTA21_IF2DTA21L	(IO_CAN1.IO_IF2DTA21.byte.IF2DTA21L)
#define	CAN1_IF2DTB11	(IO_CAN1.IO_IF2DTB11.hword)
#define	CAN1_IF2DTB11_IF2DTB11H	(IO_CAN1.IO_IF2DTB11.byte.IF2DTB11H)
#define	CAN1_IF2DTB11_IF2DTB11L	(IO_CAN1.IO_IF2DTB11.byte.IF2DTB11L)
#define	CAN1_IF2DTB21	(IO_CAN1.IO_IF2DTB21.hword)
#define	CAN1_IF2DTB21_IF2DTB21H	(IO_CAN1.IO_IF2DTB21.byte.IF2DTB21H)
#define	CAN1_IF2DTB21_IF2DTB21L	(IO_CAN1.IO_IF2DTB21.byte.IF2DTB21L)
#define	CAN1_RESV5_0  	(IO_CAN1.RESV5[CONSTANT_0])
#define	CAN1_RESV5_1  	(IO_CAN1.RESV5[CONSTANT_1])
#define	CAN1_RESV5_2  	(IO_CAN1.RESV5[CONSTANT_2])
#define	CAN1_RESV5_3  	(IO_CAN1.RESV5[CONSTANT_3])
#define	CAN1_RESV5_4  	(IO_CAN1.RESV5[CONSTANT_4])
#define	CAN1_RESV5_5  	(IO_CAN1.RESV5[CONSTANT_5])
#define	CAN1_RESV5_6  	(IO_CAN1.RESV5[CONSTANT_6])
#define	CAN1_RESV5_7  	(IO_CAN1.RESV5[CONSTANT_7])
#define	CAN1_RESV5_8  	(IO_CAN1.RESV5[CONSTANT_8])
#define	CAN1_RESV5_9  	(IO_CAN1.RESV5[CONSTANT_9])
#define	CAN1_RESV5_10  	(IO_CAN1.RESV5[CONSTANT_10])
#define	CAN1_RESV5_11  	(IO_CAN1.RESV5[CONSTANT_11])
#define	CAN1_RESV5_12  	(IO_CAN1.RESV5[CONSTANT_12])
#define	CAN1_RESV5_13  	(IO_CAN1.RESV5[CONSTANT_13])
#define	CAN1_RESV5_14  	(IO_CAN1.RESV5[CONSTANT_14])
#define	CAN1_RESV5_15  	(IO_CAN1.RESV5[CONSTANT_15])
#define	CAN1_RESV5_16  	(IO_CAN1.RESV5[CONSTANT_16])
#define	CAN1_RESV5_17  	(IO_CAN1.RESV5[CONSTANT_17])
#define	CAN1_RESV5_18  	(IO_CAN1.RESV5[CONSTANT_18])
#define	CAN1_RESV5_19  	(IO_CAN1.RESV5[CONSTANT_19])
#define	CAN1_RESV5_20  	(IO_CAN1.RESV5[CONSTANT_20])
#define	CAN1_RESV5_21  	(IO_CAN1.RESV5[CONSTANT_21])
#define	CAN1_RESV5_22  	(IO_CAN1.RESV5[CONSTANT_22])
#define	CAN1_RESV5_23  	(IO_CAN1.RESV5[CONSTANT_23])
#define	CAN1_RESV5_24  	(IO_CAN1.RESV5[CONSTANT_24])
#define	CAN1_RESV5_25  	(IO_CAN1.RESV5[CONSTANT_25])
#define	CAN1_RESV5_26  	(IO_CAN1.RESV5[CONSTANT_26])
#define	CAN1_RESV5_27  	(IO_CAN1.RESV5[CONSTANT_27])
#define	CAN1_RESV5_28  	(IO_CAN1.RESV5[CONSTANT_28])
#define	CAN1_RESV5_29  	(IO_CAN1.RESV5[CONSTANT_29])
#define	CAN1_RESV5_30  	(IO_CAN1.RESV5[CONSTANT_30])
#define	CAN1_RESV5_31  	(IO_CAN1.RESV5[CONSTANT_31])
#define	CAN1_RESV5_32  	(IO_CAN1.RESV5[CONSTANT_32])
#define	CAN1_RESV5_33  	(IO_CAN1.RESV5[CONSTANT_33])
#define	CAN1_RESV5_34  	(IO_CAN1.RESV5[CONSTANT_34])
#define	CAN1_RESV5_35  	(IO_CAN1.RESV5[CONSTANT_35])
#define	CAN1_RESV5_36  	(IO_CAN1.RESV5[CONSTANT_36])
#define	CAN1_RESV5_37  	(IO_CAN1.RESV5[CONSTANT_37])
#define	CAN1_RESV5_38  	(IO_CAN1.RESV5[CONSTANT_38])
#define	CAN1_RESV5_39  	(IO_CAN1.RESV5[CONSTANT_39])
#define	CAN1_TREQR1_12			(IO_CAN1.IO_TREQR1_12.word)
#define	CAN1_TREQR1_12_TREQR21		(IO_CAN1.IO_TREQR1_12.hword.TREQR21)
#define	CAN1_TREQR1_12_TREQR11		(IO_CAN1.IO_TREQR1_12.hword.TREQR11)
#define	CAN1_TREQR1_12_TREQR21H		(IO_CAN1.IO_TREQR1_12.byte.TREQR21H)
#define	CAN1_TREQR1_12_TREQR21L		(IO_CAN1.IO_TREQR1_12.byte.TREQR21L)
#define	CAN1_TREQR1_12_TREQR11H		(IO_CAN1.IO_TREQR1_12.byte.TREQR11H)
#define	CAN1_TREQR1_12_TREQR11L		(IO_CAN1.IO_TREQR1_12.byte.TREQR11L)
#define	CAN1_NEWDT1_12			(IO_CAN1.IO_NEWDT1_12.word)
#define	CAN1_NEWDT1_12_NEWDT21		(IO_CAN1.IO_NEWDT1_12.hword.NEWDT21)
#define	CAN1_NEWDT1_12_NEWDT11		(IO_CAN1.IO_NEWDT1_12.hword.NEWDT11)
#define	CAN1_NEWDT1_12_NEWDT21H		(IO_CAN1.IO_NEWDT1_12.byte.NEWDT21H)
#define	CAN1_NEWDT1_12_NEWDT21L		(IO_CAN1.IO_NEWDT1_12.byte.NEWDT21L)
#define	CAN1_NEWDT1_12_NEWDT11H		(IO_CAN1.IO_NEWDT1_12.byte.NEWDT11H)
#define	CAN1_NEWDT1_12_NEWDT11L		(IO_CAN1.IO_NEWDT1_12.byte.NEWDT11L)
#define	CAN1_INTPND1_12			(IO_CAN1.IO_INTPND1_12.word)
#define	CAN1_INTPND1_12_INTPND21		(IO_CAN1.IO_INTPND1_12.hword.INTPND21)
#define	CAN1_INTPND1_12_INTPND11		(IO_CAN1.IO_INTPND1_12.hword.INTPND11)
#define	CAN1_INTPND1_12_INTPND21H	(IO_CAN1.IO_INTPND1_12.byte.INTPND21H)
#define	CAN1_INTPND1_12_INTPND21L	(IO_CAN1.IO_INTPND1_12.byte.INTPND21L)
#define	CAN1_INTPND1_12_INTPND11H	(IO_CAN1.IO_INTPND1_12.byte.INTPND11H)
#define	CAN1_INTPND1_12_INTPND11L	(IO_CAN1.IO_INTPND1_12.byte.INTPND11L)
#define	CAN1_MSGVAL1_12			(IO_CAN1.IO_MSGVAL1_12.word)
#define	CAN1_MSGVAL1_12_MSGVAL21		(IO_CAN1.IO_MSGVAL1_12.hword.MSGVAL21)
#define	CAN1_MSGVAL1_12_MSGVAL11		(IO_CAN1.IO_MSGVAL1_12.hword.MSGVAL11)
#define	CAN1_MSGVAL1_12_MSGVAL21H	(IO_CAN1.IO_MSGVAL1_12.byte.MSGVAL21H)
#define	CAN1_MSGVAL1_12_MSGVAL21L	(IO_CAN1.IO_MSGVAL1_12.byte.MSGVAL21L)
#define	CAN1_MSGVAL1_12_MSGVAL11H	(IO_CAN1.IO_MSGVAL1_12.byte.MSGVAL11H)
#define	CAN1_MSGVAL1_12_MSGVAL11L	(IO_CAN1.IO_MSGVAL1_12.byte.MSGVAL11L)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_CAN2,attr=IO,locate=0x2200
#endif

__IO_EXTENDED	struct {
	union {
		__HWORD	hword;
		struct {
			__BYTE	CTRLR2H;
			__BYTE	CTRLR2L;
		} byte;
		struct {
			__BYTE	RESV00:1;
			__BYTE	RESV01:1;
			__BYTE	RESV02:1;
			__BYTE	RESV03:1;
			__BYTE	RESV04:1;
			__BYTE	RESV05:1;
			__BYTE	RESV06:1;
			__BYTE	RESV07:1;
			__BYTE	Test:1;
			__BYTE	CCE:1;
			__BYTE	DAR:1;
			__BYTE	RESV011:1;
			__BYTE	EIE:1;
			__BYTE	SIE:1;
			__BYTE	IE:1;
			__BYTE	Init:1;
		} bit;
	} IO_CTRLR2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	STATR2H;
			__BYTE	STATR2L;
		} byte;
		struct {
			__BYTE	RESV20:1;
			__BYTE	RESV21:1;
			__BYTE	RESV22:1;
			__BYTE	RESV23:1;
			__BYTE	RESV24:1;
			__BYTE	RESV25:1;
			__BYTE	RESV26:1;
			__BYTE	RESV27:1;
			__BYTE	BOff:1;
			__BYTE	EWarn:1;
			__BYTE	EPass:1;
			__BYTE	RxOk:1;
			__BYTE	TxOk:1;
			__BYTE	LEC:3;
		} bit;
	} IO_STATR2;
	const union {
		__HWORD	hword;
		struct {
			__BYTE	ERRCNT2H;
			__BYTE	ERRCNT2L;
		} byte;
		struct {
			__BYTE	RP:1;
			__BYTE	REC:7;
			__BYTE	TEC:8;
		} bit;
	} IO_ERRCNT2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	BTR2H;
			__BYTE	BTR2L;
		} byte;
		struct {
			__BYTE	RESV60:1;
			__BYTE	TSeg2:3;
			__BYTE	TSeg1:4;
			__BYTE	SJW:2;
			__BYTE	BRP:6;
		} bit;
	} IO_BTR2;
	const union {
		__HWORD	hword;
		struct {
			__BYTE	INTR2H;
			__BYTE	INTR2L;
		} byte;
	} IO_INTR2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	TESTR2H;
			__BYTE	TESTR2L;
		} byte;
		struct {
			__BYTE	RESV100:1;
			__BYTE	RESV101:1;
			__BYTE	RESV102:1;
			__BYTE	RESV103:1;
			__BYTE	RESV104:1;
			__BYTE	RESV105:1;
			__BYTE	RESV106:1;
			__BYTE	RESV107:1;
			__BYTE	Rx:1;
			__BYTE	Tx:2;
			__BYTE	LBack:1;
			__BYTE	Silent:1;
			__BYTE	Basic:1;
			__BYTE	RESV1013:1;
			__BYTE	RESV1014:1;
		} bit;
	} IO_TESTR2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	BRPER2H;
			__BYTE	BRPER2L;
		} byte;
		struct {
			__BYTE	RESV120:1;
			__BYTE	RESV121:1;
			__BYTE	RESV122:1;
			__BYTE	RESV123:1;
			__BYTE	RESV124:1;
			__BYTE	RESV125:1;
			__BYTE	RESV126:1;
			__BYTE	RESV127:1;
			__BYTE	RESV128:1;
			__BYTE	RESV129:1;
			__BYTE	RESV1210:1;
			__BYTE	RESV1211:1;
			__BYTE	BRPE:4;
		} bit;
	} IO_BRPER2;
	__HWORD	IO_RESV1;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1CREQ2H;
			__BYTE	IF1CREQ2L;
		} byte;
		struct {
			__BYTE	BUSY:1;
			__BYTE	RESV161:1;
			__BYTE	RESV162:1;
			__BYTE	RESV163:1;
			__BYTE	RESV164:1;
			__BYTE	RESV165:1;
			__BYTE	RESV166:1;
			__BYTE	RESV167:1;
			__BYTE	MN:8;
		} bit;
	} IO_IF1CREQ2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1CMSK2H;
			__BYTE	IF1CMSK2L;
		} byte;
		struct {
			__BYTE	RESV180:1;
			__BYTE	RESV181:1;
			__BYTE	RESV182:1;
			__BYTE	RESV183:1;
			__BYTE	RESV184:1;
			__BYTE	RESV185:1;
			__BYTE	RESV186:1;
			__BYTE	RESV187:1;
			__BYTE	WR_RD:1;
			__BYTE	Mask:1;
			__BYTE	Arb:1;
			__BYTE	Control:1;
			__BYTE	CIP:1;
			__BYTE	TxReq:1;
			__BYTE	DataA:1;
			__BYTE	DataB:1;
		} bit;
	} IO_IF1CMSK2;
	union {
		__WORD	word;
		struct {
			__HWORD	IF1MSK22;
			__HWORD	IF1MSK12;
		} hword;
		struct {
			__BYTE	IF1MSK22H;
			__BYTE	IF1MSK22L;
			__BYTE	IF1MSK12H;
			__BYTE	IF1MSK12L;
		} byte;
		struct {
			__WORD	MXtd:1;
			__WORD	MDir:1;
			__WORD	RESV202:1;
			__WORD	Msk:29;
		} bit;
	} IO_IF1MSK2;
	union {
		__WORD	word;
		struct {
			__HWORD	IF1ARB22;
			__HWORD	IF1ARB12;
		} hword;
		struct {
			__BYTE	IF1ARB22H;
			__BYTE	IF1ARB22L;
			__BYTE	IF1ARB12H;
			__BYTE	IF1ARB12L;
		} byte;
		struct {
			__WORD	MsgVal:1;
			__WORD	Xtd:1;
			__WORD	Dir:1;
			__WORD	ID:29;
		} bit;
	} IO_IF1ARB2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1MCTR2H;
			__BYTE	IF1MCTR2L;
		} byte;
		struct {
			__BYTE	NewDat:1;
			__BYTE	MsgLst:1;
			__BYTE	IntPnd:1;
			__BYTE	UMask:1;
			__BYTE	TxIE:1;
			__BYTE	RxIE:1;
			__BYTE	RmtEn:1;
			__BYTE	TxRqst:1;
			__BYTE	EoB:1;
			__BYTE	RESV289:1;
			__BYTE	RESV2810:1;
			__BYTE	RESV2811:1;
			__BYTE	DLC:4;
		} bit;
	} IO_IF1MCTR2;
	__HWORD	IO_RESV2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTA12H;
			__BYTE	IF1DTA12L;
		} byte;
	} IO_IF1DTA12;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTA22H;
			__BYTE	IF1DTA22L;
		} byte;
	} IO_IF1DTA22;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTB12H;
			__BYTE	IF1DTB12L;
		} byte;
	} IO_IF1DTB12;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF1DTB22H;
			__BYTE	IF1DTB22L;
		} byte;
	} IO_IF1DTB22;
	__WORD	IO_RESV3[CONSTANT_6];
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2CREQ2H;
			__BYTE	IF2CREQ2L;
		} byte;
		struct {
			__BYTE	BUSY:1;
			__BYTE	RESV641:1;
			__BYTE	RESV642:1;
			__BYTE	RESV643:1;
			__BYTE	RESV644:1;
			__BYTE	RESV645:1;
			__BYTE	RESV646:1;
			__BYTE	RESV647:1;
			__BYTE	MN:8;
		} bit;
	} IO_IF2CREQ2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2CMSK2H;
			__BYTE	IF2CMSK2L;
		} byte;
		struct {
			__BYTE	RESV660:1;
			__BYTE	RESV661:1;
			__BYTE	RESV662:1;
			__BYTE	RESV663:1;
			__BYTE	RESV664:1;
			__BYTE	RESV665:1;
			__BYTE	RESV666:1;
			__BYTE	RESV667:1;
			__BYTE	WR_RD:1;
			__BYTE	Mask:1;
			__BYTE	Arb:1;
			__BYTE	Control:1;
			__BYTE	CIP:1;
			__BYTE	TxReq:1;
			__BYTE	DataA:1;
			__BYTE	DataB:1;
		} bit;
	} IO_IF2CMSK2;
	union {
		__WORD	word;
		struct {
			__HWORD	IF2MSK22;
			__HWORD	IF2MSK12;
		} hword;
		struct {
			__BYTE	IF2MSK22H;
			__BYTE	IF2MSK22L;
			__BYTE	IF2MSK12H;
			__BYTE	IF2MSK12L;
		} byte;
		struct {
			__WORD	MXtd:1;
			__WORD	MDir:1;
			__WORD	RESV682:1;
			__WORD	Msk:29;
		} bit;
	} IO_IF2MSK2;
	union {
		__WORD	word;
		struct {
			__HWORD	IF2ARB22;
			__HWORD	IF2ARB12;
		} hword;
		struct {
			__BYTE	IF2ARB22H;
			__BYTE	IF2ARB22L;
			__BYTE	IF2ARB12H;
			__BYTE	IF2ARB12L;
		} byte;
		struct {
			__WORD	MsgVal:1;
			__WORD	Xtd:1;
			__WORD	Dir:1;
			__WORD	ID:29;
		} bit;
	} IO_IF2ARB2;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2MCTR2H;
			__BYTE	IF2MCTR2L;
		} byte;
		struct {
			__BYTE	NewDat:1;
			__BYTE	MsgLst:1;
			__BYTE	IntPnd:1;
			__BYTE	UMask:1;
			__BYTE	TxIE:1;
			__BYTE	RxIE:1;
			__BYTE	RmtEn:1;
			__BYTE	TxRqst:1;
			__BYTE	EoB:1;
			__BYTE	RESV769:1;
			__BYTE	RESV7610:1;
			__BYTE	RESV7611:1;
			__BYTE	DLC:4;
		} bit;
	} IO_IF2MCTR2;
	__HWORD	IO_RESV4;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTA12H;
			__BYTE	IF2DTA12L;
		} byte;
	} IO_IF2DTA12;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTA22H;
			__BYTE	IF2DTA22L;
		} byte;
	} IO_IF2DTA22;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTB12H;
			__BYTE	IF2DTB12L;
		} byte;
	} IO_IF2DTB12;
	union {
		__HWORD	hword;
		struct {
			__BYTE	IF2DTB22H;
			__BYTE	IF2DTB22L;
		} byte;
	} IO_IF2DTB22;
	__WORD	IO_RESV5[CONSTANT_10];
	const union {
		__WORD	word;
		struct {
			__HWORD	TREQR22;
			__HWORD	TREQR12;
		} hword;
		struct {
			__BYTE	TREQR22H;
			__BYTE	TREQR22L;
			__BYTE	TREQR12H;
			__BYTE	TREQR12L;
		} byte;
	} IO_TREQR2_12;
	__WORD	IO_RESV6;
	__WORD	IO_RESV7;
	__WORD	IO_RESV8;
	const union {
		__WORD	word;
		struct {
			__HWORD	NEWDT22;
			__HWORD	NEWDT12;
		} hword;
		struct {
			__BYTE	NEWDT22H;
			__BYTE	NEWDT22L;
			__BYTE	NEWDT12H;
			__BYTE	NEWDT12L;
		} byte;
	} IO_NEWDT2_12;
	__WORD	IO_RESV9;
	__WORD	IO_RESV10;
	__WORD	IO_RESV11;
	const union {
		__WORD	word;
		struct {
			__HWORD	INTPND22;
			__HWORD	INTPND12;
		} hword;
		struct {
			__BYTE	INTPND22H;
			__BYTE	INTPND22L;
			__BYTE	INTPND12H;
			__BYTE	INTPND12L;
		} byte;
	} IO_INTPND2_12;
	__WORD	IO_RESV12;
	__WORD	IO_RESV13;
	__WORD	IO_RESV14;
	const union {
		__WORD	word;
		struct {
			__HWORD	MSGVAL22;
			__HWORD	MSGVAL12;
		} hword;
		struct {
			__BYTE	MSGVAL22H;
			__BYTE	MSGVAL22L;
			__BYTE	MSGVAL12H;
			__BYTE	MSGVAL12L;
		} byte;
	} IO_MSGVAL2_12;
	__WORD	IO_RESV15;
	__WORD	IO_RESV16;
	__WORD	IO_RESV17;
} IO_CAN2;

#define	CAN2_CTRLR2			(IO_CAN2.IO_CTRLR2.hword)
#define	CAN2_CTRLR2_CTRLR2H		(IO_CAN2.IO_CTRLR2.byte.CTRLR2H)
#define	CAN2_CTRLR2_CTRLR2L		(IO_CAN2.IO_CTRLR2.byte.CTRLR2L)
#define	CAN2_CTRLR2_Test    	(IO_CAN2.IO_CTRLR2.bit.Test)
#define	CAN2_CTRLR2_CCE    	(IO_CAN2.IO_CTRLR2.bit.CCE)
#define	CAN2_CTRLR2_DAR    	(IO_CAN2.IO_CTRLR2.bit.DAR)
#define	CAN2_CTRLR2_EIE    	(IO_CAN2.IO_CTRLR2.bit.EIE)
#define	CAN2_CTRLR2_SIE    	(IO_CAN2.IO_CTRLR2.bit.SIE)
#define	CAN2_CTRLR2_IE    	(IO_CAN2.IO_CTRLR2.bit.IE)
#define	CAN2_CTRLR2_Init    	(IO_CAN2.IO_CTRLR2.bit.Init)
#define	CAN2_STATR2			(IO_CAN2.IO_STATR2.hword)
#define	CAN2_STATR2_STATR2H		(IO_CAN2.IO_STATR2.byte.STATR2H)
#define	CAN2_STATR2_STATR2L		(IO_CAN2.IO_STATR2.byte.STATR2L)
#define	CAN2_STATR2_BOff    	(IO_CAN2.IO_STATR2.bit.BOff)
#define	CAN2_STATR2_EWarn    	(IO_CAN2.IO_STATR2.bit.EWarn)
#define	CAN2_STATR2_EPass    	(IO_CAN2.IO_STATR2.bit.EPass)
#define	CAN2_STATR2_RxOk    	(IO_CAN2.IO_STATR2.bit.RxOk)
#define	CAN2_STATR2_TxOk    	(IO_CAN2.IO_STATR2.bit.TxOk)
#define	CAN2_STATR2_LEC    	(IO_CAN2.IO_STATR2.bit.LEC)
#define	CAN2_ERRCNT2			(IO_CAN2.IO_ERRCNT2.hword)
#define	CAN2_ERRCNT2_ERRCNT2H	(IO_CAN2.IO_ERRCNT2.byte.ERRCNT2H)
#define	CAN2_ERRCNT2_ERRCNT2L	(IO_CAN2.IO_ERRCNT2.byte.ERRCNT2L)
#define	CAN2_ERRCNT2_RP    	(IO_CAN2.IO_ERRCNT2.bit.RP)
#define	CAN2_ERRCNT2_REC    	(IO_CAN2.IO_ERRCNT2.bit.REC)
#define	CAN2_ERRCNT2_TEC    	(IO_CAN2.IO_ERRCNT2.bit.TEC)
#define	CAN2_BTR2			(IO_CAN2.IO_BTR2.hword)
#define	CAN2_BTR2_BTR2H	(IO_CAN2.IO_BTR2.byte.BTR2H)
#define	CAN2_BTR2_BTR2L	(IO_CAN2.IO_BTR2.byte.BTR2L)
#define	CAN2_BTR2_TSeg2    	(IO_CAN2.IO_BTR2.bit.TSeg2)
#define	CAN2_BTR2_TSeg1    	(IO_CAN2.IO_BTR2.bit.TSeg1)
#define	CAN2_BTR2_SJW    	(IO_CAN2.IO_BTR2.bit.SJW)
#define	CAN2_BTR2_BRP    	(IO_CAN2.IO_BTR2.bit.BRP)
#define	CAN2_INTR2			(IO_CAN2.IO_INTR2.hword)
#define	CAN2_INTR2_INTR2H	(IO_CAN2.IO_INTR2.byte.INTR2H)
#define	CAN2_INTR2_INTR2L	(IO_CAN2.IO_INTR2.byte.INTR2L)
#define	CAN2_TESTR2			(IO_CAN2.IO_TESTR2.hword)
#define	CAN2_TESTR2_TESTR2H	(IO_CAN2.IO_TESTR2.byte.TESTR2H)
#define	CAN2_TESTR2_TESTR2L	(IO_CAN2.IO_TESTR2.byte.TESTR2L)
#define	CAN2_TESTR2_Rx    	(IO_CAN2.IO_TESTR2.bit.Rx)
#define	CAN2_TESTR2_Tx    	(IO_CAN2.IO_TESTR2.bit.Tx)
#define	CAN2_TESTR2_LBack    	(IO_CAN2.IO_TESTR2.bit.LBack)
#define	CAN2_TESTR2_Silent    	(IO_CAN2.IO_TESTR2.bit.Silent)
#define	CAN2_TESTR2_Basic    	(IO_CAN2.IO_TESTR2.bit.Basic)
#define	CAN2_BRPER2			(IO_CAN2.IO_BRPER2.hword)
#define	CAN2_BRPER2_BRPER2H	(IO_CAN2.IO_BRPER2.byte.BRPER2H)
#define	CAN2_BRPER2_BRPER2L	(IO_CAN2.IO_BRPER2.byte.BRPER2L)
#define	CAN2_BRPER2_BRPE    	(IO_CAN2.IO_BRPER2.bit.BRPE)
#define	CAN2_IF1CREQ2			(IO_CAN2.IO_IF1CREQ2.hword)
#define	CAN2_IF1CREQ2_IF1CREQ2H		(IO_CAN2.IO_IF1CREQ2.byte.IF1CREQ2H)
#define	CAN2_IF1CREQ2_IF1CREQ2L		(IO_CAN2.IO_IF1CREQ2.byte.IF1CREQ2L)
#define	CAN2_IF1CREQ2_BUSY    	(IO_CAN2.IO_IF1CREQ2.bit.BUSY)
#define	CAN2_IF1CREQ2_MN    	(IO_CAN2.IO_IF1CREQ2.bit.MN)
#define	CAN2_IF1CMSK2			(IO_CAN2.IO_IF1CMSK2.hword)
#define	CAN2_IF1CMSK2_IF1CMSK2H		(IO_CAN2.IO_IF1CMSK2.byte.IF1CMSK2H)
#define	CAN2_IF1CMSK2_IF1CMSK2L		(IO_CAN2.IO_IF1CMSK2.byte.IF1CMSK2L)
#define	CAN2_IF1CMSK2_WR_RD    	(IO_CAN2.IO_IF1CMSK2.bit.WR_RD)
#define	CAN2_IF1CMSK2_Mask    	(IO_CAN2.IO_IF1CMSK2.bit.Mask)
#define	CAN2_IF1CMSK2_Arb    	(IO_CAN2.IO_IF1CMSK2.bit.Arb)
#define	CAN2_IF1CMSK2_Control    	(IO_CAN2.IO_IF1CMSK2.bit.Control)
#define	CAN2_IF1CMSK2_CIP    	(IO_CAN2.IO_IF1CMSK2.bit.CIP)
#define	CAN2_IF1CMSK2_TxReq    	(IO_CAN2.IO_IF1CMSK2.bit.TxReq)
#define	CAN2_IF1CMSK2_DataA    	(IO_CAN2.IO_IF1CMSK2.bit.DataA)
#define	CAN2_IF1CMSK2_DataB    	(IO_CAN2.IO_IF1CMSK2.bit.DataB)
#define	CAN2_IF1MSK2			(IO_CAN2.IO_IF1MSK2.word)
#define	CAN2_IF1MSK2_IF1MSK22		(IO_CAN2.IO_IF1MSK2.hword.IF1MSK22)
#define	CAN2_IF1MSK2_IF1MSK12		(IO_CAN2.IO_IF1MSK2.hword.IF1MSK12)
#define	CAN2_IF1MSK2_IF1MSK22H		(IO_CAN2.IO_IF1MSK2.byte.IF1MSK22H)
#define	CAN2_IF1MSK2_IF1MSK22L		(IO_CAN2.IO_IF1MSK2.byte.IF1MSK22L)
#define	CAN2_IF1MSK2_IF1MSK12H		(IO_CAN2.IO_IF1MSK2.byte.IF1MSK12H)
#define	CAN2_IF1MSK2_IF1MSK12L		(IO_CAN2.IO_IF1MSK2.byte.IF1MSK12L)
#define	CAN2_IF1MSK2_MXtd    	(IO_CAN2.IO_IF1MSK2.bit.MXtd)
#define	CAN2_IF1MSK2_MDir    	(IO_CAN2.IO_IF1MSK2.bit.MDir)
#define	CAN2_IF1MSK2_Msk    	(IO_CAN2.IO_IF1MSK2.bit.Msk)
#define	CAN2_IF1ARB2			(IO_CAN2.IO_IF1ARB2.word)
#define	CAN2_IF1ARB2_IF1ARB22		(IO_CAN2.IO_IF1ARB2.hword.IF1ARB22)
#define	CAN2_IF1ARB2_IF1ARB12		(IO_CAN2.IO_IF1ARB2.hword.IF1ARB12)
#define	CAN2_IF1ARB2_IF1ARB22H		(IO_CAN2.IO_IF1ARB2.byte.IF1ARB22H)
#define	CAN2_IF1ARB2_IF1ARB22L		(IO_CAN2.IO_IF1ARB2.byte.IF1ARB22L)
#define	CAN2_IF1ARB2_IF1ARB12H		(IO_CAN2.IO_IF1ARB2.byte.IF1ARB12H)
#define	CAN2_IF1ARB2_IF1ARB12L		(IO_CAN2.IO_IF1ARB2.byte.IF1ARB12L)
#define	CAN2_IF1ARB2_MsgVal    	(IO_CAN2.IO_IF1ARB2.bit.MsgVal)
#define	CAN2_IF1ARB2_Xtd    	(IO_CAN2.IO_IF1ARB2.bit.Xtd)
#define	CAN2_IF1ARB2_Dir    	(IO_CAN2.IO_IF1ARB2.bit.Dir)
#define	CAN2_IF1ARB2_ID    	(IO_CAN2.IO_IF1ARB2.bit.ID)
#define	CAN2_IF1MCTR2			(IO_CAN2.IO_IF1MCTR2.hword)
#define	CAN2_IF1MCTR2_IF1MCTR2H		(IO_CAN2.IO_IF1MCTR2.byte.IF1MCTR2H)
#define	CAN2_IF1MCTR2_IF1MCTR2L		(IO_CAN2.IO_IF1MCTR2.byte.IF1MCTR2L)
#define	CAN2_IF1MCTR2_NewDat    	(IO_CAN2.IO_IF1MCTR2.bit.NewDat)
#define	CAN2_IF1MCTR2_MsgLst    	(IO_CAN2.IO_IF1MCTR2.bit.MsgLst)
#define	CAN2_IF1MCTR2_IntPnd    	(IO_CAN2.IO_IF1MCTR2.bit.IntPnd)
#define	CAN2_IF1MCTR2_UMask    	(IO_CAN2.IO_IF1MCTR2.bit.UMask)
#define	CAN2_IF1MCTR2_TxIE    	(IO_CAN2.IO_IF1MCTR2.bit.TxIE)
#define	CAN2_IF1MCTR2_RxIE    	(IO_CAN2.IO_IF1MCTR2.bit.RxIE)
#define	CAN2_IF1MCTR2_RmtEn    	(IO_CAN2.IO_IF1MCTR2.bit.RmtEn)
#define	CAN2_IF1MCTR2_TxRqst    	(IO_CAN2.IO_IF1MCTR2.bit.TxRqst)
#define	CAN2_IF1MCTR2_EoB    	(IO_CAN2.IO_IF1MCTR2.bit.EoB)
#define	CAN2_IF1MCTR2_DLC    	(IO_CAN2.IO_IF1MCTR2.bit.DLC)
#define	CAN2_IF1DTA12	(IO_CAN2.IO_IF1DTA12.hword)
#define	CAN2_IF1DTA12_IF1DTA12H	(IO_CAN2.IO_IF1DTA12.byte.IF1DTA12H)
#define	CAN2_IF1DTA12_IF1DTA12L	(IO_CAN2.IO_IF1DTA12.byte.IF1DTA12L)
#define	CAN2_IF1DTA22	(IO_CAN2.IO_IF1DTA22.hword)
#define	CAN2_IF1DTA22_IF1DTA22H	(IO_CAN2.IO_IF1DTA22.byte.IF1DTA22H)
#define	CAN2_IF1DTA22_IF1DTA22L	(IO_CAN2.IO_IF1DTA22.byte.IF1DTA22L)
#define	CAN2_IF1DTB12	(IO_CAN2.IO_IF1DTB12.hword)
#define	CAN2_IF1DTB12_IF1DTB12H	(IO_CAN2.IO_IF1DTB12.byte.IF1DTB12H)
#define	CAN2_IF1DTB12_IF1DTB12L	(IO_CAN2.IO_IF1DTB12.byte.IF1DTB12L)
#define	CAN2_IF1DTB22	(IO_CAN2.IO_IF1DTB22.hword)
#define	CAN2_IF1DTB22_IF1DTB22H	(IO_CAN2.IO_IF1DTB22.byte.IF1DTB22H)
#define	CAN2_IF1DTB22_IF1DTB22L	(IO_CAN2.IO_IF1DTB22.byte.IF1DTB22L)
#define	CAN2_RESV3_0  	(IO_CAN2.RESV3[CONSTANT_0])
#define	CAN2_RESV3_1  	(IO_CAN2.RESV3[CONSTANT_1])
#define	CAN2_RESV3_2  	(IO_CAN2.RESV3[CONSTANT_2])
#define	CAN2_RESV3_3  	(IO_CAN2.RESV3[CONSTANT_3])
#define	CAN2_RESV3_4  	(IO_CAN2.RESV3[CONSTANT_4])
#define	CAN2_RESV3_5  	(IO_CAN2.RESV3[CONSTANT_5])
#define	CAN2_RESV3_6  	(IO_CAN2.RESV3[CONSTANT_6])
#define	CAN2_RESV3_7  	(IO_CAN2.RESV3[CONSTANT_7])
#define	CAN2_RESV3_8  	(IO_CAN2.RESV3[CONSTANT_8])
#define	CAN2_RESV3_9  	(IO_CAN2.RESV3[CONSTANT_9])
#define	CAN2_RESV3_10  	(IO_CAN2.RESV3[CONSTANT_10])
#define	CAN2_RESV3_11  	(IO_CAN2.RESV3[CONSTANT_11])
#define	CAN2_RESV3_12  	(IO_CAN2.RESV3[CONSTANT_12])
#define	CAN2_RESV3_13  	(IO_CAN2.RESV3[CONSTANT_13])
#define	CAN2_RESV3_14  	(IO_CAN2.RESV3[CONSTANT_14])
#define	CAN2_RESV3_15  	(IO_CAN2.RESV3[CONSTANT_15])
#define	CAN2_RESV3_16  	(IO_CAN2.RESV3[CONSTANT_16])
#define	CAN2_RESV3_17  	(IO_CAN2.RESV3[CONSTANT_17])
#define	CAN2_RESV3_18  	(IO_CAN2.RESV3[CONSTANT_18])
#define	CAN2_RESV3_19  	(IO_CAN2.RESV3[CONSTANT_19])
#define	CAN2_RESV3_20  	(IO_CAN2.RESV3[CONSTANT_20])
#define	CAN2_RESV3_21  	(IO_CAN2.RESV3[CONSTANT_21])
#define	CAN2_RESV3_22  	(IO_CAN2.RESV3[CONSTANT_22])
#define	CAN2_RESV3_23  	(IO_CAN2.RESV3[CONSTANT_23])
#define	CAN2_IF2CREQ2			(IO_CAN2.IO_IF2CREQ2.hword)
#define	CAN2_IF2CREQ2_IF2CREQ2H		(IO_CAN2.IO_IF2CREQ2.byte.IF2CREQ2H)
#define	CAN2_IF2CREQ2_IF2CREQ2L		(IO_CAN2.IO_IF2CREQ2.byte.IF2CREQ2L)
#define	CAN2_IF2CREQ2_BUSY    	(IO_CAN2.IO_IF2CREQ2.bit.BUSY)
#define	CAN2_IF2CREQ2_MN    	(IO_CAN2.IO_IF2CREQ2.bit.MN)
#define	CAN2_IF2CMSK2			(IO_CAN2.IO_IF2CMSK2.hword)
#define	CAN2_IF2CMSK2_IF2CMSK2H		(IO_CAN2.IO_IF2CMSK2.byte.IF2CMSK2H)
#define	CAN2_IF2CMSK2_IF2CMSK2L		(IO_CAN2.IO_IF2CMSK2.byte.IF2CMSK2L)
#define	CAN2_IF2CMSK2_WR_RD    	(IO_CAN2.IO_IF2CMSK2.bit.WR_RD)
#define	CAN2_IF2CMSK2_Mask    	(IO_CAN2.IO_IF2CMSK2.bit.Mask)
#define	CAN2_IF2CMSK2_Arb    	(IO_CAN2.IO_IF2CMSK2.bit.Arb)
#define	CAN2_IF2CMSK2_Control    	(IO_CAN2.IO_IF2CMSK2.bit.Control)
#define	CAN2_IF2CMSK2_CIP    	(IO_CAN2.IO_IF2CMSK2.bit.CIP)
#define	CAN2_IF2CMSK2_TxReq    	(IO_CAN2.IO_IF2CMSK2.bit.TxReq)
#define	CAN2_IF2CMSK2_DataA    	(IO_CAN2.IO_IF2CMSK2.bit.DataA)
#define	CAN2_IF2CMSK2_DataB    	(IO_CAN2.IO_IF2CMSK2.bit.DataB)
#define	CAN2_IF2MSK2			(IO_CAN2.IO_IF2MSK2.word)
#define	CAN2_IF2MSK2_IF2MSK22		(IO_CAN2.IO_IF2MSK2.hword.IF2MSK22)
#define	CAN2_IF2MSK2_IF2MSK12		(IO_CAN2.IO_IF2MSK2.hword.IF2MSK12)
#define	CAN2_IF2MSK2_IF2MSK22H		(IO_CAN2.IO_IF2MSK2.byte.IF2MSK22H)
#define	CAN2_IF2MSK2_IF2MSK22L		(IO_CAN2.IO_IF2MSK2.byte.IF2MSK22L)
#define	CAN2_IF2MSK2_IF2MSK12H		(IO_CAN2.IO_IF2MSK2.byte.IF2MSK12H)
#define	CAN2_IF2MSK2_IF2MSK12L		(IO_CAN2.IO_IF2MSK2.byte.IF2MSK12L)
#define	CAN2_IF2MSK2_MXtd    	(IO_CAN2.IO_IF2MSK2.bit.MXtd)
#define	CAN2_IF2MSK2_MDir    	(IO_CAN2.IO_IF2MSK2.bit.MDir)
#define	CAN2_IF2MSK2_Msk    	(IO_CAN2.IO_IF2MSK2.bit.Msk)
#define	CAN2_IF2ARB2			(IO_CAN2.IO_IF2ARB2.word)
#define	CAN2_IF2ARB2_IF2ARB22		(IO_CAN2.IO_IF2ARB2.hword.IF2ARB22)
#define	CAN2_IF2ARB2_IF2ARB12		(IO_CAN2.IO_IF2ARB2.hword.IF2ARB12)
#define	CAN2_IF2ARB2_IF2ARB22H		(IO_CAN2.IO_IF2ARB2.byte.IF2ARB22H)
#define	CAN2_IF2ARB2_IF2ARB22L		(IO_CAN2.IO_IF2ARB2.byte.IF2ARB22L)
#define	CAN2_IF2ARB2_IF2ARB12H		(IO_CAN2.IO_IF2ARB2.byte.IF2ARB12H)
#define	CAN2_IF2ARB2_IF2ARB12L		(IO_CAN2.IO_IF2ARB2.byte.IF2ARB12L)
#define	CAN2_IF2ARB2_MsgVal    	(IO_CAN2.IO_IF2ARB2.bit.MsgVal)
#define	CAN2_IF2ARB2_Xtd    	(IO_CAN2.IO_IF2ARB2.bit.Xtd)
#define	CAN2_IF2ARB2_Dir    	(IO_CAN2.IO_IF2ARB2.bit.Dir)
#define	CAN2_IF2ARB2_ID    	(IO_CAN2.IO_IF2ARB2.bit.ID)
#define	CAN2_IF2MCTR2			(IO_CAN2.IO_IF2MCTR2.hword)
#define	CAN2_IF2MCTR2_IF2MCTR2H		(IO_CAN2.IO_IF2MCTR2.byte.IF2MCTR2H)
#define	CAN2_IF2MCTR2_IF2MCTR2L		(IO_CAN2.IO_IF2MCTR2.byte.IF2MCTR2L)
#define	CAN2_IF2MCTR2_NewDat    	(IO_CAN2.IO_IF2MCTR2.bit.NewDat)
#define	CAN2_IF2MCTR2_MsgLst    	(IO_CAN2.IO_IF2MCTR2.bit.MsgLst)
#define	CAN2_IF2MCTR2_IntPnd    	(IO_CAN2.IO_IF2MCTR2.bit.IntPnd)
#define	CAN2_IF2MCTR2_UMask    	(IO_CAN2.IO_IF2MCTR2.bit.UMask)
#define	CAN2_IF2MCTR2_TxIE    	(IO_CAN2.IO_IF2MCTR2.bit.TxIE)
#define	CAN2_IF2MCTR2_RxIE    	(IO_CAN2.IO_IF2MCTR2.bit.RxIE)
#define	CAN2_IF2MCTR2_RmtEn    	(IO_CAN2.IO_IF2MCTR2.bit.RmtEn)
#define	CAN2_IF2MCTR2_TxRqst    	(IO_CAN2.IO_IF2MCTR2.bit.TxRqst)
#define	CAN2_IF2MCTR2_EoB    	(IO_CAN2.IO_IF2MCTR2.bit.EoB)
#define	CAN2_IF2MCTR2_DLC    	(IO_CAN2.IO_IF2MCTR2.bit.DLC)
#define	CAN2_IF2DTA12	(IO_CAN2.IO_IF2DTA12.hword)
#define	CAN2_IF2DTA12_IF2DTA12H	(IO_CAN2.IO_IF2DTA12.byte.IF2DTA12H)
#define	CAN2_IF2DTA12_IF2DTA12L	(IO_CAN2.IO_IF2DTA12.byte.IF2DTA12L)
#define	CAN2_IF2DTA22	(IO_CAN2.IO_IF2DTA22.hword)
#define	CAN2_IF2DTA22_IF2DTA22H	(IO_CAN2.IO_IF2DTA22.byte.IF2DTA22H)
#define	CAN2_IF2DTA22_IF2DTA22L	(IO_CAN2.IO_IF2DTA22.byte.IF2DTA22L)
#define	CAN2_IF2DTB12	(IO_CAN2.IO_IF2DTB12.hword)
#define	CAN2_IF2DTB12_IF2DTB12H	(IO_CAN2.IO_IF2DTB12.byte.IF2DTB12H)
#define	CAN2_IF2DTB12_IF2DTB12L	(IO_CAN2.IO_IF2DTB12.byte.IF2DTB12L)
#define	CAN2_IF2DTB22	(IO_CAN2.IO_IF2DTB22.hword)
#define	CAN2_IF2DTB22_IF2DTB22H	(IO_CAN2.IO_IF2DTB22.byte.IF2DTB22H)
#define	CAN2_IF2DTB22_IF2DTB22L	(IO_CAN2.IO_IF2DTB22.byte.IF2DTB22L)
#define	CAN2_RESV5_0  	(IO_CAN2.RESV5[CONSTANT_0])
#define	CAN2_RESV5_1  	(IO_CAN2.RESV5[CONSTANT_1])
#define	CAN2_RESV5_2  	(IO_CAN2.RESV5[CONSTANT_2])
#define	CAN2_RESV5_3  	(IO_CAN2.RESV5[CONSTANT_3])
#define	CAN2_RESV5_4  	(IO_CAN2.RESV5[CONSTANT_4])
#define	CAN2_RESV5_5  	(IO_CAN2.RESV5[CONSTANT_5])
#define	CAN2_RESV5_6  	(IO_CAN2.RESV5[CONSTANT_6])
#define	CAN2_RESV5_7  	(IO_CAN2.RESV5[CONSTANT_7])
#define	CAN2_RESV5_8  	(IO_CAN2.RESV5[CONSTANT_8])
#define	CAN2_RESV5_9  	(IO_CAN2.RESV5[CONSTANT_9])
#define	CAN2_RESV5_10  	(IO_CAN2.RESV5[CONSTANT_10])
#define	CAN2_RESV5_11  	(IO_CAN2.RESV5[CONSTANT_11])
#define	CAN2_RESV5_12  	(IO_CAN2.RESV5[CONSTANT_12])
#define	CAN2_RESV5_13  	(IO_CAN2.RESV5[CONSTANT_13])
#define	CAN2_RESV5_14  	(IO_CAN2.RESV5[CONSTANT_14])
#define	CAN2_RESV5_15  	(IO_CAN2.RESV5[CONSTANT_15])
#define	CAN2_RESV5_16  	(IO_CAN2.RESV5[CONSTANT_16])
#define	CAN2_RESV5_17  	(IO_CAN2.RESV5[CONSTANT_17])
#define	CAN2_RESV5_18  	(IO_CAN2.RESV5[CONSTANT_18])
#define	CAN2_RESV5_19  	(IO_CAN2.RESV5[CONSTANT_19])
#define	CAN2_RESV5_20  	(IO_CAN2.RESV5[CONSTANT_20])
#define	CAN2_RESV5_21  	(IO_CAN2.RESV5[CONSTANT_21])
#define	CAN2_RESV5_22  	(IO_CAN2.RESV5[CONSTANT_22])
#define	CAN2_RESV5_23  	(IO_CAN2.RESV5[CONSTANT_23])
#define	CAN2_RESV5_24  	(IO_CAN2.RESV5[CONSTANT_24])
#define	CAN2_RESV5_25  	(IO_CAN2.RESV5[CONSTANT_25])
#define	CAN2_RESV5_26  	(IO_CAN2.RESV5[CONSTANT_26])
#define	CAN2_RESV5_27  	(IO_CAN2.RESV5[CONSTANT_27])
#define	CAN2_RESV5_28  	(IO_CAN2.RESV5[CONSTANT_28])
#define	CAN2_RESV5_29  	(IO_CAN2.RESV5[CONSTANT_29])
#define	CAN2_RESV5_30  	(IO_CAN2.RESV5[CONSTANT_30])
#define	CAN2_RESV5_31  	(IO_CAN2.RESV5[CONSTANT_31])
#define	CAN2_RESV5_32  	(IO_CAN2.RESV5[CONSTANT_32])
#define	CAN2_RESV5_33  	(IO_CAN2.RESV5[CONSTANT_33])
#define	CAN2_RESV5_34  	(IO_CAN2.RESV5[CONSTANT_34])
#define	CAN2_RESV5_35  	(IO_CAN2.RESV5[CONSTANT_35])
#define	CAN2_RESV5_36  	(IO_CAN2.RESV5[CONSTANT_36])
#define	CAN2_RESV5_37  	(IO_CAN2.RESV5[CONSTANT_37])
#define	CAN2_RESV5_38  	(IO_CAN2.RESV5[CONSTANT_38])
#define	CAN2_RESV5_39  	(IO_CAN2.RESV5[CONSTANT_39])
#define	CAN2_TREQR2_12			(IO_CAN2.IO_TREQR2_12.word)
#define	CAN2_TREQR2_12_TREQR22		(IO_CAN2.IO_TREQR2_12.hword.TREQR22)
#define	CAN2_TREQR2_12_TREQR12		(IO_CAN2.IO_TREQR2_12.hword.TREQR12)
#define	CAN2_TREQR2_12_TREQR22H		(IO_CAN2.IO_TREQR2_12.byte.TREQR22H)
#define	CAN2_TREQR2_12_TREQR22L		(IO_CAN2.IO_TREQR2_12.byte.TREQR22L)
#define	CAN2_TREQR2_12_TREQR12H		(IO_CAN2.IO_TREQR2_12.byte.TREQR12H)
#define	CAN2_TREQR2_12_TREQR12L		(IO_CAN2.IO_TREQR2_12.byte.TREQR12L)
#define	CAN2_NEWDT2_12			(IO_CAN2.IO_NEWDT2_12.word)
#define	CAN2_NEWDT2_12_NEWDT22		(IO_CAN2.IO_NEWDT2_12.hword.NEWDT22)
#define	CAN2_NEWDT2_12_NEWDT12		(IO_CAN2.IO_NEWDT2_12.hword.NEWDT12)
#define	CAN2_NEWDT2_12_NEWDT22H		(IO_CAN2.IO_NEWDT2_12.byte.NEWDT22H)
#define	CAN2_NEWDT2_12_NEWDT22L		(IO_CAN2.IO_NEWDT2_12.byte.NEWDT22L)
#define	CAN2_NEWDT2_12_NEWDT12H		(IO_CAN2.IO_NEWDT2_12.byte.NEWDT12H)
#define	CAN2_NEWDT2_12_NEWDT12L		(IO_CAN2.IO_NEWDT2_12.byte.NEWDT12L)
#define	CAN2_INTPND2_12			(IO_CAN2.IO_INTPND2_12.word)
#define	CAN2_INTPND2_12_INTPND22		(IO_CAN2.IO_INTPND2_12.hword.INTPND22)
#define	CAN2_INTPND2_12_INTPND12		(IO_CAN2.IO_INTPND2_12.hword.INTPND12)
#define	CAN2_INTPND2_12_INTPND22H	(IO_CAN2.IO_INTPND2_12.byte.INTPND22H)
#define	CAN2_INTPND2_12_INTPND22L	(IO_CAN2.IO_INTPND2_12.byte.INTPND22L)
#define	CAN2_INTPND2_12_INTPND12H	(IO_CAN2.IO_INTPND2_12.byte.INTPND12H)
#define	CAN2_INTPND2_12_INTPND12L	(IO_CAN2.IO_INTPND2_12.byte.INTPND12L)
#define	CAN2_MSGVAL2_12			(IO_CAN2.IO_MSGVAL2_12.word)
#define	CAN2_MSGVAL2_12_MSGVAL22		(IO_CAN2.IO_MSGVAL2_12.hword.MSGVAL22)
#define	CAN2_MSGVAL2_12_MSGVAL12		(IO_CAN2.IO_MSGVAL2_12.hword.MSGVAL12)
#define	CAN2_MSGVAL2_12_MSGVAL22H	(IO_CAN2.IO_MSGVAL2_12.byte.MSGVAL22H)
#define	CAN2_MSGVAL2_12_MSGVAL22L	(IO_CAN2.IO_MSGVAL2_12.byte.MSGVAL22L)
#define	CAN2_MSGVAL2_12_MSGVAL12H	(IO_CAN2.IO_MSGVAL2_12.byte.MSGVAL12H)
#define	CAN2_MSGVAL2_12_MSGVAL12L	(IO_CAN2.IO_MSGVAL2_12.byte.MSGVAL12L)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DFCTLR,attr=IO,locate=0x2300
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	RESV0:1;
			__HWORD	FWE:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	RESV5:1;
			__HWORD	RESV6:1;
			__HWORD	RESV7:1;
			__HWORD	RESV8:1;
			__HWORD	RESV9:1;
			__HWORD	RESV10:1;
			__HWORD	RESV11:1;
			__HWORD	RESV12:1;
			__HWORD	RESV13:1;
			__HWORD	RESV14:1;
			__HWORD	RESV15:1;
	} bit;
} DFCTLRSTR;

__IO_EXTENDED	  DFCTLRSTR	IO_DFCTLR;
#define	_dfctlr		(IO_DFCTLR)
#define	DFCTLR		(IO_DFCTLR.hword)
#define	DFCTLR_FWE  	(IO_DFCTLR.bit.FWE)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DFSTR,attr=IO,locate=0x2303
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	RESV4:1;
			__BYTE	DFECCERR:1;
			__BYTE	DFHANG:1;
			__BYTE	DFRDY:1;
	} bit;
} DFSTRSTR;

__IO_EXTENDED	  DFSTRSTR	IO_DFSTR;
#define	_dfstr		(IO_DFSTR)
#define	DFSTR		(IO_DFSTR.byte)
#define	DFSTR_DFECCERR  	(IO_DFSTR.bit.DFECCERR)
#define	DFSTR_DFHANG  	(IO_DFSTR.bit.DFHANG)
#define	DFSTR_DFRDY  	(IO_DFSTR.bit.DFRDY)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_FLIFCTLR,attr=IO,locate=0x2308
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	DFWDSBL:1;
			__BYTE	RESV4:1;
			__BYTE	RESV5:1;
			__BYTE	ECCDSBL1:1;
			__BYTE	ECCDSBL0:1;
	} bit;
} FLIFCTLRSTR;

__IO_EXTENDED	  FLIFCTLRSTR	IO_FLIFCTLR;
#define	_flifctlr		(IO_FLIFCTLR)
#define	FLIFCTLR		(IO_FLIFCTLR.byte)
#define	FLIFCTLR_DFWDSBL  	(IO_FLIFCTLR.bit.DFWDSBL)
#define	FLIFCTLR_ECCDSBL1  	(IO_FLIFCTLR.bit.ECCDSBL1)
#define	FLIFCTLR_ECCDSBL0  	(IO_FLIFCTLR.bit.ECCDSBL0)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_SEEARX,attr=IO,locate=0x2400
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	D:14;
	} bit;
} SEEARXSTR;

__IO_EXTENDED	  SEEARXSTR	IO_SEEARX;
#define	_seearx		(IO_SEEARX)
#define	SEEARX		(IO_SEEARX.hword)
#define	SEEARX_D  	(IO_SEEARX.bit.D)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DEEARX,attr=IO,locate=0x2402
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	D:14;
	} bit;
} DEEARXSTR;

__IO_EXTENDED	  DEEARXSTR	IO_DEEARX;
#define	_deearx		(IO_DEEARX)
#define	DEEARX		(IO_DEEARX.hword)
#define	DEEARX_D  	(IO_DEEARX.bit.D)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EECSRX,attr=IO,locate=0x2404
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	DEIE:1;
			__BYTE	DEI:1;
			__BYTE	SEIE:1;
			__BYTE	SEI:1;
	} bit;
} EECSRXSTR;

__IO_EXTENDED	  EECSRXSTR	IO_EECSRX;
#define	_eecsrx		(IO_EECSRX)
#define	EECSRX		(IO_EECSRX.byte)
#define	EECSRX_DEIE  	(IO_EECSRX.bit.DEIE)
#define	EECSRX_DEI  	(IO_EECSRX.bit.DEI)
#define	EECSRX_SEIE  	(IO_EECSRX.bit.SEIE)
#define	EECSRX_SEI  	(IO_EECSRX.bit.SEI)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EFEARX,attr=IO,locate=0x2406
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	D:14;
	} bit;
} EFEARXSTR;

__IO_EXTENDED	  EFEARXSTR	IO_EFEARX;
#define	_efearx		(IO_EFEARX)
#define	EFEARX		(IO_EFEARX.hword)
#define	EFEARX_D  	(IO_EFEARX.bit.D)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EFECRX,attr=IO,locate=0x2408
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	RESV0:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	RESV5:1;
			__WORD	RESV6:1;
			__WORD	RESV7:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RESV10:1;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	RESV13:1;
			__WORD	RESV14:1;
			__WORD	FERR:1;
			__WORD	EY:8;
			__WORD	EI:8;
	} bit;
} EFECRXSTR;

__IO_EXTENDED	  EFECRXSTR	IO_EFECRX;
#define	_efecrx		(IO_EFECRX)
#define	EFECRX		(IO_EFECRX.word)
#define	EFECRX_FERR  	(IO_EFECRX.bit.FERR)
#define	EFECRX_EY  	(IO_EFECRX.bit.EY)
#define	EFECRX_EI  	(IO_EFECRX.bit.EI)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_SEEARA,attr=IO,locate=0x3000
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	D:11;
	} bit;
} SEEARASTR;

__IO_EXTENDED	  SEEARASTR	IO_SEEARA;
#define	_seeara		(IO_SEEARA)
#define	SEEARA		(IO_SEEARA.hword)
#define	SEEARA_D  	(IO_SEEARA.bit.D)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DEEARA,attr=IO,locate=0x3002
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	D:11;
	} bit;
} DEEARASTR;

__IO_EXTENDED	  DEEARASTR	IO_DEEARA;
#define	_deeara		(IO_DEEARA)
#define	DEEARA		(IO_DEEARA.hword)
#define	DEEARA_D  	(IO_DEEARA.bit.D)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EECSRA,attr=IO,locate=0x3004
#endif

typedef union {
	__BYTE	byte;
	struct {
			__BYTE	RESV0:1;
			__BYTE	RESV1:1;
			__BYTE	RESV2:1;
			__BYTE	RESV3:1;
			__BYTE	DEIE:1;
			__BYTE	DEI:1;
			__BYTE	SEIE:1;
			__BYTE	SEI:1;
	} bit;
} EECSRASTR;

__IO_EXTENDED	  EECSRASTR	IO_EECSRA;
#define	_eecsra		(IO_EECSRA)
#define	EECSRA		(IO_EECSRA.byte)
#define	EECSRA_DEIE  	(IO_EECSRA.bit.DEIE)
#define	EECSRA_DEI  	(IO_EECSRA.bit.DEI)
#define	EECSRA_SEIE  	(IO_EECSRA.bit.SEIE)
#define	EECSRA_SEI  	(IO_EECSRA.bit.SEI)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EFEARA,attr=IO,locate=0x3006
#endif

typedef union {
	__HWORD	hword;
	struct {
			__HWORD	RESV0:1;
			__HWORD	RESV1:1;
			__HWORD	RESV2:1;
			__HWORD	RESV3:1;
			__HWORD	RESV4:1;
			__HWORD	D:11;
	} bit;
} EFEARASTR;

__IO_EXTENDED	  EFEARASTR	IO_EFEARA;
#define	_efeara		(IO_EFEARA)
#define	EFEARA		(IO_EFEARA.hword)
#define	EFEARA_D  	(IO_EFEARA.bit.D)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EFECRA,attr=IO,locate=0x3408
#endif

typedef union {
	__WORD	word;
	struct {
			__WORD	RESV0:1;
			__WORD	RESV1:1;
			__WORD	RESV2:1;
			__WORD	RESV3:1;
			__WORD	RESV4:1;
			__WORD	RESV5:1;
			__WORD	RESV6:1;
			__WORD	RESV7:1;
			__WORD	RESV8:1;
			__WORD	RESV9:1;
			__WORD	RESV10:1;
			__WORD	RESV11:1;
			__WORD	RESV12:1;
			__WORD	RESV13:1;
			__WORD	RESV14:1;
			__WORD	FERR:1;
			__WORD	EY:8;
			__WORD	EI:8;
	} bit;
} EFECRASTR;

__IO_EXTENDED	  EFECRASTR	IO_EFECRA;
#define	_efecra		(IO_EFECRA)
#define	EFECRA		(IO_EFECRA.word)
#define	EFECRA_FERR  	(IO_EFECRA.bit.FERR)
#define	EFECRA_EY  	(IO_EFECRA.bit.EY)
#define	EFECRA_EI  	(IO_EFECRA.bit.EI)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_DSUCR,attr=IO,locate=0xFF00
#endif

__IO_EXTENDED	__HWORD	IO_DSUCR;
#define	_dsucr		(IO_DSUCR)
#define	DSUCR	(_dsucr)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PCSR,attr=IO,locate=0xFF10
#endif

__IO_EXTENDED	__WORD	IO_PCSR;
#define	_pcsr		(IO_PCSR)
#define	PCSR	(_pcsr)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_PSSR,attr=IO,locate=0xFF14
#endif

__IO_EXTENDED	__WORD	IO_PSSR;
#define	_pssr		(IO_PSSR)
#define	PSSR	(_pssr)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EDIR1,attr=IO,locate=0xFFF8
#endif

__IO_EXTENDED	const __WORD	IO_EDIR1;
#define	_edir1		(IO_EDIR1)
#define	EDIR1	(_edir1)

#ifdef __IO_DEFINE
#pragma segment     DATA=IO_EDIR0,attr=IO,locate=0xFFFC
#endif

__IO_EXTENDED	const __WORD	IO_EDIR0;
#define	_edir0		(IO_EDIR0)
#define	EDIR0	(_edir0)
