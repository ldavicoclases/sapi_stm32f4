/* Copyright 2016, Eric Pernia.
 * Copyright 2018, Nahuel Espinosa.
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/* Date: 2018-01-02 */

/*==================[inclusions]=============================================*/

#include "sapi_spi.h"
#include "sapi_gpio.h"

#include "stm32f4xx_hal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

SPI_HandleTypeDef hspi1;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

bool_t spiConfig( spiMap_t spi ){

   bool_t retVal = FALSE;

   if( spi == SPI_1 ){
      __HAL_RCC_SPI1_CLK_ENABLE();

      gpioConfig(SPI1_SCK , GPIO_ALT_FUNCTION);
      gpioConfig(SPI1_MOSI, GPIO_ALT_FUNCTION);
      gpioConfig(SPI1_MISO, GPIO_ALT_FUNCTION);

      /* SPI1 parameter configuration*/
      hspi1.Instance               = SPI1;
      hspi1.Init.Mode              = SPI_MODE_MASTER;
      hspi1.Init.Direction         = SPI_DIRECTION_2LINES;
      hspi1.Init.DataSize          = SPI_DATASIZE_8BIT;
      hspi1.Init.CLKPolarity       = SPI_POLARITY_LOW;
      hspi1.Init.CLKPhase          = SPI_PHASE_1EDGE;
      hspi1.Init.NSS               = SPI_NSS_SOFT;
      hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
      hspi1.Init.FirstBit          = SPI_FIRSTBIT_MSB;
      hspi1.Init.TIMode            = SPI_TIMODE_DISABLE;
      hspi1.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
      hspi1.Init.CRCPolynomial     = 7;
      HAL_SPI_Init(&hspi1);

      __HAL_SPI_ENABLE(&hspi1);
      retVal = TRUE;
   }

   return retVal;
}


bool_t spiRead( spiMap_t spi, uint8_t* buffer, uint32_t bufferSize ){

   bool_t retVal = TRUE;

   if( spi == SPI_1 && HAL_SPI_Receive(&hspi1, buffer, bufferSize, 10) == HAL_OK ) {
      retVal = TRUE;
   } else {
      retVal = FALSE;
   }

   return retVal;
}


bool_t spiWrite( spiMap_t spi, uint8_t* buffer, uint32_t bufferSize){

   bool_t retVal = TRUE;

   if( spi == SPI_1 && HAL_SPI_Transmit(&hspi1, buffer, bufferSize, 10) == HAL_OK ) {
      retVal = TRUE;
   } else {
      retVal = FALSE;
   }

   return retVal;
}

bool_t spiWriteRead( spiMap_t spi, uint8_t *txBuffer, uint8_t *rxBuffer, uint32_t bufferSize ) {

   bool_t retVal = TRUE;

   if( spi == SPI_1 && HAL_SPI_TransmitReceive(&hspi1, txBuffer, rxBuffer, bufferSize, 10) == HAL_OK ) {
      retVal = TRUE;
   } else {
      retVal = FALSE;
   }


   return retVal;
}


/*==================[ISR external functions definition]======================*/

/*==================[end of file]============================================*/
