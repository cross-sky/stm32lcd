#include "cominc.h"

//add task
//1.add init 2.add task.h->task 3.add taskcomps

TPC_TASK TaskComps[]=
{
	{0, 0, 201,  201, LED1on},			// 5*200=1s   ok
	{0, 0, 2001,	2001, Pcf8536RT},
	//{0, 0, 200, 200, lcd1621test},
	{0,	0, 31, 31, UB_Button_KeyIn},
	{0, 0, 70, 70, MenuLcd},
	{0, 0, 20, 20, TBeepOn},//
	{0, 0, 200, 200, TBgledOn},
	{0, 0, 51, 51, lcd_flood_data}	//must excute at last
};



void TaskInit(void)
{
	UB_LED_Init();		//PC1,PC3,PC13  ok	
	SystickInit();
	UART1_Init(BoundSet);
	IIC_Init();


	//RTtest();
	AT24CxxTest();
	PCF8563Init();
	Lcd1621Init();
	UB_Button_Init();	//key init pa0-pa5;
	MenuParaInit();		//init params
}



void TPCRemarks( TPC_TASK *pTask)
{
	u8 i;
	
	for ( i=0; i<TASKS_MAX; i++)
	{
		if( pTask[i].Timer > 0 && pTask[i].Suspend == 0)
		{
			pTask[i].Timer--;
			if (pTask[i].Timer == 0)
			{
				pTask[i].Timer = pTask[i].ItvTime;
				pTask[i].Run = TPC_RUN_STM; 
			}
		}
	}
}

void TPCProcess(TPC_TASK *pTask)
{
	u8 i;

	for (i=0; i<TASKS_MAX; i++)
	{
		if (pTask[i].Run == TPC_RUN_STM && pTask[i].Suspend == 0)
		{
			pTask[i].TaskHook();
			pTask[i].Run = TPC_RUN_CLM;
		}

	}
}



