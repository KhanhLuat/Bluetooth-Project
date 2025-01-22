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
/* file DTO459_VDV_Advance_ctrl.h						*/
/* AdvanceControlUnit����								*/
/********************************************************/
#ifndef	DTO459_VDV_ADVANCE_CTRL_H
#define	DTO459_VDV_ADVANCE_CTRL_H

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include "type.h"

/********************************************************/
/*	�}�N����`											*/
/********************************************************/

typedef struct t_advance_ctrl_unit_info {
	UI_8	invalidFlag;			/* �����t���O�i����M����TRUE) */
	UI_16	VehicleSpeed;			/* �ԑ� */
	UI_16	EngineRevolutionSpeed;	/* �G���W����]�� */
	UI_16	TurboPressure;			/* �^�[�{��(�ߋ���) */
	UI_16	EngineOilPressure;		/* �G���W������ */
	UI_16	FuelPressure;			/* �R������ */
	UI_8	DifferentialPressure;	/* ���� */
	UI_16	EngineOiTemperture;		/* �G���W������ */
	UI_16	EngineCoolantTemperture;/* �G���W������ */
	UI_16	ExhaustGasTemperture;	/* �r�C���x */
	UI_16	Volts;					/* �d�� */
} T_ADVANCE_CTRL_UNIT_INFO;


/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/

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
void Advance_control_Init(void);

/********************************************************/
/* �֐�����		: Advance_control_Main					*/
/* �@�\�̐���	: AdvanceControlUnit���䃁�C��			*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Advance_control_Main(void);

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
void Advance_control_GetData(T_ADVANCE_CTRL_UNIT_INFO **data);

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
void Set_real_mode_data(const UI_8 *data_p, UI_8 len);

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
UI_8 Get_real_mode_data(UI_8 *data_p);

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
SI_8 Advance_control_ReqInspection(void);

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
UI_16 Advance_control_getUartData(UI_8 *buf, UI_16 len);

/********************************************************/
/* �֐�����		:	Advance_control_getUartDataLen		*/
/* �@�\�̐���	:	UART�f�[�^���擾					*/
/* ����			:	void								*/
/* �߂�l		:	UART�f�[�^��(byte)					*/
/* �쐬��		:	(OFT)Toguchi						*/
/* �쐬��		:	17/01/20							*/
/* ���l			:										*/
/********************************************************/
UI_16 Advance_control_getUartDataLen(void);

#endif	/*DTO459_VDV_ADVANCE_CTRL_H*/
