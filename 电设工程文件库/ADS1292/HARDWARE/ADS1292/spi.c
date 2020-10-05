
//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "spi.h"
#include "ads1292.h"

//-----------------------------------------------------------------
SPI_HandleTypeDef SPI4_Handler; // SPI���
//-----------------------------------------------------------------
// void SPI4_Init(void)
//-----------------------------------------------------------------
//
// ��������: SPI4�����������ó�����ģʽ
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void SPI4_Init(void)
{
  SPI4_Handler.Instance               = SPI4;                     // SP5
  SPI4_Handler.Init.Mode              = SPI_MODE_MASTER;          // ����SPI����ģʽ������Ϊ��ģʽ
  SPI4_Handler.Init.Direction         = SPI_DIRECTION_2LINES;     // ����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
  SPI4_Handler.Init.DataSize          = SPI_DATASIZE_8BIT;        // ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
  SPI4_Handler.Init.CLKPolarity       = SPI_POLARITY_LOW;         // ����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
  SPI4_Handler.Init.CLKPhase          = SPI_PHASE_2EDGE;          // ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
  SPI4_Handler.Init.NSS               = SPI_NSS_SOFT;             // NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
  SPI4_Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32; // ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
  SPI4_Handler.Init.FirstBit          = SPI_FIRSTBIT_MSB;         // ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
  SPI4_Handler.Init.CRCPolynomial     = 0;                        // CRCֵ����Ķ���ʽ
  HAL_SPI_Init(&SPI4_Handler);                                    // ��ʼ��
  __HAL_SPI_ENABLE(&SPI4_Handler);                                // ʹ��SPI4
}

//-----------------------------------------------------------------
// void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
//-----------------------------------------------------------------
//
// ��������: SPI4�ײ�������ʱ��ʹ�ܣ���������
// ��ڲ���: SPI_HandleTypeDef *hspi��SPI���
// �� �� ֵ: ��
// ע������: �˺����ᱻHAL_SPI_Init()����
//
//-----------------------------------------------------------------
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef GPIO_Initure;

  __HAL_RCC_GPIOE_CLK_ENABLE(); // ʹ��GPIOEʱ��
  __HAL_RCC_SPI4_CLK_ENABLE();  // ʹ��SPI4ʱ��

	// PE12->ADS1292_SCLK, PE13->ADS1292_DOUT, PE14->ADS1292_DIN                    OLD:PF9->ADS1292_DIN
  GPIO_Initure.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
  GPIO_Initure.Mode      = GPIO_MODE_AF_PP;    // �����������
  GPIO_Initure.Pull      = GPIO_PULLUP;        // ����
  GPIO_Initure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;   // ����
  GPIO_Initure.Alternate = GPIO_AF5_SPI4; // ����ΪSPI4
  HAL_GPIO_Init(GPIOE, &GPIO_Initure);    // ��ʼ��
}

//-----------------------------------------------------------------
// void SPI4_SetSpeed(u8 SPI_BaudRatePrescaler)
//-----------------------------------------------------------------
//
// ��������: SPI�ٶ����ú���
// ��ڲ���: u8 SPI_BaudRatePrescaler��SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
// �� �� ֵ: ��
// ע������: SPI�ٶ�=fAPB1/��Ƶϵ����fAPB1ʱ��һ��Ϊ45Mhz
//
//-----------------------------------------------------------------
void SPI4_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); // �ж���Ч��
  __HAL_SPI_DISABLE(&SPI4_Handler);                               // �ر�SPI
  SPI4_Handler.Instance->CR1 &= 0XFFC7;                           // λ3-5���㣬�������ò�����
  SPI4_Handler.Instance->CR1 |= SPI_BaudRatePrescaler;            // ����SPI�ٶ�
  __HAL_SPI_ENABLE(&SPI4_Handler);                                // ʹ��SPI
}

//-----------------------------------------------------------------
// u8 SPI4_ReadWriteByte(u8 TxData)
//-----------------------------------------------------------------
//
// ��������: SPI4 ��дһ���ֽ�
// ��ڲ���: u8 TxData�� Ҫд����ֽ�
// �� �� ֵ: u8 Rxdata����ȡ�����ֽ�
// ע������: ��
//
//-----------------------------------------------------------------
u8 SPI4_ReadWrite_Byte(u8 dat)
{
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&SPI4_Handler, &dat, &Rxdata, 1, 1000);
  return Rxdata; // �����յ�������
}

//-----------------------------------------------------------------
// void SPI4_Send_Byte(u8 dat)
//-----------------------------------------------------------------
//
// ��������: SPI4����1���ֽ�����
// ��ڲ���: u8 dat�� �����͵�����
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void SPI4_Send_Byte(u8 dat)
{
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&SPI4_Handler, &dat, &Rxdata, 1, 1000);
}

//-----------------------------------------------------------------
// void SPI4_Send_Byte(u8 dat)
//-----------------------------------------------------------------
//
// ��������: SPI4��ȡ1���ֽ�����
// ��ڲ���: u8 dat�� �����͵�����
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
u8 SPI4_Read_Byte(void)
{
  u8 Txdata = 0xFF;
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&SPI4_Handler, &Txdata, &Rxdata, 1, 1000);
  return Rxdata; // �����յ�������
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
