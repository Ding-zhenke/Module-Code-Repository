#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "adc.h"
/************************************************
 ALIENTEK ������STM32F7������ ʵ��20
 ADCģ��ת��ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
u16 adcx;
extern ADC_HandleTypeDef ADC1_Handler;//ADC���
int main(void)
{
    Cache_Enable();                 //��L1-Cache
    HAL_Init();				        //��ʼ��HAL��
    Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
    delay_init(216);                //��ʱ��ʼ��
	uart_init(115200);		        //���ڳ�ʼ��
    LED_Init();                     //��ʼ��LED
	MY_ADC_Init();                  //��ʼ��ADC1ͨ��5	
	HAL_ADC_Stop_IT(&ADC1_Handler);
    
    while(1)
	{
		printf("\r\n");
        printf("����������\r\n");
		LED0_Toggle;		
		while(HAL_ADC_Start_IT(&ADC1_Handler)!=HAL_OK);//ȷ���򿪣�������ӡ����
		delay_ms(250);
		
		
		printf("��ʱ����\r\n");
		printf("\r\n");
		
		
	} 
}
