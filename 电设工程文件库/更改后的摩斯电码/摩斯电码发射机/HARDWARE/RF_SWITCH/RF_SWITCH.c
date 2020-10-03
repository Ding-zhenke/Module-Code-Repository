#include "RF_SWITCH.h"

void RF_SWITCH_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOD时钟

	GPIO_Initure.Pin=GPIO_PIN_3;     //PA3 CONTROL
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;      //输入
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);	

}









