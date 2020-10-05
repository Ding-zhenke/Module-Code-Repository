//-----------------------------------------------------------------
// LED驱动头文件
// 头文件名: LED.h
// 作    者: 凌智电子
// 开始日期: 2014-01-28
// 完成日期: 2014-01-28
// 修改日期: 2014-02-16
// 当前版本: V1.0.1
// 历史版本:
//   - V1.0: (2014-02-07)LED IO 配置
// - V1.0.1:(2014-02-16)头文件中不包含其他头文件
//-----------------------------------------------------------------

#ifndef _LED_H
#define _LED_H

//-----------------------------------------------------------------
// 位操作
//-----------------------------------------------------------------
#define LED1_ON      GPIO_ResetBits(GPIOE, GPIO_Pin_4)
#define LED1_OFF     GPIO_SetBits(GPIOE, GPIO_Pin_4)
#define LED1_State   GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_4)
#define LED1_Toggle  GPIO_WriteBit(GPIOE, GPIO_Pin_4, (BitAction)(1 - LED1_State))
#define LED2_ON      GPIO_ResetBits(GPIOE, GPIO_Pin_5)
#define LED2_OFF     GPIO_SetBits(GPIOE, GPIO_Pin_5)
#define LED2_State   GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_5)
#define LED2_Toggle  GPIO_WriteBit(GPIOE, GPIO_Pin_5, (BitAction)(1 - LED2_State))

//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
extern void GPIO_LED_Configuration(void);

#endif

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
