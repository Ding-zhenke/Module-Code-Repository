#include "adc.h"
#include "delay.h"
#include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//ADC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/26
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

ADC_HandleTypeDef ADC1_Handler;//ADC���

//��ʼ��ADC
//ch: ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
void MY_ADC_Init(void)
{
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;  //4��Ƶ��ADCCLK=PCLK2/4=108/4=27MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;            //12λģʽ
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;            //�Ҷ���
    ADC1_Handler.Init.ScanConvMode=DISABLE;                     //��ɨ��ģʽ
    ADC1_Handler.Init.EOCSelection=DISABLE;                     //�ر�EOC�ж�
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;               //�ر�����ת��
    ADC1_Handler.Init.NbrOfConversion=1;                        //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;            //��ֹ����������ģʽ
    ADC1_Handler.Init.NbrOfDiscConversion=0;                    //����������ͨ����Ϊ0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;      //�������
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;            //�ر�DMA����
    HAL_ADC_Init(&ADC1_Handler);                                //��ʼ��     
}

//ADC�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_ADC_Init()����
//hadc:ADC���
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_5;            //PA5
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //ͨ��
    ADC1_ChanConf.Rank=1;                                       //1������
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //����ʱ��
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
	
    HAL_ADC_Start(&ADC1_Handler);                               //����ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //��ѯת��
   
	return (u16)HAL_ADC_GetValue(&ADC1_Handler);	            //�������һ��ADC1�������ת�����
}

u16 Get_Adc1(u32 ch,u32 speed)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //ͨ��
    ADC1_ChanConf.Rank=1;                                       //1������
    ADC1_ChanConf.SamplingTime=speed;        //����ʱ��
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
	
    HAL_ADC_Start(&ADC1_Handler);                               //����ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //��ѯת��
   
	return (u16)HAL_ADC_GetValue(&ADC1_Handler);	            //�������һ��ADC1�������ת�����
}
//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��,��λ:��.)
short Get_Temprate(void)
{
	u32 adcx;
	short result;
 	double temperate;
	adcx=Get_Adc_Average(ADC_CHANNEL_TEMPSENSOR,10);//��ȡ�ڲ��¶ȴ�����ͨ��,10��ȡƽ��
	temperate=(float)adcx*(3.3/4096);		//��ѹֵ
	temperate=(temperate-0.76)/0.0025 + 25; //ת��Ϊ�¶�ֵ 
	result=temperate*=100;					//����100��.
	return result;
}
double fre(int f,u32 ch)
{		
	u32 speed;
	if(f<=10800)
	{
		speed=ADC_SAMPLETIME_480CYCLES;
	}
	else if(f<=34000&&f>10800)
	{
		speed=ADC_SAMPLETIME_144CYCLES;
	}
	else if(f>34000&&f<=43400)
	{
		speed=ADC_SAMPLETIME_112CYCLES;
	}
	else if(f>43400&&f<=56200)
	{
		speed=ADC_SAMPLETIME_84CYCLES;
	}
	else if(f>56200&&f<=79400)
	{
		speed=ADC_SAMPLETIME_56CYCLES;
	}
	else if(f>79400&&f<=135000)
	{
		speed=ADC_SAMPLETIME_28CYCLES;
	}
	else if(f>135000&&f<=200000)
	{
		speed=ADC_SAMPLETIME_15CYCLES;
	}
	else if(f>200000)
	{
		speed=ADC_SAMPLETIME_3CYCLES;
	}
	return Get_Adc1(ch,speed)*(double)(3.29)/4095;
}

