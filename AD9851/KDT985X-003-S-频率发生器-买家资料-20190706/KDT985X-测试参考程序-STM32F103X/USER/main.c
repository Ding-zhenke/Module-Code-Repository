#include "stm32_config.h"
#include "stdio.h"
#include "led.h"
#include "AD9851.h" 

int main(void)
{
	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//设置中断分组
	delay_init(72);	//初始化延时函数
	LED_Init();	//初始化LED接口
	AD9851_Init(ad9851_serial,1);
	AD9851_Setfq(30000000);
//	AD9851_Setfq(2000);
	while(1)
	{
		LED1=~LED1;
		delay_ms(500);
		LED2=~LED2;
		delay_ms(100);
	}	
}

