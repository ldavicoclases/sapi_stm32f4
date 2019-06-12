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

#include "sapi_rtc.h"

#include "stm32f4xx_hal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

RTC_HandleTypeDef hrtc;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*
 * @Brief: Configure RTC peripheral.
 * @param  rtc_t rtc: RTC structure
 * @return bool_t true (1) if config it is ok
 */
bool_t rtcConfig(){

   bool_t ret_val = 1;

   static bool_t init = 0;

   if( init ){
      /* Already initialized */
      ret_val = 0;
   } else {
      __HAL_RCC_RTC_ENABLE();

      hrtc.Instance = RTC;
      hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
      hrtc.Init.AsynchPrediv = 127;
      hrtc.Init.SynchPrediv = 255;
      hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
      hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
      hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

      HAL_RTC_Init(&hrtc);

      init = 1;
   }

   return ret_val;
}

/*
 * @Brief: Get time from RTC peripheral.
 * @param  rtc_t rtc: RTC structure
 * @return bool_t true (1) if config it is ok
 */
bool_t rtcRead( rtc_t * rtc ){

   bool_t ret_val = 1;

   RTC_TimeTypeDef rtcTime;
   RTC_DateTypeDef rtcDate;

   HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
   HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

   rtc->sec   = rtcTime.Seconds;
   rtc->min   = rtcTime.Minutes;
   rtc->hour  = rtcTime.Hours;
   rtc->wday  = rtcDate.WeekDay;
   rtc->mday  = rtcDate.Date;
   rtc->month = rtcDate.Month;
   rtc->year  = rtcDate.Year;

   return ret_val;
}

/*
 * @Brief: Set time on RTC peripheral.
 * @param  rtc_t rtc: RTC structure
 * @return bool_t true (1) if config it is ok
 */
bool_t rtcWrite( rtc_t * rtc ){

   bool_t ret_val = 1;

   RTC_TimeTypeDef rtcTime;
   RTC_DateTypeDef rtcDate;

   rtcTime.Seconds = rtc->sec;
   rtcTime.Minutes = rtc->min;
   rtcTime.Hours   = rtc->hour;
   rtcDate.WeekDay = rtc->wday;
   rtcDate.Date    = rtc->mday;
   rtcDate.Month   = rtc->month;
   rtcDate.Year    = rtc->year;

   HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
   HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

   return ret_val;
}

/*==================[end of file]============================================*/
