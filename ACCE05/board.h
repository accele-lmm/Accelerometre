/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2007
 **
 **    $Revision: 18137 $
 **
 ***************************************************************************/
//#include <intrinsics.h>
//#include <nxp/iolpc2478.h>
//#include "arm_comm.h"

#include "LPC2478-stk.h"

#ifndef __BOARD_H
#define __BOARD_H

// MODIF
/*
#define I_RC_OSC_FREQ   (4MHZ)
#define MAIN_OSC_FREQ   (12MHZ)
#define RTC_OSC_FREQ    (32768UL)

#define SYS_FREQ        (72MHZ)
*/

#define I_RC_OSC_FREQ		4000000
#define MAIN_OSC_FREQ		12000000
#define RTC_OSC_FREQ		32768

#define SYS_FREQ     		72000000


#define I2C0_INTR_PRIORITY  1
#define DLY_I2C_TIME_OUT    1000

//#if defined(IAR_LPC_2478_STK)

// USB Data Link LED
#define USB_D_LINK_LED_MASK (1UL<<18)
#define USB_D_LINK_LED_DIR  IO1DIR
#define USB_D_LINK_LED_FDIR FIO1DIR
#define USB_D_LINK_LED_SET  IO1SET
#define USB_D_LINK_LED_FSET FIO1SET
#define USB_D_LINK_LED_CLR  IO1CLR
#define USB_D_LINK_LED_FCLR FIO1CLR
#define USB_D_LINK_LED_IO   IO1PIN
#define USB_D_LINK_LED_FIO  FIO1PIN

// USB Host Link LED
#define USB_H_LINK_LED_MASK (1UL<<13)
#define USB_H_LINK_LED_DIR  IO1DIR
#define USB_H_LINK_LED_FDIR FIO1DIR
#define USB_H_LINK_LED_SET  IO1SET
#define USB_H_LINK_LED_FSET FIO1SET
#define USB_H_LINK_LED_CLR  IO1CLR
#define USB_H_LINK_LED_FCLR FIO1CLR
#define USB_H_LINK_LED_IO   IO1PIN
#define USB_H_LINK_LED_FIO  FIO1PIN

// Buttons
#define BUT1_MASK           (1UL<<19)
#define BUT1_FDIR           FIO2DIR
#define BUT1_FIO            FIO2PIN

#define BUT2_MASK           (1UL<<21)
#define BUT2_FDIR           FIO2DIR
#define BUT2_FIO            FIO2PIN

// MMC/SD card switches
// Card present
#define MMC_CP_MASK         (1UL << 11)
#define MMC_CP_FDIR         FIO2DIR
#define MMC_CP_FIO          FIO2PIN
#define MMC_CP_MODE         PINMODE0_bit.P0_11

// Write protect
#define MMC_WP_MASK         (1UL << 19)
#define MMC_WP_FDIR         FIO4DIR
#define MMC_WP_FIO          FIO4PIN
#define MMC_WP_MODE         PINMODE9_bit.P4_19

// Analog trim
#define ANALOG_TRIM_CHANNEL 7
#define ANALOG_TRIM_CHANNEL_SEL   PINSEL0_bit.P0_13

//#else
//#error Define type of the board
//#endif

// PCLK offset
#define WDT_PCLK_OFFSET     0
#define TIMER0_PCLK_OFFSET  2
#define TIMER1_PCLK_OFFSET  4
#define UART0_PCLK_OFFSET   6
#define UART1_PCLK_OFFSET   8
#define PWM0_PCLK_OFFSET    10
#define PWM1_PCLK_OFFSET    12
#define I2C0_PCLK_OFFSET    14
#define SPI_PCLK_OFFSET     16
#define RTC_PCLK_OFFSET     18
#define SSP1_PCLK_OFFSET    20
#define DAC_PCLK_OFFSET     22
#define ADC_PCLK_OFFSET     24
#define CAN1_PCLK_OFFSET    26
#define CAN2_PCLK_OFFSET    28
#define ACF_PCLK_OFFSET     30
#define BAT_RAM_PCLK_OFFSET 32
#define GPIO_PCLK_OFFSET    34
#define PCB_PCLK_OFFSET     36
#define I2C1_PCLK_OFFSET    38
//#define                   40
#define SSP0_PCLK_OFFSET    42
#define TIMER2_PCLK_OFFSET  44
#define TIMER3_PCLK_OFFSET  46
#define UART2_PCLK_OFFSET   48
#define UART3_PCLK_OFFSET   50
#define I2C2_PCLK_OFFSET    52
#define I2S_PCLK_OFFSET     54
#define MCI_PCLK_OFFSET     56
//#define                   58
#define PCLK_PCLK_OFFSET    60
//#define                   62

#endif /* __BOARD_H */
