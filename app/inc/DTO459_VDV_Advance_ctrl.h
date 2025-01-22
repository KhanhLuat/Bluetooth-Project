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
/* AdvanceControlUnit制御								*/
/********************************************************/
#ifndef	DTO459_VDV_ADVANCE_CTRL_H
#define	DTO459_VDV_ADVANCE_CTRL_H

/********************************************************/
/*	インクルードファイル								*/
/********************************************************/
#include "type.h"

/********************************************************/
/*	マクロ定義											*/
/********************************************************/

typedef struct t_advance_ctrl_unit_info {
	UI_8	invalidFlag;			/* 無効フラグ（未受信時はTRUE) */
	UI_16	VehicleSpeed;			/* 車速 */
	UI_16	EngineRevolutionSpeed;	/* エンジン回転数 */
	UI_16	TurboPressure;			/* ターボ圧(過給圧) */
	UI_16	EngineOilPressure;		/* エンジン油圧 */
	UI_16	FuelPressure;			/* 燃料圧力 */
	UI_8	DifferentialPressure;	/* 差圧 */
	UI_16	EngineOiTemperture;		/* エンジン油温 */
	UI_16	EngineCoolantTemperture;/* エンジン水温 */
	UI_16	ExhaustGasTemperture;	/* 排気温度 */
	UI_16	Volts;					/* 電圧 */
} T_ADVANCE_CTRL_UNIT_INFO;


/********************************************************/
/*	内部変数定義										*/
/********************************************************/

/********************************************************/
/*	外部関数											*/
/********************************************************/

/********************************************************/
/* 関数名称		: Advance_control_Init					*/
/* 機能の説明	: AdvanceControlUnit制御初期化			*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Advance_control_Init(void);

/********************************************************/
/* 関数名称		: Advance_control_Main					*/
/* 機能の説明	: AdvanceControlUnit制御メイン			*/
/* 引数			: void									*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Advance_control_Main(void);

/********************************************************/
/* 関数名称		: Advance_control_GetData				*/
/* 機能の説明	: 車両情報取得							*/
/* 引数			: 車両情報構造体						*/
/* 				: T_ADVANCE_CTRL_UNIT_INFO				*/
/* 戻り値		: void									*/
/* 作成者		: (OFT)Sakata							*/
/* 作成日		: 16/08/18								*/
/* 備考			: 										*/
/********************************************************/
void Advance_control_GetData(T_ADVANCE_CTRL_UNIT_INFO **data);

/** 
 * ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀ設定処理
 * 
 * param	*data_p		[in]	ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀのﾎﾟｲﾝﾀ 
 * param	len			[in]	ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀ長  
 *
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		<ul>
 *			<li>ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀをstatic変数に設定する。 
 *			<li>DF07701ﾕﾆｯﾄ接続時のﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀは34byte 
 *			</ul>
 */ 
void Set_real_mode_data(const UI_8 *data_p, UI_8 len);

/** 
 * ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀ取得処理 
 * 
 * param	*data_p		[out]	ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀ格納ﾊﾞｯﾌｧのﾎﾟｲﾝﾀ 
 *
 * return	ﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀ長  
 * 
 * author	Y.Sorimachi
 * date		2014/4/11
 * note		<ul>
 *			<li>static変数に設定されたﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀを取得する。   
 *			<li>DF07701ﾕﾆｯﾄ接続時のﾘｱﾙﾓｰﾄﾞﾃﾞｰﾀは34byte 
 *			</ul>
 */ 
UI_8 Get_real_mode_data(UI_8 *data_p);

/********************************************************/
/* 関数名称		: Advance_control_ReqInspection			*/
/* 機能の説明	: 検査要求								*/
/* 引数			: void									*/
/* 戻り値		: N_OK		正常						*/
/* 				: N_NG		異常						*/
/* 作成者		: (OFT)Toguchi							*/
/* 作成日		: 17/01/20								*/
/* 備考			: 本関数が正常終了した場合、検査制御モジュールに対して		*/
/*				   Insp_notify_inspection_resultにより検査結果を通知する	*/
/********************************************************/
SI_8 Advance_control_ReqInspection(void);

/********************************************************/
/* 関数名称		:	Advance_control_getUartData			*/
/* 機能の説明	:	UARTデータ取得						*/
/* 引数			:	( /O)buf	データバッファ			*/
/*				:	(I/ )len	取得データ長(byte)		*/
/* 戻り値		:	取得できたデータ長(byte)			*/
/* 作成者		:	(OFT)Toguchi						*/
/* 作成日		:	17/01/20							*/
/* 備考			:										*/
/********************************************************/
UI_16 Advance_control_getUartData(UI_8 *buf, UI_16 len);

/********************************************************/
/* 関数名称		:	Advance_control_getUartDataLen		*/
/* 機能の説明	:	UARTデータ長取得					*/
/* 引数			:	void								*/
/* 戻り値		:	UARTデータ長(byte)					*/
/* 作成者		:	(OFT)Toguchi						*/
/* 作成日		:	17/01/20							*/
/* 備考			:										*/
/********************************************************/
UI_16 Advance_control_getUartDataLen(void);

#endif	/*DTO459_VDV_ADVANCE_CTRL_H*/
