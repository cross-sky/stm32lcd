#include "cominc.h"


#define D_5MS	200
#define D_1MS	1000			//1ms中断1次

u8 G_TestExcut;

void SystickInit( void )
{
	if ( SysTick_Config( SystemCoreClock / D_5MS ) )
	{
		while ( 1 );

	}			//配置不成功，则进入死循环;

}




