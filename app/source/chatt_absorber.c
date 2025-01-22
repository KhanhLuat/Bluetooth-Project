/********************************************************************************/
/* file chatt_absorber.c														*/
/* チャタリング吸収																*/
/* 																				*/
/* <ul>																			*/
/* <li>入力ポートから取得したデータに対しチャタリング吸収する。					*/
/* <li>チャタリング吸収したデータは、設定された関数にコールバックする。			*/
/* <li>チャタリング吸収は、20ms周期に3回連続で同じ値のものとする。				*/
/* </ul>																		*/
/* 																				*/
/********************************************************************************/
/* author      Y.Sorimachi														*/
/* date        2013/--/--														*/
/********************************************************************************/
/* page */

#include "system.h"
#include "SHRVSTD_Port_Drv.h"
#include "chatt_absorber.h"

/********************************************************************************/
/*	内部マクロ																	*/
/********************************************************************************/
#define CHATT_LOCAL	static

#define CA_COUNT					(4)		/**< チャタリング吸収カウント		*/
#define CA_COUNT_MAX				(0xFF)	/**< チャタリング吸収カウント最大値	*/

#define CA_CHECKING					(0xFF)	/**< チャタリング吸収中 		*/
#define CA_CHECK_LOW				(0x00)	/**< チャタリング吸収 LOW		*/
#define CA_CHECK_HIGH				(0x0F)	/**< チャタリング吸収 HIGH		*/
#define CA_CHECK_ACTIVE_ACC			(IO_PI_LEV_ACTIVE_ACC_STAT | (IO_PI_LEV_ACTIVE_ACC_STAT << 1) | (IO_PI_LEV_ACTIVE_ACC_STAT << 2) | (IO_PI_LEV_ACTIVE_ACC_STAT << 3))
#define CA_CHECK_NONACTIVE_ACC		(IO_PI_LEV_NONACT_ACC_STAT | (IO_PI_LEV_NONACT_ACC_STAT << 1) | (IO_PI_LEV_NONACT_ACC_STAT << 2) | (IO_PI_LEV_NONACT_ACC_STAT << 3))

#define CA_CYCLE					(2)		/**< チャタリング吸収 周期		*/

#define CA_MASK_BIT_0EH				(0x0E)	/**< マスクビット（0x0E）		*/
#define CA_MASK_BIT_01H				(0x01)	/**< マスクビット（0x01）		*/

#define CA_SHIFT_1_BIT				(0x01)	/**< 1bitシフト					*/
#define CA_ACC_UNKNOWN				(0xFF)	/**< ACCポート不明				*/

#define CA_SDC_UNKNOWN				(0xFF)	/**< SDカード検出ポート不明		*/

/********************************************************************************/
/*	内部関数プロトタイプ宣言													*/
/********************************************************************************/
/*
 *	チャタリング吸収関数
 */
CHATT_LOCAL void S_ca_acc( void );				/**< ACC	*/
CHATT_LOCAL void S_ca_sdc( void );				/**< SDカード	*/


/********************************************************************************/
/*	定数宣言																	*/
/********************************************************************************/
/*
 *	チャタリング吸収関数テーブル
 */
CHATT_LOCAL void ( * const C_ca_func[CHATT_PORT_NUM] ) ( void ) = {
	S_ca_acc,								/**< ACC		*/
	S_ca_sdc,								/**< SDカード	*/
};

/********************************************************************************/
/*	構造体宣言																	*/
/********************************************************************************/
/**
 *	チャタリング吸収用構造体
 */
typedef struct t_chatt_abs {
	UI_8	data;							/**< データ		*/
	UI_8	count;							/**< カウンタ	*/
} T_CHATT_ABS;

/********************************************************************************/
/*	内部変数定義																*/
/********************************************************************************/
CHATT_LOCAL T_CHATT_ABS			S_chatt_abs[CHATT_PORT_NUM];			/**< チャタリング吸収用データ	*/
CHATT_LOCAL P_CHATT_ABS_CB_FUNC	S_ca_acc_cb[CHATT_ABS_MOD_NUM];			/**< チャタリング吸収（ACC）コールバック関数	*/
CHATT_LOCAL P_CHATT_ABS_CB_FUNC	S_ca_sdc_cb[CHATT_ABS_MOD_NUM];			/**< チャタリング吸収（SDカード検出）コールバック関数	*/

CHATT_LOCAL UI_8			 		S_acc_stat;								/**< ACCポート状態	*/
CHATT_LOCAL UI_8			 		S_sdc_stat;								/**< SDカード検出ポート状態	*/

/********************************************************************************/
/*	外部関数																	*/
/********************************************************************************/

/**
 * チャタリング吸収 初期化
 * 
 * author	Y.Sorimachi
 * date	2013/--/--
 * note	<ul>
 *			<li>チャタリング吸収で使用されるRAMを初期化する。
 *			</ul>
 */
void Chatt_absorber_Init( void )
{
	UI_8 i;
	/**	<ol>	*/
	/**	<li>初期化	*/
	for (i = 0; i < CHATT_PORT_NUM; i++)
	{
		S_chatt_abs[i].data 		= 0;
		S_chatt_abs[i].count 		= 0;
	}
	for (i = 0; i < CHATT_ABS_MOD_NUM; i++)
	{
		S_ca_acc_cb[i]				= N_NULL;
		S_ca_sdc_cb[i]				= N_NULL;
	}
	/**	<li>初回はチャタリング吸収を行うため、N_ONにする	*/
	S_acc_stat						= CA_ACC_UNKNOWN;
	S_sdc_stat						= CA_SDC_UNKNOWN;
	/**	</ol>	*/	
	return;
}

/**
 * チャタリング吸収 メイン処理
 * 
 * author	Y.Sorimachi
 * date	2013/--/--
 * note	チャタリング吸収のメイン処理。
 */
void Chatt_absorber_Main( void )
{
	UI_8 i;
	/**	<ol>	*/
	/**	<li>20ms毎に実行	*/
	/**	<li>チャタリング吸収実行	*/
	for (i = 0; i < CHATT_PORT_NUM; i++)
	{
		C_ca_func[i]();
	}
	/**	</ol>	*/	
	return;
}

/**
 * チャタリング吸収 ポートデータ要求
 * 
 * param	ca_data_cb	[in]	チャタリング吸収後のデータコールバック関数
 * param	port		[in]	指定するポート（E_CHATT_ABS_PORT）
 * param	module		[in]	コールバック関数を登録するモジュール（E_CHATT_ABS_MODULE）
 *
 * return	実行結果（E_CHATT_ABS_RESULT）
 * 
 * author	Y.Sorimachi
 * date	2013/--/--
 * note	<ul>
 *			<li>指定したポートのチャタリング吸収後のデータを要求する。
 *			<li>指定したポートにエッジ変化があったとき、チャタリング吸収結果データをコールバック関数で渡す。
 *			</ul>
 */
UI_8 Chatt_absorber_ReqPortData( P_CHATT_ABS_CB_FUNC ca_data_cb, UI_8 port, UI_8 module )
{
	/**	<ol>	*/
	/**	<li>引数チェック	*/
	if ( (ca_data_cb == N_NULL) || (port >= CHATT_PORT_NUM) || (module >= CHATT_ABS_MOD_NUM) )
	{
		return CHATT_ABS_ERR_INVALID_ARG;
	}
	
	/**	<li>コールバック関数登録	*/
	switch (port)
	{
	case (CHATT_PORT_ACC):
		S_ca_acc_cb[module] = ca_data_cb;
		break;
	case (CHATT_PORT_SDC):
		S_ca_sdc_cb[module] = ca_data_cb;
		break;
	default:
		break;
	}
	/**	</ol>	*/	
	return CHATT_ABS_OK;
}

/********************************************************************************/
/*	内部関数																	*/
/********************************************************************************/
/**
 * チャタリング吸収関数:ACC
 * 
 * author	Y.Sorimachi
 * date	2013/--/--
 * note	<ul>
 *			<li>電源OFF準備とは、以下の処理を行う。
 *			</ul>
 */
CHATT_LOCAL void S_ca_acc( void )
{
	UI_8 i;
	UI_8 port_data;
	UI_8 ca_data;
	
	/**	<ol>	*/	
	/**	<li>入力ポート（ACC）のエッジ変化があったとき、チャタリング吸収する	*/
	/**	<li>ポート（ACC）データ取得	*/
	(void)PortDrv_GetDataBit(IO_PI_GR_ACC_STAT, IO_PI_BIT_ACC_STAT, &port_data);
	
	/**	<li>チャタリング吸収のため、データ保存（４回分）	*/
	S_chatt_abs[CHATT_PORT_ACC].data = (S_chatt_abs[CHATT_PORT_ACC].data << CA_SHIFT_1_BIT) & CA_MASK_BIT_0EH;
	S_chatt_abs[CHATT_PORT_ACC].data |= (port_data & CA_MASK_BIT_01H);				
	if (S_chatt_abs[CHATT_PORT_ACC].count < CA_COUNT)
	{
		/**	<li>保存データが3回分以上たまっていない場合はカウント加算	*/
		S_chatt_abs[CHATT_PORT_ACC].count++;
	}

	/**	<li>4回分データ保存できたか判定	*/
	ca_data = CA_CHECKING;
	if (S_chatt_abs[CHATT_PORT_ACC].count >= CA_COUNT)
	{
		/**	<li>チャタリング吸収した結果、Active判定	*/
		if (S_chatt_abs[CHATT_PORT_ACC].data == CA_CHECK_ACTIVE_ACC)
		{
			ca_data = IO_PI_LEV_ACTIVE_ACC_STAT;
		}
		/**	<li>チャタリング吸収した結果、NonActive判定	*/
		else if (S_chatt_abs[CHATT_PORT_ACC].data == CA_CHECK_NONACTIVE_ACC)
		{
			ca_data = IO_PI_LEV_NONACT_ACC_STAT;
		}
		else
		{
			/* nop */
		}
	}
	
	/**	<li>チャタリング吸収が完了かつACC ON/OFF変化検出していれば、コールバック関数でACCを渡す	*/
	if ((ca_data != CA_CHECKING) && (S_acc_stat != ca_data))
	{
		/**	<li>ACCポート状態更新	*/
		S_acc_stat = ca_data;
		/**	<li>コールバック関数呼び出し	*/
		for (i = 0; i < CHATT_ABS_MOD_NUM; i++)
		{
			if (S_ca_acc_cb[i] != N_NULL)
			{
				S_ca_acc_cb[i](ca_data);
			}
		}
		/**	<li>チャタリング吸収（ACC）の変数リセット	*/
		S_chatt_abs[CHATT_PORT_ACC].count	= 0;
	}
	/**	</ol>	*/
	return;
}

/**
 * チャタリング吸収関数:SDカード検出
 * 
* author	(OFT)Toguchi
 * date	2016/09/23
 * note	<ul>
 *			<li>SDカードのIN/OUTを検出する。
 *			</ul>
 */
CHATT_LOCAL void S_ca_sdc( void )
{
	UI_8 i;
	UI_8 port_data;

	/**	<li>ポート（SDカード検出）データ取得	*/
	(void)PortDrv_GetDataBit(IO_PI_GR_SD_DTCT, IO_PI_BIT_SD_DTCT, &port_data);

	/**	<li>SDカードIN/OUT変化検出していれば、コールバック関数で検出結果を渡す	*/
	if (S_sdc_stat != port_data)
	{
		/**	<li>SDカード検出ポート状態更新	*/
		S_sdc_stat = port_data;
		/**	<li>コールバック関数呼び出し	*/
		for (i = 0; i < CHATT_ABS_MOD_NUM; i++)
		{
			if (S_ca_sdc_cb[i] != N_NULL)
			{
				S_ca_sdc_cb[i](port_data);
			}
		}
	}

	/**	</ol>	*/
	return;
}
