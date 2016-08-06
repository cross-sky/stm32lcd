#ifndef __RT_DATA_H
#define __RT_DATA_H

#include "stm32f10x.h"

typedef struct DataFrameStruct 
{
	uint8_t length;
	uint8_t command;
}DataFrameStruct;

typedef struct{
	uint8_t machineState;	//��/�ػ�״̬
	uint8_t elecState;	//����ȴ�/�ر�״̬
}RunState_t;

typedef struct{
	uint16_t state;				//״̬�����
	int16_t inTemper;			//�����¶�
	int16_t outTemper;			//�����¶�
	int16_t evaporateTemper;	//�����¶�
	int16_t current;			//����
	int16_t valveMainStep;		//���������ͷ�����
	int16_t valveSubStep;		//�����������ͷ�����
}Machine_t;

typedef struct Command3ReturnDataStruct{
	DataFrameStruct dataFrame;
	int16_t waterIn;
	int16_t waterOut;
	int16_t waterBank;
	int16_t evironT;
	int16_t innerTemper;

	uint16_t errType;
	uint16_t cd4051DectState;	//cd4051״̬

	RunState_t runState;
	Machine_t machineA;
}Command3ReturnDataStruct;


void RT_rxCommand3(stuRevWaterT *dst, uint8_t *srcData);

#endif

