#include "ADS1118.h"
#include "delay.h"
#include "usart.h" 


void ADS1118_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
  __HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT ;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
//	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);
}

void ADS1118_Init()
{
	float k;
	ADS1118_GPIO_Init();
	ADS1118_SCLK(0); //空闲时低电平
	ADS1118_CS(1);//空闲时高电平
	delay_ms(10);
	ADS1118_DIN(0);//准备数据
	delay_us(10);
	ADS1118_CS(0);
	k = ADS1118_Get_Voltage();
    printf("ADS1118第一次测试数据:%f\n",k);
}

u8 ADS1118_Write_Read(u8 data)
{
	u8 i;
	u8 temp;
	u16 read_data;
	temp=data;
	for(i=0;i<8;i++)
	{
		read_data<<=1;
		if(temp&0x80)
			ADS1118_DIN(1);
		else 
			ADS1118_DIN(0);
	
		delay_us(30);
		ADS1118_SCLK(1);
		delay_us(30);	  
		if(READ_SDO)
			read_data|=0x01;
		delay_us(30);
		ADS1118_SCLK(0);
		delay_us(30);
		temp<<=1;
	}
	
  return read_data;

}

//ADS1118采样电压
float ADS1118_Get_Voltage(void)
{
	float ADS1118_Voltage;
	u32 i=0;
	u8 Data_H,Data_L,Reg_H,Reg_L;
	u16 Data,Reg;
	while((READ_SDO)&&(i<1000))
	{
		i++;
		delay_ms(1);
	}

//	printf("%x\r\n",CRM);
	Data_H=ADS1118_Write_Read((u8)(CRM>>8));
//	printf("%x\r\n",CRM);
	Data_L=ADS1118_Write_Read((u8)CRL);
//	printf("%x\r\n",CRL);
	//printf("233\r\n");
	Reg_H=ADS1118_Write_Read(0x00);
	Reg_L=ADS1118_Write_Read(0x00);
	
	Data=(Data_H<<8)+Data_L;
//	printf("%x\r\n",Data);
	Reg=(Reg_H<<8)+Reg_L;
	printf("%x\r\n",Reg);
	if(Data>=0x8000)
	{//3.1V 差47 2.1V 差39 
		Data=0x10000-Data;
		//		printf("%d\n",Data);
		ADS1118_Voltage=(-1.0)*(Data*FS/32768);
	}
	else
	{	
		ADS1118_Voltage=(1.0)*(Data*FS/32768);
	 // 	printf("%d\n",Data);
	}
	return ADS1118_Voltage;
}
//温度传感器
float ADS1118_Temperature=25.9;
void ADS1118_Get_Temperature(void)
{
	u16 i=200;
	u8  Data_REG_H,Data_REG_L;
	u16 Data_REG;
	while((READ_SDO)&&(i<200)) i++;
	Data_REG_H=ADS1118_Write_Read(0x83);
	Data_REG_L=ADS1118_Write_Read(0xFB);
	Data_REG=(Data_REG_H<<8)+Data_REG_L;
	ADS1118_Write_Read(0x83);
	ADS1118_Write_Read(0xFB);
	ADS1118_Temperature=(Data_REG>>2)*0.03125;

}


//学长给的，我觉得一般，不是很好用，集成化太低
/*
参数：ADS1118_para
作用：用来初始化配置para的参数
*/
void para_init(ADS1118_para *s)
{
	s->DR=DR7;//采样速率 一共有七档
	s->PGA=PGA1;//电压范围
	s->MUX=MUX0;//输入模式
	s->myfft=0;//是否激活自写的myfft
	s->TS=TS_MODE_1;//adc
	//自动确定满量程
	if(s->PGA==PGA1)
		s->Fullscale=2*4.096;
	else if(s->PGA==PGA2)
		s->Fullscale=2*2.048;
}
/*
参数：ADS1118_para
作用：在程序中更改ADS1118的参数
使用方法：
	while(ad_init(&s)!=1);//如果配置不成功则一直循环
*/
int ad_init(ADS1118_para *s)
{
	u8 data_H,data_L,check_h,check_l;
	u32 i;
	para_init(s);
	ADS1118_GPIO_Init();
	ADS1118_SCLK(0); //空闲时低电平
	ADS1118_CS(1);//空闲时高电平
	delay_ms(10);
	ADS1118_DIN(0);//准备数据
	delay_us(10);
	ADS1118_CS(0);//这一步应该拉到通信协议哪里，为了日后开发方便，留作片选
	while((READ_SDO)&&(i<1000))
	{
		i++;
		delay_ms(1);
	}
	data_H=(u8)((SS|s->MUX|s->PGA|MODE)>>8);
	data_L=(u8)((s->DR|s->TS|PLL_UP_EN|NOP0|STAY));
	check_h=ADS1118_Write_Read(data_H);
	check_l=ADS1118_Write_Read(data_L);
	check_h=ADS1118_Write_Read(0x00);
	check_l=ADS1118_Write_Read(0x00);
	if ((check_h==data_H)&&(check_l==data_L))
	{
		printf("config OK!");
		return 1;
	}
	else
	{
		printf("config error!");
		return 0;
	}
}
//ADS1118
/*********
参数：ADS1118_para
作用：通过上一步的ad_init配置，自动返回选择的功能函数的值，
只用这个函数即可完成温度和AD采样,注意,如果遇到故障会自动报错
**********/
float ADS1118_fun(ADS1118_para *s)
{
	float ADS1118_Voltage;
	u32 i=0;
	u8 Data_H,Data_L;
	u16 Data;
	float ADS1118_Temperature=25.9;
	while((READ_SDO)&&(i<1000))
	{
		i++;
		delay_ms(1);
	}
	if (s->TS==TS_MODE_1)//ADC模式
	{	
		if(s->myfft==0)
		{
			Data_H=ADS1118_Write_Read((u8)((SS|s->MUX|s->PGA|MODE)>>8));
			Data_L=ADS1118_Write_Read((u8)((s->DR|s->TS|PLL_UP_EN|NOP0|STAY)));
			Data=(Data_H<<8)+Data_L;
			if(Data>=0x8000)//负电压
			{//3.1V 差47 2.1V 差39 
				Data=0x10000-Data;
				//		printf("%d\n",Data);
				ADS1118_Voltage=(-1.0)*(Data*s->Fullscale/32768);
			}
			else
			{	
				ADS1118_Voltage=(1.0)*(Data*s->Fullscale/32768);
			 // 	printf("%d\n",Data);
			}
			return ADS1118_Voltage;
		}	
		else if(s->myfft==1)//为我自己写的FFT函数准备东西
		{
			Data_H=ADS1118_Write_Read((u8)((SS|s->MUX|s->PGA|MODE)>>8));
			Data_L=ADS1118_Write_Read((u8)((s->DR|s->TS|PLL_UP_EN|NOP0|STAY)));
			Data=(Data_H<<8)+Data_L;
			if(Data>=0x8000)//负电压
			{//3.1V 差47 2.1V 差39 
				Data=0x10000-Data;
				//		printf("%d\n",Data);
				ADS1118_Voltage=(-1.0)*(Data/32768);
			}
			else
			{	
				ADS1118_Voltage=(1.0)*(Data/32768);
			 // 	printf("%d\n",Data);
			}
			return ADS1118_Voltage;
		}
	}
	else if(s->TS==TS_MODE)//温度传感器模式
	{
		Data_H=ADS1118_Write_Read(0x83);
		Data_L=ADS1118_Write_Read(0xFB);
		Data=(Data_H<<8)+Data_L;
		ADS1118_Write_Read(0x83);
		ADS1118_Write_Read(0xFB);
		ADS1118_Temperature=(Data>>2)*0.03125;
		return ADS1118_Temperature;
	}
	else
	{
		printf("error!");
		
	}
	return 0;
}




