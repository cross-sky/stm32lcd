#include "cominc.h"

#define AT24CXX 0xa0
#define PAGE_SIZE 8
#define MEMORY_SIZE 256
#define PAGE_NUM 32


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
		for (counter=0; counter<FullPageNums; counter++)
		{
			IICWriteBytes(dev, reg + FirstPageLeft + counter * PAGE_SIZE,
				PAGE_SIZE, 
				Buffer + FirstPageLeft + counter * PAGE_SIZE);
		}
		if (LastPageUsed > 0)
		{
			IICWriteBytes(dev,reg + FirstPageLeft + FullPageNums * PAGE_SIZE,
				PAGE_SIZE,
				Buffer + FirstPageLeft + FullPageNums * PAGE_SIZE);
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

				IICWriteBytes(dev, reg + FirstPageLeft, LastPageUsed, Buffer+FirstPageLeft);
			}
		}
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
		for (counter=0; counter<FullPageNums; counter++)
		{
			IICReadBytes(dev, reg + FirstPageLeft + counter * PAGE_SIZE,
				PAGE_SIZE, 
				Buffer + FirstPageLeft + counter * PAGE_SIZE);
		}
		if (LastPageUsed > 0)
		{
			IICReadBytes(dev, reg + FirstPageLeft + FullPageNums * PAGE_SIZE,
				PAGE_SIZE,
				Buffer + FirstPageLeft + FullPageNums * PAGE_SIZE);
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

				IICReadBytes(dev, reg+FirstPageLeft, LastPageUsed, Buffer+FirstPageLeft);
			}
		}
	}
}

void AT24CxxTest(void)
{
	uint8_t wrnum[6]={3};
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
		AT24CXX_WriteBuff(AT24CXX, reg, wrnum, 6);
		printf("eeprom fail \r\n");
	}
	else
		printf("eeprom success...");

}


