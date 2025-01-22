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
/*      使用ヘッダファイル      */
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
/* CRCチェック機能切替			*/
/*------------------------------*/
#define	SDDRV_CRC_ON			/* 有効にした場合、SDカードとのデータ送受信でCRCチェックを実施する */
#define SDDRV_CRC16_LEFT		/* CRC16算出方法:有効にした場合 左送り, 無効にした場合 右送り */
#define SDDRV_CRC16_XOR_0		/* CRC16 Xor方法: 有効にした場合 Xor 0x0000, 無効にした場合 Xor 0xFFFF */

/*------------------------------*/
/*        内部データ構造        */
/*------------------------------*/

typedef enum e_spierror {				/* [内部的エラーコード]		*/
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

typedef enum e_sd_init_type {			/* [SD初期化形式] */
	SD_INIT_TYPE_SD_INVALID = 0,		/* SD 初期化無効 */
	SD_INIT_TYPE_SD_VER1,				/* SD Ver1.Xとして初期化 */
	SD_INIT_TYPE_SD_VER2				/* SD Ver2.0以降として初期化 */
} E_SD_INIT_TYPE;

typedef enum e_resp_type {				/* [コマンドレスポンス形式] */
	RZ = 0,								/* NONE response */
	R1,									/* R1 response. Basic MMC response format */
	R2									/* R2 response. Used by SEND_STATUS(CMD13) in SPI mode */
} E_RESP_TYPE;


/*------------------------------*/
/*          内部マクロ          */
/*------------------------------*/
/* --- LIN-UART 初期動作設定データ--- */
#define INIT_SCR				(0x23U)		/* シリアル制御レジスタ */
											/* <07>		UPCL	: 0				(プログラマブルクリアビット 動作に影響しない) */
											/* <06>		MS		: 0				(マスタ/スレーブ機能選択ビット マスター) */
											/* <05>		SPI		: 1				(SPI対応ビット SPI通信) */
											/* <04>		RIE		: 0				(受信割込み許可ビット 禁止) */
											/* <03>		TIE		: 0				(送信割込み許可ビット 禁止) */
											/* <02>		TBIE	: 0				(送信バスアイドル割込み許可ビット 禁止) */
											/* <01>		RXE		: 1				(受信動作許可ビット 許可) */
											/* <00>		TXE		: 1				(送信動作許可ビット 許可) */

#define INIT_SMR				(0x4FU)		/* シリアルモードレジスタ */
											/* <07-05>	MD		: 010			(動作モード 動作モード2(CSIOモード)) */
											/* <04>		-		: 0				(予約bit) */
											/* <03>		SCINV	: 1				(シリアルクロック反転ビット L) */
											/* <02>		BDS		: 1				(転送方向選択ビット MSBファースト) */
											/* <01>		SCKE	: 1				(シリアルクロック出力許可ビット シリアルクロック出力許可) */
											/* <00>		SOE		: 1				(シリアルデータ出力許可ビット 出力許可) */

#define INIT_SSR				(0x83U)		/* シリアルステータスレジスタ */
											/* <07>		REC		: 1				(受信エラーフラグクリアビット クリア) */
											/* <06-04>	-		: 000			(予約bit) */
											/* <03>		ORE		: 0				(オーバランエラーフラグビット 書込み無効) */
											/* <02>		RDRF	: 0				(受信データフルフラグビット 書込み無効) */
											/* <01>		TDRE	: 1				(送信データエンプティフラグビット 書込み無効) */
											/* <00>		TBI		: 1				(送信バスアイドルフラグビット 書込み無効) */

#define INIT_ESCR				(0x00U)		/* 拡張通信制御レジスタ */
											/* <07>		SOP		: 0				(シリアル出力端子セットビット 動作に影響しない) */
											/* <06-05>	-		: 00			(予約bit) */
											/* <04-03>	WT		: 00			(データ送受信ウェイト選択ビット ウェイトなし) */
											/* <02-00>	L		: 000			(データ長選択ビット 8ビット) */

#define INIT_FCR0				(0x00U)		/* FIFO制御レジスタ0 */
											/* <07>		-		: 0				(予約bit) */
											/* <06>		FLST	: 0				(FIFO再送データロストフラグビット 書込み無効) */
											/* <05>		FLD		: 0				(FIFOポインタリロードビット 動作に影響しない) */
											/* <04>		FSET	: 0				(FIFOポインタ保存ビット 動作に影響しない) */
											/* <03>		FCL2	: 0				(FIFO2リセットビット 動作に影響しない) */
											/* <02>		FCL1	: 0				(FIFO1リセットビット 動作に影響しない) */
											/* <01>		FE2-	: 0				(FIFO2動作許可ビット 禁止) */
											/* <00>		FE1		: 0				(FIFO1動作許可ビット 禁止) */

#define INIT_FCR1				(0x00U)		/* FIFO制御レジスタ1 */
											/* <07-06>	-		: 00			(予約bit) */
											/* <05>		-		: 0				(予約bit) */
											/* <04>		FLSTE	: 0				(再送データロスト検出許可ビット 禁止) */
											/* <03>		FRIIE	: 0				(受信FIFOアイドル検出許可ビット 禁止) */
											/* <02>		FDRQ	: 0				(送信FIFOデータ要求ビット リセット) */
											/* <01>		FTIE-	: 0				(送信FIFO割込み許可ビット 禁止) */
											/* <00>		FSEL	: 0				(FIFO選択ビット 送信FIFO1、受信FIFO2) */

#define INIT_FBYTE				(0x0000U)	/* FIFOバイトレジスタ */
											/* <15-08>	FBYTE2	: 0				(FIFO2データ数表示ビット 0(未使用)) */
											/* <07-00>	FBYTE1	: 0				(FIFO1データ数表示ビット 0(未使用)) */


#define SCR_UPCL_SET		(0x80U)			/* プログラマブルクリアビットセットデータ */
#define SCR_RIE_BIT			(0x10U)			/* SCR.RIE: 受信割込み要求許可ビット:bit4 */
#define SCR_TIE_BIT			(0x08U)			/* SCR.TIE: 送信割込み要求許可ビット:bit3 */
#define SCR_RXE_BIT			(0x02U)			/* SCR.RXE: 受信許可ビット:bit1 */
#define SCR_TXE_BIT			(0x01U)			/* SCR.TXE: 送信許可ビット:bit0 */
#define SSR_REC_BIT			(0x80U)			/* 受信エラーフラグクリアビット:bit7 */
#define SMR_SOE_DISABLE		(UI_8)(~0x01U)	/* シリアルモードレジスタ データ出力許可ビット:bit0 無効 */
#define SMR_SCKE_DISABLE	(UI_8)(~0x02U)	/* シリアルモードレジスタ クロック出力許可ビット:bit1 無効 */
#define SMR_SCKE_MASK		(0x02U)			/* シリアルモードレジスタ クロック出力許可ビット抽出マスク */
#define FCR0_FE_DISABLE		(UI_8)(~0x03U)	/* FIFO制御レジスタ0 FIFO動作許可ビット:bit1-0 無効 */

/* --- LIN-UART 通常動作設定データ--- */
#define EXIRX_SSR			(0x04U)			/* ｽﾃｰﾀｽﾚｼﾞｽﾀ	受信ﾃﾞｰﾀ検出 ANDﾋﾞｯﾄ				*/
#define EMPTX_SSR			(0x02U)			/* ｽﾃｰﾀｽﾚｼﾞｽﾀ	送信ﾃﾞｰﾀ空	 ANDﾋﾞｯﾄ				*/
#define ERROR_SSR			(0x08U)			/* ｽﾃｰﾀｽﾚｼﾞｽﾀ	ｴﾗｰ=ｵｰﾊﾞﾗﾝ ANDﾋﾞｯﾄ					*/
#define SSR_TBI_BIT			(0x01U)			/* 送信バスアイドルフラグビット位置:bit0 */
#define SSR_RDRF_BIT		(0x04U)			/* 受信データフルフラグビット位置:bit2 */

/* -- LIN-UART ボーレート設定データ-- */
#define SCK_16MHZ_BR_4MBPS		(3U)		/* クロック16MHz, ボーレート4Mbps設定				*/
#define SCK_16MHZ_BR_2MBPS		(7U)		/* クロック16MHz, ボーレート2Mbps設定				*/
#define SCK_16MHZ_BR_1MBPS		(15U)		/* クロック16MHz, ボーレート1Mbps設定				*/
#define SCK_16MHZ_BR_500KBPS	(31U)		/* クロック16MHz, ボーレート500Kbps設定				*/
#define SCK_16MHZ_BR_400KBPS	(39U)		/* クロック16MHz, ボーレート400Kbps設定				*/


/* --- コマンドインデックス --- */
#define CMD_START_BIT           (UI_16)(0x40)	 					/* コマンドスタートビット								*/
#define GO_IDLE_STATE           (UI_16)(0 + CMD_START_BIT)			/* CMD0 : カードリセット								*/
#define SEND_OP_COND            (UI_16)(1 + CMD_START_BIT)			/* CMD1 : 初期化実行									*/
#define ALL_SEND_CID            (UI_16)(2 + CMD_START_BIT)			/* CMD2 : 全カードに対するCID番号送信要求				*/
#define SET_RELATIVE_ADDR       (UI_16)(3 + CMD_START_BIT)			/* CMD3 : カードに相対アドレス割り当て					*/
#define SET_DSR                 (UI_16)(4 + CMD_START_BIT)			/* CMD4 : カードのDSRをプログラムする					*/
#define SELECT_DESELECT_CARD    (UI_16)(7 + CMD_START_BIT)			/* CMD7 : カードの選択状態切り替え						*/
#define SEND_IF_COND            (UI_16)(8 + CMD_START_BIT)			/* CMD8 : インターフェース条件確認(SDカード専用)		*/
#define SEND_CSD                (UI_16)(9 + CMD_START_BIT)			/* CMD9 : CSDレジスタ読み出し							*/
#define SEND_CID                (UI_16)(10 + CMD_START_BIT)			/* CMD10: CIDレジスタ読み出し							*/
#define READ_DAT_UNTIL_STOP     (UI_16)(11 + CMD_START_BIT)			/* CMD11: データ転送終了までの間データを読み出す		*/
#define STOP_TRANSMISSION       (UI_16)(12 + CMD_START_BIT)			/* CMD12: データ転送終了								*/
#define SEND_STATUS             (UI_16)(13 + CMD_START_BIT)			/* CMD13: ステータスレジスタ送信要求					*/
#define SET_BUS_WIDTH_REGISTER  (UI_16)(14 + CMD_START_BIT)			/* CMD14: 												*/
#define GO_INACTIVE_STATE       (UI_16)(15 + CMD_START_BIT)			/* CMD15: カード状態をinactiveステートにする			*/
#define SET_BLOCKLEN            (UI_16)(16 + CMD_START_BIT)			/* CMD16: データブロック長設定							*/
#define READ_BLOCK              (UI_16)(17 + CMD_START_BIT)			/* CMD17: シングルブロック読み出し						*/
#define READ_MULTIPLE_BLOCK     (UI_16)(18 + CMD_START_BIT)			/* CMD18: マルチブロック読み出し						*/
#define WRITE_DAT_UNTIL_STOP    (UI_16)(20 + CMD_START_BIT)			/* CMD20: データ転送終了までの間データを書き込む		*/
#define SET_WR_BLK_ERASE_COUNT_ACMD23  (UI_16)(23 + CMD_START_BIT)	/* ACMD23: 書き込み開始時の消去ブロック数指定(SDカード専用)	*/
#define WRITE_BLOCK             (UI_16)(24 + CMD_START_BIT)			/* CMD24: シングルブロック書き込み						*/
#define WRITE_MULTIPLE_BLOCK    (UI_16)(25 + CMD_START_BIT)			/* CMD25: マルチブロック書き込み						*/
#define PROGRAM_CID             (UI_16)(26 + CMD_START_BIT)			/* CMD26: CIDレジスタのプログラミング					*/
#define PROGRAM_CSD             (UI_16)(27 + CMD_START_BIT)			/* CMD27: CSDレジスタのプログラミング					*/
#define SET_WRITE_PROT          (UI_16)(28 + CMD_START_BIT)			/* CMD28: ライトプロテクトビットの設定					*/
#define CLR_WRITE_PROT          (UI_16)(29 + CMD_START_BIT)			/* CMD29: ライトプロテクトビットのクリア				*/
#define SEND_WRITE_PROT         (UI_16)(30 + CMD_START_BIT)			/* CMD30: ライトプロテクトビットのステータス要求		*/
#define TAG_SECTOR_START        (UI_16)(32 + CMD_START_BIT)			/* CMD32: 連続消去範囲の先頭セクタアドレス設定			*/
#define TAG_SECTOR_END          (UI_16)(33 + CMD_START_BIT)			/* CMD33: 連続消去範囲の最終セクタアドレス設定			*/
#define UNTAG_SECTOR            (UI_16)(34 + CMD_START_BIT)			/* CMD34: 消去セクタの解除								*/
#define TAG_ERASE_GROUP_START   (UI_16)(35 + CMD_START_BIT)			/* CMD35: 連続消去時の先頭消去グループのアドレス設定	*/
#define TAG_ERASE_GROUP_END     (UI_16)(36 + CMD_START_BIT)			/* CMD36: 連続消去時の終端消去グループのアドレス設定	*/
#define UNTAG_ERASE_GROUP       (UI_16)(37 + CMD_START_BIT)			/* CMD37: 消去グループの解除							*/
#define ERASE_SECTORS           (UI_16)(38 + CMD_START_BIT)			/* CMD38: 選択セクタの消去								*/
#define SEND_OP_COND_ACMD41     (UI_16)(41 + CMD_START_BIT)			/* CMD41: SD専用コマンド								*/
#define APP_CMD                 (UI_16)(55 + CMD_START_BIT)			/* CMD55: アプリケーション特化コマンド					*/
#define READ_OCR                (UI_16)(58 + CMD_START_BIT)			/* CMD58: OCRレジスタ読み出し							*/
#define CRC_ON_OFF              (UI_16)(59 + CMD_START_BIT)			/* CMD59: CRCチェック機能の切り替え						*/

/* --- Statusレジスタのビットマスク定数--- */
#define OVERRUN                 (0x8000U)		/* オーバーラン(ライトエラー)					*/
#define CMD_PARAM_ERROR         (0x4000U)		/* パラメタエラー								*/
#define ADDRESS_ERROR           (0x2000U)		/* アドレスエラー								*/
#define ERASE_SEQ_ERROR         (0x1000U)		/* 消去エラー									*/
#define CMD_CRC_ERROR           (0x0800U)		/* コマンドCRCエラー							*/
#define COMUNC_ILLEG_COM        (0x0400U)		/* 不正コマンド									*/
#define ERASE_RESET             (0x0200U)		/* 消去リセット									*/
#define CARD_IS_NOT_READY       (0x0100U)		/* 動作不可										*/
#define CMD_PARAM_ERROR2        (0x0080U)		/* パラメタエラー2								*/
#define ERASE_PARAM             (0x0040U)		/* パラメタ消去									*/
#define WP_VIOLATION            (0x0020U)		/* ライトプロテクトブロックへの書き込み			*/
#define CARD_READ_FAILURE       (0x0010U)		/* リードエラー									*/
#define EERROR                  (0x0008U)		/* 汎用エラー									*/
#define ERROR2                  (0x0004U)		/* 汎用エラー2									*/
#define WP_ERASE_SKIP           (0x0002U)		/* ライトプロテクトブロックの消去スキップ		*/
#define UNDERRUN                (0x0001U)		/* アンダーラン(リードエラー)					*/

/* --- OCR レジスタのビットマスク定数--- */
#define OCR_BIT_MASK_BUSY		(0x80000000U)	/* BUSYビットマスク				*/

/* ----- その他データ----- */
#define DATA_TOKEN_MULTI_SEND   (UI_8)(0xFC)	/* データトークンスタートバイト(複数セクタ送信用) */
#define DATA_TOKEN_STOP_TRAN    (UI_8)(0xFD)	/* データトークン送信停止(複数セクタ送信用) */
#define DATA_TOKEN              (UI_8)(0xFE)	/* データトークンスタートバイト */
#define DUMMY_DATA              (UI_8)(0xFF)	/* ダミーデータ */
#define DEFAULT_BLK_LEN         (UI_16)(512)	/* デフォルト ブロック長 512byte */
#define CMD_BYTE_LENGTH         (6U)			/* コマンド長  */
#define CSD_BYTE_LENGTH         (17U)			/* CSDデータ長 */
#define CSD_BYTE_SIZE           (64U)			/* CSDデータのバイトサイズ */
#define OCR_BYTE_SIZE           (4U)			/* OCRデータのバイトサイズ */
#define RESP_DATA_BYTE_SIZE		(4U)			/* レスポンスデータのバイトサイズ */
#define ERASE_START_SECT        (0UL)			/* イレーススタートセクタ番号 */
#define HS_DATA_CNT             (9U)			/* HSデータ数 */
#define SPI_IDLE_LOOP           (0x0200U)		/* SPI_IDLEループ回数制限 */

/*------------------------------*/
/*           内部定数           */
/*------------------------------*/
/* CRC16 左送り用テーブル CRC-CCITT */
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
/*           内部変数           */
/*------------------------------*/
static T_CARD_INFO	S_card_info;

/* レジスタ */
static __io volatile UI_8 *volatile const C_SdReg_SMR = &(UART8_CSIO8_SMR8);			/* シリアルモードレジスタ			*/
static __io volatile UI_8 *volatile const C_SdReg_SCR = &(UART8_CSIO8_SCR8);			/* シリアル制御レジスタ				*/
static __io volatile UI_8 *volatile const C_SdReg_SSR = &(UART8_CSIO8_SSR8);			/* シリアルステータスレジスタ		*/
static __io volatile UI_8 *volatile const C_SdReg_ESCR = &(UART8_CSIO8_ESCR8);			/* 拡張通信制御レジスタ				*/
static __io volatile UI_8 *volatile const C_SdReg_TDR = &(UART8_CSIO8_TDR8_TDR8L);		/* 送信データレジスタ				*/
static __io volatile UI_8 *volatile const C_SdReg_RDR = &(UART8_CSIO8_RDR8_RDR8L);		/* 受信データレジスタ				*/
static __io volatile UI_16 *volatile const C_SdReg_BGR = &(UART8_CSIO8_BGR8);			/* ボーレートジェネレータレジスタ	*/
static __io volatile UI_8 *volatile const C_SdReg_FCR0 = &(UART8_CSIO8_FCR08);			/* FIFO制御レジスタ0				*/
static __io volatile UI_8 *volatile const C_SdReg_FCR1 = &(UART8_CSIO8_FCR18);			/* FIFO制御レジスタ1				*/
static __io volatile UI_16 *volatile const C_SdReg_FBYTE = &(UART8_CSIO8_FBYTE8);		/* FIFOバイトレジスタ				*/

/*------------------------------*/
/*         内部関数宣言         */
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
/*         外部I/F関数          */
/*------------------------------*/
/********************************************************************/
/* Name          : SDCardDrv_Init                                   */
/* Description   : SDカードドライバの初期設定                       */
/* Argument      : なし                                             */
/* Return value  : なし                                             */
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
/* Description   : SDカードインターフェースの初期設定               */
/* Argument      : なし                                             */
/* Return value  : SDCARDDRV_NOERROR      エラーなし                */
/*                 SDCARDDRV_NOCARD       カードなし                */
/*                 SDCARDDRV_CARDERROR    カードエラー              */
/*                 SDCARDDRV_FORMATERROR  フォーマットエラー        */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/10/05                                       */
/* Note          :                                                  */
/********************************************************************/
E_SDDRRET SDCardDrv_SdInit( void )
{
	E_SPIERROR		resErr;						/* SPIエラー戻り値用変数 */
	E_SD_INIT_TYPE	initType;
	UI_8 bufCSD[CSD_BYTE_SIZE];					/* CSDデータ用配列 */
	E_SD_ADR_MODE	adrMode;
	UI_8 loop_end_flg;							/* ループ終了フラグ */
	UI_16 i;

	/* カード情報構造体初期化 */
	S_CardInfo_Init();

	/* カード有無チェック */
	if ( S_Card_Exist() == D_FALSE ) {
		return SDCARDDRV_NOCARD;		/* カード無し */
	}

	/* UARTレジスタ初期設定 */
	S_UartReg_Init();

	/* SPIモードに設定(CMD0送信)  */
	resErr = S_Set_SPIMode();			
	if ( resErr != SPI_CARD_IS_NOT_READY ) {
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit CMD0 ERR!! ret:%d\n", resErr);)
		return SDCARDDRV_CARDERROR;
	}

	/* インターフェース条件確認(CMD8送信) */
	resErr = S_CheckSdIfCnd();
	if (resErr == SPI_NO_ERROR) {		/* インターフェース条件一致 */
		/* SD Ver2.0以降 */
		initType = SD_INIT_TYPE_SD_VER2;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit VER2.0 or later\n");)
	}
	else if ((resErr == SPI_CARD_IS_NOT_RESPONDING) ||	/* SDからの応答なし				*/
			 (resErr == SPI_COMUNC_ILLEG_COM)) {		/* コマンド(CMD8)未対応			*/
		/* SD Ver2.0以前 または SD以外	*/
		initType = SD_INIT_TYPE_SD_VER1;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit VER1.X or Not Sd\n");)
	}
	else {
		return SDCARDDRV_CARDERROR;
	}

	/* SPIモードオペレーションレディチェック(ACMD41送信) */
	i = 1000;
	loop_end_flg = D_OFF;
	while (( i > 0 ) && ( loop_end_flg == D_OFF )) {
		if ( S_CheckSPIReady(initType) == D_OK ) {
			/* SPIモードオペレーションレディ状態 */
			loop_end_flg = D_ON;
			OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit SPI Ready OK cnt:%d\n", i);)
		}
		else {
			i--;
			S_Wait_1ms();				/* 次回のS_CheckSPIReadyまで1msのディレイをかける */
		}
	}
	if ( loop_end_flg == D_OFF ) {		/* タイムアウト */
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit ACMD41 ERR!!\n");)
		return SDCARDDRV_CARDERROR;
	}

	/* CRCチェックON(CMD59送信)*/
	resErr = S_Set_CrcOnOff(D_ON);
	if ( resErr != SPI_NO_ERROR ) {
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit S_Set_CrcOnOff ERR!! ret:%d\n", resErr);)
		return SDCARDDRV_CARDERROR;
	}

	/* SDカードアドレッシングモード取得(OCRデータ取得)(CMD58送信)*/
	if (initType == SD_INIT_TYPE_SD_VER2) {
		if (S_Get_AddressingMode( &adrMode ) != D_OK) {
			OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit CMD58 ERR!!\n");)
			return SDCARDDRV_FORMATERROR;	/* フォーマットエラー */
		}
	}
	else {
		adrMode = SD_ADR_MODE_BYTE;
	}

	/* CSDデータ取得(CMD9送信) */
	resErr = S_Get_CSD( bufCSD );
	if ( resErr != SPI_NO_ERROR) {
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit CMD9 ERR!! ret:%d\n", resErr);)
		return SDCARDDRV_FORMATERROR;	/* フォーマットエラー */
	}

	/* カード情報設定(SD VER1はCMD16でセクタサイズ512を設定) */
	resErr = S_Set_CardInfo( bufCSD, adrMode, &S_card_info );
	if ( resErr != SPI_NO_ERROR ) {
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_SdInit S_Set_CardInfo ERR!! ret:%d\n", resErr);)
		return SDCARDDRV_CARDERROR;
	}

	/* ボーレート設定 */
	S_Set_BaudRate( SCK_16MHZ_BR_500KBPS );

	return SDCARDDRV_NOERROR;
}

/********************************************************************/
/* Name          : SDCardDrv_Read                                   */
/* Description   : カードデータリード(セクタ単位)                   */
/* Argument      : *pBuff       読み出したセクタ分のデータを格納    */
/*                 ulSectorNo   セクタ番号                          */
/*                 ulSectorCnt  セクタ数                            */
/* Return value  : SDCARDDRV_NOERROR      エラーなし                */
/*                 SDCARDDRV_NOCARD       カードなし                */
/*                 SDCARDDRV_READERROR    読み出しエラー            */
/*                 SDCARDDRV_BUSY         BUSY状態                  */
/*                 SDCARDDRV_ERROR        その他のエラー            */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/10/01                                       */
/* Note          : 指定されたセクタからデータ読み出し               */
/********************************************************************/
E_SDDRRET SDCardDrv_Read( UI_8 *pBuff, UI_32 ulSectorNo, UI_32 ulSectorCnt )
{
	E_SDDRRET ret = SDCARDDRV_CARDERROR;	/* 戻り値用変数 */
	E_SPIERROR resErr;		/* SPIエラー戻り値用変数 */
	UI_32 total_Sect;		/* 全セクタ数   */

	total_Sect = S_card_info.ulTotal_Secotors;

	if ( S_Card_Exist() == D_FALSE ) {	/* カード有無チェック */
		ret = SDCARDDRV_NOCARD;			/* カード無し */
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Read ERR!!!! NOCARD, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else if ( pBuff == D_NULL ) {		/* 第1引数チェック */
		ret = SDCARDDRV_ERROR;
	}
	else if ( total_Sect == 0UL ) {		/* 全セクタ数チェック */
		ret = SDCARDDRV_READERROR;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Read ERR!!!! Sector=0, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else if ( (ulSectorCnt == 0UL) ||
			(ulSectorNo >= total_Sect) ||
			((ulSectorNo + ulSectorCnt) > total_Sect) ) {	/* 第2,第3引数チェック */
		ret = SDCARDDRV_READERROR;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Read ERR!!!! SectorErr, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else if ( S_CheckCardBusy( D_OFF ) == D_NG ) {
		/* Busy状態 */
		ret = SDCARDDRV_BUSY;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Read ERR!!!! Busy, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else {
		/* 引数チェックOK */
		ret = SDCARDDRV_READERROR;
		if (ulSectorCnt == 1) {
			/* 1セクタリード */
			resErr = S_Read_SingleBlock(pBuff, ulSectorNo);
		}
		else {
			/* マルチセクタリード */
			resErr = S_Read_MultiBlock(pBuff, ulSectorNo, ulSectorCnt);
		}
		if ( resErr == SPI_NO_ERROR ) {		/* エラー判定 */
			ret = SDCARDDRV_NOERROR;		/* エラーなし */
		}
		else {
			OUTLOG(OutputLog("[SDDRV]SDCardDrv_Read ERR!!!! err:%d, ofssect:%u, sectCnt:%u\n", resErr, ulSectorNo, ulSectorCnt);)
		}
	}
	return ret;
}


/********************************************************************/
/* Name          : SDCardDrv_Write                                  */
/* Description   : カードデータライト(セクタ単位)                   */
/* Argument      : *pBuff       書き込み用データが格納されている    */
/*                 ulSectorNo   セクタ番号                          */
/*                 ulSectorCnt  セクタ数                            */
/* Return value  : SDCARDDRV_NOERROR      エラーなし                */
/*                 SDCARDDRV_NOCARD       カードなし                */
/*                 SDCARDDRV_WRITEPROHIB  書き込み禁止              */
/*                 SDCARDDRV_WRITEERROR   書き込みエラー            */
/*                 SDCARDDRV_BUSY         BUSY状態                  */
/*                 SDCARDDRV_ERROR        その他のエラー            */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/10/01                                       */
/* Note          : 指定されたセクタにデータ書き込み                 */
/********************************************************************/
E_SDDRRET SDCardDrv_Write( const UI_8 *pBuff, UI_32 ulSectorNo, UI_32 ulSectorCnt )
{
	E_SDDRRET ret = SDCARDDRV_CARDERROR;	/* 戻り値用変数 */
	E_SPIERROR resErr;		/* SPIエラー戻り値用変数 */
	UI_32 total_Sect;		/* 全セクタ数   */

	total_Sect = S_card_info.ulTotal_Secotors;

	if ( S_Card_Exist() == D_FALSE ) {		/* カード有無チェック */
		ret = SDCARDDRV_NOCARD;				/* カード無し */
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Write ERR!!!! NOCARD, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else if ( S_Card_Protect() == D_ON ) {	/* プロテクトチェック */
		ret = SDCARDDRV_WRITEPROHIB;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Write ERR!!!! Protect, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else if ( pBuff == D_NULL ) {			/* 第1引数チェック */
		ret = SDCARDDRV_ERROR;
	}
	else if ( total_Sect == 0UL ) {			/* 全セクタ数チェック */
		ret = SDCARDDRV_WRITEERROR;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Write ERR!!!! Sector=0, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else if ( (ulSectorCnt == 0UL) ||
			(ulSectorNo >= total_Sect) ||
			((ulSectorNo + ulSectorCnt) > total_Sect) ) {	/* 第2,第3引数チェック */
		ret = SDCARDDRV_WRITEERROR;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Write ERR!!!! SectorErr, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else if ( S_CheckCardBusy( D_OFF ) == D_NG ) {
		/* Busy状態 */
		ret = SDCARDDRV_BUSY;
		OUTLOG(OutputLog("[SDDRV]SDCardDrv_Write ERR!!!! Busy, ofssect:%u, sectCnt:%u\n", ulSectorNo, ulSectorCnt);)
	}
	else {
		/* 引数チェックOK */
		ret = SDCARDDRV_WRITEERROR;
		if (ulSectorCnt == 1) {
			/* 1セクタライト */
			resErr = S_Write_SingleBlock(pBuff, ulSectorNo);
		}
		else {
			/* マルチセクタライト */
			resErr = S_Write_MultiBlock(pBuff, ulSectorNo, ulSectorCnt);
		}
		if ( resErr == SPI_NO_ERROR ) {		/* エラー判定 */
			ret = SDCARDDRV_NOERROR;		/* エラーなし */
		}
		else {
			OUTLOG(OutputLog("[SDDRV]SDCardDrv_Write ERR!!!! err:%d, ofssect:%u, sectCnt:%u\n", resErr, ulSectorNo, ulSectorCnt);)
		}
	}
	return ret;
}


/********************************************************************/
/* Name          : SDCardDrv_GetCardInfo                            */
/* Description   : カード情報の取得                                 */
/* Argument      : *pCardInfo    カード情報を格納するための領域     */
/* Return value  : SDCARDDRV_NOERROR      エラーなし                */
/*                 SDCARDDRV_NOCARD       カードなし                */
/*                 SDCARDDRV_ERROR        その他のエラー            */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
E_SDDRRET SDCardDrv_GetCardInfo( T_CARD_INFO *pCardInfo )
{
	E_SDDRRET ret = SDCARDDRV_CARDERROR;	/* 戻り値用変数 */
	B_8 card_exist;			/* カード有無	*/

	if ( pCardInfo == D_NULL ) {		/* 第1引数チェック */
		ret = SDCARDDRV_ERROR;
	}
	else {
		/* カード有無情報の取得とセット */
		card_exist = S_Card_Exist();
		S_card_info.bCardExist = card_exist;

		if ( card_exist == D_FALSE ) {		/* カード有無チェック */
			/* カード無の場合 */
			/* カード情報設定 */
			S_card_info.uProtectState = D_OFF;
			S_card_info.ulTotal_Secotors = 0UL;
			S_card_info.AdrMode = SD_ADR_MODE_UNKNOWN;

			ret = SDCARDDRV_NOCARD;
		}
		else {
			/* カード有の場合 */
			/* 初期化処理で取得されたカード情報を設定 */
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
/*           内部宣言           */
/*------------------------------*/
/********************************************************************/
/* Name          : S_CS_OnOff                                       */
/* Description   : チップセレクト設定                               */
/* Argument      : opt          チップセレクトON (D_ON)             */
/*                                            OFF(D_OFF)            */
/* Return value  : なし                                             */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static void S_CS_OnOff( UI_8 opt )
{

	if ( opt == D_ON ) {
		/* チップセレクトON */
		(void)PortDrv_SetDataBit(IO_PO_GR_SD_SPI_CS, IO_PO_BIT_SD_SPI_CS, IO_PO_LEV_ACTIVE_SD_SPI_CS);
	}
	else if ( opt == D_OFF ) {
		/* チップセレクトOFF */
		(void)PortDrv_SetDataBit(IO_PO_GR_SD_SPI_CS, IO_PO_BIT_SD_SPI_CS, IO_PO_LEV_NONACT_SD_SPI_CS);
	}
	else {
	}

}

/********************************************************************/
/* Name          : S_Card_Exist                                     */
/* Description   : カード有無判定                                   */
/* Argument      : なし                                             */
/* Return value  : D_TRUE       カード有                            */
/*                 D_FALSE      カード無                            */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static B_8 S_Card_Exist( void )
{

	B_8  ret;		/* 戻り値用変数 */
	UI_8 port_lev;
	SI_8 ret_port;

	ret_port = PortDrv_GetDataBit(IO_PI_GR_SD_DTCT, IO_PI_BIT_SD_DTCT, &port_lev);
	if (ret_port != D_OK) {
		OUTLOG(OutputLog("[SDDRV]S_Card_Exist PortDrv_GetDataBit ERR!!!!\n");)
		ret = D_FALSE;			/* SDカードなし */
		S_CardInfo_Init();
	}
	else {
		if (port_lev == IO_PI_LEV_ACTIVE_SD_DTCT) {
			/* OUTLOG(OutputLog("[SDDRV]S_Card_Exist SD EXIST\n");) */
			ret = D_TRUE;		/* SDカード検出 */
		}
		else {
			/* OUTLOG(OutputLog("[SDDRV]S_Card_Exist SD NON\n");) */
			ret = D_FALSE;		/* SDカードなし */
			S_CardInfo_Init();
		}
	}

	return ret;

}

/********************************************************************/
/* Name          : S_Card_Protect                                   */
/* Description   : カードプロテクト有無判定                         */
/* Argument      : なし                                             */
/* Return value  : D_ON         プロテクト有                        */
/*                 D_OFF        プロテクト無                        */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static UI_8 S_Card_Protect( void )
{

	UI_8 ret;		/* 戻り値用変数 */
	UI_8 port_lev;
	SI_8 ret_port;

	ret_port = PortDrv_GetDataBit(IO_PI_GR_SD_WP, IO_PI_BIT_SD_WP, &port_lev);
	if (ret_port != D_OK) {
		OUTLOG(OutputLog("[SDDRV]S_Card_Protect PortDrv_GetDataBit ERR!!!!\n");)
		ret = D_OFF;			/* プロテクト無 */
	}
	else {
		if (port_lev == IO_PI_LEV_ACTIVE_SD_WP) {
			/* OUTLOG(OutputLog("[SDDRV]S_Card_Protect SD Write Protect OFF\n");) */
			ret = D_OFF;		/* プロテクト無 */
		}
		else {
			/* OUTLOG(OutputLog("[SDDRV]S_Card_Protect SD Write Protect ON\n");) */
			ret = D_ON;			/* プロテクト有 */
		}
	}

	return ret;

}

/********************************************************************/
/* Name          : S_CardInfo_Init                                  */
/* Description   : カード情報構造体初期化                           */
/* Argument      : なし                                             */
/* Return value  : なし                                             */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static void S_CardInfo_Init( void )
{

	/* カード情報の構造体を初期化 */
	S_card_info.bCardExist = D_FALSE;
	S_card_info.uProtectState = D_OFF;
	S_card_info.ulTotal_Secotors = 0UL;
	S_card_info.AdrMode = SD_ADR_MODE_UNKNOWN;

}

/********************************************************************/
/* Name          : S_UartReg_Init                                   */
/* Description   : LIN-UARTレジスタ初期設定                         */
/* Argument      : なし                                             */
/* Return value  : なし                                             */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static void S_UartReg_Init( void )
{

	UI_8	smr_work;			/* シリアルモードレジスタ演算用work変数 */
	UI_8	scr_work;			/* シリアル制御レジスタ一時格納用work変数 */
	UI_8	clr_work;			/* ビットクリア用work変数 */
	UI_8	fcr0_work;			/* FIFO制御レジスタ0設定用work変数 */
	UI_8	tmp_scr_mask;		/* 送受信禁止及び送受信割込み禁止設定用work変数 */

	/* 送受信禁止及び送受信割込み禁止を設定前にシリアルデータ出力を禁止に設定する */
	/* (SOTポートの髭状パルスの発生防止) */
	smr_work		= *C_SdReg_SMR;
	smr_work		&= SMR_SOE_DISABLE;
	*C_SdReg_SMR	= smr_work;

	/* プログラマブルクリアの実行前に送受信禁止及び送受信割込み禁止を設定する */
	tmp_scr_mask =	(UI_8)( ~( SCR_RIE_BIT | SCR_TIE_BIT | SCR_RXE_BIT | SCR_TXE_BIT ) );
	scr_work		= *C_SdReg_SCR;
	scr_work		&= tmp_scr_mask;
	*C_SdReg_SCR	= scr_work;

	/* プログラマブルクリアの実行前にFIFO1及びFIFO2を禁止に設定する */
	/* (FIFOは使用しない思想だが禁止の設定を先に集中して行う方針) */
	fcr0_work		= *C_SdReg_FCR0;
	fcr0_work		&= FCR0_FE_DISABLE;
	*C_SdReg_FCR0	= fcr0_work;

	/* リロード値を即有効にするためにプログラマブルクリアの実行前にBGRレジスタを設定する */
	S_Set_BaudRate( SCK_16MHZ_BR_400KBPS );

	/* プログラマブルクリア */
	clr_work		= *C_SdReg_SCR;
	clr_work		|= SCR_UPCL_SET;
	*C_SdReg_SCR	= clr_work;

	/* SMRレジスタ:SCKE以外のビットを先に設定 */
	/* 他のレジスタを設定する前にMDビットを設定する必要がある */
	smr_work		= *C_SdReg_SMR;
	smr_work		&= SMR_SCKE_MASK;
	smr_work		|= (UI_8)(INIT_SMR & SMR_SCKE_DISABLE);
	*C_SdReg_SMR	= smr_work;
	
	/* SMRレジスタ:SCKEビットを含めて再度設定 */
	/* (SOTポートの髭状パルスの発生防止) */
	*C_SdReg_SMR	= INIT_SMR;

	/* 他のレジスタはSMRレジスタのMDビットを設定した後に設定する */
	/* (設定順序は動作開始要求と合わせる) */
	*C_SdReg_FCR1	= INIT_FCR1;
	*C_SdReg_FCR0	= INIT_FCR0;
	*C_SdReg_FBYTE	= INIT_FBYTE;
	*C_SdReg_SSR	= INIT_SSR;
	*C_SdReg_ESCR	= INIT_ESCR;

	/* 他のレジスタを全て設定してからシリアル制御レジスタを設定する */
	*C_SdReg_SCR	= INIT_SCR;

	return;

}

/********************************************************************/
/* Name          : S_Set_BaudRate                                   */
/* Description   : ボーレートジェネレータ設定                       */
/* Argument      : baudrate     リロード値                          */
/* Return value  : なし                                             */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          : クロックの設定に注意                             */
/********************************************************************/
static void S_Set_BaudRate( UI_16 baudrate )
{

	*C_SdReg_BGR = baudrate;			/* ボーレートジェネレータ */

}


/********************************************************************/
/* Name          : S_Set_SPIMode                                    */
/* Description   : SPIモード設定                                    */
/* Argument      : なし                                             */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/10/05                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Set_SPIMode( void )
{

	E_SPIERROR resErr;		/* 戻り値用変数 */
	UI_8  j;

	S_StartSD80Clocks();	/* 80clock	*/

	S_CS_OnOff( D_ON );

	resErr = S_CommandAndResponse (			/* CMD0(GO_IDLE_STATE) */
				GO_IDLE_STATE,
				0UL,
				R1 );
	(void)S_SPIExchgData( DUMMY_DATA, &j );

	/* この時点では、In Idle Stateレスポンスが返ってくるはず */
	if ( resErr != SPI_CARD_IS_NOT_READY ) {
		resErr = SPI_INTERFACE_ERROR;
		OUTLOG(OutputLog("S_Set_SPIMode S_CommandAndResponse ERR!! :%d\n", resErr);)
	}

	S_CS_OnOff( D_OFF );

	return resErr;

}

/********************************************************************/
/* Name          : S_CheckSdIfCnd                                   */
/* Description   : SDカードインターフェース条件確認                 */
/* Argument      : なし                                             */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/MM/DD                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_CheckSdIfCnd( void )
{

	E_SPIERROR resErr;		/* 戻り値用変数 */
	UI_8  bufRespR7[RESP_DATA_BYTE_SIZE];
	UI_8  i;
	UI_8  j;

	S_CS_OnOff( D_ON );

	resErr = S_CommandAndResponse (	/* CMD8(SEND_IF_COND) */
				SEND_IF_COND,
				0x000001AA,		/* [31:12] 予約									*/
								/* [11: 8] 電圧供給(VHS) :0001					*/
								/*			0000:Not Defined					*/
								/*			0001:2.7-3.6V						*/
								/*			0010:Reserved for Low Voltage Range	*/
								/*			0100:Reserved						*/
								/*			1000:Reserved						*/
								/*			Others Not Defined					*/
								/* [ 7: 0] チェックパターン :10101010			*/
				R1 );

	if (resErr == SPI_CARD_IS_NOT_READY) {
		/* R7のレスポンスデータを取得 */
		for (i = 0; i < RESP_DATA_BYTE_SIZE; i++) {
			if (S_SPIExchgData( DUMMY_DATA, &bufRespR7[i] ) == D_NG) {
				break;
			}
		}
		if (i == RESP_DATA_BYTE_SIZE) {	/* レスポンスデータ取得成功 */
			if ((bufRespR7[2] == 0x01) &&	/* 電圧供給 2.7-3.6V OK			*/
				(bufRespR7[3] == 0xAA)) {	/* チェックパターン一致			*/
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
/* Description   : SPIモードオペレーションレディチェック            */
/* Argument      : なし                                             */
/* Return value  : D_OK         Ready状態                           */
/*                 D_NG         Idle状態(又はエラー)                */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/10/05                                       */
/* Note          :                                                  */
/********************************************************************/
static SI_8 S_CheckSPIReady( E_SD_INIT_TYPE initType )
{

	SI_8 ret;				/* 戻り値用変数 */
	E_SPIERROR resErr;		/* 戻り値用変数 */
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
	if ( resErr == SPI_NO_ERROR ) {			/* エラー判定 */
		/* Ready状態(SPI_CARD_IS_NOT_READY→SPI_NO_ERRORに変わる) */
		ret = D_OK;
	}
	
	S_CS_OnOff( D_OFF );

	return ret;

}

/********************************************************************/
/* Name          : S_Set_CrcOnOff                                   */
/* Description   : CRCチェック設定                                  */
/* Argument      : OnOff	CRCチェック ON:D_ON, OFF:D_OFF          */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/12/21                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Set_CrcOnOff( UI_8 OnOff )
{

	E_SPIERROR	resErr;		/* 戻り値用変数 */
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
/* Description   : 80クロック出力                                   */
/* Argument      : なし                                             */
/* Return value  : なし                                             */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static void S_StartSD80Clocks( void )
{

	UI_16 i;
	UI_8  j;

	/* カード初期化80クロックを出力 */
	for ( i = 0; i < 10; i++ ) {			/* 8bit * 10 = 80 */
		(void)S_SPIExchgData( DUMMY_DATA, &j );
	}

}


/********************************************************************/
/* Name          : S_Wait_1ms                                       */
/* Description   : 1ms待ち                                          */
/* Argument      : なし                                             */
/* Return value  : なし                                             */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static void S_Wait_1ms( void )
{

	UI_16 i;
	UI_8  j;

	/* 約1ms待ち */
	for ( i = 0; i < 30; i++ ) {
		(void)S_SPIExchgData( DUMMY_DATA, &j );
	}

}


/********************************************************************/
/* Name			: S_Receive_Data									*/
/* Description	: データトークン受信								*/
/* Argument		: pBuff			受信データ格納領域					*/
/*				  dLength		受信データ長						*/
/* Return value	: E_SPIERROR	エラーメッセージ					*/
/* Programmed by: H.Mitsumoto										*/
/* Date			: 2004/06/17										*/
/* Update		: 2004/06/29										*/
/* Note			: トークン待ちｶｳﾝﾀを0x0032から0x0064に変更			*/
/********************************************************************/
static E_SPIERROR S_Receive_Data( UI_8 *pBuff, UI_16 dLength )
{

	UI_8 *buff_ptr;			/* 受信データ用領域     */
	UI_8 dToken;			/* データトークン用領域 */
	UI_8 wk_ssr;			/* 作業用変数           */
	volatile UI_8 wk_rdr;	/* 作業用変数           */
	UI_16 i;
	UI_32 startTime1ms;
	B_8 timeOut;
	UI_16	crc;
	UI_16	crc_calc;

	if ( pBuff == D_NULL ) {		/* 第1引数チェック */
		return SPI_ERROR;
	}

	/* データトークン受信を待つ */
	timeOut = D_TRUE;
	startTime1ms = Timer_ctrl_GetTime1ms();
	while (Timer_ctrl_TimeCompare1ms(startTime1ms, 200) == N_NG) {	/* タイムアウト200ms */
		/* カード有無チェック */
		if ( S_Card_Exist() == D_FALSE ) {
			OUTLOG(OutputLog("[SDDRV]S_Receive_Data SD Card Non!!\n");)
			return SPI_INTERFACE_ERROR;
		}

		/* データトークン・スタートバイト受信を判定 */
		if ( S_SPIExchgData( DUMMY_DATA, &dToken ) == D_NG ) {
			OUTLOG(OutputLog("[SDDRV]S_Receive_Data StartToken ERR!!\n");)
			return SPI_INTERFACE_ERROR;
		}

		if ( dToken == DATA_TOKEN ) {
			/* データトークン受信完了 */
			timeOut = D_FALSE;
			break;
		}
		else if (dToken == 0xFF) {
			/* NOP */
		}
		else {
			/* 取得データ不正	*/
			OUTLOG(OutputLog("[SDDRV]RcvDataErr!! 0x%02X\n", dToken);)

			/* SDからデータが送られてくるため空読みする	*/
			for ( i = 0; i < dLength ; i++ ) {
				(void)S_SPIExchgData( DUMMY_DATA, &dToken );
			}
			/* 16Bit CRCの受信 */
			(void)S_SPIExchgData( DUMMY_DATA, &dToken );
			(void)S_SPIExchgData( DUMMY_DATA, &dToken );
			return SPI_INTERFACE_ERROR;
		}
	}
	if (timeOut == D_TRUE) {
		OUTLOG(OutputLog("[SDDRV]S_Receive_Data StartToken TimeOut!!\n");)
		return SPI_INTERFACE_ERROR;
	}

	/* データ部受信 */
	buff_ptr = pBuff;
	for ( i = 0; i < dLength ; i++ ) {
		/* S_SPIExchgData関数を呼び出して利用すると処理が遅いため、同様の処理を直接記述する */
		/* カード有無チェック */
		if ( S_Card_Exist() == D_FALSE ) {
			OUTLOG(OutputLog("[SDDRV]S_Receive_Data SD Card Non!!\n");)
			return SPI_INTERFACE_ERROR;
		}

		/* 送信データが存在しているかチェック */
		timeOut = D_TRUE;
		startTime1ms = Timer_ctrl_GetTime1ms();
		while (Timer_ctrl_TimeCompare1ms(startTime1ms, 200) == N_NG) {	/* タイムアウト200ms */
			if ( (*C_SdReg_SSR & SSR_TBI_BIT ) == SSR_TBI_BIT ) {
				timeOut = D_FALSE;
				break;
			}
		}
		if (timeOut == D_TRUE) {
			OUTLOG(OutputLog("[SDDRV]S_Receive_Data Send TimeOut!!\n");)
			return SPI_INTERFACE_ERROR;
		}

		/* 送信データレジスタに出力データをセット */
		*C_SdReg_TDR = DUMMY_DATA;

		/* 受信完了をポーリングで判定 */
		timeOut = D_TRUE;
		startTime1ms = Timer_ctrl_GetTime1ms();
		while (Timer_ctrl_TimeCompare1ms(startTime1ms, 200) == N_NG) {	/* タイムアウト200ms */
			wk_ssr = *C_SdReg_SSR;			/* UART0のステータス読出し */
			if (( wk_ssr & ERROR_SSR ) != 0 ) {	/* エラーあり */
				*C_SdReg_SSR = wk_ssr | SSR_REC_BIT;	/* エラーフラグクリア */
				wk_rdr = *C_SdReg_RDR;					/* ダミーで取得       */
				OUTLOG(OutputLog("[SDDRV]S_Receive_Data Rcv ERR!!\n");)
				return SPI_INTERFACE_ERROR;
			}

			if ( (wk_ssr & EXIRX_SSR) != 0 ) {			/* 受信データあり？ */
				*buff_ptr++ = (UI_8)(*C_SdReg_RDR & (UI_8)0xFF);	/* 受信データ取得   */
				timeOut = D_FALSE;
				break;
			}
		}
		if (timeOut == D_TRUE) {
			OUTLOG(OutputLog("[SDDRV]S_Receive_Data Rcv TimeOut!!\n");)
			return SPI_INTERFACE_ERROR;
		}
	}

	/* エラーなし */
	/* 16Bit CRCの受信 */
	(void)S_SPIExchgData( DUMMY_DATA, &dToken );
	crc = ((UI_16)dToken) << 8;
	(void)S_SPIExchgData( DUMMY_DATA, &dToken );
	crc = crc | (UI_16)dToken;
	crc_calc = S_SdDrv_CalcDataCrc16(pBuff, (UI_32)dLength);
	if (crc != crc_calc) {
		/* CRCエラー */
		OUTLOG(OutputLog("[SDDRV]S_Receive_Data CRC ERR!!\n");)
		return SPI_DATA_CRC_ERROR;
	}

	return SPI_NO_ERROR;

}


/********************************************************************/
/* Name          : S_Get_CSD                                        */
/* Description   : CSDレジスタ値取得                                */
/* Argument      : なし                                             */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Get_CSD( UI_8 *respCSD )
{

	E_SPIERROR resErr;		/* 戻り値用変数 */

	if ( respCSD == D_NULL ) {			/* 引数チェック */
		resErr = SPI_ERROR;
	}
	else {
		S_CS_OnOff( D_ON );

		resErr = S_CommandAndResponse (	/* CMD9(SEND_CSD) */
					SEND_CSD, 
					0UL,
					R1 );
		if ( resErr == SPI_NO_ERROR ) {
			/* CSDレジスタデータ取得 */
			resErr = S_Receive_Data( respCSD, (UI_16)(CSD_BYTE_LENGTH - 1) );
		}

		S_CS_OnOff( D_OFF );
	}

	return resErr;

}

/********************************************************************/
/* Name          : S_Get_AddressingMode                             */
/* Description   : SDカードのアドレッシングモード取得               */
/* Argument      : [ /O]mode    アドレッシングモード                */
/*                              (SD_ADR_MODE_xx)                    */
/* Return value  : D_OK         正常                                */
/*                 D_NG         異常                                */
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
				/* バイトアドレッシング */
				*mode = SD_ADR_MODE_BYTE;
			}
			else {
				/* ブロックアドレッシング */
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
/* Description   : OCRレジスタ値取得                                */
/* Argument      : なし                                             */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/MM/DD                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Get_OCR( UI_8 *bufOCR )
{

	E_SPIERROR resErr;		/* 戻り値用変数 */
	UI_8 i;
	UI_8 j;

	if ( bufOCR == D_NULL ) {			/* 引数チェック */
		resErr = SPI_ERROR;
	}
	else {
		S_CS_OnOff( D_ON );

		resErr = S_CommandAndResponse (	/* CMD58(READ_OCR) */
					READ_OCR, 
					0UL,
					R1 );
		if ( resErr == SPI_NO_ERROR ) {
			/* OCRレジスタデータ取得 */
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
/* Description   : カード情報のセット                               */
/* Argument      : *respCSD     CSDデータが格納されている領域       */
/*                 adrMode      アドレッシングモード                */
/*                 *wkcardinfo  カード情報を格納するための領域      */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Set_CardInfo( UI_8 *respCSD, E_SD_ADR_MODE adrMode, T_CARD_INFO *wkcardinfo )
{

	E_SPIERROR resErr;		/* 戻り値用変数 */
	UI_32 c_size;			/* C_SIZE       */
	UI_16 c_size_mult_1;	/* C_SIZE_MULT_1 */
	UI_16 c_size_mult_2;	/* C_SIZE_MULT_2 */
	UI_16 blklen;			/* ブロック長   */
	UI_32 total_Secotors;	/* トータルセクター数 */
	UI_16 ui16temp;
	UI_32 ui32temp;

	/* 引数チェック */
	if (( respCSD == D_NULL ) || ( wkcardinfo == D_NULL )) {
		return SPI_ERROR;
	}

	/* カード情報構造体初期化 */
	S_CardInfo_Init();

	/* wkcardinfo構造体にデータをセット */
	wkcardinfo -> bCardExist = S_Card_Exist();
	wkcardinfo -> uProtectState = S_Card_Protect();
	wkcardinfo -> AdrMode = adrMode;

	/* トータルブロック数を算出 */
	if ((respCSD[0] & 0xC0U) == 0x00U) {		/* CSD Ver1.0 */
		/* CSDレジスタ値からデータブロック長を獲得 */
		ui16temp = (UI_16)( (UI_16)respCSD[5] & 0x0FU );
		blklen = (UI_16)( 1U << ui16temp );					/* データブロック長(Byte) */

		/* ブロック長チェック */
		if ( blklen != DEFAULT_BLK_LEN ) {
			/* ブロック長が512byte以外の場合、SDカードにブロック長512byteを設定する */
			resErr = S_Set_BlockLen( DEFAULT_BLK_LEN );	/* ブロック長を設定 */
			if ( resErr != SPI_NO_ERROR ) {
				return resErr;
			}
			resErr = S_Get_CSD ( respCSD );			/* CSDデータ取得 */
			if ( resErr != SPI_NO_ERROR ) {
				return resErr;
			}

			/* CSDレジスタ値からデータブロック長を獲得 */
			ui16temp = (UI_16)( (UI_16)respCSD[5] & 0x0FU );
			blklen = (UI_16)( 1U << ui16temp );					/* データブロック長(Byte) */
		}

		/* CSDレジスタ値から、カード容量を計算 */
		ui16temp = (UI_16)( (UI_16)respCSD[6] & 3U );		/* C_SIZEフィールドの獲得 */
		ui16temp <<= 8;
		c_size = (UI_32)respCSD[7] + (UI_32)ui16temp;
		c_size <<= 2;
		ui16temp = (UI_16)( (UI_16)((UI_16)respCSD[8] >> 6U) & 3U );
		c_size += ( (UI_32)ui16temp + 1UL );

		c_size_mult_1 = (UI_16)( (UI_16)respCSD[9] & 3U );	/* C_SIZE_MULT フィールドの獲得 */
		c_size_mult_1 <<= 1;
		c_size_mult_2 = (UI_16)( (UI_16)((UI_16)respCSD[10] >> 7U) & 1U );

		/* トータルブロック数 BLOCKNR */
		c_size <<= ((c_size_mult_1 + c_size_mult_2 + 2));	/* (C_SIZE+1) <<= (C_SIZE_MULT+2) */
		/* 2GBのSDはブロック長がDEFAULT_BLK_LEN(512byte)以外のケースがあるため */
		/* CSDレジスタから取得したブロック長でサイズを算出してDEFAULT_BLK_LENに対応したブロックサイズにする */
		total_Secotors = (c_size * blklen) / DEFAULT_BLK_LEN;
	}
	else if ((respCSD[0] & 0xC0U) == 0x40U) {	/* CSD Ver2.0 */
		/* CSDレジスタ値から、カード容量を計算 */
		ui32temp = (UI_32)respCSD[7] & 0x3FU;	/* C_SIZEフィールドの獲得 */
		ui32temp <<= 8;
		ui32temp = (UI_32)respCSD[8] + ui32temp;
		ui32temp <<= 8;
		c_size = (UI_32)respCSD[9] + ui32temp;
		c_size += 1UL;

		/* トータルブロック数 BLOCKNR */
		c_size <<= 10;										/* (C_SIZE+1) <<= 10 */
		total_Secotors = c_size;
	}
	else {			/* MMC */
		/* 未対応 */
		return SPI_ERROR;
	}


	/* wkcardinfo構造体にデータをセット */
	wkcardinfo -> ulTotal_Secotors = total_Secotors;

	return SPI_NO_ERROR;

}

/********************************************************************/
/* Name          : S_Set_BlockLen                                   */
/* Description   : ブロック長設定                                   */
/* Argument      : blocklen     設定ブロック長                      */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Set_BlockLen( UI_16 blocklen )
{

	E_SPIERROR resErr;		/* 戻り値用変数 */
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
/* Description   : コマンドトーク送信とレスポンス受信               */
/* Argument      : cmd          コマンドインデックス                */
/*                 arg          コマンド引数                        */
/*                 resp         レスポンスタイプ                    */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_CommandAndResponse ( UI_16 cmd, UI_32 arg, E_RESP_TYPE resp )
{

	E_SPIERROR	resErr;		/* 戻り値用変数 */
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

	/* コマンドトークンを送信 */
	if ( S_PrepareAndSetup( cmd, arg ) == SPI_NO_ERROR ) {
		if (cmd == STOP_TRANSMISSION) {					/* CMD12(STOP_TRANSMISSION)を送信した	*/
			/* ダミーデータ1byte送信	*/
			(void)S_SPIExchgData( DUMMY_DATA, &rcvData );
		}

		/* レスポンスを受信 */
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
/* Description   : コマンドトークン作成・送信                       */
/* Argument      : cmd          コマンドインデックス                */
/*                 arg          コマンド引数                        */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_PrepareAndSetup( UI_16 cmd, UI_32 args )
{

	E_SPIERROR resErr;		/* 戻り値用変数 */

	if ( S_SendCommand( cmd, args ) == D_OK ) {
		/* S_SendCommand 正常終了*/
		resErr = SPI_NO_ERROR;
	}
	else {
		/* S_SendCommand 異常終了*/
		resErr = SPI_ERROR;
		OUTLOG(OutputLog("[SDDRV]S_PrepareAndSetup S_SendCommand ERR!!!! cmd:0x%04X arg:%d\n", cmd, args);)
	}

	return resErr;

}

/********************************************************************/
/* Name          : S_SendCommand                                    */
/* Description   : コマンドトークン送信                             */
/* Argument      : cmd_index    コマンドインデックス                */
/*                 arg          コマンド引数                        */
/* Return value  : D_OK         正常終了                            */
/*                 D_NG         異常終了                            */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static SI_8 S_SendCommand( UI_16 command, UI_32 args )
{

	SI_8 ret;				/* 戻り値用変数 */
	UI_8 *cmd_ptr;			/* 送信用コマンドデータ */
	UI_8 cmd_bytes[CMD_BYTE_LENGTH];		/* コマンドデータ用領域 */
	UI_8 *cmd_bytes_ptr = &cmd_bytes[0];	/* コマンドデータ用領域 */
	UI_8 loop_end_flg;		/* ループ終了フラグ */
	UI_16 i;
	UI_8  j;

	/* コマンドトークンの生成 */
	cmd_bytes[0] = (UI_8)command;		/* コマンド番号 */
	for ( i = 4; i > 0; i-- ) {			/* コマンド引数 */
		cmd_bytes[i] = (UI_8)(args & 0xFFUL);
		args >>= 8;
	}
	cmd_bytes[5] = S_SdDrv_CalcCmdCrc7( cmd_bytes );

	/* コマンドトークンの送信 */
	cmd_ptr = cmd_bytes_ptr;
	loop_end_flg = D_OFF;
	for ( i = 0; (( i < 6 ) && ( loop_end_flg == D_OFF )); i++ ) {
		if ( S_SPIExchgData( cmd_ptr[i], &j ) == D_NG ) {
			loop_end_flg = D_ON;
		}
	}
	if ( loop_end_flg == D_OFF ) {
		/* 正常終了 */
		ret = D_OK;
	}
	else {
		/* 異常終了 */
		ret = D_NG;
		OUTLOG(OutputLog("[SDDRV]S_SendCommand S_SPIExchgData ERR!!!! sendbyte:%d\n", i);)
	}

	return ret;

}


/********************************************************************/
/* Name          : S_SPIExchgData                                   */
/* Description   : SPIバスで１バイトデータ通信を行う                */
/* Argument      : odata        送信データ                          */
/*                 *idata	    受信データ格納領域                  */
/* Return value  : D_OK         受信データあり                      */
/*                 D_NG         受信データなし                      */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          : UARTの送信許可は初期設定で事前に行うこと         */
/********************************************************************/
static SI_8 S_SPIExchgData( UI_8 odata, UI_8 *idata )
{

	UI_8 wk_ssr;
	volatile UI_8 wk_rdr;
	UI_32 startTime1ms;
	B_8 timeOut;

	if ( idata == D_NULL ) {	/* 引数チェック */
		return D_NG;
	}

	/* 転送完了をポーリングで判定 */
	timeOut = D_TRUE;
	startTime1ms = Timer_ctrl_GetTime1ms();
	while (Timer_ctrl_TimeCompare1ms(startTime1ms, 200) == N_NG) {	/* タイムアウト200ms */
		if ( (*C_SdReg_SSR & SSR_TBI_BIT ) == SSR_TBI_BIT ) {	/* 送信バスアイドル	*/
			timeOut = D_FALSE;
			break;
		}
	}
	if (timeOut == D_TRUE) {
		OUTLOG(OutputLog("S_SPIExchgData TimeOut!! SndData:0x%02X\n", odata);)
		return D_NG;
	}

	/* 送信データレジスタに出力データをセット */
	*C_SdReg_TDR = odata;

	/* 受信完了をポーリングで判定 */
	timeOut = D_TRUE;
	startTime1ms = Timer_ctrl_GetTime1ms();
	while (Timer_ctrl_TimeCompare1ms(startTime1ms, 200) == N_NG) {	/* タイムアウト200ms */
		wk_ssr = *C_SdReg_SSR;			/* UART0のステータス読出し */
		if (( wk_ssr & ERROR_SSR ) != 0 ) {	/* エラー */
			*C_SdReg_SSR = wk_ssr | SSR_REC_BIT;	/* エラーフラグクリア */
			wk_rdr = *C_SdReg_RDR;					/* ダミーで取得       */
			OUTLOG(OutputLog("S_SPIExchgData OverRunErr!! SndData:0x%02X RcvData:0x%02X\n", odata, *idata);)
			return D_NG;
		}

		if ( (wk_ssr & EXIRX_SSR) != 0 ) {			/* 受信データあり？ */
			*idata = (UI_8)(*C_SdReg_RDR & (UI_8)0xFF);	/* 受信データ取得   */
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
/* Description   : コマンドレスポンストークン受信                   */
/* Argument      : resptype     レスポンスタイプ                    */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_WaitForCmdResponse( E_RESP_TYPE respType )
{

	E_SPIERROR resErr;		/* 戻り値用変数 */
	UI_16 respStatus;		/* レスポンスステータス */

	/* レスポンストークンの受信 */
	resErr = S_Get_ResponseInfo( &respStatus, respType );
	if (( resErr != SPI_CARD_IS_NOT_RESPONDING ) && ( resErr != SPI_ERROR )) {
		/* レスポンスを解析し、内部エラー値に変換 */
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
/* Description   : コマンドレスポンストークン受信                   */
/* Argument      : resptype     レスポンスタイプ                    */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Get_ResponseInfo( UI_16 *respStatus, E_RESP_TYPE respType )
{

	E_SPIERROR resErr;		/* 戻り値用変数 */
	UI_8 loop_end_flg;		/* ループ終了フラグ */
	UI_8 iData;				/* 受信データ用領域 */
	UI_16 i;

	if ( respStatus == D_NULL ) {		/* 第1引数チェック */
		resErr = SPI_ERROR;
	}
	else {
		resErr = SPI_CARD_IS_NOT_RESPONDING;
		i = 10;							/* SDカードからの応答はコマンド送信後0～8byteの間にくる	*/
		loop_end_flg = D_OFF;
		while (( i > 0 ) && ( loop_end_flg == D_OFF )) {
			iData = 0xFF;
			if ( S_Card_Exist() == D_FALSE ) {			/* カード有無チェック[2005.03.18 変更L] */
				resErr = SPI_ERROR;
				loop_end_flg = D_ON;
			}
			else if ( S_SPIExchgData( DUMMY_DATA, &iData ) == D_NG ) {
				resErr = SPI_ERROR;
				loop_end_flg = D_ON;
				OUTLOG(OutputLog("S_Get_ResponseInfo S_SPIExchgData ERR!!\n");)
			}
			else {
				if ( ( iData & (UI_8)0x80 ) == 0 ) {		/* レスポンストークン受信 */
					resErr = SPI_NO_ERROR;
					*respStatus = (UI_16)( (UI_16)iData << 8 );
					if ( respType == R2 ) {		/* 2Byteタイプのレスポンストークン */
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
/* Description   : データ送信                                       */
/* Argument      : pBuff        送信データ格納領域                  */
/*                 dLength      送信データ長                        */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_Send_Data( const UI_8 *pBuff, UI_16 dLength, UI_8 sendToken )
{

	E_SPIERROR resErr;		/* 戻り値用変数 */
	UI_8 dToken;			/* データトークン用領域 */
	UI_8 wk_ssr;			/* 作業用変数           */
	volatile UI_8 wk_rdr;	/* 作業用変数           */
	UI_16 i;
	UI_16 j;
	UI_32 startTime1ms;
	B_8 timeOut;
	UI_16 crc;

	/* 引数チェック */
	if ( pBuff == D_NULL ) {
		return SPI_ERROR;
	}

	if (S_CheckCardBusy( D_ON ) == D_NG) {
		OUTLOG(OutputLog("[SDDRV]S_Send_Data Befor SD BUSY!!\n");)
		return SPI_CARD_IS_BUSY;
	}

	/* データトークン・スタートバイトの送信 */
	(void)S_SPIExchgData( DUMMY_DATA, &dToken );
	(void)S_SPIExchgData( sendToken, &dToken );

	/* データ送信 */
	for ( i = 0; i < dLength; i++ ) {
		/* S_SPIExchgData関数を呼び出して利用すると処理が遅いため、同様の処理を直接記述する */
		/* カード有無チェック */
		if ( S_Card_Exist() == D_FALSE ) {
			return SPI_INTERFACE_ERROR;
		}

		/* 送信データが存在しているかチェック */
		j = 0x0008;	/* 通常ここでループすることはない */
		while ( j > 0 ) {
			if ( (*C_SdReg_SSR & SSR_TBI_BIT ) == SSR_TBI_BIT ) {
				break;
			}
			j--;
		}
		if (j == 0) {
			return SPI_INTERFACE_ERROR;
		}

		/* 送信データレジスタに出力データをセット */
		*C_SdReg_TDR = pBuff[i];

		/* 受信完了をポーリングで判定 */
		timeOut = D_TRUE;
		startTime1ms = Timer_ctrl_GetTime1ms();
		while (Timer_ctrl_TimeCompare1ms(startTime1ms, 200) == N_NG) {	/* タイムアウト200ms */
			wk_ssr = *C_SdReg_SSR;			/* UART0のステータス読出し */
			if (( wk_ssr & ERROR_SSR ) != 0 ) {	/* エラー */
				/* エラーあり */
				*C_SdReg_SSR = wk_ssr | SSR_REC_BIT;	/* エラーフラグクリア */
				wk_rdr = *C_SdReg_RDR;					/* ダミーで取得       */
				OUTLOG(OutputLog("S_Send_Data Rcv ERR!! SndCnt:%d\n", i);)
				return SPI_INTERFACE_ERROR;
			}

			if ( (wk_ssr & EXIRX_SSR) != 0 ) {			/* 受信データあり？ */
				dToken = (UI_8)(*C_SdReg_RDR & (UI_8)0xFF);	/* 受信データ取得   */
				timeOut = D_FALSE;
				break;
			}
		}
		if (timeOut == D_TRUE) {
			OUTLOG(OutputLog("S_Send_Data Rcv TimeOut!! SndCnt:%d\n", i);)
			return SPI_INTERFACE_ERROR;
		}
	}

	/* データトークン・16bit CRCを送信 */
	crc = S_SdDrv_CalcDataCrc16(pBuff, (UI_32)dLength);
	(void)S_SPIExchgData( (UI_8)(crc >> 8), &dToken );
	(void)S_SPIExchgData( (UI_8)(crc & (UI_8)0xFF), &dToken );

	/* カードからデータレスポンストークンを受信する */
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
/* Description   : データレスポンストークン受信                     */
/* Argument      : なし                                             */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static E_SPIERROR S_CRCErrOnWrite( void )
{

	E_SPIERROR resErr = SPI_CARD_IS_NOT_RESPONDING;		/* 戻り値用変数 */
	UI_8 dDat;											/* 受信データ用領域 */

	if ( S_Card_Exist() == D_TRUE ) {
		if ( S_SPIExchgData( DUMMY_DATA, &dDat ) == D_OK ) {	/* カード有無チェック追加[2005.03.18 変更L] */
			if ( (dDat & 0x1F) == 0x05 ) {
				resErr = SPI_NO_ERROR;	/* データ受信が正常に完了 */
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
/* Description   : Busyチェック                                     */
/* Argument      : なし                                             */
/* Return value  : D_OK         Ready状態                           */
/*                 D_NG         Busy状態                            */
/* Programmed by : H.Mitsumoto                                      */
/* Date          : 2004/06/17                                       */
/* Note          :                                                  */
/********************************************************************/
static SI_8 S_CheckCardBusy( UI_8 curCsStat )
{

	SI_8 ret;				/* 戻り値用変数 */
	UI_8 datState;			/* 出力信号状態 */
	UI_32 startTime1ms;

	if (curCsStat == D_OFF) {
		S_CS_OnOff( D_ON );
	}

	ret = D_NG;

	startTime1ms = Timer_ctrl_GetTime1ms();
	while (Timer_ctrl_TimeCompare1ms(startTime1ms, 500) == N_NG) {	/* タイムアウト500ms */

		/* Busy状態では、DataOutがLowになっているので、0x00を受信している */
		datState = 0;
		if ( S_SPIExchgData( DUMMY_DATA, &datState)  == D_NG ) {
			break;
		}
		else {
			if ( datState == (UI_8)0xFF ) {
				/* DataOutがHiになったので、Ready状態へ移行 */
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
/* Description   : カードデータリード(1セクタ)                      */
/* Argument      : *pBuff       読み出した１セクタ分のデータを格納  */
/*                 ulSectorNo   セクタ番号                          */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/10/01                                       */
/* Note          : 指定されたセクタからデータ(512byte)読み出し      */
/********************************************************************/
static E_SPIERROR S_Read_SingleBlock( UI_8 *pBuff, UI_32 ulSectorNo )
{

	E_SPIERROR resErr;		/* SPIエラー戻り値用変数 */
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
		resErr = S_Receive_Data( pBuff, DEFAULT_BLK_LEN ); /* データブロックの受信 */
	}

	S_CS_OnOff( D_OFF );
	(void)S_SPIExchgData( DUMMY_DATA, &rcvData );

	return resErr;

}

/********************************************************************/
/* Name          : S_Read_MultiBlock                                */
/* Description   : カードデータリード(複数セクタ)                   */
/* Argument      : *pBuff       読み出したデータを格納              */
/*                 ulSectorNo   セクタ番号                          */
/*                 ulSectorCnt  セクタ数                            */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/10/01                                       */
/* Note          : 指定されたセクタからデータ読み出し               */
/********************************************************************/
static E_SPIERROR S_Read_MultiBlock( UI_8 *pBuff, UI_32 ulSectorNo, UI_32 uSectorCnt )
{

	E_SPIERROR resErr;		/* SPIエラー戻り値用変数 */
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

	/* CMD18 (READ_MULTIPLE_BLOCK)送信（複数ブロックの受信） */
	resErr = S_CommandAndResponse(READ_MULTIPLE_BLOCK, block, R1);
	if (resErr == SPI_NO_ERROR) {
		/* データ受信	*/
		for (i = 0; i < uSectorCnt; i++) {
			resErr = S_Receive_Data(pBuff + (i * DEFAULT_BLK_LEN), DEFAULT_BLK_LEN);
			if (resErr != SPI_NO_ERROR) {
				OUTLOG(OutputLog("[SDDRV]S_Read_MultiBlock RcvData ERR!!\n");)
				break;
			}
		}

		/* CMD12 (STOP_TRANSMISSION)受信停止 */
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
/* Description   : カードデータライト(1セクタ)                      */
/* Argument      : *pBuff       書き込み用データが格納されている    */
/*                 ulSectorNo   セクタ番号                          */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/10/01                                       */
/* Note          : 指定されたセクタにデータ(512byte)書き込み        */
/********************************************************************/
static E_SPIERROR S_Write_SingleBlock( const UI_8 *pBuff, UI_32 ulSectorNo )
{

	E_SPIERROR resErr;		/* SPIエラー戻り値用変数 */
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
		resErr = S_Send_Data(pBuff, DEFAULT_BLK_LEN, DATA_TOKEN);	/* WRITEデータブロックをカードへ送信 */
	}

	S_CS_OnOff( D_OFF );
	(void)S_SPIExchgData( DUMMY_DATA, &rcvData );

	return resErr;

}

/********************************************************************/
/* Name          : S_Write_MultiBlock                               */
/* Description   : カードデータライト(複数セクタ)                   */
/* Argument      : *pBuff       書き込み用データが格納されている    */
/*                 ulSectorNo   セクタ番号                          */
/*                 ulSectorCnt  セクタ数                            */
/* Return value  : E_SPIERROR   エラーメッセージ                    */
/* Programmed by : (OFT)Toguchi                                     */
/* Date          : 2016/10/01                                       */
/* Note          : 指定されたセクタにデータ書き込み                 */
/********************************************************************/
static E_SPIERROR S_Write_MultiBlock( const UI_8 *pBuff, UI_32 ulSectorNo, UI_32 uSectorCnt )
{

	E_SPIERROR resErr;		/* SPIエラー戻り値用変数 */
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

	/* CMD55(APP_CMD)送信	*/
	resErr = S_CommandAndResponse(APP_CMD, 0UL, R1);
	if (resErr == SPI_NO_ERROR) {

		/* ACMD23(SET_WR_BLK_ERASE_COUNT_ACMD23)送信（送信データブロック数を通知）	*/
		resErr = S_CommandAndResponse(SET_WR_BLK_ERASE_COUNT_ACMD23, uSectorCnt, R1);
		if (resErr == SPI_NO_ERROR) {

			/* CMD25 (WRITE_MULTIPLE_BLOCK)送信（複数ブロックの送信） */
			resErr = S_CommandAndResponse(WRITE_MULTIPLE_BLOCK, block, R1);
			if (resErr == SPI_NO_ERROR) {

				/* データ送信	*/
				for (i = 0; i < uSectorCnt; i++) {
					resErr = S_Send_Data(pBuff + (i * DEFAULT_BLK_LEN), DEFAULT_BLK_LEN, DATA_TOKEN_MULTI_SEND);
					if (resErr != SPI_NO_ERROR) {
						OUTLOG(OutputLog("[SDDRV]S_Write_MultiBlock SendData ERR!!\n");)
						break;
					}
				}

				if (resErr == SPI_NO_ERROR) {
					/* 送信停止	*/
					(void)S_SPIExchgData(DATA_TOKEN_STOP_TRAN, &rcvData);
				}
				else {
					/* 送信エラー時はCMD12(STOP_TRANSMISSION)を送信して停止する	*/
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
/* Description   : コマンド送信用CRC7算出							*/
/* Argument      : *pBuff       送信コマンドバッファ				*/
/* Return value  : CRC7算出値										*/
/* Programmed by : (OFT)Toguchi										*/
/* Date          : 2016/12/23										*/
/* Note          : pBuffがNULLでないことを使用側で保証すること		*/
/********************************************************************/
static UI_8 S_SdDrv_CalcCmdCrc7( const UI_8 *pBuff )
{

	/* CRCチェック有効時の処理 */
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
/* Description   : データ送受信用CRC16算出							*/
/* Argument      : *pBuff	データバッファ							*/
/*               : len		データ長（byte）						*/
/* Return value  : CRC16算出値										*/
/* Programmed by : (OFT)Toguchi										*/
/* Date          : 2016/12/23										*/
/* Note          : pBuffがNULLでないことを使用側で保証すること		*/
/********************************************************************/
static UI_16 S_SdDrv_CalcDataCrc16(const UI_8 *pBuff, UI_32 len)
{

	UI_16	crc;
	UI_32	i;

	crc = 0;
	for (i = 0; i < len; i++) {
		crc = S_crc16_table_l[ (crc >> 8) ^ pBuff[i] ] ^ (crc << 8);	/* 左送りの場合 */
	}


	return crc;

}
