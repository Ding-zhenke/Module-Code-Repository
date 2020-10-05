#ifndef _ADF4002_H_
#define _ADF4002_H_


#include "sys.h"

#define R_Address 0X000000
#define N_Address 0X000001
#define F_Address 0X000002
#define I_Address 0X000003
#define Pre_R 0X000000          //X000,0000,DAT(14),00
#define Pre_N 0X000000          //XX0,DAT(13),XXXXXX,01

//¹Ü½ÅÉùÃ÷
//GND
#define PLL_SCK(n) (n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET))
#define PLL_SDI(n) (n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET))//
#define PLL_SEN(n) (n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET))// PC11
                                                                                                                                                                                                                                                                                                                        

void InitADF4002(void);
void Delay(unsigned int z);
void DelayMs(void);
void SendDataPll(unsigned long int Data);

void RDivideTest(u16 RData);
void NDivideTest(u16 NData);


#endif
