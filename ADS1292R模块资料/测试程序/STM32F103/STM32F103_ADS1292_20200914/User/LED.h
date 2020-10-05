//-----------------------------------------------------------------
// LED����ͷ�ļ�
// ͷ�ļ���: LED.h
// ��    ��: ���ǵ���
// ��ʼ����: 2014-01-28
// �������: 2014-01-28
// �޸�����: 2014-02-16
// ��ǰ�汾: V1.0.1
// ��ʷ�汾:
//   - V1.0: (2014-02-07)LED IO ����
// - V1.0.1:(2014-02-16)ͷ�ļ��в���������ͷ�ļ�
//-----------------------------------------------------------------

#ifndef _LED_H
#define _LED_H

//-----------------------------------------------------------------
// λ����
//-----------------------------------------------------------------
#define LED1_ON      GPIO_ResetBits(GPIOE, GPIO_Pin_4)
#define LED1_OFF     GPIO_SetBits(GPIOE, GPIO_Pin_4)
#define LED1_State   GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_4)
#define LED1_Toggle  GPIO_WriteBit(GPIOE, GPIO_Pin_4, (BitAction)(1 - LED1_State))
#define LED2_ON      GPIO_ResetBits(GPIOE, GPIO_Pin_5)
#define LED2_OFF     GPIO_SetBits(GPIOE, GPIO_Pin_5)
#define LED2_State   GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_5)
#define LED2_Toggle  GPIO_WriteBit(GPIOE, GPIO_Pin_5, (BitAction)(1 - LED2_State))

//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
extern void GPIO_LED_Configuration(void);

#endif

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
