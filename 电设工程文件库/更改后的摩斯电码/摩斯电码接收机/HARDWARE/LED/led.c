#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 

//LED驱动代码	  

//STM32H7工程模板-HAL库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 	

//初始化PA1,并使能这个口的时钟		

//LED I/O初始化
void LED_Init(void)
{
	
	
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();					          //开启PA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_1;			              //PA1
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		    //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;         			    //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  	//高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);     	     	//初始化PA1
	   
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	//PA1置1 
	
	
}














//STM32H7工程模板-HAL库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	










