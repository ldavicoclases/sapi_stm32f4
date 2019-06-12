/* Copyright 2016, Nahuel Espinosa.
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

#include "sapi_adc.h"
#include "sapi_gpio.h"

#include "stm32f4xx_hal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

typedef struct{
   gpioMap_t gpio;
   ADC_HandleTypeDef *adc;
   uint8_t channel;
} adcConfigStm32f4_t;

ADC_HandleTypeDef hadc1;

static const adcConfigStm32f4_t adcMap[] = {
      {AIN_PSW , &hadc1 , ADC_CHANNEL_11},
      {AIN_POT , &hadc1 , ADC_CHANNEL_12},
      {AIN1    , &hadc1 , ADC_CHANNEL_3 },
      {AIN2    , &hadc1 , ADC_CHANNEL_2 },
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*
 * @brief:  enable/disable the ADC and DAC peripheral
 * @param:  ADC_ENABLE, ADC_DISABLE
 * @return: none
*/
void adcConfig( adcConfig_t config ){
   uint8_t i = 0;

   switch(config){

      case ADC_ENABLE:
           /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
           */
         hadc1.Instance = ADC1;
         hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
         hadc1.Init.Resolution = ADC_RESOLUTION_12B;
         hadc1.Init.ScanConvMode = DISABLE;
         hadc1.Init.ContinuousConvMode = DISABLE;
         hadc1.Init.DiscontinuousConvMode = DISABLE;
         hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
         hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
         hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
         hadc1.Init.NbrOfConversion = 1;
         hadc1.Init.DMAContinuousRequests = DISABLE;
         hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
         HAL_ADC_Init(&hadc1);

         __HAL_RCC_ADC1_CLK_ENABLE();
           /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
           */
         for( i = 0 ; i < 4 ; i++ ) {
            gpioConfig(adcMap[i].gpio, GPIO_ALT_FUNCTION);
         }
      break;

      case ADC_DISABLE:
         /* Disable ADC peripheral */
         __HAL_RCC_ADC1_CLK_DISABLE();

         for( i = 0 ; i < 4 ; i++ ) {
            gpioConfig(adcMap[i].gpio, GPIO_INPUT);
         }

         __HAL_ADC_DISABLE(&hadc1);
      break;
   }

}


/*
 * @brief   Get the value of one ADC channel. Mode: BLOCKING
 * @param   AI0 ... AIn
 * @return  analog value
 */
uint16_t adcRead( adcMap_t analogInput ){
   ADC_ChannelConfTypeDef sConfig;
   uint8_t stmAdcChannel = analogInput - AIN_PSW;
   uint16_t analogValue = 0;

   sConfig.Channel = adcMap[stmAdcChannel].channel;
   sConfig.Rank = 1;
   sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
   HAL_ADC_ConfigChannel(&hadc1, &sConfig);

   HAL_ADC_Start( adcMap[stmAdcChannel].adc );

   if( HAL_ADC_PollForConversion( adcMap[stmAdcChannel].adc, 200 ) == HAL_OK ) {
      analogValue = HAL_ADC_GetValue( adcMap[stmAdcChannel].adc );
   }

   HAL_ADC_Stop( adcMap[stmAdcChannel].adc );

   return analogValue;
}

/*==================[end of file]============================================*/
