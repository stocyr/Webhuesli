/******************************************************************************/
/** \file       gpio.c
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
 *  \date       August 2013
 *
 *  \remark     Last Modification
 *               \li wht4, August 2013, Created
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
 *  functions  global:
 *              exportGpio
 *              unexportGpio
 *              setGpioDirection
 *              setGpioValue
 *              getGpioValue
 *              setGpioEdge
 *              pollGpio
 *  functions  local:
 *              openFdGpio
 *              closeFdGpio
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>

#include "Gpio.h"
#include "Log.h"

//----- Macros -----------------------------------------------------------------
#define GPIO_SYSFS_DIR        "/sys/class/gpio"
#define GPIO_MAX_BUF          ( 64 )

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
static BBBError openFdGpio(uint32_t u32Gpio, int *fd);

static BBBError closeFdGpio(int fd);

//----- Data -------------------------------------------------------------------
static const char * pcDirIn       = "in";
static const char * pcDirOut      = "out";
static const char * pcEdgeNone    = "none";
static const char * pcEdgeRising  = "rising";
static const char * pcEdgeFalling = "falling";
static const char * pcEdgeBoth    = "both";

//----- Implementation ---------------------------------------------------------

/*******************************************************************************
 *  function :    exportGpio
 ******************************************************************************/
/** \brief        Export a gpio pin
 *                <p>
 *                Gpio pin will be exported. This will result in a new folder
 *                in /sys/class/gpio/gpio#
 *
 *  \type         global
 *
 *  \param[in]    u32Gpio    gpio pin which shall be exported
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                </pre>
 *
 ******************************************************************************/
BBBError exportGpio(uint32_t u32Gpio) {

    int      fd = -1;
    int      len = 0;
    char     cBuf[GPIO_MAX_BUF];
    BBBError error = BBB_SUCCESS;

    fd = open(GPIO_SYSFS_DIR "/export", O_WRONLY);
    if (fd < 0) {
        ERRORPRINT("export of gpio %d failed", u32Gpio);
        error = BBB_FILE_OPEN;
    } else {
        len = snprintf(cBuf, sizeof(cBuf), "%d", u32Gpio);
        write(fd, cBuf, len);
        error = closeFdGpio(fd);
    }

    return (error);
}

/*******************************************************************************
 *  function :    unexportGpio
 ******************************************************************************/
/** \brief        Unexport a gpio pin
 *                <p>
 *                Gpio pin will be exported. Folder in /sys/class/gpio/gpio#
 *                will be removed.
 *
 *  \type         global
 *
 *  \param[in]    u32Gpio    gpio pin which shall be unexported
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                </pre>
 *
 ******************************************************************************/
BBBError unexportGpio(uint32_t u32Gpio) {

    int      fd;
    int      len;
    char     cBuf[GPIO_MAX_BUF];
    BBBError error = BBB_SUCCESS;

    fd = open(GPIO_SYSFS_DIR "/unexport", O_WRONLY);
    if (fd < 0) {
        ERRORPRINT("unexport of gpio %d failed", u32Gpio);
        error = BBB_FILE_OPEN;
    } else {
        len = snprintf(cBuf, sizeof(cBuf), "%d", u32Gpio);
        write(fd, cBuf, len);
        error = closeFdGpio(fd);
    }

    return (error);
}

/*******************************************************************************
 *  function :    setGpioDirection
 ******************************************************************************/
/** \brief        Sets the direction of the corresponding gpio.
 *                <p>
 *                The gpio must first be exported before this function can be
 *                called.
 *
 *  \type         global
 *
 *  \param[in]    u32Gpio    gpio pin where the edge will be set
 *  \param[in]    eDir       GPIO_DIR_IN or GPIO_DIR_OUT
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                BBB_ERR_PARAM    if a parameter error occurred
 *                </pre>
 *
 ******************************************************************************/
BBBError setGpioDirection(uint32_t u32Gpio, eGpioDirection eDir) {

    int      fd;
    char     cBuf[GPIO_MAX_BUF];
    BBBError error = BBB_SUCCESS;

    snprintf(cBuf, sizeof(cBuf), GPIO_SYSFS_DIR  "/gpio%d/direction", u32Gpio);

    fd = open(cBuf, O_WRONLY);
    if (fd < 0) {
        ERRORPRINT("set direction of gpio %d failed", u32Gpio);
        error = BBB_FILE_OPEN;
    } else {

        if(eDir == GPIO_DIR_OUT) {

            write(fd, pcDirOut, strlen(pcDirOut));

        } else if(eDir == GPIO_DIR_IN) {

            write(fd, pcDirIn, strlen(pcDirIn));

        } else {

            error = BBB_ERR_PARAM;
            ERRORPRINT("parameter error");
        }

        error |= closeFdGpio(fd);
    }

    return (error);
}

/*******************************************************************************
 *  function :    setGpioValue
 ******************************************************************************/
/** \brief        Sets the value of the corresponding gpio.
 *                <p>
 *                The gpio must first be exported before this function can be
 *                called.
 *
 *  \type         global
 *
 *  \param[in]    u32Gpio    gpio pin where the edge will be set
 *  \param[in]    eValue     GPIO_VALUE_LOW or GPIO_VALUE_HIGH
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                BBB_ERR_PARAM    if a parameter error occurred
 *                </pre>
 *
 ******************************************************************************/
BBBError setGpioValue(uint32_t u32Gpio, eGpioValue eValue) {

    int      fd;
    char     cBuf[GPIO_MAX_BUF];
    BBBError error = BBB_SUCCESS;

    snprintf(cBuf, sizeof(cBuf), GPIO_SYSFS_DIR "/gpio%d/value", u32Gpio);

    fd = open(cBuf, O_WRONLY);
    if (fd < 0) {
        ERRORPRINT("set value of gpio %d failed", u32Gpio);
        error = BBB_FILE_OPEN;
    } else {

        if(eValue == GPIO_VALUE_LOW) {

            write(fd, "0", 2);

        } else if(eValue == GPIO_VALUE_HIGH) {

            write(fd, "1", 2);

        } else {

            ERRORPRINT("parameter error");
            error = BBB_ERR_PARAM;
        }
        error |= closeFdGpio(fd);
    }

    return (error);
}

/*******************************************************************************
 *  function :    getGpioValue
 ******************************************************************************/
/** \brief        Get the value of the corresponding gpio.
 *                <p>
 *                The gpio must first be exported before this function can be
 *                called.
 *
 *  \type         global
 *
 *  \param[in]    u32Gpio    gpio pin where the edge will be set
 *  \param[out]   peValue    Will be set to GPIO_VALUE_LOW or GPIO_VALUE_HIGH
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                BBB_ERR_UNKNOWN  If neither '1' nor '0' are written in the
 *                                 value file
 *                </pre>
 *
 ******************************************************************************/
BBBError getGpioValue(uint32_t u32Gpio, eGpioValue * peValue) {

    int      fd;
    char     cBuf[GPIO_MAX_BUF];
    char     ch;
    BBBError error = BBB_SUCCESS;

    snprintf(cBuf, sizeof(cBuf), GPIO_SYSFS_DIR "/gpio%d/value", u32Gpio);

    fd = open(cBuf, O_RDONLY);
    if (fd < 0) {
        ERRORPRINT("get value of gpio %d failed", u32Gpio);
        error = BBB_FILE_OPEN;
    } else {

        read(fd, &ch, 1);

        if(ch == '1') {

            *peValue = GPIO_VALUE_HIGH;

        } else if(ch == '0') {

            *peValue = GPIO_VALUE_LOW;

        } else {

            ERRORPRINT("parameter error");
            error = BBB_ERR_UNKNOWN;
            *peValue = GPIO_VALUE_LOW;
        }
        error |= closeFdGpio(fd);
    }

    return (error);
}

/*******************************************************************************
 *  function :    setGpioEdge
 ******************************************************************************/
/** \brief        Sets the edge of the corresponding gpio.
 *                <p>
 *                The gpio must first be exported before this function can be
 *                called.
 *
 *  \type         global
 *
 *  \param[in]    u32Gpio    gpio pin where the edge will be set
 *  \param[in]    eValue     GPIO_EDGE_NONE, GPIO_EDGE_RISING, GPIO_EDGE_FALLING
 *                           or GPIO_EDGE_BOTH
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                BBB_ERR_PARAM    if a parameter error occurred
 *                </pre>
 *
 ******************************************************************************/
BBBError setGpioEdge(uint32_t u32Gpio, eGpioEdge eEdge) {

    int  fd;
    char cBuf[GPIO_MAX_BUF];
    BBBError error = BBB_SUCCESS;

    snprintf(cBuf, sizeof(cBuf), GPIO_SYSFS_DIR "/gpio%d/edge", u32Gpio);

    fd = open(cBuf, O_WRONLY);
    if (fd < 0) {
        ERRORPRINT("set edge of gpio %d failed", u32Gpio);
        error = BBB_FILE_OPEN;
    } else {

        if(eEdge == GPIO_EDGE_NONE) {

            write(fd, pcEdgeNone, strlen(pcEdgeNone));

        } else if(eEdge == GPIO_EDGE_RISING) {

            write(fd, pcEdgeRising, strlen(pcEdgeRising));

        } else if(eEdge == GPIO_EDGE_FALLING) {

            write(fd, pcEdgeFalling, strlen(pcEdgeFalling));

        } else if(eEdge == GPIO_EDGE_BOTH) {

            write(fd, pcEdgeBoth, strlen(pcEdgeBoth));

        } else {

            ERRORPRINT("parameter error");
            error = BBB_ERR_PARAM;
        }
        error |= closeFdGpio(fd);
    }

    return (error);
}

/*******************************************************************************
 *  function :    pollGpio
 ******************************************************************************/
/** \brief        Wait on an event of the corresponding gpio.
 *                <p>
 *                This function will call poll(2) and therefore wait/block until
 *                an event of the corresponding gpio. The gpio must first be
 *                exported before this function can be called.
 *
 *  \type         global
 *
 *  \param[in]    u32Gpio      gpio pin where the edge will be set
 *  \param[in]    s32TimeoutMs Time we wait until an event occurres. Set to
 *                             POLL_TIMEOUT_INF for infinite wait time.
 *
 *  \return       <pre>
 *                BBB_SUCCESS      on success
 *                BBB_FILE_OPEN    File could not be opened
 *                BBB_ERR_PARAM    if a parameter error occurred
 *                BBB_GPIO_POLL    poll(2) failed
 *                </pre>
 *
 ******************************************************************************/
BBBError pollGpio(uint32_t u32Gpio, int32_t s32TimeoutMs) {

    int           rc;
    BBBError      error = BBB_SUCCESS;
    char          cBuf[GPIO_MAX_BUF];
    struct pollfd sFdPoll;

    memset((void*) &sFdPoll, 0, sizeof(sFdPoll));

    if((error = openFdGpio(u32Gpio, &sFdPoll.fd)) == BBB_SUCCESS) {

        sFdPoll.events = POLLPRI;

        /* clear any backed up events */
        poll(&sFdPoll, 1, 0);
        read(sFdPoll.fd, cBuf, GPIO_MAX_BUF);

        sFdPoll.revents = 0;
        rc = poll(&sFdPoll, 1, s32TimeoutMs);
        read(sFdPoll.fd, cBuf, GPIO_MAX_BUF);

        if (rc < 0) {
            ERRORPRINT("poll of gpio %d failed", u32Gpio);
            error = BBB_GPIO_POLL;
        }

        error |= closeFdGpio(sFdPoll.fd);
    }

    return(error);
}

/*******************************************************************************
 *  function :    openFdGpio
 ******************************************************************************/
static BBBError openFdGpio(uint32_t u32Gpio, int *fd) {

    char     cBuf[GPIO_MAX_BUF];
    BBBError error = BBB_SUCCESS;

    snprintf(cBuf, sizeof(cBuf), GPIO_SYSFS_DIR "/gpio%d/value", u32Gpio);

    *fd = open(cBuf, O_RDONLY | O_NONBLOCK);
    if (*fd < 0) {
        ERRORPRINT("open FD of gpio %d failed", u32Gpio);
        error = BBB_FILE_OPEN;
    }

    return (error);
}

/*******************************************************************************
 *  function :    closeFdGpio
 ******************************************************************************/
static BBBError closeFdGpio(int fd) {

    BBBError error = BBB_SUCCESS;

    if (close(fd) < 0) {
        ERRORPRINT("close FD failed");
        error = BBB_FILE_CLOSE;
    }

    return (error);
}

