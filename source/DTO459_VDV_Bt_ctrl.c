/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: FR81S								*/
/* Date				: 16/09/02							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Kamakura						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		:									*/
/* date				:									*/
/********************************************************/
/********************************************************/
/* file DTO459_VDV_Bt_ctrl.c							*/
/* BT����												*/
/********************************************************/

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include "DTO459_VDV_Bt_ctrl.h"
#include "DTO459_VDV_Sw_ctrl.h"
#include "DTO459_VDV_Eeprom_ctrl.h"
#include "System_control_TASK.h"
#include "DTO459_VDV_bt_module_control_TASK.h"
#include "DTO459_VDV_Signal_ctrl.h"
#include "DTO459_VDV_Common.h"
#include "DTO459_VDV_Advance_ctrl.h"
#include "timer_ctrl.h"
#include "dbglog.h"
#include <string.h>

/********************************************************/
/*	�f�o�b�O��`[DEBUG]									*/
/********************************************************/

/********************************************************/
/*	���샂�[�h:�v�����ꂽID�����������ǂ����`�F�b�N	*/
/********************************************************/
/* #define D_BTC_CHECK_REQ_ID */

#define BTC_LOCAL	static

/********************************************************/
/*	�萔��`											*/
/********************************************************/
#define D_BTC_MAX_SIGNAL_COUNT			(50U)			/*	�ő�M����				*/
#define D_BTC_MAX_ERRCODE_COUNT			(10U)			/*	�ő�G���[�R�[�h��		*/

/*	VDV BT�ʐM�f�[�^�t�H�[�}�b�g	*/
#define BTC_FORMAT_VERSION				(0x00010000)	/*	�t�H�[�}�b�g�o�[�W����	*/
/*											 ^^^^^^
											  | | |
											  | | |
											  | | �����e�i�X�o�[�W����
											  | �}�C�i�[�o�[�W����
											  ���W���[�o�[�W����
*/

/* �w�b�_ */
/* START	1byte */
#define D_BTC_FMT_HEADER_START			(0x7EU)			/*	START					*/
/* LEN		2byte */
/* TYPE		1byte */
#define D_BTC_FMT_HEADER_BYTES			(4U)			/*	�w�b�_�T�C�Y			*/

/* TYPE: �f�[�^��� */
#define D_BTC_FMT_TYPE_INVALID			(0xFFU)			/*	�����ȃf�[�^���				*/
#define D_BTC_FMT_TYPE_REQ_LIST			(0x00U)			/*	�ԗ��M���ꗗ�擾(���X�g)�v��	*/
#define D_BTC_FMT_TYPE_REQ_START		(0x01U)			/*	�ԗ����(�擾�J�n)�v��			*/
#define D_BTC_FMT_TYPE_REQ_STOP			(0x02U)			/*	�ԗ����(�擾��~)�v��			*/
#define D_BTC_FMT_TYPE_REP_LIST			(0x10U)			/*	�ԗ��M���ꗗ�擾(���X�g)����	*/
#define D_BTC_FMT_TYPE_REP_START		(0x11U)			/*	�ԗ����(�擾�J�n)����			*/
#define D_BTC_FMT_TYPE_REP_STOP			(0x12U)			/*	�ԗ����(�擾��~)����			*/

/* �t�b�^ */
/* CRC		2byte */
/* END		1byte */
#define D_BTC_FMT_FOOTER_END			(0x7FU)			/*	END						*/
#define D_BTC_FMT_FOOTER_BYTES			(3U)			/*	�t�b�^�T�C�Y			*/

#define D_BTC_FMT_BYTES_FIX				(D_BTC_FMT_HEADER_BYTES + D_BTC_FMT_FOOTER_BYTES)

/* �v���f�[�^�T�C�Y�v�Z�� */
#define D_BTC_FMT_BYTES_REQ_LIST		(D_BTC_FMT_HEADER_BYTES + D_BTC_FMT_FOOTER_BYTES)
														/*	�ԗ��M���ꗗ�擾(���X�g)�v��	*/
#define D_BTC_FMT_MAX_BYTES_REQ_START	(D_BTC_FMT_HEADER_BYTES + (8 + (2 * D_BTC_MAX_SIGNAL_COUNT)) + D_BTC_FMT_FOOTER_BYTES)
														/*	�ԗ����(�擾�J�n)�v��(�ő�T�C�Y)	*/
#define D_BTC_FMT_BYTES_REQ_STOP		(D_BTC_FMT_HEADER_BYTES + D_BTC_FMT_FOOTER_BYTES)
														/*	�ԗ����(�擾��~)�v��			*/
/* �����f�[�^�T�C�Y�v�Z�� */
#define D_BTC_FMT_MAX_BYTES_REP_START	(D_BTC_FMT_HEADER_BYTES + (6 + (6 * D_BTC_MAX_SIGNAL_COUNT)+ (6 * D_BTC_MAX_ERRCODE_COUNT)) + D_BTC_FMT_FOOTER_BYTES)
														/*	�ԗ����(�擾�J�n)����(�ő�T�C�Y)	*/
#define D_BTC_FMT_BYTES_REP_STOP		(D_BTC_FMT_HEADER_BYTES + D_BTC_FMT_FOOTER_BYTES)
														/*	�ԗ����(�擾��~)����			*/

#define D_BTC_MAX_SEND_BYTES			(((D_BTC_FMT_MAX_BYTES_REP_START + 15) / 16) * 16)	/* ���M�f�[�^�ő�T�C�Y(16byte�A���C�����g����)	*/
#define D_BTC_MAX_RECV_BYTES			(((D_BTC_FMT_MAX_BYTES_REQ_START + 15) / 16) * 16)	/* ��M�f�[�^�ő�T�C�Y(16byte�A���C�����g����)	*/

/* ����M�o�b�t�@�T�C�Y */
#define D_BTC_SEND_BUFF_BYTES			(D_BTC_MAX_SEND_BYTES)
														/*	���M�o�b�t�@�T�C�Y		*/
#define D_BTC_RECV_BUFF_BYTES			(D_BTC_MAX_RECV_BYTES * 2)		
														/*	��M�o�b�t�@�T�C�Y		*/

/* ���� */
#define D_BTC_TIMEOUT_MSEC_PAIRING		((60U * 1000U) / 10U)	/*	�y�A�����O�^�C���A�E�g����(1�J�E���g10msec)			*/
#define D_BTC_TIMEOUT_MSEC_STARTUP		((5U * 1000U) / 10U)	/*	BT���W���[���N���^�C���A�E�g����(1�J�E���g10msec)	*/
#define D_BTC_TIMEOUT_MSEC_RECV_PACKET	((1U * 1000U) / 10U)	/*	��M�p�P�b�g���o�^�C���A�E�g����(1�J�E���g10msec)	*/
#define D_BTC_CYCLE_MSEC_SEND_REP_START	(50U / 10U)				/*	�ԗ��M�����M�Ԋu(1�J�E���g10msec)					*/

/* BT����C�x���g */
#define BTC_EVENT_NONE					(0L)
#define BTC_EVENT_ACC_ON				(1UL << 16)		/*	ACC ON								*/
#define BTC_EVENT_ACC_OFF				(1UL << 17)		/*	ACC OFF								*/
#define BTC_EVENT_REQ_STARTUP			(1UL << 1)		/*	BT���W���[���N���v��				*/
#define BTC_EVENT_REQ_SHUTDOWN			(1UL << 2)		/*	BT���W���[����~�v��				*/
#define BTC_EVENT_REQ_PAIRING			(1UL << 3)		/*	�y�A�����O�J�n�v��					*/
#define BTC_EVENT_PAIRING				(1UL << 4)		/*	�y�A�����O����						*/
#define BTC_EVENT_PAIRING_TIMEOUT		(1UL << 5)		/*	�y�A�����O�^�C���A�E�g				*/
#define BTC_EVENT_STARTUP				(1UL << 6)		/*	BT���W���[���N������				*/
#define BTC_EVENT_SHUTDOWN				(1UL << 7)		/*	BT���W���[����~����				*/
#define BTC_EVENT_PEER_CONNECTED		(1UL << 8)		/*	Peer�ڑ�							*/
#define BTC_EVENT_PEER_DISCONNECTED		(1UL << 9)		/*	Peer�ؒf							*/
#define BTC_EVENT_REQ_SIGNAL_LIST		(1UL << 10)		/*	�ԗ��M���ꗗ�擾(���X�g)�v��		*/
#define BTC_EVENT_REQ_START_SEND		(1UL << 11)		/*	�ԗ����(�擾�J�n)�v��				*/
#define BTC_EVENT_REQ_STOP_SEND			(1UL << 12)		/*	�ԗ����(�擾��~)�v��				*/
#define BTC_EVENT_SEND_TRIGGER			(1UL << 13)		/*	�ԗ��M�����M�g���K					*/
#define BTC_EVENT_STARTUP_TIMEOUT		(1UL << 14)		/*	BT���W���[���N���^�C���A�E�g		*/
#define BTC_EVENT_RECV_PACKET_TIMEOUT	(1UL << 15)		/*	�p�P�b�g��M�^�C���A�E�g			*/
#define BTC_EVENT_ERROR					(1UL << 18)		/*	BT���W���[���ُ팟�m				*/
#define BTC_EVENT_REQ_UART_SEND			(1UL << 19)		/*	UART�f�[�^���M						*/
#define BTC_EVENT_REQ_RETURN_SEND		(1UL << 20)		/*	�������[�h�f�[�^���M�i��M�f�[�^�����̂܂ܕԐM�j*/

/* BT������ */
typedef enum e_btc_status {
	BTC_STATUS_NONE = 0L,
	BTC_STATUS_ACC_OFF					,				/*	ACC OFF								*/
	BTC_STATUS_WAIT_STARTUP_ACC_ON		,				/*	ACC ON�ɂ��BT���W���[���N���҂�	*/
	BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF	,				/*	ACC OFF�ɂ��BT���W���[����~�҂�	*/
	BTC_STATUS_SHUTDOWN					,				/*	BT���W���[����~��					*/
	BTC_STATUS_WAIT_STARTUP				,				/*	BT���W���[���N���҂�				*/
	BTC_STATUS_STARTUP					,				/*	BT���W���[���N����					*/
	BTC_STATUS_WAIT_SHUTDOWN			,				/*	BT���W���[����~�҂�				*/
	BTC_STATUS_PEEE_CONNECTED_IDLE		,				/*	BT�ʐM��: �f�[�^�����M				*/
	BTC_STATUS_PEEE_CONNECTED_SENDING	,				/*	BT�ʐM��: �f�[�^���M��				*/
	BTC_STATUS_WAIT_STARTUP_PAIRING		,				/*	�y�A�����O�ڍs��					*/
	BTC_STATUS_PAIRING					,				/*	�y�A�����O							*/
	BTC_STATUS_ERROR					,				/*	�ُ픭��							*/
	/* ����Ԃ͂�������ɒǉ����邱�� */
	BTC_STATUS_COUNT									/*	��Ԓ�`��							*/
} E_BTC_STATUS;

/* ������� */
typedef enum e_btc_time_kind {
	BTC_TIME_KIND_PACKET_FOUND = 0		, 				/*	�p�P�b�g�擪�𔭌������ŏI����	*/
	BTC_TIME_KIND_SEND_REP_START		,				/*	�ԗ��M���p�P�b�g���M�����ŏI����*/
	BTC_TIME_KIND_PAIRING_START			,				/*	�y�A�����O���J�n��������		*/
	BTC_TIME_KIND_STARTUP				,				/*	BT���W���[���N���v������		*/
	/* ����ʂ͂�������ɒǉ����邱�� */
	BTC_TIME_KIND_COUNT
} E_BTC_TIME_KIND;

/* BT���W���[���N���v�� */
typedef enum e_btc_startup_factor {
	BTC_STARTUP_FACTOR_NONE = 0			,				/*	�v���Ȃ�						*/
	BTC_STARTUP_FACTOR_ACC_ON			,				/*	ACC ON �ɂ��N��				*/
	BTC_STARTUP_FACTOR_PAIRING			,				/*	�y�A�����O�J�n�v���ɂ��N��	*/
	BTC_STARTUP_FACTOR_OTHERS							/*	���̑��v���ɂ���~			*/
} E_BTC_STARTUP_FACTOR;

/* BT���W���[����~�v�� */
typedef enum e_btc_shutdown_factor {
	BTC_SHUTDOWN_FACTOR_NONE = 0		,				/*	�v���Ȃ�						*/
	BTC_SHUTDOWN_FACTOR_ACC_OFF			,				/*	ACC OFF �ɂ���~				*/
	BTC_SHUTDOWN_FACTOR_OTHERS							/*	���̑��v���ɂ���~			*/
} E_BTC_SHUTDOWN_FACTOR;

/********************************************************/
/*	�^��`												*/
/********************************************************/
/* �����O�o�b�t�@ */
typedef struct btc_ringbuff {
	UI_8*		pu8_buff;						/*  �o�b�t�@�A�h���X			*/
	UI_16		u16_buff_bytes;					/*  �o�b�t�@�S�̃T�C�Y			*/
	UI_16		u16_w_index;					/*	�������݈ʒu				*/
	UI_16		u16_r_index;					/*	�ǂݏo���ʒu				*/
	UI_16		u16_w_total;					/*	�������݃o�C�g��(�ʎZ)		*/
	UI_16		u16_r_total;					/*	�ǂݏo���o�C�g��(�ʎZ)		*/
	UI_8		rsv[2];
} BTC_RINGBUFF;

/* ���ԏ�� */
typedef struct btc_time_info {
	UI_16		time[BTC_TIME_KIND_COUNT];		/*	��ʖ��̎���(1�J�E���g10msec)	*/
	UI_32		flags;							/*	�����̗L���������Ǘ�		*/
} BTC_TIME_INFO;

/* �p�P�b�g��� */
typedef struct {
	UI_16		packet_bytes;					/*	�p�P�b�g�S�̂̒��� 7E=>7F	*/
	UI_16		length;							/*	�w�b�_���璊�o���� Length	*/
	UI_8		type;							/*	�w�b�_���璊�o���� Type		*/
	UI_8		rsv[3];
	UI_8*		header_part;					/*	�o�b�t�@���̃w�b�_�擪		*/
	UI_8*		data_part;						/*	�o�b�t�@���̃f�[�^�擪		*/
	UI_8*		footer_part;					/*	�o�b�t�@���̃t�b�^�擪		*/
	UI_8		packet[D_BTC_MAX_RECV_BYTES];	/*	�p�P�b�g�o�b�t�@			*/
} BTC_PACKET_INFO;

/* �C�x���g��� */
typedef UI_32	BTC_EVENT_FLG;
typedef struct {
	BTC_EVENT_FLG			event_flg;			/* �C�x���g�t���O				*/
	E_BTC_STARTUP_FACTOR	startup_factor;		/* BT���W���[���N���v��			*/
	E_BTC_SHUTDOWN_FACTOR	shutdown_factor;	/* BT���W���[����~�v��			*/
} BTC_EVENT_INFO;

/* ��Ԋ֐� */
typedef E_BTC_STATUS (*PFN_BTC_STATE_PROC)( const BTC_EVENT_INFO* event_info );

/********************************************************/
/*	�萔�e�[�u��										*/
/********************************************************/
/*	�O����Ԃւ̃}�b�v�e�[�u��	*/
BTC_LOCAL const E_BT_CONTROL_STAT S_state_map[] = {
	BT_CONTROL_STAT_UNKNOWN			,
	BT_CONTROL_STAT_SHUTDOWN		,	/*	ACC OFF								*/
	BT_CONTROL_STAT_STARTUP			,	/*	ACC ON�ɂ��BT���W���[���N���҂�	*/
	BT_CONTROL_STAT_STARTUP			,	/*	ACC OFF�ɂ��BT���W���[����~�҂�	*/
	BT_CONTROL_STAT_SHUTDOWN		,	/*	BT���W���[����~��		*/
	BT_CONTROL_STAT_STARTUP			,	/*	BT���W���[���N���҂�	*/
	BT_CONTROL_STAT_STARTUP			,	/*	BT���W���[���N����		*/
	BT_CONTROL_STAT_STARTUP			,	/*	BT���W���[����~�҂�	*/
	BT_CONTROL_STAT_COMM_ILDE		,	/*	BT�ʐM��: �f�[�^�����M	*/
	BT_CONTROL_STAT_COMM_SENDING	,	/*	BT�ʐM��: �f�[�^���M��	*/
	BT_CONTROL_STAT_PAIRING			,	/*	�y�A�����O�ڍs��		*/
	BT_CONTROL_STAT_PAIRING			,	/*	�y�A�����O				*/
	BT_CONTROL_STAT_ERROR			,	/*	�ُ픭��				*/
};
/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
/* �o�b�t�@ */
BTC_LOCAL	BTC_RINGBUFF	S_btc_recv_ring;			/*	�~�h�������M�����f�[�^��ێ����郊���O�o�b�t�@	*/
BTC_LOCAL	UI_8			S_btc_recv_data_buff[D_BTC_RECV_BUFF_BYTES];
														/*	��M�f�[�^�o�b�t�@�{��	*/
BTC_LOCAL	UI_8			S_btc_send_data_buff[D_BTC_SEND_BUFF_BYTES];
														/*	���M�f�[�^�쐬�p�o�b�t�@	*/
/* �p�P�b�g��� */
BTC_LOCAL	BTC_PACKET_INFO	S_btc_packet_info;

/* ��Ԑ��� */
BTC_LOCAL	E_BTC_STATUS	S_btc_status;					/*	BT������			*/
BTC_LOCAL	E_BTC_STARTUP_FACTOR	S_btc_startup_factor;	/* BT���W���[���N���v�� */

/* ���M�M�� */
BTC_LOCAL	SI_32			S_btc_encode_key;			/*	�Í����L�[		*/
BTC_LOCAL	UI_8			S_btc_send_signal_id_num;	/*	���M�M��ID�̐�	*/
BTC_LOCAL	UI_16			S_btc_send_signal_id_array[D_BTC_MAX_SIGNAL_COUNT];
														/*	���M�M��ID�z��(���M�J�n�v���Ŏw�肳�ꂽID���ێ�)	*/

/* ������� */
BTC_LOCAL	BTC_TIME_INFO	S_btc_time_info;			/*	�e�펞�����					*/

BTC_LOCAL	UI_8			S_btc_req_Paring_Inspection;		/*	�������[�h�y�A�����O�v��	*/
BTC_LOCAL	UI_8			S_btc_req_Communication_Inspection;	/*	�������[�h�ʐM�v��			*/

/********************************************************/
/*	�O���֐�											*/
/********************************************************/

/********************************************************/
/*	�����֐��錾										*/
/********************************************************/
/* �����O�o�b�t�@����	*/
BTC_LOCAL E_RC	btc_ring_Init( BTC_RINGBUFF* ringbuff, UI_16 bytes, UI_8* buff );
BTC_LOCAL E_RC	btc_ring_Empty( BTC_RINGBUFF* ringbuff );
BTC_LOCAL UI_16 btc_ring_GetDataBytes( const BTC_RINGBUFF* ringbuff );
BTC_LOCAL UI_16	btc_ring_GetFreeBytes( const BTC_RINGBUFF* ringbuff );
BTC_LOCAL E_RC	btc_ring_Ref( const BTC_RINGBUFF* ringbuff, UI_16 ref_bytes, UI_8* buff );
BTC_LOCAL E_RC	btc_ring_Read( BTC_RINGBUFF* ringbuff, UI_16 read_bytes, UI_8* buff );
BTC_LOCAL E_RC	btc_ring_Write( BTC_RINGBUFF* ringbuff, UI_16 write_bytes, const UI_8* data );

/* �C�x���g����֐�	*/
BTC_LOCAL void btc_state_MakeEvent( BTC_EVENT_INFO* event_info );

/* ��Ԋ֐�	*/
BTC_LOCAL E_BTC_STATUS btc_state_ProcNone( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcAccOff( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitStartupAccOn( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitShutdownAccOff( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcShutdown( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitStartup( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcStartup( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitShutdown( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcPeerConnectedIdle( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcPeerConnectedSending( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitStartupPairing( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcPairing( const BTC_EVENT_INFO* event_info );
BTC_LOCAL E_BTC_STATUS btc_state_ProcError( const BTC_EVENT_INFO* event_info );

/* ��ԑJ�ڎ��֐� */
BTC_LOCAL void btc_state_ProcOnEnter( E_BTC_STATUS status );
BTC_LOCAL void btc_state_ProcOnLeave( E_BTC_STATUS status );

/* ��ԏ������葱���֐� */
BTC_LOCAL E_RC btc_proc_BtMdlStartup( E_BTC_STARTUP_FACTOR startup_factor );
BTC_LOCAL E_RC btc_proc_BtMdlShutdown( E_BTC_SHUTDOWN_FACTOR shutdown_factor );
BTC_LOCAL E_RC btc_proc_SendData( UI_8 data_type );
BTC_LOCAL E_RC btc_proc_UartDataSend(void);
BTC_LOCAL E_RC btc_proc_ReturnSend(void);

/* ��Ԋ֐��e�[�u��	*/
BTC_LOCAL const PFN_BTC_STATE_PROC S_state_proc[] = {
	btc_state_ProcNone,
	btc_state_ProcAccOff				,	/*	ACC OFF					*/
	btc_state_ProcWaitStartupAccOn		,	/*	ACC ON �N���҂�			*/
	btc_state_ProcWaitShutdownAccOff	,	/*	ACC OFF ��~�҂�		*/
	btc_state_ProcShutdown				,	/*	BT���W���[����~��		*/
	btc_state_ProcWaitStartup			,	/*	BT���W���[���N���҂�	*/
	btc_state_ProcStartup				,	/*	BT���W���[���N����		*/
	btc_state_ProcWaitShutdown			,	/*	BT���W���[����~�҂�	*/
	btc_state_ProcPeerConnectedIdle		,	/*	BT�ʐM��: �f�[�^�����M	*/
	btc_state_ProcPeerConnectedSending	,	/*	BT�ʐM��: �f�[�^���M��	*/
	btc_state_ProcWaitStartupPairing	,	/*	�y�A�����O�ڍs��		*/
	btc_state_ProcPairing				,	/*	�y�A�����O				*/
	btc_state_ProcError					,	/*	�G���[					*/
};
/* ���M�f�[�^�쐬�֐� */
BTC_LOCAL E_RC btc_data_MakeRepList( UI_8* buff, UI_16 buff_bytes, UI_16* data_bytes );
BTC_LOCAL E_RC btc_data_MakeRepStart( UI_8* buff, UI_16 buff_bytes, UI_16* data_bytes );
BTC_LOCAL E_RC btc_data_MakeRepStop( UI_8* buff, UI_16 buff_bytes, UI_16* data_bytes );
BTC_LOCAL UI_16 btc_data_GetBytesRepList( UI_8 fixed_signal_num, UI_8 vehicle_signal_num );
BTC_LOCAL UI_16 btc_data_GetBytesRepStart( UI_8 signal_num, UI_8 errcode_num );

/* ��M�f�[�^��͊֐� */
BTC_LOCAL E_RC btc_data_AnalyzeRecvData( BTC_PACKET_INFO* packet_info );
BTC_LOCAL SI_32 btc_data_SkipToSTART( void );
BTC_LOCAL SI_32 btc_data_GetPacketInfo( BTC_PACKET_INFO* packet_info );
BTC_LOCAL SI_32 btc_data_CheckDataPartReqStart( const UI_8* data_part, UI_16 length );
BTC_LOCAL SI_32 btc_data_GetSendSignalInfo( const BTC_PACKET_INFO* packet_info );
BTC_LOCAL SI_32 btc_data_ClearRecvPacket( BTC_PACKET_INFO* packet_info );

/* ���Ԋ֘A */
BTC_LOCAL void	btc_time_RegTime( E_BTC_TIME_KIND kind );
BTC_LOCAL void	btc_time_ClearTime( E_BTC_TIME_KIND kind );
BTC_LOCAL SI_32	btc_time_CheckTimeout( E_BTC_TIME_KIND kind, UI_16 timeout_10msec );
BTC_LOCAL SI_32	btc_time_CheckTimerStartup( E_BTC_TIME_KIND kind );

/* �R�[���o�b�N�֐� */
BTC_LOCAL void btc_cb_mdlReceiveData( const UI_8 *buf, UI_16 len );

/*==========================================================================*/
/* �P�̃e�X�g�p��`															*/
/*==========================================================================*/

/*==========================================================================*/
/* �O���񋟊֐�																*/
/*==========================================================================*/
/********************************************************/
/* �֐�����		: Bt_control_Init						*/
/* �@�\�̐���	: BT���䏉����							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Bt_control_Init(void)
{
	/* BT�ʐM�~�h�������� */
	Bt_comm_mdl_Init();
	/* ��M�R�[���o�b�N�o�^ */
	Bt_comm_mdl_RegisterCbReceiveData( btc_cb_mdlReceiveData );

	/* �ϐ������� */
	S_btc_status = BTC_STATUS_NONE;
	S_btc_send_signal_id_num = 0U;
	S_btc_encode_key = 0UL;

	/* �o�b�t�@�N���A */
	(void)memset( S_btc_recv_data_buff, 0, sizeof(S_btc_recv_data_buff) );
	(void)memset( S_btc_send_data_buff, 0, sizeof(S_btc_send_data_buff) );
	(void)memset( S_btc_send_signal_id_array, 0, sizeof(S_btc_send_signal_id_array) );

	/* ��M�����O�o�b�t�@������ */
	(void)btc_ring_Init( &S_btc_recv_ring, sizeof(S_btc_recv_data_buff), S_btc_recv_data_buff );

	/* BT���W���[���N���E��~�v���N���A */
	S_btc_startup_factor = BTC_STARTUP_FACTOR_NONE;
	S_btc_req_Paring_Inspection = 0;
	S_btc_req_Communication_Inspection = 0;

}

/********************************************************/
/* �֐�����		: Bt_control_Main						*/
/* �@�\�̐���	: BT���䃁�C��							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Bt_control_Main(void)
{
	PFN_BTC_STATE_PROC	state_proc;
	BTC_EVENT_INFO		event_info;
	E_BTC_STATUS		next_status;

	/* �~�h���̃��C���������Ăяo�� */
	Bt_comm_mdl_Main();

	if( BTC_STATUS_COUNT > S_btc_status ) {
		/* ��Ԋ֐��R�[�� */
		state_proc = S_state_proc[S_btc_status];
		if( NULL != state_proc ) {
			/* �C�x���g�쐬	*/
			btc_state_MakeEvent( &event_info );
			next_status = state_proc( &event_info );

			if( (BTC_STATUS_NONE != next_status) && (next_status != S_btc_status) ) {
				/* ��ԑJ�ڔ��� */
				btc_state_ProcOnLeave( S_btc_status );
				OUTLOG(OutputLog( "[BTC] STATUS %d->%d [EV:%08x(%d:%d)][%08d]\n", S_btc_status, next_status, event_info.event_flg, event_info.startup_factor, event_info.shutdown_factor, Timer_ctrl_GetTime1ms() );)
				S_btc_status = next_status;
				btc_state_ProcOnEnter( S_btc_status );
			}
		}
	}
}

/********************************************************/
/* �֐�����		: Bt_control_GetStatus					*/
/* �@�\�̐���	: BT��Ԏ擾							*/
/* ����			: void									*/
/* �߂�l		: BT��� E_BT_CONTROL_STAT				*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
E_BT_CONTROL_STAT Bt_control_GetStatus(void)
{
	E_BT_CONTROL_STAT	stat = BT_CONTROL_STAT_UNKNOWN;

	if( BTC_STATUS_COUNT > S_btc_status ) {
		stat = S_state_map[S_btc_status];
	}
	return stat;
}

/********************************************************/
/* �֐�����		:	Bt_control_getUartData				*/
/* �@�\�̐���	:	UART�f�[�^�擾						*/
/* ����			:	( /O)buf	�f�[�^�o�b�t�@			*/
/*				:	(I/ )len	�擾�f�[�^��(byte)		*/
/* �߂�l		:	�擾�ł����f�[�^��(byte)			*/
/* �쐬��		:	(OFT)Toguchi						*/
/* �쐬��		:	17/01/20							*/
/* ���l			:										*/
/********************************************************/
UI_16 Bt_control_getUartData(UI_8 *buf, UI_16 len)
{
	UI_16	data_bytes;

	if (buf == NULL) {
		return 0;
	}
	/*�����O�o�b�t�@�̃f�[�^�T�C�Y�擾*/
	data_bytes = btc_ring_GetDataBytes( &S_btc_recv_ring );
	if (data_bytes > 0){
		/*�f�[�^����*/
		if(data_bytes > len){
			/*�擾�f�[�^�����傫���T�C�Y�̃f�[�^������Ȃ�擾�T�C�Y��ύX*/
			data_bytes = len;
		}
		/*�����O�o�b�t�@���f�[�^�擾*/
		(void)btc_ring_Read( &S_btc_recv_ring, data_bytes, buf );
	}
	return data_bytes;
}

/*------------------------------------------------------*/
/*       �ȉ��̊֐��͌������[�h���̂ݎg�p����           */
/*------------------------------------------------------*/
/*������������������������������������������������������*/
/********************************************************/
/* �֐�����		:	Bt_control_getMdlState				*/
/* �@�\�̐���	:	BT��Ԏ擾							*/
/* ����			:	void								*/
/* �߂�l		: 	BT��� E_BT_COMM_MDL_STATE			*/
/* �쐬��		:	(OFT)Sakata							*/
/* �쐬��		:	17/02/15							*/
/* ���l			:	�������[�h���̂ݎg�p				*/
/********************************************************/
E_BT_COMM_MDL_STATE Bt_control_getMdlState( void )
{
	return Bt_comm_mdl_GetState();
}

/********************************************************/
/* �֐�����		:	Bt_control_getIapAuthInfo			*/
/* �@�\�̐���	:	iAP�ڑ����擾						*/
/* ����			:	(/O)iAP�ڑ����\����				*/
/* �߂�l		: 	N_OK		����					*/
/* 				: 	N_NG		�ُ�					*/
/* �쐬��		:	(OFT)Sakata							*/
/* �쐬��		:	17/02/15							*/
/* ���l			:	�������[�h���̂ݎg�p				*/
/********************************************************/
SI_8 Bt_control_getIapAuthInfo( const S_IAP_CONNECTON_INFO** info )
{
	E_BT_COMM_MDL_RESULT result;
	SI_8 ret = N_OK;
	const S_IAP_CONNECTON_INFO* w_info = NULL;

	if (info != NULL) {
		result = Bt_comm_mdl_GetIapAuthInfo( &w_info );
		*info = w_info;
		if(result != BT_COMM_MDL_RESULT_OK)
		{
			ret = N_NG;
		}
	}
	else {
		ret = N_NG;
	}
	return ret;
}

/********************************************************/
/* �֐�����		:	Bt_control_reqPairing				*/
/* �@�\�̐���	:	�y�A�����O�����v��					*/
/* ����			: 	void								*/
/* �߂�l		: 	N_OK		����					*/
/* 				: 	N_NG		�ُ�					*/
/* �쐬��		:	(OFT)Sakata							*/
/* �쐬��		:	17/02/15							*/
/* ���l			:	�������[�h���̂ݎg�p				*/
/********************************************************/
SI_8 Bt_control_reqPairingInspection(void)
{
	/*�y�A�����O�v������ݒ�*/
	S_btc_req_Paring_Inspection = 1;
	return N_OK;
}

/********************************************************/
/* �֐�����		:	Bt_control_reqCommunicationInspection*/
/* �@�\�̐���	:	�ʐM�����v��						*/
/* ����			: 	void								*/
/* �߂�l		: 	N_OK		����					*/
/* 				: 	N_NG		�ُ�					*/
/* �쐬��		:	(OFT)Sakata							*/
/* �쐬��		:	17/02/15							*/
/* ���l			:	�������[�h���̂ݎg�p				*/
/********************************************************/
SI_8 Bt_control_reqCommunicationInspection(void)
{
	/* ��M�o�b�t�@����ɂ��� */
	(void)btc_ring_Empty( &S_btc_recv_ring );
	/*�ʐM�v������ݒ�*/
	S_btc_req_Communication_Inspection = 1;
	return N_OK;
}
/*������������������������������������������������������*/
/*------------------------------------------------------*/
/*       �ȏ�̊֐��͌������[�h���̂ݎg�p����           */
/*------------------------------------------------------*/

/*==========================================================================*/
/* �����O�o�b�t�@����֐�													*/
/*==========================================================================*/
/****************************************************************************/
/* �֐�����		: btc_ring_Init												*/
/* �@�\�̐���	: �����O�o�b�t�@�̏�����									*/
/* ����			: ringbuff	�����O�o�b�t�@									*/
/* ����			: bytes	�o�b�t�@�T�C�Y										*/
/* ����			: buff		���蓖�Ă�o�b�t�@								*/
/* �߂�l		: �������� E_RC												*/
/* �쐬��		: (OFT)Kamakura												*/
/* �쐬��		: 16/09/02													*/
/* ���l			: 															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_ring_Init( BTC_RINGBUFF* ringbuff, UI_16 bytes, UI_8* buff )
{
	E_RC	rc = E_RC_NG;

	if( (NULL == ringbuff) || (NULL == buff) || (0U == bytes) ) {
		rc = E_RC_NG_PARAM; 
	}
	else {
		(void)memset( ringbuff, 0, sizeof(BTC_RINGBUFF) );
		ringbuff->pu8_buff = buff;
		ringbuff->u16_buff_bytes = bytes;
		rc = E_RC_OK;
	}
	return rc;
}
/****************************************************************************/
/* �֐�����		: btc_ring_Empty											*/
/* �@�\�̐���	: �����O�o�b�t�@����ɂ���									*/
/* ����			: ringbuff	�����O�o�b�t�@									*/
/* �߂�l		: �������� E_RC												*/
/* �쐬��		: (OFT)Kamakura												*/
/* �쐬��		: 16/09/02													*/
/* ���l			: 															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_ring_Empty( BTC_RINGBUFF* ringbuff )
{
	E_RC	rc = E_RC_NG;
	UI_16	bytes;

	if( NULL == ringbuff ) {
		rc = E_RC_NG_PARAM;
	}
	else {
		/* �����Ă���T�C�Y���ǂݐi�߂� */
		bytes = btc_ring_GetDataBytes( ringbuff );
		if( 0U < bytes ) {
			/* �o�b�t�@�w��Ȃ��œǂݐi�߂̂ݍs�� */
			rc = btc_ring_Read( ringbuff, bytes, NULL );
		}
		else {
			rc = E_RC_OK;
		}
	}
	return rc;
}
/****************************************************************************/
/* �֐�����		: btc_ring_GetDataBytes										*/
/* �@�\�̐���	: �����O�o�b�t�@�f�[�^�T�C�Y�擾							*/
/* ����			: ringbuff	�����O�o�b�t�@									*/
/* �߂�l		: �������� �Q��(�Ǎ�)�\�f�[�^�T�C�Y						*/
/* �쐬��		: (OFT)Kamakura												*/
/* �쐬��		: 16/09/02													*/
/* ���l			: 															*/
/****************************************************************************/
BTC_LOCAL UI_16 btc_ring_GetDataBytes( const BTC_RINGBUFF* ringbuff )
{
	UI_16	bytes = 0U;

	if( NULL != ringbuff ) {
		bytes = ringbuff->u16_w_total - ringbuff->u16_r_total;
	}
	return bytes;
}
/****************************************************************************/
/* �֐�����		: btc_ring_GetFreeBytes										*/
/* �@�\�̐���	: �����O�o�b�t�@�󂫃T�C�Y�擾								*/
/* ����			: ringbuff	�����O�o�b�t�@									*/
/* �߂�l		: �������� �����\�f�[�^�T�C�Y								*/
/* �쐬��		: (OFT)Kamakura												*/
/* �쐬��		: 16/09/02													*/
/* ���l			: 															*/
/****************************************************************************/
BTC_LOCAL UI_16 btc_ring_GetFreeBytes( const BTC_RINGBUFF* ringbuff )
{
	UI_16	bytes = 0U;

	if( NULL != ringbuff ) {
		bytes = ringbuff->u16_buff_bytes - (ringbuff->u16_w_total - ringbuff->u16_r_total);
	}
	return bytes;
}
/****************************************************************************/
/* �֐�����		: btc_ring_Ref												*/
/* �@�\�̐���	: �����O�o�b�t�@����f�[�^�Q��								*/
/* ����			: ringbuff		�����O�o�b�t�@								*/
/* ����			: ref_bytes	�Q�ƃT�C�Y										*/
/* ����			: buff			�Q�ƃo�b�t�@(NULL�s��)						*/
/* �߂�l		: �������� E_RC												*/
/* �쐬��		: (OFT)Kamakura												*/
/* �쐬��		: 16/09/02													*/
/* ���l			: �{�֐��ŎQ�ƌ���ǂݏo���ʒu�͕s��						*/
/*				  �v���T�C�Y���Ȃ��Ƃ����G���[								*/
/****************************************************************************/
BTC_LOCAL E_RC btc_ring_Ref( const BTC_RINGBUFF* ringbuff, UI_16 ref_bytes, UI_8* buff )
{
	E_RC	rc = E_RC_NG;
	UI_16	data_bytes;
	UI_16	tmp_bytes;

	if( (NULL == ringbuff) || (NULL == buff) || (0U == ref_bytes) || (0U == ringbuff->u16_buff_bytes) ) {
		rc = E_RC_NG_PARAM; 
	}
	else {
		/* �f�[�^�c��o�C�g���擾 */
		data_bytes = btc_ring_GetDataBytes( ringbuff );
		if( ref_bytes > data_bytes ) {
			/* �w��T�C�Y���ǂݍ��߂Ȃ� */
			rc = E_RC_NG_PARAM;
		}
		else {
			if( (ringbuff->u16_r_index + ref_bytes) <= ringbuff->u16_buff_bytes ) {
				/* �I�[�܂ł܂Ƃߓǂ݉� */
				(void)memcpy( buff, ringbuff->pu8_buff + ringbuff->u16_r_index, ref_bytes );
			}
			else {
				/* �܂�Ԃ� */
				tmp_bytes = ringbuff->u16_buff_bytes - ringbuff->u16_r_index;
				(void)memcpy( buff, ringbuff->pu8_buff + ringbuff->u16_r_index, tmp_bytes );
				(void)memcpy( buff + tmp_bytes, ringbuff->pu8_buff, ref_bytes - tmp_bytes );
			}
			rc = E_RC_OK;
		}
	}
	return rc;
}
/****************************************************************************/
/* �֐�����		: btc_ring_Read												*/
/* �@�\�̐���	: �����O�o�b�t�@����f�[�^�ǂ݂���							*/
/* ����			: ringbuff		�����O�o�b�t�@								*/
/* ����			: read_bytes	�Ǎ��T�C�Y									*/
/* ����			: buff			�Ǎ��o�b�t�@(NULL��)						*/
/* �߂�l		: �������� E_RC												*/
/* �쐬��		: (OFT)Kamakura												*/
/* �쐬��		: 16/09/02													*/
/* ���l			: �{�֐��œǍ���ǂݏo���ʒu���X�V							*/
/*				  �v���T�C�Y���Ȃ��Ƃ����G���[								*/
/****************************************************************************/
BTC_LOCAL E_RC btc_ring_Read( BTC_RINGBUFF* ringbuff, UI_16 read_bytes, UI_8* buff )
{
	E_RC	rc = E_RC_NG;
	UI_16	data_bytes;

	if( (NULL == ringbuff) || (0U == read_bytes) || (0U == ringbuff->u16_buff_bytes) ) {
		rc = E_RC_NG_PARAM; 
	}
	else {
		/* �f�[�^�c��o�C�g���擾 */
		data_bytes = btc_ring_GetDataBytes( ringbuff );
		if( read_bytes > data_bytes ) {
			/* �w��T�C�Y���ǂݍ��߂Ȃ� */
			rc = E_RC_NG_PARAM;
		}
		else {
			if( NULL != buff ) {
				(void)btc_ring_Ref( ringbuff, read_bytes, buff );
			}
			/* �����O���X�V */
			ringbuff->u16_r_total += read_bytes;
			ringbuff->u16_r_index += read_bytes;
			if( ringbuff->u16_r_index >= ringbuff->u16_buff_bytes ) {
				ringbuff->u16_r_index -= ringbuff->u16_buff_bytes;
			}

			rc = E_RC_OK;	
		}
	}
	return rc;
}
/****************************************************************************/
/* �֐�����		: btc_ring_Write											*/
/* �@�\�̐���	: �����O�o�b�t�@�ւ̃f�[�^��������							*/
/* ����			: ringbuff		�����O�o�b�t�@								*/
/* ����			: write_bytes	�����T�C�Y									*/
/* ����			: data			�����f�[�^									*/
/* �߂�l		: �������� E_RC												*/
/* �쐬��		: (OFT)Kamakura												*/
/* �쐬��		: 16/09/02													*/
/* ���l			: �v���T�C�Y���������߂Ȃ��Ƃ��G���[						*/
/****************************************************************************/
BTC_LOCAL E_RC btc_ring_Write( BTC_RINGBUFF* ringbuff, UI_16 write_bytes, const UI_8* data )
{
	E_RC	rc = E_RC_NG;
	UI_16	free_bytes;
	UI_16	tmp_bytes;

	if( (NULL == ringbuff) || (0U == write_bytes) || (0U == ringbuff->u16_buff_bytes) ) {
		rc = E_RC_NG_PARAM; 
	}
	else {
		/* �󂫃o�C�g���擾 */
		free_bytes = btc_ring_GetFreeBytes( ringbuff );
		if( free_bytes < write_bytes ) {
			/* �w��T�C�Y���������߂Ȃ� */
			rc = E_RC_NG_PARAM;
		}
		else {
			if( (ringbuff->u16_w_index + write_bytes) <= ringbuff->u16_buff_bytes ) {
				/* �I�[�܂ł܂Ƃߏ����� */
				(void)memcpy( ringbuff->pu8_buff + ringbuff->u16_w_index, data, write_bytes );
			}
			else {
				/* �܂�Ԃ� */
				tmp_bytes = ringbuff->u16_buff_bytes - ringbuff->u16_w_index;
				(void)memcpy( ringbuff->pu8_buff + ringbuff->u16_w_index, data, tmp_bytes );
				(void)memcpy( ringbuff->pu8_buff, data + tmp_bytes, write_bytes - tmp_bytes );
			}
			/* �����O���X�V */
			ringbuff->u16_w_total += write_bytes;
			ringbuff->u16_w_index += write_bytes;
			if( ringbuff->u16_w_index >= ringbuff->u16_buff_bytes ) {
				ringbuff->u16_w_index -= ringbuff->u16_buff_bytes;
			}

			rc = E_RC_OK;
		}
	}
	return rc;
}
/*==========================================================================*/
/* �C�x���g����֐�															*/
/*==========================================================================*/
/****************************************************************************/
/* �֐�����		:	btc_state_MakeEvent										*/
/* �@�\�̐���	:	�C�x���g�쐬�i�C�x���g�t���O�`���ŕԂ��j				*/
/* ����			:	BTC_EVENT_INFO	�C�x���g���							*/
/* �߂�l		:	�Ȃ�													*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:	�C�x���g�͓����ɕ�����������B							*/
/****************************************************************************/
BTC_LOCAL void btc_state_MakeEvent( BTC_EVENT_INFO* event_info )
{
	UI_16						uartDataLen;
	UI_16						buffDataBytes;
	BTC_EVENT_FLG				event_flag = BTC_EVENT_NONE;
	E_SYS_CTRL_STATUS			sys_ctrl_status;
	E_SW_REC_BTN_STATUS			sw_rec_btn_status = SYS_REC_BTN_UNKNOWN;
	E_BT_COMM_MDL_STATE			bt_state;
	E_BT_COMM_MDL_PEER_STATUS	bt_peer_state;
	E_BT_COMM_MDL_PAIRED_STATUS	bt_paired_state;

	if( NULL == event_info ) {
		return;
	}
	(void)memset( event_info, 0, sizeof(BTC_EVENT_INFO) );

	/*======================================================================*/
	/* �C�x���g�̌��ɂȂ�e���Ԃ����W										*/
	/*======================================================================*/
	/* �V�X�e�������Ԏ擾 */
	sys_ctrl_status = System_control_GetStatus();
	
	if(	(sys_ctrl_status != SYS_OPERATION_BT_COMM) && 
		(sys_ctrl_status != SYS_OPERATION_INSPECTION) )
		/* BT���W���[���ʐM���[�h,�������[�h�ȊO�Ɏ�M�f�[�^��͂��s��*/
	{
		/* ��M�f�[�^��͂��ĂP�̃p�P�b�g���𓾂� */
		(void)btc_data_AnalyzeRecvData( &S_btc_packet_info );
	}

	/* REC�{�^����Ԏ擾 */
	sw_rec_btn_status = Sw_control_GetRecButtonStatus();

	/* BT�~�h����� */
	bt_state = Bt_comm_mdl_GetState();

	/* BT�~�h��Peer�ڑ���� */
	bt_peer_state = Bt_comm_mdl_GetPeerState();

	/* �y�A�����O��Ԏ擾 */
	bt_paired_state = Bt_comm_mdl_GetPairedStatus();

	uartDataLen = Advance_control_getUartDataLen();

	/* �����O�o�b�t�@�̃f�[�^�����擾 */
	buffDataBytes = btc_ring_GetDataBytes( &S_btc_recv_ring );

	/*======================================================================*/
	/* E01	BT���W���[���N���v��											*/
	/* E02	BT���W���[����~�v��											*/
	/*======================================================================*/
	/*
		�N���v��
			���[�h���ȉ��̂����ꂩ�ł���(or)
			�EACC ON
			�E�ʏ�:�I�[�gREC���[�h
			�E�ʏ�:�}�j���A��REC���[�h
			�E�V�~�����[�V����
		��~�v��
			���[�h���ȉ��̂����ꂩ�ł���(or)
			�E�ʏ�:OFF
			�E�ُ�
			�EEEPROM*
			�E�t�@�[���E�F�A����*
			�EACC OFF�ڍs
	*/
	switch( sys_ctrl_status ) {
	case SYS_OPERATION_WAIT_ACC_ON:			/**< ACC ON�҂�				*/
		/* �������Ȃ� */
		break;
	case SYS_OPERATION_ACC_ON_UNKNOWN_STAT:	/**< ACC ON�i��Ԕ���j		*/
		/* ACC ON */
		event_flag |= BTC_EVENT_ACC_ON;
		break;
	case SYS_OPERATION_NORMAL_AUTO:			/**< �ʏ�:�I�[�gREC			*/
	case SYS_OPERATION_NORMAL_MANUAL:		/**< �ʏ�:�}�j���A��REC		*/
	case SYS_OPERATION_SIMULATION:			/**< �V�~�����[�V����		*/
	case SYS_OPERATION_BT_COMM:				/**< BT���W���[���ʐM���[�h	*/
		/* ���̑��v���ɂ��N���v�� */
		event_flag |= BTC_EVENT_REQ_STARTUP;
		event_info->startup_factor = BTC_STARTUP_FACTOR_OTHERS;
		break;
	case SYS_OPERATION_INSPECTION:			/**< �������[�h				*/
		if( BTC_STARTUP_FACTOR_PAIRING != S_btc_startup_factor ) {
			event_flag |= BTC_EVENT_REQ_STARTUP;
			event_info->startup_factor = BTC_STARTUP_FACTOR_OTHERS;
		}
		break;
	case SYS_OPERATION_NORMAL_OFF:			/**< �ʏ�:OFF���[�h			*/
		/* OFF���[�h�ɂ���~�v�� */
		if( BTC_STARTUP_FACTOR_PAIRING != S_btc_startup_factor ) {
			event_flag |= BTC_EVENT_REQ_SHUTDOWN;
			event_info->shutdown_factor = BTC_SHUTDOWN_FACTOR_OTHERS;
		}
		break;
	case SYS_OPERATION_EEPROM_WRITE:		/**< EEPROM������			*/
	case SYS_OPERATION_EEPROM_WRITE_COMP:	/**< EEPROM��������			*/
	case SYS_OPERATION_FIRMWARE_WRITE:		/**< �t�@�[���E�F�A������	*/
	case SYS_OPERATION_FIRMWARE_WRITE_COMP:	/**< �t�@�[���E�F�A��������	*/
		/* ���̑��v���ɂ���~�v�� */
		event_flag |= BTC_EVENT_REQ_SHUTDOWN;
		event_info->shutdown_factor = BTC_SHUTDOWN_FACTOR_OTHERS;
		break;
	case SYS_OPERATION_PRE_ACC_OFF:			/**< ACC OFF�ڍs			*/
		/* ACC OFF */
		event_flag |= BTC_EVENT_ACC_OFF;
		break;
	case SYS_OPERATION_ACC_OFF:				/**< ACC OFF				*/
		/* �������Ȃ� */
		break;
	case SYS_OPERATION_ERROR:				/**< �ُ�					*/
		/* �������Ȃ� */
		break;
	default:
		/* �������Ȃ� */
		break;
	}
	/*======================================================================*/
	/* E03	�y�A�����O�J�n�v��												*/
	/*======================================================================*/
	/*
		OFF���[�h and REC�X�C�b�`ON��3�b�p��
	*/
	if( SYS_OPERATION_NORMAL_OFF == sys_ctrl_status ) {
		/* OFF ���[�h */
		if( SYS_REC_BTN_ON_LONGPRESS == sw_rec_btn_status ) {
			/* REC�X�C�b�`ON(������) */
			event_flag |= BTC_EVENT_REQ_PAIRING;
		}
	}
	else if( SYS_OPERATION_INSPECTION == sys_ctrl_status ) {
		/* �������[�h */
		if(S_btc_req_Paring_Inspection != 0) {
			/*�y�A�����O�v������*/
			event_flag |= BTC_EVENT_REQ_PAIRING;
		}
	}
	else {
		/* nop */
	}
	/*======================================================================*/
	/* E04	�y�A�����O����													*/
	/*======================================================================*/
	if( BTC_STATUS_PAIRING == S_btc_status ) {
		if( (BT_COMM_MDL_STATE_COMM_MODE == bt_state) && (BT_COMM_MDL_PAIRED_STATUS_EXIST == bt_paired_state) ) {
			if( SYS_OPERATION_INSPECTION != sys_ctrl_status ) {
				/* �������[�h�ȊO */
				event_flag |= BTC_EVENT_PAIRING;
			}
			else {
				/* �������[�h���̓X�^�[�g�A�b�v */
				event_flag |= BTC_EVENT_REQ_STARTUP;
			}
		}
	}
	/*======================================================================*/
	/* E05	�y�A�����O�^�C���A�E�g											*/
	/*======================================================================*/
	if( SYS_OPERATION_INSPECTION != sys_ctrl_status ) {
		/* �������[�h�ȊO */
		if( BTC_STATUS_PAIRING == S_btc_status ) {
			if( 1L == btc_time_CheckTimeout( BTC_TIME_KIND_PAIRING_START, D_BTC_TIMEOUT_MSEC_PAIRING ) ) {
				event_flag |= BTC_EVENT_PAIRING_TIMEOUT;
			}
		}
	}
	/*======================================================================*/
	/* E06	BT���W���[���N������											*/
	/*======================================================================*/
	if ((BTC_STATUS_WAIT_STARTUP == S_btc_status) || (BTC_STATUS_WAIT_STARTUP_ACC_ON == S_btc_status) || (BTC_STATUS_WAIT_STARTUP_PAIRING == S_btc_status)) {
		if( BT_COMM_MDL_STATE_COMM_MODE == bt_state ) {
			event_flag |= BTC_EVENT_STARTUP;
		}
	}
	/*======================================================================*/
	/* E07	BT���W���[����~����											*/
	/*======================================================================*/
	if ((BTC_STATUS_WAIT_SHUTDOWN == S_btc_status) || (BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF == S_btc_status)) {
		if( BT_COMM_MDL_STATE_IDLE == bt_state ) {
			event_flag |= BTC_EVENT_SHUTDOWN;
		}
	}
	/*======================================================================*/
	/* E08	Peer�ڑ�														*/
	/*======================================================================*/
	if( BTC_STATUS_STARTUP == S_btc_status ) {
		if( BT_COMM_MDL_PEER_STATUS_CONNECTED == bt_peer_state ) {
			event_flag |= BTC_EVENT_PEER_CONNECTED;
		}
	}
	/*======================================================================*/
	/* E09	Peer�ؒf														*/
	/*======================================================================*/
	if( (BTC_STATUS_PEEE_CONNECTED_IDLE == S_btc_status) || (BTC_STATUS_PEEE_CONNECTED_SENDING == S_btc_status) ) {
		if( BT_COMM_MDL_PEER_STATUS_DISCONNECTED == bt_peer_state ) {
			event_flag |= BTC_EVENT_PEER_DISCONNECTED;
		}
	}
	/*======================================================================*/
	/* E10	�ԗ��M���ꗗ�擾(���X�g)�v��									*/
	/*======================================================================*/
	if( (BTC_STATUS_PEEE_CONNECTED_IDLE == S_btc_status) || (BTC_STATUS_PEEE_CONNECTED_SENDING == S_btc_status) ) {
		if( D_BTC_FMT_TYPE_REQ_LIST == S_btc_packet_info.type ) {
			event_flag |= BTC_EVENT_REQ_SIGNAL_LIST;
		}
	}
	/*======================================================================*/
	/* E11	�ԗ����(�擾�J�n)�v��											*/
	/*======================================================================*/
	if( (BTC_STATUS_PEEE_CONNECTED_IDLE == S_btc_status) || (BTC_STATUS_PEEE_CONNECTED_SENDING == S_btc_status) ) {
		if( D_BTC_FMT_TYPE_REQ_START == S_btc_packet_info.type ) {
			event_flag |= BTC_EVENT_REQ_START_SEND;
		}
	}
	/*======================================================================*/
	/* E12	�ԗ����(�擾��~)�v��											*/
	/*======================================================================*/
	if( (BTC_STATUS_PEEE_CONNECTED_IDLE == S_btc_status) || (BTC_STATUS_PEEE_CONNECTED_SENDING == S_btc_status) ) {
		if( D_BTC_FMT_TYPE_REQ_STOP == S_btc_packet_info.type ) {
			event_flag |= BTC_EVENT_REQ_STOP_SEND;
		}
	}
	/*======================================================================*/
	/* E13	�ԗ��M�����M�g���K												*/
	/*======================================================================*/
	if( 1L == btc_time_CheckTimeout( BTC_TIME_KIND_SEND_REP_START, D_BTC_CYCLE_MSEC_SEND_REP_START ) ) {
		event_flag |= BTC_EVENT_SEND_TRIGGER;
	}
	/*======================================================================*/
	/* E14	BT���W���[���N���^�C���A�E�g									*/
	/*======================================================================*/
	if( 1L == btc_time_CheckTimeout( BTC_TIME_KIND_STARTUP, D_BTC_TIMEOUT_MSEC_STARTUP ) ) {
		event_flag |= BTC_EVENT_STARTUP_TIMEOUT;
	}
	/*======================================================================*/
	/* E15	�p�P�b�g���o�^�C���A�E�g										*/
	/*======================================================================*/
	if( 1L == btc_time_CheckTimeout( BTC_TIME_KIND_PACKET_FOUND, D_BTC_TIMEOUT_MSEC_RECV_PACKET ) ) {
		event_flag |= BTC_EVENT_RECV_PACKET_TIMEOUT;
	}
	/*======================================================================*/
	/* E16	BT���W���[���ُ팟�m											*/
	/*======================================================================*/
	if( BTC_STATUS_ERROR != S_btc_status ) {
		if( BT_COMM_MDL_STATE_ERROR == bt_state ) {
			event_flag |= BTC_EVENT_ERROR;
		}
	}
	/*======================================================================*/
	/* E17	UART���M�f�[�^�v��												*/
	/*======================================================================*/
	if(sys_ctrl_status == SYS_OPERATION_BT_COMM)
	{
		/**< BT���W���[���ʐM���[�h	*/
		if(uartDataLen > 0){
			/*UART�f�[�^����*/
			event_flag |= BTC_EVENT_REQ_UART_SEND;
		}
	}
	/*======================================================================*/
	/* E18	�������[�h�f�[�^���M											*/
	/*======================================================================*/
	if(sys_ctrl_status == SYS_OPERATION_INSPECTION) {
		/**< �������[�h	*/
		if (S_btc_req_Communication_Inspection != 0) {
			/*�ʐM�v������*/
			if(buffDataBytes > 0){
				/*��M�f�[�^����*/
				event_flag |= BTC_EVENT_REQ_RETURN_SEND;
			}
		}
	}
	event_info->event_flg = event_flag;
}
/*==========================================================================*/
/* ��Ԋ֐�																	*/
/*==========================================================================*/
/****************************************************************************/
/* �֐�����		:	btc_state_ProcNone										*/
/* �@�\�̐���	:	��Ԋ֐� BTC_STATUS_NONE								*/
/* ����			:	event_info	�C�x���g���								*/
/* �߂�l		:	���̑J�ڐ�	BTC_STATUS_XXX								*/
/*					�J�ڂȂ�	BTC_STATUS_NONE								*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcNone( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		/* ������Ԃ֑J�ڂ��� */
		next_status = BTC_STATUS_ACC_OFF;
	}
	return next_status;
}
/****************************************************************************/
/* �֐�����		:	btc_state_ProcAccOff									*/
/* �@�\�̐���	:	��Ԋ֐� BTC_STATUS_ACC_OFF		ACC OFF					*/
/* ����			:	event_info	�C�x���g���								*/
/* �߂�l		:	���̑J�ڐ�	BTC_STATUS_XXX								*/
/*					�J�ڂȂ�	BTC_STATUS_NONE								*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcAccOff( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_ON) ) {
			/* BT���W���[���N�� */
			(void)btc_proc_BtMdlStartup( BTC_STARTUP_FACTOR_ACC_ON );

			next_status = BTC_STATUS_WAIT_STARTUP_ACC_ON;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BT���W���[���ُ팟�m */

			/* �V�X�e������ɃC�x���g���M�y��Ԉُ�z*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* �ُ픭����Ԃ֑J�ڂ��� ACC OFF �� ON �ҋ@  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UART�f�[�^���M */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* ���̑������Ȃ� */
		}
	}
	return next_status;
}
/****************************************************************************/
/* �֐�����		:	btc_state_ProcWaitStartupAccOn							*/
/* �@�\�̐���	:	��Ԋ֐� BTC_STATUS_WAIT_STARTUP_ACC_ON		ACC ON�ɂ��BT���W���[���N���҂�	*/
/* ����			:	event_info	�C�x���g���								*/
/* �߂�l		:	���̑J�ڐ�	BTC_STATUS_XXX								*/
/*					�J�ڂȂ�	BTC_STATUS_NONE								*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitStartupAccOn( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC ON */

			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_STARTUP) ) {
			/* BT���W���[���N�� */

			/* �V�X�e������ɃC�x���g���M�y��Ԑ���z */
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_OK_BT );
			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_OTHERS );
			next_status = BTC_STATUS_WAIT_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_STARTUP_TIMEOUT) ) {
			/* BT���W���[���N���^�C���A�E�g */

			/* �V�X�e������ɃC�x���g���M�y��Ԉُ�z*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* �ُ픭����Ԃ֑J�ڂ��� ACC OFF �� ON �ҋ@  */
			next_status = BTC_STATUS_ERROR;

			OUTLOG(OutputLog( "[BTC] BTC_EVENT_STARTUP_TIMEOUT\n" );)
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BT���W���[���ُ팟�m */

			/* �V�X�e������ɃC�x���g���M�y��Ԉُ�z*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* �ُ픭����Ԃ֑J�ڂ��� ACC OFF �� ON �ҋ@  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UART�f�[�^���M */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* ���̑������Ȃ� */
		}
	}
	return next_status;
}
/****************************************************************************/
/* �֐�����		:	btc_state_ProcWaitShutdownAccOff						*/
/* �@�\�̐���	:	��Ԋ֐� BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF	ACC OFF�ɂ��BT���W���[����~�҂�	*/
/* ����			:	event_info	�C�x���g���								*/
/* �߂�l		:	���̑J�ڐ�	BTC_STATUS_XXX								*/
/*					�J�ڂȂ�	BTC_STATUS_NONE								*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/10/07												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitShutdownAccOff( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_SHUTDOWN) ) {
			/* BT���W���[����~���� */

			/* �V�X�e������ɃC�x���g���M�yBT�I���z*/
			(void)System_control_NotifyEvent( SYS_EVENT_END_PROC_COMP_BT, SYS_EVENT_PARAM_NO );

			next_status = BTC_STATUS_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BT���W���[���ُ팟�m */

			/* �V�X�e������ɃC�x���g���M�y��Ԉُ�z*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* �ُ픭����Ԃ֑J�ڂ��� ACC OFF �� ON �ҋ@  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UART�f�[�^���M */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* ���̑������Ȃ� */
		}
	}
	return next_status;
}
/****************************************************************************/
/* �֐�����		:	btc_state_ProcShutdown									*/
/* �@�\�̐���	:	��Ԋ֐� BTC_STATUS_SHUTDOWN	BT���W���[����~��		*/
/* ����			:	event_info	�C�x���g���								*/
/* �߂�l		:	���̑J�ڐ�	BTC_STATUS_XXX								*/
/*					�J�ڂȂ�	BTC_STATUS_NONE								*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcShutdown( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* �V�X�e������ɃC�x���g���M�yBT�I���z*/
			(void)System_control_NotifyEvent( SYS_EVENT_END_PROC_COMP_BT, SYS_EVENT_PARAM_NO );
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_STARTUP) ) {
			/* BT���W���[���N�� */
			(void)btc_proc_BtMdlStartup( event_info->startup_factor );

			next_status = BTC_STATUS_WAIT_STARTUP;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_PAIRING) ) {
			/* �y�A�����O�J�n�v���ł�BT���W���[���N�� */
			(void)btc_proc_BtMdlStartup( BTC_STARTUP_FACTOR_PAIRING );

			next_status = BTC_STATUS_WAIT_STARTUP_PAIRING;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BT���W���[���ُ팟�m */

			/* �V�X�e������ɃC�x���g���M�y��Ԉُ�z*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* �ُ픭����Ԃ֑J�ڂ��� ACC OFF �� ON �ҋ@  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UART�f�[�^���M */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* ���̑������Ȃ� */
		}
	}
	return next_status;
}
/****************************************************************************/
/* �֐�����		:	btc_state_ProcWaitStartup								*/
/* �@�\�̐���	:	��Ԋ֐� BTC_STATUS_WAIT_STARTUP	BT���W���[���N���҂�*/
/* ����			:	event_info	�C�x���g���								*/
/* �߂�l		:	���̑J�ڐ�	BTC_STATUS_XXX								*/
/*					�J�ڂȂ�	BTC_STATUS_NONE								*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitStartup( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SHUTDOWN) ) {
			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( event_info->shutdown_factor );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_ON) ) {
			/* ACC-ON */

			/* �N���v����ACC ON�ɕύX�i�t�F�[���Z�[�t�j*/
			S_btc_startup_factor = BTC_STARTUP_FACTOR_ACC_ON;
			next_status = BTC_STATUS_WAIT_STARTUP_ACC_ON;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_STARTUP) ) {
			/* BT���W���[���N�� */
			next_status = BTC_STATUS_STARTUP;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_STARTUP_TIMEOUT) ) {
			/* BT���W���[���N���^�C���A�E�g */

			/* �V�X�e������ɃC�x���g���M�y��Ԉُ�z*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* �ُ픭����Ԃ֑J�ڂ��� ACC OFF �� ON �ҋ@  */
			next_status = BTC_STATUS_ERROR;

			OUTLOG(OutputLog( "[BTC] BTC_EVENT_STARTUP_TIMEOUT\n" );)
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BT���W���[���ُ팟�m */

			/* �V�X�e������ɃC�x���g���M�y��Ԉُ�z*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* �ُ픭����Ԃ֑J�ڂ��� ACC OFF �� ON �ҋ@  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UART�f�[�^���M */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* ���̑������Ȃ� */
		}
	}
	return next_status;
}
/****************************************************************************/
/* �֐�����		:	btc_state_ProcStartup									*/
/* �@�\�̐���	:	��Ԋ֐� BTC_STATUS_STARTUP	BT���W���[���N����			*/
/* ����			:	event_info	�C�x���g���								*/
/* �߂�l		:	���̑J�ڐ�	BTC_STATUS_XXX								*/
/*					�J�ڂȂ�	BTC_STATUS_NONE								*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcStartup( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SHUTDOWN) ) {
			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( event_info->shutdown_factor );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_PAIRING) ) {
			/* �y�A�����O */
			(void)Bt_comm_mdl_EnterPairingMode();
			S_btc_startup_factor = BTC_STARTUP_FACTOR_PAIRING;

			next_status = BTC_STATUS_PAIRING;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_PEER_CONNECTED) ) {
			/* Peer�ڑ� */
			next_status = BTC_STATUS_PEEE_CONNECTED_IDLE;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BT���W���[���ُ팟�m */

			/* �V�X�e������ɃC�x���g���M�y��Ԉُ�z*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* �ُ픭����Ԃ֑J�ڂ��� ACC OFF �� ON �ҋ@  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UART�f�[�^���M */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* ���̑������Ȃ� */
		}
	}
	return next_status;
}
/****************************************************************************/
/* �֐�����		:	btc_state_ProcWaitShutdown								*/
/* �@�\�̐���	:	��Ԋ֐� BTC_STATUS_WAIT_SHUTDOWN	BT���W���[����~�҂�*/
/* ����			:	event_info	�C�x���g���								*/
/* �߂�l		:	���̑J�ڐ�	BTC_STATUS_XXX								*/
/*					�J�ڂȂ�	BTC_STATUS_NONE								*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitShutdown( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_SHUTDOWN) ) {
			/* BT���W���[����~���� */
			next_status = BTC_STATUS_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BT���W���[���ُ팟�m */

			/* �V�X�e������ɃC�x���g���M�y��Ԉُ�z*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* �ُ픭����Ԃ֑J�ڂ��� ACC OFF �� ON �ҋ@  */
			next_status = BTC_STATUS_ERROR;
		}
		else {
			/* ���̑������Ȃ� */
		}
	}
	return next_status;
}
/****************************************************************************/
/* �֐�����		:	btc_state_ProcPeerConnectedIdle							*/
/* �@�\�̐���	:	��Ԋ֐� BTC_STATUS_PEEE_CONNECTED_IDLE	BT�ʐM��: �f�[�^�����M	*/
/* ����			:	event_info	�C�x���g���								*/
/* �߂�l		:	���̑J�ڐ�	BTC_STATUS_XXX								*/
/*					�J�ڂȂ�	BTC_STATUS_NONE								*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcPeerConnectedIdle( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;
	E_RC			rc;

	if( NULL != event_info ) {
		if( 0L == S_btc_encode_key ) {
			/* �Í����L�[���擾�Ȃ�Í��p�������� */
			S_btc_encode_key = Signal_control_GetRandomValue();

			/* BT�p�Í����L�[�ݒ� */
			(void)Signal_control_SetEncodeKey( DEVICE_TYPE_BT, S_btc_encode_key );
		}
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SHUTDOWN) ) {
			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( event_info->shutdown_factor );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_PEER_DISCONNECTED) ) {
			/* Peer�ؒf */
			next_status = BTC_STATUS_STARTUP;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SIGNAL_LIST) ) {
			/* �ԗ��M���ꗗ�擾(���X�g)�v�� */

			/* ��M�p�P�b�g���N���A */
			(void)btc_data_ClearRecvPacket( &S_btc_packet_info );

			/* �ԗ��M���ꗗ�擾(���X�g)�����𑗐M */
			(void)btc_proc_SendData( D_BTC_FMT_TYPE_REP_LIST );
			/* ��ԑJ�ڂȂ� */
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_START_SEND) ) {
			/* �ԗ����(�擾�J�n)�v�� */

			/* ���M���M���������o�� */
			(void)btc_data_GetSendSignalInfo( &S_btc_packet_info );

			/* ��M�p�P�b�g���N���A */
			(void)btc_data_ClearRecvPacket( &S_btc_packet_info );

			/* �ԗ����(�擾�J�n)�����𑗐M */
			rc = btc_proc_SendData( D_BTC_FMT_TYPE_REP_START );
			if( E_RC_OK == rc ) {
				next_status = BTC_STATUS_PEEE_CONNECTED_SENDING;
			}
			else {
				/* ���M�ُ�͖��� */
			}
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_STOP_SEND) ) {
			/* �ԗ����(�擾��~)�v�� */

			/* ��M�p�P�b�g���N���A */
			(void)btc_data_ClearRecvPacket( &S_btc_packet_info );

			/* �ԗ����(�擾��~)�����𑗐M�i�t�F�[���j */
			(void)btc_proc_SendData( D_BTC_FMT_TYPE_REP_STOP );	
				/* �{�������s�����t�F�[���ő��M */
			/* ��ԑJ�ڂȂ� */
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_RECV_PACKET_TIMEOUT) ) {
			/* �p�P�b�g��M�^�C���A�E�g */

			/* ��M�o�b�t�@����ɂ��� */
			(void)btc_ring_Empty( &S_btc_recv_ring );

			/* �p�P�b�g��M�������N���A */
			btc_time_ClearTime( BTC_TIME_KIND_PACKET_FOUND );
			/* ��ԑJ�ڂȂ� */
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BT���W���[���ُ팟�m */

			/* �V�X�e������ɃC�x���g���M�y��Ԉُ�z*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* �ُ픭����Ԃ֑J�ڂ��� ACC OFF �� ON �ҋ@  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UART�f�[�^���M */
			(void)btc_proc_UartDataSend();
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_RETURN_SEND) ) {
			/* ��M�f�[�^�����̂܂ܑ��M */
			(void)btc_proc_ReturnSend();
		}
		else {
			/* ���̑������Ȃ� */
		}
	}
	return next_status;
}
/****************************************************************************/
/* �֐�����		:	btc_state_ProcPeerConnectedSending						*/
/* �@�\�̐���	:	��Ԋ֐� BTC_STATUS_PEEE_CONNECTED_SENDING	BT�ʐM��: �f�[�^���M��	*/
/* ����			:	event_info	�C�x���g���								*/
/* �߂�l		:	���̑J�ڐ�	BTC_STATUS_XXX								*/
/*					�J�ڂȂ�	BTC_STATUS_NONE								*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcPeerConnectedSending( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SHUTDOWN) ) {
			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( event_info->shutdown_factor );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_PEER_DISCONNECTED) ) {
			/* Peer�ؒf */
			next_status = BTC_STATUS_STARTUP;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SIGNAL_LIST) ) {
			/* �ԗ��M���ꗗ�擾(���X�g)�v�� */

			/* ��M�p�P�b�g���N���A */
			(void)btc_data_ClearRecvPacket( &S_btc_packet_info );

			/* ��ԑJ�ڂȂ� */
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_START_SEND) ) {
			/* �ԗ����(�擾�J�n)�v�� */

			/* ��M�p�P�b�g���N���A */
			(void)btc_data_ClearRecvPacket( &S_btc_packet_info );

			/* ��ԑJ�ڂȂ� */
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_STOP_SEND) ) {
			/* ��M�p�P�b�g���N���A */
			(void)btc_data_ClearRecvPacket( &S_btc_packet_info );

			/* �ԗ����(�擾��~)�����𑗐M */
			(void)btc_proc_SendData( D_BTC_FMT_TYPE_REP_STOP );	

			next_status = BTC_STATUS_PEEE_CONNECTED_IDLE;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_SEND_TRIGGER) ) {
			/* �ԗ��M�����M�g���K */

			/* �ԗ����(�擾�J�n)�����𑗐M */
			(void)btc_proc_SendData( D_BTC_FMT_TYPE_REP_START );
			/* ��ԑJ�ڂȂ� */
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_RECV_PACKET_TIMEOUT) ) {
			/* �p�P�b�g��M�^�C���A�E�g */

			/* ��M�o�b�t�@����ɂ��� */
			(void)btc_ring_Empty( &S_btc_recv_ring );

			/* �p�P�b�g��M�������N���A */
			btc_time_ClearTime( BTC_TIME_KIND_PACKET_FOUND );
			/* ��ԑJ�ڂȂ� */
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BT���W���[���ُ팟�m */

			/* �V�X�e������ɃC�x���g���M�y��Ԉُ�z*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* �ُ픭����Ԃ֑J�ڂ��� ACC OFF �� ON �ҋ@  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UART�f�[�^���M */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* ���̑������Ȃ� */
		}
	}
	return next_status;
}
/****************************************************************************/
/* �֐�����		:	btc_state_ProcWaitStartupPairing						*/
/* �@�\�̐���	:	��Ԋ֐� BTC_STATUS_WAIT_STARTUP_PAIRING	�y�A�����O�ڍs��	*/
/* ����			:	event_info	�C�x���g���								*/
/* �߂�l		:	���̑J�ڐ�	BTC_STATUS_XXX								*/
/*					�J�ڂȂ�	BTC_STATUS_NONE								*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcWaitStartupPairing( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SHUTDOWN) ) {
			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( event_info->shutdown_factor );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_STARTUP) ) {
			/* �y�A�����O�ڍs���Ƀ}�j���A�����[�h���ւ̐ؑւ͋N���҂��̏�Ԃ֑J�ڂ����� */
			S_btc_startup_factor = BTC_STARTUP_FACTOR_OTHERS;

			next_status = BTC_STATUS_WAIT_STARTUP;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_STARTUP) ) {
			/* BT���W���[���N�� */

			/* �y�A�����O�J�n */
			(void)Bt_comm_mdl_EnterPairingMode();
			S_btc_startup_factor = BTC_STARTUP_FACTOR_PAIRING;

			next_status = BTC_STATUS_PAIRING;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_STARTUP_TIMEOUT) ) {
			/* BT���W���[���N���^�C���A�E�g */

			/* �V�X�e������ɃC�x���g���M�y��Ԉُ�z*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* �ُ픭����Ԃ֑J�ڂ��� ACC OFF �� ON �ҋ@  */
			next_status = BTC_STATUS_ERROR;

			OUTLOG(OutputLog( "[BTC] BTC_EVENT_STARTUP_TIMEOUT\n" );)
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BT���W���[���ُ팟�m */

			/* �V�X�e������ɃC�x���g���M�y��Ԉُ�z*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* �ُ픭����Ԃ֑J�ڂ��� ACC OFF �� ON �ҋ@  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UART�f�[�^���M */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* ���̑������Ȃ� */
		}
	}
	return next_status;
}
/****************************************************************************/
/* �֐�����		:	btc_state_ProcPairing									*/
/* �@�\�̐���	:	��Ԋ֐� BTC_STATUS_PAIRING	�y�A�����O					*/
/* ����			:	event_info	�C�x���g���								*/
/* �߂�l		:	���̑J�ڐ�	BTC_STATUS_XXX								*/
/*					�J�ڂȂ�	BTC_STATUS_NONE								*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcPairing( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_STARTUP) ) {
			/* BT���W���[���ʐM���[�h�J�n */
			(void)Bt_comm_mdl_EnterCommMode();
			S_btc_startup_factor = BTC_STARTUP_FACTOR_OTHERS;

			next_status = BTC_STATUS_WAIT_STARTUP;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_SHUTDOWN) ) {
			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( event_info->shutdown_factor );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_PAIRING) ) {
			/* �y�A�����O���� */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_OTHERS );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;

			OUTLOG(OutputLog( "[BTC] BTC_EVENT_PAIRING\n" );)
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_PAIRING_TIMEOUT) ) {
			/* �y�A�����O�^�C���A�E�g */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_OTHERS );

			next_status = BTC_STATUS_WAIT_SHUTDOWN;

			OUTLOG(OutputLog( "[BTC] BTC_EVENT_PAIRING_TIMEOUT\n" );)
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_ERROR) ) {
			/* BT���W���[���ُ팟�m */

			/* �V�X�e������ɃC�x���g���M�y��Ԉُ�z*/
			(void)System_control_NotifyEvent( SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_NG_BT );

			/* �ُ픭����Ԃ֑J�ڂ��� ACC OFF �� ON �ҋ@  */
			next_status = BTC_STATUS_ERROR;
		}
		else if( 0UL != (event_info->event_flg & BTC_EVENT_REQ_UART_SEND) ) {
			/* UART�f�[�^���M */
			(void)btc_proc_UartDataSend();
		}
		else {
			/* ���̑������Ȃ� */
		}
	}
	return next_status;
}
/****************************************************************************/
/* �֐�����		:	btc_state_ProcError										*/
/* �@�\�̐���	:	��Ԋ֐� BTC_STATUS_ERROR	�ُ픭�� 					*/
/* ����			:	event_info	�C�x���g���								*/
/* �߂�l		:	���̑J�ڐ�	BTC_STATUS_XXX								*/
/*					�J�ڂȂ�	BTC_STATUS_NONE								*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/16												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_BTC_STATUS btc_state_ProcError( const BTC_EVENT_INFO* event_info )
{
	E_BTC_STATUS	next_status = BTC_STATUS_NONE;

	if( NULL != event_info ) {
		/* ���ݏ����Ȃ� */
		if( 0UL != (event_info->event_flg & BTC_EVENT_ACC_OFF) ) {
			/* ACC OFF */

			/* BT���W���[����~ */
			(void)btc_proc_BtMdlShutdown( BTC_SHUTDOWN_FACTOR_ACC_OFF );

			next_status = BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF;
		}
	}
	return next_status;
}
/*==========================================================================*/
/* ��ԑJ�ڎ��֐�															*/
/*==========================================================================*/
/****************************************************************************/
/* �֐�����		:	btc_state_ProcOnEnter									*/
/* �@�\�̐���	:	��Ԃɓ���Ƃ��̏���									*/
/* ����			:	���ꂩ�������										*/
/* �߂�l		:	�Ȃ�													*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL void btc_state_ProcOnEnter( E_BTC_STATUS status )
{
	switch( status ) {
	case BTC_STATUS_ACC_OFF:				/*	ACC OFF								*/
		break;
	case BTC_STATUS_WAIT_STARTUP_ACC_ON:	/*	ACC ON�ɂ��BT���W���[���N���҂�	*/
		/* BT���W���[���N�������L�^�iBT���W���[���N���^�C���A�E�g�̂��߁j*/
		btc_time_RegTime( BTC_TIME_KIND_STARTUP );
		break;
	case BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF:	/*	ACC OFF�ɂ��BT���W���[����~�҂�	*/
		S_btc_req_Paring_Inspection = 0;
		S_btc_req_Communication_Inspection = 0;
		break;
	case BTC_STATUS_SHUTDOWN:				/*	BT���W���[����~��		*/
		break;
	case BTC_STATUS_WAIT_STARTUP:			/*	BT���W���[���N���҂�	*/
		/* BT���W���[���N�������L�^�iBT���W���[���N���^�C���A�E�g�̂��߁j*/
		btc_time_RegTime( BTC_TIME_KIND_STARTUP );
		break;
	case BTC_STATUS_STARTUP:				/*	BT���W���[���N����		*/
		/* ��M�o�b�t�@����ɂ��� */
		(void)btc_ring_Empty( &S_btc_recv_ring );
		break;
	case BTC_STATUS_WAIT_SHUTDOWN:			/*	BT���W���[����~�҂�	*/
		break;
	case BTC_STATUS_PEEE_CONNECTED_IDLE:	/*	BT�ʐM��: �f�[�^�����M	*/
		break;
	case BTC_STATUS_PEEE_CONNECTED_SENDING:	/*	BT�ʐM��: �f�[�^���M��	*/
		break;
	case BTC_STATUS_WAIT_STARTUP_PAIRING:	/*	�y�A�����O�ڍs��		*/
		/* BT���W���[���N�������L�^�iBT���W���[���N���^�C���A�E�g�̂��߁j*/
		btc_time_RegTime( BTC_TIME_KIND_STARTUP );
		break;
	case BTC_STATUS_PAIRING:				/*	�y�A�����O				*/
		/* �y�A�����O�J�n�����L�^�i60�b�^�C���A�E�g�̂��߁j */
		btc_time_RegTime( BTC_TIME_KIND_PAIRING_START );
		break;
	case BTC_STATUS_ERROR:					/*	�ُ픭��				*/
		S_btc_req_Paring_Inspection = 0;
		S_btc_req_Communication_Inspection = 0;
		break;
	default:
		break;
	}
}
/****************************************************************************/
/* �֐�����		:	btc_state_ProcOnLeave									*/
/* �@�\�̐���	:	��Ԃ���o��Ƃ��̏���									*/
/* ����			:	���ꂩ��o����										*/
/* �߂�l		:	�Ȃ�													*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL void btc_state_ProcOnLeave( E_BTC_STATUS status )
{
	switch( status ) {
	case BTC_STATUS_ACC_OFF:				/*	ACC OFF								*/
		break;
	case BTC_STATUS_WAIT_STARTUP_ACC_ON:	/*	ACC ON�ɂ��BT���W���[���N���҂�	*/
		/* BT���W���[���N�������N���A�iBT���W���[���N���^�C���A�E�g�̂��߁j */
		btc_time_ClearTime( BTC_TIME_KIND_STARTUP );
		break;
	case BTC_STATUS_WAIT_SHUTDOWN_ACC_OFF:	/*	ACC OFF�ɂ��BT���W���[����~�҂�	*/
		break;
	case BTC_STATUS_SHUTDOWN:				/*	BT���W���[����~��		*/
		break;
	case BTC_STATUS_WAIT_STARTUP:			/*	BT���W���[���N���҂�	*/
		/* BT���W���[���N�������N���A�iBT���W���[���N���^�C���A�E�g�̂��߁j */
		btc_time_ClearTime( BTC_TIME_KIND_STARTUP );
		break;
	case BTC_STATUS_STARTUP:				/*	BT���W���[���N����		*/
		break;
	case BTC_STATUS_WAIT_SHUTDOWN:			/*	BT���W���[����~�҂�	*/
		break;
	case BTC_STATUS_PEEE_CONNECTED_IDLE:	/*	BT�ʐM��: �f�[�^�����M	*/
		/*�ʐM�v���Ȃ��ݒ�*/
		S_btc_req_Communication_Inspection = 0;
		break;
	case BTC_STATUS_PEEE_CONNECTED_SENDING:	/*	BT�ʐM��: �f�[�^���M��	*/
		/* ���M�������N���A */
		S_btc_send_signal_id_num = 0U;
		(void)memset( S_btc_send_signal_id_array, 0, sizeof(S_btc_send_signal_id_array) );
		break;
	case BTC_STATUS_WAIT_STARTUP_PAIRING:	/*	�y�A�����O�ڍs��		*/
		/* BT���W���[���N�������N���A�iBT���W���[���N���^�C���A�E�g�̂��߁j */
		btc_time_ClearTime( BTC_TIME_KIND_STARTUP );
		break;
	case BTC_STATUS_PAIRING:				/*	�y�A�����O				*/
		/* �y�A�����O�J�n�����N���A�i60�b�^�C���A�E�g�̂��߁j */
		btc_time_ClearTime( BTC_TIME_KIND_PAIRING_START );
		/*�y�A�����O�v���Ȃ��ݒ�*/
		S_btc_req_Paring_Inspection = 0;
		break;
	case BTC_STATUS_ERROR:					/*	�ُ픭��				*/
		break;
	default:
		break;
	}
}
/*==========================================================================*/
/* ��ԏ������葱���֐�														*/
/*==========================================================================*/
/****************************************************************************/
/* �֐�����		:	btc_proc_BtMdlStartup									*/
/* �@�\�̐���	:	BT���W���[���N���葱��									*/
/* ����			:	startup_factor	�N���v��								*/
/* �߂�l		:	E_RC	��������										*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_proc_BtMdlStartup( E_BTC_STARTUP_FACTOR startup_factor )
{
	E_BT_COMM_MDL_RESULT	ret;
	E_RC					rc = E_RC_NG;

	/* BT���W���[���N�� */
	ret = Bt_comm_mdl_Startup();
	if( BT_COMM_MDL_RESULT_OK == ret ) {
		/* �N���v�����L�^ */
		S_btc_startup_factor = startup_factor;

		rc = E_RC_OK;
	}
	return rc;
}
/****************************************************************************/
/* �֐�����		:	btc_proc_BtMdlShutdown									*/
/* �@�\�̐���	:	BT���W���[����~�葱��									*/
/* ����			:	shutdown_factor	��~�v��								*/
/* �߂�l		:	E_RC	��������										*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/26												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_proc_BtMdlShutdown( E_BTC_SHUTDOWN_FACTOR shutdown_factor )
{
	E_BT_COMM_MDL_RESULT	ret;
	E_RC					rc = E_RC_NG;

	/* BT���W���[����~ */
	ret = Bt_comm_mdl_Shutdown();
	if( BT_COMM_MDL_RESULT_OK == ret ) {

		rc = E_RC_OK;
	}
	return rc;
}
/****************************************************************************/
/* �֐�����		:	btc_proc_SendData										*/
/* �@�\�̐���	:	�f�[�^���M�葱��										*/
/* ����			:	data_type	�f�[�^���									*/
/* �߂�l		:	E_RC	��������										*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_proc_SendData( UI_8 data_type )
{
	E_RC					rc = E_RC_NG;
	UI_16					data_bytes = 0U;
	E_BT_COMM_MDL_RESULT	bt_result;

	switch( data_type ) {
	case D_BTC_FMT_TYPE_REP_LIST:	/* �ԗ��M���ꗗ�擾(���X�g)���� */
		rc = btc_data_MakeRepList( S_btc_send_data_buff, sizeof(S_btc_send_data_buff), &data_bytes );
		break;
	case D_BTC_FMT_TYPE_REP_START:	/* �ԗ����(�擾�J�n)���� */
		rc = btc_data_MakeRepStart( S_btc_send_data_buff, sizeof(S_btc_send_data_buff), &data_bytes );
		break;
	case D_BTC_FMT_TYPE_REP_STOP:	/* �ԗ����(�擾��~)���� */
		rc = btc_data_MakeRepStop( S_btc_send_data_buff, sizeof(S_btc_send_data_buff), &data_bytes );
		break;
	default:
		rc = E_RC_NG_PARAM;
		break;
	}
	if( E_RC_OK == rc ) {
		/* �쐬�����p�P�b�g�𑗐M */
		bt_result = Bt_comm_mdl_SendData( S_btc_send_data_buff, data_bytes );
		if( BT_COMM_MDL_RESULT_OK == bt_result ) {
			/* ���M���� */
			if( D_BTC_FMT_TYPE_REP_START == data_type ) {
				/* �����I�ȑ��M�g���K�����p�ɑ��M�������L�^ */
				btc_time_RegTime( BTC_TIME_KIND_SEND_REP_START );
			}
		}
		else {
			/* ���M���s */
			rc = E_RC_NG;
		}
	}
	return rc;
}
/****************************************************************************/
/* �֐�����		:	btc_proc_UartDataSend									*/
/* �@�\�̐���	:	UART�f�[�^���M											*/
/* ����			:	void													*/
/* �߂�l		:	E_RC	��������										*/
/* �쐬��		:	(OFT)Sakata												*/
/* �쐬��		:	17/02/07												*/
/* ���l			:	UART�iAdvanceControlUni�j�̃f�[�^��BT�ő��M����			*/
/* 				:	BT���ڑ���Ԃ̏ꍇ�́AUART�f�[�^�͔j�������			*/
/****************************************************************************/
BTC_LOCAL E_RC btc_proc_UartDataSend(void)
{
	E_RC					rc = E_RC_NG;
	UI_16					data_bytes = 0U;
	E_BT_COMM_MDL_RESULT	bt_result;
	
	/* AdvanceControl����UART�f�[�^�擾 */
	data_bytes = Advance_control_getUartData(S_btc_send_data_buff, sizeof(S_btc_send_data_buff));
	if( data_bytes > 0 ){
		/* �擾�����f�[�^�𑗐M */
		bt_result = Bt_comm_mdl_SendData( S_btc_send_data_buff, data_bytes );
		if( BT_COMM_MDL_RESULT_OK == bt_result ) {
			/* ���M���� */
			rc = E_RC_OK;
		}
	}
	return rc;
}
/****************************************************************************/
/* �֐�����		:	btc_proc_ReturnSend										*/
/* �@�\�̐���	:	�������[�h�f�[�^���M									*/
/* ����			:	void													*/
/* �߂�l		:	E_RC	��������										*/
/* �쐬��		:	(OFT)Sakata												*/
/* �쐬��		:	17/02/15												*/
/* ���l			:	��M�f�[�^�����̂܂ܑ��M����							*/
/****************************************************************************/
BTC_LOCAL E_RC btc_proc_ReturnSend(void)
{
	E_RC					rc = E_RC_NG;
	UI_16					data_bytes = 0U;
	E_BT_COMM_MDL_RESULT	bt_result;

	/* �c��f�[�^�o�C�g���擾 */
	data_bytes = btc_ring_GetDataBytes( &S_btc_recv_ring );
	if( 0U < data_bytes ) {
		if( data_bytes > sizeof(S_btc_send_data_buff) ) {
			data_bytes = sizeof(S_btc_send_data_buff);
		}
		/* �o�b�t�@�Q�� */
		(void)btc_ring_Read( &S_btc_recv_ring, data_bytes, S_btc_send_data_buff );
		/* �擾�����f�[�^�𑗐M */
		bt_result = Bt_comm_mdl_SendData( S_btc_send_data_buff, data_bytes );
		if( BT_COMM_MDL_RESULT_OK == bt_result ) {
			/* ���M���� */
			rc = E_RC_OK;
		}
	}
	return rc;
}

/*==========================================================================*/
/* ���M�f�[�^�쐬�֐�														*/
/*==========================================================================*/
/****************************************************************************/
/* �֐�����		:	btc_data_MakeRepList									*/
/* �@�\�̐���	:	���M�f�[�^�쐬:�ԗ��M���ꗗ�擾(���X�g)����				*/
/* ����			:	buff		[ /o]�f�[�^�쐬��o�b�t�@					*/
/* ����			:	buff_bytes	[i/ ]�o�b�t�@�T�C�Y							*/
/* ����			:	data_bytes	[ /o]�쐬�����f�[�^�̃T�C�Y					*/
/* �߂�l		:	E_RC	��������										*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_data_MakeRepList( UI_8* buff, UI_16 buff_bytes, UI_16* data_bytes )
{
	E_RC			rc = E_RC_NG;
	UI_8*			p = NULL;
	UI_16			crc = 0U;
	UI_16			data_length = 0U;
	UI_16			length = 0U;
	UI_16			packet_bytes = 0U;
	UI_16			firmware_version = 0U;
	const UI_16*	fixed_signal_id_list = NULL;	/* �擾�\�Ȏԗ��M�����i�Œ���j		*/
	const UI_16*	vehicle_signal_id_list = NULL;	/* �擾�\�Ȏԗ��M�����i�ԗ����j		*/
	UI_8			fixed_signal_num = 0U;			/* �擾�\�Ȏԗ��M�����i�Œ���j�̐�	*/
	UI_8			vehicle_signal_num = 0U;		/* �擾�\�Ȏԗ��M�����i�ԗ����j�̐�	*/
	UI_8			i;
	BTC_LOCAL		SI_32			encode_key;		/*	�Í����L�[		*/

	if( (NULL == buff) || (NULL == data_bytes) ) {
		rc = E_RC_NG_PARAM;
	}
	else {
		/* ���J�M��ID�ꗗ�擾 */
		/* �M�����:�Œ�l	*/
		(void)Signal_control_GetSignalList( DEVICE_TYPE_BT, ID_TYPE_FIXED_ID, &fixed_signal_num, &fixed_signal_id_list );
		/* �M�����:�ϐ��l	*/
		(void)Signal_control_GetSignalList( DEVICE_TYPE_BT, ID_TYPE_VARIABLE_ID, &vehicle_signal_num, &vehicle_signal_id_list );
		if ((fixed_signal_id_list == NULL) || (vehicle_signal_id_list == NULL)) {
			return E_RC_NG;
		}

		/* �p�P�b�g�� */
		packet_bytes = btc_data_GetBytesRepList(fixed_signal_num, vehicle_signal_num);
		data_length = packet_bytes - D_BTC_FMT_BYTES_FIX;

		if( packet_bytes > buff_bytes ) {
			rc = E_RC_NG_PARAM;
		}
		else {
			/* �o�b�t�@�N���A */
			(void)memset( buff, 0, buff_bytes );

			/* length���v�Z */
			length = data_length + 1U;
		
			p = buff;

			/*==================================================================*/
			/* �w�b�_�쐬														*/
			/*==================================================================*/
			*p = D_BTC_FMT_HEADER_START;
			p++;
			Common_Set16( p, length );
			p += 2;
			*p = D_BTC_FMT_TYPE_REP_LIST;
			p++;

			/*==================================================================*/
			/* �f�[�^���쐬														*/
			/*==================================================================*/
			/* �ő̎��ʔԍ�(16�o�C�g) */
			/* 00�Œ� */
			p += 16;

			/* �Ԏ�R�[�h(2�o�C�g) */
			/* 00�Œ� */
			p += 2;

			/* �t�H�[�}�b�g�o�[�W����(3�o�C�g) */
			Common_Set24( p, BTC_FORMAT_VERSION );
			p += 3;

			/* �M�����X�g�o�[�W����(3�o�C�g) */
			Common_Set24( p, Signal_control_GetSignalVersion() );
			p += 3;

			/* �t�@�[���E�F�A�o�[�W����(3�o�C�g) */
			(void)Eeprom_control_GetData( EEPROM_DATA_FIRMWARE_VER, &firmware_version );
			/* 16bit�̃o�[�W������O�l�߂ŋL�^ */
			Common_Set24( p, ((UI_32)firmware_version) << 8 );
			p += 3;

			/* �R�����g(10�o�C�g) */
			/* 00�Œ� */
			p += 10;

			/* �擾�\�Ȏԗ��M�����i�Œ���j */
			*p = fixed_signal_num;
			p++;
			for( i = 0; i < fixed_signal_num; i++ ) {
				/* �擾�\�Ȏԗ��M��ID�i�Œ���j */
				Common_Set16( p, fixed_signal_id_list[i] );
				p += 2;
			}
			/* �擾�\�Ȏԗ��M�����i�ԗ����j */
			*p = vehicle_signal_num;
			p++;
			for( i = 0; i < vehicle_signal_num; i++ ) {
				/* �擾�\�Ȏԗ��M��ID�i�ԗ����j */
				Common_Set16( p, vehicle_signal_id_list[i] );
				p += 2;
			}
			/* ����(4�o�C�g) */
			/* BT�p�Í����L�[�擾 */
			(void)Signal_control_GetEncodeKey( DEVICE_TYPE_BT, &encode_key );
			Common_Set32( p, (UI_32)encode_key );
			p += 4;

			/*==================================================================*/
			/* �t�b�^�쐬														*/
			/*==================================================================*/
			crc = Common_CalcCRC( buff, 1U, length + 2U );
			Common_Set16( p, crc );
			p += 2;
			*p = D_BTC_FMT_FOOTER_END;

			/* �f�[�^�����i�[ */
			*data_bytes = packet_bytes;

			rc = E_RC_OK;
		}
	}
	return rc;
}
/****************************************************************************/
/* �֐�����		:	btc_data_MakeRepStart									*/
/* �@�\�̐���	:	���M�f�[�^�쐬:�ԗ����(�擾�J�n)����					*/
/* ����			:	buff		[ /o]�f�[�^�쐬��o�b�t�@					*/
/* ����			:	buff_bytes	[i/ ]�o�b�t�@�T�C�Y							*/
/* ����			:	data_bytes	[ /o]�쐬�����f�[�^�̃T�C�Y					*/
/* �߂�l		:	E_RC	��������										*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_data_MakeRepStart( UI_8* buff, UI_16 buff_bytes, UI_16* data_bytes )
{
	E_RC	rc = E_RC_NG;
	UI_8*	p = NULL;
	SI_32	signal_value = 0UL;
	UI_16	crc = 0U;
	UI_16	data_length = 0U;
	UI_16	length = 0U;
	UI_16	packet_bytes = 0U;
	UI_8	errcode_num = 0U;
	UI_8	i = 0U;

	if( (NULL == buff) || (NULL == data_bytes) ) {
		rc = E_RC_NG_PARAM;
	}
	else {
		/* �p�P�b�g�� */
		packet_bytes = btc_data_GetBytesRepStart(S_btc_send_signal_id_num, errcode_num);
		data_length = packet_bytes - D_BTC_FMT_BYTES_FIX;

		if( packet_bytes > buff_bytes ) {
			rc = E_RC_NG_PARAM;
		}
		else {
			/* �o�b�t�@�N���A */
			(void)memset( buff, 0, buff_bytes );

			/* length���v�Z */
			length = data_length + 1U;

			p = buff;

			/*==================================================================*/
			/* �w�b�_�쐬														*/
			/*==================================================================*/
			*p = D_BTC_FMT_HEADER_START;
			p++;
			Common_Set16( p, length );
			p += 2;
			*p = D_BTC_FMT_TYPE_REP_START;
			p++;

			/*==================================================================*/
			/* �f�[�^���쐬														*/
			/*==================================================================*/
			/* �^�C���X�^���v */
			Common_Set32( p, Timer_ctrl_GetTime1ms() );
			p += 4;

			/* �G���[�R�[�h�� */
			*p = errcode_num;
			p++;
			/* �\��G���[�R�[�h�͌��݊i�[���Ȃ� */

			/* ��������ԗ��M����(�v�����ꂽ��) */
			*p = S_btc_send_signal_id_num;
			p++;

			/* ��������ԗ��M��ID�ƐM���l */
			for( i = 0; i < S_btc_send_signal_id_num; i++ ) {
				/* �ԗ��M��ID */
				Common_Set16( p, S_btc_send_signal_id_array[i] );
				p += 2;

				/* �M���l */
				(void)Signal_control_GetSignalInfo( DEVICE_TYPE_BT, S_btc_send_signal_id_array[i], &signal_value );
				Common_Set32( p, (UI_32)signal_value );
				p += 4;
			}
			/*==================================================================*/
			/* �t�b�^�쐬														*/
			/*==================================================================*/
			crc = Common_CalcCRC( buff, 1U, length + 2U );
			Common_Set16( p, crc );
			p += 2;
			*p = D_BTC_FMT_FOOTER_END;

			/* �f�[�^�����i�[ */
			*data_bytes = packet_bytes;

			rc = E_RC_OK;
		}
	}
	return rc;
}
/****************************************************************************/
/* �֐�����		:	btc_data_MakeRepStop									*/
/* �@�\�̐���	:	���M�f�[�^�쐬:�ԗ����(�擾��~)����					*/
/* ����			:	buff		[ /o]�f�[�^�쐬��o�b�t�@					*/
/* ����			:	buff_bytes	[i/ ]�o�b�t�@�T�C�Y							*/
/* ����			:	data_bytes	[ /o]�쐬�����f�[�^�̃T�C�Y					*/
/* �߂�l		:	E_RC	��������										*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL E_RC btc_data_MakeRepStop( UI_8* buff, UI_16 buff_bytes, UI_16* data_bytes )
{
	E_RC	rc = E_RC_NG;
	UI_8*	p;
	UI_16	crc = 0U;
	UI_16	data_length = 0U;
	UI_16	length = 0U;
	UI_16	packet_bytes = 0U;

	if( (NULL == buff) || (NULL == data_bytes) ) {
		rc = E_RC_NG_PARAM;
	}
	else {
		/* �p�P�b�g�� */
		packet_bytes = D_BTC_FMT_BYTES_REP_STOP;
		data_length = packet_bytes - D_BTC_FMT_BYTES_FIX;

		if( packet_bytes > buff_bytes ) {
			rc = E_RC_NG_PARAM;
		}
		else {
			/* �o�b�t�@�N���A */
			(void)memset( buff, 0, buff_bytes );

			/* length���v�Z */
			length = data_length + 1U;
		
			p = buff;

			/*==================================================================*/
			/* �w�b�_�쐬														*/
			/*==================================================================*/
			*p = D_BTC_FMT_HEADER_START;
			p++;
			Common_Set16( p, length );
			p += 2;
			*p = D_BTC_FMT_TYPE_REP_STOP;
			p++;

			/*==================================================================*/
			/* �t�b�^�쐬														*/
			/*==================================================================*/
			crc = Common_CalcCRC( buff, 1U, length + 2U );
			Common_Set16( p, crc );
			p += 2;
			*p = D_BTC_FMT_FOOTER_END;

			/* �f�[�^�����i�[ */
			*data_bytes = packet_bytes;

			rc = E_RC_OK;
		}
	}
	return rc;
}
/****************************************************************************/
/* �֐�����		:	btc_data_MakeRepStop									*/
/* �@�\�̐���	:	�u�ԗ��M���ꗗ�擾(���X�g)�����v�f�[�^�T�C�Y�擾		*/
/* ����			:	fixed_signal_num	[i/ ]�Œ���						*/
/* ����			:	vehicle_signal_num	[i/ ]�ԗ����						*/
/* �߂�l		:	�f�[�^�T�C�Y											*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/08/29												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL UI_16 btc_data_GetBytesRepList( UI_8 fixed_signal_num, UI_8 vehicle_signal_num )
{
	return ((UI_16)D_BTC_FMT_HEADER_BYTES + (43 + (2 * (UI_16)fixed_signal_num) + (2 * (UI_16)vehicle_signal_num)) + (UI_16)D_BTC_FMT_FOOTER_BYTES);
}
/****************************************************************************/
/* �֐�����		:	btc_data_GetBytesRepStart								*/
/* �@�\�̐���	:	�u�ԗ����(�擾�J�n)�����v�f�[�^�T�C�Y�擾				*/
/* ����			:	signal_num		[i/ ]�M����								*/
/* ����			:	errcode_num		[i/ ]�G���[��							*/
/* �߂�l		:	�f�[�^�T�C�Y											*/
/* �쐬��		:	(OFT)Toguchi											*/
/* �쐬��		:	17/08/29												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL UI_16 btc_data_GetBytesRepStart( UI_8 signal_num, UI_8 errcode_num )
{
	return ((UI_16)D_BTC_FMT_HEADER_BYTES + (6 + (6 * (UI_16)signal_num) + (6 * (UI_16)errcode_num)) + (UI_16)D_BTC_FMT_FOOTER_BYTES);
}

/*==========================================================================*/
/* ��M�f�[�^��͊֐�														*/
/*==========================================================================*/
/****************************************************************************/
/* �֐�����		:	btc_data_AnalyzeRecvData								*/
/* �@�\�̐���	:	��M�f�[�^���											*/
/* ����			:	packet_info	��͂œ����p�P�b�g�̏��					*/
/* �߂�l		:	E_RC	��������										*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:	��M�f�[�^�Ȃ��ꍇ D_BTC_FMT_TYPE_INVALID ���o�͂���	*/
/****************************************************************************/
BTC_LOCAL E_RC btc_data_AnalyzeRecvData( BTC_PACKET_INFO* packet_info )
{
	E_RC	rc = E_RC_NG;
	SI_32	ret;

	if( NULL == packet_info ) {
		rc = E_RC_NG_PARAM;
	}
	else {
		/* �p�P�b�g���𖳌��l�ŏ����� */
		(void)memset( packet_info, 0, sizeof(BTC_PACKET_INFO) );
		packet_info->type = D_BTC_FMT_TYPE_INVALID;

		/* ��M�o�b�t�@�� START �}�[�N�܂œǂݔ�΂� */
		ret = btc_data_SkipToSTART();
		/* 1: START ���� */
		/* 0: START �Ȃ� */
		while( 1L == ret ) {
			/* �^�C�}�[�N���������� */
			if(btc_time_CheckTimerStartup( BTC_TIME_KIND_PACKET_FOUND ) == 0){
				/* �N�����łȂ���΃p�P�b�g���o������o�^ */
				btc_time_RegTime( BTC_TIME_KIND_PACKET_FOUND );
			}
			
			ret = btc_data_GetPacketInfo( packet_info );
			if( 1L == ret ) {
				/* 1:  �p�P�b�g���� */
				/* �p�P�b�g���o�������N���A */
				btc_time_ClearTime( BTC_TIME_KIND_PACKET_FOUND );
				break;
			}
			else if( 0L == ret ) {
				/* 0:  �p�P�b�g�ł͂Ȃ��i�ُ�p�P�b�g���܂ށj */
				/* START�}�[�N���΂��Ď��̉�� */
				(void)btc_ring_Read( &S_btc_recv_ring, 1U, NULL ); 
			}
			else {
				/* -1: �f�[�^�s���Ŕ���s�\ */
				break;
			}
			/* ��M�o�b�t�@�� START �}�[�N�܂œǂݔ�΂� */
			ret = btc_data_SkipToSTART();
		}
		rc = E_RC_OK;
	}
	return rc;
}
/****************************************************************************/
/* �֐�����		:	btc_data_SkipToSTART									*/
/* �@�\�̐���	:	��M�o�b�t�@�� START �}�[�N�܂œǂݔ�΂�				*/
/* ����			:	�Ȃ�													*/
/* �߂�l		:	1: START ����											*/
/* �߂�l		:	0: START �Ȃ�											*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL SI_32 btc_data_SkipToSTART( void )
{
	SI_32	ret = 0L;
	UI_16	data_bytes = 0L;
	UI_16	skip_bytes = 0L;
	UI_8	buf[16];	/* �K���ȃu���b�N�T�C�Y�ŏ��� */
	UI_16	buf_bytes =	sizeof(buf);

	/* �c��f�[�^�o�C�g���擾 */
	data_bytes = btc_ring_GetDataBytes( &S_btc_recv_ring );
	while( 0U < data_bytes ) {
		if( data_bytes > buf_bytes ) {
			data_bytes = buf_bytes;
		}
		/* �o�b�t�@�Q�� */
		(void)btc_ring_Ref( &S_btc_recv_ring, data_bytes, buf );

		/* START �}�[�N��T�� */
		skip_bytes = 0U;
		while( skip_bytes < data_bytes ) {
			if( D_BTC_FMT_HEADER_START == buf[skip_bytes] ) {
				/* ���� */
				ret = 1L;
				break;
			}
			skip_bytes++;
		}
		if( 0U < skip_bytes ) {
			/* �݂���Ȃ������f�[�^���o�b�t�@����̂Ă� */
			(void)btc_ring_Read( &S_btc_recv_ring, skip_bytes, NULL );
		}
		if( 1L == ret ) {
			/* ���[�v�𔲂��ďI�� */
			break;
		}
		/* �c��f�[�^�o�C�g���擾 */
		data_bytes = btc_ring_GetDataBytes( &S_btc_recv_ring );
	}
	return ret;
}
/****************************************************************************/
/* �֐�����		:	btc_data_GetPacketInfo									*/
/* �@�\�̐���	:	��M�o�b�t�@����͂��ăp�P�b�g���𓾂�				*/
/* ����			:	packet_info	��͂œ����p�P�b�g�̏��					*/
/* �߂�l		:	1:  �p�P�b�g����										*/
/* �߂�l		:	0:  �p�P�b�g�ł͂Ȃ��i�ُ�p�P�b�g���܂ށj				*/
/* �߂�l		:	-1: �f�[�^�s���Ŕ���s�\								*/
/* �߂�l		:	-2:	�����s��											*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/02												*/
/* ���l			:	�p�P�b�g���Ȃ��ꍇ D_BTC_FMT_TYPE_INVALID ���o�͂���	*/
/****************************************************************************/
BTC_LOCAL SI_32 btc_data_GetPacketInfo( BTC_PACKET_INFO* packet_info )
{
	SI_32	ret = 0L;
	UI_16	data_bytes = 0U;
	UI_16	buf_bytes = sizeof(packet_info->packet);
	UI_8*	p = NULL;
	UI_8*	header_part = NULL;
	UI_8*	data_part = NULL;
	UI_8*	footer_part = NULL;
	UI_16	packet_bytes = 0;
	UI_16	length = 0U;
	UI_16	crc = 0U;
	UI_16	crc_calc = 0U;
	UI_8	type = 0U;

	if( NULL == packet_info ) {
		ret = -2L;
	}
	else {
		/* �p�P�b�g���𖳌��l�ŏ����� */
		(void)memset( packet_info, 0, sizeof(BTC_PACKET_INFO) );
		packet_info->type = D_BTC_FMT_TYPE_INVALID;

		/* �����O�o�b�t�@�̃f�[�^���Q�� */
		data_bytes = btc_ring_GetDataBytes( &S_btc_recv_ring );
		if( data_bytes > buf_bytes ) {
			/* �p�P�b�g�o�b�t�@�̍ő吔���܂Ńf�[�^�Q�Ƃ��� */
			data_bytes = buf_bytes;
		}
		if( D_BTC_FMT_BYTES_FIX > data_bytes ) {
			/* �Œ蕔���̃T�C�Y�ɂ������Ȃ� */
			ret = -1L;
			goto END_ANALYZE;
		}
		(void)btc_ring_Ref( &S_btc_recv_ring, data_bytes, packet_info->packet );

		/* �p�P�b�g��� */
		p = packet_info->packet;

		/*==================================================================*/
		/* �w�b�_�̉��														*/
		/*==================================================================*/
		header_part = p;
		/* START */
		if( D_BTC_FMT_HEADER_START != *p ) {
			goto END_ANALYZE;
		}
		p++;
		/* LENGTH */
		length = Common_Get16( p );
		p += 2;
		/* TYPE */
		type = *p;
		p++;

		/* �p�P�b�g�S�̂̒������v�Z */
		packet_bytes = D_BTC_FMT_BYTES_FIX + length - 1U;
		if( packet_bytes > data_bytes ) {
			/* �p�P�b�g�S�̂̃T�C�Y�ɖ����Ȃ� */
			ret = -1L;
			goto END_ANALYZE;
		}
		/*==================================================================*/
		/* �f�[�^�̉��														*/
		/*==================================================================*/
		switch( type ) {
		case D_BTC_FMT_TYPE_REQ_LIST:
			/* �f�[�^���Ȃ� */
			break;
		case D_BTC_FMT_TYPE_REQ_START:
			/* �ԗ����(�擾�J�n)�v���p�P�b�g�̃f�[�^�����`�F�b�N */
			if( 1L != btc_data_CheckDataPartReqStart( p, length - 1U ) ) {
				/* �s���ȃf�[�^�� */
				goto END_ANALYZE;
			}
			break;
		case D_BTC_FMT_TYPE_REQ_STOP:
			/* �f�[�^���Ȃ� */
			break;
		default:
			/* �s���ȃf�[�^��� */
			goto END_ANALYZE;
		}
		data_part = p;
		p += (length - 1U);

		/*==================================================================*/
		/* �t�b�^�̉��														*/
		/*==================================================================*/
		footer_part = p;
		/* CRC */
		crc = Common_Get16( p );
		p += 2;
		/* END */
		if( D_BTC_FMT_FOOTER_END != *p ) {
			goto END_ANALYZE;
		}

		/* CRC�v�Z */
		crc_calc = Common_CalcCRC( header_part, 1U, length + 2 );
		if( crc_calc != crc ) {
			/* CRC �s��v */
			goto END_ANALYZE;
		}
		/*==================================================================*/
		/* ��͌��ʂ��i�[													*/
		/*==================================================================*/
		packet_info->packet_bytes = packet_bytes;
		packet_info->length = length;
		packet_info->type = type;
		packet_info->header_part = header_part;
		packet_info->data_part = data_part;
		packet_info->footer_part = footer_part;

		/* �p�P�b�g���݂����� */
		ret = 1L;
	}
END_ANALYZE:
	return ret;
}
/****************************************************************************/
/* �֐�����		:	btc_data_GetSendSignalInfo								*/
/* �@�\�̐���	:	�ԗ����(�擾�J�n)�v���p�P�b�g�̃f�[�^�����`�F�b�N		*/
/* ����			:	data_part	i: �f�[�^���擪�A�h���X						*/
/* ����			:	length		i: �f�[�^���S�̂̃T�C�Y(�o�C�g��)			*/
/* �߂�l		:	1:  ����I���i�f�[�^���͐���j							*/
/* �߂�l		:	0:  ����I���i�f�[�^���s���j							*/
/* �߂�l		:	-1: �����s��											*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/17												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL SI_32 btc_data_CheckDataPartReqStart( const UI_8* data_part, UI_16 length )
{
	SI_32			ret = -1L;
	UI_8			num = 0U;

	if( NULL != data_part ) {
		/* �������� */
		ret = 0L;

		if( 0U < length ) {
			/* �f�[�^����擪����`�F�b�N */
			num = *data_part;
			if( (0U < num) && (num <= D_BTC_MAX_SIGNAL_COUNT) &&
				(length == (UI_16)(1U + (num * sizeof(UI_16)))) ) {
				/* �f�[�^�����P�ȏ㐧�����ȉ��ł��蒷���ƃf�[�^�������v���Ă��� */
				ret = 1L;
			}
		}
	}
	return ret;
}
/****************************************************************************/
/* �֐�����		:	btc_data_GetSendSignalInfo								*/
/* �@�\�̐���	:	���M�M�����擾										*/
/* ����			:	packet_info	��͂œ����p�P�b�g�̏��					*/
/* �߂�l		:	0:  ����I��											*/
/* �߂�l		:	-1: �����s���i�p�P�b�g��ʂ��s���ȏꍇ�܂ށj			*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/12												*/
/* ���l			:	packet_info	�ɂ� D_BTC_FMT_TYPE_REQ_START �p�P�b�g��n��*/
/****************************************************************************/
BTC_LOCAL SI_32 btc_data_GetSendSignalInfo( const BTC_PACKET_INFO* packet_info )
{
	SI_32	ret = -1L;

	if( NULL != packet_info ) {
		if( D_BTC_FMT_TYPE_REQ_START == packet_info->type ) {
			/* ���M�M��ID�̐� */
			S_btc_send_signal_id_num = *(packet_info->data_part);
			if( D_BTC_MAX_SIGNAL_COUNT < S_btc_send_signal_id_num ) {
				/* �ő吔�𒴂��Ă���Ƃ��͊ۂ߂� */
				S_btc_send_signal_id_num = D_BTC_MAX_SIGNAL_COUNT;
			}
			/* ���M�M��ID�z��𒊏o */
			(void)memcpy( S_btc_send_signal_id_array, packet_info->data_part + 1, (size_t)(S_btc_send_signal_id_num * sizeof(UI_16)) );
			
			ret = 0L;
		}
	}
	return ret;
}
/****************************************************************************/
/* �֐�����		:	btc_data_ClearRecvPacket								*/
/* �@�\�̐���	:	��M�p�P�b�g�N���A										*/
/* ����			:	packet_info	��͂œ����p�P�b�g�̏��					*/
/* �߂�l		:	0:  ����I��											*/
/* �߂�l		:	-1: �����s��											*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/12												*/
/* ���l			:	packet_info �������p�P�b�g���o�b�t�@�����菜��		*/
/****************************************************************************/
BTC_LOCAL SI_32 btc_data_ClearRecvPacket( BTC_PACKET_INFO* packet_info )
{
	SI_32	ret = -1L;

	if( NULL != packet_info ) {
		/* ��M�p�P�b�g�������O�o�b�t�@�����菜�� */
		(void)btc_ring_Read( &S_btc_recv_ring, packet_info->packet_bytes, NULL );

		(void)memset( packet_info, 0, sizeof(BTC_PACKET_INFO) );

		ret = 0L;
	}
	return ret;
}
/*==========================================================================*/
/* ���Ԋ֘A																	*/
/*==========================================================================*/
/****************************************************************************/
/* �֐�����		:	btc_time_RegTime										*/
/* �@�\�̐���	:	������o�^												*/
/* ����			:	kind	�������										*/
/* �߂�l		:	�Ȃ�													*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/03												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL void	btc_time_RegTime( E_BTC_TIME_KIND kind )
{
	UI_32	flag = (1UL << kind);
	
	if( kind < BTC_TIME_KIND_COUNT ) {
		S_btc_time_info.time[kind] = Timer_ctrl_GetTime10ms();
		S_btc_time_info.flags |= flag;
	}
}
/****************************************************************************/
/* �֐�����		:	btc_time_ClearTime										*/
/* �@�\�̐���	:	�������N���A											*/
/* ����			:	kind	�������										*/
/* �߂�l		:	�Ȃ�													*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/03												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL void	btc_time_ClearTime( E_BTC_TIME_KIND kind )
{
	UI_32	flag = (1UL << kind);

	if( kind < BTC_TIME_KIND_COUNT ) {
		S_btc_time_info.time[kind] = 0;
		S_btc_time_info.flags &= (~flag);
	}
}
/****************************************************************************/
/* �֐�����		:	btc_time_CheckTimeout									*/
/* �@�\�̐���	:	�^�C���A�E�g�`�F�b�N									*/
/* ����			:	kind			�������								*/
/* ����			:	timeout_10msec	�^�C���A�E�g����(1�J�E���g10msec)		*/
/* �߂�l		:	1: �^�C���A�E�g���o										*/
/* �߂�l		:	0: �^�C���A�E�g�Ȃ�										*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/03												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL SI_32	btc_time_CheckTimeout( E_BTC_TIME_KIND kind, UI_16 timeout_10msec )
{
	UI_32	flag = (1UL << kind);
	SI_32	ret = 0L;

	if( kind < BTC_TIME_KIND_COUNT ) {
		if( 0UL != (S_btc_time_info.flags & flag) ) {
			/* �����L���Ȃ�`�F�b�N */
			if (Timer_ctrl_TimeCompare10ms(S_btc_time_info.time[kind], timeout_10msec) == N_OK) {
				/* �^�C���A�E�g���o */
				ret = 1L;
			}
		}
	}
	return ret;
}

/****************************************************************************/
/* �֐�����		:	btc_time_CheckTimerStartup								*/
/* �@�\�̐���	:	�^�C�}�[�N��������										*/
/* ����			:	kind	�������										*/
/* �߂�l		:	1: �^�C�}�[�N����										*/
/* �߂�l		:	0: �^�C�}�[���N��										*/
/* �쐬��		:	(OFT)Sakata												*/
/* �쐬��		:	17/08/28												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL SI_32	btc_time_CheckTimerStartup( E_BTC_TIME_KIND kind )
{
	UI_32	flag = (1UL << kind);
	SI_32	ret = 0L;

	if( kind < BTC_TIME_KIND_COUNT ) {
		if( 0UL != (S_btc_time_info.flags & flag) ){
			ret = 1L;
		}
	}
	return ret;
}
/*==========================================================================*/
/* �R�[���o�b�N�֐�															*/
/*==========================================================================*/
/****************************************************************************/
/* �֐�����		:	btc_cb_mdlReceiveData									*/
/* �@�\�̐���	:	BT�~�h���R�[���o�b�N�֐�: �f�[�^��M					*/
/* ����			:	buf	��M�f�[�^											*/
/* ����			:	len	��M�f�[�^��										*/
/* �߂�l		:	�Ȃ�													*/
/* �쐬��		:	(OFT)Kamakura											*/
/* �쐬��		:	16/09/03												*/
/* ���l			:															*/
/****************************************************************************/
BTC_LOCAL void btc_cb_mdlReceiveData( const UI_8 *buf, UI_16 len )
{
	E_RC	rc;

	if( NULL != buf ) {
		/* ��M�f�[�^�������O�o�b�t�@�ɋL�^ */
		rc = btc_ring_Write( &S_btc_recv_ring, len, buf );
		if( E_RC_OK != rc ) {
			/* �ُ픭�� */
			OUTLOG(OutputLog( "[BTC] btc_ring_Write ERROR(%d:len=%d)\n", rc, len );)
		}
	}
}
