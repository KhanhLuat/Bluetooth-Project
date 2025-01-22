/*--------------------------------------------------------------------------*/
/*	BTモジュール制御														*/
/*--------------------------------------------------------------------------*/
/* 概要																		*/
/* 				BTモジュール制御											*/
/*																			*/
/*	ファイル作成時のテーマ:													*/
/*				ER330 無線アダプタ											*/
/*																			*/
/* 作成			2010/09/24	小林											*/
/* 																			*/
/* 修正			2012/03/23	松本											*/
/*				REQ-015：DUTモード時処理作成								*/
/*				REQ-016：認証モード時処理作成								*/
/*																			*/
/* 備考																		*/
/*				無し														*/
/*																			*/
/*--------------------------------------------------------------------------*/
#ifndef	ER330_BT_MODULE_CONTROL_TASK_H
#define	ER330_BT_MODULE_CONTROL_TASK_H


/*--------------------------------------------------------------------------*/
/*	外部マクロ																*/
/*--------------------------------------------------------------------------*/

/**
 *
 *	BT通信ミドル状態
 *
 */
typedef enum e_bt_comm_mdl_state {
	BT_COMM_MDL_STATE_IDLE = 0,					/**< アイドル						*/
	BT_COMM_MDL_STATE_STARTUP,					/**< BTモジュール起動処理中			*/
	BT_COMM_MDL_STATE_SHUTDOWN,					/**< BTモジュール停止処理中			*/
	BT_COMM_MDL_STATE_COMM_MODE,				/**< 通信モード						*/
	BT_COMM_MDL_STATE_PAIRING_MODE,				/**< ペアリングモード				*/
	BT_COMM_MDL_STATE_ERROR,					/**< BTモジュール異常				*/
	BT_COMM_MDL_STATE_TEST_MODE_DUT,			/**< テストモード（DUT）			*/
	BT_COMM_MDL_STATE_TEST_MODE_CONRX,			/**< テストモード（Continuous RX）	*/
	BT_COMM_MDL_STATE_TEST_MODE_TXRX,			/**< テストモード（Packet TXRX）	*/
	BT_COMM_MDL_STATE_PROC,						/**< 上記以外						*/
	BT_COMM_MDL_STATE_PRE						/**< （内部処理用）					*/
} E_BT_COMM_MDL_STATE;


/**
 *
 *	処理結果
 *
 */
typedef enum e_bt_comm_mdl_result {
	BT_COMM_MDL_RESULT_OK = 0,				/**< 正常終了	*/
	BT_COMM_MDL_RESULT_ERR_INVALID_ARG,		/**< 引数不正	*/
	BT_COMM_MDL_RESULT_ERR_INVALID_STATE,	/**< 状態不正	*/
	BT_COMM_MDL_RESULT_ERR_BUSY,			/**< ビシー		*/
	BT_COMM_MDL_RESULT_ERR_FATAL			/**< 異常終了	*/
} E_BT_COMM_MDL_RESULT;


/**
 *
 *	ペアリング状態
 *
 */
typedef enum e_bt_comm_mdl_paired_status {
	BT_COMM_MDL_PAIRED_STATUS_NONE = 0,	/**< ペアリングデバイスなし	*/
	BT_COMM_MDL_PAIRED_STATUS_EXIST		/**< ペアリングデバイスあり	*/
} E_BT_COMM_MDL_PAIRED_STATUS;

/**
 *
 *	Peer接続状態
 *
 */
typedef enum e_bt_comm_mdl_peer_status {
	BT_COMM_MDL_PEER_STATUS_DISCONNECTED = 0,	/**< 未接続	*/
	BT_COMM_MDL_PEER_STATUS_CONNECTED			/**< 接続	*/
} E_BT_COMM_MDL_PEER_STATUS;


/**
 *
 * コールバック関数（データ受信通知）
 *
 * @param	buf	[in]	受信データ
 * @param	len	[in]	受信データ長
 *
 */
typedef void ( *P_BT_COMM_MDL_FUNC_RECEIVE_DATA )( const UI_8 *buf, UI_16 len );

/*--------------------------------------------------------------------------*/
/*	定数宣言																*/
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*	構造体宣言																*/
/*--------------------------------------------------------------------------*/
/**
 *	iAP接続状態
 */
typedef struct {
	UI_32				validFlag;		/* 有効/無効フラグ */
	UI_32				deviceVersion;	/* デバイスバージョン */
	UI_32				firmwareVer;	/* ファームウェアバージョン */
	UI_32				majorVer;		/* メジャーバージョン */
	UI_32				minorVer;		/* マイナーバージョン */
	UI_32				deviceID;		/* デバイスID */
} S_IAP_CONNECTON_INFO;


/*--------------------------------------------------------------------------*/
/*	プロトタイプ宣言														*/
/*--------------------------------------------------------------------------*/
/**
 *
 * 初期化
 *
 */
void Bt_comm_mdl_Init( void );

/**
 *
 * メイン処理
 *
 */
void Bt_comm_mdl_Main( void );

/**
 *
 * コールバック登録（データ受信通知）
 *
 * @param	cb	[in]	コールバック
 *
 */
void Bt_comm_mdl_RegisterCbReceiveData( const P_BT_COMM_MDL_FUNC_RECEIVE_DATA cb );

/**
 *
 * iAP接続情報取得
 *
 * @param	info	[out]	iAP接続情報
 *
 */
E_BT_COMM_MDL_RESULT Bt_comm_mdl_GetIapAuthInfo( const S_IAP_CONNECTON_INFO** info );

/**
 *
 * BTモジュール起動
 *
 * @return	処理結果
 *
 */
E_BT_COMM_MDL_RESULT Bt_comm_mdl_Startup( void );

/**
 *
 * BTモジュール停止
 *
 * @return	処理結果
 *
 */
E_BT_COMM_MDL_RESULT Bt_comm_mdl_Shutdown( void );

/**
 *
 * 通信モード開始
 *
 * @return	処理結果
 *
 */
E_BT_COMM_MDL_RESULT Bt_comm_mdl_EnterCommMode( void );

/**
 *
 * ペアリングモード開始
 *
 * @return	処理結果
 *
 */
E_BT_COMM_MDL_RESULT Bt_comm_mdl_EnterPairingMode( void );


/**
 *
 * データ送信
 *
 * @param	buf	[in]	送信データ
 * @param	len	[in]	送信データ長
 *
 * @return	処理結果
 *
 */
E_BT_COMM_MDL_RESULT Bt_comm_mdl_SendData( const UI_8 *buf, UI_16 len );

/**
 *
 * ペアリング状態取得
 *
 * @return	ペアリング状態
 *
 */
E_BT_COMM_MDL_PAIRED_STATUS Bt_comm_mdl_GetPairedStatus( void );

/**
 *
 * BT通信ミドル状態取得
 *
 * @return	BT通信ミドル状態
 *
 */
E_BT_COMM_MDL_STATE Bt_comm_mdl_GetState( void );

/**
 *
 * Peer接続状態取得
 *
 * @return	Peer接続状態
 * 
 */
/* 2016/08/29 T.Kamakura(OFT) */
E_BT_COMM_MDL_PEER_STATUS Bt_comm_mdl_GetPeerState( void );

#endif	/* ER330_BT_MODULE_CONTROL_TASK_H */
