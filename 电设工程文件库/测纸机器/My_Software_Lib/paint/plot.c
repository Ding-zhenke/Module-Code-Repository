#include "plot.h"
#include "math.h"
/******************

整数位数：a
小数位数：b


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


