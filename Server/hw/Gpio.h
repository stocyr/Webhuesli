#ifndef GPIO_H_
#define GPIO_H_
/******************************************************************************/
/** \file       gpio.h
 *******************************************************************************
 *
 *  \brief      Control of the gpio pins
 *              <p>
 *              This module allows to export/unexport gpio pins, set the
 *              direction and the edge of the gpio and to set/read the value
 *              of a gpio. If the gpio is an input pin, it can be polled on
 *              any edge event.
 *              <p>
 *              Almost entirely based on Software by RidgeRun. See copyright
 *              disclaimer.
 *
 *  \author     wht4
 *
 *  \Copyright
 * Original source from
 * https://www.ridgerun.com/developer/wiki/index.php/Gpio-int-test.c
 * as example code.
 * Modularized by Andrew Montag, 10/30/2012
 *
 * Copyright (c) 2011, RidgeRun
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 * must display the following acknowledgement:
 * This product includes software developed by the RidgeRun.
 * 4. Neither the name of the RidgeRun nor the
 * names of its contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY RIDGERUN ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL RIDGERUN BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/
/*
 *  function    exportGpio
 *              unexportGpio
 *              setGpioDirection
 *              setGpioValue
 *              getGpioValue
 *              setGpioEdge
 *              pollGpio
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <stdint.h>

#include "BBBTypes.h"

//----- Macros -----------------------------------------------------------------
#define POLL_TIMEOUT_INF     ( -1 )

//----- Data types -------------------------------------------------------------

/** Direction of a gpio pin */
typedef enum _eGpioDirection {

    GPIO_DIR_IN  = 0, ///< Define gpio as an input pin
    GPIO_DIR_OUT = 1  ///< Define gpio as an output pin

} eGpioDirection;

/** Value of the corresponding gpio */
typedef enum _eGpioValue {

    GPIO_VALUE_LOW   = 0,  ///< gpio has a high value
    GPIO_VALUE_HIGH  = 1   ///< gpio has a low value

} eGpioValue;

/** Edge of a gpio pin */
typedef enum _eGpioEdge {

    GPIO_EDGE_NONE    = 0,  ///< No edge defined
    GPIO_EDGE_RISING  = 1,  ///< rising edge
    GPIO_EDGE_FALLING = 2,  ///< falling edge
    GPIO_EDGE_BOTH    = 3   ///< falling and rising edge

} eGpioEdge;

//----- Function prototypes ----------------------------------------------------
extern BBBError exportGpio(uint32_t u32Gpio);

extern BBBError unexportGpio(uint32_t u32Gpio);

extern BBBError setGpioDirection(uint32_t u32Gpio, eGpioDirection eDir);

extern BBBError setGpioValue(uint32_t u32Gpio, eGpioValue eValue);

extern BBBError getGpioValue(uint32_t u32Gpio, eGpioValue * peValue);

extern BBBError setGpioEdge(uint32_t u32Gpio, eGpioEdge eEdge);

extern BBBError pollGpio(uint32_t u32Gpio, int32_t s32TimeoutMs);

//----- Data -------------------------------------------------------------------

#endif /* GPIO_H_ */
