/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: FR81S								*/
/* Date				: 16/08/18							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Sakata						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		:									*/
/* date				:									*/
/********************************************************/
/********************************************************/
/* file DTO459_VDV_Gnss_ctrl.c							*/
/* GNSS制御												*/
/********************************************************/

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/

#include <type.h>
#include <system.h>
#include <string.h>
#include "DTO459_VDV_Gnss_ctrl.h"
#include "System_control_TASK.h"
#include "DTO459_VDV_Gnss_Uart_Drv.h"
#include "DTO459_VDV_Can_ctrl.h"
#include "DTO459_VDV_Sw_ctrl.h"
#include "DTO459_VDV_Advance_ctrl.h"
#include "timer_ctrl.h"
#include "dbglog.h"

/********************************************************/
/*	マクロ定義											*/
/********************************************************/
#define GNSS_CTRL_LOCAL	static

#define GNSS_UART_CH		(GNSS_UART_DRV_UART_CH_9)	/* 制御対象のUARTチャンネル				*/
#define RX_BUF_SIZE			2048							/* 受信バッファサイズ					*/
#define TX_BUF_SIZE			200							/* 送信バッファサイズ					*/

#define GNSS_REST_ON_DULATION			( 10 / 1)		/* GNSSリセットオン区間時間(1ms単位)		*/
#define WAIT_GNSS_MODULE_DULATION		(600 / 1)		/* GNSSモジュール安定待ち時間(1ms単位)		*/
#define UBX_BAUD_TX_WAIT_TIME			40				/* ボーレート変更コマンド送信完了相当時間	*/
#define ESF_MEAS_RATE					100				/* ESF-MEASメッセージ送信周期(ms)			*/

#define CMD_RESPONSE_TIMEOUT			(520 / 1)		/* コマンド応答対タイムアウト時間(1ms単位)	*/
#define CMD_SEND_RETRY_MAX				3				/* コマンド送信最大リトライ回数				*/
#define CMD_SEND_TIMEOUT_MAX			3				/* コマンド送信最大タイムアウトループ回数	*/

/* UBX メッセージヘッダ */
#define UBX_MSG_HEADER_1				0xb5			/* ヘッダ 1byte目						*/
#define UBX_MSG_HEADER_2				0x62			/* ヘッダ 2byte目						*/

/* UBX ACK */
#define CLASS_ID_ACK					0x05			/* クラスID								*/
#define MSG_ID_ACK						0x01			/* メッセージID : ACK					*/
#define MSG_ID_NACK						0x00			/*                NACK					*/

#define HNR_PVT_CLASS_ID				0x28			/* HNR-PVT クラスID						*/
#define HNR_PVT_MESSAGE_ID				0x00			/* HNR-PVT メッセージID					*/
#define HNR_PVT_MESSAGE_LEN 			  72			/* HNR-PVT メッセージ長（実データ部）	*/
#define NAV_DOP_CLASS_ID				0x01			/* NAV-DOP クラスID						*/
#define NAV_DOP_MESSAGE_ID				0x04			/* NAV-DOP メッセージID					*/
#define NAV_DOP_MESSAGE_LEN 			  18			/* NAV-DOP メッセージ長（実データ部）	*/
#define ESF_MEAS_CLASS_ID				0x10			/* ESF-MEAS クラスID					*/
#define ESF_MEAS_MESSAGE_ID				0x02			/* ESF-MEAS メッセージID				*/
#define ESF_MEAS_MESSAGE_LEN 			  12			/* ESF-MEAS メッセージ長（実データ部）	*/
#define ESF_RAW_CLASS_ID				0x10			/* ESF-RAW クラスID						*/
#define ESF_RAW_MESSAGE_ID				0x03			/* ESF-RAW メッセージID					*/
#define NAV_PVT_CLASS_ID				0x01			/* NAV-PVT クラスID						*/
#define NAV_PVT_MESSAGE_ID				0x07			/* NAV-PVT メッセージID					*/
#define NAV_PVT_MESSAGE_LEN				 92				/* NAV-PVT メッセージ長（実データ部）	*/
#define NAV_POSECEF_CLASS_ID			0x01			/* NAV-POSECEF クラスID					*/
#define NAV_POSECEF_MESSAGE_ID			0x01			/* NAV-POSECEF メッセージID				*/
#define NAV_POSECEF_MESSAGE_LEN			 20				/* NAV-POSECEF メッセージ長（実データ部）*/

/* ESF-RAW センサデータタイプ */
#define ESF_RAW_DATA_TYPE_X_AXIS_GYRO		14			/* X-axis Gyroscope						*/
#define ESF_RAW_DATA_TYPE_Y_AXIS_GYRO		13			/* Y-axis Gyroscope						*/
#define ESF_RAW_DATA_TYPE_Z_AXIS_GYRO		 5			/* Z-axis Gyroscope						*/
#define ESF_RAW_DATA_TYPE_X_AXIS_ACCEL		16			/* X-axis Accelerometer					*/
#define ESF_RAW_DATA_TYPE_Y_AXIS_ACCEL		17			/* X-axis Accelerometer					*/
#define ESF_RAW_DATA_TYPE_Z_AXIS_ACCEL		18			/* X-axis Accelerometer					*/
#define ESF_RAW_DATA_TYPE_GYRO_TEMP			12			/* Gyroscope Temperature				*/

/* ESF-MEAS センサデータタイプ */
#define ESF_MEAS_DATA_TYPE_X_AXIS_GYRO		14			/* X-axis Gyroscope						*/
#define ESF_MEAS_DATA_TYPE_Y_AXIS_GYRO		13			/* Y-axis Gyroscope						*/
#define ESF_MEAS_DATA_TYPE_Z_AXIS_GYRO		 5			/* Z-axis Gyroscope						*/
#define ESF_MEAS_DATA_TYPE_X_AXIS_ACCEL		16			/* X-axis Accelerometer					*/
#define ESF_MEAS_DATA_TYPE_Y_AXIS_ACCEL		17			/* X-axis Accelerometer					*/
#define ESF_MEAS_DATA_TYPE_Z_AXIS_ACCEL		18			/* X-axis Accelerometer					*/
#define ESF_MEAS_DATA_TYPE_GYRO_TEMP		12			/* Gyroscope Temperature				*/

/* ESF-MEAS データタイプ */
#define ESF_MEAS_DATA_TYPE_SPEED			11			/* Speed								*/

#define RECEIVE_DATA_BUF_SIZE	128						/* 受信データバッファサイズ	*/

#define GNSS_COMM_MODE_DATA_BUF_SIZE		TX_BUF_SIZE	/* GNSS通信モード用データバッファサイズ	*/
#define GNSS_MSG_RCV_TIMEOUT				(10000)		/* GNSSメッセージ受信タイムアウト(10000ms)	*/

/* GNSS制御状態 */
typedef enum {
	GNSS_STAT_IDLE = 0,					/* アイドル								*/
	GNSS_STAT_INITIALIZING,				/* 初期化中								*/
	GNSS_STAT_GNSS_SETTING,				/* GNSSセッティング中(ボーレート変更前）*/
	GNSS_STAT_BAUDRATE_CHANGING,		/* ボーレート変更中						*/
	GNSS_STAT_GNSS_SETTING_2,			/* GNSSセッティング中(ボーレート変更後）*/
	GNSS_STAT_WORKING,					/* 通常動作中							*/
	GNSS_STAT_WORKING_BAUDRATE_CHANGE,	/* 通常動作中のボーレート変更			*/
	GNSS_STAT_ERROR						/* エラー状態							*/
} E_GNSS_STAT;

/* コマンド送信状態 */
typedef enum {
	CMD_SEND_STAT_IDLE = 0,				/* アイドル（未送信）	*/
	CMD_SEND_STAT_RES_WAIT				/* 応答待ち（送信済み）	*/
} E_CMD_SEND_STAT;

/* コマンド応答 */
typedef enum {
	CMD_RESPONSE_ACK = 0,				/* ACK					*/
	CMD_RESPONSE_NACK,					/* NACK					*/
	CMD_RESPONSE_INVALID,				/* 不正応答				*/
	CMD_RESPONSE_NONE,					/* 応答なし				*/
	CMD_RESPONSE_ERROR					/* 受信エラー			*/
} E_CMD_RESPONSE;

/* メッセージ受信結果 */
typedef enum {
	RCV_RESULT_OK = 0,					/* 受信完了				*/
	RCV_RESULT_EMPTY,					/* 受信なし				*/
	RCV_RESULT_RECEIVING,				/* 受信中				*/
	RCV_RESULT_BUFFER_OVER,				/* バッファオーバー		*/
	RCV_RESULT_ERROR					/* 受信エラー			*/
} E_RCV_RESULT;

/* NAV情報有効フラグ */
typedef enum {
	NAV_INFO_ENABLE_NONE	 =  0,			/* すべて無効			*/
	NAV_INFO_ENABLE_HNR		 = (1 << 0),	/* NAV-HNR or HVR_PVT報有効		*/
	NAV_INFO_ENABLE_PVT		 = (1 << 1),	/* NAV-PVT情報有効		*/
	NAV_INFO_ENABLE_DOP		 = (1 << 2),	/* NAV-POSECEF情報有効	*/
	NAV_INFO_ENABLE_POSECEF	 = (1 << 3),	/* NAV-DOP情報有効		*/
	NAV_INFO_ENABLE_ESF_RAW	 = (1 << 4),	/* ESF-RAW情報有効		*/
	NAV_INFO_ENABLE_ESF_MEAS = (1 << 5)		/* ESF-MEAS情報有効		*/
} E_NAV_INFO_ENABLE;

/* ボーレート */
typedef enum {
	GNSS_CTRL_BAUDRATE_9600 = 0,			/* 9600bps				*/
	GNSS_CTRL_BAUDRATE_115200,				/* 115200bps			*/
	GNSS_CTRL_BAUDRATE_NUM					/* ボーレート数			*/
} E_GNSS_CTRL_BAUDRATE;

/********************************************************/
/*	構造体宣言											*/
/********************************************************/
/* GNSSコマンドデータ */
typedef struct t_gnss_cmd {
	const UI_8 *cmd;					/* コマンドデータ		*/
	UI_8 		cmd_len;				/* コマンドデータサイズ	*/
} T_gnss_cmd;

/* UBOXメッセージPrefix */
typedef struct {
	UI_8	header[2];					/* ヘッダ			*/
	UI_8	class_id;					/* クラスID			*/
	UI_8	msg_id;						/* メッセージID		*/
	UI_16	length;						/* データ長			*/
} T_ubx_msg_prefix;

/********************************************************/
/*	定数宣言											*/
/********************************************************/

/* CFG-ESFALG                                                    Length--->  bitfield------------->  yaw------------------>  pitch---->  roll----->  CheckSum->*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esflag[] = { 0xB5, 0x62, 0x06, 0x56, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x12 };

/* CFG-ESFDWT                                                    Length--->  flags---->  id------->  factorF-------------->  factorR-------------->  trkGaugeF>  trkGuageR>  wheelBase>  latency-->  quantError----------->  senTtagFactor-------->  wtCountMax----------->  senTtagMax----------->  freq  reserved1------------------------------>  CheckSum->*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfdwt[]  = {0xB5, 0x62, 0x06, 0x30, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x0A, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x7A};

/* CFG-ESFGAWT                                                   Length--->  ver>  id->  flags---->  senTtagFactor-------->  senTtagMax----------->  wtFactor------------->  wtQuantError--------->  wtCountMax----------->  wtLatency>  wtFrq gyFrq gyroAcc-->  gyroLatancy accelAcc->  accLatency  acFrq rsv1  spdDeadBnd  tcTblSaveRt gThd1 aThd1 reserve2------------->  reserve3------------->  CheckSum->*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfgawt[] = {0xB5, 0x62, 0x06, 0x2D, 0x34, 0x00, 0x00, 0x00, 0x00, 0x04, 0xE8, 0x03, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0, 0x04, 0x84, 0x20, 0x15, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0x8F};

/* CFG-ESFGWT                                                    Length--->  flags---->  id------->  wtFactor------------->  flags3--------------->  wtQuantError--------->  senTtagFactor-------->  wtCountMax----------->  senTtagMax----------->  wtLatency>  gyroLatancy wtFrq gyFrq spdDeadBand gyroAcc-------------->  tcTblSaveRt gThd1 rsv1  CheckSum->*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfgwt[]  = {0xB5, 0x62, 0x06, 0x29, 0x2C, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0, 0x04, 0x84, 0x15, 0x19, 0x57};

/* CFG-ESFLA                                                     Length--->  ver>  numCf rerserved1  lATyp rsv2  leverArmX>  leverArmY>  leverArmZ>  CheckSum->*/
/*車種により設定値を分ける*/
#if defined _VDV_86
/*86用設定*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xC8, 0x00, 0x1F, 0x00, 0x21, 0x00, 
                                                                                                     0x00, 0x00, 0xC0, 0x00, 0xC0, 0xFF, 0x3B, 0x00, 0x30, 0x9C};
#elif defined _VDV_VITZ
/*Vitz用設定*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xD8, 0x00, 0x1C, 0x00, 0x1A, 0x00, 
                                                                                                     0x00, 0x00, 0xEA, 0x00, 0xC2, 0xFF, 0x38, 0x00, 0x5F, 0x10};
#elif defined _VDV_MARKX
/*MarkX用設定*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xD2, 0x00, 0x21, 0x00, 0x2B, 0x00,
                                                                                                     0x00, 0x00, 0xDC, 0x00, 0x2D, 0x00, 0x43, 0x00, 0xD8, 0x13};
#elif defined _VDV_RCF
/*RCF用設定*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xB4, 0x00, 0x32, 0x00, 0x2E, 0x00, 
                                                                                                     0x00, 0x00, 0xD2, 0x00, 0x24, 0x00, 0x43, 0x00, 0xBB, 0xF9};
#elif defined _VDV_SUPRA
/*SUPRA用設定*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xB0, 0x00, 0x25, 0x00, 0x46, 0x00, 
                                                                                                     0x00, 0x00, 0x8C, 0x00, 0xFB, 0xFF, 0x23, 0x00, 0x32, 0x8A};
#elif defined _VDV_LC
/*LC用設定*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xB9, 0x00, 0x37, 0x00, 0x26, 0x00, 
                                                                                                     0x00, 0x00, 0xD2, 0x00, 0x38, 0x00, 0x41, 0x00, 0xCF, 0x77};
#elif defined _VDV_YARIS
/*YARIS用設定*/ 
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xC8, 0x00, 0x1F, 0x00, 0x1E, 0x00, 
                                                                                                     0x00, 0x00, 0xED, 0x00, 0x28, 0x00, 0x4B, 0x00, 0xD3, 0x4F};
#elif defined _VDV_86B
/*TODO 86B用設定*/ 
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xC8, 0x00, 0x1F, 0x00, 0x21, 0x00, 
                                                                                                     0x00, 0x00, 0xC0, 0x00, 0xC0, 0xFF, 0x3B, 0x00, 0x30, 0x9C};
#else
/*その他車種（86と同じ）*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xC8, 0x00, 0x1F, 0x00, 0x21, 0x00, 
                                                                                                     0x00, 0x00, 0xC0, 0x00, 0xC0, 0xFF, 0x3B, 0x00, 0x30, 0x9C};
#endif

/* CFG-GNSS:GNSS設定                                             Length--->  mgVer numHw numUs numBk gnsID resTC maxTC rsv   flags-----------------> CheckSum->*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_gnss[]    = {0xB5, 0x62, 0x06, 0x3E, 0x3C, 0x00, 0x00, 0x1C, 0x1C, 0x07, 0x00, 0x08, 0x10, 0x00, 0x01, 0x00, 0x01, 0x01,
                                                                                                     0x01, 0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0x01,
                                                                                                     0x02, 0x04, 0x08, 0x00, 0x00, 0x00, 0x01, 0x01,
                                                                                                     0x03, 0x08, 0x10, 0x00, 0x00, 0x00, 0x01, 0x01,
                                                                                                     0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x03,
                                                                                                     0x05, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x05,
                                                                                                     0x06, 0x08, 0x0E, 0x00, 0x00, 0x00, 0x01, 0x01, 0x4B, 0x4B};

/* CFG-PRT:ポート設定                                                    Length--->  PtID  rsv   txReady-->  mode----------------->  baudRate------------->  inPortMask  outPortMask flags---->  rsv------>  CheckSum->*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_prt_ddc[]         = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9E, 0x88};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_prt_usb[]         = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1D, 0x84};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_prt_spi[]         = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x98};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_prt_uart_115200[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xC0, 0x08, 0x00, 0x00, 0x00, 0xC2, 0x01, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0, 0x7E};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_prt_uart_9600[]   = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xC0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x92, 0xB5};

/* CFG-HNR:HNR計測レート設定                                     Length--->  Hz->  reserve1------->  CheckSum->*/
/*ADR OFFの場合は0Hz*/
/*0Hz*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_hnr[]     = {0xB5, 0x62, 0x06, 0x5C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0xCC};

/* CFG-RATE:計測レート設定                                    Length--->  measRate->  navRate-->  timeRef-->  CheckSum->*/
/*ADR OFFの場合は10Hz*/
/*10Hz*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_rate[] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00, 0x01, 0x00, 0x7A, 0x12};

/* CFG-MSG:メッセージ送信レート設定                                   Length--->  mgCls mgID  rate----------------------------->  CheckSum->*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_msg_nav_dop[]  = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x01, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x15, 0xCC};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_msg_hnr_pvt[] =  {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x28, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x38, 0xE8};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_msg_posecef[]  = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x12, 0xB7};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_msg_nav_pvt[]  = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x01, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x18, 0xE1};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_msg_nmea_gga[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x23};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_msg_nmea_gll[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_msg_nmea_gsa[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x31};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_msg_nmea_gsv[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x38};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_msg_nmea_rmc[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x3F};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_msg_nmea_vtg[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x46};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_msg_esf_meas[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x10, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x31};

/*CFG-NAVX5-*/
/* --- ADR OFF時の設定 --- */

#if defined GNSS_VER_410
/* Version 4.10以上（4.31も含む）*/
/*GNSSファームウェアバージョンアップ後(＜注意＞この設定コマンドはファームウェアアップデート後のみ使用可能)*/
/*メモ：Version 4.10からメッセージバージョンのみを変更したもの*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_navx5[] = {0xB5, 0x62, 0x06, 0x23, 0x2C, 0x00, 0x03, 0x00, 0x4C, 0x66, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4B, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA9, 0xCE};
#else /* 2021.07.06 S.Sakata Version4.10と4.31を共通化*/
/* Version 4.10未満*/
/*GNSSファームウェアバージョンアップ前*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_navx5[] = {0xB5, 0x62, 0x06, 0x23, 0x28, 0x00, 0x02, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x03, 0x02, 0x05, 0x18, 0x0C, 0x00, 0x00, 0x01, 0x00, 0x00, 0x4B, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x08, 0x64, 0x64, 0x00, 0x00, 0x01, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB5, 0xD4};
#endif /*GNSS_VER_410*/



/* ボーレート変更前発行コマンドテーブル */
GNSS_CTRL_LOCAL const T_gnss_cmd C_init_cmd_tbl[] = {
	{	C_ubx_cfg_esflag,		sizeof(C_ubx_cfg_esflag)		},
	{	C_ubx_cfg_esfdwt,		sizeof(C_ubx_cfg_esfdwt)		},
	{	C_ubx_cfg_esfgawt,		sizeof(C_ubx_cfg_esfgawt)		},
	{	C_ubx_cfg_esfgwt,		sizeof(C_ubx_cfg_esfgwt)		},
	{	C_ubx_cfg_esfla,		sizeof(C_ubx_cfg_esfla)			},
	{	C_ubx_cfg_gnss,			sizeof(C_ubx_cfg_gnss)			},
	{	C_ubx_cfg_hnr,			sizeof(C_ubx_cfg_hnr)			},
	{	C_ubx_cfg_prt_ddc,		sizeof(C_ubx_cfg_prt_ddc)		},
	{	C_ubx_cfg_prt_usb,		sizeof(C_ubx_cfg_prt_usb)		},
	{	C_ubx_cfg_prt_spi,		sizeof(C_ubx_cfg_prt_spi)		},
	{	C_ubx_cfg_msg_nmea_gga,	sizeof(C_ubx_cfg_msg_nmea_gga)	},
	{	C_ubx_cfg_msg_nmea_gll,	sizeof(C_ubx_cfg_msg_nmea_gll)	},
	{	C_ubx_cfg_msg_nmea_gsa,	sizeof(C_ubx_cfg_msg_nmea_gsa)	},
	{	C_ubx_cfg_msg_nmea_gsv,	sizeof(C_ubx_cfg_msg_nmea_gsv)	},
	{	C_ubx_cfg_msg_nmea_rmc,	sizeof(C_ubx_cfg_msg_nmea_rmc)	},
	{	C_ubx_cfg_msg_nmea_vtg,	sizeof(C_ubx_cfg_msg_nmea_vtg)	},
	{	0,						0								},	/* テーブル終端		*/
};

/* ボーレート変更コマンド */
GNSS_CTRL_LOCAL const T_gnss_cmd C_change_boudrate_cmd = {
	C_ubx_cfg_prt_uart_115200, sizeof(C_ubx_cfg_prt_uart_115200),
};

/* ボーレート変更後発行コマンドテーブル */
GNSS_CTRL_LOCAL const T_gnss_cmd C_init_cmd_tbl_2[] = {
	{	C_ubx_cfg_navx5,		sizeof(C_ubx_cfg_navx5)			},	/*ADR ON/OFF 設定用 */
	{	C_ubx_cfg_hnr,			sizeof(C_ubx_cfg_hnr)			},
	{	C_ubx_cfg_rate,			sizeof(C_ubx_cfg_rate)			},
	{	C_ubx_cfg_msg_nav_dop,	sizeof(C_ubx_cfg_msg_nav_dop)	},
	{	C_ubx_cfg_msg_hnr_pvt,	sizeof(C_ubx_cfg_msg_hnr_pvt)	},
	{	C_ubx_cfg_msg_posecef,	sizeof(C_ubx_cfg_msg_posecef)	},
	{	C_ubx_cfg_msg_nav_pvt,	sizeof(C_ubx_cfg_msg_nav_pvt)	},
	{	C_ubx_cfg_msg_esf_meas,	sizeof(C_ubx_cfg_msg_esf_meas)	},
	{	0,						0								},	/* テーブル終端		*/
};


/********************************************************/
/*	内部変数定義										*/
/********************************************************/
GNSS_CTRL_LOCAL UI_8		   S_uart_rx_buf[RX_BUF_SIZE];			/* UART受信バッファ			*/
GNSS_CTRL_LOCAL UI_8		   S_uart_er_buf[RX_BUF_SIZE];			/* UART受信エラーバッファ	*/
GNSS_CTRL_LOCAL UI_8		   S_uart_tx_buf[TX_BUF_SIZE];			/* UART送信バッファ			*/
GNSS_CTRL_LOCAL E_GNSS_STAT S_gnss_stat  = GNSS_STAT_IDLE;		/* GNSS制御状態				*/	
GNSS_CTRL_LOCAL UI_16       S_proc_phase_count = 0;				/* 処理フェーズカウント		*/
GNSS_CTRL_LOCAL UI_32       S_proc_phase_timestamp = 0;			/* 処理フェーズ変化時のタイムスタンプ	*/
GNSS_CTRL_LOCAL E_CMD_SEND_STAT	S_cmd_send_status = CMD_SEND_STAT_IDLE;		/* コマンド送信状態			*/
GNSS_CTRL_LOCAL UI_8        S_cmd_send_retry_count = 0;			/* コマンド送信リトライカウント		*/
GNSS_CTRL_LOCAL UI_8        S_cmd_send_timeout_count = 0;		/* コマンド送信タイムアウトカウント	*/

GNSS_CTRL_LOCAL UI_8 S_receive_data_buf[RECEIVE_DATA_BUF_SIZE];	/* GNSS受信データバッファサイズ	*/
GNSS_CTRL_LOCAL UI_8 S_receive_error_buf[RECEIVE_DATA_BUF_SIZE];	/* GNSS受信エラーバッファサイズ	*/
GNSS_CTRL_LOCAL UI_8 S_receive_data_size = 0;					/* GNSS受信データサイズ			*/

GNSS_CTRL_LOCAL UI_8					    S_nav_info_enable;		/* NAV情報有効フラグ		*/
GNSS_CTRL_LOCAL T_Gnss_ctrl_hnr_pvt_info	S_hnr_pvt_info;			/* HNR-PVT情報				*/
GNSS_CTRL_LOCAL T_Gnss_ctrl_nav_pvt_info	S_nav_pvt_info;			/* NAV-PVT情報				*/
GNSS_CTRL_LOCAL T_Gnss_ctrl_nav_dop_info	S_nav_dop_info;			/* NAV-DOP情報				*/
GNSS_CTRL_LOCAL T_Gnss_ctrl_nav_posecef_info	S_nav_posecef_info;	/* NAV-POSECEF情報			*/
GNSS_CTRL_LOCAL T_Gnss_ctrl_esf_raw_info	S_nav_esf_raw;			/* ESF-RAW情報				*/
GNSS_CTRL_LOCAL T_Gnss_ctrl_esf_meas_info	S_nav_esf_meas;		/* ESF-MEAS情報				*/
#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata */
GNSS_CTRL_LOCAL UI_8						S_fixtype_enabled;		/* GPS１回補足済みフラグ	*/
#endif /* _VDV_SUPRA　2018.12.14 S.Sakata */

GNSS_CTRL_LOCAL E_GNSS_CTRL_BAUDRATE		S_gnss_baudrate;		/* GNSSボーレート			*/
GNSS_CTRL_LOCAL UI_8	S_gnss_comm_mode_data[GNSS_COMM_MODE_DATA_BUF_SIZE];	/* GNSS通信モード用データバッファ	*/
GNSS_CTRL_LOCAL UI_32 S_gnss_rcv_msg_timestamp;					/* GNSSメッセージ受信タイムスタンプ		*/

/********************************************************/
/*	内部関数プロトタイプ宣言							*/
/********************************************************/
/* モジュール制御関連 */
GNSS_CTRL_LOCAL void proc_for_idle(void);
GNSS_CTRL_LOCAL void proc_for_initilizing(void);
GNSS_CTRL_LOCAL void proc_for_gnss_setting(void);
GNSS_CTRL_LOCAL void proc_for_baudrate_changing(void);
GNSS_CTRL_LOCAL void proc_for_gnss_setting_2(void);
GNSS_CTRL_LOCAL void proc_for_working(void);
GNSS_CTRL_LOCAL void proc_for_working_baudrate_change(void);

/* GNSS受信メッセージ処理関連 */
GNSS_CTRL_LOCAL void analyze_message_hnr_pvt(UI_16 datalen);
GNSS_CTRL_LOCAL void analyze_message_nav_pvt(UI_16 datalen);
GNSS_CTRL_LOCAL void analyze_message_nav_dop(UI_16 datalen);
GNSS_CTRL_LOCAL void analyze_message_nav_posecef(UI_16 datalen);
GNSS_CTRL_LOCAL void analyze_message_esf_raw(UI_16 datalen);
GNSS_CTRL_LOCAL void analyze_message_esf_meas(UI_16 datalen);

/* 状態遷移操作関連 */
GNSS_CTRL_LOCAL void change_stat(E_GNSS_STAT gnss_stat);
GNSS_CTRL_LOCAL void reset_phase_count(void);
GNSS_CTRL_LOCAL void inc_phase_count(UI_8 inc_value);

/* GNSSコマンド送受信関連 */
GNSS_CTRL_LOCAL void           send_command(const T_gnss_cmd *gnss_cmd);
GNSS_CTRL_LOCAL E_RCV_RESULT   receive_message(UI_8 *class_id, UI_8 *msg_id, UI_16 *data_len);
GNSS_CTRL_LOCAL E_CMD_RESPONSE receive_response(UI_8 *class_id, UI_8 *msg_id);
GNSS_CTRL_LOCAL E_CMD_RESPONSE analyze_ubx_ack(const UI_8 *receive_data, UI_16 data_len);
GNSS_CTRL_LOCAL UI_8           check_ubs_msg_prefix(const UI_8 *ubx_msg, UI_16 *data_len);
GNSS_CTRL_LOCAL UI_8           check_check_sum(const UI_8 *ubx_msg, UI_16 ubx_msg_len);
GNSS_CTRL_LOCAL void 		  calc_checksum(const UI_8 *pData, UI_16 len, UI_8 *checksum);

/* Byte列からの数値取得関連 */
GNSS_CTRL_LOCAL SI_32 get_si_32(const UI_8 *data);
GNSS_CTRL_LOCAL UI_32 get_ui_32(const UI_8 *data);
GNSS_CTRL_LOCAL UI_16 get_ui_16(const UI_8 *data);
GNSS_CTRL_LOCAL UI_8  get_ui_8(const UI_8 *data);


/* GNSS通信モード処理関連 */
GNSS_CTRL_LOCAL void gnss_comm_mode_proc(void);
GNSS_CTRL_LOCAL B_8 gnss_comm_mode_is_need_brate_change(E_GNSS_CTRL_BAUDRATE *brate);

/********************************************************/
/* 関数名称		: Gnss_cntrol_GetHnrPvtInfo				*/
/* 機能の説明	: GNSS HNR-PVT情報カレント値を取得する	*/
/* 引数			: void									*/
/* 戻り値		: T_Gnss_ctrl_hnr_pvt_info :			*/
/*									HNR-PVT情報格納Ptr	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/02								*/
/* 備考			: 										*/
/********************************************************/
T_Gnss_ctrl_hnr_pvt_info *Gnss_cntrol_GetHnrPvtInfo(void)
{
	/* HNR-PVT情報の有効判定 */
	if((S_nav_info_enable & NAV_INFO_ENABLE_HNR) == 0) {
		return(NULL);
	}

	return(&S_hnr_pvt_info);
}

/********************************************************/
/* 関数名称		: Gnss_cntrol_GetNavPvtInfo				*/
/* 機能の説明	: GNSS NAV-PVT情報カレント値を取得する	*/
/* 引数			: void									*/
/* 戻り値		: T_Gnss_ctrl_nav_pvt_info :			*/
/*									NAV-PVT情報格納Ptr	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/28								*/
/* 備考			: 										*/
/*   有効なNAV-PVT情報が存在しない場合、NULLをリターン	*/
/*   する。												*/
/********************************************************/
T_Gnss_ctrl_nav_pvt_info *Gnss_cntrol_GetNavPvtInfo(void)
{
	/* NAV-PVT情報の有効判定 */
	if((S_nav_info_enable & NAV_INFO_ENABLE_PVT) == 0) {
		return(NULL);
	}

	return(&S_nav_pvt_info);
}

/********************************************************/
/* 関数名称		: Gnss_control_GetNavDopInfo			*/
/* 機能の説明	: GNSS NAV-DOP情報カレント値を取得する	*/
/* 引数			: void									*/
/* 戻り値		: T_Gnss_ctrl_nav_dop_info :			*/
/*									NAV-DOP情報格納Ptr	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/02								*/
/* 備考			: 										*/
/*   有効なNAV-DOP情報が存在しない場合、NULLをリターン	*/
/*   する。												*/
/********************************************************/
T_Gnss_ctrl_nav_dop_info *Gnss_control_GetNavDopInfo(void)
{
	/* NAV-DOP情報の有効判定 */
	if((S_nav_info_enable & NAV_INFO_ENABLE_DOP) == 0) {
		return(NULL);
	}

	return(&S_nav_dop_info);
}

/********************************************************/
/* 関数名称		: Gnss_cntrol_GetNavPosecefInfo			*/
/* 機能の説明	: GNSS NAV-POSECEF情報カレント値を取得する	*/
/* 引数			: void									*/
/* 戻り値		: T_Gnss_ctrl_nav_posecef_info :		*/
/*								NAV-POSECEF情報格納Ptr	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/28								*/
/* 備考			: 										*/
/*   有効なNAV-PVT情報が存在しない場合、NULLをリターン	*/
/*   する。												*/
/********************************************************/
T_Gnss_ctrl_nav_posecef_info *Gnss_cntrol_GetNavPosecefInfo(void)
{
	/* NAV-PPOSECEF情報の有効判定 */
	if((S_nav_info_enable & NAV_INFO_ENABLE_POSECEF) == 0) {
		return(NULL);
	}

	return(&S_nav_posecef_info);
}

/********************************************************/
/* 関数名称		： Gnss_control_GetEsfMeasInfo			*/
/* 機能の説明	： GNSS ESF-MEAS情報カレント値を取得する*/
/* 引数			： void									*/
/* 戻り値		： T_Gnss_ctrl_esf_meas_info :			*/
/*									ESF-MEAS情報格納Ptr	*/
/* 作成者		： (OFT)Katsura							*/
/* 作成日		： 16/09/02								*/
/* 備考			： 										*/
/********************************************************/
T_Gnss_ctrl_esf_meas_info *Gnss_control_GetEsfMeasInfo(void)
{
	/* ESF-MEAS情報の有効判定 */
	if((S_nav_info_enable & NAV_INFO_ENABLE_ESF_MEAS) == 0) {
		return(NULL);
	}

	return(&S_nav_esf_meas);
}


/********************************************************/
/* 関数名称		: Gnss_control_GetEsfRawInfo			*/
/* 機能の説明	: GNSS ESF-RAW情報カレント値を取得する	*/
/* 引数			: void									*/
/* 戻り値		: T_Gnss_ctrl_esf_raw_info :			*/
/*									ESF-RAW情報格納Ptr	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/02								*/
/* 備考			: 										*/
/********************************************************/
T_Gnss_ctrl_esf_raw_info *Gnss_control_GetEsfRawInfo(void)
{
	/* ESF-ROWP情報の有効判定 */
	if((S_nav_info_enable & NAV_INFO_ENABLE_ESF_RAW) == 0) {
		return(NULL);
	}

	return(&S_nav_esf_raw);
}

#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata */
/********************************************************/
/* 関数名称		: Gnss_control_iSGnssFixedAtOneTime		*/
/* 機能の説明	: GPSFixType値が過去に１度でも受信済み	*/
/* 				: となったか判定						*/
/* 引数			: void									*/
/* 戻り値		: N_TRUE:受信済み / N_FALSE:未受信		*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 18/12/14								*/
/* 備考			: 										*/
/********************************************************/
B_8 Gnss_control_iSGnssFixedAtOneTime(void)
{
	/*GPS１回補足済みか？*/
	if(S_fixtype_enabled == 0)
	{
		/*未補足*/
		return(N_FALSE);
	}
	/*補足済*/
	return(N_TRUE);
}
#endif /* _VDV_SUPRA　2018.12.14 S.Sakata */

/********************************************************/
/* 関数名称		: Gnss_control_GetGpsFixType			*/
/* 機能の説明	: GPS Fix Type値を取得する				*/
/* 引数			: (O)gps_fix_type : GPS Fix Type値		*/
/* 戻り値		: E_N_RC : 取得結果(N_RC_OK, N_RC_NG)	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/07								*/
/* 備考			: 										*/
/********************************************************/
E_N_RC Gnss_control_GetGpsFixType(UI_8 *gps_fix_type)
{
	if((S_nav_info_enable & NAV_INFO_ENABLE_PVT) == 0) {
		return(N_RC_NG);
	}
	*gps_fix_type = S_nav_pvt_info.gpsFix;
	return(N_RC_OK);
}

/********************************************************/
/* 関数名称		: Gnss_control_getUartData				*/
/* 機能の説明	:	UARTデータ取得						*/
/* 引数			:	(I/ )buf	データバッファ			*/
/*				:	(I/ )len	取得データ長(byte)		*/
/* 戻り値		:	取得できたデータ長(byte)			*/
/* 作成者		:	(OFT)Toguchi						*/
/* 作成日		:	17/01/20							*/
/* 備考			: 										*/
/********************************************************/
UI_16 Gnss_control_getUartData(UI_8 *buf, UI_16 len)
{
	UI_16	get_len;

	if (buf == NULL) {
		return 0;
	}

	if (S_gnss_stat != GNSS_STAT_WORKING) {
		return 0;
	}

	get_len = len;
	if (Gnss_uart_drv_Get_rx_data(GNSS_UART_CH, buf, NULL, &get_len) != GNSS_UART_DRV_OK) {
		return 0;
	}

	return get_len;
}

/********************************************************/
/* 関数名称		: Gnss_control_Init						*/
/* 機能の説明	: GNSS制御初期化						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Gnss_control_Init(void)
{
	T_Uart_buf_info buf_info;

	/* 送受信バッファ情報作成 */
	buf_info.rx_buf      = S_uart_rx_buf;
	buf_info.er_buf      = S_uart_er_buf;
	buf_info.tx_buf      = S_uart_tx_buf;
	buf_info.rx_buf_size = RX_BUF_SIZE;
	buf_info.tx_buf_size = TX_BUF_SIZE;

	/* UARTドライバ初期設定 */
	Gnss_uart_drv_Init(GNSS_UART_CH, &buf_info);

	/* UART通信開始 */
	Gnss_uart_drv_Start(GNSS_UART_CH);

	/* 制御状態初期化 */
	change_stat(GNSS_STAT_IDLE);

	/* NAV-HNR, NAV-DOP情報クリア */
	(void)memset(&S_hnr_pvt_info, 0x00, sizeof(S_hnr_pvt_info));
	(void)memset(&S_nav_dop_info, 0x00, sizeof(S_nav_dop_info));
	(void)memset(&S_nav_pvt_info, 0x00, sizeof(S_nav_pvt_info));
	(void)memset(&S_nav_posecef_info, 0x00, sizeof(S_nav_posecef_info));
	(void)memset(&S_nav_esf_meas, 0x00, sizeof(S_nav_esf_meas));
#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata */
	S_fixtype_enabled = 0;
#endif /* _VDV_SUPRA　2018.12.14 S.Sakata */

	/* NAV情報有効フラグクリア */
	S_nav_info_enable = NAV_INFO_ENABLE_NONE;

	/* GNSS通信のボーレート初期値設定（115200bps） */
	S_gnss_baudrate = GNSS_CTRL_BAUDRATE_115200;

	S_gnss_rcv_msg_timestamp = 0;
}


/********************************************************/
/* 関数名称		: Gnss_control_Main						*/
/* 機能の説明	: GNSS制御メイン						*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Gnss_control_Main(void)
{
	switch(S_gnss_stat) {
	case GNSS_STAT_IDLE:
		/* アイドル状態での処理 */
		proc_for_idle();
		break;
	case GNSS_STAT_INITIALIZING:
		/* 初期化中の処理 */
		proc_for_initilizing();
		break;
	case GNSS_STAT_GNSS_SETTING:
		/* GNSS設定中の処理(ボーレート変更前) */
		proc_for_gnss_setting();
		break;
	case GNSS_STAT_BAUDRATE_CHANGING:
		proc_for_baudrate_changing();
		break;
	case GNSS_STAT_GNSS_SETTING_2:
		/* GNSS設定中の処理(ボーレート変更後) */
		proc_for_gnss_setting_2();
		break;
	case GNSS_STAT_WORKING:
		/* 通常動作中の処理 */
		proc_for_working();
		break;
	case GNSS_STAT_WORKING_BAUDRATE_CHANGE:
		/* 通常動作中のボーレート変更処理 */
		proc_for_working_baudrate_change();
		break;
	default:
		break;
	}
}

/********************************************************/
/* 関数名称		: proc_for_idle							*/
/* 機能の説明	: アイドル状態時定期処理				*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void proc_for_idle(void)
{
	E_SYS_CTRL_STATUS sys_ctrl_status;
	
	sys_ctrl_status = System_control_GetStatus();

	switch(sys_ctrl_status) {
	case SYS_OPERATION_WAIT_ACC_ON:
	case SYS_OPERATION_PRE_ACC_OFF:
	case SYS_OPERATION_ACC_OFF:
		/* ACC ON待ち,ACC OFF以降、ACC OFF状態なら何もしない */
		break;
	default:
		/* それ以外なら初期化処理へ遷移 */
		change_stat(GNSS_STAT_INITIALIZING);
		break;
	}
}

/********************************************************/
/* 関数名称		: proc_for_initilizing					*/
/* 機能の説明	: 初期化中状態時定期処理				*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void proc_for_initilizing(void)
{
	E_SYS_CTRL_STATUS sys_ctrl_status;

	/* 現在時刻取得 */
	UI_32 cur_time = Timer_ctrl_GetTime1ms();
	
	/* システム制御状態取得取得 */
	sys_ctrl_status = System_control_GetStatus();
	switch(sys_ctrl_status) {
	case SYS_OPERATION_PRE_ACC_OFF:
	case SYS_OPERATION_ACC_OFF:
		/* ACC OFF移行,ACC OFFならアイドルへ遷移 */
		change_stat(GNSS_STAT_IDLE);
		/*状態変化時は処理を抜ける*/
		return;
	default:
		break;
	}

	if(S_proc_phase_count == 0) {
		/* GNSS RESET オン */
		
		/* 次の処理フェーズへ */
		inc_phase_count(1);
	}
	else if(S_proc_phase_count == 1) {
		/* GNSS RESET ON区間経過判定 */
		if((cur_time - S_proc_phase_timestamp) > GNSS_REST_ON_DULATION) {
			/* GNSS RESET オフ */

			/* 次の処理フェーズへ */
			inc_phase_count(1);
		}
	}
	else if(S_proc_phase_count == 2) {
		/* GNSSモジュール安定待ち区間経過判定 */
		if((cur_time - S_proc_phase_timestamp) > WAIT_GNSS_MODULE_DULATION) {
			/* GNSS設定中状態へ遷移 */
			change_stat(GNSS_STAT_GNSS_SETTING);
		}
	}
	else {
		/* nop */
	}
}

/********************************************************/
/* 関数名称		: proc_for_gnss_setting					*/
/* 機能の説明	: GNSS設定中状態時定期処理				*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void proc_for_gnss_setting(void)
{
	UI_8 response;
	UI_8 class_id;
	UI_8 msg_id;
	E_SYS_CTRL_STATUS sys_ctrl_status;

	/* 現在時刻取得 */
	UI_32 cur_time = Timer_ctrl_GetTime1ms();
	
	sys_ctrl_status = System_control_GetStatus();
	switch(sys_ctrl_status) {
	case SYS_OPERATION_PRE_ACC_OFF:
	case SYS_OPERATION_ACC_OFF:
		/* ACC OFF移行,ACC OFFならアイドルへ遷移 */
		change_stat(GNSS_STAT_IDLE);
		/*状態変化時は処理を抜ける*/
		return;
	default:
		break;
	}

	switch(S_cmd_send_status) {
	case CMD_SEND_STAT_IDLE:		/* コマンド未送信状態 */
		if(C_init_cmd_tbl[S_proc_phase_count].cmd_len == 0) {
			/* ボーレート変更状態へ遷移 */
			change_stat(GNSS_STAT_BAUDRATE_CHANGING);
			return;
		}

		/* 初期設定コマンドを送信 */
		send_command(&C_init_cmd_tbl[S_proc_phase_count]);

		/* リトライカウント、タイムアウトカウントクリア */
		S_cmd_send_retry_count   = 0;
		S_cmd_send_timeout_count = 0;
		
		/* コマンド送信状態 応答待ちに遷移 */
		S_cmd_send_status = CMD_SEND_STAT_RES_WAIT;
		break;
	case CMD_SEND_STAT_RES_WAIT:	/* コマンド応答待ち状態	*/
		/* 応答受信 */
		response = receive_response(&class_id, &msg_id);
		
		/* ACK受信した場合 */
		if(response == CMD_RESPONSE_ACK) {
			/* クラスIDとメッセージIDが送信したコマンドと一致しているかチェック */
			if((class_id != C_init_cmd_tbl[S_proc_phase_count].cmd[2])
			 &&(msg_id   != C_init_cmd_tbl[S_proc_phase_count].cmd[3])) {
			 	/* 一致していなければ、NACKとみなして再送 */
			 	response = CMD_RESPONSE_NACK;
			}
		}

		switch(response) {
		case CMD_RESPONSE_ACK:		/* ACK受信		*/
			/* 次の処理フェーズへ */
			inc_phase_count(1);
			
			/* コマンド送信状態 アイドルへ遷移 */
			S_cmd_send_status = CMD_SEND_STAT_IDLE;
		
			/* 初期設定コマンドすべて送信したら */
			if(C_init_cmd_tbl[S_proc_phase_count].cmd_len == 0) {
				/* ボーレート変更状態へ遷移 */
				change_stat(GNSS_STAT_BAUDRATE_CHANGING);
			}
			break;
		case CMD_RESPONSE_NACK:		/* NACK受信		*/
			if(S_cmd_send_retry_count >= CMD_SEND_RETRY_MAX) {
				/* エラー状態へ遷移 */
				change_stat(GNSS_STAT_ERROR);
			}
			else {
				/* 初期設定コマンド再送 */
				send_command(&C_init_cmd_tbl[S_proc_phase_count]);
				S_cmd_send_retry_count++;

				/* フェーズ遷移時刻更新 */
				inc_phase_count(0);
			}
			break;
		case CMD_RESPONSE_NONE:		/* 応答なし		*/
		default:
			/* 応答タイムアウトチェック */
			if((cur_time - S_proc_phase_timestamp) > CMD_RESPONSE_TIMEOUT) {
				if(S_cmd_send_timeout_count >= CMD_SEND_TIMEOUT_MAX) {
					/* エラー状態へ遷移 */
					change_stat(GNSS_STAT_ERROR);
				}
				else {
					/* 初期設定コマンド再送 */
					send_command(&C_init_cmd_tbl[S_proc_phase_count]);
					S_cmd_send_timeout_count++;

					/* フェーズ遷移時刻更新 */
					inc_phase_count(0);
				}
			}
			break;
		}
		break;
	default:
		break;
	}
}

/********************************************************/
/* 関数名称		: proc_for_baudrate_changing			*/
/* 機能の説明	: ボーレート変更中状態時定期処理		*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/05								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void proc_for_baudrate_changing(void)
{
	UI_8 response;
	UI_8 class_id;
	UI_8 msg_id;

	E_SYS_CTRL_STATUS sys_ctrl_status;

	/* 現在時刻取得 */
	UI_32 cur_time = Timer_ctrl_GetTime1ms();
	
	/* システム制御状態取得取得 */
	sys_ctrl_status = System_control_GetStatus();
	switch(sys_ctrl_status) {
	case SYS_OPERATION_PRE_ACC_OFF:
	case SYS_OPERATION_ACC_OFF:
		/* ACC OFF移行,ACC OFFならアイドルへ遷移 */
		change_stat(GNSS_STAT_IDLE);
		/*状態変化時は処理を抜ける*/
		return;
	default:
		break;
	}

	if(S_proc_phase_count == 0) {
		/* ボーレート設定コマンド送信 */
		send_command(&C_change_boudrate_cmd);

		/* 次の処理フェーズへ */
		inc_phase_count(1);
	}
	else if(S_proc_phase_count == 1) {
		/* ボーレート設定コマンド送信完了相当時間経過してから、ボーレート変更 */
		if((cur_time - S_proc_phase_timestamp) >= UBX_BAUD_TX_WAIT_TIME) {
			Gnss_uart_drv_Set_bdrate(GNSS_UART_CH, GNSS_BAUDRATE_115200_BPS);

			Gnss_uart_drv_Clear_rx_data(GNSS_UART_CH);
			Gnss_uart_drv_Start_rx(GNSS_UART_CH);

			/* 次の処理フェーズへ */
			inc_phase_count(1);
		}
	}
	else if(S_proc_phase_count == 2) {
		/* 応答受信 */
		response = receive_response(&class_id, &msg_id);

		/* ACK受信した場合 */
		if(response == CMD_RESPONSE_ACK) {
			/* クラスIDとメッセージIDが送信したコマンドと一致しているかチェック */
			if((class_id != C_change_boudrate_cmd.cmd[2])
			 &&(msg_id   != C_change_boudrate_cmd.cmd[3])) {
			 	/* 一致していなければ、NACKとみなして再送 */
			 	response = CMD_RESPONSE_NACK;
			}
		}

		switch(response) {
		case CMD_RESPONSE_ACK:		/* ACK受信		*/
			/* 通常動作中状態へ遷移 */
			change_stat(GNSS_STAT_GNSS_SETTING_2);
			break;
		case CMD_RESPONSE_NACK:		/* NACK受信		*/
			/* エラー状態へ遷移 */
			change_stat(GNSS_STAT_ERROR);
			break;
		case CMD_RESPONSE_NONE:		/* 応答なし		*/
		default:

			/* 応答タイムアウトチェック */
			if((cur_time - S_proc_phase_timestamp) > CMD_RESPONSE_TIMEOUT) {
				if(S_cmd_send_timeout_count >= CMD_SEND_TIMEOUT_MAX) {
					/* エラー状態へ遷移 */
					change_stat(GNSS_STAT_ERROR);
				}
				else {
					/* ボーレート設定コマンド送信 */
					send_command(&C_change_boudrate_cmd);
					S_cmd_send_timeout_count++;

					/* フェーズ遷移時刻更新 */
					inc_phase_count(0);
				}
			}
			break;
		}
	}
	else {
		/* nop */
	}
}

/********************************************************/
/* 関数名称		: proc_for_gnss_setting_2				*/
/* 機能の説明	: ボーレート変更後GNSS設定中状態時定期処理	*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void proc_for_gnss_setting_2(void)
{
	UI_8 response;
	UI_8 class_id;
	UI_8 msg_id;
	E_SYS_CTRL_STATUS sys_ctrl_status;

	/* 現在時刻取得 */
	UI_32 cur_time = Timer_ctrl_GetTime1ms();
	
	sys_ctrl_status = System_control_GetStatus();
	switch(sys_ctrl_status) {
	case SYS_OPERATION_PRE_ACC_OFF:
	case SYS_OPERATION_ACC_OFF:
		/* ACC OFF移行,ACC OFFならアイドルへ遷移 */
		change_stat(GNSS_STAT_IDLE);
		/*状態変化時は処理を抜ける*/
		return;
	default:
		break;
	}

	switch(S_cmd_send_status) {
	case CMD_SEND_STAT_IDLE:		/* コマンド未送信状態 */
		if(C_init_cmd_tbl_2[S_proc_phase_count].cmd_len == 0) {
			/* 通常動作中状態へ遷移 */
			change_stat(GNSS_STAT_WORKING);
			return;
		}
		
		if(S_proc_phase_count == 0) {
			if((cur_time - S_proc_phase_timestamp) <= 1000) {
				return;
			}
		}
		
		/* 初期設定コマンドを送信 */
		send_command(&C_init_cmd_tbl_2[S_proc_phase_count]);

		/* リトライカウント、タイムアウトカウントクリア */
		S_cmd_send_retry_count   = 0;
		S_cmd_send_timeout_count = 0;
		
		/* コマンド送信状態 応答待ちに遷移 */
		S_cmd_send_status = CMD_SEND_STAT_RES_WAIT;
		break;
	case CMD_SEND_STAT_RES_WAIT:	/* コマンド応答待ち状態	*/
		/* 応答受信 */
		response = receive_response(&class_id, &msg_id);
		
		/* ACK受信した場合 */
		if(response == CMD_RESPONSE_ACK) {
			/* クラスIDとメッセージIDが送信したコマンドと一致しているかチェック */
			if((class_id != C_init_cmd_tbl_2[S_proc_phase_count].cmd[2])
			 &&(msg_id   != C_init_cmd_tbl_2[S_proc_phase_count].cmd[3])) {
			 	/* 一致していなければ、NACKとみなして再送 */
			 	response = CMD_RESPONSE_NACK;
			}
		}

		switch(response) {
		case CMD_RESPONSE_ACK:		/* ACK受信		*/
			/* 次の処理フェーズへ */
			inc_phase_count(1);
			
			/* コマンド送信状態 アイドルへ遷移 */
			S_cmd_send_status = CMD_SEND_STAT_IDLE;
		
			/* 初期設定コマンドすべて送信したら */
			if(C_init_cmd_tbl_2[S_proc_phase_count].cmd_len == 0) {
				/* 通常動作中状態へ遷移 */
				change_stat(GNSS_STAT_WORKING);
			}
			break;
		case CMD_RESPONSE_NACK:		/* NACK受信		*/
			if(S_cmd_send_retry_count >= CMD_SEND_RETRY_MAX) {
				/* エラー状態へ遷移 */
				change_stat(GNSS_STAT_ERROR);
			}
			else {
				/* 初期設定コマンド再送 */
				send_command(&C_init_cmd_tbl_2[S_proc_phase_count]);
				S_cmd_send_retry_count++;

				/* フェーズ遷移時刻更新 */
				inc_phase_count(0);
			}
			break;
		case CMD_RESPONSE_NONE:		/* 応答なし		*/
		default:
			/* 応答タイムアウトチェック */
			if((cur_time - S_proc_phase_timestamp) > CMD_RESPONSE_TIMEOUT) {
				if(S_cmd_send_timeout_count >= CMD_SEND_TIMEOUT_MAX) {
					/* エラー状態へ遷移 */
					change_stat(GNSS_STAT_ERROR);
				}
				else {
				/* 初期設定コマンド再送 */
					send_command(&C_init_cmd_tbl_2[S_proc_phase_count]);
					S_cmd_send_timeout_count++;

					/* フェーズ遷移時刻更新 */
					inc_phase_count(0);
				}
			}
			break;
		}
		break;
	default:
		break;
	}
}
/********************************************************/
/* 関数名称		: proc_for_working						*/
/* 機能の説明	: 通常動作中状態時定期処理				*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void proc_for_working(void)
{
	UI_8  class_id;
	UI_8  msg_id;
	UI_16 data_len;
	E_SYS_CTRL_STATUS sys_ctrl_status;
	E_GNSS_CTRL_BAUDRATE brate;

	/* 現在時刻取得 */
	UI_32 cur_time = Timer_ctrl_GetTime1ms();

	sys_ctrl_status = System_control_GetStatus();
	switch(sys_ctrl_status) {
	case SYS_OPERATION_PRE_ACC_OFF:
	case SYS_OPERATION_ACC_OFF:
		/* ACC OFF移行,ACC OFFならアイドルへ遷移 */
		change_stat(GNSS_STAT_IDLE);
		/*状態変化時は処理を抜ける*/
		return;

	case SYS_OPERATION_ACC_ON_UNKNOWN_STAT:
		/* 受信データをクリア */
		Gnss_uart_drv_Clear_rx_data(GNSS_UART_CH);
		break;

	case SYS_OPERATION_GNSS_COMM:
		if (gnss_comm_mode_is_need_brate_change(&brate) == N_TRUE) {
			/* ボーレート変更 */
			S_gnss_baudrate = brate;

			/* 通常動作中のボーレート変更状態に遷移 */
			change_stat(GNSS_STAT_WORKING_BAUDRATE_CHANGE);
		}
		else {
			gnss_comm_mode_proc();
		}
		break;

	default:

		/* メッセージ受信 */
		while(receive_message(&class_id, &msg_id, &data_len) == RCV_RESULT_OK) {
			S_gnss_rcv_msg_timestamp = Timer_ctrl_GetTime1ms();

			if((class_id == HNR_PVT_CLASS_ID) && (msg_id == HNR_PVT_MESSAGE_ID)) {
				/* HNR-PVTメッセージ解析 */
				analyze_message_hnr_pvt(data_len);
			}
			else if((class_id == NAV_PVT_CLASS_ID) && (msg_id == NAV_PVT_MESSAGE_ID)) {
				/* NAV-PVTメッセージ解析 */
				analyze_message_nav_pvt(data_len);
			}
			else if((class_id == NAV_DOP_CLASS_ID) && (msg_id == NAV_DOP_MESSAGE_ID)) {
				/* NAV-DOPメッセージ解析 */
				analyze_message_nav_dop(data_len);
			}
			else if((class_id == NAV_POSECEF_CLASS_ID) && (msg_id == NAV_POSECEF_MESSAGE_ID)) {
				/* NAV-POSECEFメッセージ解析 */
				analyze_message_nav_posecef(data_len);
			}
			else if((class_id == ESF_RAW_CLASS_ID) && (msg_id == ESF_RAW_MESSAGE_ID)) {
				/* ESF-RAWメッセージ解析 */
				analyze_message_esf_raw(data_len);
			}
			else if((class_id == ESF_MEAS_CLASS_ID) && (msg_id == ESF_MEAS_MESSAGE_ID)) {
				/* ESF-MEASメッセージ解析 */
				analyze_message_esf_meas(data_len);
			}
			else {
				/* nop */
			}
		}

		/* GNSSメッセージを受信できない期間をチェック */
		if (Timer_ctrl_TimeCompare1ms(S_gnss_rcv_msg_timestamp, GNSS_MSG_RCV_TIMEOUT) == N_OK) {
			/* メッセージ受信タイムアウト	*/
			/* エラー状態へ遷移 */
			change_stat(GNSS_STAT_ERROR);
		}
		else {
		}

		break;
	}

}

/********************************************************/
/* 関数名称		: proc_for_working_baudrate_chang		*/
/* 機能の説明	: 通常動作中のボーレート変更状態時定期処理	*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/03/01								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void proc_for_working_baudrate_change(void)
{
	UI_8	response;
	UI_8	class_id;
	UI_8	msg_id;
	UI_32	cur_time;
	T_gnss_cmd		gnss_cmd;
	E_GNSS_BAUDRATE	drv_brate;

	E_SYS_CTRL_STATUS sys_ctrl_status;

	/* システム制御状態取得取得 */
	sys_ctrl_status = System_control_GetStatus();
	switch(sys_ctrl_status) {
	case SYS_OPERATION_PRE_ACC_OFF:
	case SYS_OPERATION_ACC_OFF:
		/* ACC OFF移行,ACC OFFならアイドルへ遷移 */
		change_stat(GNSS_STAT_IDLE);
		/*状態変化時は処理を抜ける*/
		return;
	default:
		break;
	}

	/* 現在時刻取得 */
	cur_time = Timer_ctrl_GetTime1ms();

	if(S_proc_phase_count == 0) {
		/* ボーレート設定コマンド送信 */
		if (S_gnss_baudrate == GNSS_CTRL_BAUDRATE_9600) {
			gnss_cmd.cmd = C_ubx_cfg_prt_uart_9600;
			gnss_cmd.cmd_len = sizeof(C_ubx_cfg_prt_uart_9600);
		}
		else {
			gnss_cmd.cmd = C_ubx_cfg_prt_uart_115200;
			gnss_cmd.cmd_len = sizeof(C_ubx_cfg_prt_uart_115200);
		}
		send_command(&gnss_cmd);

		/* タイムアウトカウントクリア */
		S_cmd_send_timeout_count = 0;

		/* 次の処理フェーズへ */
		inc_phase_count(1);
	}
	else if(S_proc_phase_count == 1) {
		/* ボーレート設定コマンド送信完了相当時間経過してから、ボーレート変更 */
		if((cur_time - S_proc_phase_timestamp) >= UBX_BAUD_TX_WAIT_TIME) {
			if (S_gnss_baudrate == GNSS_CTRL_BAUDRATE_9600) {
				drv_brate = GNSS_BAUDRATE_9600_BPS;
			}
			else {
				drv_brate = GNSS_BAUDRATE_115200_BPS;
			}
			Gnss_uart_drv_Set_bdrate(GNSS_UART_CH, drv_brate);

			/* メッセージ受信バッファクリア */
			S_receive_data_size = 0;
			Gnss_uart_drv_Clear_rx_data(GNSS_UART_CH);
			Gnss_uart_drv_Start_rx(GNSS_UART_CH);

			/* 次の処理フェーズへ */
			inc_phase_count(1);
		}
	}
	else if(S_proc_phase_count == 2) {
		/* 応答タイムアウトチェック */
		if((cur_time - S_proc_phase_timestamp) > CMD_RESPONSE_TIMEOUT) {
			if(S_cmd_send_timeout_count >= 50) {
				OUTLOG(OutputLog("[GNSS]WBRChange timeout err!!\n");)
				/* エラー  ボーレート変更できない場合も通常動作中状態へ遷移 */
				change_stat(GNSS_STAT_WORKING);
			}
			else {
				/* ボーレート設定コマンド送信 */
				if (S_gnss_baudrate == GNSS_BAUDRATE_9600_BPS) {
					gnss_cmd.cmd = C_ubx_cfg_prt_uart_9600;
					gnss_cmd.cmd_len = sizeof(C_ubx_cfg_prt_uart_9600);
				}
				else {
					gnss_cmd.cmd = C_ubx_cfg_prt_uart_115200;
					gnss_cmd.cmd_len = sizeof(C_ubx_cfg_prt_uart_115200);
				}
				send_command(&gnss_cmd);
				S_cmd_send_timeout_count++;
				/* OUTLOG(OutputLog("[GNSS]WBRChange timeout retry cnt:%d\n", S_cmd_send_timeout_count);) */
				/* フェーズ遷移時刻更新 */
				inc_phase_count(0);
			}
		}
		else {
			/* 応答受信 */
			response = receive_response(&class_id, &msg_id);
			if(response == CMD_RESPONSE_ACK) {
				/* ACK受信した場合 */
				/* クラスIDとメッセージIDが送信したコマンドと一致しているかチェック */
				if((class_id == C_ubx_cfg_prt_uart_115200[2])
				 &&(msg_id   == C_ubx_cfg_prt_uart_115200[3])) {
					OUTLOG(OutputLog("[GNSS]WBRChange ACK\n");)
					/* 一致していれば、通常動作中状態へ遷移 */
					change_stat(GNSS_STAT_WORKING);
				}
			}
		}
	}
	else {
		/* nop */
	}
}


/********************************************************/
/* 関数名称		: analyze_message_hnr_pvt				*/
/* 機能の説明	: HVR-PVTメッセージ解析処理				*/
/* 引数			: (I)datalen:受信データ長				*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/05								*/
/* 備考			: 										*/
/*   受信したHVR-PVTメッセージの内容を解析して、外部に	*/
/*   提供するHVR-PVT情報(S_hnr_pvt_info)の内容を更新する。*/
/********************************************************/
GNSS_CTRL_LOCAL void analyze_message_hnr_pvt(UI_16 datalen)
{
	T_Gnss_ctrl_hnr_pvt_info info;
	UI_8 data_pos;
	
	/* データ長がHNR-PVTの規定と異なれば解析エラー */
	if(datalen != HNR_PVT_MESSAGE_LEN) {
		return;
	}


	/* HNR-PVT情報クリア */
	(void)memset(&info, 0x00, sizeof(T_Gnss_ctrl_hnr_pvt_info));

	/* データ参照位置初期化 */
	data_pos = 6;

	info.tow = get_ui_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;
	
	info.year = get_ui_16(&S_receive_data_buf[data_pos]);
	data_pos += 2;

	info.month = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	info.day = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	info.hour = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	info.min = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	info.sec = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	info.valid = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	info.nano = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.gpsFix = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	info.flags = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	/* reserved1 使用せず */
	data_pos += 2;

	info.lon = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.lat = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.height = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.h_msl = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.g_speed = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.speed = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.head_mot = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.head_veh = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	/* 内部バッファへコピー */
	(void)memcpy(&S_hnr_pvt_info, &info, sizeof(T_Gnss_ctrl_hnr_pvt_info));

#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata GNSS_ADR_ONとGNSS_HNR_PVT_ENABLEが有効でないとこの処理は通らない*/
	if(	S_fixtype_enabled == 0)
	{
		/* GPS１度も補足していない	*/
		if( (info.gpsFix == GNSS_GSPFIX_2DFIX) || 
			(info.gpsFix == GNSS_GSPFIX_3DFIX) || 
			(info.gpsFix == GNSS_GSPFIX_GPS_AND_DEADRECKONING) ||
			(info.gpsFix == GNSS_GSPFIX_DEADRECKONIN_ONLY)	)
		{
			/* GPS１度も補足済み*/
			S_fixtype_enabled = 1;
		}
	}
#endif /* _VDV_SUPRA　2018.12.14 S.Sakata GNSS_ADR_ONとGNSS_HNR_PVT_ENABLEが有効でないとこの処理は通らない*/
	/* HNR-PVT情報有効フラグオン */
	S_nav_info_enable |= NAV_INFO_ENABLE_HNR;
}

/********************************************************/
/* 関数名称		: analyze_message_nav_pvt				*/
/* 機能の説明	: NAV-PVTメッセージ解析処理				*/
/* 引数			: (I)datalen:受信データ長				*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/26								*/
/* 備考			: 										*/
/*   受信したNAV-PVTメッセージの内容を解析して、外部に	*/
/*   提供するNAV-PVT情報(S_nav_pvt_info)の内容を更新する。*/
/********************************************************/
GNSS_CTRL_LOCAL void analyze_message_nav_pvt(UI_16 datalen)
{
	T_Gnss_ctrl_nav_pvt_info info;
	UI_8 data_pos;

	/* データ長がNAV-HNRの規定と異なれば解析エラー */
	if(datalen != NAV_PVT_MESSAGE_LEN) {
		return;
	}


	/* NAV-HNR情報クリア */
	(void)memset(&info, 0x00, sizeof(T_Gnss_ctrl_nav_pvt_info));

	/* データ参照位置初期化 */
	data_pos = 6;

	info.tow = get_ui_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;
	
	info.year = get_ui_16(&S_receive_data_buf[data_pos]);
	data_pos += 2;

	info.month = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	info.day = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	info.hour = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	info.min = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	info.sec = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	info.valid = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	info.t_acc = get_ui_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;
	
	info.nano = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.gpsFix = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	info.flags = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;

	/* reserved1 使用せず */
	data_pos++;
	
	info.num_sv = get_ui_8(&S_receive_data_buf[data_pos]);
	data_pos++;
	
	info.lon = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.lat = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.height = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.h_msl = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.h_acc = get_ui_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.v_acc = get_ui_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.vel_n = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.vel_e = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.vel_d = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.g_speed = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.head_mot = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.s_acc = get_ui_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.head_acc = get_ui_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.p_dop = get_ui_16(&S_receive_data_buf[data_pos]);
	data_pos += 2;

	/* reserved2 使用せず */
	data_pos += 6;

	info.head_veh = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	/* 内部バッファへコピー */
	(void)memcpy(&S_nav_pvt_info, &info, sizeof(T_Gnss_ctrl_nav_pvt_info));

#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata */
	if(	S_fixtype_enabled == 0)
	{
		/* GPS１度も補足していない	*/
		if( (info.gpsFix == GNSS_GSPFIX_2DFIX) || 
			(info.gpsFix == GNSS_GSPFIX_3DFIX) || 
			(info.gpsFix == GNSS_GSPFIX_GPS_AND_DEADRECKONING) ||
			(info.gpsFix == GNSS_GSPFIX_DEADRECKONIN_ONLY)	)
		{
			/* GPS１度も補足済み*/
			S_fixtype_enabled = 1;
		}
	}
#endif /* _VDV_SUPRA　2018.12.14 S.Sakata */
	/* NAV-PVT情報有効フラグオン */
	S_nav_info_enable |= NAV_INFO_ENABLE_PVT;
}

/********************************************************/
/* 関数名称		: analyze_message_nav_posecef			*/
/* 機能の説明	: NAV-POSECEFメッセージ解析処理			*/
/* 引数			: (I)datalen:受信データ長				*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/28								*/
/* 備考			: 										*/
/*   受信したNAV-POSECEFメッセージの内容を解析して、	*/
/*   外部に提供するNAV-POSECEF情報(S_nav_posecef_info)  */
/*   の内容を更新する。									*/
/********************************************************/
GNSS_CTRL_LOCAL void analyze_message_nav_posecef(UI_16 datalen)
{
	T_Gnss_ctrl_nav_posecef_info info;
	UI_8 data_pos;

	/* データ長がNAV-HNRの規定と異なれば解析エラー */
	if(datalen != NAV_POSECEF_MESSAGE_LEN) {
		return;
	}


	/* NAV-POSECEF情報クリア */
	(void)memset(&info, 0x00, sizeof(T_Gnss_ctrl_nav_posecef_info));

	/* データ参照位置初期化 */
	data_pos = 6;

	info.i_tow = get_ui_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.ecef_x = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.ecef_y = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.ecef_z = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.p_acc = get_ui_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	/* 内部バッファへコピー */
	(void)memcpy(&S_nav_posecef_info, &info, sizeof(T_Gnss_ctrl_nav_posecef_info));

	/* NAV-POSECEF情報有効フラグオン */
	S_nav_info_enable |= NAV_INFO_ENABLE_POSECEF;
}

/********************************************************/
/* 関数名称		: analyze_message_nav_dop				*/
/* 機能の説明	: NAV-DOPメッセージ解析処理				*/
/* 引数			: (I)datalen:受信データ長				*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/05								*/
/* 備考			: 										*/
/*   受信したNAV-DOPメッセージの内容を解析して、外部に	*/
/*   提供するNAV-DOP情報(S_nav_dop_info)の内容を更新する。*/
/********************************************************/
GNSS_CTRL_LOCAL void analyze_message_nav_dop(UI_16 datalen)
{
	T_Gnss_ctrl_nav_dop_info info;
	UI_8 data_pos;

	/* データ長がNAV-DOPの規定と異なれば解析エラー */
	if(datalen != NAV_DOP_MESSAGE_LEN) {
		return;
	}


	/* NAV-HNR情報クリア */
	(void)memset(&info, 0x00, sizeof(T_Gnss_ctrl_nav_dop_info));

	/* データ参照位置初期化 */
	data_pos = 6;

	info.i_tow = get_ui_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	info.g_dop = get_ui_16(&S_receive_data_buf[data_pos]);
	data_pos += 2;

	info.p_dop = get_ui_16(&S_receive_data_buf[data_pos]);
	data_pos += 2;

	info.t_dop = get_ui_16(&S_receive_data_buf[data_pos]);
	data_pos += 2;

	info.v_dop = get_ui_16(&S_receive_data_buf[data_pos]);
	data_pos += 2;

	info.h_dop = get_ui_16(&S_receive_data_buf[data_pos]);
	data_pos += 2;

	info.n_dop = get_ui_16(&S_receive_data_buf[data_pos]);
	data_pos += 2;

	info.e_dop = get_ui_16(&S_receive_data_buf[data_pos]);
	data_pos += 2;

	/* 内部バッファへコピー */
	(void)memcpy(&S_nav_dop_info, &info, sizeof(T_Gnss_ctrl_nav_dop_info));

	/* NAV-DOP情報有効フラグオン */
	S_nav_info_enable |= NAV_INFO_ENABLE_DOP;
}


/********************************************************/
/* 関数名称		： analyze_message_esf_meas				*/
/* 機能の説明	： ESF-MEASメッセージ解析処理			*/
/* 引数			： (I)datalen:受信データ長				*/
/* 戻り値		： void									*/
/* 作成者		： (OFT)Katsura							*/
/* 作成日		： 16/09/08								*/
/* 備考			： 										*/
/*   受信したESF-MEASメッセージの内容を解析して、外部に	*/
/*   提供するESF-MEAS情報(S_nav_esf_meas)の内容を更新する。*/
/********************************************************/
void analyze_message_esf_meas(UI_16 datalen)
{
	T_Gnss_ctrl_esf_meas_info info;
	UI_16 i;
	UI_16 item_count;
	UI_16 data_pos;
	UI_32 value_32;
	UI_8  data_type;
	UI_32 data_value;

	/* データ長は(8 + 4*N)でないといけない */
	if(((datalen  - 8) % 4) != 0) {
		return;
	}

	
	/* データ項目数取得 */
	item_count = (datalen  - 8) / 4;

	/* ESF-MEAS情報カレント値をコピー */
	(void)memcpy(&info, &S_nav_esf_meas, sizeof(T_Gnss_ctrl_esf_meas_info));

	/* データ参照位置初期化 */
	data_pos = 6;

	/* 先頭4ByteはReserve */
	data_pos += 8;

	/* 全項目走査 */
	for(i = 0; i < item_count; i++) {
		/* データ取得 */
		value_32 = get_ui_32(&S_receive_data_buf[data_pos]);
		
		/* データタイプとデータ値取得 */
		data_type  = (UI_8)(value_32 >> 24);
		data_value =  (value_32 & 0x00ffffff);
		/*マイナス値*/
		if((data_value & 0x00800000) != 0)
		{
			/*上位をFFで埋める*/
			data_value |= 0xFF000000;
		}

		switch(data_type) {
		case ESF_MEAS_DATA_TYPE_X_AXIS_GYRO:
			info.x_axis_gyro = data_value;
			info.en_x_axis_gyro = 1;
			break;
		case ESF_MEAS_DATA_TYPE_Y_AXIS_GYRO:
			info.y_axis_gyro = data_value;
			info.en_y_axis_gyro = 1;
			break;
		case ESF_MEAS_DATA_TYPE_Z_AXIS_GYRO:
			info.z_axis_gyro = data_value;
			info.en_z_axis_gyro = 1;
			break;
		case ESF_MEAS_DATA_TYPE_X_AXIS_ACCEL:
			info.x_axis_accel = data_value;
			info.en_x_axis_accel = 1;
			break;
		case ESF_MEAS_DATA_TYPE_Y_AXIS_ACCEL:
			info.y_axis_accel = data_value;
			info.en_y_axis_accel = 1;
			break;
		case ESF_MEAS_DATA_TYPE_Z_AXIS_ACCEL:
			info.z_axis_accel = data_value;
			info.en_z_axis_accel = 1;
			break;
		case ESF_MEAS_DATA_TYPE_GYRO_TEMP:
			info.gyro_temp = data_value;
			info.en_gyro_temp = 1;
			break;
		default:
			break;
		}
		
		data_pos += 4;
	}

	/* ESF-MEAS情報カレント値更新 */
	(void)memcpy(&S_nav_esf_meas, &info, sizeof(T_Gnss_ctrl_esf_meas_info));

	/* ESF-MEAS情報有効フラグオン */
	S_nav_info_enable |= NAV_INFO_ENABLE_ESF_MEAS;
}

/********************************************************/
/* 関数名称		: analyze_message_esf_raw				*/
/* 機能の説明	: ESF-RAWメッセージ解析処理				*/
/* 引数			: (I)datalen:受信データ長				*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/08								*/
/* 備考			: 										*/
/*   受信したESF-RAWメッセージの内容を解析して、外部に	*/
/*   提供するESF-RAW情報(S_nav_esf_raw)の内容を更新する。*/
/********************************************************/
GNSS_CTRL_LOCAL void analyze_message_esf_raw(UI_16 datalen)
{
	T_Gnss_ctrl_esf_raw_info info;
	UI_16 i;
	UI_16 item_count;
	UI_16 data_pos;
	UI_32 value_32;
	UI_8  data_type;
	UI_32 data_value;

	/* データ長は(4 + 8*N)でないといけない */
	if(((datalen  - 4) % 8) != 0) {
		return;
	}
	
	/* データ項目数取得 */
	item_count = (datalen  - 4) / 8;

	/* ESF-RAW情報カレント値をコピー */
	(void)memcpy(&info, &S_nav_esf_raw, sizeof(T_Gnss_ctrl_esf_raw_info));

	/* データ参照位置初期化 */
	data_pos = 6;

	/* 先頭4ByteはReserve */
	data_pos += 4;

	/* 全項目走査 */
	for(i = 0; i < item_count; i++) {
		/* データ取得 */
		value_32 = get_ui_32(&S_receive_data_buf[data_pos]);
		
		/* データタイプとデータ値取得 */
		data_type  = (UI_8)(value_32 >> 24);
		data_value =  (value_32 & 0x00ffffffUL);

		switch(data_type) {
		case ESF_RAW_DATA_TYPE_X_AXIS_GYRO:
			info.x_axis_gyro = data_value;
			break;
		case ESF_RAW_DATA_TYPE_Y_AXIS_GYRO:
			info.y_axis_gyro = data_value;
			break;
		case ESF_RAW_DATA_TYPE_Z_AXIS_GYRO:
			info.z_axis_gyro = data_value;
			break;
		case ESF_RAW_DATA_TYPE_X_AXIS_ACCEL:
			info.x_axis_accel = data_value;
			break;
		case ESF_RAW_DATA_TYPE_Y_AXIS_ACCEL:
			info.y_axis_accel = data_value;
			break;
		case ESF_RAW_DATA_TYPE_Z_AXIS_ACCEL:
			info.z_axis_accel = data_value;
			break;
		case ESF_RAW_DATA_TYPE_GYRO_TEMP:
			info.gyro_temp = data_value;
			break;
		default:
			break;
		}
		
		data_pos += 8;
	}

	/* ESF-RAW情報カレント値更新 */
	(void)memcpy(&S_nav_esf_raw, &info, sizeof(T_Gnss_ctrl_esf_raw_info));

	/* ESF-RAW情報有効フラグオン */
	S_nav_info_enable |= NAV_INFO_ENABLE_ESF_RAW;
}


/********************************************************/
/* 関数名称		: change_stat							*/
/* 機能の説明	: 状態遷移処理							*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/*   状態値の更新と共に処理フェーズカウンタのリセットを	*/
/*	 行う。												*/
/********************************************************/
GNSS_CTRL_LOCAL void change_stat(E_GNSS_STAT gnss_stat)
{
	E_GNSS_STAT	prev_stat = S_gnss_stat;

	S_gnss_stat  = gnss_stat;
	reset_phase_count();

	switch(S_gnss_stat) {
	case GNSS_STAT_WORKING:
		if (prev_stat != GNSS_STAT_WORKING_BAUDRATE_CHANGE) {
			(void)System_control_NotifyEvent(SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_OK_GNSS);
		}

		/* GNSSメッセージ受信タイムスタンプ更新 */
		S_gnss_rcv_msg_timestamp = Timer_ctrl_GetTime1ms();
		break;
	case GNSS_STAT_ERROR:
		(void)System_control_NotifyEvent(SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_GNSS);
		break;
	default:
		break;
	}
}

/********************************************************/
/* 関数名称		: reset_phase_count						*/
/* 機能の説明	: 処理フェーズカウンタリセット処理		*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void reset_phase_count(void)
{
	S_proc_phase_count = 0;
	S_proc_phase_timestamp = Timer_ctrl_GetTime1ms();
}

/********************************************************/
/* 関数名称		: inc_phase_count						*/
/* 機能の説明	: 処理フェーズカウンタインクルメント処理*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/08/30								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void inc_phase_count(UI_8 inc_value)
{
	S_proc_phase_count += inc_value;
	S_proc_phase_timestamp = Timer_ctrl_GetTime1ms();
}

/********************************************************/
/* 関数名称		: send_command							*/
/* 機能の説明	: GNSSコマンド送信						*/
/* 引数			: (I)gnss_cmd : 送信対象コマンドデータ	*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/02								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void send_command(const T_gnss_cmd *gnss_cmd)
{
	/* 受信済みデータクリア */
	Gnss_uart_drv_Clear_rx_data(GNSS_UART_CH);
	S_receive_data_size = 0;

	/* コマンドデータ送信 */
	(void)Gnss_uart_drv_Set_tx_data(GNSS_UART_CH, gnss_cmd->cmd, gnss_cmd->cmd_len);
}

/********************************************************/
/* 関数名称		: receive_message						*/
/* 機能の説明	: GNSSメッセージ受信					*/
/* 引数			: (O)class_id : 受信メッセージクラスID	*/
/* 				: (O)msg_id   : 受信メッセージメッセージID	*/
/* 				: (O)data_len : 受信メッセージデータサイズ	*/
/* 戻り値		: 受信メッセージ内容					*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/05								*/
/* 備考			: 										*/
/*   RCV_RESULT_OKをリターンする場合のみ、class_id, 	*/
/*	 msg_id, data_lenを返却する。						*/
/********************************************************/
GNSS_CTRL_LOCAL E_RCV_RESULT receive_message(UI_8 *class_id, UI_8 *msg_id, UI_16 *data_len)
{
	UI_16 msg_len;
	E_GNSS_UART_DRV_RESULT rx_result;
	UI_16 size;

	if(S_receive_data_size < 1) {
		while(S_receive_data_size < 1) {
			size = 1;
			rx_result = Gnss_uart_drv_Get_rx_data(GNSS_UART_CH,
					&S_receive_data_buf[0], &S_receive_error_buf[0], &size);
			if(rx_result != GNSS_UART_DRV_OK) {
				/* メッセージ受信中 */
				return RCV_RESULT_RECEIVING;
			}
		
			if(S_receive_data_buf[0] == 0xB5) {
				S_receive_data_size++;
			}
		}
	}


	/* メッセージ固定部未だ受信していない？ */
	if(S_receive_data_size < sizeof(T_ubx_msg_prefix)) {
		size = sizeof(T_ubx_msg_prefix) - S_receive_data_size;
		rx_result = Gnss_uart_drv_Get_rx_data(GNSS_UART_CH,
				&S_receive_data_buf[S_receive_data_size], &S_receive_error_buf[S_receive_data_size], &size);
		if(rx_result != GNSS_UART_DRV_OK) {
			/* メッセージ受信中 */
			return RCV_RESULT_RECEIVING;
		}

		S_receive_data_size += (UI_8)size;
	}

	if(((S_receive_data_size >= 1) && (S_receive_data_buf[0] != UBX_MSG_HEADER_1))
	 ||((S_receive_data_size >= 2) && (S_receive_data_buf[1] != UBX_MSG_HEADER_2))) {
		/* 受信バッファクリア */
		S_receive_data_size = 0;
		return RCV_RESULT_ERROR;
	}

	if(S_receive_data_size < sizeof(T_ubx_msg_prefix)) {
		/* メッセージ受信中 */
		return RCV_RESULT_RECEIVING;
	}

	/* メッセージヘッダを解析してデータサイズ取得 */
	if(check_ubs_msg_prefix(S_receive_data_buf, data_len) != 1) {
		/* 受信バッファクリア */
		S_receive_data_size = 0;
		return RCV_RESULT_ERROR;
	}
	
	/* メッセージデータ全体のサイズ決定 */
	msg_len = sizeof(T_ubx_msg_prefix) + *data_len + 2;

	/* バッファーサイズオーバーチェック */
	if(msg_len > RECEIVE_DATA_BUF_SIZE) {
		/* 受信バッファクリア */
		S_receive_data_size = 0;
		return RCV_RESULT_BUFFER_OVER;
	}

	if(S_receive_data_size < msg_len) {
		size = msg_len - S_receive_data_size;
		rx_result = Gnss_uart_drv_Get_rx_data(GNSS_UART_CH,
				&S_receive_data_buf[S_receive_data_size], &S_receive_error_buf[S_receive_data_size], &size);
		if(rx_result != GNSS_UART_DRV_OK) {
			/* メッセージ受信中 */
			return RCV_RESULT_RECEIVING;
		}

		S_receive_data_size += (UI_8)size;
	}

	if(S_receive_data_size < msg_len) {
		/* メッセージ受信中 */
		return RCV_RESULT_RECEIVING;
	}

	/* チェックサムのチェック */
	if(check_check_sum(&S_receive_data_buf[2], *data_len + 4) != 1) {
		/* 受信バッファクリア */
		S_receive_data_size = 0;
		return RCV_RESULT_ERROR;
	}
	
	/* 受信完了 */
	*class_id = S_receive_data_buf[2];
	*msg_id   = S_receive_data_buf[3];

	/* 受信バッファクリア */
	S_receive_data_size = 0;
	return RCV_RESULT_OK;
}

/********************************************************/
/* 関数名称		: receive_response						*/
/* 機能の説明	: GNSSコマンド応答受信					*/
/* 引数			: (O)class_id : 応答対象クラスID		*/
/* 				: (O)msg_id   : 応答対象メッセージID	*/
/* 戻り値		: 応答内容								*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/02								*/
/* 備考			: 										*/
/*   CMD_RESPONSE_ACKまたはCMD_RESPONSE_NACKをリターン  */
/*   する場合のみ、class_id, msg_idを返却する。			*/
/********************************************************/
GNSS_CTRL_LOCAL E_CMD_RESPONSE receive_response(UI_8 *class_id, UI_8 *msg_id)
{
	E_CMD_RESPONSE response;
	E_RCV_RESULT result;
	UI_16 data_len;
	UI_8  res_class_id;
	UI_8  res_msg_id;
	
	result = receive_message(&res_class_id, &res_msg_id, &data_len);

	/* 応答対象のクラスIDとメッセージID取得 */
	*class_id = S_receive_data_buf[6];
	*msg_id   = S_receive_data_buf[7];

	switch(result) {
	case RCV_RESULT_OK:
		/* ACK/NACK解析 */
		response = analyze_ubx_ack(S_receive_data_buf, data_len);
		break;
	case RCV_RESULT_EMPTY:
	case RCV_RESULT_RECEIVING:
		/* メッセージPrefixを完全に受信していなければ、応答なし */
		response = CMD_RESPONSE_NONE;
		break;
	case RCV_RESULT_BUFFER_OVER:
	case RCV_RESULT_ERROR:
	default:
		response = CMD_RESPONSE_INVALID;
		break;
	}

	return response;
}

/********************************************************/
/* 関数名称		: analyze_ubx_ack						*/
/* 機能の説明	: UBOX ACKメッセージの解析				*/
/* 引数			: (I)receive_data : 受信データ			*/
/* 				: (I)data_len     : 受信データ長		*/
/* 戻り値		: 応答内容								*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/02								*/
/* 備考			: 										*/
/*   receive_dataは受信した先頭データのアドレス、       */
/*   data_lenにはプレフィックスに格納されているレングス */
/*   を指定すること。									*/
/********************************************************/
GNSS_CTRL_LOCAL E_CMD_RESPONSE analyze_ubx_ack(const UI_8 *receive_data, UI_16 data_len)
{
	/* データ長チェック */
	if(data_len != 2) {
		return CMD_RESPONSE_INVALID;
	}
	
	/* クラスIDチェック */
	if(receive_data[2] != CLASS_ID_ACK) {
		return CMD_RESPONSE_INVALID;
	}
	
	/* メッセージIDチェック */
	if(receive_data[3] == MSG_ID_ACK) {
		return CMD_RESPONSE_ACK;
	}
	else if(receive_data[3] == MSG_ID_NACK) {
		return CMD_RESPONSE_NACK;
	}
	else {
		return CMD_RESPONSE_INVALID;
	}
}

/********************************************************/
/* 関数名称		: check_ubs_msg_prefix					*/
/* 機能の説明	: UBOXメッセージプレフィックスチェック	*/
/* 引数			: (I)ubx_msg  : 受信データ				*/
/* 				: (O)data_len : データ長				*/
/* 戻り値		: 1 : チェックOK，1以外 : チェックNG	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/02								*/
/* 備考			: 										*/
/*   data_lenには、プレフィックスに格納されているレング */
/*   スを返却する。										*/
/********************************************************/
GNSS_CTRL_LOCAL UI_8 check_ubs_msg_prefix(const UI_8 *ubx_msg, UI_16 *data_len)
{
	/* メッセージヘッダチェク */
	if((ubx_msg[0] != UBX_MSG_HEADER_1)
	 ||(ubx_msg[1] != UBX_MSG_HEADER_2)) {
	 	return 0;
	 }

	/* 後続データサイズ取得 */
	*data_len = ((UI_16)ubx_msg[5] << 8) + ubx_msg[4];
	return 1;
}

/********************************************************/
/* 関数名称		: calc_checksum  						*/
/* 機能の説明	: データ列のチェックサム計算			*/
/* 引数			: (I)data : 計算対象データByte列		*/
/* 				: (I)len  : 計算対象データByte列サイズ	*/
/* 				: (O)checksum : チェックサム([0][1])	*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/05								*/
/* 備考			: 										*/
/*   UBXメッセージ後尾に付加するチェックメッセージ		*/
/*  （2Byte)を計算する。								*/
/********************************************************/
GNSS_CTRL_LOCAL void calc_checksum(const UI_8 *pData, UI_16 len, UI_8 *checksum)
{
	UI_16 i;
	
	checksum[0] = 0;
	checksum[1] = 0;
	
	for(i = 0; i < len; i++) {
		checksum[0] += pData[i];
		checksum[1] += checksum[0];
	}

	return;
}

/********************************************************/
/* 関数名称		: check_check_sum						*/
/* 機能の説明	: チェックサムのチェック				*/
/* 引数			: (I)ubx_msg     : 受信メッセージ		*/
/* 				: (O)ubx_msg_len : メッセージ長			*/
/* 戻り値		: 1 : チェックOK，1以外 : チェックNG	*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/02								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL UI_8 check_check_sum(const UI_8 *ubx_msg, UI_16 ubx_msg_len)
{
	UI_8 sum[2] = {0, 0};

	calc_checksum(ubx_msg, ubx_msg_len, sum);

	/* チェクサムの一致チェック */
	if((sum[0] == ubx_msg[ubx_msg_len])
	 &&(sum[1] == ubx_msg[ubx_msg_len + 1])) {
		return 1;
	}
	else {
		return 0;
	}
}

/********************************************************/
/* 関数名称		: get_si_32  							*/
/* 機能の説明	: Byte列から符号付き32ビット整数値取得	*/
/* 引数			: (I)data : 取得対象Byte列				*/
/* 戻り値		: 符号付き32ビット整数値				*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/05								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL SI_32 get_si_32(const UI_8 *data)
{
	SI_32 value = (SI_32)(((UI_32)data[3] << 24) + ((UI_32)data[2] << 16) + ((UI_32)data[1] << 8) + (UI_32)data[0]);
	return value;
}

/********************************************************/
/* 関数名称		: get_ui_32  							*/
/* 機能の説明	: Byte列から符号なし32ビット整数値取得	*/
/* 引数			: (I)data : 取得対象Byte列				*/
/* 戻り値		: 符号なし32ビット整数値				*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/05								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL UI_32 get_ui_32(const UI_8 *data)
{
	UI_32 value = ((UI_32)data[3] << 24) + ((UI_32)data[2] << 16) + ((UI_32)data[1] << 8) + (UI_32)data[0];
	return value;
}

/********************************************************/
/* 関数名称		: get_ui_16  							*/
/* 機能の説明	: Byte列から符号なし16ビット整数値取得	*/
/* 引数			: (I)data : 取得対象Byte列				*/
/* 戻り値		: 符号なし16ビット整数値				*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/05								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL UI_16 get_ui_16(const UI_8 *data)
{
	UI_16 value = ((UI_16)data[1] << 8) + (UI_16)data[0];
	return value;
}

/********************************************************/
/* 関数名称		: get_ui_8  							*/
/* 機能の説明	: Byte列から符号なし8ビット整数値取得	*/
/* 引数			: (I)data : 取得対象Byte列				*/
/* 戻り値		: 符号なし8ビット整数値					*/
/* 作成者		: (OFT)Katsura							*/
/* 作成日		: 16/09/05								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL UI_8  get_ui_8(const UI_8 *data)
{
	UI_8 value = data[0];
	return value;
}


/********************************************************/
/* 関数名称		: gnss_comm_mode_proc					*/
/* 機能の説明	: GNSS通信モード処理					*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void gnss_comm_mode_proc(void)
{
	UI_16	data_len;

	/* GNSS送信可能データ長取得	*/
	data_len = Gnss_uart_drv_get_can_set_tx_data_len(GNSS_UART_CH);
	if (data_len > 0) {						/* GNSS送信可能(送信バッファに空きあり)	*/

		/* ADVANCE UARTデータ取得	*/
		if (data_len > GNSS_COMM_MODE_DATA_BUF_SIZE) {
			/* データサイズをデータ格納バッファサイズ以下にする	*/
			data_len = GNSS_COMM_MODE_DATA_BUF_SIZE;
		}

		/* ADVANCE UARTデータ取得	*/
		data_len = Advance_control_getUartData(S_gnss_comm_mode_data, data_len);
		if (data_len > 0) {					/* ADVANCE UARTデータあり				*/

			/* 取得したデータをGNSSに送信する	*/
			(void)Gnss_uart_drv_Set_tx_data(GNSS_UART_CH, S_gnss_comm_mode_data, (UI_8)data_len);
		}
	}

	return;
}

/********************************************************/
/* 関数名称		: gnss_comm_mode_is_need_brate_change	*/
/* 機能の説明	: GNSS通信モードのボーレート変更検出	*/
/* 引数			: ( /O)brate	変更するボーレート		*/
/* 戻り値		: N_TRUE	ボーレート変更必要			*/
/*				: N_FALSE	ボーレート変更不要			*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/03/01								*/
/* 備考			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL B_8 gnss_comm_mode_is_need_brate_change(E_GNSS_CTRL_BAUDRATE *brate)
{
	if (brate == NULL) {
		return N_FALSE;
	}

	/* SW切り替えによるボーレート変更をチェック */
	if (Sw_control_GetSwMode() == SW_MODE_MANUAL) {
		/* SWがマニュアルRECの場合 */
		if (S_gnss_baudrate != GNSS_CTRL_BAUDRATE_9600) {
			OUTLOG(OutputLog("[GNSS]brate change 9600\n");)
			/* ボーレートが9600bpsではない場合9600bpsに変更する */
			*brate = GNSS_CTRL_BAUDRATE_9600;
			return N_TRUE;
		}
	}
	else {
		/* SWがマニュアルREC以外の場合 */
		if (S_gnss_baudrate != GNSS_CTRL_BAUDRATE_115200) {
			OUTLOG(OutputLog("[GNSS]brate change 115200\n");)
			/* ボーレートが115200bpsではない場合115200bpsに変更する */
			*brate = GNSS_CTRL_BAUDRATE_115200;
			return N_TRUE;
		}
	}

	return N_FALSE;
}
