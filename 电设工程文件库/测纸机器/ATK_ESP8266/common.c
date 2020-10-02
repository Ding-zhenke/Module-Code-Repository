#include "common.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-ESP8266 WIFIģ�� ������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//�û�������

//���Ӷ˿ں�:8086,�������޸�Ϊ�����˿�.
const u8* portnum="8086";		 

//WIFI STAģʽ,����Ҫȥ���ӵ�·�������߲���,��������Լ���·��������,�����޸�.
const u8* wifista_ssid="ALIENTEK";			//·����SSID��
const u8* wifista_encryption="wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const u8* wifista_password="15902020353"; 	//��������

//WIFI APģʽ,ģ���������߲���,�������޸�.
const u8* wifiap_ssid="ATK-ESP8266";			//����SSID��
const u8* wifiap_encryption="wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const u8* wifiap_password="12345678"; 		//�������� 

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//4������ģʽ
const u8 *ATK_ESP8266_CWMODE_TBL[3]={"STAģʽ ","APģʽ ","AP&STAģʽ "};	//ATK-ESP8266,3������ģʽ,Ĭ��Ϊ·����(ROUTER)ģʽ 
//4�ֹ���ģʽ
const u8 *ATK_ESP8266_WORKMODE_TBL[3]={"TCP������","TCP�ͻ���"," UDP ģʽ"};	//ATK-ESP8266,4�ֹ���ģʽ
//5�ּ��ܷ�ʽ
const u8 *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
u8 led0sta=1;
//usmart֧�ֲ���
//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART3_RX_STA;
//     1,����USART3_RX_STA;
void atk_8266_at_response(u8 mode)
{
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		printf("%s",USART3_RX_BUF);	//���͵�����
		if(mode)USART3_RX_STA=0;
	} 
}
//ATK-ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* atk_8266_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//��ATK-ESP8266��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//��ATK-ESP8266����ָ������
//data:���͵�����(����Ҫ��ӻس���)
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)luojian
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s",data);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))break;//�õ���Ч���� 
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}
//ATK-ESP8266�˳�͸��ģʽ
//����ֵ:0,�˳��ɹ�;
//       1,�˳�ʧ��
u8 atk_8266_quit_trans(void)
{
	while((USART3->ISR&0X40)==0);	//�ȴ����Ϳ�
	USART3->TDR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART3->ISR&0X40)==0);	//�ȴ����Ϳ�
	USART3->TDR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART3->ISR&0X40)==0);	//�ȴ����Ϳ�
	USART3->TDR='+';      
	delay_ms(500);					//�ȴ�500ms
	return atk_8266_send_cmd("AT","OK",20);//�˳�͸���ж�.
}
//��ȡATK-ESP8266ģ���AP+STA����״̬
//����ֵ:0��δ����;1,���ӳɹ�
u8 atk_8266_apsta_check(void)
{
	if(atk_8266_quit_trans())return 0;			//�˳�͸�� 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//����AT+CIPSTATUSָ��,��ѯ����״̬
	if(atk_8266_check_cmd("+CIPSTATUS:0")&&
		 atk_8266_check_cmd("+CIPSTATUS:1")&&
		 atk_8266_check_cmd("+CIPSTATUS:2")&&
		 atk_8266_check_cmd("+CIPSTATUS:4"))
		return 0;
	else return 1;
}
//��ȡATK-ESP8266ģ�������״̬
//����ֵ:0,δ����;1,���ӳɹ�.
u8 atk_8266_consta_check(void)
{
	u8 *p;
	u8 res;
	if(atk_8266_quit_trans())return 0;			//�˳�͸�� 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//����AT+CIPSTATUSָ��,��ѯ����״̬
	p=atk_8266_check_cmd("+CIPSTATUS:"); 
	res=*p;									//�õ�����״̬	
	return res;
}
//�������
const u8* kbd_tbl[13]={"1","2","3","4","5","6","7","8","9",".","0","#","DEL"}; 
u8* kbd_fn_tbl[2];
//���ؼ��̽��棨�ߴ�Ϊ240*140��
//x,y:������ʼ���꣨320*240�ֱ��ʵ�ʱ��x����Ϊ0��
void atk_8266_load_keyboard(u16 x,u16 y)
{
	u16 i;
	POINT_COLOR=RED; 
	LCD_Fill(x,y,x+240,y+140,WHITE);
	LCD_DrawRectangle(x,y,x+240,y+140);						   
	LCD_DrawRectangle(x+80,y,x+160,y+140);	 
	LCD_DrawRectangle(x,y+28,x+240,y+56);
	LCD_DrawRectangle(x,y+84,x+240,y+112);
	POINT_COLOR=BLUE;
	for(i=0;i<15;i++)
	{
		if(i<13)Show_Str_Mid(x+(i%3)*80,y+6+28*(i/3),(u8*)kbd_tbl[i],16,80);
		else Show_Str_Mid(x+(i%3)*80,y+6+28*(i/3),kbd_fn_tbl[i-13],16,80); 
	}  		 					   
}
//����״̬����
//x,y:��������
//key:��ֵ��0~8��
//sta:״̬��0���ɿ���1�����£�
void atk_8266_key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{		  
	u16 i=keyx/3,j=keyx%3;
	if(keyx>15)return;
	if(sta)LCD_Fill(x+j*80+1,y+i*28+1,x+j*80+78,y+i*28+26,GREEN);
	else LCD_Fill(x+j*80+1,y+i*28+1,x+j*80+78,y+i*28+26,WHITE); 
	if(j&&(i>3))Show_Str_Mid(x+j*80,y+6+28*i,(u8*)kbd_fn_tbl[keyx-13],16,80);
	else Show_Str_Mid(x+j*80,y+6+28*i,(u8*)kbd_tbl[keyx],16,80);		 		 
}
//�õ�������������
//x,y:��������
//����ֵ��������ֵ��1~15��Ч��0,��Ч��
u8 atk_8266_get_keynum(u16 x,u16 y)
{
	u16 i,j;
	static u8 key_x=0;//0,û���κΰ������£�1~15��1~15�Ű�������
	u8 key=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)			//������������
	{	
		for(i=0;i<5;i++)
		{
			for(j=0;j<3;j++)
			{
			 	if(tp_dev.x[0]<(x+j*80+80)&&tp_dev.x[0]>(x+j*80)&&tp_dev.y[0]<(y+i*28+28)&&tp_dev.y[0]>(y+i*28))
				{	
					key=i*3+j+1;	 
					break;	 		   
				}
			}
			if(key)
			{	   
				if(key_x==key)key=0;
				else 
				{
					atk_8266_key_staset(x,y,key_x-1,0);
					key_x=key;
					atk_8266_key_staset(x,y,key_x-1,1);
				}
				break;
			}
		}  
	}else if(key_x) 
	{
		atk_8266_key_staset(x,y,key_x-1,0);
		key_x=0;
	} 
	return key; 
}
//��ȡClient ip��ַ
//ipbuf:ip��ַ���������
void atk_8266_get_wanip(u8* ipbuf)
{
	u8 *p,*p1;
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//��ȡWAN IP��ַʧ��
		{
			ipbuf[0]=0;
			return;
		}		
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		*p1=0;
		sprintf((char*)ipbuf,"%s",p+1);	
}
/*
//��ȡAP+STA ip��ַ����ָ��λ����ʾ
//ipbuf:ip��ַ���������
void atk_8266_get_ip(u8 x,u8 y)
{
		u8 *p;
		u8 *p1;
		u8 *p2;
		u8 *ipbuf;
		u8 *buf;
		p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
		p1=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
		p2=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	  ipbuf=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
		buf=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//��ȡWAN IP��ַʧ��
		{ 
			*ipbuf=0;
		}
		else
		{		
			p=atk_8266_check_cmd("APIP,\"");
			p1=(u8*)strstr((const char*)(p+6),"\"");
			p2=p1;
			*p1=0;
			ipbuf=p+6;
			sprintf((char*)buf,"AP IP:%s �˿�:%s",ipbuf,(u8*)portnum);
			Show_Str(x,y,200,12,buf,12,0);				//��ʾAPģʽ��IP��ַ�Ͷ˿�
			p=(u8*)strstr((const char*)(p2+1),"STAIP,\"");
			p1=(u8*)strstr((const char*)(p+7),"\"");
			*p1=0;
			ipbuf=p+7;
			sprintf((char*)buf,"STA IP:%s �˿�:%s",ipbuf,(u8*)portnum);
			Show_Str(x,y+15,200,12,buf,12,0);				//��ʾSTAģʽ��IP��ַ�Ͷ˿�
			myfree(SRAMIN,p);		//�ͷ��ڴ�
			myfree(SRAMIN,p1);		//�ͷ��ڴ�
			myfree(SRAMIN,p2);		//�ͷ��ڴ�
			myfree(SRAMIN,ipbuf);		//�ͷ��ڴ�
			myfree(SRAMIN,buf);		//�ͷ��ڴ�
		}
}

//ATK-ESP8266ģ����Ϣ��ʾ
//x,y:��ʾ��Ϣ����ʼ����.
//wanip:0,ȫ��������ʾ;1,������wanip.
void atk_8266_msg_show(u16 x,u16 y,u8 wanip)
{
	u8 *p,*p1,*p2;
	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	p1=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	p2=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	POINT_COLOR=BLUE;
	atk_8266_send_cmd("AT+CWMODE=2","OK",20);
	atk_8266_send_cmd("AT+RST","OK",20);
	u2_print("page page0");
	u2_print("page0.j0.val=50");
	delay_ms(800);//��ʱ1.5s�ȴ�ģ������
	u2_print("page0.t1.txt=\"�ȴ�ģ������\"");
	delay_ms(400);//
	delay_ms(1000);
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);    //����ģ��APģʽ���߲���
	atk_8266_send_cmd(p,"OK",1000);
	
	if(wanip==0)//ȫ����
	{
		atk_8266_send_cmd("AT+GMR","OK",20);		//��ȡ�̼��汾��
		p=atk_8266_check_cmd("SDK version:");
		p1=(u8*)strstr((const char*)(p+12),"(");
		*p1=0;
//		Show_Str(x,y,240,16,"�̼��汾:",16,0);
//		Show_Str(x+72,y,240,16,p+12,16,0);
		printf("�̼��汾��%s\r\n",p+12);
		atk_8266_send_cmd("AT+CWMODE?","+CWMODE:",20);	//��ȡ����ģʽ
		p=atk_8266_check_cmd(":");
//		Show_Str(x,y+16,240,16,"����ģʽ:",16,0);
//		Show_Str(x+72,y+16,240,16,(u8*)ATK_ESP8266_CWMODE_TBL[*(p+1)-'1'],16,0);
		
		atk_8266_send_cmd("AT+CWSAP?","+CWSAP:",20);	//��ȡwifi����
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		p2=p1;
		*p1=0;
//		Show_Str(x,y+32,240,16,"SSID��:",16,0);
//		Show_Str(x+56,y+32,240,16,p+1,16,0);
		p=(u8*)strstr((const char*)(p2+1),"\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		p2=p1;
		*p1=0;		
//		Show_Str(x,y+48,240,16,"����:",16,0);
//		Show_Str(x+40,y+48,240,16,p+1,16,0);
		printf("���룺%s\r\n",p+1);
		p=(u8*)strstr((const char*)(p2+1),",");
		p1=(u8*)strstr((const char*)(p+1),",");
		*p1=0;
//		Show_Str(x,y+64,240,16,"ͨ����:",16,0);Show_Str(x+56,y+64,240,16,p+1,16,0);
//		Show_Str(x,y+80,240,16,"���ܷ�ʽ:",16,0);Show_Str(x+72,y+80,240,16,(u8*)ATK_ESP8266_ECN_TBL[*(p1+1)-'0'],16,0);
	}
	myfree(SRAMIN,p);		//�ͷ��ڴ� 
	myfree(SRAMIN,p1);		//�ͷ��ڴ� 
	myfree(SRAMIN,p2);		//�ͷ��ڴ� 
}
//ATK-ESP8266ģ��WIFI���ò�����ʾ(��WIFI STA/WIFI APģʽ����ʱʹ��)
//x,y:��ʾ��Ϣ����ʼ����.
//rmd:��ʾ��Ϣ
//ssid,encryption,password:���������SSID,���ܷ�ʽ,����
void atk_8266_wificonf_show(u16 x,u16 y,u8* rmd,u8* ssid,u8* encryption,u8* password)
{ 
	POINT_COLOR=RED;
	Show_Str(x,y,240,12,rmd,12,0);//��ʾ��ʾ��Ϣ 
	Show_Str(x,y+20,240,12,"SSID:",12,0);
	Show_Str(x,y+36,240,12,"���ܷ�ʽ:",12,0);
	Show_Str(x,y+52,240,12,"����:",12,0); 
	POINT_COLOR=BLUE;
	Show_Str(x+30,y+20,240,12,ssid,12,0);
	Show_Str(x+54,y+36,240,12,encryption,12,0);
	Show_Str(x+30,y+52,240,12,password,12,0); 	  
}
//����ģʽѡ��
//����ֵ:
//0,TCP������
//1,TCP�ͻ���
//2,UDPģʽ
u8 atk_8266_netpro_sel(u16 x,u16 y,u8* name) 
{
	u8 t=0;
	u8 netpro=0;
	char *p1,*p;
	//LCD_Clear(WHITE);
	//POINT_COLOR=RED;
	atk_8266_mtest_ui(32,45);
	p=mymalloc(SRAMIN,50);//����50���ֽڵ��ڴ�
	p1=mymalloc(SRAMIN,50);//����50���ֽڵ��ڴ�
	sprintf((char*)p,"%s ����ģʽѡ��",name);
//	Show_Str_Mid(0,y,p,16,240); 				    	 
//	Show_Str(x,y+30,200,16,"KEY0:��һ��",16,0); 				    	 
//	Show_Str(x,y+50,200,16,"KEY1:��һ��",16,0);				    	 
//	Show_Str(x,y+70,200,16,"WK_UP:ȷ��",16,0); 
//	Show_Str(x,y+100,200,16,"��ѡ��:",16,0); 
//	POINT_COLOR=BLUE;
// 	Show_Str(x+16,y+120,200,16,"TCP������",16,0); 				    	 
//	Show_Str(x+16,y+140,200,16,"TCP�ͻ���",16,0);				    	 
//	Show_Str(x+16,y+160,200,16,"UDPģʽ",16,0); 
//	POINT_COLOR=RED;
//	Show_Str(x,y+120,200,16,"��",16,0); 
	while(1)
	{
//		key=KEY_Scan(0); 
//		if(key)
//		{
//			if(key==WKUP_PRES)break;       //WK_UP����
//			Show_Str(x,y+120+netpro*20,200,16,"  ",16,0);//���֮ǰ����ʾ
//			if(key==KEY0_PRES)//KEY0����
//			{
//				if(netpro<2)netpro++;
//				else netpro=0;
//			}else if(key==KEY1_PRES)//KEY1����
//			{
//				if(netpro>0)netpro--;
//				else netpro=2; 
//			}
//			Show_Str(x,y+120+netpro*20,200,16,"��",16,0);//ָ������Ŀ
//				
//		}
		sprintf((char*)p1,"%s",USART2_RX_BUF);	
		p = strstr(p1,"TCP_SERVER");
		if(p!=NULL)
		{
			netpro=0;
			break;
		}
		p = strstr(p1,"TCP_CLIENT");
		if(p!=NULL)
		{
			netpro=1;
			break;
		}
		p = strstr(p1,"UDP");
		if(p!=NULL)
		{
			netpro=2;
			break;
		}
		delay_ms(10);
		atk_8266_at_response(1);
		if((t++)==20){t=0;LED0_Toggle;}//LED��˸
	} 
	myfree(SRAMIN,p);
	myfree(SRAMIN,p1);
	return netpro;//��������ģʽѡ��ֵ 
} 

//STAģʽ�µ�AP��TCP��UDP����ģʽ����
u8 atk_8266_mode_cofig(u8 netpro)
{
	//u8 netpro;
	u8 ipbuf[16]; 	//IP����
	u8 *p;
	u8 key;
	p=mymalloc(SRAMIN,32);//����32���ֽڵ��ڴ�
PRESTA:		
	netpro|=(atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[1]))<<4;	//����ģʽѡ��
	if(netpro&0X20)
	{
		LCD_Clear(WHITE);
		if(atk_8266_ip_set("WIFI-APԶ��UDP IP����","UDPģʽ",(u8*)portnum,ipbuf))goto PRESTA;	//IP����
		if(netpro&0X03)sprintf((char*)p,"AT+CIPSTART=1,\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��UDP������,��ID�ţ�STAģʽ��Ϊ0
		else sprintf((char*)p,"AT+CIPSTART=0,\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��UDP������,��ID�ţ�STAģʽ��Ϊ0
		delay_ms(200);
		LCD_Clear(WHITE);
		atk_8266_send_cmd(p,"OK",200);
	}
	else if(netpro&0X10)     //AP TCP Client    ͸��ģʽ����
	{
		LCD_Clear(WHITE);
		POINT_COLOR=RED;
		Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ����",16,240); 
		Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
		if(atk_8266_ip_set("WIFI-AP Զ��IP����","TCP Client",(u8*)portnum,ipbuf))goto PRESTA;	//IP����
		if(netpro&0X03)sprintf((char*)p,"AT+CIPSTART=1,\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������,��ID�ţ�STAģʽΪclientʱ��Ϊ1
		else sprintf((char*)p,"AT+CIPSTART=0,\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������,��ID�ţ�STAģʽΪserverʱ��Ϊ0
		while(atk_8266_send_cmd(p,"OK",200))
			{
				LCD_Clear(WHITE);
				POINT_COLOR=RED;
				Show_Str_Mid(0,40,"WK_UP:������ѡ",16,240);
				Show_Str(30,80,200,12,"ATK-ESP ����TCP SERVERʧ��",12,0); //����ʧ��	 
				key=KEY_Scan(0);
				if(key==WKUP_PRES)goto PRESTA;
			}				
	}
	else;   //������ģʽ��������
	myfree(SRAMIN,p);
	return netpro;
}
*/








//�յ�����֮�󿽱�һ�ݹ���
void accept(u8* p)
{
	u8 rlen;
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		rlen=USART3_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
		USART3_RX_BUF[rlen]=0;		//��ӽ����� 
		printf("%s",USART3_RX_BUF);	//���͵�����   
		sprintf((char*)p,"�յ�%d�ֽ�,��������",rlen);//���յ����ֽ���,ͬʱ�����ݿ���P���� 
		USART3_RX_STA=0;
	} 

}
//ATK-ESP8266ģ�����������
void atk_8266_test(void)
{
	u8 *p,*p1;
	u8 flag;
	p=mymalloc(SRAMIN,50);		//���붯̬�ڴ�ռ�50�ֽ�
	p1=mymalloc(SRAMIN,50);		//���붯̬�ڴ�ռ�50�ֽ�
	printf("���WIFIģ���Ƿ�����");
	while(atk_8266_send_cmd("AT","OK",20))//���WIFIģ���Ƿ�����
	{
		atk_8266_quit_trans();//�˳�͸��
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	

		delay_ms(800);
	} 
	//WiFi����
	while(atk_8266_send_cmd("ATE0","OK",20));//�رջ���
	printf("�رջ���");
	//ʹ��WiFi APģʽ
	while(atk_8266_send_cmd("AT+CWMODE=2","OK",20));
	atk_8266_send_cmd("AT+RST","OK",20);//������Ч
	printf("������Ч\r\n");
	delay_ms(2000);//�ȴ�����
	printf("���ģʽ\r\n");
	if(atk_8266_send_cmd("AT+CWMODE=2","2",20)==1)//ģʽ���ԣ���ӡģʽ���
	{
		sprintf((char *)p,"%s",USART3_RX_BUF);
		printf("%s\r\n",p);
		printf("ģʽ����");
	}
	printf("ģʽ��ȷ\r\n");
	printf("����ģ��APģʽ���߲���\r\n");
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);    //����ģ��APģʽ���߲���
	atk_8266_send_cmd(p,"OK",1000);
	atk_8266_send_cmd("AT+CWMODE=2","OK",20);
	//ģʽѡ���Ȳ���TCP��������
	printf("ѡ�еĹ���ģʽ��%s\r\n",ATK_ESP8266_WORKMODE_TBL[0]);
	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
	sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);
	atk_8266_send_cmd(p,"OK",20);     //����Serverģʽ���˿ں�Ϊ8086
	sprintf((char*)p,"ATK-8266%s����\r\n",ATK_ESP8266_WORKMODE_TBL[0]);//��������
	//Show_Str(30+54,100,200,12,p,12,0);
	atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //����ָ�����ȵ�����
	delay_ms(200);
	flag=atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
	if(flag==0)
		printf("���Գɹ�\r\n");
	while(atk_8266_consta_check()!='+')
	{
		printf("�ȴ�����\r\n");
	}
	printf("���ӳɹ�\r\n");
	while(1)
	{
		p1="���ݴ��Ͳ���";
		sprintf((char *)p,"AT+CIPSEND=0,%d",strlen(p1));
		printf("׼������");
		atk_8266_send_cmd(p,"OK",200);
		atk_8266_send_data(p1,"OK",1000);
		printf("�������");
		//accept(p1);
		delay_ms(300);
		printf("�Ƿ������");
		accept(&flag);
	}
}








