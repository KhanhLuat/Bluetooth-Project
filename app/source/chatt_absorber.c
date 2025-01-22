/********************************************************************************/
/* file chatt_absorber.c														*/
/* �`���^�����O�z��																*/
/* 																				*/
/* <ul>																			*/
/* <li>���̓|�[�g����擾�����f�[�^�ɑ΂��`���^�����O�z������B					*/
/* <li>�`���^�����O�z�������f�[�^�́A�ݒ肳�ꂽ�֐��ɃR�[���o�b�N����B			*/
/* <li>�`���^�����O�z���́A20ms������3��A���œ����l�̂��̂Ƃ���B				*/
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
/*	�����}�N��																	*/
/********************************************************************************/
#define CHATT_LOCAL	static

#define CA_COUNT					(4)		/**< �`���^�����O�z���J�E���g		*/
#define CA_COUNT_MAX				(0xFF)	/**< �`���^�����O�z���J�E���g�ő�l	*/

#define CA_CHECKING					(0xFF)	/**< �`���^�����O�z���� 		*/
#define CA_CHECK_LOW				(0x00)	/**< �`���^�����O�z�� LOW		*/
#define CA_CHECK_HIGH				(0x0F)	/**< �`���^�����O�z�� HIGH		*/
#define CA_CHECK_ACTIVE_ACC			(IO_PI_LEV_ACTIVE_ACC_STAT | (IO_PI_LEV_ACTIVE_ACC_STAT << 1) | (IO_PI_LEV_ACTIVE_ACC_STAT << 2) | (IO_PI_LEV_ACTIVE_ACC_STAT << 3))
#define CA_CHECK_NONACTIVE_ACC		(IO_PI_LEV_NONACT_ACC_STAT | (IO_PI_LEV_NONACT_ACC_STAT << 1) | (IO_PI_LEV_NONACT_ACC_STAT << 2) | (IO_PI_LEV_NONACT_ACC_STAT << 3))

#define CA_CYCLE					(2)		/**< �`���^�����O�z�� ����		*/

#define CA_MASK_BIT_0EH				(0x0E)	/**< �}�X�N�r�b�g�i0x0E�j		*/
#define CA_MASK_BIT_01H				(0x01)	/**< �}�X�N�r�b�g�i0x01�j		*/

#define CA_SHIFT_1_BIT				(0x01)	/**< 1bit�V�t�g					*/
#define CA_ACC_UNKNOWN				(0xFF)	/**< ACC�|�[�g�s��				*/

#define CA_SDC_UNKNOWN				(0xFF)	/**< SD�J�[�h���o�|�[�g�s��		*/

/********************************************************************************/
/*	�����֐��v���g�^�C�v�錾													*/
/********************************************************************************/
/*
 *	�`���^�����O�z���֐�
 */
CHATT_LOCAL void S_ca_acc( void );				/**< ACC	*/
CHATT_LOCAL void S_ca_sdc( void );				/**< SD�J�[�h	*/


/********************************************************************************/
/*	�萔�錾																	*/
/********************************************************************************/
/*
 *	�`���^�����O�z���֐��e�[�u��
 */
CHATT_LOCAL void ( * const C_ca_func[CHATT_PORT_NUM] ) ( void ) = {
	S_ca_acc,								/**< ACC		*/
	S_ca_sdc,								/**< SD�J�[�h	*/
};

/********************************************************************************/
/*	�\���̐錾																	*/
/********************************************************************************/
/**
 *	�`���^�����O�z���p�\����
 */
typedef struct t_chatt_abs {
	UI_8	data;							/**< �f�[�^		*/
	UI_8	count;							/**< �J�E���^	*/
} T_CHATT_ABS;

/********************************************************************************/
/*	�����ϐ���`																*/
/********************************************************************************/
CHATT_LOCAL T_CHATT_ABS			S_chatt_abs[CHATT_PORT_NUM];			/**< �`���^�����O�z���p�f�[�^	*/
CHATT_LOCAL P_CHATT_ABS_CB_FUNC	S_ca_acc_cb[CHATT_ABS_MOD_NUM];			/**< �`���^�����O�z���iACC�j�R�[���o�b�N�֐�	*/
CHATT_LOCAL P_CHATT_ABS_CB_FUNC	S_ca_sdc_cb[CHATT_ABS_MOD_NUM];			/**< �`���^�����O�z���iSD�J�[�h���o�j�R�[���o�b�N�֐�	*/

CHATT_LOCAL UI_8			 		S_acc_stat;								/**< ACC�|�[�g���	*/
CHATT_LOCAL UI_8			 		S_sdc_stat;								/**< SD�J�[�h���o�|�[�g���	*/

/********************************************************************************/
/*	�O���֐�																	*/
/********************************************************************************/

/**
 * �`���^�����O�z�� ������
 * 
 * author	Y.Sorimachi
 * date	2013/--/--
 * note	<ul>
 *			<li>�`���^�����O�z���Ŏg�p�����RAM������������B
 *			</ul>
 */
void Chatt_absorber_Init( void )
{
	UI_8 i;
	/**	<ol>	*/
	/**	<li>������	*/
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
	/**	<li>����̓`���^�����O�z�����s�����߁AN_ON�ɂ���	*/
	S_acc_stat						= CA_ACC_UNKNOWN;
	S_sdc_stat						= CA_SDC_UNKNOWN;
	/**	</ol>	*/	
	return;
}

/**
 * �`���^�����O�z�� ���C������
 * 
 * author	Y.Sorimachi
 * date	2013/--/--
 * note	�`���^�����O�z���̃��C�������B
 */
void Chatt_absorber_Main( void )
{
	UI_8 i;
	/**	<ol>	*/
	/**	<li>20ms���Ɏ��s	*/
	/**	<li>�`���^�����O�z�����s	*/
	for (i = 0; i < CHATT_PORT_NUM; i++)
	{
		C_ca_func[i]();
	}
	/**	</ol>	*/	
	return;
}

/**
 * �`���^�����O�z�� �|�[�g�f�[�^�v��
 * 
 * param	ca_data_cb	[in]	�`���^�����O�z����̃f�[�^�R�[���o�b�N�֐�
 * param	port		[in]	�w�肷��|�[�g�iE_CHATT_ABS_PORT�j
 * param	module		[in]	�R�[���o�b�N�֐���o�^���郂�W���[���iE_CHATT_ABS_MODULE�j
 *
 * return	���s���ʁiE_CHATT_ABS_RESULT�j
 * 
 * author	Y.Sorimachi
 * date	2013/--/--
 * note	<ul>
 *			<li>�w�肵���|�[�g�̃`���^�����O�z����̃f�[�^��v������B
 *			<li>�w�肵���|�[�g�ɃG�b�W�ω����������Ƃ��A�`���^�����O�z�����ʃf�[�^���R�[���o�b�N�֐��œn���B
 *			</ul>
 */
UI_8 Chatt_absorber_ReqPortData( P_CHATT_ABS_CB_FUNC ca_data_cb, UI_8 port, UI_8 module )
{
	/**	<ol>	*/
	/**	<li>�����`�F�b�N	*/
	if ( (ca_data_cb == N_NULL) || (port >= CHATT_PORT_NUM) || (module >= CHATT_ABS_MOD_NUM) )
	{
		return CHATT_ABS_ERR_INVALID_ARG;
	}
	
	/**	<li>�R�[���o�b�N�֐��o�^	*/
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
/*	�����֐�																	*/
/********************************************************************************/
/**
 * �`���^�����O�z���֐�:ACC
 * 
 * author	Y.Sorimachi
 * date	2013/--/--
 * note	<ul>
 *			<li>�d��OFF�����Ƃ́A�ȉ��̏������s���B
 *			</ul>
 */
CHATT_LOCAL void S_ca_acc( void )
{
	UI_8 i;
	UI_8 port_data;
	UI_8 ca_data;
	
	/**	<ol>	*/	
	/**	<li>���̓|�[�g�iACC�j�̃G�b�W�ω����������Ƃ��A�`���^�����O�z������	*/
	/**	<li>�|�[�g�iACC�j�f�[�^�擾	*/
	(void)PortDrv_GetDataBit(IO_PI_GR_ACC_STAT, IO_PI_BIT_ACC_STAT, &port_data);
	
	/**	<li>�`���^�����O�z���̂��߁A�f�[�^�ۑ��i�S�񕪁j	*/
	S_chatt_abs[CHATT_PORT_ACC].data = (S_chatt_abs[CHATT_PORT_ACC].data << CA_SHIFT_1_BIT) & CA_MASK_BIT_0EH;
	S_chatt_abs[CHATT_PORT_ACC].data |= (port_data & CA_MASK_BIT_01H);				
	if (S_chatt_abs[CHATT_PORT_ACC].count < CA_COUNT)
	{
		/**	<li>�ۑ��f�[�^��3�񕪈ȏソ�܂��Ă��Ȃ��ꍇ�̓J�E���g���Z	*/
		S_chatt_abs[CHATT_PORT_ACC].count++;
	}

	/**	<li>4�񕪃f�[�^�ۑ��ł���������	*/
	ca_data = CA_CHECKING;
	if (S_chatt_abs[CHATT_PORT_ACC].count >= CA_COUNT)
	{
		/**	<li>�`���^�����O�z���������ʁAActive����	*/
		if (S_chatt_abs[CHATT_PORT_ACC].data == CA_CHECK_ACTIVE_ACC)
		{
			ca_data = IO_PI_LEV_ACTIVE_ACC_STAT;
		}
		/**	<li>�`���^�����O�z���������ʁANonActive����	*/
		else if (S_chatt_abs[CHATT_PORT_ACC].data == CA_CHECK_NONACTIVE_ACC)
		{
			ca_data = IO_PI_LEV_NONACT_ACC_STAT;
		}
		else
		{
			/* nop */
		}
	}
	
	/**	<li>�`���^�����O�z������������ACC ON/OFF�ω����o���Ă���΁A�R�[���o�b�N�֐���ACC��n��	*/
	if ((ca_data != CA_CHECKING) && (S_acc_stat != ca_data))
	{
		/**	<li>ACC�|�[�g��ԍX�V	*/
		S_acc_stat = ca_data;
		/**	<li>�R�[���o�b�N�֐��Ăяo��	*/
		for (i = 0; i < CHATT_ABS_MOD_NUM; i++)
		{
			if (S_ca_acc_cb[i] != N_NULL)
			{
				S_ca_acc_cb[i](ca_data);
			}
		}
		/**	<li>�`���^�����O�z���iACC�j�̕ϐ����Z�b�g	*/
		S_chatt_abs[CHATT_PORT_ACC].count	= 0;
	}
	/**	</ol>	*/
	return;
}

/**
 * �`���^�����O�z���֐�:SD�J�[�h���o
 * 
* author	(OFT)Toguchi
 * date	2016/09/23
 * note	<ul>
 *			<li>SD�J�[�h��IN/OUT�����o����B
 *			</ul>
 */
CHATT_LOCAL void S_ca_sdc( void )
{
	UI_8 i;
	UI_8 port_data;

	/**	<li>�|�[�g�iSD�J�[�h���o�j�f�[�^�擾	*/
	(void)PortDrv_GetDataBit(IO_PI_GR_SD_DTCT, IO_PI_BIT_SD_DTCT, &port_data);

	/**	<li>SD�J�[�hIN/OUT�ω����o���Ă���΁A�R�[���o�b�N�֐��Ō��o���ʂ�n��	*/
	if (S_sdc_stat != port_data)
	{
		/**	<li>SD�J�[�h���o�|�[�g��ԍX�V	*/
		S_sdc_stat = port_data;
		/**	<li>�R�[���o�b�N�֐��Ăяo��	*/
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
