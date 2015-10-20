#ifndef  __STm32_UB_LCD_DRIVE_H
#define  __STm32_UB_LCD_DRIVE_H

#include "stm32f10x.h"
//#include "stm32f10x_spi.h"


#define  USE_HARDWARE_SPI 1  //1:Enable Hardware SPI;0:USE Soft SPI
#define LCD_X_SIZE	        240
#define LCD_Y_SIZE	        320

//֧�ֺ��������ٶ����л�;
#define USE_HORIZONTAL  		1	//�����Ƿ�ʹ�ú��� 		0,��ʹ��.1,ʹ��.;


/* �������Խṹ, ����LCD_DispStr() */
typedef struct
{
	uint16_t usFontCode;	/* ������� 0 ��ʾ16���� */
	uint16_t usTextColor;	/* ������ɫ */
	uint16_t usBackColor;	/* ���ֱ�����ɫ��͸�� */
	uint16_t usSpace;		/* ���ּ�࣬��λ = ���� */
}FONT_T;

enum
{
	CL_WHITE        = 0xFFFF,	/* ��ɫ */
	CL_BLACK        = 0x0000,	/* ��ɫ */
	CL_GREY         = 0xF7DE,	/* ��ɫ */
	CL_BLUE         = 0x001F,	/* ��ɫ */
	CL_BLUE2        = 0x051F,	/* ǳ��ɫ */
	CL_RED          = 0xF800,	/* ��ɫ */
	CL_MAGENTA      = 0xF81F,	/* ����ɫ�����ɫ */
	CL_GREEN        = 0x07E0,	/* ��ɫ */
	CL_CYAN         = 0x7FFF,	/* ����ɫ����ɫ */
	CL_YELLOW       = 0xFFE0,	/* ��ɫ */
	CL_MASK					= 0x9999,	/* ��ɫ���룬�������ֱ���͸�� */
	CL_GRAY1   				=	0x8410,      	//��ɫ1      00000 000000 00000
	CL_GRAY2   				=	0x4208      	//��ɫ2  1111111111011111
};

enum
{
	FC_ST_16X16 = 0,		/* ����15x16���� ����x�ߣ� */
	FC_ST_24X24 = 1			/* ����24x24���� ����x�ߣ� */
};


#ifdef USE_HORIZONTAL//��������˺��� ;
#define X_MAX_PIXEL	        LCD_Y_SIZE
#define Y_MAX_PIXEL	        LCD_X_SIZE
#else
#define X_MAX_PIXEL	        LCD_X_SIZE
#define Y_MAX_PIXEL	        LCD_Y_SIZE
#endif


#define LCD_CTRL   	  	GPIOB		//����TFT���ݶ˿�
#define LCD_LED        	GPIO_Pin_1  //MCU_PB9--->>TFT --BL-pb1
#define LCD_RS         	GPIO_Pin_11	//PB10--->>TFT --RS/DC-pb11
//#define LCD_CS        	GPIO_Pin_11 //MCU_PB11--->>TFT --CS/CE  �ѽӵأ�
#define LCD_RST     	GPIO_Pin_12	//Pb12--->>TFT --RST
#define LCD_SCL        	GPIO_Pin_13	//Pb13--->>TFT --SCL/SCK
#define LCD_SDA        	GPIO_Pin_15	//Pb15 MOSI--->>TFT --SDA/DIN

//Һ�����ƿ���1�������궨�壻
//#define	LCD_CS_SET  	LCD_CTRL->BSRR=LCD_CS    
#define	LCD_RS_SET  	GPIOB->BSRR=LCD_RS    
#define	LCD_SDA_SET  	GPIOB->BSRR=LCD_SDA    
#define	LCD_SCL_SET  	GPIOB->BSRR=LCD_SCL    
#define	LCD_RST_SET  	GPIOB->BSRR=LCD_RST    
#define	LCD_LED_SET  	GPIOB->BSRR=LCD_LED 

//Һ�����ƿ���0�������궨�壻
//#define	LCD_CS_CLR  	LCD_CTRL->BRR=LCD_CS    
#define	LCD_RS_CLR  	GPIOB->BRR=LCD_RS    
#define	LCD_SDA_CLR  	GPIOB->BRR=LCD_SDA    
#define	LCD_SCL_CLR  	GPIOB->BRR=LCD_SCL    
#define	LCD_RST_CLR  	GPIOB->BRR=LCD_RST    
#define	LCD_LED_CLR  	GPIOB->BRR=LCD_LED 

// #define LCD_DATAOUT(x) LCD_DATA->ODR=x; //�������;
// #define LCD_DATAIN     LCD_DATA->IDR;   //��������;


extern void Lcd_Init(void);  //lcd��ʼ������
extern void Lcd_SetXY(u16 Xpos, u16 Ypos);
extern void Gui_DrawPoint(u16 x,u16 y,u16 Data);
extern void Lcd_Clear(u16 color);

/*
#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
LCD_DATAOUT(data);\
LCD_WR_CLR;\

}
*/

/*
#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//��ɫ0 3165 00110 001011 00101
#define GRAY1   0x8410      	//��ɫ1      00000 000000 00000
#define GRAY2   0x4208      	//��ɫ2  1111111111011111


#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define MAGENTA       	 0xF81F
#define CYAN          	 0x7FFF
#define BROWN 			 0XBC40 //��ɫ;
#define BRRED 			 0XFC07 //�غ�ɫ;
#define GRAY  			 0X8430 //��ɫ;
//GUI��ɫ;

#define DARKBLUE      	 0X01CF	//����ɫ;
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  ;
#define GRAYBLUE       	 0X5458 //����ɫ;
//������ɫΪPANEL����ɫ ;

#define LIGHTGREEN     	 0X841F //ǳ��ɫ;
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL);
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ;

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ);
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ);

*/


#endif


