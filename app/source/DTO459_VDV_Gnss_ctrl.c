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
/* GNSS����												*/
/********************************************************/

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
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
/*	�}�N����`											*/
/********************************************************/
#define GNSS_CTRL_LOCAL	static

#define GNSS_UART_CH		(GNSS_UART_DRV_UART_CH_9)	/* ����Ώۂ�UART�`�����l��				*/
#define RX_BUF_SIZE			2048							/* ��M�o�b�t�@�T�C�Y					*/
#define TX_BUF_SIZE			200							/* ���M�o�b�t�@�T�C�Y					*/

#define GNSS_REST_ON_DULATION			( 10 / 1)		/* GNSS���Z�b�g�I����Ԏ���(1ms�P��)		*/
#define WAIT_GNSS_MODULE_DULATION		(600 / 1)		/* GNSS���W���[������҂�����(1ms�P��)		*/
#define UBX_BAUD_TX_WAIT_TIME			40				/* �{�[���[�g�ύX�R�}���h���M������������	*/
#define ESF_MEAS_RATE					100				/* ESF-MEAS���b�Z�[�W���M����(ms)			*/

#define CMD_RESPONSE_TIMEOUT			(520 / 1)		/* �R�}���h�����΃^�C���A�E�g����(1ms�P��)	*/
#define CMD_SEND_RETRY_MAX				3				/* �R�}���h���M�ő僊�g���C��				*/
#define CMD_SEND_TIMEOUT_MAX			3				/* �R�}���h���M�ő�^�C���A�E�g���[�v��	*/

/* UBX ���b�Z�[�W�w�b�_ */
#define UBX_MSG_HEADER_1				0xb5			/* �w�b�_ 1byte��						*/
#define UBX_MSG_HEADER_2				0x62			/* �w�b�_ 2byte��						*/

/* UBX ACK */
#define CLASS_ID_ACK					0x05			/* �N���XID								*/
#define MSG_ID_ACK						0x01			/* ���b�Z�[�WID : ACK					*/
#define MSG_ID_NACK						0x00			/*                NACK					*/

#define HNR_PVT_CLASS_ID				0x28			/* HNR-PVT �N���XID						*/
#define HNR_PVT_MESSAGE_ID				0x00			/* HNR-PVT ���b�Z�[�WID					*/
#define HNR_PVT_MESSAGE_LEN 			  72			/* HNR-PVT ���b�Z�[�W���i���f�[�^���j	*/
#define NAV_DOP_CLASS_ID				0x01			/* NAV-DOP �N���XID						*/
#define NAV_DOP_MESSAGE_ID				0x04			/* NAV-DOP ���b�Z�[�WID					*/
#define NAV_DOP_MESSAGE_LEN 			  18			/* NAV-DOP ���b�Z�[�W���i���f�[�^���j	*/
#define ESF_MEAS_CLASS_ID				0x10			/* ESF-MEAS �N���XID					*/
#define ESF_MEAS_MESSAGE_ID				0x02			/* ESF-MEAS ���b�Z�[�WID				*/
#define ESF_MEAS_MESSAGE_LEN 			  12			/* ESF-MEAS ���b�Z�[�W���i���f�[�^���j	*/
#define ESF_RAW_CLASS_ID				0x10			/* ESF-RAW �N���XID						*/
#define ESF_RAW_MESSAGE_ID				0x03			/* ESF-RAW ���b�Z�[�WID					*/
#define NAV_PVT_CLASS_ID				0x01			/* NAV-PVT �N���XID						*/
#define NAV_PVT_MESSAGE_ID				0x07			/* NAV-PVT ���b�Z�[�WID					*/
#define NAV_PVT_MESSAGE_LEN				 92				/* NAV-PVT ���b�Z�[�W���i���f�[�^���j	*/
#define NAV_POSECEF_CLASS_ID			0x01			/* NAV-POSECEF �N���XID					*/
#define NAV_POSECEF_MESSAGE_ID			0x01			/* NAV-POSECEF ���b�Z�[�WID				*/
#define NAV_POSECEF_MESSAGE_LEN			 20				/* NAV-POSECEF ���b�Z�[�W���i���f�[�^���j*/

/* ESF-RAW �Z���T�f�[�^�^�C�v */
#define ESF_RAW_DATA_TYPE_X_AXIS_GYRO		14			/* X-axis Gyroscope						*/
#define ESF_RAW_DATA_TYPE_Y_AXIS_GYRO		13			/* Y-axis Gyroscope						*/
#define ESF_RAW_DATA_TYPE_Z_AXIS_GYRO		 5			/* Z-axis Gyroscope						*/
#define ESF_RAW_DATA_TYPE_X_AXIS_ACCEL		16			/* X-axis Accelerometer					*/
#define ESF_RAW_DATA_TYPE_Y_AXIS_ACCEL		17			/* X-axis Accelerometer					*/
#define ESF_RAW_DATA_TYPE_Z_AXIS_ACCEL		18			/* X-axis Accelerometer					*/
#define ESF_RAW_DATA_TYPE_GYRO_TEMP			12			/* Gyroscope Temperature				*/

/* ESF-MEAS �Z���T�f�[�^�^�C�v */
#define ESF_MEAS_DATA_TYPE_X_AXIS_GYRO		14			/* X-axis Gyroscope						*/
#define ESF_MEAS_DATA_TYPE_Y_AXIS_GYRO		13			/* Y-axis Gyroscope						*/
#define ESF_MEAS_DATA_TYPE_Z_AXIS_GYRO		 5			/* Z-axis Gyroscope						*/
#define ESF_MEAS_DATA_TYPE_X_AXIS_ACCEL		16			/* X-axis Accelerometer					*/
#define ESF_MEAS_DATA_TYPE_Y_AXIS_ACCEL		17			/* X-axis Accelerometer					*/
#define ESF_MEAS_DATA_TYPE_Z_AXIS_ACCEL		18			/* X-axis Accelerometer					*/
#define ESF_MEAS_DATA_TYPE_GYRO_TEMP		12			/* Gyroscope Temperature				*/

/* ESF-MEAS �f�[�^�^�C�v */
#define ESF_MEAS_DATA_TYPE_SPEED			11			/* Speed								*/

#define RECEIVE_DATA_BUF_SIZE	128						/* ��M�f�[�^�o�b�t�@�T�C�Y	*/

#define GNSS_COMM_MODE_DATA_BUF_SIZE		TX_BUF_SIZE	/* GNSS�ʐM���[�h�p�f�[�^�o�b�t�@�T�C�Y	*/
#define GNSS_MSG_RCV_TIMEOUT				(10000)		/* GNSS���b�Z�[�W��M�^�C���A�E�g(10000ms)	*/

/* GNSS������ */
typedef enum {
	GNSS_STAT_IDLE = 0,					/* �A�C�h��								*/
	GNSS_STAT_INITIALIZING,				/* ��������								*/
	GNSS_STAT_GNSS_SETTING,				/* GNSS�Z�b�e�B���O��(�{�[���[�g�ύX�O�j*/
	GNSS_STAT_BAUDRATE_CHANGING,		/* �{�[���[�g�ύX��						*/
	GNSS_STAT_GNSS_SETTING_2,			/* GNSS�Z�b�e�B���O��(�{�[���[�g�ύX��j*/
	GNSS_STAT_WORKING,					/* �ʏ퓮�쒆							*/
	GNSS_STAT_WORKING_BAUDRATE_CHANGE,	/* �ʏ퓮�쒆�̃{�[���[�g�ύX			*/
	GNSS_STAT_ERROR						/* �G���[���							*/
} E_GNSS_STAT;

/* �R�}���h���M��� */
typedef enum {
	CMD_SEND_STAT_IDLE = 0,				/* �A�C�h���i�����M�j	*/
	CMD_SEND_STAT_RES_WAIT				/* �����҂��i���M�ς݁j	*/
} E_CMD_SEND_STAT;

/* �R�}���h���� */
typedef enum {
	CMD_RESPONSE_ACK = 0,				/* ACK					*/
	CMD_RESPONSE_NACK,					/* NACK					*/
	CMD_RESPONSE_INVALID,				/* �s������				*/
	CMD_RESPONSE_NONE,					/* �����Ȃ�				*/
	CMD_RESPONSE_ERROR					/* ��M�G���[			*/
} E_CMD_RESPONSE;

/* ���b�Z�[�W��M���� */
typedef enum {
	RCV_RESULT_OK = 0,					/* ��M����				*/
	RCV_RESULT_EMPTY,					/* ��M�Ȃ�				*/
	RCV_RESULT_RECEIVING,				/* ��M��				*/
	RCV_RESULT_BUFFER_OVER,				/* �o�b�t�@�I�[�o�[		*/
	RCV_RESULT_ERROR					/* ��M�G���[			*/
} E_RCV_RESULT;

/* NAV���L���t���O */
typedef enum {
	NAV_INFO_ENABLE_NONE	 =  0,			/* ���ׂĖ���			*/
	NAV_INFO_ENABLE_HNR		 = (1 << 0),	/* NAV-HNR or HVR_PVT��L��		*/
	NAV_INFO_ENABLE_PVT		 = (1 << 1),	/* NAV-PVT���L��		*/
	NAV_INFO_ENABLE_DOP		 = (1 << 2),	/* NAV-POSECEF���L��	*/
	NAV_INFO_ENABLE_POSECEF	 = (1 << 3),	/* NAV-DOP���L��		*/
	NAV_INFO_ENABLE_ESF_RAW	 = (1 << 4),	/* ESF-RAW���L��		*/
	NAV_INFO_ENABLE_ESF_MEAS = (1 << 5)		/* ESF-MEAS���L��		*/
} E_NAV_INFO_ENABLE;

/* �{�[���[�g */
typedef enum {
	GNSS_CTRL_BAUDRATE_9600 = 0,			/* 9600bps				*/
	GNSS_CTRL_BAUDRATE_115200,				/* 115200bps			*/
	GNSS_CTRL_BAUDRATE_NUM					/* �{�[���[�g��			*/
} E_GNSS_CTRL_BAUDRATE;

/********************************************************/
/*	�\���̐錾											*/
/********************************************************/
/* GNSS�R�}���h�f�[�^ */
typedef struct t_gnss_cmd {
	const UI_8 *cmd;					/* �R�}���h�f�[�^		*/
	UI_8 		cmd_len;				/* �R�}���h�f�[�^�T�C�Y	*/
} T_gnss_cmd;

/* UBOX���b�Z�[�WPrefix */
typedef struct {
	UI_8	header[2];					/* �w�b�_			*/
	UI_8	class_id;					/* �N���XID			*/
	UI_8	msg_id;						/* ���b�Z�[�WID		*/
	UI_16	length;						/* �f�[�^��			*/
} T_ubx_msg_prefix;

/********************************************************/
/*	�萔�錾											*/
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
/*�Ԏ�ɂ��ݒ�l�𕪂���*/
#if defined _VDV_86
/*86�p�ݒ�*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xC8, 0x00, 0x1F, 0x00, 0x21, 0x00, 
                                                                                                     0x00, 0x00, 0xC0, 0x00, 0xC0, 0xFF, 0x3B, 0x00, 0x30, 0x9C};
#elif defined _VDV_VITZ
/*Vitz�p�ݒ�*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xD8, 0x00, 0x1C, 0x00, 0x1A, 0x00, 
                                                                                                     0x00, 0x00, 0xEA, 0x00, 0xC2, 0xFF, 0x38, 0x00, 0x5F, 0x10};
#elif defined _VDV_MARKX
/*MarkX�p�ݒ�*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xD2, 0x00, 0x21, 0x00, 0x2B, 0x00,
                                                                                                     0x00, 0x00, 0xDC, 0x00, 0x2D, 0x00, 0x43, 0x00, 0xD8, 0x13};
#elif defined _VDV_RCF
/*RCF�p�ݒ�*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xB4, 0x00, 0x32, 0x00, 0x2E, 0x00, 
                                                                                                     0x00, 0x00, 0xD2, 0x00, 0x24, 0x00, 0x43, 0x00, 0xBB, 0xF9};
#elif defined _VDV_SUPRA
/*SUPRA�p�ݒ�*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xB0, 0x00, 0x25, 0x00, 0x46, 0x00, 
                                                                                                     0x00, 0x00, 0x8C, 0x00, 0xFB, 0xFF, 0x23, 0x00, 0x32, 0x8A};
#elif defined _VDV_LC
/*LC�p�ݒ�*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xB9, 0x00, 0x37, 0x00, 0x26, 0x00, 
                                                                                                     0x00, 0x00, 0xD2, 0x00, 0x38, 0x00, 0x41, 0x00, 0xCF, 0x77};
#elif defined _VDV_YARIS
/*YARIS�p�ݒ�*/ 
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xC8, 0x00, 0x1F, 0x00, 0x1E, 0x00, 
                                                                                                     0x00, 0x00, 0xED, 0x00, 0x28, 0x00, 0x4B, 0x00, 0xD3, 0x4F};
#elif defined _VDV_86B
/*TODO 86B�p�ݒ�*/ 
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xC8, 0x00, 0x1F, 0x00, 0x21, 0x00, 
                                                                                                     0x00, 0x00, 0xC0, 0x00, 0xC0, 0xFF, 0x3B, 0x00, 0x30, 0x9C};
#else
/*���̑��Ԏ�i86�Ɠ����j*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_esfla[]   = {0xB5, 0x62, 0x06, 0x2F, 0x14, 0x00, 0x00, 0x02, 0x02, 0x20, 0x01, 0x00, 0xC8, 0x00, 0x1F, 0x00, 0x21, 0x00, 
                                                                                                     0x00, 0x00, 0xC0, 0x00, 0xC0, 0xFF, 0x3B, 0x00, 0x30, 0x9C};
#endif

/* CFG-GNSS:GNSS�ݒ�                                             Length--->  mgVer numHw numUs numBk gnsID resTC maxTC rsv   flags-----------------> CheckSum->*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_gnss[]    = {0xB5, 0x62, 0x06, 0x3E, 0x3C, 0x00, 0x00, 0x1C, 0x1C, 0x07, 0x00, 0x08, 0x10, 0x00, 0x01, 0x00, 0x01, 0x01,
                                                                                                     0x01, 0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0x01,
                                                                                                     0x02, 0x04, 0x08, 0x00, 0x00, 0x00, 0x01, 0x01,
                                                                                                     0x03, 0x08, 0x10, 0x00, 0x00, 0x00, 0x01, 0x01,
                                                                                                     0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x03,
                                                                                                     0x05, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x05,
                                                                                                     0x06, 0x08, 0x0E, 0x00, 0x00, 0x00, 0x01, 0x01, 0x4B, 0x4B};

/* CFG-PRT:�|�[�g�ݒ�                                                    Length--->  PtID  rsv   txReady-->  mode----------------->  baudRate------------->  inPortMask  outPortMask flags---->  rsv------>  CheckSum->*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_prt_ddc[]         = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9E, 0x88};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_prt_usb[]         = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1D, 0x84};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_prt_spi[]         = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x98};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_prt_uart_115200[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xC0, 0x08, 0x00, 0x00, 0x00, 0xC2, 0x01, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0, 0x7E};
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_prt_uart_9600[]   = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xC0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x92, 0xB5};

/* CFG-HNR:HNR�v�����[�g�ݒ�                                     Length--->  Hz->  reserve1------->  CheckSum->*/
/*ADR OFF�̏ꍇ��0Hz*/
/*0Hz*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_hnr[]     = {0xB5, 0x62, 0x06, 0x5C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0xCC};

/* CFG-RATE:�v�����[�g�ݒ�                                    Length--->  measRate->  navRate-->  timeRef-->  CheckSum->*/
/*ADR OFF�̏ꍇ��10Hz*/
/*10Hz*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_rate[] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00, 0x01, 0x00, 0x7A, 0x12};

/* CFG-MSG:���b�Z�[�W���M���[�g�ݒ�                                   Length--->  mgCls mgID  rate----------------------------->  CheckSum->*/
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
/* --- ADR OFF���̐ݒ� --- */

#if defined GNSS_VER_410
/* Version 4.10�ȏ�i4.31���܂ށj*/
/*GNSS�t�@�[���E�F�A�o�[�W�����A�b�v��(�����Ӂ����̐ݒ�R�}���h�̓t�@�[���E�F�A�A�b�v�f�[�g��̂ݎg�p�\)*/
/*�����FVersion 4.10���烁�b�Z�[�W�o�[�W�����݂̂�ύX��������*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_navx5[] = {0xB5, 0x62, 0x06, 0x23, 0x2C, 0x00, 0x03, 0x00, 0x4C, 0x66, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4B, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA9, 0xCE};
#else /* 2021.07.06 S.Sakata Version4.10��4.31�����ʉ�*/
/* Version 4.10����*/
/*GNSS�t�@�[���E�F�A�o�[�W�����A�b�v�O*/
GNSS_CTRL_LOCAL const UI_8 C_ubx_cfg_navx5[] = {0xB5, 0x62, 0x06, 0x23, 0x28, 0x00, 0x02, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x03, 0x02, 0x05, 0x18, 0x0C, 0x00, 0x00, 0x01, 0x00, 0x00, 0x4B, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x08, 0x64, 0x64, 0x00, 0x00, 0x01, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB5, 0xD4};
#endif /*GNSS_VER_410*/



/* �{�[���[�g�ύX�O���s�R�}���h�e�[�u�� */
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
	{	0,						0								},	/* �e�[�u���I�[		*/
};

/* �{�[���[�g�ύX�R�}���h */
GNSS_CTRL_LOCAL const T_gnss_cmd C_change_boudrate_cmd = {
	C_ubx_cfg_prt_uart_115200, sizeof(C_ubx_cfg_prt_uart_115200),
};

/* �{�[���[�g�ύX�㔭�s�R�}���h�e�[�u�� */
GNSS_CTRL_LOCAL const T_gnss_cmd C_init_cmd_tbl_2[] = {
	{	C_ubx_cfg_navx5,		sizeof(C_ubx_cfg_navx5)			},	/*ADR ON/OFF �ݒ�p */
	{	C_ubx_cfg_hnr,			sizeof(C_ubx_cfg_hnr)			},
	{	C_ubx_cfg_rate,			sizeof(C_ubx_cfg_rate)			},
	{	C_ubx_cfg_msg_nav_dop,	sizeof(C_ubx_cfg_msg_nav_dop)	},
	{	C_ubx_cfg_msg_hnr_pvt,	sizeof(C_ubx_cfg_msg_hnr_pvt)	},
	{	C_ubx_cfg_msg_posecef,	sizeof(C_ubx_cfg_msg_posecef)	},
	{	C_ubx_cfg_msg_nav_pvt,	sizeof(C_ubx_cfg_msg_nav_pvt)	},
	{	C_ubx_cfg_msg_esf_meas,	sizeof(C_ubx_cfg_msg_esf_meas)	},
	{	0,						0								},	/* �e�[�u���I�[		*/
};


/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
GNSS_CTRL_LOCAL UI_8		   S_uart_rx_buf[RX_BUF_SIZE];			/* UART��M�o�b�t�@			*/
GNSS_CTRL_LOCAL UI_8		   S_uart_er_buf[RX_BUF_SIZE];			/* UART��M�G���[�o�b�t�@	*/
GNSS_CTRL_LOCAL UI_8		   S_uart_tx_buf[TX_BUF_SIZE];			/* UART���M�o�b�t�@			*/
GNSS_CTRL_LOCAL E_GNSS_STAT S_gnss_stat  = GNSS_STAT_IDLE;		/* GNSS������				*/	
GNSS_CTRL_LOCAL UI_16       S_proc_phase_count = 0;				/* �����t�F�[�Y�J�E���g		*/
GNSS_CTRL_LOCAL UI_32       S_proc_phase_timestamp = 0;			/* �����t�F�[�Y�ω����̃^�C���X�^���v	*/
GNSS_CTRL_LOCAL E_CMD_SEND_STAT	S_cmd_send_status = CMD_SEND_STAT_IDLE;		/* �R�}���h���M���			*/
GNSS_CTRL_LOCAL UI_8        S_cmd_send_retry_count = 0;			/* �R�}���h���M���g���C�J�E���g		*/
GNSS_CTRL_LOCAL UI_8        S_cmd_send_timeout_count = 0;		/* �R�}���h���M�^�C���A�E�g�J�E���g	*/

GNSS_CTRL_LOCAL UI_8 S_receive_data_buf[RECEIVE_DATA_BUF_SIZE];	/* GNSS��M�f�[�^�o�b�t�@�T�C�Y	*/
GNSS_CTRL_LOCAL UI_8 S_receive_error_buf[RECEIVE_DATA_BUF_SIZE];	/* GNSS��M�G���[�o�b�t�@�T�C�Y	*/
GNSS_CTRL_LOCAL UI_8 S_receive_data_size = 0;					/* GNSS��M�f�[�^�T�C�Y			*/

GNSS_CTRL_LOCAL UI_8					    S_nav_info_enable;		/* NAV���L���t���O		*/
GNSS_CTRL_LOCAL T_Gnss_ctrl_hnr_pvt_info	S_hnr_pvt_info;			/* HNR-PVT���				*/
GNSS_CTRL_LOCAL T_Gnss_ctrl_nav_pvt_info	S_nav_pvt_info;			/* NAV-PVT���				*/
GNSS_CTRL_LOCAL T_Gnss_ctrl_nav_dop_info	S_nav_dop_info;			/* NAV-DOP���				*/
GNSS_CTRL_LOCAL T_Gnss_ctrl_nav_posecef_info	S_nav_posecef_info;	/* NAV-POSECEF���			*/
GNSS_CTRL_LOCAL T_Gnss_ctrl_esf_raw_info	S_nav_esf_raw;			/* ESF-RAW���				*/
GNSS_CTRL_LOCAL T_Gnss_ctrl_esf_meas_info	S_nav_esf_meas;		/* ESF-MEAS���				*/
#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata */
GNSS_CTRL_LOCAL UI_8						S_fixtype_enabled;		/* GPS�P��⑫�ς݃t���O	*/
#endif /* _VDV_SUPRA�@2018.12.14 S.Sakata */

GNSS_CTRL_LOCAL E_GNSS_CTRL_BAUDRATE		S_gnss_baudrate;		/* GNSS�{�[���[�g			*/
GNSS_CTRL_LOCAL UI_8	S_gnss_comm_mode_data[GNSS_COMM_MODE_DATA_BUF_SIZE];	/* GNSS�ʐM���[�h�p�f�[�^�o�b�t�@	*/
GNSS_CTRL_LOCAL UI_32 S_gnss_rcv_msg_timestamp;					/* GNSS���b�Z�[�W��M�^�C���X�^���v		*/

/********************************************************/
/*	�����֐��v���g�^�C�v�錾							*/
/********************************************************/
/* ���W���[������֘A */
GNSS_CTRL_LOCAL void proc_for_idle(void);
GNSS_CTRL_LOCAL void proc_for_initilizing(void);
GNSS_CTRL_LOCAL void proc_for_gnss_setting(void);
GNSS_CTRL_LOCAL void proc_for_baudrate_changing(void);
GNSS_CTRL_LOCAL void proc_for_gnss_setting_2(void);
GNSS_CTRL_LOCAL void proc_for_working(void);
GNSS_CTRL_LOCAL void proc_for_working_baudrate_change(void);

/* GNSS��M���b�Z�[�W�����֘A */
GNSS_CTRL_LOCAL void analyze_message_hnr_pvt(UI_16 datalen);
GNSS_CTRL_LOCAL void analyze_message_nav_pvt(UI_16 datalen);
GNSS_CTRL_LOCAL void analyze_message_nav_dop(UI_16 datalen);
GNSS_CTRL_LOCAL void analyze_message_nav_posecef(UI_16 datalen);
GNSS_CTRL_LOCAL void analyze_message_esf_raw(UI_16 datalen);
GNSS_CTRL_LOCAL void analyze_message_esf_meas(UI_16 datalen);

/* ��ԑJ�ڑ���֘A */
GNSS_CTRL_LOCAL void change_stat(E_GNSS_STAT gnss_stat);
GNSS_CTRL_LOCAL void reset_phase_count(void);
GNSS_CTRL_LOCAL void inc_phase_count(UI_8 inc_value);

/* GNSS�R�}���h����M�֘A */
GNSS_CTRL_LOCAL void           send_command(const T_gnss_cmd *gnss_cmd);
GNSS_CTRL_LOCAL E_RCV_RESULT   receive_message(UI_8 *class_id, UI_8 *msg_id, UI_16 *data_len);
GNSS_CTRL_LOCAL E_CMD_RESPONSE receive_response(UI_8 *class_id, UI_8 *msg_id);
GNSS_CTRL_LOCAL E_CMD_RESPONSE analyze_ubx_ack(const UI_8 *receive_data, UI_16 data_len);
GNSS_CTRL_LOCAL UI_8           check_ubs_msg_prefix(const UI_8 *ubx_msg, UI_16 *data_len);
GNSS_CTRL_LOCAL UI_8           check_check_sum(const UI_8 *ubx_msg, UI_16 ubx_msg_len);
GNSS_CTRL_LOCAL void 		  calc_checksum(const UI_8 *pData, UI_16 len, UI_8 *checksum);

/* Byte�񂩂�̐��l�擾�֘A */
GNSS_CTRL_LOCAL SI_32 get_si_32(const UI_8 *data);
GNSS_CTRL_LOCAL UI_32 get_ui_32(const UI_8 *data);
GNSS_CTRL_LOCAL UI_16 get_ui_16(const UI_8 *data);
GNSS_CTRL_LOCAL UI_8  get_ui_8(const UI_8 *data);


/* GNSS�ʐM���[�h�����֘A */
GNSS_CTRL_LOCAL void gnss_comm_mode_proc(void);
GNSS_CTRL_LOCAL B_8 gnss_comm_mode_is_need_brate_change(E_GNSS_CTRL_BAUDRATE *brate);

/********************************************************/
/* �֐�����		: Gnss_cntrol_GetHnrPvtInfo				*/
/* �@�\�̐���	: GNSS HNR-PVT���J�����g�l���擾����	*/
/* ����			: void									*/
/* �߂�l		: T_Gnss_ctrl_hnr_pvt_info :			*/
/*									HNR-PVT���i�[Ptr	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/02								*/
/* ���l			: 										*/
/********************************************************/
T_Gnss_ctrl_hnr_pvt_info *Gnss_cntrol_GetHnrPvtInfo(void)
{
	/* HNR-PVT���̗L������ */
	if((S_nav_info_enable & NAV_INFO_ENABLE_HNR) == 0) {
		return(NULL);
	}

	return(&S_hnr_pvt_info);
}

/********************************************************/
/* �֐�����		: Gnss_cntrol_GetNavPvtInfo				*/
/* �@�\�̐���	: GNSS NAV-PVT���J�����g�l���擾����	*/
/* ����			: void									*/
/* �߂�l		: T_Gnss_ctrl_nav_pvt_info :			*/
/*									NAV-PVT���i�[Ptr	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/28								*/
/* ���l			: 										*/
/*   �L����NAV-PVT��񂪑��݂��Ȃ��ꍇ�ANULL�����^�[��	*/
/*   ����B												*/
/********************************************************/
T_Gnss_ctrl_nav_pvt_info *Gnss_cntrol_GetNavPvtInfo(void)
{
	/* NAV-PVT���̗L������ */
	if((S_nav_info_enable & NAV_INFO_ENABLE_PVT) == 0) {
		return(NULL);
	}

	return(&S_nav_pvt_info);
}

/********************************************************/
/* �֐�����		: Gnss_control_GetNavDopInfo			*/
/* �@�\�̐���	: GNSS NAV-DOP���J�����g�l���擾����	*/
/* ����			: void									*/
/* �߂�l		: T_Gnss_ctrl_nav_dop_info :			*/
/*									NAV-DOP���i�[Ptr	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/02								*/
/* ���l			: 										*/
/*   �L����NAV-DOP��񂪑��݂��Ȃ��ꍇ�ANULL�����^�[��	*/
/*   ����B												*/
/********************************************************/
T_Gnss_ctrl_nav_dop_info *Gnss_control_GetNavDopInfo(void)
{
	/* NAV-DOP���̗L������ */
	if((S_nav_info_enable & NAV_INFO_ENABLE_DOP) == 0) {
		return(NULL);
	}

	return(&S_nav_dop_info);
}

/********************************************************/
/* �֐�����		: Gnss_cntrol_GetNavPosecefInfo			*/
/* �@�\�̐���	: GNSS NAV-POSECEF���J�����g�l���擾����	*/
/* ����			: void									*/
/* �߂�l		: T_Gnss_ctrl_nav_posecef_info :		*/
/*								NAV-POSECEF���i�[Ptr	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/28								*/
/* ���l			: 										*/
/*   �L����NAV-PVT��񂪑��݂��Ȃ��ꍇ�ANULL�����^�[��	*/
/*   ����B												*/
/********************************************************/
T_Gnss_ctrl_nav_posecef_info *Gnss_cntrol_GetNavPosecefInfo(void)
{
	/* NAV-PPOSECEF���̗L������ */
	if((S_nav_info_enable & NAV_INFO_ENABLE_POSECEF) == 0) {
		return(NULL);
	}

	return(&S_nav_posecef_info);
}

/********************************************************/
/* �֐�����		�F Gnss_control_GetEsfMeasInfo			*/
/* �@�\�̐���	�F GNSS ESF-MEAS���J�����g�l���擾����*/
/* ����			�F void									*/
/* �߂�l		�F T_Gnss_ctrl_esf_meas_info :			*/
/*									ESF-MEAS���i�[Ptr	*/
/* �쐬��		�F (OFT)Katsura							*/
/* �쐬��		�F 16/09/02								*/
/* ���l			�F 										*/
/********************************************************/
T_Gnss_ctrl_esf_meas_info *Gnss_control_GetEsfMeasInfo(void)
{
	/* ESF-MEAS���̗L������ */
	if((S_nav_info_enable & NAV_INFO_ENABLE_ESF_MEAS) == 0) {
		return(NULL);
	}

	return(&S_nav_esf_meas);
}


/********************************************************/
/* �֐�����		: Gnss_control_GetEsfRawInfo			*/
/* �@�\�̐���	: GNSS ESF-RAW���J�����g�l���擾����	*/
/* ����			: void									*/
/* �߂�l		: T_Gnss_ctrl_esf_raw_info :			*/
/*									ESF-RAW���i�[Ptr	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/02								*/
/* ���l			: 										*/
/********************************************************/
T_Gnss_ctrl_esf_raw_info *Gnss_control_GetEsfRawInfo(void)
{
	/* ESF-ROWP���̗L������ */
	if((S_nav_info_enable & NAV_INFO_ENABLE_ESF_RAW) == 0) {
		return(NULL);
	}

	return(&S_nav_esf_raw);
}

#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata */
/********************************************************/
/* �֐�����		: Gnss_control_iSGnssFixedAtOneTime		*/
/* �@�\�̐���	: GPSFixType�l���ߋ��ɂP�x�ł���M�ς�	*/
/* 				: �ƂȂ���������						*/
/* ����			: void									*/
/* �߂�l		: N_TRUE:��M�ς� / N_FALSE:����M		*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 18/12/14								*/
/* ���l			: 										*/
/********************************************************/
B_8 Gnss_control_iSGnssFixedAtOneTime(void)
{
	/*GPS�P��⑫�ς݂��H*/
	if(S_fixtype_enabled == 0)
	{
		/*���⑫*/
		return(N_FALSE);
	}
	/*�⑫��*/
	return(N_TRUE);
}
#endif /* _VDV_SUPRA�@2018.12.14 S.Sakata */

/********************************************************/
/* �֐�����		: Gnss_control_GetGpsFixType			*/
/* �@�\�̐���	: GPS Fix Type�l���擾����				*/
/* ����			: (O)gps_fix_type : GPS Fix Type�l		*/
/* �߂�l		: E_N_RC : �擾����(N_RC_OK, N_RC_NG)	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/07								*/
/* ���l			: 										*/
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
/* �֐�����		: Gnss_control_getUartData				*/
/* �@�\�̐���	:	UART�f�[�^�擾						*/
/* ����			:	(I/ )buf	�f�[�^�o�b�t�@			*/
/*				:	(I/ )len	�擾�f�[�^��(byte)		*/
/* �߂�l		:	�擾�ł����f�[�^��(byte)			*/
/* �쐬��		:	(OFT)Toguchi						*/
/* �쐬��		:	17/01/20							*/
/* ���l			: 										*/
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
/* �֐�����		: Gnss_control_Init						*/
/* �@�\�̐���	: GNSS���䏉����						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Gnss_control_Init(void)
{
	T_Uart_buf_info buf_info;

	/* ����M�o�b�t�@���쐬 */
	buf_info.rx_buf      = S_uart_rx_buf;
	buf_info.er_buf      = S_uart_er_buf;
	buf_info.tx_buf      = S_uart_tx_buf;
	buf_info.rx_buf_size = RX_BUF_SIZE;
	buf_info.tx_buf_size = TX_BUF_SIZE;

	/* UART�h���C�o�����ݒ� */
	Gnss_uart_drv_Init(GNSS_UART_CH, &buf_info);

	/* UART�ʐM�J�n */
	Gnss_uart_drv_Start(GNSS_UART_CH);

	/* �����ԏ����� */
	change_stat(GNSS_STAT_IDLE);

	/* NAV-HNR, NAV-DOP���N���A */
	(void)memset(&S_hnr_pvt_info, 0x00, sizeof(S_hnr_pvt_info));
	(void)memset(&S_nav_dop_info, 0x00, sizeof(S_nav_dop_info));
	(void)memset(&S_nav_pvt_info, 0x00, sizeof(S_nav_pvt_info));
	(void)memset(&S_nav_posecef_info, 0x00, sizeof(S_nav_posecef_info));
	(void)memset(&S_nav_esf_meas, 0x00, sizeof(S_nav_esf_meas));
#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata */
	S_fixtype_enabled = 0;
#endif /* _VDV_SUPRA�@2018.12.14 S.Sakata */

	/* NAV���L���t���O�N���A */
	S_nav_info_enable = NAV_INFO_ENABLE_NONE;

	/* GNSS�ʐM�̃{�[���[�g�����l�ݒ�i115200bps�j */
	S_gnss_baudrate = GNSS_CTRL_BAUDRATE_115200;

	S_gnss_rcv_msg_timestamp = 0;
}


/********************************************************/
/* �֐�����		: Gnss_control_Main						*/
/* �@�\�̐���	: GNSS���䃁�C��						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Gnss_control_Main(void)
{
	switch(S_gnss_stat) {
	case GNSS_STAT_IDLE:
		/* �A�C�h����Ԃł̏��� */
		proc_for_idle();
		break;
	case GNSS_STAT_INITIALIZING:
		/* ���������̏��� */
		proc_for_initilizing();
		break;
	case GNSS_STAT_GNSS_SETTING:
		/* GNSS�ݒ蒆�̏���(�{�[���[�g�ύX�O) */
		proc_for_gnss_setting();
		break;
	case GNSS_STAT_BAUDRATE_CHANGING:
		proc_for_baudrate_changing();
		break;
	case GNSS_STAT_GNSS_SETTING_2:
		/* GNSS�ݒ蒆�̏���(�{�[���[�g�ύX��) */
		proc_for_gnss_setting_2();
		break;
	case GNSS_STAT_WORKING:
		/* �ʏ퓮�쒆�̏��� */
		proc_for_working();
		break;
	case GNSS_STAT_WORKING_BAUDRATE_CHANGE:
		/* �ʏ퓮�쒆�̃{�[���[�g�ύX���� */
		proc_for_working_baudrate_change();
		break;
	default:
		break;
	}
}

/********************************************************/
/* �֐�����		: proc_for_idle							*/
/* �@�\�̐���	: �A�C�h����Ԏ��������				*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void proc_for_idle(void)
{
	E_SYS_CTRL_STATUS sys_ctrl_status;
	
	sys_ctrl_status = System_control_GetStatus();

	switch(sys_ctrl_status) {
	case SYS_OPERATION_WAIT_ACC_ON:
	case SYS_OPERATION_PRE_ACC_OFF:
	case SYS_OPERATION_ACC_OFF:
		/* ACC ON�҂�,ACC OFF�ȍ~�AACC OFF��ԂȂ牽�����Ȃ� */
		break;
	default:
		/* ����ȊO�Ȃ珉���������֑J�� */
		change_stat(GNSS_STAT_INITIALIZING);
		break;
	}
}

/********************************************************/
/* �֐�����		: proc_for_initilizing					*/
/* �@�\�̐���	: ����������Ԏ��������				*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void proc_for_initilizing(void)
{
	E_SYS_CTRL_STATUS sys_ctrl_status;

	/* ���ݎ����擾 */
	UI_32 cur_time = Timer_ctrl_GetTime1ms();
	
	/* �V�X�e�������Ԏ擾�擾 */
	sys_ctrl_status = System_control_GetStatus();
	switch(sys_ctrl_status) {
	case SYS_OPERATION_PRE_ACC_OFF:
	case SYS_OPERATION_ACC_OFF:
		/* ACC OFF�ڍs,ACC OFF�Ȃ�A�C�h���֑J�� */
		change_stat(GNSS_STAT_IDLE);
		/*��ԕω����͏����𔲂���*/
		return;
	default:
		break;
	}

	if(S_proc_phase_count == 0) {
		/* GNSS RESET �I�� */
		
		/* ���̏����t�F�[�Y�� */
		inc_phase_count(1);
	}
	else if(S_proc_phase_count == 1) {
		/* GNSS RESET ON��Ԍo�ߔ��� */
		if((cur_time - S_proc_phase_timestamp) > GNSS_REST_ON_DULATION) {
			/* GNSS RESET �I�t */

			/* ���̏����t�F�[�Y�� */
			inc_phase_count(1);
		}
	}
	else if(S_proc_phase_count == 2) {
		/* GNSS���W���[������҂���Ԍo�ߔ��� */
		if((cur_time - S_proc_phase_timestamp) > WAIT_GNSS_MODULE_DULATION) {
			/* GNSS�ݒ蒆��Ԃ֑J�� */
			change_stat(GNSS_STAT_GNSS_SETTING);
		}
	}
	else {
		/* nop */
	}
}

/********************************************************/
/* �֐�����		: proc_for_gnss_setting					*/
/* �@�\�̐���	: GNSS�ݒ蒆��Ԏ��������				*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void proc_for_gnss_setting(void)
{
	UI_8 response;
	UI_8 class_id;
	UI_8 msg_id;
	E_SYS_CTRL_STATUS sys_ctrl_status;

	/* ���ݎ����擾 */
	UI_32 cur_time = Timer_ctrl_GetTime1ms();
	
	sys_ctrl_status = System_control_GetStatus();
	switch(sys_ctrl_status) {
	case SYS_OPERATION_PRE_ACC_OFF:
	case SYS_OPERATION_ACC_OFF:
		/* ACC OFF�ڍs,ACC OFF�Ȃ�A�C�h���֑J�� */
		change_stat(GNSS_STAT_IDLE);
		/*��ԕω����͏����𔲂���*/
		return;
	default:
		break;
	}

	switch(S_cmd_send_status) {
	case CMD_SEND_STAT_IDLE:		/* �R�}���h�����M��� */
		if(C_init_cmd_tbl[S_proc_phase_count].cmd_len == 0) {
			/* �{�[���[�g�ύX��Ԃ֑J�� */
			change_stat(GNSS_STAT_BAUDRATE_CHANGING);
			return;
		}

		/* �����ݒ�R�}���h�𑗐M */
		send_command(&C_init_cmd_tbl[S_proc_phase_count]);

		/* ���g���C�J�E���g�A�^�C���A�E�g�J�E���g�N���A */
		S_cmd_send_retry_count   = 0;
		S_cmd_send_timeout_count = 0;
		
		/* �R�}���h���M��� �����҂��ɑJ�� */
		S_cmd_send_status = CMD_SEND_STAT_RES_WAIT;
		break;
	case CMD_SEND_STAT_RES_WAIT:	/* �R�}���h�����҂����	*/
		/* ������M */
		response = receive_response(&class_id, &msg_id);
		
		/* ACK��M�����ꍇ */
		if(response == CMD_RESPONSE_ACK) {
			/* �N���XID�ƃ��b�Z�[�WID�����M�����R�}���h�ƈ�v���Ă��邩�`�F�b�N */
			if((class_id != C_init_cmd_tbl[S_proc_phase_count].cmd[2])
			 &&(msg_id   != C_init_cmd_tbl[S_proc_phase_count].cmd[3])) {
			 	/* ��v���Ă��Ȃ���΁ANACK�Ƃ݂Ȃ��čđ� */
			 	response = CMD_RESPONSE_NACK;
			}
		}

		switch(response) {
		case CMD_RESPONSE_ACK:		/* ACK��M		*/
			/* ���̏����t�F�[�Y�� */
			inc_phase_count(1);
			
			/* �R�}���h���M��� �A�C�h���֑J�� */
			S_cmd_send_status = CMD_SEND_STAT_IDLE;
		
			/* �����ݒ�R�}���h���ׂđ��M������ */
			if(C_init_cmd_tbl[S_proc_phase_count].cmd_len == 0) {
				/* �{�[���[�g�ύX��Ԃ֑J�� */
				change_stat(GNSS_STAT_BAUDRATE_CHANGING);
			}
			break;
		case CMD_RESPONSE_NACK:		/* NACK��M		*/
			if(S_cmd_send_retry_count >= CMD_SEND_RETRY_MAX) {
				/* �G���[��Ԃ֑J�� */
				change_stat(GNSS_STAT_ERROR);
			}
			else {
				/* �����ݒ�R�}���h�đ� */
				send_command(&C_init_cmd_tbl[S_proc_phase_count]);
				S_cmd_send_retry_count++;

				/* �t�F�[�Y�J�ڎ����X�V */
				inc_phase_count(0);
			}
			break;
		case CMD_RESPONSE_NONE:		/* �����Ȃ�		*/
		default:
			/* �����^�C���A�E�g�`�F�b�N */
			if((cur_time - S_proc_phase_timestamp) > CMD_RESPONSE_TIMEOUT) {
				if(S_cmd_send_timeout_count >= CMD_SEND_TIMEOUT_MAX) {
					/* �G���[��Ԃ֑J�� */
					change_stat(GNSS_STAT_ERROR);
				}
				else {
					/* �����ݒ�R�}���h�đ� */
					send_command(&C_init_cmd_tbl[S_proc_phase_count]);
					S_cmd_send_timeout_count++;

					/* �t�F�[�Y�J�ڎ����X�V */
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
/* �֐�����		: proc_for_baudrate_changing			*/
/* �@�\�̐���	: �{�[���[�g�ύX����Ԏ��������		*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/05								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void proc_for_baudrate_changing(void)
{
	UI_8 response;
	UI_8 class_id;
	UI_8 msg_id;

	E_SYS_CTRL_STATUS sys_ctrl_status;

	/* ���ݎ����擾 */
	UI_32 cur_time = Timer_ctrl_GetTime1ms();
	
	/* �V�X�e�������Ԏ擾�擾 */
	sys_ctrl_status = System_control_GetStatus();
	switch(sys_ctrl_status) {
	case SYS_OPERATION_PRE_ACC_OFF:
	case SYS_OPERATION_ACC_OFF:
		/* ACC OFF�ڍs,ACC OFF�Ȃ�A�C�h���֑J�� */
		change_stat(GNSS_STAT_IDLE);
		/*��ԕω����͏����𔲂���*/
		return;
	default:
		break;
	}

	if(S_proc_phase_count == 0) {
		/* �{�[���[�g�ݒ�R�}���h���M */
		send_command(&C_change_boudrate_cmd);

		/* ���̏����t�F�[�Y�� */
		inc_phase_count(1);
	}
	else if(S_proc_phase_count == 1) {
		/* �{�[���[�g�ݒ�R�}���h���M�����������Ԍo�߂��Ă���A�{�[���[�g�ύX */
		if((cur_time - S_proc_phase_timestamp) >= UBX_BAUD_TX_WAIT_TIME) {
			Gnss_uart_drv_Set_bdrate(GNSS_UART_CH, GNSS_BAUDRATE_115200_BPS);

			Gnss_uart_drv_Clear_rx_data(GNSS_UART_CH);
			Gnss_uart_drv_Start_rx(GNSS_UART_CH);

			/* ���̏����t�F�[�Y�� */
			inc_phase_count(1);
		}
	}
	else if(S_proc_phase_count == 2) {
		/* ������M */
		response = receive_response(&class_id, &msg_id);

		/* ACK��M�����ꍇ */
		if(response == CMD_RESPONSE_ACK) {
			/* �N���XID�ƃ��b�Z�[�WID�����M�����R�}���h�ƈ�v���Ă��邩�`�F�b�N */
			if((class_id != C_change_boudrate_cmd.cmd[2])
			 &&(msg_id   != C_change_boudrate_cmd.cmd[3])) {
			 	/* ��v���Ă��Ȃ���΁ANACK�Ƃ݂Ȃ��čđ� */
			 	response = CMD_RESPONSE_NACK;
			}
		}

		switch(response) {
		case CMD_RESPONSE_ACK:		/* ACK��M		*/
			/* �ʏ퓮�쒆��Ԃ֑J�� */
			change_stat(GNSS_STAT_GNSS_SETTING_2);
			break;
		case CMD_RESPONSE_NACK:		/* NACK��M		*/
			/* �G���[��Ԃ֑J�� */
			change_stat(GNSS_STAT_ERROR);
			break;
		case CMD_RESPONSE_NONE:		/* �����Ȃ�		*/
		default:

			/* �����^�C���A�E�g�`�F�b�N */
			if((cur_time - S_proc_phase_timestamp) > CMD_RESPONSE_TIMEOUT) {
				if(S_cmd_send_timeout_count >= CMD_SEND_TIMEOUT_MAX) {
					/* �G���[��Ԃ֑J�� */
					change_stat(GNSS_STAT_ERROR);
				}
				else {
					/* �{�[���[�g�ݒ�R�}���h���M */
					send_command(&C_change_boudrate_cmd);
					S_cmd_send_timeout_count++;

					/* �t�F�[�Y�J�ڎ����X�V */
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
/* �֐�����		: proc_for_gnss_setting_2				*/
/* �@�\�̐���	: �{�[���[�g�ύX��GNSS�ݒ蒆��Ԏ��������	*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void proc_for_gnss_setting_2(void)
{
	UI_8 response;
	UI_8 class_id;
	UI_8 msg_id;
	E_SYS_CTRL_STATUS sys_ctrl_status;

	/* ���ݎ����擾 */
	UI_32 cur_time = Timer_ctrl_GetTime1ms();
	
	sys_ctrl_status = System_control_GetStatus();
	switch(sys_ctrl_status) {
	case SYS_OPERATION_PRE_ACC_OFF:
	case SYS_OPERATION_ACC_OFF:
		/* ACC OFF�ڍs,ACC OFF�Ȃ�A�C�h���֑J�� */
		change_stat(GNSS_STAT_IDLE);
		/*��ԕω����͏����𔲂���*/
		return;
	default:
		break;
	}

	switch(S_cmd_send_status) {
	case CMD_SEND_STAT_IDLE:		/* �R�}���h�����M��� */
		if(C_init_cmd_tbl_2[S_proc_phase_count].cmd_len == 0) {
			/* �ʏ퓮�쒆��Ԃ֑J�� */
			change_stat(GNSS_STAT_WORKING);
			return;
		}
		
		if(S_proc_phase_count == 0) {
			if((cur_time - S_proc_phase_timestamp) <= 1000) {
				return;
			}
		}
		
		/* �����ݒ�R�}���h�𑗐M */
		send_command(&C_init_cmd_tbl_2[S_proc_phase_count]);

		/* ���g���C�J�E���g�A�^�C���A�E�g�J�E���g�N���A */
		S_cmd_send_retry_count   = 0;
		S_cmd_send_timeout_count = 0;
		
		/* �R�}���h���M��� �����҂��ɑJ�� */
		S_cmd_send_status = CMD_SEND_STAT_RES_WAIT;
		break;
	case CMD_SEND_STAT_RES_WAIT:	/* �R�}���h�����҂����	*/
		/* ������M */
		response = receive_response(&class_id, &msg_id);
		
		/* ACK��M�����ꍇ */
		if(response == CMD_RESPONSE_ACK) {
			/* �N���XID�ƃ��b�Z�[�WID�����M�����R�}���h�ƈ�v���Ă��邩�`�F�b�N */
			if((class_id != C_init_cmd_tbl_2[S_proc_phase_count].cmd[2])
			 &&(msg_id   != C_init_cmd_tbl_2[S_proc_phase_count].cmd[3])) {
			 	/* ��v���Ă��Ȃ���΁ANACK�Ƃ݂Ȃ��čđ� */
			 	response = CMD_RESPONSE_NACK;
			}
		}

		switch(response) {
		case CMD_RESPONSE_ACK:		/* ACK��M		*/
			/* ���̏����t�F�[�Y�� */
			inc_phase_count(1);
			
			/* �R�}���h���M��� �A�C�h���֑J�� */
			S_cmd_send_status = CMD_SEND_STAT_IDLE;
		
			/* �����ݒ�R�}���h���ׂđ��M������ */
			if(C_init_cmd_tbl_2[S_proc_phase_count].cmd_len == 0) {
				/* �ʏ퓮�쒆��Ԃ֑J�� */
				change_stat(GNSS_STAT_WORKING);
			}
			break;
		case CMD_RESPONSE_NACK:		/* NACK��M		*/
			if(S_cmd_send_retry_count >= CMD_SEND_RETRY_MAX) {
				/* �G���[��Ԃ֑J�� */
				change_stat(GNSS_STAT_ERROR);
			}
			else {
				/* �����ݒ�R�}���h�đ� */
				send_command(&C_init_cmd_tbl_2[S_proc_phase_count]);
				S_cmd_send_retry_count++;

				/* �t�F�[�Y�J�ڎ����X�V */
				inc_phase_count(0);
			}
			break;
		case CMD_RESPONSE_NONE:		/* �����Ȃ�		*/
		default:
			/* �����^�C���A�E�g�`�F�b�N */
			if((cur_time - S_proc_phase_timestamp) > CMD_RESPONSE_TIMEOUT) {
				if(S_cmd_send_timeout_count >= CMD_SEND_TIMEOUT_MAX) {
					/* �G���[��Ԃ֑J�� */
					change_stat(GNSS_STAT_ERROR);
				}
				else {
				/* �����ݒ�R�}���h�đ� */
					send_command(&C_init_cmd_tbl_2[S_proc_phase_count]);
					S_cmd_send_timeout_count++;

					/* �t�F�[�Y�J�ڎ����X�V */
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
/* �֐�����		: proc_for_working						*/
/* �@�\�̐���	: �ʏ퓮�쒆��Ԏ��������				*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void proc_for_working(void)
{
	UI_8  class_id;
	UI_8  msg_id;
	UI_16 data_len;
	E_SYS_CTRL_STATUS sys_ctrl_status;
	E_GNSS_CTRL_BAUDRATE brate;

	/* ���ݎ����擾 */
	UI_32 cur_time = Timer_ctrl_GetTime1ms();

	sys_ctrl_status = System_control_GetStatus();
	switch(sys_ctrl_status) {
	case SYS_OPERATION_PRE_ACC_OFF:
	case SYS_OPERATION_ACC_OFF:
		/* ACC OFF�ڍs,ACC OFF�Ȃ�A�C�h���֑J�� */
		change_stat(GNSS_STAT_IDLE);
		/*��ԕω����͏����𔲂���*/
		return;

	case SYS_OPERATION_ACC_ON_UNKNOWN_STAT:
		/* ��M�f�[�^���N���A */
		Gnss_uart_drv_Clear_rx_data(GNSS_UART_CH);
		break;

	case SYS_OPERATION_GNSS_COMM:
		if (gnss_comm_mode_is_need_brate_change(&brate) == N_TRUE) {
			/* �{�[���[�g�ύX */
			S_gnss_baudrate = brate;

			/* �ʏ퓮�쒆�̃{�[���[�g�ύX��ԂɑJ�� */
			change_stat(GNSS_STAT_WORKING_BAUDRATE_CHANGE);
		}
		else {
			gnss_comm_mode_proc();
		}
		break;

	default:

		/* ���b�Z�[�W��M */
		while(receive_message(&class_id, &msg_id, &data_len) == RCV_RESULT_OK) {
			S_gnss_rcv_msg_timestamp = Timer_ctrl_GetTime1ms();

			if((class_id == HNR_PVT_CLASS_ID) && (msg_id == HNR_PVT_MESSAGE_ID)) {
				/* HNR-PVT���b�Z�[�W��� */
				analyze_message_hnr_pvt(data_len);
			}
			else if((class_id == NAV_PVT_CLASS_ID) && (msg_id == NAV_PVT_MESSAGE_ID)) {
				/* NAV-PVT���b�Z�[�W��� */
				analyze_message_nav_pvt(data_len);
			}
			else if((class_id == NAV_DOP_CLASS_ID) && (msg_id == NAV_DOP_MESSAGE_ID)) {
				/* NAV-DOP���b�Z�[�W��� */
				analyze_message_nav_dop(data_len);
			}
			else if((class_id == NAV_POSECEF_CLASS_ID) && (msg_id == NAV_POSECEF_MESSAGE_ID)) {
				/* NAV-POSECEF���b�Z�[�W��� */
				analyze_message_nav_posecef(data_len);
			}
			else if((class_id == ESF_RAW_CLASS_ID) && (msg_id == ESF_RAW_MESSAGE_ID)) {
				/* ESF-RAW���b�Z�[�W��� */
				analyze_message_esf_raw(data_len);
			}
			else if((class_id == ESF_MEAS_CLASS_ID) && (msg_id == ESF_MEAS_MESSAGE_ID)) {
				/* ESF-MEAS���b�Z�[�W��� */
				analyze_message_esf_meas(data_len);
			}
			else {
				/* nop */
			}
		}

		/* GNSS���b�Z�[�W����M�ł��Ȃ����Ԃ��`�F�b�N */
		if (Timer_ctrl_TimeCompare1ms(S_gnss_rcv_msg_timestamp, GNSS_MSG_RCV_TIMEOUT) == N_OK) {
			/* ���b�Z�[�W��M�^�C���A�E�g	*/
			/* �G���[��Ԃ֑J�� */
			change_stat(GNSS_STAT_ERROR);
		}
		else {
		}

		break;
	}

}

/********************************************************/
/* �֐�����		: proc_for_working_baudrate_chang		*/
/* �@�\�̐���	: �ʏ퓮�쒆�̃{�[���[�g�ύX��Ԏ��������	*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/03/01								*/
/* ���l			: 										*/
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

	/* �V�X�e�������Ԏ擾�擾 */
	sys_ctrl_status = System_control_GetStatus();
	switch(sys_ctrl_status) {
	case SYS_OPERATION_PRE_ACC_OFF:
	case SYS_OPERATION_ACC_OFF:
		/* ACC OFF�ڍs,ACC OFF�Ȃ�A�C�h���֑J�� */
		change_stat(GNSS_STAT_IDLE);
		/*��ԕω����͏����𔲂���*/
		return;
	default:
		break;
	}

	/* ���ݎ����擾 */
	cur_time = Timer_ctrl_GetTime1ms();

	if(S_proc_phase_count == 0) {
		/* �{�[���[�g�ݒ�R�}���h���M */
		if (S_gnss_baudrate == GNSS_CTRL_BAUDRATE_9600) {
			gnss_cmd.cmd = C_ubx_cfg_prt_uart_9600;
			gnss_cmd.cmd_len = sizeof(C_ubx_cfg_prt_uart_9600);
		}
		else {
			gnss_cmd.cmd = C_ubx_cfg_prt_uart_115200;
			gnss_cmd.cmd_len = sizeof(C_ubx_cfg_prt_uart_115200);
		}
		send_command(&gnss_cmd);

		/* �^�C���A�E�g�J�E���g�N���A */
		S_cmd_send_timeout_count = 0;

		/* ���̏����t�F�[�Y�� */
		inc_phase_count(1);
	}
	else if(S_proc_phase_count == 1) {
		/* �{�[���[�g�ݒ�R�}���h���M�����������Ԍo�߂��Ă���A�{�[���[�g�ύX */
		if((cur_time - S_proc_phase_timestamp) >= UBX_BAUD_TX_WAIT_TIME) {
			if (S_gnss_baudrate == GNSS_CTRL_BAUDRATE_9600) {
				drv_brate = GNSS_BAUDRATE_9600_BPS;
			}
			else {
				drv_brate = GNSS_BAUDRATE_115200_BPS;
			}
			Gnss_uart_drv_Set_bdrate(GNSS_UART_CH, drv_brate);

			/* ���b�Z�[�W��M�o�b�t�@�N���A */
			S_receive_data_size = 0;
			Gnss_uart_drv_Clear_rx_data(GNSS_UART_CH);
			Gnss_uart_drv_Start_rx(GNSS_UART_CH);

			/* ���̏����t�F�[�Y�� */
			inc_phase_count(1);
		}
	}
	else if(S_proc_phase_count == 2) {
		/* �����^�C���A�E�g�`�F�b�N */
		if((cur_time - S_proc_phase_timestamp) > CMD_RESPONSE_TIMEOUT) {
			if(S_cmd_send_timeout_count >= 50) {
				OUTLOG(OutputLog("[GNSS]WBRChange timeout err!!\n");)
				/* �G���[  �{�[���[�g�ύX�ł��Ȃ��ꍇ���ʏ퓮�쒆��Ԃ֑J�� */
				change_stat(GNSS_STAT_WORKING);
			}
			else {
				/* �{�[���[�g�ݒ�R�}���h���M */
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
				/* �t�F�[�Y�J�ڎ����X�V */
				inc_phase_count(0);
			}
		}
		else {
			/* ������M */
			response = receive_response(&class_id, &msg_id);
			if(response == CMD_RESPONSE_ACK) {
				/* ACK��M�����ꍇ */
				/* �N���XID�ƃ��b�Z�[�WID�����M�����R�}���h�ƈ�v���Ă��邩�`�F�b�N */
				if((class_id == C_ubx_cfg_prt_uart_115200[2])
				 &&(msg_id   == C_ubx_cfg_prt_uart_115200[3])) {
					OUTLOG(OutputLog("[GNSS]WBRChange ACK\n");)
					/* ��v���Ă���΁A�ʏ퓮�쒆��Ԃ֑J�� */
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
/* �֐�����		: analyze_message_hnr_pvt				*/
/* �@�\�̐���	: HVR-PVT���b�Z�[�W��͏���				*/
/* ����			: (I)datalen:��M�f�[�^��				*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/05								*/
/* ���l			: 										*/
/*   ��M����HVR-PVT���b�Z�[�W�̓��e����͂��āA�O����	*/
/*   �񋟂���HVR-PVT���(S_hnr_pvt_info)�̓��e���X�V����B*/
/********************************************************/
GNSS_CTRL_LOCAL void analyze_message_hnr_pvt(UI_16 datalen)
{
	T_Gnss_ctrl_hnr_pvt_info info;
	UI_8 data_pos;
	
	/* �f�[�^����HNR-PVT�̋K��ƈقȂ�Ή�̓G���[ */
	if(datalen != HNR_PVT_MESSAGE_LEN) {
		return;
	}


	/* HNR-PVT���N���A */
	(void)memset(&info, 0x00, sizeof(T_Gnss_ctrl_hnr_pvt_info));

	/* �f�[�^�Q�ƈʒu������ */
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

	/* reserved1 �g�p���� */
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

	/* �����o�b�t�@�փR�s�[ */
	(void)memcpy(&S_hnr_pvt_info, &info, sizeof(T_Gnss_ctrl_hnr_pvt_info));

#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata GNSS_ADR_ON��GNSS_HNR_PVT_ENABLE���L���łȂ��Ƃ��̏����͒ʂ�Ȃ�*/
	if(	S_fixtype_enabled == 0)
	{
		/* GPS�P�x���⑫���Ă��Ȃ�	*/
		if( (info.gpsFix == GNSS_GSPFIX_2DFIX) || 
			(info.gpsFix == GNSS_GSPFIX_3DFIX) || 
			(info.gpsFix == GNSS_GSPFIX_GPS_AND_DEADRECKONING) ||
			(info.gpsFix == GNSS_GSPFIX_DEADRECKONIN_ONLY)	)
		{
			/* GPS�P�x���⑫�ς�*/
			S_fixtype_enabled = 1;
		}
	}
#endif /* _VDV_SUPRA�@2018.12.14 S.Sakata GNSS_ADR_ON��GNSS_HNR_PVT_ENABLE���L���łȂ��Ƃ��̏����͒ʂ�Ȃ�*/
	/* HNR-PVT���L���t���O�I�� */
	S_nav_info_enable |= NAV_INFO_ENABLE_HNR;
}

/********************************************************/
/* �֐�����		: analyze_message_nav_pvt				*/
/* �@�\�̐���	: NAV-PVT���b�Z�[�W��͏���				*/
/* ����			: (I)datalen:��M�f�[�^��				*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/26								*/
/* ���l			: 										*/
/*   ��M����NAV-PVT���b�Z�[�W�̓��e����͂��āA�O����	*/
/*   �񋟂���NAV-PVT���(S_nav_pvt_info)�̓��e���X�V����B*/
/********************************************************/
GNSS_CTRL_LOCAL void analyze_message_nav_pvt(UI_16 datalen)
{
	T_Gnss_ctrl_nav_pvt_info info;
	UI_8 data_pos;

	/* �f�[�^����NAV-HNR�̋K��ƈقȂ�Ή�̓G���[ */
	if(datalen != NAV_PVT_MESSAGE_LEN) {
		return;
	}


	/* NAV-HNR���N���A */
	(void)memset(&info, 0x00, sizeof(T_Gnss_ctrl_nav_pvt_info));

	/* �f�[�^�Q�ƈʒu������ */
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

	/* reserved1 �g�p���� */
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

	/* reserved2 �g�p���� */
	data_pos += 6;

	info.head_veh = get_si_32(&S_receive_data_buf[data_pos]);
	data_pos += 4;

	/* �����o�b�t�@�փR�s�[ */
	(void)memcpy(&S_nav_pvt_info, &info, sizeof(T_Gnss_ctrl_nav_pvt_info));

#if defined _VDV_SUPRA /* 2018.12.14 S.Sakata */
	if(	S_fixtype_enabled == 0)
	{
		/* GPS�P�x���⑫���Ă��Ȃ�	*/
		if( (info.gpsFix == GNSS_GSPFIX_2DFIX) || 
			(info.gpsFix == GNSS_GSPFIX_3DFIX) || 
			(info.gpsFix == GNSS_GSPFIX_GPS_AND_DEADRECKONING) ||
			(info.gpsFix == GNSS_GSPFIX_DEADRECKONIN_ONLY)	)
		{
			/* GPS�P�x���⑫�ς�*/
			S_fixtype_enabled = 1;
		}
	}
#endif /* _VDV_SUPRA�@2018.12.14 S.Sakata */
	/* NAV-PVT���L���t���O�I�� */
	S_nav_info_enable |= NAV_INFO_ENABLE_PVT;
}

/********************************************************/
/* �֐�����		: analyze_message_nav_posecef			*/
/* �@�\�̐���	: NAV-POSECEF���b�Z�[�W��͏���			*/
/* ����			: (I)datalen:��M�f�[�^��				*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/28								*/
/* ���l			: 										*/
/*   ��M����NAV-POSECEF���b�Z�[�W�̓��e����͂��āA	*/
/*   �O���ɒ񋟂���NAV-POSECEF���(S_nav_posecef_info)  */
/*   �̓��e���X�V����B									*/
/********************************************************/
GNSS_CTRL_LOCAL void analyze_message_nav_posecef(UI_16 datalen)
{
	T_Gnss_ctrl_nav_posecef_info info;
	UI_8 data_pos;

	/* �f�[�^����NAV-HNR�̋K��ƈقȂ�Ή�̓G���[ */
	if(datalen != NAV_POSECEF_MESSAGE_LEN) {
		return;
	}


	/* NAV-POSECEF���N���A */
	(void)memset(&info, 0x00, sizeof(T_Gnss_ctrl_nav_posecef_info));

	/* �f�[�^�Q�ƈʒu������ */
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

	/* �����o�b�t�@�փR�s�[ */
	(void)memcpy(&S_nav_posecef_info, &info, sizeof(T_Gnss_ctrl_nav_posecef_info));

	/* NAV-POSECEF���L���t���O�I�� */
	S_nav_info_enable |= NAV_INFO_ENABLE_POSECEF;
}

/********************************************************/
/* �֐�����		: analyze_message_nav_dop				*/
/* �@�\�̐���	: NAV-DOP���b�Z�[�W��͏���				*/
/* ����			: (I)datalen:��M�f�[�^��				*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/05								*/
/* ���l			: 										*/
/*   ��M����NAV-DOP���b�Z�[�W�̓��e����͂��āA�O����	*/
/*   �񋟂���NAV-DOP���(S_nav_dop_info)�̓��e���X�V����B*/
/********************************************************/
GNSS_CTRL_LOCAL void analyze_message_nav_dop(UI_16 datalen)
{
	T_Gnss_ctrl_nav_dop_info info;
	UI_8 data_pos;

	/* �f�[�^����NAV-DOP�̋K��ƈقȂ�Ή�̓G���[ */
	if(datalen != NAV_DOP_MESSAGE_LEN) {
		return;
	}


	/* NAV-HNR���N���A */
	(void)memset(&info, 0x00, sizeof(T_Gnss_ctrl_nav_dop_info));

	/* �f�[�^�Q�ƈʒu������ */
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

	/* �����o�b�t�@�փR�s�[ */
	(void)memcpy(&S_nav_dop_info, &info, sizeof(T_Gnss_ctrl_nav_dop_info));

	/* NAV-DOP���L���t���O�I�� */
	S_nav_info_enable |= NAV_INFO_ENABLE_DOP;
}


/********************************************************/
/* �֐�����		�F analyze_message_esf_meas				*/
/* �@�\�̐���	�F ESF-MEAS���b�Z�[�W��͏���			*/
/* ����			�F (I)datalen:��M�f�[�^��				*/
/* �߂�l		�F void									*/
/* �쐬��		�F (OFT)Katsura							*/
/* �쐬��		�F 16/09/08								*/
/* ���l			�F 										*/
/*   ��M����ESF-MEAS���b�Z�[�W�̓��e����͂��āA�O����	*/
/*   �񋟂���ESF-MEAS���(S_nav_esf_meas)�̓��e���X�V����B*/
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

	/* �f�[�^����(8 + 4*N)�łȂ��Ƃ����Ȃ� */
	if(((datalen  - 8) % 4) != 0) {
		return;
	}

	
	/* �f�[�^���ڐ��擾 */
	item_count = (datalen  - 8) / 4;

	/* ESF-MEAS���J�����g�l���R�s�[ */
	(void)memcpy(&info, &S_nav_esf_meas, sizeof(T_Gnss_ctrl_esf_meas_info));

	/* �f�[�^�Q�ƈʒu������ */
	data_pos = 6;

	/* �擪4Byte��Reserve */
	data_pos += 8;

	/* �S���ڑ��� */
	for(i = 0; i < item_count; i++) {
		/* �f�[�^�擾 */
		value_32 = get_ui_32(&S_receive_data_buf[data_pos]);
		
		/* �f�[�^�^�C�v�ƃf�[�^�l�擾 */
		data_type  = (UI_8)(value_32 >> 24);
		data_value =  (value_32 & 0x00ffffff);
		/*�}�C�i�X�l*/
		if((data_value & 0x00800000) != 0)
		{
			/*��ʂ�FF�Ŗ��߂�*/
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

	/* ESF-MEAS���J�����g�l�X�V */
	(void)memcpy(&S_nav_esf_meas, &info, sizeof(T_Gnss_ctrl_esf_meas_info));

	/* ESF-MEAS���L���t���O�I�� */
	S_nav_info_enable |= NAV_INFO_ENABLE_ESF_MEAS;
}

/********************************************************/
/* �֐�����		: analyze_message_esf_raw				*/
/* �@�\�̐���	: ESF-RAW���b�Z�[�W��͏���				*/
/* ����			: (I)datalen:��M�f�[�^��				*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/08								*/
/* ���l			: 										*/
/*   ��M����ESF-RAW���b�Z�[�W�̓��e����͂��āA�O����	*/
/*   �񋟂���ESF-RAW���(S_nav_esf_raw)�̓��e���X�V����B*/
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

	/* �f�[�^����(4 + 8*N)�łȂ��Ƃ����Ȃ� */
	if(((datalen  - 4) % 8) != 0) {
		return;
	}
	
	/* �f�[�^���ڐ��擾 */
	item_count = (datalen  - 4) / 8;

	/* ESF-RAW���J�����g�l���R�s�[ */
	(void)memcpy(&info, &S_nav_esf_raw, sizeof(T_Gnss_ctrl_esf_raw_info));

	/* �f�[�^�Q�ƈʒu������ */
	data_pos = 6;

	/* �擪4Byte��Reserve */
	data_pos += 4;

	/* �S���ڑ��� */
	for(i = 0; i < item_count; i++) {
		/* �f�[�^�擾 */
		value_32 = get_ui_32(&S_receive_data_buf[data_pos]);
		
		/* �f�[�^�^�C�v�ƃf�[�^�l�擾 */
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

	/* ESF-RAW���J�����g�l�X�V */
	(void)memcpy(&S_nav_esf_raw, &info, sizeof(T_Gnss_ctrl_esf_raw_info));

	/* ESF-RAW���L���t���O�I�� */
	S_nav_info_enable |= NAV_INFO_ENABLE_ESF_RAW;
}


/********************************************************/
/* �֐�����		: change_stat							*/
/* �@�\�̐���	: ��ԑJ�ڏ���							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/*   ��Ԓl�̍X�V�Ƌ��ɏ����t�F�[�Y�J�E���^�̃��Z�b�g��	*/
/*	 �s���B												*/
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

		/* GNSS���b�Z�[�W��M�^�C���X�^���v�X�V */
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
/* �֐�����		: reset_phase_count						*/
/* �@�\�̐���	: �����t�F�[�Y�J�E���^���Z�b�g����		*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void reset_phase_count(void)
{
	S_proc_phase_count = 0;
	S_proc_phase_timestamp = Timer_ctrl_GetTime1ms();
}

/********************************************************/
/* �֐�����		: inc_phase_count						*/
/* �@�\�̐���	: �����t�F�[�Y�J�E���^�C���N�������g����*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/08/30								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void inc_phase_count(UI_8 inc_value)
{
	S_proc_phase_count += inc_value;
	S_proc_phase_timestamp = Timer_ctrl_GetTime1ms();
}

/********************************************************/
/* �֐�����		: send_command							*/
/* �@�\�̐���	: GNSS�R�}���h���M						*/
/* ����			: (I)gnss_cmd : ���M�ΏۃR�}���h�f�[�^	*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/02								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void send_command(const T_gnss_cmd *gnss_cmd)
{
	/* ��M�ς݃f�[�^�N���A */
	Gnss_uart_drv_Clear_rx_data(GNSS_UART_CH);
	S_receive_data_size = 0;

	/* �R�}���h�f�[�^���M */
	(void)Gnss_uart_drv_Set_tx_data(GNSS_UART_CH, gnss_cmd->cmd, gnss_cmd->cmd_len);
}

/********************************************************/
/* �֐�����		: receive_message						*/
/* �@�\�̐���	: GNSS���b�Z�[�W��M					*/
/* ����			: (O)class_id : ��M���b�Z�[�W�N���XID	*/
/* 				: (O)msg_id   : ��M���b�Z�[�W���b�Z�[�WID	*/
/* 				: (O)data_len : ��M���b�Z�[�W�f�[�^�T�C�Y	*/
/* �߂�l		: ��M���b�Z�[�W���e					*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/05								*/
/* ���l			: 										*/
/*   RCV_RESULT_OK�����^�[������ꍇ�̂݁Aclass_id, 	*/
/*	 msg_id, data_len��ԋp����B						*/
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
				/* ���b�Z�[�W��M�� */
				return RCV_RESULT_RECEIVING;
			}
		
			if(S_receive_data_buf[0] == 0xB5) {
				S_receive_data_size++;
			}
		}
	}


	/* ���b�Z�[�W�Œ蕔������M���Ă��Ȃ��H */
	if(S_receive_data_size < sizeof(T_ubx_msg_prefix)) {
		size = sizeof(T_ubx_msg_prefix) - S_receive_data_size;
		rx_result = Gnss_uart_drv_Get_rx_data(GNSS_UART_CH,
				&S_receive_data_buf[S_receive_data_size], &S_receive_error_buf[S_receive_data_size], &size);
		if(rx_result != GNSS_UART_DRV_OK) {
			/* ���b�Z�[�W��M�� */
			return RCV_RESULT_RECEIVING;
		}

		S_receive_data_size += (UI_8)size;
	}

	if(((S_receive_data_size >= 1) && (S_receive_data_buf[0] != UBX_MSG_HEADER_1))
	 ||((S_receive_data_size >= 2) && (S_receive_data_buf[1] != UBX_MSG_HEADER_2))) {
		/* ��M�o�b�t�@�N���A */
		S_receive_data_size = 0;
		return RCV_RESULT_ERROR;
	}

	if(S_receive_data_size < sizeof(T_ubx_msg_prefix)) {
		/* ���b�Z�[�W��M�� */
		return RCV_RESULT_RECEIVING;
	}

	/* ���b�Z�[�W�w�b�_����͂��ăf�[�^�T�C�Y�擾 */
	if(check_ubs_msg_prefix(S_receive_data_buf, data_len) != 1) {
		/* ��M�o�b�t�@�N���A */
		S_receive_data_size = 0;
		return RCV_RESULT_ERROR;
	}
	
	/* ���b�Z�[�W�f�[�^�S�̂̃T�C�Y���� */
	msg_len = sizeof(T_ubx_msg_prefix) + *data_len + 2;

	/* �o�b�t�@�[�T�C�Y�I�[�o�[�`�F�b�N */
	if(msg_len > RECEIVE_DATA_BUF_SIZE) {
		/* ��M�o�b�t�@�N���A */
		S_receive_data_size = 0;
		return RCV_RESULT_BUFFER_OVER;
	}

	if(S_receive_data_size < msg_len) {
		size = msg_len - S_receive_data_size;
		rx_result = Gnss_uart_drv_Get_rx_data(GNSS_UART_CH,
				&S_receive_data_buf[S_receive_data_size], &S_receive_error_buf[S_receive_data_size], &size);
		if(rx_result != GNSS_UART_DRV_OK) {
			/* ���b�Z�[�W��M�� */
			return RCV_RESULT_RECEIVING;
		}

		S_receive_data_size += (UI_8)size;
	}

	if(S_receive_data_size < msg_len) {
		/* ���b�Z�[�W��M�� */
		return RCV_RESULT_RECEIVING;
	}

	/* �`�F�b�N�T���̃`�F�b�N */
	if(check_check_sum(&S_receive_data_buf[2], *data_len + 4) != 1) {
		/* ��M�o�b�t�@�N���A */
		S_receive_data_size = 0;
		return RCV_RESULT_ERROR;
	}
	
	/* ��M���� */
	*class_id = S_receive_data_buf[2];
	*msg_id   = S_receive_data_buf[3];

	/* ��M�o�b�t�@�N���A */
	S_receive_data_size = 0;
	return RCV_RESULT_OK;
}

/********************************************************/
/* �֐�����		: receive_response						*/
/* �@�\�̐���	: GNSS�R�}���h������M					*/
/* ����			: (O)class_id : �����ΏۃN���XID		*/
/* 				: (O)msg_id   : �����Ώۃ��b�Z�[�WID	*/
/* �߂�l		: �������e								*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/02								*/
/* ���l			: 										*/
/*   CMD_RESPONSE_ACK�܂���CMD_RESPONSE_NACK�����^�[��  */
/*   ����ꍇ�̂݁Aclass_id, msg_id��ԋp����B			*/
/********************************************************/
GNSS_CTRL_LOCAL E_CMD_RESPONSE receive_response(UI_8 *class_id, UI_8 *msg_id)
{
	E_CMD_RESPONSE response;
	E_RCV_RESULT result;
	UI_16 data_len;
	UI_8  res_class_id;
	UI_8  res_msg_id;
	
	result = receive_message(&res_class_id, &res_msg_id, &data_len);

	/* �����Ώۂ̃N���XID�ƃ��b�Z�[�WID�擾 */
	*class_id = S_receive_data_buf[6];
	*msg_id   = S_receive_data_buf[7];

	switch(result) {
	case RCV_RESULT_OK:
		/* ACK/NACK��� */
		response = analyze_ubx_ack(S_receive_data_buf, data_len);
		break;
	case RCV_RESULT_EMPTY:
	case RCV_RESULT_RECEIVING:
		/* ���b�Z�[�WPrefix�����S�Ɏ�M���Ă��Ȃ���΁A�����Ȃ� */
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
/* �֐�����		: analyze_ubx_ack						*/
/* �@�\�̐���	: UBOX ACK���b�Z�[�W�̉��				*/
/* ����			: (I)receive_data : ��M�f�[�^			*/
/* 				: (I)data_len     : ��M�f�[�^��		*/
/* �߂�l		: �������e								*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/02								*/
/* ���l			: 										*/
/*   receive_data�͎�M�����擪�f�[�^�̃A�h���X�A       */
/*   data_len�ɂ̓v���t�B�b�N�X�Ɋi�[����Ă��郌���O�X */
/*   ���w�肷�邱�ƁB									*/
/********************************************************/
GNSS_CTRL_LOCAL E_CMD_RESPONSE analyze_ubx_ack(const UI_8 *receive_data, UI_16 data_len)
{
	/* �f�[�^���`�F�b�N */
	if(data_len != 2) {
		return CMD_RESPONSE_INVALID;
	}
	
	/* �N���XID�`�F�b�N */
	if(receive_data[2] != CLASS_ID_ACK) {
		return CMD_RESPONSE_INVALID;
	}
	
	/* ���b�Z�[�WID�`�F�b�N */
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
/* �֐�����		: check_ubs_msg_prefix					*/
/* �@�\�̐���	: UBOX���b�Z�[�W�v���t�B�b�N�X�`�F�b�N	*/
/* ����			: (I)ubx_msg  : ��M�f�[�^				*/
/* 				: (O)data_len : �f�[�^��				*/
/* �߂�l		: 1 : �`�F�b�NOK�C1�ȊO : �`�F�b�NNG	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/02								*/
/* ���l			: 										*/
/*   data_len�ɂ́A�v���t�B�b�N�X�Ɋi�[����Ă��郌���O */
/*   �X��ԋp����B										*/
/********************************************************/
GNSS_CTRL_LOCAL UI_8 check_ubs_msg_prefix(const UI_8 *ubx_msg, UI_16 *data_len)
{
	/* ���b�Z�[�W�w�b�_�`�F�N */
	if((ubx_msg[0] != UBX_MSG_HEADER_1)
	 ||(ubx_msg[1] != UBX_MSG_HEADER_2)) {
	 	return 0;
	 }

	/* �㑱�f�[�^�T�C�Y�擾 */
	*data_len = ((UI_16)ubx_msg[5] << 8) + ubx_msg[4];
	return 1;
}

/********************************************************/
/* �֐�����		: calc_checksum  						*/
/* �@�\�̐���	: �f�[�^��̃`�F�b�N�T���v�Z			*/
/* ����			: (I)data : �v�Z�Ώۃf�[�^Byte��		*/
/* 				: (I)len  : �v�Z�Ώۃf�[�^Byte��T�C�Y	*/
/* 				: (O)checksum : �`�F�b�N�T��([0][1])	*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/05								*/
/* ���l			: 										*/
/*   UBX���b�Z�[�W����ɕt������`�F�b�N���b�Z�[�W		*/
/*  �i2Byte)���v�Z����B								*/
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
/* �֐�����		: check_check_sum						*/
/* �@�\�̐���	: �`�F�b�N�T���̃`�F�b�N				*/
/* ����			: (I)ubx_msg     : ��M���b�Z�[�W		*/
/* 				: (O)ubx_msg_len : ���b�Z�[�W��			*/
/* �߂�l		: 1 : �`�F�b�NOK�C1�ȊO : �`�F�b�NNG	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/02								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL UI_8 check_check_sum(const UI_8 *ubx_msg, UI_16 ubx_msg_len)
{
	UI_8 sum[2] = {0, 0};

	calc_checksum(ubx_msg, ubx_msg_len, sum);

	/* �`�F�N�T���̈�v�`�F�b�N */
	if((sum[0] == ubx_msg[ubx_msg_len])
	 &&(sum[1] == ubx_msg[ubx_msg_len + 1])) {
		return 1;
	}
	else {
		return 0;
	}
}

/********************************************************/
/* �֐�����		: get_si_32  							*/
/* �@�\�̐���	: Byte�񂩂畄���t��32�r�b�g�����l�擾	*/
/* ����			: (I)data : �擾�Ώ�Byte��				*/
/* �߂�l		: �����t��32�r�b�g�����l				*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/05								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL SI_32 get_si_32(const UI_8 *data)
{
	SI_32 value = (SI_32)(((UI_32)data[3] << 24) + ((UI_32)data[2] << 16) + ((UI_32)data[1] << 8) + (UI_32)data[0]);
	return value;
}

/********************************************************/
/* �֐�����		: get_ui_32  							*/
/* �@�\�̐���	: Byte�񂩂畄���Ȃ�32�r�b�g�����l�擾	*/
/* ����			: (I)data : �擾�Ώ�Byte��				*/
/* �߂�l		: �����Ȃ�32�r�b�g�����l				*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/05								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL UI_32 get_ui_32(const UI_8 *data)
{
	UI_32 value = ((UI_32)data[3] << 24) + ((UI_32)data[2] << 16) + ((UI_32)data[1] << 8) + (UI_32)data[0];
	return value;
}

/********************************************************/
/* �֐�����		: get_ui_16  							*/
/* �@�\�̐���	: Byte�񂩂畄���Ȃ�16�r�b�g�����l�擾	*/
/* ����			: (I)data : �擾�Ώ�Byte��				*/
/* �߂�l		: �����Ȃ�16�r�b�g�����l				*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/05								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL UI_16 get_ui_16(const UI_8 *data)
{
	UI_16 value = ((UI_16)data[1] << 8) + (UI_16)data[0];
	return value;
}

/********************************************************/
/* �֐�����		: get_ui_8  							*/
/* �@�\�̐���	: Byte�񂩂畄���Ȃ�8�r�b�g�����l�擾	*/
/* ����			: (I)data : �擾�Ώ�Byte��				*/
/* �߂�l		: �����Ȃ�8�r�b�g�����l					*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/09/05								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL UI_8  get_ui_8(const UI_8 *data)
{
	UI_8 value = data[0];
	return value;
}


/********************************************************/
/* �֐�����		: gnss_comm_mode_proc					*/
/* �@�\�̐���	: GNSS�ʐM���[�h����					*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL void gnss_comm_mode_proc(void)
{
	UI_16	data_len;

	/* GNSS���M�\�f�[�^���擾	*/
	data_len = Gnss_uart_drv_get_can_set_tx_data_len(GNSS_UART_CH);
	if (data_len > 0) {						/* GNSS���M�\(���M�o�b�t�@�ɋ󂫂���)	*/

		/* ADVANCE UART�f�[�^�擾	*/
		if (data_len > GNSS_COMM_MODE_DATA_BUF_SIZE) {
			/* �f�[�^�T�C�Y���f�[�^�i�[�o�b�t�@�T�C�Y�ȉ��ɂ���	*/
			data_len = GNSS_COMM_MODE_DATA_BUF_SIZE;
		}

		/* ADVANCE UART�f�[�^�擾	*/
		data_len = Advance_control_getUartData(S_gnss_comm_mode_data, data_len);
		if (data_len > 0) {					/* ADVANCE UART�f�[�^����				*/

			/* �擾�����f�[�^��GNSS�ɑ��M����	*/
			(void)Gnss_uart_drv_Set_tx_data(GNSS_UART_CH, S_gnss_comm_mode_data, (UI_8)data_len);
		}
	}

	return;
}

/********************************************************/
/* �֐�����		: gnss_comm_mode_is_need_brate_change	*/
/* �@�\�̐���	: GNSS�ʐM���[�h�̃{�[���[�g�ύX���o	*/
/* ����			: ( /O)brate	�ύX����{�[���[�g		*/
/* �߂�l		: N_TRUE	�{�[���[�g�ύX�K�v			*/
/*				: N_FALSE	�{�[���[�g�ύX�s�v			*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/03/01								*/
/* ���l			: 										*/
/********************************************************/
GNSS_CTRL_LOCAL B_8 gnss_comm_mode_is_need_brate_change(E_GNSS_CTRL_BAUDRATE *brate)
{
	if (brate == NULL) {
		return N_FALSE;
	}

	/* SW�؂�ւ��ɂ��{�[���[�g�ύX���`�F�b�N */
	if (Sw_control_GetSwMode() == SW_MODE_MANUAL) {
		/* SW���}�j���A��REC�̏ꍇ */
		if (S_gnss_baudrate != GNSS_CTRL_BAUDRATE_9600) {
			OUTLOG(OutputLog("[GNSS]brate change 9600\n");)
			/* �{�[���[�g��9600bps�ł͂Ȃ��ꍇ9600bps�ɕύX���� */
			*brate = GNSS_CTRL_BAUDRATE_9600;
			return N_TRUE;
		}
	}
	else {
		/* SW���}�j���A��REC�ȊO�̏ꍇ */
		if (S_gnss_baudrate != GNSS_CTRL_BAUDRATE_115200) {
			OUTLOG(OutputLog("[GNSS]brate change 115200\n");)
			/* �{�[���[�g��115200bps�ł͂Ȃ��ꍇ115200bps�ɕύX���� */
			*brate = GNSS_CTRL_BAUDRATE_115200;
			return N_TRUE;
		}
	}

	return N_FALSE;
}
