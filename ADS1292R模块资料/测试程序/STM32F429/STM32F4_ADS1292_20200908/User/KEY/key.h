//-----------------------------------------------------------------
// 程序描述:
// 		 独立键盘驱动程序头文件
// 作    者: 凌智电子
// 开始日期: 2018-08-04
// 完成日期: 2018-08-04
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2018-08-04)独立键盘的IO配置，键值返回
// 调试工具: 凌智STM32F429+Cyclone IV电子系统设计开发板、LZE_ST_LINK2
// 说    明: 
//    
//-----------------------------------------------------------------
#ifndef _KEY_H
#define _KEY_H
#include "stm32f429_winner.h"
//-----------------------------------------------------------------
// BUTTON引脚定义
//-----------------------------------------------------------------
typedef enum 
{  
  BUTTON_KEY1 = 0,
	BUTTON_KEY2 = 1,
	BUTTON_KEY3 = 2,
	BUTTON_KEY4 = 3
}Button_TypeDef;

#define BUTTONn                                4

#define BUTTON_KEY1_PIN                         GPIO_PIN_6
#define BUTTON_KEY1_GPIO_PORT                   GPIOD
#define BUTTON_KEY1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define BUTTON_KEY1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()

#define BUTTON_KEY2_PIN                         GPIO_PIN_4
#define BUTTON_KEY2_GPIO_PORT                   GPIOB
#define BUTTON_KEY2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUTTON_KEY2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUTTON_KEY2_EXTI_IRQn                   EXTI4_IRQn

#define BUTTON_KEY3_PIN                         GPIO_PIN_6
#define BUTTON_KEY3_GPIO_PORT                   GPIOB
#define BUTTON_KEY3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUTTON_KEY3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUTTON_KEY3_EXTI_IRQn                   EXTI9_5_IRQn

#define BUTTON_KEY4_PIN                         GPIO_PIN_9
#define BUTTON_KEY4_GPIO_PORT                   GPIOB
#define BUTTON_KEY4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUTTON_KEY4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUTTON_KEY4_EXTI_IRQn                   EXTI9_5_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)     do{if((__INDEX__) == 0) BUTTON_KEY1_GPIO_CLK_ENABLE(); else \
																									if((__INDEX__) == 1) BUTTON_KEY2_GPIO_CLK_ENABLE(); else \
																									if((__INDEX__) == 1) BUTTON_KEY3_GPIO_CLK_ENABLE(); else \
																									if((__INDEX__) == 2) BUTTON_KEY4_GPIO_CLK_ENABLE(); \
																									}while(0)
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)    do{if((__INDEX__) == 0) BUTTON_KEY1_GPIO_CLK_DISABLE(); else \
																									if((__INDEX__) == 1) BUTTON_KEY2_GPIO_CLK_DISABLE(); else \
																									if((__INDEX__) == 1) BUTTON_KEY3_GPIO_CLK_DISABLE(); else \
																									if((__INDEX__) == 2) BUTTON_KEY4_GPIO_CLK_DISABLE(); \
																									}while(0)

#define KEY1        HAL_GPIO_ReadPin(BUTTON_KEY1_GPIO_PORT,BUTTON_KEY1_PIN)  // KEY1按键PD6
#define KEY2        HAL_GPIO_ReadPin(BUTTON_KEY2_GPIO_PORT,BUTTON_KEY2_PIN)  // KEY2按键PB4
#define KEY3        HAL_GPIO_ReadPin(BUTTON_KEY3_GPIO_PORT,BUTTON_KEY3_PIN)  // KEY3按键PB6
#define KEY4       	HAL_GPIO_ReadPin(BUTTON_KEY4_GPIO_PORT,BUTTON_KEY4_PIN)  // KEY4按键PB9

#define KEY1_PRES 	1			// K1键值
#define KEY2_PRES		2     // K2键值
#define KEY3_PRES		3     // K3键值
#define KEY4_PRES   4     // K4键值

//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
extern void KEY_Init(void);
extern void BSP_KEY_Init(Button_TypeDef button);
extern u8 KEY_Scan(u8 mode);
//-----------------------------------------------------------------
#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
