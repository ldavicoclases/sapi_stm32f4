/* Copyright 2015-2016, Eric Pernia.
 * Copyright 2018, Nahuel Espinosa.
 * Copyright 2018, Leonardo Davico.
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

/* Date: 2018-10-01 */

/*==================[inclusions]=============================================*/

#include "sapi_board.h"

#include "sapi_tick.h"
#include "sapi_gpio.h"

#include "stm32f4xx_hal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

void SystemClock_Config(void);

/*==================[external functions definition]==========================*/

/* Set up and initialize board hardware */
void boardConfig(void) {

   // Reset of all peripherals, Initializes the Flash interface and the Systick
   HAL_Init();

   // Configure the system clock
   SystemClock_Config();

   // Read clock settings and update SystemCoreClock variable
   SystemCoreClockUpdate();

   // Inicializar el conteo de Ticks con resolución de 1ms, sin tickHook
   tickConfig( 1, 0 );

   // GPIO Ports Clock Enable
   __HAL_RCC_GPIOA_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();
   __HAL_RCC_GPIOC_CLK_ENABLE();
   __HAL_RCC_GPIOD_CLK_ENABLE();
   __HAL_RCC_GPIOE_CLK_ENABLE();
   __HAL_RCC_GPIOH_CLK_ENABLE();

   // Configuración de pines de entrada para Teclas
   gpioConfig( TEC1, GPIO_INPUT );
   gpioConfig( TEC2, GPIO_INPUT );
   gpioConfig( TEC3, GPIO_INPUT );
   gpioConfig( TEC4, GPIO_INPUT );
   gpioConfig( TEC5, GPIO_INPUT );
   gpioConfig( TEC_ENC, GPIO_INPUT );

   // Configuración de pines de salida para Leds
   gpioConfig( LEDR, GPIO_OUTPUT );
   gpioConfig( LEDG, GPIO_OUTPUT );
   gpioConfig( LEDB, GPIO_OUTPUT );
   gpioConfig( LED3, GPIO_OUTPUT );
   gpioConfig( LED4, GPIO_OUTPUT );
   gpioConfig( LED5, GPIO_OUTPUT );
   gpioConfig( LED6, GPIO_OUTPUT );

   gpioConfig( DOUT1, GPIO_OUTPUT );
   gpioConfig( DOUT2, GPIO_OUTPUT );

   // Configuración de pines de salida para LCD
   gpioConfig( LCD_BL, GPIO_OUTPUT );
   gpioConfig( LCD_EN, GPIO_OUTPUT );

   // Configuración de pin para habilitar disp 7 segs
   gpioConfig( SEG_EN, GPIO_OUTPUT );

   // Configuración de pin para parlante
   gpioConfig( DOUT_SPK, GPIO_OUTPUT );
}

/* System Clock Configuration */
void SystemClock_Config(void)
{
   RCC_OscInitTypeDef RCC_OscInitStruct;
   RCC_ClkInitTypeDef RCC_ClkInitStruct;
   RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage
    */
   __HAL_RCC_PWR_CLK_ENABLE();

   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks
    */
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState = RCC_HSE_ON;
   RCC_OscInitStruct.LSIState = RCC_LSI_ON;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
   RCC_OscInitStruct.PLL.PLLM = 8;
   RCC_OscInitStruct.PLL.PLLN = 336;
   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
   RCC_OscInitStruct.PLL.PLLQ = 7;
   HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /**Initializes the CPU, AHB and APB busses clocks
    */
   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

   HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

   PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
   PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
   HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    /**Configure the Systick interrupt time
    */
   HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
   HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

   /* SysTick_IRQn interrupt configuration */
   HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/*==================[end of file]============================================*/
