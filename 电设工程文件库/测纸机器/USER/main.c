#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "ltdc.h"
#include "timer.h"
#include "sdram.h"
#include "sdmmc_sdcard.h"
#include "usmart.h"
#include "math.h"
#include "arm_math.h"
#include "fft.h"
#include "numc.h"
#include "w25qxx.h"
#include "touch.h"
#include "24cxx.h"
#include "mpu.h" 
#include "fdc2214.h"
#include "ff.h"
#include "exfuns.h"
#include "malloc.h"
#include "fontupd.h"
#include "text.h"
#include "sdmmc_sdcard.h"
#include "common.h"
/************************************************
 ALIENTEK ������STM32F7������
 DSP-GUIʵ��
 ���ߣ�Dcount
 ��������ԭ��F767ģ�忪��
 update 2020.7.21
************************************************/

#if 0
#define FFT_LENGTH		1024 		//FFT���ȣ�Ĭ����1024��FFT 
float fft_inputbuf[FFT_LENGTH];	//FFT��������
float fft_outputbuf[FFT_LENGTH];	//FFT�������
float fft_outputpower[FFT_LENGTH];
float fft_outputA[FFT_LENGTH];
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
	S->load_r = 50;// ���ص���Ϊ50ŷķ
	//�������
	S->unit=dBmhz; //������ݵĵ�λ
	S->altitude=fft_outputA;
	S->power=fft_outputpower;
	//float
	S->BW=3e6;
	//���Ҫ�����Ⱥ͹��ʵĻ��������������н�
	//S->power,S->altitude ָ��
}
#endif

#if 1
u8 timeout;						   //����ʱ�������־					
u8 buf[50];                        //����ʱ��
#endif
u32 test;
float cp;	
int main(void)
{
	u8 key,fontok=0; 
    Cache_Enable();                 //��L1-Cache
    HAL_Init();				        //��ʼ��HAL��
	Write_Through();                //����ǿ��͸д��
	MPU_Memory_Protection();        //������ش洢����
    Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
    delay_init(216);                //��ʱ��ʼ��
	uart_init(115200);		        //���ڳ�ʼ��
	usart3_init(115200);  			//��ʼ������3������Ϊ115200
	usart2_init(115200);			//��ʼ������������
	LED_Init();                     //��ʼ��LED
    KEY_Init();                     //��ʼ������
	u2_print("page0.j0.val=0");
	u2_print("page0.t1.txt=\"0%\"");	
    SDRAM_Init();                   //��ʼ��SDRAM
//    LCD_Init();                     //LCD��ʼ��
	usmart_dev.init(108);			//��ʼ��usmart
	W25QXX_Init();				    //��ʼ��W25Q256
//	//fft_init(&instance);			//FFT��ʼ��
//	//TIM7_Init(999,216-1);			//1Mhz����Ƶ��,����ʱ6.5�볬��������ʱ�����
//	tp_dev.init(); 					//��������ʼ��
//	//while(FDC2214_Init());		//fdc��ʼ��
	my_mem_init(SRAMIN);            //��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);            //��ʼ���ⲿSDRAM�ڴ��
	my_mem_init(SRAMDTCM);           //��ʼ���ڲ�CCM�ڴ��
	f_mount(fs[0],"0:",1);          //����SD�� 
 	f_mount(fs[1],"1:",1);          //����SPI FLASH. 
	exfuns_init();		            	//Ϊfatfs��ر��������ڴ�
	fontok=font_init();		//����ֿ��Ƿ�OK	
	if(fontok)//��Ҫ�����ֿ�				 
	{
//		LCD_Clear(WHITE);		   	//����
// 		POINT_COLOR=RED;			//��������Ϊ��ɫ	   	   	  
		while(SD_Init())			//���SD��
		{
			//LCD_ShowString(60,70,200,16,16,"SD Card Failed!");
			u2_print("page0.t1.txt=\"SD Card Failed!\"");
			delay_ms(200);
			u2_print("page0.t1.txt=\"\"");
			delay_ms(200);
//			LCD_Fill(60,70,200+60,70+16,WHITE);
//			delay_ms(200);		    
		}								 						    
//		LCD_ShowString(60,70,200,16,16,"SD Card OK");
//		LCD_ShowString(60,90,200,16,16,"Font Updating...");
		u2_print("page0.t1.txt=\"SD Card OK,Font Updating...\"");
		key=update_font(20,110,16,"0:");//��SD������
		while(key)//����ʧ��		
		{			 		  
			//LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
			u2_print("page0.t1.txt=\"Font Update Failed!\"");
			delay_ms(200);
			u2_print("page0.t1.txt=\"\"");
			delay_ms(200);
//			LCD_Fill(20,110,200+20,110+16,WHITE);
//			delay_ms(200);		       
		} 		  
		//LCD_ShowString(60,110,200,16,16,"Font Update Success!");
		u2_print("page0.t1.txt=\"Font Update Success!\"");
		delay_ms(1000);	
		//LCD_Clear(WHITE);//����	       
	}
	u2_print("page0.j0.val=20");
	u2_print("page0.t1.txt=\"��ʼ��20%\"");
	atk_8266_test();
	while(1)
	{
//		u2_print("page0.t0.txt=\"�Ķ��ɹ�\"");
//		delay_ms(500);
		//u2_print("t0.txt=\"hh\"");

//		//printf("page0.t0.txt=\"�Ķ��ɹ�\"\r\n");
//		u4_receive(1);
		//HAL_UART_Transmit(&USART2_Handler,"OK",2,1000);
		//printf("�޸ĳɹ����𣿻��ǿ�����");
//		timer_init(&timeout);
//		cp=Cap_Calculate(1);
//		printf("����ֵ��%f\r\n",cp);
//		for(i=0;i<4;i++)
//		{
//			test = FDC_Read(RCOUNT_CH0+i);
//			printf("ch%d = %d\r\n",i,test);
//		}
//		test = FDC_Read(DEVICE_ID);
//		
//		printf("id = %x\r\n",test);
//		
//		timer(timeout,buf);

	}
	
}
