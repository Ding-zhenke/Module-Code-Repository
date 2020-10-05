//-----------------------------------------------------------------
// ��������:
//     SPI��������
// ��    ��: ���ǵ���
// ��ʼ����: 2020-09-15
// �������: 2020-09-15
// ��ǰ�汾: V1.0.0
// ��ʷ�汾:
// - V1.0.0: (2020-09-15)SPI����
// ���Թ���: ����STM32���Ŀ����塢LZE_ST_LINK2
// ˵    ��:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "spi.h"	

SPI_InitTypeDef  SPI_InitStructure;

//-----------------------------------------------------------------
// void SPI1_Init(void)
//-----------------------------------------------------------------
//
// ��������: SPI1��ʼ��
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
 
	// PA5->ADS1292_SCLK, PA6->ADS1292_DOUT, PA7->ADS1292_DIN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		// �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// ����
	GPIO_Init(GPIOA, &GPIO_InitStructure);							// ��ʼ��

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // ����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												// ����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										// ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;													// ѡ���˴���ʱ�ӵ���̬:ʱ������
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;												// ���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														// NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;	// ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ32
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									// ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 0;														// CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  																// ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE); // ʹ��SPI����
}   

//-----------------------------------------------------------------
// u8 SPI1_ReadWriteByte(u8 TxData)
//-----------------------------------------------------------------
//
// ��������: SPI1 ��дһ���ֽ�
// ��ڲ���: Ҫд����ֽ�
// �� �� ֵ: ��ȡ�����ֽ�
// ע������: ��
//
//-----------------------------------------------------------------
u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u16 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>20000)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������		
	retry=0;	
		
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>20000)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����				
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
