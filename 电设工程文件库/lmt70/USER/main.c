#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h"
#include "key.h"
#include "sdram.h"
#include "usmart.h"
#include "w25qxx.h"
#include "timer.h"
#include "arm_math.h"
#include "numc.h"
#include "adc.h"
#include "oled.h"



int main(void)
{
 	u8 t=0; 
	u8 buf[50]; 
	double temperature;
	Cache_Enable();                			//��L1-Cache
	HAL_Init();				        		//��ʼ��HAL��
	Stm32_Clock_Init(160,5,2,4);  		    //����ʱ��,400Mhz 
	delay_init(400);						//��ʱ��ʼ��
	uart_init(115200);						//���ڳ�ʼ��
	usart2_init(115200);
	usmart_dev.init(200); 		    		//��ʼ��USMART	
	LED_Init();								//��ʼ��LED
	KEY_Init();								//��ʼ������
	SDRAM_Init();                   		//��ʼ��SDRAM
	MY_ADC_Init();						//ADC
	IIC_Init();
	OLED_Init();
	OLED_CLS();
    while(1)
	{
		temperature = Get_Adc_Average(ADC_CHANNEL_19,20)*3.3/65536;
		sprintf((char *)buf,"V = %0.4f V",temperature);
		OLED_P8x16Str(0,0,buf);
		temperature = (-0.193)*temperature*1e3+212.009;
		sprintf((char *)buf,"T =%0.3f C",temperature);
		OLED_P8x16Str(0,3,buf);
		delay_ms(100);
		t++;
		if((t%2)==0)
		{	
			LED2_Toggle;
		//	u2_print("t0.txt=\"\\ff\"");
		}			
	}		 
}

