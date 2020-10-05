//-----------------------------------------------------------------
// 程序描述:
//     ADS1292驱动程序
// 作    者: 凌智电子
// 开始日期: 2020-09-15
// 完成日期: 2020-09-15
// 当前版本: V1.0.0
// 历史版本:
// - V1.0.0: (2020-09-15) ADS1292驱动
// 调试工具: 凌智STM32核心开发板、LZE_ST_LINK2
// 说    明:
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "ADS1292.h"	
#include "Delay.h"
#include "spi.h"	
#include "usart.h"	 

//-----------------------------------------------------------------
// void GPIO_LED_Configuration(void)
//-----------------------------------------------------------------
//
// 函数功能: ADS1292引脚初始化
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void GPIO_ADS1292_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 使能IO口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// ADS1292_DRDY -> PA3
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;					// 配置ADS1292_DRDY
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 高速
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;			// 上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);							// 初始化
  
	// ADS1292_PWDN  -> PA0
	// ADS1292_START -> PA1
  // ADS1292_CS  	 -> PA2
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 高速
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;		// 推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);							// 初始化
}

//-----------------------------------------------------------------
// void ADS1292_Write_Reg(u8 com, u8 data)
//-----------------------------------------------------------------
//
// 函数功能: 对ADS1292的内部寄存器进行写操作
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADS1292_Write_Reg(u8 addr, u8 data)
{
	CS_L;				// 片选拉低
  SPI1_ReadWriteByte(addr);	// 包含命令操作码和寄存器地址
  Delay_1us(10);
  SPI1_ReadWriteByte(0x00);	// 要读取的寄存器数+1
  Delay_1us(10);
  SPI1_ReadWriteByte(data);	// 写入的数据
	Delay_1us(10);
	CS_H;				// 片选置高
}

//-----------------------------------------------------------------
// u8 ADS1292_Read_Reg(u8 addr)
//-----------------------------------------------------------------
//
// 函数功能: 对ADS1292的内部寄存器进行读操作
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
u8 ADS1292_Read_Reg(u8 addr)
{
  u8 Rxdata;
	CS_L;					// 片选拉低
  SPI1_ReadWriteByte(addr); 			// 包含命令操作码和寄存器地址
  Delay_1us(10);
  SPI1_ReadWriteByte(0x00); 			// 要读取的寄存器数+1
  Delay_1us(10);
  Rxdata = SPI1_ReadWriteByte(0xFF); 	// 读取的数据
	Delay_1us(10);
	CS_H;					// 片选置高
  return Rxdata;
}

//-----------------------------------------------------------------
// void ADS1292_PowerOnInit(void)
//-----------------------------------------------------------------
//
// 函数功能: ADS1292上电复位
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADS1292_PowerOnInit(void)
{
	u8 device_id;

  START_H;
  CS_H;
  PWDN_L; // 进入掉电模式
  Delay_50ms(20);
  PWDN_H; // 退出掉电模式
  Delay_50ms(20);   // 等待稳定
  PWDN_L; // 发出复位脉冲
  Delay_1us(10);
  PWDN_H;
  Delay_50ms(20); // 等待稳定，可以开始使用ADS1292R
	
	START_L;
	CS_L;
	Delay_1us(10);
  SPI1_ReadWriteByte(SDATAC); // 发送停止连续读取数据命令
	Delay_1us(10);
	CS_H;
	
	// 获取芯片ID
	device_id = ADS1292_Read_Reg(RREG | ID);
	while(device_id != 0x73)
	{
		printf("ERROR ID:%02x\r\n",device_id);
		device_id = ADS1292_Read_Reg(RREG | ID);
		Delay_50ms(20);
	}
	
	Delay_1us(10);
  ADS1292_Write_Reg(WREG | CONFIG2,  0XE0); // 使用内部参考电压
  Delay_1ms(10);                            // 等待内部参考电压稳定
  ADS1292_Write_Reg(WREG | CONFIG1,  0X02); // 设置转换速率为500SPS
  Delay_1us(10);
  ADS1292_Write_Reg(WREG | LOFF,     0XF0);	// 该寄存器配置引出检测操作
  Delay_1us(10);
  ADS1292_Write_Reg(WREG | CH1SET,   0X00); // 增益6，连接到电极
  Delay_1us(10);
  ADS1292_Write_Reg(WREG | CH2SET,   0X00); // 增益6，连接到电极
  Delay_1us(10);
  ADS1292_Write_Reg(WREG | RLD_SENS, 0xEF);
  Delay_1us(10);
  ADS1292_Write_Reg(WREG | LOFF_SENS,0x0F);
  Delay_1us(10);
	ADS1292_Write_Reg(WREG | LOFF_STAT,0x00);
  Delay_1us(10);
  ADS1292_Write_Reg(WREG | RESP1,    0xEA); // 开启呼吸检测（ADS1292R特有）
  Delay_1us(10);
  ADS1292_Write_Reg(WREG | RESP2,    0x03);
  Delay_1us(10);
  ADS1292_Write_Reg(WREG | GPIO,     0x0C);
  Delay_1us(10);
}

//-----------------------------------------------------------------
// u8 ADS1292_Read_Data(u8 addr)
//-----------------------------------------------------------------
//
// 函数功能: 读取ADS1292的数据
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADS1292_Read_Data(u8 *data)
{
  u8 i;
	CS_L;
	Delay_1us(10);
  SPI1_ReadWriteByte(RDATAC);		// 发送启动连续读取数据命令
  Delay_1us(10);
	CS_H;						
  START_H; 				// 启动转换
  while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 1);	// 等待DRDY信号拉低
  CS_L;
	Delay_1us(10);
  for (i = 0; i < 9; i++)		// 连续读取9个数据
  {
    *data = SPI1_ReadWriteByte(0xFF);
    data++;
  }
  START_L;				// 停止转换
  SPI1_ReadWriteByte(SDATAC);		// 发送停止连续读取数据命令
	Delay_1us(10);
	CS_H;
}
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
