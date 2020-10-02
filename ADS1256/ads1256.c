#include "ads1256.h"

void ADSDelay(u32 nCount) 
{ 
	u8 i;u8 w;
	for(i=0;i<10;i++)
		for(w=0;w<nCount;w++); 
} 

void GPIO_Configuration(void) 
{ 
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
	
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	
  GPIO_InitStruct.Pin =  GPIO_PIN_4; //DOUT/DRAY
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; 
  GPIO_InitStruct.Pull = GPIO_PULLUP; 
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct); 
	
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct); 
	
} 

void SPI_WriteByte(u8 TxData)
{
  ADSDelay(0x20);
  for(u8 i = 0; i < 8; i++)
  {
    if (TxData & 0x80){ADS1256_DIN_H;}
    else {ADS1256_DIN_L;}
    ADS1256_SCLK_H;
    ADSDelay(0x0f);
    TxData <<= 1;
    ADS1256_SCLK_L;
    ADSDelay(0x0f);
  }
} 


u8 SPI_ReadByte(void)
{
  u8 read = 0;
  ADSDelay(0x20);
  for (u8 i = 0; i < 8; i++)
  {
    ADS1256_SCLK_H;
    ADSDelay(0x0f);
    read = read<<1;
    ADS1256_SCLK_L;
    if(ADS1256_DOUT)
    {
      read++;
    }
    ADSDelay(0x0f);
  }
  return read;
}


void ADS1256WREG(u8 regaddr,u8 databyte)
{
  ADS1256_CS_L;
  while(ADS1256_DRDY);
  SPI_WriteByte(ADS1256_CMD_WREG | (regaddr & 0x0F));
  SPI_WriteByte(0x00);
  SPI_WriteByte(databyte);
  ADS1256_CS_H;
}


void Channal_sel(u8 cha)
{
  ADS1256_CS_L;
  u8 cha_temp;
  cha_temp=cha;
  while(ADS1256_DRDY);
  SPI_WriteByte(0x51);
  SPI_WriteByte(0x00);
  cha_temp<<=4;
  SPI_WriteByte(cha_temp|0x0f);
  ADS1256_CS_H;
}

u32 ADS1256ReadData(u8 channel)  
{
  u32 sum=0;
  ADS1256WREG(ADS1256_MUX,channel);
  ADS1256_CS_L;
  ADSDelay(0x01);
  SPI_WriteByte(0x01);
  ADSDelay(0x1);
  sum |= (SPI_ReadByte() << 16);
  sum |= (SPI_ReadByte() << 8);
  sum |= SPI_ReadByte();
  ADS1256_CS_H;
  return sum;
}


u32 ADS1256_ReadData(void)  
{
  while(ADS1256_DRDY);
  u32 sum=0;
  ADS1256_CS_L;
  SPI_WriteByte(0x01);
  ADSDelay(0x01);
  sum |= (SPI_ReadByte() << 16);
  sum |= (SPI_ReadByte() << 8);
  sum |= SPI_ReadByte();
  ADS1256_CS_H;
  return sum;
}

void ADS1256_Init(void)
{
  ADS1256_SYNC_H;
  ADS1256_CS_L;
  ADS1256_RESET_L;
  ADSDelay(0x10);
  ADS1256_RESET_H;//reset to power-up values
  
  SPI_WriteByte(0x00);//completes sync and exits standby mode
  while(ADS1256_DRDY);
  
  SPI_WriteByte(0x50);//WREG COM for write reg 0x00
  SPI_WriteByte(0x04);//write 6 register
  SPI_WriteByte(0x06);//00000110---status register

  SPI_WriteByte(0x1F);//00000001--mux register(default values)
  SPI_WriteByte(0x00);//00000000
  SPI_WriteByte(ADS1256_DRATE_50SPS);//01100011
  SPI_WriteByte(0xE0);//default values
  while(ADS1256_DRDY);
  
  SPI_WriteByte(0xF2);//gain self-calibration
  while(ADS1256_DRDY);
  SPI_WriteByte(0xF0);//offset self-calibration
  while(ADS1256_DRDY);

  //SPI_WriteByte(0xF3);//system offset calibration
  //SPI_WriteByte(0xF4);//system gain calibration
}  

void ads1256_event(local_state_space *ADS1256_state)
{
	if(ADS1256_state->ready_state==prepare_state)
	{
		GPIO_Configuration();
		ADS1256_Init();
		ADS1256_state->data[0]=0;
		ADS1256_state->ready_state=run_state0;
	}
	else if(ADS1256_state->ready_state==run_state0)
	{
		//read
		Channal_sel(0);
		ADS1256_state->data[0]=ADS1256_ReadData();
		//show
		if(ADS1256_state->data[0]>>23==1)ADS1256_state->data[0]=0;
		printf("%f\n ",(double)ADS1256_state->data[0]*10/16777214);
	}
}

/*enum state{
					sleep_state0=1,prepare_state=2,run_state0=3,
					run_state1=4,run_state2=5,run_state3=6,
					run_state4=7,dead_state=8,sleep_state1=9,
					sleep_state2=10,sleep_state3=11,run_state5=12,
					run_state6=13};*/
