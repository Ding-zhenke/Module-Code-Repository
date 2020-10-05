//-----------------------------------------------------------------
// ��������:
//    - ���ж���������
// ��    ��: ���ǵ���
// ��ʼ����: 2014-02-02
// �������: 2014-02-02
// �޸�����: 2014-02-20
// ��    ��: V1.1
//   - V1.0: ʹ�����ж���ʵ���ж�
//   - V1.1: �޸ĸ�ʽ�ͺ�����
// ���Թ���: ����STM32���Ŀ����塢LZE_ST LINK2
// ˵    ��:
//    - ����ʹ�õ�ϵͳʱ��Ƶ��Fsysclk=72MHz��
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "EXTInterrupt.h"

//-----------------------------------------------------------------
// ��ʼ��������
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// void GPIO_EXTInterrupt_Configuration(void)
//-----------------------------------------------------------------
//
// ��������: ���ж�����I/O����
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������:  PA.6----EXTI_IN
//-----------------------------------------------------------------
void GPIO_EXTInterrupt_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // ����PAʱ�Ӻ�I/O����ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

//-----------------------------------------------------------------
// void EXTInterrupt_Mode_Configuration(void)
//-----------------------------------------------------------------
//
// ��������: ���жϵ�����
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������:
//-----------------------------------------------------------------
void EXTInterrupt_Mode_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource1);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);

  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_Init(&EXTI_InitStructure);
}

//-----------------------------------------------------------------
// void NVIC_Configuration(void)
//-----------------------------------------------------------------
//
// ��������: �������ж�Ƕ���ж�����
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������:
//-----------------------------------------------------------------
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  // ����NVIC�жϷ���2,����2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  // ѡ���ⲿ�ж�ͨ��//��ռ���ȼ�0//�����ȼ�0
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_Init(&NVIC_InitStructure);
}

//-----------------------------------------------------------------
// void Exti_Init(void)
//-----------------------------------------------------------------
//
// ��������: ���жϳ�ʼ��
// ��ڲ���: ��
// �� �� ֵ: ��
// ע������:
//-----------------------------------------------------------------
void EXTInterrupt_Init(void)
{
  GPIO_EXTInterrupt_Configuration();
  NVIC_Configuration();
  EXTInterrupt_Mode_Configuration();
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
