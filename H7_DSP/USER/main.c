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
#include "numc.h"
/************************************************
 ALIENTEK 阿波罗STM32H7开发板 实验52_2
 DSP FFT实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

#define FFT_LENGTH		1024 		//FFT长度，默认是1024点FFT

float fft_inputbuf[FFT_LENGTH*2];	//FFT输入数组
float fft_outputbuf[FFT_LENGTH];	//FFT输出数组

u8 timeout;


int main(void)
{
 	u8 key,t=0;
	float time; 
	u8 buf[50]; 
	u16 i; 
    arm_cfft_radix4_instance_f32 scfft;
	
	Cache_Enable();                			//打开L1-Cache
	HAL_Init();				        		//初始化HAL库
	Stm32_Clock_Init(160,5,2,4);  		    //设置时钟,400Mhz 
	delay_init(400);						//延时初始化
	uart_init(115200);						//串口初始化
	usart2_init(115200);
	usmart_dev.init(200); 		    		//初始化USMART	
	LED_Init();								//初始化LED
	KEY_Init();								//初始化按键
	SDRAM_Init();                   		//初始化SDRAM
    TIM3_Init(65535,200-1);        //1Mhz计数频率,最大计时6.5秒超出 
	arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1);//初始化scfft结构体，设定FFT相关参数
    while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY1_PRES)
		{
			for(i=0;i<FFT_LENGTH;i++)//生成信号序列
			{
				 fft_inputbuf[2*i]=100+
				                   10*arm_sin_f32(2*PI*i/FFT_LENGTH)+
								   30*arm_sin_f32(2*PI*i*4/FFT_LENGTH)+
				                   50*arm_cos_f32(2*PI*i*8/FFT_LENGTH);	//生成输入信号实部
				 fft_inputbuf[2*i+1]=0;//虚部全部为0
			}
			__HAL_TIM_SET_COUNTER(&TIM3_Handler,0);//重设TIM3定时器的计数器值
			timeout=0;
			arm_cfft_radix4_f32(&scfft,fft_inputbuf);	//FFT计算（基4）
            time=__HAL_TIM_GET_COUNTER(&TIM3_Handler)+(u32)timeout*65536;//计算所用时间
			sprintf((char*)buf,"%0.3fms\r\n",time/1000);		
			arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);	//把运算结果复数求模得幅值 
			printf("\r\n%d point FFT runtime:%0.3fms\r\n",FFT_LENGTH,time/1000);
			printf("FFT Result:\r\n");
			for(i=0;i<FFT_LENGTH;i++)
			{
				printf("fft_outputbuf[%d]:%f\r\n",i,fft_outputbuf[i]);
			}
		}else delay_ms(10);
		t++;
		if((t%10)==0)
		{	
			LED2_Toggle;
		//	u2_print("t0.txt=\"\\ff\"");
		}			
	}		 
}

