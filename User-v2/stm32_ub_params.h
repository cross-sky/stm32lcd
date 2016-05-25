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
	uint8_t defrostManul;	//�ֶ�ǿ�Ƴ�˪
}stuLcdParams;

typedef struct
{
	uint8_t errFlag;
	int16_t Value;
}stuRevWaterT;



typedef enum{
	ADC01_AIN=0x00,	//A����
	ADC02_AMI,		//A�����¶�
	ADC03_AOUT,		//A�����¶�
	ADC04_WIN,		//��ˮ�¶�
	ADC05_WOUT,		//��ˮ�¶�
	ADC06_ENVT,		//���¶�
	ADC07_WTANK,	//ˮ���¶�
	ADC08_BIN,		//B�����¶�
	ADC09_BMI,		//B�����¶�
	ADC10_BOUT,		//B�����¶�
	ADC11_TCA,		//������A
	ADC12_TCB,		//������B
	ADC13_INTER,	//�ڲ��¶�
	ADC14_humidity,	//ʪ��
	ADC15_humiTemper,//ʪ��ģ��������¶�
	ADC_OUTLINE,
	ADCMAX,			//���ֵ
}ADC_t;

extern stuLcdParams MenuParam;
extern stuCoreParam NumCoreParam[CoreParamsMax];
extern stuRevWaterT RECWatreT[ADCMAX];

#endif

