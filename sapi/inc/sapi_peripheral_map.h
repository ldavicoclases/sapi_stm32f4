/* Copyright 2018, Nahuel Espinosa.
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

#ifndef _SAPI_PERIPHERALMAP_H_
#define _SAPI_PERIPHERALMAP_H_

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/* ------- Begin Peripheral Map ------ */

/* Defined for sapi_gpio.h */
typedef enum{
   ENC_A = 0, ENC_B = 1,

   GPIO1 = 2, GPIO2 = 3, GPIO3 = 4, GPIO4 = 5, GPIO5 = 6,

   LCD_BL = 7, LCD_EN = 8, LCD_RW = 9, LCD_RS = 10,
   LCD4 = 11, LCD5 = 12, LCD6 = 13, LCD7 = 14,

   SEG_G = 11, SEG_F = 12, SEG_A = 13, SEG_B = 14,
   SEG_C = 15, SEG_D = 16, SEG_E = 17, SEG_EN = 18,

   LEDR = 19, LEDG = 20, LEDB = 21, LED3 = 22, LED4 = 23, LED5 = 24, LED6 = 25,

   TEC1 = 26, TEC2 = 27, TEC3 = 28, TEC4 = 29, TEC5 = 30, TEC_ENC = 31,

   T_FIL1 = 26, T_FIL2 = 27, T_FIL3 = 28, T_FIL4 = 29,
   T_COL1 = 32, T_COL2 = 33, T_COL3 = 34, T_COL4 = 35,

   DOUT1 = 36, DOUT2 = 37, DOUT_PSW = 38, DOUT_SPK = 39,

   UART3_TX = 40, UART3_RX = 41,
   UART4_TX = 42, UART4_RX = 43,

   WIFI_RST = 44,

   SPI1_SCK = 45, SPI1_MOSI = 46, SPI1_MISO = 47,

   ACC_SCK = 45, ACC_MOSI = 46, ACC_MISO = 47,
   ACC_INT1 = 48, ACC_INT2 = 49, ACC_CS = 50,

   SD_CMD = 51, SD_CK = 52, SD_CS = 53, SD_CD = 54, SD_D0 = 55,

   BOOT1 = 56,

   OTG_FS_PowerSwitchOn = 57, OTG_FS_OverCurrent = 58,
   OTG_VBUS = 59, OTG_FS_ID = 60, OTG_FS_DM = 61, OTG_FS_DP = 62

} gpioMap_t;

/* Defined for sapi_adc.h */
typedef enum{
   AIN_PSW = 63, AIN_POT = 64, AIN1 = 65, AIN2 = 66
} adcMap_t;

/* Defined for sapi_dac.h */
typedef enum{
   AOUT = 67
} dacMap_t;

/* Defined for sapi_uart.h */
typedef enum{
   //UART_0,  // Hardware UART0 not routed
   //UART_1,  // Hardware UART1 not routed
   //UART_2,  // Hardware UART2 not routed
   UART_3,    // Hardware UART3
   UART_4,    // Hardware UART4
   UART_USB   // Hardware UART via USB
} uartMap_t;

/*Defined for sapi_timer.h*/
typedef enum{
   TIMER0, TIMER1, TIMER2, TIMER3, TIMER4, TIMER5, TIMER6, TIMER7, TIMER8, TIMER9, TIMER10, TIMER11, TIMER12
} timerMap_t;

/*Defined for sapi_pwm.h*/
typedef enum{
   PWM_LEDR, PWM_LEDG, PWM_LEDB, PWM1, PWM2, PWM_PSW
} pwmMap_t;

/*Defined for sapi_spi.h*/
typedef enum{
   SPI_ACC = 0, SPI_1 = 0
} spiMap_t;

/* ------- End Peripheral Map -------- */

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* #ifndef _SAPI_PERIPHERALMAP_H_ */
