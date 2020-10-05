#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 

//KEY��������	   


//STM32H7����ģ��-HAL�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com								  
//////////////////////////////////////////////////////////////////////////////////


/************************************************************************************/

//������ʼ������


//STM32H7����ģ��-HAL�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	


/************************************************************************************/

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

	  
    __HAL_RCC_GPIOC_CLK_ENABLE();           //����GPIOCʱ��
    __HAL_RCC_GPIOE_CLK_ENABLE();           //����GPIOEʱ��

    
	  GPIO_Initure.Pin=GPIO_PIN_3;            //PE3  :��Ӧ K1
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
		GPIO_Initure.Pin=GPIO_PIN_5;            //PC5  :��Ӧ K2
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
    

}


/************************************************************************************/

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY1>KEY2


//STM32H7����ģ��-HAL�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	


/************************************************************************************/

u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;     //�����ɿ���־
    if(mode==1)key_up=1;    //֧������
    if(key_up&&(KEY1==0||KEY2==0))
    {
        delay_ms(10);
        key_up=0;
        if(KEY1==0)  return KEY1_PRES;
        else if(KEY2==0)  return KEY2_PRES;
        
    }else if(KEY1==1&&KEY2==1)key_up=1;
    return 0;   //�ް�������
}









//STM32H7����ģ��-HAL�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	












