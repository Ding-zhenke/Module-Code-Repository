//-----------------------------------------------------------------
// ��������:
// 		 ADS1292����ͷ�ļ�
// ��    ��: ���ǵ���
// ��ʼ����: 2018-08-04
// �������: 2018-08-04
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2018-08-04)ADS1292����
// ���Թ���: ����STM32F429+Cyclone IV����ϵͳ��ƿ����塢LZE_ST_LINK2
// ˵    ��: 
//    
//-----------------------------------------------------------------
#ifndef _ADS1292_H
#define _ADS1292_H
#include "spi.h"
#include "delay.h"
//-----------------------------------------------------------------
// ADS1292���Ŷ���
//-----------------------------------------------------------------
#define ADS1292_START(n)	(n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_RESET))
#define ADS1292_PWDN(n)		(n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET))	
#define ADS1292_CS(n)		(n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET))
//#define ADS1292_GPIO1 		PEout(11)	
//#define ADS1292_GPIO2 		PEout(12)	

//-----------------------------------------------------------------
// ADS1292�����
//-----------------------------------------------------------------
	// ϵͳ����
	#define WAKEUP				0X02	// �Ӵ���ģʽ����
	#define STANDBY				0X04	// �������ģʽ
	#define RESET				0X06	// ��λADS1292R
	#define START				0X08	// ������ת��
	#define STOP				0X0A	// ֹͣת��
	#define OFFSETCAL			0X1A	// ͨ��ƫ��У׼
	// ���ݶ�ȡ����
	#define RDATAC				0X10	// �������������ݶ�ȡģʽ,Ĭ��ʹ�ô�ģʽ
	#define SDATAC				0X11	// ֹͣ���������ݶ�ȡģʽ
	#define RDATA				0X12	// ͨ�������ȡ����;֧�ֶ��ֶ��ء�
	// �Ĵ�����ȡ����
	#define	RREG				0X20	// ��ȡ001r rrrr 000n nnnn  ����r rrrr����ʼ�Ĵ�����ַ
	#define WREG				0X40	// д��010r rrrr 000n nnnn	����r rrrr����ʼ�Ĵ�����ַ n nnnn=��Ҫд������*/

	// ADS1292R�ڲ��Ĵ�����ַ����
	#define ID					0X00	// ID���ƼĴ���
	#define CONFIG1				0X01	// ���üĴ���1
	#define CONFIG2				0X02	// ���üĴ���2
	#define LOFF				0X03	// ����������ƼĴ���
	#define CH1SET				0X04	// ͨ��1���üĴ���
	#define CH2SET				0X05	// ͨ��2���üĴ���
	#define RLD_SENS			0X06	// ��������ѡ��Ĵ���
	#define LOFF_SENS			0X07	// ����������ѡ��Ĵ���
	#define LOFF_STAT			0X08	// ����������״̬�Ĵ���
	#define	RESP1				0X09	// ���������ƼĴ���1
	#define	RESP2				0X0A	// ���������ƼĴ���2
	#define	GPIO				0X0B	// GPIO���ƼĴ���

//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
void ADS1292_Init(void);
void ADS1292_PowerOnInit(void);
void ADS1292_Write_Reg(u8 addr, u8 data);
u8 ADS1292_Read_Reg(u8 addr);
void ADS1292_Read_Data(u8 *data);
void rd(double *data);
#endif
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
