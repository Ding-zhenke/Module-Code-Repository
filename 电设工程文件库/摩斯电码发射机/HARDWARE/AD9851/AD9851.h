#ifndef __AD9851_H
#define __AD9851_H	 
#include "sys.h"

#define ad9851_ad9850 0///0-ad9851  1-ad9850

#define ad9851_w_clk(n)		(n?HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_RESET)) 
#define ad9851_fq_up(n) 	(n?HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET)) 
#define ad9851_rest(n) 		(n?HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET)) 
#define ad9851_bit_data(n) 		(n?HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET))  
#define AD9851_DataBus	GPIOA->BSRR

#define ad9851_parallel    	0 
#define ad9851_serial    	1 
#define ad9851_FD_0    		0 
#define ad9851_FD_6   		1 

void AD9851_Init(u8 mode,u8 FD);
void AD9851_Setfq(double fq);
void ad9851_wr_parrel(unsigned char w0,double frequence);
void ad9851_wr_serial(unsigned char w0,double frequence);



#endif

