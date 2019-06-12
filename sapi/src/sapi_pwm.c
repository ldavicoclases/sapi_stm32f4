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

#include "sapi_pwm.h"
#include "sapi_gpio.h"
#include "sapi_delay.h"

#include "stm32f4xx_hal.h"

/*==================[macros and definitions]=================================*/

#ifndef EMPTY_POSITION
   #define EMPTY_POSITION 255
#endif

#define PWM_FREC          1000 /* 1Khz */
#define PWM_PERIOD        1000 /* 1000uS = 1ms*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*
 * @Brief: Initializes the pwm timers.
 * @param  none
 * @return nothing
 */
static void pwmInitTimers(void);

/*==================[internal data definition]===============================*/

typedef struct{
   gpioMap_t gpio;
   TIM_HandleTypeDef *timer;
   uint8_t channel;
} pwmConfigStm32f4_t;


TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim12;

static const pwmConfigStm32f4_t pwmMap[] = {
      {LEDR    , &htim3 , TIM_CHANNEL_1},
      {LEDB    , &htim3 , TIM_CHANNEL_2},
      {LEDG    , &htim3 , TIM_CHANNEL_3},
      {DOUT1   , &htim12, TIM_CHANNEL_1},
      {DOUT2   , &htim12, TIM_CHANNEL_2},
      {DOUT_PSW, &htim2 , TIM_CHANNEL_1},
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/* TIM2 init function */
static void MX_TIM2_Init(void)
{
   TIM_ClockConfigTypeDef sClockSourceConfig;
   TIM_MasterConfigTypeDef sMasterConfig;
   TIM_OC_InitTypeDef sConfigOC;

   htim2.Instance = TIM2;
   htim2.Init.Prescaler = 0;
   htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
   htim2.Init.Period = 1024;
   htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

   __HAL_RCC_TIM2_CLK_ENABLE();

   sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
   HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

   HAL_TIM_PWM_Init(&htim3);

   sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
   HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

   sConfigOC.OCMode = TIM_OCMODE_PWM1;
   sConfigOC.Pulse = 1024;
   sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
   sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
   HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{
   TIM_ClockConfigTypeDef sClockSourceConfig;
   TIM_MasterConfigTypeDef sMasterConfig;
   TIM_OC_InitTypeDef sConfigOC;

   htim3.Instance = TIM3;
   htim3.Init.Prescaler = 0;
   htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
   htim3.Init.Period = 1024;
   htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

   __HAL_RCC_TIM3_CLK_ENABLE();

   sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
   HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

   HAL_TIM_PWM_Init(&htim3);

   sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
   HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

   sConfigOC.OCMode = TIM_OCMODE_PWM1;
   sConfigOC.Pulse = 1024;
   sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
   sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
   HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);
   HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);
   HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
}

/* TIM12 init function */
static void MX_TIM12_Init(void)
{
   TIM_ClockConfigTypeDef sClockSourceConfig;
   TIM_OC_InitTypeDef sConfigOC;

   htim12.Instance = TIM12;
   htim12.Init.Prescaler = 0;
   htim12.Init.CounterMode = TIM_COUNTERMODE_UP;
   htim12.Init.Period = 65535;
   htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

   __HAL_RCC_TIM12_CLK_ENABLE();

   sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
   HAL_TIM_ConfigClockSource(&htim12, &sClockSourceConfig);

   HAL_TIM_PWM_Init(&htim12);

   sConfigOC.OCMode = TIM_OCMODE_PWM1;
   sConfigOC.Pulse = 32000;
   sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
   sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
   HAL_TIM_PWM_ConfigChannel(&htim12, &sConfigOC, TIM_CHANNEL_1);
   HAL_TIM_PWM_ConfigChannel(&htim12, &sConfigOC, TIM_CHANNEL_2);
}

/*
 * @Brief:   Initializes the pwm timers (TIM2, TIM3, TIM12).
 * @param    none
 * @return   nothing
 */
static void pwmInitTimers(void){
   MX_TIM2_Init();
   MX_TIM3_Init();
   MX_TIM12_Init();
}

/*==================[external functions definition]==========================*/

/*
 * @brief:   change the value of the pwm at the selected pin
 * @param:   pwmNumber:   ID of the pwm, from 0 to 10
 * @param:   value:   8bit value, from 0 to 255
 * @return:   True if the value was successfully changed, False if not.
 */
bool_t pwmWrite( pwmMap_t pwmNumber, uint16_t value ){
   bool_t success = 1;

   __HAL_TIM_SET_COMPARE(pwmMap[pwmNumber].timer, pwmMap[pwmNumber].channel, value);

   return success;
}

/*
 * @brief:   read the value of the pwm in the pin
 * @param:   pwmNumber:   ID of the pwm, from 0 to 10
 * @return:   value of the pwm in the pin (0 ~ 255).
 *   If an error ocurred, return = EMPTY_POSITION = 255
 */
uint16_t pwmRead( pwmMap_t pwmNumber ){
   return __HAL_TIM_GET_COMPARE(pwmMap[pwmNumber].timer, pwmMap[pwmNumber].channel);
}

/*
 * @Brief: Initializes the pwm peripheral.
 * @param  uint8_t pwmNumber
 * @param  uint8_t config
 * @return bool_t true (1) if config it is ok
 */
bool_t pwmConfig( pwmMap_t pwmNumber, pwmConfig_t config ){
   bool_t ret_val = 1;

   switch(config){

      case PWM_ENABLE:
         pwmInitTimers();
      break;

      case PWM_DISABLE:
         ret_val = 0;
      break;

      case PWM_ENABLE_OUTPUT:
         gpioConfig(pwmMap[pwmNumber].gpio, GPIO_ALT_FUNCTION);
         HAL_TIM_PWM_Start(pwmMap[pwmNumber].timer, pwmMap[pwmNumber].channel);
      break;

      case PWM_DISABLE_OUTPUT:
         gpioConfig(pwmMap[pwmNumber].gpio, GPIO_OUTPUT);
         HAL_TIM_PWM_Stop(pwmMap[pwmNumber].timer, pwmMap[pwmNumber].channel);
      break;

      default:
         ret_val = 0;
      break;
   }

   return ret_val;
}

/*==================[end of file]============================================*/
