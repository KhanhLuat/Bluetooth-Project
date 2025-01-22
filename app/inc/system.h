/**===================================================================
 * File name       : system.h                                 
 * Description     : 共通システムヘッダファイル             
 *--------------------------------------------------------------------
 * Programmed by   : 渡辺 芳樹                              
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
/* マクロ定義                                                   */
/****************************************************************/
#define     N_OK            ((SI_8)0)       /* OK               */
#define     N_NG            ((SI_8)-1)      /* NG               */

#define     N_TRUE          ((B_8)1)        /* 真               */
#define     N_FALSE         ((B_8)0)        /* 偽               */

#define     N_ON            ((UI_8)1)       /* ON               */
#define     N_OFF           ((UI_8)0)       /* OFF              */

#define     N_ON_ACTL       ((UI_8)0)       /* アクティブL時ON  */
#define     N_OFF_ACTL      ((UI_8)1)       /* アクティブL時OFF */

#define     N_ON_ACTH       ((UI_8)1)       /* アクティブH時ON  */
#define     N_OFF_ACTH      ((UI_8)0)       /* アクティブHじOFF */

#define     N_HIGH          ((UI_8)1)       /* ハイレベル       */
#define     N_LOW           ((UI_8)0)       /* ロウレベル       */

#define     N_ENABLE        ((UI_8)1)       /* 許可             */
#define     N_DISABLE       ((UI_8)0)       /* 禁止             */

#define     N_NULL          (0)     		/* ヌル             */

#define     N_PI            (3.1415926535)	/* 円周率           */

#define     N_AOFF          (0x00000000)	/* 全て0            */
#define     N_AON           (0xFFFFFFFF)	/* 全て1            */	
		 
#endif	// NS_SYSTEM_H_
