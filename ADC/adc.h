#ifndef __ADC_H
#define __ADC_H
#include "sys.h"
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

void MY_ADC_Init(void); 				//ADCͨ����ʼ��
u16  Get_Adc(u32 ch); 		        //���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u32 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ
short Get_Temprate(void);
void T_test(void);
u16  Get_Adc1(u32 ch,u32 speed);

u16 Get_Adc_3(void);
u16 Get_Adc_15(void);
u16 Get_Adc_28(void);
u16 Get_Adc_56(void);
u16 Get_Adc_84(void);
u16 Get_Adc_112(void);
u16 Get_Adc_144(void);
u16 Get_Adc_480(void); 
#endif 
