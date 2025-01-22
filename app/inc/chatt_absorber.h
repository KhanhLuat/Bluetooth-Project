/********************************************************/
/* file chatt_absorber.h								*/
/* �`���^�����O�z��										*/
/* 														*/
/* <ul>													*/
/* <li>���̓|�[�g����擾�����f�[�^�ɑ΂��`���^�����O�z������B			*/
/* <li>�`���^�����O�z�������f�[�^�́A�ݒ肳�ꂽ�֐��ɃR�[���o�b�N����B	*/
/* <li>�`���^�����O�z���́A20ms������3��A���œ����l�̂��̂Ƃ���B		*/
/* </ul>												*/
/* 														*/
/********************************************************/
/* author      Y.Sorimachi								*/
/* date        2013/--/--								*/
/********************************************************/ 
/* page */

#ifndef	CHATT_ABSORBER_H
#define	CHATT_ABSORBER_H

#include "type.h"
#include "System_control_TASK.h"

/********************************************************/
/*	�O���}�N��											*/
/********************************************************/
/**
 *	�`���^�����O�z������ėp�|�[�g
 */
typedef enum e_chatt_abs_port {
	CHATT_PORT_ACC = 0,				/**< ACC				*/
	CHATT_PORT_SDC,					/**< SD�J�[�h���o		*/
	CHATT_PORT_NUM
} E_CHATT_ABS_PORT;

/**
 *	���s����
 */
typedef enum e_chatt_abs_result {
	CHATT_ABS_OK = 0,				/**< ����I��			*/
	CHATT_ABS_ERR_INVALID_ARG		/**< �����s��			*/
} E_CHATT_ABS_RESULT;

/**
 *	�`���^�����O�z�����郂�W���[��
 */
typedef enum e_chatt_abs_module {
	CHATT_ABS_MOD_SHC = 0,			/**< ���ȕێ�����		*/
	CHATT_ABS_MOD_FIO,				/**< �t�@�C��IO����		*/
	CHATT_ABS_MOD_NUM
} E_CHATT_ABS_MODULE;


/********************************************************/
/*	�萔�錾											*/
/********************************************************/

/********************************************************/
/*	�^�錾												*/
/********************************************************/
typedef void (*P_CHATT_ABS_CB_FUNC)(UI_8);	/**< �R�[���o�b�N�֐�	*/

/********************************************************/
/*	�O���֐�											*/
/********************************************************/

/**
 * �`���^�����O�z�� ������
 * 
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		�`���^�����O�z���Ŏg�p�����RAM�̏��������s���B
 */
void Chatt_absorber_Init( void );

/**
 * �`���^�����O�z�� ���C������
 * 
 * author	Y.Sorimachi
 * date		2013/--/--
 * note		�`���^�����O�z���̃��C���������s���B
 */
void Chatt_absorber_Main( void );

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
 * date		2013/--/--
 * note		<ul>
 *			<li>�w�肵���|�[�g�̃`���^�����O�z����̃f�[�^��v������B
 *			<li>�w�肵���|�[�g�ɃG�b�W�ω����������Ƃ��A�`���^�����O�z�����ʃf�[�^���R�[���o�b�N�֐��œn���B
 *			</ul>
 */
UI_8 Chatt_absorber_ReqPortData( P_CHATT_ABS_CB_FUNC ca_data_cb, UI_8 port, UI_8 module );

#endif	/* CHATT_ABSORBER_H */
