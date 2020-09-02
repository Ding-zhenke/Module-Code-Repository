#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	
#include "timer.h"
#include "stdarg.h"	 	 	 	 
#include "string.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2015/6/23
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define RXBUFFERSIZE   1 //�����С	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern UART_HandleTypeDef UART1_Handler; //UART���
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);



#define USART2_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������2����
#define RXBUFFERSIZE2   1 //�����С	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		//����״̬���
extern UART_HandleTypeDef USART2_Handler; //UART���
extern u8 aRxBuffer2[RXBUFFERSIZE2];//HAL��USART����Buffer
//����봮���жϽ��գ��벻Ҫע�����º궨��
void usart2_init(u32 bound);
void u2_print(u8 *a);
void text_print(u8 *name,u8* b);


#endif
