#include "cominc.h"


// main menu handle and clock set

void fClockOn(uint8_t hour, uint8_t min);

void MenuOnStatus(void)
{
	uint16_t keyValue=0;

	keyValue = KeyPop();
	switch(keyValue)
	{
	case BTN_CLOCK:	MenuParam.pfmenu = MenuSetClock; break;
	case BTN_TIMER: MenuParam.pfmenu = MenuSetTime; break;
	default:	break;
	}

	fClockOn(MenuParam.clock.hour, MenuParam.clock.min);
	//pfmenu();
}

void fClockOn(uint8_t hour, uint8_t min)
{
	uint8_t i,j;
	i = hour / 10;
	j = hour % 10 ;

	lcd_wr_num(i, 6, i);
	lcd_wr_num(j, 7, 1);

	i = min / 10;
	j = min % 10 ;

	lcd_wr_num(i, 8, i);
	lcd_wr_num(j, 9, 1);
}

void ClockFlash(uint8_t number, uint8_t position)
{
	uint8_t i,j;
	static uint8_t flag_flash=0;

	flag_flash &= 0x01;

	i = number / 10;
	j = number % 10 ;

	if (flag_flash == 0)
	{
		lcd_wr_num(i, position, i);// flash on when i=0, will not display; 0(i high)0(j low)
		lcd_wr_num(j, position+1, 1);// low position must be display
	}
	else
	{
		lcd_wr_num(i, position, 0);// flash off when i=0, will not display; 0(i high)0(j low)
		lcd_wr_num(j, position+1, 0);// low position will not display
	}
	flag_flash++;

}

uint8_t addHourMin(uint8_t number, uint8_t isHour)
{
	number++;	
	if (isHour == 0)	//hours add
	{
		if (number > 23)
		{
			number = 0;
		}
	}
	else
	{
		if (number > 59)// min add
		{
			number = 0;
		}
	}
	return number;
}

uint8_t subHourMin(uint8_t number, uint8_t isHour)
{
	number--;	
	if (isHour == 0)	//hours add
	{
		if (number == 0xff)
		{
			number = 23;
		}
	}
	else
	{
		if (number == 0xff)// min add
		{
			number = 59;
		}
	}
	return number;
	//ClockFlash(number, pos);//flash
}

void MenuSetClock(void)
{
	uint16_t keyValue=0;// when clock is setted ,need to keep the value on lcd,can't erase it
	static uint8_t numbs=0, pos=0;
	static uint8_t thour, tmin, isFirstFlag=1;

	if (isFirstFlag == 1)
	{
		thour = MenuParam.clock.hour;
		tmin = MenuParam.clock.min;
		isFirstFlag = 0;

		pos = MenuParam.clock.posHour;
		numbs = thour;
	}

	keyValue = KeyPop();
	switch(keyValue)
	{
	case BTN_UP:
		{
			numbs = addHourMin(numbs, MenuParam.clock.flag);
			//thour = numbs;
			//MenuParam.clock.hour = numbs;
			break;
		}
	case BTN_DOWN:
		{
			numbs = subHourMin(numbs, MenuParam.clock.flag);
			//tmin = numbs;
			//MenuParam.clock.min = numbs;
			break;
		}
	case BTN_CLOCK:
		{
			MenuParam.clock.flag++;//first click set hour, second click set min

			if (MenuParam.clock.flag  == 1)
			{
				pos = MenuParam.clock.posMin;
				thour = numbs;
				numbs = tmin;
				fClockOn(thour, tmin);// is nessary to clock on?== when next number chang,have to keep pre-number display
			}

			if (MenuParam.clock.flag > 1)
			{
				tmin = numbs;
				MenuParam.clock.flag = 0;
				MenuParam.pfmenu = MenuOnStatus; //on the third click,then save and quit
				MenuParam.clock.hour =thour;
				MenuParam.clock.min = tmin;
				isFirstFlag = 1;// 
				fClockOn(thour, tmin);
				return ;
			}
			break;
		}
	case BTN_SHUT:
		{
			MenuParam.pfmenu = MenuOnStatus;
			MenuParam.clock.flag = 0; // quit without save
			isFirstFlag = 1;
			fClockOn(MenuParam.clock.hour, MenuParam.clock.min);
			return ;
			//break;
		}
	default: break;
	}

	ClockFlash(numbs, pos);//flash
	
}

void MenuLcd(void)
{
	MenuParam.pfmenu();
}



void MenuSetTime(void)
{
	uint16_t keyValue=0;// when clock is setted ,need to keep the value on lcd,can't erase it
	static uint8_t numbs=0, pos=0;
	static uint8_t tonHour, tonMin, toffHour, toffMin, isFirstFlag=1;

	if (isFirstFlag == 1)
	{
		tonHour = MenuParam.timer.onHour;
		tonMin = MenuParam.timer.onMin;
		toffHour = MenuParam.timer.offHour;
		toffMin = MenuParam.timer.offMin;

		isFirstFlag = 0;

		pos = MenuParam.timer.posHour;//need to add timer on/off ico **********
		numbs = tonHour;
	
		fClockOn(tonHour, tonMin);
		return;
	}

	keyValue = KeyPop();
	switch(keyValue)
	{
	case BTN_UP:
		{
			numbs = addHourMin(numbs, MenuParam.timer.flag&0x01);
			break;
		}
	case BTN_DOWN:
		{
			numbs = subHourMin(numbs, MenuParam.timer.flag&0x01);
			break;
		}
	case BTN_TIMER:
		{
			MenuParam.timer.flag++;
			switch(MenuParam.timer.flag)
			{
			case 1: 
				{
					pos = MenuParam.timer.posMin;	//1. assign position
					tonHour = numbs;		//2.save  last numbers
					numbs = tonMin;	//3. assign new numbers
					fClockOn(tonHour, tonMin);

					break;
				}
			case 2:
				{
					pos = MenuParam.timer.posHour;	//1. assign position
					tonMin = numbs;		//2.save  last numbers
					numbs = toffHour;	//3. assign new numbers
					fClockOn(toffHour, toffMin);
					
					break;
				}
			case 3:
				{
					pos = MenuParam.timer.posMin;	//1. assign position
					toffHour = numbs;		//2.save  last numbers
					numbs = toffMin;	//3. assign new numbers
					fClockOn(toffHour, toffMin);
					
					break;
				}
			default:
				{
					toffMin = numbs;				//quit and save
					MenuParam.timer.flag = 0;		//1. reset flag
					MenuParam.pfmenu = MenuOnStatus;	// 2.reset pfmenu
					MenuParam.timer.onHour = tonHour;	//3. save params
					MenuParam.timer.onMin = tonMin;
					MenuParam.timer.offHour = toffHour;
					MenuParam.timer.offMin = toffMin;
					isFirstFlag = 1;					//4. reset firstflag
					fClockOn(MenuParam.clock.hour, MenuParam.clock.min);//5. display clock
					return ;					
				}
			}
			break;
		}

	case  BTN_SHUT:
		{
			MenuParam.pfmenu = MenuOnStatus;
			MenuParam.timer.flag = 0; // quit without save
			isFirstFlag = 1;
			fClockOn(MenuParam.clock.hour, MenuParam.clock.min);//5. display clock
			return ;
		}
	default: break;
	}

	ClockFlash(numbs, pos);//flash
}

