/*--------------------------------------------------------------------------*/
/*	BT���W���[������														*/
/*--------------------------------------------------------------------------*/
/* �T�v																		*/
/* 				BT���W���[������											*/
/*																			*/
/*	�t�@�C���쐬���̃e�[�}:													*/
/*				ER330 �����A�_�v�^											*/
/*																			*/
/* �쐬			2010/09/24	����											*/
/* 																			*/
/* �C��			2012/03/23	���{											*/
/*				REQ-015�FDUT���[�h�������쐬								*/
/*				REQ-016�F�F�؃��[�h�������쐬								*/
/*																			*/
/* ���l																		*/
/*				����														*/
/*																			*/
/*--------------------------------------------------------------------------*/
#ifndef	ER330_BT_MODULE_CONTROL_TASK_H
#define	ER330_BT_MODULE_CONTROL_TASK_H


/*--------------------------------------------------------------------------*/
/*	�O���}�N��																*/
/*--------------------------------------------------------------------------*/

/**
 *
 *	BT�ʐM�~�h�����
 *
 */
typedef enum e_bt_comm_mdl_state {
	BT_COMM_MDL_STATE_IDLE = 0,					/**< �A�C�h��						*/
	BT_COMM_MDL_STATE_STARTUP,					/**< BT���W���[���N��������			*/
	BT_COMM_MDL_STATE_SHUTDOWN,					/**< BT���W���[����~������			*/
	BT_COMM_MDL_STATE_COMM_MODE,				/**< �ʐM���[�h						*/
	BT_COMM_MDL_STATE_PAIRING_MODE,				/**< �y�A�����O���[�h				*/
	BT_COMM_MDL_STATE_ERROR,					/**< BT���W���[���ُ�				*/
	BT_COMM_MDL_STATE_TEST_MODE_DUT,			/**< �e�X�g���[�h�iDUT�j			*/
	BT_COMM_MDL_STATE_TEST_MODE_CONRX,			/**< �e�X�g���[�h�iContinuous RX�j	*/
	BT_COMM_MDL_STATE_TEST_MODE_TXRX,			/**< �e�X�g���[�h�iPacket TXRX�j	*/
	BT_COMM_MDL_STATE_PROC,						/**< ��L�ȊO						*/
	BT_COMM_MDL_STATE_PRE						/**< �i���������p�j					*/
} E_BT_COMM_MDL_STATE;


/**
 *
 *	��������
 *
 */
typedef enum e_bt_comm_mdl_result {
	BT_COMM_MDL_RESULT_OK = 0,				/**< ����I��	*/
	BT_COMM_MDL_RESULT_ERR_INVALID_ARG,		/**< �����s��	*/
	BT_COMM_MDL_RESULT_ERR_INVALID_STATE,	/**< ��ԕs��	*/
	BT_COMM_MDL_RESULT_ERR_BUSY,			/**< �r�V�[		*/
	BT_COMM_MDL_RESULT_ERR_FATAL			/**< �ُ�I��	*/
} E_BT_COMM_MDL_RESULT;


/**
 *
 *	�y�A�����O���
 *
 */
typedef enum e_bt_comm_mdl_paired_status {
	BT_COMM_MDL_PAIRED_STATUS_NONE = 0,	/**< �y�A�����O�f�o�C�X�Ȃ�	*/
	BT_COMM_MDL_PAIRED_STATUS_EXIST		/**< �y�A�����O�f�o�C�X����	*/
} E_BT_COMM_MDL_PAIRED_STATUS;

/**
 *
 *	Peer�ڑ����
 *
 */
typedef enum e_bt_comm_mdl_peer_status {
	BT_COMM_MDL_PEER_STATUS_DISCONNECTED = 0,	/**< ���ڑ�	*/
	BT_COMM_MDL_PEER_STATUS_CONNECTED			/**< �ڑ�	*/
} E_BT_COMM_MDL_PEER_STATUS;


/**
 *
 * �R�[���o�b�N�֐��i�f�[�^��M�ʒm�j
 *
 * @param	buf	[in]	��M�f�[�^
 * @param	len	[in]	��M�f�[�^��
 *
 */
typedef void ( *P_BT_COMM_MDL_FUNC_RECEIVE_DATA )( const UI_8 *buf, UI_16 len );

/*--------------------------------------------------------------------------*/
/*	�萔�錾																*/
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*	�\���̐錾																*/
/*--------------------------------------------------------------------------*/
/**
 *	iAP�ڑ����
 */
typedef struct {
	UI_32				validFlag;		/* �L��/�����t���O */
	UI_32				deviceVersion;	/* �f�o�C�X�o�[�W���� */
	UI_32				firmwareVer;	/* �t�@�[���E�F�A�o�[�W���� */
	UI_32				majorVer;		/* ���W���[�o�[�W���� */
	UI_32				minorVer;		/* �}�C�i�[�o�[�W���� */
	UI_32				deviceID;		/* �f�o�C�XID */
} S_IAP_CONNECTON_INFO;


/*--------------------------------------------------------------------------*/
/*	�v���g�^�C�v�錾														*/
/*--------------------------------------------------------------------------*/
/**
 *
 * ������
 *
 */
void Bt_comm_mdl_Init( void );

/**
 *
 * ���C������
 *
 */
void Bt_comm_mdl_Main( void );

/**
 *
 * �R�[���o�b�N�o�^�i�f�[�^��M�ʒm�j
 *
 * @param	cb	[in]	�R�[���o�b�N
 *
 */
void Bt_comm_mdl_RegisterCbReceiveData( const P_BT_COMM_MDL_FUNC_RECEIVE_DATA cb );

/**
 *
 * iAP�ڑ����擾
 *
 * @param	info	[out]	iAP�ڑ����
 *
 */
E_BT_COMM_MDL_RESULT Bt_comm_mdl_GetIapAuthInfo( const S_IAP_CONNECTON_INFO** info );

/**
 *
 * BT���W���[���N��
 *
 * @return	��������
 *
 */
E_BT_COMM_MDL_RESULT Bt_comm_mdl_Startup( void );

/**
 *
 * BT���W���[����~
 *
 * @return	��������
 *
 */
E_BT_COMM_MDL_RESULT Bt_comm_mdl_Shutdown( void );

/**
 *
 * �ʐM���[�h�J�n
 *
 * @return	��������
 *
 */
E_BT_COMM_MDL_RESULT Bt_comm_mdl_EnterCommMode( void );

/**
 *
 * �y�A�����O���[�h�J�n
 *
 * @return	��������
 *
 */
E_BT_COMM_MDL_RESULT Bt_comm_mdl_EnterPairingMode( void );


/**
 *
 * �f�[�^���M
 *
 * @param	buf	[in]	���M�f�[�^
 * @param	len	[in]	���M�f�[�^��
 *
 * @return	��������
 *
 */
E_BT_COMM_MDL_RESULT Bt_comm_mdl_SendData( const UI_8 *buf, UI_16 len );

/**
 *
 * �y�A�����O��Ԏ擾
 *
 * @return	�y�A�����O���
 *
 */
E_BT_COMM_MDL_PAIRED_STATUS Bt_comm_mdl_GetPairedStatus( void );

/**
 *
 * BT�ʐM�~�h����Ԏ擾
 *
 * @return	BT�ʐM�~�h�����
 *
 */
E_BT_COMM_MDL_STATE Bt_comm_mdl_GetState( void );

/**
 *
 * Peer�ڑ���Ԏ擾
 *
 * @return	Peer�ڑ����
 * 
 */
/* 2016/08/29 T.Kamakura(OFT) */
E_BT_COMM_MDL_PEER_STATUS Bt_comm_mdl_GetPeerState( void );

#endif	/* ER330_BT_MODULE_CONTROL_TASK_H */
