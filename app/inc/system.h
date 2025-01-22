/**===================================================================
 * File name       : system.h                                 
 * Description     : ���ʃV�X�e���w�b�_�t�@�C��             
 *--------------------------------------------------------------------
 * Programmed by   : �n�� �F��                              
 * Date            : 2004/01/08                             
 * Copyrights      : Nippon Seiki Co.,Ltd                   
 *--------------------------------------------------------------------
 * @version  $Revision:   1.1  $ 
 *====================================================================
 */

#ifndef NS_SYSTEM_H_
#define NS_SYSTEM_H_

#include    "type.h"


/****************************************************************/
/* �}�N����`                                                   */
/****************************************************************/
#define     N_OK            ((SI_8)0)       /* OK               */
#define     N_NG            ((SI_8)-1)      /* NG               */

#define     N_TRUE          ((B_8)1)        /* �^               */
#define     N_FALSE         ((B_8)0)        /* �U               */

#define     N_ON            ((UI_8)1)       /* ON               */
#define     N_OFF           ((UI_8)0)       /* OFF              */

#define     N_ON_ACTL       ((UI_8)0)       /* �A�N�e�B�uL��ON  */
#define     N_OFF_ACTL      ((UI_8)1)       /* �A�N�e�B�uL��OFF */

#define     N_ON_ACTH       ((UI_8)1)       /* �A�N�e�B�uH��ON  */
#define     N_OFF_ACTH      ((UI_8)0)       /* �A�N�e�B�uH��OFF */

#define     N_HIGH          ((UI_8)1)       /* �n�C���x��       */
#define     N_LOW           ((UI_8)0)       /* ���E���x��       */

#define     N_ENABLE        ((UI_8)1)       /* ����             */
#define     N_DISABLE       ((UI_8)0)       /* �֎~             */

#define     N_NULL          (0)     		/* �k��             */

#define     N_PI            (3.1415926535)	/* �~����           */

#define     N_AOFF          (0x00000000)	/* �S��0            */
#define     N_AON           (0xFFFFFFFF)	/* �S��1            */	
		 
#endif	// NS_SYSTEM_H_
