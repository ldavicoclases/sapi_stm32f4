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

#include "sapi_gpio.h"

#include "stm32f4xx_hal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

typedef struct{
   GPIO_TypeDef * port;
   uint16_t pin;
} gpioConfigStm32f4_t;

typedef struct{
   gpioConfigStm32f4_t gpio;
                int8_t mode;
                int8_t pull;
                int8_t speed;
                int8_t alternate;
} pinConfigGpioStm32f4_t;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

const pinConfigGpioStm32f4_t gpioPinsConfig[] = {

	/*{ {PORT, PIN}, MODE, PULL, SPEED, ALTERNATE  }*/
   { {GPIOA, GPIO_PIN_0 }, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF2_TIM5   },     /*   0   ENC_A                */
   { {GPIOA, GPIO_PIN_1 }, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF2_TIM5   },     /*   1   ENC_B                */

   { {GPIOC, GPIO_PIN_5 }, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   2   GPIO1                */
   { {GPIOD, GPIO_PIN_10}, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   3   GPIO2                */
   { {GPIOD, GPIO_PIN_11}, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   4   GPIO3                */
   { {GPIOD, GPIO_PIN_7 }, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   5   GPIO4                */
   { {GPIOB, GPIO_PIN_7 }, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   6   GPIO5                */

   { {GPIOB, GPIO_PIN_1 }, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   7   LCD_BL               */
   { {GPIOE, GPIO_PIN_15}, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   8   LCD_EN               */
   { {GPIOB, GPIO_PIN_11}, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   9   LCD_RW               */
   { {GPIOB, GPIO_PIN_13}, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   10  LCD_RS               */
   { {GPIOE, GPIO_PIN_13}, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   11  LCD4 / SEG_G         */
   { {GPIOE, GPIO_PIN_11}, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   12  LCD5 / SEG_F         */
   { {GPIOE, GPIO_PIN_9 }, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   13  LCD6 / SEG_A         */
   { {GPIOE, GPIO_PIN_7 }, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   14  LCD7 / SEG_B         */

   { {GPIOE, GPIO_PIN_8 }, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   15  SEG_C                */
   { {GPIOE, GPIO_PIN_12}, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   16  SEG_D                */
   { {GPIOE, GPIO_PIN_10}, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   17  SEG_E                */
   { {GPIOE, GPIO_PIN_14}, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   18  SEG_EN               */

   { {GPIOB, GPIO_PIN_4 }, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF2_TIM3   },     /*   19  LEDR                 */
   { {GPIOB, GPIO_PIN_5 }, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF2_TIM3   },     /*   20  LEDG                 */
   { {GPIOB, GPIO_PIN_0 }, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF2_TIM3   },     /*   21  LEDB                 */
   { {GPIOD, GPIO_PIN_13}, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   22  LED3                 */
   { {GPIOD, GPIO_PIN_12}, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   23  LED4                 */
   { {GPIOD, GPIO_PIN_14}, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   24  LED5                 */
   { {GPIOD, GPIO_PIN_15}, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   25  LED6                 */

   { {GPIOD, GPIO_PIN_1 }, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   26  TEC1 / T_FIL1        */
   { {GPIOD, GPIO_PIN_3 }, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   27  TEC2 / T_FIL2        */
   { {GPIOB, GPIO_PIN_8 }, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   28  TEC3 / T_FIL3        */
   { {GPIOE, GPIO_PIN_2 }, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   29  TEC4 / T_FIL4        */
   { {GPIOD, GPIO_PIN_6 }, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   30  TEC5                 */
   { {GPIOB, GPIO_PIN_12}, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   31  TEC_ENC              */

   { {GPIOE, GPIO_PIN_4 }, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   32  T_COL1               */
   { {GPIOE, GPIO_PIN_6 }, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   33  T_COL2               */
   { {GPIOC, GPIO_PIN_13}, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   34  T_COL3               */
   { {GPIOE, GPIO_PIN_5 }, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   35  T_COL4               */

   { {GPIOB, GPIO_PIN_14}, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF9_TIM12  },     /*   36  DOUT1                */
   { {GPIOB, GPIO_PIN_15}, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF9_TIM12  },     /*   37  DOUT2                */
   { {GPIOA, GPIO_PIN_15}, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF1_TIM2   },     /*   38  DOUT_PSW             */
   { {GPIOA, GPIO_PIN_8 }, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF1_TIM1   },     /*   39  DOUT_SPK             */

   { {GPIOD, GPIO_PIN_8 }, GPIO_MODE_AF_PP    , GPIO_PULLUP  , GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART3 },     /*   40  UART3_TX             */
   { {GPIOD, GPIO_PIN_9 }, GPIO_MODE_AF_PP    , GPIO_PULLUP  , GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART3 },     /*   41  UART3_RX             */

   { {GPIOC, GPIO_PIN_10}, GPIO_MODE_AF_PP    , GPIO_PULLUP  , GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF8_UART4  },     /*   42  UART4_TX             */
   { {GPIOC, GPIO_PIN_11}, GPIO_MODE_AF_PP    , GPIO_PULLUP  , GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF8_UART4  },     /*   43  UART4_RX             */

   { {GPIOD, GPIO_PIN_0 }, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   44  WIFI_RST             */

   { {GPIOA, GPIO_PIN_5 }, GPIO_MODE_AF_PP    , GPIO_PULLDOWN, GPIO_SPEED_FREQ_HIGH     , GPIO_AF5_SPI1   },     /*   45  SPI1_SKC  / ACC_SCK  */
   { {GPIOA, GPIO_PIN_7 }, GPIO_MODE_AF_PP    , GPIO_PULLDOWN, GPIO_SPEED_FREQ_HIGH     , GPIO_AF5_SPI1   },     /*   46  SPI1_MOSI / ACC_MOSI */
   { {GPIOA, GPIO_PIN_6 }, GPIO_MODE_AF_PP    , GPIO_PULLDOWN, GPIO_SPEED_FREQ_HIGH     , GPIO_AF5_SPI1   },     /*   47  SPI1_MISO / ACC_MISO */
   { {GPIOE, GPIO_PIN_0 }, GPIO_MODE_INPUT    , GPIO_PULLDOWN, GPIO_SPEED_FREQ_HIGH     , 0               },     /*   48  ACC_INT1             */
   { {GPIOE, GPIO_PIN_1 }, GPIO_MODE_INPUT    , GPIO_PULLDOWN, GPIO_SPEED_FREQ_HIGH     , 0               },     /*   49  ACC_INT2             */
   { {GPIOE, GPIO_PIN_3 }, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_FREQ_HIGH     , 0               },     /*   50  ACC_CS               */

   { {GPIOD, GPIO_PIN_2 }, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF12_SDIO  },     /*   51  SD_CMD               */
   { {GPIOC, GPIO_PIN_12}, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF12_SDIO  },     /*   52  SD_CK                */
   { {GPIOC, GPIO_PIN_9 }, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   53  SD_CS                */
   { {GPIOC, GPIO_PIN_6 }, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   54  SD_CD                */
   { {GPIOC, GPIO_PIN_8 }, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF12_SDIO  },     /*   55  SD_D0                */

   { {GPIOB, GPIO_PIN_2 }, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   56  BOOT1                */

   { {GPIOC, GPIO_PIN_0 }, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   57  OTG_FS_PowerSwitchOn */
   { {GPIOD, GPIO_PIN_5 }, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   58  OTG_FS_OverCurrent   */
   { {GPIOA, GPIO_PIN_9 }, GPIO_MODE_INPUT    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   59  OTG_VBUS             */
   { {GPIOA, GPIO_PIN_10}, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF10_OTG_FS},     /*   60  OTG_FS_ID            */
   { {GPIOA, GPIO_PIN_11}, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF10_OTG_FS},     /*   61  OTG_FS_DM            */
   { {GPIOA, GPIO_PIN_12}, GPIO_MODE_AF_PP    , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , GPIO_AF10_OTG_FS},     /*   62  OTG_FS_DP            */

   { {GPIOC, GPIO_PIN_1 }, GPIO_MODE_ANALOG   , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   63  AIN_PSW              */
   { {GPIOC, GPIO_PIN_2 }, GPIO_MODE_ANALOG   , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   64  AIN_POT              */
   { {GPIOA, GPIO_PIN_3 }, GPIO_MODE_ANALOG   , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   65  AIN1                 */
   { {GPIOA, GPIO_PIN_2 }, GPIO_MODE_ANALOG   , GPIO_NOPULL  , GPIO_SPEED_FREQ_LOW      , 0               },     /*   66  AIN2                 */

   { {GPIOA, GPIO_PIN_4 }, GPIO_MODE_ANALOG   , GPIO_NOPULL  , GPIO_SPEED_FREQ_HIGH     , 0               },     /*   67  AOUT                 */
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

bool_t gpioConfig( gpioMap_t pin, gpioConfig_t config ){
   GPIO_InitTypeDef GPIO_InitStruct;

   bool_t ret_val     = 1;

   GPIO_TypeDef * gpioPort = gpioPinsConfig[pin].gpio.port;
   uint16_t gpioPin        = gpioPinsConfig[pin].gpio.pin;
   int8_t mode             = gpioPinsConfig[pin].mode;
   int8_t pull             = gpioPinsConfig[pin].pull;
   int8_t speed            = gpioPinsConfig[pin].speed;
   int8_t alternate        = gpioPinsConfig[pin].alternate;

   switch(config){
      case GPIO_INPUT:
         GPIO_InitStruct.Pin = gpioPin;
         GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
         GPIO_InitStruct.Pull = GPIO_NOPULL;
         GPIO_InitStruct.Alternate = 0;
         HAL_GPIO_Init( gpioPort, &GPIO_InitStruct);
      break;

      case GPIO_INPUT_PULLUP:
         GPIO_InitStruct.Pin = gpioPin;
         GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
         GPIO_InitStruct.Pull = GPIO_PULLUP;
         GPIO_InitStruct.Alternate = 0;
         HAL_GPIO_Init( gpioPort, &GPIO_InitStruct);
      break;

      case GPIO_INPUT_PULLDOWN:
         GPIO_InitStruct.Pin = gpioPin;
         GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
         GPIO_InitStruct.Pull = GPIO_PULLDOWN;
         GPIO_InitStruct.Alternate = 0;
         HAL_GPIO_Init( gpioPort, &GPIO_InitStruct);
      break;

      case GPIO_INPUT_PULLUP_PULLDOWN:
         GPIO_InitStruct.Pin = gpioPin;
         GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
         GPIO_InitStruct.Pull = GPIO_PULLDOWN | GPIO_PULLUP;
         GPIO_InitStruct.Alternate = 0;
         HAL_GPIO_Init( gpioPort, &GPIO_InitStruct);
      break;

      case GPIO_OUTPUT:
         GPIO_InitStruct.Pin = gpioPin;
         GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
         GPIO_InitStruct.Pull = GPIO_NOPULL;
         GPIO_InitStruct.Alternate = 0;
         HAL_GPIO_Init( gpioPort, &GPIO_InitStruct);
      break;

      case GPIO_ALT_FUNCTION:
         GPIO_InitStruct.Pin = gpioPin;
         GPIO_InitStruct.Mode = mode;
         GPIO_InitStruct.Pull = pull;
         GPIO_InitStruct.Speed = speed;
         GPIO_InitStruct.Alternate = alternate;
         HAL_GPIO_Init( gpioPort, &GPIO_InitStruct);
	  break;

      default:
         ret_val = 0;
      break;
   }

   return ret_val;

}


bool_t gpioWrite( gpioMap_t pin, bool_t value ){

   GPIO_TypeDef * gpioPort = gpioPinsConfig[pin].gpio.port;
   uint16_t gpioPin        = gpioPinsConfig[pin].gpio.pin;

   HAL_GPIO_WritePin(gpioPort, gpioPin, value);

   return TRUE;
}


bool_t gpioToggle( gpioMap_t pin ){

   GPIO_TypeDef * gpioPort = gpioPinsConfig[pin].gpio.port;
   uint16_t gpioPin        = gpioPinsConfig[pin].gpio.pin;

   HAL_GPIO_TogglePin(gpioPort, gpioPin);

   return TRUE;
}


bool_t gpioRead( gpioMap_t pin ){

   GPIO_TypeDef * gpioPort = gpioPinsConfig[pin].gpio.port;
   uint16_t gpioPin        = gpioPinsConfig[pin].gpio.pin;

   return HAL_GPIO_ReadPin(gpioPort, gpioPin);
}

/*==================[end of file]============================================*/
