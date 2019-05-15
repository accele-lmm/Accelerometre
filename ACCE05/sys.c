/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : sys.c
 *    Description : System module
 *
 *    History :
 *    1. Date        : 13, February 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 18137 $
 **************************************************************************/
#include "sys.h"
#include "smb380_drv.h"
#include <stdio.h>

/*************************************************************************
 * Function Name: complement
 * Parameters:	int : valeur
 *				int : format (8 bits, 16 bits ou 32 bits)
 *
 * Return: int : valeur complémentée
 *
 * Description: FIQ handler
 *
 *************************************************************************/
int complement(int value,int format) {
	
	int compVal = 0 ;
	
	if (format == FORMAT_8BIT)
		compVal = 255 - value;
	
	else if (format == FORMAT_16BIT)
		compVal = 65535 - value;
	
	else if (format == FORMAT_32BIT)
		compVal = 4294967295 - value;	
	
	return compVal ;
}


/*************************************************************************
 * Function Name: SetBitValue2Register
 * Parameters:	int bit : numéro du bit à modifié (le premier est 0)
 *				int value : valeur (0 ou 1)
 *				volatile unsigned long *reg : Registre à modifier
 *				int format : format (8 bits, 16 bits ou 32 bits)
 *
 * Return: int : valeur complémentée
 *
 * Description: FIQ handler
 *
 *************************************************************************/
void SetBitValue2Register(int bit, int value, volatile unsigned long *reg,int format) {
	
	if (value == 0)
		*reg  &= complement ((1 << bit),format);
	else
  		*reg  |= (1 << bit);
	
	return;
}

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
 void __attribute__((interrupt("FIQ"))) FIQ_handler()
 #else
 __fiq __arm void FIQ_handler(void)
 #endif
 
 //__fiq __arm void FIQ_Handler (void)
 {
 while(1);
 }
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
 __attribute__((interrupt("IRQ")))
 #else
 __irq __arm
 #endif
 void IRQ_Handler(void)
 //__irq __arm void IRQ_Handler (void)
 {
 void (*interrupt_function)();
 unsigned int vector;
 
 vector = (unsigned int) VICADDRESS;     // Get interrupt vector.
 interrupt_function = (void(*)())vector;
 if(interrupt_function != NULL)
 {
 interrupt_function();  // Call vectored interrupt function.
 }
 else
 {
 VICADDRESS = 0;      // Clear interrupt in VIC.
 }
 }
 */

/*************************************************************************
 * Function Name: VIC_Init
 * Parameters: void
 * Return: void
 *
 * Description: Initialize VIC
 *
 *************************************************************************/
void VIC_Init(void)
{
	volatile unsigned long * pVecAdd, *pVecCntl;
	int i;
	// Assign all interrupt channels to IRQ
	VICINTSELECT  =  0;
	// Disable all interrupts
	VICINTENCLEAR = 0xFFFFFFFF;
	// Clear all software interrupts
	VICSOFTINTCLEAR = 0xFFFFFFFF;
	// VIC registers can be accessed in User or privileged mode
	VICPROTECTION = 0;
	// Clear interrupt
	VICADDRESS = 0;
	
	// Clear address of the Interrupt Service routine (ISR) for vectored IRQs
	// and disable all vectored IRQ slots
	for(i = 0,  pVecCntl = &VICVECTPRIORITY0, pVecAdd = &VICVECTADDR0; i < 32; ++i)
	{
		*pVecCntl++ = *pVecAdd++ = 0;
	}
}

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
                        unsigned int VicIntSource)
{
	unsigned long volatile *pReg;
	// load base address of vectored address registers
	pReg = &VICVECTADDR0;
	// Set Address of callback function to corresponding Slot
	*(pReg+VicIntSource) = (unsigned long)pIRQSub;
	// load base address of ctrl registers
	pReg = &VICVECTPRIORITY0;
	// Set source channel and enable the slot
	*(pReg+VicIntSource) = Priority;
	// Clear FIQ select bit
	VICINTSELECT &= ~(1<<VicIntSource);
}

/*************************************************************************
 * Function Name: InitClock
 * Parameters: void
 * Return: void
 *
 * Description: Initialize PLL and clocks' dividers. Hclk - 288MHz,
 * Cclk- 48MHz, Usbclk - 48MHz
 *
 *************************************************************************/
void InitClock(void)
{
	// 1. Init OSC
	// MODIF registre SCS Systeme control and status
	// OSCRANGE => bit4 = 0 ;
	// OSCEN => bit5 = 1
	SetBitValue2Register(4, 0, &SCS, FORMAT_32BIT);
	SetBitValue2Register(5, 1, &SCS, FORMAT_32BIT);
	
	// 2.  Wait for OSC ready
	// MODIF registre SCS Systeme control and status
	// OSCSTAT => bit6 = 1
	while((SCS & (1<<6)) == 0);
	
	// 3. Disconnect PLL 
	// MODIF registre PLLCON PLL Control register
	// PLLC => bit1 = 0
	SetBitValue2Register(1, 0, &PLLCON, FORMAT_8BIT);
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
	
	// 4. Disable PLL
	// MODIF registre PLLCON PLL Control register
	// PLLE => bit0 = 0
	SetBitValue2Register(0, 0, &PLLCON, FORMAT_8BIT);
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
	
	// 5. Select source clock for PLL
	// MODIF registre  CLKSRCSEL Clock Source Select Register
	//CLKSRCSEL = 1 ;
	SetBitValue2Register(0, 1, &CLKSRCSEL, FORMAT_8BIT);
	SetBitValue2Register(1, 0, &CLKSRCSEL, FORMAT_8BIT);
	
	// 6. Set PLL settings 288 MHz
	// MODIF registre  PLLCFG PLL Config Register
	//PLLCFG_bit.MSEL = 24-1; bit[14:0]
	//PLLCFG_bit.NSEL = 2-1; bit[26:16]
	//PLLCFG = (24-1) + ((2-1)<<16);
	SetBitValue2Register(0, 1, &PLLCFG, FORMAT_32BIT);
	SetBitValue2Register(1, 1, &PLLCFG, FORMAT_32BIT);
	SetBitValue2Register(2, 1, &PLLCFG, FORMAT_32BIT);
	SetBitValue2Register(4, 1, &PLLCFG, FORMAT_32BIT);
	SetBitValue2Register(16, 1, &PLLCFG, FORMAT_32BIT);
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
	
	// 7. Enable PLL
	// MODIF registre PLLCON PLL Control register
	// PLLE => bit0 = 1
	SetBitValue2Register(0, 1, &PLLCON, FORMAT_8BIT);
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
	
	// 8. Wait for the PLL to achieve lock
	// MODIF registre PLLSTAT PLL Status Register
	// PLOCK => Bit26
	while((PLLSTAT & (1<<26)) == 0);
	
	// 9. Set clk divider settings
	CCLKCFG   = 4-1;				// 1/4 Fpll - 72 MHz
	USBCLKCFG = 6-1;			// 1/6 Fpll - 48 MHz
	PCLKSEL0 = PCLKSEL1 = 0;	// other peripherals - 18MHz (Fcclk/4)
	
	// 10. Connect the PLL
	// MODIF registre PLLCON PLL Control register
	// PLLC => bit25 = 1
	SetBitValue2Register(25, 1, &PLLCON, FORMAT_8BIT);
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
	
	// stop all Peripherals' clocks
	PCONP = 0;
}

/*************************************************************************
 * Function Name: SYS_GetFsclk
 * Parameters: none
 * Return: Int32U
 *
 * Description: return Sclk [Hz]
 *
 *************************************************************************/
Int32U SYS_GetFsclk(void)
{
	Int32U Mul = 1, Div = 1, Osc, Fsclk;
	// MODIF registre PLLCON PLL Control register
	// PLLC => bit25 = 1
	// if(PLLSTAT_bit.PLLC)
	if((PLLSTAT & (1<<25)) == 1)
	{
		// when PLL is connected
		// MODIF registre  PLLCFG PLL Config Register
		//Mul = PLLCFG_bit.MSEL + 1 ; bit[14:0]
		//Div = PLLCFG_bit.NSEL + 1; bit[26:16]
		Mul = (PLLSTAT & 0x7f) + 1;
		Div = ((PLLSTAT & 0x7ff) >> 16) + 1;
	}
	
	// Find clk source
	// MODIF registre  CLKSRCSEL Clock Source Select Register
	// CLKSRCSEL_bit.CLKSRC = 1; 
	// switch(CLKSRCSEL_bit.CLKSRC)
	switch(CLKSRCSEL)
	{
		case 0:
			Osc = (Int32U) I_RC_OSC_FREQ;
			break;
		case 1:
			Osc = (Int32U) MAIN_OSC_FREQ;
			break;
		case 2:
			Osc = (Int32U) RTC_OSC_FREQ;
			break;
		default:
			Osc = 0;
	}
	// Calculate system frequency
	Fsclk = Osc*Mul*2;
	Fsclk /= Div*(CCLKCFG+1);
	return(Fsclk);
}

/*************************************************************************
 * Function Name: SYS_GetFpclk
 * Parameters: Int32U Periphery
 * Return: Int32U
 *
 * Description: return Pclk [Hz]
 *
 *************************************************************************/
Int32U SYS_GetFpclk(Int32U Periphery)
{
	Int32U Fpclk;
	pInt32U pReg = (pInt32U)((Periphery < 32) ? PCLKSEL0 : PCLKSEL1);
	
	
	Periphery  &= 0x1F;   // %32
	Fpclk = SYS_GetFsclk();
	
	// find peripheral appropriate periphery divider
	// MODIF
	//switch((*pReg >> Periphery) & 3)
	
	switch((pReg >> Periphery) & 3)
	{
		case 0:
			Fpclk /= 4;
			break;
		case 1:
			break;
		case 2:
			Fpclk /= 2;
			break;
		default:
			Fpclk /= 8;
	}
	
	return(Fpclk);
}

/*************************************************************************
 * Function Name: GpioInit
 * Parameters: void
 * Return: void
 *
 * Description: Reset all GPIO pins to default: primary function
 *
 *************************************************************************/
void GpioInit(void)
{
	// Set to inputs
	IO0DIR  = 0;
	IO1DIR  = 0;
	FIO0DIR = 0;
	FIO1DIR = 0;
	FIO2DIR = 0;
	FIO3DIR = 0;
	FIO4DIR = 0;
	
	// Enable Fast GPIO0,1
	// MODIF registre SCS Systeme control and status
	// GPIOM => bit0 = 1 ;
	// SCS_bit.GPIOM = 1;
	//SCS |= 0x1;	
	SetBitValue2Register(0, 1, &SCS, FORMAT_32BIT);
	
	// clear mask registers
	FIO0MASK = 0;
	FIO1MASK = 0;
	FIO2MASK = 0;
	FIO3MASK = 0;
	FIO4MASK = 0;
	
	// Reset all GPIO pins to default primary function
	PINSEL0 = 0;
	PINSEL1 = 0;
	PINSEL2 = 0;
	PINSEL3 = 0;
	PINSEL4 = 0;
	PINSEL5 = 0;
	PINSEL6 = 0;
	PINSEL7 = 0;
	PINSEL8 = 0;
	PINSEL9 = 0;
	PINSEL10= 0;
}

