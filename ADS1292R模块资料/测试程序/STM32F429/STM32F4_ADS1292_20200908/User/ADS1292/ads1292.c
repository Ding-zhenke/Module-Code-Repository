//-----------------------------------------------------------------
// ��������:
//     ADS1292��������
// ��    ��: ���ǵ���
// ��ʼ����: 2020-09-01
// �������: 2020-09-05
// �޸�����:
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0:  ADS1292����
// ���Թ���: ����STM32F429+CycloneIV����ϵͳ��ƿ����塢LZE_ST_LINK2
// ˵    ��:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "ads1292.h"
#include "spi.h"
#include "delay.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void ADS1292_Init(void)
//-----------------------------------------------------------------
//
// ��������: ADS1292��ʼ��
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void ADS1292_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOE_CLK_ENABLE();

  // ADS1292_DRDY -> PE9
  GPIO_InitStruct.Pin   = GPIO_PIN_9;       // ����ADS1292_DRDY
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;  // ����
  GPIO_InitStruct.Pull  = GPIO_PULLUP;      // ����
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH; 	// ����
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);  	// ��ʼ��

  // ADS1292_START -> PE7
  // ADS1292_PWDN  -> PE8
  // ADS1292_CS  	 -> PE10
  // ADS1292_GPIO1 -> PE11
  // ADS1292_GPIO2 -> PE12
  GPIO_InitStruct.Pin   = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_10 |
                          GPIO_PIN_11 | GPIO_PIN_12;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP; 	// �������
  GPIO_InitStruct.Pull  = GPIO_PULLUP;         	// ����
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;    	// ����
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);     	// ��ʼ��

  SPI5_Init(); // SPI��ʼ��
}

//-----------------------------------------------------------------
// void ADS1292_PowerOnInit(void)
//-----------------------------------------------------------------
//
// ��������: ADS1292�ϵ縴λ
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void ADS1292_PowerOnInit(void)
{
	u8 device_id;

  ADS1292_START = 1;
  ADS1292_CS = 1;
  ADS1292_PWDN = 0; // �������ģʽ
  delay_ms(1000);
  ADS1292_PWDN = 1; // �˳�����ģʽ
  delay_ms(1000);   // �ȴ��ȶ�
  ADS1292_PWDN = 0; // ������λ����
  delay_us(10);
  ADS1292_PWDN = 1;
  delay_ms(1000); // �ȴ��ȶ������Կ�ʼʹ��ADS1292R
	
	ADS1292_START = 0;
	ADS1292_CS = 0;
  SPI5_Send_Byte(SDATAC); // ����ֹͣ������ȡ��������
	delay_us(10);
	ADS1292_CS = 1;
	
	// ��ȡоƬID
	device_id = ADS1292_Read_Reg(RREG | ID);
	while(device_id != 0x73)
	{
		printf("ERROR ID:%02x\r\n",device_id);
		device_id = ADS1292_Read_Reg(RREG | ID);
		HAL_Delay(1000);
	}
	
	delay_us(10);
  ADS1292_Write_Reg(WREG | CONFIG2,  0XE0); // ʹ���ڲ��ο���ѹ
  delay_ms(10);                            	// �ȴ��ڲ��ο���ѹ�ȶ�
  ADS1292_Write_Reg(WREG | CONFIG1,  0X03); // ����ת������Ϊ1KSPS
  delay_us(10);
  ADS1292_Write_Reg(WREG | LOFF,     0XF0);	// �üĴ�����������������
  delay_us(10);
  ADS1292_Write_Reg(WREG | CH1SET,   0X00); // ����6�����ӵ��缫
  delay_us(10);
  ADS1292_Write_Reg(WREG | CH2SET,   0X00); // ����6�����ӵ��缫
  delay_us(10);
  ADS1292_Write_Reg(WREG | RLD_SENS, 0xEF);
  delay_us(10);
  ADS1292_Write_Reg(WREG | LOFF_SENS,0x0F);
  delay_us(10);
	ADS1292_Write_Reg(WREG | LOFF_STAT,0x00);
  delay_us(10);
  ADS1292_Write_Reg(WREG | RESP1,    0xEA); // ����������⣨ADS1292R���У�
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
// ��������: ��ADS1292���ڲ��Ĵ�������д����
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void ADS1292_Write_Reg(u8 addr, u8 data)
{
	ADS1292_CS = 0;				// Ƭѡ����
  SPI5_Send_Byte(addr);	// �������������ͼĴ�����ַ
  delay_us(10);
  SPI5_Send_Byte(0x00);	// Ҫ��ȡ�ļĴ�����+1
  delay_us(10);
  SPI5_Send_Byte(data);	// д�������
	delay_us(10);
	ADS1292_CS = 1;				// Ƭѡ�ø�
}

//-----------------------------------------------------------------
// u8 ADS1292_Read_Reg(u8 addr)
//-----------------------------------------------------------------
//
// ��������: ��ADS1292���ڲ��Ĵ������ж�����
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
u8 ADS1292_Read_Reg(u8 addr)
{
  u8 Rxdata;
	ADS1292_CS = 0;
  SPI5_Send_Byte(addr); 			// �������������ͼĴ�����ַ
  delay_us(10);
  SPI5_Send_Byte(0x00); 			// Ҫ��ȡ�ļĴ�����+1
  delay_us(10);
  Rxdata = SPI5_Read_Byte(); 	// ��ȡ������
	delay_us(10);
	ADS1292_CS = 1;
  return Rxdata;
}

//-----------------------------------------------------------------
// u8 ADS1292_Read_Data(u8 addr)
//-----------------------------------------------------------------
//
// ��������: ��ȡADS1292������
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void ADS1292_Read_Data(u8 *data)
{
  u8 i;
	ADS1292_CS = 0;
  SPI5_Send_Byte(RDATAC);		// ��������������ȡ��������
  delay_us(10);
	ADS1292_CS = 1;						
  ADS1292_START = 1; 				// ����ת��
  while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == 1);	// �ȴ�DRDY�ź�����
  ADS1292_CS = 0;
  for (i = 0; i < 9; i++)		// ������ȡ9������
  {
    *data = SPI5_Read_Byte();
    data++;
  }
  ADS1292_START = 0;				// ֹͣת��
  SPI5_Send_Byte(SDATAC);		// ����ֹͣ������ȡ��������
	delay_us(10);
	ADS1292_CS = 1;
}




void rd(u8 *data)
{
	
	int ch1_data = 0;									 // ͨ��1����
    int ch2_data = 0;									 // ͨ��2����
	u8 read_data[9];								 	 // ���ݻ�����
	// read_data��24 status bits + 24 bits �� 2 channels
    ADS1292_Read_Data(read_data);
		
		// ����ADS1292Rͨ��1������-������������
    ch1_data |= (uint32_t)read_data[3] << 16;
    ch1_data |= (uint32_t)read_data[4] << 8;
    ch1_data |= (uint32_t)read_data[5] << 0;

		// ����ADS1292Rͨ��2������-�ĵ�ͼ����
    ch2_data |= (uint32_t)read_data[6] << 16;
    ch2_data |= (uint32_t)read_data[7] << 8;
    ch2_data |= (uint32_t)read_data[8] << 0;

//		// �õ��������ǲ��룬��Ҫ��������ת��
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
