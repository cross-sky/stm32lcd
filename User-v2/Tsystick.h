#ifndef __TSYSTICK_H
#define __TSYSTICK_H

#define TestEx 1
extern u8 G_TestExcut;

extern void SystickInit( void );
void TestExcuteTimeGpioConfig(void);
void TestExcuteTime(u8 state);

#endif

/*
#if TestEx
	TestExcuteTime(1);
#endif

#if TestEx
	TestExcuteTime(0);
#endif
*/

