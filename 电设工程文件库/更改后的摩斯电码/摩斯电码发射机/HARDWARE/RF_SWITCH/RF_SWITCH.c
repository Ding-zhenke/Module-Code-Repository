#include "RF_SWITCH.h"

void RF_SWITCH_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIODʱ��

	GPIO_Initure.Pin=GPIO_PIN_3;     //PA3 CONTROL
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;      //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);	

}









