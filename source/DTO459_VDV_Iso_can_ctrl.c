/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: FR81S								*/
/* Date				: 16/08/18							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Katsura						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		:									*/
/* date				:									*/
/********************************************************/
/********************************************************/
/* file DTO459_VDV_Iso_canctrl.c						*/
/* ISO CAN����											*/
/********************************************************/

#ifdef ISOCAN_ENABLE
/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include <type.h>
#include <system.h>
#include <string.h>
#include "DTO459_VDV_Iso_can_ctrl.h"
#include "DTO459_VDV_Can_Drv.h"
#include "timer_ctrl.h"
#include "dbglog.h"

/********************************************************/
/*	�}�N����`											*/
/********************************************************/
#define CAN_CH_ISO_CAN					CAN_CH_0		/* ISO CAN��CAN�`�����l�� 		*/

#define ISO_CAN_SEND_BUFFER_COUNT		10				/* ISO CAN�̑��M�o�b�t�@��			*/
#define ISO_CAN_MSG_ID_RX_MIN			(ISO_CAN_SEND_BUFFER_COUNT + 1)	/* ISO CAN�̎�M�p���b�Z�[�WID�ŏ��l */

#define INIT_LOOP_CNT					0				/**< (for)���[�v�p�ϐ������l			*/
#define INIT_GROUP1_NUM					0				/**< group1_number�̏����l				*/
#define INIT_GROUP2_NUM					0				/**< group2_number�̏����l				*/
#define INIT_GROUP3_NUM					0				/**< group3_number�̏����l				*/
#define INIT_VAL_SUM					0				/**< sum�̏����l						*/
#define SHIFT_1BIT						1				/**< 1�r�b�g�V�t�g						*/
#define DATA1_SHIFT						8				/**< 8�r�b�g�V�t�g(CAN�f�[�^���i�[)		*/
#define DATA2_SHIFT						8				/**< 8�r�b�g�V�t�g(CAN�f�[�^���i�[)		*/
#define DATA3_SHIFT						8				/**< 8�r�b�g�V�t�g(CAN�f�[�^���i�[)		*/
#define JUDGE_REQ_BIT					0x8000U			/**< ���N�G�X�g����pMASK�r�b�g			*/
#define NO_REQUEST						0				/**< ���N�G�X�g�Ȃ�						*/
#define TAKEOUT_HIGH_4BIT				4				/**< ���4�r�b�g���o��				*/
#define TAKEOUT_LOW_4BIT				0x0F			/**< ����4�r�b�g���o��				*/
#define TAKEOUT_HIGH_16BIT				16				/**< ���16�r�b�g���o��				*/
#define TAKEOUT_LOW_16BIT				0x0000FFFFUL		/**< ����16�r�b�g���o��				*/
#define NO_RESPONSE						0				/**< ���X�|���X����						*/
#define GR2_EFF_REQ_0					0				/**< �O���[�v2�L�����N�G�X�g����		*/
#define GR3_EFF_REQ_0					0				/**< �O���[�v3�L�����N�G�X�g����		*/
#define JUDGE_WAIT_TIME					100				/**< CAN�ʐM�҂�����(100ms)				*/
#define JUDGE_RETRY_WAIT_TIME			5000			/**< CAN�ڑ��m�F���g���C�҂�����(5s)	*/
#define JUDGE_CONNECT_START_WAIT_TIME	500				/**< �ڑ��m�F�J�n�҂�����(500ms)		*/

/* type 1 ���N�G�X�g */
#define CAN_ID_TYPE_1_REQ_CAN_ID_ADDRESS	0x7DFUL		/* CAN ID �A�h���X���N�G�X�g	*/
#define CAN_ID_TYPE_1_REQ_CAN_ID_ECU_1		0x7E0UL		/* CAN ID ���N�G�X�g ECU #1		*/
#define CAN_ID_TYPE_1_RES_CAN_ID_ECU_1		0x7E8UL		/* CAN ID ���X�|���X ECU #1		*/
#define CAN_ID_TYPE_1_REQ_CAN_ID_ECU_8		0x7E7UL		/* CAN ID ���N�G�X�g ECU #8		*/
#define CAN_ID_TYPE_1_RES_CAN_ID_ECU_8		0x7EFUL		/* CAN ID ���X�|���X ECU #8		*/

/* type 2 ���N�G�X�g */
#define CAN_ID_TYPE_2_REQ_CAN_ID			0x18DB33F1UL	/* CAN ID Function���N�G�X�g ECUs with #33			*/
#define CAN_ID_TYPE_2_RES_CAN_ID			0x18DAF100UL	/* CAN ID ���X�|���X�i�ŉ��ʃo�C�g�����j			*/
#define CAN_ID_TYPE_2_REQ_TEMP				0x18DA00F1UL	/* �M�����N�G�X�g�̃e���v���[�g�i00�̉ӏ���ECU�ԍ��j*/
#define CAN_ID_TYPE_2_RES_TEMP				0x18DAF100UL	/* �M�����X�|���X�̃e���v���[�g�i00�̉ӏ���ECU�ԍ��j*/

/* ���[�h */
#define MODE_REQ_NORMAL						0x01		/* �ʏ탊�N�G�X�g				*/
#define MODE_REQ_TOYOTA						0x21		/* TOYOTA���N�G�X�g				*/
#define MODE_RES_NORMAL						0x41		/* �ʏ탌�X�|���X				*/
#define MODE_RES_TOYOTA						0x61		/* TOYOTA���X�|���X				*/

/* �e�D�揇�ʂɂ�����PID�� */ 
#define MAX_COUNT_PRIORITY_1_PID			2			/* �D��P�̍ő�PID��			*/
#define MAX_COUNT_PRIORITY_2_PID			5			/* �D��Q�̍ő�PID��			*/
#define	MAX_REQUEST_NUMBER					(MAX_COUNT_PRIORITY_1_PID+MAX_COUNT_PRIORITY_2_PID)
#define SUP_PID_INFO_MAX_COUNT				10			/* PID�Ή����ő吔 			*/

/* PID */ 
#define PID_SUPPORT_INFO_COUNT				3			/* PID�T�|�[�g���					*/
#define PID_SUPPORT_INFO_00					0x00		/* PID 01-20 �T�|�[�g��� 				*/
#define PID_SUPPORT_INFO_20					0x20		/* PID 21-40 �T�|�[�g��� 				*/
#define PID_SUPPORT_INFO_40					0x40		/* PID 41-60 �T�|�[�g��� 				*/
#define	PID_ENG_COOL_TEMP					0x05		/**< engine coolant temperature			*/
#define	PID_IMA_PRESSURE					0x0B		/**< intake manifold absolute pressure	*/
#define	PID_ENG_RPM							0x0C		/**< engine rpm							*/
#define	PID_VEHICLE_SPEED					0x0D		/**< vehicle speed						*/
#define	PID_INTAKE_AIR_TEMP					0x0F		/**< intake air temperature				*/
#define	PID_THROTTLE_POS					0x11		/**< throttle position					*/
#define	PID_CTRL_MOD_VOLT					0x42		/**< control module voltage				*/

/* ��͏�� */
enum e_crrespondence_num{
	CORRESPONDENCE_NUM0 = 0,							/**< CORRESPONDENCE��0�Ԗ�				*/
	CORRESPONDENCE_NUM1,								/**< CORRESPONDENCE��1�Ԗ�				*/
	CORRESPONDENCE_NUM2,								/**< CORRESPONDENCE��2�Ԗ�				*/
	CORRESPONDENCE_NUM3,								/**< CORRESPONDENCE��3�Ԗ�				*/
	CORRESPONDENCE_NUM4,								/**< CORRESPONDENCE��4�Ԗ�				*/
	CORRESPONDENCE_NUM5,								/**< CORRESPONDENCE��5�Ԗ�				*/
	CORRESPONDENCE_NUM_MAX								/**< CRRESPONDENCE�̍ő吔				*/
};

/* ���䃂�[�h */
typedef enum e_ccc_mode {
	CCC_MODE_WAIT_CONNECT = 0,							/**< �ڑ��m�F�҂����					*/
	CCC_MODE_GET_PID,									/**< �Ή�PID�擾���					*/
	CCC_MODE_REQUEST_PID,								/**< PID���N�G�X�g���M���				*/
	CCC_MODE_CONNECT_ERROR,								/**< �ʐM�G���[���						*/
	CCC_MODE_MAX										/**< �����Ԑ�							*/
} E_CCC_MODE;

/* PID���[�h */
typedef enum e_ccc_pid_mode {
	CCC_PID_STD_CAN_1A = 0,								/**< TYPE-1A�ڑ��m�F					*/
	CCC_PID_STD_CAN_1B,									/**< TYPE-1B�ڑ��m�F					*/
	CCC_PID_STD_CAN_1T,									/**< TYPE-1T�ڑ��m�F					*/
	CCC_PID_EXT_CAN,									/**< TYPE-2�ڑ��m�F						*/
	CCC_PID_PID_INFO,									/**< �Ή�PID�擾						*/
	CCC_PID_MAKE_REQUEST,								/**< ���N�G�X�g�D�揇�ʍ쐬				*/
	CCC_PID_WAIT,										/**< CAN���N�G�X�g���M�҂�				*/
	CCC_PID_MAX											/**< ��Ԋ֐���							*/
} E_CCC_PID_MODE;

/* ���N�G�X�g�^�C�v */
typedef enum {
	ISO_CAN_REQ_TYPE_1A = 0,							/* Type 1A								*/
	ISO_CAN_REQ_TYPE_1B,								/* Type 1B								*/
	ISO_CAN_REQ_TYPE_1T,								/* Type 1T								*/
	ISO_CAN_REQ_TYPE_2									/* Type 2								*/
} E_ISO_CAN_REQ_TYPE;

typedef enum {
	ISO_CAN_RESP_ACK = 0,
	ISO_CAN_RESP_NACK,
	ISO_CAN_RESP_NONE
} E_ISO_CAN_RESPONSE;

/********************************************************/
/*	�����\���̒�`										*/
/********************************************************/
/* PID�Ή���� */
typedef struct {
	UI_8		ecu_no;								/* ECU�ԍ�						*/
	UI_8		pid_supported[4];					/* PID�Ή����ꗗ				*/
} SUPPORTED_PID_INFO;

/* CAN���(UI_8�p) */
typedef struct t_can_info_ui8 {
	UI_8	data;					/* �f�[�^			*/
	UI_8	first_flg;				/* �����M�t���O	*/
	UI_8	fail_flg;				/* �t�F�[���t���O	*/
} T_CanInfo_UI8;

/* CAN���(UI_16�p) */
typedef struct t_can_info_ui16 {
	UI_16	data;					/* �f�[�^			*/
	UI_8	first_flg;				/* �����M�t���O	*/
	UI_8	fail_flg;				/* �t�F�[���t���O	*/
} T_CanInfo_UI16;

/********************************************************/
/*	�����萔��`										*/
/********************************************************/
/* CAN-DATA request index */
static const UI_8 C_CCC_REQ_INDEX[3] ={
	PID_SUPPORT_INFO_00,						/**< request 0x01 to 0x1F				*/
	PID_SUPPORT_INFO_20,						/**< request 0x21 to 0x3F				*/
	PID_SUPPORT_INFO_40							/**< request 0x41 to 0x5F				*/
};

/* CAN-DATA response offset */
static const UI_8 C_CCC_RES_OFFSET[3] = {
	0, 2, 4										/**< �Ή�PID���X�|���X buff�i�[�poffset	*/
};

/* PID�D�揇��:�D�揇�ʂ̍������ɔz�u */
static const UI_8 C_correspondence[MAX_REQUEST_NUMBER] = {
	PID_ENG_RPM,										/**< engine rpm							*/
	PID_IMA_PRESSURE,									/**< intake manifold absolute pressure	*/
	PID_THROTTLE_POS,									/**< throttle position					*/
	PID_VEHICLE_SPEED,									/**< vehicle speed						*/
	PID_CTRL_MOD_VOLT,									/**< control module voltage				*/
	PID_ENG_COOL_TEMP,									/**< engine coolant temperature			*/
	PID_INTAKE_AIR_TEMP,								/**< intake air temperature				*/
};

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
static UI_16 S_correspondence[CORRESPONDENCE_NUM_MAX];	/**< CAN��M��̓f�[�^					*/
static UI_8 S_pid_req_count;							/**< PID���N�G�X�g�J�E���g				*/
static UI_32 S_can_id;									/**< CAN-ID								*/
static UI_8 S_ccc_mode;									/**< ���䃂�[�h							*/
static UI_8 S_ccc_pid_mode;								/**< PID���[�h							*/
static UI_32 S_retry_wait_time;							/**< CAN���N�G�X�g���g���C�҂�����		*/
static UI_32 S_connect_stat_wait_time;					/**< �ڑ��m�F�J�n�҂�����				*/
static UI_32 S_connect_time;							/**< �Ή�PID���N�G�X�g�ҋ@����			*/

static UI_8 S_tx_flg;									/**< CAN���N�G�X�g���M�t���O			*/
static UI_8 S_pid_seq;									/**< �Ή�PID�擾�V�[�P���X�ԍ�			*/
static UI_8 S_connect_check_cnt;						/**< �ڑ��m�F��						*/

static UI_8					S_target_ecu_no;			/* �ʐM��ECU�ԍ�(Type 1B/Type 2�ɂĎg�p)*/
static E_ISO_CAN_REQ_TYPE	S_connection_type;			/* �ڑ��^�C�v							*/

static UI_8					S_sup_pid_info_count;					/* �^�C�v2 PID�Ή����	*/
static SUPPORTED_PID_INFO	S_sup_pid_info[SUP_PID_INFO_MAX_COUNT];	/* �^�C�v2 PID�Ή����z��	*/

static UI_8 S_priority_1_pid_count;						/* �D��P��PID��						*/
static UI_8 S_priority_2_pid_count;						/* �D��Q��PID��						*/
static UI_8 S_priority_1_pid_index;						/* �D��P��PID��̃C���f�b�N�X			*/
static UI_8 S_priority_2_pid_index;						/* �D��Q��PID��̃C���f�b�N�X			*/
static UI_8 S_priority_1_pid[MAX_COUNT_PRIORITY_1_PID];	/* �D��P��PID��						*/
static UI_8 S_priority_2_pid[MAX_COUNT_PRIORITY_2_PID];	/* �D��Q��PID��						*/

/* �e��CAN�M����� */
static T_CanInfo_UI8  S_can_info_eng_cool_temp;				/* Engine coolant temperature			*/
static T_CanInfo_UI8  S_can_info_im_abs_press;				/* Intake manifold absolute presure		*/
static T_CanInfo_UI16 S_can_info_eng_rpm;					/* Engine RPM							*/
static T_CanInfo_UI8  S_can_info_vehicle_speed;				/* Vehicle speed						*/
static T_CanInfo_UI8  S_can_info_intake_air_temp;			/* Intake air temperature				*/
static T_CanInfo_UI8  S_can_info_throttle_pos;				/* Throttle position					*/
static T_CanInfo_UI16 S_can_info_ctrl_mod_volt;				/* Control module voltage				*/

/********************************************************/
/*	�����֐�											*/
/********************************************************/
static void S_ccc_set_mode( const UI_8 mode );

/*
 *	��Ԋ֐������䃂�[�h
 */
static void S_ccc_mode_wait_connect( void );			/**< CCC_MODE_WAIT_CONNECT				*/
static void S_ccc_mode_get_pid( void );					/**< CCC_MODE_GET_PID					*/
static void S_ccc_mode_request_pid( void );				/**< CCC_MODE_REQUEST_PID				*/
static void S_ccc_mode_connect_error( void );			/**< CCC_MODE_CONNECT_ERROR				*/

/*
 *	��Ԋ֐���PID���[�h
 */
static void S_ccc_pid_std_can_1a( void );				/**< CCC_PID_STD_CAN_1A					*/
static void S_ccc_pid_std_can_1b( void );				/**< CCC_PID_STD_CAN_1B					*/
static void S_ccc_pid_std_can_1t( void );				/**< CCC_PID_STD_CAN_1T					*/
static void S_ccc_pid_ext_can( void );					/**< CCC_PID_EXT_CAN					*/
static void S_ccc_pid_info( void );						/**< CCC_PID_PID_INFO					*/
static void S_ccc_pid_make_request( void );				/**< CCC_PID_MAKE_REQUEST				*/
static void S_ccc_pid_wait( void );						/**< CCC_PID_WAIT						*/

static E_RC send_request(E_ISO_CAN_REQ_TYPE req_type, UI_32 can_id, UI_8 pid);
static E_RC receive_response(E_ISO_CAN_REQ_TYPE req_type, UI_8 pid, UI_32 *can_id, E_ISO_CAN_RESPONSE *response, SUPPORTED_PID_INFO *sup_pid_info);
static UI_8 select_ecu_for_type_2(void);
static UI_8 get_enable_pid_count(const UI_8 *pid_supported);
static E_RC  parse_response_data(const T_Can_data *t_can_data, UI_8 *pid, UI_8 *value_data_len, UI_8 *value_data);
static void update_signal_value(UI_8 pid, UI_8 value_data_len, const UI_8 *value_data);
static UI_32 get_resp_msg_id(E_ISO_CAN_REQ_TYPE connection_type);
static UI_8 get_value_data_len(UI_8 pid);

/* �����萔��` */
/*
 *	���䃂�[�h�e�[�u��
 */
static void ( * const C_ccc_mode_func[CCC_MODE_MAX] ) ( void ) = {
	S_ccc_mode_wait_connect,							/**< CCC_MODE_WAIT_CONNECT				*/
	S_ccc_mode_get_pid,     							/**< CCC_MODE_GET_PID					*/
	S_ccc_mode_request_pid, 							/**< CCC_MODE_REQUEST_PID				*/
	S_ccc_mode_connect_error							/**< CCC_MODE_CONNECT_ERROR				*/
};

/*
 *	PID���[�h�e�[�u��
 */
static void ( * const C_ccc_pid_func[CCC_PID_MAX] ) ( void ) ={
	S_ccc_pid_std_can_1a,								/**< CCC_PID_STD_CAN_1A					*/
	S_ccc_pid_std_can_1b,								/**< CCC_PID_STD_CAN_1B					*/
	S_ccc_pid_std_can_1t,								/**< CCC_PID_STD_CAN_1T					*/
	S_ccc_pid_ext_can,									/**< CCC_PID_EXT_CAN					*/
	S_ccc_pid_info,										/**< CCC_PID_PID_INFO					*/
	S_ccc_pid_make_request,								/**< CCC_PID_MAKE_REQUEST				*/
	S_ccc_pid_wait,										/**< CCC_PID_WAIT						*/
};


/********************************************************/
/*	�O���֐�											*/
/********************************************************/
/********************************************************/
/* �֐�����		: Iso_can_ctrl_get_eng_cool_temp		*/
/* �@�\�̐���	: �G���W���������擾					*/
/* ����			: (O)data:�G���W���������				*/
/* �߂�l		: �擾����								*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/11/08								*/
/* ���l			: 										*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_eng_cool_temp(UI_8 *data)
{
	E_ISO_CAN_RESULT	result;
	
	if (S_can_info_eng_cool_temp.first_flg == 0) {
		result = ISO_CAN_CTRL_NO_RECEIVE;
	} else if (S_can_info_eng_cool_temp.fail_flg == 1) {
		result = ISO_CAN_CTRL_ERROR;
	} else {
		result = ISO_CAN_CTRL_OK;
	}
	*data = S_can_info_eng_cool_temp.data;
	
	return result;
}

/********************************************************/
/* �֐�����		: Iso_can_ctrl_get_im_abs_press			*/
/* �@�\�̐���	: �z�C�}�j�t�H�[���h�����擾			*/
/* ����			: (O)data:�z�C�}�j�t�H�[���h�����		*/
/* �߂�l		: �擾����								*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/11/08								*/
/* ���l			: 										*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_im_abs_press(UI_8 *data)
{
	E_ISO_CAN_RESULT	result;
	
	if (S_can_info_im_abs_press.first_flg == 0) {
		result = ISO_CAN_CTRL_NO_RECEIVE;
	} else if (S_can_info_im_abs_press.fail_flg == 1) {
		result = ISO_CAN_CTRL_ERROR;
	} else {
		result = ISO_CAN_CTRL_OK;
	}
	*data = S_can_info_im_abs_press.data;
	
	return result;
}

/********************************************************/
/* �֐�����		: Iso_can_ctrl_get_eng_rpm				*/
/* �@�\�̐���	: �G���W����]�����擾				*/
/* ����			: (O)data:�G���W����]�����			*/
/* �߂�l		: �擾����								*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/11/08								*/
/* ���l			: 										*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_eng_rpm(UI_16 *data)
{
	E_ISO_CAN_RESULT	result;
	
	if (S_can_info_eng_rpm.first_flg == 0) {
		result = ISO_CAN_CTRL_NO_RECEIVE;
	} else if (S_can_info_eng_rpm.fail_flg == 1) {
		result = ISO_CAN_CTRL_ERROR;
	} else {
		result = ISO_CAN_CTRL_OK;
	}
	*data = S_can_info_eng_rpm.data;
	
	return result;
}

/********************************************************/
/* �֐�����		: Iso_can_ctrl_get_vehicle_speed		*/
/* �@�\�̐���	: �ԑ����擾							*/
/* ����			: (O)data:�ԑ����						*/
/* �߂�l		: �擾����								*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/11/08								*/
/* ���l			: 										*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_vehicle_speed(UI_8 *data)
{
	E_ISO_CAN_RESULT	result;
	
	if (S_can_info_vehicle_speed.first_flg == 0) {
		result = ISO_CAN_CTRL_NO_RECEIVE;
	} else if (S_can_info_vehicle_speed.fail_flg == 1) {
		result = ISO_CAN_CTRL_ERROR;
	} else {
		result = ISO_CAN_CTRL_OK;
	}
	*data = S_can_info_vehicle_speed.data;
	
	return result;
}

/********************************************************/
/* �֐�����		: Iso_can_ctrl_get_intake_air_temp		*/
/* �@�\�̐���	: �z�C�����擾						*/
/* ����			: (O)data:�z�C�����					*/
/* �߂�l		: �擾����								*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 16/12/27								*/
/* ���l			: 										*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_intake_air_temp(UI_8 *data)
{
	E_ISO_CAN_RESULT	result;
	
	if (S_can_info_intake_air_temp.first_flg == 0) {
		result = ISO_CAN_CTRL_NO_RECEIVE;
	} else if (S_can_info_intake_air_temp.fail_flg == 1) {
		result = ISO_CAN_CTRL_ERROR;
	} else {
		result = ISO_CAN_CTRL_OK;
	}
	*data = S_can_info_intake_air_temp.data;
	
	return result;
}

/********************************************************/
/* �֐�����		: Iso_can_ctrl_get_throttle_pos			*/
/* �@�\�̐���	: �X���b�g���|�W�V�������擾			*/
/* ����			: (O)data:�X���b�g���|�W�V�������		*/
/* �߂�l		: �擾����								*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/11/08								*/
/* ���l			: 										*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_throttle_pos(UI_8 *data)
{
	E_ISO_CAN_RESULT	result;
	
	if (S_can_info_throttle_pos.first_flg == 0) {
		result = ISO_CAN_CTRL_NO_RECEIVE;
	} else if (S_can_info_throttle_pos.fail_flg == 1) {
		result = ISO_CAN_CTRL_ERROR;
	} else {
		result = ISO_CAN_CTRL_OK;
	}
	*data = S_can_info_throttle_pos.data;
	
	return result;
}

/********************************************************/
/* �֐�����		: Iso_can_ctrl_get_ctrl_mod_volt		*/
/* �@�\�̐���	: �d�����擾							*/
/* ����			: (O)data:�d�����						*/
/* �߂�l		: �擾����								*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 16/12/27								*/
/* ���l			: 										*/
/********************************************************/
E_ISO_CAN_RESULT Iso_can_ctrl_get_ctrl_mod_volt(UI_16 *data)
{
	E_ISO_CAN_RESULT	result;
	
	if (S_can_info_ctrl_mod_volt.first_flg == 0) {
		result = ISO_CAN_CTRL_NO_RECEIVE;
	} else if (S_can_info_ctrl_mod_volt.fail_flg == 1) {
		result = ISO_CAN_CTRL_ERROR;
	} else {
		result = ISO_CAN_CTRL_OK;
	}
	*data = S_can_info_ctrl_mod_volt.data;
	
	return result;
}

/********************************************************/
/* �֐�����		: Iso_can_ctrl_Init						*/
/* �@�\�̐���	: ISO CAN���䏉����						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/10/24								*/
/* ���l			: 										*/
/********************************************************/
void Iso_can_ctrl_Init(void)
{
	UI_32 i;
	
	S_pid_req_count = 0;
	S_ccc_mode = CCC_MODE_WAIT_CONNECT;
	S_ccc_pid_mode = CCC_PID_STD_CAN_1A;
	S_tx_flg = N_OFF;
	S_pid_seq = 0;
	S_connect_check_cnt = 0;
	S_connect_time = 0;
	S_retry_wait_time = 0;
	S_connect_stat_wait_time = 0;
	S_connection_type = ISO_CAN_REQ_TYPE_1A;
	S_target_ecu_no = 0;
	S_sup_pid_info_count = 0;

	for(i = 0; i < CORRESPONDENCE_NUM_MAX; i++)
	{
		S_correspondence[i] = 0;
	}

	/* PID�D��e�[�u���N���A */
	S_priority_1_pid_count = 0;
	S_priority_2_pid_count = 0;
	S_priority_1_pid_index = 0;
	S_priority_2_pid_index = 0;
	for(i = 0; i < (sizeof(S_priority_1_pid) / sizeof(UI_8)); i++) {
		S_priority_1_pid[i] = 0;
	}
	for(i = 0; i < (sizeof(S_priority_2_pid) / sizeof(UI_8)); i++) {
		S_priority_2_pid[i] = 0;
	}

	/* �e��CAN�M����񏉊��� */
	(void)memset(&S_can_info_eng_cool_temp,		0, sizeof(S_can_info_eng_cool_temp));
	(void)memset(&S_can_info_im_abs_press,		0, sizeof(S_can_info_im_abs_press));
	(void)memset(&S_can_info_eng_rpm,			0, sizeof(S_can_info_eng_rpm));
	(void)memset(&S_can_info_vehicle_speed,		0, sizeof(S_can_info_vehicle_speed));
	(void)memset(&S_can_info_intake_air_temp,	0, sizeof(S_can_info_intake_air_temp));
	(void)memset(&S_can_info_throttle_pos,		0, sizeof(S_can_info_throttle_pos));
	(void)memset(&S_can_info_ctrl_mod_volt,		0, sizeof(S_can_info_ctrl_mod_volt));

	/* CAN�h���C�o�̏����� */
	Can_drv_ch_init(CAN_CH_ISO_CAN, ISO_CAN_SEND_BUFFER_COUNT);
}

/********************************************************/
/* �֐�����		: Iso_can_ctrl_Main						*/
/* �@�\�̐���	: ISO CAN���䃁�C��						*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/10/24								*/
/* ���l			: 										*/
/********************************************************/
void Iso_can_ctrl_Main(void)
{
	/**	<ol>	*/
	/**	<li>��Ԋ֐����Ăяo��	*/
	C_ccc_mode_func[S_ccc_mode]();

	/**	</ol>	*/
	return;
}

/********************************************************/
/*	�����֐�											*/
/********************************************************/
/********************************************************
 * ���䃂�[�h�ݒ菈��
 * 
 * param	mode	[in]	���䃂�[�h
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		���䃂�[�h���X�V����D
********************************************************/
static void S_ccc_set_mode( const UI_8 mode )
{
	if (S_ccc_mode != mode) {
		if (mode == (UI_8)CCC_MODE_REQUEST_PID) {
			S_pid_req_count = 0;
			S_priority_1_pid_index = 0;
			S_priority_2_pid_index = 0;
		}
		OUTLOG(OutputLog("[ISOCAN]CtrlMode:%d->%d\n", S_ccc_mode, mode);)
	}

	/**	<ol>	*/
	/**	<li>�w�肳�ꂽ���䃂�[�h������ϐ��ɕێ�	*/
	S_ccc_mode = mode;
	/**	</ol>	*/
	return;
}

/********************************************************
 * ���䃂�[�h:�ڑ��m�F�҂����
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note	
********************************************************/
static void S_ccc_mode_wait_connect( void )
{
	if (S_connect_stat_wait_time == 0) {
		S_connect_stat_wait_time = Timer_ctrl_GetTime1ms();
	}

	/* �ڑ��m�F�J�n�҂� */
	if (Timer_ctrl_TimeCompare1ms(S_connect_stat_wait_time, JUDGE_CONNECT_START_WAIT_TIME) == N_OK) {
		/**	<li>[�Ή�PID�擾]���[�h�ɑJ��	*/
		S_ccc_set_mode( CCC_MODE_GET_PID );
	}
}

/********************************************************
 * ���䃂�[�h:�Ή�PID�擾���
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		�Ή�PID���[�h�ɏ]�����������s���D
********************************************************/
static void S_ccc_mode_get_pid( void )
{
	C_ccc_pid_func[S_ccc_pid_mode]();
}

/********************************************************
 * ���䃂�[�h:PID���N�G�X�g���M���
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		PID���N�G�X�g�D�揇�ʂɏ]���āAPID���N�G�X�g�𑗐M����D
********************************************************/
static void S_ccc_mode_request_pid( void )
{
	UI_8		pid_index;
	UI_8		*priority_pid;
	T_Can_data	can_data;
	UI_8		pid;
	UI_8		value_data_len;
	UI_8		value_data[5];
	UI_8		i;

	if ((S_priority_1_pid_count + S_priority_2_pid_count) > 0) {

		priority_pid = NULL;
		for (i = 0; i < 2; i++) {
			if((S_pid_req_count % 2) == 0) {
				if (S_priority_1_pid_count > 0) {
					/* �D��P��I������ */
					priority_pid = S_priority_1_pid;
					pid_index =  S_priority_1_pid_index;
					S_priority_1_pid_index++;
					if (S_priority_1_pid_count <= S_priority_1_pid_index) {
						S_priority_1_pid_index = 0;
					}
				}
			}
			else {
				if (S_priority_2_pid_count > 0) {
					/* �D��Q��I������ */
					priority_pid = S_priority_2_pid;
					pid_index =  S_priority_2_pid_index;
					S_priority_2_pid_index++;
					if (S_priority_2_pid_count <= S_priority_2_pid_index) {
						S_priority_2_pid_index = 0;
					}
				}
			}
			S_pid_req_count++;

			if (priority_pid != NULL) {
				break;
			}
		}

		/* �M�����N�G�X�g���M */
		(void)send_request(S_connection_type, S_can_id, priority_pid[pid_index]);

		/* ���X�|���X��M���[�v */
		while(Can_drv_get_rx_data(CAN_CH_ISO_CAN, &can_data) == E_RC_OK) {
			/* ���X�|���X�f�[�^�̃p�[�X */
			if(parse_response_data(&can_data, &pid, &value_data_len, value_data) == E_RC_OK) {
				update_signal_value(pid, value_data_len, value_data);
			}
		}
	}
}

/********************************************************
 * ��Ԋ֐�:�ڑ��G���[���
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		�������Ȃ��D
********************************************************/
static void S_ccc_mode_connect_error( void )
{
	/**	<ol>	*/
	/**	<li>�������Ȃ�	*/

	/**	</ol>	*/
}


/********************************************************
 * PID���[�h:TYPE-1A�ڑ��m�F
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		�ԗ���TYPE-1A�ɑΉ����Ă��邩������s���D
 * 			�K�莞�Ԃ��߂���܂łɐ���ȃ��X�|���X������ꂽ�ꍇ�A
 * 			PID���[�h��Ή�PID�Ή��擾�ɐݒ肷��D
 * 			�K�莞�Ԓ��ɐ���ȃ��X�|���X�������Ȃ������ꍇ�A
 * 			PID���[�h��TYPE-1B�ڑ��m�F�ɐݒ肷��D
********************************************************/
static void S_ccc_pid_std_can_1a( void )
{
	E_RC				ret_code;
	UI_32				can_id;
	E_ISO_CAN_RESPONSE	response;

	if( S_tx_flg == N_OFF)
	{
		S_tx_flg = N_ON;

		/* ���b�Z�[�W�o�b�t�@�̐ݒ�(��MID:0x7E8)	*/
		Can_drv_change_message_buffer(CAN_CH_ISO_CAN, ISO_CAN_MSG_ID_RX_MIN, CAN_ID_TYPE_1_RES_CAN_ID_ECU_1);
		Can_drv_clear_rx_data(CAN_CH_ISO_CAN);

		/* �ʏ�Ή�PID���N�G�X�g */
		(void)send_request(ISO_CAN_REQ_TYPE_1A, CAN_ID_TYPE_1_REQ_CAN_ID_ECU_1, PID_SUPPORT_INFO_00);
		S_connect_time = Timer_ctrl_GetTime1ms();
		S_retry_wait_time = S_connect_time;
	}
	else
	{
		if (Timer_ctrl_TimeCompare1ms(S_connect_time, JUDGE_WAIT_TIME) == N_NG)
		{
			/* ���X�|���X�m�F */
			ret_code = receive_response(ISO_CAN_REQ_TYPE_1A, PID_SUPPORT_INFO_00, &can_id, &response, NULL);
			if ((ret_code == E_RC_OK) && (response == ISO_CAN_RESP_ACK)) {
				/* �ڑ��^�C�v 1A �m�肵�āAPID�����W��Ԃ֑J�� */
				S_can_id = can_id & 0x0000FFF7UL;
				S_ccc_pid_mode = CCC_PID_PID_INFO;
				S_connection_type = ISO_CAN_REQ_TYPE_1A;
				S_tx_flg = N_OFF;

				OUTLOG(OutputLog("[ISOCAN]TYPE-1A\n");)
			}
		}
		else
		{
			/* ���X�|���X�҂��^�C���A�E�g�̏ꍇ Type 1B ���N�G�X�g���s�֑J�� */
			S_ccc_pid_mode = CCC_PID_STD_CAN_1B;
			S_tx_flg = N_OFF;
		}
	}
}


/********************************************************
 * PID���[�h:TYPE-1B�ڑ��m�F
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		�ԗ���TYPE-1B�ɑΉ����Ă��邩������s���D
 * 			�K�莞�Ԃ��߂���܂łɐ���ȃ��X�|���X������ꂽ�ꍇ�A
 * 			PID���[�h��Ή�PID�Ή��擾�ɐݒ肷��D
 * 			�K�莞�Ԓ��ɐ���ȃ��X�|���X�������Ȃ������ꍇ�A
 * 			PID���[�h��TYPE-1T�ڑ��m�F�ɐݒ肷��D
********************************************************/
static void S_ccc_pid_std_can_1b( void )
{
	E_RC				ret_code;
	UI_32				can_id;
	E_ISO_CAN_RESPONSE	response;
	SUPPORTED_PID_INFO	sup_pid_info;

	if( S_tx_flg == N_OFF)
	{
		S_tx_flg = N_ON;

		/* ���b�Z�[�W�o�b�t�@�̐ݒ�(��MID:0x7E8)	*/
		Can_drv_change_message_buffer(CAN_CH_ISO_CAN, ISO_CAN_MSG_ID_RX_MIN, CAN_ID_TYPE_1_RES_CAN_ID_ECU_1);
		Can_drv_clear_rx_data(CAN_CH_ISO_CAN);

		/* �ʏ�Ή�PID���N�G�X�g */
		(void)send_request(ISO_CAN_REQ_TYPE_1B, CAN_ID_TYPE_1_REQ_CAN_ID_ADDRESS, PID_SUPPORT_INFO_00);
		S_connect_time = Timer_ctrl_GetTime1ms();
	}
	else
	{
		if (Timer_ctrl_TimeCompare1ms(S_connect_time, JUDGE_WAIT_TIME) == N_NG)
		{
			/* ���X�|���X�m�F */
			ret_code = receive_response(ISO_CAN_REQ_TYPE_1B, PID_SUPPORT_INFO_00, &can_id, &response, &sup_pid_info);
			if ((ret_code == E_RC_OK) && (response == ISO_CAN_RESP_ACK)) {
				/* �ڑ��^�C�v 1B �m�肵�āAPID�����W��Ԃ֑J�� */
				S_can_id = CAN_ID_TYPE_1_REQ_CAN_ID_ADDRESS;
				S_ccc_pid_mode = CCC_PID_PID_INFO;
				S_connection_type = ISO_CAN_REQ_TYPE_1B;
				S_target_ecu_no = sup_pid_info.ecu_no;
				S_tx_flg = N_OFF;

				OUTLOG(OutputLog("[ISOCAN]TYPE-1B\n");)
			}
		}
		else
		{
			/* ���X�|���X�҂��^�C���A�E�g�̏ꍇ Type 1T ���N�G�X�g���s�֑J�� */
			S_ccc_pid_mode = CCC_PID_STD_CAN_1T;
			S_tx_flg = N_OFF;
		}
	}
}

/********************************************************
 * PID���[�h:TYPE-1T�ڑ��m�F
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		�ԗ���TYPE-1T�ɑΉ����Ă��邩������s���D
 * 			�K�莞�Ԃ��߂���܂łɐ���ȃ��X�|���X������ꂽ�ꍇ�A
 * 			PID���[�h��Ή�PID�Ή��擾�ɐݒ肷��D
 * 			�K�莞�Ԓ��ɐ���ȃ��X�|���X�������Ȃ������ꍇ�A
 * 			PID���[�h��TYPE-2�ڑ��m�F�ɐݒ肷��D
********************************************************/
static void S_ccc_pid_std_can_1t( void )
{
	E_RC				ret_code;
	UI_32				can_id;
	E_ISO_CAN_RESPONSE	response;

	if( S_tx_flg == N_OFF)
	{
		S_tx_flg = N_ON;

		/* ���b�Z�[�W�o�b�t�@�̐ݒ�(��MID:0x7E8)	*/
		Can_drv_change_message_buffer(CAN_CH_ISO_CAN, ISO_CAN_MSG_ID_RX_MIN, CAN_ID_TYPE_1_RES_CAN_ID_ECU_1);
		Can_drv_clear_rx_data(CAN_CH_ISO_CAN);

		/* �ʏ�Ή�PID���N�G�X�g */
		(void)send_request(ISO_CAN_REQ_TYPE_1T, CAN_ID_TYPE_1_REQ_CAN_ID_ECU_1, PID_SUPPORT_INFO_00);
		S_connect_time = Timer_ctrl_GetTime1ms();
	}
	else
	{
		if (Timer_ctrl_TimeCompare1ms(S_connect_time, JUDGE_WAIT_TIME) == N_NG)
		{
			/* ���X�|���X�m�F */
			ret_code = receive_response(ISO_CAN_REQ_TYPE_1T, PID_SUPPORT_INFO_00, &can_id, &response, NULL);
			if ((ret_code == E_RC_OK) && (response == ISO_CAN_RESP_ACK)) {
				/* �ڑ��^�C�v 1T �m�肵�āAPID�����W��Ԃ֑J�� */
				S_can_id = can_id & 0x0000FFF7UL;
				S_ccc_pid_mode = CCC_PID_PID_INFO;
				S_connection_type = ISO_CAN_REQ_TYPE_1T;
				S_tx_flg = N_OFF;

				OUTLOG(OutputLog("[ISOCAN]TYPE-1T\n");)
			}
		}
		else
		{
			/* ���X�|���X�҂��^�C���A�E�g�̏ꍇ Type 2 ���N�G�X�g���s�֑J�� */
			S_ccc_pid_mode = CCC_PID_EXT_CAN;
			S_tx_flg = N_OFF;
		}
	}
}

/********************************************************
 * PID���[�h:TYPE-2�ڑ��m�F
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		�ԗ���TYPE-2�ɑΉ����Ă��邩������s���D
 * 			�K�莞�Ԃ��߂���܂łɐ���ȃ��X�|���X������ꂽ�ꍇ�A
 * 			PID���[�h��Ή�PID�Ή��擾�ɐݒ肷��D
 * 			�K�莞�Ԓ��ɐ���ȃ��X�|���X�������Ȃ������ꍇ�A
 * 			���N�G�X�g�񐔂�5��ȉ��ł����PID���[�h��CAN���N�G�X�g���M�҂��ɐݒ肷��D
 * 			���N�G�X�g�񐔂�6��ȏ�̏ꍇ�͐��䃂�[�h��ʐM�G���[��Ԃɐݒ肷��D
********************************************************/
static void S_ccc_pid_ext_can( void )
{
	UI_32				can_id;
	E_ISO_CAN_RESPONSE	response;
	SUPPORTED_PID_INFO	sup_pid_info;
	UI_8				i;

	/* ���MID�m�F(29bit) */
	
	if( S_tx_flg == N_OFF)
	{
		S_tx_flg = N_ON;
		S_sup_pid_info_count = 0;

		/* ���b�Z�[�W�o�b�t�@�̐ݒ�(��MID:0x18DAF1xx)	*/
		for (i = 0; i < 10; i++) {
			Can_drv_change_message_buffer_mask(CAN_CH_ISO_CAN, ISO_CAN_MSG_ID_RX_MIN + i, CAN_ID_TYPE_2_RES_CAN_ID, 0xFFFFFF00UL);
		}
		Can_drv_clear_rx_data(CAN_CH_ISO_CAN);

		/* �ʏ�Ή�PID���N�G�X�g */
		(void)send_request(ISO_CAN_REQ_TYPE_2, CAN_ID_TYPE_2_REQ_CAN_ID, PID_SUPPORT_INFO_00);
		S_connect_time = Timer_ctrl_GetTime1ms();
	}
	else
	{
		if (Timer_ctrl_TimeCompare1ms(S_connect_time, JUDGE_WAIT_TIME) == N_NG)
		{
			/* ���X�|���X�m�F */
			while(receive_response(ISO_CAN_REQ_TYPE_2, PID_SUPPORT_INFO_00, &can_id, &response, &sup_pid_info) == E_RC_OK) {
				if(response == ISO_CAN_RESP_ACK) {
					if(S_sup_pid_info_count < SUP_PID_INFO_MAX_COUNT) {
						(void)memcpy(&S_sup_pid_info[S_sup_pid_info_count], &sup_pid_info, sizeof(SUPPORTED_PID_INFO));
						S_sup_pid_info_count++;
					}
				}
			}
		}
		else
		{
			S_tx_flg = N_OFF;
			S_target_ecu_no = 0;

			/* ���b�Z�[�W�o�b�t�@�̐ݒ�(���g�p�ɂ���)	*/
			for (i = 0; i < 10; i++) {
				Can_drv_change_message_buffer_mask(CAN_CH_ISO_CAN, ISO_CAN_MSG_ID_RX_MIN + i, 0, 0);
			}
			Can_drv_clear_rx_data(CAN_CH_ISO_CAN);

			if(S_sup_pid_info_count > 0) {
				/* �f�[�^�L�����̈�ԑ���ECU��I������ */
				S_target_ecu_no = select_ecu_for_type_2();
			}

			if(S_target_ecu_no > 0) {
				/* ECU���I���ł��� */
				/* �ڑ��^�C�v 2 �m�肵�āAPID�����W��Ԃ֑J�� */
				S_can_id = CAN_ID_TYPE_2_REQ_TEMP | ((UI_32)S_target_ecu_no << 8);
				S_ccc_pid_mode = CCC_PID_PID_INFO;
				S_connection_type = ISO_CAN_REQ_TYPE_2;

				/* ���b�Z�[�W�o�b�t�@�̐ݒ�(��MID:0x18DAF1xx  xx��ecu no)	*/
				Can_drv_change_message_buffer(CAN_CH_ISO_CAN, ISO_CAN_MSG_ID_RX_MIN, (CAN_ID_TYPE_2_RES_TEMP | (UI_32)S_target_ecu_no));

				OUTLOG(OutputLog("[ISOCAN]TYPE-2 EcuNo:%d\n", S_target_ecu_no);)
			}
			else {
				S_connect_check_cnt++;
				if(S_connect_check_cnt < 6) {
					S_ccc_pid_mode = CCC_PID_WAIT;
				}
				else {
					S_ccc_set_mode(CCC_MODE_CONNECT_ERROR);
					OUTLOG(OutputLog("[ISOCAN]ERR MODE\n");)
				}
			}
		}
	}
}

/********************************************************
 * PID���[�h:�Ή�PID�擾
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		�Ή�PID�̎擾���s���D
 * 			�擾������������APID���[�h�����N�G�X�g�D�揇�ʍ쐬�ɐݒ肷��D
********************************************************/
static void S_ccc_pid_info( void )
{
	E_RC				ret_code;
	UI_32				can_id;
	E_ISO_CAN_RESPONSE	response;
	SUPPORTED_PID_INFO	sup_pid_info;
	UI_32				tmp;

	if(S_pid_seq < 3)
	{
		if( S_tx_flg == N_OFF)
		{
			S_tx_flg = N_ON;

			/* �ʏ�Ή�PID���N�G�X�g */
			(void)send_request(S_connection_type, S_can_id, C_CCC_REQ_INDEX[S_pid_seq]);
			S_connect_time = Timer_ctrl_GetTime1ms();
		}
		else
		{
			if (Timer_ctrl_TimeCompare1ms(S_connect_time, JUDGE_WAIT_TIME) == N_NG)
			{
				/* ���X�|���X�m�F */
				ret_code = receive_response(S_connection_type, C_CCC_REQ_INDEX[S_pid_seq], &can_id, &response, &sup_pid_info);
				if ((ret_code == E_RC_OK) && (response == ISO_CAN_RESP_ACK)) {
					S_tx_flg = N_OFF;

					tmp = sup_pid_info.pid_supported[0];
					tmp <<= DATA1_SHIFT;
					tmp |= sup_pid_info.pid_supported[1];
					tmp <<= DATA2_SHIFT;
					tmp |= sup_pid_info.pid_supported[2];
					tmp <<= DATA3_SHIFT;
					tmp |= sup_pid_info.pid_supported[3];

					OUTLOG(OutputLog("[ISOCAN]%d PID:%08X\n", S_pid_seq, tmp);)

					/* ���N�G�X�g�i�[bit���Y���Ă��鎖�ւ̑΍� */
					tmp >>= SHIFT_1BIT;

					S_correspondence[C_CCC_RES_OFFSET[S_pid_seq]] = (UI_16)(tmp >> TAKEOUT_HIGH_16BIT);
					S_correspondence[C_CCC_RES_OFFSET[S_pid_seq]+1] = (UI_16)(tmp & TAKEOUT_LOW_16BIT);

					S_pid_seq++;
				}
			}
			else {
				S_tx_flg = N_OFF;
				S_pid_seq++;
			}
		}
	}
	else
	{
		S_ccc_pid_mode = CCC_PID_MAKE_REQUEST;
	}
}

/********************************************************
 * PID���[�h:���N�G�X�g�D�揇�ʍ쐬
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		����ꂽPID�ɑ΂��āA���N�G�X�g�D�揇�ʂɊ�Â����N�G�X�g�e�[�u�����쐬����D
 * 			�쐬�����������琧�䃂�[�h��PID���N�G�X�g���M��Ԃɐݒ肷��D
********************************************************/
static void S_ccc_pid_make_request( void )
{
	UI_8 group;
	UI_8 value;
	UI_8 i;

	/* �Ώ�PID��D�揇�ʏ��ɑ��� */
	S_priority_1_pid_count = 0;
	S_priority_2_pid_count = 0;
	for(i = 0; i < MAX_REQUEST_NUMBER; i++) {
		/* ����PID���T�|�[�g����Ă��邩�`�F�b�N */
		group = C_correspondence[i] >> TAKEOUT_HIGH_4BIT;			/* ��ʃr�b�g�Ŏ�ʔ��f */
		value = C_correspondence[i] & TAKEOUT_LOW_4BIT;				/* ���ʃr�b�g�Œl���f */
		
		if((S_correspondence[group] & (JUDGE_REQ_BIT >> value)) != NO_RESPONSE){
			if(i < MAX_COUNT_PRIORITY_1_PID) {
				/* �D�揇�ʂP�̃X�P�W���[���ɒǉ� */
				S_priority_1_pid[S_priority_1_pid_count] = C_correspondence[i];
				S_priority_1_pid_count++;
				OUTLOG(OutputLog("[ISOCAN]pri1-%d PID:%02X\n", S_priority_1_pid_count, C_correspondence[i]);)
			}
			else {
				/* �D�揇�ʂQ�̃X�P�W���[���ɒǉ� */
				S_priority_2_pid[S_priority_2_pid_count] = C_correspondence[i];
				S_priority_2_pid_count++;
				OUTLOG(OutputLog("[ISOCAN]pri2-%d PID:%02X\n", S_priority_2_pid_count, C_correspondence[i]);)
			}
		}
	}

	S_ccc_set_mode( CCC_MODE_REQUEST_PID );
}

/********************************************************
 * PID���[�h:CAN���N�G�X�g���M�҂�
 * 
 * author	S.Iwami
 * date		2014/04/11
 * note		TYPE-1A���N�G�X�g����5sec�҂D�i���̊Ԃ͉������Ȃ��j
 * 			5sec�o�ߌ�ɁAPID���[�h��TYPE-1A�ڑ��m�F�ɐݒ肷��D
********************************************************/
static void S_ccc_pid_wait( void )
{
	/**	<ol>	*/
	/**	<li>TYPE-1A���N�G�X�g�𑗐M���Ă���5sec�҂�	*/
	if (Timer_ctrl_TimeCompare1ms(S_retry_wait_time, JUDGE_RETRY_WAIT_TIME) == N_OK) {
		/**	<li>5sec�o�߂��Ă����̂Ȃ�΁A�Ή�PID���擾���[�h��Ԃ�CCC_PID_STD_CAN_1A�ɕύX����	*/
		S_ccc_pid_mode = CCC_PID_STD_CAN_1A;
	}
	/**	</ol>	*/

}

/********************************************************/
/* �֐�����		: send_request							*/
/* �@�\�̐���	: ���N�G�X�g���M����					*/
/* ����			: (I)req_type : ���N�G�X�g�^�C�v		*/
/* �߂�l		: ���M����								*/
/*				   E_RC_OK:���M����, E_RC_NG:���M�G���[	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/10/24								*/
/* ���l			: 										*/
/********************************************************/
static E_RC send_request(E_ISO_CAN_REQ_TYPE req_type, UI_32 can_id, UI_8 pid)
{
	UI_8  msg_data[8];
	UI_8  msg_len;
	E_RC  ret_code;

	(void)memset(msg_data, 0, sizeof(msg_data));
	msg_len = 8;

	switch(req_type) {
	case ISO_CAN_REQ_TYPE_1A:
		/* type 1A �̃��N�G�X�g���b�Z�[�W�쐬 */
		msg_data[0] = 2;				/* �f�[�^�� */
		msg_data[1] = MODE_REQ_NORMAL;	/* ���[�h */
		msg_data[2] = pid;				/* PID */
		break;
	case ISO_CAN_REQ_TYPE_1B:
		/* type 1B �̃��N�G�X�g���b�Z�[�W�쐬 */
		msg_data[0] = 2;
		msg_data[1] = MODE_REQ_NORMAL;
		msg_data[2] = pid;
		break;
	case ISO_CAN_REQ_TYPE_1T:
		/* type 1T �̃��N�G�X�g���b�Z�[�W�쐬 */
		msg_data[0] = 2;
		msg_data[1] = MODE_REQ_TOYOTA;
		msg_data[2] = pid;
		break;
	case ISO_CAN_REQ_TYPE_2:
		/* type 2 �̃��N�G�X�g���b�Z�[�W�쐬 */
		msg_data[0] = 2;
		msg_data[1] = MODE_REQ_NORMAL;
		msg_data[2] = pid;
		break;
	default:
		return E_RC_NG;
	}

	/* ���N�G�X�g���b�Z�[�W���M */
	ret_code = Can_drv_set_tx_data(CAN_CH_ISO_CAN, 1, can_id, msg_data, msg_len);
	return ret_code;
}

/********************************************************/
/* �֐�����		: receive_response						*/
/* �@�\�̐���	: ���X�|���X��M����					*/
/* ����			: (I)req_type : ���N�G�X�g�^�C�v		*/
/* 				: (O)response : ��������				*/
/* 				: (O)sup_pid_info : PID�Ή����			*/
/* �߂�l		: ��M����								*/
/*				   E_RC_OK:��M����, E_RC_NG:��M�G���[	*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/10/24								*/
/* ���l			: 										*/
/*  sup_pid_info��NULL�̏ꍇ�ɂ́A���Y�̈�ւ�PID�Ή�   */
/* ���̊i�[�͍s��Ȃ��B                               */
/********************************************************/
static E_RC receive_response(E_ISO_CAN_REQ_TYPE req_type, UI_8 pid, UI_32 *can_id, E_ISO_CAN_RESPONSE *response, SUPPORTED_PID_INFO *sup_pid_info)
{
	E_RC       ret_code;
	T_Can_data can_data;

	ret_code = Can_drv_get_rx_data(CAN_CH_ISO_CAN, &can_data);
	if(ret_code != E_RC_OK) {
		return ret_code;
	}

/*
	OUTLOG(OutputLog("[ISOCAN]rcv id:%X len:%d %02X %02X %02X %02X %02X %02X %02X %02X\n",
			can_data.id, can_data.len,
			can_data.data[0], can_data.data[1], can_data.data[2], can_data.data[3],
			can_data.data[4], can_data.data[5], can_data.data[6], can_data.data[7]);)
*/

	switch(req_type) {
	case ISO_CAN_REQ_TYPE_1A:
		/* type 1A �̉������b�Z�[�W�`�F�b�N */
		if((can_data.id == CAN_ID_TYPE_1_RES_CAN_ID_ECU_1)
		 &&(can_data.data[0] == 6)
		 &&(can_data.data[1] == MODE_RES_NORMAL)
		 &&(can_data.data[2] == pid)) {
		 	*response = ISO_CAN_RESP_ACK;
		 	if(sup_pid_info != NULL) {
		 		sup_pid_info->ecu_no = 1;
				(void)memcpy(sup_pid_info->pid_supported, &can_data.data[3], 4);
		 	}
		}
		else {
		 	*response = ISO_CAN_RESP_NACK;
		}
		break;
	case ISO_CAN_REQ_TYPE_1B:
		/* type 1A �̉������b�Z�[�W�`�F�b�N */
		if((can_data.id >= CAN_ID_TYPE_1_RES_CAN_ID_ECU_1)
		 &&(can_data.id <= CAN_ID_TYPE_1_RES_CAN_ID_ECU_8)
		 &&(can_data.data[0] == 6)
		 &&(can_data.data[1] == MODE_RES_NORMAL)
		 &&(can_data.data[2] == pid)) {
		 	*response = ISO_CAN_RESP_ACK;
		 	if(sup_pid_info != NULL) {
		 		sup_pid_info->ecu_no = (UI_8)((can_data.id & 0x00000007) + 1);
				(void)memcpy(sup_pid_info->pid_supported, &can_data.data[3], 4);
		 	}
		}
		else {
		 	*response = ISO_CAN_RESP_NACK;
		}
		break;
	case ISO_CAN_REQ_TYPE_1T:
		/* type 1T �̉������b�Z�[�W�`�F�b�N */
		if((can_data.id == CAN_ID_TYPE_1_RES_CAN_ID_ECU_1)
		 &&(can_data.data[0] == 6)
		 &&(can_data.data[1] == MODE_RES_TOYOTA)
		 &&(can_data.data[2] == pid)) {
		 	*response = ISO_CAN_RESP_ACK;
		 	if(sup_pid_info != NULL) {
		 		sup_pid_info->ecu_no = 1;
				(void)memcpy(sup_pid_info->pid_supported, &can_data.data[3], 4);
		 	}
		}
		else {
		 	*response = ISO_CAN_RESP_NACK;
		}
		break;
	case ISO_CAN_REQ_TYPE_2:
		if(((can_data.id & 0xffffff00UL) == CAN_ID_TYPE_2_RES_CAN_ID)
		 &&(can_data.data[0] == 6)
		 &&(can_data.data[1] == MODE_RES_NORMAL)
		 &&(can_data.data[2] == pid)) {
		 	*response = ISO_CAN_RESP_ACK;
		 	if(sup_pid_info != NULL) {
		 		sup_pid_info->ecu_no = (UI_8)(can_data.id & 0x000000ff);
				(void)memcpy(sup_pid_info->pid_supported, &can_data.data[3], 4);
		 	}
		}
		else {
		 	*response = ISO_CAN_RESP_NACK;
		}
		break;
	default:
		return E_RC_NG;
	}

	*can_id = can_data.id;
	return E_RC_OK;
}

/********************************************************/
/* �֐�����		: select_ecu_for_type_2					*/
/* �@�\�̐���	: type 2 �v����ECU�̑I������			*/
/* ����			: void									*/
/* �߂�l		: �v����ECU�ԍ�							*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/10/24								*/
/* ���l			: 										*/
/********************************************************/
static UI_8 select_ecu_for_type_2(void)
{
	UI_8 i;
	UI_8 enable_count;
	UI_8 max_enable_count  = 0;
	UI_8 max_enable_ecu_no = 0;

	/* ���ׂĂ̎擾�ς�PID�Ή����𑖍� */
	for(i = 0; i < S_sup_pid_info_count; i++) {
		/* �L��PID�����擾 */
		enable_count = get_enable_pid_count(S_sup_pid_info[i].pid_supported);

		/* �ő�̂��̂���肷�� */
		if(enable_count > max_enable_count) {
			max_enable_count  = enable_count;
			max_enable_ecu_no = S_sup_pid_info[i].ecu_no;
		}
	}

	/* �L��PID���ő��ECU�ԍ������^�[�� */
	return max_enable_ecu_no;
}

/********************************************************/
/* �֐�����		: get_enable_pid_count					*/
/* �@�\�̐���	: �L��PID���擾����						*/
/* ����			: (i)pid_supported : PID�Ή����ꗗ	*/
/* �߂�l		: �L��PID��								*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/10/24								*/
/* ���l			: 										*/
/*   ���͂��ꂽpid_supported����A�擾������PID�̃T�|   */
/* �[�g�����J�E���g���ă��^�[������B					*/
/********************************************************/
static UI_8 get_enable_pid_count(const UI_8 *pid_supported)
{
	UI_8 i;
	UI_8 byte_index;
	UI_8 bit_index;
	UI_8 enable_count = 0;
	
	/* �擾�Ώ�PID�𑖍� */
	for(i = 0; i < MAX_REQUEST_NUMBER; i++) {
		/* 0x20�܂ł�PID�ɂ��ď������� */
		if(C_correspondence[i] <= 0x20) {
			/* PID�Ή�����Byte/bit�C���f�b�N�X�Z�o */
			byte_index = (C_correspondence[i] - 1)  / 8;
			bit_index  = 7 - ((C_correspondence[i] - 1) % 8);

			/* ���Y�r�b�g�������Ă���ΗL�������C���N�������g */
			if((pid_supported[byte_index] & (UI_8)(1<< bit_index)) != 0) {
				enable_count++;
			}
		}
	}

	return(enable_count);
}

/********************************************************/
/* �֐�����		: parse_response_data					*/
/* �@�\�̐���	: ���X�|���X�f�[�^�̃p�[�X����			*/
/* ����			: (I)t_can_data     : ��M�f�[�^		*/
/* 				: (O)pid            : PID				*/
/* 				: (O)value_data_len : ���YPID��Value�f�[�^��*/
/* 				: (O)value_data     : ���YPID��Value�f�[�^  */
/* �߂�l		: �p�[�X����							*/
/*				   E_RC_OK:��e�\�����f�[�^			*/
/*				   E_RC_NG:��e�s�f�[�^�[				*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/11/07								*/
/* ���l			: 										*/
/********************************************************/
static E_RC parse_response_data(const T_Can_data *t_can_data, UI_8 *pid, UI_8 *value_data_len, UI_8 *value_data)
{
	UI_32 resp_msg_id;
	UI_8  resp_value_data_len;
	UI_8  mode_resp;
	UI_8  i;

/*
	OUTLOG(OutputLog("[ISOCAN]rcv id:%X len:%d %02X %02X %02X %02X %02X %02X %02X %02X\n",
					t_can_data->id, t_can_data->len,
					t_can_data->data[0], t_can_data->data[1], t_can_data->data[2], t_can_data->data[3],
					t_can_data->data[4], t_can_data->data[5], t_can_data->data[6], t_can_data->data[7]);)
*/

	/* ���b�Z�[�WID�̑Ó����`�F�b�N */
	resp_msg_id = get_resp_msg_id(S_connection_type);
	if(t_can_data->id != resp_msg_id) {
/*
		OUTLOG(OutputLog("[ISOCAN]rcv id ERR!!\n");)
*/
		return E_RC_NG;
	}

	/* ���b�Z�[�W�f�[�^�擪�̃f�[�^���̑Ó����`�F�b�N */
	resp_value_data_len = get_value_data_len(t_can_data->data[2]);
	if((t_can_data->data[0] - 2) != resp_value_data_len) {
/*
		OUTLOG(OutputLog("[ISOCAN]rcv len ERR!!\n");)
*/
		return E_RC_NG;
	}

	/* ���b�Z�[�W�f�[�^2byte�ڂ̃��[�h�l�̑Ó����`�F�b�N */
	if(S_connection_type == ISO_CAN_REQ_TYPE_1T) {
		mode_resp = MODE_RES_TOYOTA;
	}
	else {
		mode_resp = MODE_RES_NORMAL;
	}
	if(t_can_data->data[1] != mode_resp) {
/*
		OUTLOG(OutputLog("[ISOCAN]rcv mode ERR!!\n");)
*/
		return E_RC_NG;
	}

	/* PID�ƌ㑱�l�f�[�^�����^�[������ */
	*pid = t_can_data->data[2];
	*value_data_len = t_can_data->data[0] - 2;
	for(i = 0; i < *value_data_len; i++) {
		value_data[i] = t_can_data->data[i + 3];
	}

	return E_RC_OK;
}

/********************************************************/
/* �֐�����		: update_signal_value					*/
/* �@�\�̐���	: �M���l�̍X�V							*/
/* ����			: (I)pid        : PID					*/
/* 				: (I)value_data : ���YPID��Value�f�[�^	*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/11/07								*/
/* ���l			: 										*/
/********************************************************/
static void update_signal_value(UI_8 pid, UI_8 value_data_len, const UI_8 *value_data)
{
	switch(pid) {
	case PID_ENG_COOL_TEMP:
		/* �G���W������ */
		S_can_info_eng_cool_temp.data = value_data[0];
		S_can_info_eng_cool_temp.first_flg = 1;
		S_can_info_eng_cool_temp.fail_flg  = 0;
		break;
	case PID_IMA_PRESSURE:
		/* �z�C�}�j�t�H�[���h�� */
		S_can_info_im_abs_press.data = value_data[0];
		S_can_info_im_abs_press.first_flg = 1;
		S_can_info_im_abs_press.fail_flg  = 0;
		break;
	case PID_ENG_RPM:
		/* �G���W����]�� */
		S_can_info_eng_rpm.data = ((UI_16)value_data[0] << 8) | (UI_16)value_data[1];
		S_can_info_eng_rpm.first_flg = 1;
		S_can_info_eng_rpm.fail_flg  = 0;
		break;
	case PID_VEHICLE_SPEED:
		/* �ԑ� */
		S_can_info_vehicle_speed.data = value_data[0];
		S_can_info_vehicle_speed.first_flg = 1;
		S_can_info_vehicle_speed.fail_flg  = 0;
		break;
	case PID_INTAKE_AIR_TEMP:
		/* �z�C�� */
		S_can_info_intake_air_temp.data = value_data[0];
		S_can_info_intake_air_temp.first_flg = 1;
		S_can_info_intake_air_temp.fail_flg  = 0;
		break;
	case PID_THROTTLE_POS:
		/* �X���b�g���|�W�V���� */
		S_can_info_throttle_pos.data = value_data[0];
		S_can_info_throttle_pos.first_flg = 1;
		S_can_info_throttle_pos.fail_flg  = 0;
		break;
	case PID_CTRL_MOD_VOLT:
		/* �d�� */
		S_can_info_ctrl_mod_volt.data = ((UI_16)value_data[0] << 8) | (UI_16)value_data[1];
		S_can_info_ctrl_mod_volt.first_flg = 1;
		S_can_info_ctrl_mod_volt.fail_flg  = 0;
		break;
	default:
		break;
	}
}

/********************************************************/
/* �֐�����		: get_resp_msg_id						*/
/* �@�\�̐���	: �������b�Z�[�W�h�c�擾				*/
/* ����			: (I)connection_type:�ڑ��^�C�v			*/
/* �߂�l		: �������b�Z�[�W�h�c					*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/11/07								*/
/* ���l			: 										*/
/********************************************************/
static UI_32 get_resp_msg_id(E_ISO_CAN_REQ_TYPE connection_type)
{
	UI_32 resp_msg_id = 0;
	
	switch(connection_type) {
	case ISO_CAN_REQ_TYPE_1A:
		resp_msg_id = CAN_ID_TYPE_1_RES_CAN_ID_ECU_1;
		break;
	case ISO_CAN_REQ_TYPE_1B:
		resp_msg_id = CAN_ID_TYPE_1_RES_CAN_ID_ECU_1 + ((UI_32)S_target_ecu_no - 1);
		break;
	case ISO_CAN_REQ_TYPE_1T:
		resp_msg_id = CAN_ID_TYPE_1_RES_CAN_ID_ECU_1;
		break;
	case ISO_CAN_REQ_TYPE_2:
		resp_msg_id = (CAN_ID_TYPE_2_RES_TEMP | (UI_32)S_target_ecu_no);
		break;
	default:
		break;
	}

	/* �������b�Z�[�W�h�c���^�[�� */
	return resp_msg_id;
}

/********************************************************/
/* �֐�����		: get_value_data_len					*/
/* �@�\�̐���	: �M���f�[�^���̎擾					*/
/* ����			: (I)pid        : PID					*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/16								*/
/* ���l			: 										*/
/********************************************************/
static UI_8 get_value_data_len(UI_8 pid)
{
	UI_8	len;

	switch(pid) {
	case PID_ENG_COOL_TEMP:
		/* �G���W������ */
		len = 1;
		break;
	case PID_IMA_PRESSURE:
		/* �z�C�}�j�t�H�[���h�� */
		len = 1;
		break;
	case PID_ENG_RPM:
		/* �G���W����]�� */
		len = 2;
		break;
	case PID_VEHICLE_SPEED:
		/* �ԑ� */
		len = 1;
		break;
	case PID_INTAKE_AIR_TEMP:
		/* �z�C�� */
		len = 1;
		break;
	case PID_THROTTLE_POS:
		/* �X���b�g���|�W�V���� */
		len = 1;
		break;
	case PID_CTRL_MOD_VOLT:
		/* �d�� */
		len = 2;
		break;
	default:
		len = 0;
		break;
	}
	return len;
}

#endif /* ISOCAN_ENABLE */
