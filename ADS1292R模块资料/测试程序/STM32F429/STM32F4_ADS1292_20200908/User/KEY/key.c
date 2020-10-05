//-----------------------------------------------------------------
// ��������:
// 		 ����������������
// ��    ��: ���ǵ���
// ��ʼ����: 2018-08-04
// �������: 2018-08-04
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2018-08-04)�������̵�IO���ã���ֵ����
// ���Թ���: ����STM32F429+Cyclone IV����ϵͳ��ƿ����塢LZE_ST_LINK2
// ˵    ��: 
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "key.h"
#include "delay.h"
//-----------------------------------------------------------------

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {BUTTON_KEY1_GPIO_PORT,BUTTON_KEY2_GPIO_PORT,BUTTON_KEY3_GPIO_PORT,BUTTON_KEY4_GPIO_PORT};
const uint16_t BUTTON_PIN[BUTTONn] = {BUTTON_KEY1_PIN,BUTTON_KEY2_PIN,BUTTON_KEY3_PIN,BUTTON_KEY4_PIN};

//-----------------------------------------------------------------
// void KEY_Init(void)
//-----------------------------------------------------------------
//
// ��������: ��ʼ���������̶˿�
// ��ڲ���: ��
// ���ز���: ��
// ע������: ��
//
//-----------------------------------------------------------------
void KEY_Init(void)
{
	BSP_KEY_Init(BUTTON_KEY1);	// KEY1����PD6
  BSP_KEY_Init(BUTTON_KEY2);	// KEY2����PB4
	BSP_KEY_Init(BUTTON_KEY3);	// KEY3����PB6
	BSP_KEY_Init(BUTTON_KEY4);	// KEY4����PB9
}

//-----------------------------------------------------------------
// void BSP_KEY_Init(Button_TypeDef button)
//-----------------------------------------------------------------
//
// ��������: ����ָ���İ���
// ��ڲ���: button������ָ���İ�����ֵ��BUTTON_KEY1��BUTTON_KEY2��BUTTON_KEY3��BUTTON_KEY4��
// ���ز���: ��
// ע������: ��
//
//-----------------------------------------------------------------
void BSP_KEY_Init(Button_TypeDef button)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  BUTTONx_GPIO_CLK_ENABLE(button);											// ����KEYʱ��

  GPIO_InitStruct.Pin = BUTTON_PIN[button];							// ����KEY
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;								// ��������
  GPIO_InitStruct.Pull = GPIO_PULLUP;										// ����
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;							// ����
  HAL_GPIO_Init(BUTTON_PORT[button], &GPIO_InitStruct);	// ��ʼ��KEY
}

//-----------------------------------------------------------------
// u8 KEY_Scan(u8 mode)
//-----------------------------------------------------------------
//
// ��������: ��������ɨ��
// ��ڲ���: u8 mode��0��֧����������1֧��������;
// ���ز���: ����ֵ
// ע������: �˺�������Ӧ���ȼ�,KEY1>KEY2>KEY3>KEY4!!
//
//-----------------------------------------------------------------
u8 KEY_Scan(u8 mode)
{
	static u8 key_up=1;     // �����ɿ���־
	if(mode==1)							// ֧������
		key_up=1;    					
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))	// �а�������
	{
		delay_ms(10);					// ��ʱ10ms
		key_up=0;							// �����ɿ���־����
		if(KEY1==0)       		// ����K1����
			return KEY1_PRES;	
		else if(KEY2==0)  		// ����K2����
			return KEY2_PRES;	
		else if(KEY3==0)  		// ����K3����
			return KEY3_PRES;
		else if(KEY4==0) 			// ����K4����
			return KEY4_PRES;          
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==01)	// �ް�������
		key_up=1;
	return 0;   // �ް�������
}
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
