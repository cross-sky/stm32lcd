#include "cominc.h"

#define AT24CXX 0xa0
#define PAGE_SIZE 8
#define MEMORY_SIZE 256
#define PAGE_NUM 32
#define STARTPAMS 0xaf

char eeprom[20]={0};
char RdEeprom[20]={0};
char StartParamCheck[8]={STARTPAMS};
uint8_t renum[8]={0};
uint8_t ReqWaterT[6]={0xfd,0xba,0x01,0,0xca};

// 开关机 电加热 
//0xfe, 0xef, 0x02, 00, 00, 校验， 结束(0xca)
const uint8_t StartEep[2]={0xfe, 0xef};
const uint8_t Timertab[8]={0,1,0xcc,0,1,0xcc,0,1};

// 设置参数
//0xff,0xcc, 0x0d,13byte,校验， 结束(0xca)

//发送温度等查询
//0xfd,0xba,0x01,校验， 结束(0xca)

uint8_t eepRdCoreParam(void);
void RequestTempe(void);

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
//	uint8_t wrnum[6]={3,1,5};	
	uint8_t reg=0x00;
	uint8_t i,vadd=0;
	
	AT24CXX_ReadBuffer(AT24CXX, reg, renum, 7);
	for (i=0; i<6; i++)
	{
		vadd+=renum[i];
	}
	if (renum[i] != vadd)
	{
		//write defined number
		//WRStartParam(1);
		eepSaveTimer();
		//AT24CXX_WriteBuff(AT24CXX, reg, wrnum, 6);
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
		MenuParam.runFlag = 0;
		MenuParam.elecFlag = 0;
		MenuParam.timer.onHour=renum[2];
		MenuParam.timer.onMin=renum[3];
		MenuParam.timer.offHour=renum[4];
		MenuParam.timer.offMin=renum[5];
		i=eepRdCoreParam();	
		//eepRdCoreParam();
#ifdef Debug
		sprintf(eeprom, "%s","eep rd...\r\n");
		len = strlen(eeprom);
		UartDMAQueue(qUartLink,(uint8_t*)eeprom,len);
		//printf("eeprom success...");
#endif
	}

	//if ( MenuParam.runFlag == 0)
	//{
	//	lcd_wr_char(_lcd3_run, 0); 
	//	lcd_wr_char(_lcd10_hotWater, 0);
	//	MenuParam.pfmenu =MenuOffStatus;
	//	
	//}
	//else
	//{
	//	MenuParam.pfmenu = MenuOnStatus; 
	//	lcd_wr_char(_lcd3_run, 1);
	//	lcd_wr_char(_lcd10_hotWater, 1);
	//	//send start param
	//}

	//send check waterT
	CheckTimer(0);
	lcd_wr_char(_lcd6_err,i^0x01);
	
}

uint8_t eepRdCoreParam(void)
{
	uint8_t reg=0x08,i,j=0,vadd=0;
	uint8_t len=CoreParamsMax+3;
	//uint8_t len = CoreParamsMax<<1;
	AT24CXX_ReadBuffer(AT24CXX, reg, (uint8_t*)&RdEeprom[3], CoreParamsMax+1);//############
	RdEeprom[0]=0xff;
	RdEeprom[1]=0xcc;//start(2) + 1 +13 + check(1)+ end(1) = 18
	RdEeprom[2]=CoreParamsMax;

	for(i=3; i<len; i++)
	{
		//high level ffront, low level back
		NumCoreParam[j].value = (int8_t)RdEeprom[i]; 
		vadd += RdEeprom[i];
		j++;
	}
	if (RdEeprom[i] != vadd)
	{
		return FALSE;
	}
	RdEeprom[i] = vadd;
	RdEeprom[17] = 0xca;
	UartDMAQueue(qUartLink,(uint8_t*)RdEeprom,18);//send i+1 13+5
	return TRUE;
}

uint8_t eepWRCoreParam(void)
{
	//save and send
	uint8_t checkPams[20]={0};
	uint8_t reg=0x08,i,j=0,vadd=0;
	uint8_t len=CoreParamsMax+3;
	for (i=3; i<len; i++)
	{
		eeprom[i] =  (uint8_t)NumCoreParam[j].value;
		vadd += eeprom[i];
		j++;
	}

	eeprom[0]=0xff;
	eeprom[1]=0xcc;
	eeprom[2]=CoreParamsMax;
	eeprom[i]=vadd;
	eeprom[17]=0xca;
	UartDMAQueue(qUartLink,(uint8_t*)eeprom,18);//send i+1 13+5

	AT24CXX_WriteBuff(AT24CXX, reg, (uint8_t*)&eeprom[3], CoreParamsMax+1);//########
	delay_ms(1);
	AT24CXX_ReadBuffer(AT24CXX, reg, (uint8_t*)&checkPams[3], CoreParamsMax+1);//############
	//len=CoreParamsMax+3;
	for(i=3;i<len;i++)
	{
		if (eeprom[i] != checkPams[i])
		{
			break;
		}
	}
	if (i!= len)
	{
		return FALSE;
	}
	else
		return TRUE;
}


void TaskSendStartParm(void)
{
	static uint8_t i=0;
	uint16_t ttime,tontime, tofftime;
	static uint8_t tcheck=0, tlastcheck=0;
	uint8_t k;
	i++;
	if (i>= 10)
	{
		i=0;
		RequestTempe();//10s 
		ttime = MenuParam.clock.hour<<6 | MenuParam.clock.min;
		tontime = MenuParam.timer.onHour<<6 | MenuParam.timer.onMin;
		tofftime = MenuParam.timer.offHour<<6 | MenuParam.timer.offMin;

		if (tontime == tofftime)
		{
			return ;
		}

		k = (tontime>tofftime)<<2 |(ttime > tofftime)<<1 | (ttime > tontime);
		tcheck = Timertab[k];

		if (tcheck != tlastcheck && tcheck!= 0xcc)
		{
			//switch
			if (MenuParam.pfmenu== MenuOffStatus || MenuParam.pfmenu== MenuOnStatus)
			{
				KeyPush(BTN_SHUT);
				
			}
			else
			{
				KeyPush(BTN_SHUT);
				KeyPush(BTN_SHUT);
			}
			MenuParam.beepFlag = 1;
		}

		tlastcheck = tcheck;


		//if (MenuParam.runFlag==0 && ttime > tontime)
		//{
		//	//tcheck = 1;
		//	MenuParam.StartParamChange = 1;
		//	MenuParam.runFlag ^= 0x01;
		//	lcd_wr_char(_lcd3_run, 0); 
		//	lcd_wr_char(_lcd10_hotWater, 0);
		//	MenuParam.pfmenu =MenuOffStatus;
		//}
		//if (MenuParam.runFlag==1 &&ttime > tofftime)
		//{
		//	//tcheck = 1;
		//	MenuParam.StartParamChange = 1;
		//	MenuParam.pfmenu = MenuOnStatus; 
		//	lcd_wr_char(_lcd3_run, 1);
		//	lcd_wr_char(_lcd10_hotWater, 1);
		//	MenuParam.runFlag ^= 0x01;
		//}

	}

	if (MenuParam.StartParamChange == 1 )
	{
		MenuParam.StartParamChange = 0;
		WRStartParam(1);
		//eepSaveTimer();
	}
}

void StartParamChecErr(void)
{
	MenuParam.runFlag=0;
	MenuParam.elecFlag=0;
	MenuParam.timer.onHour=16;
	MenuParam.timer.onMin=0;
	MenuParam.timer.offHour=2;
	MenuParam.timer.offMin=0;
}
void CheckTimer(uint8_t isWrite)
{
	uint16_t tontime, tofftime;
		uint8_t k;
	tontime = MenuParam.timer.onHour<<6 | MenuParam.timer.onMin;
	tofftime = MenuParam.timer.offHour<<6 | MenuParam.timer.offMin;

	if (tontime == tofftime)
		k=0;
	else
		k=1;
	lcd_wr_char(_lcd23_timeOn,k);
	lcd_wr_char(_lcd24_timeOff,k);
	if (isWrite > 0)
	{
		//WRStartParam(0);
		eepSaveTimer();
		//WRStartParam(1);
	}
	
}

void eepSaveTimer(void)
{
	uint8_t i,vadd=0;
	uint8_t reg=0;
	uint8_t rdPams[8]={0};
	uint8_t wrPams[8]={0};

	wrPams[0]= StartEep[0];
	wrPams[1]= StartEep[1];
	wrPams[2]= MenuParam.timer.onHour;
	wrPams[3]= MenuParam.timer.onMin;
	wrPams[4]= MenuParam.timer.offHour;
	wrPams[5]= MenuParam.timer.offMin;
	for (i=0;i<6; i++)
	{
		vadd += wrPams[i];
	}
	wrPams[6]=vadd;
	//write 
	AT24CXX_WriteBuff(AT24CXX, reg, (uint8_t*)&wrPams, 7);
	delay_ms(1);
	//read and check
	AT24CXX_ReadBuffer(AT24CXX, reg, (uint8_t*)&rdPams,7);
	for (i=0;i<7;i++)
	{
		if (wrPams[i] != rdPams[i])
		{
			break;
		}
	}

	if (i != 7)
	{
		i=0;

		// false then false do nothing
		//StartParamChecErr();
		//return ;
	}
}

void WRStartParam(uint8_t issend)
{
	uint8_t i,vadd=0;
//	uint8_t reg=0;
//	uint8_t rdPams[8];
	//StartParamCheck[0]= StartEep[0];
	//StartParamCheck[1]= StartEep[1];
	//StartParamCheck[2]= MenuParam.timer.onHour;
	//StartParamCheck[3]= MenuParam.timer.onMin;
	//StartParamCheck[4]= MenuParam.timer.offHour;
	//StartParamCheck[5]= MenuParam.timer.offMin;
	//for (i=0;i<6; i++)
	//{
	//	vadd += StartParamCheck[i];
	//}
	//StartParamCheck[6]=vadd;
	////write 
	//AT24CXX_WriteBuff(AT24CXX, reg, (uint8_t*)&StartParamCheck, 8);
	//delay_ms(1);
	////read and check
	//AT24CXX_ReadBuffer(AT24CXX, reg, (uint8_t*)&rdPams,8);

	//for (i=0;i<8;i++)
	//{
	//	if (StartParamCheck[i] != rdPams[i])
	//	{
	//		break;
	//	}
	//}

	//if (i != 8)
	//{
	//	i=0;
	//	
	//	// false then false do nothing
	//	//StartParamChecErr();
	//	//return ;
	//}

	if (issend > 0)
	{
		StartParamCheck[0]= StartEep[0];
		StartParamCheck[1]= StartEep[1];
		StartParamCheck[2]= 2;
		StartParamCheck[3]= MenuParam.runFlag;
		StartParamCheck[4]= MenuParam.elecFlag;
		for(i=0;i<5; i++)
		{
			vadd+=StartParamCheck[i];
		}
		StartParamCheck[5]= vadd;
		StartParamCheck[6]= 0xca;
		UartDMAQueue(qUartLink,(uint8_t*)StartParamCheck,7);
	}
}

void RequestTempe(void)
{
	//later than reqwater, the last bit set by seng flag,but not deal it
	//10 s
	UartDMAQueue(qUartLink,ReqWaterT,5);
}



