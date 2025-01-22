/********************************************************************************/
/* file defi_07701_CTRL.c														*/
/* Defiユニット制御(DF07701/ﾃｽﾄ治具)											*/
/* 																				*/
/* Defiユニット(DF07701/ﾃｽﾄ治具)を制御する。									*/
/* <ul>																			*/
/* <li>DF07701と通信し、OPﾃﾞｰﾀ、ﾘｱﾙﾃﾞｰﾀを取得する。								*/
/* <li>ﾃｽﾄ治具と通信し、MFi認証ﾁｯﾌﾟ ｾﾙﾌﾃｽﾄを実行、ｼﾘｱﾙﾃﾞｰﾀの読書きを実行する。	*/
/* </ul>																		*/
/********************************************************************************/
/* author      Y.Sorimachi														*/
/* date        2014/4/11														*/
/********************************************************************************/
#ifdef ADVANCE_ENABLE

#include <string.h>														/* 標準ﾍｯﾀﾞｰﾌｧｲﾙ */
#include "system.h"
#include "type.h"
#include "Timer.h"
#include "defi_uart_prt.h"												/* Defi UARTﾌﾟﾛﾄｺﾙﾍｯﾀﾞｰﾌｧｲﾙ */
#include "DTO459_VDV_Advance_ctrl.h"
#include "defi_07701_CTRL.h"											/* DF07701制御ﾍｯﾀﾞｰﾌｧｲﾙ */
#include "timer_ctrl.h"

/*--------------------------------------------------------------------------*/
/*	内部マクロ																*/
/*--------------------------------------------------------------------------*/
#define		DF7_MODE_POS				(2)								/**< Defi動作ﾓｰﾄﾞｺｰﾄﾞ ﾊﾞｲﾄ位置 							*/
#define		DF7_MODE_OPENING			(0x01)							/**< Defi動作ﾓｰﾄﾞｺｰﾄﾞ ｵｰﾌﾟﾆﾝｸﾞ							*/
#define		DF7_REQUEST_POS				(22)							/**< ﾒｰﾀ接続要求 ﾊﾞｲﾄ位置 								*/
#define		DF7_CONNECT_REQ				(0x05)							/**< 接続要求ﾘｸｴｽﾄ識別子 								*/

#define		DF7_HEAD_POS				(0)								/**< Defi動作ﾍｯﾀﾞｺｰﾄﾞ ﾊﾞｲﾄ位置 							*/
#define		DF7_MODE0_ENDING			(0x02)							/**< Defi動作ﾓｰﾄﾞｺｰﾄﾞ ｴﾝﾃﾞｨﾝｸﾞ 0ﾊﾞｲﾄ目					*/
#define		DF7_MODE1_ENDING			(0x00)							/**< Defi動作ﾓｰﾄﾞｺｰﾄﾞ ｴﾝﾃﾞｨﾝｸﾞ 1ﾊﾞｲﾄ目					*/
#define		DF7_MODE1_MASK				(0xF0)							/**< Defi動作ﾓｰﾄﾞｺｰﾄﾞ 1ﾊﾞｲﾄ目 ﾏｽｸ						*/
/** 
 * DF接続状態
 */ 
typedef enum e_df_state
{
	DF_STATE_DISCONNECT	= 0,		/**< DF未接続			*/
	DF_STATE_CONNECT,				/**< DF接続				*/
	DF_STATE_ERR_CONNECT,			/**< 接続異常			*/
	DF_STATE_NUM					
} E_DF_STATE;


/*--------------------------------------------------------------------------*/
/*	定数宣言																*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*	構造体宣言																*/
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*	内部変数定義															*/
/*--------------------------------------------------------------------------*/
static	E_DF_STATE		S_df_state;							/**< Defi通信制御の接続状態 */
static	UI_16	S_df_connect_judge_start_timer;				/**< DF接続判定開始開始タイマ */
static	UI_8	S_df_connect_judge_flg;						/**< DF接続判定フラグ */


/*--------------------------------------------------------------------------*/
/*	内部関数																*/
/*--------------------------------------------------------------------------*/
static UI_8 defi_07701_rx_data_analyze(const UI_8 *data_p, const UI_8 data_len);	/* DF07701受信ﾃﾞｰﾀ解析処理 		; ADD CHG-028(LX039) */
static B_8 S_judge_ending_mode( const UI_8 *data_p );
static void S_df_set_state( const E_DF_STATE state );

/** 
 * RAM初期化
 * 
 * author	Y.Sorimachi
 * date	2014/4/11
 * note	DF07701制御で使用するRAMの初期化処理を行う。
 */ 
void Defi_07701_control_Init( void )
{
	T_DEFI_UART_PROTOCOL	protocol;

	/**	<ol>	*/
 	/**	<li>DF07701制御で使用するRAMの初期化処理を行う。 */
	S_df_state								= DF_STATE_DISCONNECT;
	S_df_connect_judge_start_timer			= 0;
	S_df_connect_judge_flg					= N_OFF;

	/**	</ol>	*/
	Uart_ram_init();
	
	/* UART通信設定	*/
	protocol.parity		= DEFI_UART_PARITY_EVEN;
	protocol.baud_rate	= DEFI_UART_BAUD_RATE_19200;
	(void)Uart_set_comm_protocol(&protocol);

	Uart_start();
	return;
}


/** 
 * DF07701制御ﾒｲﾝ処理 
 * 
 * author	Y.Sorimachi
 * date	2014/4/11
 * note	<ul>
 *			<li>DF07701と通信し、OPﾃﾞｰﾀのDisp No.1～7接続要求を4回以上取得したとき、接続要求ﾚｽﾎﾟﾝｽを送信する。
 *			<li>DF07701と通信し、OPﾃﾞｰﾀ、RealﾃﾞｰﾀをDefiユニット制御に設定する。
 *			<li>ﾃｽﾄﾓｰﾄﾞ時にﾃｽﾄ治具と通信し、MFi認証ﾁｯﾌﾟ ｾﾙﾌﾃｽﾄを実行、ｼﾘｱﾙﾃﾞｰﾀの読書きを実行し、結果を送信する。
 *			</ul>
 */ 
void Defi_07701_control_main(void)
{
	UI_8	rx_data[34];													/* 受信ﾃﾞｰﾀ格納ﾊﾞｯﾌｧ */
	UI_8	rx_len;															/* 受信ﾃﾞｰﾀ長 */
	UI_8	rx_frame;														/* 受信ﾃﾞｰﾀ取得戻り値 */
	UI_8	analyze_jdg;													/* 受信ﾃﾞｰﾀ解析結果 */
	UI_8	tx_data[6];														/* 送信ﾃﾞｰﾀ編集ﾊﾞｯﾌｧ */

	/**	<li> UART1受信データ解析 */
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
			analyze_jdg = defi_07701_rx_data_analyze(rx_data, rx_len);		/* 引数追加						; ADD CHG-028(LX039) */
			switch (analyze_jdg) {

			case 1:
				Set_real_mode_data(rx_data, rx_len);
				break;

			case 6:															/* ED受信 */
				/* DF未接続 */
				S_df_set_state(DF_STATE_DISCONNECT);
				
				break;
			default:
				break;
			}
		} else {
			/* DF接続中に、受信データ不正またはデータ未受信だったとき、DF接続異常判定を行う。 */
			if ( S_df_state == DF_STATE_CONNECT ) {
				if ( S_df_connect_judge_flg == N_OFF ) {
					/* 接続判定開始 */
					S_df_connect_judge_start_timer =  Timer_ctrl_GetTime10ms();
					S_df_connect_judge_flg	= N_ON;
				} else {
					if ( Timer_ctrl_TimeCompare10ms(S_df_connect_judge_start_timer, TIME_MAIN_1S) == N_OK ) {
						/* DF異常接続 */
						S_df_set_state(DF_STATE_ERR_CONNECT);
					}
				}
			}
			
		}
		break;

	case (DF_STATE_ERR_CONNECT):
		/* 受信データのみ取得する */
		rx_frame = Get_uart_rx_data_defi(rx_data, &rx_len);
		break;
	default:
		break;
	}
}

/********************************************************/
/*	内部関数											*/
/********************************************************/
/** 
 * DF07701受信ﾃﾞｰﾀ解析処理
 * 
 * param	*data_p		[in]	受信ﾃﾞｰﾀ格納ﾊﾞｯﾌｧのﾎﾟｲﾝﾀ
 * param	data_len	[in]	受信ﾃﾞｰﾀ格納ﾊﾞｯﾌｧのﾃﾞｰﾀ長 
 *
 * return	受信ﾃﾞｰﾀ解析結果  
 * retval	0 : ｵｰﾌﾟﾆﾝｸﾞﾓｰﾄﾞ接続ﾚｽﾎﾟﾝｽOK
 * retval	1 : ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀ受信
 * retval	2 : ﾃｽﾄﾓｰﾄﾞ遷移要求受信
 * retval	6 : ｴﾝﾃﾞｨﾝｸﾞﾓｰﾄﾞﾃﾞｰﾀ受信
 * 
 * author	Y.Sorimachi
 * date	2014/4/11
 * note	<ul>
 *			<li>ｵｰﾌﾟﾆﾝｸﾞﾓｰﾄﾞﾃﾞｰﾀでDispNo.1～7の接続ﾘｸｴｽﾄをそれぞれ4回以上 受信した場合に接続ﾚｽﾎﾟﾝｽOKを戻り値にする。
 *			<li>ED受信時、ED受信を戻り値にする。
 *			<li>ﾘｱﾙﾃﾞｰﾀ受信時、ﾘｱﾙﾃﾞｰﾀ受信を戻り値にする。
 *			<li>Dﾃｽﾄﾓｰﾄﾞへの遷移要求のｺﾏﾝﾄﾞ受信時、ﾃｽﾄﾓｰﾄﾞへの遷移要求受信を戻り値にする。 
 *			</ul>
 */ 
static UI_8 defi_07701_rx_data_analyze(const UI_8 *data_p, const UI_8 data_len)		/* 引数追加			; ADD CHG-028(LX039) */
{
	UI_8	ret;														/* 戻り値 */

	ret = 0xFF;

	if (data_p[DF7_MODE_POS] == DF7_MODE_OPENING) {
		if (data_p[DF7_REQUEST_POS] == DF7_CONNECT_REQ) { 											/*	; ADD REQ-011(LX039) */

			S_df_set_state(DF_STATE_CONNECT);								/* DF接続 */
		}	
	} else {
		if ( S_judge_ending_mode(data_p) == N_TRUE) {
			ret = 6;													/* ｴﾝﾃﾞｨﾝｸﾞﾓｰﾄﾞﾃﾞｰﾀ受信を設定 */
		} else {														/*									; ADD REQ-011(LX039) */
			ret = 1;
		}																/* ﾘｱﾙﾃﾞｰﾀ受信						; ADD REQ-011(LX039) */
	}

	return ret;
}

/** 
 * エンディングモード判定
 * 
 * param	data_p	[in]	受信データ
 * 
 * return	判定結果
 * retval	N_TRUE	ｴﾝﾃﾞｨﾝｸﾞﾓｰﾄﾞ受信
 * retval	N_FALSE	ｴﾝﾃﾞｨﾝｸﾞﾓｰﾄﾞ未受信
 * 
 * author	Y.Sorimachi
 * date	2014/4/11
 * note	エンディングモードを受信したか判定する。
 */ 
static B_8 S_judge_ending_mode( const UI_8 *data_p )
{
	B_8 result = N_FALSE;
	/**	<ol>	*/
	/**	<li>エンディングモード判定	*/	
	if ((data_p[DF7_MODE_POS]   == DF7_MODE0_ENDING) &&
		((data_p[DF7_MODE_POS] & DF7_MODE1_MASK)  == DF7_MODE1_ENDING ))
	{
		result = N_TRUE;
	}
	/**	</ol>	*/	
	return result;
}

/** 
 * 状態設定処理
 * 
 * param	state	[in]	DF接続状態
 * 
 * author	Y.Sorimachi
 * date	2014/4/11
 * note	DF接続状態と通知イベント状態を更新する。
 */ 
static void S_df_set_state( const E_DF_STATE state )
{
	/**	<ol>	*/
	/**	<li>DF接続状態を設定する	*/	
	S_df_state = state;
	/**	</ol>	*/	
	return;
}

#endif /* ADVANCE_ENABLE */
