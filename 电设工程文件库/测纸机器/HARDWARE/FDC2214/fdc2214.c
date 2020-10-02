#include "fdc2214.h"
#include "delay.h"
#include "led.h"

u32 Data_FDC;
//FDC寄存器操作***********************************************************************

/*FDC2214设置函数
 *IIC写2个字节 
 *reg:寄存器地址
 *data1:数据1
 *data2:数据2
 *返回值:0    正常
 *     其他  错误代码
*/
u8 Set_FDC2214(u8 reg,u8 MSB,u8 LSB) 				 
{ 
    IIC_Start(); 
	IIC_Send_Byte((FDC2214_ADDR<<1)|0);//发送器件地址+写命令	
	if(IIC_Wait_Ack())	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);//写寄存器地址
    IIC_Wait_Ack();		 //等待应答 
	IIC_Send_Byte(MSB);  //发送数据1
	if(IIC_Wait_Ack())	 //等待ACK
	{
		IIC_Stop();	 
		return 1;		 
	}		 
	IIC_Send_Byte(LSB);  //发送数据2
	if(IIC_Wait_Ack())	 //等待ACK
	{
		IIC_Stop();	 
		return 1;		 
	}	
    IIC_Stop();	 
	return 0;
}

/*读取FDC2214寄存器数据
 *IIC读2个字节 
 *reg:寄存器地址 
 *返回值:读到的数据
 */
u16 FDC_Read(u8 reg)
{
	u16 res;
    IIC_Start(); 
	IIC_Send_Byte((FDC2214_ADDR<<1)|0);//发送器件地址+写命令	
	IIC_Wait_Ack();		//等待应答 
    IIC_Send_Byte(reg);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答
    IIC_Start();
	IIC_Send_Byte((FDC2214_ADDR<<1)|1);//发送器件地址+读命令	
    IIC_Wait_Ack();		//等待应答 
	res=IIC_Read_Byte(1)<<8;//读取数据,发送ACK	  
	res|=IIC_Read_Byte(0);//读取数据,发送nACK
    IIC_Stop();			//产生一个停止条件
	return res;		
}

/*得到通道x转换后的数据
 *index:0 -> CH0
 *      1 -> CH1
 *      2 -> CH2
 *      3 -> CH3
*返回值:读到的数据
 */
//u16 FCD2214_ReadCH(u8 index) 
//{
//	u16 result;
//	switch(index)
//	{
//		case 0:
//		  result = FDC_Read(DATA_CH0)&0x0FFF;
//		  result = (result<<4)|(FDC_Read(DATA_LSB_CH0)>>12);
//			break;
//		case 1:
//			result = FDC_Read(DATA_CH1)&0x0FFF;
//		  result = (result<<4)|(FDC_Read(DATA_LSB_CH1)>>12);
//			break;
//		case 2:
//			result = FDC_Read(DATA_CH2)&0x0FFF;
//		  result = (result<<4)|(FDC_Read(DATA_LSB_CH2)>>12);
//			break;
//		case 3:
//			result = FDC_Read(DATA_CH3)&0x0FFF;
//		  result = (result<<4)|(FDC_Read(DATA_LSB_CH3)>>12);
//			break;
//		default:break;
//	}
//	return result;
//}

u32 FCD2214_ReadCH(u8 index) 
{
	u32 result;
	switch(index)
	{
		case 0:
		  result = FDC_Read(DATA_CH0)&0x0FFF;
		  result = (result<<16)|(FDC_Read(DATA_LSB_CH0));
			break;
		case 1:
			result = FDC_Read(DATA_CH1)&0x0FFF;
		  result = (result<<16)|(FDC_Read(DATA_LSB_CH1));
			break;
		case 2:
			result = FDC_Read(DATA_CH2)&0x0FFF;
		  result = (result<<16)|(FDC_Read(DATA_LSB_CH2));
			break;
		case 3:
			result = FDC_Read(DATA_CH3)&0x0FFF;
		  result = (result<<16)|(FDC_Read(DATA_LSB_CH3));
			break;
		default:break;
	}
	result =result&0x0FFFFFFF;
	return result;
}

/*FDC2214初始化函数
 *返回值:0：初始化正常
 *       1：不正常
 */
u8 FDC2214_Init(void)
{
	u16 res;
	IIC_Init();
	
	//检测ID是否正确，再初始化寄存器
	res=FDC_Read(MANUFACTURER_ID);
	printf("ID=%x\r\n",res);
	if(res==0x5449)
	{
		
		//设置Set_FDC2214寄存器
		Set_FDC2214(RCOUNT_CH0,0x34,0xFB);//参考计数转换间隔时间（T=(RCOUNT_CH0*16)/Frefx）
		Set_FDC2214(RCOUNT_CH1,0x34,0xFB);		
		Set_FDC2214(RCOUNT_CH2,0x34,0xFB);
		Set_FDC2214(RCOUNT_CH3,0x34,0xFB);	
		Set_FDC2214(SETTLECOUNT_CH0,0x00,0x1B);//转换之前的稳定时间（T=(SETTLECOUNT_CHx*16)/Frefx）
		Set_FDC2214(SETTLECOUNT_CH1,0x00,0x1B);
		Set_FDC2214(SETTLECOUNT_CH2,0x00,0x1B);
		Set_FDC2214(SETTLECOUNT_CH3,0x00,0x1B);
		
		Set_FDC2214(CLOCK_DIVIDERS_C_CH0,0x20,0x02);//选择在0.01MHz ~ 10MHz的传感器频率
		Set_FDC2214(CLOCK_DIVIDERS_C_CH1,0x20,0x02);//Frefx = Fclk = 43.4MHz/2(2分频)
		Set_FDC2214(CLOCK_DIVIDERS_C_CH2,0x20,0x02);//CHx_REF_DIVIDER=2;CHx_FIN_SEL=2
		Set_FDC2214(CLOCK_DIVIDERS_C_CH3,0x20,0x02);
		
		Set_FDC2214(DRIVE_CURRENT_CH0,0x78,0x00);//0.146ma（传感器时钟建立+转换时间的驱动电流）
		Set_FDC2214(DRIVE_CURRENT_CH1,0x78,0x00);
		Set_FDC2214(DRIVE_CURRENT_CH2,0x78,0x00);
		Set_FDC2214(DRIVE_CURRENT_CH3,0x78,0x00);
		
		Set_FDC2214(ERROR_CONFIG,0x00,0x00);//全部禁止错误汇报
		
		Set_FDC2214(MUX_CONFIG,0xC2,0x0D);//通道0，1，2 ，3；选择10Mhz为超过振荡槽振荡频率的最低设置，多通道，四通道
		
		Set_FDC2214(CONFIG,0x14,0x01);
	}
	else return 1;
	
	return 0;
}

/*测量线路电容
 *index:0：线路0
 *      1：线路1
 *      2：线路2
 *      3：线路3
 *返回值：线路总电容C
 */
float Cap_Calculate(u8 index)
{
	float Cap;
	Data_FDC = FCD2214_ReadCH(index);
//	Cap = 56645.763f/((float)Data_FDC);
//	return ((Cap*Cap)-33);
	Cap = 232021045.248/(Data_FDC);
	return (Cap*Cap);
}



