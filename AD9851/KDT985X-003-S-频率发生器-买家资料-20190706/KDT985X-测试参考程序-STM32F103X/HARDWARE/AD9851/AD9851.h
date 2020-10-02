#ifndef __AD9851_H
#define __AD9851_H	 
#include "sys.h"

#define ad9851_ad9850 0///0-ad9851  1-ad9850

#define ad9851_w_clk    PAout(2)  
#define ad9851_fq_up    PAout(1) 
#define ad9851_rest     PAout(6) 
#define ad9851_bit_data PAout(3)  
#define AD9851_DataBus	GPIOA->BSRR

#define ad9851_parallel    	0 
#define ad9851_serial    		1 
#define ad9851_FD_0    			0 
#define ad9851_FD_6   			1 

void AD9851_Init(u8 mode,u8 FD);
void AD9851_Setfq(double fq);
void ad9851_wr_parrel(unsigned char w0,double frequence);
void ad9851_wr_serial(unsigned char w0,double frequence);



#endif

