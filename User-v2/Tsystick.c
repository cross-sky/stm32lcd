#include "cominc.h"


#define D_5MS	200
#define D_1MS	1000			//1ms�ж�1��

u8 G_TestExcut;

void SystickInit( void )
{
	if ( SysTick_Config( SystemCoreClock / D_5MS ) )
	{
		while ( 1 );

	}			//���ò��ɹ����������ѭ��;

}




