/**
  ******************************************************************************
<<<<<<< HEAD:电设工程文件库/摩斯电码接收机/USER/stm32h7xx_it.h
  * @file    Templates/Inc/stm32h7xx_it.h 
  * @author  MCD Application Team
  * @brief   This file contains the headers of the interrupt handlers.
=======
  * @file    stm32h7xx_hal_spi_ex.h
  * @author  MCD Application Team
  * @brief   Header file of SPI HAL Extended module.
>>>>>>> a42994a0c05022f8f4e83a98fe157da3bc53d2ec:H7_DSP/HALLIB/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_spi_ex.h
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
<<<<<<< HEAD:电设工程文件库/摩斯电码接收机/USER/stm32h7xx_it.h
#ifndef __STM32H7xx_IT_H
#define __STM32H7xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 
=======
#ifndef __STM32H7xx_HAL_SPI_EX_H
#define __STM32H7xx_HAL_SPI_EX_H

#ifdef __cplusplus
extern "C" {
#endif
>>>>>>> a42994a0c05022f8f4e83a98fe157da3bc53d2ec:H7_DSP/HALLIB/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_spi_ex.h

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal_def.h"

/** @addtogroup STM32H7xx_HAL_Driver
  * @{
  */

/** @addtogroup SPIEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
<<<<<<< HEAD:电设工程文件库/摩斯电码接收机/USER/stm32h7xx_it.h
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
=======
/* Exported macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @addtogroup SPIEx_Exported_Functions
  * @{
  */

/* Initialization and de-initialization functions  ****************************/
/* IO operation functions *****************************************************/
/** @addtogroup SPIEx_Exported_Functions_Group1
  * @{
  */
HAL_StatusTypeDef HAL_SPIEx_FlushRxFifo(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPIEx_EnableLockConfiguration(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPIEx_ConfigureUnderrun(SPI_HandleTypeDef *hspi, uint32_t UnderrunDetection, uint32_t UnderrunBehaviour);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
>>>>>>> a42994a0c05022f8f4e83a98fe157da3bc53d2ec:H7_DSP/HALLIB/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_spi_ex.h

#ifdef __cplusplus
}
#endif

<<<<<<< HEAD:电设工程文件库/摩斯电码接收机/USER/stm32h7xx_it.h
#endif /* __STM32H7xx_IT_H */
=======
#endif /* __STM32H7xx_HAL_SPI_EX_H */
>>>>>>> a42994a0c05022f8f4e83a98fe157da3bc53d2ec:H7_DSP/HALLIB/STM32H7xx_HAL_Driver/Inc/stm32h7xx_hal_spi_ex.h

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
