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
/* file DTO459_VDV_Sw_ctrl.c							*/
/* SW����												*/
/********************************************************/

/********************************************************/
/*	�C���N���[�h�t�@�C��								*/
/********************************************************/
#include "string.h"
#include "system.h"
#include "Timer.h"	
#include "timer_ctrl.h"
#include "System_control_TASK.h"							/* ���я�Ԑ��� */
#include "DTO459_VDV_Sw_ctrl.h"
#include "SHRVSTD_ADC_Drv.h"
#include "System_control_TASK.h"
#include "SSFTSTD_Macro.h"
#include "DTO459_VDV_Inspection_ctrl.h"

#define SW_LOCAL	static


/********************************************************/
/*	�}�N����`											*/
/********************************************************/
#define CA_COUNT		(5)					/**< �`���^�����O�z���J�E���g		*/
#define ADC_VALUE_MAX	(0x3FF)				/**< AD�ϊ��ő�l					*/
#define SW_REC_LONG_PRESS_TIME	(3000)		/**< ���������o����(ms)				*/


/* 
 ** SW���[�h����p�萔
 */ 
typedef enum e_sw_mode_judge_status
{
	SW_MODE_JUDGE_UNKNOWN = 0,				/**< SW���[�h:�s��(�����l)		*/
	SW_MODE_JUDGE_OFF,						/**< SW���[�h:OFF				*/
	SW_MODE_JUDGE_AUTO,						/**< SW���[�h:�I�[�gREC			*/
	SW_MODE_JUDGE_MANUAL,					/**< SW���[�h:�}�j���A��REC		*/
	SW_MODE_JUDGE_UNCONNECT,				/**< SW���[�h:���ڑ�			*/
	SW_MODE_JUDGE_RECBTN,					/**< SW���[�h:REC�{�^������		*/
	SW_MODE_JUDGE_INVALID,					/**< SW���[�h:����				*/
	SW_MODE_JUDGE_NUM						/**< SW���[�h��					*/
} E_SW_MODE_JUDGE_STATUS;


/*SW���[�h����p�\����*/
typedef struct t_sw_mode_judge_tbl 
{
	E_SW_MODE_JUDGE_STATUS	mode;				/* ���[�h������ */
	UI_16					minVale;			/* �ŏ��l */
	UI_16					maxVale;			/* �ő�l */
} T_SW_MODE_JUDGE_TBL;


#define MODE_JUDGE_TBLDATA_NUM		(5)						/**< SW���[�h����p�e�[�u���f�[�^��		*/
#define SW_NOT_NOTIFY_EVENT			(SYS_EVENT_NUM)			/**< SW�ʒm�C�x���g�Ȃ�					*/
#define SW_NOT_NOTIFY_EVENT_PARAM	(SYS_EVENT_PARAM_NUM)	/**< SW�ʒm�C�x���g�p�����^�Ȃ�			*/
#define SW_REC_BTN_CHANGED			(SW_MODE_NUM)			/**< REC�{�^���ω�						*/

#define SW_INSPECTION_CHECK_CYCLE	(200)		/**< SW���������i200ms�j			*/

/* SW�����X�e�b�v */
typedef enum e_sw_inspection_step {
	SW_INSPECTION_STEP_1 = 0,					/* ����1						*/
	SW_INSPECTION_STEP_2,						/* ����2						*/
	SW_INSPECTION_STEP_3,						/* ����3						*/
	SW_INSPECTION_STEP_4,						/* ����4						*/
	SW_INSPECTION_STEP_5,						/* ����5						*/
	SW_INSPECTION_STEP_NUM,						/* ������						*/
	SW_INSPECTION_STEP_INVALID					/* �����l						*/
} E_SW_INSPECTION_STEP;

/* SW������� */
typedef struct t_sw_inspection_info {
	UI_8					req_flg;					/* �����v���t���O				*/
	E_SW_INSPECTION_STEP	step;						/* �����X�e�b�v					*/
	UI_32					start_time;					/* �����J�n����					*/
} T_SW_INSPECTION_INFO;

/* SW�����ݒ�l */
typedef struct t_sw_inspection_config {
	E_SW_INSPECTION_STEP	step;						/* �����X�e�b�v					*/
	UI_32					checktime;					/* �������ԁi�J�n����̎��ԁj	*/
	UI_16					adMinVale;					/* AD�l �ŏ��l					*/
	UI_16					adMaxVale;					/* AD�l �ő�l					*/
} T_SW_INSPECTION_CONFIG;

/* SW�����ݒ�e�[�u�� */
SW_LOCAL const T_SW_INSPECTION_CONFIG	C_sw_inspection_config_tbl[SW_INSPECTION_STEP_NUM] = {
	{	SW_INSPECTION_STEP_1,	(UI_32)(SW_INSPECTION_CHECK_CYCLE * 1),	983	,	1023	},	/* ����1	4.8-5.0[v]			*/
	{	SW_INSPECTION_STEP_2,	(UI_32)(SW_INSPECTION_CHECK_CYCLE * 2),	594	,	 634	},	/* ����2	2.9-3.1[v]			*/
	{	SW_INSPECTION_STEP_3,	(UI_32)(SW_INSPECTION_CHECK_CYCLE * 3),	389	,	 429	},	/* ����3	1.9-2.1[v]			*/
	{	SW_INSPECTION_STEP_4,	(UI_32)(SW_INSPECTION_CHECK_CYCLE * 4),	185	,	 225	},	/* ����4	0.9-1.1[v]			*/
	{	SW_INSPECTION_STEP_5,	(UI_32)(SW_INSPECTION_CHECK_CYCLE * 5),	  0	,	  40	},	/* ����5	0.0-0.2[v]			*/
};

/*SW���[�h����p�e�[�u��*/
SW_LOCAL const T_SW_MODE_JUDGE_TBL	C_sw_mode_judge_tbl[MODE_JUDGE_TBLDATA_NUM] = {	/* SW���[�h����e�[�u�� */
	{	SW_MODE_JUDGE_RECBTN	,	  0	,	  40	},	/* SW���[�h:REC�{�^������ */
	{	SW_MODE_JUDGE_OFF		,	368	,	 450	},	/* SW���[�h:OFF */
	{	SW_MODE_JUDGE_MANUAL	,	573	,	 655	},	/* SW���[�h:�}�j���A��REC */
	{	SW_MODE_JUDGE_AUTO		,	778	,	 860	},	/* SW���[�h:�I�[�gREC */
	{	SW_MODE_JUDGE_UNCONNECT	,	982	,	1023	},	/* SW���[�h:���ڑ� */
};
/* AD�ϊ��l��10bit(0-1023)�͈̔͂Ŏ擾����� */
/* ������O�|�TV�Ɋ����Ă� */
/* 0.1v �� 1024 / 50 = 20.48                   -0.2v     �v   +0.2v */
/* SW���[�h:REC�{�^������		0.0V �{ 0.2v ( XXX		   0	  40.96 ) */
/* SW���[�h:����`				1.0V �} 0.2v ( 163.84	 204.8	 245.76 ) */
/* SW���[�h:�}�j���A��REC		2.0V �} 0.2v ( 368.64	 409.6	 450.56 ) */
/* SW���[�h:�I�[�gREC			3.0V �} 0.2v ( 573.44	 614.4	 655.36 ) */
/* SW���[�h:OFF					4.0V �} 0.2v ( 778.24	 819.2	 860.16 ) */
/* SW���[�h:���ڑ�				5.0V �| 0.2v ( 982.04	1023.0	 XXX	) */
		

/**
 *	�ʒm�C�x���g
 */
typedef struct t_sw_mode_notify_event {
	E_SYS_CTRL_EVENT			event;			/* �C�x���g */
	E_SYS_CTRL_EVENT_PARAM		param;			/* �p�����^ */
} T_SW_MODE_NOTIFY_EVENT;

/*�C�x���g�ʒm�p�e�[�u�� */
SW_LOCAL const T_SW_MODE_NOTIFY_EVENT	C_sw_mode_notify_event_tbl[SW_MODE_JUDGE_NUM] = {
	{	SW_NOT_NOTIFY_EVENT			,	SW_NOT_NOTIFY_EVENT_PARAM			},	/* SW_MODE_UNKNOWN, */
	{	SYS_EVENT_CHANGE_SW_MODE	,	SYS_EVENT_PARAM_SW_MODE_OFF			},	/* SW_MODE_OFF, */
	{	SYS_EVENT_CHANGE_SW_MODE	,	SYS_EVENT_PARAM_SW_MODE_AUTO		},	/* SW_MODE_AUTO, */
	{	SYS_EVENT_CHANGE_SW_MODE	,	SYS_EVENT_PARAM_SW_MODE_MANUAL		},	/* SW_MODE_MANUAL, */
	{	SYS_EVENT_MODULE_STAT		,	SYS_EVENT_PARAM_MODSTAT_ATT_NG_SW	},	/* SW_MODE_UNCONNECT, */
	{	SW_NOT_NOTIFY_EVENT			,	SW_NOT_NOTIFY_EVENT_PARAM			},	/* SW_MODE_JUDGE_RECBTN, */
	{	SW_NOT_NOTIFY_EVENT			,	SW_NOT_NOTIFY_EVENT_PARAM			},	/* SW_MODE_UNKNOWN, */
};

/* SW���[�h����p���O�����J�p���[�h�ɕϊ����e�[�u�� */
SW_LOCAL const E_SW_MODE_STATUS C_sw_chg_mode_tbl[SW_MODE_JUDGE_NUM] = 
{
	SW_MODE_UNKNOWN,						/* SW_MODE_JUDGE_UNKNOWN, */
	SW_MODE_OFF,	  						/* SW_MODE_JUDGE_OFF, */
	SW_MODE_AUTO,							/* SW_MODE_JUDGE_AUTO, */
	SW_MODE_MANUAL, 						/* SW_MODE_JUDGE_MANUAL, */
	SW_MODE_UNCONNECT,						/* SW_MODE_JUDGE_UNCONNECT, */
	SW_REC_BTN_CHANGED,						/* SW_MODE_JUDGE_RECBTN, */
	SW_MODE_INVALID							/* SW_MODE_JUDGE_INVALID, */
};

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/
SW_LOCAL B_8						S_firstevent_flag;						/**< �����C�x���g�ʒm�t���O	*/
SW_LOCAL UI_8						S_chatt_count;							/**< �`���^�����O�J�E���^*/
SW_LOCAL E_SW_MODE_JUDGE_STATUS		S_swmode_hist_value[CA_COUNT];			/**< AD�ϊ��l�̗���		*/
SW_LOCAL E_SW_REC_BTN_STATUS		S_sw_rec_btn_status;					/**< REC�{�^�����		*/
SW_LOCAL E_SW_MODE_STATUS			S_sw_mode_status;						/**< SW���[�h���		*/
SW_LOCAL UI_16						S_sw_btn_on_time;						/**< REC�{�^����������	*/
SW_LOCAL E_SYS_CTRL_STATUS			S_pre_sys_stat;							/**< �O��V�X�e�����	*/
SW_LOCAL UI_8						S_sw_btn_onoff_cnt;						/**< �{�^��ONOFF�ω���*/
SW_LOCAL T_SW_INSPECTION_INFO		S_sw_inspection_info;					/**< SW�������			*/


/********************************************************/
/*	�O���֐�											*/
/********************************************************/
SW_LOCAL B_8 sw_control_judge_mode( void );
SW_LOCAL void sw_control_setSwitchMode( E_SW_MODE_STATUS swmodestatus);
SW_LOCAL void sw_control_notifyEvent( E_SW_MODE_STATUS swmodestatus );
SW_LOCAL E_SW_MODE_JUDGE_STATUS sw_control_cnvSwMode( UI_16 value );
#ifdef _VDV_APP
SW_LOCAL void sw_ctrl_initInspectionInfo(void);
SW_LOCAL void sw_control_inspection(void);
#endif	/* _VDV_APP */


/********************************************************/
/* �֐�����		: Sw_control_Init						*/
/* �@�\�̐���	: Sw���䏉����							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Sw_control_Init(void)
{
	UI_8 i;
	S_firstevent_flag = N_TRUE;
	S_chatt_count = 0;
	S_sw_btn_on_time = 0;
	S_sw_btn_onoff_cnt = 0;
	S_sw_rec_btn_status = SYS_REC_BTN_UNKNOWN;
	S_sw_mode_status	= SW_MODE_UNKNOWN;
	S_pre_sys_stat		= SYS_OPERATION_NUM;
	for(i = 0 ; i < CA_COUNT ; i++)
	{
		S_swmode_hist_value[i] = SW_MODE_JUDGE_UNKNOWN;
	}

#ifdef _VDV_APP
	sw_ctrl_initInspectionInfo();
#endif	/* _VDV_APP */
}

/********************************************************/
/* �֐�����		: Sw_control_Main						*/
/* �@�\�̐���	: SW���䃁�C��							*/
/* ����			: void									*/
/* �߂�l		: void									*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
void Sw_control_Main(void)
{
	B_8 bchange;
	E_SW_MODE_STATUS	swmodestatus;
	E_SYS_CTRL_STATUS	sys_stat;
	
	/**	�V�X�e����Ԏ擾	*/
	sys_stat =  System_control_GetStatus();
	switch ( sys_stat )
	{
		case (SYS_OPERATION_ACC_ON_UNKNOWN_STAT):	/*ACC ON�i��Ԕ���j	*/
		case (SYS_OPERATION_NORMAL_OFF):			/*�ʏ�:OFF���[�h		*/
		case (SYS_OPERATION_NORMAL_AUTO):			/*�ʏ�:�I�[�gREC		*/
		case (SYS_OPERATION_NORMAL_MANUAL):			/*�ʏ�:�}�j���A��REC	*/
		case (SYS_OPERATION_SIMULATION):			/*�V�~�����[�V����		*/
		case (SYS_OPERATION_PRE_ACC_OFF):			/*ACC OFF�ڍs			*/
		case (SYS_OPERATION_INSPECTION):			/*�������[�h			*/
		case (SYS_OPERATION_GNSS_COMM):				/*GNSS���W���[���ʐM���[�h*/
			/* �����p�� */
		break;
		default:
			/* �X�C�b�`���肷��K�v�Ȃ��̂Ŕ����� */
			return;
	}
	if( (S_pre_sys_stat == SYS_OPERATION_PRE_ACC_OFF) && (sys_stat == SYS_OPERATION_ACC_ON_UNKNOWN_STAT) )
	{
		/* ACC OFF -> ACC ON�Ȃ���������� */
		Sw_control_Init();
	}
	S_pre_sys_stat = sys_stat;
	
	if (sys_stat == SYS_OPERATION_INSPECTION) {
#ifdef _VDV_APP
		sw_control_inspection();
#endif /* _VDV_APP */
	}
	else {
		/**	�X�C�b�`���[�h�m�肵�����`�F�b�N����	*/
		bchange = sw_control_judge_mode();
		if(bchange == N_TRUE)
		{
			/**	�m�肵��	*/
			/* �O�����J�p���[�h�ɕϊ� */
			swmodestatus = C_sw_chg_mode_tbl[S_swmode_hist_value[0]];
			/**	�X�C�b�`���[�h�ݒ�	*/
			sw_control_setSwitchMode( swmodestatus );
		}
	}
}


/********************************************************/
/* �֐�����		: Sw_control_GetSwMode					*/
/* �@�\�̐���	: SW���[�h��Ԏ擾						*/
/* ����			: void									*/
/* �߂�l		: SW���[�h��� E_SW_MODE_STATUS			*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
E_SW_MODE_STATUS Sw_control_GetSwMode(void)
{
	return S_sw_mode_status;
}


/********************************************************/
/* �֐�����		: Sw_control_GetRecButtonStatus			*/
/* �@�\�̐���	: REC�{�^����Ԏ擾						*/
/* ����			: void									*/
/* �߂�l		: �{�^����� E_SW_REC_BTN_STATUS		*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: 										*/
/********************************************************/
E_SW_REC_BTN_STATUS Sw_control_GetRecButtonStatus(void)
{
	return S_sw_rec_btn_status;
}

/********************************************************/
/* �֐�����		: Sw_control_GetRecButtonChangeCount	*/
/* �@�\�̐���	: REC�{�^�����OnOff�ω��񐔎擾		*/
/* ����			: void									*/
/* �߂�l		: �ω���								*/
/* �쐬��		: (OFT)Sakata							*/
/* �쐬��		: 16/08/18								*/
/* ���l			: REC�{�^�����(ON/OFF)�ω���			*/
/* 				: �������ł͕ω����Ȃ�					*/
/********************************************************/
UI_8 Sw_control_GetRecButtonOnOffChangeCount(void)
{
	return S_sw_btn_onoff_cnt;
}

#ifdef _VDV_APP
/********************************************************/
/* �֐�����		: Sw_control_ReqInspection				*/
/* �@�\�̐���	: SW�����v��							*/
/* ����			: void									*/
/* �߂�l		: ����	:N_OK							*/
/*				  �ُ�	:N_NG							*/
/* �쐬��		: (OFT)Toguchi							*/
/* �쐬��		: 17/01/20								*/
/* ���l			: �{�֐�������I�������ꍇ�A�������䃂�W���[���ɑ΂���	*/
/*				   Insp_notify_inspection_result�ɂ�茟�����ʂ�ʒm����*/
/********************************************************/
SI_8 Sw_control_ReqInspection(void)
{
	E_SYS_CTRL_STATUS	sysStat;

	/* �V�X�e����Ԏ擾	*/
	sysStat = System_control_GetStatus();
	if (sysStat != SYS_OPERATION_INSPECTION) {
		/* ������Ԃł͂Ȃ�	*/
		return N_NG;
	}

	if (S_sw_inspection_info.req_flg == N_ON) {
		/* �������s��	*/
		return N_NG;
	}

	/* ��������ݒ�	*/
	sw_ctrl_initInspectionInfo();
	S_sw_inspection_info.start_time = Timer_ctrl_GetTime1ms();
	S_sw_inspection_info.step = SW_INSPECTION_STEP_1;
	S_sw_inspection_info.req_flg = N_ON;

	return N_OK;
}
#endif /* _VDV_APP */

/************************************************************/
/* �֐�����		: sw_control_judge_mode						*/
/* �@�\�̐���	: �X�C�b�`���[�h����						*/
/* ����			: void										*/
/* �߂�l		: ���ʁi�m��true / ���m��false)				*/
/* �쐬��		: (OFT)Sakata								*/
/* �쐬��		: 16/08/18									*/
/* ���l			: 											*/
/************************************************************/
SW_LOCAL B_8 sw_control_judge_mode( void )
{
	UI_8 i;
	SI_8 result;
	T_ADCDrv_DataLevel datalevel;
	
	/**	<ol>	*/	
	/**	<li>�|�[�g�iADC�j�f�[�^�擾	*/
	result = ADCDrv_GetDataLevel(AD_CH_OPERATION_SW, &datalevel);
	if(result < 0){
		/**	<li>�`���^�����O�z���iSW�j�̕ϐ����Z�b�g	*/
		S_chatt_count	= 0;
		return N_FALSE;
	}
	/* AD�ϊ��l��SW���[�h�l�ɕϊ� */
	S_swmode_hist_value[0] = sw_control_cnvSwMode(datalevel);
	if (S_chatt_count < CA_COUNT)
	{
		/**	<li>�ۑ��f�[�^��CA_COUNT�񕪈ȏソ�܂��Ă��Ȃ��ꍇ�̓J�E���g���Z	*/
		S_chatt_count++;
	}
	if (S_chatt_count >= CA_COUNT)
	{
		/**	<li>�ۑ��f�[�^��CA_COUNT�񕪈ȏソ�܂�����SW���[�h����*/
		for(i = 0 ; i < (CA_COUNT - 1) ; i++)
		{
			if(S_swmode_hist_value[i] != S_swmode_hist_value[i + 1])
			{
				/* �Ⴄ�f�[�^���������Ă��� */
				break;
			}
		}
		if(i >= (CA_COUNT - 1))
		{
			/* �Ō�܂Ń��[�v�����������ׂē����l -> ��Ԋm�� */
			/**	<li>�`���^�����O�z���iSW�j�̕ϐ����Z�b�g	*/
			S_chatt_count	= 0;
			return N_TRUE;
		}
	}
	/* �擾�l�����ւ��炷 */
	for(i = S_chatt_count ; i > 0 ; i--)
	{
		if (i < CA_COUNT)
		{
			S_swmode_hist_value[i] = S_swmode_hist_value[i - 1];
		}
	}

	/**	</ol>	*/
	return N_FALSE;
}


/************************************************************/
/* �֐�����		: sw_control_setSwitchMode					*/
/* �@�\�̐���	: �X�C�b�`���[�h�ݒ�						*/
/* ����			: �X�C�b�`���[�h�@E_SW_MODE_STATUS			*/
/* �߂�l		: void										*/
/* �쐬��		: (OFT)Sakata								*/
/* �쐬��		: 16/08/18									*/
/* ���l			: 											*/
/************************************************************/
SW_LOCAL void sw_control_setSwitchMode( E_SW_MODE_STATUS swmodestatus)
{
	SI_8 result;
	
	if(swmodestatus == SW_REC_BTN_CHANGED)
	{
		/**< �{�^����������Ă��邽��SW���[�h�͕s���Ȃ̂ŕێ��l��ݒ�	*/
		swmodestatus = S_sw_mode_status;
		/* REC�{�^��ON���o */
		if(S_sw_rec_btn_status == SYS_REC_BTN_OFF)
		{
			/* REC�{�^�� OFF -> ON���o */
			S_sw_rec_btn_status = SYS_REC_BTN_ON_SHORTPRESS;
			/* ���������o�p�J�n���ԕێ� */
			S_sw_btn_on_time = Timer_ctrl_GetTime10ms();
			/* ON/OFF�ω����o�J�E���^���Z */
			S_sw_btn_onoff_cnt++;
		}
		else if(S_sw_rec_btn_status == SYS_REC_BTN_ON_SHORTPRESS)
		{
			/* �P������� */
			result = Timer_ctrl_TimeCompare10ms( S_sw_btn_on_time, SW_REC_LONG_PRESS_TIME/TIME_MAIN );
			if ( result == N_OK )
			{
				/* �w�莞�ԕ��o�߂����@���@���������o */
				/* REC�{�^�� ���������o */
				S_sw_rec_btn_status = SYS_REC_BTN_ON_LONGPRESS;
				/* ���������o�p�J�n���ԃN���A */
				S_sw_btn_on_time = 0;
			}
		}
		else
		{
			/* nop */
		}
	}
	else
	{
		if(S_sw_rec_btn_status != SYS_REC_BTN_OFF)
		{
			/* REC�{�^�� ON -> OFF�A�܂��͖��m�� ->OFF���o */
			/* ON/OFF�ω����o�J�E���^���Z */
			S_sw_btn_onoff_cnt++;
		}
		/* �{�^�������ȊO�Ȃ�REC�{�^�� OFF���o�Ƃ��� */
		S_sw_rec_btn_status = SYS_REC_BTN_OFF;
		/* ���������o�p�J�n���ԃN���A */
		S_sw_btn_on_time = 0;
	}
	if( swmodestatus != S_sw_mode_status )
	{
		/* �V�X�e������ɒʒm */
		sw_control_notifyEvent(swmodestatus);
		/* �����ێ����[�h�X�V */
		S_sw_mode_status = swmodestatus;
	}
}

/************************************************************/
/* �֐�����		: sw_control_cnvSwMode						*/
/* �@�\�̐���	: AD�ϊ��l�ɂ���SW���[�h�֕ϊ�				*/
/* ����			: AD�ϊ��l									*/
/* �߂�l		: SW���[�h����l							*/
/* �쐬��		: (OFT)Sakata								*/
/* �쐬��		: 16/08/18									*/
/* ���l			: 											*/
/************************************************************/
SW_LOCAL E_SW_MODE_JUDGE_STATUS sw_control_cnvSwMode( UI_16 value )
{
	UI_8 i;
	if(value > ADC_VALUE_MAX)
	{
		return SW_MODE_JUDGE_INVALID;
	}

	for(i = 0 ; i < MODE_JUDGE_TBLDATA_NUM ; i++)
	{
		if( value < C_sw_mode_judge_tbl[i].minVale)
		{
			/*�͈͂̍ŏ��l��菬����*/
			break;
		}
		if( value <= C_sw_mode_judge_tbl[i].maxVale )
		{
			/*�͈͂̍ő�l�ȉ�*/
			/*�͈͓��̃f�[�^����������*/
			return C_sw_mode_judge_tbl[i].mode;
		}
	}
	return SW_MODE_JUDGE_INVALID;
}

/************************************************************/
/* �֐�����		: sw_control_notifyEvent					*/
/* �@�\�̐���	: �V�X�e������֏�Ԓʒm					*/
/* ����			: SW���[�h E_SW_MODE_STATUS					*/
/* �߂�l		: void										*/
/* �쐬��		: (OFT)Sakata								*/
/* �쐬��		: 16/08/18									*/
/* ���l			: 											*/
/************************************************************/
SW_LOCAL void sw_control_notifyEvent( E_SW_MODE_STATUS swmodestatus )
{
	E_SYS_CTRL_EVENT event;
	E_SYS_CTRL_EVENT_PARAM param;
	
	event = C_sw_mode_notify_event_tbl[swmodestatus].event;
	param = C_sw_mode_notify_event_tbl[swmodestatus].param;
	if(event == SW_NOT_NOTIFY_EVENT)
	{
		/* �C�x���g�������͔����� */
		return;
	}
	/* ����SW���[�h�ύX�C�x���g����SW���W���[������ʒm���s�� */
	if(S_firstevent_flag == N_TRUE)
	{
		S_firstevent_flag = N_FALSE;
		if(event == SYS_EVENT_CHANGE_SW_MODE)
		{
			/* �V�X�e������փC�x���g�ʒm */
			/* SW���䃂�W���[��:��Ԑ��� */
			(void)System_control_NotifyEvent(SYS_EVENT_MODULE_STAT, SYS_EVENT_PARAM_MODSTAT_FNC_OK_SW);
		}
	}
	/* �V�X�e������փC�x���g�ʒm */
	(void)System_control_NotifyEvent(event, param);
}

#ifdef _VDV_APP
/************************************************************/
/* �֐�����		: sw_ctrl_initInspectionInfo				*/
/* �@�\�̐���	: SW������񏉊���							*/
/* ����			: void										*/
/* �߂�l		: void										*/
/* �쐬��		: (OFT)Toguchi								*/
/* �쐬��		: 17/01/20									*/
/* ���l			: 											*/
/************************************************************/
SW_LOCAL void sw_ctrl_initInspectionInfo(void)
{
	(void)memset(&S_sw_inspection_info, 0, sizeof(S_sw_inspection_info));
	S_sw_inspection_info.req_flg = N_OFF;
	S_sw_inspection_info.step = SW_INSPECTION_STEP_INVALID;

	return;
}

/************************************************************/
/* �֐�����		: sw_control_inspection						*/
/* �@�\�̐���	: SW����									*/
/* ����			: void										*/
/* �߂�l		: void										*/
/* �쐬��		: (OFT)Toguchi								*/
/* �쐬��		: 17/01/20									*/
/* ���l			: 											*/
/************************************************************/
SW_LOCAL void sw_control_inspection(void)
{
	UI_32				timeout;
	SI_8				adcRes;
	T_ADCDrv_DataLevel	adcLevel;
	E_INSP_INSPECTION_RESULT	inspectRes;
	
	if ((S_sw_inspection_info.req_flg == N_OFF) ||
		(S_sw_inspection_info.step >= SW_INSPECTION_STEP_NUM)) {
		return;
	}

	inspectRes = INSP_INSPECTION_RESULT_INVALID;
	timeout = C_sw_inspection_config_tbl[S_sw_inspection_info.step].checktime;

	if (Timer_ctrl_TimeCompare1ms( S_sw_inspection_info.start_time, timeout ) == N_OK) {

		/* SW��AD�l���擾	*/
		adcRes = ADCDrv_GetDataLevel(AD_CH_OPERATION_SW, &adcLevel);
		if ((adcRes == D_OK) &&
			(C_sw_inspection_config_tbl[S_sw_inspection_info.step].adMinVale <= adcLevel) &&
			(C_sw_inspection_config_tbl[S_sw_inspection_info.step].adMaxVale >= adcLevel)) {
			/* AD�l�`�F�b�N���ʐ��� */
			if (S_sw_inspection_info.step == SW_INSPECTION_STEP_5) {
				/* SW�����I��	*/
				inspectRes = INSP_INSPECTION_RESULT_OK;
			}
			else {
				S_sw_inspection_info.step += 1;
			}
		}
		else {
			/* AD�l�`�F�b�N���ʈُ� */
			inspectRes = INSP_INSPECTION_RESULT_NG;
		}

		if (inspectRes != INSP_INSPECTION_RESULT_INVALID) {
			/* �������䃂�W���[���Ɍ��ʂ�ʒm����	*/
			Insp_notify_inspection_result(INSP_INSPECTION_TYPE_SW, NULL, inspectRes);

			/* ���������N���A	*/
			sw_ctrl_initInspectionInfo();
		}
	}

	return;
}
#endif	/* _VDV_APP */
