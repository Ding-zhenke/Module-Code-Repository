//-----------------------------------------------------------------
//   ���ж�����
// ͷ�ļ���: EXTInterrupt.h
// ��    ��: ���ǵ���
// ��д����: 2014-02-02
// �޸�����: 2014-02-20
//-----------------------------------------------------------------

#ifndef _EXTInterrupt_H
#define _EXTInterrupt_H

//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
void GPIO_EXTInterrupt_Configuration(void);
void EXTInterrupt_Mode_Configuration(void);
void NVIC_Configuration(void);
extern void EXTInterrupt_Init(void);

#endif
