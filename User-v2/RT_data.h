#ifndef __RT_DATA_H
#define __RT_DATA_H

#include "stm32f10x.h"

typedef struct DataFrameStruct 
{
	uint8_t length;
	uint8_t command;
}DataFrameStruct;

typedef struct{
	uint8_t machineState;	//开/关机状态
	uint8_t elecState;	//电加热打开/关闭状态
}RunState_t;

typedef struct{
	uint16_t state;				//状态开或关
	int16_t inTemper;			//吸气温度
	int16_t outTemper;			//排气温度
	int16_t evaporateTemper;	//蒸发温度
	int16_t current;			//电流
	int16_t valveMainStep;		//主电子膨胀发开度
	int16_t valveSubStep;		//辅助电子膨胀阀开度
}Machine_t;

typedef struct Command3ReturnDataStruct{
	DataFrameStruct dataFrame;
	int16_t waterIn;
	int16_t waterOut;
	int16_t waterBank;
	int16_t evironT;
	int16_t innerTemper;

	uint16_t errType;
	uint16_t cd4051DectState;	//cd4051状态

	RunState_t runState;
	Machine_t machineA;
}Command3ReturnDataStruct;


void RT_rxCommand3(stuRevWaterT *dst, uint8_t *srcData);

#endif

