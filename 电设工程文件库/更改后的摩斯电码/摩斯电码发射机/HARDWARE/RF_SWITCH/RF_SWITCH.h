#ifndef __RF_SWITCH_H
#define __RF_SWITCH_H
#include "sys.h"

#define RF_control(n) (n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET))
#define RF_control_toggle (HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3))  

void RF_SWITCH_Init(void);

#endif

