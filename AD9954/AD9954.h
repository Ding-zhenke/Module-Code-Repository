#ifndef __AD9954_H
#define __AD9954_H	 
#include "sys.h"
/*
** 函数名称 ：void GPIO_AD9954_Init(void)
** 函数功能 ：AD9954接口IO初始化
**            AD9954_CS----------PB9	 		OUT
**	      		AD9954_SCLK--------PB10	 		OUT
**            AD9954_SDIO--------PB11 	  OUT
**            AD9954_OSK---------PB12   	OUT
**          	PS0----------------PB13		  OUT
**	      		PS1----------------PB14	 		OUT
**            IOUPDATE-----------PB15	  	OUT

**	      		AD9954_SDO---------PB5	  	IN
**            AD9954_IOSY--------PB6	  	OUT
**            AD9954_RES---------PB7    	OUT
**            AD9954_PWR---------PB8    	OUT
*/



#define AD9954_CS(n) 	(n?HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOG,GPIO_PIN_10,GPIO_PIN_RESET))// PG10	
#define AD9954_SCLK(n) 	(n?HAL_GPIO_WritePin(GPIOG,GPIO_PIN_11,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOG,GPIO_PIN_11,GPIO_PIN_RESET))// PG11
#define AD9954_SDIO(n) 	(n?HAL_GPIO_WritePin(GPIOG,GPIO_PIN_12,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOG,GPIO_PIN_12,GPIO_PIN_RESET))// PG12	
#define AD9954_OSK(n) 	(n?HAL_GPIO_WritePin(GPIOG,GPIO_PIN_13,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOG,GPIO_PIN_13,GPIO_PIN_RESET))// PG13	

#define PS0(n) 			(n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_8,GPIO_PIN_RESET))// PH8
#define PS1(n) 			(n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_9,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_9,GPIO_PIN_RESET))// PH9	
#define IOUPDATE(n) 	(n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_10,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_10,GPIO_PIN_RESET))// PH10

#define AD9954_SDO		HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_11)// PH11
#define AD9954_IOSY(n) 	(n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_12,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_12,GPIO_PIN_RESET))// PH12
#define AD9954_RES(n) 	(n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_13,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_13,GPIO_PIN_RESET))// PH13
#define AD9954_PWR(n) 	(n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_14,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_14,GPIO_PIN_RESET))// PH14


#define CFR1	0X00			
#define CFR2    0X01
#define ASF     0X02
#define ARR     0X03
#define FTW0    0X04
#define POW0    0X05
#define FTW1    0X06
#define NLSCW   0X07
#define PLSCW   0X08
#define RSCW0   0X07
#define RSCW1   0X08
#define RSCW2   0X09
#define RSCW3   0X0A
#define RAM     0X0B

#define No_Dwell 0X80

typedef enum {
DownScan  =   0,
UpScan,
DoubleScan
}ScanMode;


void AD9954_Init(void);
void AD9954_SETFRE(double f);//single tone
void Single_Tone(double Freq);
//void AD9954_Init(void);
//void Single_Tone(double Freq);
void Linear_Sweep(
    double  MinFreq, double MaxFreq, 
    double  UpStepFreq, u8 UpStepTime,
    double  DownStepFreq, u8 DownStepTime,
    ScanMode      Mode
);
//void PS0_WriteBit(BitAction BitVal);
void SetASF(u16 data);
void DirectSwitchFSK(double f1, double f2, double f3, double f4);
		void DirectSwitchPSK(float f, float phase1, float phase2, float phase3, float phase4);
void AM_Init(void);
void Write_ASF(u16 factor);  //2ASK
u32 Read_Vau1(void);
u32 Read_Vau2(void);
		void LINEARSWEEP(float f1,float f2);//linear sweep mode
#endif

