#ifndef _ADS1118_H_
#define _ADS1118_H_

#include "sys.h"

#define ADS1118_CS(n)		  (n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET))
#define ADS1118_SCLK(n)		(n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET))
#define ADS1118_DIN(n)		(n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET))
#define READ_SDO          HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //输入SDA

#define CRM  (SS|MUX0|PGA1|MODE)//8位
#define CRL  (DR7|TS_MODE_1|PLL_UP_EN|NOP0|STAY)
#define FS   2.048*2 //Fullscale

//bit 15
#define SS         0x8000  //启动单次转换
//bit 14-12
#define MUX0_1 	   0X0000//差分输入，前一个是ANIP，后一个是ANIN
#define MUX0_3 	   0X1000
#define MUX1_3 	   0X2000
#define MUX2_3 	   0X3000
#define MUX0       0X4000  //单端模式 AIN0-AIN3
#define MUX1       0X5000
#define MUX2       0X6000
#define MUX3       0X7000
//bit 11-9
#define PGA1       0X0200  //4.096V
#define PGA2       0X0400  //2.048V
#define MODE       0x0100  //单次转换模式
//bit 7-5
#define DR0        0x0000  //转换速率 8SPS 
#define DR1        0x0020  //转换速率 16SPS
#define DR2        0x0040  //转换速率 32SPS
#define DR3  	   0x0060  //转换速率 64SPS
#define DR4		   0x0080  //转换速率 128SPS,default
#define DR5		   0x00A0  //转换速率 250SPS
#define DR6 	   0x00C0  //转换速率 475SPS
#define DR7		   0x00E0  //转换速率 860SPS
//bit 4
#define TS_MODE    0X0010  //温度传感器模式
#define TS_MODE_1  0X0000  //ADC模式
//bit 3 
#define PLL_UP_EN  0x0008  //上拉使能
//bit 2-1
#define NOP0       0X0002  //有效数据+更新控制寄存器
#define NOP1       0x0000  //无有效数据
//bit 0
#define STAY       0x0001  //最后一位始终为1,只读，写1或0没关系

typedef struct
{
	int MUX;//输入模式
	int DR;//采样速率
	int PGA;//电压范围
	int TS;//选择模式
	double Fullscale;//两倍PGA
	int myfft;
}ADS1118_para;	
void ADS1118_GPIO_Init(void);
void ADS1118_Init(void);
u8 ADS1118_Write_Read(u8 data);
float ADS1118_Get_Voltage(void);


/**************************************************************
如果是本人使用时，先在程序初始调用ad_init()
ad_init()会检查是否连接上
之后，如果需要更改参数，只需要更改哪一个值之后调用ADS1118_fun()即可
***************************************************************/
void para_init(ADS1118_para *s);
int ad_init(ADS1118_para *s);
float ADS1118_fun(ADS1118_para *s);

#endif
