#ifndef _ADF4351_H_
#define _ADF4351_H_
#include "sys.h"

#define ADF4351_LE 				HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,GPIO_PIN_SET);
#define ADF4351_LE_L 			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,GPIO_PIN_RESET);

#define ADF4351_CE 				HAL_GPIO_WritePin(GPIOG,GPIO_PIN_11,GPIO_PIN_SET);
#define ADF4351_CE_L			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_11,GPIO_PIN_RESET);

#define ADF4351_OUTPUT_DATA 	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_12,GPIO_PIN_SET);
#define ADF4351_INPUT_DATA 		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_12,GPIO_PIN_RESET);

#define ADF4351_CLK				HAL_GPIO_WritePin(GPIOG,GPIO_PIN_13,GPIO_PIN_SET);
#define ADF4351_CLK_L			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_13,GPIO_PIN_RESET);

//										无用的管脚


#define ADF4351_RF_OFF	((u32)0XEC801C)

void ADF4351Init(void);
void ReadToADF4351(u8 count, u8 *buf);
void WriteToADF4351(u8 count, u8 *buf);
void WriteOneRegToADF4351(u32 Regster);
void ADF4351_Init_some(void);
void ADF4351WriteFreq(float Fre);		//	(xx.x) M Hz

#endif

