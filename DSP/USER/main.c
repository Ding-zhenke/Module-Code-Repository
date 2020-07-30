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
 ALIENTEK ������STM32F7������
 DSP FFTʵ��-HAL
 ���ߣ�Dcount
 ��������ԭ��F767ģ�忪��
 update 2020.4.3
************************************************/


#define FFT_LENGTH		1024 		//FFT���ȣ�Ĭ����1024��FFT

float fft_inputbuf[FFT_LENGTH];	//FFT��������
float fft_outputbuf[FFT_LENGTH];	//FFT�������
float fft_outputpower[FFT_LENGTH];
float fft_outputA[FFT_LENGTH];
u8 timeout;

fft_choice instance;
void fft_init(fft_choice *S)
{
	S->ADC_bit=12;//12
	S->Fs=1.8e6;//��������
	S->ADC_Vpp=3.3;//V
	S->size=FFT_LENGTH;//�����ݵ���
	//������ѡ��
	//int data_size;//��0,�����㷨��ûŪ��
	S->winname=hanning;//����������
	//�������
	S->unit=dBmhz; //������ݵĵ�λ
	S->altitude=fft_outputA;
	S->power=fft_outputpower;
	//float
	S->BW=3e6;
	//���Ҫ�����Ⱥ͹��ʵĻ��������������н�
	//S->power,S->altitude ָ��
}


	
int main(void)
{
 	u8 key,t=0;
	float time; 
	u8 buf[50]; 
	u16 i; 
    
    Cache_Enable();                 //��L1-Cache
    HAL_Init();				        //��ʼ��HAL��
    Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
    delay_init(216);                //��ʱ��ʼ��
	uart_init(115200);		        //���ڳ�ʼ��
    LED_Init();                     //��ʼ��LED
    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //��ʼ��SDRAM
    LCD_Init();                     //LCD��ʼ��
    TIM3_Init(65535,108-1);        //1Mhz����Ƶ��,����ʱ6.5�볬��
	fft_init(&instance);
	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7");	
	LCD_ShowString(30,70,200,16,16,"DSP FFT TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/7/18");	
	LCD_ShowString(30,130,200,16,16,"KEY0:Run FFT");//��ʾ��ʾ��Ϣ 
	LCD_ShowString(30,160,200,16,16,"FFT runtime:");//��ʾFFTִ��ʱ��
 	POINT_COLOR=BLUE;		//��������Ϊ��ɫ   
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			for(i=0;i<FFT_LENGTH;i++)//�����ź�����
			{
				 fft_inputbuf[i]=100+
				                   10*arm_sin_f32(2*PI*i/FFT_LENGTH)+
								   30*arm_sin_f32(2*PI*i*4/FFT_LENGTH)+
				                   50*arm_cos_f32(2*PI*i*8/FFT_LENGTH);	//���������ź�ʵ��
				//ʵ��������Ҫ��ADC��������������ֵ������ֵ
				//��Χ��0~2^(adc_bit-1)
			}
			__HAL_TIM_SET_COUNTER(&TIM3_Handler,0);//����TIM3��ʱ���ļ�����ֵ
			timeout=0;
			cfft(&instance,fft_inputbuf,fft_outputbuf);
			time=__HAL_TIM_GET_COUNTER(&TIM3_Handler)+(u32)timeout*65536;//��������ʱ��
			sprintf((char*)buf,"%0.3fms\r\n",time/1000);	
			LCD_ShowString(30+12*8,160,100,16,16,buf);	//��ʾ����ʱ��		
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
