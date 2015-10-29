#include "cominc.h"

#define AT24CXX 0xa0
#define PAGE_SIZE 8
#define MEMORY_SIZE 256
#define PAGE_NUM 32

char eeprom[20];


bool eepRdCoreParam(void);


void AT24CXX_WriteBuff(uint8_t dev, uint8_t reg, uint8_t *Buffer, uint16_t Length)
{
	uint16_t counter;
	uint8_t FirstPageLeft = PAGE_SIZE - reg % PAGE_SIZE;
	uint8_t LastPageUsed;
	uint16_t FullPageNums;

	if (Length <= FirstPageLeft)
	{
		LastPageUsed = 0;
		FullPageNums = 0;
	}
	else
	{		
		LastPageUsed = (reg + Length) % PAGE_SIZE;
		
		if (Length <= FirstPageLeft + PAGE_SIZE)
		{
			FullPageNums = 0;
		}
		else
		{
			FullPageNums = (Length - FirstPageLeft - LastPageUsed) / PAGE_SIZE;
		}
	}
	/*
	if (Length <= FirstPageLeft)
	{ 
		FullPageNums = (Length - FirstPageLeft - LastPageUsed) / PAGE_SIZE;
	}
	else
		FullPageNums = 0;

	*/
	if (FullPageNums > 0)
	{
		if (FirstPageLeft > 0)
		{
			IICWriteBytes(dev, reg, FirstPageLeft,Buffer);
		}
		delay_ms(1);
		for (counter=0; counter<FullPageNums; counter++)
		{
			IICWriteBytes(dev, reg + FirstPageLeft + counter * PAGE_SIZE,
				PAGE_SIZE, 
				Buffer + FirstPageLeft + counter * PAGE_SIZE);
			delay_ms(1);
		}
		
		if (LastPageUsed > 0)
		{
			IICWriteBytes(dev,reg + FirstPageLeft + FullPageNums * PAGE_SIZE,
				PAGE_SIZE,
				Buffer + FirstPageLeft + FullPageNums * PAGE_SIZE);
			delay_ms(1);
		}
	}
	else
	{
		if (Length <= FirstPageLeft)
		{
			IICWriteBytes(dev,reg,Length,Buffer);
		}
		else
		{
			if (Length <= FirstPageLeft + LastPageUsed)
			{
				IICWriteBytes(dev,reg,FirstPageLeft,Buffer);
				delay_ms(1);
				IICWriteBytes(dev, reg + FirstPageLeft, LastPageUsed, Buffer+FirstPageLeft);
			}
		}
		delay_ms(1);
	}
}

void AT24CXX_ReadBuffer(uint8_t dev, uint8_t reg, uint8_t *Buffer, uint16_t Length)
{
	uint16_t counter;
	uint8_t FirstPageLeft = PAGE_SIZE - reg%PAGE_SIZE;
	uint8_t LastPageUsed;
	uint16_t FullPageNums;

	if (Length <= FirstPageLeft)
	{
		LastPageUsed = 0;
		FullPageNums = 0;
	}
	else
	{
		LastPageUsed = (reg + Length) % PAGE_SIZE;
		
		if (Length <= FirstPageLeft + PAGE_SIZE)
		{
			FullPageNums = 0;
		}
		else
		{
			FullPageNums = (Length - FirstPageLeft - LastPageUsed) / PAGE_SIZE;
		}
	}

	if (FullPageNums > 0)
	{
		if (FirstPageLeft > 0)
		{
			IICReadBytes(dev,reg,FirstPageLeft,Buffer);
		}
		delay_ms(1);
		for (counter=0; counter<FullPageNums; counter++)
		{
			IICReadBytes(dev, reg + FirstPageLeft + counter * PAGE_SIZE,
				PAGE_SIZE, 
				Buffer + FirstPageLeft + counter * PAGE_SIZE);
			delay_ms(1);
		}

		if (LastPageUsed > 0)
		{
			IICReadBytes(dev, reg + FirstPageLeft + FullPageNums * PAGE_SIZE,
				PAGE_SIZE,
				Buffer + FirstPageLeft + FullPageNums * PAGE_SIZE);
			delay_ms(1);
		}

	}
	else
	{
		if (Length <= FirstPageLeft)
		{
			IICReadBytes(dev,reg,Length,Buffer);
		}
		else
		{
			if (Length <= FirstPageLeft + LastPageUsed)
			{
				IICReadBytes(dev,reg,FirstPageLeft,Buffer);
				delay_ms(1);
				IICReadBytes(dev, reg+FirstPageLeft, LastPageUsed, Buffer+FirstPageLeft);
			}
		}
		delay_ms(1);
	}
}

void AT24CxxTest(void)
{
	uint8_t len;
	uint8_t wrnum[6]={3,2,5};
	uint8_t renum[6]={0};
	uint8_t reg=0x00;
	uint8_t i;
	
	AT24CXX_ReadBuffer(AT24CXX, reg, renum, 6);
	for (i=0; i<6; i++)
	{
		if (wrnum[i] != renum[i])
		{
			break;
		}
	}

	if (i != 6)
	{
		//write defined number
		AT24CXX_WriteBuff(AT24CXX, reg, wrnum, 6);
		i = eepWRCoreParam();
//#ifdef Debug
//		sprintf(eeprom, "%s","eep wr..\r\n");
//		len = strlen(eeprom);
//		UartDMAQueue(qUartLink,(uint8_t*)eeprom,len);
//		//printf("eeprom fail \r\n");
//#endif

	}
	else
	{
		// read saved number
		//i=0;
		i=eepRdCoreParam();	
		//eepRdCoreParam();
#ifdef Debug
		sprintf(eeprom, "%s","eep rd...\r\n");
		len = strlen(eeprom);
		UartDMAQueue(qUartLink,(uint8_t*)eeprom,len);
		//printf("eeprom success...");
#endif
	}

	lcd_wr_char(_lcd6_err,i);
	
}

bool eepRdCoreParam(void)
{
	uint8_t coPams[16]={0};
	uint8_t reg=0x08,i,vadd=0;
	//uint8_t len = CoreParamsMax<<1;
	AT24CXX_ReadBuffer(AT24CXX, reg, coPams, CoreParamsMax+1);
	for(i=0; i<CoreParamsMax; i++)
	{
		//high level ffront, low level back
		NumCoreParam[i].value = (int8_t)coPams[i]; 
		vadd += coPams[i];
	}
	if (coPams[i] != vadd)
	{
		return FALSE;
	}
	return TRUE;
}

bool eepWRCoreParam(void)
{
	//save and send
	uint8_t checkPams[20]={0};
	uint8_t reg=0x08,i,vadd=0;
	for (i=2; i<CoreParamsMax; i++)
	{
		eeprom[i] =  (uint8_t)NumCoreParam[i].value;
		vadd += eeprom[i];
	}

	eeprom[0]=0xff;
	eeprom[1]=0xcc;
	eeprom[i]=vadd;
	UartDMAQueue(qUartLink,(uint8_t*)eeprom,i+1);//send i+1 13+5

	AT24CXX_WriteBuff(AT24CXX, reg, eeprom, CoreParamsMax+1);
	AT24CXX_ReadBuffer(AT24CXX, reg, &checkPams[2], CoreParamsMax+1);
	reg = 0;
	for(i=2;i<CoreParamsMax;i++)
	{
		if (eeprom[i] != checkPams[i])
		{
			break;
		}
		reg += checkPams[i];
	}
	if (i!= CoreParamsMax || reg != vadd)
	{
		return FALSE;
	}
	return TRUE;
}

void eepSendParam(void)
{

}

//void eepWrOneParam(uint8_t id)
//{
//	uint8_t dat,reg;
//	dat = (uint8_t)NumCoreParam[id].value;
//	reg = 0x08 + id;
//	AT24CXX_WriteBuff(AT24CXX, reg, &dat, 1);
//}