/* Copyright 2018, Nahuel Espinosa.
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

/* Date: 2018-01-02 */

/*==================[inclusions]=============================================*/

#include "sapi_lis3dsh.h"
#include "sapi_spi.h"
#include "sapi_gpio.h"
#include "sapi_delay.h"

/*==================[macros and definitions]=================================*/

/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

static bool_t lis3dshWriteReg(uint8_t* buffer, uint8_t addr, uint16_t bufferSize);
static bool_t lis3dshReadReg (uint8_t* buffer, uint8_t addr, uint16_t bufferSize);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static bool_t lis3dshWriteReg(uint8_t* buffer, uint8_t addr, uint16_t bufferSize) {
   uint8_t byte = DUMMY_BYTE;
   bool_t retVal = FALSE;

   /* Set chip select Low at the start of the transmission */
   gpioWrite(ACC_CS, LOW);
   delay(1);

   /* Send the Address of the indexed register */
   spiWriteRead( SPI_ACC, &addr, &byte, 1);

   /* Send the data that will be written into the device (MSB First) */
   while(bufferSize > 0) {
      spiWriteRead( SPI_ACC, buffer, &byte, 1);
      bufferSize--;
      buffer++;
   }

   /* Set chip select High at the end of the transmission */
   gpioWrite(ACC_CS, HIGH);

   retVal = TRUE;

   return retVal;
}

static bool_t lis3dshReadReg(uint8_t* buffer, uint8_t addr, uint16_t bufferSize) {
   uint8_t byte = DUMMY_BYTE;
   bool_t retVal = FALSE;

   addr |= (uint8_t)READWRITE_CMD;

   /* Set chip select Low at the start of the transmission */
   gpioWrite(ACC_CS, LOW);
   delay(1);

   /* Send the Address of the indexed register */
   spiWriteRead( SPI_ACC, &addr, &byte, 1);

   /* Receive the data that will be read from the device (MSB First) */
   while( bufferSize > 0 )
   {
     /* Send dummy byte (0x00) to generate the SPI clock to LIS302DL (Slave device) */
     byte = DUMMY_BYTE;
     spiWriteRead( SPI_ACC, &byte, buffer, 1);
     bufferSize--;
     buffer++;
   }

   /* Set chip select High at the end of the transmission */
   gpioWrite(ACC_CS, HIGH);

   retVal = TRUE;

   return retVal;
}

/*==================[external functions definition]==========================*/

bool_t lis3dshConfig( void ) {
   uint8_t ctrl = 0x00;
   uint8_t data = 0x00;
   bool_t retVal = FALSE;

   /* Configure the low level interface -------------------------------------*/

   spiConfig(SPI_ACC);
   gpioWrite(ACC_CS, HIGH);

   gpioConfig(ACC_INT1, GPIO_ALT_FUNCTION);
   gpioConfig(ACC_INT2, GPIO_ALT_FUNCTION);
   gpioConfig(ACC_CS  , GPIO_ALT_FUNCTION);

   lis3dshReadReg(&data, LIS3DSH_WHO_AM_I_ADDR, 1);

   if( data == 0x3F ) {
	   ctrl = 0x00;
      ctrl = LIS3DSH_ODR_100HZ | LIS3DSH_BDU_CONTINUOUS | LIS3DSH_XYZ_ENABLE;
      lis3dshWriteReg(&ctrl, LIS3DSH_CTRL_REG4_ADDR, 1);

      ctrl = 0x00;
      ctrl = LIS3DSH_FSCALE_2G | LIS3DSH_BW_800HZ | LIS3DSH_SIM_4WIRE;
      lis3dshWriteReg(&ctrl, LIS3DSH_CTRL_REG5_ADDR, 1);

      delay(30);

      retVal = TRUE;
   }

   return retVal;
}

bool_t lis3dshRead( lis3dshAxis_t axis, int16_t* out ) {
   bool_t retVal = FALSE;
   uint8_t buffer[2];
   uint8_t addr, ctrl;
   float sensitivity;

   switch( axis ) {
      case LIS3DSH_READ_X:
         addr = LIS3DSH_OUT_X_L_ADDR;
         break;
      case LIS3DSH_READ_Y:
         addr = LIS3DSH_OUT_Y_L_ADDR;
         break;
      case LIS3DSH_READ_Z:
         addr = LIS3DSH_OUT_Z_L_ADDR;
         break;
      default:
         addr = 0x00;
         break;
   }

   lis3dshReadReg(&ctrl, LIS3DSH_CTRL_REG5_ADDR, 1);

   if( (ctrl & (0x7 << 3)) == LIS3DSH_FSCALE_2G ) sensitivity = LIS3DSH_SENSITIVITY_2G;
   if( (ctrl & (0x7 << 3)) == LIS3DSH_FSCALE_4G ) sensitivity = LIS3DSH_SENSITIVITY_4G;
   if( (ctrl & (0x7 << 3)) == LIS3DSH_FSCALE_6G ) sensitivity = LIS3DSH_SENSITIVITY_6G;
   if( (ctrl & (0x7 << 3)) == LIS3DSH_FSCALE_8G ) sensitivity = LIS3DSH_SENSITIVITY_8G;
   if( (ctrl & (0x7 << 3)) == LIS3DSH_FSCALE_16G) sensitivity = LIS3DSH_SENSITIVITY_16G;

   if( addr != 0x00 ) {
      lis3dshReadReg(buffer, addr, 2);
      *out = ((buffer[1] << 8) + buffer[0]);
      *out *= sensitivity;
      retVal = TRUE;
   }

   return retVal;
}

/*==================[ISR external functions definition]======================*/

/*==================[end of file]============================================*/

