//-----------------------------------------------------------------
// LED����ͷ�ļ�
// ͷ�ļ���: spi.h
// ��    ��: ���ǵ���
// ��ʼ����: 2014-01-28
// �������: 2014-01-28
// �޸�����: 2014-02-16
// ��ǰ�汾: V1.0.1
// ��ʷ�汾:
//   - V1.0: (2014-02-07)spi����ͷ�ļ�
// - V1.0.1:(2014-02-16)ͷ�ļ��в���������ͷ�ļ�
//-----------------------------------------------------------------
#ifndef __ADS1292_H
#define __ADS1292_H
#include <stm32f10x.h>

//-----------------------------------------------------------------
// λ����
//-----------------------------------------------------------------
#define PWDN_L      GPIO_ResetBits(GPIOA, GPIO_Pin_0)
#define PWDN_H     	GPIO_SetBits(GPIOA, GPIO_Pin_0)
#define START_L     GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define START_H     GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define CS_L      	GPIO_ResetBits(GPIOA, GPIO_Pin_2)
#define CS_H     		GPIO_SetBits(GPIOA, GPIO_Pin_2)

//-----------------------------------------------------------------
// ADS1292�����
//-----------------------------------------------------------------
// ϵͳ����
#define WAKEUP			0X02	// �Ӵ���ģʽ����
#define STANDBY			0X04	// �������ģʽ
#define RESET				0X06	// ��λADS1292R
#define START				0X08	// ������ת��
#define STOP				0X0A	// ֹͣת��
#define OFFSETCAL		0X1A	// ͨ��ƫ��У׼
// ���ݶ�ȡ����
#define RDATAC			0X10	// �������������ݶ�ȡģʽ,Ĭ��ʹ�ô�ģʽ
#define SDATAC			0X11	// ֹͣ���������ݶ�ȡģʽ
#define RDATA				0X12	// ͨ�������ȡ����;֧�ֶ��ֶ��ء�
// �Ĵ�����ȡ����
#define	RREG				0X20	// ��ȡ001r rrrr 000n nnnn  ����r rrrr����ʼ�Ĵ�����ַ
#define WREG				0X40	// д��010r rrrr 000n nnnn	����r rrrr����ʼ�Ĵ�����ַ n nnnn=��Ҫд������*/

// ADS1292R�ڲ��Ĵ�����ַ����
#define ID					0X00	// ID���ƼĴ���
#define CONFIG1			0X01	// ���üĴ���1
#define CONFIG2			0X02	// ���üĴ���2
#define LOFF				0X03	// ����������ƼĴ���
#define CH1SET			0X04	// ͨ��1���üĴ���
#define CH2SET			0X05	// ͨ��2���üĴ���
#define RLD_SENS		0X06	// ��������ѡ��Ĵ���
#define LOFF_SENS		0X07	// ����������ѡ��Ĵ���
#define LOFF_STAT		0X08	// ����������״̬�Ĵ���
#define	RESP1				0X09	// ���������ƼĴ���1
#define	RESP2				0X0A	// ���������ƼĴ���2
#define	GPIO				0X0B	// GPIO���ƼĴ���			  	    													  

//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
void GPIO_ADS1292_Configuration(void);
void ADS1292_PowerOnInit(void);
void ADS1292_Write_Reg(u8 addr, u8 data);
u8 ADS1292_Read_Reg(u8 addr);
void ADS1292_Read_Data(u8 *data);
		 
#endif
