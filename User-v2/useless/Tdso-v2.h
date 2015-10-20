#ifndef __TDSOV2_H
#define __TDSOV2_H

#define ADC1_DR_Address ( (u32)0x4001244C)

#define	DMA_ADCBUFSIZE	600 	/* ������ȣ�������������� */
#define WAVBUFSIZE		300

typedef struct
{
	u16 DMA_ADCBuf[DMA_ADCBUFSIZE];	 /* ͨ��1���ݻ����� */
	u16 xCh1Buf1[310];//��ʾ���ε�����
	u16 yCh1Buf1[310];//��ʾ���ε�����
	u16 xCh1Buf2[310];//��ʾ���ε�����
	u16 yCh1Buf2[310];//��ʾ���ε�����
	u8  BufUsed;

	u16	TrigLevel;//������ƽ
	u8 	TrigPs;//����λ��:ʱ��
//	u8	TrigMode;//����ģʽ:�Զ�,��ͨ,����
	u8	TrigEdge;//��������:0������,1�½���
//	u8	TrigCh;//����ͨ��
//	u32 TrigWait;
	u8	TrigFlag;//�������ı�־

	u8  TimeId;			/* ʱ������, ���ɵõ� us��λ��ʱ�� */
	u8  SimpleNum;			/* ���������� */
	float  SimpleTim;				/* �������� */
	u16 SimplePeriod;			//Tim1ʱ����
	u8  SimplePrescal;			//Tim1ʱ�ӷ�Ƶ��;
	
	u16 ADCTrigCnt;			//ADC����λ��
	u8  DMAState;   //dma�ɼ�600�����

}T_DSO;

extern T_DSO g_DsoA;
extern const u16 Simple_Period[];
extern const u8 Simple_Prescal[];
void Dma1_Channel1Handle(void);
void InitDso(void);
void DMAReadAdc(void);
void TaskDso(void);
void UsartDmaTest(void);
#endif

