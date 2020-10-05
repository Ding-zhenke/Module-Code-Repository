//-----------------------------------------------------------------
// 程序描述:
//    - ADS1292模块测试实验
// 作    者: 凌智电子
// 开始日期: 2020-09-15
// 完成日期: 2020-09-15
// 版    本: V1.0
// 调试工具: 凌智STM32核心开发板、LZE_ST LINK2、USB转串口模块、ADS1292模块
// 说    明: 
//					 （1）接口说明
//						ADS1292模块            核心板
// 						  PWDN   			->  		 	PA0
// 					    START      	->   			PA1
// 						  CS  	 			->   			PA2
// 						  DRDY   			->   			PA3
// 						  SCLK   			->   			PA5
// 						  DOUT   			->   			PA6
// 						  DIN    			->   			PA7
//              5V 					->   			+5V
//						  GND					->	 			GND
//
//					 USB转串口模块           核心板
//							RXD					->				PA9
//							TXD					->				PA10
//							GND					->				GND
//  				 （2）显示说明
//						使用串口调试助手显示
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "stm32f10x.h"
#include "Delay.h"
#include "EXTInterrupt.h"
#include "LED.h"
#include "lze_lcd.h"
#include "usart.h"	 
#include "ADS1292.h"	
#include "spi.h"
#include "PeripheralInit.h"

//-----------------------------------------------------------------
// 主程序
//-----------------------------------------------------------------
int main(void)
{
	u32 ch1_data;
  u32 ch2_data;
  u8 read_data[9];								 	 // 数据缓存区
	
  PeripheralInit(); // 外设初始化
  while (1)
  {
		ch1_data = 0;										 // 通道1数据
    ch2_data = 0;										 // 通道2数据

		// read_data：24 status bits + 24 bits × 2 channels
    ADS1292_Read_Data(read_data);
		
		// 计算ADS1292通道1的数据-呼吸阻抗测量
    ch1_data |= (uint32_t)read_data[3] << 16;
    ch1_data |= (uint32_t)read_data[4] << 8;
    ch1_data |= (uint32_t)read_data[5] << 0;

		// 计算ADS1292通道2的数据-心电图测量
    ch2_data |= (uint32_t)read_data[6] << 16;
    ch2_data |= (uint32_t)read_data[7] << 8;
    ch2_data |= (uint32_t)read_data[8] << 0;

		// 得到的数据是补码，需要进行数据转换
    printf("A: %8d,%8d\n",ch1_data^0x800000, ch2_data^0x800000);
		
  }
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
