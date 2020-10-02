#ifndef __task_manage_H
#define __task_manage_H
#include "stm32f10x.h"
#include "lcd.h"
#include "key.h"

#define Max_Task 4

extern u32 SysTimer;
extern u8 _return;
extern u16 rDivideValue;

void USB_Port_Set(u8 usb_sw);
void USBRelinkConfig(void);
u8 Task_Delay(u32 delay_time, u8* delay_ID);
u8 TaskCycleDelay(u32 delay_time, u8* Last_delay_ID, u8* Self_delay_ID);
void welcome_KW(void);
void Main_Menu(u32 Key_Value, u8* Task_ID);
void LCD_Show_ModeCEInfo(u16 x0, u8 start_info, u8 current_deal_info);
void Copybuf2dis(u8 *source, u8 dis[10], u8  dispoint ,u8 MODNR);
void Set_PointFre(u32 Key_Value, u8* Task_ID);
void Sweep_Fre(u32 Key_Value, u8* Task_ID);
void Move_Pha(u32 Key_Value, u8* Task_ID);
void Jump_Fre(u32 Key_Value, u8* Task_ID);

#endif
