#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h"
#include "key.h"
#include "sdram.h"
#include "usmart.h"
#include "w25qxx.h"
#include "timer.h"
#include "arm_math.h"
#include "adf4002.h"
#include "numc.h"
#include "oled.h"
# include <AD9851.h>
/************************************************

************************************************/


extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值 
extern unsigned long int count;   //ETR值
void clear(u8 *buf)
{
	int i;
	for(i=0;i<50;i++)
		buf[i]='\0';
}


void decode_init()
{
	GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();		//开启PB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_1;			              //PB1
    GPIO_Initure.Mode=GPIO_MODE_INPUT  ;  		    //输入
    GPIO_Initure.Pull=GPIO_NOPULL;         			    //无
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  	//高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     	     	//初始化PA1
	   
	 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);	//PA1置1 

}
#define RP (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1))
int SOS[11]={0,0,0,1,1,1,0,0,0};//9
int _83[16]={1,1,1,0,0,0,0,0,1,1,0,0,1,1};//14
int OK[7]  ={1,1,1,1,0,1};//6
void decode(u8 *d,u8 *buf)
{
	int i,j,avg;
	int time[14];
	int code[14];
	double width;
	u8 count;
	for(j=0;j<14;j++) 
	{
		while(RP!=1)
		{
			LED2_Toggle;
			delay_ms(2);
		}
		if(RP==1)
		{
			while(RP!=0)
			{
				delay_ms(1);
				i++;
			}
			time[j]=i;
			i=0;			
		}
	}
	avg=0;
	for(i=0;i<14;i++)
	{
		avg += time[i];
	}
	avg/=14;
	width=0;
	count=0;
	for(i=0;i<14;i++)
	{
		if(time[i]>avg)
			code[i]=1;		
		else 
		{
			code[i]=0;
			width+=time[i];
			count++;
		}
	}
	width=width/count;
	if(width<20)
		sprintf((char *)buf,"width:none");
	else
		sprintf((char *)buf,"width:%0.1fms",width);
	for(i=0;i<14;i++)
	{
		printf("t:%d=%d,avg=%d\r\n",time[i],code[i],avg);
		d[i]=code[i];
	}
}	
void display(u8 *code,u8 *decode)
{
	int i,k;
	k=0;
	for(i=0;i<14;i++)
	{
		k += code[i];
	}
	clear(decode);
	k=14-k;
	if(k>6&&k<8)//如果只有15个1，那必然是OK
	{
		sprintf((char *)decode,"code:OK");
	}
	else if(k>8&k<12)//如果只有6个1，必然是SOS
	{
		sprintf((char *)decode,"code:SOS");
	}
	else if(k<4&&k>1)	
		sprintf((char *)decode,"code:83#");
	else 
		sprintf((char *)decode,"code:NONE");
}
#define RFdiv 10
int main(void)
{
	u8 t=0;
	u8 buf[50]; 
	u8 code[14];
	long temp;
	u16 i;
	Cache_Enable();                			//打开L1-Cache
	HAL_Init();				        		//初始化HAL库
	Stm32_Clock_Init(160,5,2,4);  		    //设置时钟,400Mhz 
	delay_init(400);						//延时初始化
	uart_init(115200);						//串口初始化
	LED_Init();								//初始化LED
	KEY_Init();								//初始化按键
	SDRAM_Init();                   		//初始化SDRAM
	InitADF4002();
	
//	IIC_Init();                //初始化IIC的IO口
//	OLED_Init();

	InitADF4002();
	RDivideTest(RFdiv);
//	OLED_CLS();
//	OLED_P8x16Str(5,6,"start");
	decode_init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	AD9851_Init(ad9851_serial,1);
	AD9851_Setfq(507e5);
	while(1)
	{
		i=0;
//		while(RP!=1)
//		{
//			delay_ms(5);
//			i++;
//			if(i>100)
//			{
//				i=199;
//				break;
//			}
//		}
//		if(i<190)
//		{
//			__HAL_TIM_SetCounter(&htim2,0);
//			__HAL_TIM_SetCounter(&htim3,0);
//			delay_ms(10);
//			temp=(count*100)*RFdiv;
//			sprintf((char *)buf,"f:%ldHz",temp);
//			printf("%ld\r\n",count);
//			OLED_P8x16Str(0,4,buf);
//		}	
//		
//		decode(code,buf);
//		OLED_CLS();		
//		OLED_P8x16Str(0,0,buf);
//        display(code,buf);
//		OLED_P8x16Str(0,2,buf);
		t++;
		delay_ms(100);
//		i=RP;
//		printf("%d\r\n",i);151210

		
		if((t%2)==0)
		{	
			LED2_Toggle;
		//	u2_print("t0.txt=\"\\ff\"");
			sprintf((char *)buf,"hello:%d",t);
			OLED_P8x16Str(5,6,buf);
		}			
	}		 
}

