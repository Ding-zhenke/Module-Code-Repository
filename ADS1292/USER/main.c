#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h"
#include "key.h"
#include "usmart.h"
#include "timer.h"
#include "arm_math.h"
#include "numc.h"
#include "oled.h"
#include "spi.h"
#include "ads1292.h"
#include "mlx90614.h"
/************************************************
 ALIENTEK ������STM32H7������ ʵ��52_2
 DSP FFTʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


int main(void)
{
 	u8 t=0; 
	float a;
	u8 buf[50]; 
	double data[2];
	Cache_Enable();                			//��L1-Cache
	HAL_Init();				        		//��ʼ��HAL��
	Stm32_Clock_Init(160,5,2,4);  		    //����ʱ��,400Mhz 
	delay_init(400);						//��ʱ��ʼ��
	uart_init(115200);						//���ڳ�ʼ��
	usmart_dev.init(200); 		    		//��ʼ��USMART	
	LED_Init();								//��ʼ��LED
	KEY_Init();								//��ʼ������
	//OLED_Init();							//oled��ʼ��
	SMBus_Init();
	//sprintf((char *)buf,"hello world!");
	//OLED_ShowString(0,0,buf);
	//SPI2_Init();							//SPI��ʼ��
	//ADS1292_PowerOnInit();					//ADS1292�ϵ��ʼ��
	while(1)
	{		
		//delay_ms(1);
		t++;
	//	OLED_ShowString(0,0,buf);
		//rd(data);
//		printf("data0 = %f\r\n",data[0]);
//		printf("data1 = %f\r\n",data[1]);
		a = SMBus_ReadTemp();
		//SMBus_StartBit();
		printf("%f\r\n",a);
		delay_ms(250);
		if((t%10)==0)
		{	
			LED2_Toggle;
			
		//	u2_print("t0.txt=\"\\ff\"");
		}					 

	}
}
