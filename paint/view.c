#include "view.h"
#include "lcd.h"
#include "delay.h"
#include "led.h"
#include "MATHF.h"
char startview(void)
{	
	char result=0;
	u8 key=0,i=0;
	LCD_Fill(50,140,180,220,RED);
	LCD_Fill(0,0,239,40,BLUE);
	lcd_draw_bline(4, 4, 4, 316,4,BLUE);
	lcd_draw_bline(236, 4, 236, 316,4,BLUE);
	LCD_Fill(0,310,240,320,BLUE);
	POINT_COLOR=MAGENTA;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F767"); 
	LCD_ShowString(30,70,200,16,16,"Artificial View");	
	LCD_ShowString(30,90,200,16,16,"MY_TEST");
	LCD_ShowString(30,110,200,16,16,"2019/7/21");
	POINT_COLOR=WHITE;
	LCD_ShowString(70,160,200,20,32,"START!");
	POINT_COLOR=BLACK;
	LCD_ShowString(8,250,200,20,16,"X:");
	LCD_ShowString(8,270,200,20,16,"Y:");
	while(1)
	{
		key=KEY_Scan(0);
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//������������
		{	
			if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				LED1_Toggle;
				POINT_COLOR=GREEN;
				delay_ms(10);//������ʱ
				LCD_ShowxNum(24,250,tp_dev.x[0],3,16,0);
				LCD_ShowxNum(24,270,tp_dev.y[0],3,16,0);
				if(tp_dev.x[0]<=180&&tp_dev.x[0]>50)
				{	
					if(tp_dev.y[0]<=220&&tp_dev.y[0]>=140)
					{
						result='S';
					}
				}
			}	
		}
		else delay_ms(5);
		LED0_Toggle;
		if(key==KEY0_PRES)	//KEY0����,��ִ��У׼����
		{
			LCD_Clear(WHITE);	//����
			TP_Adjust();  		//��ĻУ׼ 
			TP_Save_Adjdata();	 
			LCD_Clear(WHITE);	//����
		}
		i++;
		if(i%20==0)LED0_Toggle;
		if(result=='S') break;
	}	
	return result;
}
void interface(void)
{
	LCD_Clear(WHITE);
	LCD_Fill(0,0,239,40,BLUE);
	lcd_draw_bline(4, 4, 4, 316,4,BLUE);
	lcd_draw_bline(236, 4, 236, 316,4,BLUE);
	LCD_Fill(0,310,240,320,BLUE);
	LCD_Fill(0,0,70,40,LBBLUE);
}
void project1(void)//�����
{
	interface();
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Start RF"); 
	LCD_ShowString(30,70,200,16,16,"Channel1 f is:");	
	LCD_ShowString(30,90,200,16,16,"Channel2 f is:");
	LCD_ShowString(30,110,200,16,16,"Delay 400ms");
	delay_ms(400);
}
void project2(void)//ɨƵ
{
	interface();
	
	lcd_draw_bline(15,40,15,150,2,BLACK);//��ѹ��
	lcd_draw_bline(15,40,18,52,2,BLACK);//��ͷ
	lcd_draw_bline(15,40,12,52,2,BLACK);//��ͷ
	
	lcd_draw_bline(15,150,225,150,2,BLACK);//Ƶ����
	lcd_draw_bline(213,147,225,150,2,BLACK);//��ͷ
	lcd_draw_bline(213,153,225,150,2,BLACK);//��ͷ
	
	
	POINT_COLOR=RED;
	LCD_ShowChar(2,20,'U',24,1);
	LCD_ShowChar(220,153,'f',24,1);
	LCD_ShowString(15,170,200,16,16,"Channel1 f:          Hz "); 
	LCD_ShowString(15,190,200,16,16,"MAX Vp:           V");	
	LCD_ShowString(15,210,200,16,16,"Channel2 f:          Hz");
	LCD_ShowString(15,230,200,16,16,"MAX Vp:           V");
	LCD_ShowString(15,250,200,16,16,"Step f:           Hz");
}
/******************

����λ����a
С��λ����b


******************/
void show(u16 x,u16 y,double num,u16 color,int a,int b)
{
	int temp,X;
	double count;
	temp=num;
	POINT_COLOR=color;
	LCD_ShowxNum(x,y,temp,a,16,0x80);
	LCD_ShowChar(x+8*a,y,'.',16,0);
	count=num-temp;
	X=pow(10,b);
	count*=X;
	LCD_ShowxNum(x+8*a+8,y,count,b,16,0X80);
}

/****************************************************************
location:0Ƶ�ʣ�1�ǵ�ѹ
*p��ѹ����--�������飬���ǵ�ѹʵ��ֵ����
*freƵ������
length��������ĳ���һ��
color������ɫ����������ͬ�ź�
����˵����
����U-Fͼ��Ȼ���ҵ���ѹ��ֵ��Ӧ��Ƶ��
*****************************************************************/
void paint(int *p,u32 *fre,int length,u16 color,int max_location)
{
	int count;
	POINT_COLOR=color;
	for(count=0;count<length-1;count++)
	{
		lcd_draw_bline(17+count,p[count],17+(count+1),p[count+1],1,color);//����	
	}
	lcd_draw_bline(17,p[0],17,150,1,YELLOW);//��ʼƵ��
	LCD_ShowxNum(17,30,fre[0],8,12,1);
	lcd_draw_bline(217,p[200],217,150,1,YELLOW);//��ֹƵ��
	LCD_ShowxNum(180,30,fre[200],8,12,1);
	lcd_draw_bline(17+max_location,p[max_location],17+max_location,150,1,MAGENTA);//���ֵλ�ñ�ע
	POINT_COLOR=MAGENTA;
	LCD_ShowxNum(17+max_location,155,fre[max_location],8,12,0);
}
