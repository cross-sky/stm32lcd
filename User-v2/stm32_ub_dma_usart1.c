#include "cominc.h"

//uint8_t TxBuffer2[TxFIFOSIZE];
uint8_t RxBuffer2[FIFO_SIZE]; /* Buffer emulating the FIFO */
uint8_t TxBuffer2[FIFO_SIZE];
//stuTxBuf TxBuffer2;


//uint8_t USART1_TX_Finish=1;//send finish 1
UART_LinkQueue *qUartLink;
UART_QNode lastSend;
uint8_t DmaFirstSend=1;

UART_LinkQueue* UartLinkQueueInit(void);
bool UART_DEQueue(UART_LinkQueue *q, UART_QueuePtr e);
void DMASendSet(uint8_t *addres_8u, uint8_t length);
bool TxPush(uint8_t source[], uint8_t len);
void TxPop(uint8_t len);

void UART1_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	DMA_InitTypeDef		DMA_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|USART1_Tx_GPIO_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(USART1_Tx_CLK,ENABLE);
	//RCC_APB2PeriphClockCmd(USART1_Rx_CLK|USART1_Rx_GPIO,ENABLE);	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	/* Configure USART_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USART1_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART1_Rx_GPIO,&GPIO_InitStructure);

	 /* Configure USART_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USART1_TxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(USART1_Tx_GPIO,&GPIO_InitStructure);

	DMA_DeInit(DMA1_Ch_Usart1_Tx);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART1_Tx_DR_Base;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer2;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = FIFO_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Ch_Usart1_Tx,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Ch_Usart1_Tx,DMA_IT_TC,ENABLE);
	DMA_ITConfig(DMA1_Ch_Usart1_Tx,DMA_IT_TE,ENABLE);
	USART_DMACmd(USART1_Tx, USART_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA1_Ch_Usart1_Tx,DISABLE);

	DMA_DeInit(DMA1_Ch_Usart1_Rx);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART1_Rx_DR_Base;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer2;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = FIFO_SIZE;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

	DMA_Init(DMA1_Ch_Usart1_Rx,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Ch_Usart1_Rx,DMA_IT_TC,ENABLE);//transmit complete
	DMA_ITConfig(DMA1_Ch_Usart1_Rx,DMA_IT_TE,ENABLE);//transmit enable
	USART_DMACmd(USART1_Rx, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Ch_Usart1_Rx,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	  //Enable DMA Channel4 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Ch_Usart1_Tx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*Enable DMA Channel5 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Ch_Usart1_Rx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = Bounds;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1_Tx, &USART_InitStructure);

	//�����ж�
	USART_ITConfig(USART1_Tx, USART_IT_IDLE , ENABLE);//
	USART_Cmd(USART1_Tx, ENABLE);
	USART_ClearFlag(USART1_Tx, USART_FLAG_TC);     //* �巢����Ǳ�־��
	qUartLink = UartLinkQueueInit();

	//TxBuffer2.in=0;
	//TxBuffer2.out=0;
}


void Usart1_handle(void)
{
	uint8_t DataLen, i;
	//UART_QNode e;

	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(DMA1_Ch_Usart1_Rx, DISABLE);
		DataLen = FIFO_SIZE - DMA_GetCurrDataCounter(DMA1_Ch_Usart1_Rx);
		if (DataLen > 0)
		{
			
			for (i=0; i< DataLen; i++)
			{
				TxBuffer2[i] = RxBuffer2[i];
			}

			UartDMAQueue(qUartLink, TxBuffer2, DataLen);
			//while (USART1_TX_Finish == 0)
			//{
			//	;
			//}
			//for (i=0; i< DataLen; i++)
			//{
			//	TxBuffer2.Txbuff[i] = RxBuffer2[i];
			//}
			////USART��DMA���������ѯ��ʽ���ͣ��˷��������ȼ��ж϶�������֡����
			//DMA_Cmd(DMA1_Ch_Usart1_Tx, DISABLE);//�ı�datasizeǰ��Ҫ��ֹͨ������
			//DMA1_Ch_Usart1_Tx->CNDTR = DataLen; //DMA1,����������
			//DMA1_Ch_Usart1_Tx->CMAR = (uint32_t)TxBuffer2.Txbuff;
			//USART1_TX_Finish = 0;//DMA���俪ʼ��־��
			//DMA_Cmd(DMA1_Ch_Usart1_Tx, ENABLE);


		}

		//Tx		DMA1_Channel4, Rx		DMA1_Channel5
		//gl global flag te transmit error ht half transmit
		DMA_ClearFlag(DMA1_FLAG_GL5|DMA1_FLAG_TC5|DMA1_FLAG_TE5|DMA1_FLAG_HT5);//���־
		DMA1_Ch_Usart1_Rx->CNDTR = FIFO_SIZE;//��װ��
		DMA_Cmd(DMA1_Ch_Usart1_Rx, ENABLE);//������,�ؿ�DMA
		//��SR���DR���Idle
		i = USART1->SR;
		i = USART1->DR;
	}
	if (USART_GetITStatus(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE);
	}
	USART_ClearITPendingBit(USART1, USART_IT_TC);
	USART_ClearITPendingBit(USART1, USART_IT_IDLE);
}

void Dma1ChRxHandle(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC5|DMA1_IT_TE5);
	DMA_Cmd(DMA1_Ch_Usart1_Rx, DISABLE);//�ر�DMA,��ֹ�������������, rx
	DMA1_Ch_Usart1_Rx->CNDTR = FIFO_SIZE;//��װ��
	DMA_Cmd(DMA1_Ch_Usart1_Rx, ENABLE);
}


//set last send flag to 1
//if did not have remain to send ,set flag to 1,and quit
//else send the remains and reset last send-params
void Dma1ChTxHandle(void)
{
	UART_QNode e;
	DMA_ClearITPendingBit(DMA1_IT_TC4|DMA1_IT_TE4);
	DMA_Cmd(DMA1_Ch_Usart1_Tx, DISABLE);
	//USART1_TX_Finish = 1;//��DMA�������
	
	lastSend.Addr_8U[lastSend.length]=1; //last send success
	if (qUartLink->front != qUartLink->rear)
	{
		DmaFirstSend = 0;
		UART_DEQueue(qUartLink, &e);//����
		DMASendSet(e.Addr_8U,e.length);
		lastSend.Addr_8U = e.Addr_8U;
		lastSend.length = e.length;
		//TxPop(e.length);
	}
	else
	{
		DmaFirstSend = 1;
	}
}

bool UART_ENQueue(UART_LinkQueue *q, uint8_t *addres_8u, uint8_t length)
{
	UART_QueuePtr s = (UART_QueuePtr)malloc(sizeof(UART_QNode));
	if (!s)
		return FALSE;
	s->Addr_8U = addres_8u;
	s->length = length;
	s->Next = NULL;
	q->rear->Next = s;
	q->rear = s;
	return TRUE;
}

bool UART_DEQueue(UART_LinkQueue *q, UART_QueuePtr e)
{
	UART_QueuePtr p;
	if(q->front == q->rear)
		return FALSE;
	p = q->front->Next;
	e->Addr_8U = p->Addr_8U;
	e->length = p->length;

	q->front->Next = p->Next;
	if(q->rear == p)
		q->rear = q->front;
	free(p);
	return TRUE;
}

UART_LinkQueue* UartLinkQueueInit(void)
{
	UART_LinkQueue *q;
	UART_QueuePtr p;

	q = (UART_LinkQueue *)malloc(sizeof(UART_LinkQueue));
	p = (UART_QueuePtr)malloc(sizeof(UART_QNode));
	p->Next = NULL;
	q->front = q->rear = p;
	return q;
}

void DMASendSet(uint8_t *addres_8u, uint8_t length)
{
	if (length == 0)
	{
		return ;
	}
	DMA_Cmd(DMA1_Ch_Usart1_Tx, DISABLE);
	DMA1_Ch_Usart1_Tx->CNDTR = length;
	DMA1_Ch_Usart1_Tx->CMAR = (uint32_t)addres_8u;
	//USART1_TX_Finish = 0;//@@@@@@@@@
	DMA_Cmd(DMA1_Ch_Usart1_Tx, ENABLE);
}

void UartDMAQueue(UART_LinkQueue *q,uint8_t *addres_8u, uint8_t length)
{
	if (length > 0)
	{
		UART_ENQueue(q,addres_8u,length);
	}
	
}

////////////////////////////////////////////////////////////////////
////�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#if 1
//#pragma import(__use_no_semihosting)             
////��׼����Ҫ��֧�ֺ���                 
//struct __FILE 
//{ 
//	int handle; 
//	/* Whatever you require here. If the only file you are using is */ 
//	/* standard output using printf() for debugging, no file handling */ 
//	/* is required. */ 
//}; 
///* FILE is typedef�� d in stdio.h. */ 
//FILE __stdout;       
////����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//_sys_exit(int x) 
//{ 
//	x = x; 
//} 
////�ض���fputc���� 
//int fputc(int ch, FILE *f)
//{      
//	while((USART1->SR&0X40)==0);//ѭ������,ֱ��������� ,����uart�˿ڵ���
//	USART1->DR = (u8) ch;      
//	return ch;
//}
//#endif 
//
//Task start fisrt, then the remains send by the handle
void UartDmaSend(void)
{
	UART_QNode e;
	if (DmaFirstSend == 1)
	{		
		if (qUartLink->front != qUartLink->rear)
		{
			UART_DEQueue(qUartLink, &e);//����
			DMASendSet(e.Addr_8U,e.length);
			lastSend.Addr_8U = e.Addr_8U;
			lastSend.length = e.length;
		}
	}
}

//void TxPop(uint8_t len)
//{
//	TxBuffer2.out = (TxBuffer2.out + len)&0x7f;
//	//static uint8_t out=0, lastLen=0;
//	//TxBuffer2.out = (out + lastLen)&0x7f;
//	////lastAddr = addr;
//	//lastLen = len;
//}
//
//bool TxPush(uint8_t source[], uint8_t len)
//{
//
//}

//bool TxPush(uint8_t source[], uint8_t len)
//{
//	uint8_t i,j;
//	uint16_t k;
//	k=((TxBuffer2.in+TxFIFOSIZE+len) - TxBuffer2.out)&0x7f;
//	//ox7f relate to TxFIFOSIZE, measure used txbuff(2^n),
//	if (k < 0x7f)
//	{
//		j = TxBuffer2.in;
//		for(i=0; i< len; i++)
//		{
//			TxBuffer2.Txbuff[TxBuffer2.in++] = source[i];
//			TxBuffer2.in &=0x7f;
//		}
//		UartDMAQueue(qUartLink,&TxBuffer2.Txbuff[j],len);
//		return TRUE;
//	}
//	return FALSE;
//}
