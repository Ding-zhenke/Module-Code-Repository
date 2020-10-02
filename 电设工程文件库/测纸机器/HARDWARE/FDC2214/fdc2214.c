#include "fdc2214.h"
#include "delay.h"
#include "led.h"

u32 Data_FDC;
//FDC�Ĵ�������***********************************************************************

/*FDC2214���ú���
 *IICд2���ֽ� 
 *reg:�Ĵ�����ַ
 *data1:����1
 *data2:����2
 *����ֵ:0    ����
 *     ����  �������
*/
u8 Set_FDC2214(u8 reg,u8 MSB,u8 LSB) 				 
{ 
    IIC_Start(); 
	IIC_Send_Byte((FDC2214_ADDR<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);//д�Ĵ�����ַ
    IIC_Wait_Ack();		 //�ȴ�Ӧ�� 
	IIC_Send_Byte(MSB);  //��������1
	if(IIC_Wait_Ack())	 //�ȴ�ACK
	{
		IIC_Stop();	 
		return 1;		 
	}		 
	IIC_Send_Byte(LSB);  //��������2
	if(IIC_Wait_Ack())	 //�ȴ�ACK
	{
		IIC_Stop();	 
		return 1;		 
	}	
    IIC_Stop();	 
	return 0;
}

/*��ȡFDC2214�Ĵ�������
 *IIC��2���ֽ� 
 *reg:�Ĵ�����ַ 
 *����ֵ:����������
 */
u16 FDC_Read(u8 reg)
{
	u16 res;
    IIC_Start(); 
	IIC_Send_Byte((FDC2214_ADDR<<1)|0);//����������ַ+д����	
	IIC_Wait_Ack();		//�ȴ�Ӧ�� 
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
    IIC_Start();
	IIC_Send_Byte((FDC2214_ADDR<<1)|1);//����������ַ+������	
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	res=IIC_Read_Byte(1)<<8;//��ȡ����,����ACK	  
	res|=IIC_Read_Byte(0);//��ȡ����,����nACK
    IIC_Stop();			//����һ��ֹͣ����
	return res;		
}

/*�õ�ͨ��xת���������
 *index:0 -> CH0
 *      1 -> CH1
 *      2 -> CH2
 *      3 -> CH3
*����ֵ:����������
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

/*FDC2214��ʼ������
 *����ֵ:0����ʼ������
 *       1��������
 */
u8 FDC2214_Init(void)
{
	u16 res;
	IIC_Init();
	
	//���ID�Ƿ���ȷ���ٳ�ʼ���Ĵ���
	res=FDC_Read(MANUFACTURER_ID);
	printf("ID=%x\r\n",res);
	if(res==0x5449)
	{
		
		//����Set_FDC2214�Ĵ���
		Set_FDC2214(RCOUNT_CH0,0x34,0xFB);//�ο�����ת�����ʱ�䣨T=(RCOUNT_CH0*16)/Frefx��
		Set_FDC2214(RCOUNT_CH1,0x34,0xFB);		
		Set_FDC2214(RCOUNT_CH2,0x34,0xFB);
		Set_FDC2214(RCOUNT_CH3,0x34,0xFB);	
		Set_FDC2214(SETTLECOUNT_CH0,0x00,0x1B);//ת��֮ǰ���ȶ�ʱ�䣨T=(SETTLECOUNT_CHx*16)/Frefx��
		Set_FDC2214(SETTLECOUNT_CH1,0x00,0x1B);
		Set_FDC2214(SETTLECOUNT_CH2,0x00,0x1B);
		Set_FDC2214(SETTLECOUNT_CH3,0x00,0x1B);
		
		Set_FDC2214(CLOCK_DIVIDERS_C_CH0,0x20,0x02);//ѡ����0.01MHz ~ 10MHz�Ĵ�����Ƶ��
		Set_FDC2214(CLOCK_DIVIDERS_C_CH1,0x20,0x02);//Frefx = Fclk = 43.4MHz/2(2��Ƶ)
		Set_FDC2214(CLOCK_DIVIDERS_C_CH2,0x20,0x02);//CHx_REF_DIVIDER=2;CHx_FIN_SEL=2
		Set_FDC2214(CLOCK_DIVIDERS_C_CH3,0x20,0x02);
		
		Set_FDC2214(DRIVE_CURRENT_CH0,0x78,0x00);//0.146ma��������ʱ�ӽ���+ת��ʱ�������������
		Set_FDC2214(DRIVE_CURRENT_CH1,0x78,0x00);
		Set_FDC2214(DRIVE_CURRENT_CH2,0x78,0x00);
		Set_FDC2214(DRIVE_CURRENT_CH3,0x78,0x00);
		
		Set_FDC2214(ERROR_CONFIG,0x00,0x00);//ȫ����ֹ����㱨
		
		Set_FDC2214(MUX_CONFIG,0xC2,0x0D);//ͨ��0��1��2 ��3��ѡ��10MhzΪ�����񵴲���Ƶ�ʵ�������ã���ͨ������ͨ��
		
		Set_FDC2214(CONFIG,0x14,0x01);
	}
	else return 1;
	
	return 0;
}

/*������·����
 *index:0����·0
 *      1����·1
 *      2����·2
 *      3����·3
 *����ֵ����·�ܵ���C
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



