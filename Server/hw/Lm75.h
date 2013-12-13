#ifndef LM75_H_
#define LM75_H_
/******************************************************************************/
/** \file       Lm75.h
 *******************************************************************************
 *
 *  \brief      This module allows to read the temperature of an attached lm75
 *              device.
 *              <p>
 *              The LM75A is an industry-standard digital temperature sensor
 *              with an integrated Sigma-Delta analog-to-digital converter and
 *              I2C interface. The LM75A provides 9-bit digital temperature
 *              readings with an accuracy of +/-2°C from -25°C to 100°C and
 *              +/-3°C over -55°C to 125°C. The temperature data output of the
 *              LM75 is available at all times via the I2C bus.
 *
 *  \author     wht4
 *
 ******************************************************************************/
/*
 *  function    readTempLm75
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <stdint.h>

#include "BBBTypes.h"

//----- Macros -----------------------------------------------------------------
#define LM75_DEVICE        "/dev/i2c-1"
#define LM75_ADDR          ( 0x48 )

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
extern BBBError readTempLm75(int32_t *ps32Temp);

//----- Data -------------------------------------------------------------------

#endif /* LM75_H_ */
