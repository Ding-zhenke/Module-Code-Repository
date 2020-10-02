# include <AD9851.h>
# include <stdio.h>

u8 AD9851_FD=0x00; //倍频数
void (*_AD9851_Setfq)(u8 w0,double frequence);


//	AD9851_Init(ad9851_serial,1);
//	AD9851_Setfq(30000000);
void ad9851_reset()
{
ad9851_w_clk=0;
ad9851_fq_up=0;

ad9851_rest=0;
ad9851_rest=1;
ad9851_rest=0;
}

void ad9851_reset_serial()
{
ad9851_w_clk=0;
ad9851_fq_up=0;
//rest信号
ad9851_rest=0;
ad9851_rest=1;
ad9851_rest=0;
//w_clk信号
ad9851_w_clk=0;
ad9851_w_clk=1;
ad9851_w_clk=0;
//fq_up信号
ad9851_fq_up=0;
ad9851_fq_up=1;
ad9851_fq_up=0;
}
//***************************************************//
//          向ad9851中写命令与数据(并口)             //
//---------------------------------------------------//
void ad9851_wr_parrel(u8 w0,double frequence)
{
u32 w;
long int y;
double x;
//计算频率的HEX值
x=4294967295/180;//适合180M晶振/180为最终时钟频率（或30M六倍频）
//如果时钟频率不为180MHZ，修改该处的频率值，单位MHz ！！！
frequence=frequence/1000000;
frequence=frequence*x;
y=frequence;
//写w0数据
w=w0;   
AD9851_DataBus=w|(w^0xff)<<16;     //w0
ad9851_w_clk=1;
ad9851_w_clk=0;
//写w1数据
w=(y>>24);
AD9851_DataBus=w|(w^0xff)<<16;     //w0
ad9851_w_clk=1;
ad9851_w_clk=0;
//写w2数据
w=(y>>16);
AD9851_DataBus=w|(w^0xff)<<16;     //w0
ad9851_w_clk=1;
ad9851_w_clk=0;
//写w3数据
w=(y>>8);
AD9851_DataBus=w|(w^0xff)<<16;     //w0
ad9851_w_clk=1;
ad9851_w_clk=0;
//写w4数据
w=(y>>=0);
AD9851_DataBus=w|(w^0xff)<<16;     //w0
ad9851_w_clk=1;
ad9851_w_clk=0;
//移入始能
ad9851_fq_up=1;
ad9851_fq_up=0;
}
//***************************************************//
//          向ad9851中写命令与数据(串口)             //
//---------------------------------------------------//
void ad9851_wr_serial(u8 w0,double frequence)
{
unsigned char i,w;
long int y;
double x;
//计算频率的HEX值
x=4294967295/180;//适合180M晶振/180为最终时钟频率（或30M六倍频）
//如果时钟频率不为180MHZ，修改该处的频率值，单位MHz  ！！！
frequence=frequence/1000000;
frequence=frequence*x;
y=frequence;
//写w4数据
w=(y>>=0);
for(i=0;i<8;i++)
{
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//写w3数据
w=(y>>8);
for(i=0;i<8;i++)
{
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//写w2数据
w=(y>>16);
for(i=0;i<8;i++)
{
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//写w1数据
w=(y>>24);
for(i=0;i<8;i++)
{
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//写w0数据
w=w0;   
for(i=0;i<8;i++)
{
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//移入始能
ad9851_fq_up=1;
ad9851_fq_up=0;
}


void AD9851_IO_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure ; 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_6; 
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz ; 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 
   GPIO_Init(GPIOA ,&GPIO_InitStructure) ; 
//	 GPIO_ResetBits(GPIOA,GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_6);
}

void AD9851_Setfq(double fq)
{
	if(ad9851_ad9850) 
	{
//		fq *= 1.44;
		AD9851_FD = 0;
	}
	_AD9851_Setfq(AD9851_FD,fq);
}
//***************************************************//
//                   测试程序                 //
//---------------------------------------------------//
//输入：mode  ad9851_parallel 并口    ad9851_serial 串口
//FD：0：不倍频 1：2倍频
void AD9851_Init(u8 mode,u8 FD)
{
	AD9851_IO_Init();
	AD9851_DataBus=0x00|(~0x00)<<16;
	if(mode==ad9851_parallel)
	{
	_AD9851_Setfq=ad9851_wr_parrel;
	ad9851_reset();
	}
		else 
	{
	_AD9851_Setfq=ad9851_wr_serial;
	ad9851_reset_serial();
	}
	if(FD==1)AD9851_FD=0x01;
	else AD9851_FD=0x00;

}





