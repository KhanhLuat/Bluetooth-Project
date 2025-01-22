/********************************************************/
/* @file DTO459_VDV_main.c								*/
/* App(Flash)�̈��main����								*/
/* 														*/
/* App(Flash)�̈��main���������B						*/
/* <ul>													*/
/* <li>App��Main�����B									*/
/* <li>10msec�����^�C�}�����ݏ����B						*/
/* </ul>												*/
/* 														*/
/********************************************************/
/* @author      Y.Sorimachi								*/
/* @date        2013/--/--								*/
/********************************************************/ 
#include "SSFTSTD_MacroFunc.h"
#include "system.h"
#include "DTO459_VDV_Firm_info.h"
#include "reg_setting.h"
#include "timer_ctrl.h"
#include "System_control_TASK.h"
#ifdef _VDV_APP
#include "DTO459_VDV_Advance_ctrl.h"
#include "DTO459_VDV_Bt_ctrl.h"
#include "DTO459_VDV_Can_ctrl.h"
#include "DTO459_VDV_Gnss_ctrl.h"
#include "DTO459_VDV_Sw_ctrl.h"
#include "DTO459_VDV_Inspection_ctrl.h"
#endif /* _VDV_APP */
#include "DTO459_VDV_Eeprom_ctrl.h"
#include "DTO459_VDV_Led_ctrl.h"
#include "DTO459_VDV_Sd_ctrl.h"
#include "DTO459_VDV_Fat.h"
#include "DTO459_VDV_File_io.h"
#include "DTO459_VDV_Common.h"
#include "DTO459_VDV_Flash_ctrl.h"
#include "DTO459_VDV_main.h"
#include "dbglog.h"
#include "time_measure.h"


/********************************************************/
/*	�����}�N��											*/
/********************************************************/


/********************************************************/
/*	�萔�錾											*/
/********************************************************/


/********************************************************/
/*	�\���̐錾											*/
/********************************************************/

/********************************************************/
/*	�����ϐ���`										*/
/********************************************************/

/********************************************************/
/*	�����֐��v���g�^�C�v�錾							*/
/********************************************************/
static void S_module_init( void );
static void S_module_term( void );

/********************************************************/
/*	�O���֐�											*/
/********************************************************/


/** 
 * /// App(Flash)�̈��main����
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	App�̃��C���������s���B
 */ 
void App_Main( void )
{
	/**	<li> �f�o�b�O���O������	*/
	OUTLOG(OutputLogInit();)

	TM_MEAS(TmMeas_Init();)

	/**	<li> �����݋֎~	*/
	(void)D_DI();

	/**	<li> �e���W���[���̏�����	*/
	S_module_init();

	/**	<li> �����݋���	*/
	(void)D_EI();
	
	/**	<li> WDT���Z�b�g	*/
	Reg_setting_WtdRestart();


	/* �����݃}�X�N�ݒ�i�S�����݋��j */
	Reg_setting_EnableIntMask();

	OUTLOG(OutputFirmVer();)	/* �t�@�[���E�F�A�o�[�W�������O�o�� */

	/**	<li> �^�C�}�����݊J�n�i10msec�j	*/
	Reg_setting_TimerStart();

	/**	</ol>	*/
	return;
}

/** 
 * �^�C�}�����ݎ��s����
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	10msec�̃^�C�}�����݊֐�������s����鏈���B
 * @note	(2016/08/18�ύX�j5msec�̃^�C�}�����݊֐�������s����鏈���B
 */ 
void App_main_IntTimerRun( void )
{
	UI_32 timer;

	TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_VDVMAIN);)

	/**	<li> WDT���Z�b�g	*/
	Reg_setting_WtdRestart();

	/**	<li>5ms���̃^�C�}�[�J�E���^�l�擾	*/
	timer = Timer_ctrl_GetTime5ms();
	if((timer % 2) == 0)
	{
		/* 5ms��1��R�[������邽��10ms���i2���1��j�������s���� */
		
		/**	<li>10ms���̃^�C�}�[�J�E���^	*/
		Timer_ctrl_TimeCount10ms();

		/**	<li>���ʒ�`	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_COMMON);)
		Common_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_COMMON);)

		/**	<li>�V�X�e����Ԑ���	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_SYSCTRL);)
		System_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_SYSCTRL);)
		
#ifdef _VDV_APP
		/**	<li> �������[�h���� */
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_INSPECT);)
		Insp_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_INSPECT);)
#endif	/* _VDV_APP */

#ifdef _VDV_APP
		/**	<li>SW����	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_SW);)
		Sw_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_SW);)
#endif /* _VDV_APP */

		/**	<li>LED����	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_LED);)
		Led_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_LED);)

#ifdef _VDV_APP
		/**	<li>CAN����	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_CAN);)
		Can_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_CAN);)

		/**	<li>GNSS����	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_GNSS);)
		Gnss_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_GNSS);)

		/**	<li>AdvanceControlUnit����	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_ADVANCE);)
		Advance_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_ADVANCE);)

		/**	<li>BT����	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_BT);)
		Bt_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_BT);)
#endif /* _VDV_APP */

		/**	<li>SD����	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_SD);)
		Sd_control_Main(SD_PROCESS_MAIN);
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_SD);)

		/**	<li>EEPROM����	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_EEPROM);)
		Eeprom_control_Main();
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_EEPROM);)

#ifdef _VDV_BOOT
		/**	<li>FLASH����	*/
		Flash_control_Main();
#endif /* _VDV_BOOT */

		/**	<li>�f�o�b�O���O����	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_LOG);)
		OUTLOG(OutputLogMain();)
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_LOG);)
	}
#ifdef _VDV_APP
	if((timer % 5) == 0)
	{
		/* 5ms��1��R�[������邽��25ms���i5���1��j�������s���� */
		/**	<li>SD����	*/
		TM_MEAS(TmMeas_StartMeasure(TM_MEAS_LOG_TYPE_SD_SUB);)
		Sd_control_Main(SD_PROCESS_SUB);
		TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_SD_SUB);)
	}
#endif /* _VDV_APP */
	/**	<ol>	*/

	TM_MEAS(TmMeas_EndMeasure(TM_MEAS_LOG_TYPE_VDVMAIN);)
	TM_MEAS(TmMeas_Main();)

	return;
}

/********************************************************/
/* �֐�����		�F App_main_Term						*/
/* �@�\�̐���	�F �I������								*/
/* ����			�F void									*/
/* �߂�l		�F void									*/
/* �쐬��		�F (OFT)Toguchi							*/
/* �쐬��		�F 16/10/21								*/
/* ���l			�F 										*/
/********************************************************/
void App_main_Term( void )
{
	/* �����݃}�X�N�ݒ�i�S�����݋֎~�j	*/
	Reg_setting_DisableIntMask();

	/* WDT���Z�b�g	*/
	Reg_setting_WtdRestart();

	/* ���W���[���I������	*/
	S_module_term();

	return;
}

/********************************************************/
/*	�����֐�											*/
/********************************************************/
/** 
 * /// ���W���[��������
 * 
 * @author	Y.Sorimachi
 * @date	2013/--/--
 * @note	�e���W���[���̏��������s���B
 */ 
/* void S_module_init( void )											;@@ DEL REQ-002(BEX019) */
static void S_module_init( void )									/*	;@@ ADD REQ-002(BEX019) */
{
	/**	<ol>	*/
	/**	<li>���W�X�^�ݒ�	*/
	Reg_setting_Init();

	/**	<li>���ʒ�`	*/
	Common_Init();

	/**	<li>�V�X�e����Ԑ���	*/
	System_control_Init();

	/** <li>�t�@�C���V�X�e��������	*/
	FileSystem_Init();

	/** <li>�t�@�C��IO���䏉����	*/
	FileIO_Init();

#ifdef _VDV_APP
	/**	<li> �������[�h���� */
	Insp_control_Init();
#endif	/* _VDV_APP */

#ifdef _VDV_APP
	/**	<li>SW����	*/
	Sw_control_Init();
#endif /* _VDV_APP */

	/**	<li>LED����	*/
	Led_control_Init();

#ifdef _VDV_APP
	/**	<li>CAN����	*/
	Can_control_Init();

	/**	<li>GNSS����	*/
	Gnss_control_Init();

	/**	<li>AdvanceControlUnit����	*/
	Advance_control_Init();

	/**	<li>BT����	*/
	Bt_control_Init();
#endif /* _VDV_APP */

	/**	<li>SD����	*/
	Sd_control_Init();

	/**	<li>EEPROM����	*/
	Eeprom_control_Init();

#ifdef _VDV_BOOT
	/**	<li>FLASH����	*/
	Flash_control_Init();
#endif /* _VDV_BOOT */

	/**	</ol>	*/	
	return;
}

/********************************************************/
/* �֐�����		�F S_module_term						*/
/* �@�\�̐���	�F ���W���[���I������					*/
/* ����			�F void									*/
/* �߂�l		�F void									*/
/* �쐬��		�F (OFT)Toguchi							*/
/* �쐬��		�F 16/10/21								*/
/* ���l			�F 										*/
/********************************************************/
static void S_module_term( void )
{
	/* ���W�X�^�I��	*/
	Reg_setting_Term();

	return;
}

