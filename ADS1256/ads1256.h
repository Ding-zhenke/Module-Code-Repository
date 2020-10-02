#ifndef __ADS1256_H
#define __ADS1256_H
#include "matrixos.h"
/////////////////////////////////////////////////////////////////////////
#define ADS1256_CMD_WREG     0x50 
#define ADS1256_MUX          0x01 

#define ADS1256_DRATE_30000SPS   0xF0 
#define ADS1256_DRATE_15000SPS   0xE0 
#define ADS1256_DRATE_7500SPS   0xD0 
#define ADS1256_DRATE_3750SPS   0xC0 
#define ADS1256_DRATE_2000SPS   0xB0 
#define ADS1256_DRATE_1000SPS   0xA1 
#define ADS1256_DRATE_500SPS    0x92 
#define ADS1256_DRATE_100SPS    0x82 
#define ADS1256_DRATE_60SPS     0x72 
#define ADS1256_DRATE_50SPS     0x63 
#define ADS1256_DRATE_30SPS     0x53 
#define ADS1256_DRATE_25SPS     0x43 
#define ADS1256_DRATE_15SPS     0x33 
#define ADS1256_DRATE_10SPS     0x23 
#define ADS1256_DRATE_5SPS      0x13 
#define ADS1256_DRATE_2_5SPS    0x03

#define ADS1256_DIN_H HAL_GPIO_WritePin(GPIOH,GPIO_PIN_14,GPIO_PIN_SET)
#define ADS1256_DIN_L HAL_GPIO_WritePin(GPIOH,GPIO_PIN_14,GPIO_PIN_RESET)
#define ADS1256_SCLK_H HAL_GPIO_WritePin(GPIOH,GPIO_PIN_15,GPIO_PIN_SET)
#define ADS1256_SCLK_L HAL_GPIO_WritePin(GPIOH,GPIO_PIN_15,GPIO_PIN_RESET)

#define ADS1256_RESET_H HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_SET)
#define ADS1256_RESET_L HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_RESET)
#define ADS1256_CS_H HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_SET)
#define ADS1256_CS_L HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_SET)
#define ADS1256_DRDY HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_7)
#define ADS1256_DOUT HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_4)
#define ADS1256_SYNC_H HAL_GPIO_WritePin(GPIOI,GPIO_PIN_5,GPIO_PIN_SET)
#define ADS1256_SYNC_L HAL_GPIO_WritePin(GPIOI,GPIO_PIN_5,GPIO_PIN_RESET)
////////////////////////////////////////////////////////////////////////

void ads1256_event(local_state_space *ADS1256_state);
void SPI_WriteByte(u8 TxData);
u8 SPI_ReadByte(void);
void ADSDelay(u32 nCount);

void ADS1256WREG(u8 regaddr,u8 databyte);
void ADS1256_Init(void);
u32 ADS1256ReadData(u8 channel);
u32 ADS1256_ReadData(void);
void Channal_sel(u8 cha);
void GPIO_Configuration(void);

#endif





