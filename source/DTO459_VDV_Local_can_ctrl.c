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
/* file DTO459_VDV_Local_canctrl.c						*/
/* Local CAN����										*/
/********************************************************/
/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/

#include <type.h>
#include <system.h>
#include <string.h>
#include "DTO459_VDV_Local_can_ctrl.h"
#include "System_control_TASK.h"
#include "DTO459_VDV_Can_Drv.h"
#include "DTO459_VDV_Signal_ctrl.h"
#include "timer_ctrl.h"
#include "dbglog.h"
	
/********************************************************/
/*	�}�N����`											*/
/********************************************************/
#define LCAN_CTRL_LOCAL	static

#define CAN_CH_LOCAL_CAN			CAN_CH_1		/* Local CAN��CAN�`�����l�� */

#define SEND_BUFFER_COUNT			10				/* CAN�̑��M�o�b�t�@��		*/

#define MAX_SIGNAL_COUNT_FIXED		12				/* �ő�M�����i�Œ�l�j		*/
#define MAX_SIGNAL_COUNT_VARIABLE	50				/* �ő�M�����i�ϐ��l�j		*/

#define INTERVAL_GET_SIGNAL_DATA_FIXED		10000			/* (�Œ�l�j�M���擾���ԊԊu(ms)		*/
#define INTERVAL_GET_SIGNAL_DATA_VARIABLE	100				/* (�ϐ��l�j�M���擾���ԊԊu(ms)		*/

#define SIGNAL_ID_ENCODE_KEY				100				/* �G���R�[�h�L�[�̐M��ID				*/

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
LCAN_CTRL_LOCAL UI_32 S_get_signal_data_timestamp_fixed;							/* �ԗ��M���f�[�^(�Œ�l�j�擾�^�C���X�^���v	*/
LCAN_CTRL_LOCAL UI_32 S_get_signal_data_timestamp_variable;						/* �ԗ��M���f�[�^(�ϐ��l�j�擾�^�C���X�^���v	*/
LCAN_CTRL_LOCAL UI_8  S_signal_count_fixed;										/* �M�����i�Œ�l�j								*/
LCAN_CTRL_LOCAL UI_8  S_signal_count_variable;									/* �M�����i�ϐ��l�j								*/
LCAN_CTRL_LOCAL const UI_16 *S_signal_id_list_fixed;								/* �M��ID�l�ꗗ�i�Œ�l�j						*/
LCAN_CTRL_LOCAL const UI_16 *S_signal_id_list_variable;							/* �M��ID�l�ꗗ�i�ϐ��l�j						*/
LCAN_CTRL_LOCAL SI_32 S_signal_data_list_fixed[MAX_SIGNAL_COUNT_FIXED];			/* �M���f�[�^�l�ꗗ�i�Œ�l�j					*/
LCAN_CTRL_LOCAL SI_32 S_signal_data_list_variable[MAX_SIGNAL_COUNT_VARIABLE];	/* �M���f�[�^�l�ꗗ�i�ϐ��l�j					*/
LCAN_CTRL_LOCAL UI_8  S_waiting_signal_count_fixed;								/* ���M�҂��M�����i�Œ�l�j						*/
LCAN_CTRL_LOCAL UI_8  S_waiting_signal_count_variable;							/* ���M�҂��M�����i�ϐ��l�j						*/
LCAN_CTRL_LOCAL UI_8  S_waiting_signal_count_encode_key;							/* ���M�҂��M�����i�G���R�[�h�L�[�j				*/
LCAN_CTRL_LOCAL SI_32 S_lcan_encode_key;											/* Local CAN�p�G���R�[�h�L�[					*/

/********************************************************/
/*	�����֐�											*/
/********************************************************/
LCAN_CTRL_LOCAL SI_8 get_signal_data_fixed(void);
LCAN_CTRL_LOCAL SI_8 get_signal_data_variable(void);
LCAN_CTRL_LOCAL void send_signal_data(void);
LCAN_CTRL_LOCAL E_RC set_send_signal_data(UI_8 send_buffer_index, UI_16 signal_id, UI_32 signal_data);

/********************************************************/
/* �֐�����		: Local_can_ctrl_Init					*/
/* �@�\�̐���	: Local CAN���䏉����					*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/10/11								*/
/* ���l			: 										*/
/********************************************************/
void Local_can_ctrl_Init(void)
{
	UI_16 i;

	/* �M���f�[�^�擾�^�C���X�^���v�N���A */
	S_get_signal_data_timestamp_fixed    = 0;
	S_get_signal_data_timestamp_variable = 0;

	/* �M�����N���A */
	S_signal_count_fixed    = 0;
	S_signal_count_variable = 0;

	/* �M��ID�l�ꗗ(�|�C���^)������ */
	S_signal_id_list_fixed    = NULL;
	S_signal_id_list_variable = NULL;

	/* �M���f�[�^�l�ꗗ(����)������ */
	for(i = 0; i < MAX_SIGNAL_COUNT_FIXED; i++) {
		S_signal_data_list_fixed[i] = 0;
	}
	for(i = 0; i < MAX_SIGNAL_COUNT_VARIABLE; i++) {
		S_signal_data_list_variable[i] = 0;
	}

	/* ���M�҂��M�����N���A */
	S_waiting_signal_count_fixed    = 0;
	S_waiting_signal_count_variable = 0;
	S_waiting_signal_count_encode_key = 0;

	/* Local CAN�p�G���R�[�h�L�[�N���A */
	S_lcan_encode_key = 0;

	/* CAN�h���C�o�̏����� */
	Can_drv_ch_init(CAN_CH_LOCAL_CAN, SEND_BUFFER_COUNT);
}


/********************************************************/
/* �֐�����		: Local_can_ctrl_Main					*/
/* �@�\�̐���	: Local CAN���䃁�C��					*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/10/11								*/
/* ���l			: 										*/
/********************************************************/
void Local_can_ctrl_Main(void)
{
	UI_32 cur_time = Timer_ctrl_GetTime1ms();

	if(S_lcan_encode_key == 0) {
		/* �Í����L�[���擾�Ȃ�Í��p�������� */
		S_lcan_encode_key = Signal_control_GetRandomValue();

		/* Local CAN�p�Í����L�[�ݒ� */
		(void)Signal_control_SetEncodeKey( DEVICE_TYPE_CAN, S_lcan_encode_key );
	}

	/* �Œ�l�M���f�[�^�擾�^�C�~���O�����`�F�b�N */
	if((S_get_signal_data_timestamp_fixed == 0)
	|| ((cur_time - S_get_signal_data_timestamp_fixed) >= INTERVAL_GET_SIGNAL_DATA_FIXED)) {
		/* �Œ�l�M���f�[�^�擾 */
		if(get_signal_data_fixed() == N_OK) {
			/* �Œ�l�M���f�[�^�擾�^�C���X�^���v�X�V */
			S_get_signal_data_timestamp_fixed = cur_time;

			/* ���ׂĂ̌Œ�M���𑗐M�҂��ɂ��� */
			S_waiting_signal_count_fixed = S_signal_count_fixed;
		}
	}

	/* �ϐ��l�M���f�[�^�擾�^�C�~���O�����`�F�b�N */
	if((S_get_signal_data_timestamp_variable == 0)
	|| ((cur_time - S_get_signal_data_timestamp_variable) >= INTERVAL_GET_SIGNAL_DATA_VARIABLE)) {
		/* �ϐ��l�M���f�[�^�擾 */
		if(get_signal_data_variable() == N_OK) {
			/* �ϐ��l�M���f�[�^�擾�^�C���X�^���v�X�V */
			S_get_signal_data_timestamp_variable = cur_time;

			/* ���ׂĂ̕ϐ��M���𑗐M�҂��ɂ��� */
			S_waiting_signal_count_variable = S_signal_count_variable;

/*
			for(index = 0; index < S_signal_count_variable; index++) {
				if(S_signal_id_list_variable[index] == 221) {
					OUTLOG(OutputLog("**** send data[ID=221, data = %08x] ****\n",  S_signal_data_list_variable[index]);)
				}
			}
*/
		}
	}

	/* ���M�҂��M�������݂����Local CAN ���M�������s */
	if((S_waiting_signal_count_fixed > 0) || (S_waiting_signal_count_variable > 0) || (S_waiting_signal_count_encode_key > 0)) {
		send_signal_data();
	}
}

/********************************************************/
/* �֐�����		: get_signal_data_fixed					*/
/* �@�\�̐���	: �Œ�l�M���f�[�^�擾					*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/10/11								*/
/* ���l			: 										*/
/********************************************************/
LCAN_CTRL_LOCAL SI_8 get_signal_data_fixed(void)
{
	SI_8 result;
	
	/* �M��ID�ꗗ���擾�ł��Ă��Ȃ���Ύ擾���� */
	if(S_signal_id_list_fixed == NULL) {
		result = Signal_control_GetSignalList(DEVICE_TYPE_CAN, ID_TYPE_FIXED_ID, &S_signal_count_fixed, &S_signal_id_list_fixed);
		if(result != N_OK) {
			S_signal_count_fixed   = 0;
			S_signal_id_list_fixed = NULL;
		}
		
		/* �M�������ő�l�𒴂��Ă�����A�ő�l�Ƃ���i�t�F�[���Z�[�t�j */
		if(S_signal_count_fixed > MAX_SIGNAL_COUNT_FIXED) {
			S_signal_count_fixed = MAX_SIGNAL_COUNT_FIXED;
		}
	}

	/* �M���f�[�^��擾 */
	result = Signal_control_GetSignalsInfo(DEVICE_TYPE_CAN, S_signal_count_fixed, S_signal_id_list_fixed, S_signal_data_list_fixed);
	if(result != N_OK) {
		return N_NG;
	}

	return N_OK;
}

/********************************************************/
/* �֐�����		: get_signal_data_variable				*/
/* �@�\�̐���	: �ϐ��l�M���f�[�^�擾					*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/10/11								*/
/* ���l			: 										*/
/********************************************************/
LCAN_CTRL_LOCAL SI_8 get_signal_data_variable(void)
{

	SI_8 result;
	
	/* �M��ID�ꗗ���擾�ł��Ă��Ȃ���Ύ擾���� */
	if(S_signal_id_list_variable == NULL) {
		result = Signal_control_GetSignalList(DEVICE_TYPE_CAN, ID_TYPE_VARIABLE_ID, &S_signal_count_variable, &S_signal_id_list_variable);
		if(result != N_OK) {
			S_signal_count_variable   = 0;
			S_signal_id_list_variable = NULL;
		}
		
		/* �M�������ő�l�𒴂��Ă�����A�ő�l�Ƃ���i�t�F�[���Z�[�t�j */
		if(S_signal_count_variable > MAX_SIGNAL_COUNT_VARIABLE) {
			S_signal_count_variable = MAX_SIGNAL_COUNT_VARIABLE;
		}
	}

	/* �M���f�[�^��擾 */
	result = Signal_control_GetSignalsInfo(DEVICE_TYPE_CAN, S_signal_count_variable, S_signal_id_list_variable, S_signal_data_list_variable);
	if(result != N_OK) {
		return N_NG;
	}

	return N_OK;
}

/********************************************************/
/* �֐�����		: send_signal_data						*/
/* �@�\�̐���	: �M���f�[�^���M����					*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/10/11								*/
/* ���l			: 										*/
/********************************************************/
LCAN_CTRL_LOCAL void send_signal_data(void)
{

	E_RC result;
	UI_8 send_index;
	UI_8 data_index;
	UI_8 msg_no = 1;
	SI_32 encode_key;
	
	/* �ϐ��l�M���̑��M�҂��f�[�^�����݂��邩�H */
	if(S_waiting_signal_count_variable > 0) {
		/* ���M�o�b�t�@�P���c���āA�ϐ��l�M���𑗐M���� */
		for(send_index = 0; send_index < (SEND_BUFFER_COUNT - 1); send_index++) {
			if (S_waiting_signal_count_variable <= 0) {
				break;
			}

			/* ���񑗐M����M���C���f�b�N�X���� */
			data_index = S_signal_count_variable - S_waiting_signal_count_variable;

			/* ���M�M����CAN�h���C�o�ɐݒ� */
			result = set_send_signal_data(msg_no, S_signal_id_list_variable[data_index],
													(UI_32)S_signal_data_list_variable[data_index]);
			if(result == N_OK) {
				/* ���M�҂����f�N�������g */
				S_waiting_signal_count_variable--;
				msg_no++;
			}
		}
	}

	/* �Œ�l�M���̑��M�҂��f�[�^�����݂��邩�H */
	if(S_waiting_signal_count_fixed > 0) {
		/* ���񑗐M����M���C���f�b�N�X���� */
		data_index = S_signal_count_fixed - S_waiting_signal_count_fixed;

		/* ���M�M����CAN�h���C�o�ɐݒ� */
		result = set_send_signal_data(msg_no, S_signal_id_list_fixed[data_index],
													(UI_32)S_signal_data_list_fixed[data_index]);

		if(result == N_OK) {
			/* ���M�҂����f�N�������g */
			S_waiting_signal_count_fixed--;
			msg_no++;

			/* �Œ�l�M����S�����M������G���R�[�h�L�[�𑗐M�҂��ɂ��� */
			if(S_waiting_signal_count_fixed <= 0) {
				S_waiting_signal_count_encode_key = 1;
			}
		}
	}
	/* �G���R�[�h�L�[�����M�҂����H */
	else if(S_waiting_signal_count_encode_key > 0) {
		/* ���M�M����CAN�h���C�o�ɐݒ� */
		(void)Signal_control_GetEncodeKey(DEVICE_TYPE_CAN, &encode_key);
		result = set_send_signal_data(msg_no, SIGNAL_ID_ENCODE_KEY, (UI_32)encode_key);
		if(result == N_OK) {
			/* ���M�҂����f�N�������g */
			S_waiting_signal_count_encode_key--;
			msg_no++;
		}
	}
	else {
		/* nop */
	}
}

/********************************************************/
/* �֐�����		: set_send_signal_data					*/
/* �@�\�̐���	: ���M�M���f�[�^�ݒ菈��				*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Katsura							*/
/* �쐬��		: 16/10/11								*/
/* ���l			: 										*/
/********************************************************/
LCAN_CTRL_LOCAL E_RC set_send_signal_data(UI_8 send_buffer_index, UI_16 signal_id, UI_32 signal_data)
{

	E_RC  result;
	UI_32 msg_id;
	UI_8  msg_data[4];

	msg_id = (UI_32)signal_id;

	msg_data[0] = (UI_8)(signal_data >> 24);
	msg_data[1] = (UI_8)((signal_data >> 16) & 0xff);
	msg_data[2] = (UI_8)((signal_data >>  8) & 0xff);
	msg_data[3] = (UI_8)(signal_data & 0xff);

	result = Can_drv_set_tx_data(CAN_CH_LOCAL_CAN, send_buffer_index, msg_id, msg_data, 4);
	return result;
}
