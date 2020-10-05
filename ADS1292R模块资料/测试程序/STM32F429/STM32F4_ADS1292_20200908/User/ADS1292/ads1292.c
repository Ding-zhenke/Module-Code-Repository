//-----------------------------------------------------------------
// 程序描述:
//     ADS1292驱动程序
// 作    者: 凌智电子
// 开始日期: 2020-09-01
// 完成日期: 2020-09-05
// 修改日期:
// 当前版本: V1.0
// 历史版本:
//  - V1.0:  ADS1292驱动
// 调试工具: 凌智STM32F429+CycloneIV电子系统设计开发板、LZE_ST_LINK2
// 说    明:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "ads1292.h"
#include "spi.h"
#include "delay.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void ADS1292_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: ADS1292初始化
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADS1292_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOE_CLK_ENABLE();

  // ADS1292_DRDY -> PE9
  GPIO_InitStruct.Pin   = GPIO_PIN_9;       // 配置ADS1292_DRDY
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;  // 输入
  GPIO_InitStruct.Pull  = GPIO_PULLUP;      // 上拉
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH; 	// 高速
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);  	// 初始化

  // ADS1292_START -> PE7
  // ADS1292_PWDN  -> PE8
  // ADS1292_CS  	 -> PE10
  // ADS1292_GPIO1 -> PE11
  // ADS1292_GPIO2 -> PE12
  GPIO_InitStruct.Pin   = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_10 |
                          GPIO_PIN_11 | GPIO_PIN_12;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP; 	// 推挽输出
  GPIO_InitStruct.Pull  = GPIO_PULLUP;         	// 上拉
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;    	// 高速
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);     	// 初始化

  SPI5_Init(); // SPI初始化
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

  ADS1292_START = 1;
  ADS1292_CS = 1;
  ADS1292_PWDN = 0; // 进入掉电模式
  delay_ms(1000);
  ADS1292_PWDN = 1; // 退出掉电模式
  delay_ms(1000);   // 等待稳定
  ADS1292_PWDN = 0; // 发出复位脉冲
  delay_us(10);
  ADS1292_PWDN = 1;
  delay_ms(1000); // 等待稳定，可以开始使用ADS1292R
	
	ADS1292_START = 0;
	ADS1292_CS = 0;
  SPI5_Send_Byte(SDATAC); // 发送停止连续读取数据命令
	delay_us(10);
	ADS1292_CS = 1;
	
	// 获取芯片ID
	device_id = ADS1292_Read_Reg(RREG | ID);
	while(device_id != 0x73)
	{
		printf("ERROR ID:%02x\r\n",device_id);
		device_id = ADS1292_Read_Reg(RREG | ID);
		HAL_Delay(1000);
	}
	
	delay_us(10);
  ADS1292_Write_Reg(WREG | CONFIG2,  0XE0); // 使用内部参考电压
  delay_ms(10);                            	// 等待内部参考电压稳定
  ADS1292_Write_Reg(WREG | CONFIG1,  0X03); // 设置转换速率为1KSPS
  delay_us(10);
  ADS1292_Write_Reg(WREG | LOFF,     0XF0);	// 该寄存器配置引出检测操作
  delay_us(10);
  ADS1292_Write_Reg(WREG | CH1SET,   0X00); // 增益6，连接到电极
  delay_us(10);
  ADS1292_Write_Reg(WREG | CH2SET,   0X00); // 增益6，连接到电极
  delay_us(10);
  ADS1292_Write_Reg(WREG | RLD_SENS, 0xEF);
  delay_us(10);
  ADS1292_Write_Reg(WREG | LOFF_SENS,0x0F);
  delay_us(10);
	ADS1292_Write_Reg(WREG | LOFF_STAT,0x00);
  delay_us(10);
  ADS1292_Write_Reg(WREG | RESP1,    0xEA); // 开启呼吸检测（ADS1292R特有）
  delay_us(10);
  ADS1292_Write_Reg(WREG | RESP2,    0x03);
  delay_us(10);
  ADS1292_Write_Reg(WREG | GPIO,     0x0C);
  delay_us(10);
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
	ADS1292_CS = 0;				// 片选拉低
  SPI5_Send_Byte(addr);	// 包含命令操作码和寄存器地址
  delay_us(10);
  SPI5_Send_Byte(0x00);	// 要读取的寄存器数+1
  delay_us(10);
  SPI5_Send_Byte(data);	// 写入的数据
	delay_us(10);
	ADS1292_CS = 1;				// 片选置高
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
	ADS1292_CS = 0;
  SPI5_Send_Byte(addr); 			// 包含命令操作码和寄存器地址
  delay_us(10);
  SPI5_Send_Byte(0x00); 			// 要读取的寄存器数+1
  delay_us(10);
  Rxdata = SPI5_Read_Byte(); 	// 读取的数据
	delay_us(10);
	ADS1292_CS = 1;
  return Rxdata;
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
	ADS1292_CS = 0;
  SPI5_Send_Byte(RDATAC);		// 发送启动连续读取数据命令
  delay_us(10);
	ADS1292_CS = 1;						
  ADS1292_START = 1; 				// 启动转换
  while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == 1);	// 等待DRDY信号拉低
  ADS1292_CS = 0;
  for (i = 0; i < 9; i++)		// 连续读取9个数据
  {
    *data = SPI5_Read_Byte();
    data++;
  }
  ADS1292_START = 0;				// 停止转换
  SPI5_Send_Byte(SDATAC);		// 发送停止连续读取数据命令
	delay_us(10);
	ADS1292_CS = 1;
}




void rd(u8 *data)
{
	
	int ch1_data = 0;									 // 通道1数据
    int ch2_data = 0;									 // 通道2数据
	u8 read_data[9];								 	 // 数据缓存区
	// read_data：24 status bits + 24 bits × 2 channels
    ADS1292_Read_Data(read_data);
		
		// 计算ADS1292R通道1的数据-呼吸测量数据
    ch1_data |= (uint32_t)read_data[3] << 16;
    ch1_data |= (uint32_t)read_data[4] << 8;
    ch1_data |= (uint32_t)read_data[5] << 0;

		// 计算ADS1292R通道2的数据-心电图数据
    ch2_data |= (uint32_t)read_data[6] << 16;
    ch2_data |= (uint32_t)read_data[7] << 8;
    ch2_data |= (uint32_t)read_data[8] << 0;

//		// 得到的数据是补码，需要进行数据转换
//    printf("A: %8d,%8d\n",ch1_data^0x800000, ch2_data^0x800000);
//	
	ch1_data=ch1_data^0x800000;
	ch2_data=ch2_data^0x800000;
	data[0]=ch1_data;
	data[1]=ch2_data;
}











//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
