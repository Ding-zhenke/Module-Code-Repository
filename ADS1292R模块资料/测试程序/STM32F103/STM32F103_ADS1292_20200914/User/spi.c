//-----------------------------------------------------------------
// 程序描述:
//     SPI驱动程序
// 作    者: 凌智电子
// 开始日期: 2020-09-15
// 完成日期: 2020-09-15
// 当前版本: V1.0.0
// 历史版本:
// - V1.0.0: (2020-09-15)SPI驱动
// 调试工具: 凌智STM32核心开发板、LZE_ST_LINK2
// 说    明:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "spi.h"	

SPI_InitTypeDef  SPI_InitStructure;

//-----------------------------------------------------------------
// void SPI1_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: SPI1初始化
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
 
	// PA5->ADS1292_SCLK, PA6->ADS1292_DOUT, PA7->ADS1292_DIN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		// 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 高速
	GPIO_Init(GPIOA, &GPIO_InitStructure);							// 初始化

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // 设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												// 设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										// 设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;													// 选择了串行时钟的稳态:时钟拉低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;												// 数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														// NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;	// 定义波特率预分频的值:波特率预分频值为32
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									// 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 0;														// CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  																// 根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); // 使能SPI外设
}   

//-----------------------------------------------------------------
// u8 SPI1_ReadWriteByte(u8 TxData)
//-----------------------------------------------------------------
//
// 函数功能: SPI1 读写一个字节
// 入口参数: 要写入的字节
// 返 回 值: 读取到的字节
// 注意事项: 无
//
//-----------------------------------------------------------------
u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u16 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>20000)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据		
	retry=0;	
		
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>20000)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据				
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
