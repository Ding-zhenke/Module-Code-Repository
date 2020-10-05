#ifndef _KEY_H
#define _KEY_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 

//KEY��������	   

//STM32H7����ģ��-HAL�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

//////////////////////////////////////////////////////////////////////////////////


#define KEY1        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)  //KEY1����PE3
#define KEY2        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)  //KEY2����PC5



#define KEY1_PRES	1	//KEY1���º󷵻�ֵ
#define KEY2_PRES	2	//KEY2���º󷵻�ֵ

void KEY_Init(void);  //����IO��ʼ������
u8 KEY_Scan(u8 mode); //����ɨ�躯��
#endif











//STM32H7����ģ��-HAL�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	





