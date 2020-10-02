/**********************************************************
                       ��������					 
���ܣ������R��Ƶ��Ƶ�ʣ����������޸�Rֵ��
�ӿڣ�
				ģ��		STM32
				GND			GND
				PLL_SCK	PCout(11)
				PLL_SDI	PCout(10)
				PLL_SEN	PCout(9)
				
ʱ�䣺2019/9/9
�汾��1.0
���ߣ���������
������

������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://kvdz.taobao.com/

**********************************************************/
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "exti.h"
#include "wdg.h"
#include "timer.h"
#include "stmflash.h"
#include "usb_lib.h"
#include "math.h"
#include "hw_config.h"
#include "string.h"
#include "stdlib.h"
#include "lcd.h"
#include "task_manage.h"
#include "adf4002.h"

int main(void)
{
	 
  SystemInit();
	delay_init(72);	     //��ʱ��ʼ��
	delay_ms(200);
	NVIC_Configuration();
	uart_init(9600);   //���ڳ�ʼ��
	key_init();//������ʼ��
	delay_ms(200);
	initial_lcd();//Һ����ʼ��
	InitADF4002();
	 
	LCD_Clear();
	LCD_Show_CEStr(38,0,"��Ƶ��");
	OLED_ShowString(0, 2, (u8*)"RF   N:");
	OLED_ShowString(0, 4, (u8*)"REF  R:");
	OLED_ShowString(0, 6, (u8*)"maxN:8191R:16383");
	USBRelinkConfig();
	while(1)
	{
		KeyRead();//������ȡ
		Set_PointFre(Keycode, 0);//��������
		if(_return){_return=0;LCD_Refresh_Gram();}//������ʾ
		KEY_EXIT();
		USB_TxWrite((u8*)"usb test\r\n", 12);
	}
 }

 
 
 
 
 
 
 
 
 
