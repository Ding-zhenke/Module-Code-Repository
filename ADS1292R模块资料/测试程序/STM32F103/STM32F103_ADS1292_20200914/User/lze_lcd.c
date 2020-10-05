//-----------------------------------------------------------------
// ��������:
//      ����FSMC����SPFD5408/ST7789V�ں˵�2.8��Һ���������ӳ���
// ��    ��: ���ǵ���
// ��ʼ����: 2019-05-09
// �������: 2019-05-09
// �޸�����:
// ��ǰ�汾: V2.0
// ��ʷ�汾:
//  - V1.0: 2.8��TFTҺ����ʾ (SPFD5408)
//  - V2.0: 2.8��TFTҺ����ʾ (SPFD5408 / ST7789V)
// ���Թ���: ����STM32+FPGA����ϵͳ��ƿ����塢2.8��Һ������LZE_ST_LINK2
// ˵    ��:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "lze_lcd.h"
#include "fonts.h"
#include "Delay.h"

// �ַ���ģ
#include "./Fonts/font8.c"
#include "./Fonts/font12.c"
#include "./Fonts/font16.c"
#include "./Fonts/font20.c"
#include "./Fonts/font24.c"
// ������ģ
#include "./Fonts/GB1616.c"
#include "./Fonts/GB2424.c"
#include "./Fonts/GB3232.c"

// �궨��
#define POLY_Y(Z)               ((int32_t)((Points + Z)->X))
#define POLY_X(Z)               ((int32_t)((Points + Z)->Y))
#define ABS(X)                  ((X) > 0 ? (X) : -(X))                      // ȡ����ֵ
#define LCD_Delay(t_ms)         Delay_1ms(t_ms)                             // LCD ��ʼ����ʱ���� �ض���
//#define LCD_Delay(t_ms)         HAL_Delay(t_ms)                             // LCD ��ʼ����ʱ���� �ض���


//// ȫ�ֱ���
sFONT                         *LCD_Currentfonts;                          // ��ʾ�ַ�������Ϣ
sFONT_CN                      *LCD_Currentfonts_CN;                       // ��ʾ����������Ϣ

__IO LCD__DevTypeDef           LCD_Dev;                                    //���� LCD ��Ҫ����
__IO uint16_t                  TextColor = 0x0000,
                               BackColor = 0xFFFF;                         // ������ɫ �� ������ɫ




//-----------------------------------------------------------------
// ��ʼ��������
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void LCD_CtrlLinesConfig(void)
//-----------------------------------------------------------------
//
// ��������: LCD MCU�ӿ���������
// ��ڲ���: ��
// ���ز���: ��
// ȫ�ֱ���: ��
// ע������: ��
//
//-----------------------------------------------------------------
void LCD_CtrlLinesConfig(void)
{
  /** FSMC GPIO Configuration
  PD14   -----> FSMC_D0
  PD15   -----> FSMC_D1
  PD0   ------> FSMC_D2
  PD1   ------> FSMC_D3
  PE7   ------> FSMC_D4
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE10   -----> FSMC_D7
  PE11   -----> FSMC_D8
  PE12   -----> FSMC_D9
  PE13   -----> FSMC_D10
  PE14   -----> FSMC_D11
  PE15   -----> FSMC_D12
  PD8   ------> FSMC_D13
  PD9   ------> FSMC_D14
  PD10   -----> FSMC_D15
  PD11   -----> FSMC_A16
  PD4   ------> FSMC_NOE
  PD5   ------> FSMC_NWE
  PD7   ------> FSMC_NE1
  */
  GPIO_InitTypeDef GPIO_InitStructure;

  // ʹ��IO��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD |
                             RCC_APB2Periph_GPIOE,
                         ENABLE);
  // ���������ߡ����ֿ���������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |   // D2,D3
                                GPIO_Pin_4 | GPIO_Pin_5 |   // RD,WR
                                GPIO_Pin_7 | GPIO_Pin_8 |   // CS,D13
                                GPIO_Pin_9 | GPIO_Pin_10 |  // D14,D15
                                GPIO_Pin_11 | GPIO_Pin_14 | // RS,D0
                                GPIO_Pin_15;                // D1
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  // ���츴�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  // ��������������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 |   // D4,D5
                                GPIO_Pin_9 | GPIO_Pin_10 |  // D6,D7
                                GPIO_Pin_11 | GPIO_Pin_12 | // D8,D9
                                GPIO_Pin_13 | GPIO_Pin_14 | // D10,D11
                                GPIO_Pin_15;                // D12
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���츴�����
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  // Һ����λ���������������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; // RST,LED
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  LCD_LED_OFF; // LED��
}

//-----------------------------------------------------------------
// void LCD_FSMCConfig(void)
//-----------------------------------------------------------------
//
// ��������: LCD FMC���������� LCD(Parallel mode)
// ��ڲ���: ��
// ���ز���: ��
// ȫ�ֱ���: ��
// ע������: ��
//
//-----------------------------------------------------------------
void LCD_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef p;
  // ��FSMCʱ��
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
  // �򿪸��ù���ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  // FSMC_Bank1_NORSRAM1 ʱ������
  // ��ַ����ʱ������
  p.FSMC_AddressSetupTime                              = 0x02;
  // ��ַ�ĳ���ʱ��
  p.FSMC_AddressHoldTime                               = 0x00;
  // �趨����ʱ������
  p.FSMC_DataSetupTime                                 = 0x05;
  // ����ת��ʱ��
  p.FSMC_BusTurnAroundDuration                         = 0x00;
  // CLKʱ������źŵ�HCLK��������ʾʱ��
  p.FSMC_CLKDivision                                   = 0x00;
  // ָ���ڻ�õ�һ������ǰ��ʱ������
  p.FSMC_DataLatency                                   = 0x00;
  p.FSMC_AccessMode                                    = FSMC_AccessMode_B;
  // ָ����FSMC��
  FSMC_NORSRAMInitStructure.FSMC_Bank                  = FSMC_Bank1_NORSRAM1;
  // ��ַ������ֵ�����õ���������
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;
  // �ⲿ�洢��������
  FSMC_NORSRAMInitStructure.FSMC_MemoryType            = FSMC_MemoryType_NOR;
  // ���ݿ��
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;
  // ����ͻ������ģʽ
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;
  // ָ���ȴ��źŵļ���
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;
  // disables the Wrapped burst access mode for Flash
  FSMC_NORSRAMInitStructure.FSMC_WrapMode              = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;
  // ����ָ����FSMC���д����
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;
  // ��չģʽ
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;
  // ����дͻ������
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = &p;
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
  // ʹ�� FSMC Bank1_SRAM
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

//-----------------------------------------------------------------
// void LCD_WR_REG ( uint8_t RegIndex )
//-----------------------------------------------------------------
//
// ��������: LCD д�Ĵ���
// ��ڲ���: RegIndex  �Ĵ�������
// ���ز���: ��
// ȫ�ֱ���: ��
// ע������: ��
//
//-----------------------------------------------------------------
void LCD_WR_REG ( uint8_t RegIndex )
{
  LCD->LCD_REG = RegIndex;
}

//-----------------------------------------------------------------
//void LCD_WR_DATA(uint16_t RegValue)
//-----------------------------------------------------------------
//
// ��������: LCD д����ֵ
// ��ڲ���: RegValue  ���ݼĴ���ֵ
// ���ز���: ��
// ȫ�ֱ���: ��
// ע������: ��
//
//-----------------------------------------------------------------
void LCD_WR_DATA(uint16_t RegValue)
{
  LCD->LCD_RAM = RegValue;
}


//-----------------------------------------------------------------
// void LCD_WriteRAM(uint16_t RGB_Code)
//-----------------------------------------------------------------
//
//  ��������: LCD д���ִ�����
//  ��ڲ���: �����������
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ��
//
//-----------------------------------------------------------------
void LCD_WriteRAM(uint16_t RGB_Code)
{
  /* Write 16-bit GRAM Reg */
  LCD->LCD_RAM = RGB_Code;
}

//-----------------------------------------------------------------
// uint16_t LCD_ReadRAM(void)
//-----------------------------------------------------------------
//
//  ��������: LCD ��ȡ�Դ�����
//  ��ڲ���: ��
//  ���ز���: �Դ�����
//  ȫ�ֱ���: ��
//  ע������: ��
//
//-----------------------------------------------------------------
uint16_t LCD_ReadRAM(void)
{
  /* Read 16-bit Reg */
  return (LCD->LCD_RAM);
}

//-----------------------------------------------------------------
// void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
//-----------------------------------------------------------------
//
//  ��������: LCD ��Ӧ�Ĵ���д�˼Ĵ���ֵ
//  ��ڲ���: LCD_Reg �Ĵ�������; LCD_RegValue д�����
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ��
//
//-----------------------------------------------------------------
void LCD_WriteReg(
  uint8_t   LCD_Reg,
  uint16_t  LCD_RegValue
)
{
  /* Write 16-bit Index, then Write Reg */
  LCD->LCD_REG = LCD_Reg;
  /* Write 16-bit Reg */
  LCD->LCD_RAM = LCD_RegValue;
}

//-----------------------------------------------------------------
// uint16_t LCD_ReadReg(uint8_t LCD_Reg)
//-----------------------------------------------------------------
//
//  ��������: LCD ��Ӧ�Ĵ�����ȡ�Ĵ���ֵ
//  ��ڲ���: LCD_Reg          �Ĵ�������
//  ���ز���: ��Ӧ�Ĵ���ֵ
//  ȫ�ֱ���: ��
//  ע������: ��
//
//-----------------------------------------------------------------
uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{
  /* Write 16-bit Index (then Read Reg) */
  LCD->LCD_REG = LCD_Reg;
  /* Read 16-bit Reg */
  return (LCD->LCD_RAM);
}

//-----------------------------------------------------------------
// void LCD_WriteRAM_Prepare(void)
//-----------------------------------------------------------------
//
//  ��������: LCD ׼��д������  (��ָ��LCD RAM �Ĵ���)
//  ��ڲ���: ��
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ��
//
//-----------------------------------------------------------------
void LCD_WriteRAM_Prepare(void)
{
  LCD->LCD_REG = LCD_Dev.REG_GRAM;
}

//-----------------------------------------------------------------
// void LCD_Display_Dir ( uint8_t Dir )
//-----------------------------------------------------------------
//
//  ��������: LCD ������ʾ����
//  ��ڲ���: ��ʾ����
//            DISPLAY_DIR_L         // ��
//            DISPLAY_DIR_R         // ��
//            DISPLAY_DIR_U         // ��
//            DISPLAY_DIR_D         // ��
//  ���ز���: ��
//  ȫ�ֱ���: LCD_Dev ��ʾ��Ϣ�ṹ��
//              ����:��ʾ���ڴ�С
//                   ��ʾ�Ĵ�������
//                   ��������ֵ����
//                   ��Ļɨ�跽��
//  ע������: ��ͬ����оƬ���Ĵ���������һ��
//
//-----------------------------------------------------------------
void LCD_Display_Dir ( uint8_t Dir )
{
  LCD_Dev.DISPLAY_DIR = Dir;
  if ( Dir == DISPLAY_DIR_L )
  {
    LCD_Dev.WIDTH   = 320;                                        // ��ʾ���ڿ��
    LCD_Dev.HEIGHT  = 240;                                        // ��ʾ���ڸ߶�
    if ( LCD_Dev.REG_ID == ST7789V_ID )
    {
      LCD_Dev.REG_GRAM        = 0X2C;                             // ��ʾ�Ĵ�������
      LCD_Dev.REG_XPOSITION   = 0x2A;                             // X������Ĵ�������
      LCD_Dev.REG_YPOSITION   = 0x2B;                             // Y������Ĵ�������
      LCD_Dev.SCAN_DIR        = ((0<<7)|(1<<6)|(1<<5));           // MY, MX, MV
    }
    else if ( LCD_Dev.REG_ID == SPFD5408_ID )
    {
      LCD_Dev.REG_GRAM        = 0X22;
      LCD_Dev.REG_XPOSITION   = 0x20;
      LCD_Dev.REG_YPOSITION   = 0x21;
      LCD_Dev.SCAN_DIR        = 0x1000 | ((0<<5)|(0<<4)|(1<<3));   // MY, MX, MV
    }
  }
  else if ( Dir == DISPLAY_DIR_R )
  {
    LCD_Dev.WIDTH             = 320;
    LCD_Dev.HEIGHT            = 240;
    if ( LCD_Dev.REG_ID == ST7789V_ID )
    {
      LCD_Dev.REG_GRAM        = 0X2C;
      LCD_Dev.REG_XPOSITION   = 0x2A;
      LCD_Dev.REG_YPOSITION   = 0x2B;
      LCD_Dev.SCAN_DIR        = ((1<<7)|(0<<6)|(1<<5));             // MY, MX, MV
    }
    else if ( LCD_Dev.REG_ID == SPFD5408_ID )
    {
      LCD_Dev.REG_GRAM        = 0X22;
      LCD_Dev.REG_XPOSITION   = 0x20;
      LCD_Dev.REG_YPOSITION   = 0x21;
      LCD_Dev.SCAN_DIR        = 0x1000 | ((1<<5)|(1<<4)|(1<<3));    // MY, MX, MV
    }
  }
  else if ( Dir == DISPLAY_DIR_U )
  {
    LCD_Dev.WIDTH             = 240;
    LCD_Dev.HEIGHT            = 320;
    if ( LCD_Dev.REG_ID == ST7789V_ID )
    {
      LCD_Dev.REG_GRAM        = 0X2C;
      LCD_Dev.REG_XPOSITION   = 0x2A;
      LCD_Dev.REG_YPOSITION   = 0x2B;
      LCD_Dev.SCAN_DIR        = ((1<<7)|(1<<6)|(0<<5));             // MY, MX, MV
    }
    else if ( LCD_Dev.REG_ID == SPFD5408_ID )
    {
      LCD_Dev.REG_GRAM        = 0X22;
      LCD_Dev.REG_XPOSITION   = 0x20;
      LCD_Dev.REG_YPOSITION   = 0x21;
      LCD_Dev.SCAN_DIR        = 0x1000 | ((1<<5)|(0<<4)|(0<<3));    // MY, MX, MV 
    }
  }
  else                                                              //DISPLAY_DIR_D
  {
    LCD_Dev.WIDTH             = 240;
    LCD_Dev.HEIGHT            = 320;
    if ( LCD_Dev.REG_ID == ST7789V_ID )
    {
      LCD_Dev.REG_GRAM        = 0X2C;
      LCD_Dev.REG_XPOSITION   = 0x2A;
      LCD_Dev.REG_YPOSITION   = 0x2B;
      LCD_Dev.SCAN_DIR        = ((0<<7)|(0<<6)|(0<<5));             // MY, MX, MV
    }
    else if ( LCD_Dev.REG_ID == SPFD5408_ID )
    {
      LCD_Dev.REG_GRAM        = 0X22;
      LCD_Dev.REG_XPOSITION   = 0x20;
      LCD_Dev.REG_YPOSITION   = 0x21;
      LCD_Dev.SCAN_DIR        = 0x1000 | ((0<<5)|(1<<4)|(0<<3));    // MY, MX, MV
    }
  }

}

//-----------------------------------------------------------------
// void LCD_Init(void)
//-----------------------------------------------------------------
//
//  ��������: LCD ��ʼ��
//  ��ڲ���: ��
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ��
//
//-----------------------------------------------------------------
void LCD_Init(void)
{
// �����CubeMx ����  LCD_CtrlLinesConfig() �� LCD_FSMCConfig() ���Բ������
  LCD_CtrlLinesConfig();                                            // LCD �˿�����
  LCD_FSMCConfig();                                                 // FSMC ����������

  LCD_Delay ( 10 );                                                 // delay 10 ms
  LCD_RST_Clr;                                                      // �½��ظ�λ
  LCD_Delay ( 10 );                                                 // delay 10 ms
  LCD_RST_Set;         // �˳���λ
  LCD_Delay ( 10 );                                                 // delay 10 ms

  /* Read the LCD ID */
  //���� ST7789V  ID�Ķ�ȡ
  LCD_ReadReg ( 0x04 );
  LCD_Dev.REG_ID =   LCD_ReadRAM();                                 // ��Ч����
  LCD_Dev.REG_ID =   LCD_ReadRAM();                                 // ����0X85
  LCD_Dev.REG_ID <<= 8;
  LCD_Dev.REG_ID |=  LCD_ReadRAM();                                 // ��ȡ52

  if(LCD_Dev.REG_ID != ST7789V_ID)                                  // �����ͺŷ� ST7789V
  {
    LCD_Dev.REG_ID = LCD_ReadReg ( 0x00 );                          // ���Ի�ȡ 5408 �� ID
  }

  if ( LCD_Dev.REG_ID == ST7789V_ID )                               // ST7789V
  {
    //-------------------------------------------------------------------------------------//
    LCD_WR_REG ( 0x11 );
    LCD_Delay  ( 120  );                                            // Delay 1200 ms
    //------------------------------display and color format setting-----------------------//
    LCD_WR_REG ( 0x36 );  LCD_WR_DATA ( 0x00 );
    LCD_WR_REG ( 0x3a );  LCD_WR_DATA ( 0x05 );
    //--------------------------------ST7789V Frame rate setting---------------------------//
    LCD_WR_REG ( 0xb2 );  LCD_WR_DATA ( 0x0c );  LCD_WR_DATA ( 0x0c );
                          LCD_WR_DATA ( 0x00 );  LCD_WR_DATA ( 0x33 );
                          LCD_WR_DATA ( 0x33 );
    LCD_WR_REG ( 0xb7 );  LCD_WR_DATA ( 0x35 );
    //---------------------------------ST7789V Power setting-------------------------------//
    LCD_WR_REG ( 0xbb );  LCD_WR_DATA ( 0x28 );
    LCD_WR_REG ( 0xc0 );  LCD_WR_DATA ( 0x2c );
    LCD_WR_REG ( 0xc2 );  LCD_WR_DATA ( 0x01 );
    LCD_WR_REG ( 0xc3 );  LCD_WR_DATA ( 0x0b );
    LCD_WR_REG ( 0xc4 );  LCD_WR_DATA ( 0x20 );
    LCD_WR_REG ( 0xc6 );  LCD_WR_DATA ( 0x0f );                     // Frame Rate Control in Normal Mode
    LCD_WR_REG ( 0xd0 );  LCD_WR_DATA ( 0xa4 );  LCD_WR_DATA ( 0xa1 );
    //--------------------------------ST7789V gamma setting--------------------------------//
    LCD_WR_REG ( 0xe0 );  LCD_WR_DATA ( 0xd0 );  LCD_WR_DATA ( 0x01 );
                          LCD_WR_DATA ( 0x08 );  LCD_WR_DATA ( 0x0f );
                          LCD_WR_DATA ( 0x11 );  LCD_WR_DATA ( 0x2a );
                          LCD_WR_DATA ( 0x36 );  LCD_WR_DATA ( 0x55 );
                          LCD_WR_DATA ( 0x44 );  LCD_WR_DATA ( 0x3a );
                          LCD_WR_DATA ( 0x0b );  LCD_WR_DATA ( 0x06 );
                          LCD_WR_DATA ( 0x11 );  LCD_WR_DATA ( 0x20 );
                          
    LCD_WR_REG ( 0xe1 );  LCD_WR_DATA ( 0xd0 );  LCD_WR_DATA ( 0x02 );
                          LCD_WR_DATA ( 0x07 );  LCD_WR_DATA ( 0x0a );
                          LCD_WR_DATA ( 0x0b );  LCD_WR_DATA ( 0x18 );
                          LCD_WR_DATA ( 0x34 );  LCD_WR_DATA ( 0x43 );
                          LCD_WR_DATA ( 0x4a );  LCD_WR_DATA ( 0x2b );
                          LCD_WR_DATA ( 0x1b );  LCD_WR_DATA ( 0x1c );
                          LCD_WR_DATA ( 0x22 );  LCD_WR_DATA ( 0x1f );
                          
    LCD_WR_REG ( 0x29 );
    LCD_WR_REG ( 0x2c );

    LCD_Display_Dir ( LCD_DIR );                                    // ������ʾ����

    LCD_WR_REG ( 0X36 );  LCD_WR_DATA ( LCD_Dev.SCAN_DIR );         // ����ɨ�跽��

    LCD_SetDisplayWindow (0, 0, LCD_Dev.HEIGHT, LCD_Dev.WIDTH);     // ������ʾ����
  }
  else if ( LCD_Dev.REG_ID == SPFD5408_ID )                         // SPFD5408
  {
    /* Start Initial Sequence ------------------------------------------------*/
    LCD_WriteReg ( LCD_REG_1,   0x0100 );  /* Set SS bit */
    LCD_WriteReg ( LCD_REG_2,   0x0700 );  /* Set 1 line inversion */
    LCD_WriteReg ( LCD_REG_3,   0x1030 );  /* Set GRAM write direction and BGR=1. */
    LCD_WriteReg ( LCD_REG_4,   0x0000 );  /* Resize register */

    LCD_WriteReg ( LCD_REG_8,   0x0202 );  /* Set the back porch and front porch */
    LCD_WriteReg ( LCD_REG_9,   0x0000 );  /* Set non-display area refresh cycle ISC[3:0] */
    LCD_WriteReg ( LCD_REG_10,  0x0000 );  /* FMARK function */
    LCD_WriteReg ( LCD_REG_12,  0x0000 );  /* RGB 18-bit System interface setting */
    LCD_WriteReg ( LCD_REG_13,  0x0000 );  /* Frame marker Position */
    LCD_WriteReg ( LCD_REG_15,  0x0000 );  /* RGB interface polarity, no impact */

    /* Power On sequence -----------------------------------------------------*/
    LCD_WriteReg ( LCD_REG_16,  0x0000 );  /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    LCD_WriteReg ( LCD_REG_17,  0x0000 );  /* DC1[2:0], DC0[2:0], VC[2:0] */
    LCD_WriteReg ( LCD_REG_18,  0x0000 );  /* VREG1OUT voltage */
    LCD_WriteReg ( LCD_REG_19,  0x0000 );  /* VDV[4:0] for VCOM amplitude */
    LCD_Delay ( 200 );                     /* Dis-charge capacitor power voltage (200ms) */

    LCD_WriteReg ( LCD_REG_17,  0x0007 );  /* DC1[2:0], DC0[2:0], VC[2:0] */
    LCD_Delay ( 50 );                      /* Delay 50 ms */
    LCD_WriteReg ( LCD_REG_16,  0x12B0 );  /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    LCD_Delay ( 50 );                      /* Delay 50 ms */
    LCD_WriteReg ( LCD_REG_18,  0x01BD );  /* External reference voltage= Vci */
    LCD_Delay ( 50 );
    LCD_WriteReg ( LCD_REG_19,  0x1400 );  /* VDV[4:0] for VCOM amplitude */
    LCD_WriteReg ( LCD_REG_41,  0x000E );  /* VCM[4:0] for VCOMH */
    LCD_Delay ( 50 );                      /* Delay 50 ms */
    LCD_WriteReg ( LCD_REG_32,  0x0000 );  /* GRAM horizontal Address */
    LCD_WriteReg ( LCD_REG_33,  0x013F );  /* GRAM Vertical Address */

    /* Adjust the Gamma Curve (SPFD5408B)-------------------------------------*/
    LCD_WriteReg ( LCD_REG_48,  0x0b0d );
    LCD_WriteReg ( LCD_REG_49,  0x1923 );
    LCD_WriteReg ( LCD_REG_50,  0x1c26 );
    LCD_WriteReg ( LCD_REG_51,  0x261c );
    LCD_WriteReg ( LCD_REG_52,  0x2419 );
    LCD_WriteReg ( LCD_REG_53,  0x0d0b );
    LCD_WriteReg ( LCD_REG_54,  0x1006 );
    LCD_WriteReg ( LCD_REG_55,  0x0610 );
    LCD_WriteReg ( LCD_REG_56,  0x0706 );
    LCD_WriteReg ( LCD_REG_57,  0x0304 );
    LCD_WriteReg ( LCD_REG_58,  0x0e05 );
    LCD_WriteReg ( LCD_REG_59,  0x0e01 );
    LCD_WriteReg ( LCD_REG_60,  0x010e );
    LCD_WriteReg ( LCD_REG_61,  0x050e );
    LCD_WriteReg ( LCD_REG_62,  0x0403 );
    LCD_WriteReg ( LCD_REG_63,  0x0607 );

    /* Set GRAM area ---------------------------------------------------------*/
    LCD_WriteReg ( LCD_REG_80,  0x0000 );  /* Horizontal GRAM Start Address */
    LCD_WriteReg ( LCD_REG_81,  0x00EF );  /* Horizontal GRAM End Address */
    LCD_WriteReg ( LCD_REG_82,  0x0000 );  /* Vertical GRAM Start Address */
    LCD_WriteReg ( LCD_REG_83,  0x013F );  /* Vertical GRAM End Address */

    LCD_WriteReg ( LCD_REG_96,  0xA700 );  /* Gate Scan Line */
    LCD_WriteReg ( LCD_REG_97,  0x0001 );  /* NDL, VLE, REV */
    LCD_WriteReg ( LCD_REG_106, 0x0000 );  /* set scrolling line */

    /* Partial Display Control -----------------------------------------------*/
    LCD_WriteReg ( LCD_REG_128, 0x0000 );
    LCD_WriteReg ( LCD_REG_129, 0x0000 );
    LCD_WriteReg ( LCD_REG_130, 0x0000 );
    LCD_WriteReg ( LCD_REG_131, 0x0000 );
    LCD_WriteReg ( LCD_REG_132, 0x0000 );
    LCD_WriteReg ( LCD_REG_133, 0x0000 );

    /* Panel Control ---------------------------------------------------------*/
    LCD_WriteReg ( LCD_REG_144, 0x0010 );
    LCD_WriteReg ( LCD_REG_146, 0x0000 );
    LCD_WriteReg ( LCD_REG_147, 0x0003 );
    LCD_WriteReg ( LCD_REG_149, 0x0110 );
    LCD_WriteReg ( LCD_REG_151, 0x0000 );
    LCD_WriteReg ( LCD_REG_152, 0x0000 );

    LCD_Display_Dir ( LCD_DIR );                                    // ������ʾ����

    /* Set GRAM write direction and BGR=1
       I/D=01 (Horizontal : increment, Vertical : decrement)
       AM=1 (address is updated in vertical writing direction) */
    LCD_WriteReg ( LCD_REG_3, LCD_Dev.SCAN_DIR );

    LCD_WriteReg ( LCD_REG_7,  0x0112 );  /* 262K color and display ON */

    LCD_SetDisplayWindow ( 0, 0, LCD_Dev.HEIGHT, LCD_Dev.WIDTH );   // ������ʾ����
  }

  LCD_SetFont(&LCD_DEFAULT_FONT, &LCD_DEFAULT_FONT_CN);             // ��������
  LCD_LED_ON;                                                       // ��������
}

//-----------------------------------------------------------------
// void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor)
//-----------------------------------------------------------------
//
//  ��������: LCD ����������ɫ��������ɫ
//  ��ڲ���: _TextColor ������ɫ; _BackColor ������ɫ
//  ���ز���: ��
//  ȫ�ֱ���: TextColor; BackColor
//  ע������: RGB 565 ��ʽ
//
//-----------------------------------------------------------------
void LCD_SetColors(
  __IO uint16_t _TextColor,
  __IO uint16_t _BackColor
)
{
  TextColor = _TextColor;
  BackColor = _BackColor;
}

//-----------------------------------------------------------------
// void LCD_GetColors(__IO uint16_t *_TextColor, __IO uint16_t *_BackColor)
//-----------------------------------------------------------------
//
//  ��������: LCD ��ȡ������ɫ��������ɫ
//  ��ڲ���: _TextColor ������ɫָ��; _BackColor ������ɫָ��
//  ���ز���: ��
//  ȫ�ֱ���: TextColor; BackColor
//  ע������: RGB 565 ��ʽ
//
//-----------------------------------------------------------------
void LCD_GetColors(
  __IO uint16_t *_TextColor,
  __IO uint16_t *_BackColor
)
{
  *_TextColor = TextColor; *_BackColor = BackColor;
}

//-----------------------------------------------------------------
// void LCD_SetTextColor(__IO uint16_t Color)
//-----------------------------------------------------------------
//
//  ��������: LCD ����������ɫ
//  ��ڲ���: Color ������ɫ
//  ���ز���: ��
//  ȫ�ֱ���: TextColor
//  ע������: RGB 565 ��ʽ
//
//-----------------------------------------------------------------
void LCD_SetTextColor(__IO uint16_t Color)
{
  TextColor = Color;
}

//-----------------------------------------------------------------
// void LCD_SetBackColor(__IO uint16_t Color)
//-----------------------------------------------------------------
//
//  ��������: LCD ���ñ�����ɫ
//  ��ڲ���: Color ������ɫ
//  ���ز���: ��
//  ȫ�ֱ���: BackColor
//  ע������: RGB 565 ��ʽ
//
//-----------------------------------------------------------------
void LCD_SetBackColor(__IO uint16_t Color)
{
  BackColor = Color;
}

//-----------------------------------------------------------------
// void LCD_SetFont(sFONT *fonts)
//-----------------------------------------------------------------
//
//  ��������: LCD ��������  
//  ��ڲ���: *fonts �ַ�������Ϣָ��  *fonts_cn ����������Ϣָ��
//  ���ز���: �� 
//  ȫ�ֱ���: LCD_Currentfonts
//  ע������: ������ ��fonts.h���ж���
//
//-----------------------------------------------------------------
void LCD_SetFont(sFONT *fonts, sFONT_CN *fonts_cn)
{
  LCD_Currentfonts    = fonts;
  LCD_Currentfonts_CN = fonts_cn;
}

//-----------------------------------------------------------------
// void LCD_Clear(uint16_t Color)
//-----------------------------------------------------------------
//
//  ��������: LCD ����
//  ��ڲ���: Color ������ɫ
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ��
//
//-----------------------------------------------------------------
void LCD_Clear(uint16_t Color)
{
  uint32_t index = 0;
  
  LCD_SetDisplayWindow ( 0, 0, LCD_Dev.HEIGHT, LCD_Dev.WIDTH );     // ���ô��ڴ�С

  LCD_WriteRAM_Prepare ( );                                         /* Prepare to write GRAM */
  for(index = 0; index < LCD_Dev.WIDTH * LCD_Dev.HEIGHT; index++)   //
  {
    LCD->LCD_RAM = Color;                                           //
  }
}

//-----------------------------------------------------------------
// void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
//-----------------------------------------------------------------
//
//  ��������: LCD ������ʾ����
//  ��ڲ���: Color ������ɫ
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: 
//      ST7789V_ID  ÿ��д��8bit
//      SPFD5408_ID ÿ��д��16bit
//
//-----------------------------------------------------------------
static void LCD_SetCursor(
  uint16_t Xpos,
  uint16_t Ypos
)
{
  if(LCD_Dev.REG_ID == ST7789V_ID)                                  // �Ĵ���ÿ��д�� 8bit, 16bit �����������д��
  {
    LCD_WR_REG ( LCD_Dev.REG_XPOSITION );
    LCD_WR_DATA ( Xpos >> 8 );       LCD_WR_DATA ( Xpos & 0XFF );
    LCD_WR_REG ( LCD_Dev.REG_YPOSITION );
    LCD_WR_DATA ( Ypos >> 8 );       LCD_WR_DATA ( Ypos & 0XFF );
  }
  if(LCD_Dev.REG_ID == SPFD5408_ID)                                 // �Ĵ���ÿ��д�� 16bit
  {
    LCD_WriteReg ( LCD_Dev.REG_XPOSITION, Xpos );
    LCD_WriteReg ( LCD_Dev.REG_YPOSITION, Ypos );
  }
} 

//-----------------------------------------------------------------
// void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c)
//-----------------------------------------------------------------
//
//  ��������: LCD ��Ӧ����д��һ�� ASCII�ַ� �ַ�����ͨ��ȡģ��ȡ
//  ��ڲ���: Xpos X������; Ypos Y������; *c ��ģ�׵�ַ
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ��ʾ����Ϊ�ַ���ʾ�����Ͻ�����, ���Ͻ�Ϊ����ԭ��
//
//-----------------------------------------------------------------
static void LCD_DrawChar(
  uint16_t Xpos, 
  uint16_t Ypos, 
  const uint8_t *CharFont
)
{
  uint32_t  index = 0, i = 0;
  uint8_t   number_byte = ((LCD_Currentfonts->Width + 7) >> 3);

  LCD_SetDisplayWindow (Xpos, Ypos, LCD_Currentfonts->Height, LCD_Currentfonts->Width);
  LCD_WriteRAM_Prepare();                                               // �����Դ�����
  for(index = 0; index < LCD_Currentfonts->Height; index++)
  {
    for(i = 0; i < LCD_Currentfonts->Width; i++)
    {
      if( (CharFont[(index * number_byte) + (i >> 3)] & (0x80 >> (i % 8))) == 0x00 ) 
      {
          LCD_WriteRAM(BackColor);                                     // д�뱳��ɫ
      }
      else
      {
          LCD_WriteRAM(TextColor);                                     // д������ɫ
      }
    }
  }
  LCD_SetDisplayWindow (0, 0, LCD_Dev.HEIGHT, LCD_Dev.WIDTH);         // ������ʾ����
}

//-----------------------------------------------------------------
// void LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii)
//-----------------------------------------------------------------
//
//  ��������: LCD ��ʾ�ַ�
//  ��ڲ���: Xpos X������; Ypos Y������; Ascii ��ʾ�ַ�
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ��ʾ����Ϊ�ַ���ʾ�����Ͻ�����, ���Ͻ�Ϊ����ԭ��
//
//-----------------------------------------------------------------
void LCD_DisplayChar(
  uint16_t  Xpos, 
  uint16_t  Ypos, 
  uint8_t   Ascii
)
{
  const uint8_t *FontAddr;

  // ��ȡ�ַ���ģ��ַ
  FontAddr = &LCD_Currentfonts->table[(Ascii-' ') *  
             LCD_Currentfonts->Height * 
            ((LCD_Currentfonts->Width + 7) / 8)];
  LCD_DrawChar( Xpos, Ypos, FontAddr );                             // ��ʾ��ģ����
}

//-----------------------------------------------------------------
// void GetChineseCode( const uint8_t *pFontAddr, uint8_t *ChineseChar )
//-----------------------------------------------------------------
//
// ��������: ��ȡ��ģ��ַ
// ��ڲ���: *pFontAddr: ������ģָ��. *ChineseChar: Ҫ������ģ�ĺ��ֱ���ָ��.
// ���ز���: ��
// ȫ�ֱ���: LCD_Currentfonts_CN ������Ϣ
// ����ģ��: ��
// ע������: ����һ�����ģ�ȡ������ģ��ַ,
// 
//-----------------------------------------------------------------
const uint8_t *GetChineseCode(
  uint8_t *ChineseChar                                              // Ҫ���ҵĺ���
)
{
  uint16_t i;
  const uint8_t *pFontAddr;                                         // ������ģ��ַ

  uint16_t GBXX_LENGTH = LCD_Currentfonts_CN->Length;               // ��ȡ��ģ���ݸ���

  // ���Ҷ�Ӧ���ֵ���ģ��ַ
  for (i = 0; i < GBXX_LENGTH; i ++)
  {
    // �Ա�����ĺ��ֺ���ģ�����еĺ����Ƿ�һ��
    if ((LCD_Currentfonts_CN->table[i].Index[0] == ChineseChar[0]) && 
        (LCD_Currentfonts_CN->table[i].Index[1] == ChineseChar[1]))
    {                                                               
      pFontAddr = &LCD_Currentfonts_CN->table[i].Msk[0];            // ��ȡ��ģ��ַ
      return pFontAddr;                                             // ������ģ��ַ
    }
  }
  return 0;                                                         // û�ж�Ӧ������ģ, ����0
}

//-----------------------------------------------------------------
// void LCD_DrawChineseChar(uint16_t Xpos, uint16_t Ypos, uint8_t *cn)
//-----------------------------------------------------------------
//
// ��������: LCD ��Ӧ����д��һ�������ַ�
// ��ڲ���: Xpos X������; Ypos Y������; *ChineseFont ��ģ�׵�ַ
// ���ز���: ��
// ȫ�ֱ���: LCD_Currentfonts_CN �����ַ���Ϣ
// ����ģ��: ��
// ע������: 
// 
//-----------------------------------------------------------------
static void LCD_DrawChineseChar(
  uint16_t Xpos,
  uint16_t Ypos,
  const uint8_t *ChineseFont
)
{
  uint16_t  index = 0, i = 0;
  uint8_t   number_byte = ((LCD_Currentfonts_CN->Width + 7) >> 3);  // ������ģÿ����Ҫ���ֽ���

  // ������ʾ����
  LCD_SetDisplayWindow (Xpos, Ypos, LCD_Currentfonts_CN->Height, LCD_Currentfonts_CN->Width); 

  LCD_WriteRAM_Prepare();                                          /* Prepare to write GRAM */
  for(index = 0; index < LCD_Currentfonts_CN->Height; index++)
  {
    for(i = 0; i < LCD_Currentfonts_CN->Width; i++)
    {
      if (ChineseFont != 0)                                         // ����ģʱ, ����д��
      {
        if ( (ChineseFont[(index * number_byte) + (i >> 3)] & (0x80 >> (i % 8))) == 0x00 )
        {
          LCD_WriteRAM(BackColor);                                  // д�뱳��ɫ
        }
        else
        {
          LCD_WriteRAM(TextColor);                                  // д������ɫ
        }
      }
      else                                                          // û����ģʱд�뱳��ɫ
      {
        LCD_WriteRAM(BackColor);                                    // д�뱳��ɫ
      }
    }
  }
  LCD_SetDisplayWindow (0, 0, LCD_Dev.HEIGHT, LCD_Dev.WIDTH);         // ������ʾ����
}

//-----------------------------------------------------------------
// void LCD_DisplayChineseChar(uint16_t Xpos, uint16_t Ypos, uint8_t ChineseCode[2])
//-----------------------------------------------------------------
//
//  ��������: LCD ��ʾ�����ַ�
//  ��ڲ���: Xpos X������; Ypos Y������; ChineseCode[2] ��ʾ���ı���
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ��ʾ����Ϊ�ַ���ʾ�����Ͻ�����, ���Ͻ�Ϊ����ԭ��
//
//-----------------------------------------------------------------
void LCD_DisplayChineseChar(
  uint16_t  Xpos, 
  uint16_t  Ypos, 
  uint8_t   ChineseCode[2]
)
{
  const uint8_t *FontAddr = 0;

  // ��ȡ������ģ��ַ
  FontAddr = GetChineseCode( ChineseCode );
  LCD_DrawChineseChar( Xpos, Ypos, FontAddr);                       // ��ʾ��ģ����
}

//-----------------------------------------------------------------
// void LCD_DisplayString(uint16_t Xpos, uint16_t  Ypos, uint8_t *ptr)
//-----------------------------------------------------------------
// 
//  ��������: LCD ��ʾ�ַ���-��Ӣ�Ļ��
//  ��ڲ���: Xpos X������; Ypos Y������; *ptr ��ʾ�ַ���ָ��
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ��ʾ����Ϊ�ַ���ʾ�����Ͻ�����, ���Ͻ�Ϊ����ԭ��
// 
//-----------------------------------------------------------------
void LCD_DisplayString(
  uint16_t Xpos,
  uint16_t Ypos,
  uint8_t *ptr
)
{
  uint16_t xAddr = Xpos;
  uint16_t yAddr = Ypos;

  /* Send the string character by character on lCD */
  while ((*ptr != 0) )
  {
    if ( xAddr <= (LCD_Dev.WIDTH - LCD_Currentfonts->Width) )
    {
      if (*ptr < 128)                                                 // �ַ�
      {
        LCD_DisplayChar(xAddr, yAddr, *ptr);                          // ��ʾӢ���ַ�
        xAddr += LCD_Currentfonts->Width;
        ptr++;                                                        // Ӣ���ַ�ռ��һ���ֽ�
      }
      else                                                            // ����
      {
        LCD_DisplayChineseChar(xAddr, yAddr, ptr);                    // ��ʾ�����ַ�
        xAddr += LCD_Currentfonts_CN->Width;
        ptr+= 2;                                                      // �����ַ�ռ�������ֽ�
      }
    }
    else 
    {
      xAddr = 0;
      yAddr = yAddr + LCD_Currentfonts->Height;
    }
  }
}

//-----------------------------------------------------------------
// void LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
//-----------------------------------------------------------------
//
//  ��������: LCD ������ʾ����
//  ��ڲ���: Xpos X������; Ypos Y������; Height �߶�; Width ���
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ��ʾ����Ϊ�ַ���ʾ�����Ͻ�����, ���Ͻ�Ϊ����ԭ��
//
//-----------------------------------------------------------------
void LCD_SetDisplayWindow(
  uint16_t Xpos,
  uint16_t Ypos,
  uint16_t Height,
  uint16_t Width
)
{
   uint16_t X_Start = 0, X_End = 0; 
   uint16_t Y_Start = 0, Y_End = 0; 
   
   uint16_t Xpos_Start = 0;
   uint16_t Ypos_Start = 0;
   
  if(LCD_Dev.REG_ID == ST7789V_ID)
  {
    X_Start     = Xpos;                               // Horizontal GRAM Start Address
    X_End       = Xpos + Width - 1;                   // Horizontal GRAM End Address
    Y_Start     = Ypos;                               // Vertical GRAM Start Address
    Y_End       = Ypos + Height - 1;                  // Vertical GRAM End Address 

    Xpos_Start  = X_Start;
    Ypos_Start  = Y_Start;

    /* Horizontal GRAM Start Address */
    LCD_WR_REG(LCD_Dev.REG_XPOSITION);
    LCD_WR_DATA ( X_Start >> 8 );                 LCD_WR_DATA ( X_Start & 0xFF );

    /* Horizontal GRAM End Address */
    LCD_WR_DATA ( X_End >> 8 );  LCD_WR_DATA ( X_End & 0XFF );

    /* Vertical GRAM Start Address */
    LCD_WR_REG(LCD_Dev.REG_YPOSITION);
    LCD_WR_DATA ( Y_Start >> 8 );                 LCD_WR_DATA ( Y_Start & 0xFF );

    /* Vertical GRAM End Address */
    LCD_WR_DATA ( Y_End >> 8); LCD_WR_DATA ( Y_End & 0XFF );

  }
  else if(LCD_Dev.REG_ID == SPFD5408_ID)
  {
    if ( LCD_Dev.DISPLAY_DIR == DISPLAY_DIR_L )
    {
      X_Start     = LCD_Dev.HEIGHT - Ypos - Height;   // Horizontal GRAM Start Address
      X_End       = LCD_Dev.HEIGHT - Ypos - 1;        // Horizontal GRAM End Address
      Y_Start     = LCD_Dev.WIDTH  - Xpos - Width;    // Vertical GRAM Start Address
      Y_End       = LCD_Dev.WIDTH  - Xpos - 1;        // Vertical GRAM End Address 

      Xpos_Start  = X_End;
      Ypos_Start  = Y_End;
    }
    else if ( LCD_Dev.DISPLAY_DIR == DISPLAY_DIR_R ) 
    {
      X_Start     = Ypos;                             // Horizontal GRAM Start Address
      X_End       = Ypos + Height - 1;                // Horizontal GRAM End Address
      Y_Start     = Xpos;                             // Vertical GRAM Start Address
      Y_End       = Xpos + Width - 1;                 // Vertical GRAM End Address 

      Xpos_Start  = X_Start;
      Ypos_Start  = Y_Start;
    }
    else if ( LCD_Dev.DISPLAY_DIR == DISPLAY_DIR_U )
    {
      X_Start     = LCD_Dev.WIDTH - Xpos - Width;     // Horizontal GRAM Start Address
      X_End       = LCD_Dev.WIDTH - Xpos - 1;         // Horizontal GRAM End Address
      Y_Start     = Ypos;                             // Vertical GRAM Start Address
      Y_End       = Ypos + Height;                    // Vertical GRAM End Address 

      Xpos_Start  = X_End;
      Ypos_Start  = Y_Start;
    }
    else 
    {
      X_Start     = Xpos;                             // Horizontal GRAM Start Address
      X_End       = Xpos + Width - 1;                 // Horizontal GRAM End Address
      Y_Start     = LCD_Dev.HEIGHT - Ypos - Height;   // Vertical GRAM Start Address
      Y_End       = LCD_Dev.HEIGHT - Ypos - 1;        // Vertical GRAM End Address 

      Xpos_Start  = X_Start;
      Ypos_Start  = Y_End;
    }
    LCD_WriteReg ( LCD_REG_80, X_Start ); 
    LCD_WriteReg ( LCD_REG_81, X_End );
    LCD_WriteReg ( LCD_REG_82, Y_Start );
    LCD_WriteReg ( LCD_REG_83, Y_End );
  }
  LCD_SetCursor(Xpos_Start, Ypos_Start);
}

//-----------------------------------------------------------------
// void LCD_WindowModeDisable(void)
//-----------------------------------------------------------------
//
//  ��������: LCD �رմ�����ʾģʽ
//  ��ڲ���: ��
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ��
//
//-----------------------------------------------------------------
void LCD_WindowModeDisable(void)
{
  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
//  LCD_WriteReg(LCD_REG_3, 0x1018);
  LCD_WriteReg(LCD_REG_3, 0x1018);
}

//-----------------------------------------------------------------
// void LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
//-----------------------------------------------------------------
//
//  ��������: LCD ����ˮƽֱ��
//  ��ڲ���: Xpos X������; Ypos Y������; Length ����
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ���Ͻ�Ϊ����ԭ��
//
//-----------------------------------------------------------------
void LCD_DrawHLine(
  uint16_t Xpos,
  uint16_t Ypos,
  uint16_t Length
)
{
  uint32_t i = 0;

  LCD_SetDisplayWindow (Xpos, Ypos, 1, Length );
  LCD_WriteRAM_Prepare();                       /* Prepare to write GRAM */
  for(i = 0; i < Length; i++)
  {
    LCD_WriteRAM(TextColor);
  }
  LCD_SetDisplayWindow (0, 0, LCD_Dev.HEIGHT, LCD_Dev.WIDTH);         // ������ʾ����
}

//-----------------------------------------------------------------
// void LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
//-----------------------------------------------------------------
//
//  ��������: LCD ����ˮƽ��ֱ��
//  ��ڲ���: Xpos X������; Ypos Y������; Length ����
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ���Ͻ�Ϊ����ԭ��
//
//-----------------------------------------------------------------
void LCD_DrawDotHLine(
  uint16_t Xpos,
  uint16_t Ypos,
  uint16_t Length
)
{
  uint32_t i = 0;

  LCD_SetDisplayWindow (Xpos, Ypos, 1, Length );
  LCD_WriteRAM_Prepare();                       /* Prepare to write GRAM */
  for(i = 0; i < Length; i++)
  {
    if ((i % DOT) == (DOT -1))
    {
      LCD_WriteRAM(BackColor);
    }
    else 
    {
      LCD_WriteRAM(TextColor);
    }
  }
  LCD_SetDisplayWindow (0, 0, LCD_Dev.HEIGHT, LCD_Dev.WIDTH);         // ������ʾ����
}

//-----------------------------------------------------------------
// void LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Height)
//-----------------------------------------------------------------
//
//  ��������: LCD ���ƴ�ֱֱ��
//  ��ڲ���: Xpos X������; Ypos Y������; Height �߶�
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ���Ͻ�Ϊ����ԭ��
//
//-----------------------------------------------------------------
void LCD_DrawVLine(
  uint16_t Xpos,
  uint16_t Ypos,
  uint16_t Height
)
{
  uint32_t i = 0;

  LCD_SetDisplayWindow (Xpos, Ypos, Height, 1 );
  LCD_WriteRAM_Prepare();                     /* Prepare to write GRAM */
  for(i = 0; i < Height; i++)
  {
    LCD_WriteRAM(TextColor);
  }
  LCD_SetDisplayWindow (0, 0, LCD_Dev.HEIGHT, LCD_Dev.WIDTH);         // ������ʾ����
}

//-----------------------------------------------------------------
// void LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Height)
//-----------------------------------------------------------------
//
//  ��������: LCD ���ƴ�ֱ��ֱ��
//  ��ڲ���: Xpos X������; Ypos Y������; Height �߶�
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ���Ͻ�Ϊ����ԭ��
//
//-----------------------------------------------------------------
void LCD_DrawDotVLine(
  uint16_t Xpos,
  uint16_t Ypos,
  uint16_t Height
)
{
  uint32_t i = 0;

  LCD_SetDisplayWindow (Xpos, Ypos, Height, 1 );
  LCD_WriteRAM_Prepare();                     /* Prepare to write GRAM */
  for(i = 0; i < Height; i++)
  {
    if ((i % DOT) == (DOT -1))
    {
      LCD_WriteRAM(BackColor);
    }
    else 
    {
      LCD_WriteRAM(TextColor);
    }
  }
  LCD_SetDisplayWindow (0, 0, LCD_Dev.HEIGHT, LCD_Dev.WIDTH);         // ������ʾ����
}

//-----------------------------------------------------------------
// void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
//-----------------------------------------------------------------
//
//  ��������: LCD ���ƾ���
//  ��ڲ���: Xpos X������; Ypos Y������; Height �߶�; Width ���
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ���Ͻ�Ϊ����ԭ��, ��ʾ����Ϊ���Ͻ�
//
//-----------------------------------------------------------------
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
{
  LCD_DrawHLine(Xpos, Ypos, Width);
  LCD_DrawHLine(Xpos, (Ypos + Height -1), Width);

  LCD_DrawVLine(Xpos, Ypos, Height);
  LCD_DrawVLine((Xpos + Width - 1), Ypos, Height);
}

//-----------------------------------------------------------------
// void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
//-----------------------------------------------------------------
//
//  ��������: LCD ��ʾ��ɫͼƬ   ���ݿ���ͨ��ȡģ��ʽ��ȡ
//  ��ڲ���: Xpos X������; Ypos Y������; Height �߶�; Width ���
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ���Ͻ�Ϊ����ԭ��
//
//-----------------------------------------------------------------
void LCD_DrawMonoPict(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width, const uint8_t *Pict)
{
  uint32_t index = 0, i = 0;

  LCD_SetDisplayWindow (Xpos, Ypos, Height, Width );
  LCD_WriteRAM_Prepare();                     /* Prepare to write GRAM */

  for(index = 0; index < (Height * Width) >> 3; index++)
  {
    for (i = 0; i  < 8; i ++)
    {
      if((Pict[index] & (1 << i)) == 0x00)
      {
        LCD_WriteRAM(BackColor);
      }
      else
      {
        LCD_WriteRAM(TextColor);
      }
    }
  }
  LCD_SetDisplayWindow (0, 0, LCD_Dev.HEIGHT, LCD_Dev.WIDTH);         // ������ʾ����
}

//-----------------------------------------------------------------
// void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
//-----------------------------------------------------------------
//
//  ��������: LCD ��ʾRGBͼƬ 
//  ��ڲ���: Xpos X������; Ypos Y������; Height �߶�; Width ���
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ���Ͻ�Ϊ����ԭ��
//
//-----------------------------------------------------------------
void LCD_DrawRgbPict(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width, const uint8_t *Pict)
{
  uint32_t index = 0;

  LCD_SetDisplayWindow (Xpos, Ypos, Height, Width );
  LCD_WriteRAM_Prepare();                     /* Prepare to write GRAM */

  for(index = 0; index < (Height * Width); index++)
  {
    LCD_WriteRAM (( Pict[ index << 1 ])+ (Pict[(index<<1) + 1]  << 8));
  }
  LCD_SetDisplayWindow (0, 0, LCD_Dev.HEIGHT, LCD_Dev.WIDTH);         // ������ʾ����
}

//-----------------------------------------------------------------
// void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
//-----------------------------------------------------------------
//
//  ��������: LCD ���������� �����ɫΪ������ɫ
//  ��ڲ���: Xpos X������; Ypos Y������; Height �߶�; Width ���
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ���Ͻ�Ϊ����ԭ��
//
//-----------------------------------------------------------------
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
{
  uint32_t index = 0;

  LCD_SetDisplayWindow (Xpos, Ypos, Height, Width );
  LCD_WriteRAM_Prepare();                     /* Prepare to write GRAM */

  for(index = 0; index < (Height * Width); index++)
  {
    LCD_WriteRAM ( BackColor );
  }
  LCD_SetDisplayWindow (0, 0, LCD_Dev.HEIGHT, LCD_Dev.WIDTH);         // ������ʾ����
}

//-----------------------------------------------------------------
// void LCD_PowerOn(void)
//-----------------------------------------------------------------
//
//  ��������: LCD �򿪵�Դ
//  ��ڲ���: ��
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ��
//
//-----------------------------------------------------------------
void LCD_PowerOn(void)
{
   if ( LCD_Dev.REG_ID == ST7789V_ID )
   {

   }
   else
   {
     /* Power On sequence ---------------------------------------------------------*/
     LCD_WriteReg(LCD_REG_16, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
     LCD_WriteReg(LCD_REG_17, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
     LCD_WriteReg(LCD_REG_18, 0x0000); /* VREG1OUT voltage */
     LCD_WriteReg(LCD_REG_19, 0x0000); /* VDV[4:0] for VCOM amplitude*/
     LCD_Delay(200);                   /* Dis-charge capacitor power voltage (200ms) */
     LCD_WriteReg(LCD_REG_16, 0x17B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
     LCD_WriteReg(LCD_REG_17, 0x0137); /* DC1[2:0], DC0[2:0], VC[2:0] */
     LCD_Delay(50);                    /* Delay 50 ms */
     LCD_WriteReg(LCD_REG_18, 0x0139); /* VREG1OUT voltage */
     LCD_Delay(50);                    /* Delay 50 ms */
     LCD_WriteReg(LCD_REG_19, 0x1d00); /* VDV[4:0] for VCOM amplitude */
     LCD_WriteReg(LCD_REG_41, 0x0013); /* VCM[4:0] for VCOMH */
     LCD_Delay(50);                    /* Delay 50 ms */
     LCD_WriteReg(LCD_REG_7, 0x0173);  /* 262K color and display ON */
   }
}

//-----------------------------------------------------------------
// void LCD_DisplayOn(void)
//-----------------------------------------------------------------
//
//  ��������: LCD �ر���ʾ
//  ��ڲ���: ��
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ��
//
//-----------------------------------------------------------------
void LCD_DisplayOn(void)
{
  if ( LCD_Dev.REG_ID == ST7789V_ID )
  {
    LCD_WR_REG(0x29);
  }
  else
  {
    /* Display On */
    LCD_WriteReg(LCD_REG_7, 0x0173); /* 262K color and display ON */
  }
}

//-----------------------------------------------------------------
// void LCD_DisplayOff(void)
//-----------------------------------------------------------------
//
//  ��������: LCD ������ʾ
//  ��ڲ���: ��
//  ���ز���: ��
//  ȫ�ֱ���: ��
//  ע������: ��
//
//-----------------------------------------------------------------
void LCD_DisplayOff(void)
{
  /* Display Off */
    if ( LCD_Dev.REG_ID == ST7789V_ID )
    {
        LCD_WR_REG(0x28);
    }
    else
    {
        LCD_WriteReg(LCD_REG_7, 0x0);
    }
}

// ����һ�����ص�
/**
  * @brief  Displays a pixel.
  * @param  x: pixel x.
  * @param  y: pixel y.
  * @retval None
  */
void PutPixel(int16_t x, int16_t y)
{
  if(x < 0 || x > (LCD_Dev.WIDTH - 1) || y < 0 || y > (LCD_Dev.HEIGHT - 1))
  {
    return;
  }
  LCD_DrawHLine(x, y, 1);
}


//-----------------------------------------------------------------
//void LCD_SetPoint(
//  uint16_t Xpos,                                                    // X������
//  uint16_t Ypos,                                                    // y������
//  uint16_t PointColor                                               // ���ص���ɫ
//)
//-----------------------------------------------------------------
//
// ��������: ��ָ������ɫ��ָ��������λ���ϻ���һ����
// ��ڲ���: 
//           Xpos:           ���Ͻ�x��ʼ����. 
//           Ypos:           ���Ͻ�y��ʼ����.
//           PointColor:     ���ص���ɫ.
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: LCD_ColorBox().
// ע������:
//-----------------------------------------------------------------
void LCD_SetPoint(
  uint16_t Xpos,                                                    // X������
  uint16_t Ypos,                                                    // y������
  uint16_t PointColor                                               // ���ص���ɫ
)
{
  uint16_t x = Xpos;
  uint16_t y = Ypos;
  
  if ( LCD_Dev.REG_ID == SPFD5408_ID )                              // SPFD5408 ��Ҫ������任
  {
    if ( LCD_Dev.DISPLAY_DIR == DISPLAY_DIR_L )
    {
      x  = LCD_Dev.HEIGHT - Ypos - 1;
      y  = LCD_Dev.WIDTH  - Xpos - 1;
    }
    else if ( LCD_Dev.DISPLAY_DIR == DISPLAY_DIR_R ) 
    {
      x  = Ypos;
      y  = Xpos;
    }
    else if ( LCD_Dev.DISPLAY_DIR == DISPLAY_DIR_U )
    {
      x  = LCD_Dev.WIDTH - Xpos - 1;
      y  = Ypos;
    }
    else 
    {
      x  = Xpos;
      y  = LCD_Dev.HEIGHT - Ypos - 1;
    }
  }
  LCD_SetCursor( x, y );
  LCD_WriteRAM_Prepare();                       /* Prepare to write GRAM */
  LCD_WriteRAM( PointColor );
}


//-----------------------------------------------------------------
//void LCD_WriteString(
//  uint16_t Xpos,                                                    // X������
//  uint16_t Ypos,                                                    // y������
//  uint16_t CharColor,                                               // ������ɫ
//  uint16_t CharBackColor,                                           // ������ɫ
//  uint8_t  *sp                                                      // ָ���ַ������ַ�ָ��
//)
//-----------------------------------------------------------------
//
// ��������: LCDд�ַ�������, ����Ӣ���ַ��ͺ����ַ����д.
// ��ڲ���:
//           Xpos:           ���Ͻ�x��ʼ����. 
//           Ypos:           ���Ͻ�y��ʼ����.
//           CharColor:      �ַ���ɫ.
//           CharBackColor:  ������ɫ.
//           *sp  :          ��ʾ���ַ���ָ��.
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: LCD_WriteASCII(),  LCD_WriteChinese().
// ע������:
//-----------------------------------------------------------------
void LCD_WriteString(
  uint16_t Xpos,                                                    // X������
  uint16_t Ypos,                                                    // y������
  uint16_t CharColor,                                               // ������ɫ
  uint16_t CharBackColor,                                           // ������ɫ
  uint8_t  *sp                                                      // ָ���ַ������ַ�ָ��
)
{
  LCD_SetColors(CharColor, CharBackColor);                          // ����������ɫ�ͱ�����ɫ
  LCD_DisplayString(Xpos, Ypos, sp);                                 // ��ʾ�ַ���
}

//-----------------------------------------------------------------
//void LCD_WriteChinese24x24string(
//  uint16_t Xpos,                                                    // x������
//  uint16_t Ypos,                                                    // y������
//  uint16_t CharColor,                                               // ������ɫ
//  uint16_t CharBackColor,                                           // ������ɫ
//  uint8_t  *sp                                                      // �����ַ�ָ��
//)
//-----------------------------------------------------------------
//
// ��������: LCDд�����ַ�������(24X24)
// ��ڲ���:
//           Xpos:           ���Ͻ�x��ʼ����. 
//           Ypos:           ���Ͻ�y��ʼ����.
//           CharColor:      �ַ���ɫ.
//           CharBackColor:  ������ɫ.
//           *sp  :          ��ʾ�������ַ���ָ��.
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: 
// ע������: ԭ��ο�����Ϊ���Ͻ�, ��ʾ����Ϊ���Ͻ�
//-----------------------------------------------------------------
void LCD_WriteChinese24x24string(
  uint16_t Xpos,                                                    // x������
  uint16_t Ypos,                                                    // y������
  uint16_t CharColor,                                               // ������ɫ
  uint16_t CharBackColor,                                           // ������ɫ
  uint8_t  *sp                                                      // �����ַ�ָ��
)
{
  LCD_SetColors(CharColor, CharBackColor);                          // ����������ɫ�ͱ�����ɫ
  LCD_SetFont(&Font16, &Font24_CN);                                 // ������������Ϊ24x24
  LCD_DisplayString(Xpos, Ypos, sp);                                // ��ʾ�ַ���
  LCD_SetFont(&LCD_DEFAULT_FONT, &LCD_DEFAULT_FONT_CN);             // Ĭ������
}


//-----------------------------------------------------------------
//void LCD_WriteNumChar (
//  uint16_t Xpos,                                                    // x������
//  uint16_t Ypos,                                                    // y������
//  uint16_t CharColor,                                               // ������ɫ
//  uint16_t CharBackColor,                                           // ������ɫ
//  uint8_t  udata                                                    // ��ʾ����ֵ
//)
//-----------------------------------------------------------------
//
// ��������: LCD��ʾ�ַ���ֵ
// ��ڲ���:
//           Xpos:           ���Ͻ�x��ʼ����. 
//           Ypos:           ���Ͻ�y��ʼ����.
//           CharColor:      �ַ���ɫ.
//           CharBackColor:  ������ɫ.
//           udata:          ��ʾ����ֵ.
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: 
// ע������: 
//          (1) ��ʾ��ֵΪ0~255֮��
//          (2) ������ʾ��ʽ: �Ҷ���
//          (3) ԭ��ο�����Ϊ���Ͻ�, ��ʾ����Ϊ���Ͻ�
//-----------------------------------------------------------------
void LCD_WriteNumChar (
  uint16_t Xpos,                                                    // x������
  uint16_t Ypos,                                                    // y������
  uint16_t CharColor,                                               // ������ɫ
  uint16_t CharBackColor,                                           // ������ɫ
  uint8_t  udata                                                    // ��ʾ����ֵ
)
{
  uint8_t display_buf[4];
  
  sprintf ((char *)display_buf, "%3d", udata);
  
  LCD_WriteString(Xpos, Ypos, CharColor, CharBackColor, display_buf); 
}

//-----------------------------------------------------------------
//void LCD_WriteNumInt (
//  uint16_t Xpos,                                                    // x������
//  uint16_t Ypos,                                                    // y������
//  uint16_t CharColor,                                               // ������ɫ
//  uint16_t CharBackColor,                                           // ������ɫ
//  uint16_t udata                                                    // ��ʾ����ֵ
//)
//-----------------------------------------------------------------
//
// ��������: LCD��ʾ������ֵ
// ��ڲ���: 
//           Xpos:           ���Ͻ�x��ʼ����. 
//           Ypos:           ���Ͻ�y��ʼ����.
//           CharColor:      �ַ���ɫ.
//           CharBackColor:  ������ɫ.
//           udata:          ��ʾ����ֵ.
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: LCD_WriteString().
// ע������: 
//          (1) ��ʾ��ֵΪ0~65535֮��
//          (2) ������ʾ��ʽ: �Ҷ���
//          (3) ԭ��ο�����Ϊ���Ͻ�, ��ʾ����Ϊ���Ͻ�
//-----------------------------------------------------------------
void LCD_WriteNumInt (
  uint16_t Xpos,                                                    // x������
  uint16_t Ypos,                                                    // y������
  uint16_t CharColor,                                               // ������ɫ
  uint16_t CharBackColor,                                           // ������ɫ
  uint16_t udata                                                    // ��ʾ����ֵ
)
{
  uint8_t display_buf[6];
  
  sprintf ((char *)display_buf, "%5d", udata);
  
  LCD_WriteString(Xpos, Ypos, CharColor, CharBackColor, display_buf); 
}

//-----------------------------------------------------------------
//void LCD_WriteNumLong (
//  uint16_t Xpos,                                                    // x������
//  uint16_t Ypos,                                                    // y������
//  uint16_t CharColor,                                               // ������ɫ
//  uint16_t CharBackColor,                                           // ������ɫ
//  uint32_t udata                                                    // ��ʾ����ֵ
//)
//-----------------------------------------------------------------
//
// ��������: LCD��ʾ������ֵ
// ��ڲ���: 
//           Xpos:           ���Ͻ�x��ʼ����. 
//           Ypos:           ���Ͻ�y��ʼ����.
//           CharColor:      �ַ���ɫ.
//           CharBackColor:  ������ɫ.
//           udata:          ��ʾ����ֵ.
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: 
// ע������: 
//          (1) ��ʾ��ֵΪ0~4294967296֮��
//          (2) ������ʾ��ʽ: �Ҷ���
//          (3) ԭ��ο�����Ϊ���Ͻ�, ��ʾ����Ϊ���Ͻ�
//-----------------------------------------------------------------
void LCD_WriteNumLong (
  uint16_t Xpos,                                                    // x������
  uint16_t Ypos,                                                    // y������
  uint16_t CharColor,                                               // ������ɫ
  uint16_t CharBackColor,                                           // ������ɫ
  uint32_t udata                                                    // ��ʾ����ֵ
)
{
  uint8_t display_buf[11];

  sprintf ((char *)display_buf, "%10lld", (unsigned long long)udata);
  LCD_WriteString(Xpos, Ypos, CharColor, CharBackColor, display_buf); 
}


//-----------------------------------------------------------------
// void LCD_HLine(u16 x, u16 y, u16 Length, u16 Color)
//-----------------------------------------------------------------
//
// ��������: ����ˮƽʵ��
// ��ڲ���: 
//           Xpos:        ���Ͻ�x��ʼ����. 
//           Ypos:        ���Ͻ�y��ʼ����.
//           Length:      ��������.
//           LineColor:   ������ɫ.
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: 
// ע������: ԭ��ο�����Ϊ���Ͻ�, ��ʾ����Ϊ���Ͻ�
//-----------------------------------------------------------------
void LCD_Line_H(
  uint16_t Xpos,                                                    // x������
  uint16_t Ypos,                                                    // y������
  uint16_t Length,                                                  // ����
  uint16_t LineColor                                                // ������ɫ
)
{
  LCD_SetColors ( LineColor, BackColor );
  LCD_DrawHLine ( Xpos, Ypos, Length );
}

//-----------------------------------------------------------------
//void LCD_DotLine_H(
//  uint16_t Xpos,                                                    // x������
//  uint16_t Ypos,                                                    // y������
//  uint16_t Length,                                                  // ����
//  uint16_t LineColor,                                               // ������ɫ
//  uint16_t BackColor_                                               // ������ɫ
//)
//-----------------------------------------------------------------
//
// ��������: ����ˮƽ����
// ��ڲ���: 
//           Xpos:        ���Ͻ�x��ʼ����. 
//           Ypos:        ���Ͻ�y��ʼ����.
//           Length:      ��������.
//           LineColor:   ������ɫ.
//           BackColor_:  ������ɫ.
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: 
// ע������: ԭ��ο�����Ϊ���Ͻ�, ��ʾ����Ϊ���Ͻ�
//-----------------------------------------------------------------
void LCD_DotLine_H(
  uint16_t Xpos,                                                    // x������
  uint16_t Ypos,                                                    // y������
  uint16_t Length,                                                  // ����
  uint16_t LineColor,                                               // ������ɫ
  uint16_t BackColor_                                               // ������ɫ
)
{
  LCD_SetColors ( LineColor, BackColor_ );
  LCD_DrawDotHLine ( Xpos, Ypos, Length );
}

//-----------------------------------------------------------------
//void LCD_Line_V(
//  uint16_t Xpos,                                                    // x������
//  uint16_t Ypos,                                                    // y������
//  uint16_t High,                                                    // ����
//  uint16_t LineColor                                                // ������ɫ
//)
//-----------------------------------------------------------------
//
// ��������: ���ƴ�ֱʵ��
// ��ڲ���: 
//           Xpos:       ���Ͻ�x��ʼ����. 
//           Ypos:       ���Ͻ�y��ʼ����.
//           High:       �����߶�.
//           LineColor:  ������ɫ.
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: 
// ע������: ԭ��ο�����Ϊ���Ͻ�, ��ʾ����Ϊ���Ͻ�
//-----------------------------------------------------------------
void LCD_Line_V(
  uint16_t Xpos,                                                    // x������
  uint16_t Ypos,                                                    // y������
  uint16_t High,                                                    // ����
  uint16_t LineColor                                                // ������ɫ
)
{
  LCD_SetColors ( LineColor, BackColor );
  LCD_DrawVLine ( Xpos, Ypos, High );
}

//-----------------------------------------------------------------
//void LCD_DotLine_V(
//  uint16_t Xpos,                                                    // x������
//  uint16_t Ypos,                                                    // y������
//  uint16_t High,                                                    // ����
//  uint16_t LineColor,                                               // ������ɫ
//  uint16_t BackColor_                                               // ������ɫ
//)
//-----------------------------------------------------------------
//
// ��������: ���ƴ�ֱ����
// ��ڲ���: 
//           Xpos:       ���Ͻ�x��ʼ����. 
//           Ypos:       ���Ͻ�y��ʼ����.
//           High:       �����߶�.
//           LineColor:  ������ɫ.
//           BackColor_: ������ɫ.
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: 
// ע������: ԭ��ο�����Ϊ���Ͻ�, ��ʾ����Ϊ���Ͻ�
//-----------------------------------------------------------------
void LCD_DotLine_V(
  uint16_t Xpos,                                                    // x������
  uint16_t Ypos,                                                    // y������
  uint16_t High,                                                    // ����
  uint16_t LineColor,                                               // ������ɫ
  uint16_t BackColor_                                               // ������ɫ
)
{
  LCD_SetColors ( LineColor, BackColor_ );
  LCD_DrawDotVLine ( Xpos, Ypos, High );
}


//-----------------------------------------------------------------
//void LCD_LineBox(
//  uint16_t Xpos,                                                    // x������
//  uint16_t Ypos,                                                    // y������
//  uint16_t Width,                                                   // ���
//  uint16_t High,                                                    // �߶�
//  uint16_t LineColor                                                // ������ɫ
//)
//-----------------------------------------------------------------
//
// ��������: ���ƾ��α߿�
// ��ڲ���: 
//           Xpos:      ���Ͻ�x��ʼ����. 
//           Ypos:      ���Ͻ�y��ʼ����.
//           High:      ���θ߶�.
//           Width:     ���ο��.
//           LineColor: ����������ɫ.
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��:
// ע������: ԭ��ο�����Ϊ���Ͻ�, ��������Ϊ���Ͻ�
//-----------------------------------------------------------------
void LCD_LineBox(
  uint16_t Xpos,                                                    // x������
  uint16_t Ypos,                                                    // y������
  uint16_t High,                                                    // �߶�
  uint16_t Width,                                                   // ���
  uint16_t LineColor                                                // ������ɫ
)
{
  LCD_SetColors ( LineColor, BackColor );
  LCD_DrawRect ( Xpos, Ypos, High, Width );
}


//-----------------------------------------------------------------
//void LCD_FillBox(
//  uint16_t Xpos,                                                    // x������
//  uint16_t Ypos,                                                    // y������
//  uint16_t Width,                                                   // ���
//  uint16_t High,                                                    // �߶�
//  uint16_t FillColor                                                // ������ɫ
//)
//-----------------------------------------------------------------
//
// ��������: ����ʵ�ľ���
// ��ڲ���: 
//           Xpos:      ���Ͻ�x��ʼ����. 
//           Ypos:      ���Ͻ�y��ʼ����.
//           High:      ���θ߶�.
//           Width:     ���ο��.
//           FillColor: ���������ɫ.
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��:
// ע������:  ԭ��ο�����Ϊ���Ͻ�, ��������Ϊ���Ͻ�
//-----------------------------------------------------------------
void LCD_FillBox(
  uint16_t Xpos,                                                    // x������
  uint16_t Ypos,                                                    // y������
  uint16_t High,                                                    // �߶�
  uint16_t Width,                                                   // ���
  uint16_t FillColor                                                // ������ɫ
)
{
  LCD_SetColors ( TextColor, FillColor );
  LCD_DrawFullRect ( Xpos, Ypos, High, Width );
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
