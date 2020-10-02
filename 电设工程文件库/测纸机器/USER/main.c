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
 ALIENTEK 阿波罗STM32F7开发板
 DSP-GUI实验
 作者：Dcount
 基于正点原子F767模板开发
 update 2020.7.21
************************************************/

#if 0
#define FFT_LENGTH		1024 		//FFT长度，默认是1024点FFT 
float fft_inputbuf[FFT_LENGTH];	//FFT输入数组
float fft_outputbuf[FFT_LENGTH];	//FFT输出数组
float fft_outputpower[FFT_LENGTH];
float fft_outputA[FFT_LENGTH];
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
	S->load_r = 50;// 负载电阻为50欧姆
	//输出数据
	S->unit=dBmhz; //输出数据的单位
	S->altitude=fft_outputA;
	S->power=fft_outputpower;
	//float
	S->BW=3e6;
	//如果要看幅度和功率的话找两个数组来承接
	//S->power,S->altitude 指针
}
#endif

#if 1
u8 timeout;						   //运行时间计数标志					
u8 buf[50];                        //运行时间
#endif
u32 test;
float cp;	
int main(void)
{
	u8 key,fontok=0; 
    Cache_Enable();                 //打开L1-Cache
    HAL_Init();				        //初始化HAL库
	Write_Through();                //开启强制透写！
	MPU_Memory_Protection();        //保护相关存储区域
    Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
    delay_init(216);                //延时初始化
	uart_init(115200);		        //串口初始化
	usart3_init(115200);  			//初始化串口3波特率为115200
	usart2_init(115200);			//初始化串口屏数据
	LED_Init();                     //初始化LED
    KEY_Init();                     //初始化按键
	u2_print("page0.j0.val=0");
	u2_print("page0.t1.txt=\"0%\"");	
    SDRAM_Init();                   //初始化SDRAM
//    LCD_Init();                     //LCD初始化
	usmart_dev.init(108);			//初始化usmart
	W25QXX_Init();				    //初始化W25Q256
//	//fft_init(&instance);			//FFT初始化
//	//TIM7_Init(999,216-1);			//1Mhz计数频率,最大计时6.5秒超出，运行时间计数
//	tp_dev.init(); 					//触摸屏初始化
//	//while(FDC2214_Init());		//fdc初始化
	my_mem_init(SRAMIN);            //初始化内部内存池
	my_mem_init(SRAMEX);            //初始化外部SDRAM内存池
	my_mem_init(SRAMDTCM);           //初始化内部CCM内存池
	f_mount(fs[0],"0:",1);          //挂载SD卡 
 	f_mount(fs[1],"1:",1);          //挂载SPI FLASH. 
	exfuns_init();		            	//为fatfs相关变量申请内存
	fontok=font_init();		//检查字库是否OK	
	if(fontok)//需要更新字库				 
	{
//		LCD_Clear(WHITE);		   	//清屏
// 		POINT_COLOR=RED;			//设置字体为红色	   	   	  
		while(SD_Init())			//检测SD卡
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
		key=update_font(20,110,16,"0:");//从SD卡更新
		while(key)//更新失败		
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
		//LCD_Clear(WHITE);//清屏	       
	}
	u2_print("page0.j0.val=20");
	u2_print("page0.t1.txt=\"初始化20%\"");
	atk_8266_test();
	while(1)
	{
//		u2_print("page0.t0.txt=\"改动成功\"");
//		delay_ms(500);
		//u2_print("t0.txt=\"hh\"");

//		//printf("page0.t0.txt=\"改动成功\"\r\n");
//		u4_receive(1);
		//HAL_UART_Transmit(&USART2_Handler,"OK",2,1000);
		//printf("修改成功了吗？还是卡主了");
//		timer_init(&timeout);
//		cp=Cap_Calculate(1);
//		printf("电容值：%f\r\n",cp);
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
