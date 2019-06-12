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

#include "sapi_keypad.h"       /* <= own header */

#include "sapi_delay.h"               /* <= delay header */
#include "sapi_gpio.h"                /* <= GPIO header */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static gpioMap_t defaultKeypadRowPins[] = { T_FIL1, T_FIL2, T_FIL3, T_FIL4 };
static gpioMap_t defaultKeypadColPins[] = { T_COL1, T_COL2, T_COL3, T_COL4 };

static uint8_t defaultCharMap[] = "D#0*C987B654A321";

static keypad_t defaultKeypad = {
   defaultKeypadRowPins, 4,
   defaultKeypadColPins, 4,
   defaultCharMap
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


/* Configure keypad pins */
bool_t keypadConfig( keypad_t* keypad ){

   bool_t retVal = TRUE;

   uint8_t i = 0;

   if( keypad == KEYPAD_DEFAULT ) {
      keypad = &defaultKeypad;
   } else if( keypad->rowPins == 0 || keypad->colPins == 0 ||
         keypad->rowSize <= 0 || keypad->colSize <= 0  ) {
      retVal = FALSE;
      keypad->colSize = 0;
      keypad->rowSize = 0;
   }

   // Configure Cols as Outputs
   for( i=0; i<keypad->colSize; i++ ){
      gpioConfig( keypad->colPins[i], GPIO_OUTPUT );
   }

   // Configure Rows as Inputs
   for( i=0; i<keypad->rowSize; i++ ){
      gpioConfig( keypad->rowPins[i], GPIO_INPUT );
   }

   return retVal;
}


/* Return TRUE if any key is pressed or FALSE (0) in other cases.
 * If exist key pressed write pressed key on key variable */
bool_t keypadRead( keypad_t* keypad, uint8_t* key ){

   bool_t retVal = FALSE;

   uint8_t r = 0; // Rows
   uint8_t c = 0; // Columns

   if( keypad == KEYPAD_DEFAULT ) {
      keypad = &defaultKeypad;
   }

   // Put all Cols in LOW state
   for( c=0; c<keypad->colSize; c++ ){
      gpioWrite( keypad->colPins[c], LOW );
   }

   // Check all Rows to search if any key is pressed
   for( r=0; r<keypad->rowSize; r++ ){

      // If reads a LOW state in a row then that key may be pressed
      if( !gpioRead( keypad->rowPins[r] ) ){

         delay( 80 ); // Debounce 80 ms

         // Put all Columns in HIGH state except first one
         for( c=1; c<keypad->colSize; c++ ){
            gpioWrite( keypad->colPins[c], HIGH );
         }

         // Search what key are pressed
         for( c=0; c<keypad->colSize; c++ ){

            // Put the Col[r-1] in HIGH state and the Col[r] in LOW state
            if( c>0 ){ // Prevents negative index in array
               gpioWrite( keypad->colPins[c-1], HIGH );
            }
            gpioWrite( keypad->colPins[c], LOW );

            // Check Row[c] at Col[r] to search if the key is pressed
            // if that key is pressed (LOW state) then retuns the key
            if( !gpioRead( keypad->rowPins[r] ) ){
               *key = keypad->charMap[c * (keypad->rowSize) + r];
               retVal = TRUE;
               return retVal;
            }
         }

      }
   }

   /*
      4 rows * 5 columns Keypad

         c0 c1 c2 c3 c4
      r0  0  1  2  3  4
      r1  5  6  7  8  9    Press r[i] c[j] => (i) * amountOfColumns + (j)
      r2 10 11 12 13 14
      r3 15 16 17 18 19
   */

   // if no key are pressed then retun FALSE
   return retVal;
}

/*==================[end of file]============================================*/
