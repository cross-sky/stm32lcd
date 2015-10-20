#include "cominc.h"

stuLcdParams MenuParam;


void MenuParaInit(void)
{
	//	MenuParam.clock.hour = 10; alread set by pcf8560 Pcf8536RT
	//	MenuParam.clock.min = 10;
	MenuParam.clock.posHour= PosHour;
	MenuParam.clock.posMin = PosMin;
	MenuParam.clock.flag = 0;
	MenuParam.pfmenu = MenuOnStatus;
	//pfmenu = MenuOnStatus; // important

	MenuParam.timer.onHour = 10;
	MenuParam.timer.onMin = 10;
	MenuParam.timer.offHour = 20;
	MenuParam.timer.offMin = 20;
	MenuParam.timer.posHour = PosHour;
	MenuParam.timer.posMin = PosMin;
	MenuParam.timer.flag = 0;
}

