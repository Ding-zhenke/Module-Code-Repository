#ifndef _ADS1118_H_
#define _ADS1118_H_

#include "sys.h"

#define ADS1118_CS(n)		  (n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET))
#define ADS1118_SCLK(n)		(n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET))
#define ADS1118_DIN(n)		(n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET))
#define READ_SDO          HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //����SDA

#define CRM  (SS|MUX0|PGA1|MODE)//8λ
#define CRL  (DR7|TS_MODE_1|PLL_UP_EN|NOP0|STAY)
#define FS   2.048*2 //Fullscale

//bit 15
#define SS         0x8000  //��������ת��
//bit 14-12
#define MUX0_1 	   0X0000//������룬ǰһ����ANIP����һ����ANIN
#define MUX0_3 	   0X1000
#define MUX1_3 	   0X2000
#define MUX2_3 	   0X3000
#define MUX0       0X4000  //����ģʽ AIN0-AIN3
#define MUX1       0X5000
#define MUX2       0X6000
#define MUX3       0X7000
//bit 11-9
#define PGA1       0X0200  //4.096V
#define PGA2       0X0400  //2.048V
#define MODE       0x0100  //����ת��ģʽ
//bit 7-5
#define DR0        0x0000  //ת������ 8SPS 
#define DR1        0x0020  //ת������ 16SPS
#define DR2        0x0040  //ת������ 32SPS
#define DR3  	   0x0060  //ת������ 64SPS
#define DR4		   0x0080  //ת������ 128SPS,default
#define DR5		   0x00A0  //ת������ 250SPS
#define DR6 	   0x00C0  //ת������ 475SPS
#define DR7		   0x00E0  //ת������ 860SPS
//bit 4
#define TS_MODE    0X0010  //�¶ȴ�����ģʽ
#define TS_MODE_1  0X0000  //ADCģʽ
//bit 3 
#define PLL_UP_EN  0x0008  //����ʹ��
//bit 2-1
#define NOP0       0X0002  //��Ч����+���¿��ƼĴ���
#define NOP1       0x0000  //����Ч����
//bit 0
#define STAY       0x0001  //���һλʼ��Ϊ1,ֻ����д1��0û��ϵ

typedef struct
{
	int MUX;//����ģʽ
	int DR;//��������
	int PGA;//��ѹ��Χ
	int TS;//ѡ��ģʽ
	double Fullscale;//����PGA
	int myfft;
}ADS1118_para;	
void ADS1118_GPIO_Init(void);
void ADS1118_Init(void);
u8 ADS1118_Write_Read(u8 data);
float ADS1118_Get_Voltage(void);


/**************************************************************
����Ǳ���ʹ��ʱ�����ڳ����ʼ����ad_init()
ad_init()�����Ƿ�������
֮�������Ҫ���Ĳ�����ֻ��Ҫ������һ��ֵ֮�����ADS1118_fun()����
***************************************************************/
void para_init(ADS1118_para *s);
int ad_init(ADS1118_para *s);
float ADS1118_fun(ADS1118_para *s);

#endif
