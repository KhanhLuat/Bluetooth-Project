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
/** \file vectors.c
 **
 ** Interrupt vector definition
 **
 ** History:
 **   - 2012-03-16  1.01  MKu  First edition
 **   - 2013-07-26  2.00  SWi  Added: initialization of IRQ levels
 **                            Added: vector table with deatailed description added
 **   - 2013-10-04  2.01  SWi  changed: file style
 **							   new disclaimer added
 **   - 2014-02-17  2.02  KIs  Deleted: Interrupt Vector 12 description
 *****************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/  
#include "_fr.h"
#include "vectors.h"

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
extern __interrupt void Timer_ctrl_ReloadTimerHandler( void );
extern __interrupt void Reg_setting_ReloadTimerHandler( void );
#ifdef _VDV_APP
extern __interrupt void Intr_LinUart7_Rx (void);
extern __interrupt void Intr_LinUart7_Tx (void);
extern __interrupt void Intr_LinUart6_Rx (void);
extern __interrupt void Intr_LinUart6_Tx (void);
extern __interrupt void CanDrv_interrupt_can_0(void);
extern __interrupt void CanDrv_interrupt_can_1(void);
extern __interrupt void CanDrv_interrupt_can_2(void);
extern __interrupt void Gnss_uart_drv_Interrupt_rx9(void);
extern __interrupt void Gnss_uart_drv_Interrupt_tx9(void);
#endif /* _VDV_APP */

/**
 ******************************************************************************
  Prototypes
  
  Add your own prototypes here. Each vector definition needs is proto-
  type. Either do it here or include a header file containing them.

*****************************************************************************/


extern    __interrupt void _start(void);
__interrupt void Vectors_Isr_DefaultHandler (void);

/**
 ******************************************************************************
   Vector definiton

   Use following statements to define vectors. All resource related
   vectors are predefined. Remaining software interrupts can be added here
   as well.
*****************************************************************************/
#pragma intvect _start                      0    /* reset vector                         */
#pragma intvect Vectors_Isr_DefaultHandler_5  5    /* FPU exception                        */
#pragma intvect Vectors_Isr_DefaultHandler_6  6    /* MPU Instruction access violation     */
#pragma intvect Vectors_Isr_DefaultHandler_7  7    /* MPU Data access violation            */
#pragma intvect Vectors_Isr_DefaultHandler_8  8    /* MPU Data access error                */
#pragma intvect Vectors_Isr_DefaultHandler_9  9    /* INTE                                 */
#pragma intvect Vectors_Isr_DefaultHandler_10 10    /* Instruction Break                    */
#pragma intvect Vectors_Isr_DefaultHandler_14 14    /* Illegal Instruction exception        */
#pragma intvect Vectors_Isr_DefaultHandler_15 15    /* Non Maskable Interrupt               */
#pragma intvect Vectors_Isr_DefaultHandler_16 16    /* External Interrupt 0               */
#pragma intvect Vectors_Isr_DefaultHandler_17 17    /* External Interrupt 8                 */
#pragma intvect Timer_ctrl_ReloadTimerHandler	18		/* Reload Timer 0                       */
#pragma intvect Reg_setting_ReloadTimerHandler	19		/* Reload Timer 2                       */
#pragma intvect Vectors_Isr_DefaultHandler_20 20    /* MFS ch.0 RX completed / status       */
#pragma intvect Vectors_Isr_DefaultHandler_21 21    /* MFS ch.0 TX completed                */
#pragma intvect Vectors_Isr_DefaultHandler_22 22    /* MFS ch.1 RX completed / status       */
#pragma intvect Vectors_Isr_DefaultHandler_23 23    /* MFS ch.1 TX completed                */
#pragma intvect Vectors_Isr_DefaultHandler_24 24    /* LIN-Uart 2 RX completed              */
#pragma intvect Vectors_Isr_DefaultHandler_25 25    /* LIN-Uart 2 TX completed              */
#pragma intvect Vectors_Isr_DefaultHandler_26 26    /* LIN-Uart 3 RX completed              */
#pragma intvect Vectors_Isr_DefaultHandler_27 27    /* LIN-Uart 3 TX completed              */
#pragma intvect Vectors_Isr_DefaultHandler_28 28    /* LIN-Uart 4 RX completed              */
#pragma intvect Vectors_Isr_DefaultHandler_29 29    /* LIN-Uart 4 TX completed              */
#pragma intvect Vectors_Isr_DefaultHandler_30 30    /* LIN-Uart 5 RX completed              */
#pragma intvect Vectors_Isr_DefaultHandler_31 31    /* LIN-Uart 5 TX completed              */
#ifdef _VDV_APP
#pragma intvect Intr_LinUart6_Rx 32    /* LIN-Uart 6 RX completed              */
#pragma intvect Intr_LinUart6_Tx 33    /* LIN-Uart 6 TX completed              */
#pragma intvect CanDrv_interrupt_can_0 		  34    /* CAN 0                                */
#pragma intvect CanDrv_interrupt_can_1 		  35    /* CAN 1                                */
#pragma intvect CanDrv_interrupt_can_2 		  36    /* CAN 2                                */
#else /* _VDV_APP */
#pragma intvect Vectors_Isr_DefaultHandler_32 32    /* LIN-Uart 6 RX completed              */
#pragma intvect Vectors_Isr_DefaultHandler_33 33    /* LIN-Uart 6 TX completed              */
#pragma intvect Vectors_Isr_DefaultHandler_34 34    /* CAN 0                                */
#pragma intvect Vectors_Isr_DefaultHandler_35 35    /* CAN 1                                */
#pragma intvect Vectors_Isr_DefaultHandler_36 36    /* CAN 2                                */
#endif /* _VDV_APP */
#pragma intvect Vectors_Isr_DefaultHandler_37 37    /* Real Time Clock (RTC)                */
#ifdef _VDV_APP
#pragma intvect Intr_LinUart7_Rx 38              /* LIN-Uart7 RX completed         		 */
#pragma intvect Intr_LinUart7_Tx 39              /* LIN-Uart7 TX completed               */
#else /* _VDV_APP */
#pragma intvect Vectors_Isr_DefaultHandler_38 38    /* LIN-Uart7 RX completed         		 */
#pragma intvect Vectors_Isr_DefaultHandler_39 39    /* LIN-Uart7 TX completed               */
#endif /* _VDV_APP */
#pragma intvect Vectors_Isr_DefaultHandler_40 40    /* PPG ch.0                             */
#pragma intvect Vectors_Isr_DefaultHandler_41 41    /* PPG ch.2                             */
#pragma intvect Vectors_Isr_DefaultHandler_42 42    /* PPG ch.4                             */
#pragma intvect Vectors_Isr_DefaultHandler_43 43    /* PPG ch.6                             */
#pragma intvect Vectors_Isr_DefaultHandler_44 44    /* PPG ch.8                             */
#pragma intvect Vectors_Isr_DefaultHandler_45 45    /* MFS ch.8 RX completed / status       */
#pragma intvect Vectors_Isr_DefaultHandler_46 46    /* Main Timer       					 */
#ifdef _VDV_APP
#pragma intvect Gnss_uart_drv_Interrupt_rx9   47    /* Clock calibration unit (SUB Osc)	 */
#else /* _VDV_APP */
#pragma intvect Vectors_Isr_DefaultHandler_47 47    /* Clock calibration unit (SUB Osc)	 */
#endif /* _VDV_APP */
#pragma intvect Vectors_Isr_DefaultHandler_48 48    /* AD ch.0                              */
#ifdef _VDV_APP
#pragma intvect Gnss_uart_drv_Interrupt_tx9   49    /* Clock Calibration Unit (RC Osc.)     */
#else /* _VDV_APP */
#pragma intvect Vectors_Isr_DefaultHandler_49 49    /* Clock Calibration Unit (RC Osc.)     */
#endif /* _VDV_APP */
#pragma intvect Vectors_Isr_DefaultHandler_50 50    /* 32bit FRT0                           */
#pragma intvect Vectors_Isr_DefaultHandler_51 51    /* 32bit FRT1                           */
#pragma intvect Vectors_Isr_DefaultHandler_52 52    /* 32bit ICU 0 (fetching)       		 */
#pragma intvect Vectors_Isr_DefaultHandler_53 53    /* 32bit ICU 0 (fetching)       		 */
#pragma intvect Vectors_Isr_DefaultHandler_54 54    /* 32bit ICU 2 (fetching)       		 */
#pragma intvect Vectors_Isr_DefaultHandler_55 55    /* 32bit ICU 3 (fetching)       		 */
#pragma intvect Vectors_Isr_DefaultHandler_56 56    /* 32bit ICU 4 (fetching)       		 */
#pragma intvect Vectors_Isr_DefaultHandler_57 57    /* 32bit ICU 5 (fetching)              */
#pragma intvect Vectors_Isr_DefaultHandler_58 58    /* 32bit OCU 0 (match)                  */
#pragma intvect Vectors_Isr_DefaultHandler_59 59    /* 32bit OCU 2 (match)                  */
#pragma intvect Vectors_Isr_DefaultHandler_60 60    /* Base Timer 0 IRQ0                    */
#pragma intvect Vectors_Isr_DefaultHandler_61 61    /* Base Timer 1 IRQ0                    */
#pragma intvect Vectors_Isr_DefaultHandler_62 62    /* DMAC 0                               */
#pragma intvect Vectors_Isr_DefaultHandler_63 63    /* Delay Interrupt                      */
#pragma intvect Vectors_Isr_DefaultHandler_64 64    /* System reserved                      */
#pragma intvect Vectors_Isr_DefaultHandler_65 65    /* System reserved                      */

#pragma defvect Vectors_Isr_DefaultHandler       /* default Vector */


/**
 ******************************************************************************
 ** This function is a placeholder for all vector definitions. Either use
 ** your own placeholder or add necessary code here.
 *****************************************************************************/
__interrupt void Vectors_Isr_DefaultHandler (void)
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}


__interrupt void Vectors_Isr_DefaultHandler_5   (void)
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_6   (void)
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_7   (void)
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_8   (void)
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_9   (void)
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_10 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_14 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_15 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_16 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_17 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_20 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_21 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_22 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_23 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_24 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_25 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_26 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_27 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_28 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_29 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_30 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_31 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_32 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_33 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_34 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_35 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_36 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_37 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_38 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_39 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_40 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_41 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_42 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_43 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_44 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_45 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_46 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_47 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_48 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_49 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_50 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_51 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_52 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_53 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_54 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_55 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_56 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_57 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_58 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_59 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_60 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_61 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_62 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_63 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_64 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
__interrupt void Vectors_Isr_DefaultHandler_65 (void) 
{
    // disable interrupts
    __DI();

    // halt system or wait for watchdog reset
    while(1)
    {
        __wait_nop();
    }
}
