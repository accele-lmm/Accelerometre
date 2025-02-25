/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : i2c0_drv.c
 *    Description : I2C0 driver
 *
 *    History :
 *    1. Date        : 13, February 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 18137 $
 **************************************************************************/
#include "i2c0_drv.h"
#include "smb380_drv.h"
#include <stdio.h>
/*#define FORMAT_8BIT 8
#define FORMAT_16BIT 16
#define FORMAT_32BIT 32
#define BIT_22 22
#define BIT_23 23
#define BIT_24 24
#define BIT_25 25

#define VALUE_0 0

#define VALUE_1 1
*/

volatile int	  I2CState;
LPC_I2C_Msg_t	  I2CMsg;


//#pragma inline
void __I2C_SetFlag (int FlagType)
{
	I2C0CONSET |= FlagType;
}

/*************************************************************************
 * Function Name: I2C_EnableI2C
 * Parameters: void
 * Return: void
 * Description: Enable I2C device.
 *************************************************************************/
//#pragma inline
void I2C_EnableI2C(void)
{
	__I2C_SetFlag(I2CON_I2EN);
	//I2C0CONSET |= I2CON_I2EN;
}

/*************************************************************************
 * Function Name: I2C_DisableI2C
 * Parameters: void
 * Return: void
 * Description: Disable I2C device.
 *************************************************************************/
//#pragma inline
void I2C_DisableI2C(void)
{
	I2C0CONCLR = I2CON_I2ENC;
}

/*************************************************************************
 * Function Name: __I2C_SetFlag
 * Parameters: int FlagType -- AA, INT and START and STOP
 *
 * Return: void
 * Description: Set the flag.
 *
 *************************************************************************/


/*************************************************************************
 * Function Name: __I2C_ClearFlag
 * Parameters: int FlagType -- AA, INT and START (Excl. STOP)
 *
 * Return: void
 * Description: Clear the flag.
 *
 *************************************************************************/
//#pragma inline
void __I2C_ClearFlag (int FlagType)
{
	I2C0CONCLR = FlagType;
}

/*************************************************************************
 * Function Name: __I2C_SendData
 * Parameters: unsigned char data
 * Return: void
 * Description: Load data to I2CDAT. Just used in I2C module.
 *************************************************************************/
//#pragma inline
void __I2C_SendData(unsigned char data)
{		
	I2C0DAT = data;
}

/*************************************************************************
 * Function Name: __I2C_ReceiveData
 * Parameters: void
 * Return: unsigned char
 * Description: Load data from I2CDAT. Just used in I2C module.
 *************************************************************************/
//#pragma inline
unsigned char __I2C_ReceiveData(void)
{
	return I2C0DAT;
}

/*************************************************************************
 * Function Name: I2C_InitMaster
 * Parameters: unsigned long BusSpeed
 *
 * Return: int
 *             	0: success
 *	 non-zero: error number
 * Description: Initialize the current device as I2C bus master.
 *
 *************************************************************************/
int I2C_InitMaster (unsigned long BusSpeed)
{

	// MODIF
	// Affectation directe du bit PCI2C0 bit 7 registre PCONP
	//PCONP_bit.PCI2C0 = 1;      // enable I2C0 clock
	SetBitValue2Register(7, 1, &PCONP, FORMAT_32BIT);
	
	if (BusSpeed > I2C_MAXSPEED)
		return 1;
	
	I2C_DisableI2C();
	
	// R�glage de la fr�quence d'horloge � CCLK
	SetBitValue2Register(I2C0_PCLK_OFFSET, 1, &PCLKSEL0, FORMAT_32BIT);	
	SetBitValue2Register(I2C0_PCLK_OFFSET + 1, 0, &PCLKSEL0, FORMAT_32BIT);
	
	// Bit Frequency = Fplk / (I2C_I2SCLH + I2C_I2SCLL)
	I2C0SCLH = ((SYS_GetFpclk(I2C0_PCLK_OFFSET) / BusSpeed) / 2) + 1;
	I2C0SCLL =  (SYS_GetFpclk(I2C0_PCLK_OFFSET) / BusSpeed) / 2;
	
	if (I2C0SCLH < 4 || I2C0SCLL < 4)
		return 1;
	
	I2CState = I2C_IDLE;
	
	// PCLK_SEL0 : PCLK_I2C0 = 01 (CCLK)
	SetBitValue2Register(14, 0, &PCLKSEL0, FORMAT_32BIT);
	SetBitValue2Register(15, 0, &PCLKSEL0, FORMAT_32BIT);
	
	// MODIF
	// Affectation directe du bit P0_27 bit 23:22=01 registre PINSEL1
	// Affectation directe du bit P0_28 bit 25:24=01 registre PINSEL1
	// soit bit 24 et 22 = 1, 23 et 25 = 0
	SetBitValue2Register(22, 1, &PINSEL1, FORMAT_32BIT);
	SetBitValue2Register(23, 0, &PINSEL1, FORMAT_32BIT);
	SetBitValue2Register(24, 1, &PINSEL1, FORMAT_32BIT);
	SetBitValue2Register(25, 0, &PINSEL1, FORMAT_32BIT);
	
	// Activation des interruptions
	VICIntEnable = 0xFFFFFFFF;
	
	// Enable I2C
	I2C_EnableI2C();
	__I2C_ClearFlag(I2CON_STAC | I2CON_SIC | I2CON_AAC);
	
	/*
	// Entering in master mode
	
	__I2C_SetFlag(I2CON_STA);
	
	printf("Attente de la condition START...\n");	
	while(I2C0STAT != 0x08) {}	
	
	__I2C_ClearFlag(I2CON_SIC);
	*/
	return 0;
}

/*************************************************************************
 * Function Name: I2C_MasterWrite
 * Parameters:  unsigned char addr -- the slave address which you send message to
 *		unsigned char *pMsg -- the point to the message
 *		unsigned long numMsg -- the byte number of the message
 * Return: int
 *             	0: success
 *       non-zero: error number
 *
 * Description: Transmit messages
 *
 *************************************************************************/
int I2C_MasterWrite (unsigned char addr, unsigned char *pMsg , unsigned long numMsg)
{
	return I2C_Transfer (addr, pMsg , numMsg, WRITE, 0);
}


/*************************************************************************
 * Function Name: I2C_MasterRead
 * Parameters:  unsigned char addr -- the slave address which you send message to
 *		unsigned char *pMsg -- the point to the message
 *		unsigned long numMsg -- the byte number of the message
 * Return: int
 *             	0: success
 *	 non-zero: error number
 *
 * Description: Receive messages
 *
 *************************************************************************/
int I2C_MasterRead (unsigned char addr, unsigned char *pMsg , unsigned long numMsg)
{
	return I2C_Transfer (addr, pMsg , numMsg, READ, 0);
}

/*************************************************************************
 * Function Name: I2C_Transfer
 * Parameters:  unsigned char addr -- the slave address which you send message to
 *		unsigned char *pMsg -- the point to the message
 *		unsigned long numMsg -- the byte number of the message
 *		LPC_I2C_TransMode_t transMode -- Read, Write, Write then read
 *		unsigned long numWrite -- this is only for "Write then read" mode
 *			
 * Return: int
 *             	0: success
 *       non-zero: error number
 *
 * Description: Transfer messages
 *
 *************************************************************************/
int I2C_Transfer (unsigned char addr, unsigned char *pMsg , unsigned long numMsg,  LPC_I2C_TransMode_t transMode, unsigned long numWrite)
{
	unsigned int timeout = DLY_I2C_TIME_OUT;
	if (transMode == WRITETHENREAD)
	{
		if (numWrite >= numMsg)
			return 1;
		else
			I2CMsg.nrWriteBytes = numWrite;
	}
	else
		I2CMsg.nrWriteBytes = 0;
	
	I2CMsg.nrBytes		= numMsg;
	I2CMsg.address		= addr;
	I2CMsg.transMode	= transMode;	
	I2CMsg.buf			= pMsg;
	I2CMsg.dataCount	= 0;
	
	I2CState = I2C_BUSY;	
	
	__I2C_SetFlag(I2CON_STA);
	
	while(I2CState == I2C_BUSY)
	{
		// Wait the interrupt
		if (I2C0CONSET & 0x08)
		{
			//__I2C_ClearFlag(I2CON_SIC);
			I2C_HandleMasterState();    // Master Mode
			timeout = DLY_I2C_TIME_OUT;
		}
		if (timeout-- == 0)
		{
			I2CState = I2C_TIME_OUT;
			printf("Tentative de transmission : timeout\n");
		}
	}
	
#ifdef __DEBUG__
	if (I2C_OK != I2CState) {
		printf("I2C error : %d\n",I2CState);
		I2C_InitMaster(I2C_SPEED);
	}
#endif
	__I2C_ClearFlag(I2CON_STAC | I2CON_AAC);
	
	return I2CState;
}


/*************************************************************************
 * Function Name: I2C_HandleMasterState
 * Parameters: void
 * Return: void
 * Description: Master mode state handler for I2C bus.
 *
 *************************************************************************/
void I2C_HandleMasterState(void)
{
	unsigned long I2Cstatus = I2C0STAT & 0xF8;
	
	switch (I2Cstatus)
	{
			/* ------- MASTER TRANSMITTER FUNCTIONS ----------- */
		case 0x08 : // start condition transmitted
		case 0x10 : // restart condition transmitted
			if (I2CMsg.transMode == WRITETHENREAD)
			{
				if (I2CMsg.dataCount < I2CMsg.nrWriteBytes)
					__I2C_SendData((I2CMsg.address<<1) | WRITE);
				else
				{
					__I2C_SendData((I2CMsg.address<<1) | READ );
				}
			}
			else
			{
				__I2C_SendData((I2CMsg.address<<1) | I2CMsg.transMode);
			}
			__I2C_ClearFlag(I2CON_STAC);	// STA bit
			break;
		case 0x18 : // SLA+W transmitted and ACK received
			if ( I2CMsg.nrBytes == 0 )
			{
				__I2C_SetFlag  (I2CON_STO);             // STO bit clear automatically
				I2CState = I2C_NO_DATA;
			}
			else
			{
				// Send next data byte
				__I2C_SendData(I2CMsg.buf[I2CMsg.dataCount++]);	
			}
			break;	
		case 0x20 : // SLA+W transmitted, but no ACK received
			__I2C_SetFlag  (I2CON_STO);
			I2CState = I2C_NACK_ON_ADDRESS;
			break;		
		case 0x28 : //Data byte transmitted and ACK received
			if (I2CMsg.transMode == WRITE)
			{
				if (I2CMsg.dataCount < I2CMsg.nrBytes)
				{
					__I2C_SendData (I2CMsg.buf[I2CMsg.dataCount++]);	
				}
				else
				{
					__I2C_SetFlag  (I2CON_STO); // data transmit finished, stop transmit
					I2CState = I2C_OK;
				}
			}
			else if (I2CMsg.transMode == WRITETHENREAD)
			{
				if (I2CMsg.dataCount < I2CMsg.nrWriteBytes)
				{
					__I2C_SendData (I2CMsg.buf[I2CMsg.dataCount++]);	
				}
				else		// send start condition
				{
					__I2C_SetFlag  (I2CON_STA);
				}
			}
			break;
		case 0x30 : // no ACK for data byte
			__I2C_SetFlag  (I2CON_STO);
			I2CState = I2C_NACK_ON_DATA;
			break;
		case 0x38 : // arbitration lost in Slave Address or Data bytes
			I2CState = I2C_ARBITRATION_LOST;
			break;
			
			/* ------- MASTER RECEIVER FUNCTIONS ----------- */
		case 0x40 : // ACK for slave address + R
			if (I2CMsg.nrBytes>1)
			{
				__I2C_SetFlag  (I2CON_AA );	// send acknowledge byte
			}
			else
			{
				__I2C_ClearFlag(I2CON_AAC); // return NACK
			}
			break;
		case 0x48 : // no ACK for slave address + R
			__I2C_SetFlag  (I2CON_STO);
			I2CState = I2C_NACK_ON_ADDRESS;
			break;
		case 0x50 : // ACK for data byte
			I2CMsg.buf[I2CMsg.dataCount++] = __I2C_ReceiveData();
			if (I2CMsg.dataCount + 1 < I2CMsg.nrBytes)
			{
				__I2C_SetFlag(I2CON_AA );	  // send acknowledge byte
			}
			else
			{
				__I2C_ClearFlag(I2CON_AAC); // return NACK
			}
			break;
		case 0x58 : // no ACK for data byte
			I2CMsg.buf[I2CMsg.dataCount++] = __I2C_ReceiveData();
			
			__I2C_SetFlag  (I2CON_STO);
			I2CState = I2C_OK;
			break;
		default : // undefined error
			printf("undefined error\n");
			__I2C_SetFlag  (I2CON_STO);
			__I2C_ClearFlag(I2CON_STAC | I2CON_AAC);
			I2CState = I2C_ERROR;
			break;
	} // switch - I2C_I2STAT & 0xF8
	__I2C_ClearFlag(I2CON_SIC);
}
