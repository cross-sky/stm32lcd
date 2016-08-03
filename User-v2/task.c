#include "cominc.h"

//add task
//1.add init 2.add task.h->task 3.add taskcomps

TPC_TASK TaskComps[]=
{
	{0, 0, 201,  201, LED1on},			// 5*200=1s   ok	
	{0, 0, 4,  4, UartDmaSendV2},
	//{0, 0, 1,  1, UartDmaSend},
	{0, 0, 2001,	2001, Pcf8536RT},
	//{0, 0, 200, 200, lcd1621test},
	{0,	0, 31, 31, UB_Button_KeyIn},
	{0, 0, 83, 83, MenuLcd},
	{0, 0, 20, 20, TBeepOn},//
	{0, 0, 200, 200, TBgledOn},
	{0, 0, 200, 200, TaskSendStartParm},	
	{0, 0, 41, 41, lcd_flood_data}	//must excute at last
};



void TaskInit(void)
{
	//��2�飺���2λ����ָ����ռʽ���ȼ�-PreemptionPriority�����2λ����ָ����Ӧ���ȼ�-SubPriority
	//���顱���ȼ���>������ռ���ȼ���>���������ȼ���
	//��ֵԽС����������ȼ���Խ��,�б�Ҫ������0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	MenuParaInit();		//init params
	UB_LED_Init();		//PC1,PC3,PC13  ok	

	
	//UART1_Init(BoundSet);
	UART1_Init();
	IIC_Init();
	SystickInit();
	delay_ms(100);
	
	
	//RTtest();
	AT24CxxTest();
	PCF8563Init();
	Lcd1621Init();
	UB_Button_Init();	//key init pa0-pa5;
	RequestTempe();
	//@@@@@@@@@��ʾ��Ҫ��ʱһ�������������Ҳû��ϵ���������ط�
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



