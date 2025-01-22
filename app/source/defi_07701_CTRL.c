/********************************************************************************/
/* file defi_07701_CTRL.c														*/
/* Defi���j�b�g����(DF07701/ýĎ���)											*/
/* 																				*/
/* Defi���j�b�g(DF07701/ýĎ���)�𐧌䂷��B									*/
/* <ul>																			*/
/* <li>DF07701�ƒʐM���AOP�ް��Aر��ް����擾����B								*/
/* <li>ýĎ���ƒʐM���AMFi�F������ ���ýĂ����s�A�ر��ް��̓Ǐ��������s����B	*/
/* </ul>																		*/
/********************************************************************************/
/* author      Y.Sorimachi														*/
/* date        2014/4/11														*/
/********************************************************************************/
#ifdef ADVANCE_ENABLE

#include <string.h>														/* �W��ͯ�ް̧�� */
#include "system.h"
#include "type.h"
#include "Timer.h"
#include "defi_uart_prt.h"												/* Defi UART���ĺ�ͯ�ް̧�� */
#include "DTO459_VDV_Advance_ctrl.h"
#include "defi_07701_CTRL.h"											/* DF07701����ͯ�ް̧�� */
#include "timer_ctrl.h"

/*--------------------------------------------------------------------------*/
/*	�����}�N��																*/
/*--------------------------------------------------------------------------*/
#define		DF7_MODE_POS				(2)								/**< Defi����Ӱ�޺��� �޲Ĉʒu 							*/
#define		DF7_MODE_OPENING			(0x01)							/**< Defi����Ӱ�޺��� �����ݸ�							*/
#define		DF7_REQUEST_POS				(22)							/**< Ұ��ڑ��v�� �޲Ĉʒu 								*/
#define		DF7_CONNECT_REQ				(0x05)							/**< �ڑ��v��ظ��Ď��ʎq 								*/

#define		DF7_HEAD_POS				(0)								/**< Defi����ͯ�޺��� �޲Ĉʒu 							*/
#define		DF7_MODE0_ENDING			(0x02)							/**< Defi����Ӱ�޺��� ���ިݸ� 0�޲Ė�					*/
#define		DF7_MODE1_ENDING			(0x00)							/**< Defi����Ӱ�޺��� ���ިݸ� 1�޲Ė�					*/
#define		DF7_MODE1_MASK				(0xF0)							/**< Defi����Ӱ�޺��� 1�޲Ė� Ͻ�						*/
/** 
 * DF�ڑ����
 */ 
typedef enum e_df_state
{
	DF_STATE_DISCONNECT	= 0,		/**< DF���ڑ�			*/
	DF_STATE_CONNECT,				/**< DF�ڑ�				*/
	DF_STATE_ERR_CONNECT,			/**< �ڑ��ُ�			*/
	DF_STATE_NUM					
} E_DF_STATE;


/*--------------------------------------------------------------------------*/
/*	�萔�錾																*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*	�\���̐錾																*/
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*	�����ϐ���`															*/
/*--------------------------------------------------------------------------*/
static	E_DF_STATE		S_df_state;							/**< Defi�ʐM����̐ڑ���� */
static	UI_16	S_df_connect_judge_start_timer;				/**< DF�ڑ�����J�n�J�n�^�C�} */
static	UI_8	S_df_connect_judge_flg;						/**< DF�ڑ�����t���O */


/*--------------------------------------------------------------------------*/
/*	�����֐�																*/
/*--------------------------------------------------------------------------*/
static UI_8 defi_07701_rx_data_analyze(const UI_8 *data_p, const UI_8 data_len);	/* DF07701��M�ް���͏��� 		; ADD CHG-028(LX039) */
static B_8 S_judge_ending_mode( const UI_8 *data_p );
static void S_df_set_state( const E_DF_STATE state );

/** 
 * RAM������
 * 
 * author	Y.Sorimachi
 * date	2014/4/11
 * note	DF07701����Ŏg�p����RAM�̏������������s���B
 */ 
void Defi_07701_control_Init( void )
{
	T_DEFI_UART_PROTOCOL	protocol;

	/**	<ol>	*/
 	/**	<li>DF07701����Ŏg�p����RAM�̏������������s���B */
	S_df_state								= DF_STATE_DISCONNECT;
	S_df_connect_judge_start_timer			= 0;
	S_df_connect_judge_flg					= N_OFF;

	/**	</ol>	*/
	Uart_ram_init();
	
	/* UART�ʐM�ݒ�	*/
	protocol.parity		= DEFI_UART_PARITY_EVEN;
	protocol.baud_rate	= DEFI_UART_BAUD_RATE_19200;
	(void)Uart_set_comm_protocol(&protocol);

	Uart_start();
	return;
}


/** 
 * DF07701����Ҳݏ��� 
 * 
 * author	Y.Sorimachi
 * date	2014/4/11
 * note	<ul>
 *			<li>DF07701�ƒʐM���AOP�ް���Disp No.1�`7�ڑ��v����4��ȏ�擾�����Ƃ��A�ڑ��v��ڽ��ݽ�𑗐M����B
 *			<li>DF07701�ƒʐM���AOP�ް��AReal�ް���Defi���j�b�g����ɐݒ肷��B
 *			<li>ý�Ӱ�ގ���ýĎ���ƒʐM���AMFi�F������ ���ýĂ����s�A�ر��ް��̓Ǐ��������s���A���ʂ𑗐M����B
 *			</ul>
 */ 
void Defi_07701_control_main(void)
{
	UI_8	rx_data[34];													/* ��M�ް��i�[�ޯ̧ */
	UI_8	rx_len;															/* ��M�ް��� */
	UI_8	rx_frame;														/* ��M�ް��擾�߂�l */
	UI_8	analyze_jdg;													/* ��M�ް���͌��� */
	UI_8	tx_data[6];														/* ���M�ް��ҏW�ޯ̧ */

	/**	<li> UART1��M�f�[�^��� */
	Uart_rx_data_analyze();
	(void)memset(rx_data, 0, sizeof(rx_data));
	rx_len = 0;
	
	switch ( S_df_state )
	{
	case (DF_STATE_DISCONNECT):
	case (DF_STATE_CONNECT):
		rx_frame = Get_uart_rx_data_defi(rx_data, &rx_len);
		analyze_jdg = 0;
		(void)memset(tx_data, 0, sizeof(tx_data));

		if (rx_frame != 0xFF) {
			S_df_connect_judge_flg	= N_OFF;
			analyze_jdg = defi_07701_rx_data_analyze(rx_data, rx_len);		/* �����ǉ�						; ADD CHG-028(LX039) */
			switch (analyze_jdg) {

			case 1:
				Set_real_mode_data(rx_data, rx_len);
				break;

			case 6:															/* ED��M */
				/* DF���ڑ� */
				S_df_set_state(DF_STATE_DISCONNECT);
				
				break;
			default:
				break;
			}
		} else {
			/* DF�ڑ����ɁA��M�f�[�^�s���܂��̓f�[�^����M�������Ƃ��ADF�ڑ��ُ픻����s���B */
			if ( S_df_state == DF_STATE_CONNECT ) {
				if ( S_df_connect_judge_flg == N_OFF ) {
					/* �ڑ�����J�n */
					S_df_connect_judge_start_timer =  Timer_ctrl_GetTime10ms();
					S_df_connect_judge_flg	= N_ON;
				} else {
					if ( Timer_ctrl_TimeCompare10ms(S_df_connect_judge_start_timer, TIME_MAIN_1S) == N_OK ) {
						/* DF�ُ�ڑ� */
						S_df_set_state(DF_STATE_ERR_CONNECT);
					}
				}
			}
			
		}
		break;

	case (DF_STATE_ERR_CONNECT):
		/* ��M�f�[�^�̂ݎ擾���� */
		rx_frame = Get_uart_rx_data_defi(rx_data, &rx_len);
		break;
	default:
		break;
	}
}

/********************************************************/
/*	�����֐�											*/
/********************************************************/
/** 
 * DF07701��M�ް���͏���
 * 
 * param	*data_p		[in]	��M�ް��i�[�ޯ̧���߲��
 * param	data_len	[in]	��M�ް��i�[�ޯ̧���ް��� 
 *
 * return	��M�ް���͌���  
 * retval	0 : �����ݸ�Ӱ�ސڑ�ڽ��ݽOK
 * retval	1 : ر�Ӱ���ް���M
 * retval	2 : ý�Ӱ�ޑJ�ڗv����M
 * retval	6 : ���ިݸ�Ӱ���ް���M
 * 
 * author	Y.Sorimachi
 * date	2014/4/11
 * note	<ul>
 *			<li>�����ݸ�Ӱ���ް���DispNo.1�`7�̐ڑ�ظ��Ă����ꂼ��4��ȏ� ��M�����ꍇ�ɐڑ�ڽ��ݽOK��߂�l�ɂ���B
 *			<li>ED��M���AED��M��߂�l�ɂ���B
 *			<li>ر��ް���M���Aر��ް���M��߂�l�ɂ���B
 *			<li>Dý�Ӱ�ނւ̑J�ڗv���̺���ގ�M���Aý�Ӱ�ނւ̑J�ڗv����M��߂�l�ɂ���B 
 *			</ul>
 */ 
static UI_8 defi_07701_rx_data_analyze(const UI_8 *data_p, const UI_8 data_len)		/* �����ǉ�			; ADD CHG-028(LX039) */
{
	UI_8	ret;														/* �߂�l */

	ret = 0xFF;

	if (data_p[DF7_MODE_POS] == DF7_MODE_OPENING) {
		if (data_p[DF7_REQUEST_POS] == DF7_CONNECT_REQ) { 											/*	; ADD REQ-011(LX039) */

			S_df_set_state(DF_STATE_CONNECT);								/* DF�ڑ� */
		}	
	} else {
		if ( S_judge_ending_mode(data_p) == N_TRUE) {
			ret = 6;													/* ���ިݸ�Ӱ���ް���M��ݒ� */
		} else {														/*									; ADD REQ-011(LX039) */
			ret = 1;
		}																/* ر��ް���M						; ADD REQ-011(LX039) */
	}

	return ret;
}

/** 
 * �G���f�B���O���[�h����
 * 
 * param	data_p	[in]	��M�f�[�^
 * 
 * return	���茋��
 * retval	N_TRUE	���ިݸ�Ӱ�ގ�M
 * retval	N_FALSE	���ިݸ�Ӱ�ޖ���M
 * 
 * author	Y.Sorimachi
 * date	2014/4/11
 * note	�G���f�B���O���[�h����M���������肷��B
 */ 
static B_8 S_judge_ending_mode( const UI_8 *data_p )
{
	B_8 result = N_FALSE;
	/**	<ol>	*/
	/**	<li>�G���f�B���O���[�h����	*/	
	if ((data_p[DF7_MODE_POS]   == DF7_MODE0_ENDING) &&
		((data_p[DF7_MODE_POS] & DF7_MODE1_MASK)  == DF7_MODE1_ENDING ))
	{
		result = N_TRUE;
	}
	/**	</ol>	*/	
	return result;
}

/** 
 * ��Ԑݒ菈��
 * 
 * param	state	[in]	DF�ڑ����
 * 
 * author	Y.Sorimachi
 * date	2014/4/11
 * note	DF�ڑ���Ԃƒʒm�C�x���g��Ԃ��X�V����B
 */ 
static void S_df_set_state( const E_DF_STATE state )
{
	/**	<ol>	*/
	/**	<li>DF�ڑ���Ԃ�ݒ肷��	*/	
	S_df_state = state;
	/**	</ol>	*/	
	return;
}

#endif /* ADVANCE_ENABLE */
