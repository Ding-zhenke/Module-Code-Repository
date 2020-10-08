#ifndef _MLX90614_H
#define _MLX90614_H
#include "sys.h"
#include "delay.h"
#define ACK	 0
#define	NACK 1
#define SA				0x00 //从机地址，单个MLX90614时地址为0x00,多个时地址默认为0x5a
#define RAM_ACCESS		0x00 //RAM access command
#define EEPROM_ACCESS	0x20 //EEPROM access command
#define RAM_TOBJ1		0x07 //To1 address in the eeprom

//#define SMBUS_PORT	    GPIOB
//#define SMBUS_SCK		GPIO_Pin_6
//#define SMBUS_SDA		GPIO_Pin_7

//#define RCC_APB2Periph_SMBUS_PORT		RCC_APB2Periph_GPIOB

#define SMBUS_SCK(n)	(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET))
//#define SMBUS_SCK_L()	    SMBUS_PORT->BRR = SMBUS_SCK
#define SMBUS_SDA(n)	(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET))
//#define SMBUS_SDA_L()	    SMBUS_PORT->BRR = SMBUS_SDA
#define SMBUS_SDA_PIN()	  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7) //读取引脚电平


void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(u8 bit_out);
u8 SMBus_SendByte(u8 Tx_buffer);
u8 SMBus_ReceiveBit(void);
u8 SMBus_ReceiveByte(u8 ack_nack);
void SMBus_Delay(u16);
void SMBus_Init(void);
u16 SMBus_ReadMemory(u8 slaveAddress, u8 command);
u8 PEC_Calculation(u8 pec[]);
float SMBus_ReadTemp(void);    //获取温度值
void SMBus_DisplayTemperature(void);    //在LCD第5，6页显示温度
#endif
