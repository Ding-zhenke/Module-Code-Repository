//-----------------------------------------------------------------
// 程序描述:
//     串口驱动程序
// 作    者: 凌智电子
// 开始日期: 2020-09-15
// 完成日期: 2020-09-15
// 当前版本: V1.0.0
// 历史版本:
// - V1.0.0: (2020-09-15)串口驱动
// 调试工具: 凌智STM32核心开发板、LZE_ST_LINK2、USB转串口模块
// 说    明:
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "usart.h"	 

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	//循环发送,直到发送完毕
	while((USART1->SR&0X40)==0)
		;   
  USART1->DR = (u8) ch;      
	return ch;
}
#endif 
	
u8 USART_RX_BUF[USART_REC_LEN];     // 接收缓冲,最大USART_REC_LEN个字节
u16 USART_RX_STA=0; // 接收状态标记（bit15：接收完成标志  bit14：接收到0x0d   bit13~0：接收到的有效字节数目）

//-----------------------------------------------------------------
// void uart_init(u32 bound)
//-----------------------------------------------------------------
//
// 函数功能: 串口初始化
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void uart_init(u32 bound)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	// USART1_TX -> PA9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 				// PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	// 高速
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		// 复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);						// 初始化
   
  // USART1_RX -> PA10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;						// PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	// 浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);								// 初始化 

  // Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;	// 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				// 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						// IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	// 根据指定的参数初始化VIC寄存器
  
  // USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;									// 串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;					// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// 收发模式

  USART_Init(USART1, &USART_InitStructure); 			// 初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	// 开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    	// 使能串口1 

}

//-----------------------------------------------------------------
// void uart_init(u32 bound)
//-----------------------------------------------------------------
//
// 函数功能: 串口1中断服务函数
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void USART1_IRQHandler(void)
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);	// 读取接收到的数据
	
		if((USART_RX_STA&0x8000)==0)// 接收未完成
		{
			if(USART_RX_STA&0x4000)		// 接收到了0x0d
			{
				if(Res!=0x0a)
					USART_RX_STA=0;				// 接收错误,重新开始
				else 
					USART_RX_STA|=0x8000;	// 接收完成了 
			}
			else // 还没收到0X0D
			{	
				if(Res==0x0d)
					USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))
						USART_RX_STA=0;			// 接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
	} 
} 
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
