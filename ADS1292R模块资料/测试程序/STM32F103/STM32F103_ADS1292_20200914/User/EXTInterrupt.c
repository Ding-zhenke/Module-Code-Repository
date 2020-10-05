//-----------------------------------------------------------------
// 程序描述:
//    - 外中断驱动程序
// 作    者: 凌智电子
// 开始日期: 2014-02-02
// 完成日期: 2014-02-02
// 修改日期: 2014-02-20
// 版    本: V1.1
//   - V1.0: 使用外中断线实现中断
//   - V1.1: 修改格式和函数名
// 调试工具: 凌智STM32核心开发板、LZE_ST LINK2
// 说    明:
//    - 调试使用的系统时钟频率Fsysclk=72MHz；
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "EXTInterrupt.h"

//-----------------------------------------------------------------
// 初始化程序区
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// void GPIO_EXTInterrupt_Configuration(void)
//-----------------------------------------------------------------
//
// 函数功能: 外中断输入I/O配置
// 入口参数: 无
// 返 回 值: 无
// 注意事项:  PA.6----EXTI_IN
//-----------------------------------------------------------------
void GPIO_EXTInterrupt_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // 开启PA时钟和I/O复用时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

//-----------------------------------------------------------------
// void EXTInterrupt_Mode_Configuration(void)
//-----------------------------------------------------------------
//
// 函数功能: 外中断的配置
// 入口参数: 无
// 返 回 值: 无
// 注意事项:
//-----------------------------------------------------------------
void EXTInterrupt_Mode_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource1);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);

  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_Init(&EXTI_InitStructure);
}

//-----------------------------------------------------------------
// void NVIC_Configuration(void)
//-----------------------------------------------------------------
//
// 函数功能: 配置外中断嵌套中断向量
// 入口参数: 无
// 返 回 值: 无
// 注意事项:
//-----------------------------------------------------------------
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  // 设置NVIC中断分组2,其中2位抢占优先级，2位响应优先级
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  // 选择外部中断通道//抢占优先级0//子优先级0
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_Init(&NVIC_InitStructure);
}

//-----------------------------------------------------------------
// void Exti_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: 外中断初始化
// 入口参数: 无
// 返 回 值: 无
// 注意事项:
//-----------------------------------------------------------------
void EXTInterrupt_Init(void)
{
  GPIO_EXTInterrupt_Configuration();
  NVIC_Configuration();
  EXTInterrupt_Mode_Configuration();
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
