#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "ltdc.h"
#include "timer.h"
#include "sdram.h"
#include "usmart.h"
#include "math.h"
#include "arm_math.h"
#include "fft.h"
/************************************************
 ALIENTEK 阿波罗STM32F7开发板
 DSP FFT实验-HAL
 作者：Dcount
 基于正点原子F767模板开发
 update 2020.4.3
************************************************/


#define FFT_LENGTH		1024 		//FFT长度，默认是1024点FFT

float fft_inputbuf[FFT_LENGTH];	//FFT输入数组
float fft_outputbuf[FFT_LENGTH];	//FFT输出数组
float fft_outputpower[FFT_LENGTH];
float fft_outputA[FFT_LENGTH];
u8 timeout;

fft_choice instance;
void fft_init(fft_choice *S)
{
	S->ADC_bit=12;//12
	S->Fs=1.8e6;//采样速率
	S->ADC_Vpp=3.3;//V
	S->size=FFT_LENGTH;//总数据点数
	//窗函数选择
	//int data_size;//填0,具体算法还没弄懂
	S->winname=hanning;//窗函数名字
	//输出数据
	S->unit=dBmhz; //输出数据的单位
	S->altitude=fft_outputA;
	S->power=fft_outputpower;
	//float
	S->BW=3e6;
	//如果要看幅度和功率的话找两个数组来承接
	//S->power,S->altitude 指针
}


	
int main(void)
{
 	u8 key,t=0;
	float time; 
	u8 buf[50]; 
	u16 i; 
    
    Cache_Enable();                 //打开L1-Cache
    HAL_Init();				        //初始化HAL库
    Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
    delay_init(216);                //延时初始化
	uart_init(115200);		        //串口初始化
    LED_Init();                     //初始化LED
    KEY_Init();                     //初始化按键
    SDRAM_Init();                   //初始化SDRAM
    LCD_Init();                     //LCD初始化
    TIM3_Init(65535,108-1);        //1Mhz计数频率,最大计时6.5秒超出
	fft_init(&instance);
	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7");	
	LCD_ShowString(30,70,200,16,16,"DSP FFT TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/7/18");	
	LCD_ShowString(30,130,200,16,16,"KEY0:Run FFT");//显示提示信息 
	LCD_ShowString(30,160,200,16,16,"FFT runtime:");//显示FFT执行时间
 	POINT_COLOR=BLUE;		//设置字体为蓝色   
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			for(i=0;i<FFT_LENGTH;i++)//生成信号序列
			{
				 fft_inputbuf[i]=100+
				                   10*arm_sin_f32(2*PI*i/FFT_LENGTH)+
								   30*arm_sin_f32(2*PI*i*4/FFT_LENGTH)+
				                   50*arm_cos_f32(2*PI*i*8/FFT_LENGTH);	//生成输入信号实部
				//实际上这里要放ADC采样函数，返回值是数字值
				//范围是0~2^(adc_bit-1)
			}
			__HAL_TIM_SET_COUNTER(&TIM3_Handler,0);//重设TIM3定时器的计数器值
			timeout=0;
			cfft(&instance,fft_inputbuf,fft_outputbuf);
			time=__HAL_TIM_GET_COUNTER(&TIM3_Handler)+(u32)timeout*65536;//计算所用时间
			sprintf((char*)buf,"%0.3fms\r\n",time/1000);	
			LCD_ShowString(30+12*8,160,100,16,16,buf);	//显示运行时间		
			printf("\r\n%d point FFT runtime:%0.3fms\r\n",FFT_LENGTH,time/1000);
			printf("FFT Result:\r\n");
			for(i=0;i<FFT_LENGTH;i++)
			{
				printf("fft_outputbuf[%d]:%f\r\n",i,fft_outputbuf[i]);
			}
			for(i=0;i<FFT_LENGTH;i++)
			{
				printf("fft_outputA[%d]:%f\r\n",i,fft_outputA[i]);
			}
			for(i=0;i<FFT_LENGTH;i++)
			{
				printf("fft_outputpower[%d]:%f\r\n",i,fft_outputpower[i]);
			}
		}
		else 
			delay_ms(10);
		t++;
		if((t%10)==0)LED0_Toggle;		  
	}
}
