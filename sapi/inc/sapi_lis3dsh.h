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

#ifndef _SAPI_LIS3DSH_H_
#define _SAPI_LIS3DSH_H_

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"
#include "sapi_peripheral_map.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_LIS302DL_Exported_Constants
  * @{
  */

/* Uncomment the following line to use the default LIS302DL_TIMEOUT_UserCallback()
   function implemented in stm32f4_discovery_lis302dl.c file.
   LIS302DL_TIMEOUT_UserCallback() function is called whenever a timeout condition
   occure during communication (waiting transmit data register empty flag(TXE)
   or waiting receive data register is not empty flag (RXNE)). */
/* #define USE_DEFAULT_TIMEOUT_CALLBACK */

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define LIS3DSH_FLAG_TIMEOUT         ((uint32_t)0x1000)

/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/

/*******************************************************************************
*  WHO_AM_I Register: Device Identification Register
*  Read only register
*  Default value: 0x3F
*******************************************************************************/
#define LIS3DSH_WHO_AM_I_ADDR                  0x0F

/*******************************************************************************
*  CTRL_REG4 Register: Control Register 4
*******************************************************************************/
#define LIS3DSH_CTRL_REG4_ADDR                 0x20

/*******************************************************************************
*  CTRL_REG1 Register: SM1 Control Register
********************************************************************************/
#define LIS3DSH_CTRL_REG1_ADDR                 0x21

/*******************************************************************************
*  CTRL_REG2 Regsiter: SM2 Control Register
*******************************************************************************/
#define LIS3DSH_CTRL_REG2_ADDR                 0x22

/*******************************************************************************
*  CTRL_REG3 Register: Control Register 3
*******************************************************************************/
#define LIS3DSH_CTRL_REG3_ADDR                 0x23

/*******************************************************************************
*  CTRL_REG5 Register: Control Register 5
*******************************************************************************/
#define LIS3DSH_CTRL_REG5_ADDR                 0x24

/*******************************************************************************
*  CTRL_REG6 Register: Control Register 6
*******************************************************************************/
#define LIS3DSH_CTRL_REG6_ADDR                 0x25

/*******************************************************************************
*  STATUS_REG Register: Status Register
*  Default value: 0x00
*  7 ZYXOR: X, Y and Z axis data overrun.
*           0: no overrun has occurred
*           1: new data has overwritten the previous one before it was read
*  6 ZOR: Z axis data overrun.
*         0: no overrun has occurred
*         1: new data for Z-axis has overwritten the previous one before it was read
*  5 yOR: y axis data overrun.
*         0: no overrun has occurred
*         1: new data for y-axis has overwritten the previous one before it was read
*  4 XOR: X axis data overrun.
*         0: no overrun has occurred
*         1: new data for X-axis has overwritten the previous one before it was read
*  3 ZYXDA: X, Y and Z axis new data available
*           0: a new set of data is not yet available
*           1: a new set of data is available
*  2 ZDA: Z axis new data available.
*         0: a new set of data is not yet available
*         1: a new data for Z axis is available
*  1 YDA: Y axis new data available
*         0: a new set of data is not yet available
*         1: a new data for Y axis is available
*  0 XDA: X axis new data available
*         0: a new set of data is not yet available
*         1: a new data for X axis is available
*******************************************************************************/
#define LIS3DSH_STATUS_REG_ADDR                0x27

/*******************************************************************************
*  OUT_X Register: X-axis output Data
*  Read only register
*  Default value: output
*  7:0 XD7-XD0: X-axis output Data
*******************************************************************************/
#define LIS3DSH_OUT_X_L_ADDR                   0x28
#define LIS3DSH_OUT_X_H_ADDR                   0x29

/*******************************************************************************
*  OUT_Y Register: Y-axis output Data
*  Read only register
*  Default value: output
*  7:0 YD7-YD0: Y-axis output Data
*******************************************************************************/
#define LIS3DSH_OUT_Y_L_ADDR                   0x2A
#define LIS3DSH_OUT_Y_H_ADDR                   0x2B

/*******************************************************************************
*  OUT_Z Register: Z-axis output Data
*  Read only register
*  Default value: output
*  7:0 ZD7-ZD0: Z-axis output Data
*******************************************************************************/
#define LIS3DSH_OUT_Z_L_ADDR                   0x2C
#define LIS3DSH_OUT_Z_H_ADDR                   0x2D

/******************************************************************************/
/**************************** END REGISTER MAPPING  ***************************/
/******************************************************************************/

/** CTRL_REG4 Bits: Output data rate */
#define LIS3DSH_ODR_POWERDOWN                            (0x0 << 4)
#define LIS3DSH_ODR_3_125HZ                              (0x1 << 4)
#define LIS3DSH_ODR_6_25HZ                               (0x2 << 4)
#define LIS3DSH_ODR_12_5HZ                               (0x3 << 4)
#define LIS3DSH_ODR_25HZ                                 (0x4 << 4)
#define LIS3DSH_ODR_50HZ                                 (0x5 << 4)
#define LIS3DSH_ODR_100HZ                                (0x6 << 4)
#define LIS3DSH_ODR_400HZ                                (0x7 << 4)
#define LIS3DSH_ODR_800HZ                                (0x8 << 4)
#define LIS3DSH_ODR_1600HZ                               (0x9 << 4)

/** CTRL_REG4 Bits: Block data update */
#define LIS3DSH_BDU_CONTINUOUS                           (0x0 << 3)
#define LIS3DSH_BDU_UPDATE_ON_READ                       (0x1 << 3)

/** CTRL_REG4 Bits: Axis enable */
#define LIS3DSH_Z_ENABLE                                 (0x4)
#define LIS3DSH_Y_ENABLE                                 (0x2)
#define LIS3DSH_X_ENABLE                                 (0x1)
#define LIS3DSH_XYZ_ENABLE                               (0x7)

/** CTRL_REG5 Bits: Full-scale selection */
#define LIS3DSH_FSCALE_2G                                (0x0 << 3)
#define LIS3DSH_FSCALE_4G                                (0x1 << 3)
#define LIS3DSH_FSCALE_6G                                (0x2 << 3)
#define LIS3DSH_FSCALE_8G                                (0x3 << 3)
#define LIS3DSH_FSCALE_16G                               (0x4 << 3)

/** CTRL_REG5 Bits: Anti-aliasing filter bandwidth */
#define LIS3DSH_BW_800HZ                                 (0x0 << 6)
#define LIS3DSH_BW_200HZ                                 (0x1 << 6)
#define LIS3DSH_BW_400HZ                                 (0x2 << 6)
#define LIS3DSH_BW_50HZ                                  (0x3 << 6)

/** CTRL_REG5 Bits: SPI serial interface mode selection */
#define LIS3DSH_SIM_4WIRE                                (0x0)
#define LIS3DSH_SIM_3WIRE                                (0x1)

#define LIS3DSH_SENSITIVITY_2G                           (0.06)
#define LIS3DSH_SENSITIVITY_4G                           (0.12)
#define LIS3DSH_SENSITIVITY_6G                           (0.18)
#define LIS3DSH_SENSITIVITY_8G                           (0.24)
#define LIS3DSH_SENSITIVITY_16G                          (0.73)

/*==================[typedef]================================================*/

typedef enum {
   LIS3DSH_READ_X,
   LIS3DSH_READ_Y,
   LIS3DSH_READ_Z
} lis3dshAxis_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

bool_t lis3dshConfig( void );
bool_t lis3dshRead( lis3dshAxis_t axis, int16_t* out );

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_LIS302DL_H_ */
