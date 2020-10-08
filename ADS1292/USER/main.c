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
 ALIENTEK 阿波罗STM32H7开发板 实验52_2
 DSP FFT实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


int main(void)
{
 	u8 t=0; 
	float a;
	u8 buf[50]; 
	double data[2];
	Cache_Enable();                			//打开L1-Cache
	HAL_Init();				        		//初始化HAL库
	Stm32_Clock_Init(160,5,2,4);  		    //设置时钟,400Mhz 
	delay_init(400);						//延时初始化
	uart_init(115200);						//串口初始化
	usmart_dev.init(200); 		    		//初始化USMART	
	LED_Init();								//初始化LED
	KEY_Init();								//初始化按键
	//OLED_Init();							//oled初始化
	SMBus_Init();
	//sprintf((char *)buf,"hello world!");
	//OLED_ShowString(0,0,buf);
	//SPI2_Init();							//SPI初始化
	//ADS1292_PowerOnInit();					//ADS1292上电初始化
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
