//////////////////////////////////////////////////////////////////////////////////	 
//STM32H7工程模板-HAL库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com		


//  功能描述   : OLED 接口演示例程
//              说明: 
//              ----------------------------------------------------------------
//              VCC  接5V或3.3v电源
//              GND  电源地
//              D1   接PB15（SDI）
//              D0   接PB13（SCL）
//              CS   接PB12 片选    
//              RST  接PB14 系统复位
//              DC   接PB1  命令/数据  
//              NC   悬空          
//              ----------------------------------------------------------------
//******************************************************************************/

#ifndef __OLED_H
#define __OLED_H	

#include "sys.h"
#include "stdlib.h"	  


#define SIZE 16

#define Max_Column	128
#define Max_Row		64


/**********************************************************************************************************/

//STM32H7工程模板-HAL库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com		

/**********************************************************************************************************/	

//-----------------OLED端口定义---------------- 

#define OLED_CS_Clr()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)	   	//PB12置0/CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET)	    	//PB12置1 

#define OLED_RST_Clr()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET)	    //PB14置0 //RES
#define OLED_RST_Set()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET)	      //PB14置1 

#define OLED_DC_Clr()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)		 	  //PB1置0 //DC
#define OLED_DC_Set()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)		      //PB1置1 

#define OLED_SCLK_Clr()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET)	   //PB13置0 //CLK
#define OLED_SCLK_Set()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET)	     //PB13置1 

#define OLED_SDIN_Clr()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET)  	  //PB15置0 //DIN
#define OLED_SDIN_Set()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET) 	     //PB15置1 

 

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据




/**********************************************************************************************************/

//STM32H7工程模板-HAL库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com		

/**********************************************************************************************************/

//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 








/**********************************************************************************************************/

//STM32H7工程模板-HAL库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com		

/**********************************************************************************************************/









