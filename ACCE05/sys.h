/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : sys.c
 *    Description : System module include file
 *
 *    History :
 *    1. Date        : 13, February 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 18137 $
 **************************************************************************/
#include "board.h"

#ifndef __SYS_H
#define __SYS_H
#include <stdio.h>
#define FORMAT_8BIT 8
#define FORMAT_16BIT 16
#define FORMAT_32BIT 32
#define BIT_22 22
#define BIT_23 23
#define BIT_24 24
#define BIT_25 25

#define VALUE_0 0

#define VALUE_1 1

int complement(int value,int format);
void SetBitValue2Register(int bit, int value, volatile unsigned long *reg,int format);

/*************************************************************************
 * Function Name: fiq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: FIQ handler
 *
 *************************************************************************/
/*
// MODIF
#ifdef __GNUC__
void __attribute__((interrupt("FIQ"))) FIQ_handler() ;
#else
__fiq __arm void FIQ_handler(void) ;
#endif

//__fiq __arm void FIQ_Handler (void);
*/

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ handler
 *
 *************************************************************************/
/*
//MODIF
#ifdef __GNUC__
__attribute__((interrupt("IRQ"))) void IRQ_Handler(void) ;
#else
__irq __arm void IRQ_Handler(void) ;
#endif

//__irq __arm void IRQ_Handler (void);
*/

/*************************************************************************
 * Function Name: VIC_Init
 * Parameters: void
 * Return: void
 *
 * Description: Initialize VIC
 *
 *************************************************************************/
void VIC_Init(void);

/*************************************************************************
 * Function Name: VIC_SetVectoredIRQ
 * Parameters:  void(*pIRQSub)()
 *              unsigned int VicIrqSlot
 *              unsigned int VicIntSouce
 *
 * Return: void
 *
 * Description:  Init vectored interrupts
 *
 *************************************************************************/
void VIC_SetVectoredIRQ(void(*pIRQSub)(), unsigned int Priority,
                        unsigned int VicIntSource);

/*************************************************************************
 * Function Name: InitClock
 * Parameters: void
 * Return: void
 *
 * Description: Initialize PLL and clocks' dividers. Hclk - 288MHz,
 * Cclk- 48MHz, Usbclk - 48MHz
 *
 *************************************************************************/
void InitClock(void);

/*************************************************************************
 * Function Name: SYS_GetFsclk
 * Parameters: none
 * Return: Int32U
 *
 * Description: return Sclk [Hz]
 *
 *************************************************************************/
Int32U SYS_GetFsclk(void);

/*************************************************************************
 * Function Name: SYS_GetFpclk
 * Parameters: Int32U Periphery
 * Return: Int32U
 *
 * Description: return Pclk [Hz]
 *
 *************************************************************************/
Int32U SYS_GetFpclk(Int32U Periphery);

/*************************************************************************
 * Function Name: GpioInit
 * Parameters: void
 * Return: void
 *
 * Description: Reset all GPIO pins to default: primary function
 *
 *************************************************************************/
void GpioInit(void);

#endif // __SYS_H
