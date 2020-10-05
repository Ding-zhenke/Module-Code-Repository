//-----------------------------------------------------------------
// LED驱动头文件
// 头文件名: spi.h
// 作    者: 凌智电子
// 开始日期: 2014-01-28
// 完成日期: 2014-01-28
// 修改日期: 2014-02-16
// 当前版本: V1.0.1
// 历史版本:
//   - V1.0: (2014-02-07)spi驱动头文件
// - V1.0.1:(2014-02-16)头文件中不包含其他头文件
//-----------------------------------------------------------------
#ifndef __SPI_H
#define __SPI_H
#include <stm32f10x.h>
			  	    													  
void SPI1_Init(void);			 //初始化SPI口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI1_ReadWriteByte(u8 TxData);

		 
#endif
