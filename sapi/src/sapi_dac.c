/* Copyright 2016, Ian Olivieri.
 * Copyright 2016, Eric Pernia.
 * Copyright 2018, Nahuel Espinosa.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
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

#include "sapi_dac.h"
#include "sapi_gpio.h"

#include "stm32f4xx_hal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

DAC_HandleTypeDef hdac;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*
 * @brief:  enable/disable the ADC and DAC peripheral
 * @param:  DAC_ENABLE, DAC_DISABLE
 * @return: none
*/
void dacConfig( dacConfig_t config ){
   DAC_ChannelConfTypeDef sConfig;

   switch(config){

      case DAC_ENABLE:

         __HAL_RCC_DAC_CLK_ENABLE();

         gpioConfig(AOUT, GPIO_ALT_FUNCTION);

         /** DAC Initialization  */
         hdac.Instance = DAC;
         HAL_DAC_Init(&hdac);

         /** DAC channel OUT1 config */
         sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
         sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
         HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);

         HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

      break;

      case DAC_DISABLE:

         __HAL_RCC_DAC_CLK_DISABLE();

         gpioConfig(AOUT, GPIO_OUTPUT);
         HAL_DAC_DeInit(&hdac);

      break;
   }
}


/*
 * @brief   Write a value in the DAC.
 * @param   analogOutput: AO0 ... AOn
 * @param   value: analog value to be writen in the DAC, from 0 to 1023
 * @return  none
 */
void dacWrite( dacMap_t analogOutput, uint16_t value ){
   if( analogOutput == AOUT && HAL_DAC_GetState(&hdac) == HAL_DAC_STATE_READY ) {
      HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
   }
}

/*==================[end of file]============================================*/
