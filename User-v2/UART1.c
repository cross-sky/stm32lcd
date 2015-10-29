#include "cominc.h"

//���� ����ṹ��ʵ������������
UartBuf	 UartTxbuf;//���η��ͽṹ��
UartBuf	 UartRxbuf;//���ν��սṹ��

uint8_t rx_buffer[RX_BUFFER_SIZE];
uint8_t tx_buffer[TX_BUFFER_SIZE];



//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ��������� ,����uart�˿ڵ���
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 


//���������u32 baudrate   ����RS232���ڵĲ�����
void UART1_Init(uint32_t bound)
{
	GPIO_InitTypeDef	gpio_inits;
	USART_InitTypeDef	usart_inits;
	NVIC_InitTypeDef	nvic_inits;

	RCC_APB2PeriphClockCmd(USARTy_GPIO_CLK|USARTy_CLK, ENABLE);//����uart��gpioʱ��������
	USART_DeInit(USARTy);
	//pA.9 ����������� USART1_TX;
	gpio_inits.GPIO_Pin	= USARTy_TxPin;
	gpio_inits.GPIO_Speed = GPIO_Speed_2MHz;
	gpio_inits.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USARTy_GPIO, &gpio_inits);

	//PA.10 �������� usart1_RX;
	gpio_inits.GPIO_Pin = USARTy_RxPin;
	gpio_inits.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USARTy_GPIO, &gpio_inits);

	usart_inits.USART_BaudRate = bound;
	usart_inits.USART_WordLength = USART_WordLength_8b;
	usart_inits.USART_StopBits = USART_StopBits_1;
	usart_inits.USART_Parity = USART_Parity_No;
	usart_inits.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_inits.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_Init(USARTy, &usart_inits);
	//��2�飺���2λ����ָ����ռʽ���ȼ�-PreemptionPriority�����2λ����ָ����Ӧ���ȼ�-SubPriority
	//���顱���ȼ���>������ռ���ȼ���>���������ȼ���
	//��ֵԽС����������ȼ���Խ��,�б�Ҫ������0
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	nvic_inits.NVIC_IRQChannel = USARTy_IRQn;
	nvic_inits.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_inits.NVIC_IRQChannelSubPriority = 8;
	nvic_inits.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&nvic_inits);

	USART_ITConfig(USARTy, USART_IT_RXNE, ENABLE);  //���ջ������ǿ��ж�ʹ��
	USART_Cmd(USARTy, ENABLE);

	UartTxbuf.Wd_Index = 0;
	UartTxbuf.Rd_Index = 0;
	UartTxbuf.Mask = TX_BUFFER_SIZE - 1;
	UartTxbuf.pbuf = &tx_buffer[0];

	UartRxbuf.Wd_Index = 0;
	UartRxbuf.Rd_Index = 0;
	UartRxbuf.Mask = RX_BUFFER_SIZE -1;
	UartRxbuf.pbuf = &rx_buffer[0];

//	printf("sysclock: %dMHz \r\n", cycleCounter());
#ifdef Debug
	printf("uart1 bound: %d \r\n", bound);
#endif
}


//unsigned char DataToSend   Ҫ���͵��ֽ����� RS232����һ���ֽ�
void UART1_Put_char(uint8_t dataToSend)
{
	UartBuf_WD(&UartTxbuf, dataToSend);
	USART_ITConfig(USARTy, USART_IT_TXE, ENABLE);

}

//��һ���ֽ�д��һ�����νṹ����
void UartBuf_WD(UartBuf *ringBuf, uint8_t dataIn)
{
	ringBuf->pbuf[ringBuf->Wd_Index & ringBuf->Mask] = dataIn;
	ringBuf->Wd_Index++;
}


////��ȡ���������е�һ���ֽ�
uint8_t UartBuf_RD(UartBuf *ringBuf)
{
	uint8_t temp;
	temp = ringBuf->pbuf[ringBuf->Rd_Index &ringBuf->Mask];
	ringBuf->Rd_Index++;
	return temp;
}


//�����������Ŀ����ֽڳ��ȣ���дָ��д��һȦ��׷���˶�ָ�� (wd+mask-id)%mask
//��ô֤������д���ˣ���ʱӦ�����ӻ��������ȣ�����������Χ���ݴ���ʱ��
uint16_t UartBuf_Cnt(UartBuf *ringBuf)
{
	return (ringBuf->Mask +ringBuf->Wd_Index - ringBuf->Rd_Index) % ringBuf->Mask;
}

void UartBufClear(UartBuf *ringBuf)
{
	ringBuf->Rd_Index = ringBuf->Wd_Index =0;
}

void UartSendBuffer(uint8_t *dat, uint8_t len)
{
	uint8_t i;
	for (i=0; i<len; i++)
	{
		UartBuf_WD(&UartTxbuf, *dat);
		dat++;
	}
	USART_ITConfig(USARTy, USART_IT_TXE, ENABLE);//
}

volatile uint8_t UdataTemp;//���ڽ�����ʱ�����ֽ�

void USARTy_IRQHandler(void)
{
//	uint8_t i;
	if (USART_GetITStatus(USARTy, USART_IT_TXE) != RESET)
	{
		USART_SendData(USARTy, UartBuf_RD(&UartTxbuf));
		if(UartBuf_Cnt(&UartTxbuf) == 0 )
			USART_ITConfig(USARTy, USART_IT_TXE, DISABLE);
	}

	else if(USART_GetITStatus(USARTy, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USARTy, USART_IT_RXNE);//��������жϱ�־
		UdataTemp = (uint8_t)USART_ReceiveData(USARTy);

		UartBuf_WD(&UartRxbuf, UdataTemp);//д���ڽ��ջ�������

	}

}

void RTtest(void)
{
	uint8_t i, cnTemp;
	cnTemp = UartBuf_Cnt(&UartRxbuf);
	if (cnTemp == 5)
	{
		for (i=0; i<cnTemp; i++)
		{
			UartBuf_WD(&UartTxbuf, UartBuf_RD(&UartRxbuf));
		}
		
			USART_ITConfig(USARTy, USART_IT_TXE, ENABLE);
	}
//	UartBufClear(&UartRxbuf);


}
