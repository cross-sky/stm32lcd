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
					break;
	case BTN_LOCK: MenuParam.lockFlag ^= 1;break;//need to add lock ico
	default: break;
	}
	//lcd_wr_char(_lcd11_waterT, 1);
	//lcd_wr_char(_lcd13_c, 1);
	lcd_wr_char(_lcd4_lock, MenuParam.lockFlag);//lock
	fClockOn(MenuParam.clock.hour, MenuParam.clock.min);
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
	j = Id%CoreParamsMax;
	k = NumCoreParam[Id].value;

	lcd_wr_num(0, 3, 1);
	lcd_wr_num(i, 4, 1);
	lcd_wr_num(j, 5, 1);

	if (k >= 0)
	{
		m = k / 100;
		i = k/10;
		j = k % 10;
		lcd_wr_num(m, 0, 1);
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

