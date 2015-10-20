#ifndef __STM32_UB_PARAMS
#define __STM32_UB_PARAMS

typedef struct
{
	uint8_t hour;
	uint8_t min;
	uint8_t posHour;
	uint8_t posMin;
	uint8_t flag; // 0 select hour(flash ++ --) 1 select min(flash -- ++)
}stuHours;

typedef struct
{
	uint8_t onHour;
	uint8_t onMin;
	uint8_t offHour;
	uint8_t offMin;
	uint8_t posHour;
	uint8_t posMin;
	uint8_t flag;
}stuTimer;

typedef enum
{
	PosHour = 6,
	PosMin = 8
}posLight;

typedef struct 
{
	void (*pfmenu)(void);
	stuHours clock;
	stuTimer timer;
}stuLcdParams;

extern stuLcdParams MenuParam;

#endif

