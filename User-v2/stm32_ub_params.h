#ifndef __STM32_UB_PARAMS
#define __STM32_UB_PARAMS

#define CoreParamsMax 13

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
	uint8_t ID;
	uint8_t isChange;
	int8_t value;
	int8_t max;
	int8_t min;
}stuCoreParam;



typedef struct 
{
	void (*pfmenu)(void);
	stuHours clock;
	stuTimer timer;
	uint8_t bgledFlag;
	uint8_t beepFlag;
	uint8_t lockFlag;
	stuCoreParam *CoreParam;
	uint8_t elecFlag;
}stuLcdParams;

extern stuLcdParams MenuParam;
extern stuCoreParam NumCoreParam[CoreParamsMax];



#endif

