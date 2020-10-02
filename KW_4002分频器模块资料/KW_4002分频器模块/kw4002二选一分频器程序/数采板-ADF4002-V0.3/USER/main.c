/**********************************************************
                       康威电子					 
功能：输出经R分频后频率，按键操作修改R值。
接口：
				模块		STM32
				GND			GND
				PLL_SCK	PCout(11)
				PLL_SDI	PCout(10)
				PLL_SEN	PCout(9)
				
时间：2019/9/9
版本：1.0
作者：康威电子
其他：

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
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
	delay_init(72);	     //延时初始化
	delay_ms(200);
	NVIC_Configuration();
	uart_init(9600);   //串口初始化
	key_init();//按键初始化
	delay_ms(200);
	initial_lcd();//液晶初始化
	InitADF4002();
	 
	LCD_Clear();
	LCD_Show_CEStr(38,0,"分频器");
	OLED_ShowString(0, 2, (u8*)"RF   N:");
	OLED_ShowString(0, 4, (u8*)"REF  R:");
	OLED_ShowString(0, 6, (u8*)"maxN:8191R:16383");
	USBRelinkConfig();
	while(1)
	{
		KeyRead();//按键读取
		Set_PointFre(Keycode, 0);//按键处理
		if(_return){_return=0;LCD_Refresh_Gram();}//更新显示
		KEY_EXIT();
		USB_TxWrite((u8*)"usb test\r\n", 12);
	}
 }

 
 
 
 
 
 
 
 
 
