#include "cominc.h"

//const u8* hs_stab[]={"50us",	"100us","200us","1ms",	"2ms",	"10ms","20ms","100ms"};
const u8  Simple_Num[]={20,		40,		40,		100,	100,	100,	100,	100};
const float  Simple_Tim[]={2.5,	2.5,	5,		10,		20,		100,	200,	1000};
const u16 Simple_Period[]={179,	179,	359,	719,	1339,	7199,	7199,	7199};
const u8  Simple_Prescal[]={0,		0,		0,		0,		0,		0,		1,		9};

T_DSO g_DsoA;

//DMA���ݣ�Ѱ�Ҵ�����
//��û�ҵ������¿�dma��T2��ʱ�� cc2����
//20msѭ��1��
void DMAReadAdc(void)
{
	u16 i;
	if(g_DsoA.DMAState == 1)
	{
		for (i=0; i<300; i++)
		{
			if(g_DsoA.TrigEdge == 0)
			{
				if ((g_DsoA.DMA_ADCBuf[i] < g_DsoA.TrigLevel)
					&&(g_DsoA.DMA_ADCBuf[i+1] >= g_DsoA.TrigLevel))
				{
					g_DsoA.ADCTrigCnt = i;
					g_DsoA.TrigFlag = 1;
					break;
				}
			}
			else
			{
				if ((g_DsoA.DMA_ADCBuf[i] > g_DsoA.TrigLevel)
					&&(g_DsoA.DMA_ADCBuf[i+1] <= g_DsoA.TrigLevel))
				{
					g_DsoA.ADCTrigCnt = i;
					g_DsoA.TrigFlag = 1;
					break;
				}

			}

		}
		g_DsoA.DMAState = 0;
		
		if(i >= 300 )
		{
			g_DsoA.TrigFlag = 0;
					
			DMA_Cmd(DMA1_Channel1,DISABLE);
			DMA1_Channel1->CNDTR = DMA_ADCBUFSIZE;
			DMA1_Channel1->CMAR = (uint32_t)&g_DsoA.DMA_ADCBuf;
			DMA_Cmd(DMA1_Channel1,ENABLE);
			TIM_Cmd(TIM2, ENABLE);
		}


	}

}

/*
*********************************************************************************************************
*	�� �� ��: SetSampRate
*	����˵��: �޸Ĳ���Ƶ��
*	��    �Σ�freq : ����Ƶ�� ��λHz
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Set_SampRate(T_DSO *g_DsoA)
{
	/* ��4���� ���ö�ʱ��2��2ͨ��CC2, CC2��ΪADC1�Ĵ���Դ */
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef			TIM_OCInitStructure;
//	uint16_t Period;

	TIM_Cmd(TIM2, DISABLE);

	/*
	����Ƶ�ʼ��㹫ʽ ��
		period = 72000000 / freq ;
		
		1200 == 60KHz ����Ƶ��
	*/
//	Period = 72000 / freq -1;		

	/* ʹ�� TIM1 ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
	TIM_TimeBaseStructure.TIM_Period = g_DsoA->SimplePeriod;          
	TIM_TimeBaseStructure.TIM_Prescaler = g_DsoA->SimplePrescal;   /* ����Ƶ�� = 72000 000 / 18 = 4000 000	 */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* TIM1 ͨ��1����ΪPWMģʽ */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
	TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period / 2; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	/* TIM1 ������ʹ�� */
	TIM_Cmd(TIM2, DISABLE);

	/* TIM1 ���ʹ�� */
	TIM_CtrlPWMOutputs(TIM2, ENABLE);  
}

static void Init_DSO(T_DSO *g_DsoA)
{
	/* ����GPIO.  */
	
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef	ADC_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;

	/* ��GPIO_C �� AFIO ��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC	| RCC_APB2Periph_AFIO, ENABLE);

	/* ����PC0Ϊģ������ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ʹ�� DMA1 ʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Channel1);		/* ��λDMA1�Ĵ�����ȱʡ״̬ */
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	/* ѡ��ADC1�����ݼĴ�����ΪԴ */
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_DsoA->DMA_ADCBuf;	/* Ŀ���ַ */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	/* ����DMA���䷽������(ADC)��ΪԴ */
	DMA_InitStructure.DMA_BufferSize = DMA_ADCBUFSIZE;	/* ���û�������С */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	/* �����ַ������ */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	/* �洢����ַ��Ҫ���� */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	/* ѡ�����贫�䵥λ��16bit */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	/* ѡ���ڴ洫�䵥λ��16bit */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	/* ����ѭ��ģʽ   */
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	/* ѡ��DMA���ȼ� */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	/* DMA�������ͣ������ڴ浽�ڴ� */
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TE,ENABLE);
	/* ʹ�� DMA1 ͨ��1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/*Enable DMA Channel5 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	/* ����ADC1  */
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		/* ����ת����ֹ */
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;	/* ѡ��TIM1��CC3������ */

	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	/* �����Ҷ���,��λΪ0 */
	ADC_InitStructure.ADC_NbrOfChannel = 1;	/* 1��ͨ�� */
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 ����ͨ������ */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_1Cycles5);

	/* ʹ�� ADC1 �ⲿ����ת�� */
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);

	/* ʹ�� ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* ʹ�� ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* ʹ�� ADC1 ��λУ׼�Ĵ��� */
	ADC_ResetCalibration(ADC1);
	/* ��⸴λУ׼�Ĵ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* ��ʼ ADC1 У׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ�У׼���� */
	while(ADC_GetCalibrationStatus(ADC1));

	Set_SampRate(g_DsoA);
}



//dma�ɼ�600����T1��ʱ��
void Dma1_Channel1Handle(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC1);
	//	DMA_ClearFlag(DMA1_FLAG_TC4);
	DMA_ClearITPendingBit(DMA1_IT_TE1);
	g_DsoA.DMAState=1;//DMA�������
	DMA_Cmd(DMA1_Channel1,DISABLE);
	TIM_Cmd(TIM2, DISABLE);

}

/*
*********************************************************************************************************
*	�� �� ��: InitDsoParam
*	����˵��: ��ʼ��ȫ�ֲ�������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DsoInitParam(void)
{
	
	g_DsoA.BufUsed = 0;

	g_DsoA.TrigLevel = 2048;//������ƽ
	g_DsoA.TrigPs = 0;//����λ��:ʱ��
	g_DsoA.TrigEdge = 1;//��������:0������,1�½���
	g_DsoA.TrigFlag = 0;//�������ı�־

	g_DsoA.TimeId = 3;			/* ʱ������, ���ɵõ� us��λ��ʱ�� */
	g_DsoA.SimpleNum = Simple_Num[g_DsoA.TimeId];			/* ���������� */
	g_DsoA.SimpleTim = Simple_Tim[g_DsoA.TimeId];				/* �������� */
	g_DsoA.SimplePeriod = Simple_Period[g_DsoA.TimeId];			//Tim1ʱ����
	g_DsoA.SimplePrescal = Simple_Prescal[g_DsoA.TimeId];			//Tim1ʱ�ӷ�Ƶ��

	g_DsoA.ADCTrigCnt=0;//ADC����λ��
	g_DsoA.DMAState=0;   //dma�ɼ�600�����

}


/*
*********************************************************************************************************
*	�� �� ��: FrameDisp
*	����˵��: ��ʾ���δ��ڵı߿�Ϳ̶���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void FrameDisp(void)
{
	uint16_t x, y;

	/* ����һ��ʵ�߾��ο� x, y, h, w */
	LCD_DrawRect(9, 19, 202, 302, CL_WHITE);

	/* ���ƴ�ֱ�̶ȵ� */
	for (x = 0; x < 13; x++)
	{
		for (y = 0; y < 41; y++)
		{
			Gui_DrawPoint(10 + (x * 25), 20 + (y * 5), CL_WHITE);
		}
	}

	/* ����ˮƽ�̶ȵ� */
	for (y = 0; y < 9; y++)
	{
		for (x = 0; x < 61; x++)
		{
			Gui_DrawPoint(10 + (x * 5), 20 + (y * 25), CL_WHITE);
		}
	}

	/* ���ƴ�ֱ���Ŀ̶ȵ� */
	for (y = 0; y < 41; y++)
	{	 
		Gui_DrawPoint(9 + (6 * 25), 20 + (y * 5), CL_WHITE);
		Gui_DrawPoint(11 + (6 * 25), 20 + (y * 5), CL_WHITE);
	}

	/* ����ˮƽ���Ŀ̶ȵ� */
	for (x = 0; x < 61; x++)
	{	 
		Gui_DrawPoint(10 + (x * 5), 19 + (4 * 25), CL_WHITE);
		Gui_DrawPoint(10 + (x * 5), 21 + (4 * 25), CL_WHITE);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DispChInfo
*	����˵��: ��ʾͨ����Ϣ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ChInfoDisp(void)
{
	char buf[32];   	/* �ַ���ʾ������ */
	FONT_T tFont;		/* ����һ������ṹ���������������������� */
//	float tTem;
	/* ������������ */
	tFont.usFontCode = FC_ST_16X16;		/* ����ѡ������16���󣬸�16x��15) */
	tFont.usTextColor = CL_WHITE;		/* ������ɫ����Ϊ��ɫ */
	tFont.usBackColor = CL_BLUE;	 	/* ���ֱ�����ɫ����ɫ */
	tFont.usSpace = 0;					/* �ַ�ˮƽ���, ��λ = ���� */

	/* ��ʾʾ��������汾 */
	Gui_DrawFont_GBK16(10, 2, "v.0", &tFont);


	strcpy(buf, "CH1 DC");
	sprintf(&buf[7], "3.6V");
	tFont.usTextColor = CL_YELLOW;		/* ����������ɫ����ɫ */
	tFont.usBackColor = CL_MAGENTA;	 	/* ���ֱ�����ɫ����ɫ */
	Gui_DrawFont_GBK16(10, 224, buf, &tFont);
	
	sprintf(buf, "%.2f",tTemp[3]);
  Gui_DrawFont_GBK16(100, 224, buf, &tFont);
	/* ��ʾʱ�� */
//	tFont.usTextColor = CL_WHITE;		/* ����������ɫ����ɫ */
//	tFont.usBackColor = CL_BLUE;	 	/* ���ֱ�����ɫ����ɫ */

	/* ��ʾÿ��೤ʱ�䣬��ʱ��ֶ� */
//	sprintf(buf, "t:%.1fus",	Simple_Tim[g_DsoA.SimpleNum]);
//	Gui_DrawFont_GBK16(230, 224, buf, &tFont);

	/* ��ʾ����Ƶ�� */
	//sprintf(buf, "f:%7dHz",	Simple_Tim[g_DsoA->SimpleNum]);
	//Gui_DrawFont_GBK16(75, 2, buf, &tFont);
}

/*
*********************************************************************************************************
*	�� �� ��: DispCh1Wave
*	����˵��: ��ʾͨ��1����
*	��    �Σ���
*	�� �� ֵ: ��
��Сֵ���Ϸ�������Ĳ�Ҫ��ת
��Ļ����
0,0----------------- 320
|
|
|
|
240
*********************************************************************************************************
*/
static void Ch1WaveDisp(void)
{
	uint16_t i=0,j=0;		
	uint16_t *px,*pxcl;
	uint16_t *py,*pycl;
	int16_t iTemp;

	if (g_DsoA.BufUsed == 0)
	{
		g_DsoA.BufUsed = 1;
	}
	else
	{
		g_DsoA.BufUsed = 0;
	}

	if (g_DsoA.BufUsed == 0)
	{
		px = g_DsoA.xCh1Buf1;
		py = g_DsoA.yCh1Buf1;

		pxcl = g_DsoA.xCh1Buf2;
		pycl = g_DsoA.yCh1Buf2;
	}
	else
	{
		px = g_DsoA.xCh1Buf2;
		py = g_DsoA.yCh1Buf2;

		pxcl = g_DsoA.xCh1Buf1;
		pycl = g_DsoA.yCh1Buf1;
	}

	/* CH1ͨ��, ����0Vʱ��ADC����ֵ = 1872 ,���Ǹ��ݶ���������󵽵�ƽ��ֵ */
	j = g_DsoA.ADCTrigCnt;
	for (i=0; i < 300; i++)
	{
		px[i] = 10 + i;
		iTemp = ((u32)g_DsoA.DMA_ADCBuf[j]*100)>>12;

		if (iTemp > 100)
		{
			iTemp = 100;
		}

		py[i] = 220 - iTemp;
		j++;
	}

	/* ����Ĵ����ȡ�����֡���Σ�Ȼ������������²��Σ���������ˢ�µ���˸�� */
	/* �����֡���� */
// 	if (g_DsoA.BufUsed == 0)
// 	{
// 		LCD_DrawPoints(g_DsoA.xCh1Buf2, g_DsoA.yCh1Buf2, 300, CL_BLUE);
// 	}
// 	else
// 	{
// 		LCD_DrawPoints(g_DsoA.xCh1Buf1, g_DsoA.yCh1Buf1, 300, CL_BLUE);
// 	}
// 
// 	/* ��ʾ���µĲ��� */
	FrameDisp();
	ChInfoDisp();
// 	LCD_DrawPoints((uint16_t *)px, (uint16_t *)py, 300, CL_YELLOW);
	//	DMA_SetCurrDataCounter(DMA1_Channel4,10);
	//		DMA1_Channel4->CNDTR = 10;
	//	DMA1_Channel4->CMAR = (uint32_t)&px[0];
	//	DMA_Cmd(DMA1_Channel4,ENABLE);
	
	for (i=0; i<199; i++)
	{
		Gui_DrawLine(pxcl[i], pycl[i], pxcl[i + 1], pycl[i + 1], CL_BLUE);
		Gui_DrawLine(px[i],	  py[i],   px[i + 1],   py[i + 1],   CL_YELLOW);
	}
}


//�������ҵ�֮����ʾ����
//�Ȼ�֡��ͼ�����ñ���ɫ����һ�εĲ��Σ��������µĲ���

void TaskDso(void)
{
	if (g_DsoA.TrigFlag == 1)
	{
		Ch1WaveDisp();
		g_DsoA.TrigFlag = 0;
		
		DMA_Cmd(DMA1_Channel1,DISABLE);
		DMA1_Channel1->CNDTR = DMA_ADCBUFSIZE;
		DMA1_Channel1->CMAR = (uint32_t)&g_DsoA.DMA_ADCBuf;
		DMA_Cmd(DMA1_Channel1,ENABLE);
		TIM_Cmd(TIM2, ENABLE);
	}


}

void InitDso(void)
{
	Lcd_Clear(CL_BLUE);
	DsoInitParam();
	Init_DSO(&g_DsoA);
//	FrameDisp();
//	ChInfoDisp();
}




