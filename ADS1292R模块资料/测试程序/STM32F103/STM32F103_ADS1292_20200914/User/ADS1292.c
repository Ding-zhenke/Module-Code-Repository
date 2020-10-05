//-----------------------------------------------------------------
// ��������:
//     ADS1292��������
// ��    ��: ���ǵ���
// ��ʼ����: 2020-09-15
// �������: 2020-09-15
// ��ǰ�汾: V1.0.0
// ��ʷ�汾:
// - V1.0.0: (2020-09-15) ADS1292����
// ���Թ���: ����STM32���Ŀ����塢LZE_ST_LINK2
// ˵    ��:
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "ADS1292.h"	
#include "Delay.h"
#include "spi.h"	
#include "usart.h"	 

//-----------------------------------------------------------------
// void GPIO_LED_Configuration(void)
//-----------------------------------------------------------------
//
// ��������: ADS1292���ų�ʼ��
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void GPIO_ADS1292_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ʹ��IO��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// ADS1292_DRDY -> PA3
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;					// ����ADS1292_DRDY
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// ����
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;			// ��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);							// ��ʼ��
  
	// ADS1292_PWDN  -> PA0
	// ADS1292_START -> PA1
  // ADS1292_CS  	 -> PA2
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// ����
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;		// �������
  GPIO_Init(GPIOA, &GPIO_InitStructure);							// ��ʼ��
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
	CS_L;				// Ƭѡ����
  SPI1_ReadWriteByte(addr);	// �������������ͼĴ�����ַ
  Delay_1us(10);
  SPI1_ReadWriteByte(0x00);	// Ҫ��ȡ�ļĴ�����+1
  Delay_1us(10);
  SPI1_ReadWriteByte(data);	// д�������
	Delay_1us(10);
	CS_H;				// Ƭѡ�ø�
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
	CS_L;					// Ƭѡ����
  SPI1_ReadWriteByte(addr); 			// �������������ͼĴ�����ַ
  Delay_1us(10);
  SPI1_ReadWriteByte(0x00); 			// Ҫ��ȡ�ļĴ�����+1
  Delay_1us(10);
  Rxdata = SPI1_ReadWriteByte(0xFF); 	// ��ȡ������
	Delay_1us(10);
	CS_H;					// Ƭѡ�ø�
  return Rxdata;
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

  START_H;
  CS_H;
  PWDN_L; // �������ģʽ
  Delay_50ms(20);
  PWDN_H; // �˳�����ģʽ
  Delay_50ms(20);   // �ȴ��ȶ�
  PWDN_L; // ������λ����
  Delay_1us(10);
  PWDN_H;
  Delay_50ms(20); // �ȴ��ȶ������Կ�ʼʹ��ADS1292R
	
	START_L;
	CS_L;
	Delay_1us(10);
  SPI1_ReadWriteByte(SDATAC); // ����ֹͣ������ȡ��������
	Delay_1us(10);
	CS_H;
	
	// ��ȡоƬID
	device_id = ADS1292_Read_Reg(RREG | ID);
	while(device_id != 0x73)
	{
		printf("ERROR ID:%02x\r\n",device_id);
		device_id = ADS1292_Read_Reg(RREG | ID);
		Delay_50ms(20);
	}
	
	Delay_1us(10);
  ADS1292_Write_Reg(WREG | CONFIG2,  0XE0); // ʹ���ڲ��ο���ѹ
  Delay_1ms(10);                            // �ȴ��ڲ��ο���ѹ�ȶ�
  ADS1292_Write_Reg(WREG | CONFIG1,  0X02); // ����ת������Ϊ500SPS
  Delay_1us(10);
  ADS1292_Write_Reg(WREG | LOFF,     0XF0);	// �üĴ�����������������
  Delay_1us(10);
  ADS1292_Write_Reg(WREG | CH1SET,   0X00); // ����6�����ӵ��缫
  Delay_1us(10);
  ADS1292_Write_Reg(WREG | CH2SET,   0X00); // ����6�����ӵ��缫
  Delay_1us(10);
  ADS1292_Write_Reg(WREG | RLD_SENS, 0xEF);
  Delay_1us(10);
  ADS1292_Write_Reg(WREG | LOFF_SENS,0x0F);
  Delay_1us(10);
	ADS1292_Write_Reg(WREG | LOFF_STAT,0x00);
  Delay_1us(10);
  ADS1292_Write_Reg(WREG | RESP1,    0xEA); // ����������⣨ADS1292R���У�
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
// ��������: ��ȡADS1292������
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void ADS1292_Read_Data(u8 *data)
{
  u8 i;
	CS_L;
	Delay_1us(10);
  SPI1_ReadWriteByte(RDATAC);		// ��������������ȡ��������
  Delay_1us(10);
	CS_H;						
  START_H; 				// ����ת��
  while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 1);	// �ȴ�DRDY�ź�����
  CS_L;
	Delay_1us(10);
  for (i = 0; i < 9; i++)		// ������ȡ9������
  {
    *data = SPI1_ReadWriteByte(0xFF);
    data++;
  }
  START_L;				// ֹͣת��
  SPI1_ReadWriteByte(SDATAC);		// ����ֹͣ������ȡ��������
	Delay_1us(10);
	CS_H;
}
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
