
/**********************************************************
                       康威科技
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/
**********************************************************/


#include "adf4002.h"
#include "sys.h"

long int ReadData;

//long int Reg0x00 = 0x001F40;//r
//////long int Reg0x01 = 0x0DAC01;//n

long int functionReg0x02R = 0x0D80C2; //function FUNCTION  0000 1101 1000 0000 1100 0010 //function寄存器 设置R DIVIDER OUTPUT
long int initReg0x03R = 0x0D80C3;//init  0000 1101 1000 0000 1100 0011  //INITIALIZATION 寄存器 设置R DIVIDER OUTPUT

long int functionReg0x02N = 0x0D80A2; //function 0000 1101 1000 0000 1010 0010 //function寄存器 设置N DIVIDER OUTPUT
long int initReg0x03N = 0x0D80A3;//init  0000 1101 1000 0000 1010 0011   //INITIALIZATION寄存器 设置N DIVIDER OUTPUT

long int Reg0x02_LEDON  = 0x0D80B2; //MUXOUT CONTROL : DVDD
long int Reg0x02_LEDOFF = 0x0D80F2; //MUXOUT CONTROL: DGND


void Delay(unsigned int z)
{
  unsigned int i,j;
  
  for(i = z; i > 0; i--)
    for(j = 10; j > 0; j--) ;
}

void DelayMs(void)
{
  unsigned int i, j;
  
  for(i = 0; i < 1000; i++)
  {
    for(j = 0; j < 1000; j++)
    {
      Delay(1000);
    }
  }
}
//adf4002 IO口初始化
void InitADF4002(void)
{

	GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();					          //开启PA时钟
	
	GPIO_Initure.Pin= GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;	  //PC9,10,11
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		    //推挽输出
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  	//高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);     	     	//初始化PC9,10,11
	
	
	
	PLL_SCK(0);
	PLL_SDI(0);
	PLL_SEN(0);   
	//SendDataPll(initReg0x03R); //INIT	
  //SendDataPll(functionReg0x02R); //funtion
	SendDataPll(Reg0x02_LEDOFF); //INIT	
	//RDivideTest(1);

}

void SendDataPll(unsigned long int Data)          //传送的数据
{
  unsigned char i;

	PLL_SCK(0);
  PLL_SEN(0);  
  
  for(i = 0; i < 24; i++)
  {
    if(Data & 0x800000)
    {
      PLL_SDI(1);
    }
    else
    {
       PLL_SDI(0);
    }
    Data <<= 1;     
    PLL_SCK(1);
    
    Delay(100);
    
    PLL_SCK(0);
    Delay(100);
  }
  PLL_SDI(0);
  
  PLL_SEN(0);
  Delay(100);
  PLL_SEN(1);
}
//	OLED_ShowString(0, 2, (u8*)"RF   N:");
//	OLED_ShowString(0, 4, (u8*)"REF  R:");
//	OLED_ShowString(0, 6, (u8*)"maxN:8191R:16383");
void RDivideTest(u16 RData)
{
	u32 S_R = 0;
	
  S_R = Pre_R + (RData<<2) + R_Address;
  SendDataPll(functionReg0x02R); //funtion
  SendDataPll(S_R);
	SendDataPll(0X000001);	
}
void NDivideTest(u16 NData)
{
	u32 S_N = 0;
	
  S_N = Pre_N + (NData<<8) + N_Address;
  SendDataPll(functionReg0x02N); //funtion
	SendDataPll(0X000000);
  SendDataPll(S_N);

}














