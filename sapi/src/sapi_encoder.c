/* Copyright 2018, Nahuel Espinosa.
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

#include "sapi_encoder.h"
#include "sapi_gpio.h"

#include "stm32f4xx_hal.h"

/*==================[macros and definitions]=================================*/

#ifndef EMPTY_POSITION
   #define EMPTY_POSITION 255
#endif

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*
 * @Brief: Initializes the encoder timer.
 * @param  none
 * @return nothing
 */
static void encoderInitTimer( uint16_t limit );

/*==================[internal data definition]===============================*/

TIM_HandleTypeDef htim5;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*
 * @Brief:   Initializes the pwm timer (TIM5).
 * @param    none
 * @return   nothing
 */
static void encoderInitTimer( uint16_t limit ){
   TIM_Encoder_InitTypeDef sConfig;
   TIM_MasterConfigTypeDef sMasterConfig;

   htim5.Instance = TIM5;
   htim5.Init.Prescaler = 0;
   htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
   htim5.Init.Period = limit;
   htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
   sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
   sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
   sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
   sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
   sConfig.IC1Filter = 0;
   sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
   sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
   sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
   sConfig.IC2Filter = 0;
   HAL_TIM_Encoder_Init(&htim5, &sConfig);

   sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
   HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig);
}

/*==================[external functions definition]==========================*/

/*
 * @brief:   read the current value of the encoder
 * @return:   value of the encoder (0 ~ 255).
 *   If an error ocurred, return = EMPTY_POSITION = 255
 */
uint16_t encoderRead(){
   return __HAL_TIM_GET_COUNTER(&htim5);
}

/*
 * @brief:   write value in the encoder counter
 * @param:   uint16 value
 * @return:  void
 */
void encoderWrite( uint16_t value ){
   __HAL_TIM_SET_COUNTER(&htim5, value);
}

/*
 * @Brief: Initializes the encoder peripheral.
 * @param  uint8_t config
 * @param  uint16_t limit
 * @return bool_t true (1) if config it is ok
 */
bool_t encoderConfig( encoderConfig_t config, uint16_t limit ){
   bool_t ret_val = 1;

   switch(config){

      case ENCODER_ENABLE:
         __HAL_RCC_TIM5_CLK_ENABLE();

         gpioConfig(ENC_A, GPIO_ALT_FUNCTION);
         gpioConfig(ENC_B, GPIO_ALT_FUNCTION);

         encoderInitTimer(limit);

         HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
      break;

      case ENCODER_DISABLE:
         __HAL_RCC_TIM5_CLK_DISABLE();

         gpioConfig(ENC_A, GPIO_INPUT);
         gpioConfig(ENC_B, GPIO_INPUT);

         HAL_TIM_Encoder_Stop(&htim5, TIM_CHANNEL_ALL);
      break;

      default:
         ret_val = 0;
      break;
   }

   return ret_val;
}

/*==================[end of file]============================================*/
