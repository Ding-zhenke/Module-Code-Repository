//-----------------------------------------------------------------
// 程序描述:
// 		 ADS1292驱动头文件
// 作    者: 凌智电子
// 开始日期: 2018-08-04
// 完成日期: 2018-08-04
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2018-08-04)ADS1292驱动
// 调试工具: 凌智STM32F429+Cyclone IV电子系统设计开发板、LZE_ST_LINK2
// 说    明: 
//    
//-----------------------------------------------------------------
#ifndef _ADS1292_H
#define _ADS1292_H
#include "spi.h"
#include "delay.h"
//-----------------------------------------------------------------
// ADS1292引脚定义
//-----------------------------------------------------------------
#define ADS1292_START(n)	(n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_RESET))
#define ADS1292_PWDN(n)		(n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET))	
#define ADS1292_CS(n)		(n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET))
//#define ADS1292_GPIO1 		PEout(11)	
//#define ADS1292_GPIO2 		PEout(12)	

//-----------------------------------------------------------------
// ADS1292命令定义
//-----------------------------------------------------------------
	// 系统命令
	#define WAKEUP				0X02	// 从待机模式唤醒
	#define STANDBY				0X04	// 进入待机模式
	#define RESET				0X06	// 复位ADS1292R
	#define START				0X08	// 启动或转换
	#define STOP				0X0A	// 停止转换
	#define OFFSETCAL			0X1A	// 通道偏移校准
	// 数据读取命令
	#define RDATAC				0X10	// 启用连续的数据读取模式,默认使用此模式
	#define SDATAC				0X11	// 停止连续的数据读取模式
	#define RDATA				0X12	// 通过命令读取数据;支持多种读回。
	// 寄存器读取命令
	#define	RREG				0X20	// 读取001r rrrr 000n nnnn  其中r rrrr是起始寄存器地址
	#define WREG				0X40	// 写入010r rrrr 000n nnnn	其中r rrrr是起始寄存器地址 n nnnn=是要写的数据*/

	// ADS1292R内部寄存器地址定义
	#define ID					0X00	// ID控制寄存器
	#define CONFIG1				0X01	// 配置寄存器1
	#define CONFIG2				0X02	// 配置寄存器2
	#define LOFF				0X03	// 导联脱落控制寄存器
	#define CH1SET				0X04	// 通道1设置寄存器
	#define CH2SET				0X05	// 通道2设置寄存器
	#define RLD_SENS			0X06	// 右腿驱动选择寄存器
	#define LOFF_SENS			0X07	// 导联脱落检测选择寄存器
	#define LOFF_STAT			0X08	// 导联脱落检测状态寄存器
	#define	RESP1				0X09	// 呼吸检测控制寄存器1
	#define	RESP2				0X0A	// 呼吸检测控制寄存器2
	#define	GPIO				0X0B	// GPIO控制寄存器

//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
void ADS1292_Init(void);
void ADS1292_PowerOnInit(void);
void ADS1292_Write_Reg(u8 addr, u8 data);
u8 ADS1292_Read_Reg(u8 addr);
void ADS1292_Read_Data(u8 *data);
void rd(double *data);
#endif
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
