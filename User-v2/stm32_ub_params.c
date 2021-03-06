#include "cominc.h"

stuLcdParams MenuParam;

stuCoreParam NumCoreParam[CoreParamsMax]={
	//id,flag value max min
	{0, 0, 50,	70,	30},	//设置水温
	{1, 0, 5,	15,		4},			//设置水温回差
	{2, 0, -7,  0, -30},	//进入除霜温度
	{3, 0, 8,	12,	 1},		//除霜时间
	{4, 0, 10,	15, 5},		//退出除霜温度
	{5, 0, 5,	8, 2},		//除霜周期	30分钟为倍数
	{6, 0, 3,	5, 0},		//除霜进入偏差	
	{7, 0, -7,  0, -30},
	{8, 0, 13,	30, 2},
	{9, 0, 8,	12, 1},
	{10, 0, 1,	1, 0},
	{11, 0, 10, 20, -20},
	{12, 0, 30, 50, 10},
};

stuRevWaterT RECWatreT[ADCMAX]={
	//0 蒸发器温度 1-环境温度 2-水箱温度 3-排气温度 4-回气温度 5-断线-具体以.h文件ADCMAX为准
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0}
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

	MenuParam.timer.onHour = 11;
	MenuParam.timer.onMin = 0;
	MenuParam.timer.offHour = 11;
	MenuParam.timer.offMin = 0;
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
	MenuParam.runFlag = 0;//0 off , 1 run
	MenuParam.StartParamChange = 0;// set by timeon/off on/off elec
	MenuParam.WaterT = 0;

	lcd_wr_char(_lcd11_waterT, 1);
	lcd_wr_char(_lcd13_c, 1);
}

