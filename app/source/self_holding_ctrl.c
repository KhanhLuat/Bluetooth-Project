/****************************************************************/
/* file self_holding_ctrl.h										*/
/* ���ȕێ�����													*/
/* 																*/
/* ���ȕێ����������B											*/
/* <ul>															*/
/* <li>ACC��ON�ɂȂ����ꍇ�A���ȕێ�ON�o�͂�ACC ON�C�x���g��	*/
/* 		���s����B												*/
/* <li>ACC��OFF�ƂȂ����ꍇ�AACC OFF������J�n��ACC OFF�C�x���g	*/
/* 		�𔭍s����B											*/
/* <li>ACC��OFF�ɂȂ����ꍇ�i5sec�ȏ�ACC��OFF�ɂȂ����ꍇ�j�A	*/
/* 		ACC OFF����J�E���g5sec�o�߃C�x���g�𔭍s����B			*/
/* <li>�V�X�e����Ԃ��I�����[�h�̏ꍇ�A���ȕێ�OFF����B		*/
/* </ul>														*/
/* 																*/
/****************************************************************/
/* author      Y.Sorimachi										*/
/* date        2013/--/--										*/
/****************************************************************/

#include "system.h"
#include "Timer.h"	
#include "SSFTSTD_FR81_Port_Drv.h"
#include "timer_ctrl.h"
#include "System_control_TASK.h"
#include "chatt_absorber.h"
#include "self_holding_ctrl.h"

/********************************************************/
/*	�����}�N����`										*/
/********************************************************/
#define SELFHOLD_LOCAL	static
#define ACC_NOT_NOTIFY_EVENT			(SYS_EVENT_NUM)		/**< ACC�ʒm�C�x���g�Ȃ�	*/
#define SELF_OFF_DELAY_TIME				(10)				/**< 10ms �^�C�}�J�E���g	*/
/********************************************************/
/*	�����萔��`										*/
/********************************************************/
/**
 *	ACC������
 */
typedef enum e_acc_state {
	ACC_STATE_OFF	= 0,				/**< ACC���:OFF					*/
	ACC_STATE_ON,						/**< ACC���:ON						*/
	ACC_STATE_JUDGMENT_ON_TO_OFF,		/**< ACC���:ON����OFF�ւ̔��蒆	*/
	ACC_STATE_NUM
} E_ACC_STATE;

/**
 *	�ʒm�C�x���g
 */
SELFHOLD_LOCAL const UI_8 C_acc_notify_event[ACC_STATE_NUM] = 
{
	ACC_NOT_NOTIFY_EVENT,					/**< ACC_STATE_OFF					*/
	SYS_EVENT_ACC_ON,						/**< ACC_STATE_ON					*/
	SYS_EVENT_ACC_OFF						/**< ACC_STATE_JUDGMENT_ON_TO_OFF	*/
};

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
SELFHOLD_LOCAL UI_8 		S_acc_change_state;		/**< ACC�ω����		*/
SELFHOLD_LOCAL E_ACC_STATE 	S_acc_state;			/**< ACC���			*/
SELFHOLD_LOCAL UI_8			S_acc_val;				/**< ACC�̒l			*/
SELFHOLD_LOCAL E_SYS_CTRL_STATUS	S_acc_prev_system_state;/**< �O�̃V�X�e�����	*/
SELFHOLD_LOCAL UI_8			S_acc_noti_eve_state;	/**< �ʒm�C�x���g���	*/
SELFHOLD_LOCAL UI_16		S_self_off_delay_time;	/**< ���ȕێ�OFF�x������	*/

/********************************************************/
/*	�����֐��v���g�^�C�v�錾							*/
/********************************************************/
SELFHOLD_LOCAL void S_acc_set_state( const UI_8 state );
SELFHOLD_LOCAL void S_acc_self_cb( UI_8 ca_acc );

/********************************************************/
/*	�O���֐�											*/
/********************************************************/

/** 
 * /// ����������
 * 
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		<ul>
 *			<li>���ȕێ�����Ŏg�p�����RAM������������B
 *			<li>�`���^�����O�z�����W���[����ACC���R�[���o�b�N�����悤�v������B
 *			</ul>
 */ 
void Self_holding_ctrl_Init( void )
{
	/**	<ol>	*/
	/**	<li>RAM ������	*/
	S_acc_change_state 		= N_OFF;
	S_acc_prev_system_state	= SYS_OPERATION_WAIT_ACC_ON;
	S_acc_noti_eve_state	= ACC_NOT_NOTIFY_EVENT;
	S_acc_state				= ACC_STATE_OFF;
	S_acc_val				= N_LOW;
	S_self_off_delay_time	= 0;
	/**	<li>ACC���R�[���o�b�N�����悤�v������	*/
	(void)Chatt_absorber_ReqPortData(S_acc_self_cb, CHATT_PORT_ACC, CHATT_ABS_MOD_SHC);
	/**	</ol>	*/
	return;
}

/** 
 * ���C������
 * 
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		<ul>
 *			<li>ACC��ON�ɂȂ����ꍇ�A���ȕێ�ON�o�͂�ACC ON�C�x���g�𔭍s����B
 *			<li>ACC��OFF�ƂȂ����ꍇ�AACC OFF������J�n��ACC OFF�C�x���g�𔭍s����B
 *			<li>ACC��OFF�ɂȂ����ꍇ�i5sec�ȏ�ACC��OFF�ɂȂ����ꍇ�j�AACC OFF����J�E���g5sec�o�߃C�x���g�𔭍s����B
 *			<li>�V�X�e����Ԃ��I�����[�h�̏ꍇ�A���ȕێ�OFF����B
 *			</ul>
 */ 
void Self_holding_ctrl_Main( void )
{
	E_SYS_CTRL_STATUS system_state;
	/**	<ol>	*/
	/**	<li>ACC�ω���Ԃ�N_ON�̂Ƃ����s	*/	
	if ( S_acc_change_state == N_ON )
	{
		switch ( S_acc_state )
		{
		case ( ACC_STATE_OFF ):		/* ����ACC OFF */
		/**	<li>ACC ON����	*/
			if ( S_acc_val == IO_PI_LEV_ACTIVE_ACC_STAT )
			{
				S_acc_set_state(ACC_STATE_ON);
				/**	<li>���ȕێ�ON�o��	*/
				(void)PortDrv_SetDataBit(IO_PO_GR_5V_HOLD, IO_PO_BIT_5V_HOLD, IO_PO_LEV_ACTIVE_5V_HOLD);
				/**	<li>�|�[�g�i3.3V_EN�jON�o��	*/
				(void)PortDrv_SetDataBit(IO_PO_GR_33V_EN, IO_PO_BIT_33V_EN, IO_PO_LEV_ACTIVE_33V_EN);
				/**	<li>�|�[�g�iADC5V_EN�jON�o��	*/
				(void)PortDrv_SetDataBit(IO_PO_GR_ADC5V_EN, IO_PO_BIT_ADC5V_EN, IO_PO_LEV_ACTIVE_ADC5V_EN);
			}
			break;
		case ( ACC_STATE_ON ):		/* ����ACC ON */
		/**	<li>ACC OFF����	*/
			if ( S_acc_val == IO_PI_LEV_NONACT_ACC_STAT )
			{
				/**	<li>ACC ON����OFF�֔���J�n	*/
				S_acc_set_state(ACC_STATE_JUDGMENT_ON_TO_OFF);
			}
			break;
		case ( ACC_STATE_JUDGMENT_ON_TO_OFF ):		/* ����ACC ON->OFF�ւ̔��蒆 */
		/**	<li>ACC OFF���蒆��ACC�ω����������ꍇ	*/
			if ( S_acc_val == IO_PI_LEV_ACTIVE_ACC_STAT )
			{
				S_acc_set_state(ACC_STATE_ON);
			}
			break;
		default:
			/**	<li>���肦�Ȃ�����	*/
			break;
		}
		/**	<li>ACC�ω���Ԃ�N_OFF�ɂ���	*/
		S_acc_change_state = N_OFF;
	}

	/**	<li>�V�X�e����Ԃ��擾	*/
	system_state = System_control_GetStatus();

	/**	<li>ACC OFF���蒆�ɂȂ����Ƃ����s	*/
	if ( S_acc_state == ACC_STATE_JUDGMENT_ON_TO_OFF )
	{
		/**	<li>ACC ON����OFF�֔��蒆	*/
		if(system_state == SYS_OPERATION_ACC_OFF)
		{
			/* �V�X�e����Ԃ�ACC�QOFF�Ȃ�ACC�QOFF�Ƃ��� */
			S_acc_set_state(ACC_STATE_OFF);
		}
	}
	
	/**	<li>�ʒm�C�x���g�����������Ƃ��A�V�X�e����Ԑ���ɒʒm����	*/
	if ( S_acc_noti_eve_state != ACC_NOT_NOTIFY_EVENT )
	{
		/**	<li>�V�X�e����Ԑ���ɃC�x���g�ʒm����	*/
		(void)System_control_NotifyEvent((E_SYS_CTRL_EVENT)S_acc_noti_eve_state,SYS_EVENT_PARAM_NO);
		/**	<li>�C�x���g�ʒm��Ԃ��C�x���g�Ȃ��ɂ���	*/
		S_acc_noti_eve_state = ACC_NOT_NOTIFY_EVENT;
	}

	if ( S_acc_prev_system_state != system_state )
	{
		if (system_state == SYS_OPERATION_ACC_OFF)
		{
			if(S_self_off_delay_time == 0)
			{
				/**	���ȕێ�OFF�x�����Ԃ𖢕ێ��̏ꍇ�͎擾	*/
				S_self_off_delay_time = Timer_ctrl_GetTime10ms();
			}
			else
			{
				/**	���ȕێ�OFF�x�����Ԃ�ێ��̏ꍇ�͌o�ߎ��Ԃ��o�߂������m�F	*/
				if (Timer_ctrl_TimeCompare10ms(S_self_off_delay_time, SELF_OFF_DELAY_TIME/TIME_MAIN) == N_OK)
				{
					/**	���Ԍo��	*/
					/**	<li>�|�[�g�i3.3V_EN�jOFF�o��	*/
					(void)PortDrv_SetDataBit(IO_PO_GR_33V_EN, IO_PO_BIT_33V_EN, IO_PO_LEV_NONACT_33V_EN);
					/**	<li>�|�[�g�iADC5V_EN�jOFF�o��	*/
					(void)PortDrv_SetDataBit(IO_PO_GR_ADC5V_EN, IO_PO_BIT_ADC5V_EN, IO_PO_LEV_NONACT_ADC5V_EN);
					/**	<li>���ȕێ�OFF�o��	*/
					(void)PortDrv_SetDataBit(IO_PO_GR_5V_HOLD, IO_PO_BIT_5V_HOLD, IO_PO_LEV_NONACT_5V_HOLD);
					/**	<li>�V�X�e����Ԃ�ۑ�	*/
					S_acc_prev_system_state = system_state;
				}
				else
				{
					/**	���Ԗ��o��	*/
				}
			}
		}
		else
		{
			S_self_off_delay_time = 0;
		}
	}

	/**	</ol>	*/
	return;
}

/********************************************************/
/*	�����֐�											*/
/********************************************************/
/** 
 * /// ��Ԑݒ菈��
 * 
 * param	state	[in]	ACC���
 * 
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		ACC��Ԃƒʒm�C�x���g��Ԃ��X�V����B
 */ 
SELFHOLD_LOCAL void S_acc_set_state( const UI_8 state )
{
	/**	<ol>	*/
	/**	<li>ACC��Ԃ�ݒ肷��	*/	
	S_acc_state = (E_ACC_STATE)state;
	/**	<li>�ʒm�C�x���g��Ԃ�ݒ肷��	*/	
	S_acc_noti_eve_state = C_acc_notify_event[state];
	/**	</ol>	*/	
	return;
}

/********************************************************/
/*	�R�[���o�b�N�֐�									*/
/********************************************************/
SELFHOLD_LOCAL void S_acc_self_cb( UI_8 ca_acc )
{
	/**	<ol>	*/
	/**	<li>ACC�ω���Ԃ�N_ON�ɂ���	*/	
	S_acc_change_state = N_ON;
	/**	<li>ACC�̒l��ݒ肷��	*/	
	S_acc_val = ca_acc;
	/**	</ol>	*/	
	return;
}
