//-----------------------------------------------------------------
//   外中断驱动
// 头文件名: EXTInterrupt.h
// 作    者: 凌智电子
// 编写日期: 2014-02-02
// 修改日期: 2014-02-20
//-----------------------------------------------------------------

#ifndef _EXTInterrupt_H
#define _EXTInterrupt_H

//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
void GPIO_EXTInterrupt_Configuration(void);
void EXTInterrupt_Mode_Configuration(void);
void NVIC_Configuration(void);
extern void EXTInterrupt_Init(void);

#endif
