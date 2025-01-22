/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: FR81S								*/
/* Date				: 16/08/18							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Toguchi						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		:									*/
/* date				:									*/
/********************************************************/

/*------------------------------*/
/*      �g�p�w�b�_�t�@�C��      */
/*------------------------------*/
#include <string.h>
#include "mb91570.h"
#include "SSFTSTD_Type.h"
#include "SSFTSTD_Macro.h"
#include "DTO459_VDV_Sd_Drv.h"
#include "SSFTSTD_FR81_Port_Drv.h"
#include "timer_ctrl.h"
#include "dbglog.h"


/*------------------------------*/
/* CRC�`�F�b�N�@�\�ؑ�			*/
/*------------------------------*/
#define	SDDRV_CRC_ON			/* �L���ɂ����ꍇ�ASD�J�[�h�Ƃ̃f�[�^����M��CRC�`�F�b�N�����{���� */
#define SDDRV_CRC16_LEFT		/* CRC16�Z�o���@:�L���ɂ����ꍇ ������, �����ɂ����ꍇ �E���� */
#define SDDRV_CRC16_XOR_0		/* CRC16 Xor���@: �L���ɂ����ꍇ Xor 0x0000, �����ɂ����ꍇ Xor 0xFFFF */

/*------------------------------*/
/*        �����f�[�^�\��        */
/*------------------------------*/

typedef enum e_spierror {				/* [�����I�G���[�R�[�h]		*/
	SPI_NO_ERROR = 0,					/* 0- Successful completion */
	SPI_CARD_IS_NOT_RESPONDING,			/* 1- Time out error on card response */
	SPI_CMD_CRC_ERROR,					/* 2- CRC error detected on card response */
	SPI_DATA_CRC_ERROR,					/* 3- CRC error detected on incoming data block */
	SPI_DATA_STATUS_CRC_ERROR,			/* 4- Card is reporting CRC error on outgoing data block. */
	SPI_CARD_IS_BUSY,					/* 5- Card is busy programming */
	SPI_CARD_IS_NOT_READY,				/* 6- Card did not complete its initialization and is not ready. */
	SPI_COMUNC_CRC_ERROR,				/* 7- Card is reporting CRC error */
	SPI_COMUNC_ILLEG_COM,				/* 8- Card is reporting illegal command */
	SPI_ERASE_PARAM,					/* 9- Erase parameters error */
	SPI_WP_VIOLATION,					/* 10- Attempt to write a WP sector */
	SPI_ERROR,							/* 11- MMC card internal error */
	SPI_WP_ERASE_SKIP,					/* 12- Attempt to erase WP sector */
	SPI_ADDRESS_ERROR,					/* 13- Sector messaligned error */
	SPI_CARD_READ_FAILURE,				/* 14- Card is reporting Read command failed */
	SPI_INTERFACE_ERROR,				/* 15- Error detected by the MMC HW driver */
	SPI_ILLEGAL_MODE,					/* 16- Not support in the current mode */
	SPI_COMMAND_PARAMETER_ERROR,		/* 17- Card is reporting Address-out-of-range error */
	SPI_ERASE_SEQ_ERROR,				/* 18- Error in the sequence of erase command */
	SPI_ERASE_RESET,					/* 19- Erase command canceled before execution */
	SPI_NO_CRC_STATUS,					/* 20- Time out on CRC status for Write */
	SPI_OVERRUN,						/* 21- Overrun */
	SPI_UNDERRUN,						/* 22- Underrun */
	SPI_CIDCSD_OVERWRITE,				/* 23- a) The CID register has been already */
										/*         written and can be overwriten. */
										/*      b) The read only section of CSD does not */
										/*         match the card content. */
										/*      c) An attempt to reverse the copy (set as original)  */
										/*         or permanent WP bits was made. */
	SPI_CARD_ECC_DISABLED,				/* 24- The command has been executed without using the internal ECC. */
	SPI_READ_FOR_DATA,					/* 25- Corresponds to buffer empty signalling on the bus. */
	SPI_DATA_LENGTH_ERROR,				/* 26- Data Length more then 512 bytes. */
	SPI_TIME_OUT_RCVD,					/* 27- Time out recive data (B0 for controller) */
	SPI_OUT_OF_RANGE,					/* 28- Address out of range error */
	SPI_CARD_ECC_FAILED,				/* 29- Internal Card ECC failed */
	SPI_CARD_WRITE_ERROR				/* 30- Card is reporting Write error on outgoing data block. */
} E_SPIERROR;

typedef enum e_sd_init_type {			/* [SD�������`��] */
	SD_INIT_TYPE_SD_INVALID = 0,		/* SD ���������� */
	SD_INIT_TYPE_SD_VER1,				/* SD Ver1.X�Ƃ��ď����� */
	SD_INIT_TYPE_SD_VER2				/* SD Ver2.0�ȍ~�Ƃ��ď����� */
} E_SD_INIT_TYPE;

typedef enum e_resp_type {				/* [�R�}���h���X�|���X�`��] */
	RZ = 0,								/* NONE response */
	R1,									/* R1 response. Basic MMC response format */
	R2									/* R2 response. Used by SEND_STATUS(CMD13) in SPI mode */
} E_RESP_TYPE;


/*------------------------------*/
/*          �����}�N��          */
/*------------------------------*/
/* --- LIN-UART ��������ݒ�f�[�^--- */
#define INIT_SCR				(0x23U)		/* �V���A�����䃌�W�X�^ */
											/* <07>		UPCL	: 0				(�v���O���}�u���N���A�r�b�g ����ɉe�����Ȃ�) */
											/* <06>		MS		: 0				(�}�X�^/�X���[�u�@�\�I���r�b�g �}�X�^�[) */
											/* <05>		SPI		: 1				(SPI�Ή��r�b�g SPI�ʐM) */
											/* <04>		RIE		: 0				(��M�����݋��r�b�g �֎~) */
											/* <03>		TIE		: 0				(���M�����݋��r�b�g �֎~) */
											/* <02>		TBIE	: 0				(���M�o�X�A�C�h�������݋��r�b�g �֎~) */
											/* <01>		RXE		: 1				(��M���싖�r�b�g ����) */
											/* <00>		TXE		: 1				(���M���싖�r�b�g ����) */

#define INIT_SMR				(0x4FU)		/* �V���A�����[�h���W�X�^ */
											/* <07-05>	MD		: 010			(���샂�[�h ���샂�[�h2(CSIO���[�h)) */
											/* <04>		-		: 0				(�\��bit) */
											/* <03>		SCINV	: 1				(�V���A���N���b�N���]�r�b�g L) */
											/* <02>		BDS		: 1				(�]�������I���r�b�g MSB�t�@�[�X�g) */
											/* <01>		SCKE	: 1				(�V���A���N���b�N�o�͋��r�b�g �V���A���N���b�N�o�͋���) */
											/* <00>		SOE		: 1				(�V���A���f�[�^�o�͋��r�b�g �o�͋���) */

#define INIT_SSR				(0x83U)		/* �V���A���X�e�[�^�X���W�X�^ */
											/* <07>		REC		: 1				(��M�G���[�t���O�N���A�r�b�g �N���A) */
											/* <06-04>	-		: 000			(�\��bit) */
											/* <03>		ORE		: 0				(�I�[�o�����G���[�t���O�r�b�g �����ݖ���) */
											/* <02>		RDRF	: 0				(��M�f�[�^�t���t���O�r�b�g �����ݖ���) */
											/* <01>		TDRE	: 1				(���M�f�[�^�G���v�e�B�t���O�r�b�g �����ݖ���) */
											/* <00>		TBI		: 1				(���M�o�X�A�C�h���t���O�r�b�g �����ݖ���) */

#define INIT_ESCR				(0x00U)		/* �g���ʐM���䃌�W�X�^ */
											/* <07>		SOP		: 0				(�V���A���o�͒[�q�Z�b�g�r�b�g ����ɉe�����Ȃ�) */
											/* <06-05>	-		: 00			(�\��bit) */
											/* <04-03>	WT		: 00			(�f�[�^����M�E�F�C�g�I���r�b�g �E�F�C�g�Ȃ�) */
											/* <02-00>	L		: 000			(�f�[�^���I���r�b�g 8�r�b�g) */

#define INIT_FCR0				(0x00U)		/* FIFO���䃌�W�X�^0 */
											/* <07>		-		: 0				(�\��bit) */
											/* <06>		FLST	: 0				(FIFO�đ��f�[�^���X�g�t���O�r�b�g �����ݖ���) */
											/* <05>		FLD		: 0				(FIFO�|�C���^�����[�h�r�b�g ����ɉe�����Ȃ�) */
											/* <04>		FSET	: 0				(FIFO�|�C���^�ۑ��r�b�g ����ɉe�����Ȃ�) */
											/* <03>		FCL2	: 0				(FIFO2���Z�b�g�r�b�g ����ɉe�����Ȃ�) */
											/* <02>		FCL1	: 0				(FIFO1���Z�b�g�r�b�g ����ɉe�����Ȃ�) */
											/* <01>		FE2-	: 0				(FIFO2���싖�r�b�g �֎~) */
											/* <00>		FE1		: 0				(FIFO1���싖�r�b�g �֎~) */

#define INIT_FCR1				(0x00U)		/* FIFO���䃌�W�X�^1 */
											/* <07-06>	-		: 00			(�\��bit) */
											/* <05>		-		: 0				(�\��bit) */
											/* <04>		FLSTE	: 0				(�đ��f�[�^���X�g���o���r�b�g �֎~) */
											/* <03>		FRIIE	: 0				(��MFIFO�A�C�h�����o���r�b�g �֎~) */
											/* <02>		FDRQ	: 0				(���MFIFO�f�[�^�v���r�b�g ���Z�b�g) */
											/* <01>		FTIE-	: 0				(���MFIFO�����݋��r�b�g �֎~) */
											/* <00>		FSEL	: 0				(FIFO�I���r�b�g ���MFIFO1�A��MFIFO2) */

#define INIT_FBYTE				(0x0000U)	/* FIFO�o�C�g���W�X�^ */
											/* <15-08>	FBYTE2	: 0				(FIFO2�f�[�^���\���r�b�g 0(���g�p)) */
											/* <07-00>	FBYTE1	: 0				(FIFO1�f�[�^���\���r�b�g 0(���g�p)) */


#define SCR_UPCL_SET		(0x80U)			/* �v���O���}�u���N���A�r�b�g�Z�b�g�f�[�^ */
#define SCR_RIE_BIT			(0x10U)			/* SCR.RIE: ��M�����ݗv�����r�b�g:bit4 */
#define SCR_TIE_BIT			(0x08U)			/* SCR.TIE: ���M�����ݗv�����r�b�g:bit3 */
#define SCR_RXE_BIT			(0x02U)			/* SCR.RXE: ��M���r�b�g:bit1 */
#define SCR_TXE_BIT			(0x01U)			/* SCR.TXE: ���M���r�b�g:bit0 */
#define SSR_REC_BIT			(0x80U)			/* ��M�G���[�t���O�N���A�r�b�g:bit7 */
#define SMR_SOE_DISABLE		(UI_8)(~0x01U)	/* �V���A�����[�h���W�X�^ �f�[�^�o�͋��r�b�g:bit0 ���� */
#define SMR_SCKE_DISABLE	(UI_8)(~0x02U)	/* �V���A�����[�h���W�X�^ �N���b�N�o�͋��r�b�g:bit1 ���� */
#define SMR_SCKE_MASK		(0x02U)			/* �V���A�����[�h���W�X�^ �N���b�N�o�͋��r�b�g���o�}�X�N */
#define FCR0_FE_DISABLE		(UI_8)(~0x03U)	/* FIFO���䃌�W�X�^0 FIFO���싖�r�b�g:bit1-0 ���� */

/* --- LIN-UART �ʏ퓮��ݒ�f�[�^--- */
#define EXIRX_SSR			(0x04U)			/* �ð��ڼ޽�	��M�ް����o AND�ޯ�				*/
#define EMPTX_SSR			(0x02U)			/* �ð��ڼ޽�	���M�ް���	 AND�ޯ�				*/
#define ERROR_SSR			(0x08U)			/* �ð��ڼ޽�	�װ=������ AND�ޯ�					*/
#define SSR_TBI_BIT			(0x01U)			/* ���M�o�X�A�C�h���t���O�r�b�g�ʒu:bit0 */
#define SSR_RDRF_BIT		(0x04U)			/* ��M�f�[�^�t���t���O�r�b�g�ʒu:bit2 */

/* -- LIN-UART �{�[���[�g�ݒ�f�[�^-- */
#define SCK_16MHZ_BR_4MBPS		(3U)		/* �N���b�N16MHz, �{�[���[�g4Mbps�ݒ�				*/
#define SCK_16MHZ_BR_2MBPS		(7U)		/* �N���b�N16MHz, �{�[���[�g2Mbps�ݒ�				*/
#define SCK_16MHZ_BR_1MBPS		(15U)		/* �N���b�N16MHz, �{�[���[�g1Mbps�ݒ�				*/
#define SCK_16MHZ_BR_500KBPS	(31U)		/* �N���b�N16MHz, �{�[���[�g500Kbps�ݒ�				*/
#define SCK_16MHZ_BR_400KBPS	(39U)		/* �N���b�N16MHz, �{�[���[�g400Kbps�ݒ�				*/


/* --- �R�}���h�C���f�b�N�X --- */
#define CMD_START_BIT           (UI_16)(0x40)	 					/* �R�}���h�X�^�[�g�r�b�g								*/
#define GO_IDLE_STATE           (UI_16)(0 + CMD_START_BIT)			/* CMD0 : �J�[�h���Z�b�g								*/
#define SEND_OP_COND            (UI_16)(1 + CMD_START_BIT)			/* CMD1 : ���������s									*/
#define ALL_SEND_CID            (UI_16)(2 + CMD_START_BIT)			/* CMD2 : �S�J�[�h�ɑ΂���CID�ԍ����M�v��				*/
#define SET_RELATIVE_ADDR       (UI_16)(3 + CMD_START_BIT)			/* CMD3 : �J�[�h�ɑ��΃A�h���X���蓖��					*/
#define SET_DSR                 (UI_16)(4 + CMD_START_BIT)			/* CMD4 : �J�[�h��DSR���v���O��������					*/
#define SELECT_DESELECT_CARD    (UI_16)(7 + CMD_START_BIT)			/* CMD7 : �J�[�h�̑I����Ԑ؂�ւ�						*/
#define SEND_IF_COND            (UI_16)(8 + CMD_START_BIT)			/* CMD8 : �C���^�[�t�F�[�X�����m�F(SD�J�[�h��p)		*/
#define SEND_CSD                (UI_16)(9 + CMD_START_BIT)			/* CMD9 : CSD���W�X�^�ǂݏo��							*/
#define SEND_CID                (UI_16)(10 + CMD_START_BIT)			/* CMD10: CID���W�X�^�ǂݏo��							*/
#define READ_DAT_UNTIL_STOP     (UI_16)(11 + CMD_START_BIT)			/* CMD11: �f�[�^�]���I���܂ł̊ԃf�[�^��ǂݏo��		*/
#define STOP_TRANSMISSION       (UI_16)(12 + CMD_START_BIT)			/* CMD12: �f�[�^�]���I��								*/
#define SEND_STATUS             (UI_16)(13 + CMD_START_BIT)			/* CMD13: �X�e�[�^�X���W�X�^���M�v��					*/
#define SET_BUS_WIDTH_REGISTER  (UI_16)(14 + CMD_START_BIT)			/* CMD14: 												*/
#define GO_INACTIVE_STATE       (UI_16)(15 + CMD_START_BIT)			/* CMD15: �J�[�h��Ԃ�inactive�X�e�[�g�ɂ���			*/
#define SET_BLOCKLEN            (UI_16)(16 + CMD_START_BIT)			/* CMD16: �f�[�^�u���b�N���ݒ�							*/
#define READ_BLOCK              (UI_16)(17 + CMD_START_BIT)			/* CMD17: �V���O���u���b�N�ǂݏo��						*/
#define READ_MULTIPLE_BLOCK     (UI_16)(18 + CMD_START_BIT)			/* CMD18: �}���`�u���b�N�ǂݏo��						*/
#define WRITE_DAT_UNTIL_STOP    (UI_16)(20 + CMD_START_BIT)			/* CMD20: �f�[�^�]���I���܂ł̊ԃf�[�^����������		*/
#define SET_WR_BLK_ERASE_COUNT_ACMD23  (UI_16)(23 + CMD_START_BIT)	/* ACMD23: �������݊J�n���̏����u���b�N���w��(SD�J�[�h��p)	*/
#define WRITE_BLOCK             (UI_16)(24 + CMD_START_BIT)			/* CMD24: �V���O���u���b�N��������						*/
#define WRITE_MULTIPLE_BLOCK    (UI_16)(25 + CMD_START_BIT)			/* CMD25: �}���`�u���b�N��������						*/
#define PROGRAM_CID             (UI_16)(26 + CMD_START_BIT)			/* CMD26: CID���W�X�^�̃v���O���~���O					*/
#define PROGRAM_CSD             (UI_16)(27 + CMD_START_BIT)			/* CMD27: CSD���W�X�^�̃v���O���~���O					*/
#define SET_WRITE_PROT          (UI_16)(28 + CMD_START_BIT)			/* CMD28: ���C�g�v���e�N�g�r�b�g�̐ݒ�					*/
#define CLR_WRITE_PROT          (UI_16)(29 + CMD_START_BIT)			/* CMD29: ���C�g�v���e�N�g�r�b�g�̃N���A				*/
#define SEND_WRITE_PROT         (UI_16)(30 + CMD_START_BIT)			/* CMD30: ���C�g�v���e�N�g�r�b�g�̃X�e�[�^�X�v��		*/
#define TAG_SECTOR_START        (UI_16)(32 + CMD_START_BIT)			/* CMD32: �A�������͈͂̐擪�Z�N�^�A�h���X�ݒ�			*/
#define TAG_SECTOR_END          (UI_16)(33 + CMD_START_BIT)			/* CMD33: �A�������͈͂̍ŏI�Z�N�^�A�h���X�ݒ�			*/
#define UNTAG_SECTOR            (UI_16)(34 + CMD_START_BIT)			/* CMD34: �����Z�N�^�̉���								*/
#define TAG_ERASE_GROUP_START   (UI_16)(35 + CMD_START_BIT)			/* CMD35: �A���������̐擪�����O���[�v�̃A�h���X�ݒ�	*/
#define TAG_ERASE_GROUP_END     (UI_16)(36 + CMD_START_BIT)			/* CMD36: �A���������̏I�[�����O���[�v�̃A�h���X�ݒ�	*/
#define UNTAG_ERASE_GROUP       (UI_16)(37 + CMD_START_BIT)			/* CMD37: �����O���[�v�̉���							*/
#define ERASE_SECTORS           (UI_16)(38 + CMD_START_BIT)			/* CMD38: �I���Z�N�^�̏���								*/
#define SEND_OP_COND_ACMD41     (UI_16)(41 + CMD_START_BIT)			/* CMD41: SD��p�R�}���h								*/
#define APP_CMD                 (UI_16)(55 + CMD_START_BIT)			/* CMD55: �A�v���P�[�V���������R�}���h					*/
#define READ_OCR                (UI_16)(58 + CMD_START_BIT)			/* CMD58: OCR���W�X�^�ǂݏo��							*/
#define CRC_ON_OFF              (UI_16)(59 + CMD_START_BIT)			/* CMD59: CRC�`�F�b�N�@�\�̐؂�ւ�						*/

/* --- Status���W�X�^�̃r�b�g�}�X�N�萔--- */
#define OVERRUN                 (0x8000U)		/* �I�[�o�[����(���C�g�G���[)					*/
#define CMD_PARAM_ERROR         (0x4000U)		/* �p�����^�G���[								*/
#define ADDRESS_ERROR           (0x2000U)		/* �A�h���X�G���[								*/
#define ERASE_SEQ_ERROR         (0x1000U)		/* �����G���[									*/
#define CMD_CRC_ERROR           (0x0800U)		/* �R�}���hCRC�G���[							*/
#define COMUNC_ILLEG_COM        (0x0400U)		/* �s���R�}���h									*/
#define ERASE_RESET             (0x0200U)		/* �������Z�b�g									*/
#define CARD_IS_NOT_READY       (0x0100U)		/* ����s��										*/
#define CMD_PARAM_ERROR2        (0x0080U)		/* �p�����^�G���[2								*/
#define ERASE_PARAM             (0x0040U)		/* �p�����^����									*/
#define WP_VIOLATION            (0x0020U)		/* ���C�g�v���e�N�g�u���b�N�ւ̏�������			*/
#define CARD_READ_FAILURE       (0x0010U)		/* ���[�h�G���[									*/
#define EERROR                  (0x0008U)		/* �ėp�G���[									*/
#define ERROR2                  (0x0004U)		/* �ėp�G���[2									*/
#define WP_ERASE_SKIP           (0x0002U)		/* ���C�g�v���e�N�g�u���b�N�̏����X�L�b�v		*/
#define UNDERRUN                (0x0001U)		/* �A���_�[����(���[�h�G���[)					*/

/* --- OCR ���W�X�^�̃r�b�g�}�X�N�萔--- */
#define OCR_BIT_MASK_BUSY		(0x80000000U)	/* BUSY�r�b�g�}�X�N				*/

/* ----- ���̑��f�[�^----- */
#define DATA_TOKEN_MULTI_SEND   (UI_8)(0xFC)	/* �f�[�^�g�[�N���X�^�[�g�o�C�g(�����Z�N�^���M�p) */
#define DATA_TOKEN_STOP_TRAN    (UI_8)(0xFD)	/* �f�[�^�g�[�N�����M��~(�����Z�N�^���M�p) */
#define DATA_TOKEN              (UI_8)(0xFE)	/* �f�[�^�g�[�N���X�^�[�g�o�C�g */
#define DUMMY_DATA              (UI_8)(0xFF)	/* �_�~�[�f�[�^ */
#define DEFAULT_BLK_LEN         (UI_16)(512)	/* �f�t�H���g �u���b�N�� 512byte */
#define CMD_BYTE_LENGTH         (6U)			/* �R�}���h��  */
#define CSD_BYTE_LENGTH         (17U)			/* CSD�f�[�^�� */
#define CSD_BYTE_SIZE           (64U)			/* CSD�f�[�^�̃o�C�g�T�C�Y */
#define OCR_BYTE_SIZE           (4U)			/* OCR�f�[�^�̃o�C�g�T�C�Y */
#define RESP_DATA_BYTE_SIZE		(4U)			/* ���X�|���X�f�[�^�̃o�C�g�T�C�Y */
#define ERASE_START_SECT        (0UL)			/* �C���[�X�X�^�[�g�Z�N�^�ԍ� */
#define HS_DATA_CNT             (9U)			/* HS�f�[�^�� */
#define SPI_IDLE_LOOP           (0x0200U)		/* SPI_IDLE���[�v�񐔐��� */

/*------------------------------*/
/*           �����萔           */
/*------------------------------*/
/* CRC16 ������p�e�[�u�� CRC-CCITT */
static const UI_16 S_crc16_table_l[ 256 ] = {
	0x0000U, 0x1021U, 0x2042U, 0x3063U, 0x4084U, 0x50A5U, 0x60C6U, 0x70E7U,
	0x8108U, 0x9129U, 0xA14AU, 0xB16BU, 0xC18CU, 0xD1ADU, 0xE1CEU, 0xF1EFU,
	0x1231U, 0x0210U, 0x3273U, 0x2252U, 0x52B5U, 0x4294U, 0x72F7U, 0x62D6U,
	0x9339U, 0x8318U, 0xB37BU, 0xA35AU, 0xD3BDU, 0xC39CU, 0xF3FFU, 0xE3DEU,
	0x2462U, 0x3443U, 0x0420U, 0x1401U, 0x64E6U, 0x74C7U, 0x44A4U, 0x5485U,
	0xA56AU, 0xB54BU, 0x8528U, 0x9509U, 0xE5EEU, 0xF5CFU, 0xC5ACU, 0xD58DU,
	0x3653U, 0x2672U, 0x1611U, 0x0630U, 0x76D7U, 0x66F6U, 0x5695U, 0x46B4U,
	0xB75BU, 0xA77AU, 0x9719U, 0x8738U, 0xF7DFU, 0xE7FEU, 0xD79DU, 0xC7BCU,
	0x48C4U, 0x58E5U, 0x6886U, 0x78A7U, 0x0840U, 0x1861U, 0x2802U, 0x3823U,
	0xC9CCU, 0xD9EDU, 0xE98EU, 0xF9AFU, 0x8948U, 0x9969U, 0xA90AU, 0xB92BU,
	0x5AF5U, 0x4AD4U, 0x7AB7U, 0x6A96U, 0x1A71U, 0x0A50U, 0x3A33U, 0x2A12U,
	0xDBFDU, 0xCBDCU, 0xFBBFU, 0xEB9EU, 0x9B79U, 0x8B58U, 0xBB3BU, 0xAB1AU,
	0x6CA6U, 0x7C87U, 0x4CE4U, 0x5CC5U, 0x2C22U, 0x3C03U, 0x0C60U, 0x1C41U,
	0xEDAEU, 0xFD8FU, 0xCDECU, 0xDDCDU, 0xAD2AU, 0xBD0BU, 0x8D68U, 0x9D49U,
	0x7E97U, 0x6EB6U, 0x5ED5U, 0x4EF4U, 0x3E13U, 0x2E32U, 0x1E51U, 0x0E70U,
	0xFF9FU, 0xEFBEU, 0xDFDDU, 0xCFFCU, 0xBF1BU, 0xAF3AU, 0x9F59U, 0x8F78U,
	0x9188U, 0x81A9U, 0xB1CAU, 0xA1EBU, 0xD10CU, 0xC12DU, 0xF14EU, 0xE16FU,
	0x1080U, 0x00A1U, 0x30C2U, 0x20E3U, 0x5004U, 0x4025U, 0x7046U, 0x6067U,
	0x83B9U, 0x9398U, 0xA3FBU, 0xB3DAU, 0xC33DU, 0xD31CU, 0xE37FU, 0xF35EU,
	0x02B1U, 0x1290U, 0x22F3U, 0x32D2U, 0x4235U, 0x5214U, 0x6277U, 0x7256U,
	0xB5EAU, 0xA5CBU, 0x95A8U, 0x8589U, 0xF56EU, 0xE54FU, 0xD52CU, 0xC50DU,
	0x34E2U, 0x24C3U, 0x14A0U, 0x0481U, 0x7466U, 0x6447U, 0x5424U, 0x4405U,
	0xA7DBU, 0xB7FAU, 0x8799U, 0x97B8U, 0xE75FU, 0xF77EU, 0xC71DU, 0xD73CU,
	0x26D3U, 0x36F2U, 0x0691U, 0x16B0U, 0x6657U, 0x7676U, 0x4615U, 0x5634U,
	0xD94CU, 0xC96DU, 0xF90EU, 0xE92FU, 0x99C8U, 0x89E9U, 0xB98AU, 0xA9ABU,
	0x5844U, 0x4865U, 0x7806U, 0x6827U, 0x18C0U, 0x08E1U, 0x3882U, 0x28A3U,
	0xCB7DU, 0xDB5CU, 0xEB3FU, 0xFB1EU, 0x8BF9U, 0x9BD8U, 0xABBBU, 0xBB9AU,
	0x4A75U, 0x5A54U, 0x6A37U, 0x7A16U, 0x0AF1U, 0x1AD0U, 0x2AB3U, 0x3A92U,
	0xFD2EU, 0xED0FU, 0xDD6CU, 0xCD4DU, 0xBDAAU, 0xAD8BU, 0x9DE8U, 0x8DC9U,
	0x7C26U, 0x6C07U, 0x5C64U, 0x4C45U, 0x3CA2U, 0x2C83U, 0x1CE0U, 0x0CC1U,
	0xEF1FU, 0xFF3EU, 0xCF5DU, 0xDF7CU, 0xAF9BU, 0xBFBAU, 0x8FD9U, 0x9FF8U,
	0x6E17U, 0x7E36U, 0x4E55U, 0x5E74U, 0x2E93U, 0x3EB2U, 0x0ED1U, 0x1EF0U,
};

/*------------------------------*/
/*           �����ϐ�           */
/*------------------------------*/
static T_CARD_INFO	S_card_info;

/* ���W�X�^ */
static __io volatile UI_8 *volatile const C_SdReg_SMR = &(UART8_CSIO8_SMR8);			/* �V���A�����[�h���W�X�^			*/
static __io volatile UI_8 *volatile const C_SdReg_SCR = &(UART8_CSIO8_SCR8);			/* �V���A�����䃌�W�X�^				*/
static __io volatile UI_8 *volatile const C_SdReg_SSR = &(UART8_CSIO8_SSR8);			/* �V���A���X�e�[�^�X���W�X�^		*/
static __io volatile UI_8 *volatile const C_SdReg_ESCR = &(UART8_CSIO8_ESCR8);			/* �g���ʐM���䃌�W�X�^				*/
static __io volatile UI_8 *volatile const C_SdReg_TDR = &(UART8_CSIO8_TDR8_TDR8L);		/* ���M�f�[�^���W�X�^				*/
static __io volatile UI_8 *volatile const C_SdReg_RDR = &(UART8_CSIO8_RDR8_RDR8L);		/* ��M�f�[�^���W�X�^				*/
static __io volatile UI_16 *volatile const C_SdReg_BGR = &(UART8_CSIO8_BGR8);			/* �{�[���[�g�W�F�l���[�^���W�X�^	*/
static __io volatile UI_8 *volatile const C_SdReg_FCR0 = &(UART8_CSIO8_FCR08);			/* FIFO���䃌�W�X�^0				*/
static __io volatile UI_8 *volatile const C_SdReg_FCR1 = &(UART8_CSIO8_FCR18);			/* FIFO���䃌�W�X�^1				*/
static __io volatile UI_16 *volatile const C_SdReg_FBYTE = &(UART8_CSIO8_FBYTE8);		/* FIFO�o�C�g���W�X�^				*/

/*------------------------------*/
/*         �����֐��錾         */
/*------------------------------*/
static void			S_CS_OnOff( UI_8 );
static B_8			S_Card_Exist(void);
static UI_8			S_Card_Protect(void);
static void			S_CardInfo_Init(void);
static void			S_UartReg_Init(void);
static void			S_Set_BaudRate( UI_16 );
static E_SPIERROR	S_Set_SPIMode(void);
static E_SPIERROR	S_CheckSdIfCnd(void);
static SI_8			S_CheckSPIReady( E_SD_INIT_TYPE );
static E_SPIERROR	S_Set_CrcOnOff( UI_8 );
static void			S_StartSD80Clocks(void);
static void			S_Wait_1ms(void);
static E_SPIERROR	S_Receive_Data( UI_8 *, UI_16 );
static E_SPIERROR	S_Get_CSD( UI_8 * );
static SI_8			S_Get_AddressingMode( E_SD_ADR_MODE* );
static E_SPIERROR	S_Get_OCR( UI_8 * );
static E_SPIERROR	S_Set_CardInfo( UI_8 *, E_SD_ADR_MODE, T_CARD_INFO * );
static E_SPIERROR	S_Set_BlockLen( UI_16 );
static E_SPIERROR	S_CommandAndResponse( UI_16, UI_32, E_RESP_TYPE );
static E_SPIERROR	S_PrepareAndSetup( UI_16, UI_32 );
static SI_8			S_SendCommand( UI_16, UI_32 );
static SI_8			S_SPIExchgData( UI_8, UI_8 * );
static E_SPIERROR	S_WaitForCmdResponse( E_RESP_TYPE );
static E_SPIERROR	S_Get_ResponseInfo( UI_16 *, E_RESP_TYPE );
static E_SPIERROR	S_Send_Data( const UI_8 *, UI_16, UI_8 );
static E_SPIERROR	S_CRCErrOnWrite(void);
static SI_8			S_CheckCardBusy( UI_8 curCsStat );
static E_SPIERROR	S_Read_SingleBlock( UI_8 *pBuff, UI_32 ulSectorNo );
static E_SPIERROR	S_Read_MultiBlock( UI_8 *pBuff, UI_32 ulSectorNo, UI_32 uSectorCnt );
static E_SPIERROR	S_Write_SingleBlock( const UI_8 *pBuff, UI_32 ulSectorNo );
static E_SPIERROR	S_Write_MultiBlock( const UI_8 *pBuff, UI_32 ulSectorNo, UI_32 uSectorCnt );
static UI_8			S_SdDrv_CalcCmdCrc7( const UI_8 *pBuff );
static UI_16		S_SdDrv_CalcDataCrc16(const UI_8 *pBuff, UI_32 len);


/*------------------------------*/
/*         �O��I/F�֐�          */
/*------------------------------*/
/********************************************************************/
/* Name          : SDCardDrv_Init                                   */
/* Description   : SD�J�[�h�h���C�o�̏����ݒ�                       */
/* Argument      : �Ȃ�                                             */
/* Return value  : �Ȃ�                                             */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/10/01                                       */
/* Note          :                                                  */
/********************************************************************/
void SDCardDrv_Init( void )
{
	S_CardInfo_Init();
}

/********************************************************************/
/* Name          : SDCardDrv_SdInit                                 */
/* Description   : SD�J�[�h�C���^�[�t�F�[�X�̏����ݒ�               */
/* Argument      : �Ȃ�                                             */
/* Return value  : SDCARDDRV_NOERROR      �G���[�Ȃ�                */
/*                 SDCARDDRV_NOCARD       �J�[�h�Ȃ�                */
/*                 SDCARDDRV_CARDERROR    �J�[�h�G���[              */
/*                 SDCARDDRV_FORMATERROR  �t�H�[�}�b�g�G���[        */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/10/05                                       */
/* Note          :                                                  */
/********************************************************************/
E_SDDRRET SDCardDrv_SdInit( void )
{
	E_SPIERROR		resErr;						/* SPI�G���[�߂�l�p�ϐ� */
	E_SD_INIT_TYPE	initType;
	UI_8 bufCSD[CSD_BYTE_SIZE];					/* CSD�f�[�^�p�z�� */
	E_SD_ADR_MODE	adrMode;
	UI_8 loop_end_flg;							/* ���[�v�I���t���O */
	UI_16 i;

	/* �J�[�h���\���̏����� */
	S_CardInfo_Init();

	/* �J�[�h�L���`�F�b�N */
	if ( S_Card_Exist() == D_FALSE ) {
		return SDCARDDRV_NOCARD;		/* �J�[�h���� */
	}

	/* UART���W�X�^�����ݒ� */
	S_UartReg_Init();

	/* SPI���[�h�ɐݒ�(CMD0���M)  */
	resErr = S_Set_SPIMode();			
	if ( resErr != SPI_CARD_IS_NOT_READY ) {
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit CMD0 ERR!! ret:%d\n", resErr);)
		return SDCARDDRV_CARDERROR;
	}

	/* �C���^�[�t�F�[�X�����m�F(CMD8���M) */
	resErr = S_CheckSdIfCnd();
	if (resErr == SPI_NO_ERROR) {		/* �C���^�[�t�F�[�X������v */
		/* SD Ver2.0�ȍ~ */
		initType = SD_INIT_TYPE_SD_VER2;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit VER2.0 or later\n");)
	}
	else if ((resErr == SPI_CARD_IS_NOT_RESPONDING) ||	/* SD����̉����Ȃ�				*/
			 (resErr == SPI_COMUNC_ILLEG_COM)) {		/* �R�}���h(CMD8)���Ή�			*/
		/* SD Ver2.0�ȑO �܂��� SD�ȊO	*/
		initType = SD_INIT_TYPE_SD_VER1;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit VER1.X or Not Sd\n");)
	}
	else {
		return SDCARDDRV_CARDERROR;
	}

	/* SPI���[�h�I�y���[�V�������f�B�`�F�b�N(ACMD41���M) */
	i = 1000;
	loop_end_flg = D_OFF;
	while (( i > 0 ) && ( loop_end_flg == D_OFF )) {
		if ( S_CheckSPIReady(initType) == D_OK ) {
			/* SPI���[�h�I�y���[�V�������f�B��� */
			loop_end_flg = D_ON;
			OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit SPI Ready OK cnt:%d\n", i);)
		}
		else {
			i--;
			S_Wait_1ms();				/* �����S_CheckSPIReady�܂�1ms�̃f�B���C�������� */
		}
	}
	if ( loop_end_flg == D_OFF ) {		/* �^�C���A�E�g */
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit ACMD41 ERR!!\n");)
		return SDCARDDRV_CARDERROR;
	}

	/* CRC�`�F�b�NON(CMD59���M)*/
	resErr = S_Set_CrcOnOff(D_ON);
	if ( resErr != SPI_NO_ERROR ) {
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit S_Set_CrcOnOff ERR!! ret:%d\n", resErr);)
		return SDCARDDRV_CARDERROR;
	}

	/* SD�J�[�h�A�h���b�V���O���[�h�擾(OCR�f�[�^�擾)(CMD58���M)*/
	if (initType == SD_INIT_TYPE_SD_VER2) {
		if (S_Get_AddressingMode( &adrMode ) != D_OK) {
			OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit CMD58 ERR!!\n");)
			return SDCARDDRV_FORMATERROR;	/* �t�H�[�}�b�g�G���[ */
		}
	}
	else {
		adrMode = SD_ADR_MODE_BYTE;
	}

	/* CSD�f�[�^�擾(CMD9���M) */
	resErr = S_Get_CSD( bufCSD );
	if ( resErr != SPI_NO_ERROR) {
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit CMD9 ERR!! ret:%d\n", resErr);)
		return SDCARDDRV_FORMATERROR;	/* �t�H�[�}�b�g�G���[ */
	}

	/* �J�[�h���ݒ�(SD VER1��CMD16�ŃZ�N�^�T�C�Y512��ݒ�) */
	resErr = S_Set_CardInfo( bufCSD, adrMode, &S_card_info );
	if ( resErr != SPI_NO_ERROR ) {
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit S_Set_CardInfo ERR!! ret:%d\n", resErr);)
		return SDCARDDRV_CARDERROR;
	}

	/* �{�[���[�g�ݒ� */
	S_Set_BaudRate( SCK_16MHZ_BR_500KBPS );

	return SDCARDDRV_NOERROR;
}

/********************************************************************/
/* Name          : SDCardDrv_Read                                   */
/* Description   : �J�[�h�f�[�^���[�h(�Z�N�^�P��)                   */
/* Argument      : *pBuff       �ǂݏo�����Z�N�^���̃f�[�^���i�[    */
/*                 ulSectorNo   �Z�N�^�ԍ�                          */
/*                 ulSectorCnt  �Z�N�^��                            */
/* Return value  : SDCARDDRV_NOERROR      �G���[�Ȃ�                */
/*                 SDCARDDRV_NOCARD       �J�[�h�Ȃ�                */
/*                 SDCARDDRV_READERROR    �ǂݏo���G���[            */
/*                 SDCARDDRV_BUSY         BUSY���                  */
/*                 SDCARDDRV_ERROR        ���̑��̃G���[            */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/10/01                                       */
/* Note          : �w�肳�ꂽ�Z�N�^����f�[�^�ǂݏo��               */
/********************************************************************/
E_SDDRRET SDCardDrv_Read( UI_8 *pBuff, UI_32 ulSectorNo, UI_32 ulSectorCnt )
{
	E_SDDRRET ret = SDCARDDRV_CARDERROR;	/* �߂�l�p�ϐ� */
	E_SPIERROR resErr;		/* SPI�G���[�߂�l�p�ϐ� */
	UI_32 total_Sect;		/* �S�Z�N�^��   */

	total_Sect = S_card_info.ulTotal_Secotors;

	if ( S_Card_Exist() == D_FALSE ) {	/* �J�[�h�L���`�F�b�N */
		ret = SDCARDDRV_NOCARD;			/* �J�[�h���� */
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Read ERR!!!! NOCARD, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else if ( pBuff == D_NULL ) {		/* ��1�����`�F�b�N */
		ret = SDCARDDRV_ERROR;
	}
	else if ( total_Sect == 0UL ) {		/* �S�Z�N�^���`�F�b�N */
		ret = SDCARDDRV_READERROR;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Read ERR!!!! Sector=0, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else if ( (ulSectorCnt == 0UL) ||
			(ulSectorNo >= total_Sect) ||
			((ulSectorNo + ulSectorCnt) > total_Sect) ) {	/* ��2,��3�����`�F�b�N */
		ret = SDCARDDRV_READERROR;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Read ERR!!!! SectorErr, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else if ( S_CheckCardBusy( D_OFF ) == D_NG ) {
		/* Busy��� */
		ret = SDCARDDRV_BUSY;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Read ERR!!!! Busy, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else {
		/* �����`�F�b�NOK */
		ret = SDCARDDRV_READERROR;
		if (ulSectorCnt == 1) {
			/* 1�Z�N�^���[�h */
			resErr = S_Read_SingleBlock(pBuff, ulSectorNo);
		}
		else {
			/* �}���`�Z�N�^���[�h */
			resErr = S_Read_MultiBlock(pBuff, ulSectorNo, ulSectorCnt);
		}
		if ( resErr == SPI_NO_ERROR ) {		/* �G���[���� */
			ret = SDCARDDRV_NOERROR;		/* �G���[�Ȃ� */
		}
		else {
			OUTLOG(OutputLog("[SDDRV]SDCardDrv_Read ERR!!!! err:%d, ofssect:%u, sectCnt:%u\n", resErr, ulSectorNo, ulSectorCnt);)
		}
	}
	return ret;
}


/********************************************************************/
/* Name          : SDCardDrv_Write                                  */
/* Description   : �J�[�h�f�[�^���C�g(�Z�N�^�P��)                   */
/* Argument      : *pBuff       �������ݗp�f�[�^���i�[����Ă���    */
/*                 ulSectorNo   �Z�N�^�ԍ�                          */
/*                 ulSectorCnt  �Z�N�^��                            */
/* Return value  : SDCARDDRV_NOERROR      �G���[�Ȃ�                */
/*                 SDCARDDRV_NOCARD       �J�[�h�Ȃ�                */
/*                 SDCARDDRV_WRITEPROHIB  �������݋֎~              */
/*                 SDCARDDRV_WRITEERROR   �������݃G���[            */
/*                 SDCARDDRV_BUSY         BUSY���                  */
/*                 SDCARDDRV_ERROR        ���̑��̃G���[            */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/10/01                                       */
/* Note          : �w�肳�ꂽ�Z�N�^�Ƀf�[�^��������                 */
/********************************************************************/
E_SDDRRET SDCardDrv_Write( const UI_8 *pBuff, UI_32 ulSectorNo, UI_32 ulSectorCnt )
{
	E_SDDRRET ret = SDCARDDRV_CARDERROR;	/* �߂�l�p�ϐ� */
	E_SPIERROR resErr;		/* SPI�G���[�߂�l�p�ϐ� */
	UI_32 total_Sect;		/* �S�Z�N�^��   */

	total_Sect = S_card_info.ulTotal_Secotors;

	if ( S_Card_Exist() == D_FALSE ) {		/* �J�[�h�L���`�F�b�N */
		ret = SDCARDDRV_NOCARD;				/* �J�[�h���� */
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Write ERR!!!! NOCARD, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else if ( S_Card_Protect() == D_ON ) {	/* �v���e�N�g�`�F�b�N */
		ret = SDCARDDRV_WRITEPROHIB;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Write ERR!!!! Protect, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else if ( pBuff == D_NULL ) {			/* ��1�����`�F�b�N */
		ret = SDCARDDRV_ERROR;
	}
	else if ( total_Sect == 0UL ) {			/* �S�Z�N�^���`�F�b�N */
		ret = SDCARDDRV_WRITEERROR;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Write ERR!!!! Sector=0, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else if ( (ulSectorCnt == 0UL) ||
			(ulSectorNo >= total_Sect) ||
			((ulSectorNo + ulSectorCnt) > total_Sect) ) {	/* ��2,��3�����`�F�b�N */
		ret = SDCARDDRV_WRITEERROR;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Write ERR!!!! SectorErr, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else if ( S_CheckCardBusy( D_OFF ) == D_NG ) {
		/* Busy��� */
		ret = SDCARDDRV_BUSY;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Write ERR!!!! Busy, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else {
		/* �����`�F�b�NOK */
		ret = SDCARDDRV_WRITEERROR;
		if (ulSectorCnt == 1) {
			/* 1�Z�N�^���C�g */
			resErr = S_Write_SingleBlock(pBuff, ulSectorNo);
		}
		else {
			/* �}���`�Z�N�^���C�g */
			resErr = S_Write_MultiBlock(pBuff, ulSectorNo, ulSectorCnt);
		}
		if ( resErr == SPI_NO_ERROR ) {		/* �G���[���� */
			ret = SDCARDDRV_NOERROR;		/* �G���[�Ȃ� */
		}
		else {
			OUTLOG(OutputLog("[SDDRV]SDCardDrv_Write ERR!!!! err:%d, ofssect:%u, sectCnt:%u\n", resErr, ulSectorNo, ulSectorCnt);)
		}
	}
	return ret;
}


/********************************************************************/
/* Name          : SDCardDrv_GetCardInfo                            */
/* Description   : �J�[�h���̎擾                                 */
/* Argument      : *pCardInfo    �J�[�h�����i�[���邽�߂̗̈�     */
/* Return value  : SDCARDDRV_NOERROR      �G���[�Ȃ�                */
/*                 SDCARDDRV_NOCARD       �J�[�h�Ȃ�                */
/*                 SDCARDDRV_ERROR        ���̑��̃G���[            */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
E_SDDRRET SDCardDrv_GetCardInfo( T_CARD_INFO *pCardInfo )
{
	E_SDDRRET ret = SDCARDDRV_CARDERROR;	/* �߂�l�p�ϐ� */
	B_8 card_exist;			/* �J�[�h�L��	*/

	if ( pCardInfo == D_NULL ) {		/* ��1�����`�F�b�N */
		ret = SDCARDDRV_ERROR;
	}
	else {
		/* �J�[�h�L�����̎擾�ƃZ�b�g */
		card_exist = S_Card_Exist();
		S_card_info.bCardExist = card_exist;

		if ( card_exist == D_FALSE ) {		/* �J�[�h�L���`�F�b�N */
			/* �J�[�h���̏ꍇ */
			/* �J�[�h���ݒ� */
			S_card_info.uProtectState = D_OFF;
			S_card_info.ulTotal_Secotors = 0UL;
			S_card_info.AdrMode = SD_ADR_MODE_UNKNOWN;

			ret = SDCARDDRV_NOCARD;
		}
		else {
			/* �J�[�h�L�̏ꍇ */
			/* �����������Ŏ擾���ꂽ�J�[�h����ݒ� */
			pCardInfo -> bCardExist = S_card_info.bCardExist;
			pCardInfo -> uProtectState = S_card_info.uProtectState;
			pCardInfo -> ulTotal_Secotors = S_card_info.ulTotal_Secotors;
			pCardInfo -> AdrMode = S_card_info.AdrMode;
			
			ret = SDCARDDRV_NOERROR;
		}
	}
	return ret;
}


/*------------------------------*/
/*           �����錾           */
/*------------------------------*/
/********************************************************************/
/* Name          : S_CS_OnOff                                       */
/* Description   : �`�b�v�Z���N�g�ݒ�                               */
/* Argument      : opt          �`�b�v�Z���N�gON (D_ON)             */
/*                                            OFF(D_OFF)            */
/* Return value  : �Ȃ�                                             */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static void S_CS_OnOff( UI_8 opt )
{

	if ( opt == D_ON ) {
		/* �`�b�v�Z���N�gON */
		(void)PortDrv_SetDataBit(IO_PO_GR_SD_SPI_CS, IO_PO_BIT_SD_SPI_CS, IO_PO_LEV_ACTIVE_SD_SPI_CS);
	}
	else if ( opt == D_OFF ) {
		/* �`�b�v�Z���N�gOFF */
		(void)PortDrv_SetDataBit(IO_PO_GR_SD_SPI_CS, IO_PO_BIT_SD_SPI_CS, IO_PO_LEV_NONACT_SD_SPI_CS);
	}
	else {
	}

}

/********************************************************************/
/* Name          : S_Card_Exist                                     */
/* Description   : �J�[�h�L������                                   */
/* Argument      : �Ȃ�                                             */
/* Return value  : D_TRUE       �J�[�h�L                            */
/*                 D_FALSE      �J�[�h��                            */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static B_8 S_Card_Exist( void )
{

	B_8  ret;		/* �߂�l�p�ϐ� */
	UI_8 port_lev;
	SI_8 ret_port;

	ret_port = PortDrv_GetDataBit(IO_PI_GR_SD_DTCT, IO_PI_BIT_SD_DTCT, &port_lev);
	if (ret_port != D_OK) {
		OUTLOG(OutputLog("[SDDRV]S_Card_Exist PortDrv_GetDataBit ERR!!!!\n");)
		ret = D_FALSE;			/* SD�J�[�h�Ȃ� */
		S_CardInfo_Init();
	}
	else {
		if (port_lev == IO_PI_LEV_ACTIVE_SD_DTCT) {
			/* OUTLOG(OutputLog("[SDDRV]S_Card_Exist SD EXIST\n");) */
			ret = D_TRUE;		/* SD�J�[�h���o */
		}
		else {
			/* OUTLOG(OutputLog("[SDDRV]S_Card_Exist SD NON\n");) */
			ret = D_FALSE;		/* SD�J�[�h�Ȃ� */
			S_CardInfo_Init();
		}
	}

	return ret;

}

/********************************************************************/
/* Name          : S_Card_Protect                                   */
/* Description   : �J�[�h�v���e�N�g�L������                         */
/* Argument      : �Ȃ�                                             */
/* Return value  : D_ON         �v���e�N�g�L                        */
/*                 D_OFF        �v���e�N�g��                        */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static UI_8 S_Card_Protect( void )
{

	UI_8 ret;		/* �߂�l�p�ϐ� */
	UI_8 port_lev;
	SI_8 ret_port;

	ret_port = PortDrv_GetDataBit(IO_PI_GR_SD_WP, IO_PI_BIT_SD_WP, &port_lev);
	if (ret_port != D_OK) {
		OUTLOG(OutputLog("[SDDRV]S_Card_Protect PortDrv_GetDataBit ERR!!!!\n");)
		ret = D_OFF;			/* �v���e�N�g�� */
	}
	else {
		if (port_lev == IO_PI_LEV_ACTIVE_SD_WP) {
			/* OUTLOG(OutputLog("[SDDRV]S_Card_Protect SD Write Protect OFF\n");) */
			ret = D_OFF;		/* �v���e�N�g�� */
		}
		else {
			/* OUTLOG(OutputLog("[SDDRV]S_Card_Protect SD Write Protect ON\n");) */
			ret = D_ON;			/* �v���e�N�g�L */
		}
	}

	return ret;

}

/********************************************************************/
/* Name          : S_CardInfo_Init                                  */
/* Description   : �J�[�h���\���̏�����                           */
/* Argument      : �Ȃ�                                             */
/* Return value  : �Ȃ�                                             */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static void S_CardInfo_Init( void )
{

	/* �J�[�h���̍\���̂������� */
	S_card_info.bCardExist = D_FALSE;
	S_card_info.uProtectState = D_OFF;
	S_card_info.ulTotal_Secotors = 0UL;
	S_card_info.AdrMode = SD_ADR_MODE_UNKNOWN;

}

/********************************************************************/
/* Name          : S_UartReg_Init                                   */
/* Description   : LIN-UART���W�X�^�����ݒ�                         */
/* Argument      : �Ȃ�                                             */
/* Return value  : �Ȃ�                                             */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static void S_UartReg_Init( void )
{

	UI_8	smr_work;			/* �V���A�����[�h���W�X�^���Z�pwork�ϐ� */
	UI_8	scr_work;			/* �V���A�����䃌�W�X�^�ꎞ�i�[�pwork�ϐ� */
	UI_8	clr_work;			/* �r�b�g�N���A�pwork�ϐ� */
	UI_8	fcr0_work;			/* FIFO���䃌�W�X�^0�ݒ�pwork�ϐ� */
	UI_8	tmp_scr_mask;		/* ����M�֎~�y�ё���M�����݋֎~�ݒ�pwork�ϐ� */

	/* ����M�֎~�y�ё���M�����݋֎~��ݒ�O�ɃV���A���f�[�^�o�͂��֎~�ɐݒ肷�� */
	/* (SOT�|�[�g�̕E��p���X�̔����h�~) */
	smr_work		= *C_SdReg_SMR;
	smr_work		&= SMR_SOE_DISABLE;
	*C_SdReg_SMR	= smr_work;

	/* �v���O���}�u���N���A�̎��s�O�ɑ���M�֎~�y�ё���M�����݋֎~��ݒ肷�� */
	tmp_scr_mask =	(UI_8)( ~( SCR_RIE_BIT | SCR_TIE_BIT | SCR_RXE_BIT | SCR_TXE_BIT ) );
	scr_work		= *C_SdReg_SCR;
	scr_work		&= tmp_scr_mask;
	*C_SdReg_SCR	= scr_work;

	/* �v���O���}�u���N���A�̎��s�O��FIFO1�y��FIFO2���֎~�ɐݒ肷�� */
	/* (FIFO�͎g�p���Ȃ��v�z�����֎~�̐ݒ���ɏW�����čs�����j) */
	fcr0_work		= *C_SdReg_FCR0;
	fcr0_work		&= FCR0_FE_DISABLE;
	*C_SdReg_FCR0	= fcr0_work;

	/* �����[�h�l�𑦗L���ɂ��邽�߂Ƀv���O���}�u���N���A�̎��s�O��BGR���W�X�^��ݒ肷�� */
	S_Set_BaudRate( SCK_16MHZ_BR_400KBPS );

	/* �v���O���}�u���N���A */
	clr_work		= *C_SdReg_SCR;
	clr_work		|= SCR_UPCL_SET;
	*C_SdReg_SCR	= clr_work;

	/* SMR���W�X�^:SCKE�ȊO�̃r�b�g���ɐݒ� */
	/* ���̃��W�X�^��ݒ肷��O��MD�r�b�g��ݒ肷��K�v������ */
	smr_work		= *C_SdReg_SMR;
	smr_work		&= SMR_SCKE_MASK;
	smr_work		|= (UI_8)(INIT_SMR & SMR_SCKE_DISABLE);
	*C_SdReg_SMR	= smr_work;
	
	/* SMR���W�X�^:SCKE�r�b�g���܂߂čēx�ݒ� */
	/* (SOT�|�[�g�̕E��p���X�̔����h�~) */
	*C_SdReg_SMR	= INIT_SMR;

	/* ���̃��W�X�^��SMR���W�X�^��MD�r�b�g��ݒ肵����ɐݒ肷�� */
	/* (�ݒ菇���͓���J�n�v���ƍ��킹��) */
	*C_SdReg_FCR1	= INIT_FCR1;
	*C_SdReg_FCR0	= INIT_FCR0;
	*C_SdReg_FBYTE	= INIT_FBYTE;
	*C_SdReg_SSR	= INIT_SSR;
	*C_SdReg_ESCR	= INIT_ESCR;

	/* ���̃��W�X�^��S�Đݒ肵�Ă���V���A�����䃌�W�X�^��ݒ肷�� */
	*C_SdReg_SCR	= INIT_SCR;

	return;

}

/********************************************************************/
/* Name          : S_Set_BaudRate                                   */
/* Description   : �{�[���[�g�W�F�l���[�^�ݒ�                       */
/* Argument      : baudrate     �����[�h�l                          */
/* Return value  : �Ȃ�                                             */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          : �N���b�N�̐ݒ�ɒ���                             */
/********************************************************************/
static void S_Set_BaudRate( UI_16 baudrate )
{

	*C_SdReg_BGR = baudrate;			/* �{�[���[�g�W�F�l���[�^ */

}


/********************************************************************/
/* Name          : S_Set_SPIMode                                    */
/* Description   : SPI���[�h�ݒ�                                    */
/* Argument      : �Ȃ�                                             */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/10/05                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Set_SPIMode( void )
{

	E_SPIERROR resErr;		/* �߂�l�p�ϐ� */
	UI_8  j;

	S_StartSD80Clocks();	/* 80clock	*/

	S_CS_OnOff( D_ON );

	resErr = S_CommandAndResponse (			/* CMD0(GO_IDLE_STATE) */
				GO_IDLE_STATE,
				0UL,
				R1 );
	(void)S_SPIExchgData( DUMMY_DATA, &j );

	/* ���̎��_�ł́AIn Idle State���X�|���X���Ԃ��Ă���͂� */
	if ( resErr != SPI_CARD_IS_NOT_READY ) {
		resErr = SPI_INTERFACE_ERROR;
		OUTLOG(OutputLog("S_Set_SPIMode S_CommandAndResponse ERR!! :%d\n", resErr);)
	}

	S_CS_OnOff( D_OFF );

	return resErr;

}

/********************************************************************/
/* Name          : S_CheckSdIfCnd                                   */
/* Description   : SD�J�[�h�C���^�[�t�F�[�X�����m�F                 */
/* Argument      : �Ȃ�                                             */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/MM/DD                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_CheckSdIfCnd( void )
{

	E_SPIERROR resErr;		/* �߂�l�p�ϐ� */
	UI_8  bufRespR7[RESP_DATA_BYTE_SIZE];
	UI_8  i;
	UI_8  j;

	S_CS_OnOff( D_ON );

	resErr = S_CommandAndResponse (	/* CMD8(SEND_IF_COND) */
				SEND_IF_COND,
				0x000001AA,		/* [31:12] �\��									*/
								/* [11: 8] �d������(VHS) :0001					*/
								/*			0000:Not Defined					*/
								/*			0001:2.7-3.6V						*/
								/*			0010:Reserved for Low Voltage Range	*/
								/*			0100:Reserved						*/
								/*			1000:Reserved						*/
								/*			Others Not Defined					*/
								/* [ 7: 0] �`�F�b�N�p�^�[�� :10101010			*/
				R1 );

	if (resErr == SPI_CARD_IS_NOT_READY) {
		/* R7�̃��X�|���X�f�[�^���擾 */
		for (i = 0; i < RESP_DATA_BYTE_SIZE; i++) {
			if (S_SPIExchgData( DUMMY_DATA, &bufRespR7[i] ) == D_NG) {
				break;
			}
		}
		if (i == RESP_DATA_BYTE_SIZE) {	/* ���X�|���X�f�[�^�擾���� */
			if ((bufRespR7[2] == 0x01) &&	/* �d������ 2.7-3.6V OK			*/
				(bufRespR7[3] == 0xAA)) {	/* �`�F�b�N�p�^�[����v			*/
				resErr = SPI_NO_ERROR;
			}
			else {
				resErr = SPI_ERROR;
			}
		}
		else {
			resErr = SPI_ERROR;
		}
	}

	(void)S_SPIExchgData( DUMMY_DATA, &j );
	
	S_CS_OnOff( D_OFF );

	return resErr;

}

/********************************************************************/
/* Name          : S_CheckSPIReady                                  */
/* Description   : SPI���[�h�I�y���[�V�������f�B�`�F�b�N            */
/* Argument      : �Ȃ�                                             */
/* Return value  : D_OK         Ready���                           */
/*                 D_NG         Idle���(���̓G���[)                */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/10/05                                       */
/* Note          :                                                  */
/********************************************************************/
static SI_8 S_CheckSPIReady( E_SD_INIT_TYPE initType )
{

	SI_8 ret;				/* �߂�l�p�ϐ� */
	E_SPIERROR resErr;		/* �߂�l�p�ϐ� */
	UI_32 arg;
	UI_8  j;

	S_CS_OnOff( D_ON );

	ret = D_NG;

	(void)S_CommandAndResponse (			/* CMD55(APP_CMD) */
				APP_CMD,
				0UL,
				R1 );
	(void)S_SPIExchgData( DUMMY_DATA, &j );

	if (initType == SD_INIT_TYPE_SD_VER2) {
		arg = 0x40000000UL;
	}
	else {
		arg = 0UL;
	}
	resErr = S_CommandAndResponse (			/* CMD41(SEND_OP_COND_ACMD41) */
				SEND_OP_COND_ACMD41,
				arg,
				R1 );
	(void)S_SPIExchgData( DUMMY_DATA, &j );
	if ( resErr == SPI_NO_ERROR ) {			/* �G���[���� */
		/* Ready���(SPI_CARD_IS_NOT_READY��SPI_NO_ERROR�ɕς��) */
		ret = D_OK;
	}
	
	S_CS_OnOff( D_OFF );

	return ret;

}

/********************************************************************/
/* Name          : S_Set_CrcOnOff                                   */
/* Description   : CRC�`�F�b�N�ݒ�                                  */
/* Argument      : OnOff	CRC�`�F�b�N ON:D_ON, OFF:D_OFF          */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/12/21                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Set_CrcOnOff( UI_8 OnOff )
{

	E_SPIERROR	resErr;		/* �߂�l�p�ϐ� */
	UI_32		arg;
	UI_8		data;

	S_CS_OnOff( D_ON );

	if (OnOff == D_ON) {
		arg = 1;
	}
	else {
		arg = 0;
	}

	resErr = S_CommandAndResponse (	/* CMD59(CRC_ON_OFF) */
				CRC_ON_OFF,
				arg,
				R1 );
	(void)S_SPIExchgData( DUMMY_DATA, &data );
	
	S_CS_OnOff( D_OFF );

	return resErr;

}

/********************************************************************/
/* Name          : S_StartSD80Clocks                                */
/* Description   : 80�N���b�N�o��                                   */
/* Argument      : �Ȃ�                                             */
/* Return value  : �Ȃ�                                             */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static void S_StartSD80Clocks( void )
{

	UI_16 i;
	UI_8  j;

	/* �J�[�h������80�N���b�N���o�� */
	for ( i = 0; i < 10; i++ ) {			/* 8bit * 10 = 80 */
		(void)S_SPIExchgData( DUMMY_DATA, &j );
	}

}


/********************************************************************/
/* Name          : S_Wait_1ms                                       */
/* Description   : 1ms�҂�                                          */
/* Argument      : �Ȃ�                                             */
/* Return value  : �Ȃ�                                             */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static void S_Wait_1ms( void )
{

	UI_16 i;
	UI_8  j;

	/* ��1ms�҂� */
	for ( i = 0; i < 30; i++ ) {
		(void)S_SPIExchgData( DUMMY_DATA, &j );
	}

}


/********************************************************************/
/* Name			: S_Receive_Data									*/
/* Description	: �f�[�^�g�[�N����M								*/
/* Argument		: pBuff			��M�f�[�^�i�[�̈�					*/
/*				  dLength		��M�f�[�^��						*/
/* Return value	: E_SPIERROR	�G���[���b�Z�[�W					*/
/* Programmed by: H.Mitsumoto										*/
/* Date			: 2004/06/17										*/
/* Update		: 2004/06/29										*/
/* Note			: �g�[�N���҂�������0x0032����0x0064�ɕύX			*/
/********************************************************************/
static E_SPIERROR S_Receive_Data( UI_8 *pBuff, UI_16 dLength )
{

	UI_8 *buff_ptr;			/* ��M�f�[�^�p�̈�     */
	UI_8 dToken;			/* �f�[�^�g�[�N���p�̈� */
	UI_8 wk_ssr;			/* ��Ɨp�ϐ�           */
	volatile UI_8 wk_rdr;	/* ��Ɨp�ϐ�           */
	UI_16 i;
	UI_32 startTime1ms;
	B_8 timeOut;
	UI_16	crc;
	UI_16	crc_calc;

	if ( pBuff == D_NULL ) {		/* ��1�����`�F�b�N */
		return SPI_ERROR;
	}

	/* �f�[�^�g�[�N����M��҂� */
	timeOut = D_TRUE;
	startTime1ms = Timer_ctrl_GetTime1ms();
	while (Timer_ctrl_TimeCompare1ms(startTime1ms, 200) == N_NG) {	/* �^�C���A�E�g200ms */
		/* �J�[�h�L���`�F�b�N */
		if ( S_Card_Exist() == D_FALSE ) {
			OUTLOG(OutputLog("[SDDRV]S_Receive_Data SD Card Non!!\n");)
			return SPI_INTERFACE_ERROR;
		}

		/* �f�[�^�g�[�N���E�X�^�[�g�o�C�g��M�𔻒� */
		if ( S_SPIExchgData( DUMMY_DATA, &dToken ) == D_NG ) {
			OUTLOG(OutputLog("[SDDRV]S_Receive_Data StartToken ERR!!\n");)
			return SPI_INTERFACE_ERROR;
		}

		if ( dToken == DATA_TOKEN ) {
			/* �f�[�^�g�[�N����M���� */
			timeOut = D_FALSE;
			break;
		}
		else if (dToken == 0xFF) {
			/* NOP */
		}
		else {
			/* �擾�f�[�^�s��	*/
			OUTLOG(OutputLog("[SDDRV]RcvDataErr!! 0x%02X\n", dToken);)

			/* SD����f�[�^�������Ă��邽�ߋ�ǂ݂���	*/
			for ( i = 0; i < dLength ; i++ ) {
				(void)S_SPIExchgData( DUMMY_DATA, &dToken );
			}
			/* 16Bit CRC�̎�M */
			(void)S_SPIExchgData( DUMMY_DATA, &dToken );
			(void)S_SPIExchgData( DUMMY_DATA, &dToken );
			return SPI_INTERFACE_ERROR;
		}
	}
	if (timeOut == D_TRUE) {
		OUTLOG(OutputLog("[SDDRV]S_Receive_Data StartToken TimeOut!!\n");)
		return SPI_INTERFACE_ERROR;
	}

	/* �f�[�^����M */
	buff_ptr = pBuff;
	for ( i = 0; i < dLength ; i++ ) {
		/* S_SPIExchgData�֐����Ăяo���ė��p����Ə������x�����߁A���l�̏����𒼐ڋL�q���� */
		/* �J�[�h�L���`�F�b�N */
		if ( S_Card_Exist() == D_FALSE ) {
			OUTLOG(OutputLog("[SDDRV]S_Receive_Data SD Card Non!!\n");)
			return SPI_INTERFACE_ERROR;
		}

		/* ���M�f�[�^�����݂��Ă��邩�`�F�b�N */
		timeOut = D_TRUE;
		startTime1ms = Timer_ctrl_GetTime1ms();
		while (Timer_ctrl_TimeCompare1ms(startTime1ms, 200) == N_NG) {	/* �^�C���A�E�g200ms */
			if ( (*C_SdReg_SSR & SSR_TBI_BIT ) == SSR_TBI_BIT ) {
				timeOut = D_FALSE;
				break;
			}
		}
		if (timeOut == D_TRUE) {
			OUTLOG(OutputLog("[SDDRV]S_Receive_Data Send TimeOut!!\n");)
			return SPI_INTERFACE_ERROR;
		}

		/* ���M�f�[�^���W�X�^�ɏo�̓f�[�^���Z�b�g */
		*C_SdReg_TDR = DUMMY_DATA;

		/* ��M�������|�[�����O�Ŕ��� */
		timeOut = D_TRUE;
		startTime1ms = Timer_ctrl_GetTime1ms();
		while (Timer_ctrl_TimeCompare1ms(startTime1ms, 200) == N_NG) {	/* �^�C���A�E�g200ms */
			wk_ssr = *C_SdReg_SSR;			/* UART0�̃X�e�[�^�X�Ǐo�� */
			if (( wk_ssr & ERROR_SSR ) != 0 ) {	/* �G���[���� */
				*C_SdReg_SSR = wk_ssr | SSR_REC_BIT;	/* �G���[�t���O�N���A */
				wk_rdr = *C_SdReg_RDR;					/* �_�~�[�Ŏ擾       */
				OUTLOG(OutputLog("[SDDRV]S_Receive_Data Rcv ERR!!\n");)
				return SPI_INTERFACE_ERROR;
			}

			if ( (wk_ssr & EXIRX_SSR) != 0 ) {			/* ��M�f�[�^����H */
				*buff_ptr++ = (UI_8)(*C_SdReg_RDR & (UI_8)0xFF);	/* ��M�f�[�^�擾   */
				timeOut = D_FALSE;
				break;
			}
		}
		if (timeOut == D_TRUE) {
			OUTLOG(OutputLog("[SDDRV]S_Receive_Data Rcv TimeOut!!\n");)
			return SPI_INTERFACE_ERROR;
		}
	}

	/* �G���[�Ȃ� */
	/* 16Bit CRC�̎�M */
	(void)S_SPIExchgData( DUMMY_DATA, &dToken );
	crc = ((UI_16)dToken) << 8;
	(void)S_SPIExchgData( DUMMY_DATA, &dToken );
	crc = crc | (UI_16)dToken;
	crc_calc = S_SdDrv_CalcDataCrc16(pBuff, (UI_32)dLength);
	if (crc != crc_calc) {
		/* CRC�G���[ */
		OUTLOG(OutputLog("[SDDRV]S_Receive_Data CRC ERR!!\n");)
		return SPI_DATA_CRC_ERROR;
	}

	return SPI_NO_ERROR;

}


/********************************************************************/
/* Name          : S_Get_CSD                                        */
/* Description   : CSD���W�X�^�l�擾                                */
/* Argument      : �Ȃ�                                             */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Get_CSD( UI_8 *respCSD )
{

	E_SPIERROR resErr;		/* �߂�l�p�ϐ� */

	if ( respCSD == D_NULL ) {			/* �����`�F�b�N */
		resErr = SPI_ERROR;
	}
	else {
		S_CS_OnOff( D_ON );

		resErr = S_CommandAndResponse (	/* CMD9(SEND_CSD) */
					SEND_CSD, 
					0UL,
					R1 );
		if ( resErr == SPI_NO_ERROR ) {
			/* CSD���W�X�^�f�[�^�擾 */
			resErr = S_Receive_Data( respCSD, (UI_16)(CSD_BYTE_LENGTH - 1) );
		}

		S_CS_OnOff( D_OFF );
	}

	return resErr;

}

/********************************************************************/
/* Name          : S_Get_AddressingMode                             */
/* Description   : SD�J�[�h�̃A�h���b�V���O���[�h�擾               */
/* Argument      : [ /O]mode    �A�h���b�V���O���[�h                */
/*                              (SD_ADR_MODE_xx)                    */
/* Return value  : D_OK         ����                                */
/*                 D_NG         �ُ�                                */
/* Programmed by : (OFT)Toguchi                                      */
/* Date          : 2016/MM/DD                                       */
/* Note          :                                                  */
/********************************************************************/
static SI_8 S_Get_AddressingMode( E_SD_ADR_MODE *mode )
{

	SI_8		ret;
	E_SPIERROR	resErr;
	UI_8		bufOCR[OCR_BYTE_SIZE];

	if (mode == D_NULL) {
		ret = D_NG;
	}
	else {
		resErr = S_Get_OCR(bufOCR);
		if (resErr == SPI_NO_ERROR) {
			if ((bufOCR[0] & 0x40) == 0) {
				/* �o�C�g�A�h���b�V���O */
				*mode = SD_ADR_MODE_BYTE;
			}
			else {
				/* �u���b�N�A�h���b�V���O */
				*mode = SD_ADR_MODE_BLOCK;
			}
			ret = D_OK;
		}
		else {
			ret = D_NG;
		}
	}

	return ret;

}

/********************************************************************/
/* Name          : S_Get_OCR                                        */
/* Description   : OCR���W�X�^�l�擾                                */
/* Argument      : �Ȃ�                                             */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/MM/DD                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Get_OCR( UI_8 *bufOCR )
{

	E_SPIERROR resErr;		/* �߂�l�p�ϐ� */
	UI_8 i;
	UI_8 j;

	if ( bufOCR == D_NULL ) {			/* �����`�F�b�N */
		resErr = SPI_ERROR;
	}
	else {
		S_CS_OnOff( D_ON );

		resErr = S_CommandAndResponse (	/* CMD58(READ_OCR) */
					READ_OCR, 
					0UL,
					R1 );
		if ( resErr == SPI_NO_ERROR ) {
			/* OCR���W�X�^�f�[�^�擾 */
			for (i = 0; i < OCR_BYTE_SIZE; i++) {
				if (S_SPIExchgData( DUMMY_DATA, &bufOCR[i] ) == D_NG) {
					resErr = SPI_ERROR;
					break;
				}
			}
		}
		(void)S_SPIExchgData( DUMMY_DATA, &j );

		S_CS_OnOff( D_OFF );
	}

	return resErr;

}


/********************************************************************/
/* Name          : S_Set_CardInfo                                   */
/* Description   : �J�[�h���̃Z�b�g                               */
/* Argument      : *respCSD     CSD�f�[�^���i�[����Ă���̈�       */
/*                 adrMode      �A�h���b�V���O���[�h                */
/*                 *wkcardinfo  �J�[�h�����i�[���邽�߂̗̈�      */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Set_CardInfo( UI_8 *respCSD, E_SD_ADR_MODE adrMode, T_CARD_INFO *wkcardinfo )
{

	E_SPIERROR resErr;		/* �߂�l�p�ϐ� */
	UI_32 c_size;			/* C_SIZE       */
	UI_16 c_size_mult_1;	/* C_SIZE_MULT_1 */
	UI_16 c_size_mult_2;	/* C_SIZE_MULT_2 */
	UI_16 blklen;			/* �u���b�N��   */
	UI_32 total_Secotors;	/* �g�[�^���Z�N�^�[�� */
	UI_16 ui16temp;
	UI_32 ui32temp;

	/* �����`�F�b�N */
	if (( respCSD == D_NULL ) || ( wkcardinfo == D_NULL )) {
		return SPI_ERROR;
	}

	/* �J�[�h���\���̏����� */
	S_CardInfo_Init();

	/* wkcardinfo�\���̂Ƀf�[�^���Z�b�g */
	wkcardinfo -> bCardExist = S_Card_Exist();
	wkcardinfo -> uProtectState = S_Card_Protect();
	wkcardinfo -> AdrMode = adrMode;

	/* �g�[�^���u���b�N�����Z�o */
	if ((respCSD[0] & 0xC0U) == 0x00U) {		/* CSD Ver1.0 */
		/* CSD���W�X�^�l����f�[�^�u���b�N�����l�� */
		ui16temp = (UI_16)( (UI_16)respCSD[5] & 0x0FU );
		blklen = (UI_16)( 1U << ui16temp );					/* �f�[�^�u���b�N��(Byte) */

		/* �u���b�N���`�F�b�N */
		if ( blklen != DEFAULT_BLK_LEN ) {
			/* �u���b�N����512byte�ȊO�̏ꍇ�ASD�J�[�h�Ƀu���b�N��512byte��ݒ肷�� */
			resErr = S_Set_BlockLen( DEFAULT_BLK_LEN );	/* �u���b�N����ݒ� */
			if ( resErr != SPI_NO_ERROR ) {
				return resErr;
			}
			resErr = S_Get_CSD ( respCSD );			/* CSD�f�[�^�擾 */
			if ( resErr != SPI_NO_ERROR ) {
				return resErr;
			}

			/* CSD���W�X�^�l����f�[�^�u���b�N�����l�� */
			ui16temp = (UI_16)( (UI_16)respCSD[5] & 0x0FU );
			blklen = (UI_16)( 1U << ui16temp );					/* �f�[�^�u���b�N��(Byte) */
		}

		/* CSD���W�X�^�l����A�J�[�h�e�ʂ��v�Z */
		ui16temp = (UI_16)( (UI_16)respCSD[6] & 3U );		/* C_SIZE�t�B�[���h�̊l�� */
		ui16temp <<= 8;
		c_size = (UI_32)respCSD[7] + (UI_32)ui16temp;
		c_size <<= 2;
		ui16temp = (UI_16)( (UI_16)((UI_16)respCSD[8] >> 6U) & 3U );
		c_size += ( (UI_32)ui16temp + 1UL );

		c_size_mult_1 = (UI_16)( (UI_16)respCSD[9] & 3U );	/* C_SIZE_MULT �t�B�[���h�̊l�� */
		c_size_mult_1 <<= 1;
		c_size_mult_2 = (UI_16)( (UI_16)((UI_16)respCSD[10] >> 7U) & 1U );

		/* �g�[�^���u���b�N�� BLOCKNR */
		c_size <<= ((c_size_mult_1 + c_size_mult_2 + 2));	/* (C_SIZE+1) <<= (C_SIZE_MULT+2) */
		/* 2GB��SD�̓u���b�N����DEFAULT_BLK_LEN(512byte)�ȊO�̃P�[�X�����邽�� */
		/* CSD���W�X�^����擾�����u���b�N���ŃT�C�Y���Z�o����DEFAULT_BLK_LEN�ɑΉ������u���b�N�T�C�Y�ɂ��� */
		total_Secotors = (c_size * blklen) / DEFAULT_BLK_LEN;
	}
	else if ((respCSD[0] & 0xC0U) == 0x40U) {	/* CSD Ver2.0 */
		/* CSD���W�X�^�l����A�J�[�h�e�ʂ��v�Z */
		ui32temp = (UI_32)respCSD[7] & 0x3FU;	/* C_SIZE�t�B�[���h�̊l�� */
		ui32temp <<= 8;
		ui32temp = (UI_32)respCSD[8] + ui32temp;
		ui32temp <<= 8;
		c_size = (UI_32)respCSD[9] + ui32temp;
		c_size += 1UL;

		/* �g�[�^���u���b�N�� BLOCKNR */
		c_size <<= 10;										/* (C_SIZE+1) <<= 10 */
		total_Secotors = c_size;
	}
	else {			/* MMC */
		/* ���Ή� */
		return SPI_ERROR;
	}


	/* wkcardinfo�\���̂Ƀf�[�^���Z�b�g */
	wkcardinfo -> ulTotal_Secotors = total_Secotors;

	return SPI_NO_ERROR;

}

/********************************************************************/
/* Name          : S_Set_BlockLen                                   */
/* Description   : �u���b�N���ݒ�                                   */
/* Argument      : blocklen     �ݒ�u���b�N��                      */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Set_BlockLen( UI_16 blocklen )
{

	E_SPIERROR resErr;		/* �߂�l�p�ϐ� */
	UI_8 j;

	S_CS_OnOff( D_ON );

	resErr = S_CommandAndResponse (	/* CMD16(SET_BLOCKLEN) */
				SET_BLOCKLEN, 
				(UI_32)blocklen,
				R1 );
	(void)S_SPIExchgData( DUMMY_DATA, &j );

	S_CS_OnOff( D_OFF );

	return resErr;

}

/********************************************************************/
/* Name          : S_CommandAndResponse                             */
/* Description   : �R�}���h�g�[�N���M�ƃ��X�|���X��M               */
/* Argument      : cmd          �R�}���h�C���f�b�N�X                */
/*                 arg          �R�}���h����                        */
/*                 resp         ���X�|���X�^�C�v                    */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_CommandAndResponse ( UI_16 cmd, UI_32 arg, E_RESP_TYPE resp )
{

	E_SPIERROR	resErr;		/* �߂�l�p�ϐ� */
	UI_8		rcvData;

	if (cmd != STOP_TRANSMISSION) {
		S_CS_OnOff( D_OFF );
		(void)S_SPIExchgData( DUMMY_DATA, &rcvData );
		S_CS_OnOff( D_ON );
		(void)S_SPIExchgData( DUMMY_DATA, &rcvData );
		if ( S_CheckCardBusy( D_ON ) == D_NG ) {
			OUTLOG(OutputLog("[SDDRV]S_CommandAndResponse SD BUSY!!! cmd:0x%04X arg:%d resp:%d\n", cmd, arg, resp);)
			return SPI_CARD_IS_BUSY;
		}
	}

	/* �R�}���h�g�[�N���𑗐M */
	if ( S_PrepareAndSetup( cmd, arg ) == SPI_NO_ERROR ) {
		if (cmd == STOP_TRANSMISSION) {					/* CMD12(STOP_TRANSMISSION)�𑗐M����	*/
			/* �_�~�[�f�[�^1byte���M	*/
			(void)S_SPIExchgData( DUMMY_DATA, &rcvData );
		}

		/* ���X�|���X����M */
		resErr = S_WaitForCmdResponse( resp );
	}
	else {
		resErr = SPI_INTERFACE_ERROR;
		OUTLOG(OutputLog("[SDDRV]S_CommandAndResponse S_PrepareAndSetup ERR!!!! cmd:0x%04X arg:%d resp:%d\n", cmd, arg, resp);)
	}

	return resErr;

}


/********************************************************************/
/* Name          : S_PrepareAndSetup                                */
/* Description   : �R�}���h�g�[�N���쐬�E���M                       */
/* Argument      : cmd          �R�}���h�C���f�b�N�X                */
/*                 arg          �R�}���h����                        */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_PrepareAndSetup( UI_16 cmd, UI_32 args )
{

	E_SPIERROR resErr;		/* �߂�l�p�ϐ� */

	if ( S_SendCommand( cmd, args ) == D_OK ) {
		/* S_SendCommand ����I��*/
		resErr = SPI_NO_ERROR;
	}
	else {
		/* S_SendCommand �ُ�I��*/
		resErr = SPI_ERROR;
		OUTLOG(OutputLog("[SDDRV]S_PrepareAndSetup S_SendCommand ERR!!!! cmd:0x%04X arg:%d\n", cmd, args);)
	}

	return resErr;

}

/********************************************************************/
/* Name          : S_SendCommand                                    */
/* Description   : �R�}���h�g�[�N�����M                             */
/* Argument      : cmd_index    �R�}���h�C���f�b�N�X                */
/*                 arg          �R�}���h����                        */
/* Return value  : D_OK         ����I��                            */
/*                 D_NG         �ُ�I��                            */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static SI_8 S_SendCommand( UI_16 command, UI_32 args )
{

	SI_8 ret;				/* �߂�l�p�ϐ� */
	UI_8 *cmd_ptr;			/* ���M�p�R�}���h�f�[�^ */
	UI_8 cmd_bytes[CMD_BYTE_LENGTH];		/* �R�}���h�f�[�^�p�̈� */
	UI_8 *cmd_bytes_ptr = &cmd_bytes[0];	/* �R�}���h�f�[�^�p�̈� */
	UI_8 loop_end_flg;		/* ���[�v�I���t���O */
	UI_16 i;
	UI_8  j;

	/* �R�}���h�g�[�N���̐��� */
	cmd_bytes[0] = (UI_8)command;		/* �R�}���h�ԍ� */
	for ( i = 4; i > 0; i-- ) {			/* �R�}���h���� */
		cmd_bytes[i] = (UI_8)(args & 0xFFUL);
		args >>= 8;
	}
	cmd_bytes[5] = S_SdDrv_CalcCmdCrc7( cmd_bytes );

	/* �R�}���h�g�[�N���̑��M */
	cmd_ptr = cmd_bytes_ptr;
	loop_end_flg = D_OFF;
	for ( i = 0; (( i < 6 ) && ( loop_end_flg == D_OFF )); i++ ) {
		if ( S_SPIExchgData( cmd_ptr[i], &j ) == D_NG ) {
			loop_end_flg = D_ON;
		}
	}
	if ( loop_end_flg == D_OFF ) {
		/* ����I�� */
		ret = D_OK;
	}
	else {
		/* �ُ�I�� */
		ret = D_NG;
		OUTLOG(OutputLog("[SDDRV]S_SendCommand S_SPIExchgData ERR!!!! sendbyte:%d\n", i);)
	}

	return ret;

}


/********************************************************************/
/* Name          : S_SPIExchgData                                   */
/* Description   : SPI�o�X�łP�o�C�g�f�[�^�ʐM���s��                */
/* Argument      : odata        ���M�f�[�^                          */
/*                 *idata	    ��M�f�[�^�i�[�̈�                  */
/* Return value  : D_OK         ��M�f�[�^����                      */
/*                 D_NG         ��M�f�[�^�Ȃ�                      */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          : UART�̑��M���͏����ݒ�Ŏ��O�ɍs������         */
/********************************************************************/
static SI_8 S_SPIExchgData( UI_8 odata, UI_8 *idata )
{

	UI_8 wk_ssr;
	volatile UI_8 wk_rdr;
	UI_32 startTime1ms;
	B_8 timeOut;

	if ( idata == D_NULL ) {	/* �����`�F�b�N */
		return D_NG;
	}

	/* �]���������|�[�����O�Ŕ��� */
	timeOut = D_TRUE;
	startTime1ms = Timer_ctrl_GetTime1ms();
	while (Timer_ctrl_TimeCompare1ms(startTime1ms, 200) == N_NG) {	/* �^�C���A�E�g200ms */
		if ( (*C_SdReg_SSR & SSR_TBI_BIT ) == SSR_TBI_BIT ) {	/* ���M�o�X�A�C�h��	*/
			timeOut = D_FALSE;
			break;
		}
	}
	if (timeOut == D_TRUE) {
		OUTLOG(OutputLog("S_SPIExchgData TimeOut!! SndData:0x%02X\n", odata);)
		return D_NG;
	}

	/* ���M�f�[�^���W�X�^�ɏo�̓f�[�^���Z�b�g */
	*C_SdReg_TDR = odata;

	/* ��M�������|�[�����O�Ŕ��� */
	timeOut = D_TRUE;
	startTime1ms = Timer_ctrl_GetTime1ms();
	while (Timer_ctrl_TimeCompare1ms(startTime1ms, 200) == N_NG) {	/* �^�C���A�E�g200ms */
		wk_ssr = *C_SdReg_SSR;			/* UART0�̃X�e�[�^�X�Ǐo�� */
		if (( wk_ssr & ERROR_SSR ) != 0 ) {	/* �G���[ */
			*C_SdReg_SSR = wk_ssr | SSR_REC_BIT;	/* �G���[�t���O�N���A */
			wk_rdr = *C_SdReg_RDR;					/* �_�~�[�Ŏ擾       */
			OUTLOG(OutputLog("S_SPIExchgData OverRunErr!! SndData:0x%02X RcvData:0x%02X\n", odata, *idata);)
			return D_NG;
		}

		if ( (wk_ssr & EXIRX_SSR) != 0 ) {			/* ��M�f�[�^����H */
			*idata = (UI_8)(*C_SdReg_RDR & (UI_8)0xFF);	/* ��M�f�[�^�擾   */
			timeOut = D_FALSE;
			break;
		}
	}
	if (timeOut == D_TRUE) {
		OUTLOG(OutputLog("S_SPIExchgData Rcv TimeOut!!\n");)
		return D_NG;
	}

	return D_OK;

}


/********************************************************************/
/* Name          : S_WaitForCmdResponse                             */
/* Description   : �R�}���h���X�|���X�g�[�N����M                   */
/* Argument      : resptype     ���X�|���X�^�C�v                    */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_WaitForCmdResponse( E_RESP_TYPE respType )
{

	E_SPIERROR resErr;		/* �߂�l�p�ϐ� */
	UI_16 respStatus;		/* ���X�|���X�X�e�[�^�X */

	/* ���X�|���X�g�[�N���̎�M */
	resErr = S_Get_ResponseInfo( &respStatus, respType );
	if (( resErr != SPI_CARD_IS_NOT_RESPONDING ) && ( resErr != SPI_ERROR )) {
		/* ���X�|���X����͂��A�����G���[�l�ɕϊ� */
		if ( respStatus == 0 ) {
			resErr = SPI_NO_ERROR;
		}
		else if ( (respStatus & CMD_PARAM_ERROR) != 0U ) {
			resErr = SPI_COMMAND_PARAMETER_ERROR;
			OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! ParamErr!! Res:0x%04X\n", respStatus);)
		}
		else if ( (respStatus & ADDRESS_ERROR) != 0U ) {
			resErr = SPI_ADDRESS_ERROR;
			OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! AdrErr Res:0x%04X\n", respStatus);)
		}
		else if ( (respStatus & ERASE_SEQ_ERROR) != 0U ) {
			resErr = SPI_ERASE_SEQ_ERROR;
			OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! EraseSeqErr Res:0x%04X\n", respStatus);)
		}
		else if ( (respStatus & CMD_CRC_ERROR) != 0U ) {
			resErr = SPI_CMD_CRC_ERROR;
			OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! CmdCrcErr Res:0x%04X\n", respStatus);)
		}
		else if ( (respStatus & COMUNC_ILLEG_COM) != 0U ) {
			resErr = SPI_COMUNC_ILLEG_COM;
			OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! ComIlleg Res:0x%04X\n", respStatus);)
		}
		else if ( (respStatus & ERASE_RESET) != 0U ) {
			resErr = SPI_ERASE_RESET;
			OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! EraseRst Res:0x%04X\n", respStatus);)
		}
		else if ( (respStatus & CARD_IS_NOT_READY) != 0U ) {
			resErr = SPI_CARD_IS_NOT_READY;
			/* OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! NotReady Res:0x%04X\n", respStatus);) */
		}
		else if ( (respStatus & CMD_PARAM_ERROR2) != 0U ) {
			resErr = SPI_COMMAND_PARAMETER_ERROR;
			OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! ParamErr2 Res:0x%04X\n", respStatus);)
		}
		else if ( (respStatus & ERASE_PARAM) != 0U ) {
			resErr = SPI_ERASE_PARAM;
			OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! EraseParam Res:0x%04X\n", respStatus);)
		}
		else if ( (respStatus & WP_VIOLATION) != 0U ) {
			resErr = SPI_WP_VIOLATION;
			OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! WPViolation Res:0x%04X\n", respStatus);)
		}
		else if ( (respStatus & CARD_READ_FAILURE) != 0U ) {
			resErr = SPI_CARD_READ_FAILURE;
			OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! ReadFilure Res:0x%04X\n", respStatus);)
		}
		else if ((respStatus & EERROR) != 0U ) {
			resErr = SPI_ERROR;
			OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! ERR Res:0x%04X\n", respStatus);)
		}
		else if ( (respStatus & ERROR2) != 0U ) {
			resErr = SPI_ERROR;
			OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! ERR2 Res:0x%04X\n", respStatus);)
		}
		else if ( (respStatus & WP_ERASE_SKIP) != 0U ) {
			resErr = SPI_WP_ERASE_SKIP;
			OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! EraseSkip Res:0x%04X\n", respStatus);)
		}
		else {
			resErr = SPI_ERROR;
			OUTLOG(OutputLog("S_WaitForCmdResponse ERR!!! OtherErr Res:0x%04X\n", respStatus);)
		}
	}

	return resErr;

}


/********************************************************************/
/* Name          : S_Get_ResponseInfo                               */
/* Description   : �R�}���h���X�|���X�g�[�N����M                   */
/* Argument      : resptype     ���X�|���X�^�C�v                    */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Get_ResponseInfo( UI_16 *respStatus, E_RESP_TYPE respType )
{

	E_SPIERROR resErr;		/* �߂�l�p�ϐ� */
	UI_8 loop_end_flg;		/* ���[�v�I���t���O */
	UI_8 iData;				/* ��M�f�[�^�p�̈� */
	UI_16 i;

	if ( respStatus == D_NULL ) {		/* ��1�����`�F�b�N */
		resErr = SPI_ERROR;
	}
	else {
		resErr = SPI_CARD_IS_NOT_RESPONDING;
		i = 10;							/* SD�J�[�h����̉����̓R�}���h���M��0�`8byte�̊Ԃɂ���	*/
		loop_end_flg = D_OFF;
		while (( i > 0 ) && ( loop_end_flg == D_OFF )) {
			iData = 0xFF;
			if ( S_Card_Exist() == D_FALSE ) {			/* �J�[�h�L���`�F�b�N[2005.03.18 �ύXL] */
				resErr = SPI_ERROR;
				loop_end_flg = D_ON;
			}
			else if ( S_SPIExchgData( DUMMY_DATA, &iData ) == D_NG ) {
				resErr = SPI_ERROR;
				loop_end_flg = D_ON;
				OUTLOG(OutputLog("S_Get_ResponseInfo S_SPIExchgData ERR!!\n");)
			}
			else {
				if ( ( iData & (UI_8)0x80 ) == 0 ) {		/* ���X�|���X�g�[�N����M */
					resErr = SPI_NO_ERROR;
					*respStatus = (UI_16)( (UI_16)iData << 8 );
					if ( respType == R2 ) {		/* 2Byte�^�C�v�̃��X�|���X�g�[�N�� */
						if ( S_SPIExchgData( DUMMY_DATA, &iData ) == D_NG ) {
							resErr = SPI_ERROR;
						}
						else {
							*respStatus |= ( (UI_16)iData );
						}
					}
					loop_end_flg = D_ON;
				}
				else {
				}
			}
			i--;
		}
		if (i == 0) {
			OUTLOG(OutputLog("S_Get_ResponseInfo TimeOut!!\n");)
		}
	}

	return resErr;

}


/********************************************************************/
/* Name          : S_Send_Data                                      */
/* Description   : �f�[�^���M                                       */
/* Argument      : pBuff        ���M�f�[�^�i�[�̈�                  */
/*                 dLength      ���M�f�[�^��                        */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Send_Data( const UI_8 *pBuff, UI_16 dLength, UI_8 sendToken )
{

	E_SPIERROR resErr;		/* �߂�l�p�ϐ� */
	UI_8 dToken;			/* �f�[�^�g�[�N���p�̈� */
	UI_8 wk_ssr;			/* ��Ɨp�ϐ�           */
	volatile UI_8 wk_rdr;	/* ��Ɨp�ϐ�           */
	UI_16 i;
	UI_16 j;
	UI_32 startTime1ms;
	B_8 timeOut;
	UI_16 crc;

	/* �����`�F�b�N */
	if ( pBuff == D_NULL ) {
		return SPI_ERROR;
	}

	if (S_CheckCardBusy( D_ON ) == D_NG) {
		OUTLOG(OutputLog("[SDDRV]S_Send_Data Befor SD BUSY!!\n");)
		return SPI_CARD_IS_BUSY;
	}

	/* �f�[�^�g�[�N���E�X�^�[�g�o�C�g�̑��M */
	(void)S_SPIExchgData( DUMMY_DATA, &dToken );
	(void)S_SPIExchgData( sendToken, &dToken );

	/* �f�[�^���M */
	for ( i = 0; i < dLength; i++ ) {
		/* S_SPIExchgData�֐����Ăяo���ė��p����Ə������x�����߁A���l�̏����𒼐ڋL�q���� */
		/* �J�[�h�L���`�F�b�N */
		if ( S_Card_Exist() == D_FALSE ) {
			return SPI_INTERFACE_ERROR;
		}

		/* ���M�f�[�^�����݂��Ă��邩�`�F�b�N */
		j = 0x0008;	/* �ʏ킱���Ń��[�v���邱�Ƃ͂Ȃ� */
		while ( j > 0 ) {
			if ( (*C_SdReg_SSR & SSR_TBI_BIT ) == SSR_TBI_BIT ) {
				break;
			}
			j--;
		}
		if (j == 0) {
			return SPI_INTERFACE_ERROR;
		}

		/* ���M�f�[�^���W�X�^�ɏo�̓f�[�^���Z�b�g */
		*C_SdReg_TDR = pBuff[i];

		/* ��M�������|�[�����O�Ŕ��� */
		timeOut = D_TRUE;
		startTime1ms = Timer_ctrl_GetTime1ms();
		while (Timer_ctrl_TimeCompare1ms(startTime1ms, 200) == N_NG) {	/* �^�C���A�E�g200ms */
			wk_ssr = *C_SdReg_SSR;			/* UART0�̃X�e�[�^�X�Ǐo�� */
			if (( wk_ssr & ERROR_SSR ) != 0 ) {	/* �G���[ */
				/* �G���[���� */
				*C_SdReg_SSR = wk_ssr | SSR_REC_BIT;	/* �G���[�t���O�N���A */
				wk_rdr = *C_SdReg_RDR;					/* �_�~�[�Ŏ擾       */
				OUTLOG(OutputLog("S_Send_Data Rcv ERR!! SndCnt:%d\n", i);)
				return SPI_INTERFACE_ERROR;
			}

			if ( (wk_ssr & EXIRX_SSR) != 0 ) {			/* ��M�f�[�^����H */
				dToken = (UI_8)(*C_SdReg_RDR & (UI_8)0xFF);	/* ��M�f�[�^�擾   */
				timeOut = D_FALSE;
				break;
			}
		}
		if (timeOut == D_TRUE) {
			OUTLOG(OutputLog("S_Send_Data Rcv TimeOut!! SndCnt:%d\n", i);)
			return SPI_INTERFACE_ERROR;
		}
	}

	/* �f�[�^�g�[�N���E16bit CRC�𑗐M */
	crc = S_SdDrv_CalcDataCrc16(pBuff, (UI_32)dLength);
	(void)S_SPIExchgData( (UI_8)(crc >> 8), &dToken );
	(void)S_SPIExchgData( (UI_8)(crc & (UI_8)0xFF), &dToken );

	/* �J�[�h����f�[�^���X�|���X�g�[�N������M���� */
	resErr = S_CRCErrOnWrite();
	if ( resErr != SPI_NO_ERROR ) {
		OUTLOG(OutputLog("S_Send_Data S_CRCErrOnWrite ERR!! ret:%d\n", resErr);)
		(void)S_CheckCardBusy(D_ON);
		return resErr;
	}

	if (S_CheckCardBusy( D_ON ) == D_NG) {
		OUTLOG(OutputLog("[SDDRV]S_Send_Data After SD BUSY!!\n");)
		return SPI_CARD_IS_BUSY;
	}

	return SPI_NO_ERROR;

}


/********************************************************************/
/* Name          : S_CRCErrOnWrite                                  */
/* Description   : �f�[�^���X�|���X�g�[�N����M                     */
/* Argument      : �Ȃ�                                             */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_CRCErrOnWrite( void )
{

	E_SPIERROR resErr = SPI_CARD_IS_NOT_RESPONDING;		/* �߂�l�p�ϐ� */
	UI_8 dDat;											/* ��M�f�[�^�p�̈� */

	if ( S_Card_Exist() == D_TRUE ) {
		if ( S_SPIExchgData( DUMMY_DATA, &dDat ) == D_OK ) {	/* �J�[�h�L���`�F�b�N�ǉ�[2005.03.18 �ύXL] */
			if ( (dDat & 0x1F) == 0x05 ) {
				resErr = SPI_NO_ERROR;	/* �f�[�^��M������Ɋ��� */
			}
			else if ( (dDat & 0x1F) == 0x0B ) {
				resErr = SPI_DATA_STATUS_CRC_ERROR; /* CRCerror */
			}
			else if ( (dDat & 0x1F) == 0x0D ) {
				resErr = SPI_CARD_WRITE_ERROR; /* Write error */
			}
			else {
				/* nop */
			}
		}
	}

	return resErr;

}


/********************************************************************/
/* Name          : S_CheckCardBusy                                  */
/* Description   : Busy�`�F�b�N                                     */
/* Argument      : �Ȃ�                                             */
/* Return value  : D_OK         Ready���                           */
/*                 D_NG         Busy���                            */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static SI_8 S_CheckCardBusy( UI_8 curCsStat )
{

	SI_8 ret;				/* �߂�l�p�ϐ� */
	UI_8 datState;			/* �o�͐M����� */
	UI_32 startTime1ms;

	if (curCsStat == D_OFF) {
		S_CS_OnOff( D_ON );
	}

	ret = D_NG;

	startTime1ms = Timer_ctrl_GetTime1ms();
	while (Timer_ctrl_TimeCompare1ms(startTime1ms, 500) == N_NG) {	/* �^�C���A�E�g500ms */

		/* Busy��Ԃł́ADataOut��Low�ɂȂ��Ă���̂ŁA0x00����M���Ă��� */
		datState = 0;
		if ( S_SPIExchgData( DUMMY_DATA, &datState)  == D_NG ) {
			break;
		}
		else {
			if ( datState == (UI_8)0xFF ) {
				/* DataOut��Hi�ɂȂ����̂ŁAReady��Ԃֈڍs */
				ret = D_OK;
				break;
			}
		}
	}

	if (curCsStat == D_OFF) {
		S_CS_OnOff( D_OFF );
	}


	return ret;

}

/********************************************************************/
/* Name          : S_Read_SingleBlock                               */
/* Description   : �J�[�h�f�[�^���[�h(1�Z�N�^)                      */
/* Argument      : *pBuff       �ǂݏo�����P�Z�N�^���̃f�[�^���i�[  */
/*                 ulSectorNo   �Z�N�^�ԍ�                          */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/10/01                                       */
/* Note          : �w�肳�ꂽ�Z�N�^����f�[�^(512byte)�ǂݏo��      */
/********************************************************************/
static E_SPIERROR S_Read_SingleBlock( UI_8 *pBuff, UI_32 ulSectorNo )
{

	E_SPIERROR resErr;		/* SPI�G���[�߂�l�p�ϐ� */
	UI_32 block;
	UI_8	rcvData;

	if (S_card_info.AdrMode == SD_ADR_MODE_BYTE) {
		block = ulSectorNo * (UI_32)DEFAULT_BLK_LEN;
	}
	else if (S_card_info.AdrMode == SD_ADR_MODE_BLOCK) {
		block = ulSectorNo;
	}
	else {
		return SPI_ERROR;
	}

	S_CS_OnOff( D_ON );

	resErr = S_CommandAndResponse(READ_BLOCK, block, R1);	/* CMD17(READ_BLOCK) */
	if ( resErr == SPI_NO_ERROR ) {
		resErr = S_Receive_Data( pBuff, DEFAULT_BLK_LEN ); /* �f�[�^�u���b�N�̎�M */
	}

	S_CS_OnOff( D_OFF );
	(void)S_SPIExchgData( DUMMY_DATA, &rcvData );

	return resErr;

}

/********************************************************************/
/* Name          : S_Read_MultiBlock                                */
/* Description   : �J�[�h�f�[�^���[�h(�����Z�N�^)                   */
/* Argument      : *pBuff       �ǂݏo�����f�[�^���i�[              */
/*                 ulSectorNo   �Z�N�^�ԍ�                          */
/*                 ulSectorCnt  �Z�N�^��                            */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/10/01                                       */
/* Note          : �w�肳�ꂽ�Z�N�^����f�[�^�ǂݏo��               */
/********************************************************************/
static E_SPIERROR S_Read_MultiBlock( UI_8 *pBuff, UI_32 ulSectorNo, UI_32 uSectorCnt )
{

	E_SPIERROR resErr;		/* SPI�G���[�߂�l�p�ϐ� */
	UI_32	block;
	UI_32	i;
	UI_8	rcvData;

	if (S_card_info.AdrMode == SD_ADR_MODE_BYTE) {
		block = ulSectorNo * (UI_32)DEFAULT_BLK_LEN;
	}
	else if (S_card_info.AdrMode == SD_ADR_MODE_BLOCK) {
		block = ulSectorNo;
	}
	else {
		return SPI_ERROR;
	}

	S_CS_OnOff( D_ON );

	/* CMD18 (READ_MULTIPLE_BLOCK)���M�i�����u���b�N�̎�M�j */
	resErr = S_CommandAndResponse(READ_MULTIPLE_BLOCK, block, R1);
	if (resErr == SPI_NO_ERROR) {
		/* �f�[�^��M	*/
		for (i = 0; i < uSectorCnt; i++) {
			resErr = S_Receive_Data(pBuff + (i * DEFAULT_BLK_LEN), DEFAULT_BLK_LEN);
			if (resErr != SPI_NO_ERROR) {
				OUTLOG(OutputLog("[SDDRV]S_Read_MultiBlock RcvData ERR!!\n");)
				break;
			}
		}

		/* CMD12 (STOP_TRANSMISSION)��M��~ */
		if (S_CommandAndResponse(STOP_TRANSMISSION, block, R1) == SPI_NO_ERROR) {
			(void)S_CheckCardBusy(D_ON);
		}
	}

	S_CS_OnOff( D_OFF );
	(void)S_SPIExchgData( DUMMY_DATA, &rcvData );

	return resErr;

}

/********************************************************************/
/* Name          : S_Write_SingleBlock                              */
/* Description   : �J�[�h�f�[�^���C�g(1�Z�N�^)                      */
/* Argument      : *pBuff       �������ݗp�f�[�^���i�[����Ă���    */
/*                 ulSectorNo   �Z�N�^�ԍ�                          */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/10/01                                       */
/* Note          : �w�肳�ꂽ�Z�N�^�Ƀf�[�^(512byte)��������        */
/********************************************************************/
static E_SPIERROR S_Write_SingleBlock( const UI_8 *pBuff, UI_32 ulSectorNo )
{

	E_SPIERROR resErr;		/* SPI�G���[�߂�l�p�ϐ� */
	UI_32 block;
	UI_8	rcvData;

	if (S_card_info.AdrMode == SD_ADR_MODE_BYTE) {
		block = ulSectorNo * (UI_32)DEFAULT_BLK_LEN;
	}
	else if (S_card_info.AdrMode == SD_ADR_MODE_BLOCK) {
		block = ulSectorNo;
	}
	else {
		return SPI_ERROR;
	}

	S_CS_OnOff( D_ON );

	resErr = S_CommandAndResponse(WRITE_BLOCK, block, R1);			/* CMD24 (WRITE_BLOCK) */
	if (resErr == SPI_NO_ERROR) {
		resErr = S_Send_Data(pBuff, DEFAULT_BLK_LEN, DATA_TOKEN);	/* WRITE�f�[�^�u���b�N���J�[�h�֑��M */
	}

	S_CS_OnOff( D_OFF );
	(void)S_SPIExchgData( DUMMY_DATA, &rcvData );

	return resErr;

}

/********************************************************************/
/* Name          : S_Write_MultiBlock                               */
/* Description   : �J�[�h�f�[�^���C�g(�����Z�N�^)                   */
/* Argument      : *pBuff       �������ݗp�f�[�^���i�[����Ă���    */
/*                 ulSectorNo   �Z�N�^�ԍ�                          */
/*                 ulSectorCnt  �Z�N�^��                            */
/* Return value  : E_SPIERROR   �G���[���b�Z�[�W                    */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/10/01                                       */
/* Note          : �w�肳�ꂽ�Z�N�^�Ƀf�[�^��������                 */
/********************************************************************/
static E_SPIERROR S_Write_MultiBlock( const UI_8 *pBuff, UI_32 ulSectorNo, UI_32 uSectorCnt )
{

	E_SPIERROR resErr;		/* SPI�G���[�߂�l�p�ϐ� */
	UI_32	block;
	UI_32	i;
	UI_8	rcvData;

	if (S_card_info.AdrMode == SD_ADR_MODE_BYTE) {
		block = ulSectorNo * (UI_32)DEFAULT_BLK_LEN;
	}
	else if (S_card_info.AdrMode == SD_ADR_MODE_BLOCK) {
		block = ulSectorNo;
	}
	else {
		return SPI_ERROR;
	}

	S_CS_OnOff( D_ON );

	/* CMD55(APP_CMD)���M	*/
	resErr = S_CommandAndResponse(APP_CMD, 0UL, R1);
	if (resErr == SPI_NO_ERROR) {

		/* ACMD23(SET_WR_BLK_ERASE_COUNT_ACMD23)���M�i���M�f�[�^�u���b�N����ʒm�j	*/
		resErr = S_CommandAndResponse(SET_WR_BLK_ERASE_COUNT_ACMD23, uSectorCnt, R1);
		if (resErr == SPI_NO_ERROR) {

			/* CMD25 (WRITE_MULTIPLE_BLOCK)���M�i�����u���b�N�̑��M�j */
			resErr = S_CommandAndResponse(WRITE_MULTIPLE_BLOCK, block, R1);
			if (resErr == SPI_NO_ERROR) {

				/* �f�[�^���M	*/
				for (i = 0; i < uSectorCnt; i++) {
					resErr = S_Send_Data(pBuff + (i * DEFAULT_BLK_LEN), DEFAULT_BLK_LEN, DATA_TOKEN_MULTI_SEND);
					if (resErr != SPI_NO_ERROR) {
						OUTLOG(OutputLog("[SDDRV]S_Write_MultiBlock SendData ERR!!\n");)
						break;
					}
				}

				if (resErr == SPI_NO_ERROR) {
					/* ���M��~	*/
					(void)S_SPIExchgData(DATA_TOKEN_STOP_TRAN, &rcvData);
				}
				else {
					/* ���M�G���[����CMD12(STOP_TRANSMISSION)�𑗐M���Ē�~����	*/
					(void)S_CommandAndResponse(STOP_TRANSMISSION, 0UL, R1);
				}

				(void)S_CheckCardBusy(D_ON);
			}
		}
	}

	S_CS_OnOff( D_OFF );
	(void)S_SPIExchgData( DUMMY_DATA, &rcvData );

	return resErr;

}

/********************************************************************/
/* Name          : S_SdDrv_CalcCmdCrc7								*/
/* Description   : �R�}���h���M�pCRC7�Z�o							*/
/* Argument      : *pBuff       ���M�R�}���h�o�b�t�@				*/
/* Return value  : CRC7�Z�o�l										*/
/* Programmed by : (OFT)Toguchi										*/
/* Date          : 2016/12/23										*/
/* Note          : pBuff��NULL�łȂ����Ƃ��g�p���ŕۏ؂��邱��		*/
/********************************************************************/
static UI_8 S_SdDrv_CalcCmdCrc7( const UI_8 *pBuff )
{

	/* CRC�`�F�b�N�L�����̏��� */
	UI_8	crc;
	UI_8	crc_prev;
	UI_8	i;
	SI_8	j;

	crc = pBuff[0];
	for (i = 1; i < CMD_BYTE_LENGTH; i++) {
		for(j = 7; j >= 0; j--) {
			crc <<= 1;
			crc_prev = crc;
			if (i < 5) {
				crc |= (pBuff[i] >> j) & 1;
			}
			if ((crc & 0x80) != 0) {
				crc ^= (UI_8)0x89;
			}
		}
	}

	return (crc_prev | 1);


}

/********************************************************************/
/* Name          : S_SdDrv_CalcDataCrc16							*/
/* Description   : �f�[�^����M�pCRC16�Z�o							*/
/* Argument      : *pBuff	�f�[�^�o�b�t�@							*/
/*               : len		�f�[�^���ibyte�j						*/
/* Return value  : CRC16�Z�o�l										*/
/* Programmed by : (OFT)Toguchi										*/
/* Date          : 2016/12/23										*/
/* Note          : pBuff��NULL�łȂ����Ƃ��g�p���ŕۏ؂��邱��		*/
/********************************************************************/
static UI_16 S_SdDrv_CalcDataCrc16(const UI_8 *pBuff, UI_32 len)
{

	UI_16	crc;
	UI_32	i;

	crc = 0;
	for (i = 0; i < len; i++) {
		crc = S_crc16_table_l[ (crc >> 8) ^ pBuff[i] ] ^ (crc << 8);	/* ������̏ꍇ */
	}


	return crc;

}
