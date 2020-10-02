#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "adc.h"
/************************************************
 ALIENTEK 阿波罗STM32F7开发板 实验20
 ADC模数转换实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
u16 adcx;
extern ADC_HandleTypeDef ADC1_Handler;//ADC句柄
int main(void)
{
    Cache_Enable();                 //打开L1-Cache
    HAL_Init();				        //初始化HAL库
    Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
    delay_init(216);                //延时初始化
	uart_init(115200);		        //串口初始化
    LED_Init();                     //初始化LED
	MY_ADC_Init();                  //初始化ADC1通道5	
	HAL_ADC_Stop_IT(&ADC1_Handler);
    
    while(1)
	{
		printf("\r\n");
        printf("进入主程序\r\n");
		LED0_Toggle;		
		while(HAL_ADC_Start_IT(&ADC1_Handler)!=HAL_OK);//确保打开，否则会打印两次
		delay_ms(250);
		
		
		printf("延时过了\r\n");
		printf("\r\n");
		
		
	} 
}
