//////////////////////////////////////////////////////////////////////////////////	 
//STM32H7����ģ��-HAL�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com		


//  ��������   : OLED �ӿ���ʾ����
//              ˵��: 
//              ----------------------------------------------------------------
//              VCC  ��5V��3.3v��Դ
//              GND  ��Դ��
//              D1   ��PB15��SDI��
//              D0   ��PB13��SCL��
//              CS   ��PB12 Ƭѡ    
//              RST  ��PB14 ϵͳ��λ
//              DC   ��PB1  ����/����  
//              NC   ����          
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

//STM32H7����ģ��-HAL�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com		

/**********************************************************************************************************/	

//-----------------OLED�˿ڶ���---------------- 

#define OLED_CS_Clr()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)	   	//PB12��0/CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET)	    	//PB12��1 

#define OLED_RST_Clr()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET)	    //PB14��0 //RES
#define OLED_RST_Set()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET)	      //PB14��1 

#define OLED_DC_Clr()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)		 	  //PB1��0 //DC
#define OLED_DC_Set()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)		      //PB1��1 

#define OLED_SCLK_Clr()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET)	   //PB13��0 //CLK
#define OLED_SCLK_Set()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET)	     //PB13��1 

#define OLED_SDIN_Clr()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET)  	  //PB15��0 //DIN
#define OLED_SDIN_Set()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET) 	     //PB15��1 

 

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����




/**********************************************************************************************************/

//STM32H7����ģ��-HAL�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com		

/**********************************************************************************************************/

//OLED�����ú���
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

//STM32H7����ģ��-HAL�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com		

/**********************************************************************************************************/









