/*******************************************************************************
* Copyright (C) 2013 Spansion LLC. All Rights Reserved. 
*
* This software is owned and published by: 
* Spansion LLC, 915 DeGuigne Dr. Sunnyvale, CA  94088-3453 ("Spansion").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND 
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with Spansion 
* components. This software is licensed by Spansion to be adapted only 
* for use in systems utilizing Spansion components. Spansion shall not be 
* responsible for misuse or illegal use of this software for devices not 
* supported herein.  Spansion is providing this software "AS IS" and will 
* not be responsible for issues arising from incorrect user implementation 
* of the software.  
*
* SPANSION MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS), 
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING, 
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED 
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED 
* WARRANTY OF NONINFRINGEMENT.  
* SPANSION SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, 
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT 
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, 
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR 
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, 
* SAVINGS OR PROFITS, 
* EVEN IF SPANSION HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED 
* FROM, THE SOFTWARE.  
*
* This software may be replicated in part or whole for the licensed use, 
* with the restriction that this Disclaimer and Copyright notice must be 
* included with each copy of this software, whether used in part or whole, 
* at all times.  
*****************************************************************************/
/** \file main.c
 **
 ** Add description here...
 **
 ** History:
 **   - 2011-12-28  0.01  MKu  Preliminary version
 **   - 2012-03-09  0.02  MKu  Preliminary version(not released)
 **                            -added Watchdog timer clear in main loop
 **   - 2012-03-16  1.01  MKu  First edition
 **   - 2013-07-26  2.00  SWi  Added: initialization of IRQ levels
 **                            Fundamental Port configurations (ADC analog input disabled, global input enable)
 **   - 2013-10-04  2.01  SWi  removed: Disable all AN pins
 **							   changed: file style
 **							   added: global enable port input, new disclaimer
 *****************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/ 
#include "string.h"
#include "System_control_TASK.h"
#include "DTO459_VDV_Firm_info.h"
#include "DTO459_VDV_main.h"
#include "DTO459_VDV_File_io.h"
#ifdef _VDV_BOOT
#ifdef _FIRM_AES /* 2020.12.10 S.Sakata */
#include "DTO459_VDV_Flash_ctrl.h"
#endif /* _FIRM_AES */ /* 2020.12.10 S.Sakata */
#endif /* _VDV_BOOT */
#include "dbglog.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/


/*****************************************************************************/
/*  Main application to control the program flow                             */
/*****************************************************************************/
void main(void)
{
#ifdef _VDV_BOOT
	T_VDV_FIRM_INFO	*firmInfo;

	g_MainLoopFlag = 1;
#endif /* _VDV_BOOT */

	App_Main();

#ifdef _VDV_BOOT
    while(g_MainLoopFlag == 1)
#else /* _VDV_BOOT */
    while(1)
#endif /* _VDV_BOOT */
    {
    	/* ファイルアクセス制御 */
    	FileIO_Main();
#ifdef _VDV_BOOT
#ifdef _FIRM_AES /* 2020.12.10 S.Sakata */
		Flash_control_AesMain();
#endif /* _FIRM_AES */ /* 2020.12.10 S.Sakata */
#endif /* _VDV_BOOT */
	}

#ifdef _VDV_BOOT
	/* メイン終了処理実行 */
	App_main_Term();

	OUTLOG(OutputLog("Boot End\n");)

	/* APPスタートエントリコール */
	firmInfo = (T_VDV_FIRM_INFO*)VDV_APP_TOP_ADR;
	if (memcmp( firmInfo->label, VDV_APP_LABEL, sizeof(VDV_APP_LABEL) ) == 0) {
		firmInfo->prgEntryPoint();
	}
#endif /* _VDV_BOOT */
}
