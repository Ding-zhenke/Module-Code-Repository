//-----------------------------------------------------------------
// ��������:
// 		 LED����ͷ�ļ�
// ��    ��: ���ǵ���
// ��ʼ����: 2018-08-04
// �������: 2018-08-04
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2018-08-04)LED IO ����
// ���Թ���: ����STM32F429+Cyclone IV����ϵͳ��ƿ����塢LZE_ST_LINK2
// ˵    ��: 
//    
//-----------------------------------------------------------------
#ifndef _LED_H
#define _LED_H
#include "stm32f429_winner.h"
//-----------------------------------------------------------------
// LED���Ŷ���
//-----------------------------------------------------------------
typedef enum 
{
  LEDB = 0,
  LEDG = 1,
	LEDR = 2
}Led_TypeDef;

#define LEDn                                    3

#define LEDB_PIN                                GPIO_PIN_14
#define LEDB_GPIO_PORT                          GPIOB
#define LEDB_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LEDB_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()  
#define LED_B 																	PBout(14)												// LED_B(����)		PB14

#define LEDG_PIN                                GPIO_PIN_7
#define LEDG_GPIO_PORT                          GPIOF
#define LEDG_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOF_CLK_ENABLE()  
#define LEDG_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOF_CLK_DISABLE()
#define LED_G 																	PFout(7)												// LED_G(�̵�)		PF7

#define LEDR_PIN                                GPIO_PIN_3
#define LEDR_GPIO_PORT                          GPIOI
#define LEDR_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOI_CLK_ENABLE()  
#define LEDR_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOI_CLK_DISABLE() 
#define LED_R 																	PIout(3)												// LED_R(���)		PI3

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)  do{if((__INDEX__) == 0) LEDB_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 1) LEDG_GPIO_CLK_ENABLE(); else \
																						if((__INDEX__) == 2) LEDR_GPIO_CLK_ENABLE(); \
                                            }while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) LEDB_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 1) LEDG_GPIO_CLK_DISABLE(); else \
																						if((__INDEX__) == 2) LEDR_GPIO_CLK_DISABLE(); \
                                            }while(0)

//-----------------------------------------------------------------

//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
extern void LED_Init(void);
extern void BSP_LED_Init(Led_TypeDef Led);
#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
