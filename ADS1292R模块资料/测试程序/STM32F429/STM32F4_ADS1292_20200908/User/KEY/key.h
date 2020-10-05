//-----------------------------------------------------------------
// ��������:
// 		 ����������������ͷ�ļ�
// ��    ��: ���ǵ���
// ��ʼ����: 2018-08-04
// �������: 2018-08-04
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2018-08-04)�������̵�IO���ã���ֵ����
// ���Թ���: ����STM32F429+Cyclone IV����ϵͳ��ƿ����塢LZE_ST_LINK2
// ˵    ��: 
//    
//-----------------------------------------------------------------
#ifndef _KEY_H
#define _KEY_H
#include "stm32f429_winner.h"
//-----------------------------------------------------------------
// BUTTON���Ŷ���
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

#define KEY1        HAL_GPIO_ReadPin(BUTTON_KEY1_GPIO_PORT,BUTTON_KEY1_PIN)  // KEY1����PD6
#define KEY2        HAL_GPIO_ReadPin(BUTTON_KEY2_GPIO_PORT,BUTTON_KEY2_PIN)  // KEY2����PB4
#define KEY3        HAL_GPIO_ReadPin(BUTTON_KEY3_GPIO_PORT,BUTTON_KEY3_PIN)  // KEY3����PB6
#define KEY4       	HAL_GPIO_ReadPin(BUTTON_KEY4_GPIO_PORT,BUTTON_KEY4_PIN)  // KEY4����PB9

#define KEY1_PRES 	1			// K1��ֵ
#define KEY2_PRES		2     // K2��ֵ
#define KEY3_PRES		3     // K3��ֵ
#define KEY4_PRES   4     // K4��ֵ

//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
extern void KEY_Init(void);
extern void BSP_KEY_Init(Button_TypeDef button);
extern u8 KEY_Scan(u8 mode);
//-----------------------------------------------------------------
#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
