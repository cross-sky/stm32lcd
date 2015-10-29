#include "cominc.h"

//buffet 16bytes 16*8=128
uint16_t lcd_buffer[8]={0};
char s[16];
const uint16_t LCDNumCode[]={0x00af,0x0006,0x006d,0x004f,0x00c6,0x00cb,0x00eb,0x000e,0x00ef,0x00cf,0x40};

static void lcd_delay( void)
{
	uint8_t i=2;
	while ( i-- );
}

void lcdportInit(void)
{
	GPIO_InitTypeDef gpio_init_structure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	DBGMCU->CR &= ~(uint32_t)DBGMCU_CR_TRACE_IOEN;
	
	gpio_init_structure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3;
	gpio_init_structure.GPIO_Speed = GPIO_Speed_10MHz;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_Init(GPIOB,&gpio_init_structure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	gpio_init_structure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_6; // beep and bgled
	gpio_init_structure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&gpio_init_structure);
	LCD_LED_ON;
}

//�������������HT1621Bд��һ�����ݣ��û�������Ҫ���������ӱ�Ҫ����ʱ
void lcd_send_command(uint8_t command)
{
	uint8_t i;
	LCD_CS_L;

	LCD_WR_L; //PRESENT 100 COMMAND CODE
	LCD_DA_H;
	LCD_WR_H;

	LCD_WR_L;
	LCD_DA_L;
	LCD_WR_H;

	LCD_WR_L;
	LCD_WR_H;
	
	for (i=0; i<8; i++)
	{
		LCD_WR_L;
		if ((command&0x80) != 0)
		{
			LCD_DA_H;
		}
		else
			LCD_DA_L;

		LCD_WR_H;
		command = command << 1;
	}

	LCD_WR_L;
	LCD_WR_H;

	LCD_CS_H;
}

//�������������HT1621Bд��һ�����ݣ��û�������Ҫ���������ӱ�Ҫ����ʱ
void lcd_send_data(uint8_t address, uint8_t data)
{
	uint8_t i;
	LCD_CS_L;

	LCD_WR_L;		//PRESENT 101 DATA CODE
	LCD_DA_H;
	LCD_WR_H;

	LCD_WR_L;
	LCD_DA_L;
	LCD_WR_H;

	LCD_WR_L;
	LCD_DA_H;
	LCD_WR_H;

	address = address << 2;
	for (i=0; i<6; i++)
	{
		LCD_WR_L;
		if ((address&0x80) != 0 )
		{
			LCD_DA_H;
		}
		else
			LCD_DA_L;
		address = address << 1;
	}

	for (i=0; i<4; i++)
	{
		LCD_WR_L;
		if ((data & 0x01) != 0)
		{
			LCD_DA_H;
		}
		else
			LCD_DA_L;
		data = data >> 1;
	}

	LCD_CS_H;
}

//����������Խ���ʾ������ˢ�µ�HT1621B���û�������Ҫ���������ӱ�Ҫ����ʱ
void lcd_flood_data(void)
{
	uint16_t temp_data;
	uint8_t i,j;
	LCD_CS_L;

	LCD_WR_L;
	LCD_DA_H;
	LCD_WR_H;

	LCD_WR_L;
	LCD_DA_L;
	LCD_WR_H;

	LCD_WR_L;
	LCD_DA_H;
	LCD_WR_H;
	//a0-a5 addr 0x00
	for (i=0; i<6; i++)
	{
		LCD_WR_L;
		LCD_DA_L;
		LCD_WR_H;
	}

	for (i=0; i<8; i++)
	{
		temp_data = lcd_buffer[i];
		for (j=0;j<16; j++)
		{
			LCD_WR_L;
			if ((temp_data & 0x01) != 0)
			{
				LCD_DA_H;
			}
			else 
				LCD_DA_L;

			LCD_WR_H;
			temp_data = temp_data>>1; 
		}
	}

	LCD_CS_H;
}

//init ht1621b
void Lcd1621Init(void)
{
	
	uint8_t len;
	lcdportInit();
	lcd_send_command(0x29);//1/2bias��4comm 0x29 1/3bias
	lcd_send_command(0x03);//�����ڲ�����
	lcd_send_command(0x01);//����ʾ

#ifdef Debug
	sprintf(s, "%s","lcd on...\r\n");
	len = strlen(s);
	UartDMAQueue(qUartLink,(uint8_t*)s,len);
	//printf("lcd on...\r\n");
#endif
}

void lcd1621test(void)
{
	static uint8_t i=0,j=0;

	//lcd_clear();
	if (i>9)
	{
		i=0;
		lcd_wr_num(i, j, 0);
		j++;
		if (j>2)
		{
			j=0;
		}		
	}
	lcd_wr_num(i, j, 1);
	//lcd_flood_data();
	i++;


}
//��Һ��дһ������
//name����ֱ��д0~127��������Ҳ����д����ʼ����ı�ţ��磺_lcd_BUSY
//display������
void lcd_wr_char(uint8_t name, uint8_t display)
{
	uint8_t i,j;
	i=name/16;
	j=name%16;

	if (display == 1)
	{
		lcd_buffer[i] |= 1<<j;
	}
	else
		lcd_buffer[i] &= ~(1<<j);

//	lcd_flood_data();
}

void lcd_clear(void)
{
	uint8_t i;
	for(i=0; i<8; i++)
		lcd_buffer[i]=0;

}

//��Һ������λ��дһ��0~9����
//��λΪ�������������Ϊ0~5��С�����������Ϊ6~9��
//number������ʾ���� 0~9
//position����ʾ����λ
//display����ʾ���أ�0 �رո���λ��ʾ����ʱnumberֵ��Ч����>=1 ��ʾ����λ����


void lcd_wr_num(uint8_t number, uint8_t position, uint8_t display)
{
	switch(position)
	{
	case 0:
		{	//p1
			lcd_buffer[2] &= 0xfff1;
			lcd_buffer[1] &= 0x0fff;

			if(display == 0) 
				break;
			//case 0:lcd_buffer[2] |= 0x000a; lcd_buffer[1] |= 0xf000; break;
			lcd_buffer[2] |= (LCDNumCode[number]>>4);
			lcd_buffer[1] |= (LCDNumCode[number]<<12);
			break;
		}

	case 1:
		{	//p2
			lcd_buffer[1] &= 0xf10f;

			if(display == 0) 
				break;
			//case 0:lcd_buffer[1] |= 0x0af0; break;
			lcd_buffer[1] |= (LCDNumCode[number]<<4);
			break;
		}

	case 2:
		{	
			//p3
			lcd_buffer[1] &= 0xfff1;
			lcd_buffer[0] &= 0x0fff;

			if(display == 0) 
				break;
			//case 0:lcd_buffer[1] |= 0x000a; lcd_buffer[0] |= 0xf000; break;
			lcd_buffer[1] |= (LCDNumCode[number]>>4);
			lcd_buffer[0] |= (LCDNumCode[number]<<12);
			break;
		}

	case 3:
		{
			//p4
			lcd_buffer[5] &= 0x10ff;

			if(display == 0) 
				break;
			//case 0:lcd_buffer[5] |= 0xaf00; break;
			lcd_buffer[5] |= (LCDNumCode[number]<<8);
			break;
		}

	case 4:
		{
			//p5
			lcd_buffer[5] &= 0xff10;

			if(display == 0) 
				break;
			//case 0:lcd_buffer[5] |= 0x00af; break;
			lcd_buffer[5] |= LCDNumCode[number];
			break;
		}

	case 5:
		{
			//p6
			lcd_buffer[4] &= 0x10ff;

			if(display == 0) 
				break;

			//case 0:lcd_buffer[4] |= 0xaf00; break;
			lcd_buffer[4] |= (LCDNumCode[number]<<8);
			break;
		}

	case 6:
		{
			//p7
			lcd_buffer[4] &= 0xfff1;
			lcd_buffer[3] &= 0x0fff;

			if(display == 0) 
				break;

			//case 0:lcd_buffer[4] |= 0x000a; lcd_buffer[3] |= 0xf000; break;
			lcd_buffer[4] |= (LCDNumCode[number]>>4);
			lcd_buffer[3] |= (LCDNumCode[number]<<12);
			break;
		}

	case 7:
		{
			//p8
			lcd_buffer[3] &= 0xf10f;

			if(display == 0) 
				break;
			//case 0:lcd_buffer[3] |= 0x0af0; break;
			lcd_buffer[3] |= (LCDNumCode[number]<<4);
			break;
		}

	case 8:
		{
			//p9
			lcd_buffer[3] &= 0xfff1;
			lcd_buffer[2] &= 0x0fff;

			if(display == 0) 
				break;
			//case 0:lcd_buffer[3] |= 0x000a; lcd_buffer[2] |= 0xf000; break;
			lcd_buffer[3] |= (LCDNumCode[number]>>4);
			lcd_buffer[2] |= (LCDNumCode[number]<<12);
			break;
		}

	case 9:
		{
			//p10
			lcd_buffer[2] &= 0xf10f;

			if(display == 0) 
				break;
			//case 0:lcd_buffer[2] |= 0x0af0; break;
			lcd_buffer[2] |= (LCDNumCode[number]<<4);
			break;
		}

	default: break;
	}
}

