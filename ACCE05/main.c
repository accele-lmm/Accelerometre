#include <stdio.h>
#include "smb380_drv.h"
#include <unistd.h>
#include "fb_Graphic.h"
#include <time.h>
#include <math.h>



int main(void)
{
pSMB380_Data_t pData;
unsigned int xres, yres;
int i = 0 ;
char mess[30] ;


	printf("     ****************************************************\n");
	printf("     *                                                  *\n");	
	printf("     *                  Kit Olimex LPC2478 STK          *\n");
	printf("     *                                                  *\n");
	printf("     *                 SMB380_i2c_V1.1 Program          *\n");
	printf("     ****************************************************\n\n");

	SMB380_Init();

	while(1) 
		{
		SMB380_GetData(pData);
		printf("AccX = %d\n", pData->AccX);	
		printf("AccY = %d\n", pData->AccY);	
		printf("AccZ = %d\n", pData->AccZ);	
		printf("Temp = %d\n", pData->Temp);
		for (i=0;i<20;i++)
			usleep(1000) ;
		}

	
	return 0 ;
}


