/* Copyright 2016, Eric Pernia.
 * Copyright 2018, Nahuel Espinosa.
 * Copyright 2018, Leonardo Davico.
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

/* Date: 2018-11-09 */

/*==================[inclusions]=============================================*/

#include "sapi_uart.h"
#include "sapi_gpio.h"
#include "sapi_delay.h"

#include "string.h"
#include "sapi_circularBuffer.h"

#include "usbd_def.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

typedef struct{
   UART_HandleTypeDef* uart;
   IRQn_Type           irq;
} uartConfigStm32f4_t;

/*==================[internal data declaration]==============================*/

UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;
USBD_HandleTypeDef hUsbDeviceFS;

extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

circularBufferNew( cdcBuffer, 1, 1000 );

static volatile callBackFuncPtr_t rxIsrCallbackUART3 = 0;
static volatile callBackFuncPtr_t rxIsrCallbackUART4 = 0;

static volatile callBackFuncPtr_t txIsrCallbackUART3 = 0;
static volatile callBackFuncPtr_t txIsrCallbackUART4 = 0;

static const uartConfigStm32f4_t uartMap[] = {
   { &huart3, USART3_IRQn },
   { &huart4, UART4_IRQn  },
   { 0      , 0           }
};

/*==================[internal functions declaration]=========================*/

static void uartProcessIRQ( uartMap_t uart );

/*==================[internal functions definition]==========================*/

static void uartProcessIRQ( uartMap_t uart )
{
   /* UART in mode Receiver -------------------------------------------------*/
   if (__HAL_UART_GET_FLAG(uartMap[uart].uart, UART_FLAG_RXNE) == SET)
   {
     // Execute callback
     if( ( uartMap[uart].uart == &huart3 ) && (rxIsrCallbackUART3 != 0) )
        (*rxIsrCallbackUART3)();

     if( ( uartMap[uart].uart == &huart4 )  && (rxIsrCallbackUART4 != 0) )
        (*rxIsrCallbackUART4)();
     return;
   }

   /* UART in mode Transmitter end --------------------------------------------*/
   if (__HAL_UART_GET_FLAG(uartMap[uart].uart, UART_FLAG_TC) == SET)
   {
      // Execute callback
      if( ( uartMap[uart].uart == &huart3 ) && (txIsrCallbackUART3 != 0) )
         (*txIsrCallbackUART3)();

      if( ( uartMap[uart].uart == &huart4 )  && (txIsrCallbackUART4 != 0) )
         (*txIsrCallbackUART4)();
     return;
   }
}

void CDC_RxCallback(uint8_t *Buf, uint32_t Len) {
   uint8_t i = 0;
   for( i = 0 ; i < Len ; i++ ) {
      circularBufferWrite(&cdcBuffer, &Buf[i]);
   }
}

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

// Check for Receive a given pattern

waitForReceiveStringOrTimeoutState_t waitForReceiveStringOrTimeout(
   uartMap_t uart, waitForReceiveStringOrTimeout_t* instance ){

   uint8_t receiveByte;
   //char receiveBuffer[100];

   switch( instance->state ){

      case UART_RECEIVE_STRING_CONFIG:

         delayConfig( &(instance->delay), instance->timeout );

         instance->stringIndex = 0;

         instance->state = UART_RECEIVE_STRING_RECEIVING;

      break;

      case UART_RECEIVE_STRING_RECEIVING:

         if( uartReadByte( uart, &receiveByte ) ){

            //uartWriteByte( UART_DEBUG, receiveByte ); // TODO: DEBUG
/*            if( (instance->stringIndex) <= 100 ){
               receiveBuffer[instance->stringIndex] = receiveByte;
            }
*/
            if( (instance->string)[(instance->stringIndex)] == receiveByte ){

               (instance->stringIndex)++;

               if( (instance->stringIndex) == (instance->stringSize - 1) ){
                  instance->state = UART_RECEIVE_STRING_RECEIVED_OK;

//                  receiveBuffer[instance->stringIndex] = '\0';

                  //uartWriteString( UART_DEBUG, receiveBuffer ); // TODO: DEBUG
                  //uartWriteString( UART_DEBUG, "\r\n" );        // TODO: DEBUG
               }

            }

         }

         if( delayRead( &(instance->delay) ) ){
            instance->state = UART_RECEIVE_STRING_TIMEOUT;
            //uartWriteString( UART_DEBUG, "\r\n" ); // TODO: DEBUG
         }

      break;

      case UART_RECEIVE_STRING_RECEIVED_OK:
         instance->state = UART_RECEIVE_STRING_CONFIG;
      break;

      case UART_RECEIVE_STRING_TIMEOUT:
         instance->state = UART_RECEIVE_STRING_CONFIG;
      break;

      default:
         instance->state = UART_RECEIVE_STRING_CONFIG;
      break;
   }

   return instance->state;
}

// Recibe bytes hasta que llegue el string patron que se le manda en el
// parametro string, stringSize es la cantidad de caracteres del string.
// Devuelve TRUE cuando recibio la cadena patron, si paso el tiempo timeout
// en milisegundos antes de recibir el patron devuelve FALSE.
// No almacena los datos recibidos!! Simplemente espera a recibir cierto patron.
bool_t waitForReceiveStringOrTimeoutBlocking(
   uartMap_t uart, char* string, uint16_t stringSize, tick_t timeout ){

   bool_t retVal = TRUE; // True if OK

   waitForReceiveStringOrTimeout_t waitText;
   waitForReceiveStringOrTimeoutState_t waitTextState;

   waitTextState = UART_RECEIVE_STRING_CONFIG;

   waitText.state = UART_RECEIVE_STRING_CONFIG;
   waitText.string =  string;
   waitText.stringSize = stringSize;
   waitText.timeout = timeout;

   while( waitTextState != UART_RECEIVE_STRING_RECEIVED_OK &&
          waitTextState != UART_RECEIVE_STRING_TIMEOUT ){
      waitTextState = waitForReceiveStringOrTimeout( uart, &waitText );
   }

   if( waitTextState == UART_RECEIVE_STRING_TIMEOUT ){
      retVal = FALSE;
   }

   return retVal;
}


// Store bytes until receive a given pattern
waitForReceiveStringOrTimeoutState_t receiveBytesUntilReceiveStringOrTimeout(
   uartMap_t uart, waitForReceiveStringOrTimeout_t* instance,
   char* receiveBuffer, uint32_t* receiveBufferSize ){

   uint8_t receiveByte;
   static uint32_t i = 0;
   //uint32_t j = 0;
   //uint32_t savedReceiveBufferSize = *receiveBufferSize;

   switch( instance->state ){

      case UART_RECEIVE_STRING_CONFIG:

         delayConfig( &(instance->delay), instance->timeout );

         instance->stringIndex = 0;
         i = 0;

         instance->state = UART_RECEIVE_STRING_RECEIVING;

      break;

      case UART_RECEIVE_STRING_RECEIVING:

         if( uartReadByte( uart, &receiveByte ) ){

            //uartWriteByte( UART_DEBUG, receiveByte ); // TODO: DEBUG
            if( i < *receiveBufferSize ){
               receiveBuffer[i] = receiveByte;
               i++;
            } else{
               instance->state = UART_RECEIVE_STRING_FULL_BUFFER;
               *receiveBufferSize = i;
               i = 0;
               return instance->state;
            }

            if( (instance->string)[(instance->stringIndex)] == receiveByte ){

               (instance->stringIndex)++;

               if( (instance->stringIndex) == (instance->stringSize - 1) ){
                  instance->state = UART_RECEIVE_STRING_RECEIVED_OK;
                  *receiveBufferSize = i;
                  /*
                  // TODO: For debug purposes
                  for( j=0; j<i; j++ ){
                     uartWriteByte( UART_DEBUG, receiveBuffer[j] );
                  }
                  uartWriteString( UART_DEBUG, "\r\n" );
                  */
                  i = 0;
               }

            }

         }

         if( delayRead( &(instance->delay) ) ){
            instance->state = UART_RECEIVE_STRING_TIMEOUT;
            //uartWriteString( UART_DEBUG, "\r\n" ); // TODO: DEBUG
            *receiveBufferSize = i;
            i = 0;
         }

      break;

      case UART_RECEIVE_STRING_RECEIVED_OK:
         instance->state = UART_RECEIVE_STRING_CONFIG;
      break;

      case UART_RECEIVE_STRING_TIMEOUT:
         instance->state = UART_RECEIVE_STRING_CONFIG;
      break;

      case UART_RECEIVE_STRING_FULL_BUFFER:
         instance->state = UART_RECEIVE_STRING_CONFIG;
      break;

      default:
         instance->state = UART_RECEIVE_STRING_CONFIG;
      break;
   }

   return instance->state;
}

// Guarda todos los bytes que va recibiendo hasta que llegue el string
// patron que se le manda en el parametro string, stringSize es la cantidad
// de caracteres del string.
// receiveBuffer es donde va almacenando los caracteres recibidos y
// receiveBufferSize es el tamaño de buffer receiveBuffer.
// Devuelve TRUE cuando recibio la cadena patron, si paso el tiempo timeout
// en milisegundos antes de recibir el patron devuelve FALSE.
bool_t receiveBytesUntilReceiveStringOrTimeoutBlocking(
   uartMap_t uart, char* string, uint16_t stringSize,
   char* receiveBuffer, uint32_t* receiveBufferSize,
   tick_t timeout ){

   bool_t retVal = TRUE; // True if OK

   waitForReceiveStringOrTimeout_t waitText;
   waitForReceiveStringOrTimeoutState_t waitTextState;

   waitTextState = UART_RECEIVE_STRING_CONFIG;

   waitText.state = UART_RECEIVE_STRING_CONFIG;
   waitText.string =  string;
   waitText.stringSize = stringSize;
   waitText.timeout = timeout;

   while( waitTextState != UART_RECEIVE_STRING_RECEIVED_OK &&
          waitTextState != UART_RECEIVE_STRING_TIMEOUT ){
      waitTextState = receiveBytesUntilReceiveStringOrTimeout(
                         uart, &waitText,
                         receiveBuffer, receiveBufferSize );
   }

   if( waitTextState == UART_RECEIVE_STRING_TIMEOUT ){
      retVal = FALSE;
   }

   return retVal;
}

//-------------------------------------------------------------

// UART RX Interrupt Enable/Disable
void uartRxInterruptSet( uartMap_t uart, bool_t enable ){
   if( enable ){
      __HAL_UART_ENABLE_IT(uartMap[uart].uart, UART_IT_RXNE);

      HAL_NVIC_SetPriority(uartMap[uart].irq, 15, 15);
      NVIC_EnableIRQ( uartMap[uart].irq );
   } else{
      __HAL_UART_DISABLE_IT(uartMap[uart].uart, UART_IT_RXNE);

      NVIC_DisableIRQ( uartMap[uart].irq );
   }
}

// UART TX Interrupt Enable/Disable
void uartTxInterruptSet( uartMap_t uart, bool_t enable ){
   if( enable ){
      __HAL_UART_ENABLE_IT(uartMap[uart].uart, UART_IT_TC);

      HAL_NVIC_SetPriority(uartMap[uart].irq, 15, 15);
      HAL_NVIC_EnableIRQ(uartMap[uart].irq);               // Enable Interrupt
   } else{
      __HAL_UART_DISABLE_IT(uartMap[uart].uart, UART_IT_TC);

      NVIC_DisableIRQ( uartMap[uart].irq );
   }
}

// UART RX Interrupt set callback function that is excecuted when event ocurrs
void uartRxInterruptCallbackSet(
   uartMap_t uart,                  // UART
   callBackFuncPtr_t rxIsrCallback  // pointer to function
){
   if( rxIsrCallback != 0 ){
      // Set callback
      if( uart == UART_3 ) rxIsrCallbackUART3 = rxIsrCallback;
      if( uart == UART_4 ) rxIsrCallbackUART4 = rxIsrCallback;
   }
}

// UART TX Interrupt set callback function that is excecuted when event ocurrs
void uartTxInterruptCallbackSet(
   uartMap_t uart,                  // UART
   callBackFuncPtr_t txIsrCallback  // pointer to function
){
   if( txIsrCallback != 0 ){
      // Set callback
      if( uart == UART_3 ) txIsrCallbackUART3 = txIsrCallback;
      if( uart == UART_4  ) txIsrCallbackUART4 = txIsrCallback;
   }
}

// UART Initialization
void uartConfig( uartMap_t uart, uint32_t baudRate ){
   switch( uart ) {
   case UART_3:
      __HAL_RCC_USART3_CLK_ENABLE();

      gpioConfig(UART3_TX, GPIO_ALT_FUNCTION);
      gpioConfig(UART3_RX, GPIO_ALT_FUNCTION);

      huart3.Instance = USART3;
      huart3.Init.BaudRate = baudRate;
      huart3.Init.WordLength = UART_WORDLENGTH_8B;
      huart3.Init.StopBits = UART_STOPBITS_1;
      huart3.Init.Parity = UART_PARITY_NONE;
      huart3.Init.Mode = UART_MODE_TX_RX;
      huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
      huart3.Init.OverSampling = UART_OVERSAMPLING_16;
      HAL_UART_Init(&huart3);
      break;
   case UART_4:
      __HAL_RCC_UART4_CLK_ENABLE();

      gpioConfig(UART4_TX, GPIO_ALT_FUNCTION);
      gpioConfig(UART4_RX, GPIO_ALT_FUNCTION);

      huart4.Instance = UART4;
      huart4.Init.BaudRate = baudRate;
      huart4.Init.WordLength = UART_WORDLENGTH_8B;
      huart4.Init.StopBits = UART_STOPBITS_1;
      huart4.Init.Parity = UART_PARITY_NONE;
      huart4.Init.Mode = UART_MODE_TX_RX;
      huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
      huart4.Init.OverSampling = UART_OVERSAMPLING_16;
      HAL_UART_Init(&huart4);
      break;
   case UART_USB:
      circularBufferConfig(cdcBuffer, 1, 1000);

      gpioConfig(OTG_FS_PowerSwitchOn, GPIO_ALT_FUNCTION);
      gpioConfig(OTG_FS_OverCurrent  , GPIO_ALT_FUNCTION);

      gpioConfig(OTG_VBUS , GPIO_ALT_FUNCTION);
      gpioConfig(OTG_FS_ID, GPIO_ALT_FUNCTION);
      gpioConfig(OTG_FS_DM, GPIO_ALT_FUNCTION);
      gpioConfig(OTG_FS_DP, GPIO_ALT_FUNCTION);

      /* Peripheral clock enable */
      __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

      /* Peripheral interrupt init */
      HAL_NVIC_SetPriority(OTG_FS_IRQn, 5, 0);
      HAL_NVIC_EnableIRQ(OTG_FS_IRQn);

      /* Init Device Library,Add Supported Class and Start the library*/
      USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

      USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);

      USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);

      USBD_Start(&hUsbDeviceFS);
      break;
   }
}

// Read 1 byte from RX FIFO, check first if exist aviable data
bool_t uartReadByte( uartMap_t uart, uint8_t* receivedByte ){
   bool_t retVal = TRUE;

   if( uart == UART_USB ) {
      if( circularBufferRead( &cdcBuffer, receivedByte ) == CIRCULAR_BUFFER_EMPTY ) {
         retVal = FALSE;
      } else {
         retVal = TRUE;
      }
   } else {
      if( HAL_UART_Receive( uartMap[uart].uart, receivedByte, 1, 1) == HAL_OK ) {
         retVal = TRUE;
      } else {
         retVal = FALSE;
      }
   }

   return retVal;
}

// Blocking Write 1 byte to TX FIFO
void uartWriteByte( uartMap_t uart, uint8_t value ){
   delay_t timeoutDelay;

   delayConfig(&timeoutDelay, 10);

   if( uart == UART_USB ) {
      while( CDC_Transmit_FS(&value, 1) == USBD_BUSY && !delayRead(&timeoutDelay) );
   } else {
      HAL_UART_Transmit(uartMap[uart].uart, &value, 1, 1);
   }
}

// Blocking Send a string
void uartWriteString( uartMap_t uart, char* str ){
   delay_t timeoutDelay;

   delayConfig(&timeoutDelay, 10);

   if( uart == UART_USB ) {
      while( CDC_Transmit_FS((uint8_t*)str, strlen(str)) == USBD_BUSY && !delayRead(&timeoutDelay) );
   } else {
      while( *str != 0 ){
         uartWriteByte( uart, (uint8_t)*str );
         str++;
      }
   }
}

/*==================[ISR external functions definition]======================*/

__attribute__ ((section(".after_vectors")))

void USART3_IRQHandler(void){
   //HAL_UART_IRQHandler(&huart3);
   uartProcessIRQ( UART_3 );
}

void UART4_IRQHandler(void){
   //HAL_UART_IRQHandler(&huart4);
   uartProcessIRQ( UART_4 );
}

void OTG_FS_IRQHandler(void)
{
   HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}

/*==================[end of file]============================================*/
