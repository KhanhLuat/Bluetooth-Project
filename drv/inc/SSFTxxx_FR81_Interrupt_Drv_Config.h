/************************************************************************************************/
/* �q�於		:	�@��ˑ����W���[��															*/
/* �@�햼		:	-																			*/
/* ϲ�ݿ��ðϖ�	:	-																			*/
/*==============================================================================================*/
/* �쐬̧�ٖ�	:	SSFTxxx_FR81_Interrupt_Drv_Config.h											*/
/* 				:	�����ݐ����̨��ͯ���														*/
/*==============================================================================================*/
/* �Ώ�ϲ��		:	MB91570 Series																*/
/*==============================================================================================*/
/* �쐬�ް�ޮ�	:	010106																		*/
/* �쐬�N����	:	2013.07.01																	*/
/* �쐬��		:	M.Inoue																		*/
/*----------------------------------------------------------------------------------------------*/
/* �ύX����		:																				*/
/* [010101]		:	�V�K�쐬																	*/
/* [010102]		:																				*/
/* [010103]		:																				*/
/* [010104]		:																				*/
/* [010105]		:	�X�^���o�C���̏����ǉ�														*/
/* [010106]		:	�����ݏ����̕ύX															*/
/************************************************************************************************/
#ifndef __MB91570_INTERRUPT_DRV_CONFIG_H__
#define __MB91570_INTERRUPT_DRV_CONFIG_H__

/*==============================================================================*/
/*	typedef��`																	*/
/*==============================================================================*/

/*==============================================================================*/
/*	define��`�i�@��ˑ��j														*/
/*==============================================================================*/

/* �����萔 */
#ifdef __MB91570_INTERRUPT_DRV_INTERNAL__
/* ���������������������������������������������������������������������������������������������� */
/*            �����݃R���g���[�����W�X�^ 00�`47 : ICR00 �` ICR47                                  */
/* ���������������������������������������������������������������������������������������������� */

#define INIT_ICR00			(0x1F)		/* �O��������0-7 */
#define INIT_ICR01			(0x14)		/* �O��������8-15 */
#define INIT_ICR02			(0x11)		/* �����[�h�^�C�}0/1/4/5 */
#define INIT_ICR03			(0x19)		/* �����[�h�^�C�}2/3/6 */
#define INIT_ICR04			(0x1F)		/* �}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.0 (��M����)/�}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.0 (�X�e�[�^�X) */
#define INIT_ICR05			(0x1F)		/* �}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.0 (���M����) */
#define INIT_ICR06			(0x1F)		/* �}���`�t�@���N�V�����V���A�� �C���^�t�F�[�X ch.1 (��M����)/ �}���`�t�@���N�V�����V���A�� �C���^�t�F�[�X ch.1 (�X�e�[�^�X) */
#define INIT_ICR07			(0x1F)		/* �}���`�t�@���N�V�����V���A�� �C���^�t�F�[�X ch.1 (���M����) */
#define INIT_ICR08			(0x1F)		/* LIN-UART2 (��M����) */
#define INIT_ICR09			(0x1F)		/* LIN-UART2 (���M����) */
#define INIT_ICR10			(0x1F)		/* LIN-UART3 (��M����) */
#define INIT_ICR11			(0x1F)		/* LIN-UART3 (���M����) */
#define INIT_ICR12			(0x1F)		/* LIN-UART4 (��M����) */
#define INIT_ICR13			(0x1F)		/* LIN-UART4 (���M����) */
#define INIT_ICR14			(0x1F)		/* LIN-UART5 (��M����) */
#define INIT_ICR15			(0x1F)		/* LIN-UART5 (���M����) */
#define INIT_ICR16			(0x12)		/* Bluetooth:LIN-UART6 (��M����) */
#define INIT_ICR17			(0x12)		/* Bluetooth:LIN-UART6 (���M����) */
#define INIT_ICR18			(0x15)		/* CAN0 */
#define INIT_ICR19			(0x16)		/* CAN1 */
#define INIT_ICR20			(0x1F)		/* CAN2/�A�b�v�_�E���J�E���^ 0/ �A�b�v�_�E���J�E���^ 1 */
#define INIT_ICR21			(0x1F)		/* ���A���^�C���N���b�N */ 
#define INIT_ICR22			(0x15)		/* AdvanceControlUnit:�T�E���h�W�F�l���[�^ 0/LIN-UART7 (��M����) */
#define INIT_ICR23			(0x15)		/* AdvanceControlUnit:�T�E���h�W�F�l���[�^1 /LIN-UART7 (���M����) */
#define INIT_ICR24			(0x1F)		/* PPG0/1/10/11/20/21 */
#define INIT_ICR25			(0x1F)		/* PPG2/3/12/13/22/23 */
#define INIT_ICR26			(0x1F)		/* PPG4/5/14/15 */
#define INIT_ICR27			(0x1F)		/* PPG6/7/16/17 */
#define INIT_ICR28			(0x1F)		/* PPG8/9/18/19 */
#define INIT_ICR29			(0x1F)		/* �}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.8 (��M����)/�}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.8 (�X�e�[�^�X) /HS_SPI ��M�����ݗv�� */
#define INIT_ICR30			(0x1F)		/* ���C���^�C�}/�T�u�^�C�}/PLL �^�C�}/�}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.8(���M����)/HS_SPI ���M�����ݗv�� */
#define INIT_ICR31			(0x13)		/* GNSS:�N���b�N�L�����u���[�V�������j�b�g(�T�u���U) /�T�E���h�W�F�l���[�^ 4/�}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.9 (��M����) /�}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.9 (�X�e�[�^�X) */
#define INIT_ICR32			(0x1F)		/* A/D �R���o�[�^ */
#define INIT_ICR33			(0x13)		/* GNSS:�N���b�N�L�����u���[�V�������j�b�g(CR���U) / �}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.9 (���M����) */
#define INIT_ICR34			(0x1F)		/* �t���[�����^�C�}0/2/4 */
#define INIT_ICR35			(0x1F)		/* �t���[�����^�C�}1/3/5 */
#define INIT_ICR36			(0x1F)		/* ICU0/6 (�捞��) */
#define INIT_ICR37			(0x1F)		/* ICU1/7 (�捞��) */
#define INIT_ICR38			(0x1F)		/* ICU2/8(�捞��) */
#define INIT_ICR39			(0x1F)		/* ICU3/9(�捞��) */
#define INIT_ICR40			(0x1F)		/* ICU4/10(�捞��) */
#define INIT_ICR41			(0x1F)		/* ICU5/11(�捞��) */
#define INIT_ICR42			(0x1F)		/* OCU0/1/6/7/10/11(��v) */
#define INIT_ICR43			(0x1F)		/* OCU2/3/4/5/8/9(��v) */
#define INIT_ICR44			(0x1F)		/* �x�[�X�^�C�}0 IRQ0 /�x�[�X�^�C�}0 IRQ1 /�T�E���h�W�F�l���[�^2 */
#define INIT_ICR45			(0x1F)		/* �x�[�X�^�C�}1 IRQ0 /�x�[�X�^�C�}1 IRQ1 /�T�E���h�W�F�l���[�^3 /XBS RAM �V���O���r�b�g�G���[����/Backup RAM �V���O���r�b�g�G���[���� */
#define INIT_ICR46			(0x1F)		/* DMAC0/1/2/3/4/5/6/7/8/9/10/11/12/13/14/15 */
#define INIT_ICR47			(0x1F)		/* �x�������� */
#define INIT_DICR			(0x00)		/* �x�������ݐ��䃌�W�X�^ */
										/* <07-01>	-		: 0			(�\��bit ) */
										/* <00>		DLY1	: 0			(�x�������݋��r�b�g �x�������ݗv������) */



#define STOP_ICR00			(0x1F)		/* �O��������0-7 */
#define STOP_ICR01			(0x1F)		/* �O��������8-15 */
#define STOP_ICR02			(0x1F)		/* �����[�h�^�C�}0/1/4/5 */
#define STOP_ICR03			(0x1F)		/* �����[�h�^�C�}2/3/6 */
#define STOP_ICR04			(0x1F)		/* �}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.0 (��M����)/�}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.0 (�X�e�[�^�X) */
#define STOP_ICR05			(0x1F)		/* �}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.0 (���M����) */
#define STOP_ICR06			(0x1F)		/* �}���`�t�@���N�V�����V���A�� �C���^�t�F�[�X ch.1 (��M����)/ �}���`�t�@���N�V�����V���A�� �C���^�t�F�[�X ch.1 (�X�e�[�^�X) */
#define STOP_ICR07			(0x1F)		/* �}���`�t�@���N�V�����V���A�� �C���^�t�F�[�X ch.1 (���M����) */
#define STOP_ICR08			(0x1F)		/* LIN-UART2 (��M����) */
#define STOP_ICR09			(0x1F)		/* LIN-UART2 (���M����) */
#define STOP_ICR10			(0x1F)		/* LIN-UART3 (��M����) */
#define STOP_ICR11			(0x1F)		/* LIN-UART3 (���M����) */
#define STOP_ICR12			(0x1F)		/* LIN-UART4 (��M����) */
#define STOP_ICR13			(0x1F)		/* LIN-UART4 (���M����) */
#define STOP_ICR14			(0x1F)		/* LIN-UART5 (��M����) */
#define STOP_ICR15			(0x1F)		/* LIN-UART5 (���M����) */
#define STOP_ICR16			(0x1F)		/* LIN-UART6 (��M����) */
#define STOP_ICR17			(0x1F)		/* LIN-UART6 (���M����) */
#define STOP_ICR18			(0x1F)		/* CAN0 */
#define STOP_ICR19			(0x1F)		/* CAN1 */
#define STOP_ICR20			(0x1F)		/* CAN2/�A�b�v�_�E���J�E���^ 0/ �A�b�v�_�E���J�E���^ 1 */
#define STOP_ICR21			(0x1F)		/* ���v���[�h(�d���Ւf)�ł͊��荞�݃��x�����荞�ݖ����ɂ��� */
#define STOP_ICR22			(0x1F)		/* �T�E���h�W�F�l���[�^ 0/LIN-UART7 (��M����) */
#define STOP_ICR23			(0x1F)		/* �T�E���h�W�F�l���[�^1 /LIN-UART7 (���M����) */
#define STOP_ICR24			(0x1F)		/* PPG0/1/10/11/20/21 */
#define STOP_ICR25			(0x1F)		/* PPG2/3/12/13/22/23 */
#define STOP_ICR26			(0x1F)		/* PPG4/5/14/15 */
#define STOP_ICR27			(0x1F)		/* PPG6/7/16/17 */
#define STOP_ICR28			(0x1F)		/* PPG8/9/18/19 */
#define STOP_ICR29			(0x1F)		/* �}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.8 (��M����)/�}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.8 (�X�e�[�^�X) /HS_SPI ��M�����ݗv�� */
#define STOP_ICR30			(0x1F)		/* ���C���^�C�}/�T�u�^�C�}/PLL �^�C�}/�}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.8(���M����)/HS_SPI ���M�����ݗv�� */
#define STOP_ICR31			(0x1F)		/* �N���b�N�L�����u���[�V�������j�b�g(�T�u���U) /�T�E���h�W�F�l���[�^ 4/�}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.9 (��M����) /�}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.9 (�X�e�[�^�X) */
#define STOP_ICR32			(0x1F)		/* A/D �R���o�[�^ */
#define STOP_ICR33			(0x1F)		/* �N���b�N�L�����u���[�V�������j�b�g(CR���U) / �}���`�t�@���N�V�����V���A���C���^�t�F�[�Xch.9 (���M����) */
#define STOP_ICR34			(0x1F)		/* �t���[�����^�C�}0/2/4 */
#define STOP_ICR35			(0x1F)		/* �t���[�����^�C�}1/3/5 */
#define STOP_ICR36			(0x1F)		/* ICU0/6 (�捞��) */
#define STOP_ICR37			(0x1F)		/* ICU1/7 (�捞��) */
#define STOP_ICR38			(0x1F)		/* ICU2/8(�捞��) */
#define STOP_ICR39			(0x1F)		/* ICU3/9(�捞��) */
#define STOP_ICR40			(0x1F)		/* ICU4/10(�捞��) */
#define STOP_ICR41			(0x1F)		/* ICU5/11(�捞��) */
#define STOP_ICR42			(0x1F)		/* OCU0/1/6/7/10/11(��v) */
#define STOP_ICR43			(0x1F)		/* OCU2/3/4/5/8/9(��v) */
#define STOP_ICR44			(0x1F)		/* �x�[�X�^�C�}0 IRQ0 /�x�[�X�^�C�}0 IRQ1 /�T�E���h�W�F�l���[�^2 */
#define STOP_ICR45			(0x1F)		/* �x�[�X�^�C�}1 IRQ0 /�x�[�X�^�C�}1 IRQ1 /�T�E���h�W�F�l���[�^3 /XBS RAM �V���O���r�b�g�G���[����/Backup RAM �V���O���r�b�g�G���[���� */
#define STOP_ICR46			(0x1F)		/* DMAC0/1/2/3/4/5/6/7/8/9/10/11/12/13/14/15 */
#define STOP_ICR47			(0x1F)		/* �x�������� */
#define STOP_DICR			(0x00)		/* �x�������� */

#define STBY_REGU_OUT		(0x06)		/* �X�^���o�C���M�����[�^�o��[1.2V](�K��1.2V�Ƃ��邱��)  */

#endif									/*  __MB91570_INTERRUPT_DRV_INTERNAL__ */


/* �O�����J�萔 */
/*==============================================================================*/
/*	enum��`�i�O������J�F�A�v�����Ŏg�p����ꍇ�A�O�����J�p�ɍēx��`���邱��	*/
/*==============================================================================*/
/* �����݃R���g���[�����W�X�^���w�肷��^ */
typedef enum{
	ICR_NUM0,			/* �����݃R���g���[�����W�X�^0 */
	ICR_NUM1,			/* �����݃R���g���[�����W�X�^1 */
	ICR_NUM2,			/* �����݃R���g���[�����W�X�^2 */
	ICR_NUM3,			/* �����݃R���g���[�����W�X�^3 */
	ICR_NUM4,			/* �����݃R���g���[�����W�X�^4 */
	ICR_NUM5,			/* �����݃R���g���[�����W�X�^5 */
	ICR_NUM6,			/* �����݃R���g���[�����W�X�^6 */
	ICR_NUM7,			/* �����݃R���g���[�����W�X�^7 */
	ICR_NUM8,			/* �����݃R���g���[�����W�X�^8 */
	ICR_NUM9,			/* �����݃R���g���[�����W�X�^9 */
	ICR_NUM10,			/* �����݃R���g���[�����W�X�^10 */
	ICR_NUM11,			/* �����݃R���g���[�����W�X�^11 */
	ICR_NUM12,			/* �����݃R���g���[�����W�X�^12 */
	ICR_NUM13,			/* �����݃R���g���[�����W�X�^13 */
	ICR_NUM14,			/* �����݃R���g���[�����W�X�^14 */
	ICR_NUM15,			/* �����݃R���g���[�����W�X�^15 */
	ICR_NUM16,			/* �����݃R���g���[�����W�X�^16 */
	ICR_NUM17,			/* �����݃R���g���[�����W�X�^17 */
	ICR_NUM18,			/* �����݃R���g���[�����W�X�^18 */
	ICR_NUM19,			/* �����݃R���g���[�����W�X�^19 */
	ICR_NUM20,			/* �����݃R���g���[�����W�X�^20 */
	ICR_NUM21,			/* �����݃R���g���[�����W�X�^21 */
	ICR_NUM22,			/* �����݃R���g���[�����W�X�^22 */
	ICR_NUM23,			/* �����݃R���g���[�����W�X�^23 */
	ICR_NUM24,			/* �����݃R���g���[�����W�X�^24 */
	ICR_NUM25,			/* �����݃R���g���[�����W�X�^25 */
	ICR_NUM26,			/* �����݃R���g���[�����W�X�^26 */
	ICR_NUM27,			/* �����݃R���g���[�����W�X�^27 */
	ICR_NUM28,			/* �����݃R���g���[�����W�X�^28 */
	ICR_NUM29,			/* �����݃R���g���[�����W�X�^29 */
	ICR_NUM30,			/* �����݃R���g���[�����W�X�^30 */
	ICR_NUM31,			/* �����݃R���g���[�����W�X�^31 */
	ICR_NUM32,			/* �����݃R���g���[�����W�X�^32 */
	ICR_NUM33,			/* �����݃R���g���[�����W�X�^33 */
	ICR_NUM34,			/* �����݃R���g���[�����W�X�^34 */
	ICR_NUM35,			/* �����݃R���g���[�����W�X�^35 */
	ICR_NUM36,			/* �����݃R���g���[�����W�X�^36 */
	ICR_NUM37,			/* �����݃R���g���[�����W�X�^37 */
	ICR_NUM38,			/* �����݃R���g���[�����W�X�^38 */
	ICR_NUM39,			/* �����݃R���g���[�����W�X�^39 */
	ICR_NUM40,			/* �����݃R���g���[�����W�X�^40 */
	ICR_NUM41,			/* �����݃R���g���[�����W�X�^41 */
	ICR_NUM42,			/* �����݃R���g���[�����W�X�^42 */
	ICR_NUM43,			/* �����݃R���g���[�����W�X�^43 */
	ICR_NUM44,			/* �����݃R���g���[�����W�X�^44 */
	ICR_NUM45,			/* �����݃R���g���[�����W�X�^45 */
	ICR_NUM46,			/* �����݃R���g���[�����W�X�^46 */
	ICR_NUM47,			/* �����݃R���g���[�����W�X�^47 */
	ICR_NUMBER			/* �����݃R���g���[�����W�X�^�� �ő� */
}E_ICR_NUM;

/*==============================================================================*/
/*	define��`�i�O�����J�j														*/
/*==============================================================================*/
#define INTR_DISABLE_LEVEL			(0x10)			/* �����ݍō����x�� (16) */
#define FLASH_ACCESS_INTR_LEVEL		(0x10)			/* �t���b�V�����������p(�S�Ă̊����݂𔭐������Ȃ�) */
#define I2C_RX_INTR_LEVEL			(0x12) 			/* I2C (��M����)�����݃��x����臒l(I2C (��M����)�����݃��x���̒l-1) (18) */
#define VAR_IMASK_INTR_LEVEL		(0x14)			/* ���x�������݋֎~��臒l(DMA�AI2C��W���Ȃ��ō����x��) (20) */
#define PORT_INTR_LEVEL				(0x14)			/* GPIO�̊����ݗ}�~���x��(1m�����݂𔭐������Ȃ�) �F���x�������݋֎~�Ɠ��� (20) */
#define MOTOR_INTR_LEVEL			(0x14)			/* �Q�[�W���荞�ݗ}�~���x��(1m�����݂𔭐������Ȃ�) (20) */
#define CAN_INTR_LEVEL_PRIORITY_1	(0x14)			/* CAN�����݃��x����臒l(CAN�����݂��܂߂��A����ȉ��̗D��x�̊����݂𔭐������Ȃ��j (20) */
#define CAN_INTR_LEVEL_PRIORITY_2	(0x17)			/* CAN�����݃��x����臒l(CAN�����ݖ����̗D��x�̊����݂𔭐������Ȃ��j (23) */
#define MAIN_INTR_LEVEL				(0x19)			/* ���C�����������݃��x����臒l (25) */
#define INTR_ENABLE_LEVEL			(0x1F)			/* �����ݍŒ჌�x��(�����ݏ������N�����Ȃ����x��) (31) */

#endif									/*	__MB91570_INTERRUPT_DRV_CONFIG_H__ 	*/
