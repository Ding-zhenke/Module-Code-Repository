#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h"
#include "key.h"
#include "usmart.h"
#include "timer.h"
#include "arm_math.h"
//#include "AD9851.h"
#include "numc.h"
#include "RF_SWITCH.h"
#include "string.h"
#include "stdio.h"
u8 timeout;
//sos 83#,ok
typedef struct{
	int width;//脉宽
	int f;//频率
	int loc;//内容
	int len;//接收数据长度
}tag;
int SOS[11]={0,0,0,2,1,1,1,2,0,0,0};
int _83[16]={1,1,1,0,0,2,0,0,0,1,1,2,0,0,1,1};
int OK[7]={1,1,1,2,1,0,1};
int all[]={0,0,0,2,1,1,1,2,0,0,0,2,1,1,1,0,0,2,0,0,0,1,1,2,0,0,1,1,2,1,1,1,2,1,0,1};
u8 hmi_buf[50];
void str_for(u8 *result,u8 *t,int start,int end)
{
	int i,len;
	len = end-start;
	for(i=0;i<20;i++)
	{
		result[i]='\0';
	}
	for(i=0;i<len-1;i++)
	{
		result[i]=t[start+1+i];
	}
	//result[i+1]='\0';
}

void m_t(u8 *t,tag *s)
{
	int i=0,c=0,T=0,F=0,end=0;
	char *p;
	i=0;
	while(t[i]!='\0')
	{
		if(t[i]=='c')
			c=i;
		else if(t[i]=='F')
			F=i;
		else if(t[i]=='T')
			T=i;
		i++;
	}
	end=i;
	//printf("%c,%c,%c\r\n",t[c],t[F],t[T]);
	//先拿脉宽
	str_for((u8 *)p,t,T,F);
	sscanf(p,"%d",&s->width);//
	printf("%d\r\n",s->width);
	//f
	str_for((u8 *)p,t,F,c);
	sscanf(p,"%d",&s->f);
	printf("%d\r\n",s->f);
	//AD9851_Setfq((double)(s->f));
	//content
	str_for((u8 *)p,t,c,end);
	s->loc=0;
	i=memcmp(p,"SOS",3);
	if(i==0)
		s->loc=1;
	i=memcmp(p,"83#",4);
	if(i==0)
		s->loc=2;
	i=memcmp(p,"OK",3);
	if(i==0)
		s->loc=3;
	i=memcmp(p,"SOS83#OK",7);
	if(i==0)
		s->loc=4;
	
}
void mos(tag *s)
{
	int i;
	int *t;
	double time;
	time=s->width/1e2;
	RF_control(1);
	delay_ms(3*time);
	if(s->loc==1)
	{
		t=SOS;
		s->len=11;
	}
	else if(s->loc==2)
	{	
		t=_83;
		s->len=16;
	}
	else if(s->loc==3)
	{
		t=OK;
		s->len=7;
	}
	else if(s->loc==4)
	{
		t=all;
		s->len=11+16+7;
	}
	else ;//printf("error\r\n");
	for(i=0;i<s->len;i++)
	{
		if(t[i]==2)//字符间隔
		{
			RF_control(1);
			delay_ms(2*time);
		}			
		else
		{
			if(t[i]==0)
			{
				RF_control(0);
				delay_ms(time);
				RF_control(1);
				delay_ms(time);
			}
			else if(t[i]==1)
			{
				RF_control(0);
				delay_ms(3*time);
				RF_control(1);
				delay_ms(time);
			}
		}
	}
	RF_control(1);
	delay_ms(3*time);
}
	
int main(void)
{
 	u8 t=0;
	//u8 buf[50]; 
	int len;
	tag s;
	Cache_Enable();                			//打开L1-Cache
	HAL_Init();				        		//初始化HAL库
	Stm32_Clock_Init(160,5,2,4);  		    //设置时钟,400Mhz 
	delay_init(400);						//延时初始化
	uart_init(115200);						//串口初始化
	usmart_dev.init(200); 		    		//初始化USMART	
	LED_Init();								//初始化LED
	KEY_Init();								//初始化按键
	RF_SWITCH_Init();
	usart2_init(115200);
	//AD9851_Init(ad9851_serial,1);
	//AD9851_Setfq(30e6);
	s.loc=3;
	s.width=5000;
	while(1)	
	{
		len=data_buf(hmi_buf);
		s.len=len;
		printf("len=%d\r\n",s.len);
		if(len!=0)
		{
			//printf("%s\r\n",hmi_buf);
			m_t(hmi_buf,&s);
			
		}
		else  
		{
			//printf("未收到\r\n");
		;
		}

		mos(&s);
	
		
		t++;
		if((t%10)==0)
		{			
			LED2_Toggle;
			u2_print("page0.b4.txt=\"ff\"");				
		}	
		
	}		 
}

