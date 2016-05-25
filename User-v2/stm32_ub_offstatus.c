#include "cominc.h"

void fSetCoreParam(void);


void MenuOffStatus(void)
{
	uint16_t keyValue=0;

	keyValue = KeyPop();
	switch(keyValue)
	{
	case BTN_SETCORE:	MenuParam.pfmenu = fSetCoreParam; lcd_wr_char(_lcd5_find,1);break;
	case BTN_SHUT: MenuParam.pfmenu = MenuOnStatus; 
					lcd_wr_char(_lcd3_run, 1);
					lcd_wr_char(_lcd10_hotWater, 1);
					MenuParam.runFlag ^= 0x01;
					MenuParam.StartParamChange = 1;
					break;
	case BTN_LOCK: MenuParam.lockFlag ^= 1;break;//need to add lock ico
	case BTN_VIEWRC: MenuParam.pfmenu = DisplayRecT; lcd_wr_char(_lcd5_find,1);break;
	case BTN_DEFROSTMANUL:
		{
			MenuParam.defrostManul=1;
			MenuParam.StartParamChange = 1;
			break;
		}
	default: break;
	}
	//lcd_wr_char(_lcd11_waterT, 1);
	//lcd_wr_char(_lcd13_c, 1);
	lcd_wr_char(_lcd4_lock, MenuParam.lockFlag);//lock
	lcd_wr_char(_lcd7_defrost, MenuParam.defrostManul);
	fClockOn(MenuParam.clock.hour, MenuParam.clock.min);
	DispLayWatetT();
	LcdSetWater(0);
}

void fSetCoreParam(void)
{
	//clear ico?
	uint16_t keyValue=0;
	static uint8_t Id=0;

	keyValue = KeyPop();
	switch(keyValue)
	{
	case BTN_UP: fAddCoreParam(Id); break;
	case BTN_DOWN:fSubCoreParam(Id); break;
	case BTN_CLOCK: Id++; 
					if (Id >= CoreParamsMax)
					{
						Id = 0;
					}
					break;
	case BTN_TIMER: Id--;
					if (Id >= 0xff)
					{
						Id = CoreParamsMax-1;
					}
					break;
	case BTN_SHUT: MenuParam.pfmenu = MenuOffStatus;  Id=0; lcd_wr_char(_lcd5_find,0);return;
	case BTN_LOCK:	MenuParam.lockFlag ^= 1;break;//need to add lock ico
	default: break;
	}
	lcd_wr_char(_lcd4_lock, MenuParam.lockFlag);
	fOffStatusPara(Id);
}

void fOffStatusPara(uint8_t Id)
{
	uint8_t i,j,m;
	int16_t k;
	i = Id/10;
	j = Id%10;
	k = NumCoreParam[Id].value;

	lcd_wr_num(0, 3, 1);
	lcd_wr_num(i, 4, 1);
	lcd_wr_num(j, 5, 1);

	if (k >= 0)
	{
		m = k / 100;
		i = k/10;
		j = k % 10;
		lcd_wr_num(m, 0, m);
		lcd_wr_num(i, 1, 1);
		lcd_wr_num(j, 2, 1);
	} 
	else
	{
		k = 0 - k;
		i = k/10;
		j = k % 10;
		lcd_wr_num(10, 0, 1);
		lcd_wr_num(i, 1, 1);
		lcd_wr_num(j, 2, 1);
	}
}



void ErrorDisplay(void)
{
	uint16_t keyValue=0;
	uint8_t i,err=0;
	static uint8_t FirstInFlag=1;

	if (FirstInFlag == 1)
	{
		FirstInFlag = 0;
		lcd_wr_char(_lcd3_run, 0); 
		lcd_wr_char(_lcd10_hotWater, 0);
		
	}

	//暂时只有7个温度 1台压缩机
	for (i=0; i<7; i++)
	{
		//0 是正常状态
		//@@@@@初始时全是0 如果显示器断开则不显示断开错误
		if (RECWatreT[i].errFlag != 0)
		{
			err=1;
			break;
		}
	}

	if (RECWatreT[ADC_OUTLINE].errFlag == OutLine)
	{
		err=1;
	}

	//全部是正常，进入关机状态@@@@@@@@，应该回到错误之前的状态
	if (i==7 && err==0)
	{
		MenuParam.runFlag = 0;
		MenuParam.StartParamChange=1;
		MenuParam.pfmenu =MenuOffStatus; 
		FirstInFlag=0;
		return;
	}

	keyValue = KeyPop();
	switch(keyValue)
	{
	case BTN_SHUT:	MenuParam.runFlag = 0;
					MenuParam.StartParamChange=1;
					MenuParam.pfmenu =MenuOffStatus; 
					//clear errors then shutdown
					FirstInFlag=0;
					return;
					//break;
	case BTN_LOCK: MenuParam.lockFlag ^= 1;break;//need to add lock ico
	default: break;
	}
	if (err == 1)
	{
		lcd_wr_num(11, 0, 1);
		lcd_wr_num(i, 1, 1);
		lcd_wr_num(RECWatreT[i].errFlag, 2, 1);
	}
	fClockOn(MenuParam.clock.hour, MenuParam.clock.min);
	LcdSetWater(0);
	lcd_wr_char(_lcd4_lock, MenuParam.lockFlag);//lock
}

void DispLayWatetT(void)
{
	uint8_t i,j;
	uint16_t k;
	//0 蒸发器温度 1-环境温度 2-水箱温度 3-排气温度 4-回气温度
	k= RECWatreT[ADC07_WTANK].Value/10;
	i= k/10;
	j= k%10;
	lcd_wr_num(0, 0, 0);
	lcd_wr_num(i, 1, 1);
	lcd_wr_num(j, 2, 1);
}

