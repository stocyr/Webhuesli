/******************************************************************************/
/** \file       Lm75.c
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
 *  \date       August 2013
 *
 *  \remark     Last Modification
 *               \li wht4, August 2013, Created
 *
 ******************************************************************************/
/*
 *  functions  global:
 *              readTempLm75
 *  functions  local:
 *              .
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Lm75.h"
#include "Log.h"

//----- Macros -----------------------------------------------------------------
#define LM75_MAX_BUFFER    ( 64 )

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------

//----- Data -------------------------------------------------------------------

//----- Implementation ---------------------------------------------------------

/*******************************************************************************
 *  function :    readTempLm75
 ******************************************************************************/
/** \brief        Read the temperature of the attached LM75 device
 *                <p>
 *                The LM75 is attached over I2C with the device address
 *                LM75_ADDR. The temperature data output of the LM75 is
 *                available at all times as a 9-bit value via the I2C bus.
 *                See warnings!
 *
 *  \type         global
 *
 *  \param[out]   ps32Temp   Temperature in degree celsius
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                BBB_FILE_IOCTL   Error in device control
 *                BBB_FILE_READ    Failed to read temperature value
 *                </pre>
 *
 *  \warning      Accessing the LM75A continuously without waiting at least one
 *                conversion time between communications will prevent the device
 *                from updating the Temperature Register with a new temperature
 *                conversion result. Consequently, the LM75A should not be
 *                accessed continuously with a wait time of less than 300ms.
 *
 ******************************************************************************/
BBBError readTempLm75(int32_t *ps32Temp) {

    int fd;
    uint8_t u8Buffer[2];
    BBBError error = BBB_SUCCESS;

    if ((fd = open(LM75_DEVICE, O_RDWR)) < 0) {
        ERRORPRINT("Failed to open the bus " LM75_DEVICE);
        error = BBB_FILE_OPEN;

    } else {

        if (ioctl(fd, I2C_SLAVE, LM75_ADDR) < 0) {
            ERRORPRINT("Failed to acquire bus access " LM75_DEVICE);
            error = BBB_FILE_IOCTL;
        } else {

            /* Set read pointer to address 0x00 */
            write(fd, (char) 0x00, 1);

            /* Temperature data is represented by a 9-bit, two's complement   */
            /* word with an LSB (Least Significant Bit) equal to 0.5Grad      */
            /* Celcius:                                                       */
            /* +125Grad       0 1111 1010             0FAh                    */
            /* +25Grad        0 0011 0010             032h                    */
            /* +0.5Grad       0 0000 0001             001h                    */
            /* 0Grad          0 0000 0000             000h                    */
            /* −0.5Grad       1 1111 1111             1FFh                    */
            /* −25Grad        1 1100 1110             1CEh                    */
            /* −55Grad        1 1001 0010             192h                    */
            /* The first data byte is the most significant byte with most     */
            /* significant bit first                                          */
            if(read(fd, u8Buffer, 2) == 2) {

                *ps32Temp = 0;
                if((u8Buffer[0] & 0x80) > 0) {
                    *ps32Temp = 0xffffff00;
                }
                *ps32Temp |= (u8Buffer[0] & 0x7f) << 1;
                *ps32Temp |= ((u8Buffer[1] >> 7) & 1);
                /* Currently we are not interested in half degrees */
                *ps32Temp = *ps32Temp >> 1;

            } else {
                ERRORPRINT("Failed to read from LM75 at bus " LM75_DEVICE);
                error = BBB_FILE_READ;
            }

            close(fd);
        }
    }

    return(error);
}

