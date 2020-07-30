/**********************************************************								 
功能：ADF4351正弦波点频输出，范围35M-4400M，步进0.1M
**********************************************************/
#include "ADF4351.h"
#include "delay.h"

//#define 
#define ADF4351_R0			((u32)0X2C8018)
#define ADF4351_R1			((u32)0X8029)
#define ADF4351_R2			((u32)0x64e42)//0x10e42
#define ADF4351_R3			((u32)0X4B3)
#define ADF4351_R4			((u32)0XEC803C)//EC803C
#define ADF4351_R5			((u32)0X580005)

#define ADF4351_R1_Base	((u32)0X8001)
#define ADF4351_R4_Base	((u32)0X8C803C)
#define ADF4351_R4_ON	  ((u32)0X8C803C)
#define ADF4351_R4_OFF	((u32)0X8C883C)

//#define ADF4351_RF_OFF	((u32)0XEC801C)

#define ADF4351_PD_ON		((u32)0X10E42)
#define ADF4351_PD_OFF	((u32)0X10E02)


void ADF_Output_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOG_CLK_ENABLE();			//开启GPIOH时钟
	
    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13; //PH8,9,10,11
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
	
}

void ADF_Input_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOG_CLK_ENABLE();			//开启GPIOH时钟
	
    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
	
}

void delay (int length)
{
	while (length >0)
    	length--;
}

void WriteToADF4351(u8 count, u8 *buf)
{
	u8 ValueToWrite = 0;
	u8 i = 0;
	u8 j = 0;
	
	ADF_Output_GPIOInit();
	
	ADF4351_CE;
	delay_us(1);
	ADF4351_CLK_L;
	ADF4351_LE_L;
	delay_us(1);
	
	for(i = count; i>0; i--)
	{
		ValueToWrite = *(buf+i-1);
		for(j=0; j<8; j++)
		{
			if(0x80 == (ValueToWrite & 0x80))
			{
				ADF4351_OUTPUT_DATA;
			}
			else
			{
				ADF4351_INPUT_DATA;
			}
			delay_us(1);
			ADF4351_CLK;
			delay_us(1);
			ValueToWrite <<= 1;
			ADF4351_CLK_L;	
		}
	}
	ADF4351_INPUT_DATA;
	delay_us(1);
	ADF4351_LE;
	delay_us(1);
	ADF4351_LE_L;
}


void ReadToADF4351(u8 count, u8 *buf)
{
	u8 i = 0;
	u8 j = 0;
	u8 iTemp = 0;
	u8 RotateData = 0;
	
	ADF_Input_GPIOInit();
	ADF4351_CE;
	delay_us(1);
	ADF4351_CLK_L;
	ADF4351_LE_L;
	delay_us(1);
	
	for(i = count; i>0; i--)
	{
		for(j = 0; j<8; j++)
		{
			RotateData <<=1;
			delay_us(1);
			ADF4351_INPUT_DATA;
			iTemp =0; 
			ADF4351_CLK;
			if(0x01 == (iTemp&0x01))
			{
				RotateData |= 1;
			}
			delay_us(1);
			ADF4351_CLK_L;
		}
		*(buf+i-1) = RotateData;
	}
	delay_us(1);
	ADF4351_LE;
	delay_us(1);
	ADF4351_LE_L;
}


void ADF4351Init(void)
{
	u8 buf[4] = {0,0,0,0};
	
	ADF_Output_GPIOInit();
	
	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress 
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4351(4,buf);		

	buf[3] = 0x00;
	buf[2] = 0xec;				//(DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 16;(DB19-12:50H)R is 80
	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4351(4,buf);		

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
 	buf[0] = 0xB3;
	WriteToADF4351(4,buf);	

	buf[3] = 0x00;
	buf[2] = 0x01;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x0E;				//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4351(4,buf);		//(DB23-14:1H)R counter is 1

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x80;			   //(DB14-3:6H)MOD counter is 6;
 	buf[0] = 0x29;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync 
	WriteToADF4351(4,buf);	   //nor the spurious optimization functions are being used
							   //(DB27=1)prescaler value is 8/9

	buf[3] = 0x00;
	buf[2] = 0x2c;
	buf[1] = 0x80;
 	buf[0] = 0x18;				//(DB14-3:0H)FRAC value is 0;
	WriteToADF4351(4,buf);		//(DB30-15:140H)INT value is 320;
}
void WriteOneRegToADF4351(u32 Regster)
{
	u8 buf[4] = {0,0,0,0};
	buf[3] = (u8)((Regster>>24)&(0X000000FF));
	buf[2] = (u8)((Regster>>16)&(0X000000FF));
	buf[1] = (u8)((Regster>>8) &(0X000000FF));
	buf[0] = (u8)((Regster)&(0X000000FF));
	WriteToADF4351(4,buf);	
}
void ADF4351_Init_some(void)
{
	WriteOneRegToADF4351(ADF4351_R2);
	WriteOneRegToADF4351(ADF4351_R3);
	WriteOneRegToADF4351(ADF4351_R5);
}

void ADF4351WriteFreq(float Fre)		//	fre单位MHz -> (xx.x) M Hz
{
	u16 INT_Fre=0, Frac_temp=0, Mod_temp=0,div=1;
	u32 W_ADF4351_R0 = 0, W_ADF4351_R1 = 0, W_ADF4351_R4 = 0;
	float count,compare;
	
	
	
	if(Fre < 35.0)
		Fre = 35.0;
	if(Fre > 4400.0)
		Fre = 4400.0;
	
	
	if(Fre>=2200)//通过频段选择RFdivider
	{
		div=1;
	}
	else if(Fre<2200&&Fre>=1100)//通过频段选择RFdivider
	{
		div=2;
	}
	else if(Fre<1100&&Fre>=550)
	{
		div=4;
	}		
	else if(Fre<550&&Fre>=275)
	{
		div=8;
	}	
	else if(Fre<275&&Fre>=137.5)
	{
		div=16;
	}	
	else if(Fre<137.5&&Fre>=68.75)
	{
		div=32;
	}	
	else if(Fre<68.75&&Fre>=34)
	{
		div=64;
	}	
	//div=64;
	
	
	INT_Fre=(u16)(Fre*div);
	printf("INT:%d\r\n",INT_Fre);
	count=(float)(Fre*(float)(div)-(float)(INT_Fre));
	for(Mod_temp = 1000;Mod_temp<4096;Mod_temp++)
	{
		for(Frac_temp=0;Frac_temp<Mod_temp;Frac_temp++)
		{
			compare=(float)(Frac_temp)/(float)(Mod_temp)-count;
			if(compare<0)
			{
				compare=-compare;
			}				
			if(compare<0.00001)
			{
				break;
			}
		}
		if(compare<0.00001)
		{
			break;
		}	
	}
	printf("FRAC:%d\r\n",Frac_temp);
	
	
	/*
	W_ADF4351_R0 = ;
	W_ADF4351_R1 = 0X ;
	W_ADF4351_R4 = 0X8C803C;
	*/
	
	/*
	printf("R0:%X\r\n",W_ADF4351_R0);
	printf("R1:%X\r\n",W_ADF4351_R1);
	printf("R4:%X\r\n",W_ADF4351_R4);
	*/
	WriteOneRegToADF4351(0x580005);//R5,不变
	WriteOneRegToADF4351(0xE0803C + (div<<20));//R4
	WriteOneRegToADF4351(0x4B3);//R3不变
	WriteOneRegToADF4351(0x64E42);//R2不变
	WriteOneRegToADF4351(0x8001+ (Mod_temp<<3));//R1
	WriteOneRegToADF4351((INT_Fre<<15)+(Frac_temp<<3));//R0
}
