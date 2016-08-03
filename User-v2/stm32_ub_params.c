#include "cominc.h"

stuLcdParams MenuParam;

//1.coreparams=7 
stuCoreParam NumCoreParam[CoreParamsMax]={
	//id,flag value max min
	{0, 0, 50,	70,	30},	//����ˮ��
	{1, 0, 5,	15,		4},			//����ˮ�»ز�
	{2, 0, -7,  0, -30},	//�����˪�¶�
	{3, 0, 8,	12,	 1},		//��˪ʱ��
	{4, 0, 10,	15, 5},		//�˳���˪�¶�
	{5, 0, 5,	8, 2},		//��˪����	30����Ϊ����
	{6, 0, 3,	5, 0},		//��˪����ƫ��	
	//{7, 0, -7,  0, -30},
	//{8, 0, 13,	30, 2},
	//{9, 0, 8,	12, 1},
	//{10, 0, 1,	1, 0},
	//{11, 0, 10, 20, -20},
	//{12, 0, 30, 50, 10},
};

stuRevWaterT RECWatreT[ADCMAX]={
	//0 �������¶� 1-�����¶� 2-ˮ���¶� 3-�����¶� 4-�����¶� 5-����-������.h�ļ�ADCMAXΪ׼
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

