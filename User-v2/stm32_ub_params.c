#include "cominc.h"

stuLcdParams MenuParam;

stuCoreParam NumCoreParam[CoreParamsMax]={
	{0, 0, 50, 70, 30},
	{1, 0, 60, 80, 40}
};

void MenuParaInit(void)
{
	//	MenuParam.clock.hour = 10; alread set by pcf8560 Pcf8536RT
	//	MenuParam.clock.min = 10;
	MenuParam.clock.posHour= PosHour;
	MenuParam.clock.posMin = PosMin;
	MenuParam.clock.flag = 0;
	MenuParam.pfmenu = MenuOffStatus;
	//pfmenu = MenuOnStatus; // important

	MenuParam.timer.onHour = 10;
	MenuParam.timer.onMin = 10;
	MenuParam.timer.offHour = 20;
	MenuParam.timer.offMin = 20;
	MenuParam.timer.posHour = PosHour;
	MenuParam.timer.posMin = PosMin;
	MenuParam.timer.flag = 0;

	MenuParam.beepFlag = 1;//beep on 1, off 0
	MenuParam.bgledFlag = 1;//background led on 1, off 0
	
	MenuParam.lockFlag = 0;//lock on 1, off 0

	//MenuParam.CoreParam[0] = {0, 0, 50, 70, 30};
	//MenuParam.CoreParam[1] = {0, 0, 60, 80, 40};

	MenuParam.elecFlag = 0;

	MenuParam.CoreParam = NumCoreParam;


	lcd_wr_char(_lcd11_waterT, 1);
	lcd_wr_char(_lcd13_c, 1);
}

