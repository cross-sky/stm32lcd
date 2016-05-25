#ifndef __STM32_UB_PARAMS
#define __STM32_UB_PARAMS

#define CoreParamsMax 13

#define ShortT 3
#define OenT 2
#define OutLine 4

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
	uint8_t runFlag;
	uint8_t StartParamChange;
	uint8_t WaterT;
	uint8_t defrostManul;	//手动强制除霜
}stuLcdParams;

typedef struct
{
	uint8_t errFlag;
	int16_t Value;
}stuRevWaterT;



typedef enum{
	ADC01_AIN=0x00,	//A吸气
	ADC02_AMI,		//A蒸发温度
	ADC03_AOUT,		//A排气温度
	ADC04_WIN,		//进水温度
	ADC05_WOUT,		//出水温度
	ADC06_ENVT,		//环温度
	ADC07_WTANK,	//水箱温度
	ADC08_BIN,		//B吸气温度
	ADC09_BMI,		//B蒸发温度
	ADC10_BOUT,		//B排气温度
	ADC11_TCA,		//互感器A
	ADC12_TCB,		//互感器B
	ADC13_INTER,	//内部温度
	ADC14_humidity,	//湿度
	ADC15_humiTemper,//湿度模块测量的温度
	ADC_OUTLINE,
	ADCMAX,			//最大值
}ADC_t;

extern stuLcdParams MenuParam;
extern stuCoreParam NumCoreParam[CoreParamsMax];
extern stuRevWaterT RECWatreT[ADCMAX];

#endif

