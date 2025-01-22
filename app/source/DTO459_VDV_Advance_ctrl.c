/********************************************************/
/* Customer			: Customer_XXXX						*/
/* Model(Thema No.)	: Model_XXXX						*/
/*------------------------------------------------------*/
/* CPU				: R81S								*/
/* Date				: 16/08/18							*/
/*------------------------------------------------------*/
/* Programmed by	: (OFT)Sakata						*/
/* Copyrights Nippon Seiki Co.,Ltd						*/
/*------------------------------------------------------*/
/* update by		: 									*/
/* date				: 									*/
/********************************************************/
/********************************************************/
/* file DTO459_VDV_Advance_ctrl.c						*/
/* AdvanceControlUnit����								*/
/********************************************************/

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include <string.h>
#include "system.h"
#include "DTO459_VDV_Sw_ctrl.h"
#include "DTO459_VDV_Inspection_ctrl.h"
#include "DTO459_VDV_Gnss_ctrl.h"
#include "DTO459_VDV_Bt_ctrl.h"
#include "DTO459_VDV_Advance_ctrl.h"
#include "System_control_TASK.h"
#include "defi_07701_CTRL.h"						/* DF07701����ͯ�ް̧�� */
#include "defi_uart_prt.h"							/* Defi UART���ĺ�ͯ�ް̧�� */

/********************************************************/
/*	�}�N����`											*/
/********************************************************/
#define ADVC_LOCAL	static

#define ADVANCE_UART_DATA_BUF_SIZE		(1 * 1024)
#define		DEFI_REAL_SZ				(36)							/**< ر�Ӱ���ް�����		*/

/********************************************************/
/*	�\���̒�`											*/
/********************************************************/
typedef struct t_advance_inspection_info {
	UI_8						reqflg;
} T_ADVANCE_INSPECTION_INFO;

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
ADVC_LOCAL	UI_8	Defi_real_data[DEFI_REAL_SZ];							/**< ���A�����[�h�f�[�^ 			*/
ADVC_LOCAL	UI_8	Defi_real_len;											/**< ���A�����[�h�f�[�^�� 			*/
ADVC_LOCAL	E_SYS_CTRL_STATUS			S_df_prev_system_state;				/**< �O�̃V�X�e����� 				*/
ADVC_LOCAL	T_ADVANCE_CTRL_UNIT_INFO	S_acu_Info;							/**<AdvanceControlUnit����擾�����l*/
ADVC_LOCAL T_ADVANCE_INSPECTION_INFO	S_advance_inspection_info;			/* �������							*/
ADVC_LOCAL E_DEFI_UART_BAUD_RATE		S_advance_gnsscomm_brate;			/* GNSS�ʐM���[�h���̃{�[���[�g		*/
ADVC_LOCAL UI_8		S_advance_uart_data[ADVANCE_UART_DATA_BUF_SIZE];		/* UART�f�[�^�o�b�t�@				*/
																			/*	�ȉ��̓��샂�[�h�Ŏg�p����		*/
																			/*	�������[�h						*/
																			/*	BT�ʐM���[�h					*/
																			/*	GNSS�ʐM���[�h					*/

/********************************************************/
/*	�����֐�											*/
/********************************************************/
#ifdef ADVANCE_ENABLE
ADVC_LOCAL void advance_control_storeData(const UI_8* buff);
#endif /*ADVANCE_ENABLE*/

ADVC_LOCAL void advance_control_initInspection(void);
ADVC_LOCAL void advance_control_initInspectionInfo(void);
ADVC_LOCAL void advance_control_inspection(void);

ADVC_LOCAL void advance_control_initBtComm(void);
ADVC_LOCAL void advance_control_btComm(void);

ADVC_LOCAL void advance_control_initGnssComm(void);
ADVC_LOCAL void advance_control_gnssComm(void);

/********************************************************/
/*	�O���֐�											*/
/********************************************************/

/********************************************************/
/* �֐�����		: Advance_control_Init					*/
/* �@�\�̐���	: AdvanceControlUnit���䏉����			*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Advance_control_Init(void)
{
	Defi_real_len = 0;
	(void)memset( Defi_real_data, 0, DEFI_REAL_SZ*sizeof(UI_8));
	(void)memset(&S_acu_Info, 0x00, sizeof(T_ADVANCE_CTRL_UNIT_INFO));
	S_acu_Info.invalidFlag = 1;
	S_df_prev_system_state = SYS_OPERATION_WAIT_ACC_ON;

	return;
}

/********************************************************/
/* �֐�����		: Advance_control_Main					*/
/* �@�\�̐���	: AdvanceControlUnit���䃁�C��			*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Advance_control_Main(void)
{
	E_SYS_CTRL_STATUS	system_state;									/* �V�X�e����� */
#ifdef ADVANCE_ENABLE
	UI_8	length;
	UI_8	acu_buff[DEFI_REAL_SZ];										/*�o�b�t�@*/
#endif /* ADVANCE_ENABLE */

	/* �V�X�e����Ԃ��ω��������𔻒� */
	system_state = System_control_GetStatus();
	if ( system_state != S_df_prev_system_state )
	{
		switch (system_state)
		{
		case (SYS_OPERATION_PRE_ACC_OFF):
		case (SYS_OPERATION_NORMAL_OFF):
		case (SYS_OPERATION_INSPECTION_COMP):
			/* Advance�pUART��~	*/
			Uart_stop();
			break;

#ifdef ADVANCE_ENABLE
		case (SYS_OPERATION_NORMAL_AUTO):
		case (SYS_OPERATION_NORMAL_MANUAL):
			if ((S_df_prev_system_state != SYS_OPERATION_NORMAL_AUTO) &&
				(S_df_prev_system_state != SYS_OPERATION_NORMAL_MANUAL)) {
				/* Advance���䏉����(UART�J�n)	*/
				Defi_07701_control_Init();
			}
			break;
#endif /*ADVANCE_ENABLE*/

		case (SYS_OPERATION_INSPECTION):
			/* �������䏉����(UART�J�n)	*/
			advance_control_initInspection();
			break;

		case (SYS_OPERATION_BT_COMM):
			/* BT�ʐM���[�h���䏉����(UART�J�n)	*/
			advance_control_initBtComm();
			break;

		case (SYS_OPERATION_GNSS_COMM):
			/* GNSS�ʐM���[�h���䏉����(UART�J�n)	*/
			advance_control_initGnssComm();
			break;
			
		default:
			break;
		}
		S_df_prev_system_state = system_state;
	}

	switch (system_state)
	{
#ifdef ADVANCE_ENABLE
		case (SYS_OPERATION_NORMAL_AUTO):
		case (SYS_OPERATION_NORMAL_MANUAL):
			/**< �ʏ탂�[�h:�I�[�gREC�܂��̓}�j���A��REC	*/
			/* DF07701���䏈�������s */
			Defi_07701_control_main();

			/*���A�����[�h�f�[�^�i�ԗ����j�擾*/
			length = Get_real_mode_data(acu_buff);
			if(length > 0)
			{
				/*��M�f�[�^����Ȃ�ԗ����\���̂Ɋi�[*/
				advance_control_storeData(acu_buff);
			}
			break;
#endif /*ADVANCE_ENABLE*/

		case (SYS_OPERATION_INSPECTION):
			/**< �������[�h		*/
			advance_control_inspection();
			break;

		case (SYS_OPERATION_BT_COMM):
			/**< BT���W���[���ʐM���[�h		*/
			advance_control_btComm();
			break;

		case (SYS_OPERATION_GNSS_COMM):
			/**< GNSS���W���[���ʐM���[�h		*/
			advance_control_gnssComm();
			break;

		default:
			return;
	}
	return;
}


/********************************************************/
/* �֐�����		: Advance_control_GetData				*/
/* �@�\�̐���	: �ԗ����擾							*/
/* ����			: �ԗ����\����						*/
/* 				: T_ADVANCE_CTRL_UNIT_INFO				*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Advance_control_GetData(T_ADVANCE_CTRL_UNIT_INFO **data)
{
	*data = &S_acu_Info;
}

/** 
 * ر�Ӱ���ް��ݒ菈��
 * 
 * param	*data_p		[in]	ر�Ӱ���ް����߲�� 
 * param	len			[in]	ر�Ӱ���ް���  
 *
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		<ul>
 *			<li>ر�Ӱ���ް���static�ϐ��ɐݒ肷��B 
 *			<li>DF07701�ƯĐڑ�����ر�Ӱ���ް���34byte 
 *			</ul>
 */ 
void Set_real_mode_data(const UI_8 *data_p, UI_8 len)
{
	if ((1 <= len) &&
		(len <= DEFI_REAL_SZ)) {
		(void)memcpy(Defi_real_data, data_p, (size_t)len);
		Defi_real_len = len;
	}
}

/** 
 * ر�Ӱ���ް��擾���� 
 * 
 * param	*data_p		[out]	ر�Ӱ���ް��i�[�ޯ̧���߲�� 
 *
 * return	ر�Ӱ���ް���  
 * 
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		<ul>
 *			<li>static�ϐ��ɐݒ肳�ꂽر�Ӱ���ް����擾����B   
 *			<li>DF07701�ƯĐڑ�����ر�Ӱ���ް���34byte 
 *			</ul>
 */ 
UI_8 Get_real_mode_data(UI_8 *data_p)
{
	UI_8 real_len;
	
	if (Defi_real_len > 0) {
		(void)memcpy(data_p, Defi_real_data, (size_t)Defi_real_len);
	}
	
	real_len = Defi_real_len;
	Defi_real_len = 0;

	return real_len;
}

/********************************************************/
/* �֐�����		: Advance_control_ReqInspection			*/
/* �@�\�̐���	: �����v��								*/
/* ����			: void									*/
/* �߂�l		: N_OK		����						*/
/* 				: N_NG		�ُ�						*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: �{�֐�������I�������ꍇ�A�������䃂�W���[���ɑ΂���		*/
/*				   Insp_notify_inspection_result�ɂ�茟�����ʂ�ʒm����	*/
/********************************************************/
SI_8 Advance_control_ReqInspection(void)
{
	E_SYS_CTRL_STATUS	system_state;

	/* �V�X�e����Ԏ擾	*/
	system_state = System_control_GetStatus();
	if (system_state != SYS_OPERATION_INSPECTION) {		/* �V�X�e����Ԃ��u�������[�h�v�ȊO	*/
		return N_NG;
	}

	/* �������䃂�W���[���Ɍ���(OK)��ʒm����	*/
	Insp_notify_inspection_result(INSP_INSPECTION_TYPE_UART, NULL, INSP_INSPECTION_RESULT_OK);

	if (S_advance_inspection_info.reqflg != N_ON) {
		/* UART����ON	*/
		S_advance_inspection_info.reqflg = N_ON;

		/* UART�ʐM�J�n	*/
		Uart_start();
	}

	return N_OK;
}

/********************************************************/
/* �֐�����		:	Advance_control_getUartData			*/
/* �@�\�̐���	:	UART�f�[�^�擾						*/
/* ����			:	( /O)buf	�f�[�^�o�b�t�@			*/
/*				:	(I/ )len	�擾�f�[�^��(byte)		*/
/* �߂�l		:	�擾�ł����f�[�^��(byte)			*/
/* �쐬��		:	(OFT)Toguchi						*/
/* �쐬��		:	17/01/20							*/
/* ���l			:										*/
/********************************************************/
UI_16 Advance_control_getUartData(UI_8 *buf, UI_16 len)
{
	UI_16	get_len;

	if (buf == NULL) {
		return 0;
	}

	get_len = Uart_get_rx_data(buf, len);

	return get_len;
}

/********************************************************/
/* �֐�����		:	Advance_control_getUartDataLen		*/
/* �@�\�̐���	:	UART�f�[�^���擾					*/
/* ����			:	void								*/
/* �߂�l		:	UART�f�[�^��(byte)					*/
/* �쐬��		:	(OFT)Toguchi						*/
/* �쐬��		:	17/01/20							*/
/* ���l			:										*/
/********************************************************/
UI_16 Advance_control_getUartDataLen(void)
{
	UI_16	len;

	len = Uart_get_rx_data_len();

	return len;
}

/********************************************************/
/*	�����֐�											*/
/********************************************************/
#ifdef ADVANCE_ENABLE
/********************************************************/
/* �֐�����		: advance_control_storeData				*/
/* �@�\�̐���	: ��M�f�[�^���ԗ����Ƃ��Ċi�[		*/
/* ����			: (I/ )��M�f�[�^						*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
ADVC_LOCAL void advance_control_storeData(const UI_8* buff)
{
	/* ��M�f�[�^����Ȃ炷�ׂẴf�[�^����M�ς� */
	UI_8 index = 11;	/* �擪��11 */
	/* �����t���O��OFF�ɂ��� */
	S_acu_Info.invalidFlag = 0;
	/* �ԑ� */
	S_acu_Info.VehicleSpeed				 = (UI_16)((UI_16)(((UI_16)(buff[index + 1] & 0x7FU)) << 8) | buff[index + 0]);
	/* �G���W����]�� */
	S_acu_Info.EngineRevolutionSpeed	 = (UI_16)((UI_16)(((UI_16)(buff[index + 3] & 0x7FU)) << 8) | buff[index + 2]);
	/* �^�[�{��(�ߋ���) */
	S_acu_Info.TurboPressure			 = (UI_16)((UI_16)(((UI_16)(buff[index + 5] & 0x7FU)) << 8) | buff[index + 4]);
	/* �G���W������ */
	S_acu_Info.EngineOilPressure		 = (UI_16)((UI_16)(((UI_16)(buff[index + 7] & 0x7FU)) << 8) | buff[index + 6]);
	/* �R������ */
	S_acu_Info.FuelPressure				 = (UI_16)((UI_16)(((UI_16)(buff[index + 9] & 0x7FU)) << 8) | buff[index + 8]);
	/* ���� */
	S_acu_Info.DifferentialPressure		 = (UI_8)((buff[index + 9] >> 7) & 0x01U);
	/* �G���W������ */
	S_acu_Info.EngineOiTemperture		 = (UI_16)((UI_16)(((UI_16)(buff[index + 11] & 0x7FU)) << 8) | buff[index + 10]);
	/* �G���W������ */
	S_acu_Info.EngineCoolantTemperture	 = (UI_16)((UI_16)(((UI_16)(buff[index + 13] & 0x7FU)) << 8) | buff[index + 12]);
	/* �r�C���x */
	S_acu_Info.ExhaustGasTemperture		 = (UI_16)((UI_16)(((UI_16)(buff[index + 15] & 0x7FU)) << 8) | buff[index + 14]);
	/* �d�� */
	S_acu_Info.Volts					 = (UI_8)(buff[index + 16] & 0xFFU);
}
#endif /*ADVANCE_ENABLE*/

/************************************************************/
/* �֐�����		: advance_control_initInspection			*/
/* �@�\�̐���	: �������䏉����							*/
/* ����			: void										*/
/* �߂�l		: void										*/
/* �쐬��		: (OFT)Toguchi								*/
/* �쐬��		: 17/01/20									*/
/* ���l			: 											*/
/************************************************************/
ADVC_LOCAL void advance_control_initInspection(void)
{
	T_DEFI_UART_PROTOCOL	protocol;

	/* ���������񏉊���	*/
	advance_control_initInspectionInfo();

	/* UART�ʐM�ݒ�	*/
	protocol.parity		= DEFI_UART_PARITY_EVEN;
	protocol.baud_rate	= DEFI_UART_BAUD_RATE_19200;
	(void)Uart_set_comm_protocol(&protocol);

	return;
}

/************************************************************/
/* �֐�����		: advance_control_initInspectionInfo		*/
/* �@�\�̐���	: ������񏉊���							*/
/* ����			: void										*/
/* �߂�l		: void										*/
/* �쐬��		: (OFT)Toguchi								*/
/* �쐬��		: 17/01/20									*/
/* ���l			: 											*/
/************************************************************/
ADVC_LOCAL void advance_control_initInspectionInfo(void)
{
	(void)memset(&S_advance_inspection_info, 0, sizeof(S_advance_inspection_info));
	S_advance_inspection_info.reqflg = N_OFF;

	return;
}

/************************************************************/
/* �֐�����		: advance_control_inspection				*/
/* �@�\�̐���	: ����										*/
/* ����			: void										*/
/* �߂�l		: void										*/
/* �쐬��		: (OFT)Toguchi								*/
/* �쐬��		: 17/01/20									*/
/* ���l			: 											*/
/************************************************************/
ADVC_LOCAL void advance_control_inspection(void)
{
	UI_16	data_len;

	if (S_advance_inspection_info.reqflg != N_ON) {
		return;
	}

	/* UART���M�\�f�[�^���擾	*/
	data_len = Uart_get_can_set_tx_data_len();
	if (data_len > 0) {						/* UART���M�\(���M�o�b�t�@�ɋ󂫂���)	*/

		/* UART�f�[�^��M	*/
		if (data_len > ADVANCE_UART_DATA_BUF_SIZE) {
			/* ��M�f�[�^���f�[�^�i�[�o�b�t�@�T�C�Y�ȉ��ɂ���	*/
			data_len = ADVANCE_UART_DATA_BUF_SIZE;
		}
		data_len = Uart_get_rx_data(S_advance_uart_data, data_len);
		if (data_len > 0) {					/* UART��M�f�[�^����					*/

			/* ��M�����f�[�^�𑗐M����	*/
			(void)Uart_set_tx_data(S_advance_uart_data, data_len);
		}
	}

	return;
}

/************************************************************/
/* �֐�����		: advance_control_initBtComm				*/
/* �@�\�̐���	: BT�ʐM���[�h���䏉����					*/
/* ����			: void										*/
/* �߂�l		: void										*/
/* �쐬��		: (OFT)Toguchi								*/
/* �쐬��		: 17/01/20									*/
/* ���l			: 											*/
/************************************************************/
ADVC_LOCAL void advance_control_initBtComm(void)
{
	T_DEFI_UART_PROTOCOL	protocol;

	/* UART�ʐM�ݒ�	*/
	protocol.parity		= DEFI_UART_PARITY_NONE;
	protocol.baud_rate	= DEFI_UART_BAUD_RATE_115200;
	(void)Uart_set_comm_protocol(&protocol);

	/* UART�ʐM�J�n	*/
	Uart_start();

	return;
}

/************************************************************/
/* �֐�����		: advance_control_btComm					*/
/* �@�\�̐���	: BT�ʐM���[�h����							*/
/* ����			: void										*/
/* �߂�l		: void										*/
/* �쐬��		: (OFT)Toguchi								*/
/* �쐬��		: 17/01/20									*/
/* ���l			: 											*/
/************************************************************/
ADVC_LOCAL void advance_control_btComm(void)
{
	UI_16	data_len;

	/* UART���M�\�f�[�^���擾	*/
	data_len = Uart_get_can_set_tx_data_len();
	if (data_len > 0) {						/* UART���M�\(���M�o�b�t�@�ɋ󂫂���)	*/

		if (data_len > ADVANCE_UART_DATA_BUF_SIZE) {
			/* �f�[�^�T�C�Y���f�[�^�i�[�o�b�t�@�T�C�Y�ȉ��ɂ���	*/
			data_len = ADVANCE_UART_DATA_BUF_SIZE;
		}
		/* BT�f�[�^�擾	*/
		data_len = Bt_control_getUartData(S_advance_uart_data, data_len);
		if (data_len > 0) {					/* BT��M�f�[�^����					*/
			
			/* ��M�����f�[�^�𑗐M����	*/
			(void)Uart_set_tx_data(S_advance_uart_data, data_len);
		}
	}

	return;
}

/************************************************************/
/* �֐�����		: advance_control_initGnssComm				*/
/* �@�\�̐���	: GNSS�ʐM���[�h���䏉����					*/
/* ����			: void										*/
/* �߂�l		: void										*/
/* �쐬��		: (OFT)Toguchi								*/
/* �쐬��		: 17/01/20									*/
/* ���l			: 											*/
/************************************************************/
ADVC_LOCAL void advance_control_initGnssComm(void)
{
	T_DEFI_UART_PROTOCOL	protocol;

	/* UART�ʐM�ݒ�	*/
	protocol.parity		= DEFI_UART_PARITY_NONE;
	if (Sw_control_GetSwMode() == SW_MODE_MANUAL) {
		S_advance_gnsscomm_brate = DEFI_UART_BAUD_RATE_9600;
	}
	else {
		S_advance_gnsscomm_brate = DEFI_UART_BAUD_RATE_115200;
	}
	protocol.baud_rate	= S_advance_gnsscomm_brate;
	(void)Uart_set_comm_protocol(&protocol);

	/* UART�ʐM�J�n	*/
	Uart_start();

	return;
}

/************************************************************/
/* �֐�����		: advance_control_gnssComm					*/
/* �@�\�̐���	: GNSS�ʐM���[�h����						*/
/* ����			: void										*/
/* �߂�l		: void										*/
/* �쐬��		: (OFT)Toguchi								*/
/* �쐬��		: 17/01/20									*/
/* ���l			: 											*/
/************************************************************/
ADVC_LOCAL void advance_control_gnssComm(void)
{
	T_DEFI_UART_PROTOCOL	protocol;
	UI_16	data_len;

	/* SW�ɂ��{�[���[�g�؂�ւ����`�F�b�N	*/
	protocol.baud_rate = DEFI_UART_BAUD_RATE_NUM;
	if (Sw_control_GetSwMode() == SW_MODE_MANUAL) {
		if (S_advance_gnsscomm_brate != DEFI_UART_BAUD_RATE_9600) {
			protocol.baud_rate = DEFI_UART_BAUD_RATE_9600;
		}
	}
	else {
		if (S_advance_gnsscomm_brate != DEFI_UART_BAUD_RATE_115200) {
			protocol.baud_rate = DEFI_UART_BAUD_RATE_115200;
		}
	}
	if (protocol.baud_rate != DEFI_UART_BAUD_RATE_NUM) {
		/* UART��~ */
		Uart_stop();

		/* �ʐM�ݒ� */
		protocol.parity = DEFI_UART_PARITY_NONE;
		(void)Uart_set_comm_protocol(&protocol);

		/* UART�J�n */
		Uart_start();

		/* ���݂̃{�[���[�g��ێ� */
		S_advance_gnsscomm_brate = protocol.baud_rate;
	}

	/* UART���M�\�f�[�^���擾	*/
	data_len = Uart_get_can_set_tx_data_len();
	if (data_len > 0) {						/* UART���M�\(���M�o�b�t�@�ɋ󂫂���)	*/

		if (data_len > ADVANCE_UART_DATA_BUF_SIZE) {
			/* ��M�f�[�^���f�[�^�i�[�o�b�t�@�T�C�Y�ȉ��ɂ���	*/
			data_len = ADVANCE_UART_DATA_BUF_SIZE;
		}
		/* GNSS�f�[�^�擾	*/
		data_len = Gnss_control_getUartData(S_advance_uart_data, data_len);
		if (data_len > 0) {					/* GNSS��M�f�[�^����					*/

			/* ��M�����f�[�^�𑗐M����	*/
			(void)Uart_set_tx_data(S_advance_uart_data, data_len);
		}
	}

	return;
}
