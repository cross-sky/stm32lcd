#include "cominc.h"

void RT_rxCommand3(stuRevWaterT *dst, uint8_t *srcData)
{
	uint8_t i=0;
	Command3ReturnDataStruct *sc3 = (Command3ReturnDataStruct*) &(srcData[2]);
	dst[ADC01_AIN].Value = sc3->machineA.inTemper;
	dst[ADC02_AMI].Value = sc3->machineA.evaporateTemper;
	dst[ADC03_AOUT].Value =sc3->machineA.outTemper;
	dst[ADC04_WIN].Value =sc3->waterIn;
	dst[ADC05_WOUT].Value =sc3->waterOut;
	dst[ADC06_ENVT].Value =sc3->evironT;
	dst[ADC07_WTANK].Value =sc3->waterBank;
	dst[ADC08_BIN].Value =sc3->machineA.valveMainStep;
	dst[ADC09_BMI].Value =sc3->machineA.valveSubStep;
	dst[ADC10_BOUT].Value =0;
	dst[ADC11_TCA].Value =0;
	dst[ADC12_TCB].Value =0;
	dst[ADC13_INTER].Value =sc3->innerTemper;
	dst[ADC14_humidity].Value =0;
	dst[ADC15_humiTemper].Value =0;

	for (i=0;i<ADC_OUTLINE;i++)
	{
		dst[i].errFlag = 0;
	}
}

