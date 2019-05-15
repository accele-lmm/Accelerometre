/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : smb380_drv.c
 *    Description : SMB380 acceleration sensor driver (I2C data mode)
 *
 *    History :
 *    1. Date        : 13, February 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 **************************************************************************/


#include "smb380_drv.h"
#include "i2c0_drv.h"
#include "stdio.h"
/*************************************************************************
 * Function Name: SMB380_Init
 * Parameters: none
 *
 * Return: SMB380_Status_t
 *
 * Description: SMB380 init
 *
 *************************************************************************/
SMB380_Status_t SMB380_Init(void)
{
	unsigned char buf[2], value;
	
	// Init I2C module as master
	I2C_InitMaster(I2C_SPEED);
	
	// Lecture du registre 0x15
	buf[0] = 0x15; 	
	I2C_MasterWrite(SMB380_ADDR, buf, 1);	
	I2C_MasterRead(SMB380_ADDR, &value, 1);
	
	// Modification : shadow_dis = 1
	value |= 0x08;	
	
	// Stockage
	buf[0] = 0x15;
	buf[1] = value;
	
	I2C_MasterWrite(SMB380_ADDR, buf, 2);
	
	buf[0] = 0x14; 	
	I2C_MasterWrite(SMB380_ADDR, buf, 1);
	I2C_MasterRead(SMB380_ADDR, &value, 1);
	
	// bandwidth = 100hz, full scale range = +- 2g
	value &= 0xe0;
	value |= 0x02;
	
	buf[0] = 0x14;
	buf[1] = value;
	
	I2C_MasterWrite(SMB380_ADDR, buf, 2);
	
	return SMB380_PASS;
}
/*************************************************************************
 * Function Name: SMB380_GetID
 * Parameters: none
 *
 * Return: SMB380_Status_t
 *
 * Description: SMB380 get chip ID and revision
 *
 *************************************************************************/
SMB380_Status_t SMB380_GetID (pInt8U *pChipId, pInt8U *pRevision)
{
	unsigned char buf[2] = {SMB380_CHIP_ID};
	
	//Write the address of Chip ID register
	I2C_MasterWrite(SMB380_ADDR, buf, 1);
	
	I2C_MasterRead(SMB380_ADDR, buf, 2);
	*pChipId = buf[0];
	*pRevision = buf[1];
	
	return SMB380_PASS;
}

/*************************************************************************
 * Function Name: SMB380_Init
 *
 * Parameters: pSMB380_Data_t : Structure de donnees
 *
 * Return: SMB380_Status_t
 *
 * Description: SMB380 init
 *
 *************************************************************************/
SMB380_Status_t SMB380_GetData (pSMB380_Data_t pData)
{
	unsigned char regaddr[4] = {SMB380_ACCX_ADDR, SMB380_ACCY_ADDR, SMB380_ACCZ_ADDR, SMB380_TEMP_ADDR};
	unsigned char buf;
	int i, value;
	
	for(i = 0; i < 4; i++) {		
		
		// ecriture de l'adresse du registre
		I2C_MasterWrite(SMB380_ADDR, &regaddr[i], 1);
		
		// Lecture de la valeur
		I2C_MasterRead(SMB380_ADDR, &buf, 1);
		
		// Interpretation de la valeur
		if(i != 3) 			
			value = convertValue(buf);
		else	
			value = -30 + 0.5 * buf;
		
		// Stockage de la valeur dans la structure de donnees
		switch(i) {
			case 0:				
				pData->AccX = (value>>2)<<2;
				break;
			case 1:
				pData->AccY = (value>>2)<<2;
				break;
			/*case 2:
				pData->AccZ = (value>>2)<<2;
				break;*/
			case 3:
				pData->Temp = value;
				break;		
			default:
				break;
		}
	}
	
	return SMB380_PASS;
}

/*************************************************************************
 * Function Name: convertValue
 *
 * Description : Converti le codage binaire complement a 2 de 8 bits a 32 bits
 *
 * Parameters: unsigned char : octet recu
 *
 * Return: int : valeur convertie
 *
 * Description: SMB380 init
 *
 *************************************************************************/
int convertValue(unsigned char buf) {
	
	int value;
	
	value = buf & 0x7f;	
	
	if(buf & 0x80)
		value -= 128;
	
	return value;
}


