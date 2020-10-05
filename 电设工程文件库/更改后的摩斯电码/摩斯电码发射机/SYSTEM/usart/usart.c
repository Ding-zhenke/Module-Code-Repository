#include "usart.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2015/6/23
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
//#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->ISR&0X40)==0);//ѭ������,ֱ���������   
	USART1->TDR=(u8)ch;      
	return ch;
}
#endif 

UART_HandleTypeDef UART1_Handler; //UART1���
UART_HandleTypeDef USART2_Handler;  //USART2���




#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	
u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���


//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound)
{	
	//UART ��ʼ������
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA10
		
#if EN_USART1_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}
	else if(huart->Instance==USART2)
	{
		GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_GPIOD_CLK_ENABLE();			//ʹ��GPIODʱ��
		__HAL_RCC_USART2_CLK_ENABLE();			//ʹ��USART2ʱ��
		GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6; //PD5 T 6 R
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART2;	//����ΪUSART2
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//��ʼ��PD5,6
		
		HAL_NVIC_EnableIRQ(USART2_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART2_IRQn,3,2);			//��ռ���ȼ�3�������ȼ�3
#if EN_USART2_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
		HAL_NVIC_EnableIRQ(USART2_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART2_IRQn,3,2);			//��ռ���ȼ�3�������ȼ�3
#endif
	}
	else printf("��ʼ������\r\n");

}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//����Ǵ���1
	{
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer[0]!=0x0a)
					USART_RX_STA=0;//���մ���,���¿�ʼ
				else 
					USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer[0]==0x0d)
					USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}

	}
	else if(huart->Instance==USART2)//����Ǵ���2
	{
		if((USART2_RX_STA&0x8000)==0)//����δ���
		{
			if(USART2_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer2[0]!=0x0a)
					USART2_RX_STA=0;//���մ���,���¿�ʼ
				else 
					USART2_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer2[0]==0x0d)
					USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=aRxBuffer2[0] ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))
						USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}

	}
}
 
//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART1_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler)!=HAL_UART_STATE_READY)//�ȴ�����
	{
        timeout++;////��ʱ����
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler,(u8 *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
        timeout++; //��ʱ����
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 
#endif	
	



#if EN_USART2_RX   //���ʹ���˽���
//����2�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART2_RX_STA=0;       //����״̬���	
u8 end[3]={0xff,0xff,0xff};
u8 aRxBuffer2[RXBUFFERSIZE2];//HAL��ʹ�õĴ��ڽ��ջ���

void usart2_init(u32 bound)//Ĭ��9600��ԭװ9600
{
    //USART ��ʼ������
	USART2_Handler.Instance=USART2;					    //USART2
	USART2_Handler.Init.BaudRate=bound;				    //������
	USART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART2_Handler.Init.StopBits=UART_STOPBITS_1;		//һ��ֹͣλ
	USART2_Handler.Init.Parity=UART_PARITY_NONE;		//����żУ��λ
	USART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//��Ӳ������
	USART2_Handler.Init.Mode=UART_MODE_TX_RX;			    //�շ�ģʽ
	HAL_UART_Init(&USART2_Handler);					//HAL_UART_Init()��ʹ��USART2
  	HAL_UART_Receive_IT(&USART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE2);
	HAL_UART_Transmit(&USART2_Handler, end, 3, 0xffff);
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	//�ȴ����ͽ���
	HAL_UART_Transmit(&USART2_Handler, end, 3, 0xffff);
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	//�ȴ����ͽ���
	HAL_UART_Transmit(&USART2_Handler, end, 3, 0xffff);
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	//�ȴ����ͽ���
}
void USART2_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&USART2_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&USART2_Handler)!=HAL_UART_STATE_READY)//�ȴ�����
	{
        timeout++;////��ʱ����
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&USART2_Handler,(u8 *)aRxBuffer2, RXBUFFERSIZE2)!=HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
        timeout++; //��ʱ����
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 
void u2_print(u8 *a)
{
	u8 i=0;
	while(a[i]!=0)
	{		
		HAL_UART_Transmit(&USART2_Handler,&a[i],1,1000);
		while(__HAL_UART_GET_FLAG(&USART2_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
		i++;
	}

	HAL_UART_Transmit(&USART2_Handler, end, 3, 1000);
	while(__HAL_UART_GET_FLAG(&USART2_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
}
//���û�ӵ����ݾ���0���ӵ��˾���1
int data_buf(u8 *hmi_buf)
{
	int len,i;
	if(USART2_RX_STA!=0)
	{					   
		len=USART2_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		for(i=0;i<50;i++)
			hmi_buf[i]='\0';
		sprintf((char *)hmi_buf,(uint8_t*)USART2_RX_BUF);
		USART2_RX_STA=0;
		for(i=0;i<USART2_REC_LEN;i++)
			USART2_RX_BUF[i]='\0';
	}
	else 
		len=0;
	return len;
}
void text_print(u8 *name,u8* b)
{
	u8 out[40];
	sprintf((char *)out,"t%d.txt=\"%c\"",name,b);
	u2_print(out);
}


#endif



 




