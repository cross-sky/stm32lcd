#ifndef __MPU6050M_H
#define __MPU6050M_H

#include "stm32f10x.h"

//#define FALSE 0
//#define TRUE 1

#define SDA_H GPIOB->BSRR = GPIO_Pin_5
#define SDA_L GPIOB->BRR = GPIO_Pin_5

#define SCL_H GPIOB->BSRR = GPIO_Pin_6
#define SCL_L GPIOB->BRR = GPIO_Pin_6

#define SDA_READ (GPIOB->IDR & GPIO_Pin_5)

// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz);
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz);
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s);
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz);

#define INT_PIN_CFG     0x37    //������·��Ч ��ֵ��0x42 AUX_DA�ĸ���I2C;
#define USER_CTRL       0x6A    //�û����üĴ��� ��ֵ��0x40  AUX_DA�ĸ���I2C;

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������);
//#define	WHO_AM_I		0x75	//IIC��ַ�Ĵ���(Ĭ�϶�����ֵ0x68��ֻ��);
//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ;
#define	MPU6050_Addr   0xD0	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�;

typedef enum {FALSE = 0, TRUE = 1} bool;

extern char  test; 				 //IIC�õ�
extern float tTemp[7];
extern bool mpu_read_xyz(u8 slave_address);
extern void mpu6050m_init(void );
//extern void i2c1_gpio_config(void);
extern u8 single_read(u8 slave_address, u8 reg_address);
void MpuRead(void);
#endif



